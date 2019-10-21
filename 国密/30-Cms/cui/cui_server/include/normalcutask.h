/*****************************************************************************
ģ����      : cui_server 
�ļ���      : normalcutask.h
����ļ�    : 
�ļ�ʵ�ֹ���: normalcutask.h ��������CU������������
����        : liangli
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2011/11/9   1.0         liangli        ����
2013/07/29              huzhiyun       �����ع�
******************************************************************************/
#ifndef _CUI_NORMAL_CU_TASK_H_
#define _CUI_NORMAL_CU_TASK_H_
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cuidata.h"
#include "cms/commonconfig/cmcfg.h"
#include "common.h"

#include "sdk/fastdbsdk/fastdb.h"
#include "sdk/fastdbsdk/keydef.h"
#include "UserStreamNumRestorer.h"

enum EServerType
{
    SERVER_3AS = 0,     //��ѯ��Ŀ���������3AS
    SERVER_CMU,
    SERVER_RCS,
    SERVER_TVWALL,
    SERVER_TYPE_NUM
};

/* req-rsp���ҵ�� */
class CSingleInterAction : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_RESPONSE_STATE,                        //�Ѿ��������󣬵ȴ�����Ӧ��
    };
public:
    CSingleInterAction(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CSingleInterAction();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CSingleInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CSingleInterAction[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());

#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
    template <typename CReq>
    u32 PostMsgReqToCMU(CReq &cReq)
    {
        const TSipURI &tDstURI = m_pcCuAgent->m_tCmuURI;
        return FakePostMsgReq(cReq, tDstURI);
    }
    template <typename CReq>
    u32 PostMsgReqTo3AS(CReq &cReq)
    {
        if ( m_pcCuAgent->IsTasOnline() )
        { //3AS������
            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());
            CMS_ASSERT( NULL != pc3acAgent );
            CMS_ASSERT( 0 == strcmp(pc3acAgent->GetObjName(),"C3acAgent") );
            const TSipURI &tDstURI = pc3acAgent->m_t3asURI;
            FakePostMsgReq(cReq, tDstURI);
        }
        else
        { //3AS����
            return PROCMSG_DEL;
        }
        return PROCMSG_OK;
    }

private:
    template <typename CReq>
    bool GetWaterLogDesc(CReq &cReq, string &strDesc)
    {
        bool bWaterMarkEnabled = false;
        if( cReq.GetWaterMarkEnabled(bWaterMarkEnabled) && bWaterMarkEnabled  )
        {
            stringstream strStrm;
            strStrm << GetLanDesc( LAN_DESC_WATER_MARK_MODIFICATION ) << " ";

            string strFontName;
            if( cReq.GetWaterMarkFont( strFontName ) )
            {
                strStrm << GetLanDesc(LAN_DESC_FONT_NAME) << ":" << strFontName << " "; 
            }

            s32 nFontSize = 0;
            if( cReq.GetWaterMarkFontSize( nFontSize ) )
            {
                strStrm << GetLanDesc(LAN_DESC_FONT_NAME) << ":" << nFontSize << " ";
            }

            s32 nRotation = 0;
            if( cReq.GetWaterMarkRotationAngle( nRotation ) )
            {
                strStrm << GetLanDesc(LAN_DESC_ROTATION) << ":" << nRotation << " ";
            }
            bool bTiled = false;
            if( cReq.GetWaterMarkTiled( bTiled ) )
            {
                strStrm << GetLanDesc(LAN_DESC_TILE) << ":" << bTiled  << " ";
            }

            int nTransparent = 0;
            if( cReq.GetWaterMarkTransparent( nTransparent ) )
            {
                strStrm << GetLanDesc(LAN_DESC_TRANSPARENCY) << ":" << nTransparent  << " ";
            }
            
            int nColor = 0;
            if( cReq.GetWaterMarkColor( nColor ) )
            {
                strStrm << GetLanDesc(LAN_DESC_COLOR) << ":" << nColor  << " ";
            }

            strDesc = strStrm.str();
            return true;
        }
        return false;
    }

public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();

    //����NotReq״̬�µ���Ϣ�ķ��ͺ���
    template<typename CReq>
    u32 OnNotReqMsg(CMessage *const pcMsg, EServerType eSendToDst = SERVER_CMU)
    {
        CMS_ASSERT(GetState() == NOT_REQUEST_STATE);

        CReq *pcReq = (CReq *)pcMsg->content;
        //1 ���������������, �������ͱ����Ƿ���������Ľӿ�����
        u32 dwRet = PROCMSG_OK;
        switch (eSendToDst)
        {
        case SERVER_3AS:
            {
                dwRet = PostMsgReqTo3AS(*pcReq);
            }
            break;
        case SERVER_CMU:
            {
                PostMsgReqToCMU(*pcReq);
            }
            break;
        default:
            {
                return PROCMSG_FAIL;
            }
            break;
        }
        if ( PROCMSG_DEL == dwRet )
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]��������[%s--%d]��TAS�����ߣ�ֱ�Ӿܾ�����\n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                pcReq->GetEventStr().c_str(), pcReq->GetEventId());
            CMS_ASSERT(NULL != m_ptRecvNode);
            
            CEventRsp cRsp;
            cRsp.SetSeqNum(pcReq->GetSeqNum());
            cRsp.SetSession(pcReq->GetSession());
            cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
            SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
            NextState(NOT_REQUEST_STATE);
            delete this;
            return dwRet;
        }

        m_strReq = pcReq->ToXml();

        //2 ת��״̬���ȴ�Ӧ��
        NextState(WAIT_RESPONSE_STATE);
        //3 ���ó�ʱ��Ϣ����
        
        return PROCMSG_OK;
    }

    string GetOptLogDesc(const string& Opt, const string& OptDevInfo, s32 dwErrorCode);
    string GetOptLogDesc(const string& OptType,const string& Type, const string& OptDevInfo, s32 dwErrorCode);

    void WriteLog(const string& strDomain, TOperateType enOprType,
        u32 dwErrorCode, const string& strLogDesc, const string &strDevUri=string());

    void WriteLog(const TChannel& tDevChn, const u32 dwErrorCode, const u32 dwEvent, const string& strDomainName);
    template<typename CRsp>
    void GetRspMsg(CRsp& cRsp, CMessage *const pcMsg)
    {
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        pcOspSipMsg->GetMsgBody(cRsp);
    }

    string GetErrorCode(u32 dwErrorCode);
    void WriteAbnormalLog(u32 dwErrorCode);

    template<typename CRsp>
    u32 OnWaitRspMsg(CRsp& cRsp)
    {
        CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

//         COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
//         CRsp cRsp;
//         pcOspSipMsg->GetMsgBody(cRsp);
        SendRspToCu<CRsp>(cRsp, m_ptRecvNode);
        //�������, ����֮
        NextState(NOT_REQUEST_STATE);
        delete this;
        return PROCMSG_OK;
    }
public:
    PTRecvNode m_ptRecvNode;
    string m_strReq;
    CCuAgent *m_pcCuAgent;
    string m_strLogDesc;
};
template<typename CMapType, typename CKeyType>
class CRspMarks
{
public:
    CRspMarks()
    {

    }
    void SetRspMark(const CKeyType &tKey, const bool bHasRsp)
    {
        bool *pbRsp = m_tMarks.Find(tKey);
        if ( pbRsp )
        {
            *pbRsp = bHasRsp;
        }
    }
    void InsertToWaitRsp(const CKeyType &tKey)
    {
        bool bHasRsp = false;
        m_tMarks.Insert(tKey, bHasRsp);
    }
    bool IsAllReqHasRsp()
    {
        bool bAllHasRsp = true;
        Iterator pPos;
        while (!pPos.End())
        {
            CKeyType tKey;
            bool bValue;
            if (m_tMarks.Iterate(pPos, tKey, bValue))
            {
                if ( ! bValue )
                {
                    bAllHasRsp = false;
                    break;
                }
            }
        }
        return bAllHasRsp;
    }
public:
    CMapType m_tMarks;
};
class CBatchInterAction : public CSipTask // �������������ࣨ��Ҫ�ռ���������
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_RESPONSE_STATE,                        //�Ѿ��������󣬵ȴ�����Ӧ��
    };
public:
    CBatchInterAction(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CBatchInterAction();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CBatchInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatchInterAction[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());

#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
    template <typename CReq>
    u32 PostMsgReqToCMU(CReq &cReq)
    {
        const TSipURI &tDstURI = m_pcCuAgent->m_tCmuURI;
        return FakePostMsgReq(cReq, tDstURI);
    }
    template <typename CReq>
    u32 PostMsgReqTo3AS(CReq &cReq)
    {
        C3acAgent *pc3acAgent = NULL;
        if ( NULL == m_pc3acAgent )
        { //�״η�����Ϣ��TAS
            //����3AS����
            if (m_pcCuAgent->IsTasOnline())
            {
                pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());
                CMS_ASSERT(NULL != pc3acAgent);
                CMS_ASSERT( 0 == strcmp(pc3acAgent->GetObjName(), "C3acAgent") );
                m_pc3acAgent = pc3acAgent;
            }
            else
            { //TAS������
                return PROCMSG_DEL;
            }
        }
        else
        {
            pc3acAgent = m_pc3acAgent;
        }
        const TSipURI &tDstURI = pc3acAgent->m_t3asURI;
        FakePostMsgReq(cReq, tDstURI);
        return PROCMSG_OK;
    }
    void SetRspErrorCode(int nErrorCode);

    template<typename CRsp>
    void GetRspMsg(CRsp& cRsp, CMessage *const pcMsg)
    {
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        pcOspSipMsg->GetMsgBody(cRsp);
    }

    template<typename CRspFromSvr, typename CRspToCu>
    u32 OnWaitRspMsg(CRspFromSvr& cRsp)
    {
        CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

        m_tRspMarks.SetRspMark(cRsp.GetSeqNum(), true);
        SetRspErrorCode(cRsp.GetErrorCode());

        //�ж��Ƿ��Ѿ��ռ������н��
        if ( m_tRspMarks.IsAllReqHasRsp() )
        {
            CRspToCu *pcRsp = (CRspToCu *)m_pRsp;
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]����CBatchInterAction�У������豸���յ�Ӧ���������������\n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
            SendRspToCu<CRspToCu>(*pcRsp, m_ptRecvNode);
            //�������, ����֮
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        return PROCMSG_OK;
    }
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;
    C3acAgent *m_pc3acAgent;    //����漰��3AS��ѯ�����ָ�벻Ϊ��
    CEventRsp *m_pRsp;
    CRspMarks<CXMap<int, bool>, int> m_tRspMarks;
};

// ��ѯreq-rsp-ntfreq-ntfrsp��ҵ��(����queryTag·��)
class CQueryTask : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //����״̬��δ���Ͳ�ѯ����
        WAIT_RESPONSE_STATE,                        //�ȴ���ѯRsp
        WAIT_NOTIFY_STATE,                          //�ȴ���ѯNtf
    };
