"""Background specialty exports with durable records and cancellation."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import json
import re
import threading
import uuid
import zipfile
from outputs import image_bytes
from special_reports import plots,plot_picture,workbook,word_report
from sel_reader import SelError


class SpecialtyJobs:
    def __init__(self,root,compute):
        self.root=Path(root);self.root.mkdir(parents=True,exist_ok=True);self.compute=compute
        self.pool=ThreadPoolExecutor(max_workers=1,thread_name_prefix='HSJ-special-export');self.lock=threading.RLock();self.records={}
        for path in self.root.glob('*.json'):
            try:
                record=json.loads(path.read_text('utf-8'))
                if str(uuid.UUID(record['id']))!=path.stem:continue
                if record['status'] in ('queued','running'):record.update(status='interrupted',current='服务重启，任务已中断')
                self.records[record['id']]=record
            except (OSError,ValueError,KeyError,TypeError,AttributeError):continue

    def save(self,record):
        with self.lock:
            tmp=self.root/(record['id']+'.tmp');tmp.write_text(json.dumps(record,ensure_ascii=False),encoding='utf-8');tmp.replace(self.root/(record['id']+'.json'))

    def status(self,identifier):
        with self.lock:
            if identifier not in self.records:raise SelError('专项任务不存在')
            return json.loads(json.dumps({k:v for k,v in self.records[identifier].items() if k!='cancel'}))

    def start(self,body):
        items=body.get('items')
        if not isinstance(items,list) or not 0<len(items)<=4096 or any(not isinstance(x,dict) for x in items):raise SelError('批量项列表无效')
        if body.get('output') not in ('images','xlsx','docx','data'):raise SelError('请选择有效输出内容')
        if body.get('format','png') not in ('png','bmp','jpg','gif','tiff'):raise SelError('图像格式无效')
        identifier=str(uuid.uuid4());record=dict(id=identifier,status='queued',done=0,total=len(items),current='',errors=[],cancel=False)
        with self.lock:self.records[identifier]=record
        self.save(record);self.pool.submit(self.run,identifier,json.loads(json.dumps(body)))
        return self.status(identifier)

    def cancel(self,identifier):
        with self.lock:
            if identifier not in self.records:raise SelError('专项任务不存在')
            self.records[identifier]['cancel']=True;self.save(self.records[identifier])
        return self.status(identifier)

    def download(self,identifier):
        record=self.status(identifier)
        if record['status'] not in ('completed','completed_with_errors'):raise SelError('任务尚未完成')
        return (self.root/(identifier+'.zip')).read_bytes()

    def run(self,identifier,body):
        record=self.records[identifier];record['status']='running';self.save(record)
        partial=self.root/(identifier+'.partial');results=[];manifest=[]
        try:
            with zipfile.ZipFile(partial,'w',zipfile.ZIP_DEFLATED) as archive:
                for index,item in enumerate(body['items']):
                    if record['cancel']:break
                    params={**body.get('parameters',{}),**item}
                    if params.get('module')=='trend' and body['output'] in ('xlsx','docx'):
                        params['legacy_report_interval']=True;params['report_details']=True
                    record['current']=f"通道 {params.get('channel',index+1)}";self.save(record)
                    try:
                        result=self.compute(params)
                        result['include_graphs']=body.get('include_graphs',True)
                        visible=body.get('metrics')
                        if visible is not None:
                            for c in result['curves']:c['visible']=c['name'] in visible
                            if params.get('module')=='trend':
                                indices=[0,1]+[i+2 for i,c in enumerate(result['curves']) if c['visible']]
                                result['columns']=[result['columns'][i] for i in indices]
                                result['rows']=[[row[i] for i in indices] for row in result['rows']]
                        colors=body.get('colors',{})
                        for c in result['curves']:
                            if c['name'] in colors:c['color']=colors[c['name']]
                        name=re.sub(r'[\\/:*?"<>|\x00-\x1f]','_',f"{index+1:03}_{result['title']}")[:160]
                        manifest.append(dict(name=name,request=params,columns=result.get('columns'),rows=result.get('rows')))
                        if body['output']=='images':
                            for j,p in enumerate(plots(result),1):
                                blob,_=image_bytes(plot_picture(p,int(body.get('width',1400)),int(body.get('height',950)),body.get('limits')),body.get('format','png'))
                                archive.writestr(f"{name}-{j}.{body.get('format','png')}",blob)
                        elif body['output']=='data':archive.writestr(name+'.json',json.dumps(result,ensure_ascii=False,indent=2))
                        else:results.append(result)
                    except Exception as error:record['errors'].append(dict(item=item,error=str(error)))
                    record['done']=index+1;self.save(record)
                if record['cancel']:
                    record['status']='cancelled'
                else:
                    if results:
                        archive.writestr('专项汇总.xlsx' if body['output']=='xlsx' else '专项汇总.docx',workbook(results) if body['output']=='xlsx' else word_report(results))
                    archive.writestr('分析清单.json',json.dumps(manifest,ensure_ascii=False,indent=2));archive.writestr('错误清单.json',json.dumps(record['errors'],ensure_ascii=False,indent=2))
            if record['cancel']:partial.unlink(missing_ok=True)
            else:
                partial.replace(self.root/(identifier+'.zip'));record['status']='completed_with_errors' if record['errors'] else 'completed'
        except Exception as error:
            partial.unlink(missing_ok=True);record['status']='failed';record['errors'].append(dict(error=str(error)))
        finally:self.save(record)
