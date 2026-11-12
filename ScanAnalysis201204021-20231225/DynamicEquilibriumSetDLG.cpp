// DynamicEquilibriumSetDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "DynamicEquilibriumSetDLG.h"
#include "DynamicEquilibrium.h"
#include "MainFrm.h"
#include "ChildFrm.h"
using namespace std;
// CDynamicEquilibriumSetDLG 对话框

IMPLEMENT_DYNAMIC(CDynamicEquilibriumSetDLG, CHSJDialog)
CDynamicEquilibrium* cdeView;
CDynamicEquilibriumSetDLG::CDynamicEquilibriumSetDLG(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CDynamicEquilibriumSetDLG::IDD, pParent),DataType(0)
{
	cdeView=(CDynamicEquilibrium*)pParent;
}

CDynamicEquilibriumSetDLG::~CDynamicEquilibriumSetDLG()
{
}

void CDynamicEquilibriumSetDLG::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAMECMBO, fileNameComb);
	//DDX_Control(pDX, IDC_EXAMNAMECOMBO, examNameComb);
	DDX_Control(pDX, IDC_EXAMTYPECOMBO, examTypeComb);
	DDX_Control(pDX, IDC_EXAMPARAMCOMBO, examParamComb);
	DDX_Control(pDX, IDC_EXAMCHNCOMBOX, chnNameCombChn);
}


BEGIN_MESSAGE_MAP(CDynamicEquilibriumSetDLG, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CDynamicEquilibriumSetDLG::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &CDynamicEquilibriumSetDLG::OnCbnSelchangeFilenamecmbo)
	ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &CDynamicEquilibriumSetDLG::OnCbnSelchangeExamtypecombo)
	ON_CBN_SELCHANGE(IDC_EXAMPARAMCOMBO, &CDynamicEquilibriumSetDLG::OnCbnSelchangeExamparamcombo)
END_MESSAGE_MAP()


// CDynamicEquilibriumSetDLG 消息处理程序

void CDynamicEquilibriumSetDLG::OnBnClickedOk()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString filePath,exmType,exmPara,chnName;
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filePath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),exmType);
	examParamComb.GetLBText(examParamComb.GetCurSel(),exmPara);
	chnNameCombChn.GetLBText(chnNameCombChn.GetCurSel(),chnName);
	
	//int samplFreq;
	//map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
	//for (;its!=pCMF->fileData->samFreq.end();its++)
	//{
	//	if (its->first==pCMF->FilePathName)
	//	{
	//		samplFreq=atoi(its->second);
	//	}
	//}

	if (DataType==0)
	{
		CString titleStrAxis=exmType+"_"+exmPara+","+chnName+"("+pCMF->fileData->GetChnUnit(filePath,chnName)+"),"+" (配重前)";
		int save_freq=0;
		vector<float> vf=pCMF->fileData->GetDataFromFile(filePath,exmType,exmPara,chnName,&save_freq);
		cdeView->SetBBWData(vf,save_freq,titleStrAxis);

		//////////////////////////////////////////////////////////////////////////
		//int cout=chnName.Find('#',0);
		//CString keyPStr=chnName.Left(cout+1)+"SJ90键相";
		//cdeView->SetKeyPhaseB(pCMF->fileData->GetDataFromFile(filePath,exmType,exmPara,keyPStr));
		//cdeView->SetKeyPhaseB(pCMF->fileData->GetKeyPhase(filePath,exmType,exmPara));
		int key_Freq=0;
		vector<float> keyData = pCMF->fileData->GetKeyPhase(filePath,exmType,exmPara,&key_Freq);

		cdeView->SetKeyPhaseB(keyData,key_Freq,save_freq);


		//////////////////////////////////////////////////////////////////////////
	}
	else
	{		
		CString titleStrAxis=exmType+"_"+exmPara+","+chnName+"("+pCMF->fileData->GetChnUnit(filePath,chnName)+"),"+" (配重后)";
		int save_freq=0;
		vector<float> vf=pCMF->fileData->GetDataFromFile(filePath,exmType,exmPara,chnName,&save_freq);

		//if ((int)vf.size()==0)
		//{
		//	return;
		//}
		//else
		cdeView->SetABWData(vf,save_freq,titleStrAxis);
			
		//////////////////////////////////////////////////////////////////////////
		//int cout=chnName.Find('#',0);
		//CString keyPStr=chnName.Left(cout+1)+"SJ90键相";
		//cdeView->SetKeyPhaseA(pCMF->fileData->GetDataFromFile(filePath,exmType,exmPara,keyPStr));
		
		//cdeView->SetKeyPhaseA(pCMF->fileData->GetKeyPhase(filePath,exmType,exmPara));
		int key_Freq=0;
		vector<float> keyData = pCMF->fileData->GetKeyPhase(filePath,exmType,exmPara,&key_Freq);

		cdeView->SetKeyPhaseA(keyData,key_Freq,save_freq);
		//////////////////////////////////////////////////////////////////////////

	}

	OnOK();
}

