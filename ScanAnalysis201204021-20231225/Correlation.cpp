#include "stdafx.h"
#include "ScanAnalysis.h"
#include "Correlation.h"
#include "MainFrm.h"
#include "WaveFilter.h"
using namespace std;
// CCorrelation

IMPLEMENT_DYNCREATE(CCorrelation, CFormView)

CCorrelation::CCorrelation()
: CFormView(CCorrelation::IDD),XTitle(""),YTitle(""),WinTitle(""),isWaveFilter(FALSE),samplingFreq(0),m_deltaFreq(1.0),m_deltaFreqX(1.0),m_deltaFreqY(1.0)
{
	FunTitle="相关性分析";
}

CCorrelation::~CCorrelation()
{
}

void CCorrelation::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPHX, m_GraphX);
	DDX_Control(pDX, IDC_CWGRAPHY, m_GraphY);
	DDX_Control(pDX, IDC_CWGRAPHAXIS, m_GraphAxis);
}

BEGIN_MESSAGE_MAP(CCorrelation, CFormView)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()

	ON_WM_CREATE()
	//ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_CoherentResetY, &CCorrelation::OnCoherentresetY)
	ON_COMMAND(ID_CoherentUnZoomY, &CCorrelation::OnCoherentunzoomY)
	ON_COMMAND(ID_CoherentResetX, &CCorrelation::OnCoherentresetX)
	ON_COMMAND(ID_CoherentSaveToImage, &CCorrelation::OnCoherentsavetoimage)
	ON_COMMAND(ID_CoherentUnZoomX, &CCorrelation::OnCoherentunzoomX)
	//	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()

	ON_COMMAND(ID_CORR_TITLE, &CCorrelation::OnCorrTitle)
END_MESSAGE_MAP()


// CCorrelation 诊断

#ifdef _DEBUG
void CCorrelation::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCorrelation::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

//初始化
void CCorrelation::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	HSJStyleChildControls(GetSafeHwnd(), m_skinFonts);
	HSJStyleGraph(m_GraphX);
	HSJStyleGraph(m_GraphY);
	HSJStyleGraph(m_GraphAxis);


	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/
	//调整界面，不出现滚动条////Marshall  2011.3.2 12:23
	SetScrollSizes(MM_TEXT,CSize(0,0));
	//InitNiGraph();
	ResizeParentToFit();
	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/

	//在此处设置标题//Marshall  2011.3.3 18:15
	CWnd* pWndW=GetDlgItem(IDC_TitleXY);
	if(pWndW == NULL)
		return;
	pWndW->SetWindowText(WinTitle);

	m_GraphX.SetWindowText(XTitle);
	m_GraphX.GetPlots().Item(1).PlotY(dataX,0,1);
	m_GraphX.Axes.Item("XAxis").SetMinMax(0,dataX.GetSize());


	m_GraphY.SetWindowText(YTitle);
	m_GraphY.GetPlots().Item(1).PlotY(dataY,0,1);
	m_GraphY.Axes.Item("XAxis").SetMinMax(0,dataY.GetSize());


		CNiReal64Vector correlation;
		//m_GraphAxis.SetWindowText("二维轴心轨迹");	
		//m_GraphAxis.GetPlots().Item(1).PlotXvsY (dataX,dataY);
		m_GraphAxis.SetWindowText("相关分析结果");	
		CNiMath::Correlate (dataX, dataY, correlation);
		m_GraphAxis.ClearData();
		m_GraphAxis.PlotY (correlation);


	SetQuaternionX(&m_GraphX,&m_GraphY);
	SetQuaternionY(&m_GraphX,&m_GraphY);
}
//消息：改变窗体大小
void CCorrelation::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

		CWnd *pWnd; 
		pWnd = GetDlgItem(IDC_CWGRAPHX);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			//pWnd->MoveWindow(0,cy/20,cx,cy/3-cy/60);//设置控件大小
			pWnd->MoveWindow(0,0,cx,cy/3);//设置控件大小//cxy20231224
		}	

		pWnd = GetDlgItem(IDC_CWGRAPHY);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			//pWnd->MoveWindow(0,cy/20+cy/3-cy/60,cx,cy/3-cy/60);//设置控件大小
			pWnd->MoveWindow(0,cy/3,cx,cy/3);//设置控件大小//cxy20231224
		}
		pWnd = GetDlgItem(IDC_CWGRAPHAXIS);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			//pWnd->MoveWindow(0,cy/20+2*cy/3-cy/30,cx,cy/3-cy/60);//设置控件大小
			pWnd->MoveWindow(0,2*cy/3,cx,cy/3);//设置控件大小//cxy20231224
		}
		pWnd = GetDlgItem(IDC_TitleXY);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			//pWnd->MoveWindow(0,0,cx,cy/20);//设置控件大小
			pWnd->MoveWindow(0,0,cx,0);//设置控件大小//cxy20231224
		}

}

