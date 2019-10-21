/*****************************************************************************
模块名      : cui_server 
文件名      : normalcutask.h
相关文件    : 
文件实现功能: normalcutask.h 定义所有CU常规请求事务
作者        : liangli
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/11/9   1.0         liangli        创建
2013/07/29              huzhiyun       代码重构
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
    SERVER_3AS = 0,     //查询的目标服务器是3AS
    SERVER_CMU,
    SERVER_RCS,
    SERVER_TVWALL,
    SERVER_TYPE_NUM
};

/* req-rsp类简单业务 */
class CSingleInterAction : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_RESPONSE_STATE,                        //已经发送请求，等待单次应答
    };
public:
    CSingleInterAction(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CSingleInterAction();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CSingleInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CSingleInterAction[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
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
        { //3AS还在线
            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());
            CMS_ASSERT( NULL != pc3acAgent );
            CMS_ASSERT( 0 == strcmp(pc3acAgent->GetObjName(),"C3acAgent") );
            const TSipURI &tDstURI = pc3acAgent->m_t3asURI;
            FakePostMsgReq(cReq, tDstURI);
        }
        else
        { //3AS下线
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
    //状态函数
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();

    //处理NotReq状态下的消息的泛型函数
    template<typename CReq>
    u32 OnNotReqMsg(CMessage *const pcMsg, EServerType eSendToDst = SERVER_CMU)
    {
        CMS_ASSERT(GetState() == NOT_REQUEST_STATE);

        CReq *pcReq = (CReq *)pcMsg->content;
        //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
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
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
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

        //2 转换状态，等待应答
        NextState(WAIT_RESPONSE_STATE);
        //3 设置超时消息处理
        
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
        //任务完成, 销毁之
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
class CBatchInterAction : public CSipTask // 批量操作辅助类（需要收集处理结果）
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_RESPONSE_STATE,                        //已经发送请求，等待单次应答
    };
public:
    CBatchInterAction(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CBatchInterAction();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CBatchInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatchInterAction[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
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
        { //首次发送消息给TAS
            //查找3AS任务
            if (m_pcCuAgent->IsTasOnline())
            {
                pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());
                CMS_ASSERT(NULL != pc3acAgent);
                CMS_ASSERT( 0 == strcmp(pc3acAgent->GetObjName(), "C3acAgent") );
                m_pc3acAgent = pc3acAgent;
            }
            else
            { //TAS不在线
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

        //判断是否已经收集完所有结果
        if ( m_tRspMarks.IsAllReqHasRsp() )
        {
            CRspToCu *pcRsp = (CRspToCu *)m_pRsp;
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]在类CBatchInterAction中，所有设备都收到应答，批处理任务完成\n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
            SendRspToCu<CRspToCu>(*pcRsp, m_ptRecvNode);
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        return PROCMSG_OK;
    }
public:
    //状态函数
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;
    C3acAgent *m_pc3acAgent;    //如果涉及向3AS查询，则该指针不为空
    CEventRsp *m_pRsp;
    CRspMarks<CXMap<int, bool>, int> m_tRspMarks;
};

// 查询req-rsp-ntfreq-ntfrsp类业务(利用queryTag路由)
class CQueryTask : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //空闲状态，未发送查询请求
        WAIT_RESPONSE_STATE,                        //等待查询Rsp
        WAIT_NOTIFY_STATE,                          //等待查询Ntf
    };
public:
    CQueryTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CQueryTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CQueryTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
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
    //状态函数
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
    u32 WaitNotifyTimerPoll();

    //NotReq状态下的泛型处理
    //因为CReq不能透传，CUI需要转换CU和3AS之间的数据，所以这里不直接处理CMessage
    //数据转换放到外面完成
    /*====================================================================
    函数名      : OnNotQueryMsg
    功能        : NotReq状态下的泛型处理
    因为CReq不能透传，CUI需要转换CU和3AS之间的数据，所以这里不直接处理CMessage
    数据转换放到外面完成
    算法实现    :
    引用全局变量:
    输入参数说明: CReqToSvr &cReq            请求类型必须是服务器定义的接口类型
    EServerType eSendToDst     服务器是3AS或者CMU，以后还有可能是RecMgr
    返回值说明  : 成功:true，失败:false
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/2/3/             liangli
    ====================================================================*/
    template<typename CReqToSvr, typename CRspToCu>
    u32 OnNotQueryMsg(CReqToSvr &cReq, EServerType eSendToDst = SERVER_3AS)
    {
        CMS_ASSERT(GetState() == NOT_QUERY_STATE);
        //缓存req消息，如果在wait rsp state收到ntf消息好处理
        m_dwReqEvent = cReq.GetEventId();
        //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
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
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
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

        //将QueryTag插入到会话表中，等待NTF
        g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());
        m_strQueryTag = cReq.GetQueryTag();

        //2 转换状态，等待应答
        NextState(WAIT_RESPONSE_STATE);

        //3 创建结果保存数据，超时时复用此结果数据
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
        { //查询失败 或者 无结果
            if ( cRsp.GetErrorCode() != CMS_SUCCESS )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CQueryTask[NO: %u]查询应答[%s--%d]返回失败，错误码[%d]\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[NO: %u]查询应答[%s--%d]成功，结果为空\n",
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

        TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%d]，本次Ntf中含带[%d]条记录\n",
            cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
            cNtf.GetEntryNum());

        //先回复NTF通知
        CNtfToSvr cRsp;
        cRsp.SetEvent(pcMsg->event+1);
        cRsp.SetSeqNum(cNtf.GetSeqNum());
        cRsp.SetSession(cNtf.GetSession());
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

        CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
        pcRspToCu->push_back(cNtf, this);

        //判断是否后续还有Ntf
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
            NextState(WAIT_NOTIFY_STATE);  //切换状态以清空轮询计数，继续超时判断
        }
        return PROCMSG_OK;
    }
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;      //该指针一定不为空
    C3acAgent *m_pc3acAgent;    //只有当查询成功后(说明TAS在线)，该指针才有效，为了效率缓存
    string m_strRspOfErr;
    CEventRsp *m_pRsp;
    string m_strQueryTag;       //当其不为空时，表面在m_mapSessToTaskMap中登记过
    std::list<string> m_VecNtfReqList; //缓存在rsp消息中接受到得ntf消息
    u16 m_dwReqEvent;
};


