// ColorEdit.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "ColorEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorEdit

CColorEdit::CColorEdit()
{
	m_hBrush = CreateSolidBrush(RGB(255, 0, 0));	
}

CColorEdit::~CColorEdit()
{
}


BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
	//{{AFX_MSG_MAP(CColorEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorEdit message handlers

BOOL CColorEdit::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnEraseBkgnd(pDC);
	return true;
}

HBRUSH CColorEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	if (m_hBrush) 
    { 
        pDC->SetBkMode(TRANSPARENT);
		return m_hBrush ; 
    }
	return NULL;
}

void CColorEdit::SetBkColor(COLORREF color)
{
	m_hBrush = CreateSolidBrush(color);
}

void CColorEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CColorDialog choosecolor;
	if (choosecolor.DoModal() == IDOK)
	{
		m_colorsel = choosecolor.GetColor();
		SetBkColor(m_colorsel);
		Invalidate();
	}
	CEdit::OnLButtonDblClk(nFlags, point);
	::ReleaseCapture();
}

BOOL CColorEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return true;
}

void CColorEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;
}

void CColorEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;
}
