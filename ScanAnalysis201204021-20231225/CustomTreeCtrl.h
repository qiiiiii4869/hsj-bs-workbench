#pragma once
#include "afxcmn.h"
#include "ScanAnalysisDoc.h"
#include <vector>

class CCustomTreeCtrl :
	public CTreeCtrl
{
public:

	CCustomTreeCtrl(void);
public:
	~CCustomTreeCtrl(void);
public:
	CScanAnalysisDoc* pDoc;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSkinCustomDraw(NMHDR* header, LRESULT* result);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	std::vector<CString> vectorSelName;		//所选择的通道名
	std::vector<CString> vectorTreeName;	//存储所有通道名
	//int cout;								//
	//int xMin;								//
	//int xMax;								//
	//CString title;						//
	//std::vector<float> GetDataFromFile(CString parName0,CString parName1,CString selName);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	/*--------------------------Marshall's Code2011.2.25 17:58--------------*/
	//std::map<CString,int> m_mapFullPRAlarm;//单击通道时，若无键相信号，用于判断是否显示警告当前数据无键相信号//Marshall  2011.2.25 18:0
	//set<CString> m_setFullPRAlarm;
	CStringList m_lsFullPRAlarm;
	
	/*--------------------------Marshall's Code2011.2.25 17:58--------------*/
	BOOL ShowFullPRAlarm(const CString& strFileName);//是否显示警告：当前数据无键相信号//Marshall  2011.2.25 18:7
};
