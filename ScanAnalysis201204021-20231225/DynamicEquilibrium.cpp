// DynamicEquilibrium.cpp : 实现文件
//

#include "stdafx.h"
#include "HSJLayoutBridge.h"
#include "ScanAnalysis.h"
#include "DynamicEquilibrium.h"
#include "DynamicEquilibriumSetDLG.h"
#include "MainFrm.h"
#include <math.h>
//#define  mPI 3.1415926535897932384626433832795

// CDynamicEquilibrium
using namespace std;
IMPLEMENT_DYNCREATE(CDynamicEquilibrium, CFormView)

//const int intermetallicSize=256;

CDynamicEquilibrium::CDynamicEquilibrium()
	: CFormView(CDynamicEquilibrium::IDD),m_bClockwise(false)
	, m_beforeBalanceWeight(0)
	, m_afterBalanceWeight(0),ASamplingFreq(1024),BSamplingFreq(1024),intermetallicSize(0),isHasKeyPhase(FALSE),
	BAmplitude(0),BPhase(0),AAmplitude(0),APhase(0),tWAngle(0),tBAngle(0),outWeight(0),outAngle(0),isBDiss(FALSE),isWeightVisual(FALSE)
{

	m_AFlag=false;
	m_BFlag=false;

	m_deltaFreq_B=1.0;
	m_deltaFreq_A=1.0;
	m_deltaFreq_Key=1.0;


}

CDynamicEquilibrium::~CDynamicEquilibrium()
{
}

void CDynamicEquilibrium::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH1, m_Graph);
	DDX_Control(pDX, IDC_CWGRAPH2, m_DynEquGraph);
	DDX_Control(pDX, IDC_CWGRAPH3, m_newGraph);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	//DDX_Control(pDX, IDC_DynEquWAVE, m_DynEquPic);
	DDX_Control(pDX, IDC_COMBO1, m_rotateTypeComb);
	DDX_Text(pDX, IDC_BSP, m_beforeBalanceWeight);
	DDX_Text(pDX, IDC_ASP, m_afterBalanceWeight);

}

BEGIN_MESSAGE_MAP(CDynamicEquilibrium, CFormView)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDynamicEquilibrium::OnCbnSelchangeCombo1)
	ON_COMMAND(ID_LoadBBWData, &CDynamicEquilibrium::OnLoadbbwdata)
	ON_COMMAND(ID_LoadABWData, &CDynamicEquilibrium::OnLoadabwdata)
	ON_COMMAND(ID_DeleteBSel, &CDynamicEquilibrium::OnDeleteBSel)
	ON_COMMAND(ID_LoadABWData, &CDynamicEquilibrium::OnLoadabwdata)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDynamicEquilibrium::OnNMClickList1)
	ON_COMMAND(ID_DeleteASel, &CDynamicEquilibrium::OnDeleteasel)
	ON_COMMAND(ID_SaveBWImage, &CDynamicEquilibrium::OnSavebwimage)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID__BAnalysis, &CDynamicEquilibrium::ONBAnalysis)
	ON_COMMAND(ID__AAnalysis, &CDynamicEquilibrium::ONAAnalysis)
END_MESSAGE_MAP()

CNiReal64Vector BSX(2);
CNiReal64Vector BSY(2);
CNiReal64Vector BEX(2);
CNiReal64Vector BEY(2);

CNiReal64Vector ASX(2);
CNiReal64Vector ASY(2);
CNiReal64Vector AEX(2);
CNiReal64Vector AEY(2);

// CDynamicEquilibrium 诊断

#ifdef _DEBUG
void CDynamicEquilibrium::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDynamicEquilibrium::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDynamicEquilibrium 消息处理程序

int CDynamicEquilibrium::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if(pMainFrame)
	{
		pMainFrame->m_DynEqu = this;
	}

	return 0;
}

HBRUSH CDynamicEquilibrium::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (HSJSetWhiteControlColor(pDC, pWnd, nCtlColor)) return HSJWhiteBrush();
    return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CDynamicEquilibrium::OnInitialUpdate()
{
	
	CFormView::OnInitialUpdate();
	HSJStyleChildControls(GetSafeHwnd(), m_skinFonts);
	HSJStyleGraph(m_Graph);
	HSJStyleGraph(m_newGraph);
	HSJStyleGraph(m_DynEquGraph);


	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/
	//调整界面，不出现滚动条////Marshall  2011.3.2 12:23
	//SetScrollSizes(MM_TEXT,CSize(0,0));
	////InitNiGraph();
	//ResizeParentToFit();
	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/

	m_rotateTypeComb.InsertString(0,"顺时针旋转");
	m_rotateTypeComb.InsertString(1,"逆时针旋转");
	m_rotateTypeComb.SetCurSel(0);
	
	m_Graph.SetWindowText("配重前");
	m_newGraph.SetWindowText("配重后");
	m_DynEquGraph.SetWindowText("动平衡试验分析");
	IsertDataInit();



	HSJApplyViewLayout(this, 0, 1, 1);
	FitHeaderLabels();
}


