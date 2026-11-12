// JiggerExamSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "JiggerExamSet.h"
#include "MainFrm.h"
#include "JiggerView.h"


double oChnData[JIGGERCOUNT];
double nChnData[JIGGERCOUNT];
//#define  mPI 3.1415926535897932384626433832795

CJiggerView* JView;

// CJiggerExamSet 对话框
using namespace std;
IMPLEMENT_DYNAMIC(CJiggerExamSet, CHSJDialog)

CJiggerExamSet::CJiggerExamSet(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CJiggerExamSet::IDD, pParent),savePath("")
{
	JView =(CJiggerView*) pParent;
}

CJiggerExamSet::~CJiggerExamSet()
{
}

void CJiggerExamSet::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAMECMBO, fileNameComb);
	DDX_Control(pDX, IDC_EXAMTYPECOMBO, examTypeComb);

	DDX_Control(pDX, IDC_OEXAMPARAMCOMBO, oExamParam);
	DDX_Control(pDX, IDC_NEXAMPARAMCOMBO, nExamParam);

	DDX_Control(pDX, IDC_OCHN1, oCHN1);
	DDX_Control(pDX, IDC_OCHN2, oCHN2);
	DDX_Control(pDX, IDC_OCHN3, oCHN3);
	DDX_Control(pDX, IDC_OCHN4, oCHN4);
	DDX_Control(pDX, IDC_OCHN5, oCHN5);
	DDX_Control(pDX, IDC_OCHN6, oCHN6);
	DDX_Control(pDX, IDC_OCHN7, oCHN7);
	DDX_Control(pDX, IDC_OCHN8, oCHN8);

	DDX_Control(pDX, IDC_NCHN1, nCHN1);
	DDX_Control(pDX, IDC_NCHN2, nCHN2);
	DDX_Control(pDX, IDC_NCHN3, nCHN3);
	DDX_Control(pDX, IDC_NCHN4, nCHN4);
	DDX_Control(pDX, IDC_NCHN5, nCHN5);
	DDX_Control(pDX, IDC_NCHN6, nCHN6);
	DDX_Control(pDX, IDC_NCHN7, nCHN7);
	DDX_Control(pDX, IDC_NCHN8, nCHN8);

}