// 查询req-rsp-ntf类业务(利用queryTag路由)
class CNotifyTransferTask : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //空闲状态，未发送查询请求
        WAIT_RESPONSE_STATE,                        //等待查询Rsp
        WAIT_NOTIFY_STATE,                          //等待查询Ntf
    };
public:
    CNotifyTransferTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CNotifyTransferTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CNotifyTransferTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CNotifyTransferTask[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
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
    //状态函数
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
    u32 WaitNotifyTimerPoll();

    //NotReq状态下的泛型处理
    //因为CReq不能透传，CUI需要转换CU和3AS之间的数据，所以这里不直接处理CMessage
    //数据转换放到外面完成
    /*====================================================================
    函数名      : OnNotQueryMsg
    功能        : NotReq状态下的泛型处理
    因为CReq不能透传，CUI需要转换CU和3AS之间的数据，所以这里不直接处理CMessage
    数据转换放到外面完成
    算法实现    :
    引用全局变量:
    输入参数说明: CReqToSvr &cReq            请求类型必须是服务器定义的接口类型
    EServerType eSendToDst     服务器是3AS或者CMU，以后还有可能是RecMgr
    返回值说明  : 成功:true，失败:false
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/2/3/             liangli
    ====================================================================*/
    template<typename CReqToSvr, typename CRspToCu>
    u32 OnNotQueryMsg(CReqToSvr &cReq, EServerType eSendToDst = SERVER_3AS)
    {
        CMS_ASSERT(GetState() == NOT_QUERY_STATE);
        //缓存req消息，如果在wait rsp state收到ntf消息好处理
        m_dwReqEvent = cReq.GetEventId();
		m_tChannel = cReq.GetDevChn();
        //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
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
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
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

        //将QueryTag插入到会话表中，等待NTF
        g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());
        m_strQueryTag = cReq.GetQueryTag();

        //2 转换状态，等待应答
        NextState(WAIT_RESPONSE_STATE);

        //3 创建结果保存数据，超时时复用此结果数据
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
        { //查询失败 或者 无结果
            if ( cRsp.GetErrorCode() != CMS_SUCCESS )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CQueryTask[NO: %u]查询应答[%s--%d]返回失败，错误码[%d]\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[NO: %u]查询应答[%s--%d]成功，不含有NTF通知\n",
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

        TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%d]，本次Ntf中含带[%d]条记录\n",
            cNtf.GetEventStr().c_str(), cNtf.GetEventId(),cNtf.GetEntryNum());

        //先回复NTF通知
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

        //判断是否后续还有Ntf
        if ( cNtf.IsLastNtf() )
        {
            g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
            m_strQueryTag.clear();
            NextState(NOT_QUERY_STATE);
            delete this;
        }
        else
        {
            NextState(WAIT_NOTIFY_STATE);  //切换状态以清空轮询计数，继续超时判断
        }
        return PROCMSG_OK;
    }
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;      //该指针一定不为空
    C3acAgent *m_pc3acAgent;    //只有当查询成功后(说明TAS在线)，该指针才有效，为了效率缓存
    string m_strRspOfErr;
    CEventRsp *m_pRsp;
    string m_strQueryTag;       //当其不为空时，表面在m_mapSessToTaskMap中登记过
    std::list<string> m_VecNtfReqList; //缓存在rsp消息中接受到得ntf消息
	TChannel m_tChannel;        //qry channel
    u16 m_dwReqEvent;
};

