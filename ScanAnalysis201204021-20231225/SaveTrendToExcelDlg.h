#pragma once
#include "CWaveFreqCom.h"
#include <vector>
#include "afxwin.h"

// CSaveTrendToExcelDlg 对话框

class CSaveTrendToExcelDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(CSaveTrendToExcelDlg)

public:
	CSaveTrendToExcelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveTrendToExcelDlg();


//////////////////////////////////////////////////////////////////////////

	int m_samp;
	int m_count;
	int m_saveSamp;


	double minTem[100],maxTem[100];
	double m_start,m_end;
	int m_xMin,m_xMax;
	BOOL m_Bconfig;
	CString configPath;
	CEdit m_configPath;
//////////////////////////////////////////////////////////////////////////

	


	CButton m_graphChk1;
	BOOL isGraph1;

	CString m_strUnit;
	CString m_strAcc;

	CString filePath;
	CString savePath;
	CString examType;
	BOOL isGraph;	
	std::vector<double> ParamXmean;
	CNiReal64Vector NiChData;
	CNiReal64Matrix excelCHNMatrix;
	std::vector<FFTParam> fftVector;	
	std::vector<FFTParam> fftVectorDesc;
	CNiExcelApplication m_application;
	CNiExcelWorkbook m_workBook;
	//void SetParamVector(std::vector<CString> s);
	void SetHead();
	void SetChData(std::vector <float> vData,int sFreq);
	void SetFraqData(CNiReal64Vector niData, double sample);
	void CompChnData(CNiReal64Vector NiChData,int cout);


	/*----------------------------------------*/
	void SetKeyPhase(std::vector<float> kp,int key_freq, int wave_freq,int nDataLen);
	BOOL isHasKeyPhase;					//是否存在正确的健相信息
	std::vector<float> kPhaseX;			//健相X坐标集合
	//	std::vector<float> kPhaseY;			//健相Y坐标集合//在此不需要//Marshall  2011.2.25 19:19
	CNiReal64Vector re_time (CNiReal64Vector p1,double samplingFreq);//等周期作此处理，//当前版本，默认不重采样//Marshall  2011.2.27 10:41//Marshall  2011.2.26 10:2
	/*----------------------------------------*/



// 对话框数据
	enum { IDD = IDD_SaveTrendToExcel };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	virtual BOOL OnInitDialog();

public:
	CComboBox fileNameComb;
public:
	CComboBox examTypeComb;
public:
	CComboBox examParamComb;
public:
	CComboBox chnNameCombAcd;
public:
	CStatic m_msg;
public:
	CEdit m_savePath;
public:
	//	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CButton m_OK;
public:
	CButton m_CANCEL;
public:
	CProgressCtrl m_progress;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnCbnSelchangeFilenamecmbo();
public:
	afx_msg void OnCbnSelchangeExamtypecombo();
public:
	afx_msg void OnCbnSelchangeExamparamcombo();
public:
	afx_msg void OnCbnSelchangeExamchncomboy();
public:
	CButton m_graphChk;
public:
	afx_msg void OnBnClickedCheck1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedChgrng2();
public:
	afx_msg void OnBnClickedCheck2();
};
