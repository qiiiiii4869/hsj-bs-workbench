// efficiency.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "efficiency.h"
#include "MainFrm.h"

// CEfficiency 对话框
using namespace std;

IMPLEMENT_DYNAMIC(CEfficiency, CHSJDialog)

CEfficiency::CEfficiency(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CEfficiency::IDD, pParent),diffElevation(0.0),density(1000.0),gravityAcc(9.83),elecEfficiency(0.985),FCoefficient(150.0),
	vInArea(100.0),vOutArea(350.0),isDWhead(TRUE),factorQ(1.0),factorS(1.0)
{

}

CEfficiency::~CEfficiency()
{
}

void CEfficiency::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Density, m_density);
	DDX_Control(pDX, IDC_gravityAcc, m_gravityAcc);
	DDX_Control(pDX, IDC_elecEfficiency, m_elecEfficiency);
	DDX_Control(pDX, IDC_inPressure, m_inPressure);
	DDX_Control(pDX, IDC_outPressure, m_outPressure);
	DDX_Control(pDX, IDC_isWHead, m_isWHead);
	DDX_Control(pDX, IDC_diffElevation, m_diffElevation);
	DDX_Control(pDX, IDC_activePower, m_activePower);
	DDX_Control(pDX, IDC_Flowmeter, m_Flowmeter);
	DDX_Control(pDX, IDC_FCoefficient, m_FCoefficient);
	DDX_Control(pDX, IDC_vInArea, m_vInArea);
	DDX_Control(pDX, IDC_vOutArea, m_vOutArea);
	DDX_Control(pDX, IDC_DiffPressSensor, m_DiffPressSensor);
	DDX_Control(pDX, IDC_highPress, m_highPress);
	DDX_Control(pDX, IDC_lowPress, m_lowPress);
	DDX_Control(pDX, IDC_isFlowmeter, m_isFlowmeter);
	DDX_Control(pDX, IDC_isVolutePres, m_isVolutePres);
	DDX_Control(pDX, IDC_isDiffPressSensor, m_isDiffPressSensor);
	DDX_Control(pDX, IDC_isPressSensor, m_isPressSensor);

	DDX_Control(pDX, IDC_FILENAMECMBO, m_fileNameCmbo);
	DDX_Control(pDX, IDC_EXAMTYPECOMBO, m_examTypeComb);

	DDX_Text(pDX, IDC_Density, density);
	DDX_Text(pDX, IDC_gravityAcc, gravityAcc);
	DDX_Text(pDX, IDC_elecEfficiency, elecEfficiency);
	DDX_Text(pDX, IDC_diffElevation, diffElevation);
	DDX_Text(pDX, IDC_FCoefficient, FCoefficient);
	DDX_Text(pDX, IDC_vInArea, vInArea);
	DDX_Text(pDX, IDC_vOutArea, vOutArea);
	DDX_Text(pDX, IDC_FactorS, factorS);
	DDX_Text(pDX, IDC_FactorQ, factorQ);
	DDX_Control(pDX, IDC_MSG, m_msg);

	DDV_MinMaxDouble(pDX,density,0.0,10e12);
	DDV_MinMaxDouble(pDX,gravityAcc,0.0,10e12);
	DDV_MinMaxDouble(pDX,elecEfficiency,0.0,10e12);
	DDV_MinMaxDouble(pDX,diffElevation,0.0,10e12);
	DDV_MinMaxDouble(pDX,FCoefficient,0.0,10e12);
	DDV_MinMaxDouble(pDX,vInArea,0.0,10e12);
	DDV_MinMaxDouble(pDX,vOutArea,0.0,10e12);
	DDV_MinMaxDouble(pDX,factorS,0.0,10e12);
	DDV_MinMaxDouble(pDX,factorQ,0.0,10e12);

	DDX_Control(pDX, IDC_SHeaderCHK, m_SheaderChk);
	DDX_Control(pDX, IDC_SHeaderDiffPress, m_SHeaderDiffPress);
	DDX_Control(pDX, IDC_FactorS, m_factorS);
	DDX_Control(pDX, IDC_FactorQ, m_factorQ);
}


