// Set3DFreqDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "Set3DFreqDlg.h"


// CSet3DFreqDlg 对话框

IMPLEMENT_DYNAMIC(CSet3DFreqDlg, CHSJDialog)

CSet3DFreqDlg::CSet3DFreqDlg(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CSet3DFreqDlg::IDD, pParent)
{
	m_brhTl.CreateSolidBrush(RGB(0,0,0));
	m_clrTl = RGB(0,0,0);
	m_clrX = RGB(0,0,0);
	m_clrY = RGB(0,0,0);
	m_clrZ = RGB(0,0,0);

}

CSet3DFreqDlg::~CSet3DFreqDlg()
{

	if(m_brhTl.GetSafeHandle())
		m_brhTl.DeleteObject();

	m_fntTl.DeleteObject();
	m_fntX.DeleteObject();
	m_fntY.DeleteObject();
	m_fntZ.DeleteObject();
}

void CSet3DFreqDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_TLFONT, m_stcTlFont);
	DDX_Control(pDX, IDC_STC_XFONT, m_stcXFont);
	DDX_Control(pDX, IDC_STC_YFONT, m_stcYFont);
	DDX_Control(pDX, IDC_STC_ZFONT, m_stcZFont);
}


BEGIN_MESSAGE_MAP(CSet3DFreqDlg, CHSJDialog)
	ON_BN_CLICKED(IDC_BTN_TITLEFNT, &CSet3DFreqDlg::OnBnClickedBtnTitlefnt)
	ON_BN_CLICKED(IDC_BTN_TITLECLR, &CSet3DFreqDlg::OnBnClickedBtnTitleclr)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_XFONT, &CSet3DFreqDlg::OnBnClickedBtnXfont)
	ON_BN_CLICKED(IDC_BTN_YFONT, &CSet3DFreqDlg::OnBnClickedBtnYfont)
	ON_BN_CLICKED(IDC_BTN_ZFONT, &CSet3DFreqDlg::OnBnClickedBtnZfont)
	ON_BN_CLICKED(IDC_BTN_XCLR, &CSet3DFreqDlg::OnBnClickedBtnXclr)
	ON_BN_CLICKED(IDC_BTN_YCLR, &CSet3DFreqDlg::OnBnClickedBtnYclr)
	ON_BN_CLICKED(IDC_BTN_ZCLR, &CSet3DFreqDlg::OnBnClickedBtnZclr)
	ON_BN_CLICKED(IDOK, &CSet3DFreqDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSet3DFreqDlg 消息处理程序

void CSet3DFreqDlg::OnBnClickedBtnTitlefnt()
{
	// TODO: 在此添加控件通知处理程序代码
	CFontDialog fntDlg;
	fntDlg.m_cf.Flags &= ~CF_EFFECTS;
	fntDlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	LOGFONT lgfnt;
	m_fntTl.GetLogFont(fntDlg.m_cf.lpLogFont);

	if(IDOK == fntDlg.DoModal())
	{
		LOGFONT lf;
		fntDlg.GetCurrentFont(&lf);
		//CFont font;
		m_fntTl.DeleteObject();
		m_fntTl.CreateFontIndirect(&lf);
		//m_fntTl = font;
		//m_stcTlFont.SetFont(&font);
		m_stcTlFont.SetFont(&m_fntTl);
		//font.DeleteObject();
	}
}

void CSet3DFreqDlg::OnBnClickedBtnTitleclr()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_STC_TLCLR);
	//CDC* pDC = pWnd->GetDC();
	
	if(pWnd != NULL)
		pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CColorDialog clrDlg;
	clrDlg.m_cc.Flags |= CC_FULLOPEN;
	if(IDOK == clrDlg.DoModal())
	{
		m_clrTl = clrDlg.GetColor();
		//CBrush brh(m_clrTl);
		//CBrush *pOld = pDC->SelectObject(&brh);
		//pDC->FillRect(&rect,&brh);
		InvalidateRect(&rect);
	}
}

