#pragma once


// CTrendGraphSet 对话框

class CTrendGraphSet : public CHSJDialog
{
	DECLARE_DYNAMIC(CTrendGraphSet)

public:
	CTrendGraphSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTrendGraphSet();

// 对话框数据
	enum { IDD = IDD_TrendGraphSetDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetTrendParam(CString t,int xmin,int xmax,int ymin,int ymax,COLORREF c1,COLORREF c2,COLORREF c3,COLORREF c4);
public:
	CBCGPColorButton m_ColorPicker1;
	CBCGPColorButton m_ColorPicker2;
	CBCGPColorButton m_ColorPicker3;
	CBCGPColorButton m_ColorPicker4;

	CString title;
	int xMin;
	int xMax;
	int yMin;
	int yMax;
	COLORREF color1;
	COLORREF color2;
	COLORREF color3;
	COLORREF color4;
public:
	afx_msg void OnBnClickedColorPicker1();
public:
	afx_msg void OnBnClickedColorPicker2();
public:
	afx_msg void OnBnClickedColorPicker3();
public:
	afx_msg void OnBnClickedColorPicker4();
public:
	virtual BOOL OnInitDialog();
};
