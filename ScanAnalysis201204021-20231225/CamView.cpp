// CamView.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "CamView.h"
#include "MainFrm.h"
IMPLEMENT_DYNCREATE(CCamView, CFormView)

using namespace std;
CCamView::CCamView()
: CFormView(CCamView::IDD)
{

}

CCamView::~CCamView()
{
}

void CCamView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH1, m_Graph);
}

BEGIN_MESSAGE_MAP(CCamView, CFormView)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_COMMAND(ID_SaveCamData, &CCamView::OnSavecamdata)
	ON_COMMAND(ID_SaveCamToPic, &CCamView::OnSavecamtopic)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CCamView 诊断

#ifdef _DEBUG
void CCamView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCamView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// Cdemo 消息处理程序

void CCamView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	HSJStyleChildControls(GetSafeHwnd(), m_skinFonts);
	HSJStyleGraph(m_Graph);

	try
	{
		m_Graph.SetWindowText("协联工况 实验分析结果");
		CDC* pDC=GetDC();
	
	/*	
		int nb,bpoint[15];
		double q[15][30],eta[15][30];//定桨数据点,流量,开度,效率
		double de[15][30];//定桨数据节点导数
		double qc[15],ec[15];//协联数据点
		double dec[15];//协联数据节点导数
		double u[100],v[100],dq,t;
		double *qq[15],*ee[15],*dee[20];//用于传递参数的指针
	*/

		int i,j,k,nb,bpoint[20];
		double q[20][30],eta[20][30],l[20][30];//定桨数据点,流量,效率
		double de[20][30],dq[20][30];//定桨数据节点导数
		double qc[20],ec[20];//协联数据点
		double dec[20];//协联数据节点导数
		double u[100],v[100],t,dl;//工作单元
		double a[30],b[30],c[30],d[30],delta[30],s;//光顺样条工作单元
		double *qq[20],*ee[20],*dee[20],*ll[20],*dqq[20];//用于传递参数的指针
		//FILE *fp,*fp1,*fp2,*fp3;

		//1、 读入定桨试验数据
		fscanf (file,"%d",&nb);  //叶片角度数

		vector<double> MX1,MY1;
		vector<double> MX2,MY2;
		PX2.clear();
		PY2.clear();


			for (i=0;i<nb;i++) {
			fscanf (file,"%d",bpoint+i);  //测点数
			for (j=0;j<bpoint[i];j++) {
				fscanf (file,"%lf%lf",q[i]+j,eta[i]+j);


				//u[j]=q[i][j];///8.;
				//v[j]=eta[i][j];//*100.;
				//point (fp2,u[j],v[j]);//绘制原始数据点
			}
/*
			
*/

		}





			//绘制原始的曲线
			for (i=0;i<nb;i++) {
				quasp2l (bpoint[i],q[i],eta[i],l[i],dq[i],de[i]);//求二次样条的节点导数
				dl=(l[i][bpoint[i]-1]-l[i][0])/99.;
				for (j=0;j<100;j++) {
					t=l[i][0]+dl*j;
					v[j]=spinte (bpoint[i],t,l[i],eta[i],de[i]);//*100.;
					u[j]=spinte (bpoint[i],t,l[i],q[i],dq[i]);///8.;
				}
				//akurve2 (fp2,100,u,v);
			}
			//fclose (fp2);

			//光顺处理
			for (i=0;i<nb;i++) {
				s=(double)bpoint[i];

				for (j=0;j<30;j++) {
					if (i==nb-1) delta[j]=0.001;//此值用以控制光顺程度，越小，越接近原始数据，
					//越大则越光顺
					else  delta[j]=0.002;
				}
				smooth (bpoint[i],l[i],eta[i],delta,s,a,b,c,d);
				for (j=0;j<bpoint[i];j++) {
					eta[i][j]=a[j];//用新点替换旧点,但建议不这样做，建议保留两种点的数据
				}

				for (j=0;j<30;j++) delta[j]=(q[i][bpoint[i]-1]-q[i][0])/100.;
				smooth (bpoint[i],l[i],q[i],delta,s,a,b,c,d);
				for (j=0;j<bpoint[i];j++) {
					q[i][j]=a[j];//用新点替换旧点,但建议不这样做，建议保留两种点的数据
				}

				for (j=0;j<bpoint[i];j++) {
					u[j]=q[i][j];///8.;
					v[j]=eta[i][j];//*100.;
					//point (fp2,u[j],v[j]);//绘制光顺以后的点
					//fprintf (fp3,"%lf    %lf\n",q[i][j],a[j]);
				}
				//akurve2 (fp2,bpoint[i],u,v);
				//fprintf (fp3,"\n\n");
			}
			//fclose (fp3);
			//fclose (fp2);

			for (i=0;i<nb;i++) {
				quasp2l (bpoint[i],q[i],eta[i],l[i],dq[i],de[i]);//光顺后再求节点导数
				dl=(l[i][bpoint[i]-1]-l[i][0])/99.;

				MX1.clear();
				MY1.clear();
				for (j=0;j<100;j++) {
					t=l[i][0]+dl*j;
					v[j]=spinte (bpoint[i],t,l[i],eta[i],de[i]);//*100.;
					u[j]=spinte (bpoint[i],t,l[i],q[i],dq[i]);///8.;

					MX1.push_back(u[j]);
					MY1.push_back(v[j]);
				}

				CNiReal64Vector mxv1,myv1;

				myv1.SetSize((int)MX1.size());
				mxv1.SetSize((int)MX1.size());

				for (int p=0;p<(int)MX1.size();p++)
				{
					mxv1[p]=MX1[p];
					myv1[p]=MY1[p];
				}
				CString gpn;
				gpn.Format("%d",i);

				gpn.Format("%i",i);
				m_Graph.Plots.Add().Name=gpn;
				m_Graph.Plots.Add();
				m_Graph.Plots.Item(gpn).LineWidth=2;
				m_Graph.Plots.Item(gpn).LineColor=RGB(128,128,128);
				m_Graph.Plots.Item(gpn).PlotXvsY(mxv1,myv1);


				//akurve2 (fp2,100,u,v);//绘制光顺以后的定桨曲线
			}
			//fclose (fp2);

			//计算协联工况点
			for (i=0;i<nb;i++) {  //用于向函数传递二维数组的指针变量
				ll[i]=l[i];
				qq[i]=q[i];
				dqq[i]=dq[i];
				ee[i]=eta[i];
				dee[i]=de[i];
			}
			oncam (nb,bpoint,ll,qq,ee,dqq,dee,qc,ec,dec);//计算协联工况

			//绘制协联曲线
			for (i=0;i<nb;i++) {
				u[i]=qc[i];///8.;
				v[i]=ec[i];//*100.;
			//	point (fp2,u[i],v[i]);
			}
			//fclose (fp2);

			MX1.clear();
			MY1.clear();

			dl=(qc[nb-1]-qc[0])/99.;
			for (j=0;j<100;j++) {
				t=qc[0]+dl*j;
				v[j]=spinte (nb,t,qc,ec,dec);//*100.;
				u[j]=t;///8.;

				MX1.push_back(u[j]);
				MY1.push_back(v[j]);
			}

			CNiReal64Vector mxv1,myv1;

			myv1.SetSize((int)MX1.size());
			mxv1.SetSize((int)MX1.size());

			for (int p=0;p<(int)MX1.size();p++)
			{
				mxv1[p]=MX1[p];
				myv1[p]=MY1[p];
			}


			CString gpn;
			int tem=nb;
			
			gpn.Format("%d",tem);

			gpn.Format("%i",i);
			m_Graph.Plots.Add().Name=gpn;
			m_Graph.Plots.Add();
			m_Graph.Plots.Item(gpn).LineWidth=2;
			m_Graph.Plots.Item(gpn).LineColor=RGB(255,0,0);
			m_Graph.Plots.Item(gpn).PlotXvsY(mxv1,myv1);
			//akurve2 (fp2,100,u,v);

			for (i=0;i<nb;i++) 
			{
				
				 
					PX2.push_back(qc[i]);
					PY2.push_back(ec[i]);
			 

			}

		/*	//保存数据
			for (i=0;i<nb;i++) {
				fprintf (fp1,"\n   i=%d\n",i);
				for (j=0;j<bpoint[i];j++) fprintf (fp1,"  %lf  %lf\n",q[i][j],eta[i][j]);
			}
			fprintf (fp1,"%d\n",nb);
			for (i=0;i<nb;i++) fprintf (fp1,"%f  %f  %f\n",qc[i],ec[i],dec[i]);
			fprintf (fp1,"\n");
			fclose (fp);
			fclose (fp1);
*/


			

















/*

			CNiReal64Vector mxv1,myv1;

			myv1.SetSize((int)MX1.size());
			mxv1.SetSize((int)MX1.size());

			for (int p=0;p<(int)MX1.size();p++)
			{
				mxv1[p]=MX1[p];
				myv1[p]=MY1[p];
			}
			m_Graph.Plots.Item(gpn).LineWidth=2;
			m_Graph.Plots.Item(gpn).LineColor=RGB(255,0,0);
			m_Graph.Plots.Item(gpn).PlotXvsY(mxv1,myv1);

		}


		//2、计算协联工况点
		for (int i=0;i<nb;i++) 
		{  //用于向函数传递二维数组的指针变量
			qq[i]=q[i];
			ee[i]=eta[i];
			dee[i]=de[i];
		}
		oncam (nb,bpoint,qq,ee,dee,qc,ec,dec);//计算协联工况

		for (int i=0;i<nb;i++) 
		{
			dq=(q[i][bpoint[i]-1]-q[i][0])/99.;
			for (int j=0;j<100;j++) 
			{
				t=q[i][0]+dq*j;
				v[j]=cubspip (bpoint[i],t,q[i],eta[i],de[i]);
				u[j]=t/8.;
			}
		}

		dq=(qc[nb-1]-qc[0])/99.;
		for (int j=0;j<100;j++) 
		{
			t=qc[0]+dq*j;
			v[j]=cubspip (nb,t,qc,ec,dec);
			u[j]=t/8.;
		}

		//3、保存数据

		double sxc; 
		double xc; 

		//WinToView(qc[0], ec[0],point);
		MX2.push_back(qc[0]);
		MY2.push_back(ec[0]);
		for(int i=0;i<nb-1;i++)
		{
			for(int k=1;k<20;k++)
			{			
				xc=k*(qc[i+1]-qc[i])/19.0+qc[i];

				double alpha,beta,gama,deta;

				alpha=(xc-qc[i+1])*(xc-qc[i+1])*
					((qc[i+1]-qc[i])+2*(xc-qc[i]))*ec[i]
				/((qc[i+1]-qc[i])*(qc[i+1]-qc[i])*(qc[i+1]-qc[i]));

				beta=(xc-qc[i])*(xc-qc[i])*((qc[i+1]-qc[i])+2*(qc[i+1]-xc))*ec[i+1]/((qc[i+1]-qc[i])*(qc[i+1]-qc[i])*(qc[i+1]-qc[i]));
				gama=(xc-qc[i+1])*(xc-qc[i+1])*(xc-qc[i])*dec[i]/((qc[i+1]-qc[i])*(qc[i+1]-qc[i]));

				deta=(xc-qc[i])*(xc-qc[i])*(xc-qc[i+1])*dec[i+1] /((qc[i+1]-qc[i])*(qc[i+1]-qc[i]));

				sxc=alpha+beta+gama+deta;

				//WinToView(xc, sxc,point);
				MX2.push_back(xc);
				MY2.push_back(sxc);
			}	
			PX2.push_back(qc[i]);
			PY2.push_back(ec[i]);
		}
		PX2.push_back(qc[i]);
		PY2.push_back(ec[i]);
	

		CNiReal64Vector mxv2,myv2;
		myv2.SetSize((int)MX2.size());
		mxv2.SetSize((int)MX2.size());

		m_Graph.Plots.Add().Name="lp";
		for (int p=0;p<(int)MX2.size();p++)
		{
			mxv2[p]=MX2[p];
			myv2[p]=MY2[p];
		}
		m_Graph.Plots.Item("lp").LineWidth=2;
		m_Graph.Plots.Item("lp").LineColor=RGB(0,0,255);
		m_Graph.Plots.Item("lp").PlotXvsY(mxv2,myv2);
		double maxValue=m_Graph.Axes.Item("YAxis-1").Maximum, 
			minValue=m_Graph.Axes.Item("YAxis-1").Minimum, valueDecl;
		valueDecl=(maxValue-minValue)*0.1;

		m_Graph.Axes.Item("YAxis-1").SetMinimum(minValue-valueDecl);
		m_Graph.Axes.Item("YAxis-1").SetMaximum(maxValue+valueDecl);

		CNiReal64Vector pxv2,pyv2;
		pyv2.SetSize((int)PX2.size());
		pxv2.SetSize((int)PX2.size());

		m_Graph.Plots.Add().Name="Max";
		m_Graph.Plots.Item("Max").PointColor=RGB(0,255,0);
		m_Graph.Plots.Item("Max").LineWidth=3;
		m_Graph.Plots.Item("Max").PointStyle=CNiPlot::PointEmptyCircle;
		m_Graph.Plots.Item("Max").LineStyle=CNiPlot::LineNone;
		for (int p=0;p<(int)PX2.size();p++)
		{
			pxv2[p]=PX2[p];
			pyv2[p]=PY2[p];
		}
		m_Graph.Plots.Item("Max").PlotXvsY(pxv2,pyv2 );	
*/
	}

	catch (CException* e)
	{
		CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
		AfxMessageBox("数据有误，请重新加载！");
		pCMF->m_wndOutput.FillText(_T("加载协联工况数据文件有误！"));
	}
	
	//fclose (file);

}

