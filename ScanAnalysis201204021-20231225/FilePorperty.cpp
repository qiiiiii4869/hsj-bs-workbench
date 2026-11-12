// FilePorperty.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "FilePorperty.h"
#include "MainFrm.h"
#include <vector>

using namespace std;

// FilePorperty 对话框



IMPLEMENT_DYNAMIC(FilePorperty, CHSJDialog)

FilePorperty::FilePorperty(CWnd* pParent /*=NULL*/)
	: CHSJDialog(FilePorperty::IDD, pParent)
{

}

FilePorperty::~FilePorperty()
{
}

void FilePorperty::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Path, path);
	DDX_Control(pDX, IDC_COMBO1, fileNameComb);
	DDX_Control(pDX, IDC_STATIC_ExamName, examName);
	DDX_Control(pDX, IDC_STATIC_CTime, cTtime);
	DDX_Control(pDX, IDC_STATIC_MTime, mTime);
	DDX_Control(pDX, IDC_STATIC_ATime, aTime);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Control(pDX, IDC_STATIC_sFreq, sFreq);
	DDX_Control(pDX, IDC_STATIC_tNum, sNum);
	DDX_Control(pDX, IDC_STATIC_sNum, sLen);
	DDX_Control(pDX, IDC_STATIC_cLen, cLen);
}


BEGIN_MESSAGE_MAP(FilePorperty, CHSJDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &FilePorperty::OnCbnSelchangeCombo1)	
END_MESSAGE_MAP()

const int nColumns = 6;
const int nRows = 50;
// FilePorperty 消息处理程序

