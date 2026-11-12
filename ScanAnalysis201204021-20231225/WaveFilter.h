#pragma once
#include "afxwin.h"


// CWaveFilter 对话框

class CWaveFilter : public CHSJDialog
{
	DECLARE_DYNAMIC(CWaveFilter)

public:
	CWaveFilter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaveFilter();

// 对话框数据
	enum { IDD = IDD_WaveFilterDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox m_filters;
public:
	double m_FreqLowCut;
public:
	double m_FreqHighCut;
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
	int m_FilterType;
public:
	afx_msg void OnBnClickedOk();
public:
	int m_Freq;
public:
	CEdit m_lowCut;
public:
	CEdit m_highCut;
public:
	int m_order;
};