BEGIN_MESSAGE_MAP(CEfficiency, CHSJDialog)
	ON_BN_CLICKED(IDC_SaveToText, &CEfficiency::OnBnClickedSavetotext)
	ON_BN_CLICKED(IDC_isFlowmeter, &CEfficiency::OnBnClickedisflowmeter)
	ON_BN_CLICKED(IDC_isVolutePres, &CEfficiency::OnBnClickedisvolutepres)
	ON_BN_CLICKED(IDC_isDiffPressSensor, &CEfficiency::OnBnClickedisdiffpresssensor)
	ON_BN_CLICKED(IDC_isPressSensor, &CEfficiency::OnBnClickedispresssensor)
	ON_CBN_SELCHANGE(IDC_FILENAMECMBO, &CEfficiency::OnCbnSelchangeFilenamecmbo)
	ON_CBN_SELCHANGE(IDC_EXAMTYPECOMBO, &CEfficiency::OnCbnSelchangeExamtypecombo)
	ON_BN_CLICKED(IDC_SaveToExcel, &CEfficiency::OnBnClickedSavetoexcel)
	ON_BN_CLICKED(IDC_isWHead, &CEfficiency::OnBnClickediswhead)
	ON_BN_CLICKED(IDCANCEL, &CEfficiency::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SHeaderCHK, &CEfficiency::OnBnClickedSheaderchk)
END_MESSAGE_MAP()


// CEfficiency 消息处理程序

