/******************************************************************************
模块名	： camera
文件名	： newcamera.h
相关文件： 
实现功能： 模块头文件，函数和变量的定义,对外接口声明 

  -------------------------------------------------------------------------------
  修改记录:
  日  期		版本		修改人		修改内容
  2005/09/14	3.0		    乔章池      增加类注释
******************************************************************************/
#ifndef _NEWCAMERA_H
#define _NEWCAMERA_H

#include "kdvtype.h"

// for Microsoft c++
#ifdef _MSC_VER

#ifndef API
#define API extern "C"			__declspec(dllexport)
#endif

// for gcc
#else
#ifndef API
#define API extern "C"
#endif

#endif	// _MSC_VER

#define CAM_TYPE_LEN            (u8)32    //协议名称长度
#define MAX_PROTOCOL_LEN		(u8)30    //协议的最大长度
#define MAX_CAM_PRTCL_NUM       (u8)4     //最大控制命令数量 
#define	MAX_CAM_CAP_NUM			(u8)40	  //最大摄像头能力集个数
#define	MAX_MATRIX_CAP_NUM		(u8)10	  //最大矩阵能力集个数
//库调用说明

//摄像头别名
#define CAM_NAME_SONY	                        "SONY"
#define CAM_NAME_AD								"AD"
#define CAM_NAME_PELCO							"PELCO-D"
#define CAM_NAME_PIH							"利凌"
#define CAM_NAME_PELCO_P						"PELCO-P"
#define CAM_NAME_PARCO							"PARCO柏高"
#define CAM_NAME_AB								"AB"
#define CAM_NAME_YUNTAI							"独立云台"
#define CAM_NAME_Samsung						"Samsung三星"
#define CAM_NAME_YAAN							"YUNTAI亚安"
#define CAM_NAME_AD168							"AD168"
#define CAM_NAME_KALATEL						"KALATEL卡拉特"
#define CAM_NAME_JN								"JN伟华"
#define CAM_NAME_WV_CS650						"松下CS650"
#define CAM_NAME_WV_CS850						"松下CS850"
#define CAM_NAME_ALEC							"ALEC艾立克"
#define CAM_NAME_AB_P							"AB-P"
#define CAM_NAME_AB_D							"AB-D"
#define CAM_NAME_PELCO_D_JEC					"JEC PELCO-D"
#define CAM_NAME_PELCO_P_JEC					"JEC PELCO-P"
#define CAM_NAME_TIANDY                         "天地伟业"
#define CAM_NAME_G100                           "G100"
#define CAM_NAME_368ZT                          "368ZT"
#define CAM_NAME_GaoDeTI                        "GaoDeTI"
#define CAM_NAME_DaliTI                         "DaliTI"

#define CAM_NAME_MATRIX							"通用矩阵协议"
#define CAM_NAME_ROBOT							"ROBOT 画面合成矩阵"

//摄像头的类型
enum TCamType{
        CAM_TYPE_BEGIN = 0,
	    CAM_TYPE_SONY,	        //	SONY
		CAM_TYPE_AD,			//	AD
		CAM_TYPE_PELCO,			//	PELCO-D
		CAM_TYPE_PIH,			//	利凌
		CAM_TYPE_PELCO_P,		//	PELCO-P
		CAM_TYPE_PARCO,			//	PARCO柏高
		CAM_TYPE_AB,			//	AB
		CAM_TYPE_YUNTAI,		//	独立云台
		CAM_TYPE_Samsung,		//	三星
		CAM_TYPE_YAAN,			//	亚安
		CAM_TYPE_AD168,			//	AD168类型
		CAM_TYPE_KALATEL,		//	卡拉特
		CAM_TYPE_JN,			//	JN伟华
		CAM_TYPE_WV_CS650,		//	松下,是850的老协议
		CAM_TYPE_WV_CS850,		//	松下,是850的新协议
		CAM_TYPE_ALEC,		    //	艾立克
		CAM_TYPE_AB_P,          //	AB-P
		CAM_TYPE_AB_D,          //	AB-D
		CAM_TYPE_PELCO_D_JEC,   //	JEC PELCO-D
		CAM_TYPE_PELCO_P_JEC,   //	JEC PELCO-P
        CAM_TYPE_TIANDY,        //  天地伟业
        CAM_TYPE_G100,          //  G100
        CAM_TYPE_368ZT,         //  368转台插件
        CAM_TYPE_GaoDeTI,       //  GaoDeTI红外插件
        CAM_TYPE_DaliTI,        //  DaliTI红外插件
		CAM_TYPE_2130_PELCO_P,  //  非标准PELCO_P协议
		CAM_TYPE_K_PELCO_D,
		CAM_TYPE_JVCTKC676,
		CAM_TYPE_INFINOVA_PELCO_D,
		CAM_TYPE_PEARMAIN_PELCO_D,
		CAM_TYPE_PEARMAIN_PELCO_P,
		CAM_TYPE_YAAN_PELCO_D,
		CAM_TYPE_CAMERA_NUM,

