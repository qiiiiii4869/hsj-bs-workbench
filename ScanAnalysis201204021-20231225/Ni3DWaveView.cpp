// T3DWaveView.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "Ni3DWaveView.h"
#include "MainFrm.h"
#include "SaveT3dViewDlg.h"

#include "Set3DFreqDlg.h"

#include "3DFreqShowCurveDlg.h"

//#include "msword.h"
//#include "excel.h"
// CSetAxis 对话框

IMPLEMENT_DYNAMIC(CSetAxis, CHSJDialog)
CNi3DWaveView* T3dView;

CString m_title;

extern bool g_bWaveNew;

CSetAxis::CSetAxis(CWnd* pParent /*=NULL*/)
: CHSJDialog(CSetAxis::IDD, pParent)/*,Xmin(0),Xmax(0),Ymin(0),Ymax(0),Zmin(0),Zmax(0)*/
, m_strFreqName(_T(""))
{
	T3dView=(CNi3DWaveView*)pParent;
	if(g_bWaveNew)
		m_strFreqName = _T("时间");
	else
		m_strFreqName = _T("频率");
}

CSetAxis::~CSetAxis()
{
}

void CSetAxis::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XMinEDIT, m_xMin);
	DDX_Text(pDX, IDC_XMaxEDIT, m_xMax);
	DDX_Text(pDX, IDC_YMinEDIT, m_yMin);
	DDX_Text(pDX, IDC_YMaxEDIT, m_yMax);
	DDX_Text(pDX, IDC_ZMinEDIT, m_zMin);
	DDX_Text(pDX, IDC_ZMaxEDIT, m_zMax);

	DDX_Control(pDX, IDC_STATIC_FREQ, m_stcFreqName);
}


BEGIN_MESSAGE_MAP(CSetAxis, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CSetAxis::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetAxis::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetAxis 消息处理程序

void CSetAxis::OnBnClickedOk()
{
	//CString xminstr,xmaxstr,yminstr,ymaxstr,zminstr,zmaxstr;

	//m_xMin.GetWindowText(xminstr);
	//m_xMax.GetWindowText(xmaxstr);

	//m_yMin.GetWindowText(yminstr);
	//m_yMax.GetWindowText(ymaxstr);

	//m_zMin.GetWindowText(zminstr);
	//m_zMax.GetWindowText(zmaxstr);

	//T3dView->Xmin=atoi(xminstr);
	//T3dView->Xmax=atoi(xmaxstr);
	//T3dView->Ymin=atoi(yminstr);
	//T3dView->Ymax=atoi(ymaxstr);
	//T3dView->Zmin=atoi(zminstr);
	//T3dView->Zmax=atoi(zmaxstr);
	if(UpdateData(TRUE))
	{

		T3dView->Xmin=m_xMin;
		T3dView->Xmax=m_xMax;
		T3dView->Ymin=m_yMin;
		T3dView->Ymax=m_yMax;
		T3dView->Zmin=m_zMin;
		T3dView->Zmax=m_zMax;
		OnOK();
	}

	//



}

BOOL CSetAxis::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	//m_xMin=T3dView->Xmin;
	//m_xMax=T3dView->Xmax;
	//m_yMin=T3dView->Ymin;
	//m_yMax=T3dView->Ymax;
	//m_zMin=T3dView->Zmin;
	//m_zMax=T3dView->Zmax;

	//CString xminstr,xmaxstr,yminstr,ymaxstr,zminstr,zmaxstr;

	//xminstr.Format("%d",Xmin);
	//m_xMin.SetWindowText(xminstr);
	//xmaxstr.Format("%d",Xmax);
	//m_xMax.SetWindowText(xmaxstr);

	//yminstr.Format("%d",Ymin);
	//m_yMin.SetWindowText(yminstr);
	//ymaxstr.Format("%d",Ymax);
	//m_yMax.SetWindowText(ymaxstr);

	//zminstr.Format("%d",Zmin);
	//m_zMin.SetWindowText(zminstr);
	//zmaxstr.Format("%d",Zmax);
	//m_zMax.SetWindowText(zmaxstr);

	//UpdateData(true);
	m_stcFreqName.SetWindowTextA(m_strFreqName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void  CSetAxis::SetAxisRange(int iXmin,int iXmax,int iYmin,int iYmax,int iZmin,int iZmax)
void  CSetAxis::SetAxisRange(float iXmin,float iXmax,float iYmin,float iYmax,float iZmin,float iZmax)
{
	m_xMin=iXmin;
	m_xMax=iXmax;
	m_yMin=iYmin;
	m_yMax=iYmax;
	m_zMin=iZmin;
	m_zMax=iZmax;
	//UpdateData(FALSE);
}
// T3DWaveView
using namespace std;
IMPLEMENT_DYNCREATE(CNi3DWaveView, CBCGPFormView)

CNi3DWaveView::CNi3DWaveView()
	: CBCGPFormView(CNi3DWaveView::IDD),unitX(""),unitY(""),unitZ("")
{

}

CNi3DWaveView::~CNi3DWaveView()
{

}

void CNi3DWaveView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH3D1, m_graph);
}

