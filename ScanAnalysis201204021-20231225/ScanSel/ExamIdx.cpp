// ExamIdx.cpp: implementation of the CExamIdx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExamIdx.h"
#include "StrTool.h"
#include "SysConf.h"
//#include "CSpreadSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CExamIdx, CObject, 1)
CExamIdx::CExamIdx()
{
	// TODO Auto-generated constructor stub
	//试验数量；
	m_ExamNum = 0;

	//设备数量；
	m_DevNum = 0;

	//通道数量；
    m_ChnNum = 0;

	m_PlantNum = 0;

	memset(m_SetNum,0,MAX_DEV_NUM*sizeof(int));

	//试验状态；
	memset(m_ExamState,0,MAX_EXAM_NUM);

	//试验类型状态；
	memset(m_ExamTypeState,0,MAX_EXAM_NUM*MAX_EXAM_TYPE_NUM);

	//试验参数状态；
	memset(m_ExamTypeParaState,0,MAX_EXAM_NUM*MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM);

	//试验参数通道状态；
	memset(m_ExamTypeParaChnState,0,MAX_EXAM_NUM*MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM*MAX_EXAM_CHN_NUM);

	//数采设备状态；
	memset(m_DevState,0,MAX_DEV_NUM);

	//数采设备通道数量；
	memset(m_DevChnNum,0,MAX_DEV_NUM*sizeof(short));

	//数采设备通道状态；
	memset(m_DevChnState,0,MAX_DEV_NUM*MAX_DEV_CHN_NUM);

	//试验描述名；
	memset(m_ExamName,0,MAX_EXAM_NUM*FILE_NAME_LEN);

	//试验类别数量；
	memset(m_ExamTypeNum,0,MAX_EXAM_NUM*sizeof(short));

	//试验类型标记；（转速、升压、降压、升负荷、降负荷）；
	//memset(m_ExamType,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * sizeof(short));
	memset(m_ExamType,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * MAX_CHN_NAME_LEN * sizeof(char));

	//试验类型含参数数量；
	memset(m_ExamTypeParaNum,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * sizeof(short));

	//试验参数标记；
	//memset(m_ExamTypePara,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(short));
	memset(m_ExamTypePara,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * MAX_CHN_NAME_LEN * sizeof(char));

	//试验参数通道ID数量;
	memset(m_ExamChnNumArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(short));

	//试验参数通道ID;
	memset(m_ExamChnArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * MAX_EXAM_CHN_NUM * sizeof(short));

	//试验记录时间(S)；
	memset(m_ExamTimeArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(int));

	//采样频率；（一个参数试验只有一个）
	memset(m_ExamFreqArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(int));

	//处理周期；（一个参数试验只有一个）
	memset(m_ExamDealSpanArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(int));

	//试验电厂序号；
	memset(m_ExamPlantIdx,0,MAX_EXAM_NUM*sizeof(int));

	//试验机组序号；
	memset(m_ExamSetIdx,0,MAX_EXAM_NUM*sizeof(int));

	//试验机组类型；
	memset(m_ExamSetType,0,MAX_EXAM_NUM*sizeof(char));

	//试验机组额定转速；
	memset(m_ExamSetRpm,0,MAX_EXAM_NUM*sizeof(int));

	//试验机组额定功率；
	memset(m_ExamSetPower,0,MAX_EXAM_NUM*sizeof(float));

	//试验机组额定电压；
	memset(m_ExamSetVoltage,0,MAX_EXAM_NUM*sizeof(float));

	//数采设备序号；
	memset(m_ExamDevIdxArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * MAX_EXAM_CHN_NUM * sizeof(short));

	//数采设备通道号；
	memset(m_ExamDevChnIdxArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * MAX_EXAM_CHN_NUM * sizeof(short));

	//由设备索引通道ID；
	memset(m_ExamDevChnArr,0,MAX_DEV_NUM * MAX_DEV_CHN_NUM * sizeof(short));

	//通道ID的设备idx；
	memset(m_ChnDevIdx,0,MAX_CHANNEL_NUM * sizeof(short));

	//通道ID的设备通道idx；
	memset(m_ChnDevChnIdx,0,MAX_CHANNEL_NUM * sizeof(short));

	//键相通道数量；
	memset(m_ExamKeyNumArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(short));

	//键相通道ID；
	memset(m_ExamKeyIDArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * MAX_PARA_KEY_NUM * sizeof(short));

	//参考通道数量；
	memset(m_ExamRefNumArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(short));

	//参考通道ID；
	memset(m_ExamRefIDArr,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * MAX_PARA_REF_NUM * sizeof(short));

	//试验开始时间；
	memset(m_ExamStartTime,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(long));

	//试验终止时间；
	memset(m_ExamEndTime,0,MAX_EXAM_NUM * MAX_EXAM_TYPE_NUM * EXAM_PARA_NUM * sizeof(long));

	//通道中文描述桶；以设备为索引；
	memset(m_DevChnDesc,0,MAX_DEV_NUM*MAX_DEV_CHN_NUM*CHN_NAME_LEN);

	//通道单位；以设备为索引；
	memset(m_DevChnUnit,0,MAX_DEV_NUM*MAX_DEV_CHN_NUM*UNIT_STR_LEN);

	//通道所属机组序号，以设备为索引；
	memset(m_DevChnSetIdx,0,MAX_DEV_NUM*MAX_DEV_CHN_NUM*sizeof(int));

	//通道所属电厂序号，以设备为索引；
	memset(m_DevChnPlantIdx,0,MAX_DEV_NUM*MAX_DEV_CHN_NUM*sizeof(int));

	memset(m_PlantID,0,MAX_DEV_NUM*sizeof(int));

	memset(m_SetID,0,MAX_DEV_NUM*MAX_DEV_NUM*sizeof(int));

	//设备描述；
	memset(m_DevDesc,0,MAX_DEV_NUM * FILE_NAME_LEN);

	//1.通道名；V_Name
	memset(m_ChnName,0,MAX_CHANNEL_NUM * CHN_NAME_LEN);

	//2.通道ID;V_Address
	memset(m_ChnID,0,MAX_CHANNEL_NUM * sizeof(short));

	//3.通道键相ID；V_KeyID
	memset(m_ChnKeyID,0,MAX_CHANNEL_NUM * sizeof(short));

	//4.安装位置；V_BearName
	memset(m_ChnSetPlace,0,MAX_CHANNEL_NUM*CHN_NAME_LEN);

	//5.通道类型；V_MName
	memset(m_ChnType,0,MAX_CHANNEL_NUM*UNIT_STR_LEN);

	//6.监测方向；V_LName
    memset(m_ChnScanDiret,0,MAX_CHANNEL_NUM*UNIT_STR_LEN);

	//7.通道有效性；V_Valid
	memset(m_ChnValid,0,MAX_CHANNEL_NUM);

	//8.通道传感器类型；V_ChType
	memset(m_ChnSensorType,0,MAX_CHANNEL_NUM);

	//9.通道单位描述；V_Unit
	memset(m_ChnUnit,0,MAX_CHANNEL_NUM*UNIT_STR_LEN);

	//10.V_Model;
	memset(m_ChnModel,0,MAX_CHANNEL_NUM);

	//11.V_Maker;
    memset(m_ChnMaker,0,MAX_CHANNEL_NUM*UNIT_STR_LEN);

	//12.V_IntegralWave;
    memset(m_ChnIntegralWave,0,MAX_CHANNEL_NUM);

	//13.V_FilterWave;
	memset(m_ChnFilterWave,0,MAX_CHANNEL_NUM);

	//14.V_SensitivePara;
	memset(m_ChnSensitivePara,0,MAX_CHANNEL_NUM*sizeof(float));

	//15.V_ACCor;
	memset(m_ChnACCor,0,MAX_CHANNEL_NUM*sizeof(float));

	//16.V_DCCor;
	memset(m_ChnDCCor,0,MAX_CHANNEL_NUM*sizeof(float));

	//17.V_IntegralCor;
	memset(m_ChnIntegralCor,0,MAX_CHANNEL_NUM*sizeof(float));

	//18.V_ZeroOffset;
	memset(m_ChnZeroOffset,0,MAX_CHANNEL_NUM*sizeof(float));

	//19.V_FreqHi1;
	memset(m_ChnFreqHi1,0,MAX_CHANNEL_NUM*sizeof(float));

	//20.V_FreqLow1;
	memset(m_ChnFreqLow1,0,MAX_CHANNEL_NUM*sizeof(float));

	//21.V_FreqHi2;
	memset(m_ChnFreqHi2,0,MAX_CHANNEL_NUM*sizeof(float));
	
	//22.V_FreqLow2;
	memset(m_ChnFreqLow2,0,MAX_CHANNEL_NUM*sizeof(float));

	//22.V_FreqLow2;
	memset(m_ChnFreqLow2,0,MAX_CHANNEL_NUM*sizeof(float));

	//24.V_LowVol;
	memset(m_ChnLowVol,0,MAX_CHANNEL_NUM*sizeof(float));

	//25.V_RefGapVol;
	memset(m_ChnRefGapVol,0,MAX_CHANNEL_NUM*sizeof(float));

	//26.V_AlmHH;
	memset(m_ChnAlmHH,0,MAX_CHANNEL_NUM*sizeof(float));

	//27.V_AlmH;
	memset(m_ChnAlmH,0,MAX_CHANNEL_NUM*sizeof(float));

	//28.V_RefAllFreq;
	memset(m_ChnRefAllFreq,0,MAX_CHANNEL_NUM*sizeof(float));

	//29.V_AllFreqDev;
	memset(m_ChnAllFreqDev,0,MAX_CHANNEL_NUM*sizeof(float));

	//30.V_OneFreqX;
	memset(m_ChnOneFreqX,0,MAX_CHANNEL_NUM*sizeof(float));

	//31.V_OneFreqY;
	memset(m_ChnOneFreqY,0,MAX_CHANNEL_NUM*sizeof(float));

	//32.V_TwoFreqX;
	memset(m_ChnOneFreqX,0,MAX_CHANNEL_NUM*sizeof(float));

	//33.V_TwoFreqY;
	memset(m_ChnOneFreqY,0,MAX_CHANNEL_NUM*sizeof(float));

	//34.V_TwoFreqDev;
	memset(m_ChnTwoFreqDev,0,MAX_CHANNEL_NUM*sizeof(float));

	//35.V_HalfFreqDev;
	memset(m_ChnHalfFreqDev,0,MAX_CHANNEL_NUM*sizeof(float));

	//36.V_OptFreqDev;
	memset(m_ChnOptFreqDev,0,MAX_CHANNEL_NUM*sizeof(float));

	//37.V_RemainderDev;
	memset(m_ChnRemainderDev,0,MAX_CHANNEL_NUM*sizeof(float));

	//38.V_SensitiveType;
	memset(m_ChnSensitiveType,0,MAX_CHANNEL_NUM*sizeof(float));

	//39.V_FFTMinVal;
	memset(m_ChnFFTMinVal,0,MAX_CHANNEL_NUM*sizeof(float));

	//40.V_FFTMinNum;
	memset(m_ChnFFTMinNum,0,MAX_CHANNEL_NUM*sizeof(short));

	//41.V_VibCalType;
	memset(m_ChnVibCalType,0,MAX_CHANNEL_NUM);

	//42.V_AlmPowerUp1;
	memset(m_ChnAlmPowerUp1,0,MAX_CHANNEL_NUM*sizeof(float));

	//43.V_AlmPowerDown1;
	memset(m_ChnAlmPowerDown1,0,MAX_CHANNEL_NUM*sizeof(float));

	//44.V_AlmPowerUp2;
	memset(m_ChnAlmPowerUp2,0,MAX_CHANNEL_NUM*sizeof(float));

	//45.V_AlmPowerDown2;
	memset(m_ChnAlmPowerDown2,0,MAX_CHANNEL_NUM*sizeof(float));

	memset(m_DevPlantID,0,MAX_DEV_NUM*UNIT_STR_LEN);

	memset(m_DevSetID,0,MAX_DEV_NUM*UNIT_STR_LEN);
	//设备信息；
	//5.SJ_IP1;
	memset(m_DevIP1,0,MAX_DEV_NUM*UNIT_STR_LEN);

	//6.SJ_IP2;
	memset(m_DevIP2,0,MAX_DEV_NUM*UNIT_STR_LEN);

	//15.采样模式；
	memset(m_DevSamMode,0,MAX_DEV_NUM);

	//16.采样频率；
	memset(m_DevSamFreq,0,MAX_DEV_NUM*sizeof(short));

	//18.起始通道；
	memset(m_DevStartID,0,MAX_DEV_NUM*sizeof(short));

	//19.终止通道；
	memset(m_DevEndID,0,MAX_DEV_NUM*sizeof(short));
}

