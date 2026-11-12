// SaveCHNToExcel.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "SaveCHNToExcel.h"
#include "NiWaveFraqView.h"
#include "MainFrm.h"
#include <atlimage.h>
//#include "msword.h"
//#include "excel.h"

// CSaveCHNToExcel 对话框
using namespace std;
IMPLEMENT_DYNAMIC(CSaveCHNToExcel, CHSJDialog)
const int col=32;
CString ExcelHeadArray[col];

/*--------------------------Marshall's Code2011.2.24 10:51--------------*/
extern bool g_bConf97;
extern bool g_bConf95;
extern bool g_bConf100;
/*--------------------------Marshall's Code2011.2.24 10:51--------------*/

CSaveCHNToExcel::CSaveCHNToExcel(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CSaveCHNToExcel::IDD, pParent),filePath(""), 
	m_application(false), m_workBook(m_application.NewWorkbook())/*, m_workSheet(m_workBook.GetWorksheet(1))*/
{
	isHasKeyPhase=FALSE;
	configPath="";
	m_Bconfig=false;
	m_samp=2048;
	m_count=0;
	m_saveSamp=1;
	isGraph=true;


	for (int i=0;i<100;i++)
	{
		minTem[i]=10000.0;
		maxTem[i]=-10000.0;
	}

	//m_xMin=10000000000;
	//m_xMax=-10000000000;

	m_start=0.0;
	m_end=0.0;
}

CSaveCHNToExcel::~CSaveCHNToExcel()
{
}

void CSaveCHNToExcel::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_savePath);
	DDX_Control(pDX, IDC_STATIC_MSG, m_msg);
	DDX_Control(pDX, IDC_STATIC_TYPEEXAM, m_examType);
	DDX_Control(pDX, IDC_LIST1, m_List);

	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_CANCEL);
	
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_EDIT10, m_configPath);
	DDX_Control(pDX, IDC_CHECK1, m_graphChk);
}