BEGIN_MESSAGE_MAP(CNi3DWaveView, CBCGPFormView)
	ON_WM_SIZE()
	ON_COMMAND(ID_SetAxisRange, &CNi3DWaveView::OnSetaxisrange)
	ON_COMMAND(ID_T3dSaveImage, &CNi3DWaveView::OnT3dsaveimage)
	//	ON_COMMAND(ID_SaveT3dViewBatDlg, &CNi3DWaveView::OnSavet3dviewbatdlg)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_SaveT3dViewBatDlg, &CNi3DWaveView::OnSavet3dviewbatdlg)
	//ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_3DFreqNew, &CNi3DWaveView::On3DFreqNew)////Marshall  2011.2.21 21:58
	ON_COMMAND(ID_SETFONT, &CNi3DWaveView::OnSetfont)
	ON_COMMAND(ID_SETCURVE, &CNi3DWaveView::OnSetcurve)
	//ON_COMMAND(ID_SAVET3DWAVEVIEWBATDLG, &CNi3DWaveView::OnSavet3dwaveviewbatdlg)
	ON_COMMAND(ID_3D_TITLE, &CNi3DWaveView::On3dTitle)
END_MESSAGE_MAP()


// T3DWaveView 诊断

#ifdef _DEBUG
void CNi3DWaveView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNi3DWaveView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// T3DWaveView 消息处理程序

void CNi3DWaveView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();
	HSJStyleChildControls(GetSafeHwnd(), m_skinFonts);
	HSJStyleGraph3D(m_graph);


	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/
	//调整界面，不出现滚动条////Marshall  2011.3.2 12:23
	SetScrollSizes(MM_TEXT,CSize(0,0));
	//InitNiGraph();
	ResizeParentToFit();
	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/

	colorVector.push_back(RGB(0,255,0));
	colorVector.push_back(RGB(255,0,0));
	colorVector.push_back(RGB(0,64,0));	
	colorVector.push_back(RGB(0,0,255));
	colorVector.push_back(RGB(255,0,255));
	colorVector.push_back(RGB(128,128,0));	
	colorVector.push_back(RGB(255,128,0));
	colorVector.push_back(RGB(0,64,255));

	//m_graph.SetCaption("");
	//-------------------1.0.0.2------------------------//
	//m_graph.Axes.Item("XAxis").SetCaption("趋势："+TrendCap+ "("+ unitX +")");
	m_graph.Axes.Item("XAxis").SetCaption(/*"趋势："+*/TrendCap+ "("+ unitX +")");//去掉趋势//Marshall  2011.3.1 14:0
	//--------------------------------------------------//


	//m_graph.Axes.Item("YAxis").SetCaption("频率 ("+ unitY +")");////Marshall  2011.2.24 18:41

	if(g_bWaveNew)//三维波形瀑布图//Marshall  2011.2.24 18:42
		m_graph.Axes.Item("YAxis").SetCaption("时间 ("+ unitY +")");
	else
		m_graph.Axes.Item("YAxis").SetCaption("频率 ("+ unitY +")");

	m_graph.Axes.Item("ZAxis").SetCaption("幅值 ("+ unitZ +")");

	//this->GetParentFrame()->SetWindowText(winTitle);
	//m_graph.SetCaption(winTitle);

	//std::map<CString,CNiReal64Vector>::iterator itx=XData.begin();
	//std::map<CString,CNiReal64Vector>::iterator ity=YData.begin();
	//std::map<CString,CNiReal64Vector>::iterator itz=ZData.begin();
	//int q=0;
	//for (;itx!=XData.end();itx++)
	//{
	//	q++;
	//	m_graph.Plots.Add();
	//	m_graph.Plots.Item(q).Plot3DCurve(itx->second, ity->second, itz->second);
	//	m_graph.Axes.Item("XAxis").SetMinMax(-20,100);
	//	//m_graph.Axes.Item("YAxis").SetMinMax(0,50);
	//	m_graph.Plots.Item(q).ColorMapStyle=CNiPlot3D::None;

	//	m_graph.Plots.Item(q).LineColor=colorVector[q%(int)colorVector.size()];

	//	m_graph.Plots.Item(q).PointColor=RGB(255,255,255);
	//	ity++;
	//	itz++;
	//}


}

void CNi3DWaveView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFormView::OnSize(nType, cx, cy);
	CWnd *pWnd; 

	pWnd = GetDlgItem(IDC_CWGRAPH3D1);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		pWnd->MoveWindow(0,0,cx,cy);//设置控件大小   //cxy20120410
	
	}
}

void CNi3DWaveView::SetChnData(CString file,CString exmName, CString type)
{
	FileStr=file;
	ExamNameStr=exmName;
	TypeStr=type;
}

