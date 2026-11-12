"""One-time extension of the first-page skin; preserves resource geometry."""
from pathlib import Path
import re
import json

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / 'ScanAnalysis201204021-20231225'
changed = []


def load(p):
    raw = p.read_bytes()
    enc = 'utf-8-sig' if raw.startswith(b'\xef\xbb\xbf') else 'gbk'
    return raw.decode(enc).replace('\r\n', '\n'), enc


def save(p, t, enc):
    p.write_bytes(t.replace('\n', '\r\n').encode(enc))
    changed.append(p.relative_to(SRC).as_posix())


def once(t, old, new):
    if t.count(old) != 1:
        raise ValueError('Expected unique anchor: ' + old)
    return t.replace(old, new, 1)


p = SRC / 'stdafx.h'; t, enc = load(p)
t = once(t, '#include "NiIncludes.h"', '#include "NiIncludes.h"\n#include "HSJDialogAppearance.h"')
save(p, t, enc)

# Explicitly replace the application parameter-dialog base, constructors,
# base calls and map declarations. Windows common CFileDialog is untouched.
dialog_classes = []
for p in sorted(SRC.rglob('*')):
    if p.suffix.lower() not in {'.cpp', '.h'} or p.name.startswith('HSJ'):
        continue
    t, enc = load(p)
    if not re.search(r'\bCDialog\b', t):
        continue
    dialog_classes += re.findall(r'class\s+(\w+)\s*:\s*public\s+CDialog\b', t)
    t = re.sub(r'\bCDialog\b', 'CHSJDialog', t)
    save(p, t, enc)

p = SRC / 'MainFrm.cpp'; t, enc = load(p)
t = once(t, 'IMPLEMENT_DYNCREATE(CHSJVisualManager, CBCGPVisualManagerXP)', '''IMPLEMENT_DYNCREATE(CHSJVisualManager, CBCGPVisualManagerXP)
IMPLEMENT_DYNAMIC(CHSJDialog, CDialog)
BEGIN_MESSAGE_MAP(CHSJDialog, CDialog)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()''')
save(p, t, enc)

views = {
    'AxisOrbit': ('CAxisOrbit', 'CFormView', ['m_GraphX', 'm_GraphY', 'm_GraphAxis']),
    'Correlation': ('CCorrelation', 'CFormView', ['m_GraphX', 'm_GraphY', 'm_GraphAxis']),
    'TrendView': ('CTrendView', 'CBCGPFormView', ['m_TrendGraph']),
    'Ni3DWaveView': ('CNi3DWaveView', 'CBCGPFormView', []),
    'DynamicEquilibrium': ('CDynamicEquilibrium', 'CFormView', ['m_Graph', 'm_newGraph', 'm_DynEquGraph']),
    'JiggerView': ('CJiggerView', 'CFormView', ['m_Graph', 'm_NewGraph']),
    'CamView': ('CCamView', 'CFormView', ['m_Graph']),
}
for name, (owner, parent, graphs) in views.items():
    p = SRC / (name + '.h'); t, enc = load(p)
    marker = re.search(r'DECLARE_DYNCREATE\s*\(\s*' + owner + r'\s*\)', t)
    if not marker:
        raise ValueError('Missing view declaration: ' + name)
    t = t[:marker.end()] + '\n\tCHSJFontStore m_skinFonts; // Child typefaces only; original font sizes and bounds.\n' + t[marker.end():]
    save(p, t, enc)
    p = SRC / (name + '.cpp'); t, enc = load(p)
    pattern = r'(void\s+' + owner + r'::OnInitialUpdate\(\)[\s\S]*?\b' + parent + r'::OnInitialUpdate\(\);)'
    calls = '\n\tHSJStyleChildControls(GetSafeHwnd(), m_skinFonts);\n'
    calls += ''.join('\tHSJStyleGraph(' + g + ');\n' for g in graphs)
    if name == 'Ni3DWaveView': calls += '\tHSJStyleGraph3D(m_graph);\n'
    t, n = re.subn(pattern, lambda m: m[1] + calls, t, count=1)
    if n != 1: raise ValueError('Missing view init: ' + name)
    pattern = r'HBRUSH\s+' + owner + r'::OnCtlColor\([^)]*\)\s*\{[\s\S]*?\n\}'
    replacement = f'''HBRUSH {owner}::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{{
    if (HSJSetWhiteControlColor(pDC, pWnd, nCtlColor)) return HSJWhiteBrush();
    return {parent}::OnCtlColor(pDC, pWnd, nCtlColor);
}}'''
    t, n = re.subn(pattern, lambda m: replacement, t, count=1)
    if n != 1: raise ValueError('Missing view color handler: ' + name)
    save(p, t, enc)

