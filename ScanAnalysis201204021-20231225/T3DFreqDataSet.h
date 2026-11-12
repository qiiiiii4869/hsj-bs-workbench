#pragma once
#include "afxwin.h"
#include <vector>

// CT3DFreqDataSet 对话框

class CT3DFreqDataSet : public CHSJDialog
{
	DECLARE_DYNAMIC(CT3DFreqDataSet)

public:
	CT3DFreqDataSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CT3DFreqDataSet();

// 对话框数据
	enum { IDD = IDD_T3DFreqDLG };

	int samplFreq;
	std::map<CString,std::vector<float>> MultiChndata;//通道数据//Marshall  2011.2.27 10:55
	std::map<CString,std::vector<float>> MultiChndataAcd;//参考通道数据//Marshall  2011.2.27 10:55
	//int chnCout;
	void GetMultiChnData();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	CComboBox fileNameComb;
public:
	CComboBox examTypeComb;
public:
	CComboBox examParamComb;
public:
	CComboBox chnNameComb;
public:
	CComboBox chnNameCombAcd;
public:
	afx_msg void OnCbnSelchangeFilenamecmbo();
public:
	afx_msg void OnCbnSelchangeExamtypecombo();
public:
	afx_msg void OnCbnSelchangeExamparamcombo();
public:
	afx_msg void OnCbnSelchangeExamchncombox();
public:
	NI::CNiGraph m_Graph;
public:
	DECLARE_EVENTSINK_MAP()

public:
	void DblClickCwgraph2();
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
	bool m_bWaveNew;//加一个新建三维波形瀑布图的标识,默认不创建//Marshall  2011.2.24 18:11
	void SetKeyPhase(std::vector<float> kp,int key_freq, int wave_freq,int nDataLen);
	BOOL isHasKeyPhase;					//是否存在正确的健相信息
	std::vector<float> kPhaseX;			//健相X坐标集合
};
#pragma once

