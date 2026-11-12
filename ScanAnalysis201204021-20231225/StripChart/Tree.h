#if !defined(AFX_TREE_H__71A192CE_7571_4C59_A246_FC6D4118295F__INCLUDED_)
#define AFX_TREE_H__71A192CE_7571_4C59_A246_FC6D4118295F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTree window

class CTree : public CTreeCtrl
{
// Construction
public:
	CTree();

// Attributes
public:
	CString filePath;
	CString examType;
	CString examParam;
	int samplFreq;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	HTREEITEM m_HRoot;
	int m_nGroupCount;
	HTREEITEM m_HitItem;
	int flag;
	virtual ~CTree();
	void SetChnInit(CString fPath,CString eType,CString eParam,int sFreq);
	// Generated message map functions
protected:
	//{{AFX_MSG(CTree)
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREE_H__71A192CE_7571_4C59_A246_FC6D4118295F__INCLUDED_)
