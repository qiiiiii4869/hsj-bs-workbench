// AddChnData.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "AddChnData.h"
#include "MainFrm.h"
#include "NiWaveFraqView.h"
using namespace std;

// CAddChnData 对话框

IMPLEMENT_DYNAMIC(CAddChnData, CHSJDialog)


CNiWaveFraqView* wave;

CAddChnData::CAddChnData(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CAddChnData::IDD, pParent)
	/*, m_strRGB(_T(""))*/
	, m_ParamStr(_T(""))
{
	m_strRGB = _T("255, 0, 255");
	color=RGB (255, 0, 255);
	wave=(CNiWaveFraqView*)pParent;
}

CAddChnData::~CAddChnData()
{
}

void CAddChnData::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR_PICKER, m_ColorPicker);
	DDX_Text(pDX, IDC_RGB, m_strRGB);
	DDX_Text(pDX, IDC_PARAMSTC, m_ParamStr);
	DDX_Control(pDX, IDC_COMBO1, chnNameComb);
}


BEGIN_MESSAGE_MAP(CAddChnData, CHSJDialog)
	ON_BN_CLICKED(IDC_COLOR_PICKER, &CAddChnData::OnBnClickedColorPicker)
	ON_BN_CLICKED(IDOK, &CAddChnData::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddChnData 消息处理程序

void CAddChnData::OnBnClickedColorPicker()
{
	color = m_ColorPicker.GetColor ();

	m_strRGB.Format (_T("%d,%d,%d"), 
		GetRValue (color),
		GetGValue (color),
		GetBValue (color));

	UpdateData (FALSE);
}

BOOL CAddChnData::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	chnNameComb.Clear();
	m_ColorPicker.EnableAutomaticButton (_T("缺省颜色"), RGB (255, 0, 255));
	m_ColorPicker.EnableOtherButton (_T("其它"));
	m_ColorPicker.SetColor ((COLORREF)-1);
	m_ColorPicker.SetColumnsNumber (10);
	
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	pCMF->fileData->SetExamTree(FileStr);
	vector<CString> chnName= pCMF->fileData->GetChnName(FileStr,ParamStr);
	vector<CString>::iterator itc=chnName.begin();

	for (int p=(int)chnName.size()-1;p>=0;p--)
	{
		chnNameComb.InsertString(0,itc[p]);
	}
	chnNameComb.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAddChnData::SetChnData(CString file,CString exmName, CString type, CString param,CString chn)
{
	FileStr=file;
	ExamNameStr=exmName;
	TypeStr=type;
	ParamStr=param;
	chnStr=chn;
	m_ParamStr=param;
}


void CAddChnData::OnBnClickedOk()
{
	CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
	CString str;
	chnNameComb.GetLBText(chnNameComb.GetCurSel(),str);
	if (chnStr==str)
	{
		AfxMessageBox("数据已存在，请重选！");
	}
	else
	{
		//-------------------1.0.0.2------------------------//
		selChnStr=str;		
		//--------------------------------------------------//
		int save_freq=0;
		wave->AddNewWave(save_freq,pCMF->fileData->GetDataFromFile(FileStr,TypeStr,ParamStr,str,&save_freq),color);
		OnOK();
	}
}
