"""Stage a relocatable Windows BS workspace; never run application tests."""
from pathlib import Path
from datetime import datetime
import argparse
import hashlib
import json
import shutil
import zipfile

ROOT=Path(__file__).resolve().parent.parent
APP=ROOT/'bs-workbench'
EXCLUDED={'.venv','.runtime','__pycache__','.pytest_cache','delivery-backups','data'}
SOURCE_SUFFIXES={'.cpp','.h','.rc','.rc2','.sln','.vcproj','.def','.idc','.ico','.bmp','.png','.jpg','.jpeg','.gif','.txt','.con'}
DATA_SUFFIXES={'.sel','.chd','.jgr','.iag','.ica','.con','.bar','.total'}


def copy_file(path,stage):
    target=stage/path.relative_to(ROOT)
    target.parent.mkdir(parents=True,exist_ok=True)
    shutil.copy2(path,target)


def stage_files(stage):
    for path in sorted(APP.rglob('*')):
        rel=path.relative_to(APP)
        if not path.is_file() or any(part in EXCLUDED for part in rel.parts):continue
        if path.suffix.lower() in ('.pyc','.pyo','.obj','.log','.tmp','.partial'):continue
        copy_file(path,stage)
    # Imported inputs retain their UUIDs and paired metadata. Generated export
    # jobs and process identity files must not be mistaken for portable state.
    for meta in sorted((APP/'data').glob('*.json')):
        payloads=[meta.with_suffix(ext) for ext in ('.sel','.chd') if meta.with_suffix(ext).is_file()]
        if not payloads:continue
        json.loads(meta.read_text('utf-8'))
        for path in [meta,*payloads]:copy_file(path,stage)
    for folder in ('frontend-review','ScanAnalysis201204021-20231225','测试系统实例'):
        for path in sorted((ROOT/folder).rglob('*')):
            if not path.is_file():continue
            rel=path.relative_to(ROOT/folder)
            if folder=='frontend-review':
                if len(rel.parts)!=1 or path.suffix.lower() not in ('.md','.json','.py','.diff'):continue
            elif folder=='ScanAnalysis201204021-20231225':
                if any(part.lower() in ('debug','release','ipch','.vs','__pycache__') for part in rel.parts):continue
                if path.suffix.lower() not in SOURCE_SUFFIXES:continue
            elif path.suffix.lower() not in DATA_SUFFIXES:continue
            copy_file(path,stage)
    for name in ('启动浏览器分析版.cmd','停止浏览器分析版.cmd','重启浏览器分析版.cmd','BS.cmd','HSJ数据分析系统_CS优化与BS可行性分析.md'):
        path=ROOT/name
        if path.is_file():copy_file(path,stage)
    (stage/'bs-workbench/data').mkdir(exist_ok=True)
    embedded=stage/'python-3.13.13-embed-amd64.zip'
    if not embedded.is_file():raise SystemExit('Download the official Python embedded ZIP into the staging directory first.')
    runtime=stage/'runtime'
    runtime.mkdir(exist_ok=True)
    with zipfile.ZipFile(embedded) as package:package.extractall(runtime)
    (runtime/'python313._pth').write_text('python313.zip\n.\nLib/site-packages\n../bs-workbench\nimport site\n',encoding='utf-8')
    (runtime/'Lib/site-packages').mkdir(parents=True,exist_ok=True)
    wheels=stage/'wheelhouse'
    # Keep the exact source-machine lock and pip, discarding preliminary
    # resolver versions from the staging download directory only.
    versions={}
    for line in (APP/'requirements-transfer.lock').read_text('utf-8').splitlines():
        if line and not line.startswith('#'):
            name,version=line.split('==');versions[name.lower().replace('_','-')]=version
    for wheel in wheels.glob('*.whl'):
        name,version,*_=wheel.name.split('-');name=name.lower().replace('_','-')
        if name!='pip' and versions.get(name)!=version:wheel.unlink()
    pip_wheel=next(wheels.glob('pip-*.whl'))
    # Vendoring pip gives the embedded runtime a fully offline dependency
    # repair command. Pip itself is pure Python and includes its licenses.
    with zipfile.ZipFile(pip_wheel) as package:package.extractall(runtime/'Lib/site-packages')
    print('Staged sources, original references, imported/sample data and embedded runtime:',stage)


def archive(stage):
    manifest={
        'application':'HSJ BS transfer workspace','version':'0.17-orbit-classic-layout',
        'packaged_at':datetime.now().isoformat(timespec='seconds'),'platform':'Windows 10/11 x64',
        'runtime':'CPython 3.13.13 embedded, application dependencies vendored locally',
        'verification':'Archive CRC and inventory only; application behavior and GUI not tested.',
        'excludes':['machine-specific .venv','.runtime PID/logs','caches','generated export jobs','old delivery backups','old MFC executables','third-party C++ trial toolchains'],
        'files':[],
    }
    for path in sorted(stage.rglob('*')):
        if not path.is_file() or path.name=='PACKAGE-MANIFEST.json' or '__pycache__' in path.parts:continue
        blob=path.read_bytes();manifest['files'].append({'path':path.relative_to(stage).as_posix(),'bytes':len(blob),'sha256':hashlib.sha256(blob).hexdigest()})
    (stage/'PACKAGE-MANIFEST.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2),encoding='utf-8')
    target=stage.with_suffix('.zip')
    if target.exists():raise SystemExit('Archive already exists; use a new staging name.')
    with zipfile.ZipFile(target,'w',zipfile.ZIP_DEFLATED,compresslevel=6,allowZip64=True) as package:
        for path in sorted(stage.rglob('*')):
            if path.is_file() and '__pycache__' not in path.parts:package.write(path,stage.name+'/'+path.relative_to(stage).as_posix())
    with zipfile.ZipFile(target) as package:
        error=package.testzip()
        if error:raise SystemExit('Archive CRC failed: '+error)
        if len(package.infolist())!=len(manifest['files'])+1:raise SystemExit('Archive inventory count does not match the manifest.')
    digest=hashlib.sha256(target.read_bytes()).hexdigest()
    target.with_suffix('.zip.sha256').write_text(digest+'  '+target.name+'\n',encoding='ascii')
    print(json.dumps({'zip':str(target),'bytes':target.stat().st_size,'files':len(manifest['files'])+1,'sha256':digest},ensure_ascii=False))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('stage',type=Path)
    parser.add_argument('--archive',action='store_true')
    args=parser.parse_args();stage=args.stage.resolve()
    if not stage.is_relative_to((ROOT/'release').resolve()):raise SystemExit('Staging must remain inside this workspace release directory.')
    if args.archive:archive(stage)
    else:stage_files(stage)


if __name__=='__main__':main()
