// TrendView.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "TrendView.h"
#include "MainFrm.h"
#include "TrendGraphSet.h"
#include "SaveTrendToExcelDlg.h"
#include "TrendSelSet.h"

#include "NiWaveFraqView.h"////Marshall  2011.2.23 22:3

using namespace std;
// CTrendView

/*--------------------------Marshall's Code2011.2.24 10:51--------------*/
extern bool g_bConf97;
extern bool g_bConf95;
extern bool g_bConf100;
/*--------------------------Marshall's Code2011.2.24 10:51--------------*/

IMPLEMENT_DYNCREATE(CTrendView, CFormView)

CTrendView::CTrendView()
	: CBCGPFormView(CTrendView::IDD),showMean(FALSE),showMedian(FALSE),showRMS(FALSE),showPPvalue(FALSE)
{

}

CTrendView::~CTrendView()
{
}

void CTrendView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_CWGRAPH, m_Graph);
	DDX_Control(pDX, IDC_CWGRAPH, m_TrendGraph);
}

BEGIN_MESSAGE_MAP(CTrendView, CBCGPFormView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_TrendImageSave, &CTrendView::OnTrendimagesave)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TrendMean, &CTrendView::OnTrendmean)
	ON_UPDATE_COMMAND_UI(ID_TrendMean, &CTrendView::OnUpdateTrendmean)
	ON_COMMAND(ID_TrendMedian, &CTrendView::OnTrendmedian)
	ON_UPDATE_COMMAND_UI(ID_TrendMedian, &CTrendView::OnUpdateTrendmedian)
	ON_COMMAND(ID_TrendRMS, &CTrendView::OnTrendrms)
	ON_UPDATE_COMMAND_UI(ID_TrendRMS, &CTrendView::OnUpdateTrendrms)
	ON_COMMAND(ID_TrendPPValue, &CTrendView::OnTrendppvalue)
	ON_UPDATE_COMMAND_UI(ID_TrendPPValue, &CTrendView::OnUpdateTrendppvalue)
	ON_COMMAND(ID_SetTrendParam, &CTrendView::OnSettrendparam)
//	ON_WM_ACTIVATE()
//ON_COMMAND(ID_LoadTxtJigger, &CTrendView::OnLoadtxtjigger)
ON_COMMAND(ID_TrendReport, &CTrendView::OnTrendreport)
ON_COMMAND(ID_TrendImageSaveBat, &CTrendView::OnTrendimagesavebat)
ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CTrendView 诊断

#ifdef _DEBUG
void CTrendView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTrendView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTrendView 消息处理程序

