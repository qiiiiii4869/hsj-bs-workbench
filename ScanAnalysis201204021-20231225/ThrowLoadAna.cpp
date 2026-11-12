// ThrowLoadAna.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "ThrowLoadAna.h"


// CThrowLoadAna 对话框

IMPLEMENT_DYNAMIC(CThrowLoadAna, CHSJDialog)

CThrowLoadAna::CThrowLoadAna(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CThrowLoadAna::IDD, pParent)
	, m_baseTime(0)
	, m_maxTime(0)
{
	isGraph=true;

}

CThrowLoadAna::~CThrowLoadAna()
{
}

void CThrowLoadAna::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_baseTime);
	DDX_Control(pDX, IDC_CHECK1, m_graphChk);

	DDV_MinMaxDouble(pDX, m_baseTime, 0.0, 10000.0);
	DDX_Text(pDX, IDC_EDIT2, m_maxTime);
	DDV_MinMaxDouble(pDX, m_maxTime, 0.0, 10000.0);
}


BEGIN_MESSAGE_MAP(CThrowLoadAna, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CThrowLoadAna::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CThrowLoadAna::OnBnClickedCheck1)
	ON_BN_CLICKED(IDCANCEL, &CThrowLoadAna::OnBnClickedCancel)
END_MESSAGE_MAP()


// CThrowLoadAna 消息处理程序

void CThrowLoadAna::OnBnClickedOk()
{
	if (m_graphChk.GetCheck())
		isGraph=TRUE;
	else
		isGraph=false;

	UpdateData();
}

void CThrowLoadAna::OnBnClickedCheck1()
{


	if (m_graphChk.GetCheck())
	{
		isGraph=TRUE;
		m_graphChk.SetCheck(TRUE);
		//AfxMessageBox("1");
	}
	else
	{
		isGraph=FALSE;
		m_graphChk.SetCheck(FALSE);	
		//AfxMessageBox("2");

	}
}



BOOL CThrowLoadAna::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_graphChk.SetCheck(true);
	return TRUE;
}

void CThrowLoadAna::OnBnClickedCancel()
{
	UpdateData(false);
	OnCancel();
}
