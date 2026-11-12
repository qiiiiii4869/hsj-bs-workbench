// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "ScanSel/SortData.h"
#include "ScanSel/SysConf.h"
#include "ScanSel/StrTool.h"
#include "MainFrm.h"
#include "HSJAppearance.h"

IMPLEMENT_DYNCREATE(CHSJVisualManager, CBCGPVisualManagerXP)
IMPLEMENT_DYNAMIC(CHSJDialog, CDialog)
BEGIN_MESSAGE_MAP(CHSJDialog, CDialog)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
#include "SaveImageBatDlg.h"
#include "ChildFrm.h"
#include "SaveDataDlg.h"
#include "Ni3DWaveView.h"
#include "AxisSetDlg.h"
#include "LodDataFFT.h"
#include "T3DFreqDataSet.h"
#include "SaveT3dViewDlg.h"
#include "TrendSelSet.h"
#include "SaveT3dViewDlg.h"
#include "SaveCHNToExcel.h"
#include "SaveTrendToExcelDlg.h"
#include "DynamicEquilibriumSetDLG.h"
#include "JiggerView.h"
#include "StripChart/InitialConfig.h"
#include "Efficiency.h"
#include "AirGap.h"
#include "CamView.h"

#include "DynEquCmpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*--------------------------Marshall's Code2011.2.21 18:13--------------*/
bool g_bConf97 = true;
bool g_bConf95 = false;
bool g_bConf100 = false;

//创建三维波形瀑布图////Marshall  2011.2.24 18:48
bool g_bWaveNew = false;
/*--------------------------Marshall's Code2011.2.21 18:13--------------*/

using   namespace   std;  

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)

//const int  iMaxUserToolbars		= 10;
//const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
//const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_MODERN, ID_VIEW_APPLOOK_MODERN, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_MODERN, ID_VIEW_APPLOOK_MODERN, OnUpdateAppLook)
	//ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	//ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2008, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2008, OnUpdateAppLook)
	ON_COMMAND_RANGE(ID_TABGRAY,ID_TABCOLOUR, OnTabcolour)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TABGRAY,ID_TABCOLOUR,OnUpdateTabcolour)
	ON_COMMAND(ID_FILEOPENDATA, &CMainFrame::OnFileopendata)
//	ON_COMMAND(ID_ALLGRAPHVISUAL, &CMainFrame::OnAllgraphvisual)
	ON_COMMAND(ID_FILEPproperty, &CMainFrame::OnFilepproperty)
	//ON_UPDATE_COMMAND_UI(ID_FILEPproperty, &CMainFrame::OnUpdateFilepproperty)
//	ON_COMMAND(ID_SAVEPICBAT, &CMainFrame::OnSavepicbat)
//	ON_COMMAND(ID_SAVETXTDATA, &CMainFrame::OnSavetxtdata)
//	ON_COMMAND(ID_SAVEEXCELDATA, &CMainFrame::OnSaveexceldata)
//	ON_COMMAND(ID_T3DWave, &CMainFrame::OnT3dwave)
	ON_COMMAND(ID_AXISNEW, &CMainFrame::OnAxisnew)
	ON_COMMAND(ID_CoherentAys, &CMainFrame::OnCoherentays)
	ON_COMMAND(ID_LODDATA_FFT, &CMainFrame::OnLoddataFft)
	ON_COMMAND(ID_T3DFreqNew, &CMainFrame::OnT3dfreqnew)

	/*--------------------------Marshall's Code2011.2.24 18:9--------------*/
	ON_COMMAND(ID_T3DWAVENEW, &CMainFrame::OnT3dWaveNew)
	/*--------------------------Marshall's Code2011.2.24 18:9--------------*/

//	ON_COMMAND(ID_T3dSaveImage, &CMainFrame::OnT3dsaveimage)
//	ON_COMMAND(ID_SaveT3dViewBatDlg, &CMainFrame::OnSavet3dviewbatdlg)
	ON_COMMAND(ID_TrendGraph, &CMainFrame::OnTrendGraph)
//	ON_COMMAND(ID_TrendImageSaveBat, &CMainFrame::OnTrendimagesavebat)
//	ON_COMMAND(ID_TrendReport, &CMainFrame::OnTrendreport)
	ON_COMMAND(ID_StripChartGen, &CMainFrame::OnStripchartgen)
	//ON_COMMAND(ID_DYNEQUCMP, &CMainFrame::OnDynEquCmp)
	ON_COMMAND(ID_DynamicEquilibrium, &CMainFrame::OnDynamicequilibrium)
	/*--------------------------Marshall's Code2011.3.9 13:53--------------*/
	//ON_COMMAND(ID_DynamicEquilibrium, &CMainFrame::OnDynamicequilibrium)
	ON_COMMAND(ID_DYNEQUCMP, &CMainFrame::OnDynEquCmp)
	/*--------------------------Marshall's Code2011.3.9 13:53--------------*/

	ON_COMMAND(ID_JiggerExam, &CMainFrame::OnJiggerexam)
	ON_COMMAND(ID_Efficiency, &CMainFrame::OnEfficiency)
	ON_COMMAND(ID_CAM, &CMainFrame::OnCAM)
	ON_COMMAND(ID_SaveCamData, &CMainFrame::OnCamSaveRequiresView)
	ON_COMMAND(ID_SaveCamToPic, &CMainFrame::OnCamSaveRequiresView)
	ON_COMMAND(ID_FileTB, &CMainFrame::OnFileTB)
	ON_UPDATE_COMMAND_UI(ID_FileTB, &CMainFrame::OnUpdateFileTB)
	ON_COMMAND(ID_WAVETB, &CMainFrame::OnWavetb)
	ON_UPDATE_COMMAND_UI(ID_WAVETB, &CMainFrame::OnUpdateWavetb)
	ON_COMMAND(ID_ChannelsTB, &CMainFrame::OnChannelstb)
	ON_UPDATE_COMMAND_UI(ID_ChannelsTB, &CMainFrame::OnUpdateChannelstb)
	ON_COMMAND(ID_ExamGrap, &CMainFrame::OnExamGraptb)
	ON_UPDATE_COMMAND_UI(ID_ExamGrap, &CMainFrame::OnUpdateExamGraptb)
	/*ON_COMMAND(ID_StriptTB, &CMainFrame::OnStripttb)
	ON_UPDATE_COMMAND_UI(ID_StriptTB, &CMainFrame::OnUpdateStripttb)
	ON_COMMAND(ID_T3DTB, &CMainFrame::OnT3dtb)
	ON_UPDATE_COMMAND_UI(ID_T3DTB, &CMainFrame::OnUpdateT3dtb)
	ON_COMMAND(ID_AxisTB, &CMainFrame::OnAxistb)
	ON_UPDATE_COMMAND_UI(ID_AxisTB, &CMainFrame::OnUpdateAxistb)*/
	ON_COMMAND(ID_ExamAnysTB, &CMainFrame::OnExamanystb)
	ON_UPDATE_COMMAND_UI(ID_ExamAnysTB, &CMainFrame::OnUpdateExamanystb)
	ON_COMMAND(ID_ExamData, &CMainFrame::OnExamdata)
	ON_UPDATE_COMMAND_UI(ID_ExamData, &CMainFrame::OnUpdateExamdata)
	ON_COMMAND(ID_MsgOut, &CMainFrame::OnMsgout)
	ON_UPDATE_COMMAND_UI(ID_MsgOut, &CMainFrame::OnUpdateMsgout)
	ON_WM_ACTIVATE()
//	ON_COMMAND(ID_CloseAllView, &CMainFrame::OnCloseallview)
	ON_COMMAND(ID_AirGap, &CMainFrame::OnAirgap)
	//ON_COMMAND(ID_SAVEWORDDATA, &CMainFrame::OnSaveworddata)
	ON_COMMAND(ID_SAVERECORDTOTEXT, &CMainFrame::OnSaverecordtotext)

//	ON_COMMAND(ID_FullPeriodFFT, &CMainFrame::OnFullperiodfft)
//	ON_UPDATE_COMMAND_UI(ID_FullPeriodFFT, &CMainFrame::OnUpdateFullperiodfft)
ON_COMMAND(ID_UserHelp, &CMainFrame::OnUserhelp)
ON_COMMAND(ID_REFRESHAPP, &CMainFrame::OnRefreshApp)

////Marshall  2011.2.20 21:34
ON_COMMAND(ID_CLOSEALL,&CMainFrame::OnCloseAll)
////Marshall  2011.2.21 18:7
ON_COMMAND(ID_NINESEVEN,&CMainFrame::OnNinetySeven)
ON_COMMAND(ID_NINEFIVE,&CMainFrame::OnNinetyFive)
ON_COMMAND(ID_ONEHUNDRED,&CMainFrame::OnOneHundred)

ON_UPDATE_COMMAND_UI(ID_NINESEVEN, &CMainFrame::OnUpdateNinetySeven)
ON_UPDATE_COMMAND_UI(ID_NINEFIVE, &CMainFrame::OnUpdateNinetyFive)
ON_UPDATE_COMMAND_UI(ID_ONEHUNDRED, &CMainFrame::OnUpdateOneHundred)

