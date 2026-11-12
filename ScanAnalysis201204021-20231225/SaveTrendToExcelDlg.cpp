// SaveTrendToExcelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "SaveTrendToExcelDlg.h"
#include "MainFrm.h"

// CSaveTrendToExcelDlg 对话框
using namespace std;
IMPLEMENT_DYNAMIC(CSaveTrendToExcelDlg, CHSJDialog)
const int trendCol=33;
CString ExcelTrendHeadArray[trendCol];

/*--------------------------Marshall's Code2011.2.24 10:51--------------*/
extern bool g_bConf97;
extern bool g_bConf95;
extern bool g_bConf100;
/*--------------------------Marshall's Code2011.2.24 10:51--------------*/

CSaveTrendToExcelDlg::CSaveTrendToExcelDlg(CWnd* pParent /*=NULL*/)
: CHSJDialog(CSaveTrendToExcelDlg::IDD, pParent), filePath(""), isGraph(false),
m_application(false), m_workBook(m_application.NewWorkbook()),m_strUnit(""),m_strAcc("")
{

	isHasKeyPhase=FALSE;
	isGraph1=true;


	configPath="";
	m_Bconfig=false;
	m_samp=2048;
	m_count=0;
	m_saveSamp=1;


	for (int i=0;i<100;i++)
	{
		minTem[i]=10000.0;
		maxTem[i]=-10000.0;
	}

	m_start=0.0;
	m_end=0.0;

}

CSaveTrendToExcelDlg::~CSaveTrendToExcelDlg()
{
}

void CSaveTrendToExcelDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_savePath);
	DDX_Control(pDX, IDC_STATIC_MSG, m_msg);

	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_CANCEL);
	DDX_Control(pDX, IDC_FILENAMECMBO, fileNameComb);
	DDX_Control(pDX, IDC_EXAMTYPECOMBO, examTypeComb);
	DDX_Control(pDX, IDC_EXAMPARAMCOMBO, examParamComb);
	DDX_Control(pDX, IDC_EXAMCHNCOMBOY, chnNameCombAcd);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_CHECK1, m_graphChk);
	DDX_Control(pDX, IDC_CHECK2, m_graphChk1);

	DDX_Control(pDX, IDC_EDIT10, m_configPath);
}


BEGIN_MESSAGE_MAP(CSaveTrendToExcelDlg, CHSJDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSaveTrendToExcelDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CSaveTrendToExcelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSaveTrendToExcelDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &CSaveTrendToExcelDlg::OnCbnSelchangeFilenamecmbo)
	ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &CSaveTrendToExcelDlg::OnCbnSelchangeExamtypecombo)
	ON_CBN_SELCHANGE(IDC_EXAMPARAMCOMBO, &CSaveTrendToExcelDlg::OnCbnSelchangeExamparamcombo)
	ON_CBN_SELCHANGE(IDC_EXAMCHNCOMBOY, &CSaveTrendToExcelDlg::OnCbnSelchangeExamchncomboy)
	ON_BN_CLICKED(IDC_CHECK1, &CSaveTrendToExcelDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSaveTrendToExcelDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_ChgRng2, &CSaveTrendToExcelDlg::OnBnClickedChgrng2)
	ON_BN_CLICKED(IDC_CHECK2, &CSaveTrendToExcelDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CSaveTrendToExcelDlg 消息处理程序

void CSaveTrendToExcelDlg::OnBnClickedButton1()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	m_savePath.SetWindowText(savePath);

}