/* invite类业务 优化网络适应性方案，支持新NAT穿越方案*/
class CInviteTaskEx : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_SETUP_RSP_STATE,
        /*
        invite的流程是
        1，UAC发送invite_req
        2，UAS发送invite_rsp
        3，此时会话还未完全建立，需要等UAC再发送invite_ack
        如果一段时间内还没有ack，则底层协议栈会销毁会话，并通知上层OSP_SIP_DIALOG_TERMINATE
        上层收到OSP_SIP_DIALOG_TERMINATE后，调用OspSipReleaseDlg
        4，UAC发送invite_ack，UAS可以处理此消息，也可以不处理
        如果是手拉手交换，则应该在此处开始发送码流
        invite_ack是一条单向的通知消息，协议栈不会给UAC任何回应
        5，当UAS收到invite_ack，表示会话已经建立，UAS可以处理该消息，也可以不处理
        6，在会话开始建立的任何阶段，UAC都可以发送Bye来终止会话
        UAS如果是在InviteRsp之前发送ByeReq，其实是非法操作，协议栈会转为4.3错误做应答
        UAS只有在呼叫成功（InviteRsp）后才能“挂断”（Bye），否则只能拒绝（403错误码的InviteRsp）
        7，BYE是SIP消息，没有定义相应的业务层消息，BYE的应答由协议栈自己完成，并且也没有消息体
        为了删除OspSip的Bye事务绑定，上层还是要主动发送ByeRsp

        当CUI收到CMU的ByeReq时，底层协议栈就自己发送ByeRsp了
        但是由于OspSip层还存在Invite会话绑定关系，所以上层还是要调一次ByeRsp，让OspSip删除绑定关系
        CUI端的OspSip收到ByeRsp时，不调用kdsip底层接口，只删除绑定关系
        CMU端的OspSip收到kdsip自动回复的ByeRsp，也删除绑定关系并向上层回调OSP_SIP_BYE_RSP
        */
        WAIT_SETUP_ACK_STATE,   //此状态没有应答
        INVITE_HOLD_STATE,
        WAIT_BYE_RSP_STATE,
    };
public:
    CInviteTaskEx(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask,s32 nPlayId);
    ~CInviteTaskEx();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CInviteTaskEx";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
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
    //对外接口
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
    CXMap<int, int> m_cChildTaskList; //对话内事务列表，比如vcr控制事务
	s32 m_nPlayId;                    //当前会话对应的cu playid
    CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};

/* invite类业务 */
class CInviteTask : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_SETUP_RSP_STATE,
        /*
        invite的流程是
        1，UAC发送invite_req
        2，UAS发送invite_rsp
        3，此时会话还未完全建立，需要等UAC再发送invite_ack
        如果一段时间内还没有ack，则底层协议栈会销毁会话，并通知上层OSP_SIP_DIALOG_TERMINATE
        上层收到OSP_SIP_DIALOG_TERMINATE后，调用OspSipReleaseDlg
        4，UAC发送invite_ack，UAS可以处理此消息，也可以不处理
        如果是手拉手交换，则应该在此处开始发送码流
        invite_ack是一条单向的通知消息，协议栈不会给UAC任何回应
        5，当UAS收到invite_ack，表示会话已经建立，UAS可以处理该消息，也可以不处理
        6，在会话开始建立的任何阶段，UAC都可以发送Bye来终止会话
        UAS如果是在InviteRsp之前发送ByeReq，其实是非法操作，协议栈会转为4.3错误做应答
        UAS只有在呼叫成功（InviteRsp）后才能“挂断”（Bye），否则只能拒绝（403错误码的InviteRsp）
        7，BYE是SIP消息，没有定义相应的业务层消息，BYE的应答由协议栈自己完成，并且也没有消息体
        为了删除OspSip的Bye事务绑定，上层还是要主动发送ByeRsp

        当CUI收到CMU的ByeReq时，底层协议栈就自己发送ByeRsp了
        但是由于OspSip层还存在Invite会话绑定关系，所以上层还是要调一次ByeRsp，让OspSip删除绑定关系
        CUI端的OspSip收到ByeRsp时，不调用kdsip底层接口，只删除绑定关系
        CMU端的OspSip收到kdsip自动回复的ByeRsp，也删除绑定关系并向上层回调OSP_SIP_BYE_RSP
        */
        WAIT_SETUP_ACK_STATE,   //此状态没有应答
        INVITE_HOLD_STATE,
        WAIT_BYE_RSP_STATE,
    };
