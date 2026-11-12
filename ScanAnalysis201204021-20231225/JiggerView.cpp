// JiggerView.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "JiggerView.h"
#include "MainFrm.h"
#include "JiggerExamSet.h"


//////////////////////////////////////////////////////////////////////////

/**
* 函数功能：矩阵相乘
* @param a m行n列的矩阵
* @param b n行p列的矩阵
*/
void MatrixMultiply(double** a, double** b, double **c, int m, int n, int p)
{
	int i, j, k;
	for(i = 0; i < m; i ++)
		for(j = 0; j < p; j ++)
		{
			c[i][j] = 0.0;
			for(k = 0; k < n; k ++)
				c[i][j] += a[i][k]*b[k][j];
		}
}

/**
* 函数功能：矩阵转置
* @param a m行n列的矩阵
* @param b n行m列的矩阵
*/
void MatrixTranspose(double** a, double** t, int m, int n)
{
	int i, j;
	for(i=0;i<=m-1;i++){
		for(j=0;j<=n - 1;j++){
			t[j][i]=a[i][j];
		}
	}
}

/**
* 函数功能：矩阵求逆
* @param a 二维数组，开始为原矩阵，返回时为逆矩阵
*/
int MatrixInverse(double** a, int n)
{
	int i,j,k;
	int *is = new int[n];
	int *js = new int[n];

	double t,d;

	for (k=0;k<n;k++) {
		d=0.;
		for (i=k;i<n;i++) {
			for (j=k;j<n;j++) {
				if (fabs(*(a[i]+j))>d) {
					d=fabs(*(a[i]+j));
					is[k]=i;
					js[k]=j;
				}				
			}
		}

		if (d+1.0==1.0) return (0);

		for (j=0;j<n;j++) {
			t=*(a[k]+j);
			*(a[k]+j)=*(a[is[k]]+j);
			*(a[is[k]]+j)=t;
		}

		for (i=0;i<n;i++) {
			t=*(a[i]+k);
			*(a[i]+k)=*(a[i]+js[k]);
			*(a[i]+js[k])=t;
		}

		*(a[k]+k)=1./(*(a[k]+k));

		for (j=0;j<n;j++) {
			if (j!=k) *(a[k]+j)=*(a[k]+j)*(*(a[k]+k));
		}

		for (i=0;i<n;i++) {
			if (i!=k) {
				for (j=0;j<n;j++) {
					if (j!=k) *(a[i]+j)=*(a[i]+j)-(*(a[i]+k))*(*(a[k]+j));
				}
			}
		}

		for (i=0;i<n;i++) {
			if (i!=k) *(a[i]+k)=-(*(a[i]+k))*(*(a[k]+k));
		}
	}

	for (k=n-1;k>=0;k--) {
		for (j=0;j<n;j++) {
			t=*(a[k]+j);
			*(a[k]+j)=*(a[js[k]]+j);
			*(a[js[k]]+j)=t;
		}

		for (i=0;i<n;i++) {
			t=*(a[i]+k);
			*(a[i]+k)=*(a[i]+is[k]);
			*(a[i]+is[k])=t;
		}
	}
	delete is;
	delete js;
	return 1;
}