// The legacy header labels are 8 DLU high. Fit the new typeface vertically
// without moving label origins, color keys, graphs or the results table.
void CDynamicEquilibrium::FitHeaderLabels()
{
    CRect graphBounds;
    m_Graph.GetWindowRect(&graphBounds);
    ScreenToClient(&graphBounds);
    if (graphBounds.top <= 0) return;

    for (CWnd* child = GetWindow(GW_CHILD); child; child = child->GetNextWindow()) {
        TCHAR className[32] = {};
        ::GetClassName(child->GetSafeHwnd(), className, 32);
        if (lstrcmpi(className, _T("Static")) != 0) continue;
        const DWORD type = child->GetStyle() & SS_TYPEMASK;
        if (type != SS_LEFT && type != SS_CENTER && type != SS_RIGHT) continue;

        CRect bounds;
        child->GetWindowRect(&bounds);
        ScreenToClient(&bounds);
        if (bounds.top < 0 || bounds.top >= graphBounds.top) continue;

        CClientDC dc(child);
        CFont* font = child->GetFont();
        if (!font) continue;
        CFont* previous = dc.SelectObject(font);
        TEXTMETRIC metrics = {};
        dc.GetTextMetrics(&metrics);
        dc.SelectObject(previous);

        const int maximum = graphBounds.top - bounds.top - 2;
        const int required = metrics.tmHeight + 2;
        const int height = min(maximum, max(bounds.Height(), required));
        if (height > bounds.Height())
            child->SetWindowPos(NULL, 0, 0, bounds.Width(), height,
                SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void CDynamicEquilibrium::IsertDataInit()
{
	m_wndList.SetColumnHeader(_T(" , 100; 转速(转/分), 150; 总振值(双峰), 150;转频幅值(双峰), 150;转频相位(°), 150; 配重重量(kg), 150; 配重角度(°), 150"));

	IDX0 = m_wndList.InsertItem(0, _T(""));
	m_wndList.SetItemText(IDX0, 0, "原始参数");

	IDX1 = m_wndList.InsertItem(1, _T(""));
	m_wndList.SetItemText(IDX1, 0, "试重参数");

	IDX2 = m_wndList.InsertItem(2, _T(""));
	m_wndList.SetItemText(IDX2, 0, "配重方案");

	IDX3 = m_wndList.InsertItem(3, _T(""));
	m_wndList.SetItemText(IDX3, 0, "分解1");

	IDX4 = m_wndList.InsertItem(4, _T(""));
	m_wndList.SetItemText(IDX4, 0, "分解2");

	m_wndList.SetGridLines(TRUE); // SHow grid lines

	m_wndList.SetEditable(TRUE); // Allow sub-text edit
	m_wndList.SetSortable(FALSE);
	UpdateData(FALSE);

	m_wndList.SetItemTextColor(-1, 2, RGB(0, 0, 0));
	m_wndList.SetItemBkColor(-1, 0, RGB(192, 192, 192));
	m_wndList.SetItemBkColor(0, 5, RGB(223, 223, 223));
	m_wndList.SetItemBkColor(0, 6, RGB(223, 223, 223));
	for(int k=1;k<5;k++)
		for (int t=2;t<5;t++)
		{
			m_wndList.SetItemBkColor(t, k, RGB(223, 223, 223));
		}
	m_wndList.SetItemBkColor(2, 5, RGB(255, 255, 200));
	m_wndList.SetItemBkColor(2, 6, RGB(255, 255, 200));
	m_wndList.SetItemBkColor(3, 5, RGB(255, 255, 200));
	m_wndList.SetItemBkColor(4, 5, RGB(255, 255, 200));
}
void CDynamicEquilibrium::OnDraw(CDC* /*pDC*/)
{
	FillRect();
}
void CDynamicEquilibrium::DrawDynEquGraph(double bR,double aR,double bA,double aA,double tW,double tB,double di1,double di2)
{

/*	int i=0;
	CNiReal64Vector xdata(100);
	CNiReal64Vector ydata(100);

	double radius=bR>aR?bR:aR;

	for (i=0;i<100;i++)
	{
		xdata[i]=radius*(sin(2.05*i*mPI*0.01));
		ydata[i]=radius*(cos(2.05*i*mPI*0.01));
	}

	CNiReal64Vector xacurdata(2);
	CNiReal64Vector yacurdata(2);

	if (m_bClockwise==FALSE)
	{
		xacurdata[0]=(radius+radius/10.0)*(sin(19*mPI*0.01+mPI/10));
		yacurdata[0]=(radius+radius/10.0)*(cos(19*mPI*0.01+mPI/10));

		xacurdata[1]=(radius+radius/10.0)*(sin(19*mPI*0.01+mPI/10))+(radius/10.0)*(sin(19*mPI*0.01+mPI/10+3*mPI/4));
		yacurdata[1]=(radius+radius/10.0)*(cos(19*mPI*0.01+mPI/10))-(radius/10.0)*(cos(19*mPI*0.01+mPI/10+3*mPI/4));
		bA=-bA;
		aA=-aA;
		tW=-tW;
		tB=-tB;
		di1=-di1;
		di2=-di2;
	}
	else
	{
		xacurdata[0]=(radius+radius/10.0)*(sin(mPI/10));
		yacurdata[0]=(radius+radius/10.0)*(cos(mPI/10));

		xacurdata[1]=(radius+radius/10.0)*(sin(mPI/10))+(radius/10.0)*(sin(mPI/10+mPI/2));
		yacurdata[1]=(radius+radius/10.0)*(cos(mPI/10))-(radius/10.0)*(cos(mPI/10+mPI/2));
	}

	m_DynEquGraph.Plots.RemoveAll();

	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(1).SetLineColor(RGB(0,0,0));
	m_DynEquGraph.Plots.Item(1).PlotXvsY(xdata,ydata);
	
	m_DynEquGraph.Annotations.Add();
	m_DynEquGraph.Annotations.Item(1).Plot=m_DynEquGraph.Plots.Item(1);
	m_DynEquGraph.Annotations.Item(1).Caption.Text="0°";
	m_DynEquGraph.Annotations.Item(1).Caption.SetCoordinates(7.0*m_DynEquGraph.Axes.Item("XAxis").Maximum/8.0,0.0);
	m_DynEquGraph.Annotations.Item(1).Caption.Color=RGB(0,0,255);
	
	m_DynEquGraph.Annotations.Add();
	m_DynEquGraph.Annotations.Item(2).Plot=m_DynEquGraph.Plots.Item(1);
	m_DynEquGraph.Annotations.Item(2).Caption.Text="90°";
	if (m_bClockwise==FALSE)
		m_DynEquGraph.Annotations.Item(2).Caption.SetCoordinates((m_DynEquGraph.Axes.Item("XAxis").Maximum+m_DynEquGraph.Axes.Item("XAxis").Minimum)/2.0,9.0*(m_DynEquGraph.Axes.Item("YAxis-1").Maximum)/10);
	else
		m_DynEquGraph.Annotations.Item(2).Caption.SetCoordinates((m_DynEquGraph.Axes.Item("XAxis").Maximum+m_DynEquGraph.Axes.Item("XAxis").Minimum)/2.0,9.0*(m_DynEquGraph.Axes.Item("YAxis-1").Minimum)/10);

	m_DynEquGraph.Annotations.Item(2).Caption.Color=RGB(0,0,255);
	
	CNiReal64Vector oVibrationLineX(2);
	CNiReal64Vector oVibrationLineY(2);
	oVibrationLineX[0]=0;
	oVibrationLineX[1]=bR*(sin(bA*mPI/180+mPI/2));
	oVibrationLineY[0]=0;
	oVibrationLineY[1]=bR*(cos(bA*mPI/180+mPI/2));

	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(2).SetPointColor(RGB(255,0,0));
	m_DynEquGraph.Plots.Item(2).SetLineColor(RGB(255,0,0));
	m_DynEquGraph.Plots.Item(2).SetPointStyle(CNiPlot::PointSolidCircle);
	m_DynEquGraph.Plots.Item(2).PlotXvsY(oVibrationLineX,oVibrationLineY);

	CNiReal64Vector tVibrationLineX(2);
	CNiReal64Vector tVibrationLineY(2);
	tVibrationLineX[0]=0;
	tVibrationLineX[1]=aR*(sin(aA*mPI/180+mPI/2));
	tVibrationLineY[0]=0;
	tVibrationLineY[1]=aR*(cos(aA*mPI/180+mPI/2));

	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(3).SetPointColor(RGB(0,0,255));
	m_DynEquGraph.Plots.Item(3).SetLineColor(RGB(0,0,255));
	m_DynEquGraph.Plots.Item(3).SetPointStyle(CNiPlot::PointSolidCircle);
	m_DynEquGraph.Plots.Item(3).PlotXvsY(tVibrationLineX,tVibrationLineY);

	CNiReal64Vector tWeightBlockX(1);
	CNiReal64Vector tWeightBlockY(1);
	tWeightBlockX[0]=radius*(sin(tW*mPI/180+mPI/2));
	tWeightBlockY[0]=radius*(cos(tW*mPI/180+mPI/2));

	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(4).SetPointColor(RGB(0,128,128));
	m_DynEquGraph.Plots.Item(4).SetLineWidth(5);
	m_DynEquGraph.Plots.Item(4).SetPointStyle(CNiPlot::PointDottedEmptySquare);

	
	CNiReal64Vector tBalanceBlockX(1);
	CNiReal64Vector tBalanceBlockY(1);
	tBalanceBlockX[0]=radius*(sin(tB*mPI/180+mPI/2));
	tBalanceBlockY[0]=radius*(cos(tB*mPI/180+mPI/2));

	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(5).SetPointColor(RGB(255,0,255));
	m_DynEquGraph.Plots.Item(5).SetLineWidth(5);
	m_DynEquGraph.Plots.Item(5).SetPointStyle(CNiPlot::PointDottedEmptySquare);
	
	if (isWeightVisual)
	{
		m_DynEquGraph.Plots.Item(4).PlotXvsY(tWeightBlockX,tWeightBlockY);
		m_DynEquGraph.Plots.Item(5).PlotXvsY(tBalanceBlockX,tBalanceBlockY);
	}


	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(6).SetPointColor(RGB(0,0,0));
	m_DynEquGraph.Plots.Item(6).SetPointStyle(CNiPlot::PointSolidCircle);
	m_DynEquGraph.Plots.Item(6).PlotXY(0,0);

	CNiReal64Vector xcurdata(20);
	CNiReal64Vector ycurdata(20);

	for (i=0;i<20;i++)
	{
		xcurdata[i]=(radius+radius/10.0)*(sin(i*mPI*0.01+mPI/10));
		ycurdata[i]=(radius+radius/10.0)*(cos(i*mPI*0.01+mPI/10));
	}

	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(7).SetLineColor(RGB(0,0,255));
	m_DynEquGraph.Plots.Item(7).PlotXvsY(xcurdata,ycurdata);


	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(8).SetLineColor(RGB(0,0,255));
	m_DynEquGraph.Plots.Item(8).PlotXvsY(xacurdata,yacurdata);

	if (isBDiss)
	{
		CNiReal64Vector tDissX1(1);
		CNiReal64Vector tDissY1(1);
		tDissX1[0]=radius*(sin(di1*mPI/180+mPI/2));
		tDissY1[0]=radius*(cos(di1*mPI/180+mPI/2));

		m_DynEquGraph.Plots.Add();
		m_DynEquGraph.Plots.Item(9).SetPointColor(RGB(255,180,255));
		m_DynEquGraph.Plots.Item(9).SetLineWidth(5);
		m_DynEquGraph.Plots.Item(9).SetPointStyle(CNiPlot::PointDottedEmptySquare);
		m_DynEquGraph.Plots.Item(9).PlotXvsY(tDissX1,tDissY1);

		CNiReal64Vector tDissX2(1);
		CNiReal64Vector tDissY2(1);
		tDissX2[0]=radius*(sin(di2*mPI/180+mPI/2));
		tDissY2[0]=radius*(cos(di2*mPI/180+mPI/2));

		m_DynEquGraph.Plots.Add();
		m_DynEquGraph.Plots.Item(10).SetPointColor(RGB(255,200,255));
		m_DynEquGraph.Plots.Item(10).SetLineWidth(5);
		m_DynEquGraph.Plots.Item(10).SetPointStyle(CNiPlot::PointDottedEmptySquare);
		m_DynEquGraph.Plots.Item(10).PlotXvsY(tDissX2,tDissY2);
	}


	m_DynEquGraph.Axes.Item("XAxis").SetMinMax(-radius-radius/5.0,radius+radius/5.0);
	m_DynEquGraph.Axes.Item("YAxis-1").SetMinMax(-radius-radius/5.0,radius+radius/5.0);
*/

int i=0;
CNiReal64Vector xdata(100);
CNiReal64Vector ydata(100);

double radius=bR>aR?bR:aR;

for (i=0;i<100;i++)
{
	xdata[i]=radius*(sin(2.05*i*mPI*0.01));
	ydata[i]=radius*(cos(2.05*i*mPI*0.01));
}

CNiReal64Vector xacurdata(2);
CNiReal64Vector yacurdata(2);

if (m_bClockwise==true)
{
	
	bA=-bA;
	aA=-aA;
	tW=-tW;
	tB=-tB;
	di1=-di1;
	di2=-di2;


	xacurdata[0]=(radius+radius/10.0)*(sin(mPI/10));
	yacurdata[0]=(radius+radius/10.0)*(cos(mPI/10));

	xacurdata[1]=(radius+radius/10.0)*(sin(mPI/10))+(radius/10.0)*(sin(mPI/10+mPI/2));
	yacurdata[1]=(radius+radius/10.0)*(cos(mPI/10))-(radius/10.0)*(cos(mPI/10+mPI/2));
}
else
{
	xacurdata[0]=(radius+radius/10.0)*(sin(19*mPI*0.01+mPI/10));
	yacurdata[0]=(radius+radius/10.0)*(cos(19*mPI*0.01+mPI/10));

	xacurdata[1]=(radius+radius/10.0)*(sin(19*mPI*0.01+mPI/10))+(radius/10.0)*(sin(19*mPI*0.01+mPI/10+3*mPI/4));
	yacurdata[1]=(radius+radius/10.0)*(cos(19*mPI*0.01+mPI/10))-(radius/10.0)*(cos(19*mPI*0.01+mPI/10+3*mPI/4));

	
}

m_DynEquGraph.Plots.RemoveAll();

m_DynEquGraph.Plots.Add();
m_DynEquGraph.Plots.Item(1).SetLineColor(RGB(0,0,0));
m_DynEquGraph.Plots.Item(1).PlotXvsY(xdata,ydata);

m_DynEquGraph.Annotations.Add();
m_DynEquGraph.Annotations.Item(1).Plot=m_DynEquGraph.Plots.Item(1);
m_DynEquGraph.Annotations.Item(1).Caption.Text="∠0°";
m_DynEquGraph.Annotations.Item(1).Caption.SetCoordinates(7.0*m_DynEquGraph.Axes.Item("XAxis").Maximum/8.0,0.0);
m_DynEquGraph.Annotations.Item(1).Caption.Color=RGB(0,0,255);

m_DynEquGraph.Annotations.Add();
m_DynEquGraph.Annotations.Item(2).Plot=m_DynEquGraph.Plots.Item(1);
m_DynEquGraph.Annotations.Item(2).Caption.Text="∠90°";
if (m_bClockwise==TRUE)
m_DynEquGraph.Annotations.Item(2).Caption.SetCoordinates((m_DynEquGraph.Axes.Item("XAxis").Maximum+m_DynEquGraph.Axes.Item("XAxis").Minimum)/2.0,9.0*(m_DynEquGraph.Axes.Item("YAxis-1").Maximum)/10);
else
m_DynEquGraph.Annotations.Item(2).Caption.SetCoordinates((m_DynEquGraph.Axes.Item("XAxis").Maximum+m_DynEquGraph.Axes.Item("XAxis").Minimum)/2.0,9.0*(m_DynEquGraph.Axes.Item("YAxis-1").Minimum)/10);

m_DynEquGraph.Annotations.Item(2).Caption.Color=RGB(0,0,255);


CNiReal64Vector oVibrationLineX(2);
CNiReal64Vector oVibrationLineY(2);
oVibrationLineX[0]=0;
oVibrationLineX[1]=bR*(sin(bA*mPI/180+mPI/2));
oVibrationLineY[0]=0;
oVibrationLineY[1]=bR*(cos(bA*mPI/180+mPI/2));

m_DynEquGraph.Plots.Add();
m_DynEquGraph.Plots.Item(2).SetPointColor(RGB(255,0,0));
m_DynEquGraph.Plots.Item(2).SetLineColor(RGB(255,0,0));
m_DynEquGraph.Plots.Item(2).SetPointStyle(CNiPlot::PointSolidCircle);
m_DynEquGraph.Plots.Item(2).PlotXvsY(oVibrationLineX,oVibrationLineY);

CNiReal64Vector tVibrationLineX(2);
CNiReal64Vector tVibrationLineY(2);
tVibrationLineX[0]=0;
tVibrationLineX[1]=aR*(sin(aA*mPI/180+mPI/2));
tVibrationLineY[0]=0;
tVibrationLineY[1]=aR*(cos(aA*mPI/180+mPI/2));

m_DynEquGraph.Plots.Add();
m_DynEquGraph.Plots.Item(3).SetPointColor(RGB(0,0,255));
m_DynEquGraph.Plots.Item(3).SetLineColor(RGB(0,0,255));
m_DynEquGraph.Plots.Item(3).SetPointStyle(CNiPlot::PointSolidCircle);
m_DynEquGraph.Plots.Item(3).PlotXvsY(tVibrationLineX,tVibrationLineY);

CNiReal64Vector tWeightBlockX(1);
CNiReal64Vector tWeightBlockY(1);
tWeightBlockX[0]=radius*(sin(tW*mPI/180+mPI/2));
tWeightBlockY[0]=radius*(cos(tW*mPI/180+mPI/2));

m_DynEquGraph.Plots.Add();
m_DynEquGraph.Plots.Item(4).SetPointColor(RGB(0,128,128));
m_DynEquGraph.Plots.Item(4).SetLineWidth(5);
m_DynEquGraph.Plots.Item(4).SetPointStyle(CNiPlot::PointDottedEmptySquare);


CNiReal64Vector tBalanceBlockX(1);
CNiReal64Vector tBalanceBlockY(1);
tBalanceBlockX[0]=radius*(sin(tB*mPI/180+mPI/2));
tBalanceBlockY[0]=radius*(cos(tB*mPI/180+mPI/2));

m_DynEquGraph.Plots.Add();
m_DynEquGraph.Plots.Item(5).SetPointColor(RGB(255,0,255));
m_DynEquGraph.Plots.Item(5).SetLineWidth(5);
m_DynEquGraph.Plots.Item(5).SetPointStyle(CNiPlot::PointDottedEmptySquare);

if (isWeightVisual)
{
	m_DynEquGraph.Plots.Item(4).PlotXvsY(tWeightBlockX,tWeightBlockY);
	m_DynEquGraph.Plots.Item(5).PlotXvsY(tBalanceBlockX,tBalanceBlockY);
}


m_DynEquGraph.Plots.Add();
m_DynEquGraph.Plots.Item(6).SetPointColor(RGB(0,0,0));
m_DynEquGraph.Plots.Item(6).SetPointStyle(CNiPlot::PointSolidCircle);
m_DynEquGraph.Plots.Item(6).PlotXY(0,0);

CNiReal64Vector xcurdata(20);
CNiReal64Vector ycurdata(20);

for (i=0;i<20;i++)
{
	xcurdata[i]=(radius+radius/10.0)*(sin(i*mPI*0.01+mPI/10));
	ycurdata[i]=(radius+radius/10.0)*(cos(i*mPI*0.01+mPI/10));
}

m_DynEquGraph.Plots.Add();
m_DynEquGraph.Plots.Item(7).SetLineColor(RGB(0,0,255));
m_DynEquGraph.Plots.Item(7).PlotXvsY(xcurdata,ycurdata);


m_DynEquGraph.Plots.Add();
m_DynEquGraph.Plots.Item(8).SetLineColor(RGB(0,0,255));
m_DynEquGraph.Plots.Item(8).PlotXvsY(xacurdata,yacurdata);

if (isBDiss)
{
	CNiReal64Vector tDissX1(1);
	CNiReal64Vector tDissY1(1);
	tDissX1[0]=radius*(sin(di1*mPI/180+mPI/2));
	tDissY1[0]=radius*(cos(di1*mPI/180+mPI/2));

	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(9).SetPointColor(RGB(255,180,255));
	m_DynEquGraph.Plots.Item(9).SetLineWidth(5);
	m_DynEquGraph.Plots.Item(9).SetPointStyle(CNiPlot::PointDottedEmptySquare);
	m_DynEquGraph.Plots.Item(9).PlotXvsY(tDissX1,tDissY1);

	CNiReal64Vector tDissX2(1);
	CNiReal64Vector tDissY2(1);
	tDissX2[0]=radius*(sin(di2*mPI/180+mPI/2));
	tDissY2[0]=radius*(cos(di2*mPI/180+mPI/2));

	m_DynEquGraph.Plots.Add();
	m_DynEquGraph.Plots.Item(10).SetPointColor(RGB(255,200,255));
	m_DynEquGraph.Plots.Item(10).SetLineWidth(5);
	m_DynEquGraph.Plots.Item(10).SetPointStyle(CNiPlot::PointDottedEmptySquare);
	m_DynEquGraph.Plots.Item(10).PlotXvsY(tDissX2,tDissY2);
}


m_DynEquGraph.Axes.Item("XAxis").SetMinMax(-radius-radius/5.0,radius+radius/5.0);
m_DynEquGraph.Axes.Item("YAxis-1").SetMinMax(-radius-radius/5.0,radius+radius/5.0);


//Invalidate(TRUE);

	//Invalidate(TRUE);
}
void CDynamicEquilibrium::FillRect()
{
	CWnd* pWnd=GetDlgItem(IDC_RED);
	CDC* pDC=pWnd->GetDC();	
	pWnd->UpdateWindow();

	pDC->FillSolidRect(0,0,30,15,RGB(255,0,0));	
	pWnd=GetDlgItem(IDC_BLUE);
	pWnd->UpdateWindow();
	pDC=pWnd->GetDC();
	pDC->FillSolidRect(0,0,30,15,RGB(0,0,255));
	pWnd=GetDlgItem(IDC_BLACK);
	pWnd->UpdateWindow();
	pDC=pWnd->GetDC();
	pDC->FillSolidRect(0,0,30,15,RGB(0,128,128));
	pWnd=GetDlgItem(IDC_PURPLE);
	pWnd->UpdateWindow();
	pDC=pWnd->GetDC();
	pDC->FillSolidRect(0,0,30,15,RGB(255,0,255));
	pWnd=GetDlgItem(IDC_PURPLE2);
	pWnd->UpdateWindow();
	pDC=pWnd->GetDC();
	pDC->FillSolidRect(0,0,30,15,RGB(255,180,255));
	ReleaseDC(pDC);

}

void CDynamicEquilibrium::OnSize(UINT nType, int cx, int cy)
{
	// Responsive layout and a scrollable minimum canvas.
	if (HSJApplyViewLayout(this, 0, cx, cy)) return;
	CFormView::OnSize(nType, cx, cy);

	CWnd *pWnd; 

	pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		pWnd->MoveWindow(0,30,cx-cy+120,(cy-120)/2);//设置控件大小
	}	
	pWnd = GetDlgItem(IDC_CWGRAPH3);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		pWnd->MoveWindow(0,cy/2-30,cx-cy+120,(cy-120)/2);//设置控件大小
	}
	pWnd = GetDlgItem(IDC_CWGRAPH2);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		pWnd->MoveWindow(cx-cy+120,30,cy-120,cy-120);//设置控件大小
	}
	pWnd = GetDlgItem(IDC_LIST1);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		// Keep the list inside the view so its native vertical scrollbar is visible.
		pWnd->MoveWindow(0,cy-120+30,cx,90);//设置控件大小
	}
}
BEGIN_EVENTSINK_MAP(CDynamicEquilibrium, CFormView)
	ON_EVENT(CDynamicEquilibrium, IDC_CWGRAPH1, DISPID_DBLCLICK, CDynamicEquilibrium::DblClickCwgraph1, VTS_NONE)
	ON_EVENT(CDynamicEquilibrium, IDC_CWGRAPH1, 1, CDynamicEquilibrium::CursorChangeCwgraph1, VTS_PI4 VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CDynamicEquilibrium, IDC_CWGRAPH3, 1, CDynamicEquilibrium::CursorChangeCwgraph3, VTS_PI4 VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CDynamicEquilibrium, IDC_CWGRAPH3, DISPID_DBLCLICK, CDynamicEquilibrium::DblClickCwgraph3, VTS_NONE)
	ON_EVENT(CDynamicEquilibrium, IDC_CWGRAPH1, DISPID_MOUSEUP, CDynamicEquilibrium::MouseUpCwgraph1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDynamicEquilibrium, IDC_CWGRAPH1, DISPID_MOUSEDOWN, CDynamicEquilibrium::MouseDownCwgraph1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CDynamicEquilibrium::OnCbnSelchangeCombo1()
{
	if(m_rotateTypeComb.GetCurSel()==0)
	{
		m_bClockwise=false;
	}
	else
	{
		m_bClockwise=true;
	}
	GetAParams();
	UpdateData(FALSE);
}
//加载配重前通道数据
void CDynamicEquilibrium::OnLoadbbwdata()
{
	CDynamicEquilibriumSetDLG desd(this);
	desd.DataType=0;
	desd.title="加载配重前通道数据";
	if(desd.DoModal()==IDOK)
	{
		if (isHasKeyPhase)
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：加载配重前通道数据...");
	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：取消加载配重前通道数据");

}
//加载配重后通道数据
void CDynamicEquilibrium::OnLoadabwdata()
{
	CDynamicEquilibriumSetDLG desd(this);
	desd.DataType=1;
	desd.title="加载配重后通道数据";
	if(desd.DoModal()==IDOK)
	{
		if (isHasKeyPhase)
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：加载配重后通道数据...");
	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：取消加载配重后通道数据");

}

void CDynamicEquilibrium::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;

	VERIFY(menu.LoadMenu (IDR_BalanceWeight));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x, point.y, AfxGetMainWnd());

	menu.Detach();
}

void CDynamicEquilibrium::SetBBWData(vector <float> vData,int sFreq,CString t)
{
	BSamplingFreq=sFreq;

	CNiReal64Vector NiDt((unsigned int)vData.size());
	vector<float>::iterator it2 = vData.begin();
	int k=-1;
	for(; it2 != vData.end(); it2++)
	{
		k++;
		NiDt[k]=*it2;
	}

	BBWData=NiDt;
	m_Graph.SetWindowText(t);



	m_deltaFreq_B =1.0/ BSamplingFreq; 


}


void CDynamicEquilibrium::SetABWData(vector <float> vData,int sFreq,CString t)
{
	ASamplingFreq=sFreq;

	CNiReal64Vector NiDt((unsigned int)vData.size());
	vector<float>::iterator it2 = vData.begin();
	int k=-1;
	for(; it2 != vData.end(); it2++)
	{
		k++;
		NiDt[k]=*it2;
	}

	ABWData=NiDt;
	m_newGraph.SetWindowText(t);


	m_deltaFreq_A =1.0/ ASamplingFreq; 
	
}

void CDynamicEquilibrium::SetKeyPhaseA(std::vector<float> kp,int key_freq,int samplingFreq)
{

	//if ((int)kp.size()==0)
	//{
	//	isHasKeyPhase=FALSE;
	//	AfxMessageBox("无健相信息，无法完成动平衡试验！");
	//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("无健相信息，无法完成动平衡试验！");
	//	return;
	//}
	//else
	//{
	//	isHasKeyPhase=TRUE;
	//	kPhaseXA.clear();
	//	kPhaseYA.clear();
	//	for (int k=0;k<(int)kp.size();k++)
	//	{
	//		int kpCout=0;
	//		if (k%ASamplingFreq==0)
	//		{
	//			kpCout=(int)kp[k];
	//			for (int i=1;i<=kpCout;i++)
	//			{
	//				float x=k+kp[k+i];
	//				kPhaseXA.push_back(x);
	//				kPhaseYA.push_back((float)ABWData[(int)x]);
	//			}
	//		}
	//	}
	//	intermetallicSize=kPhaseXA[1]-kPhaseXA[0];
	//	DrawABWData(ABWData);
	//}
	int /*key_freq=8192,*/wave_freq=samplingFreq;
	int d_num = key_freq/wave_freq;
	if(key_freq%wave_freq!=0) d_num += 1;
	if(d_num<=0) d_num = 1;
	if ((int)kp.size()==0)
	{
		isHasKeyPhase=FALSE;
		AfxMessageBox("无健相信息，无法完成动平衡试验！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("无健相信息，无法完成动平衡试验！");
		return;
	}
	else
	{
		kPhaseXA.clear();
		kPhaseYA.clear();
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
					if(x<ABWData.GetSize())
					{
						kPhaseXA.push_back(x);
						kPhaseYA.push_back((float)ABWData[(int)x]);
					}
				}			
			}			
		}
		intermetallicSize=kPhaseXA[1]-kPhaseXA[0];
		DrawABWData(ABWData);

	}

}

