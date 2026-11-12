#pragma once
#include "afxcmn.h"

#include "Ni3DWaveView.h"
// C3DFreqShowCurveDlg 对话框////Marshall  2011.2.24 15:27

struct CurveSet{
	CString strCurveName;
	bool    bVisible;
} ;

class C3DFreqShowCurveDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(C3DFreqShowCurveDlg)

public:
	C3DFreqShowCurveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~C3DFreqShowCurveDlg();

// 对话框数据
	enum { IDD = IDD_SHOWCURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CListCtrl m_lscShowCurve;
	virtual BOOL OnInitDialog();
public:
	CList<CurveSet, CurveSet&> m_listSet;
	CWnd *m_pWnd;
public:
	//afx_msg void OnClickListCurvename(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListCurvename(NMHDR* pNMHDR, LRESULT* pResult);
public:
	afx_msg void OnBnClickedCancel();
};
