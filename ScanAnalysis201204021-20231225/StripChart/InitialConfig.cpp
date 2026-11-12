// InitialConfig.cpp : implementation file
//

#include "stdafx.h"
#include "..\ScanAnalysis.h"
#include "..\MainFrm.h"
#include "..\ChildFrm.h"
//#include "NaviBar.h"
#include "..\ScanAnalysisDoc.h"
#include "boxingView.h"
#include "InitialConfig.h"
#include "ConfigX.h"

#define IDC_LIST 101
#define IDC_TREE 102

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CInitialConfig dialog
//COLORREF color[10] = {RGB(255, 0, 0), RGB(0, 0, 255), RGB(128, 128, 0), RGB(255, 0, 255), 
					//RGB(0, 255, 64),RGB(128, 0, 255), RGB(0, 0, 0), RGB(255, 128, 0), 
				//RGB(236, 236, 0), RGB(64, 128, 128)};

//重新设置颜色//Marshall  2011.3.22 9:40

//COLORREF color[10] = {RGB(255, 0, 0), RGB(0, 0, 255),
//
//RGB(64, 0, 128)/*RGB(64, 128, 128)*/ /*RGB(128, 128, 0)*/, RGB(255, 0, 255), 
//
//RGB(0, 64, 0)/*RGB(0, 0, 0)*/,
//
//
//RGB(255, 0, 0), RGB(0, 0, 255), 
//
//RGB(64, 0, 128)/*RGB(64, 128, 128)*/, RGB(255, 0, 255), 
//
//RGB(0, 64, 0)};

COLORREF color[10] = {

RGB(255, 0, 0), RGB(0, 0, 255),

RGB(0, 64, 0)/*RGB(64, 128, 128)*/ /*RGB(128, 128, 0)*/, RGB(255, 0, 255), 

RGB(64, 0, 128)/*RGB(0, 0, 0)*/,


RGB(255, 0, 0), RGB(0, 0, 255),

RGB(0, 64, 0)/*RGB(64, 128, 128)*/ /*RGB(128, 128, 0)*/, RGB(255, 0, 255), 

RGB(64, 0, 128)/*RGB(0, 0, 0)*/};

CInitialConfig::CInitialConfig(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CInitialConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInitialConfig)
	m_Size = _T("");
	m_Height = 0;
	m_Width = 0;
	m_Rows = 0;
	m_Columns = 0;
	m_yGridCount = 0;
 
	//}}AFX_DATA_INIT
}


void CInitialConfig::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitialConfig)
	DDX_Control(pDX, IDC_BUTTON2, m_buttonopp);
	DDX_Control(pDX, IDC_BUTTON1, m_buttonall);
	DDX_Control(pDX, IDC_EDIT1, m_yGridCountEdit);
	DDX_Control(pDX, IDC_EDIT9, m_ColumnsEdit);
	DDX_Control(pDX, IDC_EDIT8, m_RowsEdit);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_EDIT7, m_WidthEdit);
	DDX_Control(pDX, IDC_EDIT6, m_HeightEdit);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_CBString(pDX, IDC_COMBO1, m_Size);
	DDX_Text(pDX, IDC_EDIT6, m_Height);
	DDX_Text(pDX, IDC_EDIT7, m_Width);
	DDX_Text(pDX, IDC_EDIT8, m_Rows);
	DDV_MinMaxInt(pDX, m_Rows, 1, 1000);
	DDX_Text(pDX, IDC_EDIT9, m_Columns);
	DDV_MinMaxInt(pDX, m_Columns, 1, 1000);
	DDX_Text(pDX, IDC_EDIT1, m_yGridCount);
	DDV_MinMaxInt(pDX, m_yGridCount, 1, 10);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO2, m_param);
}


