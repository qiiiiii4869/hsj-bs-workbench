// Tree.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "InitialConfig.h"
#include "Tree.h"
#include "GroupSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTree

CTree::CTree()
{
	m_nGroupCount = 2;
}

CTree::~CTree()
{
}


BEGIN_MESSAGE_MAP(CTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CTree)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(NM_SETFOCUS, OnSetfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTree message handlers

void CTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint point;
    ::GetCursorPos(&point);
	CPoint pos = point;
	ScreenToClient(&pos);
	m_HitItem = HitTest(pos);
	CRect rect;
	GetItemRect(m_HRoot, &rect, TRUE);
	if (rect.PtInRect(pos))
	{
		SelectItem(m_HRoot);
		CMenu menu;
		menu.LoadMenu(IDR_StripChartGroupMenu);
		CMenu *pContextMenu = menu.GetSubMenu(0);
		int nCmd = (int)pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, this);
		if (nCmd == ID_MENUITEMAddGroup)
		{
			CString string;
			string.Format("µÚ%d×é", ++m_nGroupCount);
			InsertItem(string, m_HRoot);
		}
	}
	else if (m_HitItem && GetParentItem(m_HitItem) == m_HRoot)
	{
		GetItemRect(m_HitItem, &rect, TRUE);
		if (rect.PtInRect(pos))
		{
			SelectItem(m_HitItem);
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

void CTree::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int count = 0;
	HTREEITEM firstG = GetChildItem(GetRootItem());
	while (firstG)
	{
		if (GetChildItem(firstG))
			count++;
		firstG = GetNextSiblingItem(firstG);
	}
	CInitialConfig *pi = (CInitialConfig *)GetParent();
	CString string, string1;
	pi->m_RowsEdit.GetWindowText(string);
	int rows = ::atoi(string);
	pi->m_yGridCountEdit.GetWindowText(string1);
	while ((::atoi(string1) + 1) * count > rows)
		rows++;
	string.Format("%d", rows);
	pi->m_RowsEdit.SetWindowText(string);
	*pResult = 0;
}
void CTree::SetChnInit(CString fPath,CString eType,CString eParam,int sFreq)
{
	filePath=fPath;
	examType=eType;
	examParam=eParam;
	samplFreq=sFreq;
}