BOOL CSaveTrendToExcelDlg::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_graphChk.SetCheck(false);

	m_graphChk1.SetCheck(true);


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
	
	SetHead();
	OnCbnSelchangeExamchncomboy();
	m_progress.ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSaveTrendToExcelDlg::OnBnClickedOk()
{
	int m_flag=0;
	
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	CString editString;
	m_savePath.GetWindowText(editString);
	if (editString=="" )
	{
		AfxMessageBox("请选择路径");
	}
	else
	{		
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：存储趋势数据报表...");

		m_OK.ShowWindow(FALSE);
		m_CANCEL.ShowWindow(FALSE);

		m_progress.ShowWindow(TRUE);

		int samplFreq;

		map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
		for (;its!=pCMF->fileData->samFreq.end();its++)
		{
			if (its->first==pCMF->FilePathName)
			{
				samplFreq=atoi(its->second);
			}
		}
		int iSheet=0;

		CString filepath,examParam,chnNameAcd,examType;
		CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
		//pCMF->fileData->SetExamTree(filepath);

		fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
		examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
		examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);
		chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);

		vector<CString> examParamName= pCMF->fileData->GetExamParam(filepath,examType);		
		vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
		CString examName=pCMF->fileData->GetExamName(filepath);

		//double mean;
		
		
		vector<CString>::iterator itch=chnName.begin();
		CreateDirectory(savePath+"\\"+examName+"_"+examType,0);
		CreateDirectory(savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析",0);


		DeleteFile(savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析"+"\\"+chnNameAcd+"(参考通道) 趋势表分析时段记录"+".txt");

		FILE * fp;
		fp=fopen(savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析"+"\\"+chnNameAcd+"(参考通道) 趋势表分析时段记录"+".txt","w");
		fprintf(fp, "%s\t\t%s\t\t%s\n","工况参数","分析起始时间(秒)","分析终止时间(秒)");

		CString coutStr;
		coutStr.Format("%d",(int)chnName.size());




		for (;itch<chnName.end();itch++)
		{
			CString  chnAcd[300];
			vector<CString>::iterator itpara=examParamName.begin();
			int pos=0;
			vector<int> chID;
			m_progress.SetPos(0);
			excelCHNMatrix.SetSize((int)examParamName.size(),trendCol-3);
			iSheet++;
			CString idStr;
			idStr.Format("%d",iSheet);
			CNiExcelWorksheet ws(m_workBook.NewWorksheet(iSheet));
			idStr=idStr+"_"+*itch;
			ws.SetName(idStr);
			CNiExcelCellRange titleRange = ws.GetRange("A4:AG4");
			titleRange.FontBold=true;
			titleRange.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
			titleRange.WriteDataFromArray(ExcelTrendHeadArray, 1, trendCol);


			

			int unitTemp;
			unitTemp=-1;
			


		

			for (;itpara<examParamName.end();itpara++)

			{


				unitTemp++;

				
				chnAcd[pos]=*itpara;
				pos++;
				chID.push_back(pos);				
				int save_freq=0;
				vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,*itpara,*itch,&save_freq);


				//默认为整周期
				


			
				int key_Freq=0;
				int nDataLen = pCMF->fileData->GetChnDataLength(filepath,examType,*itpara,*itch);

				vector<float> keyData = pCMF->fileData->GetKeyPhase(
				filepath,
				examType,
				*itpara,
				&key_Freq);


				SetKeyPhase(keyData,key_Freq,save_freq,nDataLen);


				m_xMin=ceil(minTem[unitTemp]*save_freq);
				m_xMax=floor(maxTem[unitTemp]*save_freq);


 /* 
				CString strTemp;
				strTemp.Format("m_xMin:  %d",m_xMin);
				AfxMessageBox(strTemp);


				strTemp.Format("m_xMax:  %d",m_xMax);
				AfxMessageBox(strTemp);
 	 
*/


		        SetChData(chndata,save_freq);

				SetFraqData(NiChData, save_freq);

				CompChnData(NiChData,pos-1);

				
				if (m_flag<examParamComb.GetCount())
				{CString strT1,strT2;
				strT1.Format("%lf",m_start);
				strT2.Format("%lf",m_end);
				fprintf(fp, "%s\t\t%s\t\t%s\n",*itpara,strT1,strT2);
				}
				else
				{
					fclose(fp);
				}

				m_flag++;
 
				

				

				//m_10.SetCellValue(m_start);
				//m_12.SetCellValue(m_end);


				//CString posStr;
				//posStr.Format("%d",pos);
				//CString coutStr;
				//coutStr.Format("%d",(int)examParamName.size()-1);
				//m_msg.SetWindowText("处理通道("+posStr+"/"+coutStr+")： "+*itpara+"_"+*itch);
				m_progress.SetPos (100*pos/((int)examParamName.size()-1));
			}


			//CString strTemp;
			//strTemp.Format("[%d] %s",unitTemp,*itch);

			//AfxMessageBox(*itch);

			m_strUnit=pCMF->fileData->GetChnUnit(filepath,*itch);
			m_strAcc=pCMF->fileData->GetChnUnit(filepath,chnNameAcd);



			CNiExcelCellRange m_1= ws.GetRange("A1:A1");
			//m_1.FontBold=true;
			m_1.SetCellValue("当前通道");

			CNiExcelCellRange m_2 = ws.GetRange("B1:B1");
			m_2.SetCellValue(*itch);

			CNiExcelCellRange m_3 = ws.GetRange("C1:C1");
			//examchnTRange.FontBold=true;
			m_3.SetCellValue(m_strUnit);

			CNiExcelCellRange m_4 = ws.GetRange("D1:D1");
			m_4.SetCellValue("参考通道");


			CNiExcelCellRange m_5 = ws.GetRange("E1:E1");
			//chnNameAcdTRange.FontBold=true;
			m_5.SetCellValue(chnNameAcd);

			CNiExcelCellRange m_6 = ws.GetRange("F1:F1");
			m_6.SetCellValue(m_strAcc);

			CNiExcelCellRange m_7 = ws.GetRange("G1:G1");
			//samplFreqTRange.FontBold=true;
			m_7.SetCellValue("采样频率(HZ)");

			CNiExcelCellRange m_8 = ws.GetRange("H1:H1");
			m_8.SetCellValue(samplFreq);
	
/*

			CNiExcelCellRange m_9 = ws.GetRange("I1:I1");
			m_9.SetCellValue("起始时间(秒)");

			CNiExcelCellRange m_10 = ws.GetRange("J1:J1");



			CNiExcelCellRange m_11 = ws.GetRange("K1:K1");
			m_11.SetCellValue("结束时间(秒)");

			CNiExcelCellRange m_12 = ws.GetRange("L1:L1");

*/			
			CString posStr;
			posStr.Format("%d",iSheet);

			m_msg.SetWindowText("处理通道("+posStr+"/"+coutStr+")： "+(*itch));
			CNiReal64Matrix chIDMatrix(1,(int)examParamName.size());
			for (int p=0;p<(int)chID.size();p++)
			{
				chIDMatrix(0,p)=chID[p];
			}

			CString aStr,tmp;
			tmp.Format("%d",5);
			aStr="A"+tmp+":A";
			tmp.Format("%d",5+examParamName.size());
			aStr=aStr+tmp;
			CNiExcelCellRange cellID = ws.GetRange(aStr);
			cellID.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
			cellID.WriteDataFromArray(chIDMatrix);

			tmp.Format("%d",5);
			aStr="B"+tmp+":B";
			tmp.Format("%d",5+examParamName.size());
			aStr=aStr+tmp;


			CNiExcelCellRange cellChnAcd = ws.GetRange(aStr);
			cellChnAcd.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentLeft;
			cellChnAcd.ColumnWidth=20;
			cellChnAcd.WriteDataFromArray(chnAcd,(int)examParamName.size(),1);

			tmp.Format("%d",5);
			aStr="C"+tmp+":C";
			tmp.Format("%d",5+examParamName.size());
			aStr=aStr+tmp;

			CNiReal64Matrix Xmean((unsigned int)examParamName.size(),1);
			for (int k=0;k<(int)ParamXmean.size();k++)
			{
				Xmean(k,0)=ParamXmean[k];
			}

			CNiExcelCellRange ParamXmeanData = ws.GetRange(aStr);
			ParamXmeanData.WriteDataFromArray(Xmean);


			tmp.Format("%d",5);
			aStr="D"+tmp+":AG";
			tmp.Format("%d",5+examParamName.size());
			aStr=aStr+tmp;

			CNiExcelCellRange cellCHNData = ws.GetRange(aStr);
			cellCHNData.WriteDataFromArray(excelCHNMatrix);

			if (isGraph==TRUE)
			{
				CNiExcelChart chartSheet = ws.AddChart(10, 40*examParamName.size(), 400, 250);
				tmp.Format("%d",4);
				aStr="C"+tmp+":I";
				tmp.Format("%d",4+examParamName.size());
				aStr=aStr+tmp;
				chartSheet.ChartWizard(iSheet, aStr, NiExcelChartWizardGalleryLine, NiExcelColumns, 1, 1, true, *itch+"趋势曲线", "趋势", "通道", "");
				chartSheet.ChartType=NiExcelChartTypesXYScatterLines;
			}

		}


		DeleteFile(savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析"+"\\"+chnNameAcd+"(参考通道) 趋势表"+".xls");
		m_workBook.Save(savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析"+"\\"+chnNameAcd+"(参考通道) 趋势表"+".xls",CNiExcelWorkbook::FileFormatExcel7);
		m_msg.SetWindowText("Excel数据写入完成！");
		
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("趋势分析：完成存储趋势数据报表："+savePath+"\\"+examName+"_"+examType+"\\"+"趋势分析"+"\\"+chnNameAcd+"(参考通道) 趋势表"+".xls");

		m_application.Quit();

		AfxMessageBox("excel文件存储完毕！");
		
		OnOK();
	}
}

void CSaveTrendToExcelDlg::OnBnClickedCancel()
{
	m_application.Quit();
	OnCancel();
}
void CSaveTrendToExcelDlg::OnCbnSelchangeFilenamecmbo()
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
}

void CSaveTrendToExcelDlg::OnCbnSelchangeExamtypecombo()
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

void CSaveTrendToExcelDlg::OnCbnSelchangeExamparamcombo()
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
}
void CSaveTrendToExcelDlg::SetHead()
{
	ExcelTrendHeadArray[0]="序号";
	ExcelTrendHeadArray[1]="参考通道";
	ExcelTrendHeadArray[2]="趋势X值";
	ExcelTrendHeadArray[3]="最小值";
	ExcelTrendHeadArray[4]="最大值";
	ExcelTrendHeadArray[5]="平均值";
	ExcelTrendHeadArray[6]="中间值";
	ExcelTrendHeadArray[7]="有效值";
	//ExcelTrendHeadArray[8]="峰峰值";//加上置信度标记//Marshall  2011.2.24 12:32
	/*--------------------------Marshall's Code2011.2.24 10:53--------------*/
	if(g_bConf97)
		ExcelTrendHeadArray [8] = "97%峰峰值";
	else if(g_bConf95)
		ExcelTrendHeadArray [8] = "95%峰峰值";
	else if(g_bConf100)
		ExcelTrendHeadArray [8] = "100%峰峰值";
	/*--------------------------Marshall's Code2011.2.24 10:53--------------*/

	int t=0;
	for (int k=0;k<8;k++)
	{
		CString str;
		str.Format("%d",k+1);
		t++;
		ExcelTrendHeadArray[8+t]="频率"+str;
		t++;
		ExcelTrendHeadArray[8+t]="幅值"+str;
		t++;
		ExcelTrendHeadArray[8+t]="相位"+str;
	}	

	//ExcelTrendHeadArray[33]="起始时间(秒)";
	//ExcelTrendHeadArray[34]="终止时间(秒)";

}