void MatrixInverse(double** a, double ** b, int n)
{
	int i, j, row, col, k;
	double max, temp;

	int* p = new int[n];

	double **a_origin=new double*[n];   
	for(i=0;i<n;i++)
	{
		a_origin[i]=new double[n];
	}

	for(i = 0; i < n; i ++)
	{
		for(j = 0; j < n; j ++)
		{
			a_origin[i][j] = a[i][j];
		}
	}


	for(i = 0; i < n; i ++)
	{
		p[i] = i;
		b[i][i] = 1;
	}

	for(k = 0; k < n; k ++)
	{
		//找主元
		max = 0;
		row = col = i;
		for(i = k; i < n; i ++){
			for(j= k; j < n; j ++)
			{
				temp = fabs(b[i][j]);
				if(max < temp)
				{
					max = temp;
					row = i;
					col = j;
				}
			}
		}
		//交换行列，将主元调整到k行k列
		if(row != k)
		{
			for(j = 0; j < n; j ++)
			{
				temp = a[row][j];
				a[row][j] = a[k][j];
				a[k][j] = temp;

				temp = b[row][j];
				b[row][j] = b[k][j];
				b[k][j] = temp;
			}
			i = p[row];
			p[row] = p[k];
			p[k] = i;
		}

		if(col != k)
		{
			for(i = 0; i < n; i ++)
			{
				temp = a[i][col];
				a[i][col] = a[i][k];
				a[i][k] = temp;
			}
		}

		//处理
		for(j = k+1; j < n; j ++)
			a[k][j] /= a[k][k];
		for(j = 0; j < n; j ++)
			b[k][j] /= a[k][k];
		a[k][k] = 1;
		for(j = k+1; j < n; j ++)
		{
			for(i = 0; i < k; i ++)
				a[i][j] -= a[i][k] * a[k][j];
			for(i = k+1; i < n; i ++)
				a[i][j] -= a[i][k] * a[k][j];
		}
		for(j = 0; j < n; j ++)
		{
			for(i = 0; i < k; i++)
				b[i][j] -= a[i][k] * b[k][j];
			for(i = k + 1; i < n; i++)
				b[i][j] -= a[i][k] * b[k][j];
		}
		for(i = 0; i < k; i ++)
			a[i][k] = 0.0;
		a[k][k] = 1.0;
	}
	//恢复行列次序
	for(j = 0; j < n; j ++){
		for(i = 0; i < n; i ++)
			a[p[i]][j] = b[i][j];
	}

	for(i = 0; i < n; i ++){
		for(j = 0; j < n; j ++)
		{
			b[i][j] = a[i][j];
			a[i][j] = a_origin[i][j];
		}
	}

	for (i = 0; i < n; ++i)   
	{   
		delete a_origin[i];
		a_origin[i] = NULL;
	}   
	delete []a_origin;   
	a_origin = NULL;   
	delete p;

}

void MatrixHouseholderTransfer(double** a, double** h, int m, int n)
{
	/*
	* a 的维数m×n
	*/
	int i = 0;
	double **c=new double*[n];   
	for(i=0;i<n;i++)
	{
		c[i]=new double[n];
	}

	double **d=new double*[n];   
	for(i=0;i<n;i++)
	{
		d[i]=new double[n];
	}

	double **b=new double*[n];   
	for(i=0;i<n;i++)
	{
		b[i]=new double[m];
	}

	MatrixTranspose(a,b,m,n); //矩阵a转置后变成b，b的维数n×m
	MatrixMultiply(b,a,c,n,m,n); //矩阵b(n×m)乘以a(m×n)，得c(n×n)
	MatrixInverse(c,n);//逆矩阵c(n×n)
	MatrixMultiply(c,b,h,n,n,m);//d(n×n) × b (n×m) = h(n×m)


	for (i = 0; i < n; ++i)   
	{   
		delete c[i];
		c[i] = NULL;
		delete d[i];
		d[i] = NULL;
	}   
	delete []c;   
	c = NULL;
	delete []d;   
	d = NULL;

	for (i = 0; i < n; ++i)   
	{   
		delete b[i];
		b[i] = NULL;
	}   
	delete []b;   
	b = NULL;
}

void CreateFullRankHouseholderMatrix(double *alfa, double **A, int m)
{
	int i,j;

	for(i = 0; i < 2*m; i++){
		for(j = 0; j < m + 4; j ++){
			A[i][j] = 0;
		}
	}//init

	for(i = 0; i < m; i++){
		for(j = 0; j < m; j ++){
			if(i == j)
				A[i][j] = 1;
		}
	}
	for(i = 0; i < m; i++){
		A[i][m] = - cos(alfa[i]);
		A[i][m+1] = - sin(alfa[i]);
	}//up half

	for(i = 0; i < m; i++){
		for(j = 0; j < m; j ++){
			if(i == j)
				A[i+m][j] = 1;
		}
	}
	for(i = 0; i < m; i++){
		A[i+m][m+2] = - cos(alfa[i]);
		A[i+m][m+3] = - sin(alfa[i]);
	}


	A[0+m][m+2] = - cos(alfa[0+2]);
	A[0+m][m+3] = - sin(alfa[0+2]);

	A[1+m][m+2] = - cos(alfa[1+2]);
	A[1+m][m+3] = - sin(alfa[1+2]);
}

void CreateBMatrix(double *h1, double *h2, double **h, int m)
{
	int i;
	for(i = 0; i < m; i ++){
		h[i][0]   = h1[i];
		h[i+m][0] = h2[i];
	}
	h[m][0]   = h[m+2][0];
	h[m+1][0] = h[m+3][0];
}