/*--------------------------Marshall's Code2011.2.27 10:43--------------*/
//精确计算FFT
CNiReal64Vector CNi3DWaveView::re_time (CNiReal64Vector p1, double samplingFreq)
{
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
}
/*--------------------------Marshall's Code2011.2.27 10:43--------------*/
CNiReal64Vector CNi3DWaveView::GetFullPeriodData(const CNiReal64Vector& NiDt, int samplFreq)
{
	//CNiReal64Vector NiDt((unsigned int)vData.size());
	//vector<float>::iterator it2 = vData.begin();
	//int k=-1;//整周期处理//Marshall  2011.2.25 19:41
	//for(; it2 != vData.end(); it2++)
	//{
	//	k++;
	//	NiDt[k]=*it2;
	//}

	int minT=0,maxT=0;
	///*if(isHasKeyPhase)*///有键相点等周期时才对数据作处理//Marshall  2011.2.26 18:4
	//if(isHasKeyPhase)//有键相并且整周期时//Marshall  2011.2.26 18:10
	//{
		int xMin = 0;
		int xMax = NiDt.GetSize() - 1;//点的个数索引//Marshall  2011.2.25 20:10
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

		//找到位置//Marshall  2011.2.25 20:11
	//	int k = -1;
		int m = -1;
		//int nLen = maxT - minT+1;
		int nLen = maxT - minT -1;//注意数据长度,与原程序保持一致//Marshall  2011.2.26 15:17
		
	/*	CNiReal64Vector NiTemp;
		NiTemp.SetSize(nLen);*/
		CNiReal64Vector NiChData;
		NiChData.SetSize(nLen);

		for(int i = 0; i < NiDt.GetSize(); i++)
		{
			if(i>=minT && i<maxT-1)//注意数据长度nLen = maxT - minT -1;//Marshall  2011.2.26 15:17
				//NiDt[++m]=*it2;
				NiChData[++m] = NiDt[i];
				//NiTemp[++m]=*it2;
		}

		//NiChnData += NiTemp;

		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
		////打印数据，调式时用
		//FILE* f = fopen("c:\\setchnData_Excel.txt","w+");
		//for(int i = 0; i < NiChData.GetSize(); i++)
		//	fprintf(f,"%f\n",NiChData[i]);
		//fclose(f);
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
	//}
	//else
	//{
	//	int k=-1;//无键相则不取整周期//Marshall  2011.2.25 19:41
	//	for(; it2 != vData.end(); it2++)
	//	{
	//		k++;
	//		NiDt[k]=*it2;
	//	}
	//	NiChData=NiDt;
	//	
	//	/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
	//	////打印数据，调式时用
	//	//FILE* f = fopen("c:\\setchnData_Excel.txt","w+");
	//	//for(int i = 0; i < NiChData.GetSize(); i++)
	//	//	fprintf(f,"%f\n",NiChData[i]);
	//	//fclose(f);
	//	/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
	//}
	//
	////NiChData=NiDt;
	////int nL = NiChData.GetSize();//调试时用的代码//Marshall  2011.2.26 15:18
		return NiChData;
}

void CNi3DWaveView::SetFFTData(std::map<CString,std::vector<float>> multChnData,std::map<CString,std::vector<float>> multChnDataAcd,int samplFreq,CString title)
{
	std::map<CString,std::vector<float>> ChnData=multChnData;
	std::map<CString,std::vector<float>> ChnDataAcd=multChnDataAcd;
	std::map<CString,std::vector<float>>::iterator itchA=ChnDataAcd.begin();
	std::map<CString,std::vector<float>>::iterator itch=ChnData.begin();

	winTitle=title;
	m_title=title;
	this->GetParentFrame()->SetWindowText(title);
	m_graph.SetCaption(title);
	int q=0;

	m_graph.Plots.RemoveAll();//先清除曲线，之后再添加//Marshall  2011.2.24 16:41

	for(;itch!=ChnData.end();itch++)
	{
		CNiReal64Vector chDA((unsigned int)itchA->second.size());

		//把X值赋给chDA//
		vector<float>::iterator itDA = itchA->second.begin();
		int m=-1;
		for(; itDA != itchA->second.end(); itDA++)
		{
			m++;
			chDA[m]=*itDA;
		}

		double chnMean;
		CNiMath::Mean(chDA,chnMean);
		itchA++;
		//原程序没有给chDA赋值，所以出现趋势数据不正确的情况，现在添加数值就正确了//21.31 2011.4.26
		///////////////////////////////////

		q++;
		//m_graph.Plots.RemoveAll();
		CNiReal64Vector frequencyData;
		int  freqRange;
		double deltaFreq;
		int dataSize = (int)itch->second.size();                         //Number of data points acquired
		CNiReal64Vector realData, imagData;
		//realData.SetSize(dataSize);//如果有键相则使用整周期处理后的数据赋给realData//Marshall  2011.2.27 11:9
		imagData.SetSize(dataSize);

		CNiReal64Vector NiDt((unsigned int)itch->second.size());
		vector<float>::iterator it2 = itch->second.begin();
		int k=-1;
		for(; it2 != itch->second.end(); it2++)
		{
			k++;
			NiDt[k]=*it2;
		}
		
		CNiComplexReal64Vector fftData;

		/*--------------------------Marshall's Code2011.2.27 11:2--------------*/
		//如果有键相，则采用整周期处理,否则就取原始数据//Marshall  2011.2.27 11:3
		//CNiReal64Vector niTemp;
		if(isHasKeyPhase)
			realData = GetFullPeriodData(NiDt,samplFreq);
			//niTemp = GetFullPeriodData(NiDt,samplFreq);
		else
		{
			realData.SetSize(dataSize);
			realData += NiDt;
		}
		/*--------------------------Marshall's Code2011.2.27 11:2--------------*/
		if(bRetime)
			//realData =re_time(NiDt,samplFreq);//重采样//Marshall  2011.2.27 10:40
			realData =re_time(realData,samplFreq);//重采样//Marshall  2011.2.27 10:40
		//else
		//	//realData+=NiDt;
		//	realData = realData;

		// Transform the time domain data    
		CNiMath::FFT(realData,false,fftData);

		int numPts = dataSize/2;
		double scalingFactor = 1.0/(double)dataSize;
		deltaFreq = samplFreq*scalingFactor;    

		// Get only the first half of the double sided spectrum and scale it to return a
		// single sided spectrum in RMS.
		CNiReal64Vector temp(numPts);
		frequencyData=temp;
		for( int i=1;i< numPts;i++)
		{
			double x=fftData[i].Magnitude()*scalingFactor*2;
			frequencyData[i] = fftData[i].Magnitude()*scalingFactor*2;
		}

		// Scale DC value
		//frequencyData[0] = fftData[0].Magnitude() * scalingFactor;//去掉直流分量//Marshall  2011.2.24 9:57
		frequencyData[0]  = 0;

		freqRange=(int)(frequencyData.GetSize()*deltaFreq)+1;

		CNiReal64Vector xData((int)itch->second.size()),yData((int)itch->second.size());

		/*zData=frequencyData;*/
		for (int k=0;k<(int)itch->second.size();k++)
		{
			/************************************************************************/
					//	xData[k]=10*q+chnMean;	//原程序调试时用，现在修改//Marshall  2011.2.23 22:5                                
			  xData[k] = chnMean;
			/************************************************************************/

			yData[k]=k*deltaFreq;
		}
		double minValue, maxValue, valueDecl;
		unsigned int minIndex, maxIndex;
		xData.MinMax(minValue, minIndex, maxValue, maxIndex);
		valueDecl=(maxIndex-minIndex)*0.1;
		XData.insert(make_pair(itch->first,xData));
		YData.insert(make_pair(itch->first,yData));
		ZData.insert(make_pair(itch->first,frequencyData));
		m_graph.Plots.Add();
		m_graph.Plots.Item(q).Plot3DCurve(xData, yData, frequencyData);
		//m_graph.Axes.Item("XAxis").SetMinMax(-20,100);//注释掉//21.10 2011.4.26
		//m_graph.Axes.Item("XAxis").SetMinMax(minIndex-valueDecl,maxIndex+valueDecl);//added //21.10 2011.4.26
		//m_graph.Axes.Item("XAxis").SetMinMax(minIndex-valueDecl,maxIndex+valueDecl);
		//m_graph.Axes.Item("YAxis").SetMinMax(0,50);

/*
		if(int(minValue/10.0)<1 && int(maxValue/10.0)<1)
			m_graph.Axes.Item("XAxis").SetMinMax(minValue,maxValue);
		else
			m_graph.Axes.Item("XAxis").SetMinMax((int(minValue/10.0-1.0))*10,(int(maxValue/10.0+1.0))*10);
*/

		m_graph.Plots.Item(q).ColorMapStyle=CNiPlot3D::None;

		m_graph.Plots.Item(q).LineColor=colorVector[q%(int)colorVector.size()];

		m_graph.Plots.Item(q).PointColor=RGB(255,255,255);


		/*--------------------------Marshall's Code2011.2.24 15:24--------------*/
		//添加曲线名称
		m_graph.Plots.Item(q).Name = itch->first;
		/*--------------------------Marshall's Code2011.2.24 15:24--------------*/
	}
	
	//
}