public:
    CInviteTask(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask, s32 nPlayId);
    ~CInviteTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CInviteTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
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
    //对外接口
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
    CXMap<int, int> m_cChildTaskList; //对话内事务列表，比如vcr控制事务

	s32 m_nPlayId;                    //当前会话对应的cu playid
    CUserStreamNumRestorer m_cStrmNumRestorer;
private:
    u32 m_dwErrorCode;
};
class CInviteInterAction : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_RESPONSE_STATE,                        //已经发送请求，等待单次应答
    };
public:
    CInviteInterAction(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CInviteInterAction();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CInviteInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
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

/* 第三方呼叫 优化网络适应性，支持新穿NAT方案*/
class CCallPuTaskEx : public CSipTask
{
public:
        /*
        invite的流程是
        1，UAC发送invite_req
        2，UAS发送invite_rsp
        3，此时会话还未完全建立，需要等UAC再发送invite_ack
        如果一段时间内还没有ack，则底层协议栈会销毁会话，并通知上层OSP_SIP_DIALOG_TERMINATE
        上层收到OSP_SIP_DIALOG_TERMINATE后，调用OspSipReleaseDlg
        4，UAC发送invite_ack，UAS可以处理此消息，也可以不处理
        如果是手拉手交换，则应该在此处开始发送码流
        invite_ack是一条单向的通知消息，协议栈不会给UAC任何回应
        5，当UAS收到invite_ack，表示会话已经建立，UAS可以处理该消息，也可以不处理
        6，在会话开始建立的任何阶段，UAC都可以发送Bye来终止会话
        UAS如果是在InviteRsp之前发送ByeReq，其实是非法操作，协议栈会转为4.3错误做应答
        UAS只有在呼叫成功（InviteRsp）后才能“挂断”（Bye），否则只能拒绝（403错误码的InviteRsp）
        7，BYE是SIP消息，没有定义相应的业务层消息，BYE的应答由协议栈自己完成，并且也没有消息体
        为了删除OspSip的Bye事务绑定，上层还是要主动发送ByeRsp

        当CUI收到CMU的ByeReq时，底层协议栈就自己发送ByeRsp了
        但是由于OspSip层还存在Invite会话绑定关系，所以上层还是要调一次ByeRsp，让OspSip删除绑定关系
        CUI端的OspSip收到ByeRsp时，不调用kdsip底层接口，只删除绑定关系
        CMU端的OspSip收到kdsip自动回复的ByeRsp，也删除绑定关系并向上层回调OSP_SIP_BYE_RSP
        */
    enum
    {
        //NOT_3PC_REQ_STATE ,
        WAIT_SETUP_REQ_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_ACK_STATE,
        INVITE_HOLD_STATE,
    };
public:
    CCallPuTaskEx(CInstExt *pcInst, const string &strCuSession,string strCallPuReq, CSipTask *pcTask);
    ~CCallPuTaskEx();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CCallPuTaskEx";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
    u32 WaitSetUpReqProcMsg(CMessage *const pcMsg);
    u32 WaitSetUpReqTimerPoll();
    u32 WaitAckProcMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();
    u32 InviteHoldProcMsg(CMessage *const pcMsg);
	void ProcNatCBMsg(CMessage *const pcMsg);//NAT探测包回调地址，如果地址有变化需要通知VTDU 更改交换地址

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
    //第三方呼叫中DevChn是唯一标识
    TChannel m_tCuChn;
    string m_strModuleId;
    //TPuChnList m_tPuChnIDList;

private:
    //音频呼叫不做码流限制
    //CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};

/* 第三方呼叫 */
class CCallPuTask : public CSipTask
{
public:
        /*
        invite的流程是
        1，UAC发送invite_req
        2，UAS发送invite_rsp
        3，此时会话还未完全建立，需要等UAC再发送invite_ack
        如果一段时间内还没有ack，则底层协议栈会销毁会话，并通知上层OSP_SIP_DIALOG_TERMINATE
        上层收到OSP_SIP_DIALOG_TERMINATE后，调用OspSipReleaseDlg
        4，UAC发送invite_ack，UAS可以处理此消息，也可以不处理
        如果是手拉手交换，则应该在此处开始发送码流
        invite_ack是一条单向的通知消息，协议栈不会给UAC任何回应
        5，当UAS收到invite_ack，表示会话已经建立，UAS可以处理该消息，也可以不处理
        6，在会话开始建立的任何阶段，UAC都可以发送Bye来终止会话
        UAS如果是在InviteRsp之前发送ByeReq，其实是非法操作，协议栈会转为4.3错误做应答
        UAS只有在呼叫成功（InviteRsp）后才能“挂断”（Bye），否则只能拒绝（403错误码的InviteRsp）
        7，BYE是SIP消息，没有定义相应的业务层消息，BYE的应答由协议栈自己完成，并且也没有消息体
        为了删除OspSip的Bye事务绑定，上层还是要主动发送ByeRsp

        当CUI收到CMU的ByeReq时，底层协议栈就自己发送ByeRsp了
        但是由于OspSip层还存在Invite会话绑定关系，所以上层还是要调一次ByeRsp，让OspSip删除绑定关系
        CUI端的OspSip收到ByeRsp时，不调用kdsip底层接口，只删除绑定关系
        CMU端的OspSip收到kdsip自动回复的ByeRsp，也删除绑定关系并向上层回调OSP_SIP_BYE_RSP
        */
    enum
    {
        //NOT_3PC_REQ_STATE ,
        WAIT_SETUP_REQ_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_ACK_STATE,
        INVITE_HOLD_STATE,
    };
public:
    CCallPuTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcTask);
    ~CCallPuTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CCallPuTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
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
    //第三方呼叫中DevChn是唯一标识
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
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_RESPONSE_STATE,                        //已经发送请求，等待单次应答
    };

	enum ECallPuTaskType
	{
		CALL_PU_TASK_TYPE_PUNCH = 0,		// 支持老的punch方式
		CALL_PU_TASK_TYPE_NAT,				// nat探测包模式
		CALL_PU_TASK_TYPE_CUI_NatSwitch,	// CUI转发码流模式
	};

public:
    CCallPuInterAction(CInstExt *pcInst, const string& strCuSession, PTRecvNode ptRecvNode);
    ~CCallPuInterAction();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CCallPuInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
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

/* 外设的SingleInterAction,QueryTask,InviteTask参见非外设的 */
class CPeSingleInterAction : public CCallBackTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_RESPONSE_STATE,                        //已经发送请求，等待单次应答
    };