HBRUSH CCamView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (HSJSetWhiteControlColor(pDC, pWnd, nCtlColor)) return HSJWhiteBrush();
    return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCamView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	CWnd *pWnd; 

	pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		pWnd->MoveWindow(0,0,cx,cy);//设置控件大小
	}
	// TODO: 在此处添加消息处理程序代码
}

void CCamView::OnSetFocus(CWnd* pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);

	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	pCMF->ActivateTabEA(2);
}



int CCamView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	if(pMainFrame)
	{
		//pMainFrame->m_pExamView = this;
		pMainFrame->m_Cam = this;
	}
	return 0;
}
//消息：保存协联工况数据
void CCamView::OnSavecamdata()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath!="")
	{
		pCMF->m_wndOutput.FillText(_T("协联工况数据存储：保存协联工况数据..."));

		CreateDirectory(savePath+"\\"+"协联工况",0);
		CString str,path;

		CTime tm;
		tm=CTime::GetCurrentTime();

		str=tm.Format("%Y-%m-%d  %H-%M-%S");
		path=savePath+"\\"+"协联工况"+"\\"+"协联工况 "+str+".oca";

		int i=0;
		//char tmp[50];
		//printf("\n");
		FILE	*fp = 0;
		fp = fopen(path,"w");
		if(fp==0)return;

		for(i=0;i<(int)PX2.size();i++)
		{
			fprintf(fp,"%.6lf\t%.6lf\n",PX2[i],PY2[i]); 
		}

		fseek(fp,0,SEEK_END);
		fclose(fp);
		AfxMessageBox("数据保存完成！");
		pCMF->m_wndOutput.FillText(_T("协联工况数据存储：完成保存："+path));
	}
	else
		pCMF->m_wndOutput.FillText(_T("协联工况数据存储：取消保存协联工况数据"));

}
//消息：保存协联工况图形
void CCamView::OnSavecamtopic()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CRect rcClient = NULL; 
	HWND hwnd=this->GetSafeHwnd();  //令HWND为本窗口的句柄 
	::GetWindowRect(hwnd,&rcClient);  
	int w = rcClient.right - rcClient.left; 
	int h = rcClient.bottom - rcClient.top; 

	HDC dc = ::GetDC(hwnd); 

	HDC gpBmpHdc = CreateCompatibleDC(dc); 
	HBITMAP hBitmap = CreateCompatibleBitmap(dc, w, h); 

	HGDIOBJ oldhdc = SelectObject(gpBmpHdc, hBitmap); 

	::PrintWindow(hwnd,gpBmpHdc,NULL); 

	CImage image; 
	image.Attach(hBitmap); 


	CString filePath;
	CString strFilter;
	strFilter = _T("BMP文件(*.BMP)|*.BMP|JPEG文件(*.JPG)|*.JPG|GIF文件(*.GIF)|*GIF|PNG文件(*.PNG)|*.PNG|TIFF文件(*.TIFF)|*.TIFF||");

	CFileDialog FileOpenDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_NOCHANGEDIR|OFN_OVERWRITEPROMPT, strFilter);

	CString  strBuff ;
	CString  strFileName="协联工况";   
	LPTSTR   pszfilename;   
	//m_Graph.GetWindowText(strFileName);
	pszfilename=strFileName.GetBuffer(_MAX_PATH);
	FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  

	if(FileOpenDlg.DoModal() == IDOK)
	{
		//CString strSaveFilapath = FileOpenDlg.GetPathName();
		//CString strSaveFilapath;
		//FileOpenDlg.()
		//m_Graph.GetWindowText(strSaveFilapath);
		pCMF->m_wndOutput.FillText(_T("协联工况图形存储：保存协联工况图形..."));
		CString strSaveFilapath = FileOpenDlg.GetPathName();

		strSaveFilapath.MakeLower();


		switch(FileOpenDlg.m_ofn.nFilterIndex)
		{
		case 1:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".bmp") != 0)
				strSaveFilapath = strSaveFilapath + ".bmp";	
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatBMP);
			break;
		case 2:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".jpg") != 0)
				strSaveFilapath = strSaveFilapath + ".jpg";
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatJPEG);
			break;
		case 3:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".gif") != 0)
				strSaveFilapath = strSaveFilapath + ".gif";
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatGIF);
			break;
		case 4:
			strBuff = strSaveFilapath.Right(4);
			if(strBuff.Compare(".png") != 0)
				strSaveFilapath = strSaveFilapath + ".png";
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatPNG);
			break;
		default:
			strBuff = strSaveFilapath.Right(5);
			if(strBuff.Compare(".tiff") != 0)				
				strSaveFilapath = strSaveFilapath + ".tiff";
			strSaveFilapath.MakeLower();
			image.Save(strSaveFilapath,Gdiplus::ImageFormatTIFF);
			break;
		}		
		AfxMessageBox("保存完成!");
		pCMF->m_wndOutput.FillText(_T("协联工况图形存储：完成保存，存储为  "+strSaveFilapath));

	}
	else
		pCMF->m_wndOutput.FillText(_T("协联工况图形存储：取消保存协联工况图形"));

	image.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
}

