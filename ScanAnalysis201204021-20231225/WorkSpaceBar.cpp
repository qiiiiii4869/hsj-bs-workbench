// workspace.cpp : implementation of the CWorkSpaceBar class
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "WorkSpaceBar.h"
#include "HSJAppearance.h"
#include "ScanSel/SysConf.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;
/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar

BEGIN_MESSAGE_MAP(CWorkSpaceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkSpaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
//	ON_COMMAND(ID_ALLGRAPHVISUAL, &CWorkSpaceBar::OnAllgraphvisual)
ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar construction/destruction

CWorkSpaceBar::CWorkSpaceBar()
{
	// TODO: add one-time construction code here
	fileCount=0;
}

CWorkSpaceBar::~CWorkSpaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar message handlers

int CWorkSpaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
		TVS_LINESATROOT | TVS_HASBUTTONS|TVS_SHOWSELALWAYS;
	
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	const int originalItemHeight = m_wndTree.GetItemHeight();
	m_wndTree.SetFont(HSJUIFont());
	m_wndTree.SetItemHeight(originalItemHeight);
	m_wndTree.SetBkColor(RGB(255, 255, 255));
	m_wndTree.SetTextColor(HSJInk());
	m_wndTree.SetLineColor(HSJBorder());

	// Setup trees content:
	//HTREEITEM hRoot1 = m_wndTree.InsertItem (_T("Root 1"));
	//m_wndTree.InsertItem (_T("Item 1"), hRoot1);
	//m_wndTree.InsertItem (_T("Item 2"), hRoot1);
	m_wndTreeImages.Create (16,16,ILC_COLOR|ILC_MASK,5,5);//创建图标列表
	m_wndTreeImages.Add (AfxGetApp()->LoadIcon (IDI_EXAM_ROOT));
	m_wndTreeImages.Add (AfxGetApp()->LoadIcon (IDI_EXAM_E));
	m_wndTreeImages.Add (AfxGetApp()->LoadIcon (IDI_EXAM_T));
	m_wndTreeImages.Add (AfxGetApp()->LoadIcon (IDI_EXAM_P));
	m_wndTreeImages.Add (AfxGetApp()->LoadIcon (IDI_EXAM_C));


	hRoot = m_wndTree.InsertItem (_T("<试验数据>"), 0, 0);
	m_wndTree.SetItemState (hRoot, TVIS_BOLD, TVIS_BOLD);
	//m_wndTree.SetImageList (&m_wndTreeImages, TVSIL_NORMAL);//树形控件中不加图片//Marshall  2011.2.28 11:13
	return 0;
}

void CWorkSpaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkSpaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	HSJBorder(), HSJBorder());
}
//通道名填充到数据操作目录树
void CWorkSpaceBar::FillTree(void)
{
	int i=0,j=0,k=0,m=0;
	int cout=0;
	CString strCout,temp;
	//m_wndTree.DeleteAllItems();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	HTREEITEM hSrc;
	HTREEITEM hInc;
	HTREEITEM hRes;

	char para_str[128];
	memset(para_str,0,128);
	
	try
	{
		int itmp = 0;
		unsigned short stmp = 0;
		fileCount ++;
		strCout=pCMF->fileData->AddCoutToString(fileCount,_T(CSysConf::m_Sort_Data->m_ExamName));
		hSrc = m_wndTree.InsertItem (strCout, 1, 1, hRoot);
		
		int etcnt=0;
		for(j=0;j<CSysConf::m_Sort_Data->m_ExamTypeNum;j++)
		{
			etcnt++;
			strCout=pCMF->fileData->AddCoutToString(etcnt,_T(CSysConf::m_Sort_Data->m_ExamType[j]));
			hInc = m_wndTree.InsertItem (strCout, 2, 2, hSrc);
			int ecnt=0;
			m_wndTree.vectorTreeName.push_back(_T(CSysConf::m_Sort_Data->m_ExamType[j]));
			for(k=0;k<CSysConf::m_Sort_Data->m_ExamTypeParaNum[j];k++)
			{
				ecnt++;
				strCout=pCMF->fileData->AddCoutToString(ecnt,_T(CSysConf::m_Sort_Data->m_ExamTypePara[j][k]));
				hRes = m_wndTree.InsertItem (strCout, 3, 3, hInc);
				
				int cnt=0;
				//m_wndTree.vectorTreeName.push_back(_T(CSysConf::m_Sort_Data->m_ExamTypePara[j][k]));
				for(m=0;m<CSysConf::m_Sort_Data->m_chnNum;m++)
				{
					stmp = CSysConf::m_Exam_Idx->GetChnIdxByID(CSysConf::m_Sort_Data->m_chnIDArr[m]);
					if(stmp<0||stmp>=MAX_CHANNEL_NUM) continue;
					cnt++;
					strCout=pCMF->fileData->AddCoutToString(cnt,_T(CSysConf::m_Sort_Data->m_chnNameArr[m]));
					m_wndTree.InsertItem (strCout, 4, 4, hRes);				
				}
				m_wndTree.Expand (hRes, TVE_COLLAPSE);
			}
			m_wndTree.Expand (hInc, TVE_EXPAND);
		}
		m_wndTree.Expand (hSrc, TVE_EXPAND);
		m_wndTree.Expand (hRoot, TVE_EXPAND);
	}
	catch (CException* e)
	{
		AfxMessageBox("分拣文件数据错误，打开失败！");
		pCMF->m_wndOutput.FillText("分拣文件数据错误，打开失败！");
	}


}
//void CWorkSpaceBar::OnAllgraphvisual()
//{
//	// TODO: 在此添加命令处理程序代码
//}

void CWorkSpaceBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
	SetFocus();
}


/*--------------------------Marshall's Code2011.2.20 20:2--------------*/
int CWorkSpaceBar::ClearTree(void)
{
//
//HTREEITEM hRoot = m_wndTree.GetRootItem();
//if(!hRoot) return -1;
	m_wndTree.DeleteAllItems();

	m_wndTree.vectorSelName.clear();
	m_wndTree.vectorTreeName.clear();

	fileCount = 0; 

	hRoot = m_wndTree.InsertItem (_T("<试验数据>"), 0, 0);

	if(!hRoot) return -1;

	m_wndTree.SetItemState (hRoot, TVIS_BOLD, TVIS_BOLD);
	//m_wndTree.SetImageList (&m_wndTreeImages, TVSIL_NORMAL);//树形控件不设置图标//Marshall  2011.2.28 11:25


	return 0;
}
/*--------------------------Marshall's Code2011.2.20 20:2--------------*/