BEGIN_MESSAGE_MAP(CSaveCHNToExcel, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CSaveCHNToExcel::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CSaveCHNToExcel::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CSaveCHNToExcel::OnBnClickedCancel)
//	ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON2, &CSaveCHNToExcel::OnBnClickedButton2)
ON_BN_CLICKED(IDC_ChgRng2, &CSaveCHNToExcel::OnBnClickedChgrng2)
ON_BN_CLICKED(IDC_CHECK1, &CSaveCHNToExcel::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CSaveCHNToExcel 消息处理程序

BOOL CSaveCHNToExcel::OnInitDialog()
{


	
	CHSJDialog::OnInitDialog();
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_List.SetColumnHeader(_T("参数,150;"));

	for (int k=0;k<(int)paramVector.size();k++)
	{
		m_List.InsertItem(k, paramVector[k]);
		m_List.SetCheck(k,TRUE);
	}

	m_examType.SetWindowText("类型： "+examType);
	m_progress.ShowWindow(FALSE);
	SetHead();

	m_graphChk.SetCheck(true);

	return TRUE;  // return TRUE unless you set the focus to a control
}

/*--------------------------Marshall's Code2011.2.25 19:17--------------*/
//设置键相信号
void CSaveCHNToExcel::SetKeyPhase(std::vector<float> kp,
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

		if (isGraph)
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
/*--------------------------Marshall's Code2011.2.25 19:17--------------*/
void CSaveCHNToExcel::OnBnClickedOk()
{

				//	CString strTemp = examParamStr;//应急
				//int nLen = strTemp.Find(_T("/"));
				//strTemp.Delete(nLen,1);
				//examParamStr = strTemp.GetBuffer(0);//应急

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	CString editString;
	m_savePath.GetWindowText(editString);
	if (editString=="")
	{
		AfxMessageBox("请选择路径");
	}
	else
	{	

		//CNiReal64Vector minTime(3000),maxTime(3000);	

		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：批量存储为Excel文件...");
		m_OK.ShowWindow(FALSE);
		m_CANCEL.ShowWindow(FALSE);
		m_progress.ShowWindow(TRUE);

		/*CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem (IDC_PROGRESS1);*/

		CString s;
		pCMF->fileData->SetExamTree(filePath);
		int samplFreq;

		map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
		for (;its!=pCMF->fileData->samFreq.end();its++)
		{
			if (its->first==pCMF->FilePathName)
			{
				samplFreq=atoi(its->second);
			}
		}
		//m_application.Visible = true;
		//m_application.WindowState = CNiExcelApplication::WindowStateMaximized;
		int iSheet=0;
		CreateDirectory(savePath+"\\"+examName+"_"+examType,0);

		for ( int i = 0; i < m_List.GetItemCount(); i++)
		{
			if (m_List.GetCheck(i))
			{
				s=m_List.GetItemText(i,0);

		        CString strTemp = s;//应急
			/*	int nLen = strTemp.Find(_T("/"));
				strTemp.Delete(nLen,1);
				//s = strTemp;//应急
		*/

				iSheet++;

				CNiExcelWorksheet ws(/*m_workBook.GetWorksheet(iSheet)*/m_workBook.NewWorksheet(iSheet));
				//ws.SetName(examType+"_"+s);

				ws.SetName(examType+"_"+strTemp);

				int pos=0;
				vector<CString> chn=pCMF->fileData->GetChnName(filePath,s);
				vector<int> chID;

				std::vector<CString>::iterator  it=chn.begin();
				int p=pCMF->fileData->GetChnDataLength(filePath,examType,s,*it);		

				CNiExcelCellRange titleRange = ws.GetRange("A4:AF4");
				titleRange.FontBold=true;
				titleRange.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
				titleRange.WriteDataFromArray(ExcelHeadArray, 1, 32);

				CNiExcelCellRange examTypeTRange = ws.GetRange("A1:A1");
				examTypeTRange.FontBold=true;
				examTypeTRange.SetCellValue("实验类型");

				CNiExcelCellRange examTypeRange = ws.GetRange("B1:B1");
				examTypeRange.SetCellValue(examType);

				CNiExcelCellRange examParamTRange = ws.GetRange("C1:C1");
				examParamTRange.FontBold=true;
				examParamTRange.SetCellValue("实验参数");

				CNiExcelCellRange examParamRange = ws.GetRange("D1:D1");
				examParamRange.SetCellValue(s);
				
				CNiExcelCellRange samplFreqTRange = ws.GetRange("E1:E1");
				samplFreqTRange.FontBold=true;
				samplFreqTRange.SetCellValue("频率(HZ)");

				CNiExcelCellRange samplFreqRange = ws.GetRange("F1:F1");
				samplFreqRange.SetCellValue(samplFreq);

				CNiExcelCellRange samplNumTRange = ws.GetRange("G1:G1");
				samplNumTRange.FontBold=true;
				samplNumTRange.SetCellValue("起始时间(秒)");


				CNiExcelCellRange sampleTimeTRange = ws.GetRange("I1:I1");
				sampleTimeTRange.FontBold=true;
				sampleTimeTRange.SetCellValue("终止时间(秒)");
/*
				CNiExcelCellRange samplNumRange = ws.GetRange("H1:H1");
				samplNumRange.SetCellValue(p);



				CNiExcelCellRange sampleTimeRange = ws.GetRange("J1:J1");
				sampleTimeRange.SetCellValue((double)p/(double)samplFreq);
*/		
				excelCHNMatrix.SetSize((int)chn.size(),col-2);
				m_progress.SetPos(0);
				CString  chnStrArray[200];
				CString coutStr;
				coutStr.Format("%d",(int)chn.size());
				for (;it<chn.end();it++)
				{	
					chnStrArray[pos]=*it;
					pos++;
					chID.push_back(pos);
					int save_freq = 0;
					vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examType,s,*it,&save_freq);

						/*--------------------------Marshall's Code2011.2.25 19:8--------------*/
				//默认为整周期//Marshall  2011.2.25 15:0
				int key_Freq=0;
 

 
				vector<float> keyData = pCMF->fileData->GetKeyPhase(
					filePath,
					examType,
					s,
					&key_Freq);

				int nDataLen = pCMF->fileData->GetChnDataLength(filePath,
					examType,s,*it);





				SetKeyPhase(keyData,key_Freq,save_freq,nDataLen);

					/*--------------------------Marshall's Code2011.2.25 19:8--------------*/


				    m_xMin=ceil(minTem[i]*save_freq);
					m_xMax=floor(maxTem[i]*save_freq);
/*
for (int kk=0;kk<kPhaseX.size();kk++)
{
					CString strTemp;
					strTemp.Format("%d:   %lf",kk,kPhaseX[kk]);
					AfxMessageBox(strTemp);
}

*/				



					m_samp=save_freq;
					
										
					SetChData(chnData,samplFreq);
					if ((int)pCMF->fileData->GetChnFftFlag(filePath,*it)==1)
					{
						//SetFraqData(NiChData, samplFreq, TRUE);//频率应该是通道的保存频率//Marshall  2011.2.25 20:36
						SetFraqData(NiChData, save_freq, TRUE);
					}
					else
					{
						//频率应该是通道的保存频率//Marshall  2011.2.25 20:36
						//SetFraqData(NiChData, samplFreq, FALSE);
						SetFraqData(NiChData, save_freq, FALSE);
					}
					
					CompChnData(NiChData,pos-1);		
					CString posStr;
					posStr.Format("%d",pos);
					m_msg.SetWindowText("存储通道（"+posStr+"/"+coutStr+")： "+s+"_"+*it);
					
					//m_nProgress+=pos;
					m_progress.SetPos (100*pos/(int)chn.size());
					
				}
				
				m_msg.SetWindowText("写入Excel数据数据，通道为："+s);

				CNiReal64Matrix chIDMatrix(1,(int)chn.size());
				for (int p=0;p<(int)chID.size();p++)
				{
					chIDMatrix(0,p)=chID[p];
				}

				CString aStr,tmp;
				tmp.Format("%d",5);
				aStr="A"+tmp+":A";
				tmp.Format("%d",5+chn.size());
				aStr=aStr+tmp;
				CNiExcelCellRange cellID = ws.GetRange(aStr);
				cellID.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
				cellID.WriteDataFromArray(chIDMatrix);



				CNiExcelCellRange sampleTimeRange = ws.GetRange("J1:J1");
				sampleTimeRange.SetCellValue(m_end);

				//m_start=m_end-(double)(m_count)/m_saveSamp;


				CNiExcelCellRange samplNumRange = ws.GetRange("H1:H1");
				samplNumRange.SetCellValue(m_start);





				tmp.Format("%d",5);
				aStr="B"+tmp+":B";
				tmp.Format("%d",5+chn.size());
				aStr=aStr+tmp;

				CNiExcelCellRange cellCHN = ws.GetRange(aStr);
				cellCHN.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentLeft;
				cellCHN.ColumnWidth=20;
				cellCHN.WriteDataFromArray(chnStrArray,(int)chn.size(),1);

				tmp.Format("%d",5);
				aStr="C"+tmp+":AF";
				tmp.Format("%d",5+chn.size());
				aStr=aStr+tmp;

				CNiExcelCellRange cellCHNData = ws.GetRange(aStr);
				cellCHNData.WriteDataFromArray(excelCHNMatrix);

			}		




	
		}
		CTime ctm;
		CString timeStr;
		ctm=CTime::GetCurrentTime();
		timeStr=ctm.Format("%Y-%m-%d %H-%M-%S");

		//DeleteFile(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" excel汇总表"+".xls");

		m_workBook.Save(savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" excel汇总表"+timeStr+".xls",CNiExcelWorkbook::FileFormatExcel7);
		m_msg.SetWindowText("Excel数据写入完成！");
	
		m_application.Quit();
		AfxMessageBox("excel文件存储完毕！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：完成批量存储为Excel文件，存储为 "+savePath+"\\"+examName+"_"+examType+"\\"+examName+"_" +examType+" excel汇总表"+timeStr+".xls");
		OnOK();	
	}

}

extern int  _cdecl  Compare(const void* a,const void* b);//函数声明////Marshall  2011.2.24 13:1

void CSaveCHNToExcel::CompChnData(CNiReal64Vector NiChData,int cout)
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
		excelCHNMatrix(cout,6+t) = 0;
		excelCHNMatrix(cout,6+t)=fftVectorDesc[k].frequency;
		t++;

		excelCHNMatrix(cout,6+t) = 0;
		excelCHNMatrix(cout,6+t)=fftVectorDesc[k].amplitude;
		t++;

		excelCHNMatrix(cout,6+t) = 0;
		excelCHNMatrix(cout,6+t)=fftVectorDesc[k].phase;
		t++;
	}
}
bool greatSecond(const FFTParam & m1, const FFTParam & m2)
{
	return m1.amplitude > m2.amplitude;
}

