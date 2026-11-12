// NiWaveFraqView.cpp : CNiWaveFraqView 类的实现
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include   <stdio.h>
#include "MainFrm.h"
#include "ScanAnalysisDoc.h"
#include "NiWaveFraqView.h"
#include "HSJAppearance.h"
#include "CustomSaveDlg.h"
#include <atlimage.h>
#include <GdiPlus.h>
#include <cmath>
#include <sstream> 
#include "AddChnData.h"
#include "workspacebar.h"
#include <math.h>
#include "AddMobTrend.h"
//#include "CSpreadSheet.h"
#include "WaveFilter.h"
#include "SaveDataDlg.h"
#include "SaveCHNToExcel.h"
#include "SaveImageBatDlg.h"
#include "SaveCHNWord.h"
#include "ThrowLoadSetDlg.h"
#include "WaveAnalysisTimeSpanSetDlg.h"

#include "ThrowLoadAna.h"
//#include "msword.h"
//#include "excel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(disable : 4996)
using namespace Gdiplus;

/*--------------------------Marshall's Code2011.2.24 10:51--------------*/
extern bool g_bConf97;
extern bool g_bConf95;
extern bool g_bConf100;
//#define WM_USER_FULLPR (WM_USER + 100)//整周期用////Marshall  2011.2.25 16:36
/*--------------------------Marshall's Code2011.2.24 10:51--------------*/


//#define TEXT_WIDTH 200
#define TEXT_WIDTH 250  //cxy20231224

static 		CString title;
// CSetTitle 对话框
using namespace std;
IMPLEMENT_DYNAMIC(CSetTitle, CHSJDialog)

CSetTitle::CSetTitle(CWnd* pParent /*=NULL*/)
: CHSJDialog(CSetTitle::IDD, pParent)
{

}

CSetTitle::~CSetTitle()
{
}

void CSetTitle::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_title);	
}
BOOL CSetTitle::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	//m_title.SetWindowText(title);//在标注时也使用这个对话框，所以采用下面的方法初始化//Marshall  2011.2.24 22:3
	m_title.SetWindowText(mTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CSetTitle, CHSJDialog)
END_MESSAGE_MAP()


// CSetTitle 消息处理程序

void CSetTitle::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_title.GetWindowText(mTitle);
	CHSJDialog::OnOK();
}
// CNiWaveFraqView

IMPLEMENT_DYNCREATE(CNiWaveFraqView, CFormView)

BEGIN_MESSAGE_MAP(CNiWaveFraqView, CFormView)
	ON_COMMAND(ID_Zoom, &CNiWaveFraqView::OnZoom)
	ON_COMMAND(ID_Moving, &CNiWaveFraqView::OnMoving)
	ON_COMMAND(ID_UnZoom, &CNiWaveFraqView::OnUnzoom)
	ON_COMMAND(ID_resume, &CNiWaveFraqView::Onresume)
	ON_COMMAND(ID_SaveAllView, &CNiWaveFraqView::OnSave)
	ON_COMMAND(ID_CustomSaveAllView, &CNiWaveFraqView::OnCustomsave)
	ON_COMMAND(ID_WaveFilter, &CNiWaveFraqView::OnWaveFilter)
	ON_COMMAND(ID_ResumeFilter, &CNiWaveFraqView::OnResumeFilter)
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_COMMAND(ID_XZOOM, &CNiWaveFraqView::OnXzoom)
	ON_COMMAND(ID_CurrorNearestPoint, &CNiWaveFraqView::OnCurrornearestpoint)
	ON_COMMAND(ID_CurrorFloat, &CNiWaveFraqView::OnCurrorfloat)
	ON_COMMAND(ID_Annotations, &CNiWaveFraqView::OnAnnotations)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_SaveSingleView, &CNiWaveFraqView::OnSavesingleview)
	ON_COMMAND(ID_SetTitle, &CNiWaveFraqView::OnSettitle)
//	ON_WM_TIMER()
//	ON_COMMAND(ID_WordReport, &CNiWaveFraqView::OnWordreport)
	ON_WM_CREATE()
	ON_COMMAND(ID_XZOOMF, &CNiWaveFraqView::OnXzoomf)
	ON_COMMAND(ID_ZoomF, &CNiWaveFraqView::OnZoomf)
	ON_COMMAND(ID_XMovingF, &CNiWaveFraqView::OnXmovingf)
	ON_COMMAND(ID_CurrorNearestPointF, &CNiWaveFraqView::OnCurrornearestpointf)
	ON_COMMAND(ID_YZOOM, &CNiWaveFraqView::OnYzoom)
	ON_COMMAND(ID_WaveGraph, &CNiWaveFraqView::OnWavegraph)
	ON_COMMAND(ID_WAVEFRAQGRAP, &CNiWaveFraqView::OnWavefraqgrap)
	ON_COMMAND(ID_FRAQGRAPH, &CNiWaveFraqView::OnFraqgraph)
	ON_COMMAND(ID_UnZoomF, &CNiWaveFraqView::OnUnzoomf)
	ON_COMMAND(ID_resumeF, &CNiWaveFraqView::Onresumef)
	ON_COMMAND(ID_AddChnData, &CNiWaveFraqView::OnAddchndata)
	//ON_WM_CLOSE()
	ON_COMMAND(ID_AddMobTrend, &CNiWaveFraqView::OnAddmobtrend)

	ON_COMMAND(ID_ExcelReport, &CNiWaveFraqView::OnExcelreport)
	ON_COMMAND(ID_WordReport, &CNiWaveFraqView::OnWordreport)
	ON_COMMAND(ID_SAVETXTDATA, &CNiWaveFraqView::OnSavetxtdata)
	ON_COMMAND(ID_SAVEPICBAT, &CNiWaveFraqView::OnSavepicbat)
	ON_COMMAND(ID_SAVEEXCELDATA, &CNiWaveFraqView::OnSaveexceldata)
	ON_COMMAND(ID_TextSave, &CNiWaveFraqView::OnTextsave)
	ON_COMMAND(ID_KeyPhaseVisual, &CNiWaveFraqView::OnKeyphasevisual)
	ON_UPDATE_COMMAND_UI(ID_KeyPhaseVisual, &CNiWaveFraqView::OnUpdateKeyphasevisual)
	ON_COMMAND(ID_SAVEWORDDATA, &CNiWaveFraqView::OnSaveworddata)
	ON_COMMAND(ID_FullPeriodFFT, &CNiWaveFraqView::OnFullperiodfft)
	ON_UPDATE_COMMAND_UI(ID_FullPeriodFFT, &CNiWaveFraqView::OnUpdateFullperiodfft)
	ON_UPDATE_COMMAND_UI(ID_WaveGraph, &CNiWaveFraqView::OnUpdateWavegraph)
	ON_UPDATE_COMMAND_UI(ID_FRAQGRAPH, &CNiWaveFraqView::OnUpdateFraqgraph)
	ON_UPDATE_COMMAND_UI(ID_WAVEFRAQGRAP, &CNiWaveFraqView::OnUpdateWavefraqgrap)
	//波形图标注消息响应//Marshall  2011.2.24 21:13
	ON_COMMAND(ID_ANNOTATION,&CNiWaveFraqView::OnAnnotation)
	ON_MESSAGE(WM_USER_FULLPR,&CNiWaveFraqView::OnFullPR)//整周期//Marshall  2011.2.25 16:58
	ON_COMMAND(ID_CURR_TEXTDATA, &CNiWaveFraqView::OnCurrTextdata)
	ON_COMMAND(ID_FREQ_TEXTDATA, &CNiWaveFraqView::OnFreqTextdata)
	ON_COMMAND(ID__TIME_SPAN_NON_FUUL_PERIOD, &CNiWaveFraqView::OnTimeSpan)
	ON_COMMAND(ID__CANCEL_ALL_ANNOTATION, &CNiWaveFraqView::OnCancelAnnotations)
	ON_COMMAND(ID_ADJUST_ANNOTATION_PLACE, &CNiWaveFraqView::OnAdjustAnnotationPlace)
	ON_UPDATE_COMMAND_UI(ID_ADJUST_ANNOTATION_PLACE, &CNiWaveFraqView::OnUpdateAdjustAnnotationPlace)
	ON_COMMAND(ID_WINDOW_SET, &CNiWaveFraqView::OnWindowSet)
END_MESSAGE_MAP()

// CNiWaveFraqView 消息处理程序
BEGIN_EVENTSINK_MAP(CNiWaveFraqView, CBCGPFormView)
	//ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, DISPID_MOUSEUP, CNiWaveFraqView::MouseUpCwgraph2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, 15, CNiWaveFraqView::ZoomCwgraph2, VTS_NONE)
	ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, 1, CNiWaveFraqView::CursorChangeCwgraph2, VTS_PI4 VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	//	ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, DISPID_MOUSEMOVE, CNiWaveFraqView::MouseMoveCwgraph2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//	ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH1, DISPID_MOUSEMOVE, CNiWaveFraqView::MouseMoveCwgraph1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH1, DISPID_CLICK, CNiWaveFraqView::ClickCwgraph1, VTS_NONE)
	ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, DISPID_CLICK, CNiWaveFraqView::ClickCwgraph2, VTS_NONE)
	ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH1, 1, CNiWaveFraqView::CursorChangeCwgraph1, VTS_PI4 VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	//ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, 6, CNiWaveFraqView::PlotMouseMoveCwgraph2, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT VTS_PI2 VTS_PI4)
	//ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, 9, CNiWaveFraqView::PlotAreaMouseMoveCwgraph2, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT)
	//ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, 17, CNiWaveFraqView::PlotAreaBoundsChangeCwgraph2, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	//ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH2, DISPID_MOUSEMOVE, CNiWaveFraqView::MouseMoveCwgraph2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH1, 15, CNiWaveFraqView::ZoomCwgraph1, VTS_NONE)
	//ON_EVENT(CNiWaveFraqView, IDC_CWGRAPH1, 17, CNiWaveFraqView::PlotAreaBoundsChangeCwgraph1, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
END_EVENTSINK_MAP()

// CNiWaveFraqView 构造/析构

CNiWaveFraqView::CNiWaveFraqView()
	: CBCGPFormView(CNiWaveFraqView::IDD),isWaveFreq(FALSE),wordVision(1),isHasKeyPhase(FALSE),
	annotationsVisual(0)/*,pointVisual(0)*/,isKeyPhaseVisual(TRUE),isFullPeriodFFT(TRUE),minKZ(0),maxKZ(0),	isFreqgraph(TRUE),								  
	currorFVisual(0),currorNVisual(0),currorNVisualF(0),waveHeightMax(0),waveHeightMin(0),ChnFftFlag(0),isWavegraph(TRUE),
	windowStyle(0),samplingFreq(0),waveWidthMin(0),waveWidthMax(0),waveCout(2),col(),menuShow(TRUE),isFreqRangeChg(FALSE),m_batchSaveBit(false)
										 
	, m_strWaveTitle(_T("")),m_deltaFreq(1.0)
{
	isGraph=true;
	m_fontSize=18;
	isAnnotation=false;
	m_width=50;
	m_increase=10;
	isThrowload=false;
	m_isAnnotation=false;
	
}

CNiWaveFraqView::~CNiWaveFraqView()
{
}

void CNiWaveFraqView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH2, m_Graph);
	DDX_Control(pDX, IDC_CWGRAPH1, m_newGraph);
	DDX_Control(pDX, IDC_WAVE, m_wavePic);
	DDX_Control(pDX, IDC_FRAQ, m_fraqPic);
	DDX_Control(pDX, IDC_TITLE, m_ctrlTitle);
	DDX_Text(pDX, IDC_TITLE, m_strWaveTitle);
}

BOOL CNiWaveFraqView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CBCGPFormView::PreCreateWindow(cs);
}
//初始化
void CNiWaveFraqView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	//this->SetScrollSizes(MM_TEXT,CSize(0,0));//取消窗体滚动条

	SetScrollSizes(MM_TEXT,CSize(0,0));
	// Change the title typeface without changing its point size or window bounds.
	CFont* titleFont = m_ctrlTitle.GetFont();
	if (titleFont && !m_skinTitleFont.GetSafeHandle()) {
		LOGFONT lf = {};
		titleFont->GetLogFont(&lf);
		lstrcpyn(lf.lfFaceName, _T("Microsoft YaHei UI"), LF_FACESIZE);
		lf.lfQuality = CLEARTYPE_QUALITY;
		if (m_skinTitleFont.CreateFontIndirect(&lf)) m_ctrlTitle.SetFont(&m_skinTitleFont);
	}
	HSJStyleGraph(m_Graph);
	HSJStyleGraph(m_newGraph);
	InitNiGraph();
	ResizeParentToFit();
	//GetWordVision();
}

// CNiWaveFraqView 诊断
#ifdef _DEBUG
void CNiWaveFraqView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CNiWaveFraqView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

//CScanAnalysisDoc* CNiWaveFraqView::GetDocument() const // 非调试版本是内联的
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanAnalysisDoc)));
//	return (CScanAnalysisDoc*)m_pDocument;
//}
#endif //_DEBUG


//初始化波形频谱图
void CNiWaveFraqView::InitNiGraph()
{
	double minValue, maxValue, valueDecl;
	unsigned int minIndex, maxIndex;
	NiData.MinMax(minValue, minIndex, maxValue, maxIndex);
	valueDecl=(maxValue-minValue)*0.1;
	
	CNiColor blue(40,104,197);
	m_Graph.GetPlots().Add();
	if (col!=NULL)
		m_Graph.GetPlots().Item(1).LineColor=col;	
	else
		m_Graph.GetPlots().Item(1).LineColor=blue;			
	
	m_Graph.GetPlots().Item(1).PlotY(NiData, 0, m_deltaFreq);
 	//m_Graph.Axes.Item("XAxis").SetMinMax(waveWidthMin*m_deltaFreq,waveWidthMax*m_deltaFreq);
	//FFTProc();


 /*	
	CString   strTemp;
	double m_t;
	m_t=double(1.0*waveWidthMin/samplingFreq);
	strTemp.Format("%lf",m_t);
	AfxMessageBox(strTemp);
*/ 




	if ((int)ChnFftFlag==1)
	{
		frequencyData.MinMax(minValue, minIndex, maxValue, maxIndex);
		valueDecl=(maxValue-minValue)*0.1;

	
		m_newGraph.Plots.Item(1).LineStyle = CNiPlot::LineNone;
	
		SetFraqData(NiData,samplingFreq);
		m_newGraph.GetPlots().Item(1).PlotY(frequencyData,0,deltaFreq);
		m_newGraph.Axes.Item("XAxis").SetMinMax(0,freqRange);
		SetQuaternionF();

		windowStyle=0;
		CRect   rect   ;   
		this->GetWindowRect(   rect   );   
		ScreenToClient(   rect   )   ;   
		this->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());   

	}
	else
	{
		windowStyle=1;
		CRect   rect   ;   
		this->GetWindowRect(   rect   );   
		ScreenToClient(   rect   )   ;   
		this->OnSize(SIZE_RESTORED,rect.Width(),rect.Height()); 		
	}


	RECT controlBounds;
	m_Graph.GetClientRect(&controlBounds);

	CNiColor col(0,0,0);
	m_Graph.Annotations.Add();
	m_Graph.Annotations.Item(1).Plot = m_Graph.Plots.Item(1);
	m_Graph.Annotations.Item(1).SnapMode = CNiAnnotation::SnapAnchoredToPoint;
	m_Graph.Annotations.Item(1).Shape.Type = CNiShape::Point;
	m_Graph.Annotations.Item(1).Arrow.LineStyle = CNiArrow::LineDash;
	m_Graph.Annotations.Item(1).Caption.Text = _T("Max");
	m_Graph.Annotations.Item(1).Caption.Color=col;
	m_Graph.Annotations.Item(1).Arrow.Color=col;
	m_Graph.Annotations.Item(1).Enabled = true;
	m_Graph.Annotations.Item(1).Visible = false;
	m_Graph.Annotations.Add();
	m_Graph.Annotations.Item(2).Plot = m_Graph.Plots.Item(1);
	m_Graph.Annotations.Item(2).SnapMode = CNiAnnotation::SnapAnchoredToPoint;
	m_Graph.Annotations.Item(2).Shape.Type = CNiShape::Point;
	m_Graph.Annotations.Item(2).Arrow.LineStyle = CNiArrow::LineDash;
	m_Graph.Annotations.Item(2).Caption.Text = _T("Min");
	m_Graph.Annotations.Item(2).Caption.Color=col;
	m_Graph.Annotations.Item(2).Arrow.Color=col;
	m_Graph.Annotations.Item(2).Enabled = true;
	m_Graph.Annotations.Item(2).Visible = false;
	
 	SetWaveParam();		
 	//TextData();
 	SetQuaternion();  
	//Onresume();

/*	CString strTemp;
	strTemp.Format("001:%lf",m_Graph.Axes.Item("XAxis").GetMaximum()*m_deltaFreq);
	AfxMessageBox(strTemp);
*/

	

	m_ctrlTitle.SetWindowText(title);////Marshall  2011.2.21 10:39
	ResumeData=NiData;

}
//消息：X轴向数据放大
void CNiWaveFraqView::OnXzoom()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：X轴向数据放大...");
	m_Graph.SetTrackMode(CNiGraph::TrackZoomRectXWithUndo);
}
//消息：Y轴向数据放大
void CNiWaveFraqView::OnYzoom()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：Y轴向数据放大...");
	m_Graph.SetTrackMode(CNiGraph::TrackZoomRectYWithUndo);

}
//消息：XY轴数据窗口式放大
void CNiWaveFraqView::OnZoom()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：XY轴数据窗口式放大...");
	m_Graph.SetTrackMode(CNiGraph::TrackZoomRectXYWithUndo);

}

//消息：XY轴向平移绘图区域视窗
void CNiWaveFraqView::OnMoving()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：XY轴向平移绘图区域视窗...");
	m_Graph.SetTrackMode(CNiGraph::TrackPanPlotAreaXY);	
	SetQuaternion();
}

//消息：显示捡取线，捡取临近点
void CNiWaveFraqView::OnCurrornearestpoint()
{	

	currorNVisual++;
	if (currorNVisual%2==0)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：关闭显示捡取线，捡取临近点");
		m_Graph.Cursors.Item(1).CrosshairStyle = CNiCursor::CrosshairNone;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：显示捡取线，捡取临近点");
		m_Graph.Cursors.RemoveAll();
		m_Graph.Cursors.Add();
		m_Graph.SetTrackMode(CNiGraph::TrackDragCursor);
		m_Graph.Cursors.Item(1).SetColor(RGB(0,255,0));
		m_Graph.Cursors.Item(1).SnapMode = CNiCursor::SnapNearestPoint;
	 
		double xValue = (m_Graph.Axes.Item("XAxis").GetMaximum() + m_Graph.Axes.Item("XAxis").GetMinimum()) / 2;
		double yValue = (m_Graph.Axes.Item("YAxis-1").GetMaximum() + m_Graph.Axes.Item("YAxis-1").GetMinimum()) / 2;

		m_Graph.Cursors.Item(1).SetPosition(xValue, yValue);
		if (currorFVisual%2!=0)
		{
			currorFVisual++;
		}
	}
}


//消息：显示捡取线，捡取浮动点
void CNiWaveFraqView::OnCurrorfloat()
{	
	currorFVisual++;

	if (currorFVisual%2==0)
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：关闭显示捡取线，捡取浮动点");
		m_Graph.Cursors.Item(1).CrosshairStyle = CNiCursor::CrosshairNone;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：显示捡取线，捡取浮动点");
		m_Graph.Cursors.RemoveAll();
		m_Graph.Cursors.Add();
		m_Graph.SetTrackMode(CNiGraph::TrackDragCursor);

		m_Graph.Cursors.Item(1).SnapMode = CNiCursor::SnapFloating;
		m_Graph.Cursors.Item(1).SetColor(RGB(0,255,0));
		double xValue = (m_Graph.Axes.Item("XAxis").GetMaximum() + m_Graph.Axes.Item("XAxis").GetMinimum()) / 2;
		double yValue = (m_Graph.Axes.Item("YAxis-1").GetMaximum() + m_Graph.Axes.Item("YAxis-1").GetMinimum()) / 2;

		m_Graph.Cursors.Item(1).SetPosition(xValue, yValue);
		if (currorNVisual%2!=0)
		{
			currorNVisual++;
		}
	}

}
//消息：撤销平移或缩放操作
void CNiWaveFraqView::OnUnzoom()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：撤销平移或缩放操作");
	//isUnZoom=TRUE;
	GetQuaternion();
	SetWaveParam();
	FFTProc();
	

/*
	CString strTemp;
	strTemp.Format("002:%lf",m_Graph.Axes.Item("XAxis").GetMaximum()*m_deltaFreq);
	AfxMessageBox(strTemp);
*/
	TextData();    //20120110
}

