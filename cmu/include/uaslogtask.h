/*****************************************************************************
ģ����      : cmu 
�ļ���      : uaslogtask.h
����ļ�    : 
�ļ�ʵ�ֹ���: CUasLogTask����
����        : 
�汾        : vs_phoenix  Copyright(C) 2009-2016 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2016/7/6       1.0         zhou        ����
******************************************************************************/

#ifndef _UAS_LOG_TASK_H
#define _UAS_LOG_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/uas/uas_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cmucoreapp.h"
#include <queue>

using std::queue;


class CUasLogTask : public CSipTask
{
public:
    enum
    {
        WaitRsp = UNKNOWN_STATE+1,
        Service,
    };

public:
    CUasLogTask(CInstExt *pcInst);
    virtual ~CUasLogTask();

public:
    virtual const char* GetObjName() const
    {
        return "CUasLogTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitRsp:
            return "WaitRsp";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return false;
    }

public:
    virtual void InitStateMachine();
    virtual void PrintData() const;

public:
    //״̬����
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
	void AddAlarmLog(const TAlarmLogInfo& var);
	void AddDeviceLog(const TDeviceLogInfo& var);
	void AddPuLog(const TDeviceLogInfo& var);
	void AddOperateLog(const TOperateLogInfo& var);
	void SendLog();

private:
	queue<TAlarmLogInfo>   m_tAlarmLogList;   //�澯��־����
	queue<TDeviceLogInfo>  m_tDeviceLogList;  //�豸��־����
	queue<TOperateLogInfo> m_tOperateLogList; //������־����
	u32                    m_dwAlarmLogTop;   //�澯��־����Topֵ
	u32                    m_dwDeviceLogTop;  //�豸��־����Topֵ
	u32                    m_dwOperateLogTop; //������־����Topֵ
	string                 m_strUasUri;       //UasUri
	CBatchOperateReq       m_cBatchReq;       //������־����
	u16                    m_wReqSendedTime;  //������־�����ѷ��ʹ���
};

#endif  //#ifndef _UAS_LOG_TASK_H