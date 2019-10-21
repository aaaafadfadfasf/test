/*****************************************************************************
   模块名      : cms_struct
   文件名      : cms_struct.h
   相关文件    : 
   文件实现功能: cms_struct.h 定义CMS系统的公共结构体
   作者        : 
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人				修改内容
   2012/06/11   1.0        huangzhichun			创建
******************************************************************************/
#ifndef _CMS_STRUCT_H_
#define _CMS_STRUCT_H_

#include<vector>
using std::vector;
#include <string>
using std::string;
#include <map>
using std::map;
#include <set>
using std::set;
#include <sstream>
using std::ostream;
using std::ostringstream;
#include "kdvtype.h"
#include "cbb/osp/osp.h"
#include "cms/cms_const.h"
#include "cms/utility/uuid.h"

//使用自动协议生成
#ifndef WAIT_AUTO_PROTO
#define WAIT_AUTO_PROTO
#endif


class TiXmlDocument;
class TiXmlNode;


typedef string TIpAddr;
typedef vector<TIpAddr> CIpList;
typedef vector<string> CStrList;
typedef vector<string> CDevUriList;
typedef int TChannelID;

typedef string TNtfOpType;
#define NTF_OP_TYPE_ADD "ADD"
#define NTF_OP_TYPE_DEL "DEL"
#define NTF_OP_TYPE_MOD "MOD"


typedef string TDevType;
#define DEV_TYPE_UNKNOWN    "UNKNOWN"
#define DEV_TYPE_PROXY  "proxy"
#define DEV_TYPE_PMC    "pmc"
#define DEV_TYPE_CU     "cu"
#define DEV_TYPE_PU     "pu"

#define DEV_TYPE_PMCS   "pmcs"
#define DEV_TYPE_PMS    "pms"
#define DEV_TYPE_CMU    "cmu"
#define DEV_TYPE_3AS    "3as"
#define DEV_TYPE_CUI    "cui"
#define DEV_TYPE_PUI    "pui"
#define DEV_TYPE_PUI_GB "puigb"
#define DEV_TYPE_GBU    "gbu"
#define DEV_TYPE_VTDU   "vtdu"
#define DEV_TYPE_NRU    "nru"
#define DEV_TYPE_RCS    "rcs"
#define DEV_TYPE_TVS    "tvs"
#define DEV_TYPE_ALS    "als"
#define DEV_TYPE_UAS    "uas"
#define DEV_TYPE_MPS    "mps"
#define DEV_TYPE_STUN   "stun"
#define DEV_TYPE_GBS    "gbs"
#define DEV_TYPE_GBU    "gbu"
#define DEV_TYPE_CUI1   "cui1"
#define DEV_TYPE_GXX    "gxx"
#define DEV_TYPE_CAPS    "caps"
#define DEV_TYPE_GBU    "gbu"
#define DEV_TYPE_DPS	"dps"

inline bool IsNeedNmsAlarmDevType(const string& var)
{
	bool bRet = false;

	if (var == DEV_TYPE_PUI
		||var == DEV_TYPE_CUI
		||var == DEV_TYPE_VTDU
		||var == DEV_TYPE_RCS
		||var == DEV_TYPE_TVS
		||var == DEV_TYPE_ALS
		||var == DEV_TYPE_MPS
		||var == DEV_TYPE_UAS
		||var == DEV_TYPE_3AS
		||var == DEV_TYPE_GBS
		||var == DEV_TYPE_CUI1
		||var == DEV_TYPE_GXX
		||var == DEV_TYPE_DPS
		||var == DEV_TYPE_GBU
		||var == DEV_TYPE_PUI_GB)
	{
		bRet = true;
	}
	
	return bRet;
};

struct TNetAddr
{
private:
    TIpAddr ip;
    int     port;

public:
    TNetAddr() 
    {
        Clear();
    }

    TNetAddr(const TIpAddr& tNetIp, int dwNetPort) 
    {
        SetNetIp(tNetIp);
        SetNetPort(dwNetPort);
    }

    void Clear()
    {
        ip.clear();
        port = 0;
    }

    const TIpAddr& GetNetIp() const 
    {
        return ip;
    }

    TIpAddr& GetNetIp()
    {
        return ip;
    }

    void SetNetIp(const TIpAddr& tNetIp) 
    {
        ip = tNetIp;
    }

    unsigned long GetNetIpv4() const;
    void SetNetIpv4(u32 dwIp);

    int GetNetPort() const 
    {
        return port;
    }