//设置三维波形瀑布图数据//Marshall  2011.2.24 18:25
void CNi3DWaveView::SetWaveData(std::map<CString,std::vector<float>> multChnData,std::map<CString,std::vector<float>> multChnDataAcd,int samplFreq,CString title)
{


	std::map<CString,std::vector<float>> ChnData=multChnData;
	std::map<CString,std::vector<float>> ChnDataAcd=multChnDataAcd;
	std::map<CString,std::vector<float>>::iterator itchA=ChnDataAcd.begin();
	std::map<CString,std::vector<float>>::iterator itch=ChnData.begin();

	winTitle=title;
	m_title=title;
	this->GetParentFrame()->SetWindowText(title);
	m_graph.SetCaption(title);
	int q=0;

	m_graph.Plots.RemoveAll();//先清除曲线，之后再添加

	for(;itch!=ChnData.end();itch++)
	{
		CNiReal64Vector chDA((unsigned int)itchA->second.size());
		vector<float>::iterator itDA = itchA->second.begin();
		int m=-1;
		for(; itDA != itchA->second.end(); itDA++)
		{
			m++;
			chDA[m]=*itDA;
		}

		double chnMean;
		CNiMath::Mean(chDA,chnMean);
		itchA++;

		///////////////////////////////////

		q++;
		CNiReal64Vector frequencyData;
		int  freqRange;
		double deltaFreq;
		int dataSize = (int)itch->second.size();                        
		CNiReal64Vector realData, imagData;
		imagData.SetSize(dataSize);

		CNiReal64Vector NiDt((unsigned int)itch->second.size());
		vector<float>::iterator it2 = itch->second.begin();
		int k=-1;
		for(; it2 != itch->second.end(); it2++)
		{
			k++;
			NiDt[k]=*it2;
		}
/*
		CNiComplexReal64Vector fftData;

		
		if(isHasKeyPhase)
			realData = GetFullPeriodData(NiDt,samplFreq);
		else
		{
			realData.SetSize(dataSize);
			realData += NiDt;
		}

		if(bRetime)
			realData =re_time(realData,samplFreq);
  
		CNiMath::FFT(realData,false,fftData);

		int numPts = dataSize/2;
		double scalingFactor = 1.0/(double)dataSize;
		deltaFreq = samplFreq*scalingFactor;    
		CNiReal64Vector temp(numPts);
		frequencyData=temp;
		for( int i=1;i< numPts;i++)
		{
			double x=fftData[i].Magnitude()*scalingFactor*2;
			frequencyData[i] = fftData[i].Magnitude()*scalingFactor*2;
		}
		frequencyData[0]  = 0;

		freqRange=(int)(frequencyData.GetSize()*deltaFreq)+1;
	*/

		CNiReal64Vector xData((int)itch->second.size()),yData((int)itch->second.size());

		double deltaTime = 1.0 / samplFreq; 

		for (int k=0;k<(int)itch->second.size();k++)
		{
                             
			xData[k] = chnMean;
			yData[k]=k*deltaTime;
		}
		double minValue, maxValue, valueDecl;
		unsigned int minIndex, maxIndex;
		xData.MinMax(minValue, minIndex, maxValue, maxIndex);

		XData.insert(make_pair(itch->first,xData));
		YData.insert(make_pair(itch->first,yData));

		ZData.insert(make_pair(itch->first,NiDt));
		m_graph.Plots.Add();
		m_graph.Plots.Item(q).Plot3DCurve(xData, yData, NiDt);

	

		m_graph.Plots.Item(q).ColorMapStyle=CNiPlot3D::None;

		m_graph.Plots.Item(q).LineColor=colorVector[q%(int)colorVector.size()];

		m_graph.Plots.Item(q).PointColor=RGB(255,255,255);

		m_graph.Plots.Item(q).Name = itch->first;
		//OnSetaxisrange();
		
		
	}
	
/*	
    std::map<CString,std::vector<float>> ChnData=multChnData;
	std::map<CString,std::vector<float>> ChnDataAcd=multChnDataAcd;
	std::map<CString,std::vector<float>>::iterator itchA=ChnDataAcd.begin();
	std::map<CString,std::vector<float>>::iterator itch=ChnData.begin();

	winTitle=title;
	this->GetParentFrame()->SetWindowText(title);
	m_graph.SetCaption(title);
	int q=0;

	m_graph.Plots.RemoveAll();

	for(;itch!=ChnData.end();itch++)
	{
		CNiReal64Vector chDA((unsigned int)itchA->second.size());

		double chnMean;
		CNiMath::Mean(chDA,chnMean);
		itchA++;
		q++;
		CNiReal64Vector frequencyData;
		int  freqRange;
		int dataSize = (int)itch->second.size();  
		CNiReal64Vector NiDt((unsigned int)itch->second.size());
		vector<float>::iterator it2 = itch->second.begin();
		int k=-1;
		for(; it2 != itch->second.end(); it2++)
		{
			k++;
			NiDt[k]=*it2;
		}

   
		double deltaTime = 1.0 / samplFreq; 

		

		CNiReal64Vector xData((int)itch->second.size()),yData((int)itch->second.size());
		for (int k=0;k<(int)itch->second.size();k++)
		{
			
			 xData[k] = chnMean;

			yData[k]=k*deltaTime;
		}
		double minValue, maxValue, valueDecl;
		unsigned int minIndex, maxIndex;
		xData.MinMax(minValue, minIndex, maxValue, maxIndex);
	
		XData.insert(make_pair(itch->first,xData));
		YData.insert(make_pair(itch->first,yData));

		ZData.insert(make_pair(itch->first,NiDt));
		m_graph.Plots.Add();
		m_graph.Plots.Item(q).Plot3DCurve(xData, yData, NiDt);
		m_graph.Axes.Item("XAxis").SetMinMax(-20,100);

		m_graph.Plots.Item(q).ColorMapStyle=CNiPlot3D::None;

		m_graph.Plots.Item(q).LineColor=colorVector[q%(int)colorVector.size()];

		m_graph.Plots.Item(q).PointColor=RGB(255,255,255);

		m_graph.Plots.Item(q).Name = itch->first;

	}
*/



 
}