void CCamView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;

	VERIFY(menu.LoadMenu (IDR_CAMMENU));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x, point.y, AfxGetMainWnd());

	menu.Detach();
}




/*-------------------- 求斜率等于给定值的点 ---------------------------*/
double CCamView::d_dd(int n,double x[],double y[],double dy[],double dd,double &xx)
{
	double x1,x2,x3,f1,f2,f3,err;
	int i;

	//若有一个节点导数值等于dd，即为所求的点
	for (i=0;i<n;i++) {  
		if (dy[i]==dd) {
			xx=x[i];
			return (y[i]);
		}
	}

	//一般情况
	for (i=0;i<n-1;i++) {
		if ((dd-dy[i])*(dy[i+1]-dd)>0.0) {//解所在的区间  
			x1=x[i];
			x2=x[i+1];
			f1=dy[i]-dd;
			f2=dy[i+1]-dd;
			break;
		}
	}

	//迭代求解
	/*err=1e5;
	while (err>0.0000001) {
	x3=x2-0.01*(f2*(x2-x1)/(f2-f1));
	err=fabs(x3-x2);
	x1=x2;
	x2=x3;
	f1=f2;
	f2=dspinte(n,x2,x,y,dy)-dd;
	xx=x3;
	}*/

	//两分法求解
	err=1e5;
	while (err>0.0000001) {
		err=fabs(x2-x1);
		x3=(x1+x2)/2.;
		f3=dspinte(n,x3,x,y,dy)-dd;
		xx=x3;
		if (f1*f3<0.) {
			x2=x3;
			f2=f3;
		}
		else {
			x1=x3;
			f1=f3;
		}
	}
	return (spinte(n,x3,x,y,dy));
}

