/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : cuiapp.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cuiapp.h ����CUIҵ��APP
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/7   1.0         liangli        ����
******************************************************************************/
#ifndef _CUI_APP_H_
#define _CUI_APP_H_

#include "cms/ospext/ospext.h"
#include "culogintask.h"
#include "normalcutask.h"
#include "cms/cmu/vtdulib.h"
#include "sdk/redissdk/redissdk.h"
class CCmuListSubscribeTask;

#define CUI_REG_CMU_INTERVAL (3000)
#define CUIAPP_AID (202)
#define CUIAPP_DAEMON (MAKEIID(CUIAPP_AID, CInstance::DAEMON))
#define CUI_CLEAR_VTDUNATPACKET_INTERVAL (60000) // CU�˳�����Ҫ�ٴ����
#define CUI_QUERY_CMU_OFFLINE_VS_OVER_TIME (35) // s CUû�в���������Ϊ�ϴβ�ѯ����


struct TRedisSdkDiscCB
{
    EFDB_StatusCode emCode;
};

struct TRedisSdkSubCB
{
    CRedisSsChannel cSsChn;
    string          strContent;
};

void NatDetect_CB(TNatData tNatData);

class CCuAgent;
struct TRecvNode;

class CCuiInst : public CInstExt
{
public:
    //ʵ��
    enum{Idle = 0, Service, ReLogin}; // ReLogin��ʾrelogin cmu

    CCuiInst():m_pcCmuListSubscribeTask(NULL){};
    virtual ~CCuiInst()
    {
        if (NULL != m_pcCmuListSubscribeTask)
        {
            delete m_pcCmuListSubscribeTask;
            m_pcCmuListSubscribeTask = NULL;
        }
    };
public:
    virtual LPCSTR GetStrState() const;
private:
    //��Ϣ���
    virtual void DaemonEntry(CMessage* const pcMsg);
    virtual void NormalEntry(CMessage* const pcMsg){};

    void OnIdle(CMessage* const pcMsg);
    void OnService(CMessage* const pcMsg);
    void OnReLogin(CMessage* const pcMsg);

    void OnIdleCuRequestMsg(CMessage* const pcMsg);
    u32 OnServiceCuRequestMsg(CMessage* const pcMsg);

    void OnSipUADisconnect(CMessage* const pcMsg);

	void OnRedisDisconnect();

    void ProcCmuDisconnect();

	void OnRedisNtfCallBack(const string &strChnName, const string &strMsgData);

    void ProcRedisSdkCB_SubsNtf(const TRedisSdkSubCB* const ptCB);
    void ProcRedisSdkDiscCB(const TRedisSdkDiscCB* const ptCB);
    u16 Redis2CUI_ModuleState(const TRedisModuleState &tRedisModuleState);

    void SubscribeModuleStatus();
    /*====================================================================
    ������      : OnServicePlatformNtfMsg
    ����        : ����ƽ̨������֪ͨ��Ϣ, ����3as��cmu��
    �㷨ʵ��    : ���е�֪ͨ��Ϣ��Ӧ�ð���sessionID��Ϣ��CuiApp����ID�ҵ���Ӧ��Task����
    ����ȫ�ֱ���:
    �������˵��:
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2011/11/10/             liangli
    ====================================================================*/
    void OnServicePlatformNtfMsg(CMessage* const pcMsg);

