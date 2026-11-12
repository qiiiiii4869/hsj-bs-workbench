// LodDataFFT.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "LodDataFFT.h"
#include "ChildFrm.h"
#include "MainFrm.h"

// CLodDataFFT 对话框

IMPLEMENT_DYNAMIC(CLodDataFFT, CHSJDialog)

CLodDataFFT::CLodDataFFT(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CLodDataFFT::IDD, pParent)
	, m_FilePathName(_T(""))
	, m_Title(_T(""))
	, m_Freq(_T("1024")),fileData()/*,isReadFileOK(TRUE)*/
{
	color=RGB (0, 255, 0);
}

CLodDataFFT::~CLodDataFFT()
{
}

void CLodDataFFT::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR_PICKER, m_ColorPicker);
	DDX_Text(pDX, IDC_RGB, m_strRGB);
	DDX_Text(pDX, IDC_FILENAMEEDIT, m_FilePathName);
	DDX_Text(pDX, IDC_TITLEEDIT, m_Title);
	DDX_Text(pDX, IDC_FREQEDIT, m_Freq);
}


BEGIN_MESSAGE_MAP(CLodDataFFT, CHSJDialog)
	ON_BN_CLICKED(IDC_COLOR_PICKER, &CLodDataFFT::OnBnClickedColorPicker)
	ON_BN_CLICKED(IDOK, &CLodDataFFT::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CLodDataFFT::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLodDataFFT 消息处理程序
//消息：选择波形曲线颜色
void CLodDataFFT::OnBnClickedColorPicker()
{
	color = m_ColorPicker.GetColor ();

	m_strRGB.Format (_T("%d,%d,%d"), 
		GetRValue (color),
		GetGValue (color),
		GetBValue (color));

	UpdateData (FALSE);
}
//消息：初始化
BOOL CLodDataFFT::OnInitDialog()
{
	CHSJDialog::OnInitDialog();
	//设置对话框图标
	HICON   hIcon= AfxGetApp()->LoadIcon(IDI_DLG);   
	SetIcon(hIcon,   TRUE); //   Set   big   icon   
	SetIcon(hIcon,   FALSE); //   Set   small   icon   

	m_ColorPicker.EnableAutomaticButton (_T("缺省颜色"), RGB (0, 255, 0));
	m_ColorPicker.EnableOtherButton (_T("其它"));
	m_ColorPicker.SetColor ((COLORREF)-1);
	m_ColorPicker.SetColumnsNumber (10);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
//消息：加载数据完成FFT运算
void CLodDataFFT::OnBnClickedOk()
{
	UpdateData (TRUE);

	if (m_Title==""||m_Freq==""||m_FilePathName=="")
	{
		AfxMessageBox("请设置正确信息！");
	}
	else
	{	
		if ((int)fileData.size()!=0)
		{
			CMainFrame* pCMF=(CMainFrame*)AfxGetMainWnd();
			CChildFrame* pCFrame = new CChildFrame();
			CCreateContext context;
			context.m_pNewViewClass=RUNTIME_CLASS(CNiWaveFraqView);
			if(!pCFrame->LoadFrame(IDI_EXAM_C,WS_MAXIMIZE|WS_OVERLAPPEDWINDOW|ESB_DISABLE_BOTH,AfxGetApp()->m_pMainWnd,&context))
				return;		
			pCMF->m_PWave->menuShow=FALSE;

			pCFrame->titles=m_Title;
			pCFrame->ShowWindow(SW_SHOW);


			pCMF->m_PWave->SetTitle(m_Title);

			pCMF->m_PWave->LoadDataFFT(fileData,m_Title,color,atoi(m_Freq));
			pCMF->ActivateTabG(0);
			//pCMF->ActivateTabC(0);
			pCFrame->InitialUpdateFrame(NULL,true);	

		}
		else
		{
			AfxMessageBox("数据有误，请重新加载！");
			return;
		}
		
		OnOK();
	}

}
//消息：选取通道数据文件
void CLodDataFFT::OnBnClickedButton1()
{
	CString strFilter="通道数据文件 (*.CHD) | *.chd;*.CHD ||";
	CFileDialog dlg(TRUE,//open/Save as
		NULL,//Ext added automatically
		NULL,//filename
		OFN_HIDEREADONLY |OFN_FILEMUSTEXIST,
		strFilter);
	
	if(dlg.DoModal()==IDOK)
	{		
		m_FilePathName=dlg.GetPathName();
		m_Title = m_FilePathName.Mid(m_FilePathName.ReverseFind('\\')+1);
		m_Title = m_Title.Left(m_Title.ReverseFind('.'));
		FILE * f=fopen(m_FilePathName,"r");
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
			/*fileData.pop_back();*/
			fclose(f);
		}				
	}
	UpdateData (FALSE);
}
