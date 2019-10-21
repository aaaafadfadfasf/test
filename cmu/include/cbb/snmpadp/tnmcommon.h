/*=======================================================================
模块名      : TNM
文件名      : tnmcommon.h
相关文件    :
文件实现功能 :
作者        : 李  博
版本        : V3.9  Copyright(C) 2003-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2005/12/20  0.1         李  博      创建
2006/03/03  3.9         李  博      TnmMsg调整消息子类型字段位置及保留字节减一使其4字节对齐
=======================================================================*/
#ifndef _TNMCOMMON_H_
#define _TNMCOMMON_H_

#include "osp.h"
//#include "snmpadp.h"
//#include "kdmdef.h"

#ifdef _LINUX_
#ifndef min_nms
#define min_nms(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max_nms
#define max_nms(a,b) (((a) > (b)) ? (a) : (b))
#endif
#endif

//消息长度相关
#define TNM_MSG_LEN                 0x7000  //消息长度
#define TNM_MSGHEAD_LEN             0x20    //消息头长度

#define TNM_MSG_GET                 0       //查询消息
#define TNM_MSG_SET                 1       //设置消息

//业务消息类，最大处理28K长度消息
class CTnmMsg
{
protected:
	u16 m_wSerialNO;        //流水号
    u16 m_wTimer;           //定时
    u32 m_dwEventId;        //事件号
    u32 m_dwSrcId;          //源ID
    u32 m_dwDstId;          //目的ID
    u32 m_dwAgentId;        //代理号
    u16 m_wReasonCode;      //原因码
    u16 m_wMsgBodyLen;      //消息体长度
    u8	m_bySubEvent;		//消息子类型
    u8  m_byMsgType;        //消息类型(TNM_MSG_GET、TNM_MSG_SET)
    u16 m_wReserverd;       //保留字节
	u32 m_dwUserId;      //用户ID
    u8  m_abyMsgBody[TNM_MSG_LEN-TNM_MSGHEAD_LEN];  //消息体

public:
    CTnmMsg(void);  //constructor
    CTnmMsg(u8 const * pbyMsg, u16  wMsgLen);   //constructor
    ~CTnmMsg(void); //distructor

    void SetSerialNO(u16 wSerialNO) { m_wSerialNO = htons(wSerialNO); } 
    u16  GetSerialNO(void) const { return ntohs(m_wSerialNO); }
    void SetSrcId(u32 dwSrcId){ m_dwSrcId = htonl(dwSrcId); } 
    u32  GetSrcId(void) const { return ntohl(m_dwSrcId); }
    void SetDstId(u32 dwDstId){ m_dwDstId = htonl(dwDstId); }
    u32  GetDstId(void) const { return ntohl(m_dwDstId); }
    void SetAgentId(u32 dwAgentId){ m_dwAgentId = htonl(dwAgentId); }
    u32  GetAgentId(void) const { return ntohl(m_dwAgentId); }
    void SetEventId(u32 dwEventId) { m_dwEventId = htonl(dwEventId); } 
    u32  GetEventId(void) const { return ntohl(m_dwEventId); }
    void SetTimer(u16 wTimer){ m_wTimer = htons(wTimer);} 
    u16  GetTimer(void) const { return ntohs(m_wTimer); }
    void SetReasonCode(u16 wReasonCode) { m_wReasonCode = htons(wReasonCode); } 
    u16  GetReasonCode(void) const { return ntohs(m_wReasonCode); }
    void SetMsgType(u8 byMsgType) { m_byMsgType = byMsgType; } 
    u8   GetMsgType(void) const { return m_byMsgType; }
    void SetSubEvent(u8 bySubEvent) { m_bySubEvent = bySubEvent; } 
    u8   GetSubEvent(void) const { return m_bySubEvent; }
    //消息时间
    void SetMsgTime(u32 dwTime){ m_dwAgentId = htonl(dwTime); }
    u32  GetMsgTime(void) const { return ntohl(m_dwAgentId); }
    //设置用户流水号
    //void SetUserID(u32 dwUserID){ m_dwAgentId = htonl(dwUserID); }
    //u32  GetUserID(void) const { return ntohl(m_dwAgentId); }
    //设置DB事件号
    void SetDBEventID(u16 wEventID){ m_wReserverd = htons(wEventID); }
    u16  GetDBEventID(void) const { return ntohs(m_wReserverd); }
	//用户ID
	u32  GetUserID(void) const { return ntohl(m_dwUserId); }
    void SetUserID(u32 dwUserId){ m_dwUserId = htonl(dwUserId); }

