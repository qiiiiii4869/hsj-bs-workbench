#pragma once
#include "afxwin.h"
#include <vector>
#include "CWaveFreqCom.h"
#include "ReportCtrl.h"
#include "afxcmn.h"
// CDynamicEquilibrium 窗体视图

class CDynamicEquilibrium : public CFormView
{
	DECLARE_DYNCREATE(CDynamicEquilibrium)
	CHSJFontStore m_skinFonts; // Child typefaces only; original font sizes and bounds.


protected:
	virtual ~CDynamicEquilibrium();

public:
	enum { IDD = IDD_DynamicEquilibrium };
public:

	CDynamicEquilibrium();           // 动态创建所使用的受保护的构造函数
	NI::CNiGraph m_Graph;
	NI::CNiGraph m_newGraph;
	NI::CNiGraph m_DynEquGraph;
	BOOL m_bClockwise;
	int BSamplingFreq;
	int ASamplingFreq;

	double m_deltaFreq_B;
	double m_deltaFreq_A;
	double m_deltaFreq_Key;

	std::vector<float> kPhaseXA;
	std::vector<float> kPhaseYA;
	std::vector<float> kPhaseXB;
	std::vector<float> kPhaseYB;
	double BAmplitude;
	double AAmplitude;
	double BPhase;
	double APhase;
	int intermetallicSize;//键相间隔大小

	double tWAngle;
	double tBAngle;
	double outWeight;
	double outAngle;
 
	//double diss1;
	//double diss2;
	BOOL isHasKeyPhase;
	BOOL isWeightVisual;
	BOOL isBDiss;
	//int nItem, nSubItem;
	CReportCtrl	m_wndList;
	//CString m_editStr;

	//std::vector<Quaternion> qVector;

	CNiReal64Vector  BBWData;
	CNiReal64Vector  ABWData;
	//CNiReal64Vector aNewdata;
	//CNiReal64Vector bNewdata;
	//std::vector<Quaternion> qNewVector;

	void SetBBWData(std::vector <float> vData,int sFreq,CString t);
	void SetABWData(std::vector <float> vData,int sFreq,CString t);
	void DrawBBWData(CNiReal64Vector  bBWData,int m_begin=0);
	void DrawABWData(CNiReal64Vector  aBWData,int m_begin=0);
	void IsertDataInit();
public:
	void SetKeyPhaseA(std::vector<float> kp,int key_freq,int samplingFreq);
	void SetKeyPhaseB(std::vector<float> kp,int key_freq,int samplingFreq);
	void CompDyn(NMHDR *pN);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	//void InsertItems();
	//void SetCell(HWND hWnd1, CString value, int nRow, int nCol);
	//CString GetItemText(HWND hWnd, int nItem, int nSubItem) const;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void FillRect();
	void FitHeaderLabels();
	void DrawDynEquGraph(double bR,double aR,double bA,double aA,double tW,double tB,double di1,double di2);
	CNiReal64Vector re_timeA (CNiReal64Vector p1);
	CNiReal64Vector re_timeB (CNiReal64Vector p1);
	void SetBParams(double rs,double fs,double am,double ph);
	void SetAParams(double rs,double fs,double am,double ph);
	bool balancingMethod(double amplitude1, double phase1,double amplitude2, double phase2, double angleTest, double weightTest,double& outWeight, double& outAngle);
	bool balancingMethod2(double amplitude1, double phase1,double amplitude2, double phase2, double weightTest,double& outWeight, double& outAngle);
	bool balanceBreak (double angle, double v, double a0, double a1, double *v0, double *v1);

	void GetAParams();
	int IDX0;
	int IDX1;
	int IDX2;
	int IDX3;
	int IDX4;
	int m_B_p;
	int m_A_p;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	virtual void OnInitialUpdate();
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CComboBox m_rotateTypeComb;
public:
	int m_beforeBalanceWeight;

	int m_AAStart;
	int m_BBStart;

	int m_AAEnd;
	int m_BBEnd;

	bool m_AFlag;
	bool m_BFlag;

	bool m_AAFlag;
	bool m_BBFlag;
	std::vector<Quaternion> qVectorF;	//记录频谱图平移缩放坐标范围

public:
	int m_afterBalanceWeight;
public:
	DECLARE_EVENTSINK_MAP()
	
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
	afx_msg void OnLoadbbwdata();
public:
	afx_msg void OnLoadabwdata();
public:
	afx_msg void OnDeleteBSel();
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	void DblClickCwgraph1();
public:
	void CursorChangeCwgraph1(long* CursorIndex, VARIANT* XPos, VARIANT* YPos, BOOL* bTracking);
public:
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void CursorChangeCwgraph3(long* CursorIndex, VARIANT* XPos, VARIANT* YPos, BOOL* bTracking);
public:
	void DblClickCwgraph3();
public:
	afx_msg void OnDeleteasel();

public:
//	afx_msg void OnNMKillfocusList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnSavebwimage();
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	afx_msg void OnEditWeight();
public:
	void MouseUpCwgraph1(short Button, short Shift, long x, long y);
public:
	void MouseDownCwgraph1(short Button, short Shift, long x, long y);
public:
	afx_msg void ONBAnalysis();
public:
	afx_msg void ONAAnalysis();
};


