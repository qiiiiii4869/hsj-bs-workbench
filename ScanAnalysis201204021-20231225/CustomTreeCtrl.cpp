#include "StdAfx.h"
#include <stdio.h>
#include "CustomTreeCtrl.h"
#include "HSJAppearance.h"
#include "ScanAnalysisView.h"
#include "NiWaveFraqView.h"
#include "ChildFrm.h"
#include "ScanSel/SysConf.h"
#include "ScanSel/StrTool.h"
#include "MainFrm.h"
#include   <vector>   
#include   <string>   
#include   <algorithm>  


using namespace std;
CCustomTreeCtrl::CCustomTreeCtrl(void)/*:title(_T("")),cout(0)*/
{
	pDoc=new CScanAnalysisDoc;	

}

CCustomTreeCtrl::~CCustomTreeCtrl(void)
{
}
BEGIN_MESSAGE_MAP(CCustomTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnSkinCustomDraw)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


/*--------------------------Marshall's Code2011.2.25 18:8--------------*/
//如果是第一次则返回ture，表示要显示警告//Marshall  2011.2.25 18:48
BOOL CCustomTreeCtrl::ShowFullPRAlarm(const CString& strFileName)
{
	POSITION pos;
	pos = m_lsFullPRAlarm.Find(strFileName);
	if(pos != NULL)
		return FALSE;
	else
		m_lsFullPRAlarm.AddTail(strFileName);

	return TRUE;
}
/*--------------------------Marshall's Code2011.2.25 18:8--------------*/
//消息：双击通道名列表
void CCustomTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	UINT flags=0;
	//CPoint pos;
	//::GetCursorPos(&pos);
	//ScreenToClient(&pos);
	//HTREEITEM m_HitItem = HitTest(pos);
	CRect rect(0,0,0,0);
	this->GetItemRect(HitTest(point, &flags), &rect, TRUE);
	if (rect.PtInRect(point))
	{
		CString CTitle;
		BOOL vFlag=TRUE;
		int treeClass = 1,i=0;
		CString m_ParName[3];
		for(i=0;i<3;i++)
		{
			m_ParName[i] = _T("");
		}
		SetFocus();

		HTREEITEM m_selectedItem = HitTest(point, &flags);
		CString m_SelName = GetItemText(m_selectedItem);

		if(m_selectedItem==GetRootItem()) 
			return;	

		HTREEITEM hItem = GetParentItem(m_selectedItem);
		
		while(hItem!=GetRootItem())
		{
			m_ParName[treeClass-1] = GetItemText(hItem);//////////////////////////////////////////////////////////////////////////717
			
			treeClass++;
			hItem = GetParentItem(hItem);

		}


		for (int p=2;p>=0;p--)
		{
			CTitle+=pCMF->fileData->RemoveCoutFromString(m_ParName[p])+", ";
		}
		//str=strCout.Right(strCout.GetLength()-strCout.Find(']',0)-2);
		//////////////////////////////////////////////////////////////////////////
		map<int,CString>::iterator it=pCMF->fileData->fullPathName.begin();
		for (;it!=pCMF->fileData->fullPathName.end();it++)
		{
			if (it->first==0)
			{
				pCMF->fileData->fullPathName.erase(it);	
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		int fileCount=0;	

		fileCount=atoi(pCMF->fileData->GetCout(m_ParName[2]));
		CString fullPath=pCMF->fileData->fullPathName[fileCount];
		unsigned char isFft=0;
		CString unitStr="";
		if (treeClass==4)
		{
			unitStr=pCMF->fileData->GetChnUnit(fullPath,pCMF->fileData->RemoveCoutFromString(m_SelName));

/*
			AfxMessageBox(unitStr);

			AfxMessageBox(fullPath);

			AfxMessageBox(m_SelName);

*/
			isFft=pCMF->fileData->GetChnFftFlag(fullPath,pCMF->fileData->RemoveCoutFromString(m_SelName));
			CTitle+=pCMF->fileData->RemoveCoutFromString(m_SelName)+"通道波形频谱图  "+"(单位："+unitStr+")";

			vector<CString>::iterator iter;
			for( iter = vectorSelName.begin(); iter != vectorSelName.end(); iter++ )
			{
				if(*iter== CTitle)
				{
					std::vector<CChildFrame*>::iterator cfv=pCMF->childFV.begin();
					//CFrameWnd* fwnd=pCMF->GetActiveFrame();
					for (;cfv!=pCMF->childFV.end();)
					{
						if((*cfv)->titles==CTitle)
						{
							if (pCMF->fileData->fullPathName.size()==1)
							{
								pCMF->MDIActivate((*cfv));
							}
							else
							{
								(*cfv)->OnClose();
								CChildFrame* pCFrame = new CChildFrame();
								CCreateContext context;
								context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);

								if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
									return;

								pCFrame->titles=CTitle;
								pCFrame->ShowWindow(SW_SHOW);
								((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("数据通道：查看通道数据-->"+CTitle);


								CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
								pCMF->m_PWave->SetTitle(CTitle);
								CString str;
								str=m_SelName.Right(m_SelName.GetLength()-m_SelName.Find(']',0)-2);
								//int samplFreq;
								//map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
								//for (;its!=pCMF->fileData->samFreq.end();its++)
								//{
								//	if (its->first==pCMF->FilePathName)
								//	{
								//		samplFreq=atoi(its->second);
								//	}
								//}
								
								fullPath=pCMF->fileData->fullPathName[fileCount];
								/*pCMF->m_PWave->strPath=fullPath;*/
								int p=pCMF->fileData->GetChnDataLength(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),str);
								pCMF->m_PWave->SetChnDataStr(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[2]),pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),pCMF->fileData->RemoveCoutFromString(m_SelName));
								int save_freq=0;
								vector<float> chnData=pCMF->fileData->GetDataFromFile(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),str,&save_freq);
								pCMF->m_PWave->SetData(chnData,save_freq,0,p);
								pCMF->m_PWave->ChnFftFlag=isFft;

								//////////////////////////////////////////////////////////////////////////
								//int cout=str.Find('#',0);
								//CString keyPStr=str.Left(cout+1)+"SJ90键相";
								//pCMF->m_PWave->SetKeyPhase(pCMF->fileData->GetDataFromFile(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),keyPStr));
								int key_Freq=0;
								vector<float> keyData = pCMF->fileData->GetKeyPhase(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),&key_Freq);
								
								pCMF->m_PWave->SetKeyPhase(keyData,key_Freq);
								
								//////////////////////////////////////////////////////////////////////////

								pCMF->ActivateTabG(0);
								//pCMF->ActivateTabC(0);
								pCFrame->InitialUpdateFrame(NULL,true);	
								pCMF->childFV.push_back(pCFrame);
								vectorSelName.push_back(CTitle);

								//默认为整周期处理//Marshall  2011.2.25 10:4
								//pCMF->m_PWave->SendMessage(WM_COMMAND,(WPARAM)ID_FullPeriodFFT);
								BOOL bShowAlarm = ShowFullPRAlarm(fullPath);
								pCMF->m_PWave->SendMessage(WM_USER_FULLPR,bShowAlarm,0);
								//pCMF->m_PWave->SendMessage(WM_USER_FULLPR,0,0);
								
							}


							break;
						}
						cfv++;
					}
					vFlag=FALSE;
					break;
				}
				else
				{				
					vFlag=TRUE;
				}
			}
		}


		if(treeClass==4&&vFlag==TRUE)
		{
			CChildFrame* pCFrame = new CChildFrame();
			CCreateContext context;
			context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
			
			if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
				return;
			
			pCFrame->titles=CTitle;
			pCFrame->ShowWindow(SW_SHOW);


			CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

			pCMF->m_PWave->SetTitle(CTitle);
			CString str;
			str=m_SelName.Right(m_SelName.GetLength()-m_SelName.Find(']',0)-2);
			//int samplFreq;
			//map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
			//for (;its!=pCMF->fileData->samFreq.end();its++)
			//{
			//	if (its->first==pCMF->FilePathName)
			//	{
			//		samplFreq=atoi(its->second);
			//	}
			//}
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("数据通道：查看通道数据-->"+CTitle);

			fullPath=pCMF->fileData->fullPathName[fileCount];
			int p=pCMF->fileData->GetChnDataLength(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),str);
			pCMF->m_PWave->SetChnDataStr(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[2]),pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),pCMF->fileData->RemoveCoutFromString(m_SelName));
			int save_freq=0;
			vector<float> chnData=pCMF->fileData->GetDataFromFile(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),str,&save_freq);
			pCMF->m_PWave->SetData(chnData,save_freq,0,p);
			pCMF->m_PWave->ChnFftFlag=isFft;
			//////////////////////////////////////////////////////////////////////////
			//int cout=str.Find('#',0);
			//CString keyPStr=str.Left(cout+1)+"机组键相";
			//CSysConf::m_Sort_Data->m_keyIdx;
			//pCMF->m_PWave->SetKeyPhase(pCMF->fileData->GetDataFromFile(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),keyPStr));
			int key_Freq=0;
			vector<float> keyData = pCMF->fileData->GetKeyPhase(fullPath,pCMF->fileData->RemoveCoutFromString(m_ParName[1]),pCMF->fileData->RemoveCoutFromString(m_ParName[0]),&key_Freq);

			pCMF->m_PWave->SetKeyPhase(keyData,key_Freq);

			//////////////////////////////////////////////////////////////////////////

			pCMF->ActivateTabG(0);
			//pCMF->ActivateTabC(0);
			pCFrame->InitialUpdateFrame(NULL,true);	
			pCMF->childFV.push_back(pCFrame);
			vectorSelName.push_back(CTitle);
			//默认为整周期处理//Marshall  2011.2.25 10:4
			BOOL bShowAlarm = ShowFullPRAlarm(fullPath);
			//pCMF->m_PWave->SendMessage(WM_COMMAND,(WPARAM)ID_FullPeriodFFT);
			pCMF->m_PWave->SendMessage(WM_USER_FULLPR,bShowAlarm,0);

		}
	}

	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

