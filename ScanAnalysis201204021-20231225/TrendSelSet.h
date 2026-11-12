#pragma once
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"

// CTrendSelSet 对话框
class TrendGraph
{
public:
	int isSel;
	COLORREF colors;
};
class CTrendSelSet : public CHSJDialog
{
	DECLARE_DYNAMIC(CTrendSelSet)

public:
	CTrendSelSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTrendSelSet();

// 对话框数据
	enum { IDD = IDD_TrendSelSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
	
	CStatic m_Msg;
	CEdit m_savePath;

	int showType;
	CBCGPColorButton m_ColorPicker1;
	CBCGPColorButton m_ColorPicker2;
	CBCGPColorButton m_ColorPicker3;
	CBCGPColorButton m_ColorPicker4;

	COLORREF color1;
	COLORREF color2;
	COLORREF color3;
	COLORREF color4;

	int sel1;
	int sel2;
	int sel3;
	int sel4;
	
	CString savePath;
	int m_fileType;
	CButton m_Radio1;
	CButton m_Radio2;
	CButton m_Radio3;
	CButton m_Radio4;
	CButton m_Radio5;

	std::map<CString,std::vector<float>> MultiChndata;
	std::map<CString,std::vector<float>> MultiChndataAcd;
	std::vector<TrendGraph> graphVector;

	void SetShowType(int s);
	void GetChnData();
	void GetMultiChnData(CString chn);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedColorPicker1();
public:
	afx_msg void OnBnClickedColorPicker2();
public:
	afx_msg void OnBnClickedColorPicker3();
public:
	afx_msg void OnBnClickedColorPicker4();
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
public:
	afx_msg void OnBnClickedCheck1();
public:
	CButton m_check1;
public:
	CButton m_check2;
public:
	CButton m_check3;
public:
	CButton m_check4;
public:
	afx_msg void OnBnClickedCheck2();
public:
	afx_msg void OnBnClickedCheck3();
public:
	afx_msg void OnBnClickedCheck4();
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
	CProgressCtrl m_progress;
public:
	CButton m_OK;
public:
	CButton m_CANCEL;
public:
	CStatic m_signSatic;
public:
	CButton m_btn;
public:
	CStatic m_pathStatic;
};
