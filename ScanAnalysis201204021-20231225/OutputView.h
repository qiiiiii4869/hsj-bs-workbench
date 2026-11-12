#if !defined(AFX_OUTPUTVIEW_H__F9C28949_2C41_47AD_9BAB_E2516CEEC6E0__INCLUDED_)
#define AFX_OUTPUTVIEW_H__F9C28949_2C41_47AD_9BAB_E2516CEEC6E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputViewBar : public CBCGPDockingControlBar  
{
// Construction
public:
	COutputViewBar();

// Attributes
protected:

	CFont		m_Font;
	CListBox	m_wndOutput;

protected:
	void AdjusrHorzScroll (CListBox& wndListBox);
public:
	void FillText (CString str);	//填充文本
	void AutoScroll();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputViewBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutputViewBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(COutputViewBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	void SaveRecord();									//保存操作日志
	void AutoSaveRecord(CString path,CString rcdStr);								//自动保存操作日志
	CString GetCurTimeStr();							//获取当前的时刻
//public:
//	afx_msg void OnSaverecordtotext();
};

#endif // !defined(AFX_OUTPUTVIEW_H__F9C28949_2C41_47AD_9BAB_E2516CEEC6E0__INCLUDED_)