        CAM_TYPE_CAMERA_NOCTLR = 63,    //不可控设备
		CAM_TYPE_MATRIX=128,	//	矩阵协议
		MATRIX_TYPE_AB=128,		//  AB 矩阵
		CAM_TYPE_ROBOT,			//  ROBOT 画面合成矩阵
		MATRIX_TYPE_SUNNHO,		//  SUNNHO	矩阵
		MATRIX_TYPE_PEARMAIN,   //  红苹果  矩阵
		MATRIX_TYPE_AD,         //  AD 矩阵
		MATRIX_TYPE_INFORNOVA,
		MATRIX_TYPE_PECLO,      
		MATRIX_TYPE_HONEWELL,  //霍尼韦尔
		MATRIX_TYPE_TIANDI,
		MATRIX_TYPE_HUATONG,//华通矩阵
		MATRIX_TYPE_TONGCHUANG,//同创矩阵
		MATRIX_TYPE_BOCOM,     //博康矩阵
		MATRIX_TYPE_EXTRON,    //爱思创
		MATRIX_TYPE_ST_MS650,  //金三立
		MATRIX_TYPE_LIKEN,     //利肯
		MATRIX_TYPE_VINET,     //微耐特
		MATRIX_TYPE_WEPUS,     //威普士
		CAM_TYPE_MATRIX_NUM
};

//摄像头的控制命令
enum TCamCommand{
	CAM_COMMAND_MOVEUP=1,			//(u8)1		//向上移动
	CAM_COMMAND_MOVEDOWN,	    	//(u8)2		//向下移动
	CAM_COMMAND_MOVELEFT,			//(u8)3		//向左移动
	CAM_COMMAND_MOVERIGHT,			//(u8)4		//向右移动
	CAM_COMMAND_MOVEHOME,			//(u8)5		//回归
	CAM_COMMAND_MOVESTOP,			//(u8)6		//停止移动
	CAM_COMMAND_ZOOMTELE,		    //(u8)7		//拉近摄像头
	CAM_COMMAND_ZOOMWIDE,			//(u8)8		//拉远摄像头
	CAM_COMMAND_ZOOMSTOP,			//(u8)9		//视野调节停止
	CAM_COMMAND_FOCUSFAR,		    //(u8)10	//将焦距调远
	CAM_COMMAND_FOCUSNEAR,			//(u8)11	//将焦距调近
	CAM_COMMAND_FOCUSAUTO,			//(u8)12	//自动调焦
	CAM_COMMAND_FOCUSSTOP,			//(u8)13	//调焦停止
	CAM_COMMAND_PRESETSET,	    	//(u8)14	//摄象头预存
	CAM_COMMAND_PRESETCALL,			//(u8)15	//调摄象头预存
	CAM_COMMAND_CAMERASET,			//(u8)16	//初始化摄像头
	CAM_COMMAND_BRIGHTUP,			//(u8)17	//画面调亮
	CAM_COMMAND_BRIGHTDOWN,			//(u8)18	//画面调暗
	CAM_COMMAND_BRIGHTSTOP,			//(u8)19	//停止调亮
	CAM_COMMAND_NEW,				//(u8)20	//新命令
	CAM_COMMAND_APPEND,				//(u8)21	//附加命令
	CAM_COMMAND_GOTOPOINT,			//(u8)22	//中心定位
	CAM_COMMAND_ZOOMPART,			//(u8)23	//局部放大
	
