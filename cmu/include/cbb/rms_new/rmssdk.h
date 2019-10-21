#ifndef _RMSSDK_H_
#define _RMSSDK_H_

#include "kdvtype.h"

/************************************************************************/
/*          宏定义                                                      */
/************************************************************************/
#define RS_MAXLEN_UUID                   65
#define RS_MAXLEN_MANUFAC                32
#define RS_MAXLEN_NAME                   32
#define RS_MAXLEN_TYPE                   32
#define RS_MAXLEN_ADDRESS                39
#define RS_MAXLEN_ENCODINGNAME           8
#define RS_MAXLEN_TIMEZONE               64
#define RS_MAXLEN_TOKEN                  256
#define RS_MAXLEN_ERROR_DESCRIPTION      256
#define RS_MAXLEN_PARTITIONNAME          128
#define RS_MAXLEN_MOUNTNAME              64
#define RS_MAXLEN_PASSWORD               16
#define RS_MAXLEN_RECTAG                 128
#define RS_MAXNUM_MEDIASTREAM            3      
#define RS_MAXLEN_MEDIAINFOEXT           1024
#define RS_MAXLEN_FILEPATH               256
#define RS_MAXLEN_FILENAME               256
#define RS_MAXLEN_PUNCHKEY               64
#define RS_MAXLEN_FILEALIAS              128
#define RS_MAX_ADDR_NUM                  8

/************************************************************************/
/*          枚举定义                                                    */
/************************************************************************/

typedef enum
{
	//rmssdk的错误码
	ERS_OK    = 0,
	ERS_OPER_TIMEOUT,
	ERS_PARAM_INVALID,
	ERS_INNER_ERROR,
	ERS_SEQ_ERROR,
	ERS_NOTCREATE,
	ERS_NOTREG,
	ERS_NO_MEMORY,
	ERS_STRLEN,
	ERS_ERROR, //临时

	//rms以及nru的错误码
	ERS_NOT_IN_SERVICE = ERS_OK + 100,
	ERS_NOT_IN_TOPO, //101
	ERS_DEVICE_IS_NOT_ENABLE,//102
	ERS_DEVICE_ALREADY_ONLINE,//103
	ERS_REACH_MAXNUM_NRU,//104
	ERS_NO_SUITABLE_PARTITION,//105
	ERS_HAS_NO_REC_RESOURCE,//106
	ERS_REPEAT_ALLOC_RESOURCE,//107
	ERS_CANNOT_STOP_ALARM_RECORD,//108
	ERS_RECORD_FILE_IN_USE,//109
	ERS_RECORD_FILE_NOT_EXIST,//110
	ERS_PARTITION_OFFLINE,//111
	ERS_PLAYTASK_TOO_MUCH,//112
	ERS_PLAYTASK_NOTEXIST,//113
	ERS_DOWNLOADTASK_TOO_MUCH,//114
	ERS_DOWNLOADTASK_NOTEXIST,//115
	ERS_DATABASE_OPERATION_FAILED,//116
	ERS_DELETE_FILE_TIMEOUT,//117
	ERS_UDP_BANDWIDTH_OVERLOAD,//118
	ERS_RESOLV_CHANGED,//119
	ERS_NO_PLAY_RESOURCE,//120
	ERS_NO_DOWNLOAD_RESOURCE,//121
	ERS_GET_OUTADDR,//122
	ERS_POUNCH,//123
	ERS_PATH_CREATE_FAIL,//124
	ERS_STUN_TIMEOUT,//125
	ERS_PUNCH_TIMEOUT,//126
	ERS_FILE_FAIL,//127
	ERS_RPCTRL_NOT_INITIAL,//128
	ERS_NRU_NO_MEMORY,//129
	ERS_REC_STATUS_ERROR,//130
	ERS_RECTASK_TOO_MUCH,//131
	ERS_REC_OVER_CAPACITY,//132
	ERS_WAIT_KEYFRAME,//133
	ERS_TIMESTAMP_ERR,//134
	ERS_PREREC_TIME_INVALID,//135
	ERS_SMALL_FILE_INTERVAL_INVALID,//136
	ERS_ALREADY_ALARM_TWO,//137
	ERS_PLAY_OVER_CAPACITY,//138
	ERS_PLAY_DROG_TIME_INVALID,//139
	ERS_PLAY_GRAN_INVALID,//140
	ERS_PLAY_RECORD_LENGTH_ZERO,//141
	ERS_PLAY_REACH_STOP_TIME,//142
	ERS_DOWNLOAD_STATUS_ERROR,//143
	ERS_SOCKET_ERROR,//144
	ERS_CONNECT_ERROR,//145
	ERS_PATH_ALREADY_EXIST,//146
	ERS_PATH_NOT_EXIST,//147
	ERS_PATH_CREATE_FAIL_RPC,//148
	ERS_PATH_DELETE_FAIL,//149
	ERS_FILE_PATH_INVALID,//150
	ERS_FILE_ALREADY_EXIST,//151
	ERS_FILE_NOT_EXIST,//152
	ERS_FILE_DELETE_FAIL,//153
	ERS_FILE_RENAME_FAIL,//154
	ERS_FILE_NAME_INVALID,//155
	ERS_MEDIATYPE_UNSUPPORT,//156
	ERS_START_TIME_INVALID,//157
	ERS_RECLOG_FILE_NOT_EXIST,//158
	ERS_RPCMD_IS_BUSY,//159
	ERS_INODE_DESTROYED,//160
	ERS_DISC_TIMEOUT,//161
	ERS_FORMAT_CHANGE,//162
	
}ERS_ErrType;

typedef enum
{
	RS_TIMETYPE_LOCAL,
	RS_TIMETYPE_UTC,
}ERS_TimeType;

typedef enum
{
	RS_RECTYPE_ALARM = 1,            /*告警录像*/
	RS_RECTYPE_HANDLE,               /*手动录像*/
	RS_RECTYPE_PLAN,                 /*计划录像，包括定时录像和周期录像*/
	RS_RECTYPE_UNKNOWN,              
}ERS_RecType;

typedef enum
{
	RS_RECMODE_ALL,                   /*既录视频又录音频*/
	RS_RECMODE_ONLYVIDEO,             /*只录视频*/
	RS_RECMODE_ONLYKEYFRAME,          /*只录关键帧*/
	RS_RECMODE_UNKNOWN,               /*未知模式*/
}ERS_RecMode;  

typedef enum
{
	RS_RECSTATE_RECORD,               /*录像*/
	RS_RECSTATE_PRERECORD,            /*预录*/
	RS_RECSTATE_UNKNOWN,              /*未知状态*/
}ERS_RecState;