/*--------------------------Marshall's Code2011.2.26 10:1--------------*/
//精确计算FFT
CNiReal64Vector CSaveCHNToExcel::re_time (CNiReal64Vector p1, double samplingFreq)
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
/*--------------------------Marshall's Code2011.2.26 10:1--------------*/

void CSaveCHNToExcel::SetFraqData(CNiReal64Vector niData, double sample,BOOL isfft)
{

	int dataSize = niData.GetSize();                         //Number of data points acquired
	m_count=dataSize;
	m_saveSamp=sample;
	CNiReal64Vector realData, imagData;
	realData.SetSize(dataSize);
	imagData.SetSize(dataSize);

	CNiComplexReal64Vector fftData;

			/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
	/*	FILE* f = fopen("c:\\niData_Excel.txt","w+");

		for(int i = 0; i< niData.GetSize(); i++)
		{

				fprintf(f,"%f\n",niData[i]);

		}
		fclose(f);*/
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/

	if(bRetime)//重采样//Marshall  2011.2.26 18:19
		realData = re_time(niData,sample);
	else
		realData+=niData;
	//不re_time重新采//Marshall  2011.2.26 17:5
	//要将重采样与//Marshall  2011.2.26 17:11
	//realData = re_time(niData,sample);//事实上，应该用re_time重新采//Marshall  2011.2.26 11:23
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
	/*	f = fopen("c:\\realData_Excel.txt","w+");

		for(int i = 0; i< realData.GetSize(); i++)
		{

				fprintf(f,"%f\n",realData[i]);

		}
		fclose(f);*/
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/

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

	/*	for (int p=1;p<numPts;p++)//不用它的排序函数，自己排//Marshall  2011.2.23 19:58
	{	
	FFTParam fp;
	if (isfft)
	{

	fp.amplitude=frequencyData[p];
	fp.frequency=deltaFreq*p;
	fp.phase=atan2(fftData[p].imag,fftData[p].real);
	}
	else
	{
	fp.amplitude=0;
	fp.frequency=0;
	fp.phase=0;
	}
	fftVector.push_back(fp);
	}*/	


	//保存时注释掉下面这句代码//Marshall  2011.2.26 19:48
	/*--------------------------Marshall's Code2011.2.26 19:48--------------*/
	//clock_t beg,end,cost;
	//beg = clock();
	//std::sort(fftVector.begin(), fftVector.end(), greatSecond);
	//end = clock();
	//cost = end - beg;
	/*--------------------------Marshall's Code2011.2.26 19:48--------------*/
	//std::sort(fftVector.begin(), fftVector.end(), greatSecond);//不用它的排序函数，自己排//Marshall  2011.2.23 19:58

	/*--------------------------Marshall's Code2011.2.23 20:8--------------*/
//保存时注释掉下面这句代码//Marshall  2011.2.26 19:48

	CNiReal64Vector niTmp;
	niTmp.SetSize(numPts-1);

	for (int p=1;p<numPts;p++)////Marshall  2011.2.23 19:5
	{

		FFTParam fp;
		niTmp[p-1]=frequencyData[p];//p 注意 p + 1 //Marshall  2011.2.23 19:7
	}	

	int nIdx[8];
	double dTmp = 0.0;

	/*--------------------------Marshall's Code2011.2.26 19:54--------------*/
	/*clock_t beg1,end1;
	beg1 = clock();*/
	/*--------------------------Marshall's Code2011.2.26 19:54--------------*/
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

	/*--------------------------Marshall's Code2011.2.26 19:55--------------*/
	//end1 = clock();
	//cost = end1 - beg1;
	/*--------------------------Marshall's Code2011.2.26 19:55--------------*/
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
		if(isfft)
		{
			fp.amplitude=frequencyData[nIdx[k]];
			fp.frequency=deltaFreq*nIdx[k];
			fp.phase=atan2(fftData[nIdx[k]].imag,fftData[nIdx[k]].real);
			fftVector.push_back(fp);
		}
		else
		{
			fp.amplitude=0;
			fp.frequency=0;
			fp.phase=0;
		}
	}

	/*--------------------------Marshall's Code2011.2.23 20:8--------------*/
	if (fftVector.size()>=8)
	{
		for (int k=0;k<8;k++)
		{
			if (isfft)
			{
				if (fftVector[k].phase<0)
					fftVector[k].phase=fftVector[k].phase*180.0/3.1415926535+360.0;
				else
					fftVector[k].phase=fftVector[k].phase*180.0/3.1415926535;
			}
			else
				fftVector[k].phase=0;

			fftVectorDesc.push_back(fftVector[k]);
		}
	}
}

