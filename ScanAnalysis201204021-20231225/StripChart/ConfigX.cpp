// ConfigX.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "boxingView.h"
#include "ConfigX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigX dialog


CConfigX::CConfigX(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CConfigX::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigX)
	m_StartTime = 0.0f;
	m_EndTime = 0.0f;
	m_Columns = 0;
	//}}AFX_DATA_INIT
}


void CConfigX::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigX)
	DDX_Control(pDX, IDC_EDIT2, m_EditEndTime);
	DDX_Control(pDX, IDC_EDIT1, m_EditStartTime);
	DDX_Control(pDX, IDC_EDIT3, m_EditColumns);
	DDX_Text(pDX, IDC_EDIT1, m_StartTime);
	DDV_MinMaxFloat(pDX, m_StartTime, 0.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT2, m_EndTime);
	DDV_MinMaxFloat(pDX, m_EndTime, 0.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT3, m_Columns);
	//}}AFX_DATA_MAP
}

void CConfigX::OnOK()
{
	CString string;
	m_EditColumns.GetWindowText(string);
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd *pChildFrame = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CBoxingView *pView = (CBoxingView *)pChildFrame->GetActiveView();

	CString stringst, stringen;
	m_EditStartTime.GetWindowText(stringst);
	m_EditEndTime.GetWindowText(stringen);
	if (::atof(stringst) >= ::atof(stringen))
	{
		MessageBox("起始时刻必须小于结束时刻！");
		return;
	}
	if (pView->m_ViewPortExt.cx / ::atoi(string) < 15)
	{
		MessageBox("网格线太密，请重新设置参数！");
		return;
	}
	CHSJDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CConfigX, CHSJDialog)
	//{{AFX_MSG_MAP(CConfigX)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CConfigX::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigX message handlers



void CConfigX::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CConfigX::OnInitDialog()
{
	CHSJDialog::OnInitDialog();

	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	return TRUE;  // return TRUE unless you set the focus to a control
}
