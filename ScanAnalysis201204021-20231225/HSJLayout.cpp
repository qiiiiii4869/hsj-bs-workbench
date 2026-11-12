// HSJ_SOURCE_BUILD links typed MFC calls into a rebuilt executable. The other
// mode remains exclusively for the checksum-pinned original binary helper.
#ifdef HSJ_SOURCE_BUILD
#include "stdafx.h"
#include "resource.h"
#include "MainFrm.h"
#else
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#include <commctrl.h>
#include "HSJLayoutModel.h"

static const wchar_t *StateKey = L"HSJ.Analysis.LayoutState.20261029";
static const UINT_PTR SubclassId = 0x48534A32;
struct ViewState {
    void *object;
    int kind, x, y, headerX, headerY, contentWidth, contentHeight;
    BOOL busy, ready;
};

// NI ActiveX graphs can be windowless. Win32 GetDlgItem/SetWindowPos cannot
// address those controls, whereas MFC's CWnd methods delegate to the OLE site.
// These two pre-existing thunks are pinned by the patcher's full-image checksum.
typedef void *(__thiscall *MfcGetControl)(void *, int);
typedef void (__thiscall *MfcMoveControl)(void *, int, int, int, int, BOOL);
static void *Control(ViewState *state, int id) {
#ifdef HSJ_SOURCE_BUILD
    return static_cast<CWnd *>(state->object)->GetDlgItem(id);
#else
    return ((MfcGetControl)0x49486A)(state->object, id);
#endif
}

