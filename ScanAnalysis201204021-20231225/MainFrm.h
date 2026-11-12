// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "WorkSpaceBar.h"
#include "NiWaveFraqView.h"
#include "OutputView.h"
#include "FilePorperty.h"
//#include "WatchBar.h"
#include<vector>
#include <algorithm>
#include "FileData.h"
#include "AxisOrbit.h"
#include "Ni3DWaveView.h"
#include "TrendView.h"
#include "DynamicEquilibrium.h"
#include "ChildFrm.h"
#include "CamView.h"
#include "Correlation.h"
class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CBCGPStatusBar			m_wndStatusBar;
	CBCGPMenuBar			m_wndMenuBar;
	//CBCGPToolBar			m_wndToolBar;
	CTrendView* m_TrendView;
	CNiWaveFraqView* m_PWave; 

	CNi3DWaveView* m_P3DFFT;
	CCamView* m_Cam;

	CAxisOrbit* m_Axis; 
	CCorrelation* corr; 
	CDynamicEquilibrium* m_DynEqu;
	
	std::vector<CChildFrame*> childFV;
	CWorkSpaceBar			m_wndWorkSpace;
	COutputViewBar				m_wndOutput;
	FileData* fileData;
	CString					FilePathName;
	bool m_bFlag;

	/*std::vector<CChildFrame*> frame;*/
	//CWatchBar				m_wndWatchBar;	
	std::vector<CString> FilePathVector;
	//CBCGPToolBarImages	m_UserImages;

