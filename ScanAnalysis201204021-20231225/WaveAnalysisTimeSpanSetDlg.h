#pragma once


// CWaveAnalysisTimeSpanSetDlg 对话框

class CWaveAnalysisTimeSpanSetDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(CWaveAnalysisTimeSpanSetDlg)

public:
	CWaveAnalysisTimeSpanSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaveAnalysisTimeSpanSetDlg();

// 对话框数据
	enum { IDD = IDD_WAVE_SPAN_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
public:

public:
	double m_begin;
public:
	double m_end;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
};
