#if !defined(AFX_CONFIGZUOBIAO_H__2BDF3A3D_D9E7_4F0F_AF00_0A06A301FEEC__INCLUDED_)
#define AFX_CONFIGZUOBIAO_H__2BDF3A3D_D9E7_4F0F_AF00_0A06A301FEEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Configzuobiao.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigzuobiao dialog
#include "ColorEdit.h"
class CConfigzuobiao : public CHSJDialog
{
// Construction
public:
	CConfigzuobiao(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigzuobiao)
	enum { IDD = IDD_StripChartAxisDLG };
	CEdit	m_Editymax;
	CEdit	m_Editymin;
	CEdit	m_EditEndIndex;
	CEdit	m_EditStartIndex;
	CEdit	m_PixEdit;
	CEdit	m_TitleEdit;
	CComboBox   m_Combo;
	CColorEdit	m_color;
	CString	m_Title;
	int		m_StartIndex;
	int		m_EndIndex;
	float	m_ymax;
	float	m_ymin;
	int		m_pix;
	int 	m_Signal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigzuobiao)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigzuobiao)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGZUOBIAO_H__2BDF3A3D_D9E7_4F0F_AF00_0A06A301FEEC__INCLUDED_)
