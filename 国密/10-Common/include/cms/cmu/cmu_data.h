#ifndef _CMU_REDIS_DATA_H
#define _CMU_REDIS_DATA_H

#include <map>

#include "kdvtype.h"
#include "cms/cmu/cmu_struct.h"

typedef std::map<int, TAlarmInput>   CAlarmInputList;
typedef std::map<string, CAlarmInputList>   TPuAlarmInputList;

class CRedisDevData
{
public:
	CRedisDevData()  {}
	~CRedisDevData() {}

	void Clear()
	{
		m_tPuOnOff.Clear();
		m_tPuConfig.Clear();
		m_tPuAlarmCollector.clear();
		m_tPuServiceStatus.Clear();
		m_tGpsData.Clear();
		m_tTransparentData.Clear();
	}

public:
	// 数据操作
	const string& GetDevUri() const
	{
		return m_tPuConfig.GetDevUri();
	}

	bool GetIsOnline() const
	{
		return m_tPuOnOff.GetIsOnline();
	}
	void SetIsOnline(bool bIsOnline)
	{
		m_tPuOnOff.SetIsOnline(bIsOnline);
	}

	bool GetIsInTop() const
	{
		return m_tPuOnOff.GetIsInTop();
	}
	void SetIsInTop(bool bIsInTop)
	{
		m_tPuOnOff.SetIsInTop(bIsInTop);
	}

	const TPuStatus& GetPuStatus() const
	{
		return m_tPuOnOff;
	}
	TPuStatus& GetPuStatus()
	{
		return m_tPuOnOff;
	}
	void SetPuStatus(const TPuStatus& tPuStatus)
	{
		m_tPuOnOff = tPuStatus;
	}

	const TPuConfig& GetPuConfig() const
	{
		return m_tPuConfig;
	}
	TPuConfig& GetPuConfig()
	{
		return m_tPuConfig;
	}
	void SetPuConfig(const TPuConfig& tPuConfig)
	{
		m_tPuConfig = tPuConfig;
	}
	void MergePuConfig(const TPuDevConfig& tPuConfig)
	{
		m_tPuConfig.MergeDevConfig(tPuConfig);
	}

	const TPuServiceStatus& GetPuServiceStatus() const
	{
		return m_tPuServiceStatus;
	}
	TPuServiceStatus& GetPuServiceStatus()
	{
		return m_tPuServiceStatus;
	}
	void SetPuServiceStatus(const TPuServiceStatus& tPuServiceStatus)
	{
		m_tPuServiceStatus = tPuServiceStatus;
	}

	const map<string, CAlarmInputList> &GetPuAlarmCollector() const
	{
		return m_tPuAlarmCollector;
	}
	map<string, CAlarmInputList> &GetPuAlarmCollector()
	{
		return m_tPuAlarmCollector;
	}
	void SetPuAlarmCollector(const map<string, CAlarmInputList> &tPuAlarmCollector)
	{
		m_tPuAlarmCollector = tPuAlarmCollector;
	}
	TPuAlarmStatus GetPuAlarmStatus() const
	{
		TPuAlarmStatus  tPuAlarmStatus;

		typedef map<string, CAlarmInputList>::const_iterator TypeMapIt;
		for (TypeMapIt mtIt = m_tPuAlarmCollector.begin(); mtIt != m_tPuAlarmCollector.end(); ++mtIt)
		{
			typedef CAlarmInputList::const_iterator InputMapIt;
			for (InputMapIt miIt = mtIt->second.begin(); miIt != mtIt->second.end(); ++miIt)
			{
				tPuAlarmStatus.AddAlarmItem(miIt->second);
			}
		}

		return tPuAlarmStatus;
	}

	bool UpdateAlarmStatus(const TPuAlarmStatus &tAlarmStatus)
	{
		const CAlarmInputArray &cAlarmArray = tAlarmStatus.GetAlarmArray();
		for (vector<TAlarmInput>::const_iterator it = cAlarmArray.begin(); cAlarmArray.end() != it; ++it)
		{
			UpdateAlarmInput(*it);
		}

		return true;
	}

	bool IsValidAlarmType(const string& strAlarmType)
	{
		bool bValid = false;

		if (   ALARM_TYPE_PIN             == strAlarmType
			|| ALARM_TYPE_MOVE            == strAlarmType
			|| ALARM_TYPE_VIDLOST         == strAlarmType
			|| ALARM_TYPE_DISKFULL        == strAlarmType
			|| ALARM_TYPE_INTELLIGENT     == strAlarmType
			|| ALARM_TYPE_VIDEOPOLL       == strAlarmType
			|| ALARM_TYPE_EXCEPTION_EVENT == strAlarmType
			|| ALARM_TYPE_DEFOCUS         == strAlarmType
			|| ALARM_TYPE_SCENE_CHANGE    == strAlarmType
			|| ALARM_TYPE_REGION_INVASION == strAlarmType
			|| ALARM_TYPE_REGION_LEAVING  == strAlarmType
			|| ALARM_TYPE_OBJECT_TAKEN    == strAlarmType
			|| ALARM_TYPE_OBJECT_LEFT     == strAlarmType
			|| ALARM_TYPE_PEOPLE_GATHER   == strAlarmType
			|| ALARM_TYPE_CORGON          == strAlarmType
			|| ALARM_TYPE_AUDIO_ABNORMAL  == strAlarmType
			|| ALARM_TYPE_LUMA_CHANGE     == strAlarmType)
		{
			bValid = true;
		}

		return bValid;
	}

