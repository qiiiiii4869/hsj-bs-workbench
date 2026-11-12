// TrendSelSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "TrendSelSet.h"
#include "TrendView.h"
#include "ChildFrm.h"
#include "MainFrm.h"
// CTrendSelSet 对话框

using namespace std;
IMPLEMENT_DYNAMIC(CTrendSelSet, CHSJDialog)

CTrendSelSet::CTrendSelSet(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CTrendSelSet::IDD, pParent),sel1(1),sel2(1),sel3(1),sel4(1),showType(0),m_fileType(3)
{
	color1=RGB (255, 0, 0);
	color2=RGB (0, 255, 0);
	color3=RGB (0, 0, 255);
	color4=RGB (255, 0, 255);
}

CTrendSelSet::~CTrendSelSet()
{
}

void CTrendSelSet::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAMECMBO, fileNameComb);
	DDX_Control(pDX, IDC_EXAMTYPECOMBO, examTypeComb);
	DDX_Control(pDX, IDC_EXAMPARAMCOMBO, examParamComb);
	DDX_Control(pDX, IDC_EXAMCHNCOMBOX, chnNameComb);
	DDX_Control(pDX, IDC_EXAMCHNCOMBOY, chnNameCombAcd);

	DDX_Control(pDX, IDC_COLOR_PICKER1, m_ColorPicker1);
	DDX_Control(pDX, IDC_COLOR_PICKER2, m_ColorPicker2);
	DDX_Control(pDX, IDC_COLOR_PICKER3, m_ColorPicker3);
	DDX_Control(pDX, IDC_COLOR_PICKER4, m_ColorPicker4);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK4, m_check4);

	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
	DDX_Control(pDX, IDC_RADIO3, m_Radio3);
	DDX_Control(pDX, IDC_RADIO4, m_Radio4);
	DDX_Control(pDX, IDC_RADIO5,m_Radio5);

	DDX_Control(pDX, IDC_EDIT1, m_savePath);
	DDX_Control(pDX, IDC_MSG, m_Msg);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_CANCEL);
	DDX_Control(pDX, IDC_STATIC_Sign, m_signSatic);
	DDX_Control(pDX, IDC_BUTTON1, m_btn);
	DDX_Control(pDX, IDC_STATIC_Path, m_pathStatic);
}


