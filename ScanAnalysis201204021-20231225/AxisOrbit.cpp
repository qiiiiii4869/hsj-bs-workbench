// AxisOrbit.cpp : 实现文件
//

#include "stdafx.h"
#include "HSJLayoutBridge.h"
#include "ScanAnalysis.h"
#include "AxisOrbit.h"
#include "MainFrm.h"
#include "WaveFilter.h"
using namespace std;
// CAxisOrbit

IMPLEMENT_DYNCREATE(CAxisOrbit, CFormView /*CBCGPFormView*/)



CAxisOrbit::CAxisOrbit()
	: CFormView /*CBCGPFormView*/(CAxisOrbit::IDD),XTitle(""),YTitle(""),WinTitle(""),/*FunSel(1),isWaveFilter(FALSE),*/samplingFreq(0),m_deltaFreq(1.0)
	/*, m_strTitle(_T(""))*/
{
	FunTitle="二维轴心轨迹";
}

CAxisOrbit::~CAxisOrbit()
{
}

void CAxisOrbit::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPHX, m_GraphX);
	DDX_Control(pDX, IDC_CWGRAPHY, m_GraphY);
	DDX_Control(pDX, IDC_CWGRAPHAXIS, m_GraphAxis);
}

BEGIN_MESSAGE_MAP(CAxisOrbit, CFormView /*CBCGPFormView*/)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_AxisUnZoom, &CAxisOrbit::OnAxisunzoom)
	ON_COMMAND(ID_AxisResume, &CAxisOrbit::OnResumexy)
	ON_COMMAND(ID_AxisSaveAllView, &CAxisOrbit::OnSaveallviewxy)
	ON_WM_CREATE()
	//ON_WM_LBUTTONDBLCLK()
	//ON_COMMAND(ID_CoherentReset, &CAxisOrbit::OnCoherentreset)
	//ON_COMMAND(ID_CoherentSaveToImage, &CAxisOrbit::OnCoherentsavetoimage)
	//ON_COMMAND(ID_CoherentUnZoom, &CAxisOrbit::OnCoherentunzoom)
//	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_YWaveFilter, &CAxisOrbit::OnYwavefilter)
	ON_COMMAND(ID_XWaveFilter, &CAxisOrbit::OnXwavefilter)
	ON_COMMAND(ID_ResumeFilterXY, &CAxisOrbit::OnResumefilterxy)
	ON_COMMAND(ID_ORBIT_TITLE, &CAxisOrbit::OnOrbitTitle)
	ON_COMMAND(ID_Y_ZOOM, &CAxisOrbit::OnYZoom)
	ON_COMMAND(ID_X_ZOOM, &CAxisOrbit::OnXZoom)
END_MESSAGE_MAP()


// CAxisOrbit 诊断

#ifdef _DEBUG
void CAxisOrbit::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAxisOrbit::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