//vector<float> CCustomTreeCtrl::GetDataFromFile(CString parName0,CString parName1,CString selName)
//{
//	//////////////////////////////////////////////////////////////////////////
//	
//	CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
//	CString FilePathName=pMainFrame->FilePathName.GetBuffer();	
//	FILE * f=0;	
//	f = fopen(FilePathName,"r+b");
//	CSysConf::m_Sort_Data->ReadHead(f);
//
//	int type_idx = CSysConf::m_Sort_Data->GetExamTypeIdx((LPSTR)(LPCSTR)parName1);
//	int para_idx = CSysConf::m_Sort_Data->GetExamParaIdx(type_idx,(LPSTR)(LPCSTR)parName0);
//	//int data_len = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan;
//
//
//
//	//读取分拣某实验类型参数的通道数据
//	int data_len = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan;
//	//请填入实验类型名，参看分拣结果目录树，按上面的中文写即可；
//	//int type_idx = CSysConf::m_Sort_Data->GetExamTypeIdx("升负荷");
//	////请填入实验参数名，参看分拣结果目录树，按上面的中文写即可；
//	//int para_idx = CSysConf::m_Sort_Data->GetExamParaIdx(type_idx,"10％");
//
//	char str_tmp[MAX_CHN_NAME_LEN];
//	memset(str_tmp,0,MAX_CHN_NAME_LEN);
//
//	//请填入通道名，参看分拣结果目录树，按上面的中文写即可；
//	strcpy(str_tmp,selName.GetBuffer());
//	int chn_idx = CSysConf::m_Sort_Data->GetChnIdx(str_tmp);
//	sprintf(str_tmp,"%d",chn_idx);
//
//	//以下获取分拣文件所有通道信息；
//	//以下获取分拣文件所有通道信息；
//	int chn_num = CSysConf::m_Sort_Data->m_chnNum;
//	int all_data_len = 0;
//	char ** selchnarr = CStrTool::alloc_arr(chn_num,MAX_CHN_NAME_LEN);
//	int idx = 0;
//	//unsigned short idt = 0;
//	for(int i=0;i<chn_num;i++)
//	{
//		CStrTool::CStrcpy(selchnarr[i],CSysConf::m_Sort_Data->m_chnNameArr[idx],MAX_CHN_NAME_LEN);
//	}
//	unsigned char chn_id = (unsigned char)CSysConf::m_Sort_Data->m_chnIDArr[idx];
//
//	//获取分拣序号；
//	int sort_code = CSysConf::m_Sort_Data->m_ExamTypeParaSortCode[type_idx][para_idx];
//	//获取起始终止时间；
//	unsigned long begin_time = CSysConf::m_Sort_Data->m_SortTimeBeginArr[sort_code];
//	unsigned long end_time = CSysConf::m_Sort_Data->m_SortTimeEndArr[sort_code];
//	//获取采样频率；
//	int sam_freq = CSysConf::m_Sort_Data->m_SamFreq;
//	//获取点数；
//	int sec_num = CSysConf::m_Sort_Data->m_SortEndPoint[sort_code] - CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];
//
//	int buffer_len = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan * 3;
//
//	if(f!=0)
//	{
//		CSysConf::m_Sort_Data->DestroyConf();
//		CSysConf::m_Sort_Data->SetConf(CSysConf::m_Sort_Data->m_SortNum,CSysConf::m_Sort_Data->m_SortLen+buffer_len,CSysConf::m_Sort_Data->m_chnNum,CSysConf::m_Sort_Data->m_chnIDArr,selchnarr);
//		/*******************************/
//		CSysConf::m_Sort_Data->m_OriSortNum = CSysConf::m_Sort_Data->m_SortNum;
//		/*******************************/
//		CSysConf::m_Sort_Data->m_SortLen = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan * MAX_SORT_LEN;
//		CSysConf::m_Sort_Data->ReadData(f,sort_code,chn_idx,&begin_time,&end_time);
//		//!!!注意：CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx]为读取结果，共sec_num*data_len点数据；
//		CStrTool::print_xf(CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx],sec_num*data_len);
//	}
//
//	//int i=0;
//	////初始配置文件目录；
//	//CString FilePathName;
//	//CString strFilter="数据分拣文件 (*.SEL) | *.SEL;*.SEL ||";
//	//CFileDialog dlg(TRUE,//open/Save as
//	//	NULL,//Ext added automatically
//	//	NULL,//filename
//	//	OFN_HIDEREADONLY |OFN_FILEMUSTEXIST,
//	//	strFilter);
//	//if(dlg.DoModal()==IDOK)
//	//	FilePathName=dlg.GetPathName();
//	////	FilePathName = "D:\\Work\\ExamDir\\1号机组升压试验.SEL";
//	//if(FilePathName.Right(3)=="SEL")
//	//{
//	//	//	AfxMessageBox(FilePathName);
//	//	char file_path[MAX_CHN_NAME_LEN*2];
//	//	memset(file_path,0,MAX_CHN_NAME_LEN*2);
//	//	char tmpstr[32];
//	//	memset(tmpstr,0,32);
//	//	FILE * f=fopen(FilePathName,"r+b");
//	//	if(f!=0)
//	//	{
//	//		CSysConf::m_Sort_Data->ReadHead(f);
//	//		fclose(f);
//	//	}
//
//	//}
//
//
//	////读取分拣某实验类型参数的通道数据
//	//int data_len = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan;
//	////请填入实验类型名，参看分拣结果目录树，按上面的中文写即可；
//	//int type_idx = CSysConf::m_Sort_Data->GetExamTypeIdx("升负荷");
//	////请填入实验参数名，参看分拣结果目录树，按上面的中文写即可；
//	//int para_idx = CSysConf::m_Sort_Data->GetExamParaIdx(type_idx,"10％");
//
//	//char str_tmp[MAX_CHN_NAME_LEN];
//	//memset(str_tmp,0,MAX_CHN_NAME_LEN);
//
//	//	//请填入通道名，参看分拣结果目录树，按上面的中文写即可；
//	//	strcpy(str_tmp,"备用4振动");
//	//	int chn_idx = CSysConf::m_Sort_Data->GetChnIdx(str_tmp);
//	//	sprintf(str_tmp,"%d",chn_idx);
//
//	//	//以下获取分拣文件所有通道信息；
//	//	int chn_num = CSysConf::m_Sort_Data->m_chnNum;
//	//	int all_data_len = 0;
//	//	char ** selchnarr = CStrTool::alloc_arr(chn_num,MAX_CHN_NAME_LEN);
//	//	int idx = 0;
//	//	unsigned short idt = 0;
//	//	for(i=0;i<chn_num;i++)
//	//	{
//	//		CStrTool::CStrcpy(selchnarr[i],CSysConf::m_Sort_Data->m_chnNameArr[idx],MAX_CHN_NAME_LEN);
//	//	}
//	//	unsigned char chn_id = CSysConf::m_Sort_Data->m_chnIDArr[idx];
//
//	//	//获取分拣序号；
//	//	int sort_code = CSysConf::m_Sort_Data->m_ExamTypeParaSortCode[type_idx][para_idx];
//	//	//获取起始终止时间；
//	//	unsigned long begin_time = CSysConf::m_Sort_Data->m_SortTimeBeginArr[sort_code];
//	//	unsigned long end_time = CSysConf::m_Sort_Data->m_SortTimeEndArr[sort_code];
//	//	//获取采样频率；
//	//	int sam_freq = CSysConf::m_Sort_Data->m_SamFreq;
//	//	//获取点数；
//	//	int sec_num = CSysConf::m_Sort_Data->m_SortEndPoint[sort_code] - CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];
//	//	FILE * f=0;
//	//	int buffer_len = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan * 3;
//	//	f = fopen(FilePathName,"r+b");
//	//	if(f!=0)
//	//	{
//	//		CSysConf::m_Sort_Data->DestroyConf();
//	//		CSysConf::m_Sort_Data->SetConf(CSysConf::m_Sort_Data->m_SortNum,CSysConf::m_Sort_Data->m_SortLen+buffer_len,CSysConf::m_Sort_Data->m_chnNum,CSysConf::m_Sort_Data->m_chnIDArr,selchnarr);
//	//		CSysConf::m_Sort_Data->m_SortLen = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan * MAX_SORT_LEN;
//	//		CSysConf::m_Sort_Data->ReadData(f,sort_code,chn_idx,&begin_time,&end_time);
//	//		//!!!注意：CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx]为读取结果，共sec_num*data_len点数据；
//	//		//C:\\DATA_INFO.txt可以看到数据;
//	//		//CStrTool::print_xf(CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx],sec_num*data_len);
//	//	}
//
//	CStrTool::del_arr(selchnarr,chn_num,1);
//	float * ori_ptr=CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx];
//	
//	vector<float> vData;
//	vData.clear();
//	for(i=0;i<sec_num*data_len;i++)
//	{
//		vData.push_back(ori_ptr[i]);
//	}
//
//	return vData;
//	//
//	//return pChnData;
//}

