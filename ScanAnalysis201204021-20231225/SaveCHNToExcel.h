#pragma once
#include "afxcmn.h"
#include <vector>
#include "CWaveFreqCom.h"
#include "afxwin.h"
#include "ReportCtrl.h"
// CSaveCHNToExcel 对话框

class CSaveCHNToExcel : public CHSJDialog
{
	DECLARE_DYNAMIC(CSaveCHNToExcel)

public:
	
	CSaveCHNToExcel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveCHNToExcel();


	BOOL isGraph;


	int m_samp;
	int m_count;
	int m_saveSamp;


	double minTem[100],maxTem[100];
	double m_start,m_end;
	int m_xMin,m_xMax;
	BOOL m_Bconfig;
	CString configPath;
	CString filePath;
	CString savePath;
	CString examType;
	CString examName;
	CEdit m_configPath;
	std::vector<CString> paramVector;
	CNiReal64Vector NiChData;
	CNiReal64Matrix excelCHNMatrix;
	std::vector<FFTParam> fftVector;	
	std::vector<FFTParam> fftVectorDesc;
	CNiExcelApplication m_application;
	CNiExcelWorkbook m_workBook;

	//CNiExcelWorksheet m_workSheet;

// 对话框数据
	enum { IDD = IDD_SaveCHNToExcel };

	//void SetParamVector(std::vector<CString> s);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void WriteToExcel(CString FilePathName,int q);
	void SetHead();
	void SetChData(std::vector <float> vData,int sFreq);
	void CompChnData(CNiReal64Vector NiChData,int cout);
	void SetFraqData(CNiReal64Vector niData, double sample,BOOL isfft);
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButton1();
public:
	CStatic m_msg;
public:
	CEdit m_savePath;
public:
	CStatic m_examType;
public:
	CReportCtrl m_List;

public:
	afx_msg void OnBnClickedCancel();
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CButton m_OK;
public:
	CButton m_CANCEL;
	CButton m_graphChk;
public:
	CProgressCtrl m_progress;

	/*--------------------------Marshall's Code2011.2.25 19:19--------------*/
	void SetKeyPhase(std::vector<float> kp,int key_freq, int wave_freq,int nDataLen);
	BOOL isHasKeyPhase;					//是否存在正确的健相信息
	std::vector<float> kPhaseX;			//健相X坐标集合
//	std::vector<float> kPhaseY;			//健相Y坐标集合//在此不需要//Marshall  2011.2.25 19:19
	CNiReal64Vector re_time (CNiReal64Vector p1,double samplingFreq);//等周期作此处理，//当前版本，默认不重采样//Marshall  2011.2.27 10:41//Marshall  2011.2.26 10:2
	/*--------------------------Marshall's Code2011.2.25 19:19--------------*/
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedChgrng2();
public:
	afx_msg void OnBnClickedCheck1();
};