void CTrendView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	HSJStyleChildControls(GetSafeHwnd(), m_skinFonts);
	HSJStyleGraph(m_TrendGraph);

	
	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/
	//调整界面，不出现滚动条////Marshall  2011.3.2 12:23
	SetScrollSizes(MM_TEXT,CSize(0,0));
	//InitNiGraph();
	ResizeParentToFit();
	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/

	m_TrendGraph.Plots.RemoveAll();
	double minValue, maxValue,indexDecx,indexDecy;
	unsigned int minIndex, maxIndex;
	GxData.MinMax(minValue, minIndex, maxValue, maxIndex);
	indexDecx=(maxValue-minValue)*0.01;
	indexDecy=(GppValue[ichSize-1]-Gmean[ichSize-1])*0.1;
	//m_TrendGraph.Axes.Item("XAxis").SetMinMax(minValue-indexDecx,maxValue+10*indexDecx);
	m_TrendGraph.Axes.Item("XAxis").SetMinMax(minValue,maxValue+15*indexDecx);
	
	//double minYValue, maxYValue,indexYDec;
	//unsigned int minYIndex, maxYIndex;

	//CNiReal64Vector ydata((int)GyData.size());
	//for (int p=0;p<(int)GyData.size();p++)
	//{
	//	ydata[p]=GyData[p];
	//}
	//ydata.MinMax(minYValue, minYIndex, maxYValue, maxYIndex);
	//indexYDec=(maxYValue-minYValue)*0.01;
	//m_TrendGraph.Axes.Item("YAxis-1").SetMinMax(minYValue-15*indexYDec,maxYValue+15*indexYDec);

	for (int k=0;k<(int)GV.size();k++)
	{
		m_TrendGraph.Plots.Add();	
		m_TrendGraph.Annotations.Add();


		m_TrendGraph.Plots.Item(k+1).PointStyle=CNiPlot::PointDottedSolidCircle;

		m_TrendGraph.Annotations.Item(k+1).Plot = m_TrendGraph.Plots.Item(k+1);
		m_TrendGraph.Annotations.Item(k+1).SnapMode = CNiAnnotation::SnapAnchoredToPoint;
		m_TrendGraph.Annotations.Item(k+1).Shape.Type = CNiShape::Point;

		switch (k)
		{
		case 0:
			m_TrendGraph.Plots.Item(k+1).PlotXvsY(GxData,Gmean);
			m_TrendGraph.Annotations.Item(k+1).Caption.Text = _T("平均值 ");
			m_TrendGraph.Annotations.Item(k+1).Caption.SetCoordinates(m_TrendGraph.Axes.Item("XAxis").GetMaximum()-8*indexDecx, Gmean[ichSize-1]);
			break;
		case 1:
			m_TrendGraph.Plots.Item(2).PlotXvsY(GxData,Gmedian);
			m_TrendGraph.Annotations.Item(k+1).Caption.Text = _T("中间值 ");
			m_TrendGraph.Annotations.Item(k+1).Caption.SetCoordinates(m_TrendGraph.Axes.Item("XAxis").GetMaximum()-8*indexDecx, Gmedian[ichSize-1]+2*indexDecy);

			break;
		case 2:
			m_TrendGraph.Plots.Item(3).PlotXvsY(GxData,GRMS);
			m_TrendGraph.Annotations.Item(k+1).Caption.Text = _T("有效值 ");
			m_TrendGraph.Annotations.Item(k+1).Caption.SetCoordinates(m_TrendGraph.Axes.Item("XAxis").GetMaximum()-8*indexDecx, GRMS[ichSize-1]+2*indexDecy);

			break;
		default:
			m_TrendGraph.Plots.Item(4).PlotXvsY(GxData,GppValue);
			//m_TrendGraph.Annotations.Item(k+1).Caption.Text = _T("峰峰值 ");//加置信度标记//Marshall  2011.2.24 13:15
			if(g_bConf97)
				m_TrendGraph.Annotations.Item(k+1).Caption.Text = _T("97%峰峰值 ");
			else if(g_bConf95)
				m_TrendGraph.Annotations.Item(k+1).Caption.Text = _T("95%峰峰值 ");
			else if(g_bConf100)
				m_TrendGraph.Annotations.Item(k+1).Caption.Text = _T("100%峰峰值 ");

			m_TrendGraph.Annotations.Item(k+1).Caption.SetCoordinates(m_TrendGraph.Axes.Item("XAxis").GetMaximum()-8*indexDecx, GppValue[ichSize-1]);

			break;
		}		
		m_TrendGraph.Plots.Item(k+1).LineColor=GV[k].colors;
		m_TrendGraph.Plots.Item(k+1).PointColor=GV[k].colors;
		m_TrendGraph.Annotations.Item(k+1).Caption.Color = GV[k].colors;
		m_TrendGraph.Annotations.Item(k+1).PointIndex = maxIndex;

		if (GV[k].isSel!=0)
		{
			m_TrendGraph.Plots.Item(k+1).Visible=TRUE;
			m_TrendGraph.Annotations.Item(k+1).Visible=TRUE;
		}
		else
		{
			m_TrendGraph.Plots.Item(k+1).Visible=FALSE;
			m_TrendGraph.Annotations.Item(k+1).Visible=FALSE;
		}
	}

	this->GetParentFrame()->SetWindowText(title);
	m_TrendGraph.SetCaption(title);
	title=title;
	double maxValueY=m_TrendGraph.Axes.Item("YAxis-1").Maximum, 
		minValueY=m_TrendGraph.Axes.Item("YAxis-1").Minimum, valueDeclY;
	valueDeclY=(maxValueY-minValueY)*0.05;

	m_TrendGraph.Axes.Item("YAxis-1").SetMinimum(minValueY-valueDeclY);
	m_TrendGraph.Axes.Item("YAxis-1").SetMaximum(maxValueY+valueDeclY);
}

