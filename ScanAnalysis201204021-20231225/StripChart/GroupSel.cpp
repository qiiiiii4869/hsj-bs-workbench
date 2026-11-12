// GroupSel.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "..\MainFrm.h"
#include "InitialConfig.h"
#include "boxingView.h"
#include "AddGroup.h"
#include "Tree.h"
#include "GroupSel.h"

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF gcolor[10] = {RGB(255, 0, 0), RGB(0, 0, 255), RGB(128, 128, 0), RGB(255, 0, 255), RGB(0, 255, 64),
RGB(128, 0, 255), RGB(0, 0, 0), RGB(255, 128, 0), RGB(236, 236, 0), RGB(64, 128, 128)};

/////////////////////////////////////////////////////////////////////////////
// CGroupSel dialog

CGroupSel::CGroupSel(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CGroupSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGroupSel::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupSel)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupSel, CHSJDialog)
	//{{AFX_MSG_MAP(CGroupSel)
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDOK, &CGroupSel::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupSel message handlers

BOOL CGroupSel::OnInitDialog() 
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_List.InsertColumn(0, "信号", LVCFMT_LEFT, 195);

	FillList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CGroupSel::SetChnInit(CString fPath,CString eType,CString eParam/*,int sFreq*/)
{
	filePath=fPath;
	examType=eType;
	examParam=eParam;
	//samplFreq=sFreq;
}
void CGroupSel::FillList()
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	m_List.DeleteAllItems();
	//CString paramStr;
	//m_param.GetLBText(m_param.GetCurSel(),paramStr);
	vector<CString> chnVector=pCMF->fileData->GetChnName(filePath,examParam);
	for (int p=0;p<(int)chnVector.size();p++)
	{
		m_List.InsertItem(p, chnVector[p]);
	}
}
void CGroupSel::OnOK()
{
	if (m_Mode == 0)
	{
		int c = 0;
		for (int i = 0; i < m_List.GetItemCount(); i++)
		{
			if (m_List.GetCheck(i))
				c++;
		}
		if (c > 10)
		{
			MessageBox("一组内信号不能超过10个！");
			return;
		}
		for (int i = 0; i < m_List.GetItemCount(); i++)
		{
			if (m_List.GetCheck(i))
			{
				((CInitialConfig*)GetParent())->m_Tree.InsertItem(m_List.GetItemText(i, 0), ((CInitialConfig*)GetParent())->m_Tree.m_HitItem);
			}
		}
	}
	else if (m_Mode == 2)
	{
		int c = 0;
		for (int i = 0; i < m_List.GetItemCount(); i++)
		{
			if (m_List.GetCheck(i))
				c++;
		}
		if (c > 10)
		{
			MessageBox("一组内信号不能超过10个！");
			return;
		}
		for (int i = 0; i < m_List.GetItemCount(); i++)
		{
			if (m_List.GetCheck(i))
			{
				((CAddGroup*)GetParent())->m_Tree.InsertItem(m_List.GetItemText(i, 0), ((CAddGroup*)GetParent())->m_HitItem);
			}
		}
	}
	else
	{
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd *pChildFrame = (CMDIChildWnd *)pFrame->GetActiveFrame();
		CBoxingView *pView = (CBoxingView *)pChildFrame->GetActiveView();
		int count = (int)pView->boxing.GetSize();
		/*CStdioFile file(((CBoxingApp *)AfxGetApp())->filename, CFile::modeRead|CFile::shareDenyWrite);
		CInitialConfig::SeekToData(file);*/

		initConfig.ReadFromList(filePath,examType,examParam,/*samplFreq,*/ m_List, pView);
		initConfig.InfoComplete(pView, count, pView->yGridCount);			
	}
	CHSJDialog::OnOK();
}