void CNi3DWaveView::OnSetaxisrange()
{
	CSetAxis sa(this);
	sa.SetAxisRange((float)m_graph.Axes.Item("XAxis").GetMinimum(),
					(float)m_graph.Axes.Item("XAxis").GetMaximum(),
					(float)m_graph.Axes.Item("YAxis").GetMinimum(),
					(float)m_graph.Axes.Item("YAxis").GetMaximum(),
					(float)m_graph.Axes.Item("ZAxis").GetMinimum(),
					(float)m_graph.Axes.Item("ZAxis").GetMaximum());
	if (sa.DoModal()==IDOK)
	{
		if(!g_bWaveNew)
//			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：设置三维坐标轴范围");
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维频谱瀑布图操作：设置三维坐标轴范围");
		else
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维波形瀑布图操作：设置三维坐标轴范围");
		m_graph.Axes.Item("XAxis").SetMinMax(Xmin,Xmax);
		m_graph.Axes.Item("YAxis").SetMinMax(Ymin,Ymax);
		m_graph.Axes.Item("ZAxis").SetMinMax(Zmin,Zmax);

	}
	else
	{
	//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：取消设置三维坐标轴范围");
		if(!g_bWaveNew)
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维频谱瀑布图操作：取消设置三维坐标轴范围");
		else
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维波形瀑布图操作：取消设置三维坐标轴范围");
	}
}
//消息：保存视图到图像
void CNi3DWaveView::OnT3dsaveimage()
{
	CImage bmp;	
	RECT controlBounds;

	m_graph.GetClientRect(&controlBounds);
	int bmpWidth = controlBounds.right - controlBounds.left;
	int bmpHeight = controlBounds.bottom - controlBounds.top;

	// Create the bitmap with the same dimensions of the control.

	bmp.Create(bmpWidth, bmpHeight, 24);

	// Play the enhanced metafile from the CNiPicture to the bitmap.
	RECT imageBounds = { 0, 0, bmpWidth, bmpHeight };
	CNiPicture controlImage = m_graph.ControlImage();
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
	m_graph.GetWindowText(strFileName);
	pszfilename=strFileName.GetBuffer(_MAX_PATH);
	FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  
	if(FileOpenDlg.DoModal() == IDOK)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：保存视图到图像...");

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
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：保存完成，存储为  "+strSaveFilapath);

	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：取消保存视图到图像");


	DeleteObject(bmp); 
}