public:
    CQueryTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CQueryTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CQueryTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_QUERY_STATE:
            {
                return "Not Query State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        case WAIT_NOTIFY_STATE:
            {
                return "Wait Notify State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());

#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
    template <typename CReq>
    u32 PostMsgReqToCMU(CReq &cReq)
    {
        const TSipURI &tDstURI = m_pcCuAgent->m_tCmuURI;
        return FakePostMsgReq(cReq, tDstURI);
    }
    template <typename CReq>
    u32 PostMsgReqTo3AS(CReq &cReq)
    {
        if ( m_pcCuAgent->IsTasOnline() )
        {
            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());;
            CMS_ASSERT( NULL != pc3acAgent );
            CMS_ASSERT( 0 == strcmp(pc3acAgent->GetObjName(), "C3acAgent") );
            if(pc3acAgent == NULL)
            {
                return PROCMSG_DEL;
            }
            const TSipURI &tDstURI = pc3acAgent->m_t3asURI;
            FakePostMsgReq(cReq, tDstURI);
            m_pc3acAgent = pc3acAgent;
        }
        else
        {
            return PROCMSG_DEL;
        }
        return PROCMSG_OK;
    }
public:
    //״̬����
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
    u32 WaitNotifyTimerPoll();

    //NotReq״̬�µķ��ʹ���
    //��ΪCReq����͸����CUI��Ҫת��CU��3AS֮������ݣ��������ﲻֱ�Ӵ���CMessage
    //����ת���ŵ��������
    /*====================================================================
    ������      : OnNotQueryMsg
    ����        : NotReq״̬�µķ��ʹ���
    ��ΪCReq����͸����CUI��Ҫת��CU��3AS֮������ݣ��������ﲻֱ�Ӵ���CMessage
    ����ת���ŵ��������
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��: CReqToSvr &cReq            �������ͱ����Ƿ���������Ľӿ�����
    EServerType eSendToDst     ��������3AS����CMU���Ժ��п�����RecMgr
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/2/3/             liangli
    ====================================================================*/
    template<typename CReqToSvr, typename CRspToCu>
    u32 OnNotQueryMsg(CReqToSvr &cReq, EServerType eSendToDst = SERVER_3AS)
    {
        CMS_ASSERT(GetState() == NOT_QUERY_STATE);
        //����req��Ϣ�������wait rsp state�յ�ntf��Ϣ�ô���
        m_dwReqEvent = cReq.GetEventId();
        //1 ���������������, �������ͱ����Ƿ���������Ľӿ�����
        u32 dwRet = PROCMSG_OK;
        switch (eSendToDst)
        {
        case SERVER_3AS:
            {
                dwRet = PostMsgReqTo3AS(cReq);
            }
            break;
        case SERVER_CMU:
            {
                PostMsgReqToCMU(cReq);
            }
            break;
        default:
            {
                return PROCMSG_FAIL;
            }
            break;
        }
        if ( PROCMSG_DEL == dwRet )
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]��������[%s--%d]��TAS�����ߣ�ֱ�Ӿܾ�����\n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                cReq.GetEventStr().c_str(), cReq.GetEventId());
            CEventRsp cRsp;
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
            m_strRspOfErr = cRsp.ToXml();
            SendRspToCu(m_strRspOfErr, m_ptRecvNode);
            NextState(NOT_QUERY_STATE);
            delete this;
            return dwRet;
        }

        //��QueryTag���뵽�Ự���У��ȴ�NTF
        g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());
        m_strQueryTag = cReq.GetQueryTag();

        //2 ת��״̬���ȴ�Ӧ��
        NextState(WAIT_RESPONSE_STATE);

        //3 ��������������ݣ���ʱʱ���ô˽������
        m_pRsp = new CRspToCu;
        m_pRsp->SetSeqNum(cReq.GetSeqNum());
        m_pRsp->SetSession(cReq.GetSession());
        return PROCMSG_OK;
    }
    template<typename CRspFromSvr, typename CRspToCu>
    u32 OnWaitRspMsg(CMessage *const pcMsg)
    {
        CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CRspFromSvr cRsp;
        pcOspSipMsg->GetMsgBody(cRsp);
        if ( cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetEntryNum() == 0)
        { //��ѯʧ�� ���� �޽��
            if ( cRsp.GetErrorCode() != CMS_SUCCESS )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CQueryTask[NO: %u]��ѯӦ��[%s--%d]����ʧ�ܣ�������[%d]\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[NO: %u]��ѯӦ��[%s--%d]�ɹ������Ϊ��\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
            }

            m_pRsp->SetErrorCode(cRsp.GetErrorCode());
            m_strRspOfErr = m_pRsp->ToXml();
            SendRspToCu(m_strRspOfErr, m_ptRecvNode);
            NextState(NOT_QUERY_STATE);
            delete this;
            return PROCMSG_OK;
        }

        CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
        pcRspToCu->SetTotalEntryNum(cRsp.GetTotalEntryNum());
        NextState(WAIT_NOTIFY_STATE);
        return PROCMSG_OK;
    }
    template<typename CNtfFromSvr, typename CNtfToSvr, typename CRspToCu>
    u32 OnWaitNotifyMsg(CMessage *const pcMsg)
    {
        CMS_ASSERT(GetState() == WAIT_NOTIFY_STATE);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CNtfFromSvr cNtf;
        pcOspSipMsg->GetMsgBody(cNtf);

        TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�Ntf��Ϣ[%s--%d]������Ntf�к���[%d]����¼\n",
            cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
            cNtf.GetEntryNum());

        //�Ȼظ�NTF֪ͨ
        CNtfToSvr cRsp;
        cRsp.SetEvent(pcMsg->event+1);
        cRsp.SetSeqNum(cNtf.GetSeqNum());
        cRsp.SetSession(cNtf.GetSession());
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

        CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
        pcRspToCu->push_back(cNtf, this);

        //�ж��Ƿ��������Ntf
        if ( cNtf.IsLastNtf() )
        {
            SendRspToCu<CRspToCu>(*pcRspToCu, m_ptRecvNode);
            g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
            m_strQueryTag.clear();
            NextState(NOT_QUERY_STATE);
            delete this;
        }
        else
        {
            NextState(WAIT_NOTIFY_STATE);  //�л�״̬�������ѯ������������ʱ�ж�
        }
        return PROCMSG_OK;
    }
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;      //��ָ��һ����Ϊ��
    C3acAgent *m_pc3acAgent;    //ֻ�е���ѯ�ɹ���(˵��TAS����)����ָ�����Ч��Ϊ��Ч�ʻ���
    string m_strRspOfErr;
    CEventRsp *m_pRsp;
    string m_strQueryTag;       //���䲻Ϊ��ʱ��������m_mapSessToTaskMap�еǼǹ�
    std::list<string> m_VecNtfReqList; //������rsp��Ϣ�н��ܵ���ntf��Ϣ
    u16 m_dwReqEvent;
};


// ��ѯreq-rsp-ntf��ҵ��(����queryTag·��)
class CNotifyTransferTask : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //����״̬��δ���Ͳ�ѯ����
        WAIT_RESPONSE_STATE,                        //�ȴ���ѯRsp
        WAIT_NOTIFY_STATE,                          //�ȴ���ѯNtf
    };
public:
    CNotifyTransferTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CNotifyTransferTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CNotifyTransferTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_QUERY_STATE:
            {
                return "Not Query State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        case WAIT_NOTIFY_STATE:
            {
                return "Wait Notify State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CNotifyTransferTask[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());

#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
    template <typename CReq>
    u32 PostMsgReqToCMU(CReq &cReq)
    {
        const TSipURI &tDstURI = m_pcCuAgent->m_tCmuURI;
        return FakePostMsgReq(cReq, tDstURI);
    }
    template <typename CReq>
    u32 PostMsgReqTo3AS(CReq &cReq)
    {
        if ( m_pcCuAgent->IsTasOnline() )
        {
            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());;
            CMS_ASSERT( NULL != pc3acAgent );
            CMS_ASSERT( 0 == strcmp(pc3acAgent->GetObjName(), "C3acAgent") );
            if(pc3acAgent == NULL)
            {
                return PROCMSG_DEL;
            }
            const TSipURI &tDstURI = pc3acAgent->m_t3asURI;
            FakePostMsgReq(cReq, tDstURI);
            m_pc3acAgent = pc3acAgent;
        }
        else
        {
            return PROCMSG_DEL;
        }
        return PROCMSG_OK;
    }
public:
    //״̬����
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
    u32 WaitNotifyTimerPoll();

    //NotReq״̬�µķ��ʹ���
    //��ΪCReq����͸����CUI��Ҫת��CU��3AS֮������ݣ��������ﲻֱ�Ӵ���CMessage
    //����ת���ŵ��������
    /*====================================================================
    ������      : OnNotQueryMsg
    ����        : NotReq״̬�µķ��ʹ���
    ��ΪCReq����͸����CUI��Ҫת��CU��3AS֮������ݣ��������ﲻֱ�Ӵ���CMessage
    ����ת���ŵ��������
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��: CReqToSvr &cReq            �������ͱ����Ƿ���������Ľӿ�����
    EServerType eSendToDst     ��������3AS����CMU���Ժ��п�����RecMgr
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/2/3/             liangli
    ====================================================================*/
    template<typename CReqToSvr, typename CRspToCu>
    u32 OnNotQueryMsg(CReqToSvr &cReq, EServerType eSendToDst = SERVER_3AS)
    {
        CMS_ASSERT(GetState() == NOT_QUERY_STATE);
        //����req��Ϣ�������wait rsp state�յ�ntf��Ϣ�ô���
        m_dwReqEvent = cReq.GetEventId();
		m_tChannel = cReq.GetDevChn();
        //1 ���������������, �������ͱ����Ƿ���������Ľӿ�����
        u32 dwRet = PROCMSG_OK;
        switch (eSendToDst)
        {
        case SERVER_3AS:
            {
                dwRet = PostMsgReqTo3AS(cReq);
            }
            break;
        case SERVER_CMU:
            {
                PostMsgReqToCMU(cReq);
            }
            break;
        default:
            {
                return PROCMSG_FAIL;
            }
            break;
        }
        if ( PROCMSG_DEL == dwRet )
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]��������[%s--%d]��TAS�����ߣ�ֱ�Ӿܾ�����\n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                cReq.GetEventStr().c_str(), cReq.GetEventId());
            CEventRsp cRsp;
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
            m_strRspOfErr = cRsp.ToXml();
            SendRspToCu(m_strRspOfErr, m_ptRecvNode);
            NextState(NOT_QUERY_STATE);
            delete this;
            return dwRet;
        }

        //��QueryTag���뵽�Ự���У��ȴ�NTF
        g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());
        m_strQueryTag = cReq.GetQueryTag();

        //2 ת��״̬���ȴ�Ӧ��
        NextState(WAIT_RESPONSE_STATE);

        //3 ��������������ݣ���ʱʱ���ô˽������
        m_pRsp = new CRspToCu;
        m_pRsp->SetSeqNum(cReq.GetSeqNum());
        m_pRsp->SetSession(cReq.GetSession());
        return PROCMSG_OK;
    }
    template<typename CRspFromSvr, typename CRspToCu>
    u32 OnWaitRspMsg(CMessage *const pcMsg)
    {
        CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CRspFromSvr cRsp;
        pcOspSipMsg->GetMsgBody(cRsp);
        if ( cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetTotalEntryNum() == 0)
        { //��ѯʧ�� ���� �޽��
            if ( cRsp.GetErrorCode() != CMS_SUCCESS )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CQueryTask[NO: %u]��ѯӦ��[%s--%d]����ʧ�ܣ�������[%d]\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[NO: %u]��ѯӦ��[%s--%d]�ɹ���������NTF֪ͨ\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
            }

            m_pRsp->SetErrorCode(cRsp.GetErrorCode());
			CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
			pcRspToCu->SetTotalEntryNum(cRsp.GetTotalEntryNum());
			pcRspToCu->SetMsgType(cRsp.GetMsgType());
			pcRspToCu->SetData(cRsp.GetData());
			SendRspToCu<CRspToCu>(*pcRspToCu, m_ptRecvNode);

            delete this;
            return PROCMSG_OK;
        }

		m_pRsp->SetErrorCode(cRsp.GetErrorCode());
		CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
		pcRspToCu->SetTotalEntryNum(cRsp.GetTotalEntryNum());
		SendRspToCu<CRspToCu>(*pcRspToCu, m_ptRecvNode);
        NextState(WAIT_NOTIFY_STATE);
        return PROCMSG_OK;
    }
    template<typename CNtfFromSvr, typename CNtfToSvr>
    u32 OnWaitNotifyMsg(CMessage *const pcMsg)
    {
        CMS_ASSERT(GetState() == WAIT_NOTIFY_STATE);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CNtfFromSvr cNtf;
        pcOspSipMsg->GetMsgBody(cNtf);

        TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�Ntf��Ϣ[%s--%d]������Ntf�к���[%d]����¼\n",
            cNtf.GetEventStr().c_str(), cNtf.GetEventId(),cNtf.GetEntryNum());

        //�Ȼظ�NTF֪ͨ
        CNtfToSvr cRsp;
        cRsp.SetEvent(pcMsg->event+1);
        cRsp.SetSeqNum(cNtf.GetSeqNum());
        cRsp.SetSession(cNtf.GetSession());
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

		CCuPuTransparentQryNtfReq cCuNtf;
		cCuNtf.SetDevChn(m_tChannel);
		cCuNtf.SetData(cNtf.GetData());
		cCuNtf.SetMsgType(cNtf.GetMsgType());
		cCuNtf.SetLastNtf(cNtf.IsLastNtf());

		m_pcCuAgent->m_cNotifyPool.InsertTransparentQurNotify(cCuNtf);

        //�ж��Ƿ��������Ntf
        if ( cNtf.IsLastNtf() )
        {
            g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
            m_strQueryTag.clear();
            NextState(NOT_QUERY_STATE);
            delete this;
        }
        else
        {
            NextState(WAIT_NOTIFY_STATE);  //�л�״̬�������ѯ������������ʱ�ж�
        }
        return PROCMSG_OK;
    }
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;      //��ָ��һ����Ϊ��
    C3acAgent *m_pc3acAgent;    //ֻ�е���ѯ�ɹ���(˵��TAS����)����ָ�����Ч��Ϊ��Ч�ʻ���
    string m_strRspOfErr;
    CEventRsp *m_pRsp;
    string m_strQueryTag;       //���䲻Ϊ��ʱ��������m_mapSessToTaskMap�еǼǹ�
    std::list<string> m_VecNtfReqList; //������rsp��Ϣ�н��ܵ���ntf��Ϣ
	TChannel m_tChannel;        //qry channel
    u16 m_dwReqEvent;
};

