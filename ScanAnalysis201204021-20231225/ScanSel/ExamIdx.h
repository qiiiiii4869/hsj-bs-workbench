// ExamIdx.h: interface for the CExamIdx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXAMIDX_H__43AD0DA9_631C_49A4_BBDD_B2C01A89D2B5__INCLUDED_)
#define AFX_EXAMIDX_H__43AD0DA9_631C_49A4_BBDD_B2C01A89D2B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/************************************************************************/
/* 试验索引分为4级：                                                    */
/*1）电厂名试验名；                                                     */
/*2）试验类别（转速、升压、降压、升负荷、降负荷）；                     */
/*3）试验参数 （5％，10％，15％等）；                                   */
/*4）试验通道序列；                                                     */
/************************************************************************/

#include "common.h"
class CExamIdx : public CObject
{
	DECLARE_SERIAL(CExamIdx)
public:
	/************************************************************************/
	/* 4层试验数据结构；                                                    */
	/************************************************************************/
//试验数量；
	unsigned short m_ExamNum;

	//设备数量；
	unsigned short m_DevNum;

	int m_PlantNum;

	//试验状态；
	unsigned char m_ExamState[MAX_EXAM_NUM];

	//试验类型状态；
	unsigned char m_ExamTypeState[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM];

	//试验参数状态；
	unsigned char m_ExamTypeParaState[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//试验参数通道状态；
	unsigned char m_ExamTypeParaChnState[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_EXAM_CHN_NUM];

	//数采设备状态；
	unsigned char m_DevState[MAX_DEV_NUM];

	//20.数采设备通道数量；
	unsigned short  m_DevChnNum[MAX_DEV_NUM];

	//数采设备通道状态；
	unsigned char m_DevChnState[MAX_DEV_NUM][MAX_DEV_CHN_NUM];

	//试验描述名；
	char m_ExamName[MAX_EXAM_NUM][FILE_NAME_LEN];

	//试验类别数量；
	unsigned short m_ExamTypeNum[MAX_EXAM_NUM];

	//试验类型标记；（转速、升压、降压、升负荷、降负荷）；
	//unsigned short m_ExamType[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM];
	char m_ExamType[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][MAX_CHN_NAME_LEN];

	//试验类型含参数数量；
	unsigned short m_ExamTypeParaNum[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM];

	//试验参数标记；
	//unsigned short m_ExamTypePara[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];
	char m_ExamTypePara[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_CHN_NAME_LEN];

	//试验参数通道ID数量;
	unsigned short m_ExamChnNumArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//试验参数通道ID;
	unsigned short m_ExamChnArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_EXAM_CHN_NUM];

	//试验记录时间(S)；
	int m_ExamTimeArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//采样频率；（一个参数试验只有一个）
	int m_ExamFreqArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//处理周期；（一个参数试验只有一个）
	int m_ExamDealSpanArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//试验电厂序号；
	int m_ExamPlantIdx[MAX_EXAM_NUM];

	//试验机组序号；
	int m_ExamSetIdx[MAX_EXAM_NUM];

	//试验机组类型；
	unsigned char m_ExamSetType[MAX_EXAM_NUM];

	//试验机组额定转速；
	int m_ExamSetRpm[MAX_EXAM_NUM];

	//试验机组额定功率；
	float m_ExamSetPower[MAX_EXAM_NUM];

	//试验机组额定电压；
	float m_ExamSetVoltage[MAX_EXAM_NUM];