ON_UPDATE_COMMAND_UI(ID_Efficiency, &CMainFrame::OnUpdateEfficiency)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
	fileData=new FileData();

	m_bFlag=false;
	
	m_PWave=NULL;
	m_P3DFFT=NULL;
	m_Axis=NULL;
	m_TrendView=NULL;
	m_DynEqu=NULL;
	m_Cam=NULL;
	m_nTabLook=ID_TABGRAY;
	m_nAppLook = ID_VIEW_APPLOOK_MODERN;
	//m_nTabLook=	theApp.GetInt (_T("ApplicationLook"), ID_TABGRAY);
	//m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_VS2008);
	// TODO: add member initialization code here

	///*--------------------------Marshall's Code2011.2.21 18:20--------------*/
	// m_bConf97 = true;
	// m_bConf95 = false;
	// m_bConf100 = false;
	///*--------------------------Marshall's Code2011.2.21 18:20--------------*/
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//this->m_bAutoMenuEnable=FALSE;
	// Preserve the existing menu metrics while changing its typeface.
	LOGFONT uiFont = {};
	globalData.fontRegular.GetLogFont(&uiFont);
	lstrcpyn(uiFont.lfFaceName, _T("Microsoft YaHei UI"), LF_FACESIZE);
	uiFont.lfQuality = CLEARTYPE_QUALITY;
	CBCGPMenuBar::SetMenuFont(&uiFont);
	OnAppLook (m_nAppLook);
	
	//---------------------------------
	// Set toolbar and menu image size:
	//---------------------------------
	//CBCGPToolBar::SetSizes (CSize (28, 28), CSize (24, 24));
	//CBCGPToolBar::SetMenuSizes (CSize (22, 21), CSize (16, 15));

	//
	//if (CBCGPToolBar::GetUserImages () == NULL)
	//{
	//	// Load toolbar user images:
	//	if (!m_UserImages.Load (_T(".\\UserImages.bmp")))
	//	{
	//		TRACE(_T("Failed to load user images\n"));
	//	}
	//	else
	//	{
	//		CBCGPToolBar::SetUserImages (&m_UserImages);
	//	}
	//}

	//CBCGPToolBar::EnableQuickCustomization ();


	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.

	CList<UINT, UINT>	lstBasicCommands;

	//lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
	//lstBasicCommands.AddTail (ID_FILE_NEW);
	//lstBasicCommands.AddTail (ID_FILE_OPEN);
	//lstBasicCommands.AddTail (ID_FILE_SAVE);
	//lstBasicCommands.AddTail (ID_FILE_PRINT);
	lstBasicCommands.AddTail (ID_APP_EXIT);
	//lstBasicCommands.AddTail (ID_EDIT_CUT);
	//lstBasicCommands.AddTail (ID_EDIT_PASTE);
	//lstBasicCommands.AddTail (ID_EDIT_UNDO);
	//lstBasicCommands.AddTail (ID_RECORD_NEXT);
	//lstBasicCommands.AddTail (ID_RECORD_LAST);
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	//lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	//lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
	lstBasicCommands.AddTail (ID_WINDOW_TILE_HORZ);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2000);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_XP);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2003);
	//lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2005);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_WIN_XP);
	//lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_1);
	//lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_2);
	//lstBasicCommands.AddTail (ID_VIEW_APPLOOK_2007_3);
	lstBasicCommands.AddTail (ID_VIEW_APPLOOK_VS2008);

	//CBCGPToolBar::SetBasicCommands (lstBasicCommands);

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	if (!m_wndToolBarStandard.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+100) ||
		!m_wndToolBarStandard.LoadToolBar(IDR_FILETOOlBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndWaveToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+101) ||
		!m_wndWaveToolBar.LoadToolBar(IDR_WAVETOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndFraqToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+102) ||
		!m_wndFraqToolBar.LoadToolBar(IDR_FRAQTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndChannelsToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+103) ||
		!m_wndChannelsToolBar.LoadToolBar(IDR_CHANNELSTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndStripToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+104) ||
		!m_wndStripToolBar.LoadToolBar(IDR_STRIPTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndT3dToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+105) ||
		!m_wndT3dToolBar.LoadToolBar(IDR_T3dTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndAxisToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+106) ||
		!m_wndAxisToolBar.LoadToolBar(IDR_AxisTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndTrendToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+107) ||
		!m_wndTrendToolBar.LoadToolBar(IDR_TrendTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndDynEqToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+108) ||
		!m_wndDynEqToolBar.LoadToolBar(IDR_DynEqTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndJiggerToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+109) ||
		!m_wndJiggerToolBar.LoadToolBar(IDR_JiggerTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndEffToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+110) ||
		!m_wndEffToolBar.LoadToolBar(IDR_EfficiencyTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndCoherentToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+111) ||
		!m_wndCoherentToolBar.LoadToolBar(IDR_CoherentTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndSCHNToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+112) ||
		!m_wndSCHNToolBar.LoadToolBar(IDR_SaveWaveToolBar))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//if (!m_wndChannelsBATBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR+104) ||
	//	!m_wndChannelsBATBar.LoadToolBar(IDR_CHNBAT))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}



	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	//CClientDC dc (this);
	//BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	//UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBARFILE : 0;

	//if (!m_wndToolBarStandard.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBarStandard.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, uiToolbarHotID))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}
	if (!m_wndChannelsBATBarFile.Create (this, 0x50402808, AFX_IDW_TOOLBAR + 180))
	{
		TRACE0("Failed to create tabbed toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndTabbedToolBarGrap.Create (this, 0x50402808, AFX_IDW_TOOLBAR + 181))
	{
		TRACE0("Failed to create tabbed toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndTabbedToolBarChanl.Create (this, 0x50402808, AFX_IDW_TOOLBAR + 182))
	{
		TRACE0("Failed to create tabbed toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndTabbedToolBarExamGrap.Create (this, 0x50402808, AFX_IDW_TOOLBAR + 183))
	{
		TRACE0("Failed to create tabbed toolbar\n");
		return -1;      // fail to create
	}
	//if (!m_wndTabbedToolBarStrip.Create (this))
	//{
	//	TRACE0("Failed to create tabbed toolbar\n");
	//	return -1;      // fail to create
	//}
	//if (!m_wndTabbedToolBarT3d.Create (this))
	//{
	//	TRACE0("Failed to create tabbed toolbar\n");
	//	return -1;      // fail to create
	//}
	//if (!m_wndTabbedToolBarAxis.Create (this))
	//{
	//	TRACE0("Failed to create tabbed toolbar\n");
	//	return -1;      // fail to create
	//}
	if (!m_wndTabbedToolBarExamAnsy.Create (this, 0x50402808, AFX_IDW_TOOLBAR + 184))
	{
		TRACE0("Failed to create tabbed toolbar\n");
		return -1;      // fail to create
	}
	//if (!m_wndTabbedToolBarJigger.Create (this))
	//{
	//	TRACE0("Failed to create tabbed toolbar\n");
	//	return -1;      // fail to create
	//}
	//if (!m_wndTabbedToolBarDynEq.Create (this))
	//{
	//	TRACE0("Failed to create tabbed toolbar\n");
	//	return -1;      // fail to create
	//}
	//if (!m_wndTabbedToolBarEff.Create (this))
	//{
	//	TRACE0("Failed to create tabbed toolbar\n");
	//	return -1;      // fail to create
	//}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	// Load control bar icons:
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.SetTransparentColor (RGB (255, 0, 255));
	imagesWorkspace.Load (IDB_WORKSPACE);

	if (!m_wndWorkSpace.Create (_T("数据"), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);

	//if (!m_wndWorkSpace2.Create (_T("效率"), this, CRect (0, 0, 200, 200),
	//	TRUE, ID_VIEW_WORKSPACE2,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Workspace bar 2\n");
	//	return -1;      // fail to create
	//}

	//m_wndWorkSpace2.SetIcon (imagesWorkspace.ExtractIcon (1), FALSE);


	if (!m_wndOutput.Create (_T("输出"), this, CRect (0, 0, 200, 200),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}

	m_wndOutput.SetIcon (imagesWorkspace.ExtractIcon (2), FALSE);

	//if (!m_wndWatchBar.Create (_T("观测"), this, CRect (0, 0, 200, 200),
	//	TRUE, 
	//	ID_VIEW_WATCH,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create watch bar\n");
	//	return FALSE;      // fail to create
	//}

	//m_wndWatchBar.SetIcon (imagesWorkspace.ExtractIcon (3), FALSE);

	//CString strMainToolbarTitle;
	//strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	//m_wndToolBar.SetWindowText (strMainToolbarTitle);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndWatchBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);


	m_wndChannelsBATBarFile.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	m_wndTabbedToolBarGrap.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	m_wndTabbedToolBarChanl.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	m_wndTabbedToolBarExamGrap.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);	
	//m_wndTabbedToolBarStrip.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);	
	//m_wndTabbedToolBarT3d.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	//m_wndTabbedToolBarAxis.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	m_wndTabbedToolBarExamAnsy.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);

	
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndChannelsBATBarFile);
	DockControlBar(&m_wndTabbedToolBarGrap);
	DockControlBar(&m_wndTabbedToolBarChanl);
	DockControlBar(&m_wndTabbedToolBarExamGrap);
	//DockControlBar(&m_wndTabbedToolBarStrip);
	//DockControlBar(&m_wndTabbedToolBarT3d);
	//DockControlBar(&m_wndTabbedToolBarAxis);
	DockControlBar(&m_wndTabbedToolBarExamAnsy);

	

	DockControlBar(&m_wndWorkSpace);
	//m_wndWorkSpace2.AttachToTabWnd (&m_wndWorkSpace, BCGP_DM_STANDARD, FALSE, NULL);

	DockControlBar(&m_wndOutput);
	//DockControlBar(&m_wndWatchBar);
	//m_wndWatchBar.AttachToTabWnd (&m_wndOutput, BCGP_DM_STANDARD, FALSE, NULL);
	//m_wndWatchBar.DockToWindow (&m_wndOutput, CBRS_ALIGN_LEFT);
	m_wndChannelsBATBarFile.SetWindowText (_T("文件操作"));
	m_wndToolBarStandard.SetWindowText (_T("文件"));

	m_wndTabbedToolBarGrap.SetWindowText (_T("图形操作"));
	m_wndWaveToolBar.SetWindowText (_T("波形图"));
	m_wndFraqToolBar.SetWindowText (_T("频谱图"));

	m_wndTabbedToolBarChanl.SetWindowText (_T("通道数据处理"));
	m_wndChannelsToolBar.SetWindowText (_T("通道数据处理"));
	m_wndSCHNToolBar.SetWindowText (_T("通道数据存储"));

	m_wndTabbedToolBarExamGrap.SetWindowText (_T("试验图谱"));
	//m_wndTabbedToolBarStrip.SetWindowText(_T("长图操作"));
	m_wndStripToolBar.SetWindowText(_T("长图操作"));

	//m_wndTabbedToolBarT3d.SetWindowText(_T("三维瀑布图"));
	m_wndT3dToolBar.SetWindowText(_T("三维瀑布图"));

	//m_wndTabbedToolBarAxis.SetWindowText(_T("轴心轨迹"));
	m_wndAxisToolBar.SetWindowText(_T("轴心轨迹"));

	m_wndTabbedToolBarExamAnsy.SetWindowText(_T("试验分析"));
	m_wndTrendToolBar.SetWindowText(_T("趋势分析"));

	//m_wndTabbedToolBarDynEq.SetWindowText(_T("动平衡分析"));
	m_wndDynEqToolBar.SetWindowText(_T("动平衡分析"));

	//m_wndTabbedToolBarJigger.SetWindowText(_T("盘车试验"));
	m_wndJiggerToolBar.SetWindowText(_T("盘车试验"));

	//m_wndTabbedToolBarEff.SetWindowText(_T("效率分析"));
	m_wndEffToolBar.SetWindowText(_T("效率分析"));
	m_wndCoherentToolBar.SetWindowText(_T("相关分析"));

	m_wndChannelsBATBarFile.AddTab (&m_wndToolBarStandard);
	m_wndTabbedToolBarGrap.AddTab (&m_wndWaveToolBar);
	m_wndTabbedToolBarGrap.AddTab (&m_wndFraqToolBar, TRUE, FALSE, TRUE);	

	m_wndTabbedToolBarChanl.AddTab (&m_wndChannelsToolBar);
	m_wndTabbedToolBarChanl.AddTab (&m_wndSCHNToolBar, TRUE, FALSE, TRUE);

	m_wndTabbedToolBarExamGrap.AddTab (&m_wndStripToolBar);
	m_wndTabbedToolBarExamGrap.AddTab (&m_wndT3dToolBar, TRUE, FALSE, TRUE);
	m_wndTabbedToolBarExamGrap.AddTab (&m_wndAxisToolBar, TRUE, FALSE, TRUE);
	m_wndTabbedToolBarExamGrap.AddTab (&m_wndTrendToolBar, TRUE, FALSE, TRUE);
	m_wndTabbedToolBarExamGrap.AddTab (&m_wndCoherentToolBar, TRUE, FALSE, TRUE);
	//m_wndTabbedToolBarStrip.AddTab (&m_wndStripToolBar);

	//m_wndTabbedToolBarT3d.AddTab (&m_wndT3dToolBar);

	//m_wndTabbedToolBarAxis.AddTab (&m_wndAxisToolBar);


	m_wndTabbedToolBarExamAnsy.AddTab (&m_wndDynEqToolBar);
	m_wndTabbedToolBarExamAnsy.AddTab (&m_wndJiggerToolBar, TRUE, FALSE, TRUE);
	m_wndTabbedToolBarExamAnsy.AddTab (&m_wndEffToolBar, TRUE, FALSE, TRUE);



	//	m_wndToolBar.EnableCustomizeButton (TRUE, ID_VIEW_CUSTOMIZE, _T("Customize..."));

	// Allow user-defined toolbars operations:
	//InitUserToobars (NULL,
	//	uiFirstUserToolBarId,
	//	uiLastUserToolBarId);

	// Enable windows manager:
	//EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// Enable control bar context menu (list of bars + customize command):
	//EnableControlBarMenu (	
	//	TRUE,				// Enable
	//	ID_VIEW_CUSTOMIZE, 	// Customize command ID
	//	_T("Customize..."),	// Customize command text
	//	ID_VIEW_TOOLBARS);	// Menu items with this ID will be replaced by
	//CString s=fileData->ReturnCurrentPath();
	if (!PathFileExists(fileData->ReturnCurrentPath()+"\\log\\"))
	{
		CreateDirectory(fileData->ReturnCurrentPath()+"\\log\\",0);

		//AfxMessageBox("文档不存在，或路径不正确！");

	}
	return 0;
}

void CMainFrame::ActivateTabG(int cout)//ActivateTabGrap
{
	m_wndTabbedToolBarGrap.GetUnderlinedWindow()->SetActiveTab(cout);	//////////////////////////////////////////////////////////////////////////
	
}

void CMainFrame::ActivateTabEG(int cout)//ActivateTabGrap
{
	m_wndTabbedToolBarExamGrap.GetUnderlinedWindow()->SetActiveTab(cout);	//////////////////////////////////////////////////////////////////////////

}

void CMainFrame::ActivateTabEA(int cout)//ActivateTabGrap
{
	m_wndTabbedToolBarExamAnsy.GetUnderlinedWindow()->SetActiveTab(cout);	//////////////////////////////////////////////////////////////////////////

}
//void CMainFrame::ActivateTabC(int cout)
//{
//	//m_wndTabbedToolBarChanl.m_pTabWnd->SetActiveTab(cout);	//////////////////////////////////////////////////////////////////////////
//}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




//void CMainFrame::OnViewCustomize()
//{
//	//------------------------------------
//	// Create a customize toolbars dialog:
//	//------------------------------------
//	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize (this,
//		TRUE /* Automatic menus scaning */);
//
//	pDlgCust->EnableUserDefinedToolbars ();
//	pDlgCust->Create ();
//}

//afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
//{
//	// TODO: reset toolbar with id = (UINT) wp to its initial state:
//	//
//	// UINT uiToolBarId = (UINT) wp;
//	// if (uiToolBarId == IDR_MAINFRAME)
//	// {
//	//		do something with m_wndToolBar
//	// }
//
//	return 0;
//}

void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}

void CMainFrame::OnAppLook(UINT id)
{
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);

	m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_MODERN:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CHSJVisualManager));
		break;

	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	//case ID_VIEW_APPLOOK_2007:
	//case ID_VIEW_APPLOOK_2007_1:
	//case ID_VIEW_APPLOOK_2007_2:
	//case ID_VIEW_APPLOOK_2007_3:
	//	// enable Office 2007 look:
	//	switch (m_nAppLook)
	//	{
	//	case ID_VIEW_APPLOOK_2007:
	//		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	//		break;

	//	case ID_VIEW_APPLOOK_2007_1:
	//		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_ObsidianBlack);
	//		break;

	//	case ID_VIEW_APPLOOK_2007_2:
	//		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Silver);
	//		break;

	//	case ID_VIEW_APPLOOK_2007_3:
	//		CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_Aqua);
	//		break;
	//	}

		//CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
		//CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		//break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS 2005 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
		CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D_ROUNDED;
		CBCGPVisualManagerVS2005::m_bRoundedAutohideButtons = TRUE;
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2008:
		// enable VS 2008 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2008));
		CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
		break;
	}

	CBCGPDockManager* pDockManager = GetDockManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustBarFrames ();
	}

	CBCGPTabbedControlBar::ResetTabs ();

	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	theApp.WriteInt (_T("ApplicationLook"), m_nAppLook);

}

void CMainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (m_nAppLook == pCmdUI->m_nID);
}

CBCGPMDIChildWnd* CMainFrame::CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/)
{
	if (lpcszDocName != NULL && lpcszDocName [0] != '\0')
	{
		CDocument* pDoc = AfxGetApp()->OpenDocumentFile (lpcszDocName);

		if (pDoc != NULL)
		{
			POSITION pos = pDoc->GetFirstViewPosition();

			if (pos != NULL)
			{
				CView* pView = pDoc->GetNextView (pos);
				if (pView == NULL)
				{
					return NULL;
				}

				return DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, pView->GetParent ());
			}   
		}
	}

	return NULL;
}

void CMainFrame::OnClose() 
{
	const HWND hMainWindow = m_hWnd;
	SaveMDIState (theApp.GetRegSectionPath ());

	CBCGPMDIFrameWnd::OnClose();

	// The base close handler can delete this frame; use only the saved HWND.
	// MFC can hide the frame while OLE objects remain locked. Finish that close.
	if (::IsWindow(hMainWindow) && !::IsWindowVisible(hMainWindow))
		::DestroyWindow(hMainWindow);
	// Preserve a cancelled close, but end the message loop once the window is gone.
	if (!::IsWindow(hMainWindow))
		::PostQuitMessage(0);
}
void CMainFrame::OnChangeLook ( BOOL bDockTabColors )
{
	CBCGPTabbedControlBar::EnableTabAutoColor (bDockTabColors);
}

  //RIBBON_APP

void CMainFrame::OnTabcolour(UINT id)
{
	CBCGPMDITabParams mdiTabParams;
	m_nTabLook=id;
	switch (m_nTabLook)
	{
		case ID_TABGRAY:
			OnChangeLook (FALSE);
			UnlockWindowUpdate ();
			RedrawWindow ();
			CBCGPDockManager::SetDockMode (BCGP_DT_SMART);

			mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005;
			mdiTabParams.m_bAutoColor = FALSE;
			EnableMDITabbedGroups (TRUE, mdiTabParams);

			break;
		case ID_TABCOLOUR:
			OnChangeLook (TRUE);
			UnlockWindowUpdate ();
			RedrawWindow ();

			mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_ONENOTE;
			mdiTabParams.m_bAutoColor = TRUE;
			EnableMDITabbedGroups (TRUE, mdiTabParams);
			CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
				break;
	}
	theApp.WriteInt (_T("ApplicationLook"), m_nTabLook);
}

void CMainFrame::OnUpdateTabcolour(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio (m_nTabLook == pCmdUI->m_nID);
	
}

void CMainFrame::OnFileopendata()	//打开分拣数据文件
{
	
	CString strFilter="数据分拣文件 (*.SEL) | *.SEL;*.SEL ||";
	CFileDialog dlg(TRUE,//open/Save as
		NULL,//Ext added automatically
		NULL,//filename
		OFN_HIDEREADONLY |OFN_FILEMUSTEXIST,
		strFilter);
	if(dlg.DoModal()==IDOK)

	{	
		bool m_tempB=false;
		if (m_bFlag)
			m_tempB=true;
		OnRefreshApp();
		if (m_tempB)
			m_bFlag=true;

		FilePathName=dlg.GetPathName();		
		vector<CString>::iterator result =std::find(FilePathVector.begin(),FilePathVector.end(),FilePathName);
		if (result==FilePathVector.end())
		{
			if(FilePathName.Right(3)=="SEL"||FilePathName.Right(3)=="sel")
			{
				//	AfxMessageBox(FilePathName);
				char file_path[MAX_CHN_NAME_LEN*2];
				memset(file_path,0,MAX_CHN_NAME_LEN*2);
				char tmpstr[32];
				memset(tmpstr,0,32);

				FILE * f=fopen(FilePathName,"r+b");
				if(f!=0)
				{
					if(CSysConf::m_Sort_Data->ReadHead(f)<0)
					{
						AfxMessageBox("分拣文件数据有误！");
						m_wndOutput.FillText(_T("分拣文件加载失败！"));
						return;
					}
					fclose(f);
				}
				
				SetFilePorperty((int)fileData->fullPathName.size()+1,FilePathName);
				m_wndWorkSpace.FillTree();

				m_wndOutput.FillText(_T("分拣文件加载成功！		文件名为："+FilePathName));
				m_bFlag=true;


			}
			else
			{
				if(FilePathName=="") return;
				AfxMessageBox("未知类型文件名！");
			}
			FilePathVector.push_back(FilePathName);
		}
		else
		{
			m_wndOutput.FillText(_T("分拣文件加载失败！"));
			CString str(_T("分拣文件 "));
			str.Append(FilePathName);
			str.Append(_T(" 已经加载！"));
			AfxMessageBox(str);
		}
	}
}

