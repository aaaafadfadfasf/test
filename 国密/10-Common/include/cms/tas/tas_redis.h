/*****************************************************************************
   功能        : tas redis头文件定义
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2016/03/39  1.0         huzhiyun       创建
******************************************************************************/
#ifndef _TAS_REDIS_H_
#define _TAS_REDIS_H_

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include "cms/cms_const.h"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using std::string;
using std::vector;
using std::set;
using std::map;

class TiXmlDocument;
class TiXmlNode;

const string FDB_CHN_TAS_DEV_CHANGE="tas_dev_change";              // tas发布的设备变更通道       内容为CTasDevChangeNtf
const string FDB_CHN_TAS_DEV_VID_NAME="tas_dev_video_name_change"; // tas发布的视频源别名变更通知  内容为CTasDevVideoNameNtf
const string FDB_CHN_TAS_DEV_VID_GPS="tas_dev_video_gps_change";   // tas发布的视频源GPS变更通知   内容为CTasDevVideoGpsNtf
const string FDB_CHN_TAS_DOMAIN_CHANGE="tas_domain_change";        // tas发布的域变更通道          内容为CTasDomainChangeNtf
const string FDB_CHN_TAS_USER_DEVPRIVILEGE_CHANGE="tas_user_devprivilege_change";  // tas发布用户设备权限变更通道 内容为CTasUserDevPrivilegeChangeNtf
const string FDB_CHN_TAS_USERGRP_DEVPRIVILEGE_CHANGE="tas_usergrp_devprivilege_change"; //tas发布用户组设备权限变更通道 内容为CTasUserGrpDevPrivilegeChangeNtf

// gbu与tas之间混排的批量设备入网修改退网的接口定义
// tas接收请求的通道：所有gbu模块共用--FDB_CHN_GBU_DEVICE_CHANGE
// gbu接收应答的通道：通过RedisGetDeviceBatchRspSubChn获取
// 请求的消息体CTasDeviceBatchChangeReq, 应答的消息体CTasDeviceBatchChangeRsp
inline string RedisGetDeviceBatchRspSubChn(const string &strGbuId)
{
    return string("gbu:devbatrsp:")+strGbuId;
}


// tas进度通知数值，-1表示失败,[0,100]表示进度值
const string FDB_TAS_PROCESS_VALUE_KEY="tas_process_value";
enum ENTasProcessType{
    enTasGatewayDevDelProcess=1   // id为CGatewayDelReq中的uri的user部分
};
inline string RedisProcessValueFiled(ENTasProcessType type, const string &id){
    char szFiled[100];
    sprintf(szFiled, "%d:%s", (int)type, id.c_str());
    return szFiled;
}


//设备信息变更通知之具体变更字段
typedef string TChangedField;
#define TCHANGED_FIELD_DEVNAME "devName"
#define TCHANGED_FIELD_DEVALIAS   "devAlias"
#define TCHANGED_FIELD_DEVMODEL "devModel"
#define TCHANGED_FIELD_DEVUSEFLAG "devUseFlag"
#define TCHANGED_FIELD_DEVNETTYPE "devNetType"
#define TCHANGED_FIELD_DEVNETADDRESS "devNetAddress"
#define TCHANGED_FIELD_DEVGROUPID "devGroupId"

struct TTasDevChangeItem
{
    string devId;
    string domainId;
    int oprType;                 // 0:add 1:del 2:mod
    // oprType为add或mod时订阅者可以根据属性忽略通知
    map<string,string> property; // creator-0:pmc 1:sdk 2:gbs 3:subgbs ,owner-<ownerid> ,devtype-1:enc 2:dec 4:tvwall ,domainid-<domainid>
    set<TChangedField> changedFields;                //oprType==2(mod)时有效              
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetChangedFields(const set<TChangedField>& cChangedFields)
    {
        changedFields = cChangedFields;
    }
    set<TChangedField>& GetChangedFields()
    {
        return changedFields;
    }
    const set<TChangedField>& GetChangedFields() const
    {
        return changedFields;
    }

};

class CTasDevChangeNtf
{
public:
    vector<TTasDevChangeItem> Items;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct CTasDomainChangeNtf
{
    string domainId;
    int oprType;                // 0:add 1:del 2:mod
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TTasDevVideoNameItem
{
    string devId;
    string domainId;
    int idx;
    string name;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTasDevVideoNameNtf
{
public:
    vector<TTasDevVideoNameItem> Items;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TTasDevVideoGpsItem
{
    string devId;
    string domainId;
    int idx;
    double latitude;
    double longitude;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTasDevVideoGpsNtf
{
public:
    vector<TTasDevVideoGpsItem> Items;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTasUserDevPrivilegeChangeNtf
{
	int oprType;                               // 0:add 1:del 2:mod
	string content;                            // add:CUserDeviceInfo ToXml结果；mod:CUserDeviceModReq ToXml结果;del:CUserDeviceInfoKey ToXml结果
public:
    void SetOprType(int nOprType)
    {
        oprType = nOprType;
    }
    int GetOprType() const
    {
        return oprType;
    }

    void SetContent(const string& strContent)
    {
        content = strContent;
    }
    string& GetContent()
    {
        return content;
    }
    const string& GetContent() const
    {
        return content;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTasUserGrpDevPrivilegeChangeNtf
{
	int oprType;                               // 0:add 1:del 2:mod
	string content;                            // add:CUserGroupDeviceInfo ToXml结果；mod:CUserGroupDeviceModReq ToXml结果;del:CUserGroupDeviceInfoKey ToXml结果
public:
    void SetOprType(int nOprType)
    {
        oprType = nOprType;
    }
    int GetOprType() const
    {
        return oprType;
    }

    void SetContent(const string& strContent)
    {
        content = strContent;
    }
    string& GetContent()
    {
        return content;
    }
    const string& GetContent() const
    {
        return content;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TTasDeviceBatchChangeItem
{
    string  type;          // 当前支持以下子消息类型：TAS_DEVICE_ADD/MOD/DEL_REQ、TAS_DEVICE_CAP_INDEX_NAME_SET_REQ、TAS_DEVICE_GROUP_DEVICE_ADD_REQ
    CTransparentData  content;                      // 相应子消息体的ToXml
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetContent(const CTransparentData& cContent)
    {
        content = cContent;
    }
    CTransparentData& GetContent()
    {
        return content;
    }
    const CTransparentData& GetContent() const
    {
        return content;
    }

};

class CTasDeviceBatchChangeReq
{
public:
	u32 taskno;               
    string gbuId;
	string loginsession;                //模块注册会话id，用于设备变更广播通知
    vector<TTasDeviceBatchChangeItem> Items;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTasDeviceBatchChangeRsp
{
public:
	u32 taskno;        //请求者Task               
    vector<int> Items; // 结果码数组
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif // _TAS_REDIS_H_







