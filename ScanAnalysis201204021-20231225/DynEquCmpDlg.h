#pragma once


// CDynEquCmpDlg 对话框

class CDynEquCmpDlg : public CHSJDialog
{
	DECLARE_DYNAMIC(CDynEquCmpDlg)

public:
	CDynEquCmpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDynEquCmpDlg();

// 对话框数据
	enum { IDD = IDD_DYNEQUCMPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double BAmplitude;
public:
	double BPhase;
public:
	double AAmplitude;
public:
	double APhase;
public:
	double param5;
public:
	double param7;
public:
	double outWeight;
public:
	double outAngle;
public:
	afx_msg void OnBnClickedOk();

public:
	bool balancingMethod(double amplitude1, double phase1,
					 double amplitude2, double phase2, double angleTest, double weightTest,double& outWeight, double& outAngle);
	bool balancingMethod2(double amplitude1, double phase1,
					  double amplitude2, double phase2, double weightTest,double& outWeight, double& outAngle);
	bool balanceBreak (double angle, double v, double a0, double a1, double *v0, double *v1);
public:
	double m_freqA;
public:
	double param8;
public:
	double param;
public:
	double v0;
public:
	double v1;
public:
	afx_msg void OnBnClickedBreak();
};
