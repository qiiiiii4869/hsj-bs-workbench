// Shared native-dialog appearance. Resource templates and DLU metrics stay intact.
#pragma once
#include <afxtempl.h>
#include "HSJAppearance.h"

class CHSJFontStore {
    CMap<HFONT, HFONT, CFont*, CFont*> m_fonts;
public:
    ~CHSJFontStore() {
        POSITION pos = m_fonts.GetStartPosition();
        while (pos) {
            HFONT key; CFont* font;
            m_fonts.GetNextAssoc(pos, key, font);
            delete font;
        }
    }
    void Apply(HWND child) {
        const HFONT original = (HFONT)::SendMessage(child, WM_GETFONT, 0, 0);
        if (!original) return;
        CFont* font = NULL;
        if (!m_fonts.Lookup(original, font)) {
            LOGFONT lf = {};
            if (!::GetObject(original, sizeof(lf), &lf)) return;
            lstrcpyn(lf.lfFaceName, _T("Microsoft YaHei UI"), LF_FACESIZE);
            lf.lfQuality = CLEARTYPE_QUALITY;
            font = new CFont;
            if (!font->CreateFontIndirect(&lf)) { delete font; return; }
            m_fonts.SetAt(original, font);
        }
        ::SendMessage(child, WM_SETFONT, (WPARAM)font->GetSafeHandle(), FALSE);
    }
private:
    CHSJFontStore(const CHSJFontStore&);
    CHSJFontStore& operator=(const CHSJFontStore&);
public:
    CHSJFontStore() {}
};

inline void HSJStyleChildControls(HWND parent, CHSJFontStore& fonts) {
    for (HWND child = ::GetWindow(parent, GW_CHILD); child;
         child = ::GetWindow(child, GW_HWNDNEXT)) {
        TCHAR name[64] = {};
        ::GetClassName(child, name, 64);
        const BOOL tree = lstrcmpi(name, WC_TREEVIEW) == 0;
        const BOOL list = lstrcmpi(name, WC_LISTVIEW) == 0;
        const BOOL listbox = lstrcmpi(name, _T("ListBox")) == 0;
        if (tree || list || listbox ||
            lstrcmpi(name, _T("Static")) == 0 ||
            lstrcmpi(name, _T("Button")) == 0 ||
            lstrcmpi(name, _T("Edit")) == 0 ||
            lstrcmpi(name, _T("ComboBox")) == 0 ||
            lstrcmpi(name, WC_TABCONTROL) == 0 ||
            lstrcmpi(name, WC_HEADER) == 0) {
            // Keep explicit row heights when replacing a native control's font.
            LRESULT itemHeight = 0;
            if (tree) itemHeight = ::SendMessage(child, TVM_GETITEMHEIGHT, 0, 0);
            if (listbox) itemHeight = ::SendMessage(child, LB_GETITEMHEIGHT, 0, 0);
            fonts.Apply(child);
            if (tree && itemHeight > 0) ::SendMessage(child, TVM_SETITEMHEIGHT, itemHeight, 0);
            if (listbox && itemHeight > 0) ::SendMessage(child, LB_SETITEMHEIGHT, 0, itemHeight);
        }
        if (list) {
            ::SendMessage(child, LVM_SETBKCOLOR, 0, RGB(255, 255, 255));
            ::SendMessage(child, LVM_SETTEXTBKCOLOR, 0, RGB(255, 255, 255));
            ::SendMessage(child, LVM_SETTEXTCOLOR, 0, HSJInk());
        }
        if (tree) {
            ::SendMessage(child, TVM_SETBKCOLOR, 0, RGB(255, 255, 255));
            ::SendMessage(child, TVM_SETTEXTCOLOR, 0, HSJInk());
            ::SendMessage(child, TVM_SETLINECOLOR, 0, HSJBorder());
        }
        // OLE/NI child windows retain their own font and drawing interfaces.
        if (tree || list || listbox || lstrcmpi(name, _T("ComboBox")) == 0)
            HSJStyleChildControls(child, fonts);
    }
}

inline BOOL HSJSetWhiteControlColor(CDC* dc, CWnd* child, UINT type) {
    if (type != CTLCOLOR_DLG && type != CTLCOLOR_STATIC && type != CTLCOLOR_BTN)
        return FALSE;
    dc->SetTextColor(child && !child->IsWindowEnabled() ? RGB(145, 154, 167) : HSJInk());
    dc->SetBkColor(RGB(255, 255, 255));
    dc->SetBkMode(TRANSPARENT);
    return TRUE;
}

class CHSJDialog : public CDialog {
    DECLARE_DYNAMIC(CHSJDialog)
    DECLARE_MESSAGE_MAP()
    CHSJFontStore m_skinFonts;
public:
    CHSJDialog() : CDialog() {}
    CHSJDialog(UINT id, CWnd* parent = NULL) : CDialog(id, parent) {}
    CHSJDialog(LPCTSTR id, CWnd* parent = NULL) : CDialog(id, parent) {}
protected:
    virtual BOOL OnInitDialog() {
        const BOOL focus = CDialog::OnInitDialog();
        // Leave the dialog font itself unchanged: MapDialogRect still uses
        // the original resource font. Only the existing child typefaces change.
        HSJStyleChildControls(GetSafeHwnd(), m_skinFonts);
        return focus;
    }
    afx_msg HBRUSH OnCtlColor(CDC* dc, CWnd* child, UINT type) {
        HBRUSH original = CDialog::OnCtlColor(dc, child, type);
        if (HSJSetWhiteControlColor(dc, child, type)) return HSJWhiteBrush();
        // Keep reflected edit/color-picker brushes and native read-only colors.
        return original;
    }
};

inline void HSJStyleGraph3D(NI::CNiGraph3D& graph) {
    try {
        graph.BackColor = NI::CNiColor(255, 255, 255);
        graph.PlotAreaColor = NI::CNiColor(255, 255, 255);
        graph.CaptionColor = NI::CNiColor(38, 51, 69);
        graph.GraphFrameColor = NI::CNiColor(221, 228, 236);
        graph.GridFrameColor = NI::CNiColor(221, 228, 236);
        graph.Font.Name = _T("Microsoft YaHei UI");
        NI::CNiAxes3D axes = graph.Axes;
        for (long i = 1; i <= axes.Count; ++i) {
            NI::CNiAxis3D axis = axes.Item(i);
            axis.CaptionColor = NI::CNiColor(70, 83, 102);
            axis.CaptionFont.Name = _T("Microsoft YaHei UI");
            axis.Labels.Color = NI::CNiColor(70, 83, 102);
            axis.Labels.Font.Name = _T("Microsoft YaHei UI");
            axis.Ticks.MajorGridColor = NI::CNiColor(232, 237, 243);
            axis.Ticks.MinorGridColor = NI::CNiColor(242, 245, 248);
            axis.Ticks.MajorTickColor = NI::CNiColor(137, 151, 169);
        }
        // Preserve camera, projection, ranges, plots and user curve colors.
    } catch (CException* e) {
        TRACE(_T("HSJ 3D appearance property unavailable.\n"));
        e->Delete();
    }
}
