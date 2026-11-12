// SaveImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "SaveImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveImageDlg

IMPLEMENT_DYNAMIC(CSaveImageDlg, CFileDialog)

CSaveImageDlg::CSaveImageDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CSaveImageDlg, CFileDialog)
	//{{AFX_MSG_MAP(CSaveImageDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSaveImageDlg::OnTypeChange()
{
	if (m_ofn.nFilterIndex == 1)
	{
		CEdit *edit = (CEdit*)GetParent()->GetDlgItem(1152);
		edit->SetWindowText("综合波形图.bmp");
	}
	else
	{
		CEdit *edit = (CEdit*)GetParent()->GetDlgItem(1152);
		edit->SetWindowText("综合波形图.jpg");
	}
	return;
}