typedef enum
{
	RS_VCRCMD_START,                  /*开始放像*/
	RS_VCRCMD_STOP,                   /*停止放像*/
	RS_VCRCMD_PAUSE,                  /*暂停放像*/
	RS_VCRCMD_RESUME,                 /*恢复放像*/
	RS_VCRCMD_SEEK,                   /*时间定位*/
	RS_VCRCMD_FASTPLAY,               /*快放*/
	RS_VCRCMD_SLOWPLAY,               /*慢放*/
	RS_VCRCMD_ONEFRAME,               /*单帧播放*/
	RS_VCRCMD_KEYFRAME,               /*只放关键帧*/
	RS_VCRCMD_KEYSEEK,                /*时间定位(从前一个关键帧开始)*/
	RS_VCRCMD_REVERSEPLAY,            /*倒放*/
	RS_VCRCMD_FORWARDPLAY,            /*正放*/
	RS_VCRCMD_AUTO_KEYFRAME,          /*自动模式，是否抽帧由NRU确定：低倍速全帧，高倍速只放关键帧*/
	RS_VCRCMD_FORCE_KEYFRAME,         /*强制只放关键帧*/
	RS_VCRCMD_FORCE_ALLFRAME,         /*强制回放全帧*/
	RS_VCRCMD_UNKNOWN,
}ERS_VCRCmd;

typedef enum
{
	RS_IPTYPE_IPv4,
	RS_IPTYPE_IPv6,
	RS_IPTYPE_UNKNOWN,
}ERS_IPType;

typedef enum
{
	RS_TRANSPORTPROTOCOL_UDP,           /*UDP*/
	RS_TRANSPORTPROTOCOL_TCP,           /*TCP*/
	RS_TRANSPORTPROTOCOL_RTSP,          /*RTSP/TCP*/
	RS_TRANSPORTPROTOCOL_UNIXSOCKET,    /*域套接字*/
	RS_TRANSPORTPROTOCOL_UNKNOWN,
}ERS_TransportProtocol;

typedef enum
{
	RS_STREAMTYPE_RTP_UNICAST,
	RS_STREAMTYPE_RTP_MULTICAST,
}ERS_StreamType;

typedef enum
{
	RS_MEDIATYPE_AUDIO,
	RS_MEDIATYPE_VIDEO,
	RS_MEDIATYPE_DATA,
}ERS_MediaType;

typedef enum
{
	RS_COVERPOLICY_STOPRECORD,          /*通道空间满停录像*/
	RS_COVERPOLICY_COVERALL,            /*通道空间满覆盖所有录像*/
	RS_COVERPOLICY_COVERNORMAL,         /*通道空间满覆盖普通录像*/
	RS_COVERPOLICY_UNKNOWN,
}ERS_CoverPolicy;

typedef enum
{
	RS_PARTFORMAT_EXT3,
	RS_PARTFORMAT_VBFS,
	RS_PARTFORMAT_UNKNOWN,
}ERS_PartFormat;

typedef enum
{
	RS_PARTSTATUS_ONLINE,
	RS_PARTSTATUS_OFFLINE,
	RS_PARTSTATUS_ALARM,
	RS_PARTSTATUS_UNKNOWN,
}ERS_PartStatus;

typedef enum
{
	RS_REASON_LOAD_BALANCE = 1,
	RS_REASON_PARTITION_FULL,
	RS_REASON_BIGFILE_SWITCH_FAIL,
	RS_REASON_FORMAT_CHANGE,
	RS_REASON_NOSTREAM,
	RS_REASON_UNKNOWN,
}ERS_ReasonOfStopRecord;

typedef enum
{
	RS_CHANNELTYPE_RTP,
	RS_CHANNELTYPE_RTCP,
	RS_CHANNELTYPE_DATA,
}ERS_ChannelType;

typedef enum
{
	RS_OPERSTYLE_NORMAL,            /*正常操作*/
	RS_OPERSTYLE_CHECKOUT,          /*退网操作*/
	RS_OPERSTYLE_UNKNOWN,
}ERS_OperStyle;

typedef enum
{
	RS_ALARMTYPE_ALARM,            /*告警*/
	RS_ALARMTYPE_LIFTALARM,        /*解除告警*/
	RS_ALARMTYPE_UNKNOWN,
}ERS_AlarmType;

typedef enum
{
	RS_ALARMCODE_REACHTO_THRESHOLDOFRECORDBANDWIDTH = 1,   /*达到录像带宽的阈值*/
}ERS_AlarmCode;

typedef enum
{ 
	RS_OPERTYPE_NULL, 
	RS_OPERTYPE_GET, 
	RS_OPERTYPE_LOCK, 
	RS_OPERTYPE_UNLOCK,
}ERS_LockOperType;

//{{{{{{{{{{{{{{{ add by Royan 20140314 
typedef enum
{
	RS_REC_STATUS_UNLOCK,     /*未锁定*/
	RS_REC_STATUS_LOCK,       /*锁定*/
	RS_REC_STATUS_UNKNOWN,    /*未知*/
}ERS_RecLockStatus;
//}}}}}}}}}}}}}}} add by Royan 20140314