//void CMainFrame::OnAllgraphvisual()
//{
//	//int treeClass = 1,i=0;
//	//CString m_ParName[3];
//	//for(i=0;i<3;i++)
//	//{
//	//	m_ParName[i] = _T("");
//	//}	
//
//	//HTREEITEM   m_selectedItem   =   m_wndWorkSpace.m_wndTree.GetSelectedItem();   
//
//	//CString m_SelName = m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
//	//m_ParName[0]=m_SelName;
//	//HTREEITEM hItem = m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
//
//	//while(hItem!=m_wndWorkSpace.m_wndTree.GetRootItem())
//	//{
//	//	treeClass++;
//	//	m_ParName[treeClass-1] = m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
//	//	hItem = m_wndWorkSpace.m_wndTree.GetParentItem(hItem);
//	//}
//
//	////
//
//	//HTREEITEM    hmyItem=m_wndWorkSpace.m_wndTree.GetSelectedItem();   
//	//if(m_wndWorkSpace.m_wndTree.ItemHasChildren(hmyItem))   
//	//{   
//	//	HTREEITEM    hNextItem;   
//	//	HTREEITEM    hChildItem    =  m_wndWorkSpace.m_wndTree.GetChildItem(hmyItem);   
//
//	//	while    (hChildItem    !=    NULL)   
//	//	{ 
//	//		CString CTitle;
//	//		for (int p=2;p>=0;p--)
//	//		{
//	//			CTitle+=m_ParName[p]+", ";
//	//		}
//
//	//		CString m_Name = m_wndWorkSpace.m_wndTree.GetItemText(hChildItem);
//	//		CTitle+=m_Name+"通道波形图";
//
//	//		char str_tmp[MAX_CHN_NAME_LEN];
//	//		memset(str_tmp,0,MAX_CHN_NAME_LEN);	
//
//	//		CChildFrame* pCFrame = new CChildFrame();
//
//	//		CCreateContext context;
//	//		context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
//	//		if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
//	//			return;
//
//	//		pCFrame->titles=CTitle;
//	//		pCFrame->ShowWindow(SW_SHOW);
//
//
//	//		m_PWave->SetTitle(CTitle);
//
//	//		m_PWave->SetData(fileData->GetDataFromFile(FilePathName,m_ParName[0],m_ParName[1],m_Name));
//
//	//		ActivateTab(1);
//	//		pCFrame->InitialUpdateFrame(NULL,true);	
//	//		m_PWave->SaveAllView();
//	//		//pCFrame->DestroyWindow();
//	//		//pCFrame->CloseWindow();
//	//		
//	//		hNextItem    =    m_wndWorkSpace.m_wndTree.GetNextItem(hChildItem,    TVGN_NEXT); 
//	//		hChildItem    =    hNextItem;	
//
//	//	}   
//	//}   
//	int treeClass = 1,i=0;
//	CString m_ParName[3];
//	for(i=0;i<3;i++)
//	{
//		m_ParName[i] = _T("");
//	}	
//	SaveImageBatDlg sBD;
//	HTREEITEM   m_selectedItem   =   m_wndWorkSpace.m_wndTree.GetSelectedItem();   
//
//	CString m_SelName = m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
//	m_ParName[0]=m_SelName;
//	HTREEITEM hItem = m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
//
//	while(hItem!=m_wndWorkSpace.m_wndTree.GetRootItem())
//	{
//		treeClass++;
//		m_ParName[treeClass-1] = m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
//		hItem = m_wndWorkSpace.m_wndTree.GetParentItem(hItem);
//
//	}
//	CString s=m_ParName[treeClass-1];
//	
//	//fileData
//	CString ct=fileData->GetCout(s);
//	int p=0;
//	CString strPath;
//	vector<CString>::iterator fullPath=fileData->fullPathName.begin();
//	for (;fullPath!=fileData->fullPathName.end();fullPath++)
//	{
//		p++;
//		if (p==atoi(ct))
//		{
//			strPath=*fullPath;			
//		}
//		//fileNameComb.InsertString(0,*fullPath);		
//	}
//	sBD.filePath=strPath;
//	sBD.examParam=fileData->RemoveCoutFromString(m_ParName[0]);
//	sBD.examType=fileData->RemoveCoutFromString(m_ParName[1]);
//	sBD.examName=fileData->RemoveCoutFromString(m_ParName[2]);
//	
//	if(m_wndWorkSpace.m_wndTree.ItemHasChildren(m_selectedItem))   
//	{   
//		HTREEITEM    hNextItem;   
//		HTREEITEM    hChildItem    =  m_wndWorkSpace.m_wndTree.GetChildItem(m_selectedItem);   
//
//		while    (hChildItem    !=    NULL)   
//		{ 
//
//			CString m_Name = m_wndWorkSpace.m_wndTree.GetItemText(hChildItem);
//
//			CString mName=fileData->RemoveCoutFromString(m_Name);
//			sBD.SetCHNnameBool(mName,TRUE);
//
//			hNextItem    =    m_wndWorkSpace.m_wndTree.GetNextItem(hChildItem,    TVGN_NEXT); 
//			hChildItem    =    hNextItem;	
//
//		}   
//	}   
//
//	sBD.DoModal();
//}

void CMainFrame::OnFilepproperty()
{
	// TODO: 在此添加命令处理程序代码
	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{
		m_wndOutput.FillText(_T("查看分拣文件属性..."));
		FilePorperty fp;
		fp.DoModal();		
	}

}

//void CMainFrame::OnUpdateFilepproperty(CCmdUI *pCmdUI)
//{
//	// TODO: 在此添加命令更新用户界面处理程序代码
//
//}