BEGIN_MESSAGE_MAP(CTrendSelSet, CHSJDialog)
	ON_BN_CLICKED(IDC_COLOR_PICKER1, &CTrendSelSet::OnBnClickedColorPicker1)
	ON_BN_CLICKED(IDC_COLOR_PICKER2, &CTrendSelSet::OnBnClickedColorPicker2)
	ON_BN_CLICKED(IDC_COLOR_PICKER3, &CTrendSelSet::OnBnClickedColorPicker3)
	ON_BN_CLICKED(IDC_COLOR_PICKER4, &CTrendSelSet::OnBnClickedColorPicker4)
	ON_BN_CLICKED(IDOK, &CTrendSelSet::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &CTrendSelSet::OnCbnSelchangeFilenamecmbo)
	ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &CTrendSelSet::OnCbnSelchangeExamtypecombo)
	ON_CBN_SELCHANGE(IDC_EXAMPARAMCOMBO, &CTrendSelSet::OnCbnSelchangeExamparamcombo)
	ON_BN_CLICKED(IDC_CHECK1, &CTrendSelSet::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CTrendSelSet::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CTrendSelSet::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CTrendSelSet::OnBnClickedCheck4)

	ON_BN_CLICKED(IDC_RADIO1, &CTrendSelSet::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTrendSelSet::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CTrendSelSet::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CTrendSelSet::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CTrendSelSet::OnBnClickedRadio5)

	ON_BN_CLICKED(IDC_BUTTON1, &CTrendSelSet::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTrendSelSet 消息处理程序

void CTrendSelSet::OnBnClickedColorPicker1()
{
	color1 = m_ColorPicker1.GetColor ();

	//UpdateData (FALSE);
}

void CTrendSelSet::OnBnClickedColorPicker2()
{
	color2 = m_ColorPicker2.GetColor ();

	//UpdateData (FALSE);
}

void CTrendSelSet::OnBnClickedColorPicker3()
{
	color3 = m_ColorPicker3.GetColor ();

	//UpdateData (FALSE);
}

void CTrendSelSet::OnBnClickedColorPicker4()
{
	color4 = m_ColorPicker4.GetColor ();

	//UpdateData (FALSE);
}

void CTrendSelSet::OnBnClickedOk()
{	
	TrendGraph tg1;
	tg1.isSel=sel1;
	tg1.colors=color1;
	graphVector.push_back(tg1);

	TrendGraph tg2;
	tg2.isSel=sel2;
	tg2.colors=color2;
	graphVector.push_back(tg2);

	TrendGraph tg3;
	tg3.isSel=sel3;
	tg3.colors=color3;
	graphVector.push_back(tg3);

	TrendGraph tg4;
	tg4.isSel=sel4;
	tg4.colors=color4;
	graphVector.push_back(tg4);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	CString examType,chnName,chnNameAcd;
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
	chnNameComb.GetLBText(chnNameComb.GetCurSel(),chnName);
	chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);

	CString filepath;
	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);

	if (showType==0)
	{
		CString title= examType+"_"+chnName+"("+pCMF->fileData->GetChnUnit(filepath,chnName)+")_"+chnNameAcd+"_趋势分析图";

		CChildFrame* pCFrame = new CChildFrame();
		CCreateContext context;
		context.m_pNewViewClass=RUNTIME_CLASS(CTrendView);
		if(!pCFrame->LoadFrame(IDI_EXAM_C,WM_MDIDESTROY |WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
			return;
		pCFrame->titles=title;
		/*pCFrame->ShowWindow(SW_SHOW);*/

		GetChnData();	


		pCMF->m_TrendView->SetTrendData(MultiChndata, MultiChndataAcd,graphVector,title);	
		pCFrame->InitialUpdateFrame(NULL,TRUE);
	
		//pCFrame->ActivateFrame(-1);
		OnOK();
	}
	if (showType==1)
	{		
		
		CString editString;
		m_savePath.GetWindowText(editString);
		if (editString=="")
		{
			AfxMessageBox("请选择路径");
		}
		else
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：批量存储趋势图...");
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
			CreateDirectory(savePath+"\\"+examName+"_"+examType,0);//savePath+"\\"+examName+"_"+examType+"\\"+"趋势_"+chnNameAcd+"(参考通道)"+
			CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析",0);
			
			int pos=0;
			m_progress.SetPos(0);
			CString coutStr;
			coutStr.Format("%d",(int)chnName.size());
			for (;it!=chnName.end();it++)
			{
				pos++;
				CString title= examType+"_"+*it+"("+pCMF->fileData->GetChnUnit(filepath,*it)+")_"+chnNameAcd+"_趋势分析图";
				CChildFrame* pCFrame = new CChildFrame();			
				CCreateContext context;
				context.m_pNewViewClass=RUNTIME_CLASS(CTrendView);
				if(!pCFrame->LoadFrame(IDI_EXAM_C,SW_SHOWMAXIMIZED,AfxGetApp()->m_pMainWnd,&context))
					return;

				pCFrame->titles=title;
				//pos++;
				/*pCFrame->SetWindowPos(NULL,0,0,0,0,0);*/


				 
				

				GetMultiChnData(*it);
					
//pCMF->ShowWindow(TRUE);

				pCMF->m_TrendView->SetTrendData(MultiChndata, MultiChndataAcd,graphVector,title);	

				
				 /*pCFrame->ShowWindow(SW_SHOW); */
                
				pCFrame->InitialUpdateFrame(NULL,true);/*		*/
				pCMF->m_TrendView->SaveAllView(savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析"+"\\"+title,m_fileType);/**/
				/*pCFrame->Invalidate(TRUE);	*/
				/*pCMF->m_PWave->FFTProc();*/
  
				pCFrame->DestroyWindow();
				m_progress.SetPos (100*pos/(int)chnName.size());
				CString posStr;
				posStr.Format("%d",pos);
				m_Msg.SetWindowText("存储通道（"+posStr+"/"+coutStr+")： "+title);
			}			
			
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：完成批量存储，路径为 "+savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析"+"\\");

			OnOK();
		}

	}

}


void CTrendSelSet::SetShowType(int s)
{
	showType=s;
}

