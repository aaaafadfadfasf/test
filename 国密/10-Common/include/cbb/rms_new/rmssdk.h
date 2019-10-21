#ifndef _RMSSDK_H_
#define _RMSSDK_H_

#include "kdvtype.h"

/************************************************************************/
/*          �궨��                                                      */
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
/*          ö�ٶ���                                                    */
/************************************************************************/

typedef enum
{
	//rmssdk�Ĵ�����
	ERS_OK    = 0,
	ERS_OPER_TIMEOUT,
	ERS_PARAM_INVALID,
	ERS_INNER_ERROR,
	ERS_SEQ_ERROR,
	ERS_NOTCREATE,
	ERS_NOTREG,
	ERS_NO_MEMORY,
	ERS_STRLEN,
	ERS_ERROR, //��ʱ

	//rms�Լ�nru�Ĵ�����
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
	RS_RECTYPE_ALARM = 1,            /*�澯¼��*/
	RS_RECTYPE_HANDLE,               /*�ֶ�¼��*/
	RS_RECTYPE_PLAN,                 /*�ƻ�¼�񣬰�����ʱ¼�������¼��*/
	RS_RECTYPE_UNKNOWN,              
}ERS_RecType;

typedef enum
{
	RS_RECMODE_ALL,                   /*��¼��Ƶ��¼��Ƶ*/
	RS_RECMODE_ONLYVIDEO,             /*ֻ¼��Ƶ*/
	RS_RECMODE_ONLYKEYFRAME,          /*ֻ¼�ؼ�֡*/
	RS_RECMODE_UNKNOWN,               /*δ֪ģʽ*/
}ERS_RecMode;  

typedef enum
{
	RS_RECSTATE_RECORD,               /*¼��*/
	RS_RECSTATE_PRERECORD,            /*Ԥ¼*/
	RS_RECSTATE_UNKNOWN,              /*δ֪״̬*/
}ERS_RecState;

typedef enum
{
	RS_VCRCMD_START,                  /*��ʼ����*/
	RS_VCRCMD_STOP,                   /*ֹͣ����*/
	RS_VCRCMD_PAUSE,                  /*��ͣ����*/
	RS_VCRCMD_RESUME,                 /*�ָ�����*/
	RS_VCRCMD_SEEK,                   /*ʱ�䶨λ*/
	RS_VCRCMD_FASTPLAY,               /*���*/
	RS_VCRCMD_SLOWPLAY,               /*����*/
	RS_VCRCMD_ONEFRAME,               /*��֡����*/
	RS_VCRCMD_KEYFRAME,               /*ֻ�Źؼ�֡*/
	RS_VCRCMD_KEYSEEK,                /*ʱ�䶨λ(��ǰһ���ؼ�֡��ʼ)*/
	RS_VCRCMD_REVERSEPLAY,            /*����*/
	RS_VCRCMD_FORWARDPLAY,            /*����*/
	RS_VCRCMD_AUTO_KEYFRAME,          /*�Զ�ģʽ���Ƿ��֡��NRUȷ�����ͱ���ȫ֡���߱���ֻ�Źؼ�֡*/
	RS_VCRCMD_FORCE_KEYFRAME,         /*ǿ��ֻ�Źؼ�֡*/
	RS_VCRCMD_FORCE_ALLFRAME,         /*ǿ�ƻط�ȫ֡*/
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
	RS_TRANSPORTPROTOCOL_UNIXSOCKET,    /*���׽���*/
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
	RS_COVERPOLICY_STOPRECORD,          /*ͨ���ռ���ͣ¼��*/
	RS_COVERPOLICY_COVERALL,            /*ͨ���ռ�����������¼��*/
	RS_COVERPOLICY_COVERNORMAL,         /*ͨ���ռ���������ͨ¼��*/
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
	RS_OPERSTYLE_NORMAL,            /*��������*/
	RS_OPERSTYLE_CHECKOUT,          /*��������*/
	RS_OPERSTYLE_UNKNOWN,
}ERS_OperStyle;

typedef enum
{
	RS_ALARMTYPE_ALARM,            /*�澯*/
	RS_ALARMTYPE_LIFTALARM,        /*����澯*/
	RS_ALARMTYPE_UNKNOWN,
}ERS_AlarmType;

typedef enum
{
	RS_ALARMCODE_REACHTO_THRESHOLDOFRECORDBANDWIDTH = 1,   /*�ﵽ¼��������ֵ*/
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
	RS_REC_STATUS_UNLOCK,     /*δ����*/
	RS_REC_STATUS_LOCK,       /*����*/
	RS_REC_STATUS_UNKNOWN,    /*δ֪*/
}ERS_RecLockStatus;
//}}}}}}}}}}}}}}} add by Royan 20140314

