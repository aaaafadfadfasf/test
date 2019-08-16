#ifndef _RS_TYPES_H_
#define _RS_TYPES_H_

#include "osp.h"
#include "rscomm.h"
#include <time.h>

#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif

/************************************************************************/
/* 宏定义                                                               */
/************************************************************************/

#define RSCOMM_MAXLEN_OSPMSG           (u16)(0x7000)
#define RSCOMM_MAXLEN_NRUID             32
#define RSCOMM_MAXLEN_UUID              65
#define RSCOMM_MAXLEN_TIMEZONE          64
#define RSCOMM_MAXLEN_DEVICENAME        32
#define RSCOMM_MAXLEN_TYPE              32
#define RSCOMM_MAXLEN_MANUFAC           32
#define RSCOMM_MAXLEN_MODEL             8
#define RSCOMM_MAXLEN_ENCODINGNAME      8
#define RSCOMM_MAXLEN_IPADDRESS         39
#define RSCOMM_MAXLEN_PARTITIONNAME     128
#define RSCOMM_MAXLEN_MOUNTNAME         64
#define RSCOMM_MAXLEN_FILENAME          128
#define RSCOMM_MAXLEN_FILEPATH          128
#define RSCOMM_MAXLEN_IFNAME            16
#define RSCOMM_MAXLEN_RECTAG            128
#define RSCOMM_MAXLEN_MEDIAINFOEXT      1024
#define RSCOMM_MAXLEN_PUNCHKEY          64
#define RSCOMM_MAXNUM_CHANNELADDR       3
#define RSCOMM_MAXNUM_ADDR              8
#define RSCOMM_MAXLEN_FILEALIAS         128
#define RSCOMM_MAXLEN_DBUSERNAME        32
#define RSCOMM_MAXLEN_DBPASSWORD        32
#define RSCOMM_MAXLEN_DBNAME            32
#define RSCOMM_MAXNUM_NETSEGIP          4
#define RSCOMM_MAXLEN_STORAGEVENDOR     10
#define RSCOMM_STORAGEVENDOR_KDM	    "kdm"
#define RSCOMM_MAX_EXTRA_CHN_NUM	    8

/************************************************************************/
/* 无效值                                                               */
/************************************************************************/
#define RSCOMM_INVALID_CHANNEL          (s16)-1


/************************************************************************/
/* 枚举类型定义                                                         */
/************************************************************************/

typedef enum
{
	RSCOMM_IPTYPE_IPv4,
	RSCOMM_IPTYPE_IPv6,
	RSCOMM_IPTYPE_UNKNOWN,
}ERSComm_IPType;

typedef enum
{
	RSCOMM_TIMETYPE_LOCAL,
	RSCOMM_TIMETYPE_UTC,
}ERSComm_TimeType;

typedef enum
{
	RSCOMM_COVERPOLICY_STOPRECORD,          /*磁盘满停录像*/
	RSCOMM_COVERPOLICY_COVERALL,            /*磁盘满覆盖所有录像*/
	RSCOMM_COVERPOLICY_COVERNORMAL,         /*磁盘满覆盖普通录像*/
	RSCOMM_COVERPOLICY_UNKNOWN,
}ERSComm_CoverPolicy;

typedef enum
{
	RSCOMM_RECTYPE_ALARM = 1,            /*告警录像*/
	RSCOMM_RECTYPE_HANDLE,               /*手动录像*/
	RSCOMM_RECTYPE_PLAN,                 /*计划录像*/
	RSCOMM_RECTYPE_UNKNOWN,
}ERSComm_RecType;

typedef enum
{
	RSCOMM_RECMODE_ALL,                   /*既录视频又录音频*/
	RSCOMM_RECMODE_ONLYVIDEO,             /*只录视频*/
	RSCOMM_RECMODE_ONLYKEYFRAME,          /*只录关键帧*/
	RSCOMM_RECMODE_UNKNOWN,               /*未知模式*/
}ERSComm_RecMode;  

typedef enum
{
	RSCOMM_RECSTATE_RECORD,               /*录像*/
	RSCOMM_RECSTATE_PRERECORD,            /*预录*/
	RSCOMM_RECSTATE_UNKNOWN,              /*未知状态*/
}ERSComm_RecState;

typedef enum
{
	RSCOMM_TRANSCHNTYPE_MEDIA,
	RSCOMM_TRANSCHNTYPE_DATA,
}ERSComm_TransChnType;

typedef enum
{
	RSCOMM_MEDIATYPE_AUDIO,
	RSCOMM_MEDIATYPE_VIDEO,
	RSCOMM_MEDIATYPE_DATA,
	RSCOMM_MEDIATYPE_UNKNOWN,
}ERSComm_MediaType;

typedef enum
{
	RSCOMM_TRANSPORTPROTOCOL_UDP,           /*UDP*/
	RSCOMM_TRANSPORTPROTOCOL_TCP,           /*TCP*/
	RSCOMM_TRANSPORTPROTOCOL_RTSP,          /*RTSP/TCP*/
	RSCOMM_TRANSPORTPROTOCOL_UNIXSOCKET,    /*域套接字*/
	RSCOMM_TRANSPORTPROTOCOL_UNKNOWN,
}ERSComm_TransportProtocol;

typedef enum
{
	RSCOMM_STREAMTYPE_RTP_UNICAST,
	RSCOMM_STREAMTYPE_RTP_MULTICAST,
}ERSComm_StreamType;

typedef enum
{
	RSCOMM_VCRCMD_PAUSE,                  /*暂停放像*/
	RSCOMM_VCRCMD_RESUME,                 /*恢复放像*/
	RSCOMM_VCRCMD_SEEK,                   /*时间定位*/
	RSCOMM_VCRCMD_FASTPLAY,               /*快放*/
	RSCOMM_VCRCMD_SLOWPLAY,               /*慢放*/
	RSCOMM_VCRCMD_ONEFRAME,               /*单帧播放*/
	RSCOMM_VCRCMD_KEYFRAME,               /*只放关键帧*/
	RSCOMM_VCRCMD_KEYSEEK,                /*时间定位(从前一个关键帧开始)*/
	RSCOMM_VCRCMD_REVERSEPLAY,            /*倒放*/
	RSCOMM_VCRCMD_FORWARDPLAY,            /*正放*/
	RSCOMM_VCRCMD_AUTO_KEYFRAME,          /*自动模式，是否抽帧由NRU确定：低倍速全帧，高倍速只放关键帧*/
	RSCOMM_VCRCMD_FORCE_KEYFRAME,         /*强制只放关键帧*/
	RSCOMM_VCRCMD_FORCE_ALLFRAME,         /*强制回放全帧*/
	RSCOMM_VCRCMD_UNKNOWN,
}ERSComm_VCRCmd;

typedef enum
{
	RSCOMM_RUNMODE_STANDALONE = 1,      /*非集成在阵列上的Nru运行模式*/
	RSCOMM_RUNMODE_INTEGRATED = 2,      /*集成在阵列上的Nru运行模式*/
	RSCOMM_RUNMODE_UNKNOWN
}ERSComm_RunMode;

typedef enum
{
	RSCOMM_PARTFORMAT_NORMAL = 1,
	RSCOMM_PARTFORMAT_VBFS,
	RSCOMM_PARTFORMAT_CLOUD,
	RSCOMM_PARTFORMAT_UNKNOWN,
}ERSComm_PartFormat;

typedef enum
{
	RSCOMM_PARTSTATUS_OFFLINE,
	RSCOMM_PARTSTATUS_ONLINE,
	RSCOMM_PARTSTATUS_ALARM_LEVEL_ONE,
	RSCOMM_PARTSTATUS_ALARM_LEVEL_TWO,
	RSCOMM_PARTSTATUS_UNKNOWN,
}ERSComm_PartStatus;

typedef enum
{
	RSCOMM_TRIGGERREASON_HANDLE,
	RSCOMM_TRIGGERREASON_LOAD_BALANCE,              /*负载均衡*/
	RSCOMM_TRIGGERREASON_SWITCHTO_BINDINGDISK,      /*切换到绑定磁盘*/
	RSCOMM_TRIGGERREASON_SWITCHFROM_BINDINGDISK,    /*从已经绑定了前端通道的磁盘分区切换到其他分区*/
	RSCOMM_TRIGGERREASON_PARTITION_FULL,
	RSCOMM_TRIGGERREASON_BIGFILE_SWITCH_FAIL,
	RSCOMM_TRIGGERREASON_FORMAT_CHANGE,
	RSCOMM_TRIGGERREASON_SWITCHRECORD_FAIL,
	RSCOMM_TRIGGERREASON_NOSTREAM,
	RSCOMM_TRIGGERREASON_UNKNOWN,
}ERSComm_TriggerReasonOfStopRecord;

typedef enum
{
	RSCOMM_CHANNELTYPE_RTP,
	RSCOMM_CHANNELTYPE_RTCP,
	RSCOMM_CHANNELTYPE_DATA,
}ERSComm_ChannelType;

typedef enum
{
	RSCOMM_OPERSTYLE_NORMAL,
	RSCOMM_OPERSTYLE_CHECKOUT,
	RSCOMM_OPERSTYLE_UNKNOWN,
}ERSComm_OperStyle;

typedef enum
{
	RSCOMM_ALARMTYPE_ALARM,
	RSCOMM_ALARMTYPE_LIFTALARM,
	RSCOMM_ALARMTYPE_UNKNOWN,
}ERSComm_AlarmType;

typedef enum
{
	RSCOMM_ALARMCODE_REACHTO_THRESHOLDOFRECORDBANDWIDTH = 1,
}ERSComm_AlarmCode;

typedef enum
{ 
	RSCOMM_OPERTYPE_NULL, 
	RSCOMM_OPERTYPE_GET, 
	RSCOMM_OPERTYPE_LOCK, 
	RSCOMM_OPERTYPE_UNLOCK,
}ERSComm_LockOperType;

//{{{{{{{{{{{{{{{ add by Royan 20140317
typedef enum
{
	RSCOMM_REC_STAT_UNLOCK,
	RSCOMM_REC_STAT_LOCK, 
	RSCOMM_REC_STAT_UNKNOWN,
}ERSComm_RecLockStat;
//}}}}}}}}}}}}}}} add by Royan 20140317 

typedef enum
{
	RSCOMM_STORAGE_MODE_NORMAL,
	RSCOMM_STORAGE_MODE_CLOUD, 
	RSCOMM_STORAGE_MODE_MIXED,
}ERSComm_StorageMode;

typedef enum
{
	RSCOMM_SYNCOPERTYPE_UPDATE,
	RSCOMM_SYNCOPERTYPE_DEL,
	RSCOMM_SYNCOPERTYPE_UNKNOWN,
}ERSComm_SyncOperType;

typedef enum
{
	RSCOMM_TRIGGERREASONOFDELRECORD_EXT3_RECORD_COVER,
	RSCOMM_TRIGGERREASONOFDELRECORD_VBFS_RECORD_COVER,
	RSCOMM_TRIGGERREASONOFDELRECORD_SMALL_FILE,
	RSCOMM_TRIGGERREASONOFDELRECORD_HANDLE,
	
}ERSComm_TriggerReasonOfDelRecord;

/************************************************************************/
/* 结构体定义                                                           */
/************************************************************************/

struct TRSComm_Time
{
public:
	TRSComm_Time()
	{
		memset(this, 0, sizeof(TRSComm_Time));
	}

	void SetTime(const struct tm *ptTime)
	{
		if (NULL == ptTime)
		{
			return;
		}

		m_nSec = ptTime->tm_sec;
		m_nMin = ptTime->tm_min;
		m_nHour = ptTime->tm_hour;
		m_nMday = ptTime->tm_mday;
		m_nMon = ptTime->tm_mon;
		m_nWday = ptTime->tm_wday;
		m_nYear = ptTime->tm_year;
		m_nYday = ptTime->tm_yday;
		m_nIsdst = ptTime->tm_isdst;
	}

	void SetTime(const time_t dwTime)
	{
		struct tm * ptTime = NULL;
#ifdef _LINUX_
		struct tm ttmTime;
		ptTime = localtime_r(&dwTime, &ttmTime);
#else
		ptTime = localtime((const time_t*)&dwTime);
#endif
		m_nSec = ptTime->tm_sec;
		m_nMin = ptTime->tm_min;
		m_nHour = ptTime->tm_hour;
		m_nMday = ptTime->tm_mday;
		m_nMon = ptTime->tm_mon;
		m_nWday = ptTime->tm_wday;
		m_nYear = ptTime->tm_year;
		m_nYday = ptTime->tm_yday;
		m_nIsdst = ptTime->tm_isdst;
	}

	time_t GetTime()
	{
		struct tm tmTime;
		tmTime.tm_sec = m_nSec;
		tmTime.tm_min = m_nMin;
		tmTime.tm_hour = m_nHour;
		tmTime.tm_mon = m_nMon;
		tmTime.tm_mday = m_nMday;
		tmTime.tm_wday = m_nWday;
		tmTime.tm_yday = m_nYday;
		tmTime.tm_year = m_nYear;
		tmTime.tm_isdst = m_nIsdst;

		return mktime(&tmTime);
	}

	u16 GetYear()
	{
		return (m_nYear + 1900);
	}

	u8 GetMonth()
	{
		return (m_nMon + 1);
	}

	u8 GetDay()
	{
		return m_nMday;
	}

	u8 GetHour()
	{
		return m_nHour;
	}

	u8 GetMinute()
	{
		return m_nMin;
	}

	u8 GetSeconds()
	{
		return m_nSec;
	}

	BOOL32 SetYear(const u16 wYear)
	{
		if (wYear < 1900)
		{
			return FALSE;
		}

		m_nYear = wYear-1900;

		return TRUE;
	}

	BOOL32 SetMonth(const u8 byMonth)
	{
		if (byMonth > 0 && byMonth <= 12)
		{
			m_nMon  = byMonth - 1;
		}
		else
		{
			m_nMon  = 0;
		}

		return TRUE;
	}

	BOOL32 SetDay(const u8 byDay)
	{
		if (byDay > 0 && byDay <= 31)
		{
			m_nMday = byDay;
		}
		else
		{
			m_nMday = 1;
		}

		return TRUE;
	}