void CDynamicEquilibrium::SetKeyPhaseB(std::vector<float> kp,int key_freq,int samplingFreq)
{
	int /*key_freq=8192,*/wave_freq=samplingFreq;
	int d_num = key_freq/wave_freq;
	if(key_freq%wave_freq!=0) d_num += 1;
	if(d_num<=0) d_num = 1;
	if ((int)kp.size()==0)
	{
		isHasKeyPhase=FALSE;
		AfxMessageBox("无健相信息，无法完成动平衡试验！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("无健相信息，无法完成动平衡试验！");
		return;
	}
	else
	{
		kPhaseXB.clear();
		kPhaseYB.clear();
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
					if(x<BBWData.GetSize())
					{
						kPhaseXB.push_back(x);
						kPhaseYB.push_back((float)BBWData[(int)x]);
					}			

				}			
			}			
		}
		intermetallicSize=kPhaseXB[1]-kPhaseXB[0];
		DrawBBWData(BBWData);
	}

	//KeyPhase.SetSize((int)kp.size());
	/*vector<float> kPhaseX;*/
	
}

void  CDynamicEquilibrium::DrawBBWData(	CNiReal64Vector  bBWData,int m_begin)
{

	double minValue, maxValue, valueDecl;
	unsigned int minIndex, maxIndex;
	bBWData.MinMax(minValue, minIndex, maxValue, maxIndex);
	valueDecl=(maxValue-minValue)*0.1;

	

	m_Graph.Plots.RemoveAll();
	m_Graph.Plots.Add();
	m_Graph.Plots.Item(1).SetLineColor(RGB(0,0,255));
	
	CNiReal64Vector XbBWData((int)bBWData.GetSize());
	for (int kk=0;kk<(int)XbBWData.GetSize();kk++)
	{
		XbBWData[kk]=(m_begin+kk)*m_deltaFreq_B;
	}

	//m_Graph.Plots.Item(1).PlotY(bBWData);


	 

   m_Graph.GetPlots().Item(1).PlotXvsY(XbBWData,bBWData);



	CNiReal64Vector kpX((int)kPhaseXB.size());
	CNiReal64Vector kpY((int)kPhaseYB.size());
	for (int k=0;k<(int)kPhaseXB.size();k++)
	{
		kpX[k]=kPhaseXB[k]*m_deltaFreq_B;
		kpY[k]=kPhaseYB[k];
	}



	m_Graph.GetPlots().Add();

	m_Graph.GetPlots().Item(2).PointColor=RGB(255,0,0);
	m_Graph.GetPlots().Item(2).LineStyle=CNiPlot::LineNone;
	m_Graph.GetPlots().Item(2).PointStyle=CNiPlot::PointEmptyCircle;
	m_Graph.GetPlots().Item(2).LineWidth=2;
	m_Graph.GetPlots().Item(2).PlotXvsY(kpX,kpY);

	
 
	m_Graph.Axes.Item(1).SetMinMax(0,BBWData.GetSize()*m_deltaFreq_B);
//	m_Graph.Axes.Item(2).SetMinMax((minValue-valueDecl)*m_deltaFreq_B,(maxValue+valueDecl)*m_deltaFreq_B);

	m_Graph.Cursors.RemoveAll();
	m_Graph.Cursors.Add();
	m_Graph.SetTrackMode(CNiGraph::TrackDragCursor);
	m_Graph.Cursors.Item(1).SetCrosshairStyle(CNiCursor::CrosshairMajorXMinorY);
	m_Graph.Cursors.Item(1).SetColor(RGB(0,255,0));
	m_Graph.Cursors.Item(1).SnapMode = CNiCursor::SnapNearestPoint;//SnapNearestPoint

 	m_Graph.Cursors.Item(1).SetPosition(BBWData.GetSize()/2*m_deltaFreq_B,(maxValue-minIndex)/2);
 
	 


};

