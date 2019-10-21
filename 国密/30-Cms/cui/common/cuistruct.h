/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : cuistruct.h
   相关文件    : 
   文件实现功能: 定义CUI_FCGI 和CUI_SERVER之间的协议
   作者        : lianlgi
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/12/28   1.0         liangli        创建
******************************************************************************/
#ifndef _CUI_STRUCT_H_
#define _CUI_STRUCT_H_

/*
    注意：CUI的大部分协议都是透传的，此些协议复用3as或者cmu的即可
    一旦出现附加的信息，则需要重新定义
*/

#include <string>
#include <vector>
#include <list>
using std::string;
using std::vector;
using std::list;
#include "cms/cms_errorcode.h"
#include "cms/tas/tas_proto.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/rcs/rcs_proto.h"
#include "cms/mps/mps_proto.h"
#include "cms/uas/uas_proto.h"
#include "cms/gbs/gbs_proto.h"
#include "cms/caps/caps_proto.h"
#include "cms/dps/dps_proto.h"
#include "cuievent.h"
#include <sstream>


#define INVALID_PLAY_ID             (s32)(-1)
#define INVALID_ENCODE_ID           INVALID_PLAY_ID
#define INVALID_VIDEOSOURCE_ID      INVALID_PLAY_ID

#define MAX_CUI_SEQMARKS_SIZE       (s32)(0x7FFFFFFF) // 2147483647 
#define MIN_CUI_SEQMARKS_SIZE       (s32)(1000)
class CPeQueryTask;

/*====================================================================
函数名      : GetNameWithoutDomainFromURI
功能        : URI = name@domain;从URI中返回name字段
算法实现    : 注意，返回的string不能是引用
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/2/             liangli
====================================================================*/
inline string GetNameWithoutDomainFromURI( const string &strURI )
{
    string::size_type strPos = strURI.find('@');
    if (strPos != string::npos)
    {
        string strName;
        strName.assign(strURI, 0, strPos);
        return strName;
    }

    return strURI;
}

/*====================================================================
函数名      : GetDomainNameWithoutPreNameFromURI
功能        : 获取URI = name@domain；中的域名部分
算法实现    : 注意，返回的是string，不是引用
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/17/             liangli
====================================================================*/
inline string GetDomainNameWithoutPreNameFromURI(const string &strURI)
{
    string strDomain;
    string::size_type strPos = strURI.find('@');
    if ( strPos != string::npos )
    {
        strDomain.assign(strURI, strPos+1, strURI.size()-strPos-1);
    }
    return strDomain;
}

inline void GetValueWithoutDescFromFormatParam(const string &strFormatParam, int &VideoWidth, int &videoHeight)
{
    if(!strFormatParam.empty())
    {
        if (2 != sscanf(strFormatParam.c_str(), "W:%d,H:%d", &VideoWidth, &videoHeight))
        {
            //解析失败,不处理，走默认值
        }        
//         string strVideoWidth;
//         string::size_type strPos = strFormatParam.find(':');
//         if (strPos != string::npos)
//         {
//             strVideoWidth.assign(strFormatParam, 0, strPos);
//         }
// 
//         string strVideoHeight;
//         if ( strPos != string::npos )
//         {
//             strVideoHeight.assign(strFormatParam, strPos+1, strFormatParam.size()-strPos-1);
//         }
// 
//         if(!strVideoWidth.empty())
//         {
//             VideoWidth = atoi(strVideoWidth.c_str());
//         }
//         
//         if(!strVideoHeight.empty())
//         {
//             videoHeight = atoi(strVideoHeight.c_str());
//         }     
    }
}

inline string TransInt2String(int value)
{
    std::stringstream out;

    out<<value;
    string str = out.str();

    return str;
}

inline string TransDouble2String(double value)
{
    std::stringstream out;

    out<<value;
    string str = out.str();

    return str;
}

inline int GetPtzPriLevelFromSet( const set<int> &cPri, const int nMaxPri = CMS_PTZ_MAX_PRI_LEV )
{
    if ( cPri.empty() )
    {
        return 0;
    }
    //循环寻找最大权限
    int nPriLevel = 0;
    set<int>::iterator pItem = cPri.begin();
    while (pItem != cPri.end())
    {
        if ( *pItem+1 <= nMaxPri )
        {
            if (*pItem+1 > nPriLevel)
            {
                nPriLevel = *pItem+1;
            }
        }
        if ( nPriLevel == nMaxPri )
        { //已经是最大值
            break;
        }

        pItem ++;
    }
    return nPriLevel;
}

inline int GetPtzPriLevelFromSet( const CBitsContainer &cPri, const int nMaxPri = CMS_PTZ_MAX_PRI_LEV )
{
    s32 nPrivi = 0;
    for (s32 i = 9; i >= 0; --i)
    {
        if (cPri.IsBitOn((u32)(CVideosourcePrivilege::en_PTZ_Ctrl+i-1)))
        {
            nPrivi = i+1;
            break;
        }
    }
    return nPrivi;

//     CMS_ASSERT(nMaxPri<32);
//     //循环寻找最大权限
//     int nPriLevel = 0;
//     for ( int nIndex=1;nIndex<=nMaxPri;nIndex++ )
//     {
//         if ( cPri.IsBitOn(nIndex-1) )
//         {
//             if ( nIndex <= nMaxPri )
//             {
//                 if (nIndex > nPriLevel)
//                 {
//                     nPriLevel = nIndex;
//                 }
//             }
//             if ( nPriLevel == nMaxPri )
//             { //已经是最大值
//                 break;
//             }
//         }
//     }
//     return nPriLevel;
}

inline string GetCuURL(const string &strIP, const string &strPort,const string &strCuURL)
{
    return (string("http://") + strIP + ":" + strPort + "/" + strCuURL);
}



class CCuiCuLoginReq : public CCuLoginReq //因为CCuLoginReq可以透传给CMU，所以可以typedef
{
public:
    const TIpAddr &GetClientIp() const
    {
        return clientIp;
    }

    TIpAddr &GetClientIp()
    {
        return clientIp;
    }

    void SetClientIp(const TIpAddr& strIp)
    {
        clientIp = strIp;
    }

    const TIpAddr &GetServiceIp() const
    {
        return serviceIp;
    }

    TIpAddr &GetServiceIp()
    {
        return serviceIp;
    }

    void SetServiceIp(const TIpAddr& strIp)
    {
        serviceIp = strIp;
    }

    const TIpAddr &GetHostIp() const
    {
        return hostIp;
    }

    TIpAddr &GetHostIp()
    {
        return hostIp;
    }

    void SetHostIp(const TIpAddr& strIp)
    {
        hostIp = strIp;
    }

	const TIpAddr &GetWebCuIp() const
	{
		return webCUClientIP;
	}

	TIpAddr &GetWebCuIp()
	{
		return webCUClientIP;
	}

	void SetWebCuIp(const TIpAddr& strIp)
	{
		webCUClientIP = strIp;
	}

    const string &GetClientType() const
    {
        return clientType;
    }

    string &GetClientType()
    {
        return clientType;
    }

    void SetClientType(const string& strClientType)
    {
        clientType = strClientType;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
protected:
    TIpAddr clientIp;
    TIpAddr serviceIp; // 服务器的外部IP，也即客户端使用的服务器IP
    TIpAddr hostIp;    // 服务器的本地IP
	TIpAddr webCUClientIP; //WebCU客户端地址
    string clientType; // 客户端的类型
};

class CCuiCuLoginRsp : public CCuLoginRsp
{
public:
    void Clear()
    {
        stunInfo.Clear();
    }
    const CSTUNServerInfo& GetStunServerInfo() const
    {
        return stunInfo;
    }
    void SetStunServerInfo(const CSTUNServerInfo &cInfo)
    {
        stunInfo = cInfo;
    }
    string& GetCuVersion()
    {
        return cuVersion;
    }
    const string& GetCuVersion() const
    {
        return cuVersion;
    }

    void SetCuVersion(const string&cCuVersion)
    {
        cuVersion = cCuVersion;
    }

    const string& GetUpdateUrl() const
    {
        return cuUpdateUrl;
    }

    void SetUpdateUrl(const string&cCuUpdateUrl)
    {
        cuUpdateUrl = cCuUpdateUrl;
    }

    void SetDataVersion(const string& strServerDataVersion)
    {
        serverDataVersion = strServerDataVersion;
    }

    const string& GetDataVersion() const
    {
        return serverDataVersion;
    }

    string& GetDataVersion()
    {
        return serverDataVersion;
    }

	void SetUserLockRemainTime(const s32 val)
	{
		userLockRemainTime = val;
	}
	s32 GetUserLockRemainTime()
	{
		return userLockRemainTime;
	}
	const s32 GetUserLockRemainTime() const
	{
		return userLockRemainTime;
	}

