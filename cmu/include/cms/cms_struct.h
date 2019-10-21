/*****************************************************************************
   ģ����      : cms_struct
   �ļ���      : cms_struct.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cms_struct.h ����CMSϵͳ�Ĺ����ṹ��
   ����        : 
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���				�޸�����
   2012/06/11   1.0        huangzhichun			����
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

//ʹ���Զ�Э������
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
	TIpAddr ip;                  //����ip
	string  nicName;             //������
	TIpAddr mask;                //��������

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
    int    chnNO;    //ͨ���п����Ǳ���ͨ�����п�������ƵԴ�����п����Ƿ���ͨ��, Ҫ����Э�����Ӧ�ó����ж�

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

typedef string TCronTime;                   //��CCronTime�����
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
    string recStatus;               // ¼��״̬
    int    lastError;               // ���Ĵ���
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
����        : CTransparentData
����        : ҵ���д���XML͸��XML���������Ҫ�ô����Ͷ��壬��������һ��string
              ע�⣺XML�Ǵ�Сд���еģ����Ҳ����ַ�������
�㷨ʵ��    : XML��׼���������CDATA�ֶν��͸�����ݣ�
             <![CDATA[ ... ]]> �ֶ��ڵ��κ��ַ������Բ��ý�����ԭ������
             tinyxml������֧��CDATA��
����ȫ�ֱ���:
�������˵��: ���鴫��ɼ��ַ������ǿɼ��ַ�������base64����
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
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
//CTransparentData �Ѿ�����ԭ�����ʹ�������Ҫ��������½ӿ�
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
    TNetAddr   toAddr;                                 //Ŀ��UA��ַ
    TNetAddr   fromAddr;                               //��Ŀ��UA����ֱ��ͨ�ŵ�ԴUA��ַ

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
	PU_VIDSRC_OSD_TYPE_NULL            = 0, //��Ч��Ļ
	PU_VIDSRC_OSD_TYPE_TIME            = 1, //ʱ����Ļ
	PU_VIDSRC_OSD_TYPE_ALARM_MOTD      = 2, //�ƶ����澯��Ļ
	PU_VIDSRC_OSD_TYPE_ALARM_VIDEOLOST = 3, //��ƵԴ�澯��Ļ
	PU_VIDSRC_OSD_TYPE_LOGO            = 4, //̨��
	PU_VIDSRC_OSD_TYPE_3GSingle        = 5, //3G�ź�
	PU_VIDSRC_OSD_TYPE_BANNER          = 6, //���
	PU_VIDSRC_OSD_TYPE_ALARM_PIN       = 7, //���ڸ澯��Ļ
	PU_VIDSRC_OSD_TYPE_ALARM_TIME      = 8, //�澯ʱ��
	PU_VIDSRC_OSD_TYPE_ALARM           = 9, //�澯��Ļ
};
typedef int    TPuVidSrcOsdType;
typedef int    TPuVidSrcOsdId;
typedef BOOL32 TPuVidSrcOsdEnable;
typedef string TPuVidSrcOsdContent;
struct TPuVidSrcOsdCfgItem
{
private:
	TPuVidSrcOsdType    type;     //��Ļ���ͣ��μ�emPuVidSrcOsdCfgType
	TPuVidSrcOsdId      id;       //��Ļ��ţ���0��ʼ������Ļ����Ϊ̨��ͺ��ʱ����ʶ̨�������ţ���Ϊ���ڸ澯��Ļʱ����ʶ���ں�
	TPuVidSrcOsdEnable  enable;   //��Ļʹ��
	TPuVidSrcOsdContent content;  //��Ļ���ݣ���󳤶�128

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