//------------------- 求协联工况点 ---------------------------
void CCamView::oncam (int nb,int bpoint[],double *l[],double *q[],double *eta[],double *dq[],
			double *de[],double qc[],double ec[],double dec[])
{
	int i;
	double err,t,t1;

	//   各叶片角曲线的样条拟合
	//for (i=0;i<nb;i++) {
	//	quasp2 (bpoint[i],q[i],eta[i],de[i]);
	//}

	//各角度的最高效率点作为协联工况的初值
	for (i=0;i<nb;i++) {
		ec[i]=d_dl (bpoint[i],l[i],q[i],eta[i],dq[i],de[i],0.,t);
		qc[i]=spinte (bpoint[i],t,l[i],q[i],dq[i]);
	}

	//逐次逼近求切点(导数相同)
	err=1e9;
	while (err>0.000001) {
		quasp2 (nb,qc,ec,dec);
		err=0.;
		for (i=0;i<nb;i++) {
			t1=qc[i];
			ec[i]=d_dl (bpoint[i],l[i],q[i],eta[i],dq[i],de[i],dec[i],t);
			qc[i]=spinte (bpoint[i],t,l[i],q[i],dq[i]);
			if (err<fabs(t1-qc[i])) err=fabs(t1-qc[i]);
		}
	}
}

