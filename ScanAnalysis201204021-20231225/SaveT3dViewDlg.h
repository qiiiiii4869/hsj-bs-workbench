#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"

// CSaveT3dViewDlg 对话框

class CSaveT3dViewDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(CSaveT3dViewDlg)

public:
	CSaveT3dViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveT3dViewDlg();

	BOOL isCAxis;

	int m_fileType;
	CString savePath;
	CButton m_Radio1;
	CButton m_Radio2;
	CButton m_Radio3;
	CButton m_Radio4;
	CButton m_Radio5;

	CButton m_Radio6;
	CButton m_Radio7;

	CComboBox fileNameComb;
	CComboBox examTypeComb;
	CComboBox examParamComb;
	CComboBox chnNameCombAcd;
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	int zMin;
	int zMax;
	int samplFreq;
	std::map<CString,std::vector<float>> MultiChndata;
	std::map<CString,std::vector<float>> MultiChndataAcd;

	void GetMultiChnData(CString chn);
 
// 对话框数据
	enum { IDD = IDD_SaveT3DViewDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	CEdit m_savePath;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedRadio1();
public:
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnBnClickedRadio3();
public:
	afx_msg void OnBnClickedRadio4();
public:
	afx_msg void OnBnClickedRadio5();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnCbnSelchangeFilenamecmbo();
public:
	afx_msg void OnCbnSelchangeExamtypecombo();
public:
	afx_msg void OnCbnSelchangeExamparamcombo();

public:
	CStatic m_Msg;
public:
	CEdit m_xMin;
public:
	CEdit m_xMax;
public:
	CEdit m_yMin;
public:
	CEdit m_yMax;
public:
	CEdit m_zMin;
public:
	CEdit m_zMax;
public:
	CProgressCtrl m_progress;
public:
	CButton m_OK;
public:
	CButton m_CANCEL;
public:
	afx_msg void OnBnClickedRadio6();
public:
	afx_msg void OnBnClickedRadio7();

	/*--------------------------Marshall's Code2011.2.27 15:42--------------*/
	void SetKeyPhase(std::vector<float> kp,int key_freq, int wave_freq,int nDataLen);
	BOOL isHasKeyPhase;					//是否存在正确的健相信息
	std::vector<float> kPhaseX;			//健相X坐标集合
	void SaveAll3DWaveView();
	void SaveAll3DFreqView();
	/*--------------------------Marshall's Code2011.2.27 15:42--------------*/
};
