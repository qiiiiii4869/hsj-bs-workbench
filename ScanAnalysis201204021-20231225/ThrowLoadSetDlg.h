#pragma once


// CThrowLoadSetDlg 对话框

class CThrowLoadSetDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(CThrowLoadSetDlg)

public:
	CThrowLoadSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThrowLoadSetDlg();

// 对话框数据
	enum { IDD = IDD_THROW_LOAD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_width;
public:
	int m_increase;
	
public:
	afx_msg void OnBnClickedOk();
};
