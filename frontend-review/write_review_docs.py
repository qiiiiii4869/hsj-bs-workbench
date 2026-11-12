from pathlib import Path
from collections import Counter
import hashlib
import json
import re

OUT = Path(__file__).resolve().parent
J = json.loads((OUT / 'frontend-inventory.json').read_text(encoding='utf-8'))
SRC = Path(J['source_root'])


def cell(v):
    return str(v).replace('|', '｜').replace('\r', '').replace('\n', ' ')


def source(file, line):
    return f'[{file}:{line}]({(SRC / file).as_posix()}:{line})'


def status(c):
    ids = set(c['ids'])
    if ids & {'ID_CoherentUnZoom', 'ID_CoherentReset'}:
        return '需确认：资源存在，未找到非注释处理绑定'
    if 'ID_UserHelp' in ids:
        return '历史占位：菜单 INACTIVE，处理函数为空'
    if ids & {'ID_APP_EXIT', 'ID_VIEW_STATUS_BAR'}:
        return '框架路由候选：需运行确认'
    if not c['entries']:
        return '内部/框架命令：未找到菜单或工具栏资源入口'
    if any(h['macro'] == 'TPM_RETURNCMD/if' for h in c['handlers']):
        return '已追到右键返回值的直接处理分支；待实测'
    return '已追到资源及非注释处理绑定；待实测'


def command_label(c):
    labels = list(dict.fromkeys(e['label'].replace('&', '') for e in c['entries'] if e.get('label')))
    if labels:
        return ' / '.join(labels)
    if c['descriptions']:
        return c['descriptions'][0].replace('\\n', ' / ')
    return c['ids'][0]


def routes(c):
    entries = []
    for e in c['entries']:
        label = ' > '.join(e.get('path', []) + [e.get('label', '')]).replace('&', '')
        label = label.replace('_pop_ > ', '').replace('top > ', '').replace('con > ', '').replace('PopUp > ', '')
        name = {'MENU': '主菜单' if e['resource'] == 'IDR_MAINFRAME' else '右键菜单', 'TOOLBAR': '工具栏', 'ACCELERATORS': '快捷键'}[e['type']]
        entries.append(f'{name} {e["resource"]}' + (f'：{label}' if label else '') + f'（RC:{e["line"]}）')
    return '<br>'.join(dict.fromkeys(entries)) or '资源中未定位，需检查框架入口'


def proposal(c):
    ids = set(c['ids'])
    if ids & {'ID_CoherentUnZoom', 'ID_CoherentReset', 'ID_UserHelp'}:
        return '先确认原版状态；不计作已验收可用；保留记录'
    if not c['entries']:
        return '保留底层路由；可见入口待核定，不新增业务承诺'
    return '保留原菜单/右键能力；工具栏可重排，最终位置待定'


def handler_text(c):
    result = []
    for h in c['handlers']:
        label = h.get('target_expression') or f'{h["class"]}::{h["handler"]}'
        note = '（直接分支）' if h['macro'] == 'TPM_RETURNCMD/if' else ''
        result.append(f'`{label}`{note} {source(h["file"], h["line"])}')
    return '<br>'.join(dict.fromkeys(result)) or '项目内未找到非注释命令绑定'


lines = ['# 前端功能入口与源码证据附录', '', '日期：2026-11-06。配套阅读 [功能保留清单](功能保留清单.md)。', '',
         '本附录记录静态证据。存在资源或函数不代表运行成功；预处理器条件分支未求值。RC 指 ScanAnalysis.rc 的行号。数值相同的命令别名归为同一项；不同窗口使用相同数值 ID 的控件仍按对话框分别列出。', '',
         '## 一、命令逐项映射', '',
         '每项均保留“改版后入口、执行结果、输出证据”的填写位置。当前尚未实施界面改版，最终位置没有定稿。', '',
         '| 编号 / 数值 ID | 名称 / 符号 | 原入口 | 处理函数 / 绑定证据 | 静态判定 | 改版后入口要求 | 运行核对 |',
         '| --- | --- | --- | --- | --- | --- | --- |']
for i, c in enumerate(J['commands'], 1):
    c['review_id'] = f'CMD-{i:03}'
    c['review_status'] = status(c)
    c['proposed_entry'] = proposal(c)
    observed = '此前已观察打开电厂变负荷.SEL；取消/错误路径待测' if 'ID_FILEOPENDATA' in c['ids'] else '待实测'
    lines.append('| ' + ' | '.join(map(cell, [f'{c["review_id"]} / {c["key"]}', command_label(c) + '<br>' + '<br>'.join('`' + s + '`' for s in c['ids']), routes(c), handler_text(c), status(c), proposal(c), observed])) + ' |')

lines += ['', '## 二、菜单与工具栏的加载位置', '', '| 资源 | 类型 / 条目数 | 加载或创建证据 |', '| --- | --- | --- |']
for r in J['resources']:
    if r['type'] not in {'MENU', 'TOOLBAR', 'ACCELERATORS'}:
        continue
    hits = [source(h['file'], h['line']) for h in J['hooks'] if h['kind'] in {'LoadMenu', 'LoadToolBar', 'LoadAccelTable', 'LoadAccelerators'} and re.search(r'\b' + re.escape(r['id']) + r'\b', h['expression'])]
    note = '<br>'.join(dict.fromkeys(hits))
    if r['id'] == 'IDR_MAINFRAME':
        note = '主框架资源；见 ScanAnalysis.cpp 的 CMultiDocTemplate / LoadFrame 初始化，不依赖显式 LoadMenu'
    lines.append('| ' + ' | '.join(map(cell, [f'`{r["id"]}` {source("ScanAnalysis.rc", r["line"])}', f'{r["type"]} / {len(r["entries"])}', note or '需追踪初始化路径'])) + ' |')