//////////////////////////////////////////////////////////////////////////

// CJiggerView

IMPLEMENT_DYNCREATE(CJiggerView, CFormView)

CJiggerView::CJiggerView()
	: CFormView(CJiggerView::IDD),m_XAxies(0.0),m_YAxies(0.0),m_Distance(0.0),m_Angle(0.0)
{

}

CJiggerView::~CJiggerView()
{
}

void CJiggerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH1, m_Graph);
	DDX_Control(pDX, IDC_CWGRAPH2, m_NewGraph);
	DDX_Text(pDX, IDC_XAxies, m_XAxies);
	DDX_Text(pDX, IDC_YAxies, m_YAxies);
	DDX_Text(pDX, IDC_Distance, m_Distance);
	DDX_Text(pDX, IDC_Angle, m_Angle);
}

BEGIN_MESSAGE_MAP(CJiggerView, CFormView)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_LoadJiggerData, &CJiggerView::OnLoadjiggerdata)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LoadTxtJigger, &CJiggerView::OnLoadtxtjigger)
	ON_COMMAND(ID_SaveJiggerToTxt, &CJiggerView::OnSavejiggertotxt)
	ON_COMMAND(ID_SaveJiggerToImage, &CJiggerView::OnSavejiggertoimage)
	ON_BN_CLICKED(IDC_BUTTON1, &CJiggerView::OnBnClickedButton1)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CJiggerView 诊断

#ifdef _DEBUG
void CJiggerView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CJiggerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CJiggerView 消息处理程序

void CJiggerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	HSJStyleChildControls(GetSafeHwnd(), m_skinFonts);
	HSJStyleGraph(m_Graph);
	HSJStyleGraph(m_NewGraph);

	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/
	//调整界面，不出现滚动条////Marshall  2011.3.2 12:23
	SetScrollSizes(MM_TEXT,CSize(0,0));
	//InitNiGraph();
	ResizeParentToFit();
	/*--------------------------Marshall's Code2011.3.2 12:23--------------*/
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	// TODO: 在此添加专用代码和/或调用基类
}

HBRUSH CJiggerView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (HSJSetWhiteControlColor(pDC, pWnd, nCtlColor)) return HSJWhiteBrush();
    return CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
}

int CJiggerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;


	//CMainFrame*   pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;  
	//if(pMainFrame)
	//{
	//	//pMainFrame->m_pExamView = this;
	//	pMainFrame->m_P3DFFT = this;
	//}

	return 0;
}

void CJiggerView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CWnd *pWnd; 

	pWnd = GetDlgItem(IDC_CWGRAPH1);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		pWnd->MoveWindow(0,30,cx,cy/2-15);//设置控件大小
	}

	pWnd = GetDlgItem(IDC_CWGRAPH2);     //获取控件句柄
	if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		pWnd->MoveWindow(0,cy/2+15,cx,cy/2-15);//设置控件大小
	}
	
	/*--------------------------Marshall's Code2011.3.3 19:12--------------*/
	//移动视图区头部控件
	int nDist = 5;//与顶部间隔设为5
	int nPer = 10;
	int nHeight = 20;

	int arrID[] = {IDC_BUTTON1,IDC_STATIC_ONE,IDC_XAxies,IDC_STATIC_TWO,IDC_YAxies,IDC_STATIC_THR,IDC_Distance,IDC_STATIC_FOR,IDC_Angle};
	for(int i = 0; i < 9; i++)
	{
		pWnd = GetDlgItem(arrID[i]);     
		if (pWnd!=NULL)		pWnd->ShowWindow(TRUE);
		if(pWnd)
		{
			pWnd->MoveWindow(nDist + i*(cx-2*nDist)/9,nDist,cx/nPer,nHeight);
		}
	}

	/*--------------------------Marshall's Code2011.3.3 19:12--------------*/
}
//消息：加载通道数据
void CJiggerView::OnLoadjiggerdata()
{
	CJiggerExamSet jes(this);
	if(jes.DoModal()==IDOK)
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：加载通道数据");
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：取消加载通道数据");

}

void CJiggerView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;

	VERIFY(menu.LoadMenu (IDR_JiggerMenu));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,point.x, point.y, AfxGetMainWnd());

	menu.Detach();
}

