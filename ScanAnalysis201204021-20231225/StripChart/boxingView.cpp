// boxingView.cpp : implementation of the CBoxingView class
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"

#include "..\ScanAnalysisDoc.h"
#include "boxingView.h"
#include "Config1.h"
#include "Configzuobiao.h"
#include "ConfigTitle.h"
#include "ConfigX.h"
#include "SaveImageDlg.h"
#include "GroupSel.h"
#include "AddGroup.h"
#include "math.h"
#include "..\MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoxingView
static float StartTime;
static float m_EndTime;
static int Columns;
IMPLEMENT_DYNCREATE(CBoxingView, CScrollView)

BEGIN_MESSAGE_MAP(CBoxingView, CScrollView)
	//{{AFX_MSG_MAP(CBoxingView)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENUITEMSetRange, OnSetRange)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MENUITEMSaveToImage, OnSaveToImage)
	ON_COMMAND(ID_MENUITEMAddSignal, OnAddSignal)
	ON_WM_VSCROLL()
	ON_COMMAND(ID_MENUITEMGroup, OnAddGroup)
	//}}AFX_MSG_MAP
//	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBoxingView construction/destruction

CBoxingView::CBoxingView()/*:isConfigX(FALSE)*/
{
	// TODO: add construction code here
	saveflag = 0;
	delflag = 0;
	flag = 0;
	m_ViewPortExt = CSize(0, 0);
	m_ViewOrg = CPoint(0, 0);
}

CBoxingView::~CBoxingView()
{
}