/* invite��ҵ�� �Ż�������Ӧ�Է�����֧����NAT��Խ����*/
class CInviteTaskEx : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_SETUP_RSP_STATE,
        /*
        invite��������
        1��UAC����invite_req
        2��UAS����invite_rsp
        3����ʱ�Ự��δ��ȫ��������Ҫ��UAC�ٷ���invite_ack
        ���һ��ʱ���ڻ�û��ack����ײ�Э��ջ�����ٻỰ����֪ͨ�ϲ�OSP_SIP_DIALOG_TERMINATE
        �ϲ��յ�OSP_SIP_DIALOG_TERMINATE�󣬵���OspSipReleaseDlg
        4��UAC����invite_ack��UAS���Դ������Ϣ��Ҳ���Բ�����
        ����������ֽ�������Ӧ���ڴ˴���ʼ��������
        invite_ack��һ�������֪ͨ��Ϣ��Э��ջ�����UAC�κλ�Ӧ
        5����UAS�յ�invite_ack����ʾ�Ự�Ѿ�������UAS���Դ������Ϣ��Ҳ���Բ�����
        6���ڻỰ��ʼ�������κν׶Σ�UAC�����Է���Bye����ֹ�Ự
        UAS�������InviteRsp֮ǰ����ByeReq����ʵ�ǷǷ�������Э��ջ��תΪ4.3������Ӧ��
        UASֻ���ں��гɹ���InviteRsp������ܡ��Ҷϡ���Bye��������ֻ�ܾܾ���403�������InviteRsp��
        7��BYE��SIP��Ϣ��û�ж�����Ӧ��ҵ�����Ϣ��BYE��Ӧ����Э��ջ�Լ���ɣ�����Ҳû����Ϣ��
        Ϊ��ɾ��OspSip��Bye����󶨣��ϲ㻹��Ҫ��������ByeRsp

        ��CUI�յ�CMU��ByeReqʱ���ײ�Э��ջ���Լ�����ByeRsp��
        ��������OspSip�㻹����Invite�Ự�󶨹�ϵ�������ϲ㻹��Ҫ��һ��ByeRsp����OspSipɾ���󶨹�ϵ
        CUI�˵�OspSip�յ�ByeRspʱ��������kdsip�ײ�ӿڣ�ֻɾ���󶨹�ϵ
        CMU�˵�OspSip�յ�kdsip�Զ��ظ���ByeRsp��Ҳɾ���󶨹�ϵ�����ϲ�ص�OSP_SIP_BYE_RSP
        */
        WAIT_SETUP_ACK_STATE,   //��״̬û��Ӧ��
        INVITE_HOLD_STATE,
        WAIT_BYE_RSP_STATE,
    };
public:
    CInviteTaskEx(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask,s32 nPlayId);
    ~CInviteTaskEx();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CInviteTaskEx";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_SETUP_RSP_STATE:
            {
                return "Wait Setup Rsp State";
            }
            break;
        case WAIT_SETUP_ACK_STATE:
            {
                return "Wait Setup Ack State";
            }
            break;
        case INVITE_HOLD_STATE:
            {
                return "Invite Hold State";
            }
            break;
        case WAIT_BYE_RSP_STATE:
            {
                return "Wait Bye Rsp State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState();
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitSetupRspProcMsg(CMessage *const pcMsg);
    u32 WaitSetupAckProcMsg(CMessage *const pcMsg);
    u32 WaitByeRspProcMsg(CMessage *const pcMsg);
    u32 InviteHoldProcMsg(CMessage *const pcMsg);
    u32 WaitSetupRspTimerPoll();
    u32 WaitAckTimerPoll();
    u32 WaitByeRspTimerPoll();
public:
    void NotifyStop2Cu(s32 dwErrorCode);
	void SwitchToNatAddress(TSdp &tSdp);
	TNetAddr GetSendBindAddr() const;
	void ProcNatCBMsg(CMessage *const pcMsg);
public:
    //����ӿ�
    s32 StartSendInviteReq(CInviteReq &cReq);
    s32 StartSendInviteAck();
    s32 StartSendInviteBye();
    bool TryBye(CSipTask *pcActTask);
    s32 StartVcrCtrl(const CCuVcrCtrlReq&cCuReq);
    CPlaybackProgressNtfReq &GetPlayBackProgressNtf();
    const CPlaybackProgressNtfReq &GetPlayBackProgressNtf() const;
public:
    TChannel& GetDevChn();
public:
    void AddChildTask(int nSeqNo, int TaskNo);
    void DelChildTask(int nSeqNo);
    CTask* FindChildTask(int nSeqNo);
    u32 GetErrorCode() const { return m_dwErrorCode; }
private:
    void ReleaseVtduResource();

public:
    CCuAgent *m_pcCuAgent;
    CSipTask *m_pcInviteInterActTask;
    TKDSIP_DIALOG_ID m_tInviteID;
    bool m_bByeWhileWaitInviteRsp;

    //TChannel m_tCuChn;
    //TChannel m_tDevChn;

    CInviteReq m_tReq;
    CInviteRsp m_tRsp;
    CPlaybackProgressNtfReq m_tNtfReq;
    std::string m_strVtduResourceId;
    std::string m_strVtduId;
    CXMap<int, int> m_cChildTaskList; //�Ի��������б�����vcr��������
	s32 m_nPlayId;                    //��ǰ�Ự��Ӧ��cu playid
    CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};

/* invite��ҵ�� */
class CInviteTask : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_SETUP_RSP_STATE,
        /*
        invite��������
        1��UAC����invite_req
        2��UAS����invite_rsp
        3����ʱ�Ự��δ��ȫ��������Ҫ��UAC�ٷ���invite_ack
        ���һ��ʱ���ڻ�û��ack����ײ�Э��ջ�����ٻỰ����֪ͨ�ϲ�OSP_SIP_DIALOG_TERMINATE
        �ϲ��յ�OSP_SIP_DIALOG_TERMINATE�󣬵���OspSipReleaseDlg
        4��UAC����invite_ack��UAS���Դ������Ϣ��Ҳ���Բ�����
        ����������ֽ�������Ӧ���ڴ˴���ʼ��������
        invite_ack��һ�������֪ͨ��Ϣ��Э��ջ�����UAC�κλ�Ӧ
        5����UAS�յ�invite_ack����ʾ�Ự�Ѿ�������UAS���Դ������Ϣ��Ҳ���Բ�����
        6���ڻỰ��ʼ�������κν׶Σ�UAC�����Է���Bye����ֹ�Ự
        UAS�������InviteRsp֮ǰ����ByeReq����ʵ�ǷǷ�������Э��ջ��תΪ4.3������Ӧ��
        UASֻ���ں��гɹ���InviteRsp������ܡ��Ҷϡ���Bye��������ֻ�ܾܾ���403�������InviteRsp��
        7��BYE��SIP��Ϣ��û�ж�����Ӧ��ҵ�����Ϣ��BYE��Ӧ����Э��ջ�Լ���ɣ�����Ҳû����Ϣ��
        Ϊ��ɾ��OspSip��Bye����󶨣��ϲ㻹��Ҫ��������ByeRsp

        ��CUI�յ�CMU��ByeReqʱ���ײ�Э��ջ���Լ�����ByeRsp��
        ��������OspSip�㻹����Invite�Ự�󶨹�ϵ�������ϲ㻹��Ҫ��һ��ByeRsp����OspSipɾ���󶨹�ϵ
        CUI�˵�OspSip�յ�ByeRspʱ��������kdsip�ײ�ӿڣ�ֻɾ���󶨹�ϵ
        CMU�˵�OspSip�յ�kdsip�Զ��ظ���ByeRsp��Ҳɾ���󶨹�ϵ�����ϲ�ص�OSP_SIP_BYE_RSP
        */
        WAIT_SETUP_ACK_STATE,   //��״̬û��Ӧ��
        INVITE_HOLD_STATE,
        WAIT_BYE_RSP_STATE,
    };