void CTrendSelSet::OnCbnSelchangeFilenamecmbo()
{
	CString filepath,examName,examParam;
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

BOOL CTrendSelSet::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	if (showType==1)
	{			
		SetWindowText("趋势图形批量存储");
	}
	m_check1.SetCheck(TRUE);
	m_check2.SetCheck(TRUE);
	m_check3.SetCheck(TRUE);
	m_check4.SetCheck(TRUE);
	m_ColorPicker1.EnableAutomaticButton (_T("缺省颜色"), RGB (255, 0, 0));
	m_ColorPicker1.EnableOtherButton (_T("其它"));
	m_ColorPicker1.SetColor ((COLORREF)-1);
	m_ColorPicker1.SetColumnsNumber (10);

	m_ColorPicker2.EnableAutomaticButton (_T("缺省颜色"), RGB (0, 255, 0));
	m_ColorPicker2.EnableOtherButton (_T("其它"));
	m_ColorPicker2.SetColor ((COLORREF)-1);
	m_ColorPicker2.SetColumnsNumber (10);

	m_ColorPicker3.EnableAutomaticButton (_T("缺省颜色"), RGB (0, 0, 255));
	m_ColorPicker3.EnableOtherButton (_T("其它"));
	m_ColorPicker3.SetColor ((COLORREF)-1);
	m_ColorPicker3.SetColumnsNumber (10);

	m_ColorPicker4.EnableAutomaticButton (_T("缺省颜色"), RGB (255, 0, 255));
	m_ColorPicker4.EnableOtherButton (_T("其它"));
	m_ColorPicker4.SetColor ((COLORREF)-1);
	m_ColorPicker4.SetColumnsNumber (10);

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


	RECT rc;
	GetWindowRect(&rc);
	if (showType==0)
	{
		MoveWindow(rc.left,rc.top,rc.right-rc.left-145,rc.bottom-rc.top-50,TRUE);		
		m_pathStatic.ShowWindow(FALSE);
		m_btn.ShowWindow(FALSE);
		m_savePath.ShowWindow(FALSE);

		m_OK.MoveWindow(80,290,60,25,TRUE);
		m_CANCEL.MoveWindow(180,290,60,25,TRUE);
	}
	else
	{
		m_pathStatic.ShowWindow(TRUE);
		m_btn.ShowWindow(TRUE);
		m_savePath.ShowWindow(TRUE);
		m_signSatic.ShowWindow(FALSE);
		chnNameComb.ShowWindow(FALSE);

		MoveWindow(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,TRUE);

		m_Radio1.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BMP)));   

		m_Radio2.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_JPG)));  

		m_Radio3.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_GIF)));  

		m_Radio4.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_PNG)));  

		m_Radio5.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TIFF)));  

		CheckRadioButton(IDC_RADIO1,IDC_RADIO5,IDC_RADIO3);

	}
	m_progress.ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTrendSelSet::OnCbnSelchangeExamtypecombo()
{
	CString filepath,examType;
	examParamComb.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//pCMF->fileData->SetExamTree(filepath);

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

void CTrendSelSet::OnCbnSelchangeExamparamcombo()
{
	CString filepath,examParam;
	chnNameComb.ResetContent();
	chnNameCombAcd.ResetContent();
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//pCMF->fileData->SetExamTree(filepath);

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

void CTrendSelSet::OnBnClickedCheck1()
{
	if (m_check1.GetCheck()==0)
		sel1=0;
	else
		sel1=1;
}

void CTrendSelSet::OnBnClickedCheck2()
{
	if (m_check2.GetCheck()==0)
		sel2=0;
	else
		sel2=1;
}

void CTrendSelSet::OnBnClickedCheck3()
{
	if (m_check3.GetCheck()==0)
		sel3=0;
	else
		sel3=1;
}

void CTrendSelSet::OnBnClickedCheck4()
{
	if (m_check4.GetCheck()==0)
		sel4=0;
	else
		sel4=1;
}

void CTrendSelSet::GetChnData()
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
		MultiChndata.insert(make_pair(paramType[p],chndata));
	}

	MultiChndataAcd.clear();
	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,paramType[p],chnNameAcd,&save_freq);
		MultiChndataAcd.insert(make_pair(paramType[p],chndata));
	}

}

void CTrendSelSet::GetMultiChnData(CString chn)
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
		MultiChndata.insert(make_pair(paramType[p],chndata));
	}

	MultiChndataAcd.clear();
	for (int p=(int)paramType.size()-1;p>=0;p--)
	{
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,paramType[p],chnNameAcd,&save_freq);
		MultiChndataAcd.insert(make_pair(paramType[p],chndata));
	}
}


void CTrendSelSet::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=1;
}

void CTrendSelSet::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=2;
}

void CTrendSelSet::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=3;
}

void CTrendSelSet::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=4;
}

void CTrendSelSet::OnBnClickedRadio5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fileType=5;
}

void CTrendSelSet::OnBnClickedButton1()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	m_savePath.SetWindowText(savePath);
}

