#include "StdAfx.h"
#include "FileData.h"
#include "ScanSel/SysConf.h"
#include "ScanSel/SortData.h"
#include "ScanSel/StrTool.h"
//#include "CSpreadSheet.h"
using namespace std;
FileData::FileData(void):fullPathName(),fileTitle(),filePath(),samFreq(),sortNum(),sortLen(),
							strATime(),strMTime(),strCTime(),examName(),examType(),chnName(),
							examParam(),dataLength(0),ChnUnitStr("")
{

}

FileData::~FileData(void)
{
}

CString FileData::AddCoutToString(int k,CString strCout)
{
	CString temp,str;
	temp.Format("%d",k);
	str="[";
	str.Append(temp);
	str.Append("] ");
	str.Append(strCout);
	return str;
}

CString FileData::RemoveCoutFromString(CString strCout)
{
	CString str;
	str=strCout.Right(strCout.GetLength()-strCout.Find(']',0)-2);
	return str;
}

CString FileData::GetCout(CString strCout)
{
	CString str=strCout.Left(strCout.Find(']',0));
	CString st=str.Right(str.GetLength()-1);
	return st;
}


void FileData::SetFullPathName(int k,CString name)
{
	if (name!="")
		fullPathName.insert(make_pair(k, name));
}
//
//std::vector<CString> FileData::GetFileName()
//{
//	return fileName;
//}
//void FileData::ClearAll()
//{
//	filePath.clear();
//	strCTime.clear();
//	strMTime.clear();
//	strATime.clear();
//	examName.clear();
//	examType.clear();
//	examParam.clear();
//}
void FileData::SetFilePorperty(CString fileFullName,CString filename, CString path, CString cTime,CString mTime,CString aTime,CString eName,
							   CString sFreq,CString tNum,CString sLen,CString cNum)
{
	//fileName.insert(make_pair(fileFullName,filename));
	fileTitle.insert(make_pair(fileFullName,filename));
	filePath.insert(make_pair(fileFullName,path));
	strCTime.insert(make_pair(fileFullName,cTime));
	strATime.insert(make_pair(fileFullName,mTime));
	strMTime.insert(make_pair(fileFullName,aTime));
	examName.insert(make_pair(fileFullName,eName));
	samFreq.insert(make_pair(fileFullName,sFreq));
	sortNum.insert(make_pair(fileFullName,tNum));
	sortLen.insert(make_pair(fileFullName,sLen));
	chnLen.insert(make_pair(fileFullName,cNum));
}


//void FileData::SetExamType(CString name,CString type)
//{
//	//vector<CString> vt;
//	//vt.push_back(type);
//	//examType.insert(make_pair(name,vt));
//}
//
//void FileData::SetExamParams(CString param)
//{	
//	//vtP.push_back(param);
//}

