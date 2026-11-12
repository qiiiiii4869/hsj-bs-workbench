// WaveFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "WaveFilter.h"


// CWaveFilter 对话框

IMPLEMENT_DYNAMIC(CWaveFilter, CHSJDialog)

CWaveFilter::CWaveFilter(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CWaveFilter::IDD, pParent)
	, m_FreqLowCut(0)
	, m_FreqHighCut(0)
	, m_FilterType(0)
	, m_Freq(0)
	, m_order(1)
{

}

CWaveFilter::~CWaveFilter()
{
}

void CWaveFilter::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_filters);
	DDX_Text(pDX,IDC_FreqLowCut,m_FreqLowCut);
	DDX_Text(pDX,IDC_Freq,m_Freq);
	DDX_Text(pDX,IDC_FreqHighCut,m_FreqHighCut);
	DDX_Text(pDX,IDC_Order,m_order);
	//DDV_MinMaxDouble(pDX, m_FreqLowCut, 0.0, 1000.0);
	//DDV_MinMaxInt(pDX, m_order, 1, 100);
	DDX_Control(pDX, IDC_FreqLowCut, m_lowCut);
	DDX_Control(pDX, IDC_FreqHighCut, m_highCut);
}


BEGIN_MESSAGE_MAP(CWaveFilter, CHSJDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CWaveFilter::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CWaveFilter::OnBnClickedOk)
END_MESSAGE_MAP()


// CWaveFilter 消息处理程序

BOOL CWaveFilter::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_filters.InsertString(0,"低通滤波");
	m_filters.InsertString(1,"高通滤波");
	m_filters.InsertString(2,"带通滤波");
	m_filters.InsertString(3,"带阻滤波");

	m_filters.SetCurSel(0);
	m_FreqLowCut=10.0;
	m_FreqHighCut=100.0;
	m_order=5;
	m_highCut.EnableWindow(FALSE);
	m_lowCut.EnableWindow(TRUE);


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CWaveFilter::OnCbnSelchangeCombo1()
{
	int sel=m_filters.GetCurSel();
	switch (sel)
	{
	case 0:
		m_highCut.EnableWindow(FALSE);
		m_lowCut.EnableWindow(TRUE);
		m_FilterType=0;
		break;
	case 1:
		m_highCut.EnableWindow(TRUE);
		m_lowCut.EnableWindow(FALSE);
		m_FilterType=1;
		break;
	case 2:
		m_highCut.EnableWindow(TRUE);
		m_lowCut.EnableWindow(TRUE);
		m_FilterType=2;
		break;
	case 3:
		m_highCut.EnableWindow(TRUE);
		m_lowCut.EnableWindow(TRUE);
		m_FilterType=3;
		break;
	default:
		break;
	}
}

void CWaveFilter::OnBnClickedOk()
{

	UpdateData(TRUE);
	if (m_order<1 || m_order>100)
	{
		AfxMessageBox("取值范围：1-100");
		return;
	}
	if (m_FreqLowCut>=m_Freq/2)
	{
		AfxMessageBox("截止频率<=波形频率/2");
		return;
	}
	if (m_FreqHighCut>=m_Freq/2)
	{
		AfxMessageBox("截止频率<=波形频率/2");
		return;
	}
	OnOK();
}
