// SaveImageBatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "SaveImageBatDlg.h"
#include "MainFrm.h"
#include "ChildFrm.h"
using namespace std;


IMPLEMENT_DYNAMIC(SaveImageBatDlg, CHSJDialog)

SaveImageBatDlg::SaveImageBatDlg(CWnd* pParent /*=NULL*/)
	: CHSJDialog(SaveImageBatDlg::IDD, pParent),m_fileType(3),m_bShowHeader(TRUE),examParam()/*,isSelFile(FALSE)*/,
											examName(""),sortFilePath(""),filePath(""),examType(""),configPath(""),m_Bconfig(FALSE),m_index(0),m_temp(0),m_con(false)
{
	for (int i=0;i<100;i++)
	{
		minTem[i]=10000.0;
		maxTem[i]=-10000.0;
		

	}
	m_k=0;
	isGraph=true;

}

SaveImageBatDlg::~SaveImageBatDlg()
{
}

void SaveImageBatDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_FILENAMECMBO, fileNameComb);
	//DDX_Control(pDX, IDC_EXAMTYPECOMBO, examTypeComb);
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
	DDX_Control(pDX, IDC_RADIO3, m_Radio3);
	DDX_Control(pDX, IDC_RADIO4, m_Radio4);
	DDX_Control(pDX, IDC_RADIO5,m_Radio5);
	DDX_Control(pDX, IDC_EDIT1, m_savePath);
	DDX_Control(pDX, IDC_EDIT10, m_configPath);
	DDX_Control(pDX, IDC_LIST2, m_chnList);
	DDX_Control(pDX, IDC_STATIC_MSG, m_msg);
	DDX_Control(pDX, IDC_STATIC_EXAMTYPE, m_examType);
	//DDX_Control(pDX, IDC_EDIT_Min, m_EditMin);
	//DDX_Control(pDX, IDC_EDIT_Max, m_EditMax);
	DDX_Control(pDX, IDC_EDIT_Min2, m_EditMin2);
	DDX_Control(pDX, IDC_EDIT_Max2, m_EditMax2);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_CHECK1, m_graphChk);
}


BEGIN_MESSAGE_MAP(SaveImageBatDlg, CHSJDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &SaveImageBatDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &SaveImageBatDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &SaveImageBatDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &SaveImageBatDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &SaveImageBatDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &SaveImageBatDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDOK, &SaveImageBatDlg::OnOk)
	//ON_BN_CLICKED(IDC_BUTTON2, &SaveImageBatDlg::OnBnClickedButton2)
//	ON_LBN_DBLCLK(IDC_LIST2, &SaveImageBatDlg::OnLbnDblclkList2)
//ON_LBN_SELCANCEL(IDC_LIST2, &SaveImageBatDlg::OnLbnSelcancelList2)
ON_LBN_SELCHANGE(IDC_LIST2, &SaveImageBatDlg::OnLbnSelchangeList2)
ON_BN_CLICKED(IDC_ChgRng, &SaveImageBatDlg::OnBnClickedChgrng)
//ON_BN_CLICKED(IDC_BUTTON2, &SaveImageBatDlg::OnBnClickedButton2)
//ON_BN_CLICKED(IDCANCEL, &SaveImageBatDlg::OnBnClickedCancel)
//ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &SaveImageBatDlg::OnCbnSelchangeFilenamecmbo)
//ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &SaveImageBatDlg::OnCbnSelchangeExamtypecombo)
ON_BN_CLICKED(IDCANCEL, &SaveImageBatDlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BUTTON2, &SaveImageBatDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_ChgRng2, &SaveImageBatDlg::OnBnClickedChgrng2)
ON_BN_CLICKED(IDC_CHECK1, &SaveImageBatDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// SaveImageBatDlg 消息处理程序

void SaveImageBatDlg::OnBnClickedButton1()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	m_savePath.SetWindowText(savePath);

}

void SaveImageBatDlg::OnBnClickedRadio1()
{
	m_fileType=1;
}

void SaveImageBatDlg::OnBnClickedRadio2()
{
	m_fileType=2;
}

void SaveImageBatDlg::OnBnClickedRadio3()
{
	m_fileType=3;
}

