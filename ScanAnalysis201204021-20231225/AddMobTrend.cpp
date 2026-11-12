// AddMobTrend.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "AddMobTrend.h"
#include "NiWaveFraqView.h"

// CAddMobTrend 对话框
extern CNiWaveFraqView* wave;

IMPLEMENT_DYNAMIC(CAddMobTrend, CHSJDialog)

CAddMobTrend::CAddMobTrend(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CAddMobTrend::IDD, pParent)
	, m_winStep(10),m_winWidth(10),TrendType(1)
{
	m_strRGB = _T("255, 0, 255");
	color=RGB (255, 0, 255);
	wave=(CNiWaveFraqView*)pParent;
}

void CAddMobTrend::OnBnClickedColorPicker()
{
	color = m_ColorPicker.GetColor ();

	m_strRGB.Format (_T("%d,%d,%d"), 
		GetRValue (color),
		GetGValue (color),
		GetBValue (color));

	//UpdateData (FALSE);
}

CAddMobTrend::~CAddMobTrend()
{
}

void CAddMobTrend::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR_PICKER, m_ColorPicker);
	DDX_Text(pDX, IDC_RGB, m_strRGB);
	DDX_Text(pDX, IDC_EDIT2, m_winStep);
	DDX_Text(pDX, IDC_EDIT1, m_winWidth);
	DDX_Control(pDX, IDC_COMBO1, m_TrendParam);
}


BEGIN_MESSAGE_MAP(CAddMobTrend, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CAddMobTrend::OnBnClickedOk)
	ON_BN_CLICKED(IDC_COLOR_PICKER, &CAddMobTrend::OnBnClickedColorPicker)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAddMobTrend::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CAddMobTrend 消息处理程序

void CAddMobTrend::OnBnClickedOk()
{

	UpdateData(TRUE);
	wave->AddTrendWave(TrendType,m_winWidth,m_winStep,color);
	OnOK();
}

BOOL CAddMobTrend::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_ColorPicker.EnableAutomaticButton (_T("缺省颜色"), RGB (255, 0, 255));
	m_ColorPicker.EnableOtherButton (_T("其它"));
	m_ColorPicker.SetColor ((COLORREF)-1);
	m_ColorPicker.SetColumnsNumber (10);
	
	m_TrendParam.InsertString(0,"峰峰值");	
	m_TrendParam.InsertString(0,"有效值");
	m_TrendParam.InsertString(0,"中间值");
	m_TrendParam.InsertString(0,"平均值");
	m_TrendParam.SetCurSel(0);
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAddMobTrend::OnCbnSelchangeCombo1()
{
	CString trendStr;
	m_TrendParam.GetLBText(m_TrendParam.GetCurSel(),trendStr);
	if (trendStr=="平均值")
	{
		TrendType=1;
	} 
	else if(trendStr=="中间值")
	{
		TrendType=2;
	}
	else if (trendStr=="有效值")
	{
		TrendType=3;
	}
	else
	{
		TrendType=4;
	}
}