    void Init(void);
    void SetMsgBodyLen(u16 wMsgBodyLen);
    void ClearHdr(void);//消息头清零
    u16  GetMsgBodyLen(void) const;//获取消息体长度信息
    u16  GetMsgBody(u8 * pbyMsgBodyBuf, u16  wBufLen) const;//获取消息体，由用户申请BUFFER，如果过小做截断处理
    u8 const * GetMsgBody(void) const;//获取消息体指针，用户不需提供BUFFER
    void SetMsgBody(u8 const * pbyMsgBody = NULL, u16  wLen = 0);//设置消息体
    void CatMsgBody(u8 const * pbyMsgBody, u16  wLen);//添加消息体
    u16  GetTnmMsgLen(void) const;//获取整个消息长度
    u16  GetTnmMsg(u8 * pbyMsgBuf, u16  wBufLen) const;//获取整个消息，由用户申请BUFFER，如果过小做截断处理
    u8 const * GetTnmMsg(void) const;//获取整个消息指针，用户不需提供BUFFER
    void SetTnmMsg(u8 const * pbyMsg, u16  wLen);//设置整个消息
    const CTnmMsg & operator= (const CTnmMsg & CTnmMsg);//操作符重载
}
#ifndef WIN32
__attribute__((packed))
#endif
;

/*-------------------------------------------------------------------
                               CTnmMsg                              
--------------------------------------------------------------------*/

/*====================================================================
    函数名      ：CTnmMsg
    功能        ：constructor
    算法实现    ：
    引用全局变量：
    输入参数说明：
    返回值说明  ：
====================================================================*/
inline CTnmMsg::CTnmMsg(void)
{
    Init();
}

/*====================================================================
    函数名      ：CTnmMsg
    功能        ：constructor
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * const pbyMsg, 要赋值的消息指针
				  u16 wMsgLen, 要赋值的消息长度，必须大于等于16
    返回值说明  ：
====================================================================*/
inline CTnmMsg::CTnmMsg(u8 const * pbyMsg, u16 wMsgLen)
{
    Init();

    if (wMsgLen < TNM_MSGHEAD_LEN || pbyMsg == NULL)
    {
        return;
    }

    wMsgLen = min_nms(wMsgLen, TNM_MSG_LEN);
    memcpy(this, pbyMsg, wMsgLen);

    //set length
    SetMsgBodyLen(wMsgLen - TNM_MSGHEAD_LEN);
}

/*====================================================================
    函数名      ：~CTnmMsg
    功能        ：distructor
    算法实现    ：
    引用全局变量：
    输入参数说明：
    返回值说明  ：
====================================================================*/
inline CTnmMsg::~CTnmMsg(void)
{

}

/*====================================================================
    函数名      ：Init
    功能        ：BUFFER初始化
    算法实现    ：
    引用全局变量：
    输入参数说明：
    返回值说明  ：
====================================================================*/
inline void CTnmMsg::Init(void)
{
    memset(this, 0, TNM_MSG_LEN);	//清零
}

/*====================================================================
    函数名      ：ClearHdr
    功能        ：消息头清零
    算法实现    ：
    引用全局变量：
    输入参数说明：
    返回值说明  ：
====================================================================*/
inline void CTnmMsg::ClearHdr(void)
{
    u16 wBodyLen = GetMsgBodyLen();

    memset(this, 0, TNM_MSGHEAD_LEN);	//清零
    SetMsgBodyLen(wBodyLen);
}

/*====================================================================
    函数名      ：GetMsgBodyLen
    功能        ：获取消息体长度信息
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：消息体长度
====================================================================*/
inline u16 CTnmMsg::GetMsgBodyLen(void) const
{
    return(ntohs(m_wMsgBodyLen));
}

/*====================================================================
    函数名      ：SetMsgBodyLen
    功能        ：设置消息体长度信息
    算法实现    ：
    引用全局变量：
    输入参数说明：u16 wMsgBodyLen, 消息体长度
    返回值说明  ：无
====================================================================*/
inline void CTnmMsg::SetMsgBodyLen(u16 wMsgBodyLen)
{
    m_wMsgBodyLen = htons(wMsgBodyLen);
}

/*====================================================================
    函数名      ：GetMsgBody
    功能        ：获取消息体指针，用户不需提供BUFFER
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：u8 * const指针
====================================================================*/
inline u8 const * CTnmMsg::GetMsgBody(void) const
{
    return((u8 const *)m_abyMsgBody);
}

/*====================================================================
    函数名      ：GetMsgBodyLen
    功能        ：获取整个消息长度
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：整个消息长度
====================================================================*/
inline u16 CTnmMsg::GetTnmMsgLen(void) const
{
    return(GetMsgBodyLen() + TNM_MSGHEAD_LEN);
}

