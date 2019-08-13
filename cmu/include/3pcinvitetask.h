/*****************************************************************************
模块名      : cmu 
文件名      : 3pcinvitetask.h 
相关文件    : 
文件实现功能: 交换任务类
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2012/08/02     1.0        黄至春        创建
******************************************************************************/

#ifndef _3PC_INVITE_TASK_H
#define _3PC_INVITE_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xmap.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"
#include "cmutask.h"
#include "3pctask.h"

///////////////////////////////////////////////////////////////////////////////////////
//C3PCInviteTask
///////////////////////////////////////////////////////////////////////////////////////
class C3PCInviteTask : public C3PCTask
{
public:
    enum
    {
        Idle = UNKNOWN_STATE+1,
        WaitRsp,
        WaitAck,
        Service
    };

public:
    C3PCInviteTask(CInstExt *pcInst);

    virtual ~C3PCInviteTask();

    /////////////////////////////////////////////////////////////
    //重载函数
public:
    virtual const char* GetObjName() const
    {
        return "C3PCInviteTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case WaitRsp:
            return "WaitRsp";
        case WaitAck:
            return "WaitAck";
        case Service:
            return "Service";

        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == Service);
    }


    virtual void InitStateMachine();
    void ReleaseResource();

	s32 GetSrcChnForDecoderAudioCall();				// 获取解码器呼叫源端（即解码器）的通道
	s32 GetDstChnForDecoderAudioCall();				// 获取解码器呼叫目的端（即前端IPC）的通道


	void ConstructDecoderAudioCallSwitch();		// 构造解码器呼叫的交换
	void DestructDecoderAudioCallSwitch();		// 析构解码器呼叫的交换

    /////////////////////////////////////////////////////////////
    //状态处理函数
public:
	u32 OnIdle(CMessage *const pcMsg);
	u32 OnIdleTimer();
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();
	u32 OnWaitAck(CMessage *const pcMsg);
	u32 OnWaitAckTimer();
	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

    /////////////////////////////////////////////////////////////
    //业务逻辑
public:
    CMS_RESULT ProcInvite3PCReq();
    CMS_RESULT ProcSwitchFinishCB(CMessage *const pcMsg);
    CMS_RESULT ProcInvite3PCAck(const COspSipMsg*const pcOspSipMsg);
    //CMS_RESULT ProcByeReq(COspSipMsg*const pcOspSipMsg);
    //CMS_RESULT ProcByeRsp(COspSipMsg*const pcOspSipMsg);

	void DisplayAreaInfo(const string& strAreaInfo);
	u16  GetEncVidSrcNO();
	void ProcVidSrcInfoQryRsp(CMessage *const pcMsg);
	void ProcVidSrcInfoQryNtfReq(CMessage *const pcMsg);
	void ProcVidSrcOsdSetRsp(CMessage *const pcMsg);

	string CutDevGrpGbId(const string& strOrgName);
	bool IsAllDigit(const string& str);

    /////////////////////////////////////////////////////////////
    //成员属性
public:

protected:
    CLoginSession*		        m_pDevSsnTask;
	bool                        m_bCreateDecoderAudioCall;	// 是否创建解码器呼叫交换
	TChannel                    m_tEncChn;
	TChannel                    m_tDecChn;
	UUIDString                  m_strQueryTag;
};


#endif  //#ifndef _3PC_INVITE_TASK_H