void  CDynamicEquilibrium::DrawABWData(	CNiReal64Vector  aBWData,int m_begin)
{

	double minValue, maxValue, valueDecl;
	unsigned int minIndex, maxIndex;
	aBWData.MinMax(minValue, minIndex, maxValue, maxIndex);
	valueDecl=(maxValue-minValue)*0.1;

	//aNewdata.Reverse();

	//int size=aBWData.GetSize()/intermetallicSize;

	//aNewdata.SetSize(size);//////////////////////////////////////////////////////////////////////////
	//for (int k=0;k<size;k++)
	//{
	//	aNewdata[k]=aBWData[intermetallicSize*k];
	//}

	m_newGraph.Plots.RemoveAll();
	m_newGraph.Plots.Add();
	m_newGraph.Plots.Item(1).SetLineColor(RGB(0,0,255));
//	m_newGraph.Plots.Item(1).PlotY(aBWData);

	CNiReal64Vector XbBWData((int)aBWData.GetSize());
	for (int kk=0;kk<(int)XbBWData.GetSize();kk++)
	{
		XbBWData[kk]=(m_begin+kk)*m_deltaFreq_A;
	}

	//m_Graph.Plots.Item(1).PlotY(bBWData);

	m_newGraph.GetPlots().Item(1).PlotXvsY(XbBWData,aBWData);



	CNiReal64Vector kpX((int)kPhaseXA.size());
	CNiReal64Vector kpY((int)kPhaseYA.size());
	for (int k=0;k<(int)kPhaseXA.size();k++)
	{
		kpX[k]=kPhaseXA[k]*m_deltaFreq_A;
		kpY[k]=kPhaseYA[k];
	}
	m_newGraph.GetPlots().Add();

	m_newGraph.GetPlots().Item(2).PointColor=RGB(255,0,0);
	m_newGraph.GetPlots().Item(2).LineStyle=CNiPlot::LineNone;
	m_newGraph.GetPlots().Item(2).PointStyle=CNiPlot::PointEmptyCircle;
	m_newGraph.GetPlots().Item(2).LineWidth=2;
	m_newGraph.GetPlots().Item(2).PlotXvsY(kpX,kpY);

	m_newGraph.Axes.Item(1).SetMinMax(0,ABWData.GetSize()*m_deltaFreq_A);
//	m_newGraph.Axes.Item(2).SetMinMax(minValue-valueDecl,maxValue+valueDecl);

	m_newGraph.Cursors.RemoveAll();
	m_newGraph.Cursors.Add();
	m_newGraph.SetTrackMode(CNiGraph::TrackDragCursor);
	m_newGraph.Cursors.Item(1).SetCrosshairStyle(CNiCursor::CrosshairMajorXMinorY);
	m_newGraph.Cursors.Item(1).SetColor(RGB(0,255,0));
	m_newGraph.Cursors.Item(1).SnapMode = CNiCursor::SnapNearestPoint;//SnapNearestPoint

	m_newGraph.Cursors.Item(1).SetPosition(ABWData.GetSize()/2*m_deltaFreq_A,(maxValue-minIndex)/2);
	//m_Graph.Cursors.Item(1).SetPlot(m_Graph.Plots.Item(2));


};

