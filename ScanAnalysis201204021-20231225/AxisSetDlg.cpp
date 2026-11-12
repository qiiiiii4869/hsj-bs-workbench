// AxisSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "AxisSetDlg.h"
#include "AxisOrbit.h"
#include "ChildFrm.h"
#include "MainFrm.h"
// CAxisSetDlg 对话框

using namespace std;
IMPLEMENT_DYNAMIC(CAxisSetDlg, CHSJDialog)

CAxisSetDlg::CAxisSetDlg(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CAxisSetDlg::IDD, pParent),FunSel(1)
{

}

CAxisSetDlg::~CAxisSetDlg()
{
}

void CAxisSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAMECMBO, fileNameComb);
	//DDX_Control(pDX, IDC_EXAMNAMECOMBO, examNameComb);
	DDX_Control(pDX, IDC_EXAMTYPECOMBO, examTypeComb);
	DDX_Control(pDX, IDC_EXAMPARAMCOMBO, examParamComb);
	DDX_Control(pDX, IDC_EXAMCHNCOMBOX, chnNameCombX);
	DDX_Control(pDX, IDC_EXAMCHNCOMBOY, chnNameCombY);
}


BEGIN_MESSAGE_MAP(CAxisSetDlg, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CAxisSetDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &CAxisSetDlg::OnCbnSelchangeFilenamecmbo)
	//ON_CBN_SELCHANGE(IDC_EXAMNAMECOMBO, &CAxisSetDlg::OnCbnSelchangeExamnamecombo)
	ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &CAxisSetDlg::OnCbnSelchangeExamtypecombo)
	ON_CBN_SELCHANGE(IDC_EXAMCHNCOMBOY, &CAxisSetDlg::OnCbnSelchangeExamchncomboy)
	ON_CBN_SELCHANGE(IDC_EXAMPARAMCOMBO, &CAxisSetDlg::OnCbnSelchangeExamparamcombo)
	ON_CBN_SELCHANGE(IDC_EXAMCHNCOMBOX, &CAxisSetDlg::OnCbnSelchangeExamchncombox)
END_MESSAGE_MAP()


// CAxisSetDlg 消息处理程序

void CAxisSetDlg::SetFunSel(int sel)
{
	FunSel=sel;
}
void CAxisSetDlg::OnBnClickedOk()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString chnNameX,chnNameY,filePath,exmType,exmPara,chnName;
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filePath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),exmType);
	examParamComb.GetLBText(examParamComb.GetCurSel(),exmPara);
	chnNameCombX.GetLBText(chnNameCombX.GetCurSel(),chnNameX);
	chnNameCombY.GetLBText(chnNameCombY.GetCurSel(),chnNameY);

	int dataLenX=pCMF->fileData->GetChnDataLength(filePath, exmType,exmPara,chnNameX);
	int dataLenY=pCMF->fileData->GetChnDataLength(filePath, exmType,exmPara,chnNameY);
	//if (dataLenX==dataLenY)
	{

		int save_freqx=0;
		int save_freqy=0;


		//int samplFreq=0;
		//map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
		//for (;its!=pCMF->fileData->samFreq.end();its++)
		//{
		//	if (its->first==pCMF->FilePathName)
		//	{
		//		samplFreq=atoi(its->second);
		//	}
		//}



		CString titleStrAxis=exmType+"_"+exmPara+","+"[上X]"+chnNameX+","+"[下Y]"+chnNameY+"轴心轨迹";
		CString titleStrCorl=exmType+"_"+exmPara+","+"[上X]"+chnNameX+","+"[下Y]"+chnNameY+"相关分析";
		//CString titleStrAxis=exmType+"_"+exmPara+","+" "+"轴心轨迹";
		//CString titleStrCorl=exmType+"_"+exmPara+","+" "+"相关分析";
		if (FunSel==1)
		{
			CChildFrame* pCFrame = new CChildFrame();
			CCreateContext context;
			context.m_pNewViewClass=RUNTIME_CLASS(CAxisOrbit);
			if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
				return;
			if (chnNameX==chnNameY)
			{
				AfxMessageBox("请选择不同的X，Y轴信号！");
				return;
			}
			if (dataLenX!=dataLenY)
			{
				AfxMessageBox("通道数据不相同，请交换选择通道！");
				return;
			}
			else
			{	
				fileDataX=pCMF->fileData->GetDataFromFile(filePath, exmType,exmPara,chnNameX,&save_freqx);
				fileDataY=pCMF->fileData->GetDataFromFile(filePath, exmType,exmPara,chnNameY,&save_freqy);

				pCMF->m_Axis->SetDataX(fileDataX,save_freqx,chnNameX+" ("+pCMF->fileData->GetChnUnit(filePath,chnNameX)+")");
				pCMF->m_Axis->SetDataY(fileDataY,save_freqy,chnNameY+" ("+pCMF->fileData->GetChnUnit(filePath,chnNameY)+")");

				pCFrame->titles=titleStrAxis;
				pCFrame->SetWindowText(titleStrAxis);
				pCMF->m_Axis->SetWinTitle(titleStrAxis);
			}
			pCMF->m_Axis->SetFunSel();
			pCFrame->ShowWindow(SW_SHOW);
			pCFrame->InitialUpdateFrame(NULL,true);

		}
		if (FunSel==2)
		{
			CChildFrame* pCFrame = new CChildFrame();
			CCreateContext context;
			context.m_pNewViewClass=RUNTIME_CLASS(CCorrelation);
			if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
				return;


			fileDataX=pCMF->fileData->GetDataFromFile(filePath, exmType,exmPara,chnNameX,&save_freqx);
			fileDataY=pCMF->fileData->GetDataFromFile(filePath, exmType,exmPara,chnNameY,&save_freqy);

			pCMF->corr->SetDataX(fileDataX,save_freqx,chnNameX+" ("+pCMF->fileData->GetChnUnit(filePath,chnNameX)+")");
			pCMF->corr->SetDataY(fileDataY,save_freqy,chnNameY+" ("+pCMF->fileData->GetChnUnit(filePath,chnNameY)+")");
			pCFrame->titles=titleStrCorl;
			pCFrame->SetWindowText(titleStrCorl);
			pCMF->corr->SetWinTitle(titleStrCorl);

			pCMF->corr->SetFunSel();	
			pCFrame->ShowWindow(SW_SHOW);
			pCFrame->InitialUpdateFrame(NULL,true);

		}
		


		OnOK();

	}


}