CExamIdx::~CExamIdx()
{

}

//根据chnID取机组描述；
int CExamIdx::GetSetIDByChnID(unsigned short chn_id,char * set_id)
{
	int ret_flag = 1,i=0;
	if(chn_id<0||chn_id>=MAX_CHANNEL_NUM) return -1;
	if(set_id==0) return -1;
	for(i=0;i<m_DevNum;i++)
	{
		if(chn_id>=m_DevStartID[i]&&chn_id<=m_DevEndID[i])
		{
			break;
		}
	}
	CStrTool::CStrcpy(set_id,m_DevSetID[i],FILE_NAME_LEN);
	return ret_flag;
}


/************************************************************************/
/*                          加入试验名                                  */
/*          成功返回试验序号0～MAX_EXAM_NUM-1;                          */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::AddExamName(char * exam_name)
{
	int ret_flag = 1;
	if(exam_name==0) return -1;
	if(strlen(exam_name)>FILE_NAME_LEN) return -2;
	int i=0;
	if(m_ExamNum>=MAX_EXAM_NUM)return -3;
	for(i=0;i<MAX_EXAM_NUM;i++)
	{
		if(m_ExamState[i]==NO_USED)
			break;
		else
		{
			if(!strcmp(exam_name,m_ExamName[i])) return i;
		}
	}
	strcpy(m_ExamName[i],exam_name);
	m_ExamState[i] = IN_USED;
	m_ExamNum++;
	ret_flag = i;
	return ret_flag;
}