void CSaveTrendToExcelDlg::SetChData(vector <float> vData,int sFreq)
{

/*
	CNiReal64Vector NiDt((unsigned int)vData.size());
	vector<float>::iterator it2 = vData.begin();
	int k=-1;
	for(; it2 != vData.end(); it2++)
	{
		k++;
		NiDt[k]=*it2;
	}

	NiChData=NiDt;
*/


	CNiReal64Vector NiDt((unsigned int)vData.size());
	vector<float>::iterator it2 = vData.begin();


	int minT=0,maxT=0;
	if(isHasKeyPhase&&isGraph1)
	{
		
		int xMin ;
		int xMax ;

		if (m_xMin>=0&&m_xMax>0&&m_xMin<m_xMax&&m_xMax<vData.size())
		{
			xMin=m_xMin;
			xMax=m_xMax;

		}

		for (int k=0;k<(int)kPhaseX.size();k++)
		{
			if (kPhaseX[k]>=xMin)
			{
				minT=kPhaseX[k];
				break;
			}
		}

		for (int k=0;k<(int)kPhaseX.size();k++)
		{
			if (kPhaseX[k]<=xMax)
			{
				maxT=kPhaseX[k];
			}
			else
				break;
		}

		if (minT==0)
			minT=kPhaseX[0];

 
		
 
		int k = -1;
		int m = -1;

		int nLen = maxT - minT -1;

		m_start=double(minT)/sFreq;
		m_end=double(maxT)/sFreq;


/*
		CString strTemp;
		strTemp.Format("minT:  %d",minT);
		AfxMessageBox(strTemp);


		strTemp.Format("maxT:  %d",maxT);
		AfxMessageBox(strTemp);




 
		strTemp.Format("m_start:  %lf",m_start);
		AfxMessageBox(strTemp);


		strTemp.Format("m_end:  %lf",m_end);
		AfxMessageBox(strTemp);

	
		strTemp.Format("sFreq: %d",sFreq);
		AfxMessageBox(strTemp);
*/ 

		NiChData.SetSize(nLen);

		for(; it2 != vData.end(); it2++)
		{
			k++;
			if(k>=minT && k<maxT-1)

				NiChData[++m]=*it2;

		}

		
	}
	else
	{
		
		int xMin,xMax;
		if (m_xMin>=0&&m_xMax>0&&m_xMin<m_xMax&&m_xMax<vData.size())
		{
			xMin=m_xMin;
			xMax=m_xMax;

		}
		else
		{
			xMin=0;
			xMax=vData.size();
		}

		int nLen = xMax - xMin -1;
		NiChData.SetSize(nLen);


		m_start=double(xMin)/sFreq;
		m_end=double(xMax)/sFreq;

/*
		CString strTemp;
	
		strTemp.Format("m_start  1:  %lf",m_start);
		AfxMessageBox(strTemp);


		strTemp.Format("m_end    1:  %lf",m_end);
		AfxMessageBox(strTemp);


		strTemp.Format("sFreq    1: %d",sFreq);
		AfxMessageBox(strTemp);

*/
		


		int k=-1,m=-1;

		for(; it2 != vData.end(); it2++)
		{
			k++;
			if(k>=xMin && k<xMax-1)
	
				NiChData[++m]=*it2;

		}


	}

}
bool gSecond(const FFTParam & m1, const FFTParam & m2)
{
	return m1.amplitude > m2.amplitude;
}
void CSaveTrendToExcelDlg::SetFraqData(CNiReal64Vector niData, double sample)
{
	int dataSize = niData.GetSize();                         //Number of data points acquired
	CNiReal64Vector realData, imagData;
	realData.SetSize(dataSize);
	imagData.SetSize(dataSize);

	CNiComplexReal64Vector fftData;

	realData+=niData;
	CNiMath::FFT(realData,false,fftData);

	int numPts = dataSize/2;
	double scalingFactor = 1.0/(double)dataSize;
	double deltaFreq = sample*scalingFactor;    

	CNiReal64Vector frequencyData(numPts);

	for( int i=1;i< numPts;i++)
	{
		frequencyData[i] = fftData[i].Magnitude()*scalingFactor*2;
	}

	// Scale DC value
	frequencyData[0] = 0;//fftData[0].Magnitude() * scalingFactor;//剔除直流分量
	int freqRange=(int)(frequencyData.GetSize()*deltaFreq)+1;

	fftVector.clear();
	fftVectorDesc.clear();

/*	for (int p=1;p<numPts;p++)
	{

		FFTParam fp;
		fp.amplitude=frequencyData[p];
		fp.frequency=deltaFreq*p;
		fp.phase=atan2(fftData[p].imag,fftData[p].real);
		fftVector.push_back(fp);
	}*/	

	//std::sort(fftVector.begin(), fftVector.end(), gSecond);////Marshall  2011.2.23 20:11

	/*--------------------------Marshall's Code2011.2.23 20:11--------------*/
	CNiReal64Vector niTmp;
	niTmp.SetSize(numPts-1);

	for (int p=1;p<numPts;p++)////Marshall  2011.2.23 19:5
	{

		FFTParam fp;
		niTmp[p-1]=frequencyData[p];//p 注意 p + 1 //Marshall  2011.2.23 19:7
	}	

	int nIdx[8];
	double dTmp = 0.0;
	for(int m = 0; m < 8; m++)
	{
		for(int n = m + 1; n < numPts -1; n++)
		{
			if(niTmp[m] < niTmp[n])
			{
				dTmp = niTmp[n];
				niTmp[n] = niTmp[m];
				niTmp[m] = dTmp;
			}
		}
	}

	for( m = 0; m < 8; m++)
		for(int n = 0 ; n < numPts - 1; n++)
			if(niTmp[m] == frequencyData[n+1])
			{
				nIdx[m] = n + 1;
				break;
			}


	for (int k=0;k<8;k++)
	{
		FFTParam fp;
		fp.amplitude=frequencyData[nIdx[k]];
		fp.frequency=deltaFreq*nIdx[k];
		fp.phase=atan2(fftData[nIdx[k]].imag,fftData[nIdx[k]].real);
		fftVector.push_back(fp);
	}
	/*--------------------------Marshall's Code2011.2.23 20:11--------------*/
	if (fftVector.size()>=8)
	{
		for (int k=0;k<8;k++)
		{

			if (fftVector[k].phase<0)
				fftVector[k].phase=fftVector[k].phase*180.0/3.1415926535+360.0;
			else
				fftVector[k].phase=fftVector[k].phase*180.0/3.1415926535;

			fftVectorDesc.push_back(fftVector[k]);
		}
	}
}
extern int  _cdecl  Compare(const void* a,const void* b);//函数声明////Marshall  2011.2.24 13:1