void CMainFrame::SetFilePorperty(int k, CString FilePathName)
{
	//1 获取文件名
	int Where;   
	Where = FilePathName.ReverseFind('\\');  
	if (Where == -1)  
		Where = FilePathName.ReverseFind('/');  
	CString fileTitle = FilePathName.Right(FilePathName.GetLength() - 1 - Where); 
	int Which = fileTitle.ReverseFind('.');   
	if (Which != -1)   
		fileTitle = fileTitle.Left(Which); 
	//fileData->SetFileName(fileTitle);		//1 设置文件名
	fileData->SetFullPathName(k,FilePathName);
	//2 获取文件路径
	CString filePath = FilePathName.Left(Where+1);		

	//3 获取文件时间
	CFileStatus   Status;   
	CString strCTime,strMTime,strATime;
	if(CFile::GetStatus(FilePathName,Status))   
	{   
		strCTime=Status.m_ctime.Format(_T("%Y-%m-%d  %H: %M: %S")); // 文件创建时间
		strMTime=Status.m_mtime.Format(_T("%Y-%m-%d  %H: %M: %S"));// 文件最近一次修改时间 
		strATime=Status.m_atime.Format(_T("%Y-%m-%d  %H: %M: %S")); // 文件最近一次访问时间
	}  


	//4 获取试验名
	CString examName;
	examName=_T(CSysConf::m_Sort_Data->m_ExamName);
	
	CString samFreq;
		samFreq.Format("%d",CSysConf::m_Sort_Data->m_SamFreq);
	CString sortLen;
		sortLen.Format("%d",CSysConf::m_Sort_Data->m_SortLen);
	CString etpNum;
		for(int j=0;j<CSysConf::m_Sort_Data->m_ExamTypeNum;j++)
		{		
			etpNum.Format("%d",CSysConf::m_Sort_Data->m_ExamTypeParaNum[j]);
		}		
	CString cNum;
		cNum.Format("%d",CSysConf::m_Sort_Data->m_chnNum);	

	fileData->SetFilePorperty(FilePathName,fileTitle,filePath,strCTime,strMTime,strATime,examName,
							  samFreq,etpNum,sortLen,cNum);		//2 设置文件路径	//3 设置文件创建时间 //4 设置试验名
	////5 试验类型
	////6 试验参数
	//
	//for(int j=0;j<CSysConf::m_Sort_Data->m_ExamTypeNum;j++)
	//{
	//	fileData->SetExamType(fileTitle,_T(CSysConf::m_Sort_Data->m_ExamType[j]));		//5 设置试验类型
	//	for(int k=0;k<CSysConf::m_Sort_Data->m_ExamTypeParaNum[j];k++)
	//	{
	//		fileData->SetExamParams(_T(CSysConf::m_Sort_Data->m_ExamTypePara[j][k]));		//5 设置试验参数
	//	}
	//		fileData->AddExamParams(_T(CSysConf::m_Sort_Data->m_ExamType[j]));		//5 设置试验参数

	//}
	
	//设置文件分拣信息
	//int s=CSysConf::m_Sort_Data->m_SortBaseIDArr[0];
	//int s0=CSysConf::m_Sort_Data->GetChnIdx(s);
	//CString strchn;
	//strchn.Format("%s",CSysConf::m_Sort_Data->m_chnNameArr[s0]);


	fileData->SetSortPorperty(FilePathName);

	//int k=CSysConf::m_Sort_Data->GetChnIdx(1);
	//int k0=CSysConf::m_Sort_Data->GetChnIdx("备用4振动");

	//int k1=CSysConf::m_Sort_Data->GetChnIdxByDesc("备用4振动");
	//int k2=CSysConf::m_Sort_Data->GetExamTypeIdx("升负荷23");
	//int k3=CSysConf::m_Sort_Data->GetExamParaIdx(0,"0％");
	
	//int s=CSysConf::m_Sort_Data->m_SortBaseIDArr[0];
	////unsigned short s0=CSysConf::m_Sort_Data->m_chnIDArr[s];
	//int s0=CSysConf::m_Sort_Data->GetChnIdx(s);
	//////通道名；

	//char* s1=CSysConf::m_Sort_Data->m_chnNameArr[s0];
	//		hRes = m_wndTree.InsertItem (_T(CSysConf::m_Sort_Data->m_ExamTypePara[j][k]), 3, 3, hInc);
	//		m_wndTree.vectorTreeName.push_back(_T(CSysConf::m_Sort_Data->m_ExamTypePara[j][k]));
	//		for(m=0;m<CSysConf::m_Sort_Data->m_chnNum;m++)
	//		{
	//			stmp = CSysConf::m_Exam_Idx->GetChnIdxByID(CSysConf::m_Sort_Data->m_chnIDArr[m]);
	//			if(stmp<0||stmp>=MAX_CHANNEL_NUM) continue;
	//			//m_wndTree.InsertItem (_T(CSysConf::m_Exam_Idx->m_ChnName[stmp]), 4, 4, hRes);
	//			//strCout="[";
	//			//i++;
	//			//temp.Format("%d",i);
	//			//strCout.Append(temp);
	//			//strCout.Append("] ");
	//			//strCout.Append(_T(CSysConf::m_Sort_Data->m_chnNameArr[m]));
	//			m_wndTree.InsertItem (CSysConf::m_Sort_Data->m_chnNameArr[m], 4, 4, hRes);				
	//		}
	//		m_wndTree.Expand (hRes, TVE_COLLAPSE);
	//	}
	//	m_wndTree.Expand (hInc, TVE_EXPAND);


	//int i0=CSysConf::m_Sort_Data->m_SamFreq;//1024//////////////////////////////////////////////////////////////////////////
	//int i1=CSysConf::m_Sort_Data->m_SortNum;;//3//////////////////////////////////////////////////////////////////////////
	//int i2=CSysConf::m_Sort_Data->m_OriSortNum;;//0
	//int i3=CSysConf::m_Sort_Data->m_SortLen;//61440
	//long i4=CSysConf::m_Sort_Data->m_SortTimeBeginArr[MAX_SORT_SPAN_NUM];//61440//////////////////////////////////////////////////////////////////////////
	//long i5=CSysConf::m_Sort_Data->m_SortTimeEndArr[MAX_SORT_SPAN_NUM];//61440//////////////////////////////////////////////////////////////////////////
	//int i6=CSysConf::m_Sort_Data->m_SortStartPoint[MAX_SORT_SPAN_NUM];

	////终止点数；
	//int i7=CSysConf::m_Sort_Data->m_SortEndPoint[MAX_SORT_SPAN_NUM];

	////分拣起始点数；
	//int i8=CSysConf::m_Sort_Data->m_ActionStartPoint[MAX_SORT_SPAN_NUM];//////////////////////////////////////////////////////////////////////////

	////分拣终止点数；
	//int i9=CSysConf::m_Sort_Data->m_ActionEndPoint[MAX_SORT_SPAN_NUM];//////////////////////////////////////////////////////////////////////////
	///***************************************************/

	////分拣有效队列；
	//unsigned char i10=CSysConf::m_Sort_Data->m_SortValidArr[MAX_SORT_SPAN_NUM];

	///************************************************************************/
	///* 20090506                                                             */
	///************************************************************************/
	////分拣依据ID
	//unsigned short i11=CSysConf::m_Sort_Data->m_SortBaseIDArr[MAX_SORT_SPAN_NUM];//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	////试验类型数量；
	//int i12=CSysConf::m_Sort_Data->m_ExamTypeNum;

	////试验参数数量；
	//int i13=CSysConf::m_Sort_Data->m_ExamTypeParaNum[MAX_EXAM_TYPE_NUM];//////////////////////////////////////////////////////////////////////////

	////试验状态；
	//unsigned char i14=CSysConf::m_Sort_Data->m_ExamState;

	////试验类型状态；
	//unsigned char i15=CSysConf::m_Sort_Data->m_ExamTypeState[MAX_EXAM_TYPE_NUM];

	////试验参数状态；
	//unsigned char i16=CSysConf::m_Sort_Data->m_ExamTypeParaState[MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	////试验参数通道状态；
	//unsigned char i17=CSysConf::m_Sort_Data->m_ChnState[MAX_EXAM_CHN_NUM];

	////机组ID；
	//char i18=CSysConf::m_Sort_Data->m_SetID[MAX_CHN_NAME_LEN];

	////通道数量；
	//int i19=CSysConf::m_Sort_Data->m_chnNum;//////////////////////////////////////////////////////////////////////////

	////通道ID;
	//unsigned short i20=CSysConf::m_Sort_Data->m_chnIDArr[MAX_EXAM_CHN_NUM];

	////通道名；
	//char i21=CSysConf::m_Sort_Data->m_chnNameArr[MAX_EXAM_CHN_NUM][MAX_CHN_NAME_LEN];//////////////////////////////////////////////////////////////////////////

	///************************************************************************/
	///* 20090613                                                             */
	///************************************************************************/
	////单位；
	//char i22=CSysConf::m_Sort_Data->m_chnUnitArr[MAX_EXAM_CHN_NUM][UNIT_STR_LEN];//////////////////////////////////////////////////////////////////////////

	////变换A系数；
	//float i23=CSysConf::m_Sort_Data->m_aParaArr[MAX_EXAM_CHN_NUM];

	////变换B系数；
	//float i24=CSysConf::m_Sort_Data->m_bParaArr[MAX_EXAM_CHN_NUM];
	////////////////////////////////////////////////////////////////////////////

	///************************************************************************/
	///* 20090620                                                             */
	///************************************************************************/
	////fft计算开关；
	//unsigned char i25=CSysConf::m_Sort_Data->m_fftFlagArr[MAX_EXAM_CHN_NUM];
	////////////////////////////////////////////////////////////////////////////

	////试验名；
	//char i26=CSysConf::m_Sort_Data->m_ExamName[MAX_CHN_NAME_LEN];//////////////////////////////////////////////////////////////////////////

	////类型名；
	//char i27=CSysConf::m_Sort_Data->m_ExamType[MAX_EXAM_TYPE_NUM][MAX_CHN_NAME_LEN];//////////////////////////////////////////////////////////////////////////

	////参数名；
	//char i28=CSysConf::m_Sort_Data->m_ExamTypePara[MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_CHN_NAME_LEN];//////////////////////////////////////////////////////////////////////////

	////对应的分拣号；
	//unsigned short i29=CSysConf::m_Sort_Data->m_ExamTypeParaSortCode[MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];
}

//void CMainFrame::GetFilePorperty()
//{
//
//}
//void CMainFrame::OnSavepicbat()
//{
//
//	int treeClass = 1,i=0;
//	CString m_ParName[2];
//	for(i=0;i<2;i++)
//	{
//		m_ParName[i] = _T("");
//	}	
//	SaveImageBatDlg sBD;
//	HTREEITEM   m_selectedItem   =   m_wndWorkSpace.m_wndTree.GetSelectedItem();   
//	
//	
//
//	CString m_SelName = m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
//	m_ParName[0]=m_SelName;
//	HTREEITEM hItem = m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
//	if (m_wndWorkSpace.m_wndTree.GetRootItem()==m_wndWorkSpace.m_wndTree.GetParentItem(hItem))
//	{
//	while(hItem!=m_wndWorkSpace.m_wndTree.GetRootItem())
//	{
//		treeClass++;
//		m_ParName[treeClass-1] = m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
//		hItem = m_wndWorkSpace.m_wndTree.GetParentItem(hItem);
//
//	}
//	CString s=m_ParName[treeClass-1];
//
//	//fileData
//	CString ct=fileData->GetCout(s);
//	int p=0;
//	CString strPath;
//	map<int,CString>::iterator fullPath=fileData->fullPathName.begin();
//	for (;fullPath!=fileData->fullPathName.end();fullPath++)
//	{
//		p++;
//		if (p==atoi(ct))
//		{
//			strPath=fullPath->second;			
//		}
//		//fileNameComb.InsertString(0,*fullPath);		
//	}
//	sBD.filePath=strPath;
//	sBD.examType=fileData->RemoveCoutFromString(m_ParName[0]);
//	sBD.examName=fileData->RemoveCoutFromString(m_ParName[1]);
//	//sBD.examName=fileData->RemoveCoutFromString(m_ParName[2]);//examParam
//
//	if(m_wndWorkSpace.m_wndTree.ItemHasChildren(m_selectedItem))   
//	{   
//		HTREEITEM    hNextItem;   
//		HTREEITEM    hChildItem    =  m_wndWorkSpace.m_wndTree.GetChildItem(m_selectedItem);   
//
//		while    (hChildItem    !=    NULL)   
//		{ 
//			
//			CString m_Name = m_wndWorkSpace.m_wndTree.GetItemText(hChildItem);
//
//			CString mName=fileData->RemoveCoutFromString(m_Name);
//			sBD.SetExamParam(mName);
//			HTREEITEM    hCNextItem;
//			HTREEITEM    hChildCItem    =  m_wndWorkSpace.m_wndTree.GetChildItem(hChildItem);
//			int k=1000;
//			while    (hChildCItem    !=    NULL)  
//			{
//				k++;
//				CString cout;
//				cout.Format("#%d",k);
//				CString m_CName = m_wndWorkSpace.m_wndTree.GetItemText(hChildCItem);
//				sBD.SetCHNnameBool(fileData->RemoveCoutFromString(m_Name)+cout,fileData->RemoveCoutFromString(m_CName),TRUE);
//				
//				hCNextItem    =    m_wndWorkSpace.m_wndTree.GetNextItem(hChildCItem,    TVGN_NEXT); 
//				hChildCItem    =    hCNextItem;
//			}
//			//
//
//			hNextItem    =    m_wndWorkSpace.m_wndTree.GetNextItem(hChildItem,    TVGN_NEXT); 
//			hChildItem    =    hNextItem;	
//
//		}   
//	}
//	sBD.DoModal();
//	}
//	else
//	{
//		if (fileData->fileTitle.size()==0)
//		{
//			AfxMessageBox("尚未加载文件");
//			OnFileopendata();
//		}
//		else
//		{
//			AfxMessageBox("请选择正确的试验类型");
//			m_wndWorkSpace.m_wndTree.SetFocus();
//			HTREEITEM root=m_wndWorkSpace.m_wndTree.GetRootItem();
//			HTREEITEM rFirst=m_wndWorkSpace.m_wndTree.GetChildItem(root);
//			HTREEITEM rFFirst=m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);			
//			m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);
//			
//		}
//	}
//
//}