void CTrendView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);

	CWnd *pWnd; 

	pWnd = GetDlgItem(IDC_CWGRAPH);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		pWnd->MoveWindow(0,0,cx,cy);//设置控件大小
	}

}
void CTrendView::OnDraw(CDC* )
{
	CWnd* pWnd = GetDlgItem(IDC_CWGRAPH);
	if(pWnd == NULL)
		return;
	CDC* pDCTrend = pWnd->GetDC();
	
	CRect rc;
	pWnd->GetWindowRect(&rc);
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	CPen pen(PS_SOLID,2,RGB(192,192,192));
	CPen *pOld = pDCTrend->SelectObject(&pen);
	//pDCTrend->Rectangle(rc.left,rc.top,rc.Width(),rc.Height());
	pDCTrend->MoveTo(0,0);
	pDCTrend->LineTo(rc.Width(),0);

	pDCTrend->MoveTo(rc.Width(),0);
	pDCTrend->LineTo(rc.Width(),rc.Height());

	pDCTrend->MoveTo(rc.Width(),rc.Height());
	pDCTrend->LineTo(0,rc.Height());

	pDCTrend->MoveTo(0,rc.Height());
	pDCTrend->LineTo(0,0);

	pDCTrend->SelectObject(pOld);

	ReleaseDC(pDCTrend);

}
int CTrendView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if(pMainFrame)
	{		
		pMainFrame->m_TrendView = this;

	}

	return 0;

}

////Marshall  2011.2.23 21:58
extern bool g_bConf97;
extern bool g_bConf95;
extern bool g_bConf100;
extern int  _cdecl  /*CNiWaveFraqView::*/Compare(const void* a,const void* b);
//extern static double CNiWaveFraqView::GetConf(const CNiReal64Vector& niWaveData, double dPercent);


void CTrendView::SetTrendData(std::map<CString,std::vector<float>> multChnData,std::map<CString,std::vector<float>> multChnDataAcd,std::vector<TrendGraph> gV,CString t)
{
	std::map<CString,std::vector<float>> ChnData=multChnData;
	std::map<CString,std::vector<float>> ChnDataAcd=multChnDataAcd;
	std::map<CString,std::vector<float>>::iterator itchA=ChnDataAcd.begin();
	std::map<CString,std::vector<float>>::iterator itch=ChnData.begin();

	//winTitle=title;
	//this->GetParentFrame()->SetWindowText(title);
	//m_graph.SetCaption(title);
	int q=0;	
	//GyData.clear();
	CNiReal64Vector xData((int)ChnData.size());
	CNiReal64Vector mean((int)ChnData.size());
	CNiReal64Vector median((int)ChnData.size());
	CNiReal64Vector RMS((int)ChnData.size());
	CNiReal64Vector ppValue((int)ChnData.size());

	for(;itch!=ChnData.end();itch++)
	{
		CNiReal64Vector chDA(itchA->second.size());

		vector<float>::iterator ita = itchA->second.begin();
		int kc=-1;
		for(; ita != itchA->second.end(); ita++)
		{
			kc++;
			chDA[kc]=*ita;
		}
		double chnMean;
		CNiMath::Mean(chDA,chnMean);
		itchA++;

		CNiReal64Vector NiDt(itch->second.size());
		vector<float>::iterator it2 = itch->second.begin();
		int k=-1;
		for(; it2 != itch->second.end(); it2++)
		{
			k++;
			NiDt[k]=*it2;
		}
		double dRelt;
		CNiMath::Mean(NiDt,dRelt);
		CNiMath::Mean(chDA,chnMean);
		mean[q]=dRelt;
		//GyData.push_back(dRelt);

		CNiMath::Median(NiDt,dRelt);
		median[q]=dRelt;
		//GyData.push_back(dRelt);

		//CNiMath::RMS(NiDt,dRelt);
		//RMS[q]=dRelt;

		CNiReal64Vector m_temp_Data(itch->second.size());
		for (int i=0;i<itch->second.size();i++)
		{
			m_temp_Data[i]=NiDt[i]-mean[q];
		}
		CNiMath::RMS(m_temp_Data,dRelt);   //cxy 20110512

		dRelt=dRelt*2*1.41421356;

		RMS[q]=dRelt;




		//GyData.push_back(dRelt);
		double minV, maxV;
		unsigned int minI, maxI;
		NiDt.MinMax(minV, minI, maxV, maxI);

		/*--------------------------Marshall's Code2011.2.23 21:54--------------*/

		if(g_bConf97)
			//waveParam.ppValue=97;
			dRelt = CNiWaveFraqView::GetConf(NiDt,97);
		else if(g_bConf95)
			//waveParam.ppValue=95;
			dRelt = CNiWaveFraqView::GetConf(NiDt,95);
		else if(g_bConf100)
			//waveParam.ppValue=100;
			dRelt = maxV-minV;
		/*--------------------------Marshall's Code2011.2.23 21:54--------------*/

		//dRelt=maxV-minV;////Marshall  2011.2.23 22:0
		ppValue[q]=dRelt;	
		//GyData.push_back(dRelt);

		/************************************************************************/
		//xData[q]=10*q+chnMean;	//正式版本去掉10*q
		xData[q]=chnMean;	////Marshall  2011.2.23 21:53

		/************************************************************************/	

		q++;

	}
	GxData=xData;
	Gmean=mean;
	Gmedian=median;
	GRMS=RMS;
	GppValue=ppValue;
	GV=gV;
	ichSize=(int)ChnData.size();
	title=t;

}

