// ConfigTitle.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "ConfigTitle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigTitle dialog


CConfigTitle::CConfigTitle(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CConfigTitle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigTitle)
	m_title = _T("");
	//}}AFX_DATA_INIT
}


void CConfigTitle::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigTitle)
	DDX_Text(pDX, IDC_EDIT1, m_title);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigTitle, CHSJDialog)
	//{{AFX_MSG_MAP(CConfigTitle)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigTitle message handlers


BOOL CConfigTitle::OnInitDialog()
{
	CHSJDialog::OnInitDialog();

	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	return TRUE;  // return TRUE unless you set the focus to a control
}