	bool UpdateAlarmInput(const TAlarmInput &tAlarmInput)
	{
		if (IsValidAlarmType(tAlarmInput.GetAlarmType()))
		{
			CAlarmInputList& tAlarmList = m_tPuAlarmCollector[tAlarmInput.GetAlarmType()];

			if (tAlarmInput.GetAlarmFlag() == ALARM_FLAG_OCCUR)
			{
				//告警产生
                CAlarmInputList::iterator it = tAlarmList.find(tAlarmInput.GetInputId());
                if (tAlarmList.end() == it || tAlarmInput.GetAlarmFlag() != it->second.GetAlarmFlag())
                {
                    tAlarmList[tAlarmInput.GetInputId()] = tAlarmInput;
                    return true;
                }
			}
			else
			{
				//告警恢复
                CAlarmInputList::iterator it = tAlarmList.find(tAlarmInput.GetInputId());
				if (tAlarmList.end() != it && tAlarmInput.GetAlarmFlag() != it->second.GetAlarmFlag())
				{
                    it->second = tAlarmInput;
                    return true;
				}
			}
		}

		return false;
	}

	const TGpsData& GetPuGpsData() const
	{
		return m_tGpsData;
	}
	TGpsData& GetPuGpsData()
	{
		return m_tGpsData;
	}
	void SetPuGpsData(const TGpsData& tGpsData)
	{
		m_tGpsData = tGpsData;
	}

	const TTransparentData& GetPuTransparentData() const
	{
		return m_tTransparentData;
	}
	TTransparentData& GetPuTransparentData()
	{
		return m_tTransparentData;
	}
	void SetPuTransparentData(const TTransparentData& tTransparentData)
	{
		m_tTransparentData = tTransparentData;
	}

private:
	TPuStatus m_tPuOnOff;                				// pu 在线状态
	TPuConfig m_tPuConfig;               				// pu 配置信息
	TPuServiceStatus m_tPuServiceStatus; 				// pu 业务状态
	map<string, CAlarmInputList> m_tPuAlarmCollector;	// pu 告警信息
	TGpsData m_tGpsData;                 				// pu gps数据
	TTransparentData m_tTransparentData; 				// pu 透明数据
};


class CRedisPuData : public CRedisDevData
{
public:
	CRedisPuData()  {}
	~CRedisPuData() {}

public:
	
	bool IsUsed() const
	{
		return m_tDevInfo.GetUsedFlag();
	}

	bool IsExpire() const
	{
		// 生成当前时间
		CCMSTime cCurrTime;

		// 获取expire
		string strTime = m_tDevInfo.GetExpireDate();

		CCMSTime cExpireTime;
		cExpireTime.FromString(strTime);

		return (cExpireTime < cCurrTime);
	}

	s32 GetPuLicenseNum() const
	{
		const vector<TDeviceCapInfo>& tDevCapInfo = m_tDevInfo.GetDeviceModelCapDetail();
		typedef vector<TDeviceCapInfo>::const_iterator VIT;
		for (VIT vecIt=tDevCapInfo.begin(); vecIt!=tDevCapInfo.end(); ++vecIt)
		{
			if (vecIt->type == DEVICE_CAP_VIDEOSOURCE)
			{
				return vecIt->license;
			}
		}

		return 0;
	}

	s32 GetPuVidSrcNum() const
	{
		const vector<TDeviceCapInfo>& tDevCapInfo = m_tDevInfo.GetDeviceModelCapDetail();
		typedef vector<TDeviceCapInfo>::const_iterator VIT;
		for (VIT vecIt=tDevCapInfo.begin(); vecIt!=tDevCapInfo.end(); ++vecIt)
		{
			if (vecIt->type == DEVICE_CAP_VIDEOSOURCE)
			{
				return vecIt->num;
			}
		}

		return 0;
	}

	s32 GetPuInputPinNum() const
	{
		const vector<TDeviceCapInfo>& tDevCapInfo = m_tDevInfo.GetDeviceModelCapDetail();
		typedef vector<TDeviceCapInfo>::const_iterator VIT;
		for (VIT vecIt=tDevCapInfo.begin(); vecIt!=tDevCapInfo.end(); ++vecIt)
		{
			if (vecIt->type == DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT)
			{
				return vecIt->num;
			}
		}

		return 0;
	}

	bool IsGBDevice() const
	{
		const vector<TDeviceCapInfo>& tDevCapInfo = m_tDevInfo.GetDeviceModelCapDetail();
		typedef vector<TDeviceCapInfo>::const_iterator VIT;
		for (VIT vecIt=tDevCapInfo.begin(); vecIt!=tDevCapInfo.end(); ++vecIt)
		{
			if (vecIt->type == DEVICE_CAP_LABEL_GBDEVICE)
			{
				return (vecIt->num > 0);
			}
		}

		return false;
	}

	const CDeviceInfo& GetDevInfo() const
	{
		return m_tDevInfo;
	}
	void SetDevInfo(const CDeviceInfo& tDevinfo)
	{
		m_tDevInfo = tDevinfo;
	}

private:
	CDeviceInfo  m_tDevInfo;				//从3AS获取的设备信息
};

#endif /*_CMU_REDIS_DATA_H*/