static UINT Dpi(HWND window) {
    typedef UINT (WINAPI *GetDpi)(HWND);
    GetDpi get = (GetDpi)GetProcAddress(GetModuleHandleW(L"user32.dll"), "GetDpiForWindow");
    return get ? get(window) : 96;
}
static bool IsPlot(int kind, int id) {
    return kind == 0 ? (id == 1028 || id == 7030 || id == 1001 || id == 1052)
                     : (id == 1072 || id == 1073 || id == 1071 || id == 1074);
}
struct Shift { HWND view; ViewState *state; int dx, dy; };
static BOOL CALLBACK ShiftHeader(HWND child, LPARAM value) {
    Shift *shift = (Shift *)value;
    if (GetParent(child) != shift->view || IsPlot(shift->state->kind, GetDlgCtrlID(child))) return TRUE;
    RECT rect;
    GetWindowRect(child, &rect);
    MapWindowPoints(NULL, shift->view, (POINT *)&rect, 2);
    SetWindowPos(child, NULL, rect.left + shift->dx, rect.top + shift->dy, 0, 0,
                 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    return TRUE;
}
static void Place(HWND view, int id, const HSJRect &rect, const ViewState *state) {
    (void)view;
    void *control = Control((ViewState *)state, id);
    if (control) {
#ifdef HSJ_SOURCE_BUILD
        static_cast<CWnd *>(control)->MoveWindow(rect.x - state->x, rect.y - state->y,
                                               rect.width, rect.height, TRUE);
#else
        ((MfcMoveControl)0x49487C)(control, rect.x - state->x, rect.y - state->y,
                                 rect.width, rect.height, TRUE);
#endif
    }
}
static void Arrange(HWND view, ViewState *state) {
    if (state->busy || !IsWindow(view)) return;
    if (!Control(state, state->kind == 0 ? 1028 : 1072)) return;
    state->busy = TRUE;
    state->ready = TRUE;
    HSJLayoutGeometry g = {};
    RECT client = {};
    // Scrollbar changes may shrink the client area; settle both axes before placement.
    for (int pass = 0; pass < 3; ++pass) {
        GetClientRect(view, &client);
        g = HSJComputeLayout(state->kind, client.right, client.bottom, Dpi(view));
        SCROLLINFO horizontal = {sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE | SIF_POS};
        horizontal.nMax = g.width - 1; horizontal.nPage = client.right; horizontal.nPos = state->x;
        SetScrollInfo(view, SB_HORZ, &horizontal, TRUE);
        SCROLLINFO vertical = {sizeof(SCROLLINFO), SIF_RANGE | SIF_PAGE | SIF_POS};
        vertical.nMax = g.height - 1; vertical.nPage = client.bottom; vertical.nPos = state->y;
        SetScrollInfo(view, SB_VERT, &vertical, TRUE);
    }
    state->x = GetScrollPos(view, SB_HORZ);
    state->y = GetScrollPos(view, SB_VERT);
    state->contentWidth = g.width; state->contentHeight = g.height;
    Shift shift = {view, state, state->headerX - state->x, state->headerY - state->y};
    if (shift.dx || shift.dy) EnumChildWindows(view, ShiftHeader, (LPARAM)&shift);
    state->headerX = state->x; state->headerY = state->y;
    if (state->kind == 0) {
        Place(view, 1028, g.first, state);
        Place(view, 7030, g.second, state);
        Place(view, 1001, g.right, state);
        Place(view, 1052, g.table, state);
    } else {
        Place(view, 1072, g.first, state);
        Place(view, 1073, g.second, state);
        Place(view, 1071, g.right, state);
        Place(view, 1074, g.title, state);
    }
    state->busy = FALSE;
    InvalidateRect(view, NULL, TRUE);
}
static void Scroll(HWND view, ViewState *state, int bar, int request, int wheel = 0) {
    SCROLLINFO info = {sizeof(SCROLLINFO), SIF_ALL};
    GetScrollInfo(view, bar, &info);
    int value = info.nPos;
    int line = 32 * Dpi(view) / 96;
    switch (request) {
    case SB_TOP: value = info.nMin; break;
    case SB_BOTTOM: value = info.nMax; break;
    case SB_LINEUP: value -= line; break;
    case SB_LINEDOWN: value += line; break;
    case SB_PAGEUP: value -= info.nPage; break;
    case SB_PAGEDOWN: value += info.nPage; break;
    case SB_THUMBTRACK: case SB_THUMBPOSITION: value = info.nTrackPos; break;
    default: value -= wheel * line / WHEEL_DELTA; break;
    }
    info.fMask = SIF_POS;
    info.nPos = HSJMax(info.nMin, HSJMin(value, HSJMax(info.nMin, info.nMax - (int)info.nPage + 1)));
    SetScrollInfo(view, bar, &info, TRUE);
    int next = GetScrollPos(view, bar);
    if (bar == SB_HORZ) state->x = next; else state->y = next;
    Arrange(view, state);
}
static LRESULT CALLBACK ViewSubclass(HWND view, UINT message, WPARAM wParam, LPARAM lParam,
                                    UINT_PTR, DWORD_PTR reference) {
    ViewState *state = (ViewState *)reference;
    if (message == WM_NCDESTROY) {
        RemoveWindowSubclass(view, ViewSubclass, SubclassId);
        RemovePropW(view, StateKey);
        HeapFree(GetProcessHeap(), 0, state);
        return DefSubclassProc(view, message, wParam, lParam);
    }
    if (message == WM_VSCROLL && !lParam) { Scroll(view, state, SB_VERT, LOWORD(wParam)); return 0; }
    if (message == WM_HSCROLL && !lParam) { Scroll(view, state, SB_HORZ, LOWORD(wParam)); return 0; }
    if (message == WM_MOUSEWHEEL && !(GET_KEYSTATE_WPARAM(wParam) & MK_CONTROL)) {
        Scroll(view, state, (GET_KEYSTATE_WPARAM(wParam) & MK_SHIFT) ? SB_HORZ : SB_VERT,
               -1, GET_WHEEL_DELTA_WPARAM(wParam));
        return 0;
    }
    LRESULT result = DefSubclassProc(view, message, wParam, lParam);
    if (message == WM_SIZE && !state->busy) Arrange(view, state);
    // The first WM_SIZE can precede creation of the form's ActiveX children.
    if (message == WM_PAINT && !state->ready && !state->busy) Arrange(view, state);
    return result;
}

extern "C" BOOL WINAPI LayoutView(void *viewObject, int kind, int width, int height) {
    if (!viewObject) return FALSE;
#ifdef HSJ_SOURCE_BUILD
    HWND view = static_cast<CWnd *>(viewObject)->GetSafeHwnd();
#else
    HWND view = *(HWND *)((BYTE *)viewObject + 0x20);
#endif
    if (!IsWindow(view) || width <= 0 || height <= 0) return TRUE;
    ViewState *state = (ViewState *)GetPropW(view, StateKey);
    if (!state) {
        state = (ViewState *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ViewState));
        if (!state) return FALSE;
        state->object = viewObject;
        state->kind = kind;
        if (!SetPropW(view, StateKey, state) || !SetWindowSubclass(view, ViewSubclass, SubclassId, (DWORD_PTR)state)) {
            RemovePropW(view, StateKey); HeapFree(GetProcessHeap(), 0, state); return FALSE;
        }
    }
    Arrange(view, state);
    return TRUE;
}