//void FileData::AddExamParams(CString type)
//{	
//	examParam.insert(make_pair(type,vtP));	
//}
//fileData->SetSortPorperty(CSysConf::m_Sort_Data->m_ExamTypeNum,
//						  CSysConf::m_Sort_Data->m_ExamName[MAX_CHN_NAME_LEN],
//						  CSysConf::m_Sort_Data->m_ExamType[MAX_EXAM_TYPE_NUM][MAX_CHN_NAME_LEN],
//						  CSysConf::m_Sort_Data->m_ExamTypePara[MAX_EXAM_TYPE_NUM][EXAM_PARA_NUM][MAX_CHN_NAME_LEN],
//						  CSysConf::m_Sort_Data->m_SortTimeBeginArr[MAX_SORT_SPAN_NUM],
//						  CSysConf::m_Sort_Data->m_SortTimeEndArr[MAX_SORT_SPAN_NUM],
//						  CSysConf::m_Sort_Data->m_SortBaseIDArr[MAX_SORT_SPAN_NUM],
//						  CSysConf::m_Sort_Data->m_SortStartPoint[MAX_SORT_SPAN_NUM],
//						  CSysConf::m_Sort_Data->m_SortEndPoint[MAX_SORT_SPAN_NUM],
//						  CSysConf::m_Sort_Data->m_ActionStartPoint[MAX_SORT_SPAN_NUM],
//						  CSysConf::m_Sort_Data->m_ActionEndPoint[MAX_SORT_SPAN_NUM]
//);
void  FileData::SetSortPorperty(CString FilePathName)
{	
	SortInfo sf;
	vector<SortInfo> vf;
	sf.m_ExamName.Format("%s",CSysConf::m_Sort_Data->m_ExamName);//////////////////////////////////////////////////////////////////////////

	for(int j=0;j<CSysConf::m_Sort_Data->m_ExamTypeNum;j++)	
	{
		sf.m_ExamType.Format("%s",CSysConf::m_Sort_Data->m_ExamType[j]);		//5 设置试验类型
		for(int k=0;k<CSysConf::m_Sort_Data->m_ExamTypeParaNum[j];k++)//////////////////////////////////////////////////////////////////////////***
		{
			sf.sortId.Format("%d",k+1);

			sf.m_ExamTypePara.Format("%s",CSysConf::m_Sort_Data->m_ExamTypePara[j][k]);

			CTime bTime(CSysConf::m_Sort_Data->m_SortTimeBeginArr[k]-28800); //--------------++--------------//
			sf.m_SortTimeBeginArr=bTime.Format("%Y-%m-%d  %H: %M: %S");

			CTime eTime(CSysConf::m_Sort_Data->m_SortTimeEndArr[k]-28800); //---------------++--------------//
			sf.m_SortTimeEndArr=eTime.Format("%Y-%m-%d  %H: %M: %S");

			int s=CSysConf::m_Sort_Data->m_SortBaseIDArr[j];
			int s0=CSysConf::m_Sort_Data->GetChnIdx(s);
			sf.m_SortBaseName.Format("%s",CSysConf::m_Sort_Data->m_chnNameArr[s0]);

		    sf.m_SortStartPoint.Format("%d",CSysConf::m_Sort_Data->m_SortStartPoint[k]);
		    sf.m_SortEndPoint.Format("%d",CSysConf::m_Sort_Data->m_SortEndPoint[k]);
		    //sf.m_ActionStartPoint.Format("%d",CSysConf::m_Sort_Data->m_ActionStartPoint[k]);
		    //sf.m_ActionEndPoint.Format("%d",CSysConf::m_Sort_Data->m_ActionEndPoint[k]);
			
			vf.push_back(sf);
		}
		
	}

	sortPorperty.insert(pair<CString, vector<SortInfo>>(FilePathName,vf));
}

