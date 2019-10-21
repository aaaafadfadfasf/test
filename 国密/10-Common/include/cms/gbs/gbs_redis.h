/*****************************************************************************
   ����        : gbs redisͷ�ļ�����
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2016/03/39  1.0         huzhiyun       ����
******************************************************************************/
#ifndef _GBS_REDIS_H_
#define _GBS_REDIS_H_

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include "cms/cms_const.h"
#include "cms/gbs/gbs_struct.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using std::string;
using std::vector;
using std::set;
using std::map;
#include "cms/gb/gbspecial_struct.h"

class TiXmlDocument;
class TiXmlNode;
const string FDB_KEY_GBS_LOCALDOMAIN_GBID = "gbs_localdomain_gbid:"; //key�� valueΪ�������ID
const string FDB_KEY_PRIMARY_GBS = "primary_gbs";                 //key��primary_gbs,value:gbscfg.ini��cmssdk��uuid
const string FDB_KEY_GBS_RUN_TIME = "gbs_runnormal_time:";        //keyǰ׺��ð�ź��沿��ΪgbsIp��ֵΪ�ô�GBS���һ��ˢ�µ�ʱ��
const string FDB_KEY_GBS_IP      = "gbs_ip";                       //key,values:��gbs���̵�ips
const string FDB_KEY_GBSIP_UASID = "gbsip_uasId:";                 //keyǰ׺��ð�ź��沿��ΪGBSIP��ַ��valueΪ���ϣ���GBS�ֵ�������������ID��
const string FDB_KEY_UASID_GBSIP = "uasid_gbsip:";                 //keyǰ׺��ð�ź��沿��ΪuasId��value�ǳе����ϼ�ƽ̨���������gbsip
const string FDB_KEY_UAS_SELECT_FILE = "uas_select_file:";         //keyǰ׺��ð�ź��沿��ΪuasId,valueΪѡ�������ļ�����
const string FDB_CHN_UAS_SELECT_FILE_CHANGE = "uas_select_file_change"; //����ѡ�����ͱ��֪ͨ����Ϣ�壺uasId#treeId(��Ϊȫ����ʱ��treeidΪ"all").

/*��άϵͳ��̬���͵�key begin*/
const string FDB_KEY_UAS_SELECT_FILE_CAN_REPORT = "uas_select_file_can_report:"; //keyǰ׺��ð�ź��沿��Ϊuasid value:1��ʾ��������,0��ʾ��������
const string FDB_CHN_UAS_SELECT_FILE_EXTERN_CHANGE = "uas_select_file:extern:";  //keyǰ׺��ð�ź��沿��Ϊuasid valueΪѡ�������ļ�����
/*��άϵͳ��̬���͵�key end*/

const string FDB_CHN_PRIMARYGBS_CATALOG_QRY = "primarygbs_catalog_qry:"; //keyǰ׺��ð�ź�Ϊgbs-cmssdk-id
const string FDB_CHN_PRIMARYGBS_CATALOG_CHANGE="primarygbs_catalog_change";   // ��GBS����Ŀ¼���ͨ����to ��gbs��,��Ϣ�壺CGbsDevInfoNtf.toxml()
const string FDB_KEY_GBUAS_STATE = "gbuas_state:";                 //keyǰ׺��ð�ź�ΪgbuasId,valueΪ0-δע��,����-��ע�ᣬ
const string FDB_KEY_GBUAS_REPORT_TREEINFO = "gbuas_report_treeinfo:";//keyǰ׺��ð�ź�ΪgbuasId,����ΪtreeId#treever

const string FDB_KEY_GBS_GBSCFG="gbs_gbscfg";               // gbscfg�����ļ���key, valueΪ�����ļ�����
const string FDB_CHN_GBS_GBU_CFGFILE="gbs_gbu_cfgfile";     // gbs������gbscfg�����ļ����ͨ��������Ϊ�����ļ�����
const string FDB_CHN_GBS_UAS_RELATION ="gbs_uas_relation";  // gbs����ͨ����gbs��uas�Ķ�Ӧ��ϵ���֪ͨ,����Ϊ��TGbsUasRelationChangeNtf.toxml����
const string strVsip2GbFreeLicenseDevList =  "vsip2gb_freelicense_devlist"; //�˼����е��豸Ϊvsip2gb��������1��license�������1��license

const string FDB_KEY_GBS_OPTION_CFG_PREFIX = "gbs_option_cfg:";             // GBSѡ������ǰ׺��ð�ź��沿��Ϊ��������
const string FDB_KEY_GBS_OPTION_SYNC_GBPU_LONGLATI = "syncGbpuLongLati";    // GBSѡ����������Ƿ�ͬ��GBPU�ľ�γ����Ϣ��Ĭ��0��ʾ��ͬ��
const string FDB_CHN_GBS_OPTION_CFG = "gbs_option_cfg_change";              // GBSѡ�����ñ��ͨ��������ΪCGbsOptionConfSetReq.toXml����

