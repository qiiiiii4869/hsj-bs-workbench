"""Read generated archive members without extracting paths on the server."""
from pathlib import PurePosixPath
import uuid
import zipfile
from sel_reader import SelError


def archive(job,identifier):
    try:canonical=str(uuid.UUID(identifier))
    except (ValueError,TypeError,AttributeError):raise SelError('任务标识无效')
    record=job.status(canonical)
    if record['status'] not in ('completed','completed_with_errors'):raise SelError('任务尚未完成')
    try:return zipfile.ZipFile(job.root/(canonical+'.zip'),'r')
    except (OSError,zipfile.BadZipFile):raise SelError('结果文件已清理或损坏，请重新生成')


def listing(job,identifier):
    with archive(job,identifier) as package:
        return dict(files=[dict(index=index,name=PurePosixPath(info.filename.replace('\\','/')).name,size=info.file_size) for index,info in enumerate(package.infolist()) if not info.is_dir()])


def read(job,identifier,index):
    if isinstance(index,bool) or not isinstance(index,int):raise SelError('结果文件索引无效')
    with archive(job,identifier) as package:
        infos=package.infolist()
        if not 0<=index<len(infos) or infos[index].is_dir():raise SelError('结果文件不存在')
        info=infos[index]
        if info.file_size>512*1024*1024:raise SelError('单个结果文件超过512MB，请下载ZIP')
        return package.read(info),PurePosixPath(info.filename.replace('\\','/')).name