/*====================================================================
    函数名      ：GetMsgBody
    功能        ：获取消息体，由用户申请BUFFER，如果过小做截断处理
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * pbyMsgBodyBuf, 返回的消息体
				  u16 wBufLen, BUFFER大小
    返回值说明  ：实际返回的消息体长度
====================================================================*/
inline u16 CTnmMsg::GetMsgBody(u8 * pbyMsgBodyBuf, u16 wBufLen) const
{
    u16 wCopyLen = min_nms(GetMsgBodyLen(), wBufLen);
    memcpy(pbyMsgBodyBuf, m_abyMsgBody, wCopyLen);

    return wCopyLen;
}

/*====================================================================
    函数名      ：SetMsgBody
    功能        ：设置消息体
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * const pbyMsgBody, 传入的消息体，缺省为NULL
				  u16 wLen, 传入的消息体长度，缺省为0
    返回值说明  ：无
====================================================================*/
inline void CTnmMsg::SetMsgBody(u8 const * pbyMsgBody, u16 wLen)
{
    wLen = min_nms(wLen, TNM_MSG_LEN - TNM_MSGHEAD_LEN);
    memcpy(m_abyMsgBody, pbyMsgBody, wLen);
    SetMsgBodyLen(wLen);
}

/*====================================================================
    函数名      ：CatMsgBody
    功能        ：添加消息体
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * const pbyMsgBody, 传入的消息体，缺省为NULL
				  u16 wLen, 传入的消息体长度，缺省为0
    返回值说明  ：无
====================================================================*/
inline void CTnmMsg::CatMsgBody(u8 const * pbyMsgBody, u16 wLen)
{
    wLen = min_nms(wLen, TNM_MSG_LEN - TNM_MSGHEAD_LEN - GetMsgBodyLen());
    memcpy(m_abyMsgBody + GetMsgBodyLen(), pbyMsgBody, wLen);
    SetMsgBodyLen(GetMsgBodyLen() + wLen);
}

/*====================================================================
    函数名      ：GetMsgBody
    功能        ：获取整个消息指针，用户不需提供BUFFER
    算法实现    ：
    引用全局变量：
    输入参数说明：无
    返回值说明  ：u8 * const指针
====================================================================*/
inline u8 const * CTnmMsg::GetTnmMsg(void) const
{
    return((u8 const *)(this));
}

/*====================================================================
    函数名      ：GetTnmMsg
    功能        ：获取整个消息，由用户申请BUFFER，如果过小做截断处理
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * pbyMsgBuf, 返回的消息
				  u16 wBufLen, BUFFER大小
    返回值说明  ：实际返回的消息长度
====================================================================*/
inline u16 CTnmMsg::GetTnmMsg(u8 * pbyMsgBuf, u16 wBufLen) const
{
    wBufLen = min_nms(TNM_MSG_LEN,wBufLen);
    memcpy(pbyMsgBuf, this, wBufLen);

    return(min_nms(GetMsgBodyLen() + TNM_MSGHEAD_LEN, wBufLen));
}

/*====================================================================
    函数名      ：SetTnmMsg
    功能        ：设置整个消息
    算法实现    ：
    引用全局变量：
    输入参数说明：u8 * const pbyMsg, 传入的整个消息
				  u16 wMsgLen, 传入的消息长度
    返回值说明  ：无
====================================================================*/
inline void CTnmMsg::SetTnmMsg( u8 const * pbyMsg, u16 wMsgLen )
{
    if (wMsgLen < TNM_MSGHEAD_LEN)
    {
        OspPrintf(TRUE, FALSE, "CTnmMsg: SetTnmMsg() Exception -- invalid MsgLen!\n");
        return;
    }

    wMsgLen = min_nms(wMsgLen, TNM_MSG_LEN);
    memcpy(this, pbyMsg, wMsgLen);
    SetMsgBodyLen(wMsgLen - TNM_MSGHEAD_LEN);
}

/*====================================================================
    函数名      ：operator=
    功能        ：操作符重载
    算法实现    ：
    引用全局变量：
    输入参数说明：CTnmMsg & CTnmMsg, 赋值的消息引用
    返回值说明  ：CTnmMsg对象应用
====================================================================*/
inline const CTnmMsg & CTnmMsg::operator= (const CTnmMsg & CTnmMsg)
{
    u16	wLen = CTnmMsg.GetTnmMsgLen();
    memcpy(this, CTnmMsg.GetTnmMsg(), wLen);

    return(*this);
}




#endif