public:
    CInviteTask(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask, s32 nPlayId);
    ~CInviteTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CInviteTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_SETUP_RSP_STATE:
            {
                return "Wait Setup Rsp State";
            }
            break;
        case WAIT_SETUP_ACK_STATE:
            {
                return "Wait Setup Ack State";
            }
            break;
        case INVITE_HOLD_STATE:
            {
                return "Invite Hold State";
            }
            break;
        case WAIT_BYE_RSP_STATE:
            {
                return "Wait Bye Rsp State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState();
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitSetupRspProcMsg(CMessage *const pcMsg);
    u32 WaitSetupAckProcMsg(CMessage *const pcMsg);
    u32 WaitByeRspProcMsg(CMessage *const pcMsg);
    u32 InviteHoldProcMsg(CMessage *const pcMsg);
    u32 WaitSetupRspTimerPoll();
    u32 WaitAckTimerPoll();
    u32 WaitByeRspTimerPoll();
public:
    void NotifyStop2Cu(s32 dwErrorCode);
public:
    //����ӿ�
    s32 StartSendInviteReq(CInviteReq &cReq);
    s32 StartSendInviteAck();
    s32 StartSendInviteBye();
    bool TryBye(CSipTask *pcActTask);
    s32 StartVcrCtrl(const CCuVcrCtrlReq&cCuReq);
    CPlaybackProgressNtfReq &GetPlayBackProgressNtf();
    const CPlaybackProgressNtfReq &GetPlayBackProgressNtf() const;

	TNetAddr GetSendBindAddr() const;
	void SwitchToNatAddress(TSdp &tSdp);

public:
    TChannel& GetDevChn();
public:
    void AddChildTask(int nSeqNo, int TaskNo);
    void DelChildTask(int nSeqNo);
    CTask* FindChildTask(int nSeqNo);
    u32 GetErrorCode() const{ return m_dwErrorCode; }
private:
    void ReleaseVtduResource();

public:
    CCuAgent *m_pcCuAgent;
    CSipTask *m_pcInviteInterActTask;
    TKDSIP_DIALOG_ID m_tInviteID;
    bool m_bByeWhileWaitInviteRsp;

    //TChannel m_tCuChn;
    //TChannel m_tDevChn;

    CInviteReq m_tReq;
    CInviteRsp m_tRsp;
    CPlaybackProgressNtfReq m_tNtfReq;
    std::string m_strVtduResourceId;
    std::string m_strVtduId;
    CXMap<int, int> m_cChildTaskList; //�Ի��������б�����vcr��������

	s32 m_nPlayId;                    //��ǰ�Ự��Ӧ��cu playid
    CUserStreamNumRestorer m_cStrmNumRestorer;
private:
    u32 m_dwErrorCode;
};
class CInviteInterAction : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_RESPONSE_STATE,                        //�Ѿ��������󣬵ȴ�����Ӧ��
    };
public:
    CInviteInterAction(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CInviteInterAction();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CInviteInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
public:
    /*CInviteTask*/CTask *GetInviteTask(s32 nPlayId);
	u32 ProcInviteReq();
	u32 ProcInviteReqEx();
	u32 ProcInviteReqNatSwitch();
	void ProcInviteAck(CMessage *const pcMsg);
	void ProcInviteAckEx(CMessage *const pcMsg);
	void ProcInviteAckNatSwitch(CMessage *const pcMsg);
	void ProcInviteBye(CMessage *const pcMsg);
	void ProcInviteByeEx(CMessage *const pcMsg);
	void ProcInviteByeNatSwitch(CMessage *const pcMsg);
	void ProcVcrReq(CMessage *const pcMsg);
	void ProcVcrReqEx(CMessage *const pcMsg);
	void ProcVcrReqNatSwitch(CMessage *const pcMsg);
	void SwitchPlatAddrToNat(TSdp& tSdp);
public:
    void WriteLog(const u32 dwErrorCode, const u32 dwEvent);
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;
    TChannel m_tDevChn;
    s32 m_nplayId;
    u32 m_dwEvent;
    string m_strReq;
    u32 m_dwSeqNo;
};

/* ���������� �Ż�������Ӧ�ԣ�֧���´�NAT����*/
class CCallPuTaskEx : public CSipTask
{
public:
        /*
        invite��������
        1��UAC����invite_req
        2��UAS����invite_rsp
        3����ʱ�Ự��δ��ȫ��������Ҫ��UAC�ٷ���invite_ack
        ���һ��ʱ���ڻ�û��ack����ײ�Э��ջ�����ٻỰ����֪ͨ�ϲ�OSP_SIP_DIALOG_TERMINATE
        �ϲ��յ�OSP_SIP_DIALOG_TERMINATE�󣬵���OspSipReleaseDlg
        4��UAC����invite_ack��UAS���Դ������Ϣ��Ҳ���Բ�����
        ����������ֽ�������Ӧ���ڴ˴���ʼ��������
        invite_ack��һ�������֪ͨ��Ϣ��Э��ջ�����UAC�κλ�Ӧ
        5����UAS�յ�invite_ack����ʾ�Ự�Ѿ�������UAS���Դ������Ϣ��Ҳ���Բ�����
        6���ڻỰ��ʼ�������κν׶Σ�UAC�����Է���Bye����ֹ�Ự
        UAS�������InviteRsp֮ǰ����ByeReq����ʵ�ǷǷ�������Э��ջ��תΪ4.3������Ӧ��
        UASֻ���ں��гɹ���InviteRsp������ܡ��Ҷϡ���Bye��������ֻ�ܾܾ���403�������InviteRsp��
        7��BYE��SIP��Ϣ��û�ж�����Ӧ��ҵ�����Ϣ��BYE��Ӧ����Э��ջ�Լ���ɣ�����Ҳû����Ϣ��
        Ϊ��ɾ��OspSip��Bye����󶨣��ϲ㻹��Ҫ��������ByeRsp

        ��CUI�յ�CMU��ByeReqʱ���ײ�Э��ջ���Լ�����ByeRsp��
        ��������OspSip�㻹����Invite�Ự�󶨹�ϵ�������ϲ㻹��Ҫ��һ��ByeRsp����OspSipɾ���󶨹�ϵ
        CUI�˵�OspSip�յ�ByeRspʱ��������kdsip�ײ�ӿڣ�ֻɾ���󶨹�ϵ
        CMU�˵�OspSip�յ�kdsip�Զ��ظ���ByeRsp��Ҳɾ���󶨹�ϵ�����ϲ�ص�OSP_SIP_BYE_RSP
        */
    enum
    {
        //NOT_3PC_REQ_STATE ,
        WAIT_SETUP_REQ_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_ACK_STATE,
        INVITE_HOLD_STATE,
    };
public:
    CCallPuTaskEx(CInstExt *pcInst, const string &strCuSession,string strCallPuReq, CSipTask *pcTask);
    ~CCallPuTaskEx();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CCallPuTaskEx";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
//         case NOT_3PC_REQ_STATE:
//             {
//                 return "Not 3pc Req State";
//             }
//             break;
        case WAIT_SETUP_REQ_STATE:
            {
                return "Wait Set Up Req State";
            }
            break;
        case WAIT_ACK_STATE:
            {
                return "Wait Ack State";
            }
            break;
        case INVITE_HOLD_STATE:
            {
                return "Invite Hold State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void TerminateTask();
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState();
public:
    //״̬����
    u32 WaitSetUpReqProcMsg(CMessage *const pcMsg);
    u32 WaitSetUpReqTimerPoll();
    u32 WaitAckProcMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();
    u32 InviteHoldProcMsg(CMessage *const pcMsg);
	void ProcNatCBMsg(CMessage *const pcMsg);//NAT̽����ص���ַ�������ַ�б仯��Ҫ֪ͨVTDU ���Ľ�����ַ

    virtual void PrintData()const;
public:
    u32 GetErrorCode() const { return m_dwErrorCode; }
public:
    string m_strCuSession;
    string m_strReq;
	string m_strCallPuReq;
    CSipTask *m_pcCallPuInterActTask;
public:
    //CSipTask *m_pcInviteInterActTask;
    TKDSIP_DIALOG_ID m_tDialogID;
    //������������DevChn��Ψһ��ʶ
    TChannel m_tCuChn;
    string m_strModuleId;
    //TPuChnList m_tPuChnIDList;

private:
    //��Ƶ���в�����������
    //CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};

/* ���������� */
class CCallPuTask : public CSipTask
{
public:
        /*
        invite��������
        1��UAC����invite_req
        2��UAS����invite_rsp
        3����ʱ�Ự��δ��ȫ��������Ҫ��UAC�ٷ���invite_ack
        ���һ��ʱ���ڻ�û��ack����ײ�Э��ջ�����ٻỰ����֪ͨ�ϲ�OSP_SIP_DIALOG_TERMINATE
        �ϲ��յ�OSP_SIP_DIALOG_TERMINATE�󣬵���OspSipReleaseDlg
        4��UAC����invite_ack��UAS���Դ������Ϣ��Ҳ���Բ�����
        ����������ֽ�������Ӧ���ڴ˴���ʼ��������
        invite_ack��һ�������֪ͨ��Ϣ��Э��ջ�����UAC�κλ�Ӧ
        5����UAS�յ�invite_ack����ʾ�Ự�Ѿ�������UAS���Դ������Ϣ��Ҳ���Բ�����
        6���ڻỰ��ʼ�������κν׶Σ�UAC�����Է���Bye����ֹ�Ự
        UAS�������InviteRsp֮ǰ����ByeReq����ʵ�ǷǷ�������Э��ջ��תΪ4.3������Ӧ��
        UASֻ���ں��гɹ���InviteRsp������ܡ��Ҷϡ���Bye��������ֻ�ܾܾ���403�������InviteRsp��
        7��BYE��SIP��Ϣ��û�ж�����Ӧ��ҵ�����Ϣ��BYE��Ӧ����Э��ջ�Լ���ɣ�����Ҳû����Ϣ��
        Ϊ��ɾ��OspSip��Bye����󶨣��ϲ㻹��Ҫ��������ByeRsp

        ��CUI�յ�CMU��ByeReqʱ���ײ�Э��ջ���Լ�����ByeRsp��
        ��������OspSip�㻹����Invite�Ự�󶨹�ϵ�������ϲ㻹��Ҫ��һ��ByeRsp����OspSipɾ���󶨹�ϵ
        CUI�˵�OspSip�յ�ByeRspʱ��������kdsip�ײ�ӿڣ�ֻɾ���󶨹�ϵ
        CMU�˵�OspSip�յ�kdsip�Զ��ظ���ByeRsp��Ҳɾ���󶨹�ϵ�����ϲ�ص�OSP_SIP_BYE_RSP
        */
    enum
    {
        //NOT_3PC_REQ_STATE ,
        WAIT_SETUP_REQ_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_ACK_STATE,
        INVITE_HOLD_STATE,
    };
public:
    CCallPuTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcTask);
    ~CCallPuTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CCallPuTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
//         case NOT_3PC_REQ_STATE:
//             {
//                 return "Not 3pc Req State";
//             }
//             break;
        case WAIT_SETUP_REQ_STATE:
            {
                return "Wait Set Up Req State";
            }
            break;
        case WAIT_ACK_STATE:
            {
                return "Wait Ack State";
            }
            break;
        case INVITE_HOLD_STATE:
            {
                return "Invite Hold State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void TerminateTask();
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState();
public:
    //״̬����
    u32 WaitSetUpReqProcMsg(CMessage *const pcMsg);
    u32 WaitSetUpReqTimerPoll();
    u32 WaitAckProcMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();
    u32 InviteHoldProcMsg(CMessage *const pcMsg);
public:
    u32 GetErrorCode() const { return m_dwErrorCode; }
public:
    string m_strCuSession;
    string m_strReq;
    CSipTask *m_pcCallPuInterActTask;
public:
    //CSipTask *m_pcInviteInterActTask;
    TKDSIP_DIALOG_ID m_tDialogID;
    //������������DevChn��Ψһ��ʶ
    TChannel m_tCuChn;
    string m_strModuleId;
    //TPuChnList m_tPuChnIDList;
private:
    //CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};
class CCallPuInterAction : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_RESPONSE_STATE,                        //�Ѿ��������󣬵ȴ�����Ӧ��
    };

	enum ECallPuTaskType
	{
		CALL_PU_TASK_TYPE_PUNCH = 0,		// ֧���ϵ�punch��ʽ
		CALL_PU_TASK_TYPE_NAT,				// nat̽���ģʽ
		CALL_PU_TASK_TYPE_CUI_NatSwitch,	// CUIת������ģʽ
	};

public:
    CCallPuInterAction(CInstExt *pcInst, const string& strCuSession, PTRecvNode ptRecvNode);
    ~CCallPuInterAction();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CCallPuInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 NotReqTimerPoll();
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
public:
    void WriteLog(const TChannel& tDevChn, const u32 dwErrorCode);
public:
    CSipTask *GetCallPuTask(s32 nPlayId);
	void ProcStartCallPuReq(CMessage *const pcMsg);
	void ProcStartCallPuReqEx(CMessage *const pcMsg);
	void ProcStartCallPuReqNatSwitch(CMessage *const pcMsg);
	void ProcStartCallPuRsp(CMessage *const pcMsg);
	void ProcStartCallPuRspEx(CMessage *const pcMsg);
	void ProcStartCallPuRspNatSwitch(CMessage *const pcMsg);
	u32 ProcStopCallPuReq(CMessage *const pcMsg);
	u32 ProcStopCallPuReqEx(CMessage *const pcMsg);
	u32 ProcStopCallPuReqNatSwitch(CMessage *const pcMsg);
	void SwitchCuAddrToNatAddr(TSdp& tSdp);
	void SwitchCuAddrToNatAddrNatSwitch(TSdp& tSdp);
	bool SwitchToCuiNatSwitchAddressForCu(TSdp& tSdp);
	bool SwitchToCuiNatSwitchAddressForPlat(TSdp& tSdp);
	TNetAddr GetSendBindAddr() const;
	void SwitchPlatAddrToNat(TSdp& tSdp);
	bool GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr);
	bool GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr);