vector<float> FileData::GetDataFromFile(CString FilePathName, CString type,CString para,CString selName,int * save_freq)
{
	//////////////////////////////////////////////////////////////////////////
	vector<float> vData;

	FILE * f=0;	
	f = fopen(FilePathName,"r+b");
	CSysConf::m_Sort_Data->ReadHead(f);

	//读取分拣某实验类型参数的通道数据
	int type_idx = CSysConf::m_Sort_Data->GetExamTypeIdx((LPSTR)(LPCSTR)type);
	int para_idx = CSysConf::m_Sort_Data->GetExamParaIdx(type_idx,(LPSTR)(LPCSTR)para);
	int data_len = 0;

	//char str_tmp[MAX_CHN_NAME_LEN];
	//memset(str_tmp,0,MAX_CHN_NAME_LEN);

	////请填入通道名，参看分拣结果目录树
	//strcpy(str_tmp,selName.GetBuffer());
	//int chn_idx = CSysConf::m_Sort_Data->GetChnIdx(str_tmp);
	int chn_idx = CSysConf::m_Sort_Data->GetChnIdx(selName.GetBuffer());
	if (chn_idx!=-1)
	{
		////
		*save_freq = CSysConf::m_Sort_Data->m_ChnFreqArr[chn_idx];
	//sprintf(str_tmp,"%d",chn_idx);

		//CSysConf::m_Sort_Data->m_chnUnitArr[MAX_EXAM_CHN_NUM][UNIT_STR_LEN];
		//CSysConf::m_Sort_Data->m_fftFlagArr[MAX_EXAM_CHN_NUM]
		//ChnUnitStr= CSysConf::m_Sort_Data->m_chnUnitArr[chn_idx];
		//ChnFftFlag=CSysConf::m_Sort_Data->m_fftFlagArr[chn_idx];

		//以下获取分拣文件所有通道信息；
		int chn_num = CSysConf::m_Sort_Data->m_chnNum;
		int all_data_len = 0;
		char ** selchnarr = CStrTool::alloc_arr(chn_num,MAX_CHN_NAME_LEN);
		int idx = 0;
		//unsigned short idt = 0;
		for(int i=0;i<chn_num;i++)
		{
			CStrTool::CStrcpy(selchnarr[i],CSysConf::m_Sort_Data->m_chnNameArr[i],MAX_CHN_NAME_LEN);
		}
		//unsigned char chn_id = (unsigned char)CSysConf::m_Sort_Data->m_chnIDArr[idx];

		//获取分拣序号；
		int sort_code = CSysConf::m_Sort_Data->m_ExamTypeParaSortCode[type_idx][para_idx];
		//获取起始终止时间；
		unsigned long begin_time = CSysConf::m_Sort_Data->m_SortTimeBeginArr[sort_code];
		unsigned long end_time = CSysConf::m_Sort_Data->m_SortTimeEndArr[sort_code];
		//获取采样频率；
		int sam_freq = CSysConf::m_Sort_Data->m_SamFreq;
		//获取点数；
		int k=CSysConf::m_Sort_Data->m_SortEndPoint[sort_code];
		int p=CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];
		int sec_num = CSysConf::m_Sort_Data->m_SortEndPoint[sort_code] - CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];

		int buffer_len = 0;

		if(f!=0)
		{
			CSysConf::m_Sort_Data->SetConf(CSysConf::m_Sort_Data->m_SortNum,0,CSysConf::m_Sort_Data->m_chnNum,CSysConf::m_Sort_Data->m_chnIDArr,selchnarr);
			CSysConf::m_Sort_Data->m_OriSortNum = CSysConf::m_Sort_Data->m_SortNum;
			//CSysConf::m_Sort_Data->m_SortLen = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan * MAX_SORT_LEN;
			CSysConf::m_Sort_Data->ReadData(f,sort_code,chn_idx,&begin_time,&end_time);
			//CStrTool::print_xf(CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx],sec_num*data_len);
			fclose(f);		
		}

		CStrTool::del_arr(selchnarr,chn_num,1);
		float * ori_ptr=CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx];


		/*vData.clear();*/
		data_len = CSysConf::m_Sort_Data->m_ChnFreqArr[chn_idx]*CSysConf::m_Sort_Data->m_DealSpan;
		for(int i=0;i<sec_num*data_len;i++)//调整，临时，键相锁定数据//Marshall  2011.3.5 14:13
		//for(int i=0;i<CSysConf::m_Sort_Data->m_SortLenArr[sort_code][chn_idx];i++)
		{
			vData.push_back(ori_ptr[i]);
		}
		dataLength=sec_num*data_len;		
		//dataLength=CSysConf::m_Sort_Data->m_SortLenArr[sort_code][chn_idx];	
	//sec_num*data_len 小于 CSysConf::m_Sort_Data->m_SortLenArr[sort_code][chn_idx]//Marshall  2011.3.5 14:40
		//一个保存频率1秒钟保存的数据个数//Marshall  2011.3.5 14:41

		CSysConf::m_Sort_Data->DestroyConf();

		//if(f!=0)
		//{


		//}

	}
	return vData;	
}