int CExamIdx::AddExamType(int exam_idx,char * exam_type)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM||exam_type==0) return -1;
	if(m_ExamTypeNum[exam_idx]>=MAX_EXAM_TYPE_NUM) return -3;
	int i=0;
	for(i=0;i<MAX_EXAM_TYPE_NUM;i++)
	{
		if(m_ExamTypeState[exam_idx][i]==NO_USED)
			break;
		else
		{
			if(!strcmp(m_ExamType[exam_idx][i],exam_type)) return i;
		}
	}
	strcpy(m_ExamType[exam_idx][i],exam_type);
	m_ExamTypeState[exam_idx][i]= IN_USED;
	m_ExamTypeNum[exam_idx]++;
	ret_flag = i;
	return ret_flag;
}

int CExamIdx::AddExamTypePara(int exam_idx,int exam_type_idx,char * exam_para)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(m_ExamTypeParaNum[exam_idx][exam_type_idx]>=EXAM_PARA_NUM) return -3;
	if(exam_para==0) return -1;
	int i=0,j=0;
	for(i=0;i<EXAM_PARA_NUM;i++)
	{
		if(m_ExamTypeParaState[exam_idx][exam_type_idx][i]==NO_USED)
			break;
		else
		{
			if(!strcmp(m_ExamTypePara[exam_idx][exam_type_idx][i],exam_para)) 
			{
				for(j=0;j<m_ExamChnNumArr[exam_idx][exam_type_idx][i];j++)
				{
					m_ExamTypeParaChnState[exam_idx][exam_type_idx][i][j]=NO_USED;
				}
				m_ExamKeyNumArr[exam_idx][exam_type_idx][i]=0;
				m_ExamChnNumArr[exam_idx][exam_type_idx][i]=0;
				return i;
			}
		}
	}
	strcpy(m_ExamTypePara[exam_idx][exam_type_idx][i],exam_para);
	m_ExamTypeParaState[exam_idx][exam_type_idx][i] = IN_USED;
	m_ExamTypeParaNum[exam_idx][exam_type_idx]++;
	ret_flag = i;
	return ret_flag;
}

/************************************************************************/
/*                          加入试验通道                                */
/*          成功返回试验中的类型参数序号0～MAX_EXAM_CHN_NUM-1;          */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::AddExamTypeParaChn(int exam_idx,int exam_type_idx,int exam_para_idx,int dev_idx,int dev_chn_idx)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(exam_para_idx<0||exam_para_idx>=EXAM_PARA_NUM) return -1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if(dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;

	if(m_ExamChnNumArr[exam_idx][exam_type_idx][exam_para_idx]>=MAX_EXAM_CHN_NUM) return -3;
	int i=0;
	for(i=0;i<MAX_EXAM_CHN_NUM;i++)
	{
		if(m_ExamTypeParaChnState[exam_idx][exam_type_idx][exam_para_idx][i]==NO_USED)
			break;
	}
	m_ExamChnArr[exam_idx][exam_type_idx][exam_para_idx][i] = m_ExamDevChnArr[dev_idx][dev_chn_idx];
	m_ExamDevIdxArr[exam_idx][exam_type_idx][exam_para_idx][i] = dev_idx;
	m_ExamDevChnIdxArr[exam_idx][exam_type_idx][exam_para_idx][i] = dev_chn_idx;
	m_ExamTypeParaChnState[exam_idx][exam_type_idx][exam_para_idx][i] = IN_USED;
	m_ExamChnNumArr[exam_idx][exam_type_idx][exam_para_idx]++;
	ret_flag = i;
	return ret_flag;
}

/************************************************************************/
/*                          加入试验通道                                */
/*          成功返回试验中的类型参数序号0～MAX_EXAM_CHN_NUM-1;          */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::AddExamTypeParaChn(int exam_idx,int exam_type_idx,int exam_para_idx,int chn_id)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(exam_para_idx<0||exam_para_idx>=EXAM_PARA_NUM) return -1;
	if(chn_id<0||chn_id>=MAX_CHANNEL_NUM) return -1;

	if(m_ExamChnNumArr[exam_idx][exam_type_idx][exam_para_idx]>=MAX_EXAM_CHN_NUM) return -3;
	int i=0;
	for(i=0;i<MAX_EXAM_CHN_NUM;i++)
	{
		if(m_ExamTypeParaChnState[exam_idx][exam_type_idx][exam_para_idx][i]==NO_USED)
			break;
	}
	m_ExamChnArr[exam_idx][exam_type_idx][exam_para_idx][i] = chn_id;
	m_ExamTypeParaChnState[exam_idx][exam_type_idx][exam_para_idx][i] = IN_USED;
	m_ExamChnNumArr[exam_idx][exam_type_idx][exam_para_idx]++;
	ret_flag = i;
	return ret_flag;
}


/************************************************************************/
/*                          加入键相通道                                */
/*          成功返回试验中的类型参数序号0～MAX_EXAM_CHN_NUM-1;          */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::AddExamTypeKeyChn(int exam_idx,int exam_type_idx,int exam_para_idx,int chn_id)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(exam_para_idx<0||exam_para_idx>=EXAM_PARA_NUM) return -1;
	if(chn_id<0||chn_id>=MAX_CHANNEL_NUM) return -1;

	if(m_ExamKeyNumArr[exam_idx][exam_type_idx][exam_para_idx]>=MAX_PARA_KEY_NUM) return -3;
	int i=m_ExamKeyNumArr[exam_idx][exam_type_idx][exam_para_idx];
	m_ExamKeyIDArr[exam_idx][exam_type_idx][exam_para_idx][i] = chn_id;
	m_ExamKeyNumArr[exam_idx][exam_type_idx][exam_para_idx]++;
	ret_flag = i;
	return ret_flag;
}

/************************************************************************/
/*                          加入设备                                */
/*          成功返回试验中的类型参数序号0～MAX_DEV_NUM-1;          */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::AddDev(char * dev_desc)
{
	int ret_flag = 1;
	if(dev_desc==0) return -1;
	if(strlen(dev_desc)>FILE_NAME_LEN) return -2;
	int i=0;
	if(m_DevNum>=MAX_DEV_NUM)return -3;
	for(i=0;i<MAX_DEV_NUM;i++)
	{
		if(m_DevState[i]==NO_USED)
			break;
		else
		{
			if(!strcmp(dev_desc,m_DevDesc[i])) return -4;
		}
	}
	strcpy(m_DevDesc[i],dev_desc);
	m_DevState[i] = IN_USED;
	m_DevNum++;
	ret_flag = i;
	return ret_flag;
}