//初始化
void CAxisOrbit::OnInitialUpdate()
{
	
	
CFormView::OnInitialUpdate();
	HSJStyleChildControls(GetSafeHwnd(), m_skinFonts);
	HSJStyleGraph(m_GraphX);
	HSJStyleGraph(m_GraphY);
	HSJStyleGraph(m_GraphAxis);


	SetScrollSizes(MM_TEXT,CSize(0,0));
	ResizeParentToFit();
	m_GraphX.SetWindowText(XTitle);
	m_GraphY.SetWindowText(YTitle);
	m_GraphAxis.SetWindowText("二维轴心轨迹");	

	int round;
	round=dataX.GetSize();
	CNiReal64Vector temp_data(round);
	if (round>0)
	{
		for (int i=0;i<round;i++)
		{
			temp_data[i]=double(i)*m_deltaFreq;

		}
	}


	m_GraphX.GetPlots().Item(1).ClearData();
	m_GraphX.GetPlots().Item(1).PlotXvsY(temp_data,dataX);
	m_GraphX.Axes.Item("XAxis").SetMinMax(0,dataX.GetSize()*m_deltaFreq);


	m_GraphY.GetPlots().Item(1).ClearData();
	m_GraphY.GetPlots().Item(1).PlotXvsY(temp_data,dataY);
	m_GraphX.Axes.Item("XAxis").SetMinMax(0,dataY.GetSize()*m_deltaFreq);

    m_GraphAxis.GetPlots().Item(1).ClearData();
	m_GraphAxis.GetPlots().Item(1).PlotXvsY (dataX,dataY);

 	SetQuaternion(&m_GraphX,&m_GraphY);
	CWnd* pWndW=GetDlgItem(IDC_TitleXY);
	if(pWndW == NULL)
		return;
	 pWndW->SetWindowText(WinTitle);
	


	HSJApplyViewLayout(this, 1, 1, 1);
}
//消息：改变窗体大小
void CAxisOrbit::OnSize(UINT nType, int cx, int cy)
{
	// Responsive layout and a scrollable minimum canvas.
	if (HSJApplyViewLayout(this, 1, cx, cy)) return;
	CFormView::OnSize(nType, cx, cy);

	//if (FunSel==1)
	{
		CWnd *pWnd; 
		//pWnd = GetDlgItem(IDC_CWGRAPHX);     //获取控件句柄
		pWnd = GetDlgItem(IDC_CWGRAPHX);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(0,cy/8,cx-3*cy/4,cy/2-cy/8);//设置控件大小
		}	

		//pWnd = GetDlgItem(IDC_CWGRAPHY);     //获取控件句柄
		pWnd = GetDlgItem(IDC_CWGRAPHY);     //获取控件句柄  //cxy20231224
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(0,cy/2,cx-3*cy/4,cy/2-cy/8);//设置控件大小
		}
		pWnd = GetDlgItem(IDC_CWGRAPHAXIS);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			//pWnd->MoveWindow(cx-3*cy/4,cy/8,3*cy/4,3*cy/4 );//设置控件大小
			pWnd->MoveWindow(cx-3*cy/4,cy/8,3*cy/4,3*cy/4 + 1);///调整界面，使轨迹图与Y图下端刚好对齐///Marshall  2011.3.2 21:20
		}
		pWnd = GetDlgItem(IDC_TitleXY);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			//pWnd->MoveWindow(0,0,cx,cy/10 );//设置控件大小
			pWnd->MoveWindow(0,0,cx,cy/8 - 1 );//设置控件大小
			//pWnd->MoveWindow(0,0,cx,cy/10 - 2);//调整界面//Marshall  2011.3.2 15:21
		}
	} 
	//if(FunSel==2)
	//{
	//	CWnd *pWnd; 
	//	pWnd = GetDlgItem(IDC_CWGRAPHX);     //获取控件句柄
	//	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	//	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	//	{
	//		pWnd->MoveWindow(0,cy/20,cx,cy/3-cy/60);//设置控件大小
	//	}	

	//	pWnd = GetDlgItem(IDC_CWGRAPHY);     //获取控件句柄
	//	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	//	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	//	{
	//		pWnd->MoveWindow(0,cy/20+cy/3-cy/60,cx,cy/3-cy/60);//设置控件大小
	//	}
	//	pWnd = GetDlgItem(IDC_CWGRAPHAXIS);     //获取控件句柄
	//	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	//	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	//	{
	//		pWnd->MoveWindow(0,cy/20+2*cy/3-cy/30,cx,cy/3-cy/60);//设置控件大小
	//	}
	//	pWnd = GetDlgItem(IDC_TitleXY);     //获取控件句柄
	//	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	//	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	//	{
	//		pWnd->MoveWindow(0,0,cx,cy/20);//设置控件大小
	//	}
	//}

}

//消息：设置背景色
HBRUSH CAxisOrbit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (HSJSetWhiteControlColor(pDC, pWnd, nCtlColor)) return HSJWhiteBrush();
    return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}
BEGIN_EVENTSINK_MAP(CAxisOrbit, CFormView)
	ON_EVENT(CAxisOrbit, IDC_CWGRAPHX, 15, CAxisOrbit::ZoomCwgraphx, VTS_NONE)
	ON_EVENT(CAxisOrbit, IDC_CWGRAPHY, 15, CAxisOrbit::ZoomCwgraphy, VTS_NONE)
