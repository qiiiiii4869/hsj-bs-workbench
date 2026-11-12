// SortData.cpp: implementation of the CSortData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ScanAnalysis.h"
#include "SortData.h"
#include "StrTool.h"
#include "SysConf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSortData::CSortData()
{
	m_SamFreq = 0;

	m_DealSpan = 0;
	//分拣数量；
	m_SortNum = 0;

	m_OriSortNum = 0;

	//分拣长度；
	m_SortLen = 0;

	//分拣起始时间；
	memset(m_SortTimeBeginArr,0,MAX_SORT_SPAN_NUM*sizeof(long));

	//分拣终止时间；
	memset(m_SortTimeEndArr,0,MAX_SORT_SPAN_NUM*sizeof(long));

	/***************************************************/
	//起始点数；
	memset(m_SortStartPoint,0,MAX_SORT_SPAN_NUM*sizeof(int));

	//终止点数；
	memset(m_SortEndPoint,0,MAX_SORT_SPAN_NUM*sizeof(int));

	//分拣起始点数；
	memset(m_ActionStartPoint,0,MAX_SORT_SPAN_NUM*sizeof(int));

	//分拣终止点数；
	memset(m_ActionEndPoint,0,MAX_SORT_SPAN_NUM*sizeof(int));
	/***************************************************/

	//分拣有效队列；
	memset(m_SortValidArr,0,MAX_SORT_SPAN_NUM);

	memset(m_SortBaseIDArr,0,MAX_SORT_SPAN_NUM*sizeof(short));

	//试验类型数量；
	m_ExamTypeNum = 0;

	//试验参数数量；
	memset(m_ExamTypeParaNum,0,MAX_EXAM_TYPE_NUM*sizeof(int));

	//试验状态；
	m_ExamState = 0;

	//试验类型状态；
	memset(m_ExamTypeState,0,MAX_EXAM_TYPE_NUM);

	//试验参数状态；
	memset(m_ExamTypeParaState,0,MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM);

	//试验参数通道状态；
	memset(m_ChnState,0,MAX_EXAM_CHN_NUM);

	//机组ID；
	memset(m_SetID,0,MAX_CHN_NAME_LEN);

	//键相序号；
	m_keyIdx = 0;

	m_velIdx = 0;

	//通道数量；
	m_chnNum = 0;

	//通道ID;
	memset(m_chnIDArr,0,MAX_EXAM_CHN_NUM*sizeof(short));

	//通道名；
	memset(m_chnNameArr,0,MAX_EXAM_CHN_NUM*MAX_CHN_NAME_LEN);

	/************************************************************************/
	/* 20090613                                                             */
	/************************************************************************/
	//单位；
	memset(m_chnUnitArr,0,MAX_EXAM_CHN_NUM*UNIT_STR_LEN);

	//变换A系数；
	memset(m_aParaArr,0,MAX_EXAM_CHN_NUM*sizeof(float));

	//变换B系数；
	memset(m_bParaArr,0,MAX_EXAM_CHN_NUM*sizeof(float));
	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/* 20090620                                                             */
	/************************************************************************/
	memset(m_fftFlagArr,0,MAX_EXAM_CHN_NUM);
	//////////////////////////////////////////////////////////////////////////

	//试验名；
	memset(m_ExamName,0,MAX_CHN_NAME_LEN);

	//类型名；
	memset(m_ExamType,0,MAX_EXAM_TYPE_NUM*MAX_CHN_NAME_LEN);

	//参数名；
	memset(m_ExamTypePara,0,MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM*MAX_CHN_NAME_LEN);

	memset(m_ExamTypeParaSortCode,0,MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM*sizeof(unsigned short));

	//数据；
	m_sortData = 0;
	/************************************************************************/
	/* 20091203支持分频功能                                                 */
	/************************************************************************/
	memset(m_ChnFreqArr,0,MAX_EXAM_CHN_NUM*sizeof(int));

	memset(m_ChnDividArr,0,MAX_EXAM_CHN_NUM*sizeof(int));

	memset(m_SortLenArr,0,MAX_SORT_SPAN_NUM*MAX_EXAM_CHN_NUM*sizeof(int));
	//m_SortLenArr = 0;

	m_Version = 20091203;
}

