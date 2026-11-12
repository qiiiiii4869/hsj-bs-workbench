#pragma once
#include <vector>

// CDynamicEquilibriumSetDLG 对话框

class CDynamicEquilibriumSetDLG : public CHSJDialog
{
	DECLARE_DYNAMIC(CDynamicEquilibriumSetDLG)

public:
	CDynamicEquilibriumSetDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDynamicEquilibriumSetDLG();

// 对话框数据
	enum { IDD = IDD_DynamicEquilibriumSetDLG };
	int DataType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox fileNameComb;
public:
	CComboBox examTypeComb;
public:
	CComboBox examParamComb;
public:
	CComboBox chnNameCombChn;
	CString title;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnCbnSelchangeFilenamecmbo();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeExamtypecombo();
public:
	afx_msg void OnCbnSelchangeExamparamcombo();
};