	CAM_COMMAND_PANSPEED,			//(u8)24	//艾立克水平移动速度设置命令，（内部使用）
	CAM_COMMAND_TILTSPEED,			//(u8)25	//艾立克垂直移动速度设置命令，（内部使用）
	CAM_COMMAND_SETBRIGHT,			//(u8)26	//sony亮度调节启动命令（内部使用）
	CAM_COMMAND_MATRIX_TRANSIO,		//(u8)27
	CAM_COMMAND_MATRIX_SWTICH=27,	//(u8)27	//切换视频
	CAM_COMMAND_MATRIX_VIDMIX4,		//(u8)28	//画面合成2×2
	CAM_COMMAND_MATRIX_VIDMIX9,		//(u8)29	//画面合成3×3
	CAM_COMMAND_MATRIX_VIDMIX16,	//(u8)30	//画面合成4×4
	CAM_COMMAND_MATRIX_VIDMIXPP,	//(u8)31	//画面合成画中画

	CAM_COMMAND_ZOOMWHOLE,			//(u8)32    //局部缩小 

////菜单//////////////////////////////////////////////////////////////////////	
	CAM_COMMAND_MENU_UP,			//(u8)33	//菜单上
	CAM_COMMAND_MENU_DOWN,			//(u8)34	//菜单下
	CAM_COMMAND_MENU_LEFT,			//(u8)35	//菜单左
	CAM_COMMAND_MENU_RIGHT,			//(u8)36	//菜单右

	CAM_COMMAND_MENU_SHOW,			//(u8)37	//菜单显示
	CAM_COMMAND_MENU_HIDE,			//(u8)38	//菜单隐藏

	CAM_COMMAND_MENU_ENTER,			//(u8)39	//菜单确定
	CAM_COMMAND_MENU_EXIT,		    //(u8)40	//菜单取消

	CAM_COMMAND_PRESETDEL,			//(u8)41	//删除摄象头预存
////矩阵切换//////////////////////////////////////////////////////////////////////
	CAM_COMMAND_MATRIX_SWTICHASC,	//(u8)42	//正向切换
	CAM_COMMAND_MATRIX_SWTICHDESC,	//(u8)43	//反向切换
	CAM_COMMAND_MATRIX_IMAGEHOLD,	//(U8)44	//图像保持

	CAM_COMMAND_MOVELEFTUP,			//(u8)45	//向左上移动
	CAM_COMMAND_MOVELEFTDOWN,	    //(u8)46	//向左下移动
	CAM_COMMAND_MOVERIGHTUP,		//(u8)47	//向右上移动
	CAM_COMMAND_MOVERIGHTDOWN,		//(u8)48	//向右下移动

	CAM_COMMAND_WIDEDYNAMIC_ON=65,	//(u8)65	//宽动态开
	CAM_COMMAND_WIDEDYNAMIC_OFF,	//(u8)66	//宽动态关
////巡航//////////////////////////////////////////////////////////////////////	
	CAM_COMMAND_STARTAUTOSCAN=128,  //(u8)128	//开始自动巡航
	CAM_COMMAND_STOPAUTOSCAN,       //(u8)129	//停止自动巡航	
	CAM_COMMAND_STARTPRESETSCAN,	//(u8)130	//开始巡检
	CAM_COMMAND_STOPPRESETSCAN,     //(u8)131	//停止巡检
	CAM_COMMAND_SCANSPEED_SET,		//(u8)132	//水平扫描速度（影响自动扫描速度）bySpeed(1-40s)