//光顺样条函数
void CCamView::smooth (int n,double x[],double y[],double dy[],double s,
			 double a[],double b[],double c[],double d[])
{
	double t[50],t1[50],r[51],r1[51],r2[51],u[51],v[51];
	double p,h,f,g,e,gg,w,w1,w2;
	int k,n1,i;
	//       n 节点数    x,y 节点坐标    dy 权因子    s 光顺参数
	//       光顺样条是分段三次多项式,在每一段内,多项式的系数为a,b,c,d
	//       t,t1,r,r1,r2,u,v为工作单元

	r2[0]=0.;
	r2[n-1]=0.;
	r2[n]=0.;
	r1[0]=0.;
	r1[n-1]=0.;
	r[0]=0.;
	u[n-1]=0.;
	u[n]=0.;
	u[0]=0.;
	d[0]=0.;
	p=0.;
	k=0;
	n1=n-1;
	h=x[1]-x[0];
	f=(y[1]-y[0])/h;
	for (i=1;i<n1;i++) {
		g=h;
		e=f;
		h=x[i+1]-x[i];
		f=(y[i+1]-y[i])/h;
		a[i]=f-e;
		t[i]=(g+h)*2.0/3.;
		t1[i]=h/3.0;
		r2[i]=dy[i-1]/g;
		r[i]=dy[i+1]/h;
		r1[i]=-dy[i]/g-dy[i]/h;
	}
	for(i=1;i<n1;i++) {
		b[i]=r[i]*r[i]+r1[i]*r1[i]+r2[i]*r2[i];
		c[i]=r[i]*r1[i+1]+r1[i]*r2[i+1];
		d[i]=r[i]*r2[i+2];
	}
loop:
	k=k+1;
	for (i=1;i<n1;i++) {
		if (i==1) {
			r[i]=1./(b[i]+p*t[i]);
			u[i]=a[i];
		}
		else {
			g=d[i-2];
			f=c[i-1]+p*t1[i-1]-g*r1[i-2];
			r1[i-1]=f*r[i-1];
			r2[i-2]=g*r[i-2];
			r[i]=1./(b[i]+p*t[i]-f*r1[i-1]-g*r2[i-2]);
			u[i]=a[i]-r1[i-1]*u[i-1]-r2[i-2]*u[i-2];
		}
	}
	for (i=n1-1;i>=1;i--) {
		u[i]=r[i]*u[i]-r1[i]*u[i+1]-r2[i]*u[i+2];
	}
	e=0.;
	h=0.;
	for (i=0;i<n1;i++) {
		g=h;
		h=(u[i+1]-u[i])/(x[i+1]-x[i]);
		v[i]=(h-g)*dy[i];
		e=e+v[i]*v[i];
	}
	v[n-1]=u[n1-1]/(x[n-1]-x[n1-1])*dy[n-1];
	e=e+v[n-1]*v[n-1];
	if (e-s<=1e-04) goto label20;
	f=0.;
	gg=0.;
	w1=0.;
	w2=0.;
	for (i=1;i<n1;i++) {
		g=u[i-1]*t1[i-1]+u[i+1]*t1[i]+u[i]*t[i];
		f=f+g*u[i];
		if (i>1) w=g-r1[i-1]*w1-r2[i-2]*w2;
		else w=g;
		w2=w1;
		w1=w;
		gg=gg+w*w*r[i];
	}
	p=p+(e-sqrt(s*e))/(f-p*gg);
	if (p<0.0) {
		s=s*0.8;
		p=0.;
	}
	goto loop;

label20:
	for (i=0;i<n;i++) {
		a[i]=y[i]-dy[i]*v[i];
		c[i]=p*u[i];
	}
	for (i=0;i<n1;i++) {
		h=x[i+1]-x[i];
		d[i]=(c[i+1]-c[i])/3./h;
		b[i]=(a[i+1]-a[i])/h-(h*d[i]+c[i])*h;
	}
	b[n-1]=b[n1-1]+2.*c[n1-1]*h+3.*d[n1-1]*h*h;
}

