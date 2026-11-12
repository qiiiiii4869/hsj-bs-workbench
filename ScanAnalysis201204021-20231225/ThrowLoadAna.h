#pragma once


// CThrowLoadAna 对话框

class CThrowLoadAna : public CHSJDialog
{
	DECLARE_DYNAMIC(CThrowLoadAna)

public:
	CThrowLoadAna(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThrowLoadAna();

// 对话框数据
	enum { IDD = IDD_SET_THROWLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_baseTime;
	CButton m_graphChk;
	BOOL isGraph;
public:
	double m_maxTime;
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCheck1();
public:
	afx_msg void OnBnClickedCancel();
};
