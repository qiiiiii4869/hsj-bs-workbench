#pragma once
#include "afxwin.h"

#define JIGGERCOUNT 8

// CJiggerView 窗体视图

class CJiggerView : public CFormView
{
	DECLARE_DYNCREATE(CJiggerView)
	CHSJFontStore m_skinFonts; // Child typefaces only; original font sizes and bounds.

	CJiggerView();           // 动态创建所使用的受保护的构造函数
protected:

	virtual ~CJiggerView();
	NI::CNiGraph m_Graph;				//配重前波形频谱
	NI::CNiGraph m_NewGraph;			//试配重波形频谱


public:
	enum { IDD = IDD_JiggerView };
public:
	//CString examName;					//试验名
	//CString examType;					//实验类型
	double m_XAxies;					//X偏心坐标
	double m_YAxies;					//Y偏心坐标
	double m_Distance;					//偏心距
	double m_Angle;						//偏心角
	CNiReal64Vector oChnDataMean;		//配重前数据
	CNiReal64Vector nChnDataMean;		//试配重数据

	void DrawGraph();
	void setChnData(CNiReal64Vector o,CNiReal64Vector n);
	BOOL SaveDataToText(CString FilePathName,CNiReal64Vector oChnPtr,CNiReal64Vector nChnPtr,int len);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnLoadjiggerdata();
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnLoadtxtjigger();
public:
	afx_msg void OnSavejiggertotxt();
public:
	afx_msg void OnSavejiggertoimage();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
protected:
	virtual void OnDraw(CDC* /*pDC*/);//调整界面//Marshall  2011.3.3 13:6
};


