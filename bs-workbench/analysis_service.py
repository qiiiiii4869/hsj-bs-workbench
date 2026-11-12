"""Analysis services shared by the browser API and future agent tools."""
from dataclasses import dataclass
import csv
import io
import json
import math
from pathlib import Path
import threading
import uuid
from datetime import datetime,timezone,timedelta

from analysis import analyze
from sel_reader import SelError, sha256
from data_reader import open_reader as SelFile
from interaction import register, read_chart, annotation_records
from overlays import read_overlays
from moving_trend import create_trend
from throwload import compute_throw
from outputs import plot_image,image_bytes,convert_image,excel_report,word_report,legacy_data
from export_jobs import ExportJobs
from strip_chart import prepare_strip
from waterfall import prepare_waterfall

ROOT = Path(__file__).resolve().parent
WORKSPACE = ROOT.parent
DATA = ROOT / 'data'
STATIC = ROOT / 'static'
COMPUTE = threading.BoundedSemaphore(1)


def samples():
    paths = sorted((WORKSPACE / '测试系统实例').rglob('*.SEL'), key=lambda p: (p.name != '电厂变负荷.SEL', p.name))
    return {f'sample-{i}': p for i, p in enumerate(paths)}


def resolve_file(file_id):
    available = samples()
    if file_id in available:
        return available[file_id], available[file_id].name
    try:
        canonical = str(uuid.UUID(file_id))
    except (ValueError, AttributeError, TypeError):
        raise SelError('文件标识无效')
    record=DATA/f'{canonical}.json'
    if not record.is_file():raise SelError('数据文件不存在，请重新导入')
    meta=json.loads(record.read_text('utf-8'))
    path = DATA / f"{canonical}{'.chd' if meta.get('format')=='chd' else '.sel'}"
    if not path.is_file():
        raise SelError('数据文件不存在，请重新导入')
    return path, json.loads(path.with_suffix('.json').read_text('utf-8'))['name']


def file_info(file_id):
    path, name = resolve_file(file_id)
    stat=path.stat();zone=timezone(timedelta(hours=8))
    return dict(SelFile(path).metadata(), file_id=file_id, filename=name, sha256=sha256(path),
                properties=dict(path=str(path.resolve()),created=datetime.fromtimestamp(stat.st_ctime,zone).isoformat(),modified=datetime.fromtimestamp(stat.st_mtime,zone).isoformat(),accessed=datetime.fromtimestamp(stat.st_atime,zone).isoformat()))


def execute(body, display_full=False):
    path, name = resolve_file(body.get('file_id'))
    reader = SelFile(path)
    condition, channel = int(body.get('condition', 0)), int(body.get('channel', 0))
    raw = reader.read(condition, channel)
    remove_mean = body.get('remove_mean', False)
    if not isinstance(remove_mean, bool):
        raise SelError('去均值参数须为布尔值')
    include_original = body.get('include_original', False)
    if not isinstance(include_original, bool):
        raise SelError('原始对照参数须为布尔值')
    key_phase = reader.key_phase_points(condition, channel)
    full_period = body.get('full_period', len(key_phase)>=2)
    if not isinstance(full_period, bool):
        raise SelError('键相整周期参数须为布尔值')
    result, arrays = analyze(raw, int(reader.rates[channel]), float(body.get('start', 0)),
                             None if body.get('end') in (None, '') else float(body['end']),
                             body.get('window', 'rectangular'), remove_mean,
                             body.get('confidence', 97), body.get('filters', []), include_original,
                             key_phase, full_period,display_full)
    title = body.get('title', reader.channels[channel]['name'])
    if not isinstance(title, str) or not title.strip() or len(title) > 200:
        raise SelError('标题须为 1–200 个字符')
    result['source'] = dict(file_id=body['file_id'], filename=name, sha256=sha256(path),
                            condition=reader.conditions[condition], channel=reader.channels[channel],
                            title=title.strip(), total_count=len(raw), duration=len(raw)/int(reader.rates[channel]),
                            color=body.get('main_color','#1762b5'))
    if not reader.channels[channel]['fft_enabled']:
        import numpy as np
        result['peaks']=[];result['spectrum']=[];arrays=(*arrays[:2],np.empty(0),np.empty(0))
        if '_snapshot_arrays' in result:result['_snapshot_arrays']=(*result['_snapshot_arrays'][:2],np.empty(0),np.empty(0))
    result['parameters']['fft_enabled']=reader.channels[channel]['fft_enabled']
    return result, arrays


JOBS=ExportJobs(ROOT/'data'/'exports',execute,resolve_file)


