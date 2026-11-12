"""FastAPI transport for the existing HSJ analysis services."""
from contextlib import asynccontextmanager
import json
import logging
import mimetypes
from pathlib import Path
import secrets
from urllib.parse import quote, unquote
import uuid

import anyio
from fastapi import Depends, FastAPI, Request
from fastapi.exceptions import RequestValidationError
from starlette.datastructures import Headers, MutableHeaders
from starlette.exceptions import HTTPException
from starlette.responses import Response
from starlette.concurrency import run_in_threadpool
from starlette.background import BackgroundTask

import analysis_service as service
import specialty
import special_reports
import export_files
from special_jobs import SpecialtyJobs
from api_docs import document_schema
from sel_reader import SelError
from stable_filter import FILTER_ENGINE_VERSION

VERSION = '0.17-orbit-classic-layout'
TOKEN = secrets.token_urlsafe(32)
MAX_UPLOAD = 256 * 1024 * 1024


def run_special_job(body):
    with service.COMPUTE:
        return specialty.dispatch(body,service.resolve_file)


SPECIAL_JOBS=SpecialtyJobs(service.DATA/'special-exports',run_special_job)
SECURITY_HEADERS = {
    'X-Content-Type-Options': 'nosniff',
    'Cache-Control': 'no-store',
    'Content-Security-Policy': "default-src 'self'; script-src 'self'; style-src 'self'; img-src 'self' data: blob:; object-src 'none'; frame-ancestors 'self'; base-uri 'none'",
}
ASSETS = {
    '/orbit-layout.js':'orbit-layout.js','/orbit-layout.css':'orbit-layout.css',
    '/balance':'balance.html','/balance.js':'balance.js','/balance.css':'balance.css',
    '/balance-grid.js':'balance-grid.js',
    '/save-target.js':'save-target.js',
    '/files-workspace.js':'files-workspace.js',
    '/output-extra.js':'output-extra.js',
    '/about.json':'about.json',
    '/desktop': 'desktop.html', '/desktop.js': 'desktop.js', '/desktop.css': 'desktop.css',
    '/desktop-client.js': 'desktop-client.js',
    '/specialty': 'specialty.html', '/specialty.js': 'specialty.js', '/specialty-extra.js': 'specialty-extra.js', '/specialty.css': 'specialty.css',
    '/wave-batch-extra.js':'wave-batch-extra.js', '/properties.js':'properties.js', '/workspace.js': 'workspace.js', '/workspace.css': 'workspace.css',
    '/waterfall': 'waterfall.html', '/waterfall.js': 'waterfall.js', '/waterfall.css': 'waterfall.css',
    '/docs': 'docs.html', '/docs/': 'docs.html',
    '/docs.js': 'docs.js', '/docs.css': 'docs.css',
    '/vendor/swagger-ui/swagger-ui-bundle.js': 'vendor/swagger-ui/swagger-ui-bundle.js',
    '/vendor/swagger-ui/swagger-ui.css': 'vendor/swagger-ui/swagger-ui.css',
    '/vendor/swagger-ui/LICENSE': 'vendor/swagger-ui/LICENSE',
    '/': 'index.html', '/index.html': 'index.html',
    '/classic.js': 'classic.js', '/classic.css': 'classic.css',
    '/startup.js': 'startup.js', '/charts.js': 'charts.js',
    '/channels.js': 'channels.js', '/trends.js': 'trends.js',
    '/page.js': 'page.js', '/exports.js': 'exports.js',
    '/app.js': 'app.js', '/style.css': 'style.css',
    '/strip': 'strip.html', '/strip.html': 'strip.html',
    '/strip.js': 'strip.js', '/strip.css': 'strip.css',
}
ANALYSIS_ROUTES = (
    '/api/analyze', '/api/export', '/api/overlays', '/api/trends',
    '/api/trend-export', '/api/throwload', '/api/report', '/api/image',
    '/api/legacy-data', '/api/strip-data', '/api/waterfall-data',
)
JSON_REQUEST = {'requestBody': {'required': True, 'content': {
    'application/json': {'schema': {'type': 'object', 'additionalProperties': True}}
}}}