/************************************************************************/
/*          回调事件类型定义                                            */
/************************************************************************/
typedef enum
{
	RS_EVENT_BEGIN                    = 1000,
		
	RS_EVENT_RMS_ONLINE_NOTIFY					= RS_EVENT_BEGIN + 1,       /*TRS_DeviceBaseInfo*/
	RS_EVENT_RMS_OFFLINE_NOTIFY					= RS_EVENT_BEGIN + 2,       /*TRS_DevID*/

	/*搜索发现及入网相关消息*/
	RS_EVENT_DISCOVERY_NRU                      = RS_EVENT_BEGIN + 3,       /*u32(搜索到的Nru数量) + TRS_DiscoveryNruInfo 列表*/
	RS_EVENT_ADD_NRU                            = RS_EVENT_BEGIN + 4,       /*TRS_OperResult*/
	RS_EVENT_CHANGE_NRUINFO                     = RS_EVENT_BEGIN + 5,       /*TRS_OperResult*/
	RS_EVENT_DEL_NRU                            = RS_EVENT_BEGIN + 6,       /*TRS_OperResult*/
	RS_EVENT_MODIFY_NRU_NETWORK                 = RS_EVENT_BEGIN + 7,       /*TRS_OperResult*/
	RS_EVENT_GET_ALLNRUINFO                     = RS_EVENT_BEGIN + 8,       /*TRS_OperResult + (u32)(入网NRU设备数) + TRS_NruInfo 列表*/

	/*业务消息*/
	RS_EVENT_NRU_ONLINE_NOTIFY					= RS_EVENT_BEGIN + 20,       /*TRS_NruOnlineNotify*/
	RS_EVENT_NRU_OFFLINE_NOTIFY					= RS_EVENT_BEGIN + 21,       /*TRS_DevID(录像服务器编号)*/

	RS_EVENT_GET_NRUSYSSTATUS                   = RS_EVENT_BEGIN + 22,       /*TRS_OperResult + TRS_NruSysStatus*/

	RS_EVENT_PARTITIONINFO_UPDATE_NOTIFY        = RS_EVENT_BEGIN + 23,       /*TRS_DevID(录像服务器编号) + TRS_PartitionInfo*/
		
	RS_EVENT_SET_SYSPARAM						= RS_EVENT_BEGIN + 24,       /*TRS_OperResult*/
	RS_EVENT_QUERY_SYSPARAM						= RS_EVENT_BEGIN + 25,       /*TRS_OperResult + TRS_SysParam*/

	RS_EVENT_BINDING_PUCHN						= RS_EVENT_BEGIN + 26,       /*TRS_OperResult + TRS_PuChnBinding(失败的前端绑定关系)*/
	RS_EVENT_UNBINDING_PUCHN					= RS_EVENT_BEGIN + 27,       /*TRS_OperResult*/
	RS_EVENT_QUERY_PUCHN_BINDINGRELATIVE		= RS_EVENT_BEGIN + 28,       /*TRS_OperResult + TRS_PuChnBinding*/
		
	RS_EVENT_ALLOC_RECRESOURCE				    = RS_EVENT_BEGIN + 29,       /*TRS_OperResult + TRS_TaskHandle + TRS_MediaTranParam*/
	RS_EVENT_RELEASE_RECRESOURCE				= RS_EVENT_BEGIN + 30,       /*TRS_OperResult*/
	RS_EVENT_START_RECTASK						= RS_EVENT_BEGIN + 31,       /*TRS_OperResult*/
	RS_EVENT_STOP_RECTASK						= RS_EVENT_BEGIN + 32,       /*TRS_OperResult*/
	RS_EVENT_RECTASK_STOPPED_NOTIFY             = RS_EVENT_BEGIN + 33,       /*TRS_RecStopNotify*/
	RS_EVENT_UPDATE_RECTYPE						= RS_EVENT_BEGIN + 34,       /*TRS_OperResult*/
	RS_EVENT_QUERY_RECORD						= RS_EVENT_BEGIN + 35,       /*TRS_OperResult + TRS_RecTimeTable*/
	RS_EVENT_QUERY_RECORD_DAYS                  = RS_EVENT_BEGIN + 36,       /*TRS_OperResult + TRS_RecDays*/

	RS_EVENT_ALLOC_PLAYRESOURCE				    = RS_EVENT_BEGIN + 37,       /*TRS_OperResult + TRS_TaskHandle + TRS_TimeRange + TRS_MediaTranParam + TRS_MediaStreamDescrition*/
	RS_EVENT_RELEASE_PLAYRESOURCE				= RS_EVENT_BEGIN + 38,       /*TRS_OperResult*/
	RS_EVENT_START_PLAYTASK						= RS_EVENT_BEGIN + 39,       /*TRS_OperResult*/
	RS_EVENT_STOP_PLAYTASK						= RS_EVENT_BEGIN + 40,       /*TRS_OperResult*/
	RS_EVENT_VCR_CTRL							= RS_EVENT_BEGIN + 41,       /*TRS_OperResult*/
	RS_EVENT_PLAYPROG_NOTIFY                    = RS_EVENT_BEGIN + 42,       /*TRS_TaskHandle + TRS_PlayProg*/
	RS_EVENT_PLAYTASK_STOPPED_NOTIFY            = RS_EVENT_BEGIN + 43,       /*TRS_TaskHandle*/

	RS_EVENT_ALLOC_DOWNLOADRESOURCE				= RS_EVENT_BEGIN + 44,       /*TRS_OperResult + TRS_TaskHandle + TRS_MediaTranParam(下载) + TRS_DownloadFileInfo*/
	RS_EVENT_RELEASE_DOWNLOADRESOURCE			= RS_EVENT_BEGIN + 45,       /*TRS_OperResult*/
	RS_EVENT_START_DOWNLOADTASK					= RS_EVENT_BEGIN + 46,       /*TRS_OperResult*/
	RS_EVENT_STOP_DOWNLOADTASK					= RS_EVENT_BEGIN + 47,       /*TRS_OperResult*/

	/*录像标签*/
	RS_EVENT_ADD_RECTAG                         = RS_EVENT_BEGIN + 48,        /*TRS_OperResult + TRS_RecTag*/
	RS_EVENT_DEL_RECTAG                         = RS_EVENT_BEGIN + 49,        /*TRS_OperResult*/
	RS_EVENT_UPDATE_RECTAG                      = RS_EVENT_BEGIN + 50,        /*TRS_OperResult*/
	RS_EVENT_QUERY_RECTAG                       = RS_EVENT_BEGIN + 51,        /*TRS_OperResult + TRS_RecTags*/
		
	/*配置stun服务器地址*/
	RS_EVENT_SET_STUNADDR                       = RS_EVENT_BEGIN + 52,        /*TRS_OperResult*/ 

	/*设备告警*/
	RS_EVENT_DEVICE_ALARM                       = RS_EVENT_BEGIN + 53,        /*TRS_DeviceAlarm*/

	/*录像删除*/
	RS_EVENT_DEL_RECORD                         = RS_EVENT_BEGIN + 54,        /*TRS_OperResult*/

	/*文件锁定或者解锁操作*/
	RS_EVENT_REC_LOCK                           = RS_EVENT_BEGIN + 55,        /*TRS_OperResult + TRS_RecLockStat*/  //add by Royan 20140314

	/*设置录像别名*/
	RS_EVENT_SET_RECALIAS                       = RS_EVENT_BEGIN + 56,        /*TRS_OperResult*/

	RS_EVENT_UPDATE_OTHER_STORAGE_LICENSE       = RS_EVENT_BEGIN + 57,        /*TRS_OtherStorageLicense*/

	/*根据条件删除录像*/
	RS_EVENT_REC_DEL_BY_COND                    = RS_EVENT_BEGIN + 58,        /*TRS_OperResult*/

	RS_EVENT_END,
}ERS_EventType;

/************************************************************************/
/*          结构体定义                                                  */
/************************************************************************/

typedef struct tagRSSequence
{
	u32                m_dwSequenceNO;
}TRS_Sequence;

typedef struct tagRSToken
{
	u8                  m_achToken[RS_MAXLEN_TOKEN+1];
	u16                 m_wTokenLength;
}TRS_Token;

typedef struct tagRSOperResult
{
	ERS_ErrType   m_eOperResult;
	s8            m_szErrDesc[RS_MAXLEN_ERROR_DESCRIPTION+1];
}TRS_OperResult;

typedef struct tagRSOtherStorageLicense
{
	u32           m_dwOtherStorageLicense;//允许接入的外厂商容量（T）
}TRS_OtherStorageLicense;

//{{{{{{{{{{{{{{{ add by Royan 20140314 
typedef struct tagRSRecLockStat
{
	ERS_RecLockStatus  m_emRecLockStat;
}TRS_RecLockStat;
//}}}}}}}}}}}}}}} add by Royan 20140314

typedef struct tagRSEventHandle
{
	ERS_EventType m_eEventType;
	TRS_Sequence  m_tSequence;
}TRS_EventHandle;

typedef struct tagRSIPAddress
{
	ERS_IPType   m_eIpType;
	s8           m_szAddress[RS_MAXLEN_ADDRESS+1];
	u16          m_wPrefixLength;
}TRS_IPAddress;

typedef struct tagRSDevID
{
	s8                  m_szDevUUID[RS_MAXLEN_UUID+1];         /*设备的唯一标识*/
}TRS_DevID;

typedef struct tagRSDevChn
{
	TRS_DevID           m_tDevID;         /*设备编号*/
	s16                 m_wChannel;       /*设备通道*/
}TRS_DevChn;

typedef struct tagRSDeviceBaseInfo
{
	TRS_DevID          m_tDeviceID;
	s8                 m_szDeviceName[RS_MAXLEN_NAME+1];            /*设备名*/
	s8                 m_szDeviceType[RS_MAXLEN_TYPE+1];            /*设备类型:VS200,KDM2801E等*/
	s8                 m_szManuFactory[RS_MAXLEN_MANUFAC+1];        /*设备厂商*/
}TRS_DeviceBaseInfo;