void CNi3DWaveView::SaveAllView(CString Path,int type,int xmin,int xmax,int ymin,int ymax,BOOL isCAxis)/*int zmin,int zmax,*/
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
	if (isCAxis)
	{
		m_graph.Axes.Item("XAxis").SetMinMax(xmin,xmax);
		m_graph.Axes.Item("YAxis").SetMinMax(ymin,ymax);
		//m_graph.Axes.Item("ZAxis").SetMinMax(zmin,zmax);
	}



	CImage bmp;	
	RECT controlBounds;

	m_graph.GetClientRect(&controlBounds);
	int bmpWidth = controlBounds.right - controlBounds.left;
	int bmpHeight = controlBounds.bottom - controlBounds.top;

	// Create the bitmap with the same dimensions of the control.

	bmp.Create(bmpWidth, bmpHeight, 24);

	// Play the enhanced metafile from the CNiPicture to the bitmap.
	RECT imageBounds = { 0, 0, bmpWidth, bmpHeight };
	CNiPicture controlImage = m_graph.ControlImage();
	HDC bmpDC = bmp.GetDC();
	::PlayEnhMetaFile(bmpDC, reinterpret_cast< HENHMETAFILE >(controlImage.Handle), &imageBounds);
	

	CString filePath;

	CString strBuff ;
	CString   strFileName;   
	LPTSTR   pszfilename;   
	m_graph.GetWindowText(strFileName);
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
//void CNi3DWaveView::OnSavet3dviewbatdlg()
//{
//	CSaveT3dViewDlg st3d;
//	st3d.DoModal();
//}

int CNi3DWaveView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if(pMainFrame)
	{
		//pMainFrame->m_pExamView = this;
		pMainFrame->m_P3DFFT = this;
	}

	return 0;
}

HBRUSH CNi3DWaveView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (HSJSetWhiteControlColor(pDC, pWnd, nCtlColor)) return HSJWhiteBrush();
    return CBCGPFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}
//批量存储视图
void CNi3DWaveView::OnSavet3dviewbatdlg()
{
	CSaveT3dViewDlg st3d;
	if (st3d.DoModal()==IDCANCEL)
	{
		if(!g_bWaveNew)
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：取消批量存储视图到图像");
		else
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维波形瀑布图操作：取消批量存储视图到图像");

		return;
	}
	
}

//void CNi3DWaveView::OnSavet3dwaveviewbatdlg()
//{
//	CSaveT3dViewDlg st3d;
//	if (st3d.DoModal()==IDCANCEL)
//	{
//		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：取消批量存储视图到图像");
//
//		return;
//	}
//}
//void CNi3DWaveView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CBCGPFormView::OnActivate(nState, pWndOther, bMinimized);
//
//}

void CNi3DWaveView::OnSetFocus(CWnd* pOldWnd)
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	pCMF->ActivateTabEG(1);	
	CBCGPFormView::OnSetFocus(pOldWnd);
	

	// TODO: 在此处添加消息处理程序代码
}

void CSetAxis::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


/*--------------------------Marshall's Code2011.2.21 21:21--------------*/
//void CNi3DWaveView::OnContextMenu(CWnd* pWnd, CPoint point)
//{
//	AfxMessageBox("Hello");
//	CMenu menu;
//
//	VERIFY(menu.LoadMenu(IDR_3DFREQPOPUP));
//	
//	CMenu* pPopup = menu.GetSubMenu(0);
//	ASSERT(pPopup != NULL);
//	//if (menuShow==FALSE)
//	//{
//	//	CString str ;
//	//	for(int i=menu.GetSubMenu (0)->GetMenuItemCount()-1;i>=0;i--) 
//	//	{ 
//	//		menu.GetSubMenu (0)->GetMenuString(i,str,MF_BYPOSITION); 
//	//		if(str=="添加通道数据") 
//	//		{ 
//
//	//			menu.GetSubMenu (0)->DeleteMenu(i,MF_BYPOSITION);
//	//			menu.GetSubMenu (0)->DeleteMenu(i-1,MF_BYPOSITION);  
//	//			break; 
//	//		}
//	//	}
//	//	for(int i=menu.GetSubMenu (0)->GetMenuItemCount()-1;i>=0;i--) 
//	//	{ 
//	//		menu.GetSubMenu (0)->GetMenuString(i,str,MF_BYPOSITION); 
//	//		if(str=="显示健相") 
//	//		{ 
//	//			menu.GetSubMenu (0)->DeleteMenu(i,MF_BYPOSITION);
//	//			break; 
//	//		}
//	//	}
//	//	for(int i=menu.GetSubMenu (0)->GetMenuItemCount()-1;i>=0;i--) 
//	//	{ 
//	//		menu.GetSubMenu (0)->GetMenuString(i,str,MF_BYPOSITION); 
//	//		if(str=="整周期FFT") 
//	//		{ 
//
//	//			menu.GetSubMenu (0)->DeleteMenu(i,MF_BYPOSITION);
//	//			menu.GetSubMenu (0)->DeleteMenu(i-1,MF_BYPOSITION);  
//	//			break; 
//	//		}
//	//	}
//	//}
//	////(mainmenu->GetSubMenu (0))->AppendMenu (MF_SEPARATOR);//添加分隔符
//	////(mainmenu->GetSubMenu (0))->AppendMenu(MF_STRING,ID_APP_ABOUT,_T("Always on &Top")); //添加新的菜单项
//	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x, point.y, AfxGetMainWnd());
//
//	menu.Detach();
//}
/*--------------------------Marshall's Code2011.2.21 21:21--------------*/
BEGIN_EVENTSINK_MAP(CNi3DWaveView, CBCGPFormView)
ON_EVENT(CNi3DWaveView, IDC_CWGRAPH3D1, DISPID_MOUSEUP, CNi3DWaveView::MouseUpCwgraph3d1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CNi3DWaveView::MouseUpCwgraph3d1(short Button, short Shift, long X, long Y)
{
	if(Button==CNiGraph::RightButton)
	{
		LPPOINT lpoint=new tagPOINT;   
		::GetCursorPos(lpoint);                 //获得鼠标
		CMenu   menu,*pSubMenu; 
		menu.LoadMenu(IDR_3DFREQPOPUP);
		pSubMenu=menu.GetSubMenu(0);           //设置菜单弹出项目起始位置
		//获得鼠标位置
		if(pSubMenu != NULL)
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); 
		return;
	}
}

