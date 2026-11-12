// boxingView.h : interface of the CBoxingView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOXINGVIEW_H__4F80A6A3_26DC_4E9E_AAFD_A8D607AA70A1__INCLUDED_)
#define AFX_BOXINGVIEW_H__4F80A6A3_26DC_4E9E_AAFD_A8D607AA70A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBoxingDoc;
class CBoxingView : public CScrollView
{
protected: // create from serialization only
	CBoxingView();
	DECLARE_DYNCREATE(CBoxingView)

	//enum { IDD = IDD_StripChart };
// Attributes
public:
	int flag;
	int yGridCount;
	float xinterval;
	CSize m_ViewPortExt;
	CPoint m_ViewOrg;
	int m_Rows;				//网格行
	int m_Columns;			//网格列
	float m_StartTime;		//起始时间
	float m_EndTime;		//终止时间
	int m_viewportx;
	int m_Count;
	int GriWidth;			//网格宽度
	int GriHeight;			//网格高度
	int startx;
	int starty;
	CPoint oldpoint;
	CPoint oldpoint2;
	int moveindex;
	int saveflag;
	int delflag;
	int selindex;			//被选图谱的索引
	CString savename;		//保存文件名
	//CString filename;		//文件名
	CString m_title;		//标题
	CBitmap GraphBmp;		//存储位图
	CString filePath;		//分拣文件路径
	CString examType;		//试验类型
	CString examParam;		//试验参数
	//int sampleFreq;			//采样频率

	//BOOL isConfigX;
	typedef struct
	{
		float x;
		float y;
	}LOGPOINT;					//数据点
	typedef struct sboxing
	{
		LOGPOINT *logpoint;		//坐标点
		int count;				//点数
		int windowextY;			//坐标包围区域Y窗体坐标
		int viewportextY;		//坐标包围区域Y视图坐标
		int vieworgY;			//Y视图原始坐标
		float ymax;				//y最大值
		float ymin;				//y最小值
		int pos;				//坐标包围序列
		int startindex;			//y起始网格号
		int endindex;			//y终止网格号
		int pix;				//曲线宽度
		int offsety;			//y轴偏移量
		float yinterval;		//y间隔数值
		int frequency;			//频率
		CString title;			//标题
		COLORREF color;			//颜色
		sboxing *pNext;			//下一个坐标包围区域
	}BOXING;
	CArray <BOXING, BOXING&> boxing;

public:
	CBoxingDoc* GetDocument();
	void SetInit(CSize vpt,int flg,int ygc,int rs,int mc,float st,float et,float xi);	//长图网格初始化
	void SetChnInit(CString fPath,CString eType,CString eParam/*,int sFreq*/);				//初始化通道数据
	//void SetConfigX(float ms, float me, int mc);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoxingView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	//CDC dcMem;
	void DrawGraph();
	BOOL SaveBitMapToFile(HBITMAP hBitmap, CString lpFileName);
	virtual ~CBoxingView();
	void LPtoDPEx(float xlog, float ylog, float WindowExtX, float WindowExtY, CSize ViewPortExt, CPoint ViewOrg, CPoint& point);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBoxingView)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSetRange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSaveToImage();
	afx_msg void OnAddSignal();
	afx_msg void OnAddGroup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // debug version in boxingView.cpp
inline CBoxingDoc* CBoxingView::GetDocument()
   { return (CBoxingDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOXINGVIEW_H__4F80A6A3_26DC_4E9E_AAFD_A8D607AA70A1__INCLUDED_)