void SaveImageBatDlg::OnBnClickedRadio4()
{
	m_fileType=4;
}

void SaveImageBatDlg::OnBnClickedRadio5()
{
	m_fileType=5;
}

BOOL SaveImageBatDlg::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_progress.ShowWindow(FALSE);
	m_Radio1.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BMP)));
	m_Radio2.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_JPG)));  
	m_Radio3.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_GIF)));
	m_Radio4.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_PNG)));
	m_Radio5.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TIFF)));  

	CheckRadioButton(IDC_RADIO1,IDC_RADIO5,IDC_RADIO3);
	m_examType.SetWindowText("类型： "+examType);


	m_graphChk.SetCheck(true);


	//CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();


	//map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
	//for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
	//{
	//	fileNameComb.InsertString(0,fullPath->second);		
	//}
	//fileNameComb.SetCurSel(0);
	//CString filepath;

	//pCMF->fileData->SetExamTree(filepath);
	//fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);	
	//pCMF->fileData->SetExamTree(filepath);

	////CString examName,examStr,examParam;
	//map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(pCMF->FilePathName);
	//examName=iterE->second;
	//vector<CString> examTypeV= pCMF->fileData->GetExamType(filepath,examName);
	//vector<CString>::iterator it=examTypeV.begin();
	//examTypeComb.InsertString(0,*it);
	//examTypeComb.SetCurSel(0);
	//examTypeComb.EnableWindow(FALSE);

	FillData();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void SaveImageBatDlg::FillData()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	int      iItemIndex;
	HRESULT  hr;

	//hr = m_chnList.AddCategory( examParam );
	//hr = m_chnList.AddCategoryItem( examParam,"aaa" );
	vector<CString>::iterator exn=examParam.begin();
	CString temp;
	CString str;
	for (;exn!=examParam.end();exn++)
	{	
		hr = m_chnList.AddCategory( *exn );
		map<CString,map<CString,BOOL>>::iterator chn=cHNnameBool.begin();		

		for (;chn!=cHNnameBool.end();chn++)
		{		
			temp=chn->first;
			str=temp.Left(temp.Find('#',0));
			if (*exn==str)
			{	
				map<CString,BOOL>::iterator ch=chn->second.begin();

				for (;ch!=chn->second.end();ch++)
				{
					m_chnList.AddCategoryItem( str,ch->first);	
				}			

			}
		}

		//m_chnList.AddCategoryItem( examParam,chn->first);		
	}

	//CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();	
	LPCTSTR  pCategory;
	int      iState;
	int      iCatIndex;
	/*   int      iItemIndex;*/
	m_chnList.ShowCategoryItemStates(TRUE);
	for (iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++) 
	{

		pCategory = m_chnList.GetCategoryName( iCatIndex, false );
		CString min,max;
		CString min2,max2;
		//max.Format("%d",pCMF->fileData->GetParaDataLength(filePath,examType,pCategory));
		//min.Format("%d",0);

		map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
		for (;its!=pCMF->fileData->samFreq.end();its++)
		{
			if (its->first==pCMF->FilePathName)
			{
				max2.Format("%d",atoi(its->second)/2);
			}
		}

		min2.Format("%d",0);
		xWaveMin.insert(make_pair(pCategory,min));
		xWaveMax.insert(make_pair(pCategory,max));
		xWaveMin2.insert(make_pair(pCategory,min2));
		xWaveMax2.insert(make_pair(pCategory,max2));

		for (iItemIndex = 0; iItemIndex < m_chnList.GetCategoryItemCount( pCategory ); iItemIndex++) 
		{
			iState = m_chnList.GetCategoryItemState( pCategory, iItemIndex );
			m_chnList.SetCategoryItemState( pCategory, iItemIndex, 1 );
		}
	}

	for (iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++) {
		pCategory = m_chnList.GetCategoryName( iCatIndex, false );
		m_chnList.SetCategoryState( pCategory, FALSE );
	}

	m_chnList.SetCurSel( 0 );	


	map<CString,map<CString,BOOL>>::iterator chn=cHNnameBool.begin();
	map<CString,BOOL>::iterator ch=chn->second.begin();
	//LPCTSTR  pCategory;
	//int      iState;
	//int      iCatIndex;
	//int      iItemIndex;

	for (iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++) {
		pCategory = m_chnList.GetCategoryName( iCatIndex, false );
		for (iItemIndex = 0; iItemIndex < m_chnList.GetCategoryItemCount( pCategory ); iItemIndex++) {
			iState = m_chnList.GetCategoryItemState( pCategory, iItemIndex );

			if (chn!=cHNnameBool.end())
			{
				ch=chn->second.begin();
				chn++;					
			}		
			ch->second=iState;

		}
	}

	chn=cHNnameBool.begin();
	int/* min=0,max,*/min2=0,max2;
	CString minstr,maxstr,minstr2,maxstr2;


	CString examParamStr = m_chnList.GetCategoryName( 0, false );
	/*max=pCMF->fileData->GetParaDataLength(filePath,examType,examParamStr);*/	

	map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
	for (;its!=pCMF->fileData->samFreq.end();its++)
	{
		if (its->first==pCMF->FilePathName)
		{
			max2=atoi(its->second)/2;
		}
	}
	//minstr.Format("%d",min);
	//maxstr.Format("%d",max);

	//m_EditMin.SetWindowText(minstr);
	//m_EditMax.SetWindowText(maxstr);

	minstr2.Format("%d",min2);
	maxstr2.Format("%d",max2);

	m_EditMin2.SetWindowText(minstr2);
	m_EditMax2.SetWindowText(maxstr2);
}
void SaveImageBatDlg::OnOk()
{

	CString editString;
	m_savePath.GetWindowText(editString);
	if (editString=="")
	{
		AfxMessageBox("请选择路径");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：批量存储为图像文件...");

		m_ok.ShowWindow(FALSE);
		m_cancel.ShowWindow(FALSE);
		m_progress.ShowWindow(TRUE);
		

		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		map<CString,map<CString,BOOL>>::iterator chn=cHNnameBool.begin();
		map<CString,BOOL>::iterator ch=chn->second.begin();


		//map<double,double>::iterator itemTime=timeSpan.begin();
	
		LPCTSTR  pCategory;
		int      iState;
		int      iCatIndex;
		int      iItemIndex;
		CNiReal64Vector minTime(3000),maxTime(3000);	
		int k=0;

		for (iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++)


		{
			

			//m_index++;   //cxy
			

			pCategory = m_chnList.GetCategoryName( iCatIndex, false );
			for (iItemIndex = 0; iItemIndex < m_chnList.GetCategoryItemCount( pCategory ); iItemIndex++)
			{
				iState = m_chnList.GetCategoryItemState( pCategory, iItemIndex );
				
				if (chn!=cHNnameBool.end())
				{
					ch=chn->second.begin();
					chn++;	
					//timeSpan.insert()
					//timeSpan.insert((make_pair(minTem[m_index-1],maxTem[m_index-1])));

					//m_index=int(k/(m_chnList.GetCategoryCount()));
					m_index=iCatIndex;
					minTime[k]=minTem[m_index];
					maxTime[k]=maxTem[m_index];

					k++;

					
					

 /*
					CString strTemp;
					strTemp.Format("%lf",minTime[k-1]);
					AfxMessageBox(strTemp);
					
*/
 				
					
				}	
				
				m_k=k;


				ch->second=iState;



			}
		}
		chn=cHNnameBool.begin();
		int cout=0;
		for(;chn!=cHNnameBool.end();chn++)
		{	

			ch=chn->second.begin();

			if (ch->second!=0)
			{
				cout++;
			}
		}
		CString coutStr;
		coutStr.Format("%d",cout);
		CChildFrame* gcf=pCMF->childFV[(int)pCMF->childFV.size()-1];
		unsigned int gisFft=((CNiWaveFraqView*)gcf->GetActiveView())->ChnFftFlag;
		CString gchnstr=((CNiWaveFraqView*)gcf->GetActiveView())->chnStr;
		CString gtitle=gcf->titles;

		chn=cHNnameBool.begin();
		int pos=0;
		m_progress.SetPos(0);
		//int samplFreq;
		map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();


		int m_pos=0;

		
		for(;chn!=cHNnameBool.end();chn++)
			
		{

			

 

			ch=chn->second.begin();
			CString temp=chn->first;
			CString examParamStr=temp.Left(temp.Find('#',0));
			
			
			if (ch->second!=0)
			{
				//char str_tmp[MAX_CHN_NAME_LEN];
				//memset(str_tmp,0,MAX_CHN_NAME_LEN);	

				CChildFrame* pCFrame = new CChildFrame();
				pos++;
				CCreateContext context;
				context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
				if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
					return;
				pCFrame->SetWindowPos(NULL,0,0,0,0,0);
				//pCFrame->ShowWindow(SW_SHOW);

				
				int min=0,max;
				CString minstr,maxstr,chnstr;

				int min2=0,max2;
				CString minstr2,maxstr2;

				map<CString,CString>::iterator itMax=xWaveMax.begin();
				for (;itMax!=xWaveMax.end();itMax++)
				{
					if (itMax->first==examParamStr)
					{
						max=atoi(itMax->second);
						break;
					}
				}

				map<CString,CString>::iterator itMin=xWaveMin.begin();
				for (;itMin!=xWaveMin.end();itMin++)
				{
					if (itMin->first==examParamStr)
					{
						min=atoi(itMin->second);
						break;
					}
				}

				map<CString,CString>::iterator itMax2=xWaveMax2.begin();
				for (;itMax2!=xWaveMax2.end();itMax2++)
				{
					if (itMax2->first==examParamStr)
					{
						max2=atoi(itMax2->second);
						break;
					}
				}

				map<CString,CString>::iterator itMin2=xWaveMin2.begin();
				for (;itMin2!=xWaveMin2.end();itMin2++)
				{
					if (itMin2->first==examParamStr)
					{
						min2=atoi(itMin2->second);
						break;
					}
				}

	
								
				unsigned char isFft=pCMF->fileData->GetChnFftFlag(filePath,ch->first);
				CString unitStr=pCMF->fileData->GetChnUnit(filePath,ch->first);
				int save_freq=0;
				max=pCMF->fileData->GetChnDataLength(filePath,examType,examParamStr,ch->first);
				vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examType,examParamStr,ch->first,&save_freq);
				
				int tempMin,tempMax;
				tempMin=ceil(minTime[m_k-m_pos-1]*save_freq);
				tempMax=floor(maxTime[m_k-m_pos-1]*save_freq);



				m_pos++;
 
				//CString strTemp;
				//strTemp.Format("%lf",minTime[pos-1]);
				//strTemp.Format("%d",pos-1);
				//AfxMessageBox(strTemp);
 
				if (tempMin>=0&&tempMax>0&&tempMin<tempMax)
				{
					//min=tempMin<min?tempMin:min;
					//max=tempMax>max?max:tempMax;
					if (min<tempMin&&tempMin<max)
						min=tempMin;
					if (max>tempMax)
						max=tempMax;


				}

				//min=maxValue>waveHeightMax?minValue:waveHeightMax;
				//max=minValue<waveHeightMin?minValue:waveHeightMin;


				//pCMF->m_PWave->isHasKeyPhase=false;
				//pCMF->m_PWave->isFullPeriodFFT=false;
				
				pCMF->m_PWave->SetData(chnData,save_freq,min,max);/**/


				CString   strTemp;
				double m_t;				


				pCMF->m_PWave->ChnFftFlag=isFft;
				pCMF->m_PWave->SetTitle(examName+","+examType+","+examParamStr+","+ch->first+"(单位："+unitStr+")");



				pCFrame->InitialUpdateFrame(NULL,TRUE);//位置放到后面一点//Marshall  2011.2.25 10:59

				/*--------------------------Marshall's Code2011.2.25 10:56--------------*/
				//默认为整周期//Marshall  2011.2.25 15:0
				int key_Freq=0;

				vector<float> keyData = pCMF->fileData->GetKeyPhase(
					filePath,
					examType,
					examParamStr,
					&key_Freq);


				pCMF->m_PWave->isGraph=isGraph;

			 	pCMF->m_PWave->SetKeyPhase(keyData,key_Freq);    //cxy


				pCMF->m_PWave->m_batchSaveBit=true;
				pCMF->m_PWave->m_minX=min;
				pCMF->m_PWave->m_maxX=max;



				//pCMF->m_PWave->SendMessage(WM_COMMAND,(WPARAM)ID_FullPeriodFFT);
				pCMF->m_PWave->SendMessage(WM_USER_FULLPR,0,0);


				
				m_t=double(pCMF->m_PWave->m_Graph.Axes.Item("XAxis").GetMinimum());
				strTemp.Format("%lf",m_t);
				//AfxMessageBox(strTemp);

				pCMF->m_PWave->SetFreqRange(min2,max2);

				CreateDirectory(savePath+"\\"+examName+"_"+examType,0);
				CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"波形频谱图",0);

				bool bTest  = true;
/*
				CString strTemp = examParamStr;//应急
				int nLen = strTemp.Find(_T("/"));
				strTemp.Delete(nLen,1);
				examParamStr = strTemp.GetBuffer(0);//应急
*/				
				bTest = CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"波形频谱图"+"\\"+examType+"_"+examParamStr,0);
				//pCMF->m_PWave->SaveAllView(savePath+"\\"+examName+"_"+examType+"\\"+"波形频谱图"+"\\"+examType+"_"+examParamStr+"\\"+ch->first,m_fileType);/**/
				
				pCMF->m_PWave->SaveAllView(savePath+"\\"+examName+"_"+examType+"\\"+"波形频谱图"+
					"\\"+examType+"_"+examParamStr+"\\"+ch->first,m_fileType);/**/
				//pCMF->m_PWave->m_batchSaveBit=false;
  
				m_progress.SetPos (100*pos/cout);

				if(pos==cout)
				{
					pCMF->m_PWave->ChnFftFlag=gisFft;
					pCMF->m_PWave->chnStr=gchnstr;
					pCMF->m_PWave->SetTitle(gtitle);
				}
				pCFrame->DestroyWindow();
				CString posStr;
				posStr.Format("%d",pos);

				m_msg.SetWindowText("存储通道（"+posStr+"/"+coutStr+")： "+examParamStr+ch->first);
				/*m_pProgressDlg->m_strCurrFile.Format (chn->first);				
				m_pProgressDlg->RedrawHeader ();*/
				//m_pProgressDlg->SetPos(pos*2.5);
				//m_pProgressDlg->StepIt ();
				//pCFrame->CloseWindow();
			}
			//delete m_pProgressDlg;
			//m_pProgressDlg = NULL;
		}
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：完成批量存储为图像文件，路径为 "+savePath+"\\"+examName+"_"+examType+"\\"+"波形频谱图"+"\\");

		AfxMessageBox("存储完毕!");
		OnCancel();
	}

}

