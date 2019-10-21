#ifndef _REC_RECOVER_H_
#define _REC_RECOVER_H_

#include "kdvtype.h"

/************************************************************************/
/*          错误码定义                                                  */
/************************************************************************/
typedef enum
{
	RRErrCode_OK = 0,
	RRErrCode_Error,
	RRErrCode_ERR_InvalidParam,
	RRErrCode_ERR_LoadDisk,
	RRErrCode_ERR_ReadDisk,
	RRErrCode_ERR_NotVbfs,
	RRErrCode_ERR_FsDestroyed,
	RRErrCode_ERR_NoMemeory,
}ERRErrCode;

/************************************************************************/
/*          宏定义                                                      */
/************************************************************************/
#define RR_MAXLEN_DISKNAME          64
#define RR_MAXLEN_FILEPATH          128
#define RR_MAXLEN_FILENAME          128
#define RR_MAXLEN_STREAMFORMAT      32
#define RR_MAXNUM_STREAM            2
#define RR_MAXLEN_IPADDR            64
#define RR_MAXLEN_MACHINEID			64
#define RR_MAXLEN_ZONENAME          32
#define RR_MAXLEN_USERNAME          32
#define RR_MAXLEN_PASSWD            16
#define RR_MAXLEN_MEDIAINFO         1024

/************************************************************************/
/*          结构体定义                                                  */
/************************************************************************/

typedef enum
{
	E_StorType_Normal = 1,
	E_StorType_Vbfs,   
	E_StorType_Cloud,
}ERR_StorType;

typedef enum
{
	E_DType_File = 1,         /*文件*/
	E_Dtype_Dir,              /*路径*/
}ERR_DType;

typedef enum
{
	E_RR_StreamType_Audio = 1,
	E_RR_StreamType_Video,
}ERR_StreamType;

typedef enum
{
	E_IpType_IPv4 = 1,
	E_IpType_IPv6,
}ERR_IpType;

typedef struct tagRRDirect
{
	ERR_DType  m_eRRDtype;
	s8         m_szDirect[RR_MAXLEN_FILEPATH+1];
}TRR_Direct;

typedef struct tagRRStreamInfo
{
	ERR_StreamType  m_eStreamType;
	u8              m_byStreamFormat;                            /*码流的编码格式*/
	s8              m_szFormatName[RR_MAXLEN_STREAMFORMAT+1];    /*码流的编码格式名称*/
}TRR_StreamInfo;

typedef struct tagRRExtInfo
{
	u16   m_wExtInfoLen;
	u8    m_byExtInfo[RR_MAXLEN_MEDIAINFO];
}TRR_ExtInfo;

typedef struct tagRRFileInfo
{
	u32        m_dwStartTime;           /*单位：秒*/
	u32        m_dwEndTime;             /*单位：秒*/
	u64        m_llFileSize;            /*单位：字节*/
	BOOL32     m_bLocked;               /*录像文件是否被锁定*/
	u8         m_byStreamCount;         /*录像文件中流的数量*/
	TRR_StreamInfo m_atStreamInfo[RR_MAXNUM_STREAM];
	TRR_ExtInfo    m_tExtInfo;
}TRR_FileInfo;

typedef struct tagRRIPAddr 
{
	ERR_IpType  m_eIpType;
	s8          m_szIpAddr[RR_MAXLEN_IPADDR+1];
}TRR_IPAddr;

typedef struct tagRRNetAddr
{
	TRR_IPAddr   m_tIpAddr;
	u16          m_wPort;
}TRR_NetAddr;

typedef struct tagCloudStorLoadParam
{
	u16          m_wNetAddrNum;                            /*需要连接的云地址数量*/
	TRR_NetAddr  *m_ptNetAddr;                             /*数组，需要连接的云地址信息*/
	s8           m_szMachineID[RR_MAXLEN_MACHINEID+1];     /*机器ID， 即UUID*/        
	s8           m_szZoneName[RR_MAXLEN_ZONENAME+1];       /*域名*/
	s8           m_szUserName[RR_MAXLEN_USERNAME+1];       /*连接云的用户名*/
	s8           m_szPasswd[RR_MAXLEN_PASSWD+1];           /*连接云的密码*/
	u32          m_dwTimeOut;                              /*连接超时时间*/
}TRR_CloudStorLoadParam;

typedef struct tagOtherStorLoadParam
{
	s8           m_szDiskName[RR_MAXLEN_DISKNAME+1];
}TRR_OtherStorLoadParam;

/*====================================================================
函数名      : RR_LoadStor
功能        : 加载存储
算法实现    : 
引用全局变量: 
输入参数说明:  [IN] ERR_StorType eStorType -- 需要加载的存储类型：ext3, vbfs or cloud
			   [IN] void* pvLoadParam -- 加载参数：
					云存储加载参数结构体 -- TRR_CloudStorLoadParam
					其他存储加载参数结构体 -- TRR_OtherStorLoadParam
返回值说明  :  0：成功， other：失败
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2015/01/29             linlifen         创建
====================================================================*/
ERRErrCode RR_LoadStor(const ERR_StorType eStorType, void* pvLoadParam);

/*====================================================================
函数名      : RR_UnloadStor
功能        : 卸载存储
算法实现    : 
引用全局变量: 
输入参数说明: 无
返回值说明  : 0：成功， other：失败
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2015/01/29             linlifen         创建
====================================================================*/
ERRErrCode RR_UnloadStor();

/*====================================================================
函数名      : RR_ScanDir
功能        : 扫描路径，注意，该接口不进行迭代扫描，即如果扫描路径为A,
			  而路径A下面存在路径B，且路径B还包含其他的路径，则只会返回
			  路径B，如果要获得路径B下面的其他路径，可以再次调用该接口
			  对路径B进行扫描，依次类推
算法实现    : 
引用全局变量: 
输入参数说明:  [IN] const s8* pszScanDirName -- 需要扫描的路径
			   [OUT] TRR_Direct ***DirList -- 扫描到的文件获取路径列表
			   [OUT] s32& nDirNum  -- 扫描到的文件或者路径的数量
返回值说明  :  0：成功， other：失败
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2015/01/29             linlifen         创建
====================================================================*/
ERRErrCode RR_ScanDir(const s8* pszScanDirName, TRR_Direct ***ppptDirList, s32& nDirNum);

/*====================================================================
函数名      : RR_GetFileInfo
功能        : 获取文件的信息
算法实现    : 
引用全局变量: 
输入参数说明:  [IN] const s8* pszFileName -- 文件名
			   [OUT] TRR_FileInfo* ptFileInfo -- 文件信息
返回值说明  :  0：成功， other：失败
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2015/01/29             linlifen         创建
====================================================================*/
ERRErrCode RR_GetFileInfo(const s8* pszFileName, TRR_FileInfo* ptFileInfo);

void RR_ReleaseDirect(TRR_Direct ***ppptDirList, const s32 nDirNum);

#endif