//消息：标注绘图区域最大最小值
void CNiWaveFraqView::OnAnnotations()
{

	CString strTemp;
if (isThrowload)
{
  
   CNiColor blue(40,104,197);
   double m_1,m_2;
   double m_baseX,m_baseY,m_maxminX,m_maxminY;
   
   m_1=m_Graph.Axes.Item("XAxis").GetMinimum();
   m_2=m_Graph.Axes.Item("XAxis").GetMaximum();
   



  
   isFullPeriodFFT=true;
   
    m_throw_start=m_1;
	m_throw_end=m_2;
/*	if (m_1<m_width)
		
	{
		m_1=m_width*m_deltaFreq;
		m_Graph.Axes.Item("XAxis").SetMinMax(m_1,m_2);

	}
*/

	CThrowLoadAna dlg;
	dlg.m_baseTime=m_1;
	dlg.m_maxTime=m_2;
	double maxTemp;
	int temIndex=0;
	BOOL m_dlgisGraph;
	//dlg.m_graphChk.SetCheck(true);
	if(dlg.DoModal()==IDCANCEL)
	{

		 if ((!(dlg.m_baseTime<m_1))&&(!(dlg.m_maxTime>m_2))&&dlg.m_baseTime<dlg.m_maxTime&&(!(dlg.m_baseTime<m_width*m_deltaFreq)))
		 {

			// m_Graph.Axes.Item("XAxis").SetMinMax(dlg.m_baseTime,dlg.m_maxTime);

			 m_Graph.Axes.Item("XAxis").SetMinMax(dlg.m_baseTime,dlg.m_maxTime);
			 SetQuaternion();
			 FFTProc();
			 TextData();
			 OnWavegraph();

			 m_baseTime=dlg.m_baseTime;
			 m_baseX=m_baseTime;

			 int x_1,x_2;
			 x_1=floor(dlg.m_maxTime/m_deltaFreq);
			 x_2=ceil(dlg.m_baseTime/m_deltaFreq);
	
			 m_baseY=m_tempTrendResult[x_2-m_width];

			 maxTemp=m_tempTrendResult[x_2-m_width];
			 m_dlgisGraph=dlg.isGraph;

	    if (dlg.isGraph)   
		{
			for (int i=x_2;i<x_1;i++)
			{
				if(m_tempTrendResult[i-m_width]>maxTemp)
				{
					maxTemp=m_tempTrendResult[i-m_width];
					temIndex=i-m_width;
				}
			}
		}
		else
		{
			for (int i=x_2;i<x_1;i++)  //cxy  
		{
			if(m_tempTrendResult[i-m_width]<maxTemp)
			{
				maxTemp=m_tempTrendResult[i-m_width];
				temIndex=i-m_width;
			}
		}

		}

		double x=temIndex*m_deltaFreq;
		double y=maxTemp;
		CString strAntName;

		
		


  		m_maxminX=temIndex*m_deltaFreq;   
		m_maxminY=maxTemp;
		
/*
		strTemp.Format("001:%lf",m_maxminX);
		AfxMessageBox(strTemp);
		strTemp.Format("002:%lf",m_maxminY);
		AfxMessageBox(strTemp);
		strTemp.Format("003:%lf",m_baseTime);
		AfxMessageBox(strTemp);
		strTemp.Format("004:%lf",m_baseY);
		AfxMessageBox(strTemp);
*/

       
		double m_tem=0;
		if (m_baseY!=0)
			m_tem=100.0*abs((m_maxminY-m_baseY)/m_baseY);
		CString str1,str2;
		str1.Format("%5.2f",float(m_maxminY));
		str2.Format("%5.2f",float(m_tem));

		if (m_dlgisGraph)
		{
			strAntName = _T("最大值: ")+str1+_T("上升率: ")+str2+_T("%");
		}
		else
		{
			strAntName = _T("最小值: ")+str1+_T("下降率: ")+str2+_T("%");
		}



		CString m_strAntName;
		str1.Format("%5.2f",float(m_baseY));
		m_strAntName = _T("基准值: ")+str1;


		//m_Graph.Annotations.Item(1).Caption.Text = _T("Max: ")+str;



		
			m_Graph.Cursors.RemoveAll();
			CNiCursor cursorTmp=m_Graph.Cursors.Add();
			m_Graph.SetTrackMode(CNiGraph::TrackDragCursor);
			cursorTmp.SetColor(RGB(0,255,0));
			//m_Graph.Cursors.Item(1).SnapMode = CNiCursor::SnapNearestPoint;
     		//cursorTmp.SnapMode = CNiCursor::SnapNearestPoint;
			//double xValue = (m_Graph.Axes.Item("XAxis").GetMaximum() + m_Graph.Axes.Item("XAxis").GetMinimum()) / 2;
			//double yValue = (m_Graph.Axes.Item("YAxis-1").GetMaximum() + m_Graph.Axes.Item("YAxis-1").GetMinimum()) / 2;
			//m_Graph.Cursors.Item(1).SetPosition(xValue, yValue);
			cursorTmp.SetPosition(m_maxminX, m_maxminY);


			 cursorTmp.XPosition=x;
			 cursorTmp.YPosition=y;

			CNiAnnotation annotation = m_Graph.Annotations.Add();
			CNiAnnotation m_annotation = m_Graph.Annotations.Add();



			annotation.SetBuiltinStyle(CNiAnnotation::TextAndArrow);
			annotation.Enabled  = true;
			annotation.Name = strAntName;

			m_annotation.SetBuiltinStyle(CNiAnnotation::TextAndArrow);
			m_annotation.Enabled  = true;
			m_annotation.Name = m_strAntName;


			m_Graph.GetPlots().Item("tempGrap111").PlotY(m_tempTrendResult, m_width*m_deltaFreq, m_increase*m_deltaFreq);
			m_Graph.Annotations.Item(1).Plot = m_Graph.Plots.Item("tempGrap111");
			m_Graph.Annotations.Item(2).Plot = m_Graph.Plots.Item("tempGrap111");

			CNiPlot plot = m_Graph.Plots.Item("tempGrap111");
			annotation.SnapMode = CNiAnnotation::SnapPointsOnPlot;
			annotation.Plot = plot;

			m_annotation.SnapMode = CNiAnnotation::SnapPointsOnPlot;
			m_annotation.Plot = plot;

			CNiColor clr(0, 0, 255);
			annotation.PointIndex = temIndex;//cursorTmp.PointIndex;

			m_annotation.PointIndex =  int(m_baseX/m_deltaFreq)-m_width;//cursorTmp.PointIndex;
			

			annotation.SnapMode = CNiAnnotation::SnapAnchoredToPoint;

			annotation.Visible = true;
			annotation.CoordinateType = CNiAnnotation::AxesCoordinates ;
			CNiArrow arrow = annotation.Arrow;
			arrow.HeadStyle = CNiArrow::Stealth;
			arrow.LineStyle = CNiArrow::LineSolid;
			arrow.Width = 1;
			arrow.Color =RGB(0, 0, 255);
			arrow.Visible = true;
			CNiCaption caption = annotation.Caption;
			caption.Font.Size = m_fontSize;
			caption.Text = strAntName;
			caption.Color = RGB(0, 0, 255);
			caption.XCoordinate = x;
			caption.YCoordinate = y;
			CNiShape shape = annotation.Shape;
			shape.Color = CNiColor(255, 0, 0);
			shape.PointStyle = CNiShape::PointAsterisk ;
			shape.FillVisible = true;
			shape.LineWidth = 2;


			m_annotation.SnapMode = CNiAnnotation::SnapAnchoredToPoint;

			m_annotation.Visible = true;
			m_annotation.CoordinateType = CNiAnnotation::AxesCoordinates ;
			CNiArrow m_arrow = m_annotation.Arrow;
			m_arrow.HeadStyle = CNiArrow::Stealth;
			m_arrow.LineStyle = CNiArrow::LineSolid;
			m_arrow.Width = 1;
			m_arrow.Color =RGB(0, 0, 255);
			m_arrow.Visible = true;
			CNiCaption m_caption = m_annotation.Caption;
			m_caption.Font.Size = m_fontSize;
			m_caption.Text = m_strAntName;
			m_caption.Color = RGB(0, 0, 255);
			m_caption.XCoordinate = x;
			m_caption.YCoordinate = y;
			CNiShape m_shape = m_annotation.Shape;
			m_shape.Color = CNiColor(255, 0, 0);
			m_shape.PointStyle = CNiShape::PointAsterisk ;
			m_shape.FillVisible = true;
			m_shape.LineWidth = 2;

			
			OnCurrornearestpoint();
			OnCurrornearestpoint();
			m_Graph.TrackMode = CNiGraph::TrackZoomRectXWithUndo;




		}

		 else
		 {
			 //m_width*m_deltaFreq
			 double intTemp=m_width*m_deltaFreq;
			 CString strTemp,str;
			 strTemp.Format("%lf",intTemp);
			 str="基准时刻应大于"+strTemp;

			 intTemp=dlg.m_maxTime;
             strTemp.Format("%lf",intTemp);
			 str=str+";"+"最终时间应小于或等于"+strTemp;
			 AfxMessageBox(str);
			 OnAnnotations();
		 }

		 }

		
}
else
{
	AfxMessageBox("请先设置甩负荷趋势线分析窗口!");
	OnWindowSet();
}
 



/*
	Annotations();
	annotationsVisual++;
	if (annotationsVisual%2==0)
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：标注绘图区域的最大最小值");
		m_Graph.Annotations.Item(1).Visible = FALSE;
		m_Graph.Annotations.Item(2).Visible = FALSE;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：取消标注绘图区域的最大最小值");
		m_Graph.Annotations.Item(1).Visible = TRUE;
		m_Graph.Annotations.Item(2).Visible = TRUE;
	}
*/

}
//消息：重绘波形图
void CNiWaveFraqView::Onresume()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：重新绘制全图");
	Resume();
}
//重新绘制波形图
void CNiWaveFraqView::Resume()
{
	double minValue, maxValue, valueDecl;
	unsigned int minIndex, maxIndex;
	NiData.MinMax(minValue, minIndex, maxValue, maxIndex);
	valueDecl=(maxValue-minValue)*0.1;
	qVector.clear();

	/*m_Graph.GetPlots().RemoveAll();*/
	CNiColor blue(40,104,197);
	/*m_Graph.GetPlots().Add();*/
	if (col!=NULL)
		m_Graph.GetPlots().Item(1).LineColor=col;	
	else
		m_Graph.GetPlots().Item(1).LineColor=blue;			
	m_Graph.GetPlots().Item(1).PlotY(NiData, 0, m_deltaFreq);
	m_Graph.Axes.Item("XAxis").SetMinMax(0,xRange*m_deltaFreq);
	minValue=minValue-valueDecl;
	maxValue=maxValue+valueDecl;

	if (waveHeightMax==0&&waveHeightMin==0)
	{
		m_Graph.Axes.Item("YAxis-1").SetMinMax(minValue,maxValue);	
	}
	else
	{
		maxValue=maxValue>waveHeightMax?minValue:waveHeightMax;
		minValue=minValue<waveHeightMin?minValue:waveHeightMin;
		m_Graph.Axes.Item("YAxis-1").SetMinMax(minValue,maxValue);	
	}

	SetQuaternion();	
	SetWaveParam();
	FFTProc();
	//Annotations();
/*
	CString strTemp;
	strTemp.Format("003:%lf",m_Graph.Axes.Item("XAxis").GetMaximum()*m_deltaFreq);
	AfxMessageBox(strTemp);
*/
	TextData();    ////20120110     cxy  加的
}
//消息：是否显示健相
void CNiWaveFraqView::OnKeyphasevisual()
{
	//if (isFullPeriodFFT==FALSE)//整周期时，不能显示隐藏键相信号，去掉这个限制//Marshall  2011.2.25 13:9
	//{
	//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：整周期FFT运算时间，无法关闭健相点显示");
	//	return;
	//}

	/*--------------------------Marshall's Code2011.2.25 13:21--------------*/
	//显示隐藏键相信号//Marshall  2011.2.25 13:25
	isKeyPhaseVisual = TRUE;
	int nCnt = m_Graph.GetPlots().GetCount();
	if(nCnt <= 0)
		return;
	CString strKPName = _T("kp");
	for(int i = 0; i < nCnt; i++)
	{
		CString strTmp = m_Graph.Plots.Item(i+1).Name;
		if(strTmp == strKPName)
		{
			isKeyPhaseVisual = FALSE;
			
			break;
		}
	}
	/*--------------------------Marshall's Code2011.2.25 13:21--------------*/
	if (menuShow && isHasKeyPhase)
	{
		if (isKeyPhaseVisual==TRUE)
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：显示健相点");
			CNiReal64Vector kpX((int)kPhaseX.size());
			CNiReal64Vector kpY((int)kPhaseY.size());
			for (int k=0;k<(int)kPhaseX.size();k++)
			{
				kpX[k]=kPhaseX[k]*m_deltaFreq;
				kpY[k]=kPhaseY[k];
			}
			m_Graph.GetPlots().Add().Name="kp";

			m_Graph.GetPlots().Item("kp").PointColor=RGB(255,0,0);
			m_Graph.GetPlots().Item("kp").LineStyle=CNiPlot::LineNone;
			m_Graph.GetPlots().Item("kp").PointStyle=CNiPlot::PointEmptyCircle;
			m_Graph.GetPlots().Item("kp").LineWidth=2;
			m_Graph.GetPlots().Item("kp").PlotXvsY(kpX,kpY);

			isKeyPhaseVisual=FALSE;
		}
		else
		{

			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：关闭健相点的显示");
			//m_Graph.GetPlots().Remove("kp");////Marshall  2011.2.25 13:33
			/*--------------------------Marshall's Code2011.2.25 13:28--------------*/
			if(!m_Graph.GetPlots().Item(i+1).Visible)
				m_Graph.GetPlots().Item(i+1).Visible = true;
			else
				m_Graph.GetPlots().Item(i+1).Visible = false;
			/*--------------------------Marshall's Code2011.2.25 13:28--------------*/
			isKeyPhaseVisual=TRUE;
		}
	}
	else
	{
		AfxMessageBox("外部加载数据无健相信息！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：外部加载数据无健相信息！");
	}
}


//消息：是否显示健相 开关
void CNiWaveFraqView::OnUpdateKeyphasevisual(CCmdUI *pCmdUI)
{

	/*--------------------------Marshall's Code2011.2.25 13:38--------------*/
	isKeyPhaseVisual = FALSE;
	int nCnt = m_Graph.GetPlots().GetCount();
	if(nCnt > 0)
	{
		CString strKPName = _T("kp");
		for(int i = 0; i < nCnt; i++)
		{
			CString strTmp = m_Graph.Plots.Item(i+1).Name;
			if(strTmp == strKPName)
			{
				isKeyPhaseVisual =  m_Graph.Plots.Item(i+1).Visible;
				break;
			}
		}
	}
	/*--------------------------Marshall's Code2011.2.25 13:38--------------*/
	//pCmdUI->SetCheck(!isKeyPhaseVisual);////Marshall  2011.2.25 13:40
	pCmdUI->SetCheck(isKeyPhaseVisual);
}

//整周期处理自己的消息//Marshall  2011.2.25 16:54
LPARAM CNiWaveFraqView::OnFullPR(WPARAM wParam,LPARAM lParam)
{
	int bShowAlarm = (int)wParam;
	if(bShowAlarm > 0)
		OnFullperiodfft();
	else
		FullPR();////Marshall  2011.2.26 16:32
		//OnFullperiodfft();



	return 1;
}

//OnFullperiodfft函数的重写，添加了是否显示警告的标识//Marshall  2011.2.25 17:1
void CNiWaveFraqView::FullPR()
{
	if (menuShow&&isHasKeyPhase)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：打开依据健相点完成整周期FFT运算");

		if (isFullPeriodFFT==TRUE)
		{	
			isFullPeriodFFT=FALSE;
		}
		else
		{	
			if (isKeyPhaseVisual==TRUE)
			{
				isKeyPhaseVisual=FALSE;
			}
			else
			{
				isKeyPhaseVisual=TRUE;
			}
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：关闭依据健相点完成整周期FFT运算");

			isFullPeriodFFT=TRUE;
		}
		FFTProc();

		TextData();    //cxy  加

		CString strTemp;
		strTemp.Format("004:%lf",m_Graph.Axes.Item("XAxis").GetMaximum()*m_deltaFreq);
		//AfxMessageBox(strTemp);

	
	}
	else
	{
		//此处添加是否弹出提示的判断//Marshall  2011.2.25 17:2
		//AfxMessageBox(_T("外部加载数据无健相信息！"));
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：外部加载数据无健相信息！");
	}

}
//消息：打开/关闭依据健相点完成整周期FFT运算
void CNiWaveFraqView::OnFullperiodfft()
{
	if (menuShow&&isHasKeyPhase)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：打开依据健相点完成整周期FFT运算");

		if (isFullPeriodFFT==TRUE)
		{	
			////整周期默认不显示键相信号//Marshall  2011.2.25 13:13
			//////实现键相点绘制//Marshall  2011.2.25 9:36
			//CNiReal64Vector kpX((int)kPhaseX.size());
			//CNiReal64Vector kpY((int)kPhaseY.size());
			//for (int k=0;k<(int)kPhaseX.size();k++)
			//{
			//	kpX[k]=kPhaseX[k];
			//	kpY[k]=kPhaseY[k];
			//}
			//m_Graph.GetPlots().Add().Name="kp";

			////默认不显示键相//Marshall  2011.2.25 13:15
			//m_Graph.GetPlots().Item("kp").Visible = false;

			//m_Graph.GetPlots().Item("kp").PointColor=RGB(255,0,0);
			//m_Graph.GetPlots().Item("kp").LineStyle=CNiPlot::LineNone;
			//m_Graph.GetPlots().Item("kp").PointStyle=CNiPlot::PointEmptyCircle;
			//m_Graph.GetPlots().Item("kp").LineWidth=2;
			//m_Graph.GetPlots().Item("kp").PlotXvsY(kpX,kpY);
			//isKeyPhaseVisual=TRUE;
			isFullPeriodFFT=FALSE;
		}
		else
		{	
			if (isKeyPhaseVisual==TRUE)
			{
				isKeyPhaseVisual=FALSE;
			}
			else
			{
				isKeyPhaseVisual=TRUE;
			}
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：关闭依据健相点完成整周期FFT运算");

			isFullPeriodFFT=TRUE;
		}
		FFTProc();
/*
		CString strTemp;
		strTemp.Format("005:%lf",m_Graph.Axes.Item("XAxis").GetMaximum()*m_deltaFreq);
		AfxMessageBox(strTemp);
*/		//TextData();
	}
	else
	{
		AfxMessageBox(_T("外部加载数据无健相信息！"));
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：外部加载数据无健相信息！");
	}
}


//消息：打开/关闭依据健相点完成整周期FFT运算 开关
void CNiWaveFraqView::OnUpdateFullperiodfft(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!isFullPeriodFFT);	
}
//精确计算FFT
CNiReal64Vector CNiWaveFraqView::re_time (CNiReal64Vector p1)
{
	//不处理
/*
	int n1, n2;
	double dt1,T;


	n1=p1.GetSize();
	n2=pow(2.0,(int)(log((double)n1)/log(2.0)));
	CNiReal64Vector p2(n2);	
	dt1=1.0/(double)samplingFreq;
	T=(double)p1.GetSize()/(double)samplingFreq;

	int jj;
	double t,dt2;

	dt2=T/(double)n2;
	jj=0;
	for (int i=0;i<n2;i++) 
	{
		t=dt2*(double)i;
		for (int j=jj;j<n1;j++) 
		{
			if (t>=dt1*j && t<=dt1*j+dt1) 
			{
				p2[i]=p1[j]+(p1[j+1]-p1[j])/dt1*(t-dt1*(double)j);
				jj=j;
				break;
			}
		}
	}
	return p2;
*/
	return p1;
}
//消息：显示波形频谱图
void CNiWaveFraqView::OnWavefraqgrap()
{
	if ((int)ChnFftFlag==1)
	{
		if (isWaveFreq)
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形频谱操作：同时显示波形频谱图...");
			
			windowStyle=0;
			CRect   rect   ;   
			this->GetWindowRect(   rect   );   
			ScreenToClient(   rect   )   ;   
			this->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());   
			
			//m_Graph.SetWindowText(title);////Marshall  2011.2.21 14:50
			m_ctrlTitle.SetWindowText(title);
		//	m_strWaveTitle = title;//标题设置//Marshall  2011.2.28 15:27

			m_newGraph.SetWindowText("");
			isWavegraph=TRUE;
			isWaveFreq=FALSE;
			isFreqgraph=TRUE;

			//UpdateData(false);

		} 
		else
		{
			m_newGraph.SetWindowText("");

			isWavegraph=TRUE;
			isWaveFreq=TRUE;
			isFreqgraph=TRUE;
		}
		
		//::SendMessage(this->GetSafeHwnd(),   WM_SIZE,   SIZE_RESTORED,MAKEWPARAM(rect.Width(),   rect.Height()));  
	}
	else
	{
		AfxMessageBox("无效的FFT运算，窗体变换无效！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形频谱操作：无效的FFT运算，窗体变换无效！");

		return;
	}
}

//消息：仅显示波形图
void CNiWaveFraqView::OnWavegraph()
{
	if ((int)ChnFftFlag==1)
	{
		if (isWavegraph)
		{
			
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形频谱操作：仅显示波形图...");
			windowStyle=1;
			CRect   rect   ;   
			this->GetWindowRect(   rect   );   
			ScreenToClient(   rect   )   ;   
			this->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());		
			CString waveTitle=title;
			waveTitle.Replace("频谱","");
			
			//m_Graph.SetWindowText(waveTitle);////Marshall  2011.2.21 14:48
			m_ctrlTitle.SetWindowText(waveTitle);
			//m_strWaveTitle = waveTitle;//标题设置//Marshall  2011.2.28 15:27

			isWavegraph=FALSE;
			isWaveFreq=TRUE;
			isFreqgraph=TRUE;

			//UpdateData(false);
		} 
		else
		{
			m_newGraph.SetWindowText("");

			isWavegraph=TRUE;
			isWaveFreq=TRUE;
			isFreqgraph=TRUE;
		}
		//::SendMessage(this->GetSafeHwnd(),   WM_SIZE,   SIZE_RESTORED,MAKEWPARAM(rect.Width(),   rect.Height()));  
	}
	else
	{
		AfxMessageBox("无效的FFT运算，窗体变换无效！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形频谱操作：无效的FFT运算，窗体变换无效！");

		return;
	}
}

