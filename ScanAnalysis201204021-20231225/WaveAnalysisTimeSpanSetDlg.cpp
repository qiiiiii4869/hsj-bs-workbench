// WaveAnalysisTimeSpanSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "WaveAnalysisTimeSpanSetDlg.h"


// CWaveAnalysisTimeSpanSetDlg 对话框

IMPLEMENT_DYNAMIC(CWaveAnalysisTimeSpanSetDlg, CHSJDialog)

CWaveAnalysisTimeSpanSetDlg::CWaveAnalysisTimeSpanSetDlg(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CWaveAnalysisTimeSpanSetDlg::IDD, pParent)

	, m_begin(0)
	, m_end(0)
{

}

CWaveAnalysisTimeSpanSetDlg::~CWaveAnalysisTimeSpanSetDlg()
{
}

void CWaveAnalysisTimeSpanSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BEGIN, m_begin);
	DDV_MinMaxDouble(pDX, m_begin, 0.0, 1000000.0);
	DDX_Text(pDX, IDC_END, m_end);
	DDV_MinMaxDouble(pDX, m_end, 0.0, 1000000.0);
}


BEGIN_MESSAGE_MAP(CWaveAnalysisTimeSpanSetDlg, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CWaveAnalysisTimeSpanSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWaveAnalysisTimeSpanSetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CWaveAnalysisTimeSpanSetDlg 消息处理程序

void CWaveAnalysisTimeSpanSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	//OnOK();
}

void CWaveAnalysisTimeSpanSetDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
