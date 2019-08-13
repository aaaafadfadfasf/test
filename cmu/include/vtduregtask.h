/*****************************************************************************
模块名      : cmu 
文件名      : vtduregtask.h
相关文件    : 
文件实现功能: CVtduRegTask定义
作者        : 
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2011/11/04     1.0        黄至春        创建
******************************************************************************/

#ifndef _VTDU_REG_CMU_TASK_H
#define _VTDU_REG_CMU_TASK_H

#include "loginsession.h"
#include "cmudata.h"
#include "cms/cmu/cmu_proto.h"

typedef CArrayList<u16> CVtduChnNOList;
class CSwitchTask;

class CVtduRegTask : public CLoginSession
{
public:
    enum
    {
        WaitReg = UNKNOWN_STATE+1,
        Service,
    };

public:
    CVtduRegTask(CInstExt *pcInst);
    virtual ~CVtduRegTask();

public:
    virtual const char* GetObjName() const
    {
        return "CVtduRegTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitReg:
            return "WaitReg";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == WaitReg || GetState() == Service);
    }

public:
    virtual void InitStateMachine();
    virtual void PrintData() const;

public:
    //状态处理
    u32 OnWaitReg(CMessage *const pcMsg);
    u32 OnWaitRegTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
    //业务处理
    bool DisProc(CMessage *const pcMsg);

    bool IsService() const
    {
        return (GetState() == Service);
    }

    //bool ProcVtduAddrAssignRsp(CMessage *const pcMsg);
    //bool ProcVtduAddSwitchRsp(CMessage *const pcMsg);
    //bool ProcVtduPunchResultNtfReq(CMessage *const pcMsg);
    //bool ProcVtduRemoveSwitchRsp(CMessage *const pcMsg);

    //bool VtduAddrAssignReq(const TMediaTransChannelList&  tMediaTransChnList);
    //bool VtduAddSwitchReq(const TSwitchChannel& tSwitchChannel);
    //bool VtduRemoveSwitchReq(const TSwitchSsnID& tSwitchSsnID);

	//通知所有的CUI映射表信息
	void NotifyAllCuiMappingTable();

public:
    /////////////////////////////////////////////////////////////
    //属性成员访问接口
    ////////////////////////////////////////////////////////////
    inline const TSipURI GetDevUri() const
    {
        return TSipURI(m_cVtduRegReq.GetDevUri());
    }

    //地址列表
    inline const CIpList& GetIpAddrList() const
    {
        return m_cVtduRegReq.GetIpAddrList();
    }
    inline CIpList& GetIpAddrList()
    {
        return m_cVtduRegReq.GetIpAddrList();
    }

    //码流交换起始端口
    inline u16 GetMediaStartPort() const
    {
        return m_cVtduRegReq.GetMediaStartPort();
    }

    //按源转发端口
    inline u16 GetMediaM2PPort() const
    {
        return m_cVtduRegReq.GetMediaM2PPort();
    }

    //最大交换路数
    inline u32 GetMaxSwitchNum() const
    {
        return m_cVtduRegReq.GetMaxSwitchNum();
    }

    //最大接收组
    inline int GetMaxRcvGroup() const
    {
        return m_cVtduRegReq.GetMaxRcvGroup();
    }

    //每组最大发送目标数
    inline int GetMaxSndMmbPerRcvGrp() const
    {
        return m_cVtduRegReq.GetMaxSndMmbPerRcvGrp();
    }

    //是否穿越网闸
    inline bool GetIsTravGAP() const
    {
        return m_cVtduRegReq.GetIsTravGAP();
    }

    //信任域
    inline const string& GetCreditDomainUri() const
    {
        return m_cVtduRegReq.GetCreditDomainUri();
    }
    inline string& GetCreditDomainUri()
    {
        return m_cVtduRegReq.GetCreditDomainUri();
    }


    //当前交换路数
    void AddSwitchNum(u32 dwAdd) //增加交换路数
    {
        m_dwCurSwitchNum += dwAdd;
    }

    void SubSwitchNum(u32 dwSub) //减少交换路数
    {
        if( m_dwCurSwitchNum <= dwSub )
        {
            m_dwCurSwitchNum = 0;
            return ;
        }

        m_dwCurSwitchNum -= dwSub;
    }

    void SetCurSwitchNum(u32 wSwitchNum)  //设置当前交换路数
    {
        m_dwCurSwitchNum = wSwitchNum;
    }

    u32 GetCurSwitchNum() const  //获取当前交换路数
    {
        return m_dwCurSwitchNum;
    }

    /////////////////////////////////////////////////////////////
    //操作
    ////////////////////////////////////////////////////////////
    inline s32 GetFreeSwitchNum()
    {
        return m_cVtduRegReq.GetMaxSwitchNum() - m_dwCurSwitchNum;
    }

    bool HasSwitchCapFor(u32 dwSwitchNumToAdd)
    {
        if( (s32)(m_cVtduRegReq.GetMaxSwitchNum() - m_dwCurSwitchNum) < (s32)dwSwitchNumToAdd )
        {
            CLASSLOG(CRITICAL_LEV, "VTDU[%s]总转发能力已满, MaxSwitchNum[%lu] - CurSwitchNum[%lu] < SwitchNumToAdd[%lu].\n", 
                GetDevUri().GetNOString(), m_cVtduRegReq.GetMaxSwitchNum(), m_dwCurSwitchNum, dwSwitchNumToAdd);

            return false;
        }

        return true;
    }

    u32 GeneralVtduSerialNo()
    {
        m_dwSerialNo++;

        if( 0 == m_dwSerialNo || 0xffffffff == m_dwSerialNo )
        {
            m_dwSerialNo = 1;
        }

        return m_dwSerialNo;
    }

	//网段信息
	const TNetSegmentAllInfo& GetNetSegAllInfo() const
	{
		return m_tNetSegAllInfo;
	}
	TNetSegmentAllInfo& GetNetSegAllInfo()
	{
		return m_tNetSegAllInfo;
	}
	void SetNetSegAllInfo(const TNetSegmentAllInfo& var)
	{
		m_tNetSegAllInfo = var;
	}

	//M2PPort端口映射
	const TMappingTable& GetMappingTable() const
	{
		return m_tMappingTable;
	}
	TMappingTable& GetMappingTable()
	{
		return m_tMappingTable;
	}
	void SetMappingTable(const TMappingTable& var)
	{
		m_tMappingTable = var;
	}

private:
    CVtduRegReq m_cVtduRegReq;

    u32         m_dwCurSwitchNum;       //当前码流交换路数
    time_t      m_nRegTime;             //注册时间
    u32         m_dwSerialNo;			//流水号

	TNetSegmentAllInfo m_tNetSegAllInfo;   //网段信息
	TMappingTable m_tMappingTable;      //M2PPort端口映射
};

typedef CStrMap<CLoginSession*> CVtduMap;

class CVtduList : public CVtduMap
{
public:
    void PrintData();
    u32 GetTotalSwitchNum();
    u32 GetMaxSwitchNum();
};
extern CVtduList g_cVtduList;




#endif  //#ifndef _VTDU_REG_CMU_TASK_H