def reply(data, status=200, content_type='application/json; charset=utf-8', filename=None):
    if isinstance(data, service.Output):
        content_type, filename, data = data.content_type, data.filename, data.data
    if isinstance(data, (dict, list)):
        data = json.dumps(data, ensure_ascii=False, allow_nan=False).encode('utf-8')
    elif isinstance(data, str):
        data = data.encode('utf-8')
    headers = dict(SECURITY_HEADERS, **{'Content-Type': content_type})
    if filename:
        headers['Content-Disposition'] = f"attachment; filename*=UTF-8''{quote(filename)}"
    return Response(data, status_code=status, headers=headers)


def body_limit(path):
    if path in ('/api/upload', '/api/chd-upload'):
        return MAX_UPLOAD
    return 32 * 1024 * 1024 if path == '/api/image-convert' else 1024 * 1024


class LocalAccessMiddleware:
    """Apply the existing host, session and origin rules before reading a body."""
    def __init__(self, app):
        self.app = app

    async def __call__(self, scope, receive, send):
        if scope['type'] != 'http':
            return await self.app(scope, receive, send)
        headers = Headers(scope=scope)
        port = scope['server'][1]
        hosts = {f'127.0.0.1:{port}', f'localhost:{port}'}

        async def send_headers(message):
            if message['type'] == 'http.response.start':
                response_headers = MutableHeaders(scope=message)
                for key, value in SECURITY_HEADERS.items():
                    if key == 'Content-Security-Policy' and scope['path'] in ('/docs', '/docs/'):
                        # Swagger uses inline element styles; scripts remain local external files.
                        value = value.replace("style-src 'self'", "style-src 'self' 'unsafe-inline'; font-src 'self' data:")
                    response_headers[key] = value
            await send(message)

        error = None
        if headers.get('host') not in hosts:
            error = reply({'error': '仅允许本机访问'}, 403)
        elif scope['method'] == 'POST':
            if not secrets.compare_digest(headers.get('X-HSJ-Token', ''), TOKEN):
                error = reply({'error': '会话已失效，请刷新页面'}, 403)
            elif headers.get('origin') and headers['origin'] not in {f'http://{host}' for host in hosts}:
                error = reply({'error': '请求来源无效'}, 403)
            else:
                try:
                    length = int(headers.get('content-length', '0'))
                except ValueError:
                    length = 0
                if not 0 < length <= body_limit(scope['path']):
                    message = '文件须在 256 MB 以内，且不能为空' if scope['path'] in ('/api/upload', '/api/chd-upload') else '请求内容超过接口大小限制，或内容为空'
                    error = reply({'error': message}, 413)
        if error is not None:
            return await error(scope, receive, send_headers)
        await self.app(scope, receive, send_headers)


@asynccontextmanager
async def lifespan(app):
    service.DATA.mkdir(exist_ok=True)
    yield
    await run_in_threadpool(service.JOBS.pool.shutdown, wait=True, cancel_futures=True)
    await run_in_threadpool(SPECIAL_JOBS.pool.shutdown, wait=True, cancel_futures=True)


app = FastAPI(
    title='HSJ 本机分析接口', version=VERSION,
    description='波形、频谱、长图与导出接口。POST 使用 /api/bootstrap 返回的 X-HSJ-Token。',
    docs_url=None, redoc_url=None, openapi_url='/api/schema', lifespan=lifespan,
)
app.add_middleware(LocalAccessMiddleware)
app.openapi = lambda: document_schema(app)


