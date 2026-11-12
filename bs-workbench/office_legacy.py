"""Real binary DOC/XLS conversion through locally installed Microsoft Office."""
from pathlib import Path
import os
import subprocess
import tempfile
from sel_reader import SelError


def available(kind):
    if os.name!='nt':return False
    import winreg
    try:
        with winreg.OpenKey(winreg.HKEY_CLASSES_ROOT,('Word.Application' if kind=='doc' else 'Excel.Application')+'\\CLSID'):return True
    except OSError:return False


def convert(blob,kind):
    if kind not in ('doc','xls'):raise SelError('旧版Office格式无效')
    if not available(kind):raise SelError('输出真实二进制 '+kind.upper()+' 需要本机安装 Microsoft '+('Word' if kind=='doc' else 'Excel')+'；可直接使用 DOCX / XLSX 输出')
    with tempfile.TemporaryDirectory(prefix='hsj-office-') as folder:
        path=Path(folder);source=path/('source.'+('docx' if kind=='doc' else 'xlsx'));target=path/('output.'+kind);source.write_bytes(blob)
        try:
            result=subprocess.run(['powershell','-NoProfile','-NonInteractive','-STA','-ExecutionPolicy','Bypass','-File',str(Path(__file__).with_name('Convert-Office.ps1')),'-Source',str(source),'-Target',str(target),'-Kind',kind],capture_output=True,timeout=180,creationflags=getattr(subprocess,'CREATE_NO_WINDOW',0))
        except subprocess.TimeoutExpired:raise SelError('Office 格式转换超时，请检查本机 Office 状态后重新导出')
        if result.returncode or not target.is_file():raise SelError('Office 格式转换失败，请使用 DOCX / XLSX 或检查本机 Office 安装')
        return target.read_bytes()