void CDynamicEquilibrium::DblClickCwgraph1()
{


	m_BFlag=false;
	
	double valueDecl=(m_Graph.Axes.Item("YAxis-1").Maximum-m_Graph.Axes.Item("YAxis-1").Minimum)*0.05;
	if (m_Graph.Plots.Count<3)
	{
		m_Graph.Plots.Add();
		m_Graph.Plots.Item(3).SetLineColor(RGB(255,0,255));
		//m_Graph.Plots.Item(3).SetLineStyle(CNiPlot::LineDot);

		BSX[0]=BSX[1]=m_Graph.Cursors.Item(1).GetXPosition();
		BSY[0]=m_Graph.Axes.Item("YAxis-1").Minimum+valueDecl;
		BSY[1]=m_Graph.Axes.Item("YAxis-1").Maximum-valueDecl;
		m_Graph.Plots.Item(3).PlotXvsY(BSX,BSY);

	}
	else
	{
        m_Graph.Plots.Remove(3);
		m_Graph.Plots.Remove(4);

		m_Graph.Plots.Add();
		m_Graph.Plots.Item(3).SetLineColor(RGB(255,0,255));
		//m_Graph.Plots.Item(3).SetLineStyle(CNiPlot::LineDot);

		BSX[0]=BSX[1]=m_Graph.Cursors.Item(1).GetXPosition();
		BSY[0]=m_Graph.Axes.Item("YAxis-1").Minimum+valueDecl;
		BSY[1]=m_Graph.Axes.Item("YAxis-1").Maximum-valueDecl;
		m_Graph.Plots.Item(3).PlotXvsY(BSX,BSY);


		m_Graph.Plots.Add();
		m_Graph.Plots.Item(4).SetLineColor(RGB(255,0,128));
		m_Graph.Plots.Item(4).SetLineStyle(CNiPlot::LineDot);

		BEX[0]=BEX[1]=m_Graph.Cursors.Item(1).GetXPosition();
		BEY[0]=m_Graph.Axes.Item("YAxis-1").Minimum+valueDecl;
		BEY[1]=m_Graph.Axes.Item("YAxis-1").Maximum-valueDecl;
		m_Graph.Plots.Item(4).PlotXvsY(BEX,BEY);
	}
}

void CDynamicEquilibrium::CursorChangeCwgraph1(long* CursorIndex, VARIANT* XPos, VARIANT* YPos, BOOL* bTracking)
{
	double valueDecl=(m_Graph.Axes.Item("YAxis-1").Maximum-m_Graph.Axes.Item("YAxis-1").Minimum)*0.05;	
	int p=-1;


	m_BFlag=true;

	 
	

	if (m_Graph.Plots.Count>=3)
	{
		
		
		
		m_Graph.Plots.Add();
		m_Graph.Plots.Item(5).SetLineColor(RGB(255,0,255));
		//m_Graph.Plots.Item(5).SetLineStyle(CNiPlot::LineDot);
		CNiReal64Vector etX(2);
		CNiReal64Vector etY(2);
		etX[0]=BSX[0];
		etX[1]=XPos->dblVal;
		etY[0]=m_Graph.Axes.Item("YAxis-1").Minimum+valueDecl;
		etY[1]=m_Graph.Axes.Item("YAxis-1").Minimum+valueDecl;
		m_Graph.Plots.Item(5).PlotXvsY(etX,etY);

		m_Graph.Plots.Add();
		m_Graph.Plots.Item(6).SetLineColor(RGB(255,0,255));
		//m_Graph.Plots.Item(6).SetLineStyle(CNiPlot::LineDot);
		CNiReal64Vector ebX(2);
		CNiReal64Vector ebY(2);
		ebX[0]=BSX[0];
		ebX[1]=XPos->dblVal;
		ebY[0]=m_Graph.Axes.Item("YAxis-1").Maximum-valueDecl;
		ebY[1]=m_Graph.Axes.Item("YAxis-1").Maximum-valueDecl;
		m_Graph.Plots.Item(6).PlotXvsY(ebX,ebY);
		

		int sIntermetallic=0,eIntermetallic=0;
		for (int k=0;k<(int)kPhaseXB.size();k++)
		{		
			if(intermetallicSize*k>=BSX[0]/m_deltaFreq_B&&intermetallicSize*k<=XPos->dblVal/m_deltaFreq_B)
			{
				p++;
				if(p==0)
				{
					sIntermetallic=eIntermetallic=intermetallicSize*k;
				}				
				eIntermetallic=eIntermetallic>intermetallicSize*k?sIntermetallic:intermetallicSize*k;
				
			}
		}
		if (BSX[0]>XPos->dblVal/m_deltaFreq_B)
		{
			p=-1;
		}
	//	double rotateSpeed=0.0;

		m_beforeBalanceWeight=p-1;

		UpdateData(FALSE);
 


		m_BBStart=sIntermetallic;
		m_BBEnd=eIntermetallic;
//////////////////////////////////////////////////////////cxy
/*
 
		double minValue, maxValue;
		unsigned int minIndex, maxIndex;		
		CNiReal64Vector tempData(eIntermetallic-sIntermetallic);




		for (int k=0;k<(int)tempData.GetSize();k++)
		{
			tempData[k]=BBWData[k+sIntermetallic];
		}


		CNiReal64Vector  interBBWData=re_timeB(tempData);

		//FFT变换 

		int dataSize = interBBWData.GetSize();                         //Number of data points acquired
		CNiReal64Vector realData, imagData;
		realData.SetSize(dataSize);
		imagData.SetSize(dataSize);

		CNiComplexReal64Vector fftData;

		realData+=interBBWData;
		
		try
		{
			CNiMath::FFT(realData,false,fftData);
		}
		catch(CException* e)
		{
			SetBParams(0.0,0.0,0.0,0.0);
			UpdateData(FALSE);
			return;
		}


		int numPts = dataSize/2;
		double scalingFactor = 1.0/(double)dataSize;
		double deltaFreq = BSamplingFreq*scalingFactor*(double)dataSize/(double)tempData.GetSize();    

		CNiReal64Vector temp(numPts);
		CNiReal64Vector frequencyData=temp;
		for( int i=1;i< numPts;i++)
		{
			frequencyData[i] = fftData[i].Magnitude()*scalingFactor*2;
		}

		frequencyData[0] = 0;//fftData[0].Magnitude() * scalingFactor;//剔除直流分量
		int freqRange=(int)(frequencyData.GetSize()*deltaFreq)+1;

		vector<double> OutX;
		for (int k=0;k<freqRange;k++)
		{
			OutX.push_back(k*deltaFreq);
		}

		// 二分法查找与键相频率最接近的频点(OutX是根据频率大小递增排序的)
		// 并将相应的幅值、相位等
		double fKeyFreq=BSamplingFreq*(double)p/((double)eIntermetallic-(double)sIntermetallic);
		vector<double>::iterator pLowerBound =
			std::lower_bound(OutX.begin(), OutX.end() - 1, fKeyFreq);
		int nKeyIndex =(int)(pLowerBound - OutX.begin());
		if (pLowerBound != OutX.begin())
		{
			double prv = *(pLowerBound - 1);
			if ( (*pLowerBound - fKeyFreq) > (fKeyFreq - prv) )
				nKeyIndex --;	// 上一个点更近，使用上一个点
		}

		BAmplitude=frequencyData[nKeyIndex];

		double phase=atan2(fftData[nKeyIndex].imag,fftData[nKeyIndex].real);
		if (phase<0)
			BPhase=phase*180.0/3.1415926535+360.0;
		else
			BPhase=phase*180.0/3.1415926535;

		if (p>0)
		{
			rotateSpeed=BSamplingFreq*60.0*(double)p/((double)eIntermetallic-(double)sIntermetallic);
			interBBWData.MinMax(minValue, minIndex, maxValue, maxIndex);
			SetBParams(rotateSpeed,(maxValue-minValue)/2,BAmplitude,BPhase);
			m_BFlag=false;
		}
		  //    DrawBBWData(tempData);//cxy 20111219
	
		//    m_Graph.PlotY(tempData);

	 	UpdateData(FALSE);

*/  //cxy
		
   //     DrawBBWData(tempData,sIntermetallic);//cxy 20111219
			
       
	// 	m_Graph.Axes.Item("XAxis").SetMinMax(sIntermetallic,sIntermetallic+tempData.GetSize());

		
	}


/*  //cxy
 
	if (p<=0)
	{
		if (m_BFlag)
		{
		 SetBParams(0.0,0.0,0.0,0.0);
         UpdateData(FALSE);
		 m_BFlag=false;
		}

	}
*/

}
//取消配重前周期选择
void CDynamicEquilibrium::OnDeleteBSel()
{
	if (m_Graph.Plots.Count>0)
	{
		m_Graph.Plots.RemoveAll();
		DrawBBWData(BBWData);
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：取消配重前周期选择");
	}
}

