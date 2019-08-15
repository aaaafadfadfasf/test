/*****************************************************************************
   ����        : cmu redisͷ�ļ�����
   ����        : xiongxh
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2016/03/30  1.0         xiongxh       ����
******************************************************************************/
#ifndef _CMU_REDIS_H_
#define _CMU_REDIS_H_

#include <string>
using std::string;

#include "cms/cms_proto.h"
#include "cms/rcs/rcs_proto.h"

//===============================================================================================================================
// ���㶩�ģ������豸Id�Ͷ�����������������ͨ��������Ĺ���λ��DevId + ":" + SsType��
//===============================================================================================================================

// ����DevId�Ͷ������ͻ�ȡ��Ӧ��Redis����ͨ��
// Redis�Ķ���ͨ������Ϊ���豸Id + ":" + �������ͣ�
// ����DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"���������߶�Ӧ�Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_ON_OFF"
inline string RedisGetSubChn(const string &strDevId, const TSsType &tSsType)
{
	return strDevId + string(":") + tSsType;
}

inline vector<string> RedisGetSubChn(const string &strDevId, const TSsTypes &tSsTypes)
{
    vector<string> cSubChn;
    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
    {
        cSubChn.push_back(RedisGetSubChn(strDevId, *it));
    }

    return cSubChn;
}

inline vector<string> RedisGetSubChn(const TSsDevList &tSsDevList)
{
    vector<string> cSubChn;
    for (TSsDevList::const_iterator it4Dev = tSsDevList.begin(); tSsDevList.end() != it4Dev; ++it4Dev)
    {
        const TSsTypes &tSsTypes = it4Dev->second;
        for (TSsTypes::const_iterator it4Type = tSsTypes.begin(); tSsTypes.end() != it4Type; ++it4Type)
        {
            cSubChn.push_back(RedisGetSubChn(it4Dev->first, *it4Type));
        }
    }

    return cSubChn;
}

// ����Redis����ͨ����������Ӧ��DevId�Ͷ������ͣ�������ΪRedisGetSubChn�ķ�����
// ���綩��ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_ON_OFF"������Խ�������Ӧ��DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"����������ΪPU_ON_OFF
inline bool RedisGetSubTypeByChn(const string &strChn, string &strDevId, TSsType &tSsType)
{
    string::size_type nPos = strChn.find(":");
    if (string::npos != nPos)
    {
        strDevId = strChn.substr(0, nPos);
        tSsType  = strChn.substr(nPos + 1);
        return true;
    }
    return false;
}

// ��ȡ�豸������״̬��Redis����ͨ��
// ͨ�������豸Id + ":" + ��������
// ͨ��������DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"���������ߵĶ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_ON_OFF"
// �������ݣ�TPuStatus����ToXml���л��������
inline string RedisGetOnOffSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_ON_OFF_STATUS);
}

// ��ȡ�豸������Ϣ��Redis����ͨ��
// ͨ�������豸Id + ":" + ��������
// ͨ��������DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"����������Ϣ�Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_CONFIG"
// �������ݣ�TPuDevConfig����ToXml���л��������
inline string RedisGetConfigSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_CONFIG_STATUS);
}

// ��ȡ�豸����״̬��Redis����ͨ��
// ͨ�������豸Id + ":" + ��������
// ͨ��������DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"�������״̬�Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_SERVICE"
// �������ݣ�TPuServiceStatus����ToXml���л��������
inline string RedisGetServiceSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_SERVICE_STATUS);
}

// ��ȡ�豸�澯��Ϣ��Redis����ͨ��
// ͨ�������豸Id + ":" + ��������
// ͨ��������DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"����澯��Ϣ�Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_ALARM"
// �������ݣ�TPuAlarmStatus����ToXml���л��������
inline string RedisGetAlarmSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_ALARM_STATUS);
}

// ��ȡ�豸GPS���ݵ�Redis����ͨ��
// ͨ�������豸Id + ":" + ��������
// ͨ��������DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"����GPS���ݵĶ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_GPS"
// �������ݣ�TGpsData����ToXml���л��������
inline string RedisGetGpsSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_GPS_STATUS);
}

// ��ȡ�豸͸�����ݵ�Redis����ͨ��
// ͨ�������豸Id + ":" + ��������
// ͨ��������DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"����͸�����ݵĶ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_TRANSDATA"
// �������ݣ�TTransparentData����ToXml���л��������
inline string RedisGetTransDataSubChn(const string &strDevId)
{
	return RedisGetSubChn(strDevId, PU_TRANSDATA_STATUS);
}

// ��ȡ�豸ƽ̨¼���Redis����ͨ��
// ͨ�������豸Id + ":" + ��������
// ͨ��������DevIdΪ"172e5444150a4b8ea17e241e2ded97e5"����ƽ̨¼��Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5:PU_PLATFORM_RECORD"
// �������ݣ�TRcsPuRecordStatus����ToXml���л��������
inline string RedisGetPlatRecSubChn(const string &strDevId)
{
    return RedisGetSubChn(strDevId, SS_TYPE_PU_PLATFORM_RECORD_STATUS);
}