BEGIN_MESSAGE_MAP(CJiggerExamSet, CHSJDialog)
	ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &CJiggerExamSet::OnCbnSelchangeFilenamecmbo)
	ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &CJiggerExamSet::OnCbnSelchangeExamtypecombo)
	ON_CBN_SELCHANGE(IDC_OEXAMPARAMCOMBO, &CJiggerExamSet::OnCbnSelchangeOexamparamcombo)
	ON_CBN_SELCHANGE(IDC_OCHN1, &CJiggerExamSet::OnCbnSelchangeOchn1)
	ON_CBN_SELCHANGE(IDC_OCHN2, &CJiggerExamSet::OnCbnSelchangeOchn2)
	ON_CBN_SELCHANGE(IDC_OCHN3, &CJiggerExamSet::OnCbnSelchangeOchn3)
	ON_CBN_SELCHANGE(IDC_OCHN4, &CJiggerExamSet::OnCbnSelchangeOchn4)
	ON_CBN_SELCHANGE(IDC_OCHN5, &CJiggerExamSet::OnCbnSelchangeOchn5)
	ON_CBN_SELCHANGE(IDC_OCHN6, &CJiggerExamSet::OnCbnSelchangeOchn6)
	ON_CBN_SELCHANGE(IDC_OCHN7, &CJiggerExamSet::OnCbnSelchangeOchn7)
	ON_CBN_SELCHANGE(IDC_OCHN8, &CJiggerExamSet::OnCbnSelchangeOchn8)
	ON_CBN_SELCHANGE(IDC_NEXAMPARAMCOMBO, &CJiggerExamSet::OnCbnSelchangeNexamparamcombo)
	ON_CBN_SELCHANGE(IDC_NCHN1, &CJiggerExamSet::OnCbnSelchangeNchn1)
	ON_CBN_SELCHANGE(IDC_NCHN2, &CJiggerExamSet::OnCbnSelchangeNchn2)
	ON_CBN_SELCHANGE(IDC_NCHN3, &CJiggerExamSet::OnCbnSelchangeNchn3)
	ON_CBN_SELCHANGE(IDC_NCHN4, &CJiggerExamSet::OnCbnSelchangeNchn4)
	ON_CBN_SELCHANGE(IDC_NCHN5, &CJiggerExamSet::OnCbnSelchangeNchn5)
	ON_CBN_SELCHANGE(IDC_NCHN6, &CJiggerExamSet::OnCbnSelchangeNchn6)
	ON_CBN_SELCHANGE(IDC_NCHN7, &CJiggerExamSet::OnCbnSelchangeNchn7)
	ON_CBN_SELCHANGE(IDC_NCHN8, &CJiggerExamSet::OnCbnSelchangeNchn8)
	ON_BN_CLICKED(IDC_BUTTON19, &CJiggerExamSet::OnBnClickedButton19)
	ON_BN_CLICKED(IDOK, &CJiggerExamSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CJiggerExamSet 消息处理程序

BOOL CJiggerExamSet::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
	for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
	{
		fileNameComb.InsertString(0,fullPath->second);		
	}
	fileNameComb.SetCurSel(0);
	CString filepath;

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	pCMF->fileData->SetExamTree(filepath);

	/************************************************************************/
	/*      7.2                                                                */
	/************************************************************************/

	CString examName,examStr,examParam;
	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(pCMF->FilePathName);
	examName=iterE->second;
	//examNameComb.InsertString(0,iterE->second);
	//examNameComb.SetCurSel(0);




	//examNameComb.GetLBText(examNameComb.GetCurSel(),examName);

	vector<CString> examType= pCMF->fileData->GetExamType(filepath,examName);
	vector<CString>::iterator it=examType.begin();
	examTypeComb.InsertString(0,*it);
	examTypeComb.SetCurSel(0);

	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examStr);

	examTypeComb.EnableWindow(FALSE);
	oExamParam.EnableWindow(FALSE);
	nExamParam.EnableWindow(FALSE);
	
	oCHN1.EnableWindow(FALSE);
	oCHN2.EnableWindow(FALSE);
	oCHN3.EnableWindow(FALSE);
	oCHN4.EnableWindow(FALSE);
	oCHN5.EnableWindow(FALSE);
	oCHN6.EnableWindow(FALSE);
	oCHN7.EnableWindow(FALSE);
	oCHN8.EnableWindow(FALSE);


	nCHN1.EnableWindow(FALSE);
	nCHN2.EnableWindow(FALSE);
	nCHN3.EnableWindow(FALSE);
	nCHN4.EnableWindow(FALSE);
	nCHN5.EnableWindow(FALSE);
	nCHN6.EnableWindow(FALSE);
	nCHN7.EnableWindow(FALSE);
	nCHN8.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CJiggerExamSet::OnCbnSelchangeFilenamecmbo()
{
	CString filepath,examName;
	//examNameComb.ResetContent();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//pCMF->fileData->SetExamTree(filepath);

	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(filepath);
	//examNameComb.InsertString(0,iterE->second);
	//examNameComb.SetCurSel(0);

	examTypeComb.ResetContent();
	pCMF->fileData->SetExamTree(filepath);
	/*map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(pCMF->FilePathName);*/
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	//examNameComb.GetLBText(examNameComb.GetCurSel(),examName);
	examName=iterE->second;
	vector<CString> examType= pCMF->fileData->GetExamType(filepath,examName);
	vector<CString>::iterator it=examType.begin();

	for (;it!=examType.end();it++)
	{
		examTypeComb.InsertString(0,*it);
	}
	examTypeComb.SetCurSel(0);
	examTypeComb.EnableWindow(TRUE);
}