//与autoCAD的接口
//------------------ 在CAD图面上设定一个点 -------------------------
void CCamView::point (FILE *fp,double x,double y)
{
	fprintf (fp,"point %f,%f\n",x,y);
}

//---------- 向autoCAD输出样条曲线 -----------------
void CCamView::akurve2 (FILE *fp,int n,double x[],double y[])
{
	int i,j;

	i=0;
	fprintf (fp,"spline ");
	for (j=0;j<n;j++) {
		fprintf (fp,"%f,%f",x[j],y[j]);
		if (i==1||j==n-1) fprintf (fp,"\n");
		else fprintf (fp," ");
		i++;
		if (i==2) i=0;
	}
	fprintf (fp,"\n\n\n");
}

//二次样条函数

//----------------- 一个区间中二次样条求值公式 -----------------------
double CCamView::sp2(double x,double x1,double x2,double y1,double y2,
		   double dy1,double dy2)
{
	double h,t,sp;

	h=x2-x1;
	t=x-x1;
	if (t<=0.5*h)
		sp=y1+t*(dy1+0.5*t*(4.0*(y2-y1)/pow(h,2.0)-(dy2+3.0*dy1)/h));
	else {
		t=h-t;
		sp=y2+t*(-dy2+0.5*t*((3.0*dy2+dy1)/h-4.0*(y2-y1)/pow(h,2.0)));
	}
	return(sp);
}

