#pragma once
#include "Resource.h"


// CScanAnalysisView 窗体视图

class CScanAnalysisView : public CFormView
{
	DECLARE_DYNCREATE(CScanAnalysisView)

protected:
	CScanAnalysisView();           // 动态创建所使用的受保护的构造函数
	virtual ~CScanAnalysisView();

public:
	enum { IDD = IDD_GRAPHVIEW };
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
	//	afx_msg void OnFileOpen();
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
//	afx_msg void OnPoint();
public:
//	afx_msg void OnUnzoomf();
public:
//	afx_msg void OnClose();
};