	//数采设备序号；
	unsigned short m_ExamDevIdxArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_EXAM_CHN_NUM];

	//数采设备通道号；
	unsigned short m_ExamDevChnIdxArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_EXAM_CHN_NUM];

	//由设备索引通道ID；
	unsigned short m_ExamDevChnArr[MAX_DEV_NUM][MAX_DEV_CHN_NUM];

	//通道ID的设备idx；
	unsigned short m_ChnDevIdx[MAX_CHANNEL_NUM];

	//通道ID的设备通道idx；
	unsigned short m_ChnDevChnIdx[MAX_CHANNEL_NUM];

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	//通道数量；
	unsigned short m_ChnNum;

	//1.通道名；V_Name
	char m_ChnName[MAX_CHANNEL_NUM][CHN_NAME_LEN];

	//2.通道ID;V_Address
	unsigned short m_ChnID[MAX_CHANNEL_NUM];

	//3.通道键相ID；V_KeyID*****20090515******对慢变量而言就是映射到慢变量ID的通道；
	unsigned short m_ChnKeyID[MAX_CHANNEL_NUM];

	//4.安装位置；V_BearName
	char m_ChnSetPlace[MAX_CHANNEL_NUM][CHN_NAME_LEN];

	//5.通道类型；V_MName
	char m_ChnType[MAX_CHANNEL_NUM][UNIT_STR_LEN];

	//6.监测方向；V_LName
	char m_ChnScanDiret[MAX_CHANNEL_NUM][UNIT_STR_LEN];

	//7.通道有效性；V_Valid
	unsigned char m_ChnValid[MAX_CHANNEL_NUM];

	//8.通道传感器类型；V_ChType
	unsigned char m_ChnSensorType[MAX_CHANNEL_NUM];

	//9.通道单位描述；V_Unit
	char m_ChnUnit[MAX_CHANNEL_NUM][UNIT_STR_LEN];

	//10.V_Model;
	unsigned char m_ChnModel[MAX_CHANNEL_NUM];

	//11.V_Maker;
	char m_ChnMaker[MAX_CHANNEL_NUM][UNIT_STR_LEN];

	//12.V_IntegralWave;
	unsigned char m_ChnIntegralWave[MAX_CHANNEL_NUM];

	//13.V_FilterWave;
	unsigned char m_ChnFilterWave[MAX_CHANNEL_NUM];

	//14.V_SensitivePara;
	float m_ChnSensitivePara[MAX_CHANNEL_NUM];

	//15.V_ACCor;
	float m_ChnACCor[MAX_CHANNEL_NUM];

	//16.V_DCCor;
	float m_ChnDCCor[MAX_CHANNEL_NUM];

	//17.V_IntegralCor;
	float m_ChnIntegralCor[MAX_CHANNEL_NUM];

	//18.V_ZeroOffset;
	float m_ChnZeroOffset[MAX_CHANNEL_NUM];

	//19.V_FreqHi1;
	float m_ChnFreqHi1[MAX_CHANNEL_NUM];

	//20.V_FreqLow1;
	float m_ChnFreqLow1[MAX_CHANNEL_NUM];

	//21.V_FreqHi2;
	float m_ChnFreqHi2[MAX_CHANNEL_NUM];

	//22.V_FreqLow2;
	float m_ChnFreqLow2[MAX_CHANNEL_NUM];

	//23.V_HiVol;
	float m_ChnHiVol[MAX_CHANNEL_NUM];

	//24.V_LowVol;
	float m_ChnLowVol[MAX_CHANNEL_NUM];

	//25.V_RefGapVol;
	float m_ChnRefGapVol[MAX_CHANNEL_NUM];

	//26.V_AlmHH;
	float m_ChnAlmHH[MAX_CHANNEL_NUM];

	//27.V_AlmH;
	float m_ChnAlmH[MAX_CHANNEL_NUM];

	//28.V_RefAllFreq;
	float m_ChnRefAllFreq[MAX_CHANNEL_NUM];

	//29.V_AllFreqDev;
	float m_ChnAllFreqDev[MAX_CHANNEL_NUM];

	//30.V_OneFreqX;
	float m_ChnOneFreqX[MAX_CHANNEL_NUM];

	//31.V_OneFreqY;
	float m_ChnOneFreqY[MAX_CHANNEL_NUM];

	//31.V_OneFreqDev;
	float m_ChnOneFreqDev[MAX_CHANNEL_NUM];

	//32.V_TwoFreqX;
	float m_ChnTwoFreqX[MAX_CHANNEL_NUM];

	//33.V_TwoFreqY;
	float m_ChnTwoFreqY[MAX_CHANNEL_NUM];

	//34.V_TwoFreqDev;
	float m_ChnTwoFreqDev[MAX_CHANNEL_NUM];

	//35.V_HalfFreqDev;
	float m_ChnHalfFreqDev[MAX_CHANNEL_NUM];

	//36.V_OptFreqDev;
	float m_ChnOptFreqDev[MAX_CHANNEL_NUM];

	//37.V_RemainderDev;
	float m_ChnRemainderDev[MAX_CHANNEL_NUM];

	//38.V_SensitiveType;
	float m_ChnSensitiveType[MAX_CHANNEL_NUM];

	//39.V_FFTMinVal;
	float m_ChnFFTMinVal[MAX_CHANNEL_NUM];

	//40.V_FFTMinNum;
	unsigned short m_ChnFFTMinNum[MAX_CHANNEL_NUM];

	//41.V_VibCalType;
	unsigned char m_ChnVibCalType[MAX_CHANNEL_NUM];

	//42.V_AlmPowerUp1;
	float m_ChnAlmPowerUp1[MAX_CHANNEL_NUM];

	//43.V_AlmPowerDown1;
	float m_ChnAlmPowerDown1[MAX_CHANNEL_NUM];

	//44.V_AlmPowerUp2;
	float m_ChnAlmPowerUp2[MAX_CHANNEL_NUM];

	//45.V_AlmPowerDown2;
	float m_ChnAlmPowerDown2[MAX_CHANNEL_NUM];

	int m_PlantID[MAX_DEV_NUM];

	int m_SetNum[MAX_DEV_NUM];

	int m_SetID[MAX_DEV_NUM][MAX_DEV_NUM];

	//3.SJ_PlantID
	char m_DevPlantID[MAX_DEV_NUM][UNIT_STR_LEN];

	//4.SJ_SetID
	char m_DevSetID[MAX_DEV_NUM][UNIT_STR_LEN];
	
	//设备信息；
	//5.SJ_IP1;
	char m_DevIP1[MAX_DEV_NUM][UNIT_STR_LEN];

	//6.SJ_IP2;
	char m_DevIP2[MAX_DEV_NUM][UNIT_STR_LEN];

	//15.采样模式；
	unsigned char m_DevSamMode[MAX_DEV_NUM];

	//16.采样频率；
	unsigned short m_DevSamFreq[MAX_DEV_NUM];

	//18.起始通道；
	unsigned short m_DevStartID[MAX_DEV_NUM];

	//19.终止通道；
	unsigned short m_DevEndID[MAX_DEV_NUM];

	//通道数量；在上面定义；


	////////////////////////////////////////////////////////////////////////////

	//键相通道数量；
	unsigned short m_ExamKeyNumArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//键相通道ID；
	unsigned short m_ExamKeyIDArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_PARA_KEY_NUM];

	//参考通道数量；
	unsigned short m_ExamRefNumArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//参考通道ID；
	unsigned short m_ExamRefIDArr[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_PARA_REF_NUM];

	//试验开始时间；
	unsigned long m_ExamStartTime[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//试验终止时间；
	unsigned long m_ExamEndTime[MAX_EXAM_NUM][MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//通道中文描述桶；以设备为索引；
	char m_DevChnDesc[MAX_DEV_NUM][MAX_DEV_CHN_NUM][CHN_NAME_LEN];

	//通道单位；以设备为索引；
	char m_DevChnUnit[MAX_DEV_NUM][MAX_DEV_CHN_NUM][UNIT_STR_LEN];

	//设备通道类型；振动、摆度
	unsigned char m_DevChnType[MAX_DEV_NUM][MAX_DEV_CHN_NUM];

	//通道所属机组序号，以设备为索引；
	int m_DevChnSetIdx[MAX_DEV_NUM][MAX_DEV_CHN_NUM];

	//通道所属电厂序号，以设备为索引；
	int m_DevChnPlantIdx[MAX_DEV_NUM][MAX_DEV_CHN_NUM];

	//设备描述；
	char m_DevDesc[MAX_DEV_NUM][FILE_NAME_LEN];

public:
	int SetChnInfo(int col_idx,int row_idx,char * val_str);

	int GetChnInfo(int col_idx, int row_idx, char * val_str);

	int SetDevInfo(int col_idx,int row_idx,char * val_str);

	int GetDevInfo(int col_idx,int row_idx,char * val_str);

	int GetDevTitle(int col_idx,char * val_str);

	int GetChnTitle(int col_idx,char * val_str);

	int GetChnIdxByID(unsigned short chn_id);

	int GetChnIdxByDesc(char * chn_desc);

	int GetChnIdxBySetID(char * set_id,int * chn_idx,int * chn_num);

	int GetDevIdxBySetID(char * set_id);

	int GetSetIDByChnID(unsigned short chn_id,char * set_id);

	int GetSamFreqByChnID(unsigned short chn_id,int * sam_freq);


	/************************************************************************/
	/*                          加入试验名                                  */
	/*          成功返回试验序号0～MAX_EXAM_NUM-1;                          */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int AddExamName(char * exam_name);

	/************************************************************************/
	/*                          加入试验类型                                */
	/*          成功返回试验中的类型序号0～MAX_EXAM_TYPE_NUM-1;             */
	/*          失败返回－1;                                                */
	/************************************************************************/
	//int AddExamType(int exam_idx,unsigned short exam_type);
	int AddExamType(int exam_idx,char * exam_type);

	/************************************************************************/
	/*                          加入试验参数                                */
	/*          成功返回试验中的类型参数序号0～EXAM_PARA_NUM-1;             */
	/*          失败返回－1;                                                */
	/************************************************************************/
	//int AddExamTypePara(int exam_idx,int exam_type_idx,unsigned short exam_para);
	int AddExamTypePara(int exam_idx,int exam_type_idx,char * exam_para);

	/************************************************************************/
	/*                          加入试验通道                                */
	/*          成功返回试验中的类型参数序号0～MAX_EXAM_CHN_NUM-1;          */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int AddExamTypeParaChn(int exam_idx,int exam_type_idx,int exam_para_idx,int dev_idx,int dev_chn_idx);

	int AddExamTypeParaChn(int exam_idx,int exam_type_idx,int exam_para_idx,int chn_id);

	int AddExamTypeKeyChn(int exam_idx,int exam_type_idx,int exam_para_idx,int chn_id);

	/************************************************************************/
	/*                          加入设备                                */
	/*          成功返回试验中的类型参数序号0～MAX_DEV_NUM-1;          */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int AddDev(char * dev_desc);

	int LoadDevChn(int dev_idx
			      ,int dev_chn_num
			      ,unsigned short * chn_id
			      ,char ** chn_desc
			      ,char ** chn_unit
			      ,unsigned char * chn_state
				  ,unsigned char * chn_type 
			      ,int * plant_idx
			      ,int * set_idx);

	int SetDevState(int dev_idx,unsigned char dev_state);

	int SetDevChnState(int dev_idx,int dev_chn_idx,unsigned char dev_state);

	/************************************************************************/
	/*                          设置试验数据采集时间                        */
	/*          成功返回标识1;                                              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamTypeParaTimeSpan(int exam_idx,int exam_type_idx,int exam_para_idx,int exam_time_span);

	/************************************************************************/
	/*                          设置试验数据采样频率                        */
	/*          成功返回标识1;                                              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamTypeParaSamFreq(int exam_idx,int exam_type_idx,int exam_para_idx,int sam_freq);

	/************************************************************************/
	/*                          设置试验数据处理周期                        */
	/*          成功返回标识1;                                              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamTypeParaDealSpan(int exam_idx,int exam_type_idx,int exam_para_idx,int deal_span);

	/************************************************************************/
	/*                          设置试验电厂序号                            */
	/*          成功返回标识1;                                              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamPlantIdx(int exam_idx,int plant_idx);

	/************************************************************************/
	/*                          设置试验机组序号                                                                             */
	/*          成功返回标识1;                                              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamSetIdx(int exam_idx,int set_idx);

	/************************************************************************/
	/*                          设置试验机组类型                                                                            */
	/*          成功返回标识1;                                              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamSetType(int exam_idx,unsigned char set_type);

	/************************************************************************/
	/*                          设置试验机组额定转速                                                                  */
	/*          成功返回标识1;                                              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamSetRpm(int exam_idx,int set_rpm);

	/************************************************************************/
	/*                          设置试验机组额定功率                                                                 */
	/*          成功返回1;              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamSetPower(int exam_idx,float set_power);

	/************************************************************************/
	/*                          设置试验机组额定电压                                                                 */
	/*          成功返回1;              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamSetVoltage(int exam_idx,float set_voltage);

	/************************************************************************/
	/*                          设置数采设备序号 和通道号                                                       */
	/*          成功返回1;              */
	/*          失败返回－1;                                                */
	/************************************************************************/
	int SetExamDevIdxChn(int exam_idx,int type_idx,int para_idx,int chn_idx,int dev_idx,int dev_chn_idx);

	/************************************************************************/
	/*                          设置由设备索引通道ID                          */
	/*          成功返回1;                                                   */
	/*          失败返回－1;                                                 */
	/************************************************************************/
	int SetExamDevChnArr(int dev_idx,int dev_chn_idx,unsigned short chn_id);

	//通道ID的设备idx；
	int SetChnDevIdx(int chn_id,int dev_idx,int dev_chn_idx);

	//键相通道数量,键相通道ID；
	int SetExamKeyInfo(int exam_idx,int type_idx,int para_idx,int key_num,unsigned short * key_id_arr);

	//参考通道数量,参考通道ID；
	int SetRefChnInfo(int exam_idx,int type_idx,int para_idx,int ref_num,unsigned short * ref_id_arr);

	//试验开始时间；
	int SetExamStartTime(int exam_idx,int type_idx,int para_idx,unsigned long start_time);

	//试验终止时间；
	int SetExamEndTime(int exam_idx,int type_idx,int para_idx,unsigned long end_time);

	//通道中文描述桶；以设备为索引；
	int SetDevChnDesc(int dev_idx,int dev_chn_idx,char * chn_desc);

	//通道单位；以设备为索引；
	int SetDevChnUnit(int dev_idx,int dev_chn_idx,char * unit_desc);

	//通道所属机组序号，以设备为索引；
	int SetDevChnSetIdx(int dev_idx,int dev_chn_idx,int set_idx);

	//通道所属电厂序号，以设备为索引；
	int SetDevChnPlantIdx(int dev_idx,int dev_chn_idx,int plant_idx);

	//通道类型：振动、摆度、串动、键相、压力脉动、功率、电流、电压、励磁、气隙；

	int LoadDevCfg(char * file_path);

	int LoadChnCfg(char * file_path);

	CExamIdx();
	virtual ~CExamIdx();
	int Serialize(int type,CString strPath);
	BOOL FolderExist(CString strPath);
};

#endif // !defined(AFX_EXAMIDX_H__43AD0DA9_631C_49A4_BBDD_B2C01A89D2B5__INCLUDED_)