    void SetNetPort(int dwNetPort) 
    {
        port = dwNetPort;
    }

public:
    friend std::ostream& operator<<(std::ostream& output, const TNetAddr& obj)
    {
        output << obj.ip << " :"<<obj.port;
        return output;
    }
    std::string ToString() const
    {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

    bool operator == (const TNetAddr& obj) const
    {
        if (this->ip == obj.ip && this->port == obj.port)
        {
            return true;
        }
        return false;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TNetAddr> TNetAddrList;

static std::ostream& operator<<(std::ostream& output, const TNetAddrList& obj)
{
    TNetAddrList::const_iterator itr = obj.begin();
    for(; itr != obj.end(); itr++)
    {
        output<<*itr<<" ";
    }

    return output;
}


struct TNetSegment
{
private:
	TIpAddr ip;                  //网段ip
	string  nicName;             //网卡名
	TIpAddr mask;                //网段掩码

public:
    void SetNetIp(const TIpAddr& strSegmentIp)
    {
        ip = strSegmentIp;
    }
    TIpAddr& GetNetIp()
    {
        return ip;
    }
    const TIpAddr& GetNetIp() const
    {
        return ip;
    }

	void SetNicName(const string& strNicName)
	{
		nicName = strNicName;
	}
	const string& GetNicName() const
	{
		return nicName;
	}
	string& GetNicName()
	{
		return nicName;
	}

    void SetNetMask(const TIpAddr& strSegmentMask)
    {
        mask = strSegmentMask;
    }
    TIpAddr& GetNetMask()
    {
        return mask;
    }
    const TIpAddr& GetNetMask() const
    {
        return mask;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
	friend std::ostream& operator<<(std::ostream& output, const TNetSegment& obj)
	{
		output << obj.ip <<"--"<<obj.nicName<<"--"<<obj.mask;
		return output;
	}
	std::string ToString() const
	{
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

	bool operator == (const TNetSegment& obj) const
	{
		if (this->ip == obj.ip && this->nicName == obj.nicName &&
			this->mask == obj.mask)
		{
			return true;
		}
		return false;
	}

	bool operator != (const TNetSegment& obj) const
	{
		if (this->ip != obj.ip || this->nicName != obj.nicName ||
			this->mask != obj.mask)
		{
			return true;
		}
		return false;
	}
};
typedef vector<TNetSegment> TNetSegmentList;

struct TNetSegmentAllInfo
{
private:
	TNetSegmentList netSegmentList;
	TIpAddr defaultRoute;
public:
	void Clear()
	{
		defaultRoute.clear();
		netSegmentList.clear();
	}
public:
	void SetNetSegmentList(const TNetSegmentList& tNetSegmentList)
	{
		netSegmentList = tNetSegmentList;
	}
	TNetSegmentList& GetNetSegmentList()
	{
		return netSegmentList;
	}
	const TNetSegmentList& GetNetSegmentList() const
	{
		return netSegmentList;
	}

	void SetDefaultRoute(const TIpAddr& strDefaultRoute)
	{
		defaultRoute = strDefaultRoute;
	}
	TIpAddr& GetDafaultRoute()
	{
		return defaultRoute;
	}
	const TIpAddr& GetDafaultRoute() const
	{
		return defaultRoute;
	}

	bool operator == (const TNetSegmentAllInfo& obj) const
	{
		if (this->defaultRoute == obj.defaultRoute && this->netSegmentList.size() == obj.netSegmentList.size())
		{
			int nNetSegSize = this->netSegmentList.size();
			for (int nPos=0;nPos<nNetSegSize;++nPos)
			{
				if (this->netSegmentList[nPos] != obj.netSegmentList[nPos])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSTUNServerInfo
{
private:
    TIpAddr ip;
    int     port;

public:
    CSTUNServerInfo()
    {
        Clear();
    }
    ~CSTUNServerInfo()
    {
        Clear();
    }

    void Clear()
    {
        ip.clear();
        port = 0;
    }

public:
    const TIpAddr& GetIp() const 
    {
        return ip;
    }

    TIpAddr& GetIp()
    {
        return ip;
    }

    void SetIp(const TIpAddr& tIp) 
    {
        ip = tIp;
    }

    int GetPort() const 
    {
        return port;
    }

    void SetPort(int dwPort) 
    {
        port = dwPort;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TChannel
{
private:
    string devUri;
    int    chnNO;    //通道有可能是编码通道，有可能是视频源，还有可能是放像通道, 要根据协议具体应用场景判断

public:
    TChannel()
    {
        Clear();
    }

	TChannel(const string& strDevUri, int nChnNO)
	{
		devUri = strDevUri;
		chnNO = nChnNO;
	}

    void Clear()
    {
        devUri.clear();
        chnNO = 0;
    }

    const string& GetDevUri() const 
    {
        return devUri;
    }

    string& GetDevUri() 
    {
        return devUri;
    }

    void SetDevUri(const string& strDevUri) 
    {
        devUri = strDevUri;
    }

    int GetChnNO() const
    {
        return chnNO;
    }

	
    void SetChnNO(int dwChnNO)
    {
        chnNO = dwChnNO;
    }

    bool operator < (const TChannel &tValue) const
    {
        if (devUri < tValue.devUri)
        {
            return true;
        }
        else if (devUri == tValue.devUri)
        {
            if (chnNO < tValue.chnNO)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    bool operator == (const TChannel& tValue) const
    {
        if (&tValue == this
            ||(tValue.devUri == devUri && tValue.chnNO == chnNO))
        {
            return true;
        }
        return false;
    }

    bool operator != (const TChannel& tValue) const
    {
        return !(operator ==(tValue));
    }

    bool IsNull() const
    {
        return devUri.empty();
    }

    bool IsValid() const
    {
        if (devUri.empty())
        {
            return false;
        }

        string::size_type strPos = devUri.find('@');
        if (strPos == string::npos)
        {
            return false;
        }

        string strUser;
        string strDomain;
        strUser.assign(devUri, 0, strPos);
        strDomain.assign(devUri, strPos+1, devUri.size()-strPos-1);

        if (strUser.empty() || strDomain.empty())
        {
            return false;
        }

        if (chnNO < 0)
        {
            return false;
        }

        return true;
    }

public:
    friend std::ostream& operator<<(std::ostream& output, const TChannel& obj)
    {
        if(obj.devUri.length() != 0)
        {
            output<<obj.devUri<<"_"<<obj.chnNO;
        }
        return output;
    }
    std::string ToString() const
    {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TChannel> TChannelList;

typedef string TCronTime;                   //用CCronTime类解析
typedef vector<TCronTime> TCronTimeList;


struct TTimeRange
{
private:
    string          startTime;
    string          endTime;

public:
    TTimeRange()
    {
        Clear();
    }

    void Clear()
    {
        startTime.clear();
        endTime.clear();
    }

public:

    //startTime
    const string& GetStartTime() const
    {
        return startTime;
    }
    string& GetStartTime()
    {
        return startTime;
    }
    void SetStartTime(const string& var)
    {
        startTime = var;
    }

    //endTime
    const string& GetEndTime() const
    {
        return endTime;
    }
    string& GetEndTime()
    {
        return endTime;
    }
    void SetEndTime(const string& var)
    {
        endTime = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TTimeRange> TTimeRangeList;

typedef string TRecToken;
struct TRecStatus
{
private:
    string recStatus;               // 录像状态
    int    lastError;               // 最后的错误
public:
    TRecStatus()
    {
        lastError = 0;
    }
    void SetRecStatus(const string& strRecStatus)
    {
        recStatus = strRecStatus;
    }
    string& GetRecStatus()
    {
        return recStatus;
    }
    const string& GetRecStatus() const
    {
        return recStatus;
    }

    void SetLastError(int nLastError)
    {
        lastError = nLastError;
    }
    int GetLastError() const
    {
        return lastError;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

/*====================================================================
类名        : CTransparentData
功能        : 业务中存在XML透传XML的情况，需要用此类型定义，该类型是一个string
              注意：XML是大小写敏感的，并且不对字符集负责
算法实现    : XML标准中提出采用CDATA字段解决透传数据，
             <![CDATA[ ... ]]> 字段内的任何字符都可以不用解析，原样传输
             tinyxml是完整支持CDATA的
引用全局变量:
输入参数说明: 建议传输可见字符集，非可见字符集请用base64编码
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/01/08/             liangli
====================================================================*/
#ifndef CMS_CTRANSPARENTDATA
#define CMS_CTRANSPARENTDATA

class CTransparentData : public std::string
{
public:
    CTransparentData()
    {

    }
    CTransparentData(const string& cOther)
    {
        assign(cOther);
    }
    CTransparentData& operator=(const string& cOther)
    {
        if (this != &cOther)
        {
            assign(cOther);
        }

        return *this;
    }
//CTransparentData 已经当做原生类型处理，不需要再添加以下接口
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
};

#endif  //#ifndef CMS_CTRANSPARENTDATA

struct TNetParam
{
private:
    string   ifName;             
    TNetAddr netAddr;             
    string   netMask;             

public:
    TNetParam() 
    {
        Clear();
    }

    void Clear()
    {
        ifName.clear();
        netAddr.Clear();
        netMask.clear();
    }
public:
    void SetIfName(const string& strIfName)
    {
        ifName = strIfName;
    }
    string& GetIfName()
    {
        return ifName;
    }
    const string& GetIfName() const
    {
        return ifName;
    }

    void SetNetAddr(const TNetAddr& cNetAddr)
    {
        netAddr = cNetAddr;
    }
    TNetAddr& GetNetAddr()
    {
        return netAddr;
    }
    const TNetAddr& GetNetAddr() const
    {
        return netAddr;
    }

    void SetNetMask(const string& strNetMask)
    {
        netMask = strNetMask;
    }
    string& GetNetMask()
    {
        return netMask;
    }
    const string& GetNetMask() const
    {
        return netMask;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TNetParam> TNetParamList;

struct TUaAddrPair
{
private:
    TNetAddr   toAddr;                                 //目的UA地址
    TNetAddr   fromAddr;                               //和目的UA可以直接通信的源UA地址

public:
    TUaAddrPair() 
    {
        Clear();
    }

    void Clear()
    {
        toAddr.Clear();
        fromAddr.Clear();
    }   
public:
    void SetToAddr(const TNetAddr& cToAddr)
    {
        toAddr = cToAddr;
    }
    TNetAddr& GetToAddr()
    {
        return toAddr;
    }
    const TNetAddr& GetToAddr() const
    {
        return toAddr;
    }

    void SetFromAddr(const TNetAddr& cFromAddr)
    {
        fromAddr = cFromAddr;
    }
    TNetAddr& GetFromAddr()
    {
        return fromAddr;
    }
    const TNetAddr& GetFromAddr() const
    {
        return fromAddr;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef map<string, TUaAddrPair> CUaRouteTable;  //key:DstUaUri, value:AddrPair

typedef vector<int> TPuVidSrcOsdRet;
enum emPuVidSrcOsdCfgType
{
	PU_VIDSRC_OSD_TYPE_NULL            = 0, //无效字幕
	PU_VIDSRC_OSD_TYPE_TIME            = 1, //时间字幕
	PU_VIDSRC_OSD_TYPE_ALARM_MOTD      = 2, //移动侦测告警字幕
	PU_VIDSRC_OSD_TYPE_ALARM_VIDEOLOST = 3, //视频源告警字幕
	PU_VIDSRC_OSD_TYPE_LOGO            = 4, //台标
	PU_VIDSRC_OSD_TYPE_3GSingle        = 5, //3G信号
	PU_VIDSRC_OSD_TYPE_BANNER          = 6, //横幅
	PU_VIDSRC_OSD_TYPE_ALARM_PIN       = 7, //并口告警字幕
	PU_VIDSRC_OSD_TYPE_ALARM_TIME      = 8, //告警时间
	PU_VIDSRC_OSD_TYPE_ALARM           = 9, //告警字幕
};
typedef int    TPuVidSrcOsdType;
typedef int    TPuVidSrcOsdId;
typedef BOOL32 TPuVidSrcOsdEnable;
typedef string TPuVidSrcOsdContent;
struct TPuVidSrcOsdCfgItem
{
private:
	TPuVidSrcOsdType    type;     //字幕类型，参见emPuVidSrcOsdCfgType
	TPuVidSrcOsdId      id;       //字幕编号，从0开始，当字幕类型为台标和横幅时，标识台标或横幅编号；若为并口告警字幕时，标识并口号
	TPuVidSrcOsdEnable  enable;   //字幕使能
	TPuVidSrcOsdContent content;  //字幕内容，最大长度128

public:
	TPuVidSrcOsdCfgItem() 
	{
		Clear();
	}

	void Clear()
	{
		type   = 0;
		id     = 0;
		enable = FALSE;
		content.clear();
	}   

public:
	void SetPuVidSrcOsdType(const TPuVidSrcOsdType& var)
	{
		type = var;
	}
	TPuVidSrcOsdType& GetPuVidSrcOsdType()
	{
		return type;
	}
	const TPuVidSrcOsdType& GetPuVidSrcOsdType() const
	{
		return type;
	}

	void SetPuVidSrcOsdId(const TPuVidSrcOsdId& var)
	{
		id = var;
	}
	TPuVidSrcOsdId& GetPuVidSrcOsdId()
	{
		return id;
	}
	const TPuVidSrcOsdId& GetPuVidSrcOsdId() const
	{
		return id;
	}

	void SetPuVidSrcOsdEnable(const TPuVidSrcOsdEnable& var)
	{
		enable = var;
	}
	TPuVidSrcOsdEnable& GetPuVidSrcOsdEnable()
	{
		return enable;
	}
	const TPuVidSrcOsdEnable& GetPuVidSrcOsdEnable() const
	{
		return enable;
	}

	void SetPuVidSrcOsdContent(const TPuVidSrcOsdContent& var)
	{
		content = var;
	}
	TPuVidSrcOsdContent& GetPuVidSrcOsdContent()
	{
		return content;
	}
	const TPuVidSrcOsdContent& GetPuVidSrcOsdContent() const
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
typedef vector<TPuVidSrcOsdCfgItem> TPuVidSrcOsdCfg;


#endif // _CMS_STRUCT_H_

