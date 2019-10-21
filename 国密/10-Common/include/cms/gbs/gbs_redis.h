/*****************************************************************************
   功能        : gbs redis头文件定义
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2016/03/39  1.0         huzhiyun       创建
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
const string FDB_KEY_GBS_LOCALDOMAIN_GBID = "gbs_localdomain_gbid:"; //key， value为本域国标ID
const string FDB_KEY_PRIMARY_GBS = "primary_gbs";                 //key，primary_gbs,value:gbscfg.ini中cmssdk的uuid
const string FDB_KEY_GBS_RUN_TIME = "gbs_runnormal_time:";        //key前缀，冒号后面部分为gbsIp，值为该从GBS最近一次刷新的时间
const string FDB_KEY_GBS_IP      = "gbs_ip";                       //key,values:各gbs进程的ips
const string FDB_KEY_GBSIP_UASID = "gbsip_uasId:";                 //key前缀，冒号后面部分为GBSIP地址，value为集合（该GBS分担的上联国标域ID）
const string FDB_KEY_UASID_GBSIP = "uasid_gbsip:";                 //key前缀，冒号后面部分为uasId，value是承担该上级平台上联任务的gbsip
const string FDB_KEY_UAS_SELECT_FILE = "uas_select_file:";         //key前缀，冒号后面部分为uasId,value为选择推送文件内容
const string FDB_CHN_UAS_SELECT_FILE_CHANGE = "uas_select_file_change"; //国标选择推送变更通知，消息体：uasId#treeId(改为全推送时，treeid为"all").

/*运维系统动态推送的key begin*/
const string FDB_KEY_UAS_SELECT_FILE_CAN_REPORT = "uas_select_file_can_report:"; //key前缀，冒号后面部分为uasid value:1表示可以推送,0表示不能推送
const string FDB_CHN_UAS_SELECT_FILE_EXTERN_CHANGE = "uas_select_file:extern:";  //key前缀，冒号后面部分为uasid value为选择推送文件内容
/*运维系统动态推送的key end*/

const string FDB_CHN_PRIMARYGBS_CATALOG_QRY = "primarygbs_catalog_qry:"; //key前缀，冒号后为gbs-cmssdk-id
const string FDB_CHN_PRIMARYGBS_CATALOG_CHANGE="primarygbs_catalog_change";   // 主GBS发布目录变更通道（to 从gbs）,消息体：CGbsDevInfoNtf.toxml()
const string FDB_KEY_GBUAS_STATE = "gbuas_state:";                 //key前缀，冒号后为gbuasId,value为0-未注册,其他-已注册，
const string FDB_KEY_GBUAS_REPORT_TREEINFO = "gbuas_report_treeinfo:";//key前缀，冒号后为gbuasId,内容为treeId#treever

const string FDB_KEY_GBS_GBSCFG="gbs_gbscfg";               // gbscfg配置文件的key, value为配置文件内容
const string FDB_CHN_GBS_GBU_CFGFILE="gbs_gbu_cfgfile";     // gbs发布的gbscfg配置文件变更通道，内容为配置文件内容
const string FDB_CHN_GBS_UAS_RELATION ="gbs_uas_relation";  // gbs发布通道：gbs与uas的对应关系变更通知,内容为：TGbsUasRelationChangeNtf.toxml内容
const string strVsip2GbFreeLicenseDevList =  "vsip2gb_freelicense_devlist"; //此集合中的设备为vsip2gb，最少收1个license，最多免1个license

const string FDB_KEY_GBS_OPTION_CFG_PREFIX = "gbs_option_cfg:";             // GBS选项配置前缀，冒号后面部分为具体子项
const string FDB_KEY_GBS_OPTION_SYNC_GBPU_LONGLATI = "syncGbpuLongLati";    // GBS选项配置子项，是否同步GBPU的经纬度信息，默认0表示不同步
const string FDB_CHN_GBS_OPTION_CFG = "gbs_option_cfg_change";              // GBS选项配置变更通道，内容为CGbsOptionConfSetReq.toXml内容

struct TGbuDeviceCatalog
{
	TGbuDeviceCatalog()
	{
		kdid.clear();
		kdDomainId.clear();
		gbua.clear();
		kdChannelIndex = 0;
		channel = 0;
		gbLabel = 1; //gbu发过来的目录，默认认为有国标能力
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
	int type;       //1-add;2-del;3-mod,0-无效值        
	int gpsCap;
	int kdPinIndex;                    //并口告警号
	int vidsrcNum;                
	TGbsDevInfo tDev;                              
	vector<string> vecAlarmInput;                  //告警输入
	vector<string> vecAudioOutput;                 //音频输出
	vector<string> vecGbPuAduioOutput;                 //GbPu的音频输出
	vector<string> vecGbPuAlarmInPut;                  //GbPu的告警输入

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
