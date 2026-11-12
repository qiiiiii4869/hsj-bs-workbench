// NiCtrlTestView.h : CNiCtrlTestView 类的接口
//


#pragma once
#include "nigraph.h"
#include <vector>
#include "afxwin.h"
#include "CWaveFreqCom.h"

class CSetTitle : public CHSJDialog
{
	DECLARE_DYNAMIC(CSetTitle)

public:
	CSetTitle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetTitle();

	// 对话框数据
	enum { IDD = IDD_SetTitle };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_title;
	CString mTitle;			//标题
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};

class CNiWaveFraqView : public CBCGPFormView//CFormView
{
public: // 仅从序列化创建
	CNiWaveFraqView();
	DECLARE_DYNCREATE(CNiWaveFraqView)

public:
	enum{ IDD = IDD_NIWAVEFRAQ_FORM };
	CFont m_skinTitleFont; // Owned for the lifetime of this view.

// 属性
public:
	//CScanAnalysisDoc* GetDocument() const;
	
// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	BOOL SaveWaveTemp();
	//void SaveFraqTemp();
// 实现
public:
	virtual ~CNiWaveFraqView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:

	double m_baseTime,m_maxminTime;
	BOOL m_isAnnotation;
	double m_myMin,m_myMax;
	int m_wavecount;

	int m_fontSize;

	int m_width;
	int m_increase;


	


	BOOL isGraph;
	BOOL isAnnotation;

	BOOL m_batchSaveBit;
	int m_minX,m_maxX;
	double m_deltaFreq;
	NI::CNiGraph m_Graph;				//波形图
	NI::CNiGraph m_newGraph;			//频谱图
	//CNiReal64Vector data;				
	CNiReal64Vector NiData;				//波形图数据

	CNiReal64Vector newData;			//将newData变为成员变量，便于保存当前时段波形数据//Marshall  2011.3.1 14:43

	CNiReal64Vector ResumeData;			//波形图重绘数据
	double deltaFreq;					//频谱增量
	int xRange;							//波形图X范围
	std::vector<Quaternion> qVector;	//记录波形图平移缩放坐标范围
	std::vector<Quaternion> qVectorF;	//记录频谱图平移缩放坐标范围
	int annotationsVisual;				//是否标注
	//int pointVisual;					
	int currorFVisual;					//波形图捡取浮动点光标操作计数
	int currorNVisual;					//波形图捡取临近点光标操作计数
	int currorNVisualF;					//频谱图捡取浮动点光标操作计数
	CNiReal64Vector frequencyData;		//频谱图数据
	//std::vector<int> ExcelIDVector;	
	//std::vector<CString> ExcelChnVector;
	//std::vector<CString> ExcelHeadVector;
	
	CNiReal64Vector KeyPhase;			//键相
	BOOL isKeyPhaseVisual;				//是否显示健相点
	BOOL isFullPeriodFFT;				//是否进行整周期FFT运算
	//BOOL isFFT;						//是否进行FFT变换
	CString unitStr;					//单位
	unsigned char ChnFftFlag;			//fft计算开关
	BOOL isWavegraph;					//是否仅显示波形图
	BOOL isFreqgraph;					//是否仅显示频谱图
	BOOL isWaveFreq;					//是否全部显示波形频谱图

	std::vector<float> kPhaseX;			//健相X坐标集合

	std::vector<float> kPhaseY;			//健相Y坐标集合
	int minKZ;							//整周期FFT运算最小健相点位置
	int maxKZ;							//整周期FFT运算最大健相点位置