void CDynamicEquilibrium::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{	

	CompDyn(pNMHDR);
	*pResult = 0;
}

void CDynamicEquilibrium::CompDyn(NMHDR *pN)
{

	Invalidate();
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pN;
	m_wndList.SetEditable(FALSE);
	int nItem = temp->iItem;
	//get the column number
	int nSubItem = temp->iSubItem;
	if(nItem == 1)
	{	
		m_wndList.SetEditable(TRUE);		
	}
	if(nItem == 0)
	{	
		m_wndList.SetEditable(TRUE);		
	}
	if(nSubItem == 6 && nItem == 3)
	{	
		m_wndList.SetEditable(TRUE);
		return ;
	}
	if(nSubItem == 6 && nItem == 4)
	{	
		m_wndList.SetEditable(TRUE);
		return ;
	}

	if(nSubItem == 0 )
	{	
		m_wndList.SetEditable(FALSE);
		return ;
	}
	if(nItem == 0&&nSubItem == 5 )
	{	
		m_wndList.SetEditable(FALSE);
		return ;
	}
	if(nItem == 0&&nSubItem == 6 )
	{	
		m_wndList.SetEditable(FALSE);
		return ;
	}
	GetAParams();
}
void CDynamicEquilibrium::CursorChangeCwgraph3(long* CursorIndex, VARIANT* XPos, VARIANT* YPos, BOOL* bTracking)
{
	
	
	double valueDecl=(m_newGraph.Axes.Item("YAxis-1").Maximum-m_newGraph.Axes.Item("YAxis-1").Minimum)*0.05;		
	int p=-1;


	m_AFlag=true;




	if (m_newGraph.Plots.Count>=3)
	{



		m_newGraph.Plots.Add();
		m_newGraph.Plots.Item(5).SetLineColor(RGB(255,0,255));
		//m_Graph.Plots.Item(5).SetLineStyle(CNiPlot::LineDot);
		CNiReal64Vector etX(2);
		CNiReal64Vector etY(2);
		etX[0]=ASX[0];
		etX[1]=XPos->dblVal;
		etY[0]=m_newGraph.Axes.Item("YAxis-1").Minimum+valueDecl;
		etY[1]=m_newGraph.Axes.Item("YAxis-1").Minimum+valueDecl;
		m_newGraph.Plots.Item(5).PlotXvsY(etX,etY);

		m_newGraph.Plots.Add();
		m_newGraph.Plots.Item(6).SetLineColor(RGB(255,0,255));
		//m_Graph.Plots.Item(6).SetLineStyle(CNiPlot::LineDot);
		CNiReal64Vector ebX(2);
		CNiReal64Vector ebY(2);
		ebX[0]=ASX[0];
		ebX[1]=XPos->dblVal;
		ebY[0]=m_newGraph.Axes.Item("YAxis-1").Maximum-valueDecl;
		ebY[1]=m_newGraph.Axes.Item("YAxis-1").Maximum-valueDecl;
		m_newGraph.Plots.Item(6).PlotXvsY(ebX,ebY);


		int sIntermetallic=0,eIntermetallic=0;
		for (int k=0;k<(int)kPhaseXA.size();k++)
		{		
			if(intermetallicSize*k>=ASX[0]/m_deltaFreq_A&&intermetallicSize*k<=XPos->dblVal/m_deltaFreq_A)
			{
				p++;
				if(p==0)
				{
					sIntermetallic=eIntermetallic=intermetallicSize*k;
				}				
				eIntermetallic=eIntermetallic>intermetallicSize*k?sIntermetallic:intermetallicSize*k;

			}
		}
		if (ASX[0]>XPos->dblVal/m_deltaFreq_A)
		{
			p=-1;
		}
		//double rotateSpeed=0.0;

		m_afterBalanceWeight=p-1;

		UpdateData(FALSE);



		m_AAStart=sIntermetallic;
		m_AAEnd=eIntermetallic;
	}


}

CNiReal64Vector CDynamicEquilibrium::re_timeB(CNiReal64Vector p1)
{
	int n1, n2;
	double dt1,T;


	n1=p1.GetSize();
	n2=pow(2.0,(int)(log((double)n1)/log(2.0)));
	CNiReal64Vector p2(n2);	
	dt1=1.0/(double)ASamplingFreq;
	T=(double)p1.GetSize()/(double)BSamplingFreq;

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
}



CNiReal64Vector CDynamicEquilibrium::re_timeA (CNiReal64Vector p1)
{
	int n1, n2;
	double dt1,T;


	n1=p1.GetSize();
	n2=pow(2.0,(int)(log((double)n1)/log(2.0)));
	CNiReal64Vector p2(n2);	
	dt1=1.0/(double)ASamplingFreq;
	T=(double)p1.GetSize()/(double)ASamplingFreq;
	
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
}


void CDynamicEquilibrium::SetBParams(double rs,double fs,double am,double ph)
{
	CString Str;
	Str.Format("%.3f",rs);
	m_wndList.SetItemText(IDX0, 1, Str);	
	Str.Format("%.3f",fs);
	m_wndList.SetItemText(IDX0, 2, Str);	
	Str.Format("%.3f",am);
	m_wndList.SetItemText(IDX0, 3, Str);	
	Str.Format("%.3f",ph);
	m_wndList.SetItemText(IDX0, 4, Str);	



	GetAParams();
}
void CDynamicEquilibrium::SetAParams(double rs,double fs,double am,double ph)
{
	CString Str;
	Str.Format("%.3f",rs);
	m_wndList.SetItemText(IDX1, 1, Str);	
	Str.Format("%.3f",fs);
	m_wndList.SetItemText(IDX1, 2, Str);	
	Str.Format("%.3f",am);
	m_wndList.SetItemText(IDX1, 3, Str);	
	Str.Format("%.3f",ph);
	m_wndList.SetItemText(IDX1, 4, Str);	



	GetAParams();

}

void CDynamicEquilibrium::DblClickCwgraph3()
{

	
	
	double valueDecl=(m_newGraph.Axes.Item("YAxis-1").Maximum-m_newGraph.Axes.Item("YAxis-1").Minimum)*0.05;
	
	m_AFlag=false;
	if (m_newGraph.Plots.Count<3)
	{
		m_newGraph.Plots.Add();
		m_newGraph.Plots.Item(3).SetLineColor(RGB(255,0,255));
		//m_Graph.Plots.Item(3).SetLineStyle(CNiPlot::LineDot);

		ASX[0]=ASX[1]=m_newGraph.Cursors.Item(1).GetXPosition();
		ASY[0]=m_newGraph.Axes.Item("YAxis-1").Minimum+valueDecl;
		ASY[1]=m_newGraph.Axes.Item("YAxis-1").Maximum-valueDecl;
		m_newGraph.Plots.Item(3).PlotXvsY(ASX,ASY);

	}
	else
	{
		m_newGraph.Plots.Remove(3);
		m_newGraph.Plots.Remove(4);

		m_newGraph.Plots.Add();
		m_newGraph.Plots.Item(3).SetLineColor(RGB(255,0,255));
		//m_Graph.Plots.Item(3).SetLineStyle(CNiPlot::LineDot);

		ASX[0]=ASX[1]=m_newGraph.Cursors.Item(1).GetXPosition();
		ASY[0]=m_newGraph.Axes.Item("YAxis-1").Minimum+valueDecl;
		ASY[1]=m_newGraph.Axes.Item("YAxis-1").Maximum-valueDecl;
		m_newGraph.Plots.Item(3).PlotXvsY(ASX,ASY);

		m_newGraph.Plots.Add();
		m_newGraph.Plots.Item(4).SetLineColor(RGB(255,0,128));
		m_newGraph.Plots.Item(4).SetLineStyle(CNiPlot::LineDot);

		AEX[0]=AEX[1]=m_newGraph.Cursors.Item(1).GetXPosition();
		AEY[0]=m_newGraph.Axes.Item("YAxis-1").Minimum+valueDecl;
		AEY[1]=m_newGraph.Axes.Item("YAxis-1").Maximum-valueDecl;
		m_newGraph.Plots.Item(4).PlotXvsY(AEX,AEY);
	}
}
//取消试配重周期选择
void CDynamicEquilibrium::OnDeleteasel()
{
	if (m_newGraph.Plots.Count>0)
	{
		m_newGraph.Plots.RemoveAll();
		DrawABWData(ABWData);
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：取消试配重周期选择");
	}
}