    /*====================================================================
    ������      : OnUserInnerCommonMsg
    ����        : �ڲ���ӡ����
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��:
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/3/3/             liangli
    ====================================================================*/
    BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);

    template<typename CReq>
    u32 SetCuReqMsg(CReq &cReq, CSipTask *pcTask)
    {
        CMessage cCuReqMsg;
        cCuReqMsg.event = cReq.GetEventId();
        cCuReqMsg.content = (u8*)&cReq;
        return pcTask->ProcMsg(&cCuReqMsg);
    }
    void ReplyBackToCu(CEventReq &cReq, TRecvNode * ptRecvNode, s32 nErrorCode = ERR_CUI_SIP_MSG_FAIL)
    {
        INSTLOG(CUI_APP, ERROR_LEV, "ֱ�ӷ����û�[Session: %s]Ӧ��[%s--%d]��������[%d]\n", 
            cReq.GetSession().c_str(), OspExtEventDesc(cReq.GetEventId()+1).c_str(), cReq.GetEventId()+1, nErrorCode);
        CEventRsp cRsp;
        cRsp.SetSession(cReq.GetSession());
        cRsp.SetErrorCode(nErrorCode);
        cRsp.SetEvent(cReq.GetEventId()+1);
        cRsp.SetSeqNum(cReq.GetSeqNum());
        SendRspToCu<CEventRsp>(cRsp, ptRecvNode);
    }
    template<typename CCuReq>
    void CuQryReqToTAS(const char *pXmlMsgContent, CCuAgent *pcCuAgent, TRecvNode * ptRecvNode)
    {
        CMS_ASSERT(NULL != pXmlMsgContent);

        CCuReq cReq;
        cReq.FromXml(pXmlMsgContent);
        if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
        {
            ReplyBackToCu(cReq, ptRecvNode);
            return;
        }
        cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

        CQueryTask *pcCuQryReqTask = new CQueryTask(this, pcCuAgent, ptRecvNode);
        pcCuQryReqTask->InitStateMachine();
        SetCuReqMsg(cReq, pcCuQryReqTask);
    }

	template<typename CCuReq>
	void CuQryReqToCmu(const char *pXmlMsgContent, CCuAgent *pcCuAgent, TRecvNode * ptRecvNode)
	{
		CMS_ASSERT(NULL != pXmlMsgContent);

		CCuReq cReq;
		cReq.FromXml(pXmlMsgContent);
		if ( pcCuAgent->m_strSession.empty() )
		{
			ReplyBackToCu(cReq, ptRecvNode);
			return;
		}
		cReq.SetSession(pcCuAgent->m_strSession);

		CQueryTask *pcCuQryReqTask = new CQueryTask(this, pcCuAgent, ptRecvNode);
		pcCuQryReqTask->InitStateMachine();
		SetCuReqMsg(cReq, pcCuQryReqTask);
	}

    template<typename CCuReq>
    void CuSingleOperReqToTAS(const char *pXmlMsgContent, CCuAgent *pcCuAgent, TRecvNode * ptRecvNode)
    {
        CMS_ASSERT(NULL != pXmlMsgContent);

        CCuReq cReq;
        cReq.FromXml(pXmlMsgContent);
        if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
        {
            ReplyBackToCu(cReq, ptRecvNode);
            return;
        }
        //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
        cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

        CSingleInterAction *pcCuSingleOperTask = new CSingleInterAction(this, pcCuAgent, ptRecvNode);
        pcCuSingleOperTask->InitStateMachine();
        SetCuReqMsg(cReq, pcCuSingleOperTask);
    }
    template<typename CCuReq>
    void CuBatchOperReqToTAS(const char *pXmlMsgContent, CCuAgent *pcCuAgent, TRecvNode * ptRecvNode)
    {
        CMS_ASSERT(NULL != pXmlMsgContent);

        CCuReq cReq;
        cReq.FromXml(pXmlMsgContent);
        if ( pcCuAgent->m_strCuSessionIn3AS.empty() )
        {
            ReplyBackToCu(cReq, ptRecvNode);
            return;
        }
        //��Ϊ�Ƿ���3AS�ģ�sessionҪ��дCU��3AS�ĻỰID
        cReq.SetSession(pcCuAgent->m_strCuSessionIn3AS);

        CBatchInterAction *pcCuBatchOperTask = new CBatchInterAction(this, pcCuAgent, ptRecvNode);
        pcCuBatchOperTask->InitStateMachine();
        SetCuReqMsg(cReq, pcCuBatchOperTask);
    }

    template<typename CCuReq, typename COperTask>
    void CuOperReqToPe(const char *pXmlMsgContent, const string& strSession, const string& strPeUri, TRecvNode * ptRecvNode)
    {
        u32 dwRet = PROCMSG_OK;
        CMS_ASSERT(NULL != pXmlMsgContent);

        CCuReq cReq;
        cReq.FromXml(pXmlMsgContent);

        COperTask* pcTask = new COperTask(this, strSession, strPeUri, ptRecvNode);
        pcTask->InitStateMachine();
        dwRet = SetCuReqMsg(cReq, pcTask);
        if (dwRet == PROCMSG_DEL)
        {
            INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
                pcTask->GetObjName(), pcTask->GetTaskNO(),
                cReq.GetEventStr().c_str(), cReq.GetEventId());

            delete pcTask;
            pcTask = NULL;
        }
        else if (dwRet == PROCMSG_FAIL)
        {
            INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
                pcTask->GetObjName(), pcTask->GetTaskNO(),
                cReq.GetEventStr().c_str(), cReq.GetEventId());
        }
        else if (dwRet == PROCMSG_OK)
        {
            INSTLOG(CUI_APP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
                pcTask->GetObjName(), pcTask->GetTaskNO(),
                cReq.GetEventStr().c_str(), cReq.GetEventId());
        }
        else
        {
            INSTLOG(CUI_APP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
                pcTask->GetObjName(), pcTask->GetTaskNO(),
                cReq.GetEventStr().c_str(), cReq.GetEventId());
        }
    }
    void SendNtfRspToServer(CMessage* const pcMsg);
    void ClearAllTask();

    void CreateCmuListSubscribeTask(const string &strCmuSession);

