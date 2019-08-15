/*=======================================================================
ģ����      : TNM
�ļ���      : tnmcommon.h
����ļ�    :
�ļ�ʵ�ֹ��� :
����        : ��  ��
�汾        : V3.9  Copyright(C) 2003-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2005/12/20  0.1         ��  ��      ����
2006/03/03  3.9         ��  ��      TnmMsg������Ϣ�������ֶ�λ�ü������ֽڼ�һʹ��4�ֽڶ���
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

//��Ϣ�������
#define TNM_MSG_LEN                 0x7000  //��Ϣ����
#define TNM_MSGHEAD_LEN             0x20    //��Ϣͷ����

#define TNM_MSG_GET                 0       //��ѯ��Ϣ
#define TNM_MSG_SET                 1       //������Ϣ

//ҵ����Ϣ�࣬�����28K������Ϣ
class CTnmMsg
{
protected:
	u16 m_wSerialNO;        //��ˮ��
    u16 m_wTimer;           //��ʱ
    u32 m_dwEventId;        //�¼���
    u32 m_dwSrcId;          //ԴID
    u32 m_dwDstId;          //Ŀ��ID
    u32 m_dwAgentId;        //�����
    u16 m_wReasonCode;      //ԭ����
    u16 m_wMsgBodyLen;      //��Ϣ�峤��
    u8	m_bySubEvent;		//��Ϣ������
    u8  m_byMsgType;        //��Ϣ����(TNM_MSG_GET��TNM_MSG_SET)
    u16 m_wReserverd;       //�����ֽ�
	u32 m_dwUserId;      //�û�ID
    u8  m_abyMsgBody[TNM_MSG_LEN-TNM_MSGHEAD_LEN];  //��Ϣ��

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
    //��Ϣʱ��
    void SetMsgTime(u32 dwTime){ m_dwAgentId = htonl(dwTime); }
    u32  GetMsgTime(void) const { return ntohl(m_dwAgentId); }
    //�����û���ˮ��
    //void SetUserID(u32 dwUserID){ m_dwAgentId = htonl(dwUserID); }
    //u32  GetUserID(void) const { return ntohl(m_dwAgentId); }
    //����DB�¼���
    void SetDBEventID(u16 wEventID){ m_wReserverd = htons(wEventID); }
    u16  GetDBEventID(void) const { return ntohs(m_wReserverd); }
	//�û�ID
	u32  GetUserID(void) const { return ntohl(m_dwUserId); }
    void SetUserID(u32 dwUserId){ m_dwUserId = htonl(dwUserId); }

    void Init(void);
    void SetMsgBodyLen(u16 wMsgBodyLen);
    void ClearHdr(void);//��Ϣͷ����
    u16  GetMsgBodyLen(void) const;//��ȡ��Ϣ�峤����Ϣ
    u16  GetMsgBody(u8 * pbyMsgBodyBuf, u16  wBufLen) const;//��ȡ��Ϣ�壬���û�����BUFFER�������С���ضϴ���
    u8 const * GetMsgBody(void) const;//��ȡ��Ϣ��ָ�룬�û������ṩBUFFER
    void SetMsgBody(u8 const * pbyMsgBody = NULL, u16  wLen = 0);//������Ϣ��
    void CatMsgBody(u8 const * pbyMsgBody, u16  wLen);//�����Ϣ��
    u16  GetTnmMsgLen(void) const;//��ȡ������Ϣ����
    u16  GetTnmMsg(u8 * pbyMsgBuf, u16  wBufLen) const;//��ȡ������Ϣ�����û�����BUFFER�������С���ضϴ���
    u8 const * GetTnmMsg(void) const;//��ȡ������Ϣָ�룬�û������ṩBUFFER
    void SetTnmMsg(u8 const * pbyMsg, u16  wLen);//����������Ϣ
    const CTnmMsg & operator= (const CTnmMsg & CTnmMsg);//����������
}
#ifndef WIN32
__attribute__((packed))
#endif
;

/*-------------------------------------------------------------------
                               CTnmMsg                              
--------------------------------------------------------------------*/

/*====================================================================
    ������      ��CTnmMsg
    ����        ��constructor
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����
    ����ֵ˵��  ��
====================================================================*/
inline CTnmMsg::CTnmMsg(void)
{
    Init();
}

/*====================================================================
    ������      ��CTnmMsg
    ����        ��constructor
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * const pbyMsg, Ҫ��ֵ����Ϣָ��
				  u16 wMsgLen, Ҫ��ֵ����Ϣ���ȣ�������ڵ���16
    ����ֵ˵��  ��
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
    ������      ��~CTnmMsg
    ����        ��distructor
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����
    ����ֵ˵��  ��
====================================================================*/
inline CTnmMsg::~CTnmMsg(void)
{

}

/*====================================================================
    ������      ��Init
    ����        ��BUFFER��ʼ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����
    ����ֵ˵��  ��
====================================================================*/
inline void CTnmMsg::Init(void)
{
    memset(this, 0, TNM_MSG_LEN);	//����
}

/*====================================================================
    ������      ��ClearHdr
    ����        ����Ϣͷ����
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����
    ����ֵ˵��  ��
====================================================================*/
inline void CTnmMsg::ClearHdr(void)
{
    u16 wBodyLen = GetMsgBodyLen();

    memset(this, 0, TNM_MSGHEAD_LEN);	//����
    SetMsgBodyLen(wBodyLen);
}