vector<float> FileData::GetKeyPhase(CString FilePathName, CString type,CString para, int* key_Freq)
{
	//////////////////////////////////////////////////////////////////////////


	FILE * f=0;	
	f = fopen(FilePathName,"r+b");
	CSysConf::m_Sort_Data->ReadHead(f);

	//读取分拣某实验类型参数的通道数据
	int type_idx = CSysConf::m_Sort_Data->GetExamTypeIdx((LPSTR)(LPCSTR)type);
	int para_idx = CSysConf::m_Sort_Data->GetExamParaIdx(type_idx,(LPSTR)(LPCSTR)para);
	int data_len = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan;

	//char str_tmp[MAX_CHN_NAME_LEN];
	//memset(str_tmp,0,MAX_CHN_NAME_LEN);

	////请填入通道名，参看分拣结果目录树
	//strcpy(str_tmp,selName.GetBuffer());
	//int chn_idx = CSysConf::m_Sort_Data->GetChnIdx(str_tmp);
	int chn_idx = CSysConf::m_Sort_Data->m_keyIdx;
	vector<float> vData;

	if (chn_idx!=-1)
	{
		//以下获取分拣文件所有通道信息；
		*key_Freq = CSysConf::m_Sort_Data->m_ChnFreqArr[chn_idx];
		int chn_num = CSysConf::m_Sort_Data->m_chnNum;
		int all_data_len = 0;
		char ** selchnarr = CStrTool::alloc_arr(chn_num,MAX_CHN_NAME_LEN);
		int idx = 0;
		//unsigned short idt = 0;
		for(int i=0;i<chn_num;i++)
		{
			CStrTool::CStrcpy(selchnarr[i],CSysConf::m_Sort_Data->m_chnNameArr[i],MAX_CHN_NAME_LEN);
		}
		//unsigned char chn_id = (unsigned char)CSysConf::m_Sort_Data->m_chnIDArr[idx];

		//获取分拣序号；
		int sort_code = CSysConf::m_Sort_Data->m_ExamTypeParaSortCode[type_idx][para_idx];
		//获取起始终止时间；
		unsigned long begin_time = CSysConf::m_Sort_Data->m_SortTimeBeginArr[sort_code];
		unsigned long end_time = CSysConf::m_Sort_Data->m_SortTimeEndArr[sort_code];
		//获取采样频率；
		int sam_freq = CSysConf::m_Sort_Data->m_SamFreq;
		//获取点数；
		int k=CSysConf::m_Sort_Data->m_SortEndPoint[sort_code];
		int p=CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];
		int sec_num = CSysConf::m_Sort_Data->m_SortEndPoint[sort_code] - CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];

		int buffer_len = 0;

		if(f!=0)
		{
			CSysConf::m_Sort_Data->SetConf(CSysConf::m_Sort_Data->m_SortNum,0,CSysConf::m_Sort_Data->m_chnNum,CSysConf::m_Sort_Data->m_chnIDArr,selchnarr);
			CSysConf::m_Sort_Data->m_OriSortNum = CSysConf::m_Sort_Data->m_SortNum;
			//CSysConf::m_Sort_Data->m_SortLen = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan * MAX_SORT_LEN;
			CSysConf::m_Sort_Data->ReadData(f,sort_code,chn_idx,&begin_time,&end_time);
			//CStrTool::print_xf(CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx],sec_num*data_len);
			fclose(f);		
		}

		CStrTool::del_arr(selchnarr,chn_num,1);
		float * ori_ptr=CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx];

		/*vData.clear();*/
		data_len = CSysConf::m_Sort_Data->m_ChnFreqArr[chn_idx]*CSysConf::m_Sort_Data->m_DealSpan;
		for(int i=0;i<sec_num*data_len;i++)
		{
			vData.push_back(ori_ptr[i]);
		}
		dataLength=sec_num*data_len;		
		CSysConf::m_Sort_Data->DestroyConf();

		//if(f!=0)
		//{


		//}
		
	}
	return vData;
	
}


CString FileData::GetChnUnit(CString FilePathName, CString selName)
{
	FILE * f=0;	
	f = fopen(FilePathName,"r+b");
	CSysConf::m_Sort_Data->ReadHead(f);

	int chn_idx = CSysConf::m_Sort_Data->GetChnIdx(selName.GetBuffer());
	ChnUnitStr= CSysConf::m_Sort_Data->m_chnUnitArr[chn_idx];
	fclose(f);

	return ChnUnitStr;
}

unsigned char FileData::GetChnFftFlag(CString FilePathName, CString selName)
{
	FILE * f=0;	
	f = fopen(FilePathName,"r+b");
	CSysConf::m_Sort_Data->ReadHead(f);
	int chn_idx = CSysConf::m_Sort_Data->GetChnIdx(selName.GetBuffer());
	ChnFftFlag=CSysConf::m_Sort_Data->m_fftFlagArr[chn_idx];
	fclose(f);

	return ChnFftFlag;		 
}