void CEfficiency::GetEfficiencyData()
{
	if (!UpdateData(TRUE))
	{
		return;
	}

	//UpdateData(TRUE);
	inPressureM.clear();
	outPressureM.clear();
	activePowerM.clear();
	FlowmeterM.clear();
	DiffPressSensorM.clear();
	highPressM.clear();
	lowPressM.clear();
	sWaterHeadM.clear();
	sWaterHeadDPM.clear();
	dWaterHeadM.clear();
	WaterHeadM.clear();

	efficiencyResult.clear();

	CString inPressureStr,outPressureStr,sWaterHeadDPStr,activePowerStr,FlowmeterStr,DiffPressSensorStr,highPressStr,lowPressStr;

	m_inPressure.GetLBText(m_inPressure.GetCurSel(),inPressureStr);
	m_outPressure.GetLBText(m_outPressure.GetCurSel(),outPressureStr);
	m_SHeaderDiffPress.GetLBText(m_SHeaderDiffPress.GetCurSel(),sWaterHeadDPStr);
	m_activePower.GetLBText(m_activePower.GetCurSel(),activePowerStr);
	m_Flowmeter.GetLBText(m_Flowmeter.GetCurSel(),FlowmeterStr);
	m_DiffPressSensor.GetLBText(m_DiffPressSensor.GetCurSel(),DiffPressSensorStr);
	m_highPress.GetLBText(m_highPress.GetCurSel(),highPressStr);
	m_lowPress.GetLBText(m_lowPress.GetCurSel(),lowPressStr);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString filepath,examType;
	m_fileNameCmbo.GetLBText(m_fileNameCmbo.GetCurSel(),filepath);
	m_examTypeComb.GetLBText(m_examTypeComb.GetCurSel(),examType);
	pCMF->fileData->SetExamTree(filepath);
	vector<CString> examParam=pCMF->fileData->GetExamParam(filepath,examType);
	vector<CString>::iterator itT = examParam.begin();
	int save_freq=0;
	for (;itT!=examParam.end();itT++)
	{
		double inPressure=0.0,outPressure=0.0,activePower=0.0,Flowmeter=0.0,
			DiffPressSensor=0.0,highPress=0.0,lowPress=0.0,
			waterHead=0.0,sWaterHead=0.0,dWaterHead=0.0,sWaterHeadDP=0.0;

		activePower=ComputParams(pCMF->fileData->GetDataFromFile(filepath,examType,*itT,activePowerStr,&save_freq));		
		inPressure=ComputParams(pCMF->fileData->GetDataFromFile(filepath,examType,*itT,inPressureStr,&save_freq));
		outPressure=ComputParams(pCMF->fileData->GetDataFromFile(filepath,examType,*itT,outPressureStr,&save_freq));
		sWaterHeadDP=ComputParams(pCMF->fileData->GetDataFromFile(filepath,examType,*itT,sWaterHeadDPStr,&save_freq));
		if (m_SheaderChk.GetCheck())
		{
			sWaterHead=(sWaterHeadDP+diffElevation)*factorS;
		}
		else
		{
			sWaterHead=(abs(inPressure-outPressure)/(density*gravityAcc)+diffElevation)*factorS;
		}



		activePowerM.insert(make_pair(*itT,activePower));		
		inPressureM.insert(make_pair(*itT,inPressure));
		outPressureM.insert(make_pair(*itT,outPressure));
		sWaterHeadM.insert(make_pair(*itT,sWaterHead));
		sWaterHeadDPM.insert(make_pair(*itT,sWaterHeadDP));
		WaterTurbineForceM.insert(make_pair(*itT,activePower/elecEfficiency));



		if (m_isFlowmeter.GetCheck())
		{
			Flowmeter=ComputParams(pCMF->fileData->GetDataFromFile(filepath,examType,*itT,FlowmeterStr,&save_freq));
			FlowmeterM.insert(make_pair(*itT,Flowmeter));			
		}

		else if (m_isVolutePres.GetCheck()&&m_isDiffPressSensor.GetCheck())
		{
			DiffPressSensor=ComputParams(pCMF->fileData->GetDataFromFile(filepath,examType,*itT,DiffPressSensorStr,&save_freq));

			Flowmeter=FCoefficient*sqrt(abs(DiffPressSensor))*factorQ;
			FlowmeterM.insert(make_pair(*itT,Flowmeter));			
		}

		else 
		{
			highPress=ComputParams(pCMF->fileData->GetDataFromFile(filepath,examType,*itT,highPressStr,&save_freq));
			lowPress=ComputParams(pCMF->fileData->GetDataFromFile(filepath,examType,*itT,lowPressStr,&save_freq));

			Flowmeter=FCoefficient*sqrt(abs(highPress-lowPress))*factorQ;
			FlowmeterM.insert(make_pair(*itT,Flowmeter));
		}

		if (m_isWHead.GetCheck())
		{
			isDWhead=TRUE;
			dWaterHead=((Flowmeter*Flowmeter)/(vInArea*vInArea)-(Flowmeter*Flowmeter)/(vOutArea*vOutArea))/(2*gravityAcc);
			dWaterHeadM.insert(make_pair(*itT,dWaterHead));
			waterHead=sWaterHead+dWaterHead;	
		}

		else 
		{
			isDWhead=FALSE;
			waterHead=sWaterHead;	
		}		
		WaterHeadM.insert(make_pair(*itT,waterHead));
		double efficiency=abs((activePower/elecEfficiency)/(density*gravityAcc*Flowmeter*waterHead));
		efficiencyResult.insert(make_pair(*itT,efficiency));
		m_msg.SetWindowText("数据计算中...");
	}
}
void CEfficiency::OnBnClickedSavetotext()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("效率分析：存储为文本数据...");
	GetEfficiencyData();
	SaveToText() ;
	//OnOK();
}
void CEfficiency::SaveToExcel() 
{
	m_msg.SetWindowText("Excel数据写入...");
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	CNiExcelApplication m_application(false);
	CNiExcelWorkbook m_workBook=m_application.NewWorkbook();

	CString savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if(savePath!="")
	{
		if (savePath!="")
		{

			if (isDWhead)
			{
				CString filepath,examType;
				m_fileNameCmbo.GetLBText(m_fileNameCmbo.GetCurSel(),filepath);
				m_examTypeComb.GetLBText(m_examTypeComb.GetCurSel(),examType);

				CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
				pCMF->fileData->SetExamTree(filepath);
				vector<CString> examParam=pCMF->fileData->GetExamParam(filepath,examType);
				vector<CString>::iterator itT = examParam.begin();


				CreateDirectory(savePath+"\\"+"效率分析",0);
				//DeleteFile(savePath+"\\"+"效率分析"+"\\"+"效率分析结果.xls");

				CNiExcelWorksheet ws(/*m_workBook.GetWorksheet(iSheet)*/m_workBook.NewWorksheet(1));
				ws.SetName("效率分析结果");

				CString ExcelHeadArray[9],IDArray[100],RecordName[100];
				ExcelHeadArray[0]="序号";
				ExcelHeadArray[1]="记录名称";
				ExcelHeadArray[2]="有功功率";
				ExcelHeadArray[3]="水机出力";
				ExcelHeadArray[4]="静水头";
				ExcelHeadArray[5]="动水头";
				ExcelHeadArray[6]="工作水头";
				ExcelHeadArray[7]="流量";
				ExcelHeadArray[8]="效率";


				CNiExcelCellRange titleRange = ws.GetRange("A1:I1");
				titleRange.FontBold=true;
				titleRange.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
				titleRange.WriteDataFromArray(ExcelHeadArray,1,9);

				CNiReal64Matrix excelEfficiencyDataMatrix;
				excelEfficiencyDataMatrix.SetSize((int)examParam.size(),7);
				int k=0;
				for (;itT!=examParam.end();itT++)
				{
					k++;
					excelEfficiencyDataMatrix(k-1,0)=activePowerM[*itT];
					excelEfficiencyDataMatrix(k-1,1)=WaterTurbineForceM[*itT];
					excelEfficiencyDataMatrix(k-1,2)=sWaterHeadM[*itT];
					excelEfficiencyDataMatrix(k-1,3)=dWaterHeadM[*itT];
					excelEfficiencyDataMatrix(k-1,4)=WaterHeadM[*itT];
					excelEfficiencyDataMatrix(k-1,5)=FlowmeterM[*itT];
					excelEfficiencyDataMatrix(k-1,6)=efficiencyResult[*itT];

					CString str;
					str.Format("%d",k);
					IDArray[k-1]=str;
					RecordName[k-1]=*itT;
				}	

				CString aStr,tmp;
				tmp.Format("%d",2);
				aStr="A"+tmp+":A";
				tmp.Format("%d",2+(int)examParam.size());
				aStr=aStr+tmp;

				CNiExcelCellRange IDRange = ws.GetRange(aStr);
				IDRange.WriteDataFromArray(IDArray,1,(int)examParam.size());

				tmp.Format("%d",2);
				aStr="B"+tmp+":B";
				tmp.Format("%d",2+(int)examParam.size());
				aStr=aStr+tmp;
				CNiExcelCellRange RecordNRange = ws.GetRange(aStr);
				RecordNRange.WriteDataFromArray(RecordName,1,(int)examParam.size());

				tmp.Format("%d",2);
				aStr="C"+tmp+":I";
				tmp.Format("%d",2+(int)examParam.size());
				aStr=aStr+tmp;
				CNiExcelCellRange cellCHNData = ws.GetRange(aStr);
				cellCHNData.WriteDataFromArray(excelEfficiencyDataMatrix);

				CNiExcelChart chartSheet = ws.AddChart(10, 40*(int)examParam.size(), 400, 250);
				CString tmp1,tmp2,aStr2;
				tmp1.Format("%d",1);
				aStr="C"+tmp1+":C";
				aStr2="I"+tmp1+":I";
				tmp2.Format("%d",1+examParam.size());
				aStr=aStr+tmp2+","+aStr2+tmp2;

				chartSheet.ChartWizard(1, aStr, NiExcelChartWizardGalleryLine, NiExcelColumns, 1, 1, true, "效率分析", "有功", "效率", "");
				chartSheet.ChartType=NiExcelChartTypesXYScatterLines;

				CTime tm;
				CString timeStr;
				tm=CTime::GetCurrentTime();
				timeStr=tm.Format("%Y-%m-%d %H-%M-%S");

				m_workBook.Save(savePath+"\\"+"效率分析"+"\\"+"效率分析结果 "+timeStr+".xls",CNiExcelWorkbook::FileFormatExcel7);
				m_msg.SetWindowText("Excel数据写入完成！");
				((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("效率分析：存储完成,存储文件为  "+savePath+"\\"+"效率分析"+"\\"+"效率分析结果 "+timeStr+".xls");

			}
			else
			{
				CString filepath,examType;
				m_fileNameCmbo.GetLBText(m_fileNameCmbo.GetCurSel(),filepath);
				m_examTypeComb.GetLBText(m_examTypeComb.GetCurSel(),examType);

				CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
				pCMF->fileData->SetExamTree(filepath);
				vector<CString> examParam=pCMF->fileData->GetExamParam(filepath,examType);
				vector<CString>::iterator itT = examParam.begin();


				CreateDirectory(savePath+"\\"+"效率分析",0);
				//DeleteFile(savePath+"\\"+"效率分析"+"\\"+"效率分析结果.xls");

				CNiExcelWorksheet ws(/*m_workBook.GetWorksheet(iSheet)*/m_workBook.NewWorksheet(1));
				ws.SetName("效率分析结果");

				CString ExcelHeadArray[8],IDArray[100],RecordName[100];
				ExcelHeadArray[0]="序号";
				ExcelHeadArray[1]="记录名称";
				ExcelHeadArray[2]="有功功率";
				ExcelHeadArray[3]="水机出力";
				ExcelHeadArray[4]="静水头";
				ExcelHeadArray[5]="工作水头";
				ExcelHeadArray[6]="流量";
				ExcelHeadArray[7]="效率";


				CNiExcelCellRange titleRange = ws.GetRange("A1:H1");
				titleRange.FontBold=true;
				titleRange.HorizontalAlignment=CNiExcelCellRange::HorizontalAlignmentCenter;
				titleRange.WriteDataFromArray(ExcelHeadArray,1,8);

				CNiReal64Matrix excelEfficiencyDataMatrix;
				excelEfficiencyDataMatrix.SetSize((int)examParam.size(),6);
				int k=0;
				for (;itT!=examParam.end();itT++)
				{
					k++;
					excelEfficiencyDataMatrix(k-1,0)=activePowerM[*itT];
					excelEfficiencyDataMatrix(k-1,1)=WaterTurbineForceM[*itT];
					excelEfficiencyDataMatrix(k-1,2)=sWaterHeadM[*itT];
					excelEfficiencyDataMatrix(k-1,3)=WaterHeadM[*itT];
					excelEfficiencyDataMatrix(k-1,4)=FlowmeterM[*itT];
					excelEfficiencyDataMatrix(k-1,5)=efficiencyResult[*itT];

					CString str;
					str.Format("%d",k);
					IDArray[k-1]=str;
					RecordName[k-1]=*itT;
				}	

				CString aStr,tmp;
				tmp.Format("%d",2);
				aStr="A"+tmp+":A";
				tmp.Format("%d",2+(int)examParam.size());
				aStr=aStr+tmp;

				CNiExcelCellRange IDRange = ws.GetRange(aStr);
				IDRange.WriteDataFromArray(IDArray,1,(int)examParam.size());

				tmp.Format("%d",2);
				aStr="B"+tmp+":B";
				tmp.Format("%d",2+(int)examParam.size());
				aStr=aStr+tmp;
				CNiExcelCellRange RecordNRange = ws.GetRange(aStr);
				RecordNRange.WriteDataFromArray(RecordName,1,(int)examParam.size());

				tmp.Format("%d",2);
				aStr="C"+tmp+":H";
				tmp.Format("%d",2+(int)examParam.size());
				aStr=aStr+tmp;
				CNiExcelCellRange cellCHNData = ws.GetRange(aStr);
				cellCHNData.WriteDataFromArray(excelEfficiencyDataMatrix);

				CNiExcelChart chartSheet = ws.AddChart(10, 40*(int)examParam.size(), 400, 250);
				CString tmp1,tmp2,aStr2;
				tmp1.Format("%d",1);
				aStr="C"+tmp1+":C";
				aStr2="H"+tmp1+":H";
				tmp2.Format("%d",1+examParam.size());
				aStr=aStr+tmp2+","+aStr2+tmp2;

				chartSheet.ChartWizard(1, aStr, NiExcelChartWizardGalleryLine, NiExcelColumns, 1, 1, true, "效率分析", "有功", "效率", "");
				chartSheet.ChartType=NiExcelChartTypesXYScatterLines;

				CTime tm;
				CString timeStr;
				tm=CTime::GetCurrentTime();
				timeStr=tm.Format("%Y-%m-%d %H-%M-%S");

				m_workBook.Save(savePath+"\\"+"效率分析"+"\\"+"效率分析结果 "+timeStr+".xls",CNiExcelWorkbook::FileFormatExcel7);
				m_msg.SetWindowText("Excel数据写入完成！");
				((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("效率分析：存储完成,存储文件为  "+savePath+"\\"+"效率分析"+"\\"+"效率分析结果 "+timeStr+".xls");

			}


		}
		
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("效率分析：取消存储为excel报表");
		m_msg.SetWindowText("");
	}
	m_application.Quit();


}

void CEfficiency::SaveToText() 
{
	m_msg.SetWindowText("数据写入...");

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath!="")
	{
		if (savePath!="")
		{
			//m_savePath.SetWindowText(savePath);
			CString filepath,examType;
			m_fileNameCmbo.GetLBText(m_fileNameCmbo.GetCurSel(),filepath);
			m_examTypeComb.GetLBText(m_examTypeComb.GetCurSel(),examType);

			CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
			vector<CString> examParam=pCMF->fileData->GetExamParam(filepath,examType);
			vector<CString>::iterator itT = examParam.begin();


			CreateDirectory(savePath+"\\"+"效率分析",0);
			//DeleteFile(savePath+"\\"+"效率分析"+"\\"+"效率分析结果.txt");
			CTime tm;
			CString timeStr;
			tm=CTime::GetCurrentTime();
			timeStr=tm.Format("%Y-%m-%d %H-%M-%S");

			FILE	*fp = 0;
			fp = fopen(savePath+"\\"+"效率分析"+"\\"+"效率分析结果 "+timeStr+".txt","w");
			if(fp==0)return;

			if (isDWhead)
			{
				fprintf(fp,"%s\t%s\t%s\t%s\t%s\t\t%s\t\t%s\t%s\t\t%s\n","序号","记录名称","有功功率","水机出力","静水头","动水头","工作水头","流量","效率");
				int k=0;
				for (;itT!=examParam.end();itT++)
				{
					k++;
					fprintf(fp,"%d\t%s\t\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
						k,*itT,activePowerM[*itT],WaterTurbineForceM[*itT],sWaterHeadM[*itT],dWaterHeadM[*itT],WaterHeadM[*itT],FlowmeterM[*itT],efficiencyResult[*itT]);
				}
			}
			else
			{
				fprintf(fp,"%s\t%s\t%s\t%s\t%s\t\t%s\t%s\t\t%s\n","序号","记录名称","有功功率","水机出力","静水头","工作水头","流量","效率");
				int k=0;
				for (;itT!=examParam.end();itT++)
				{
					k++;
					fprintf(fp,"%d\t%s\t\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
						k,*itT,activePowerM[*itT],WaterTurbineForceM[*itT],sWaterHeadM[*itT],WaterHeadM[*itT],FlowmeterM[*itT],efficiencyResult[*itT]);
				}

			}


			fseek(fp,0,SEEK_END);
			fclose(fp);
			AfxMessageBox("数据写入完成!");
			m_msg.SetWindowText("数据写入完成！");

			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("效率分析：存储完成,存储文件为  "+savePath+"\\"+"效率分析"+"\\"+"效率分析结果 "+timeStr+".txt");
		}
		//m_application.Quit();
	} 
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("效率分析：取消存储为文本数据");
		m_msg.SetWindowText("");

	}

}

double CEfficiency::ComputParams(vector<float> chnData)
{
	CNiReal64Vector NiDt((unsigned int)chnData.size());
	vector<float>::iterator it2 = chnData.begin();
	int k=-1;
	for(; it2 != chnData.end(); it2++)
	{
		k++;
		NiDt[k]=*it2;
	}
	double dRelt;
	CNiMath::Mean(NiDt,dRelt);
	return dRelt;
}

void CEfficiency::OnBnClickedSavetoexcel()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("效率分析：存储为excel报表...");
	GetEfficiencyData();
	SaveToExcel() ;

	//OnOK();
}

BOOL CEfficiency::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	CheckRadioButton(IDC_isFlowmeter,IDC_isVolutePres,IDC_isFlowmeter);
	CheckRadioButton(IDC_isDiffPressSensor,IDC_isPressSensor,IDC_isDiffPressSensor);
	
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	map<int,CString>::iterator fullPath=pCMF->fileData->fullPathName.begin();
	for (;fullPath!=pCMF->fileData->fullPathName.end();fullPath++)
	{
		m_fileNameCmbo.InsertString(0,fullPath->second);		
	}
	m_fileNameCmbo.SetCurSel(0);
	CString filepath;

	m_fileNameCmbo.GetLBText(m_fileNameCmbo.GetCurSel(),filepath);
	pCMF->fileData->SetExamTree(filepath);

	CString examName,examStr,examParam;
	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(pCMF->FilePathName);
	examName=iterE->second;

	vector<CString> examType= pCMF->fileData->GetExamType(filepath,examName);
	vector<CString>::iterator it=examType.begin();
	m_examTypeComb.InsertString(0,*it);
	m_examTypeComb.SetCurSel(0);

	m_examTypeComb.GetLBText(m_examTypeComb.GetCurSel(),examStr);

	vector<CString> paramType= pCMF->fileData->GetExamParam(filepath,examStr);
	vector<CString>::iterator itT=paramType.begin();

	//for (int p=(int)paramType.size()-1;p>=0;p--)
	//{
	//	examParamComb.InsertString(0,itT[p]);
	//}
	//examParamComb.SetCurSel(0);

	//fileNameComb.GetLBText(fileNameComb.GetCurSel(),filepath);
	//examParamComb.GetLBText(examParamComb.GetCurSel(),examParam);

	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,itT[0]);
	vector<CString>::iterator itc=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,itc[p])==1)
		{
			m_inPressure.InsertString(0,itc[p]);
			m_outPressure.InsertString(0,itc[p]);
			m_SHeaderDiffPress.InsertString(0,itc[p]);
			m_activePower.InsertString(0,itc[p]);
			m_Flowmeter.InsertString(0,itc[p]);
			m_DiffPressSensor.InsertString(0,itc[p]);
			m_highPress.InsertString(0,itc[p]);
			m_lowPress.InsertString(0,itc[p]);
		}

	}
	m_inPressure.SetCurSel(0);
	m_outPressure.SetCurSel(0);
	m_SHeaderDiffPress.SetCurSel(0);
	m_activePower.SetCurSel(0);
	m_Flowmeter.SetCurSel(0);
	m_DiffPressSensor.SetCurSel(0);
	m_highPress.SetCurSel(0);
	m_lowPress.SetCurSel(0);

	m_density.EnableWindow(FALSE);
	m_gravityAcc.EnableWindow(FALSE);
	m_elecEfficiency.EnableWindow(FALSE);
	m_inPressure.EnableWindow(FALSE);
	m_outPressure.EnableWindow(FALSE);
	m_isWHead.EnableWindow(FALSE);
	m_diffElevation.EnableWindow(FALSE);
	m_activePower.EnableWindow(FALSE);
	m_Flowmeter.EnableWindow(FALSE);
	m_FCoefficient.EnableWindow(FALSE);
	m_vInArea.EnableWindow(FALSE);
	m_vOutArea.EnableWindow(FALSE);
	m_DiffPressSensor.EnableWindow(FALSE);
	m_highPress.EnableWindow(FALSE);
	m_lowPress.EnableWindow(FALSE);
	m_isFlowmeter.EnableWindow(FALSE);
	m_isVolutePres.EnableWindow(FALSE);
	m_isDiffPressSensor.EnableWindow(FALSE);
	m_isPressSensor.EnableWindow(FALSE);
	m_examTypeComb.EnableWindow(FALSE);

	m_isFlowmeter.SetCheck(TRUE);
	m_isVolutePres.SetCheck(FALSE);
	m_isDiffPressSensor.SetCheck(FALSE);
	m_isPressSensor.SetCheck(FALSE);
	
	m_SheaderChk.SetCheck(TRUE);
	m_SheaderChk.EnableWindow(FALSE);	
	m_SHeaderDiffPress.EnableWindow(FALSE);	
	m_isWHead.SetCheck(TRUE);

	m_factorQ.EnableWindow(FALSE);
	m_factorS.EnableWindow(FALSE);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	
}


