/*****************************************************************************
ģ����      : SNMP����ģ��
�ļ���      : snmpadp.h
����ļ�    : snmpadp.cpp
�ļ�ʵ�ֹ���: 
����        : ��  ��
�汾        : V3.1  Copyright(C) 2005-2006 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2005/12/15  3.1         ��  ��      ����
******************************************************************************/ 
#ifndef _SNMPADP_NMS_H_
#define _SNMPADP_NMS_H_

#include "tnmcommon.h"
#include "snmpadp.h"

#define TNMTRAP_BGN     500
#define TNMTRAP_END     TNMTRAP_BGN + 199


#define TNMNODE_BGN     700
#define TNMNODE_END     TNMNODE_BGN + 199

//��������֪ͨ
//֪ͨ���ݣ�TTnmAgentRegReq
#define TRAP_TNM_AGENTCONNECTED     TNMTRAP_BGN
//�������֪ͨ
//֪ͨ���ݣ�
#define TRAP_TNM_AGENTDISCONNECTED  TNMTRAP_BGN + 1

//�豸ע��ɹ���ʧ�ܵķ�����Ϣ ����ҵ�������ܴ���֮�����Ϣ����
#define EV_AGENT_REG_MSG_FROM_NMS  TNMTRAP_BGN + 2


#define NODE_TNM_CALLAGENT          TNMNODE_BGN

#define AAA_REG_ALREAD  (u8)1
#define AAA_REG_NOTYET  (u8)0

//���豸��Ϣ�ṹ����
typedef struct tagSnmpAdpMsgLen
{
    u16 wCapSetLen;
    u16 wPuSysCfgLen;
}TSnmpAdpMsgLen;

//Ŀ���ַ�ṹ
typedef struct tagTnmTarget
{
    u32  dwIp;      //������
    u16  wPort;
}PACKED TTnmTarget;

//typedef struct tagTnmCallParm
//{
//    u32  dwIp;      //������
//    u16  wPort;
//    u32  dwUnitId;
//}TTnmCallParm;

//����ע��������Ϣ��
typedef struct tagTnmAgentRegReq
{
protected:
    u32 m_dwIpAddr;         //�豸IP��ַ
    u16 m_wPort;            //�豸��ʼ���ն˿�
    u8  m_byLayer;           //�����豸�Ĳ�ۺţ��ǵ����豸ʱ��Ϊ�����ֽڣ�
    u8  m_bySlot;            //
    TTnmUnitType m_tUnitType;
    s8  m_achAlias[TNM_MAX_STRING_LEN + 4];
    TTnmDeviceId m_tDevId;
    s8  m_achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1]; //�豸������
    s8  m_achBoxID[MAX_BOX_ID_LEN + 4]; //�����ʶ
    u8  m_byReqConfigFile;              //�Ƿ����������ļ�(0��������  1������)
    u8  m_byUpgradeMode;
    s8  m_achReserverd[6];

public:
    tagTnmAgentRegReq(void)
    {
        m_dwIpAddr = TNM_INVALED_IP;
        m_wPort = TNM_INVALED_PORT;
        m_byLayer = 0;
        m_bySlot = 0;
        m_tUnitType.byDeviceType = TNM_UNIT_UNKNOW;
        m_tUnitType.byServiceType = TNM_SERVICE_UNKNOW;
        memset(m_achAlias, 0, sizeof(m_achAlias));
        m_tDevId.SetNull();
        memset(m_achDevTypeName, 0, sizeof(m_achDevTypeName));
        memset(m_achBoxID, 0, sizeof(m_achBoxID));
        m_byReqConfigFile = 0;
        m_byUpgradeMode = TNM_UPGRADE_MODE_FTP;
    }
    //��������IP��ַ
    //���룺IP��ַ(������)
    void SetUnitIpAddr(u32 dwIpAddr) { m_dwIpAddr = dwIpAddr; }
    //�������IP��ַ
    //����ֵ��IP��ַ(������)
    u32  GetUnitIpAddr(void) { return m_dwIpAddr; }
    //�������������ʼ�˿�(������)
    void SetUnitPort(u16 wPort) { m_wPort = wPort; }
    //������������ʼ�˿�(������)
    u16  GetUnitPort(void) { return m_wPort; }
    //�����豸�ͺ�
    void SetUnitType(TTnmUnitType &tUnitType) { m_tUnitType = tUnitType; }
    //����豸�ͺ�
    TTnmUnitType GetUnitType(void) { return m_tUnitType; }
    //�����豸����
    void SetUnitAlias(const s8 * pchAlias)
    {
        if (NULL != pchAlias)
        {
            memcpy(m_achAlias, pchAlias, TNM_MAX_STRING_LEN);
            m_achAlias[TNM_MAX_STRING_LEN] = '\0';
        }
    }
    //����豸������
    const s8 *GetDevTypeName(void) const { return ((const s8 *)m_achDevTypeName); }
    //�����豸������
    void SetDevTypeName(const s8 *pchDevTypeName)
    {
        if (NULL != pchDevTypeName)
        {
            memcpy(m_achDevTypeName, pchDevTypeName, TNM_MAX_DEVTYPE_NAME_LEN);
            m_achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN] = '\0';
        }
    }
    //����豸����ID
    const s8 * GetBoxID(void) const { return ((const s8 *)m_achBoxID); }
    //�����豸����ID
    void SetBoxID(const s8 * pchBoxID)
    {
        if (NULL != pchBoxID)
        {
            memcpy(m_achBoxID, pchBoxID, MAX_BOX_ID_LEN);
            m_achBoxID[MAX_BOX_ID_LEN] = '\0';
        }
    }
    //����豸����
    const s8 * GetUnitAlias(void) const { return ((const s8 *)m_achAlias); }

    //�����豸ID
    void SetDeviceID(TTnmDeviceId &tDevId) { m_tDevId = tDevId; }
    //����豸ID
    TTnmDeviceId GetDeviceID(void) { return m_tDevId; }

    //���ò��
    void SetDeviceLayer(u8 byLyer) {m_byLayer = byLyer;}
    //��ò��
    u8 GetDeviceLayer(void) {return m_byLayer;}

    //���òۺ�
    void SetDeviceSlot(u8 bySlot) {m_bySlot = bySlot;}
    //��òۺ�
    u8 GetDeviceSlot(void) {return m_bySlot;}

    //�������������ļ�
    void SetReqConfigFile(u8 byReqConfigFile) {m_byReqConfigFile = byReqConfigFile;}
    //������������ļ�
    u8 GetReqConfigFile(void) {return m_byReqConfigFile;}

    //����������ʽ
    void SetUpgradeMode(u8 byUpgradeMode) { m_byUpgradeMode = byUpgradeMode; }
    //���������ʽ
    u8 GetUpgradeMode(void) { return m_byUpgradeMode; }

}PACKED TTnmAgentRegReq;

