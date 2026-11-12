#pragma once
#include "afxwin.h"
#include <vector>

// CAddChnData 对话框

class CAddChnData : public CHSJDialog
{
	DECLARE_DYNAMIC(CAddChnData)

public:
	CAddChnData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddChnData();

// 对话框数据
	enum { IDD = IDD_AddChnData };
	CString FileStr;
	CString ExamNameStr;
	CString TypeStr;
	CString ParamStr;
	CString chnStr;
	COLORREF color;
	/*std::vector<CString> CString chnVector;*/
	void SetChnData(CString file,CString exmName, CString type, CString param,CString chn);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBCGPColorButton m_ColorPicker;
public:
	afx_msg void OnBnClickedColorPicker();
public:
	CString m_strRGB;
public:
	virtual BOOL OnInitDialog();
public:
	CString m_ParamStr;
public:
	CComboBox chnNameComb;

	//-------------------1.0.0.2------------------------//
	CString selChnStr;		//所选通道名称
	//--------------------------------------------------//

public:
	afx_msg void OnBnClickedOk();
};
