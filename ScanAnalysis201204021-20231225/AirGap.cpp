// AirGap.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "AirGap.h"
#include "MainFrm.h"

// CAirGap 对话框

IMPLEMENT_DYNAMIC(CAirGap, CHSJDialog)

CAirGap::CAirGap(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CAirGap::IDD, pParent)
{
	m_x = 0.0;
	m_y = 0.0;
	m_r = 0.0;
	m_nNum=0;
	m_l = 0.0;
	m_angle = 0.0;
}

CAirGap::~CAirGap()
{
}

void CAirGap::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_X, m_x);
	DDX_Text(pDX, IDC_Y, m_y);
	DDX_Text(pDX, IDC_R, m_r);
	DDX_Text(pDX, IDC_Distance, m_l);
	DDX_Text(pDX, IDC_Angle, m_angle);

	DDX_Control(pDX, IDC_BUTTON2, m_save);
	DDX_Control(pDX, IDOK, m_comp);
}


BEGIN_MESSAGE_MAP(CAirGap, CHSJDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAirGap::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CAirGap::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CAirGap::OnBnClickedButton2)
END_MESSAGE_MAP()


// CAirGap 消息处理程序

void CAirGap::OnBnClickedButton1()
{

	oData.clear();
	std::vector<double> dataV;
	CString FilePathName;
	FILE * stream;
	CString strFilter="圆度拟合文件 (*.IAG) | *.iag;*.IAG ||";
	CFileDialog dlg(TRUE,//open/Save as
		NULL,//Ext added automatically
		NULL,//filename
		OFN_HIDEREADONLY |OFN_FILEMUSTEXIST,
		strFilter);

	//CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal()==IDOK)
	{

		FilePathName=dlg.GetPathName();
		stream=fopen(FilePathName,"r");
		if(stream==NULL)
		{
			AfxMessageBox("Can't open  the data!");
			return;
		}

		fscanf(stream,"%d\n",&m_nNum);
		double temX/*,temY*/;
		
		for (int i=0;i<m_nNum;i++)
		{
			//fscanf(stream,"%lf%lf\n",&temX,&temY);
			//m_points[i].x=temX;
			//m_points[i].y=temY;
			
			fscanf(stream,"%lf%\n",&temX);
			oData.push_back(temX);
			m_points[i].x=(LONG)(temX*cos(i*2*3.1415926/(double)m_nNum));
			m_points[i].y=(LONG)(temX*sin(i*2*3.1415926/(double)m_nNum));
			dataV.push_back(m_points[i].x);
			dataV.push_back(m_points[i].y);
			
		}

		NiData.SetSize((int)dataV.size());
		for (int k=0;k<(int)dataV.size();k++)
		{
			NiData[k]=dataV[k];
		}
		fclose(stream);
		m_save.EnableWindow(TRUE);
		m_comp.EnableWindow(TRUE);
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("圆度拟合：完成加载圆度拟合数据");
	}
}
void CAirGap::LeastSquaresFitting()
{
	// 	m_nNum = 4;
	// 	m_points[0].x = 5;
	// 	m_points[0].y = 0;
	// 	m_points[1].x = 0;
	// 	m_points[1].y = 5;
	// 	m_points[2].x = -7;
	// 	m_points[2].y = 0;
	// 	m_points[3].x = 0;
	// 	m_points[3].y = -5;

	if (m_nNum<3)
	{
		return;
	}


	int i=0;

	double X1=0;
	double Y1=0;
	double X2=0;
	double Y2=0;
	double X3=0;
	double Y3=0;
	double X1Y1=0;
	double X1Y2=0;
	double X2Y1=0;

	for (i=0;i<m_nNum;i++)
	{
		X1 = X1 + m_points[i].x;
		Y1 = Y1 + m_points[i].y;
		X2 = X2 + m_points[i].x*m_points[i].x;
		Y2 = Y2 + m_points[i].y*m_points[i].y;
		X3 = X3 + m_points[i].x*m_points[i].x*m_points[i].x;
		Y3 = Y3 + m_points[i].y*m_points[i].y*m_points[i].y;
		X1Y1 = X1Y1 + m_points[i].x*m_points[i].y;
		X1Y2 = X1Y2 + m_points[i].x*m_points[i].y*m_points[i].y;
		X2Y1 = X2Y1 + m_points[i].x*m_points[i].x*m_points[i].y;
	}

	double C,D,E,G,H,N;
	double a,b,c;
	N = m_nNum;
	C = N*X2 - X1*X1;
	D = N*X1Y1 - X1*Y1;
	E = N*X3 + N*X1Y2 - (X2+Y2)*X1;
	G = N*Y2 - Y1*Y1;
	H = N*X2Y1 + N*Y3 - (X2+Y2)*Y1;
	a = (H*D-E*G)/(C*G-D*D);
	b = (H*C-E*D)/(D*D-G*C);
	c = -(a*X1 + b*Y1 + X2 + Y2)/N;

	double A,B,R;
	A = a/(-2);
	B = b/(-2);
	R = sqrt(a*a+b*b-4*c)/2;
	/*1111*/

	/*
	double a,b,c,d,e,n;
	double A,B,C,R;

	n = m_nNum;
	a = 2*n*X2-4*X1*X1;
	b = 2*n*X1*Y1-4*X1*Y1;
	c = n*(X3+X1Y2)-2*X1*(X2+Y2);
	d = 2*n*Y2-4*Y1*Y1;
	e = n*(X2Y1+Y3)-2*Y1*(X2+Y2);
	A = (c*d-e*b)/(a*d-b*b);
	B = (c*b-e*a)/(b*b-d*a);
	C = (X2+Y2-A*2*X1-B*2*Y1)/n;
	R = sqrt (A*A+B*B+C);
	*/
	m_fCenterX = A;
	m_fCenterY = B;
	m_fRadius = R;

	return;
}
void CAirGap::OnBnClickedOk()
{
	LeastSquaresFitting();
	m_x=m_fCenterX;
	m_y=m_fCenterY;
	m_r=m_fRadius;
	m_l=sqrt(m_x*m_x+m_y*m_y);
	m_angle=360.0*atan2(m_y,m_x)/3.1415926;
	if (m_angle<0.0)
		m_angle=m_angle+360.0;
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("圆度拟合：完成最小二乘法圆度拟合计算");
	UpdateData(false);
	//FILE * stream;
	//stream=fopen("out.txt","w+");
	//double m_temp;

	//for (int i=0;i<m_nNum;i++)
	//{
	//	m_temp=sqrt((m_x-(double)m_points[i].x)*(m_x-(double)m_points[i].x)-(m_y-(double)m_points[i].y)*(m_y-(double)m_points[i].y));
	//	if (m_temp<1e-10)
	//		m_temp=0.0;
	//	fprintf(stream,"%lf\n",m_temp);

	//}
	//fclose(stream);
	//OnOK();
}

