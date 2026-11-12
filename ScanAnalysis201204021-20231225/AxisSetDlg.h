#pragma once
#include <vector>
#include "afxwin.h"

// CAxisSetDlg 对话框

class CAxisSetDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(CAxisSetDlg)

public:
	CAxisSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAxisSetDlg();

// 对话框数据
	enum { IDD = IDD_AXISSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	std::vector<float> fileDataX;
	//int save_freqX;
	//int save_freqY;
	std::vector<float> fileDataY;
	int FunSel;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetFunSel(int sel);
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox fileNameComb;
public:
	//CComboBox examNameComb;
public:
	CComboBox examTypeComb;
public:
	CComboBox examParamComb;
public:
	CComboBox chnNameCombX;
public:
	afx_msg void OnCbnSelchangeFilenamecmbo();
//public:
//	afx_msg void OnCbnSelchangeExamnamecombo();
public:
	afx_msg void OnCbnSelchangeExamtypecombo();
public:
	CComboBox chnNameCombY;
public:
	afx_msg void OnCbnSelchangeExamchncomboy();
public:
	afx_msg void OnCbnSelchangeExamparamcombo();
public:
	afx_msg void OnCbnSelchangeExamchncombox();
};
