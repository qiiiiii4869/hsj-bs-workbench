"""Sequential batch exports with progress, cancellation and per-item failures."""
from concurrent.futures import ThreadPoolExecutor
import io
import json
import re
import threading
import uuid
import zipfile
from pathlib import Path
import numpy as np
from analysis import envelope
from outputs import plot_image,image_bytes,excel_report,word_report,legacy_data
from sel_reader import SelError

class ExportJobs:
    def __init__(self,root,execute,resolve):
        self.root=Path(root);self.root.mkdir(exist_ok=True,parents=True);self.execute=execute;self.resolve=resolve
        self.pool=ThreadPoolExecutor(max_workers=1,thread_name_prefix='HSJ-export');self.jobs={};self.lock=threading.Lock()
        for path in self.root.glob('*.job.json'):
            try:
                record=json.loads(path.read_text('utf-8'))
                if str(uuid.UUID(record['id']))!=path.name[:-len('.job.json')]:continue
                if record['status'] in ('queued','running'):record.update(status='interrupted',current='服务重启，原任务中断')
                self.jobs[record['id']]=record
            except (ValueError,KeyError,TypeError,AttributeError,OSError):continue

    def save(self,record):
        with self.lock:
            temporary=self.root/(record['id']+'.job.tmp');temporary.write_text(json.dumps(record,ensure_ascii=False),encoding='utf-8');temporary.replace(self.root/(record['id']+'.job.json'))

    def start(self,body):
        items=body.get('items');kind=body.get('output','images')
        if not isinstance(items,list) or not items:raise SelError('至少勾选一个工况通道')
        if kind not in ('images','data','excel','word'):raise SelError('批量输出类型无效')
        body['output']=kind
        identifier=str(uuid.uuid4());record=dict(id=identifier,status='queued',done=0,total=len(items),current='',errors=[],cancel=False)
        with self.lock:self.jobs[identifier]=record
        self.save(record)
        self.pool.submit(self.run,identifier,json.loads(json.dumps(body)))
        return self.status(identifier)

    def status(self,identifier):
        with self.lock:
            if identifier not in self.jobs:raise SelError('批量任务不存在，服务重启后请重新生成')
            return {k:v for k,v in self.jobs[identifier].items() if k!='cancel'}

    def cancel(self,identifier):
        with self.lock:
            if identifier not in self.jobs:raise SelError('批量任务不存在')
            if self.jobs[identifier]['status'] in ('queued','running'):self.jobs[identifier]['cancel']=True
        self.save(self.jobs[identifier])
        return self.status(identifier)

    def download(self,identifier):
        record=self.status(identifier)
        if record['status'] not in ('completed','completed_with_errors'):raise SelError('批量任务尚未完成')
        return (self.root/(identifier+'.zip')).read_bytes()

    def run(self,identifier,body):
        job=self.jobs[identifier];job['status']='running';results=[];manifest=[]
        self.save(job)
        path=self.root/(identifier+'.zip');tmp=self.root/(identifier+'.partial')
        try:
            with zipfile.ZipFile(tmp,'w',zipfile.ZIP_DEFLATED) as archive:
                for number,item in enumerate(body['items']):
                    if job['cancel']:job['status']='cancelled';break
                    label=f"工况 {item.get('condition')} / 通道 {item.get('channel')}";job['current']=label
                    try:
                        params={**body,**item,'annotations':[],'overlays':[],'trends':[],'include_original':False}
                        from data_reader import open_reader
                        src,_=self.resolve(params['file_id']);reader=open_reader(src)
                        frequency=body.get('channel_frequency_ranges',{}).get(str(item['channel']))
                        if frequency is not None:params['frequency_range']=frequency
                        if params.get('title')=='批量波形与频谱分析':params['title']=reader.exam+'，'+reader.conditions[item['condition']]['name']+'，'+reader.channels[item['channel']]['name']+'波形频谱图'
                        points=reader.key_phase_points(item['condition'],item['channel'])
                        if params.get('full_period') and len(points)<2:params['full_period']=False
                        if body['output']=='data':params['full_period']=False
                        # CON entries are seconds; the old exporter ignores an invalid
                        # pair and leaves that category's full channel range unchanged.
                        pair=body.get('con',{}).get(str(item['condition']))
                        if pair is not None:
                            start,end=map(float,pair)
                            params['start']=start if 0<=start<end else 0;params['end']=end if 0<=start<end else None
                            if params['end'] is not None:
                                from data_reader import open_reader as SelFile
                                src,_=self.resolve(params['file_id']);reader=SelFile(src);c=reader.conditions[item['condition']]['sort_code'];duration=reader.logical_count(c,item['channel'])/int(reader.rates[item['channel']]);params['end']=min(params['end'],duration)
                        result,arrays=self.execute(params);source=result['source'];label=f"{source['condition']['name']} / {source['channel']['name']}";job['current']=label
                        name=re.sub(r'[\\/:*?"<>|\x00-\x1f]','_',f"{number+1:03}_{source['condition']['name']}_{source['channel']['name']}")[:180]
                        manifest.append(dict(name=name,source=source,parameters=result['parameters'],statistics=result['statistics']))
                        if body['output']=='images':
                            fmt=body.get('format','png');blob,_=image_bytes(plot_image(result,arrays,params),fmt);archive.writestr(name+'.'+fmt,blob)
                        elif body['output']=='data':archive.writestr(name+'.chd',legacy_data(result,arrays,'chd'))
                        else:
                            w=np.asarray(envelope(arrays[0],arrays[1])).reshape(-1,2);f=np.asarray(envelope(arrays[2],arrays[3])).reshape(-1,2);results.append((result,(w[:,0],w[:,1],f[:,0],f[:,1])))
                    except Exception as error:job['errors'].append(dict(item=item,error=str(error)))
                    job['done']=number+1
                    self.save(job)
                if job['status']!='cancelled':
                    if results:
                        is_excel=body['output']=='excel'
                        extension=('xls' if is_excel else 'doc') if body.get('legacy_office') else ('xlsx' if is_excel else 'docx')
                        blob=excel_report(results,summary_only=True) if is_excel else word_report(results)
                        if body.get('legacy_office'):
                            from office_legacy import convert
                            blob=convert(blob,extension)
                        archive.writestr('通道汇总.'+extension,blob)
                    archive.writestr('分析清单.json',json.dumps(manifest,ensure_ascii=False,indent=2))
                    archive.writestr('错误清单.json',json.dumps(job['errors'],ensure_ascii=False,indent=2))
            if job['status']=='cancelled':tmp.unlink(missing_ok=True);self.save(job);return
            tmp.replace(path);job['status']='completed_with_errors' if job['errors'] else 'completed'
        except Exception as error:
            job['status']='failed';job['errors'].append(dict(error=str(error)));tmp.unlink(missing_ok=True)
        finally:self.save(job)