// Generated message map functions
public:
	afx_msg void OnCloseAll();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnViewCustomize();
	//afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	//void OnToolsViewUserToolbar (UINT id);
	//void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg void OnWindowManager();
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnClose();


	/*--------------------------Marshall's Code2011.2.21 18:11--------------*/
	afx_msg void OnNinetySeven();
	afx_msg void OnNinetyFive();
	afx_msg void OnOneHundred();

	afx_msg void OnUpdateNinetySeven(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNinetyFive(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOneHundred(CCmdUI* pCmdUI);
	/*--------------------------Marshall's Code2011.2.21 18:11--------------*/
	//void GetFilePorperty();
	void SetFilePorperty(int k, CString FilePathName);	
	DECLARE_MESSAGE_MAP()
	virtual CBCGPMDIChildWnd* CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/);

public:
	CBCGPToolBar		m_wndToolBarStandard;
	CBCGPToolBar		m_wndWaveToolBar;
	CBCGPToolBar		m_wndFraqToolBar;
	CBCGPToolBar		m_wndChannelsToolBar;
	CBCGPToolBar		m_wndSCHNToolBar;
	CBCGPToolBar		m_wndStripToolBar;
	CBCGPToolBar		m_wndT3dToolBar;
	CBCGPToolBar		m_wndAxisToolBar;
	CBCGPToolBar		m_wndTrendToolBar;
	CBCGPToolBar		m_wndDynEqToolBar;
	CBCGPToolBar		m_wndJiggerToolBar;
	CBCGPToolBar		m_wndEffToolBar;
	CBCGPToolBar		m_wndCoherentToolBar;

	CBCGPTabbedToolbar	m_wndChannelsBATBarFile;
	CBCGPTabbedToolbar	m_wndTabbedToolBarGrap;
	CBCGPTabbedToolbar	m_wndTabbedToolBarChanl;
	//CBCGPTabbedToolbar	m_wndTabbedToolBarStrip;
	//CBCGPTabbedToolbar	m_wndTabbedToolBarT3d;
	//CBCGPTabbedToolbar	m_wndTabbedToolBarAxis;
	CBCGPTabbedToolbar	m_wndTabbedToolBarExamGrap;
	CBCGPTabbedToolbar	m_wndTabbedToolBarExamAnsy;
	//CBCGPTabbedToolbar	m_wndTabbedToolBarDynEq;
	//CBCGPTabbedToolbar	m_wndTabbedToolBarJigger;
	//CBCGPTabbedToolbar	m_wndTabbedToolBarEff;
	UINT	m_nAppLook;
	UINT	m_nTabLook;
public:
	void ActivateTabG(int cout);
	void ActivateTabEG(int cout);
	void ActivateTabEA(int cout);

	//void ActivateTabC(int cout);
	void OnChangeLook ( BOOL bDockTabColors );
public:
	afx_msg void OnTabcolour(UINT id);
public:
	afx_msg void OnUpdateTabcolour(CCmdUI *pCmdUI);
public:
	afx_msg void OnFileopendata();
public:
//	afx_msg void OnAllgraphvisual();
public:
	afx_msg void OnFilepproperty();
//public:
//	afx_msg void OnUpdateFilepproperty(CCmdUI *pCmdUI);
public:
//	afx_msg void OnSavepicbat();
public:
//	afx_msg void OnSavetxtdata();
public:
//	afx_msg void OnSaveexceldata();
public:
//	afx_msg void OnT3dwave();
public:
	afx_msg void OnAxisnew();
public:
	afx_msg void OnCoherentays();
public:
	afx_msg void OnLoddataFft();
public:
	afx_msg void OnT3dfreqnew();

	/*--------------------------Marshall's Code2011.2.24 18:10--------------*/
	afx_msg void OnT3dWaveNew();
	/*--------------------------Marshall's Code2011.2.24 18:10--------------*/

public:
//	afx_msg void OnT3dsaveimage();
public:
//	afx_msg void OnSavet3dviewbatdlg();
public:
	afx_msg void OnTrendGraph();
public:
//	afx_msg void OnTrendimagesavebat();
public:
//	afx_msg void OnTrendreport();
public:
	afx_msg void OnStripchartgen();
public:
	afx_msg void OnDynamicequilibrium();
public:
	afx_msg void OnJiggerexam();
public:
	afx_msg void OnEfficiency();
public:
	afx_msg void OnCAM();
public:
	afx_msg void OnFileTB();
public:
	afx_msg void OnUpdateFileTB(CCmdUI *pCmdUI);
public:
	afx_msg void OnWavetb();
public:
	afx_msg void OnUpdateWavetb(CCmdUI *pCmdUI);
public:
	afx_msg void OnChannelstb();
public:
	afx_msg void OnUpdateChannelstb(CCmdUI *pCmdUI);
	
public:
	afx_msg void OnExamGraptb();
public:
	afx_msg void OnUpdateExamGraptb(CCmdUI *pCmdUI);
//public:
//	afx_msg void OnStripttb();
//public:
//	afx_msg void OnUpdateStripttb(CCmdUI *pCmdUI);
//public:
//	afx_msg void OnT3dtb();
//public:
//	afx_msg void OnUpdateT3dtb(CCmdUI *pCmdUI);
//public:
//	afx_msg void OnAxistb();
//public:
//	afx_msg void OnUpdateAxistb(CCmdUI *pCmdUI);
public:
	afx_msg void OnExamanystb();
public:
	afx_msg void OnUpdateExamanystb(CCmdUI *pCmdUI);
public:
	afx_msg void OnExamdata();
public:
	afx_msg void OnUpdateExamdata(CCmdUI *pCmdUI);
public:
	afx_msg void OnMsgout();
public:
	afx_msg void OnUpdateMsgout(CCmdUI *pCmdUI);
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
//	afx_msg void OnCloseallview();
public:
	afx_msg void OnAirgap();
//public:
//	afx_msg void OnSaveworddata();
public:
		afx_msg void OnSaverecordtotext();
public:
//	afx_msg void OnFullperiodfft();
public:
//	afx_msg void OnUpdateFullperiodfft(CCmdUI *pCmdUI);
public:
	afx_msg void OnUserhelp();
public:
	afx_msg void OnRefreshApp();
	afx_msg void OnDynEquCmp();
public:
	afx_msg void OnUpdateEfficiency(CCmdUI *pCmdUI);
	afx_msg void OnCamSaveRequiresView();
};


