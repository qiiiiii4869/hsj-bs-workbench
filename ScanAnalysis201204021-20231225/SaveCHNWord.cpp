// SaveCHNWord.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "SaveCHNWord.h"
#include "MainFrm.h"

// CSaveCHNWord 对话框
using namespace std;
IMPLEMENT_DYNAMIC(CSaveCHNWord, CHSJDialog)

CSaveCHNWord::CSaveCHNWord(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CSaveCHNWord::IDD, pParent),wordVision(1)
{

}

CSaveCHNWord::~CSaveCHNWord()
{
}

void CSaveCHNWord::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_savePath);
	DDX_Control(pDX, IDC_STATIC_MSG, m_msg);
	DDX_Control(pDX, IDC_STATIC_TYPEEXAM, m_examType);
	//DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_LIST2, m_chnList);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_CANCEL);

	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(CSaveCHNWord, CHSJDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSaveCHNWord::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CSaveCHNWord::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSaveCHNWord::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_RADIO1, &CSaveCHNWord::OnBnClickedRadio1)
	//ON_BN_CLICKED(IDC_RADIO2, &CSaveCHNWord::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CSaveCHNWord 消息处理程序

//判断word版本
void CSaveCHNWord::GetWordVision()
{
	//int wordVision=0;//word类型，0：word2003		1：word2007

	HKEY hKEY; //定义有关的 hKE
	LPCTSTR data_Set0="SOFTWARE\\Microsoft\\Office\\12.0\\Word\\InstallRoot"; 
	//打开与路径 data_Set 相关的 hKEY，第一个参数为根键名称，第二个参数表。 
	//表示要访问的键的位置，第三个参数必须为0，KEY_READ表示以查询的方式。 
	//访问注册表，hKEY则保存此函数所打开的键的句柄。 
	long ret0=RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set0, 0, KEY_READ, &hKEY); 
	if(ret0==ERROR_SUCCESS) //如果无法打开hKEY，则终止程序的执行 
	{
		wordVision=0;
	} 

	//LPCTSTR data_Set1="SOFTWARE\\Microsoft\\Office\\11.0\\Word\\InstallRoot"; 
	//long ret1=RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set1, 0, KEY_READ, &hKEY); 
	//if(ret1==ERROR_SUCCESS) //如果无法打开hKEY，则终止程序的执行 
	else
	{
		wordVision=1;
	} 
	/*else
	{
		AfxMessageBox("请选择word2003或word2007版本！");
		return;
	}*/

	RegCloseKey(hKEY);
}

void CSaveCHNWord::OnBnClickedButton1()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	m_savePath.SetWindowText(savePath);
}

void CSaveCHNWord::OnBnClickedCancel()
{
	//application.Quit();
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消批量存储为word报表");

	OnCancel();
}