BEGIN_MESSAGE_MAP(CInitialConfig, CHSJDialog)
	//{{AFX_MSG_MAP(CInitialConfig)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP

	ON_CBN_SELCHANGE(IDC_COMBO2, &CInitialConfig::OnCbnSelchangeCombo2)

	ON_BN_CLICKED(IDOK, &CInitialConfig::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInitialConfig message handlers

BOOL CInitialConfig::OnInitDialog() 
{	
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	CHSJDialog::OnInitDialog();	
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_TabCtrl.MoveWindow(10,50,223,260);
	m_List.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, CRect(15,72,228,280), this, IDC_LIST);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_List.InsertColumn(0, _T("信号"), LVCFMT_LEFT, 175);
	//vector<CString>::iterator paramIt=paramVector.begin();
	for (int p=(int)paramVector.size()-1;p>=0;p--)
	{
		m_param.InsertString(0,paramVector[p]);
	}
	m_param.SetCurSel(0);
	
	//获取通道的采样频率
			
	//map<CString,CString>::iterator its=pCMF->fileData->samFreq.begin();
	//for (;its!=pCMF->fileData->samFreq.end();its++)
	//{
	//	if (its->first==pCMF->FilePathName)
	//	{
	//		samplFreq=atoi(its->second);
	//	}
	//}
	
	//填充通道名称到目录树
	FillList();

	m_HeightEdit.SetWindowText("2800");
	m_WidthEdit.SetWindowText("2000");
	m_yGridCountEdit.SetWindowText("3");
	m_ColumnsEdit.SetWindowText("20");
	m_RowsEdit.SetWindowText("60");

	m_TabCtrl.InsertItem(0, "单通道显示");
	//m_TabCtrl.InsertItem(1, "分组通道显示");////Marshall  2011.2.20 22:17
	m_Combo.AddString("A4竖排");
	m_Combo.AddString("A4横排");
	m_Combo.AddString("A3竖排");
	m_Combo.AddString("A3横排");
	m_Combo.SelectString(-1, "A4竖排");

	return TRUE;  
}

void CInitialConfig::OnSelchangeCombo1() 
{
	 
	if (m_Combo.GetCurSel() == 0)
	{
		m_HeightEdit.SetWindowText("2800");
		m_WidthEdit.SetWindowText("2000");
	}
	else if (m_Combo.GetCurSel() == 1)
	{
		m_HeightEdit.SetWindowText("2000");
		m_WidthEdit.SetWindowText("2800");
	}
	else if (m_Combo.GetCurSel() == 2)
	{
		m_HeightEdit.SetWindowText("5600");
		m_WidthEdit.SetWindowText("4000");
	}
	else
	{
		m_HeightEdit.SetWindowText("4000");
		m_WidthEdit.SetWindowText("5600");
	}
}

void CInitialConfig::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString paramStr;
	m_param.GetLBText(m_param.GetCurSel(),paramStr);

	if (m_TabCtrl.GetCurSel() == 0)//添加通道信号
	{	
		m_TabCtrl.MoveWindow(10,50,223,260);
		m_Tree.DestroyWindow();
		m_List.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT, CRect(15,72,228,280), this, IDC_LIST);
		m_List.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
		m_List.InsertColumn(0, _T("信号"), LVCFMT_LEFT, 175);
		m_buttonall.ShowWindow(TRUE);
		m_buttonopp.ShowWindow(TRUE);
		FillList();
	}
	else							//添加分组信号
	{
		m_TabCtrl.MoveWindow(10,50,223,236);
		m_List.DestroyWindow();
		m_buttonall.ShowWindow(FALSE);
		m_buttonopp.ShowWindow(FALSE);
		m_Tree.Create(WS_VISIBLE|WS_CHILD|WS_BORDER|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_HASLINES|TVS_DISABLEDRAGDROP|TVS_NOTOOLTIPS, CRect(15,72,228,280), this, IDC_TREE);
		//m_Tree.SetChnInit(filePath,examType,paramStr,samplFreq);	
		m_Tree.flag = 0;
		m_Tree.m_HRoot = m_Tree.InsertItem("选择分组");
		m_Tree.InsertItem("第1组", m_Tree.m_HRoot);
		m_Tree.InsertItem("第2组", m_Tree.m_HRoot);
	}
	*pResult = 0;
}