	//add by Royan 20140305 添加雨刷功能
////雨刷//////////////////////////////////////////////////////////////////////
	CAM_COMMAND_STARTWIPER = 160,	//(u8)160	//开始雨刷，第一个参数为雨刷速度，值为0-15，其他参数置零
	CAM_COMMAND_STOPWIPER,			//(u8)161	//停止雨刷
	CAM_COMMAND_LIGHTON,            //(u8)162   //开启灯光
	CAM_COMMAND_LIGHTOFF,           //(u8)163   //关闭灯光
	CAM_COMMAND_START_EXTEND1,      //(u8)164   //扩展功能一（暂定开启喷水）
	CAM_COMMAND_STOP_EXTEND1,       //(u8)165   //关闭喷水
	CAM_COMMAND_START_EXTEND2,      //(u8)166   //扩展功能二（暂定开启除霜）
	CAM_COMMAND_STOP_EXTEND2,       //(u8)167   //关闭除霜
};

//#define MAX_TYPE_DISCRIBE		(u8)10    //最大协议长度
#define MAX_PAN_SPEED			(u8)15    //pan speed 0-MAX_PAN_SPEED
#define MAX_TILT_SPEED			(u8)15    //tilt speed 0-MAX_TILT_SPEED
#define MAX_ZOOM_SPEED			(u8)15    //zoom speed 0-MAX_ZOOM_SPEED
#define MAX_PREPOS_RANGE		(u8)16    //预置位号范围
#define MAX_ALEC_WIDE           (u8)254   //艾立克局部放大矩形最大宽度
#define MAX_ALEC_HIGH           (u8)254   //艾立克局部放大矩形最大高度
#define MIN_ALEC_WIDE           (u8)11    //艾立克局部放大矩形最小宽度
#define MIN_ALEC_HIGH           (u8)11    //艾立克局部放大矩形最小高度
#define MAX_MATRIX_VIDMIX_NUM   (u8)16    //矩阵最大画面合成输入数量


#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct tagCamInfo
{
	u8	m_byCamType;			//摄像头的类型，现在共有13种
	u16 m_wCamId;				//摄像头的地址
	u8	m_byCommandId;			//命令类型
    u8	m_byParam1;				//参数1，各种命令的参数含义不同,详见命令参数表
	u8	m_byParam2;				//参数2
    u8	m_byParam3;				//参数3，参数3，4为艾立克局部放大功能使用
	u8	m_byParam4;				//参数4
	u8	m_byCamCtrlWay;			//摄象头正反向控制方式
}
#ifndef WIN32
__attribute__((packed))
#endif
TCamInfo;

typedef struct tagMatrixInfo 
{
	u8 m_byCamType;    //矩阵类型
	u8 m_byCommandID;  //命令类型
	u8 m_byInputNum;  //选择的输入端口数量, camera
	//输入端口列表，当m_byCommID=Switch时，默认仅m_awInputList[0]有效
	u16 m_awInputList[MAX_MATRIX_VIDMIX_NUM];
	u8 m_byOutputPort;  //矩阵输出端口, monitor
}
#ifndef WIN32
__attribute__((packed))
#endif
TMatrixInfo;



typedef struct tagCamData
{
	u8	m_abyCamData[MAX_PROTOCOL_LEN];//协议内容
    s32 m_iNumOfData;			//协议的实际长度
	public:
		tagCamData()
		{
			memset(this, 0, sizeof(tagCamData));
		};
}
#ifndef WIN32
__attribute__((packed))
#endif
TCamData;

typedef struct tagCamPrtcl
{
	TCamData m_atCamPrtcl[MAX_CAM_PRTCL_NUM];	//协议数据
	u8 m_byInterval;            //步进时间
    u8 m_byNumPrtcl;			//协议数目
	public:
		tagCamPrtcl()
		{
			memset(this, 0, sizeof(tagCamPrtcl));
		};
}
#ifndef WIN32
__attribute__((packed))
#endif
TCamPrtcl;

