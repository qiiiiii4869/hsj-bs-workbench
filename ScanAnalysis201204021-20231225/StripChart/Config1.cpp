// Config1.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "Config1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfig1 dialog


CConfig1::CConfig1(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CConfig1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfig1)
	m_Height = 0;
	m_Width = 0;
	m_Rows = 0;
	m_Columns = 0;
	//}}AFX_DATA_INIT
}


void CConfig1::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfig1)
	DDX_Control(pDX, IDC_EDIT4, m_EditColumns);
	DDX_Control(pDX, IDC_EDIT3, m_EditRows);
	DDX_Control(pDX, IDC_EDIT2, m_EditWidth);
	DDX_Control(pDX, IDC_EDIT1, m_EditHeight);
	DDX_Text(pDX, IDC_EDIT1, m_Height);
	DDX_Text(pDX, IDC_EDIT2, m_Width);
	DDX_Text(pDX, IDC_EDIT3, m_Rows);
	DDX_Text(pDX, IDC_EDIT4, m_Columns);
	//}}AFX_DATA_MAP
}

void CConfig1::OnOK()
{
	CString string1, string2;
	m_EditHeight.GetWindowText(string1);
	m_EditRows.GetWindowText(string2);
	int height = ::atoi(string1);
	if (::atoi(string1) / ::atoi(string2) < 15)
	{
		MessageBox("网格线太密，请重新设置参数！");
		return;
	}
	m_EditWidth.GetWindowText(string1);
	m_EditColumns.GetWindowText(string2);
	if (::atoi(string1) / ::atoi(string2) < 15)
	{
		MessageBox("网格线太密，请重新设置参数！");
		return;
	}
	if (height * ::atoi(string1) > 22500000)
	{
		MessageBox("视图太大，请重新设置参数！");
		return;
	}
	CHSJDialog::OnOK();
}


BEGIN_MESSAGE_MAP(CConfig1, CHSJDialog)
	//{{AFX_MSG_MAP(CConfig1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfig1 message handlers


BOOL CConfig1::OnInitDialog()
{
	CHSJDialog::OnInitDialog();

	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	return TRUE;  // return TRUE unless you set the focus to a control
}