//消息：仅显示频谱图
void CNiWaveFraqView::OnFraqgraph()
{
	if ((int)ChnFftFlag==1)
	{
		if (isFreqgraph)
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形频谱操作：仅显示频谱图...");
			windowStyle=2;
			CRect   rect   ;   
			this->GetWindowRect(   rect   );   
			ScreenToClient(   rect   )   ;   
			this->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());   
			CString freqTitle=title;
			freqTitle.Replace("波形","");

			/*	freqTitle.Replace("(单位：","");
			freqTitle.Replace("","");*///标题调整//Marshall  2011.2.21 15:27

			/*--------------------------Marshall's Code2011.2.21 15:29--------------*/
			int nPos = freqTitle.ReverseFind('(');
			//int nLen = freqTitle.GetLength();
			CString strTmp = freqTitle.Left(nPos);
			freqTitle = strTmp;
			/*--------------------------Marshall's Code2011.2.21 15:29--------------*/


			//freqTitle.Replace("波形","");
			//freqTitle.Right(freqTitle.Find('(',0));////Marshall  2011.2.21 15:29
			//freqTitle.Replace("(","");
			//int p=freqTitle.Find("图",0);
			//freqTitle.Left(freqTitle.Find('(',0));

			//m_newGraph.SetWindowText(freqTitle);////Marshall  2011.2.21 14:49
			m_ctrlTitle.SetWindowText(freqTitle);
			//m_strWaveTitle = freqTitle;//标题设置//Marshall  2011.2.28 15:27


			isWavegraph=TRUE;
			isWaveFreq=TRUE;
			isFreqgraph=FALSE;

			//UpdateData(false);
		} 
		else
		{
			m_Graph.SetWindowText("");
			isWavegraph=TRUE;
			isWaveFreq=TRUE;
			isFreqgraph=TRUE;
		}

	}
	else
	{		
		AfxMessageBox("无效的FFT运算，窗体变换无效！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形频谱操作：无效的FFT运算，窗体变换无效！");
		return;
	}
	//::SendMessage(this->GetSafeHwnd(),   WM_SIZE,   SIZE_RESTORED,MAKEWPARAM(rect.Width(),   rect.Height()));  

}




//消息：添加通道数据绘制波形
void CNiWaveFraqView::OnAddchndata()
{
	if (menuShow)
	{
		CAddChnData acd(this);
		acd.SetChnData(FileStr,ExamNameStr,TypeStr,ParamStr,chnStr);
		if(acd.DoModal()==IDOK)
		{
			//((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：添加通道数据绘制波形...");
			//-------------------1.0.0.2------------------------//
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：添加通道数据：    "+acd.selChnStr+"    绘制波形...");
			//-------------------------------------------------//
		}
		else
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：取消添加通道数据绘制波形");
		}
	}
	else
	{
		AfxMessageBox("波形信号运算：外部加载数据不支持本操作！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：外部加载数据不支持本操作！");
		return;
	}
}


//消息：添加移动趋势
void CNiWaveFraqView::OnAddmobtrend()
{
	CAddMobTrend adt(this);
	if(adt.DoModal()==IDOK)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：添加移动趋势...");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：取消添加移动趋势");
	}
}

//消息：波形滤波处理
void CNiWaveFraqView::OnWaveFilter()
{

	//CNiReal64Vector vFilter(NiData.GetSize());

	int m_flag=NiData.GetSize();

	if (m_flag>10240)
		m_flag=3072;
	else
		m_flag=int(NiData.GetSize()/10);


	CNiReal64Vector tempNiData(NiData.GetSize()+m_flag);

	CNiReal64Vector vFilter(NiData.GetSize()+m_flag);


	for (int i=0;i<m_flag;i++)
		tempNiData[i]=NiData[i];

	for (int i=m_flag;i<m_flag+NiData.GetSize();i++)
		tempNiData[i]=NiData[i-m_flag];





	//isWaveFilter=TRUE;

	CWaveFilter wf;
	wf.m_Freq=samplingFreq;
	
	if(wf.DoModal()==IDOK)
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：波形滤波处理...");

		double low=wf.m_FreqLowCut;
		double high=wf.m_FreqHighCut;
		int order=wf.m_order;
		switch(wf.m_FilterType)
		{
		case 0:
			wf.m_FreqLowCut;
			//CNiMath::ButterworthLowPass(NiData, vFilter, samplingFreq, low, order);
			CNiMath::ButterworthLowPass(tempNiData, vFilter, samplingFreq, low, order);
			//NiData=vFilter;
			for (int i=m_flag;i<m_flag+NiData.GetSize();i++)
				NiData[i-m_flag]=vFilter[i];

			break;
		case 1:
			//CNiMath::ButterworthHighPass(NiData, vFilter, samplingFreq, high, order);
			CNiMath::ButterworthHighPass(tempNiData, vFilter, samplingFreq, high, order);
			//NiData=vFilter;
			for (int i=m_flag;i<m_flag+NiData.GetSize();i++)
				NiData[i-m_flag]=vFilter[i];
			break;
		case 2:
			//CNiMath::ButterworthBandPass(NiData, vFilter, samplingFreq, low, high, order);
			CNiMath::ButterworthBandPass(tempNiData, vFilter, samplingFreq, low, high, order);
			//NiData=vFilter;
			for (int i=m_flag;i<m_flag+NiData.GetSize();i++)
				NiData[i-m_flag]=vFilter[i];
			break;
		case 3:
			//CNiMath::ButterworthBandStop(NiData, vFilter, samplingFreq, low, high, order);
			CNiMath::ButterworthBandStop(tempNiData, vFilter, samplingFreq, low, high, order);
			for (int i=m_flag;i<m_flag+NiData.GetSize();i++)
				NiData[i-m_flag]=vFilter[i];
			//NiData=vFilter;
			break;
		default:
			break;
		}
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：取消波形滤波处理");
	}
	Resume();
}



//消息：恢复滤波前的数据
void CNiWaveFraqView::OnResumeFilter()
{
	
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形信号运算：恢复滤波前的数据");
	NiData=ResumeData;
	Resume();
}

//波形图波形缩放
void CNiWaveFraqView::ZoomCwgraph2()
{
	if (m_Graph.TrackMode==CNiGraph::TrackZoomRectXWithUndo)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：X轴向数据放大");
	}
	if (m_Graph.TrackMode==CNiGraph::TrackZoomRectYWithUndo)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：Y轴数据放大");
	}
	if (m_Graph.TrackMode==CNiGraph::TrackZoomRectXYWithUndo)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("波形图操作：XY轴向数据窗口式放大");
	}

	if (annotationsVisual%2!=0)	
	{
		annotationsVisual++;
		m_Graph.Annotations.Item(1).Visible = FALSE;
		m_Graph.Annotations.Item(2).Visible = FALSE;
	}

	//double x =m_Graph.Cursors.Item(1).GetXPosition();
	if (GetXRang()>32*m_deltaFreq && GetYRang()>0.001)
	{
		//if (((int)(m_Graph.Axes.Item("XAxis").GetMaximum())-(int)(m_Graph.Axes.Item("XAxis").GetMinimum()))>32)
		{
			SetQuaternion();
			SetWaveParam();
			FFTProc();
		}
	}
	else
	{
		if(!isFullPeriodFFT)
		{
			FFTProc();
		}
		else
		{
			Quaternion q=qVector.back();
			m_Graph.Axes.Item("XAxis").SetMinimum(q.xMin);
			m_Graph.Axes.Item("XAxis").SetMaximum(q.xMax);	
			m_Graph.Axes.Item("YAxis-1").SetMinimum(q.yMin);
			m_Graph.Axes.Item("YAxis-1").SetMaximum(q.yMax);
		}
	}
	if (GetXRang()<5*m_deltaFreq)
	{
		AfxMessageBox("请重新选择区间!");
	}
/*
	CString strTemp;
	strTemp.Format("006:%lf",m_Graph.Axes.Item("XAxis").GetMaximum()*m_deltaFreq);
	AfxMessageBox(strTemp);
*/
	//Annotations();
	//Invalidate();////Marshall  2011.2.21 16:14
}



//消息：显示捡取线，捡取临近点
void CNiWaveFraqView::OnCurrornearestpointf()
{
	currorNVisualF++;

	if (currorNVisualF%2==0)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：关闭捡取线，捡取临近点");
		m_newGraph.Cursors.RemoveAll();
		/*m_newGraph.Cursors.Add();
		m_Graph.Cursors.Item(1).CrosshairStyle = CNiCursor::CrosshairNone;*/
	}
	else
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：显示捡取线，捡取临近点");

		m_newGraph.Cursors.RemoveAll();
		m_newGraph.Cursors.Add();
		m_newGraph.SetTrackMode(CNiGraph::TrackDragCursor);
		m_newGraph.Cursors.Item(1).SetColor(RGB(0,255,0));
		m_newGraph.Cursors.Item(1).SnapMode = CNiCursor::SnapNearestPoint;	
		double xValue = (m_newGraph.Axes.Item("XAxis").GetMaximum() + m_newGraph.Axes.Item("XAxis").GetMinimum()) / 2;
		double yValue = (m_newGraph.Axes.Item("YAxis-1").GetMaximum() + m_newGraph.Axes.Item("YAxis-1").GetMinimum()) / 2;

		m_newGraph.Cursors.Item(1).SetPosition(xValue, yValue);
	}
}

//消息：X轴向平移绘图区域视窗
void CNiWaveFraqView::OnXmovingf()
{	
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：X轴向平移绘图区域视窗...");
	SetQuaternionF();
	m_newGraph.SetTrackMode(CNiGraph::TrackPanPlotAreaX);
}

//消息：X轴向数据放大
void CNiWaveFraqView::OnXzoomf()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：X轴向数据放大...");
	m_newGraph.SetTrackMode(CNiGraph::TrackZoomRectXWithUndo);
}

//消息：XY轴数据窗口式放大
void CNiWaveFraqView::OnZoomf()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：XY轴数据窗口式放大...");
	m_newGraph.SetTrackMode(CNiGraph::TrackZoomRectXYWithUndo);
}
//消息：撤销平移或缩放操作
void CNiWaveFraqView::OnUnzoomf()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：撤销平移或缩放操作");
	//isUnZoomF=TRUE;
	GetQuaternionF();
}
//消息：重新绘制全图
void CNiWaveFraqView::Onresumef()
{
	//if (!isUnZoomF)	
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：重新绘制全图");
	FFTProc();
	SetQuaternionF();
	//isUnZoomF=FALSE;
/*
	CString strTemp;
	strTemp.Format("007:%lf",m_Graph.Axes.Item("XAxis").GetMaximum()*m_deltaFreq);
	AfxMessageBox(strTemp);
*/
}

//显示当前光标线所捕捉的坐标
void CNiWaveFraqView::CursorChangeCwgraph1(long* CursorIndex, VARIANT* XPos, VARIANT* YPos, BOOL* bTracking)
{
	CWnd* pWnd=GetDlgItem(IDC_CWGRAPH2);

	CDC* pDC=pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();	

	//创造画笔对象
	CPen* pPenBlue=new CPen;
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0,255));

	pDC->SelectObject(pPenBlue);

	std::stringstream   xxx; 
	xxx <<"当前位置：("<<m_newGraph.Cursors.Item(1).GetXPosition()<<","<<m_newGraph.Cursors.Item(1).GetYPosition()<<")"; 

	std::string str;xxx>> str; 
	pDC->SetTextColor(RGB(0,0,255));
	/*pDC->TextOut(10,2,str.c_str());*/
	pDC->TextOut(10,2,str.c_str());////Marshall  2011.2.21 22:24

	delete pPenBlue;

}


//获取绘图边界
void CNiWaveFraqView::GetQuaternionF()
{
	if (qVectorF.size()>1)
	{
		qVectorF.pop_back();
		Quaternion q=qVectorF.back();
		m_newGraph.Axes.Item("XAxis").SetMinimum(q.xMin);
		m_newGraph.Axes.Item("XAxis").SetMaximum(q.xMax);	
		m_newGraph.Axes.Item("YAxis-1").SetMinimum(q.yMin);
		m_newGraph.Axes.Item("YAxis-1").SetMaximum(q.yMax);

	}
	else
	{
		FFTProc();
		SetQuaternionF();
	}
}
//设置绘图边界
void CNiWaveFraqView::SetQuaternionF()
{
	Quaternion q;	

	q.xMin=m_newGraph.Axes.Item("XAxis").GetMinimum();
	q.xMax=m_newGraph.Axes.Item("XAxis").GetMaximum();
	q.yMin=m_newGraph.Axes.Item("YAxis-1").GetMinimum();
	q.yMax=m_newGraph.Axes.Item("YAxis-1").GetMaximum();

	qVectorF.push_back(q);
}


//频谱图波形缩放
void CNiWaveFraqView::ZoomCwgraph1()
{
	if (m_newGraph.TrackMode==CNiGraph::TrackZoomRectXWithUndo)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：X轴向数据放大");
	}
	if (m_newGraph.TrackMode==CNiGraph::TrackZoomRectXYWithUndo)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图操作：XY轴数据窗口式放大");
	}

	if (m_newGraph.Axes.Item("XAxis").GetMaximum()-m_newGraph.Axes.Item("XAxis").GetMinimum()>5.0*m_deltaFreq)
	{
		SetQuaternionF();
	}
	else
	{
		Quaternion q=qVectorF.back();
		m_newGraph.Axes.Item("XAxis").SetMinimum(q.xMin);
		m_newGraph.Axes.Item("XAxis").SetMaximum(q.xMax);	
		m_newGraph.Axes.Item("YAxis-1").SetMinimum(q.yMin);
		m_newGraph.Axes.Item("YAxis-1").SetMaximum(q.yMax);
	}

}
//消息：保存整个视图
void CNiWaveFraqView::OnSave()
{
	SaveAllView();
}
//消息：自定义保存全部视图
void CNiWaveFraqView::OnCustomsave()
{	
	CustomSaveAllView();	
}
//消息：保存单个波形图
void CNiWaveFraqView::OnSavesingleview()
{
	SaveSigleView();
}
//消息：设置并修改标题
void CNiWaveFraqView::OnSettitle()
{
	// TODO: 在此添加命令处理程序代码
	CSetTitle setTitleDlg;

	//if (setTitleDlg.DoModal()==IDOK)//注释掉//Marshall  2011.2.21 12:15
	//{
	//	m_Graph.SetWindowText(setTitleDlg.mTitle);	
	//}

	/*--------------------------Marshall's Code2011.2.21 12:17--------------*/
	setTitleDlg.mTitle = title;

	if (setTitleDlg.DoModal()==IDOK)
	{
		m_ctrlTitle.SetWindowText(setTitleDlg.mTitle);
		//m_strWaveTitle = setTitleDlg.mTitle;//标题设置//Marshall  2011.2.28 15:27
		//UpdateData(false);

	}
	/*--------------------------Marshall's Code2011.2.21 12:17--------------*/
	
}
//设置标题
void CNiWaveFraqView::SetTitle(CString str)
{
	title=str;	
}
//消息：菜单处理
void CNiWaveFraqView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	
	CMenu menu;

	VERIFY(menu.LoadMenu (IDR_PopMENU));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	//int k=menu.EnableMenuItem(ID_AddChnData,   MF_BYCOMMAND   |   MF_DISABLED   |   MF_GRAYED);
	if (menuShow==FALSE)
	{
		CString str ;
		for(int i=menu.GetSubMenu (0)->GetMenuItemCount()-1;i>=0;i--) 
		{ 
			menu.GetSubMenu (0)->GetMenuString(i,str,MF_BYPOSITION); 
			if(str=="添加通道数据") 
			{ 

				menu.GetSubMenu (0)->DeleteMenu(i,MF_BYPOSITION);
				menu.GetSubMenu (0)->DeleteMenu(i-1,MF_BYPOSITION);  
				break; 
			}
		}
		for(int i=menu.GetSubMenu (0)->GetMenuItemCount()-1;i>=0;i--) 
		{ 
			menu.GetSubMenu (0)->GetMenuString(i,str,MF_BYPOSITION); 
			if(str=="显示健相") 
			{ 
				menu.GetSubMenu (0)->DeleteMenu(i,MF_BYPOSITION);
				break; 
			}
		}
		for(int i=menu.GetSubMenu (0)->GetMenuItemCount()-1;i>=0;i--) 
		{ 
			menu.GetSubMenu (0)->GetMenuString(i,str,MF_BYPOSITION); 
			if(str=="整周期FFT") 
			{ 

				menu.GetSubMenu (0)->DeleteMenu(i,MF_BYPOSITION);
				menu.GetSubMenu (0)->DeleteMenu(i-1,MF_BYPOSITION);  
				break; 
			}
		}
	}

	//(mainmenu->GetSubMenu (0))->AppendMenu (MF_SEPARATOR);//添加分隔符
	//(mainmenu->GetSubMenu (0))->AppendMenu(MF_STRING,ID_APP_ABOUT,_T("Always on &Top")); //添加新的菜单项
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x, point.y, AfxGetMainWnd());

	menu.Detach();
	
}

//消息：改变窗体大小
void CNiWaveFraqView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
	//CRect MyClientRect;
	//this->GetClientRect(MyClientRect);
	GraphShowType(windowStyle,cx, cy);
}
//设置波形频谱图显示模式//注释掉原来的函数//Marshall  2011.2.21 10:49
//void CNiWaveFraqView::GraphShowType(int type, int cx, int cy)
//{
//	CWnd *pWnd; 
//
//	switch(type)
//	{
//	case 0:
//		pWnd = GetDlgItem(IDC_CWGRAPH2);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(0,0,cx-200,cy/2);//设置控件大小
//		}	
//
//		pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(0,cy/2,cx-200,cy/2);//设置控件大小
//		}
//		pWnd = GetDlgItem(IDC_WAVE);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(cx-200,0,200,cy/2);//设置控件大小
//		}
//		pWnd = GetDlgItem(IDC_FRAQ);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(cx-200,cy/2,200,cy/2);//设置控件大小
//		}
//		break;
//	case 1:
//
//		pWnd = GetDlgItem(IDC_CWGRAPH2);     //获取控件句柄
//
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);		
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(0,0,cx-200,cy);//设置控件大小
//		}	
//
//
//		pWnd = GetDlgItem(IDC_WAVE);        //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);	
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(cx-200,0,200,cy);//设置控件大小
//		}
//
//		pWnd = GetDlgItem(IDC_CWGRAPH1);
//		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
//		pWnd = GetDlgItem(IDC_FRAQ); 
//		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
//		break;
//	case 2:
//		pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(0,0,cx-200,cy);//设置控件大小
//		}
//		
//		pWnd = GetDlgItem(IDC_FRAQ);      //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(cx-200,0,200,cy);//设置控件大小
//		}
//		pWnd = GetDlgItem(IDC_WAVE);
//		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
//		pWnd = GetDlgItem(IDC_CWGRAPH2); 
//		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
//		break;
//	default:
//		break;
//	}
//}