/************************************************************************/
/*          �ص��¼����Ͷ���                                            */
/************************************************************************/
typedef enum
{
	RS_EVENT_BEGIN                    = 1000,
		
	RS_EVENT_RMS_ONLINE_NOTIFY					= RS_EVENT_BEGIN + 1,       /*TRS_DeviceBaseInfo*/
	RS_EVENT_RMS_OFFLINE_NOTIFY					= RS_EVENT_BEGIN + 2,       /*TRS_DevID*/

	/*�������ּ����������Ϣ*/
	RS_EVENT_DISCOVERY_NRU                      = RS_EVENT_BEGIN + 3,       /*u32(��������Nru����) + TRS_DiscoveryNruInfo �б�*/
	RS_EVENT_ADD_NRU                            = RS_EVENT_BEGIN + 4,       /*TRS_OperResult*/
	RS_EVENT_CHANGE_NRUINFO                     = RS_EVENT_BEGIN + 5,       /*TRS_OperResult*/
	RS_EVENT_DEL_NRU                            = RS_EVENT_BEGIN + 6,       /*TRS_OperResult*/
	RS_EVENT_MODIFY_NRU_NETWORK                 = RS_EVENT_BEGIN + 7,       /*TRS_OperResult*/
	RS_EVENT_GET_ALLNRUINFO                     = RS_EVENT_BEGIN + 8,       /*TRS_OperResult + (u32)(����NRU�豸��) + TRS_NruInfo �б�*/

	/*ҵ����Ϣ*/
	RS_EVENT_NRU_ONLINE_NOTIFY					= RS_EVENT_BEGIN + 20,       /*TRS_NruOnlineNotify*/
	RS_EVENT_NRU_OFFLINE_NOTIFY					= RS_EVENT_BEGIN + 21,       /*TRS_DevID(¼����������)*/

	RS_EVENT_GET_NRUSYSSTATUS                   = RS_EVENT_BEGIN + 22,       /*TRS_OperResult + TRS_NruSysStatus*/

	RS_EVENT_PARTITIONINFO_UPDATE_NOTIFY        = RS_EVENT_BEGIN + 23,       /*TRS_DevID(¼����������) + TRS_PartitionInfo*/
		
	RS_EVENT_SET_SYSPARAM						= RS_EVENT_BEGIN + 24,       /*TRS_OperResult*/
	RS_EVENT_QUERY_SYSPARAM						= RS_EVENT_BEGIN + 25,       /*TRS_OperResult + TRS_SysParam*/

	RS_EVENT_BINDING_PUCHN						= RS_EVENT_BEGIN + 26,       /*TRS_OperResult + TRS_PuChnBinding(ʧ�ܵ�ǰ�˰󶨹�ϵ)*/
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

	RS_EVENT_ALLOC_DOWNLOADRESOURCE				= RS_EVENT_BEGIN + 44,       /*TRS_OperResult + TRS_TaskHandle + TRS_MediaTranParam(����) + TRS_DownloadFileInfo*/
	RS_EVENT_RELEASE_DOWNLOADRESOURCE			= RS_EVENT_BEGIN + 45,       /*TRS_OperResult*/
	RS_EVENT_START_DOWNLOADTASK					= RS_EVENT_BEGIN + 46,       /*TRS_OperResult*/
	RS_EVENT_STOP_DOWNLOADTASK					= RS_EVENT_BEGIN + 47,       /*TRS_OperResult*/

	/*¼���ǩ*/
	RS_EVENT_ADD_RECTAG                         = RS_EVENT_BEGIN + 48,        /*TRS_OperResult + TRS_RecTag*/
	RS_EVENT_DEL_RECTAG                         = RS_EVENT_BEGIN + 49,        /*TRS_OperResult*/
	RS_EVENT_UPDATE_RECTAG                      = RS_EVENT_BEGIN + 50,        /*TRS_OperResult*/
	RS_EVENT_QUERY_RECTAG                       = RS_EVENT_BEGIN + 51,        /*TRS_OperResult + TRS_RecTags*/
		
	/*����stun��������ַ*/
	RS_EVENT_SET_STUNADDR                       = RS_EVENT_BEGIN + 52,        /*TRS_OperResult*/ 

	/*�豸�澯*/
	RS_EVENT_DEVICE_ALARM                       = RS_EVENT_BEGIN + 53,        /*TRS_DeviceAlarm*/

	/*¼��ɾ��*/
	RS_EVENT_DEL_RECORD                         = RS_EVENT_BEGIN + 54,        /*TRS_OperResult*/

	/*�ļ��������߽�������*/
	RS_EVENT_REC_LOCK                           = RS_EVENT_BEGIN + 55,        /*TRS_OperResult + TRS_RecLockStat*/  //add by Royan 20140314

	/*����¼�����*/
	RS_EVENT_SET_RECALIAS                       = RS_EVENT_BEGIN + 56,        /*TRS_OperResult*/

	RS_EVENT_UPDATE_OTHER_STORAGE_LICENSE       = RS_EVENT_BEGIN + 57,        /*TRS_OtherStorageLicense*/

	/*��������ɾ��¼��*/
	RS_EVENT_REC_DEL_BY_COND                    = RS_EVENT_BEGIN + 58,        /*TRS_OperResult*/

	RS_EVENT_END,
}ERS_EventType;

/************************************************************************/
/*          �ṹ�嶨��                                                  */
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
	u32           m_dwOtherStorageLicense;//���������⳧��������T��
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
	s8                  m_szDevUUID[RS_MAXLEN_UUID+1];         /*�豸��Ψһ��ʶ*/
}TRS_DevID;

typedef struct tagRSDevChn
{
	TRS_DevID           m_tDevID;         /*�豸���*/
	s16                 m_wChannel;       /*�豸ͨ��*/
}TRS_DevChn;

typedef struct tagRSDeviceBaseInfo
{
	TRS_DevID          m_tDeviceID;
	s8                 m_szDeviceName[RS_MAXLEN_NAME+1];            /*�豸��*/
	s8                 m_szDeviceType[RS_MAXLEN_TYPE+1];            /*�豸����:VS200,KDM2801E��*/
	s8                 m_szManuFactory[RS_MAXLEN_MANUFAC+1];        /*�豸����*/
}TRS_DeviceBaseInfo;