#ifdef HSJ_SOURCE_BUILD
extern "C" BOOL WINAPI NormalizeFrame(void *frameObject) {
    CMainFrame *frame = static_cast<CMainFrame *>(frameObject);
    if (!frame || !IsWindow(frame->GetSafeHwnd())) return FALSE;
    HWND window = frame->GetSafeHwnd();
    HWND mdi = FindWindowExW(window, NULL, L"MDIClient", NULL);
    if (!mdi) return FALSE;
    HKEY settings = NULL;
    DWORD version = 0, bytes = sizeof(version), type = 0;
    const wchar_t *path = L"Software\\HUST-CXY_SJY\\ScanAnalysis";
    if (RegOpenKeyExW(HKEY_CURRENT_USER, path, 0, KEY_QUERY_VALUE, &settings) == ERROR_SUCCESS) {
        RegQueryValueExW(settings, L"CompactLayoutVersion", NULL, &type, (BYTE *)&version, &bytes);
        RegCloseKey(settings);
    }
    RECT mdiRect, toolbarRect, menuRect;
    GetWindowRect(mdi, &mdiRect);
    MapWindowPoints(NULL, window, (POINT *)&mdiRect, 2);
    frame->m_wndChannelsBATBarFile.GetWindowRect(&toolbarRect);
    frame->m_wndMenuBar.GetWindowRect(&menuRect);
    int barHeight = HSJMax(40, toolbarRect.bottom - toolbarRect.top);
    bool badTop = mdiRect.top > (menuRect.bottom - menuRect.top) + 3 * barHeight + 24;
    if (version == 20261029 && !badTop) return TRUE;
    frame->ShowControlBar(&frame->m_wndOutput, FALSE, FALSE, TRUE);
    CBCGPTabbedToolbar *bars[5] = {&frame->m_wndChannelsBATBarFile,
        &frame->m_wndTabbedToolBarGrap, &frame->m_wndTabbedToolBarChanl,
        &frame->m_wndTabbedToolBarExamGrap, &frame->m_wndTabbedToolBarExamAnsy};
    for (int i = 0; i < 5; ++i) bars[i]->UnDockControlBar(FALSE);
    frame->m_wndMenuBar.UnDockControlBar(FALSE);
    frame->RecalcLayout(TRUE);
    frame->DockControlBar(&frame->m_wndMenuBar, AFX_IDW_DOCKBAR_TOP);
    frame->DockControlBar(bars[4], AFX_IDW_DOCKBAR_TOP);
    for (int i = 3; i >= 0; --i) frame->DockControlBarLeftOf(bars[i], bars[i + 1]);
    frame->RecalcLayout(TRUE);
    RedrawWindow(window, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
    if (RegCreateKeyExW(HKEY_CURRENT_USER, path, 0, NULL, 0, KEY_SET_VALUE, NULL, &settings, NULL) == ERROR_SUCCESS) {
        version = 20261029;
        RegSetValueExW(settings, L"CompactLayoutVersion", 0, REG_DWORD, (BYTE *)&version, sizeof(version));
        RegCloseKey(settings);
    }
    return TRUE;
}
#else
// These offsets are pinned to the SHA256-checked original VC80 build. The helper
// calls the vendor's public exported methods so the docking manager stays in sync.
static void *Member(void *frame, int offset) { return (BYTE *)frame + offset; }
static HWND ObjectWindow(void *object) { return *(HWND *)((BYTE *)object + 0x20); }
extern "C" BOOL WINAPI NormalizeFrame(void *frame) {
    if (!frame || !IsWindow(ObjectWindow(frame))) return FALSE;
    HMODULE bcg = GetModuleHandleW(L"BCGCBPRO10080.dll");
    typedef void (__thiscall *Dock)(void *, void *, UINT, const RECT *);
    typedef BOOL (__thiscall *LeftOf)(void *, void *, void *);
    typedef void (__thiscall *Undock)(void *, BOOL);
    typedef void (__thiscall *Show)(void *, void *, BOOL, BOOL, BOOL);
    typedef void (__thiscall *Recalc)(void *, BOOL);
    Dock dock = (Dock)GetProcAddress(bcg, "?DockControlBar@CBCGPMDIFrameWnd@@QAEXPAVCBCGPBaseControlBar@@IPBUtagRECT@@@Z");
    LeftOf leftOf = (LeftOf)GetProcAddress(bcg, "?DockControlBarLeftOf@CBCGPMDIFrameWnd@@QAEHPAVCBCGPControlBar@@0@Z");
    Undock undock = (Undock)GetProcAddress(bcg, "?UnDockControlBar@CBCGPControlBar@@UAEXH@Z");
    Show show = (Show)GetProcAddress(bcg, "?ShowControlBar@CBCGPMDIFrameWnd@@QAEXPAVCBCGPBaseControlBar@@HHH@Z");
    Recalc recalc = (Recalc)GetProcAddress(bcg, "?RecalcLayout@CBCGPMDIFrameWnd@@UAEXH@Z");
    if (!dock || !leftOf || !undock || !show || !recalc) return FALSE;
    HWND window = ObjectWindow(frame);
    HWND mdi = FindWindowExW(window, NULL, L"MDIClient", NULL);
    if (!mdi) return FALSE;
    HKEY settings = NULL;
    DWORD version = 0, bytes = sizeof(version), type = 0;
    const wchar_t *path = L"Software\\HUST-CXY_SJY\\ScanAnalysis";
    if (RegOpenKeyExW(HKEY_CURRENT_USER, path, 0, KEY_QUERY_VALUE, &settings) == ERROR_SUCCESS) {
        RegQueryValueExW(settings, L"CompactLayoutVersion", NULL, &type, (BYTE *)&version, &bytes);
        RegCloseKey(settings);
    }
    RECT client, mdiRect, toolbarRect, menuRect;
    GetClientRect(window, &client);
    GetWindowRect(mdi, &mdiRect);
    MapWindowPoints(NULL, window, (POINT *)&mdiRect, 2);
    GetWindowRect(ObjectWindow(Member(frame, 0xDE10)), &toolbarRect);
    GetWindowRect(ObjectWindow(Member(frame, 0x2D5C)), &menuRect);
    int barHeight = HSJMax(40, toolbarRect.bottom - toolbarRect.top);
    // Preserve normal later customizations; repair a stale excessive top docking area.
    bool badTop = mdiRect.top > (menuRect.bottom - menuRect.top) + 3 * barHeight + 24;
    if (version == 20261029 && !badTop) return TRUE;
    show(frame, Member(frame, 0x3DC4), FALSE, FALSE, TRUE);
    const int offsets[5] = {0xDE10, 0xEA20, 0xF630, 0x10240, 0x10E50};
    for (int i = 0; i < 5; ++i) undock(Member(frame, offsets[i]), FALSE);
    undock(Member(frame, 0x2D5C), FALSE);
    recalc(frame, TRUE);
    dock(frame, Member(frame, 0x2D5C), 0xE81B, NULL);
    dock(frame, Member(frame, offsets[4]), 0xE81B, NULL);
    for (int i = 3; i >= 0; --i) leftOf(frame, Member(frame, offsets[i]), Member(frame, offsets[i + 1]));
    recalc(frame, TRUE);
    RedrawWindow(window, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
    if (RegCreateKeyExW(HKEY_CURRENT_USER, path, 0, NULL, 0, KEY_SET_VALUE, NULL, &settings, NULL) == ERROR_SUCCESS) {
        version = 20261029;
        RegSetValueExW(settings, L"CompactLayoutVersion", 0, REG_DWORD, (BYTE *)&version, sizeof(version));
        RegCloseKey(settings);
    }
    return TRUE;
}
#endif