void CAirGap::OnBnClickedButton2()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	CString savePath;
	savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath==_T(""))
	{
		return;
	}
	else
	{
		CTime tm;
		CString timeStr;
		tm=CTime::GetCurrentTime();
		timeStr=tm.Format("%Y-%m-%d %H-%M-%S");
		CreateDirectory(savePath+"\\"+"气隙试验",0);
		savePath=savePath+"\\"+"气隙试验"+"\\"+"最小二乘法圆度拟合"+timeStr+".oag";

		double minValue, maxValue;
		unsigned int minIndex, maxIndex;
		CNiReal64Vector oNiData((int)oData.size());
		for (int k=0;k<(int)oData.size();k++)
		{
			oNiData[k]=oData[k];
		}
		oNiData.MinMax(minValue, minIndex, maxValue, maxIndex);
		double mean;
		CNiMath::Mean(oNiData,mean);

		double RminValue=0, RmaxValue=0;
		//unsigned int RminIndex, RmaxIndex;
		CNiReal64Vector RNiData((int)oData.size());
		int q=0;
		for (int k=0;k<(int)oData.size();k++)
		{		
			//double a=NiData[k];
			//double b=NiData[k+1];
			RNiData[k]=sqrt(NiData[q]*NiData[q]+NiData[q+1]*NiData[q+1]);
			q+=2;
		}
		RNiData.MinMax(RminValue, minIndex, RmaxValue, maxIndex);
		double Rmean;
		CNiMath::Mean(RNiData,Rmean);

		FILE	*fp = 0;
		fp = fopen(savePath,"w");
		if(fp==0)return;

		fprintf(fp,"圆心x坐标 =\t%lf;\n",m_x); 
		fprintf(fp,"圆心y坐标 =\t%lf;\n",m_y); 
		fprintf(fp,"半径R =\t\t%lf;\n",m_r); 
		fprintf(fp,"偏心距 =\t%lf;\n",m_l); 
		fprintf(fp,"偏心角 =\t%lf;\n",m_angle);

		fprintf(fp,"\n");
		fprintf(fp,"原始数据\n",maxValue);
		fprintf(fp,"----------------------\n");

		fprintf(fp,"最大值 =\t%lf;\n",maxValue);
		fprintf(fp,"最小值 =\t%lf;\n",minValue);
		fprintf(fp,"平均值 =\t%lf;\n",mean);
		
		fprintf(fp,"\n");
		fprintf(fp,"半径R\n",maxValue);
		fprintf(fp,"----------------------\n");

		fprintf(fp,"最大值 =\t%lf;\n",RmaxValue);
		fprintf(fp,"最小值 =\t%lf;\n",RminValue);
		fprintf(fp,"平均值 =\t%lf;\n",Rmean);

		//for(int i=0;i<(int)NiData.GetSize();i++)
		//{
		//	fprintf(fp,"%lf\n",NiData[i]); 
		//	//fprintf(fp,"[%d]=%6.2f;\n",i,ori_ptr[i]); 

		//}
		fseek(fp,0,SEEK_END);
		fclose(fp);
		AfxMessageBox("存储数据完毕！");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("圆度拟合：存储完成,存储文件为  "+savePath);

		OnOK();
	}
}

BOOL CAirGap::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_save.EnableWindow(FALSE);
	m_comp.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