public:
    CPeSingleInterAction(CInstExt *pcInst, const string &strCuSession, const string &strPeUri, PTRecvNode ptRecvNode);
    ~CPeSingleInterAction();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CPeSingleInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeSingleInterAction[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
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
    //状态函数
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

        //考虑失败的结果
        CRsp cRsp;
        cRsp.SetSession(pcReq->GetSession());
        cRsp.SetSeqNum(pcReq->GetSeqNum());
        cRsp.SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = cRsp.ToXml();
    }

    //处理NotReq状态下的消息的泛型函数
    template<typename CReq, typename CRsp>
    u32 OnNotReqMsg(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_OK;
        CMS_ASSERT(GetState() == NOT_REQUEST_STATE);

        CReq *pcReq = (CReq *)pcMsg->content;

        //缓存错误消息，如果失败可以返回消息
        SetErrorRsp<CRsp>(pcReq);
        //缓存req消息
        m_strReq = pcReq->ToXml();

        string strPeSession = GetPeSession();
        if(!strPeSession.empty())
        {
            pcReq->SetSession(strPeSession);
            dwRet = PostMsgReqToPe(*pcReq);
            if(dwRet != PROCMSG_OK)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "收到[%s--%d]消息，操作外设[%s]CU用户会话[%s]发送消息失败\n",
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
            TASKLOG(NORMAL_TASK, ERROR_LEV, "收到[%s--%d]消息，操作外设[%s]CU用户会话[%s]收到错误码[%d]\n",
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
    string m_strReq; //缓存的req消息
};
class CPeQueryTask : public CCallBackTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //空闲状态，未发送查询请求
        WAIT_RESPONSE_STATE,                        //等待查询Rsp
        WAIT_NOTIFY_STATE,                          //等待查询Ntf
    };
public:
    CPeQueryTask(CInstExt *pcInst, const string& strCuSession, const string& strPeUri, PTRecvNode ptRecvNode);
    ~CPeQueryTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CPeQueryTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeQueryTask[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
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
    //状态函数
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitReqTimerPoll();
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
    u32 WaitNotifyTimerPoll();

    //void SetErrorRsp(CEventReq *pcReq);
    //NotReq状态下的泛型处理
    //因为CReq不能透传，CUI需要转换CU和3AS之间的数据，所以这里不直接处理CMessage
    //数据转换放到外面完成
    /*====================================================================
    函数名      : OnNotQueryMsg
    功能        : NotReq状态下的泛型处理
    因为CReq不能透传，CUI需要转换CU和3AS之间的数据，所以这里不直接处理CMessage
    数据转换放到外面完成
    算法实现    :
    引用全局变量:
    输入参数说明: CReqToSvr &cReq            请求类型必须是服务器定义的接口类型
    EServerType eSendToDst     服务器是3AS或者CMU，以后还有可能是RecMgr
    返回值说明  : 成功:true，失败:false
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/2/3/             liangli
    ====================================================================*/
    template<typename CReq, typename CRspToCu>
    u32 OnNotQueryMsg(CMessage *const pcMsg)
    {
        CMS_ASSERT(GetState() == NOT_QUERY_STATE);
        //缓存req消息，如果在wait rsp state收到ntf消息好处理
        //m_dwReqEvent = cReq.GetEventId();
        //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
        CReq *pcReq = (CReq *)pcMsg->content;
        //缓存错误消息，如果失败可以返回消息
        //存储相应的信息
        m_pRsp = new CRspToCu;
        m_pRsp->SetSeqNum(pcReq->GetSeqNum());
        m_pRsp->SetSession(pcReq->GetSession());
        //存储QueryTag
        g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
        m_strQueryTag = pcReq->GetQueryTag();

        u32 dwRet = PROCMSG_OK;
        
        string strPeSession = GetPeSession();
        if(strPeSession.empty())
        {
            //缓存req消息
            m_strReq = pcReq->ToXml();

			// 不常驻的外设模块特殊处理，如果不在线，直接返回外设不在线 [4/13/2015 pangubing]
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
        { //查询失败 或者 无结果
            if ( cRsp.GetErrorCode() != CMS_SUCCESS )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CPeQueryTask[NO: %u]查询应答[%s--%d]返回失败，错误码[%d]\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeQueryTask[NO: %u]查询应答[%s--%d]成功，结果为空\n",
                    GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
            }

            m_pRsp->SetErrorCode(cRsp.GetErrorCode());
            CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
            //无论是没有结果还是查询出错都填写0
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

        TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%d]，本次Ntf中含带[%d]条记录\n",
            cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
            cNtf.GetEntryNum());

        //先回复NTF通知
        CNtfRsp cRsp;
        cRsp.SetEvent(pcMsg->event+1);
        cRsp.SetSeqNum(cNtf.GetSeqNum());
        cRsp.SetSession(cNtf.GetSession());
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

        CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
        pcRspToCu->push_back(cNtf, this);

        //判断是否后续还有Ntf
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
            NextState(WAIT_NOTIFY_STATE);  //切换状态以清空轮询计数，继续超时判断
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
    string m_strQueryTag;       //当其不为空时，表面在m_mapSessToTaskMap中登记过
    string m_strReq;
    std::list<string> m_VecNtfReqList; //缓存在rsp消息中接受到得ntf消息
};
class CPeInviteTask : public CCallBackTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_SETUP_RSP_STATE,
        WAIT_SETUP_ACK_STATE,   //此状态没有应答
        INVITE_HOLD_STATE,
        WAIT_BYE_RSP_STATE,
    };
