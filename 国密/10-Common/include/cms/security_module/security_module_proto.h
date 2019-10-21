/*****************************************************************************
   ģ����      : security_module
   �ļ���      : security_module_proto.h
   �ļ�ʵ�ֹ���: ��ȫģ����ⲿЭ�鶨��

       ����˵����xmlЭ��ʵ������tinyxml����׼��tinyxml�⣬�򼯳�ʱ���Ա���ͨ������Ŀ��΢����İ汾��
                ��osp�⣨����������ṩ�İ汾��osp��������pthread��rtϵͳ�⣩
       ����˵����
           2.0��Ŀ�飺�����а���10-Common/include/cms/security_module/security_module_proto.cpp����
           ��2.0��Ŀ�飺
               1�����̱���ű�����_NO_OSPEXT��
               2������ʵ���ͷ�ļ�����·����ȷ������������include��������
               #include "osp.h"  #include "kdvtype.h"
               #include "system/tinyxml/tinyxml.h"
               #include "system/converthelper/toxml.h"
               #include "security_module_event.h"
               3������tinyxml,osp,pthread,rt�ȿ�


   ����        : ��־��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2017/07/20  1.0         huzhiyun   ����
******************************************************************************/
#ifndef _SECURITY_MODULE_PROTO_H
#define _SECURITY_MODULE_PROTO_H
#include <string>
#include <vector>
#include "security_module_event.h"
#include "security_module_proto_dependent.h"

class CSecurityModuleSipEvent
{
public:
    virtual ~CSecurityModuleSipEvent(){}     
    virtual const std::string ToXml()const;
    virtual void FromXml(const std::string& xml_string);
protected:
    virtual void ToXml(TiXmlDocument& xmlDoc)const;
public:
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual void FromXml(const TiXmlNode& xmlNode);
};

class CSecurityModuleEventReq: public CSecurityModuleSipEvent
{
public:
    CSecurityModuleEventReq()
    {
        seqNum = InnerGenSequenceNum();
    }
    const int GetSeqNum() const
    {
        return seqNum;
    }
    void SetSeqNum(const int dwSeqNum)
    {
        seqNum = dwSeqNum;
    }
    const std::string GetEventStr() const 
    {
        return event;
    }
    const int GetEventId() const
    {
        return OspExtEventID(event.c_str());
    }
    void SetEvent(const std::string strEvent)
    {
        event = strEvent;
    }
    void SetEvent(const u16 wEvent)
    {
        event = OspExtEventDesc(wEvent);
    }
    const std::string GetSession() const
    {
        return session;
    }
    void SetSession(const std::string strSession)
    {
        session = strSession;
    }
    void Clear()
    {
        seqNum = InnerGenSequenceNum();
        session.clear();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

protected:
    std::string event;
    int seqNum;
    std::string session;
private:
    int InnerGenSequenceNum()
    {
        static unsigned int s_dwSeqGenerator=1;
        u32 dwRet = s_dwSeqGenerator;
        if ( ++s_dwSeqGenerator >= 0x1FFFFFFFU ) s_dwSeqGenerator = 1;
        return (int)dwRet;
    }
};

class CSecurityModuleEventRsp: public CSecurityModuleSipEvent
{
public:
    CSecurityModuleEventRsp()
    {
        seqNum = 0;
        errorCode = 0;
    }
    const int GetSeqNum() const
    {
        return seqNum;
    }
    void SetSeqNum(const int dwSeqNum)
    {
        seqNum = dwSeqNum;
    }
    const std::string GetEventStr() const 
    {
        return event;
    }
    const int GetEventId() const
    {
        return OspExtEventID(event.c_str());
    }
    void SetEvent(const std::string strEvent)
    {
        event = strEvent;
    }
    void SetEvent(const u16 wEvent)
    {
        event = OspExtEventDesc(wEvent);
    }
    const std::string GetSession() const
    {
        return session;
    }
    void SetSession(const std::string strSession)
    {
        session = strSession;
    }
    const int GetErrorCode() const
    {
        return errorCode;
    }
    void SetErrorCode(const int dwErrorCode) 
    {
        errorCode = dwErrorCode;
    }
    void Clear()
    {
        seqNum = 0;
        session.clear();
        errorCode = 0;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
protected:
    void ToXml(TiXmlDocument& xmlDoc)const;
public:
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
protected:
    std::string event;
    int seqNum;
    std::string session;
    int errorCode;
};


struct TSecurityModuleVkek
{
    std::string vkek;                
    std::string version;                
public:
    void SetVkek(const std::string& strVkek)
    {
        vkek = strVkek;
    }
    std::string& GetVkek()
    {
        return vkek;
    }
    const std::string& GetVkek() const
    {
        return vkek;
    }

