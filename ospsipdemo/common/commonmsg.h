/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : commonmsg.h
   相关文件    : 
   文件实现功能: 
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/9/28   1.0         liangli        创建
******************************************************************************/
#ifndef _COMMON_MSG_H_
#define _COMMON_MSG_H_

#include <string>
using std::string;
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cms/cmsassert.h"
#include "system/tinyxml/xml.h"

//////////////////////////////////////////////////////////////////////////
//常量定义
#define CLIENT_AID 200
#define CLIENT_SIP_LOCAL_PORT 6160
#define CLIENT_URI ("client@china")

#define SERVER_AID 201
#define SERVER_SIP_LOCAL_PORT 6161
#define SERVER_URI ("server@china")

#define PROXY_IP ("172.16.251.72")
#define LOCAL_IP ("172.16.64.95")
#define PROXY_SIP_PORT 5060

//////////////////////////////////////////////////////////////////////////
//消息定义
enum CommonMsg
{
    EV_MSG_BEGIN = OSP_EXT_USER_EVENT_BEGIN,

    EV_CLIENT_SERVER_REG_REQ,       //客户端注册请求，消息体CRegReq
    EV_SERVER_CLIENT_REG_RSP,       //服务端应答，消息体CRegRsp

    EV_CLIENT_SERVER_SEND_MSG_REQ,  //客户端注册成功后可以向服务端发送消息
    EV_SERVER_CLIENT_SEND_MSG_RSP,

    EV_CLIENT_SERVER_UNREG_REQ,     //客户端退出
    EV_SERVER_CLIENT_UNREG_RSP,

    EV_C_S_SUBSCRIBE_MSG_REQ,       //客户端订阅服务器广播消息
    EV_S_C_SUBSCRIBE_MSG_RSP,

    EV_C_S_UNSUBSCRIBE_MSG_REQ,     //客户端退订
    EV_S_C_UNSUBSCRIBE_MSG_RSP,

    EV_S_C_BROADCAST_MSG_NTF_REQ,   //服务端广播消息给所有订阅者
    EV_S_C_BROADCAST_MSG_NTF_RSP,


	EV_C_S_INVITE_REQ,              //
	EV_S_C_INVITE_RSP,
	EV_C_S_INVITE_ACK,

	EV_C_S_INVITE_BYE_REQ,          //
	EV_S_C_INVITE_BYE_RSP,

	EV_C_S_INFO_REQ,
	EV_S_C_INFO_RSP,

	EV_C_S_MESSAGE_DIALOG_REQ,
	EV_S_C_MESSAGE_DIALOG_RSP,

    EV_MSG_END,
};

inline void InitEventDesc()
{
    OSP_ADD_EVENT_DESC(EV_CLIENT_SERVER_REG_REQ);
    OSP_ADD_EVENT_DESC(EV_SERVER_CLIENT_REG_RSP);

    OSP_ADD_EVENT_DESC(EV_CLIENT_SERVER_SEND_MSG_REQ);
    OSP_ADD_EVENT_DESC(EV_SERVER_CLIENT_SEND_MSG_RSP);

    OSP_ADD_EVENT_DESC(EV_CLIENT_SERVER_UNREG_REQ);
    OSP_ADD_EVENT_DESC(EV_SERVER_CLIENT_UNREG_RSP);

    OSP_ADD_EVENT_DESC(EV_C_S_SUBSCRIBE_MSG_REQ);
    OSP_ADD_EVENT_DESC(EV_S_C_SUBSCRIBE_MSG_RSP);

    OSP_ADD_EVENT_DESC(EV_C_S_UNSUBSCRIBE_MSG_REQ);
    OSP_ADD_EVENT_DESC(EV_S_C_UNSUBSCRIBE_MSG_RSP);

    OSP_ADD_EVENT_DESC(EV_S_C_BROADCAST_MSG_NTF_REQ);
    OSP_ADD_EVENT_DESC(EV_S_C_BROADCAST_MSG_NTF_RSP);


	OSP_ADD_EVENT_DESC(EV_C_S_INVITE_REQ);
	OSP_ADD_EVENT_DESC(EV_S_C_INVITE_RSP);
	OSP_ADD_EVENT_DESC(EV_C_S_INVITE_ACK);
	OSP_ADD_EVENT_DESC(EV_C_S_INVITE_BYE_REQ);
	OSP_ADD_EVENT_DESC(EV_S_C_INVITE_BYE_RSP);

	OSP_ADD_EVENT_DESC(EV_C_S_INFO_REQ);
	OSP_ADD_EVENT_DESC(EV_S_C_INFO_RSP);

	OSP_ADD_EVENT_DESC(EV_C_S_MESSAGE_DIALOG_REQ);
	OSP_ADD_EVENT_DESC(EV_S_C_MESSAGE_DIALOG_RSP);
}