	void SetPasswordRemainTime(const s32 val)
	{
		passwordRemainTime = val;
	}
	s32 GetPasswordRemainTime()
	{
		return passwordRemainTime;
	}
	const s32 GetPasswordRemainTime() const
	{
		return passwordRemainTime;
	}
	void SetSecureEnable(bool bEnable)
	{
		isSecureEnabled = bEnable;
	}
	bool IsSecureEnable() const
	{
		return isSecureEnabled;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
protected:
    CSTUNServerInfo stunInfo;
    string cuVersion;
    string cuUpdateUrl;
    string serverDataVersion;
	s32 userLockRemainTime;    //用户锁定剩余时间(单位:秒)
	s32 passwordRemainTime;   //密码距离到期还剩余的时间
	bool isSecureEnabled;
};

//CCuLogoutReq 和 CCuLogoutRsp复用cmu_proto.h的

//定义CU的通知消息
class CCuDeviceGroupRelation
{
private:
    std::string devGroupId;	/* required element of type xsd:string */
    std::string devId;	/* required element of type xsd:string */
    std::vector<int > videoSrcIdList;	/* optional element of type xsd:int */
    std::vector<int > alarmInputChnList;	/* optional element of type xsd:int */
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuDeviceFullInfo : public CDeviceInfo
{
private:
    std::vector<CCuDeviceGroupRelation > devGroupToChannel;	/* optional element of type cusdk:DeviceGroupRelation */
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//Invite停止通知，该消息不属于平台内部消息，是CUI和CU之间的通知，所以直接继承CSipEvent
class CInviteStopNotify : public CSipEvent
{
public:
    int playId;
    int errorCode;
public:
    CInviteStopNotify()
    {
        playId = INVALID_PLAY_ID;
        errorCode = CMS_SUCCESS;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuUserInfo : public CUserInfo
{
public:
    string &GetDomainID()
    {
        return domainID;
    }
    const string &GetDomainID() const
    {
        return domainID;
    }
    void SetDomainID(const string &val)
    {
        domainID = val;
    }
    CCuUserInfo &operator =(const CUserInfo &val)
    {
        CUserInfo *pBaseUser = this;
        *pBaseUser = val;
  //      SetUserID(val.GetUserID());
  //      SetUsergrpID(val.GetUsergrpID());
  //      SetUserName(val.GetUserName());
  //      SetUserPwd(val.GetUserPwd());
  //      SetUserType(val.GetUserType());
  //      SetMLoginFlag(val.GetMLoginFlag());
  //      SetEnabledFlag(val.GetEnabledFlag());
  //      SetDescription(val.GetDescription());
  //      SetNameExpireDate(val.GetNameExpireDate());
  //      SetPwdExpireDate(val.GetPwdExpireDate());
  //      SetCreateDate(val.GetCreateDate());
  //      SetCreatorID(val.GetCreatorID());
  //      SetUserPrivilege(val.GetUserPrivilege());
		//SetPwdExpireEnabledFlag(val.GetPwdExpireEnabledFlag());
		//SetPwdExpireTime(val.GetPwdExpireTime());
		//SetLoginIpList(val.GetLoginIpList());
		//SetIpListEnabledFlag(val.GetIpListEnabledFlag());
		//SetIpBindFlagForMobileClient(val.GetIpBindFlagForMobileClient());
  //      SetSwitchNum( val.GetSwitchNum() );

  //      SetWaterMarkEnabled( val.GetWaterMarkEnabled() );
  //      SetWaterMarkFont( val.GetWaterMarkFont() );
  //      SetWaterMarkFontSize(  val.GetWaterMarkFontSize() );
  //      SetWaterMarkRotationAngle( val.GetWaterMarkRotationAngle() );
  //      SetWaterMarkColor( val.GetWaterMarkColor() );
  //      SetWaterMarkTransparent( val.GetWaterMarkTransparent() );
  //      SetWaterMarkTiled( val.GetWaterMarkTiled() );

        return *this;
    }
protected:
    //string userName;			// 登录时用的用户名. lee@kedacom //将3AS发送过来的CUserInfo的UserName修改
    string domainID;			// 用户所在域ID.
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGetNotifyReq : public CEventReq
{
public:
    CGetNotifyReq()
    {
        event = OspExtEventDesc(CU_CUI_GET_NOTIFY_REQ);
    }
    void Clear()
    {
        CEventReq::Clear();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
protected:
    int timeoutSeconds;                
public:
    void SetTimeoutSeconds(int nTimeoutSeconds)
    {
        timeoutSeconds = nTimeoutSeconds;
    }
    int GetTimeoutSeconds() const
    {
        return timeoutSeconds;
    }

};

class CCuExportFailNotify : public CSipEvent
{
public:
    string exportFileUrl;
    int errorCode;
public:
    CCuExportFailNotify()
    {
        errorCode = CMS_SUCCESS;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TServiceStatus
{
    enum
    {
        enServiceRecord = 0, 
        enServiceAlarmLink = 1, 
        enServiceTVWall = 2, 
        enServiceMap = 3, 
        enServiceLog = 4, 
        enServiceConfig = 5
    };

    int nServiceType; // 值参见上面的枚举
    bool bInService;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuPuTransparentQryNtfReq : public CPuTransparentQryNtfReq
{
public:
	CCuPuTransparentQryNtfReq()
   {
         Clear();
   }
   void Clear()
   {
        devChn.Clear();
        srvName.clear();
   }

   void SetSrvName(const TPuTransOprType& strSrvName)
    {
        srvName = strSrvName;
    }
    TPuTransOprType& GetSrvName()
    {
        return srvName;
    }
    const TPuTransOprType& GetSrvName() const
    {
        return srvName;
    }

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

private:
   TChannel          devChn;                          //设备通道
   TPuTransOprType   srvName;                         //业务类型

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuStartEncoderTransNtfReq : public CEventReq
{
public:
	CCuStartEncoderTransNtfReq()
	{
		event = OspExtEventDesc(CU_CUI_START_ENCODER_TRANS_NTF_REQ);
	}

	void Clear()
	{
		clientEncChnId = INVALID_ENCODE_ID;
		tMediaDescList.clear();
		tExtInfo.clear();
		CEventReq::Clear();
	}

private:
	int clientEncChnId;
	TMediaDescList tMediaDescList;
	TExtInfo tExtInfo;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetClientEncChnId(int nClientEncChnId)
    {
        clientEncChnId = nClientEncChnId;
    }
    int GetClientEncChnId() const
    {
        return clientEncChnId;
    }

    void SetTMediaDescList(const TMediaDescList& cTMediaDescList)
    {
        tMediaDescList = cTMediaDescList;
    }
    TMediaDescList& GetTMediaDescList()
    {
        return tMediaDescList;
    }
    const TMediaDescList& GetTMediaDescList() const
    {
        return tMediaDescList;
    }

    void SetTExtInfo(const TExtInfo& cTExtInfo)
    {
        tExtInfo = cTExtInfo;
    }
    TExtInfo& GetTExtInfo()
    {
        return tExtInfo;
    }
    const TExtInfo& GetTExtInfo() const
    {
        return tExtInfo;
    }

};

class CCuStartEncoderTransNtfRsp : public CEventRsp
{
public:
	CCuStartEncoderTransNtfRsp()
	{
		event = OspExtEventDesc(CU_CUI_START_ENCODER_TRANS_NTF_RSP);
	}

	void Clear()
	{
		CEventRsp::Clear();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCertificateQryReq : public CEventReq
{
public:
	CCertificateQryReq()
	{
		event = OspExtEventDesc(CU_CUI_SECURE_CERTIFICATE_QRY_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();

		moduleId = "";
		id = "";
	}

public:
	void SetModuleId(const std::string& strModuleId)
	{
		moduleId = strModuleId;
	}
	std::string& GetModuleId()
	{
		return moduleId;
	}
	const std::string& GetModuleId() const
	{
		return moduleId;
	}
	void SetId(const std::string& strId)
	{
		id = strId;
	}
	std::string& GetId()
	{
		return id;
	}
	const std::string& GetId() const
	{
		return id;
	}

protected:
	std::string moduleId;// 表示谁查询的
	std::string id; // 证书拥有者id

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CCertificateQryRsp : public CEventRsp
{
public:
	CCertificateQryRsp()
	{
		event = OspExtEventDesc(CU_CUI_SECURE_CERTIFICATE_QRY_RSP);
	}

	void Clear()
	{
		CEventRsp::Clear();
	}

public:
	void SetSignatureCert(const std::string& strSignatureCert)
	{
		signatureCert = strSignatureCert;
	}
	std::string& GetSignatureCert()
	{
		return signatureCert;
	}
	const std::string& GetSignatureCert() const
	{
		return signatureCert;
	}

	void SetEncryptCert(const std::string& strEncryptCert)
	{
		encryptCert = strEncryptCert;
	}
	std::string& GetEncryptCert()
	{
		return encryptCert;
	}
	const std::string& GetEncryptCert() const
	{
		return encryptCert;
	}

protected:
	std::string signatureCert; // 签名证书                
	std::string encryptCert; // 加密证书    

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CCertificateQryReq CCuCertificateQryReq;
typedef CCertificateQryRsp CCuCertificateQryRsp;
typedef CPuSignatrueCtrlReq CCuDeviceSignatrueCtrlReq;
typedef CPuSignatrueCtrlRsp CCuDeviceSignatrueCtrlRsp;
typedef CPuEncryptionCtrlReq CCuDeviceEncryptionCtrlReq;
typedef CPuEncryptionCtrlRsp CCuDeviceEncryptionCtrlRsp;

/*
    缓存所有的3AS和CMU或者其他平台模块的通知消息
*/

class CCuNotifyPool
{
public:
    enum ENotifyType
    {
        ETYPE_INVALID = 0,    //无效通知类型
        ETYPE_DEVICE_ADD_NOTIFY = 1,
        ETYPE_DEVICE_DEL_NOTIFY = 2,
        ETYPE_DEVICE_MOD_NOTIFY = 3,
        ETYPE_VIDEOSRCALIAS_CHANGE_NOTIFY = 4,
        ETYPE_DEVICE_GROUP_ADD_NOTIFY = 5,
        ETYPE_DEVICE_GROUP_DEL_NOTIFY = 6,
        ETYPE_DEVICE_GROUP_MOD_NOTIFY = 7,
        ETYPE_DEVICE_ONLINE_NOTIFY = 8,
        ETYPE_DEVICE_CONFIG_NOTIFY = 9,
        ETYPE_DEVICE_SERVICE_STATUS_NOTIFY = 10,
        ETYPE_DEVICE_ALARM_NOTIFY = 11,
        ETYPE_INVITE_STOP_NOTIFY  = 12,           //Invite终止通知
        ETYPE_TVWALL_STATUS_NOTIFY = 13,
        ETYPE_USER_ADD_NOTIFY = 14,
        ETYPE_USER_DEL_NOTIFY = 15,
        ETYPE_USER_MOD_NOTIFY = 16,
        ETYPE_USER_GROUP_ADD_NOTIFY = 17,
        ETYPE_USER_GROUP_DEL_NOTIFY = 18,
        ETYPE_USER_GROUP_MOD_NOTIFY = 19,
        ETYPE_TVWALL_ADD_NOTIFY = 20,
        ETYPE_TVWALL_DEL_NOTIFY = 21,
        ETYPE_TVWALL_MOD_NOTIFY = 22,
        ETYPE_DEVICE_GROUP_ADD_DEVICE_NOTIFY = 23,
        ETYPE_DEVICE_GROUP_DEL_DEVICE_NOTIFY = 24,
        ETYPE_ALARMLINK_CONFIG_CHANGE_NOTIFY = 25,
        ETYPE_EXPORT_FAIL_NOTIFY = 26,
        ETYPE_STUN_UPDATE_NOTIFY = 27,
        ETYPE_SERVICE_STATUS_NOTIFY = 28,
        ETYPE_GPS_NOTIFY = 29,
		ETYPE_DEVICE_TRANSDATA_NOTIFY = 30,
		ETYPE_GBS_PDCL_PROGRESS_NOTIFY = 31,
		ETYPE_ALARM_EVENT_NOTIFY = 32,
		ETYPE_START_ENCODER_TRANS_NOTIFY = 33, // cusdk__GetNotifyRsp last seqmarks for cusdk.h


		//CU尚未实现
		ETYPE_MAP_ADD_NOTIFY,
		ETYPE_MAP_MOD_NOTIFY,
		ETYPE_MAP_DEL_NOTIFY,
		ETYPE_MAP_LAYER_ADD_NOTIFY,
		ETYPE_MAP_LAYER_MOD_NOTIFY,
		ETYPE_MAP_LAYER_DEL_NOTIFY,
		ETYPE_MAP_AREA_ADD_NOTIFY,
		ETYPE_MAP_AREA_MOD_NOTIFY,
		ETYPE_MAP_AREA_DEL_NOTIFY,
		ETYPE_MAP_ELEMENT_ADD_NOTIFY,
		ETYPE_MAP_ELEMENT_MOD_NOTIFY,
		ETYPE_MAP_ELEMENT_DEL_NOTIFY,
		ETYPE_TRANSPARENT_QUR_NOTIFY,
		ETYPE_NUMBER
    };
public:
    std::list< CDeviceInfo > deviceAddNotify;     //实现上存在问题，CU需要的是用户设备完整信息，而3AS给的分散的信息
    std::list< string > deviceDelNotify;  // deviceId
    //std::vector< CCuDeviceFullInfo > deviceModNotify;
    std::list< CDeviceCapIndexNameModNtfReq> CapChangeNotify;
    std::list< CDeviceGroupAddNtfReq > deviceGroupAddNotify;
    std::list< string > deviceGroupDelNotify;
    std::list< CDeviceGroupModNtfReq > deviceGroupModNotify;

    std::list< CPuStatusNtyReq > devOnlineNotify;
    std::list< CPuConfigNtyReq > devConfigNotify;
    std::list< CPuServiceStatusNtyReq > devServiceStatusNotify;
    std::list< CPuAlarmStatusNtyReq > devAlarmNotify;
    std::list< CPuTransDataStatusNtyReq > devTransdataNotify;
    std::list< CPuGpsNtyReq > devGpsNotify;

    ///////////////////////////////////////////////////////////
    std::list< CInviteStopNotify > inviteStopNotify;
    std::list< CTvWallStatusNtyReq > tvWallStatusNotify;
    std::list< CCuUserInfo > userAddNotify;
    std::list< string > userDelNotify;
    std::list< CCuUserInfo >  userModNotify;
//     std::list< CUserGroupAddNtfReq> userGroupAddNotify;
//     std::list< CUserGroupDelNtfReq> userGroupDelNotify;
//     std::list< CUserGroupModNtfReq> userGroupModNotify;
    std::list< CTvwallAddNtfReq> tvWallAddNotify;
    std::list< string > tvWallDelNotify;
    std::list< CTvwallModNtfReq> tvWallModNotify;
    std::list< CUserGroupAddNtfReq> userGroupAddNotify;
    std::list< string > userGroupDelNotify;
    std::list< CUserGroupModNtfReq> userGroupModNotify;
    std::list< CDeviceGroupDeviceAddNtfReq> devGroupDevAddNotify;
    std::list< CDeviceGroupDeviceDelNtfReq> devGroupDevDelNotify;
    std::list< CAlarmLinkModNtfReq> alarmLinkModNotify;
    ///////////////////////////////////////////////////////////
    std::list< CCuExportFailNotify > exportFailNotify;
    std::list< CStunUpdateReq> stunUpdateNotify; //stun服务器变更通知
    std::list< TServiceStatus> serviceStatusNotify;
	////////////电子地图变更通知/////////////////////////////
	std::list<CMapAddNtfReq> mapAddNotify;
	std::list<CMapModNtfReq> mapModNotify;
	std::list<CMapDelNtfReq> mapDelNotify;
	std::list<CMapLayerAddNtfReq> mapLayerAddNotify;
	std::list<CMapLayerModNtfReq> mapLayerModNotify;
	std::list<CMapLayerDelNtfReq> mapLayerDelNotify;
	std::list<CMapAreaAddNtfReq> mapAreaAddNotify;
	std::list<CMapAreaModNtfReq> mapAreaModNotify;
	std::list<CMapAreaDelNtfReq> mapAreaDelNotify;
	std::list<CMapElementAddNtfReq> mapElementAddNotify;
	std::list<CMapElementModNtfReq> mapElementModNotify;
	std::list<CMapElementDelNtfReq> mapElementDelNotify;
	std::list<CCuPuTransparentQryNtfReq> mapTransparentNotify;
	/////////////////////////////////////////////////////////
	std::list<CGbsGbPDCLProgressNtfReq> gbGbsGbPDCLProgressNotify;
	std::list<CAlarmEventNtfReq> alarmEventNotify;//告警事件通知
	std::list<CCuStartEncoderTransNtfReq> startEncoderTransNotify; // 解决音频呼叫停止再呼叫的时序问题
	std::list< int > seqMarks;   //通知消息的序列标记
    bool hasDataLeft;   //是否还有剩余通知数据
private:
	int seqMarksSize;   // 缓存的通知消息数量
	int maxSeqMarksSize; // 缓存的通知消息最大数量
public:
    CCuNotifyPool()
    {
        Clear();
    }
    void Clear()
    {
		deviceAddNotify.clear();
        deviceDelNotify.clear();
        CapChangeNotify.clear();
        deviceGroupAddNotify.clear();
        deviceGroupDelNotify.clear();
        deviceGroupModNotify.clear();
        devOnlineNotify.clear();
        devConfigNotify.clear();
        devServiceStatusNotify.clear();
        devAlarmNotify.clear();
        devTransdataNotify.clear();
        inviteStopNotify.clear();
        tvWallStatusNotify.clear();
        devGpsNotify.clear();
        userAddNotify.clear();
        userDelNotify.clear();
        userModNotify.clear();
        tvWallAddNotify.clear();
        tvWallDelNotify.clear();
        tvWallModNotify.clear();
        devGroupDevAddNotify.clear();
        devGroupDevDelNotify.clear();
        alarmLinkModNotify.clear();

        exportFailNotify.clear();
        stunUpdateNotify.clear();
        serviceStatusNotify.clear();

		mapAddNotify.clear();
		mapModNotify.clear();
		mapDelNotify.clear();

		mapLayerAddNotify.clear();
		mapLayerModNotify.clear();
		mapLayerDelNotify.clear();

		mapAreaAddNotify.clear();
		mapAreaModNotify.clear();
		mapAreaDelNotify.clear();

		mapElementAddNotify.clear();
		mapElementModNotify.clear();
		mapElementDelNotify.clear();

		mapTransparentNotify.clear();

		gbGbsGbPDCLProgressNotify.clear();

		alarmEventNotify.clear();
		startEncoderTransNotify.clear();

        seqMarks.clear();
		seqMarksSize = 0;
		maxSeqMarksSize = MAX_CUI_SEQMARKS_SIZE;

        hasDataLeft = false;
    }
	bool IsCuNtfEmpty() const
	{
		return seqMarksSize == 0;
	}
	bool IsCuNtfOverFlow() const
	{
		if ( seqMarksSize < maxSeqMarksSize )
		{
			return false;
		}

		return true;
	}
	int GetCuNtfSize()
	{
		return seqMarksSize;
	}
	int GetMaxCuNtfSize()
	{
		return maxSeqMarksSize;
	}
	void SetMaxCuNtfSize( int ntfSize )
	{
		if ( ntfSize <= 0 )
		{
			maxSeqMarksSize = MAX_CUI_SEQMARKS_SIZE;
		}

		maxSeqMarksSize  = ntfSize;
	}
    bool IsEmpty() const
    {
        bool bEmpty = true;
        do 
		{
			if (!deviceAddNotify.empty())
			{
				bEmpty = false;
				break;
			}
            if ( ! deviceDelNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! CapChangeNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! deviceGroupAddNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! deviceGroupDelNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! deviceGroupModNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! devOnlineNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! devConfigNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! devServiceStatusNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! devAlarmNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( !devTransdataNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if ( ! inviteStopNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if( ! tvWallStatusNotify.empty() )
            {
                bEmpty = false;
                break;
            }
            if (! devGpsNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !userAddNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !userDelNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !userModNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !tvWallAddNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !tvWallDelNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !tvWallModNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if(!userGroupAddNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if(!userGroupDelNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if(!userGroupModNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !devGroupDevAddNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !devGroupDevDelNotify.empty())
            {
                bEmpty = false;
                break;
            }
            if( !alarmLinkModNotify.empty())
            {
                bEmpty = false;
                break;
            }

            if( !exportFailNotify.empty())
            {
                bEmpty = false;
                break;
            }

            if( !stunUpdateNotify.empty())
            {
                bEmpty = false;
                break;
            }

            if (!serviceStatusNotify.empty())
            {
                bEmpty = false;
                break;
            }

			if (!mapAddNotify.empty())
			{
				bEmpty = false;
				break;
			}

			if (!mapModNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapDelNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapLayerAddNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapLayerModNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapLayerDelNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapAreaAddNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapAreaModNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapAreaDelNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapElementAddNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapElementModNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapElementDelNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!mapTransparentNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!gbGbsGbPDCLProgressNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!alarmEventNotify.empty())
			{
				bEmpty = false;
				break;
			}
			if (!startEncoderTransNotify.empty())
			{
				bEmpty = false;
				break;
			}
        } while (0);
        return bEmpty;
    }
    void InsertDeviceAddNotify(const CDeviceInfo &devInfo)
    {
		if (IsCuNtfOverFlow())
		{
			return;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_DEVICE_ADD_NOTIFY);
		deviceAddNotify.push_back(devInfo);
    }
    void InsertDeviceDelNotify(const string &Ntf)
    {
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_DEL_NOTIFY);
        deviceDelNotify.push_back(Ntf);
    }
    void InsertDeviceModNotify()
    {
		// hzytodo2
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

    }
    void InsertCapChangeNotify(const CDeviceCapIndexNameModNtfReq &Ntf, bool bIsSplitSeqMarks = false)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		// 发送到CU的消息需要拆分成多条，seqMarks不能只有1个，且必须保证顺序正确
		seqMarksSize++;
		seqMarks.push_back(ETYPE_VIDEOSRCALIAS_CHANGE_NOTIFY);

		if ( bIsSplitSeqMarks )
		{
			int seqNum = Ntf.GetDeviceCapName().deviceCapIndexNames.size();
			for (int i = 1; i < seqNum; i++)
			{
				seqMarksSize++;
				seqMarks.push_back(ETYPE_VIDEOSRCALIAS_CHANGE_NOTIFY);
			}
		}

        CapChangeNotify.push_back(Ntf);
    }
    void InsertDeviceGroupAddNotify(const CDeviceGroupAddNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_GROUP_ADD_NOTIFY);
        deviceGroupAddNotify.push_back(Ntf);
    }
    void InsertDeviceGroupDelNotify(const string &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_GROUP_DEL_NOTIFY);
        deviceGroupDelNotify.push_back(Ntf);
    }
    void InsertDeviceGroupModNotify(const CDeviceGroupModNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_GROUP_MOD_NOTIFY);
        deviceGroupModNotify.push_back(Ntf);
    }
    void InsertSubNotify(const CPuStatusNtyReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_ONLINE_NOTIFY);
        devOnlineNotify.push_back(Ntf);
    }
    void InsertSubNotify(const CPuTransDataStatusNtyReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_TRANSDATA_NOTIFY);
        devTransdataNotify.push_back(Ntf);
    }
    void InsertSubNotify(const CPuConfigNtyReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_CONFIG_NOTIFY);
        devConfigNotify.push_back(Ntf);
    }
    void InsertSubNotify(const CPuServiceStatusNtyReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_SERVICE_STATUS_NOTIFY);
        devServiceStatusNotify.push_back(Ntf);
    }

    void InsertSubNotify(const CRcsPuRecordStatusNtyReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

        std::list< CPuServiceStatusNtyReq >::iterator item = devServiceStatusNotify.begin();
        while( devServiceStatusNotify.end() != item)
        {
            if( Ntf.GetDevURI() == (*item).GetDevURI())
            {
                item->GetServiceStatus().SetPlatRec(Ntf.GetRecordStatus().GetPlatRec());
            }
            item++;
        }

        if( devServiceStatusNotify.end() == item)
        {
            CPuServiceStatusNtyReq cReq;
            cReq.SetSeqNum(Ntf.GetSeqNum());
            cReq.SetDevURI(Ntf.GetDevURI());
            cReq.SetSession(Ntf.GetSession());
            cReq.GetServiceStatus().SetPlatRec(Ntf.GetRecordStatus().GetPlatRec());

			seqMarksSize++;
            seqMarks.push_back(ETYPE_DEVICE_SERVICE_STATUS_NOTIFY);
            devServiceStatusNotify.push_back(cReq);
        }
    }

    void InsertSubNotify(const CPuAlarmStatusNtyReq &Ntf, bool bIsSplitSeqMarks = false)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_ALARM_NOTIFY);

		// 发送到CU的消息需要拆分成多条，seqMarks不能只有1个，且必须保证顺序正确
		if ( bIsSplitSeqMarks )
		{
			int seqNum = Ntf.GetAlarmStatus().GetAlarmArray().size();
			for (int i = 1; i < seqNum; i++)
			{
				seqMarksSize++;
				seqMarks.push_back(ETYPE_DEVICE_ALARM_NOTIFY);
			}
		}

        devAlarmNotify.push_back(Ntf);
    }

    void InsertSubNotify(const CTvWallStatusNtyReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_TVWALL_STATUS_NOTIFY);
        tvWallStatusNotify.push_back(Ntf);
    }

    void InsertSubNotify(const CPuGpsNtyReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_GPS_NOTIFY);
        devGpsNotify.push_back(Ntf);
    }

    void InsertInviteStopNotify(const CInviteStopNotify &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_INVITE_STOP_NOTIFY);
        inviteStopNotify.push_back(Ntf);
    }

    void InsertUserAddNotify(const CCuUserInfo &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_USER_ADD_NOTIFY);
        userAddNotify.push_back(Ntf);
    }

    void InsertUserDelNotify(const string &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_USER_DEL_NOTIFY);
        userDelNotify.push_back(Ntf);
    }

    void InsertUserModNotify(const CCuUserInfo &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_USER_MOD_NOTIFY);
        userModNotify.push_back(Ntf);
    }

	void InsertTvWallAddNotify(const CTvwallAddNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_TVWALL_ADD_NOTIFY);
		tvWallAddNotify.push_back(Ntf);
	}

    void InsertTvWallDelNotify(const string &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_TVWALL_DEL_NOTIFY);
        tvWallDelNotify.push_back(Ntf);
    }

    void InsertTvWallModNotify(const CTvwallModNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_TVWALL_MOD_NOTIFY);
        tvWallModNotify.push_back(Ntf);
    }

    void InsertUserGroupAddNotify(const CUserGroupAddNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_USER_GROUP_ADD_NOTIFY);
        userGroupAddNotify.push_back(Ntf);
    }

    void InsertUserGroupDelNotify(const string &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_USER_GROUP_DEL_NOTIFY);
        userGroupDelNotify.push_back(Ntf);
    }

    void InsertUserGroupModNotify(const CUserGroupModNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_USER_GROUP_MOD_NOTIFY);
        userGroupModNotify.push_back(Ntf);
    }

