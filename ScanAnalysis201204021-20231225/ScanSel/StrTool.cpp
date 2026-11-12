// StrTool.cpp: implementation of the CStrTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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

CStrTool::CStrTool()
{

}

CStrTool::~CStrTool()
{

}

int CStrTool::split(char sp, char *line, char **lst, int lst_len)
{
        int i;
        int j=0;
        int p = 0;
        int str_len = (int)strlen(line);
        p = 0;
        for (i=0; i<str_len; i++) {
            if (line[i] == sp) {
                 if (j >= lst_len) break;
               //     lst[i++] = line + 1;
            	if(i>p)
            	{
				   memcpy(lst[j],&line[p],i-p);
				   lst[j++][i-p]='\0';
				   p = i+1;
            	}
           }
        }
        if(i>p)
        {
			memcpy(lst[j],&line[p],i-p);
			lst[j++][i-p]='\0';
        }
        return j;
}

char ** CStrTool::alloc_arr(int x,int y)
{
	if(x<=0||y<=0) return 0;
	char **ptr = new char*[x];
	int i=0;
	for(i=0;i<x;i++)
	{
		ptr[i] = new char[y];
		memset(ptr[i],0,sizeof(char)*y);
	}
	return ptr;
}

bool CStrTool::del_arr(char ** ptr,int x,int y)
{
	if(ptr==0||x<=0||y<=0) return false;
	int i=0;
	for(i=0;i<x;i++)
	{
		delete [] ptr[i];
		ptr[i] = 0;
	}
	delete [] ptr;
	ptr = 0;
	return true;
}

float *** CStrTool::alloc_float_arr(int x,int y,int z)
{
	if(x<=0||y<=0||z<=0) return 0;
	int i=0,j=0;
	float *** ptr = new float**[x];
	for(i=0;i<x;i++)
	{
		ptr[i] = new float*[y];
		for(j=0;j<y;j++)
		{
			ptr[i][j] = new float[z];
			memset(ptr[i][j],0,z*sizeof(float));
		}
	}
	return ptr;
}

bool CStrTool::del_float_arr(float *** ptr,int x,int y)
{
	if(x<=0||y<=0) return false;
	int i=0,j=0;
	if(ptr!=0)
	{
		for(i=0;i<x;i++)
		{
			for(j=0;j<y;j++)
			{
				if(ptr[i][j]!=0)
				{
					delete [] ptr[i][j];
					ptr[i][j] = 0;
				}
			}
			if(ptr[i]!=0)
			{
				delete [] ptr[i];
				ptr[i] = 0;
			}
		}
		delete [] ptr;
		ptr = 0;
	}
	return true;
}

bool CStrTool::del_float_arr(float ** ptr,int x)
{
	if(ptr==0||x<=0) return false;
	int i=0;
	for(i=0;i<x;i++)
	{
		delete [] ptr[i];
		ptr[i] = 0;
	}
	delete [] ptr;
	ptr = 0;
	return true;
}

bool CStrTool::log_str(char * file_name,char * log_str)
{
		bool bflag = true;
		if(file_name==0||log_str==0) return false;
		FILE	*fp = 0;
		int num = 0;
		fp = fopen(file_name,"a+");
		while(fp==0&&num<5)
		{
			fp = fopen(file_name,"a+");
			Sleep(20);
			num++;
		}
		if(fp==0)return false;
		fseek(fp,0,SEEK_END);
		fprintf(fp,	"%s",log_str);
		fclose(fp);
		return bflag;
}

bool CStrTool::print_x(char * ori_ptr,int ori_len)
{
	/*if(ori_ptr==0||ori_len<=0) return false;
	int i=0;
	for(i=0;i<ori_len;i++)
	{
		if(i==0)printf("\n[MSG£º]");
		printf("%.2x ",(unsigned char)ori_ptr[i]);
	}*/
	return true;
}

bool CStrTool::print_xf(char * ori_ptr,int ori_len)
{
		if(ori_ptr==0||ori_len<=0) return false;
		int i=0;
		char tmp[50];
		//printf("\n");
		for(i=0;i<ori_len;i++)
		{
			sprintf(tmp,"tmp_buf[%d]=0x%.2x;\n",i,(unsigned char)ori_ptr[i]);
			log_str("ERR_INFO.txt",tmp);
			//printf("%s",tmp);
		}
		//printf("**ori_len=%d**\n",ori_len);
		//printf("\n");
		return true;
}

bool CStrTool::print_xf(float * ori_ptr,int ori_len)
{
		if(ori_ptr==0||ori_len<=0) return false;
		int i=0;
		char tmp[50];
		//printf("\n");
		for(i=0;i<ori_len;i++)
		{
			sprintf(tmp,"tmp_buf[%d]=%6.2f;\n",i,ori_ptr[i]);
			log_str("C:\\DATA_INFO.txt",tmp);
			//printf("%s",tmp);
		}
		//printf("**ori_len=%d**\n",ori_len);
		//printf("\n");
		return true;
}

int CStrTool::CStrcpy(char * dest_str,char * src_str,int len)
{
	int ret_flag = 1,str_len=0;
	if(dest_str==0||src_str==0) return -1;
	if(len<(int)strlen(src_str)) str_len = len;
	else str_len = (int)strlen(src_str);
	memcpy(dest_str,src_str,str_len);
	dest_str[str_len] = '\0';
	return ret_flag;
}

//20091203;
float *** CStrTool::alloc_float_arr(int x,int y,int ** z)
{
	if(x<=0||y<=0||z==0) return 0;
	int i=0,j=0;
	float *** ptr = new float**[x];
	for(i=0;i<x;i++)
	{
		ptr[i] = new float*[y];
		for(j=0;j<y;j++)
		{
			ptr[i][j] = new float[z[i][j]];
			memset(ptr[i][j],0,z[i][j]*sizeof(float));
		}
	}
	return ptr;
}

////////////////////////////////////////////////////

/*bool CStrTool::del_float_arr(float *** ptr,int x,int y)
{
	if(x<=0||y<=0) return false;
	int i=0,j=0;
	if(ptr!=0)
	{
		for(i=0;i<x;i++)
		{
			for(j=0;j<y;j++)
			{
				if(ptr[i][j]!=0)
				{
					delete [] ptr[i][j];
					ptr[i][j] = 0;
				}
			}
			if(ptr[i]!=0)
			{
				delete [] ptr[i];
				ptr[i] = 0;
			}
		}
		delete [] ptr;
		ptr = 0;
	}
	return true;
}*/

int ** CStrTool::alloc_int_arr(int x,int y)
{
	if(x<=0||y<=0) return 0;
	int **ptr = new int*[x];
	int i=0;
	for(i=0;i<x;i++)
	{
		ptr[i] = new int[y];
		memset(ptr[i],0,sizeof(int)*y);
	}
	return ptr;
}

bool CStrTool::del_arr(int ** ptr,int x)
{
	if(ptr==0||x<=0) return false;
	int i=0;
	for(i=0;i<x;i++)
	{
		delete [] ptr[i];
		ptr[i] = 0;
	}
	delete [] ptr;
	ptr = 0;
	return true;
}