int FileData::GetChnDataLength(CString FilePathName, CString type,CString para,CString chn)
{
	FILE * f=0;	
	f = fopen(FilePathName,"r+b");
	CSysConf::m_Sort_Data->ReadHead(f);
	//读取分拣某实验类型参数的通道数据
	int type_idx = CSysConf::m_Sort_Data->GetExamTypeIdx((LPSTR)(LPCSTR)type);
	int para_idx = CSysConf::m_Sort_Data->GetExamParaIdx(type_idx,(LPSTR)(LPCSTR)para);

	int chn_idx = CSysConf::m_Sort_Data->GetChnIdx((LPSTR)(LPCSTR)chn);
	int save_freq = CSysConf::m_Sort_Data->m_ChnFreqArr[chn_idx];
	int data_len = save_freq * CSysConf::m_Sort_Data->m_DealSpan;


	//获取分拣序号；
	int sort_code = CSysConf::m_Sort_Data->m_ExamTypeParaSortCode[type_idx][para_idx];
	//获取点数；
	int sec_num = CSysConf::m_Sort_Data->m_SortEndPoint[sort_code] - CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];

	dataLength=sec_num*data_len;
	if(f!=0)
	{		
		fclose(f);
	}
	return dataLength;
}
int FileData::GetParaDataLength(CString FilePathName, CString type,CString para)
{
	/*FILE * f=0;	
	f = fopen(FilePathName,"r+b");
	CSysConf::m_Sort_Data->ReadHead(f);
	//读取分拣某实验类型参数的通道数据
	int type_idx = CSysConf::m_Sort_Data->GetExamTypeIdx((LPSTR)(LPCSTR)type);
	int para_idx = CSysConf::m_Sort_Data->GetExamParaIdx(type_idx,(LPSTR)(LPCSTR)para);

	int chn_idx = CSysConf::m_Sort_Data->GetChnIdx((LPSTR)(LPCSTR)chn);
	int save_freq = CSysConf::m_Sort_Data->m_ChnFreqArr[chn_idx];
	int data_len = save_freq * CSysConf::m_Sort_Data->m_DealSpan;


	//获取分拣序号；
	int sort_code = CSysConf::m_Sort_Data->m_ExamTypeParaSortCode[type_idx][para_idx];
	//获取点数；
	int sec_num = CSysConf::m_Sort_Data->m_SortEndPoint[sort_code] - CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];

	dataLength=sec_num*data_len;
	if(f!=0)
	{		
		fclose(f);
	}*/
	return CSortData::GetMaxDataLen(FilePathName, type, para);
}
//void FileData::WriteDataToFile(CString FilePathName,CString parName0,CString parName1,CString selName,CString fileName)/*,int fType*/
//{
//	FILE * f=0;	
//	f = fopen(FilePathName,"r+b");
//	CSysConf::m_Sort_Data->ReadHead(f);
//
//	//读取分拣某实验类型参数的通道数据
//	int type_idx = CSysConf::m_Sort_Data->GetExamTypeIdx((LPSTR)(LPCSTR)parName1);
//	int para_idx = CSysConf::m_Sort_Data->GetExamParaIdx(type_idx,(LPSTR)(LPCSTR)parName0);
//	int data_len = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan;
//
//	char str_tmp[MAX_CHN_NAME_LEN];
//	memset(str_tmp,0,MAX_CHN_NAME_LEN);
//
//	//请填入通道名，参看分拣结果目录树
//	strcpy(str_tmp,selName.GetBuffer());
//	int chn_idx = CSysConf::m_Sort_Data->GetChnIdx(str_tmp);
//	sprintf(str_tmp,"%d",chn_idx);
//
//	//以下获取分拣文件所有通道信息；
//	int chn_num = CSysConf::m_Sort_Data->m_chnNum;
//	int all_data_len = 0;
//	char ** selchnarr = CStrTool::alloc_arr(chn_num,MAX_CHN_NAME_LEN);
//	int idx = 0;
//	//unsigned short idt = 0;
//	for(int i=0;i<chn_num;i++)
//	{
//		CStrTool::CStrcpy(selchnarr[i],CSysConf::m_Sort_Data->m_chnNameArr[idx],MAX_CHN_NAME_LEN);
//	}
//	unsigned char chn_id = (unsigned char)CSysConf::m_Sort_Data->m_chnIDArr[idx];
//
//	//获取分拣序号；
//	int sort_code = CSysConf::m_Sort_Data->m_ExamTypeParaSortCode[type_idx][para_idx];
//	//获取起始终止时间；
//	unsigned long begin_time = CSysConf::m_Sort_Data->m_SortTimeBeginArr[sort_code];
//	unsigned long end_time = CSysConf::m_Sort_Data->m_SortTimeEndArr[sort_code];
//	//获取采样频率；
//	int sam_freq = CSysConf::m_Sort_Data->m_SamFreq;
//	//获取点数；
//	int sec_num = CSysConf::m_Sort_Data->m_SortEndPoint[sort_code] - CSysConf::m_Sort_Data->m_SortStartPoint[sort_code];
//
//	int buffer_len = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan * 3;
//
//	if(f!=0)
//	{
//		CSysConf::m_Sort_Data->DestroyConf();
//		CSysConf::m_Sort_Data->SetConf(CSysConf::m_Sort_Data->m_SortNum,CSysConf::m_Sort_Data->m_SortLen+buffer_len,CSysConf::m_Sort_Data->m_chnNum,CSysConf::m_Sort_Data->m_chnIDArr,selchnarr);
//		CSysConf::m_Sort_Data->m_OriSortNum = CSysConf::m_Sort_Data->m_SortNum;
//		CSysConf::m_Sort_Data->m_SortLen = CSysConf::m_Sort_Data->m_SamFreq * CSysConf::m_Sort_Data->m_DealSpan * MAX_SORT_LEN;
//		CSysConf::m_Sort_Data->ReadData(f,sort_code,chn_idx,&begin_time,&end_time);
//		//if (fType==1)
//			printFileDataToTXT(fileName,CSysConf::m_Sort_Data->m_sortData[sort_code][chn_idx],sec_num*data_len);
//		/*if (fType==2)
//		{			
//			printFileDataToExcel(fileName,parName0,parName1,selName,begin_time,end_time);
//		}*/
//
//	}
//
//	if(f!=0)
//	{		
//		fclose(f);
//	}
//
//	CStrTool::del_arr(selchnarr,chn_num,1);
//}
//
//bool FileData::printFileDataToTXT(CString FilePathName,float * ori_ptr,int ori_len)
//{
//	if(ori_ptr==0||ori_len<=0) return false;
//	int i=0;
//	//char tmp[50];
//	//printf("\n");
//	FILE	*fp = 0;
//	fp = fopen(FilePathName,"a+");
//	if(fp==0)return false;
//
//	for(i=0;i<ori_len;i++)
//	{
//		fprintf(fp,"%6.2f\n",ori_ptr[i]); 
//		//fprintf(fp,"[%d]=%6.2f;\n",i,ori_ptr[i]); 
//
//	}
//	fseek(fp,0,SEEK_END);
//	fclose(fp);
//	return true;
//}
//
//bool FileData::printFileDataToExcel(CString FilePathName,CString parName0,CString parName1,CString selName,unsigned long begin_time,unsigned long end_time)
//{
//	CSpreadSheet SS(FilePathName, parName0+parName1);
//	CStringArray sampleArray, testRow;
//
//	SS.BeginTransaction();
//
//	// 加入标题
//	sampleArray.RemoveAll();
//	//sampleArray.Add("ID");
//	sampleArray.Add("通道名");
//	sampleArray.Add("类型名");
//	sampleArray.Add("参数名");
//	sampleArray.Add("起始时间");
//	sampleArray.Add("终止时间");
//	SS.AddHeaders(sampleArray);
//	CString bgTime;
//	CString edTime;
//	CTime bTime(begin_time); 
//	bgTime=bTime.Format("%Y-%m-%d  %H: %M: %S");
//
//	CTime eTime(end_time); 
//	edTime=eTime.Format("%Y-%m-%d  %H: %M: %S");	
//		sampleArray.RemoveAll();
//		sampleArray.Add(selName);
//		sampleArray.Add(parName0);
//		sampleArray.Add(parName1);
//		sampleArray.Add(bgTime);
//		sampleArray.Add(edTime);
//		SS.AddRow(sampleArray);
//
//	SS.Commit();
//	return true;
//}