END_EVENTSINK_MAP()
//放大X波形图
void CAxisOrbit::ZoomCwgraphx()
{

	OnXZoom();
	//if (FunSel==1)
	{
		if (GetXRang(&m_GraphX)>20*m_deltaFreq)
		{
			//if (FunSel==1)
				((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱操作：X参照通道图形放大");
			//else
			//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：X参照通道图形放大");
			SetQuaternion(&m_GraphX,&m_GraphY);
		}
		else
		{
			QuaternionX q=qVector.back();
			m_GraphX.Axes.Item("XAxis").SetMinimum(q.xMin);
			m_GraphX.Axes.Item("XAxis").SetMaximum(q.xMax);	
			//m_GraphX.Axes.Item("YAxis-1").SetMinimum(q.yMin);
			//m_GraphX.Axes.Item("YAxis-1").SetMaximum(q.yMax);
		}
	}
	//else
	//{
	//	if (GetXRang(&m_GraphX)>20)
	//	{
	//		if (FunSel==1)
	//			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱操作：X参照通道图形放大");
	//		else
	//			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：X参照通道图形放大");
	//		SetQuaternion(&m_GraphX,&m_GraphY);
	//	}
	//	else
	//	{
	//		QuaternionX q=qVector.back();
	//		m_GraphX.Axes.Item("XAxis").SetMinimum(q.xMin);
	//		m_GraphX.Axes.Item("XAxis").SetMaximum(q.xMax);	
	//		//m_GraphX.Axes.Item("YAxis-1").SetMinimum(q.yMin);
	//		//m_GraphX.Axes.Item("YAxis-1").SetMaximum(q.yMax);
	//	}
	//}
Invalidate();////Marshall  2011.3.2 16:36
}
//放大Y波形图
void CAxisOrbit::ZoomCwgraphy()
{


	OnXZoom();
	//if (FunSel==1)
	{
		if (GetXRang(&m_GraphY)>20*m_deltaFreq)
		{
			//if (FunSel==1)
				((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱操作：Y参照通道图形放大");
			//else
			//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：Y参照通道图形放大");

			SetQuaternion(&m_GraphY,&m_GraphX);
		}
		else
		{
			QuaternionX q=qVector.back();
			m_GraphY.Axes.Item("XAxis").SetMinimum(q.xMin);
			m_GraphY.Axes.Item("XAxis").SetMaximum(q.xMax);	
			/*m_GraphY.Axes.Item("YAxis-1").SetMinimum(q.yMin);
			m_GraphY.Axes.Item("YAxis-1").SetMaximum(q.yMax);*/
		}
	}
	//else
	//{
	//	if (GetXRang(&m_GraphY)>20)
	//	{
	//		if (FunSel==1)
	//			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱操作：Y参照通道图形放大");
	//		else
	//			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：Y参照通道图形放大");

	//		SetQuaternion(&m_GraphY,&m_GraphX);
	//	}
	//	else
	//	{
	//		QuaternionX q=qVector.back();
	//		m_GraphY.Axes.Item("XAxis").SetMinimum(q.xMin);
	//		m_GraphY.Axes.Item("XAxis").SetMaximum(q.xMax);	
	//		/*m_GraphY.Axes.Item("YAxis-1").SetMinimum(q.yMin);
	//		m_GraphY.Axes.Item("YAxis-1").SetMaximum(q.yMax);*/
	//	}
	//}

	Invalidate();////Marshall  2011.3.2 16:36
}


//获取波形图范围
//@NI::CNiGraph* mG	通道数据
double CAxisOrbit::GetXRang(NI::CNiGraph* mG)
{
	return mG->Axes.Item("XAxis").GetMaximum()-mG->Axes.Item("XAxis").GetMinimum();
}

//设置通道数据
//@NI::CNiGraph* mG1		通道数据
void CAxisOrbit::SetAxisData(NI::CNiGraph* mG1)
{
	int xMax=floor(mG1->Axes.Item("XAxis").GetMaximum()/m_deltaFreq);

	int xMin=ceil(mG1->Axes.Item("XAxis").GetMinimum()/m_deltaFreq);
	int round=xMax-xMin;
	CNiReal64Vector newData1(round);
	if (round>0)
	{
		for (int i=0;i<round;i++)
		{
			newData1[i]=dataX[xMin];
			xMin++;
		}
	}

	xMax=floor(mG1->Axes.Item("XAxis").GetMaximum()/m_deltaFreq);

	xMin=ceil(mG1->Axes.Item("XAxis").GetMinimum()/m_deltaFreq);
	round=xMax-xMin;

	CNiReal64Vector newData2(round);
	if (round>0)
	{
		for (int i=0;i<round;i++)
		{
			newData2[i]=dataY[xMin];
			xMin++;
		}
	}
	//if (FunSel==1)
	{
		m_GraphAxis.GetPlots().Item(1).ClearData();

		m_GraphAxis.GetPlots().Item(1).PlotXvsY(newData1,newData2);
	}
	//if (FunSel==2)
	//{
	//	CNiReal64Vector correlation;
	//	CNiMath::Correlate (newData1, newData2, correlation);
	//	m_GraphAxis.ClearData();
	//	m_GraphAxis.PlotY (correlation);
	//}
	//m_GraphAxis.GetPlots().Item(1).ClearData();

 //	m_GraphAxis.GetPlots().Item(1).PlotXvsY(newData1,newData2);
}
//消息：撤销图形放大
void CAxisOrbit::OnAxisunzoom()
{
	//OnXZoom();
	//if (FunSel==1)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱操作：撤销图形放大");
		GetQuaternion(&m_GraphX,&m_GraphY);
		SetAxisData(&m_GraphX);	
	}
	//else
	//{
	//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：撤销图形放大");
	//	GetQuaternion(&m_GraphX,&m_GraphY);
	//	SetAxisData(&m_GraphX);
	//}

Invalidate();////Marshall  2011.3.2 16:36
}
//存储图形范围
//@NI::CNiGraph* mG1		X通道数据
//@NI::CNiGraph* mG2		Y通道数据
void CAxisOrbit::SetQuaternion(NI::CNiGraph* mG1, NI::CNiGraph* mG2)
{
	//if (FunSel==1)
	{
		QuaternionX q;	

		q.xMin=mG1->Axes.Item("XAxis").GetMinimum();
		q.xMax=mG1->Axes.Item("XAxis").GetMaximum();
		//q.yMin=mG1->Axes.Item("YAxis-1").GetMinimum();
		//q.yMax=mG1->Axes.Item("YAxis-1").GetMaximum();

		qVector.push_back(q);

		mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
		mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
		//mG2->Axes.Item("YAxis-1").SetMinimum(q.yMin);
		//mG2->Axes.Item("YAxis-1").SetMaximum(q.yMax);

		SetAxisData(mG1);
	}
	//else
	//{
	//	QuaternionX q;	

	//	q.xMin=mG1->Axes.Item("XAxis").GetMinimum();
	//	q.xMax=mG1->Axes.Item("XAxis").GetMaximum();
	//	//q.yMin=mG1->Axes.Item("YAxis-1").GetMinimum();
	//	//q.yMax=mG1->Axes.Item("YAxis-1").GetMaximum();

	//	qVector.push_back(q);

	//	mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
	//	mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
	//	//mG2->Axes.Item("YAxis-1").SetMinimum(q.yMin);
	//	//mG2->Axes.Item("YAxis-1").SetMaximum(q.yMax);

	//	SetAxisData(mG1);
	//}

}
//获取图形范围
//@NI::CNiGraph* mG1		X通道数据
//@NI::CNiGraph* mG2		Y通道数据
void CAxisOrbit::GetQuaternion(NI::CNiGraph* mG1, NI::CNiGraph* mG2)
{
	//if (FunSel==1)
	{
		if (qVector.size()>1)
		{
			qVector.pop_back();
			QuaternionX q=qVector.back();
			mG1->Axes.Item("XAxis").SetMinimum(q.xMin);
			mG1->Axes.Item("XAxis").SetMaximum(q.xMax);	
			//mG1->Axes.Item("YAxis-1").SetMinimum(q.yMin);
			//mG1->Axes.Item("YAxis-1").SetMaximum(q.yMax);
			
			mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
			mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
			//mG2->Axes.Item("YAxis-1").SetMinimum(q.yMin);
			//mG2->Axes.Item("YAxis-1").SetMaximum(q.yMax);
		}
		else
		{
			OnResumexy();
		}
	}
	//else
	//{
	//	if (qVector.size()>1)
	//	{
	//		qVector.pop_back();
	//		QuaternionX q=qVector.back();
	//		mG1->Axes.Item("XAxis").SetMinimum(q.xMin);
	//		mG1->Axes.Item("XAxis").SetMaximum(q.xMax);	
	//		//mG1->Axes.Item("YAxis-1").SetMinimum(q.yMin);
	//		//mG1->Axes.Item("YAxis-1").SetMaximum(q.yMax);

	//		mG2->Axes.Item("XAxis").SetMinimum(q.xMin);
	//		mG2->Axes.Item("XAxis").SetMaximum(q.xMax);	
	//		//mG2->Axes.Item("YAxis-1").SetMinimum(q.yMin);
	//		//mG2->Axes.Item("YAxis-1").SetMaximum(q.yMax);
	//	}
	//	else
	//	{
	//		OnResumexy();
	//	}
	//}

}
//消息：重新加载数据绘制图形
void CAxisOrbit::OnResumexy()

{
	OnXZoom();
	//if (!isWaveFilter)
	{
		//if (FunSel==1)
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱操作：重新加载数据绘制图形");
		//else
		//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱操作：重新加载数据绘制图形");
	}

	double minValue, maxValue, valueDecl;
	unsigned int minIndex, maxIndex;
	dataX.MinMax(minValue, minIndex, maxValue, maxIndex);
	valueDecl=(maxValue-minValue)*0.1;
	qVector.clear();

	/*
	m_GraphX.GetPlots().Item(1).PlotY(dataX,0,1);
	m_GraphY.GetPlots().Item(1).PlotY(dataY,0,1);
	m_GraphX.Axes.Item("XAxis").SetMinMax(0,dataX.GetSize());
	*/



	///////////////////////////////////////////////////////////////////
	int round;
	round=ResumeDataX.GetSize();
	CNiReal64Vector temp_data(round);
	if (round>0)
	{
		for (int i=0;i<round;i++)
		{
			temp_data[i]=i*m_deltaFreq;

		}
	}


	m_GraphX.GetPlots().Item(1).ClearData();
	m_GraphX.GetPlots().Item(1).PlotXvsY(temp_data,dataX);
	m_GraphX.Axes.Item("XAxis").SetMinMax(0,dataX.GetSize()*m_deltaFreq);


	m_GraphY.GetPlots().Item(1).ClearData();
	m_GraphY.GetPlots().Item(1).PlotXvsY(temp_data,dataY);
	m_GraphY.Axes.Item("XAxis").SetMinMax(0,dataY.GetSize()*m_deltaFreq);


	m_GraphAxis.GetPlots().Item(1).ClearData();
	m_GraphAxis.GetPlots().Item(1).PlotXvsY (dataX,dataY);
	/////////////////////////////////////////////////////////////////
 	SetQuaternion(&m_GraphX,&m_GraphY);

	Invalidate();////Marshall  2011.3.2 16:36
}
//消息：保存视图到图像
void CAxisOrbit::OnSaveallviewxy()
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
	//strFileName="kdkkdkfkdskfkf";

	int nRight = strFileName.Find(_T('//'));
	strFileName.Delete(nRight, 1);
	//m_Graph.GetWindowText(strFileName) 
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
		//if (FunSel==1)
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱存储：保存视图到图像...");
		//else
		//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱存储：保存视图到图像...");

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
		//if (FunSel==1)
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱存储：保存完成，存储为  "+strSaveFilapath);
		//else
		//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱存储：保存完成，存储为  "+strSaveFilapath);

	}
	else
	{
		//if (FunSel==1)
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("二维轴心轨迹图谱存储：取消保存视图到图像");
		//else
		//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("相干分析图谱存储：取消保存视图到图像");

	}
	image.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
}

//消息：重绘，显示特征值
//void CAxisOrbit::OnDraw(CDC* /*pDC*/)
void CAxisOrbit::OnDraw(CDC* pDC)
{
	 CWnd* pWndW=GetDlgItem(IDC_TitleXY);
	if(pWndW == NULL)
		return;
   // pWndW->SetWindowText(WinTitle);  //cxy

	CRect rc;
	pWndW->GetWindowRect(&rc);
	pWndW->Invalidate();
	pWndW->UpdateWindow();
	
 
	//
	//CFont   font;   
	//CFont   *pOldFont=NULL;   
	//font.CreatePointFont(150,"宋体");   
	//pOldFont=(CFont* )pDCF->SelectObject(&font);

	//pDCF->SetTextColor(RGB(0,0,255)); 
	////pDCF->TextOut(200,2,WinTitle);  //调整标题//Marshall  2011.3.2 13:21
	////pDCF->TextOut(100,2,WinTitle);  ////Marshall  2011.3.2 13:21
	//pWndW->SetWindowText(WinTitle);
	//pDCF->SelectObject(pOldFont);
	//
	////this->SetWindowText(WinTitle);
	//ReleaseDC(pDCF);


	/*--------------------------Marshall's Code2011.3.2 12:41--------------*/
	

	CWnd* pWave=GetDlgItem(IDC_TitleXY);
	if(pWave == NULL)
		return;
	//CDC* pDCWave=pWave->GetDC();
	CPen pen(PS_SOLID,2,RGB(192,192,192));
	CPen *pOld = pDC->SelectObject(&pen);
	//
//	CRect rc;
	pWave->GetWindowRect(&rc);
	pDC->MoveTo(0,rc.Height());
	pDC->LineTo(rc.Width() + 1000,rc.Height());


	//pDC->MoveTo(rc.Width() - rc.Height()*6,rc.Height());
	//pDC->LineTo(rc.Width() - rc.Height()*6,rc.Height() + 100);

	pWave = GetDlgItem(IDC_CWGRAPHX);
	if(pWave == NULL)
		return;
	pWave->GetWindowRect(&rc);
	CDC* pDCWave=pWave->GetDC();

	
	//CWnd* pWndY = GetDlgItem(IDC_CWGRAPHY);
	//pWndY->GetWindowRect(&rc);

	pWave->Invalidate();
	pWave->UpdateWindow();
	//pWndY->Invalidate();
	//pWndY->UpdateWindow();

	CPen* pOldWave = pDCWave->SelectObject(&pen);
	pDCWave->MoveTo(rc.Width(),0);
	pDCWave->LineTo(rc.Width(),1000);

	pDCWave->MoveTo(0,rc.Height());
	pDCWave->LineTo(1000,rc.Height());

	pDC->SelectObject(pOld);
	pDCWave->SelectObject(pOldWave);
	//pDCWave->ReleaseDC();

	CWnd* pWndY = GetDlgItem(IDC_CWGRAPHY);
	if(pWndY == NULL)
		return;

	pWndY->GetWindowRect(&rc);
	CDC* pDCY = pWndY->GetDC();

	pWndY->Invalidate();
	pWndY->UpdateWindow();

	CPen* pOldPenY = (CPen*) pDCY->SelectObject(&pen);
	pDCY->MoveTo(rc.Width(),0);
	pDCY->LineTo(rc.Width(),1000);

	pDCY->MoveTo(0,rc.Height());
	pDCY->LineTo(1000,rc.Height());

	pDCY->SelectObject(pOldPenY);


	CWnd* pWndXY = GetDlgItem(IDC_CWGRAPHAXIS);
	if(pWndXY == NULL)
		return;

	pWndXY->GetWindowRect(&rc);
	CDC* pDCXY = pWndXY->GetDC();

	pWndXY->Invalidate();
	pWndXY->UpdateWindow();

	CPen* pOldPenXY = (CPen*) pDCXY->SelectObject(&pen);
	pDCXY->MoveTo(rc.Width(),0);
	pDCXY->LineTo(rc.Width(),1000);

	pDCXY->MoveTo(-1000,rc.Height());
	pDCXY->LineTo(1000,rc.Height());

	pDCXY->SelectObject(pOldPenXY);

	ReleaseDC(pDCWave);
	ReleaseDC(pDCY);
	ReleaseDC(pDCXY);
	/*--------------------------Marshall's Code2011.3.2 12:41--------------*/
}

//设置通道Y参照坐标数据
//@vector <float> vData		Y通道数据
//@CString title			Y标题
void CAxisOrbit::SetDataY(vector <float> vData,int sampling,CString title)
{
	/*xRange=(unsigned int)vData.size()+1;*/
	m_deltaFreq=1.0/samplingFreq;
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
void CAxisOrbit::SetDataX(vector <float> vData,int sampling,CString title)
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
void CAxisOrbit::SetWinTitle(CString title)
{
	WinTitle=title;
}
//消息：创建窗体
int CAxisOrbit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if(pMainFrame)
	{
		//pMainFrame->m_pExamView = this;
		pMainFrame->m_Axis = this;
	}

	return 0;
}

//根据类型设置波形图显示样式
//@int sel		窗体样式
void CAxisOrbit::SetFunSel(/*int sel*/)
{
	//FunSel=sel;
	CRect   rect   ;   
	this->GetWindowRect(   rect   );   
	ScreenToClient(   rect   )   ;   
	this->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());  
}
////消息：恢复放大
//void CAxisOrbit::OnCoherentreset()
//{
//	OnResumexy();
//}
////消息：保存图片
//void CAxisOrbit::OnCoherentsavetoimage()
//{
//	OnSaveallviewxy();
//}
////消息：撤销放大
//void CAxisOrbit::OnCoherentunzoom()
//{
//	OnAxisunzoom();
//}

