"""Read-only source inventory. Writes review artifacts only beside this script."""
from pathlib import Path
from collections import defaultdict, Counter
import hashlib
import json
import re
import xml.etree.ElementTree as ET

OUT = Path(__file__).resolve().parent
ROOT = OUT.parent
SRC = ROOT / 'ScanAnalysis201204021-20231225'


def read(p):
    raw = p.read_bytes()
    for enc in ('utf-8-sig', 'gbk'):
        try:
            return raw.decode(enc)
        except UnicodeDecodeError:
            pass
    raise ValueError('Unknown source encoding: ' + str(p))


def strip_comments(t):
    pattern = r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*[\s\S]*?\*/'
    return re.sub(pattern, lambda m: re.sub(r'[^\n]', ' ', m[0]) if m[0].startswith(('//', '/*')) else m[0], t)


def mask_strings(t):
    return re.sub(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', lambda m: re.sub(r'[^\n]', ' ', m[0]), t)


def line_at(t, pos):
    return t.count('\n', 0, pos) + 1


def loc(p, line):
    return {'file': p.relative_to(SRC).as_posix(), 'line': line}


source_files = sorted(p for p in SRC.rglob('*') if p.is_file() and p.suffix.lower() in {'.cpp', '.h', '.rc', '.rc2', '.vcproj', '.sln', '.def', '.idc'})
snapshot = {p.relative_to(SRC).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest() for p in source_files}
texts = {p: read(p) for p in source_files}
clean = {p: strip_comments(t) for p, t in texts.items()}
project = ET.fromstring(read(SRC / 'ScanAnalysis.vcproj').split('?>', 1)[-1])
project_files = {f.attrib['RelativePath'].replace('\\', '/').removeprefix('./') for f in project.iter('File')}
compiled_cpp = {str(Path(f).as_posix()).lower() for f in project_files if f.lower().endswith('.cpp')}
cpps = [p for p in source_files if p.suffix.lower() == '.cpp' and p.relative_to(SRC).as_posix().lower() in compiled_cpp]

defines = {}
for p in [SRC / 'resource.h', ROOT / 'dev-deps/vc80-trial/VC/atlmfc/include/afxres.h', ROOT / 'dev-deps/vc80-trial/VC/PlatformSDK/include/WinResrc.h']:
    if not p.exists():
        continue
    for m in re.finditer(r'^\s*#define\s+(\w+)\s+(-?0x[\da-fA-F]+|-?\d+|\w+)\b', strip_comments(read(p)), re.M):
        value = m[2]
        try:
            defines[m[1]] = int(value, 0)
        except ValueError:
            if value in defines:
                defines[m[1]] = defines[value]
aliases = defaultdict(list)
for k, v in defines.items():
    aliases[v].append(k)


def value_of(s):
    s = s.strip()
    if s in defines:
        return defines[s]
    try:
        return int(s, 0)
    except ValueError:
        return None


def args_of(s):
    return [a.strip() for a in re.findall(r'"(?:""|[^"\n])*"|[^,]+', s) if a.strip()]


rcpath = SRC / 'ScanAnalysis.rc'
rct = clean[rcpath]
resources = []
control_re = re.compile(r'^\s*(DEFPUSHBUTTON|PUSHBUTTON|LTEXT|CTEXT|RTEXT|EDITTEXT|COMBOBOX|LISTBOX|GROUPBOX|CONTROL|ICON|CHECKBOX|AUTOCHECKBOX|RADIOBUTTON|AUTORADIOBUTTON|SCROLLBAR)\s+(.+)$')
for match in re.finditer(r'^(\w+)\s+(MENU|TOOLBAR|ACCELERATORS|DIALOGEX|DIALOG)\b[^\n]*', rct, re.M):
    start = match.start()
    pre = rct[start:].splitlines()
    depth = 0
    began = False
    block = []
    for line in pre:
        block.append(line)
        if line.strip() == 'BEGIN':
            began = True
            depth += 1
        elif line.strip() == 'END':
            depth -= 1
            if began and depth == 0:
                break
    res = {'id': match[1], 'type': match[2], 'line': line_at(rct, start), 'entries': []}
    if res['type'] == 'MENU':
        stack = []
        pending = None
        for offset, line in enumerate(block):
            s = line.strip()
            pop = re.match(r'POPUP\s+"([^"]*)"', s)
            if pop:
                pending = pop[1]
            elif s == 'BEGIN':
                stack.append(pending)
                pending = None
            elif s == 'END':
                if stack:
                    stack.pop()
            else:
                m = re.match(r'MENUITEM\s+"([^"]*)"\s*,\s*(\w+)(.*)', s)
                if m:
                    res['entries'].append({'id': m[2], 'label': m[1], 'path': [x for x in stack if x], 'flags': m[3].strip(' ,'), 'line': res['line'] + offset})
    elif res['type'] == 'TOOLBAR':
        for offset, line in enumerate(block):
            m = re.match(r'\s*BUTTON\s+(\w+)', line)
            if m:
                res['entries'].append({'id': m[1], 'line': res['line'] + offset})
    elif res['type'] == 'ACCELERATORS':
        for offset, line in enumerate(block):
            s = line.strip()
            if s and s not in {'BEGIN', 'END'} and not s.startswith(res['id']):
                a = args_of(s)
                if len(a) >= 2:
                    res['entries'].append({'id': a[1], 'key': a[0], 'modifiers': a[2:], 'line': res['line'] + offset})
    else:
        res['caption'] = next((m[1] for line in block if (m := re.match(r'\s*CAPTION\s+"([^"]*)"', line))), '')
        logical = []
        for offset, line in enumerate(block):
            if control_re.match(line):
                logical.append([offset, line.strip()])
            elif logical and line.startswith(' ') and line.strip() and line.strip() not in {'BEGIN', 'END'}:
                logical[-1][1] += ' ' + line.strip()
        for offset, s in logical:
            m = control_re.match(s)
            a = args_of(m[2])
            quoted = a and a[0].startswith('"')
            cid = a[1] if (quoted or m[1] == 'ICON') and len(a) > 1 else a[0]
            res['entries'].append({'id': cid, 'kind': m[1], 'label': a[0].strip('"') if quoted else '', 'definition': s, 'line': res['line'] + offset})
    resources.append(res)

strings = {}
for m in re.finditer(r'^\s*(ID\w+)\s+"((?:""|[^"\n])*)"', rct, re.M):
    strings[m[1]] = m[2]

bindings, direct_dispatches, events, hooks, commented, functions = [], [], [], [], [], {}
for p in cpps:
    t = clean[p]
    masked = mask_strings(t)
    for m in re.finditer(r'\b(\w+)::(\w+)\s*\([^;{}]*\)\s*(?:const\s*)?\{', masked):
        opening = m.end() - 1
        depth = 1
        end = opening + 1
        while depth and end < len(masked):
            depth += (masked[end] == '{') - (masked[end] == '}')
            end += 1
        functions[(m[1], m[2], p.relative_to(SRC).as_posix())] = {'line': line_at(t, m.start()), 'end_line': line_at(t, end), 'body': t[opening + 1:end - 1].strip()}
    for mm in re.finditer(r'BEGIN_MESSAGE_MAP\s*\(\s*(\w+)\s*,\s*([^)]*)\)([\s\S]*?)END_MESSAGE_MAP\s*\(\s*\)', t):
        owner = mm[1]
        body = mm[3]
        for m in re.finditer(r'\b(ON_\w+)\s*\(([^)]*)\)', body):
            kind = m[1]
            a = [s.strip() for s in m[2].split(',')]
            record = {'class': owner, 'macro': kind, 'args': a, **loc(p, line_at(t, mm.start(3) + m.start()))}
            if kind in {'ON_COMMAND', 'ON_UPDATE_COMMAND_UI', 'ON_COMMAND_RANGE', 'ON_UPDATE_COMMAND_UI_RANGE', 'ON_COMMAND_EX', 'ON_COMMAND_EX_RANGE'}:
                record['id'] = a[0]
                record['handler'] = a[-1].lstrip('&').split('::')[-1]
                record['target_expression'] = a[-1]
                if 'RANGE' in kind:
                    record['end_id'] = a[1]
                fn = functions.get((owner, record['handler'], record['file']))
                record['implementation_line'] = fn['line'] if fn else None
                record['empty_body'] = not fn['body'] if fn else None
                record['function_found'] = fn is not None
                bindings.append(record)
            else:
                events.append(record)
    for mm in re.finditer(r'BEGIN_EVENTSINK_MAP\s*\(\s*(\w+)\s*,[^)]*\)([\s\S]*?)END_EVENTSINK_MAP\s*\(\s*\)', t):
        for line_match in re.finditer(r'^\s*(ON_EVENT\w*)\s*\((.*)', mm[2], re.M):
            events.append({'class': mm[1], 'macro': line_match[1], 'args': line_match[2].strip(), **loc(p, line_at(t, mm.start(2) + line_match.start(1)))})
    for m in re.finditer(r'\bCFileDialog\s+(\w+)\s*\(([\s\S]*?)\);', t):
        hooks.append({'kind': 'file_dialog', 'expression': m[1] + '(' + ' '.join(m[2].split()) + ')', **loc(p, line_at(t, m.start()))})
    for m in re.finditer(r'^.*(?:VK_[A-Z_]+|WM_KEYDOWN|strFilter\s*=|strFilter=|WM_USER_FULLPR).*$' , t, re.M):
        hooks.append({'kind': 'keyboard_or_filter', 'expression': m[0].strip(), **loc(p, line_at(t, m.start()))})
    for m in re.finditer(r'\b(LoadMenu|LoadToolBar|LoadAccelTable|LoadAccelerators|AppendMenu|InsertMenu|ModifyMenu|TrackPopupMenu|SendMessage|PostMessage|CFileDialog|PreTranslateMessage|OnLButtonDblClk|OnRButtonDown|OnContextMenu)\s*\(([^;\n]*)', t):
        hooks.append({'kind': m[1], 'expression': m[2][:450], **loc(p, line_at(t, m.start()))})
    for m in re.finditer(r'\bnCmd\s*==\s*(ID_\w+)', t):
        lineno = line_at(t, m.start())
        owners = [(key, fn) for key, fn in functions.items() if key[2] == p.relative_to(SRC).as_posix() and fn['line'] <= lineno <= fn['end_line']]
        if len(owners) != 1:
            raise RuntimeError('Direct command owner unresolved')
        (owner, name, _), fn = owners[0]
        direct_dispatches.append({'class': owner, 'macro': 'TPM_RETURNCMD/if', 'id': m[1], 'handler': name, 'implementation_line': fn['line'], 'function_found': True, 'empty_body': False, **loc(p, lineno)})
    for i, (original, active) in enumerate(zip(texts[p].splitlines(), t.splitlines()), 1):
        if original != active and re.search(r'ON_COMMAND(?:_RANGE)?\s*\(', original) and not re.search(r'ON_COMMAND(?:_RANGE)?\s*\(', active):
            commented.append({'text': original.strip(), **loc(p, i)})

class_dialogs = defaultdict(list)
for p in source_files:
    if p.suffix.lower() not in {'.h', '.cpp'}:
        continue
    t = clean[p]
    for m in re.finditer(r'class\s+(\w+)\s*:[^{]+\{([\s\S]*?)(?=\nclass\s|\Z)', t):
        for d in re.finditer(r'\bIDD\s*=\s*(IDD_\w+)', m[2]):
            class_dialogs[d[1]].append({'class': m[1], **loc(p, line_at(t, m.start(2) + d.start()))})
for res in resources:
    if res['type'] in {'DIALOG', 'DIALOGEX'}:
        res['classes'] = class_dialogs.get(res['id'], [])
        owner_names = {x['class'] for x in res['classes']}
        res['events'] = [e for e in events if e['class'] in owner_names]

commands = {}


def ensure(cid):
    num = value_of(cid)
    key = str(num) if num is not None else cid
    if key not in commands:
        commands[key] = {'key': key, 'numeric_id': num, 'ids': [], 'entries': [], 'handlers': [], 'updates': []}
    if cid not in commands[key]['ids']:
        commands[key]['ids'].append(cid)
    return commands[key]


for res in resources:
    if res['type'] in {'MENU', 'TOOLBAR', 'ACCELERATORS'}:
        for e in res['entries']:
            ensure(e['id'])['entries'].append({'resource': res['id'], 'type': res['type'], **e})
for b in bindings + direct_dispatches:
    if 'RANGE' not in b['macro']:
        c = ensure(b['id'])
        c['updates' if b['macro'].startswith('ON_UPDATE') else 'handlers'].append(b)
for b in bindings:
    if 'RANGE' in b['macro']:
        lo, hi = value_of(b['id']), value_of(b['end_id'])
        for c in list(commands.values()):
            if lo is not None and hi is not None and c['numeric_id'] is not None and lo <= c['numeric_id'] <= hi:
                c['updates' if b['macro'].startswith('ON_UPDATE') else 'handlers'].append(b)
for c in commands.values():
    c['aliases'] = aliases.get(c['numeric_id'], [])
    c['descriptions'] = list(dict.fromkeys(strings[cid] for cid in c['ids'] + c['aliases'] if cid in strings))
    c['runtime_status'] = '待实测'
    c['proposed_entry'] = '待界面方案确定；必须保留本命令的可达入口'
    c['static_status'] = '资源入口及活动处理绑定' if c['entries'] and c['handlers'] else '仅活动处理绑定，入口需追踪' if c['handlers'] else '仅资源入口，需核对框架路由或遗留失效项'

summary = {'source_files_hashed': len(snapshot), 'project_cpp_files': len(cpps), 'menus': sum(r['type'] == 'MENU' for r in resources), 'toolbars': sum(r['type'] == 'TOOLBAR' for r in resources), 'accelerator_tables': sum(r['type'] == 'ACCELERATORS' for r in resources), 'dialogs': sum(r['type'] in {'DIALOG', 'DIALOGEX'} for r in resources), 'menu_entries': sum(len(r['entries']) for r in resources if r['type'] == 'MENU'), 'toolbar_buttons': sum(len(r['entries']) for r in resources if r['type'] == 'TOOLBAR'), 'accelerators': sum(len(r['entries']) for r in resources if r['type'] == 'ACCELERATORS'), 'dialog_controls': sum(len(r['entries']) for r in resources if r['type'] in {'DIALOG', 'DIALOGEX'}), 'unique_commands': len(commands), 'command_bindings': len(bindings), 'direct_dispatch_branches': len(direct_dispatches), 'event_bindings': len(events), 'commented_command_lines': len(commented)}
inventory = {'date': '2026-11-06', 'source_root': str(SRC), 'method': '静态提取；去除注释；按命令数值合并别名；保留条件编译分支，未经预处理器求值；不是运行验收。', 'summary': summary, 'compiled_cpp': sorted(p.relative_to(SRC).as_posix() for p in cpps), 'resources': resources, 'commands': sorted(commands.values(), key=lambda c: (c['numeric_id'] is None, c['numeric_id'] or 0, c['key'])), 'bindings': bindings, 'direct_dispatches': direct_dispatches, 'events': events, 'hooks': hooks, 'commented_command_lines': commented}
OUT.mkdir(exist_ok=True)
(OUT / 'frontend-inventory.json').write_text(json.dumps(inventory, ensure_ascii=False, indent=2), encoding='utf-8')
(OUT / 'source-sha256.json').write_text(json.dumps(snapshot, ensure_ascii=False, indent=2), encoding='utf-8')
for p in source_files:
    if hashlib.sha256(p.read_bytes()).hexdigest() != snapshot[p.relative_to(SRC).as_posix()]:
        raise RuntimeError('Source changed during inventory: ' + str(p))
print(json.dumps(summary, ensure_ascii=False, indent=2))
print('SOURCE_UNCHANGED')