BOOL CAxisSetDlg::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	if (FunSel==1)
	{
		this->SetWindowText("轴心轨迹通道数据设置");
	} 
	else
	{
		this->SetWindowText("相关分析通道数据设置");
	}

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
		{
			chnNameCombX.InsertString(0,itc[p]);
			chnNameCombY.InsertString(0,itc[p]);
		}

	}
	chnNameCombX.SetCurSel(0);
	chnNameCombY.SetCurSel(0);

	examParamComb.EnableWindow(FALSE);
	examTypeComb.EnableWindow(FALSE);
	chnNameCombX.EnableWindow(FALSE);
	chnNameCombY.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAxisSetDlg::OnCbnSelchangeFilenamecmbo()
{
	CString filepath,examName,examParam;
	//examNameComb.ResetContent();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(filepath);

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
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);
	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator itc=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,itc[p])==1)
		{
			chnNameCombX.InsertString(0,itc[p]);
			chnNameCombY.InsertString(0,itc[p]);
		}

	}
	chnNameCombX.SetCurSel(0);
	chnNameCombY.SetCurSel(0);

}

//void CAxisSetDlg::OnCbnSelchangeExamnamecombo()
//{
//	CString filepath,examName;
//	examTypeComb.ResetContent();
//	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
//	pCMF->fileData->SetExamTree(filepath);
//	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(pCMF->FilePathName);
//	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
//	//examNameComb.GetLBText(examNameComb.GetCurSel(),examName);
//
//	vector<CString> examType= pCMF->fileData->GetExamType(filepath,examName);
//	vector<CString>::iterator it=examType.begin();
//
//	for (;it!=examType.end();it++)
//	{
//		examTypeComb.InsertString(0,*it);
//	}
//	examTypeComb.SetCurSel(0);
//}

void CAxisSetDlg::OnCbnSelchangeExamtypecombo()
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



void CAxisSetDlg::OnCbnSelchangeExamparamcombo()
{
	CString filepath,examParam;
	chnNameCombX.ResetContent();
	chnNameCombY.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(filepath);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator it=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,it[p])==1)
		{
			chnNameCombX.InsertString(0,it[p]);
			chnNameCombY.InsertString(0,it[p]);

		}
	}
	chnNameCombX.SetCurSel(0);
	chnNameCombY.SetCurSel(0);
	chnNameCombX.EnableWindow(TRUE);
	chnNameCombY.EnableWindow(TRUE);
}

void CAxisSetDlg::OnCbnSelchangeExamchncombox()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString chnNameX,chnNameY,filePath,exmType,exmPara,chnName;
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filePath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),exmType);
	examParamComb.GetLBText(examParamComb.GetCurSel(),exmPara);
	chnNameCombX.GetLBText(chnNameCombX.GetCurSel(),chnNameX);
	chnNameCombY.GetLBText(chnNameCombY.GetCurSel(),chnNameY);
	//if (chnNameX==chnNameY)
	//{
	//	AfxMessageBox("请选择不同的X，Y轴信号！");
	//}

	//fileDataX=pCMF->fileData->GetDataFromFile(filePath, exmType,exmPara,chnNameX);
}

void CAxisSetDlg::OnCbnSelchangeExamchncomboy()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString chnNameX,chnNameY,filePath,exmType,exmPara,chnName;
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filePath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),exmType);
	examParamComb.GetLBText(examParamComb.GetCurSel(),exmPara);
	chnNameCombX.GetLBText(chnNameCombX.GetCurSel(),chnNameX);
	chnNameCombY.GetLBText(chnNameCombY.GetCurSel(),chnNameY);
	//if (chnNameX==chnNameY)
	//{
	//	AfxMessageBox("请选择不同的X，Y轴信号！");
	//}

	//fileDataY=pCMF->fileData->GetDataFromFile(filePath, exmType,exmPara,chnNameY);
}