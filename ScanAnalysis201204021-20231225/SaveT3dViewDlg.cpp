// SaveT3dViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "SaveT3dViewDlg.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "Ni3DWaveView.h"
// CSaveT3dViewDlg 对话框

extern bool g_bWaveNew;

using namespace std;
IMPLEMENT_DYNAMIC(CSaveT3dViewDlg, CHSJDialog)

CSaveT3dViewDlg::CSaveT3dViewDlg(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CSaveT3dViewDlg::IDD, pParent),m_fileType(3),xMax(100),xMin(0),yMin(0),yMax(512),zMax(100),zMin(-10),isCAxis(FALSE)
{

}

CSaveT3dViewDlg::~CSaveT3dViewDlg()
{
}

void CSaveT3dViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_savePath);
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
	DDX_Control(pDX, IDC_RADIO3, m_Radio3);
	DDX_Control(pDX, IDC_RADIO4, m_Radio4);
	DDX_Control(pDX, IDC_RADIO5,m_Radio5);

	DDX_Control(pDX, IDC_RADIO6,m_Radio6);
	DDX_Control(pDX, IDC_RADIO7,m_Radio7);
	DDX_Control(pDX, IDC_FILENAMECMBO, fileNameComb);
	DDX_Control(pDX, IDC_EXAMTYPECOMBO, examTypeComb);
	DDX_Control(pDX, IDC_EXAMPARAMCOMBO, examParamComb);
	DDX_Control(pDX, IDC_COMBO1, chnNameCombAcd);

	DDX_Control(pDX, IDC_MSG, m_Msg);
	DDX_Control(pDX, IDC_XMinEDIT, m_xMin);
	DDX_Control(pDX, IDC_XMaxEDIT, m_xMax);
	DDX_Text(pDX, IDC_XMinEDIT, xMin);
	DDX_Text(pDX, IDC_XMaxEDIT, xMax);
	DDX_Control(pDX, IDC_YMinEDIT, m_yMin);
	DDX_Control(pDX, IDC_YMaxEDIT, m_yMax);
	DDX_Text(pDX, IDC_YMinEDIT, yMin);
	DDX_Text(pDX, IDC_YMaxEDIT, yMax);
	//DDX_Control(pDX, IDC_ZMinEDIT, m_zMin);
	//DDX_Control(pDX, IDC_ZMaxEDIT, m_zMax);
	//DDX_Text(pDX, IDC_ZMinEDIT, zMin);
	//DDX_Text(pDX, IDC_ZMaxEDIT, zMax);
	DDV_MinMaxInt(pDX, xMin, -10000, 10000);
	DDV_MinMaxInt(pDX, xMax, -10000, 10000);
	DDV_MinMaxInt(pDX, yMin, -10000, 10000);
	DDV_MinMaxInt(pDX, yMax, -10000, 10000);
	//DDV_MinMaxInt(pDX, zMin, -10000, 10000);
	//DDV_MinMaxInt(pDX, zMax, -10000, 10000);


	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_CANCEL);

	
}


BEGIN_MESSAGE_MAP(CSaveT3dViewDlg, CHSJDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSaveT3dViewDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &CSaveT3dViewDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSaveT3dViewDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CSaveT3dViewDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CSaveT3dViewDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CSaveT3dViewDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDOK, &CSaveT3dViewDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &CSaveT3dViewDlg::OnCbnSelchangeFilenamecmbo)
	ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &CSaveT3dViewDlg::OnCbnSelchangeExamtypecombo)
	ON_CBN_SELCHANGE(IDC_EXAMPARAMCOMBO, &CSaveT3dViewDlg::OnCbnSelchangeExamparamcombo)
	
	ON_BN_CLICKED(IDC_RADIO6, &CSaveT3dViewDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CSaveT3dViewDlg::OnBnClickedRadio7)
END_MESSAGE_MAP()


// CSaveT3dViewDlg 消息处理程序

void CSaveT3dViewDlg::OnBnClickedButton1()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	m_savePath.SetWindowText(savePath);
}