//消息：设置背景色
HBRUSH CCorrelation::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (HSJSetWhiteControlColor(pDC, pWnd, nCtlColor)) return HSJWhiteBrush();
    return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}
BEGIN_EVENTSINK_MAP(CCorrelation, CFormView)
	ON_EVENT(CCorrelation, IDC_CWGRAPHX, 15, CCorrelation::ZoomCwgraphx, VTS_NONE)
	ON_EVENT(CCorrelation, IDC_CWGRAPHY, 15, CCorrelation::ZoomCwgraphy, VTS_NONE)
END_EVENTSINK_MAP()
//放大X波形图
void CCorrelation::ZoomCwgraphx()
{
		if (GetXRang(&m_GraphX)>20)
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：X参照通道图形放大");
			SetQuaternionX(&m_GraphX,&m_GraphY);
		}
		else
		{
			QuaternionX qx=qVectorCX.back();
			m_GraphX.Axes.Item("XAxis").SetMinimum(qx.xMin);
			m_GraphX.Axes.Item("XAxis").SetMaximum(qx.xMax);
			//m_GraphX.Axes.Item("YAxis-1").SetMinimum(q.yMin);
			//m_GraphX.Axes.Item("YAxis-1").SetMaximum(q.yMax);
		}
		Invalidate();
}
//放大Y波形图
void CCorrelation::ZoomCwgraphy()
{
	if (GetXRang(&m_GraphY)>20)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：Y参照通道图形放大");
		SetQuaternionY(&m_GraphX,&m_GraphY);
	}
	else
	{
		QuaternionX qx=qVectorCY.back();
		m_GraphY.Axes.Item("XAxis").SetMinimum(qx.xMin);
		m_GraphY.Axes.Item("XAxis").SetMaximum(qx.xMax);
	}
	//QuaternionX q=qVector.back();
	//m_GraphY.Axes.Item("XAxis").SetMinimum(q.xMin);
	//m_GraphY.Axes.Item("XAxis").SetMaximum(q.xMax);	
	Invalidate();

}


//获取波形图范围
//@NI::CNiGraph* mG	通道数据
double CCorrelation::GetXRang(NI::CNiGraph* mG)
{
	return mG->Axes.Item("XAxis").GetMaximum()-mG->Axes.Item("XAxis").GetMinimum();
}

