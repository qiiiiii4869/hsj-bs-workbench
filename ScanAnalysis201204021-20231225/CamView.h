#pragma once
#include <vector>

// CCamView 窗体视图

class CCamView : public CFormView
{
	DECLARE_DYNCREATE(CCamView)
	CHSJFontStore m_skinFonts; // Child typefaces only; original font sizes and bounds.


protected:
	CCamView();					// 动态创建所使用的受保护的构造函数
	virtual ~CCamView();
	NI::CNiGraph m_Graph;		//协联工况图形
	
	std::vector<double> PX2,PY2;//记录协联工况X，Y坐标数据
public:
	enum { IDD = IDD_CAMVIEW };	
	FILE * file;
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//==============  光顺样条函数
	void smooth (int n,double x[],double y[],double dy[],double s,
		double a[],double b[],double c[],double d[]);


	//===============   autoCAD接口函数

	//------------------ 在CAD图面上设定一个点 -------------------------
	void point (FILE *fp,double x,double y);

	//------------------ 在CAD图上绘制曲线 -------------------------
	void akurve2 (FILE *fp,int n,double x[],double y[]);

	//-------------- sqrt(x^2+y^2)----------
	double cll(double x,double y);

	//====================  二次样条函数

	//--------- 一个区间中二次样条函数求值公式
	double sp2(double x,double x1,double x2,double y1,double y2,
		double dy1,double dy2);

	//--------- 一个区间中二次样条函数求导公式
	double sp2d1(double x,double x1,double x2,double y1,
		double y2,double dy1,double dy2);

	//--------- 一条曲线上二次样条函数插值公式
	double spinte(int n,double xx,double x[],double y[],double dy[]);

	//--------- 一条曲线上二次样条函数求导公式
	double dspinte(int n,double xx,double x[],double y[],double dy[]);

	//--------- 求二次样条函数节点导数值
	void quasp2 (int n,double x[],double y[],double dy[]);

	//-------------- 
	void quasp2l(int n,double x[],double y[],double l[],double dx[],double dy[]);

	//============= 求包络线的函数

	//-------------------- 求斜率等于给定值的点 ---------------------
	double d_dd(int n,double x[],double y[],double dy[],double dd,double &xx);
	double d_dl (int n,double l[],double x[],double y[],double dx[],double dy[],double dd,double &ll);

	//---------------------- 求协联工况点 ---------------------------
	void oncam (int nb,int bpoint[],double *l[],double *q[],double *eta[],double *dq[],
		double *de[],double qc[],double ec[],double dec[]);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSavecamdata();
public:
	afx_msg void OnSavecamtopic();
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