void CSaveTrendToExcelDlg::CompChnData(CNiReal64Vector NiChData,int cout)
{

	double minValue, maxValue;
	unsigned int minIndex, maxIndex;
	double ppValue,mean,median,RMS;

	NiChData.MinMax(minValue, minIndex, maxValue, maxIndex);

	//ppValue=maxValue-minValue;//加上置信度算法

	/*--------------------------Marshall's Code2011.2.24 12:59--------------*/
		if(g_bConf97)
	
			ppValue = CNiWaveFraqView::GetConf(NiChData,97);
		else if(g_bConf95)

			ppValue = CNiWaveFraqView::GetConf(NiChData,95);
		else if(g_bConf100)
			ppValue = maxValue-minValue;
	/*--------------------------Marshall's Code2011.2.24 12:59--------------*/

	CNiMath::Mean(NiChData,mean);	
	CNiMath::Median(NiChData,median);
	//CNiMath::RMS(NiChData,RMS);

	CNiReal64Vector m_temp_Data(NiChData.GetSize());
	for (int i=0;i<NiChData.GetSize();i++)
	{
		m_temp_Data[i]=NiChData[i]-mean;
	}
	CNiMath::RMS(m_temp_Data,RMS);   //cxy 20110512

	RMS=RMS*2*1.41421356;

	excelCHNMatrix(cout,0)=minValue;
	excelCHNMatrix(cout,1)=maxValue;
	excelCHNMatrix(cout,2)=mean;
	excelCHNMatrix(cout,3)=median;
	excelCHNMatrix(cout,4)=RMS;
	excelCHNMatrix(cout,5)=ppValue;


	int t=0;
	for (int k=0;k<(int)fftVectorDesc.size();k++)
	{	
		excelCHNMatrix(cout,6+t)=fftVectorDesc[k].frequency;
		t++;
		excelCHNMatrix(cout,6+t)=fftVectorDesc[k].amplitude;
		t++;
		excelCHNMatrix(cout,6+t)=fftVectorDesc[k].phase;
		t++;
	}
}
void CSaveTrendToExcelDlg::OnCbnSelchangeExamchncomboy()
{
	CString filepath,examParam,chnNameAcd,examType;
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(filepath);

	fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	examTypeComb.GetLBText(examTypeComb.GetCurSel(),examType);
	examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);
	chnNameCombAcd.GetLBText(chnNameCombAcd.GetCurSel(),chnNameAcd);

	vector<CString> examParamName= pCMF->fileData->GetExamParam(filepath,examType);
	vector<CString>::iterator itpara=examParamName.begin();
	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,examParam);
	
	double mean;
	int q=0;
	ParamXmean.clear();
	for (;itpara<examParamName.end();itpara++)
	{
		q++;
		vector<CString>::iterator itch=chnName.begin();
		for (;itch<chnName.end();itch++)
		{
			if (*itch==chnNameAcd)
			{
				int save_freq=0;
				vector<float> chndata=pCMF->fileData->GetDataFromFile(filepath,examType,*itpara,chnNameAcd,&save_freq);
				CNiReal64Vector NiDt((unsigned int)chndata.size());
				vector<float>::iterator it2 = chndata.begin();
				int k=-1;
				for(; it2 != chndata.end(); it2++)
				{
					k++;
					NiDt[k]=*it2;
				}
				CNiMath::Mean(NiDt,mean);

				//ParamXmean.push_back(10*q+mean);//////?????10*q////////////////////////////////////////////////////////////////////
			ParamXmean.push_back(mean);
			}
			//chnNameCombAcd.InsertString(0,it[p]);
		}

	}
	
}

