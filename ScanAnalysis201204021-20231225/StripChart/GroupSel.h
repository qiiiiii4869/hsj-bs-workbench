#if !defined(AFX_GROUPSEL_H__DD2A0E1A_CB95_4BFB_BB8D_D46F7B5F68E4__INCLUDED_)
#define AFX_GROUPSEL_H__DD2A0E1A_CB95_4BFB_BB8D_D46F7B5F68E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupSel.h : header file
//
#include "InitialConfig.h"
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// CGroupSel dialog

class CGroupSel : public CHSJDialog
{
// Construction
public:
	int m_Mode;

	CString filePath;
	CString examType;
	CString examParam;
	int samplFreq;
	CInitialConfig initConfig;
	CGroupSel(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CGroupSel)
	enum { IDD = IDD_StripChartSignDLG };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupSel)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	std::vector<CString> paramVector;
	//void ReadFromList(CString filePath,CString examtype,CListCtrl &m_List, CBoxingView *pView);  /////¶ÁÎÄ¼þ
	//void InfoComplete(CBoxingView *pView, int start, int gridc);
	void SetChnInit(CString fPath,CString eType,CString eParam/*,int sFreq*/);
	void FillList();
	//float GetYMax(CBoxingView::LOGPOINT *point, int count);
	//float GetYMin(CBoxingView::LOGPOINT *point, int count);
	//afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPSEL_H__DD2A0E1A_CB95_4BFB_BB8D_D46F7B5F68E4__INCLUDED_)
