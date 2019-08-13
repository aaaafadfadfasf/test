/*****************************************************************************
ģ����      : cmu 
�ļ���      : vtduregtask.h
����ļ�    : 
�ļ�ʵ�ֹ���: CVtduRegTask����
����        : 
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2011/11/04     1.0        ������        ����
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
    //״̬����
    u32 OnWaitReg(CMessage *const pcMsg);
    u32 OnWaitRegTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
    //ҵ����
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

	//֪ͨ���е�CUIӳ�����Ϣ
	void NotifyAllCuiMappingTable();

public:
    /////////////////////////////////////////////////////////////
    //���Գ�Ա���ʽӿ�
    ////////////////////////////////////////////////////////////
    inline const TSipURI GetDevUri() const
    {
        return TSipURI(m_cVtduRegReq.GetDevUri());
    }

    //��ַ�б�
    inline const CIpList& GetIpAddrList() const
    {
        return m_cVtduRegReq.GetIpAddrList();
    }
    inline CIpList& GetIpAddrList()
    {
        return m_cVtduRegReq.GetIpAddrList();
    }

    //����������ʼ�˿�
    inline u16 GetMediaStartPort() const
    {
        return m_cVtduRegReq.GetMediaStartPort();
    }

    //��Դת���˿�
    inline u16 GetMediaM2PPort() const
    {
        return m_cVtduRegReq.GetMediaM2PPort();
    }

    //��󽻻�·��
    inline u32 GetMaxSwitchNum() const
    {
        return m_cVtduRegReq.GetMaxSwitchNum();
    }

    //��������
    inline int GetMaxRcvGroup() const
    {
        return m_cVtduRegReq.GetMaxRcvGroup();
    }

    //ÿ�������Ŀ����
    inline int GetMaxSndMmbPerRcvGrp() const
    {
        return m_cVtduRegReq.GetMaxSndMmbPerRcvGrp();
    }

    //�Ƿ�Խ��բ
    inline bool GetIsTravGAP() const
    {
        return m_cVtduRegReq.GetIsTravGAP();
    }

    //������
    inline const string& GetCreditDomainUri() const
    {
        return m_cVtduRegReq.GetCreditDomainUri();
    }
    inline string& GetCreditDomainUri()
    {
        return m_cVtduRegReq.GetCreditDomainUri();
    }


    //��ǰ����·��
    void AddSwitchNum(u32 dwAdd) //���ӽ���·��
    {
        m_dwCurSwitchNum += dwAdd;
    }

    void SubSwitchNum(u32 dwSub) //���ٽ���·��
    {
        if( m_dwCurSwitchNum <= dwSub )
        {
            m_dwCurSwitchNum = 0;
            return ;
        }

        m_dwCurSwitchNum -= dwSub;
    }

    void SetCurSwitchNum(u32 wSwitchNum)  //���õ�ǰ����·��
    {
        m_dwCurSwitchNum = wSwitchNum;
    }

    u32 GetCurSwitchNum() const  //��ȡ��ǰ����·��
    {
        return m_dwCurSwitchNum;
    }

    /////////////////////////////////////////////////////////////
    //����
    ////////////////////////////////////////////////////////////
    inline s32 GetFreeSwitchNum()
    {
        return m_cVtduRegReq.GetMaxSwitchNum() - m_dwCurSwitchNum;
    }

    bool HasSwitchCapFor(u32 dwSwitchNumToAdd)
    {
        if( (s32)(m_cVtduRegReq.GetMaxSwitchNum() - m_dwCurSwitchNum) < (s32)dwSwitchNumToAdd )
        {
            CLASSLOG(CRITICAL_LEV, "VTDU[%s]��ת����������, MaxSwitchNum[%lu] - CurSwitchNum[%lu] < SwitchNumToAdd[%lu].\n", 
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

	//������Ϣ
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

	//M2PPort�˿�ӳ��
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

    u32         m_dwCurSwitchNum;       //��ǰ��������·��
    time_t      m_nRegTime;             //ע��ʱ��
    u32         m_dwSerialNo;			//��ˮ��

	TNetSegmentAllInfo m_tNetSegAllInfo;   //������Ϣ
	TMappingTable m_tMappingTable;      //M2PPort�˿�ӳ��
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