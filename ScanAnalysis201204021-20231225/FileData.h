#pragma once
#include <vector>

typedef struct SortRecd
{
	CString sortId;					//分拣文件ID
	CString  m_ExamName;			//试验名
	CString  m_ExamType;			//实验类型名
	CString  m_ExamTypePara;		//试验参数名
	CString m_SortTimeBeginArr;		//分拣开始时间
	CString m_SortTimeEndArr;		//分拣终止时间
	CString m_SortBaseName;			//分拣依据通道名
	CString m_SortStartPoint;		//分拣起始点数
	CString m_SortEndPoint;			//分拣终止点数

	//bool operator < (SortRecd const& _A) const	//
	//{return false;}

}SortInfo, *PSortInfo;				//分拣文件信息
						  //);
class FileData
{
public:
	FileData(void);

public:
	~FileData(void);
public:
	int dataLength;									//数据长度
	CString ChnUnitStr;								//每个通道的单位
	unsigned char ChnFftFlag;						//fft计算开关
	//////////////////////////////////////////////////////////////////////////
	//						多文件数据特征管理								//
	//////////////////////////////////////////////////////////////////////////
	std::map<int,CString> fullPathName;				//分拣文件全路径（文件名+路径）
	std::map<CString,CString> fileTitle;			//文件名
	std::map<CString,CString> filePath;				//文件路径
	std::map<CString,CString> strCTime;				//文件创建时间
	std::map<CString,CString> strMTime;				//文件最后一次修改时间
	std::map<CString,CString> strATime;				//文件最后一次访问时间
	std::map<CString,CString> examName;				//试验名
	std::map<CString,CString> samFreq;				//采样频率
	std::map<CString,CString> sortNum;				//分拣次数
	std::map<CString,CString> sortLen;				//分拣点数
	std::map<CString,CString> chnLen;				//通道数量
	std::map<CString,std::vector<CString>> examType;		//试验类型
	std::map<CString,std::vector<CString>> examParam;		//试验参数
	std::map<CString,std::vector<CString>> chnName;			//试验名
	std::map<CString, std::vector<SortInfo>> sortPorperty;	//文件属性
	//////////////////////////////////////////////////////////////////////////
public:
	void SetFullPathName(int k, CString name);
	//获取文件属性
	void SetFilePorperty(CString fileFullName,CString filename,CString path, CString cTime,CString mTime,CString aTime, CString eName,CString sFreq,CString tNum,CString sLen,CString cNum);
	//void SetExamType(CString name,CString type);
	//void SetExamParams(CString param);
	//void AddExamParams(CString type);
	//std::vector<CString> GetFileName();
	//void ClearAll();
	std::vector<float> GetDataFromFile(CString FilePathName,CString type,CString para,CString selName,int * save_freq);//获取通道数据
	//void WriteDataToFile(CString FilePathName,CString type,CString para,CString selName,CString fileName);/*,int fType*/
	void SetSortPorperty(CString FilePathName);
	//bool printFileDataToTXT(CString FilePathName,float * ori_ptr,int ori_len);
	//bool printFileDataToExcel(CString FilePathName,CString parName0,CString parName1,CString selName,unsigned long begin_time,unsigned long end_time);
	int GetChnDataLength(CString FilePathName, CString parName0,CString parName1,CString chn);//获取数据长度
	int GetParaDataLength(CString FilePathName, CString type,CString para);
	CString AddCoutToString(int k,CString strCout);
	CString RemoveCoutFromString(CString strCout);
	CString GetCout(CString strCout);
	void SetExamTree(CString FilePathName);
	CString GetChnUnit(CString FilePathName,CString selName);		//获取单位
	unsigned char GetChnFftFlag(CString FilePathName,CString selName);//是否进行FFT运算
	std::vector<float> GetKeyPhase(CString FilePathName, CString type,CString para, int* key_Freq);//获取健相数据

	CString GetExamName(CString FilePathName);
	std::vector<CString> GetExamType(CString FilePathName,CString ExamName);
	std::vector<CString> GetExamParam(CString FilePathName,CString ExamType);
	std::vector<CString> GetChnName(CString FilePathName,CString ExamParam);
	BOOL FolderExist(CString strPath);						//目录是否存在
	CString ShowSelectDirectoryDlg(HWND m_hWnd,const   CString   &   strDlgTitle)  ;	//显示所选路径
	CString  ReturnCurrentPath();							//函数返回应用程序所在的路径 

	////Marshall  2011.2.20 19:24
	void RemoveAllInfo();

};