void CSaveCHNWord::OnBnClickedOk()
{



	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CChildFrame* pv=pCMF->childFV[pCMF->childFV.size()-1];
	CString editString;
	m_savePath.GetWindowText(editString);
	if (editString=="")
	{
		AfxMessageBox("请选择路径");
		
	}
	else
	{						
		//if(AfxMessageBox("需要关闭所有波形频谱图，是否继续？",MB_OKCANCEL)==IDOK)
		//{
		//	//std::vector<CChildFrame*>::iterator cfv=pCMF->childFV.end();
		//	for (int p=0;p<(int)pCMF->childFV.size();p++)
		//	{
		//		pCMF->childFV[p]->OnClose();
		//	}
		//	pCMF->childFV.clear();
		//}
		//else
		//{
		//	return;
		//}

		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：批量存储为word报表...");

		//CString s;
		pCMF->fileData->SetExamTree(filePath);

		//int iSheet=0;
				//		CString strTemp = examParamStr;//应急
				//int nLen = strTemp.Find(_T("/"));
				//strTemp.Delete(nLen,1);
				//examParamStr = strTemp.GetBuffer(0);//应急


		CreateDirectory(savePath+"\\"+examName+"_"+examType,0);
		map<CString,map<CString,BOOL>>::iterator chn=cHNnameBool.begin();
		map<CString,BOOL>::iterator ch=chn->second.begin();
		LPCTSTR  pCategory;
		int      iState;
		int      iCatIndex;
		int      iItemIndex;

		for (iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++)
		{
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
		
		//int samplFreq;

		//map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
		//for (;its!=pCMF->fileData->samFreq.end();its++)
		//{
		//	if (its->first==pCMF->FilePathName)
		//	{
		//		samplFreq=atoi(its->second);
		//	}
		//}
		CTime ctm;
		CString timeStr;
		ctm=CTime::GetCurrentTime();
		timeStr=ctm.Format("%Y-%m-%d %H-%M-%S");

		chn=cHNnameBool.begin();
		int pos=0;
		m_progress.SetPos(0);

		//CChildFrame* pCFrameT = new CChildFrame();
		//CCreateContext contextT;
		//contextT.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
		//if(!pCFrameT->LoadFrame(IDI_EXAM_C,WS_MINIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&contextT))
		//	return;
		//pCFrameT->SetWindowPos(NULL,0,0,0,0,0);
		//CString unitStr=pCMF->fileData->GetChnUnit(filePath,ch->first);
		//CString strT=chn->first;
		//strT.Left(strT.Find('#',0));
		//pCMF->m_PWave->SetData(pCMF->fileData->GetDataFromFile(filePath,examType,strT,ch->first),samplFreq,0,pCMF->fileData->GetDataFromFile(filePath,examType,strT,ch->first).size());/**/


		//pCMF->m_PWave->SetTitle("批量存储到word");				

		//pCFrameT->InitialUpdateFrame(NULL,TRUE);
		CChildFrame* gcf=pCMF->childFV[(int)pCMF->childFV.size()-1];
		unsigned int gisFft=((CNiWaveFraqView*)gcf->GetActiveView())->ChnFftFlag;
		CString gchnstr=((CNiWaveFraqView*)gcf->GetActiveView())->chnStr;
		CString gtitle=gcf->titles;

		if (wordVision==0)
		{	
			if (cout>=65)
			{
				AfxMessageBox("通道存储数应小于65个,当前通道数："+coutStr+",请重选通道！");
				return;
			}
			m_OK.ShowWindow(FALSE);
			m_CANCEL.ShowWindow(FALSE);
			m_progress.ShowWindow(TRUE);

			//CNiWordApplication application(false);
			CNiWordApplication application(FALSE);
			//application.SetVisible(FALSE);

			CNiWordDocument document = application.CreateDocument();
			document.SetHeader("", examName+"_"+examType, "", CNiWordDocument::FieldDate, CNiWordDocument::FieldEmpty, CNiWordDocument::FieldTime);
			
			//pCMF->m_PWave->ChnFftFlag=isFft;
			//pCMF->m_PWave->chnStr=examParamStr+"_"+ch->first;
			//pCMF->m_PWave->SetData(chnData,samplFreq,min,max);/**/
			//pCMF->m_PWave->SetTitle(examName+","+examType+","+examParamStr+","+ch->first+"(单位："+unitStr+")");


			for(;chn!=cHNnameBool.end();chn++)
			{
				ch=chn->second.begin();
				CString temp=chn->first;
				CString examParamStr=temp.Left(temp.Find('#',0));
				
				if (ch->second!=0)
				{
					pos++;
					CString posStr;
					posStr.Format("%d",pos);
					m_msg.SetWindowText("处理通道("+posStr+"/"+coutStr+")： "+examParamStr+"_"+ch->first);	
					int save_freq=0;
					int p=pCMF->fileData->GetChnDataLength(filePath,examType,examParamStr,ch->first);	
					vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examType,examParamStr,ch->first,&save_freq);

					pCMF->childFV;
					//chID.push_back(pos);

					int min=0,max=p;

					CChildFrame* pCFrame = new CChildFrame();
					CCreateContext context;
					context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
					if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MINIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
						return;
					pCFrame->SetWindowPos(NULL,0,0,0,0,0);

					unsigned char isFft=pCMF->fileData->GetChnFftFlag(filePath,ch->first);
					CString unitStr=pCMF->fileData->GetChnUnit(filePath,ch->first);
					pCMF->m_PWave->ChnFftFlag=isFft;
					pCMF->m_PWave->chnStr=examParamStr+"_"+ch->first;
					pCMF->m_PWave->SetData(chnData,save_freq,min,max);/**/
					pCMF->m_PWave->SetTitle(examName+","+examType+","+examParamStr+","+ch->first+"(单位："+unitStr+")");				


					//pCFrame->InitialUpdateFrame(NULL,TRUE);//放到后面一点//Marshall  2011.2.25 11:6

							/*--------------------------Marshall's Code2011.2.25 10:56--------------*/

								//////////////////////////////////////////////////////////////////////////
								//int cout=str.Find('#',0);
								//CString keyPStr=str.Left(cout+1)+"SJ90键相";
								//pCMF->m_PWave->SetKeyPhase(pCMF->fileData->GetDataFromFile(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),keyPStr));
								int key_Freq=0;

							/*	vector<float> chnData=pCMF->fileData->GetDataFromFile(
									filePath,
									examType,
									examParamStr,
									ch->first,&save_freq);*/

								vector<float> keyData = pCMF->fileData->GetKeyPhase(
									filePath,
									examType,
									examParamStr,
									&key_Freq);
								
								pCMF->m_PWave->SetKeyPhase(keyData,key_Freq);
							/*--------------------------Marshall's Code2011.2.25 10:56--------------*/

								pCFrame->InitialUpdateFrame(NULL,TRUE);

					//默认整周期处理,没有实现//Marshall  2011.2.25 10:17
					//pCMF->m_PWave->SendMessage(WM_COMMAND,(WPARAM)ID_FullPeriodFFT);
					//pCMF->m_PWave->SetFreqRange(min2,max2);
					pCMF->m_PWave->SendMessage(WM_USER_FULLPR,0,0);

					pCMF->m_PWave->WriteToWord(document,wordVision);

					/*pCMF->m_PWave->FFTProc();*/
/*					if(pCMF->m_PWave!=NULL)  
					{  
						pCMF->m_PWave->DestroyWindow();  
						pCMF->m_PWave = NULL;  
					}  */ 

					//m_nProgress+=pos;
					m_progress.SetPos (100*pos/cout);
					if (pos!=cout)
					{
						document.NewPage();
					}
					else
					{
						pCMF->m_PWave->ChnFftFlag=gisFft;
						pCMF->m_PWave->chnStr=gchnstr;
						pCMF->m_PWave->SetTitle(gtitle);
					}
					pCFrame->DestroyWindow();

				}

			}
			document.SetFooter("","","",CNiWordDocument::FieldEmpty, CNiWordDocument::FieldPage, CNiWordDocument::FieldEmpty);

			//DeleteFile(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" word汇总表"+timeStr+".doc");
			document.SaveAs(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType/*+"_"+ch->first*/+" word汇总表"+timeStr+".doc");
			m_msg.SetWindowText("Word数据写入完成！");
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：完成批量存储为word报表，存储为 "+savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" word汇总表"+timeStr+".doc");
			document.Close();
			//application.Quit();	//如果已经打开一个word，再执行此程序，会弹出警告//Marshall  2011.2.25 12:46
			application.Quit(CNiWordApplication::DoNotSaveChanges);	
			//恢复前通道波形图特征
			

		}
		if (wordVision==1)
		{	
			if (cout>=65)
			{
				AfxMessageBox("通道存储数应小于65个,当前通道数："+coutStr+",请重选通道！");
				return;
			}
			m_OK.ShowWindow(FALSE);
			m_CANCEL.ShowWindow(FALSE);
			m_progress.ShowWindow(TRUE);

			int chCount=0;
			//int wordCout=0;
			//CString wordCoutStr="";
			
			for(;chn!=cHNnameBool.end();chn++)
			{				
				ch=chn->second.begin();									
				if (ch->second!=0)
				{
					chCount++;
					CString temp=chn->first;
					CString examParamStr=temp.Left(temp.Find('#',0));
					
					if (chCount==1)
					{
						((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：写出文档 "+savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType/*+"_"+ch->first*/+" word汇总表"+timeStr+".doc");

						CNiWordApplication application(false);
						//wordCout++;
						//wordCoutStr.Format("%d",wordCout);
						CNiWordDocument document=application.CreateDocument();
						document.SetHeader("", examName+"_"+examType, "", CNiWordDocument::FieldDate, CNiWordDocument::FieldEmpty, CNiWordDocument::FieldTime);
						
						pos++;

						CString posStr;
						posStr.Format("%d",pos);
						m_msg.SetWindowText("通处理道("+posStr+"/"+coutStr+")： "+examParamStr+"_"+ch->first);					
						int save_freq=0;
						int p=pCMF->fileData->GetChnDataLength(filePath,examType,examParamStr,ch->first);	
						vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examType,examParamStr,ch->first,&save_freq);

							//chID.push_back(pos);

							int min=0,max=p;

							CChildFrame* pCFrame = new CChildFrame();
							CCreateContext context;
							context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
							if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MINIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
								return;
							pCFrame->SetWindowPos(NULL,0,0,0,0,0);

							unsigned char isFft=pCMF->fileData->GetChnFftFlag(filePath,ch->first);
							CString unitStr=pCMF->fileData->GetChnUnit(filePath,ch->first);
							pCMF->m_PWave->ChnFftFlag=isFft;
							pCMF->m_PWave->chnStr=examParamStr+"_"+ch->first;
							pCMF->m_PWave->SetData(chnData,save_freq,min,max);/**/
							pCMF->m_PWave->SetTitle(examName+","+examType+","+examParamStr+","+ch->first+"(单位："+unitStr+")");				

							//pCFrame->InitialUpdateFrame(NULL,TRUE);//位置放到后面一点//Marshall  2011.2.25 10:59

							/*--------------------------Marshall's Code2011.2.25 10:56--------------*/

								//////////////////////////////////////////////////////////////////////////
								//int cout=str.Find('#',0);
								//CString keyPStr=str.Left(cout+1)+"SJ90键相";
								//pCMF->m_PWave->SetKeyPhase(pCMF->fileData->GetDataFromFile(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),keyPStr));
								int key_Freq=0;

							/*	vector<float> chnData=pCMF->fileData->GetDataFromFile(
									filePath,
									examType,
									examParamStr,
									ch->first,&save_freq);*/

								vector<float> keyData = pCMF->fileData->GetKeyPhase(
									filePath,
									examType,
									examParamStr,
									&key_Freq);
								
								pCMF->m_PWave->SetKeyPhase(keyData,key_Freq);
							/*--------------------------Marshall's Code2011.2.25 10:56--------------*/

									pCFrame->InitialUpdateFrame(NULL,TRUE);

					//默认整周期处理,没有实现//Marshall  2011.2.25 10:17
					//pCMF->m_PWave->SendMessage(WM_COMMAND,(WPARAM)ID_FullPeriodFFT);
									pCMF->m_PWave->SendMessage(WM_USER_FULLPR,0,0);

							//pCMF->m_PWave->SetFreqRange(min2,max2);

							pCMF->m_PWave->WriteToWord(document,wordVision);

							/*pCMF->m_PWave->FFTProc();*/
 
							pCFrame->DestroyWindow();

							//m_nProgress+=pos;
							m_progress.SetPos (100*pos/cout);
							/*if (pos!=cout)*/
							document.NewPage();	
							document.SetFooter("","","",CNiWordDocument::FieldEmpty, CNiWordDocument::FieldPage, CNiWordDocument::FieldEmpty);

							//DeleteFile(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" word汇总表"+".doc");
							document.SaveAs(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType/*+"_"+ch->first*/+" word汇总表"+timeStr+".doc");
							//m_msg.SetWindowText("Word汇总表"+wordCoutStr+"写入完成！");
							document.Close();
							//application.Quit();	//如果已经打开一个word，再执行此程序，会弹出警告//Marshall  2011.2.25 12:46
							application.Quit(CNiWordApplication::DoNotSaveChanges);	
							//m_msg.SetWindowText("通道数据："+examParamStr+"_"+ch->first+"写入完成！");

						//}

					}
					else
					{
						CNiWordApplication application(false);
						CNiWordDocument document=application.OpenDocument(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" word汇总表"+timeStr+".doc");
						//document.SetHeader("", examName+"_"+examType, "", CNiWordDocument::FieldDate, CNiWordDocument::FieldEmpty, CNiWordDocument::FieldTime);
						document.GoToDocumentEnd();
						ch=chn->second.begin();
						CString temp=chn->first;
						CString examParamStr=temp.Left(temp.Find('#',0));
							
						//if (ch->second!=0)
						//{
						pos++;

						CString posStr;
						posStr.Format("%d",pos);
						m_msg.SetWindowText("处理通道("+posStr+"/"+coutStr+")： "+examParamStr+"_"+ch->first);	
						int save_freq=0;
						int p=pCMF->fileData->GetChnDataLength(filePath,examType,examParamStr,ch->first);
						vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examType,examParamStr,ch->first,&save_freq);

							//pos++;
							//chID.push_back(pos);

							int min=0,max=p;

							CChildFrame* pCFrame = new CChildFrame();
							CCreateContext context;
							context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
							if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MINIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
								return;
							pCFrame->SetWindowPos(NULL,0,0,0,0,0);

							unsigned char isFft=pCMF->fileData->GetChnFftFlag(filePath,ch->first);
							CString unitStr=pCMF->fileData->GetChnUnit(filePath,ch->first);
							pCMF->m_PWave->ChnFftFlag=isFft;
							pCMF->m_PWave->chnStr=examParamStr+"_"+ch->first;
							pCMF->m_PWave->SetData(chnData,save_freq,min,max);/**/
							pCMF->m_PWave->SetTitle(examName+","+examType+","+examParamStr+","+ch->first+"(单位："+unitStr+")");				

							//pCFrame->InitialUpdateFrame(NULL,TRUE);//放到后面一点//Marshall  2011.2.25 11:6

							/*--------------------------Marshall's Code2011.2.25 10:56--------------*/

								//////////////////////////////////////////////////////////////////////////
								//int cout=str.Find('#',0);
								//CString keyPStr=str.Left(cout+1)+"SJ90键相";
								//pCMF->m_PWave->SetKeyPhase(pCMF->fileData->GetDataFromFile(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),keyPStr));
								int key_Freq=0;

							/*	vector<float> chnData=pCMF->fileData->GetDataFromFile(
									filePath,
									examType,
									examParamStr,
									ch->first,&save_freq);*/

								vector<float> keyData = pCMF->fileData->GetKeyPhase(
									filePath,
									examType,
									examParamStr,
									&key_Freq);
								
								pCMF->m_PWave->SetKeyPhase(keyData,key_Freq);
							/*--------------------------Marshall's Code2011.2.25 10:56--------------*/

								pCFrame->InitialUpdateFrame(NULL,TRUE);

					//默认整周期处理,没有实现，注释掉//Marshall  2011.2.25 10:17
					//pCMF->m_PWave->SendMessage(WM_COMMAND,(WPARAM)ID_FullPeriodFFT);
								pCMF->m_PWave->SendMessage(WM_USER_FULLPR,0,0);


							//pCMF->m_PWave->SetFreqRange(min2,max2);

							pCMF->m_PWave->WriteToWord(document,wordVision);

							/*pCMF->m_PWave->FFTProc();*/
 
							//if(!pCFrame->DestroyWindow())
							//	pCFrame->DestroyWindow();

							//m_nProgress+=pos;
							m_progress.SetPos (100*pos/cout);
							if (pos!=cout)
							{
								document.NewPage();
							}
							else
							{
								pCMF->m_PWave->ChnFftFlag=gisFft;
								pCMF->m_PWave->chnStr=gchnstr;
								pCMF->m_PWave->SetTitle(gtitle);
							}
							pCFrame->DestroyWindow();
							//document.SetFooter("","","",CNiWordDocument::FieldEmpty, CNiWordDocument::FieldPage, CNiWordDocument::FieldEmpty);

							//DeleteFile(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" word汇总表"+".doc");
							document.SaveAs(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType/*+"_"+ch->first*/+" word汇总表"+timeStr+".doc");
							
							document.Close();
//							application.Quit();	//如果已经打开一个word，再执行此程序，会弹出警告//Marshall  2011.2.25 12:46
							application.Quit(CNiWordApplication::DoNotSaveChanges);	
							//Sleep(100);
							
			
						}	
						/*if (chCount%10==9)
						{
							
							((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：完成批量存储为word报表，存储路径为 "+savePath+"\\"+examName+"_"+examType+"\\");

						}		
						*/
						//CString curChn;
						//curChn.Format("%d",pos);
						//m_msg.SetWindowText("通道("+curChn+")："+examParamStr+"_"+ch->first+"写入完成！");

				}
				//application.Quit();
	
			}	

		}
		//for (int iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++)
		//{
		//	CNiWordApplication application(false);

		//	if (m_List.GetCheck(i))
		//	{
		//		CNiWordDocument document = application.CreateDocument();				

		//		/*document.NewPage();*/
		//		s=m_List.GetItemText(i,0);
		//		iSheet++;

		//		document.SetHeader("", examName+"_"+examType+"_"+s, "", CNiWordDocument::FieldDate, CNiWordDocument::FieldEmpty, CNiWordDocument::FieldTime);

		//		int pos=0;
		//		vector<CString> chn=pCMF->fileData->GetChnName(filePath,s);
		//		vector<int> chID;

		//		std::vector<CString>::iterator  it=chn.begin();
		//		int p=pCMF->fileData->GetDataLength(filePath,examType,s);	

		//		m_progress.SetPos(0);

		//		for (;it<chn.end();it++)
		//		{	
		//			m_msg.SetWindowText("计算数据,通道： "+s+"_"+*it);	
		//			vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examType,examParamStr,ch->first);

		//			pos++;
		//			chID.push_back(pos);

		//			int min=0,max=p;
	
		//			CChildFrame* pCFrame = new CChildFrame();
		//			CCreateContext context;
		//			context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
		//			if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MINIMIZE|WS_DISABLED|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
		//				return;
		//			pCFrame->SetWindowPos(NULL,0,0,0,0,0);
		//			
		//			pCMF->m_PWave->chnStr=*it;
		//			pCMF->m_PWave->SetData(chnData,samplFreq,min,max);/**/
		//			pCMF->m_PWave->SetTitle(examName+","+examType+","+s+","+*it);					
		//			
		//			pCFrame->InitialUpdateFrame(NULL,true);


		//			//pCMF->m_PWave->SetFreqRange(min2,max2);
		//			
		//			pCMF->m_PWave->WriteToWord(document);

		//			/*pCMF->m_PWave->FFTProc();*/
		//			pCFrame->DestroyWindow();

		//			//m_nProgress+=pos;
		//			m_progress.SetPos (100*pos/(int)chn.size());
		//			document.NewPage();

		//		}
		//		document.SetFooter("","","",CNiWordDocument::FieldEmpty, CNiWordDocument::FieldPage, CNiWordDocument::FieldEmpty);

		//		DeleteFile(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+"_"+s+" word汇总表"+".doc");
		//		document.SaveAs(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+"_"+s+" word汇总表"+".doc");
		//	}
		//pCFrameT->DestroyWindow();
		AfxMessageBox("word存储完成！");

		OnOK();
		
		}

		//m_workBook.Save(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" word报表"+".doc",CNiExcelWorkbook::FileFormatExcel7);
}

BOOL CSaveCHNWord::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
	//m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	////m_List.SetColumnHeader(_T("参数,150;"));

	//for (int k=0;k<(int)paramVector.size();k++)
	//{
	//	m_List.InsertItem(k, paramVector[k]);
	//	m_List.SetCheck(k,TRUE);
	//}
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	FillData();
	m_examType.SetWindowText("类型： "+examType);
	m_progress.ShowWindow(FALSE);
	
	GetWordVision();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSaveCHNWord::FillData()
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

}

map<CString,map<CString,BOOL>> CSaveCHNWord::SetCHNnameBool(CString examParam,CString chnName,BOOL isSel)
{
	map<CString,BOOL> chn;
	chn.insert(make_pair(chnName,isSel));
	cHNnameBool.insert(make_pair(examParam,chn));
	return cHNnameBool;
}

vector<CString> CSaveCHNWord::SetExamParam(CString paramName)
{
	examParam.push_back(paramName);
	return examParam;
}


//void CSaveCHNWord::OnBnClickedRadio1()
//{
//	//saveType=0;
//}
//
//void CSaveCHNWord::OnBnClickedRadio2()
//{
//	//saveType=1;
//}