typedef struct tagRSInitParam
{
 	TRS_DevID      m_tDevID;                          /*初始化SDK库的设备UUID*/
 	TRS_IPAddress  m_tRMSAddr;                        /*需要注册的RMS服务器的IP地址*/
 	u16            m_wRMSPort;                        /*需要注册的RMS服务器的端口*/
	u16            m_wDisconnChkTime;                 /*断链检测时长*/
	u8             m_wDisconnChkCnt;                  /*断链检测次数*/
	s8             m_szLogPath[RS_MAXLEN_FILEPATH+1]; /*日志路径:/var/log/rmssdklog*/
	u8             m_byMaxLogCount;                   /*日志个数*/
	u32            m_dwSizePerLog;                    /*单个日志大小*/
}TRS_InitParam;

typedef struct tagRSSysParam
{
	ERS_CoverPolicy  m_eSysCoverPolicy;         /*系统覆盖策略*/
}TRS_SysParam;

typedef struct tagRSNetworkInfo
{
	s8                   m_szIfName[RS_MAXLEN_NAME+1];
	TRS_IPAddress        m_tNruAddr;                         /*nru的IP地址*/
	TRS_IPAddress        m_tNruGateWay;                      /*nru的网关地址*/
}TRS_NetworkInfo;

/*nru搜索信息*/
typedef struct tagRSDiscoveryNruInfo
{
	TRS_DeviceBaseInfo   m_tNruBaseInfo;
	TRS_NetworkInfo      m_tNruNetworkInfo;                   /*nru的网络参数*/
	TRS_IPAddress        m_tRegisterSvrAddr;                  /*nru注册的服务器地址，服务器地址为空，则说明该nru处于空闲状态，可以进行分配*/
}TRS_DiscoveryNruInfo;

/*nru信息*/
typedef struct tagRSNruInfo
{
	TRS_DeviceBaseInfo   m_tNruBaseInfo;              /*需要入网的NRU设备信息*/
	TRS_IPAddress        m_tRegisterSvrAddr;          /*入网的RMS服务器地址*/
	BOOL32               m_bUsed;                     /*是否启用*/
}TRS_NruInfo;

typedef TRS_NruInfo  TRS_AddNruInfo;

/*修改nru的设备信息结构体*/
typedef struct tagRSChangeNruInfo
{
	TRS_DeviceBaseInfo   m_tNruBaseInfo;              /*NRU设备信息*/
	BOOL32               m_bUsed;                     /*是否启用*/
}TRS_ChangeNruInfo;

typedef struct  tagRSNetAddr
{
	TRS_IPAddress  m_tIPAddr;
	u16            m_wPort;
}TRS_NetAddr;

typedef struct  tagRSNetAddrList
{
	u16         m_wAddrNum;
	TRS_NetAddr m_tNetAddr[RS_MAX_ADDR_NUM];
}TRS_NetAddrList;

typedef struct  tagRSPunchKey
{
	s8          m_szPunchKey[RS_MAXLEN_PUNCHKEY+1];
	u8          m_byLength;
}TRS_PunchKey;

typedef struct  tagRSChannelAddr
{
	ERS_ChannelType  m_byChannelType;                /*rtp,rtcp,data*/
	TRS_PunchKey     m_tPunchKey;
	u16              m_wNetAddrNum;                  /*地址数量*/
	TRS_NetAddr*     m_ptNetAddrList;		         /*地址列表*/
}TRS_ChannelAddr;

typedef struct tagRSMediaAddr
{
	ERS_MediaType         m_byMediaType;					/*video,audio,data*/
	u16                   m_wChannelNum;                    /*通道数量*/	
	TRS_ChannelAddr*      m_ptChannelAddrList;              /*通道列表*/         
}TRS_MediaAddr;

typedef struct tagRSMediaTranParam
{
	ERS_StreamType            m_eStreamType;                   /*媒体流类型*/
	ERS_TransportProtocol     m_eTransportProtocol;            /*传输协议*/
	u16                       m_wAddrNum;                      /*地址数量*/
	TRS_MediaAddr*            m_ptMediaTransAddrList;          /*媒体传输地址*/
}TRS_MediaTranParam;

typedef struct tagRSDevChnInfo
{
	TRS_DeviceBaseInfo  m_tDeviceBaseInfo;
	s16                 m_wChannel;
	s8                  m_szChnName[RS_MAXLEN_NAME+1];
}TRS_DevChnInfo;

typedef struct tagRSPartitionInfo
{
	s8                 m_szPartitionName[RS_MAXLEN_PARTITIONNAME+1];     /*分区名称*/
	s8                 m_szPartitionMountName[RS_MAXLEN_MOUNTNAME+1];    /*分区的挂载名(uuid)*/
	u32                m_dwPartitionTotalSize;                           /*分区大小，单位：M*/
	u32                m_dwPartitionRemainSize;                          /*分区可用大小，单位：M*/
	ERS_PartFormat     m_ePartitionFormat;                               /*分区格式*/
	ERS_PartStatus     m_ePartitionStatus;                               /*分区状态*/
	u32                m_dwPartitionAbnormalTimes;                       /*分区异常次数*/
	u32                m_dwPartitionCurRecordNum;                        /*分区上当前录像数*/
	u32                m_dwPartitionRecLockSize;                         /*分区录像锁定空间大小，单位：M*/ //add by Royan 20140314
	u32                m_dwPartitionReservedSize;                        /*分区预留空间大小，单位：M*/ //add by Royan 20140314
}TRS_PartitionInfo;

typedef struct tagRSPartitionsInfo
{
	u8                  m_byPartitionNum;
	TRS_PartitionInfo*  m_ptPartitionInfoList;
}TRS_PartitionsInfo;

typedef struct tagRSNruOnlineNotify
{
	TRS_DeviceBaseInfo     m_tNruBaseInfo;
	TRS_PartitionsInfo     m_tPartitionsInfo;
	TRS_NetworkInfo        m_tNruIpAddress;
	u32                    m_dwNruMaxBandwidth;         /*nru的最大数据带宽，单位：Kb/s*/
}TRS_NruOnlineNotify;

typedef struct tagRSNruSysStatus
{
	u8                     m_byMemUsage;                 /*内存占用百分比，80，标识百分之八十*/
	u8                     m_byCpuUsage;                 /*CPU使用百分比*/
}TRS_NruSysStatus;

/*前端通道绑定到录像服务器的某个磁盘分区*/
typedef struct tagRSPuChnBinding
{
	TRS_DevID          m_tNruID;
	s8                 m_szPartitionMountName[RS_MAXLEN_MOUNTNAME+1];
	u16                m_wPuChnNum;
	TRS_DevChn*        m_ptPuChnList;
}TRS_PuChnBinding;

typedef struct tagRSPuChnUnBinding
{
	ERS_OperStyle      m_eOperStyle;
	u16                m_wPuChnNum;
	TRS_DevChn*        m_ptPuChnList;
}TRS_PuChnUnBinding;