/*--------------------------Marshall's Code2011.2.25 22:12--------------*/
//void CNiWaveFraqView::FFTProc()
//{
//	qVectorF.clear();
//	int xMax=(int)(m_Graph.Axes.Item("XAxis").GetMaximum());
//	int xMin=(int)(m_Graph.Axes.Item("XAxis").GetMinimum());
//
//	if (!isFullPeriodFFT)
//	{	
//		//此时isFullPeriodFFT = 0//Marshall  2011.2.25 9:46
//		//整周期时处理波形图横坐标最大值、最小值//Marshall  2011.2.25 9:45
//		int minT=0,maxT=0;
//		for (int k=0;k<(int)kPhaseX.size();k++)
//		{
//			if (kPhaseX[k]>=xMin)
//			{
//				minT=kPhaseX[k];
//				break;
//			}
//		}
//
//		for (int k=0;k<(int)kPhaseX.size();k++)
//		{
//			if (kPhaseX[k]<=xMax)
//			{
//				maxT=kPhaseX[k];
//			}
//			else
//				break;
//		}
//		if (minT==maxT)
//		{
//			minT=minKZ;
//			maxT=maxKZ;
//		}
//		if (maxT-minT>=kPhaseX[1]-kPhaseX[0])
//		{
//			xMax=maxT;
//			xMin=minT;
//			m_Graph.Axes.Item("XAxis").SetMinimum(xMin);
//			m_Graph.Axes.Item("XAxis").SetMaximum(xMax);
//		}
//		else
//		{
//			xMax=maxT>minT?maxT:minT;
//			xMin=maxT<minT?maxT:minT;
//			m_Graph.Axes.Item("XAxis").SetMinimum(xMin);
//			m_Graph.Axes.Item("XAxis").SetMaximum(xMax);
//
//		}
//	}
//
//	minKZ=xMin;
//	maxKZ=xMax;
//
//	int round=xMax-xMin-1;
//
//	CNiReal64Vector newData(round);
//	if (round>16)
//	{
//		if (xMin>=0&&xMax<=(int)NiData.GetSize()+1)
//		{
//			for (int i=0;i<round;i++)
//			{
//				newData[i]=NiData[xMin];
//				xMin++;
//			}
//
//		}
//		else
//		{
//			for (int i=0;i<round;i++)
//			{
//				newData[i]=0;				
//			}
//		}
//		SetFraqData(newData,samplingFreq);
/*--------------------------Marshall's Code2011.2.25 22:12--------------*/