/*--------------------------Marshall's Code2011.2.21 10:50--------------*/
//void CNiWaveFraqView::GraphShowType(int type, int cx, int cy)
//{
//	CWnd *pWnd; 
//	int nTtlHeight = 20;
//	switch(type)
//	{
//	case 0:
//		pWnd = GetDlgItem(IDC_CWGRAPH2);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(0,nTtlHeight,cx-TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
//			//pWnd->MoveWindow(2,nTtlHeight,cx-200,(cy-nTtlHeight)/2);//设置控件大小
//		}	
//
//		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
//		pWnd = GetDlgItem(IDC_TITLE);
//		if(pWnd != NULL) pWnd->ShowWindow(TRUE);
//		if(pWnd)
//		{
//			pWnd->MoveWindow(0,2,cx,nTtlHeight-4);
//			//pWnd->MoveWindow(2,2,cx,nTtlHeight-4);
//		}
//		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
//
//		pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(0,(cy+nTtlHeight)/2,cx-TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
//		}
//		pWnd = GetDlgItem(IDC_WAVE);     //波形特征值
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(cx-200,nTtlHeight,TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
//		}
//		pWnd = GetDlgItem(IDC_FRAQ);     //频率主值
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(cx-200,(cy+nTtlHeight)/2,TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
//		}
//		break;
//	case 1:
//
//		pWnd = GetDlgItem(IDC_CWGRAPH2);     //获取控件句柄
//
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);		
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(0,nTtlHeight,cx-TEXT_WIDTH,cy-nTtlHeight);//设置控件大小
//		}	
//
//		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
//		pWnd = GetDlgItem(IDC_TITLE);
//		if(pWnd != NULL) pWnd->ShowWindow(TRUE);
//		if(pWnd)
//		{
//			pWnd->MoveWindow(0,2,cx,nTtlHeight-4);
//			//pWnd->MoveWindow(2,2,cx,nTtlHeight-4);
//		}
//		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
//
//		pWnd = GetDlgItem(IDC_WAVE);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(cx-200,nTtlHeight,TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
//		}
//
//		pWnd = GetDlgItem(IDC_CWGRAPH1);
//		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
//		pWnd = GetDlgItem(IDC_FRAQ); 
//		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
//		break;
//	case 2:
//		pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(0,nTtlHeight,cx-TEXT_WIDTH,cy-nTtlHeight);//设置控件大小
//		}
//		
//		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
//		pWnd = GetDlgItem(IDC_TITLE);
//		if(pWnd != NULL) pWnd->ShowWindow(TRUE);
//		if(pWnd)
//		{
//			pWnd->MoveWindow(0,2,cx,nTtlHeight-4);
//			//pWnd->MoveWindow(2,2,cx,nTtlHeight-4);
//		}
//		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
//
//
//		pWnd = GetDlgItem(IDC_FRAQ);      //获取控件句柄
//		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
//		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
//		{
//			pWnd->MoveWindow(cx-200,nTtlHeight,TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
//		}
//		pWnd = GetDlgItem(IDC_WAVE);
//		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
//		pWnd = GetDlgItem(IDC_CWGRAPH2); 
//		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
//		break;
//	default:
//		break;
//	}
//}
/*--------------------------Marshall's Code2011.2.21 10:50--------------*/
////////////////////////////////////////////////////////////////////////////////////////

void CNiWaveFraqView::GraphShowType(int type, int cx, int cy)
{
	CWnd *pWnd; 
	int nTtlHeight = 20;
	switch(type)
	{
	case 0:
		pWnd = GetDlgItem(IDC_CWGRAPH2);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(0,nTtlHeight,cx-TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
			//pWnd->MoveWindow(2,nTtlHeight,cx-200,(cy-nTtlHeight)/2);//设置控件大小
		}	

		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
		pWnd = GetDlgItem(IDC_TITLE);
		if(pWnd != NULL) pWnd->ShowWindow(TRUE);
		if(pWnd)
		{
			pWnd->MoveWindow(0,2,cx,nTtlHeight-4);
			//pWnd->MoveWindow(2,2,cx,nTtlHeight-4);
		}
		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/

		pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(0,(cy+nTtlHeight)/2,cx-TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
		}
		pWnd = GetDlgItem(IDC_WAVE);     //波形特征值
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(cx-TEXT_WIDTH,nTtlHeight,TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
		}
		pWnd = GetDlgItem(IDC_FRAQ);     //频率主值
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(cx-TEXT_WIDTH,(cy+nTtlHeight)/2,TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
		}
		break;
	case 1:

		pWnd = GetDlgItem(IDC_CWGRAPH2);     //获取控件句柄

		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);		
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(0,nTtlHeight,cx-TEXT_WIDTH,cy-nTtlHeight);//设置控件大小
		}	

		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
		pWnd = GetDlgItem(IDC_TITLE);
		if(pWnd != NULL) pWnd->ShowWindow(TRUE);
		if(pWnd)
		{
			pWnd->MoveWindow(0,2,cx,nTtlHeight-4);
			//pWnd->MoveWindow(2,2,cx,nTtlHeight-4);
		}
		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/

		pWnd = GetDlgItem(IDC_WAVE);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(cx-TEXT_WIDTH,nTtlHeight,TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
		}

		pWnd = GetDlgItem(IDC_CWGRAPH1);
		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_FRAQ); 
		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
		break;
	case 2:
		pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(0,nTtlHeight,cx-TEXT_WIDTH,cy-nTtlHeight);//设置控件大小
		}
		
		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/
		pWnd = GetDlgItem(IDC_TITLE);
		if(pWnd != NULL) pWnd->ShowWindow(TRUE);
		if(pWnd)
		{
			pWnd->MoveWindow(0,2,cx,nTtlHeight-4);
			//pWnd->MoveWindow(2,2,cx,nTtlHeight-4);
		}
		/*--------------------------Marshall's Code2011.2.21 10:41--------------*/


		pWnd = GetDlgItem(IDC_FRAQ);      //获取控件句柄
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			pWnd->MoveWindow(cx-TEXT_WIDTH,nTtlHeight,TEXT_WIDTH,(cy-nTtlHeight)/2);//设置控件大小
		}
		pWnd = GetDlgItem(IDC_WAVE);
		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_CWGRAPH2); 
		if (pWnd!=NULL)		pWnd->ShowWindow(FALSE);
		break;
	default:
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////

//消息：重绘
//void CNiWaveFraqView::OnDraw(CDC* /*pDC*/)//绘制边框//Marshall  2011.3.1 17:4
void CNiWaveFraqView::OnDraw(CDC* pDC)
{	
	//频谱图上面边框绘制


	////CWnd* pWnd = 
	//CPen pen(PS_SOLID,10,RGB(255,0,0));
	//CPen* pOldPen = pDC->SelectObject(&pen);
	//CRect rect;
	//GetClientRect(&rect);

	//

	//
	//CWnd* pWndFGraph = GetDlgItem();
	//CDC* pDCFGraph = pWndFGraph->GetDC();
	////pWndFGraph->Invalidate();
	////pWndFGraph->UpdateWindow();	
	//pDCFGraph->SelectObject(&pen);

	//pDCFGraph->MoveTo(rect.Width()-2,0);
	//pDCFGraph->LineTo(rect.Width()-2,1000);

	//pDCFGraph->SelectObject(pOldPen);

 	TextData();    //cxy   cxy
/*
	CString strTemp;
	strTemp.Format("008:%lf",m_Graph.Axes.Item("XAxis").GetMaximum()*m_deltaFreq);
	AfxMessageBox(strTemp);
*/
}

//显示当前光标线所捕捉的坐标
void CNiWaveFraqView::CursorChangeCwgraph2(long* CursorIndex, VARIANT* XPos, VARIANT* YPos, BOOL* bTracking)
{
	// TODO: 在此处添加消息处理程序代码
	//CPaintDC dc(this);

	CWnd* pWnd=GetDlgItem(IDC_CWGRAPH2);

	CDC* pDC=pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();	

	//创造画笔对象
	CPen* pPenBlue=new CPen;
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0,255));

	pDC->SelectObject(pPenBlue);

	std::stringstream   xxx; 
	xxx <<"当前位置：("<<m_Graph.Cursors.Item(1).GetXPosition()<<","<<m_Graph.Cursors.Item(1).GetYPosition()<<")"; 

	std::string str;xxx>> str; 
	pDC->SetTextColor(RGB(0,0,255));
	//pDC->TextOut(10,2,str.c_str());
	pDC->TextOut(10,2,str.c_str());////Marshall  2011.2.21 22:30

	delete pPenBlue;

	//SetWaveParam();//三行代码注释掉了//Marshall  2011.2.21 22:32
	//SetWaveParam();
	//TextData();
}

//激活工具栏Tab
void CNiWaveFraqView::ClickCwgraph2()
{
	((CMainFrame*)AfxGetMainWnd())->ActivateTabG(0);
}

//消息：窗体底色
HBRUSH CNiWaveFraqView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_BTN) {
        pDC->SetTextColor(HSJInk());
        pDC->SetBkColor(RGB(255, 255, 255));
        pDC->SetBkMode(TRANSPARENT);
        return HSJWhiteBrush();
    }
    return CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}

//波形图标注
void CNiWaveFraqView::Annotations()
{
	double minValue, maxValue;
	unsigned int minIndex, maxIndex;

	int xMax=floor(m_Graph.Axes.Item("XAxis").GetMaximum()/m_deltaFreq);

	int xMin=ceil(m_Graph.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);
	
	CNiColor blue(40,104,197);	
	m_Graph.GetPlots().Item(1).LineColor=blue;	
	GetXRang();   //cxy



	if (xMax<=xRange&&xMin>=0)
	{
		 	int round=xMax-xMin;
			CNiReal64Vector newData(round);
			if (round>0)
			{
				for (int i=0;i<round;i++)
				{
					newData[i]=NiData[xMin];
					xMin++;
				}
				xMin=ceil(m_Graph.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);	


				m_Graph.GetPlots().Add().Name="tempGrap";
				if (col!=NULL)
					m_Graph.GetPlots().Item("tempGrap").LineColor=col;	
				else
					m_Graph.GetPlots().Item("tempGrap").LineColor=blue;

				m_Graph.GetPlots().Item("tempGrap").PlotY(newData, xMin*m_deltaFreq, m_deltaFreq);


				m_Graph.Annotations.Item(1).Plot = m_Graph.Plots.Item("tempGrap");
				m_Graph.Annotations.Item(2).Plot = m_Graph.Plots.Item("tempGrap");

				newData.MinMax(minValue, minIndex, maxValue, maxIndex);

				double posAdjustmentX = (GetXRang()) *0.1;  //cxy
				double posAdjustmentY = (GetYRang()) * 0.02;

				CString   str;
				str.Format("%.3f",(float)maxValue);
				m_Graph.Annotations.Item(1).Caption.Text = _T("Max: ")+str;
				str.Format("%.3f",(float)minValue);
				m_Graph.Annotations.Item(2).Caption.Text = _T("Min ")+str;

				m_Graph.Annotations.Item(1).PointIndex = maxIndex;
				m_Graph.Annotations.Item(1).Caption.SetCoordinates(m_Graph.Axes.Item("XAxis").GetMaximum()-posAdjustmentX, m_Graph.Axes.Item("YAxis-1").GetMaximum()-posAdjustmentY);


				m_Graph.Annotations.Item(2).PointIndex = minIndex;
				m_Graph.Annotations.Item(2).Caption.SetCoordinates(m_Graph.Axes.Item("XAxis").GetMaximum()-posAdjustmentX, m_Graph.Axes.Item("YAxis-1").GetMinimum()+posAdjustmentY);
			}		
	
		
		
	}
	else if(xMin<0&&xMax>0)
	{
		xMin=0;
		int round=xMax-xMin;
		CNiReal64Vector newData(round);
		if (round>0)
		{
			for (int i=0;i<round;i++)
			{
				newData[i]=NiData[xMin];
				xMin++;
			}
			xMin=ceil(m_Graph.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);	

			m_Graph.GetPlots().Add().Name="tempGrap";
			if (col!=NULL)
				m_Graph.GetPlots().Item("tempGrap").LineColor=col;	
			else
				m_Graph.GetPlots().Item("tempGrap").LineColor=blue;			
			m_Graph.GetPlots().Item("tempGrap").PlotY(newData, 0, m_deltaFreq);
			m_Graph.Annotations.Item(1).Plot = m_Graph.Plots.Item("tempGrap");
			m_Graph.Annotations.Item(2).Plot = m_Graph.Plots.Item("tempGrap");
			newData.MinMax(minValue, minIndex, maxValue, maxIndex);

			double posAdjustmentX = (GetXRang()) * 0.1;  //cxy
			double posAdjustmentY = (GetYRang()) * 0.02;

			CString   str;
			str.Format("%.3f",(float)maxValue);
			m_Graph.Annotations.Item(1).Caption.Text = _T("Max: ")+str;
			str.Format("%.3f",(float)minValue);
			m_Graph.Annotations.Item(2).Caption.Text = _T("Min ")+str;

			m_Graph.Annotations.Item(1).PointIndex = maxIndex;
			m_Graph.Annotations.Item(1).Caption.SetCoordinates(m_Graph.Axes.Item("XAxis").GetMaximum()-posAdjustmentX, m_Graph.Axes.Item("YAxis-1").GetMaximum()-posAdjustmentY);

			m_Graph.Annotations.Item(2).PointIndex = minIndex;
			m_Graph.Annotations.Item(2).Caption.SetCoordinates(m_Graph.Axes.Item("XAxis").GetMaximum()-posAdjustmentX, m_Graph.Axes.Item("YAxis-1").GetMinimum()+posAdjustmentY);
		}
	}
	else if(xMax>xRange&&xMin<xRange)
	{
		xMax=floor(xRange/m_deltaFreq);
		xMin=ceil(m_Graph.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);
		int round=xMax-xMin;
		CNiReal64Vector newData(round);
		if (round>0)
		{
			for (int i=0;i<round;i++)
			{
				newData[i]=NiData[xMin];
				xMin++;
			}
			xMin=ceil(m_Graph.Axes.Item("XAxis").GetMinimum());	

			m_Graph.GetPlots().Add().Name="tempGrap";
			if (col!=NULL)
				m_Graph.GetPlots().Item("tempGrap").LineColor=col;	
			else
				m_Graph.GetPlots().Item("tempGrap").LineColor=blue;		
			m_Graph.GetPlots().Item("tempGrap").PlotY(newData, xMin*m_deltaFreq, m_deltaFreq);
			m_Graph.Annotations.Item(1).Plot = m_Graph.Plots.Item("tempGrap");
			m_Graph.Annotations.Item(2).Plot = m_Graph.Plots.Item("tempGrap");
			newData.MinMax(minValue, minIndex, maxValue, maxIndex);

			double posAdjustmentX = (GetXRang()) * 0.1;//cxy
			double posAdjustmentY = (GetYRang()) * 0.02;

			CString   str;
			str.Format("%.3f",(float)maxValue);
			m_Graph.Annotations.Item(1).Caption.Text = _T("Max: ")+str;
			str.Format("%.3f",(float)minValue);
			m_Graph.Annotations.Item(2).Caption.Text = _T("Min ")+str;

			m_Graph.Annotations.Item(1).PointIndex = maxIndex;
			m_Graph.Annotations.Item(1).Caption.SetCoordinates(m_Graph.Axes.Item("XAxis").GetMaximum()-posAdjustmentX, m_Graph.Axes.Item("YAxis-1").GetMaximum()-posAdjustmentY);

			m_Graph.Annotations.Item(2).PointIndex = minIndex;
			m_Graph.Annotations.Item(2).Caption.SetCoordinates(m_Graph.Axes.Item("XAxis").GetMaximum()-posAdjustmentX, m_Graph.Axes.Item("YAxis-1").GetMinimum()+posAdjustmentY);
		}

	}
	else
	{
		AfxMessageBox("标注区域超出范围！");
		Onresume();
	}
}
//获取X轴范围
double CNiWaveFraqView::GetXRang()
{
	return m_Graph.Axes.Item("XAxis").GetMaximum()-m_Graph.Axes.Item("XAxis").GetMinimum();
}

//获取Y轴范围
double CNiWaveFraqView::GetYRang()
{
	return m_Graph.Axes.Item("YAxis-1").GetMaximum()-m_Graph.Axes.Item("YAxis-1").GetMinimum();
}
//显示特征数据
void CNiWaveFraqView::TextData()
{	

	//创造画笔对象
	CWnd* pWndW=GetDlgItem(IDC_WAVE);
	CDC* pDCW=pWndW->GetDC();
	pDCW->SetTextColor(HSJInk());
	pDCW->SetBkColor(RGB(255, 255, 255));
	pWndW->Invalidate();
	pWndW->UpdateWindow();

	CPen* pPenGray=new CPen;
	pPenGray->CreatePen(PS_SOLID,1,HSJBorder());	
	//CWnd* pWndF=GetDlgItem(IDC_FRAQ);////Marshall  2011.2.21 9:50
	CWnd* pWndF=GetDlgItem(IDC_FRAQ);
	CDC* pDCF=pWndF->GetDC();
	pDCF->SetBkColor(RGB(255, 255, 255));
	pWndF->Invalidate();
	pWndF->UpdateWindow();	


	/*--------------------------Marshall's Code2011.2.21 11:52--------------*/
	//波形图上面边框
	CWnd* pWndWGraph = GetDlgItem(IDC_CWGRAPH2);
	CDC* pDCWGraph = pWndWGraph->GetDC();
	pWndWGraph->Invalidate();
	pWndWGraph->UpdateWindow();	
	pDCWGraph->SelectObject(pPenGray);
	pDCWGraph->MoveTo(0,0);
	CRect rect;
	pWndWGraph->GetWindowRect(&rect);
	//pDCWGraph->LineTo(rect.Width(),0);
	pDCWGraph->LineTo(1000,0);

	//频谱图上面边框绘制
	CWnd* pWndFGraph = GetDlgItem(IDC_CWGRAPH1);
	CDC* pDCFGraph = pWndFGraph->GetDC();
	pWndFGraph->Invalidate();
	pWndFGraph->UpdateWindow();	
	pDCFGraph->SelectObject(pPenGray);
	pDCFGraph->MoveTo(0,0);
	rect;
	pWndFGraph->GetWindowRect(&rect);
	//pDCFGraph->LineTo(rect.Width(),0);
	pDCFGraph->LineTo(1000,0);

	/*--------------------------Marshall's Code2011.2.21 11:52--------------*/



	/*--------------------------Marshall's Code2011.2.21 9:20--------------*/
	//设定字体
	//CFont *fnt = ( CFont* )pWndW->GetFont();
	CNiFont font = m_Graph.Font;
	CFont fnt;
	//fnt.CreatePointFont(font.Size,font.Name);
	fnt.CreatePointFont(100,_T("Microsoft YaHei UI"));
	//CFont *fnt = m_Graph.GetFont();
	CFont *def_fnt = (CFont*)pDCW->SelectObject(&fnt);



	/*--------------------------Marshall's Code2011.2.21 9:20--------------*/
	if (GetXRang()>32*m_deltaFreq&&GetYRang()>0.001)
	{

		TCHAR str[50]; 	

		//pDCW->SetTextColor(RGB(0,0,255));//更改颜色//Marshall  2011.2.21 9:9
		pDCW->SetTextColor(HSJInk());

		

		sprintf(str,"起始时刻(秒)：  %10.3f",waveParam.timeStart);
		pDCW->TextOut(2,2,str);
		
		sprintf(str,"终止时刻(秒)：  %10.3f",waveParam.timeEnd);
		pDCW->TextOut(2,22,str);	

		
		sprintf(str,"采样频率(赫兹)：%10d",waveParam.simpling);
		pDCW->TextOut(2,42,str);
/*
		sprintf(str,"起始点(点数)：  %10d",waveParam.startPoint);
		pDCW->TextOut(2,62,str);		

		sprintf(str,"终止点(点数)：  %10d",waveParam.endPoint);
		pDCW->TextOut(2,82,str);
*/
		sprintf(str,"最大值：        %10.2f",waveParam.aMax);
		pDCW->TextOut(2,62,str);

		sprintf(str,"最小值：        %10.2f",waveParam.aMin);
		pDCW->TextOut(2,82,str);

		sprintf(str,"平均值：        %10.2f",waveParam.mean);

		pDCW->TextOut(2,102,str);

		sprintf(str,"中间值：        %10.2f",waveParam.median);
		pDCW->TextOut(2,122,str);


		sprintf(str,"有效值：        %10.2f",waveParam.RMS);
		pDCW->TextOut(2,142,str);

		//sprintf(str,"峰峰值：        %10.2f",waveParam.ppValue);//加上置信度的标记//Marshall  2011.2.24 10:52

		/*--------------------------Marshall's Code2011.2.24 10:53--------------*/
		if(g_bConf97)
			sprintf(str,"97%%峰峰值：     %10.2f",waveParam.ppValue);
		else if(g_bConf95)
			sprintf(str,"95%%峰峰值：     %10.2f",waveParam.ppValue);
		else if(g_bConf100)
			sprintf(str,"100%%峰峰值：    %10.2f",waveParam.ppValue);
		/*--------------------------Marshall's Code2011.2.24 10:53--------------*/

		pDCW->TextOut(2,162,str);

		pDCW->SelectObject(pPenGray);
		CRect rc;
		pWndW->GetWindowRect(&rc);

		
		pDCW->MoveTo(0,0);
		pDCW->LineTo(1000,0);

	 
	}
	else
	{
		Quaternion q=qVector.back();
		m_Graph.Axes.Item("XAxis").SetMinimum(q.xMin);
		m_Graph.Axes.Item("XAxis").SetMaximum(q.xMax);	
		m_Graph.Axes.Item("YAxis-1").SetMinimum(q.yMin);
		m_Graph.Axes.Item("YAxis-1").SetMaximum(q.yMax);
		TCHAR str[50]; 	

		//pDCW->SetTextColor(RGB(0,0,255));////Marshall  2011.2.21 9:12
		pDCW->SetTextColor(HSJInk());

		
		sprintf(str,"起始时刻(秒)：%10.3f",waveParam.timeStart);
		pDCW->TextOut(2,20,str);

		sprintf(str,"终止时刻(秒)：%10.3f",waveParam.timeEnd);
		pDCW->TextOut(2,40,str);		

		sprintf(str,"采样频率(赫兹)：%10d",waveParam.simpling);
		pDCW->TextOut(2,60,str);
/*
		sprintf(str,"起始点(点数)：%10d",waveParam.startPoint);
		pDCW->TextOut(2,80,str);		

		sprintf(str,"终止点(点数)：%10d",waveParam.endPoint);
		pDCW->TextOut(2,100,str);
*/
		sprintf(str,"最大值：      %10.2f",waveParam.aMax);
		pDCW->TextOut(2,80,str);

		sprintf(str,"最小值：      %10.2f",waveParam.aMin);
		pDCW->TextOut(2,100,str);

		sprintf(str,"平均值：      %10.2f",waveParam.mean);

		pDCW->TextOut(2,120,str);

		sprintf(str,"中间值：      %10.2f",waveParam.median);
		pDCW->TextOut(2,140,str);

		sprintf(str,"有效值：      %10.2f",waveParam.RMS);
		pDCW->TextOut(2,160,str);

		
		//sprintf(str,"峰峰值：        %10.2f",waveParam.ppValue);////Marshall  2011.2.24 10:55

		/*--------------------------Marshall's Code2011.2.24 10:53--------------*/
		if(g_bConf97)
			sprintf(str,"97%%峰峰值：   %10.2f",waveParam.ppValue);
		else if(g_bConf95)
			sprintf(str,"95%%峰峰值：   %10.2f",waveParam.ppValue);
		else if(g_bConf100)
			sprintf(str,"100%%峰峰值：  %10.2f",waveParam.ppValue);
		/*--------------------------Marshall's Code2011.2.24 10:53--------------*/

		pDCW->TextOut(2,180,str);
	}
	pDCW->SelectObject(def_fnt);////Marshall  2011.2.21 9:36

	if ((int)ChnFftFlag==1)
	{
		//pDCF->SetTextColor(RGB(0,0,255));////Marshall  2011.2.21 9:13
		CFont *old_fnt = (CFont*)pDCF->SelectObject(&fnt);////Marshall  2011.2.21 9:38
		pDCF->SetTextColor(HSJInk());
		//pDCF->TextOut(2,10,"序号  频率    幅值    相位");	////Marshall  2011.2.21 9:46
		 pDCF->TextOut(2,10,"序号     频率        幅值");
		for (int k=0;k<(int)fftVectorDesc.size();k++)
		{
			CString temp;
			//temp.Format("[%d]%8.3f%8.3f%8.3f",k+1,fftVectorDesc[k].frequency,fftVectorDesc[k].amplitude,fftVectorDesc[k].phase);////Marshall  2011.2.21 9:47
		  temp.Format(      "[%d]  %8.3f    %8.3f",k+1,fftVectorDesc[k].frequency,fftVectorDesc[k].amplitude);
			//pDCF->TextOut(2,25+30*k,temp);////Marshall  2011.2.21 9:51	
			pDCF->TextOut(2,25+25*k,temp);	
		}	

		pDCF->SelectObject(old_fnt);////Marshall  2011.2.21 9:36

		pDCF->SelectObject(pPenGray);
		//pDCF->MoveTo(0,0);	//pDCF->LineTo(151,0);////Marshall  2011.2.21 9:42
		//pDCF->LineTo(1000,0);////Marshall  2011.2.21 9:44
		CRect rc;
		pWndF->GetWindowRect(&rc);
		
		//pDCF->MoveTo(0,rc.Height()-1);
		//pDCF->LineTo(1000,rc.Height()-1);////Marshall  2011.2.21 9:44

		pDCF->MoveTo(0,0);
		pDCF->LineTo(1000,0);////Marshall  2011.2.21 9:44
	}


	ReleaseDC(pDCW);	
	ReleaseDC(pDCF);
	ReleaseDC(pDCWGraph);////Marshall  2011.2.21 12:9
	delete pPenGray;


	///*--------------------------Marshall's Code2011.2.21 12:32--------------*/

	//CRect rect1;


	////CWnd* pChildFrm = (CWnd*) GetParent();
	//CWnd* pChildFrm = (CWnd*) this;
	//pChildFrm->GetClientRect(&rect1);//可以不是ChildFrame的ClientRect
	////CDC* pChildFrmDC = pChildFrm->GetClientDC();//必须是ChildFrame的DC
	//CDC* pChildFrmDC = new CClientDC(this);//必须是ChildFrame的DC
	////绘制边框
	//CBrush brush(RGB(255,0,0));
	//CBrush *pOldBrush= pChildFrmDC->SelectObject(&brush);//选入设备描述表
	//rect1.DeflateRect(1,1);
	//pChildFrmDC ->FrameRect(&rect1,&brush);
	//pChildFrmDC->SelectObject(pOldBrush);
	//ReleaseDC(pChildFrmDC);
	///*--------------------------Marshall's Code2011.2.21 12:32--------------*/
}