    void InsertDevGroupDevAddNotify(const CDeviceGroupDeviceAddNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_GROUP_ADD_DEVICE_NOTIFY);
        devGroupDevAddNotify.push_back(Ntf);
    }

    void InsertDevGroupDevDelNotify(const CDeviceGroupDeviceDelNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_DEVICE_GROUP_DEL_DEVICE_NOTIFY);
        devGroupDevDelNotify.push_back(Ntf);
    }

    void InsertAlarmLinkCfgModNotify(const CAlarmLinkModNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_ALARMLINK_CONFIG_CHANGE_NOTIFY);
        alarmLinkModNotify.push_back(Ntf);
    }

    void InsertExportFailNotify(const CCuExportFailNotify &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_EXPORT_FAIL_NOTIFY);
        exportFailNotify.push_back(Ntf);
    }

    void InsertStunUpdateNotify(const CStunUpdateReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_STUN_UPDATE_NOTIFY);
        stunUpdateNotify.push_back(Ntf);
    }

    void InsertServiceStatusNotify(const TServiceStatus &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
        seqMarks.push_back(ETYPE_SERVICE_STATUS_NOTIFY);
        serviceStatusNotify.push_back(Ntf);
    }

	void InsertMapAddNotify(const CMapAddNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_ADD_NOTIFY);
		mapAddNotify.push_back(Ntf);
	}

	void InsertMapModNotify(const CMapModNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_MOD_NOTIFY);
		mapModNotify.push_back(Ntf);
	}

	void InsertMapDelNotify(const CMapDelNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_DEL_NOTIFY);
		mapDelNotify.push_back(Ntf);
	}

	void InsertMapLayerAddNotify(const CMapLayerAddNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_LAYER_ADD_NOTIFY);
		mapLayerAddNotify.push_back(Ntf);
	}

	void InsertMapLayerModNotify(const CMapLayerModNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_LAYER_MOD_NOTIFY);
		mapLayerModNotify.push_back(Ntf);
	}

	void InsertMapLayerDelNotify(const CMapLayerDelNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_LAYER_DEL_NOTIFY);
		mapLayerDelNotify.push_back(Ntf);
	}

	void InsertMapAreaAddNotify(const CMapAreaAddNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_AREA_ADD_NOTIFY);
		mapAreaAddNotify.push_back(Ntf);
	}

	void InsertMapAreaModNotify(const CMapAreaModNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_AREA_MOD_NOTIFY);
		mapAreaModNotify.push_back(Ntf);
	}

	void InsertMapAreaDelNotify(const CMapAreaDelNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_AREA_DEL_NOTIFY);
		mapAreaDelNotify.push_back(Ntf);
	}

	void InsertMapElementAddNotify(const CMapElementAddNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_ELEMENT_ADD_NOTIFY);
		mapElementAddNotify.push_back(Ntf);
	}

	void InsertMapElementModNotify(const CMapElementModNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_ELEMENT_MOD_NOTIFY);
		mapElementModNotify.push_back(Ntf);
	}

	void InsertMapElementDelNotify(const CMapElementDelNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_MAP_ELEMENT_DEL_NOTIFY);
		mapElementDelNotify.push_back(Ntf);
	}

	void InsertTransparentQurNotify(const CCuPuTransparentQryNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_TRANSPARENT_QUR_NOTIFY);
		mapTransparentNotify.push_back(Ntf);
	}

	void InsertGbsGbPDCLProgressNotify(const CGbsGbPDCLProgressNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_GBS_PDCL_PROGRESS_NOTIFY);
		gbGbsGbPDCLProgressNotify.push_back(Ntf);
	}

	void InsertAlarmEventNotify(const CAlarmEventNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return ;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_ALARM_EVENT_NOTIFY);
		alarmEventNotify.push_back(Ntf);
	}

	void InsertStartEncoderTransNotify(const CCuStartEncoderTransNtfReq &Ntf)
	{
		if ( IsCuNtfOverFlow() )
		{
			return;
		}

		seqMarksSize++;
		seqMarks.push_back(ETYPE_START_ENCODER_TRANS_NOTIFY);
		startEncoderTransNotify.push_back(Ntf);
	}
    /*====================================================================
    函数名      : PopNotifyPool
    功能        : 提取部分或者全部Notify到一个新的NotifyPool
                  提取之后，本身Pool会自动删除这些Notify
    算法实现    :
    引用全局变量:
    输入参数说明:
    返回值说明  : 成功:true，失败:false
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/3/23/             liangli
    ====================================================================*/
    bool PopNotifyPool(CCuNotifyPool &cOutPool, u32 dwMaxNum)
    {
        //将消息池中的Ntf拷贝到新消息中
        /*
        算法：
        1，所有的Ntf消息都有序号，用枚举值按顺序存放在seqMarks里
        2，按照m_cNotifyPool的seqMarks顺序依次取出一个Notify，填入cRsp
        3，每次提取的Notify个数不能超过20个，如果不足20个就提取实际个数
        */
        //cOutPool = *this;
        u32 dwNtfNum = 0;
        while (dwNtfNum<dwMaxNum && (!IsCuNtfEmpty()/*IsEmpty()*/))
        {
            switch (seqMarks.front())
            {
            case ETYPE_DEVICE_ADD_NOTIFY:
                {
					cOutPool.InsertDeviceAddNotify(deviceAddNotify.front());
					deviceAddNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_DEL_NOTIFY:
                {
                    cOutPool.InsertDeviceDelNotify(deviceDelNotify.front());
                    deviceDelNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_MOD_NOTIFY:
                {
                    CMS_ASSERT(0&&"没有 ETYPE_DEVICE_MOD_NOTIFY 通知消息");
                }
                break;
            case ETYPE_VIDEOSRCALIAS_CHANGE_NOTIFY:
                {
                    cOutPool.InsertCapChangeNotify(CapChangeNotify.front(), true);
                    CapChangeNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_GROUP_ADD_NOTIFY:
                {
                    cOutPool.InsertDeviceGroupAddNotify(deviceGroupAddNotify.front());
                    deviceGroupAddNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_GROUP_DEL_NOTIFY:
                {
                    cOutPool.InsertDeviceGroupDelNotify(deviceGroupDelNotify.front());
                    deviceGroupDelNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_GROUP_MOD_NOTIFY:
                {
                    cOutPool.InsertDeviceGroupModNotify(deviceGroupModNotify.front());
                    deviceGroupModNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_ONLINE_NOTIFY:
                {
                    cOutPool.InsertSubNotify(devOnlineNotify.front());
                    devOnlineNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_CONFIG_NOTIFY:
                {
                    cOutPool.InsertSubNotify(devConfigNotify.front());
                    devConfigNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_SERVICE_STATUS_NOTIFY:
                {
                    cOutPool.InsertSubNotify(devServiceStatusNotify.front());
                    devServiceStatusNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_ALARM_NOTIFY:
                {
                    cOutPool.InsertSubNotify(devAlarmNotify.front(), true);
                    devAlarmNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_TRANSDATA_NOTIFY:
                {
                    cOutPool.InsertSubNotify(devTransdataNotify.front());
                    devTransdataNotify.pop_front();
                }
                break;
            case ETYPE_INVITE_STOP_NOTIFY:
                {
                    cOutPool.InsertInviteStopNotify(inviteStopNotify.front());
                    inviteStopNotify.pop_front();
                }
                break;
            case ETYPE_TVWALL_STATUS_NOTIFY:
                {
                    cOutPool.InsertSubNotify(tvWallStatusNotify.front());
                    tvWallStatusNotify.pop_front();
                }
                break;
            case ETYPE_GPS_NOTIFY:
                {
                    cOutPool.InsertSubNotify(devGpsNotify.front());
                    devGpsNotify.pop_front();
                }
                break;
            case ETYPE_USER_ADD_NOTIFY:
                {
                    cOutPool.InsertUserAddNotify(userAddNotify.front());
                    userAddNotify.pop_front();
                }
                break;
            case ETYPE_USER_DEL_NOTIFY:
                {
                    cOutPool.InsertUserDelNotify(userDelNotify.front());
                    userDelNotify.pop_front();
                }
                break;
            case ETYPE_USER_MOD_NOTIFY:
                {
                    cOutPool.InsertUserModNotify(userModNotify.front());
                    userModNotify.pop_front();
                }
                break;
            case ETYPE_USER_GROUP_ADD_NOTIFY:
                {
                    cOutPool.InsertUserGroupAddNotify(userGroupAddNotify.front());
                    userGroupAddNotify.pop_front();
                }
                break;
            case ETYPE_USER_GROUP_DEL_NOTIFY:
                {
                    cOutPool.InsertUserGroupDelNotify(userGroupDelNotify.front());
                    userGroupDelNotify.pop_front();
                }
                break;
            case ETYPE_USER_GROUP_MOD_NOTIFY:
                {
                    cOutPool.InsertUserGroupModNotify(userGroupModNotify.front());
                    userGroupModNotify.pop_front();
                }
                break;
            case ETYPE_TVWALL_ADD_NOTIFY:
                {
                    cOutPool.InsertTvWallAddNotify(tvWallAddNotify.front());
                    tvWallAddNotify.pop_front();
                }
                break;
            case ETYPE_TVWALL_DEL_NOTIFY:
                {
                    cOutPool.InsertTvWallDelNotify(tvWallDelNotify.front());
                    tvWallDelNotify.pop_front();
                }
                break;
            case ETYPE_TVWALL_MOD_NOTIFY:
                {
                    cOutPool.InsertTvWallModNotify(tvWallModNotify.front());
                    tvWallModNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_GROUP_ADD_DEVICE_NOTIFY:
                {
                    cOutPool.InsertDevGroupDevAddNotify(devGroupDevAddNotify.front());
                    devGroupDevAddNotify.pop_front();
                }
                break;
            case ETYPE_DEVICE_GROUP_DEL_DEVICE_NOTIFY: 
                {
                    cOutPool.InsertDevGroupDevDelNotify(devGroupDevDelNotify.front());
                    devGroupDevDelNotify.pop_front();
                }
                break;
            case ETYPE_ALARMLINK_CONFIG_CHANGE_NOTIFY:
                {
                    cOutPool.InsertAlarmLinkCfgModNotify(alarmLinkModNotify.front());
                    alarmLinkModNotify.pop_front();
                }
                break;
            case ETYPE_EXPORT_FAIL_NOTIFY:
                {
                    cOutPool.InsertExportFailNotify(exportFailNotify.front());
                    exportFailNotify.pop_front();
                }
                break;
            case ETYPE_STUN_UPDATE_NOTIFY:
                {
                    cOutPool.InsertStunUpdateNotify(stunUpdateNotify.front());
                    stunUpdateNotify.pop_front();
                }
                break;
            case ETYPE_SERVICE_STATUS_NOTIFY:
                {
                    cOutPool.InsertServiceStatusNotify(serviceStatusNotify.front());
                    serviceStatusNotify.pop_front();
                }
                break;
			case ETYPE_MAP_ADD_NOTIFY:
				{
					cOutPool.InsertMapAddNotify(mapAddNotify.front());
					mapAddNotify.pop_front();
				}
				break;
			case ETYPE_MAP_MOD_NOTIFY:
				{
					cOutPool.InsertMapModNotify(mapModNotify.front());
					mapModNotify.pop_front();
				}
				break;
			case ETYPE_MAP_DEL_NOTIFY:
				{
					cOutPool.InsertMapDelNotify(mapDelNotify.front());
					mapDelNotify.pop_front();
				}
				break;
			case ETYPE_MAP_LAYER_ADD_NOTIFY:
				{
					cOutPool.InsertMapLayerAddNotify(mapLayerAddNotify.front());
					mapLayerAddNotify.pop_front();
				}
				break;
			case ETYPE_MAP_LAYER_MOD_NOTIFY:
				{
					cOutPool.InsertMapLayerModNotify(mapLayerModNotify.front());
					mapLayerModNotify.pop_front();
				}
				break;
			case ETYPE_MAP_LAYER_DEL_NOTIFY:
				{
					cOutPool.InsertMapLayerDelNotify(mapLayerDelNotify.front());
					mapLayerDelNotify.pop_front();
				}
				break;
			case ETYPE_MAP_AREA_ADD_NOTIFY:
				{
					cOutPool.InsertMapAreaAddNotify(mapAreaAddNotify.front());
					mapAreaAddNotify.pop_front();
				}
				break;
			case ETYPE_MAP_AREA_MOD_NOTIFY:
				{
					cOutPool.InsertMapAreaModNotify(mapAreaModNotify.front());
					mapAreaModNotify.pop_front();
				}
				break;
			case ETYPE_MAP_AREA_DEL_NOTIFY:
				{
					cOutPool.InsertMapAreaDelNotify(mapAreaDelNotify.front());
					mapAreaDelNotify.pop_front();
				}
				break;
			case ETYPE_MAP_ELEMENT_ADD_NOTIFY:
				{
					cOutPool.InsertMapElementAddNotify(mapElementAddNotify.front());
					mapElementAddNotify.pop_front();
				}
				break;
			case ETYPE_MAP_ELEMENT_MOD_NOTIFY:
				{
					cOutPool.InsertMapElementModNotify(mapElementModNotify.front());
					mapElementModNotify.pop_front();
				}
				break;
			case ETYPE_MAP_ELEMENT_DEL_NOTIFY:
				{
					cOutPool.InsertMapElementDelNotify(mapElementDelNotify.front());
					mapElementDelNotify.pop_front();
				}
				break;
			case ETYPE_TRANSPARENT_QUR_NOTIFY:
				{
					cOutPool.InsertTransparentQurNotify(mapTransparentNotify.front());
					mapTransparentNotify.pop_front();
				}
				break;
			case ETYPE_GBS_PDCL_PROGRESS_NOTIFY:
				{
					cOutPool.InsertGbsGbPDCLProgressNotify(gbGbsGbPDCLProgressNotify.front());
					gbGbsGbPDCLProgressNotify.pop_front();
				}
				break;
			case ETYPE_ALARM_EVENT_NOTIFY:
				{
					cOutPool.InsertAlarmEventNotify(alarmEventNotify.front());
					alarmEventNotify.pop_front();
				}
				break;
			case ETYPE_START_ENCODER_TRANS_NOTIFY:
				{
					cOutPool.InsertStartEncoderTransNotify(startEncoderTransNotify.front());
					startEncoderTransNotify.pop_front();
				}
				break;
            default:
                {
                   // CMS_ASSERT(0&&"[CCuAgent::CuHBAndGetNotify]未知通知消息类型");
                }
                break;
            }

			seqMarksSize--;
            seqMarks.pop_front();
            dwNtfNum++;
        }

        if ( ! IsCuNtfEmpty()/*IsEmpty()*/ )
        {
            cOutPool.hasDataLeft = true;
        }
        else
        {
            cOutPool.hasDataLeft = false;
        }
        return true;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CGetNotifyRsp : public CEventRsp
{
public:
    CGetNotifyRsp()
    {
        event = OspExtEventDesc(CUI_CU_GET_NOTIFY_RSP);
    }
    void Clear()
    {
        CEventRsp::Clear();
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    CCuNotifyPool notifyPool;
};

/*
    CU的查询结果是在QueryTask中收集完毕之后一次性发送给cui_fcgi的
    所以查询结果比如单独定义，不能继承复用TAS的NtfReq
    独立定义一套自己的查询基类，用于规范CU查询过程
*/
class CCUQueryResultBase: public CEventRsp
{
public:
    CCUQueryResultBase()
    {
        totalEntryNum = 0;
        actualResultNum = 0;
		allVidSrcNum = 0;
		onlineVidSrcNum = 0;
    }
    int GetTotalEntryNum() const
    {
        return totalEntryNum;
    }
    void SetTotalEntryNum(const int nNum)
    {
        totalEntryNum = nNum;
    }
    int GetActualResultNum() const
    {
        return actualResultNum;
    }
    void SetActualResultNum(const int nNum)
    {
        actualResultNum = nNum;
    }
	void SetVidSrcNum(const int &nVidSrcNum)
	{
		allVidSrcNum = nVidSrcNum;
	}
	const int GetVidSrcNum() const
	{
		return allVidSrcNum;
	}
	int GetVidSrcNum()
	{
		return allVidSrcNum;
	}
	void SetOnlineVidSrcNum(const int &nOnlineVidSrcNum)
	{
		onlineVidSrcNum = nOnlineVidSrcNum;
	}
	const int GetOnlineVidSrcNum() const
	{
		return onlineVidSrcNum;
	}
	int GetOnlineVidSrcNum()
	{
		return onlineVidSrcNum;
	}
private:
    //例如查询Nanjing域的设备, 满足条件的有1000个，每页有20条，每次Ntf发送2条
    int totalEntryNum;      // 符合本次查询条件的记录总数，当BeginIndex = 0时totalEntryNum = 1000，否则无效
    int actualResultNum;    // 本次返回的记录数，每次都是20，一页的数目，除非接收超时，提前终止
	int allVidSrcNum;       // 结果总数.
	int onlineVidSrcNum;	// 在线视频源数目.
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string LongQueryHandle;
class CCULongQueryResultBase: public CCUQueryResultBase
{
public:
    CCULongQueryResultBase()
    {
        hasLeft = false;
    }
private:
    LongQueryHandle queryHandle;                
    bool hasLeft;                
public:
    void SetQueryHandle(const LongQueryHandle& strQueryHandle)
    {
        queryHandle = strQueryHandle;
    }
    LongQueryHandle& GetQueryHandle()
    {
        return queryHandle;
    }
    const LongQueryHandle& GetQueryHandle() const
    {
        return queryHandle;
    }

    void SetHasLeft(bool bHasLeft)
    {
        hasLeft = bHasLeft;
    }
    bool GetHasLeft() const
    {
        return hasLeft;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CQueryTask;

typedef CUserQryReq CCuQueryUserReq;

class CCuQueryUserRsp : public CCUQueryResultBase
{
public:
    CCuQueryUserRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_QRY_RSP);
    }
    vector<CCuUserInfo>& GetUserInfo() 
    {
        return infos;
    }
    const vector<CCuUserInfo>& GetUserInfo() const
    {
        return infos;
    }
    void SetUserInfo(const vector<CCuUserInfo> &val)
    {
        infos = val;
    }
    void push_back(const CUserQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
    vector<CCuUserInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
//由于WebService是应答式，没有NTF，所以数据都是在Rsp中回带

typedef CPuTransparentOperateReq CCuPuTransparentOperateReq;
typedef CPuTransparentOperateRsp CCuPuTransparentOperateRsp;

typedef CPuTransparentQryReq CCuPuTransparentQryReq;
typedef CPuTransparentQryRsp CCuPuTransparentQryRsp;

typedef CUserGroupQryReq CCuUserGroupQryReq;
class CCuUserGroupQryRsp : public CCUQueryResultBase
{
public:
    CCuUserGroupQryRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_QRY_RSP);
    }
    vector<CUserGroupInfo>& GetUserGroupInfo()
    {
        return infos;
    }
    const vector<CUserGroupInfo>& GetUserGroupInfo() const
    {
        return infos;
    }
    void SetUserGroupInfo(const vector<CUserGroupInfo> &val)
    {
        infos = val;
    }
    void push_back(CUserGroupQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
    vector<CUserGroupInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuUserDeviceGroupInfo : public CDeviceGroupInfo
{
public:
    CCuUserDeviceGroupInfo()
    {
        groupHasDev = true;
		emptyDevGroup = false;
    }
    bool IsGroupHasDev() const
    {
        return groupHasDev;
    }
    void SetGroupHasDev(const bool bHas)
    {
        groupHasDev = bHas;
	}
	bool IsEmptyDevGroup() const
	{
		return emptyDevGroup;
	}
	void SetEmptyDevGroup(const bool bEmpty)
	{
		emptyDevGroup = bEmpty;
	}
    CCuUserDeviceGroupInfo &operator =(const CDeviceGroupInfo& val)
    {
        *(CDeviceGroupInfo*)this = val;
        return (*this);
    }
private:
    bool groupHasDev;   //该用户的设备组下是否含有设备，也就是说用户的该组是否为空组

	bool emptyDevGroup; // 递归查询下级分组是否没有设备(默认有设备) 数据由DPS提供
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CDeviceGroupQryReq CCuUserDevGroupQryReq;
class CCuUserDevGroupQryRsp : public CCUQueryResultBase
{
public:
    CCuUserDevGroupQryRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_QRY_RSP);
    }
    vector<CCuUserDeviceGroupInfo> &GetUserDevGroupInfo()
    {
        return infos;
    }
    const vector<CCuUserDeviceGroupInfo> &GetUserDevGroupInfo() const
    {
        return infos;
    }
    void SetUserDevGroupInfo(const vector<CCuUserDeviceGroupInfo> &val)
    {
        infos = val;
    }
    void push_back(const CDeviceGroupQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
    vector<CCuUserDeviceGroupInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CCuserDevicefullinfoAndPrivilegeQryReq CCuUserDevFullInfoQryReq;
class CCuUserDevFullInfoQryRsp : public CCUQueryResultBase
{
public:
    CCuUserDevFullInfoQryRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP);
    }
	void SetQueryHint(const string& strQueryHint)
	{
		queryHint = strQueryHint;
	}
	string& GetQueryHint()
	{
		return queryHint;
	}
	const string& GetQueryHint() const
	{
		return queryHint;
	}
    vector<TDevicefullinfoAndPrivilege> &GetUserDevFullInfo()
    {
        return infos;
    }
    const vector<TDevicefullinfoAndPrivilege> &GetUserDevFullInfo() const
    {
        return infos;
    }
    void SetUserDevFullInfo(const vector<TDevicefullinfoAndPrivilege> &val)
    {
        infos = val;
    }
    void push_back(CCuserDevicefullinfoAndPrivilegeQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
	string                              queryHint;
    vector<TDevicefullinfoAndPrivilege> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//////////////////////////////////////////////////////////////////////////
//订阅
enum EPuSubscribeType
{
    PuSubscribeTypeInvalid = 0,
    SubscribeOnlineStatus,
    SubscribeConfigStatus,
    SubscribeServiceStatus,
    SubscribeAlarmStatus,
    SubscribeGPS,
    SubscribeTransdata
};

enum ESubNotifyType
{  
    SubNotifyTypeInvalid = 0,
    SubscribePuStatus,
    SubscribeTVWallStatus,
    SubscribeNRUStatus,
 //   SubscribeAlarmLinkConfig,
    SubscribeSeviceStatus
};

class CNotifySubscription
{
public:
    CNotifySubscription()
    {
    }
    void Clear()
    {
        devURI.clear();
        subscribeTypes.clear();
    }
    string &GetDeviceURI()
    {
        return devURI;
    }
    const string &GetDeviceURI() const
    {
        return devURI;
    }
    void SetDeviceURI(const string &val)
    {
        devURI = val;
    }
    const int GetSubscribeNotifyType() const
    {
        return subscribeNotifyType;
    }
    int GetSubscribeNotifyType()
    {
        return subscribeNotifyType;
    }
    void SetSubscribeNotifyType(int dwSubscribeNotifyType)
    {
        subscribeNotifyType = dwSubscribeNotifyType;
    }
    set<int> & GetSubscribeType()
    {
        return subscribeTypes;
    };
    const set<int> & GetSubscribeType() const
    {
        return subscribeTypes;
    };
    void SetSubscribeType(const set<int> &val)
    {
        subscribeTypes = val;
    };
    void SetAllSubscribeType()
    {
        subscribeTypes.clear();
        subscribeTypes.insert(SubscribeOnlineStatus);
        subscribeTypes.insert(SubscribeConfigStatus);
        subscribeTypes.insert(SubscribeServiceStatus);
        subscribeTypes.insert(SubscribeAlarmStatus);
    }
    string GetSubscribeTypesString()
    {
        string strSubscribeType;
        if ( subscribeTypes.count(SubscribeOnlineStatus) > 0 )
        {
            if ( ! strSubscribeType.empty() )
            {
                strSubscribeType += "/";
            }
            strSubscribeType += "Online";
        }
        if ( subscribeTypes.count(SubscribeConfigStatus) > 0 )
        {
            if ( ! strSubscribeType.empty() )
            {
                strSubscribeType += "/";
            }
            strSubscribeType += "Config";
        }
        if ( subscribeTypes.count(SubscribeServiceStatus) > 0 )
        {
            if ( ! strSubscribeType.empty() )
            {
                strSubscribeType += "/";
            }
            strSubscribeType += "Service";
        }
        if ( subscribeTypes.count(SubscribeAlarmStatus) > 0 )
        {
            if ( ! strSubscribeType.empty() )
            {
                strSubscribeType += "/";
            }
            strSubscribeType += "Alarm";
        }
        return strSubscribeType;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string devURI;
    int subscribeNotifyType;
    set<int> subscribeTypes;
};

class CCuSubscribeNotifyReq : public CEventReq
{
public:
    CCuSubscribeNotifyReq()
    {
        event = OspExtEventDesc(CU_CUI_SUBSCRIBE_PUSTATUS_REQ);
    }
    void Clear()
    {
        CEventReq::Clear();
    }
    vector<CNotifySubscription> &GetSubscribeList()
    {
        return subscriptionList;
    }
    const vector<CNotifySubscription> &GetSubscribeList() const
    {
        return subscriptionList;
    }
    void SetSubscribeList(const vector<CNotifySubscription> &val)
    {
        subscriptionList = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<CNotifySubscription> subscriptionList;
};

class CCuSubscribeNotifyRsp : public CEventRsp
{
public:
    CCuSubscribeNotifyRsp()
    {
        event = OspExtEventDesc(CUI_CU_SUBSCRIBE_PUSTATUS_RSP);
    }
    void Clear()
    {
        CEventRsp::Clear();
    }
    vector<CNotifySubscription> &GetFailedSubscribeList()
    {
        return failedList;
    }
    const vector<CNotifySubscription> &GetFailedSubscribeList() const
    {
        return failedList;
    }
    void SetFailedSubscribeList(const vector<CNotifySubscription> &val)
    {
        failedList = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<CNotifySubscription> failedList;
};

class CCuUnSubscribeNotifyReq : public CEventReq
{
public:
    CCuUnSubscribeNotifyReq()
    {
        event = OspExtEventDesc(CU_CUI_UNSUBSCRIBE_PUSTATUS_REQ);
    }
    void Clear()
    {
        CEventReq::Clear();
    }
    vector<CNotifySubscription> &GetUnSubscribeList()
    {
        return unsubscriptionList;
    }
    const vector<CNotifySubscription> &GetUnSubscribeList() const
    {
        return unsubscriptionList;
    }
    void SetUnSubscribeList(const vector<CNotifySubscription> &val)
    {
        unsubscriptionList = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<CNotifySubscription> unsubscriptionList;
};

class CCuUnSubscribeNotifyRsp : public CEventRsp
{
public:
    CCuUnSubscribeNotifyRsp()
    {
        event = OspExtEventDesc(CUI_CU_UNSUBSCRIBE_PUSTATUS_RSP);
    }
    void Clear()
    {
        CEventRsp::Clear();
    }
    vector<CNotifySubscription> &GetFailedUnSubscribeList()
    {
        return failedList;
    }
    const vector<CNotifySubscription> &GetFailedUnSubscribeList() const
    {
        return failedList;
    }
    void SetFailedUnSubscribeList(const vector<CNotifySubscription> &val)
    {
        failedList = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<CNotifySubscription> failedList;
};

typedef CDeviceGroupDeviceQryReq CCuQryDevGroupOfDevInfoReq;
class CCuQryDevGroupOfDevInfoRsp : public CCUQueryResultBase
{
public:
    CCuQryDevGroupOfDevInfoRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_QRY_RSP);
    }
    vector<CDeviceGroupDeviceInfo> &GetDeviceGroupDeviceInfo()
    {
        return infos;
    }
    const vector<CDeviceGroupDeviceInfo> &GetDeviceGroupDeviceInfo() const
    {
        return infos;
    }
    void SetDeviceGroupDeviceInfo(const vector<CDeviceGroupDeviceInfo> &val)
    {
        infos = val;
    }
    void push_back(CDeviceGroupDeviceQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
    vector<CDeviceGroupDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CDeviceGroupAddReq CCuCreateDeviceGroupReq;
typedef CDeviceGroupAddRsp CCuCreateDeviceGroupRsp;

typedef CDeviceGroupDelReq CCuDeleteDeviceGroupReq;
typedef CDeviceGroupDelRsp CCuDeleteDeviceGroupRsp;

typedef CDeviceGroupModReq CCuModifyDeviceGroupReq;
typedef CDeviceGroupModRsp CCuModifyDeviceGroupRsp;

typedef CDeviceGroupMod4GbReq CCuModifyGBDeviceGroupReq;
typedef CDeviceGroupMod4GbRsp CCuModifyGBDeviceGroupRsp;

typedef CDeviceGroupDeviceOrderReq CCuAdjustVideoSrcOrderReq;
typedef CDeviceGroupDeviceOrderRsp CCuAdjustVideoSrcOrderRsp;

typedef CGetAlarmTypesReq CCuGetAlarmTypesReq;
typedef CGetAlarmTypesRsp CCuGetAlarmTypesRsp;

class CCuDeviceGroupAddDeviceReq : public CEventReq
{
public:
    CCuDeviceGroupAddDeviceReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ADD_REQ);
    }
    vector<CDeviceGroupDeviceInfo> &GetDeviceGroupDeviceInfo()
    {
        return infos;
    }
    const vector<CDeviceGroupDeviceInfo> &GetDeviceGroupDeviceInfo() const
    {
        return infos;
    }
    void SetDeviceGroupDeviceInfo(const vector<CDeviceGroupDeviceInfo> &val)
    {
        infos = val;
    }

	const string &GetSrcDevTreeId()const
	{
		return srcDevTreeId;
	}
	string &GetSrcDevTreeId()
	{
		return srcDevTreeId;
	}
	void SetSrcDevTreeId(const string &strSrcDevTreeId)
	{
		srcDevTreeId = strSrcDevTreeId;
	}
	
	const string &GetDstDevTreeId()const
	{
		return dstDevTreeId;
	}
	string &GetDstDevTreeId()
	{
		return dstDevTreeId;
	}
	void SetDstDevTreeId(const string &strDstDevTreeId)
	{
		dstDevTreeId = strDstDevTreeId;
	}
private:
    vector<CDeviceGroupDeviceInfo> infos;

	string srcDevTreeId; //标识添加之前，视频源所属的设备树Id
	string dstDevTreeId; //标识添加之后，视频源所属的设备树Id
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CCuDeviceGroupAddDeviceRsp : public CEventRsp
{
public:
    CCuDeviceGroupAddDeviceRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_ADD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuDeviceGroupDelDeviceReq : public CEventReq
{
public:
    CCuDeviceGroupDelDeviceReq()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_DEL_REQ);
    }
    vector<CDeviceGroupDeviceInfo> &GetDeviceGroupDeviceInfo()
    {
        return infos;
    }
    const vector<CDeviceGroupDeviceInfo> &GetDeviceGroupDeviceInfo() const
    {
        return infos;
    }
    void SetDeviceGroupDeviceInfo(const vector<CDeviceGroupDeviceInfo> &val)
    {
        infos = val;
    }
	const string &GetDevTreeId()const
	{
		return devTreeId;
	}
	string &GetDevTreeId()
	{
		return devTreeId;
	}
	void SetDevTreeId(const string &strDevTreeId)
	{
		devTreeId = strDevTreeId;
	}
private:
    vector<CDeviceGroupDeviceInfo> infos;

	string devTreeId; //标识添加之前，视频源所属的设备树Id
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CCuDeviceGroupDelDeviceRsp : public CEventRsp
{
public:
    CCuDeviceGroupDelDeviceRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_DEL_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CDevTreeDevGrpAssignReq CCuDevTreeDevGrpAssignReq;
typedef CDevTreeDevGrpAssignRsp CCuDevTreeDevGrpAssignRsp;

typedef CUserGroupAddReq CCuCreateUserGroupReq;
typedef CUserGroupAddRsp CCuCreateUserGroupRsp;

typedef CUserGroupDelReq CCuDeleteUserGroupReq;
typedef CUserGroupDelRsp CCuDeleteUserGroupRsp;

typedef CUserGroupModReq CCuModifyUserGroupReq;
typedef CUserGroupModRsp CCuModifyUserGroupRsp;

typedef CUserAddReq CCuCreateUserReq;
typedef CUserAddRsp CCuCreateUserRsp;

typedef CUserDelReq CCuDeleteUserReq;
typedef CUserDelRsp CCuDeleteUserRsp;

typedef CUserModReq CCuModifyUserReq;
typedef CUserModRsp CCuModifyUserRsp;

typedef CUserDeviceQryReq CCuGetUserDeviceReq;
class CCuGetUserDeviceRsp : public CCUQueryResultBase
{
public:
    CCuGetUserDeviceRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_QRY_RSP);
    }
    vector<CUserDeviceInfo>& GetCuserDevInfo() 
    {
        return infos;
    }
    const vector<CUserDeviceInfo>& GetCuserDevInfo() const
    {
        return infos;
    }
    void SetCuserDevInfo(const vector<CUserDeviceInfo> &val)
    {
        infos = val;
    }
    void push_back(CUserDeviceQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
    vector<CUserDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CUserGroupDeviceQryReq CCuGetUserGroupDeviceReq;
class CCuGetUserGroupDeviceRsp : public CCUQueryResultBase
{
public:
    CCuGetUserGroupDeviceRsp()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_QRY_RSP);
    }
    void push_back(CUserGroupDeviceQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
    vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfo() 
    {
        return infos;
    }
    const vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfo() const
    {
        return infos;
    }
    void SetUserGroupDeviceInfo(const vector<CUserGroupDeviceInfo> &val)
    {
        infos = val;
    }
private:
    vector<CUserGroupDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuUserAddDeviceReq : public CEventReq
{
public:
    CCuUserAddDeviceReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_ADD_REQ);
    }
    vector<CUserDeviceInfo>& GetUserDeviceInfoList() 
    {
        return infos;
    }
    const vector<CUserDeviceInfo>& GetUserDeviceInfoList() const
    {
        return infos;
    }
    void SetUserDeviceInfoList(const vector<CUserDeviceInfo> &val)
    {
        infos = val;
    }

private:
    vector<CUserDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CUserDeviceAddRsp CCuUserAddDeviceRsp;

class CCuUserDelDeviceReq : public CEventReq
{
public:
    CCuUserDelDeviceReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_DEL_REQ);
    }
    vector<CUserDeviceInfo>& GetUserDeviceInfoList() 
    {
        return infos;
    }
    const vector<CUserDeviceInfo>& GetUserDeviceInfoList() const
    {
        return infos;
    }
    void SetUserDeviceInfoList(const vector<CUserDeviceInfo> &val)
    {
        infos = val;
    }

private:
    // 具体能力信息为空时表示删除整个设备，另外若TAS发现所有的能力信息都已经被删除时也会自动删除整个设备
    vector<CUserDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CUserDeviceDelRsp CCuUserDelDeviceRsp;

class CCuUserModDeviceReq : public CEventReq
{
public:
    CCuUserModDeviceReq()
    {
        event = OspExtEventDesc(TAS_CUSER_DEVICE_MOD_REQ);
    }
    vector<CUserDeviceInfo>& GetUserDeviceInfoList() 
    {
        return infos;
    }
    const vector<CUserDeviceInfo>& GetUserDeviceInfoList() const
    {
        return infos;
    }
    void SetUserDeviceInfoList(const vector<CUserDeviceInfo> &val)
    {
        infos = val;
    }

private:
    vector<CUserDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CUserDeviceModRsp CCuUserModDeviceRsp;

class CCuUserGroupAddDeviceReq : public CEventReq
{
public:
    CCuUserGroupAddDeviceReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_ADD_REQ);
    }
    vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfoList() 
    {
        return infos;
    }
    const vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfoList() const
    {
        return infos;
    }
    void SetUserGroupDeviceInfoList(const vector<CUserGroupDeviceInfo> &val)
    {
        infos = val;
    }
private:
    vector<CUserGroupDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CUserGroupDeviceAddRsp CCuUserGroupAddDeviceRsp;

class CCuUserGroupDelDeviceReq : public CEventReq
{
public:
    CCuUserGroupDelDeviceReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_DEL_REQ);
    }
    vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfoList() 
    {
        return infos;
    }
    const vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfoList() const
    {
        return infos;
    }
    void SetUserGroupDeviceInfoList(const vector<CUserGroupDeviceInfo> &val)
    {
        infos = val;
    }
private:
    vector<CUserGroupDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CUserGroupDeviceDelRsp CCuUserGroupDelDeviceRsp;

class CCuUserGroupModDeviceReq : public CEventReq
{
public:
    CCuUserGroupModDeviceReq()
    {
        event = OspExtEventDesc(TAS_CUSER_GROUP_DEVICE_MOD_REQ);
    }
    vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfoList() 
    {
        return infos;
    }
    const vector<CUserGroupDeviceInfo>& GetUserGroupDeviceInfoList() const
    {
        return infos;
    }
    void SetUserGroupDeviceInfoList(const vector<CUserGroupDeviceInfo> &val)
    {
        infos = val;
    }
private:
    vector<CUserGroupDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CUserGroupDeviceModRsp CCuUserGroupModDeviceRsp;

typedef CTempAuthorizationQryReq CCuGetAuthorizationDeviceReq;
class CCuGetAuthorizationDeviceRsp : public CCUQueryResultBase
{
public:
    CCuGetAuthorizationDeviceRsp()
    {
        event = OspExtEventDesc(TAS_TEMP_AUTHORIZATION_QRY_RSP);
    }
    vector<CTempAuthorizationInfo>& GetTempAuthorizationInfo()
    {
        return infos;
    }
    const vector<CTempAuthorizationInfo>& GetTempAuthorizationInfo() const
    {
        return infos;
    }
    void SetTempAuthorizationInfo(const vector<CTempAuthorizationInfo> &val)
    {
        infos = val;
    }
    void push_back(CTempAuthorizationQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
    vector<CTempAuthorizationInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CTempAuthorizationAddReq CCuAddAuthorizationDeviceReq;
typedef CTempAuthorizationAddRsp CCuAddAuthorizationDeviceRsp;

typedef CTempAuthorizationDelReq CCuDelAuthorizationDeviceReq;
typedef CTempAuthorizationDelRsp CCuDelAuthorizationDeviceRsp;

typedef CTempAuthorizationModReq CCuModifyAuthorizationDeviceReq;
typedef CTempAuthorizationModRsp CCuModifyAuthorizationDeviceRsp;

typedef CDeviceQryReq CCuGetDeviceBaseInfoReq;
class CCuGetDeviceBaseInfoRsp : public CCUQueryResultBase
{
public:
    CCuGetDeviceBaseInfoRsp()
    {
        event = OspExtEventDesc(TAS_DEVICE_QRY_RSP);
    }
    vector<CDeviceInfo>& GetDeviceInfo()
    {
        return infos;
    }
    const vector<CDeviceInfo>& GetDeviceInfo() const
    {
        return infos;
    }
    void SetDeviceInfo(const vector<CDeviceInfo> &val)
    {
        infos = val;
    }
    void push_back(CDeviceQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
    vector<CDeviceInfo> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CInviteReq CCuSetupStreamReq;
typedef CInviteRsp CCuSetupStreamRsp;
class CCuPlayStreamReq : public CEventReq
{
public:
    CCuPlayStreamReq()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_ACK_REQ);
    }
    const s32 GetPlayId() const
    {
        return playId;
    }
    s32 GetPlayId()
    {
        return playId;
    }
    void SetPlayId(const s32 nId)
    {
        playId = nId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    s32 playId;
};

typedef CKeyFrameRsp CCuPlayStreamRsp;

// class CCuPlayStreamRsp : public CEventRsp
// {
// public:
//     CCuPlayStreamRsp()
//     {
//         event = OspExtEventDesc(CUI_CU_INVITE_ACK_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };

class CCuStopStreamReq : public CEventReq
{
public:
    CCuStopStreamReq()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_BYE_REQ);
    }
    const s32 GetPlayId() const
    {
        return playId;
    }
    s32 GetPlayId()
    {
        return playId;
    }
    void SetPlayId(const s32 nId)
    {
        playId = nId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    s32 playId;
};

class CCuStopInviteRsp : public CEventRsp
{
public:
    CCuStopInviteRsp()
    {
        event = OspExtEventDesc(CUI_CU_INVITE_BYE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuSetupPlayPlatformRecordReq : public CEventReq
{
public:
    CCuSetupPlayPlatformRecordReq()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ);
    }
    const CInviteReq &GetReq() const
    {
        return cReq;
    }
    CInviteReq &GetReq()
    {
        return cReq;
    }
    void SetReq(const CInviteReq &var)
    {
        cReq = var;
    }
    const string &GetDomainName()const
    {
        return recordDomainName;
    }
    string &GetDomainName()
    {
        return recordDomainName;
    }
    void SetDomainName(const string &var)
    {
        recordDomainName = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    CInviteReq cReq;
    string recordDomainName;
};

class CCuPlayPlatformRecordReq : public CEventReq
{
public:
    CCuPlayPlatformRecordReq()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_REQ);
    }
    const s32 &GetPlayId()const
    {
        return playId;
    }

    s32 &GetPlayId()
    {
        return playId;
    }

    void SetPlayId(const s32 &var)
   {
       playId = var;
   }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    s32 playId;
};

class CCuPlayPlatformRecordRsp : public CEventRsp
{
public:
    CCuPlayPlatformRecordRsp()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CInviteRsp CCuSetupPlayPlatformRecordRsp;

class CCuDownloadPlatformRecordReq : public CEventReq
{
public:
    CCuDownloadPlatformRecordReq()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ);
    }
    const CInviteReq &GetReq() const
    {
        return cReq;
    }
    CInviteReq &GetReq()
    {
        return cReq;
    }
    void SetReq(const CInviteReq &var)
    {
        cReq = var;
    }
    const string &GetDomainName()const
    {
        return recordDomainName;
    }
    string &GetDomainName()
    {
        return recordDomainName;
    }
    void SetDomainName(const string &var)
    {
        recordDomainName = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    CInviteReq cReq;
    string recordDomainName;
};
typedef CInviteRsp CCuDownloadPlatformRecordRsp;

class CCuStartDownloadPlatformRecordReq : public CEventReq
{
public:
    CCuStartDownloadPlatformRecordReq()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_REQ);
    }
    const s32 GetPlayId() const
    {
        return playId;
    }
    s32 GetPlayId()
    {
        return playId;
    }
    void SetPlayId(const s32 nId)
    {
        playId = nId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    s32 playId;
};

typedef CCuPlayPlatformRecordRsp CCuStartDownloadPlatformRecordRsp;
// class CCuStartDownloadPlatformRecordRsp : public CEventRsp
// {
// public:
//     CCuStartDownloadPlatformRecordRsp()
//     {
//         event = OspExtEventDesc(CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_RSP);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };

class CCuStopDownloadPlatformRecordReq : public CEventReq
{
public:
    CCuStopDownloadPlatformRecordReq()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ);
    }
    const s32 GetPlayId() const
    {
        return playId;
    }
    s32 GetPlayId()
    {
        return playId;
    }
    void SetPlayId(const s32 nId)
    {
        playId = nId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    s32 playId;
};

typedef CCuStopInviteRsp CCuStopDownloadPlatformRecordRsp;
// class CCuStopDownloadPlatformRecordRsp : public CEventRsp
// {
//  public:
//      CCuStopDownloadPlatformRecordRsp()
//      {
//          event = OspExtEventDesc(CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_RSP);
//      }
//  public:
//      const std::string ToXml()const;
//      void FromXml(const std::string& xml_string);
//      void ToXml(TiXmlDocument& xmlDoc)const;
//      void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//      void FromXml(const TiXmlNode& xmlNode);
//  };

class CCuStopPlatformRecordReq : public CEventReq
{
public:
    CCuStopPlatformRecordReq()
    {
        event = OspExtEventDesc(CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ);
    }
    const s32 GetPlayId() const
    {
        return playId;
    }
    s32 GetPlayId()
    {
        return playId;
    }
    void SetPlayId(const s32 nId)
    {
        playId = nId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    s32 playId;
};

typedef CCuStopInviteRsp CCuStopPlatformRecordRsp;
//  class CCuStopPlatformRecordRsp : public CEventRsp
//  {
//  public:
//      CCuStopPlatformRecordRsp()
//      {
//          event = OspExtEventDesc(CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_RSP);
//      }
//  public:
//      const std::string ToXml()const;
//      void FromXml(const std::string& xml_string);
//      void ToXml(TiXmlDocument& xmlDoc)const;
//      void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//      void FromXml(const TiXmlNode& xmlNode);
//  };

typedef CRcsGetSysParamReq CCuRcsGetSysParamReq;
typedef CRcsGetSysParamRsp CCuRcsGetSysParamRsp;

typedef CRcsSetSysParamReq CCuRcsSetSysParamReq;
typedef CRcsSetSysParamRsp CCuRcsSetSysParamRsp;

typedef CRcsQueryNruReq CCuRcsQueryNruReq;
class CCuRcsQueryNruRsp : public CCUQueryResultBase
{
public:
    CCuRcsQueryNruRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_NRU_RSP);
    }

    void Clear()
    {
        nruInfos.clear();
    }

public:
    //nruInfo
    const std::vector<TNruInfo>& GetNruInfo() const
    {
        return nruInfos;
    }
    std::vector<TNruInfo>& GetNruInfo()
    {
        return nruInfos;
    }
    void SetNruInfo(const std::vector<TNruInfo>& var)
    {
        nruInfos = var;
    }
    void push_back(CRcsQueryNruNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    std::vector<TNruInfo> nruInfos;
};

typedef CRcsQueryPuChnBindingReq CCuRcsQueryPuChnBindingReq;
class CCuRcsQueryPuChnBindingRsp : public CCUQueryResultBase
{
public:
    CCuRcsQueryPuChnBindingRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_PUCHN_BINDING_RSP);
    }

    void Clear()
    {
        puChnList.clear();
    }
public:
    const TChannelList& GetChnList()const 
    {
        return puChnList;
    }
    TChannelList& GetChnList()
    {
        return puChnList;
    }
    void SetChnList(const TChannelList& var)
    {
        puChnList = var;
    }
    void AddChnList(const TChannelList& var)
    {
        TChannelList::const_iterator item = var.begin();
        while(item != var.end())
        {
            puChnList.push_back(*item);
            item++;
        }
    }
    void push_back(CRcsQueryPuChnBindingNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    TChannelList puChnList;
};

typedef CRcsBindingPuChnReq CCuRcsBindingPuChnReq;
typedef CRcsBindingPuChnRsp CCuRcsBindingPuChnRsp; 

typedef CRcsUnbindingPuChnReq CCuRcsUnbindingPuChnReq;
typedef CRcsUnbindingPuChnRsp CCuRcsUnbindingPuChnRsp;

//typedef CRcsQueryRecordDaysReq CCuRcsQueryRecordDaysReq;
class CCuRcsQueryRecordDaysReq : public CEventReq
{
public:
    CCuRcsQueryRecordDaysReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_DAYS_REQ);
    }
public:
    const string& GetReq()const
    {
        return req;
    }
    string& GetReq()
    {
        return req;
    }
    void SetReq(const string& tReq)
    {
        req = tReq;
    }
    const string& GetDomainName()const
    {
        return recordDomainName;
    }
    string& GetDomainName()
    {
        return recordDomainName;
    }
    void SetRecordDomainName(const string& strRecordDomainName)
    {
        recordDomainName = strRecordDomainName;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private: 
    //CRcsQueryRecordDaysReq req;
    string req;
    string recordDomainName;
};


typedef CRcsQueryRecordDaysRsp CCuRcsQueryRecordDaysRsp;

typedef CPuRecDaysQryReq CCuPuRecDaysQryReq;
typedef CPuRecDaysQryRsp CCuPuRecDaysQryRsp;

class CCuRcsQueryRecordReq : public CEventReq
{
public:
    CCuRcsQueryRecordReq()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_REQ);
    }
public:
    const CTransparentData& GetReq()const
    {
        return req;
    }
    CTransparentData& GetReq()
    {
        return req;
    }
    void SetReq(const CTransparentData& tReq)
    {
        req = tReq;
    }
    const string& GetDomainName()const
    {
        return recordDomainName;
    }
    string& GetDomainName()
    {
        return recordDomainName;
    }
    void SetRecordDomainName(const string& strRecordDomainName)
    {
        recordDomainName = strRecordDomainName;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:   
    CTransparentData req;
    string recordDomainName;
};

class CCuRcsQueryRecordRsp : public CCUQueryResultBase
{
public:
    CCuRcsQueryRecordRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_RSP);
    }

    void Clear()
    {
        recDurationList.clear();
    }
public:
    const TRecDurationList& GetDurantionList()const
    {
        return recDurationList;
    }

    TRecDurationList& GetDurantionList()
    {
        return recDurationList;
    }
    void SetDurationList(const TRecDurationList& var)
    {
        recDurationList = var;
    }

    const string& GetDomainName() const
    {
        return domainName;
    }

    string& GetDomainName()
    {
        return domainName;
    }
    void SetDomainName(const string & var)
    {
        domainName = var;
    }

    void push_back(CRcsQueryRecordNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string domainName;
    TRecDurationList    recDurationList;
};

//开启放像这块还没有接口给定
typedef CRcsSetRecordTaskReq CCuRcsSetRecordTaskReq;
typedef CRcsSetRecordTaskRsp CCuRcsSetRecordTaskRsp;

typedef CRcsSetRecordTaskReq CCuRcsStartRecordTaskReq;
typedef CRcsSetRecordTaskRsp CCuRcsStartRecordTaskRsp;

typedef CRcsSetRecordTaskReq CCuRcsStopRecordTaskReq;
//typedef CCuStopInviteRsp CCuRcsStopRecordTaskRsp;
typedef CRcsSetRecordTaskRsp CCuRcsStopRecordTaskRsp;

typedef CPuRecordStartReq CCuPuRecordStartReq;
typedef CPuRecordStartRsp CCuPuRecordStartRsp;

typedef CPuRecordStopReq CCuPuRecordStopReq;
typedef CPuRecordStopRsp CCuPuRecordStopRsp;

class CCuVcrCtrlReq : public CVcrCtrlReq

//typedef CPlaybackProgressQueryReq CCuPlaybackProgressQueryReq;
{
public:
    int playId;
public:
    const int GetPlayId()const
    {
        return playId;
    }

    int GetPlayId()
    {
        return playId;
    }
    void SetPlayId(int var)
    {
        playId = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CVcrCtrlRsp  CCuVcrCtrlRsp;

typedef CRcsQueryRecordParamReq CCuRcsQueryRecordParamReq;
class CCuRcsQueryRecordParamRsp : public CCUQueryResultBase
{
public:
    CCuRcsQueryRecordParamRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_RECORD_PARAM_RSP);
    }

    void Clear()
    {
        recordParamList.clear();
    }
public:
    const TRecordParamList& GetRecordParamList()const
    {
        return recordParamList;
    }

    TRecordParamList& GetRecordParamList()
    {
        return recordParamList;
    }
    void SetRecordParamList(const TRecordParamList& var)
    {
        recordParamList = var;
    }
    void push_back(CRcsQueryRecordParamNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    TRecordParamList  recordParamList;
};

typedef CRcsSetRecordParamReq CCuRcsSetRecordParamReq;
typedef CRcsSetRecordParamRsp CCuRcsSetRecordParamRsp;

typedef CTvwallAddReq CCuTvwallAddReq;
typedef CTvwallAddRsp CCuTvwallAddRsp;

typedef CTvwallDelReq CCuTvwallDelReq;
typedef CTvwallDelRsp CCuTvwallDelRsp;

typedef CTvwallModReq CCuTvwallModReq;
typedef CTvwallModRsp CCuTvwallModRsp;

// class CCuPlatRecOnTvReq : public CEventReq
// {
// public:
//     CCuPlatRecOnTvReq()
//     {
//         event = OspExtEventDesc(REAL_MEDIA_ON_TV_REQ);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// public:
//     const string& GetDomainName() const
//     {
//         return DomainName;
//     }
// 
//     string& GetDomainName()
//     {
//         return DomainName;
//     }
// 
//     void SetDomainName(const string& var)
//     {
//         DomainName = var;
//     }
// 
//     const CPlatRecOnTvReq& GetReq() const
//     {
//         return cReq;
//     }
// 
//     CPlatRecOnTvReq& GetReq()
//     {
//         return cReq;
//     }
// 
//     void SetReq(const CPlatRecOnTvReq& var)
//     {
//         cReq = var;
//     }
// private:
//     string DomainName;
//     CPlatRecOnTvReq cReq;
// };
typedef CPlatRecOnTvReq CCuPlatRecOnTvReq;
typedef CPlatRecOnTvRsp CCuPlatRecOnTvRsp;
typedef CPlatRecOffTvReq CCuPlatRecOffTvReq;
typedef CPlatRecOffTvRsp CCuPlatRecOffTvRsp;

typedef CRecordOnTvReq  CCuRecordOnTvReq;
typedef CRecordOnTvRsp  CCuRecordOnTvRsp;
typedef CRecordOffTvReq CCuRecordOffTvReq;
typedef CRecordOffTvRsp CCuRecordOffTvRsp;
// class CCuPlatRecOffTvReq : public CEventReq
// {
// public:
//     CCuPlatRecOffTvReq()
//     {
//         event = OspExtEventDesc(PLAT_REC_OFF_TV_REQ);
//     }
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// public:
//     const string& GetDomainName() const
//     {
//         return DomainName;
//     }
// 
//     string& GetDomainName()
//     {
//         return DomainName;
//     }
// 
//     void SetDomainName(const string& var)
//     {
//         DomainName = var;
//     }
// 
//     const CPlatRecOffTvReq& GetReq() const
//     {
//         return cReq;
//     }
// 
//     CPlatRecOffTvReq& GetReq()
//     {
//         return cReq;
//     }
// 
//     void SetReq(const CPlatRecOffTvReq& var)
//     {
//         cReq = var;
//     }
// private:
//     string DomainName;
//     CPlatRecOffTvReq cReq;
// };


typedef CDomainRelationQryReq CCuDomainRelationQryReq;
class CCuDomainRelationQryRsp : public CCUQueryResultBase
{
public:
    CCuDomainRelationQryRsp()
    {
        event = OspExtEventDesc(TAS_DOMAIN_RELATION_QRY_RSP);
    }
public:
    const vector<CDomainRelationInfo>& GetDomainRelationInfo() const
    {
        return infos;
    }

    vector<CDomainRelationInfo>& GetDomainRelationInfo()
    {
        return infos;
    }

    void SetDomainRelationInfo(const vector<CDomainRelationInfo>& var)
    {
        infos = var;
    }

    void push_back(CDomainRelationQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<CDomainRelationInfo> infos;
};

typedef CTvwallQryReq CCuTvwallQryReq;
class  CCuTvwallQryRsp : public CCUQueryResultBase
{
public:
    CCuTvwallQryRsp()
    {
        event = OspExtEventDesc(TAS_TVWALL_QRY_RSP);
    }
public:
    const vector<CTvwallInfo>& GetTvwallInfo() const
    {
        return infos;
    }

    vector<CTvwallInfo>& GetTvwallInfo()
    {
        return infos;
    }

    void SetTvwallInfo(const vector<CTvwallInfo>& var)
    {
        infos = var;
    }
    void push_back(CTvwallQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<CTvwallInfo> infos;
};

typedef CRealMediaOnTvReq CCuRealMediaOnTvReq;
typedef CRealMediaOnTvRsp CCuRealMediaOnTvRsp;

typedef CRealMediaOffTvReq CCuRealMediaOffTvReq;
typedef CRealMediaOffTvRsp CCuRealMediaOffTvRsp;

typedef CTvPollOperateReq CCuTvPollOperateReq;
typedef CTvPollOperateRsp CCuTvPollOperateRsp;

typedef CTvWallSchemeSaveReq CCuTvWallSchemeSaveReq;
typedef CTvWallSchemeSaveRsp CCuTvWallSchemeSaveRsp;

typedef CTvWallSchemeDelReq CCuTvWallSchemeDelReq;
typedef CTvWallSchemeDelRsp CCuTvWallSchemeDelRsp;

typedef CTvWallSchemeLoadReq CCuTvWallSchemeLoadReq;
typedef CTvWallSchemeLoadRsp CCuTvWallSchemeLoadRsp;

typedef CTvWallSchemeQueryReq CCuTvWallSchemeQueryReq;
class CCuTvWallSchemeQueryRsp : public CCUQueryResultBase
{
public:
    CCuTvWallSchemeQueryRsp()
    {
        event = OspExtEventDesc(TVWALL_SCHEME_QUERY_RSP);
    }
public:
    const CTTvWallSchemeList& GetTvWallScheme() const
    {
        return schemeList;
    }

    CTTvWallSchemeList& GetTvWallScheme()
    {
        return schemeList;
    }

    void SetTvWallScheme(const CTTvWallSchemeList& var)
    {
        schemeList = var;
    }

    void push_back(CTvWallSchemeNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    CTTvWallSchemeList  schemeList;
};

typedef CTvWallSchemeGroupSetReq CCuTvWallSchemeGroupSetReq;
typedef CTvWallSchemeGroupSetRsp CCuTvWallSchemeGroupSetRsp;

typedef CTvWallSchemeGroupGetReq CCuTvWallSchemeGroupGetReq;
typedef CTvWallSchemeGroupGetRsp CCuTvWallSchemeGroupGetRsp;

typedef CTvWallSchemePollOperateReq CCuTvWallSchemePollOperateReq;
typedef CTvWallSchemePollOperateRsp CCuTvWallSchemePollOperateRsp;

typedef CMapCreateReq CCuMapCreateReq;
typedef CMapCreateRsp CCuMapCreateRsp;

typedef CMapModReq CCuMapModReq;
typedef CMapModRsp CCuMapModRsp;

typedef CMapDelReq CCuMapDelReq;
typedef CMapDelRsp CCuMapDelRsp;

typedef CMapQryReq CCuMapQryReq;
class CCuMapQryRsp : public CCUQueryResultBase
{
public:
    CCuMapQryRsp()
    {
        event = OspExtEventDesc(MPS_MAP_QRY_RSP);
    }
public:
    const vector<MapInfo>& GetMapInfos() const
    {
        return mapInfos;
    }

    vector<MapInfo>& GetMapInfos()
    {
        return mapInfos;
    }

    void SetMapInfos(const vector<MapInfo>& var)
    {
        mapInfos = var;
    }

    void push_back(CMapQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<MapInfo> mapInfos;
};

typedef CMapElementCreateReq CCuMapElementCreateReq;
typedef CMapElementCreateRsp CCuMapElementCreateRsp;

typedef CMapElementModReq CCuMapElementModReq;
typedef CMapElementModRsp CCuMapElementModRsp;

typedef CMapElementDelReq CCuMapElementDelReq;
typedef CMapElementDelRsp CCuMapElementDelRsp;

typedef CMapElementQryReq CCuMapElementQryReq;
class CCuMapElementQryRsp : public CCUQueryResultBase
{
public:
    CCuMapElementQryRsp()
    {
        event = OspExtEventDesc(MPS_MAP_ELEMENT_QRY_RSP);
    }
public:
    const vector<MapElement> &GetMapElement() const
    {
        return mapElements;
    }

    vector<MapElement> &GetMapElement()
    {
        return mapElements;
    }

    void SetMapElement(const vector<MapElement> &Var)
    {
        mapElements = Var;
    } 

    void push_back(CMapElementQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<MapElement> mapElements;
};

typedef CMapElementSearchReq CCuMapElementSearchReq;
class CCuMapElementSearchRsp : public CCUQueryResultBase
{
public:
	CCuMapElementSearchRsp()
	{
		event = OspExtEventDesc(MPS_MAP_ELEMENT_SEARCH_RSP);
	}
public:
	const vector<MapElement> &GetMapElement() const
	{
		return mapElements;
	}

	vector<MapElement> &GetMapElement()
	{
		return mapElements;
	}

	void SetMapElement(const vector<MapElement> &Var)
	{
		mapElements = Var;
	}

	void push_back(CMapElementSearchNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	vector<MapElement> mapElements;
};
typedef CPosCorrectMarsToEarthReq CCuPosCorrectMarsToEarthReq;
typedef CPosCorrectMarsToEarthRsp CCuPosCorrectMarsToEarthRsp;

typedef CPosCorrectEarthToMarsReq CCuPosCorrectEarthToMarsReq;
typedef CPosCorrectEarthToMarsRsp CCuPosCorrectEarthToMarsRsp;

typedef CMpsDataVersionGetReq CCuMpsDataVersionGetReq;
typedef CMpsDataVersionGetRsp CCuMpsDataVersionGetRsp;

typedef CGpsDataBakeUpReq CCuGpsDataBakeUpReq;
typedef CGpsDataBakeUpRsp CCuGpsDataBakeUpRsp;

typedef CMpsGpsStoreCfgGetReq CCuMpsGpsStoreCfgGetReq;
typedef CMpsGpsStoreCfgGetRsp CCuMpsGpsStoreCfgGetRsp;

typedef CAlarmLinkQueryReq CCuAlarmLinkQueryReq;
class CCuAlarmLinkQueryRsp :public CCUQueryResultBase
{
public:
    CCuAlarmLinkQueryRsp()
    {
        event = OspExtEventDesc(ALARMLINK_QUERY_RSP);
    }
public:
    const CAlarmLinkageList& GetAlarmLink() const
    {
        return cfgList;
    }

    CAlarmLinkageList& GetAlarmLink()
    {
        return cfgList;
    }

    void SetAlarmLink(const CAlarmLinkageList& var)
    {
        cfgList = var;
    }

    void push_back(CAlarmLinkNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    CAlarmLinkageList  cfgList;                                     //配置列表
};

typedef CAlarmLinkVersionGetReq CCuAlarmLinkVersionGetReq;
typedef CAlarmLinkVersionGetRsp CCuAlarmLinkVersionGetRsp;
typedef CAlarmLinkCfgGetReq CCuAlarmLinkCfgGetReq;
typedef CAlarmLinkCfgGetRsp CCuAlarmLinkCfgGetRsp;

typedef CAlarmLinkCfgSetReq CCuAlarmLinkCfgSetReq;
typedef CAlarmLinkCfgSetRsp CCuAlarmLinkCfgSetRsp;

typedef COperateLogAddReq CCuOperateLogAddReq;
typedef CEventRsp CCuOperateLogAddRsp;

typedef COperateLogQryReq CCuOperateLogQryReq;
class CCuOperateLogQryRsp : public CCUQueryResultBase
{
public:
    CCuOperateLogQryRsp()
    {
        event = OspExtEventDesc(UAS_OPERATE_LOG_QRY_RSP);
    }
public:
    const vector<TOperateLogInfo>& GetInfo() const
    {
        return infos;
    }

    vector<TOperateLogInfo>& GetInfo()
    {
        return infos;
    }

    void SetInfo(const vector<TOperateLogInfo>& var)
    {
        infos = var;
    }

    void push_back(COperateLogQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<TOperateLogInfo> infos;
};

typedef CGpsTraceQryReq CCuGpsTraceQryReq;
class CCuGpsTraceQryRsp : public CCUQueryResultBase
{
public:
	CCuGpsTraceQryRsp()
	{
		event = OspExtEventDesc(MPS_GPS_TRACE_QRY_RSP);
	}
public:
	const vector<TGpsStatus>& GetInfo() const
	{
		return infos;
	}
	vector<TGpsStatus>& GetInfo()
	{
		return infos;
	}
	void SetInfo(const vector<TGpsStatus>& var)
	{
		infos = var;
	}

	void push_back(CGpsTraceQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
private:
	vector<TGpsStatus> infos;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CPuSerialParamCfgGetReq CCuPuSerialParamCfgGetReq;
typedef CPuSerialParamCfgGetRsp CCuPuSerialParamCfgGetRsp;
typedef CPuSerialParamCfgSetReq CCuPuSerialParamCfgSetReq;
typedef CPuSerialParamCfgSetRsp CCuPuSerialParamCfgSetRsp;

typedef CDeviceLogQryReq CCuDeviceLogQryReq;
class CCuDeviceLogQryRsp : public CCUQueryResultBase
{
public:
    CCuDeviceLogQryRsp()
    {
        event = OspExtEventDesc(UAS_DEVICE_LOG_QRY_RSP);
    }
public:
    const vector<TDeviceLogInfo>& GetInfo() const
    {
        return infos;
    }

    vector<TDeviceLogInfo>& GetInfo()
    {
        return infos;
    }

    void SetInfo(const vector<TDeviceLogInfo>& var)
    {
        infos = var;
    }

    void push_back(CDeviceLogQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<TDeviceLogInfo> infos;
};

typedef CAlarmLogQryReq CCuAlarmLogQryReq;
class CCuAlarmLogQryRsp : public CCUQueryResultBase
{
public:
    CCuAlarmLogQryRsp()
    {
        event = OspExtEventDesc(UAS_ALARM_LOG_QRY_RSP);
    }
public:
    const vector<TAlarmLogInfo>& GetInfo() const
    {
        return infos;
    }

    vector<TAlarmLogInfo>& GetInfo()
    {
        return infos;
    }

    void SetInfo(const vector<TAlarmLogInfo>& var)
    {
        infos = var;
    }

    void push_back(CAlarmLogQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    vector<TAlarmLogInfo> infos;
};

typedef CUserCustomDataAddReq CCuUserCustomDataAddReq;
typedef CUserCustomDataAddRsp CCuUserCustomDataAddRsp;
typedef CUserCustomDataDelReq CCuUserCustomDataDelReq;
typedef CUserCustomDataDelRsp CCuUserCustomDataDelRsp;
typedef CUserCustomDataQryReq CCuUserCustomDataQryReq;
class CCuUserCustomDataQryRsp : public CCUQueryResultBase
{
public:
    CCuUserCustomDataQryRsp()
    {
        event = OspExtEventDesc(UAS_CUSER_CUSTOMDATA_QRY_RSP);
    }
private:    
    vector<TUserCustomDataInfo> infos;

public:
    void SetInfos(const vector<TUserCustomDataInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TUserCustomDataInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TUserCustomDataInfo>& GetInfos() const
    {
        return infos;
    }

    void push_back(CUserCustomDataQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CConfigUasGetReq CCuConfigUasGetReq;
typedef CConfigUasGetRsp CCuConfigUasGetRsp;

typedef CConfigUasSetReq CCuConfigUasSetReq;
typedef CConfigUasSetRsp CCuConfigUasSetRsp;

typedef CStartThirdPartyCallReq CCuStartThirdPartyCallReq;
typedef CStartThirdPartyCallRsp CCuStartThirdPartyCallRsp;

typedef CStopThirdPartyCallReq CCuStopThirdPartyCallReq;
typedef CStopThirdPartyCallRsp CCuStopThirdPartyCallRsp;

typedef CPuVidEncParamGetReq CCuPuVidEncParamGetReq;
typedef CPuVidEncParamGetRsp CCuPuVidEncParamGetRsp;

typedef CPuVidEncParamSetReq CCuPuVidEncParamSetReq;
typedef CPuVidEncParamSetRsp CCuPuVidEncParamSetRsp;

typedef CPuAudEncParamGetReq CCuPuAudEncParamGetReq;
typedef CPuAudEncParamGetRsp CCuPuAudEncParamGetRsp;

typedef CPuVidSrcOsdSetReq CCuPuVidSrcOsdSetReq;
typedef CPuVidSrcOsdSetRsp CCuPuVidSrcOsdSetRsp;

typedef CPuVidSrcOsdGetReq CCuPuVidSrcOsdGetReq;
typedef CPuVidSrcOsdGetRsp CCuPuVidSrcOsdGetRsp;

typedef CRcsQueryDiskInfoReq CCuRcsQueryDiskInfoReq;
class CCuRcsQueryDiskInfoRsp : public CCUQueryResultBase
{
public:
    CCuRcsQueryDiskInfoRsp()
    {
        event = OspExtEventDesc(RCS_QUERY_DISK_INFO_RSP);
    }
public:
    const TDiskInfoList& GetDiskInfo() const
    {
        return diskInfoList;
    }

    TDiskInfoList& GetDiskInfo()
    {
        return diskInfoList;
    }

    void SetDiskInfo(const TDiskInfoList& var)
    {
        diskInfoList = var;
    }

    void AddDiskInfo(const TDiskInfoList& var)
    {
        diskInfoList.insert( diskInfoList.end(), var.begin(), var.end() );
    }

    void push_back(CRcsQueryDiskInfoNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:    
    TDiskInfoList	diskInfoList;
};

typedef CDeviceModReq CCuDeviceNameModReq;
typedef CDeviceModRsp CCuDeviceNameModRsp;

// typedef CDeviceCapIndexNameSetReq CCuDeviceCapIndexNameSetReq;
// typedef CDeviceCapIndexNameSetRsp CCuDeviceCapIndexNameSetRsp;

typedef CPuMotionDetectGetReq CCuPuMotionDetectGetReq;
typedef CPuMotionDetectGetRsp CCuPuMotionDetectGetRsp;

typedef CPuMotionDetectSetReq CCuPuMotionDetectSetReq;
typedef CPuMotionDetectSetRsp CCuPuMotionDetectSetRsp;

typedef CPuAreaShadeGetReq CCuPuAreaShadeGetReq;
typedef CPuAreaShadeGetRsp CCuPuAreaShadeGetRsp;

typedef CPuAreaShadeSetReq CCuPuAreaShadeSetReq;
typedef CPuAreaShadeSetRsp CCuPuAreaShadeSetRsp;

typedef CRcsRecordLockOprReq CCuRcsRecordLockOprReq;
typedef CRcsRecordLockOprRsp CCuRcsRecordLockOprRsp;

typedef CRcsRecordLockOprReq CCuRcsRecordUnLockOprReq;
typedef CRcsRecordLockOprRsp CCuRcsRecordUnLockOprRsp;

class CCuChangePasswordReq :public CEventReq
{
public:
    CCuChangePasswordReq()
    {
        event = OspExtEventDesc(CU_CUI_CHANGE_PASSWORD_REQ);
    }
public:
    const string& GetOldPassword() const
    {
        return oldPassword;
    }

    string& GetOldPassword()
    {
        return oldPassword;
    }

    void SetOldPassword(const string& var)
    {
        oldPassword = var;
    }

    const string& GetNewPassword() const
    {
        return newPassword;
    }

    string& GetNewPassword()
    {
        return newPassword;
    }

    void SetNewPassword(const string& var)
    {
        newPassword = var;
    }

    const string& GetPasswordOverdueTime() const
    {
        return passwordOverdueTime;
    }

    string& GetPasswordOverdueTime()
    {
        return passwordOverdueTime;
    }

    void SetPasswordOverdueTime(const string& var)
    {
        passwordOverdueTime = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string oldPassword;
    string newPassword;
    string passwordOverdueTime;
};
typedef CUserModRsp CCuChangePasswordRsp; 

typedef CKeyFrameReq CCuKeyFrameReq;
typedef CKeyFrameRsp CCuKeyFrameRsp;

typedef CTvWinStyleSetReq CCuTvWinStyleSetReq;
typedef CTvWinStyleSetRsp CCuTvWinStyleSetRsp;

typedef CDeviceCapIndexNameGetReq CCuDeviceCapIndexNameGetReq;
typedef CDeviceCapIndexNameGetRsp CCuDeviceCapIndexNameGetRsp;

typedef CDeviceCapIndexNameSetReq CCuDeviceCapIndexNameSetReq;
typedef CDeviceCapIndexNameSetRsp CCuDeviceCapIndexNameSetRsp;

typedef CCfgSyncSwitchGetReq CCuGetSyncVideoSrcNameEnableReq;
typedef CCfgSyncSwitchGetRsp CCuGetSyncVideoSrcNameEnableRsp;

typedef CCfgSyncSwitchSetReq CCuSetSyncVideoSrcNameEnableReq;
typedef CCfgSyncSwitchSetRsp CCuSetSyncVideoSrcNameEnableRsp;

typedef CGetSupportMapTypesReq CCuGetSupportMapTypesReq;
typedef CGetSupportMapTypesRsp CCuGetSupportMapTypesRsp;

typedef CMapAreaQryReq CCuMapAreaQryReq;
class CCuMapAreaQryRsp : public CCUQueryResultBase
{
public:
    CCuMapAreaQryRsp()
    {
        event = OspExtEventDesc(MPS_MAP_AREA_QRY_RSP);
    }
public:
    void SetResults(const vector<MapArea>& cResults)
    {
        results = cResults;
    }
    vector<MapArea>& GetResults()
    {
        return results;
    }
    const vector<MapArea>& GetResults() const
    {
        return results;
    }

    void push_back(CMapAreaQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:    
    vector<MapArea> results;
};

typedef CMapAreaCreateReq  CCuMapAreaCreateReq;
typedef CMapAreaCreateRsp  CCuMapAreaCreateRsp; 

typedef CMapAreaModReq CCuMapAreaModReq;
typedef CMapAreaModRsp CCuMapAreaModRsp;

typedef CPuVsipOperateReq CCuPuVsipOperateReq;
typedef CPuVsipOperateRsp CCuPuVsipOperateRsp;

typedef CMapUploadReq CCuMapUploadReq;
typedef CMapUploadRsp CCuMapUploadRsp;

typedef CElementIconUploadReq CCuElementIconUploadReq;
typedef CElementIconUploadRsp CCuElementIconUploadRsp;

typedef CElementImageQryReq CCuElementImageQryReq;
class CCuElementImageQryRsp : public CCUQueryResultBase
{
public:
    CCuElementImageQryRsp()
    {
        event = OspExtEventDesc(MPS_ELEMENT_IMAGE_QRY_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void push_back(CElementImageQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);

    void SetImageList(const vector<string>& imageList)
    {
        imageUrlList = imageList;
    }
    vector<string>& GetImageList()
    {
        return imageUrlList;
    }
    const vector<string>& GetImageList() const
    {
        return imageUrlList;
    }
private:
    vector<string> imageUrlList;
};

typedef CPuCameraTypeSetReq CCuPuCameraTypeSetReq;
typedef CPuCameraTypeSetRsp CCuPuCameraTypeSetRsp;

typedef CPuAlarmResetReq CCuPuAlarmResetReq;
typedef CPuAlarmResetRsp CCuPuAlarmResetRsp;

class CCuPuRecQryReq: public CPuRecQryReq
{
public:
    CCuPuRecQryReq()
    {
        event = OspExtEventDesc(CU_CUI_PU_REC_QRY_REQ);
    }

private:
    LongQueryHandle queryHandle;                
public:
    void SetQueryHandle(const LongQueryHandle& strQueryHandle)
    {
        queryHandle = strQueryHandle;
    }
    LongQueryHandle& GetQueryHandle()
    {
        return queryHandle;
    }
    const LongQueryHandle& GetQueryHandle() const
    {
        return queryHandle;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuPuRecQryRsp : public CCULongQueryResultBase
{
public:
    CCuPuRecQryRsp()
    {
		Clear();
        event = OspExtEventDesc(CU_CUI_PU_REC_QRY_RSP);
    }

	void Clear()
	{
		isUseVSIPTimeAsGMTTime = false;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetPuChn(const TChannel& cPuChn)
    {
        puChn = cPuChn;
    }

    TChannel& GetPuChn()
    {
        return puChn;
    }
    const TChannel& GetPuChn() const
    {
        return puChn;
    }

    void SetRecInfoList(const CPuRecInfoList& cRecInfoList)
    {
        recInfoList = cRecInfoList;
    }

    CPuRecInfoList& GetRecInfoList()
    {
        return recInfoList;
    }
    const CPuRecInfoList& GetRecInfoList() const
    {
        return recInfoList;
    }

	void SetIsUseVSIPTimeAsGMTTime(bool var)
	{
		isUseVSIPTimeAsGMTTime = var;
	}
	bool GetIsUseVSIPTimeAsGMTTime() const
	{
		return isUseVSIPTimeAsGMTTime;
	}

    void push_back(CPuRecQryNtfReq &cNtf, const CTask *pcQueryTask);
private:            
    TChannel            puChn;                         //设备通道
    CPuRecInfoList      recInfoList;                   //录像信息列表
	bool                isUseVSIPTimeAsGMTTime;        //是否使用VSIP时间作为GMT时间
};


class CDeviceRecordSvrQryReq: public CEventReq
{
public:
    CDeviceRecordSvrQryReq()
    {
        event = OspExtEventDesc(CU_CUI_DEV_RECORD_SVR_QRY_REQ);
    }
public:
    
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


typedef   vector<string>    CDevRecordSvrUriList;

class CDeviceRecordSvrQryRsp: public CEventRsp
{
public:
    CDeviceRecordSvrQryRsp()
    {
        event = OspExtEventDesc(CU_CUI_DEV_RECORD_SVR_QRY_RSP);
    }

public:
    const CDevRecordSvrUriList& GetUriList() const
    {
        return uriList;
    }

    CDevRecordSvrUriList& GetUriList()
    {
        return uriList;
    }
    void SetUriList( const CDevRecordSvrUriList& uris )
    {
        uriList = uris;
    }

private:
    CDevRecordSvrUriList uriList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CDeviceRecordSvrQryReq CCuDeviceRecordSvrQryReq;
typedef CDeviceRecordSvrQryRsp CCuDeviceRecordSvrQryRsp;


class CCuPlaybackProgressQueryReq : public CEventReq
{
private:
    string  playbackSessUri;
    int playId;
public:
    CCuPlaybackProgressQueryReq()
    {
        Clear();
        event = OspExtEventDesc(CU_CUI_PLAYBACK_PROGRESS_QUERY_REQ);
    }
    void Clear()
    {
        playbackSessUri.clear();
    }
public:
    const int GetPlayId()const
    {
        return playId;
    }
    int GetPlayId()
    {
        return playId;
    }
    void SetPlayId(const int var)
    {
        playId = var;
    }
    void SetPlaybackSessUri(const string& var)
    {
        playbackSessUri = var;
    }
    string& GetPlaybackSessUri()
    {
        return playbackSessUri;
    }
    const string& GetPlaybackSessUri() const
    {
        return playbackSessUri;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CCuPlaybackProgressQueryRsp : public CEventRsp
{
private:
    u32         curPlayTime;                             //当前放像时间，单位：秒
    u32         totalPlayTime;                           //放像总时长
    u32         startPlayTime;                           //起始放像时间，单位：秒
    bool        isPlayFinished;                          //放像是否结束
public:
    CCuPlaybackProgressQueryRsp()
    {
        event = OspExtEventDesc(CU_CUI_PLAYBACK_PROGRESS_QUERY_RSP);
    }
    void SetCurPlayTime(u32 nCurPlayTime)
    {
        curPlayTime = nCurPlayTime;
    }
    u32 GetCurPlayTime() const
    {
        return curPlayTime;
    }
    void SetTotalPlayTime(u32 nTotalPlayTime)
    {
        totalPlayTime = nTotalPlayTime;
    }
    u32 GetTotalPlayTime() const
    {
        return totalPlayTime;
    }
    void SetStartPlayTime(u32 nStartPlayTime)
    {
        startPlayTime = nStartPlayTime;
    }
    u32 GetStartPlayTime() const
    {
        return startPlayTime;
    }
    void SetIsPlayFinished(bool bIsPlayFinished)
    {
        isPlayFinished = bIsPlayFinished;
    }
    bool GetIsPlayFinished() const
    {
        return isPlayFinished;
    }
    void SetPlayBackProgressNtf(const CPlaybackProgressNtfReq& cReq)
    {
        curPlayTime = cReq.GetCurPlayTime();                          
        totalPlayTime = cReq.GetTotalPlayTime();                        
        startPlayTime = cReq.GetStartPlayTime();      
        isPlayFinished = cReq.GetIsPlayFinished();          
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CVideosourcePresetcfgQryReq CCuVideosourcePresetcfgQryReq;
class CCuVideosourcePresetcfgQryRsp : public CCUQueryResultBase
{
public:
    CCuVideosourcePresetcfgQryRsp()
    {
        event = OspExtEventDesc(UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP);
    }
private:
    vector<TVideosourcePresetCfgInfo> infos; 
public:
    void push_back(CVideosourcePresetcfgQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask);

    void SetInfos(const vector<TVideosourcePresetCfgInfo>& cInfos)
    {
        infos = cInfos;
    }
    vector<TVideosourcePresetCfgInfo>& GetInfos()
    {
        return infos;
    }
    const vector<TVideosourcePresetCfgInfo>& GetInfos() const
    {
        return infos;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CGbsDevGbidGetReq CCuGbsDevGbidGetReq;
typedef CGbsDevGbidGetRsp CCuGbsDevGbidGetRsp;

typedef CVideosourcePresetcfgAddReq CCuVideosourcePresetcfgAddReq;
typedef CVideosourcePresetcfgAddRsp CCuVideosourcePresetcfgAddRsp;

typedef CVideosourcePresetcfgDelReq CCuVideosourcePresetcfgDelReq;
typedef CVideosourcePresetcfgDelRsp CCuVideosourcePresetcfgDelRsp;

typedef CFileExportReq CCuFileExportReq; 
typedef CFileExportRsp CCuFileExportRsp; 

// 仅限于和3A UAS通信，旧接口
class CCuGetExportProcessReq : public CEventReq
{
private:
    string  exportFileUrl;
public:
    CCuGetExportProcessReq()
    {
        Clear();
        event = OspExtEventDesc(CU_CUI_GET_EXPORT_PROCESS_REQ); 
    }
    void Clear()
    {
        exportFileUrl.clear();
    }
public:
    void SetExpoetFileUrl(const string& var)
    {
        exportFileUrl = var;
    }
    string& GetExportFileUrl()
    {
        return exportFileUrl;
    }
    const string& GetExportFileUrl() const
    {
        return exportFileUrl;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CCuGetExportProcessRsp : public CEventRsp
{
public:
    CCuGetExportProcessRsp()
    {
        Clear();
        event = OspExtEventDesc(CU_CUI_GET_EXPORT_PROCESS_RSP);
    }
    void Clear()
    {
        percent = 0;
    }
private:
    int percent;                          //文件收集进度百分比[0-100]，达到100时，可以开启http下载
public:
    void SetPercent(int nPercent)
    {
        percent = nPercent;
    }
    const int GetPercent() const
    {
        return percent;
    }
    int GetPercent()
    {
        return percent;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuStopExportReq : public CEventReq
{
public:
	CCuStopExportReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_STOP_EXPORT_REQ);
	}
	void Clear()
	{
		exportFileUrl.clear();
	}
private:
	string  exportFileUrl;
public:
	void SetExportFileUrl(const string& var)
	{
		exportFileUrl = var;
	}
	string& GetExportFileUrl()
	{
		return exportFileUrl;
	}
	const string& GetExportFileUrl() const
	{
		return exportFileUrl;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CCuStopExportRsp : public CEventRsp
{
public:
	CCuStopExportRsp()
	{
		event = OspExtEventDesc(CU_CUI_STOP_EXPORT_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileExportStartExReq : public CEventReq
{
public:
	CCuFileExportStartExReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_FILE_EXPORT_START_EX_REQ);
		fileCharacterset = "GBK";
	}

	void Clear()
	{
		srvName.clear();
		fileReceiverUri.clear();
		fileSenderUri.clear();
		fileToken.clear();
		CEventReq::Clear();
	}

private: 
	TFileSvcType        srvName;                            //业务类型
	string              fileReceiverUri;                    //文件接受者：pmc、cu等
	string              fileSenderUri;                      //文件发送者: pu、pms、uas等
	CTransparentData    fileToken;                          //文件标识：表示文件标识或者导出条件（表示导出条件时内容参见具体的业务类型说明）
	string              fileCharacterset;                   //文件的字符集：字符集名称表示参见linux "iconv --list"的输出
	string              fileTimeZone;                       //文件中时间的时区
	std::vector<string> customParams;						// 自定义参数,每种业务类型不同.
public:
	void SetSrvName(const TFileSvcType& strSrvName)
	{
		srvName = strSrvName;
	}
	TFileSvcType& GetSrvName()
	{
		return srvName;
	}
	const TFileSvcType& GetSrvName() const
	{
		return srvName;
	}

	void SetFileReceiverUri(const string& strFileReceiverUri)
	{
		fileReceiverUri = strFileReceiverUri;
	}
	string& GetFileReceiverUri()
	{
		return fileReceiverUri;
	}
	const string& GetFileReceiverUri() const
	{
		return fileReceiverUri;
	}

	void SetFileSenderUri(const string& strFileSenderUri)
	{
		fileSenderUri = strFileSenderUri;
	}
	string& GetFileSenderUri()
	{
		return fileSenderUri;
	}
	const string& GetFileSenderUri() const
	{
		return fileSenderUri;
	}

	void SetFileToken(const CTransparentData& strFileToken)
	{
		fileToken = strFileToken;
	}
	CTransparentData& GetFileToken()
	{
		return fileToken;
	}
	const CTransparentData& GetFileToken() const
	{
		return fileToken;
	}
	void SetFileCharacterset(const string& strFileCharacterset)
	{
		fileCharacterset = strFileCharacterset;
	}
	string& GetFileCharacterset()
	{
		return fileCharacterset;
	}
	const string& GetFileCharacterset() const
	{
		return fileCharacterset;
	}
	void SetFileTimeZone(const string& strTimeZone)
	{
		fileTimeZone = strTimeZone;
	}
	string& GetFileTimeZone()
	{
		return fileTimeZone;
	}
	const string& GetFileTimeZone() const
	{
		return fileTimeZone;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
    void SetCustomParams(const std::vector<string>& cCustomParams)
    {
        customParams = cCustomParams;
    }
    std::vector<string>& GetCustomParams()
    {
        return customParams;
    }
    const std::vector<string>& GetCustomParams() const
    {
        return customParams;
    }

};
class CCuFileExportStartExRsp : public CEventRsp
{
private: 
	string			fileURL;        //文件URL
	u32				taskNo;
	string			taskType;		// 定义在 cuidata.h #define DEF_TASK_TYPE_xxx
public:
	CCuFileExportStartExRsp()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_FILE_EXPORT_START_EX_RSP);
	}

	void Clear()
	{
		fileURL.clear();
		taskNo = INVALID_TASKNO;
		taskType.clear();
		CEventRsp::Clear();
	}

public:
	void SetFileURL(const string& strFileURL)
	{
		fileURL = strFileURL;
	}
	string& GetFileURL()
	{
		return fileURL;
	}
	const string& GetFileURL() const
	{
		return fileURL;
	}

	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}
	void SetTaskType(const string &strTaskType)
	{
		taskType = strTaskType;
	}
	const string &GetTaskType() const
	{
		return taskType;
	}
	string &GetTaskType()
	{
		return taskType;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileExportGetProcessExReq : public CEventReq
{
private:
	u32			taskNo;
	string		taskType;		// 定义在 cuidata.h #define DEF_TASK_TYPE_xxx
public:
	CCuFileExportGetProcessExReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_FILE_EXPORT_GET_PROCESS_EX_REQ); 
	}
	void Clear()
	{
		taskNo = INVALID_TASKNO;
		taskType.clear();
	}
public:
	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}

	void SetTaskType(const string &strTaskType)
	{
		taskType = strTaskType;
	}
	const string &GetTaskType() const
	{
		return taskType;
	}
	string &GetTaskType()
	{
		return taskType;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileExportGetProcessExRsp : public CEventRsp
{
private:
	int percent;                          //文件收集进度百分比[0-100]，达到100时，可以开启http下载
public:
	CCuFileExportGetProcessExRsp()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_FILE_EXPORT_GET_PROCESS_EX_RSP);
	}
	void Clear()
	{
		percent = 0;
	}
public:
	void SetPercent(int nPercent)
	{
		percent = nPercent;
	}
	const int GetPercent() const
	{
		return percent;
	}
	int GetPercent()
	{
		return percent;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CCuFileExportStopExReq : public CEventReq
{
private:
	u32 taskNo;
public:
	CCuFileExportStopExReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_FILE_EXPORT_STOP_EX_REQ);
	}
	void Clear()
	{
		taskNo = INVALID_TASKNO;
	}
public:
	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CCuFileExportStopExRsp : public CEventRsp
{
public:
	CCuFileExportStopExRsp()
	{
		event = OspExtEventDesc(CU_CUI_FILE_EXPORT_STOP_EX_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileImportStartExReq : public CEventReq
{
public:
	CCuFileImportStartExReq()
	{
		event = OspExtEventDesc(CU_CUI_FILE_IMPORT_START_EX_REQ);
	}

	void Clear()
	{
		srvName.clear();
		fileReceiverUri.clear();
		fileSenderUri.clear();
		fileToken.clear();
		CEventReq::Clear();
	}

private: 
	TFileSvcType        srvName;                            //业务类型
	string              fileReceiverUri;                    //文件接受者：pmc、cu等
	string              fileSenderUri;                      //文件发送者: pu、pms、uas等
	string              fileToken;                          //文件标识
public:
	void SetSrvName(const TFileSvcType& strSrvName)
	{
		srvName = strSrvName;
	}
	TFileSvcType& GetSrvName()
	{
		return srvName;
	}
	const TFileSvcType& GetSrvName() const
	{
		return srvName;
	}

	void SetFileReceiverUri(const string& strFileReceiverUri)
	{
		fileReceiverUri = strFileReceiverUri;
	}
	string& GetFileReceiverUri()
	{
		return fileReceiverUri;
	}
	const string& GetFileReceiverUri() const
	{
		return fileReceiverUri;
	}

	void SetFileSenderUri(const string& strFileSenderUri)
	{
		fileSenderUri = strFileSenderUri;
	}
	string& GetFileSenderUri()
	{
		return fileSenderUri;
	}
	const string& GetFileSenderUri() const
	{
		return fileSenderUri;
	}

	void SetFileToken(const string& strFileToken)
	{
		fileToken = strFileToken;
	}
	string& GetFileToken()
	{
		return fileToken;
	}
	const string& GetFileToken() const
	{
		return fileToken;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileImportStartExRsp : public CEventRsp
{
private: 
	string	dirURL;			//目录URL，给出一个目录，请求端将文件上传到该目录，多个文件可以在一个会话内
	u32 taskNo;
public:
	CCuFileImportStartExRsp()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_FILE_IMPORT_START_EX_RSP);
	}

	void Clear()
	{
		dirURL.clear();
		taskNo = INVALID_TASKNO;
		CEventRsp::Clear();
	}

public:
	void SetDirURL(const string& strDirURL)
	{
		dirURL = strDirURL;
	}
	string& GetDirURL()
	{
		return dirURL;
	}
	const string& GetDirURL() const
	{
		return dirURL;
	}

	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileImportSetProcessExReq : public CEventReq
{
private:
	int progress;	// 处理完成的数量
	int total;		// 需要处理的总数
    u32 taskNo;		// taskNo
public:
	CCuFileImportSetProcessExReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_FILE_IMPORT_SET_PROCESS_EX_REQ);
	}
	void Clear()
	{
		progress = 0;
		total = 0;
		taskNo = INVALID_TASKNO;
	}

	void SetProgress(const int &progressNum)
	{
		progress = progressNum;
	}
	const int &GetProgress() const
	{
		return progress;
	}
	int &GetProgress()
	{
		return progress;
	}

	void SetTotal(const int& totalNum)
	{
		total = totalNum;
	}
	const int &GetTotal() const
	{
		return total;
	}
	int &GetTotal()
	{
		return total;
	}

	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileImportSetProcessExRsp : public CEventRsp
{
public:
	CCuFileImportSetProcessExRsp()
	{
		event = OspExtEventDesc(CU_CUI_FILE_IMPORT_SET_PROCESS_EX_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileImportStopExReq : public CEventReq
{
public:
	CCuFileImportStopExReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_FILE_IMPORT_STOP_EX_REQ);
	}
	void Clear()
	{
		taskNo = INVALID_TASKNO;
		cancelImport = false;
	}
private:
	u32 taskNo;
	bool cancelImport;
public:
	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}

	void SetCancelImport(bool val)
	{
		cancelImport = val;
	}
	bool& GetCancelImport()
	{
		return cancelImport;
	}
	const bool& GetCancelImport() const
	{
		return cancelImport;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuFileImportStopExRsp : public CEventRsp
{
public:
	CCuFileImportStopExRsp()
	{
		event = OspExtEventDesc(CU_CUI_FILE_IMPORT_STOP_EX_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuTaskStartReq : public CEventReq
{
public:
    CCuTaskStartReq()
    {
        Clear();
        event = OspExtEventDesc(RCS_TASK_START_REQ);
    }
    void Clear()
    {
    }
private:
    int  playId;
public:
    void SetPlayId(const int val)
    {
        playId = val;
    }
    int GetPlayId()
    {
        return playId;
    }
    const int GetPlayId() const
    {
        return playId;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef CCuPlayPlatformRecordRsp CCuTaskStartRsp;

enum ENATMode
{
	NATModeAuto,	// 自动模式,由服务端自动检测
	NATModeInner,	// 使用内网IP，即在NAT内
	NATModeOutside,	// 使用外网IP，即不再NAT内
};

#define MAX_NATINFO 3
const string NATMODEDSC[MAX_NATINFO]=
{
	"UnKnow",
	"InNat",
	"NotInNat"
}; 

class CCuUpdateCUPropertyReq : public CEventReq
{
public:
	CCuUpdateCUPropertyReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_CU_PROPERTY_REQ);
	}
	void Clear()
	{
		clientLocalIp.clear();
		PlatNatMode=NATModeAuto;
		clientSupCuiTransPlatRec = false;
	}
private:
	TIpAddr clientLocalIp;			// 客户端本机IP     
	int  PlatNatMode;				// 平台NAT模式.ENATMode
	bool clientSupCuiTransPlatRec;	// 客户端是否支持平台录像回放和下载走NAT探测包模式
public:
    void SetClientLocalIp(const TIpAddr& strClientLocalIp)
    {
        clientLocalIp = strClientLocalIp;
    }
    TIpAddr& GetClientLocalIp()
    {
        return clientLocalIp;
    }
    const TIpAddr& GetClientLocalIp() const
    {
        return clientLocalIp;
    }

    void SetPlatNatMode(int nPlatNatMode)
    {
        PlatNatMode = nPlatNatMode;
    }
    int GetPlatNatMode() const
    {
        return PlatNatMode;
    }
	
    void SetClientSupCuiTransPlatRec(bool bClientSupCuiTransPlatRec)
	{
	    clientSupCuiTransPlatRec = bClientSupCuiTransPlatRec;
	}
	bool GetClientSupCuiTransPlatRec() const
	{
        return clientSupCuiTransPlatRec;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
class CCuUpdateCUPropertyRsp : public CEventRsp
{
public:
	CCuUpdateCUPropertyRsp()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_CU_PROPERTY_RSP);
	}
private:
	int  ClientNat;					// 客户端NAT.
	int  PlatNat;					// 平台NAT.
	TNetAddr CuPunchAddr;			// CU punch打洞的地址
	bool cuiSupTransPlatRec;		// CUI是否支持平台录像回放和下载走NAT探测包模式
public:
	void Clear()
	{
		CEventRsp::Clear();
		ClientNat = 0;
		PlatNat = 0;
		CuPunchAddr.Clear();
		cuiSupTransPlatRec = false;
	}
    void SetClientNat(int nClientNat)
    {
        ClientNat = nClientNat;
    }
    int GetClientNat() const
    {
        return ClientNat;
    }

    void SetPlatNat(int nPlatNat)
    {
        PlatNat = nPlatNat;
    }
    int GetPlatNat() const
    {
        return PlatNat;
    }

    void SetCuPunchAddr(const TNetAddr& cCuPunchAddr)
    {
        CuPunchAddr = cCuPunchAddr;
    }
    TNetAddr& GetCuPunchAddr()
    {
        return CuPunchAddr;
    }
    const TNetAddr& GetCuPunchAddr() const
    {
        return CuPunchAddr;
    }
	
    void SetCuiSupTransPlatRec(bool bCuiSupTransPlatRec)
	{
	    cuiSupTransPlatRec = bCuiSupTransPlatRec;
	}
	bool GetCuiSupTransPlatRec() const
	{
        return cuiSupTransPlatRec;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CGetCAPSConfigReq  CCuGetCAPSConfigReq;
typedef CGetCAPSConfigRsp CCuGetCAPSConfigRsp;

typedef CSetCAPSConfigReq CCuSetCAPSConfigReq;
typedef	CSetCAPSConfigRsp CCuSetCAPSConfigRsp;

typedef CQueryDeviceListReq CCuQueryCAPSDeviceListReq;
class CCuQueryCAPSDeviceListRsp : public CCUQueryResultBase
{
public:
	CCuQueryCAPSDeviceListRsp()
	{
		event = OspExtEventDesc(CAPS_QUERY_DEVICE_LIST_RSP);
	}
private: 
	vector<CWechatDevice> devList;                
public:
	void push_back(CQueryDeviceListNtfReq &cNtf, const CPeQueryTask *pcQueryTask);

	void SetDevList(const vector<CWechatDevice>& cDevList)
	{
		devList = cDevList;
	}
	vector<CWechatDevice>& GetDevList()
	{
		return devList;
	}
	const vector<CWechatDevice>& GetDevList() const
	{
		return devList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef CAddDeviceReq CCuCAPSAddDeviceReq;
typedef CAddDeviceRsp CCuCAPSAddDeviceRsp;

typedef CModifyDeviceReq CCuCAPSModifyDeviceReq;
typedef CModifyDeviceRsp CCuCAPSModifyDeviceRsp;

typedef CDelDeviceReq CCuCAPSDelDeviceReq;
typedef CDelDeviceRsp CCuCAPSDelDeviceRsp;


class CCuOffLineDevQryReq : public COffLineDevQryReq
{
public:
	CCuOffLineDevQryReq()
	{
		event = OspExtEventDesc(CU_CUI_OFFLINEDEV_QRY_REQ);
	}

	std::string GetCuQueryTag() const
	{
		return cuQueryTag;
	}

	void SetCuQueryTag(std::string queryTag)
	{
		cuQueryTag = queryTag;
	}

private:
	std::string cuQueryTag; // 用于区分CU是否为同一次查询行为
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuOffLineDevQryRsp : public CCUQueryResultBase
{
public:
	CCuOffLineDevQryRsp()
	{
		event = OspExtEventDesc(CU_CUI_OFFLINEDEV_QRY_RSP);
		onlineVideoSrcNum=0;
		offlineDevNum=0;
		idleDevNum=0;
		devVideoSrcList.clear();
	}
private: 
	TDeviceVideoSrcIdList devVideoSrcList; 
	int onlineVideoSrcNum;  
	int offlineDevNum;       //离线设备数目
	int idleDevNum;          //idle设备数目
	std::string domainName;  //查询的域名
public:
	void push_back(COffLineDevQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
public:
    void SetDevVideoSrcList(const TDeviceVideoSrcIdList& cDevVideoSrcList)
    {
        devVideoSrcList = cDevVideoSrcList;
    }
    TDeviceVideoSrcIdList& GetDevVideoSrcList()
    {
        return devVideoSrcList;
    }
    const TDeviceVideoSrcIdList& GetDevVideoSrcList() const
    {
        return devVideoSrcList;
    }

	void SetOnlineVideoSrcNum(int nOnlineVideoSrcNum)
	{
		onlineVideoSrcNum = nOnlineVideoSrcNum;
	}
	int GetOnlineVideoSrcNum() const
	{
		return onlineVideoSrcNum;
	}

	void SetOfflineDevNum(int nOfflineDevNum)
	{
		offlineDevNum = nOfflineDevNum;
	}
	int GetOfflineDevNum()
	{
		return offlineDevNum;
	}

	void SetIdleDevNum(int nIdleDevNum)
	{
		idleDevNum = nIdleDevNum;
	}
	int GetIdleDevNum()
	{
		return idleDevNum;
	}

	std::string GetDomainName()
	{
		return domainName;
	}
	void SetDomainName(std::string domain)
	{
		domainName = domain;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 国标设备远程重启
typedef CGbsGbDevTeleBootReq CCuGbsGbDevTeleBootReq;
typedef CGbsGbDevTeleBootRsp CCuGbsGbDevTeleBootRsp;

// 分组划归设备
// 将指定的设备组划归给指定用户
typedef CUserDeviceGroupAddReq CCuUserDeviceGroupAddReq;
typedef CUserDeviceGroupAddRsp CCuUserDeviceGroupAddRsp;
typedef CUserDeviceGroupDelReq CCuUserDeviceGroupDelReq;
typedef CUserDeviceGroupDelRsp CCuUserDeviceGroupDelRsp;

// 将指定的设备组划归给指定用户组
typedef CUserGroupDeviceGroupAddReq CCuUserGroupDeviceGroupAddReq;
typedef CUserGroupDeviceGroupAddRsp CCuUserGroupDeviceGroupAddRsp;
typedef CUserGroupDeviceGroupDelReq CCuUserGroupDeviceGroupDelReq;
typedef CUserGroupDeviceGroupDelRsp CCuUserGroupDeviceGroupDelRsp;

// 将指定的设备组设置为自动划归组
typedef	CDeviceAutoAssignToUserSetReq			CCuDeviceAutoAssignToUserSetReq;
typedef	CDeviceAutoAssignToUserSetRsp			CCuDeviceAutoAssignToUserSetRsp;
typedef	CDeviceAutoAssignToUserQryReq			CCuDeviceAutoAssignToUserQryReq;
class CCuDeviceAutoAssignToUserQryRsp : public CCUQueryResultBase
{
public:
	CCuDeviceAutoAssignToUserQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP);
	}
	vector<CDeviceAutoAssignInfo> &GetDeviceAutoAssignInfo()
	{
		return devAutoAssignInfo;
	}
	const vector<CDeviceAutoAssignInfo> &GetDeviceAutoAssignInfo() const
	{
		return devAutoAssignInfo;
	}
	void SetDeviceAutoAssignInfo(const vector<CDeviceAutoAssignInfo> &val)
	{
		devAutoAssignInfo = val;
	}
	void push_back( CDeviceAutoAssignToUserQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
	vector<CDeviceAutoAssignInfo> devAutoAssignInfo;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef	CDeviceAutoAssignToUserGroupSetReq		CCuDeviceAutoAssignToUserGroupSetReq;
typedef	CDeviceAutoAssignToUserGroupSetRsp		CCuDeviceAutoAssignToUserGroupSetRsp;
typedef	CDeviceAutoAssignToUserGroupQryReq		CCuDeviceAutoAssignToUserGroupQryReq;
class CCuDeviceAutoAssignToUserGroupQryRsp : public CCUQueryResultBase
{
public:
	CCuDeviceAutoAssignToUserGroupQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP);
	}
	vector<CDeviceAutoAssignInfo> &GetDeviceAutoAssignInfo()
	{
		return devAutoAssignInfo;
	}
	const vector<CDeviceAutoAssignInfo> &GetDeviceAutoAssignInfo() const
	{
		return devAutoAssignInfo;
	}
	void SetDeviceAutoAssignInfo(const vector<CDeviceAutoAssignInfo> &val)
	{
		devAutoAssignInfo = val;
	}
	void push_back( CDeviceAutoAssignToUserGroupQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
	vector<CDeviceAutoAssignInfo> devAutoAssignInfo;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CTransactionProgressGetReq CCuTransactionProgressGetReq;
typedef CTransactionProgressGetRsp CCuTransactionProgressGetRsp;

typedef CGbsGbParentDomainGetReq CCuGbsGbParentDomainGetReq;
typedef CGbsGbParentDomainGetRsp CCuGbsGbParentDomainGetRsp;

typedef CGbsGbPDCatalogLoadReq CCuGbsGbPDCatalogLoadReq;
typedef CGbsGbPDCatalogLoadRsp CCuGbsGbPDCatalogLoadRsp;

typedef CCuserDataVersionGetReq CCuUserDataVersionGetReq;
typedef CCuserDataVersionGetRsp CCuUserDataVersionGetRsp;

typedef CGbsDevGrpCivilCodeGetReq CCuGbsDevGrpCivilCodeGetReq;
typedef CGbsDevGrpCivilCodeGetRsp CCuGbsDevGrpCivilCodeGetRsp;

typedef CGbsDevKdidGetReq CCuGbsDevKdidGetReq;
typedef CGbsDevKdidGetRsp CCuGbsDevKdidGetRsp;

// 云台锁定
typedef CPtzLockAddReq CCuPtzLockAddReq;
typedef CPtzLockAddRsp CCuPtzLockAddRsp;

typedef CPtzLockDelReq CCuPtzLockDelReq;
typedef CPtzLockDelRsp CCuPtzLockDelRsp;

typedef CPtzLockQryReq CCuPtzLockQryReq;
typedef CPtzLockQryRsp CCuPtzLockQryRsp;

typedef CDevTreeAddReq CCuDevTreeAddReq;
typedef CDevTreeAddRsp CCuDevTreeAddRsp;

typedef CDevTreeModReq CCuDevTreeModReq;
typedef CDevTreeModRsp CCuDevTreeModRsp;

typedef CDevTreeDelReq CCuDevTreeDelReq;
typedef CDevTreeDelRsp CCuDevTreeDelRsp;

typedef CDevTreeQryReq CCuDevTreeQryReq;
class CCuDevTreeQryRsp : public CCUQueryResultBase
{
public:
	CCuDevTreeQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVTREE_QRY_RSP);
	}
	vector<CDevTreeInfo> &GetDevTreeInfo()
	{
		return infos;
	}
	const vector<CDevTreeInfo> &GetDevTreeInfo() const
	{
		return infos;
	}
	void SetDevTreeInfo(const vector<CDevTreeInfo> &val)
	{
		infos = val;
	}
	void push_back(const CDevTreeQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
	vector<CDevTreeInfo> infos;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CDeviceGrpPathQryReq CCuDeviceGrpPathQryReq;

class CCuDeviceGrpPathQryRsp : public CCUQueryResultBase
{
public:
	CCuDeviceGrpPathQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUPPATH_QRY_RSP);
	}
	vector<CDeviceGroupInfo> &GetDeviceGroupInfo()
	{
		return infos;
	}
	const vector<CDeviceGroupInfo> &GetDeviceGroupInfo() const
	{
		return infos;
	}
	void SetDeviceGroupInfo(const vector<CDeviceGroupInfo> &val)
	{
		infos = val;
	}
	void push_back( CDeviceGrpPathQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
	vector<CDeviceGroupInfo> infos;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// for dsp
class CCuDPSSearchReq : public CEventReq
{
private:
	string keywords;
	int maxResultItemNum;
	int maxSearchTimeMSec;
public:
	CCuDPSSearchReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_DPS_SEARCH_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
		maxResultItemNum = 0;
		maxSearchTimeMSec = 0;
		keywords.clear();
	}

public:
	string& GetKeywords()
	{
		return keywords;
	}
	const string& GetKeywords() const
	{
		return keywords;
	}

	void SetKeywords(const string &strKeywords)
	{
		keywords = strKeywords;
	}
	void SetMaxResultItemNum(int nMaxResultItemNum)
	{
		maxResultItemNum = nMaxResultItemNum;
	}
	int GetMaxResultItemNum() const
	{
		return maxResultItemNum;
	}
	void SetMaxSearchTimeMSec(int nMaxSearchTimeMSec)
	{
		maxSearchTimeMSec = nMaxSearchTimeMSec;
	}
	int GetMaxSearchTimeMSec() const
	{
		return maxSearchTimeMSec;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuDPSSearchRsp : public CEventRsp
{
private:
	u32			taskNo;
	string		taskType;		// 定义在 cuidata.h #define  CUI_TASK_TYPE_xxx
public:
	CCuDPSSearchRsp()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_DPS_SEARCH_RSP);
	}

	void Clear()
	{
		taskNo = INVALID_TASKNO;
		taskType.clear();
		CEventRsp::Clear();
	}

public:
	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}

	void SetTaskType(const string &strTaskType)
	{
		taskType = strTaskType;
	}
	const string &GetTaskType() const
	{
		return taskType;
	}
	string &GetTaskType()
	{
		return taskType;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuDPSGetResultReq : public CQueryReqBase
{
private:
	u32 taskNo;
public:
	CCuDPSGetResultReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_DPS_GET_SEARCH_RESULT_REQ);
	}
	void Clear()
	{
		//CQueryReqBase::Clear();
		taskNo = INVALID_TASKNO;
	}
public:
	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuDPSGetResultRsp : public CCUQueryResultBase
{
private:
	std::vector<CDPSSearchResult> resultList;
	bool lastPage;
public:
	CCuDPSGetResultRsp()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_DPS_GET_SEARCH_RESULT_RSP);
	}
	void Clear()
	{
		lastPage = false;
	}
	std::vector<CDPSSearchResult>& GetSearchResultList()
	{
		return resultList;
	}
	const std::vector<CDPSSearchResult>& GetSearchResultList() const
	{
		return resultList;
	}
	void SetSearchResultList(const std::vector<CDPSSearchResult>&val)
	{
		resultList = val;
	}
	bool IsLastPage()
	{
		return lastPage;
	}
	void SetLastPage(bool bLastPage)
	{
		lastPage = bLastPage;
	}
	void push_back(const CDPSGetResultNtfReq &cNtf, const CQueryTask *pcQueryTask);
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuDPSGetResultByGroupReq : public CQueryReqBase
{
private:
	u32 taskNo;
	string deviceGroupId;
	int type;
public:
	CCuDPSGetResultByGroupReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_REQ);
	}
	void Clear()
	{
		taskNo = INVALID_TASKNO;
		deviceGroupId.clear();
		type = 0;
	}
public:
	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}

	void SetDevGroupId(const string &devGroupId)
	{
		deviceGroupId = devGroupId;
	}
	const string &GetDevGroupId() const
	{
		return deviceGroupId;
	}
	string &GetDevGroupId()
	{
		return deviceGroupId;
	}

	void SetType(const int &nType)
	{
		type = nType;
	}
	int GetType() const
	{
		return type;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuDPSGetResultByGroupRsp : public CCUQueryResultBase
{
private:
	std::vector<CDPSSearchResult> resultList;
	bool lastPage;
public:
	CCuDPSGetResultByGroupRsp()
	{
		event = OspExtEventDesc(CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_RSP);
	}
	std::vector<CDPSSearchResult>& GetSearchResultList()
	{
		return resultList;
	}
	const std::vector<CDPSSearchResult>& GetSearchResultList() const
	{
		return resultList;
	}
	void SetSearchResultList(const std::vector<CDPSSearchResult>&val)
	{
		resultList = val;
	}
	bool IsLastPage()
	{
		return lastPage;
	}
	void SetLastPage(bool bLastPage)
	{
		lastPage = bLastPage;
	}
	//void push_back(const CDPSGetResultNtfReq &cNtf, const CQueryTask *pcQueryTask);
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuDPSSearchStopReq : public CEventReq
{
private:
	u32 taskNo;
public:
	CCuDPSSearchStopReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_DPS_SEARCH_STOP_REQ);
	}
	void Clear()
	{
		taskNo = INVALID_TASKNO;
	}
public:
	void SetTaskNo(const u32 &nTaskNo)
	{
		taskNo = nTaskNo;
	}
	const u32 &GetTaskNo() const
	{
		return taskNo;
	}
	u32 &GetTaskNo()
	{
		return taskNo;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuDPSSearchStopRsp : public CEventRsp
{
public:
	CCuDPSSearchStopRsp()
	{
		event = OspExtEventDesc(CU_CUI_DPS_SEARCH_STOP_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CDPSGetDeviceStatusReq CCuDPSGetDeviceStatusReq;
typedef CDPSGetDeviceStatusRsp CCuDPSGetDeviceStatusRsp;

class CCuGetDevInfoAndStatusReq : public CEventReq
{
private:
	std::vector<string> devIdList;               
public:
	CCuGetDevInfoAndStatusReq()
	{
		event = OspExtEventDesc(CU_CUI_GET_DEV_INFO_AND_STATUS_REQ);
	}
	void Clear()
	{
		devIdList.clear();
	}

public:
    void SetDevIdList(const std::vector<string>& cDevIdList)
    {
        devIdList = cDevIdList;
    }
    std::vector<string>& GetDevIdList()
    {
        return devIdList;
    }
    const std::vector<string>& GetDevIdList() const
    {
        return devIdList;
    }

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TDeviceInfoAndStatus
{
public:
	TDeviceInfoAndStatus()
	{
		isDevInfoValid = false;
	};
	TDevicefullinfoAndPrivilege devInfo;
	bool isOnline;
	TPuConfig devConfig;
    TPuServiceStatus serviceStatus;
    TPuAlarmStatus alarmStatus;

	bool isDevInfoValid;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCuGetDevInfoAndStatusRsp : public CEventRsp
{
private:
	vector<TDeviceInfoAndStatus> devInfoList;
public:
	CCuGetDevInfoAndStatusRsp()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_GET_DEV_INFO_AND_STATUS_RSP);
	}
	void Clear()
	{
		devInfoList.clear();
	}    

public:
    void SetDevInfoList(const vector<TDeviceInfoAndStatus>& cDevInfoList)
    {
        devInfoList = cDevInfoList;
    }
    vector<TDeviceInfoAndStatus>& GetDevInfoList()
    {
        return devInfoList;
    }
    const vector<TDeviceInfoAndStatus>& GetDevInfoList() const
    {
        return devInfoList;
    }
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuUserDevGrpVidsrcQryReq: public CUserDevGrpVidsrcQryReq
{
public:
    CCuUserDevGrpVidsrcQryReq()
    {
        event = OspExtEventDesc(CU_CUI_GET_USER_VIDEOSRC_LIST_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

private:
    // CUserDevGrpVidsrcQryReq中的querytag字段不能用，它的构造函数会赋值，CUserDevGrpVidsrcQryReq::FromXml会利用局部变量赋值
    string queryTag2;                               

public:
    void SetQueryTag2(const string& strQueryTag2)
    {
        queryTag2 = strQueryTag2;
    }
    string& GetQueryTag2()
    {
        return queryTag2;
    }
    const string& GetQueryTag2() const
    {
        return queryTag2;
    }

};
class CCuUserDevGrpVidsrcQryRsp: public CUserDevGrpVidsrcQryNtfReq
{
public:
    CCuUserDevGrpVidsrcQryRsp()
    {
        event = OspExtEventDesc(CU_CUI_GET_USER_VIDEOSRC_LIST_RSP);
    }

private:
    int errorCode; 
    // CUserDevGrpVidsrcQryNtfReq中的设备devDomainID由cuiserver换成domainName
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetErrorCode(int nErrorCode)
    {
        errorCode = nErrorCode;
    }
    int GetErrorCode() const
    {
        return errorCode;
    }
};

class CCuBatOptReq : public CEventReq
{
private:
	string treeId;
	string groupId;
	string userName;
	string userId;
	int optType;// 操作类型 enum ECuBatOptType cuidata.h
	bool recursive; // 默认递归
	std::vector< string > paramList;	// 参数列表, 根据操作类型不同,需要不同的参数.
public:
	CCuBatOptReq()
	{
		Clear();
		event = OspExtEventDesc(CU_CUI_BATCH_OPT_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();

		userId.clear();
		userName.clear();
		treeId.clear();
		groupId.clear();
		optType = -1;
		recursive = true;
		paramList.clear();
	};

	void SetUserName(const string &strUserName)
	{
		userName = strUserName;
	}
	const string &GetUserName() const
	{
		return userName;
	}
	string &GetUserName()
	{
		return userName;
	}

	void SetUserId(const string &strUserId)
	{
		userId = strUserId;
	}
	const string &GetUserId() const
	{
		return userId;
	}
	string &GetUserId()
	{
		return userId;
	}

	void SetTreeId(const string &strTreeId)
	{
		treeId = strTreeId;
	}
	const string &GetTreeId() const
	{
		return treeId;
	}
	string &GetTreeId()
	{
		return treeId;
	}

	void SetGroupId(const string &strGroupId)
	{
		groupId = strGroupId;
	}
	const string &GetGroupId() const
	{
		return groupId;
	}
	string &GetGroupId()
	{
		return groupId;
	}

	void SetOptType(const int &nType)
	{
		optType = nType;
	}
	int GetOptType() const
	{
		return optType;
	}

	void SetRecursive(bool bRecursive)
	{
		recursive = bRecursive;
	}
	bool IsRecursive() const
	{
		return recursive;
	}

	void SetParamList(const vector<string> &strParamList)
	{
		paramList = strParamList;
	}
	const vector<string> &GetParamList() const
	{
		return paramList;
	}
	vector<string> &GetParamList()
	{
		return paramList;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CCuBatOptRsp : public CEventRsp
{
public:
	CCuBatOptRsp()
	{
		event = OspExtEventDesc(CU_CUI_BATCH_OPT_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CDevGrpVidsrcTotalnumGetReq CCuDevGrpVidsrcTotalnumGetReq;
typedef CDevGrpVidsrcTotalnumGetRsp CCuDevGrpVidsrcTotalnumGetRsp;

typedef CVidsrcInfoQryReq CCuVidsrcInfoQryReq;
class CCuVidsrcInfoQryRsp : public CCUQueryResultBase
{
private:
	std::vector<CDevVidSrcInfo> resultList;
public:
	CCuVidsrcInfoQryRsp()
	{
		event = OspExtEventDesc(TAS_VIDSRC_INFO_QRY_RSP);
	}
	std::vector<CDevVidSrcInfo>& GetSearchResultList()
	{
		return resultList;
	}
	const std::vector<CDevVidSrcInfo>& GetSearchResultList() const
	{
		return resultList;
	}
	void SetSearchResultList(const std::vector<CDevVidSrcInfo>&val)
	{
		resultList = val;
	}
	void push_back(CVidsrcInfoQryNtfReq &cNtf, const CQueryTask *pcQueryTask);
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CCuiGetAzimuthReq CCuGetAzimuthReq;
typedef CCuiGetAzimuthRsp CCuGetAzimuthRsp;


typedef CDeviceGroupDeviceQry4GbReq CCuDeviceGroupDeviceQry4GbReq;
class CCuDeviceGroupDeviceQry4GbRsp : public CCUQueryResultBase
{
public:
	CCuDeviceGroupDeviceQry4GbRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP);
	}
	vector<CDeviceGroupDeviceInfo4Gb> &GetDeviceGroupGBDeviceInfo()
	{
		return infos;
	}
	const vector<CDeviceGroupDeviceInfo4Gb> &GetDeviceGroupGBDeviceInfo() const
	{
		return infos;
	}
	void SetDeviceGroupGBDeviceInfo(const vector<CDeviceGroupDeviceInfo4Gb> &val)
	{
		infos = val;
	}
	void push_back(CDeviceGroupDeviceQry4GbNtfReq &cNtf, const CQueryTask *pcQueryTask);
private:
	vector<CDeviceGroupDeviceInfo4Gb> infos;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CPuLongOsdGetReq CCuPuLongOsdGetReq;
typedef CPuLongOsdGetRsp CCuPuLongOsdGetRsp;

typedef CPuLongOsdSetReq CCuPuLongOsdSetReq;
typedef CPuLongOsdSetRsp CCuPuLongOsdSetRsp;

typedef CDeviceMatchGbidReq CCuDeviceMatchGbidReq;
typedef CDeviceMatchGbidRsp CCuDeviceMatchGbidRsp;

typedef CRealMediaSwitchOccurTypeReq CCuRealMediaSwitchOccurTypeReq;
typedef CRealMediaSwitchOccurTypeRsp CCuRealMediaSwitchOccurTypeRsp;

typedef CDeviceGrpDevSumNumReq CCuDeviceGrpDevSumNumReq;
typedef CDeviceGrpDevSumNumRsp CCuDeviceGrpDevSumNumRsp;

typedef CDeviceGroupDeviceMod4GbReq CCuDeviceGroupDeviceMod4GbReq;
typedef CDeviceGroupDeviceMod4GbRsp CCuDeviceGroupDeviceMod4GbRsp;

typedef CDeviceGbidErrGetReq CCuDeviceGbidErrGetReq;
typedef CDeviceGbidErrGetRsp CCuDeviceGbidErrGetRsp;

typedef CGbsCatalogSelectReportStartReq CCuGbsCatalogSelectReportStartReq;
typedef CGbsCatalogSelectReportStartRsp CCuGbsCatalogSelectReportStartRsp;

typedef CDeviceGbinfoQryReq CCuDeviceGbinfoQryReq;
class CCuDeviceGbinfoQryRsp : public CCUQueryResultBase
{
public:
	CCuDeviceGbinfoQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVICE_GBINFO_QRY_RSP);
	}
private:
	vector<CDeviceFullGbInfo> infos;
public:
	void push_back(CDeviceGbinfoQryNtfReq &cNtf, const CQueryTask *pcQueryTask);

	void SetInfos(const vector<CDeviceFullGbInfo>& cInfos)
	{
		infos = cInfos;
	}
	vector<CDeviceFullGbInfo>& GetInfos()
	{
		return infos;
	}
	const vector<CDeviceFullGbInfo>& GetInfos() const
	{
		return infos;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
		
typedef CDevgrpGbinfoQryReq CCuDevgrpGbinfoQryReq;
class CCuDevgrpGbinfoQryRsp: public CCUQueryResultBase
{
public:
	CCuDevgrpGbinfoQryRsp()
	{
		event = OspExtEventDesc(TAS_DEVGRP_GBINFO_QRY_RSP);
	}
private:
	vector<TDevGrpFullGbInfos> infos;
public:
	void push_back(CDevgrpGbinfoQryNtfReq &cNtf, const CQueryTask *pcQueryTask);

	void SetInfos(const vector<TDevGrpFullGbInfos>& cInfos)
	{
		infos = cInfos;
	}
	vector<TDevGrpFullGbInfos>& GetInfos()
	{
		return infos;
	}
	const vector<TDevGrpFullGbInfos>& GetInfos() const
	{
		return infos;
	}
	
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CGbsGbdomainParentPrivilegeGetReq CCuGbsGbdomainParentPrivilegeGetReq;
typedef CGbsGbdomainParentPrivilegeGetRsp CCuGbsGbdomainParentPrivilegeGetRsp;
typedef CGbsGbdomainParentPrivilegeSetReq CCuGbsGbdomainParentPrivilegeSetReq;
typedef CGbsGbdomainParentPrivilegeSetRsp CCuGbsGbdomainParentPrivilegeSetRsp;

typedef CGbsOptionConfSetReq CCuGbsOptionConfSetReq;
typedef CGbsOptionConfSetRsp CCuGbsOptionConfSetRsp;
typedef CGbsOptionConfGetReq CCuGbsOptionConfGetReq;
typedef CGbsOptionConfGetRsp CCuGbsOptionConfGetRsp;

//安全平台 双向鉴权
class CSecurityAuthorizationFirstReq : public CEventReq
{
public:
	CSecurityAuthorizationFirstReq()
	{
		event = OspExtEventDesc(CU_CUI_SECURITY_AUTHORIZATION_FIRST_REQ);
	}

public:
	const string& GetAlgorithm() const
	{
		return algorithm;
	}
	void SetAlgorithm(const std::string &specifiedAlgorithm)
	{
		algorithm = specifiedAlgorithm;
	}

private:
	std::string algorithm;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CSecurityAuthorizationFirstRsp : public CEventRsp
{
public:
	CSecurityAuthorizationFirstRsp()
	{
		event = OspExtEventDesc(CU_CUI_SECURITY_AUTHORIZATION_FIRST_RSP);
	}

public:
	const string& GetAlgorithm() const
	{
		return algorithm;
	}
	void SetAlgorithm(const std::string &specifiedAlgorithm)
	{
		algorithm = specifiedAlgorithm;
	}

	const string& GetRandom1() const
	{
		return random1;
	}
	void SetRandom1(const std::string &randomVal)
	{
		random1 = randomVal;
	}

private:
	std::string random1;
	std::string algorithm;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CSecurityAuthorizationSecondReq : public CEventReq
{
public:
	CSecurityAuthorizationSecondReq()
	{
		event = OspExtEventDesc(CU_CUI_SECURITY_AUTHORIZATION_SECOND_REQ);
	}

public:
	const string& GetRandom1() const
	{
		return random1;
	}
	void SetRandom1(const std::string &randomVal)
	{
		random1 = randomVal;
	}
	const string& GetRandom2() const
	{
		return random2;
	}
	void SetRandom2(const std::string &randomVal)
	{
		random2 = randomVal;
	}
	const string& GetServerId() const
	{
		return serverId;
	}
	void SetServerId(const std::string &svrId)
	{
		serverId = svrId;
	}
	const string& GetSign1() const
	{
		return signature1;
	}
	void SetSign1(const std::string &sign)
	{
		signature1 = sign;
	}

	const string& GetAlgorithm() const
	{
		return algorithm;
	}
	void SetAlgorithm(const std::string &specifiedAlgorithm)
	{
		algorithm = specifiedAlgorithm;
	}

private:
	std::string random1;
	std::string random2;
	std::string serverId;
	std::string signature1;
	std::string algorithm;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CSecurityAuthorizationSecondRsp : public CEventRsp
{
public:
	CSecurityAuthorizationSecondRsp()
	{
		event = OspExtEventDesc(CU_CUI_SECURITY_AUTHORIZATION_SECOND_RSP);
	}
public:
	const string& GetRandom1() const
	{
		return random1;
	}
	void SetRandom1(const std::string &randomVal)
	{
		random1 = randomVal;
	}

	const string& GetRandom2() const
	{
		return random2;
	}
	void SetRandom2(const std::string &randomVal)
	{
		random2 = randomVal;
	}

	const string& GetSign2() const
	{
		return signature2;
	}
	void SetSign2(const std::string &sign)
	{
		signature2 = sign;
	}

	const string& GetAlgorithm() const
	{
		return algorithm;
	}
	void SetAlgorithm(const std::string &specifiedAlgorithm)
	{
		algorithm = specifiedAlgorithm;
	}

	const string& GetCrptKey() const
	{
		return cryptkey;
	}
	void SetCrptKey(const std::string &key)
	{
		cryptkey = key;
	}

private:
	std::string random1;
	std::string random2;
	std::string cryptkey;
	std::string signature2;
	std::string algorithm;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef CSecurityAuthorizationFirstReq  CCuSecurityAuthorizationFirstReq;
typedef CSecurityAuthorizationFirstRsp  CCuSecurityAuthorizationFirstRsp;
typedef CSecurityAuthorizationSecondReq CCuSecurityAuthorizationSecondReq;
typedef CSecurityAuthorizationSecondRsp CCuSecurityAuthorizationSecondRsp;


typedef CGbsCertificateUploadReq CCuGbsCertificateUploadReq;
typedef CGbsCertificateUploadRsp CCuGbsCertificateUploadRsp;
typedef CGbsCertificateGetReq CCuGbsCertificateGetReq;
typedef CGbsCertificateGetRsp CCuGbsCertificateGetRsp;

#endif