//消息：设置焦点，激活工具栏
void CAxisOrbit::OnSetFocus(CWnd* pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);
	//if (FunSel==1)
	{
		CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
		pCMF->ActivateTabEG(2);
		//pCMF->ActivateTabEA(0);
	}
	//else
	//{
	//	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	//	pCMF->ActivateTabEG(4);
	//	//pCMF->ActivateTabEG(0);
	//}

	// TODO: 在此处添加消息处理程序代码
}

void CAxisOrbit::OnYwavefilter()
{

	OnXZoom();
	
	int m_min,m_max;
	m_min=ceil(m_GraphX.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);
	m_max=floor(m_GraphX.Axes.Item("XAxis").GetMaximum()/m_deltaFreq);	

	//CNiReal64Vector vFilter(dataX.GetSize());
	CNiReal64Vector vFilter(m_max-m_min);

	//isWaveFilter=TRUE;

	CWaveFilter wf;
	wf.m_Freq=samplingFreq;

	if(wf.DoModal()==IDOK)
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：X波形滤波处理");

		double low=wf.m_FreqLowCut;
		double high=wf.m_FreqHighCut;
		int order=wf.m_order;
/////////////////////////////////////////////////////////////////////////////
		int m_flag=dataY.GetSize();

		if (m_flag>3072)
			m_flag=3072;
		else
			m_flag=int(dataY.GetSize());


		CNiReal64Vector tempNiData(dataY.GetSize()+m_flag);

		CNiReal64Vector tempvFilter(dataY.GetSize()+m_flag);


		for (int i=0;i<m_flag;i++)
			tempNiData[i]=dataY[i];

		for (int i=m_flag;i<m_flag+dataY.GetSize();i++)
			tempNiData[i]=dataY[i-m_flag];
//////////////////////////////////////////////////////////////////////////////////



		switch(wf.m_FilterType)
		{
		case 0:
			wf.m_FreqLowCut;
			//CNiMath::ButterworthLowPass(dataY, vFilter, samplingFreq, low, order);
			//dataY=vFilter;
			CNiMath::ButterworthLowPass(tempNiData, tempvFilter, samplingFreq, low, order);
			for (int i=m_flag;i<m_flag+dataY.GetSize();i++)
				dataY[i-m_flag]=tempvFilter[i];


			break;
		case 1:
			//CNiMath::ButterworthHighPass(dataY, vFilter, samplingFreq, high, order);
			//dataY=vFilter;
			CNiMath::ButterworthHighPass(tempNiData, tempvFilter, samplingFreq, low, order);
			for (int i=m_flag;i<m_flag+dataY.GetSize();i++)
				dataY[i-m_flag]=tempvFilter[i];

			break;
		case 2:
			CNiMath::ButterworthBandPass(tempNiData, tempvFilter, samplingFreq, low, order);
			for (int i=m_flag;i<m_flag+dataY.GetSize();i++)
				dataY[i-m_flag]=tempvFilter[i];

			//CNiMath::ButterworthBandPass(dataY, vFilter, samplingFreq, low, high, order);
			//dataY=vFilter;
			break;
		case 3:
			CNiMath::ButterworthBandStop(tempNiData, tempvFilter, samplingFreq, low, order);
			for (int i=m_flag;i<m_flag+dataY.GetSize();i++)
				dataY[i-m_flag]=tempvFilter[i];

			//CNiMath::ButterworthBandStop(dataY, vFilter, samplingFreq, low, high, order);
			//dataY=vFilter;
			break;
		default:
			break;
		}
		///////////////////////////////////////////////////////////////////
		int round;
		round=dataY.GetSize();
		//round=m_dataY.GetSize();
		CNiReal64Vector temp_data(round);
		if (round>0)
		{
			for (int i=0;i<round;i++)
			{
				//temp_data[i]=double(i+m_min)*m_deltaFreq;
				temp_data[i]=double(i)*m_deltaFreq;

			}
		}

	//	dataY=vFilter;

		m_GraphX.GetPlots().Item(1).ClearData();
		m_GraphX.GetPlots().Item(1).PlotXvsY(temp_data,dataX);
		m_GraphX.Axes.Item("XAxis").SetMinMax(double(m_min)*m_deltaFreq,double(m_max)*m_deltaFreq);

		m_GraphY.GetPlots().Item(1).ClearData();
		m_GraphY.GetPlots().Item(1).PlotXvsY(temp_data,dataY);
		m_GraphY.Axes.Item("XAxis").SetMinMax(double(m_min)*m_deltaFreq,double(m_max)*m_deltaFreq);

		//	m_GraphAxis.GetPlots().Item(1).ClearData();
		//	m_GraphAxis.GetPlots().Item(1).PlotXvsY (dataX,dataY);

		SetAxisData(&m_GraphX);


		Invalidate();
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：取消X波形滤波处理");
	}
	//OnResumexy();

	
}