void CInitialConfig::OnOK()
{	
	UpdateData(TRUE);

	/*--------------------------Marshall's Code2011.2.27 21:34--------------*/
	//如果没有选择通道信号，则弹出警告
	int nSelChnCnt = 0;
	for (int i = 0; i < m_List.GetItemCount(); i++)
	{
		if (m_List.GetCheck(i))
		{	
			nSelChnCnt++;
			break;
		}
		//else
		//{//在这里做判断，逻辑上有错误，已更正//Marshall  2011.3.22 19:34
		//	MessageBox("请选择信号！");
		//	return;
		//}
	}

	if(nSelChnCnt <= 0 )
	{
		MessageBox("请选择信号！");
		return;
	}
	/*--------------------------Marshall's Code2011.2.27 21:34--------------*/

	CString string1, string2;
	m_HeightEdit.GetWindowText(string1);
	m_RowsEdit.GetWindowText(string2);
	int height = ::atoi(string1);
	if (::atoi(string1) / ::atoi(string2) < 15)
	{
		MessageBox("网格线太密，请重新设置参数！");
		return;
	}
	m_WidthEdit.GetWindowText(string1);
	m_ColumnsEdit.GetWindowText(string2);
	if (::atoi(string1) / ::atoi(string2) < 15)
	{
		MessageBox("网格线太密，请重新设置参数！");
		return;
	}	
	if (height * ::atoi(string1) > 22500000)
	{
		MessageBox("视图太大，请重新设置参数！");
		return;
	}
	if (height < 300 || ::atoi(string1) < 300)
	{
		MessageBox("视图太小，请重新设置参数！");
		return;
	}

	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();

	CChildFrame* pCFrame = new CChildFrame();
	CCreateContext context;
	context.m_pNewViewClass=RUNTIME_CLASS(CBoxingView);
	if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
		return;
	CString paramStr;
	m_param.GetLBText(m_param.GetCurSel(),paramStr);
	pCFrame->titles=/*"波形长图";*/examType+"_"+paramStr+" 综合波形长图";

	//int dataLength=pCMF->fileData->GetParaDataLength(filePath,examType,paramStr);
	//float dataTime=(float)dataLength/(float)samplFreq;



	pCFrame->InitialUpdateFrame(NULL,true);		
	CBoxingView *pView = (CBoxingView *)pCFrame->GetActiveView();
	
	if (m_TabCtrl.GetCurSel() == 0)
	{
		ReadFromList(filePath,examType,paramStr,/*samplFreq,*/   m_List, pView);
	}
	else
	{
		ReadFromTree(filePath,examType,paramStr,/*samplFreq,*/ m_Tree, pView);	
	}
	float dataTime=(float)vDataLength[0]/(float)vSamplingFreq[0];
	pView->SetInit(CSize(m_Width, m_Height),1,m_yGridCount,m_Rows,m_Columns,0.0f,dataTime,(dataTime - 0.0f) / (float)m_Columns);
	pView->SetChnInit(filePath,examType,paramStr/*,samplFreq*/);

	//pView->AdjustZuobiao((dataTime - 0) / m_Columns, 0, dataTime, m_Columns);	
	//pView->SetConfigX(0.0, dataTime, m_Columns);

	pView->DrawGraph();
	pCFrame->ShowWindow(SW_SHOW);

	
	CString string;
	m_yGridCountEdit.GetWindowText(string);
	InfoComplete(pView, 0, ::atoi(string));

	pView->m_title = examType+"_"+paramStr+" 综合波形长图";
	pView->GetParentFrame()->SetWindowText(examType+"_"+paramStr+" 综合波形长图");

	//CConfigX configx;
	//if (configx.DoModal() == IDOK)
	//{

	//	pView->AdjustZuobiao((dataTime - 0.0) / m_Columns, 0.0, dataTime, m_Columns);
	//	pView->Invalidate();
	//}
 
    pView->Invalidate(TRUE);
	CHSJDialog::OnOK();
}

float CInitialConfig::GetYMax(CBoxingView::LOGPOINT *point, int count)
{
	float max = point[0].y;
	for(int i = 1; i < count; i++)
	{
		if (point[i].y > max)
			max = point[i].y;
	}
	return max;
}

float CInitialConfig::GetYMin(CBoxingView::LOGPOINT *point, int count)
{
	float min = point[0].y;
	for(int i = 1; i < count; i++)
	{
		if (point[i].y < min)
			min = point[i].y;
	}
	return min;
}