public:
    string IsProvideServer(const TSipURI &tSvrURI, u32 dwCallbackTaskNO);
    void RegAllLocalServer();

    void RegServer(const string& strUri, bool bSupportNa = false); 
	string GetPeLoginStateStr(const string& strSession);
	string Get3asLoginStateStr(const string& strSession);
	bool GetDefaultIface(string& strIface) const;
	TNetSegmentAllInfo GetNetSegmentInfo(); //��ȡ��ǰ���������б�
	void UpdateRegNetSeg();                  //����ע�ᣨcmu����������Ϣ
	bool IsTvChnTaskExit(const TTvChannel& tTvChn);

	void ProcMappingTableNtfReq(CMessage* const pcMsg);

	bool GetDomainNameByDomainId(const string &strDomainId, string &strDomainName);

public:
	bool CheckCuVersionSupportive(const string& strCuVer)
	{
		bool bSupportive = false;

		if (! g_cCuiConfig.IsCheckCuVersion())
		{
			//���ò�У��CU Version������ΪУ��ͨ����
			return true;
		}

		const string strUnSupportCuVer = "1.0";
		if (strCuVer != strUnSupportCuVer)
		{
			bSupportive = true;
		}

		return bSupportive;
	}
    
public:
    void SetStunAddr(const TNetAddr& cStunAddr)
    {
        m_tStunAddr = cStunAddr;
    }
    TNetAddr& GetStunAddr()
    {
        return m_tStunAddr;
    }
    const TNetAddr& GetStunAddr() const
    {
        return m_tStunAddr;
    }
	//���񴴽�
	virtual CTask* CreateTask(u16 wEvent,CCuAgent *pcCuAgent,TRecvNode * ptRecvNode);
	//������Ϣ����ͳһ��������ķ��ؽ��
	void ProcTaskMsg(CTask* pTask, CMessage* const pcMsg);
private:
	//�Ի�û��sessionʱ���û���¼ʧ�����������־��¼ 2018/8/14 zsy
	void WriteLogWithoutLoginSession(u32 dwErrorCode, const string &userName, const string &strIp);

private:
    TNetAddr    m_tStunAddr; 
    string m_strSession4PrintCuReq; // ��ӡָ���Ự��CU����Ϊ��ʱ����ӡ
    CCmuListSubscribeTask *m_pcCmuListSubscribeTask;
	TNetSegmentAllInfo m_tCuiNetSegInfo;    //cuiģ��������Ϣ
};

typedef COspApp<CCuiInst, 1> CCuiApp;
extern CCuiApp g_cCuiApp;


#endif