//--------------- 一个区间中二次样条的导数值 -----------------------
double CCamView::sp2d1(double x,double x1,double x2,double y1,
			 double y2,double dy1,double dy2)
{
	double h,t,dy;

	h=x2-x1;
	t=(x-x1)/h;
	if (t<=0.5)
		dy=dy1+t*(4.*(y2-y1)/h-dy2-3.*dy1);
	else
		dy=-dy1-2.*dy2+4.*(y2-y1)/h+t*(4.*(y1-y2)/h+3.*dy2+dy1);
	return (dy);
}

//--------------- 一条曲线上二次样条插值 -------------------------
double CCamView::spinte(int n,double xx,double x[],double y[],double dy[])
{
	double re;
	int i;

	if (xx<x[0] && xx>1.1*x[0]-0.1*x[1])
		return (y[0]+(xx-x[0])*dy[0]);
	else if (xx>x[n-1] && xx<1.1*x[n-1]-0.1*x[n-2])
		return (y[n-1]+(xx-x[n-1])*dy[n-1]);
	else {
		for (i=0;i<n-1;i++){
			if (xx>=x[i]&&xx<=x[i+1]){
				re=sp2 (xx,x[i],x[i+1],y[i],y[i+1],dy[i],dy[i+1]);
				return(re);
			}
		}
	}
	return(-1e99);
}