/************************************************************************/
/* A = AgentID(u32), B = SubAgentID(u16), C = CenterAgentID(u16)        */
/*                                                                      */
/************************************************************************/
//#define GET_SUBAGENTID(A)       ((u16)((A) >> 16))
//#define GET_CENTERAGENTID(A)    ((u16)((A) & 0x0000FFFF))
//
//#define MAKE_AGENTID(B, C)  (((u32)(B) << 16) + ((u32)(C) & 0x0000FFFF))
/************************************************************************/
/* A = AgentID(u32), B = SubAgentID(u16), C = CenterAgentID(u16)        */
/*                                                                      */
/************************************************************************/
#define GET_SUBAGENTID(A)       ((u16)(((A) >> 16) & 0x0FFF))   //renhouping 2006.07.31 �޸��Ӵ���ֻ��12λ��������λ��ΪЭ������λ
#define GET_CENTERAGENTID(A)    ((u16)((A) & 0x0000FFFF))

#define MAKE_AGENTID(B, C)  (((u32)(B) << 16) + ((u32)(C) & 0x0000FFFF))

#define MAKE_UDPAGTID(B, C) (((u32)((B) | 0xF000) << 16) + ((u32)(C) & 0x0000FFFF))
#define MAKE_TCPAGTID(B, C) (((u32)((B) & 0x0FFF) << 16) + ((u32)(C) & 0x0000FFFF))

/************************************************************************/
/* Manager�ص�����                                                      */
/************************************************************************/
/*=============================================================================
  ˵    ���� Manager�ص���������
  ��    ���� CTnmMsg &cTnmMsg -- �ص�����
  �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
=============================================================================*/
typedef u16 (*TTnmManagerCallBack)(CTnmMsg &cTnmMsg);

class CTnmManagerAdapter
{

public:
    CTnmManagerAdapter();
    virtual ~CTnmManagerAdapter();

    /*=============================================================================
      �� �� ���� Initialize
      ��    �ܣ� ��ʼ��Manager Adapter 
      ��    ���� TTnmAdpInitParam * ptAgentInitParam --- Manager��ʼ������
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
	u16  Initialize(TTnmAdpInitParam * ptAgentInitParam = NULL, u32 dwAdpInst = 0, u32 dwSnmpAdpAgentNum = 2000);     //��ʼ��Snmp Adapter

    /*=============================================================================
      �� �� ���� SetCommunity
      ��    �ܣ� ���ö�д��ͬ��
      ��    ���� s8 * pchCommunity --- ��ͬ��
                 u8 byCommunityType --- ��ͬ������
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  SetCommunity(s8 * pchCommunity, u8 byCommunityType);

    /*=============================================================================
      �� �� ���� SetHBTime
      ��    �ܣ� ���ö������ʱ��
      ��    ���� u16 wHb --- ��·�������(����Ϊ��λ)
                 u8 byHbNum --- �������ping��uHbNum��û��Ӧ��, �����Ͽ�
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  SetHBTime(u16 wHb, u8 byHbNum);

    /*=============================================================================
      �� �� ���� CallAgent
      ��    �ܣ� �������д���
      ��    ���� TTnmTarget & tCallTarget --- �����д���ĵ�ַ
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  CallAgent(TTnmTarget & tCallTarget);

    /*=============================================================================
      �� �� ���� GetAgentNodeValue
      ��    �ܣ� ��ѯ����ڵ�ֵ
      ��    ���� CTnmMsg & cTnmMsg --- ��ѯ����
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  GetAgentNodeValue(CTnmMsg & cTnmMsg);

    /*=============================================================================
      �� �� ���� SetAgentNodeValue
      ��    �ܣ� ���ô���ڵ�ֵ
      ��    ���� CTnmMsg & cTnmMsg --- ��������
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  SetAgentNodeValue(CTnmMsg & cTnmMsg);

    /*=============================================================================
      �� �� ���� SetAgentNodeValue
      ��    �ܣ� ���ûص�
      ��    ���� TTnmManagerCallBack tManagerCallBack --- �ص�����
                 void * pCBData --- �ص�����ָ��
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  SetManagerCallBack(TTnmManagerCallBack tManagerCallBack = NULL,
                            void * pCBData = NULL);

	u16 SetNmsStateEnable(u8 by3asReg = AAA_REG_ALREAD);

    /*=============================================================================
      �� �� ���� CloseManager
      ��    �ܣ� ����Manager�ĺ���
      ��    ���� ��
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  CloseManager(void);

protected:

private:
    CTnmSnmpAdapter * m_pcSnmpAdp;
};


#endif /* _EV_AGTSVC_H_ */