def enrich_output(body,result,arrays):
    result['annotations']=annotation_records(body)
    if body.get('key_visible'):
        path,_=resolve_file(body['file_id']);reader=SelFile(path);keys=reader.key_phase_points(int(body.get('condition',0)),int(body.get('channel',0)))
        start=result['parameters']['start_index'];stop=result['parameters']['end_index_exclusive'];rate=result['parameters']['sample_rate']
        result['key_phase_points']=[[float(x/rate),float(arrays[1][int(x)-start])] for x in keys if start<=int(x)<stop]
    if body.get('overlays') or body.get('trends'):
        path,_=resolve_file(body['file_id']);reader=SelFile(path)
        if body.get('overlays'):result['overlays']=read_overlays(reader,int(body.get('condition',0)),int(body.get('channel',0)),result['parameters']['start'],result['parameters']['end'],body['overlays'],result['source'],False)
        if body.get('trends'):
            raw=reader.read(int(body.get('condition',0)),int(body.get('channel',0)))
            result['trends']=[create_trend(raw,result['parameters']['sample_rate'],arrays[1],body.get('filters',[]),spec,result['source'],False)[0] for spec in body['trends']]
    if body.get('throw_spec'):
        path,_=resolve_file(body['file_id']);reader=SelFile(path);raw=reader.read(int(body.get('condition',0)),int(body.get('channel',0)))
        record=compute_throw(raw,result['parameters']['sample_rate'],body.get('filters',[]),body['throw_spec'],result['source'])
        result['throwload']=record;result.setdefault('trends',[]).append(record['curve'])
    return result


@dataclass
class Output:
    data: object
    content_type: str = 'application/json; charset=utf-8'
    filename: str | None = None


class AnalysisBusy(SelError):
    pass


