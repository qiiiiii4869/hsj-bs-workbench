#if !defined(AFX_CONFIG1_H__D29B598A_8875_4F06_AD06_B2F9531FE1DF__INCLUDED_)
#define AFX_CONFIG1_H__D29B598A_8875_4F06_AD06_B2F9531FE1DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Config1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfig1 dialog

class CConfig1 : public CHSJDialog
{
// Construction
public:
	CConfig1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfig1)
	enum { IDD = IDD_StripChartRangeDLG };
	CEdit	m_EditColumns;
	CEdit	m_EditRows;
	CEdit	m_EditWidth;
	CEdit	m_EditHeight;
	int		m_Height;
	int		m_Width;
	int		m_Rows;
	int		m_Columns;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfig1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfig1)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIG1_H__D29B598A_8875_4F06_AD06_B2F9531FE1DF__INCLUDED_)
