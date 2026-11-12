#pragma once
#include "afxwin.h"


// CAirGap 对话框
#define MAXPOINTS 2048
class CAirGap : public CHSJDialog
{
	DECLARE_DYNAMIC(CAirGap)

public:
	CAirGap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAirGap();

	double	m_x;			//x偏心坐标
	double	m_y;			//y偏心坐标
	double	m_r;			//半径
	double	m_l;			//偏心距
	double	m_angle;		//偏心角
		
	std::vector<double> oData;
	CNiReal64Vector NiData;

	int m_nNum;				//记录数
	double m_fCenterX;		//圆心x坐标
	double m_fCenterY;		//圆心y坐标
	double m_fRadius;		//半径R
	CPoint m_points[MAXPOINTS];
// 对话框数据
	enum { IDD = IDD_AirGap };
	void LeastSquaresFitting();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButton2();
public:
	CButton m_save;
public:
	CButton m_comp;
public:
	virtual BOOL OnInitDialog();
};