//void CMainFrame::OnSavetxtdata()
//{
//	int treeClass = 1,i=0;
//	CString m_ParName[2];
//	for(i=0;i<2;i++)
//	{
//		m_ParName[i] = _T("");
//	}	
//	SaveDataDlg sBD;
//	HTREEITEM   m_selectedItem   =   m_wndWorkSpace.m_wndTree.GetSelectedItem();   
//	//sBD.SaveFileType=1;
//	CString m_SelName = m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
//	m_ParName[0]=m_SelName;
//	HTREEITEM hItem = m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
//	if (m_wndWorkSpace.m_wndTree.GetRootItem()==m_wndWorkSpace.m_wndTree.GetParentItem(hItem))
//	{
//	while(hItem!=m_wndWorkSpace.m_wndTree.GetRootItem())
//	{
//		treeClass++;
//		m_ParName[treeClass-1] = m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
//		hItem = m_wndWorkSpace.m_wndTree.GetParentItem(hItem);
//
//	}
//	CString s=m_ParName[treeClass-1];
//
//	//fileData
//	CString ct=fileData->GetCout(s);
//	int p=0;
//	CString strPath;
//	map<int,CString>::iterator fullPath=fileData->fullPathName.begin();
//	for (;fullPath!=fileData->fullPathName.end();fullPath++)
//	{
//		p++;
//		if (p==atoi(ct))
//		{
//			strPath=fullPath->second;			
//		}
//		//fileNameComb.InsertString(0,*fullPath);		
//	}
//	sBD.filePath=strPath;
//	//sBD.examParam=fileData->RemoveCoutFromString(m_ParName[0]);
//	sBD.examType=fileData->RemoveCoutFromString(m_ParName[0]);
//	sBD.examName=fileData->RemoveCoutFromString(m_ParName[1]);
//	if(m_wndWorkSpace.m_wndTree.ItemHasChildren(m_selectedItem))   
//	{   
//		HTREEITEM    hNextItem;   
//		HTREEITEM    hChildItem    =  m_wndWorkSpace.m_wndTree.GetChildItem(m_selectedItem);   
//
//		while    (hChildItem    !=    NULL)   
//		{ 
//
//			CString m_Name = m_wndWorkSpace.m_wndTree.GetItemText(hChildItem);
//
//			CString mName=fileData->RemoveCoutFromString(m_Name);
//			sBD.SetExamParam(mName);
//			HTREEITEM    hCNextItem;
//			HTREEITEM    hChildCItem    =  m_wndWorkSpace.m_wndTree.GetChildItem(hChildItem);
//			int k=1000;
//			while    (hChildCItem    !=    NULL)  
//			{
//				k++;
//				CString cout;
//				cout.Format("#%d",k);
//				CString m_CName = m_wndWorkSpace.m_wndTree.GetItemText(hChildCItem);
//				sBD.SetCHNnameBool(fileData->RemoveCoutFromString(m_Name)+cout,fileData->RemoveCoutFromString(m_CName),TRUE);
//
//				hCNextItem    =    m_wndWorkSpace.m_wndTree.GetNextItem(hChildCItem,    TVGN_NEXT); 
//				hChildCItem    =    hCNextItem;
//			}
//			//
//
//			hNextItem    =    m_wndWorkSpace.m_wndTree.GetNextItem(hChildItem,    TVGN_NEXT); 
//			hChildItem    =    hNextItem;	
//
//		}   
//	}
//	sBD.DoModal();
//	}
//	//if(m_wndWorkSpace.m_wndTree.ItemHasChildren(m_selectedItem))   
//	//{   
//	//	HTREEITEM    hNextItem;   
//	//	HTREEITEM    hChildItem    =  m_wndWorkSpace.m_wndTree.GetChildItem(m_selectedItem);   
//
//	//	while    (hChildItem    !=    NULL)   
//	//	{ 
//
//	//		CString m_Name = m_wndWorkSpace.m_wndTree.GetItemText(hChildItem);
//
//	//		CString mName=fileData->RemoveCoutFromString(m_Name);
//	//		sBD.SetCHNnameBool(mName,TRUE);
//
//	//		hNextItem    =    m_wndWorkSpace.m_wndTree.GetNextItem(hChildItem,    TVGN_NEXT); 
//	//		hChildItem    =    hNextItem;	
//
//	//	}   
//	//}   
//	else
//	{
//		if (fileData->fileTitle.size()==0)
//		{
//			AfxMessageBox("尚未加载文件");
//			OnFileopendata();
//		}
//		else
//		{
//			AfxMessageBox("请选择正确的试验类型");
//			m_wndWorkSpace.m_wndTree.SetFocus();
//			HTREEITEM root=m_wndWorkSpace.m_wndTree.GetRootItem();
//			HTREEITEM rFirst=m_wndWorkSpace.m_wndTree.GetChildItem(root);
//			HTREEITEM rFFirst=m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);			
//			m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);
//
//		}
//	}
//	
//}

//void CMainFrame::OnSaveexceldata()
//{
//	//m_wndWorkSpace.m_wndTree.GetRootItem();
//	int treeClass = 1,i=0;
//	CString m_ParName[2];
//	for(i=0;i<2;i++)
//	{
//		m_ParName[i] = _T("");
//	}	
//	CSaveCHNToExcel cte;
//	HTREEITEM   m_selectedItem   =   m_wndWorkSpace.m_wndTree.GetSelectedItem();   
//	//sBD.SaveFileType=2;
//	CString m_SelName = m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
//	m_ParName[0]=m_SelName;
//	HTREEITEM hItem = m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
//
//	if (m_wndWorkSpace.m_wndTree.GetRootItem()==m_wndWorkSpace.m_wndTree.GetParentItem(hItem))
//	{
//	while(hItem!=m_wndWorkSpace.m_wndTree.GetRootItem())
//	{
//		treeClass++;
//		m_ParName[treeClass-1] = m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
//		hItem = m_wndWorkSpace.m_wndTree.GetParentItem(hItem);
//
//	}
//	CString s=m_ParName[treeClass-1];
//
//	//fileData
//	CString ct=fileData->GetCout(s);
//	int p=0;
//	CString strPath;
//	map<int,CString>::iterator fullPath=fileData->fullPathName.begin();
//	for (;fullPath!=fileData->fullPathName.end();fullPath++)
//	{
//		p++;
//		if (p==atoi(ct))
//		{
//			strPath=fullPath->second;			
//		}			
//	}
//	fileData->SetExamTree(strPath);
//	cte.examName=fileData->RemoveCoutFromString(m_ParName[1]);
//	cte.examType=fileData->RemoveCoutFromString(m_ParName[0]);
//	cte.filePath=strPath;
//	cte.paramVector=fileData->GetExamParam(strPath,fileData->RemoveCoutFromString(m_ParName[0]));
//	
//	cte.DoModal();
//	}
//	else
//	{
//		if (fileData->fileTitle.size()==0)
//		{
//			AfxMessageBox("尚未加载文件");
//			OnFileopendata();
//		}
//		else
//		{
//			AfxMessageBox("请选择正确的试验类型");
//			m_wndWorkSpace.m_wndTree.SetFocus();
//			HTREEITEM root=m_wndWorkSpace.m_wndTree.GetRootItem();
//			HTREEITEM rFirst=m_wndWorkSpace.m_wndTree.GetChildItem(root);
//			HTREEITEM rFFirst=m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);			
//			m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);
//
//		}
//	}
//
//}

//void CMainFrame::OnT3dwave()
//{
//
//	
//}
//消息：新建二维轴心轨迹试验
void CMainFrame::OnAxisnew()
{
	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{	
		CAxisSetDlg asd;
		asd.SetFunSel(1);
		if (asd.DoModal()==IDOK)
		{
			m_wndOutput.FillText(_T("新建二维轴心轨迹试验..."));
		}
		else
			m_wndOutput.FillText(_T("取消新建二维轴心轨迹试验"));

		ActivateTabEG(2);
	}

}
//消息：新建相干分析试验
void CMainFrame::OnCoherentays()
{
	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{	
		CAxisSetDlg asd;
		asd.SetFunSel(2);
		
		if (asd.DoModal()==IDOK)		
			m_wndOutput.FillText(_T("新建相干分析试验..."));		
		else
			m_wndOutput.FillText(_T("取消新建相干分析试验"));

		ActivateTabEG(4);
	}
}
//消息：加载外部数据完成FFT运算
void CMainFrame::OnLoddataFft()
{
	//m_wndChannelsToolBar.SetButtonStyle(0,TBBS_DISABLED);
	//{		CMenu menu;
	//VERIFY(menu.LoadMenu (IDR_MAINFRAME));
	//CMenu* pPopup = menu.GetSubMenu(2);
	//ASSERT(pPopup != NULL);
	//m_bAutoMenuEnable=FALSE;

	//CString str ;
	////for(int i=menu.GetSubMenu (2)->GetMenuItemCount()-1;i>=0;i--) 
	////{ 
	////	menu.GetSubMenu (2)->GetMenuString(i,str,MF_BYPOSITION); 
	////	if(str=="添加通道数据") 
	////	{ 

	//int k=menu.GetSubMenu (2)->EnableMenuItem(ID_KeyPhaseVisual,MF_BYCOMMAND   | MF_DISABLED   |   MF_GRAYED);
	////menu.GetSubMenu (2)->DeleteMenu(i-1,MF_BYPOSITION);  
	////		break; 
	////	}
	////}
	//for(int i=menu.GetSubMenu (2)->GetMenuItemCount()-1;i>=0;i--) 
	//{ 
	//	menu.GetSubMenu (2)->GetMenuString(i,str,MF_BYPOSITION); 
	//	if(str=="显示健相") 
	//	{ 

	//		int k=menu.GetSubMenu (2)->DeleteMenu(i,MF_BYPOSITION);
	//		//menu.GetSubMenu (2)->DeleteMenu(i-1,MF_BYPOSITION);  
	//		break; 
	//	}
	//}
	//menu.Detach();

	//}
	CLodDataFFT ldt;
	if (ldt.DoModal()==IDOK)
		m_wndOutput.FillText(_T("加载外部数据完成FFT运算..."));
	else
		m_wndOutput.FillText(_T("取消加载外部数据完成FFT运算"));

}
//消息：新建三维瀑布图
void CMainFrame::OnT3dfreqnew()
{
	g_bWaveNew = false;//设置是否创建三维波形瀑布图标识//Marshall  2011.2.24 18:49

	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{
		CT3DFreqDataSet fds;
		if (fds.DoModal()==IDOK)
		{
			//m_wndOutput.FillText(_T("新建三维瀑布图..."));////Marshall  2011.2.24 19:0
			m_wndOutput.FillText(_T("新建三维频谱瀑布图..."));
		}
		else
			//m_wndOutput.FillText(_T("取消新建三维瀑布图"));
			m_wndOutput.FillText(_T("取消新建三维频谱瀑布图"));

		//else
		//	m_wndOutput.FillText(_T("取消新建三维瀑布图"));

		ActivateTabEG(1);
	}

}


