// T3DFreqDataSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "T3DFreqDataSet.h"
#include "ChildFrm.h"
#include "Ni3DWaveView.h"
#include "MainFrm.h"
// CT3DFreqDataSet 对话框

extern bool g_bWaveNew;

using namespace std;
IMPLEMENT_DYNAMIC(CT3DFreqDataSet, CHSJDialog)

CT3DFreqDataSet::CT3DFreqDataSet(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CT3DFreqDataSet::IDD, pParent)
{
	m_bWaveNew = false;
//	isHasKeyPhase=FALSE;
}

CT3DFreqDataSet::~CT3DFreqDataSet()
{
}

void CT3DFreqDataSet::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAMECMBO, fileNameComb);
	DDX_Control(pDX, IDC_EXAMTYPECOMBO, examTypeComb);
	DDX_Control(pDX, IDC_EXAMPARAMCOMBO, examParamComb);
	DDX_Control(pDX, IDC_EXAMCHNCOMBOX, chnNameComb);
	DDX_Control(pDX, IDC_COMBO1, chnNameCombAcd);
	DDX_Control(pDX, IDC_CWGRAPH2, m_Graph);
}


BEGIN_MESSAGE_MAP(CT3DFreqDataSet, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CT3DFreqDataSet::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &CT3DFreqDataSet::OnCbnSelchangeFilenamecmbo)
	ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &CT3DFreqDataSet::OnCbnSelchangeExamtypecombo)
	ON_CBN_SELCHANGE(IDC_EXAMPARAMCOMBO, &CT3DFreqDataSet::OnCbnSelchangeExamparamcombo)
	ON_CBN_SELCHANGE(IDC_EXAMCHNCOMBOX, &CT3DFreqDataSet::OnCbnSelchangeExamchncombox)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CT3DFreqDataSet::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CT3DFreqDataSet 消息处理程序


BOOL CT3DFreqDataSet::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	HSJStyleGraph(m_Graph);
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_Graph.SetWindowText("");

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
			chnNameComb.InsertString(0,itc[p]);
		

		}
		chnNameCombAcd.InsertString(0,itc[p]);
	}
	chnNameComb.SetCurSel(0);
	chnNameCombAcd.SetCurSel(0);

	examParamComb.EnableWindow(FALSE);
	examTypeComb.EnableWindow(FALSE);
	chnNameComb.EnableWindow(FALSE);
	chnNameCombAcd.EnableWindow(FALSE);

	OnCbnSelchangeExamchncombox();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//新建三维频谱瀑布图////Marshall  2011.2.27 10:28
