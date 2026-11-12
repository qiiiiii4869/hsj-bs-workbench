// StrTool.h: interface for the CStrTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRTOOL_H__8E1639EC_544C_47BC_9EC3_3342511447B0__INCLUDED_)
#define AFX_STRTOOL_H__8E1639EC_544C_47BC_9EC3_3342511447B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define BUFF_STR_LEN 1024
#include "common.h"
class CStrTool  
{
public:
	CStrTool();
	virtual ~CStrTool();

	static int split(char sp, char *line, char **lst, int lst_len);

	static char ** alloc_arr(int x,int y);

	static bool del_arr(char ** ptr,int x,int y);

	static bool del_float_arr(float ** ptr,int x);

	static bool log_str(char * file_name,char * log_str);

	static bool print_x(char * ori_ptr,int ori_len);

	static bool print_xf(char * ori_ptr,int ori_len);

	static bool print_xf(float * ori_ptr,int ori_len);

	static int CStrcpy(char * dest_str,char * src_str,int len);

	static float *** alloc_float_arr(int x,int y,int z);

	static bool del_float_arr(float *** ptr,int x,int y);

	static float *** alloc_float_arr(int x,int y,int ** z);

	static int ** alloc_int_arr(int x,int y);

	static bool del_arr(int ** ptr,int x);


};

#endif // !defined(AFX_STRTOOL_H__8E1639EC_544C_47BC_9EC3_3342511447B0__INCLUDED_)
