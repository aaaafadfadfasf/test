/*========================================================================================
模块名    ：cmu
文件名    ：pusnapshottask.h
相关文件  ：
实现功能  ：前端图片抓拍事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/09/18         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _PU_SNAPSHOT_TASK_H
#define _PU_SNAPSHOT_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"


class CPuSnapshotTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
        Ntf,
    };

public:
    CPuSnapshotTask(CInstExt *pcInst);
    virtual ~CPuSnapshotTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuSnapshotTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case Rsp:
            return "Rsp";
        case Ntf:
            return "Ntf";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();
    virtual void PrintData();

public:
    //状态处理
    u32 OnWait(CMessage *const pcMsg);
    u32 OnWaitTimer();

    u32 OnRsp(CMessage *const pcMsg);
    u32 OnRspTimer();

    u32 OnNtf(CMessage *const pcMsg);
    u32 OnNtfTimer();


public:
    //业务处理 

public:

    //数据获取
    const TKDSIP_TRANS_ID GetSipTransID() const
    {
        return m_tSipTransId;
    }
    void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
    {
        m_tSipTransId = tSipTransID;
    }

private:
    TKDSIP_TRANS_ID m_tSipTransId;
    CPuPicSnapshotReq m_tReq;
    string m_strPicID;
    FILE* m_pPicFile;
};

class CPuPicQryTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
        Ntf,
        Download,
    };

public:
    CPuPicQryTask(CInstExt *pcInst);
    virtual ~CPuPicQryTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuPicQryTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case Rsp:
            return "Rsp";
        case Ntf:
            return "Ntf";
        case Download:
            return "Download";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();
    virtual void PrintData();

public:
    //状态处理
    u32 OnWait(CMessage *const pcMsg);
    u32 OnWaitTimer();

    u32 OnRsp(CMessage *const pcMsg);
    u32 OnRspTimer();

    u32 OnNtf(CMessage *const pcMsg);
    u32 OnNtfTimer();

    u32 OnDownload(CMessage *const pcMsg);
    u32 OnDownloadTimer();


public:
    //业务处理 

public:

    //数据获取
    const TKDSIP_TRANS_ID GetSipTransID() const
    {
        return m_tSipTransId;
    }
    void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
    {
        m_tSipTransId = tSipTransID;
    }

    const CPuPicQryReq& GetReq() const
    {
        return m_tReq;
    }
    CPuPicQryReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuPicQryReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    TKDSIP_TRANS_ID m_tSipTransId;
    CPuPicQryReq m_tReq;
    string m_strQryTag;
    u32 m_dwTotalNum;
    u32 m_dwRecvNum;

    CPuPicInfoList m_cPicInfoList;

    string m_strPicID;
    FILE* m_pPicFile;
};

#endif  //#ifndef _PU_SNAPSHOT_TASK_H