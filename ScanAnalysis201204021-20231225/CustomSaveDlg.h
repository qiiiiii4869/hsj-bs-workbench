#pragma once
#include "afxwin.h"

// CCustomSaveDlg 对话框

class CCustomSaveDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(CCustomSaveDlg)

public:
	//CCustomSaveDlg(CWnd* pParent = NULL);   // 标准构造函数
	CCustomSaveDlg(CWnd* pParent = NULL,int w=0,int h=0);   // 重载构造函数
	virtual ~CCustomSaveDlg();

// 对话框数据
	enum { IDD = IDD_CusomSaveImageDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_nWidth;
public:
	CEdit m_nHeight;		//
	int mWidth;				//图像宽度
	int mHeight;			//图像高度
	int m_bmpSize;			//图像类型

protected:
	virtual void OnOK();
public:
	CButton m_Radio1;
	CButton m_Radio2;
	CButton m_Radio3;
	CButton m_Radio4;
	CButton m_Radio5;
public:
	int m_fileType;
public:
	virtual BOOL OnInitDialog();
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
	afx_msg void OnBnClickedRadio6();
public:
	afx_msg void OnBnClickedRadio7();
public:
	CEdit m_nWidthC;
public:
	CEdit m_nHeightC;
//public:
//	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedOk();
};