void CJiggerView::setChnData(CNiReal64Vector o,CNiReal64Vector n)
{
	oChnDataMean=o;
	nChnDataMean=n;
	DrawGraph();
}
//显示通道数据计算结果频谱图
void CJiggerView::DrawGraph()
{	
	CNiReal64Vector XAxies(JIGGERCOUNT);
	for (int k=0;k<JIGGERCOUNT;k++)
	{
		XAxies[k]=k+1;
	}
	m_Graph.SetWindowText("第一组通道数据计算结果");
	m_Graph.Plots.Item(1).LineColor=RGB(0,0,255);
	m_Graph.Plots.Item(1).PlotXvsY(XAxies,oChnDataMean);


	for (int k=0;k<JIGGERCOUNT;k++)
	{	
		m_Graph.Plots.Add();
		m_Graph.Plots.Item(k+2).LineColor=RGB(255,0,0);
		//m_Graph.Plots.Item(k+2).LineWidth=2;
		m_Graph.Plots.Item(k+2).LineStyle=CNiPlot::LineDash;
		CNiReal64Vector etX(2);
		CNiReal64Vector etY(2);
		etX[0]=etX[1]=XAxies[k];
		etY[0]=0;
		etY[1]=oChnDataMean[k];	
		m_Graph.Plots.Item(k+2).PlotXvsY(etX,etY);
	}

	m_NewGraph.SetWindowText("第二组通道数据计算结果");
	m_NewGraph.Plots.Item(1).LineColor=RGB(0,0,255);
	m_NewGraph.Plots.Item(1).PlotXvsY(XAxies,nChnDataMean);
	for (int k=0;k<JIGGERCOUNT;k++)
	{	
		m_NewGraph.Plots.Add();
		m_NewGraph.Plots.Item(k+2).LineColor=RGB(255,0,0);
		//m_Graph.Plots.Item(k+2).LineWidth=2;
		m_NewGraph.Plots.Item(k+2).LineStyle=CNiPlot::LineDash;
		CNiReal64Vector etX(2);
		CNiReal64Vector etY(2);
		etX[0]=etX[1]=XAxies[k];
		etY[0]=0;
		etY[1]=nChnDataMean[k];	
		m_NewGraph.Plots.Item(k+2).PlotXvsY(etX,etY);
	}
}