CSortData::~CSortData()
{
	DestroyConf();
}

void CSortData::SetChnID(int chn_num,unsigned short * m_chnidarr)
{
	if(chn_num<=0||m_chnidarr==0) return;
	m_chnNum = chn_num;
	memcpy(m_chnIDArr,m_chnidarr,m_chnNum*sizeof(short));
}

/*void CSortData::SetConf(int sort_num,int sort_len,int chn_num,unsigned short * m_chnidarr,char ** m_chnnamearr)
{
	if(sort_num<=0||sort_len<=0||chn_num<=0||m_chnidarr==0||m_chnnamearr==0) return;
	/************************************************************************/
	/* 确保DestroyOldConf生效；                                             */
	/************************************************************************/
	/*m_OriSortNum = m_SortNum;
	/////////////////////////////////////////////////////////////////////////
	m_SortNum = sort_num;
	m_SortLen = sort_len;
	m_chnNum = chn_num;
	memcpy(m_chnIDArr,m_chnidarr,m_chnNum*sizeof(short));
	int i=0;
	for(i=0;i<m_chnNum&&i<MAX_EXAM_CHN_NUM;i++)
	{
		CStrTool::CStrcpy(m_chnNameArr[i],m_chnnamearr[i],MAX_CHN_NAME_LEN);
	}
	m_sortData = CStrTool::alloc_float_arr(m_SortNum,m_chnNum,m_SortLen);
}*/

void CSortData::SetConf(int sort_num,int ** sort_len,int chn_num,unsigned short * m_chnidarr,char ** m_chnnamearr)
{
	if(sort_num<=0/*||sort_len==0*/||chn_num<=0||m_chnidarr==0||m_chnnamearr==0) return;
	/************************************************************************/
	/* 确保DestroyOldConf生效；                                             */
	/************************************************************************/
	m_OriSortNum = m_SortNum;
	/////////////////////////////////////////////////////////////////////////
	m_SortNum = sort_num;
	m_chnNum = chn_num;
	int i=0,j=0;
	int ** m_sort_len_arr = CStrTool::alloc_int_arr(m_SortNum,m_chnNum);
	if(sort_len!=0)
	{
		for(i=0;i<m_SortNum;i++)
		{
			for(j=0;j<m_chnNum;j++)
			{
				m_SortLenArr[i][j] = sort_len[i][j];
				m_sort_len_arr[i][j] = sort_len[i][j];
			}
		}
	}
	else
	{
		for(i=0;i<m_SortNum;i++)
		{
			for(j=0;j<m_chnNum;j++)
			{
				m_sort_len_arr[i][j] = m_SortLenArr[i][j];
			}
		}
	}

	memcpy(m_chnIDArr,m_chnidarr,m_chnNum*sizeof(short));
	for(i=0;i<m_chnNum&&i<MAX_EXAM_CHN_NUM;i++)
	{
		CStrTool::CStrcpy(m_chnNameArr[i],m_chnnamearr[i],MAX_CHN_NAME_LEN);
	}
	m_sortData = CStrTool::alloc_float_arr(m_SortNum,m_chnNum,m_sort_len_arr);
	CStrTool::del_arr(m_sort_len_arr,m_SortNum);
	m_sort_len_arr = 0;
}

void CSortData::DestroyConf()
{
	if(m_sortData!=0)
	{
		CStrTool::del_float_arr(m_sortData,m_SortNum,m_chnNum);
		m_sortData = 0;
	}
	
}

//void CSortData::DestroyOldConf()
//{
//	if(m_sortData!=0&&m_OriSortNum>0)
//	{
//		CStrTool::del_float_arr(m_sortData,m_OriSortNum,m_chnNum);
//		m_sortData = 0;
//	}
//	
//}

