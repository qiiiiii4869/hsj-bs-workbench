#pragma once
#include "nigraph.h"
#include <vector>
#include "AxisOrbit.h"

// CCorrelation 窗体视图

class CCorrelation : public CFormView
{
	DECLARE_DYNCREATE(CCorrelation)
	CHSJFontStore m_skinFonts; // Child typefaces only; original font sizes and bounds.


protected:
	CCorrelation();           // 动态创建所使用的受保护的构造函数
	virtual ~CCorrelation();

public:
	enum { IDD = IDD_CorrView };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_deltaFreq,m_deltaFreqX,m_deltaFreqY;
	NI::CNiGraph m_GraphX;									//X数据图谱
	NI::CNiGraph m_GraphY;									//Y数据图谱
	NI::CNiGraph m_GraphAxis;								//二维轴心轨迹
	CNiReal64Vector dataX,dataY;							//X,Y通道数据
	CString XTitle;											//X标题
	CString YTitle;											//Y标题
	CString WinTitle;										//窗体标题
	CString FunTitle;										//“二维轴心轨迹”
	//std::vector<QuaternionX> qVector;						//存储缩放数据
	std::vector<QuaternionX> qVectorCX;						//存储缩放数据
	std::vector<QuaternionX> qVectorCY;						//存储缩放数据
	BOOL isWaveFilter;										//是否滤波
	int samplingFreq;										//采样频率
	CNiReal64Vector ResumeDataX;							//X波形图重绘数据
	CNiReal64Vector ResumeDataY;							//Y波形图重绘数据


	void SetQuaternionX(NI::CNiGraph* mG1, NI::CNiGraph* mG2);//存储图形范围
	void GetQuaternionX(NI::CNiGraph* mG1, NI::CNiGraph* mG2);//获取图形范围
	void SetQuaternionY(NI::CNiGraph* mG1, NI::CNiGraph* mG2);//存储图形范围
	void GetQuaternionY(NI::CNiGraph* mG1, NI::CNiGraph* mG2);//获取图形范围
	double GetXRang(NI::CNiGraph* mG);						 //获取波形图范围
	void SetAxisData(NI::CNiGraph* mG,NI::CNiGraph* mG2);						 //设置通道数据
	void SetDataX(std::vector <float> vData,int sampling,CString title);  //设置通道X参照坐标数据
	void SetDataY(std::vector <float> vData,int sampling,CString title);  //设置通道Y参照坐标数据
	void SetWinTitle(CString title);						 //设置标题
	void SetFunSel();								 //根据类型设置波形图显示样式

public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	DECLARE_EVENTSINK_MAP()
public:
	void ZoomCwgraphx();
public:
	void ZoomCwgraphy();
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnCoherentresetX();
public:
	afx_msg void OnCoherentsavetoimage();
public:
	afx_msg void OnCoherentunzoomX();
public:
	afx_msg void OnCoherentresetY();
public:
	afx_msg void OnCoherentunzoomY();
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	afx_msg void OnCorrTitle();
};
