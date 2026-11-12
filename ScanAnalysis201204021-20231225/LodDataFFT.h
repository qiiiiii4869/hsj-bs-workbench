#pragma once
#include <vector>
//加载外部数据完成FFT运算

// CLodDataFFT 对话框

class CLodDataFFT : public CHSJDialog
{
	DECLARE_DYNAMIC(CLodDataFFT)

public:
	CLodDataFFT(CWnd* pParent = NULL);		// 标准构造函数
	virtual ~CLodDataFFT();

// 对话框数据
	enum { IDD = IDD_LOGDATAFFT };
	COLORREF color;							//曲线颜色
	CString m_strRGB;						//颜色RGB值
	//BOOL isReadFileOK;					//是否读取到文件
	CBCGPColorButton m_ColorPicker;			//选取颜色按钮
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedColorPicker();	//消息：选择波形曲线颜色
public:
	virtual BOOL OnInitDialog();			//消息：初始化
public:
	CString m_FilePathName;					//文件路径名
	CString m_Title;						//标题
	CString m_Freq;							//频率
	std::vector<double> fileData;			//通道数据
public:
	afx_msg void OnBnClickedOk();			//消息：加载数据完成FFT运算
public:
	afx_msg void OnBnClickedButton1();		//消息：选取通道数据文件
};
