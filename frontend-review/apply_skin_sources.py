"""Apply the reviewed visual changes, preserving legacy GBK and CRLF."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / 'ScanAnalysis201204021-20231225'


def edit(name, transform):
    p = SRC / name
    t = p.read_bytes().decode('gbk')
    newline = '\r\n' if '\r\n' in t else '\n'
    t = t.replace('\r\n', '\n')
    result = transform(t)
    p.write_bytes(result.replace('\n', newline).encode('gbk'))


def replace_once(t, old, new):
    if t.count(old) != 1:
        raise ValueError('Expected unique source anchor: ' + old[:100])
    return t.replace(old, new, 1)


def mainframe(t):
    t = replace_once(t, '#include "MainFrm.h"', '#include "MainFrm.h"\n#include "HSJAppearance.h"\n\nIMPLEMENT_DYNCREATE(CHSJVisualManager, CBCGPVisualManagerXP)')
    t = replace_once(t, 'm_nAppLook = ID_VIEW_APPLOOK_XP;', 'm_nAppLook = ID_VIEW_APPLOOK_MODERN;')
    t = replace_once(t, 'ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)', 'ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)\n\tON_COMMAND_RANGE(ID_VIEW_APPLOOK_MODERN, ID_VIEW_APPLOOK_MODERN, OnAppLook)\n\tON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_MODERN, ID_VIEW_APPLOOK_MODERN, OnUpdateAppLook)')
    t = replace_once(t, '\tOnAppLook (m_nAppLook);', '\t// Preserve the existing menu metrics while changing its typeface.\n\tLOGFONT uiFont = {};\n\tglobalData.fontRegular.GetLogFont(&uiFont);\n\tlstrcpyn(uiFont.lfFaceName, _T("Microsoft YaHei UI"), LF_FACESIZE);\n\tuiFont.lfQuality = CLEARTYPE_QUALITY;\n\tCBCGPMenuBar::SetMenuFont(&uiFont);\n\tOnAppLook (m_nAppLook);')
    anchor='\tswitch (m_nAppLook)\n\t{'
    t = replace_once(t, anchor, anchor+'\n\tcase ID_VIEW_APPLOOK_MODERN:\n\t\tCBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CHSJVisualManager));\n\t\tbreak;\n')
    return t


edit('MainFrm.cpp', mainframe)
edit('resource.h', lambda t: replace_once(t, '#define ID_WINDOW_SET                   33200', '#define ID_WINDOW_SET                   33200\n#define ID_VIEW_APPLOOK_MODERN           33201').replace('#define _APS_NEXT_COMMAND_VALUE         33201', '#define _APS_NEXT_COMMAND_VALUE         33202'))
edit('ScanAnalysis.rc', lambda t: replace_once(t, 'MENUITEM "Office &XP",', 'MENUITEM "浅色扁平（默认）",                  ID_VIEW_APPLOOK_MODERN\n            MENUITEM "Office &XP",'))


def workspace(t):
    t = replace_once(t, '#include "WorkSpaceBar.h"', '#include "WorkSpaceBar.h"\n#include "HSJAppearance.h"')
    anchor='\t// Setup trees content:'
    t = replace_once(t, anchor, '\tconst int originalItemHeight = m_wndTree.GetItemHeight();\n\tm_wndTree.SetFont(HSJUIFont());\n\tm_wndTree.SetItemHeight(originalItemHeight);\n\tm_wndTree.SetBkColor(RGB(255, 255, 255));\n\tm_wndTree.SetTextColor(HSJInk());\n\tm_wndTree.SetLineColor(HSJBorder());\n\n'+anchor)
    t = replace_once(t, '::GetSysColor (COLOR_3DSHADOW), \n\t\t\t\t\t\t\t\t::GetSysColor (COLOR_3DSHADOW)', 'HSJBorder(), HSJBorder()')
    return t


edit('WorkSpaceBar.cpp', workspace)
edit('CustomTreeCtrl.h', lambda t: replace_once(t, '\tDECLARE_MESSAGE_MAP()', '\tDECLARE_MESSAGE_MAP()\n\tafx_msg void OnSkinCustomDraw(NMHDR* header, LRESULT* result);'))


def tree(t):
    t = replace_once(t, '#include "CustomTreeCtrl.h"', '#include "CustomTreeCtrl.h"\n#include "HSJAppearance.h"')
    t = replace_once(t, 'BEGIN_MESSAGE_MAP(CCustomTreeCtrl, CTreeCtrl)', 'BEGIN_MESSAGE_MAP(CCustomTreeCtrl, CTreeCtrl)\n\tON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnSkinCustomDraw)')
    t += '''
// Drawing only; selection, keyboard navigation and item hit testing stay native.
void CCustomTreeCtrl::OnSkinCustomDraw(NMHDR* header, LRESULT* result)
{
    NMTVCUSTOMDRAW* draw = reinterpret_cast<NMTVCUSTOMDRAW*>(header);
    if (draw->nmcd.dwDrawStage == CDDS_PREPAINT) {
        *result = CDRF_NOTIFYITEMDRAW;
    } else if (draw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
        const BOOL selected = (HTREEITEM)draw->nmcd.dwItemSpec == GetSelectedItem();
        draw->clrText = selected ? HSJAccent() : HSJInk();
        draw->clrTextBk = selected ? RGB(232, 241, 253) : RGB(255, 255, 255);
        if (selected) draw->nmcd.uItemState &= ~(CDIS_SELECTED | CDIS_FOCUS);
        *result = CDRF_DODEFAULT;
    } else {
        *result = CDRF_DODEFAULT;
    }
}
'''
    return t


edit('CustomTreeCtrl.cpp', tree)


def wave(t):
    t = replace_once(t, '#include "NiWaveFraqView.h"', '#include "NiWaveFraqView.h"\n#include "HSJAppearance.h"')
    t = replace_once(t, '\tInitNiGraph();', '\tHSJStyleGraph(m_Graph);\n\tHSJStyleGraph(m_newGraph);\n\tInitNiGraph();')
    # Default signal blue only; user-selected/additional-channel colors are kept.
    t = t.replace('CNiColor blue(0,0,255);', 'CNiColor blue(40,104,197);')
    t = t.replace('fnt.CreatePointFont(100,font.Name);', 'fnt.CreatePointFont(100,_T("Microsoft YaHei UI"));')
    start = t.index('HBRUSH CNiWaveFraqView::OnCtlColor(')
    end = t.index('\n}', start) + 2
    t = t[:start]+'''HBRUSH CNiWaveFraqView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_BTN) {
        pDC->SetTextColor(HSJInk());
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetBkMode(TRANSPARENT);
        return HSJWhiteBrush();
    }
    return CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}'''+t[end:]
    # Keep every TextOut coordinate/format and all numeric computations untouched.
    start = t.index('void CNiWaveFraqView::TextData()')
    end = t.index('\n}', start) + 2
    body = t[start:end].replace('RGB(192,192,192)', 'HSJBorder()').replace('RGB(70,70,70)', 'HSJInk()')
    body = body.replace('CDC* pDCW=pWndW->GetDC();', 'CDC* pDCW=pWndW->GetDC();\n\tpDCW->SetTextColor(HSJInk());\n\tpDCW->SetBkColor(RGB(255, 255, 255));')
    body = body.replace('CDC* pDCF=pWndF->GetDC();', 'CDC* pDCF=pWndF->GetDC();\n\tpDCF->SetBkColor(RGB(255, 255, 255));')
    t = t[:start]+body+t[end:]
    return t


edit('niwavefraqview.cpp', wave)


def output(t):
    t = replace_once(t, '#include "OutputView.h"', '#include "OutputView.h"\n#include "HSJAppearance.h"')
    t = replace_once(t, '\tm_Font.CreateStockObject (DEFAULT_GUI_FONT);', '\tLOGFONT font = {};\n\tHSJUIFont()->GetLogFont(&font);\n\tm_Font.CreateFontIndirect(&font);')
    t = t.replace('::GetSysColor (COLOR_3DSHADOW)', 'HSJBorder()')
    return t


edit('OutputView.cpp', output)
print('Applied GBK/CRLF-preserving visual changes.')
