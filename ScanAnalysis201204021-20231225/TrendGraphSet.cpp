// TrendGraphSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "TrendGraphSet.h"
#include "TrendView.h"

// CTrendGraphSet 对话框

IMPLEMENT_DYNAMIC(CTrendGraphSet, CHSJDialog)
CTrendView* tv;

CTrendGraphSet::CTrendGraphSet(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CTrendGraphSet::IDD, pParent)
	, title(_T(""))
	, xMin(0)
	, xMax(0)
	, yMin(0)
	, yMax(0)
{
	tv=(CTrendView*)pParent;
}

CTrendGraphSet::~CTrendGraphSet()
{
}

void CTrendGraphSet::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, title);

	DDX_Text(pDX, IDC_EDIT2, xMin);
	DDX_Text(pDX, IDC_EDIT3, xMax);
	DDX_Text(pDX, IDC_EDIT4, yMin);
	DDX_Text(pDX, IDC_EDIT5, yMax);
	DDX_Control(pDX, IDC_COLOR_PICKER1, m_ColorPicker1);
	DDX_Control(pDX, IDC_COLOR_PICKER2, m_ColorPicker2);
	DDX_Control(pDX, IDC_COLOR_PICKER3, m_ColorPicker3);
	DDX_Control(pDX, IDC_COLOR_PICKER4, m_ColorPicker4);
	
}


BEGIN_MESSAGE_MAP(CTrendGraphSet, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CTrendGraphSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_COLOR_PICKER1, &CTrendGraphSet::OnBnClickedColorPicker1)
	ON_BN_CLICKED(IDC_COLOR_PICKER2, &CTrendGraphSet::OnBnClickedColorPicker2)
	ON_BN_CLICKED(IDC_COLOR_PICKER3, &CTrendGraphSet::OnBnClickedColorPicker3)
	ON_BN_CLICKED(IDC_COLOR_PICKER4, &CTrendGraphSet::OnBnClickedColorPicker4)
END_MESSAGE_MAP()


// CTrendGraphSet 消息处理程序

void CTrendGraphSet::OnBnClickedOk()
{
	UpdateData(TRUE);
	tv->SetTrendParam(title,xMin,xMax,yMin,yMax,color1,color2,color3,color4);
	OnOK();
}

void CTrendGraphSet::SetTrendParam(CString t,int xmin,int xmax,int ymin,int ymax,COLORREF c1,COLORREF c2,COLORREF c3,COLORREF c4)
{
	title=t;
	xMin=xmin;
	xMax=xmax;
	yMin=ymin;
	yMax=ymax;
	color1=c1;
	color2=c2;
	color3=c3;
	color4=c4;

}
void CTrendGraphSet::OnBnClickedColorPicker1()
{
	color1 = m_ColorPicker1.GetColor ();
}

void CTrendGraphSet::OnBnClickedColorPicker2()
{
	color2 = m_ColorPicker2.GetColor ();
}

void CTrendGraphSet::OnBnClickedColorPicker3()
{
	color3 = m_ColorPicker3.GetColor ();
}

void CTrendGraphSet::OnBnClickedColorPicker4()
{
	color4 = m_ColorPicker4.GetColor ();
}

BOOL CTrendGraphSet::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_ColorPicker1.SetColor(color1);
	m_ColorPicker2.SetColor(color2);
	m_ColorPicker3.SetColor(color3);
	m_ColorPicker4.SetColor(color4);

	m_ColorPicker1.EnableOtherButton (_T("其它"));
	m_ColorPicker1.EnableOtherButton (_T("其它"));
	m_ColorPicker1.EnableOtherButton (_T("其它"));
	m_ColorPicker1.EnableOtherButton (_T("其它"));
	UpdateData(FALSE);

	return TRUE;  
}