void CInitialConfig::ReadFromList(CString filePath,CString examtype,CString paramStr,/*int sFreq,*/  CListCtrl &m_List, CBoxingView *pView)  /////读文件
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();


	for (int i = 0; i < m_List.GetItemCount(); i++)
	{
		if (m_List.GetCheck(i))
		{			
			CBoxingView::BOXING b;
			b.title = m_List.GetItemText(i, 0)+"("+pCMF->fileData->GetChnUnit(filePath,m_List.GetItemText(i, 0))+")";
			int save_freq=0;
			vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examtype, paramStr,m_List.GetItemText(i, 0),&save_freq);
			int len=pCMF->fileData->GetChnDataLength(filePath,examtype,paramStr,m_List.GetItemText(i, 0));
			
			vSamplingFreq.push_back(save_freq);
			vDataLength.push_back(len);
			
			b.count = (int)chnData.size();
			b.logpoint = new CBoxingView::LOGPOINT[b.count];
			//file.ReadString(string);
			b.frequency = save_freq;
			for (int j = 0; j < (int)chnData.size(); j++)
			{						
				b.logpoint[j].x = (float)j/(float)save_freq;
				b.logpoint[j].y = chnData[j];	
			}				
			b.pNext = NULL;
			pView->boxing.Add(b);
		}
	}

}

void CInitialConfig::ReadFromTree(CString filePath,CString examtype,CString paramStr, /*int sFreq,*/  CTree &m_Tree, CBoxingView *pView)  //////读文件
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();

	CString string;
	//DWORD pos = file.GetPosition();
	HTREEITEM firstG = m_Tree.GetChildItem(m_Tree.GetRootItem());
	while (firstG)
	{
		//file.Seek(pos, CFile::begin);
		if (m_Tree.GetChildItem(firstG))
		{

			CBoxingView::BOXING b;
			b.title = m_Tree.GetItemText(m_Tree.GetChildItem(firstG))+"("+pCMF->fileData->GetChnUnit(filePath,m_Tree.GetItemText(m_Tree.GetChildItem(firstG)))+")";
			int save_freq=0;
			vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examtype, paramStr,m_Tree.GetItemText(m_Tree.GetChildItem(firstG)),&save_freq);
			
			int len=pCMF->fileData->GetChnDataLength(filePath,examtype,paramStr,m_Tree.GetItemText(m_Tree.GetChildItem(firstG)));
			vSamplingFreq.push_back(save_freq);
			vDataLength.push_back(len);
			b.count = (int)chnData.size();
			b.logpoint = new CBoxingView::LOGPOINT[b.count];
			//file.ReadString(string);
			b.frequency = save_freq;
			for (int j = 0; j < b.count; j++)
			{						
				b.logpoint[j].x = (float)j/(float)save_freq;
				b.logpoint[j].y = chnData[j];	
			}				
			b.pNext = NULL;

			HTREEITEM firstS = m_Tree.GetChildItem(firstG);
			CBoxingView::BOXING *p = &b;
			while (firstS && m_Tree.GetNextSiblingItem(firstS))
			{
				CBoxingView::BOXING *p1 = new CBoxingView::BOXING;
				p1->title = m_Tree.GetItemText(m_Tree.GetNextSiblingItem(firstS))+"("+pCMF->fileData->GetChnUnit(filePath,m_Tree.GetItemText(m_Tree.GetNextSiblingItem(firstS)))+")";
				int save_freq=0;
				vector<float> chnData=pCMF->fileData->GetDataFromFile(filePath,examtype, paramStr,m_Tree.GetItemText(m_Tree.GetNextSiblingItem(firstS)),&save_freq);
				
				int len=pCMF->fileData->GetChnDataLength(filePath,examtype,paramStr,m_Tree.GetItemText(m_Tree.GetChildItem(firstG)));
				vSamplingFreq.push_back(save_freq);
				vDataLength.push_back(len);

				p1->count = (int)chnData.size();
				p1->logpoint = new CBoxingView::LOGPOINT[p1->count];
				//file.ReadString(string);
				p1->frequency = save_freq;
				for (int j = 0; j < p1->count; j++)
				{						
					p1->logpoint[j].x = (float)j/(float)save_freq;
					p1->logpoint[j].y = chnData[j];	
				}	
				
				p1->pix = 1;
				p1->pNext = NULL;
				p->pNext = p1;
				p = p->pNext;
				firstS = m_Tree.GetNextSiblingItem(firstS);
			}
			pView->boxing.Add(b);
		}
		firstG = m_Tree.GetNextSiblingItem(firstG);
	}	
}