public:
    PTRecvNode m_ptRecvNode;
    string m_strCuSession;
    s32 m_nplayId;
	bool m_bNewInvite;
    string m_strSrcReq;
	string m_strSrcReqNatSwitch;
    string m_strDstReq;
    string m_strLogDesc;
    u32 m_dwEvent;
	ECallPuTaskType m_eCallPuTaskType;
};

/* �����SingleInterAction,QueryTask,InviteTask�μ�������� */
class CPeSingleInterAction : public CCallBackTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_RESPONSE_STATE,                        //�Ѿ��������󣬵ȴ�����Ӧ��
    };
public:
    CPeSingleInterAction(CInstExt *pcInst, const string &strCuSession, const string &strPeUri, PTRecvNode ptRecvNode);
    ~CPeSingleInterAction();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CPeSingleInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeSingleInterAction[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());

#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }

    template <typename CReq>
    u32 PostMsgReqToPe(CReq &cReq)
    {
        return FakePostMsgReq(cReq, m_tPeUri);
    }
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitReqTimerPoll();
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 ProcReqMsgAfterReg();

    void SetErrorRsp(CEventReq *pcReq);

    template<typename CRsp>
    void SetErrorRsp(CEventReq *pcReq)
    {
        CMS_ASSERT(NULL != pcReq);

        //����ʧ�ܵĽ��
        CRsp cRsp;
        cRsp.SetSession(pcReq->GetSession());
        cRsp.SetSeqNum(pcReq->GetSeqNum());
        cRsp.SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = cRsp.ToXml();
    }

    //����NotReq״̬�µ���Ϣ�ķ��ͺ���
    template<typename CReq, typename CRsp>
    u32 OnNotReqMsg(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_OK;
        CMS_ASSERT(GetState() == NOT_REQUEST_STATE);

        CReq *pcReq = (CReq *)pcMsg->content;

        //���������Ϣ�����ʧ�ܿ��Է�����Ϣ
        SetErrorRsp<CRsp>(pcReq);
        //����req��Ϣ
        m_strReq = pcReq->ToXml();

        string strPeSession = GetPeSession();
        if(!strPeSession.empty())
        {
            pcReq->SetSession(strPeSession);
            dwRet = PostMsgReqToPe(*pcReq);
            if(dwRet != PROCMSG_OK)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ�[%s--%d]��Ϣ����������[%s]CU�û��Ự[%s]������Ϣʧ��\n",
                    OspExtEventDesc(pcReq->GetEventId()).c_str(), pcReq->GetEventId(),
                    m_tPeUri.GetURIString().c_str(), m_strCuSession.c_str());
                return dwRet;
            }
            NextState(WAIT_RESPONSE_STATE);
        }
		else
		{
			if (IsNormalNotOpenPeEevent(m_dwEventID))
			{
				CRsp cRsp;
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetErrorCode(ERR_CUI_PE_OFFLINE);
				string strRsp = cRsp.ToXml();

				SendRspToCu(strRsp, m_ptRecvNode);
				return PROCMSG_DEL; 
			}
		}
        return dwRet;
    }

    template<typename CRsp>
    void GetRspMsg(CRsp& cRsp, CMessage *const pcMsg)
    {

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

        pcOspSipMsg->GetMsgBody(cRsp);
    }
    template<typename CRsp>
    u32 OnWaitRspMsg(CRsp& cRsp)
    {
        CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

        if(CMS_SUCCESS != cRsp.GetErrorCode())
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ�[%s--%d]��Ϣ����������[%s]CU�û��Ự[%s]�յ�������[%d]\n",
                OspExtEventDesc(cRsp.GetEventId()).c_str(), cRsp.GetEventId(),
                m_tPeUri.GetURIString().c_str(), m_strCuSession.c_str(), 
                cRsp.GetErrorCode() );
        }           

        SendRspToCu<CRsp>(cRsp, m_ptRecvNode);
        
        NextState(NOT_REQUEST_STATE);
        
        return PROCMSG_DEL;
    }
    string GetOptLogDesc(const string& Opt, 
        const string& OptDevInfo, u32 dwErrorCode);
    string GetErrorCode(u32 dwErrorCode);
    string GetOptLogDesc(const string& OptType,const string& Type, const string& OptDevInfo, u32 dwErrorCode);
    void WriteLog(TOperateType enOprType, u32 dwErrorCode, const string& strLogDesc, const string &strDevUri=string());
    void WriteLog(const TChannel& tDevChn, const u32 dwErrorCode, const u32 dwEvent, const string& strDomainName);
    void WriteTimeOutLog();

private:
    void SendUniformRsp(const CEventReq &cReq, s32 nErrorNum)
    {
        CEventRsp cRsp;
        cRsp.SetSeqNum(cReq.GetSeqNum());
        cRsp.SetSession(cReq.GetSession());
        cRsp.SetErrorCode(nErrorNum);
        SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
    }

public:
    string GetPeSession();
    u32 StartSendReq();
public:
    PTRecvNode m_ptRecvNode;
    string m_strRspOfErr;
    u16 m_dwEventID;
public:
    string m_strReq; //�����req��Ϣ
};
class CPeQueryTask : public CCallBackTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //����״̬��δ���Ͳ�ѯ����
        WAIT_RESPONSE_STATE,                        //�ȴ���ѯRsp
        WAIT_NOTIFY_STATE,                          //�ȴ���ѯNtf
    };
public:
    CPeQueryTask(CInstExt *pcInst, const string& strCuSession, const string& strPeUri, PTRecvNode ptRecvNode);
    ~CPeQueryTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CPeQueryTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_QUERY_STATE:
            {
                return "Not Query State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        case WAIT_NOTIFY_STATE:
            {
                return "Wait Notify State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeQueryTask[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());

#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }

    template <typename CReq>
    u32 PostMsgReqToPe(CReq &cReq)
    {
        return FakePostMsgReq(cReq, m_tPeUri);    
    }
public:
    //״̬����
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitReqTimerPoll();
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
    u32 WaitNotifyTimerPoll();

    //void SetErrorRsp(CEventReq *pcReq);
    //NotReq״̬�µķ��ʹ���
    //��ΪCReq����͸����CUI��Ҫת��CU��3AS֮������ݣ��������ﲻֱ�Ӵ���CMessage
    //����ת���ŵ��������
    /*====================================================================
    ������      : OnNotQueryMsg
    ����        : NotReq״̬�µķ��ʹ���
    ��ΪCReq����͸����CUI��Ҫת��CU��3AS֮������ݣ��������ﲻֱ�Ӵ���CMessage
    ����ת���ŵ��������
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��: CReqToSvr &cReq            �������ͱ����Ƿ���������Ľӿ�����
    EServerType eSendToDst     ��������3AS����CMU���Ժ��п�����RecMgr
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/2/3/             liangli
    ====================================================================*/
    template<typename CReq, typename CRspToCu>
    u32 OnNotQueryMsg(CMessage *const pcMsg)
    {
        CMS_ASSERT(GetState() == NOT_QUERY_STATE);
        //����req��Ϣ�������wait rsp state�յ�ntf��Ϣ�ô���
        //m_dwReqEvent = cReq.GetEventId();
        //1 ���������������, �������ͱ����Ƿ���������Ľӿ�����
        CReq *pcReq = (CReq *)pcMsg->content;
        //���������Ϣ�����ʧ�ܿ��Է�����Ϣ
        //�洢��Ӧ����Ϣ
        m_pRsp = new CRspToCu;
        m_pRsp->SetSeqNum(pcReq->GetSeqNum());
        m_pRsp->SetSession(pcReq->GetSession());
        //�洢QueryTag
        g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
        m_strQueryTag = pcReq->GetQueryTag();

        u32 dwRet = PROCMSG_OK;
        
        string strPeSession = GetPeSession();
        if(strPeSession.empty())
        {
            //����req��Ϣ
            m_strReq = pcReq->ToXml();

			// ����פ������ģ�����⴦����������ߣ�ֱ�ӷ������費���� [4/13/2015 pangubing]
			if (IsNormalNotOpenPeEevent(pcMsg->event))
			{
				m_pRsp->SetErrorCode(ERR_CUI_PE_OFFLINE);
				string strRsp = m_pRsp->ToXml();
				SendRspToCu(strRsp, m_ptRecvNode);
				return PROCMSG_DEL; 
			}
        }
        else
        {
            pcReq->SetSession(strPeSession);
            dwRet = PostMsgReqToPe(*pcReq);
            if(dwRet != PROCMSG_OK)
            {
                return dwRet;
            }

            NextState(WAIT_RESPONSE_STATE);
        }
        return dwRet;
    }

    template<typename CRspFromSvr, typename CRspToCu>
    u32 OnWaitRspMsg(CMessage *const pcMsg)
    {
        CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CRspFromSvr cRsp;
        pcOspSipMsg->GetMsgBody(cRsp);
        if ( cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetEntryNum() == 0 )
        { //��ѯʧ�� ���� �޽��
            if ( cRsp.GetErrorCode() != CMS_SUCCESS )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CPeQueryTask[NO: %u]��ѯӦ��[%s--%d]����ʧ�ܣ�������[%d]\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeQueryTask[NO: %u]��ѯӦ��[%s--%d]�ɹ������Ϊ��\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
            }

            m_pRsp->SetErrorCode(cRsp.GetErrorCode());
            CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
            //������û�н�����ǲ�ѯ������д0
            pcRspToCu->SetTotalEntryNum(0);
            SendRspToCu<CRspToCu>(*pcRspToCu, m_ptRecvNode);
            NextState(NOT_QUERY_STATE);
    
            return PROCMSG_DEL;
        }

        CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
        pcRspToCu->SetTotalEntryNum(cRsp.GetTotalEntryNum());

        NextState(WAIT_NOTIFY_STATE);
        return PROCMSG_OK;
    }
    template<typename CNtfFromSvr, typename CRspToCu>
    u32 OnWaitNotifyMsg(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_OK;
        CMS_ASSERT(GetState() == WAIT_NOTIFY_STATE);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CNtfFromSvr cNtf;
        pcOspSipMsg->GetMsgBody(cNtf);

        TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�Ntf��Ϣ[%s--%d]������Ntf�к���[%d]����¼\n",
            cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
            cNtf.GetEntryNum());

        //�Ȼظ�NTF֪ͨ
        CNtfRsp cRsp;
        cRsp.SetEvent(pcMsg->event+1);
        cRsp.SetSeqNum(cNtf.GetSeqNum());
        cRsp.SetSession(cNtf.GetSession());
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

        CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
        pcRspToCu->push_back(cNtf, this);

        //�ж��Ƿ��������Ntf
        if ( cNtf.IsLastNtf() )
        {
            SendRspToCu<CRspToCu>(*pcRspToCu, m_ptRecvNode);
            g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
            m_strQueryTag.clear();
            NextState(NOT_QUERY_STATE);

            dwRet = PROCMSG_DEL;
        }
        else
        {
            NextState(WAIT_NOTIFY_STATE);  //�л�״̬�������ѯ������������ʱ�ж�
        }
        return dwRet;
    }
