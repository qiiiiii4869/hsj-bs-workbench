#include "afxcmn.h"
#if !defined(AFX_ADDGROUP_H__37F00088_379F_4C82_A231_32DD50D79913__INCLUDED_)
#define AFX_ADDGROUP_H__37F00088_379F_4C82_A231_32DD50D79913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddGroup.h : header file
//
#include "InitialConfig.h"
/////////////////////////////////////////////////////////////////////////////
// CAddGroup dialog
//#include "Tree.h"
class CAddGroup : public CHSJDialog
{
// Construction
public:
	CAddGroup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddGroup)
	enum { IDD = IDD_StripChartGroupDLG };
	HTREEITEM m_HRoot;
	int m_nGroupCount;
	HTREEITEM m_HitItem;
	int flag;
	//}}AFX_DATA
	CString filePath;
	CString examType;
	CString examParam;
	int samplFreq;
	CTreeCtrl m_Tree;
	CInitialConfig initConfig;
	//CTree	m_Tree;

	void SetChnInit(CString fPath,CString eType,CString eParam/*,int sFreq*/);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	//void ReadFromList(CString filePath,CString examtype,CListCtrl &m_List, CBoxingView *pView);  /////¶ÁÎÄ¼þ
	//void InfoComplete(CBoxingView *pView, int start, int gridc);
	//void SetChnInit(CString fPath,CString eType,CString eParam,int sFreq);
	//void FillList();
	//float GetYMax(CBoxingView::LOGPOINT *point, int count);
	//float GetYMin(CBoxingView::LOGPOINT *point, int count);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddGroup)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult);

//public:
//	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
//public:
//	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDGROUP_H__37F00088_379F_4C82_A231_32DD50D79913__INCLUDED_)