//消息：点击鼠标右键
void CCustomTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	SetFocus();   
	
	ClientToScreen(&point);   

	//   Get   the   currently   selected   item   
	HTREEITEM   hCurrSel   =   GetSelectedItem();   

	//   Figure   out   which   item   was   right   clicked   
	CPoint   pt(0,   0);   
	::GetCursorPos(&pt);   
	ScreenToClient   (&pt);   

	HTREEITEM   hNewSel=HitTest(pt,   &nFlags);  
	CString str=GetItemText(hNewSel); 
	CString m_SelName = pCMF->fileData->RemoveCoutFromString(str);
	HTREEITEM   m_selectedItem   =   GetSelectedItem(); 

	if(m_selectedItem==GetRootItem()) return;	
	if(NULL   ==   hNewSel)   
	{   
		SelectItem(NULL);   
	}   
	else if(hCurrSel   !=   hNewSel)   
	{   
		SelectItem(hNewSel);   
		SetFocus();   
	} 
	BOOL tFlag;
	vector<CString>::iterator iter;
	for( iter = vectorTreeName.begin(); iter != vectorTreeName.end(); iter++ )
	{
		if(*iter== m_SelName)
		{
			//pCMF->ActivateTabC(1);
			tFlag=TRUE;
			break;
		}
		else
		{	
			//pCMF->ActivateTabC(0);			
			tFlag=FALSE;
		}
	}

	if(hNewSel&&tFlag==TRUE)   
	{   	
		
		CMenu   menu;   
		VERIFY(menu.LoadMenu(IDR_BATCHPROC));
		CMenu*   pPopup   =   menu.GetSubMenu(0);   
		ASSERT(pPopup   !=   NULL);   
		CWnd*   pWndPopupOwner   =   GetTopLevelFrame();
		pPopup->TrackPopupMenu(TPM_LEFTALIGN   |   TPM_RIGHTBUTTON,   point.x,   point.y, pWndPopupOwner); 
		menu.Detach();
	}  
	if(hNewSel&&tFlag!=TRUE)   
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("请选择试验类型！");
	}
	
}




// Drawing only; selection, keyboard navigation and item hit testing stay native.
void CCustomTreeCtrl::OnSkinCustomDraw(NMHDR* header, LRESULT* result)
{
    NMTVCUSTOMDRAW* draw = reinterpret_cast<NMTVCUSTOMDRAW*>(header);
    if (draw->nmcd.dwDrawStage == CDDS_PREPAINT) {
        *result = CDRF_NOTIFYITEMDRAW;
    } else if (draw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
        const BOOL selected = (HTREEITEM)draw->nmcd.dwItemSpec == GetSelectedItem();
        draw->clrText = selected ? HSJAccent() : HSJInk();
        draw->clrTextBk = selected ? RGB(232, 241, 253) : RGB(255, 255, 255);
        if (selected) draw->nmcd.uItemState &= ~(CDIS_SELECTED | CDIS_FOCUS);
        *result = CDRF_DODEFAULT;
    } else {
        *result = CDRF_DODEFAULT;
    }
}
