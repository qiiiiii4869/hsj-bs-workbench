#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// FilePorperty 对话框

class FilePorperty : public CHSJDialog
{
	DECLARE_DYNAMIC(FilePorperty)

public:
	FilePorperty(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FilePorperty();

// 对话框数据
	enum { IDD = IDD_Fileproperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CStatic path;
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox fileNameComb;
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
	CStatic examName;
public:
	CStatic cTtime;
public:
	CStatic mTime;
public:
	CStatic aTime;
public:
	CBCGPListCtrl m_wndList;

public:
	CStatic sFreq;
public:
	CStatic sNum;
public:
	CStatic sLen;
public:
	CStatic cLen;
};
