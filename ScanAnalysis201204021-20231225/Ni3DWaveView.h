#pragma once

#include "NiGraph3D.h"
#include "NiAxis3d.h"	// Added by ClassView
#include <vector>
#include "afxwin.h"
// CSetAxis 对话框

class CSetAxis : public CHSJDialog
{
	DECLARE_DYNAMIC(CSetAxis)

public:
	CSetAxis(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetAxis();

	// 对话框数据
	enum { IDD = IDD_SETT3DAXISDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
//public:
//	int Xmin;
//	int Xmax;
//	int Ymin;
//	int Ymax;
//	int Zmin;
//	int Zmax;

public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
public:
	//int m_xMin;
	//int m_xMax;
	//int m_yMin;
	//int m_yMax;
	//int m_zMin;
	//int m_zMax;

	float m_xMin;
	float m_xMax;
	float m_yMin;
	float m_yMax;
	float m_zMin;
	float m_zMax;

	//void SetAxisRange(int iXmin,int iXmax,int iYmin,int iYmax,int iZmin,int iZmax);
	void SetAxisRange(float iXmin,float iXmax,float iYmin,float iYmax,float iZmin,float iZmax);
public:
	afx_msg void OnBnClickedCancel();
public:
	CStatic m_stcFreqName;
public:
	CString m_strFreqName;
};


// T3DWaveView 窗体视图

class CNi3DWaveView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CNi3DWaveView)
	CHSJFontStore m_skinFonts; // Child typefaces only; original font sizes and bounds.

public:

//protected:
	CNi3DWaveView();           // 动态创建所使用的受保护的构造函数
	virtual ~CNi3DWaveView();
	
	//CNiAxis3D m_Axis3D;
	//CNiGraph3D	m_graph;
public:
	enum { IDD = IDD_3DWaveVIEW };

	//int Xmin;
	//int Xmax;
	//int Ymin;
	//int Ymax;
	//int Zmin;
	//int Zmax;

	float Xmin;
	float Xmax;
	float Ymin;
	float Ymax;
	float Zmin;
	float Zmax;

	CString unitX;
	CString unitY;
	CString unitZ;
	CString TrendCap;

	std::map<CString,CNiReal64Vector> XData;
	std::map<CString,CNiReal64Vector> YData;
	std::map<CString,CNiReal64Vector> ZData;
	CString  winTitle;
	CString FileStr;
	CString ExamNameStr;
	CString TypeStr;
	CString ParamStr;
	CString chnStr;
	std::vector<COLORREF> colorVector;
	void SetChnData(CString file,CString exmName, CString type);
	void SetFFTData(std::map<CString,std::vector<float>> multChnData, std::map<CString,std::vector<float>> multChnDataAcd, int samplFreq,CString title);
	void SaveAllView(CString Path,int type,int xmin,int xmax,int ymin,int ymax,BOOL isCAxis);/*int Zmin,int zmax,*/
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
	virtual void OnInitialUpdate();
public:
	CNiGraph3D m_graph;
	CNiAxis3D m_Axis3D;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnSetaxisrange();
public:
	afx_msg void OnT3dsaveimage();
//public:
//	afx_msg void OnSavet3dviewbatdlg();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnSavet3dviewbatdlg();
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	////Marshall  2011.2.21 21:25
	//afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void On3DFreqNew();
public:
	DECLARE_EVENTSINK_MAP()
public:
	void MouseUpCwgraph3d1(short Button, short Shift, long X, long Y);////Marshall  2011.2.21 21:50
public:
	afx_msg void OnSetfont();////Marshall  2011.2.23 11:6
public:
	void FontConvert(CFont& font,  CNiFont ni_font);
	CNiFont NiFontConvert( CFont font);
public:
	afx_msg void OnSetcurve();
public:
	//设置三维波形瀑布图数据//Marshall  2011.2.24 18:26
	void SetWaveData(std::map<CString,std::vector<float>> multChnData,std::map<CString,std::vector<float>> multChnDataAcd,int samplFreq,CString title);
	CNiReal64Vector re_time (CNiReal64Vector p1, double samplingFreq);//重采样//Marshall  2011.2.27 10:43
	CNiReal64Vector GetFullPeriodData(const CNiReal64Vector& NiDt, int samplFreq);//获取整周期数据////Marshall  2011.2.27 11:27
	//void SetKeyPhase(std::vector<float> kp,int key_freq, int wave_freq,int nDataLen);
	BOOL isHasKeyPhase;					//是否存在正确的健相信息
	std::vector<float> kPhaseX;			//健相X坐标集合
public:
	afx_msg void OnSavet3dwaveviewbatdlg();
public:
	//afx_msg void OnTitle();
public:
	afx_msg void On3dTitle();
};


