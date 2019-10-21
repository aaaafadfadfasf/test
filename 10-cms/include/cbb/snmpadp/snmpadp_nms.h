/*****************************************************************************
模块名      : SNMP适配模块
文件名      : snmpadp.h
相关文件    : snmpadp.cpp
文件实现功能: 
作者        : 李  博
版本        : V3.1  Copyright(C) 2005-2006 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2005/12/15  3.1         李  博      创建
******************************************************************************/ 
#ifndef _SNMPADP_NMS_H_
#define _SNMPADP_NMS_H_

#include "tnmcommon.h"
#include "snmpadp.h"

#define TNMTRAP_BGN     500
#define TNMTRAP_END     TNMTRAP_BGN + 199


#define TNMNODE_BGN     700
#define TNMNODE_END     TNMNODE_BGN + 199

//代理上线通知
//通知内容：TTnmAgentRegReq
#define TRAP_TNM_AGENTCONNECTED     TNMTRAP_BGN
//代理掉线通知
//通知内容：
#define TRAP_TNM_AGENTDISCONNECTED  TNMTRAP_BGN + 1

//设备注册成功或失败的返回消息 网管业务层和网管代理之间的消息交互
#define EV_AGENT_REG_MSG_FROM_NMS  TNMTRAP_BGN + 2


#define NODE_TNM_CALLAGENT          TNMNODE_BGN

#define AAA_REG_ALREAD  (u8)1
#define AAA_REG_NOTYET  (u8)0

//老设备消息结构长度
typedef struct tagSnmpAdpMsgLen
{
    u16 wCapSetLen;
    u16 wPuSysCfgLen;
}TSnmpAdpMsgLen;

//目标地址结构
typedef struct tagTnmTarget
{
    u32  dwIp;      //网络序
    u16  wPort;
}PACKED TTnmTarget;

//typedef struct tagTnmCallParm
//{
//    u32  dwIp;      //网络序
//    u16  wPort;
//    u32  dwUnitId;
//}TTnmCallParm;

//外设注册请求消息类
typedef struct tagTnmAgentRegReq
{
protected:
    u32 m_dwIpAddr;         //设备IP地址
    u16 m_wPort;            //设备起始接收端口
    u8  m_byLayer;           //单板设备的层槽号（非单板设备时作为保留字节）
    u8  m_bySlot;            //
    TTnmUnitType m_tUnitType;
    s8  m_achAlias[TNM_MAX_STRING_LEN + 4];
    TTnmDeviceId m_tDevId;
    s8  m_achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1]; //设备类型名
    s8  m_achBoxID[MAX_BOX_ID_LEN + 4]; //机框标识
    u8  m_byReqConfigFile;              //是否请求配置文件(0：不请求  1：请求)
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
    //设置外设IP地址
    //输入：IP地址(网络序)
    void SetUnitIpAddr(u32 dwIpAddr) { m_dwIpAddr = dwIpAddr; }
    //获得外设IP地址
    //返回值：IP地址(网络序)
    u32  GetUnitIpAddr(void) { return m_dwIpAddr; }
    //设置外设接收起始端口(网络序)
    void SetUnitPort(u16 wPort) { m_wPort = wPort; }
    //获得外设接收起始端口(网络序)
    u16  GetUnitPort(void) { return m_wPort; }
    //设置设备型号
    void SetUnitType(TTnmUnitType &tUnitType) { m_tUnitType = tUnitType; }
    //获得设备型号
    TTnmUnitType GetUnitType(void) { return m_tUnitType; }
    //设置设备别名
    void SetUnitAlias(const s8 * pchAlias)
    {
        if (NULL != pchAlias)
        {
            memcpy(m_achAlias, pchAlias, TNM_MAX_STRING_LEN);
            m_achAlias[TNM_MAX_STRING_LEN] = '\0';
        }
    }
    //获得设备类型名
    const s8 *GetDevTypeName(void) const { return ((const s8 *)m_achDevTypeName); }
    //设置设备类型名
    void SetDevTypeName(const s8 *pchDevTypeName)
    {
        if (NULL != pchDevTypeName)
        {
            memcpy(m_achDevTypeName, pchDevTypeName, TNM_MAX_DEVTYPE_NAME_LEN);
            m_achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN] = '\0';
        }
    }
    //获得设备机框ID
    const s8 * GetBoxID(void) const { return ((const s8 *)m_achBoxID); }
    //设置设备机框ID
    void SetBoxID(const s8 * pchBoxID)
    {
        if (NULL != pchBoxID)
        {
            memcpy(m_achBoxID, pchBoxID, MAX_BOX_ID_LEN);
            m_achBoxID[MAX_BOX_ID_LEN] = '\0';
        }
    }
    //获得设备别名
    const s8 * GetUnitAlias(void) const { return ((const s8 *)m_achAlias); }

    //设置设备ID
    void SetDeviceID(TTnmDeviceId &tDevId) { m_tDevId = tDevId; }
    //获得设备ID
    TTnmDeviceId GetDeviceID(void) { return m_tDevId; }

    //设置层号
    void SetDeviceLayer(u8 byLyer) {m_byLayer = byLyer;}
    //获得层号
    u8 GetDeviceLayer(void) {return m_byLayer;}

    //设置槽号
    void SetDeviceSlot(u8 bySlot) {m_bySlot = bySlot;}
    //获得槽号
    u8 GetDeviceSlot(void) {return m_bySlot;}

    //设置请求配置文件
    void SetReqConfigFile(u8 byReqConfigFile) {m_byReqConfigFile = byReqConfigFile;}
    //获得请求配置文件
    u8 GetReqConfigFile(void) {return m_byReqConfigFile;}

    //设置升级方式
    void SetUpgradeMode(u8 byUpgradeMode) { m_byUpgradeMode = byUpgradeMode; }
    //获得升级方式
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
#define GET_SUBAGENTID(A)       ((u16)(((A) >> 16) & 0x0FFF))   //renhouping 2006.07.31 修改子代理只有12位，其他两位做为协议类型位
#define GET_CENTERAGENTID(A)    ((u16)((A) & 0x0000FFFF))