public:
    u32 StartSendReq();
    string GetPeSession();
public:
	void WriteLog(TOperateType enOprType, u32 dwErrorCode, const string& strLogDesc, const string &strDevUri=string());
public:
    PTRecvNode m_ptRecvNode;  
    //string m_strRspOfErr;
    CEventRsp *m_pRsp;
    string m_strQueryTag;       //���䲻Ϊ��ʱ��������m_mapSessToTaskMap�еǼǹ�
    string m_strReq;
    std::list<string> m_VecNtfReqList; //������rsp��Ϣ�н��ܵ���ntf��Ϣ
};
class CPeInviteTask : public CCallBackTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_SETUP_RSP_STATE,
        WAIT_SETUP_ACK_STATE,   //��״̬û��Ӧ��
        INVITE_HOLD_STATE,
        WAIT_BYE_RSP_STATE,
    };
public:
    CPeInviteTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcInviteInterActTask, const string &strPeUri, CCuAgent *pcCuAgent, s32 nPlayId);
    ~CPeInviteTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CPeInviteTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_SETUP_RSP_STATE:
            {
                return "Wait Setup Rsp State";
            }
            break;
        case WAIT_SETUP_ACK_STATE:
            {
                return "Wait Setup Ack State";
            }
            break;
        case INVITE_HOLD_STATE:
            {
                return "Invite Hold State";
            }
            break;
        case WAIT_BYE_RSP_STATE:
            {
                return "Wait Bye Rsp State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState();
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitSetupRspProcMsg(CMessage *const pcMsg);
    u32 WaitSetupAckProcMsg(CMessage *const pcMsg);
    u32 WaitByeRspProcMsg(CMessage *const pcMsg);
    u32 InviteHoldProcMsg(CMessage *const pcMsg);
    u32 WaitSetupReqTimerPoll();
    u32 WaitSetupRspTimerPoll();
    u32 WaitAckTimerPoll();
    u32 WaitByeRspTimerPoll();
public:
    void NotifyStop2Cu(s32 dwErrorCode);
public:
    //����ӿ�
    u32 StartSendReq();
    u32 StartSendInviteReq(CInviteReq &cReq);
    u32 StartSendInviteAck();
    u32 StartSendInviteBye();
    bool TryBye(CSipTask *pcActTask);
    s32 StartVcrCtrl(const CCuVcrCtrlReq&cCuReq);
    u32 StartTask();
    CPlaybackProgressNtfReq &GetPlayBackProgressNtf();
    const CPlaybackProgressNtfReq &GetPlayBackProgressNtf() const;

	TNetAddr GetSendBindAddr() const;
	void SwitchToNatAddress(TSdp &tSdp);
    u32 GetErrorCode() const { return m_dwErrorCode; }

public:

    void AddChildTask(int nSeqNo, int TaskNo);
    void DelChildTask(int nSeqNo);
    CTask* FindChildTask(int nSeqNo);
public:
    TChannel& GetDevChn();
public:
    string m_strPeSession;
public:
    CSipTask *m_pcInviteInterActTask;
    TKDSIP_DIALOG_ID m_tInviteID;
    bool m_bByeWhileWaitInviteRsp;

    CInviteReq m_tReq;
    CInviteRsp m_tRsp;
    CPlaybackProgressNtfReq m_tNtfReq;
    CXMap<int, int> m_cChildTaskList; //�Ի��������б�����vcr��������

	CCuAgent *m_pcCuAgent;
	s32 m_nPlayId;                    //��ǰ�Ự��Ӧ��cu playid
private:
    CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};
class CPeInviteInterAction : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WAIT_RESPONSE_STATE,                        //�Ѿ��������󣬵ȴ�����Ӧ��
    };
public:
    CPeInviteInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode);
    virtual ~CPeInviteInterAction();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CPeInviteInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void TerminateTask();
    virtual void InitStateMachine();
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitReqTimerPoll();
    u32 WaitRspTimerPoll();
public:
    void WriteLog(u32 dwErrorCode, const u32 dwEvent);
public:
    template<typename CRspToCu>
    u32 SendErrorMsg2Cu(u32 dwErrorCode = ERR_CUI_OPR_TIME_OUT)
    {
        CRspToCu cRsp;
        cRsp.SetErrorCode(dwErrorCode);
        return SendRspToCu<CRspToCu>(cRsp, m_ptRecvNode);
    }
    void PostErrMsg2Cu(u32 dwErrorCode); 
public:
    CTask *GetInviteTask(s32 nPlayId);
public:
    PTRecvNode m_ptRecvNode;
    string m_strRecordPlaySession;
    s32 m_nplayId;
    string m_strCuSession;
    u32 m_dwEventID;
    TChannel m_tDevChn;
    int m_dwSeqNum;
    string m_strReq;
};

/* ������ҵ�� */
class CSubBaseTask : public CSipTask
{
public:
    typedef set<string> TCuSessionList;