int CSortData::Clear()
{
	int ret_flag = 1,i=0;
	//试验类型数量；
	m_ExamTypeNum = 0;

	//试验参数数量；
	memset(m_ExamTypeParaNum,0,MAX_EXAM_TYPE_NUM*sizeof(int));

	//试验状态；
	m_ExamState = 0;

	//试验类型状态；
	memset(m_ExamTypeState,0,MAX_EXAM_TYPE_NUM);

	//试验参数状态；
	memset(m_ExamTypeParaState,0,MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM);
	//试验名；
	memset(m_ExamName,0,MAX_CHN_NAME_LEN);

	//类型名；
	memset(m_ExamType,0,MAX_EXAM_TYPE_NUM*MAX_CHN_NAME_LEN);

	//参数名；
	memset(m_ExamTypePara,0,MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM*MAX_CHN_NAME_LEN);
	/************************************************************************/
	/* 20091203支持分频功能                                                 */
	/************************************************************************/
	memset(m_ChnFreqArr,0,MAX_EXAM_CHN_NUM*sizeof(int));

	memset(m_ChnDividArr,0,MAX_EXAM_CHN_NUM*sizeof(int));

	memset(m_SortLenArr,0,MAX_SORT_SPAN_NUM*MAX_EXAM_CHN_NUM*sizeof(int));

	m_Version = 20091203;

	return ret_flag;
}

int CSortData::AddExamName(char * exam_name)
{
	int ret_flag = 1;
	if(exam_name==0) return -1;
	if(strlen(exam_name)>FILE_NAME_LEN) return -2;
	CStrTool::CStrcpy(m_ExamName,exam_name,MAX_CHN_NAME_LEN);
	m_ExamState = IN_USED;
	return ret_flag;
}

int CSortData::AddExamType(char * exam_type)
{
	int ret_flag = 1;
	if(exam_type==0) return -1;
	if(m_ExamTypeNum>=MAX_EXAM_TYPE_NUM) return -3;
	int i=0;
	for(i=0;i<MAX_EXAM_TYPE_NUM;i++)
	{
		if(m_ExamTypeState[i]==NO_USED)
			break;
		else
		{
			if(!strcmp(m_ExamType[i],exam_type)) return i;
		}
	}
	strcpy(m_ExamType[i],exam_type);
	m_ExamTypeState[i]= IN_USED;
	m_ExamTypeNum++;
	ret_flag = i;
	return ret_flag;
}

int CSortData::GetExamTypeIdx(char * exam_type)
{
	if(exam_type==0) return -1;
	int i=0;
	for(i=0;i<MAX_EXAM_TYPE_NUM;i++)
	{
		if(m_ExamTypeState[i]==NO_USED)
			break;
		else
		{
			if(!strcmp(m_ExamType[i],exam_type)) return i;
		}
	}
	return -1;
}

int CSortData::GetExamParaIdx(int exam_type_idx,char * exam_para)
{
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(m_ExamTypeParaNum[exam_type_idx]>=EXAM_PARA_NUM) return -3;
	if(exam_para==0) return -1;
	int i=0,j=0;
	for(i=0;i<EXAM_PARA_NUM;i++)
	{
		if(m_ExamTypeParaState[exam_type_idx][i]==NO_USED)
			break;
		else
		{
			if(!strcmp(m_ExamTypePara[exam_type_idx][i],exam_para)) 
			{
				return i;
			}
		}
	}
	return -1;
}


int CSortData::AddExamTypePara(int exam_type_idx,char * exam_para)
{
	int ret_flag = 1;
	if(exam_type_idx<0||exam_type_idx>=MAX_EXAM_TYPE_NUM) return -1;
	if(m_ExamTypeParaNum[exam_type_idx]>=EXAM_PARA_NUM) return -3;
	if(exam_para==0) return -1;
	int i=0,j=0;
	for(i=0;i<EXAM_PARA_NUM;i++)
	{
		if(m_ExamTypeParaState[exam_type_idx][i]==NO_USED)
			break;
		else
		{
			if(!strcmp(m_ExamTypePara[exam_type_idx][i],exam_para)) 
			{
				return i;
			}
		}
	}
	strcpy(m_ExamTypePara[exam_type_idx][i],exam_para);
	m_ExamTypeParaState[exam_type_idx][i] = IN_USED;
	m_ExamTypeParaNum[exam_type_idx]++;
	ret_flag = i;
	return ret_flag;
}

