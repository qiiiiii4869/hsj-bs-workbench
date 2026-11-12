#pragma once
#include "catlistbox.h"
#include "afxwin.h"
#include "afxcmn.h"

// SaveDataDlg 对话框

class SaveDataDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(SaveDataDlg)

public:
	SaveDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SaveDataDlg();

// 对话框数据
	enum { IDD = IDD_SAVEDATADLG };
	BOOL	m_bShowHeader;
	CString filePath;
	//int SaveFileType;
	CString examName;
	CString examType;
	CString sortFilePath;
	CString savePath;
	//CString examParam;
	/*std::map<CString,BOOL> cHNnameBool;*/
	/*std::map<CString,BOOL> SetCHNnameBool(CString chnName,BOOL isSel);*/
	std::vector<CString> examParam;
	std::map<CString,std::map<CString,BOOL>> cHNnameBool;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//CBCGPToolBarImages	m_Header;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	std::map<CString,std::map<CString,BOOL>> SetCHNnameBool(CString examParam,CString chnName,BOOL isSel);
	vector<CString> SetExamParam(CString paramName);

public:
	virtual BOOL OnInitDialog();

public:
	CCatListBox m_chnList;
//public:
//	afx_msg void OnBnClickedButton2();

public:
	CStatic m_msg;
public:
	CEdit m_savePath;
public:
	CStatic m_examType;
public:
	CProgressCtrl m_progress;
public:
	CButton m_ok;
public:
	CButton m_cancel;
public:
	afx_msg void OnBnClickedCancel();
};
