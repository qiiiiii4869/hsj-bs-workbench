#if !defined(AFX_SAVEIMAGEDLG_H__673D27A1_DDA2_4214_B408_A3FCD73F5D97__INCLUDED_)
#define AFX_SAVEIMAGEDLG_H__673D27A1_DDA2_4214_B408_A3FCD73F5D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveImageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveImageDlg dialog

class CSaveImageDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CSaveImageDlg)

public:
	CSaveImageDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	    void OnTypeChange();

protected:
	//{{AFX_MSG(CSaveImageDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEIMAGEDLG_H__673D27A1_DDA2_4214_B408_A3FCD73F5D97__INCLUDED_)
