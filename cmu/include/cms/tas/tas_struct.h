#ifndef _TAS_STRUCT_H_
#define _TAS_STRUCT_H_

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
#include <sstream>
using std::string;
using std::vector;
using std::set;
using std::map;
using std::istringstream;
using std::ostringstream;

class TiXmlDocument;
class TiXmlNode;

//enum TQueryOrderFieldId // 查询时排序字段标识
//{
//    // hzytodo(具体需求时再做) 它里面指定具体的列名, 使用方式，类似于1.0中的“enum EmQryOrderOption”
//    //TUser..
//    em_TUserNOOrderTerm          = 1, // 电信用户编号
//    em_TUserNameOrderTerm        = 2, // 电信用户名
//    em_TUserTypeOrderTerm        = 3, // 电信用户类型
//    em_TUserSexOrderTerm         = 4, // 电信用户性别
//    em_TUserUseStateOrderTerm    = 5, // 电信用户启停状态
//    em_TUserDepartmentOrderTerm  = 6, // 电信用户工作单位
//    em_TUserStaffNumOrderTerm    = 7, // 电信用户工号
//    em_TUserTelephoneOrderTerm   = 8, // 电信用户联系电话
//    em_TUserEmailOrderTerm       = 9, // 电信用户邮件
//    em_TUserDescriptionOrderTerm = 10,// 电信用户工作单位
//    em_TUser2DomainNOOrderTerm   = 11,// 电信用户所属域全局编号 
//    em_TUserExpireDateOrderTerm  = 12,// 电信用户使用截止日期
//    em_TUserCreateDateOrderTerm  = 13,// 电信用户创建日期
//    em_EndQryOrderTermForTUser   = 14,// 结束点..
//};

enum ENTasResourceTypes
{
    TasResourceGeneralDev = 1,      // 通用设备(不包括特殊处理的设备类型，比如电视墙设备)
    TasResourceTvwallDev  = 2,      // 电视墙设备
    TasResourceDevModel   = 3,      // 设备型号
    TasResourceDevCapIndexName = 4, // 设备能力索引名称
    TasResourceDevGrp = 5,          // 设备分组
    TasResourceDevGrp2Dev = 6,      // 设备分组设备划归
    TasResourceDomainRelation = 7,  // 域关系

    TasResourceCuser = 8,           // CU用户
    TasResourceCuser2Dev = 9,       // CU用户设备划归
    TasResourceCuserGrp = 10,       // CU用户分组
    TasResourceCuserGrp2Dev = 11,   // CU用户分组设备划归

    TasResourceTuser = 12,               // tac用户
    TasResourceSyncDstDomainCfg = 13,    // 同步目的域配置
    TasResourceTmpAuthorizationCfg = 14, // 跨域授权配置

	// 细化通用设备通知 [11/24/2015 pangubing]
	TasResourceGeneralDevAdd = 15,      // 通用设备增加(不包括特殊处理的设备类型，比如电视墙设备)
	TasResourceGeneralDevDel = 16,      // 通用设备删除(不包括特殊处理的设备类型，比如电视墙设备)
	TasResourceGeneralDevMod = 17,      // 通用设备修改(不包括特殊处理的设备类型，比如电视墙设备)
	TasResourcePtzLock  = 18,      // Ptz锁

    TasLicenseUpdateNotify = 19,        // license更新时通知（用于扩容时实时通知新的license数值）

	TasResourceIgnoreGbChildDomainDevChg = 20, // 忽略国标子域设备变更（只要是国标子域设备相关变更，全忽略）

