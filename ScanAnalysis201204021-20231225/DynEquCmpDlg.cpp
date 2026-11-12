// DynEquCmpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAnalysis.h"
#include "DynEquCmpDlg.h"


// CDynEquCmpDlg 对话框

IMPLEMENT_DYNAMIC(CDynEquCmpDlg, CHSJDialog)

CDynEquCmpDlg::CDynEquCmpDlg(CWnd* pParent /*=NULL*/)
	: CHSJDialog(CDynEquCmpDlg::IDD, pParent)
	, BAmplitude(0)
	, BPhase(0)
	, AAmplitude(0)
	, APhase(0)
	, param5(0)
	, param7(0)
	, outWeight(0)
	, outAngle(0)
	, m_freqA(0)
	, param8(0)
	, param(0)
	, v0(0)
	, v1(0)
{

}

CDynEquCmpDlg::~CDynEquCmpDlg()
{
}

void CDynEquCmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CHSJDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgChaToXls)
	//DDX_Control(pDX, IDC_EDIT_ENDTIME, m_editEndTime);
	//DDX_Control(pDX, IDC_EDIT_BEGINTIME, m_editBeginTime);
	//DDX_Control(pDX, IDC_CHA, m_cmbCha);
	//DDX_Control(pDX, IDC_EDIT_XLSPATH, m_editXlsPath);
	//DDX_Text(pDX, IDC_EDIT_TIMEINTERVAL, m_editTimeInterval);
	//DDV_MinMaxLong(pDX, m_editTimeInterval, 1, 36000);
	DDX_Text(pDX, IDC_AMP_B, BAmplitude);
	DDX_Text(pDX, IDC_PHASE_B, BPhase);
	//DDV_MinMaxDouble(pDX, BPhase, 0.0, 360.0);

	DDX_Text(pDX, IDC_AMP_A, AAmplitude);
	DDX_Text(pDX, IDC_PHASE_A, APhase);
	//DDV_MinMaxDouble(pDX, APhase, 0.0, 360.0);

	DDX_Text(pDX, IDC_OUTWEIGHT, outWeight);
	DDX_Text(pDX, IDC_OUTANGLE, outAngle);
	//DDV_MinMaxDouble(pDX, outAngle, 0.0, 360.0);

	DDX_Text(pDX, IDC_PARAM5, param5);
	DDX_Text(pDX, IDC_PARAM7, param7);
	//DDV_MinMaxDouble(pDX, param7, 0.0, 360.0);

	DDX_Text(pDX, IDC_EDIT17, v0);
	DDX_Text(pDX, IDC_EDIT19, v1);

	DDX_Text(pDX, IDC_EDIT18, param8);
	//DDV_MinMaxDouble(pDX, param8, 0.0, 360.0);
	DDX_Text(pDX, IDC_EDIT20, param);
	//DDV_MinMaxDouble(pDX, param, 0.0, 360.0);
//	DDX_Text(pDX, IDC_EDIT_ENDTIME, m_strEndTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDynEquCmpDlg, CHSJDialog)
	ON_BN_CLICKED(IDOK, &CDynEquCmpDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_BREAK, &CDynEquCmpDlg::OnBnClickedBreak)
END_MESSAGE_MAP()


// CDynEquCmpDlg 消息处理程序

/**
* 动平衡计算1，计算出配重方向与试重方向之间的偏角、重量
* @param amplitude1, phase1 原始幅值、相位(弧度)
* @param amplitude2, phase2, weightTest 试重幅值、相位、重量
* @param outWeight, outAngle 输出配重重量、与试重位置的角度差（逆时针）
*/
bool CDynEquCmpDlg::balancingMethod2(double amplitude1, double phase1,
					  double amplitude2, double phase2, double weightTest,double& outWeight, double& outAngle)
{
	double influenceAngle = 0;
	// 计算试重块引起的振动幅值 amplitude3
	double amplitude3 = sqrt(amplitude1 * amplitude1 + amplitude2 * amplitude2
		- 2 * amplitude1 * amplitude2 * cos( (phase1 - phase2) * mPI / 180));

	//若为 0 则需调整试重角度
	if(amplitude3 != 0)
	{
		//计算配重大小、影响系数、影响系数角
		double weightBalance = amplitude1 * weightTest / amplitude3;
		double influenceCoeff = amplitude3 / weightTest;
		double k = (amplitude2 * sin(phase2 * mPI / 180)
			- amplitude1 * sin(phase1 * mPI / 180)) / (amplitude2 * cos(phase2 * mPI / 180) - amplitude1 * cos(phase1 * mPI / 180));
		double kangle1 = (atan(k)) * 180 / mPI;
		double kangle2 = kangle1 + 180;

		// 计算影响的
		if (amplitude2 * cos(phase2 * mPI / 180) < amplitude1 * cos(phase1 * mPI / 180))
			influenceAngle = phase1 + 180 - kangle2;
		else
			influenceAngle = phase1 + 180 - kangle1;

		if (influenceAngle < 0)
			influenceAngle = influenceAngle + 360;

		outWeight = weightBalance;
		outAngle = influenceAngle;
		return true;
	}
	else
	{
		return false;
	}

	return true;

}