typedef struct tagRSPuChnBindingQuery
{
	TRS_DevID          m_tNruID;
	s8                 m_szPartitionMountName[RS_MAXLEN_MOUNTNAME+1];  /*磁盘分区必须填写，否则无法查询*/
}TRS_PuChnBindingQuery;

typedef struct tagRSVideoResolution
{
	s32   m_nWidth;                                   /*图像宽*/
	s32   m_nHeight;                                  /*图像高*/
}TRS_VideoResolution;

typedef struct tagRSVideoEncParam
{
	u8					m_byPayloadType;               /*载荷类型，无效值：MEDIA_TYPE_NULL，定义见kdvdef.h*/
	s8                  m_szEncodingName[RS_MAXLEN_ENCODINGNAME];    /*编码名称 rfc3551*/
	TRS_VideoResolution m_tVideoResolution;            /*视频分辨率*/
	s32                 m_nVideoFrameRate;             /*码流帧率*/
	s32                 m_nVideoBitRate;               /*码流比特率, Kbps*/
	s32                 m_nVideoKeyFrameIntvl;         /*码流的关键帧间隔*/
	s32                 m_nClockRate;                  /*时间戳采样率， 取值范围：1000 or 90000*/
}TRS_VideoEncParam;

typedef struct tagRSAudioEncParam
{
	u8                  m_byPlayloadType;              /*载荷类型，无效值：MEDIA_TYPE_NULL，定义见kdvdef.h*/
	s8                  m_szEncodingName[RS_MAXLEN_ENCODINGNAME];   /*编码名称*/
	s32                 m_nSampleRate;                  /*真实采样率*/
	s32                 m_nClockRate;                   /*时间戳采样率, 取值范围：1000 or 真实采样率*/
	s32                 m_nChannelsNum;                 /*声道数*/
}TRS_AudioEncParam;

//媒体扩展信息
typedef struct tagRSMediaInfoExt
{
	u16                 m_wMediaInfoDataLen;                           /*媒体扩展信息实际长度*/
	u8                  m_abyMediaInfoData[RS_MAXLEN_MEDIAINFOEXT];    /*媒体扩展信息*/
}TRS_MediaInfoExt;

typedef struct tagRSRecordParam
{
	ERS_RecState            m_eRecState;              /*录像状态：预录 or 录像*/
	ERS_RecType             m_eRecType;               /*录像类型：告警录像 or 非告警录像*/
	ERS_RecMode             m_eRecMode;               /*录像模式：视频 or 音频频*/
	TRS_DevChn              m_tDevChn;
	BOOL32                  m_bBlindRec;              /*是否盲录，盲录即不解释码流，直接存储*/
	BOOL32                  m_bSupportResend;         /*是否支持重传*/
	TRS_VideoEncParam       m_tVideoEncParam;         /*视频通道编码参数配置*/
	TRS_AudioEncParam   *   m_ptAudioEncParam;        /*音频通道编码参数配置*/
	u32                 *   m_pdwPreRecTime;          /*预录时长*/
	TRS_MediaInfoExt    *   m_ptExtension;            /*扩展信息*/
}TRS_RecordParam;

//媒体流属性参数
typedef struct tagRSMediaStreamPropertyParam
{
	ERS_MediaType               m_eMediaType;      // 媒体类型
	union
	{
		TRS_VideoEncParam	    m_tVideoEncParam;    //视频编码参数
		TRS_AudioEncParam	    m_tAudioEncParam;	  //音频编码参数
	};
}TRS_MediaStreamPropertyParam;

//媒体流属性参数集
typedef struct tagRSMediaStreamPropertyParamSet
{
	u8                            m_byMediaStreamNum;
	TRS_MediaStreamPropertyParam  m_atMediaStreamPropertyParam[RS_MAXNUM_MEDIASTREAM];
}TRS_MediaStreamPropertyParamSet;

//媒体流描述
typedef struct tagRSMediaStreamDescription
{
	u32                              m_dwStreamStartTime;               //码流的起始时间
	TRS_MediaStreamPropertyParamSet  m_tMediaStreamPropertyParamSet;
	TRS_MediaInfoExt                 m_tMediaInfoExt;
}TRS_MediaStreamDescrition;

typedef struct tagRSRecordTypeUpdate
{
	ERS_RecType             m_eRecType;                   //新的录像类型
	u32                     m_dwRecTypeChangeTime;        //录像类型改变的时间点/*绝对时间*/
}TRS_RecordTypeUpdate;

typedef struct tagRSTimeRange
{
	u32         m_dwStartTime;                   /*绝对时间*/
	u32         m_dwEndTime;                     /*绝对时间*/
}TRS_TimeRange;

typedef struct tagRSRecTimeRange
{
	ERS_RecType        m_eRecType;               /*录像类型：手动录像、计划录像 or 告警录像*/
	TRS_TimeRange      m_tTimeRange;             /*录像时间范围*/
}TRS_RecTimeRange;

typedef struct tagRSRecDuration
{
	TRS_Token           m_tRecDurationToken;      /*录像时间段标识*/
	TRS_TimeRange       m_tTimeRange;             /*录像时间段*/
	u32                 m_dwRecSize;              /*录像大小，单位KB*/          //add by Royan 20140312 添加录像大小
	s8                  m_szRecAlias[RS_MAXLEN_FILEALIAS+1];           /*录像别名*/
	TRS_RecLockStat     m_tRecLockStat;           /*录像锁定状态*/  //add by Royan 20140314
	u16                 m_wRecTimeRangeNum;       /*子录像时间段数量*/
	TRS_RecTimeRange*   m_ptRecTimeRangeList;     /*子录像时间段信息*/
}TRS_RecDuration;

typedef struct tagRSRecTimeTable
{
	TRS_DevChn         m_tDevChn;                    /*设备通道号*/
	u16                m_wDurationNum;               /*有录像的时间段数量*/
	TRS_RecDuration*   m_ptRecDurationList;          /*有录像的时间段列表*/
}TRS_RecTimeTable;

typedef struct tagRSRecAliasSet
{
	TRS_Token         m_tRecDurationToken;                             /*录像时间段标识*/
	s8                m_szRecAlias[RS_MAXLEN_FILEALIAS+1];             /*录像别名*/
}TRS_RecAliasSet;

/*录像查询条件结构体*/
typedef struct tagRSRecQueryCond
{
	TRS_DevChn       m_tPuChn;
	ERS_RecType    * m_peRecType;
	TRS_TimeRange  * m_ptQueryTimeRange;
	ERS_RecLockStatus * m_pemRecLockStat;    //add by Royan 20140319
}TRS_RecQueryCond;

typedef struct tagRSRecDaysQueryCond
{
	u8              m_byQueryPuChnNum;               /*同时查询的前端通道数量，必须大于等于1*/
	TRS_DevChn    * m_ptPuChnList;                   /*同时查询的前端通道列表*/
	TRS_TimeRange   m_tQueryTimeRange;               /*查询的时间区间, 最长一个月*/
}TRS_RecDaysQueryCond;

typedef struct tagRSOnePuChnRecDays
{
	TRS_DevChn      m_tPuChn;
	u32             m_dwDayBitMask;                  /*有录像的天置为1*/
}TRS_OnePuChnRecDays;