typedef struct tagRSInitParam
{
 	TRS_DevID      m_tDevID;                          /*��ʼ��SDK����豸UUID*/
 	TRS_IPAddress  m_tRMSAddr;                        /*��Ҫע���RMS��������IP��ַ*/
 	u16            m_wRMSPort;                        /*��Ҫע���RMS�������Ķ˿�*/
	u16            m_wDisconnChkTime;                 /*�������ʱ��*/
	u8             m_wDisconnChkCnt;                  /*����������*/
	s8             m_szLogPath[RS_MAXLEN_FILEPATH+1]; /*��־·��:/var/log/rmssdklog*/
	u8             m_byMaxLogCount;                   /*��־����*/
	u32            m_dwSizePerLog;                    /*������־��С*/
}TRS_InitParam;

typedef struct tagRSSysParam
{
	ERS_CoverPolicy  m_eSysCoverPolicy;         /*ϵͳ���ǲ���*/
}TRS_SysParam;

typedef struct tagRSNetworkInfo
{
	s8                   m_szIfName[RS_MAXLEN_NAME+1];
	TRS_IPAddress        m_tNruAddr;                         /*nru��IP��ַ*/
	TRS_IPAddress        m_tNruGateWay;                      /*nru�����ص�ַ*/
}TRS_NetworkInfo;

/*nru������Ϣ*/
typedef struct tagRSDiscoveryNruInfo
{
	TRS_DeviceBaseInfo   m_tNruBaseInfo;
	TRS_NetworkInfo      m_tNruNetworkInfo;                   /*nru���������*/
	TRS_IPAddress        m_tRegisterSvrAddr;                  /*nruע��ķ�������ַ����������ַΪ�գ���˵����nru���ڿ���״̬�����Խ��з���*/
}TRS_DiscoveryNruInfo;

/*nru��Ϣ*/
typedef struct tagRSNruInfo
{
	TRS_DeviceBaseInfo   m_tNruBaseInfo;              /*��Ҫ������NRU�豸��Ϣ*/
	TRS_IPAddress        m_tRegisterSvrAddr;          /*������RMS��������ַ*/
	BOOL32               m_bUsed;                     /*�Ƿ�����*/
}TRS_NruInfo;

typedef TRS_NruInfo  TRS_AddNruInfo;

/*�޸�nru���豸��Ϣ�ṹ��*/
typedef struct tagRSChangeNruInfo
{
	TRS_DeviceBaseInfo   m_tNruBaseInfo;              /*NRU�豸��Ϣ*/
	BOOL32               m_bUsed;                     /*�Ƿ�����*/
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
	u16              m_wNetAddrNum;                  /*��ַ����*/
	TRS_NetAddr*     m_ptNetAddrList;		         /*��ַ�б�*/
}TRS_ChannelAddr;

typedef struct tagRSMediaAddr
{
	ERS_MediaType         m_byMediaType;					/*video,audio,data*/
	u16                   m_wChannelNum;                    /*ͨ������*/	
	TRS_ChannelAddr*      m_ptChannelAddrList;              /*ͨ���б�*/         
}TRS_MediaAddr;

typedef struct tagRSMediaTranParam
{
	ERS_StreamType            m_eStreamType;                   /*ý��������*/
	ERS_TransportProtocol     m_eTransportProtocol;            /*����Э��*/
	u16                       m_wAddrNum;                      /*��ַ����*/
	TRS_MediaAddr*            m_ptMediaTransAddrList;          /*ý�崫���ַ*/
}TRS_MediaTranParam;

typedef struct tagRSDevChnInfo
{
	TRS_DeviceBaseInfo  m_tDeviceBaseInfo;
	s16                 m_wChannel;
	s8                  m_szChnName[RS_MAXLEN_NAME+1];
}TRS_DevChnInfo;

typedef struct tagRSPartitionInfo
{
	s8                 m_szPartitionName[RS_MAXLEN_PARTITIONNAME+1];     /*��������*/
	s8                 m_szPartitionMountName[RS_MAXLEN_MOUNTNAME+1];    /*�����Ĺ�����(uuid)*/
	u32                m_dwPartitionTotalSize;                           /*������С����λ��M*/
	u32                m_dwPartitionRemainSize;                          /*�������ô�С����λ��M*/
	ERS_PartFormat     m_ePartitionFormat;                               /*������ʽ*/
	ERS_PartStatus     m_ePartitionStatus;                               /*����״̬*/
	u32                m_dwPartitionAbnormalTimes;                       /*�����쳣����*/
	u32                m_dwPartitionCurRecordNum;                        /*�����ϵ�ǰ¼����*/
	u32                m_dwPartitionRecLockSize;                         /*����¼�������ռ��С����λ��M*/ //add by Royan 20140314
	u32                m_dwPartitionReservedSize;                        /*����Ԥ���ռ��С����λ��M*/ //add by Royan 20140314
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
	u32                    m_dwNruMaxBandwidth;         /*nru��������ݴ�����λ��Kb/s*/
}TRS_NruOnlineNotify;

typedef struct tagRSNruSysStatus
{
	u8                     m_byMemUsage;                 /*�ڴ�ռ�ðٷֱȣ�80����ʶ�ٷ�֮��ʮ*/
	u8                     m_byCpuUsage;                 /*CPUʹ�ðٷֱ�*/
}TRS_NruSysStatus;

/*ǰ��ͨ���󶨵�¼���������ĳ�����̷���*/
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
	s8                 m_szPartitionMountName[RS_MAXLEN_MOUNTNAME+1];  /*���̷���������д�������޷���ѯ*/
}TRS_PuChnBindingQuery;

typedef struct tagRSVideoResolution
{
	s32   m_nWidth;                                   /*ͼ���*/
	s32   m_nHeight;                                  /*ͼ���*/
}TRS_VideoResolution;