BOOL CSaveT3dViewDlg::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	m_progress.ShowWindow(FALSE);
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_Radio1.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BMP)));
	m_Radio2.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_JPG)));  
	m_Radio3.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_GIF)));
	m_Radio4.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_PNG)));
	m_Radio5.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TIFF)));  

	CheckRadioButton(IDC_RADIO1,IDC_RADIO5,IDC_RADIO3);
	CheckRadioButton(IDC_RADIO6,IDC_RADIO7,IDC_RADIO6);

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
		chnNameCombAcd.InsertString(0,itc[p]);
	}
	chnNameCombAcd.SetCurSel(0);

	examParamComb.EnableWindow(FALSE);
	examTypeComb.EnableWindow(FALSE);
	chnNameCombAcd.EnableWindow(FALSE);

	m_xMin.EnableWindow(FALSE);
	m_xMax.EnableWindow(FALSE);
	m_yMin.EnableWindow(FALSE);
	m_yMax.EnableWindow(FALSE);
	//m_zMin.EnableWindow(FALSE);
	//m_zMax.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSaveT3dViewDlg::OnBnClickedRadio1()
{
	m_fileType=1;
}

void CSaveT3dViewDlg::OnBnClickedRadio2()
{
	m_fileType=2;
}

void CSaveT3dViewDlg::OnBnClickedRadio3()
{
	m_fileType=3;
}

void CSaveT3dViewDlg::OnBnClickedRadio4()
{
	m_fileType=4;
}

void CSaveT3dViewDlg::OnBnClickedRadio5()
{
	m_fileType=5;
}


/*--------------------------Marshall's Code2011.2.27 15:43--------------*/
//设置键相信号
void CSaveT3dViewDlg::SetKeyPhase(std::vector<float> kp,
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
/*--------------------------Marshall's Code2011.2.27 15:43--------------*/

/*--------------------------Marshall's Code2011.2.27 15:51--------------*/
void CSaveT3dViewDlg::SaveAll3DWaveView()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString xminStr,xmaxStr,yminStr,ymaxStr,zminStr,zmaxStr;
	m_xMin.GetWindowText(xminStr);
	m_xMax.GetWindowText(xmaxStr);
	m_yMin.GetWindowText(yminStr);
	m_yMax.GetWindowText(ymaxStr);
	//m_zMin.GetWindowText(zminStr);
	//m_zMax.GetWindowText(zmaxStr);
	//if(xminStr==""|| yminStr=="" || zminStr=="" || xmaxStr=="" || ymaxStr=="" || zmaxStr=="")
	if(xminStr==""|| yminStr=="" || xmaxStr=="" || ymaxStr=="" )
	{
		//AfxMessageBox("请输入坐标参数");
		return;
	}

	CString editString;
	m_savePath.GetWindowText(editString);
	if (editString=="")
	{
		AfxMessageBox("请选择路径");
		return;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维波形瀑布图操作：批量存储视图到图像...");

		m_OK.ShowWindow(FALSE);
		m_CANCEL.ShowWindow(FALSE);
		m_progress.ShowWindow(TRUE);

		CString filepath,examParam,examName;

		fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
		examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

		examName=pCMF->fileData->GetExamName(filepath);

		vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
		vector<CString>::iterator it=chnName.begin();	

		CString examType;
		examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
		CreateDirectory(savePath+"\\"+examName+"_"+examType,0);
		CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"三维波形瀑布图",0);

		CString chnNameAcd;
		chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);	

		m_progress.SetPos(0);
		int pos=0;
		CString coutStr;
		coutStr.Format("%d",(int)chnName.size());
		for (;it!=chnName.end();it++)
		{
			if((int)pCMF->fileData->GetChnFftFlag(filepath,*it)==1)
			{
				CString title= examType+"_"+*it+" 三维波形瀑布图";

				CChildFrame* pCFrame = new CChildFrame();
				pos++;
				CCreateContext context;
				context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
				if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MINIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
					return;
				pCFrame->SetWindowPos(NULL,0,0,0,0,0);
				pCFrame->titles=title;

				//CChildFrame* pCFrame = new CChildFrame();	
				//CCreateContext context;
				//context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
				//if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
				//	return;

				//pCFrame->ShowWindow(SW_SHOW);
		

				pCMF->m_P3DFFT->unitZ=pCMF->fileData->GetChnUnit(filepath,*it);
				pCMF->m_P3DFFT->unitX=pCMF->fileData->GetChnUnit(filepath,chnNameAcd);
				pCMF->m_P3DFFT->unitY="s";
				GetMultiChnData(*it);

				pCFrame->InitialUpdateFrame(NULL,true);


				/*--------------------------Marshall's Code2011.2.27 15:47--------------*/
			/*	pCMF->m_P3DFFT->isHasKeyPhase = isHasKeyPhase;
				if(isHasKeyPhase)
					pCMF->m_P3DFFT->kPhaseX = kPhaseX;*/
				/*--------------------------Marshall's Code2011.2.27 15:47--------------*/

				//pCMF->m_P3DFFT->SetFFTData(MultiChndata, MultiChndataAcd,samplFreq,title);			
				pCMF->m_P3DFFT->SetWaveData(MultiChndata, MultiChndataAcd,samplFreq,title);

				pCMF->m_P3DFFT->SaveAllView(savePath+"\\"+examName+"_"+examType+"\\"+"三维波形瀑布图"+"\\"+title,m_fileType,xMin,xMax,yMin,yMax,isCAxis);/*zMin,zMax,*/

				
				/*pCMF->m_PWave->FFTProc();*/
 
				pCFrame->DestroyWindow();
				m_progress.SetPos (100*pos/(int)chnName.size());
				CString posStr;
				posStr.Format("%d",pos);
				m_Msg.SetWindowText("存储通道（"+posStr+"/"+coutStr+")： "+title);

			}

		}
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：完成保存，路径为"+savePath+"\\"+examName+"_"+examType+"\\"+"三维瀑布图"+"\\");

		OnOK();
	}
}