//加载通道数据
void CJiggerView::OnLoadtxtjigger()
{

	CString strFilter="盘车试验文件 (*.JGR) | *.jgr;*.JGR ||";
	CFileDialog dlg(TRUE,//open/Save as
		NULL,//Ext added automatically
		NULL,//filename
		OFN_HIDEREADONLY |OFN_FILEMUSTEXIST,
		strFilter);

	CString m_FilePathName,m_Title;
	std::vector<double> fileData;

	if(dlg.DoModal()==IDOK)
	{		

		CString invalidStr;
		m_FilePathName=dlg.GetPathName();
		m_Title = m_FilePathName.Mid(m_FilePathName.ReverseFind('\\')+1);
		m_Title = m_Title.Left(m_Title.ReverseFind('.'));
		FILE * f=fopen(m_FilePathName,"r");	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：文件加载　　"+m_FilePathName);

		if(f!=0)
		{	
			//int k=fscanf(f,"%s",invalidStr);
			//if (invalidStr!="Jigger")
			//{
			//	fclose(f);
			//	AfxMessageBox("数据不全或数据格式不正确");
			//	
			//	return;
			//}
			while(!feof(f))
			{
				double data;
				
				fscanf(f,"%lf",&data);
				fileData.push_back(data);
			}
			fileData.pop_back();
			fclose(f);
		}	

		if (fileData.size()!=2*JIGGERCOUNT )/*|| invalidStr!="Jigger")*/
		{
			((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：所加载的文件数据不全或数据格式不正确！");
			AfxMessageBox("数据不全或数据格式不正确！");
			return;
		}
		else
		{
			oChnDataMean.Reverse();
			nChnDataMean.Reverse();
			oChnDataMean.SetSize(JIGGERCOUNT);
			nChnDataMean.SetSize(JIGGERCOUNT);

			for (int k=0;k<JIGGERCOUNT;k++)
			{
				oChnDataMean[k]=fileData[k];
			}
			for (int k=JIGGERCOUNT;k<2*JIGGERCOUNT;k++)
			{
				nChnDataMean[k-JIGGERCOUNT]=fileData[k];
			}	
			DrawGraph();
		}			
	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：取消文件加载");
}
//消息：保存通道数据
void CJiggerView::OnSavejiggertotxt()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();

	CString savePath = pCMF->fileData->ShowSelectDirectoryDlg(this->m_hWnd,"系统工作目录选择");
	if (savePath!="")
	{	
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：保存通道数据...");

		CreateDirectory(savePath+"\\"+"盘车试验",0);
		CString str,path;

		CTime tm;
		tm=CTime::GetCurrentTime();

		str=tm.Format("%Y-%m-%d  %H-%M-%S");
		path=savePath+"\\"+"盘车试验"+"\\"+"盘车数据 "+str+".jgr";

		SaveDataToText(path,oChnDataMean,nChnDataMean,JIGGERCOUNT);
		AfxMessageBox("保存完成!");
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：保存完成，存储为："+path);

	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：取消保存通道数据");

	//m_savePath.SetWindowText(savePath);	
}

BOOL CJiggerView::SaveDataToText(CString FilePathName,CNiReal64Vector oChnPtr,CNiReal64Vector nChnPtr,int len)
{
	if(oChnPtr.GetSize()==0||nChnPtr.GetSize()==0||len<=0) return FALSE;
	int i=0;
	//char tmp[50];
	//printf("\n");
	FILE	*fp = 0;
	fp = fopen(FilePathName,"w");
	if(fp==0)return FALSE;

	for(i=0;i<len;i++)
	{
		fprintf(fp,"%.20lf\n",oChnPtr[i]); 
	}
	for(i=0;i<len;i++)
	{
		fprintf(fp,"%.20lf\n",nChnPtr[i]); 
	}
	fseek(fp,0,SEEK_END);
	fclose(fp);
	return TRUE;
}
//消息：保存视图到图像
void CJiggerView::OnSavejiggertoimage()
{
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

	CString strBuff ;
	CString   strFileName="盘车试验结果";   
	LPTSTR   pszfilename;   
	//m_Graph.GetWindowText(strFileName);
	pszfilename=strFileName.GetBuffer(_MAX_PATH);
	FileOpenDlg.m_ofn.lpstrFile   =   strFileName.GetBuffer(0);   
	FileOpenDlg.m_ofn.nMaxFile =_MAX_PATH;   
	FileOpenDlg.m_ofn.lpstrTitle           =   "保存";  

	if(FileOpenDlg.DoModal() == IDOK)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：保存视图到图像...");
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
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：完成保存，存储为  "+strSaveFilapath);
	}
	else
		((CMainFrame*)AfxGetMainWnd())->m_wndOutput.FillText("盘车试验分析：取消保存视图到图像");

	image.Detach(); 

	DeleteDC(gpBmpHdc); 
	::ReleaseDC(hwnd, dc); 
	DeleteObject(oldhdc); 
	DeleteObject(hBitmap); 
}

void CJiggerView::OnBnClickedButton1()
{
	if (oChnDataMean.GetSize()==0||nChnDataMean.GetSize()==0)
	{
		AfxMessageBox("请加载通道数据！");
	}
	else
	{
		int i;
		int p = 8;  //八个方位
		int q = p+4; //未知数个数
		double PI = 3.14159265358979;
		double *alfa = new double[p]; //alpha存放8个方位的传感器所在的角度

		double **X = new double*[q]; //存放最小二乘解，X[0]=c,X[1]=xm,X[2]=ym
		for(i=0;i<q;i++)
		{
			X[i]=new double[1];
		}
		double **h01 = new double*[p];//存放8个间隙值
		for(i=0;i<p;i++)
		{
			h01[i]=new double[1];
		}

		double **h02 = new double*[p];//存放8个间隙值
		for(i=0;i<p;i++)
		{
			h02[i]=new double[1];
		}

		double **h = new double*[2*p];//存放16个间隙值
		for(i=0;i<2*p;i++)
		{
			h[i]=new double[1];
		}

		double **A=new double*[2*p]; //A为方程组系数矩阵
		for(i=0;i<2*p;i++)
		{
			A[i]=new double[q];
		}

		double **AH=new double*[q];//AH为Householder变换后的矩阵
		for(i=0;i<q;i++)
		{
			AH[i]=new double[2*p];
		}

		for(i = 0; i < p; i ++){
			alfa[i] = i * 45;
			alfa[i] = alfa[i]*PI/180.0;
		}
		CreateFullRankHouseholderMatrix(alfa,A,p);

		//double h1[] = {35.5003729487125,67.2592776366006,5.50037294871254,17.2592776366006,
		//	5.86614967479977,10.9659589566327,12.5003703703027,14.7416406841901};
		//double h2[] = {36.5251550601065,65.2821051977173,3.04571202055308,14.5440030997910,
		//	4.48340030518591,11.7237138687060,14.9545569513397,17.4589111849775};

		double h1[JIGGERCOUNT],h2[JIGGERCOUNT];
		for (int k=0;k<JIGGERCOUNT;k++)
		{
			h1[k]=oChnDataMean[k];
			h2[k]=nChnDataMean[k];
		}
		//double h1[] = {35.5003729487125,67.2592776366006,5.50037294871254,17.2592776366006,
		//	5.86614967479977,10.9659589566327,12.5003703703027,14.7416406841901};

		//double h2[] = {36.5251550601065,65.2821051977173,3.04571202055308,14.5440030997910,
		//	4.48340030518591,11.7237138687060,14.9545569513397,17.4589111849775};
		CreateBMatrix(h1,h2,h,p);

		MatrixHouseholderTransfer(A,AH,2*p,q);
		MatrixMultiply(AH,h,X,q,2*p,1);
		
		m_XAxies=X[10][0];
		m_YAxies=X[11][0];

		m_Distance=sqrt((m_XAxies)*(m_XAxies)+(m_YAxies)*(m_YAxies));

		m_Angle=m_Angle>0?180.0*atan2(m_YAxies,m_XAxies)/mPI:(360.0+180.0*atan2(m_YAxies,m_XAxies)/mPI);


		delete[] alfa;
		alfa = NULL; // free alfa

		for (i = 0; i < 2*p; ++i)   
		{   
			delete A[i];
			A[i] = NULL;
		}   
		delete []A;   
		A = NULL; // free A

		for (i = 0; i < q; ++i)   
		{   
			delete AH[i];
			AH[i] = NULL;
		}   
		delete []AH;   
		AH = NULL; //free AH

		for (i = 0; i < 2*p; ++i)   
		{   
			delete h[i];
			h[i] = NULL;
		}   
		delete []h;   
		h = NULL; //free h

		for (i = 0; i < q; ++i)   
		{   
			delete X[i];
			X[i] = NULL;
		}   
		delete []X;   
		X = NULL; // free X

		UpdateData(FALSE);

	}
}

void CJiggerView::OnSetFocus(CWnd* pOldWnd)
{
	CFormView::OnSetFocus(pOldWnd);
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	pCMF->ActivateTabEA(1);
	// TODO: 在此处添加消息处理程序代码
}

void CJiggerView::OnDraw(CDC* )
{
	CWnd* pWnd = GetDlgItem(IDC_CWGRAPH1);
	if(pWnd == NULL)
		return;
	CDC* pDCTrend = pWnd->GetDC();
	
	CRect rc;
	pWnd->GetWindowRect(&rc);
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	CPen pen(PS_SOLID,2,RGB(192,192,192));
	CPen *pOld = pDCTrend->SelectObject(&pen);
	
	pDCTrend->MoveTo(0,0);
	pDCTrend->LineTo(rc.Width(),0);

	pDCTrend->MoveTo(rc.Width(),0);
	pDCTrend->LineTo(rc.Width(),rc.Height());

	pDCTrend->SelectObject(pOld);
	ReleaseDC(pDCTrend);

	///////////////////////////////////////////////////////////
	CWnd* pWnd1 = GetDlgItem(IDC_CWGRAPH2);
	if(pWnd1 == NULL)
		return;
	CDC* pDCTrend1 = pWnd1->GetDC();
	
	pWnd1->GetWindowRect(&rc);
	pWnd1->Invalidate();
	pWnd1->UpdateWindow();

	pOld = pDCTrend1->SelectObject(&pen);
	
	//pDCTrend1->MoveTo(0,0);
	//pDCTrend1->LineTo(rc.Width(),0);

	pDCTrend1->MoveTo(rc.Width(),0);
	pDCTrend1->LineTo(rc.Width(),rc.Height());

	pDCTrend1->SelectObject(pOld);
	ReleaseDC(pDCTrend1);

}