//设置通道数据
//@NI::CNiGraph* mG1		通道数据
void CCorrelation::SetAxisData(NI::CNiGraph* mG1,NI::CNiGraph* mG2)
{
	int xMax=(int)(mG1->Axes.Item("XAxis").GetMaximum());
	int xMin=(int)(mG1->Axes.Item("XAxis").GetMinimum());
	int xRound=xMax-xMin;
	CNiReal64Vector newData1(xRound);
	if (xRound>0)
	{
		for (int i=0;i<xRound;i++)
		{
			newData1[i]=dataX[xMin];
			xMin++;
		}
	}
	int yMax=(int)(mG2->Axes.Item("XAxis").GetMaximum());
	int yMin=(int)(mG2->Axes.Item("XAxis").GetMinimum());
	int yRound=yMax-yMin;
	
	CNiReal64Vector newData2(yRound);
	if (yRound>0)
	{
		for (int i=0;i<yRound;i++)
		{
			newData2[i]=dataY[yMin];
			yMin++;
		}
	}
	CNiReal64Vector correlation;
	CNiMath::Correlate (newData1, newData2, correlation);
	m_GraphAxis.ClearData();
	m_GraphAxis.PlotY (correlation);

	//m_GraphAxis.GetPlots().Item(1).ClearData();

	//	m_GraphAxis.GetPlots().Item(1).PlotXvsY(newData1,newData2);
}
//消息：撤销图形放大X
void CCorrelation::OnCoherentunzoomX()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：撤销X通道图形放大");
	GetQuaternionX(&m_GraphX,&m_GraphY);
	SetAxisData(&m_GraphX,&m_GraphY);

	Invalidate();////Marshall  2011.3.3 18:54
}
//消息：撤销图形放大Y
void CCorrelation::OnCoherentunzoomY()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：撤销Y通道图形放大");
	GetQuaternionY(&m_GraphX,&m_GraphY);
	SetAxisData(&m_GraphY,&m_GraphY);
	Invalidate();////Marshall  2011.3.3 18:54
}
//存储图形范围
//@NI::CNiGraph* mG1		X通道数据
//@NI::CNiGraph* mG2		Y通道数据
void CCorrelation::SetQuaternionX(NI::CNiGraph* mG1, NI::CNiGraph* mG2)
{
	QuaternionX q;	

	q.xMin=mG1->Axes.Item("XAxis").GetMinimum();
	q.xMax=mG1->Axes.Item("XAxis").GetMaximum();
	//q.yMin=mG1->Axes.Item("YAxis-1").GetMinimum();
	//q.yMax=mG1->Axes.Item("YAxis-1").GetMaximum();

	qVectorCX.push_back(q);

	//mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
	//mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
	//mG2->Axes.Item("YAxis-1").SetMinimum(q.yMin);
	//mG2->Axes.Item("YAxis-1").SetMaximum(q.yMax);

	SetAxisData(mG1,mG2);

}
//存储图形范围
//@NI::CNiGraph* mG1		X通道数据
//@NI::CNiGraph* mG2		Y通道数据
void CCorrelation::SetQuaternionY(NI::CNiGraph* mG1, NI::CNiGraph* mG2)
{
	QuaternionX q;	

	q.xMin=mG2->Axes.Item("XAxis").GetMinimum();
	q.xMax=mG2->Axes.Item("XAxis").GetMaximum();
	//q.yMin=mG1->Axes.Item("YAxis-1").GetMinimum();
	//q.yMax=mG1->Axes.Item("YAxis-1").GetMaximum();

	qVectorCY.push_back(q);

	//mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
	//mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
	//mG2->Axes.Item("YAxis-1").SetMinimum(q.yMin);
	//mG2->Axes.Item("YAxis-1").SetMaximum(q.yMax);

	SetAxisData(mG1,mG2);

}
//获取图形范围X
//@NI::CNiGraph* mG1		X通道数据

void CCorrelation::GetQuaternionX(NI::CNiGraph* mG1, NI::CNiGraph* mG2)
{
	if (qVectorCX.size()>1)
	{
		qVectorCX.pop_back();
		QuaternionX q=qVectorCX.back();
		mG1->Axes.Item("XAxis").SetMinimum(q.xMin);
		mG1->Axes.Item("XAxis").SetMaximum(q.xMax);	
		//mG1->Axes.Item("YAxis-1").SetMinimum(q.yMin);
		//mG1->Axes.Item("YAxis-1").SetMaximum(q.yMax);

		//mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
		//mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
		//mG2->Axes.Item("YAxis-1").SetMinimum(q.yMin);
		//mG2->Axes.Item("YAxis-1").SetMaximum(q.yMax);
	}
	else
	{
		OnCoherentresetX();
	}

}
//获取图形范围Y
//@NI::CNiGraph* mG2		Y通道数据