/**
* 动平衡计算2, 计算出配重角度、重量
* @param amplitude1, phase1 原始幅值、相位(弧度)
* @param amplitude2, phase2, angleTest, weightTest 试重幅值、相位、角度、重量
* @param outWeight, outAngle 输出配重重量、与试重位置的绝对角度
*/
bool CDynEquCmpDlg::balancingMethod(double amplitude1, double phase1,
					 double amplitude2, double phase2, double angleTest, double weightTest,double& outWeight, double& outAngle)
{
	double influenceAngle;
	if (!balancingMethod2 (amplitude1, phase1,
		amplitude2, phase2, weightTest,
		outWeight, influenceAngle))
		return false;
	//计算配重角
	outAngle = angleTest + influenceAngle;
	while (outAngle < 0)
		outAngle += 360;

	while (outAngle > 360)
		outAngle -= 360;

	return true;
}

//balanceBreak (outAngle, outWeight, param8, param9, &v0, &v1)

/**
* 将配重方案分解到两个指定角度
*	@param angle	配重角
*	@param v		配重重量
*	@param a0, a1	分解配重角, 自动根据angle的角度确定方向
*	@param v0, v1	输出计算出来的角度
*/
bool CDynEquCmpDlg::balanceBreak (double angle, double v, double a0, double a1, double *v0, double *v1)
{
	// 校正a0到 0 ~ 360 之间
	while (a0 < 0)
		a0 += 360;
	while (a0 > 360)
		a0 -= 360;

	// 校正a1到 0 ~ 360 之间
	while (a1 < 0)
		a1 += 360;
	while (a1 > 360)
		a1 -= 360;

	// 校正a1到 0 ~ 360 之间
	while (angle < 0)
		angle += 360;
	while (angle > 360)
		angle -= 360;

	// 如果 a0 < (a1 + 180), 则交换a0, a1
	if (a0 > a1)
	{
		if (a0 <= (a1 + 180))
			std::swap (a0, a1);
	}
	else if (a0 < a1)
	{
		if (a1 > (a0 + 180))
			std::swap (a0, a1);
	}

	if (a0 > a1)
		a1 += 360;

	// 如果angle不在 a0 -> a0 + 180 范围内，为错误
	if (angle < a0 || angle > a1)
		return false;

	double A0 = abs(a0 - angle) * mPI / 180;
	double A1 = abs(angle - a1) * mPI / 180;

	if (A0 == 0)
	{
		*v0 = v;
		*v1 = 0;
	}
	else if (A1 == 0)
	{
		*v0 = 0;
		*v1 = v;
	}
	else
	{
		double d = v / (1/tan(A0) + 1/tan(A1));		// 垂线长度
		*v0 = d / sin (A0);
		*v1 = d / sin (A1);
	}

	return true;
}


//配重方案计算
void CDynEquCmpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	//做参数判断
	if(BPhase > 360.0 || BPhase < 0.0)
	{
		AfxMessageBox("超出角度范围，请输入一个0～360之间的数值！");
		return;
	}

	if(APhase > 360.0 || APhase < 0.0)
	{
		AfxMessageBox("超出角度范围，请输入一个0～360之间的数值！");
		return;
	}

	//if(outAngle > 360.0 || outAngle < 0.0)
	//{
	//	AfxMessageBox("超出角度范围，请输入一个0～360之间的数值！");
	//	return;
	//}

	if(param7 > 360.0 || param7 < 0.0)
	{
		AfxMessageBox("超出角度范围，请输入一个0～360之间的数值！");
		return;
	}

	bool is_ok = balancingMethod(BAmplitude, BPhase,
		AAmplitude, APhase, param7,param5, outWeight, outAngle);

	if (!is_ok)
	{
		AfxMessageBox("输入参数不正确，无法计算出正确结果！");
			return;
	}

	UpdateData(false);
	//OnOK();
}

void CDynEquCmpDlg::OnBnClickedBreak()
{
	UpdateData(true);
	//做参数判断
	if(param8 > 360.0 || param8 < 0.0)
	{
		AfxMessageBox("超出角度范围，请输入一个0～360之间的数值！");
		return;
	}

	if(param > 360.0 || param < 0.0)
	{
		AfxMessageBox("超出角度范围，请输入一个0～360之间的数值！");
		return;
	}

	bool is_ok = balanceBreak(outAngle, outWeight, param8, param, &v0, &v1);

	if (!is_ok || v0==0.0 || v1==0.0)
	{
		AfxMessageBox("输入参数不正确，无法计算出正确结果！");
		return;
	}
	UpdateData(false);
}
