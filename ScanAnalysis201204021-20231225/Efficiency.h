#pragma once
#include "afxwin.h"
#include <vector>

// CEfficiency 对话框

class CEfficiency : public CHSJDialog
{
	DECLARE_DYNAMIC(CEfficiency)

public:
	CEfficiency(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEfficiency();


	double density;			//水密度
	double gravityAcc;		//重力加速度
	double elecEfficiency;	//发电机效率
	double diffElevation;	//差力
	double FCoefficient;	//高程差
	double vInArea;			//蜗壳进口面积
	double vOutArea;		//蜗壳出口面积
	double factorS;			//静水头调整因子
	double factorQ;			//流量传感器调整因子
	
	std::map<CString,double> inPressureM;			
	std::map<CString,double> outPressureM;
	std::map<CString,double> activePowerM;
	std::map<CString,double> FlowmeterM;
	std::map<CString,double> DiffPressSensorM;
	std::map<CString,double> highPressM;
	std::map<CString,double> lowPressM;
	std::map<CString,double> sWaterHeadM;
	std::map<CString,double> sWaterHeadDPM;
	std::map<CString,double> dWaterHeadM;
	std::map<CString,double> WaterHeadM;
	std::map<CString,double> WaterTurbineForceM;

	BOOL isDWhead;
	//BOOL isSWheadDP;

	std::map<CString,double> efficiencyResult;

	double ComputParams(std::vector<float> chnData);

// 对话框数据
	enum { IDD = IDD_Efficiency };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void SaveToText();
	void SaveToExcel(); 
	void GetEfficiencyData();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_density;
public:
	CEdit m_gravityAcc;
public:
	CEdit m_elecEfficiency;
public:
	CEdit m_diffElevation;
public:
	CEdit m_FCoefficient;
public:
	CEdit m_vInArea;
public:
	CEdit m_vOutArea;
public:
	CComboBox m_inPressure;
public:
	CComboBox m_outPressure;
public:
	CButton m_isWHead;

public:
	CComboBox m_activePower;
public:
	CComboBox m_Flowmeter;
public:
	CComboBox m_DiffPressSensor;
public:
	CComboBox m_highPress;
public:
	CComboBox m_lowPress;
public:
	CButton m_isFlowmeter;
public:
	CButton m_isVolutePres;
public:
	CButton m_isDiffPressSensor;
public:
	CButton m_isPressSensor;
public:
	afx_msg void OnBnClickedSavetotext();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedisflowmeter();
public:
	afx_msg void OnBnClickedisvolutepres();
public:
	afx_msg void OnBnClickedisdiffpresssensor();
public:
	afx_msg void OnBnClickedispresssensor();
public:
	CComboBox m_fileNameCmbo;
public:
	CComboBox m_examTypeComb;
public:
	afx_msg void OnCbnSelchangeFilenamecmbo();
public:
	afx_msg void OnCbnSelchangeExamtypecombo();
public:
	afx_msg void OnBnClickedSavetoexcel();
public:
	CStatic m_msg;
public:
	afx_msg void OnBnClickediswhead();
public:
	afx_msg void OnBnClickedCancel();
public:
	CButton m_SheaderChk;
public:
	CComboBox m_SHeaderDiffPress;
public:
	afx_msg void OnBnClickedSheaderchk();
public:
	CEdit m_factorQ;
public:
	CEdit m_factorS;
};
