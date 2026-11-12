// SortData.h: interface for the CSortData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTDATA_H__E282FC6C_D1BA_4594_970D_BD8147348A26__INCLUDED_)
#define AFX_SORTDATA_H__E282FC6C_D1BA_4594_970D_BD8147348A26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "common.h"

//分拣数据；
#define MAX_SORT_SPAN_NUM 100
#define SEL_DATA_VER 20091203

class CSortData  
{
public:
	/************************************************************************/
	/* 20091203支持分频功能                                                 */
	/************************************************************************/
	int m_ChnFreqArr[MAX_EXAM_CHN_NUM];

	int m_ChnDividArr[MAX_EXAM_CHN_NUM];

	int m_Version;

	int m_SortLenArr[MAX_SORT_SPAN_NUM][MAX_EXAM_CHN_NUM];
	//int ** m_SortLenArr;
	/************************************************************************/

	int m_SamFreq;

	int m_DealSpan;
	//分拣数量；
	int m_SortNum;

	int m_OriSortNum;

	//分拣长度；
	int m_SortLen;

	//分拣起始时间；
	unsigned long m_SortTimeBeginArr[MAX_SORT_SPAN_NUM];

	//分拣终止时间；
	unsigned long m_SortTimeEndArr[MAX_SORT_SPAN_NUM];

	/***************************************************/
	//起始点数；
	int m_SortStartPoint[MAX_SORT_SPAN_NUM];

	//终止点数；
	int m_SortEndPoint[MAX_SORT_SPAN_NUM];

	//分拣起始点数；
	int m_ActionStartPoint[MAX_SORT_SPAN_NUM];

	//分拣终止点数；
	int m_ActionEndPoint[MAX_SORT_SPAN_NUM];
	/***************************************************/

	//分拣有效队列；
	unsigned char m_SortValidArr[MAX_SORT_SPAN_NUM];

	/************************************************************************/
	/* 20090506                                                             */
	/************************************************************************/
	//分拣依据ID
	unsigned short m_SortBaseIDArr[MAX_SORT_SPAN_NUM];
	//////////////////////////////////////////////////////////////////////////

	//试验类型数量；
	int m_ExamTypeNum;

	//试验参数数量；
	int m_ExamTypeParaNum[MAX_EXAM_TYPE_NUM];

	//试验状态；
	unsigned char m_ExamState;

	//试验类型状态；
	unsigned char m_ExamTypeState[MAX_EXAM_TYPE_NUM];

	//试验参数状态；
	unsigned char m_ExamTypeParaState[MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//试验参数通道状态；
	unsigned char m_ChnState[MAX_EXAM_CHN_NUM];

	//机组ID；
	char m_SetID[MAX_CHN_NAME_LEN];

	/************************************************************************/
	/* 20090726                                                             */
	/************************************************************************/
	//键相序号；
	int m_keyIdx;

	//转速序号；
	int m_velIdx;
	//////////////////////////////////////////////////////////////////////////

	//通道数量；
	int m_chnNum;

	//通道ID;
	unsigned short m_chnIDArr[MAX_EXAM_CHN_NUM];

	//通道名；
	char m_chnNameArr[MAX_EXAM_CHN_NUM][MAX_CHN_NAME_LEN];

	/************************************************************************/
	/* 20090613                                                             */
	/************************************************************************/
	//单位；
	char m_chnUnitArr[MAX_EXAM_CHN_NUM][UNIT_STR_LEN];

	//变换A系数；
	float m_aParaArr[MAX_EXAM_CHN_NUM];

	//变换B系数；
	float m_bParaArr[MAX_EXAM_CHN_NUM];
	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 20090620                                                             */
	/************************************************************************/
	//fft计算开关；
	unsigned char m_fftFlagArr[MAX_EXAM_CHN_NUM];
	//////////////////////////////////////////////////////////////////////////

	//试验名；
	char m_ExamName[MAX_CHN_NAME_LEN];

	//类型名；
	char m_ExamType[MAX_EXAM_TYPE_NUM][MAX_CHN_NAME_LEN];

	//参数名；
	char m_ExamTypePara[MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_CHN_NAME_LEN];

	//对应的分拣号；
	unsigned short m_ExamTypeParaSortCode[MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM];

	//数据；
	float *** m_sortData;
	
public:
	CSortData();
	virtual ~CSortData();
	int Clear();
	//void SetConf(int sort_num,int sort_len,int chn_num,unsigned short * m_chnidarr,char ** m_chnnamearr);
	void SetConf(int sort_num,int ** sort_len,int chn_num,unsigned short * m_chnidarr,char ** m_chnnamearr);
	void SetChnID(int chn_num,unsigned short * m_chnidarr);
	void DestroyConf();
	int AddExamName(char * exam_name);
	int AddExamType(char * exam_type);
	int AddExamTypePara(int exam_type_idx,char * exam_para);
	int ReadHead(FILE *f);
	int GetChnIdx(unsigned short chn_id);
	int GetChnIdx(char * chn_name);
	int GetSeriesPos(int sort_code,int chn_idx);
	int AddSeries(int sort_code,unsigned short chn_id,int data_num,float * val_arr);
	int ReadData(FILE *f,int sort_code,int chn_idx,unsigned long * time_start,unsigned long * time_end);
	//void DestroyOldConf();
	int GetChnIdxByDesc(char * chn_desc);
	int GetExamTypeIdx(char * exam_type);
	int GetExamParaIdx(int exam_type_idx,char * exam_para);
	//int ReadHead(CString FileName);
	int CheckVersion();
	int CheckVersion(FILE * f);
	int GetChnFreqById(unsigned short chn_id);
	int GetKeyFreq();
	//////////////////////////////////////////
	static int GetMaxDataLen(CString FilePathName, CString type,CString para);
};

#endif // !defined(AFX_SORTDATA_H__E282FC6C_D1BA_4594_970D_BD8147348A26__INCLUDED_)