//获取绘图边界
void CNiWaveFraqView::GetQuaternion()
{
	if (qVector.size()>1)
	{
		qVector.pop_back();
		Quaternion q=qVector.back();
		m_Graph.Axes.Item("XAxis").SetMinimum(q.xMin);
		m_Graph.Axes.Item("XAxis").SetMaximum(q.xMax);	
		m_Graph.Axes.Item("YAxis-1").SetMinimum(q.yMin);
		m_Graph.Axes.Item("YAxis-1").SetMaximum(q.yMax);

	}
	else
	{
		Resume();
	}
}
//设置波形图绘图区域范围
void CNiWaveFraqView::SetQuaternion()
{
	Quaternion q;	

	q.xMin=m_Graph.Axes.Item("XAxis").GetMinimum();
	q.xMax=m_Graph.Axes.Item("XAxis").GetMaximum();
	q.yMin=m_Graph.Axes.Item("YAxis-1").GetMinimum();
	q.yMax=m_Graph.Axes.Item("YAxis-1").GetMaximum();

	qVector.push_back(q);
}

//保存波形频谱图
//@path	存储路径
//@type	存储图片格式
void CNiWaveFraqView::SaveAllView(CString Path,int type)
{

	FFTProc();





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

	CString strBuff ;
	CString   strFileName;   
	LPTSTR   pszfilename;   
	m_Graph.GetWindowText(strFileName);
	pszfilename=strFileName.GetBuffer(_MAX_PATH);

	CString strSaveFilapath = Path;

	strSaveFilapath.MakeLower();

	switch(type)
	{
	case 1:
		strSaveFilapath = strSaveFilapath + ".bmp";	
		strSaveFilapath.MakeLower();
		image.Save(strSaveFilapath,Gdiplus::ImageFormatBMP);
		break;
	case 2:
		strSaveFilapath = strSaveFilapath + ".jpg";
		strSaveFilapath.MakeLower();
		image.Save(strSaveFilapath,Gdiplus::ImageFormatJPEG);
		break;
	case 3:
		strSaveFilapath = strSaveFilapath + ".gif";
		strSaveFilapath.MakeLower();
		image.Save(strSaveFilapath,Gdiplus::ImageFormatGIF);
		break;
	case 4:
		strSaveFilapath = strSaveFilapath + ".png";
		strSaveFilapath.MakeLower();
		image.Save(strSaveFilapath,Gdiplus::ImageFormatPNG);
		break;
	default:
		strSaveFilapath = strSaveFilapath + ".tiff";
		strSaveFilapath.MakeLower();
		image.Save(strSaveFilapath,Gdiplus::ImageFormatTIFF);
		break;
	}		


	image.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
}
//保存波形频谱图
void CNiWaveFraqView::SaveAllView()
{
	OnWavefraqgrap();
	UpdateWindow(); 

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

	//m_Graph.GetWindowText(strFileName);////Marshall  2011.2.21 15:10
	m_ctrlTitle.GetWindowText(strFileName);////Marshall  2011.2.21 15:10


		int nRight = strFileName.ReverseFind(_T(','));
	CString strFile = strFileName.Right(strFileName.GetLength() - nRight - 1);

	//pszfilename=strFileName.GetBuffer(_MAX_PATH);
	pszfilename=strFile.GetBuffer(_MAX_PATH);

	//FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.lpstrFile   =   strFile.GetBuffer(0);   

	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  

	if(FileOpenDlg.DoModal() == IDOK)
	{
		//CString strSaveFilapath = FileOpenDlg.GetPathName();
		//CString strSaveFilapath;
		//FileOpenDlg.()
		//m_Graph.GetWindowText(strSaveFilapath);
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存整个视图...");

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
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存完成，存储为  "+strSaveFilapath);

	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消保存整个视图");
	}
		
	image.Detach(); 


	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
}

//自定义保存波形频谱图
void CNiWaveFraqView::CustomSaveAllView()
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
	//RECT controlBounds;
	//m_Graph.GetClientRect(&controlBounds);
	//int bmpWidth = controlBounds.right - controlBounds.left;
	//int bmpHeight = controlBounds.bottom - controlBounds.top;

	CCustomSaveDlg dlg(NULL,w,h);
	//CCustomSaveDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		//// Create the bitmap with the same dimensions of the control.
		//CImage bmp;
		//bmp.Create(bmpWidth, bmpHeight, 32);

		//// Play the enhanced metafile from the CNiPicture to the bitmap.
		//RECT imageBounds = { 1, 1, bmpWidth, bmpHeight };
		//CNiPicture controlImage = m_Graph.ControlImage();

		//HDC bmpDC = bmp.GetDC();
		//::PlayEnhMetaFile(bmpDC, reinterpret_cast< HENHMETAFILE >(controlImage.Handle), &imageBounds);
		// 
		//bmp.ReleaseDC();

		CString strFilter;

		switch(dlg.m_fileType)
		{
		case 1:
			strFilter = _T("BMP文件(*.BMP)|*.BMP||");
			break;
		case 2:
			strFilter = _T("JPEG文件(*.JPG)|*.JPG||");
			break;
		case 3:
			strFilter = _T("GIF文件(*.GIF)|*GIF||");
			break;
		case 4:
			strFilter = _T("PNG文件(*.PNG)|*.PNG||");
			break;
		default:
			strFilter = _T("TIFF文件(*.TIFF)|*.TIFF||");
			break;
		}

		CFileDialog FileOpenDlg(FALSE, NULL, NULL,OFN_HIDEREADONLY | OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT, strFilter);
		CString strBuff ;

		CString   strFileName;   
		LPTSTR   pszfilename;   

		//m_Graph.GetWindowText(strFileName);////Marshall  2011.2.21 15:17
//		m_ctrlTitle.GetWindowText(strFileName);////Marshall  2011.2.21 15:17
		int nRight = strFileName.ReverseFind(_T(','));
	CString strFile = strFileName.Right(strFileName.GetLength() - nRight - 1);
	//strFileName = strFile;

		//pszfilename=strFileName.GetBuffer(_MAX_PATH);
	pszfilename=strFile.GetBuffer(_MAX_PATH);

		//FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.lpstrFile   =   strFile.GetBuffer(0); 

		FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;  

		FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  
		int iNewWidth=dlg.mWidth,iNewHeight=dlg.mHeight;

		IStream *pStm;
		CreateStreamOnHGlobal(NULL, true, &pStm); 
		if(FileOpenDlg.DoModal() == IDOK)
		{	
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：自定义保存全部视图...");

			CString strSaveFilapath = FileOpenDlg.GetPathName();
			strSaveFilapath.MakeLower();


			switch(dlg.m_fileType)
			{
			case 1:
				strBuff = strSaveFilapath.Right(4);
				strSaveFilapath = strSaveFilapath + ".bmp";	
				image.Save(pStm, Gdiplus::ImageFormatBMP);
				break;
			case 2:
				strBuff = strSaveFilapath.Right(4);
				strSaveFilapath = strSaveFilapath + ".jpg";
				image.Save(pStm, Gdiplus::ImageFormatJPEG);
				break;
			case 3:
				strBuff = strSaveFilapath.Right(4);
				strSaveFilapath = strSaveFilapath + ".gif";
				image.Save(pStm, Gdiplus::ImageFormatGIF);
				break;
			case 4:
				strBuff = strSaveFilapath.Right(4);
				strSaveFilapath = strSaveFilapath + ".png";
				image.Save(pStm, Gdiplus::ImageFormatPNG);
				break;
			default:
				strBuff = strSaveFilapath.Right(5);
				strSaveFilapath = strSaveFilapath + ".tiff";
				image.Save(pStm, Gdiplus::ImageFormatTIFF);
				break;
			}
			CImage image;
			image.Load(pStm);
			//image.Load(_T(pStm));

			CDC *screenDC = GetDC();
			CDC *pMDC = new CDC;
			pMDC->CreateCompatibleDC(screenDC);

			CBitmap *pb = new CBitmap;
			pb->CreateCompatibleBitmap(screenDC, iNewWidth, iNewHeight);

			CBitmap *pob = pMDC->SelectObject(pb);
			image.StretchBlt(pMDC->m_hDC,0, 0, iNewWidth, iNewHeight, 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);
			pMDC->SelectObject(pob);

			CImage new_image;
			new_image.Attach((HBITMAP)(*pb));
			new_image.Save(strSaveFilapath);
			new_image.Detach();

			ReleaseDC(screenDC);

			pStm->Release();
			AfxMessageBox("保存完成!");
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存完成，存储为  "+strSaveFilapath);

		}
		else
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消自定义保存全部视图");
		}

	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消自定义保存全部视图");
	}
	image.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
}

//保存波形图
void CNiWaveFraqView::SaveSigleView()
{
	OnWavegraph();
	UpdateWindow();

	CImage bmp;	
	RECT controlBounds;

	//	CRect rcClient = NULL; 
	//HWND hwnd=this->GetSafeHwnd();  //令HWND为本窗口的句柄 
	//::GetWindowRect(hwnd,&rcClient);  
	//int bmpWidth = rcClient.right - rcClient.left; 
	//int bmpHeight = rcClient.bottom - rcClient.top; 

	m_Graph.GetClientRect(&controlBounds);
	int bmpWidth = controlBounds.right - controlBounds.left;
	int bmpHeight = controlBounds.bottom - controlBounds.top;

	// Create the bitmap with the same dimensions of the control.

	bmp.Create(bmpWidth, bmpHeight, 24);

	// Play the enhanced metafile from the CNiPicture to the bitmap.
	RECT imageBounds = { 0, 0, bmpWidth, bmpHeight };
	CNiPicture controlImage = m_Graph.ControlImage();
	HDC bmpDC = bmp.GetDC();
	::PlayEnhMetaFile(bmpDC, reinterpret_cast< HENHMETAFILE >(controlImage.Handle), &imageBounds);
	bmp.ReleaseDC();
	
	CString filePath;
	CString strFilter;
	strFilter = _T("BMP文件(*.BMP)|*.BMP|JPEG文件(*.JPG)|*.JPG|GIF文件(*.GIF)|*GIF|PNG文件(*.PNG)|*.PNG|TIFF文件(*.TIFF)|*.TIFF||");

	CFileDialog FileOpenDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT, strFilter);

	CString strBuff ;
	CString   strFileName;   
	LPTSTR   pszfilename;   

	//m_Graph.GetWindowText(strFileName);////Marshall  2011.2.21 15:7
	m_ctrlTitle.GetWindowText(strFileName);////Marshall  2011.2.21 15:7

	int nRight = strFileName.ReverseFind(_T(','));
	CString strFile = strFileName.Right(strFileName.GetLength() - nRight - 1);
	//strFileName = strFile;

	//pszfilename=strFileName.GetBuffer(_MAX_PATH);
	pszfilename=strFile.GetBuffer(_MAX_PATH);

	//FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.lpstrFile   =   strFile.GetBuffer(0);   

	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;     

	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  

	if(FileOpenDlg.DoModal() == IDOK)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存单个波形图...");

		CString strSaveFilapath = FileOpenDlg.GetPathName();
		strSaveFilapath.MakeLower();


		switch(FileOpenDlg.m_ofn.nFilterIndex)
		{
		case 1:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".bmp") != 0)
				strSaveFilapath = strSaveFilapath + ".bmp";	
			strSaveFilapath.MakeLower();
			bmp.Save(strSaveFilapath,Gdiplus::ImageFormatBMP);
			break;
		case 2:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".jpg") != 0)
				strSaveFilapath = strSaveFilapath + ".jpg";
			strSaveFilapath.MakeLower();
			bmp.Save(strSaveFilapath,Gdiplus::ImageFormatJPEG);
			break;
		case 3:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".gif") != 0)
				strSaveFilapath = strSaveFilapath + ".gif";
			strSaveFilapath.MakeLower();
			bmp.Save(strSaveFilapath,Gdiplus::ImageFormatGIF);
			break;
		case 4:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".png") != 0)
				strSaveFilapath = strSaveFilapath + ".png";
			strSaveFilapath.MakeLower();
			bmp.Save(strSaveFilapath,Gdiplus::ImageFormatPNG);
			break;
		default:
			strBuff = strSaveFilapath.Right(5);
			if(strBuff.Compare(".tiff") != 0)				
				strSaveFilapath = strSaveFilapath + ".tiff";
			strSaveFilapath.MakeLower();
			bmp.Save(strSaveFilapath,Gdiplus::ImageFormatTIFF);
			break;
		}		
		AfxMessageBox("保存完成!");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存完成，存储为  "+strSaveFilapath);
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消保存单个波形图");
	}
	DeleteObject(bmp); 

}

//保存临时图片
BOOL CNiWaveFraqView::SaveWaveTemp()
{
	CString strSaveFilapath = "C:\\WINDOWS\\waveTemp.gif";
	
	CRect rcClient = NULL; 
	HWND hwnd=this->GetSafeHwnd();  //令HWND为本窗口的句柄 
	::GetWindowRect(hwnd,&rcClient);  
	int w = rcClient.right - rcClient.left; 
	int h = rcClient.bottom - rcClient.top; 

	HDC dc = ::GetDC(hwnd); 

	HDC gpBmpHdc = CreateCompatibleDC(dc); 
	HBITMAP hBitmap = NULL; 
	hBitmap = CreateCompatibleBitmap(dc, w, h); 
	HGDIOBJ oldhdc = SelectObject(gpBmpHdc, hBitmap); 

	::PrintWindow(hwnd,gpBmpHdc,NULL); 

	CImage imageT;
	if (hBitmap!=NULL)
	{
		imageT.Attach(hBitmap); 
	} 
	else
	{
		return FALSE;
	}

		IStream *pStm;
		CreateStreamOnHGlobal(NULL, true, &pStm); 


		imageT.Save(pStm, Gdiplus::ImageFormatGIF);

			CImage image;
			image.Load(pStm);
			//image.Load(_T(pStm));

			CDC *screenDC = GetDC();
			CDC *pMDC = new CDC;
			pMDC->CreateCompatibleDC(screenDC);

			CBitmap *pb = new CBitmap;
			pb->CreateCompatibleBitmap(screenDC, 1000, 600);

			CBitmap *pob = pMDC->SelectObject(pb);
			image.StretchBlt(pMDC->m_hDC,0, 0, 1000, 600, 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);
			pMDC->SelectObject(pob);

			CImage new_image;
			new_image.Attach((HBITMAP)(*pb));
			new_image.Save(strSaveFilapath,Gdiplus::ImageFormatGIF);
			new_image.Detach();

			ReleaseDC(screenDC);

			pStm->Release();

	imageT.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
	if (PathFileExists(strSaveFilapath))	
		return TRUE;
	else
		return FALSE;

}

//创建窗体
int CNiWaveFraqView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if(pMainFrame)
	{
		//pMainFrame->m_pExamView = this;
		pMainFrame->m_PWave = this;
	}	

	return 0;
}


//根据范围设置波形数据
//@vector <float> vData	通道数据
//@int sFreq			频率
//@int min				最小值
//@int max				最大值
void CNiWaveFraqView::SetData(vector <float> vData,int sFreq,int min,int max)
{
	samplingFreq=sFreq;
	m_deltaFreq=1.0/samplingFreq;
	//vSamplingFreq.push_back(samplingFreq);
	xRange=(unsigned int)vData.size();//(unsigned int)vData.size()+1;   //cxy
	CNiReal64Vector NiDt((unsigned int)vData.size());
	//CNiReal64Vector ResumeData((unsigned int)vData.size());
	vector<float>::iterator it2 = vData.begin();
	int k=0;   //cxy
	for(; it2 != vData.end(); it2++)
	{
		
		NiDt[k]=*it2;
		k++;
	}

	NiData=NiDt;

	
	ResumeData=NiDt;

	waveWidthMin=min;
	//waveWidthMax=max+1;
	waveWidthMax=max;//cxy
/*	
	CString strTemp;

    double m_t;
	m_t=double(1.0*waveWidthMin/samplingFreq);
	strTemp.Format("%lf",m_t);
	AfxMessageBox(strTemp);
*/
}

