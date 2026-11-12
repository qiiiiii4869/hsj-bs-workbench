// Visual-only helpers. No window geometry or analysis settings are changed here.
#pragma once
#include "BCGPVisualManagerXP.h"

inline COLORREF HSJInk() { return RGB(38, 51, 69); }
inline COLORREF HSJAccent() { return RGB(40, 104, 197); }
inline COLORREF HSJBorder() { return RGB(221, 228, 236); }
inline COLORREF HSJSurface() { return RGB(245, 247, 250); }

inline CFont* HSJUIFont() {
    static CFont font;
    if (!font.GetSafeHandle()) {
        LOGFONT lf = {};
        CFont* original = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
        original->GetLogFont(&lf);
        lstrcpyn(lf.lfFaceName, _T("Microsoft YaHei UI"), LF_FACESIZE);
        lf.lfQuality = CLEARTYPE_QUALITY;
        font.CreateFontIndirect(&lf);
    }
    return &font;
}

inline HBRUSH HSJWhiteBrush() {
    return (HBRUSH)GetStockObject(WHITE_BRUSH);
}

class CHSJVisualManager : public CBCGPVisualManagerXP {
    DECLARE_DYNCREATE(CHSJVisualManager)
public:
    CHSJVisualManager() { OnUpdateSystemColors(); }
protected:
    static void Brush(CBrush& brush, COLORREF color) {
        brush.DeleteObject();
        brush.CreateSolidBrush(color);
    }
    virtual void OnUpdateSystemColors() {
        CBCGPVisualManagerXP::OnUpdateSystemColors();
        m_clrBarBkgnd = HSJSurface();
        m_clrMenuLight = RGB(255, 255, 255);
        m_clrMenuRarelyUsed = HSJSurface();
        m_clrHighlight = RGB(233, 241, 253);
        m_clrHighlightDn = RGB(215, 230, 251);
        m_clrHighlightChecked = RGB(229, 239, 252);
        m_clrPressedButtonBorder = HSJAccent();
        m_clrSeparator = HSJBorder();
        m_clrGripper = RGB(178, 190, 204);
        m_clrPaneBorder = m_clrMenuBorder = HSJBorder();
        m_clrMenuItemBorder = RGB(202, 219, 243);
        m_clrInactiveTabText = RGB(106, 119, 136);
        m_bShdowDroppedDownMenuButton = FALSE;
        Brush(m_brBarBkgnd, m_clrBarBkgnd);
        Brush(m_brMenuLight, m_clrMenuLight);
        Brush(m_brMenuRarelyUsed, m_clrMenuRarelyUsed);
        Brush(m_brHighlight, m_clrHighlight);
        Brush(m_brHighlightDn, m_clrHighlightDn);
        Brush(m_brHighlightChecked, m_clrHighlightChecked);
        Brush(m_brTabBack, HSJSurface());
        Brush(m_brFloatToolBarBorder, HSJBorder());
        m_penSeparator.DeleteObject();
        m_penSeparator.CreatePen(PS_SOLID, 1, HSJBorder());
        m_penMenuItemBorder.DeleteObject();
        m_penMenuItemBorder.CreatePen(PS_SOLID, 1, m_clrMenuItemBorder);
        globalData.clrBarFace = HSJSurface();
        globalData.clrBarLight = globalData.clrBarHilite = RGB(255, 255, 255);
        globalData.clrBarShadow = globalData.clrBarDkShadow = HSJBorder();
        globalData.clrBarText = globalData.clrMenuText = HSJInk();
        Brush(globalData.brBarFace, HSJSurface());
        globalData.penBarFace.DeleteObject();
        globalData.penBarFace.CreatePen(PS_SOLID, 1, HSJSurface());
        globalData.penBarShadow.DeleteObject();
        globalData.penBarShadow.CreatePen(PS_SOLID, 1, HSJBorder());
    }
    virtual void OnFillBarBackground(CDC* dc, CBCGPBaseControlBar* bar,
        CRect client, CRect clip, BOOL nc = FALSE) {
        dc->FillSolidRect(clip.IsRectEmpty() ? client : clip,
            bar && bar->IsKindOf(RUNTIME_CLASS(CBCGPPopupMenuBar))
                ? RGB(255, 255, 255) : HSJSurface());
    }
    virtual void OnDrawBarBorder(CDC* dc, CBCGPBaseControlBar* bar, CRect& rect) {
        dc->Draw3dRect(rect, HSJBorder(), HSJBorder());
    }
    virtual void OnDrawBarGripper(CDC* dc, CRect rect, BOOL horz, CBCGPBaseControlBar* bar) {
        dc->FillSolidRect(rect, HSJSurface());
    }
    virtual void OnDrawSeparator(CDC* dc, CBCGPBaseControlBar* bar, CRect rect, BOOL horizontal) {
        if (horizontal)
            dc->FillSolidRect(rect.CenterPoint().x, rect.top + 3, 1, max(0, rect.Height() - 6), HSJBorder());
        else
            dc->FillSolidRect(rect.left + 3, rect.CenterPoint().y, max(0, rect.Width() - 6), 1, HSJBorder());
    }
    virtual COLORREF OnDrawControlBarCaption(CDC* dc, CBCGPDockingControlBar* bar,
        BOOL active, CRect caption, CRect buttons) {
        dc->FillSolidRect(caption, active ? RGB(235, 241, 249) : HSJSurface());
        dc->FillSolidRect(caption.left, caption.bottom - 1, caption.Width(), 1, HSJBorder());
        return HSJInk();
    }
    virtual void OnEraseTabsArea(CDC* dc, CRect rect, const CBCGPBaseTabWnd* tabs) {
        dc->FillSolidRect(rect, HSJSurface());
    }
    virtual void OnFillTab(CDC* dc, CRect rect, CBrush* brush, int index,
        BOOL active, const CBCGPBaseTabWnd* tabs) {
        dc->FillSolidRect(rect, active ? RGB(255, 255, 255) : HSJSurface());
    }
    virtual void OnDrawTab(CDC* dc, CRect rect, int index, BOOL active,
        const CBCGPBaseTabWnd* tabs) {
        // Keep tab dimensions and hit targets supplied by the existing container.
        COLORREF custom = tabs->GetTabBkColor(index);
        dc->FillSolidRect(rect, custom != (COLORREF)-1 ? custom :
            (active ? RGB(255, 255, 255) : HSJSurface()));
        OnDrawTabContent(dc, rect, index, active, tabs, active ? HSJAccent() : HSJInk());
        if (active) dc->FillSolidRect(rect.left, rect.bottom - 2, rect.Width(), 2, HSJAccent());
    }
    virtual void OnDrawStatusBarPaneBorder(CDC* dc, CBCGPStatusBar* bar,
        CRect rect, UINT id, UINT style) {
        dc->FillSolidRect(rect.right - 1, rect.top + 3, 1, max(0, rect.Height() - 6), HSJBorder());
    }
};

inline void HSJStyleGraph(NI::CNiGraph& graph) {
    // Cosmetic properties only: no axes ranges, data, cursors or track modes.
    try {
        graph.BackColor = NI::CNiColor(255, 255, 255);
        graph.PlotAreaColor = NI::CNiColor(255, 255, 255);
        graph.GraphFrameStyle = NI::CNiGraph::FrameClassic;
        graph.GraphFrameColor = NI::CNiColor(221, 228, 236);
        NI::CNiFont font = graph.Font;
        font.Name = _T("Microsoft YaHei UI");
        NI::CNiAxes axes = graph.Axes;
        for (long i = 1; i <= axes.Count; ++i) {
            NI::CNiAxis axis = axes.Item(i);
            axis.CaptionColor = NI::CNiColor(70, 83, 102);
            axis.Labels.Color = NI::CNiColor(70, 83, 102);
            axis.Ticks.MajorGridColor = NI::CNiColor(232, 237, 243);
            axis.Ticks.MinorGridColor = NI::CNiColor(242, 245, 248);
        }
    } catch (CException* e) {
        // A cosmetic COM property failure must not replace analysis error handling.
        TRACE(_T("HSJ graph appearance property unavailable.\n"));
        e->Delete();
    }
}
