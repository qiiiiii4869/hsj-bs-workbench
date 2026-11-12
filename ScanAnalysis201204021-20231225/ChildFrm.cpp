// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "ScanAnalysis.h"
#include "MainFrm.h"
#include "WorkSpaceBar.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CBCGPMDIChildWnd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame():titles(_T(""))
{
	//CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//pCMF->frame.push_back(this);
}

CChildFrame::~CChildFrame()
{
}

//BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
//{
//	//return TRUE;
//	return m_wndSplitter.Create(this,
//		2, 2,			// TODO: adjust the number of rows, columns
//		CSize(10, 10),	// TODO: adjust the minimum pane size
//		pContext);
//}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers

void CChildFrame::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	CRect rc; 
	GetClientRect(&rc); 
	MoveWindow(&rc,true);
	CMDIChildWnd::PreSubclassWindow();
}

void CChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
	SetWindowText(titles);

	CBCGPMDIChildWnd::OnUpdateFrameTitle(bAddToTitle);
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(GetMDIFrame()->MDIGetActive())   
		CMDIChildWnd::ActivateFrame(nCmdShow);     
	else   
		CMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);   //   else   open   maximized.   

	
	//CMDIChildWnd::ActivateFrame(nCmdShow);
}
void CChildFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	GetWindowText(str);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	std::vector<CString>::iterator it=pCMF->m_wndWorkSpace.m_wndTree.vectorSelName.begin();
	
	for (;it<pCMF->m_wndWorkSpace.m_wndTree.vectorSelName.end();it++)
	{

		if (*it==str)
		{
			pCMF->m_wndWorkSpace.m_wndTree.vectorSelName.erase(it);
			break;
		}
	}
	
	std::vector<CChildFrame*>::iterator cfv=pCMF->childFV.begin();
	for (;cfv!=pCMF->childFV.end();cfv++)
	{
		if ((*cfv)->titles==str)
		{
			pCMF->childFV.erase(cfv);
			break;
		}
	}
			
	//std::vector<CChildFrame*>::iterator itF=pCMF->frame.begin();
	//for (;itF!=pCMF->frame.end();itF++)
	//{
	//	if (*itF==this)
	//	{
	//		pCMF->frame.erase(itF);
	//		break;
	//	}
	//}
	//if (!isColse)
	//{
	//	std::vector<CChildFrame*>::iterator itF=pCMF->frame.begin();
	//	for (;itF!=pCMF->frame.end();itF++)
	//	{
	//		if (*itF==this)
	//		{
	//			pCMF->frame.erase(itF);
	//			break;
	//		}
	//	}
	//}
	CBCGPMDIChildWnd::OnClose();
}