void CDynamicEquilibrium::GetAParams()
{
	CString Str;
	double param1,param2,param3,param4,param5,param6,param7,param8,param9,v0,v1;
	
	param7=atof(m_wndList.GetItemText(1, 5));
	param5=atof(m_wndList.GetItemText(1, 6));
	
	if (param7!=0.0 && param5!=0.0)
	{	
		bool is_ok = balancingMethod(BAmplitude, BPhase,
		AAmplitude, APhase, param5,param7, outWeight, outAngle);

		if (!is_ok)
			return;
		Str.Format("%.3f",outWeight);
		m_wndList.SetItemText(IDX2, 5, Str);	
		Str.Format("%.3f",outAngle);
		m_wndList.SetItemText(IDX2, 6, Str);	
		isWeightVisual=TRUE;
	}
	else
	{
		isWeightVisual=FALSE;
		m_wndList.SetItemText(IDX2, 5, "-");	
		m_wndList.SetItemText(IDX2, 6, "-");
	}


	param1=atof(m_wndList.GetItemText(0, 3));

	param2=atof(m_wndList.GetItemText(1, 3));

	param3=atof(m_wndList.GetItemText(0, 4));

	param4=atof(m_wndList.GetItemText(1, 4));
	param6=atof(m_wndList.GetItemText(2, 6));

	param8=atof(m_wndList.GetItemText(3, 6));
	param9=atof(m_wndList.GetItemText(4, 6));
	
	tBAngle=outAngle;
	tWAngle=outWeight;
	

	//diss1=param8;
	//diss2=param9;

	if (balanceBreak (outAngle, outWeight, param8, param9, &v0, &v1))
	{
		if (v0==0.0||v1==0.0)
		{
			isBDiss=FALSE;
			m_wndList.SetItemText(IDX3, 5, "-");	
			m_wndList.SetItemText(IDX4, 5, "-");	
		}
		else
		{
			isBDiss=TRUE;
			Str.Format("%.3f",v0);
			m_wndList.SetItemText(IDX3, 5, Str);	
			Str.Format("%.3f",v1);
			m_wndList.SetItemText(IDX4, 5, Str);	
		}
	}
	else
	{
		isBDiss=FALSE;
		m_wndList.SetItemText(IDX3, 5, "-");	
		m_wndList.SetItemText(IDX4, 5, "-");	
	}
	if (param1!=0)
	{
		DrawDynEquGraph(param1,param2,param3,param4,param5,param6,param8,param9);
	}
	UpdateData(FALSE);
}


/**
* 动平衡计算1，计算出配重方向与试重方向之间的偏角、重量
* @param amplitude1, phase1 原始幅值、相位(弧度)
* @param amplitude2, phase2, weightTest 试重幅值、相位、重量
* @param outWeight, outAngle 输出配重重量、与试重位置的角度差（逆时针）
*/
bool CDynamicEquilibrium::balancingMethod2(double amplitude1, double phase1,
					  double amplitude2, double phase2, double weightTest,double& outWeight, double& outAngle)
{
	double influenceAngle = 0;
	// 计算试重块引起的振动幅值 amplitude3
	double amplitude3 = sqrt(amplitude1 * amplitude1 + amplitude2 * amplitude2
		- 2 * amplitude1 * amplitude2 * cos( (phase1 - phase2) * mPI / 180));

	//若为 0 则需调整试重角度
	if(amplitude3 != 0)
	{
		//计算配重大小、影响系数、影响系数角
		double weightBalance = amplitude1 * weightTest / amplitude3;
		double influenceCoeff = amplitude3 / weightTest;
		double k = (amplitude2 * sin(phase2 * mPI / 180)
			- amplitude1 * sin(phase1 * mPI / 180)) / (amplitude2 * cos(phase2 * mPI / 180) - amplitude1 * cos(phase1 * mPI / 180));
		double kangle1 = (atan(k)) * 180 / mPI;
		double kangle2 = kangle1 + 180;

		// 计算影响的
		if (amplitude2 * cos(phase2 * mPI / 180) < amplitude1 * cos(phase1 * mPI / 180))
			influenceAngle = phase1 + 180 - kangle2;
		else
			influenceAngle = phase1 + 180 - kangle1;

		if (influenceAngle < 0)
			influenceAngle = influenceAngle + 360;

		outWeight = weightBalance;
		outAngle = influenceAngle;
		return true;
	}
	else
	{
		return false;
	}

	return true;

}

/**
* 动平衡计算2, 计算出配重角度、重量
* @param amplitude1, phase1 原始幅值、相位(弧度)
* @param amplitude2, phase2, angleTest, weightTest 试重幅值、相位、角度、重量
* @param outWeight, outAngle 输出配重重量、与试重位置的绝对角度
*/
bool CDynamicEquilibrium::balancingMethod(double amplitude1, double phase1,
					 double amplitude2, double phase2, double angleTest, double weightTest,double& outWeight, double& outAngle)
{
	double influenceAngle;
	if (!balancingMethod2 (amplitude1, phase1,
		amplitude2, phase2, weightTest,
		outWeight, influenceAngle))
		return false;
	//计算配重角
	outAngle = angleTest + influenceAngle;
	while (outAngle < 0)
		outAngle += 360;

	while (outAngle > 360)
		outAngle -= 360;

	return true;
}

/**
* 将配重方案分解到两个指定角度
*	@param angle	配重角
*	@param v		配重重量
*	@param a0, a1	分解配重角, 自动根据angle的角度确定方向
*	@param v0, v1	输出计算出来的角度
*/
bool CDynamicEquilibrium::balanceBreak (double angle, double v, double a0, double a1, double *v0, double *v1)
{
	// 校正a0到 0 ~ 360 之间
	while (a0 < 0)
		a0 += 360;
	while (a0 > 360)
		a0 -= 360;

	// 校正a1到 0 ~ 360 之间
	while (a1 < 0)
		a1 += 360;
	while (a1 > 360)
		a1 -= 360;

	// 校正a1到 0 ~ 360 之间
	while (angle < 0)
		angle += 360;
	while (angle > 360)
		angle -= 360;

	// 如果 a0 < (a1 + 180), 则交换a0, a1
	if (a0 > a1)
	{
		if (a0 <= (a1 + 180))
			std::swap (a0, a1);
	}
	else if (a0 < a1)
	{
		if (a1 > (a0 + 180))
			std::swap (a0, a1);
	}

	if (a0 > a1)
		a1 += 360;

	// 如果angle不在 a0 -> a0 + 180 范围内，为错误
	if (angle < a0 || angle > a1)
		return false;

	double A0 = abs(a0 - angle) * mPI / 180;
	double A1 = abs(angle - a1) * mPI / 180;

	if (A0 == 0)
	{
		*v0 = v;
		*v1 = 0;
	}
	else if (A1 == 0)
	{
		*v0 = 0;
		*v1 = v;
	}
	else
	{
		double d = v / (1/tan(A0) + 1/tan(A1));		// 垂线长度
		*v0 = d / sin (A0);
		*v1 = d / sin (A1);
	}

	return true;
}
//存储视图到图像文件
void CDynamicEquilibrium::OnSavebwimage()
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

	CString strBuff ;
	CString   strFileName="动平衡试验分析";   
	LPTSTR   pszfilename;   
	//m_Graph.GetWindowText(strFileName);
	pszfilename=strFileName.GetBuffer(_MAX_PATH);
	FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  

	if(FileOpenDlg.DoModal() == IDOK)
	{
		//CString strSaveFilapath = FileOpenDlg.GetPathName();
		//CString strSaveFilapath;
		//FileOpenDlg.()
		//m_Graph.GetWindowText(strSaveFilapath);
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：存储视图到图像...");

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
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：保存完成，存储为  "+strSaveFilapath);

	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("动平衡试验：取消存储视图到图像");

	image.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
}


void CDynamicEquilibrium::OnSetFocus(CWnd* pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	pCMF->ActivateTabEA(0);
	// TODO: 在此处添加消息处理程序代码
}

void CDynamicEquilibrium::MouseUpCwgraph1(short Button, short Shift, long x, long y)
{
	
 
}

void CDynamicEquilibrium::MouseDownCwgraph1(short Button, short Shift, long x, long y)
{
	
}