void CJiggerExamSet::OnCbnSelchangeExamtypecombo()
{
	CString filepath,examType;
	oExamParam.ResetContent();
	nExamParam.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
	pCMF->fileData->SetExamTree(filepath);

	vector<CString> paramType= pCMF->fileData->GetExamParam(filepath,examType);
	vector<CString>::iterator it=paramType.begin();

	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		oExamParam.InsertString(0,it[p]);
		nExamParam.InsertString(0,it[p]);
	}
	oExamParam.SetCurSel(0);
	nExamParam.SetCurSel(0);
	oExamParam.EnableWindow(TRUE);
	nExamParam.EnableWindow(TRUE);
}

void CJiggerExamSet::OnCbnSelchangeOexamparamcombo()
{
	CString filepath,examParam;
	oCHN1.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();


	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);
	pCMF->fileData->SetExamTree(filepath);
	oChnVector= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator it=oChnVector.begin();

	for (int p=(int)oChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
			oCHN1.InsertString(0,it[p]);
	}
	oCHN1.SetCurSel(0);
	oCHN1.EnableWindow(TRUE);
}

void CJiggerExamSet::OnCbnSelchangeOchn1()
{
	CString filepath,examParam;
	oCHN2.ResetContent();
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	vector<CString>::iterator it=oChnVector.begin();

	for (int p=(int)oChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		oCHN2.InsertString(0,it[p]);
	}
	oCHN2.SetCurSel(0);
	oCHN2.EnableWindow(TRUE);

	CString chn;
	oCHN1.GetLBText(oCHN1.GetCurSel(),chn);
	AddchnMean(examParam,chn,1, 0);
}

void CJiggerExamSet::OnCbnSelchangeOchn2()
{
	CString filepath,examParam;
	oCHN3.ResetContent();
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=oChnVector.begin();

	for (int p=(int)oChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		oCHN3.InsertString(0,it[p]);
	}
	oCHN3.SetCurSel(0);
	oCHN3.EnableWindow(TRUE);

	CString chn;
	oCHN2.GetLBText(oCHN2.GetCurSel(),chn);
	AddchnMean(examParam,chn,2, 0);
}

void CJiggerExamSet::OnCbnSelchangeOchn3()
{
	CString filepath,examParam;
	oCHN4.ResetContent();
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=oChnVector.begin();

	for (int p=(int)oChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		oCHN4.InsertString(0,it[p]);
	}
	oCHN4.SetCurSel(0);
	oCHN4.EnableWindow(TRUE);

	CString chn;
	oCHN3.GetLBText(oCHN3.GetCurSel(),chn);
	AddchnMean(examParam,chn,3, 0);
}

void CJiggerExamSet::OnCbnSelchangeOchn4()
{
	CString filepath,examParam;
	oCHN5.ResetContent();
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=oChnVector.begin();

	for (int p=(int)oChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		oCHN5.InsertString(0,it[p]);
	}
	oCHN5.SetCurSel(0);
	oCHN5.EnableWindow(TRUE);

	CString chn;
	oCHN4.GetLBText(oCHN4.GetCurSel(),chn);
	AddchnMean(examParam,chn,4, 0);

}

void CJiggerExamSet::OnCbnSelchangeOchn5()
{
	CString filepath,examParam;
	oCHN6.ResetContent();
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=oChnVector.begin();

	for (int p=(int)oChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		oCHN6.InsertString(0,it[p]);
	}
	oCHN6.SetCurSel(0);
	oCHN6.EnableWindow(TRUE);


	CString chn;
	oCHN5.GetLBText(oCHN5.GetCurSel(),chn);
	AddchnMean(examParam,chn,5, 0);

}

void CJiggerExamSet::OnCbnSelchangeOchn6()
{
	CString filepath,examParam;
	oCHN7.ResetContent();
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=oChnVector.begin();

	for (int p=(int)oChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		oCHN7.InsertString(0,it[p]);
	}
	oCHN7.SetCurSel(0);
	oCHN7.EnableWindow(TRUE);


	CString chn;
	oCHN6.GetLBText(oCHN6.GetCurSel(),chn);
	AddchnMean(examParam,chn,6, 0);

}