typedef struct tagRSVideoEncParam
{
	u8					m_byPayloadType;               /*�غ����ͣ���Чֵ��MEDIA_TYPE_NULL�������kdvdef.h*/
	s8                  m_szEncodingName[RS_MAXLEN_ENCODINGNAME];    /*�������� rfc3551*/
	TRS_VideoResolution m_tVideoResolution;            /*��Ƶ�ֱ���*/
	s32                 m_nVideoFrameRate;             /*����֡��*/
	s32                 m_nVideoBitRate;               /*����������, Kbps*/
	s32                 m_nVideoKeyFrameIntvl;         /*�����Ĺؼ�֡���*/
	s32                 m_nClockRate;                  /*ʱ��������ʣ� ȡֵ��Χ��1000 or 90000*/
}TRS_VideoEncParam;

typedef struct tagRSAudioEncParam
{
	u8                  m_byPlayloadType;              /*�غ����ͣ���Чֵ��MEDIA_TYPE_NULL�������kdvdef.h*/
	s8                  m_szEncodingName[RS_MAXLEN_ENCODINGNAME];   /*��������*/
	s32                 m_nSampleRate;                  /*��ʵ������*/
	s32                 m_nClockRate;                   /*ʱ���������, ȡֵ��Χ��1000 or ��ʵ������*/
	s32                 m_nChannelsNum;                 /*������*/
}TRS_AudioEncParam;

//ý����չ��Ϣ
typedef struct tagRSMediaInfoExt
{
	u16                 m_wMediaInfoDataLen;                           /*ý����չ��Ϣʵ�ʳ���*/
	u8                  m_abyMediaInfoData[RS_MAXLEN_MEDIAINFOEXT];    /*ý����չ��Ϣ*/
}TRS_MediaInfoExt;

typedef struct tagRSRecordParam
{
	ERS_RecState            m_eRecState;              /*¼��״̬��Ԥ¼ or ¼��*/
	ERS_RecType             m_eRecType;               /*¼�����ͣ��澯¼�� or �Ǹ澯¼��*/
	ERS_RecMode             m_eRecMode;               /*¼��ģʽ����Ƶ or ��ƵƵ*/
	TRS_DevChn              m_tDevChn;
	BOOL32                  m_bBlindRec;              /*�Ƿ�ä¼��ä¼��������������ֱ�Ӵ洢*/
	BOOL32                  m_bSupportResend;         /*�Ƿ�֧���ش�*/
	TRS_VideoEncParam       m_tVideoEncParam;         /*��Ƶͨ�������������*/
	TRS_AudioEncParam   *   m_ptAudioEncParam;        /*��Ƶͨ�������������*/
	u32                 *   m_pdwPreRecTime;          /*Ԥ¼ʱ��*/
	TRS_MediaInfoExt    *   m_ptExtension;            /*��չ��Ϣ*/
}TRS_RecordParam;

//ý�������Բ���
typedef struct tagRSMediaStreamPropertyParam
{
	ERS_MediaType               m_eMediaType;      // ý������
	union
	{
		TRS_VideoEncParam	    m_tVideoEncParam;    //��Ƶ�������
		TRS_AudioEncParam	    m_tAudioEncParam;	  //��Ƶ�������
	};
}TRS_MediaStreamPropertyParam;

//ý�������Բ�����
typedef struct tagRSMediaStreamPropertyParamSet
{
	u8                            m_byMediaStreamNum;
	TRS_MediaStreamPropertyParam  m_atMediaStreamPropertyParam[RS_MAXNUM_MEDIASTREAM];
}TRS_MediaStreamPropertyParamSet;

//ý��������
typedef struct tagRSMediaStreamDescription
{
	u32                              m_dwStreamStartTime;               //��������ʼʱ��
	TRS_MediaStreamPropertyParamSet  m_tMediaStreamPropertyParamSet;
	TRS_MediaInfoExt                 m_tMediaInfoExt;
}TRS_MediaStreamDescrition;

typedef struct tagRSRecordTypeUpdate
{
	ERS_RecType             m_eRecType;                   //�µ�¼������
	u32                     m_dwRecTypeChangeTime;        //¼�����͸ı��ʱ���/*����ʱ��*/
}TRS_RecordTypeUpdate;

typedef struct tagRSTimeRange
{
	u32         m_dwStartTime;                   /*����ʱ��*/
	u32         m_dwEndTime;                     /*����ʱ��*/
}TRS_TimeRange;

typedef struct tagRSRecTimeRange
{
	ERS_RecType        m_eRecType;               /*¼�����ͣ��ֶ�¼�񡢼ƻ�¼�� or �澯¼��*/
	TRS_TimeRange      m_tTimeRange;             /*¼��ʱ�䷶Χ*/
}TRS_RecTimeRange;

typedef struct tagRSRecDuration
{
	TRS_Token           m_tRecDurationToken;      /*¼��ʱ��α�ʶ*/
	TRS_TimeRange       m_tTimeRange;             /*¼��ʱ���*/
	u32                 m_dwRecSize;              /*¼���С����λKB*/          //add by Royan 20140312 ���¼���С
	s8                  m_szRecAlias[RS_MAXLEN_FILEALIAS+1];           /*¼�����*/
	TRS_RecLockStat     m_tRecLockStat;           /*¼������״̬*/  //add by Royan 20140314
	u16                 m_wRecTimeRangeNum;       /*��¼��ʱ�������*/
	TRS_RecTimeRange*   m_ptRecTimeRangeList;     /*��¼��ʱ�����Ϣ*/
}TRS_RecDuration;

typedef struct tagRSRecTimeTable
{
	TRS_DevChn         m_tDevChn;                    /*�豸ͨ����*/
	u16                m_wDurationNum;               /*��¼���ʱ�������*/
	TRS_RecDuration*   m_ptRecDurationList;          /*��¼���ʱ����б�*/
}TRS_RecTimeTable;

