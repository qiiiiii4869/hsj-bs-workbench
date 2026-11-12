// AddGroup.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "boxingView.h"
#include "InitialConfig.h"
#include "AddGroup.h"
#include "GroupSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddGroup dialog


CAddGroup::CAddGroup(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CAddGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddGroup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddGroup::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddGroup)
	//DDX_Control(pDX, IDC_TREE1, m_Tree);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CAddGroup, CHSJDialog)
	//{{AFX_MSG_MAP(CAddGroup)
	//}}AFX_MSG_MAP

	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CAddGroup::OnNMRclickTree1)
	//ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CAddGroup::OnNMDblclkTree1)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CAddGroup::OnTvnSelchangedTree1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddGroup message handlers

BOOL CAddGroup::OnInitDialog() 
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_nGroupCount = 2;	
	flag = 1;
	m_HRoot = m_Tree.InsertItem("选择分组");
	m_Tree.InsertItem("第1组", m_HRoot);
	m_Tree.InsertItem("第2组", m_HRoot);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddGroup::OnOK()
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd *pChildFrame = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CBoxingView *pView = (CBoxingView *)pChildFrame->GetActiveView();
	int count = (int)pView->boxing.GetSize();
	//CStdioFile file(((CBoxingApp *)AfxGetApp())->filename, CFile::modeRead|CFile::shareDenyWrite);
	//CInitialConfig::SeekToData(file);
	initConfig.ReadFromTree(filePath,examType,examParam,/*samplFreq, */(CTree &)m_Tree, pView);		
	initConfig.InfoComplete(pView, count, pView->yGridCount);
	CHSJDialog::OnOK();
}

void CAddGroup::SetChnInit(CString fPath,CString eType,CString eParam/*,int sFreq*/)
{
	filePath=fPath;
	examType=eType;
	examParam=eParam;
	//samplFreq=sFreq;
}

void CAddGroup::OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	::GetCursorPos(&point);
	CPoint pos = point;
	ScreenToClient(&pos); 
	m_HitItem = m_Tree.HitTest(pos);
	CRect rect;
	m_Tree.GetItemRect(m_HRoot, &rect, TRUE);
	//HTREEITEM   m_selectedItem   =  m_Tree.GetSelectedItem(); 
	if(rect.PtInRect(pos))/*m_selectedItem==m_Tree.GetRootItem()&&*/
	{
		m_Tree.SelectItem(m_HRoot);
		CMenu menu;
		menu.LoadMenu(IDR_StripChartGroupMenu);
		CMenu *pContextMenu = menu.GetSubMenu(0);
		int nCmd = (int)pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, this);
		if (nCmd == ID_MENUITEMAddGroup)
		{
			CString string;
			string.Format("第%d组", ++m_nGroupCount);
			m_Tree.InsertItem(string, m_HRoot);
		}
	}
	else if(m_HitItem && m_Tree.GetParentItem(m_HitItem) == m_HRoot)/*m_selectedItem!=m_Tree.GetRootItem()*/
	{
		m_Tree.GetItemRect(m_HitItem, &rect, TRUE);
		if (rect.PtInRect(pos))
		{
			m_Tree.SelectItem(m_HitItem);
			CMenu menu;
			menu.LoadMenu(IDR_StripAddSignMenu);
			CMenu *pContextMenu = menu.GetSubMenu(0);
			int nCmd = (int)pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, this);
			if (nCmd == ID_MENUITEMAddSign)
			{
				CGroupSel groupsel;
				groupsel.SetChnInit(filePath,examType,examParam/*,samplFreq*/);
				if (flag == 0)
					groupsel.m_Mode = 0;
				else
					groupsel.m_Mode = 2;
				groupsel.DoModal();
			}
		}
	  }	
	  *pResult = 0;
}


//
//void CAddGroup::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}
//
//void CAddGroup::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}