int CSortData::ReadHead(FILE *f)
{
	//Clear();
	try
	{
		int ret_flag = 1,i=0,j=0;
		if(f==0) return -1;
		if (fseek(f, 0L, SEEK_SET) < 0) return -2;
		/************************************************************************/
		/* 20091203支持分频功能                                                 */
		/************************************************************************/
		fread(&m_Version,sizeof(int),1,f);
		fread(m_ChnFreqArr,sizeof(int),MAX_EXAM_CHN_NUM,f);
		fread(m_ChnDividArr,sizeof(int),MAX_EXAM_CHN_NUM,f);
		fread(m_SortLenArr,sizeof(int),MAX_SORT_SPAN_NUM*MAX_EXAM_CHN_NUM,f);
		/***************************************************/

		if(fread(&m_SamFreq,sizeof(int),1,f)==0)return -1;
		if(fread(&m_DealSpan,sizeof(int),1,f)==0)return -1;
		fread(&m_SortNum,sizeof(int),1,f);
		fread(&m_SortLen,sizeof(int),1,f);
		fread(m_SortTimeBeginArr,sizeof(long),MAX_SORT_SPAN_NUM,f);
		fread(m_SortTimeEndArr,sizeof(long),MAX_SORT_SPAN_NUM,f);
		/***************************************************/
		//起始点数；
		fread(m_SortStartPoint,sizeof(int),MAX_SORT_SPAN_NUM,f);

		//终止点数；
		fread(m_SortEndPoint,sizeof(int),MAX_SORT_SPAN_NUM,f);

		//分拣起始点数；
		fread(m_ActionStartPoint,sizeof(int),MAX_SORT_SPAN_NUM,f);

		//分拣终止点数；
		fread(m_ActionEndPoint,sizeof(int),MAX_SORT_SPAN_NUM,f);
		/***************************************************/
		fread(m_SortValidArr,sizeof(char),MAX_SORT_SPAN_NUM,f);
		//////////////////////////////////////////////////////20090506;
		fread(m_SortBaseIDArr,sizeof(short),MAX_SORT_SPAN_NUM,f);
		fread(&m_ExamTypeNum,sizeof(int),1,f);
		fread(m_ExamTypeParaNum,sizeof(int),MAX_EXAM_TYPE_NUM,f);
		fread(&m_ExamState,sizeof(char),1,f);
		fread(m_ExamTypeState,sizeof(char),MAX_EXAM_TYPE_NUM,f);
		fread(m_ExamTypeParaState,sizeof(char),MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM,f);
		fread(m_ChnState,sizeof(char),MAX_EXAM_CHN_NUM,f);
		fread(m_SetID,sizeof(char),MAX_CHN_NAME_LEN,f);
		fread(&m_keyIdx,sizeof(int),1,f);
		fread(&m_velIdx,sizeof(int),1,f);
		fread(&m_chnNum,sizeof(int),1,f);
		fread(m_chnIDArr,sizeof(unsigned short),MAX_EXAM_CHN_NUM,f);
		/*********************************************************************/
		fread(m_chnNameArr,sizeof(char),MAX_EXAM_CHN_NUM*MAX_CHN_NAME_LEN,f);
		/*********************************************************************/
		/*********************************************************************/
		/************************************************************************/
		/* 20090613                                                             */
		/************************************************************************/
		//单位；
		fread(m_chnUnitArr,sizeof(char),MAX_EXAM_CHN_NUM*UNIT_STR_LEN,f);

		//变换A系数；
		fread(m_aParaArr,sizeof(float),MAX_EXAM_CHN_NUM,f);

		//变换B系数；
		fread(m_bParaArr,sizeof(float),MAX_EXAM_CHN_NUM,f);
		//////////////////////////////////////////////////////////////////////////
		/************************************************************************/
		/* 20090620                                                             */
		/************************************************************************/
		fread(m_fftFlagArr,sizeof(char),MAX_EXAM_CHN_NUM,f);
		//////////////////////////////////////////////////////////////////////////
		fread(m_ExamName,sizeof(char),MAX_CHN_NAME_LEN,f);
		fread(m_ExamType,sizeof(char),MAX_EXAM_TYPE_NUM*MAX_CHN_NAME_LEN,f);
		fread(m_ExamTypePara,sizeof(char),MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM*MAX_CHN_NAME_LEN,f);
		fread(m_ExamTypeParaSortCode,sizeof(short),MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM,f);	
		return ret_flag;

	}
	catch(CException* e)
	{
		AfxMessageBox("分拣文件数据错误，打开失败！");
		//pCMF->m_wndOutput.FillText("分拣文件数据错误，打开失败！");

	}
}