//----------------- 一条曲线上二次样条求导 -----------------------
double CCamView::dspinte(int n,double xx,double x[],double y[],double dy[])
{
	double re;
	int i;

	for (i=0;i<n-1;i++){
		if (xx>=x[i]&&xx<=x[i+1]){
			re=sp2d1(xx,x[i],x[i+1],y[i],y[i+1],dy[i],dy[i+1]);
			return(re);
		}
	}
	if (i==n-1) re=-1e99;
	return(re);
}

//--------------- 计算二次样条曲线的节点导数 ---------------------
void CCamView::quasp2 (int n,double x[],double y[],double dy[])
{
	int n1,j;
	double b[100],h,h1,yy,yy1,af,bt;

	if (n==1) dy[0]=0.;//如果只有一个点，不计算
	else if (n==2) {//如果是两个点，按直线计算
		dy[0]=(y[1]-y[0])/(x[1]-x[0]);
		dy[1]=dy[0];
	}
	else {//三个点以上，正常计算
		h=x[1]-x[0];
		yy=y[1]-y[0];
		dy[0]=-.333333333;
		b[0]=1.33333333*yy/h;
		n1=n-1;
		for (j=1;j<n1;j++) {
			h1=h;
			yy1=yy;
			h=x[j+1]-x[j];
			yy=y[j+1]-y[j];
			af=h1/(h1+h);
			bt=4.*((1.-af)*yy1/h1+af*yy/h);
			dy[j]=-af/(3.+(1.-af)*dy[j-1]);
			b[j]=(bt-(1.-af)*b[j-1])/(3.+(1.-af)*dy[j-1]);
		}
		dy[n-1]=(4.*yy/h-b[n-2])/(3.+dy[n-2]);
		for (j=n1-1;j>=0;j--) dy[j]=dy[j]*dy[j+1]+b[j];
	}
}

double CCamView::d_dl (int n,double l[],double x[],double y[],double dx[],double dy[],double dd,double &ll) {

	double l1,l2,l3,f1,f2,f3,err,df;
	int i;

	//若有一个节点导数值等于dd，即为所求的点
	for (i=0;i<n;i++) {  
		if (dy[i]/dx[i]==dd) {
			ll=l[i];
			return (y[i]/dx[i]);
		}
	}

	//一般情况
	for (i=0;i<n-1;i++) {
		if ((dd-dy[i]/dx[i])*(dy[i+1]/dx[i+1]-dd)>0.0) {//解所在的区间  
			l1=l[i];
			l2=l[i+1];
			f1=dy[i]/dx[i]-dd;
			f2=dy[i+1]/dx[i+1]-dd;
			break;
		}
	}

	//两分法求解
	err=1e5;
	while (err>0.0000001) {
		err=fabs(l2-l1);
		l3=(l1+l2)/2.;
		f3=dspinte(n,l3,l,y,dy)/dspinte(n,l3,l,x,dx)-dd;
		ll=l3;
		if (f1*f3<0.) {
			l2=l3;
			f2=f3;
		}
		else {
			l1=l3;
			f1=f3;
		}
	}
	return (spinte(n,l3,l,y,dy));
}

void CCamView::quasp2l(int n,double x[],double y[],double l[],double dx[],double dy[])
{
	int i;
	l[0]=0.0;
	for (i=1;i<n;i++) {
		l[i]=l[i-1]+cll(x[i]-x[i-1],y[i]-y[i-1]);
	}
	quasp2(n,l,x,dx);
	quasp2(n,l,y,dy);
}

double CCamView::cll(double x,double y)
{
	double re;
	re=sqrt(pow(x,2.)+pow(y,2.));
	return(re);
}