@app.exception_handler(Exception)
async def handle_error(request, error):
    if isinstance(error, service.AnalysisBusy):
        return reply({'error': str(error)}, 409)
    if isinstance(error, (SelError, ValueError, TypeError, KeyError, OverflowError)):
        return reply({'error': str(error) if isinstance(error, SelError) else '请求参数无效'}, 400)
    logging.getLogger('uvicorn.error').error('Service request failed', exc_info=error)
    return reply({'error': '服务处理失败，请查看服务窗口日志'}, 500)


# Expected input errors are handled inside Starlette's ExceptionMiddleware.
for error_type in (SelError, ValueError, TypeError, KeyError, OverflowError):
    app.add_exception_handler(error_type, handle_error)


@app.exception_handler(RequestValidationError)
async def invalid_request(request, error):
    return reply({'error': '请求参数无效'}, 400)


@app.exception_handler(HTTPException)
async def http_error(request, error):
    if error.status_code == 413:
        return reply({'error': '请求内容超过接口大小限制'}, 413)
    message = '请求方法不支持' if error.status_code == 405 else '接口不存在' if request.url.path.startswith('/api/') else '页面不存在'
    return reply({'error': message}, error.status_code)


async def read_json(request: Request):
    raw = bytearray()
    async for chunk in request.stream():
        if len(raw) + len(chunk) > body_limit(request.url.path):
            raise HTTPException(413)
        raw.extend(chunk)
    if len(raw) != int(request.headers['content-length']):
        raise SelError('请求内容不完整')
    body = await run_in_threadpool(json.loads, raw)
    if not isinstance(body, dict):
        raise SelError('请求参数格式无效')
    return body


@app.get('/api/health')
def health():
    return reply({'application': 'hsj-workbench', 'version': VERSION, 'filter_engine': FILTER_ENGINE_VERSION, 'framework': 'FastAPI', 'server': 'Uvicorn'})


@app.post('/api/shutdown',openapi_extra=JSON_REQUEST)
def shutdown(body: dict=Depends(read_json)):
    if not hasattr(app.state,'shutdown'):raise SelError('此启动方式不支持从页面退出服务，请使用停止脚本')
    for identifier in list(service.JOBS.jobs):
        service.JOBS.cancel(identifier)
    for identifier in list(SPECIAL_JOBS.records):
        SPECIAL_JOBS.cancel(identifier)
    response=reply({'status':'shutting_down'});response.background=BackgroundTask(app.state.shutdown);return response


@app.get('/api/coverage')
def coverage():
    return reply(json.loads((service.ROOT / 'migration-ledger.json').read_text('utf-8')))


@app.get('/api/bootstrap')
def bootstrap():
    imported = []
    for info in service.DATA.glob('*.json'):
        if info.with_suffix('.sel').is_file() or info.with_suffix('.chd').is_file():
            imported.append(dict(id=info.stem, name=json.loads(info.read_text('utf-8'))['name']))
    return reply(dict(token=TOKEN, samples=[dict(id=k, name=p.name, size=p.stat().st_size) for k, p in service.samples().items()], imported=imported))


@app.post('/api/open', openapi_extra=JSON_REQUEST)
def open_file(body: dict = Depends(read_json)):
    return reply(service.file_info(body.get('file_id')))


@app.post('/api/chart', openapi_extra=JSON_REQUEST)
def chart(body: dict = Depends(read_json)):
    return reply(service.read_chart(body))


@app.post('/api/batch', openapi_extra=JSON_REQUEST)
def batch(body: dict = Depends(read_json)):
    return reply(service.JOBS.start(body))


@app.post('/api/batch-status', openapi_extra=JSON_REQUEST)
def batch_status(body: dict = Depends(read_json)):
    return reply(service.JOBS.status(body.get('id')))


@app.post('/api/batch-cancel', openapi_extra=JSON_REQUEST)
def batch_cancel(body: dict = Depends(read_json)):
    return reply(service.JOBS.cancel(body.get('id')))


