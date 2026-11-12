// Configzuobiao.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "boxingView.h"
#include "Configzuobiao.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigzuobiao dialog


CConfigzuobiao::CConfigzuobiao(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CConfigzuobiao::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigzuobiao)
	m_Title = _T("");
	m_StartIndex = 0;
	m_EndIndex = 0;
	m_ymax = 0.0f;
	m_ymin = 0.0f;
	m_pix = 0;
	m_Signal = 0;
	//}}AFX_DATA_INIT
}


void CConfigzuobiao::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigzuobiao)
	DDX_Control(pDX, IDC_EDIT5, m_Editymax);
	DDX_Control(pDX, IDC_EDIT6, m_Editymin);
	DDX_Control(pDX, IDC_EDIT4, m_EditEndIndex);
	DDX_Control(pDX, IDC_EDIT3, m_EditStartIndex);
	DDX_Control(pDX, IDC_EDIT7, m_PixEdit);
	DDX_Control(pDX, IDC_EDIT1, m_TitleEdit);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_Control(pDX, IDC_EDIT8, m_color);
	DDX_Text(pDX, IDC_EDIT1, m_Title);
	DDV_MaxChars(pDX, m_Title, 50);

	DDX_Text(pDX, IDC_EDIT3, m_StartIndex);
	DDX_Text(pDX, IDC_EDIT4, m_EndIndex);
	DDX_Text(pDX, IDC_EDIT5, m_ymax);
	DDX_Text(pDX, IDC_EDIT6, m_ymin);
	DDX_Text(pDX, IDC_EDIT7, m_pix);
	DDV_MinMaxInt(pDX, m_pix, 1, 30);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Signal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigzuobiao, CHSJDialog)
	//{{AFX_MSG_MAP(CConfigzuobiao)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigzuobiao message handlers

BOOL CConfigzuobiao::OnInitDialog() 
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_color.SetReadOnly();
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd *pChildFrame = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CBoxingView *pView = (CBoxingView *)pChildFrame->GetActiveView();
	CBoxingView::BOXING *p = &pView->boxing[pView->selindex];
	int index = 1;
	while (p)
	{
		CString string;
		string.Format("信号%d", index++);
		m_Combo.AddString(string);
		p = p->pNext;
	}
	m_Combo.SetCurSel(0);
	CString str;
	str.Format("%0.2f",m_ymax);
	m_Editymax.SetWindowText(str);
	str.Format("%0.2f",m_ymin);
	m_Editymin.SetWindowText(str);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control	             
}

void CConfigzuobiao::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	int s = m_Combo.GetCurSel();
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd *pChildFrame = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CBoxingView *pView = (CBoxingView *)pChildFrame->GetActiveView();
	CBoxingView::BOXING *p = &pView->boxing[pView->selindex];
	while (s)
	{
		p = p->pNext;
		s--;
	}
	m_TitleEdit.SetWindowText(p->title);
	CString string;
	string.Format("%d", p->pix);
	m_PixEdit.SetWindowText(string);
	m_color.SetBkColor(p->color);
	m_color.m_colorsel = p->color;
	Invalidate();
}

void CConfigzuobiao::OnOK()
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd *pChildFrame = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CBoxingView *pView = (CBoxingView *)pChildFrame->GetActiveView();

	CString stringst, stringen;
	m_Editymin.GetWindowText(stringst);
	m_Editymax.GetWindowText(stringen);
	if (::atof(stringst) >= ::atof(stringen))
	{
		MessageBox("最大值必须大于最小值！");
		return;
	}

	m_EditStartIndex.GetWindowText(stringst);
	m_EditEndIndex.GetWindowText(stringen);
	if (::atoi(stringst) >= ::atoi(stringen))
	{
		MessageBox("起始网格号必须小于结束网格号！");
		return;
	}
	if (::atoi(stringst) < 0 || ::atoi(stringen) < 0)
	{
		MessageBox("网格号不能为负数！");
		return;
	}

	if (::atoi(stringst) > pView->m_Rows || ::atoi(stringen) > pView->m_Rows)
	{
		MessageBox("网格号超出显示范围！");
		return;
	}

	CHSJDialog::OnOK();
}