//保存数据到文本文件
void CNiWaveFraqView::SaveToTxt(CString FilePathName)
{
	FILE	*fp = 0;
	fp = fopen(FilePathName,"w");
	if(fp==0)return;

	for(int i=0;i<(int)NiData.GetSize();i++)
	{
		fprintf(fp,"%lf\n",NiData[i]); 
		//fprintf(fp,"[%d]=%6.2f;\n",i,ori_ptr[i]); 

	}
	fseek(fp,0,SEEK_END);
	fclose(fp);
}
//加载文件数据进行FFT运算
void CNiWaveFraqView::LoadDataFFT(vector<double> vData,CString cap,COLORREF colref,int freq)
{
	samplingFreq=freq;
	m_deltaFreq=1.0/samplingFreq;
	//xRange=(unsigned int)vData.size()+1;
	xRange=(unsigned int)vData.size();  //cxy
	CNiReal64Vector NiDt((unsigned int)vData.size());
	vector<double>::iterator it2 = vData.begin();
	int k=-1;
	for(; it2 != vData.end(); it2++)
	{
		k++;
		NiDt[k]=*it2;
	}

	waveWidthMin=0;
	//waveWidthMax=(int)vData.size()+1;
	waveWidthMax=(int)vData.size();//cxy
	title=cap;
	col=colref;
	NiData=NiDt;
	ChnFftFlag=1;
	//CMainFrame*   pCMF = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	//pCMF->m_wndChannelsToolBar.SetButtonStyle(0,TBBS_DISABLED);
	//pCMF->m_wndChannelsToolBar.SetButtonStyle(4,TBBS_HIDDEN);
	/*pCMF->m_wndChannelsToolBar.ResetImages();*/
	//pCMF->m_wndChannelsToolBar.ResetAll();
	//pCMF->m_wndChannelsToolBar.UpdateButton(4);

}
//添加移动趋势
void CNiWaveFraqView::AddTrendWave(int TrendType,int winW,int winS,COLORREF color)
{
	
	vector<double> vecResult;
	double dRelt=0.0;
	//vecResult.clear();
	for (int k=0;k<(int)NiData.GetSize()-winS;k+=winS)
	{
		CNiReal64Vector TrendData(winW);

		for (int p=0;p<winW;p++)
		{
			
			if(p+k>=(int)NiData.GetSize())
				break;
			else
				TrendData[p]=NiData[p+k];
		}
		double m_mean_temp=0.0;
		if (TrendType==1)
		{
			CNiMath::Mean(TrendData,dRelt);
			m_mean_temp=dRelt;
		} 
		else if(TrendType==2)
		{
			CNiMath::Median(TrendData,dRelt);
		}
		else if(TrendType==3)

		{	
			CNiMath::Mean(TrendData,dRelt);


			CNiReal64Vector m_temp_Data(winW);
			for (int i=0;i<winW;i++)
			{
				m_temp_Data[i]=newData[i]-m_mean_temp;
			}
			CNiMath::RMS(m_temp_Data,dRelt);   //cxy 20110512



			dRelt=dRelt*2*1.41421356;
		//	CNiMath::RMS(TrendData,dRelt);
			
		}
		else
		{
			double minV, maxV;
			unsigned int minI, maxI;
			TrendData.MinMax(minV, minI, maxV, maxI);
			dRelt=maxV-minV;
		}

		vecResult.push_back(dRelt);
	}		
	CNiReal64Vector TrendResult((int)vecResult.size());
	m_tempTrendResult.SetSize((int)vecResult.size());

	for (int k=0;k<(int)vecResult.size();k++)
	{
		TrendResult[k]=vecResult[k];
		m_tempTrendResult[k]=vecResult[k];
	}


	//m_tempTrendResult[vecResult.size()]=m_tempTrendResult[vecResult.size()-1];

	



	double minValue, maxValue, valueDecl;
	unsigned int minIndex, maxIndex;
	double currentMin,currentMax;
	NiData.MinMax(minValue, minIndex, maxValue, maxIndex);
	valueDecl=(maxValue-minValue)*0.1;

	waveHeightMin=(int)(minValue-valueDecl)-1;
	waveHeightMax=(int)(maxValue+valueDecl)+1;


	minValue=(int)m_Graph.Axes.Item("YAxis-1").GetMinimum();
	maxValue=(int)m_Graph.Axes.Item("YAxis-1").GetMaximum();
	valueDecl=(maxValue-minValue)*0.1;

	currentMin=(int)(minValue-valueDecl)-1;
	currentMax=(int)(maxValue+valueDecl)+1;

	m_Graph.GetPlots().Add().Name="tempGrap111";;
	m_Graph.GetPlots().Add().Name="tempGrap222";;

	m_Graph.GetPlots().Item(waveCout+1).LineColor=color;			
	//m_Graph.GetPlots().Item(waveCout+1).PointColor=RGB(255,0,0);
	//m_Graph.GetPlots().Item(waveCout+1).PointStyle=CNiPlot::PointX;
	m_Graph.GetPlots().Item(waveCout+1).PlotY(TrendResult, winW*m_deltaFreq, winS*m_deltaFreq);


	m_wavecount=waveCout+1;

	//if (currentMin>waveHeightMin&&currentMax>waveHeightMax)
	//{
	waveHeightMax=waveHeightMax>(int)currentMax?waveHeightMax:(int)currentMax;
	waveHeightMin=waveHeightMin<(int)currentMin?waveHeightMin:(int)currentMin;
	m_Graph.Axes.Item("YAxis-1").SetMinMax(waveHeightMin,waveHeightMax);

	//if (m_Graph.Axes.Item("YAxis-1").GetMinimum()<)
	double m_1,m_2;
	m_1=m_Graph.Axes.Item("XAxis").GetMinimum();
	m_2=m_Graph.Axes.Item("XAxis").GetMaximum();

	if (m_2>NiData.GetSize()*m_deltaFreq)
	{
		m_2=NiData.GetSize()*m_deltaFreq;
		m_Graph.Axes.Item("XAxis").SetMinMax(m_1,m_2);
	}



	waveCout+=2;

}

//添加新的波形数据
//@vector<float> vData	通道数据
//@COLORREF color		通道曲线颜色
//@save_freq			采样频率
void CNiWaveFraqView::AddNewWave(int save_freq,std::vector<float> vData,COLORREF color)
{
	//vSamplingFreq.push_back(save_freq);
	
	double scale;
	if (samplingFreq)	
		scale=(double)samplingFreq/(double)save_freq;

	CNiReal64Vector NiDt((unsigned int)vData.size());
	vector<float>::iterator it2 = vData.begin();
	int k=-1;
	for(; it2 != vData.end(); it2++)
	{
		k++;
		NiDt[k]=*it2;
	}

	double minValue, maxValue, valueDecl;
	unsigned int minIndex, maxIndex;
	double currentMin,currentMax;
	NiDt.MinMax(minValue, minIndex, maxValue, maxIndex);
	valueDecl=(maxValue-minValue)*0.1;

	waveHeightMin=(int)(minValue-valueDecl)-1;
	waveHeightMax=(int)(maxValue+valueDecl)+1;
	
	
	minValue=(int)m_Graph.Axes.Item("YAxis-1").GetMinimum();
	maxValue=(int)m_Graph.Axes.Item("YAxis-1").GetMaximum();
	valueDecl=(maxValue-minValue)*0.1;

	currentMin=(int)(minValue-valueDecl)-1;
	currentMax=(int)(maxValue+valueDecl)+1;

	m_Graph.GetPlots().Add();
	m_Graph.GetPlots().Add();

	m_Graph.GetPlots().Item(waveCout+1).LineColor=color;			
	m_Graph.GetPlots().Item(waveCout+1).PlotY(NiDt, 0, scale*m_deltaFreq);

	//if (currentMin>waveHeightMin&&currentMax>waveHeightMax)
	//{
	waveHeightMax=waveHeightMax>(int)currentMax?waveHeightMax:(int)currentMax;
	waveHeightMin=waveHeightMin<(int)currentMin?waveHeightMin:(int)currentMin;
	m_Graph.Axes.Item("YAxis-1").SetMinMax(waveHeightMin,waveHeightMax);
	

	waveCout+=2;

}


//激活工具栏Tab
void CNiWaveFraqView::ClickCwgraph1()
{
	((CMainFrame*)AfxGetMainWnd())->ActivateTabG(1);
}

//排序算法
//bool great_second(const FFTParam & m1, const FFTParam & m2) {//原来的代码，采用改进算法后，不用该函数//Marshall  2011.2.23 20:29
//	return m1.amplitude > m2.amplitude;
//}

//bool _cdecl great_second(const void * m1, const void * m2) {//不用该函数//Marshall  2011.2.23 20:29
//	FFTParam Temp1,Temp2;
//	Temp1 = *(FFTParam*)m1;
//	Temp2 = *(FFTParam*)m2;
//
//	if(Temp1.amplitude > Temp2.amplitude)
//		return 1;
//	else if(Temp1.amplitude < Temp2.amplitude )
//		return -1;
//	else
//		return 0;
//	//return m1.amplitude > m2.amplitude;
//}

//int  _cdecl  /*CNiWaveFraqView::*/Compare(const void* a,const void* b)//不用该函数//Marshall  2011.2.23 20:28
//{
//	double Temp1,Temp2;
//	Temp1=*(double*)a,Temp2=*(double*)b;
//	if(Temp1>Temp2)
//		return 1;
//	else if(Temp1<Temp2)
//		return -1;
//	else
//		return 0;
//}

//设置频谱数据
//@CNiReal64Vector niData	波形数据
//@double sample			采样频率
void CNiWaveFraqView::SetFraqData(CNiReal64Vector niData, double sample)
{
	int dataSize = niData.GetSize();                         //Number of data points acquired
	CNiReal64Vector realData, imagData;
	realData.SetSize(dataSize);
	imagData.SetSize(dataSize);

	CNiComplexReal64Vector fftData;
			/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
		//FILE* f = fopen("c:\\niData.txt","w+");

		//for(int i = 0; i< niData.GetSize(); i++)
		//{

		//		fprintf(f,"%f\n",niData[i]);

		//}
		//fclose(f);
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
	// Transform the time domain data  
	if (!isFullPeriodFFT)
	{
		if(bRetime)
			realData=re_time(niData);//默认不进行re_time重新采样//Marshall  2011.2.26 11:16
		else
			realData = niData;
	}
	else
	{
		//realData+=NiData;//原程序不进行重采样//Marshall  2011.2.26 18:41
		if(bRetime)
			realData=re_time(NiData);
		else
			realData+=NiData;
	}
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
		//FILE *f = fopen("c:\\realData.txt","w+");

		//for(int i = 0; i< realData.GetSize(); i++)
		//{

		//		fprintf(f,"%f\n",realData[i]);

		//}
		//fclose(f);
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/

	CNiMath::FFT(realData,false,fftData);


	int numPts = dataSize/2;
	double scalingFactor = 1.0/(double)dataSize;
	deltaFreq = samplingFreq*scalingFactor;    

	// Get only the first half of the double sided spectrum and scale it to return a
	// single sided spectrum in RMS.
	CNiReal64Vector temp(numPts);
	frequencyData=temp;
	double dtmp = 0;
	//for( int i=1;i< numPts;i++)
	//{
	//	dtmp = fftData[i].Magnitude();
	//	frequencyData[i] = fftData[i].Magnitude()*scalingFactor*2;
	//}

	/*--------------------------Marshall's Code2011.2.26 9:51--------------*/
	//float *dTemp = new float(numPts);
	//memset(dTemp,0,sizeof(float) * numPts);
	for( int i=1;i< numPts;i++)
	{
		dtmp = fftData[i].Magnitude();
		frequencyData[i] = fftData[i].Magnitude()*scalingFactor*2;
	//	dTemp[i] = frequencyData[i];
	}
	/*--------------------------Marshall's Code2011.2.26 9:51--------------*/
	// Scale DC value
	frequencyData[0] = 0;//fftData[0].Magnitude() * scalingFactor;//剔除直流分量
	freqRange=(int)(frequencyData.GetSize()*deltaFreq)+1;
	
	fftVector.clear();
	fftVectorDesc.clear();

	//计算费时从此处开始测试//Marshall  2011.3.6 14:18


	/*--------------------------Marshall's Code2011.3.6 14:19-Temp-------------*/

	/*--------------------------Marshall's Code Test Cost Time2011.3.6 14:20--------------*/
	//clock_t begin_time,end_time;
	//int cost_time = 0;
	//begin_time = clock();

	/*--------------------------Marshall's Code2011.3.6 14:19--------------*/
	//for (int p=1;p<numPts;p++)//原来的排序算法用到//Marshall  2011.2.23 19:5
	//{
	//	
	//	FFTParam fp;
	//	fp.amplitude=frequencyData[p];
	//	fp.frequency=deltaFreq*p;
	//	fp.phase=atan2(fftData[p].imag,fftData[p].real);
	//	//fp.phase=fp.phase*180.0/3.1415926535;
	//	fftVector.push_back(fp);
	//}	
	CNiReal64Vector niTmp;
	niTmp.SetSize(numPts-1);

	for (int p=1;p<numPts;p++)//将所有幅值放入niTmp中，注意下标//Marshall  2011.2.23 19:5
	{
		
		//FFTParam fp;//没有用到，可以注释掉//Marshall  2011.2.27 19:26
		niTmp[p-1]=frequencyData[p];//p 注意 p + 1 //Marshall  2011.2.23 19:7
	}	
	
	//排序，niTmp的前8个放入最大的8个幅值//Marshall  2011.2.23 20:27
	int nIdx[8];
	double dTmp = 0.0;
	for(int m = 0; m < 8; m++)
	{
		for(int n = m + 1; n < numPts -1; n++)
		{
			if(niTmp[m] < niTmp[n])
			{
				dTmp = niTmp[n];
				niTmp[n] = niTmp[m];
				niTmp[m] = dTmp;
			}
		}
	}

	//找最大的8个值对应的索引//Marshall  2011.2.23 20:25
	for( m = 0; m < 8; m++)
		for(int n = 0 ; n < numPts - 1; n++)
			if(niTmp[m] == frequencyData[n+1])
			{
				nIdx[m] = n + 1;
				break;
			}


	//std::sort(fftVector.begin(), fftVector.end(), great_second);//原来的排序算法用到//Marshall  2011.2.23 18:26
	//if (fftVector.size()>=8)
	//{
	
	//		if (fftVector[k].phase<0)
	//			fftVector[k].phase=fftVector[k].phase*180.0/3.1415926535+360.0;
	//		else
	//	for (int k=0;k<8;k++)
	//	{
//			fftVector[k].phase=fftVector[k].phase*180.0/3.1415926535;

	//		fftVectorDesc.push_back(fftVector[k]);
	//	}
	//}
		//end_time = clock();
		//cost_time = (end_time - begin_time)/CLOCKS_PER_SEC;
		//FILE* f = fopen("c:\\costtime.txt","w+");
		//if(!f)
		//{
		//	fprintf(f,"cost_time = %f\n",cost_time);
		//}
		//fclose(f);
		//f = 0;

		for (int k=0;k<8;k++)
		{

			/*--------------------------Marshall's Code2011.2.23 20:13--------------*/
			//新排序方法得到数据索引，将最大的8个值保存到fftVector
			FFTParam fp;
			fp.amplitude=frequencyData[nIdx[k]];
			fp.frequency=deltaFreq*nIdx[k];
			fp.phase=atan2(fftData[nIdx[k]].imag,fftData[nIdx[k]].real);
			//fp.phase=fp.phase*180.0/3.1415926535;
			fftVector.push_back(fp);
			/*--------------------------Marshall's Code2011.2.23 20:13--------------*/
			if (fftVector[k].phase<0)
				fftVector[k].phase=fftVector[k].phase*180.0/3.1415926535+360.0;
			else
				fftVector[k].phase=fftVector[k].phase*180.0/3.1415926535;

			fftVectorDesc.push_back(fftVector[k]);
		}
}
//FFT数据处理
void CNiWaveFraqView::FFTProc()
{
	qVectorF.clear();

//	int xMax=33568;
//	int xMin=12284;


	int xMax;
	int xMin;

	if (m_batchSaveBit)
	{
		xMax=m_maxX;
		xMin=m_minX;

		//xMax=33568;
		//xMin=12284;


/*

		if(!isHasKeyPhase)
			m_Graph.Axes.Item("XAxis").SetMinMax(m_maxX*m_deltaFreq,m_maxX*m_deltaFreq);

		CString strTemp;
		double m_t=double(m_maxX);
		strTemp.Format("001:%lf",m_t);
		AfxMessageBox(strTemp);

		strTemp.Format("001:%lf",m_deltaFreq);
		AfxMessageBox(strTemp);
*/

	}
     
	else
	{

		xMax=floor(m_Graph.Axes.Item("XAxis").GetMaximum()/m_deltaFreq);
		xMin=ceil(m_Graph.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);
		//xMax=waveWidthMax;
		//xMin=waveWidthMin;
	/*	
	    CString strTemp;
		double m_t=double(xMin);
		strTemp.Format("002:%lf",m_t);
		AfxMessageBox(strTemp);
	*/
	}

	




	if (!isFullPeriodFFT)
	{	
		//此时isFullPeriodFFT = 0//Marshall  2011.2.25 9:46
		//整周期时处理波形图横坐标最大值、最小值//Marshall  2011.2.25 9:45


		int minT=0,maxT=0;
/*
		minT=int(kPhaseX[0]);
		if (kPhaseX.size()>1)
		 	maxT=int(kPhaseX[kPhaseX.size()-1]);
*/
	 

 

/*

		if (xMax>0&&xMin>=0&&xMax>xMin)
		{
			xMax=maxT>xMax?xMax:maxT;
			xMin=minT<xMin?xMin:minT;

		}
*/


		

		for (int k=0;k<(int)kPhaseX.size();k++)
		{
			if (kPhaseX[k]>=xMin)
			{
				minT=kPhaseX[k];
				break;
			}
		}

		for (int k=0;k<(int)kPhaseX.size();k++)
		{
			if (kPhaseX[k]<=xMax)
			{
				maxT=kPhaseX[k];
			}
			else
				break;
		}

		CString strTemp;
		strTemp.Format("009:%d",minT);
		//AfxMessageBox(strTemp);


		if (minT==maxT)
		{
			minT=minKZ;
			maxT=maxKZ;
		}
		if (maxT-minT>=kPhaseX[1]-kPhaseX[0])
		{
			xMax=maxT;
			xMin=minT;
			m_Graph.Axes.Item("XAxis").SetMinimum(xMin*m_deltaFreq);
			m_Graph.Axes.Item("XAxis").SetMaximum(xMax*m_deltaFreq);

 
		}
		else
		{
			xMax=maxT>minT?maxT:minT;
			xMin=maxT<minT?maxT:minT;
			m_Graph.Axes.Item("XAxis").SetMinimum(xMin*m_deltaFreq);
			m_Graph.Axes.Item("XAxis").SetMaximum(xMax*m_deltaFreq);

		}



//		m_Graph.Axes.Item("XAxis").SetMinimum(xMin*m_deltaFreq);
//		m_Graph.Axes.Item("XAxis").SetMaximum(xMax*m_deltaFreq);

	}

	minKZ=xMin;
	maxKZ=xMax;

    //cxy   整周期起始点坐标问题

 

	SetWaveParam(); 

/*
	CString strTemp;
	strTemp.Format("%lf",waveParam.timeStart);
	AfxMessageBox(strTemp);
*/
	//m_Graph.Axes.Item("XAxis").SetMinMax(waveParam.timeStart,waveParam.timeEnd);
 
	int round=xMax-xMin;

//	CNiReal64Vector newData(round);//将newData变为成员变量，便于保存当前时段波形数据//Marshall  2011.3.1 14:41
	newData.SetSize(round);
	if (round>16)
	{
		if (xMin>=0&&xMax<=(int)NiData.GetSize())
		{
			for (int i=0;i<round;i++)
			{
				newData[i]=NiData[xMin];
				xMin++;
			}


			/*--------------------------Marshall's Code2011.2.25 22:23--------------*/
	/*		FILE* f = fopen("c:\\setchnData.txt","w+");

	for (int i=0;i<round;i++)
		{
			fprintf(f,"%f\n",NiData[i]);
			}
	fclose(f);*/
			/*--------------------------Marshall's Code2011.2.25 22:23--------------*/

		}
		else
		{
			for (int i=0;i<round;i++)
			{
				newData[i]=0;				
			}
		}


		SetFraqData(newData,samplingFreq);


		if(!isHasKeyPhase&&m_batchSaveBit)
		{
			m_Graph.Axes.Item("XAxis").SetMinMax(m_minX*m_deltaFreq,m_maxX*m_deltaFreq);
			SetWaveParam();
		}









		double minValue, maxValue, valueDecl;
		unsigned int minIndex, maxIndex;
		frequencyData.MinMax(minValue, minIndex, maxValue, maxIndex);
		valueDecl=(maxValue-minValue)*0.1;
		/*m_newGraph.GetPlots().RemoveAll();*/
		CNiColor blue(40,104,197);
		/*m_newGraph.GetPlots().Add();*/
		m_newGraph.Plots.Item(1).LineStyle = CNiPlot::LineNone;
		m_newGraph.GetPlots().Item(1).LineColor=blue;			
		m_newGraph.GetPlots().Item(1).PlotY(frequencyData, 0, deltaFreq);
		if (isFreqRangeChg==TRUE)
		{
			m_newGraph.Axes.Item("XAxis").SetMinMax(FreqXmin,FreqXmax);		
		}
		else
			m_newGraph.Axes.Item("XAxis").SetMinMax(0,freqRange);

		m_newGraph.Axes.Item("YAxis-1").SetMinMax(0,maxValue+valueDecl);

		
		SetQuaternionF();
		isFreqRangeChg = FALSE;

		//Invalidate();//拉伸x区域，重新进行FFT计算，刷新整个绘图区后，图形标题不见了//Marshall  2011.2.21 16:25


		/*--------------------------Marshall's Code2011.2.21 16:24--------------*/
		//只刷新部分区域
		//CWnd* pWave = GetDlgItem(IDC_WAVE);
		//CRect rc_wave;
		//pWave->GetWindowRect(&rc_wave);
		//ScreenToClient(&rc_wave);
		//InvalidateRect(&rc_wave);

		//
		//pWave = GetDlgItem(IDC_FRAQ);
		//CRect rc_freq;
		//ScreenToClient(&rc_freq);
		//pWave->GetWindowRect(&rc_freq);
		//InvalidateRect(&rc_freq);

		TextData ();



		CRect rc;
		GetClientRect(&rc);
		CRect rect(rc.Width()-200,20,rc.Width(),rc.Height());
		InvalidateRect(&rect);///上面注释掉的代码也可以，不过从安全角度考虑，用这部分代码//Marshall  2011.2.21 16:34
		//Invalidate();
		/*--------------------------Marshall's Code2011.2.21 16:24--------------*/

	}

}
//设置波形图范围	
//@min	最小值		
//@max	最大值
void CNiWaveFraqView::SetWaveRange(int min,int max)    //cxy   cxy cxy
{
	m_Graph.Axes.Item("XAxis").SetMinMax(waveWidthMin,waveWidthMax);
}
//设置通道数据各层名称	 
//@file		文件名		
//@exmName	实验名称	
//@type		试验类型 
//@param	试验参数		
//@chn		通道名
void CNiWaveFraqView::SetChnDataStr(CString file,CString exmName, CString type, CString param,CString chn)
{
	FileStr=file;
	ExamNameStr=exmName;
	TypeStr=type;
	ParamStr=param;
	chnStr=chn;
}


/*--------------------------Marshall's Code2011.2.21 18:47--------------*/
//extern bool g_bConf97;//放到文档开始的地方，便于前面其它函数用//Marshall  2011.2.24 10:50
//extern bool g_bConf95;
//extern bool g_bConf100;

int  _cdecl  /*CNiWaveFraqView::*/Compare(const void* a,const void* b);

int GetLowHi( CNiReal64Vector& niData, int nLow,double& dLow, double& dHi)
{
	int nSize = niData.GetSize();
	/*CNiReal64Vector niTmp;
	niTmp.SetSize(nSize);
	niTmp = niData;*/

	double dTmp = niData[0];
	for(int i = 0; i <= nLow ; i++)
		for(int j = i + 1; j < nSize; j++)
		{
			if(niData[i] > niData[j])
			{
				dTmp = niData[j];
				niData[j] = niData[i];
				niData[i] = dTmp;
			}
		}

		dLow = niData[nLow];

	//for(int i = 0; i <= nLow ; i++)
	//	for(int j = i + 1; j < nSize; j++)
	//	{
	//		if(niTmp[i] < niTmp[j])
	//		{
	//			dTmp = niTmp[j];
	//			niTmp[j] = niTmp[i];
	//			niTmp[i] = dTmp;
	//		}
	//	}

	//	dHi = niTmp[nLow];

		return 0;
}

