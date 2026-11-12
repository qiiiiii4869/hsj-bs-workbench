// ScanAnalysis.h : main header file for the ScanAnalysis application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"       // main symbols


// CScanAnalysisApp:
// See ScanAnalysis.cpp for the implementation of this class
//

class CScanAnalysisApp : public CWinApp,
							public CBCGPWorkspace
{
public:
	CScanAnalysisApp();

	// Override from CBCGPWorkspace
	//virtual void PreLoadState ();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CScanAnalysisApp theApp;