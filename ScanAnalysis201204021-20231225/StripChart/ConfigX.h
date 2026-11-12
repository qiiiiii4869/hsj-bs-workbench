#if !defined(AFX_CONFIGX_H__18692787_7BFD_429C_9FCE_8B70DDB6E869__INCLUDED_)
#define AFX_CONFIGX_H__18692787_7BFD_429C_9FCE_8B70DDB6E869__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigX.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigX dialog

class CConfigX : public CHSJDialog
{
// Construction
public:
	CConfigX(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigX)
	enum { IDD = IDD_StripChartXSetDLG };
	CEdit	m_EditEndTime;
	CEdit	m_EditStartTime;
	CEdit	m_EditColumns;
	float	m_StartTime;
	float	m_EndTime;
	int		m_Columns;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigX)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGX_H__18692787_7BFD_429C_9FCE_8B70DDB6E869__INCLUDED_)