BOOL CBoxingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBoxingView drawing
void CBoxingView::DrawGraph()
{
	SetScrollSizes(MM_TEXT, m_ViewPortExt);
	
}
//消息：重绘
void CBoxingView::OnDraw(CDC* pDC)
{
	//this->GetParentFrame()->SetWindowText(m_title);

	CRect rect;
	GetClientRect(&rect);
	//pDC->GetClipBox(rect);
	CDC dcMem;
	CBitmap bmp;
	//bmp.Attach(hbmp); 
	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, (m_ViewPortExt.cx > rect.Width())? m_ViewPortExt.cx : rect.Width(), (m_ViewPortExt.cy > rect.Height())? m_ViewPortExt.cy : rect.Height());
	dcMem.SelectObject(&bmp);
	dcMem.FillSolidRect(0,0,(m_ViewPortExt.cx > rect.Width())? m_ViewPortExt.cx : rect.Width(), (m_ViewPortExt.cy > rect.Height())? m_ViewPortExt.cy : rect.Height(), RGB(255, 255, 255)); 
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.Draw3dRect(0, 0, m_ViewPortExt.cx, m_ViewPortExt.cy, HSJBorder(), HSJBorder());

	GriWidth = (m_ViewPortExt.cx - 130) / m_Columns;
	GriHeight = (m_ViewPortExt.cy - 130) / m_Rows;
	startx = (m_ViewPortExt.cx - 130 - GriWidth * m_Columns) / 2;
	starty = (m_ViewPortExt.cy - 130 - GriHeight * m_Rows) / 2;
	m_ViewOrg.x = 60 + startx;
	m_viewportx = m_ViewPortExt.cx - 130 - 2 * startx;	

	CFont font1;
	font1.CreatePointFont(110, _T("Microsoft YaHei UI"));
	//font1.CreatePointFont(200, "Arial");//cxy 20101126
	dcMem.SelectObject(&font1);
	CPen dotpen(PS_SOLID, 1, RGB(232, 237, 243));
	CPen *poldpen = dcMem.SelectObject(&dotpen);
	for (int i = 60 + startx; i < m_ViewPortExt.cx - 70; i = i + GriWidth)
	{
		dcMem.MoveTo(i, 70);
		dcMem.LineTo(i, m_ViewPortExt.cy - 61);
	}
	for (int j = 70 + starty; j < m_ViewPortExt.cy - 60; j = j + GriHeight)
	{
		dcMem.MoveTo(60, j);
		dcMem.LineTo(m_ViewPortExt.cx - 70, j);
	}
	CPen axisPen(PS_SOLID, 1, HSJBorder());
	dcMem.SelectObject(&axisPen);
	dcMem.MoveTo(60, 70);
	dcMem.LineTo(m_ViewPortExt.cx - 71, 70);
	dcMem.LineTo(m_ViewPortExt.cx - 71, m_ViewPortExt.cy - 61);
	dcMem.LineTo(60, m_ViewPortExt.cy - 61);
	dcMem.LineTo(60, 70);

	dcMem.SetTextAlign(TA_CENTER);
	dcMem.MoveTo(60 + startx, m_ViewPortExt.cy - 59);
	dcMem.LineTo(m_ViewPortExt.cx - 70 - startx, m_ViewPortExt.cy - 59);

	float start = m_StartTime;
	CString stringz;
	int zindex = 0;
	for (int i = 60 + startx; i < m_ViewPortExt.cx - 70; i = i + GriWidth)
	{
		dcMem.MoveTo(i, m_ViewPortExt.cy - 59);
		dcMem.LineTo(i, m_ViewPortExt.cy - 52);			
		stringz.Format("%g", m_StartTime + zindex * xinterval);
		dcMem.TextOut(i, m_ViewPortExt.cy - 49, stringz);
		zindex++;
	}

	dcMem.TextOut(m_ViewPortExt.cx - 40, m_ViewPortExt.cy - 50, "秒");	
	dcMem.SetTextAlign(TA_LEFT);
	for (int yid = 0; yid < boxing.GetSize(); yid++)
	{	
		BOXING *p = &boxing[yid];
		int offsetx = 0, offsety = 0;
		boxing[yid].vieworgY = 70 + starty + boxing[yid].startindex * GriHeight + boxing[yid].ymax / boxing[yid].yinterval * GriHeight;	
		boxing[yid].windowextY = boxing[yid].ymin - boxing[yid].ymax;
		boxing[yid].viewportextY = (boxing[yid].endindex - boxing[yid].startindex) * GriHeight;	
		while (p)
		{
			CPen pen(PS_SOLID, p->pix, p->color);
			dcMem.SelectObject(&pen);
			dcMem.SetTextColor(p->color);				

			CPoint *aPoint;
			aPoint = new CPoint[(int)((m_EndTime - m_StartTime) * p->frequency + 2)];
			int endp = m_EndTime * p->frequency < p->count? m_EndTime * p->frequency : p->count - 1;
			int startp = m_StartTime * p->frequency;
			while (p->logpoint[startp].x < m_StartTime)
				startp++;
			for (int k = startp; k <= endp; k++)
			{				
				float x = p->logpoint[k].x;
				float y = p->logpoint[k].y;
				LPtoDPEx(x - m_StartTime, y, m_EndTime - m_StartTime, boxing[yid].windowextY, CSize(m_viewportx, boxing[yid].viewportextY), CPoint(m_ViewOrg.x, boxing[yid].vieworgY + boxing[yid].offsety), aPoint[k - startp]);
			}
			if (endp - startp > 0)
				dcMem.Polyline(aPoint, endp - startp + 1);
			if (62 + startx + offsetx + dcMem.GetTextExtent(p->title).cx > m_ViewPortExt.cx - 70)
			{
				offsety += dcMem.GetTextExtent(p->title).cy + 1;
				offsetx = 0;
			}
			dcMem.TextOut(62 + startx + offsetx, 75 + starty + boxing[yid].endindex * GriHeight + boxing[yid].offsety + offsety, p->title);
			offsetx += dcMem.GetTextExtent(p->title).cx + 20;
			delete[] aPoint;
			p = p->pNext;				
		}
	}

	dcMem.SelectObject(&axisPen);
	dcMem.SetTextAlign(TA_RIGHT);
	dcMem.SetTextColor(HSJInk());
	for (int m = 0; m < boxing.GetSize(); m++)
	{
		dcMem.MoveTo(58 + (boxing[m].pos + 1) * GriWidth, 70 + starty + boxing[m].startindex * GriHeight + boxing[m].offsety);
		dcMem.LineTo(58 + (boxing[m].pos + 1) * GriWidth, 70 + starty + boxing[m].endindex * GriHeight + boxing[m].offsety);
		int count = 0;
		CString string;
		for (int m1 = 70 + starty + boxing[m].endindex * GriHeight; m1 >= 70 + starty + boxing[m].startindex * GriHeight; m1 = m1 - GriHeight)
		{			
			string.Format("%0.1f", boxing[m].ymin + boxing[m].yinterval * count);
			count++;
			dcMem.MoveTo(58 + (boxing[m].pos + 1) * GriWidth, m1 + boxing[m].offsety);
			dcMem.LineTo(53 + (boxing[m].pos + 1) * GriWidth, m1 + boxing[m].offsety);
			dcMem.TextOut(48 + (boxing[m].pos + 1) * GriWidth, m1 - 6 + boxing[m].offsety, string);//////////////////////////////////////////////////////////////////////////
		}
	}
	CFont font;
	//font.CreatePointFont(180, _T("Microsoft YaHei UI"));
	font.CreatePointFont(180, _T("Microsoft YaHei UI")); //cxy 20101126
	dcMem.SelectObject(&font);
	dcMem.SetTextAlign(TA_CENTER);
	dcMem.TextOut(m_ViewPortExt.cx / 2, 30, m_title);	

	pDC->BitBlt(0, 0, (m_ViewPortExt.cx > rect.Width())? m_ViewPortExt.cx : rect.Width(), (m_ViewPortExt.cy > rect.Height())? m_ViewPortExt.cy : rect.Height(), &dcMem, 0, 0, SRCCOPY);

	if (saveflag == 1)
	{
		CDC dcMem1;
		CBitmap bmp1;
		dcMem1.CreateCompatibleDC(pDC);
		bmp1.CreateCompatibleBitmap(pDC, m_ViewPortExt.cx, m_ViewPortExt.cy);
		dcMem1.SelectObject(&bmp1);
		dcMem1.BitBlt(0, 0, m_ViewPortExt.cx, m_ViewPortExt.cy, &dcMem, 0, 0, SRCCOPY);
		SaveBitMapToFile((HBITMAP)bmp1.GetSafeHandle(), savename);
		saveflag = 0;
	}

	dcMem.SelectObject(poldpen);
	dcMem.DeleteDC();
	bmp.DeleteObject();	
}

void CBoxingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	//this->GetParentFrame()->SetWindowText(m_title);
	
}

/////////////////////////////////////////////////////////////////////////////
// CBoxingView diagnostics

#ifdef _DEBUG
void CBoxingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CBoxingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CBoxingDoc* CBoxingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanAnalysisDoc)));
	return (CBoxingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBoxingView message handlers
void CBoxingView::LPtoDPEx(float xlog, float ylog, float WindowExtX, float WindowExtY, CSize ViewPortExt, CPoint ViewOrg, CPoint& point)
{
	point.x = (int)(xlog * ViewPortExt.cx / WindowExtX) + ViewOrg.x;
	point.y = (int)(ylog * ViewPortExt.cy / WindowExtY) + ViewOrg.y;
}

void CBoxingView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CPoint pos = point;
	ScreenToClient(&pos);
	pos.x += GetDeviceScrollPosition().x;
	pos.y += GetDeviceScrollPosition().y;

	CRect title(0, 0, m_ViewPortExt.cx, 65);
	if (title.PtInRect(pos))
	{
		CMenu menu;
		menu.LoadMenu(IDR_StripChartTitleMenu);
		CMenu *pContextMenu = menu.GetSubMenu(0);
		int nCmd = (int)pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, AfxGetMainWnd());
		//修改标题
		if (nCmd == ID_MENUITEMSetTitle)
		{
			CConfigTitle configtitle;
			configtitle.m_title = m_title;
			if (configtitle.DoModal() == IDOK)
			{		
				((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：修改标题");
				m_title = configtitle.m_title;
				Invalidate();
			}	
			else
				((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：取消修改标题");

			return;
		}
	}

	CRect x(0, m_ViewPortExt.cy - 60, m_ViewPortExt.cx, m_ViewPortExt.cy);
	if (x.PtInRect(pos))
	{
		CMenu menu;
		menu.LoadMenu(IDR_StripChartXSetMenu);
		CMenu *pContextMenu = menu.GetSubMenu(0);
		int nCmd = (int)pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, AfxGetMainWnd());
		//设置X轴坐标范围
		if (nCmd == ID_MENUITEMSetXAxis)
		{

			CConfigX configx;
			configx.m_StartTime = m_StartTime;
			configx.m_EndTime = m_EndTime;
			configx.m_Columns = m_Columns;
			if (configx.DoModal() == IDOK)
			{		
				((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：设置X轴坐标范围");
				m_StartTime = configx.m_StartTime;
				m_EndTime = configx.m_EndTime;
				m_Columns = configx.m_Columns;
				xinterval = (m_EndTime - m_StartTime) / m_Columns;
				/*AdjustZuobiao(xinterval, m_StartTime, m_EndTime, m_Columns);*/
				Invalidate();
			}
			else
				((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：取消设置X轴坐标范围");
			return;
		}
	}
	for (int i = 0; i < boxing.GetSize(); i++)
	{
		selindex = i;
		CRect zuobiao;
		zuobiao.left = 10 + (boxing[i].pos + 1) * GriWidth;
		zuobiao.right = 59 + (boxing[i].pos + 1) * GriWidth;
		zuobiao.top = 70 + starty + boxing[i].startindex * GriHeight;
		zuobiao.bottom = 70 + starty + boxing[i].endindex * GriHeight;
		if (zuobiao.PtInRect(pos))
		{
			CMenu menu;
			menu.LoadMenu(IDR_StripChartAxisMenu);
			CMenu *pContextMenu = menu.GetSubMenu(0);
			int nCmd = (int)pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, point.x, point.y, this);
			//设置波形及坐标信息
			if (nCmd == ID_MENUITEMSetAxis)
			{
				CConfigzuobiao configzuobiao;
				configzuobiao.m_Title = boxing[i].title;
				configzuobiao.m_StartIndex = boxing[i].startindex;
				configzuobiao.m_EndIndex = boxing[i].endindex;
				configzuobiao.m_ymax = boxing[i].ymax;
				configzuobiao.m_ymin = boxing[i].ymin;
				configzuobiao.m_pix = boxing[i].pix;				
				configzuobiao.m_color.SetBkColor(boxing[i].color);
				configzuobiao.m_color.m_colorsel = boxing[i].color;	

				if (configzuobiao.DoModal() == IDOK)
				{						
					((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：设置波形及坐标信息");				
					boxing[i].startindex = configzuobiao.m_StartIndex;
					boxing[i].endindex = configzuobiao.m_EndIndex;
					boxing[i].ymax = configzuobiao.m_ymax;
					boxing[i].ymin = configzuobiao.m_ymin;					
					boxing[i].yinterval = (boxing[i].ymax - boxing[i].ymin) / (boxing[i].endindex - boxing[i].startindex);
					int s = configzuobiao.m_Signal;
					BOXING *p = &boxing[i];
					while (s)
					{
						p = p->pNext;
						s--;
					}
					p->title = configzuobiao.m_Title;
					p->pix = configzuobiao.m_pix;
					p->color = configzuobiao.m_color.m_colorsel;
					//AdjustZuobiao(boxing[i].yinterval, boxing[i].ymin, boxing[i].ymax, boxing[i].endindex - boxing[i].startindex);
					Invalidate();
				}
				else
					((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：取消设置波形及坐标信息");				
			}
			else if (nCmd == ID_MENUITEMDeleteSign)
			{
				if(MessageBox("确定要执行删除操作吗?", 0, MB_ICONQUESTION|MB_YESNO) == IDYES)
				{	
					((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：删除所选通道波形");
					int dis = boxing[i].endindex - boxing[i].startindex + 1;
					for (int j = i + 1; j < boxing.GetSize(); j++)
					{
						boxing[j].startindex -= dis;
						boxing[j].endindex -= dis;
					}
					boxing.RemoveAt(i);				 
					Invalidate();
				}
				else
					((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：取消删除所选通道波形");				
			}
			return;
		}
	}

	CRect client;
	client.left = 61;
	client.top = 71;
	client.right = m_ViewPortExt.cx - 71;
	client.bottom = m_ViewPortExt.cy - 61;
	if (client.PtInRect(pos))
	{
		CMenu menu;
		menu.LoadMenu(IDR_StripChartGraphMenu);
		CMenu *pContextMenu = menu.GetSubMenu(0);
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
		return;
	}
	CWnd::OnContextMenu(pWnd, point);
}
//消息：设置波形区域信息
void CBoxingView::OnSetRange() 
{
	// TODO: Add your command handler code here
	CConfig1 configdlg1;
	configdlg1.m_Height = m_ViewPortExt.cy;
	configdlg1.m_Width = m_ViewPortExt.cx;
	configdlg1.m_Rows = m_Rows;
	configdlg1.m_Columns = m_Columns;
	if (configdlg1.DoModal() == IDOK)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：设置波形区域信息...");
		m_ViewPortExt.cy = configdlg1.m_Height;
		m_ViewPortExt.cx = configdlg1.m_Width;
		m_Rows = configdlg1.m_Rows;
		m_Columns = configdlg1.m_Columns;

		/*--------------------------Marshall's Code2011.3.3 20:28--------------*/
		//尝试调整界面 
			CSize sizeTotal;
	sizeTotal.cx = configdlg1.m_Width ;
	sizeTotal.cy = configdlg1.m_Height ;

	SetScrollSizes(MM_TEXT, sizeTotal);

	ResizeParentToFit( ); 
		/*--------------------------Marshall's Code2011.3.3 20:28--------------*/
		Invalidate();
	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：取消设置波形区域信息");
}

void CBoxingView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pos = point;
	pos.x += GetDeviceScrollPosition().x;
	pos.y += GetDeviceScrollPosition().y;
	for (int i = 0; i < boxing.GetSize(); i++)
	{
		CRect zuobiao;
		zuobiao.left = 10 + (boxing[i].pos + 1) * GriWidth;
		zuobiao.right = 59 + (boxing[i].pos + 1) * GriWidth;
		zuobiao.top = 70 + starty + boxing[i].startindex * GriHeight;
		zuobiao.bottom = 70 + starty + boxing[i].endindex * GriHeight;
		if (zuobiao.PtInRect(pos))
		{
			SetCapture();
			oldpoint = point;
			oldpoint2 = point;
			moveindex = i;
			HCURSOR hCurMove = ::LoadCursor(NULL, IDC_SIZEALL);
			::SetCursor(hCurMove);
			break;
		}
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CBoxingView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int flag = 0;	
	CPoint pos = point;
	pos.x += GetDeviceScrollPosition().x;
	pos.y += GetDeviceScrollPosition().y;
	for (int i = 0; i < boxing.GetSize(); i++)
	{
		CRect zuobiao;
		zuobiao.left = 10 + (boxing[i].pos + 1) * GriWidth;
		zuobiao.right = 59 + (boxing[i].pos + 1) * GriWidth;
		zuobiao.top = 70 + starty + boxing[i].startindex * GriHeight;
		zuobiao.bottom = 70 + starty + boxing[i].endindex * GriHeight;
		if (zuobiao.PtInRect(pos))
		{			
			HCURSOR hCurMove = ::LoadCursor(NULL, IDC_SIZEALL);
			::SetCursor(hCurMove);
			flag = 1;
			break;
		}
	}
	if (!flag)
	{
		HCURSOR hCurDef = ::LoadCursor(NULL, IDC_ARROW);	
		::SetCursor(hCurDef);
	}
	if (GetCapture() == this)
	{
		HCURSOR hCurMove = ::LoadCursor(NULL, IDC_SIZEALL);
		::SetCursor(hCurMove);
		if (nFlags & MK_SHIFT)
		{
			CClientDC dc(this);
			dc.SetROP2(R2_NOTXORPEN);
			CPen pen(PS_DOT, 0, RGB(0, 0, 0));
			dc.SelectObject(&pen);
			CRect zuobiao;
			zuobiao.left = 10;
			zuobiao.right = 57;
			zuobiao.top = 70 + starty + boxing[moveindex].startindex * GriHeight - GetDeviceScrollPosition().y;
			zuobiao.bottom = 70 + starty + boxing[moveindex].endindex * GriHeight - GetDeviceScrollPosition().y + oldpoint.y - oldpoint2.y;
			if (oldpoint == oldpoint2)
				dc.Rectangle(&zuobiao);	
			if ((boxing[moveindex].endindex - boxing[moveindex].startindex) * GriHeight + point.y - oldpoint2.y > GriHeight)
			{
				dc.Rectangle(&zuobiao);
				zuobiao.bottom = 70 + starty + boxing[moveindex].endindex * GriHeight - GetDeviceScrollPosition().y + point.y - oldpoint2.y;
				dc.Rectangle(&zuobiao);			
				oldpoint = point;
			}
		}
		else
		{			
			boxing[moveindex].offsety += point.y - oldpoint.y;
			if ((point.y - oldpoint.y < 0) && (boxing[moveindex].offsety + boxing[moveindex].startindex * GriHeight < 0))
			{
				boxing[moveindex].offsety -= point.y - oldpoint.y;
			}
			else if ((point.y - oldpoint.y > 0) && (boxing[moveindex].offsety + boxing[moveindex].endindex * GriHeight > m_ViewPortExt.cy - 130 - 2 * starty))
			{
				boxing[moveindex].offsety -= point.y - oldpoint.y;
			}
			else
			{
				oldpoint.y = point.y;
				Invalidate();
			}
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CBoxingView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	if (nFlags & MK_SHIFT)
	{
		boxing[moveindex].endindex = ((boxing[moveindex].endindex - boxing[moveindex].startindex) * GriHeight + oldpoint.y - oldpoint2.y) / GriHeight + boxing[moveindex].startindex;
		boxing[moveindex].yinterval = (boxing[moveindex].ymax - boxing[moveindex].ymin) / (boxing[moveindex].endindex - boxing[moveindex].startindex);
		//AdjustZuobiao(boxing[moveindex].yinterval, boxing[moveindex].ymin, boxing[moveindex].ymax, boxing[moveindex].endindex - boxing[moveindex].startindex);
	}
	else
	{
		for (int i = 0; i < boxing.GetSize(); i++)
		{
			if (boxing[i].offsety)
			{
				if (boxing[i].offsety > 0)
				{
					if (boxing[i].offsety - (boxing[i].offsety / GriHeight) * GriHeight > GriHeight / 2.0)
					{	
						boxing[i].startindex += boxing[i].offsety / GriHeight + 1;
						boxing[i].endindex += boxing[i].offsety / GriHeight + 1;
					}
					else
					{
						boxing[i].startindex += boxing[i].offsety / GriHeight;
						boxing[i].endindex += boxing[i].offsety / GriHeight;
					}
				}
				else
				{
					if (boxing[i].offsety - (boxing[i].offsety / GriHeight) * GriHeight < -GriHeight / 2.0)
					{	
						boxing[i].startindex += boxing[i].offsety / GriHeight - 1;
						boxing[i].endindex += boxing[i].offsety / GriHeight - 1;
					}
					else
					{
						boxing[i].startindex += boxing[i].offsety / GriHeight;
						boxing[i].endindex += boxing[i].offsety / GriHeight;
					}
				}
				boxing[i].offsety = 0;				
				break;
			}
		}
	}
	Invalidate();
	CScrollView::OnLButtonUp(nFlags, point);
}

BOOL CBoxingView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	
	return true;
	//return CView::OnEraseBkgnd(pDC);
}
//消息：保存视图到图象
void CBoxingView::OnSaveToImage() 
{
	saveflag = 1;
	Invalidate();
}
//保存视图到图象
BOOL CBoxingView::SaveBitMapToFile(HBITMAP hBitmap, CString lpFileName)
{

	CImage image; 
	image.Attach(hBitmap); 
	CString filePath;
	CString strFilter;
	strFilter = _T("BMP文件(*.BMP)|*.BMP|JPEG文件(*.JPG)|*.JPG|GIF文件(*.GIF)|*GIF|PNG文件(*.PNG)|*.PNG|TIFF文件(*.TIFF)|*.TIFF||");

	CFileDialog FileOpenDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT, strFilter);

	CString strBuff =m_title;
	LPTSTR   pszfilename;   

	//int nRight = strBuff.ReverseFind(_T('/'));
	//CString strFile = strBuff.Right(strBuff.GetLength() - nRight - 1);
	//strBuff.Delete(nRight,1);
	CString strFile = strBuff;

	//pszfilename=strBuff.GetBuffer(_MAX_PATH);
	pszfilename=strFile.GetBuffer(_MAX_PATH);

	//FileOpenDlg.m_ofn.lpstrFile   =   strBuff.GetBuffer(0);   
	FileOpenDlg.m_ofn.lpstrFile   =   strFile.GetBuffer(0);   

	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  

	if(FileOpenDlg.DoModal() == IDOK)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：保存视图到图像");

		saveflag = 0;	
		Invalidate();
		CString strSaveFilapath = FileOpenDlg.GetPathName();


		strSaveFilapath.MakeLower();


		switch(FileOpenDlg.m_ofn.nFilterIndex)
		{
		case 1:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".bmp") != 0)
				strSaveFilapath = strSaveFilapath + ".bmp";	
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatBMP);
			break;
		case 2:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".jpg") != 0)
				strSaveFilapath = strSaveFilapath + ".jpg";
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatJPEG);
			break;
		case 3:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".gif") != 0)
				strSaveFilapath = strSaveFilapath + ".gif";
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatGIF);
			break;
		case 4:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".png") != 0)
				strSaveFilapath = strSaveFilapath + ".png";
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatPNG);
			break;
		default:
			strBuff = strSaveFilapath.Right(5);
			if(strBuff.Compare(".tiff") != 0)				
				strSaveFilapath = strSaveFilapath + ".tiff";
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatTIFF);
			break;
		}

		AfxMessageBox("保存完成!");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：保存完成，存储为  "+strSaveFilapath);
	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：取消保存视图到图像");
	image.Detach(); 

	DeleteObject(hBitmap); 

	return TRUE;
}
//消息：添加通道数据
void CBoxingView::OnAddSignal() 
{
	// TODO: Add your command handler code here
	CGroupSel dialog;
	dialog.m_Mode = 1;
	dialog.SetChnInit(filePath,examType,examParam/*,sampleFreq*/);

	if (dialog.DoModal()==IDOK)
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：添加通道数据...");
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：取消添加通道数据");

	//dialog.FillList();
	Invalidate();
}
//消息：添加分组数据
void CBoxingView::OnAddGroup() 
{
	// TODO: Add your command handler code here
	CAddGroup dialog;
	dialog.SetChnInit(filePath,examType,examParam/*,sampleFreq*/);

	if (dialog.DoModal()==IDOK)
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：添加分组数据...");
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形长图操作：取消添加分组数据");

	Invalidate();
}

//长图网格初始化
void CBoxingView::SetInit(CSize vpt,int flg,int ygc,int rs,int mc,float st,float et,float xi)
{
	m_ViewPortExt = vpt;
	flag = flg;
	yGridCount = ygc;
	m_Rows = rs;
	m_Columns = mc;
	m_StartTime = st;
	m_EndTime = et;
	xinterval = xi;
	
	Invalidate(TRUE);
}
//初始化通道数据
//@CString fPath		分拣文件路径
//@CString eType		试验类型
//@CString eParam		试验参数
//@int sFreq			频率
void CBoxingView::SetChnInit(CString fPath,CString eType,CString eParam/*,int sFreq*/)
{
	filePath=fPath;
	examType=eType;
	examParam=eParam;
	//sampleFreq=sFreq;
	
}


//void CBoxingView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CScrollView::OnActivate(nState, pWndOther, bMinimized);
//
//}
//消息：激活工具栏tab
void CBoxingView::OnSetFocus(CWnd* pOldWnd)
{
	CScrollView::OnSetFocus(pOldWnd);
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	pCMF->ActivateTabEG(0);
	// TODO: 在此处添加消息处理程序代码
}