@app.post('/api/batch-download', openapi_extra=JSON_REQUEST)
def batch_download(body: dict = Depends(read_json)):
    return reply(service.JOBS.download(body.get('id')), content_type='application/zip', filename='HSJ批量结果.zip')


@app.post('/api/image-convert', openapi_extra=JSON_REQUEST)
def image_convert(body: dict = Depends(read_json)):
    blob, mime = service.convert_image(body)
    return reply(blob, content_type=mime, filename='HSJ图像.' + body.get('format', 'png'))


@app.post('/api/special', openapi_extra=JSON_REQUEST)
def special_analysis(body: dict = Depends(read_json)):
    if not service.COMPUTE.acquire(blocking=False):
        raise service.AnalysisBusy('正在执行其他分析，请稍后重试')
    try:
        return reply(specialty.dispatch(body, service.resolve_file))
    finally:
        service.COMPUTE.release()


@app.post('/api/special-report', openapi_extra=JSON_REQUEST)
def special_report(body: dict = Depends(read_json)):
    if body.get('source_request'):
        computed=run_special_job(body['source_request']);computed['title']=body.get('title',computed['title']);computed['include_graphs']=body.get('include_graphs',True)
        for c in computed.get('curves',[]):
            style=next((v for v in body.get('curve_styles',[]) if v.get('name')==c['name']),None)
            if style:c.update(color=style.get('color',c['color']),visible=style.get('visible',True))
        computed['format']=body.get('format','xlsx');body=computed
    results=body.get('results',[body])
    if not isinstance(results,list) or not results or len(results)>4096 or any(not isinstance(result,dict) for result in results):raise SelError('专项报表结果列表无效')
    for index,result in enumerate(results):
        if result.get('request',{}).get('module')=='trend':
            params={**result['request'],'report_details':True,'legacy_report_interval':True}
            computed=run_special_job(params);computed['title']=result.get('title',computed['title'])
            computed['include_graphs']=result.get('include_graphs',True)
            for curve in computed['curves']:
                style=next((c for c in result.get('curves',[]) if c['name']==curve['name']),None)
                if style:curve.update(color=style.get('color',curve['color']),visible=style.get('visible',True))
            wanted=result.get('columns',computed['columns']);indices=[i for i,name in enumerate(computed['columns']) if name in wanted]
            computed['columns']=[computed['columns'][i] for i in indices];computed['rows']=[[row[i] for i in indices] for row in computed['rows']]
            results[index]=computed
    fmt=body.get('format','xlsx')
    if fmt not in ('xlsx','docx','xls','doc'):raise SelError('报表格式无效')
    blob=special_reports.word_report(results) if fmt in ('doc','docx') else special_reports.workbook(results)
    if fmt in ('doc','xls'):
        from office_legacy import convert
        blob=convert(blob,fmt)
    mime={'doc':'application/msword','xls':'application/vnd.ms-excel','docx':'application/vnd.openxmlformats-officedocument.wordprocessingml.document','xlsx':'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'}[fmt]
    return reply(blob,content_type=mime,filename='HSJ专项分析.'+fmt)


@app.post('/api/special-legacy',openapi_extra=JSON_REQUEST)
def special_legacy(body: dict = Depends(read_json)):
    extension=body.get('legacy_extension');text=body.get('legacy_text')
    if extension not in ('oca','oag') or not isinstance(text,str):raise SelError('原版专项结果格式无效')
    return reply(text.encode('gb18030'),content_type='application/octet-stream',filename='HSJ专项结果.'+extension)



@app.post('/api/special-batch',openapi_extra=JSON_REQUEST)
def special_batch(body: dict = Depends(read_json)):
    return reply(SPECIAL_JOBS.start(body))


@app.post('/api/special-batch-status',openapi_extra=JSON_REQUEST)
def special_batch_status(body: dict = Depends(read_json)):
    return reply(SPECIAL_JOBS.status(body.get('id')))