//void SaveImageBatDlg::OnBnClickedButton2()
//{
//	LPCTSTR  pCategory;
//	int      iState;
//	int      iCatIndex;
//	int      iItemIndex;
//
//	for (iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++) {
//		pCategory = m_chnList.GetCategoryName( iCatIndex, false );
//		for (iItemIndex = 0; iItemIndex < m_chnList.GetCategoryItemCount( pCategory ); iItemIndex++) {
//			iState = m_chnList.GetCategoryItemState( pCategory, iItemIndex );
//			m_chnList.SetCategoryItemState( pCategory, iItemIndex, (iState + 1) % 2 );
//		}
//	}
//}

map<CString,map<CString,BOOL>> SaveImageBatDlg::SetCHNnameBool(CString examParam,CString chnName,BOOL isSel)
{
	map<CString,BOOL> chn;
	chn.insert(make_pair(chnName,isSel));
	cHNnameBool.insert(make_pair(examParam,chn));
	return cHNnameBool;
}

vector<CString> SaveImageBatDlg::SetExamParam(CString paramName)
{
	examParam.push_back(paramName);
	return examParam;
}

void SaveImageBatDlg::OnLbnSelchangeList2()
{

	int /*min=0,max,*/chnItm;
	CString minstr,maxstr,chnstr;
	int min2=0,max2;
	CString minstr2,maxstr2;
	BOOL isReadonly=0;
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	chnstr = m_chnList.GetCategoryName( m_chnList.GetCurSel(), false );
	//int k=m_chnList.GetCurSel();
	chnItm = m_chnList.GetCategoryItemIndex( m_chnList.GetCurSel());

	//max=pCMF->fileData->GetParaDataLength(filePath,examType,chnstr);
	
	vector<CString>::iterator exn=examParam.begin();
	for (;exn!=examParam.end();exn++)
	{		
		if (*exn==chnstr&&chnItm==-1)//&&chnItmName!=""
		{	
			//isReadonly=m_EditMax.SetReadOnly(FALSE);
			//m_EditMin.SetReadOnly(FALSE);
			isReadonly=m_EditMax2.SetReadOnly(FALSE);
			m_EditMin2.SetReadOnly(FALSE);
			break;
		}
		else
		{
			//m_EditMax.SetReadOnly(TRUE);
			//m_EditMin.SetReadOnly(TRUE);
			m_EditMax2.SetReadOnly(TRUE);
			m_EditMin2.SetReadOnly(TRUE);
		}
	}

	//map<CString,CString>::iterator itMax=xWaveMax.begin();
	//for (;itMax!=xWaveMax.end();itMax++)
	//{
	//	if (itMax->first==chnstr)
	//	{
	//		max=atoi(itMax->second);
	//		maxstr.Format("%d",max);				
	//		break;
	//	}
	//}

	//map<CString,CString>::iterator itMin=xWaveMin.begin();
	//for (;itMin!=xWaveMin.end();itMin++)
	//{
	//	if (itMin->first==chnstr)
	//	{
	//		min=atoi(itMin->second);
	//		minstr.Format("%d",min);
	//		break;
	//	}
	//}
	map<CString,CString>::iterator itMax2=xWaveMax2.begin();
	for (;itMax2!=xWaveMax2.end();itMax2++)
	{
		if (itMax2->first==chnstr)
		{
			max2=atoi(itMax2->second);
			maxstr2.Format("%d",max2);
			break;
		}
	}

	map<CString,CString>::iterator itMin2=xWaveMin2.begin();
	for (;itMin2!=xWaveMin2.end();itMin2++)
	{
		if (itMin2->first==chnstr)
		{
			min2=atoi(itMin2->second);
			minstr2.Format("%d",min2);
			break;
		}
	}
	
	
	if (isReadonly)
	{
		//m_EditMin.SetWindowText(minstr);
		//m_EditMax.SetWindowText(maxstr);
		m_EditMin2.SetWindowText(minstr2);
		m_EditMax2.SetWindowText(maxstr2);
	}
	else
	{
		//m_EditMin.SetWindowText("");
		//m_EditMax.SetWindowText("");
		m_EditMin2.SetWindowText("");
		m_EditMax2.SetWindowText("");
	}
}

