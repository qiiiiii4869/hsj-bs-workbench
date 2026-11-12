#pragma once

#include "TrendSelSet.h"
#include "nigraph.h"
#include <vector>

// CTrendView 窗体视图

class CTrendView : public CBCGPFormView
{
	DECLARE_DYNCREATE(CTrendView)
	CHSJFontStore m_skinFonts; // Child typefaces only; original font sizes and bounds.


protected:
	CTrendView();           // 动态创建所使用的受保护的构造函数
	virtual ~CTrendView();
	
	NI::CNiGraph m_TrendGraph;
	
	CString title;
	BOOL showMean;
	BOOL showMedian;
	BOOL showRMS;
	BOOL showPPvalue;

	//std::vector<double> mean;
	//std::vector<double> median;
	//std::vector<double> RMS;
	//std::vector<double> ppValue;
	std::vector<TrendGraph> GV;
	int ichSize;
	CNiReal64Vector GxData;
	CNiReal64Vector Gmean;
	CNiReal64Vector Gmedian;
	CNiReal64Vector GRMS;
	CNiReal64Vector GppValue;
	//std::vector<double>  GyData;
public:
	enum { IDD = IDD_TRENDVIEW };
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void SetTrendParam(CString t,int xmin,int xmax,int ymin,int ymax,COLORREF c1,COLORREF c2,COLORREF c3,COLORREF c4);
	void SetTrendData(std::map<CString,std::vector<float>> multChnData,std::map<CString,std::vector<float>> multChnDataAcd,std::vector<TrendGraph> gVector,CString t);
	void SaveAllView(CString Path,int type);
public:
	void SaveAllView();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnTrendimagesave();
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnTrendmean();
public:
	afx_msg void OnUpdateTrendmean(CCmdUI *pCmdUI);
public:
	afx_msg void OnTrendmedian();
public:
	afx_msg void OnUpdateTrendmedian(CCmdUI *pCmdUI);
public:
	afx_msg void OnTrendrms();
public:
	afx_msg void OnUpdateTrendrms(CCmdUI *pCmdUI);
public:
	afx_msg void OnTrendppvalue();
public:
	afx_msg void OnUpdateTrendppvalue(CCmdUI *pCmdUI);
public:
	afx_msg void OnSettrendparam();
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
//	afx_msg void OnLoadtxtjigger();
public:
	afx_msg void OnTrendreport();
public:
	afx_msg void OnTrendimagesavebat();
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);

protected:
	virtual void OnDraw(CDC* /*pDC*/);//调整界面//Marshall  2011.3.3 13:6
};