BOOL FilePorperty::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	//map<CString,CString> iterT=pCMF->fileData->fileTitle;
	//map<CString,CString>::iterator itT=iterT.begin();
	//
	//while(itT!=iterT.end())
	//{
	//	fileNameComb.InsertString(0,itT->second);
	//	itT++;
	//}
	map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
	for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
	{
		fileNameComb.InsertString(0,fullPath->second);		
	}
	fileNameComb.SetCurSel(0);

	map<CString,CString>::iterator iterT=pCMF->fileData->fileTitle.find(pCMF->FilePathName);
	this->SetWindowText("分拣文件："+iterT->first);
	map<CString,CString>::iterator iterF=pCMF->fileData->filePath.find(pCMF->FilePathName);
	path.SetWindowText(iterF->second);
	map<CString,CString>::iterator iterA=pCMF->fileData->strATime.find(pCMF->FilePathName);
	cTtime.SetWindowText(iterA->second);
	map<CString,CString>::iterator iterM=pCMF->fileData->strMTime.find(pCMF->FilePathName);
	mTime.SetWindowText(iterM->second);
	map<CString,CString>::iterator iterC=pCMF->fileData->strCTime.find(pCMF->FilePathName);
	aTime.SetWindowText(iterC->second);
	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(pCMF->FilePathName);
	examName.SetWindowText(iterE->second);
	map<CString,CString>::iterator iterSF=pCMF->fileData->samFreq.find(pCMF->FilePathName);
	sFreq.SetWindowText(iterSF->second);
	map<CString,CString>::iterator iterSN=pCMF->fileData->sortNum.find(pCMF->FilePathName);
	sNum.SetWindowText(iterSN->second);
	map<CString,CString>::iterator iterSL=pCMF->fileData->sortLen.find(pCMF->FilePathName);
	sLen.SetWindowText(iterSL->second);
	map<CString,CString>::iterator iterCL=pCMF->fileData->chnLen.find(pCMF->FilePathName);
	cLen.SetWindowText(iterCL->second);
	//m_wndList.InsertColumn (0, _T("#"), LVCFMT_LEFT, 20);
	//m_wndList.InsertColumn (0, _T("#"), LVCFMT_LEFT, 20);

	//int nColumn = 0;

	//for (nColumn = 1; nColumn < nColumns - 1; nColumn++)
	//{
	//	m_wndList.InsertColumn (nColumn, CString ((TCHAR)(_T('A') + nColumn - 1)), LVCFMT_LEFT, 70);
	//}

	//for (int i = 0; i < nRows; i++)
	//{
	//	CString str;
	//	str.Format ("%d", i);
	//	m_wndList.InsertItem (i, str);
	//	m_wndList.SetItemData (i, i);

	//	for (nColumn = 1; nColumn < nColumns - 1; nColumn++)
	//	{
	//		str.Format ("Item (%d, %d)", nColumn - 1, i);
	//		m_wndList.SetItemText (i, nColumn, str);
	//	}
	//}

	//m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
	//	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndList.InsertColumn (0, _T("序号"), LVCFMT_LEFT, 20);
	m_wndList.InsertColumn (1, _T("试验名"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (2, _T("类型名"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (3, _T("参数名"), LVCFMT_LEFT, 50);
	m_wndList.InsertColumn (4, _T("起始时间"), LVCFMT_LEFT, 150);
	m_wndList.InsertColumn (5, _T("终止时间"), LVCFMT_LEFT, 150);
	m_wndList.InsertColumn (6, _T("通道描述"), LVCFMT_LEFT, 100);
	m_wndList.InsertColumn (7, _T("起始点数"), LVCFMT_LEFT, 80);
	m_wndList.InsertColumn (8, _T("终止点数"), LVCFMT_LEFT, 80);
	//m_wndList.InsertColumn (9, _T("分拣起始点数"), LVCFMT_LEFT, 100);
	//m_wndList.InsertColumn (10, _T("分拣终止点数"), LVCFMT_LEFT, 100);
	//CString filename;
	//fileNameComb.GetLBText(fileNameComb.GetCurSel(),filename);

	//CString filepath=pCMF->fileData->filePath[filename];
	//filepath+=filename+".SEL";

	map<CString,vector<SortInfo>>::iterator iterS=pCMF->fileData->sortPorperty.find(pCMF->FilePathName);

	if (iterS!=pCMF->fileData->sortPorperty.end())
	{	
		vector<SortInfo>::iterator itS=iterS->second.begin();	
		//while(itS!=iterS->second.end())
		for (int p=(int)iterS->second.size()-1;p>=0;p--)
		{
			SortInfo sf=itS[p];
			m_wndList.InsertItem (0, sf.sortId);
			m_wndList.SetItemText (0, 1, sf.m_ExamName);
			m_wndList.SetItemText (0, 2, sf.m_ExamType);
			m_wndList.SetItemText (0, 3, sf.m_ExamTypePara);
			m_wndList.SetItemText (0, 4, sf.m_SortTimeBeginArr);
			m_wndList.SetItemText (0, 5, sf.m_SortTimeEndArr);
			m_wndList.SetItemText (0, 6, sf.m_SortBaseName);
			m_wndList.SetItemText (0, 7, sf.m_SortStartPoint);
			m_wndList.SetItemText (0, 8, sf.m_SortEndPoint);
			//m_wndList.SetItemText (0, 9, sf.m_ActionStartPoint);
			//m_wndList.SetItemText (0, 10, sf.m_ActionEndPoint);

		}

	}

	//m_wndList.InsertItem (0, _T("123"));
	//m_wndList.SetItemText (0, 1, _T("100"));

	//m_wndList.SetFont (&m_Font);
	//map<CString,vector<CString>>::iterator iterT=pCMF->fileData->examType.find((CString)pCMF->fileData->GetFileName()[0]);
	//
	//if (iterT!=pCMF->fileData->examType.end())
	//{	
	//	vector<CString>::iterator itT=iterT->second.begin();	
	//	while(itT!=iterT->second.end())
	//		examTypeComb.InsertString(0,*itT++);
	//}
	//examTypeComb.SetCurSel(0);

	//CString strtype;
	//examTypeComb.GetLBText(0,strtype);
	//map<CString,vector<CString>>::iterator iterP=pCMF->fileData->examParam.find(strtype);

	//if (iterP!=pCMF->fileData->examParam.end())
	//{	
	//	vector<CString>::iterator itP=iterP->second.begin();	
	//	while(itP!=iterP->second.end())
	//		examparamsComb.InsertString(0,*itP++);
	//}
	//examparamsComb.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void FilePorperty::OnCbnSelchangeCombo1()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	CString filepath;
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	//CString filepath=pCMF->fileData->filePath[filename];
	//filepath+=filename+".SEL";

	m_wndList.DeleteAllItems();
	map<CString,CString>::iterator iterT=pCMF->fileData->fileTitle.find(filepath);
	this->SetWindowText("分拣文件："+iterT->first);
	map<CString,CString>::iterator iterF=pCMF->fileData->filePath.find(filepath);
	path.SetWindowText(iterF->second);
	map<CString,CString>::iterator iterA=pCMF->fileData->strATime.find(filepath);
	cTtime.SetWindowText(iterA->second);
	map<CString,CString>::iterator iterM=pCMF->fileData->strMTime.find(filepath);
	mTime.SetWindowText(iterM->second);
	map<CString,CString>::iterator iterC=pCMF->fileData->strCTime.find(filepath);
	aTime.SetWindowText(iterC->second);
	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(filepath);
	examName.SetWindowText(iterE->second);
	map<CString,CString>::iterator iterSF=pCMF->fileData->samFreq.find(filepath);
	sFreq.SetWindowText(iterSF->second);
	map<CString,CString>::iterator iterSN=pCMF->fileData->sortNum.find(filepath);
	sNum.SetWindowText(iterSN->second);
	map<CString,CString>::iterator iterSL=pCMF->fileData->sortLen.find(filepath);
	sLen.SetWindowText(iterSL->second);
	map<CString,CString>::iterator iterCL=pCMF->fileData->chnLen.find(filepath);
	cLen.SetWindowText(iterCL->second);

	map<CString,vector<SortInfo>>::iterator iterS=pCMF->fileData->sortPorperty.find(filepath);

	if (iterS!=pCMF->fileData->sortPorperty.end())
	{	
		vector<SortInfo>::iterator itS=iterS->second.begin();	
		//while(itS!=iterS->second.end())
		for (int p=(int)iterS->second.size()-1;p>=0;p--)
		{
			SortInfo sf=itS[p];
			m_wndList.InsertItem (0, sf.sortId);
			m_wndList.SetItemText (0, 1, sf.m_ExamName);
			m_wndList.SetItemText (0, 2, sf.m_ExamType);
			m_wndList.SetItemText (0, 3, sf.m_ExamTypePara);
			m_wndList.SetItemText (0, 4, sf.m_SortTimeBeginArr);
			m_wndList.SetItemText (0, 5, sf.m_SortTimeEndArr);
			m_wndList.SetItemText (0, 6, sf.m_SortBaseName);
			m_wndList.SetItemText (0, 7, sf.m_SortStartPoint);
			m_wndList.SetItemText (0, 8, sf.m_SortEndPoint);
			//m_wndList.SetItemText (0, 9, sf.m_ActionStartPoint);
			//m_wndList.SetItemText (0, 10, sf.m_ActionEndPoint);

		}

	}

	/*map<CString,vector<CString>>::iterator iterT=pCMF->fileData->examType.find((CString)pCMF->fileData->GetFileName()[fileNameComb.GetCurSel()]);
	if (iterT!=pCMF->fileData->examType.end())
	{	

		vector<CString>::iterator itT=iterT->second.begin();	
		while(itT!=iterT->second.end())
			examTypeComb.InsertString(0,*itT++);
	}
	examTypeComb.SetCurSel(0);

	CString strtype;
	examTypeComb.GetLBText(0,strtype);
	map<CString,vector<CString>>::iterator iterP=pCMF->fileData->examParam.find(strtype);

	if (iterP!=pCMF->fileData->examParam.end())
	{	
		vector<CString>::iterator itP=iterP->second.begin();	
		while(itP!=iterP->second.end())
			examparamsComb.InsertString(0,*itP++);
	}
	examparamsComb.SetCurSel(0);*/
}



