
#pragma once


/************************************************************************/
/* 试验重要参数定义                                                     */
/************************************************************************/
#define MAX_EXAM_NUM                        16

#define MAX_EXAM_CHN_NUM                    64

//一个试验包含最大试验类别数量；
#define MAX_EXAM_TYPE_NUM                   16

//一个试验类别包含最大试验参数数量；
#define MAX_EXAM_PARA_NUM                   16

//支持最大数采设备；
#define MAX_DEV_NUM                         32

//数采设备最大支持通道数量
#define MAX_DEV_CHN_NUM                     128

//一个参数试验支持的最大键相；
#define MAX_PARA_KEY_NUM                     4

//一个参数试验支持的最大参考通道数量；
#define MAX_PARA_REF_NUM                     4

//总试验类别数；
#define EXAM_TYPE_NUM                        7

//负荷试验；
#define LOAD_BOOST                           1
//降负荷试验；
#define LOAD_DOWN                            2
//升压试验；
#define VOLTAGE_BOOST                        3
//降压试验；
#define VOLTAGE_DOWN                         4
//升转速试验；
#define RPM_BOOST                            5
//降转速试验；
#define RPM_DOWN                             6
//稳态试验；
#define STABLE_STATE                         7

//最大试验参数种类数；
#define EXAM_PARA_NUM                       50
//最大时间段（秒）
#define MAX_TIME_SPAN                   3600*6

/*********************************************/
//空闲状态；
#define NO_USED                              0
//使用状态；
#define IN_USED                              1
//阻塞状态；
#define IN_BLOCKED                           2
/*********************************************/

#define MAX_REC_SPAN   3600

/************************************************************************/
/*    通道类型；                                                        */
/************************************************************************/
//快变量；
#define Q_CHN_KEY      1
//慢变量；
#define S_CHN_KEY      2
//键相通道；
#define K_CHN_KEY      3



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////




#define FIRST_FREQ       1
#define FIRST_FREQ_VAL   2
#define SECOND_FREQ      3
#define SECOND_FREQ_VAL  4
#define THIRD_FREQ       5
#define THIRD_FREQ_VAL   6
#define PEAK_VAL         7
#define PEAK_98_VAL      8 
#define VALID_VAL        9
#define VALID_98_VAL     10 
#define AVG_VAL          11


static char * EXAM_TYPE_DEF[] = {"转速","升压","降压","升负荷","降负荷"};

static unsigned char EXAM_TYPE_NUM_NOW = 5;

static char * DATA_LIST_TITLE[] = {"序号","通道ID","通道名","主频","主频幅值","次频","次频幅值","季频","季频幅值","峰峰值","置信98％峰峰值","有效值","置信98％有效值","平均值"};

static int DATA_LIST_COL_WIDTH[] = {35,45,150,100,100,100,100,100,100,100,100,100,100,100};

static unsigned char DATA_LIST_TITLE_NUM = 14;

typedef struct tARROWSTRUCT {
	int nWidth;     // width
	float fTheta;   // angle
	bool bFill;     // flag
	//  filled
} ARROWSTRUCT;

const unsigned long tcCursor = 0; 
const unsigned long tcDragMarks = 1; 
const unsigned long tcAxisArrow = 2; 
const unsigned long tcDraw = 3; 
const unsigned long tcHints = 4; 
const unsigned long tcColorband = 5; 
const unsigned long tcColorLine = 6; 
const unsigned long tcRotate = 7; 
const unsigned long tcMarksTip = 8; 
const unsigned long tcChartImage = 9; 
const unsigned long tcAnnotate = 10; 
const unsigned long tcPageNumber = 11; 

static long B_COLORS [] =
{
	RGB (255,255,255),
	RGB (0,128,0),
	RGB (0,255,0),
	RGB (0,0,255),
	RGB (128,128,128),
	RGB (204,204,0),
	RGB (0,204,204),
	RGB (0,0,0),
	RGB (255,0,0),
	RGB (0,255,0),
};


static long S_COLORS [] =
{
	RGB (0,0,0),
	RGB (255,0,0),
	RGB (0,255,0),
	RGB (0,0,255),
	RGB (128,128,128),
	RGB (204,204,0),
	RGB (0,204,204),
	RGB (0,0,0),
	RGB (255,0,0),
	RGB (0,255,0),
	RGB (0,0,255),
	RGB (128,128,128),
	RGB (204,204,0),
	RGB (0,204,204),
	RGB (0,0,0),
	RGB (255,0,0),
	RGB (0,255,0),
	RGB (0,0,255),
	RGB (128,128,128),
	RGB (204,204,0),
	RGB (0,204,204),
	RGB (0,0,0),
	RGB (255,0,0),
	RGB (0,255,0),
	RGB (0,0,255),
	RGB (128,128,128),
	RGB (204,204,0),
	RGB (0,204,204),
	RGB (0,0,0),
	RGB (255,0,0),
	RGB (0,255,0),
	RGB (0,0,255),
	RGB (128,128,128),
	RGB (204,204,0),
	RGB (0,204,204),
	RGB (0,0,0),
	RGB (255,0,0),
	RGB (0,255,0),
	RGB (0,0,255),
	RGB (128,128,128),
	RGB (204,204,0),
	RGB (0,204,204),
};


