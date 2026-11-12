#pragma once
#include "afxwin.h"


// CAddMobTrend 对话框

class CAddMobTrend : public CHSJDialog
{
	DECLARE_DYNAMIC(CAddMobTrend)

public:
	CAddMobTrend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddMobTrend();
	COLORREF color;
// 对话框数据
	enum { IDD = IDD_AddMobTrend };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CBCGPColorButton m_ColorPicker;
public:
	afx_msg void OnBnClickedColorPicker();
public:
	CString m_strRGB;
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox m_TrendParam;
	int TrendType;		//计算依据
public:
	int m_winWidth;		//窗口宽度
public:
	int m_winStep;		//步长
public:
	afx_msg void OnCbnSelchangeCombo1();
};