void CDynamicEquilibriumSetDLG::OnCbnSelchangeFilenamecmbo()
{
	CString filepath,examName;
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(filepath);

	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(filepath);

	examTypeComb.ResetContent();
	pCMF->fileData->SetExamTree(filepath);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	examName=iterE->second;
	vector<CString> examType= pCMF->fileData->GetExamType(filepath,examName);
	vector<CString>::iterator it=examType.begin();

	for (;it!=examType.end();it++)
	{
		examTypeComb.InsertString(0,*it);
	}
	examTypeComb.SetCurSel(0);
	examTypeComb.EnableWindow(TRUE);

	CString examParam;
	//chnNameCombChn.ResetContent();
	//CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//pCMF->fileData->SetExamTree(filepath);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator itch=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,itch[p])==1)
			chnNameCombChn.InsertString(0,itch[p]);
	}
	chnNameCombChn.SetCurSel(0);
	//chnNameCombChn.EnableWindow(TRUE);
}

BOOL CDynamicEquilibriumSetDLG::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	SetWindowText(title);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();


	map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
	for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
	{
		fileNameComb.InsertString(0,fullPath->second);		
	}
	fileNameComb.SetCurSel(0);
	CString filepath;

	pCMF->fileData->SetExamTree(filepath);
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);	
	pCMF->fileData->SetExamTree(filepath);
	/************************************************************************/
	/* 7.2                                                                     */
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

	vector<CString> paramType= pCMF->fileData->GetExamParam(filepath,examStr);
	vector<CString>::iterator itT=paramType.begin();

	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		examParamComb.InsertString(0,itT[p]);
	}
	examParamComb.SetCurSel(0);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator itc=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,itc[p])==1)
			chnNameCombChn.InsertString(0,itc[p]);
	}
	chnNameCombChn.SetCurSel(0);

	examParamComb.EnableWindow(FALSE);
	examTypeComb.EnableWindow(FALSE);
	chnNameCombChn.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDynamicEquilibriumSetDLG::OnCbnSelchangeExamtypecombo()
{
	CString filepath,examType;
	examParamComb.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(filepath);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);

	vector<CString> paramType= pCMF->fileData->GetExamParam(filepath,examType);
	vector<CString>::iterator it=paramType.begin();
	
	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		examParamComb.InsertString(0,it[p]);
	}
	examParamComb.SetCurSel(0);
	examParamComb.EnableWindow(TRUE);
}

void CDynamicEquilibriumSetDLG::OnCbnSelchangeExamparamcombo()
{
	CString filepath,examParam;
	chnNameCombChn.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(filepath);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator it=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
			chnNameCombChn.InsertString(0,it[p]);
	}
	chnNameCombChn.SetCurSel(0);
	chnNameCombChn.EnableWindow(TRUE);
}
