// ScanAnalysisDoc.h : interface of the CScanAnalysisDoc class
//


#pragma once


class CScanAnalysisDoc : public CDocument
{
protected: // create from serialization only

	DECLARE_DYNCREATE(CScanAnalysisDoc)

// Attributes
public:
	CScanAnalysisDoc();
// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CScanAnalysisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


