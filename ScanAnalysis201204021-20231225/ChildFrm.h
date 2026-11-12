// ChildFrm.h : interface of the CChildFrame class
//


#pragma once


class CChildFrame : public CBCGPMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

	// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

	// Operations
public:
	CString titles;
	//BOOL isColse;
	// Overrides
public:
	//virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	virtual void ActivateFrame(int nCmdShow = -1);
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
		virtual void PreSubclassWindow();
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
public:
	afx_msg void OnClose();

};