void CSaveT3dViewDlg::SaveAll3DFreqView()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString xminStr,xmaxStr,yminStr,ymaxStr,zminStr,zmaxStr;
	m_xMin.GetWindowText(xminStr);
	m_xMax.GetWindowText(xmaxStr);
	m_yMin.GetWindowText(yminStr);
	m_yMax.GetWindowText(ymaxStr);
	//m_zMin.GetWindowText(zminStr);
	//m_zMax.GetWindowText(zmaxStr);
	//if(xminStr==""|| yminStr=="" || zminStr=="" || xmaxStr=="" || ymaxStr=="" || zmaxStr=="")
	if(xminStr==""|| yminStr=="" || xmaxStr=="" || ymaxStr=="" )
	{
		//AfxMessageBox("请输入坐标参数");
		return;
	}

	CString editString;
	m_savePath.GetWindowText(editString);
	if (editString=="")
	{
		AfxMessageBox("请选择路径");
		return;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：批量存储视图到图像...");

		m_OK.ShowWindow(FALSE);
		m_CANCEL.ShowWindow(FALSE);
		m_progress.ShowWindow(TRUE);

		CString filepath,examParam,examName;

		fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
		examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

		examName=pCMF->fileData->GetExamName(filepath);

		vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
		vector<CString>::iterator it=chnName.begin();	

		CString examType;
		examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
		CreateDirectory(savePath+"\\"+examName+"_"+examType,0);
		CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"三维瀑布图",0);

		CString chnNameAcd;
		chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);	

		m_progress.SetPos(0);
		int pos=0;
		CString coutStr;
		coutStr.Format("%d",(int)chnName.size());
		for (;it!=chnName.end();it++)
		{
			if((int)pCMF->fileData->GetChnFftFlag(filepath,*it)==1)
			{
				CString title= examType+"_"+*it+" 三维瀑布图";

				CChildFrame* pCFrame = new CChildFrame();
				pos++;
				CCreateContext context;
				context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
				
				if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MINIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
					return;
				pCFrame->SetWindowPos(NULL,0,0,0,0,0);
				pCFrame->titles=title;


				pCMF->m_P3DFFT->TrendCap = chnNameAcd;//_T("趋势");

				//CChildFrame* pCFrame = new CChildFrame();	
				//CCreateContext context;
				//context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
				//if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
				//	return;

				//pCFrame->ShowWindow(SW_SHOW);
		

				pCMF->m_P3DFFT->unitZ=pCMF->fileData->GetChnUnit(filepath,*it);
				pCMF->m_P3DFFT->unitX=pCMF->fileData->GetChnUnit(filepath,chnNameAcd);
				pCMF->m_P3DFFT->unitY="Hz";
				GetMultiChnData(*it);
				pCFrame->InitialUpdateFrame(NULL,true);


				/*--------------------------Marshall's Code2011.2.27 15:47--------------*/
				pCMF->m_P3DFFT->isHasKeyPhase = isHasKeyPhase;
				if(isHasKeyPhase)
					pCMF->m_P3DFFT->kPhaseX = kPhaseX;
				/*--------------------------Marshall's Code2011.2.27 15:47--------------*/

				pCMF->m_P3DFFT->SetFFTData(MultiChndata, MultiChndataAcd,samplFreq,title);			

				pCMF->m_P3DFFT->SaveAllView(savePath+"\\"+examName+"_"+examType+"\\"+"三维瀑布图"+"\\"+title,m_fileType,xMin,xMax,yMin,yMax,isCAxis);/*zMin,zMax,*/

				
				/*pCMF->m_PWave->FFTProc();*/
 
				pCFrame->DestroyWindow();
				m_progress.SetPos (100*pos/(int)chnName.size());
				CString posStr;
				posStr.Format("%d",pos);
				m_Msg.SetWindowText("存储通道（"+posStr+"/"+coutStr+")： "+title);

			}

		}
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：完成保存，路径为"+savePath+"\\"+examName+"_"+examType+"\\"+"三维瀑布图"+"\\");

		OnOK();
	}
}
/*--------------------------Marshall's Code2011.2.27 15:51--------------*/
void CSaveT3dViewDlg::OnBnClickedOk()
{
	UpdateData(TRUE);


	/*--------------------------Marshall's Code2011.2.27 16:48--------------*/
	if(g_bWaveNew)
		SaveAll3DWaveView();
	else
		SaveAll3DFreqView();
	/*--------------------------Marshall's Code2011.2.27 16:48--------------*/

	//CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	//CString xminStr,xmaxStr,yminStr,ymaxStr,zminStr,zmaxStr;
	//m_xMin.GetWindowText(xminStr);
	//m_xMax.GetWindowText(xmaxStr);
	//m_yMin.GetWindowText(yminStr);
	//m_yMax.GetWindowText(ymaxStr);
	////m_zMin.GetWindowText(zminStr);
	////m_zMax.GetWindowText(zmaxStr);
	////if(xminStr==""|| yminStr=="" || zminStr=="" || xmaxStr=="" || ymaxStr=="" || zmaxStr=="")
	//if(xminStr==""|| yminStr=="" || xmaxStr=="" || ymaxStr=="" )
	//{
	//	//AfxMessageBox("请输入坐标参数");
	//	return;
	//}

	//CString editString;
	//m_savePath.GetWindowText(editString);
	//if (editString=="")
	//{
	//	AfxMessageBox("请选择路径");
	//	return;
	//}
	//else
	//{
	//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：批量存储视图到图像...");

	//	m_OK.ShowWindow(FALSE);
	//	m_CANCEL.ShowWindow(FALSE);
	//	m_progress.ShowWindow(TRUE);

	//	CString filepath,examParam,examName;

	//	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	//	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

	//	examName=pCMF->fileData->GetExamName(filepath);

	//	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	//	vector<CString>::iterator it=chnName.begin();	

	//	CString examType;
	//	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
	//	CreateDirectory(savePath+"\\"+examName+"_"+examType,0);
	//	CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"三维瀑布图",0);

	//	CString chnNameAcd;
	//	chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);	

	//	m_progress.SetPos(0);
	//	int pos=0;
	//	CString coutStr;
	//	coutStr.Format("%d",(int)chnName.size());
	//	for (;it!=chnName.end();it++)
	//	{
	//		if((int)pCMF->fileData->GetChnFftFlag(filepath,*it)==1)
	//		{
	//			CString title= examType+"_"+*it+" 三维瀑布图";

	//			CChildFrame* pCFrame = new CChildFrame();
	//			pos++;
	//			CCreateContext context;
	//			context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
	//			if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MINIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
	//				return;
	//			pCFrame->SetWindowPos(NULL,0,0,0,0,0);
	//			pCFrame->titles=title;

	//			//CChildFrame* pCFrame = new CChildFrame();	
	//			//CCreateContext context;
	//			//context.m_pNewViewClass=RUNTIME_CLASS(CNi3DWaveView);
	//			//if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
	//			//	return;

	//			//pCFrame->ShowWindow(SW_SHOW);
	//	

	//			pCMF->m_P3DFFT->unitZ=pCMF->fileData->GetChnUnit(filepath,*it);
	//			pCMF->m_P3DFFT->unitX=pCMF->fileData->GetChnUnit(filepath,chnNameAcd);
	//			pCMF->m_P3DFFT->unitY="Hz";
	//			GetMultiChnData(*it);
	//			pCFrame->InitialUpdateFrame(NULL,true);


	//			/*--------------------------Marshall's Code2011.2.27 15:47--------------*/
	//			pCMF->m_P3DFFT->isHasKeyPhase = isHasKeyPhase;
	//			if(isHasKeyPhase)
	//				pCMF->m_P3DFFT->kPhaseX = kPhaseX;
	//			/*--------------------------Marshall's Code2011.2.27 15:47--------------*/

	//			pCMF->m_P3DFFT->SetFFTData(MultiChndata, MultiChndataAcd,samplFreq,title);			

	//			pCMF->m_P3DFFT->SaveAllView(savePath+"\\"+examName+"_"+examType+"\\"+"三维瀑布图"+"\\"+title,m_fileType,xMin,xMax,yMin,yMax,isCAxis);/*zMin,zMax,*/

	//			
	//			/*pCMF->m_PWave->FFTProc();*/
 //
	//			pCFrame->DestroyWindow();
	//			m_progress.SetPos (100*pos/(int)chnName.size());
	//			CString posStr;
	//			posStr.Format("%d",pos);
	//			m_Msg.SetWindowText("存储通道（"+posStr+"/"+coutStr+")： "+title);

	//		}

	//	}
	//	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("三维瀑布图操作：完成保存，路径为"+savePath+"\\"+examName+"_"+examType+"\\"+"三维瀑布图"+"\\");

	//	OnOK();
	//}

}