    CSubBaseTask(CInstExt *pcInst, const string& strDevUri, const string& strSvrUri, const string& strSubType);
    virtual ~CSubBaseTask();
public:
    virtual u32 StartSub() = 0;
    virtual u32 StopSub() = 0;
    virtual u32 ReSub() = 0;
    virtual void PrintData();
    string GetKey();
    bool IsOverFlowLimit4Subscribe();
    bool IsOverFlowLimit4Unsubscribe();
    virtual void EraseCuSession(const string &strCuSess);

public:
    TKDSIP_DIALOG_ID m_tSubscribeID;
    string m_strDevUri;
    string m_strSubType;
    TSipURI m_tSvrUri;

protected:
    //�Ѷ��ĵ��û���
    TCuSessionList m_tCuSessionList;
};
template<class CSubReq, class CSubRsp, class CNtfReq, class CNtfRsp, class CUnSubReq, class CUnSubRsp>
class CSubTask : public CSubBaseTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE + 1,
        WaitRsp,
        Ntf,
        WaitUnSub, // �������صȴ�UnSub
        UnSub
    };

    CSubTask(CInstExt *pcInst, const string& strDevUri, const string& strSvrUri, const string& strSubType, CStrMap<u32> &strMapList) : CSubBaseTask(pcInst, strDevUri, strSvrUri, strSubType) 
    {
        m_bIsNtf = false;
        strMapList.Insert(GetKey(), GetTaskNO());
    }

    ~CSubTask()
    {
        ReleaseResource();
    }

    void ReleaseResource()
    {
        //�ͷ�dlg_ID
        if( m_tSubscribeID != INVALID_DIALOG_ID)
        {
            OspSipReleaseDlg(m_tSubscribeID);
            m_tSubscribeID = INVALID_DIALOG_ID;
        }

        if( !m_strDevUri.empty())
        {
            TCuSessionList::iterator item = m_tCuSessionList.begin();
            while(item != m_tCuSessionList.end())
            {
                CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(*item, GetInstance());
                if(NULL != pcCuAgent)
                {
                    //����ֹͣ
                    u32 TaskNum = INVALID_TASKNO;
                    pcCuAgent->m_cDevSubTaskSet.Find(GetKey(), TaskNum);
                    if(TaskNum == GetTaskNO())
                    {
                        pcCuAgent->m_cDevSubTaskSet.Erase(GetKey());
                    }             
                }        
                item++;
            }
            m_tCuSessionList.clear();

            if( PU_ON_OFF_STATUS  == m_strSubType || 
                PU_CONFIG_STATUS  == m_strSubType ||
                PU_SERVICE_STATUS == m_strSubType ||
                PU_ALARM_STATUS   == m_strSubType)
            {
                u32 TaskNum = INVALID_TASKNO;
                g_cCuiDataMgr.m_mapDevUriTypeToSubTaskMap.Find(GetKey(), TaskNum);
                if(TaskNum == GetTaskNO())
                {
                    g_cCuiDataMgr.m_mapDevUriTypeToSubTaskMap.Erase(GetKey());
                }               
            }
            else
            {
                CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tSvrUri.GetURIString(), GetInstance());
                if(NULL != pcPeAgent)
                {
                    u32 TaskNum = INVALID_TASKNO;
                    pcPeAgent->m_cSubscribeTaskSet.Find(GetKey(), TaskNum);
                    if(TaskNum == GetTaskNO())
                    {
                        pcPeAgent->m_cSubscribeTaskSet.Erase(GetKey());
                    }    
                }
            }
            

            m_strDevUri.clear();
        }


        return;
    }

    void PrintData()
    {
        CSubBaseTask::PrintData();

        if(PU_ON_OFF_STATUS  == m_strSubType)
        {
            CPuStatusNtyReq * ptNtfReq = (CPuStatusNtyReq*)&m_cNtfReq;
            ptNtfReq->GetOnOffStatus().PrintData();
        }
        else if(PU_CONFIG_STATUS  == m_strSubType)
        {
            CPuConfigNtyReq* ptNtfReq = (CPuConfigNtyReq*)&m_cNtfReq;
            TPuConfig tPuConfig;
            tPuConfig.MergeDevConfig(ptNtfReq->GetConfig());
            tPuConfig.PrintData();
        }
        else if(PU_SERVICE_STATUS == m_strSubType)
        {
            CPuServiceStatusNtyReq* ptNtfReq = (CPuServiceStatusNtyReq*)&m_cNtfReq;
        }
        else if(PU_ALARM_STATUS == m_strSubType)
        {
            CPuAlarmStatusNtyReq* ptNtfReq = (CPuAlarmStatusNtyReq*)&m_cNtfReq;
        }
        else if (PU_TRANSDATA_STATUS == m_strSubType)
        {
            CPuTransDataStatusNtyReq* ptNtfReq = (CPuTransDataStatusNtyReq*)&m_cNtfReq;
        }
        else if( TVWALL_ALL_STATUS == m_strSubType)
        {
            CTvWallStatusNtyReq* ptNtfReq = (CTvWallStatusNtyReq*)&m_cNtfReq;
        }
        else if( SS_TYPE_PU_PLATFORM_RECORD_STATUS == m_strSubType)
        {
            CRcsPuRecordStatusNtyReq* ptNtfReq = (CRcsPuRecordStatusNtyReq*)&m_cNtfReq;
        }
    }
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CSubTask";
    }

    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case Req:
            {
                return "Req";
            }
            break;
        case WaitRsp:
            {
                return "WaitRsp";
            }
            break;
        case Ntf:
            {
                return "Ntf";
            }
            break;
        case WaitUnSub:
            {
                return "WaitUnSub";
            }
            break;
        case UnSub:
            {
                return "UnSub";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    
    virtual void InitStateMachine()
    {
        CStateProc cReqProc;
        cReqProc.ProcMsg = (CSipTask::PFProcMsg)&CSubTask::ReqProcMsg;
        cReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CSubTask::ReqTimerPoll;
        cReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSubTask::ReqProcMsg;
        AddRuleProc(Req, cReqProc);

        CStateProc cRspProc;
        cRspProc.ProcMsg = (CSipTask::PFProcMsg)&CSubTask::RspProcMsg;
        cRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CSubTask::RspTimerPoll;
        cRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSubTask::RspProcMsg;
        AddRuleProc(WaitRsp, cRspProc);

        CStateProc cNtfProc;
        cNtfProc.ProcMsg = (CSipTask::PFProcMsg)&CSubTask::NtfProcMsg;
        cNtfProc.TimerPoll = &CTask::TimerPoll;
        cNtfProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSubTask::NtfProcMsg;
        AddRuleProc(Ntf, cNtfProc);

        CStateProc cWaitUnSubProc;
        cWaitUnSubProc.ProcMsg = (CSipTask::PFProcMsg)&CSubTask::NtfProcMsg;
        cWaitUnSubProc.TimerPoll = (CSipTask::PFTimerPoll)&CSubTask::WaitUnSubTimerPoll;
        cWaitUnSubProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSubTask::NtfProcMsg;
        AddRuleProc(WaitUnSub, cWaitUnSubProc);

        CStateProc cUnSubProc;
        cUnSubProc.ProcMsg = (CSipTask::PFProcMsg)&CSubTask::UnSubProcMsg;
        cUnSubProc.TimerPoll = (CSipTask::PFTimerPoll)&CSubTask::UnSubTimerPoll;
        cUnSubProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSubTask::UnSubProcMsg;
        AddRuleProc(UnSub, cUnSubProc);

        NextState(Req);
    }
    virtual bool IsNotNeedPollState()
    {
        if (Ntf == GetState())
        {
            return true;
        }

        return false;
    }
public:
    //�û���Ϣ�Ͷ�����Ϣ����洢
    void FlashCuInfo(PCuAgent pcCuAgent)
    {
        CMS_ASSERT(pcCuAgent != NULL);
        if((Ntf == GetState()||WaitUnSub == GetState()) && true == m_bIsNtf)
        {
            pcCuAgent->m_cNotifyPool.InsertSubNotify(m_cNtfReq);
        }

        m_tCuSessionList.insert(pcCuAgent->m_strSession);
        pcCuAgent->m_cDevSubTaskSet.Insert(GetKey(), GetTaskNO());
    }

    void EarseCuInfo(PCuAgent pcCuAgent)
    {
        CMS_ASSERT(pcCuAgent != NULL);

        EraseCuSession(pcCuAgent->m_strSession);
        pcCuAgent->m_cDevSubTaskSet.Erase(GetKey());
    }

    string IsService()
    {
        string strNull;

        if(m_tSvrUri.GetCmuURI().GetURIString() == m_tSvrUri.GetURIString())
        {
            return g_cCuiDataMgr.m_cCuiState.m_strSession;
        }

        CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tSvrUri.GetURIString(), GetInstance());
        CMS_ASSERT( NULL != pcPeAgent );

        if((CPeAgent::Service != pcPeAgent->GetState() || pcPeAgent->m_strSessionID.empty()))
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "����ʧ�ܣ�ģ��[%s]δ�������̬\n",
                m_tSvrUri.GetURIString().c_str());
            return strNull;
        }

        return pcPeAgent->m_strSessionID;
    }

    u32 StartSub()
    {
        if (IsOverFlowLimit4Subscribe())
        {
            return PROCMSG_FAIL; 
        }

        string strSeesionID = IsService();
        if(strSeesionID.empty())
        {
            return PROCMSG_FAIL;
        }

        CSubReq cReq;

        cReq.SetDevUri(m_strDevUri);
        cReq.SetUserUri(g_cCuiConfig.GetLocalURI().GetURIString());
        cReq.SetSession(strSeesionID);
        cReq.GetSsTypes().insert(m_strSubType);

        if(CMS_SUCCESS != PostReq<CSubReq>(KDSIP_EVENT_SUBSCRIBE_REQ, cReq, m_tSvrUri))
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��Ϣ����ʧ��[%s--%hu] �豸Uri[%s]\n",
                cReq.GetEventStr().c_str(), cReq.GetEventId(), m_strDevUri.c_str());

            return PROCMSG_FAIL;
        }
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��Ϣ���ͳɹ�[%s--%hu] �豸Uri[%s]\n",
            cReq.GetEventStr().c_str(), cReq.GetEventId(), m_strDevUri.c_str());

        NextState(WaitRsp);
        return PROCMSG_OK;
        
    }

    u32 StopSub()
    {
        string strSessionID = IsService();
        if(strSessionID.empty())
        {
            return PROCMSG_FAIL;
        }

        CUnSubReq cReq;

        cReq.SetDevUri(m_strDevUri);
        cReq.SetUserUri(g_cCuiConfig.GetLocalURI().GetURIString());
        cReq.SetSession(strSessionID);

        if(CMS_SUCCESS != PostInDlgReq<CUnSubReq>(KDSIP_EVENT_UNSUBSCRIBE_REQ, cReq, m_tSubscribeID))
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��Ϣ����ʧ��[%s--%hu] �豸Uri[%s]\n",
                cReq.GetEventStr().c_str(), cReq.GetEventId(), m_strDevUri.c_str());
        }
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��Ϣ���ͳɹ�[%s--%hu] �豸Uri[%s]\n",
            cReq.GetEventStr().c_str(), cReq.GetEventId(), m_strDevUri.c_str());

        NextState(UnSub);
        return PROCMSG_OK;
    }

    u32 ReSub()
    {
        OspSipReleaseDlg(m_tSubscribeID);
        m_tSubscribeID = INVALID_DIALOG_ID;

        m_bIsNtf = false;

        if(TVWALL_STATUS_NTF_REQ == (u16)m_cNtfReq.GetEventId())
        {
            CTvWallStatusNtyReq* ptNtfReq = (CTvWallStatusNtyReq*)&m_cNtfReq;
            ptNtfReq->Clear();
            ptNtfReq->SetDevURI(m_strDevUri);
            NotifyAllUser();
        }

        //�л�״̬���·�����
        NextState(Req);

        return PROCMSG_OK;
    }

    void EraseCuSession(const string &strCuSess)
    {
        m_tCuSessionList.erase(strCuSess);
        if (m_tCuSessionList.empty() && GetState() == Ntf)
        {
            NextState(WaitUnSub);
        }
    }
