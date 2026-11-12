#pragma once
#include "afxwin.h"


// CSet3DFreqDlg 对话框

class CSet3DFreqDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(CSet3DFreqDlg)

public:
	CSet3DFreqDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSet3DFreqDlg();

// 对话框数据
	enum { IDD = IDD_SET3DFREQDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTitlefnt();
public:
	CStatic m_stcTlFont;

	CFont   m_fntTl;
	CFont   m_fntX;
	CFont   m_fntY;
	CFont   m_fntZ;

	COLORREF m_clrTl;
	COLORREF m_clrX;
	COLORREF m_clrY;
	COLORREF m_clrZ;

	CBrush   m_brhTl;
	
public:
	afx_msg void OnBnClickedBtnTitleclr();
public:
	afx_msg void OnBnClickedBtnXfont();
public:
	CStatic m_stcXFont;
public:
	CStatic m_stcYFont;
public:
	CStatic m_stcZFont;
public:
	afx_msg void OnBnClickedBtnYfont();
public:
	afx_msg void OnBnClickedBtnZfont();
public:
	afx_msg void OnBnClickedBtnXclr();
public:
	afx_msg void OnBnClickedBtnYclr();
public:
	afx_msg void OnBnClickedBtnZclr();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
};
