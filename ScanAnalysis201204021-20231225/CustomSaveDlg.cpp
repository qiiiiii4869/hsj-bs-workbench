// CustomSaveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "CustomSaveDlg.h"
//#include "NiCtrlTestView.h"

// CCustomSaveDlg 对话框
//CNiCtrlTestView* tv;

IMPLEMENT_DYNAMIC(CCustomSaveDlg, CHSJDialog)

//CCustomSaveDlg::CCustomSaveDlg(CWnd* pParent /*=NULL*/)
//	: CHSJDialog(CCustomSaveDlg::IDD, pParent)
//	, mWidth(0)
//	, mHeight(0)
//	, m_fileType(1)
//	, m_bmpSize(1)
//{
//
//}

CCustomSaveDlg::CCustomSaveDlg(CWnd* pParent /*=NULL*/,int w, int h)
: CHSJDialog(CCustomSaveDlg::IDD, pParent)
,m_fileType(1)
, m_bmpSize(1)
{
	mWidth=w;
	mHeight=h;
}

CCustomSaveDlg::~CCustomSaveDlg()
{
}

void CCustomSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Width, m_nWidth);
	DDX_Control(pDX, IDC_Height, m_nHeight);
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
	DDX_Control(pDX, IDC_RADIO3, m_Radio3);
	DDX_Control(pDX, IDC_RADIO4, m_Radio4);
	DDX_Control(pDX, IDC_RADIO5,m_Radio5);
	DDX_Control(pDX, IDC_WidthC, m_nWidthC);
	DDX_Control(pDX, IDC_HeightC, m_nHeightC);
}


BEGIN_MESSAGE_MAP(CCustomSaveDlg, CHSJDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CCustomSaveDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CCustomSaveDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CCustomSaveDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CCustomSaveDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CCustomSaveDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, &CCustomSaveDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CCustomSaveDlg::OnBnClickedRadio7)
	//ON_BN_CLICKED(IDOK, &CCustomSaveDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK, &CCustomSaveDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCustomSaveDlg 消息处理程序

void CCustomSaveDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strCurrent;

	m_nWidth.GetWindowText(strCurrent);
	mWidth = atoi(strCurrent.GetBuffer());

	m_nHeight.GetWindowText(strCurrent);
	mHeight = atoi(strCurrent.GetBuffer());
	CHSJDialog::OnOK();
}



BOOL CCustomSaveDlg::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_Radio1.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BMP)));   
	
	m_Radio2.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_JPG)));  
	
	m_Radio3.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_GIF)));  
	
	m_Radio4.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_PNG)));  
	
	m_Radio5.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TIFF)));  
	
	CString   str;   
	str.Format("%d",mWidth);   
	m_nWidth.SetWindowText((LPCTSTR)str);
	m_nWidthC.SetWindowText((LPCTSTR)str);
	str.Format("%d",mHeight); 
	m_nHeight.SetWindowText((LPCTSTR)str);
	m_nHeightC.SetWindowText((LPCTSTR)str);

	CheckRadioButton(IDC_RADIO1,IDC_RADIO5,IDC_RADIO1);
	CheckRadioButton(IDC_RADIO6,IDC_RADIO7,IDC_RADIO6);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCustomSaveDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=1;
}

void CCustomSaveDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=2;
}

void CCustomSaveDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=3;
}

void CCustomSaveDlg::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=4;
}

void CCustomSaveDlg::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=5;
}

void CCustomSaveDlg::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bmpSize=1;
	m_nWidth.SetReadOnly(TRUE);
	m_nHeight.SetReadOnly(TRUE);
}

void CCustomSaveDlg::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bmpSize=2;
	m_nWidth.SetReadOnly(FALSE);
	m_nHeight.SetReadOnly(FALSE);
}



//void CCustomSaveDlg::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	OnOK();
//}

void CCustomSaveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