void CEfficiency::OnBnClickedisflowmeter()
{
	//CheckRadioButton(IDC_isFlowmeter,IDC_isVolutePres,IDC_isFlowmeter);
	m_Flowmeter.EnableWindow(TRUE);
	m_FCoefficient.EnableWindow(FALSE);
	m_DiffPressSensor.EnableWindow(FALSE);
	m_isDiffPressSensor.EnableWindow(FALSE);
	m_isPressSensor.EnableWindow(FALSE);
	m_highPress.EnableWindow(FALSE);
	m_lowPress.EnableWindow(FALSE);
	m_factorQ.EnableWindow(FALSE);
}

void CEfficiency::OnBnClickedisvolutepres()
{
	CheckRadioButton(IDC_isDiffPressSensor,IDC_isPressSensor,IDC_isDiffPressSensor);
	
	m_Flowmeter.EnableWindow(FALSE);
	m_FCoefficient.EnableWindow(TRUE);
	m_vInArea.EnableWindow(TRUE);
	m_vOutArea.EnableWindow(TRUE);
	m_DiffPressSensor.EnableWindow(TRUE);
	m_isDiffPressSensor.EnableWindow(TRUE);
	m_isPressSensor.EnableWindow(TRUE);
	m_factorQ.EnableWindow(TRUE);
	//CheckRadioButton(IDC_isFlowmeter,IDC_isVolutePres,IDC_isVolutePres);
}