//the maximum number of channels;
#define MAX_CHANNEL_NUM 200

//the frequency of sampling;
#define MAX_SAMPLE_FREQ 1024

//the time span of dealing;
#define DEAL_SPAN 1

#define MAX_AIN_NUM 1024

#define MAX_DIN_NUM 1024

#define MAX_SOE_NUM 1024

#define MAX_DIAG_NUM 64


#define	DEFAULT_FILENAME	NULL

#define	DEFAULT_MAPNAME		"_SHM_OBJ_"

#define	DEFAULT_MAPANA		"_SHM_ANA_"

#define	DEFAULT_MAPSIZE		(0xFFFF + 1)

#define DATA_LEN MAX_SAMPLE_FREQ*DEAL_SPAN



#define DB_SUCC_OP 1;

#define DB_FAIL_OP -1;

#define DB_FAIL_PARA -2;

#define DB_FAIL_INIT -3;

#define DB_FAIL_CON -4;


//Web communication protocol constants;

//functional code;
#define WEB_START_CODE 0x91

//functional code;
#define WEB_FUNC_ORI  0x1 

#define WEB_FUNC_ANA  0x2 

#define WEB_FUNC_PEAK  0x3

#define WEB_FUNC_VALID  0x4

#define WEB_FUNC_AVE  0x5

#define WEB_FUNC_MIN  0x6

#define WEB_FUNC_DEV  0x7

#define WEB_FUNC_STA  0x8

#define WEB_FUNC_ORIPEAK  0x13

#define WEB_FUNC_ORIVALID  0x14

#define WEB_FUNC_ORIAVE  0x15  

#define WEB_FUNC_ORIMIN  0x16

#define WEB_FUNC_ORIDEV  0x17

#define WEB_FUNC_ANAPEAK  0x23

#define WEB_FUNC_ANAVALID  0x24

#define WEB_FUNC_ANAAVE  0x25  

#define WEB_FUNC_ANAMIN  0x26

#define WEB_FUNC_ANADEV  0x27

//drive code;
#define WEB_DRV_BATCH  0x1

#define WEB_DRV_SINGLE  0x2

#define WEB_DRV_CHARACTER  0x3

#define WEB_DRV_HISWAVE  0x4

#define WEB_DRV_ACCWAVE  0x5

#define WEB_DRV_HISPROC  0x6

#define WEB_DRV_HISDIN  0x6


#define WEB_DRV_CHARACTER_MAX_REC  1000

#define WEB_DRV_HISWAVE_MAX_REC  1

#define WEB_DRV_ACCWAVE_MAX_REC  4

#define WEB_DRV_HISPROC_MAX_REC  1000

#define WEB_DRV_HISDIN_MAX_REC  1000



//data type code;
#define WEB_DATA_WAVE 0x1 

#define WEB_DATA_AIN 0x2

#define WEB_DATA_DIN 0x3

#define WEB_DATA_ALIVE 0x4

#define WEB_DATA_CHARACTER 0x5   

#define WEB_DATA_HISWAVE 0x6 

#define WEB_DATA_ACCWAVE 0x7

#define WEB_DATA_HISPROC 0x8 

#define WEB_DATA_HISDIN 0x9 

//2008-1-3 状态说明帧；
#define WEB_STATE_DATA 0x77

//2008-1-3 标识无数据帧；
#define WEB_NONE_DATA 0x5d

//2008-1-3 标识异常帧；
#define WEB_ABNORMAL_DATA 0x1c


#define WEB_CLIENT_ALIVE 2

//off line time;
#define WEB_OFFLINE_TIME 10

//wave data split upbound;
#define WAVE_DATA_SPLIT_NUM 8192

//ana data split upbound;
#define ANA_DATA_SPLIT_NUM 3072

//ain data split upbound;
#define AIN_DATA_SPLIT_NUM 1024

//din data split upbound;
#define DIN_DATA_SPLIT_NUM 1024

//th sleep span of send data;
#define A_SEND_SLEEP 1000

//FDR data deal span;
#define FDR_SPAN_TIME 150

//事故前最大数据文件数；
#define MAX_BEFORE_ACC_PAC 10

