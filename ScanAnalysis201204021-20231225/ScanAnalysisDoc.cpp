// ScanAnalysisDoc.cpp : implementation of the CScanAnalysisDoc class
//

#include "stdafx.h"
#include "ScanAnalysis.h"

#include "ScanAnalysisDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScanAnalysisDoc

IMPLEMENT_DYNCREATE(CScanAnalysisDoc, CDocument)

BEGIN_MESSAGE_MAP(CScanAnalysisDoc, CDocument)
END_MESSAGE_MAP()


// CScanAnalysisDoc construction/destruction

CScanAnalysisDoc::CScanAnalysisDoc()
{
	// TODO: add one-time construction code here

}

CScanAnalysisDoc::~CScanAnalysisDoc()
{
}

BOOL CScanAnalysisDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CScanAnalysisDoc serialization

void CScanAnalysisDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CScanAnalysisDoc diagnostics

#ifdef _DEBUG
void CScanAnalysisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScanAnalysisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CScanAnalysisDoc commands