/*====================================================================
    ������      ��GetMsgBodyLen
    ����        ����ȡ��Ϣ�峤����Ϣ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ����Ϣ�峤��
====================================================================*/
inline u16 CTnmMsg::GetMsgBodyLen(void) const
{
    return(ntohs(m_wMsgBodyLen));
}

/*====================================================================
    ������      ��SetMsgBodyLen
    ����        ��������Ϣ�峤����Ϣ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u16 wMsgBodyLen, ��Ϣ�峤��
    ����ֵ˵��  ����
====================================================================*/
inline void CTnmMsg::SetMsgBodyLen(u16 wMsgBodyLen)
{
    m_wMsgBodyLen = htons(wMsgBodyLen);
}

/*====================================================================
    ������      ��GetMsgBody
    ����        ����ȡ��Ϣ��ָ�룬�û������ṩBUFFER
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ��u8 * constָ��
====================================================================*/
inline u8 const * CTnmMsg::GetMsgBody(void) const
{
    return((u8 const *)m_abyMsgBody);
}

/*====================================================================
    ������      ��GetMsgBodyLen
    ����        ����ȡ������Ϣ����
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ��������Ϣ����
====================================================================*/
inline u16 CTnmMsg::GetTnmMsgLen(void) const
{
    return(GetMsgBodyLen() + TNM_MSGHEAD_LEN);
}

/*====================================================================
    ������      ��GetMsgBody
    ����        ����ȡ��Ϣ�壬���û�����BUFFER�������С���ضϴ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * pbyMsgBodyBuf, ���ص���Ϣ��
				  u16 wBufLen, BUFFER��С
    ����ֵ˵��  ��ʵ�ʷ��ص���Ϣ�峤��
====================================================================*/
inline u16 CTnmMsg::GetMsgBody(u8 * pbyMsgBodyBuf, u16 wBufLen) const
{
    u16 wCopyLen = min_nms(GetMsgBodyLen(), wBufLen);
    memcpy(pbyMsgBodyBuf, m_abyMsgBody, wCopyLen);

    return wCopyLen;
}

/*====================================================================
    ������      ��SetMsgBody
    ����        ��������Ϣ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * const pbyMsgBody, �������Ϣ�壬ȱʡΪNULL
				  u16 wLen, �������Ϣ�峤�ȣ�ȱʡΪ0
    ����ֵ˵��  ����
====================================================================*/
inline void CTnmMsg::SetMsgBody(u8 const * pbyMsgBody, u16 wLen)
{
    wLen = min_nms(wLen, TNM_MSG_LEN - TNM_MSGHEAD_LEN);
    memcpy(m_abyMsgBody, pbyMsgBody, wLen);
    SetMsgBodyLen(wLen);
}

/*====================================================================
    ������      ��CatMsgBody
    ����        �������Ϣ��
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * const pbyMsgBody, �������Ϣ�壬ȱʡΪNULL
				  u16 wLen, �������Ϣ�峤�ȣ�ȱʡΪ0
    ����ֵ˵��  ����
====================================================================*/
inline void CTnmMsg::CatMsgBody(u8 const * pbyMsgBody, u16 wLen)
{
    wLen = min_nms(wLen, TNM_MSG_LEN - TNM_MSGHEAD_LEN - GetMsgBodyLen());
    memcpy(m_abyMsgBody + GetMsgBodyLen(), pbyMsgBody, wLen);
    SetMsgBodyLen(GetMsgBodyLen() + wLen);
}

/*====================================================================
    ������      ��GetMsgBody
    ����        ����ȡ������Ϣָ�룬�û������ṩBUFFER
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵������
    ����ֵ˵��  ��u8 * constָ��
====================================================================*/
inline u8 const * CTnmMsg::GetTnmMsg(void) const
{
    return((u8 const *)(this));
}

/*====================================================================
    ������      ��GetTnmMsg
    ����        ����ȡ������Ϣ�����û�����BUFFER�������С���ضϴ���
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * pbyMsgBuf, ���ص���Ϣ
				  u16 wBufLen, BUFFER��С
    ����ֵ˵��  ��ʵ�ʷ��ص���Ϣ����
====================================================================*/
inline u16 CTnmMsg::GetTnmMsg(u8 * pbyMsgBuf, u16 wBufLen) const
{
    wBufLen = min_nms(TNM_MSG_LEN,wBufLen);
    memcpy(pbyMsgBuf, this, wBufLen);

    return(min_nms(GetMsgBodyLen() + TNM_MSGHEAD_LEN, wBufLen));
}

/*====================================================================
    ������      ��SetTnmMsg
    ����        ������������Ϣ
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����u8 * const pbyMsg, �����������Ϣ
				  u16 wMsgLen, �������Ϣ����
    ����ֵ˵��  ����
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
    ������      ��operator=
    ����        ������������
    �㷨ʵ��    ��
    ����ȫ�ֱ�����
    �������˵����CTnmMsg & CTnmMsg, ��ֵ����Ϣ����
    ����ֵ˵��  ��CTnmMsg����Ӧ��
====================================================================*/
inline const CTnmMsg & CTnmMsg::operator= (const CTnmMsg & CTnmMsg)
{
    u16	wLen = CTnmMsg.GetTnmMsgLen();
    memcpy(this, CTnmMsg.GetTnmMsg(), wLen);

    return(*this);
}




#endif