void CAxisOrbit::OnXwavefilter()
{

	OnXZoom();



	int m_min,m_max;
	m_min=ceil(m_GraphX.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);
	m_max=floor(m_GraphX.Axes.Item("XAxis").GetMaximum()/m_deltaFreq);	

	//CNiReal64Vector vFilter(dataX.GetSize());
	CNiReal64Vector vFilter(m_max-m_min);

	//isWaveFilter=TRUE;

	CWaveFilter wf;
	wf.m_Freq=samplingFreq;

	/////////////////////////////////////////////////////////////////////////////
	int m_flag=dataX.GetSize();

	if (m_flag>3072)
		m_flag=3072;
	else
		m_flag=int(dataX.GetSize());


	CNiReal64Vector tempNiData(dataX.GetSize()+m_flag);

	CNiReal64Vector tempvFilter(dataX.GetSize()+m_flag);


	for (int i=0;i<m_flag;i++)
		tempNiData[i]=dataX[i];

	for (int i=m_flag;i<m_flag+dataX.GetSize();i++)
		tempNiData[i]=dataX[i-m_flag];
	//////////////////////////////////////////////////////////////////////////////////

	if(wf.DoModal()==IDOK)
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：X波形滤波处理");

		double low=wf.m_FreqLowCut;
		double high=wf.m_FreqHighCut;
		int order=wf.m_order;
		switch(wf.m_FilterType)
		{
		case 0:
			wf.m_FreqLowCut;
		//	CNiMath::ButterworthLowPass(dataX, vFilter, samplingFreq, low, order);
		//	dataX=vFilter;

			CNiMath::ButterworthLowPass(tempNiData, tempvFilter, samplingFreq, low, order);
			for (int i=m_flag;i<m_flag+dataX.GetSize();i++)
				dataX[i-m_flag]=tempvFilter[i];
			break;
		case 1:
			//CNiMath::ButterworthHighPass(dataX, vFilter, samplingFreq, high, order);
			//dataX=vFilter;
			CNiMath::ButterworthHighPass(tempNiData, tempvFilter, samplingFreq, low, order);
			for (int i=m_flag;i<m_flag+dataX.GetSize();i++)
				dataX[i-m_flag]=tempvFilter[i];
			break;
		case 2:
			//CNiMath::ButterworthBandPass(dataX, vFilter, samplingFreq, low, high, order);
			//dataX=vFilter;
			CNiMath::ButterworthBandPass(tempNiData, tempvFilter, samplingFreq, low, order);
			for (int i=m_flag;i<m_flag+dataX.GetSize();i++)
				dataX[i-m_flag]=tempvFilter[i];
			break;
		case 3:
			//CNiMath::ButterworthBandStop(dataX, vFilter, samplingFreq, low, high, order);
			//dataX=vFilter;
			CNiMath::ButterworthBandStop(tempNiData, tempvFilter, samplingFreq, low, order);
			for (int i=m_flag;i<m_flag+dataX.GetSize();i++)
				dataX[i-m_flag]=tempvFilter[i];
			break;
		default:
			break;
		}
		///////////////////////////////////////////////////////////////////
		int round;
		round=dataX.GetSize();
		//round=m_dataY.GetSize();
		CNiReal64Vector temp_data(round),temX(round),temY(round);
		if (round>0)
		{
			for (int i=0;i<round;i++)
			{
				//temp_data[i]=double(i+m_min)*m_deltaFreq;
				temp_data[i]=double(i)*m_deltaFreq;


			}
		}



		//dataX=vFilter;

		m_GraphX.GetPlots().Item(1).ClearData();
		m_GraphX.GetPlots().Item(1).PlotXvsY(temp_data,dataX);
		m_GraphX.Axes.Item("XAxis").SetMinMax(double(m_min)*m_deltaFreq,double(m_max)*m_deltaFreq);


		//m_GraphX.SetTrackMode(CNiGraph::TrackZoomRectYWithUndo);
		//m_GraphX.SetTrackMode(CNiGraph::TrackZoomRectXWithUndo);





		//m_GraphX.Axes.Item("YAxis").

		m_GraphY.GetPlots().Item(1).ClearData();
		m_GraphY.GetPlots().Item(1).PlotXvsY(temp_data,dataY);
		m_GraphY.Axes.Item("XAxis").SetMinMax(double(m_min)*m_deltaFreq,double(m_max)*m_deltaFreq);




		SetAxisData(&m_GraphX);


		Invalidate();

	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：取消X波形滤波处理");
	}
	//OnResumexy();

	
}