	int  freqRange;						//频谱图范围
	int windowStyle;					//窗体类型
	int samplingFreq;					//采样频率
	int waveWidthMin;					//波形图X最小值
	int waveWidthMax;					//波形图X最大值
	int waveHeightMin;					//波形图Y最小值
	int waveHeightMax;					//波形图Y最大值
	CString FileStr;					//文件路径
	CString ExamNameStr;				//试验名
	CString TypeStr;					//试验类型
	CString ParamStr;					//试验参数
	CString chnStr;						//通道名
	int waveCout;						//波形曲线个数
	COLORREF col;						//曲线颜色
	BOOL menuShow;						//是否为外部数据FFT
	std::vector<FFTParam> fftVector;	//频谱图特征描述
	std::vector<FFTParam> fftVectorDesc;//按幅值由大到小排序，前8位频谱特征值
	CWaveParam waveParam;				//波形图特征
	BOOL isFreqRangeChg;				//整周期运算，频谱图范围是否改变
	int FreqXmin;						//频谱图X最小值
	int FreqXmax;						//频谱图X最大值
	int wordVision;						//word类型，0：word2003		1：word2007
	BOOL isHasKeyPhase;					//是否存在正确的健相信息

	BOOL isThrowload;
	double m_throw_start;
	double m_throw_end;

	CNiReal64Vector m_tempTrendResult;