/*static*/ double CNiWaveFraqView::GetConf(const CNiReal64Vector& niWaveData, double dPercent)
{

	//clock_t beginTime, endTime;
	//beginTime = clock();
	int nSize = niWaveData.GetSize();

	double dTail = (100-dPercent)/2.0;

	int nLow=nSize*dTail/100;
	int nHi=nSize*(dPercent + dTail)/100;

	CNiReal64Vector niTmp;
	niTmp.SetSize(nSize);
	niTmp = niWaveData;

	//clock_t beginTime, endTime;
	//beginTime = clock();
	qsort(niTmp,nSize,niWaveData.GetElementSize(),Compare);	////Marshall  2011.2.23 16:2
	double dLow = 0.0, dHi = 0.0;

	//GetLowHi(niTmp,nLow,dLow,dHi);//

	//endTime = clock();
	//double  duration;
	//duration = (double)(endTime - beginTime) / CLOCKS_PER_SEC;

	//FILE* file;////Marshall  2011.2.26 10:7
	//file = fopen("c:\\2.txt","a+");
	//if(file == NULL)
	//	return 0;
	//
	//fprintf(file,"size = %d, cost time = %f\n",nSize,duration);
	//fclose(file);

	double conf = 0;
	conf = niTmp[nHi] - niTmp[nLow];
	//conf = dHi - dLow;
 
	return conf;
}


int  _cdecl  /*CNiWaveFraqView::*/Compare(const void* a,const void* b)
{
	double Temp1,Temp2;
	Temp1=*(double*)a,Temp2=*(double*)b;
	if(Temp1>Temp2)
		return 1;
	else if(Temp1<Temp2)
		return -1;
	else
		return 0;
}
/*--------------------------Marshall's Code2011.2.21 18:47--------------*/
//处理新的数据和新的绘图区域
void CNiWaveFraqView::SetWaveParam()
{
	//GetQuaternionF();

	int xMax=floor(m_Graph.Axes.Item("XAxis").GetMaximum()/m_deltaFreq);

	int xMin=ceil(m_Graph.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);

	int round=xMax-xMin;
	CNiReal64Vector newData(round);
	
	if (round>16)
	{
		if (xMin>=0&&xMax<=(int)NiData.GetSize())
		{
			for (int i=0;i<round;i++)
			{
				newData[i]=NiData[xMin];
				xMin++;
			}
		}
		else
		{
			for (int i=0;i<round;i++)
			{
				newData[i]=0;				
			}
		}
		double minValue, maxValue;
		unsigned int minIndex, maxIndex;

		newData.MinMax(minValue, minIndex, maxValue, maxIndex);
	    
		waveParam.startPoint=ceil(m_Graph.Axes.Item("XAxis").GetMinimum()/m_deltaFreq);
		waveParam.endPoint=floor(m_Graph.Axes.Item("XAxis").GetMaximum()/m_deltaFreq);
		waveParam.simpling=samplingFreq;
		waveParam.timeStart=(m_Graph.Axes.Item("XAxis").GetMinimum());
		waveParam.timeEnd=(m_Graph.Axes.Item("XAxis").GetMaximum());



		waveParam.aMax=maxValue;
		waveParam.aMin=minValue;
       


		if(g_bConf97)
			//waveParam.ppValue=97;
			waveParam.ppValue = GetConf(newData,97);
		else if(g_bConf95)
			//waveParam.ppValue=95;
			waveParam.ppValue = GetConf(newData,95);
		else if(g_bConf100)
			//waveParam.ppValue=100;
			waveParam.ppValue = maxValue-minValue;


	
		CNiMath::Mean(newData,waveParam.mean);	
		CNiMath::Median(newData,waveParam.median);

		CNiReal64Vector m_temp_Data(round);
		for (int i=0;i<round;i++)
		{
			m_temp_Data[i]=newData[i]-waveParam.mean;
		}
		CNiMath::RMS(m_temp_Data,waveParam.RMS);   //cxy 20110512
		waveParam.RMS=waveParam.RMS*2*1.41421356;
	 
	}
	//Invalidate();

}


//设置频谱图范围
//@min	最小值		
//@max	最大值
void CNiWaveFraqView::SetFreqRange(int xmin,int xmax)
{
	isFreqRangeChg=TRUE;
	FreqXmax=xmax;
	FreqXmin=xmin;
	//m_newGraph.Axes.Item("XAxis").SetMinimum(xmin);
	//m_newGraph.Axes.Item("XAxis").SetMaximum(xmax);
}

//消息：输出到excel报表
void CNiWaveFraqView::OnExcelreport()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//数据存储路径
	CString savePath;
	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath==_T(""))
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("轴心轨迹图形操作：取消输出到excel报表");
		return;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：输出到excel报表");

		CNiExcelApplication m_application(false);
		CNiExcelWorkbook m_workBook(m_application.NewWorkbook());
		CNiExcelWorksheet ws(m_workBook.NewWorksheet(1));
		ws.SetName("波形频谱图");

		CString ExcelHeadArray [] = {"点数","时间", "频率","最大值","最小值","平均值","中间值","有效值","峰峰值"};/*(秒)(赫兹)*///加上置信度的标记//Marshall  2011.2.24 10:56
		/*--------------------------Marshall's Code2011.2.24 10:53--------------*/
		if(g_bConf97)
			 ExcelHeadArray [8] = "97%峰峰值";
		else if(g_bConf95)
			 ExcelHeadArray [8] = "95%峰峰值";
		else if(g_bConf100)
			 ExcelHeadArray [8] = "100%峰峰值";
		/*--------------------------Marshall's Code2011.2.24 10:53--------------*/

		CNiExcelCellRange titleRange = ws.GetRange("A1:I1");
		titleRange.FontBold=true;
		titleRange.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
		titleRange.WriteDataFromArray(ExcelHeadArray,1,9);

		CString tData [9];
		CString temp;
		temp.Format("%d",waveParam.endPoint-waveParam.startPoint);
		tData[0]=temp;
		temp.Format("%.3lf",waveParam.timeEnd-waveParam.timeStart);
		tData[1]=temp;
		temp.Format("%d",waveParam.simpling);
		tData[2]=temp;
		temp.Format("%.2lf",waveParam.aMax);
		tData[3]=temp;
		temp.Format("%.2lf",waveParam.aMin);
		tData[4]=temp;
		temp.Format("%.2lf",waveParam.mean);
		tData[5]=temp;
		temp.Format("%.2lf",waveParam.median);
		tData[6]=temp;
		temp.Format("%.2lf",waveParam.RMS);
		tData[7]=temp;
		temp.Format("%.2lf",waveParam.ppValue);
		tData[8]=temp;

		CNiExcelCellRange tDataRange = ws.GetRange("A2:I2");
		//tDataRange.FontBold=true;
		tDataRange.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
		tDataRange.WriteDataFromArray(tData,1,9);

		if ((int)ChnFftFlag==1)
		{
			CString hNewTitles [] = {"1","2", "3","4","5","6","7","8"};/*(秒)(赫兹)*/

			CNiExcelCellRange newtitleRange = ws.GetRange("A6:A14");
			newtitleRange.FontBold=true;
			newtitleRange.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
			newtitleRange.WriteDataFromArray(hNewTitles,1,8);

			CString r1[]={"频率"};
			CNiExcelCellRange Range1 = ws.GetRange("B5:B5");
			Range1.FontBold=true;
			Range1.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
			Range1.WriteDataFromArray(r1,1,1);

			CString r2[]={"幅值"};
			CNiExcelCellRange Range2 = ws.GetRange("C5:C5");
			Range2.FontBold=true;
			Range2.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
			Range2.WriteDataFromArray(r2,1,1);

			CString r3[]={"相位"};
			CNiExcelCellRange Range3 = ws.GetRange("D5:D5");
			Range3.FontBold=true;
			Range3.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
			Range3.WriteDataFromArray(r3,1,1);

			CNiReal64Matrix dm;
			dm.SetSize(8,3);

			for (int k=0;k<8;k++)
			{
				CString ntemp;
				ntemp.Format(" %.3f",fftVectorDesc[k].frequency);
				dm(k,0)=fftVectorDesc[k].frequency;
				ntemp.Format(" %.3f",fftVectorDesc[k].amplitude);
				dm(k,1)=fftVectorDesc[k].amplitude;
				ntemp.Format(" %.3f",fftVectorDesc[k].phase);
				dm(k,2)=fftVectorDesc[k].phase;

			}	

			
			/*--------------------------Marshall's Code2011.3.1 15:25--------------*/
			//注释掉，不画出图表//Marshall  2011.3.1 15:26
			CNiExcelCellRange RangeDm = ws.GetRange("B6:D14");
			//RangeDm.FontBold=true;
			RangeDm.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
			RangeDm.WriteDataFromArray(dm);

			//CNiExcelChart chartSheet1 = ws.AddChart(10, 200, 400, 250);

			//chartSheet1.ChartWizard(1, "B5:C13", NiExcelChartWizardGalleryLine, NiExcelColumns, 1, 1, true, "频率-幅值", "频率", "幅值", "");
			//chartSheet1.ChartType=NiExcelChartTypesXYScatterSmooth;//NiExcelChartTypesXYScatterLines

			//CNiExcelChart chartSheet2 = ws.AddChart(450, 200, 400, 250);

			//chartSheet2.ChartWizard(1, "B5:B13,D5:D13", NiExcelChartWizardGalleryLine, NiExcelColumns, 1, 1, true, "频率-相位", "频率", "相位", "");
			//chartSheet2.ChartType=NiExcelChartTypesXYScatterSmooth;
			/*--------------------------Marshall's Code2011.3.1 15:25--------------*/
		}

		//-------------------1.0.0.2------------------------//
		CTime ctm;
		CString timeStr;
		ctm=CTime::GetCurrentTime();
		timeStr=ctm.Format("%Y-%m-%d %H-%M-%S");

		//DeleteFile(savePath+"\\"+chnStr+" excel报表"+".xls");
		m_workBook.Save(savePath+"\\"+chnStr+" excel报表  "+timeStr+".xls",CNiExcelWorkbook::FileFormatExcel7);

		m_application.Quit();
		AfxMessageBox("存储完毕！");	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存完成，存储为　"+savePath+"\\"+chnStr+" excel报表  "+timeStr+".xls");
		//------------------------------------------------//
	}
}

//判断word版本
void CNiWaveFraqView::GetWordVision()
{
	//int wordVision=0;//word类型，0：word2003		1：word2007

	HKEY hKEY; //定义有关的 hKE
	LPCTSTR data_Set0="SOFTWARE\\Microsoft\\Office\\12.0\\Word\\InstallRoot"; 
	//打开与路径 data_Set 相关的 hKEY，第一个参数为根键名称，第二个参数表。 
	//表示要访问的键的位置，第三个参数必须为0，KEY_READ表示以查询的方式。 
	//访问注册表，hKEY则保存此函数所打开的键的句柄。 
	long ret0=RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set0, 0, KEY_READ, &hKEY); 
	if(ret0==ERROR_SUCCESS) //如果无法打开hKEY，则终止程序的执行 
	{
		wordVision=0;
	} 

	//LPCTSTR data_Set1="SOFTWARE\\Microsoft\\Office\\11.0\\Word\\InstallRoot"; 
	//long ret1=RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set1, 0, KEY_READ, &hKEY); 
	//if(ret1==ERROR_SUCCESS) //如果无法打开hKEY，则终止程序的执行 
	else
	{
		wordVision=1;
	} 


	RegCloseKey(hKEY);
}
//消息：输出到word文档
void CNiWaveFraqView::OnWordreport()
{
	GetWordVision();
	OnWavefraqgrap();
	UpdateWindow(); 
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//数据存储路径
	CString savePath;
	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath==_T(""))
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消输出数据到word文档");
		return;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：输出数据到word文档...");

		CNiWordApplication application(FALSE);

		CNiWordDocument document = application.CreateDocument();

		document.SetHeader("", title, "", CNiWordDocument::FieldDate, CNiWordDocument::FieldEmpty, CNiWordDocument::FieldTime);

		WriteToWord(document,wordVision);
		document.SetFooter("","","",CNiWordDocument::FieldEmpty, CNiWordDocument::FieldPage, CNiWordDocument::FieldEmpty);

		//DeleteFile(savePath+"\\"+chnStr+" word报表"+".doc");
		//-------------------1.0.0.2------------------------//
		CTime ctm;
		CString timeStr;
		ctm=CTime::GetCurrentTime();
		timeStr=ctm.Format("%Y-%m-%d %H-%M-%S");

		document.SaveAs(savePath+"\\"+chnStr+" word报表  "+timeStr+".doc");
		document.Close();
		application.Quit();	
		AfxMessageBox("存储完毕！");	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存完成，存储为　"+savePath+"\\"+chnStr+" word报表  "+timeStr+".doc");
		//--------------------------------------------------//
	}



}

//写出word文档
//@CNiWordDocument document		word文档对象
//@int wordVision;//word类型，0：word2003		1：word2007

void CNiWaveFraqView::WriteToWord(CNiWordDocument document, int wordVision)
{	
	this->GetParentFrame()->SetWindowText(title);

	CString   waveReport="";

	waveReport.Append(chnStr);
	waveReport.Append("波形数据");

	document.FontBold = true;
	document.FontSize = 16;
	document.AppendText("波形频谱图表");
	document.SetTextAlign(CNiWordDocument::AlignCenter);
	document.NewLine();	


	document.FontBold = false;
	document.FontUnderLine=true;
	document.FontSize = 12;
	document.AppendText("通道："+waveReport);
	document.SetTextAlign(CNiWordDocument::AlignLeft);
	document.NewLine();
	document.NewLine();
	document.SetTextAlign(CNiWordDocument::AlignCenter);
	document.FontUnderLine=false;
	document.FontBold = true;
	document.FontSize = 10;
	document.AppendText("图-1.  波形频谱图");
	document.NewLine();
	while (!SaveWaveTemp())
	{
		SaveWaveTemp();
	}
	
	//CNiWordImage NiWaveImage=
	if (wordVision==0)
	{
		document.InsertImage("C:\\WINDOWS\\waveTemp.gif");
	}
	if (wordVision==1)
	{
		CNiWordImage NiWaveImage=document.InsertImage("C:\\WINDOWS\\waveTemp.gif");
		//NiWaveImage.WrapText(CNiWordImage::WrapTopBottom,CNiWordImage::WrapBoth);
		NiWaveImage.MoveImage( 4.5,-3.65);
	}


	//CNiWordTable picTable = document.AddTable(0, 0);
	//
	//picTable.InsertImageInCell("C:\\WINDOWS\\waveTemp.gif");

	//NiWaveImage.WrapText(CNiWordImage::WrapNone,CNiWordImage::WrapBoth);
	//NiWaveImage.MoveImage( 4.5,-3.65);
	for (int p=0;p<15;p++)	
		document.NewLine();

	//document.FontBold = true;DeleteFile("C:\\waveTemp.gif");	
	//document.AppendText("频谱图");
	////SaveFraqTemp();


	//DeleteFile("C:\\fraqTemp.gif");

	//document.NewLine();


	document.NewLine();
	document.AppendText("表-1.  波形特征");

	document.NewLine();
	document.FontBold = false;
	//document.NewLine();
	//document.SetTextAlign(CNiWordDocument::AlignLeft);

	CNiWordTable table = document.AddTable(2, 9);

   //table.
	//CString discussion="discussion";
	//document.AppendLine(discussion);
	//document.NewLine();
	document.NewLine();
	//document.FontBold = true;
	if ((int)ChnFftFlag==1)
	{
		document.FontBold = true;
		document.AppendText("表-2.  频谱特征");

		document.NewLine();
		document.FontBold = false;
		CNiWordTable newTable = document.AddTable(4, 9);
		document.NewLine();

		document.AddBookMark("Table2");
		CString hNewTitles [] = {"1","2", "3","4","5","6","7","8"};/*(秒)(赫兹)*/
		for(int k=0;k<8;k++)
		{
			newTable.WriteToCell(1,k+2,hNewTitles[k]);
		}
		newTable.WriteToCell(2,1,"频率");
		newTable.WriteToCell(3,1,"幅值");
		newTable.WriteToCell(4,1,"相位");

		for (int k=0;k<(int)fftVectorDesc.size();k++)
		{
			CString ntemp;
			ntemp.Format(" %.3f",fftVectorDesc[k].frequency);
			newTable.WriteToCell(2,k+2,ntemp);
			ntemp.Format(" %.3f",fftVectorDesc[k].amplitude);
			newTable.WriteToCell(3,k+2,ntemp);
			ntemp.Format(" %.3f",fftVectorDesc[k].phase);
			newTable.WriteToCell(4,k+2,ntemp);
			newTable.SetColumnWidth(k+2, .68);
		}	

		document.GoToBookMark("Table2");
	}
	else
	{
		for (int p=0;p<6;p++)
		{
			document.NewLine();
		}
	}

	document.FontSize = 12;
	document.FontBold = true;
	document.AppendText("分析与结论：");
	document.SetTextAlign(CNiWordDocument::AlignLeft);
	document.NewLine();
	document.NewLine();
	document.NewLine();
	document.NewLine();
	document.NewLine();
	document.NewLine();
	document.NewLine();
	document.AppendText("													  															      签字：______________");


	document.AddBookMark("Table1");
	CString hTitles [] = {"点数","时间", "频率","最大值","最小值","平均值","中间值","有效值","峰峰值"};/*(秒)(赫兹)*///加上置信度的标记//Marshall  2011.2.24 10:56

	/*--------------------------Marshall's Code2011.2.24 10:53--------------*/
		if(g_bConf97)
			 hTitles [8] = "97%峰峰值";
		else if(g_bConf95)
			 hTitles [8] = "95%峰峰值";
		else if(g_bConf100)
			 hTitles [8] = "100%峰峰值";
	/*--------------------------Marshall's Code2011.2.24 10:53--------------*/
	CString tData [9];
	CString temp;
	temp.Format("%d",waveParam.endPoint-waveParam.startPoint+1);
	tData[0]=temp;
	temp.Format("%.3lf",waveParam.timeEnd-waveParam.timeStart);
	tData[1]=temp;
	temp.Format("%d",waveParam.simpling);
	tData[2]=temp;
	temp.Format("%.2lf",waveParam.aMax);
	tData[3]=temp;
	temp.Format("%.2lf",waveParam.aMin);
	tData[4]=temp;
	temp.Format("%.2lf",waveParam.mean);
	tData[5]=temp;
	temp.Format("%.2lf",waveParam.median);
	tData[6]=temp;
	temp.Format("%.2lf",waveParam.RMS);
	tData[7]=temp;
	temp.Format("%.2lf",waveParam.ppValue);
	tData[8]=temp;
	for(int k=0;k<9;k++)
	{
		table.WriteToCell(1,k+1,hTitles[k]);
		table.WriteToCell(2,k+1,tData[k]);
		table.SetColumnWidth(k+1, .68);
	}
	document.GoToBookMark("Table1");


	//DeleteFile("C:\\WINDOWS\\waveTemp.gif");	
}

//消息：写出数据到文件
void CNiWaveFraqView::OnTextsave()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//数据存储路径
	CString savePath;
	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath==_T(""))
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消保存当前波形数据到文件");
		return;
	}
	else
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存当前波形数据到文件...");
		//-------------------1.0.0.2------------------------//
		CTime ctm;
		CString timeStr;
		ctm=CTime::GetCurrentTime();
		timeStr=ctm.Format("%Y-%m-%d %H-%M-%S");

		FILE	*fp = 0;
		//fp = fopen(savePath+"\\"+chnStr+".chd","w");
		fp = fopen(savePath+"\\"+chnStr+"  "+timeStr+".chd","w");
		//--------------------------------------------------//
		if(fp==0)return;

		for(int i=0;i<(int)NiData.GetSize();i++)
		{
			fprintf(fp,"%lf\n",NiData[i]); 
			//fprintf(fp,"[%d]=%6.2f;\n",i,ori_ptr[i]); 

		}
		fseek(fp,0,SEEK_END);
		fclose(fp);

		AfxMessageBox("存储完毕！");	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存完成，存储为　"+savePath+"\\"+chnStr+"  "+timeStr+".chd");

	}
}

