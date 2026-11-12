#pragma once
#include "afxcmn.h"
#include <vector>
#include "CWaveFreqCom.h"
#include "afxwin.h"
#include "catlistbox.h"
// CSaveCHNWord 对话框

class CSaveCHNWord : public CHSJDialog
{
	DECLARE_DYNAMIC(CSaveCHNWord)

public:
	CSaveCHNWord(CWnd* pParent = NULL);   // 标准构造函数
	
	//int saveType;			//存储形式
	CString filePath;
	CString savePath;
	CString examType;
	CString examName;
	std::vector<CString> paramVector;
	CNiReal64Vector NiChData;
	CNiReal64Matrix excelCHNMatrix;
	std::vector<FFTParam> fftVector;	
	std::vector<FFTParam> fftVectorDesc;
	void FillData();
	int wordVision;
	//CNiWordApplication application;
	std::vector<CString> examParam;
	std::map<CString,std::map<CString,BOOL>> cHNnameBool;
	virtual ~CSaveCHNWord();

// 对话框数据
	enum { IDD = IDD_SaveCHNToWord };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void GetWordVision();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	CStatic m_msg;
public:
	CEdit m_savePath;
public:
	CStatic m_examType;
//public:
//	CListCtrl m_List;
public:
	CCatListBox m_chnList;
public:
	CButton m_OK;
public:
	CButton m_CANCEL;
public:
	CProgressCtrl m_progress;
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
	std::map<CString,map<CString,BOOL>> SetCHNnameBool(CString examParam,CString chnName,BOOL isSel);
	std::vector<CString> SetExamParam(CString paramName);
//public:
//	afx_msg void OnBnClickedRadio1();
//public:
//	afx_msg void OnBnClickedRadio2();
};