void CTrendView::SaveAllView(CString Path,int type)
{

	//CRect rcClient = NULL; 
	//HWND hwnd=this->GetSafeHwnd();  //令HWND为本窗口的句柄 
	//::GetWindowRect(hwnd,&rcClient);  
	//int w = rcClient.right - rcClient.left; 
	//int h = rcClient.bottom - rcClient.top; 

	//HDC dc = ::GetDC(hwnd); 

	//HDC gpBmpHdc = CreateCompatibleDC(dc); 
	//HBITMAP hBitmap = CreateCompatibleBitmap(dc, w, h); 
	//HGDIOBJ oldhdc = SelectObject(gpBmpHdc, hBitmap); 

	//::PrintWindow(hwnd,gpBmpHdc,NULL); 

	//CImage image; 
	//image.Attach(hBitmap);

	CImage bmp;	
	RECT controlBounds;

	m_TrendGraph.GetClientRect(&controlBounds);
	int bmpWidth = controlBounds.right - controlBounds.left;
	int bmpHeight = controlBounds.bottom - controlBounds.top;

	// Create the bitmap with the same dimensions of the control.

	bmp.Create(bmpWidth, bmpHeight, 24);

	// Play the enhanced metafile from the CNiPicture to the bitmap.
	RECT imageBounds = { 0, 0, bmpWidth, bmpHeight };
	CNiPicture controlImage = m_TrendGraph.ControlImage();
	HDC bmpDC = bmp.GetDC();
	::PlayEnhMetaFile(bmpDC, reinterpret_cast< HENHMETAFILE >(controlImage.Handle), &imageBounds);


	CString filePath;

	CString strBuff ;
	CString   strFileName;   
	LPTSTR   pszfilename;   
	m_TrendGraph.GetWindowText(strFileName);
	pszfilename=strFileName.GetBuffer(_MAX_PATH);

	CString strSaveFilapath = Path;

	//strSaveFilapath.MakeLower();

	switch(type)
	{
	case 1:
		strSaveFilapath = strSaveFilapath + ".bmp";	
		strSaveFilapath.MakeLower();
		bmp.Save(strSaveFilapath,Gdiplus::ImageFormatBMP);
		break;
	case 2:
		strSaveFilapath = strSaveFilapath + ".jpg";
		strSaveFilapath.MakeLower();
		bmp.Save(strSaveFilapath,Gdiplus::ImageFormatJPEG);
		break;
	case 3:
		strSaveFilapath = strSaveFilapath + ".gif";
		strSaveFilapath.MakeLower();
		bmp.Save(strSaveFilapath,Gdiplus::ImageFormatGIF);
		break;
	case 4:
		strSaveFilapath = strSaveFilapath + ".png";
		strSaveFilapath.MakeLower();
		bmp.Save(strSaveFilapath,Gdiplus::ImageFormatPNG);
		break;
	default:
		strSaveFilapath = strSaveFilapath + ".tiff";
		strSaveFilapath.MakeLower();
		bmp.Save(strSaveFilapath,Gdiplus::ImageFormatTIFF);
		break;
	}		

	bmp.ReleaseDC();
	DeleteObject(bmp); 
}
HBRUSH CTrendView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (HSJSetWhiteControlColor(pDC, pWnd, nCtlColor)) return HSJWhiteBrush();
    return CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CTrendView::OnTrendimagesave()
{
	SaveAllView();
}
//取消存储当前趋势图
void CTrendView::SaveAllView()
{
	m_TrendGraph.SetTrackMode(CNiGraph::TrackDragCursor);
	CRect rcClient = NULL; 
	HWND hwnd=this->GetSafeHwnd();  //令HWND为本窗口的句柄 
	::GetWindowRect(hwnd,&rcClient);  
	int w = rcClient.right - rcClient.left; 
	int h = rcClient.bottom - rcClient.top; 

	HDC dc = ::GetDC(hwnd); 

	HDC gpBmpHdc = CreateCompatibleDC(dc); 
	HBITMAP hBitmap = CreateCompatibleBitmap(dc, w, h); 

	HGDIOBJ oldhdc = SelectObject(gpBmpHdc, hBitmap); 

	::PrintWindow(hwnd,gpBmpHdc,NULL); 

	CImage image; 
	image.Attach(hBitmap); 


	CString filePath;
	CString strFilter;
	strFilter = _T("BMP文件(*.BMP)|*.BMP|JPEG文件(*.JPG)|*.JPG|GIF文件(*.GIF)|*GIF|PNG文件(*.PNG)|*.PNG|TIFF文件(*.TIFF)|*.TIFF||");

	CFileDialog FileOpenDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT, strFilter);

	CString strBuff ;
	CString   strFileName;   
	LPTSTR   pszfilename;   

	m_TrendGraph.GetWindowText(strFileName);
	pszfilename=strFileName.GetBuffer(_MAX_PATH);
	FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  

	if(FileOpenDlg.DoModal() == IDOK)
	{

		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：存储当前趋势图...");

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
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：保存完成，存储为  "+strSaveFilapath);

	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：取消存储当前趋势图");

	image.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 

	m_TrendGraph.SetTrackMode(CNiGraph::TrackDragAnnotation);
}
void CTrendView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;

	VERIFY(menu.LoadMenu (IDR_TrendMenu));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x, point.y, AfxGetMainWnd());

	menu.Detach();
}
//消息：是否显示平均值
void CTrendView::OnTrendmean()
{
	if (showMean==TRUE)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：显示平均值");
		m_TrendGraph.Plots.Item(1).Visible=TRUE;
		m_TrendGraph.Annotations.Item(1).Visible=TRUE;
		showMean=FALSE;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：不显示平均值");
		m_TrendGraph.Plots.Item(1).Visible=FALSE;
		m_TrendGraph.Annotations.Item(1).Visible=FALSE;
		showMean=TRUE;
	}

	Invalidate();
}
//消息：是否显示平均值开关
void CTrendView::OnUpdateTrendmean(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!showMean);
}