public:
    //״̬����
    u32 ReqProcMsg(CMessage *const pcMsg)
    {
        //SIP�������
        if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::ReqProcMsg]��rsp״̬�յ�������Ϣ[%s--%hu],��������[%s] �豸Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, m_strSubType.c_str(),
                m_strDevUri.c_str());

            StopSub();

            return PROCMSG_FAIL;
        }
        else if(pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::ReqProcMsg]��req״̬�յ�������Ϣ[%s--%hu], ��������[%s] �豸Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, m_strSubType.c_str(),
                m_strDevUri.c_str());
            delete this;
            return PROCMSG_DEL;
        }

        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CSubRsp cRsp;
        if(pcMsg->event == cRsp.GetEventId())
        {
            OspSipReleaseDlg(pcOspSipMsg->GetSipDlgID());
            return PROCMSG_FAIL;
        }

        CMS_ASSERT(0&&"ReqProcMsg�յ�δ֪��Ϣ");
        return PROCMSG_OK;
    }

    u32 ReqTimerPoll()
    {
        //����Ƶ��
        if ( GetCurStateHoldTime() > 2 )
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "[CSubTask::ReqTimerPoll]���Ͷ�������ʱ�����·�����[TaskNO %u],��������[%s] �豸Uri[%s]\n",
                GetTaskNO(), m_strSubType.c_str(),
                m_strDevUri.c_str());

            if(m_tCuSessionList.empty())
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "û���û�����TcsSubTask[TaskNO %u]���ģ�ɾ��������, ��������[%s] �豸Uri[%s]\n",
                    GetTaskNO(), m_strSubType.c_str(),
                    m_strDevUri.c_str());

                return TIMERPOLL_DEL;
            }

            NextState(Req);
            StartSub();
        }
        return TIMERPOLL_DONE;
    }

    u32 RspProcMsg(CMessage *const pcMsg)
    {
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��[CSubTask::RspProcMsg]�յ���Ϣ[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        u32 dwRet = PROCMSG_FAIL;

        //SIP�������
        if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL )
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::RspProcMsg]��rsp״̬�յ�������Ϣ[%s--%hu], ��������[%s] �豸Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, m_strSubType.c_str(),
                m_strDevUri.c_str());

            //���·�����
            NextState(Req);
            return PROCMSG_FAIL;
        }
        else if(pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::RspProcMsg]��rsp״̬�յ�������Ϣ[%s--%hu], ��������[%s] �豸Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, m_strSubType.c_str(),
                m_strDevUri.c_str());
            delete this;
            return PROCMSG_DEL;
        }

        CSubRsp cRsp;
        if((u16)cRsp.GetEventId() == pcMsg->event)
        {
            pcOspSipMsg->GetMsgBody(cRsp);

            if( cRsp.GetErrorCode() != CMS_SUCCESS) 
            {
                //ҵ��㶩��ʧ�ܣ������˶�
                TASKLOG(NORMAL_TASK, ERROR_LEV, "�����豸[%s]״̬ʧ�ܣ�������[%d]������ȡ����������, ��������[%s]\n",
                    m_strDevUri.c_str(), cRsp.GetErrorCode(), m_strSubType.c_str());

                m_bIsNtf = false;
                OspSipReleaseDlg(pcOspSipMsg->GetSipDlgID());  

                //�������ʧ�����¶���
                NextState(Req);
            }
            else
            {
                //һ���ʹ洢SubID
                m_tSubscribeID = pcOspSipMsg->GetSipDlgID();

                if(m_bIsNtf)
                {

                    NotifyAllUser();                

                    //���ڸ澯�ָ����⴦��
                    if( PU_ALARM_STATUS_NTF_REQ == (u16)m_cNtfReq.GetEventId())
                    {
                        CPuAlarmStatusNtyReq* ptNtfReq = (CPuAlarmStatusNtyReq*)&m_cNtfReq;
                        if(ptNtfReq->GetAlarmStatus().GetAlarmArray().size() > 0)
                        {
                            CAlarmInputArray::iterator item = ptNtfReq->GetAlarmStatus().GetAlarmArray().begin();
                            while(item != ptNtfReq->GetAlarmStatus().GetAlarmArray().end())
                            {
                                if( ALARM_FLAG_RESTORE == item->GetAlarmFlag())
                                {
                                    item = ptNtfReq->GetAlarmStatus().GetAlarmArray().erase(item);
                                }
                                else
                                {
                                    item++;
                                }
                            }  
                        }                        
                    }
                }        

                TASKLOG(NORMAL_TASK, EVENT_LEV, "�豸[%s]���ĳɹ�,, ��������[%s] �豸Uri[%s]\n",
                    m_strDevUri.c_str(), m_strSubType.c_str(), m_strDevUri.c_str());
                NextState(Ntf);

                dwRet = PROCMSG_OK;
            }
        }
        else if((u16)m_cNtfReq.GetEventId() == pcMsg->event)
        {
            if(PU_CONFIG_NTF_REQ == (u16)m_cNtfReq.GetEventId())
            {
                CPuConfigNtyReq cReq;
                pcOspSipMsg->GetMsgBody(cReq);

                CPuConfigNtyReq* ptNtfReq = (CPuConfigNtyReq*)&m_cNtfReq;
                ptNtfReq->GetConfig().MergeDevConfig(cReq.GetConfig());
            }
            else
            {
                pcOspSipMsg->GetMsgBody(m_cNtfReq);
            }

            
            m_bIsNtf = true;

            TASKLOG(NORMAL_TASK, ERROR_LEV, "��Rsp״̬�յ���Ϣ[%s--%hu]�����豸[%s], ��������[%s]  �豸Uri[%s]\n",
                             OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                             m_strDevUri.c_str(), m_strSubType.c_str(),
                             m_strDevUri.c_str());
        }
        else
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }

        return dwRet;
    }

    u32 RspTimerPoll()
    {
        if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "[CSubTask::RspTimerPoll]���Ͷ�������ʱ[%d]�����·�����[TaskNO %u],��������[%s]  �豸Uri[%s]\n",
                 g_cCuiConfig.GetDlgWaitRspTimeout(),GetTaskNO(), m_strSubType.c_str(), m_strDevUri.c_str());

            if(0 == m_tCuSessionList.size())
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "û���û����ж��ģ�ֹͣ�ö���CSubTask[TaskNO %u],��������[%s] �豸Uri[%s]\n",
                    GetTaskNO(), m_strSubType.c_str(),
                    m_strDevUri.c_str());
                return TIMERPOLL_DEL;
            }

            NextState(Req);
        }

        return TIMERPOLL_DONE;
    }

    void NotifyAllUser()
    {
        TCuSessionList::iterator item = m_tCuSessionList.begin();
        while(item != m_tCuSessionList.end())
        {
            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(*item, GetInstance());
            if(NULL != pcCuAgent)
            {
                pcCuAgent->m_cNotifyPool.InsertSubNotify(m_cNtfReq);
            }

            item++;
        }   
    }

    u32 NtfProcMsg(CMessage *const pcMsg)
    {
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��[CSubTask::NtfProcMsg]�յ���Ϣ[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        u32 dwRet = PROCMSG_FAIL;

        //SIP�������
        if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::RspProcMsg]��ntf״̬�յ�������Ϣ[%s--%hu], ��������[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                m_strSubType.c_str());

            ReSub();
            return PROCMSG_FAIL;
        }
        else if(pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::NtfProcMsg]��ntf״̬�յ�������Ϣ[%s--%hu], ��������[%s] �豸Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, m_strSubType.c_str(),
                m_strDevUri.c_str());
            delete this;
            return PROCMSG_DEL;
        }

        if((u16)m_cNtfReq.GetEventId() == pcMsg->event)
        {
            m_bIsNtf = true;
            
            pcOspSipMsg->GetMsgBody(m_cNtfReq);
            NotifyAllUser();

            CNtfRsp cRsp;
            cRsp.SetEvent(m_cNtfReq.GetEventId() + 1);
            cRsp.SetSession(m_cNtfReq.GetSession());
            cRsp.SetSeqNum(m_cNtfReq.GetSeqNum());

            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            if (m_tCuSessionList.empty())
            {
                NextState(WaitUnSub);
            }

            return PROCMSG_OK;
        }
        
        CSubRsp cRsp;
        if((u16)cRsp.GetEventId() == pcMsg->event)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "��ntf״̬�յ���Ϣ[%s--%hu]�����豸[%s]״̬������ȡ����������, ��������[%s] �豸Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                m_strDevUri.c_str(), m_strSubType.c_str(),
                m_strDevUri.c_str());

            pcOspSipMsg->GetMsgBody(cRsp);

            if( cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                string strSessionID = IsService();
                if(strSessionID.empty())
                {
                    return PROCMSG_FAIL;
                }

                CUnSubReq cReq;
                cReq.SetDevUri(m_strDevUri);
                cReq.SetUserUri(g_cCuiConfig.GetLocalURI().GetURIString());
                cReq.SetSession(strSessionID);

                if(CMS_SUCCESS != PostInDlgReq<CUnSubReq>(KDSIP_EVENT_UNSUBSCRIBE_REQ, cReq, pcOspSipMsg->GetSipDlgID()))
                {
                    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��Ϣ����ʧ��[%s--%hu]\n",
                        cReq.GetEventStr().c_str(), cReq.GetEventId());
                    OspSipReleaseDlg(pcOspSipMsg->GetSipDlgID());  
                }

            }
            else
            {
                OspSipReleaseDlg(pcOspSipMsg->GetSipDlgID());   
            }                     

            return PROCMSG_OK;
        }

        //����unsubRsp
        CUnSubRsp cUnSubRsp;
        if((u16)cUnSubRsp.GetEventId() == pcMsg->event)
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ��쳣ȡ��������Ϣ[%s--%hu] �豸Uri[%s]\n",
                cUnSubRsp.GetEventStr().c_str(), cUnSubRsp.GetEventId(),
                m_strDevUri.c_str());
        }
        else
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu] �豸Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                m_strDevUri.c_str());
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }

        return dwRet;
    }

    u32 WaitUnSubTimerPoll()
    {
        if(m_tCuSessionList.empty())
        {
            if (!IsOverFlowLimit4Unsubscribe())
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "û���û����ж��ģ�ֹͣ�ö���CSubTask[TaskNO %u], ��������[%s] �豸Uri[%s]\n",
                    GetTaskNO(), 
                    m_strSubType.c_str(),
                    m_strDevUri.c_str());

                StopSub();
            }
        }
        else
        {
            NextState(Ntf);
        }

        return TIMERPOLL_DONE;
    }

    u32 UnSubProcMsg(CMessage *const pcMsg)
    {
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��[CSubTask::UnSubProcMsg]�յ���Ϣ[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

        //SIP�������
        if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL || pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::RspProcMsg]��rsp״̬�յ�������Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            delete this;
            return PROCMSG_DEL;
        }

        CUnSubRsp cRsp;
        if((u16)cRsp.GetEventId() == pcMsg->event)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ�[%s--%hu]��Ϣ��ͬ[%s]ģ�鶩������[dev:%s]��ȡ��, ��������[%s] �豸Uri[%s]. Rsp error code:%d\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                m_tSvrUri.GetURIString().c_str(), m_strDevUri.c_str(),
                m_strSubType.c_str(),
                m_strDevUri.c_str(), cRsp.GetErrorCode());

            if (m_tCuSessionList.empty())
            {
                delete this;
                return PROCMSG_DEL;
            }
            else
            {
                OspSipReleaseDlg(m_tSubscribeID);
                m_tSubscribeID = INVALID_DIALOG_ID;

                NextState(Req);
                StartSub();

                return PROCMSG_OK;
            }
        }
        else
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "[CSubTask::UnSubProcMsg]�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }

        return PROCMSG_FAIL;
    }

    u32 UnSubTimerPoll()
    {
        if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::UnSubTimerPoll]�ȴ�[%s]��ȡ������Ӧ��ʱ������CSubTask[TaskNO %u], ��������[%s] �豸Uri[%s]\n",
                m_tSvrUri.GetURIString().c_str(),GetTaskNO(),
                m_strSubType.c_str(),
                m_strDevUri.c_str());

            return TIMERPOLL_DEL;
        }
        return TIMERPOLL_DONE;
    }

public:
    bool m_bIsNtf;
    CNtfReq m_cNtfReq;
};
typedef CSubTask<CTvWallStatusSsReq, CTvWallStatusSsRsp, CTvWallStatusNtyReq, CTvWallStatusNtyRsp, CTvWallStatusUnSsReq, CTvWallStatusUnSsRsp> CTvsSubTask;

class CSubBatchTask : public CSipTask // �������ĸ����ࣨ����Ҫ�ռ���������
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //����״̬��δ��������
    };
public:
    CSubBatchTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CSubBatchTask();
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CSubBatchTask";
    }

    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NOT_REQUEST_STATE:
            {
                return "Not Request State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    //״̬����
    u32 NotReqProcMsg(CMessage *const pcMsg);
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;
};

// ��UAS��¼�û�����
class CUsrOprLogTask : public CCallBackTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WaitRsp,                        //�Ѿ��������󣬵ȴ�����Ӧ��
    };
public:
    CUsrOprLogTask(CInstExt *pcInst, const string& strCuSession, const string& strPeUri, 
        const int operateType,const u32 operateResult, const string& operateDesc, const string& strDevUri);
    ~CUsrOprLogTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CUsrOprLogTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case Req:
            {
                return "Req";
            }
            break;
        case WaitRsp:
            {
                return "Wait Rsp";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void TerminateTask();
    virtual void InitStateMachine();
public:
    u32 StartSendReq();
    string GetPeSession();
    void SetLogInfo(const int operateType,const u32 operateResult, const string& operateDesc, const string& strDevUri);
    u32 StartSendLog();
public:
    //״̬����
    u32 ReqProcMsg(CMessage *const pcMsg);
    u32 ReqTimerPoll();
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
public:
    TOperateLogInfo m_tLogInfo;
    s32 m_nTrySendNum;
    string m_strPeSession;
};
// ��ȡTAS�е�PU����
class CGetPuData : public CSipTask
{
public:
    enum
    {
        WaitGetName = UNKNOWN_STATE + 1,
        WaitRspName,
        WaitNtfName,
        WaitGetVidSrcAlias,
        WaitRspVidSrcAlias
    };
public:
    CGetPuData(CInstExt *pcInst, const string& strCuSession, const TChannel& tDevChn, const u32 dwErrorCode,  const string& strReq, const u32 dwEvent, const string& strDomainName = GetDomainName()) : CSipTask(pcInst)
    {
        m_strCuSession = strCuSession;
        m_tDevChn = tDevChn;
        m_dwErrorCode = dwErrorCode;
        m_dwEvent = dwEvent;
        m_tTasURI = g_cCuiConfig.GetLocalURI().Get3ASURI();
        m_strReq = strReq;
        m_strDomainName = strDomainName;
    }
    ~CGetPuData();
public:
    virtual const char* GetObjName() const
    {
        return "CGetPuData";
    }
    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitGetName:
            return "WaitGetName";
        case WaitRspName:
            return "WaitRspName";
        case WaitNtfName:
            return "WaitNtfName";
        case WaitGetVidSrcAlias:
            return "WaitGetVidSrcAlias";
        case WaitRspVidSrcAlias:
            return "WaitRspVidSrcAlias";
        default:
            break;
        }
        return "Unknown State";
    }
public:
    virtual void InitStateMachine();
public:
    u32 OnWaitGetName(CMessage *const pcMsg);
    u32 OnWaitGetNameTimer();
    u32 OnWaitRspName(CMessage *const pcMsg);
    u32 OnWaitRspNameTimer();
    u32 OnWaitNtfName(CMessage *const pcMsg);
    u32 OnWaitNtfNameTimer();
    u32 OnWaitGetVidSrcAlias(CMessage *const pcMsg);
    u32 OnWaitGetVidSrcAliasTimer();
    u32 OnWaitRspVidSrcAlias(CMessage *const pcMsg);
    u32 OnWaitRspVidSrcAliasTimer();
public:
    u32 StartQryName();
    u32 StartQryVidSrcAlias();
public:
    string WriteLogDesc(const string& strOpt, const string& strDevName, const string& strVidSrcAlaias);
    string WriteLogDesc(const string& strAction, const string& strOpt, const string& strDevName, const string& strVidSrcAlaias);
    string GetLogDesc(const string& strDevName, const string& strVidSrcAlaias);
    u32 StartWriteLog();
    u32 WriteLog(const string& strLogDesc);
public:
    string m_strCuSession;
    TChannel m_tDevChn;
    string m_strQueryTag;
    u32 m_dwErrorCode;
    u32 m_dwEvent;
    string m_strReq;
    TSipURI m_tTasURI;
    string m_strDomainName;
	string m_strDevName;
	string m_strVSname;
};

#endif