//三维波形瀑布图////Marshall  2011.2.24 18:7
/*--------------------------Marshall's Code2011.2.24 18:6--------------*/
void CMainFrame::OnT3dWaveNew()
{
	g_bWaveNew = true;//设置是否创建三维波形瀑布图标识//Marshall  2011.2.24 18:49

	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{
		CT3DFreqDataSet fds;
		//加一个新建三维波形瀑布图的标识//Marshall  2011.2.24 18:11
		fds.m_bWaveNew = true;

		if (fds.DoModal()==IDOK)
		{
			m_wndOutput.FillText(_T("新建三维波形瀑布图..."));
		}
		else
			m_wndOutput.FillText(_T("取消新建三维波形瀑布图"));

		//else
		//	m_wndOutput.FillText(_T("取消新建三维瀑布图"));

		ActivateTabEG(1);
	}

}
/*--------------------------Marshall's Code2011.2.24 18:6--------------*/


//void CMainFrame::OnT3dsaveimage()
//{
//	// TODO: 在此添加命令处理程序代码
//}

//void CMainFrame::OnSavet3dviewbatdlg()
//{
//	if (fileData->fileTitle.size()==0)
//	{
//		AfxMessageBox("尚未加载文件");
//	}
//	else
//	{
//		CSaveT3dViewDlg st3d;
//		st3d.DoModal();
//	}
//
//}

void CMainFrame::OnTrendGraph()
{
	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{
		CTrendSelSet tss;
		tss.SetShowType(0);
		if(tss.DoModal()==IDOK)
		{
			m_wndOutput.FillText(_T("新建趋势分析试验..."));
		}
		else
			m_wndOutput.FillText(_T("取消新建趋势分析试验"));

		ActivateTabEA(3);
	}
}
//void CMainFrame::OnTrendimagesavebat()
//{
//	if (fileData->fileTitle.size()==0)
//	{
//		AfxMessageBox("尚未加载文件");
//	}
//	else
//	{
//		CTrendSelSet tss;
//		tss.SetShowType(1);
//		tss.DoModal();
//	}
//}

//void CMainFrame::OnTrendreport()
//{
//	if (fileData->fileTitle.size()==0)
//	{
//		AfxMessageBox("尚未加载文件");
//	}
//	else
//	{
//		CSaveTrendToExcelDlg stted;
//		stted.DoModal();
//	}
//}

void CMainFrame::OnStripchartgen()
{
	//CInitialConfig ic;
	//ic.DoModal();

	int treeClass = 1,i=0;
	CString m_ParName[2];
	for(i=0;i<2;i++)
	{
		m_ParName[i] = _T("");
	}	
	if (m_wndWorkSpace.m_wndTree)

	{

		//AfxMessageBox("1");
		CInitialConfig ic;

		//m_wndWorkSpace.m_wndTree.SetFocus();  //cxy20231225

		//m_wndWorkSpace.m_wndTree.SetFocus();

 if(m_bFlag)
 {
				HTREEITEM root=m_wndWorkSpace.m_wndTree.GetRootItem();
				//if (!root)return;
				HTREEITEM rFirst=m_wndWorkSpace.m_wndTree.GetChildItem(root);
				//if (!rFirst)return;
				HTREEITEM rFFirst=m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);	
				//if (!rFFirst)return;
				m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);

				
				
		//m_wndWorkSpace.m_wndTree.GetRootItem();//cxy20231225
 }
      

		HTREEITEM   m_selectedItem   =   m_wndWorkSpace.m_wndTree.GetSelectedItem(); 
		if (m_selectedItem)
		{
			//sBD.SaveFileType=2;
			CString m_SelName = m_wndWorkSpace.m_wndTree.GetItemText(m_selectedItem);
			m_ParName[0]=m_SelName;
			HTREEITEM hItem = m_wndWorkSpace.m_wndTree.GetParentItem(m_selectedItem);
			
			if (!hItem)return;
			if (m_wndWorkSpace.m_wndTree.GetRootItem()==m_wndWorkSpace.m_wndTree.GetParentItem(hItem))
			{
				while(hItem!=m_wndWorkSpace.m_wndTree.GetRootItem())
				{
					treeClass++;
					m_ParName[treeClass-1] = m_wndWorkSpace.m_wndTree.GetItemText(hItem);		
					hItem = m_wndWorkSpace.m_wndTree.GetParentItem(hItem);

				}
				CString s=m_ParName[treeClass-1];

				//fileData
				CString ct=fileData->GetCout(s);
				int p=0;
				CString strPath;
				if (fileData->fullPathName.size()==0)return;
				map<int,CString>::iterator fullPath=fileData->fullPathName.begin();
				for (;fullPath!=fileData->fullPathName.end();fullPath++)
				{
					p++;
					if (p==atoi(ct))
					{
						strPath=fullPath->second;			
					}			
				}
				fileData->SetExamTree(strPath);
				ic.examName=fileData->RemoveCoutFromString(m_ParName[1]);
				ic.examType=fileData->RemoveCoutFromString(m_ParName[0]);
				ic.filePath=strPath;
				ic.paramVector=fileData->GetExamParam(strPath,fileData->RemoveCoutFromString(m_ParName[0]));
				ActivateTabEG(0);

				if(ic.DoModal()==IDOK)
				{
					m_wndOutput.FillText(_T("新建波形长图..."));

				}
				else
					m_wndOutput.FillText(_T("取消新建波形长图"));
			}		

		}
		else
		{
			if (fileData->fileTitle.size()==0)
			{
				AfxMessageBox("尚未加载文件");
				OnFileopendata();
			}
			else
			{
				/*AfxMessageBox("请选择正确的试验类型");*/
				m_wndWorkSpace.m_wndTree.SetFocus();
				HTREEITEM root=m_wndWorkSpace.m_wndTree.GetRootItem();
				if (!root)return;
				HTREEITEM rFirst=m_wndWorkSpace.m_wndTree.GetChildItem(root);
				if (!rFirst)return;
				HTREEITEM rFFirst=m_wndWorkSpace.m_wndTree.GetChildItem(rFirst);	
				if (!rFFirst)return;
				m_wndWorkSpace.m_wndTree.SelectItem(rFFirst);

				OnStripchartgen();
			}
		}
	}

}

void CMainFrame::OnDynamicequilibrium()
{
	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{
		CChildFrame* pCFrame = new CChildFrame();
		CCreateContext context;
		context.m_pNewViewClass=RUNTIME_CLASS(CDynamicEquilibrium);
		if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
			return;

		CString titleStrAxis="动平衡试验分析";
		pCFrame->titles=titleStrAxis;
		pCFrame->SetWindowText(titleStrAxis);

		pCFrame->ShowWindow(SW_SHOW);
		pCFrame->InitialUpdateFrame(NULL,true);

		ActivateTabEA(0);
		m_wndOutput.FillText(_T("新建动平衡试验..."));

	}
}

void CMainFrame::OnJiggerexam()
{
	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{
		CChildFrame* pCFrame = new CChildFrame();
		CCreateContext context;
		context.m_pNewViewClass=RUNTIME_CLASS(CJiggerView);
		if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
			return;

		CString titleStrAxis="盘车试验";

		pCFrame->titles=titleStrAxis;
		pCFrame->SetWindowText(titleStrAxis);

		pCFrame->ShowWindow(SW_SHOW);
		pCFrame->InitialUpdateFrame(NULL,true);
		ActivateTabEA(1);
		m_wndOutput.FillText("新建盘车试验分析...");
	}
}
//消息：新建效率分析
void CMainFrame::OnEfficiency()
{
	if (fileData->fileTitle.size()==0)
	{
		AfxMessageBox("尚未加载文件");
		OnFileopendata();
	}
	else
	{
		m_wndOutput.FillText(_T("新建效率分析..."));
		CEfficiency ce;
		ce.DoModal();					

		ActivateTabEA(2);
	}
}
//消息：协联工况试验分析
void CMainFrame::OnCAM()
{
	CString m_FilePathName,m_Title;
	CString strFilter="协联工况文件 (*.ICA) | *.ica;*.ICA ||";
	CFileDialog dlg(TRUE,//open/Save as
		NULL,//Ext added automatically
		NULL,//filename
		OFN_HIDEREADONLY |OFN_FILEMUSTEXIST,
		strFilter);

	if(dlg.DoModal()==IDOK)
	{		
		m_FilePathName=dlg.GetPathName();
		m_Title = m_FilePathName.Mid(m_FilePathName.ReverseFind('\\')+1);
		m_Title = m_Title.Left(m_Title.ReverseFind('.'));
		FILE * f;
		f=fopen(m_FilePathName,"r");
		if(f!=0)
		{	
			m_wndOutput.FillText(_T("协联工况试验分析：加载文件："+m_FilePathName));

			CChildFrame* pCFrame = new CChildFrame();
			CCreateContext context;
			context.m_pNewViewClass=RUNTIME_CLASS(CCamView);
			if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
				return;

			CString titleStrAxis="协联工况";

			//pCFrame->titles=titleStrAxis;
			pCFrame->SetWindowText(titleStrAxis);
			m_Cam->file=f;
			pCFrame->ShowWindow(SW_SHOW);
			pCFrame->InitialUpdateFrame(NULL,true);
			ActivateTabEA(2);
			fclose(f);	
		}
		else
		{
			AfxMessageBox("数据有误，请重新加载！");	
			m_wndOutput.FillText(_T("加载协联工况数据文件有误！"));
			fclose(f);		
		}				
	}
}

void CMainFrame::OnFileTB()
{
	ShowControlBar (&m_wndChannelsBATBarFile,
		!(m_wndChannelsBATBarFile.IsVisible ()),
		FALSE, TRUE);
	RecalcLayout ();

}