#define MAKE_AGENTID(B, C)  (((u32)(B) << 16) + ((u32)(C) & 0x0000FFFF))

#define MAKE_UDPAGTID(B, C) (((u32)((B) | 0xF000) << 16) + ((u32)(C) & 0x0000FFFF))
#define MAKE_TCPAGTID(B, C) (((u32)((B) & 0x0FFF) << 16) + ((u32)(C) & 0x0000FFFF))

/************************************************************************/
/* Manager回调函数                                                      */
/************************************************************************/
/*=============================================================================
  说    明： Manager回调函数定义
  参    数： CTnmMsg &cTnmMsg -- 回调内容
  返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
=============================================================================*/
typedef u16 (*TTnmManagerCallBack)(CTnmMsg &cTnmMsg);

class CTnmManagerAdapter
{

public:
    CTnmManagerAdapter();
    virtual ~CTnmManagerAdapter();

    /*=============================================================================
      函 数 名： Initialize
      功    能： 初始化Manager Adapter 
      参    数： TTnmAdpInitParam * ptAgentInitParam --- Manager初始化参数
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
	u16  Initialize(TTnmAdpInitParam * ptAgentInitParam = NULL, u32 dwAdpInst = 0, u32 dwSnmpAdpAgentNum = 2000);     //初始化Snmp Adapter

    /*=============================================================================
      函 数 名： SetCommunity
      功    能： 设置读写共同体
      参    数： s8 * pchCommunity --- 共同体
                 u8 byCommunityType --- 共同体类型
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  SetCommunity(s8 * pchCommunity, u8 byCommunityType);

    /*=============================================================================
      函 数 名： SetHBTime
      功    能： 设置断链检测时间
      参    数： u16 wHb --- 链路检测周期(以秒为单位)
                 u8 byHbNum --- 如果连续ping了uHbNum次没有应答, 主动断开
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  SetHBTime(u16 wHb, u8 byHbNum);

    /*=============================================================================
      函 数 名： CallAgent
      功    能： 主动呼叫代理
      参    数： TTnmTarget & tCallTarget --- 被呼叫代理的地址
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  CallAgent(TTnmTarget & tCallTarget);

    /*=============================================================================
      函 数 名： GetAgentNodeValue
      功    能： 查询代理节点值
      参    数： CTnmMsg & cTnmMsg --- 查询内容
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  GetAgentNodeValue(CTnmMsg & cTnmMsg);

    /*=============================================================================
      函 数 名： SetAgentNodeValue
      功    能： 设置代理节点值
      参    数： CTnmMsg & cTnmMsg --- 设置内容
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  SetAgentNodeValue(CTnmMsg & cTnmMsg);

    /*=============================================================================
      函 数 名： SetAgentNodeValue
      功    能： 设置回调
      参    数： TTnmManagerCallBack tManagerCallBack --- 回调函数
                 void * pCBData --- 回调数据指针
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  SetManagerCallBack(TTnmManagerCallBack tManagerCallBack = NULL,
                            void * pCBData = NULL);

	u16 SetNmsStateEnable(u8 by3asReg = AAA_REG_ALREAD);

    /*=============================================================================
      函 数 名： CloseManager
      功    能： 结束Manager的函数
      参    数： 无
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  CloseManager(void);

protected:

private:
    CTnmSnmpAdapter * m_pcSnmpAdp;
};


#endif /* _EV_AGTSVC_H_ */