//得到某一类型某一命令的摄像头协议
//当返回值为TRUE时得到正确的协议;
//当为FALSE时可能时无此摄像头类型或参数不对
//ptCamInfo已知参数，ptCamData返回协议，pbyInterval//控制类型=0起停,=0xff停止命令,（0,0xff）步进间隔
BOOL32 GetCamProtocol(const TCamInfo *ptCamInfo,TCamPrtcl *ptCamPrtcl);
BOOL32 GetCamProtocol(TMatrixInfo *ptMatrixInfo,TCamPrtcl *ptCamPrtcl);

/*命令参数表*/
/*
命令:CAM_COMMAND_MOVEUP,CAM_COMMAND_MOVEDOWN,CAM_COMMAND_MOVELEFT,
CAM_COMMAND_MOVERIGHT,CAM_COMMAND_MOVEHOME,CAM_COMMAND_MOVESTOP
参数:(m_byParam1,m_byParam2都有意义)
m_byParam1/范围:0-MAX_PAN_SPEED
m_byParam2/范围:0-MAX_TILT_SPEED

命令:CAM_COMMAND_ZOOMTELE,CAM_COMMAND_ZOOMWIDE
参数:(m_byParam1有意义)
m_byParam1/范围:0-MAX_ZOOM_SPEED

命令:CAM_COMMAND_ZOOMSTOP,CAM_COMMAND_FOCUSFAR,CAM_COMMAND_FOCUSNEAR,
CAM_COMMAND_FOCUSAUTO,CAM_COMMAND_FOCUSSTOP
参数:(m_byParam1,m_byParam2都无意义)

命令:CAM_COMMAND_PRESETSET,CAM_COMMAND_PRESETCALL
参数:(m_byParam1有意义)
m_byParam1/范围:1-16

命令:CAM_COMMAND_CAMERASET
参数:(m_byParam1,m_byParam2都有意义)
*/
struct TProtoclInfo
{
	u8 byID;  //类型id
	s8 ptachType[CAM_TYPE_LEN+1]; //类型名称
public:
	TProtoclInfo()
	{
		memset(this, 0, sizeof(TProtoclInfo));
	};
}
#ifndef WIN32
__attribute__((packed))
#endif
;

#ifdef WIN32
#pragma pack(pop)
#endif


/*====================================================================
函数名      : GetCamCapSet
功能        : 获取支持协议类型
输入参数说明: TProtoclInfo tCam[]
返回值说明  : 协议数目
====================================================================*/
API u8 GetCamCapSet(TProtoclInfo tCam[]);

/*====================================================================
函数名      : GetStrCamType
功能        : 获取字符串格式的协议类型
输入参数说明: dwCamType
返回值说明  : StrCamType
====================================================================*/
API LPCSTR GetStrCamType(u32 dwCamType);

/*====================================================================
函数名      : GetStrCamCmd
功能        : 获取字符串格式的协议命令
输入参数说明: dwCamCmd
返回值说明  : StrCamCmd
====================================================================*/
API LPCSTR GetStrCamCmd(u32 dwCamCmd);  //返回英文
API LPCSTR GetStrCommand(u8 byCmd);      //返回中文

/*====================================================================
函数名      : GetCamCapSet
功能        : 获取支持协议数目
返回值说明  : 协议数目
====================================================================*/
API u8 GetCamCapSetNum();
/*====================================================================
函数名      : GetMatrCapSet
功能        : 获取支持协议类型
输入参数说明: TProtoclInfo tMatr[]
返回值说明  : 协议数目
====================================================================*/
API u8 GetMatrCapSet(TProtoclInfo tMatr[]);

/*====================================================================
函数名      : GetCamCapSet
功能        : 获取支持协议数目
返回值说明  : 协议数目
====================================================================*/
API u8 GetMatrCapSetNum();

#ifdef _LINUX_
void CamRegHelpCmd(); 
#endif

#endif