//int CSortData::ReadHead(CString FileName)
//{
//	//Clear();
//	FILE * f=0;	
//	f = fopen(FileName,"r+b");/*+b*/
//	int ret_flag = 1,i=0,j=0;
//	if(f!=0)
//	{	
//		CSysConf::m_Sort_Data->ReadHead(f);	
//
//
//		if(f==0) return -1;
//		if (fseek(f, 0L, SEEK_SET) < 0) return -2;
//		fread(&m_SamFreq,sizeof(int),1,f);
//		fread(&m_DealSpan,sizeof(int),1,f);
//		fread(&m_SortNum,sizeof(int),1,f);
//		fread(&m_SortLen,sizeof(int),1,f);
//		fread(m_SortTimeBeginArr,sizeof(long),MAX_SORT_SPAN_NUM,f);
//		fread(m_SortTimeEndArr,sizeof(long),MAX_SORT_SPAN_NUM,f);
//		/***************************************************/
//		//起始点数；
//		fread(m_SortStartPoint,sizeof(int),MAX_SORT_SPAN_NUM,f);
//
//		//终止点数；
//		fread(m_SortEndPoint,sizeof(int),MAX_SORT_SPAN_NUM,f);
//
//		//分拣起始点数；
//		fread(m_ActionStartPoint,sizeof(int),MAX_SORT_SPAN_NUM,f);
//
//		//分拣终止点数；
//		fread(m_ActionEndPoint,sizeof(int),MAX_SORT_SPAN_NUM,f);
//		/***************************************************/
//		fread(m_SortValidArr,sizeof(char),MAX_SORT_SPAN_NUM,f);
//		//////////////////////////////////////////////////////20090506;
//		fread(m_SortBaseIDArr,sizeof(short),MAX_SORT_SPAN_NUM,f);
//		fread(&m_ExamTypeNum,sizeof(int),1,f);
//		fread(m_ExamTypeParaNum,sizeof(int),MAX_EXAM_TYPE_NUM,f);
//		fread(&m_ExamState,sizeof(char),1,f);
//		fread(m_ExamTypeState,sizeof(char),MAX_EXAM_TYPE_NUM,f);
//		fread(m_ExamTypeParaState,sizeof(char),MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM,f);
//		fread(m_ChnState,sizeof(char),MAX_EXAM_CHN_NUM,f);
//		fread(m_SetID,sizeof(char),MAX_CHN_NAME_LEN,f);
//		fread(&m_chnNum,sizeof(int),1,f);
//		fread(m_chnIDArr,sizeof(unsigned short),MAX_EXAM_CHN_NUM,f);
//		/*********************************************************************/
//		fread(m_chnNameArr,sizeof(char),MAX_EXAM_CHN_NUM*MAX_CHN_NAME_LEN,f);
//		/*********************************************************************/
//		/*********************************************************************/
//		/************************************************************************/
//		/* 20090613                                                             */
//		/************************************************************************/
//		//单位；
//		fread(m_chnUnitArr,sizeof(char),MAX_EXAM_CHN_NUM*UNIT_STR_LEN,f);
//
//		//变换A系数；
//		fread(m_aParaArr,sizeof(float),MAX_EXAM_CHN_NUM,f);
//
//		//变换B系数；
//		fread(m_bParaArr,sizeof(float),MAX_EXAM_CHN_NUM,f);
//		//////////////////////////////////////////////////////////////////////////
//		/************************************************************************/
//		/* 20090620                                                             */
//		/************************************************************************/
//		fread(m_fftFlagArr,sizeof(char),MAX_EXAM_CHN_NUM,f);
//		//////////////////////////////////////////////////////////////////////////
//		fread(m_ExamName,sizeof(char),MAX_CHN_NAME_LEN,f);
//		fread(m_ExamType,sizeof(char),MAX_EXAM_TYPE_NUM*MAX_CHN_NAME_LEN,f);
//		fread(m_ExamTypePara,sizeof(char),MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM*MAX_CHN_NAME_LEN,f);
//		fread(m_ExamTypeParaSortCode,sizeof(short),MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM,f);
//		fclose(f);
//
//	}
//	return ret_flag;
//}