	TasResourceEnd,
};
// 外设模块登录的扩展信息
struct TPSLoginExternInfo
{
public:
	TPSLoginExternInfo()
	{
		needGrpGbid=false;
	}
public:
    set<int> resourceTypes; // 关注的资源类型，TAS内部的一些变更只有外设关注时才通知。数值类型为ENTasResourceTypes
	bool needGrpGbid;  //是否需要国标编码
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 上下级同步登录的扩展信息
struct TParentSyncLoginExternInfo
{
public:
	string     platVersion;				// 平台版本信息

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// 上下级同步登录返回的扩展信息
struct TParentSyncRspLoginExternInfo
{
public:
	string     platVersion;				// 平台版本信息

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
protected:
	void ToXml(TiXmlDocument& xmlDoc)const;
public:
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CBitsContainer
{
public:
    CBitsContainer(){}
    void SetBit(u32 dwIndex, bool bOn = true)
    {
        CMS_ASSERT(dwIndex < 32);
        char ch = bOn? '1':'0';

        if (bits.size() <= dwIndex)
        {
            bits.resize(dwIndex+1, '0');
        }

        bits[dwIndex] = ch;
    }
    bool IsBitOn(u32 dwIndex) const
    {
        CMS_ASSERT(dwIndex < 32);
        if (bits.size() <= dwIndex)
        {
            return false;
        }
        else
        {
            return bits[dwIndex] == '1';
        }
    }
    u32 Convert2Int() const
    {
        CMS_ASSERT(bits.size() <= 32);
        u32 dwVal = 0;
        for (u32 i = 0; i < bits.size(); ++i)
        {
            if (bits[i] == '1') dwVal |= (0x1 << i);
        }
        return dwVal;
    }
    void ConvertFromInt(u32 dwVal)
    {
        bits.clear();

        u32 dwMask = 0x1;
        for (s32 i = 0; i < 32; ++i)
        {
            if (dwVal & (dwMask<<i))
            {
                SetBit(i);
            }
        }
    }
    void ConvertFromBitsContainer(const CBitsContainer &val)
    {
        bits = val.bits;
    }
    void Clear()
    {
        bits.clear();
    }
    bool IsSuperSet(const CBitsContainer &val) // 判断是否为val的超集
    {
        size_t i;

        size_t size = bits.size()<val.bits.size()? bits.size():val.bits.size();
        for (i = 0; i < size; ++i)
        {
            if (bits[i] < val.bits[i]) return false;
        }
        for (; i < val.bits.size(); ++i)
        {
            if (val.bits[i] == '1') return false; 
        }

        return true;
    }
private:
    string bits; // 位有效时相应位数值为1，否则为0. 此成员不要公布给外部
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserPrivilege: public CBitsContainer
{ // cu用户权限位
public:
    CUserPrivilege(){}
    enum TPrivilege
    {
        en_Vod_Rec              = 1, // 平台录像 第一位
        en_Vod_Play             = 2, // 平台放像
        en_Vod_Del              = 3, // 平台录像删除
        en_Vod_Download         = 4, // 平台录像下载
        en_Vod_Local_Rec        = 5, // 本地录像，本地抓拍
        en_Rec_Query_Remote     = 6, // 跨域录像查询

        en_Call_Ctrl            = 7,  // 呼叫控制
        en_Alarm_Manage         = 8,  // 现场告警管理

        // 下面两权限暂时用不到, 先注释掉
        //en_Explore              = 9,  // 视频浏览权限
        //en_Cycle_Explore        = 10, // 轮巡权限
    };
    void SetPrivilege(CUserPrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CUserPrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserPrivilege: public CBitsContainer
{ // tac用户权限位
public:
    CTacUserPrivilege(){}
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CEncoderPrivilege: public CBitsContainer
{ // 编码器权限
public:
    CEncoderPrivilege(){}
    enum TPrivilege
    {
        en_Vod_Rec              = 1, // 前端录像
        en_Vod_Play             = 2, // 前端放像
        en_Vod_Del              = 3, // 前端录像删除
        en_Vod_Download         = 4, // 前端录像下载

        en_Param_Config         = 5, // 前端参数设置
        en_IO_Ctrl              = 6, // 前端开关量输出控制
        en_Defence_Ctrl         = 7, // 布防/撤防
    };
    void SetPrivilege(CEncoderPrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CEncoderPrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CVideosourcePrivilege: public CBitsContainer
{ // 视频源权限
public:
    CVideosourcePrivilege(){}
    enum TPrivilege
    {
        en_PTZ_Ctrl              = 1, // 云镜控制(1-CMS_PTZ_MAX_PRI_LEV)级 共占用CMS_PTZ_MAX_PRI_LEV位(为了XML便于阅读)
        en_AUDIO_PLAY            = 11, // 声音播放
		en_PrePos_Set            = 12, // 预置位设置   [12/10/2015 pangubing]
		en_PTZ_Lock              = 13  // PTZ锁定   [3/28/2016 pangubing]
    };
    void SetPrivilege(CVideosourcePrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CVideosourcePrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
    void SetPTZCtrl(int pri)
    {
        if (pri > CMS_PTZ_MAX_PRI_LEV)
        {
            CMS_ASSERT(false && "ptz privilege extend CMS_PTZ_MAX_PRI_LEV level");
            pri = CMS_PTZ_MAX_PRI_LEV;
        }

        s32 i;
        for (i = 0; i < CMS_PTZ_MAX_PRI_LEV; ++i)
        {
            SetBit(en_PTZ_Ctrl+i-1, false);
        }

        for (i = 1; i <= pri; ++i)
        {
            SetBit(en_PTZ_Ctrl+(i-1)-1);
        }
    }
    int GetPTZCtrl() const
    {
        s32 nPrivi = 0;
        for (s32 i = CMS_PTZ_MAX_PRI_LEV-1; i >= 0; --i)
        {
            if (IsBitOn(en_PTZ_Ctrl+i-1))
            {
                nPrivi = i+1;
                break;
            }
        }
        return nPrivi;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CAlarmInputPrivilege: public CBitsContainer
{ // 告警权限
public:
    CAlarmInputPrivilege(){}
    enum TPrivilege
    {
        em_Alarm_Manage       = 1,   // 告警管理
    };
    void SetPrivilege(CAlarmInputPrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CAlarmInputPrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTvwallPrivilege: public CBitsContainer
{ // 电视墙权限, 电视墙的具体能力权限使用伪能力且权限位全部置位
public:
    CTvwallPrivilege(){}
    enum TPrivilege
    {
        em_Tvwall_Manage       = 1,   // 电视墙管理
    };
    void SetPrivilege(CTvwallPrivilege::TPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CTvwallPrivilege::TPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 设备权限位
class CDevicePrivilege: public CBitsContainer
{
public:
    CDevicePrivilege(){}
	CEncoderPrivilege GetEncoderPrivilege() 
    {
		CEncoderPrivilege val;
        val.ConvertFromBitsContainer(*this);
		return val;
    }
	CEncoderPrivilege GetEncoderPrivilege() const
    {
		CEncoderPrivilege val;
        val.ConvertFromBitsContainer(*this);
		return val;
    }
    void SetEncoderPrivilege(const CEncoderPrivilege &val)
    {
        ConvertFromBitsContainer(val);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 设备能力不带权限位
struct TDeviceCapIndexs
{
    TDeviceCapIndexs()
    {
        id = DEVICE_CAP_INVALID;
    }
    int id;                 // 能力标识
    vector<int> indexs;     // 能力索引占用数组
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 设备能力权限位
struct TDeviceCapPrivilege
{
    TDeviceCapPrivilege()
    {
        id = DEVICE_CAP_INVALID;
    }

    int id;  // 能力标识
    map<int, CBitsContainer> privileges; // 能力索引权限, key为能力索引, value为权限数值
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceCapInfo
{
    TDeviceCapInfo()
    {
        type = DEVICE_CAP_INVALID;
        num = 0;
        license = 0;
    }
    int type;    // 设备能力类型
    int num;   // 设备能力数目(也即具有多少个能力，比如具体多少个编码通道)
    int license; // 收取license数目
public:
    bool operator==(const TDeviceCapInfo& rh) const
    {
        return (type == rh.type && num == rh.num && license == rh.license);
    }
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TDeviceCapExtDataType;
#define DEVICE_CAP_EXT_DATA_TYPE_LON					"Lon"		//经度
#define DEVICE_CAP_EXT_DATA_TYPE_LAT					"Lat"		//纬度

struct TDeviceCapExtData
{
public:
	TDeviceCapExtData()
	{

	}

	void Clear()
	{
		extData.clear();
	}

	const map<TDeviceCapExtDataType, string> &GetExtData() const
	{
		return extData;
	}
	map<TDeviceCapExtDataType, string> &GetExtData()
	{
		return extData;
	}
	void SetExtData(const map<TDeviceCapExtDataType, string> &val)
	{
		extData = val;
	}

	bool GetLon(double &val) const
	{
		if (!GetDoubleData(DEVICE_CAP_EXT_DATA_TYPE_LON, val))
		{
			val = INVALID_LONGITUDE;
		}

		return true;
	}
	void SetLon(double val)
	{
		SetDoubleData(DEVICE_CAP_EXT_DATA_TYPE_LON, val);
	}

	bool GetLat(double &val) const
	{
		if (!GetDoubleData(DEVICE_CAP_EXT_DATA_TYPE_LAT, val))
		{
			val = INVALID_LATITUDE;
		}

		return true;
	}
	void SetLat(double val)
	{
		SetDoubleData(DEVICE_CAP_EXT_DATA_TYPE_LAT, val);
	}

private:
	bool GetDoubleData(const TDeviceCapExtDataType &tType, double &val) const
	{
		map<TDeviceCapExtDataType, string>::const_iterator it = extData.find(tType);
		if (extData.end() != it)
		{
			istringstream iss(it->second);
			iss>>val;
			return true;
		}

		return false;
	}
	void SetDoubleData(const TDeviceCapExtDataType &tType, double val)
	{
		ostringstream oss;
		oss.precision(18);
		oss<<val;
		extData[tType] = oss.str();
	}

private:
	map<TDeviceCapExtDataType, string> extData;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceCapIndexName
{
    TDeviceCapIndexName()
    {
        index = -1;
    }
    int index;   // 设备能力索引
    string name; // 设备能力索引对应的名称
	TDeviceCapExtData extData;	//设备能力对应的扩展信息
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceCapName
{
    UUIDString deviceId;  // 设备编号
	UUIDString devDomainId; // 设备域编号
    int capId;            // 能力标识, 也即能力类型
    vector<TDeviceCapIndexName> deviceCapIndexNames;

	void Clear()
	{
		deviceId.clear();
		devDomainId.clear();
		capId = DEVICE_CAP_VIDEOSOURCE;
		deviceCapIndexNames.clear();
	}

    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }

	const UUIDString& GetDevDomainId() const
	{
		return devDomainId;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum TTacUserType
{// hzytodo
    enTacUserType_SuperAdmin
};

enum TSyncDataType
{
    enDataType_PlatformRelation = 1, // 上下级关系
    enDataType_Device = 2,           // 设备
    enDataType_DeviceGroup = 3,      // 设备分组
    enDataType_DeviceGroupDevice = 4,// 设备分组划归信息

    enDataType_Tmp_Device = 5,       // 临时授权设备信息
    enDataType_Tmp_UserDevice = 6,   // 临时授权设备划归
};
enum TSyncOperateType
{
    enOperateType_Add = 1,  // 添加
    enOperateType_Del = 2,  // 删除
    enOperateType_Mod = 3,  // 修改
};

// 设备属性标志
class TDevicePropertyFlags: public CBitsContainer
{
public:
    TDevicePropertyFlags(){}
    enum TPropertyFlag
    {
        em_PropertyFlags_PrivateDev = 1,              // 私有设备
        em_PropertyFlags_P2PDev_DISABLE_REASSIGN = 2, // 授权设备且禁止二次划归
        em_PropertyFlags_P2PDev_ABLE_REASSIGN = 3,    // 授权设备且允许二次划归
    };
    void SetPropertyFlag(TDevicePropertyFlags::TPropertyFlag flag)
    {
        SetBit(flag-1);
    }
    bool HasPropertyFlag(TDevicePropertyFlags::TPropertyFlag flag) const
    {
        return IsBitOn(flag-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef s32 TCreater;
#define CREATE_BY_PLAT                  0           // 由平台内部创建
#define CREATE_BY_CMSSDK                1           // 由cmssdk创建
#define CREATE_BY_GBS                   2           // 由gbs创建，只限本域设备
#define CREATE_BY_GBS_FOREIGN_DOMAIN    3           // 由gbs创建，只限异域设备

typedef string TDevExtDataType;
#define DEV_EXT_FIXADDR					"fixAddr"	// 设备安装位置
#define DEV_EXT_KDMNO					"kdmNO"		// 1.0设备编号
#define DEV_EXT_GBID					"gbId"		// 国标编号

struct TDevExtData
{
public:
	TDevExtData()
	{
	}

	void Clear()
	{
		extData.clear();
	}

	const map<TDevExtDataType, string> &GetExtData() const
	{
		return extData;
	}
	map<TDevExtDataType, string> &GetExtData()
	{
		return extData;
	}
	void SetExtData(const map<TDevExtDataType, string> &val)
	{
		extData = val;
	}

private:
	map<TDevExtDataType, string> extData;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceInfo
{
public:
    CDeviceInfo()
    {
        type = DEVICE_TYPE_INVALID;
        usedFlag = true;
        netType = DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
        creater = CREATE_BY_PLAT;
    }

	void Clear()
	{
		deviceId.clear();
		domainId.clear();
		name.clear();
		alias.clear();
		type = DEVICE_TYPE_INVALID;
		modelId.clear();
		propertyFlags.Clear();
		modelName.clear();
		manufacturer.clear();
		modelCapDetails.clear();
		usedFlag = true;
		netType = DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
		netAddress.clear();
		description.clear();
		expireDate.clear();
		createDate.clear();
		creater = CREATE_BY_PLAT;
		devData.Clear();
		groupId.clear();
	}

    const UUIDString& GetDeviceUUID() const
    {
        return deviceId;
    }
    UUIDString& GetDeviceUUID() 
    {
        return deviceId;
    }
    void SetDeviceUUID(const UUIDString &val)
    {
        deviceId = val;
    }
    const UUIDString& GetDomainUUID() const
    {
        return domainId;
    }
    UUIDString& GetDomainUUID()
    {
        return domainId;
    }
    void SetDomainUUID(const UUIDString &val)
    {
        domainId = val;
    }
    const string& GetDeviceName() const
    {
        return name;
    }
    string& GetDeviceName() 
    {
        return name;
    }
    void SetDeviceName(const string &val)
    {
        name = val;
    }
    const string& GetDeviceAlias() const
    {
        return alias;
    }
    string& GetDeviceAlias()
    {
        return alias;
    }
    void SetDeviceAlias(const string &val)
    {
        alias = val;
    }
	int GetDeviceType() 
    {
		return type;
    }
    int GetDeviceType() const
    {
        return type;
    }
    void SetDeviceType(int val)
    {
        type = val;
    }
    const UUIDString& GetDeviceModel() const
    {
        return modelId;
    }
    UUIDString& GetDeviceModel() 
    {
        return modelId;
    }
    void SetDeviceModel(const UUIDString &val)
    {
        modelId = val;
    }
    const TDevicePropertyFlags& GetDevicePropertyFlags() const
    {
        return propertyFlags;
    }
    TDevicePropertyFlags& GetDevicePropertyFlags()
    {
        return propertyFlags;
    }
    void SetPropertyFlags(const TDevicePropertyFlags& val)
    {
        propertyFlags = val;
    }
    vector<TDeviceCapInfo>& GetDeviceModelCapDetail()
    {
        return modelCapDetails;
    }
    const vector<TDeviceCapInfo>& GetDeviceModelCapDetail() const
    {
        return modelCapDetails;
    }
	void SetDeviceModelCapDetail(const vector<TDeviceCapInfo>& val)
	{
		modelCapDetails = val;
	}
	bool GetUsedFlag() 
    {
		return usedFlag;
    }
	const bool GetUsedFlag() const
    {
        return usedFlag;
    }
    void SetUsedFlag(bool val)
    {
        usedFlag = val;
    }
	int GetNetType() 
    {
		return netType;
    }
    int GetNetType() const
    {
        return netType;
    }
    void SetNetType(int val)
    {
        netType = val;
    }
    const string& GetNetAddress() const
    {
        return netAddress;
    }
    string& GetNetAddress()
    {
        return netAddress;
    }
    void SetNetAddress(const string &val)
    {
        netAddress = val;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    void SetManufacturer(const string &val)
    {
        manufacturer = val;
    }
    const string& GetModelName() const
    {
        return modelName;
    }
    string& GetModelName()
    {
        return modelName;
    }
    void SetModelName(const string &val)
    {
        modelName = val;
    }
    const string& GetDescription() const
    {
        return description;
    }
    string& GetDescription()
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    const string& GetExpireDate() const
    {
        return expireDate;
    }
    string& GetExpireDate()
    {
        return expireDate;
    }
    void SetExpireDate(const string &val)
    {
        expireDate = val;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    string& GetCreateDate()
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    TCreater GetCreater() const
    {
        return creater;
    }
    void SetCreater(TCreater val)
    {
        creater = val;
    }

	const TDevExtData &GetDevData() const
	{
		return devData;
	}
	TDevExtData &GetDevData()
	{
		return devData;
	}
	void SetDevData(const TDevExtData &val)
	{
		devData = val;
	}

	const UUIDString &GetOwner() const
	{
		return owner;
	}
	UUIDString &GetOwner()
	{
		return owner;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
	}

	void SetGroupId(const UUIDString& strGroupId)
	{
		groupId = strGroupId;
	}
	UUIDString& GetGroupId()
	{
		return groupId;
	}
	const UUIDString& GetGroupId() const
	{
		return groupId;
	}
private:
    UUIDString deviceId; // 设备UUIDString
    UUIDString domainId; // 设备所属域编号
    string name;      // 设备名
    string alias;     // 设备别名
    int type;         // 设备类型：参见常量定义
    UUIDString modelId; // 设备型号
    TDevicePropertyFlags propertyFlags; // 设备属性标志
    string modelName;    // 设备型号名称，与设备型号相关联，出于方便考虑，3AS根据设备型号动态拼接出来
    string manufacturer; // 设备制造商, 与设备型号相关联，出于方便考虑，3AS根据设备型号动态拼接出来
    vector<TDeviceCapInfo> modelCapDetails; // 设备型号具体能力内容，出于方便的考虑，3AS根据设备型号动态拼接出来
    bool usedFlag; // 启停状态标志：0停用，非0启用
    int netType;   // 接入线路方式：参见常量定义
    string netAddress;   // 设备网络地址，IP或域名
    string description;  // 设备描述
    string expireDate;   // 使用截止日期
    string createDate;   // 入网日期
    TCreater creater;    // 创建者
	TDevExtData devData; // 设备扩展信息
	UUIDString owner;    // 所属者
	UUIDString groupId;  // 设备所属组编号（用于指定设备组入网）
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CDeviceInfoQueryCondition
{
public:
    CDeviceInfoQueryCondition()
    {
        usedFlag = true;
        netType = DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
    }
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetDeviceUUID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetDeviceUUID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetDeviceUUIDBegin(UUIDString &val) const
    {
        if (!optionField.HasOption("idBegin"))
            return false;
        val = idBegin;
        return true;
    }
    void SetDeviceUUIDBegin(const UUIDString &val)
    {
        idBegin = val;
        optionField.SetOption("idBegin");
    }
    bool GetDomainId(UUIDString &val) const
    {
        if (!optionField.HasOption("domainId"))
            return false;
        val = domainId;
        return true;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
        optionField.SetOption("domainId");
    }
    bool GetDeviceName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetDeviceName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    /*bool GetDeviceNameBegin(string &val) const
    {
        if (!optionField.HasOption("nameBegin"))
            return false;
        val = nameBegin;
        return true;
    }
    void SetDeviceNameBegin(const string &val)
    {
        nameBegin = val;
        optionField.SetOption("nameBegin");
    }*/
    bool GetDeviceAlias(string &val) const
    {
        if (!optionField.HasOption("alias"))
            return false;
        val = alias;
        return true;
    }
    void SetDeviceAlias(const string &val)
    {
        alias = val;
        optionField.SetOption("alias");
    }
    bool GetDeviceType(set<int> &val) const
    {
        if (!optionField.HasOption("types"))
            return false;
        val = types;
        return true;
    }
    void SetDeviceType(const set<int> &val)
    {
        types = val;
        optionField.SetOption("types");
    }
    bool GetDeviceModelId(UUIDString &val) const
    {
        if (!optionField.HasOption("modelId"))
            return false;
        val = modelId;
        return true;
    }
    void SetDeviceModelId(const UUIDString &val)
    {
        modelId = val;
        optionField.SetOption("modelId");
    }
    bool GetUsedFlag(bool &val) const
    {
        if (!optionField.HasOption("usedFlag"))
            return false;
        val = usedFlag;
        return true;
    }
    void SetUsedFlag(bool val)
    {
        usedFlag = val;
        optionField.SetOption("usedFlag");
    }
    bool GetNetType(int &val) const
    {
        if (!optionField.HasOption("netType"))
            return false;
        val = netType;
        return true;
    }
	void SetNetType(int val)
    {
        netType = val;
        optionField.SetOption("netType");
    }
    bool GetNetAddress(string &val) const
    {
        if (!optionField.HasOption("netAddress"))
            return false;
        val = netAddress;
        return true;
    }
    void SetNetAddress(const string &val)
    {
        netAddress = val;
        optionField.SetOption("netAddress");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("expireDate_From"))
            return false;
        val = expireDate_From;
        return true;
    }
    void SetExpireDate_From(const string &val)
    {
        expireDate_From = val;
        optionField.SetOption("expireDate_From");
    }
    bool GetExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("expireDate_To"))
            return false;
        val = expireDate_To;
        return true;
    }
    void SetExpireDate_To(const string &val)
    {
        expireDate_To = val;
        optionField.SetOption("expireDate_To");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
    bool GetCreaters(set<TCreater> &val) const
    {
        if (!optionField.HasOption("creaters"))
            return false;
        val = creaters;
        return true;
    }
    void SetCreaters(const set<TCreater> &val)
    {
        creaters = val;
        optionField.SetOption("creaters");
    }
	bool GetOwner(UUIDString &val) const
	{
		if (!optionField.HasOption("owner"))
			return false;
		val = owner;
		return true;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
		optionField.SetOption("owner");
	}
	bool GetNeedSearchIp(string &val) const
	{
		if (!optionField.HasOption("needSearchIp"))
			return false;
		val = needSearchIp;
		return true;
	}
	void SetNeedSearchIp(const string &val)
	{
		needSearchIp = val;
		optionField.SetOption("needSearchIp");
	}
private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString cuserID;          // 发起查询的CU用户编号，指定时能够限制返回的数据为该用户权限范围内的

    UUIDString id; // 设备Id
    UUIDString idBegin; // 设备Id大于idBegin
    UUIDString domainId; // 所属域名
    string name;   // 设备名
    //string nameBegin; // 设备名大于nameBegin  指定它时同时要求指定idBegin字段，因为设备名不唯一
    string alias;  // 设备别名
    set<int> types;     // 设备类型集合（支持一次指定多个类型）
    UUIDString modelId; // 设备型号ID
    bool usedFlag; // 启停状态标志：0停用，非0启用
    int netType;   // 接入线路方式：参见常量定义
    string netAddress; // 设备网络地址，IP或域名
    string description;  // 设备描述
    string expireDate_From; // 使用截止日期_查询起点
    string expireDate_To;   // 使用截止日期_查询终点
    string createDate_From; // 入网日期_查询起点
    string createDate_To;   // 入网日期_查询终点
    set<TCreater> creaters;       // 创建者集合（支持一次指定多个创建者）
	UUIDString owner;         // 所属者
	string needSearchIp;                     // 是否需搜索网络地址netAddress字段，可取值："true"/"false"
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceCapIndexNameQryCondition
{
public:
    CDeviceCapIndexNameQryCondition(){}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceModelInfo
{
public:
    CDeviceModelInfo()
    {
        typeId = DEVICE_TYPE_INVALID;
        creater = CREATE_BY_PLAT;
    }

	void Clear()
	{
		id.clear();
		domainId.clear();
		typeId = DEVICE_TYPE_INVALID;
		name.clear();
		manufacturer.clear();
		capInfos.clear();
		creater = CREATE_BY_PLAT;
	}

    UUIDString& GetDeviceModelId() 
    {
        return id;
    }
    const UUIDString& GetDeviceModelId() const
    {
        return id;
    }
    void SetDeviceModelId(const UUIDString &val)
    {
        id = val;
    }
    int GetDeviceTypeId() 
    {
        return typeId;
    }
    int GetDeviceTypeId() const
    {
        return typeId;
    }
    void SetDeviceTypeId(int val)
    {
        typeId = val;
    }
    UUIDString& GetDomainId() 
    {
        return domainId;
    }
    const UUIDString& GetDomainId() const
    {
        return domainId;
    } 
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
    }
    string& GetDeviceModelName() 
    {
        return name;
    }
    const string& GetDeviceModelName() const
    {
        return name;
    }
    void SetDeviceModelName(const string &val)
    {
        name = val;
    }
    const vector<TDeviceCapInfo>& GetDeviceCapInfo() const
    {
        return capInfos;
    }
    vector<TDeviceCapInfo>& GetDeviceCapInfo()
    {
        return capInfos;
    }
	void SetDeviceCapInfo(const vector<TDeviceCapInfo>& val)
	{
		capInfos = val;
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
    void SetManufacturer(const string &val)
    {
        manufacturer = val;
    }
    TCreater GetCreater() const
    {
        return creater;
    }
    void SetCreater(TCreater val)
    {
        creater = val;
    }

	const UUIDString& GetOwner() const
	{
		return owner;
	}
	UUIDString& GetOwner() 
	{
		return owner;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
	}
private:
    UUIDString id;      // 设备型号编号
    UUIDString domainId; // 所属域编号
    int typeId;         // 所属设备类型
    string name;         // 设备型号名称
    string manufacturer;  // 设备厂商信息
    vector<TDeviceCapInfo> capInfos; // 设备能力信息
    TCreater creater;       // 创建者
	UUIDString owner;       // 所属者
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceModelQueryCondition
{
public:
    CDeviceModelQueryCondition()
    {
        devTypeId = DEVICE_TYPE_INVALID;
    }
    bool GetModelId(UUIDString &val) const
    {
        if (!optionField.HasOption("modelId"))
            return false;
        val = modelId;
        return true;
    }
    void SetModelId(const UUIDString &val)
    {
        modelId = val;
        optionField.SetOption("modelId");
    }
    bool GetDomainId(UUIDString &val) const
    {
        if (!optionField.HasOption("domainId"))
            return false;
        val = domainId;
        return true;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
        optionField.SetOption("domainId");
    }
    bool GetDevTypeId(int &val) const
    {
        if (!optionField.HasOption("devTypeId"))
            return false;
        val = devTypeId;
        return true;
    }
    void SetDevTypeId(int val)
    {
        devTypeId = val;
        optionField.SetOption("devTypeId");
    }
    bool GetCreater(TCreater &val) const
    {
        if (!optionField.HasOption("creater"))
            return false;
        val = creater;
        return true;
    }
    void SetCreater(TCreater val)
    {
        creater = val;
        optionField.SetOption("creater");
    }

	bool GetOwner(UUIDString &val) const
	{
		if (!optionField.HasOption("owner"))
			return false;
		val = owner;
		return true;
	}
	void SetOwner(UUIDString val)
	{
		owner = val;
		optionField.SetOption("owner");
	}

	void SetModelName(const string& strModelName)
	{
		modelName = strModelName;
		optionField.SetOption("modelName");
	}
	bool GetModelName(string &val) const
	{
		if (!optionField.HasOption("modelName"))
			return false;
		val = modelName;
		return true;
	}
private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString modelId;
    UUIDString domainId;    // 所属域编号
    int devTypeId;          // 所属设备类型
    TCreater creater;       // 创建者
	UUIDString owner;       // 所属者
	string modelName;       //设备型号，支持模糊查询             
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CDeviceManufacturerInfo
{
public:
    CDeviceManufacturerInfo()
    {
        buildin = true;
    }

	void Clear()
	{
		manufacturer.clear();
		buildin = false;
	}

    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
    void SetManufacturer(const string &val)
    {
        manufacturer = val;
    }
    bool GetBuildIn() 
    {
        return buildin;
    }
	bool GetBuildIn() const
    {
        return buildin;
    }
    void SetBuildIn(bool val)
    {
        buildin = val;
    }
private:
    string manufacturer;
    bool buildin; // 是否为内置厂商，内置厂商不能删除
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserInfo
{
public:
    CUserInfo()
    {
        type = CUSER_TYPE_SUPERADMIN;
        mLoginFlag = true;
        enabledFlag = true;
		pwdExpireEnabledFlag = false;
		pwdExpireTime = 90;
		ipListEnabledFlag = false;
		ipBindFlagForMobileClient = false;
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 1;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
		waterMarkColor = 16711680;
		waterMarkFont = "";
    }

	void Clear()
	{
		id.clear();
		grpID.clear();
		name.clear();
		password.clear();
		type = CUSER_TYPE_SUPERADMIN;
		mLoginFlag = true;
		enabledFlag = true;
		description.clear();
		nameExpireDate.clear();
		pwdExpireDate.clear();
		pwdExpireEnabledFlag = false;
		pwdExpireTime = 90;
		createDate.clear();
		creatorID.clear();
		privilege.Clear();
		deviceTreeVer.clear();
		loginIpList.clear();
		ipListEnabledFlag = false;
		ipBindFlagForMobileClient = false;
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 1;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
		waterMarkColor = 16711680;
		waterMarkFont = "";
	}

    UUIDString& GetUserID() 
    {
        return id;
    }
    const UUIDString& GetUserID() const
    {
        return id;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
    }
    UUIDString& GetUsergrpID() 
    {
        return grpID;
    }
    const UUIDString& GetUsergrpID() const
    {
        return grpID;
    }
    void SetUsergrpID(const UUIDString &val)
    {
        grpID = val;
    }
    string& GetUserName() 
    {
        return name;
    }
    const string& GetUserName() const
    {
        return name;
    }
    void SetUserName(const string &val)
    {
        name = val;
    }
    string& GetUserPwd() 
    {
        return password;
    }
    const string& GetUserPwd() const
    {
        return password;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
    }
    int GetUserType() 
    {
        return type;
    }
    int GetUserType() const
    {
        return type;
    }
    void SetUserType(int val)
    {
        type = val;
    }
    bool GetMLoginFlag() 
    {
        return mLoginFlag;
    }
	const bool GetMLoginFlag() const
    {
        return mLoginFlag;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
    }
    bool GetEnabledFlag() 
    {
        return enabledFlag;
    }
    bool GetEnabledFlag() const
    {
        return enabledFlag;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
    }
    string& GetDescription()
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    string& GetNameExpireDate() 
    {
        return nameExpireDate;
    }
    const string& GetNameExpireDate() const
    {
        return nameExpireDate;
    }
    void SetNameExpireDate(const string &val)
    {
        nameExpireDate = val;
    }
    string& GetPwdExpireDate() 
    {
        return pwdExpireDate;
    }
    const string& GetPwdExpireDate() const
    {
        return pwdExpireDate;
    }
    void SetPwdExpireDate(const string &val)
    {
        pwdExpireDate = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    UUIDString& GetCreatorID()
    {
        return creatorID;
    }
    const UUIDString& GetCreatorID() const
    {
        return creatorID;
    }
    void SetCreatorID(const UUIDString &val)
    {
        creatorID = val;
    }
    CUserPrivilege& GetUserPrivilege() 
    {
        return privilege;
    }
    const CUserPrivilege& GetUserPrivilege() const
    {
        return privilege;
    }
    void SetUserPrivilege(const CUserPrivilege &val)
    {
        privilege = val;
    }
    void SetDeviceTreeVer(const string& strDeviceTreeVer)
    {
        deviceTreeVer = strDeviceTreeVer;
    }
    string& GetDeviceTreeVer()
    {
        return deviceTreeVer;
    }
    const string& GetDeviceTreeVer() const
    {
        return deviceTreeVer;
    }

	void SetPwdExpireEnabledFlag(bool bPwdExpireEnabledFlag)
	{
		pwdExpireEnabledFlag = bPwdExpireEnabledFlag;
	}
	bool GetPwdExpireEnabledFlag() const
	{
		return pwdExpireEnabledFlag;
	}

	void SetPwdExpireTime(int nPwdExpireTime)
	{
		pwdExpireTime = nPwdExpireTime;
	}
	int GetPwdExpireTime() const
	{
		return pwdExpireTime;
	}

	void SetLoginIpList(const string& strLoginIpList)
	{
		loginIpList = strLoginIpList;
	}
	string& GetLoginIpList()
	{
		return loginIpList;
	}
	const string& GetLoginIpList() const
	{
		return loginIpList;
	}
	void SetIpListEnabledFlag(bool bIpListEnabledFlag)
	{
		ipListEnabledFlag = bIpListEnabledFlag;
	}
	bool GetIpListEnabledFlag() const
	{
		return ipListEnabledFlag;
	}
	void SetIpBindFlagForMobileClient(bool bIpBindFlagForMobileClient)
	{
		ipBindFlagForMobileClient = bIpBindFlagForMobileClient;
	}
	bool GetIpBindFlagForMobileClient() const
	{
		return ipBindFlagForMobileClient;
	}
	void SetSwitchNum(int nSwitchNum)
	{
		switchNum = nSwitchNum;
	}
	int GetSwitchNum() const
	{
		return switchNum;
	}

	void SetWaterMarkEnabled(bool bWaterMarkEnabled)
	{
		waterMarkEnabled = bWaterMarkEnabled;
	}
	bool GetWaterMarkEnabled() const
	{
		return waterMarkEnabled;
	}

	void SetWaterMarkFont(const string& strWaterMarkFont)
	{
		waterMarkFont = strWaterMarkFont;
	}
	string& GetWaterMarkFont()
	{
		return waterMarkFont;
	}
	const string& GetWaterMarkFont() const
	{
		return waterMarkFont;
	}

	void SetWaterMarkFontSize(int nWaterMarkFontSize)
	{
		waterMarkFontSize = nWaterMarkFontSize;
	}
	int GetWaterMarkFontSize() const
	{
		return waterMarkFontSize;
	}

	void SetWaterMarkRotationAngle(int nWaterMarkRotationAngle)
	{
		waterMarkRotationAngle = nWaterMarkRotationAngle;
	}
	int GetWaterMarkRotationAngle() const
	{
		return waterMarkRotationAngle;
	}

	void SetWaterMarkColor(int nWaterMarkColor)
	{
		waterMarkColor = nWaterMarkColor;
	}
	int GetWaterMarkColor() const
	{
		return waterMarkColor;
	}

	void SetWaterMarkTransparent(int nWaterMarkTransparent)
	{
		waterMarkTransparent = nWaterMarkTransparent;
	}
	int GetWaterMarkTransparent() const
	{
		return waterMarkTransparent;
	}

	void SetWaterMarkTiled(bool bWaterMarkTiled)
	{
		waterMarkTiled = bWaterMarkTiled;
	}
	bool GetWaterMarkTiled() const
	{
		return waterMarkTiled;
	}
private:
    UUIDString id;    // 用户编号
    UUIDString grpID; // 用户所属组
    string name;     // 用户名
    string password; // 登陆密码, 密文形式
    int type;        // CU用户类型：参见常量定义
    bool mLoginFlag; // 多次登陆标志, 1表示允许多人登录
    bool enabledFlag;   // 启停状态标志
    string description; // 描述
    string nameExpireDate; // 帐户使用截止日期
    string pwdExpireDate;  // 密码使用截止日期
	bool   pwdExpireEnabledFlag; //密码有效期是否启用
	int    pwdExpireTime;   //密码有效期时间
	string loginIpList;    //绑定的IP地址
	bool   ipListEnabledFlag; //绑定IP列表是否启用              
    string createDate;    // 创建日期
    UUIDString creatorID; // 创建者编号
    CUserPrivilege privilege;
    string deviceTreeVer; // 设备树版本号
	bool ipBindFlagForMobileClient;//移动客户端ip绑定是否启用                
	int switchNum;               //码流并发数
	//视频水印功能
	bool waterMarkEnabled;                    //是否启用水印
	string waterMarkFont;                     //水印字体
	int waterMarkFontSize;                    //字体大小
	int waterMarkRotationAngle;               //旋转角度
	int waterMarkColor;                       //颜色
	int waterMarkTransparent;                 //透明度
	bool waterMarkTiled;                      //是否平铺

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CUserInfoQueryCondition
{
public:
    CUserInfoQueryCondition()
    {
        type = CUSER_TYPE_SUPERADMIN;
        mLoginFlag = true;
        enabledFlag = true;
		pwdExpireEnableFlag = false;
    }
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetUserName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetUserName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetUserPwd(string &val) const
    {
        if (!optionField.HasOption("password"))
            return false;
        val = password;
        return true;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
        optionField.SetOption("password");
    }
    bool GetUserType(int &val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetUserType(int val)
    {
        type = val;
        optionField.SetOption("type");
    }
    bool GetMLoginFlag(bool &val) const
    {
        if (!optionField.HasOption("mLoginFlag"))
            return false;
        val = mLoginFlag;
        return true;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
        optionField.SetOption("mLoginFlag");
    }
    bool GetEnabledFlag(bool &val) const
    {
        if (!optionField.HasOption("enabledFlag"))
            return false;
        val = enabledFlag;
        return true;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
        optionField.SetOption("enabledFlag");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetCreatorNO(UUIDString &val) const
    {
        if (!optionField.HasOption("creatorNO"))
            return false;
        val = creatorNO;
        return true;
    }
    void SetCreatorNO(const UUIDString &val)
    {
        creatorNO = val;
        optionField.SetOption("creatorNO");
    }
    bool GetNameExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_From"))
            return false;
        val = nameExpireDate_From;
        return true;
    }
    void SetNameExpireDate_From(const string &val)
    {
        nameExpireDate_From = val;
        optionField.SetOption("nameExpireDate_From");
    }
    bool GetNameExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_To"))
            return false;
        val = nameExpireDate_To;
        return true;
    }
    void SetNameExpireDate_To(const string &val)
    {
        nameExpireDate_To = val;
        optionField.SetOption("nameExpireDate_To");
    }
    bool GetPwdExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_From"))
            return false;
        val = pwdExpireDate_From;
        return true;
    }
    void SetPwdExpireDate_From(const string &val)
    {
        pwdExpireDate_From = val;
        optionField.SetOption("pwdExpireDate_From");
    }
    bool GetPwdExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_To"))
            return false;
        val = pwdExpireDate_To;
        return true;
    }
    void SetPwdExpireDate_To(const string &val)
    {
        pwdExpireDate_To = val;
        optionField.SetOption("pwdExpireDate_To");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
	void SetPwdExpireEnableFlag(bool bPwdExpireEnableFlag)
	{
		pwdExpireEnableFlag = bPwdExpireEnableFlag;
		optionField.SetOption("pwdExpireEnableFlag");
	}
	bool GetPwdExpireEnableFlag(bool &val) const
	{
		if (!optionField.HasOption("pwdExpireEnableFlag"))
			return false;
		val = pwdExpireEnableFlag;
		return true;
	}

	void SetPwdExpireTime(s32 nPwdExpireTime)
	{
		pwdExpireTime = nPwdExpireTime;
		optionField.SetOption("pwdExpireTime");
	}
	bool GetPwdExpireTime(s32 &val) const
	{
		if (!optionField.HasOption("pwdExpireTime"))
			return false;
		val = pwdExpireTime;
		return true;
	}

	void SetLoginIpList(const string& strLoginIpList)
	{
		loginIpList = strLoginIpList;
		optionField.SetOption("loginIpList");
	}

	bool GetLoginIpList(string &val) const
	{
		if (!optionField.HasOption("loginIpList"))
			return false;
		val = loginIpList;
		return true;
	}

private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString cuserID;          // 发起查询的CU用户编号，指定时能够限制返回的数据为该用户权限范围内的(目前指定该字段时不支持下面的条件过滤!!!)

    UUIDString id; // 用户编号
    string name; // 用户名
    string password; // 登陆密码, 密文形式
    int type; // CU用户类型：参见常量定义
    bool mLoginFlag; // 多次登陆标志
    bool enabledFlag; // 启停状态标志
	bool pwdExpireEnableFlag;  //密码有效期启停标志
	s32  pwdExpireTime;   //密码有效期时间，单位：天
	string loginIpList; //绑定的IP                             
    string description; // 描述
    UUIDString creatorNO; // 创建者编号
    string nameExpireDate_From; //帐户使用截止日期_查询起点
    string nameExpireDate_To; //帐户使用截止日期_查询终点
    string pwdExpireDate_From; //密码使用截止日期_查询起点
    string pwdExpireDate_To; //密码使用截止日期_查询终点
    string createDate_From; //帐户创建日期_查询起点
    string createDate_To; //帐户创建日期_查询终点
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CUserDeviceInfo
{
public:
    CUserDeviceInfo(){}

	void Clear()
	{
		userID.clear();
		deviceID.clear();
		devDomainID.clear();
		devicePrivilege.Clear();
		deviceCapPrivilege.clear();
	}

    UUIDString& GetUserID()
    {
        return userID;
    }
    const UUIDString& GetUserID() const
    {
        return userID;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
    }
    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    UUIDString& GetDevDomainID()
    {
        return devDomainID;
    }
    const UUIDString& GetDevDomainID() const
    {
        return devDomainID;
    }
    void SetDevDomainID(const UUIDString& val)
    {
        devDomainID = val;
    }
    
    CDevicePrivilege& GetDevicePrivilege() 
    {
        return devicePrivilege;
    }
    const CDevicePrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }
    void SetDevicePrivilege(const CDevicePrivilege &val)
    {
        devicePrivilege = val;
    }
    vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege()
    {
        return deviceCapPrivilege;
    }
    const vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege() const
    {
        return deviceCapPrivilege;
    }
    void SetDeviceCapPrivilege(const vector<TDeviceCapPrivilege>& val)
    {
        deviceCapPrivilege = val;
    }

private:
    UUIDString userID;
    UUIDString deviceID;
    UUIDString devDomainID; // 设备所属域Id, 发给客户端时TAS拼装出来
    CDevicePrivilege devicePrivilege;
    vector<TDeviceCapPrivilege> deviceCapPrivilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserDeviceInfoKey
{
public:
    CUserDeviceInfoKey(){}
    UUIDString& GetUserID()
    {
        return userID;
    }
    const UUIDString& GetUserID() const
    {
        return userID;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
    }
    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    vector<TDeviceCapIndexs>& GetDeviceCapIndexs()
    {
        return deviceCapset;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapset;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs> &val)
    {
        deviceCapset = val;
    }
private:
    UUIDString userID;
    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapset;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserDeviceInfoQueryCondition
{
public:
    CUserDeviceInfoQueryCondition()
    {
        capID = DEVICE_CAP_INVALID;
        capIndex = -1;
    }
    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("userID"))
            return false;
        val = userID;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        userID = val;
        optionField.SetOption("userID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetCapID(int &val) const
    {
        if (!optionField.HasOption("capID"))
            return false;
        val = capID;
        return true;
    }
    void SetCapID(int val)
    {
        capID = val;
        optionField.SetOption("capID");
    }
    bool GetCapIndex(int &val) const
    {
        if (!optionField.HasOption("capIndex"))
            return false;
        val = capIndex;
        return true;
    }
    void SetCapIndex(int val)
    {
        capIndex = val;
        optionField.SetOption("capIndex");
    }

    bool GetDevGrpID(UUIDString &val) const
    {
        if (!optionField.HasOption("devGrpID"))
            return false;
        val = devGrpID;
        return true;
    }
    void SetDevGrpID(const UUIDString &val)
    {
        devGrpID = val;
        optionField.SetOption("devGrpID");
    }

	bool GetDevTreeID(UUIDString& val) const
	{
		if (!optionField.HasOption("devTreeID"))
			return false;
		val = devTreeID;
		return true;
	}

	void SetDevTreeID(const UUIDString& val)
	{
		devTreeID = val;
		optionField.SetOption("devTreeID");
	}

private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString userID;
    UUIDString deviceID;
    int capID;    // 设备能力ID
    int capIndex; // 设备能力索引

    UUIDString devGrpID;	// 设备所属组限制
	UUIDString devTreeID;	// 设备树ID 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum TDevGroupType
{
	TDEV_GROUP_TYPE_UNKNOWN = 0,    //未知类型，兼容老版本cu，后台统一按照行政区划子分组处理
	TDEV_GROUP_TYPE_CIVILCODE ,     //行政区划子分组
	TDEV_GROUP_TYPE_VIRTUALGRP,     //虚拟组织子分组
};

class CDeviceGroupInfo
{
public:
    CDeviceGroupInfo()
    {
        creater = CREATE_BY_PLAT;
		grpseq=0.0;
		Clear();
    }

	void Clear()
	{
		id.clear();
		parentId.clear();
		domainUUID.clear();
		parentDomainUUID.clear();
		name.clear();
		description.clear();
		createDate.clear();
		creater = CREATE_BY_PLAT;
		owner.clear();
		grpseq=0.0;
		fullpath.clear();
		devtreeId.clear();
		oldgbid.clear();
		newgbid.clear();
		grpType = TDEV_GROUP_TYPE_UNKNOWN;
	}

    bool IsAuthorizationDevGrp() const
    {
        return 0 == strcmp(description.c_str(), "domain_authorization_device_group");
    }

    UUIDString& GetDeviceGroupID() 
    {
        return id;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return id;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        id = val;
    }
    UUIDString& GetDeviceParentGroupID() 
    {
        return parentId;
    }
    const UUIDString& GetDeviceParentGroupID() const
    {
        return parentId;
    }
    void SetDeviceParentGroupID(const UUIDString &val)
    {
        parentId = val;
    }
    UUIDString& GetDomainUUID()
    {
        return domainUUID;
    }
    const UUIDString& GetDomainUUID() const
    {
        return domainUUID;
    }
    void SetDomainUUID(const UUIDString &val)
    {
        domainUUID = val;
    }
	UUIDString& GetParentDomainUUID()
	{
		return parentDomainUUID;
	}
	const UUIDString& GetParentDomainUUID() const
	{
		return parentDomainUUID;
	}
	void SetParentDomainUUID(const UUIDString &val)
	{
		parentDomainUUID = val;
	}
    string& GetDeviceGroupName()
    {
        return name;
    }
    const string& GetDeviceGroupName() const
    {
        return name;
    }
    void SetDeviceGroupName(const string &val)
    {
        name = val;
    }
    string& GetDeviceGroupDesc() 
    {
        return description;
    }
    const string& GetDeviceGroupDesc() const
    {
        return description;
    }
    void SetDeviceGroupDesc(const string &val)
    {
        description = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    TCreater GetCreater() const
    {
        return creater;
    }
    void SetCreater(TCreater val)
    {
        creater = val;
    }

	const UUIDString& GetOwner() const
	{
		return owner;
	}
	UUIDString& GetOwner() 
	{
		return owner;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
	}

	void SetGrpseq(double nGrpseq)
	{
		grpseq = nGrpseq;
	}
	double GetGrpseq() const
	{
		return grpseq;
	}

	void SetFullpath(const string& strFullpath)
	{
		fullpath = strFullpath;
	}
	string& GetFullpath()
	{
		return fullpath;
	}
	const string& GetFullpath() const
	{
		return fullpath;
	}

	void SetDevtreeId(const UUIDString& strDevtreeId)
	{
		devtreeId = strDevtreeId;
	}
	UUIDString& GetDevtreeId()
	{
		return devtreeId;
	}
	const UUIDString& GetDevtreeId() const
	{
		return devtreeId;
	}
private:
    UUIDString id;
    UUIDString parentId;   // 组为域内根组时，该值为空
    UUIDString domainUUID; // 设备组所属域编号
	UUIDString parentDomainUUID;	// 父组所属域编号
    string name;
    string description;
    string createDate;
    TCreater creater;       // 创建者
	UUIDString owner;       // 所属者
	double     grpseq;       //设备组在所在层级的序号
	string	fullpath;        //TAS内部使用，其他模块不要使用
	UUIDString devtreeId;    //设备组所属设备树Id
	string  oldgbid;                                //国标选择推送专用，用于修改分组ID
	string  newgbid;                
	TDevGroupType grpType;                  //分组类型：仅国标选择推送树有效
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetOldgbid(const string& strOldgbid)
    {
        oldgbid = strOldgbid;
    }
    string& GetOldgbid()
    {
        return oldgbid;
    }
    const string& GetOldgbid() const
    {
        return oldgbid;
    }

    void SetNewgbid(const string& strNewgbid)
    {
        newgbid = strNewgbid;
    }
    string& GetNewgbid()
    {
        return newgbid;
    }
    const string& GetNewgbid() const
    {
        return newgbid;
    }
public:
    void SetGrpType(TDevGroupType eGrpType)
    {
        grpType = eGrpType;
    }
    TDevGroupType GetGrpType() const
    {
        return grpType;
    }

};

class CDeviceGroupInfoKey
{
public:
    CDeviceGroupInfoKey(){}

    UUIDString& GetDeviceGroupID() 
    {
        return id;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return id;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        id = val;
    }

	void SetDevtreeId(const UUIDString& strDevtreeId)
	{
		devtreeId = strDevtreeId;
	}
	UUIDString& GetDevtreeId()
	{
		return devtreeId;
	}
	const UUIDString& GetDevtreeId() const
	{
		return devtreeId;
	}
private:
    UUIDString id;
	UUIDString devtreeId;               // 设备组所属设备树ID [3/10/2016 pangubing]
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CDeviceGroupInfoQueryCondition
{
public:
    CDeviceGroupInfoQueryCondition()
    {
    }

    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetDeviceGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetDeviceParentGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("parentId"))
            return false;
        val = parentId;
        return true;
    }
    void SetDeviceParentGroupID(const UUIDString &val)
    {
        parentId = val;
        optionField.SetOption("parentId");
    }
    bool GetDomainUUID(UUIDString &val) const
    {
        if (!optionField.HasOption("domainUUID"))
            return false;
        val = domainUUID;
        return true;
    }
    void SetDomainUUID(const UUIDString &val)
    {
        domainUUID = val;
        optionField.SetOption("domainUUID");
    }
    bool GetDeviceGroupName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetDeviceGroupName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetDeviceGroupDesc(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDeviceGroupDesc(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
    bool GetCreaters(set<TCreater> &val) const
    {
        if (!optionField.HasOption("creaters"))
            return false;
        val = creaters;
        return true;
    }
    void SetCreaters(const set<TCreater> &val)
    {
        creaters = val;
        optionField.SetOption("creaters");
    }

	bool GetOwner(UUIDString &val) const
	{
		if (!optionField.HasOption("owner"))
			return false;
		val = owner;
		return true;
	}
	void SetOwner(const UUIDString &val)
	{
		owner = val;
		optionField.SetOption("owner");
	}

	bool GetDevTreeId(string &val) const
	{
		if (!optionField.HasOption("devtreeId"))
			return false;
		val = devtreeId;
		return true;
	}
	void SetDevTreeId(const string &val)
	{
		devtreeId = val;
		optionField.SetOption("devtreeId");
	}
private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString cuserID;  // 指定此定段时3as计算此组下该CU用户是否拥有设备，否则计算组中是否拥有设备

    UUIDString id;
    UUIDString parentId; // 设置此值时表示只查询它的儿子组，它的值为空时表示查询根组
    UUIDString domainUUID;
    string name;
    string description;
    string createDate_From;
    string createDate_To;
    set<TCreater> creaters;       // 创建者集合（支持一次指定多个创建者）
	UUIDString owner;    // 所属者
	UUIDString devtreeId;   // 设备组所属设备树ID [3/10/2016 pangubing]
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



struct TDeviceGbid4Gb
{
	UUIDString oldprimarygbid;                              
	UUIDString oldsecondarygbid;               
	UUIDString newprimarygbid;               
	UUIDString newsecondarygbid;               
public:

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOldprimarygbid(const UUIDString& strOldprimarygbid)
    {
        oldprimarygbid = strOldprimarygbid;
    }
    UUIDString& GetOldprimarygbid()
    {
        return oldprimarygbid;
    }
    const UUIDString& GetOldprimarygbid() const
    {
        return oldprimarygbid;
    }

    void SetOldsecondarygbid(const UUIDString& strOldsecondarygbid)
    {
        oldsecondarygbid = strOldsecondarygbid;
    }
    UUIDString& GetOldsecondarygbid()
    {
        return oldsecondarygbid;
    }
    const UUIDString& GetOldsecondarygbid() const
    {
        return oldsecondarygbid;
    }

    void SetNewprimarygbid(const UUIDString& strNewprimarygbid)
    {
        newprimarygbid = strNewprimarygbid;
    }
    UUIDString& GetNewprimarygbid()
    {
        return newprimarygbid;
    }
    const UUIDString& GetNewprimarygbid() const
    {
        return newprimarygbid;
    }

    void SetNewsecondarygbid(const UUIDString& strNewsecondarygbid)
    {
        newsecondarygbid = strNewsecondarygbid;
    }
    UUIDString& GetNewsecondarygbid()
    {
        return newsecondarygbid;
    }
    const UUIDString& GetNewsecondarygbid() const
    {
        return newsecondarygbid;
    }

};

struct TDeviceInfo4Gb
{
	UUIDString vidSrcName;               
	int vidIndex;               
	TDeviceGbid4Gb gbid;  //设备的gbid，包括主辅流的old，newgbid                             
public:
    void SetVidSrcName(const UUIDString& strVidSrcName)
    {
        vidSrcName = strVidSrcName;
    }
    UUIDString& GetVidSrcName()
    {
        return vidSrcName;
    }
    const UUIDString& GetVidSrcName() const
    {
        return vidSrcName;
    }

    void SetVidIndex(int nVidIndex)
    {
        vidIndex = nVidIndex;
    }
    int GetVidIndex() const
    {
        return vidIndex;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetGbid(const TDeviceGbid4Gb& cGbid)
    {
        gbid = cGbid;
    }
    TDeviceGbid4Gb& GetGbid()
    {
        return gbid;
    }
    const TDeviceGbid4Gb& GetGbid() const
    {
        return gbid;
    }

};

class CDeviceGroupDeviceInfo4Gb
{
public:
	CDeviceGroupDeviceInfo4Gb(){}
	void Clear()
	{
		devGrpId.clear();
		devDevTreeId.clear();
		devName.clear();
		deviceId.clear();
		devinfo.clear();
	}
private:
	UUIDString devGrpId;                
	UUIDString devDevTreeId;               // 设备所属设备树ID
	UUIDString devDomainId;               
	UUIDString devName;               
	UUIDString deviceId;                
	vector<TDeviceInfo4Gb> devinfo;               
public:
    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetDevDevTreeId(const UUIDString& strDevDevTreeId)
    {
        devDevTreeId = strDevDevTreeId;
    }
    UUIDString& GetDevDevTreeId()
    {
        return devDevTreeId;
    }
    const UUIDString& GetDevDevTreeId() const
    {
        return devDevTreeId;
    }

    void SetDevName(const UUIDString& strDevName)
    {
        devName = strDevName;
    }
    UUIDString& GetDevName()
    {
        return devName;
    }
    const UUIDString& GetDevName() const
    {
        return devName;
    }

    void SetDeviceId(const UUIDString& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    UUIDString& GetDeviceId()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceId() const
    {
        return deviceId;
    }

    void SetDevinfo(const vector<TDeviceInfo4Gb>& cDevinfo)
    {
        devinfo = cDevinfo;
    }
    vector<TDeviceInfo4Gb>& GetDevinfo()
    {
        return devinfo;
    }
    const vector<TDeviceInfo4Gb>& GetDevinfo() const
    {
        return devinfo;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevDomainId(const UUIDString& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    UUIDString& GetDevDomainId()
    {
        return devDomainId;
    }
    const UUIDString& GetDevDomainId() const
    {
        return devDomainId;
    }

};
class CDeviceGroupDevSeqInfo
{               
	UUIDString devGrpId;               
	UUIDString devId;
	int        vidSrcIdx;               
	double     devSeq;                       
public:
    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVidSrcIdx(int nVidSrcIdx)
    {
        vidSrcIdx = nVidSrcIdx;
    }
    int GetVidSrcIdx() const
    {
        return vidSrcIdx;
    }

    void SetDevSeq(double nDevSeq)
    {
        devSeq = nDevSeq;
    }
    double GetDevSeq() const
    {
        return devSeq;
    }

};
class CDeviceGroupDeviceInfo
{
public:
    CDeviceGroupDeviceInfo(){}

	void Clear()
	{
		groupId.clear();
		deviceId.clear();
		devGrpId.clear();
		domainId.clear();
		deviceCapIndexs.clear();
		grpDevTreeId.clear();         
		devDevTreeId.clear();
	}

    UUIDString& GetDeviceGroupID() 
    {
        return groupId;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return groupId;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        groupId = val;
    }
    UUIDString& GetDeviceID()
    {
        return deviceId;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceId;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceId = val;
    }

	void SetDevGrpId(const UUIDString& strDevGrpId)
	{
		devGrpId = strDevGrpId;
	}
	UUIDString& GetDevGrpId()
	{
		return devGrpId;
	}
	const UUIDString& GetDevGrpId() const
	{
		return devGrpId;
	}

    vector<TDeviceCapIndexs>& GetDeviceCapIndexs()
    {
        return deviceCapIndexs;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapIndexs;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs>& val)
    {
        deviceCapIndexs = val;
    }
    UUIDString& GetDomainId()
    {
        return domainId;
    }
    const UUIDString& GetDomainId() const
    {
        return domainId;
    }
    void SetDomainId(const UUIDString& val)
    {
        domainId = val;
    }

	void SetGrpDevTreeId(const UUIDString& strGrpDevTreeId)
	{
		grpDevTreeId = strGrpDevTreeId;
	}
	UUIDString& GetGrpDevTreeId()
	{
		return grpDevTreeId;
	}
	const UUIDString& GetGrpDevTreeId() const
	{
		return grpDevTreeId;
	}

	void SetDevDevTreeId(const UUIDString& strDevDevTreeId)
	{
		devDevTreeId = strDevDevTreeId;
	}
	UUIDString& GetDevDevTreeId()
	{
		return devDevTreeId;
	}
	const UUIDString& GetDevDevTreeId() const
	{
		return devDevTreeId;
	}
private:
    UUIDString groupId;
    UUIDString deviceId;
	UUIDString devGrpId;               
    vector<TDeviceCapIndexs> deviceCapIndexs;

    UUIDString domainId; // 组或设备所属的域Id(业务上设备只能被划归到同域的设备组中)，发给客户端时TAS拼装出来
	UUIDString grpDevTreeId;                 // 设备组所属设备树ID
	UUIDString devDevTreeId;                 // 设备所属设备树ID
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};
class CDeviceGroupDeviceInfoKey
{
public:
    CDeviceGroupDeviceInfoKey(){}

    UUIDString& GetDeviceGroupID() 
    {
        return groupID;
    }
    const UUIDString& GetDeviceGroupID() const
    {
        return groupID;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        groupID = val;
    }
    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    vector<TDeviceCapIndexs>& GetDeviceCapIndexs()
    {
        return deviceCapIndexs;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapIndexs;
    }
	void SetDeviceCapIndexs(const vector<TDeviceCapIndexs>& val)
	{
		deviceCapIndexs = val;
    }

	void SetDevtreeId(const UUIDString& strDevtreeId)
	{
		devtreeId = strDevtreeId;
	}
	UUIDString& GetDevtreeId()
	{
		return devtreeId;
	}
	const UUIDString& GetDevtreeId() const
	{
		return devtreeId;
	}
private:
    UUIDString groupID;
    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapIndexs;
	UUIDString devtreeId;  // 设备组所属设备树ID [3/10/2016 pangubing]
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceGroupDeviceInfoQuery4GbCondition
{
public:
	CDeviceGroupDeviceInfoQuery4GbCondition()
	{
	}
private:
    CQryOptionField optionField; // 查询字段标识
	UUIDString devicegroupId;                               
	UUIDString devTreeId;                 
public:
    void SetDevicegroupId(const UUIDString& strDevicegroupId)
    {
        devicegroupId = strDevicegroupId;
		optionField.SetOption("devicegroupId");
    }
	bool GetDevicegroupId(UUIDString& val) const 
	{
		if (!optionField.HasOption("devicegroupId"))
			return false;
		val = devicegroupId;
		return true;
	}
    void SetDevTreeId(const UUIDString& strDevTreeId)
    {
        devTreeId = strDevTreeId;
		optionField.SetOption("devTreeId");
    }
	bool GetDevTreeId(UUIDString& val) const
	{
		if (!optionField.HasOption("devTreeId"))
			return false;
		val = devTreeId;
		return true;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDeviceGroupDeviceInfoQueryCondition
{
public:
    CDeviceGroupDeviceInfoQueryCondition()
    {
        deviceCapID = DEVICE_CAP_INVALID;
        deviceCapIndex = -1;
    }
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetDeviceGroupID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceGroupID"))
            return false;
        val = deviceGroupID;
        return true;
    }
    void SetDeviceGroupID(const UUIDString &val)
    {
        deviceGroupID = val;
        optionField.SetOption("deviceGroupID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDeviceCapID(int &val) const
    {
        if (!optionField.HasOption("deviceCapID"))
            return false;
        val = deviceCapID;
        return true;
    }
    void SetDeviceCapID(int val)
    {
        deviceCapID = val;
        optionField.SetOption("deviceCapID");
    }
    bool GetDeviceCapIndex(int &val) const
    {
        if (!optionField.HasOption("deviceCapIndex"))
            return false;
        val = deviceCapIndex;
        return true;
    }
    void SetDeviceCapIndex(int val)
    {
        deviceCapIndex = val;
        optionField.SetOption("deviceCapIndex");
    }
private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString cuserID;          // 发起查询的CU用户编号，指定时能够限制返回的数据为该用户权限范围内的

    UUIDString deviceGroupID;
    UUIDString deviceID;
    int deviceCapID;  
    int deviceCapIndex;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupInfo
{
public:
    CUserGroupInfo()
    {
        usertype = CUSER_TYPE_SUPERADMIN;
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 1;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
		waterMarkColor = 16711680;
		waterMarkFont = "";
    }

	void Clear()
	{
		id.clear();
		name.clear();
		description.clear();
		createDate.clear();
		usertype = CUSER_TYPE_SUPERADMIN;
		userPrivilege.Clear();
		createUser.clear();
		deviceTreeVer.clear();
		switchNum = -1;
		waterMarkEnabled = false;
		waterMarkFontSize = 72;
		waterMarkRotationAngle = 30;
		waterMarkTransparent = 30;
		waterMarkTiled = true;
	}

    UUIDString& GetGroupNO() 
    {
        return id;
    }
    const UUIDString& GetGroupNO() const
    {
        return id;
    }
    void SetGroupNO(const UUIDString &val)
    {
        id = val;
    }
    string& GetGroupName() 
    {
        return name;
    }
    const string& GetGroupName() const
    {
        return name;
    }
    void SetGroupName(const string &val)
    {
        name = val;
    }
    string& GetDescription()
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
	int GetUserType() 
	{
		return usertype;
	}
    int GetUserType() const
    {
        return usertype;
    }
    void SetUserType(int val)
    {
        usertype = val;
    }
    CUserPrivilege& GetUserPrivilege() 
    {
        return userPrivilege;
    }
    const CUserPrivilege& GetUserPrivilege() const
    {
        return userPrivilege;
    }
    void SetUserPrivilege(const CUserPrivilege &val)
    {
        userPrivilege = val;
    }
    UUIDString& GetCreateUser()
    {
        return createUser;
    }
    const UUIDString& GetCreateUser() const
    {
        return createUser;
    }
    void SetCreateUser(const UUIDString &val)
    {
        createUser = val;
    }
    void SetDeviceTreeVer(const string& strDeviceTreeVer)
    {
        deviceTreeVer = strDeviceTreeVer;
    }
    string& GetDeviceTreeVer()
    {
        return deviceTreeVer;
    }
    const string& GetDeviceTreeVer() const
    {
        return deviceTreeVer;
    }
	void SetSwitchNum(int nSwitchNum)
	{
		switchNum = nSwitchNum;
	}
	int GetSwitchNum() const
	{
		return switchNum;
	}
	void SetWaterMarkEnabled(bool bWaterMarkEnabled)
	{
		waterMarkEnabled = bWaterMarkEnabled;
	}
	bool GetWaterMarkEnabled() const
	{
		return waterMarkEnabled;
	}

	void SetWaterMarkFont(const string& strWaterMarkFont)
	{
		waterMarkFont = strWaterMarkFont;
	}
	string& GetWaterMarkFont()
	{
		return waterMarkFont;
	}
	const string& GetWaterMarkFont() const
	{
		return waterMarkFont;
	}

	void SetWaterMarkFontSize(int nWaterMarkFontSize)
	{
		waterMarkFontSize = nWaterMarkFontSize;
	}
	int GetWaterMarkFontSize() const
	{
		return waterMarkFontSize;
	}

	void SetWaterMarkRotationAngle(int nWaterMarkRotationAngle)
	{
		waterMarkRotationAngle = nWaterMarkRotationAngle;
	}
	int GetWaterMarkRotationAngle() const
	{
		return waterMarkRotationAngle;
	}

	void SetWaterMarkColor(int nWaterMarkColor)
	{
		waterMarkColor = nWaterMarkColor;
	}
	int GetWaterMarkColor() const
	{
		return waterMarkColor;
	}

	void SetWaterMarkTransparent(int nWaterMarkTransparent)
	{
		waterMarkTransparent = nWaterMarkTransparent;
	}
	int GetWaterMarkTransparent() const
	{
		return waterMarkTransparent;
	}

	void SetWaterMarkTiled(bool bWaterMarkTiled)
	{
		waterMarkTiled = bWaterMarkTiled;
	}
	bool GetWaterMarkTiled() const
	{
		return waterMarkTiled;
	}
private:
    UUIDString id;
    string name;
    string description;  // 描述 
    string createDate;   // 创建日期
    int usertype;        // CU用户类型：参见常量定义
    CUserPrivilege userPrivilege; // 用户权限
    UUIDString createUser;        // 创建者ID
    string deviceTreeVer;         // 设备树版本号
	int switchNum;               //码流并发数
	//视频水印功能
	bool waterMarkEnabled;                                   //是否启用水印
	string waterMarkFont;                                    //水印字体
	int waterMarkFontSize;                                   //字体大小
	int waterMarkRotationAngle;                              //旋转角度
	int waterMarkColor;                                      //颜色
	int waterMarkTransparent;                                //透明度
	bool waterMarkTiled;                                     //是否平铺
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupInfoKey
{
public:
    CUserGroupInfoKey(){}

    UUIDString& GetGroupNO() 
    {
        return id;
    }
    const UUIDString& GetGroupNO() const
    {
        return id;
    }
    void SetGroupNO(const UUIDString &val)
    {
        id = val;
    }
private:
    UUIDString id;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupInfoQueryCondition
{
public:
    CUserGroupInfoQueryCondition(){}
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetGroupNO(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetGroupNO(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetGroupName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetGroupName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString cuserID;          // 发起查询的CU用户编号，指定时能够限制返回的数据为该用户权限范围内的
                                 // 用户组没有树结构，指定此ID时返回用户创建的组

    UUIDString id;
    string name;
    string description; //描述
    string createDate_From; //创建日期起始日期
    string createDate_To; //创建日期起始日期
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupDeviceInfo
{
public:
    CUserGroupDeviceInfo(){}

	void Clear()
	{
		groupID.clear();
		deviceID.clear();
		devDomainID.clear();
		devicePrivilege.Clear();
		capPrivileges.clear();
	}

    UUIDString& GetGroupID() 
    {
        return groupID;
    }
    const UUIDString& GetGroupID() const
    {
        return groupID;
    }
    void SetGroupID(const UUIDString &val)
    {
        groupID = val;
    }
    UUIDString& GetDeviceID()
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    UUIDString& GetDevDomainID()
    {
        return devDomainID;
    }
    const UUIDString& GetDevDomainID() const
    {
        return devDomainID;
    }
    void SetDevDomainID(const UUIDString& val)
    {
        devDomainID = val;
    }
    CDevicePrivilege& GetDevicePrivilege() 
    {
        return devicePrivilege;
    }
    const CDevicePrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }
    void SetDevicePrivilege(const CDevicePrivilege &val)
    {
        devicePrivilege = val;
    }
    vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege()
    {
        return capPrivileges;
    }
    const vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege() const
    {
        return capPrivileges;
    }
	void SetDeviceCapPrivilege(const vector<TDeviceCapPrivilege>& val)
	{
		capPrivileges = val;
	}
private:
    UUIDString groupID;
    UUIDString deviceID;
    UUIDString devDomainID; // 设备所属域ID, 发给客户端时TAS拼装出来
    CDevicePrivilege devicePrivilege;
    vector<TDeviceCapPrivilege> capPrivileges;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupDeviceInfoKey
{
public:
    CUserGroupDeviceInfoKey(){}

    UUIDString& GetGroupID() 
    {
        return groupID;
    }
    const UUIDString& GetGroupID() const
    {
        return groupID;
    }
    void SetGroupID(const UUIDString &val)
    {
        groupID = val;
    }
    UUIDString& GetDeviceID()
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    vector<TDeviceCapIndexs>& GetDeviceCapIndexs() 
    {
        return deviceCapset;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapset;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs> &val)
    {
        deviceCapset = val;
    }
private:
    UUIDString groupID;
    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapset;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CUserGroupDeviceInfoQueryCondition
{
public:
    CUserGroupDeviceInfoQueryCondition()
    {
        capID = DEVICE_CAP_INVALID;
        capIndex = -1;
    }
    bool GetCuserGrpID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserGrpID"))
            return false;
        val = cuserGrpID;
        return true;
    }
    void SetCuserGrpID(const UUIDString &val)
    {
        cuserGrpID = val;
        optionField.SetOption("cuserGrpID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDeviceCapID(int &val) const
    {
        if (!optionField.HasOption("capID"))
            return false;
        val = capID;
        return true;
    }
    void SetDeviceCapID(int val)
    {
        capID = val;
        optionField.SetOption("capID");
    }
    bool GetDeviceCapIndex(int &val) const
    {
        if (!optionField.HasOption("capIndex"))
            return false;
        val = capIndex;
        return true;
    }
    void SetDeviceCapIndex(int val)
    {
        capIndex = val;
        optionField.SetOption("capIndex");
    }

    bool GetDevGrpID(UUIDString &val) const
    {
        if (!optionField.HasOption("devGrpID"))
            return false;
        val = devGrpID;
        return true;
    }
    void SetDevGrpID(const UUIDString &val)
    {
        devGrpID = val;
        optionField.SetOption("devGrpID");
    }

	bool GetDevTreeId(UUIDString &val) const
	{
		if (!optionField.HasOption("devTreeId"))
			return false;
		val = devTreeID;
		return true;
	}
	void SetDevTreeId(const UUIDString &val)
	{
		devTreeID = val;
		optionField.SetOption("devTreeId");
	}
private:
    CQryOptionField optionField; // 查询字段标识

    UUIDString cuserGrpID;
    UUIDString deviceID;
    int capID;    // 设备能力ID
    int capIndex; // 设备能力索引

    UUIDString devGrpID; // 设备所属设备组限制
	UUIDString devTreeID; // 设备所属设备树ID

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceFullGbInfo
{
	UUIDString devId;               
	string     devName;
	string     devDomainId;               
	string     vidSrcName;               
	int        vidIdx;                     //视频源Id
	string     oldPriGbid;               
	string     newPriGbid;               
	string     oldSecGbid;               
	string     newSecGbid;               

	vector<CDeviceGroupInfo> devGrpInfos;                //设备-->根分组路径上每层分组的详细信息
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOldPriGbid(const string& strOldPriGbid)
    {
        oldPriGbid = strOldPriGbid;
    }
    string& GetOldPriGbid()
    {
        return oldPriGbid;
    }
    const string& GetOldPriGbid() const
    {
        return oldPriGbid;
    }

    void SetNewPriGbid(const string& strNewPriGbid)
    {
        newPriGbid = strNewPriGbid;
    }
    string& GetNewPriGbid()
    {
        return newPriGbid;
    }
    const string& GetNewPriGbid() const
    {
        return newPriGbid;
    }

    void SetOldSecGbid(const string& strOldSecGbid)
    {
        oldSecGbid = strOldSecGbid;
    }
    string& GetOldSecGbid()
    {
        return oldSecGbid;
    }
    const string& GetOldSecGbid() const
    {
        return oldSecGbid;
    }

    void SetNewSecGbid(const string& strNewSecGbid)
    {
        newSecGbid = strNewSecGbid;
    }
    string& GetNewSecGbid()
    {
        return newSecGbid;
    }
    const string& GetNewSecGbid() const
    {
        return newSecGbid;
    }

public:
    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetDevName(const string& strDevName)
    {
        devName = strDevName;
    }
    string& GetDevName()
    {
        return devName;
    }
    const string& GetDevName() const
    {
        return devName;
    }

    void SetVidSrcName(const string& strVidSrcName)
    {
        vidSrcName = strVidSrcName;
    }
    string& GetVidSrcName()
    {
        return vidSrcName;
    }
    const string& GetVidSrcName() const
    {
        return vidSrcName;
    }

    void SetVidIdx(int nVidIdx)
    {
        vidIdx = nVidIdx;
    }
    int GetVidIdx() const
    {
        return vidIdx;
    }

public:
    void SetDevGrpInfos(const vector<CDeviceGroupInfo>& cDevGrpInfos)
    {
        devGrpInfos = cDevGrpInfos;
    }
    vector<CDeviceGroupInfo>& GetDevGrpInfos()
    {
        return devGrpInfos;
    }
    const vector<CDeviceGroupInfo>& GetDevGrpInfos() const
    {
        return devGrpInfos;
    }

public:
    void SetDevDomainId(const string& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    string& GetDevDomainId()
    {
        return devDomainId;
    }
    const string& GetDevDomainId() const
    {
        return devDomainId;
    }

};

typedef vector<CDeviceGroupInfo> TDevGrpFullGbInfos ;

class CDomainRelationQueryCondition
{
public:
    CDomainRelationQueryCondition(){}

    bool GetDomainID(UUIDString &val) const
    {
        if (!optionField.HasOption("domainID"))
            return false;
        val = domainID;
        return true;
    }
    void SetDomainID(const UUIDString &val)
    {
        domainID = val;
        optionField.SetOption("domainID");
    }
    bool GetParentDomainID(UUIDString &val) const
    {
        if (!optionField.HasOption("parentDomainID"))
            return false;
        val = parentDomainID;
        return true;
    }
    void SetParentDomainID(const UUIDString &val)
    {
        parentDomainID = val;
        optionField.SetOption("parentDomainID");
    }
    bool GetDomainName(UUIDString &val) const
    {
        if (!optionField.HasOption("domainName"))
            return false;
        val = domainName;
        return true;
    }
    void SetDomainName(const UUIDString &val)
    {
        domainName = val;
        optionField.SetOption("domainName");
    }
private:
    CQryOptionField optionField; // 查询字段标识

    UUIDString domainID;       // 域编号
    UUIDString parentDomainID; // 父域编号
    string domainName;         // 域名称
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CTacUserInfo
{
public:
    CTacUserInfo()
    {
        type = enTacUserType_SuperAdmin;
        mLoginFlag = true;
        enabledFlag = true;
    }

    UUIDString& GetUserID() 
    {
        return id;
    }
    const UUIDString& GetUserID() const
    {
        return id;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
    }
    string& GetUserName()
    {
        return name;
    }
    const string& GetUserName() const
    {
        return name;
    }
    void SetUserName(const string &val)
    {
        name = val;
    }
    string& GetUserPwd() 
    {
        return password;
    }
    const string& GetUserPwd() const
    {
        return password;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
    }
	TTacUserType GetUserType() 
    {
		return type;
    }
	TTacUserType GetUserType() const
    {
        return type;
    }
    void SetUserType(TTacUserType val)
    {
        type = val;
    }
    bool GetMLoginFlag() 
    {
        return mLoginFlag;
    }
	bool GetMLoginFlag() const
    {
        return mLoginFlag;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
    }
    bool GetEnabledFlag() 
    {
        return enabledFlag;
    }
    bool GetEnabledFlag() const
    {
        return enabledFlag;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
    }
    string& GetDescription() 
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    string& GetNameExpireDate()
    {
        return nameExpireDate;
    }
    const string& GetNameExpireDate() const
    {
        return nameExpireDate;
    }
    void SetNameExpireDate(const string &val)
    {
        nameExpireDate = val;
    }
    string& GetPwdExpireDate() 
    {
        return pwdExpireDate;
    }
    const string& GetPwdExpireDate() const
    {
        return pwdExpireDate;
    }
    void SetPwdExpireDate(const string &val)
    {
        pwdExpireDate = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    UUIDString& GetCreatorID() 
    {
        return creatorID;
    }
    const UUIDString& GetCreatorID() const
    {
        return creatorID;
    }
    void SetCreatorID(const UUIDString &val)
    {
        creatorID = val;
    }
    CTacUserPrivilege& GetTasUserPrivilege()
    {
        return tasUserPrivilege;
    }
    const CTacUserPrivilege& GetTasUserPrivilege() const
    {
        return tasUserPrivilege;
    }
    void SetTasUserPrivilege(const CTacUserPrivilege &val)
    {
        tasUserPrivilege = val;
    }
private:
    UUIDString id; // 用户编号
    string name; // 用户名
    string password; // 登陆密码, 密文形式
    TTacUserType type; // 用户类型
    bool mLoginFlag; // 多次登陆标志
    bool enabledFlag; // 启停状态标志
    string description; // 描述
    string nameExpireDate; // 帐户使用截止日期
    string pwdExpireDate; // 密码使用截止日期
    string createDate; // 创建日期
    UUIDString creatorID; // 创建者编号
    CTacUserPrivilege tasUserPrivilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CTacUserInfoKey
{
public:
    CTacUserInfoKey(){}

    UUIDString& GetUserID() 
    {
        return id;
    }
    const UUIDString& GetUserID() const
    {
        return id;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
    }
private:
    UUIDString id; // 用户编号
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CTacUserInfoQueryCondition
{
public:
    CTacUserInfoQueryCondition()
    {
        type = enTacUserType_SuperAdmin;
        mLoginFlag = true;
        enabledFlag = true;
    }
    bool GetUserID(UUIDString &val) const
    {
        if (!optionField.HasOption("id"))
            return false;
        val = id;
        return true;
    }
    void SetUserID(const UUIDString &val)
    {
        id = val;
        optionField.SetOption("id");
    }
    bool GetUserName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetUserName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetUserType(TTacUserType &val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetUserType(TTacUserType val)
    {
        type = val;
        optionField.SetOption("type");
    }
    bool GetMLoginFlag(bool &val) const
    {
        if (!optionField.HasOption("mLoginFlag"))
            return false;
        val = mLoginFlag;
        return true;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
        optionField.SetOption("mLoginFlag");
    }
    bool GetEnabledFlag(bool &val) const
    {
        if (!optionField.HasOption("enabledFlag"))
            return false;
        val = enabledFlag;
        return true;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
        optionField.SetOption("enabledFlag");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetNameExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_From"))
            return false;
        val = nameExpireDate_From;
        return true;
    }
    void SetNameExpireDate_From(const string &val)
    {
        nameExpireDate_From = val;
        optionField.SetOption("nameExpireDate_From");
    }
    bool GetNameExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_To"))
            return false;
        val = nameExpireDate_To;
        return true;
    }
    void SetNameExpireDate_To(const string &val)
    {
        nameExpireDate_To = val;
        optionField.SetOption("nameExpireDate_To");
    }
    bool GetPwdExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_From"))
            return false;
        val = pwdExpireDate_From;
        return true;
    }
    void SetPwdExpireDate_From(const string &val)
    {
        pwdExpireDate_From = val;
        optionField.SetOption("pwdExpireDate_From");
    }
    bool GetPwdExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_To"))
            return false;
        val = pwdExpireDate_To;
        return true;
    }
    void SetPwdExpireDate_To(const string &val)
    {
        pwdExpireDate_To = val;
        optionField.SetOption("pwdExpireDate_To");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
    bool GetCreatorID(UUIDString &val) const
    {
        if (!optionField.HasOption("creatorID"))
            return false;
        val = creatorID;
        return true;
    }
    void SetCreatorID(const UUIDString &val)
    {
        creatorID = val;
        optionField.SetOption("creatorID");
    }
private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString id; // 用户编号
    string name; // 用户名
    TTacUserType type; // 用户类型：参见常量定义
    bool mLoginFlag;   // 多次登陆标志
    bool enabledFlag;  // 启停状态标志
    string description; // 描述
    string nameExpireDate_From; // 帐户使用截止日期起始时间
    string nameExpireDate_To; // 帐户使用截止日期结束时间
    string pwdExpireDate_From; // 密码使用截止日期起始时间
    string pwdExpireDate_To;   // 密码使用截止日期结束时间
    string createDate_From; // 创建日期起始时间
    string createDate_To;   // 创建日期结束时间
    UUIDString creatorID;   // 创建者编号
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CTempAuthorizationInfo
{
public:
    CTempAuthorizationInfo(){}

    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    CDevicePrivilege& GetDevicePrivilege() 
    {
        return devicePrivilege;
    }
    const CDevicePrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }
    void SetDevicePrivilege(const CDevicePrivilege &val)
    {
        devicePrivilege = val;
    }
    vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege()
    {
        return deviceCapPrivilege;
    }
    const vector<TDeviceCapPrivilege>& GetDeviceCapPrivilege() const
    {
        return deviceCapPrivilege;
    }
	void SetDeviceCapPrivilege(const vector<TDeviceCapPrivilege>& val)
	{
		deviceCapPrivilege = val;
	}
    string& GetDesc() 
    {
        return desc;
    }
    const string& GetDesc() const
    {
        return desc;
    }
    void SetDesc(const string &val)
    {
        desc = val;
    }
    string& GetDestinationDomain() 
    {
        return destinationDomain;
    }
    const string& GetDestinationDomain() const
    {
        return destinationDomain;
    }
    void SetDestinationDomain(const string &val)
    {
        destinationDomain = val;
    }
    string& GetUsername()
    {
        return username;
    }
    const string& GetUsername() const
    {
        return username;
    }
    void SetUsername(const string &val)
    {
        username = val;
    }
    string& GetCreateTime()
    {
        return createTime;
    }
    const string& GetCreateTime() const
    {
        return createTime;
    }
    void SetCreateTime(const string &val)
    {
        createTime = val;
    }

private:
    UUIDString deviceID;
    UUIDString devDomainId;    // 发给客户端时TAS拼装出来 hzytodo 未实现
    CDevicePrivilege devicePrivilege;
    vector<TDeviceCapPrivilege> deviceCapPrivilege;
    string desc;
    string destinationDomain;  // 被授权用户所在域名
    string username;           // 被授权用户的用户名
    string createTime;         // 由TAS生成
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevDomainId(const UUIDString& strDevDomainId)
    {
        devDomainId = strDevDomainId;
    }
    UUIDString& GetDevDomainId()
    {
        return devDomainId;
    }
    const UUIDString& GetDevDomainId() const
    {
        return devDomainId;
    }

};
class CTempAuthorizationInfoKey
{
public:
    CTempAuthorizationInfoKey(){}

    UUIDString& GetDeviceID() 
    {
        return deviceID;
    }
    const UUIDString& GetDeviceID() const
    {
        return deviceID;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
    }
    vector<TDeviceCapIndexs>& GetDeviceCapIndexs() 
    {
        return deviceCapIndexs;
    }
    const vector<TDeviceCapIndexs>& GetDeviceCapIndexs() const
    {
        return deviceCapIndexs;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs> &val)
    {
        deviceCapIndexs = val;
    }
    string& GetDestinationDomain()
    {
        return destinationDomain;
    }
    const string& GetDestinationDomain() const
    {
        return destinationDomain;
    }
    void SetDestinationDomain(const string &val)
    {
        destinationDomain = val;
    }
    string& GetUserName() 
    {
        return username;
    }
    const string& GetUserName() const
    {
        return username;
    }
    void SetUserName(const string &val)
    {
        username = val;
    }
private:
    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapIndexs;
    string destinationDomain;  // 被授权用户所在域名
    string username;           // 被授权用户的用户名
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CTempAuthorizationInfoQueryCondition
{
public:
    CTempAuthorizationInfoQueryCondition(){}
    bool GetCuserID(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const UUIDString &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetDeviceID(UUIDString &val) const
    {
        if (!optionField.HasOption("deviceID"))
            return false;
        val = deviceID;
        return true;
    }
    void SetDeviceID(const UUIDString &val)
    {
        deviceID = val;
        optionField.SetOption("deviceID");
    }
    bool GetDeviceCapIndexs(vector<TDeviceCapIndexs> &val) const
    {
        if (!optionField.HasOption("deviceCapIndexs"))
            return false;
        val = deviceCapIndexs;
        return true;
    }
    void SetDeviceCapIndexs(const vector<TDeviceCapIndexs> &val)
    {
        deviceCapIndexs = val;
        optionField.SetOption("deviceCapIndexs");
    }
    bool GetDesc(string &val) const
    {
        if (!optionField.HasOption("desc"))
            return false;
        val = desc;
        return true;
    }
    void SetDesc(const string &val)
    {
        desc = val;
        optionField.SetOption("desc");
    }
    bool GetDestinationDomain(string &val) const
    {
        if (!optionField.HasOption("destinationDomain"))
            return false;
        val = destinationDomain;
        return true;
    }
    void SetDestinationDomain(const string &val)
    {
        destinationDomain = val;
        optionField.SetOption("destinationDomain");
    }
    bool GetUsername(string &val) const
    {
        if (!optionField.HasOption("username"))
            return false;
        val = username;
        return true;
    }
    void SetUsername(const string &val)
    {
        username = val;
        optionField.SetOption("username");
    }
    bool GetCreateTime_From(string &val) const
    {
        if (!optionField.HasOption("createTime_From"))
            return false;
        val = createTime_From;
        return true;
    }
    void SetCreateTime_From(const string &val)
    {
        createTime_From = val;
        optionField.SetOption("createTime_From");
    }
    bool GetCreateTime_To(string &val) const
    {
        if (!optionField.HasOption("createTime_To"))
            return false;
        val = createTime_To;
        return true;
    }
    void SetCreateTime_To(const string &val)
    {
        createTime_To = val;
        optionField.SetOption("createTime_To");
    }
private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString cuserID;          // 发起查询的CU用户编号，指定时能够限制返回的数据为该用户权限范围内的

    UUIDString deviceID;
    vector<TDeviceCapIndexs> deviceCapIndexs;
    string desc;
    string destinationDomain;
    string username;
    string createTime_From;
    string createTime_To;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TDomainExtDataType;
#define DOMAIN_EXT_KDMNO        "kdmNO"     // 1.0域编号
#define DOMAIN_EXT_FIXADDR      "fixAddr"   // 域的安装位置

struct TDomainExtData
{
public:
    TDomainExtData()
    {
    }

    void Clear()
    {
        extData.clear();
    }

    bool IsEmpty() const
    {
        return extData.empty();
    }

private:
    map<TDomainExtDataType, string> extData;                   
public:
    void SetExtData(const map<TDomainExtDataType, string>& cExtData)
    {
        extData = cExtData;
    }
    map<TDomainExtDataType, string>& GetExtData()
    {
        return extData;
    }
    const map<TDomainExtDataType, string>& GetExtData() const
    {
        return extData;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDomainRelationInfo
{
public:
    CDomainRelationInfo(){}

	void Clear()
	{
		domainID.clear();
		parentDomainID.clear();
		domainName.clear();
		domainAlias.Clear();
	}

    UUIDString& GetDomainID() 
    {
        return domainID;
    }
    const UUIDString& GetDomainID() const
    {
        return domainID;
    }
    void SetDomainID(const UUIDString &val)
    {
        domainID = val;
    }
    string& GetDomainName() 
    {
        return domainName;
    }
    const string& GetDomainName() const
    {
        return domainName;
    }
    void SetDomainName(const string &val)
    {
        domainName = val;
    }
    UUIDString& GetParentDomainID() 
    {
        return parentDomainID;
    }
    const UUIDString& GetParentDomainID() const
    {
        return parentDomainID;
    }
    void SetParentDomainID(const UUIDString &val)
    {
        parentDomainID = val;
    }
    void SetDomainAlias(const TDomainExtData& cDomainAlias)
    {
        domainAlias = cDomainAlias;
    }
    TDomainExtData& GetDomainAlias()
    {
        return domainAlias;
    }
    const TDomainExtData& GetDomainAlias() const
    {
        return domainAlias;
    }
private:
    UUIDString domainID;            // 域编号
    UUIDString parentDomainID;      // 父域编号
    string domainName;              // 域名称
	TDomainExtData domainAlias;		// 域别名 mod by liangli 2014.3.21 将domainAlias字段扩展成一个map，数据库表不改动

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuserDevicefullinfoAndPrivilegeQueryCondition
{
public:
    CCuserDevicefullinfoAndPrivilegeQueryCondition(){}
    bool GetDevicegroupId(UUIDString &val) const
    {
        if (!optionField.HasOption("devicegroupId"))
            return false;
        val = devicegroupId;
        return true;
    }
    void SetDevicegroupId(const UUIDString &val)
    {
        devicegroupId = val;
        optionField.SetOption("devicegroupId");
    }
    bool GetDomainId(UUIDString &val) const
    {
        if (!optionField.HasOption("domainId"))
            return false;
        val = domainId;
        return true;
    }
    void SetDomainId(const UUIDString &val)
    {
        domainId = val;
        optionField.SetOption("domainId");
    }
	bool GetDevTreeId(UUIDString &val) const
	{
		if (!optionField.HasOption("devTreeId"))
			return false;
		val = devTreeId;
		return true;
	}
	void SetDevTreeId(const UUIDString &val)
	{
		devTreeId = val;
		optionField.SetOption("devTreeId");
	}
private:
    CQryOptionField optionField; // 查询字段标识
    UUIDString devicegroupId; 
    UUIDString domainId;
	UUIDString devTreeId;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
};
struct TUserVideoSourceCapInfo
{
    TUserVideoSourceCapInfo(){}
    string name;
	string extData;
    CVideosourcePrivilege privilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
};
struct TUserAlarmInputCapInfo
{
    TUserAlarmInputCapInfo(){}
    string name;
    CAlarmInputPrivilege privilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
};
struct TDevicefullinfoAndPrivilege
{
    TDevicefullinfoAndPrivilege(){}
    CDeviceInfo deviceInfo;
    CDevicePrivilege devicePrivilege;
	vector<int> vidSrcIndex;                //按先后顺序存放视频源索引值
    map<int, TUserVideoSourceCapInfo> videosourceinfos; // key为videosource索引
    map<int, TUserAlarmInputCapInfo> alarminputinfos;   // key为alarminput索引
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVidSrcIndex(const vector<int>& cVidSrcIndex)
    {
        vidSrcIndex = cVidSrcIndex;
    }
    vector<int>& GetVidSrcIndex()
    {
        return vidSrcIndex;
    }
    const vector<int>& GetVidSrcIndex() const
    {
        return vidSrcIndex;
    }

};

struct TSyncDataUserDeviceInfo
{
    string userName;
    UUIDString deviceID;
    CDevicePrivilege devicePrivilege;
    vector<TDeviceCapPrivilege> deviceCapPrivilege;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum ENSyncDataType
{
    enSyncDataDomainInfo = 1,         // 域信息, 数据内容为CDomainRelationInfo序列化内容
    enSyncDataDeviceModel = 2,        // 设备型号信息, 数据内容为CDeviceModelInfo序列化内容
    enSyncDataDeviceInfo = 3,         // 设备信息, 数据内容为CDeviceInfo序列化内容
    enSyncDataDeviceCapIndexName = 4, // 设备能力索引名称, 数据内容为TDeviceCapName序列化
    enSyncDataDeviceGroup = 5,        // 设备分组信息, 数据内容为CDeviceGroupInfo序列化内容
    enSyncDataDeviceGroup2Dev = 6,    // 设备分组划归, 数据内容为CDeviceGroupDeviceInfo序列化内容
    enSyncDataUserDevice = 7,          // 设备跨域授权, 数据内容为TSyncDataUserDeviceInfo序列化内容
	enSyncDataDevGrpDevSeq = 8,       // 设备组设备排序值， 数据内容为CDeviceGroupDevSeqInfo序列化内容
};
enum ENSyncOperateType
{
    enSyncOperateAdd = 1,  // 添加同步
    enSyncOperateDel = 2,  // 删除同步
    enSyncOperateMod = 3   // 修改同步
};

// 同步数据内容
struct TSyncDataInfo
{
    TSyncDataInfo():sn(0)
    {
    }
    ENSyncDataType dataType;       // 同步数据的类型
    int sn;                        // 同步数据的序列号
    ENSyncOperateType operateType; // 操作类型
    string dataDomainId;           // 同步数据所属域，也就是同步数据的直接所属域
    CTransparentData dataContent;  // 同步数据内容, 由同步数据类型决定；空时表示无需处理

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 同步数据结果
struct TSyncDataResult
{
    TSyncDataResult():sn(0), result(0)
    {
    }
    int sn;
    int result; // 可能的值：CMS_SUCCESS, ERR_TAS_INVALID_SYNCDATA

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 电视墙的电视与解码器绑定关系
struct TvDecoderBinder
{
    TvDecoderBinder():tvId(0), decoderOutputId(0)
    {
    }
	int tvId;             // 电视编号
	UUIDString decoderId; // 解码器编号
    int decoderOutputId;  // 解码器视频输出通道Id
    
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
// 电视墙信息
class CTvwallInfo: public CDeviceInfo
{
public:
    CTvwallInfo():CDeviceInfo(), tvNum(0),tvwallStyle(0)
    {
    }

	void Clear()
	{
		tvNum = 0;
		tvwallStyle = 0;
		tvwallCustomStyleData.clear();
		tvDecoderBinders.clear();
	}
private:
	int tvNum;            // 电视机数目             
	int tvwallStyle;      // 电视墙风格, 不使用枚举，因为TAS不关心具体的内容，否则到时风格类型添加时TAS也要改了             
	string tvwallCustomStyleData; // 电视墙自定义风格数据             

	vector<TvDecoderBinder> tvDecoderBinders; // 电视与解码器绑定关系             

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
public:
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvNum(int nTvNum)
    {
        tvNum = nTvNum;
    }
    int GetTvNum() const
    {
        return tvNum;
    }

    void SetTvwallStyle(int nTvwallStyle)
    {
        tvwallStyle = nTvwallStyle;
    }
    int GetTvwallStyle() const
    {
        return tvwallStyle;
    }

    void SetTvwallCustomStyleData(const string& strTvwallCustomStyleData)
    {
        tvwallCustomStyleData = strTvwallCustomStyleData;
    }
    string& GetTvwallCustomStyleData()
    {
        return tvwallCustomStyleData;
    }
    const string& GetTvwallCustomStyleData() const
    {
        return tvwallCustomStyleData;
    }

    void SetTvDecoderBinders(const vector<TvDecoderBinder>& cTvDecoderBinders)
    {
        tvDecoderBinders = cTvDecoderBinders;
    }
    vector<TvDecoderBinder>& GetTvDecoderBinders()
    {
        return tvDecoderBinders;
    }
    const vector<TvDecoderBinder>& GetTvDecoderBinders() const
    {
        return tvDecoderBinders;
    }

};

class CTvwallInfoQueryCondition
{
public:
	CTvwallInfoQueryCondition()
	{
	}
    bool GetCuserId(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserId"))
            return false;
        val = cuserId;
        return true;
    }
    void SetCuserId(const UUIDString &val)
    {
        cuserId = val;
        optionField.SetOption("cuserId");
    }
    bool GetCuserGrpId(UUIDString &val) const
    {
        if (!optionField.HasOption("cuserGrpId"))
            return false;
        val = cuserGrpId;
        return true;
    }
    void SetCuserGrpId(const UUIDString &val)
    {
        cuserGrpId = val;
        optionField.SetOption("cuserGrpId");
    }
    bool GetTvwallId(UUIDString &val) const
    {
        if (!optionField.HasOption("tvwallId"))
            return false;
        val = tvwallId;
        return true;
    }
    void SetTvwallId(const UUIDString &val)
    {
        tvwallId = val;
        optionField.SetOption("tvwallId");
    }

private:
	CQryOptionField optionField; // 查询字段标识

    UUIDString cuserId;    // 按照用户编号条件过滤，指定时能够限制返回的数据为该用户权限范围内的；此条件与下面的条件不能同时指定
    UUIDString cuserGrpId; // 按照用户组编号条件过滤，指定时能够限制返回的数据为该用户组权限范围内的；此条件与上面的条件不能同时指定
	UUIDString tvwallId;   // 电视墙编号

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum ENSyncType
{
    enSyncTypeParent     = 1, // 向上级同步，自动递归向上级同步数据
    enSyncTypeP2PLimit   = 2, // 点对点同步，权限受权；用于跨域授权
    enSyncTypeP2PNoLimit = 3, // 点对点同步，权限不限；可以用于全联通配置
};

struct TSyncDstDomainInfo
{
    UUIDString domainId;
    string domainName;
    ENSyncType syncType;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TTransProgress
{
	int startTime;		// 事务的开始时间
	int endTime;		// 事务的完成时间
	int totalNum;		// 需要处理的总数
	int finishNum;		// 处理完成的数量

public:
	TTransProgress()
	{
		startTime = 0;
		endTime   = 0;
		totalNum  = 0;
		finishNum = 0;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDeviceAutoAssignInfo
{
public:
	CDeviceAutoAssignInfo()
	{
		ownerType = 0;
	}

	void Clear()
	{
		devGrpId.clear();
		domainId.clear();
		ownerId.clear();
		ownerType = 0;
		operTime.clear();
	}

	void SetDevGrpId(const UUIDString &val)
	{
		devGrpId = val;
	}
	const UUIDString &GetDevGrpId() const
	{
		return devGrpId;
	}
	UUIDString &GetDevGrpId()
	{
		return devGrpId;
	}

	void SetDomainId(const UUIDString &val)
	{
		domainId = val;
	}
	const UUIDString &GetDomainId() const
	{
		return domainId;
	}
	UUIDString &GetDomainId()
	{
		return domainId;
	}

	void SetOwnerId(const UUIDString &val)
	{
		ownerId = val;
	}
	const UUIDString &GetOwnerId() const
	{
		return ownerId;
	}
	UUIDString &GetOwnerId()
	{
		return ownerId;
	}

	void SetOwnerType(int val)
	{
		ownerType = val;
	}
	int GetOwnerType() const
	{
		return ownerType;
	}
	int &GetOwnerType()
	{
		return ownerType;
	}

	void SetOperTime(const string &val)
	{
		operTime = val;
	}
	const string &GetOperTime() const
	{
		return operTime;
	}
	string &GetOperTime()
	{
		return operTime;
	}

private:
	UUIDString devGrpId;
	UUIDString domainId;
	UUIDString ownerId;
	int        ownerType;
	string     operTime;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDevTreeInfo
{
public:
	CDevTreeInfo()
	{
		Clear();
	}

	void Clear()
	{
		devtreeId.clear();
		devtreeName.clear();
		canBeUsed4Gb = false;
        type=SystemCustomTree;
	}

    enum TreeType
    {
        SystemCustomTree=0, // 系统自定义设备树
        UserFavorites=1,    // 用户收藏夹
        AllTree=255         // 所有的树
    };

private:
	UUIDString devtreeId;               
	string     devtreeName;  
	string     devtreeVer;
	bool       canBeUsed4Gb;
    TreeType   type;                
public:
    void SetDevtreeId(const UUIDString& strDevtreeId)
    {
        devtreeId = strDevtreeId;
    }
    UUIDString& GetDevtreeId()
    {
        return devtreeId;
    }
    const UUIDString& GetDevtreeId() const
    {
        return devtreeId;
    }

    void SetDevtreeName(const string& strDevtreeName)
    {
        devtreeName = strDevtreeName;
    }
    string& GetDevtreeName()
    {
        return devtreeName;
    }
    const string& GetDevtreeName() const
    {
        return devtreeName;
    }

	void SetDevtreeVer(const string& strDevtreeVer)
	{
		devtreeVer = strDevtreeVer;
	}
	string& GetDevtreeVer()
	{
		return devtreeVer;
	}
	const string& GetDevtreeVer() const
	{
		return devtreeVer;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCanBeUsed4Gb(bool bCanBeUsed4Gb)
    {
        canBeUsed4Gb = bCanBeUsed4Gb;
    }
    bool GetCanBeUsed4Gb() const
    {
        return canBeUsed4Gb;
    }

public:
    void SetType(TreeType eType)
    {
        type = eType;
    }
    TreeType GetType() const
    {
        return type;
    }

};

class CPtzLockInfo
{
public:
	CPtzLockInfo()
	{

	}

	void Clear()
	{
		deviceId.clear();
		videoSrcId=0;
		beginTime.clear();
		endTime.clear();	
		userId.clear();
		password.clear();
	}

private:
	string deviceId;               	// 设备id
	int videoSrcId;               		// 视频源id
	string beginTime;               	// 锁定开始时间.
	string endTime;               	    // 自动解锁时间.
	string userId;               		// 加锁用户
	string password;               	// 手动解锁密码  
public:
    void SetDeviceId(const string& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    string& GetDeviceId()
    {
        return deviceId;
    }
    const string& GetDeviceId() const
    {
        return deviceId;
    }

    void SetVideoSrcId(int nVideoSrcId)
    {
        videoSrcId = nVideoSrcId;
    }
    int GetVideoSrcId() const
    {
        return videoSrcId;
    }

    void SetBeginTime(const string& strBeginTime)
    {
        beginTime = strBeginTime;
    }
    string& GetBeginTime()
    {
        return beginTime;
    }
    const string& GetBeginTime() const
    {
        return beginTime;
    }

    void SetEndTime(const string& strEndTime)
    {
        endTime = strEndTime;
    }
    string& GetEndTime()
    {
        return endTime;
    }
    const string& GetEndTime() const
    {
        return endTime;
    }

    void SetUserId(const string& strUserId)
    {
        userId = strUserId;
    }
    string& GetUserId()
    {
        return userId;
    }
    const string& GetUserId() const
    {
        return userId;
    }

    void SetPassword(const string& strPassword)
    {
        password = strPassword;
    }
    string& GetPassword()
    {
        return password;
    }
    const string& GetPassword() const
    {
        return password;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CPtzLockQueryCondition
{
public:
	CPtzLockQueryCondition(){}

	bool GetDeviceID(UUIDString &val) const
	{
		if (!optionField.HasOption("deviceId"))
			return false;
		val = deviceId;
		return true;
	}
	void SetDeviceID(const UUIDString &val)
	{
		deviceId = val;
		optionField.SetOption("deviceId");
	}

	bool GetVideoSrcId(int &val) const
	{
		if (!optionField.HasOption("videoSrcId"))
			return false;
		val = videoSrcId;
		return true;
	}
	void SetVideoSrcId(const int &val)
	{
		videoSrcId = val;
		optionField.SetOption("videoSrcId");
	}
private:
	CQryOptionField optionField; // 查询字段标识

	string deviceId;       // 设备id
	int videoSrcId;        // 视频源id

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUserDevGrpVidSrcInfo
{
	UUIDString devId;                                   //设备ID
	UUIDString devDomainID;                             //设备所属域ID, 发给客户端时TAS拼装出来
	CDevicePrivilege devPrivilege;               
	TDeviceCapPrivilege devCapPri;                                                         //该设备隶属于指定用户指定设备组下的视频源index
	vector<int> vidSrcIndex;                               //按先后顺序存放视频源索引值

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetDevDomainID(const UUIDString& strDevDomainID)
    {
        devDomainID = strDevDomainID;
    }
    UUIDString& GetDevDomainID()
    {
        return devDomainID;
    }
    const UUIDString& GetDevDomainID() const
    {
        return devDomainID;
    }

    void SetDevPrivilege(const CDevicePrivilege& cDevPrivilege)
    {
        devPrivilege = cDevPrivilege;
    }
    CDevicePrivilege& GetDevPrivilege()
    {
        return devPrivilege;
    }
    const CDevicePrivilege& GetDevPrivilege() const
    {
        return devPrivilege;
    }

public:
    void SetDevCapPri(const TDeviceCapPrivilege& cDevCapPri)
    {
        devCapPri = cDevCapPri;
    }
    TDeviceCapPrivilege& GetDevCapPri()
    {
        return devCapPri;
    }
    const TDeviceCapPrivilege& GetDevCapPri() const
    {
        return devCapPri;
    }

public:
    void SetVidSrcIndex(const vector<int>& cVidSrcIndex)
    {
        vidSrcIndex = cVidSrcIndex;
    }
    vector<int>& GetVidSrcIndex()
    {
        return vidSrcIndex;
    }
    const vector<int>& GetVidSrcIndex() const
    {
        return vidSrcIndex;
    }

};

class CDevGrpVidSrcNum
{
private:
	UUIDString devGrpId;               
	int vidSrcNum;               
public:
    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetVidSrcNum(int nVidSrcNum)
    {
        vidSrcNum = nVidSrcNum;
    }
    int GetVidSrcNum() const
    {
        return vidSrcNum;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevVidsrc
{
private:
	UUIDString devId;                       //设备ID
	int vidSrcIdx;                          //视频源索引
public:
    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetVidSrcIdx(int nVidSrcIdx)
    {
        vidSrcIdx = nVidSrcIdx;
    }
    int GetVidSrcIdx() const
    {
        return vidSrcIdx;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CDevVidSrcInfo
{
private:
	UUIDString devId;                //设备ID
	bool exist;                      //应先判断此字段，为true时，其他字段才有效，false表示视频源不存在
	int vidsrcIdx;                   //视频源索引             
	UUIDString devGrpId;             //视频源所属分组ID
	string devGrpName;                              //视频源所述分组名
	UUIDString devGrpdomainId;       //设备组所属域ID
	string devName;                  //设备名
	string vidAlias;                 //视频源别名
	string ipAddress;                
	TDeviceCapExtData extData;       //视频源经纬度信息
public:
    void SetDevId(const UUIDString& strDevId)
    {
        devId = strDevId;
    }
    UUIDString& GetDevId()
    {
        return devId;
    }
    const UUIDString& GetDevId() const
    {
        return devId;
    }

    void SetExist(bool bExist)
    {
        exist = bExist;
    }
    bool GetExist() const
    {
        return exist;
    }

    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetVidAlias(const string& strVidAlias)
    {
        vidAlias = strVidAlias;
    }
    string& GetVidAlias()
    {
        return vidAlias;
    }
    const string& GetVidAlias() const
    {
        return vidAlias;
    }

    void SetIpAddress(const string& strIpAddress)
    {
        ipAddress = strIpAddress;
    }
    string& GetIpAddress()
    {
        return ipAddress;
    }
    const string& GetIpAddress() const
    {
        return ipAddress;
    }

    void SetExtData(const TDeviceCapExtData& cExtData)
    {
        extData = cExtData;
    }
    TDeviceCapExtData& GetExtData()
    {
        return extData;
    }
    const TDeviceCapExtData& GetExtData() const
    {
        return extData;
    }

public:
	void Clear()
	{
		devId.clear();
		exist = false;
		vidsrcIdx = -1;
		devGrpId.clear();
		devGrpdomainId.clear();
		devName.clear();
		vidAlias.clear();
		ipAddress.clear();
		extData.Clear();
	}
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVidsrcIdx(int nVidsrcIdx)
    {
        vidsrcIdx = nVidsrcIdx;
    }
    int GetVidsrcIdx() const
    {
        return vidsrcIdx;
	}
public:
    void SetDevGrpdomainId(const UUIDString& strDevGrpdomainId)
    {
        devGrpdomainId = strDevGrpdomainId;
    }
    UUIDString& GetDevGrpdomainId()
    {
        return devGrpdomainId;
    }
    const UUIDString& GetDevGrpdomainId() const
    {
        return devGrpdomainId;
    }

public:
    void SetDevName(const string& strDevName)
    {
        devName = strDevName;
    }
    string& GetDevName()
    {
        return devName;
    }
    const string& GetDevName() const
    {
        return devName;
    }

public:
    void SetDevGrpName(const string& strDevGrpName)
    {
        devGrpName = strDevGrpName;
    }
    string& GetDevGrpName()
    {
        return devGrpName;
    }
    const string& GetDevGrpName() const
    {
        return devGrpName;
    }

};

enum TDevGbidErrType
{
	GBID_ERRTYPE_NOERROR = 0,
	GBID_ERRTYPE_PRIREPEAT = 1, //主流gbid重复
	GBID_ERRTYPE_SECREPEAT = 2, //辅流gbid重复
};

struct TDevGbidErrItem
{
	TDevGbidErrType gbidErrType; //错误类型
	int  errIndex;//本次错误设备的序号

	//设备信息
	UUIDString domainId;
	UUIDString devId;
	string     devName;
	string     vidSrcName;
	int        vidSrcId;                   //视频源Id
	string     primaryOldGbid;
	string     primaryNewGbid;
	string     secondaryOldGbid;
	string     secondaryNewGbid;

	//分组信息
	UUIDString devGrpId;
	int        vidSrcGrpIdx;               //视频源在分组内的序号

	vector<CDeviceGroupInfo> devGrpFullGbinfos; //设备---->根分组路径上每个分组的详细信息
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum ENOrderType
{
	ENORDER_TYPE_INNER_DEVGRP_MOVE=1,
	ENORDER_TYPE_INNER_DEVGRP_TOP=2,
	ENORDER_TYPE_INNER_DEVGRP_BOTTOM=3,
	ENORDER_TYPE_INNER_DEVICE_MOVE=4,
	ENORDER_TYPE_INNER_DEVICE_TOP=5,
	ENORDER_TYPE_INNER_DEVICE_BOTTOM=6,
};

struct TVidSrcOrderInfo
{
	TVidSrcOrderInfo()
	{
		vidsrcIndexMoved = -2;
		vidsrcIndexRefer = -2;
	}
private:
	UUIDString devGrpId;               
	UUIDString devMovedId;                //被移动的设备Id
	int        vidsrcIndexMoved;                //被移动的设备视频源ID，-1标识该分组内该设备所有视频源均移动
	UUIDString devReferId;                //移动参照设备，标识将被移动设备，移动到此设备（视频源）之后
	int        vidsrcIndexRefer;          //移动参照设备视频源ID，-1表示移动到该设备最末视频源之后
	ENOrderType orderType;                         //排序操作类型
public:
    void SetDevGrpId(const UUIDString& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    UUIDString& GetDevGrpId()
    {
        return devGrpId;
    }
    const UUIDString& GetDevGrpId() const
    {
        return devGrpId;
    }

    void SetDevMovedId(const UUIDString& strDevMovedId)
    {
        devMovedId = strDevMovedId;
    }
    UUIDString& GetDevMovedId()
    {
        return devMovedId;
    }
    const UUIDString& GetDevMovedId() const
    {
        return devMovedId;
    }

    void SetVidsrcIndexMoved(int nVidsrcIndexMoved)
    {
        vidsrcIndexMoved = nVidsrcIndexMoved;
    }
    int GetVidsrcIndexMoved() const
    {
        return vidsrcIndexMoved;
    }

    void SetDevReferId(const UUIDString& strDevReferId)
    {
        devReferId = strDevReferId;
    }
    UUIDString& GetDevReferId()
    {
        return devReferId;
    }
    const UUIDString& GetDevReferId() const
    {
        return devReferId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOrderType(ENOrderType eOrderType)
    {
        orderType = eOrderType;
    }
    ENOrderType GetOrderType() const
    {
        return orderType;
    }

public:
    void SetVidsrcIndexRefer(int nVidsrcIndexRefer)
    {
        vidsrcIndexRefer = nVidsrcIndexRefer;
    }
    int GetVidsrcIndexRefer() const
    {
        return vidsrcIndexRefer;
    }

};

#endif // _TAS_STRUCT_H_























