void CDynamicEquilibrium::ONBAnalysis()
{
	if (m_beforeBalanceWeight>16 || m_beforeBalanceWeight<1)
	{
		AfxMessageBox("所选时段周期过长或为负，请重新选择！");
		m_BFlag=false;
		return;

	}
	if (m_BFlag)
	{

    m_Graph.Axes.Item("XAxis").SetMinMax(m_BBStart,m_BBEnd);
 
	int xMax=(int)(m_Graph.Axes.Item("XAxis").GetMaximum());
	int xMin=(int)(m_Graph.Axes.Item("XAxis").GetMinimum());
 
	
	int minT=0,maxT=0;
	for (int k=0;k<(int)kPhaseXB.size();k++)
	{
		if (kPhaseXB[k]>=xMin)
		{
			minT=kPhaseXB[k];
			break;
		}
	}

	for (int k=0;k<(int)kPhaseXB.size();k++)
	{
		if (kPhaseXB[k]<=xMax)
		{
			maxT=kPhaseXB[k];
		}
		else
			break;
	}

	int minKZ=0;
	int maxKZ=0;

	if (minT==maxT)
	{
		minT=minKZ;
		maxT=maxKZ;
	}
	if (maxT-minT>=kPhaseXB[1]-kPhaseXB[0])
	{
		xMax=maxT;
		xMin=minT;
		m_Graph.Axes.Item("XAxis").SetMinimum(xMin*m_deltaFreq_B);
		m_Graph.Axes.Item("XAxis").SetMaximum(xMax*m_deltaFreq_B);
	}
	else
	{
		xMax=maxT>minT?maxT:minT;
		xMin=maxT<minT?maxT:minT;
		m_Graph.Axes.Item("XAxis").SetMinimum(xMin*m_deltaFreq_B);
		m_Graph.Axes.Item("XAxis").SetMaximum(xMax*m_deltaFreq_B);

	}
	//}


	double minValue, maxValue, valueDecl;
	unsigned int minIndex, maxIndex;



	m_Graph.Cursors.RemoveAll();
	m_Graph.Cursors.Add();
	m_Graph.SetTrackMode(CNiGraph::TrackDragCursor);
	m_Graph.Cursors.Item(1).SetCrosshairStyle(CNiCursor::CrosshairMajorXMinorY);
	m_Graph.Cursors.Item(1).SetColor(RGB(0,255,0));
	m_Graph.Cursors.Item(1).SnapMode = CNiCursor::SnapNearestPoint;//SnapNearestPoint

	m_Graph.Cursors.Item(1).SetPosition(xMin*m_deltaFreq_B,BBWData[xMin]);


	minKZ=xMin;
	maxKZ=xMax;

	int round=xMax-xMin-1;

	CNiReal64Vector newData(round);//将newData变为成员变量，便于保存当前时段波形数据
	newData.SetSize(round);
	if (round>16)
	{
		if (xMin>=0&&xMax<=(int)BBWData.GetSize()+1)
		{
			for (int i=0;i<round;i++)
			{
				newData[i]=BBWData[xMin];
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

	}


	
 


	CNiReal64Vector  interBBWData=re_timeB(newData);

 

	int dataSize = interBBWData.GetSize();                         //Number of data points acquired

 
	CNiReal64Vector realData, imagData;
	realData.SetSize(dataSize);
	imagData.SetSize(dataSize);

	CNiComplexReal64Vector fftData;

	realData+=interBBWData;

	try
	{
		CNiMath::FFT(realData,false,fftData);
	}
	catch(CException* e)
	{
		SetBParams(0.0,0.0,0.0,0.0);
		UpdateData(FALSE);
		return;
	}


	int numPts = dataSize/2;
	double scalingFactor = 1.00/(double)dataSize;
	double deltaFreq = BSamplingFreq*scalingFactor*(double)dataSize/(double)newData.GetSize();  




	CNiReal64Vector frequencyData;		//频谱图数据

	CNiReal64Vector temp(numPts);
	frequencyData=temp;
	double dtmp = 0;

	for( int i=1;i< numPts;i++)
	{
		dtmp = fftData[i].Magnitude();
		frequencyData[i] = fftData[i].Magnitude()*scalingFactor*2;
		
	}


	int  freqRange;						//频谱图范围

	// Scale DC value
	frequencyData[0] = 0;//fftData[0].Magnitude() * scalingFactor;//剔除直流分量
	freqRange=(int)(frequencyData.GetSize()*deltaFreq)+1;

			newData.MinMax(minValue, minIndex, maxValue, maxIndex);
			double rotateSpeed=BSamplingFreq*60.0*(double)m_beforeBalanceWeight/((double)maxKZ-(double)minKZ);



			vector<double> OutX;
			for (int k=0;k<freqRange;k++)
			{
				OutX.push_back(k*deltaFreq);
			}

			double fKeyFreq=BSamplingFreq*(double)m_beforeBalanceWeight/((double)maxKZ-(double)minKZ);

			vector<double>::iterator pLowerBound =
				std::lower_bound(OutX.begin(), OutX.end() - 1, fKeyFreq);
			int nKeyIndex =(int)(pLowerBound - OutX.begin());
			if (pLowerBound != OutX.begin())
			{
				double prv = *(pLowerBound - 1);
				if ( (*pLowerBound - fKeyFreq) > (fKeyFreq - prv) )
					nKeyIndex --;	// 上一个点更近，使用上一个点
			}
 
			FFTParam fp;
			fp.amplitude=frequencyData[nKeyIndex];
			fp.frequency=deltaFreq*nKeyIndex;
			fp.phase=atan2(fftData[nKeyIndex].imag,fftData[nKeyIndex].real);

			 

			if (fp.phase<0)
				fp.phase=fp.phase*180.0/3.1415926535+360.0;
			else
				fp.phase=fp.phase*180.0/3.1415926535;
			SetBParams(rotateSpeed,(maxValue-minValue),2*fp.amplitude,fp.phase);
			BAmplitude=fp.amplitude;
			BPhase=fp.phase;
			UpdateData(FALSE);


	m_BFlag=false;
	DblClickCwgraph1();
}

}

void CDynamicEquilibrium::ONAAnalysis()
{
	if (m_afterBalanceWeight>16 || m_afterBalanceWeight<1)
	{
		AfxMessageBox("所选时段周期过长或为负，请重新选择！");
		m_AFlag=false;
		return;

	}
	if (m_AFlag)
	{

		m_newGraph.Axes.Item("XAxis").SetMinMax(m_AAStart,m_AAEnd);

		int xMax=(int)(m_newGraph.Axes.Item("XAxis").GetMaximum());
		int xMin=(int)(m_newGraph.Axes.Item("XAxis").GetMinimum());


		int minT=0,maxT=0;
		for (int k=0;k<(int)kPhaseXA.size();k++)
		{
			if (kPhaseXA[k]>=xMin)
			{
				minT=kPhaseXA[k];
				break;
			}
		}

		for (int k=0;k<(int)kPhaseXA.size();k++)
		{
			if (kPhaseXA[k]<=xMax)
			{
				maxT=kPhaseXA[k];
			}
			else
				break;
		}

		int minKZ=0;
		int maxKZ=0;

		if (minT==maxT)
		{
			minT=minKZ;
			maxT=maxKZ;
		}
		if (maxT-minT>=kPhaseXA[1]-kPhaseXA[0])
		{
			xMax=maxT;
			xMin=minT;
			m_newGraph.Axes.Item("XAxis").SetMinimum(xMin*m_deltaFreq_A);
			m_newGraph.Axes.Item("XAxis").SetMaximum(xMax*m_deltaFreq_A);
		}
		else
		{
			xMax=maxT>minT?maxT:minT;
			xMin=maxT<minT?maxT:minT;
			m_newGraph.Axes.Item("XAxis").SetMinimum(xMin*m_deltaFreq_A);
			m_newGraph.Axes.Item("XAxis").SetMaximum(xMax*m_deltaFreq_A);

		}
		//}


		double minValue, maxValue, valueDecl;
		unsigned int minIndex, maxIndex;



		m_newGraph.Cursors.RemoveAll();
		m_newGraph.Cursors.Add();
		m_newGraph.SetTrackMode(CNiGraph::TrackDragCursor);
		m_newGraph.Cursors.Item(1).SetCrosshairStyle(CNiCursor::CrosshairMajorXMinorY);
		m_newGraph.Cursors.Item(1).SetColor(RGB(0,255,0));
		m_newGraph.Cursors.Item(1).SnapMode = CNiCursor::SnapNearestPoint;//SnapNearestPoint

		m_newGraph.Cursors.Item(1).SetPosition(xMin*m_deltaFreq_A,ABWData[xMin]);


		minKZ=xMin;
		maxKZ=xMax;

		int round=xMax-xMin-1;

		CNiReal64Vector newData(round);//将newData变为成员变量，便于保存当前时段波形数据
		newData.SetSize(round);
		if (round>16)
		{
			if (xMin>=0&&xMax<=(int)ABWData.GetSize()+1)
			{
				for (int i=0;i<round;i++)
				{
					newData[i]=ABWData[xMin];
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

		}






		CNiReal64Vector  interBBWData=re_timeA(newData);



		int dataSize = interBBWData.GetSize();                         //Number of data points acquired


		CNiReal64Vector realData, imagData;
		realData.SetSize(dataSize);
		imagData.SetSize(dataSize);

		CNiComplexReal64Vector fftData;

		realData+=interBBWData;

		try
		{
			CNiMath::FFT(realData,false,fftData);
		}
		catch(CException* e)
		{
			SetAParams(0.0,0.0,0.0,0.0);
			UpdateData(FALSE);
			return;
		}


		int numPts = dataSize/2;
		double scalingFactor = 1.00/(double)dataSize;
		double deltaFreq = ASamplingFreq*scalingFactor*(double)dataSize/(double)newData.GetSize();  




		CNiReal64Vector frequencyData;		//频谱图数据

		CNiReal64Vector temp(numPts);
		frequencyData=temp;
		double dtmp = 0;

		for( int i=1;i< numPts;i++)
		{
			dtmp = fftData[i].Magnitude();
			frequencyData[i] = fftData[i].Magnitude()*scalingFactor*2;

		}


		int  freqRange;						//频谱图范围

		// Scale DC value
		frequencyData[0] = 0;//fftData[0].Magnitude() * scalingFactor;//剔除直流分量
		freqRange=(int)(frequencyData.GetSize()*deltaFreq)+1;

		newData.MinMax(minValue, minIndex, maxValue, maxIndex);
		double rotateSpeed=ASamplingFreq*60.0*(double)m_afterBalanceWeight/((double)maxKZ-(double)minKZ);



		vector<double> OutX;
		for (int k=0;k<freqRange;k++)
		{
			OutX.push_back(k*deltaFreq);
		}

		double fKeyFreq=ASamplingFreq*(double)m_afterBalanceWeight/((double)maxKZ-(double)minKZ);

		vector<double>::iterator pLowerBound =
			std::lower_bound(OutX.begin(), OutX.end() - 1, fKeyFreq);
		int nKeyIndex =(int)(pLowerBound - OutX.begin());
		if (pLowerBound != OutX.begin())
		{
			double prv = *(pLowerBound - 1);
			if ( (*pLowerBound - fKeyFreq) > (fKeyFreq - prv) )
				nKeyIndex --;	// 上一个点更近，使用上一个点
		}

		FFTParam fp;
		fp.amplitude=frequencyData[nKeyIndex];
		fp.frequency=deltaFreq*nKeyIndex;
		fp.phase=atan2(fftData[nKeyIndex].imag,fftData[nKeyIndex].real);



		if (fp.phase<0)
			fp.phase=fp.phase*180.0/3.1415926535+360.0;
		else
			fp.phase=fp.phase*180.0/3.1415926535;
		SetAParams(rotateSpeed,(maxValue-minValue),2*fp.amplitude,fp.phase);
        AAmplitude=fp.amplitude;
		APhase=fp.phase;

		//balancingMethod(BAmplitude, BPhase,AAmplitude, APhase, param5,param7, outWeight, outAngle);
		UpdateData(FALSE);


		m_AFlag=false;
		DblClickCwgraph3();
	}
}