//===============================================================================================================================
// ���ģ�������Id�Ͷ�����������������ͨ��������Ĺ���λ��DomainId + "#" + SsType��
//===============================================================================================================================
inline string RedisGetCatalogSubChn(const string &strDomainId, const TSsType &tSsType)
{
    return strDomainId + string("#") + tSsType;
}

inline vector<string> RedisGetCatalogSubChn(const string &strDomainId, const TSsTypes &tSsTypes)
{
    vector<string> cChn;
    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
    {
        cChn.push_back(RedisGetCatalogSubChn(strDomainId, *it));
    }

    return cChn;
}

// ����Redis����ͨ����������Ӧ����Id�Ͷ������ͣ�������ΪRedisGetCatalogSubChn�ķ�����
// ���綩��ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_ON_OFF"������Խ�������Ӧ��DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"����������ΪPU_ON_OFF
inline bool RedisGetCatalogSubTypeByChn(const string &strChn, string &strDomainId, TSsType &tSsType)
{
    string::size_type nPos = strChn.find("#");
    if (string::npos != nPos)
    {
        strDomainId = strChn.substr(0, nPos);
        tSsType = strChn.substr(nPos + 1);
        return true;
    }
    return false;
}

// ��ȡ�豸�����ߵ�Redis����ͨ��
// ͨ��������Id + "#" + ��������
// ͨ��������DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"���������߶�Ӧ�Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_ON_OFF"
// �������ݣ�CPuStatusNtyReq����ToXml���л��������
inline string RedisGetOnOffCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_ON_OFF_STATUS);
}

// ��ȡ�豸������Ϣ��Redis����ͨ��
// ͨ��������Id + "#" + ��������
// ͨ��������DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"����������Ϣ�Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_CONFIG"
// �������ݣ�CPuDevConfigNtyReq����ToXml���л��������
inline string RedisGetConfigCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_CONFIG_STATUS);
}

// ��ȡ�豸����״̬��Redis����ͨ��
// ͨ��������Id + "#" + ��������
// ͨ��������DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"�������״̬�Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_SERVICE"
// �������ݣ�CPuServiceStatusNtyReq����ToXml���л��������
inline string RedisGetServiceCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_SERVICE_STATUS);
}

// ��ȡ�豸�澯��Ϣ��Redis����ͨ��
// ͨ��������Id + "#" + ��������
// ͨ��������DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"����澯��Ϣ�Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_ALARM"
// �������ݣ�CPuAlarmStatusNtyReq����ToXml���л��������
inline string RedisGetAlarmCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_ALARM_STATUS);
}

// ��ȡ�豸GPS���ݵ�Redis����ͨ��
// ͨ��������Id + "#" + ��������
// ͨ��������DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"����GPS���ݵĶ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_GPS"
// �������ݣ�CPuGpsStatusNtyReq����ToXml���л��������
inline string RedisGetGpsCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_GPS_STATUS);
}

// ��ȡ�豸͸�����ݵ�Redis����ͨ��
// ͨ��������Id + "#" + ��������
// ͨ��������DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"����͸�����ݵĶ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_TRANSDATA"
// �������ݣ�CPuTransDataStatusNtyReq����ToXml���л��������
inline string RedisGetTransDataCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_TRANSDATA_STATUS);
}

// ģ��ע�����������֪ͨcmu�豸���ñ���Ķ���ͨ��
// ͨ��������Id + "#" + ��������
// ͨ��������DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"����͸�����ݵĶ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_DEVCONFIG_NTF"
// �������ݣ�CPuDevConfigNtyReq����ToXml���л��������
inline string RedisGetDevConfigNtfCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, PU_DEV_CONFIG_NTF);
}

// ��ȡ�豸ƽ̨¼���Redis����ͨ��
// ͨ��������Id + ":" + ��������
// ͨ��������DomainIdΪ"172e5444150a4b8ea17e241e2ded97e5"����ƽ̨¼��Ķ���ͨ��Ϊ��"172e5444150a4b8ea17e241e2ded97e5#PU_PLATFORM_RECORD"
// �������ݣ�CRcsPuRecordStatusNtyReq����ToXml���л��������
inline string RedisGetPlatRecCatalogSubChn(const string &strDomainId)
{
    return RedisGetCatalogSubChn(strDomainId, SS_TYPE_PU_PLATFORM_RECORD_STATUS);
}

//===============================================================================================================================
// �豸�ϱ����ݶ��ģ�������Id�Ͷ�����������������ͨ��������Ĺ���λ��DomainId + "#" + SsType��
//===============================================================================================================================
//inline RedisGetDevReportDataSubChn()

#endif // _CMU_REDIS_H_