lines += ['', '## 三、对话框与嵌入视图：逐控件清单', '',
          '涵盖资源定义中的按钮、输入框、列表、单选/复选项、图表宿主和静态标签；不把 650 个控件当成 650 项业务功能。空文本控件的用途需结合类成员、DDX、初始化和处理函数。通用确定/取消也必须保持语义。', '']
for i, r in enumerate([r for r in J['resources'] if r['type'].startswith('DIALOG')], 1):
    classes = ', '.join(x['class'] for x in r.get('classes', [])) or '未在类 IDD 声明中定位'
    lines += [f'### DLG-{i:02} {r.get("caption") or "嵌入视图"} / {r["id"]}', '', f'资源：{source("ScanAnalysis.rc", r["line"])}；类：`{classes}`。原版/改版：均待逐控件实测。', '',
              '| 控件 ID | 类型 | 文本 / 样式定义 | 证据 |', '| --- | --- | --- | --- |']
    for e in r['entries']:
        lines.append('| ' + ' | '.join(map(cell, [f'`{e["id"]}`', e['kind'], e['definition'], source('ScanAnalysis.rc', e['line'])])) + ' |')
    if r.get('events'):
        lines += ['', '关联事件：', '', '| 宏 | 参数 / 处理函数 | 证据 |', '| --- | --- | --- |']
        for e in r['events']:
            args = ', '.join(e['args']) if isinstance(e['args'], list) else e['args']
            lines.append('| ' + ' | '.join(map(cell, [e['macro'], f'`{args}`', source(e['file'], e['line'])])) + ' |')

lines += ['', '## 四、全部非命令事件', '',
          '包括按钮、下拉列表、列表通知、NI/ActiveX 图表事件、鼠标、键盘及窗口事件。命令入口保持不代表这些交互自动保持。', '',
          '| 编号 | 所属类 | 事件宏 | 参数 / 处理函数 | 证据 |', '| --- | --- | --- | --- | --- |']
for i, e in enumerate(J['events'], 1):
    args = ', '.join(e['args']) if isinstance(e['args'], list) else e['args']
    lines.append('| ' + ' | '.join(map(cell, [f'EVT-{i:03}', e['class'], e['macro'], f'`{args}`', source(e['file'], e['line'])])) + ' |')

lines += ['', '## 五、键盘、文件选择与格式线索', '',
          '本项目 RC 未发现 ACCELERATORS 表，源码未发现自定义 RegisterHotKey/LoadAccelerators 调用。以下为源码键盘处理和文件选择线索；Alt 菜单助记键、系统快捷键、Tab/Enter/Esc 的控件默认行为仍需人工核对。文件过滤器是格式入口证据，不代表每个格式已成功导入/导出。', '',
          '| 类型 | 表达式 | 证据 |', '| --- | --- | --- |']
for h in J['hooks']:
    if h['kind'] not in {'keyboard_or_filter', 'file_dialog'}:
        continue
    lines.append('| ' + ' | '.join(map(cell, [h['kind'], '`' + h['expression'] + '`', source(h['file'], h['line'])])) + ' |')

lines += ['', '## 六、注释中的历史命令绑定', '',
          '只登记历史，不作为可用功能计数；例如主框架注释掉某个绑定，而当前视图存在同命令的活动绑定时，不能据此判定功能失效。', '',
          '| 历史行 | 证据 |', '| --- | --- |']
for c in J['commented_command_lines']:
    lines.append('| ' + ' | '.join(map(cell, ['`' + c['text'] + '`', source(c['file'], c['line'])])) + ' |')

lines += ['', '## 七、核查范围和限制', '',
          '- 按 vcproj 文件清单覆盖 62 个 C++ 编译单元，同时读取应用头文件和 RC；文件名匹配按 Windows 规则忽略大小写。',
          '- 注释先剥离；同数值命令 ID 合并；额外追踪 TPM_RETURNCMD 的 8 处直接分支。',
          '- ON_COMMAND_RANGE 仅把实际出现的命令 ID 纳入逐项表，范围宏本身保存在 JSON，不虚构范围内未暴露的功能。',
          '- 未执行 C++ 预处理器，不承诺各条件编译分支在每种构建配置下都有效。第三方控件内部默认交互不可能仅凭项目消息映射完整列出，需要后续实测。',
          '- 这次不执行计算/导出回归、不安装环境、不改程序源码；源码 SHA256 对照见 source-sha256.json。', '']
(OUT / '功能入口与源码证据.md').write_text('\n'.join(lines), encoding='utf-8')
(OUT / 'frontend-inventory.json').write_text(json.dumps(J, ensure_ascii=False, indent=2), encoding='utf-8')
snapshot = json.loads((OUT / 'source-sha256.json').read_text(encoding='utf-8'))
changed = [name for name, sha in snapshot.items() if hashlib.sha256((SRC / name).read_bytes()).hexdigest() != sha]
if changed:
    raise RuntimeError('Source changed: ' + repr(changed))
assert len({c['review_id'] for c in J['commands']}) == 139
assert sum(len(r['entries']) for r in J['resources'] if r['type'].startswith('DIALOG')) == 650
print('Generated appendix:', len(J['commands']), 'commands;', len(J['events']), 'events')
print('Verified unchanged source files:', len(snapshot))
