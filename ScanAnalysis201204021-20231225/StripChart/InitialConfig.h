#if !defined(AFX_INITIALCONFIG_H__7FAACE43_E313_4142_8D88_D6BFA088ADE5__INCLUDED_)
#define AFX_INITIALCONFIG_H__7FAACE43_E313_4142_8D88_D6BFA088ADE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CInitialConfig dialog
#include "boxingView.h"
#include "Tree.h"
//#include "BoxingList.h"
#include "afxwin.h"
class CInitialConfig : public CHSJDialog
{
// Construction
public:
	CListCtrl m_List;
	CTree m_Tree;
	CInitialConfig(CWnd* pParent = NULL);   // standard constructor
	 float GetYMax(CBoxingView::LOGPOINT *point, int count);
	float GetYMin(CBoxingView::LOGPOINT *point, int count);
	//static void SeekToData(CStdioFile &file);
	//static void ReadFromList(CStdioFile &file, CListCtrl &m_List, CBoxingView *pView);
	void ReadFromList(CString filePath,CString examtype,CString paramStr, /*int sFreq,*/ CListCtrl &m_List, CBoxingView *pView);
	void ReadFromTree(CString filePath,CString examtype,CString paramStr, /*int sFreq,*/ CTree &m_Tree, CBoxingView *pView);
	void InfoComplete(CBoxingView *pView, int start, int gridc);
// Dialog Data
	//{{AFX_DATA(CInitialConfig)
	enum { IDD = IDD_StripChartInitDLG };
	CButton	m_buttonopp;
	CButton	m_buttonall;
	CEdit	m_yGridCountEdit;
	CEdit	m_ColumnsEdit;
	CEdit	m_RowsEdit;
	CTabCtrl	m_TabCtrl;
	CEdit	m_WidthEdit;
	CEdit	m_HeightEdit;
	CComboBox	m_Combo;
	CString	m_Size;
	int		m_Height;
	int		m_Width;
	int		m_Rows;
	int		m_Columns;
	int		m_yGridCount;

	std::vector<int> vSamplingFreq;
	std::vector<int> vDataLength; 

	std::vector<CString> paramVector;
	CString filePath;
	CString examType;
	CString examName;
	//int samplFreq;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitialConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInitialConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	void FillList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_param;
public:
	afx_msg void OnCbnSelchangeCombo2();

public:
	afx_msg void OnBnClickedOk();//这个函数没什么用//Marshall  2011.3.3 20:7
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITIALCONFIG_H__7FAACE43_E313_4142_8D88_D6BFA088ADE5__INCLUDED_)