void CSaveCHNToExcel::SetChData(vector <float> vData,int sFreq)
{
	CNiReal64Vector NiDt((unsigned int)vData.size());
	vector<float>::iterator it2 = vData.begin();
	//int k=-1;//整周期处理//Marshall  2011.2.25 19:41
	//for(; it2 != vData.end(); it2++)
	//{
	//	k++;
	//	NiDt[k]=*it2;
	//}

	int minT=0,maxT=0;
	//if ()
	/*if(isHasKeyPhase)*///有键相点等周期时才对数据作处理//Marshall  2011.2.26 18:4
	if(isHasKeyPhase)//有键相并且整周期时//Marshall  2011.2.26 18:10
	{
		//int xMin = 0;
		//int xMax = vData.size() - 1;//点的个数索引//Marshall  2011.2.25 20:10

		int xMin ;//= m_xMin;
		int xMax ;//= m_xMax;//点的个数索引//Marshall  2011.2.25 20:10

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

/*
		CString strTemp;
		strTemp.Format("%d",minT);
		AfxMessageBox(strTemp);
*/
		//找到位置//Marshall  2011.2.25 20:11
		int k = -1;
		int m = -1;
		//int nLen = maxT - minT+1;
		int nLen = maxT - minT -1;//注意数据长度,与原程序保持一致//Marshall  2011.2.26 15:17

/*
		CString strTemp;
		strTemp.Format("%lf",double(minT)/m_samp);
		AfxMessageBox(strTemp);

		strTemp.Format("%lf",double(maxT)/m_samp);
		AfxMessageBox(strTemp);
*/

		m_start=double(minT)/m_samp;
		m_end=double(maxT)/m_samp;

	/*	CNiReal64Vector NiTemp;
		NiTemp.SetSize(nLen);*/

		NiChData.SetSize(nLen);

		for(; it2 != vData.end(); it2++)
		{
			k++;
			if(k>=minT && k<maxT-1)//注意数据长度//Marshall  2011.2.26 15:17
				//NiDt[++m]=*it2;
				NiChData[++m]=*it2;
				//NiTemp[++m]=*it2;
		}

		//NiChnData += NiTemp;

		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
		////打印数据，调式时用
		//FILE* f = fopen("c:\\setchnData_Excel.txt","w+");
		//for(int i = 0; i < NiChData.GetSize(); i++)
		//	fprintf(f,"%f\n",NiChData[i]);
		//fclose(f);
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
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


		m_start=double(xMin)/m_samp;
		m_end=double(xMax)/m_samp;
  

/*
		CString strTemp;
		strTemp.Format("%d",xMin);
		AfxMessageBox(strTemp);
		strTemp.Format("%d",xMax);
		AfxMessageBox(strTemp);
*/


	    
		int k=-1,m=-1;//无键相则不取整周期//Marshall  2011.2.25 19:41
	/*	for(; it2 != vData.end(); it2++)
		{
			k++;
			NiDt[k]=*it2;
		}
		NiChData=NiDt;
	*/


		for(; it2 != vData.end(); it2++)
		{
			k++;
			if(k>=xMin && k<xMax-1)//注意数据长度//Marshall  2011.2.26 15:17
				//NiDt[++m]=*it2;
				NiChData[++m]=*it2;
			//NiTemp[++m]=*it2;
		}


		
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
		////打印数据，调式时用
		//FILE* f = fopen("c:\\setchnData_Excel.txt","w+");
		//for(int i = 0; i < NiChData.GetSize(); i++)
		//	fprintf(f,"%f\n",NiChData[i]);
		//fclose(f);
		/*--------------------------Marshall's Code2011.2.25 22:20--------------*/
	}
	
	//NiChData=NiDt;
	//int nL = NiChData.GetSize();//调试时用的代码//Marshall  2011.2.26 15:18
}
void CSaveCHNToExcel::SetHead()
{
	ExcelHeadArray[0]="序号";
	ExcelHeadArray[1]="通道名";
	ExcelHeadArray[2]="最小值";
	ExcelHeadArray[3]="最大值";
	ExcelHeadArray[4]="平均值";
	ExcelHeadArray[5]="中间值";
	ExcelHeadArray[6]="有效值";
	//ExcelHeadArray[7]="峰峰值";//加上置信度标记//Marshall  2011.2.24 12:32

	/*--------------------------Marshall's Code2011.2.24 10:53--------------*/
	if(g_bConf97)
		ExcelHeadArray [7] = "97%峰峰值";
	else if(g_bConf95)
		ExcelHeadArray [7] = "95%峰峰值";
	else if(g_bConf100)
		ExcelHeadArray [7] = "100%峰峰值";
	/*--------------------------Marshall's Code2011.2.24 10:53--------------*/

	int t=0;
	for (int k=0;k<8;k++)
	{
		CString str;
		str.Format("%d",k+1);
		t++;
		ExcelHeadArray[7+t]="频率"+str;
		t++;
		ExcelHeadArray[7+t]="幅值"+str;
		t++;
		ExcelHeadArray[7+t]="相位"+str;
	}			

}
void CSaveCHNToExcel::OnBnClickedButton1()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	m_savePath.SetWindowText(savePath);
}

void CSaveCHNToExcel::OnBnClickedCancel()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("频谱图存储操作：取消批量存储为Excel文件");
	m_application.Quit();
	OnCancel();
}


void CSaveCHNToExcel::OnBnClickedButton2()
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

void CSaveCHNToExcel::OnBnClickedChgrng2()
{
	if (m_Bconfig)
	{
		//AfxMessageBox("true");

		FILE * fp=fopen(configPath,"r");
		int i=0;
		//m_chnList.GetCategoryCount()


		if(NULL != fp)
		{
			for (;i<m_List.GetItemCount();i++)
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

	//		m_con=true;

			fclose(fp);

		}

	}
	else

	{
		AfxMessageBox("请选择波形时段配置文件!");

	}
}

void CSaveCHNToExcel::OnBnClickedCheck1()
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