void SaveImageBatDlg::OnBnClickedChgrng()
{
	CString /*minstr,maxstr,*/chnstr;
	CString minstr2,maxstr2;

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	
	chnstr = m_chnList.GetCategoryName( m_chnList.GetCurSel(), false );

	//m_EditMin.GetWindowText(minstr);
	//m_EditMax.GetWindowText(maxstr);
	m_EditMin2.GetWindowText(minstr2);
	m_EditMax2.GetWindowText(maxstr2);

	//map<CString,CString>::iterator itMax=xWaveMax.begin();
	//for (;itMax!=xWaveMax.end();itMax++)
	//{
	//	if (itMax->first==chnstr)
	//	{
	//		xWaveMax.erase(itMax);
	//		break;
	//	}
	//}
	//map<CString,CString>::iterator itMin=xWaveMin.begin();
	//for (;itMin!=xWaveMin.end();itMin++)
	//{
	//	if (itMin->first==chnstr)
	//	{
	//		xWaveMin.erase(itMin);
	//		break;
	//	}
	//}
	map<CString,CString>::iterator itMax2=xWaveMax2.begin();
	for (;itMax2!=xWaveMax2.end();itMax2++)
	{
		if (itMax2->first==chnstr)
		{
			xWaveMax2.erase(itMax2);
			break;
		}
	}
	map<CString,CString>::iterator itMin2=xWaveMin2.begin();
	for (;itMin2!=xWaveMin2.end();itMin2++)
	{
		if (itMin2->first==chnstr)
		{
			xWaveMin2.erase(itMin2);
			break;
		}
	}
	//xWaveMax.erase()
	//xWaveMin.insert(make_pair(chnstr,minstr));
	//xWaveMax.insert(make_pair(chnstr,maxstr));
	xWaveMin2.insert(make_pair(chnstr,minstr2));
	xWaveMax2.insert(make_pair(chnstr,maxstr2));
}