void CInitialConfig::InfoComplete(CBoxingView *pView, int start, int gridc)
{	
	for (int j = start; j < pView->boxing.GetSize(); j++)
	{
		pView->boxing[j].startindex = (gridc + 1) * j;
		pView->boxing[j].endindex = gridc + (gridc + 1) * j;
		pView->boxing[j].ymin = GetYMin(pView->boxing[j].logpoint, pView->boxing[j].count);
		pView->boxing[j].ymax = GetYMax(pView->boxing[j].logpoint, pView->boxing[j].count);

		//////////////////////////////////////////////////////////////////////////
		pView->boxing[j].ymin=(int)pView->boxing[j].ymin-1;
		pView->boxing[j].ymax=(int)pView->boxing[j].ymax+1;
		//////////////////////////////////////////////////////////////////////////

		pView->boxing[j].color = color[j % 10];
		pView->boxing[j].pix = 1;
		pView->boxing[j].pos = -1;
		pView->boxing[j].offsety = 0;
		int k = j;
		CBoxingView::BOXING *p = pView->boxing[j].pNext;
		while (p)
		{
			p->color = color[(++k) % 10];
			float m = GetYMin(p->logpoint, p->count);
			if (m < pView->boxing[j].ymin)
				pView->boxing[j].ymin = m;
			m = GetYMax(p->logpoint, p->count);
			if (m > pView->boxing[j].ymax)
				pView->boxing[j].ymax = m;

			p = p->pNext;
		}
		pView->boxing[j].yinterval = (pView->boxing[j].ymax - pView->boxing[j].ymin) / (pView->boxing[j].endindex - pView->boxing[j].startindex);
		//pView->AdjustZuobiao(pView->boxing[j].yinterval, pView->boxing[j].ymin, pView->boxing[j].ymax, pView->boxing[j].endindex - pView->boxing[j].startindex);					
	}
}

void CInitialConfig::OnButton1() 
{
	// TODO: Add your control notification handler code here
	int count = m_List.GetItemCount();
	for (int i = 0; i < count; i++)
		m_List.SetCheck(i);	

	CString string, string1;
	m_RowsEdit.GetWindowText(string);
	int rows = ::atoi(string);
	m_yGridCountEdit.GetWindowText(string1);
	while ((::atoi(string1) + 1) * count > rows)
		rows++;
	string.Format("%d", rows);
	m_RowsEdit.SetWindowText(string);
}

void CInitialConfig::OnButton2() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_List.GetItemCount(); i++)
	{
		if (m_List.GetCheck(i))
			m_List.SetCheck(i, FALSE);
		else
			m_List.SetCheck(i);
	}
	int count = 0;
	for (int i = 0; i < m_List.GetItemCount(); i++)
	{
		if (m_List.GetCheck(i))
			count++;
	}
	CString string, string1;
	m_RowsEdit.GetWindowText(string);
	int rows = ::atoi(string);
	m_yGridCountEdit.GetWindowText(string1);
	while ((::atoi(string1) + 1) * count > rows)
		rows++;
	string.Format("%d", rows);
	m_RowsEdit.SetWindowText(string);
}



void CInitialConfig::OnCbnSelchangeCombo2()
{
	FillList();
}


void CInitialConfig::FillList()
{
	CMainFrame* pCMF = (CMainFrame*)AfxGetMainWnd();
	m_List.DeleteAllItems();
	CString paramStr;
	m_param.GetLBText(m_param.GetCurSel(),paramStr);
	vector<CString> chnVector=pCMF->fileData->GetChnName(filePath,paramStr);
	for (int p=0;p<(int)chnVector.size();p++)
	{
		m_List.InsertItem(p, chnVector[p]);
	}
}



void CInitialConfig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