void FileData::SetExamTree(CString FilePathName)
{
	FILE * f=0;	
	f = fopen(FilePathName,"r+b");/*+b*/

	if(f!=0)
	{	
		CSysConf::m_Sort_Data->ReadHead(f);	
		fclose(f);
		
		examType.clear();
		examParam.clear();
		chnName.clear();
		CString exmName=CSysConf::m_Sort_Data->m_ExamName;
		//CString temp;
		vector<CString> typeVector,paramVector,chnVector;
		for(int j=0;j<CSysConf::m_Sort_Data->m_ExamTypeNum;j++)
		{
			typeVector.push_back(_T(CSysConf::m_Sort_Data->m_ExamType[j]));		
			for(int k=0;k<CSysConf::m_Sort_Data->m_ExamTypeParaNum[j];k++)
			{
				paramVector.push_back(_T(CSysConf::m_Sort_Data->m_ExamTypePara[j][k]));			
				for(int m=0;m<CSysConf::m_Sort_Data->m_chnNum;m++)
				{
					/*chnVector.clear();*/
					chnVector.push_back(_T(CSysConf::m_Sort_Data->m_chnNameArr[m]));				
					//temp.Format("#%d",m);				
				}
				CString paramStr=_T(CSysConf::m_Sort_Data->m_ExamTypePara[j][k]);
				chnName.insert(make_pair(paramStr,chnVector));
				chnVector.clear();
			}
			CString typStr=_T(CSysConf::m_Sort_Data->m_ExamType[j]);
			examParam.insert(make_pair(typStr,paramVector));
		}
		examType.insert(make_pair(exmName,typeVector));

	}

}

