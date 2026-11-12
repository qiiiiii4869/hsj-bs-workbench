// ThrowLoadSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "ThrowLoadSetDlg.h"

#include "NiWaveFraqView.h"


CNiWaveFraqView* pwave;


// CThrowLoadSetDlg 对话框

IMPLEMENT_DYNAMIC(CThrowLoadSetDlg, CHSJDialog)

CThrowLoadSetDlg::CThrowLoadSetDlg(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CThrowLoadSetDlg::IDD, pParent)
	, m_width(50)
	, m_increase(1)
{

}

CThrowLoadSetDlg::~CThrowLoadSetDlg()
{
}

void CThrowLoadSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_width);
	DDV_MinMaxDouble(pDX, m_width, 1, 10000);
	//DDX_Text(pDX, IDC_EDIT2, m_increase);

}


BEGIN_MESSAGE_MAP(CThrowLoadSetDlg, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CThrowLoadSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CThrowLoadSetDlg 消息处理程序

void CThrowLoadSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	//pwave->AddTrendWave(1,m_width,m_increase,RGB (255, 0, 255));
	OnOK();
	//pwave->m_width=m_width;
	//pwave->m_increase=m_increase;

}