public:
    CPeInviteTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcInviteInterActTask, const string &strPeUri, CCuAgent *pcCuAgent, s32 nPlayId);
    ~CPeInviteTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CPeInviteTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
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
    //对外接口
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
    CXMap<int, int> m_cChildTaskList; //对话内事务列表，比如vcr控制事务

	CCuAgent *m_pcCuAgent;
	s32 m_nPlayId;                    //当前会话对应的cu playid
private:
    CUserStreamNumRestorer m_cStrmNumRestorer;
    u32 m_dwErrorCode;
};
class CPeInviteInterAction : public CSipTask
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WAIT_RESPONSE_STATE,                        //已经发送请求，等待单次应答
    };
public:
    CPeInviteInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode);
    virtual ~CPeInviteInterAction();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CPeInviteInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
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

/* 订阅类业务 */
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
    //已订阅的用户表
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
        WaitUnSub, // 由于流控等待UnSub
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
        //释放dlg_ID
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
                    //订阅停止
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
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CSubTask";
    }

    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //用户信息和订阅信息互相存储
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
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "订阅失败，模块[%s]未进入服务态\n",
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
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "消息发送失败[%s--%hu] 设备Uri[%s]\n",
                cReq.GetEventStr().c_str(), cReq.GetEventId(), m_strDevUri.c_str());

            return PROCMSG_FAIL;
        }
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "消息发送成功[%s--%hu] 设备Uri[%s]\n",
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
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "消息发送失败[%s--%hu] 设备Uri[%s]\n",
                cReq.GetEventStr().c_str(), cReq.GetEventId(), m_strDevUri.c_str());
        }
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "消息发送成功[%s--%hu] 设备Uri[%s]\n",
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

        //切换状态重新发起订阅
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
    //状态函数
    u32 ReqProcMsg(CMessage *const pcMsg)
    {
        //SIP层错误处理
        if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::ReqProcMsg]在rsp状态收到错误消息[%s--%hu],订阅类型[%s] 设备Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, m_strSubType.c_str(),
                m_strDevUri.c_str());

            StopSub();

            return PROCMSG_FAIL;
        }
        else if(pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::ReqProcMsg]在req状态收到错误消息[%s--%hu], 订阅类型[%s] 设备Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, m_strSubType.c_str(),
                m_strDevUri.c_str());
            delete this;
            return PROCMSG_DEL;
        }

        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CSubRsp cRsp;
        if(pcMsg->event == cRsp.GetEventId())
        {
            OspSipReleaseDlg(pcOspSipMsg->GetSipDlgID());
            return PROCMSG_FAIL;
        }

        CMS_ASSERT(0&&"ReqProcMsg收到未知消息");
        return PROCMSG_OK;
    }

    u32 ReqTimerPoll()
    {
        //订阅频率
        if ( GetCurStateHoldTime() > 2 )
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "[CSubTask::ReqTimerPoll]发送订阅请求超时，重新发起订阅[TaskNO %u],订阅类型[%s] 设备Uri[%s]\n",
                GetTaskNO(), m_strSubType.c_str(),
                m_strDevUri.c_str());

            if(m_tCuSessionList.empty())
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "没有用户进行TcsSubTask[TaskNO %u]订阅，删除该事务, 订阅类型[%s] 设备Uri[%s]\n",
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
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CSubTask::RspProcMsg]收到消息[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        u32 dwRet = PROCMSG_FAIL;

        //SIP层错误处理
        if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL )
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::RspProcMsg]在rsp状态收到错误消息[%s--%hu], 订阅类型[%s] 设备Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, m_strSubType.c_str(),
                m_strDevUri.c_str());

            //重新发起订阅
            NextState(Req);
            return PROCMSG_FAIL;
        }
        else if(pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::RspProcMsg]在rsp状态收到错误消息[%s--%hu], 订阅类型[%s] 设备Uri[%s]\n",
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
                //业务层订阅失败，发送退订
                TASKLOG(NORMAL_TASK, ERROR_LEV, "订阅设备[%s]状态失败，错误码[%d]，发送取消订阅请求, 订阅类型[%s]\n",
                    m_strDevUri.c_str(), cRsp.GetErrorCode(), m_strSubType.c_str());

                m_bIsNtf = false;
                OspSipReleaseDlg(pcOspSipMsg->GetSipDlgID());  

                //如果订阅失败重新订阅
                NextState(Req);
            }
            else
            {
                //一来就存储SubID
                m_tSubscribeID = pcOspSipMsg->GetSipDlgID();

                if(m_bIsNtf)
                {

                    NotifyAllUser();                

                    //对于告警恢复特殊处理
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

                TASKLOG(NORMAL_TASK, EVENT_LEV, "设备[%s]订阅成功,, 订阅类型[%s] 设备Uri[%s]\n",
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

            TASKLOG(NORMAL_TASK, ERROR_LEV, "在Rsp状态收到消息[%s--%hu]订阅设备[%s], 订阅类型[%s]  设备Uri[%s]\n",
                             OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                             m_strDevUri.c_str(), m_strSubType.c_str(),
                             m_strDevUri.c_str());
        }
        else
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }

        return dwRet;
    }

    u32 RspTimerPoll()
    {
        if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "[CSubTask::RspTimerPoll]发送订阅请求超时[%d]，重新发起订阅[TaskNO %u],订阅类型[%s]  设备Uri[%s]\n",
                 g_cCuiConfig.GetDlgWaitRspTimeout(),GetTaskNO(), m_strSubType.c_str(), m_strDevUri.c_str());

            if(0 == m_tCuSessionList.size())
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "没有用户进行订阅，停止该订阅CSubTask[TaskNO %u],订阅类型[%s] 设备Uri[%s]\n",
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
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CSubTask::NtfProcMsg]收到消息[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        u32 dwRet = PROCMSG_FAIL;

        //SIP层错误处理
        if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::RspProcMsg]在ntf状态收到错误消息[%s--%hu], 订阅类型[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                m_strSubType.c_str());

            ReSub();
            return PROCMSG_FAIL;
        }
        else if(pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::NtfProcMsg]在ntf状态收到错误消息[%s--%hu], 订阅类型[%s] 设备Uri[%s]\n",
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
            TASKLOG(NORMAL_TASK, ERROR_LEV, "在ntf状态收到消息[%s--%hu]订阅设备[%s]状态，发送取消订阅请求, 订阅类型[%s] 设备Uri[%s]\n",
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
                    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "消息发送失败[%s--%hu]\n",
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

        //处理unsubRsp
        CUnSubRsp cUnSubRsp;
        if((u16)cUnSubRsp.GetEventId() == pcMsg->event)
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到异常取消订阅消息[%s--%hu] 设备Uri[%s]\n",
                cUnSubRsp.GetEventStr().c_str(), cUnSubRsp.GetEventId(),
                m_strDevUri.c_str());
        }
        else
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu] 设备Uri[%s]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event,
                m_strDevUri.c_str());
            CMS_ASSERT(0&&"收到未知消息");
        }

        return dwRet;
    }

    u32 WaitUnSubTimerPoll()
    {
        if(m_tCuSessionList.empty())
        {
            if (!IsOverFlowLimit4Unsubscribe())
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "没有用户进行订阅，停止该订阅CSubTask[TaskNO %u], 订阅类型[%s] 设备Uri[%s]\n",
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
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CSubTask::UnSubProcMsg]收到消息[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

        //SIP层错误处理
        if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL || pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::RspProcMsg]在rsp状态收到错误消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            delete this;
            return PROCMSG_DEL;
        }

        CUnSubRsp cRsp;
        if((u16)cRsp.GetEventId() == pcMsg->event)
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "收到[%s--%hu]消息，同[%s]模块订阅事务[dev:%s]被取消, 订阅类型[%s] 设备Uri[%s]. Rsp error code:%d\n",
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
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "[CSubTask::UnSubProcMsg]收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }

        return PROCMSG_FAIL;
    }

    u32 UnSubTimerPoll()
    {
        if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "[CSubTask::UnSubTimerPoll]等待[%s]的取消订阅应答超时，销毁CSubTask[TaskNO %u], 订阅类型[%s] 设备Uri[%s]\n",
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

class CSubBatchTask : public CSipTask // 批量订阅辅助类（不需要收集处理结果）
{
public:
    enum
    {
        NOT_REQUEST_STATE = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
    };
public:
    CSubBatchTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CSubBatchTask();
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CSubBatchTask";
    }

    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
    u32 NotReqProcMsg(CMessage *const pcMsg);
public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;
};

// 向UAS记录用户操作
class CUsrOprLogTask : public CCallBackTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
        WaitRsp,                        //已经发送请求，等待单次应答
    };
public:
    CUsrOprLogTask(CInstExt *pcInst, const string& strCuSession, const string& strPeUri, 
        const int operateType,const u32 operateResult, const string& operateDesc, const string& strDevUri);
    ~CUsrOprLogTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CUsrOprLogTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
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
    //状态函数
    u32 ReqProcMsg(CMessage *const pcMsg);
    u32 ReqTimerPoll();
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
public:
    TOperateLogInfo m_tLogInfo;
    s32 m_nTrySendNum;
    string m_strPeSession;
};
// 获取TAS中的PU数据
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