typedef struct tagRSRecAliasSet
{
	TRS_Token         m_tRecDurationToken;                             /*¼��ʱ��α�ʶ*/
	s8                m_szRecAlias[RS_MAXLEN_FILEALIAS+1];             /*¼�����*/
}TRS_RecAliasSet;

/*¼���ѯ�����ṹ��*/
typedef struct tagRSRecQueryCond
{
	TRS_DevChn       m_tPuChn;
	ERS_RecType    * m_peRecType;
	TRS_TimeRange  * m_ptQueryTimeRange;
	ERS_RecLockStatus * m_pemRecLockStat;    //add by Royan 20140319
}TRS_RecQueryCond;

typedef struct tagRSRecDaysQueryCond
{
	u8              m_byQueryPuChnNum;               /*ͬʱ��ѯ��ǰ��ͨ��������������ڵ���1*/
	TRS_DevChn    * m_ptPuChnList;                   /*ͬʱ��ѯ��ǰ��ͨ���б�*/
	TRS_TimeRange   m_tQueryTimeRange;               /*��ѯ��ʱ������, �һ����*/
}TRS_RecDaysQueryCond;

typedef struct tagRSOnePuChnRecDays
{
	TRS_DevChn      m_tPuChn;
	u32             m_dwDayBitMask;                  /*��¼�������Ϊ1*/
}TRS_OnePuChnRecDays;

typedef struct tagRSRecDays
{
	u8                     m_byPuChnNum;              /*ǰ������*/
	TRS_OnePuChnRecDays*   m_ptRecDaysList;         /*����ǰ��ͨ����¼�����*/
	TRS_TimeRange          m_tQueryTimeRange;
}TRS_RecDays;

typedef struct tagRSPlayParam
{
	TRS_DevChn         m_tPuChn;
	TRS_Token          m_tRecDurationToken;  /*��Ҫ�����¼��ʱ��α�ʶ*/
	u32                m_dwPlayStartTime;    /*����ʼʱ�䣨����ʱ�䣩*/
	u32                m_dwSSRC;             /*����ʱ����ʹ�õ�SSRC��0��ʾ��ָ������SSRC*/
	u8                 m_byReversePlay;      /*����or����*/
}TRS_PlayParam;

/*������ƽṹ��*/
typedef struct tagRSVCRCtrl
{
	ERS_VCRCmd         m_eVCRCmd;
	s32                m_nPlayRate;         /*�������ʣ�����Ϊ����������Ϊ����*/
	s32                m_nSeekTime;         /*���ʱ��*/
}TRS_VCRCtrl;

typedef struct tagRSRecDownLoadParam
{
	TRS_DevChn       m_tPuChn;                      /*���ض�����豸���*/
	TRS_Token        m_tRecDurationToken;           /*��Ҫ���ص�¼��ʱ��α�ʶ*/
	TRS_TimeRange    m_tDownloadTimeRange;          /*���ص�¼��ʱ���*/
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
	u32                m_dwCurPlayTime;                /*��ǰ����ʱ�䣬��λ���� ����ʱ��*/
	u32                m_dwTotalPlayTime;              /*������ʱ��*/
	u32                m_dwStartPlayTime;              /*��ʼ����ʱ�䣬��λ���� ����ʱ��*/
	BOOL32             m_bPlayFinished;                /*�����Ƿ����*/
}TRS_PlayProg;

typedef struct tagRSDownloadTask
{
	u32                    m_dwDownloadTaskID;
	TRS_RecDownLoadParam   m_tDownloadParam;
}TRS_DownloadTask;

typedef struct tagRSTaskHandle
{
	u32                 m_dwTaskID;                /*������*/
	TRS_Token           m_tResourceToken;          /*��Դ��ʶ*/
}TRS_TaskHandle;

typedef struct tagRSRecStopNotify
{
	u32                      m_dwTaskID;
	TRS_DevChn               m_tPuChn;
	ERS_ReasonOfStopRecord   m_eStopReason;
}TRS_RecStopNotify;

typedef struct tagRSRecTagAdd
{
	TRS_DevChn               m_tPuChn;            /*ǰ��ͨ��*/
	u32                      m_dwCreateTime;      /*��ǩ����ʱ��*/
	s8                       m_szRecTagDesc[RS_MAXLEN_RECTAG+1];    /*��ǩ����*/
}TRS_RecTagAdd;

typedef struct tagRSRecTagDel
{
	u32                      m_dwRecTagID;        /*��Ҫɾ���ı�ǩ���*/
}TRS_RecTagDel;

typedef struct tagRSRecTagQueryCond
{
	TRS_DevChn         *     m_ptDevChn;         /*��Ҫ��ѯ�ı�ǩ������ǰ��ͨ��*/
	TRS_TimeRange      *     m_ptTimeRange;      /*��Ҫ��ѯ��ʱ��Σ����72Сʱ�����û�и�����ѯʱ��Σ�Ĭ�ϲ�ѯ����ļ�¼*/
	s8                       m_szRecTagDescKeyWord[RS_MAXLEN_RECTAG+1];    /*��ǩ�����ؼ���*/
}TRS_RecTagQueryCond;

typedef struct tagRSRecTag
{
	u32                      m_dwRecTagID;           /*¼���ǩ���*/
	TRS_RecTagAdd            m_tRecTag;              /*¼���ǩ*/
}TRS_RecTag;

typedef struct tagRSRecTags
{
	u32                      m_dwTotalRecTagNum;       /*¼���ǩ����*/
	TRS_RecTag*              m_ptRecTagList;           /*¼���ǩ�б�*/
}TRS_RecTags;

