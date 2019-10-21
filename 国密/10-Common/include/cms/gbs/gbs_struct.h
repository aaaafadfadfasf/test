#ifndef _GBS_STRUCT_H_
#define _GBS_STRUCT_H_

#include "kdvtype.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include "cms/cms_const.h"
#include "cms/gb/gbspecial_struct.h"
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

typedef string TStreamType;
#define GBS_STREAM_TYPE_UDP "UDP"
#define GBS_STREAM_TYPE_TCP "TCP"

struct TGbsCivilCode
{
    string civilCode;  // 行政区划编号
    string civilName;  // 行政区划名称

    bool operator == (const TGbsCivilCode &rh) const
    {
        return civilCode == rh.civilCode && civilName == rh.civilName;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsConf
{
    TGbsConf()
    {
        useOneGbDomainId = true;
        gb_port = 0;
        g800_port = 0;
    }

    string  gbDomainId;      // 科达本域对应的中心信令控制服务器编码 20位
    vector<TGbsCivilCode> civilCode; // 行政区划列表
    string  gbParentId;      // gbDomainId 的父域编号
    // true时所有科达域使用同一个国标域编号，false时其它子科达域需要逐个配置
    bool useOneGbDomainId;   // 科达下级域全部使用一个国标域标志

    string realm;            // GBS分配密码的范围标识，默认为gbDomainId的前8位
    string uniformPassword;  // 统一的登录密码, 为空表示不校验

    int gb_port;     // gbs监听的国标端口 不支持修改
    string g800_ip;  // g800 ip
    int g800_port;   // g800 端口

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsAdvanceConf
{
    TGbsAdvanceConf()
    {
        
    }
    enum EnRecordSource
    {
        enRecordSourcePU, // 前端录像
        enRecordSourcePlatform, // 平台录像
		enRecordSourceAll, //全部录像
    };

    EnRecordSource recordSourceConf; // 录像来源配置，用于GBS对外提供录像资源

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsOptionConf
{
	TGbsOptionConf()
	{
		syncGbpuLongLati=false;
	}

	bool syncGbpuLongLati;  // 同步国标pu的经纬度数据至平台
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsKdDomainConf
{
    string kdDomainId;       // 科达域编号
    string gbDomainId;       // 对应的国标域编号 中心信令控制服务器编码 20位
    string gbParentDomainId; // 对应的国标域的父域编号 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsGbPuConf
{
    TGbsGbPuConf()
    {
        videoSourceNum = 0;
        encodeChannelNum = 0;
        licenseNum = 0;
        useFlag = true;
        localStorageCap = false;
		streamType = GBS_STREAM_TYPE_UDP;
		parallelAlarmInputNum = 0;
    }

    string devName;          // 设备名称
    string devModel;         // 设备型号
    string manufacturer;     // 设备厂商
    string userName;         // 为空时使用PU编号
    string userPassword;     // 为空时使用统一的登录密码
    string ip;               // 设备IP地址
    bool useFlag;            // 启用停用
	
    int  videoSourceNum;     // 视频源数目
    int  encodeChannelNum;   // 编码通道数目
    int  licenseNum;         // license收取数目
    bool localStorageCap;    // 本地存储能力
    // 设备能力标签 支持如下标签
    // DEVICE_CAP_LABEL_ENCODE_NVR DEVICE_CAP_LABEL_ENCODE_IPC_BULLET DEVICE_CAP_LABEL_ENCODE_IPC_DOME
    vector<int> capLabel;    

    string devAddr;          // 设备地址                
    string devDesc;          // 设备描述                

    string gbDevId;          // 入网gbs时分配的设备国标编号
    vector<string> gbChnIds; // 入网gbs时分配的通道国标编号列表
    string kdDevId;          // 入网tas时分配的科达设备号
    string kdmDevId;         // 入网tas时指定的kdmno                 
    TStreamType streamType;                //码流类型：UDP--默认，TCP
    string gbNatId;                            //国标前端NAT信息
    int parallelAlarmInputNum;			   //并口告警数
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevAddr(const string& strDevAddr)
    {
        devAddr = strDevAddr;
    }
    string& GetDevAddr()
    {
        return devAddr;
    }
    const string& GetDevAddr() const
    {
        return devAddr;
    }

    void SetDevDesc(const string& strDevDesc)
    {
        devDesc = strDevDesc;
    }
    string& GetDevDesc()
    {
        return devDesc;
    }
    const string& GetDevDesc() const
    {
        return devDesc;
    }

public:
    void SetKdmDevId(const string& strKdmDevId)
    {
        kdmDevId = strKdmDevId;
    }
    string& GetKdmDevId()
    {
        return kdmDevId;
    }
    const string& GetKdmDevId() const
    {
        return kdmDevId;
    }

public:
    void SetStreamType(const TStreamType& cStreamType)
    {
        streamType = cStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }
};
struct TGbsGbDecodeConf
{
	string devName;                                        // 设备名称
	string devModelName;                                       // 设备型号
	string manufacturer;                                   // 设备厂商
	int    decodeChnNum;                                                  // 解码通道数
	int    decodeOutputChnNum;                                            // 解码输出通道数
	string userName;                                      // 为空时使用PU编号
	string userPassword;                                  // 为空时使用统一的登录密码
	string ip;                                             // 设备IP地址
	bool useFlag;                                          // 启用停用
	string devAddr;                                        // 设备地址                
	string devDesc;                                        // 设备描述                
	string kdDevId;                                                       // 入网是分配的设备科达编号
	string gbDevId;                                        // 入网gbs时分配的设备国标编号
	vector<string> gbChnIds;                               // 入网gbs时分配的通道国标编号列表
	string gbNatId;                                                          //国标前端NAT信息
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
    void SetDevModelName(const string& strDevModelName)
    {
        devModelName = strDevModelName;
    }
    string& GetDevModelName()
    {
        return devModelName;
    }
    const string& GetDevModelName() const
    {
        return devModelName;
    }
    void SetManufacturer(const string& strManufacturer)
    {
        manufacturer = strManufacturer;
    }
    string& GetManufacturer()
    {
        return manufacturer;
    }
    const string& GetManufacturer() const
    {
        return manufacturer;
    }
    void SetDecodeChnNum(int nDecodeChnNum)
    {
        decodeChnNum = nDecodeChnNum;
    }
    int GetDecodeChnNum() const
    {
        return decodeChnNum;
    }
    void SetDecodeOutputChnNum(int nDecodeOutputChnNum)
    {
        decodeOutputChnNum = nDecodeOutputChnNum;
    }
    int GetDecodeOutputChnNum() const
    {
        return decodeOutputChnNum;
    }
    void SetUserName(const string& strUserName)
    {
        userName = strUserName;
    }
    string& GetUserName()
    {
        return userName;
    }
    const string& GetUserName() const
    {
        return userName;
    }
    void SetUserPassword(const string& strUserPassword)
    {
        userPassword = strUserPassword;
    }
    string& GetUserPassword()
    {
        return userPassword;
    }
    const string& GetUserPassword() const
    {
        return userPassword;
    }
    void SetIp(const string& strIp)
    {
        ip = strIp;
    }
    string& GetIp()
    {
        return ip;
    }
    const string& GetIp() const
    {
        return ip;
    }
    void SetUseFlag(bool bUseFlag)
    {
        useFlag = bUseFlag;
    }
    bool GetUseFlag() const
    {
        return useFlag;
    }
    void SetDevAddr(const string& strDevAddr)
    {
        devAddr = strDevAddr;
    }
    string& GetDevAddr()
    {
        return devAddr;
    }
    const string& GetDevAddr() const
    {
        return devAddr;
    }
    void SetDevDesc(const string& strDevDesc)
    {
        devDesc = strDevDesc;
    }
    string& GetDevDesc()
    {
        return devDesc;
    }
    const string& GetDevDesc() const
    {
        return devDesc;
    }
    void SetKdDevId(const string& strKdDevId)
    {
        kdDevId = strKdDevId;
    }
    string& GetKdDevId()
    {
        return kdDevId;
    }
    const string& GetKdDevId() const
    {
        return kdDevId;
    }
    void SetGbDevId(const string& strGbDevId)
    {
        gbDevId = strGbDevId;
    }
    string& GetGbDevId()
    {
        return gbDevId;
    }
    const string& GetGbDevId() const
    {
        return gbDevId;
    }
    void SetGbChnIds(const vector<string>& cGbChnIds)
    {
        gbChnIds = cGbChnIds;
    }
    vector<string>& GetGbChnIds()
    {
        return gbChnIds;
    }
    const vector<string>& GetGbChnIds() const
    {
        return gbChnIds;
    }
    void SetGbNatId(const string& strGbNatId)
    {
        gbNatId = strGbNatId;
    }
    string& GetGbNatId()
    {
        return gbNatId;
    }
    const string& GetGbNatId() const
    {
        return gbNatId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef int TGbsGbPeripheralType;
#define GB_GBPERIPHERAL_TYPE_OTHER          0
#define GB_GBPERIPHERAL_TYPE_INNER_MSS      1

struct TGbsGbPeripheralConf
{
	TGbsGbPeripheralConf()
	{
		onlineStatus = "offline";
		type = GB_GBPERIPHERAL_TYPE_OTHER;
	}

	string name;       // 外设名称
	string gbDevId;    // 外设国标编号
	string userName;   // 一般为相应的外设编号
	string password;   // 为空时使用统一的登录密码
	string ip;         // 设备IP
	string onlineStatus; // 在线状态：online, offline
	TGbsGbPeripheralType type;	// 外设类型，目前主要有：内置mss和其他


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//国标目录推送方式
enum EnumCatalogType
{
	GBS_CATALOG_TYPE_CIVILCODE,   //civilcode方式组网
	GBS_CATALOG_TYPE_BUSINNESS,   //216方式组网
};

struct TGbsGbDomainChildConf
{
    TGbsGbDomainChildConf()
    {
        onlineStatus = "offline";
		catalogtype = GBS_CATALOG_TYPE_CIVILCODE;
		streamType = GBS_STREAM_TYPE_UDP;
		qryCatalogOnReg = "TRUE";
        interConnect = false;
		reportCatalog2OtherUas = false;
    }
	
    string name;       // 下级域名称
    string gbDevId;    // 国标编号
    string userName;   // 一般为相应的下级编号
    string password;   // 为空时使用统一的登录密码
    string ip;         // 设备IP
    string onlineStatus; // 在线状态：online, offline
	int   catalogtype;   //目录推送方式
    string gbNatId;     //与对方所处的NAT关系
	TStreamType streamType;                //码流类型，可选择UDP、TCP
	string qryCatalogOnReg;                //注册成功后是否自动查询目录，"TRUE"--自动查询，"FALSE"--不自动查询
    bool interConnect;                               //true-组建互联平台
	bool reportCatalog2OtherUas;                     //true-可将该uas目录上报到其他uas。
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetStreamType(const TStreamType& strStreamType)
    {
        streamType = strStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }

public:
    void SetQryCatalogOnReg(const string& strQryCatalogOnReg)
    {
        qryCatalogOnReg = strQryCatalogOnReg;
    }
    string& GetQryCatalogOnReg()
    {
        return qryCatalogOnReg;
    }
    const string& GetQryCatalogOnReg() const
    {
        return qryCatalogOnReg;
    }
    
    void SetInterConnect(bool bInterConnect)
    {
        interConnect = bInterConnect;
    }
    bool GetInterConnect() const
    {
        return interConnect;
    }

    void SetReportCatalog2OtherUas(bool bReportCatalog2OtherUas)
    {
        reportCatalog2OtherUas = bReportCatalog2OtherUas;
    }
    bool GetReportCatalog2OtherUas() const
    {
        return reportCatalog2OtherUas;
    }

};

struct TGbsGbDomainParentConf
{
    TGbsGbDomainParentConf()
    {
        serverPort = 0;
        loginStatus = "unlogin";
		catalogtype = GBS_CATALOG_TYPE_CIVILCODE;
		streamType = GBS_STREAM_TYPE_UDP;
		fixGbsIp = "0.0.0.0";
		curGbsIp = "0.0.0.0";
		interConnect = false;
		reportCatalog2OtherUas  = false;
    }

    string name;      // 上级域名称
    string gbDevId;   // 国标编号
    string userName;  // 一般为本域编号
    string password;  // 登录密码
    string serverIp;  // 平台IP
    int    serverPort;// 平台端口
    string loginStatus; // 登录状态：logined, unlogin
	int    catalogtype; //国标目录推送方式
    string gbNatId;     //与对方所处的NAT关系
	TStreamType streamType;                 //码流类型，对上级而言，仅限语音呼叫有效
	string fixGbsIp;                   //用户指定的GBSIP，"0.0.0.0"表示支持动态均衡，否则表示由指定GBS完成上联任务
	string curGbsIp;                   //当前级联使用的GBSIP
	bool interConnect;                               //true-组建互联平台
	bool reportCatalog2OtherUas;                     //true-可将该uas目录上报到其他uas。
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetStreamType(const TStreamType& strStreamType)
    {
        streamType = strStreamType;
    }
    TStreamType& GetStreamType()
    {
        return streamType;
    }
    const TStreamType& GetStreamType() const
    {
        return streamType;
    }

public:
    void SetFixGbsIp(const string& strFixGbsIp)
    {
        fixGbsIp = strFixGbsIp;
    }
    string& GetFixGbsIp()
    {
        return fixGbsIp;
    }
    const string& GetFixGbsIp() const
    {
        return fixGbsIp;
    }

    void SetCurGbsIp(const string& strCurGbsIp)
    {
        curGbsIp = strCurGbsIp;
    }
    string& GetCurGbsIp()
    {
        return curGbsIp;
    }
    const string& GetCurGbsIp() const
    {
        return curGbsIp;
    }

public:
    void SetInterConnect(bool bInterConnect)
    {
        interConnect = bInterConnect;
    }
    bool GetInterConnect() const
    {
        return interConnect;
    }

    void SetReportCatalog2OtherUas(bool bReportCatalog2OtherUas)
    {
        reportCatalog2OtherUas = bReportCatalog2OtherUas;
    }
    bool GetReportCatalog2OtherUas() const
    {
        return reportCatalog2OtherUas;
    }

};

struct TGbsGbdevInfo
{
    TGbsGbdevInfo()
    {
        kdChannelIndex = -1;
        parental = 0;
        ipPort = 0;
        devStatus = "OFF";
    }

    string gbid;   // 设备国标编号
    string kdid;   // 设备科达编号
    string kdDomainId;     // 设备科达域号
    int    kdChannelIndex; // 设备本身时为-1
    string gbua;   // 设备通过哪个国标设备上线的
    string name;   // 设备名字
    string manufacturer; // 设备厂商
    string model;        // 设备型号
    string address;      // 设备地址
    int    parental;     // 是否拥有子设备
    string parentID;     // 父设备编号
    string ipAddress;    // 设备IP
    int    ipPort;       // 设备端口
    string devStatus;    // 设备可用状态 ON或OFF

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsBusinessGroup
{
    string id;   // 业务分组编号
    string name; // 业务分组名称
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsVirtualOrganization
{
    string id;              // 虚拟组织编号
    string name;            // 虚拟组织名称
    string parentId;        // 父目录编号，允许为系统目录ID或虚拟组织编号
    string businessGroupId; // 所属的业务分组编号
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsOrganization2Dev
{
    string id;      // 虚拟组织编号
    string devId;   // 设备编号
    string devName; // 设备名称 服务器返回数据时有效
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbsGbParentDomainInfo
{
	string gbid;                //域国标ID
	string name;                //域名
	string devTreeId;           //设备树ID
	string devTreeVer;         //设备树版本

public:
    void SetGbid(const string& strGbid)
    {
        gbid = strGbid;
    }
    string& GetGbid()
    {
        return gbid;
    }
    const string& GetGbid() const
    {
        return gbid;
    }

    void SetName(const string& strName)
    {
        name = strName;
    }
    string& GetName()
    {
        return name;
    }
    const string& GetName() const
    {
        return name;
    }

	void SetDevTreeId(const string& strDevTreeId)
	{
		devTreeId = strDevTreeId;
	}
	string& GetDevTreeId()
	{
		return devTreeId;
	}
	const string& GetDevTreeId() const
	{
		return devTreeId;
	}

	void SetDevTreeVer(const string& strDevTreeVer)
	{
		devTreeVer = strDevTreeVer;
	}
	string& GetDevTreeVer()
	{
		return devTreeVer;
	}
	const string& GetDevTreeVer() const
	{
		return devTreeVer;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

struct TGbDevGrpGbidBatchGetInfo
{
	string groupkdid;           //分组kdid
	string gbid;               //分组gbid
public:
    void SetGroupkdid(const string& strGroupkdid)
    {
        groupkdid = strGroupkdid;
    }
    string& GetGroupkdid()
    {
        return groupkdid;
    }
    const string& GetGroupkdid() const
    {
        return groupkdid;
    }

    void SetGbid(const string& strGbid)
    {
        gbid = strGbid;
    }
    string& GetGbid()
    {
        return gbid;
    }
    const string& GetGbid() const
    {
        return gbid;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TGbDevGbidBatchGetCondition
{
	string  devGrpId;             //分组id(可选)
	string  kdid;               
	int     vidnum;               //视频源总数
	vector<int> vidindexs;        //视频源index
	vector<int> prisec;           //0代表主流，1代表辅流，2代表全部 和vidindexs一一对应
public:
    void SetKdid(const string& strKdid)
    {
        kdid = strKdid;
    }
    string& GetKdid()
    {
        return kdid;
    }
    const string& GetKdid() const
    {
        return kdid;
    }

    void SetVidnum(int nVidnum)
    {
        vidnum = nVidnum;
    }
    int GetVidnum() const
    {
        return vidnum;
    }

	void SetVidindexs(const vector<int>& cVidindexs)
	{
		vidindexs = cVidindexs;
	}
	vector<int>& GetVidindexs()
	{
		return vidindexs;
	}
	const vector<int>& GetVidindexs() const
	{
		return vidindexs;
	}

	void SetPrisec(const vector<int>& cPrisec)
	{
		prisec = cPrisec;
	}
	vector<int>& GetPrisec()
	{
		return prisec;
	}
	const vector<int>& GetPrisec() const
	{
		return prisec;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevGrpId(const string& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    string& GetDevGrpId()
    {
        return devGrpId;
    }
    const string& GetDevGrpId() const
    {
        return devGrpId;
    }

};
struct TGbDevGbidBatchGetInfo
{
	string  devGrpId;             //分组ID
	string  kdid;               
	int     vidindex;             //视频源index
	int     prisec;               //主辅流：0-主流  1-辅流 2-全部
	string  primary;               //主流
	string  second;               //辅流
public:
    void SetKdid(const string& strKdid)
    {
        kdid = strKdid;
    }
    string& GetKdid()
    {
        return kdid;
    }
    const string& GetKdid() const
    {
        return kdid;
    }

    void SetVidindex(int nVidindex)
    {
        vidindex = nVidindex;
    }
    int GetVidindex() const
    {
        return vidindex;
    }
	void SetPrimary(const string& strPrimary)
	{
		primary = strPrimary;
	}
	string& GetPrimary()
	{
		return primary;
	}
	const string& GetPrimary() const
	{
		return primary;
	}

	void SetSecond(const string& strSecond)
	{
		second = strSecond;
	}
	string& GetSecond()
	{
		return second;
	}
	const string& GetSecond() const
	{
		return second;
	}
	void SetPrisec(int nPrisec)
	{
		prisec = nPrisec;
	}
	int GetPrisec() const
	{
		return prisec;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevGrpId(const string& strDevGrpId)
    {
        devGrpId = strDevGrpId;
    }
    string& GetDevGrpId()
    {
        return devGrpId;
    }
    const string& GetDevGrpId() const
    {
        return devGrpId;
    }

};

struct TGbsNatData
{
    enum ProtoType
    {
        udp = 1,
        tcp = 2
    };
    TGbsNatData()
    {
        proto = udp;
        intraNetPortBegin = 0;
        intraNetPortEnd = 0;
        extraNetPortBegin = 0;
        extraNetPortEnd = 0;
    }

    ProtoType proto;
    string intraNetIp;
    u16 intraNetPortBegin;
    u16 intraNetPortEnd;
    string extraNetIp;
    u16 extraNetPortBegin;
    u16 extraNetPortEnd;
    string desc;

public:
    void SetProto(u16 wProto)
    {
        proto = (ProtoType)wProto;
    }
    u16 GetProto()
    {
        return (u32)proto;
    }

    void SetDesc(const string& strDesc)
    {
        desc = strDesc;
    }
    const string& GetDesc() const
    {
        return desc;
    }

    void SetIntraNetIp(const string &strIntraNetIp)
    {
        intraNetIp = strIntraNetIp;
    }
    const string& GetIntraNetIp() const
    {
        return intraNetIp;
    }
    string& GetIntraNetIp()
    {
        return intraNetIp;
    }

    void SetIntraNetPortBegin(u16 wIntraNetPortBegin)
    {
        intraNetPortBegin = wIntraNetPortBegin;
    }
    int GetIntraNetPortBegin()
    {
        return intraNetPortBegin;
    }

    void SetIntraNetPortEnd(u16 wIntraNetPortEnd)
    {
        intraNetPortEnd= wIntraNetPortEnd;
    }
    int GetIntraNetPortEnd()
    {
        return intraNetPortEnd;
    }

    void SetExtraNetIp(const string &strExtraNetIp)
    {
        extraNetIp = strExtraNetIp;
    }
    const string& GetExtraNetIp() const
    {
        return extraNetIp;
    }
    string& GetExtraNetIp()
    {
        return extraNetIp;
    }


    void SetExtraNetPortBegin(u16 wExtraNetPortBegin)
    {
        extraNetPortBegin = wExtraNetPortBegin;
    }
    int GetExtraNetPortBegin()
    {
        return extraNetPortBegin;
    }

    void SetExtraNetPortEnd(u16 wExtraNetPortEnd)
    {
        extraNetPortEnd = wExtraNetPortEnd;
    }
    int GetExtraNetPortEnd()
    {
        return extraNetPortEnd;
    }

public:
    const std::string ToXml() const;
    void FromXml(const std::string &xml_string);
    void ToXml(TiXmlDocument &xmlDoc) const;
    void ToXmlWithoutClassNameNode(TiXmlNode &xmlNode) const;
    void FromXml(const TiXmlNode &xmlNode);
};

struct TGbsRegUasInfo
{
	string gbsIp;                           //GBSIP
	int    upCapacity;                      //上联能力
	bool   primarGbs;                                      //是否主GBS：true-是，false--否
	map<string, string> uasInfo;            //key:国标上级域ID,value:国标上级域别名

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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetUpCapacity(int nUpCapacity)
    {
        upCapacity = nUpCapacity;
    }
    int GetUpCapacity() const
    {
        return upCapacity;
    }

public:
    void SetUasInfo(const map<string, string>& cUasInfo)
    {
        uasInfo = cUasInfo;
    }
    map<string, string>& GetUasInfo()
    {
        return uasInfo;
    }
    const map<string, string>& GetUasInfo() const
    {
        return uasInfo;
    }

public:
    void SetPrimarGbs(bool bPrimarGbs)
    {
        primarGbs = bPrimarGbs;
    }
    bool GetPrimarGbs() const
    {
        return primarGbs;
    }

};

struct TGbsInfo 
{
	int upCapacity;                               //上联能力
	string gbsIp;                                 //gbsIp
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetUpCapacity(int nUpCapacity)
    {
        upCapacity = nUpCapacity;
    }
    int GetUpCapacity() const
    {
        return upCapacity;
    }

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
};

struct TGbsDevInfo
{
	TGbsDevInfo()
	{
		gbLabel = 1;
		devStatus = "OFF";
		online = "OFFLINE";
		channel = 0;
		runStatus = "OK";

		certEndTime = "2037-01-01T01:00:00";
		longitude = INVALID_LONGITUDE;
		latitude = INVALID_LATITUDE;

		kdChannelIndex = -1;
		parental = 0;
		safetyWay = 0;
		registerWay = 0;
		certifiable = 0;
		certErrCode = 0;
		secrecy = 0;
		ipPort = 0;
		channel = 0;
	}

	string gbid;               
	string kdid;               
	string kdDomainId;               
	s32    kdChannelIndex;                // 设备本身时为-1, 设备分组时为-2
	s32 gbLabel;                   // 设备的国标标签, 用于区分设备是否为国标设备
	string gbua;               
	string name;               
	string manufacturer;               
	string model;               
	string owner;               
	string civilCode;               
	string block;               
	string address;               
	s32    parental;               
private:
	string parentID;               
public:
	s32    safetyWay;               
	s32    registerWay;               
	string certNum;               
	s32    certifiable;               
	s32    certErrCode;               
	string certEndTime;               
	s32    secrecy;               
	string ipAddress;               
	s32    ipPort;               
	string password;               
	string devStatus;                                             
	double longitude;               
	double latitude;               
	string firmware;               
	s32    channel;               
	string online;                              
	string runStatus;               
	string abnormalReason;               
	string businessGroupID;               
	TGbDeviceCatalogInfo info;               
public:
	void SetGbid(const string& strGbid)
	{
		gbid = strGbid;
	}
	string& GetGbid()
	{
		return gbid;
	}
	const string& GetGbid() const
	{
		return gbid;
	}

	void SetKdid(const string& strKdid)
	{
		kdid = strKdid;
	}
	string& GetKdid()
	{
		return kdid;
	}
	const string& GetKdid() const
	{
		return kdid;
	}

	void SetKdDomainId(const string& strKdDomainId)
	{
		kdDomainId = strKdDomainId;
	}
	string& GetKdDomainId()
	{
		return kdDomainId;
	}
	const string& GetKdDomainId() const
	{
		return kdDomainId;
	}

	void SetKdChannelIndex(s32 nKdChannelIndex)
	{
		kdChannelIndex = nKdChannelIndex;
	}
	s32 GetKdChannelIndex() const
	{
		return kdChannelIndex;
	}

	void SetGbLabel(s32 cGbLabel)
	{
		gbLabel = cGbLabel;
	}
	s32 GetGbLabel()
	{
		return gbLabel;
	}
	const s32 GetGbLabel() const
	{
		return gbLabel;
	}

	void SetGbua(const string& strGbua)
	{
		gbua = strGbua;
	}
	string& GetGbua()
	{
		return gbua;
	}
	const string& GetGbua() const
	{
		return gbua;
	}

	void SetName(const string& strName)
	{
		name = strName;
	}
	string& GetName()
	{
		return name;
	}
	const string& GetName() const
	{
		return name;
	}

	void SetManufacturer(const string& strManufacturer)
	{
		manufacturer = strManufacturer;
	}
	string& GetManufacturer()
	{
		return manufacturer;
	}
	const string& GetManufacturer() const
	{
		return manufacturer;
	}

	void SetModel(const string& strModel)
	{
		model = strModel;
	}
	string& GetModel()
	{
		return model;
	}
	const string& GetModel() const
	{
		return model;
	}

	void SetOwner(const string& strOwner)
	{
		owner = strOwner;
	}
	string& GetOwner()
	{
		return owner;
	}
	const string& GetOwner() const
	{
		return owner;
	}

	void SetCivilCode(const string& strCivilCode)
	{
		civilCode = strCivilCode;
	}
	string& GetCivilCode()
	{
		return civilCode;
	}
	const string& GetCivilCode() const
	{
		return civilCode;
	}

	void SetBlock(const string& strBlock)
	{
		block = strBlock;
	}
	string& GetBlock()
	{
		return block;
	}
	const string& GetBlock() const
	{
		return block;
	}

	void SetAddress(const string& strAddress)
	{
		address = strAddress;
	}
	string& GetAddress()
	{
		return address;
	}
	const string& GetAddress() const
	{
		return address;
	}

	void SetParental(s32 nParental)
	{
		parental = nParental;
	}
	s32 GetParental() const
	{
		return parental;
	}

	void SetParentID(const string& strParentID)
	{
		parentID = strParentID;
	}
	string& GetParentID()
	{
		return parentID;
	}
	const string& GetParentID() const
	{
		return parentID;
	}

	void SetSafetyWay(s32 nSafetyWay)
	{
		safetyWay = nSafetyWay;
	}
	s32 GetSafetyWay() const
	{
		return safetyWay;
	}

	void SetRegisterWay(s32 nRegisterWay)
	{
		registerWay = nRegisterWay;
	}
	s32 GetRegisterWay() const
	{
		return registerWay;
	}

	void SetCertNum(const string& strCertNum)
	{
		certNum = strCertNum;
	}
	string& GetCertNum()
	{
		return certNum;
	}
	const string& GetCertNum() const
	{
		return certNum;
	}

	void SetCertifiable(s32 nCertifiable)
	{
		certifiable = nCertifiable;
	}
	s32 GetCertifiable() const
	{
		return certifiable;
	}

	void SetCertErrCode(s32 nCertErrCode)
	{
		certErrCode = nCertErrCode;
	}
	s32 GetCertErrCode() const
	{
		return certErrCode;
	}

	void SetCertEndTime(const string& strCertEndTime)
	{
		certEndTime = strCertEndTime;
	}
	string& GetCertEndTime()
	{
		return certEndTime;
	}
	const string& GetCertEndTime() const
	{
		return certEndTime;
	}

	void SetSecrecy(s32 nSecrecy)
	{
		secrecy = nSecrecy;
	}
	s32 GetSecrecy() const
	{
		return secrecy;
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

	void SetIpPort(s32 nIpPort)
	{
		ipPort = nIpPort;
	}
	s32 GetIpPort() const
	{
		return ipPort;
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

	void SetLongitude(double nLongitude)
	{
		longitude = nLongitude;
	}
	double GetLongitude() const
	{
		return longitude;
	}

	void SetLatitude(double nLatitude)
	{
		latitude = nLatitude;
	}
	double GetLatitude() const
	{
		return latitude;
	}

	void SetFirmware(const string& strFirmware)
	{
		firmware = strFirmware;
	}
	string& GetFirmware()
	{
		return firmware;
	}
	const string& GetFirmware() const
	{
		return firmware;
	}

	void SetChannel(s32 nChannel)
	{
		channel = nChannel;
	}
	s32 GetChannel() const
	{
		return channel;
	}

	void SetOnline(const string& cOnline)
	{
		online = cOnline;
	}
	string& GetOnline()
	{
		return online;
	}
	const string& GetOnline() const
	{
		return online;
	}

	void SetRunStatus(const string& strRunStatus)
	{
		runStatus = strRunStatus;
	}
	string& GetRunStatus()
	{
		return runStatus;
	}
	const string& GetRunStatus() const
	{
		return runStatus;
	}

	void SetAbnormalReason(const string& strAbnormalReason)
	{
		abnormalReason = strAbnormalReason;
	}
	string& GetAbnormalReason()
	{
		return abnormalReason;
	}
	const string& GetAbnormalReason() const
	{
		return abnormalReason;
	}

	void SetBusinessGroupID(const string& strBusinessGroupID)
	{
		businessGroupID = strBusinessGroupID;
	}
	string& GetBusinessGroupID()
	{
		return businessGroupID;
	}
	const string& GetBusinessGroupID() const
	{
		return businessGroupID;
	}

	void SetInfo(const TGbDeviceCatalogInfo& cInfo)
	{
		info = cInfo;
	}
	TGbDeviceCatalogInfo& GetInfo()
	{
		return info;
	}
	const TGbDeviceCatalogInfo& GetInfo() const
	{
		return info;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetDevStatus(const string& strDevStatus)
    {
        devStatus = strDevStatus;
    }
    string& GetDevStatus()
    {
        return devStatus;
    }
    const string& GetDevStatus() const
    {
        return devStatus;
    }

};

#endif // _GBS_STRUCT_H_
































