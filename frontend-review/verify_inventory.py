"""Verify inventory coverage and its links against the read-only source tree."""
import hashlib
import json
import re
from pathlib import Path

out = Path(__file__).resolve().parent
j = json.loads((out / 'frontend-inventory.json').read_text(encoding='utf-8'))
src = Path(j['source_root'])
cache = {}
for record in j['bindings'] + j['events'] + j['direct_dispatches']:
    file = record['file']
    if file not in cache:
        raw = (src / file).read_bytes()
        try:
            cache[file] = raw.decode('utf-8-sig').splitlines()
        except UnicodeDecodeError:
            cache[file] = raw.decode('gbk').splitlines()
    text = cache[file][record['line'] - 1]
    if record['macro'] == 'TPM_RETURNCMD/if':
        assert record['id'] in text and 'nCmd' in text, record
    else:
        assert record['macro'] in text, record

commands = j['commands']
assert len({c['key'] for c in commands}) == j['summary']['unique_commands']
assert all(c['proposed_entry'] and c['review_status'] and c['review_id'] for c in commands)
assert sum(not c['handlers'] for c in commands) == 4
assert len(j['direct_dispatches']) == 8
assert sum(e['macro'] == 'ON_EVENT' for e in j['events']) == 18
assert all(r.get('classes') for r in j['resources'] if r['type'].startswith('DIALOG'))
print('PASS: binding line references, unique command IDs, retention columns, popup branches, graph events, dialog owners')

main = (out / '功能保留清单.md').read_text(encoding='utf-8')
appendix = (out / '功能入口与源码证据.md').read_text(encoding='utf-8')
assert len(re.findall(r'^\| CMD-', appendix, re.M)) == 139
assert len(re.findall(r'^### DLG-', appendix, re.M)) == 45
assert len(re.findall(r'^\| EVT-', appendix, re.M)) == 276
assert '\ufffd' not in main + appendix
for file, line in re.findall(r'\]\(([^)]+):(\d+)\)', appendix):
    path = Path(file)
    assert path.is_file(), path
    assert 0 < int(line) <= len(path.read_bytes().splitlines()), (file, line)
print('PASS: document coverage, encoding and source links')

snapshot = json.loads((out / 'source-sha256.json').read_text(encoding='utf-8'))
assert all(hashlib.sha256((src / f).read_bytes()).hexdigest() == sha for f, sha in snapshot.items())
print('PASS: all 135 source files unchanged')