typedef struct tagRSRecTagUpdate
{
	u32                       m_dwRecTagID;                             /*��ǩ���*/
	s8                        m_szRecTagDesc[RS_MAXLEN_RECTAG+1];       /*�޸ĺ�ı�ǩ����*/
}TRS_RecTagUpdate;

typedef struct  tagRSDownloadFileInfo
{
	s8                        m_szDownloadFilePath[RS_MAXLEN_FILEPATH+1];
	s8                        m_szDownloadFileName[RS_MAXLEN_FILENAME];
	TRS_MediaInfoExt          m_tMediaInfoExt;
}TRS_DownloadFileInfo;

typedef struct tagRSDeviceAlarm
{
	TRS_DevID                 m_tDeviceID;        /*�澯���߽���澯���豸���*/
	ERS_AlarmType             m_eAlarmType;       /*�澯����*/
	ERS_AlarmCode             m_eAlarmCode;       /*�澯��*/
}TRS_DeviceAlarm;

typedef struct tagRSRecLock
{
	TRS_TaskHandle *          m_ptTaskHandle;     /*�������ڽ��е�¼�������������벻Ϊ��*/
	TRS_Token *               m_ptRecordToken;    /*�����Ѿ����ڵ�¼�������������벻Ϊ��*/
	ERS_LockOperType          m_eLockOperType;
	u32                       m_dwLockDeadLine;
}TRS_RecLock;

/*¼��ɾ�������ṹ��*/
typedef struct tagRSRecDelCond
{
	TRS_DevChn       m_tPuChn;
	TRS_TimeRange    m_tDelTimeRange;
}TRS_RecDelCond;

/************************************************************************/
/*          �ص���������                                                */
/************************************************************************/
typedef void (*RSEventCB)(TRS_EventHandle tEventHandle, u8* pbyBuf, u32 dwBufLen, void* pvContext);

/************************************************************************/
/*          �����ӿڶ���                                                */
/************************************************************************/