void CEfficiency::OnBnClickedisdiffpresssensor()
{
	CheckRadioButton(IDC_isFlowmeter,IDC_isVolutePres,IDC_isVolutePres);
	m_DiffPressSensor.EnableWindow(TRUE);
	m_highPress.EnableWindow(FALSE);
	m_lowPress.EnableWindow(FALSE);
	//CheckRadioButton(IDC_isDiffPressSensor,IDC_isPressSensor,IDC_isDiffPressSensor);
}

void CEfficiency::OnBnClickedispresssensor()
{
	CheckRadioButton(IDC_isFlowmeter,IDC_isVolutePres,IDC_isVolutePres);
	m_DiffPressSensor.EnableWindow(FALSE);
	m_highPress.EnableWindow(TRUE);
	m_lowPress.EnableWindow(TRUE);
	//CheckRadioButton(IDC_isDiffPressSensor,IDC_isPressSensor,IDC_isPressSensor);
}

void CEfficiency::OnCbnSelchangeFilenamecmbo()
{
	m_examTypeComb.EnableWindow(FALSE);

	CString filepath,examName,examStr;
	//examNameComb.ResetContent();
	m_fileNameCmbo.GetLBText(m_fileNameCmbo.GetCurSel(),filepath);

	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	//pCMF->fileData->SetExamTree(filepath);

	map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(filepath);
	//examNameComb.InsertString(0,iterE->second);
	//examNameComb.SetCurSel(0);

	m_examTypeComb.ResetContent();
	pCMF->fileData->SetExamTree(filepath);
	/*map<CString,CString>::iterator iterE=pCMF->fileData->examName.find(pCMF->FilePathName);*/
	m_fileNameCmbo.GetLBText(m_fileNameCmbo.GetCurSel(),filepath);
	//examNameComb.GetLBText(examNameComb.GetCurSel(),examName);
	examName=iterE->second;
	vector<CString> examType= pCMF->fileData->GetExamType(filepath,examName);
	vector<CString>::iterator it=examType.begin();

	for (;it!=examType.end();it++)
	{
		m_examTypeComb.InsertString(0,*it);
	}

	m_examTypeComb.SetCurSel(0);
	m_examTypeComb.EnableWindow(TRUE);
	m_examTypeComb.GetLBText(m_examTypeComb.GetCurSel(),examStr);

	vector<CString> paramType= pCMF->fileData->GetExamParam(filepath,examStr);
	vector<CString>::iterator itT=paramType.begin();

	vector<CString> chnName= pCMF->fileData->GetChnName(filepath,itT[0]);
	vector<CString>::iterator itc=chnName.begin();
	//-------------------1.0.0.2------------------------//
	m_inPressure.ResetContent();
	m_outPressure.ResetContent();
	m_SHeaderDiffPress.ResetContent();
	m_activePower.ResetContent();
	m_Flowmeter.ResetContent();
	m_DiffPressSensor.ResetContent();
	m_highPress.ResetContent();
	m_lowPress.ResetContent();
	//--------------------------------------------------//



	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		if((int)pCMF->fileData->GetChnFftFlag(filepath,itc[p])==1)
		{
			m_inPressure.InsertString(0,itc[p]);
			m_outPressure.InsertString(0,itc[p]);
			m_SHeaderDiffPress.InsertString(0,itc[p]);
			m_activePower.InsertString(0,itc[p]);
			m_Flowmeter.InsertString(0,itc[p]);
			m_DiffPressSensor.InsertString(0,itc[p]);
			m_highPress.InsertString(0,itc[p]);
			m_lowPress.InsertString(0,itc[p]);
		}

	}

	m_inPressure.SetCurSel(0);
	m_outPressure.SetCurSel(0);
	m_SHeaderDiffPress.SetCurSel(0);
	m_activePower.SetCurSel(0);
	m_Flowmeter.SetCurSel(0);
	m_DiffPressSensor.SetCurSel(0);
	m_highPress.SetCurSel(0);
	m_lowPress.SetCurSel(0);
}