struct TGbuDeviceCatalog
{
	TGbuDeviceCatalog()
	{
		kdid.clear();
		kdDomainId.clear();
		gbua.clear();
		kdChannelIndex = 0;
		channel = 0;
		gbLabel = 1; //gbu��������Ŀ¼��Ĭ����Ϊ�й�������
		gpsCap = 0;
	}
    TGBDeviceCataLog catalog;
    string kdid;
    string kdDomainId;
    int kdChannelIndex;
    string gbua;
    int channel;
    int gbLabel;
    int gpsCap;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGbuDeviceCatalogNtf
{
public:
    vector< TGbuDeviceCatalog > DeviceList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsUasRelationChangeNtf
{
	string gbsIp;               
	string uasGbid;               
	int changeType;                //0-add;1-del
public:
    void SetGbsIp(const string& strGbsIp)
    {
        gbsIp = strGbsIp;
    }
    string& GetGbsIp()
    {
        return gbsIp;
    }
    const string& GetGbsIp() const
    {
        return gbsIp;
    }

    void SetUasGbid(const string& strUasGbid)
    {
        uasGbid = strUasGbid;
    }
    string& GetUasGbid()
    {
        return uasGbid;
    }
    const string& GetUasGbid() const
    {
        return uasGbid;
    }

    void SetChangeType(int nChangeType)
    {
        changeType = nChangeType;
    }
    int GetChangeType() const
    {
        return changeType;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsDevInfoItem
{
	TGbsDevInfoItem()
	{
		type = 0;
		gpsCap = 0;
		kdPinIndex =0;
		vidsrcNum = 0;
	}
	int type;       //1-add;2-del;3-mod,0-��Чֵ        
	int gpsCap;
	int kdPinIndex;                    //���ڸ澯��
	int vidsrcNum;                
	TGbsDevInfo tDev;                              
	vector<string> vecAlarmInput;                  //�澯����
	vector<string> vecAudioOutput;                 //��Ƶ���
	vector<string> vecGbPuAduioOutput;                 //GbPu����Ƶ���
	vector<string> vecGbPuAlarmInPut;                  //GbPu�ĸ澯����

public:
    void SetType(int nType)
    {
        type = nType;
    }
    int GetType() const
    {
        return type;
    }

    void SetGpsCap(int nGpsCap)
    {
        gpsCap = nGpsCap;
    }
    int GetGpsCap() const
    {
        return gpsCap;
    }

    void SetVecAlarmInput(const vector<string>& cVecAlarmInput)
    {
        vecAlarmInput = cVecAlarmInput;
    }
    vector<string>& GetVecAlarmInput()
    {
        return vecAlarmInput;
    }
    const vector<string>& GetVecAlarmInput() const
    {
        return vecAlarmInput;
    }

    void SetVecAudioOutput(const vector<string>& cVecAudioOutput)
    {
        vecAudioOutput = cVecAudioOutput;
    }
    vector<string>& GetVecAudioOutput()
    {
        return vecAudioOutput;
    }
    const vector<string>& GetVecAudioOutput() const
    {
        return vecAudioOutput;
    }

    void SetVecGbPuAduioOutput(const vector<string>& cVecGbPuAduioOutput)
    {
        vecGbPuAduioOutput = cVecGbPuAduioOutput;
    }
    vector<string>& GetVecGbPuAduioOutput()
    {
        return vecGbPuAduioOutput;
    }
    const vector<string>& GetVecGbPuAduioOutput() const
    {
        return vecGbPuAduioOutput;
    }

    void SetVecGbPuAlarmInPut(const vector<string>& cVecGbPuAlarmInPut)
    {
        vecGbPuAlarmInPut = cVecGbPuAlarmInPut;
    }
    vector<string>& GetVecGbPuAlarmInPut()
    {
        return vecGbPuAlarmInPut;
    }
    const vector<string>& GetVecGbPuAlarmInPut() const
    {
        return vecGbPuAlarmInPut;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTDev(const TGbsDevInfo& cTDev)
    {
        tDev = cTDev;
    }
    TGbsDevInfo& GetTDev()
    {
        return tDev;
    }
    const TGbsDevInfo& GetTDev() const
    {
        return tDev;
    }

public:
    void SetKdPinIndex(int nKdPinIndex)
    {
        kdPinIndex = nKdPinIndex;
    }
    int GetKdPinIndex() const
    {
        return kdPinIndex;
    }

public:
    void SetVidsrcNum(int nVidsrcNum)
    {
        vidsrcNum = nVidsrcNum;
    }
    int GetVidsrcNum() const
    {
        return vidsrcNum;
    }

};

class CGbsDevInfoNtf
{
public:
	vector<TGbsDevInfoItem> DevInfoList;               
public:
    void SetDevInfoList(const vector<TGbsDevInfoItem>& cDevInfoList)
    {
        DevInfoList = cDevInfoList;
    }
    vector<TGbsDevInfoItem>& GetDevInfoList()
    {
        return DevInfoList;
    }
    const vector<TGbsDevInfoItem>& GetDevInfoList() const
    {
        return DevInfoList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPriGbsCatalogQryRsp
{
private:
	int taskNo;               
	vector<TGbsDevInfoItem> DevInfoList;                
public:
    void SetTaskNo(int nTaskNo)
    {
        taskNo = nTaskNo;
    }
    int GetTaskNo() const
    {
        return taskNo;
    }

    void SetDevInfoList(const vector<TGbsDevInfoItem>& cDevInfoList)
    {
        DevInfoList = cDevInfoList;
    }
    vector<TGbsDevInfoItem>& GetDevInfoList()
    {
        return DevInfoList;
    }
    const vector<TGbsDevInfoItem>& GetDevInfoList() const
    {
        return DevInfoList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif // _GBS_REDIS_H_
