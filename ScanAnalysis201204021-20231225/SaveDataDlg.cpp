// SaveDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "SaveDataDlg.h"
#include "MainFrm.h"
#include "ChildFrm.h"

using namespace std;

// SaveDataDlg 对话框

IMPLEMENT_DYNAMIC(SaveDataDlg, CHSJDialog)


SaveDataDlg::SaveDataDlg(CWnd* pParent /*=NULL*/)
: CHSJDialog(SaveDataDlg::IDD, pParent),m_bShowHeader(TRUE),examParam(),
								examName(""),sortFilePath(""),filePath(""),examType("")/*,SaveFileType(1)*/
{

}


SaveDataDlg::~SaveDataDlg()
{
}

void SaveDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_chnList);
	DDX_Control(pDX, IDC_LIST2, m_chnList);
	DDX_Control(pDX, IDC_STATIC_MSG, m_msg);
	DDX_Control(pDX, IDC_EDIT1, m_savePath);
	DDX_Control(pDX, IDC_STATIC_TYPEEXAM, m_examType);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
}


BEGIN_MESSAGE_MAP(SaveDataDlg, CHSJDialog)
	ON_BN_CLICKED(IDOK, &SaveDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &SaveDataDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &SaveDataDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDCANCEL, &SaveDataDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// SaveDataDlg 消息处理程序

void SaveDataDlg::OnBnClickedOk()
{
	CString editString;
	m_savePath.GetWindowText(editString);
	if (editString=="")
	{
		AfxMessageBox("请选择路径");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：批量存储数据到文件...");

		m_ok.ShowWindow(FALSE);
		m_cancel.ShowWindow(FALSE);
		m_progress.ShowWindow(TRUE);

		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		map<CString,map<CString,BOOL>>::iterator chn=cHNnameBool.begin();
		map<CString,BOOL>::iterator ch=chn->second.begin();

		LPCTSTR  pCategory;
		int      iState;
		int      iCatIndex;
		int      iItemIndex;


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
		int ich=1;

		CString str="";
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
		/*
		chn=cHNnameBool.begin();
		CString temp=chn->first;
		CString examParamStr=temp.Left(temp.Find('#',0));	
		*/		
		chn=cHNnameBool.begin();  //cxy  20100713
		int pos=0;
		m_progress.SetPos(0);
		CString coutStr;
		coutStr.Format("%d",cout);
		for(;chn!=cHNnameBool.end();chn++)
		{	





			//cxy 20100713
			//		chn=cHNnameBool.begin();
			CString temp=chn->first;
			CString examParamStr=temp.Left(temp.Find('#',0));
			ch=chn->second.begin();	
			//cxy 20100713

			if (ch->second!=0)
			{

			/*	CString strTemp = examParamStr;//应急
				int nLen = strTemp.Find(_T("/"));
				strTemp.Delete(nLen,1);
				examParamStr = strTemp;//应急
			*/	

				pos++;
				CreateDirectory(savePath+"\\"+examName+"_"+examType,0);
				CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"波形数据",0);

				CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"波形数据"+"\\"+examType+"_"+examParamStr,0);

				int samplFreq;
				map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
				for (;its!=pCMF->fileData->samFreq.end();its++)
				{
					if (its->first==pCMF->FilePathName)
					{
						samplFreq=atoi(its->second);
					}
				}
				int p=pCMF->fileData->GetChnDataLength(filePath,examType,examParamStr,ch->first);
				CNiWaveFraqView* pwave=pCMF->m_PWave;
				pwave=new CNiWaveFraqView;
				pwave->SetChnDataStr(filePath,examName, examType,examParamStr,ch->first);
				int save_freq=0;
				vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examType,examParamStr,ch->first,&save_freq);
				pwave->SetData(chnData,save_freq,0,p);
				pwave->SaveToTxt(savePath+"\\"+examName+"_"+examType+"\\"+"波形数据"+"\\"+examType+"_"+examParamStr+"\\"+ch->first+".chd");				


				delete pwave;
				m_progress.SetPos (100*pos/cout);
				CString posStr;
				posStr.Format("%d",pos);
				m_msg.SetWindowText("存储通道（"+posStr+"/"+coutStr+")： "+examParamStr+ch->first+".chd");



			}

		}
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：完成批量存储数据到文件，路径为 "+savePath+"\\"+examName+"_"+examType+"\\"+"波形数据"+"\\");

		AfxMessageBox("存储完毕!");
		OnCancel();
	}
}

void SaveDataDlg::OnBnClickedButton1()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	m_savePath.SetWindowText(savePath);

}

BOOL SaveDataDlg::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   
	//srand ((unsigned)time (NULL));
	//m_Header.Load (IDB_HEADER);
	//m_Header.SetSingleImage ();
	m_examType.SetWindowText("类型->"+examType);
	int      iItemIndex;
	HRESULT  hr;
	m_progress.ShowWindow(FALSE);

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

	LPCTSTR  pCategory;
	int      iState;
	int      iCatIndex;
	/*   int      iItemIndex;*/
	m_chnList.ShowCategoryItemStates(TRUE);
	for (iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++) {
		pCategory = m_chnList.GetCategoryName( iCatIndex, false );
		for (iItemIndex = 0; iItemIndex < m_chnList.GetCategoryItemCount( pCategory ); iItemIndex++) {
			iState = m_chnList.GetCategoryItemState( pCategory, iItemIndex );
			m_chnList.SetCategoryItemState( pCategory, iItemIndex, 1 );
		}
	}

	for (iCatIndex = 0; iCatIndex < m_chnList.GetCategoryCount(); iCatIndex++) {
		pCategory = m_chnList.GetCategoryName( iCatIndex, false );
		m_chnList.SetCategoryState( pCategory, FALSE );
	}

	m_chnList.SetCurSel( 0 );


	return TRUE;  // return TRUE unless you set the focus to a control
}

//
//void SaveDataDlg::OnBnClickedButton2()
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

//map<CString,BOOL> SaveDataDlg::SetCHNnameBool(CString chnName,BOOL isSel)
//{
//	cHNnameBool.insert(make_pair(chnName,isSel));
//	return cHNnameBool;
//}

map<CString,map<CString,BOOL>> SaveDataDlg::SetCHNnameBool(CString examParam,CString chnName,BOOL isSel)
{
	map<CString,BOOL> chn;
	chn.insert(make_pair(chnName,isSel));
	cHNnameBool.insert(make_pair(examParam,chn));
	return cHNnameBool;
}

vector<CString> SaveDataDlg::SetExamParam(CString paramName)
{
	examParam.push_back(paramName);
	return examParam;
}
void SaveDataDlg::OnBnClickedCancel()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消批量存储数据到文件");
	OnCancel();
}