void CEfficiency::OnCbnSelchangeExamtypecombo()
{


	m_density.EnableWindow(TRUE);
	m_gravityAcc.EnableWindow(TRUE);
	m_elecEfficiency.EnableWindow(TRUE);
	//m_inPressure.EnableWindow(TRUE);
	//m_outPressure.EnableWindow(TRUE);
	m_isWHead.EnableWindow(TRUE);
	//m_diffElevation.EnableWindow(TRUE);
	m_SheaderChk.EnableWindow(TRUE);
	if (m_SheaderChk.GetCheck())
	{
		m_SHeaderDiffPress.EnableWindow(TRUE);
	}
	if (m_isFlowmeter.GetCheck())
	{
		m_Flowmeter.EnableWindow(TRUE);

	}
	m_activePower.EnableWindow(TRUE);
	m_isFlowmeter.EnableWindow(TRUE);
	m_isVolutePres.EnableWindow(TRUE);
	m_vInArea.EnableWindow(TRUE);
	m_vOutArea.EnableWindow(TRUE);
	m_SheaderChk.EnableWindow(TRUE);

	m_factorS.EnableWindow(TRUE);
	/*m_FCoefficient.EnableWindow(TRUE);
	
	m_DiffPressSensor.EnableWindow(TRUE);
	m_highPress.EnableWindow(TRUE);
	m_lowPress.EnableWindow(TRUE);
	m_isFlowmeter.EnableWindow(TRUE);
	m_isVolutePres.EnableWindow(TRUE);
	m_isDiffPressSensor.EnableWindow(TRUE);
	m_isPressSensor.EnableWindow(FALSE);*/
}



void CEfficiency::OnBnClickediswhead()
{
	if (m_isWHead.GetCheck())
	{
		isDWhead=TRUE;
		m_vInArea.EnableWindow(TRUE);
		m_vOutArea.EnableWindow(TRUE);
	}
	else
	{
		isDWhead=FALSE;
		m_vInArea.EnableWindow(FALSE);
		m_vOutArea.EnableWindow(FALSE);
	}
}

void CEfficiency::OnBnClickedCancel()
{
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("完成效率分析");

	OnCancel();
}

void CEfficiency::OnBnClickedSheaderchk()
{
	if (m_SheaderChk.GetCheck())
	{
		//isSWheadDP=TRUE;
		m_outPressure.EnableWindow(FALSE);
		m_inPressure.EnableWindow(FALSE);
		m_diffElevation.EnableWindow(FALSE);
		m_SHeaderDiffPress.EnableWindow(TRUE);
	}
	else
	{
		//isSWheadDP=FALSE;
		m_outPressure.EnableWindow(TRUE);
		m_inPressure.EnableWindow(TRUE);
		m_diffElevation.EnableWindow(TRUE);
		m_SHeaderDiffPress.EnableWindow(FALSE);
	}
}