void CNi3DWaveView::On3DFreqNew()
{
	//MessageBox("Hello");
	CMainFrame *pMainFrm = (CMainFrame*) AfxGetApp()->GetMainWnd();
	if(pMainFrm != NULL)
		pMainFrm->SendMessage(WM_COMMAND,(WPARAM)ID_T3DFreqNew);	
}
/*--------------------------Marshall's Code2011.2.21 21:21--------------*/


void CNi3DWaveView::FontConvert(CFont& font,  CNiFont ni_font)
{

//Size 
//Bold 
 


	/*typedef struct tagLOGFONT { 
  LONG lfHeight; 
  LONG lfWidth; 
  LONG lfEscapement; 
  LONG lfOrientation; 
  LONG lfWeight; 
  BYTE lfItalic; 
  BYTE lfUnderline; 
  BYTE lfStrikeOut; 
  BYTE lfCharSet; 
  BYTE lfOutPrecision; 
  BYTE lfClipPrecision; 
  BYTE lfQuality; 
  BYTE lfPitchAndFamily; 
  TCHAR lfFaceName[LF_FACESIZE]; 
} LOGFONT, *PLOGFONT; */

	//LOGFONT lgfnt;
	////lgfnt.lfFaceName = ni_font.Name;
	//sprintf(lgfnt.lfFaceName,"%s",ni_font.Name);
	//lgfnt.lfWeight = ni_font.Weight;
	//lgfnt.lfItalic = ni_font.Italic;
	//lgfnt.lfUnderline = ni_font.Underline;
	//lgfnt.lfStrikeOut = ni_font.Strikethrough;
	//lgfnt.lfHeight = ni_font.Size * 10;//The size of the font in points.//Marshall  2011.2.23 11:30

	//LOGFONT lgfnt;
	//sprintf(lgfnt.lfFaceName,"%s",ni_font.Name);
	////lgfnt.lfFaceName = ni_font->Name;
	//lgfnt.lfWeight = ni_font->Weight;
	//lgfnt.lfItalic = ni_font->Italic;
	//lgfnt.lfUnderline = ni_font->Underline;
	//lgfnt.lfStrikeOut = ni_font->Strikethrough;
	//lgfnt.lfHeight = ni_font->Size * 10;//The size of the font in points.//Marshall  2011.2.23 11:30


	font.DeleteObject();
	font.CreatePointFont(ni_font.Size * 10, ni_font.Name);
	//font.CreatePointFontIndirect(&lgfnt);//这种方法得不到预想的结果//Marshall  2011.2.23 12:12
}

/*--------------------------Marshall's Code2011.2.22 9:3--------------*/
void CNi3DWaveView::OnSetfont()
{
	CNiFont grh_font = m_graph.Font;
	CNiFont xAxis_font = m_graph.Axes.Item("XAxis").CaptionFont;
	CNiFont yAxis_font = m_graph.Axes.Item("YAxis").CaptionFont;
	CNiFont zAxis_font = m_graph.Axes.Item("ZAxis").CaptionFont;

	CSet3DFreqDlg dlg;

	FontConvert(dlg.m_fntTl, grh_font);
	FontConvert(dlg.m_fntX, xAxis_font);
	FontConvert(dlg.m_fntY, yAxis_font);
	FontConvert(dlg.m_fntZ, zAxis_font);

	CNiColor clr = m_graph.CaptionColor;
	dlg.m_clrTl = RGB(clr.Red,clr.Green,clr.Blue);

	clr = m_graph.Axes.Item("XAxis").CaptionColor;
	dlg.m_clrX = RGB(clr.Red,clr.Green,clr.Blue);

	clr = m_graph.Axes.Item("YAxis").CaptionColor;
	dlg.m_clrY = RGB(clr.Red,clr.Green,clr.Blue);

	clr = m_graph.Axes.Item("ZAxis").CaptionColor;
	dlg.m_clrZ = RGB(clr.Red,clr.Green,clr.Blue);

	if(IDOK == dlg.DoModal())
	{
	/*	NiFontConvert(grh_font,dlg.m_fntTl);
		NiFontConvert(xAxis_font,dlg.m_fntX);
		NiFontConvert(yAxis_font,dlg.m_fntY);
		NiFontConvert(zAxis_font,dlg.m_fntZ);*/
	/*	grh_font	= NiFontConvert(dlg.m_fntTl);
		xAxis_font = NiFontConvert(dlg.m_fntX);
		yAxis_font = NiFontConvert(dlg.m_fntY);
		zAxis_font = NiFontConvert(dlg.m_fntZ);*/

		CClientDC dc(this); 

		LOGFONT lgfnt;
		dlg.m_fntTl.GetLogFont(&lgfnt);
		grh_font.SetName(lgfnt.lfFaceName);
		grh_font.SetWeight( lgfnt.lfWeight);
		grh_font.SetItalic(lgfnt.lfItalic);
		grh_font.SetUnderline(lgfnt.lfUnderline);
		grh_font.SetStrikethrough(lgfnt.lfStrikeOut);
		grh_font.SetSize(-MulDiv(lgfnt.lfHeight,72,dc.GetDeviceCaps(LOGPIXELSY)));
	//	grh_font.SetSize(lgfnt.lfHeight);

		dlg.m_fntX.GetLogFont(&lgfnt);
		xAxis_font.SetName(lgfnt.lfFaceName);
		xAxis_font.SetWeight( lgfnt.lfWeight);
		xAxis_font.SetItalic(lgfnt.lfItalic);
		xAxis_font.SetUnderline(lgfnt.lfUnderline);
		xAxis_font.SetStrikethrough(lgfnt.lfStrikeOut);
		xAxis_font.SetSize(-MulDiv(lgfnt.lfHeight,72,dc.GetDeviceCaps(LOGPIXELSY)));
		//xAxis_font.SetSize(lgfnt.lfHeight);

		dlg.m_fntY.GetLogFont(&lgfnt);
		yAxis_font.SetName(lgfnt.lfFaceName);
		yAxis_font.SetWeight( lgfnt.lfWeight);
		yAxis_font.SetItalic(lgfnt.lfItalic);
		yAxis_font.SetUnderline(lgfnt.lfUnderline);
		yAxis_font.SetStrikethrough(lgfnt.lfStrikeOut);
		yAxis_font.SetSize(-MulDiv(lgfnt.lfHeight,72,dc.GetDeviceCaps(LOGPIXELSY)));
		//yAxis_font.SetSize(lgfnt.lfHeight );

		dlg.m_fntZ.GetLogFont(&lgfnt);
		zAxis_font.SetName(lgfnt.lfFaceName);
		zAxis_font.SetWeight( lgfnt.lfWeight);
		zAxis_font.SetItalic(lgfnt.lfItalic);
		zAxis_font.SetUnderline(lgfnt.lfUnderline);
		zAxis_font.SetStrikethrough(lgfnt.lfStrikeOut);
		zAxis_font.SetSize(-MulDiv(lgfnt.lfHeight,72,dc.GetDeviceCaps(LOGPIXELSY)));
		//zAxis_font.SetSize(lgfnt.lfHeight);

		m_graph.SetFont(grh_font);
		m_graph.Axes.Item("XAxis").SetCaptionFont(xAxis_font);
		m_graph.Axes.Item("YAxis").SetCaptionFont(yAxis_font);
		m_graph.Axes.Item("ZAxis").SetCaptionFont(zAxis_font);


		clr.ConvertFromOleColor(dlg.m_clrTl);
		m_graph.SetCaptionColor(clr);

		clr.ConvertFromOleColor(dlg.m_clrX);
		m_graph.Axes.Item("XAxis").SetCaptionColor(clr);
		
		clr.ConvertFromOleColor(dlg.m_clrY);
		m_graph.Axes.Item("YAxis").SetCaptionColor(clr);

		clr.ConvertFromOleColor(dlg.m_clrZ);
		m_graph.Axes.Item("ZAxis").SetCaptionColor(clr);

	}
}

