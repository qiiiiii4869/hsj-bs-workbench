// SysConf.cpp: implementation of the CSysConf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SysConf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//试验工作目录；
char * CSysConf::EXAM_PATH = 0;

char * CSysConf::INI_FILE_PATH = 0;

CExamIdx * CSysConf::m_Exam_Idx = new CExamIdx();

CSortData * CSysConf::m_Sort_Data =  new CSortData();

CSysConf::CSysConf()
{

}

CSysConf::~CSysConf()
{

}