//消息：是否显示中间值
void CTrendView::OnTrendmedian()
{
	if (showMedian==TRUE)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：显示中间值");

		m_TrendGraph.Plots.Item(2).Visible=TRUE;
		m_TrendGraph.Annotations.Item(2).Visible=TRUE;
		showMedian=FALSE;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：不显示中间值");

		m_TrendGraph.Plots.Item(2).Visible=FALSE;
		m_TrendGraph.Annotations.Item(2).Visible=FALSE;
		showMedian=TRUE;
	}

	Invalidate();
}
//消息：是否显示中间值开关
void CTrendView::OnUpdateTrendmedian(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!showMedian);
}

//消息：是否显示有效值
void CTrendView::OnTrendrms()
{
	if (showRMS==TRUE)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：显示有效值");

		m_TrendGraph.Plots.Item(3).Visible=TRUE;
		m_TrendGraph.Annotations.Item(3).Visible=TRUE;
		showRMS=FALSE;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：不显示有效值");

		m_TrendGraph.Plots.Item(3).Visible=FALSE;
		m_TrendGraph.Annotations.Item(3).Visible=FALSE;
		showRMS=TRUE;
	}

	Invalidate();
}
//消息：是否显示有效值开关
void CTrendView::OnUpdateTrendrms(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!showRMS);
}
//消息：是否显示峰峰值
void CTrendView::OnTrendppvalue()
{
	if (showPPvalue==TRUE)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：显示峰峰值");

		m_TrendGraph.Plots.Item(4).Visible=TRUE;
		m_TrendGraph.Annotations.Item(4).Visible=TRUE;
		showPPvalue=FALSE;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：不显示峰峰值");

		m_TrendGraph.Plots.Item(4).Visible=FALSE;
		m_TrendGraph.Annotations.Item(4).Visible=FALSE;
		showPPvalue=TRUE;
	}

	Invalidate();
}
//消息：是否显示峰峰值开关
void CTrendView::OnUpdateTrendppvalue(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!showPPvalue);
}
//消息：设置图形特征
void CTrendView::OnSettrendparam()
{
	CTrendGraphSet dgs(this);

	dgs.SetTrendParam(title,
		              (int)m_TrendGraph.Axes.Item("XAxis").GetMinimum(),
					  (int)m_TrendGraph.Axes.Item("XAxis").GetMaximum(),
					  (int)m_TrendGraph.Axes.Item("YAxis-1").GetMinimum(),
					  (int)m_TrendGraph.Axes.Item("YAxis-1").GetMaximum(),
					  m_TrendGraph.Plots.Item(1).GetLineColor(),
					  m_TrendGraph.Plots.Item(2).GetLineColor(),
					  m_TrendGraph.Plots.Item(3).GetLineColor(),
					  m_TrendGraph.Plots.Item(4).GetLineColor()
					  );
	if(dgs.DoModal())
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：设置图形特征");
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：取消设置图形特征");

	Invalidate();
}

