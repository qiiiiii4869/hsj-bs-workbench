#include "stdafx.h"
#include "ScanAnalysis.h"
#include "MainFrm.h"
#include "OutputView.h"
#include "HSJAppearance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputViewBar::COutputViewBar()
{
}

COutputViewBar::~COutputViewBar()
{
}

BEGIN_MESSAGE_MAP(COutputViewBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(COutputViewBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	
	//ON_COMMAND(ID_SAVERECORDTOTEXT, &COutputViewBar::OnSaverecordtotext)
END_MESSAGE_MAP()
//创建视窗对象
int COutputViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	LOGFONT font = {};
	HSJUIFont()->GetLogFont(&font);
	m_Font.CreateFontIndirect(&font);

	CRect rectClient (0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	// Create output pane:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutput.Create (dwStyle, rectClient, this, 1))
	{
		TRACE0("Failed to create output window\n");
		return -1;      // fail to create
	}

	m_wndOutput.SetFont (&m_Font);

	return 0;
}
//改变视窗大小
void COutputViewBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

    CRect rc;
    GetClientRect(rc);

	m_wndOutput.SetWindowPos(NULL,
            rc.left + 1, rc.top + 1,
            rc.Width() - 2, rc.Height () - 2,
            SWP_NOACTIVATE | SWP_NOZORDER );

	AdjusrHorzScroll (m_wndOutput);
}
//竖直滚动条
void COutputViewBar::AdjusrHorzScroll (CListBox& wndListBox)
{
	CClientDC dc (this);
	CFont* pOldFont = dc.SelectObject (&m_Font);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount (); i ++)
	{
		CString strItem;
		wndListBox.GetText (i, strItem);

		cxExtentMax = max (cxExtentMax, dc.GetTextExtent (strItem).cx);
	}
	
	wndListBox.SetHorizontalExtent (cxExtentMax);
	dc.SelectObject (pOldFont);
}
CString logFileCreteTime="";//是指文件创建时间
//填充文本
void COutputViewBar::FillText(CString str)
{
	CTime tm;
	CString timeStr;
	tm=CTime::GetCurrentTime();
	timeStr=tm.Format("%Y-%m-%d %H-%M-%S");
	if (logFileCreteTime=="")
	{
		logFileCreteTime=timeStr;
	}

	m_wndOutput.AddString (timeStr+": "+str);
	AutoSaveRecord(((CMainFrame*)AfxGetMainWnd())->fileData->ReturnCurrentPath()+"\\log\\"+"操作日志  "+logFileCreteTime+".log",timeStr+": "+str);
	//m_wndOutput.AddString (_T("Enjoy using it!"));

	AutoScroll();
}
//自动滚动滚动条
void COutputViewBar::AutoScroll()
{
	int   minpos,maxpos;   
	m_wndOutput.GetScrollRange(SB_VERT,&minpos,&maxpos);   
	m_wndOutput.SetScrollPos(SB_VERT,maxpos);   

	int nCount = m_wndOutput.GetCount(); 
	m_wndOutput.SetCurSel(nCount-1); 
	//m_wndOutput.SetCurSel(-1); 
}
//重绘
void COutputViewBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndOutput.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (1, 1);
	dc.Draw3dRect (rectTree, HSJBorder(), HSJBorder());
}
//处理上下文菜单
void COutputViewBar::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;

	VERIFY(menu.LoadMenu (IDR_SAVERECORD));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x, point.y, AfxGetMainWnd());

	menu.Detach();
	//SetFocus ();
}
//获取当前的时刻
CString COutputViewBar::GetCurTimeStr()
{
	CTime tm;
	CString timeStr;
	tm=CTime::GetCurrentTime();
	timeStr=tm.Format("%Y-%m-%d %H-%M-%S");
	return timeStr;
}

//保存操作日志
void COutputViewBar::SaveRecord()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString strFilter="日志文件 (*.LOG) | *.log;*.LOG ||";
	CFileDialog FileOpenDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT, strFilter);
	
	CString  strFileName="操作日志 "+GetCurTimeStr();
	LPTSTR   pszfilename;   
	pszfilename=strFileName.GetBuffer(_MAX_PATH);
	FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle   =   "保存操作日志";  
	if(FileOpenDlg.DoModal()==IDOK)
	{		
		CString strSaveFilapath = FileOpenDlg.GetPathName()+".log";
		pCMF->m_wndOutput.FillText(_T("保存操作日志文件完成，存储为："+strSaveFilapath));

		FILE * f=fopen(strSaveFilapath,"w");
		if(f!=0)
		{	
			CString rcdStr;
			for (int p=0;p<m_wndOutput.GetCount();p++)
			{
				m_wndOutput.GetText(p,rcdStr);
				fprintf(f,"%s\n",rcdStr);
			}			
		}	

		fseek(f,0,SEEK_END);
		fclose(f);

	}
	else
	{
		pCMF->m_wndOutput.FillText(_T("取消保存操作日志文件！"));
	}
}

//自动保存操作日志
void COutputViewBar::AutoSaveRecord(CString path,CString rcdStr)
{

	FILE * f=fopen(path,"a+");
	if(f!=0)
	{	
		fprintf(f,"%s\n",rcdStr);
	}	

	fseek(f,0,SEEK_END);
	fclose(f);

}