#pragma once
#include <vector>
#include "afxwin.h"

// CJiggerExamSet 对话框

class CJiggerExamSet : public CHSJDialog
{
	DECLARE_DYNAMIC(CJiggerExamSet)

public:
	CJiggerExamSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJiggerExamSet();
public:
	CComboBox fileNameComb;
public:
	CComboBox examTypeComb;
// 对话框数据
	enum { IDD = IDD_JiggerExamSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	void AddchnMean();
	CComboBox oExamParam;
public:
	CComboBox nExamParam;
public:
	CComboBox oCHN1;
	CComboBox oCHN2;
	CComboBox oCHN3;
	CComboBox oCHN4;
	CComboBox oCHN5;
	CComboBox oCHN6;
	CComboBox oCHN7;
	CComboBox oCHN8;
public:
	CComboBox nCHN1;
	CComboBox nCHN2;
	CComboBox nCHN3;
	CComboBox nCHN4;
	CComboBox nCHN5;
	CComboBox nCHN6;
	CComboBox nCHN7;
	CComboBox nCHN8;

	//std::map<int,double> oChnData;
	//std::map<int,double> nChnData;

	std::vector<CString> oChnVector;
	std::vector<CString> nChnVector;

	CString savePath;

	void AddchnMean(CString examParam,CString chnName,int k, int t);
	bool SaveDataToText(CString FilePathName,double * oChnPtr,double * nChnPtr,int len);

public:
	afx_msg void OnCbnSelchangeFilenamecmbo();
public:
	afx_msg void OnCbnSelchangeExamtypecombo();
public:
	afx_msg void OnCbnSelchangeOexamparamcombo();
public:
	afx_msg void OnCbnSelchangeOchn1();
public:
	afx_msg void OnCbnSelchangeOchn2();
public:
	afx_msg void OnCbnSelchangeOchn3();
public:
	afx_msg void OnCbnSelchangeOchn4();
public:
	afx_msg void OnCbnSelchangeOchn5();
public:
	afx_msg void OnCbnSelchangeOchn6();
public:
	afx_msg void OnCbnSelchangeOchn7();
public:
	afx_msg void OnCbnSelchangeOchn8();
public:
	afx_msg void OnCbnSelchangeNexamparamcombo();
public:
	afx_msg void OnCbnSelchangeNchn1();
public:
	afx_msg void OnCbnSelchangeNchn2();
public:
	afx_msg void OnCbnSelchangeNchn3();
public:
	afx_msg void OnCbnSelchangeNchn4();
public:
	afx_msg void OnCbnSelchangeNchn5();
public:
	afx_msg void OnCbnSelchangeNchn6();
public:
	afx_msg void OnCbnSelchangeNchn7();
public:
	afx_msg void OnCbnSelchangeNchn8();
public:
	afx_msg void OnBnClickedButton19();
public:
	afx_msg void OnBnClickedOk();
};