def process_analysis(route, body):
    if not COMPUTE.acquire(blocking=False):
        raise AnalysisBusy('正在分析另一项任务，请稍后重试')
    try:
        if route=='/api/strip-data':return Output(prepare_strip(body,resolve_file))
        if route=='/api/waterfall-data':return Output(prepare_waterfall(body,resolve_file))
        if route == '/api/overlays':
            path, name = resolve_file(body.get('file_id'))
            reader = SelFile(path)
            condition, main_channel = int(body.get('condition',0)),int(body.get('channel',0))
            # Validate the main selection and use its actual analyzed interval.
            reader.read(condition,main_channel)
            start,end = float(body['actual_start']),float(body['actual_end'])
            if not 0<=start<end or not math.isfinite(end):
                raise SelError('叠加时段无效')
            source=dict(filename=name,sha256=sha256(path),condition=reader.conditions[condition])
            return Output({'curves':read_overlays(reader,condition,main_channel,start,end,body.get('overlays',[]),source)})
        result, arrays = execute(body,route=='/api/analyze')
        if route=='/api/throwload':
            path,_=resolve_file(body['file_id']);reader=SelFile(path);raw=reader.read(int(body.get('condition',0)),int(body.get('channel',0)))
            return Output(compute_throw(raw,result['parameters']['sample_rate'],body.get('filters',[]),body,result['source']))
        if route=='/api/legacy-data':
            if body.get('kind')=='full':
                from analysis import filter_signal
                import numpy as np
                path,_=resolve_file(body['file_id']);reader=SelFile(path);raw=reader.read(int(body.get('condition',0)),int(body.get('channel',0)));values,_=filter_signal(raw,result['parameters']['sample_rate'],body.get('filters',[]));arrays=(np.arange(len(values))/result['parameters']['sample_rate'],values,*arrays[2:])
            return Output(legacy_data(result,arrays,'peaks' if body.get('kind')=='peaks' else 'chd'),'application/octet-stream','HSJ-'+body.get('kind','current')+'.chd')
        if route in ('/api/report','/api/image'):
            enrich_output(body,result,arrays)
            if route=='/api/image':
                fmt=body.get('format','png');blob,mime=image_bytes(plot_image(result,arrays,body),fmt);return Output(blob,mime,'HSJ图像.'+fmt)
            fmt=body.get('format','docx')
            if fmt in ('xlsx','xls'):
                blob=excel_report([(result,arrays)])
                if fmt=='xls':
                    from office_legacy import convert
                    blob=convert(blob,'xls')
                return Output(blob,'application/vnd.ms-excel' if fmt=='xls' else 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet','HSJ分析报表.'+fmt)
            if fmt not in ('docx','doc'):raise SelError('报表格式无效')
            blob=word_report([(result,arrays)])
            if fmt=='doc':
                from office_legacy import convert
                blob=convert(blob,'doc')
            return Output(blob,'application/msword' if fmt=='doc' else 'application/vnd.openxmlformats-officedocument.wordprocessingml.document','HSJ分析报表.'+fmt)
        if route in ('/api/trends','/api/trend-export'):
            specs=body.get('trends',[])
            if not isinstance(specs,list):raise SelError('移动趋势列表无效')
            path,_=resolve_file(body['file_id']);reader=SelFile(path)
            raw=reader.read(int(body.get('condition',0)),int(body.get('channel',0)))
            curves=[];data=[]
            for spec in specs:
                curve,points=create_trend(raw,result['parameters']['sample_rate'],arrays[1],body.get('filters',[]),spec,result['source'],route=='/api/trends')
                curves.append(curve);data.append(points)
            if route=='/api/trends':return Output({'curves':curves})
            if len(curves)!=1:raise SelError('每次导出一条移动趋势')
            out=io.StringIO(newline='');writer=csv.writer(out)
            writer.writerow(['# source',json.dumps(result['source'],ensure_ascii=False)])
            writer.writerow(['# trend',json.dumps(curves[0]['trend'],ensure_ascii=False)])
            writer.writerow(['时间(s)',f"{curves[0]['channel']['name']} ({curves[0]['channel']['unit']})"])
            writer.writerows(zip(*data[0]))
            return Output('\ufeff'+out.getvalue(),'text/csv; charset=utf-8','HSJ-moving-trend.csv')
        if route == '/api/analyze':
            register(result,result.pop('_snapshot_arrays',arrays))
            return Output(result)
        result['annotations'] = annotation_records(body)
        if body.get('throw_spec'):
            path,_=resolve_file(body['file_id']);reader=SelFile(path);raw=reader.read(int(body.get('condition',0)),int(body.get('channel',0)))
            result['throwload']=compute_throw(raw,result['parameters']['sample_rate'],body.get('filters',[]),body['throw_spec'],result['source'])
        if body.get('trends'):
            path,_=resolve_file(body['file_id']);reader=SelFile(path);raw=reader.read(int(body.get('condition',0)),int(body.get('channel',0)))
            result['trends']=[create_trend(raw,result['parameters']['sample_rate'],arrays[1],body.get('filters',[]),spec,result['source'],False)[0] for spec in body['trends']]
        if body.get('overlays'):
            path,_ = resolve_file(body.get('file_id'))
            result['overlays'] = read_overlays(SelFile(path),int(body.get('condition',0)),int(body.get('channel',0)),
                                              result['parameters']['start'], result['parameters']['end'],
                                              body['overlays'],result['source'],keep_snapshot=False)
        kind = body.get('kind', 'json')
        if kind == 'json':
            return Output(result, filename='HSJ分析结果.json')
        if kind not in ('waveform', 'spectrum'):
            raise SelError('导出类型无效')
        out = io.StringIO(newline='')
        writer = csv.writer(out)
        writer.writerow(['# source_sha256', result['source']['sha256']])
        writer.writerow(['# parameters', json.dumps(result['parameters'], ensure_ascii=False)])
        writer.writerow(['# title', result['source']['title']])
        writer.writerow(['# source', json.dumps(result['source'], ensure_ascii=False)])
        writer.writerow(['# annotations', json.dumps(result['annotations'], ensure_ascii=False)])
        if 'overlays' in result:
            writer.writerow(['# overlays',json.dumps([dict(channel=c['channel'],color=c['color'],visible=c['visible'],processing=c['processing']) for c in result['overlays']],ensure_ascii=False)])
        if kind == 'waveform':
            writer.writerow(['时间(s)', f"{result['source']['channel']['name']} ({result['source']['channel']['unit']})"])
            writer.writerows(zip(arrays[0], arrays[1]))
        else:
            writer.writerow(['频率(Hz)', f"单边峰值幅度 ({result['source']['channel']['unit']})"])
            writer.writerows(zip(arrays[2], arrays[3]))
        return Output('\ufeff'+out.getvalue(), 'text/csv; charset=utf-8', f'HSJ-{kind}.csv')
    finally:
        COMPUTE.release()


def complete_upload(path, identifier, name, metadata):
    if metadata.get('format') == 'chd':
        path.with_suffix('.json').write_text(json.dumps(metadata, ensure_ascii=False), 'utf-8')
    info = SelFile(path).metadata()
    if metadata.get('format') != 'chd':
        path.with_suffix('.json').write_text(json.dumps(metadata, ensure_ascii=False), 'utf-8')
    return dict(info, file_id=identifier, filename=name, sha256=sha256(path))