/*====================================================================
������      :RSInit
����        :RS��ĳ�ʼ��
             1. ����RSInitʱ������RMS�������ӣ����ӳɹ����ʼ���ӿڷ��سɹ���
			    ���򷵻�ʧ�ܣ�
			 2. �����ӳɹ��󣬻��Զ�����ע����̣�ע��ɹ��󣬻ص�RMS����֪ͨ��
			    �ϲ�ֻ�����յ�RMS���ߵ�֪ͨ�󣬲ſ��Խ��к�������������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSInit(const TRS_InitParam *ptInitParam);

/*====================================================================
������      :RSUninit
����        :RS��ķ���ʼ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSUninit();

/*====================================================================
������      :RSSetEventCallback
����        :ע��ص�����
             1. ����RSInit�ɹ������������øýӿڽ��лص�������ע�᣻
			 2. ����SDK������нӿ�Ϊ�첽�ӿڣ�������нӿڵĽ����ͨ��
			    ע��Ļص��������أ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSSetEventCallback(RSEventCB pFunc, void* pvContext);

/*====================================================================
������      :RSNruDiscovery
����        :�������ߵ�����NRU�豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSNruDiscovery(TRS_Sequence *ptSequence);

/*====================================================================
������      :RSNruDevAdd
����        :���¼�������
			 1. �������������¼����������ڷ�����IP��ַ����˵����¼��������Ѿ���ʹ��
			 2. ���������Ѿ���ʹ�õ�¼������������Ƚ���¼���������Ϊ����״̬
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSNruDevAdd(const TRS_AddNruInfo *ptNruInfo, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSNruDevChange
����        :�޸�¼�������
			 1. �����豸����޸��豸��Ϣ�������豸�����豸��ͣ��ʶ��
			 2. �豸��Ų��ɸı�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSNruDevChange(const TRS_ChangeNruInfo *ptNruInfo, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSNruDevDel
����        :ɾ��¼�������
			 1. ��һ̨¼���������ƽ̨��ɾ��ʱ����Ӱ�쵽��nru�ϵ�¼�����������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSNruDevDel(const TRS_DevID *ptNruID, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSNruNetworkParamModify
����        :�޸�¼����������������
			 1. �޸���¼��������������������Ҫ����NRU�ſ�����Ч
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSNruNetworkParamModify(const TRS_DevID *ptNruID, const TRS_NetworkInfo *ptNetworkParam, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSAllNruInfoGet
����        :��ȡ����������Nru��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSAllNruInfoGet(TRS_Sequence *ptSequence);

/*====================================================================
������      :RSNruSysStatusGet
����        :��ȡNru��ϵͳ״̬��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSNruSysStatusGet(const TRS_DevID *ptNruID, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSSysParamSet
����        :�ƶ������޸�ϵͳ����
             1. ������RSInit֮������ϵͳ���� 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_SysRecPlan *ptSysRecPlan          -- [IN] ¼��ƻ�
		      TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSSysParamSet(const TRS_SysParam *ptSysParam, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSSysParamQuery
����        :��ѯϵͳ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TRS_Sequence *ptSequence                -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSSysParamQuery(TRS_Sequence *ptSequence);

/*====================================================================
������      :RSPuChnBinding
����        :��ǰ��ͨ���󶨵�ĳ��¼���������ĳ�����̷���
			 1. ���ǰ��ͨ�����˴��̷�������ô������ǰ��ͨ�������ڸ�
			    ��������¼��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_PuChnBinding *ptPuChnBinding      -- [IN] ǰ��ͨ������Ϣ
		      TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSPuChnBinding(const TRS_PuChnBinding *ptPuChnBinding, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSPuChnUnBinding
����        :���ǰ��ͨ����ĳ��¼���������ĳ�����̷����İ󶨹�ϵ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_PuChnUnBinding *ptPuChnUnBinding  -- [IN] ��Ҫ��󶨵�ǰ��ͨ����Ϣ
		      TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSPuChnUnBinding(const TRS_PuChnUnBinding *ptPuChnUnBinding, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSPuChnBindingQuery
����        :��ѯĳ��¼�����ĳ�����̷����ϰ󶨵�ǰ��ͨ����Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_PuChnBindingQuery *ptPuChnBindingQuery  -- [IN] ǰ��ͨ������Ϣ
		      TRS_Sequence *ptSequence                          -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSPuChnBindingQuery(const TRS_PuChnBindingQuery *ptPuChnBindingQuery, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecResourceAlloc
����        :¼����Դ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecTask *ptRecTask       -- [IN] ¼���������
			  TRS_Sequence *ptSequence           -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecResourceAlloc(const TRS_RecTask *ptRecTask, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecResourceRelease
����        :¼����Դ�ͷ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecordHandle *ptRecHandle         -- [IN] ��Ҫ�ͷŵ���Դ
			  TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecResourceRelease(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecTaskStart
����        :����¼������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_TaskHandle *ptTaskHandle          -- [IN] ������Դ���
              TRS_MediaTranParam   *ptTranParam           -- [IN] �������
			  TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecTaskStart(const TRS_TaskHandle *ptTaskHandle, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecTypeUpdate
����        :����¼�����ͣ������������Ҫʹ�ã�
			 1. �澯¼�� -- �ƻ�¼��
			 2. �ƻ�¼�� -- �澯¼��
			 3. �ֶ�¼�� -- �澯¼��
			 4. �澯¼�� -- �ֶ�¼��
			 5. �ֶ�¼�� -- �ƻ�¼��
			 6. �ƻ�¼�� -- �ֶ�¼��
             
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_TaskHandle *ptTaskHandle          -- [IN]  ��Դ���
              TRS_RecordTypeUpdate * ptRecordTypeUpdate   -- [IN]  ¼�����͸�����Ϣ
              TRS_Sequence *ptSequence                    -- [OUT] �������к� 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecTypeUpdate(const TRS_TaskHandle *ptTaskHandle, TRS_RecordTypeUpdate * ptRecordTypeUpdate, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecTaskStop
����        :ֹͣ¼������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_TaskHandle *ptTaskHandle  -- [IN]  ��Դ���
              TRS_Sequence *ptSequence            -- [OUT] �������к� 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecTaskStop(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecordQuery
����        :¼���ѯ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: 	 

  ����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
  ----------------------------------------------------------------------
  �޸ļ�¼    :
  ��  ��      �汾        �޸���        �޸�����
  2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecordQuery(const TRS_RecQueryCond *ptQueryCond, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecordDaysQuery
����        :��ѯǰ��ͨ����ĳ��ʱ������Щ����¼���¼
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: 	 

  ����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
  ----------------------------------------------------------------------
  �޸ļ�¼    :
  ��  ��      �汾        �޸���        �޸�����
  2012/05/11	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecordDaysQuery(const TRS_RecDaysQueryCond *ptQueryCond, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecordAliasSet
����        :����¼���ļ��ı���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecAliasSet *ptRecAliasSet  -- ��IN��¼�������Ϣ	 
              TRS_Sequence *ptSequence              -- ��OUT���������к�

  ����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
  ----------------------------------------------------------------------
  �޸ļ�¼    :
  ��  ��      �汾        �޸���        �޸�����
  2014/03/12	1.0		  �����    	  ����
====================================================================*/
ERS_ErrType RSRecordAliasSet(const TRS_RecAliasSet *ptRecAliasSet, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSPlayResourceAlloc
����        :������Դ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_PlayTask *ptPlayTask     -- [IN] �����������
			  TRS_MediaTranParam   *ptTranParam           -- [IN] �������
			  TRS_Sequence *ptSequence           -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSPlayResourceAlloc(const TRS_PlayTask *ptPlayTask, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSPlayResourceRelease
����        :������Դ�ͷ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecordHandle *ptRecHandle         -- [IN] ��Ҫ�ͷŵ���Դ
			  TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSPlayResourceRelease(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSPlayTaskStart
����        :������������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_TaskHandle *ptTaskHandle          -- [IN] ������Դ���
			  TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSPlayTaskStart(const TRS_TaskHandle *ptTaskHandle, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSVCRCtrl
����        :VCR����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_TaskHandle *ptTaskHandle    -- [IN] ������
              const TRS_VCRCtrl *ptVcrCtrl          -- [IN] VCR��������	
			  TRS_Sequence *ptSequence 	            -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSVCRCtrl(const TRS_TaskHandle *ptTaskHandle, const TRS_VCRCtrl *ptVcrCtrl, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSPlayTaskStop
����        :ֹͣ��������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_TaskHandle *ptTaskHandle  -- [IN]  ��Դ���
              TRS_Sequence *ptSequence            -- [OUT] �������к� 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSPlayTaskStop(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSDownloadResourceAlloc
����        :������Դ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_DownloadTask *ptDownloadTask     -- [IN] �����������
			  TRS_MediaTranParam   *ptTranParam          -- [IN] �������
			  TRS_Sequence *ptSequence           -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSDownloadResourceAlloc(const TRS_DownloadTask *ptDownloadTask, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSDownloadResourceRelease
����        :������Դ�ͷ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecordHandle *ptRecHandle         -- [IN] ��Ҫ�ͷŵ���Դ
			  TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSDownloadResourceRelease(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSPlayTaskStart
����        :������������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_TaskHandle *ptTaskHandle          -- [IN] ������Դ���
			  TRS_Sequence *ptSequence                    -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSDownloadTaskStart(const TRS_TaskHandle *ptTaskHandle, TRS_MediaTranParam *ptTranParam, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSDownloadTaskStop
����        :ֹͣ��������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_TaskHandle *ptTaskHandle  -- [IN]  ��Դ���
              TRS_Sequence *ptSequence            -- [OUT] �������к� 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/15	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSDownloadTaskStop(const TRS_TaskHandle *ptTaskHandle, TRS_Sequence *ptSequence);


/*====================================================================
������      :RSRecTagAdd
����        :���¼���ǩ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecTagAdd *ptRecTagAdd  -- [IN]  ��ӵ�¼���ǩ��Ϣ
              TRS_Sequence *ptSequence          -- [OUT] �������к� 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/05/08	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecTagAdd(const TRS_RecTagAdd *ptRecTagAdd, TRS_Sequence *ptSequence);


/*====================================================================
������      :RSRecTagDel
����        :ɾ��¼���ǩ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecTagDel *ptRecTagDel  -- [IN]  ��Ҫɾ����¼���ǩ��Ϣ
              TRS_Sequence *ptSequence          -- [OUT] �������к� 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/05/08	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecTagDel(const TRS_RecTagDel *ptRecTagDel, TRS_Sequence *ptSequence);


/*====================================================================
������      :RSRecTagUpdate
����        :�޸�¼���ǩ��ֻ���޸ı�ǩ��������Ϣ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecTagUpdate *ptRecTagUpdate  -- [IN]  ���µ�¼���ǩ��Ϣ
              TRS_Sequence *ptSequence                -- [OUT] �������к� 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/05/08	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecTagUpdate(const TRS_RecTagUpdate *ptRecTagUpdate, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecTagQuery
����        :��ѯ¼���ǩ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecTagQueryCond *ptRecTagQueryCond  -- [IN]  ��ѯ����
              TRS_Sequence *ptSequence                      -- [OUT] �������к� 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/05/08	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecTagQuery(const TRS_RecTagQueryCond *ptRecTagQueryCond, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSEventDescriptionGetFromEventID
����        :����eventid��ѯ��Ϣ������Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: u32 dwEventID                              -- [IN]  eventid
              s8* szEventDescription                     -- [OUT] ������Ϣ 	 

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/09/08	1.0			��Ф��    	  ����
====================================================================*/
ERS_ErrType RSEventDescriptionGetFromEventID(u32 dwEventID, s8* szEventDescription);

/*====================================================================
������      :RSIsTaskHandleMatchDev
����        :�ж�handle�Ƿ����ڸ��豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: TRS_TaskHandle *ptHandle                      -- [IN]  handle
              TRS_DevID* ptDevId                            -- [IN] �豸��� 	 

����ֵ˵��  :�����ɹ�����TRUE,ʧ�ܷ���FALSE
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/05/08	1.0			�����    	  ����
====================================================================*/
BOOL32  RSIsTaskHandleMatchDev(TRS_TaskHandle *ptHandle, TRS_DevID* ptDevId);

/*====================================================================
������      :RSTokenContruct
����        :����token��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: TRS_DevChn* ptDevChn            -- [IN]  ǰ���豸��Ϣ
              s8* szToken                     -- [IN��OUT] ������Ϣ 
			  u16 wTokenLength                -- [IN]token����

����ֵ˵��  :�����ɹ�����TRUE,ʧ�ܷ���FALSE
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/05/08	1.0			��Ф��    	  ����
====================================================================*/
BOOL32  RSTokenContruct(TRS_DevChn* ptDevChn, void* szToken, u16 wTokenLength);

/*====================================================================
������      :RSStunAddrSet
����        ����stun��������ip��Ϣ��������rms�ɹ�֮����Ҫ���ñ��ӿ�����stun�������ĵ�ַ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: TRS_NetAddr* ptStunAddr          -- [IN]  stun��������ַ 
			  TRS_Sequence *ptSequence         -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/09/08	1.0			��Ф��    	  ����
====================================================================*/
ERS_ErrType RSStunAddrSet(TRS_NetAddr* ptStunAddr, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecDel
����         ɾ��¼���¼
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_Token *ptRecordToken  -- [IN]  ��Ҫɾ�����ļ�Token 
			  TRS_Sequence *ptSequence        -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2014/02/27	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecDel(const TRS_Token *ptRecordToken, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecDelbyCond
����        :��������ɾ��¼���¼
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecDelCond *ptRecDelCond  -- [IN]  ɾ�������� 
			  TRS_Sequence *ptSequence        -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2016/08/29	1.0			Royan    	  ����
====================================================================*/
ERS_ErrType RSRecDelbyCond(const TRS_RecDelCond *ptRecDelCond, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSRecLock
����         ¼������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_RecLock *ptRecLock  -- [IN]  ������Ϣ 
			  TRS_Sequence *ptSequence      -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2014/03/10	1.0			�����    	  ����
====================================================================*/
ERS_ErrType RSRecLock(const TRS_RecLock * ptRecLock, TRS_Sequence *ptSequence);

/*====================================================================
������      :RSUpdateOtherStorageLicense
����        :�����⳧�������������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const TRS_OtherStorageLicense *ptOtherStorageLicense  -- [IN]  �⳧���������������T�� 
			  TRS_Sequence *ptSequence                -- [OUT] �������к�

����ֵ˵��  :�����ɹ�����ERS_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2016/08/05	1.0			��³��    	  ����
====================================================================*/
ERS_ErrType RSUpdateOtherStorageLicense(const TRS_OtherStorageLicense *ptOtherStorageLicense, TRS_Sequence *ptSequence);

#endif

