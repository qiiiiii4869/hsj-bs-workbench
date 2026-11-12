// 3DFreqShowCurveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "3DFreqShowCurveDlg.h"


// C3DFreqShowCurveDlg 对话框

IMPLEMENT_DYNAMIC(C3DFreqShowCurveDlg, CHSJDialog)

C3DFreqShowCurveDlg::C3DFreqShowCurveDlg(CWnd* pParent /*=NULL*/)
	: CHSJDialog(C3DFreqShowCurveDlg::IDD, pParent)
{

	m_pWnd = NULL;

}

C3DFreqShowCurveDlg::~C3DFreqShowCurveDlg()
{
}

void C3DFreqShowCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSCCURVE, m_lscShowCurve);
}


BEGIN_MESSAGE_MAP(C3DFreqShowCurveDlg, CHSJDialog)
	ON_BN_CLICKED(IDOK, &C3DFreqShowCurveDlg::OnBnClickedOk)
	//ON_NOTIFY(NM_CLICK, IDC_LSCCURVE, &C3DFreqShowCurveDlg::OnClickListCurvename)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSCCURVE, &C3DFreqShowCurveDlg::OnItemchangedListCurvename)
	ON_BN_CLICKED(IDCANCEL, &C3DFreqShowCurveDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// C3DFreqShowCurveDlg 消息处理程序

void C3DFreqShowCurveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL C3DFreqShowCurveDlg::OnInitDialog() 
{
	CHSJDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lscShowCurve.SetExtendedStyle(  LVS_EX_CHECKBOXES| LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);

	CRect rect;
	m_lscShowCurve.GetWindowRect(&rect);
	int nWidth = rect.Width();

	m_lscShowCurve.InsertColumn(0,_T("功况"),LVCFMT_LEFT,nWidth, 0);

	int nCnt = m_listSet.GetCount();

	//if(m_listSet.GetCount() > 0)
	//if(nCnt > 0)
	//{
		//CRect rect;
		//m_listCurve.GetWindowRect(&rect);

		//int   nWidth[2];
		//nWidth[0] = rect.Width() * 1.5/5, nWidth[1] = rect.Width() * 3.5/5;
		//int nWidth = rect.Width();
/*
		for(int i = 0; i < 2; i++)
			m_listcurve.insertcolumn(i, strclmtext[i], 
			lvcfmt_left,nwidth[i], 0);*/	
		
		//int nCnt = m_listSet.GetCount();
		if(nCnt > 0)
		{
			int iItem, iSubItem, iActualItem;
			LV_ITEM lvitem;
			POSITION pos = m_listSet.GetHeadPosition();
			for (iItem = 0; iItem < nCnt; iItem++)  // insert the items and subitems into the list view.
			{
				CurveSet CS;
				CS = m_listSet.GetNext(pos);
				CString strNO;
				//iActualItem = m_listCurve.InsertItem(&lvitem);
				//strNO.Format("%d",iItem + 1);
				//for (iSubItem = 0; iSubItem < 2; iSubItem++)
				//{
					lvitem.mask = LVIF_TEXT;
				//	lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
					lvitem.iItem = iItem;
					lvitem.iSubItem = 0;
				//	
					CString str =  CS.strCurveName;
				//	if(iSubItem == 0)
				//	{
						lvitem.pszText = str.GetBuffer(strNO.GetLength());
				//		iActualItem = m_listCurve.InsertItem(&lvitem);
						m_lscShowCurve.InsertItem(&lvitem);
				//	}
				//	else
				//	{
				//		lvitem.pszText =str.GetBuffer(str.GetLength());
				//		m_listCurve.SetItem(&lvitem); 
				//	}					
				//}
				str.ReleaseBuffer();
				m_lscShowCurve.SetCheck(iItem, CS.bVisible);
			}
			
		}
	//}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DFreqShowCurveDlg::OnItemchangedListCurvename(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CNi3DWaveView* pView = (CNi3DWaveView*) m_pWnd;
	if(m_lscShowCurve.GetCheck(nItem))
	{
		if(pView != NULL)
			pView->m_graph.Plots.Item(nItem + 1).Visible = true;	
	}
	else
	{
		if(pView != NULL)
			pView->m_graph.Plots.Item(nItem + 1).Visible = false;	
	}

	*pResult = 0;
}
void C3DFreqShowCurveDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CNi3DWaveView* pView = (CNi3DWaveView*) m_pWnd;

	if(pView != NULL)
	{
		int nCnt = pView->m_graph.GetPlots().Count;
		POSITION pos = m_listSet.GetHeadPosition();

		//int nCnt = pView->m_graph.GetPlots().Count();
		for(int i = 0; i < nCnt; i++)
		{
			CurveSet CS;
			CS = m_listSet.GetNext(pos);

			CNiPlot3D  plot = pView->m_graph.Plots.Item(i+1);
			pView->m_graph.Plots.Item(i + 1).Visible = CS.bVisible;	
			//CString strPlotName = plot.GetName();
			//bool	bVisible = plot.Visible;
			//CurveSet curveset;
			//curveset.strCurveName = strPlotName;
			//curveset.bVisible = bVisible;
			//dlg.m_listSet.AddTail(curveset);
		}
	}

	OnCancel();
}