void CMainFrame::OnUpdateFileTB(CCmdUI *pCmdUI)
{	
	pCmdUI->SetCheck (m_wndChannelsBATBarFile.IsVisible ());
}

void CMainFrame::OnWavetb()
{
	ShowControlBar (&m_wndTabbedToolBarGrap,
		!(m_wndTabbedToolBarGrap.IsVisible ()),
		FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateWavetb(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (m_wndTabbedToolBarGrap.IsVisible ());
}

void CMainFrame::OnChannelstb()
{
	ShowControlBar (&m_wndTabbedToolBarChanl,
		!(m_wndTabbedToolBarChanl.IsVisible ()),
		FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateChannelstb(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (m_wndTabbedToolBarChanl.IsVisible ());
}

void CMainFrame::OnExamGraptb()
{
	ShowControlBar (&m_wndTabbedToolBarExamGrap,
		!(m_wndTabbedToolBarExamGrap.IsVisible ()),
		FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateExamGraptb(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (m_wndTabbedToolBarExamGrap.IsVisible ());
}

//void CMainFrame::OnT3dtb()
//{
//	//ShowControlBar (&m_wndTabbedToolBarT3d,
//	//	!(m_wndTabbedToolBarT3d.IsVisible ()),
//	//	FALSE, TRUE);
//	RecalcLayout ();
//}
//
//void CMainFrame::OnUpdateT3dtb(CCmdUI *pCmdUI)
//{
//	//pCmdUI->SetCheck (m_wndTabbedToolBarT3d.IsVisible ());
//}
//
//void CMainFrame::OnAxistb()
//{
//	//ShowControlBar (&m_wndTabbedToolBarAxis,
//	//	!(m_wndTabbedToolBarAxis.IsVisible ()),
//	//	FALSE, TRUE);
//	RecalcLayout ();
//}
//
//void CMainFrame::OnUpdateAxistb(CCmdUI *pCmdUI)
//{
//	//pCmdUI->SetCheck (m_wndTabbedToolBarAxis.IsVisible ());
//}

void CMainFrame::OnExamanystb()
{
	ShowControlBar (&m_wndTabbedToolBarExamAnsy,
		!(m_wndTabbedToolBarExamAnsy.IsVisible ()),
		FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateExamanystb(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (m_wndTabbedToolBarExamAnsy.IsVisible ());
}

void CMainFrame::OnExamdata()
{
	ShowControlBar (&m_wndWorkSpace,
		!(m_wndWorkSpace.IsVisible ()),
		FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateExamdata(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (m_wndWorkSpace.IsVisible ());
}

void CMainFrame::OnMsgout()
{
	ShowControlBar (&m_wndOutput,
		!(m_wndOutput.IsVisible ()),
		FALSE, TRUE);
	RecalcLayout();
}

void CMainFrame::OnUpdateMsgout(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBCGPMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	m_wndChannelsBATBarFile.SetWindowText (_T("文件操作"));
	m_wndTabbedToolBarGrap.SetWindowText (_T("图形操作"));
	m_wndTabbedToolBarChanl.SetWindowText (_T("通道数据处理"));
	m_wndTabbedToolBarExamGrap.SetWindowText(_T("试验图谱"));
	m_wndTabbedToolBarExamAnsy.SetWindowText(_T("试验分析"));	
}
//关闭所有窗口
//void CMainFrame::OnCloseallview()
//{
//
//	CFrameWnd* fwnd=GetActiveFrame();
//	CString str=((CChildFrame*)GetActiveFrame())->titles;
//	//CString s=GetActiveFrame()->m_;
//	while (str!="")
//	{
//		/*fwnd=GetActiveFrame();*/
//		((CChildFrame*)GetActiveFrame())->OnClose();
//		str=((CChildFrame*)GetActiveFrame())->titles;
//		
//	}
//	childFV.clear();
//}
//最小二乘法圆度拟合,气息试验
void CMainFrame::OnAirgap()
{
	m_wndOutput.FillText(_T("最小二乘法圆度拟合..."));
	CAirGap cag;
	if(cag.DoModal()==IDCANCEL)
	{
		m_wndOutput.FillText(_T("取消最小二乘法圆度拟合计算"));
	}
		
}
//保存操作日志文件
void CMainFrame::OnSaverecordtotext()
{
	m_wndOutput.FillText(_T("保存操作日志文件..."));
	m_wndOutput.SaveRecord();
}

//查看用户使用手册
void CMainFrame::OnUserhelp()
{

/*
	m_wndOutput.FillText(_T("查看用户使用手册..."));

	if (!PathFileExists(fileData->ReturnCurrentPath()+"\\help\\数据分析工具用户手册.doc"))
	{	
		AfxMessageBox("文档不存在，或路径不正确！");
		m_wndOutput.FillText(_T("用户使用手册文档不存在，或路径不正确！"));
		return;
	}
	else
	{
		CNiWordApplication application;
		application.OpenDocument(fileData->ReturnCurrentPath()+"\\help\\数据分析工具用户手册.doc");
	}
*/
}


//函数OnRefreshApp//Marshall  2011.2.20 19:19
void CMainFrame::OnRefreshApp()
{
	// TODO: 在此添加命令处理程序代码
//	std::vector<CChildFrame*>::iterator iter = childFV.begin();
//	CChildFrame *ptr = NULL;
//	while(iter != childFV.end())
//	{
//		//(CChildFrame*)(*iter)->OnClose();
//		ptr = *iter;
//		//ptr->OnClose();
//		//ptr->MDIActivate();
////		ptr->SendMessage(WM_CLOSE);
//		ptr->DestroyWindow();
//		iter++;
//	}

    m_bFlag=false;

	CChildFrame* pChildWnd = (CChildFrame*)MDIGetActive();
	while(pChildWnd != NULL)
	{
		pChildWnd->DestroyWindow();
		pChildWnd = (CChildFrame*)MDIGetActive();
	}

	childFV.clear();

	FilePathName = _T("");

	m_wndWorkSpace.ClearTree();

	FilePathVector.clear();
	m_wndOutput.FillText(_T("刷新应用程序！"));
	fileData->RemoveAllInfo();
	//fileData = NULL;//不能为空//Marshall  2011.2.20 20:19

	m_PWave=NULL;
	m_P3DFFT=NULL;
	m_Axis=NULL;
	m_TrendView=NULL;
	m_DynEqu=NULL;
	m_Cam=NULL;

	m_wndWorkSpace.m_wndTree.m_lsFullPRAlarm.RemoveAll();//整周期时，有无键相的警告//Marshall  2011.2.26 16:1
}

//OnCloseAll//Marshall  2011.2.20 21:35
void CMainFrame::OnCloseAll()
{

	//std::vector<CChildFrame*>::iterator iter = childFV.begin();//这部分代码只关闭频谱图窗口，没有关闭其它类形的窗口//Marshall  2011.2.27 21:17
	//CChildFrame *ptr = NULL;
	//while(iter != childFV.end())
	//{
	//	//(CChildFrame*)(*iter)->OnClose();
	//	ptr = *iter;
	//	//ptr->OnClose();
	//	//ptr->MDIActivate();
	//	//		ptr->SendMessage(WM_CLOSE);
	//	ptr->DestroyWindow();
	//	iter++;
	//}

	CChildFrame* pChildWnd = (CChildFrame*)MDIGetActive();
	while(pChildWnd != NULL)
	{
		pChildWnd->DestroyWindow();
		pChildWnd = (CChildFrame*)MDIGetActive();
	}


	m_wndWorkSpace.m_wndTree.vectorSelName.clear();
	childFV.clear();

	m_wndOutput.FillText(_T("关闭所有窗口！"));

	m_PWave=NULL;
	m_P3DFFT=NULL;
	m_Axis=NULL;
	m_TrendView=NULL;
	m_DynEqu=NULL;
	m_Cam=NULL;
}

//设置峰峰值置信度，97%//Marshall  2011.2.21 18:9
void CMainFrame::OnNinetySeven()
{
	 g_bConf97 = true;
	 g_bConf95 = false;
	 g_bConf100 = false;

	std::vector<CChildFrame*>::iterator iter = childFV.begin();
	CChildFrame *ptr = NULL;
	while(iter != childFV.end())
	{
		ptr = *iter;
		CNiWaveFraqView* pView = (CNiWaveFraqView*)ptr->GetActiveView();
		if(pView != NULL)
		{
			pView->SetWaveParam();
			pView->Invalidate();
		}

		iter++;
	}
}

//设置峰峰值置信度，95%//Marshall  2011.2.21 18:9
void CMainFrame::OnNinetyFive()
{
	 g_bConf97 = false;
	 g_bConf95 = true;
	 g_bConf100 = false;

	std::vector<CChildFrame*>::iterator iter = childFV.begin();
	CChildFrame *ptr = NULL;
	while(iter != childFV.end())
	{
		ptr = *iter;
		CNiWaveFraqView* pView = (CNiWaveFraqView*)ptr->GetActiveView();
		if(pView != NULL)
		{
			pView->SetWaveParam();
			pView->Invalidate();
		}

		iter++;
	}
}

//设置峰峰值置信度，100%//Marshall  2011.2.21 18:9
void CMainFrame::OnOneHundred()
{
	 g_bConf97 = false;
	 g_bConf95 = false;
	 g_bConf100 = true;

	std::vector<CChildFrame*>::iterator iter = childFV.begin();
	CChildFrame *ptr = NULL;
	while(iter != childFV.end())
	{
		ptr = *iter;
		CNiWaveFraqView* pView = (CNiWaveFraqView*)ptr->GetActiveView();
		if(pView != NULL)
		{
			pView->SetWaveParam();
			pView->Invalidate();
		}

		iter++;
	}
}


/*--------------------------Marshall's Code2011.2.21 18:30--------------*/
void CMainFrame::OnUpdateNinetySeven(CCmdUI *pCmdUI)
{
	if(g_bConf97)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateNinetyFive(CCmdUI *pCmdUI)
{
	if(g_bConf95)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateOneHundred(CCmdUI *pCmdUI)
{
	if(g_bConf100)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}
/*--------------------------Marshall's Code2011.2.21 18:30--------------*/

void CMainFrame::OnDynEquCmp()
{
	CDynEquCmpDlg dlg;
	dlg.DoModal();
}
void CMainFrame::OnUpdateEfficiency(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnCamSaveRequiresView()
{
	AfxMessageBox("请先打开并激活协联工况视图。");
}