typedef struct tagRSRecDays
{
	u8                     m_byPuChnNum;              /*前端数量*/
	TRS_OnePuChnRecDays*   m_ptRecDaysList;         /*各个前端通道的录像情况*/
	TRS_TimeRange          m_tQueryTimeRange;
}TRS_RecDays;

typedef struct tagRSPlayParam
{
	TRS_DevChn         m_tPuChn;
	TRS_Token          m_tRecDurationToken;  /*需要放像的录像时间段标识*/
	u32                m_dwPlayStartTime;    /*放像开始时间（绝对时间）*/
	u32                m_dwSSRC;             /*放像时必须使用的SSRC，0表示不指定放像SSRC*/
	u8                 m_byReversePlay;      /*正放or倒放*/
}TRS_PlayParam;

/*放像控制结构体*/
typedef struct tagRSVCRCtrl
{
	ERS_VCRCmd         m_eVCRCmd;
	s32                m_nPlayRate;         /*放像速率，正数为正反，负数为倒放*/
	s32                m_nSeekTime;         /*相对时间*/
}TRS_VCRCtrl;

typedef struct tagRSRecDownLoadParam
{
	TRS_DevChn       m_tPuChn;                      /*下载对象的设备编号*/
	TRS_Token        m_tRecDurationToken;           /*需要下载的录像时间段标识*/
	TRS_TimeRange    m_tDownloadTimeRange;          /*下载的录像时间段*/
}TRS_RecDownLoadParam;

typedef struct tagRSRecTask
{
	u32                m_dwRecTaskID;
	TRS_RecordParam    m_tRecordParam;
}TRS_RecTask;

typedef struct tagRSPlayTask
{
	u32                m_dwPlayTaskID;
	TRS_PlayParam      m_tPlayParam;
}TRS_PlayTask;

typedef struct tagRSPlayProg
{
	u32                m_dwCurPlayTime;                /*当前放像时间，单位：秒 绝对时间*/
	u32                m_dwTotalPlayTime;              /*放像总时长*/
	u32                m_dwStartPlayTime;              /*起始放像时间，单位：秒 绝对时间*/
	BOOL32             m_bPlayFinished;                /*放像是否结束*/
}TRS_PlayProg;

typedef struct tagRSDownloadTask
{
	u32                    m_dwDownloadTaskID;
	TRS_RecDownLoadParam   m_tDownloadParam;
}TRS_DownloadTask;

typedef struct tagRSTaskHandle
{
	u32                 m_dwTaskID;                /*任务编号*/
	TRS_Token           m_tResourceToken;          /*资源标识*/
}TRS_TaskHandle;

typedef struct tagRSRecStopNotify
{
	u32                      m_dwTaskID;
	TRS_DevChn               m_tPuChn;
	ERS_ReasonOfStopRecord   m_eStopReason;
}TRS_RecStopNotify;

typedef struct tagRSRecTagAdd
{
	TRS_DevChn               m_tPuChn;            /*前端通道*/
	u32                      m_dwCreateTime;      /*标签创建时间*/
	s8                       m_szRecTagDesc[RS_MAXLEN_RECTAG+1];    /*标签描述*/
}TRS_RecTagAdd;

typedef struct tagRSRecTagDel
{
	u32                      m_dwRecTagID;        /*需要删除的标签编号*/
}TRS_RecTagDel;

typedef struct tagRSRecTagQueryCond
{
	TRS_DevChn         *     m_ptDevChn;         /*需要查询的标签所属的前端通道*/
	TRS_TimeRange      *     m_ptTimeRange;      /*需要查询的时间段，最大72小时，如果没有给定查询时间段，默认查询当天的记录*/
	s8                       m_szRecTagDescKeyWord[RS_MAXLEN_RECTAG+1];    /*标签描述关键字*/
}TRS_RecTagQueryCond;

typedef struct tagRSRecTag
{
	u32                      m_dwRecTagID;           /*录像标签编号*/
	TRS_RecTagAdd            m_tRecTag;              /*录像标签*/
}TRS_RecTag;

typedef struct tagRSRecTags
{
	u32                      m_dwTotalRecTagNum;       /*录像标签总数*/
	TRS_RecTag*              m_ptRecTagList;           /*录像标签列表*/
}TRS_RecTags;

typedef struct tagRSRecTagUpdate
{
	u32                       m_dwRecTagID;                             /*标签编号*/
	s8                        m_szRecTagDesc[RS_MAXLEN_RECTAG+1];       /*修改后的标签描述*/
}TRS_RecTagUpdate;

typedef struct  tagRSDownloadFileInfo
{
	s8                        m_szDownloadFilePath[RS_MAXLEN_FILEPATH+1];
	s8                        m_szDownloadFileName[RS_MAXLEN_FILENAME];
	TRS_MediaInfoExt          m_tMediaInfoExt;
}TRS_DownloadFileInfo;

typedef struct tagRSDeviceAlarm
{
	TRS_DevID                 m_tDeviceID;        /*告警或者解除告警的设备编号*/
	ERS_AlarmType             m_eAlarmType;       /*告警类型*/
	ERS_AlarmCode             m_eAlarmCode;       /*告警码*/
}TRS_DeviceAlarm;

typedef struct tagRSRecLock
{
	TRS_TaskHandle *          m_ptTaskHandle;     /*对于正在进行的录像操作，该项必须不为空*/
	TRS_Token *               m_ptRecordToken;    /*对于已经存在的录像操作，该项必须不为空*/
	ERS_LockOperType          m_eLockOperType;
	u32                       m_dwLockDeadLine;
}TRS_RecLock;

/*录像删除条件结构体*/
typedef struct tagRSRecDelCond
{
	TRS_DevChn       m_tPuChn;
	TRS_TimeRange    m_tDelTimeRange;
}TRS_RecDelCond;

/************************************************************************/
/*          回调函数定义                                                */
/************************************************************************/
typedef void (*RSEventCB)(TRS_EventHandle tEventHandle, u8* pbyBuf, u32 dwBufLen, void* pvContext);

/************************************************************************/
/*          公共接口定义                                                */
/************************************************************************/