void CCorrelation::GetQuaternionY(NI::CNiGraph* mG1, NI::CNiGraph* mG2)
{
	if (qVectorCY.size()>1)
	{
		qVectorCY.pop_back();
		QuaternionX q=qVectorCY.back();
		mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
		mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
		//mG1->Axes.Item("YAxis-1").SetMinimum(q.yMin);
		//mG1->Axes.Item("YAxis-1").SetMaximum(q.yMax);

		//mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
		//mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
		//mG2->Axes.Item("YAxis-1").SetMinimum(q.yMin);
		//mG2->Axes.Item("YAxis-1").SetMaximum(q.yMax);
	}
	else
	{
		OnCoherentresetY();
	}

}
//消息：重新加载数据绘制图形
void CCorrelation::OnCoherentresetX()
{
	if (!isWaveFilter)
	{

		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：重绘X通道图形");

		double minValue, maxValue, valueDecl;
		unsigned int minIndex, maxIndex;
		dataX.MinMax(minValue, minIndex, maxValue, maxIndex);
		valueDecl=(maxValue-minValue)*0.1;
		qVectorCX.clear();

		//m_GraphX.GetPlots().RemoveAll();
		//m_GraphX.GetPlots().Add();
		m_GraphX.GetPlots().Item(1).PlotY(dataX,0,1);
		m_GraphY.GetPlots().Item(1).PlotY(dataY,0,1);
		m_GraphX.Axes.Item("XAxis").SetMinMax(0,dataX.GetSize());
		SetQuaternionX(&m_GraphX,&m_GraphY);

	}

	Invalidate();
}
//消息：重新加载数据绘制图形
void CCorrelation::OnCoherentresetY()
{
	if (!isWaveFilter)
	{

		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：重绘Y通道图形");

		double minValue, maxValue, valueDecl;
		unsigned int minIndex, maxIndex;
		dataX.MinMax(minValue, minIndex, maxValue, maxIndex);
		valueDecl=(maxValue-minValue)*0.1;
		qVectorCY.clear();

		//m_GraphX.GetPlots().RemoveAll();
		//m_GraphX.GetPlots().Add();
		m_GraphY.GetPlots().Item(1).PlotY(dataX,0,1);
		m_GraphY.GetPlots().Item(1).PlotY(dataY,0,1);
		m_GraphY.Axes.Item("XAxis").SetMinMax(0,dataY.GetSize());
		SetQuaternionY(&m_GraphX,&m_GraphY);

	}

	Invalidate();////Marshall  2011.3.3 18:55
}
//消息：保存视图到图像
void CCorrelation::OnCoherentsavetoimage()
{
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
/*
	CString strBuff ;
	CString   strFileName;   
	LPTSTR   pszfilename;  
	strFileName=WinTitle;


	int nRight = strFileName.Find(_T('//'));
	strFileName.Delete(nRight,1);

	//m_Graph.GetWindowText(strFileName);
	pszfilename=strFileName.GetBuffer(_MAX_PATH);
	FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  
*/

	CString strBuff ;
	CString   strFileName;   
	LPTSTR   pszfilename;  
	strFileName=WinTitle;
	/*m_Graph.GetWindowText(strFileName);*/
	pszfilename=strFileName.GetBuffer(_MAX_PATH);
	FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存"; 



	if(FileOpenDlg.DoModal() == IDOK)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱存储：保存视图到图像...");

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
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱存储：保存完成，存储为  "+strSaveFilapath);

	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱存储：取消保存视图到图像");

	}
	image.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
}

//消息：重绘，显示特征值
void CCorrelation::OnDraw(CDC* /*pDC*/)
{
//	CWnd* pWndW=GetDlgItem(IDC_TitleXY);
	//CDC* pDCF=pWndW->GetDC();//标题改为static控件，不在此处设置标题//Marshall  2011.3.3 18:14
	//CRect rc;
	//pWndW->GetWindowRect(&rc);
	//pWndW->Invalidate();
	//pWndW->UpdateWindow();

	//CFont   font;   
	//CFont   *pOldFont=NULL;   
	//font.CreatePointFont(150,"宋体");   
	//pOldFont=(CFont   *)pDCF->SelectObject(&font);

	//pDCF->SetTextColor(RGB(0,0,255)); 
	//pDCF->TextOut(200,2,WinTitle);   
	//pDCF->SelectObject(pOldFont);

	////this->SetWindowText(WinTitle);
	//ReleaseDC(pDCF);


	/*--------------------------Marshall's Code2011.3.3 18:18--------------*/
	//边框绘制
	CWnd* pWnd = GetDlgItem(IDC_CWGRAPHX);
	if(pWnd == NULL)
		return;
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	CDC* pDC = pWnd->GetDC();
	CPen pen(PS_SOLID,2,RGB(192,192,192));
	CPen* pOldPen = pDC->SelectObject(&pen);

	CRect rc;
	pWnd->GetWindowRect(&rc);

	//pDC->MoveTo(0,0);
	//pDC->LineTo(rc.Width(),0);

	pDC->MoveTo(rc.Width(),0);
	pDC->LineTo(rc.Width(),rc.Height());

	pDC->SelectObject(pOldPen);
	ReleaseDC(pDC);
	///////////////////////////////
	CWnd* pWndTitle = GetDlgItem(IDC_TitleXY);
	if(pWndTitle == NULL)
		return;
	pWndTitle->Invalidate();
	pWndTitle->UpdateWindow();

	CDC* pDCTitle = pWndTitle->GetDC();
	//CPen pen(PS_SOLID,2,RGB(192,192,192));
	 pOldPen = pDCTitle->SelectObject(&pen);

	//CRect rc;
	pWndTitle->GetWindowRect(&rc);
	
	pDCTitle->MoveTo(rc.Width(),0);
	pDCTitle->LineTo(rc.Width(),rc.Height());

	pDCTitle->SelectObject(pOldPen);
	ReleaseDC(pDCTitle);
	///////////////////////////////
	CWnd* pWndY = GetDlgItem(IDC_CWGRAPHY);
	if(pWndY == NULL)
		return;
	pWndY->Invalidate();
	pWndY->UpdateWindow();

	CDC* pDCY = pWndY->GetDC();
	//CPen pen(PS_SOLID,2,RGB(192,192,192));
	 pOldPen = pDCY->SelectObject(&pen);

	//CRect rc;
	pWndY->GetWindowRect(&rc);
	
	pDCY->MoveTo(rc.Width(),0);
	pDCY->LineTo(rc.Width(),rc.Height());

	pDCY->SelectObject(pOldPen);
	ReleaseDC(pDCY);
	////////////////////////////////
	CWnd* pWndXY = GetDlgItem(IDC_CWGRAPHAXIS);
	if(pWndXY == NULL)
		return;
	pWndXY->Invalidate();
	pWndXY->UpdateWindow();

	CDC* pDCXY = pWndXY->GetDC();
	//CPen pen(PS_SOLID,2,RGB(192,192,192));
	 pOldPen = pDCXY->SelectObject(&pen);

	//CRect rc;
	pWndXY->GetWindowRect(&rc);
	
	pDCXY->MoveTo(rc.Width(),0);
	pDCXY->LineTo(rc.Width(),rc.Height());

	pDCXY->SelectObject(pOldPen);
	ReleaseDC(pDCXY);
	/*--------------------------Marshall's Code2011.3.3 18:18--------------*/

}