@app.post('/api/special-batch-cancel',openapi_extra=JSON_REQUEST)
def special_batch_cancel(body: dict = Depends(read_json)):
    return reply(SPECIAL_JOBS.cancel(body.get('id')))


@app.post('/api/special-batch-download',openapi_extra=JSON_REQUEST)
def special_batch_download(body: dict = Depends(read_json)):
    return reply(SPECIAL_JOBS.download(body.get('id')),content_type='application/zip',filename='HSJ专项批量结果.zip')


def export_job(body):
    if body.get('type')=='wave':return service.JOBS
    if body.get('type')=='special':return SPECIAL_JOBS
    raise SelError('导出任务类型无效')


@app.post('/api/export-file-list',openapi_extra=JSON_REQUEST)
def export_file_list(body: dict=Depends(read_json)):
    return reply(export_files.listing(export_job(body),body.get('id')))


@app.post('/api/export-file',openapi_extra=JSON_REQUEST)
def export_file(body: dict=Depends(read_json)):
    blob,name=export_files.read(export_job(body),body.get('id'),body.get('index'))
    return reply(blob,content_type='application/octet-stream',filename=name)


def process_analysis(request: Request, body: dict = Depends(read_json)):
    # FastAPI puts synchronous handlers in its worker thread pool.
    # Numerical work therefore does not execute on the ASGI event loop.
    return reply(service.process_analysis(request.url.path, body))


for route in ANALYSIS_ROUTES:
    app.add_api_route(route, process_analysis, methods=['POST'], openapi_extra=JSON_REQUEST,
                      name=route.removeprefix('/api/').replace('-', '_'))


async def receive_upload(request: Request, chd=False):
    name = Path(unquote(request.headers.get('X-Filename', 'data.chd' if chd else 'data.sel')).replace('\\', '/')).name
    if not chd and Path(name).suffix.lower() != '.sel':
        raise SelError('请选择 .SEL 数据文件')
    identifier = str(uuid.uuid4())
    path = service.DATA / (identifier + ('.chd' if chd else '.sel'))
    metadata = dict(name=name)
    if chd:
        metadata.update(format='chd', sample_rate=request.headers.get('X-Sample-Rate', ''),
                        title=unquote(request.headers.get('X-Title', name)),
                        unit=unquote(request.headers.get('X-Unit', '')), color=request.headers.get('X-Color', '#1762b5'))
    completed = False
    try:
        received = 0
        async with await anyio.open_file(path, 'xb') as stream:
            async for chunk in request.stream():
                received += len(chunk)
                if received > MAX_UPLOAD:
                    raise HTTPException(413)
                await stream.write(chunk)
        if received != int(request.headers['content-length']):
            raise SelError('文件上传中断')
        info = await run_in_threadpool(service.complete_upload, path, identifier, name, metadata)
        completed = True
        return reply(info)
    finally:
        if not completed:
            path.unlink(missing_ok=True)
            path.with_suffix('.json').unlink(missing_ok=True)


UPLOAD_REQUEST = {'requestBody': {'required': True, 'content': {
    'application/octet-stream': {'schema': {'type': 'string', 'format': 'binary'}}
}}}


@app.post('/api/upload', openapi_extra=UPLOAD_REQUEST)
async def upload(request: Request):
    return await receive_upload(request)


@app.post('/api/chd-upload', openapi_extra=UPLOAD_REQUEST)
async def upload_chd(request: Request):
    return await receive_upload(request, chd=True)


def static_asset(request: Request):
    asset='balance.html' if request.url.path=='/specialty' and request.query_params.get('module')=='balance' else ASSETS[request.url.path]
    path = service.STATIC / asset
    return reply(path.read_bytes(), content_type=(mimetypes.guess_type(path)[0] or 'application/octet-stream') + '; charset=utf-8')


for route in ASSETS:
    app.add_api_route(route, static_asset, methods=['GET'], include_in_schema=False)
