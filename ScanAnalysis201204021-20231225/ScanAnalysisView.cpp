// ScanAnalysisView.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "ScanAnalysisView.h"


// CScanAnalysisView

IMPLEMENT_DYNCREATE(CScanAnalysisView, CFormView)

CScanAnalysisView::CScanAnalysisView()
: CFormView(CScanAnalysisView::IDD)
{

}

CScanAnalysisView::~CScanAnalysisView()
{
}

void CScanAnalysisView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScanAnalysisView, CFormView)
	//	ON_COMMAND(ID_FILE_OPEN, &CScanAnalysisView::OnFileOpen)
	ON_WM_SIZE()
//	ON_COMMAND(ID_Point, &CScanAnalysisView::OnPoint)
//ON_COMMAND(ID_UnZoomF, &CScanAnalysisView::OnUnzoomf)
//ON_WM_CLOSE()
END_MESSAGE_MAP()


// CScanAnalysisView 诊断

#ifdef _DEBUG
void CScanAnalysisView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CScanAnalysisView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScanAnalysisView 消息处理程序

//void CScanAnalysisView::OnFileOpen()
//{
//
//}

void CScanAnalysisView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
}

void CScanAnalysisView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

//ShowScrollBar(SB_BOTH,FALSE); 
	// TODO: 在此处添加消息处理程序代码
}

//void CScanAnalysisView::OnPoint()
//{
//	// TODO: 在此添加命令处理程序代码
//}

//void CScanAnalysisView::OnUnzoomf()
//{
//	// TODO: 在此添加命令处理程序代码
//}

//void CScanAnalysisView::OnClose()
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CFormView::OnClose();
//}