//事故后最大数据文件数；
#define MAX_AFTER_ACC_PAC 10



#define MAX_FLOAT_NUM 400000000

#define MIN_FLOAT_NUM -100000000


#define FDR_IMG_TIME 150

#define FDR_MID_LEN  2

//压缩还原数据两端边缘修正长度；
#define FDR_FRINGE_LEN  100

//最多SJ90数量；
#define MAX_SJ90_NUM 10

//最多键相数量；
#define MAX_KEY_NUM 50

//SJ90定时采样标识；
#define SJ90_TIME_SAM_FLAG 0

//SJ90等周期采样标识；
#define SJ90_CYC_SAM_FLAG 1

//一个键相最大点数；
#define KEY_MAX_POINT 700

#define MAX_SJ90_NUM 10

#define MAX_SET_NUM 10


//SJ90数据结构；
// CONSTANT
#define MAX_STR_LEN						300
#define MAX_NAME_LEN					64
#define PROC_NAME_LEN					16
#define	PAI								3.1415926535
#define SJ90_ERROR						-1
#define SJ90_OK							0

#define MAX_SPOT_NUM					24
#define MAX_CH_NUM						24
#define SAMPLE_LEN						1024
#define MAX_KEYPT_NUM					256
#define TSAMPLE_CYCLE					8

#define WM_ICON_NOTIFY WM_USER + 1


//峰峰值计算；
#define IDX_NUM_MAX 21

#define IDX_NUM_MIN 21

//历史波形数据采集密度；
#define WAVE_CHA_SPAN 60*1000

//波形定时采集密度；
#define WAVE_DATA_SPAN 300*1000

//一级峰峰值波形采集密度；(second)
#define PEAK_ALM1_SPAN 600

//二级峰峰值波形采集密度；(second)
#define PEAK_ALM2_SPAN 60

//一级有效值波形采集密度；(second)
#define VALID_ALM1_SPAN 600

//二级有效值波形采集密度；(second)
#define VALID_ALM2_SPAN 60

//模拟量数据采集密度；
#define PROC_CHA_SPAN 60*1000

//开关量数据采集密度；
#define DIN_CHA_SPAN 2*1000

//管理线程间隔时间；
#define MAN_SPAN 20*1000

//重联数据库间隔；
#define DB_CON_SPAN 30*1000

/*试验定义*/
#define UNIT_STR_LEN  16

#define FILE_NAME_LEN 64

#define CHN_NAME_LEN 128

//#define MAX_EXAM_NUM 100

//#define MAX_EXAM_CHN_NUM 50

#define SYS_CONF_SUFFIX "IDX"

#define SYS_CONF_DOT_SUFFIX "*.IDX"

#define EXAM_CONF_PATH "C:\\_EXAM_CONF.CFG"

#define MAIN_PANEL 2001;

//记录波形的显示周期数；
#define SHOW_CYC_NUM 20;

/*自定义消息*/
#define WM_FRESH_CURVE_MSG   WM_USER+101

#define WM_SHOW_CURVE_MSG    WM_USER+102

#define WM_SHOW_ANA_DATA     WM_USER+103

#define WM_SHOW_WATER_MSG    WM_USER+104

#define WM_BAN_CURVE_MSG     WM_USER+105

#define WM_BAN_FREQ_MSG      WM_USER+106

#define WM_BAN_PHASE_MSG     WM_USER+107

#define WM_PROMPT_CURVE_MSG  WM_USER+108

#define WM_CHA_CURVE_MSG     WM_USER+109


#define MAX_VAL              100000

#define MIN_VAL              -100000

//试验报告行数限制
#define MAX_RPT_LINE  200

#define MAX_SET_CHN_NUM  100

//图层定义；
#define VIEW_CURVE 81000

#define VIEW_WATERFALL 81001


#define MAX_CUV_NUM   20
#define MAX_CUV_POINT_NUM 1024*60
#define MAX_TIME_LEN    50
#define Q_SEND_SLEEP    1
#define PRINT_BUFF_LEN      128
#define MAX_SQL_STR_LEN     256*4
#define IP_STR_LEN          18
#define MAX_TAG_IP_LEN      32
#define MAX_TAB_NAME_LEN    64
#define MAX_ALIAS_NAME_LEN  64
#define MAX_REC_DEAL_NUM    256
#define MAX_COL_NUM         128
#define MAX_ENG_NAME_LEN    32
#define MAX_CHN_NAME_LEN    64
#define MAX_DESC_STR_LEN    128
#define MAX_HELP_STR_LEN    128
#define MAX_PATH_STR_LEN    128

#define MAX_SORT_LEN        60

#define MAX_DISPMSG_NUM     500