void CSaveTrendToExcelDlg::OnBnClickedCheck1()
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



/*---------------------------------------*/
//精确计算FFT
CNiReal64Vector CSaveTrendToExcelDlg::re_time (CNiReal64Vector p1, double samplingFreq)
{

	/*
	int n1, n2;
	double dt1,T;


	n1=p1.GetSize();
	n2=pow(2.0,(int)(log((double)n1)/log(2.0)));
	CNiReal64Vector p2(n2);	
	dt1=1.0/(double)samplingFreq;
	T=(double)p1.GetSize()/(double)samplingFreq;

	int jj;
	double t,dt2;

	dt2=T/(double)n2;
	jj=0;
	for (int i=0;i<n2;i++) 
	{
	t=dt2*(double)i;
	for (int j=jj;j<n1;j++) 
	{
	if (t>=dt1*j && t<=dt1*j+dt1) 
	{
	p2[i]=p1[j]+(p1[j+1]-p1[j])/dt1*(t-dt1*(double)j);
	jj=j;
	break;
	}
	}
	}
	return p2;
	*/

	return p1;
}



//设置键相信号
void CSaveTrendToExcelDlg::SetKeyPhase(std::vector<float> kp,
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

		if (isGraph1)
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
						if(x<nDataLen)
						{
							kPhaseX.push_back(x);
						}
					}			
				}
			}

		}
		else
		{
			isHasKeyPhase=FALSE;

		}


	}


}

void CSaveTrendToExcelDlg::OnBnClickedButton2()
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

		
	}				
}

void CSaveTrendToExcelDlg::OnBnClickedChgrng2()
{
	if (m_Bconfig)
	{
		//AfxMessageBox("true");

		FILE * fp=fopen(configPath,"r");
		int i=0;
		//m_chnList.GetCategoryCount()


		if(NULL != fp)
		{
			for (;i<examParamComb.GetCount();i++)
			{
				fscanf(fp,"%lf\t%lf\n",minTem+i,maxTem+i); 

			}
/*
			CString strTemp;
			strTemp.Format("%d",examParamComb.GetCount());
			AfxMessageBox(strTemp);

*/

			fclose(fp);

		}

	}
	else

	{
		AfxMessageBox("请选择波形时段配置文件!");

	}
}

void CSaveTrendToExcelDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_graphChk1.GetCheck())
	{
		isGraph1=TRUE;
		m_graphChk1.SetCheck(TRUE);
	}
	else
	{
		isGraph1=FALSE;
		m_graphChk1.SetCheck(FALSE);		
	}	
}