int CExamIdx::LoadDevChn(int dev_idx
		      ,int dev_chn_num
		      ,unsigned short * chn_id
		      ,char ** chn_desc
		      ,char ** chn_unit
		      ,unsigned char * chn_state
			  ,unsigned char * chn_type 
		      ,int * plant_idx
		      ,int * set_idx)
{
	int ret_flag = 1,i=0,str_len = 0;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if (dev_chn_num<0||dev_chn_num>=MAX_DEV_CHN_NUM) return -1;
	if(chn_id==0) return -1;
	if(chn_desc==0) return -1;
	if(chn_unit==0) return -1;
	if(chn_state==0) return -1;
	if(chn_type==0) return -1;
	if(plant_idx==0||set_idx==0) return -1;

	m_DevChnNum[dev_idx] = dev_chn_num;
	for(i=0;i<dev_chn_num;i++)
	{
		m_ExamDevChnArr[dev_idx][i] = chn_id[i];
		CStrTool::CStrcpy(m_DevChnDesc[dev_idx][i],chn_desc[i],CHN_NAME_LEN-1);
		CStrTool::CStrcpy(m_DevChnUnit[dev_idx][i],chn_unit[i],UNIT_STR_LEN-1);
		m_DevChnState[dev_idx][i] = chn_state[i];
		m_DevChnType[dev_idx][i] = chn_type[i];
		m_DevChnPlantIdx[dev_idx][i] = plant_idx[i];
		m_DevChnSetIdx[dev_idx][i] = set_idx[i];
	}
	SetDevState(dev_idx,IN_USED);
	return ret_flag;
}

int CExamIdx::SetDevState(int dev_idx,unsigned char dev_state)
{
	int ret_flag = 1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	m_DevState[dev_idx] = dev_state;
	return ret_flag;
}

