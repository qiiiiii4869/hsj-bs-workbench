#pragma once
#include "catlistbox.h"
#include <vector>
#include "afxwin.h"
#include "afxcmn.h"
#define ID_TIMER_DOIT			1
#define ID_TIMER_CHECK_CANCEL	2

// SaveImageBatDlg 对话框

class SaveImageBatDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(SaveImageBatDlg)

public:
	SaveImageBatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SaveImageBatDlg();

// 对话框数据
	enum { IDD = IDD_SAVEIMAGEBAT };

	//BOOL    isSelFile;
	int m_k;
	
	int m_index,m_temp;
	BOOL	m_bShowHeader;
	BOOL m_con;
	int m_fileType;
	CButton m_Radio1;
	CButton m_Radio2;
	CButton m_Radio3;
	CButton m_Radio4;
	CButton m_Radio5;
	CEdit m_savePath;
	BOOL m_Bconfig;
	CEdit m_configPath;
	CString filePath;
	double minTem[100],maxTem[100];
	std::map<CString,CString> xWaveMin;
	std::map<CString,CString> xWaveMax;
	std::map<CString,CString> xWaveMin2;
	std::map<CString,CString> xWaveMax2;


	std::map<double,double> timeSpan;



	CString examName;
	CString examType;
	CString sortFilePath;
	CString savePath;
	CString configPath;
	std::vector<CString> examParam;
	std::map<CString,std::map<CString,BOOL>> cHNnameBool;
public:
	CComboBox fileNameComb;
public:
	CComboBox examTypeComb;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//CBCGPToolBarImages	m_Header;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	std::map<CString,std::map<CString,BOOL>> SetCHNnameBool(CString examParam,CString chnName,BOOL isSel);
	vector<CString> SetExamParam(CString paramName);
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
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnOk();
public:
	CCatListBox m_chnList;
	void FillData();
//public:
//	afx_msg void OnBnClickedButton2();


public:
	CStatic m_msg;
public:
	CStatic m_examType;
public:
	CEdit m_EditMin2;
public:
	CEdit m_EditMax2;
	CButton m_graphChk;
	BOOL isGraph;
//public:
//	CEdit m_EditMin;
//public:
//	CEdit m_EditMax;
public:
	afx_msg void OnLbnSelchangeList2();
public:
	afx_msg void OnBnClickedChgrng();
//public:
//	afx_msg void OnBnClickedButton2();
//public:
//	afx_msg void OnBnClickedCancel();
//public:
//	afx_msg void OnCbnSelchangeFilenamecmbo();
//public:
//	afx_msg void OnCbnSelchangeExamtypecombo();
public:
	CProgressCtrl m_progress;
public:
	CButton m_ok;
public:
	CButton m_cancel;
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedChgrng2();
public:
	afx_msg void OnBnClickedCheck1();
};