//设置趋势曲线特征
//@CString t	标题
//@int xmin		x最小值
//@int xmax		x最大值
//@int ymin		y最小值
//@int ymax		y最大值
//@COLORREF c1	平均值曲线颜色
//@COLORREF c2	中间值曲线颜色
//@COLORREF c3	有效值曲线颜色
//@COLORREF c4	峰峰值曲线颜色
void CTrendView::SetTrendParam(CString t,int xmin,int xmax,int ymin,int ymax,COLORREF c1,COLORREF c2,COLORREF c3,COLORREF c4)
{
	this->GetParentFrame()->SetWindowText(t);
	m_TrendGraph.SetCaption(t);

	m_TrendGraph.Axes.Item("XAxis").SetMinimum(xmin);
	m_TrendGraph.Axes.Item("XAxis").SetMaximum(xmax);
	m_TrendGraph.Axes.Item("YAxis-1").SetMinimum(ymin);
	m_TrendGraph.Axes.Item("YAxis-1").SetMaximum(ymax);

	m_TrendGraph.Plots.Item(1).LineColor=c1;
	m_TrendGraph.Plots.Item(1).PointColor=c1;
	m_TrendGraph.Annotations.Item(1).Caption.Color = c1;

	m_TrendGraph.Plots.Item(2).LineColor=c2;
	m_TrendGraph.Plots.Item(2).PointColor=c2;
	m_TrendGraph.Annotations.Item(2).Caption.Color = c2;

	m_TrendGraph.Plots.Item(3).LineColor=c3;
	m_TrendGraph.Plots.Item(3).PointColor=c3;
	m_TrendGraph.Annotations.Item(3).Caption.Color = c3;

	m_TrendGraph.Plots.Item(4).LineColor=c4;
	m_TrendGraph.Plots.Item(4).PointColor=c4;
	m_TrendGraph.Annotations.Item(4).Caption.Color = c4;
}

//消息：存储趋势数据报表
void CTrendView::OnTrendreport()
{
	CSaveTrendToExcelDlg stted;
	if(stted.DoModal()==IDCANCEL)
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：取消存储趋势数据报表");
		
}
//消息：批量存储趋势图
void CTrendView::OnTrendimagesavebat()
{
	CTrendSelSet tss;
	tss.SetShowType(1);

	if(tss.DoModal()==IDCANCEL)
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：取消批量存储趋势图");


}

void CTrendView::OnSetFocus(CWnd* pOldWnd)
{
	CBCGPFormView::OnSetFocus(pOldWnd);
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	pCMF->ActivateTabEG(3);
	// TODO: 在此处添加消息处理程序代码
}
