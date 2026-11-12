#if !defined(AFX_CONFIGTITLE_H__95651534_DA41_4E52_8739_74359DAD30BA__INCLUDED_)
#define AFX_CONFIGTITLE_H__95651534_DA41_4E52_8739_74359DAD30BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigTitle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigTitle dialog

class CConfigTitle : public CHSJDialog
{
// Construction
public:
	CConfigTitle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigTitle)
	enum { IDD = IDD_StripChartTiTleDLG };
	CString	m_title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigTitle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigTitle)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGTITLE_H__95651534_DA41_4E52_8739_74359DAD30BA__INCLUDED_)