# This parameter dialog contains an NI preview plot in addition to standard fields.
p = SRC / 'T3DFreqDataSet.cpp'; t, enc = load(p)
t = once(t, '\tCHSJDialog::OnInitDialog();', '\tCHSJDialog::OnInitDialog();\n\tHSJStyleGraph(m_Graph);')
save(p, t, enc)

# Long-chart GDI drawing: keep dimensions, coordinates, line widths and user colors.
p = SRC / 'StripChart/boxingView.cpp'; t, enc = load(p)
t = t.replace('font1.CreatePointFont(110, "Arial");', 'font1.CreatePointFont(110, _T("Microsoft YaHei UI"));')
t = t.replace('font.CreatePointFont(180, "Arial");', 'font.CreatePointFont(180, _T("Microsoft YaHei UI"));')
t = once(t, 'CPen dotpen(PS_DOT, 0, RGB(0, 0, 0));', 'CPen dotpen(PS_SOLID, 1, RGB(232, 237, 243));')
t = once(t, '\tdcMem.Rectangle(0, 0, m_ViewPortExt.cx, m_ViewPortExt.cy);', '\tdcMem.Draw3dRect(0, 0, m_ViewPortExt.cx, m_ViewPortExt.cy, HSJBorder(), HSJBorder());')
t = t.replace('dcMem.SetTextColor(RGB(0, 0, 0));', 'dcMem.SetTextColor(HSJInk());')
t = once(t, '\tdcMem.SelectObject(poldpen);\n\tdcMem.MoveTo(60, 70);', '\tCPen axisPen(PS_SOLID, 1, HSJBorder());\n\tdcMem.SelectObject(&axisPen);\n\tdcMem.MoveTo(60, 70);')
t = once(t, '\tdcMem.SelectObject(poldpen);\n\tdcMem.SetTextAlign(TA_RIGHT);', '\tdcMem.SelectObject(&axisPen);\n\tdcMem.SetTextAlign(TA_RIGHT);')
# Restore the original GDI pen before local pens leave scope.
t = once(t, '\tdcMem.DeleteDC();\n\tbmp.DeleteObject();', '\tdcMem.SelectObject(poldpen);\n\tdcMem.DeleteDC();\n\tbmp.DeleteObject();')
save(p, t, enc)

# Owner-drawn batch lists retain their category/checkbox positions and handlers.
p = SRC / 'CatListBox.cpp'; t, enc = load(p)
t = once(t, 'GetSysColor( bHighlight ? COLOR_HIGHLIGHT : COLOR_WINDOW )', '(bHighlight ? RGB(232, 241, 253) : RGB(255, 255, 255))')
t = t.replace('GetSysColor( COLOR_3DFACE )', 'HSJSurface()')
t = t.replace('GetSysColor( COLOR_HIGHLIGHTTEXT )', 'HSJAccent()')
t = t.replace('GetSysColor( COLOR_WINDOWTEXT )', 'HSJInk()')
t = t.replace('pDC->DrawFrameControl( rcButton, DFC_BUTTON, DFCS_BUTTONPUSH );', 'pDC->Draw3dRect(rcButton, HSJBorder(), HSJBorder());')
save(p, t, enc)

out = ROOT / 'frontend-review/all-page-skin-scope.json'
out.write_text(json.dumps({'dialog_classes':sorted(set(dialog_classes)), 'dialog_count':len(set(dialog_classes)), 'analysis_views':list(views), 'changed_files':sorted(set(changed)), 'runtime_testing':'Not performed, per user instruction'},ensure_ascii=False,indent=2),encoding='utf-8')
print('Dialog classes:', len(set(dialog_classes)))
print('Analysis views:', ', '.join(views))
print('Changed files:', len(set(changed)))