//消息：保存当前波形数据到文件
void CNiWaveFraqView::OnSavetxtdata()
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	int treeClass = 1,i=0;
	CString m_ParName[2];
	for(i=0;i<2;i++)
	{
		m_ParName[i] = _T("");
	}	
	SaveDataDlg sBD;
	HTREEITEM   m_selectedItem   =   pCMF->m_wndWorkSpace.m_wndTree.GetSelectedItem();   
	//sBD.SaveFileType=1;
	CString m_SelName = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
	m_ParName[0]=m_SelName;
	HTREEITEM hItem = pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
	if (pCMF->m_wndWorkSpace.m_wndTree.GetRootItem()==pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(hItem))
	{
	while(hItem!=pCMF->m_wndWorkSpace.m_wndTree.GetRootItem())
	{
		treeClass++;
		m_ParName[treeClass-1] = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
		hItem = pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(hItem);

	}
	CString s=m_ParName[treeClass-1];

	//fileData
	CString ct=pCMF->fileData->GetCout(s);
	int p=0;
	CString strPath;
	map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
	for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
	{
		p++;
		if (p==atoi(ct))
		{
			strPath=fullPath->second;			
		}
	}
	sBD.filePath=strPath;
	sBD.examType=pCMF->fileData->RemoveCoutFromString(m_ParName[0]);
	sBD.examName=pCMF->fileData->RemoveCoutFromString(m_ParName[1]);
	if(pCMF->m_wndWorkSpace.m_wndTree.ItemHasChildren(m_selectedItem))   
	{   
		HTREEITEM    hNextItem;   
		HTREEITEM    hChildItem    =  pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(m_selectedItem);   

		while    (hChildItem    !=    NULL)   
		{ 

			CString m_Name = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hChildItem);

			CString mName=pCMF->fileData->RemoveCoutFromString(m_Name);
			sBD.SetExamParam(mName);
			HTREEITEM    hCNextItem;
			HTREEITEM    hChildCItem    =  pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(hChildItem);
			int k=1000;
			while    (hChildCItem    !=    NULL)  
			{
				k++;
				CString cout;
				cout.Format("#%d",k);
				CString m_CName = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hChildCItem);
				sBD.SetCHNnameBool(pCMF->fileData->RemoveCoutFromString(m_Name)+cout,pCMF->fileData->RemoveCoutFromString(m_CName),TRUE);

				hCNextItem    =    pCMF->m_wndWorkSpace.m_wndTree.GetNextItem(hChildCItem,    TVGN_NEXT); 
				hChildCItem    =    hCNextItem;
			}
			//

			hNextItem    =    pCMF->m_wndWorkSpace.m_wndTree.GetNextItem(hChildItem,    TVGN_NEXT); 
			hChildItem    =    hNextItem;	

		}   
	}
	sBD.DoModal();

	}
	else
	{
		if (pCMF->fileData->fileTitle.size()!=0)
		{
			pCMF->m_wndWorkSpace.m_wndTree.SetFocus();
			HTREEITEM root=pCMF->m_wndWorkSpace.m_wndTree.GetRootItem();
			HTREEITEM rFirst=pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(root);
			HTREEITEM rFFirst=pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);			
			pCMF->m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);
			OnSavetxtdata();
		}
	}
}
//消息：批量存储为图像文件
void CNiWaveFraqView::OnSavepicbat()
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();

	int treeClass = 1,i=0;
	CString m_ParName[2];
	for(i=0;i<2;i++)
	{
		m_ParName[i] = _T("");
	}	
	SaveImageBatDlg sBD;
	HTREEITEM   m_selectedItem   =   pCMF->m_wndWorkSpace.m_wndTree.GetSelectedItem();   
	
	

	CString m_SelName = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
	m_ParName[0]=m_SelName;
	HTREEITEM hItem = pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
	if (pCMF->m_wndWorkSpace.m_wndTree.GetRootItem()==pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(hItem))
	{
	while(hItem!=pCMF->m_wndWorkSpace.m_wndTree.GetRootItem())
	{
		treeClass++;
		m_ParName[treeClass-1] = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
		hItem = pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(hItem);

	}
	CString s=m_ParName[treeClass-1];

	//fileData
	CString ct=pCMF->fileData->GetCout(s);
	int p=0;
	CString strPath;
	map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
	for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
	{
		p++;
		if (p==atoi(ct))
		{
			strPath=fullPath->second;			
		}
		//fileNameComb.InsertString(0,*fullPath);		
	}
	sBD.filePath=strPath;
	sBD.examType=pCMF->fileData->RemoveCoutFromString(m_ParName[0]);
	sBD.examName=pCMF->fileData->RemoveCoutFromString(m_ParName[1]);
	//sBD.examName=fileData->RemoveCoutFromString(m_ParName[2]);//examParam

	if(pCMF->m_wndWorkSpace.m_wndTree.ItemHasChildren(m_selectedItem))   
	{   
		HTREEITEM    hNextItem;   
		HTREEITEM    hChildItem    =  pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(m_selectedItem);   

		while    (hChildItem    !=    NULL)   
		{ 
			
			CString m_Name = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hChildItem);

			CString mName=pCMF->fileData->RemoveCoutFromString(m_Name);
			sBD.SetExamParam(mName);
			HTREEITEM    hCNextItem;
			HTREEITEM    hChildCItem    =  pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(hChildItem);
			int k=1000;
			while    (hChildCItem    !=    NULL)  
			{
				k++;
				CString cout;
				cout.Format("#%d",k);
				CString m_CName = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hChildCItem);
				sBD.SetCHNnameBool(pCMF->fileData->RemoveCoutFromString(m_Name)+cout,pCMF->fileData->RemoveCoutFromString(m_CName),TRUE);
				
				hCNextItem    =    pCMF->m_wndWorkSpace.m_wndTree.GetNextItem(hChildCItem,    TVGN_NEXT); 
				hChildCItem    =    hCNextItem;
			}
			//

			hNextItem    =    pCMF->m_wndWorkSpace.m_wndTree.GetNextItem(hChildItem,    TVGN_NEXT); 
			hChildItem    =    hNextItem;	

		}   
	}
	sBD.DoModal();
	}
	else
	{
		if (pCMF->fileData->fileTitle.size()!=0)
		//{
		//	AfxMessageBox("尚未加载文件");
		//	OnFileopendata();
		//}
		//else
		{
			//AfxMessageBox("请选择正确的试验类型");
			pCMF->m_wndWorkSpace.m_wndTree.SetFocus();
			HTREEITEM root=pCMF->m_wndWorkSpace.m_wndTree.GetRootItem();
			HTREEITEM rFirst=pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(root);
			HTREEITEM rFFirst=pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);			
			pCMF->m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);
			OnSavepicbat();
		}
	}

}

//消息：批量存储为Excel文件
void CNiWaveFraqView::OnSaveexceldata()
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	int treeClass = 1,i=0;
	CString m_ParName[2];
	for(i=0;i<2;i++)
	{
		m_ParName[i] = _T("");
	}	
	HTREEITEM   m_selectedItem   =   pCMF->m_wndWorkSpace.m_wndTree.GetSelectedItem();   
	//sBD.SaveFileType=2;
	CString m_SelName = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
	m_ParName[0]=m_SelName;
	HTREEITEM hItem = pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);

	if (pCMF->m_wndWorkSpace.m_wndTree.GetRootItem()==pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(hItem))
	{
		CSaveCHNToExcel cte;

		while(hItem!=pCMF->m_wndWorkSpace.m_wndTree.GetRootItem())
		{
			treeClass++;
			m_ParName[treeClass-1] = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
			hItem = pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(hItem);

		}
		CString s=m_ParName[treeClass-1];

		//fileData
		CString ct=pCMF->fileData->GetCout(s);
		int p=0;
		CString strPath;
		map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
		for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
		{
			p++;
			if (p==atoi(ct))
			{
				strPath=fullPath->second;			
			}			
		}
		pCMF->fileData->SetExamTree(strPath);
		cte.examName=pCMF->fileData->RemoveCoutFromString(m_ParName[1]);
		cte.examType=pCMF->fileData->RemoveCoutFromString(m_ParName[0]);
		cte.filePath=strPath;
		cte.paramVector=pCMF->fileData->GetExamParam(strPath,pCMF->fileData->RemoveCoutFromString(m_ParName[0]));
		
		cte.DoModal();
	}
	else
	{
		if (pCMF->fileData->fileTitle.size()!=0)
		//{
		//	AfxMessageBox("尚未加载文件");
		//	pCMF->OnFileopendata();
		//}
		//else
		{
			//AfxMessageBox("请选择正确的试验类型");
			pCMF->m_wndWorkSpace.m_wndTree.SetFocus();
			HTREEITEM root=pCMF->m_wndWorkSpace.m_wndTree.GetRootItem();
			HTREEITEM rFirst=pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(root);
			HTREEITEM rFFirst=pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);			
			pCMF->m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);
			/*pCMF->m_wndWorkSpace.m_application.Quit();*/
			OnSaveexceldata();
		}
	}
}

//批量存储为word报表
void CNiWaveFraqView::OnSaveworddata()
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();

	int treeClass = 1,i=0;
	CString m_ParName[2];
	for(i=0;i<2;i++)
	{
		m_ParName[i] = _T("");
	}	
	CSaveCHNWord scw;
	HTREEITEM   m_selectedItem   =   pCMF->m_wndWorkSpace.m_wndTree.GetSelectedItem();   



	CString m_SelName = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
	m_ParName[0]=m_SelName;
	HTREEITEM hItem = pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
	if (pCMF->m_wndWorkSpace.m_wndTree.GetRootItem()==pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(hItem))
	{
		while(hItem!=pCMF->m_wndWorkSpace.m_wndTree.GetRootItem())
		{
			treeClass++;
			m_ParName[treeClass-1] = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
			hItem = pCMF->m_wndWorkSpace.m_wndTree.GetParentItem(hItem);

		}
		CString s=m_ParName[treeClass-1];

		//fileData
		CString ct=pCMF->fileData->GetCout(s);
		int p=0;
		CString strPath;
		map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
		for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
		{
			p++;
			if (p==atoi(ct))
			{
				strPath=fullPath->second;			
			}
			//fileNameComb.InsertString(0,*fullPath);		
		}
		scw.filePath=strPath;
		scw.examType=pCMF->fileData->RemoveCoutFromString(m_ParName[0]);
		scw.examName=pCMF->fileData->RemoveCoutFromString(m_ParName[1]);
		//sBD.examName=fileData->RemoveCoutFromString(m_ParName[2]);//examParam

		if(pCMF->m_wndWorkSpace.m_wndTree.ItemHasChildren(m_selectedItem))   
		{   
			HTREEITEM    hNextItem;   
			HTREEITEM    hChildItem    =  pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(m_selectedItem);   

			while    (hChildItem    !=    NULL)   
			{ 

				CString m_Name = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hChildItem);

				CString mName=pCMF->fileData->RemoveCoutFromString(m_Name);
				scw.SetExamParam(mName);
				HTREEITEM    hCNextItem;
				HTREEITEM    hChildCItem    =  pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(hChildItem);
				int k=1000;
				while    (hChildCItem    !=    NULL)  
				{
					k++;
					CString cout;
					cout.Format("#%d",k);
					CString m_CName = pCMF->m_wndWorkSpace.m_wndTree.GetItemText(hChildCItem);
					scw.SetCHNnameBool(pCMF->fileData->RemoveCoutFromString(m_Name)+cout,pCMF->fileData->RemoveCoutFromString(m_CName),TRUE);

					hCNextItem    =    pCMF->m_wndWorkSpace.m_wndTree.GetNextItem(hChildCItem,    TVGN_NEXT); 
					hChildCItem    =    hCNextItem;
				}
				//

				hNextItem    =    pCMF->m_wndWorkSpace.m_wndTree.GetNextItem(hChildItem,    TVGN_NEXT); 
				hChildItem    =    hNextItem;	

			}   
		}
		scw.DoModal();
	}
	else
	{
		if (pCMF->fileData->fileTitle.size()!=0)
		{
			//AfxMessageBox("请选择正确的试验类型");
			pCMF->m_wndWorkSpace.m_wndTree.SetFocus();
			HTREEITEM root=pCMF->m_wndWorkSpace.m_wndTree.GetRootItem();
			HTREEITEM rFirst=pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(root);
			HTREEITEM rFFirst=pCMF->m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);			
			pCMF->m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);
			OnSaveworddata();
		}
	}
}


//设置健相数据
//@vector<float> kp		健相点
void CNiWaveFraqView::SetKeyPhase(std::vector<float> kp,int key_freq/*,int wave_freq*/)
{
	
	//KeyPhase.SetSize((int)kp.size());
	/*vector<float> kPhaseX;*/

	/////////////////////////////////////
	int /*key_freq=8192,*/wave_freq=samplingFreq;
	int d_num = key_freq/wave_freq;
	if(key_freq%wave_freq!=0) d_num += 1;
	if(d_num<=0) d_num = 1;
	if ((int)kp.size()==0)
	{
		isHasKeyPhase=FALSE;
	}
	else
	{
		if (isGraph)
		{
			isHasKeyPhase=TRUE;
			for (int k=0;k<(int)kp.size();k++)
			{
				int kpCout=0;
				if (k%key_freq==0)
				{
					kpCout=(int)kp[k];
					for (int i=1;i<=kpCout;i++)
					{
						float x=k+kp[k+i];
						x /= d_num;
						if(x<NiData.GetSize())
						{
							kPhaseX.push_back(x);
							kPhaseY.push_back((float)NiData[(int)x]);
						}
					}			
				}
			}

		}
		else
		{
			isHasKeyPhase=false;
		}
		
	}

/*
	CString strTemp;
	strTemp.Format("%d",kPhaseX.getSize());
	AfxMessageBox(strTemp);
*/
	/////////////////////////////////////

	/*if ((int)kp.size()==0)
	{
		isHasKeyPhase=FALSE;
	}
	else
	{
		isHasKeyPhase=TRUE;

		for (int k=0;k<(int)kp.size();k++)
		{
			int kpCout=0;
			if (k%samplingFreq==0)
			{
				kpCout=(int)kp[k];
				for (int i=1;i<=kpCout;i++)
				{
					float x=k+kp[k+i];
					kPhaseX.push_back(x);
					kPhaseY.push_back((float)NiData[(int)x]);
				}			
			}
		}	
	}*/

}

//消息：是否仅显示波形图

void CNiWaveFraqView::OnUpdateWavegraph(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!isWavegraph);
}
//消息：是否仅显示频谱图
void CNiWaveFraqView::OnUpdateFraqgraph(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!isFreqgraph);
}
//消息：是否显示波形频谱图
void CNiWaveFraqView::OnUpdateWavefraqgrap(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!isWaveFreq);
}
//波形图标注函数//Marshall  2011.2.24 21:14
void CNiWaveFraqView::OnAnnotation()
{
	CSetTitle dlg;
	dlg.mTitle = _T("");
	if(IDOK == dlg.DoModal())
	{
		CString strAntName = dlg.mTitle;//标注名称//Marshall  2011.2.24 21:22

		if( m_Graph.Cursors.GetCount() <= 0)
		{
			MessageBox("请先用光标定位要标注的位置(拾取临近点模式)！","提示",MB_ICONEXCLAMATION);
			return;
		}

		CNiCursor cursorTmp = m_Graph.Cursors.Item(1);

		if(cursorTmp.SnapMode != CNiCursor::SnapNearestPoint)
		{
			MessageBox("请将光标定位到曲线上！","提示",MB_ICONEXCLAMATION);
			return;
		}


		double	x = cursorTmp.XPosition;
		double	y = cursorTmp.YPosition;

		CNiAnnotation annotation = m_Graph.Annotations.Add();
		annotation.SetBuiltinStyle(CNiAnnotation::TextAndArrow);
		annotation.Enabled  = true;
		annotation.Name = strAntName;
		CNiPlot plot = m_Graph.Plots.Item(1);
		annotation.SnapMode = CNiAnnotation::SnapPointsOnPlot;
		annotation.Plot = plot;

		CNiColor clr(0, 0, 255);
		annotation.PointIndex = cursorTmp.PointIndex;

		annotation.SnapMode = CNiAnnotation::SnapAnchoredToPoint;

		annotation.Visible = true;
		annotation.CoordinateType = CNiAnnotation::AxesCoordinates ;
		CNiArrow arrow = annotation.Arrow;
		arrow.HeadStyle = CNiArrow::Stealth;
		arrow.LineStyle = CNiArrow::LineSolid;
		arrow.Width = 1;
		arrow.Color = clr;
		arrow.Visible = true;
		CNiCaption caption = annotation.Caption;
		caption.Font.Size = m_fontSize;
		caption.Text = strAntName;
		caption.Color = clr;
		caption.XCoordinate = x;
		caption.YCoordinate = y;
		CNiShape shape = annotation.Shape;
		shape.Color = CNiColor(255, 0, 0);
		shape.PointStyle = CNiShape::PointAsterisk ;
		shape.FillVisible = true;
		shape.LineWidth = 2;
		
		m_Graph.TrackMode = CNiGraph::TrackDragAnnotation;
	}

	//m_Graph.TrackMode = CNiGraph::TrackDragAnnotation;
}

//保存当前波形时段的数据
void CNiWaveFraqView::OnCurrTextdata()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//数据存储路径
	CString savePath;
	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath==_T(""))
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消保存当前时段波形数据到文件");
		return;
	}
	else
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存当前时段波形数据到文件...");
		//-------------------1.0.0.2------------------------//
		CTime ctm;
		CString timeStr;
		ctm=CTime::GetCurrentTime();
		timeStr=ctm.Format("%Y-%m-%d %H-%M-%S");

		FILE	*fp = 0;
		//fp = fopen(savePath+"\\"+chnStr+".chd","w");
		//fp = fopen(savePath+"\\"+chnStr+"  "+timeStr+".chd","w");
		fp = fopen(savePath+"\\"+chnStr+"  " + "当前时段波形" +timeStr+ ".chd","w");
		//--------------------------------------------------//
		if(fp==0)return;

		for(int i=0;i<(int)newData.GetSize();i++)
		{
			//fprintf(fp,"%lf\n",NiData[i]); 
			fprintf(fp,"%lf\n",newData[i]); //保存当前时段波形//Marshall  2011.3.1 14:39
			//fprintf(fp,"[%d]=%6.2f;\n",i,ori_ptr[i]); 

		}
		fseek(fp,0,SEEK_END);
		fclose(fp);

		AfxMessageBox("存储完毕！");	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存当前时段波形完成，存储为　"+savePath+"\\"+chnStr+"  "+ "当前时段波形" +timeStr+".chd");

	}
}

//保存当前频谱分析结果
void CNiWaveFraqView::OnFreqTextdata()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//数据存储路径
	CString savePath;
	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath==_T(""))
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消保存当前频谱分析结果到文件");
		return;
	}
	else
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存当前频谱分析结果到文件...");
		//-------------------1.0.0.2------------------------//
		CTime ctm;
		CString timeStr;
		ctm=CTime::GetCurrentTime();
		timeStr=ctm.Format("%Y-%m-%d %H-%M-%S");

		FILE	*fp = 0;
		//fp = fopen(savePath+"\\"+chnStr+".chd","w");
		//fp = fopen(savePath+"\\"+chnStr+"  "+timeStr+".chd","w");
		fp = fopen(savePath+"\\"+chnStr+"  " + "当前频谱分析结果" +timeStr+ ".chd","w");
		//--------------------------------------------------//
		if(fp==0)return;

		//CString strTemp;
		//strTemp = _T("序号\t"频率" "\t" "幅值" "\t" "相位");
        


		fprintf(fp,"%s\t%s\n", _T("起始时间"),_T("终止时间")); 


		fprintf(fp,"%lf\t%lf\n\n",
			waveParam.timeStart,
			waveParam.timeEnd); //保存当前时段波形//Marshall  2011.3.1 14:39

		fprintf(fp,"%s\t%s\t\t%s\t\t%s\n", _T("序号"),_T("频率"),_T("幅值"),_T("相位")); 

		for(int i=0; i<8; i++)
		{
			//fprintf(fp,"%lf\n",NiData[i]); 
			fprintf(fp,"%d\t%lf\t%lf\t%lf\n",
				i+1,
				fftVectorDesc[i].frequency,
				fftVectorDesc[i].amplitude,
				fftVectorDesc[i].phase); //保存当前时段波形//Marshall  2011.3.1 14:39
			//fprintf(fp,"[%d]=%6.2f;\n",i,ori_ptr[i]); 

		}
		fseek(fp,0,SEEK_END);
		fclose(fp);

		AfxMessageBox("存储完毕！");	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：保存当前频谱分析结果完成，存储为　"+savePath+"\\"+chnStr+"  "+ "当前时段波形" +timeStr+".chd");

	}
}

void CNiWaveFraqView::OnTimeSpan()
{


	   CWaveAnalysisTimeSpanSetDlg dlg;

	   double m_1=0.0;
	   double m_2=double(ResumeData.GetSize())*m_deltaFreq;


       
	   dlg.m_begin=m_1;
	   dlg.m_end=m_2;
	   //dlg.DoModal();
	   //dlg.UpdateData(false);

	   if(dlg.DoModal()==IDCANCEL)
	   {

		       isFullPeriodFFT=true;

			  if (dlg.m_begin<m_2&&dlg.m_end>dlg.m_begin)
			  {
			   m_1=dlg.m_begin>m_1?dlg.m_begin:m_1;
			   m_2=dlg.m_end<m_2?dlg.m_end:m_2;
			  
			   
		   		m_Graph.Axes.Item("XAxis").SetMinMax(m_1,m_2);
		   		SetQuaternion();
		  		FFTProc();
				TextData();
			  }
			  else
			  {
				  AfxMessageBox("请设置合理的分析时段区间!");
			  }

	   }
	   




}

void CNiWaveFraqView::OnCancelAnnotations()
{
	// TODO: 在此添加命令处理程序代码;


	
	//if (m_Graph.Annotations.GetCount()>0)
	 	m_Graph.Annotations.RemoveAll();


	
}

void CNiWaveFraqView::OnAdjustAnnotationPlace()
{
	if (!isAnnotation)
	{
		m_Graph.TrackMode = CNiGraph::TrackDragAnnotation;
		isAnnotation=true;
	}
	else
	{
		isAnnotation=false;
		m_Graph.TrackMode = CNiGraph::TrackZoomRectXWithUndo;

	}

	
}

void CNiWaveFraqView::OnUpdateAdjustAnnotationPlace(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isAnnotation);	
}

void CNiWaveFraqView::OnWindowSet()
{
	CThrowLoadSetDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		
		m_width=dlg.m_width;
		m_increase=1;
		AddTrendWave(1,m_width,1,RGB (255, 0, 255));
		isThrowload=true;
		isFullPeriodFFT=true;

	}
	
}