    void SetVersion(const std::string& strVersion)
    {
        version = strVersion;
    }
    std::string& GetVersion()
    {
        return version;
    }
    const std::string& GetVersion() const
    {
        return version;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSecurityModuleNewVkekReq:public CSecurityModuleEventReq
{
public:
    CSecurityModuleNewVkekReq()
    {
        event = OspExtEventDesc(SECURITY_MODULE_NEW_VKEK_REQ);
    }
protected:
    std::string moduleId;// ��ʾ˭�����                
    std::string withId; // ��ʾvkek������˭֮��
	std::string vkekVersion; // vkek�汾��
public:
    void SetWithId(const std::string& strWithId)
    {
        withId = strWithId;
    }
    std::string& GetWithId()
    {
        return withId;
    }
    const std::string& GetWithId() const
    {
        return withId;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
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

    void SetVkekVersion(const std::string& strVkekVersion)
    {
        vkekVersion = strVkekVersion;
    }
    std::string& GetVkekVersion()
    {
        return vkekVersion;
    }
    const std::string& GetVkekVersion() const
    {
        return vkekVersion;
    }

};

class CSecurityModuleNewVkekRsp:public CSecurityModuleEventRsp
{
public:
    CSecurityModuleNewVkekRsp()
    {
        event = OspExtEventDesc(SECURITY_MODULE_NEW_VKEK_RSP);
    }
protected:
    TSecurityModuleVkek vkek;// vkek������ʹ�ü���֤����ܵ�                
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVkek(const TSecurityModuleVkek& cVkek)
    {
        vkek = cVkek;
    }
    TSecurityModuleVkek& GetVkek()
    {
        return vkek;
    }
    const TSecurityModuleVkek& GetVkek() const
    {
        return vkek;
    }

};

class CSecurityModuleVkekSaveReq:public CSecurityModuleEventReq
{
public:
    CSecurityModuleVkekSaveReq()
    {
        event = OspExtEventDesc(SECURITY_MODULE_VKEK_SAVE_REQ);
		chnid = 0;
    }
protected:
    std::string moduleId;// ��ʾ˭����� 
    std::string puid; // �豸id                 
    int chnid; // ͨ�����                 
    std::string tm_from; // vkek����ʱ�� cmstime���ַ�����cmstime��ʵ�ּ�cmstime.h
    std::vector<TSecurityModuleVkek> vkeks; // vkek������ʹ�ü���֤����ܵ�                
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
    void SetPuid(const std::string& strPuid)
    {
        puid = strPuid;
    }
    std::string& GetPuid()
    {
        return puid;
    }
    const std::string& GetPuid() const
    {
        return puid;
    }

    void SetChnid(int nChnid)
    {
        chnid = nChnid;
    }
    int GetChnid() const
    {
        return chnid;
    }

    void SetTm_from(const std::string& strTm_from)
    {
        tm_from = strTm_from;
    }
    std::string& GetTm_from()
    {
        return tm_from;
    }
    const std::string& GetTm_from() const
    {
        return tm_from;
    }

    void SetVkeks(const std::vector<TSecurityModuleVkek>& cVkeks)
    {
        vkeks = cVkeks;
    }
    std::vector<TSecurityModuleVkek>& GetVkeks()
    {
        return vkeks;
    }
    const std::vector<TSecurityModuleVkek>& GetVkeks() const
    {
        return vkeks;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSecurityModuleVkekSaveRsp:public CSecurityModuleEventRsp
{
public:
    CSecurityModuleVkekSaveRsp()
    {
        event = OspExtEventDesc(SECURITY_MODULE_VKEK_SAVE_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//��ѯ���n��vkek
class CSecurityModuleNVkekQueryReq:public CSecurityModuleEventReq
{
public:
	CSecurityModuleNVkekQueryReq()
	{
		event = OspExtEventDesc(SECURITY_MODULE_NVKEK_QUERY_REQ);
	}
protected:
	std::string puid;               // �豸id                                           
	int num;                  //��ѯ���n��vkek
public:
    void SetPuid(const std::string& strPuid)
    {
        puid = strPuid;
    }
    std::string& GetPuid()
    {
        return puid;
    }
    const std::string& GetPuid() const
    {
        return puid;
    }

    void SetNum(int nNum)
    {
        num = nNum;
    }
    int GetNum() const
    {
        return num;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CSecurityModuleNVkekQueryRsp:public CSecurityModuleEventRsp
{
public:
	CSecurityModuleNVkekQueryRsp()
	{
		event = OspExtEventDesc(SECURITY_MODULE_NVKEK_QUERY_RSP);
	}
protected:
	std::vector<TSecurityModuleVkek> vkeks;                // vkek������ʹ�ü���֤����ܵ�  
public:
    void SetVkeks(const std::vector<TSecurityModuleVkek>& cVkeks)
    {
        vkeks = cVkeks;
    }
    std::vector<TSecurityModuleVkek>& GetVkeks()
    {
        return vkeks;
    }
    const std::vector<TSecurityModuleVkek>& GetVkeks() const
    {
        return vkeks;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



class CSecurityModuleVkekQueryReq:public CSecurityModuleEventReq
{
public:
    CSecurityModuleVkekQueryReq()
    {
        event = OspExtEventDesc(SECURITY_MODULE_VKEK_QUERY_REQ);
		chnid = 0;
    }
protected:
    std::string moduleId;// ��ʾ˭��ѯ��
    std::string puid; // �豸id                 
    int chnid; // ͨ����� ��-1��ʾ�豸��                
    std::string tm_from; // ʱ�䷶Χ cmstime���ַ�����cmstime��ʵ�ּ�cmstime.h
    std::string tm_to;                
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
    void SetPuid(const std::string& strPuid)
    {
        puid = strPuid;
    }
    std::string& GetPuid()
    {
        return puid;
    }
    const std::string& GetPuid() const
    {
        return puid;
    }

    void SetChnid(int nChnid)
    {
        chnid = nChnid;
    }
    int GetChnid() const
    {
        return chnid;
    }

    void SetTm_from(const std::string& strTm_from)
    {
        tm_from = strTm_from;
    }
    std::string& GetTm_from()
    {
        return tm_from;
    }
    const std::string& GetTm_from() const
    {
        return tm_from;
    }

    void SetTm_to(const std::string& strTm_to)
    {
        tm_to = strTm_to;
    }
    std::string& GetTm_to()
    {
        return tm_to;
    }
    const std::string& GetTm_to() const
    {
        return tm_to;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSecurityModuleVkekQueryRsp:public CSecurityModuleEventRsp
{
public:
    CSecurityModuleVkekQueryRsp()
    {
        event = OspExtEventDesc(SECURITY_MODULE_VKEK_QUERY_RSP);
    }
protected:
    std::vector<TSecurityModuleVkek> vkeks; // vkek������ʹ�ü���֤����ܵ�                
public:
    void SetVkeks(const std::vector<TSecurityModuleVkek>& cVkeks)
    {
        vkeks = cVkeks;
    }
    std::vector<TSecurityModuleVkek>& GetVkeks()
    {
        return vkeks;
    }
    const std::vector<TSecurityModuleVkek>& GetVkeks() const
    {
        return vkeks;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSecurityModuleCertificateQueryReq:public CSecurityModuleEventReq
{
public:
    CSecurityModuleCertificateQueryReq()
    {
        event = OspExtEventDesc(SECURITY_MODULE_CERTIFICATE_QUERY_REQ);
    }
protected:
    std::string moduleId;// ��ʾ˭��ѯ��
    std::string id; // ֤��ӵ����id                 
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSecurityModuleCertificateQueryRsp:public CSecurityModuleEventRsp
{
public:
    CSecurityModuleCertificateQueryRsp()
    {
        event = OspExtEventDesc(SECURITY_MODULE_CERTIFICATE_QUERY_RSP);
    }
protected:
    std::string signatureCert; // ǩ��֤��                
    std::string encryptCert; // ����֤��                
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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSecurityModuleCertificateSaveReq :public CSecurityModuleEventReq
{
public:
	CSecurityModuleCertificateSaveReq()
	{
		event = OspExtEventDesc(SECURITY_MODULE_CERTIFICATE_SAVE_REQ);
		type = 0;
	}
public:
	void SetType(int nType)
	{
		type = nType;
	}
	int GetType() const
	{
		return type;
	}

	void SetOwnerid(const string& strOwnerid)
	{
		ownerid = strOwnerid;
	}
	string& GetOwnerid()
	{
		return ownerid;
	}
	const string& GetOwnerid() const
	{
		return ownerid;
	}

	void SetCert4sign(const string& strCert4sign)
	{
		cert4sign = strCert4sign;
	}
	string& GetCert4sign()
	{
		return cert4sign;
	}
	const string& GetCert4sign() const
	{
		return cert4sign;
	}

	void SetCert4encry(const string& strCert4encry)
	{
		cert4encry = strCert4encry;
	}
	string& GetCert4encry()
	{
		return cert4encry;
	}
	const string& GetCert4encry() const
	{
		return cert4encry;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	int type; // 1:�豸֤�� 2���û�֤�� ����ֵ���Ƿ�                
	string ownerid; // ֤��ӵ����id  �豸֤��ʱΪ�豸��ţ��û�֤��ʱΪ�û����                
	string cert4sign;  // ǩ��֤��PEM��ʽ����                
	string cert4encry; // ����֤��PEM��ʽ����     
};
class CSecurityModuleCertificateSaveRsp :public CSecurityModuleEventRsp
{
public:
	CSecurityModuleCertificateSaveRsp()
	{
		event = OspExtEventDesc(SECURITY_MODULE_CERTIFICATE_SAVE_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
class CSecurityModuleCertificateGetReq :public CSecurityModuleEventReq
{
public:
	CSecurityModuleCertificateGetReq()
	{
		event = OspExtEventDesc(SECURITY_MODULE_CERTIFICATE_GET_REQ);
	}
public:
	void SetOwnerid(const string& strOwnerid)
	{
		ownerid = strOwnerid;
	}
	string& GetOwnerid()
	{
		return ownerid;
	}
	const string& GetOwnerid() const
	{
		return ownerid;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string ownerid;
};
class CSecurityModuleCertificateGetRsp :public CSecurityModuleEventRsp
{
public:
	CSecurityModuleCertificateGetRsp()
	{
		event = OspExtEventDesc(SECURITY_MODULE_CERTIFICATE_GET_RSP);
	}
public:
	void SetCertContent(const string& strCertContent)
	{
		certContent = strCertContent;
	}
	string& GetOwnerid()
	{
		return certContent;
	}
	const string& GetOwnerid() const
	{
		return certContent;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	string certContent;
};
#endif // _SECURITY_MODULE_PROTO_H





