/*****************************************************************************
   ����        : tas redisͷ�ļ�����
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2016/03/39  1.0         huzhiyun       ����
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

const string FDB_CHN_TAS_DEV_CHANGE="tas_dev_change";              // tas�������豸���ͨ��       ����ΪCTasDevChangeNtf
const string FDB_CHN_TAS_DEV_VID_NAME="tas_dev_video_name_change"; // tas��������ƵԴ�������֪ͨ  ����ΪCTasDevVideoNameNtf
const string FDB_CHN_TAS_DEV_VID_GPS="tas_dev_video_gps_change";   // tas��������ƵԴGPS���֪ͨ   ����ΪCTasDevVideoGpsNtf
const string FDB_CHN_TAS_DOMAIN_CHANGE="tas_domain_change";        // tas����������ͨ��          ����ΪCTasDomainChangeNtf
const string FDB_CHN_TAS_USER_DEVPRIVILEGE_CHANGE="tas_user_devprivilege_change";  // tas�����û��豸Ȩ�ޱ��ͨ�� ����ΪCTasUserDevPrivilegeChangeNtf
const string FDB_CHN_TAS_USERGRP_DEVPRIVILEGE_CHANGE="tas_usergrp_devprivilege_change"; //tas�����û����豸Ȩ�ޱ��ͨ�� ����ΪCTasUserGrpDevPrivilegeChangeNtf

// gbu��tas֮����ŵ������豸�����޸������Ľӿڶ���
// tas���������ͨ��������gbuģ�鹲��--FDB_CHN_GBU_DEVICE_CHANGE
// gbu����Ӧ���ͨ����ͨ��RedisGetDeviceBatchRspSubChn��ȡ
// �������Ϣ��CTasDeviceBatchChangeReq, Ӧ�����Ϣ��CTasDeviceBatchChangeRsp
inline string RedisGetDeviceBatchRspSubChn(const string &strGbuId)
{
    return string("gbu:devbatrsp:")+strGbuId;
}


// tas����֪ͨ��ֵ��-1��ʾʧ��,[0,100]��ʾ����ֵ
const string FDB_TAS_PROCESS_VALUE_KEY="tas_process_value";
enum ENTasProcessType{
    enTasGatewayDevDelProcess=1   // idΪCGatewayDelReq�е�uri��user����
};
inline string RedisProcessValueFiled(ENTasProcessType type, const string &id){
    char szFiled[100];
    sprintf(szFiled, "%d:%s", (int)type, id.c_str());
    return szFiled;
}


//�豸��Ϣ���֪֮ͨ�������ֶ�
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
    // oprTypeΪadd��modʱ�����߿��Ը������Ժ���֪ͨ
    map<string,string> property; // creator-0:pmc 1:sdk 2:gbs 3:subgbs ,owner-<ownerid> ,devtype-1:enc 2:dec 4:tvwall ,domainid-<domainid>
    set<TChangedField> changedFields;                //oprType==2(mod)ʱ��Ч              
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
	string content;                            // add:CUserDeviceInfo ToXml�����mod:CUserDeviceModReq ToXml���;del:CUserDeviceInfoKey ToXml���
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
	string content;                            // add:CUserGroupDeviceInfo ToXml�����mod:CUserGroupDeviceModReq ToXml���;del:CUserGroupDeviceInfoKey ToXml���
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
    string  type;          // ��ǰ֧����������Ϣ���ͣ�TAS_DEVICE_ADD/MOD/DEL_REQ��TAS_DEVICE_CAP_INDEX_NAME_SET_REQ��TAS_DEVICE_GROUP_DEVICE_ADD_REQ
    CTransparentData  content;                      // ��Ӧ����Ϣ���ToXml
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
	string loginsession;                //ģ��ע��Ựid�������豸����㲥֪ͨ
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
	u32 taskno;        //������Task               
    vector<int> Items; // ���������
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif // _TAS_REDIS_H_