enum ErrCode
{
    EV_SUCCESS = 0,
    EV_USER_ALREADY_EXIST,
    EV_SUBSCRIBE_FAIL,
    EV_UNSUBSCRIBE,     //未订阅，却取消订阅
    EV_UNINVITE,
	EV_UNKNOWN_ERROR
};

//////////////////////////////////////////////////////////////////////////
class CEventReq
{
public:
    CEventReq()
    {
        seqNum = GenSequenceNum();
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
public:
    const std::string ToXml()const;
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const std::string& xml_string);
    void FromXml(const TiXmlNode& xmlNode);
protected:
    std::string event;
    int seqNum;
    string session;
protected:
private:
};

class CEventRsp
{
public:
    CEventRsp()
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
public:
    const std::string ToXml()const;
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const std::string& xml_string);
    void FromXml(const TiXmlNode& xmlNode);
protected:
    std::string event;
    int seqNum;
    string session;
    int errorCode;
protected:
private:
};

//结构体定义
class CRegReq : public CEventReq
{
public:
    CRegReq()
    {
        event = OspExtEventDesc(EV_CLIENT_SERVER_REG_REQ);
    }
    const std::string GetUserName() const
    {
        return userName;
    }
    void SetUserName(const std::string strUserName)
    {
        userName = strUserName;
    }
    const std::string GetPassWord() const
    {
        return passWord;
    }
    void SetPassWord(const std::string strPassWord)
    {
        passWord = strPassWord;
    }
    const std::string GetClientURI() const
    {
        return clientURI;
    }
    void SetClientURI(const std::string strClientURI)
    {
        clientURI = strClientURI;
    }
public:
    const std::string ToXml()const;
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const std::string& xml_string);
    void FromXml(const TiXmlNode& xmlNode);

protected:
    std::string userName;
    std::string passWord;
    std::string clientURI;
private:
};

class CRegRsp : public CEventRsp
{
public:
    CRegRsp()
    {
        event = OspExtEventDesc(EV_SERVER_CLIENT_REG_RSP);
        errorCode = EV_UNKNOWN_ERROR;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSendMsgReq : public CEventReq
{
public:
    CSendMsgReq()
    {
        event = OspExtEventDesc(EV_CLIENT_SERVER_SEND_MSG_REQ);
    }
    const std::string GetMsg() const
    {
        return msg;
    }
    void SetMsg(const std::string strMsg)
    {
        msg = strMsg;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string msg;
};

class CSendMsgRsp : public CEventRsp
{
public:
    CSendMsgRsp()
    {
        event = OspExtEventDesc(EV_SERVER_CLIENT_SEND_MSG_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUnRegReq : public CEventReq
{
public:
    CUnRegReq()
    {
        event = OspExtEventDesc(EV_CLIENT_SERVER_UNREG_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUnRegRsp : public CEventRsp
{
public:
    CUnRegRsp()
    {
        event = OspExtEventDesc(EV_SERVER_CLIENT_UNREG_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSubscribeReq : public CEventReq
{
public:
    CSubscribeReq()
    {
        event = OspExtEventDesc(EV_C_S_SUBSCRIBE_MSG_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CSubscribeRsp : public CEventRsp
{
public:
    CSubscribeRsp()
    {
        event = OspExtEventDesc(EV_S_C_SUBSCRIBE_MSG_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUnSubscribeReq : public CEventReq
{
public:
    CUnSubscribeReq()
    {
        event = OspExtEventDesc(EV_C_S_UNSUBSCRIBE_MSG_REQ);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CUnSubscribeRsp : public CEventRsp
{
public:
    CUnSubscribeRsp()
    {
        event = OspExtEventDesc(EV_S_C_UNSUBSCRIBE_MSG_RSP);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CBroadcastMsgNtfReq : public CEventReq
{
public:
    CBroadcastMsgNtfReq()
    {
        event = OspExtEventDesc(EV_S_C_BROADCAST_MSG_NTF_REQ);
    }
    const std::string GetMsg() const
    {
        return msg;
    }
    void SetMsg(const std::string strMsg)
    {
        msg = strMsg;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
private:
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
private:
    string msg;
};



#endif