CNiFont CNi3DWaveView::NiFontConvert(  CFont font)
{
	//	LOGFONT lgfnt;
	////lgfnt.lfFaceName = ni_font.Name;
	//sprintf(lgfnt.lfFaceName,"%s",ni_font.Name);
	//lgfnt.lfWeight = ni_font.Weight;
	//lgfnt.lfItalic = ni_font.Italic;
	//lgfnt.lfUnderline = ni_font.Underline;
	//lgfnt.lfStrikeOut = ni_font.Strikethrough;
	//lgfnt.lfHeight = ni_font.Size * 10;
    CNiFont ni_font;

	LOGFONT lgfnt;
	//CFont fnt(font);
	//fnt = font;
	font.GetLogFont(&lgfnt);
	//fnt.GetLogFont(&lgfnt);
	//ni_font.Name = lgfnt.lfFaceName;
	//ni_font.Weight = lgfnt.lfWeight;
	//ni_font.Italic = lgfnt.lfItalic;
	//ni_font.Underline = lgfnt.lfUnderline;
	//ni_font.Strikethrough = lgfnt.lfStrikeOut;
	//ni_font.Size = lgfnt.lfHeight / 10;
	ni_font.SetName(lgfnt.lfFaceName);
	ni_font.SetWeight( lgfnt.lfWeight);
	ni_font.SetItalic(lgfnt.lfItalic);
	ni_font.SetUnderline(lgfnt.lfUnderline);
	ni_font.SetStrikethrough(lgfnt.lfStrikeOut);
	ni_font.SetSize(lgfnt.lfHeight / 10);

	return ni_font;
}
/*--------------------------Marshall's Code2011.2.22 9:3--------------*/

void CNi3DWaveView::OnSetcurve()
{
	C3DFreqShowCurveDlg dlg;
	dlg.m_pWnd = this;
	int nCnt = m_graph.Plots.Count;	

	for(int i = 0; i < nCnt; i++)
	{
		CNiPlot3D  plot = m_graph.Plots.Item(i+1);
		CString strPlotName = plot.GetName();
		bool	bVisible = plot.Visible;
		CurveSet curveset;
		curveset.strCurveName = strPlotName;
		curveset.bVisible = bVisible;
		dlg.m_listSet.AddTail(curveset);
	}

	dlg.DoModal();
}



void CNi3DWaveView::On3dTitle()
{
	CSetTitle setTitleDlg;

 	setTitleDlg.mTitle = m_title;

	if (setTitleDlg.DoModal()==IDOK)
	{
//		m_stcFreqName.SetWindowText(setTitleDlg.mTitle);
		//m_strWaveTitle = setTitleDlg.mTitle;//标题设置//Marshall  2011.2.28 15:27
		//setTitleDlg.UpdateData(true);
		//this->GetParentFrame()->SetWindowText(title);
		m_graph.SetCaption(setTitleDlg.mTitle);
//		Invalidate();

	}
}