int CExamIdx::SetDevChnState(int dev_idx,int dev_chn_idx,unsigned char dev_state)
{
	int ret_flag = 1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if (dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;
	m_DevChnState[dev_idx][dev_chn_idx] = dev_state;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验数据采集时间                        */
/*          成功返回标识1;                                              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamTypeParaTimeSpan(int exam_idx,int exam_type_idx,int exam_para_idx,int exam_time_span)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(exam_para_idx<0||exam_para_idx>=EXAM_PARA_NUM) return -1;
	if(exam_time_span<0||exam_time_span>=MAX_TIME_SPAN) return -1;
	m_ExamTimeArr[exam_idx][exam_type_idx][exam_para_idx] = exam_time_span;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验数据采样频率                        */
/*          成功返回标识1;                                              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamTypeParaSamFreq(int exam_idx,int exam_type_idx,int exam_para_idx,int sam_freq)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(exam_para_idx<0||exam_para_idx>=EXAM_PARA_NUM) return -1;
	if(sam_freq<=0||sam_freq>=1024*100) return -1;
	m_ExamFreqArr[exam_idx][exam_type_idx][exam_para_idx] = sam_freq;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验数据处理周期                        */
/*          成功返回标识1;                                              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamTypeParaDealSpan(int exam_idx,int exam_type_idx,int exam_para_idx,int deal_span)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(exam_para_idx<0||exam_para_idx>=EXAM_PARA_NUM) return -1;
	if(deal_span<=0||deal_span>=10) return -1;
	m_ExamDealSpanArr[exam_idx][exam_type_idx][exam_para_idx] = deal_span;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验电厂序号                            */
/*          成功返回标识1;                                              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamPlantIdx(int exam_idx,int plant_idx)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(plant_idx<0) return -1;
	m_ExamPlantIdx[exam_idx] = plant_idx;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验机组序号                                                                             */
/*          成功返回标识1;                                              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamSetIdx(int exam_idx,int set_idx)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(set_idx<0) return -1;
	m_ExamSetIdx[exam_idx] = set_idx;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验机组类型                                                                            */
/*          成功返回标识1;                                              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamSetType(int exam_idx,unsigned char set_type)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	m_ExamSetType[exam_idx] = set_type;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验机组额定转速                                                                  */
/*          成功返回标识1;                                              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamSetRpm(int exam_idx,int set_rpm)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(set_rpm<0||exam_idx>=3000) return -1;
	m_ExamSetRpm[exam_idx] = set_rpm;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验机组额定功率                                                                 */
/*          成功返回1;              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamSetPower(int exam_idx,float set_power)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(set_power<=0) return -1;
	m_ExamSetPower[exam_idx] = set_power;
	return ret_flag;
}

/************************************************************************/
/*                          设置试验机组额定电压                                                                 */
/*          成功返回1;              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamSetVoltage(int exam_idx,float set_voltage)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(set_voltage<=0) return -1;
	m_ExamSetVoltage[exam_idx] = set_voltage;
	return ret_flag;
}

/************************************************************************/
/*                          设置数采设备序号 和通道号                                                       */
/*          成功返回1;              */
/*          失败返回－1;                                                */
/************************************************************************/
int CExamIdx::SetExamDevIdxChn(int exam_idx,int type_idx,int para_idx,int chn_idx,int dev_idx,int dev_chn_idx)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(type_idx<0||type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(para_idx<0||para_idx>=EXAM_PARA_NUM) return -1;
	if(chn_idx<0||chn_idx>=MAX_EXAM_CHN_NUM) return -1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if(dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;
	m_ExamDevIdxArr[exam_idx][type_idx][para_idx][chn_idx] = dev_idx;
	m_ExamDevChnIdxArr[exam_idx][type_idx][para_idx][chn_idx] = dev_chn_idx;
	return ret_flag;
}

/************************************************************************/
/*                          设置由设备索引通道ID                          */
/*          成功返回1;                                                   */
/*          失败返回－1;                                                 */
/************************************************************************/
int CExamIdx::SetExamDevChnArr(int dev_idx,int dev_chn_idx,unsigned short chn_id)
{
	int ret_flag = 1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if(dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;
	if(chn_id<0||chn_id>=MAX_CHANNEL_NUM) return -1;
	m_ExamDevChnArr[dev_idx][dev_chn_idx] = chn_id;
	return ret_flag;
}

//通道ID的设备idx；
int CExamIdx::SetChnDevIdx(int chn_id,int dev_idx,int dev_chn_idx)
{
	int ret_flag = 1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if(dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;
	if(chn_id<0||chn_id>=MAX_CHANNEL_NUM) return -1;
	m_ChnDevIdx[chn_id] = dev_idx;
	m_ChnDevChnIdx[chn_id] = dev_chn_idx;
	return ret_flag;
}

//键相通道数量,键相通道ID；
int CExamIdx::SetExamKeyInfo(int exam_idx,int type_idx,int para_idx,int key_num,unsigned short * key_id_arr)
{
	int ret_flag = 1,i=0;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(type_idx<0||type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(para_idx<0||para_idx>=EXAM_PARA_NUM) return -1;
	if(key_num<=0||key_id_arr==0) return -1;
	m_ExamKeyNumArr[exam_idx][type_idx][para_idx] = key_num;
	for(i=0;i<key_num;i++)
	{
		m_ExamKeyIDArr[exam_idx][type_idx][para_idx][i] = key_id_arr[i];
	}
	return ret_flag;
}

//参考通道数量,参考通道ID；
int CExamIdx::SetRefChnInfo(int exam_idx,int type_idx,int para_idx,int ref_num,unsigned short * ref_id_arr)
{
	int ret_flag = 1,i=0;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(type_idx<0||type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(para_idx<0||para_idx>=EXAM_PARA_NUM) return -1;
	if(ref_num<=0||ref_id_arr==0) return -1;
	m_ExamKeyNumArr[exam_idx][type_idx][para_idx] = ref_num;
	for(i=0;i<ref_num;i++)
	{
		m_ExamKeyIDArr[exam_idx][type_idx][para_idx][i] = ref_id_arr[i];
	}
	return ret_flag;
}

//试验开始时间；
int CExamIdx::SetExamStartTime(int exam_idx,int type_idx,int para_idx,unsigned long start_time)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(type_idx<0||type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(para_idx<0||para_idx>=EXAM_PARA_NUM) return -1;
	m_ExamStartTime[exam_idx][type_idx][para_idx] = start_time; 
	return ret_flag;
}

//试验终止时间；
int CExamIdx::SetExamEndTime(int exam_idx,int type_idx,int para_idx,unsigned long end_time)
{
	int ret_flag = 1;
	if(exam_idx<0||exam_idx>=MAX_EXAM_NUM) return -1;
	if(type_idx<0||type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(para_idx<0||para_idx>=EXAM_PARA_NUM) return -1;
	m_ExamEndTime[exam_idx][type_idx][para_idx] = end_time; 
	return ret_flag;
}

//通道中文描述桶；以设备为索引；
int CExamIdx::SetDevChnDesc(int dev_idx,int dev_chn_idx,char * chn_desc)
{
	int ret_flag = 1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if (dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;
	if(chn_desc==0) return -1;
	CStrTool::CStrcpy(m_DevChnDesc[dev_idx][dev_chn_idx],chn_desc,CHN_NAME_LEN-1);
	return ret_flag;
}

//通道单位；以设备为索引；
int CExamIdx::SetDevChnUnit(int dev_idx,int dev_chn_idx,char * chn_unit)
{
	int ret_flag = 1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if (dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;
	if(chn_unit==0) return -1;
	CStrTool::CStrcpy(m_DevChnUnit[dev_idx][dev_chn_idx],chn_unit,UNIT_STR_LEN-1);
	return ret_flag;
}

//通道所属机组序号，以设备为索引；
int CExamIdx::SetDevChnSetIdx(int dev_idx,int dev_chn_idx,int set_idx)
{
	int ret_flag = 1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if (dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;
	if(set_idx<0) return -1;
	m_DevChnSetIdx[dev_idx][dev_chn_idx] = set_idx;
	return ret_flag;
}

//通道所属电厂序号，以设备为索引；
int CExamIdx::SetDevChnPlantIdx(int dev_idx,int dev_chn_idx,int plant_idx)
{
	int ret_flag = 1;
	if(dev_idx<0||dev_idx>=MAX_DEV_NUM) return -1;
	if (dev_chn_idx<0||dev_chn_idx>=MAX_DEV_CHN_NUM) return -1;
	if(plant_idx<0) return -1;
	m_DevChnPlantIdx[dev_idx][dev_chn_idx] = plant_idx;
	return ret_flag;
}


BOOL CExamIdx::FolderExist(CString strPath)
{
     WIN32_FIND_DATA   wfd;
     BOOL rValue = FALSE;
     HANDLE hFind = FindFirstFile(strPath, &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
              rValue = TRUE;   
     }
     FindClose(hFind);
    return rValue;
}

int CExamIdx::GetChnIdxByID(unsigned short chn_id)
{
	int ret_flag = 0,i=0;
	for(i=0;i<m_ChnNum;i++)
	{
		if(m_ChnID[i]==chn_id)
			return i;
	}
	return ret_flag;
}

int CExamIdx::Serialize(int type,CString strPath)
{
	int flag = 1;

	return flag;

}

int CExamIdx::LoadDevCfg(char * file_path)
{
	int ret_flag = 1;
	if(file_path==0) return -1;
	return ret_flag;
}

int CExamIdx::LoadChnCfg(char * file_path)
{
	int ret_flag = 1;
	if(file_path==0) return -1;
	return ret_flag;
}

//增加试验；
/*
examname,试验名； 
int chnnum, 通道数量；
int *chnarr, 通道ID队列；
int *samfreqarr, 采样频率队列；
int *spanarr,处理周期队列； 
int time，试验需要时间；
*/
/*int CExamIdx::AddExam(char *examname, int chnnum, int *chnarr, int *samfreqarr, int *spanarr,char ** examchnnamearr, int time)
{
	int flag = 1,i=0,j=0;

	//数据校验；
	if(examname==NULL) return -1;
	if(chnnum<=0||chnnum>=MAX_EXAM_CHN_NUM) return -1;
	if(samfreqarr==NULL) return -1;
	if(spanarr==NULL) return -1;
	if(examchnnamearr==NULL) return -1;
	if(time<=0) return -1;

	for(i=0;i<chnnum;i++)
	{
		if(chnarr[i]<0||chnarr[i]>MAX_CHANNEL_NUM) return -1;
		if(samfreqarr[i]<=0) return -1;
		if(spanarr[i]<=0) return -1;
	}

	for(i=0;i<m_ExamNum;i++)
	{
		if(!strcmp(examname,m_ExamName[i])) return -2;
	}

	//数据装填；
	for(i=0;i<chnnum;i++)
	{
		strcpy(m_ExamChnNameArr[m_ExamNum][i],examchnnamearr[i]);
	}
	strcpy(m_ExamName[m_ExamNum],examname);
	m_ExamChnNumArr[m_ExamNum] = chnnum;
	memcpy(m_ExamChnArr[m_ExamNum],chnarr,sizeof(int) * chnnum);
	memcpy(m_ExamFreqArr[m_ExamNum],samfreqarr,sizeof(int) * chnnum);
	memcpy(m_ExamSpanArr[m_ExamNum],spanarr,sizeof(int) * chnnum);
	m_ExamTimeArr[m_ExamNum] = time;
	m_ExamNum = m_ExamNum + 1;

	return flag;
}*/


int CExamIdx::SetDevInfo(int col_idx,int row_idx,char * val_str)
{
	int ret_flag = 1;
	if(col_idx<1||col_idx>21) return -1;
	if(row_idx<0||row_idx>MAX_DEV_NUM) return -1;
	if(val_str==0) return -1;
	if(col_idx==1) CStrTool::CStrcpy(m_DevDesc[row_idx],val_str,FILE_NAME_LEN);
	else if(col_idx==3) CStrTool::CStrcpy(m_DevPlantID[row_idx],val_str,UNIT_STR_LEN);
	else if(col_idx==5) CStrTool::CStrcpy(m_DevSetID[row_idx],val_str,UNIT_STR_LEN);
	else if(col_idx==6) CStrTool::CStrcpy(m_DevIP1[row_idx],val_str,UNIT_STR_LEN);
	else if(col_idx==7) CStrTool::CStrcpy(m_DevIP2[row_idx],val_str,UNIT_STR_LEN);
	else if(col_idx==16) m_DevSamMode[row_idx] = atoi(val_str);
	else if(col_idx==17) m_DevSamFreq[row_idx] = atoi(val_str);
	else if(col_idx==19) m_DevStartID[row_idx] = atoi(val_str);
	else if(col_idx==20) m_DevEndID[row_idx] = atoi(val_str);
	else if(col_idx==21) m_DevChnNum[row_idx] = atoi(val_str);
	return ret_flag;
}

int CExamIdx::GetDevInfo(int col_idx,int row_idx,char * val_str)
{
	int ret_flag = 1;
	if(col_idx<1||col_idx>21) return -1;
	if(row_idx<0||row_idx>MAX_DEV_NUM) return -1;
	if(val_str==0) return -1;
	if(col_idx==1) CStrTool::CStrcpy(val_str,m_DevDesc[row_idx],FILE_NAME_LEN);
	else if(col_idx==3) CStrTool::CStrcpy(val_str,m_DevPlantID[row_idx],UNIT_STR_LEN);
	else if(col_idx==5) CStrTool::CStrcpy(val_str,m_DevSetID[row_idx],UNIT_STR_LEN);
	else if(col_idx==6) CStrTool::CStrcpy(val_str,m_DevIP1[row_idx],UNIT_STR_LEN);
	else if(col_idx==7) CStrTool::CStrcpy(val_str,m_DevIP2[row_idx],UNIT_STR_LEN);
	else if(col_idx==16) sprintf(val_str,"%d",m_DevSamMode[row_idx]);
	else if(col_idx==17) sprintf(val_str,"%d",m_DevSamFreq[row_idx]);
	else if(col_idx==19) sprintf(val_str,"%d",m_DevStartID[row_idx]);
	else if(col_idx==20) sprintf(val_str,"%d",m_DevEndID[row_idx]);
	else if(col_idx==21) sprintf(val_str,"%d",m_DevChnNum[row_idx]);
	return ret_flag;
}

int CExamIdx::GetDevTitle(int col_idx,char * val_str)
{
	int ret_flag = 1;
	if(col_idx<1||col_idx>21) return -1;
	if(val_str==0) return -1;
	if(col_idx==1) CStrTool::CStrcpy(val_str,"DevID",CHN_NAME_LEN);
	else if(col_idx==3) CStrTool::CStrcpy(val_str,"PlantID",CHN_NAME_LEN);
	else if(col_idx==5) CStrTool::CStrcpy(val_str,"SetID",CHN_NAME_LEN);
	else if(col_idx==6) CStrTool::CStrcpy(val_str,"DevIP1",CHN_NAME_LEN);
	else if(col_idx==7) CStrTool::CStrcpy(val_str,"DevIP2",CHN_NAME_LEN);
	else if(col_idx==16) CStrTool::CStrcpy(val_str,"DevSamMode",CHN_NAME_LEN);
	else if(col_idx==17) CStrTool::CStrcpy(val_str,"DevSamFreq",CHN_NAME_LEN);
	else if(col_idx==19) CStrTool::CStrcpy(val_str,"DevStartID",CHN_NAME_LEN);
	else if(col_idx==20) CStrTool::CStrcpy(val_str,"DevEndID",CHN_NAME_LEN);
	else if(col_idx==21) CStrTool::CStrcpy(val_str,"DevChnNum",CHN_NAME_LEN);
	return ret_flag;
}

int CExamIdx::SetChnInfo(int col_idx, int row_idx, char *val_str)
{
	int ret_flag = 1;
	if(col_idx<1||col_idx>46) return -1;
	if(row_idx<0||row_idx>MAX_CHANNEL_NUM) return -1;
	if(val_str==0) return -1;
	if(col_idx==1) CStrTool::CStrcpy(m_ChnName[row_idx],val_str,CHN_NAME_LEN);
	else if(col_idx==2) m_ChnID[row_idx] = atoi(val_str);
	else if(col_idx==3) m_ChnKeyID[row_idx] = atoi(val_str);
	else if(col_idx==4) CStrTool::CStrcpy(m_ChnSetPlace[row_idx],val_str,CHN_NAME_LEN);
	else if(col_idx==5) CStrTool::CStrcpy(m_ChnType[row_idx],val_str,UNIT_STR_LEN);
	else if(col_idx==6) CStrTool::CStrcpy(m_ChnScanDiret[row_idx],val_str,UNIT_STR_LEN);
	else if(col_idx==7) m_ChnValid[row_idx] = atoi(val_str);
	else if(col_idx==8) m_ChnSensorType[row_idx] = atoi(val_str);
	else if(col_idx==9) CStrTool::CStrcpy(m_ChnUnit[row_idx],val_str,UNIT_STR_LEN);
	else if(col_idx==10) m_ChnModel[row_idx] = atoi(val_str);
	else if(col_idx==11) CStrTool::CStrcpy(m_ChnMaker[row_idx],val_str,UNIT_STR_LEN);
	else if(col_idx==12) m_ChnIntegralWave[row_idx] = atoi(val_str);
	else if(col_idx==13) m_ChnFilterWave[row_idx] = atoi(val_str);
	else if(col_idx==14) m_ChnSensitivePara[row_idx] = (float)atof(val_str);
	else if(col_idx==15) m_ChnACCor[row_idx] =  (float)atof(val_str);
	else if(col_idx==16) m_ChnDCCor[row_idx] =  (float)atof(val_str);
	else if(col_idx==17) m_ChnIntegralCor[row_idx] =  (float)atof(val_str);
	else if(col_idx==18) m_ChnZeroOffset[row_idx] =  (float)atof(val_str);
	else if(col_idx==19) m_ChnFreqHi1[row_idx] =  (float)atof(val_str);
	else if(col_idx==20) m_ChnFreqLow1[row_idx] =  (float)atof(val_str);
	else if(col_idx==21) m_ChnFreqHi2[row_idx] =  (float)atof(val_str);
	else if(col_idx==22) m_ChnFreqLow2[row_idx] =  (float)atof(val_str);
	else if(col_idx==23) m_ChnHiVol[row_idx] =  (float)atof(val_str);
	else if(col_idx==24) m_ChnLowVol[row_idx] =  (float)atof(val_str);
	else if(col_idx==25) m_ChnRefGapVol[row_idx] =  (float)atof(val_str);
	else if(col_idx==26) m_ChnAlmHH[row_idx] =  (float)atof(val_str);
	else if(col_idx==27) m_ChnAlmH[row_idx] =  (float)atof(val_str);
	else if(col_idx==28) m_ChnRefAllFreq[row_idx] =  (float)atof(val_str);
	else if(col_idx==29) m_ChnAllFreqDev[row_idx] =  (float)atof(val_str);
	else if(col_idx==30) m_ChnOneFreqX[row_idx] =  (float)atof(val_str);
	else if(col_idx==31) m_ChnOneFreqY[row_idx] =  (float)atof(val_str);
	else if(col_idx==32) m_ChnOneFreqDev[row_idx] =  (float)atof(val_str);
	else if(col_idx==33) m_ChnTwoFreqX[row_idx] =  (float)atof(val_str);
	else if(col_idx==34) m_ChnTwoFreqY[row_idx] =  (float)atof(val_str);
	else if(col_idx==35) m_ChnTwoFreqDev[row_idx] =  (float)atof(val_str);
	else if(col_idx==36) m_ChnHalfFreqDev[row_idx] =  (float)atof(val_str);
	else if(col_idx==37) m_ChnOptFreqDev[row_idx] =  (float)atof(val_str);
	else if(col_idx==38) m_ChnRemainderDev[row_idx] =  (float)atof(val_str);
	else if(col_idx==39) m_ChnSensitiveType[row_idx] =  (float)atof(val_str);
	else if(col_idx==40) m_ChnFFTMinVal[row_idx] =  (float)atof(val_str);
	else if(col_idx==41) m_ChnFFTMinNum[row_idx] =  atoi(val_str);
	else if(col_idx==42) m_ChnVibCalType[row_idx] =  atoi(val_str);
	else if(col_idx==43) m_ChnAlmPowerUp1[row_idx] =  (float)atof(val_str);
	else if(col_idx==44) m_ChnAlmPowerDown1[row_idx] =  (float)atof(val_str);
	else if(col_idx==45) m_ChnAlmPowerUp2[row_idx] =  (float)atof(val_str);
	else if(col_idx==46) m_ChnAlmPowerDown2[row_idx] =  (float)atof(val_str);
	return ret_flag;
}


int CExamIdx::GetChnInfo(int col_idx, int row_idx, char * val_str)
{
	int ret_flag = 1;
	if(col_idx<1||col_idx>46) return -1;
	if(row_idx<0||row_idx>MAX_CHANNEL_NUM) return -1;
	if(val_str==0) return -1;
	if(col_idx==1) CStrTool::CStrcpy(val_str,m_ChnName[row_idx],CHN_NAME_LEN);
	else if(col_idx==2) sprintf(val_str,"%d",m_ChnID[row_idx]);
	else if(col_idx==3) sprintf(val_str,"%d",m_ChnKeyID[row_idx]);
	else if(col_idx==4) CStrTool::CStrcpy(val_str,m_ChnSetPlace[row_idx],CHN_NAME_LEN);
	else if(col_idx==5) CStrTool::CStrcpy(val_str,m_ChnType[row_idx],UNIT_STR_LEN);
	else if(col_idx==6) CStrTool::CStrcpy(val_str,m_ChnScanDiret[row_idx],UNIT_STR_LEN);
	else if(col_idx==7) sprintf(val_str,"%d",m_ChnValid[row_idx]);
	else if(col_idx==8) sprintf(val_str,"%d",m_ChnSensorType[row_idx]);
	else if(col_idx==9) CStrTool::CStrcpy(val_str,m_ChnUnit[row_idx],UNIT_STR_LEN);
	else if(col_idx==10) sprintf(val_str,"%d",m_ChnModel[row_idx]);
	else if(col_idx==11) CStrTool::CStrcpy(val_str,m_ChnMaker[row_idx],UNIT_STR_LEN);
	else if(col_idx==12) sprintf(val_str,"%d",m_ChnIntegralWave[row_idx]);
	else if(col_idx==13) sprintf(val_str,"%d",m_ChnFilterWave[row_idx]);
	else if(col_idx==14) sprintf(val_str,"%6.2f",m_ChnSensitivePara[row_idx]);
	else if(col_idx==15) sprintf(val_str,"%6.2f",m_ChnACCor[row_idx]);
	else if(col_idx==16) sprintf(val_str,"%6.2f",m_ChnDCCor[row_idx]);
	else if(col_idx==17) sprintf(val_str,"%6.2f",m_ChnIntegralCor[row_idx]);
	else if(col_idx==18) sprintf(val_str,"%6.2f",m_ChnZeroOffset[row_idx]);
	else if(col_idx==19) sprintf(val_str,"%6.2f",m_ChnFreqHi1[row_idx]);
	else if(col_idx==20) sprintf(val_str,"%6.2f",m_ChnFreqLow1[row_idx]);
	else if(col_idx==21) sprintf(val_str,"%6.2f",m_ChnFreqHi2[row_idx]);
	else if(col_idx==22) sprintf(val_str,"%6.2f",m_ChnFreqLow2[row_idx]);
	else if(col_idx==23) sprintf(val_str,"%6.2f",m_ChnHiVol[row_idx]);
	else if(col_idx==24) sprintf(val_str,"%6.2f",m_ChnLowVol[row_idx]);
	else if(col_idx==25) sprintf(val_str,"%6.2f",m_ChnRefGapVol[row_idx]);
	else if(col_idx==26) sprintf(val_str,"%6.2f",m_ChnAlmHH[row_idx]);
	else if(col_idx==27) sprintf(val_str,"%6.2f",m_ChnAlmH[row_idx]);
	else if(col_idx==28) sprintf(val_str,"%6.2f",m_ChnRefAllFreq[row_idx]);
	else if(col_idx==29) sprintf(val_str,"%6.2f",m_ChnAllFreqDev[row_idx]);
	else if(col_idx==30) sprintf(val_str,"%6.2f",m_ChnOneFreqX[row_idx]);
	else if(col_idx==31) sprintf(val_str,"%6.2f",m_ChnOneFreqY[row_idx]);
	else if(col_idx==32) sprintf(val_str,"%6.2f",m_ChnOneFreqY[row_idx]);
	else if(col_idx==33) sprintf(val_str,"%6.2f",m_ChnTwoFreqDev[row_idx]);
	else if(col_idx==34) sprintf(val_str,"%6.2f",m_ChnTwoFreqY[row_idx]);
	else if(col_idx==35) sprintf(val_str,"%6.2f",m_ChnTwoFreqDev[row_idx]);
	else if(col_idx==36) sprintf(val_str,"%6.2f",m_ChnHalfFreqDev[row_idx]);
	else if(col_idx==37) sprintf(val_str,"%6.2f",m_ChnOptFreqDev[row_idx]);
	else if(col_idx==38) sprintf(val_str,"%6.2f",m_ChnRemainderDev[row_idx]);
	else if(col_idx==39) sprintf(val_str,"%6.2f",m_ChnSensitiveType[row_idx]);
	else if(col_idx==40) sprintf(val_str,"%6.2f",m_ChnFFTMinVal[row_idx]);
	else if(col_idx==41) sprintf(val_str,"%d",m_ChnFFTMinNum[row_idx]);
	else if(col_idx==42) sprintf(val_str,"%d",m_ChnVibCalType[row_idx]);
	else if(col_idx==43) sprintf(val_str,"%6.2f",m_ChnAlmPowerUp1[row_idx]);
	else if(col_idx==44) sprintf(val_str,"%6.2f",m_ChnAlmPowerDown1[row_idx]);
	else if(col_idx==45) sprintf(val_str,"%6.2f",m_ChnAlmPowerUp2[row_idx]);
	else if(col_idx==46) sprintf(val_str,"%6.2f",m_ChnAlmPowerDown2[row_idx]);
	return ret_flag;
}

int CExamIdx::GetChnTitle(int col_idx,char * val_str)
{
	int ret_flag = 1;
	if(col_idx<1||col_idx>46) return -1;
	if(val_str==0) return -1;
	if(col_idx==1) CStrTool::CStrcpy(val_str,"ChnName",CHN_NAME_LEN);
	else if(col_idx==2) CStrTool::CStrcpy(val_str,"ChnID",CHN_NAME_LEN);
	else if(col_idx==3) CStrTool::CStrcpy(val_str,"KeyID",CHN_NAME_LEN);
	else if(col_idx==4) CStrTool::CStrcpy(val_str,"ChnSetPlace",CHN_NAME_LEN);
	else if(col_idx==5) CStrTool::CStrcpy(val_str,"ChnType",CHN_NAME_LEN);
	else if(col_idx==6) CStrTool::CStrcpy(val_str,"ChnScanDiret",CHN_NAME_LEN);
	else if(col_idx==7) CStrTool::CStrcpy(val_str,"ChnValid",CHN_NAME_LEN);
	else if(col_idx==8) CStrTool::CStrcpy(val_str,"ChnSensorType",CHN_NAME_LEN);
	else if(col_idx==9) CStrTool::CStrcpy(val_str,"m_ChnUnit",CHN_NAME_LEN);
	else if(col_idx==10) CStrTool::CStrcpy(val_str,"ChnModel",CHN_NAME_LEN);
	else if(col_idx==11) CStrTool::CStrcpy(val_str,"Maker",CHN_NAME_LEN);
	else if(col_idx==12) CStrTool::CStrcpy(val_str,"ChnIntegralWave",CHN_NAME_LEN);
	else if(col_idx==13) CStrTool::CStrcpy(val_str,"ChnFilterWave",CHN_NAME_LEN);
	else if(col_idx==14) CStrTool::CStrcpy(val_str,"ChnSensitivePara",CHN_NAME_LEN);
	else if(col_idx==15) CStrTool::CStrcpy(val_str,"ChnACCor",CHN_NAME_LEN);
	else if(col_idx==16) CStrTool::CStrcpy(val_str,"ChnDCCor",CHN_NAME_LEN);
	else if(col_idx==17) CStrTool::CStrcpy(val_str,"ChnIntegralCor",CHN_NAME_LEN);
	else if(col_idx==18) CStrTool::CStrcpy(val_str,"ChnZeroOffset",CHN_NAME_LEN);
	else if(col_idx==19) CStrTool::CStrcpy(val_str,"ChnFreqHi1",CHN_NAME_LEN);
	else if(col_idx==20) CStrTool::CStrcpy(val_str,"ChnFreqLow1",CHN_NAME_LEN);
	else if(col_idx==21) CStrTool::CStrcpy(val_str,"ChnFreqHi2",CHN_NAME_LEN);
	else if(col_idx==22) CStrTool::CStrcpy(val_str,"ChnFreqLow2",CHN_NAME_LEN);
	else if(col_idx==23) CStrTool::CStrcpy(val_str,"ChnHiVol",CHN_NAME_LEN);
	else if(col_idx==24) CStrTool::CStrcpy(val_str,"ChnLowVol",CHN_NAME_LEN);
	else if(col_idx==25) CStrTool::CStrcpy(val_str,"ChnRefGapVol",CHN_NAME_LEN);
	else if(col_idx==26) CStrTool::CStrcpy(val_str,"ChnAlmHH",CHN_NAME_LEN);
	else if(col_idx==27) CStrTool::CStrcpy(val_str,"ChnAlmH",CHN_NAME_LEN);
	else if(col_idx==28) CStrTool::CStrcpy(val_str,"ChnRefAllFreq",CHN_NAME_LEN);
	else if(col_idx==29) CStrTool::CStrcpy(val_str,"ChnAllFreqDev",CHN_NAME_LEN);
	else if(col_idx==30) CStrTool::CStrcpy(val_str,"ChnOneFreqX",CHN_NAME_LEN);
	else if(col_idx==31) CStrTool::CStrcpy(val_str,"ChnOneFreqY",CHN_NAME_LEN);
	else if(col_idx==32) CStrTool::CStrcpy(val_str,"ChnOneFreqDev",CHN_NAME_LEN);
	else if(col_idx==33) CStrTool::CStrcpy(val_str,"ChnTwoFreqX",CHN_NAME_LEN);
	else if(col_idx==34) CStrTool::CStrcpy(val_str,"ChnTwoFreqY",CHN_NAME_LEN);
	else if(col_idx==35) CStrTool::CStrcpy(val_str,"ChnTwoFreqDev",CHN_NAME_LEN);
	else if(col_idx==36) CStrTool::CStrcpy(val_str,"ChnHalfFreqDev",CHN_NAME_LEN);
	else if(col_idx==37) CStrTool::CStrcpy(val_str,"ChnOptFreqDev",CHN_NAME_LEN);
	else if(col_idx==38) CStrTool::CStrcpy(val_str,"ChnRemainderDev",CHN_NAME_LEN);
	else if(col_idx==39) CStrTool::CStrcpy(val_str,"ChnSensitiveType",CHN_NAME_LEN);
	else if(col_idx==40) CStrTool::CStrcpy(val_str,"ChnFFTMinVal",CHN_NAME_LEN);
	else if(col_idx==41) CStrTool::CStrcpy(val_str,"ChnFFTMinNum",CHN_NAME_LEN);
	else if(col_idx==42) CStrTool::CStrcpy(val_str,"ChnVibCalType",CHN_NAME_LEN);
	else if(col_idx==43) CStrTool::CStrcpy(val_str,"ChnAlmPowerUp1",CHN_NAME_LEN);
	else if(col_idx==44) CStrTool::CStrcpy(val_str,"ChnAlmPowerDown1",CHN_NAME_LEN);
	else if(col_idx==45) CStrTool::CStrcpy(val_str,"ChnAlmPowerUp2",CHN_NAME_LEN);
	else if(col_idx==46) CStrTool::CStrcpy(val_str,"ChnAlmPowerDown2",CHN_NAME_LEN);
	return ret_flag;
}


int CExamIdx::GetDevIdxBySetID(char * set_id)
{
	int ret_flag = 1,i=0;
	if(set_id==0) return -1;
	for(i=0;i<m_DevNum;i++)
	{
		if(!strcmp(m_DevSetID[i],set_id))
			return i;
	}
	ret_flag = -1;
	return ret_flag;
}

int CExamIdx::GetSamFreqByChnID(unsigned short chn_id,int * sam_freq)
{
	int ret_flag = 1;
	if(chn_id<0||chn_id>=MAX_CHANNEL_NUM) return -1;
	if(sam_freq==0) return -1;
	char set_id[MAX_CHN_NAME_LEN];
	memset(set_id,0,MAX_CHN_NAME_LEN);
	ret_flag = GetSetIDByChnID(chn_id,set_id);
	if(ret_flag<=0) return -2;
	ret_flag = GetDevIdxBySetID(set_id);
	if(ret_flag<0) return -3;
	* sam_freq = m_DevSamFreq[ret_flag];
	return 1;
}


int CExamIdx::GetChnIdxBySetID(char * set_id,int * chn_idx,int * chn_num)
{
	int ret_flag = 1,i=0,p=0,q=0;
	unsigned short j=0;
	if(set_id==0||chn_idx==0||chn_num==0) return -1;
	* chn_num = 0;
	for(i=0;i<m_DevNum;i++)
	{
		if(!strcmp(m_DevSetID[i],set_id))
		{
			p = m_DevStartID[i];
			q = m_DevEndID[i];
			for(j=p;j<=q;j++)
			{
				chn_idx[(*chn_num)++] = GetChnIdxByID(j);
			}
		}
	}
	return ret_flag;
}