void CT3DFreqDataSet::OnBnClickedOk()
{
		/*CString filepath;//原代码不包含创建三维波形瀑布图，注释掉//Marshall  2011.2.24 18:19
		fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
		

		
		CString examType,chnName,chnNameAcd;
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

		examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
		chnNameComb.GetLBText(chnNameComb.GetCurSel(),chnName);
		chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);	

		CString title= examType+"_"+chnName+"("+pCMF->fileData->GetChnUnit(filepath,chnName)+")"+" 三维瀑布图";

		CChildFrame* pCFrame = new CChildFrame();
		CCreateContext context;
		context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
		if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
			return;
		pCFrame->titles=title;
		pCFrame->ShowWindow(SW_SHOW);
		pCMF->m_P3DFFT->TrendCap=chnNameAcd;
		pCMF->m_P3DFFT->unitZ=pCMF->fileData->GetChnUnit(filepath,chnName);
		pCMF->m_P3DFFT->unitX=pCMF->fileData->GetChnUnit(filepath,chnNameAcd);
		pCMF->m_P3DFFT->unitY="Hz";

		GetMultiChnData();
		pCFrame->InitialUpdateFrame(NULL,true);	
		pCMF->m_P3DFFT->SetFFTData(MultiChndata, MultiChndataAcd,samplFreq,title);*/

	if(!m_bWaveNew)
	{
		CString filepath;
		fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
		

		
		CString examType,chnName,chnNameAcd;
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

		examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
		chnNameComb.GetLBText(chnNameComb.GetCurSel(),chnName);
		chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);	

		CString title= examType+"_"+chnName+"("+pCMF->fileData->GetChnUnit(filepath,chnName)+")"+" 三维频谱瀑布图";

		CChildFrame* pCFrame = new CChildFrame();
		CCreateContext context;
		context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
		if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
			return;
		pCFrame->titles=title;
		pCFrame->ShowWindow(SW_SHOW);
		pCMF->m_P3DFFT->TrendCap=chnNameAcd;
		pCMF->m_P3DFFT->unitZ=pCMF->fileData->GetChnUnit(filepath,chnName);
		pCMF->m_P3DFFT->unitX=pCMF->fileData->GetChnUnit(filepath,chnNameAcd);
		pCMF->m_P3DFFT->unitY="Hz";

		GetMultiChnData();

		/*--------------------------Marshall's Code2011.2.27 15:47--------------*/
		pCMF->m_P3DFFT->isHasKeyPhase = isHasKeyPhase;
		if(isHasKeyPhase)
			pCMF->m_P3DFFT->kPhaseX = kPhaseX;
		/*--------------------------Marshall's Code2011.2.27 15:47--------------*/

		pCFrame->InitialUpdateFrame(NULL,true);	
		pCMF->m_P3DFFT->SetFFTData(MultiChndata, MultiChndataAcd,samplFreq,title);
	}
	else
	{
		CString filepath;
		fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
		

		
		CString examType,chnName,chnNameAcd;
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

		examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
		chnNameComb.GetLBText(chnNameComb.GetCurSel(),chnName);
		chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);	

		CString title= examType+"_"+chnName+"("+pCMF->fileData->GetChnUnit(filepath,chnName)+")"+" 三维波形瀑布图";

		CChildFrame* pCFrame = new CChildFrame();
		CCreateContext context;
		context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
		if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
			return;
		pCFrame->titles=title;
		pCFrame->ShowWindow(SW_SHOW);
		pCMF->m_P3DFFT->TrendCap=chnNameAcd;
		pCMF->m_P3DFFT->unitZ=pCMF->fileData->GetChnUnit(filepath,chnName);
		pCMF->m_P3DFFT->unitX=pCMF->fileData->GetChnUnit(filepath,chnNameAcd);
		//pCMF->m_P3DFFT->unitY="Hz";
		pCMF->m_P3DFFT->unitY="s";

		GetMultiChnData();
		pCFrame->InitialUpdateFrame(NULL,true);	
		//pCMF->m_P3DFFT->SetFFTData(MultiChndata, MultiChndataAcd,samplFreq,title);
		pCMF->m_P3DFFT->SetWaveData(MultiChndata, MultiChndataAcd,samplFreq,title);
	}


	OnOK();
}

void CT3DFreqDataSet::OnCbnSelchangeFilenamecmbo()
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

	//fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator itc=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,itc[p])==1)
		{
			chnNameComb.InsertString(0,itc[p]);


		}
		chnNameCombAcd.InsertString(0,itc[p]);
	}
	chnNameComb.SetCurSel(0);
	chnNameCombAcd.SetCurSel(0);
}

void CT3DFreqDataSet::OnCbnSelchangeExamtypecombo()
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

void CT3DFreqDataSet::OnCbnSelchangeExamparamcombo()
{
	CString filepath,examParam;
	chnNameComb.ResetContent();
	chnNameCombAcd.ResetContent();
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
			chnNameComb.InsertString(0,it[p]);
			
		}
		chnNameCombAcd.InsertString(0,it[p]);

	}
	chnNameComb.SetCurSel(0);
	chnNameCombAcd.SetCurSel(0);
	chnNameComb.EnableWindow(TRUE);
	chnNameCombAcd.EnableWindow(TRUE);
}

void CT3DFreqDataSet::OnCbnSelchangeExamchncombox()
{
	CString filepath,examParam,examType,chnName;

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
	chnNameComb.GetLBText(chnNameComb.GetCurSel(),chnName);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	int save_freq=0;
	vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,examParam,chnName,&save_freq);
	
	
	CNiReal64Vector NiDt((unsigned int)chndata.size());
	vector<float>::iterator itD = chndata.begin();
	int k=-1;
	for(; itD != chndata.end(); itD++)
	{
		k++;
		NiDt[k]=*itD;
	}
	
	//CNiColor blue(0,0,255);
	//m_Graph.GetPlots().Item(1).LineColor=blue;
	m_Graph.GetPlots().Item(1).PlotY(NiDt, 0, 1);
	m_Graph.SetWindowText(chnName);
	m_Graph.Axes.Item("XAxis").SetMinMax(0,NiDt.GetSize());

}
BEGIN_EVENTSINK_MAP(CT3DFreqDataSet, CHSJDialog)
	ON_EVENT(CT3DFreqDataSet, IDC_CWGRAPH2, DISPID_DBLCLICK, CT3DFreqDataSet::DblClickCwgraph2, VTS_NONE)