	BOOL32 SetHour(const u8 byHour)
	{
		if (byHour < 24)
		{
			m_nHour = byHour;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	BOOL32 SetMinute(const u8 byMin)
	{
		if (byMin < 60)
		{
			m_nMin = byMin;

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	BOOL32 SetSecond(const u8 bySecond)
	{
		if (bySecond < 60)
		{
			m_nSec = bySecond;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	BOOL32 GetString(s8* pszString, s32 nStringLen)
	{
		if (nStringLen < 20)
		{
			return FALSE;
		}

		sprintf(pszString, "%.4d-%.2d-%.2d_%.2d:%.2d:%.2d", 
							GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSeconds()); 		
		return TRUE;
	}

protected:
	s32     m_nSec;         /* seconds */
	s32     m_nMin;         /* minutes */
	s32     m_nHour;        /* hours */
	s32     m_nMday;        /* day of the month */
	s32     m_nMon;         /* month */
	s32     m_nYear;        /* year */
	s32     m_nWday;        /* day of the week */
	s32     m_nYday;        /* day in the year */
	s32     m_nIsdst;       /* daylight saving time */   
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_DevID
{
public:
	TRSComm_DevID(void)
	{
		SetNull();
	}

	BOOL32 operator == ( const TRSComm_DevID &tDevID ) const
	{
		if (0 == memcmp(m_szUUID, tDevID.m_szUUID, RSCOMM_MAXLEN_UUID))
		{
			return TRUE;
		}
		return FALSE;
	}

	BOOL32 operator != ( const TRSComm_DevID &tDevID ) const
	{
		if (0 != memcmp(m_szUUID, tDevID.m_szUUID, RSCOMM_MAXLEN_UUID))
		{
			return TRUE;
		}

		return FALSE;
	}

	void   operator = ( const TRSComm_DevID &tDevID )
	{
		memcpy(m_szUUID, tDevID.m_szUUID, RSCOMM_MAXLEN_UUID);
		m_szUUID[RSCOMM_MAXLEN_UUID] = '\0';
	}

	BOOL32 SetDevID(const s8* pszDevID)
	{
		if (NULL == pszDevID)
		{
			return FALSE;
		}

		if (0 == strlen(pszDevID))
		{
			SetNull();
			return TRUE;
		}
			
		memcpy(m_szUUID, pszDevID, RSCOMM_MAXLEN_UUID);
		m_szUUID[RSCOMM_MAXLEN_UUID] = '\0';

		return TRUE;
	}

	const TRSComm_DevID GetDevID() const 
	{
		return (*this);
	}

	const s8* GetStringID() const 
	{
		return m_szUUID;
	}

	BOOL32 IsNull() const
	{
		if (strlen(m_szUUID) == 0 || memcmp(m_szUUID, "00000000000000000000000000000000", 32) == 0)
		{
			return TRUE;
		}

		return FALSE;
	}

protected:
	void SetNull()
	{
		memset(m_szUUID, '0', RSCOMM_MAXLEN_UUID);
		m_szUUID[RSCOMM_MAXLEN_UUID] = '\0';
	}

protected:
	s8      m_szUUID[RSCOMM_MAXLEN_UUID+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_DeletePuBinding : public TRSComm_DevID
{
public:
    TRSComm_DeletePuBinding()
    {
        SetNull();
        memset(this, 0, sizeof(TRSComm_DeletePuBinding));
    }

    u16 GetStartIndex() const
    {
        return m_wStartIndex;
    }

    void SetStartIndex(u16 var)
    {
        m_wStartIndex = var;
    }

private:
    u16           m_wStartIndex;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
    ;

struct TRSComm_DevDeadLine : public TRSComm_DevID
{
public:
	TRSComm_DevDeadLine()
	{
		SetNull();
		m_dwDeadLine = 0;
	}

	BOOL32 operator== (const TRSComm_DevDeadLine& var) const
	{
		if (GetDevID() != var.GetDevID())
		{
			return FALSE;
		}

		if (m_dwDeadLine != var.GetDeadLine())
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL32 operator!= (const TRSComm_DevDeadLine& var)
	{
		return !((*this) == var);
	}

	u32  GetDeadLine() const
	{
		return m_dwDeadLine;
	}

	void SetDeadLine(u32 var)
	{
		m_dwDeadLine = var;
	}

protected:
	u32          m_dwDeadLine;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_DevChn : public TRSComm_DevID
{
public:
	TRSComm_DevChn()
	{
		SetNull();
		m_wChnID = (s16)-1;
	}

	BOOL32 operator== (const TRSComm_DevChn& tDevChan) const
	{
		if (GetDevID() != tDevChan.GetDevID())
		{
			return FALSE;
		}

		if (m_wChnID != tDevChan.GetEdcChnId())
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL32 operator!= (const TRSComm_DevChn& tDevChan)
	{
		return !((*this) == tDevChan);
	}
    
    s16  GetEdcChnId() const
	{
		return m_wChnID;
	}

    void SetEdcChnId(s16 wEdcChnId)
	{
		m_wChnID = wEdcChnId;
	}

protected:
	s16          m_wChnID;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;



struct TRSComm_DevBaseInfo
{
public:
	TRSComm_DevBaseInfo()
	{
		memset(this, 0, sizeof(TRSComm_DevBaseInfo));
	}
	
	TRSComm_DevID& GetDevID()
	{
		return m_tDevID;
	}

	const TRSComm_DevID& GetDevID() const
	{
		return m_tDevID;
	}
	
	void SetDevID(const TRSComm_DevID& tDevID)
	{
		m_tDevID = tDevID;
	}
	
	s8 * GetDevName()
	{
		return m_szDevName;
	}

	const s8 * GetDevName() const
	{
		return m_szDevName;
	}
	
	void SetDevName(const s8* pszDevName)
	{
		if (NULL == pszDevName)
		{
			return ;
		}
		
		s16 wNameLen = strlen(pszDevName);
		wNameLen = wNameLen <= RSCOMM_MAXLEN_DEVICENAME ? wNameLen : RSCOMM_MAXLEN_DEVICENAME;
		strncpy(m_szDevName, pszDevName, wNameLen);
		m_szDevName[wNameLen] = '\0';
	}
	
	s8 * GetDeviceType()
	{
		return m_szDeviceType;
	}

	const s8 * GetDeviceType() const
	{
		return m_szDeviceType;
	}
	
	void SetDeviceType(const s8* pszDevcieType)
	{
		if (0 == strlen(pszDevcieType))
		{
			return ;
		}
		
		s16 wCopyLen = strlen(pszDevcieType);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_TYPE ? wCopyLen : RSCOMM_MAXLEN_TYPE;
		strncpy(m_szDeviceType, pszDevcieType, wCopyLen);
		m_szDeviceType[wCopyLen] = '\0';
	}
	
	s8 * GetDeviceManuFactory()
	{
		return m_szManuFactory;
	}

	const s8 * GetDeviceManuFactory() const
	{
		return m_szManuFactory;
	}
	
	void SetDeviceManuFactory(const s8* pszManuFactory)
	{
		if (NULL == pszManuFactory)
		{
			return;
		}
		
		s16 wCopyLen = strlen(pszManuFactory);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_MANUFAC ? wCopyLen : RSCOMM_MAXLEN_MANUFAC;
		strncpy(m_szManuFactory, pszManuFactory, wCopyLen);
		m_szManuFactory[wCopyLen] = '\0';
	}
	
protected:
	TRSComm_DevID   m_tDevID;
	s8              m_szDevName[RSCOMM_MAXLEN_DEVICENAME+1];
	s8              m_szDeviceType[RSCOMM_MAXLEN_TYPE+1];
	s8              m_szManuFactory[RSCOMM_MAXLEN_MANUFAC+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_SDKRegInfo : public TRSComm_DevBaseInfo
{
public:
	TRSComm_SDKRegInfo()
	{
		memset(this, 0, sizeof(TRSComm_SDKRegInfo));
	}

	time_t GetSysTime()
	{
		return m_tSysTime.GetTime();
	}

	void SetSysTime(struct tm *ptTime)
	{
		if (NULL == ptTime)
		{
			return;
		}

		m_tSysTime.SetTime(ptTime);
	}

protected:
	TRSComm_Time   m_tSysTime; 
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecSysParam
{
public:
	TRSComm_RecSysParam()
	{
		memset(this, 0, sizeof(TRSComm_RecSysParam));
	}

	ERSComm_CoverPolicy GetCoverPolicy()
	{
		return m_eSysCoverPolicy;
	}

	void SetCoverPolicy(const ERSComm_CoverPolicy eCoverPolicy)
	{
		m_eSysCoverPolicy = eCoverPolicy;
	}

	void SetCoverPolicy(const u8 byCoverPolicy)
	{
		switch(byCoverPolicy)
		{
		case RSCOMM_COVERPOLICY_STOPRECORD:
			m_eSysCoverPolicy = RSCOMM_COVERPOLICY_STOPRECORD;
			break;
		case RSCOMM_COVERPOLICY_COVERALL:
			m_eSysCoverPolicy = RSCOMM_COVERPOLICY_COVERALL;
			break;
		case RSCOMM_COVERPOLICY_COVERNORMAL:
			m_eSysCoverPolicy = RSCOMM_COVERPOLICY_COVERNORMAL;
			break;
		case RSCOMM_COVERPOLICY_UNKNOWN:
			break;
		}
	}

protected:
	ERSComm_CoverPolicy   m_eSysCoverPolicy;
	
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_CoverParam
{
public:

	TRSComm_CoverParam()
	{
		memset(this, 0, sizeof(TRSComm_CoverParam));
	}

	const s8 * GetPartitionName() const 
	{
		return m_szPartitionMountName;
	}

	s8 * GetPartitionName()
	{
		return m_szPartitionMountName;
	}

	void SetPartitionName(const s8* pszPartitionName)
	{
		if (NULL == pszPartitionName)
		{
			return;
		}

		s16 wCopyLen = strlen(pszPartitionName);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_MOUNTNAME ? wCopyLen : RSCOMM_MAXLEN_MOUNTNAME;
		strncpy(m_szPartitionMountName, pszPartitionName, wCopyLen);
		m_szPartitionMountName[wCopyLen] = '\0';
	}

	const u32 GetRecKeepDays() const
	{
		return m_dwRecKeepDays;
	}

	u32 GetRecKeepDays()
	{
		return m_dwRecKeepDays;
	}

	void SetRecKeepDays(const u32 dwRecKeepDays)
	{
		m_dwRecKeepDays = dwRecKeepDays;
	}

	const u32 GetRecCoverThreasholdTime() const
	{
		return m_dwRecCoverThreasholdTime;
	}

	u32 GetRecCoverThreasholdTime()
	{
		return m_dwRecCoverThreasholdTime;
	}

	void SetRecCoverThreasholdTime(const u32 dwTime)
	{
		m_dwRecCoverThreasholdTime = dwTime;
	}

	const u32 GetRecCoverPermillage() const
	{
		return m_dwRecCoverPermillageOfPartRemainSize;
	}

	u32 GetRecCoverPermillage()
	{
		return m_dwRecCoverPermillageOfPartRemainSize;
	}

	void SetRecCoverPermillage(const u32 dwPermillage)
	{
		m_dwRecCoverPermillageOfPartRemainSize = dwPermillage;
	}

protected:
	s8                    m_szPartitionMountName[RSCOMM_MAXLEN_MOUNTNAME+1];
	u32                   m_dwRecKeepDays;                /*录像保留时间，该时间之内的录像文件，将不被覆盖，单位：天*/
	u32                   m_dwRecCoverThreasholdTime;     /*录像覆盖的触发时间，即分区可录像时长低于该值，则开始覆盖， 单位：秒*/
	u32                   m_dwRecCoverPermillageOfPartRemainSize;  /*千分比,磁盘剩余空间/总空间低于该千分比的值,则开始覆盖,该项只针对云存储有效*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_Num
{
public:
	TRSComm_Num()
	{
		memset(this, 0, sizeof(TRSComm_Num));
	}

	u32  GetNum()
	{
		return m_dwNum;
	}

	void SetNum(const u32 dwNum)
	{
		m_dwNum = dwNum;
	}

protected:
	u32       m_dwNum;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_QueryBaseCond
{
public:
	TRSComm_QueryBaseCond()
	{
		memset(this, 0, sizeof(TRSComm_QueryBaseCond));
	}

	u32 GetQueryStartIndex()
	{
		return m_dwQueryStartIndex;
	}

	void SetQueryStartIndex(const u32 dwIndex)
	{
		m_dwQueryStartIndex = dwIndex;
	}

	u32 GetExpectQueryNum()
	{
		return m_dwExpectQueryNum;
	}

	void SetExpectQueryNum(const u32 dwNum)
	{
		m_dwExpectQueryNum = dwNum;
	}

protected:
	u32               m_dwQueryStartIndex;
	u32               m_dwExpectQueryNum;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_QueryResultBaseInfo
{
public:
	TRSComm_QueryResultBaseInfo()
	{
		memset(this, 0, sizeof(TRSComm_QueryResultBaseInfo));
	}

	TRSComm_Num& GetTotalNum()
	{
		return m_tTotalNum;
	}

	void SetTotalNum(const u32 dwTotalNum)
	{
		m_tTotalNum.SetNum(dwTotalNum);
	}

	TRSComm_Num& GetCurPageTotalNum()
	{
		return m_tCurQueryPageTotalNum;
	}

	void SetCurPageTotalNum(const u32 dwTotalNum)
	{
		m_tCurQueryPageTotalNum.SetNum(dwTotalNum);
	}

protected:
	TRSComm_Num     m_tTotalNum;                     /*满足查询条件的总记录数，只在查询第一页时有效*/
	TRSComm_Num     m_tCurQueryPageTotalNum;         /*本次查询页的总记录数*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_QueryResultNtfHeader
{
public:
	TRSComm_QueryResultNtfHeader()
	{
		memset(this, 0, sizeof(TRSComm_QueryResultNtfHeader));
	}

	u16 GetCurNum()
	{
		return m_wCurNum;
	}

	void SetCurNum(const u16 wNum)
	{
		m_wCurNum = wNum;
	}
	
	BOOL32 IsFinished()
	{
		return m_bFinished;
	}

	void SetFinished(BOOL32 bFinished)
	{
		m_bFinished = bFinished;
	}
	
protected:
	u16                m_wCurNum;
	BOOL32             m_bFinished;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_PuChnBindingRelativeQueryCond
{
public:
	TRSComm_PuChnBindingRelativeQueryCond()
	{
		memset(this, 0, sizeof(TRSComm_PuChnBindingRelativeQueryCond));
	}

	TRSComm_DevID& GetNruID()
	{
		return m_tNruID;
	}

	void SetNruID(const TRSComm_DevID& tNruID)
	{
		m_tNruID = tNruID;
	}

	s8 * GetPartitionName()
	{
		return m_szPartitionName;
	}

	void SetPartitionName(const s8 * pszPartitionName)
	{
		if (NULL == pszPartitionName)
		{
			return;
		}

		s16 wCopyLen = strlen(pszPartitionName);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_MOUNTNAME ? wCopyLen : RSCOMM_MAXLEN_MOUNTNAME;
		strncpy(m_szPartitionName, pszPartitionName, wCopyLen);
		m_szPartitionName[wCopyLen] = '\0';
	}

protected:
	TRSComm_DevID     m_tNruID;
	s8                m_szPartitionName[RSCOMM_MAXLEN_MOUNTNAME+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_PuChnBindingRelative
{
public:
	TRSComm_PuChnBindingRelative()
	{
		memset(this, 0, sizeof(TRSComm_PuChnBindingRelative));
	}
	
	TRSComm_DevChn& GetPuChn()
	{
		return m_tPuChn;
	}

	void SetPuChn(const TRSComm_DevChn& tPuChn)
	{
		m_tPuChn = tPuChn;
	}
	
	TRSComm_DevID& GetNruID()
	{
		return m_tNruID;
	}

	void SetNruID(const TRSComm_DevID& tNruID)
	{
		m_tNruID = tNruID;
	}
	
	s8 * GetBindingPartitionName()
	{
		return m_szBindingPartition;
	}

	void SetBindingPartitionName(const s8* pszPartitionName)
	{
		if (NULL == pszPartitionName)
		{
			memset(m_szBindingPartition, 0, sizeof(m_szBindingPartition));
			return;
		}

		s16 wCopyLen = strlen(pszPartitionName);
		wCopyLen = rsmin(wCopyLen, RSCOMM_MAXLEN_MOUNTNAME);
		strncpy(m_szBindingPartition, pszPartitionName, wCopyLen);
		m_szBindingPartition[wCopyLen] = '\0';
	}
	
protected:
	TRSComm_DevChn      m_tPuChn;
	TRSComm_DevID       m_tNruID;     /*绑定的磁盘分区所属的nru*/
	s8                  m_szBindingPartition[RSCOMM_MAXLEN_MOUNTNAME+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_PuChnBindingHeader
{
public:
	TRSComm_PuChnBindingHeader()
	{
		memset(this, 0, sizeof(TRSComm_PuChnBindingHeader));
	}
	
	TRSComm_DevID& GetNruID()
	{
		return m_tNruID;
	}

	void SetNruID(const TRSComm_DevID& tNruID)
	{
		m_tNruID = tNruID;
	}

	s8 * GetPartitionName()
	{
		return m_szPartitionName;
	}

	void SetPartitionName(const s8* pszPartitionName)
	{		
		if (NULL == pszPartitionName)
		{
			return;
		}
		
		s16 wCopyLen = strlen(pszPartitionName);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_MOUNTNAME ? wCopyLen : RSCOMM_MAXLEN_MOUNTNAME;
		strncpy(m_szPartitionName, pszPartitionName, wCopyLen);
		m_szPartitionName[wCopyLen] = '\0';
	}

	u16  GetBindingPuChnNum()
	{
		return m_wBindingPuChnNum;
	}

	void SetBindingPuChnNum(const u16 wNum)
	{
		m_wBindingPuChnNum = wNum;
	}

protected:
	TRSComm_DevID     m_tNruID;
	s8                m_szPartitionName[RSCOMM_MAXLEN_MOUNTNAME+1];
	u16               m_wBindingPuChnNum;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct  TRSComm_PuChnUnBindingHeader
{
public:
	TRSComm_PuChnUnBindingHeader()
	{
		memset(this, 0, sizeof(TRSComm_PuChnUnBindingHeader));
	}

	ERSComm_OperStyle GetOperStyle()
	{
		return m_eOperStyle;
	}

	void SetOperStyle(const ERSComm_OperStyle& eOperStyle)
	{
		m_eOperStyle = eOperStyle;
	}

	u16 GetUnBindingPuChnNum()
	{
		return m_wUnBindingPuChnNum;
	}

	void SetUnBindingPuChnNum(const u16 wNum)
	{
		m_wUnBindingPuChnNum = wNum;
	}

	const s8* GetOperStyleDesc()
	{
		switch(m_eOperStyle)
		{
		case RSCOMM_OPERSTYLE_NORMAL:
			return "Normal";
		case RSCOMM_OPERSTYLE_CHECKOUT:
			return "CheckOut";
		case RSCOMM_OPERSTYLE_UNKNOWN:
			return "Unknown";
		}

		return "Unknown";
	}

protected:
	ERSComm_OperStyle m_eOperStyle;
	u16               m_wUnBindingPuChnNum;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_VideoResolution
{
public:
	TRSComm_VideoResolution()
	{
		memset(this, 0, sizeof(TRSComm_VideoResolution));
	}

	const s32 GetWidth() const
	{
		return m_nWidth;
	}
	s32 GetWidth()
	{
		return m_nWidth;
	}

	void SetWidth(const s32 nWidth)
	{
		m_nWidth = nWidth;
	}

	const s32 GetHeight() const
	{
		return m_nHeight;
	}

	s32 GetHeight()
	{
		return m_nHeight;
	}
	void SetHeight(const s32 nHeight)
	{
		m_nHeight = nHeight;
	}

protected:
	s32   m_nWidth;                                   /*图像宽*/
	s32   m_nHeight;                                  /*图像高*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_VideoEncParam
{
public:
	TRSComm_VideoEncParam()
	{
		memset(this, 0, sizeof(TRSComm_VideoEncParam));
	}

	const u8 GetPayloadType() const
	{
		return m_byPayloadType;
	}

	void SetPayloadType(const u8 byPayloadType)
	{
		m_byPayloadType = byPayloadType;
	}

	const s8 * GetEncodingName() const
	{
		return m_szEncodingName;
	}

	void SetEncodingName(const s8* pszEncodingName)
	{
		if (NULL == pszEncodingName)
		{
			return;
		}

		s16 wNameLen = strlen(pszEncodingName);
		wNameLen = wNameLen <= RSCOMM_MAXLEN_ENCODINGNAME ? wNameLen : RSCOMM_MAXLEN_ENCODINGNAME;
		strncpy(m_szEncodingName, pszEncodingName, wNameLen);
		m_szEncodingName[wNameLen] = '\0';
	}

	const TRSComm_VideoResolution& GetVideoResolution() const
	{
		return m_tVideoResolution;
	}
	TRSComm_VideoResolution& GetVideoResolution()
	{
		return m_tVideoResolution;
	}

	void SetVideoResolution(const TRSComm_VideoResolution& tVideoResolution)
	{
		memcpy(&m_tVideoResolution, &tVideoResolution, sizeof(TRSComm_VideoResolution));
	}

	const s32 GetVideoFrameRate() const
	{
		return m_nVideoFrameRate;
	}

	s32 GetVideoFrameRate()
	{
		return m_nVideoFrameRate;
	}

	void SetVideoFrameRate(const s32 nFrameRate)
	{
		m_nVideoFrameRate = nFrameRate;
	}

	const s32 GetVideoBitRate() const
	{
		return m_nVideoBitRate;
	}

	s32 GetVideoBitRate() 
	{
		return m_nVideoBitRate;
	}

	void SetVideoBitRate(const s32 nBitRate)
	{
		m_nVideoBitRate = nBitRate;
	}

	const s32 GetKeyFrameIntvl() const
	{
		return m_nVideoKeyFrameIntvl;
	}

	s32 GetKeyFrameIntvl()
	{
		return m_nVideoKeyFrameIntvl;
	}

	void SetKeyFrameIntvl(const s32 nKeyFrameIntvl)
	{
		m_nVideoKeyFrameIntvl = nKeyFrameIntvl;
	}

	const s32 GetClockRate() const
	{
		return m_nClockRate;
	}

	s32 GetClockRate()
	{
		return m_nClockRate;
	}

	void SetClockRate(const s32 nClockRate)
	{
		m_nClockRate = nClockRate;
	}

protected:
	u8					     m_byPayloadType;               /*载荷类型*/
	s8                       m_szEncodingName[RSCOMM_MAXLEN_ENCODINGNAME+1];    /*编码名称 rfc3551*/
	TRSComm_VideoResolution  m_tVideoResolution;            /*视频分辨率*/
	s32                      m_nVideoFrameRate;             /*码流帧率*/
	s32                      m_nVideoBitRate;               /*码流比特率*/
	s32                      m_nVideoKeyFrameIntvl;         /*码流的关键帧间隔*/
	s32                      m_nClockRate;                  /*时间戳采样率， 1000 or 90000*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_AudioEncParam
{
public:
	TRSComm_AudioEncParam()
	{
		memset(this, 0, sizeof(TRSComm_AudioEncParam));
	}

	u8 GetPayloadType()
	{
		return m_byPayloadType;
	}

	void SetPayloadType(const u8 byPayloadType)
	{
		m_byPayloadType = byPayloadType;
	}

	s8 * GetEncodingName()
	{
		return m_szEncodingName;
	}

	void SetEncodingName(const s8* pszEncodingName)
	{
		if (NULL == pszEncodingName)
		{
			return;
		}

		s16 wNameLen = strlen(pszEncodingName);
		wNameLen = wNameLen <= RSCOMM_MAXLEN_ENCODINGNAME ? wNameLen : RSCOMM_MAXLEN_ENCODINGNAME;
		strncpy(m_szEncodingName, pszEncodingName, wNameLen);
		m_szEncodingName[wNameLen] = '\0';
	}

	s32 GetSampleRate()
	{
		return m_nSampleRate;
	}

	void SetSampleRate(const s32 nSampleRate)
	{
		m_nSampleRate = nSampleRate;
	}

	s32 GetChannelsNum()
	{
		return m_nChannelsNum;
	}

	void SetChannelsNum(const s32 nChannelsNum)
	{
		m_nChannelsNum = nChannelsNum;
	}

	s32 GetClockRate()
	{
		return m_nClockRate;
	}

	void SetClockRate(s32 nClockRate)
	{
		m_nClockRate = nClockRate;
	}

protected:
	u8                  m_byPayloadType;              /*载荷类型*/
	s8                  m_szEncodingName[RSCOMM_MAXLEN_ENCODINGNAME+1];   /*编码名称*/
	s32                 m_nSampleRate;                  /*真实采样率*/
	s32                 m_nClockRate;                   /*时间戳采样率*/
	s32                 m_nChannelsNum;                 /*声道数*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_Handle
{
public:
	TRSComm_Handle()
	{
		memset(this, 0, sizeof(TRSComm_Handle));
		m_nResourceID = -1;
	}

	u32 GetTaskID()
	{
		return m_dwTaskID;
	}

	const u32 GetTaskID() const
	{
		return m_dwTaskID;
	}

	void SetTaskID(const u32 dwTaskID)
	{
		m_dwTaskID = dwTaskID;
	}
	
	TRSComm_DevChn& GetPuChn()
	{
		return m_tPuChn;
	}

	const TRSComm_DevChn& GetPuChn() const
	{
		return m_tPuChn;
	}

	void SetPuChn(const TRSComm_DevChn& tPuChn)
	{
		m_tPuChn = tPuChn;
	}
	
	TRSComm_DevID& GetNruID()
	{
		return m_tNruID;
	}

	const TRSComm_DevID& GetNruID() const
	{
		return m_tNruID;
	}

	void SetNruID(const TRSComm_DevID& tNruID)
	{
		m_tNruID = tNruID;
	}
	
	s32 GetResourceID()
	{
		return m_nResourceID;
	}

	const s32 GetResourceID() const
	{
		return m_nResourceID;
	}

	void SetResourceID(const s32 nResourceID)
	{
		m_nResourceID = nResourceID;
	}
	
protected:
	u32             m_dwTaskID;
	TRSComm_DevChn  m_tPuChn;
	TRSComm_DevID   m_tNruID;
	s32             m_nResourceID;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecordParam : public TRSComm_Handle
{
public:
	TRSComm_RecordParam()
	{
		memset(this, 0, sizeof(TRSComm_RecordParam));
	}

	ERSComm_RecState GetRecState()
	{
		return m_eRecState;
	}

	const ERSComm_RecState GetRecState() const 
	{
		return m_eRecState;
	}

	void SetRecState(const ERSComm_RecState eRecState)
	{
		m_eRecState = eRecState;
	}

	const ERSComm_RecType GetRecType() const
	{
		return m_eRecType;
	}

	void SetRecType(const ERSComm_RecType eRecType)
	{
		m_eRecType = eRecType;
	}

	ERSComm_RecMode GetRecMode()
	{
		return m_eRecMode;
	}

	void SetRecMode(const ERSComm_RecMode eRecMode)
	{
		m_eRecMode = eRecMode;
	}

	TRSComm_DevChn& GetDevChn()
	{
		return m_tDevChn;
	}

	const TRSComm_DevChn& GetDevChn() const
	{
		return m_tDevChn;
	}

	void SetDevChn(const TRSComm_DevChn& tDevChn)
	{
		m_tDevChn = tDevChn;
	}

	BOOL32 IsBlindRecord()
	{
		return m_bBlindRecord;
	}

	void   SetBlindRecord(BOOL32 bBlindRecord)
	{
		m_bBlindRecord = bBlindRecord;
	}

	BOOL32 IsSupportResend()
	{
		return m_bSupportResend;
	}

	void SetResend(BOOL32 bSupportResend)
	{
		m_bSupportResend = bSupportResend;
	}

	const TRSComm_VideoEncParam& GetVideoEncParam() const
	{
		return m_tVideoEncParam;
	}

	void SetVideoEncParam(const TRSComm_VideoEncParam& tVideoEncParam)
	{
		memcpy(&m_tVideoEncParam, &tVideoEncParam, sizeof(TRSComm_VideoEncParam));
	}

	TRSComm_AudioEncParam& GetAudioEncParam()
	{
		return m_tAudioEncParam;
	}

	void SetAudioEncParam(const TRSComm_AudioEncParam& tAudioEncParam)
	{
		memcpy(&m_tAudioEncParam, &tAudioEncParam, sizeof(TRSComm_AudioEncParam));
	}

	u32 GetPreRecTime()
	{
		return m_dwPreRecTime;
	}

	void SetPreRecTime(const u32 dwPreRecTime)
	{
		m_dwPreRecTime = dwPreRecTime;
	}

protected:
	ERSComm_RecState          m_eRecState;        /*录像状态：预录 or 录像*/
	ERSComm_RecType           m_eRecType;         /*录像类型：告警录像 or 非告警录像*/
	ERSComm_RecMode           m_eRecMode;         /*录像模式：视频 or 音频频*/
	TRSComm_DevChn            m_tDevChn;
	BOOL32                    m_bBlindRecord;     /*是否盲录，盲录即不解析码流，直接写磁盘*/
	BOOL32                    m_bSupportResend;   /*是否支持重传*/
	TRSComm_VideoEncParam     m_tVideoEncParam;   /*视频通道编码参数配置*/
	TRSComm_AudioEncParam     m_tAudioEncParam;   /*音频通道编码参数配置*/
	u32                       m_dwPreRecTime;     /*预录时长*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_MediaInfoExt
{
public:
	TRSComm_MediaInfoExt()
	{
		memset(this, 0, sizeof(TRSComm_MediaInfoExt));
	}

	u16  GetMediaInfoDataLen()
	{
		return m_wMediaInfoDataLen;
	}

	void SetMediaInfoDataLen(const u16 wLen)
	{
		m_wMediaInfoDataLen = wLen;
	}

	u8 * GetMediaInfoData()
	{
		return m_abyMediaInfoData;
	}

	void GetMediaInfoData(u8 *pbyData, u16 *pwDataLen)
    {
        if(pbyData == NULL) return;
        if(pwDataLen == NULL) return;
		
        *pwDataLen = GetMediaInfoDataLen();
        memcpy(pbyData, m_abyMediaInfoData, *pwDataLen);
    }

	void SetMediaInfoData(u8* pbyMediaInfoData, u16 wDataLen)
	{
		u16 wCopyLen = RSCOMM_MAXLEN_MEDIAINFOEXT >= wDataLen ? wDataLen : RSCOMM_MAXLEN_MEDIAINFOEXT;
		memcpy(m_abyMediaInfoData, pbyMediaInfoData, wCopyLen);
		SetMediaInfoDataLen(wCopyLen);
	}

	u32 GetStructRealLen()
    {
        return (sizeof(m_wMediaInfoDataLen) + GetMediaInfoDataLen());
    }

protected:
	u16                 m_wMediaInfoDataLen;                               /*媒体扩展信息实际长度*/
	u8                  m_abyMediaInfoData[RSCOMM_MAXLEN_MEDIAINFOEXT];    /*媒体扩展信息*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_MediaInfoDescription
{
public:
	TRSComm_MediaInfoDescription()
	{
		memset(this, 0, sizeof(TRSComm_MediaInfoDescription));
	}

	u8  GetAudioStreamNum()
	{
		return m_byAudioStreamNum;
	}

	void SetAudioStreamNum(const u8 byAudioStreamNum)
	{
		m_byAudioStreamNum = byAudioStreamNum;
	}

	u8  GetVideoStreamNum()
	{
		return m_byVideoStreamNum;
	}
	
	void SetVideoStreamNum(const u8 byVideoStreamNum)
	{
		m_byVideoStreamNum = byVideoStreamNum;
	}

	BOOL32 IsExistMediaExtensionInfo()
	{
		return m_bExistMediaExtInfo;
	}

	void SetExistMediaExtensionInfoFlag(BOOL32 bExist)
	{
		m_bExistMediaExtInfo = bExist;
	}

	void SetStartTime(u32 dwStartTime)
	{
		m_dwStartTime = dwStartTime;
	}

	u32 GetStartTime()
	{
		return m_dwStartTime;
	}
protected:
	u8           m_byAudioStreamNum;
	u8           m_byVideoStreamNum;
	BOOL32       m_bExistMediaExtInfo;
	u32          m_dwStartTime;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecordTask : public TRSComm_RecordParam
{
	TRSComm_RecordTask()
	{
		memset(this, 0, sizeof(TRSComm_RecordTask));
	}

	s8 * GetRecFileName()
	{
		return m_szRecFileName;
	}

	const s8 * GetRecFileName() const
	{
		return m_szRecFileName;
	}

	void SetRecFileName(const s8 * pszRecFileName)
	{
		if (NULL == pszRecFileName)
		{
			return;
		}

		s16 wNameLen = strlen(pszRecFileName);
		wNameLen = wNameLen <= RSCOMM_MAXLEN_FILENAME ? wNameLen : RSCOMM_MAXLEN_FILENAME;
		strncpy(m_szRecFileName, pszRecFileName, wNameLen);
		m_szRecFileName[wNameLen] = '\0';
	}

	s8 * GetRecFilePath()
	{
		return m_szRecFilePath;
	}

	const s8 * GetRecFilePath() const 
	{
		return m_szRecFilePath;
	}

	void SetRecFilePath(const s8 * pszRecFilePath)
	{
		if (NULL == pszRecFilePath)
		{
			return;
		}

		s16 wNameLen = strlen(pszRecFilePath);
		wNameLen = wNameLen <= RSCOMM_MAXLEN_FILEPATH ? wNameLen : RSCOMM_MAXLEN_FILEPATH;
		strncpy(m_szRecFilePath, pszRecFilePath, wNameLen);
		m_szRecFilePath[wNameLen] = '\0';
	}

protected:
	s8                  m_szRecFileName[RSCOMM_MAXLEN_FILENAME+1];
	s8                  m_szRecFilePath[RSCOMM_MAXLEN_FILEPATH+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecordStatus : public TRSComm_RecordTask
{
public:
	TRSComm_RecordStatus()
	{
		memset(this, 0, sizeof(TRSComm_RecordStatus));
	}

	u32 GetRecCurSize()
	{
		return m_dwRecCurSize;
	}

	void SetRecCurSize(const u32 dwRecSize)
	{
		m_dwRecCurSize = dwRecSize;
	}

	u32 GetRecDuration()
	{
		return m_dwRecDuration;
	}

	void SetRecDuration(const u32 dwDuration)
	{
		m_dwRecDuration = dwDuration;
	}

	u32 GetRecStartTime()
	{
		return m_dwStartTime;
	}

	void SetRecStartTime(const u32 dwStartTime)
	{
		m_dwStartTime = dwStartTime;
	}

protected:
	u32 m_dwRecCurSize;
	u32 m_dwRecDuration;
	u32 m_dwStartTime;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecTypeChange
{
public:
	TRSComm_RecTypeChange()
	{
		memset(this, 0, sizeof(TRSComm_RecTypeChange));
	}

	ERSComm_RecType GetRecType()
	{
		return m_eRecType;
	}

	void SetRecType(const ERSComm_RecType eRecType)
	{
		m_eRecType = eRecType;
	}

	u32 GetRecTypeStartTime()
	{
		return m_dwRecTypeStartTime;
	}

	const u32 GetRecTypeStartTime() const
	{
		return m_dwRecTypeStartTime;
	}

	void SetRecTypeStartTime(const u32 dwTime)
	{
		m_dwRecTypeStartTime = dwTime;
	}

	u32 GetRecTypeEndTime()
	{
		return m_dwRecTypeEndTime;
	}
	
	void SetRecTypeEndTime(const u32 dwTime)
	{
		m_dwRecTypeEndTime = dwTime;
	}

protected:
	ERSComm_RecType m_eRecType;
	u32             m_dwRecTypeStartTime;
	u32             m_dwRecTypeEndTime;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_IPAddress
{
public:
	TRSComm_IPAddress()
	{
		memset(this, 0, sizeof(TRSComm_IPAddress));
	}

	ERSComm_IPType GetIpType()
	{
		return m_eIpType;
	}

	void SetIpType(const ERSComm_IPType eIpType)
	{
		m_eIpType = eIpType;
	}

	s8 * GetAddress()
	{
		return m_szAddress;
	}

	void SetAddress(const s8* pszAddress)
	{
		if (NULL == pszAddress)
		{
			return ;
		}

		s16 wAddressLen = strlen(pszAddress);
		wAddressLen = wAddressLen <= RSCOMM_MAXLEN_IPADDRESS ? wAddressLen : RSCOMM_MAXLEN_IPADDRESS;
		strncpy(m_szAddress, pszAddress, wAddressLen);
		m_szAddress[wAddressLen] = '\0';
	}

	u16 GetPrefixLen()
	{
		return m_wPrefixLength;
	}

	void SetPrefixLen(const u16 wPrefixLen)
	{
		m_wPrefixLength = wPrefixLen;
	}

	void SetPrefixLen(const s8 * pszNetMask)
	{
		if (NULL == pszNetMask)
		{
			return;
		}

		if (NULL != strchr(pszNetMask, '.'))
		{
			//IPV4子网掩码
			u32 dwIpAddr = inet_addr(pszNetMask);
			u16 wPrefixLen = 0;

			while(wPrefixLen < 32)
			{
				if (0 == ((dwIpAddr >> wPrefixLen) & 0xFFFFFFFF))
				{
					break;
				}
				wPrefixLen++;
			}

			m_wPrefixLength = wPrefixLen;
		}

	}

protected:
	ERSComm_IPType    m_eIpType;
	s8                m_szAddress[RSCOMM_MAXLEN_IPADDRESS+1];
	u16               m_wPrefixLength;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct  TRSComm_NetAddr
{
public:
	TRSComm_NetAddr()
	{
		memset(this, 0, sizeof(TRSComm_NetAddr));
	}

	TRSComm_IPAddress& GetIPAddr()
	{
		return m_tIP;
	}

	void SetIPAddr(TRSComm_IPAddress& tIP)
	{
		memcpy(&m_tIP, &tIP, sizeof(TRSComm_IPAddress));
	}

	const u16 GetPort()
	{
		return m_wPort;
	}

	
	void SetPort(u16 wPort)
	{
		m_wPort = wPort;
	}
protected:
	TRSComm_IPAddress  m_tIP;
	u16                m_wPort;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_NetWork
{
public:
	TRSComm_NetWork()
	{
		memset(this, 0, sizeof(TRSComm_NetWork));
	}

	const s8* GetIfName()
	{
		return m_szIfName;
	}

	void  SetIfName(const s8* szName)
	{
		if (NULL == szName)
		{
			return;
		}

		u16 wLen = strlen(szName);
		wLen = RSCOMM_MAXLEN_IFNAME <= wLen ? RSCOMM_MAXLEN_IFNAME : wLen;
		strncpy(m_szIfName, szName, wLen);
		m_szIfName[wLen] = '\0';
	}

	TRSComm_IPAddress& GetNruAddr()
	{
		return m_tNruAddr;
	}

	void SetNruAddr(const TRSComm_IPAddress& tNruAddr)
	{
		memcpy(&m_tNruAddr, &tNruAddr, sizeof(TRSComm_IPAddress));
	}

	TRSComm_IPAddress& GetNruGateWay()
	{
		return m_tNruGateWay;
	}

	void SetNruGateWay(const TRSComm_IPAddress& tGateWay)
	{
		memcpy(&m_tNruGateWay, &tGateWay, sizeof(TRSComm_IPAddress));
	}

protected:
	s8                       m_szIfName[RSCOMM_MAXLEN_IFNAME+1];
	TRSComm_IPAddress        m_tNruAddr;                           /*nru的IP地址*/
	TRSComm_IPAddress        m_tNruGateWay;                        /*nru的网关地址*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_Port
{
public:
	TRSComm_Port()
	{
		memset(this, 0, sizeof(TRSComm_Port));
	}

	u16   GetPort() const
	{
		return ntohs(m_wPort);
	}

	void  SetPort(const u16 wPort)
	{
		m_wPort = htons(wPort);
	}

protected:
	u16        m_wPort;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct  TRSComm_PunchKey
{
public:
	TRSComm_PunchKey()
	{
		memset(this, 0, sizeof(TRSComm_PunchKey));
	}

	s8* GetPunchKey()
	{
		return m_szPunchKey;
	}

	u16 GetPunchKeyLength()
	{
		return m_wLength;
	}

	BOOL32  SetPunchKey(const s8* szPunchKey)
	{
		if (NULL == szPunchKey)
		{
			return FALSE;
		}
				
		s16 wLength = strlen(szPunchKey);
		wLength = wLength <= RSCOMM_MAXLEN_PUNCHKEY ? wLength : RSCOMM_MAXLEN_PUNCHKEY;
		strncpy(m_szPunchKey, szPunchKey, wLength);
		m_szPunchKey[wLength] = '\0';
		m_wLength = wLength;
		return TRUE;
	}
protected:
	s8                m_szPunchKey[RSCOMM_MAXLEN_PUNCHKEY+1];
	u16               m_wLength;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct  TRSComm_ChannelAddr
{
public:
	TRSComm_ChannelAddr()
	{
		memset(this, 0, sizeof(TRSComm_ChannelAddr));
	}

	void SetChannelType(ERSComm_ChannelType eChannelType)
	{
		m_eChannelType = eChannelType;
	}

	ERSComm_ChannelType GetChannelType()
	{
		return m_eChannelType;
	}

	void SetPunchKey(const TRSComm_PunchKey& tPunchKey)
	{
		memcpy(&m_tPunchKey, &tPunchKey, sizeof(TRSComm_PunchKey));
	}

	TRSComm_PunchKey& GetPunchKey()
	{
		return m_tPunchKey;
	}

	void SetAddrNum(u16 wAddrNum)
	{
		if (wAddrNum > RSCOMM_MAXNUM_ADDR)
		{
			//OspPrintf(TRUE, FALSE, "warning! SetAddrNum (%d) error!\n", wAddrNum);
			wAddrNum = RSCOMM_MAXNUM_ADDR;
		}
		m_wAddrNum = wAddrNum;
	};

	u16 GetAddrNum()
	{
		return m_wAddrNum;
	}

	void SetNetAddr(u16 wAddrNum, TRSComm_NetAddr& tAddr)
	{
		if (wAddrNum > RSCOMM_MAXNUM_ADDR)
		{
			//OspPrintf(TRUE, FALSE, "wAddrNum(%d) > RSCOMM_MAXNUM_ADDR \n", wAddrNum);
			return;
		}
		memcpy(&m_atAddrList[wAddrNum], &tAddr, sizeof(TRSComm_NetAddr));
	}

	TRSComm_NetAddr& GetNetAddr(u16 wAddrNum)
	{
		if (wAddrNum > RSCOMM_MAXNUM_ADDR)
		{
			return m_atAddrList[0];
		}
		return m_atAddrList[wAddrNum];
	}
	
protected:
	ERSComm_ChannelType  m_eChannelType;
	TRSComm_PunchKey     m_tPunchKey;
	u16                  m_wAddrNum;
	TRSComm_NetAddr      m_atAddrList[RSCOMM_MAXNUM_ADDR];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_MediaAddr
{
public:
	TRSComm_MediaAddr()
	{
		memset(this, 0, sizeof(TRSComm_MediaAddr));
	}

	void SetMediaType(ERSComm_MediaType eMediaType)
	{
		m_eMediaType = eMediaType;
	};

	ERSComm_MediaType GetMediaType()
	{
		return m_eMediaType;
	};

	void SetChannelNum(u16 wChannelNum)
	{
		m_wNum = wChannelNum;
	};

	u16 GetChannelNum()
	{
		return m_wNum;
	}

	void SetChannelAddrList(u16 wAddrNum, TRSComm_ChannelAddr& tChannelAddr)
	{
		if (wAddrNum > RSCOMM_MAXNUM_CHANNELADDR)
		{
			return;
		}
		memcpy(&m_atChannelAddrList[wAddrNum], &tChannelAddr, sizeof(TRSComm_ChannelAddr));
	}
	
	TRSComm_ChannelAddr& GetChannelAddr(u16 wAddrNum)
	{
		if (wAddrNum > RSCOMM_MAXNUM_CHANNELADDR)
		{
			return m_atChannelAddrList[0];
		}
		return m_atChannelAddrList[wAddrNum];
	}

protected:
	ERSComm_MediaType   m_eMediaType;
	u16                 m_wNum;
	TRSComm_ChannelAddr m_atChannelAddrList[RSCOMM_MAXNUM_CHANNELADDR];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_TransParamHeader
{
public:
	TRSComm_TransParamHeader()
	{
		memset(this, 0 , sizeof(TRSComm_TransParamHeader));
	}

	ERSComm_TransportProtocol GetTransportProtocol()
	{
		return m_eTransportProtocol;
	}

	void SetTransportProtocol(const ERSComm_TransportProtocol eTransportProtocol)
	{
		m_eTransportProtocol = eTransportProtocol;
	}

	ERSComm_StreamType GetStreamType()
	{
		return m_eStreamType;
	}

	void SetStreamType(const ERSComm_StreamType eStreamType)
	{
		m_eStreamType = eStreamType;
	}

	ERSComm_TransChnType GetTransChnType()
	{
		return m_eChannelType;
	}

	void SetTransChnType(const ERSComm_TransChnType eTransChnType)
	{
		m_eChannelType = eTransChnType;
	}

	void SetIpAddrNum(const u16 wNum)
	{
		m_wIpAddrNum = wNum;
	}

	const u16 GetIpAddrNum()
	{
		return m_wIpAddrNum;
	}

protected:
	ERSComm_TransportProtocol m_eTransportProtocol;
	ERSComm_StreamType        m_eStreamType;
	ERSComm_TransChnType      m_eChannelType;
	u16                       m_wIpAddrNum;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_TimeRange
{
public:
	TRSComm_TimeRange()
	{
		memset(this, 0 , sizeof(TRSComm_TimeRange));
	}

	const u32 GetStartTime() const
	{
		return m_dwStartTime;
	}

	u32 GetStartTime()
	{
		return m_dwStartTime;
	}

	void SetStartTime(const u32 dwStartTime)
	{
		m_dwStartTime = dwStartTime;
	}

	const u32 GetEndTime() const
	{
		return m_dwEndTime;
	}

	u32 GetEndTime() 
	{
		return m_dwEndTime;
	}

	void SetEndTime(const u32 dwEndTime)
	{
		m_dwEndTime = dwEndTime;
	}

protected:
	u32               m_dwStartTime;
	u32               m_dwEndTime;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_ExtraChn
{
    u8               m_byExtraChnNum;                            /*辅流数量*/
    s16              m_wChannel[RSCOMM_MAX_EXTRA_CHN_NUM];       /*辅流通道*/
}
#ifndef WIN32
    __attribute__ ((packed))
#endif
    ;;

struct TRSComm_RecQueryCond
{
public:
	TRSComm_RecQueryCond()
	{
		memset(this, 0, sizeof(TRSComm_RecQueryCond));
	}

	BOOL32 IsQueryByRecType()
	{
		return (m_wQueryCond & (0x01 << 0));
	}
	void QueryByRecType(BOOL32 bQueryByRecType)
	{
		if(bQueryByRecType) 
        {
            m_wQueryCond |= (0x1 << 0); 
        }
        else 
        {
            m_wQueryCond &= ~(0x1 << 0); 
        }
	}

	BOOL32 IsQueryByTimeRange()
	{
		return (m_wQueryCond & (0x01 << 1));
	}

	void QueryByTimeRange(BOOL32 bQueryByTimeRange)
	{
		if (bQueryByTimeRange)
		{
			m_wQueryCond |= (0x01 << 1);
		}
		else
		{
			m_wQueryCond &= ~(0x01 << 1);
		}
	}

	TRSComm_DevChn& GetPuChn()
	{
		return m_tPuChn;
	}

	void SetPuChn(const TRSComm_DevChn& tPuChn)
	{
		m_tPuChn = tPuChn;
	}

	ERSComm_RecType GetRecType()
	{
		return m_eRecType;
	}

	void SetRecType(const ERSComm_RecType eRecType)
	{
		m_eRecType = eRecType;
	}

	TRSComm_TimeRange& GetTimeRange()
	{
		return m_tTimeRange;
	}

	void SetTimeRange(const TRSComm_TimeRange& tTimeRange)
	{
		memcpy(&m_tTimeRange, &tTimeRange, sizeof(TRSComm_TimeRange));
	}

	//{{{{{{{{{{{{{{{ add by Royan 20140320
	ERSComm_RecLockStat GetRecLockStat()
	{
		return m_tRecLockStat;
	}

	void SetRecLockStat(const ERSComm_RecLockStat eRecLockType)
	{
		m_tRecLockStat = eRecLockType;
	}
	//}}}}}}}}}}}}}}} add by Royan 20140320 

    const TRSComm_ExtraChn &GetExtraChn() const
    {
        return m_tExtraChn;
    }

    TRSComm_ExtraChn &GetExtraChn()
    {
        return m_tExtraChn;
    }

    void SetExtraChn(const TRSComm_ExtraChn tExtraChn)
    {
        m_tExtraChn = tExtraChn;
    }

protected:
	u16                   m_wQueryCond;          
	TRSComm_DevChn        m_tPuChn;
	ERSComm_RecType       m_eRecType;
	TRSComm_TimeRange     m_tTimeRange;
	ERSComm_RecLockStat   m_tRecLockStat;
    TRSComm_ExtraChn      m_tExtraChn;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecFileSubTimeRangeInfo
{
public:
	TRSComm_RecFileSubTimeRangeInfo()
	{
		memset(this, 0, sizeof(TRSComm_RecFileSubTimeRangeInfo));
	}

	u32  GetSubRecID()
	{
		return m_dwSubRecID;
	}

	void SetSubRecID(const u32 dwRecID)
	{
		m_dwSubRecID = dwRecID;
	}
	
	ERSComm_RecType GetRecType()
	{
		return m_eRecType;
	}

	void SetRecType(const ERSComm_RecType eRecType)
	{
		m_eRecType = eRecType;
	}

	void SetRecType(const u8 byRecType)
	{
		if (byRecType == RSCOMM_RECTYPE_ALARM)
		{
			m_eRecType = RSCOMM_RECTYPE_ALARM;
		}
		else if (byRecType == RSCOMM_RECTYPE_HANDLE)
		{
			m_eRecType = RSCOMM_RECTYPE_HANDLE;
		}
		else if (byRecType == RSCOMM_RECTYPE_PLAN)
		{
			m_eRecType = RSCOMM_RECTYPE_PLAN;
		}
	}
	
	TRSComm_TimeRange& GetTimeRange()
	{
		return m_tTimeRange;
	}

	void SetTimeRange(const TRSComm_TimeRange& tTimeRange)
	{
		memcpy(&m_tTimeRange, &tTimeRange, sizeof(TRSComm_TimeRange));
	}
	
protected:
	u32                 m_dwSubRecID;
	ERSComm_RecType     m_eRecType;               /*录像类型*/
	TRSComm_TimeRange   m_tTimeRange;             /*录像时间段*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;


struct TRSComm_RecFileSchema
{
public:
	TRSComm_RecFileSchema()
	{
		memset(this, 0, sizeof(TRSComm_RecFileSchema));
	}

	u32 GetRecID()
	{
		return m_dwRecID;
	}

	void SetRecID(const u32 dwRecID)
	{
		m_dwRecID = dwRecID;
	}
	
	s8 * GetFilePath()
	{
		return m_szFilePath;
	}

	const s8 * GetFilePath() const
	{
		return m_szFilePath;
	}

	void SetFilePath(const s8* pszFilePath)
	{
		if (NULL == pszFilePath)
		{
			return ;
		}

		s16 wPathLen = strlen(pszFilePath);
		wPathLen = wPathLen <= RSCOMM_MAXLEN_FILEPATH ? wPathLen : RSCOMM_MAXLEN_FILEPATH;
		strncpy(m_szFilePath, pszFilePath, wPathLen);
		m_szFilePath[wPathLen] = '\0';
	}

	s8 * GetFileName()
	{
		return m_szFileName;
	}

	const s8 * GetFileName() const
	{
		return m_szFileName;
	}

	void SetFileName(const s8* pszFileName)
	{
		if (NULL == pszFileName)
		{
			return ;
		}
		
		s16 wFileLen = strlen(pszFileName);
		wFileLen = wFileLen <= RSCOMM_MAXLEN_FILENAME ? wFileLen : RSCOMM_MAXLEN_FILENAME;
		strncpy(m_szFileName, pszFileName, wFileLen);
		m_szFileName[wFileLen] = '\0';
	}

	ERSComm_RecType GetRecType()
	{
		return m_eRecType;
	}
	
	void SetRecType(const ERSComm_RecType eRecType)
	{
		m_eRecType = eRecType;
	}
	
	void SetRecType(const u8 byRecType)
	{
		if (byRecType == RSCOMM_RECTYPE_ALARM)
		{
			m_eRecType = RSCOMM_RECTYPE_ALARM;
		}
		else if (byRecType == RSCOMM_RECTYPE_HANDLE)
		{
			m_eRecType = RSCOMM_RECTYPE_HANDLE;
		}
		else if (byRecType == RSCOMM_RECTYPE_PLAN)
		{
			m_eRecType = RSCOMM_RECTYPE_PLAN;
		}
	}

	TRSComm_TimeRange& GetTimeRange()
	{
		return m_tTimeRange;
	}

	void SetTimeRange(const TRSComm_TimeRange& tTimeRange)
	{
		memcpy(&m_tTimeRange, &tTimeRange, sizeof(TRSComm_TimeRange));
	}

	u32 GetRecSize()
	{
		return m_dwRecSize;
	}

	void SetRecSize(const u32 dwRecSize)
	{
		m_dwRecSize = dwRecSize;
	}

	s8 * GetFileAlias()
	{
		return m_szFileAlias;
	}

	const s8 * GetFileAlias() const
	{
		return m_szFileAlias;
	}

	void SetFileAlias(const s8* pszFileAlias)
	{
		if (NULL == pszFileAlias)
		{
			return ;
		}
		
		s16 wFileLen = strlen(pszFileAlias);
		wFileLen = wFileLen <= RSCOMM_MAXLEN_FILEALIAS ? wFileLen : RSCOMM_MAXLEN_FILEALIAS;
		strncpy(m_szFileAlias, pszFileAlias, wFileLen);
		m_szFileAlias[wFileLen] = '\0';
	}

	//{{{{{{{{{{{{{{{ add by Royan 20140317
	ERSComm_RecLockStat GetRecLockStat()
	{
		return m_eRecLockStat;
	}

	void SetRecLockStat(const ERSComm_RecLockStat eRecLockStat)
	{
		m_eRecLockStat = eRecLockStat;
	}

	void SetRecLockStat(const u8 byRecLockStat)
	{
		if (byRecLockStat == RSCOMM_REC_STAT_UNLOCK)
		{
			m_eRecLockStat = RSCOMM_REC_STAT_UNLOCK;
		}
		else if (byRecLockStat == RSCOMM_REC_STAT_LOCK)
		{
			m_eRecLockStat = RSCOMM_REC_STAT_LOCK;
		}
		else
		{
			m_eRecLockStat = RSCOMM_REC_STAT_UNKNOWN;
		}
	}
	//}}}}}}}}}}}}}}} add by Royan 20140317 
	
protected:
	u32                 m_dwRecID;
	s8                  m_szFilePath[RSCOMM_MAXLEN_FILEPATH+1];  /*录像文件路径*/
	s8                  m_szFileName[RSCOMM_MAXLEN_FILENAME+1];  /*录像文件名称*/
	ERSComm_RecType     m_eRecType;                              /*录像类型*/
	TRSComm_TimeRange   m_tTimeRange;                            /*录像时间段*/
	u32                 m_dwRecSize;
	s8                  m_szFileAlias[RSCOMM_MAXLEN_FILEALIAS+1];/*录像别名*/
	ERSComm_RecLockStat m_eRecLockStat;                          /*录像锁定状态*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;


struct TRSComm_DeleteFile
{
public:
	TRSComm_DeleteFile()
	{
		memset(this, 0, sizeof(TRSComm_DeleteFile));
	}

	u32 GetRecID()
	{
		return m_dwRecID;
	}

	void SetRecID(const u32 dwRecID)
	{
		m_dwRecID = dwRecID;
	}

	s8 * GetFilePath()
	{
		return m_szFilePath;
	}

	const s8 * GetFilePath() const
	{
		return m_szFilePath;
	}

	void SetFilePath(const s8* pszFilePath)
	{
		if (NULL == pszFilePath)
		{
			return ;
		}

		s16 wPathLen = strlen(pszFilePath);
		wPathLen = wPathLen <= RSCOMM_MAXLEN_FILEPATH ? wPathLen : RSCOMM_MAXLEN_FILEPATH;
		strncpy(m_szFilePath, pszFilePath, wPathLen);
		m_szFilePath[wPathLen] = '\0';
	}

	s8 * GetFileName()
	{
		return m_szFileName;
	}

	const s8 * GetFileName() const
	{
		return m_szFileName;
	}

	void SetFileName(const s8* pszFileName)
	{
		if (NULL == pszFileName)
		{
			return ;
		}

		s16 wFileLen = strlen(pszFileName);
		wFileLen = wFileLen <= RSCOMM_MAXLEN_FILENAME ? wFileLen : RSCOMM_MAXLEN_FILENAME;
		strncpy(m_szFileName, pszFileName, wFileLen);
		m_szFileName[wFileLen] = '\0';
	}

protected:
	u32                 m_dwRecID;
	s8                  m_szFilePath[RSCOMM_MAXLEN_FILEPATH+1];  /*录像文件路径*/
	s8                  m_szFileName[RSCOMM_MAXLEN_FILENAME+1];  /*录像文件名称*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecDelCond
{
public:
	TRSComm_RecDelCond()
	{
		memset(this, 0, sizeof(TRSComm_RecDelCond));
	}

	TRSComm_DevChn& GetPuChn()
	{
		return m_tPuChn;
	}

	const TRSComm_DevChn& GetPuChn() const
	{
		return m_tPuChn;
	}

	void SetPuChn(const TRSComm_DevChn& tPuChn)
	{
		m_tPuChn = tPuChn;
	}

	TRSComm_TimeRange& GetDelTimeRange()
	{
		return m_tDelTimeRange;
	}

	void SetDelTimeRange(const TRSComm_TimeRange& tTimeRange)
	{
		memcpy(&m_tDelTimeRange, &tTimeRange, sizeof(TRSComm_TimeRange));
	}

protected:
	TRSComm_DevChn    m_tPuChn;
	TRSComm_TimeRange m_tDelTimeRange; 
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecStatusDetectionParam
{
public:
	TRSComm_RecStatusDetectionParam()
	{
		memset(this, 0, sizeof(TRSComm_RecStatusDetectionParam));
	}

	u32 GetRecStatusNotifyIntval()
	{
		return m_dwRecStatusNotifyIntval;
	}

	void SetRecStatusNotifyIntval(const u32 dwTimeval)
	{
		m_dwRecStatusNotifyIntval = dwTimeval;
	}

	u32 GetRecNoStreamCheckIntval()
	{
		return m_dwRecNoStreamCheckIntval;
	}

	void SetRecNoStreamCheckIntval(const u32 dwTimeval)
	{
		m_dwRecNoStreamCheckIntval = dwTimeval;
	}

protected:
	u32                m_dwRecStatusNotifyIntval;		   /* 录像状态上报间隔，单位秒 */
	u32                m_dwRecNoStreamCheckIntval;        /* 录像无码流检测间隔，单位秒 */
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecFileDetailedInfo : public TRSComm_RecFileSchema
{
public:
	TRSComm_RecFileDetailedInfo()
	{
		memset(this, 0, sizeof(TRSComm_RecFileDetailedInfo));
	}

	u16   GetSubTimeRangeNum()
	{
		return m_wSubRecTimeRangeNum;
	}

	void  SetSubTimeRangeNum(const u16 wNum)
	{
		m_wSubRecTimeRangeNum = wNum;
	}

	void AddSubTimeRangeNum()
	{
		m_wSubRecTimeRangeNum++;
	}

protected:
	u16                                         m_wSubRecTimeRangeNum;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_DBRecord
{
public:
	TRSComm_DBRecord()
	{
		memset(this, 0, sizeof(TRSComm_DBRecord));
	}

	TRSComm_DBRecord(TRSComm_RecordTask& tRecordTask)
	{
		memset(this, 0, sizeof(TRSComm_DBRecord));
		
		SetRecType(tRecordTask.GetRecType());
		SetHandle(*(TRSComm_Handle*)&tRecordTask);
		SetFileName(tRecordTask.GetRecFileName());
		SetFilePath(tRecordTask.GetRecFilePath());
		
		if (RSCOMM_RECMODE_ALL == tRecordTask.GetRecMode())
		{
			SetRecAudio(TRUE);
			SetAudPayloadType(tRecordTask.GetAudioEncParam().GetPayloadType());
			SetAudEncodingName(tRecordTask.GetAudioEncParam().GetEncodingName());
		}
		else
		{
			SetRecAudio(FALSE);
		}
		
		SetVidPayloadType(tRecordTask.GetVideoEncParam().GetPayloadType());
		SetVidEncodingName(tRecordTask.GetVideoEncParam().GetEncodingName());
	}

	u32 GetFileID()
	{
		return m_dwFileID;
	}

	const u32 GetFileID() const
	{
		return m_dwFileID;
	}

	void SetFileID(const u32 dwFileID)
	{
		m_dwFileID = dwFileID;
	}

	TRSComm_Handle& GetHandle()
	{
		return m_tRecHandle;
	}

	const TRSComm_Handle& GetHandle() const
	{
		return m_tRecHandle;
	}

	void SetHandle(const TRSComm_Handle& tHandle)
	{
		memcpy(&m_tRecHandle, &tHandle, sizeof(TRSComm_Handle));
	}

	s8 * GetFilePath()
	{
		return m_szFilePath;
	}

	const s8 * GetFilePath() const
	{
		return m_szFilePath;
	}

	void SetFilePath(const s8* pszFilePath)
	{
		if (NULL == pszFilePath)
		{
			return ;
		}

		s16 wPathLen = strlen(pszFilePath);
		wPathLen = wPathLen <= RSCOMM_MAXLEN_FILEPATH ? wPathLen : RSCOMM_MAXLEN_FILEPATH;
		strncpy(m_szFilePath, pszFilePath, wPathLen);
		m_szFilePath[wPathLen] = '\0';
	}

	s8 * GetFileName()
	{
		return m_szFileName;
	}

	const s8 * GetFileName() const
	{
		return m_szFileName;
	}

	void SetFileName(const s8* pszFileName)
	{
		if (NULL == pszFileName)
		{
			return ;
		}
		
		s16 wFileLen = strlen(pszFileName);
		wFileLen = wFileLen <= RSCOMM_MAXLEN_FILEPATH ? wFileLen : RSCOMM_MAXLEN_FILEPATH;
		strncpy(m_szFileName, pszFileName, wFileLen);
		m_szFileName[wFileLen] = '\0';
	}

	ERSComm_RecType GetRecType()
	{
		return m_eRecType;
	}

	const ERSComm_RecType GetRecType() const
	{
		return m_eRecType;
	}
	
	void SetRecType(const ERSComm_RecType eRecType)
	{
		m_eRecType = eRecType;
	}
	
	void SetRecType(const u8 byRecType)
	{
		if (byRecType == RSCOMM_RECTYPE_ALARM)
		{
			m_eRecType = RSCOMM_RECTYPE_ALARM;
		}
		else if (byRecType == RSCOMM_RECTYPE_HANDLE)
		{
			m_eRecType = RSCOMM_RECTYPE_HANDLE;
		}
		else if (byRecType == RSCOMM_RECTYPE_PLAN)
		{
			m_eRecType = RSCOMM_RECTYPE_PLAN;
		}
	}

	TRSComm_TimeRange& GetTimeRange()
	{
		return m_tTimeRange;
	}

	const TRSComm_TimeRange& GetTimeRange() const
	{
		return m_tTimeRange;
	}

	void SetTimeRange(const TRSComm_TimeRange& tTimeRange)
	{
		memcpy(&m_tTimeRange, &tTimeRange, sizeof(TRSComm_TimeRange));
	}

	u32 GetRecSize()
	{
		return m_dwRecSize;
	}

	const u32 GetRecSize() const
	{
		return m_dwRecSize;
	}

	void SetRecSize(const u32 dwRecSize)
	{
		m_dwRecSize = dwRecSize;
	}
	
	u32 GetLockDeadLine()
	{
		return m_dwRecLockDeadLine;
	}

	const u32 GetLockDeadLine() const
	{
		return m_dwRecLockDeadLine;
	}
	
	void SetLockDeadLine(const u32 dwDeadLine)
	{
		m_dwRecLockDeadLine = dwDeadLine;
	}
	
	BOOL32 IsRecLocked()
	{
		if (m_dwRecLockDeadLine == 0)
		{
			return FALSE;
		}
		
		return TRUE;
	}

	const BOOL32 IsRecLocked() const
	{
		if (m_dwRecLockDeadLine == 0)
		{
			return FALSE;
		}
		
		return TRUE;
	}
	
	BOOL32 IsRecAudio()
	{
		if ( 0 == m_byIsHasAudio)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	const BOOL32 IsRecAudio() const
	{
		if ( 0 == m_byIsHasAudio)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	
	void SetRecAudio(const BOOL32 bRecAudio)
	{
		if (bRecAudio)
		{
			m_byIsHasAudio = 1;
		}
		else
		{
			m_byIsHasAudio = 0;
		}
	}
	
	u8  GetVidPayloadType()
	{
		return m_byVidPayloadType;
	}

	const u8  GetVidPayloadType() const
	{
		return m_byVidPayloadType;
	}
	
	void SetVidPayloadType(const u8 byPayloadType)
	{
		m_byVidPayloadType = byPayloadType;
	}
	
	s8 * GetVidEncodingName()
	{
		return m_szVidEncodingName;
	}
	
	const s8 * GetVidEncodingName() const
	{
		return m_szVidEncodingName;
	}
	
	void SetVidEncodingName(const s8 * pszEncodingName)
	{
		if (NULL == pszEncodingName)
		{
			return;
		}
		
		s16 wCopyLen = strlen(pszEncodingName);
		wCopyLen = rsmin(wCopyLen, RSCOMM_MAXLEN_ENCODINGNAME);
		strncpy(m_szVidEncodingName, pszEncodingName, wCopyLen);
		m_szVidEncodingName[wCopyLen] = '\0';
	}
	
	u8   GetAudPayloadType()
	{
		return m_byAudPayloadType;
	}

	const u8   GetAudPayloadType() const
	{
		return m_byAudPayloadType;
	}
	
	void SetAudPayloadType(const u8 byPayloadType)
	{
		m_byAudPayloadType = byPayloadType;
	}
	
	s8 * GetAudEncodingName()
	{
		return m_szAudEncodingName;
	}
	
	const s8 * GetAudEncodingName() const
	{
		return m_szAudEncodingName;
	}

	void SetAudEncodingName(const s8 * pszEncodingName)
	{
		if (NULL == pszEncodingName)
		{
			return;
		}
		
		s16 wCopyLen = strlen(pszEncodingName);
		wCopyLen = rsmin(wCopyLen, RSCOMM_MAXLEN_ENCODINGNAME);
		strncpy(m_szAudEncodingName, pszEncodingName, wCopyLen);
		m_szAudEncodingName[wCopyLen] = '\0';
	}

protected:
	u32					m_dwFileID;
	TRSComm_Handle      m_tRecHandle;
	s8                  m_szFilePath[RSCOMM_MAXLEN_FILEPATH+1];  /*录像文件路径*/
	s8                  m_szFileName[RSCOMM_MAXLEN_FILENAME+1];  /*录像文件名称*/
	ERSComm_RecType     m_eRecType;                              /*录像类型*/
	TRSComm_TimeRange   m_tTimeRange;                            /*录像时间段*/
	u32                 m_dwRecSize;
	u32                 m_dwRecLockDeadLine;                     /*0:没有锁定，>0:锁定期限，0xFFFFFFFF:永久锁定*/
	u8                  m_byIsHasAudio;
    u8                  m_byVidPayloadType;
	s8                  m_szVidEncodingName[RSCOMM_MAXLEN_ENCODINGNAME+1];
    u8                  m_byAudPayloadType;
	s8                  m_szAudEncodingName[RSCOMM_MAXLEN_ENCODINGNAME+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_SyncRecord : public TRSComm_DBRecord
{
public:
	TRSComm_SyncRecord()
	{
		memset(this, 0, sizeof(TRSComm_SyncRecord));
	}
	
	ERSComm_SyncOperType GetSyncOperType()
	{
		return m_eSyncOperType;
	}

	const ERSComm_SyncOperType GetSyncOperType() const
	{
		return m_eSyncOperType;
	}

	void SetSyncOperType(const ERSComm_SyncOperType eSyncOperType)
	{
		m_eSyncOperType = eSyncOperType;
	}

	u32 GetSyncNO()
	{
		return m_dwSyncNO;
	}

	const u32 GetSyncNO() const
	{
		return m_dwSyncNO;
	}

	void SetSyncNO(u32 dwSyncNO)
	{
		m_dwSyncNO = dwSyncNO;
	}

protected:
	ERSComm_SyncOperType m_eSyncOperType;
	u32                  m_dwSyncNO;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_PlayParam : public TRSComm_Handle
{
public:
	TRSComm_PlayParam()
	{
		memset(this, 0, sizeof(TRSComm_PlayParam));
	}
	
	TRSComm_RecFileSchema& GetRecFileSchema()
	{
		return m_tRecFileSchema;
	}

	void SetRecFileSchema(const TRSComm_RecFileSchema& tRecFileSchema)
	{
		memcpy(&m_tRecFileSchema, &tRecFileSchema, sizeof(TRSComm_RecFileSchema));
	}

	u32  GetPlayStartTime()
	{
		return m_dwPlayStartTime;
	}

	void SetPlayStartTime(const u32 dwPlayStartTime)
	{
		m_dwPlayStartTime = dwPlayStartTime;
	}

	u32 GetPlaySSRC()
	{
		return m_dwSSRC;
	}

	void SetPlaySSRC(const u32 dwSSRC)
	{
		m_dwSSRC = dwSSRC;
	}

	BOOL32 IsReversePlay()
	{
		if (m_byReversePlay != 0)
		{
			return TRUE;
		}

		return FALSE;
	}

	void SetReversePlay(const u8 byReversePlay)
	{
		m_byReversePlay = byReversePlay;
	}

protected:
	TRSComm_RecFileSchema  m_tRecFileSchema;
	u32                    m_dwPlayStartTime;
	u32                    m_dwSSRC;
	u8                     m_byReversePlay;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_VCRCtrl
{
public:
	TRSComm_VCRCtrl()
	{
		memset(this , 0, sizeof(TRSComm_VCRCtrl));
	}

	ERSComm_VCRCmd GetVcrCmd()
	{
		return m_eVCRCmd;
	}

	void SetVcrCmd(const ERSComm_VCRCmd eVcrCmd)
	{
		m_eVCRCmd = eVcrCmd;
	}

	s32  GetPlayRate()
	{
		return m_nPlayRate;
	}

	void SetPlayRate(const s32 nPlayRate)
	{
		m_nPlayRate = nPlayRate;
	}

	s32  GetSeekTime()
	{
		return m_nSeekTime;
	}

	void SetSeekTime(const s32 nSeekTime)
	{
		m_nSeekTime = nSeekTime;
	}

protected:
	ERSComm_VCRCmd     m_eVCRCmd;
	s32                m_nPlayRate;         /*放像速率，正数为正反，负数为倒放*/
	s32                m_nSeekTime;         
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_PlayProg : public TRSComm_Handle
{
public:
	TRSComm_PlayProg()
	{
		memset(this, 0, sizeof(TRSComm_PlayProg));
	}

	u32  GetCurPlayTime()
	{
		return m_dwCurPlayTime;
	}

	void SetCurPlayTime(const u32 dwTime)
	{
		m_dwCurPlayTime = dwTime;
	}

	u32  GetTotalPlayTime()
	{
		return m_dwTotalPlayTime;
	}

	void SetTotalPlayTime(const u32 dwTime)
	{
		m_dwTotalPlayTime = dwTime;
	}

	u32  GetStartPlayTime()
	{
		return m_dwStartPlayTime;
	}

	void SetStartPlayTime(const u32 dwTime)
	{
		m_dwStartPlayTime = dwTime;
	}

	BOOL32 bPlayFinished()
	{
		return m_bPlayFinished;
	}

	void SetPlayFinished(BOOL32 bFinished)
	{
		m_bPlayFinished = bFinished;
	}

protected:
	u32                m_dwCurPlayTime;
	u32                m_dwTotalPlayTime;
	u32                m_dwStartPlayTime;
	BOOL32             m_bPlayFinished;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_DownloadParam : public TRSComm_Handle
{
public:
	TRSComm_DownloadParam()
	{
		memset(this, 0, sizeof(TRSComm_DownloadParam));
	}

	TRSComm_RecFileSchema& GetRecFileSchema()
	{
		return m_tRecFileSchema;
	}

	void SetRecFileSchema(const TRSComm_RecFileSchema& tRecFileSchema)
	{
		memcpy(&m_tRecFileSchema, &tRecFileSchema, sizeof(TRSComm_RecFileSchema));
	}
	
	TRSComm_TimeRange&  GetDownloadTimeRange()
	{
		return m_tDownloadTimeRange;
	}

	void SetDownloadTimeRange(const TRSComm_TimeRange& tTimeRange)
	{
		memcpy(&m_tDownloadTimeRange, &tTimeRange, sizeof(TRSComm_TimeRange));
	}

protected:
	TRSComm_RecFileSchema  m_tRecFileSchema;
	TRSComm_TimeRange      m_tDownloadTimeRange; 
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_MediaStreamInfo
{
public:
	TRSComm_MediaStreamInfo()
	{
		Clear();
	}

	const u32  GetRecBandwidth() const
	{
		return m_dwRecBandwidth;
	}

	void SetRecBandwidth(const u32 dwRecBandwidth)
	{
		m_dwRecBandwidth = dwRecBandwidth;
	}

	void AddRecBandwidth(const u32 dwAddBandwidth)
	{
		m_dwRecBandwidth += dwAddBandwidth;
	}

	void SubRecBandwidth(const u32 dwSubBandwidth)
	{
		if (m_dwRecBandwidth <= dwSubBandwidth)
		{
			m_dwRecBandwidth = 0;
		}
		else
		{
			m_dwRecBandwidth -= dwSubBandwidth;
		}
	}

	const u32  GetPreRecBandwidth() const 
	{
		return m_dwPreRecBandwidth;
	}

	void SetPreRecBandwidth(const u32 dwPreRecBandwidth)
	{
		m_dwPreRecBandwidth = dwPreRecBandwidth;
	}

	void AddPreRecBandwidth(const u32 dwAddBandwidth)
	{
		m_dwPreRecBandwidth += dwAddBandwidth;
	}

	void SubPreRecBandwidth(const u32 dwSubBandwidth)
	{
		if (m_dwPreRecBandwidth <= dwSubBandwidth)
		{
			m_dwPreRecBandwidth = 0;
		}
		else
		{
			m_dwPreRecBandwidth -= dwSubBandwidth;
		}
	}

	u16  GetPlayNum()
	{
		return m_wPlayNum;
	}

	void SetPlayNum(const u16 wPlayNum)
	{
		m_wPlayNum = wPlayNum;
	}

	void AddPlayNum()
	{
		m_wPlayNum++;
	}

	void SubPlayNum()
	{
		if (m_wPlayNum > 1)
		{
			m_wPlayNum--;
		}
		else
		{
			m_wPlayNum = 0;
		}
	}

	u16  GetDownloadNum()
	{
		return m_wDownloadNum;
	}

	void SetDownloadNum(const u16 wDownloadNum)
	{
		m_wDownloadNum = wDownloadNum;
	}

	void AddDownloadNum()
	{
		m_wDownloadNum++;
	}

	void SubDownloadNum()
	{
		if (m_wDownloadNum > 1)
		{
			m_wDownloadNum--;
		}
		else
		{
			m_wDownloadNum = 0;
		}
	}

	void Clear()
	{
		memset(this, 0, sizeof(TRSComm_MediaStreamInfo));
	}

protected:
	u32           m_dwRecBandwidth;     //录像带宽，Kbps
	u32           m_dwPreRecBandwidth;  //预录带宽，Kbps
	u16           m_wPlayNum;
	u16           m_wDownloadNum;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_PartitionInfo
{
public:
	TRSComm_PartitionInfo()
	{
		memset(this, 0, sizeof(TRSComm_PartitionInfo));
	}

	const s8 * GetPartitionName() const
	{
		return m_szPartitionName;
	}

	s8 * GetPartitionName()
	{
		return m_szPartitionName;
	}

	void SetPartitionName(const s8* pszPartitionName)
	{
		if (NULL == pszPartitionName)
		{
			return;
		}
		
		s16 wCopyLen = strlen(pszPartitionName);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_PARTITIONNAME ? wCopyLen : RSCOMM_MAXLEN_PARTITIONNAME;
		strncpy(m_szPartitionName, pszPartitionName, wCopyLen);
		m_szPartitionName[wCopyLen] = '\0';
	}

	const s8 * GetPartitionMountName() const 
	{
		return m_szPartitionMountName;
	}

	s8 * GetPartitionMountName()
	{
		return m_szPartitionMountName;
	}

	void SetPartitionMountName(const s8* pszPartitionMountName)
	{
		if (NULL == pszPartitionMountName)
		{
			return;
		}

		s16 wCopyLen = strlen(pszPartitionMountName);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_MOUNTNAME ? wCopyLen : RSCOMM_MAXLEN_MOUNTNAME;
		strncpy(m_szPartitionMountName, pszPartitionMountName, wCopyLen);
		m_szPartitionMountName[wCopyLen] = '\0';
	}

	const u64  GetPartitionTotalSize() const
	{
		return m_llPartitionTotalSize;
	}

	u64  GetPartitionTotalSize()
	{
		return m_llPartitionTotalSize;
	}

	void SetPartitionTotalSize(const u64 llTotalSize)
	{
		m_llPartitionTotalSize = llTotalSize;
	}

	u64  GetPartitionRemainSize()
	{
		return m_llPartitionRemainSize;
	}

	const u64  GetPartitionRemainSize() const
	{
		return m_llPartitionRemainSize;
	}

	void SetPartitionRemainSize(const u64 llRemainSize)
	{
		m_llPartitionRemainSize = llRemainSize;
	}

	u32 GetRemainRecTimeThresholdOfStopRecord()
	{
		return m_dwRemainRecTimeOfStopRecord;
	}

	const u32 GetRemainRecTimeThresholdOfStopRecord() const
	{
		return m_dwRemainRecTimeOfStopRecord;
	}

	void SetRemainRecTimeThresholdOfStopRecord(const u32 dwThresholdTime)
	{
		m_dwRemainRecTimeOfStopRecord = dwThresholdTime;
	}

	ERSComm_PartFormat GetPartitionFormat()
	{
		return m_ePartitionFormat;
	}

	const ERSComm_PartFormat GetPartitionFormat() const
	{
		return m_ePartitionFormat;
	}

	void SetPartitionFormat(const ERSComm_PartFormat ePartFormat)
	{
		m_ePartitionFormat = ePartFormat;
	}

	ERSComm_PartStatus GetPartitionStatus()
	{
		return m_ePartitionStatus;
	}

	const ERSComm_PartStatus GetPartitionStatus() const 
	{
		return m_ePartitionStatus;
	}

	void SetPartitionStatus(const ERSComm_PartStatus ePartStatus)
	{
		m_ePartitionStatus = ePartStatus;
	}

	BOOL32 IsPartitionOnline()
	{
		if (m_ePartitionStatus < RSCOMM_PARTSTATUS_UNKNOWN && RSCOMM_PARTSTATUS_OFFLINE != m_ePartitionStatus)
		{
			return TRUE;
		}

		return FALSE;
	}

	BOOL32 IsPartitionAlarm()
	{
		if (RSCOMM_PARTSTATUS_ALARM_LEVEL_ONE == m_ePartitionStatus || RSCOMM_PARTSTATUS_ALARM_LEVEL_TWO == m_ePartitionStatus)
		{
			return TRUE;
		}

		return FALSE;
	}

	const BOOL32 IsPartitionAlarm() const
	{
		if (RSCOMM_PARTSTATUS_ALARM_LEVEL_ONE == m_ePartitionStatus || RSCOMM_PARTSTATUS_ALARM_LEVEL_TWO == m_ePartitionStatus)
		{
			return TRUE;
		}
		
		return FALSE;
	}

	u16  GetPartitionAbnormalTimes()
	{
		return m_wPartitionAbnormalTimes;
	}

	const u16  GetPartitionAbnormalTimes() const
	{
		return m_wPartitionAbnormalTimes;
	}

	void SetPartitionAbnormalTimes(const u16 wAbnormalTimes)
	{
		m_wPartitionAbnormalTimes = wAbnormalTimes;
	}

	void IncreasePartitionAbnormalTimes()
	{
		m_wPartitionAbnormalTimes++;
	}

	void ClearPartitionAbnormalTimes()
	{
		m_wPartitionAbnormalTimes = 0;
	}

	u16 GetPartitionCurRecordNum()
	{
		return m_wPartitionCurRecordNum;
	}

	const u16 GetPartitionCurRecordNum() const
	{
		return m_wPartitionCurRecordNum;
	}

	void SetPartitionCurRecordNum(const u16 wNum)
	{
		m_wPartitionCurRecordNum = wNum;
	}

	//{{{{{{{{{{{{{{{ add by Royan 20140317
	u64 GetPartitionRecLockSize()
	{
		return m_llPartitionRecLockSize;
	}

	const u64 GetPartitionRecLockSize() const
	{
		return m_llPartitionRecLockSize;
	}

	void SetPartitionRecLockSize(const u64 llSize)
	{
		m_llPartitionRecLockSize = llSize;
	}

	u64 GetPartitionReservedSize()
	{
		return m_llPartitionReservedSize;
	}

	const u64 GetPartitionReservedSize() const
	{
		return m_llPartitionReservedSize;
	}

	void SetPartitionReservedSize(const u64 llSize)
	{
		m_llPartitionReservedSize = llSize;
	}
	//}}}}}}}}}}}}}}} add by Royan 20140317 

	const s8 * GetStorageVendor() const 
	{
		return m_szStorageVendor;
	}

	s8 * GetStorageVendor()
	{
		return m_szStorageVendor;
	}

	void SetStorageVendor(const s8* pszStorageVendor)
	{
		if (NULL == pszStorageVendor)
		{
			return;
		}

		s16 wCopyLen = strlen(pszStorageVendor);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_STORAGEVENDOR ? wCopyLen : RSCOMM_MAXLEN_STORAGEVENDOR;
		strncpy(m_szStorageVendor, pszStorageVendor, wCopyLen);
		m_szStorageVendor[wCopyLen] = '\0';
	}

	void PrintData(s8 * pszPrefix)
	{
		OspPrintf(TRUE, FALSE, "%sPartitionName : %s\n", pszPrefix, m_szPartitionName);
		OspPrintf(TRUE, FALSE, "%sPartMountName : %s\n", pszPrefix, m_szPartitionMountName);
		OspPrintf(TRUE, FALSE, "%sFormat        : %s\n", pszPrefix, GetFormatDesc());
		OspPrintf(TRUE, FALSE, "%sStatus        : %s\n", pszPrefix, GetStatus());
		OspPrintf(TRUE, FALSE, "%sAbnormalTimes : %d\n", pszPrefix, m_wPartitionAbnormalTimes);

		OspPrintf(TRUE, FALSE, "%sTotalSize     : %llu\n", pszPrefix, m_llPartitionTotalSize);
		OspPrintf(TRUE, FALSE, "%sRemainSize    : %llu\n", pszPrefix, m_llPartitionRemainSize);
		OspPrintf(TRUE, FALSE, "%sReservedSize  : %llu\n", pszPrefix, m_llPartitionReservedSize);
		OspPrintf(TRUE, FALSE, "%sRecLockSize   : %llu\n", pszPrefix, m_llPartitionRecLockSize);
		
		OspPrintf(TRUE, FALSE, "%sTimeOfStopRec : %u(s)\n", pszPrefix, m_dwRemainRecTimeOfStopRecord);
		OspPrintf(TRUE, FALSE, "%sCurRecordNum  : %u\n", pszPrefix, m_wPartitionCurRecordNum);

		OspPrintf(TRUE, FALSE, "%sStorageVendor : %s\n", pszPrefix, m_szStorageVendor);
	}

private:
	const s8 * GetStatus()
	{
		switch (m_ePartitionStatus)
		{
		case RSCOMM_PARTSTATUS_OFFLINE:
			return "Offline";
		case RSCOMM_PARTSTATUS_ONLINE:
			return "Online";
		case RSCOMM_PARTSTATUS_ALARM_LEVEL_ONE:
			return "AlarmOne";
		case RSCOMM_PARTSTATUS_ALARM_LEVEL_TWO:
			return "AlarmTwo";
		default:
			return "Unknown";
		}
	}

	const s8 * GetFormatDesc()
	{
		switch (m_ePartitionFormat)
		{
		case RSCOMM_PARTFORMAT_NORMAL:
			return "Ext3";
		case RSCOMM_PARTFORMAT_VBFS:
			return "Vbfs";
		case RSCOMM_PARTFORMAT_CLOUD:
			return "Cloud";
		default:
			return "Unknown";
		}
	}

protected:
	s8                   m_szPartitionName[RSCOMM_MAXLEN_PARTITIONNAME+1];
	s8                   m_szPartitionMountName[RSCOMM_MAXLEN_MOUNTNAME+1];
	u64                  m_llPartitionTotalSize;                           /*分区总大小，单位：M*/
	u64                  m_llPartitionRemainSize;                          /*分区剩余可用大小，单位：M*/
	u32                  m_dwRemainRecTimeOfStopRecord;                    /*分区剩余空间可以录像时长阈值（在策略为停录像时，剩余空间不满足该条件，则停录像），单位：秒*/
	ERSComm_PartFormat   m_ePartitionFormat;                               /*分区格式*/
	ERSComm_PartStatus   m_ePartitionStatus;                               /*分区状态*/
	u16                  m_wPartitionAbnormalTimes;                        /*分区的异常次数*/
	u16                  m_wPartitionCurRecordNum;
	u64                  m_llPartitionRecLockSize;                         /*分区录像锁定空间大小，单位：M*/ //add by Royan 20140317
	u64                  m_llPartitionReservedSize;                        /*分区预留空间大小，单位：M*/ //add by Royan 20140317
	s8					 m_szStorageVendor[RSCOMM_MAXLEN_STORAGEVENDOR+1]; /*存储设备厂商*/ //add by zhichun 20160825
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_SysStatusParam
{
public:
	TRSComm_SysStatusParam()
	{
		memset(this, 0, sizeof(TRSComm_SysStatusParam));
	}

	u8   GetMemUsage()
	{
		return m_byMemUsage;
	}

	void SetMemUsage(const u8 byMemUsage)
	{
		m_byMemUsage = byMemUsage;
	}

	u8   GetCPUUsage()
	{
		return m_byCPUUsage;
	}

	void SetCPUUsage(const u8 byCPUUsage)
	{
		m_byCPUUsage = byCPUUsage;
	}

	u32  GetInLost()
	{
		return m_dwInLost;
	}

	const u32 GetInLost() const
	{
		return m_dwInLost;
	}

	void SetInLost(const u32 dwInLost)
	{
		m_dwInLost = dwInLost;
	}

	u32  GetFullLost()
	{
		return m_dwFullLost;
	}

	const u32  GetFullLost() const
	{
		return m_dwFullLost;
	}

	void SetFullLost(const u32 dwFullLost)
	{
		m_dwFullLost = dwFullLost;
	}

protected:
	u8                   m_byMemUsage;       /*内存使用率，80则代表内存使用率为百分之八十*/
	u8                   m_byCPUUsage;
	u32                  m_dwInLost;         /*录像时由于网络或者CPU过高导致的丢包数量*/
	u32                  m_dwFullLost;       /*录像时录像缓冲满引起的丢包数*/
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_NruRegInfo : public TRSComm_DevBaseInfo
{
public:
	TRSComm_NruRegInfo()
	{
		memset(this, 0, sizeof(TRSComm_NruRegInfo));
	}

	ERSComm_RunMode GetNruRunMode()
	{
		return m_eNruRunMode;
	}

	void SetNruRunMode(const ERSComm_RunMode eRunMode)
	{
		m_eNruRunMode = eRunMode;
	}

	TRSComm_MediaStreamInfo& GetMediaStreamCap()
	{
		return m_tMediaStreamCap;
	}

	void SetMediaStreamCap(const TRSComm_MediaStreamInfo& tMediaStreamCap)
	{
		memcpy(&m_tMediaStreamCap, &tMediaStreamCap, sizeof(TRSComm_MediaStreamInfo));
	}

	TRSComm_SysStatusParam& GetNruSysStatus()
	{
		return m_tSysStatus;
	}

	void SetNruSysStatus(const TRSComm_SysStatusParam& tSysStatus)
	{
		memcpy(&m_tSysStatus, &tSysStatus, sizeof(TRSComm_SysStatusParam));
	}

	const TRSComm_Time& GetSysTime()
	{
		return m_tSysTime;
	}

	void SetSysTime(u32 dwSysTime)
	{
		m_tSysTime.SetTime(dwSysTime);
	}

	TRSComm_NetWork& GetNruNetWork()
	{
		return m_tNruIpAddr;
	}

	void SetNruNetWork(const TRSComm_NetWork& tNetWork)
	{
		memcpy(&m_tNruIpAddr, &tNetWork, sizeof(TRSComm_NetWork));
	}

protected:
	ERSComm_RunMode               m_eNruRunMode;
	TRSComm_MediaStreamInfo       m_tMediaStreamCap;
	TRSComm_SysStatusParam        m_tSysStatus;
	TRSComm_Time                  m_tSysTime;  
	TRSComm_NetWork               m_tNruIpAddr;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_DiscoveryNruInfo : public TRSComm_DevBaseInfo
{
public:
	TRSComm_DiscoveryNruInfo()
	{
		memset(this, 0, sizeof(TRSComm_DiscoveryNruInfo));
	}
	
	TRSComm_NetWork& GetNruNetWork()
	{
		return m_tNruNetWork;
	}
	
	void SetNruNetWork(const TRSComm_NetWork& tNetWork)
	{
		memcpy(&m_tNruNetWork, &tNetWork, sizeof(TRSComm_NetWork));
	}

	TRSComm_IPAddress& GetRegisterRmsIpAddr()
	{
		return m_tRegisterRmsIp;
	}
	
	void SetRegisterRmsIpAddr(const TRSComm_IPAddress& tRmsIpAddr)
	{
		memcpy(&m_tRegisterRmsIp, &tRmsIpAddr, sizeof(TRSComm_IPAddress));
	}
	
protected:
	TRSComm_NetWork               m_tNruNetWork;
	TRSComm_IPAddress             m_tRegisterRmsIp;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_NruInfo : public TRSComm_DevBaseInfo
{
public:
	TRSComm_NruInfo()
	{
		memset(this, 0, sizeof(TRSComm_NruInfo));
	}
	
	TRSComm_IPAddress& GetRegisterRmsIpAddr()
	{
		return m_tRegisterRmsIp;
	}
	
	void SetNruNetWork(const TRSComm_IPAddress& tRmsIpAddr)
	{
		memcpy(&m_tRegisterRmsIp, &tRmsIpAddr, sizeof(TRSComm_IPAddress));
	}

	BOOL32 IsUsed()
	{
		return m_bUsed;
	}

	const BOOL32 IsUsed() const
	{
		return m_bUsed;
	}

	void SetUsed(BOOL32 bUsed)
	{
		m_bUsed = bUsed;
	}
	
protected:
	TRSComm_IPAddress             m_tRegisterRmsIp;
	BOOL32                        m_bUsed;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_ChangeNruInfo : public TRSComm_DevBaseInfo
{
public:
	TRSComm_ChangeNruInfo()
	{
		memset(this, 0, sizeof(TRSComm_ChangeNruInfo));
	}
	
	BOOL32 IsUsed()
	{
		return m_bUsed;
	}
	
	void SetUsed(BOOL32 bUsed)
	{
		m_bUsed = bUsed;
	}
	
protected:
	BOOL32                        m_bUsed;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecTag
{
public:
	TRSComm_RecTag()
	{
		memset(this, 0, sizeof(TRSComm_RecTag));
	}

	u32 GetRecTagID()
	{
		return m_dwRecTagID;
	}

	void SetRecTagID(const u32 dwRecTagID)
	{
		m_dwRecTagID = dwRecTagID;
	}

	TRSComm_DevChn& GetPuChn()
	{
		return m_tPuChn;
	}

	void SetPuChn(const TRSComm_DevChn& tPuChn)
	{
		memcpy(&m_tPuChn, &tPuChn, sizeof(TRSComm_DevChn));
	}

	u32 GetTagCreateTime()
	{
		return m_dwCreateTime;
	}

	void SetTagCreateTime(const u32 dwCreateTime)
	{
		m_dwCreateTime = dwCreateTime;
	}

	const s8 * GetRecTagDesc()
	{
		return m_szRecTagDesc;
	}

	void SetRecTagDesc(const s8* szDesc)
	{
		if (NULL == szDesc)
		{
			return;
		}

		u16 dwDescLen = strlen(szDesc);
		dwDescLen = RSCOMM_MAXLEN_RECTAG <= dwDescLen ? RSCOMM_MAXLEN_RECTAG : dwDescLen;
		strncpy(m_szRecTagDesc, szDesc, dwDescLen);
		m_szRecTagDesc[dwDescLen] = '\0';
	}

protected:
	u32                           m_dwRecTagID;
	TRSComm_DevChn                m_tPuChn;
	u32                           m_dwCreateTime;
	s8                            m_szRecTagDesc[RSCOMM_MAXLEN_RECTAG+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

typedef   TRSComm_RecTag    TRSComm_RecTagAdd;

struct TRSComm_RecTagUpdate
{
public:
	TRSComm_RecTagUpdate()
	{
		memset(this, 0, sizeof(TRSComm_RecTagUpdate));
	}

	u32 GetRecTagID()
	{
		return m_dwRecTagID;
	}

	const u32 GetRecTagID() const
	{
		return m_dwRecTagID;
	}
	
	void SetRecTagID(const u32 dwRecTagID)
	{
		m_dwRecTagID = dwRecTagID;
	}

	const s8 * GetRecTagDesc() const
	{
		return m_szRecTagDesc;
	}
	
	void SetRecTagDesc(const s8* szDesc)
	{
		if (NULL == szDesc)
		{
			return;
		}
		
		u16 dwDescLen = strlen(szDesc);
		dwDescLen = RSCOMM_MAXLEN_RECTAG <= dwDescLen ? RSCOMM_MAXLEN_RECTAG : dwDescLen;
		strncpy(m_szRecTagDesc, szDesc, dwDescLen);
		m_szRecTagDesc[dwDescLen] = '\0';
	}

protected:
	u32                           m_dwRecTagID;
	s8                            m_szRecTagDesc[RSCOMM_MAXLEN_RECTAG+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct  TRSComm_RecTagDel
{
public:
	TRSComm_RecTagDel()
	{
		memset(this, 0, sizeof(TRSComm_RecTagDel));
	}
	
	u32 GetRecTagID()
	{
		return m_dwRecTagID;
	}

	const u32 GetRecTagID() const
	{
		return m_dwRecTagID;
	}
	
	void SetRecTagID(const u32 dwRecTagID)
	{
		m_dwRecTagID = dwRecTagID;
	}

protected:
	u32                           m_dwRecTagID;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecTagQueryCond : public TRSComm_QueryBaseCond
{
public:
	TRSComm_RecTagQueryCond()
	{
		memset(this, 0, sizeof(TRSComm_RecTagQueryCond));
	}

	BOOL32 IsQueryByPuChn()
	{
		return (m_dwQueryCond & (0x01 << 0));
	}

	const BOOL32 IsQueryByPuChn() const
	{
		return (m_dwQueryCond & (0x01 << 0));
	}

	void SetQueryByPuChn(BOOL32 bQueryByPuChn)
	{
		if(bQueryByPuChn) 
        {
            m_dwQueryCond |= (0x1 << 0); 
        }
        else 
        {
            m_dwQueryCond &= ~(0x1 << 0); 
        }
	}

	BOOL32 IsQueryByTimeRange()
	{
		return (m_dwQueryCond & (0x01 << 1));
	}

	const BOOL32 IsQueryByTimeRange() const
	{
		return (m_dwQueryCond & (0x01 << 1));
	}

	void SetQueryByTimeRange(BOOL32 bQueryByTimeRange)
	{
		if(bQueryByTimeRange) 
        {
            m_dwQueryCond |= (0x1 << 1); 
        }
        else 
        {
            m_dwQueryCond &= ~(0x1 << 1); 
        }
	}

	BOOL32 IsQueryByDescKeyWord()
	{
		return (m_dwQueryCond & (0x01 << 2));
	}

	const BOOL32 IsQueryByDescKeyWord() const
	{
		return (m_dwQueryCond & (0x01 << 2));
	}

	void SetQueryByDescKeyWord(BOOL32 bQueryByDescKeyWord)
	{
		if(bQueryByDescKeyWord) 
        {
            m_dwQueryCond |= (0x1 << 2); 
        }
        else 
        {
            m_dwQueryCond &= ~(0x1 << 2); 
        }
	}

	TRSComm_DevChn& GetPuChn()
	{
		return m_tPuChn;
	}

	const TRSComm_DevChn& GetPuChn() const
	{
		return m_tPuChn;
	}
	
	void SetPuChn(const TRSComm_DevChn& tPuChn)
	{
		memcpy(&m_tPuChn, &tPuChn, sizeof(TRSComm_DevChn));
	}

	TRSComm_TimeRange& GetQueryTimeRange()
	{
		return m_tQueryTimeRange;
	}

	const TRSComm_TimeRange& GetQueryTimeRange() const 
	{
		return m_tQueryTimeRange;
	}
	
	void SetQueryTimeRange(const TRSComm_TimeRange& tTimeRange)
	{
		memcpy(&m_tQueryTimeRange, &tTimeRange, sizeof(TRSComm_TimeRange));
	}

	const s8 * GetRecTagDescKeyWord() const 
	{
		return m_szRecTagDescKeyWord;
	}
	
	void SetRecTagDescKeyWord(const s8* szDescKeyWord)
	{
		if (NULL == szDescKeyWord)
		{
			return;
		}
		
		u16 dwDescLen = strlen(szDescKeyWord);
		dwDescLen = RSCOMM_MAXLEN_RECTAG <= dwDescLen ? RSCOMM_MAXLEN_RECTAG : dwDescLen;
		strncpy(m_szRecTagDescKeyWord, szDescKeyWord, dwDescLen);
		m_szRecTagDescKeyWord[dwDescLen] = '\0';
	}

protected:
	u32                m_dwQueryCond;
	TRSComm_DevChn     m_tPuChn;
	TRSComm_TimeRange  m_tQueryTimeRange;
	s8                 m_szRecTagDescKeyWord[RSCOMM_MAXLEN_RECTAG+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;


struct TRSComm_OnePuChnRecDays
{
public:
	TRSComm_OnePuChnRecDays()
	{
		memset(this, 0, sizeof(TRSComm_OnePuChnRecDays));
	}

	TRSComm_DevChn& GetPuChn()
	{
		return m_tPuChn;
	}

	void SetPuChn(const TRSComm_DevChn& tDevChn)
	{
		memcpy(&m_tPuChn, &tDevChn, sizeof(TRSComm_DevChn));
	}

	void SetDayBitMask(s8 byDayIndex, BOOL32 bMask)
	{
		if (byDayIndex > 31)
		{
			return;
		}

		if (bMask)
		{
			m_dwDayBitMask |= (0x01 << byDayIndex);
		}
		else
		{
			m_dwDayBitMask &= ~(0x01 << byDayIndex);
		}
	}

	u32 GetDayBitMask()
	{
		return m_dwDayBitMask;
	}

protected:
	TRSComm_DevChn        m_tPuChn;
	u32                   m_dwDayBitMask;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_SwitchRecord
{
public:
	TRSComm_SwitchRecord()
	{
		memset(this, 0, sizeof(TRSComm_SwitchRecord));
	}

	const s8 * GetSwitchToPartitionName() const 
	{
		return m_szSwitchToPartitionName;
	}
	
	s8 * GetSwitchToPartitionName()
	{
		return m_szSwitchToPartitionName;
	}
	
	void SetSwitchToPartitionName(const s8* pszPartitionName)
	{
		if (NULL == pszPartitionName)
		{
			return;
		}
		
		s16 wCopyLen = strlen(pszPartitionName);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_MOUNTNAME ? wCopyLen : RSCOMM_MAXLEN_MOUNTNAME;
		strncpy(m_szSwitchToPartitionName, pszPartitionName, wCopyLen);
		m_szSwitchToPartitionName[wCopyLen] = '\0';
	}

	const s8 * GetSwitchFromPartitionName() const 
	{
		return m_szSwitchFromPartitionName;
	}
	
	s8 * GetSwitchFromPartitionName()
	{
		return m_szSwitchFromPartitionName;
	}
	
	void SetSwitchFromPartitionName(const s8* pszPartitionName)
	{
		if (NULL == pszPartitionName)
		{
			return;
		}
		
		s16 wCopyLen = strlen(pszPartitionName);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_MOUNTNAME ? wCopyLen : RSCOMM_MAXLEN_MOUNTNAME;
		strncpy(m_szSwitchFromPartitionName, pszPartitionName, wCopyLen);
		m_szSwitchFromPartitionName[wCopyLen] = '\0';
	}

	TRSComm_RecordTask& GetRecordTask()
	{
		return m_tRecordTask;
	}

	void SetRecordTask(const TRSComm_RecordTask& tRecordTask)
	{
		memcpy(&m_tRecordTask, &tRecordTask, sizeof(TRSComm_RecordTask));
	}
	
protected:
	TRSComm_RecordTask   m_tRecordTask;
	s8                   m_szSwitchFromPartitionName[RSCOMM_MAXLEN_MOUNTNAME+1];
	s8                   m_szSwitchToPartitionName[RSCOMM_MAXLEN_MOUNTNAME+1];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_DeviceAlarm
{
public:
	TRSComm_DeviceAlarm()
	{
		memset(this, 0, sizeof(TRSComm_DeviceAlarm));
	}

	TRSComm_DevID& GetDeviceID()
	{
		return m_tDeviceID;
	}

	void SetDeviceID(const TRSComm_DevID& tDeviceID)
	{
		m_tDeviceID = tDeviceID;
	}

	ERSComm_AlarmType GetAlarmType()
	{
		return m_eAlarmType;
	}

	const ERSComm_AlarmType GetAlarmType() const
	{
		return m_eAlarmType;
	}

	void SetAlarmType(const ERSComm_AlarmType eAlarmType)
	{
		m_eAlarmType = eAlarmType;
	}

	ERSComm_AlarmCode GetAlarmCode()
	{
		return m_eAlarmCode;
	}

	const ERSComm_AlarmCode GetAlarmCode() const
	{
		return m_eAlarmCode;
	}

	void SetAlarmCode(const ERSComm_AlarmCode eAlarmCode)
	{
		m_eAlarmCode =  eAlarmCode;
	}

protected:
	TRSComm_DevID         m_tDeviceID;
	ERSComm_AlarmType     m_eAlarmType;
	ERSComm_AlarmCode     m_eAlarmCode;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

struct TRSComm_RecLock
{
public:
	TRSComm_RecLock()
	{
		memset(this, 0, sizeof(TRSComm_RecLock));
	}

	void SetLockRecording(BOOL32 bRecording)
	{
		if (bRecording)
		{
			m_byRecording = 1;
		}
		else
		{
			m_byRecording = 0;
		}
	}

	BOOL32 IsLockRecording()
	{
		return (m_byRecording == 1 ? TRUE : FALSE);
	}

	const BOOL32 IsLockRecording() const
	{
		return (m_byRecording == 1 ? TRUE : FALSE);
	}

	void SetHandle(const TRSComm_Handle& tHandle)
	{
		memcpy(&m_tTaskHandle, &tHandle, sizeof(TRSComm_Handle));
	}

	TRSComm_Handle& GetHandle()
	{
		return m_tTaskHandle;
	}

	const TRSComm_Handle& GetHandle() const
	{
		return m_tTaskHandle;
	}

	void SetRecFileSchema(const TRSComm_RecFileSchema& tRecFile)
	{
		memcpy(&m_tRecFile, &tRecFile, sizeof(TRSComm_RecFileSchema));
	}

	TRSComm_RecFileSchema& GetRecFileSchema()
	{
		return m_tRecFile;
	}

	const TRSComm_RecFileSchema& GetRecFileSchema() const
	{
		return m_tRecFile;
	}

	ERSComm_LockOperType GetLockOperType()
	{
		return m_eLockOperType;
	}

	const ERSComm_LockOperType GetLockOperType() const
	{
		return m_eLockOperType;
	}

	void SetLockOperType(const ERSComm_LockOperType eOperType)
	{
		m_eLockOperType = eOperType;
	}

	u32 GetLockDeadLine()
	{
		return m_dwLockDeadLine;
	}

	const u32 GetLockDeadLine() const
	{
		return m_dwLockDeadLine;
	}

	void SetLockDeadLine(const u32 dwDeadLine)
	{
		m_dwLockDeadLine = dwDeadLine;
	}

protected:
	u8                      m_byRecording;
	TRSComm_Handle          m_tTaskHandle;
	TRSComm_RecFileSchema   m_tRecFile;
	ERSComm_LockOperType    m_eLockOperType;
	u32                     m_dwLockDeadLine;
};

//{{{{{{{{{{{{{{{ add by Royan 20140317
struct TRSComm_RecLockStat
{
public:
	TRSComm_RecLockStat()
	{
		memset(this, 0, sizeof(TRSComm_RecLockStat));
	}

	ERSComm_RecLockStat GetRecLockStat()
	{
		return m_eRecLockStat;
	}

	const ERSComm_RecLockStat GetRecLockStat() const
	{
		return m_eRecLockStat;
	}

	void SetRecLockStat(const ERSComm_RecLockStat eRecLockStat)
	{
		m_eRecLockStat = eRecLockStat;
	}

protected:
	ERSComm_RecLockStat    m_eRecLockStat;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;
//}}}}}}}}}}}}}}} add by Royan 20140317 

struct TRSComm_OtherStorageLicense
{
public:
	TRSComm_OtherStorageLicense()
	{
		memset(this, 0, sizeof(TRSComm_OtherStorageLicense));
	}

	u32 GetOtherStorageLicense()
	{
		return m_dwOtherStorageLicense;
	}

	const u32 GetOtherStorageLicense() const
	{
		return m_dwOtherStorageLicense;
	}

	void SetOtherStorageLicense(const u32 dwLicense)
	{
		m_dwOtherStorageLicense = dwLicense;
	}

protected:
	u32 m_dwOtherStorageLicense;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

/*
 网段配置结构体 
*/
struct TRSComm_NetSegConfig
{
protected:
	u8 m_byNetID;
	u32 m_dwIpAddr;

public:
	TRSComm_NetSegConfig(void)
    {
        memset(this, 0, sizeof(TRSComm_NetSegConfig) );
    }

	TRSComm_NetSegConfig( u32 dwIpAddr, u8 byNetID)
	{
		this->SetNetID( byNetID );
		this->SetIpAddr( dwIpAddr );
	}

	void SetNetID( const u8 byNetID )
	{
		m_byNetID = byNetID;
	}
	u8 GetNetID( void ) const
	{
		return m_byNetID;
	}
	
	void SetIpAddr( u32 dwIpAddr )
	{
		m_dwIpAddr = dwIpAddr;
	}
	u32 GetIpAddr ( void ) const
	{
		return m_dwIpAddr;
	}
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

/************************************************************************/
/* 录像系统APPID定义                                                    */
/************************************************************************/
#define   RSCOMM_APPID_BASE                 (u16)200

#define   RSCOMM_RMSSDK_APPID_BEGIN          (u16) (RSCOMM_APPID_BASE + 1)
#define   RSCOMM_RMSSDK_APPID_END            (u16) (RSCOMM_RMSSDK_APPID_BEGIN + 9)


#define   RSCOMM_RMS_APPID_BEGIN             (u16) (RSCOMM_RMSSDK_APPID_END + 1)
#define	  APPID_RMS_RMSSDK                   (u16) (RSCOMM_RMS_APPID_BEGIN)
#define   APPID_RMS_VC                       (u16) (APPID_RMS_RMSSDK + 1)
#define   APPID_RMS_NRU                      (u16) (APPID_RMS_VC + 1)
#define   APPID_RMS_DISCOVERY                (u16) (APPID_RMS_NRU + 1)

#define   RSCOMM_RMS_APPID_END               (u16) (RSCOMM_RMS_APPID_BEGIN + 99)

#define   RSCOMM_NRU_APPID_BEGIN             (u16) (RSCOMM_RMS_APPID_END + 1)
#define   RSCOMM_NRU_APPID_END               (u16) (RSCOMM_NRU_APPID_BEGIN + 49)


/************************************************************************/
/* 录像系统内部OSP消息内容定义                                          */
/************************************************************************/

struct TRSComm_OspMsgHead
{
    u32  m_dwSequenceNO;                      //消息流水号
    
    char m_szSrcUUID[RSCOMM_MAXLEN_UUID + 1]; //源ID
    char m_szDstUUID[RSCOMM_MAXLEN_UUID + 1]; //目的ID
        
    u16  m_wEventId;                //消息ID
	u16  m_wTriggerReason;          //消息触发的原因
    u32  m_dwSendTime;              //消息发送时间
    u32  m_dwErrorCode;             //消息错误码
    u16  m_wMsgBodyLen;             //消息体长度
    
    u32  m_dwReserved1;	            //保留1
    u32  m_dwReserved2;	            //保留2
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

#define RSCOMM_OFFSET_MSGBODY     sizeof(TRSComm_OspMsgHead)
#define RSCOMM_MAXLEN_MSGBODY     (RSCOMM_MAXLEN_OSPMSG - RSCOMM_OFFSET_MSGBODY)

class CRSComm_OspMsg
{
public:
    CRSComm_OspMsg(void)
	{
		memset(m_abyBuffer, 0, RSCOMM_MAXLEN_OSPMSG);
	}
    
    CRSComm_OspMsg& operator= (const CRSComm_OspMsg& cMsg)
	{
		if(&cMsg == this)
        {
            return *this;
        }
        memcpy(m_abyBuffer, cMsg.m_abyBuffer, cMsg.GetMsgLen());
        return (*this);
	}

    CRSComm_OspMsg(const CRSComm_OspMsg& cMsg)
	{
		this->operator=(cMsg);
	}

    CRSComm_OspMsg(u8 * const pbyMsg, u16 wMsgLen)
	{
		memset(m_abyBuffer, 0, RSCOMM_MAXLEN_OSPMSG);
        
        if( wMsgLen < RSCOMM_OFFSET_MSGBODY || NULL == pbyMsg)
		{
            return;
		}
        
        TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        wMsgLen = rsmin( wMsgLen, sizeof( m_abyBuffer ) );
        memcpy( m_abyBuffer, pbyMsg, wMsgLen );
        ptMsgHead->m_wMsgBodyLen = htons(wMsgLen - RSCOMM_OFFSET_MSGBODY);
	}

    void ReverseID()
	{
		TRSComm_DevID tDevID = GetSrcDevID();
		SetSrcDevID(GetDstDevID());
		SetDstDevID(tDevID);
	}
    
    void ClearMsgHead(void)
	{
		memset(m_abyBuffer, 0, RSCOMM_OFFSET_MSGBODY);
	}

    void ClearMsgBody(void)
	{
		TRSComm_OspMsgHead* ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}
        
		if (ptMsgHead->m_wMsgBodyLen > 0)
		{
			memset(&m_abyBuffer[RSCOMM_OFFSET_MSGBODY], 0, ntohs(ptMsgHead->m_wMsgBodyLen));
			ptMsgHead->m_wMsgBodyLen = 0;
		}
	}

    void ClearMsg(void)
	{
		memset(m_abyBuffer, 0, RSCOMM_MAXLEN_OSPMSG);
	}

    u32  GetSequenceNo(void) const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return 0;
		}
        return ntohl(ptMsgHead->m_dwSequenceNO);
	}

    void SetSequenceNo(u32 dwSequenceNo)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}
		
		ptMsgHead->m_dwSequenceNO = htonl(dwSequenceNo);
	}
    
    LPCSTR GetSrcUUID() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return NULL;
		}

		return ptMsgHead->m_szSrcUUID;
	}
    TRSComm_DevID GetSrcDevID() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return TRSComm_DevID();
		}

		TRSComm_DevID tDevID;
		tDevID.SetDevID(ptMsgHead->m_szSrcUUID);

		return tDevID;
	}

    void SetSrcUUID(LPCSTR lpszSrcUUID)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead || NULL == lpszSrcUUID)
		{
            return;
		}

		s16 wCopyLen = strlen(lpszSrcUUID);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_UUID ? wCopyLen : RSCOMM_MAXLEN_UUID;
		strncpy(ptMsgHead->m_szSrcUUID, lpszSrcUUID, wCopyLen);
		ptMsgHead->m_szSrcUUID[wCopyLen] = '\0';
	}

    void SetSrcDevID(const TRSComm_DevID& tDevID)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}

		strncpy(ptMsgHead->m_szSrcUUID, tDevID.GetStringID(), RSCOMM_MAXLEN_UUID);
		ptMsgHead->m_szSrcUUID[RSCOMM_MAXLEN_UUID] = '\0';
	}
    
    LPCSTR GetDstUUID() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return NULL;
		}
		
		return ptMsgHead->m_szDstUUID;
	}

    TRSComm_DevID GetDstDevID() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return TRSComm_DevID();
		}
		
		TRSComm_DevID tDevID;
		tDevID.SetDevID(ptMsgHead->m_szDstUUID);
		
		return tDevID;
	}

    void SetDstUUID(LPCSTR lpszDstUUID)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead || NULL == lpszDstUUID)
		{
            return;
		}
		
		s16 wCopyLen = strlen(lpszDstUUID);
		wCopyLen = wCopyLen <= RSCOMM_MAXLEN_UUID ? wCopyLen : RSCOMM_MAXLEN_UUID;
		strncpy(ptMsgHead->m_szDstUUID, lpszDstUUID, wCopyLen);
		ptMsgHead->m_szDstUUID[wCopyLen] = '\0';
	}

    void SetDstDevID(const TRSComm_DevID& tDevID)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}
		
		strncpy(ptMsgHead->m_szDstUUID, tDevID.GetStringID(), RSCOMM_MAXLEN_UUID);
		ptMsgHead->m_szDstUUID[RSCOMM_MAXLEN_UUID] = '\0';
	}
 
    u16  GetEventID() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return 0;
		}

		return ntohs(ptMsgHead->m_wEventId);
	}

    void SetEventID(const u16 wEventID)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}

		ptMsgHead->m_wEventId = htons(wEventID);
	}

	u16  GetTriggerReason() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return 0;
		}

		return ntohs(ptMsgHead->m_wTriggerReason);
	}

	void SetTriggerReason(const u16 wReason)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}

		ptMsgHead->m_wTriggerReason = htons(wReason);
	}

    u32  GetMsgSendTime() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return (u32)time(NULL);
		}

		return ntohl(ptMsgHead->m_dwSendTime);
	}

    void SetMsgSendTime(u32 dwSendTime)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}

		ptMsgHead->m_dwSendTime = htonl(dwSendTime);
	}

    u32  GetErrorCode(void) const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return 0;
		}

		return ntohl(ptMsgHead->m_dwErrorCode);
	}

    void SetErrorCode(u32 dwErrorCode)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}

		ptMsgHead->m_dwErrorCode = htonl(dwErrorCode);
	}
    
    u32 GetReserved1() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return 0;
		}

		return ntohl(ptMsgHead->m_dwReserved1);
	}

    void SetReserved1(u32 dwReserved)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}

		ptMsgHead->m_dwReserved1 = htonl(dwReserved);
	}
    
    u32 GetReserved2() const
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return 0;
		}
		
		return ntohl(ptMsgHead->m_dwReserved2);
	}

    void SetReserved2(u32 dwReserved)
	{
		TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return;
		}
		
		ptMsgHead->m_dwReserved2 = htonl(dwReserved);
	}
    
    u16 GetMsgBodyLen(void) const
	{
		TRSComm_OspMsgHead* ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return 0;
		}
        return ntohs(ptMsgHead->m_wMsgBodyLen);
	}

    void* GetMsgBody(u16 wOffset = 0) const
	{
		TRSComm_OspMsgHead* ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return NULL;
		}

        if(ptMsgHead->m_wMsgBodyLen == 0)
		{
            return NULL;
		}

        return (void*)&m_abyBuffer[RSCOMM_OFFSET_MSGBODY + wOffset];
	}

    BOOL32 SetMsgBody(const void* pMsgBody, u16 wBodyLen)
	{
		if(pMsgBody != NULL && wBodyLen > 0)
        {			
            if(wBodyLen > RSCOMM_MAXLEN_MSGBODY)
            {
                return FALSE;
            }

            TRSComm_OspMsgHead* ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
            if(NULL == ptMsgHead)
			{
                return FALSE;
			}
            
            memcpy(&m_abyBuffer[RSCOMM_OFFSET_MSGBODY], pMsgBody, wBodyLen);
            ptMsgHead->m_wMsgBodyLen = htons(wBodyLen);
            
            return TRUE;
        }
        return FALSE;
	}

    BOOL32 CatMsgBody(const void* pMsgBody, u16 wBodyLen)
	{
		if( NULL != pMsgBody && wBodyLen > 0)
        {
            TRSComm_OspMsgHead* ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
            if(NULL == ptMsgHead)
			{
                return FALSE;
			}
            
            if(wBodyLen > (RSCOMM_MAXLEN_MSGBODY - ntohs(ptMsgHead->m_wMsgBodyLen)))
            {
                return FALSE;
            }
            
            memcpy(&m_abyBuffer[RSCOMM_OFFSET_MSGBODY + ntohs(ptMsgHead->m_wMsgBodyLen)],
                    pMsgBody, wBodyLen);
            ptMsgHead->m_wMsgBodyLen = htons(ntohs(ptMsgHead->m_wMsgBodyLen) + wBodyLen);

            return TRUE;
        }
        return FALSE;
	}

    u16 GetMsgLen(void) const
	{
		TRSComm_OspMsgHead* ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        if(NULL == ptMsgHead)
		{
            return 0;
		}

        return (ntohs(ptMsgHead->m_wMsgBodyLen) + RSCOMM_OFFSET_MSGBODY);
	}
   
    void SetMsg(u8 * const pbyMsg, u16 wMsgLen)
	{
		memset(m_abyBuffer, 0, RSCOMM_MAXLEN_OSPMSG);
        
        if( wMsgLen < RSCOMM_OFFSET_MSGBODY || NULL == pbyMsg)
		{
            return;
		}
        
        TRSComm_OspMsgHead *ptMsgHead = (TRSComm_OspMsgHead*)m_abyBuffer;
        wMsgLen = rsmin( wMsgLen, sizeof( m_abyBuffer ) );
        memcpy( m_abyBuffer, pbyMsg, wMsgLen );
        ptMsgHead->m_wMsgBodyLen = htons(wMsgLen - RSCOMM_OFFSET_MSGBODY);
	}

    u8 * const GetMsg( void ) const
	{
		return ( u8 * const )( m_abyBuffer );
	}
protected:    
    u8   m_abyBuffer[RSCOMM_MAXLEN_OSPMSG];
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

#ifdef WIN32
#pragma pack(pop)
#endif

#endif