void CAxisOrbit::OnResumefilterxy()
{
	OnXZoom();
	
	//isWaveFilter=TRUE;
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：恢复滤波前的数据");
	dataX=ResumeDataX;
	dataY=ResumeDataY;
	/*
	m_GraphX.GetPlots().Item(1).ClearData();
	m_GraphX.GetPlots().Item(1).PlotY(dataX,0,1);
	m_GraphY.GetPlots().Item(1).ClearData();
	m_GraphY.GetPlots().Item(1).PlotY(dataY,0,1);
*/
	int round;
	round=dataX.GetSize();
	//round=m_dataY.GetSize();
	CNiReal64Vector temp_data(round);
	if (round>0)
	{
		for (int i=0;i<round;i++)
		{
			temp_data[i]=double(i)*m_deltaFreq;

		}
	}

	m_GraphX.GetPlots().Item(1).ClearData();
	m_GraphX.GetPlots().Item(1).PlotXvsY(temp_data,dataX);


	m_GraphY.GetPlots().Item(1).ClearData();
	m_GraphY.GetPlots().Item(1).PlotXvsY(temp_data,dataY);



	SetAxisData(&m_GraphX);
	//isWaveFilter=FALSE;

	Invalidate();////Marshall  2011.3.2 16:36
}

//BOOL CAxisOrbit::PreCreateWindow(CREATESTRUCT& cs)//尝试调整界面//Marshall  2011.3.2 12:11
//{
//	// TODO: 在此处通过修改
//	//  CREATESTRUCT cs 来修改窗口类或样式
//
//	return CBCGPFormView::PreCreateWindow(cs);
//}
void CAxisOrbit::OnOrbitTitle()
{
	// TODO: 
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

void CAxisOrbit::OnYZoom()
{
	m_GraphX.SetTrackMode(CNiGraph::TrackZoomRectYWithUndo);
	m_GraphY.SetTrackMode(CNiGraph::TrackZoomRectYWithUndo);
}

void CAxisOrbit::OnXZoom()
{
	m_GraphX.SetTrackMode(CNiGraph::TrackZoomRectXWithUndo);
	m_GraphY.SetTrackMode(CNiGraph::TrackZoomRectXWithUndo);
}