END_EVENTSINK_MAP()


void CT3DFreqDataSet::DblClickCwgraph2()
{
	CString filepath,examParam,examType,chnName;

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
	chnNameComb.GetLBText(chnNameComb.GetCurSel(),chnName);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	int save_freq=0;
	vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,examParam,chnName,&save_freq);

	CNiReal64Vector NiDt((unsigned int)chndata.size());
	vector<float>::iterator itD = chndata.begin();
	int k=-1;
	for(; itD != chndata.end(); itD++)
	{
		k++;
		NiDt[k]=*itD;
	}

	//CNiColor blue(0,0,255);
	//m_Graph.GetPlots().Item(1).LineColor=blue;
	m_Graph.GetPlots().Item(1).PlotY(NiDt, 0, 1);
	m_Graph.SetWindowText(chnName);
	m_Graph.Axes.Item("XAxis").SetMinMax(0,NiDt.GetSize());

}

/*--------------------------Marshall's Code2011.2.27 10:59--------------*/
//设置键相信号
void CT3DFreqDataSet::SetKeyPhase(std::vector<float> kp,
								  int key_freq,
								  int wave_freq,
								  int nDataLen)
{
	//KeyPhase.SetSize((int)kp.size());
	/*vector<float> kPhaseX;*/
	kPhaseX.clear();//如果没有此句，则程序运算结果不正确//Marshall  2011.2.26 14:18

	/////////////////////////////////////
	//int /*key_freq=8192,*/wave_freq=samplingFreq;
	int d_num = key_freq/wave_freq;
	if(key_freq%wave_freq!=0) d_num += 1;
	if(d_num<=0) d_num = 1;
	if ((int)kp.size()==0)
	{
		isHasKeyPhase=FALSE;
	}
	else
	{
		isHasKeyPhase=TRUE;
		for (int k=0;k<(int)kp.size();k++)
		{
			int kpCout=0;
			if (k%key_freq==0)
			{
				kpCout=(int)kp[k];
				for (int i=1;i<=kpCout;i++)
				{
					float x=k+kp[k+i];
					x /= d_num;
//					if(x<NiData.GetSize())
					if(x<nDataLen)
					{
						kPhaseX.push_back(x);
						//kPhaseY.push_back((float)NiData[(int)x]);//此处不需要//Marshall  2011.2.25 19:21
					}
				}			
			}
		}
	}

}
/*--------------------------Marshall's Code2011.2.27 10:59--------------*/
void CT3DFreqDataSet::GetMultiChnData()
{
	CString filepath,examType,chnName,chnNameAcd;
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	//examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
	chnNameComb.GetLBText(chnNameComb.GetCurSel(),chnName);
	chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);


	vector<CString> paramType= pCMF->fileData->GetExamParam(filepath,examType);
	vector<CString>::iterator it=paramType.begin();
	MultiChndata.clear();
	int save_freq=0;
	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,paramType[p],chnName,&save_freq);
		/*--------------------------Marshall's Code2011.2.27 10:47--------------*/
		//默认为整周期。为了方便，在此处获取键相信号数据////Marshall  2011.2.27 10:47
		if(!g_bWaveNew)
		{
			int key_Freq=0;

			vector<float> keyData = pCMF->fileData->GetKeyPhase(
				filepath,
				examType,
				paramType[p],
				&key_Freq);

			int nDataLen = pCMF->fileData->GetChnDataLength(filepath,
				examType,paramType[p],chnName);

			SetKeyPhase(keyData,key_Freq,save_freq,nDataLen);
		}
		/*--------------------------Marshall's Code2011.2.27 10:47--------------*/
		MultiChndata.insert(make_pair(paramType[p],chndata));
	}

	samplFreq=save_freq;

	MultiChndataAcd.clear();
	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,paramType[p],chnNameAcd,&save_freq);
		MultiChndataAcd.insert(make_pair(paramType[p],chndata));
	}

	//map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
	//for (;its!=pCMF->fileData->samFreq.end();its++)
	//{
	//	if (its->first==pCMF->FilePathName)
	//	{
	//		samplFreq=atoi(its->second);
	//	}
	//}
}
void CT3DFreqDataSet::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}