void CJiggerExamSet::OnCbnSelchangeOchn7()
{
	CString filepath,examParam;
	oCHN8.ResetContent();
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=oChnVector.begin();

	for (int p=(int)oChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		oCHN8.InsertString(0,it[p]);
	}
	oCHN8.SetCurSel(0);
	oCHN8.EnableWindow(TRUE);


	CString chn;
	oCHN7.GetLBText(oCHN7.GetCurSel(),chn);
	AddchnMean(examParam,chn,7, 0);

}

void CJiggerExamSet::OnCbnSelchangeOchn8()
{
	CString filepath,examParam;
	oExamParam.GetLBText(oExamParam.GetCurSel(),examParam);

	CString chn;
	oCHN8.GetLBText(oCHN8.GetCurSel(),chn);
	AddchnMean(examParam,chn,8, 0);

}

void CJiggerExamSet::OnCbnSelchangeNexamparamcombo()
{
	CString filepath,examParam;
	nCHN1.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(filepath);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);

	nChnVector= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator it=nChnVector.begin();

	for (int p=(int)nChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		nCHN1.InsertString(0,it[p]);
	}
	nCHN1.SetCurSel(0);
	nCHN1.EnableWindow(TRUE);
}

void CJiggerExamSet::OnCbnSelchangeNchn1()
{
	CString filepath,examParam;
	nCHN2.ResetContent();
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=nChnVector.begin();

	for (int p=(int)nChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		nCHN2.InsertString(0,it[p]);
	}
	nCHN2.SetCurSel(0);
	nCHN2.EnableWindow(TRUE);

	CString chn;
	nCHN1.GetLBText(nCHN1.GetCurSel(),chn);
	AddchnMean(examParam,chn,1, 1);
}

void CJiggerExamSet::OnCbnSelchangeNchn2()
{
	CString filepath,examParam;
	nCHN3.ResetContent();
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=nChnVector.begin();

	for (int p=(int)nChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		nCHN3.InsertString(0,it[p]);
	}
	nCHN3.SetCurSel(0);
	nCHN3.EnableWindow(TRUE);

	CString chn;
	nCHN2.GetLBText(nCHN2.GetCurSel(),chn);
	AddchnMean(examParam,chn,2, 1);
}


void CJiggerExamSet::OnCbnSelchangeNchn3()
{
	CString filepath,examParam;
	nCHN4.ResetContent();
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=nChnVector.begin();

	for (int p=(int)nChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		nCHN4.InsertString(0,it[p]);
	}
	nCHN4.SetCurSel(0);
	nCHN4.EnableWindow(TRUE);

	CString chn;
	nCHN3.GetLBText(nCHN3.GetCurSel(),chn);
	AddchnMean(examParam,chn,3, 1);
}


void CJiggerExamSet::OnCbnSelchangeNchn4()
{
	CString filepath,examParam;
	nCHN5.ResetContent();
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=nChnVector.begin();

	for (int p=(int)nChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		nCHN5.InsertString(0,it[p]);
	}
	nCHN5.SetCurSel(0);
	nCHN5.EnableWindow(TRUE);

	CString chn;
	nCHN4.GetLBText(nCHN4.GetCurSel(),chn);
	AddchnMean(examParam,chn,4, 1);
}

void CJiggerExamSet::OnCbnSelchangeNchn5()
{
	CString filepath,examParam;
	nCHN6.ResetContent();
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=nChnVector.begin();

	for (int p=(int)nChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		nCHN6.InsertString(0,it[p]);
	}
	nCHN6.SetCurSel(0);
	nCHN6.EnableWindow(TRUE);

	CString chn;
	nCHN5.GetLBText(nCHN5.GetCurSel(),chn);
	AddchnMean(examParam,chn,5, 1);
}