//void SaveImageBatDlg::OnBnClickedButton2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
//
//void SaveImageBatDlg::OnBnClickedCancel()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	OnCancel();
//}

//void SaveImageBatDlg::OnCbnSelchangeFilenamecmbo()
//{
//	CString filepath,examName;
//	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
//
//	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
//	pCMF->fileData->SetExamTree(filepath);
//
//	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(filepath);
//
//	examTypeComb.ResetContent();
//	pCMF->fileData->SetExamTree(filepath);
//
//	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
//
//	examName=iterE->second;
//	vector<CString> examType= pCMF->fileData->GetExamType(filepath,examName);
//	vector<CString>::iterator it=examType.begin();
//
//	for (;it!=examType.end();it++)
//	{
//		examTypeComb.InsertString(0,*it);
//	}
//	examTypeComb.SetCurSel(0);
//	examTypeComb.EnableWindow(TRUE);
//}
//
//void SaveImageBatDlg::OnCbnSelchangeExamtypecombo()
//{
//	CString filepath,examType;
//	//examParamComb.ResetContent();
//	examParam.clear();
//	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
//	pCMF->fileData->SetExamTree(filepath);
//
//	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
//	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
//
//	//vector<CString> paramType
//	examParam= pCMF->fileData->GetExamParam(filepath,examType);
//	
//	FillData();
//	//vector<CString>::iterator it=paramType.begin();
//
//	//for (int p=(int)paramType.size()-1;p>=0;p--)
//	//{
//	//	examParamComb.InsertString(0,it[p]);
//	//}
//	//examParamComb.SetCurSel(0);
//	//examParamComb.EnableWindow(TRUE);
//}

