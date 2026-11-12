"""Complete OpenAPI contracts over the existing synchronous business handlers."""
from copy import deepcopy
from fastapi.openapi.utils import get_openapi
from api_contracts import OPERATIONS, SCHEMAS, TAGS, ref

DOCS_REVISION='2026-11-11-complete-contracts-v1'


def document_schema(app):
    if app.openapi_schema:
        return app.openapi_schema
    schema=get_openapi(title=app.title,version=app.version,description=app.description,routes=app.routes)
    runtime_paths=set(schema['paths'])
    expected_paths=set(OPERATIONS)-{'/api/schema'}
    if runtime_paths!=expected_paths:
        raise RuntimeError('接口文档与路由不一致：未登记='+str(sorted(runtime_paths-expected_paths))+'；无路由='+str(sorted(expected_paths-runtime_paths)))
    schema['paths']['/api/schema']={'get':{'operationId':'read_api_schema','responses':{}}}
    schema['info']['description']='''本机HSJ数据分析完整接口。先GET /api/bootstrap取得会话token和文件ID，再读取/api/open，POST携带X-HSJ-Token。

本服务仅允许localhost/127.0.0.1访问。JSON请求通常不超过1MiB；原文件二进制上传不超过256MiB，图像转换请求不超过32MiB。POST必须有非空请求体和正确Content-Length，即使停止接口也应传{}。

峰峰值支持95/97/100%，默认97%；普通固定区间请显式full_period=false，有键相时省略该字段默认整周期。显示曲线会抽样，完整数值请导出CSV。

任务和上传文件落在本地data目录。已完成任务记录可重载，运行中任务在重启后标为interrupted，不会自动恢复执行。analysis_id是临时快照，重启或缓存逐出后须重新分析。

DOC/XLS需本机Microsoft Office，DOCX/XLSX不需要。计算成功不等于已证明全部原NI参数域精度等价，参见滤波验证记录。'''
    schema['info']['x-docs-revision']=DOCS_REVISION
    schema['servers']=[{'url':'/','description':'当前本机服务（同源）'}]
    schema['tags']=deepcopy(TAGS)
    components=schema.setdefault('components',{})
    components.setdefault('schemas',{}).update(deepcopy(SCHEMAS))
    components.setdefault('securitySchemes',{})['HSJSession']={'type':'apiKey','in':'header','name':'X-HSJ-Token',
        'description':'bootstrap返回的当前进程token。文档自动授权；服务重启后点击“刷新本机会话”。'}
    for path,contract in OPERATIONS.items():
        method=contract['method'];operation=schema['paths'][path][method]
        operation.update(operationId=method+'-'+path.removeprefix('/api/'),tags=[contract['tag']],summary=contract['summary'],description=contract['description'])
        responses={'200':{'description':'成功结果','content':{}}}
        if contract['response'] is not None:
            responses['200']['content']['application/json']={'schema':deepcopy(contract['response'])}
        for mime in contract['downloads']:
            responses['200']['content'][mime]={'schema':{'type':'string','format':'binary'}}
        if contract['downloads']:
            responses['200']['headers']={'Content-Disposition':{'description':'attachment与UTF-8编码文件名；解析filename*。','schema':{'type':'string'}}}
        for code,description in [('400','参数/文件/任务无效，或缺少当前格式的运行依赖'),('403','会话token、Host或Origin无效'),('404','未知路径'),('405','方法不支持'),('409','已有计算正在执行，请稍后重试'),('413','空请求或超过大小限制'),('500','未预期的服务故障，查看本地日志')]:
            responses[code]={'description':description,'content':{'application/json':{'schema':ref('Error'),'example':{'error':description}}}}
        operation['responses']=responses
        if method=='post':
            operation['security']=[{'HSJSession':[]}]
            if path in ('/api/upload','/api/chd-upload'):
                operation['requestBody']={'required':True,'content':{'application/octet-stream':{'schema':{'type':'string','format':'binary'}}}}
                headers=[('X-Filename','原文件名，中文须encodeURIComponent URL编码。','data.chd' if path=='/api/chd-upload' else 'data.sel',False)]
                if path=='/api/chd-upload':headers.extend([('X-Sample-Rate','采样率，正整数Hz。','1024',True),('X-Title','通道名，中文URL编码。','CHD',False),('X-Unit','通道单位，中文URL编码。','um',False),('X-Color','十六进制颜色。','#0000ff',False)])
                operation['parameters']=[{'name':name,'in':'header','description':description,'required':required,'schema':{'type':'string','default':value}} for name,description,value,required in headers]
            else:
                operation['requestBody']={'required':True,'content':{'application/json':{'schema':deepcopy(contract['request']),'examples':deepcopy(contract['examples'])}}}
        else:operation['security']=[]
    schema['paths']['/api/analyze']['post']['responses']['200']['links']={'ReadFirstWavePoint':{'operationId':'post-chart','requestBody':{'analysis_id':'$response.body#/analysis_id','plot':'wave','action':'index','index':0},'description':'把返回analysis_id用于精确读取原始采样点。'}}
    for path,prefix in [('/api/batch','batch'),('/api/special-batch','special-batch')]:
        schema['paths'][path]['post']['responses']['200']['links']={'ReadProgress':{'operationId':'post-'+prefix+'-status','requestBody':{'id':'$response.body#/id'},'description':'使用返回id查询任务进度；完成后再调用下载。'}}
    app.openapi_schema=schema
    return schema
