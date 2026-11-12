"""Package the user-approved runtime, without launching the business application."""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess
import zipfile

ROOT = Path(__file__).resolve().parent.parent
APP = ROOT / 'build-analysis/out/Skin-Preview-HeaderFix'
VERSION = '2026.11.07.1'
NAME = 'HSJ2010-Analysis-20261107'
RELEASE = ROOT / 'release'
DEST = RELEASE / NAME
ARCHIVE = RELEASE / (NAME + '.zip')


def sha256(p):
    h = hashlib.sha256()
    with p.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024*1024), b''):
            h.update(chunk)
    return h.hexdigest()


def write(path, text, encoding='utf-8-sig'):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(text.replace('\r\n', '\n').replace('\n', '\r\n').encode(encoding))


def main():
    if DEST.exists() or ARCHIVE.exists():
        raise RuntimeError('Release destination already exists; refusing to overwrite.')
    build = json.loads((APP / 'build-result.json').read_text(encoding='utf-8'))
    if sha256(APP / 'ScanAnalysis.exe') != build['sha256']:
        raise RuntimeError('Selected executable does not match its successful build record.')
    (DEST / 'app').mkdir(parents=True)
    (DEST / 'tools').mkdir()
    (DEST / 'installers').mkdir()
    (DEST / 'maintenance').mkdir()
    # Copy the known runtime set and all original data/configuration examples.
    binaries = ['ScanAnalysis.exe', 'BCGCBPRO10080.dll', 'nimercu.dll', 'ScanSel.exe', 'ReportToolD.exe']
    data_ext = {'.mcx','.xls','.bar','.total','.ica','.sel','.esl','.iag','.jgr','.chd','.con'}
    for name in binaries:
        shutil.copy2(APP / name, DEST / 'app' / name)
    for p in APP.iterdir():
        if p.is_file() and p.suffix.lower() in data_ext:
            shutil.copy2(p, DEST / 'app' / p.name)
    for name in ['数据', '示例配置']:
        if (APP / name).is_dir():
            shutil.copytree(APP / name, DEST / 'app' / name)
    (DEST / 'app/log').mkdir()
    for name in ['vcredist2005-x86.exe', 'NI-controls.msi']:
        shutil.copy2(ROOT / 'installers' / name, DEST / 'installers' / name)
    shutil.copy2(ROOT / 'tools/Install-Prerequisites.ps1', DEST / 'tools/Install-Prerequisites.ps1')
    shutil.copytree(ROOT / 'licenses', DEST / 'licenses')

    # Build the same established launcher with a runtime-only app directory.
    launcher = (ROOT / 'tools/RuntimeLauncher.cs').read_text(encoding='utf-8-sig')
    old_path = 'Path.Combine(Root, "build-analysis", "out", "Trial-Run")'
    if launcher.count(old_path) != 3:
        raise RuntimeError('Unexpected launcher layout; manual review required.')
    launcher = launcher.replace(old_path, 'Path.Combine(Root, "app")')
    launcher = launcher.replace('const string Title = "HSJ2010 数据分析";',
                                'const string Title = "HSJ 数据分析 · 浅色版";')
    launcher_src = DEST / 'maintenance/RuntimeLauncher.cs'
    write(launcher_src, launcher)
    compiler = Path('C:/Windows/Microsoft.NET/Framework/v4.0.30319/csc.exe')
    result = subprocess.run([
        str(compiler), '/nologo', '/target:winexe', '/platform:x86', '/optimize+',
        '/reference:System.Web.Extensions.dll', '/reference:System.Windows.Forms.dll',
        '/reference:System.Drawing.dll',
        '/win32icon:' + str(ROOT / 'ScanAnalysis201204021-20231225/res/ScanAnalysis.ico'),
        '/out:' + str(DEST / '启动数据分析.exe'), str(launcher_src)
    ], capture_output=True)
    if result.returncode:
        raise RuntimeError(result.stdout.decode('gb18030',errors='replace') + result.stderr.decode('gb18030',errors='replace'))
    write(DEST / '启动数据分析.cmd', '''@echo off
setlocal
start "" "%~dp0启动数据分析.exe"
''', 'gbk')
    write(DEST / '检查运行环境.cmd', '''@echo off
setlocal
"%~dp0启动数据分析.exe" --check "%~dp0runtime-check.json"
set "HSJ_CHECK_EXIT=%ERRORLEVEL%"
if exist "%~dp0runtime-check.json" notepad.exe "%~dp0runtime-check.json"
exit /b %HSJ_CHECK_EXIT%
''', 'gbk')
    write(DEST / '使用说明.md', f'''# HSJ 数据分析：浅色界面使用包

版本：{VERSION}，整理日期：2026-11-07。

## 开始使用

1. 完整解压到本地可写文件夹，例如 `D:\\HSJAnalysis`。不要直接在压缩包预览窗口里运行，也不要只复制 EXE。
2. 关闭正在运行的数据分析程序，双击根目录的 **启动数据分析.exe**。`启动数据分析.cmd` 是同一启动器的备用入口。
3. 启动器核对核心文件并检查运行环境。若缺少 VC++ 2005 x86 或 NI 图表／分析运行组件，会提示使用包内安装文件；首次安装需要管理员权限，取消后停止启动。
4. 若安装程序明确要求重启，保存工作并重启后再次启动；启动器不会自动重启电脑。
5. 打开数据文件进行分析。示例位于 `app` 和 `app/数据`；可直接使用 `app/电厂变负荷.SEL`。

后续正常使用不需要编译器、Python、Visual Studio 或联网下载依赖。面向 Windows 10/11 x64，主分析程序为 32 位；启动器需要系统已有 .NET Framework 4.x 和 Windows PowerShell。

## 本版内容

- 主窗口、菜单、工具栏、数据树及分析页面采用统一浅色样式。
- 轴心轨迹、相关、趋势、三维、动平衡、盘车、协联及参数／导出窗口已接入统一外观。
- 保持原有图表、分析结果和参数区的整体位置与原有分析计算逻辑。
- 修正动平衡顶部说明文字因换字体而发生的高度裁切。
- 本包使用用户已确认的 `Skin-Preview-HeaderFix` 构建，不需要旧的 HSJLayout.dll。

## 文件夹

| 路径 | 内容 |
| --- | --- |
| `app` | 主程序、配套 DLL、辅助程序、模板与示例数据 |
| `app/数据` | 原配分析样例 |
| `app/示例配置` | 批量保存等功能使用的 CON 时间区间示例 |
| `installers` | VC++ 2005 x86 与 NI 控件离线安装包 |
| `tools` | 首次运行环境配置脚本 |
| `maintenance` | 启动器源码、发布记录和包完整性记录 |
| `licenses` | 原环境随附的许可文本 |

## 日常使用

- 分析程序要求运行目录可写，用于日志和输出。建议把工作数据和导出结果另存到个人工作目录，并定期备份。
- 根目录启动器只严格校验程序、DLL、安装文件和启动脚本；修改示例数据或新增结果文件不会触发核心文件错误。
- Word／Excel 自动化报表仍依赖电脑安装相应 Office 应用。
- 中文显示沿用原程序的多字节字符设置；建议使用中文 Windows 系统区域设置。
- 程序运行时不要移动整个包或替换其中的 EXE／DLL。

## 遇到问题

- 确认所有文件已完整解压，程序目录可写，并已关闭另一份数据分析程序。
- 可运行“检查运行环境.cmd”查看诊断；该入口只核对文件与运行依赖，不执行分析业务或安装组件。
- 启动和安装诊断在 `%LOCALAPPDATA%\\HSJ2010Analysis\\Logs`；分析日志在 `app/log`。
- 若提示核心文件缺失／校验错误，从完整压缩包重新解压到新目录。不要用旧 EXE 或 DLL 覆盖本版。

## 构建与确认范围

Release／Win32 的 62 个 C++ 编译单元、资源、链接及清单嵌入已通过。用户已确认换肤效果及动平衡顶部文字修正。本次整理执行文件和压缩包完整性核对，没有额外执行运行、计算或报表回归测试；全新电脑的首次安装仍需在目标电脑确认。

当前主程序沿用 NI Measurement Studio 8.6 评估 SDK 构建，原试用／评估性质和限制保留。“使用包”表示运行文件整理交付，不代表新增长期开发、商用或分发授权。有关第三方条件见 `licenses` 及原许可。
''')
    metadata = {
        'version': VERSION, 'date': '2026-11-07',
        'selected_build': 'Skin-Preview-HeaderFix', 'configuration': build['configuration'],
        'source_count': build['source_count'], 'app_sha256': build['sha256'],
        'app_size': (DEST/'app/ScanAnalysis.exe').stat().st_size,
        'layout': build['layout'], 'ni_edition': build['ni_edition'],
        'user_confirmation': 'Style accepted; dynamic-balance header correction accepted.',
        'packaging_verification': 'File hashes and archive content only; no application run.',
        'fresh_machine_install_tested': False
    }
    write(DEST / 'maintenance/release-info.json',json.dumps(metadata,ensure_ascii=False,indent=2),'utf-8')
    records = []
    for p in sorted(DEST.rglob('*')):
        if not p.is_file(): continue
        relative = p.relative_to(DEST).as_posix()
        required = (relative.startswith(('installers/','tools/')) or relative in
                    {'启动数据分析.exe','启动数据分析.cmd','检查运行环境.cmd'} or
                    relative in {'app/' + name for name in binaries})
        records.append({'path': relative, 'size': p.stat().st_size,
                        'sha256':sha256(p), 'required_for_launch':required})
    manifest = {'version':VERSION, 'purpose':'private-offline-analysis-runtime',
                'target':'Windows 10/11 x64 host; x86 analysis application',
                'requires_admin_for_initial_runtime_registration':True, 'files':records}
    write(DEST/'manifest.json',json.dumps(manifest,ensure_ascii=False,indent=2),'utf-8')
    # Integrity checks read bytes only, and do not execute the launcher or application.
    for record in records:
        p=DEST/record['path']
        if p.stat().st_size!=record['size'] or sha256(p)!=record['sha256']:
            raise RuntimeError('Packaged file changed: '+record['path'])
    expected={p.relative_to(RELEASE).as_posix() for p in DEST.rglob('*')if p.is_file()}
    with zipfile.ZipFile(ARCHIVE,'w',zipfile.ZIP_DEFLATED,compresslevel=6)as z:
        for p in sorted(DEST.rglob('*')):
            z.write(p,p.relative_to(RELEASE).as_posix())
    with zipfile.ZipFile(ARCHIVE)as z:
        names={info.filename for info in z.infolist()if not info.is_dir()}
        if names!=expected:raise RuntimeError('Archive file list mismatch')
        if z.testzip() is not None:raise RuntimeError('Archive CRC failure')
        for record in records:
            payload=z.read(NAME+'/'+record['path'])
            if hashlib.sha256(payload).hexdigest()!=record['sha256']:
                raise RuntimeError('Archive hash mismatch: '+record['path'])
    digest=sha256(ARCHIVE)
    write(RELEASE/(NAME+'.zip.sha256'),digest+'  '+ARCHIVE.name+'\n','ascii')
    summary={'folder':str(DEST),'archive':str(ARCHIVE),'archive_size':ARCHIVE.stat().st_size,
             'archive_sha256':digest,'file_count':len(expected),'core_files':sum(r['required_for_launch']for r in records),
             'verified':'Folder hashes, archive file list, CRC and each archived file SHA256',
             'application_run':False}
    write(RELEASE/(NAME+'-package-result.json'),json.dumps(summary,ensure_ascii=False,indent=2),'utf-8')
    print(json.dumps(summary,ensure_ascii=False,indent=2))


if __name__=='__main__':main()