void SaveImageBatDlg::OnBnClickedCancel()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消批量存储为图像文件");
	OnCancel();
}

void SaveImageBatDlg::OnBnClickedButton2()
{
	CString strFilter="波形图时段配置文件 (*.CON) | *.con;*.CON ||";
	CFileDialog dlg(TRUE,//open/Save as
		NULL,//Ext added automatically
		NULL,//filename
		OFN_HIDEREADONLY |OFN_FILEMUSTEXIST,
		strFilter);
	//m_Bconfig=false;

	if(dlg.DoModal()==IDOK)
	{		
		configPath=dlg.GetPathName();

	 	m_configPath.SetWindowText(configPath);
		m_Bconfig=TRUE;

		//AfxMessageBox(configPath);

/*		FILE * f=fopen(configPath,"r");
		if(f!=0)
		{	
			while(!feof(f))
			{
				double data;
				if(fscanf(f,"%lf",&data)==1)
					fileData.push_back(data);
				else
				{
					break;
				}
			}
	 
			
			fclose(f);*/
		}				
}

void SaveImageBatDlg::OnBnClickedChgrng2()
{
	if (m_Bconfig)
	{
		//AfxMessageBox("true");
	
		FILE * fp=fopen(configPath,"r");
		int i=0;
		//m_chnList.GetCategoryCount()

	
		if(NULL != fp)
		{
			for (;i<m_chnList.GetCategoryCount();i++)
			{
				fscanf(fp,"%lf\t%lf\n",minTem+i,maxTem+i); 

				
			}

/*			for (i=0;i<10;i++)
			{
				CString strTemp;
				strTemp.Format("%lf",minTem[i]);
				AfxMessageBox(strTemp);

			}


			
*/

			m_con=true;

			fclose(fp);

		}
		
	}
	else
		
	{
		AfxMessageBox("请选择波形时段配置文件!");

	}
	
	
}

void SaveImageBatDlg::OnBnClickedCheck1()
{
	if (m_graphChk.GetCheck())
	{
		isGraph=TRUE;
		m_graphChk.SetCheck(TRUE);
	}
	else
	{
		isGraph=FALSE;
		m_graphChk.SetCheck(FALSE);		
	}
}
