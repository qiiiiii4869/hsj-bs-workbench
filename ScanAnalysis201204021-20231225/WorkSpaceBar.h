// workspacebar.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CustomTreeCtrl.h"
class CWorkSpaceBar : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar();
	CCustomTreeCtrl	m_wndTree;
// Attributes
protected:

	CImageList	m_wndTreeImages;
	HTREEITEM hRoot;
// Operations
public:
	void FillTree(void);					//通道名填充到数据操作目录树
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkSpaceBar();
	int fileCount;
// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnAllgraphvisual();
	int ClearTree(void);////Marshall  2011.2.20 20:9
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

/////////////////////////////////////////////////////////////////////////////