void CSaveT3dViewDlg::OnCbnSelchangeFilenamecmbo()
{
	CString filepath,examName;
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

}

void CSaveT3dViewDlg::OnCbnSelchangeExamtypecombo()
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

void CSaveT3dViewDlg::OnCbnSelchangeExamparamcombo()
{
	CString filepath,examParam;
	chnNameCombAcd.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(filepath);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	vector<CString>::iterator it=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		chnNameCombAcd.InsertString(0,it[p]);
	}
	chnNameCombAcd.SetCurSel(0);
	chnNameCombAcd.EnableWindow(TRUE);
	//m_yMax.EnableWindow(TRUE);
	//m_yMin.EnableWindow(TRUE);
	//m_zMax.EnableWindow(TRUE);
	//m_zMin.EnableWindow(TRUE);
}


void CSaveT3dViewDlg::GetMultiChnData(CString chn)
{
	CString filepath,examType,chnName,chnNameAcd;
	chnName=chn;

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);

	chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);	

	vector<CString> paramType= pCMF->fileData->GetExamParam(filepath,examType);
	vector<CString>::iterator it=paramType.begin();
	MultiChndata.clear();
	int save_freq=0;
	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,paramType[p],chnName,&save_freq);
		
		/*--------------------------Marshall's Code2011.2.27 15:45--------------*/
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
		/*--------------------------Marshall's Code2011.2.27 15:45--------------*/
		MultiChndata.insert(make_pair(paramType[p],chndata));
	}

	MultiChndataAcd.clear();
	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,paramType[p],chnNameAcd,&save_freq);
		MultiChndataAcd.insert(make_pair(paramType[p],chndata));
	}

	map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
	for (;its!=pCMF->fileData->samFreq.end();its++)
	{
		if (its->first==pCMF->FilePathName)
		{
			samplFreq=atoi(its->second);
		}
	}
}


void CSaveT3dViewDlg::OnBnClickedRadio6()
{
	isCAxis=FALSE;
	m_xMin.EnableWindow(FALSE);
	m_xMax.EnableWindow(FALSE);
	m_yMin.EnableWindow(FALSE);
	m_yMax.EnableWindow(FALSE);
	//m_zMin.EnableWindow(FALSE);
	//m_zMax.EnableWindow(FALSE);
}

void CSaveT3dViewDlg::OnBnClickedRadio7()
{
	isCAxis=TRUE;
	m_xMin.EnableWindow(TRUE);
	m_xMax.EnableWindow(TRUE);
	m_yMin.EnableWindow(TRUE);
	m_yMax.EnableWindow(TRUE);
	//m_zMin.EnableWindow(TRUE);
	//m_zMax.EnableWindow(TRUE);

}
