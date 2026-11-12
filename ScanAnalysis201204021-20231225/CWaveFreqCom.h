#pragma once
#include <vector>

struct Quaternion	//坐标四元数
{
	double xMin;	//x最小数
	double xMax;	//x最大值
	double yMin;	//y最小值
	double yMax;	//y最大值
};

class FFTParam			//FFT特征值
{
public:
	double frequency;	//频率
	double amplitude;	//幅值
	double phase;		//相位
	bool operator < (const FFTParam &m)const //排序
	{
		return amplitude < m.amplitude;
	}
};

class CWaveParam		//波性特征
{
public:
	int startPoint;		//起点								
	int endPoint;		//时间
	int simpling;		//频率
	double timeStart;	//起始时间
	double timeEnd;		//终止时间
	double aMax;		//最大值
	double aMin;		//最小值
	double mean;		//平均值
	double median;		//中间值
	double RMS;			//有效值
	double ppValue;		//峰峰值
};

class ChnExcelData		//excel存储通道数据特征
{
public:
	int ID;				//ID序列标号
	CString chnName;	//通道名
	double aMin;		//最小值
	double aMax;		//最大值
	double mean;		//中间值
	double median;		//中间值
	double RMS;			//有效值
	double ppValue;		//峰峰值
	std::vector<FFTParam> fftData;	////FFT特征值
};