	//std::vector<int> vSamplingFreq;

public:
	DECLARE_EVENTSINK_MAP()
public:
	afx_msg void OnZoom();
public:
	afx_msg void OnMoving();
public:
	afx_msg void OnUnzoom();
public:
	afx_msg void Onresume();
public:
	afx_msg void OnSave();
public:
	afx_msg void OnCustomsave();
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
//public:
//	NI::CNiNumEdit m_Ex;
//public:
//	NI::CNiNumEdit m_Ey;
//public:
	//void MouseUpCwgraph2(short Button, short Shift, long x, long y);
public:
	afx_msg void OnXzoom();
public:
	afx_msg void OnCurrornearestpoint();
public:
	afx_msg void OnCurrorfloat();
protected:
//	virtual void OnDraw(CDC* /*pDC*/);

//public:
//	NI::CNiNumEdit m_Exx;
//public:
//	NI::CNiNumEdit m_Eyy;
public:
	afx_msg void OnAnnotations();
//public:
//	HBITMAP CopyScreenToBitmap(LPRECT lpRect);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	void ZoomCwgraph2();
public:
	//void WriteToExcel(CString FilePathName,int q);
	//void SetExcelCHNMatrix(int x,int y);
	//void SaveToExcel(CString FilePathName,CString chn,int cout);
	void SaveToTxt(CString FilePathName);
	double GetXRang();
	double GetYRang();
	void GetQuaternion();
	void SetQuaternion();
	void GetQuaternionF();
	void SetQuaternionF();
	void InitNiGraph();
	void SaveAllView();
	void SaveAllView(CString Path,int type);
	void CustomSaveAllView();
	void SaveSigleView();
	void Annotations();
	void TextData();
	void SetTitle(CString str);
	void SetData(std::vector<float> vData,int sFreq, int min, int max);
	void AddNewWave(int save_freq,std::vector<float> vData,COLORREF color);
	void GraphShowType(int type, int cx, int cy);
	void SetFraqData(CNiReal64Vector niData, double sample);
	void SetWaveRange(int min,int max);
	void SetChnDataStr(CString file,CString exmName, CString type, CString param,CString chn);
	void FFTProc();
	void LoadDataFFT(std::vector<double> vData,CString cap,COLORREF colref,int freq);
	void AddTrendWave(int TrendType,int winW,int winS,COLORREF color);
	void SetWaveParam();
	void SetFreqRange(int xmin,int xmax);
	void SetKeyPhase(std::vector<float> kp,int key_freq);
	void WriteToWord(CNiWordDocument document, int wordVision);
	CNiReal64Vector re_time (CNiReal64Vector p1);
	//重新绘制全图
	void Resume();
	void GetWordVision();
//	void PanCwgraph2();
public:
//	void PanCwgraph2();
public:
	afx_msg void OnSavesingleview();
public:
	afx_msg void OnSettitle();
public:
	void CursorChangeCwgraph2(long* CursorIndex, VARIANT* XPos, VARIANT* YPos, BOOL* bTracking);
public:
	CStatic m_wavePic;
public:
	CStatic m_fraqPic;
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
//	void MouseMoveCwgraph2(short Button, short Shift, long x, long y);
public:
//	afx_msg void OnWordreport();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnCurrorvisual();
public:
//	void MouseMoveCwgraph1(short Button, short Shift, long x, long y);
public:
	afx_msg void OnXzoomf();
public:
	afx_msg void OnZoomf();
public:
	afx_msg void OnXmovingf();
public:
	void ClickCwgraph1();
public:
	void ClickCwgraph2();
public:
	afx_msg void OnCurrornearestpointf();
public:
	void CursorChangeCwgraph1(long* CursorIndex, VARIANT* XPos, VARIANT* YPos, BOOL* bTracking);
public:
//	void PlotMouseMoveCwgraph2(short* Button, short* Shift, VARIANT* xData, VARIANT* YData, short* PlotIndex, long* PointIndex);
public:
//	void PlotAreaMouseMoveCwgraph2(short* Button, short* Shift, VARIANT* XPos, VARIANT* YPos);
public:
//	void PlotAreaBoundsChangeCwgraph2(long* Left, long* top, long* Width, long* Height);
public:
//	void MouseMoveCwgraph2(short Button, short Shift, long x, long y);
public:
	afx_msg void OnYzoom();
public:
	afx_msg void OnWavegraph();
public:
	afx_msg void OnWavefraqgrap();
public:
	afx_msg void OnFraqgraph();
public:
	afx_msg void OnUnzoomf();
public:
	void ZoomCwgraph1();
public:
//	void PlotAreaBoundsChangeCwgraph1(long* Left, long* top, long* Width, long* Height);
public:
	afx_msg void Onresumef();
public:
	afx_msg void OnAddchndata();
public:
//	afx_msg void OnClose();
public:
//	virtual BOOL DestroyWindow();
public:
	afx_msg void OnAddmobtrend();
public:
	afx_msg void OnWaveFilter();
public:
	afx_msg void OnResumeFilter();
	
public:
	afx_msg void OnExcelreport();
public:
	afx_msg void OnWordreport();
public:
	afx_msg void OnSavetxtdata();
public:
	afx_msg void OnSavepicbat();
public:
	afx_msg void OnSaveexceldata();
public:
	afx_msg void OnTextsave();
public:
	afx_msg void OnKeyphasevisual();
public:
	afx_msg void OnUpdateKeyphasevisual(CCmdUI *pCmdUI);
public:
	afx_msg void OnSaveworddata();
public:
	afx_msg void OnFullperiodfft();
public:
	afx_msg void OnUpdateFullperiodfft(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateWavegraph(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateFraqgraph(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateWavefraqgrap(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateAdjustAnnotationPlace(CCmdUI *pCmdUI);
public:
	CStatic m_ctrlTitle;

	/*--------------------------Marshall's Code2011.2.22 16:52--------------*/
	//int  _cdecl  Compare(const void* a,const void* b);
	static	double GetConf(const CNiReal64Vector& niWaveData, double dPercent);
	//波形图标注//Marshall  2011.2.24 21:12
	afx_msg void OnAnnotation();
	//整周期//Marshall  2011.2.25 16:41
	afx_msg LPARAM OnFullPR(WPARAM,LPARAM);
	void FullPR();
	/*--------------------------Marshall's Code2011.2.22 16:52--------------*/
public:
	CString m_strWaveTitle;
public:
	afx_msg void OnCurrTextdata();
public:
	afx_msg void OnFreqTextdata();
public:
	afx_msg void OnTimeSpan();
public:
	afx_msg void OnCancelAnnotations();
public:
	afx_msg void OnAdjustAnnotationPlace();
public:
	afx_msg void OnWindowSet();
};