HBRUSH CSet3DFreqDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CHSJDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	int nID = pWnd->GetDlgCtrlID();
	switch(nID)
	{
	case IDC_STC_TLCLR:
		//pDC->SetTextColor(m_clrTl);
		pDC->SetBkMode(TRANSPARENT);
		if(m_brhTl.GetSafeHandle())
		{
			m_brhTl.DeleteObject();
			m_brhTl.CreateSolidBrush(m_clrTl);
			return (HBRUSH)m_brhTl.GetSafeHandle();
		}
		break;
	case IDC_STC_XCLR:
		//pDC->SetTextColor(m_clrX);
		pDC->SetBkMode(TRANSPARENT);
		if(m_brhTl.GetSafeHandle())
		{
			m_brhTl.DeleteObject();
			m_brhTl.CreateSolidBrush(m_clrX);
			return (HBRUSH)m_brhTl.GetSafeHandle();
		}
		break;
	case IDC_STC_YCLR:
		//pDC->SetTextColor(m_clrY);
		pDC->SetBkMode(TRANSPARENT);
		if(m_brhTl.GetSafeHandle())
		{
			m_brhTl.DeleteObject();
			m_brhTl.CreateSolidBrush(m_clrY);
			return (HBRUSH)m_brhTl.GetSafeHandle();
		}
		break;
	case IDC_STC_ZCLR:
		//pDC->SetTextColor(m_clrX);
		pDC->SetBkMode(TRANSPARENT);
		if(m_brhTl.GetSafeHandle())
		{
			m_brhTl.DeleteObject();
			m_brhTl.CreateSolidBrush(m_clrZ);
			return (HBRUSH)m_brhTl.GetSafeHandle();
		}
		break;
	default:
		;
	}
	//if(pWnd->GetDlgCtrlID() == IDC_STC_TLCLR)
	//{
	//	//BYTE    r = GetRValue(m_color);
	//	//r = 255-r;
	//	//BYTE    g = GetGValue(m_color);
	//	//g = 255-g;
	//	//BYTE    b = GetBValue(m_color);
	//	//b = 255-b;
	//	//COLORREF clr = RGB(r,g,b);

	//	//pDC->SetTextColor(m_clrTl);
	//	pDC->SetBkMode(TRANSPARENT);
	//	//pDC->SetBkMode(RGB(255,0,0));
	//	if(m_brhTl.GetSafeHandle())
	//	{
	//		m_brhTl.DeleteObject();
	//		m_brhTl.CreateSolidBrush(m_clrTl);
	//		return (HBRUSH)m_brhTl.GetSafeHandle();
	//	}
	//}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
void CSet3DFreqDlg::OnBnClickedBtnXfont()
{
	CFontDialog fntDlg;
	fntDlg.m_cf.Flags &= ~CF_EFFECTS;
	fntDlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	m_fntX.GetLogFont(fntDlg.m_cf.lpLogFont);
	if(IDOK == fntDlg.DoModal())
	{
		LOGFONT lf;
		fntDlg.GetCurrentFont(&lf);
		m_fntX.DeleteObject();
		m_fntX.CreateFontIndirect(&lf);
		m_stcXFont.SetFont(&m_fntX);
	}
}

void CSet3DFreqDlg::OnBnClickedBtnYfont()
{
	// TODO: 在此添加控件通知处理程序代码
	CFontDialog fntDlg;
	fntDlg.m_cf.Flags &= ~CF_EFFECTS;
	fntDlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	m_fntY.GetLogFont(fntDlg.m_cf.lpLogFont);
	if(IDOK == fntDlg.DoModal())
	{
		LOGFONT lf;
		fntDlg.GetCurrentFont(&lf);
		m_fntY.DeleteObject();
		m_fntY.CreateFontIndirect(&lf);
		m_stcYFont.SetFont(&m_fntY);
	}
}

void CSet3DFreqDlg::OnBnClickedBtnZfont()
{
	// TODO: 在此添加控件通知处理程序代码
	CFontDialog fntDlg;
	fntDlg.m_cf.Flags &= ~CF_EFFECTS;
	fntDlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	m_fntZ.GetLogFont(fntDlg.m_cf.lpLogFont);
	if(IDOK == fntDlg.DoModal())
	{
		LOGFONT lf;
		fntDlg.GetCurrentFont(&lf);
		m_fntZ.DeleteObject();
		m_fntZ.CreateFontIndirect(&lf);
		m_stcZFont.SetFont(&m_fntZ);
	}
}

void CSet3DFreqDlg::OnBnClickedBtnXclr()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_STC_XCLR);
	
	if(pWnd != NULL)
		pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CColorDialog clrDlg;
	clrDlg.m_cc.Flags |= CC_FULLOPEN;
	if(IDOK == clrDlg.DoModal())
	{
		m_clrX = clrDlg.GetColor();
		InvalidateRect(&rect);
	}
}

void CSet3DFreqDlg::OnBnClickedBtnYclr()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_STC_YCLR);
	
	if(pWnd != NULL)
		pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CColorDialog clrDlg;
	clrDlg.m_cc.Flags |= CC_FULLOPEN;
	if(IDOK == clrDlg.DoModal())
	{
		m_clrY = clrDlg.GetColor();
		InvalidateRect(&rect);
	}
}

void CSet3DFreqDlg::OnBnClickedBtnZclr()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_STC_ZCLR);
	
	if(pWnd != NULL)
		pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	CColorDialog clrDlg;
	clrDlg.m_cc.Flags |= CC_FULLOPEN;
	if(IDOK == clrDlg.DoModal())
	{
		m_clrZ = clrDlg.GetColor();
		InvalidateRect(&rect);
	}
}


BOOL CSet3DFreqDlg::OnInitDialog() 
{
	CHSJDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_stcTlFont.SetFont(&m_fntTl);
	m_stcXFont.SetFont(&m_fntX);
	m_stcYFont.SetFont(&m_fntY);
	m_stcZFont.SetFont(&m_fntZ);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CSet3DFreqDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	OnOK();
}