//设置通道Y参照坐标数据
//@vector <float> vData		Y通道数据
//@CString title			Y标题
void CCorrelation::SetDataY(vector <float> vData,int sampling,CString title)
{
	/*xRange=(unsigned int)vData.size()+1;*/
	samplingFreq=sampling;

	CNiReal64Vector NiDt((unsigned int)vData.size());
	vector<float>::iterator it2 = vData.begin();
	int k=-1;
	for(; it2 != vData.end(); it2++)
	{
		k++;
		NiDt[k]=*it2;
	}

	dataY=NiDt;
	ResumeDataY=NiDt;
	YTitle=title;
}
//设置通道X参照坐标数据
//@vector <float> vData		X通道数据
//@CString title			X标题
void CCorrelation::SetDataX(vector <float> vData,int sampling,CString title)
{
	/*xRange=(unsigned int)vData.size()+1;*/
	samplingFreq=sampling;

	CNiReal64Vector NiDt((unsigned int)vData.size());
	//CNiReal64Vector ResumeData((unsigned int)vData.size());

	vector<float>::iterator it2 = vData.begin();
	int k=-1;
	for(; it2 != vData.end(); it2++)
	{
		k++;
		NiDt[k]=*it2;
	}

	dataX=NiDt;
	ResumeDataX=NiDt;
	XTitle=title;
}
//设置标题
//@CString title			窗体标题
void CCorrelation::SetWinTitle(CString title)
{
	WinTitle=title;
}
//消息：创建窗体
int CCorrelation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if(pMainFrame)
	{
		//pMainFrame->m_pExamView = this;
		pMainFrame->corr = this;
	}

	return 0;
}

//根据类型设置波形图显示样式
//@int sel		窗体样式
void CCorrelation::SetFunSel()
{
	CRect   rect   ;   
	this->GetWindowRect(   rect   );   
	ScreenToClient(   rect   )   ;   
	this->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());  
}
//消息：恢复放大
//void CCorrelation::OnCoherentreset()
//{
//	OnResumexy();
//}
//消息：保存图片
//void CCorrelation::OnCoherentsavetoimage()
//{
//	OnSaveallviewxy();
//}
//消息：撤销放大
//void CCorrelation::OnCoherentunzoom()
//{
//	OnAxisunzoom();
//}

//消息：设置焦点，激活工具栏
void CCorrelation::OnSetFocus(CWnd* pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	pCMF->ActivateTabEG(4);
		//pCMF->ActivateTabEG(0);


	// TODO: 在此处添加消息处理程序代码
}


void CCorrelation::OnCorrTitle()
{
	CSetTitle setTitleDlg;
	setTitleDlg.mTitle = WinTitle;

	if (setTitleDlg.DoModal()==IDOK)
	{

		CWnd* pWndW=GetDlgItem(IDC_TitleXY);
		if(pWndW == NULL)
			return;


		pWndW->SetWindowText(setTitleDlg.mTitle);

	}

}