/*====================================================================
函数名      :RSInit
功能        :RS库的初始化
             1. 调用RSInit时，会向RMS请求连接，连接成功则初始化接口返回成功，
			    否则返回失败；
			 2. 在连接成功后，会自动启动注册过程，注册成功后，回调RMS上线通知，
			    上层只有在收到RMS上线的通知后，才可以进行后续的其他操作
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSInit(const TRS_InitParam *ptInitParam);

/*====================================================================
函数名      :RSUninit
功能        :RS库的反初始化
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSUninit();

/*====================================================================
函数名      :RSSetEventCallback
功能        :注册回调函数
             1. 调用RSInit成功后请立即调用该接口进行回调函数的注册；
			 2. 由于SDK库的所有接口为异步接口，因此所有接口的结果均通过
			    注册的回调函数返回；
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSSetEventCallback(RSEventCB pFunc, void* pvContext);

/*====================================================================
函数名      :RSNruDiscovery
功能        :搜索在线的所有NRU设备
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSNruDiscovery(TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSNruDevAdd
功能        :添加录像服务器
			 1. 如果搜索出来的录像服务器存在服务器IP地址，则说明该录像服务器已经被使用
			 2. 如果想添加已经在使用的录像服务器，请先将该录像服务器变为空闲状态
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSNruDevAdd(const TRS_AddNruInfo *ptNruInfo, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSNruDevChange
功能        :修改录像服务器
			 1. 根据设备编号修改设备信息，比如设备名、设备启停标识等
			 2. 设备编号不可改变
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSNruDevChange(const TRS_ChangeNruInfo *ptNruInfo, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSNruDevDel
功能        :删除录像服务器
			 1. 将一台录像服务器从平台中删除时，会影响到该nru上的录像，请谨慎操作
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSNruDevDel(const TRS_DevID *ptNruID, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSNruNetworkParamModify
功能        :修改录像服务器的网络参数
			 1. 修改了录像服务器的网络参数，需要重启NRU才可以生效
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSNruNetworkParamModify(const TRS_DevID *ptNruID, const TRS_NetworkInfo *ptNetworkParam, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSAllNruInfoGet
功能        :获取所有入网的Nru信息
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSAllNruInfoGet(TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSNruSysStatusGet
功能        :获取Nru的系统状态信息
算法实现    :
引用全局变量:
输入参数说明:

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSNruSysStatusGet(const TRS_DevID *ptNruID, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSSysParamSet
功能        :制定或者修改系统参数
             1. 建议在RSInit之后，设置系统参数 
算法实现    :
引用全局变量:
输入参数说明: const TRS_SysRecPlan *ptSysRecPlan          -- [IN] 录像计划
		      TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSSysParamSet(const TRS_SysParam *ptSysParam, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSSysParamQuery
功能        :查询系统参数
算法实现    :
引用全局变量:
输入参数说明:TRS_Sequence *ptSequence                -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSSysParamQuery(TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSPuChnBinding
功能        :将前端通道绑定到某个录像服务器的某个磁盘分区
			 1. 如果前端通道绑定了磁盘分区，那么其他的前端通道则不能在该
			    分区上面录像
算法实现    :
引用全局变量:
输入参数说明: const TRS_PuChnBinding *ptPuChnBinding      -- [IN] 前端通道绑定信息
		      TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSPuChnBinding(const TRS_PuChnBinding *ptPuChnBinding, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSPuChnUnBinding
功能        :解除前端通道到某个录像服务器的某个磁盘分区的绑定关系
算法实现    :
引用全局变量:
输入参数说明: const TRS_PuChnUnBinding *ptPuChnUnBinding  -- [IN] 需要解绑定的前端通道信息
		      TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSPuChnUnBinding(const TRS_PuChnUnBinding *ptPuChnUnBinding, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSPuChnBindingQuery
功能        :查询某个录像机的某个磁盘分区上绑定的前端通道信息
算法实现    :
引用全局变量:
输入参数说明: const TRS_PuChnBindingQuery *ptPuChnBindingQuery  -- [IN] 前端通道绑定信息
		      TRS_Sequence *ptSequence                          -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSPuChnBindingQuery(const TRS_PuChnBindingQuery *ptPuChnBindingQuery, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecResourceAlloc
功能        :录像资源分配
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecTask *ptRecTask       -- [IN] 录像任务参数
			  TRS_Sequence *ptSequence           -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecResourceAlloc(const TRS_RecTask *ptRecTask, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecResourceRelease
功能        :录像资源释放
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecordHandle *ptRecHandle         -- [IN] 需要释放的资源
			  TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecResourceRelease(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecTaskStart
功能        :开启录像任务
算法实现    :
引用全局变量:
输入参数说明: const TRS_TaskHandle *ptTaskHandle          -- [IN] 任务资源句柄
              TRS_MediaTranParam   *ptTranParam           -- [IN] 传输参数
			  TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecTaskStart(const TRS_TaskHandle *ptTaskHandle, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecTypeUpdate
功能        :更新录像类型，在以下情况需要使用：
			 1. 告警录像 -- 计划录像
			 2. 计划录像 -- 告警录像
			 3. 手动录像 -- 告警录像
			 4. 告警录像 -- 手动录像
			 5. 手动录像 -- 计划录像
			 6. 计划录像 -- 手动录像
             
算法实现    :
引用全局变量:
输入参数说明: const TRS_TaskHandle *ptTaskHandle          -- [IN]  资源句柄
              TRS_RecordTypeUpdate * ptRecordTypeUpdate   -- [IN]  录像类型更新信息
              TRS_Sequence *ptSequence                    -- [OUT] 操作序列号 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecTypeUpdate(const TRS_TaskHandle *ptTaskHandle, TRS_RecordTypeUpdate * ptRecordTypeUpdate, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecTaskStop
功能        :停止录像任务
算法实现    :
引用全局变量:
输入参数说明: const TRS_TaskHandle *ptTaskHandle  -- [IN]  资源句柄
              TRS_Sequence *ptSequence            -- [OUT] 操作序列号 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecTaskStop(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecordQuery
功能        :录像查询
算法实现    :
引用全局变量:
输入参数说明: 	 

  返回值说明  :操作成功返回ERS_OK,失败返回错误码
  ----------------------------------------------------------------------
  修改记录    :
  日  期      版本        修改人        修改内容
  2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecordQuery(const TRS_RecQueryCond *ptQueryCond, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecordDaysQuery
功能        :查询前端通道在某段时期内那些天有录像记录
算法实现    :
引用全局变量:
输入参数说明: 	 

  返回值说明  :操作成功返回ERS_OK,失败返回错误码
  ----------------------------------------------------------------------
  修改记录    :
  日  期      版本        修改人        修改内容
  2012/05/11	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecordDaysQuery(const TRS_RecDaysQueryCond *ptQueryCond, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecordAliasSet
功能        :设置录像文件的别名
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecAliasSet *ptRecAliasSet  -- 【IN】录像别名信息	 
              TRS_Sequence *ptSequence              -- 【OUT】操作序列号

  返回值说明  :操作成功返回ERS_OK,失败返回错误码
  ----------------------------------------------------------------------
  修改记录    :
  日  期      版本        修改人        修改内容
  2014/03/12	1.0		  林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecordAliasSet(const TRS_RecAliasSet *ptRecAliasSet, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSPlayResourceAlloc
功能        :放像资源分配
算法实现    :
引用全局变量:
输入参数说明: const TRS_PlayTask *ptPlayTask     -- [IN] 放像任务参数
			  TRS_MediaTranParam   *ptTranParam           -- [IN] 传输参数
			  TRS_Sequence *ptSequence           -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSPlayResourceAlloc(const TRS_PlayTask *ptPlayTask, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSPlayResourceRelease
功能        :放像资源释放
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecordHandle *ptRecHandle         -- [IN] 需要释放的资源
			  TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSPlayResourceRelease(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSPlayTaskStart
功能        :开启放像任务
算法实现    :
引用全局变量:
输入参数说明: const TRS_TaskHandle *ptTaskHandle          -- [IN] 任务资源句柄
			  TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSPlayTaskStart(const TRS_TaskHandle *ptTaskHandle, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSVCRCtrl
功能        :VCR控制
算法实现    :
引用全局变量:
输入参数说明: const TRS_TaskHandle *ptTaskHandle    -- [IN] 放像句柄
              const TRS_VCRCtrl *ptVcrCtrl          -- [IN] VCR控制命令	
			  TRS_Sequence *ptSequence 	            -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSVCRCtrl(const TRS_TaskHandle *ptTaskHandle, const TRS_VCRCtrl *ptVcrCtrl, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSPlayTaskStop
功能        :停止放像任务
算法实现    :
引用全局变量:
输入参数说明: const TRS_TaskHandle *ptTaskHandle  -- [IN]  资源句柄
              TRS_Sequence *ptSequence            -- [OUT] 操作序列号 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSPlayTaskStop(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSDownloadResourceAlloc
功能        :下载资源分配
算法实现    :
引用全局变量:
输入参数说明: const TRS_DownloadTask *ptDownloadTask     -- [IN] 下载任务参数
			  TRS_MediaTranParam   *ptTranParam          -- [IN] 传输参数
			  TRS_Sequence *ptSequence           -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSDownloadResourceAlloc(const TRS_DownloadTask *ptDownloadTask, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSDownloadResourceRelease
功能        :下载资源释放
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecordHandle *ptRecHandle         -- [IN] 需要释放的资源
			  TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSDownloadResourceRelease(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSPlayTaskStart
功能        :开启下载任务
算法实现    :
引用全局变量:
输入参数说明: const TRS_TaskHandle *ptTaskHandle          -- [IN] 任务资源句柄
			  TRS_Sequence *ptSequence                    -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSDownloadTaskStart(const TRS_TaskHandle *ptTaskHandle, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSDownloadTaskStop
功能        :停止下载任务
算法实现    :
引用全局变量:
输入参数说明: const TRS_TaskHandle *ptTaskHandle  -- [IN]  资源句柄
              TRS_Sequence *ptSequence            -- [OUT] 操作序列号 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/15	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSDownloadTaskStop(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);


/*====================================================================
函数名      :RSRecTagAdd
功能        :添加录像标签
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecTagAdd *ptRecTagAdd  -- [IN]  添加的录像标签信息
              TRS_Sequence *ptSequence          -- [OUT] 操作序列号 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/05/08	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecTagAdd(const TRS_RecTagAdd *ptRecTagAdd, TRS_Sequence *ptSequence);


/*====================================================================
函数名      :RSRecTagDel
功能        :删除录像标签
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecTagDel *ptRecTagDel  -- [IN]  需要删除的录像标签信息
              TRS_Sequence *ptSequence          -- [OUT] 操作序列号 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/05/08	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecTagDel(const TRS_RecTagDel *ptRecTagDel, TRS_Sequence *ptSequence);


/*====================================================================
函数名      :RSRecTagUpdate
功能        :修改录像标签（只能修改标签的描述信息）
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecTagUpdate *ptRecTagUpdate  -- [IN]  更新的录像标签信息
              TRS_Sequence *ptSequence                -- [OUT] 操作序列号 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/05/08	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecTagUpdate(const TRS_RecTagUpdate *ptRecTagUpdate, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecTagQuery
功能        :查询录像标签
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecTagQueryCond *ptRecTagQueryCond  -- [IN]  查询条件
              TRS_Sequence *ptSequence                      -- [OUT] 操作序列号 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/05/08	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecTagQuery(const TRS_RecTagQueryCond *ptRecTagQueryCond, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSEventDescriptionGetFromEventID
功能        :根据eventid查询消息描述信息
算法实现    :
引用全局变量:
输入参数说明: u32 dwEventID                              -- [IN]  eventid
              s8* szEventDescription                     -- [OUT] 描述信息 	 

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/09/08	1.0			顾肖康    	  创建
====================================================================*/
ERS_ErrType RSEventDescriptionGetFromEventID(u32 dwEventID, s8* szEventDescription);