int CSortData::GetChnIdx(unsigned short chn_id)
{
	int i=0;
	for(i=0;i<m_chnNum;i++)
	{
		if(m_chnIDArr[i]==chn_id) return i;
	}
	return -1;
}

int CSortData::GetChnIdx(char * chn_name)
{
	int i=0;
	if(chn_name==0) return -1;
	for(i=0;i<m_chnNum;i++)
	{
		if(!strcmp(m_chnNameArr[i],chn_name)) return i;
	}
	return -1;
}

int CSortData::GetChnIdxByDesc(char * chn_desc)
{
	int ret_flag = -1;
	if(chn_desc==0) return -1;
	char str_tmp[MAX_CHN_NAME_LEN];
	memset(str_tmp,0,MAX_CHN_NAME_LEN);
	int i=0,chn_idx=0;
	for(i=0;i<m_chnNum;i++)
	{
		chn_idx = CSysConf::m_Exam_Idx->GetChnIdxByID(m_chnIDArr[i]);
		CStrTool::CStrcpy(str_tmp,CSysConf::m_Exam_Idx->m_ChnName[chn_idx],MAX_CHN_NAME_LEN);
		if(!strcmp(str_tmp,chn_desc))
		{
			return i;
		}
	}
	return ret_flag;
}

int CSortData::AddSeries(int sort_code,unsigned short chn_id,int data_num,float * val_arr)
{
	int ret_flag = 1;
	if(sort_code<0||sort_code>=m_SortNum) return -1;
	if(chn_id<0||sort_code>=MAX_CHANNEL_NUM) return -1;
	if(data_num>m_SortLen) return -1;
	if(val_arr==0) return -1;
	int i=GetChnIdx(chn_id);
	if(i<0) return -2;
	memcpy(m_sortData[sort_code][i],val_arr,data_num*sizeof(float));
	return ret_flag;
}

int CSortData::GetSeriesPos(int sort_code,int chn_idx)
{
	int head_len =  sizeof(int) * 2 + sizeof(int) * 2 + 2* sizeof(long) * MAX_SORT_SPAN_NUM+ 4* sizeof(int) * MAX_SORT_SPAN_NUM
		+ sizeof(char) * MAX_SORT_SPAN_NUM + sizeof(short) * MAX_SORT_SPAN_NUM + sizeof(int)
		+ sizeof(int) * MAX_EXAM_TYPE_NUM + sizeof(char)
		+ sizeof(char) * MAX_EXAM_TYPE_NUM + sizeof(char) * MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM
		+ sizeof(char) * MAX_EXAM_CHN_NUM + sizeof(char) * MAX_CHN_NAME_LEN
		+ sizeof(int) + sizeof(short) * MAX_EXAM_CHN_NUM + sizeof(char) * MAX_EXAM_CHN_NUM*MAX_CHN_NAME_LEN
		+ sizeof(char) * MAX_EXAM_CHN_NUM * UNIT_STR_LEN + sizeof(float) * MAX_EXAM_CHN_NUM + sizeof(float) * MAX_EXAM_CHN_NUM 
		+ sizeof(char) * MAX_EXAM_CHN_NUM
		+ sizeof(char) * MAX_CHN_NAME_LEN
		+ sizeof(char)*MAX_EXAM_TYPE_NUM*MAX_CHN_NAME_LEN 
		+ sizeof(char)*MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM*MAX_CHN_NAME_LEN
		+ sizeof(short)*MAX_EXAM_TYPE_NUM*EXAM_PARA_NUM
		+ sizeof(int)*2
		//20091203;
		+ MAX_EXAM_CHN_NUM*sizeof(int)*2 + sizeof(int) + MAX_SORT_SPAN_NUM*MAX_EXAM_CHN_NUM*sizeof(int);	

	int off_set = 0,p_idx=0;
	//off_set = (sort_code * m_chnNum + chn_idx) * m_SortLen * sizeof(float);
	//20091203;
	int i=0,j=0;
	for(i=0;i<sort_code;i++)
	{
		for(j=0;j<m_chnNum;j++)
			off_set += m_SortLenArr[i][j] * sizeof(float);
	}
	for(i=0;i<chn_idx;i++)
	{
		off_set += m_SortLenArr[sort_code][i] * sizeof(float);
	}

	///////////////////////////////////////////////////////////////////////
	p_idx = head_len + off_set;
	return p_idx;
}

