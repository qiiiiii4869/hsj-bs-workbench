// SysConf.h: interface for the CSysConf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSCONF_H__AD630F9B_3CC8_4391_BF13_A58E244E2E20__INCLUDED_)
#define AFX_SYSCONF_H__AD630F9B_3CC8_4391_BF13_A58E244E2E20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SortData.h"
#include "ExamIdx.h"
class CSysConf  
{
public:
	//试验工作目录；
	static char * EXAM_PATH;
	static char * INI_FILE_PATH;
	static CExamIdx * m_Exam_Idx;
	static CSortData * m_Sort_Data;
	CSysConf();
	virtual ~CSysConf();

};

#endif // !defined(AFX_SYSCONF_H__AD630F9B_3CC8_4391_BF13_A58E244E2E20__INCLUDED_)