/*====================================================================
函数名      :RSIsTaskHandleMatchDev
功能        :判断handle是否属于该设备
算法实现    :
引用全局变量:
输入参数说明: TRS_TaskHandle *ptHandle                      -- [IN]  handle
              TRS_DevID* ptDevId                            -- [IN] 设备编号 	 

返回值说明  :操作成功返回TRUE,失败返回FALSE
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/05/08	1.0			林莉芬    	  创建
====================================================================*/
BOOL32  RSIsTaskHandleMatchDev(TRS_TaskHandle *ptHandle, TRS_DevID* ptDevId);

/*====================================================================
函数名      :RSTokenContruct
功能        :构造token信息
算法实现    :
引用全局变量:
输入参数说明: TRS_DevChn* ptDevChn            -- [IN]  前端设备信息
              s8* szToken                     -- [IN，OUT] 描述信息 
			  u16 wTokenLength                -- [IN]token长度

返回值说明  :操作成功返回TRUE,失败返回FALSE
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/05/08	1.0			顾肖康    	  创建
====================================================================*/
BOOL32  RSTokenContruct(TRS_DevChn* ptDevChn, void* szToken, u16 wTokenLength);

/*====================================================================
函数名      :RSStunAddrSet
功能        配置stun服务器的ip信息，在连接rms成功之后需要调用本接口设置stun服务器的地址
算法实现    :
引用全局变量:
输入参数说明: TRS_NetAddr* ptStunAddr          -- [IN]  stun服务器地址 
			  TRS_Sequence *ptSequence         -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/09/08	1.0			顾肖康    	  创建
====================================================================*/
ERS_ErrType RSStunAddrSet(TRS_NetAddr* ptStunAddr, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecDel
功能         删除录像记录
算法实现    :
引用全局变量:
输入参数说明: const TRS_Token *ptRecordToken  -- [IN]  需要删除的文件Token 
			  TRS_Sequence *ptSequence        -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2014/02/27	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecDel(const TRS_Token *ptRecordToken, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecDelbyCond
功能        :根据条件删除录像记录
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecDelCond *ptRecDelCond  -- [IN]  删除的条件 
			  TRS_Sequence *ptSequence        -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2016/08/29	1.0			Royan    	  创建
====================================================================*/
ERS_ErrType RSRecDelbyCond(const TRS_RecDelCond *ptRecDelCond, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSRecLock
功能         录像锁定
算法实现    :
引用全局变量:
输入参数说明: const TRS_RecLock *ptRecLock  -- [IN]  锁定信息 
			  TRS_Sequence *ptSequence      -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2014/03/10	1.0			林莉芬    	  创建
====================================================================*/
ERS_ErrType RSRecLock(const TRS_RecLock * ptRecLock, TRS_Sequence *ptSequence);

/*====================================================================
函数名      :RSUpdateOtherStorageLicense
功能        :更新外厂商允许接入容量
算法实现    :
引用全局变量:
输入参数说明: const TRS_OtherStorageLicense *ptOtherStorageLicense  -- [IN]  外厂商允许接入容量（T） 
			  TRS_Sequence *ptSequence                -- [OUT] 操作序列号

返回值说明  :操作成功返回ERS_OK,失败返回错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2016/08/05	1.0			周鲁阳    	  创建
====================================================================*/
ERS_ErrType RSUpdateOtherStorageLicense(const TRS_OtherStorageLicense *ptOtherStorageLicense, TRS_Sequence *ptSequence);

#endif