CString FileData::GetExamName(CString FilePathName)
{	
	FILE * f=0;	
	f = fopen(FilePathName,"r+b");/*+b*/
	if(f!=0)
	{
		CSysConf::m_Sort_Data->ReadHead(f);
		fclose(f);
	}
	
	return CSysConf::m_Sort_Data->m_ExamName;
}

std::vector<CString> FileData::GetExamType(CString FilePathName,CString ExamName)
{
	FILE * f=0;	
	f = fopen(FilePathName,"r+b");/*+b*/
	if(f!=0)
	{
		CSysConf::m_Sort_Data->ReadHead(f);
		fclose(f);
	}
	map<CString,std::vector<CString>>::iterator itT= examType.find(ExamName);
	
	vector<CString> it=itT->second;
	return it;
}

std::vector<CString> FileData::GetExamParam(CString FilePathName,CString ExamType)
{
	FILE * f=0;	
	f = fopen(FilePathName,"r+b");
	if(f!=0)
	{
		CSysConf::m_Sort_Data->ReadHead(f);
		fclose(f);
	}
	map<CString,std::vector<CString>>::iterator itT= examParam.find(ExamType);

	vector<CString> it=itT->second;
	return it;
}

std::vector<CString> FileData::GetChnName(CString FilePathName,CString ExamParam)
{
	FILE * f=0;	
	f = fopen(FilePathName,"r+b");/*+b*/
	if(f!=0)
	{
		CSysConf::m_Sort_Data->ReadHead(f);
		fclose(f);
	}
	map<CString,std::vector<CString>>::iterator itT= chnName.find(ExamParam);

	vector<CString> it=itT->second;
	return it;
}

CString FileData::ShowSelectDirectoryDlg(HWND m_hWnd,const   CString   &   strDlgTitle)     
{   
	char   szDir[MAX_PATH];   
	BROWSEINFO   bi;   
	ITEMIDLIST   *pidl;   
	bi.hwndOwner   =   m_hWnd;   
	bi.pidlRoot   =   NULL;   
	bi.pszDisplayName   =   szDir;   
	bi.lpszTitle   =   strDlgTitle;   
	bi.ulFlags   =   BIF_RETURNONLYFSDIRS;   
	bi.lpfn   =   NULL;   
	bi.lParam   =   0;   
	bi.iImage   =   0;   
	pidl   =   SHBrowseForFolder(&bi);   
	if(pidl   ==   NULL)   return   _T("");   
	if(!SHGetPathFromIDList(pidl,   szDir))   return   _T("");  
	CString rStr(szDir);

	if(rStr.Find('\\',0)==rStr.GetLength()-1)
	{
		rStr=rStr.Left(rStr.Find('\\',0));
	}
	return   rStr;     
}

BOOL FileData::FolderExist(CString strPath)
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

//返回应用程序所在的路径 

CString  FileData::ReturnCurrentPath()   
{  
	CString    sPath; 
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer(); 
	int    nPos;
	nPos=sPath.ReverseFind('\\');
	sPath=sPath.Left(nPos); 
	return    sPath;   

}

////Marshall  2011.2.20 19:25
void FileData::RemoveAllInfo()
{
	fullPathName.clear();
	fileTitle.clear();
	filePath.clear();
	samFreq.clear();
	sortNum.clear();
	sortLen.clear();
	strATime.clear();
	strMTime.clear();
	strCTime.clear();
	examName.clear();
	examType.clear();
	chnName.clear();
	examParam.clear();
	dataLength = 0;
	ChnUnitStr = _T("");
	ChnFftFlag = '0';
	chnLen.clear();
	chnName.clear();
	sortPorperty.clear();
}