int CSortData::ReadData(FILE *f,int sort_code,int chn_idx,unsigned long * time_start,unsigned long * time_end)
{
	int ret_flag = 1;
	if(f==0) return -1;
	if(sort_code<0||sort_code>=m_SortNum) return -1;
	if(chn_idx<0||chn_idx>=MAX_EXAM_CHN_NUM) return -1;
	if(time_start==0||time_end==0) return -1;
	int p_idx = GetSeriesPos(sort_code,chn_idx);
	if (fseek(f, p_idx, SEEK_SET) < 0) return -2;
	//fread(m_sortData[sort_code][chn_idx],sizeof(float),m_SortLen,f);
	///////////////////////////////////////////////////////////////////////////////
	//20091203;
	fread(m_sortData[sort_code][chn_idx],sizeof(float),m_SortLenArr[sort_code][chn_idx],f);
	///////////////////////////////////////////////////////////////////////////////
	*time_start =  m_SortTimeBeginArr[sort_code];
	*time_end =  m_SortTimeEndArr[sort_code];
	return ret_flag;
}


int CSortData::CheckVersion()
{
	int ret_flag = 1;
	if(m_Version!=20091203)
		return -1;
	return ret_flag;
}

int CSortData::CheckVersion(FILE * f)
{
	int ret_flag = 1;
	if(f==0) return -1;
	if (fseek(f, 0L, SEEK_SET) < 0) return -2;
	/************************************************************************/
	/* 20091203支持分频功能                                                 */
	/************************************************************************/
	fread(&m_Version,sizeof(int),1,f);
	ret_flag = CheckVersion();
	return ret_flag;
}

int CSortData::GetChnFreqById(unsigned short chn_id)
{
	int ret_flag = 1;
	if(chn_id<0||chn_id>=MAX_CHANNEL_NUM) return -1;
	int chn_idx = GetChnIdx(chn_id);
	if(chn_idx<0||chn_idx>=m_chnNum) return -1;
	ret_flag = m_ChnFreqArr[chn_idx];
	return ret_flag;
}

int CSortData::GetKeyFreq()
{
	int ret_flag = 1;
	if(m_keyIdx<0||m_keyIdx>=m_chnNum) return -1;
	ret_flag = m_ChnFreqArr[m_keyIdx];
	return ret_flag;
}

int CSortData::GetMaxDataLen(CString FilePathName, CString type,CString para)
{
	int max_len = 0;
	if(FilePathName.GetLength()<=0||type.GetLength()<=0||para.GetLength()<=0)
		return -1;

	FILE * f=0;	
	f = fopen(FilePathName,"r+b");
	CSortData m_sortData;
	m_sortData.ReadHead(f);
	//读取分拣某实验类型参数的通道数据
	int type_idx = m_sortData.GetExamTypeIdx((LPSTR)(LPCSTR)type);
	int para_idx = m_sortData.GetExamParaIdx(type_idx,(LPSTR)(LPCSTR)para);
	//获取分拣序号；
	int sort_code = m_sortData.m_ExamTypeParaSortCode[type_idx][para_idx];
	int i=0;
	for(i=0;i<m_sortData.m_chnNum;i++)
	{
		if(m_sortData.m_SortLenArr[sort_code][i]>max_len)
			max_len = m_sortData.m_SortLenArr[sort_code][i];
	}
	fclose(f);
	return max_len;
}