void CJiggerExamSet::OnCbnSelchangeNchn6()
{
	CString filepath,examParam;
	nCHN7.ResetContent();
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=nChnVector.begin();

	for (int p=(int)nChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		nCHN7.InsertString(0,it[p]);
	}
	nCHN7.SetCurSel(0);
	nCHN7.EnableWindow(TRUE);

	CString chn;
	nCHN6.GetLBText(nCHN6.GetCurSel(),chn);
	AddchnMean(examParam,chn,6, 1);
}

void CJiggerExamSet::OnCbnSelchangeNchn7()
{
	CString filepath,examParam;
	nCHN8.ResetContent();
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	vector<CString>::iterator it=nChnVector.begin();

	for (int p=(int)nChnVector.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		nCHN8.InsertString(0,it[p]);
	}
	nCHN8.SetCurSel(0);
	nCHN8.EnableWindow(TRUE);

	CString chn;
	nCHN7.GetLBText(nCHN7.GetCurSel(),chn);
	AddchnMean(examParam,chn,7, 1);
}

void CJiggerExamSet::OnCbnSelchangeNchn8()
{
	CString filepath,examParam;
	nExamParam.GetLBText(nExamParam.GetCurSel(),examParam);

	CString chn;
	nCHN8.GetLBText(nCHN8.GetCurSel(),chn);
	AddchnMean(examParam,chn,8, 1);
}

void CJiggerExamSet::AddchnMean(CString examParam,CString chnName,int k, int t)
{
	CString filepath,examType;

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	int save_freq=0;
	vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,examParam,chnName,&save_freq);
	
	CNiReal64Vector NiDt((unsigned int)chndata.size());
	vector<float>::iterator itD = chndata.begin();
	int p=-1;
	for(; itD != chndata.end(); itD++)
	{
		p++;
		NiDt[p]=*itD;
	}
	double chnMean;

	CNiMath::Mean(NiDt,chnMean);
	if (t==0)
	{
		oChnData[k-1]=chnMean;
	}
	else
	{
		nChnData[k-1]=chnMean;
	}
}


void CJiggerExamSet::OnBnClickedButton19()
{
	if (!nCHN8.IsWindowEnabled())
	{
		AfxMessageBox("请确认所有通道！");
	}
	else
	{
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

		savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
		if (savePath!="")
		{
			CreateDirectory(savePath+"\\"+"盘车试验",0);
			CString str,path;

			CTime tm;
			tm=CTime::GetCurrentTime();
			
			str=tm.Format("%Y-%m-%d  %H-%M-%S");
			path=savePath+"\\"+"盘车试验"+"\\"+"盘车数据_"+str+".txt";
			SaveDataToText(path,oChnData,nChnData,JIGGERCOUNT);
			//m_savePath.SetWindowText(savePath);	
		}

	}

}

bool CJiggerExamSet::SaveDataToText(CString FilePathName,double * oChnPtr,double * nChnPtr,int len)
{
	if(oChnPtr==0||nChnPtr==0||len<=0) return false;
	int i=0;
	//char tmp[50];
	//printf("\n");
	FILE	*fp = 0;
	fp = fopen(FilePathName,"w");
	if(fp==0)return false;

	for(i=0;i<len;i++)
	{
		fprintf(fp,"%.20lf\n",oChnPtr[i]); 
	}
	for(i=0;i<len;i++)
	{
		fprintf(fp,"%.20lf\n",nChnPtr[i]); 
	}
	fseek(fp,0,SEEK_END);
	fclose(fp);
	return true;
}



void CJiggerExamSet::OnBnClickedOk()
{

	if (!nCHN8.IsWindowEnabled())
	{
		AfxMessageBox("请确认所有通道！");
	}
	else
	{
		CNiReal64Vector o(JIGGERCOUNT);
		CNiReal64Vector n(JIGGERCOUNT);

		for(int i=0;i<JIGGERCOUNT;i++)
		{
			o[i]=oChnData[i]; 
		}
		for(int i=0;i<JIGGERCOUNT;i++)
		{
			n[i]=nChnData[i]; 
		}

		JView->setChnData(o,n);

		OnOK();
	}
}
