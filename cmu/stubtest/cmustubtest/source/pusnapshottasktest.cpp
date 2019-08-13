
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/utility/base64codec.h"

#include "pusnapshottasktest.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"
#include "cmustubtestcoreapp.h"
#include "culogintasktest.h"


/*
qrypurec "3d31caec96534a5498cb1aca2de064df@jiangsu"
*/

CPuSnapshotTask::CPuSnapshotTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSipTransId = INVALID_TRANS_ID;
    m_pPicFile = NULL;
}

CPuSnapshotTask::~CPuSnapshotTask()
{
    if (m_pPicFile != NULL)
    {
        fclose(m_pPicFile);
        m_pPicFile = NULL;
    }
}

void CPuSnapshotTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CPuSnapshotTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSnapshotTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CPuSnapshotTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cRspProc;
    cRspProc.ProcMsg = (CTask::PFProcMsg)&CPuSnapshotTask::OnRsp;
    cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSnapshotTask::OnRsp;
    cRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuSnapshotTask::OnRspTimer;
    AddRuleProc(Rsp, cRspProc);

    CStateProc cNtfProc;
    cNtfProc.ProcMsg = (CTask::PFProcMsg)&CPuSnapshotTask::OnNtf;
    cNtfProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSnapshotTask::OnNtf;
    cNtfProc.TimerPoll = (CTask::PFTimerPoll)&CPuSnapshotTask::OnNtfTimer;
    AddRuleProc(Ntf, cNtfProc);

    NextState(Wait);
}

void CPuSnapshotTask::PrintData()
{
    CTask::PrintData();
}

u32 CPuSnapshotTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_PIC_SNAPSHOT_REQ:
        {         
            string strDevUri;
            if (pcMsg->content == NULL)
            {
                strDevUri = TEST_PU_URI;
            }
            else
            {
                const char* szPuUri = (const char*) (pcMsg->content);
                strDevUri = szPuUri;
            }

            CCuLoginTask* ptCuLoginTask = NULL;

            Iterator pPos;
            string strSession;
            CLoginSession* pTask = NULL;
            while (!pPos.End())
            {
                if (g_cCuList.Iterate(pPos, strSession, pTask) && pTask != NULL)
                {
                    ptCuLoginTask = (CCuLoginTask*)(pTask);
                    break;
                }
            }

            if (ptCuLoginTask == NULL)
            {
                TASKLOG(ERROR_LEV, "没有cu用户登录\n");
                return PROCMSG_DEL;
            }

            CPuData* ptPuData = g_cPuList.FindPuData(strDevUri);
            if (ptPuData == NULL)
            {
                TASKLOG(ERROR_LEV, "设备[%s]不存在, 创建新设备\n",
                    strDevUri.c_str());

                CPuData* ptPuData = new CPuData;
                ptPuData->SetDevUri(strDevUri);
                g_cPuList.AddPuData(ptPuData);
            }

            CPuPicSnapshotReq cReq;
            cReq.GetDevChn().SetDevUri(strDevUri);
            cReq.GetDevChn().SetChnNO(0);

            m_tReq = cReq;

            PostMsgReq(cReq, GetCmuUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuSnapshotTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuSnapshotTask::OnRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_PIC_SNAPSHOT_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CPuPicSnapshotRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            m_strPicID = cRsp.GetPicID();

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_CU, EVENT_LEV, "前端抓拍成功. picID[%s]\n",
                    m_strPicID.c_str());

                string strPicFileName = m_strPicID;
                strPicFileName += ".jpg";
                m_pPicFile = fopen(strPicFileName.c_str(),"wb");
                if (m_pPicFile == NULL)
                {
                    TASKLOG(MOD_CU, EVENT_LEV, "创建文件[%s]失败\n",
                        strPicFileName.c_str());
                    return PROCMSG_DEL;
                }

                //开始下载
                CPuPicDownloadReq cReq;
                cReq.SetDevChn(m_tReq.GetDevChn());
                cReq.SetPicID(m_strPicID);
                cReq.SetPackIndex(1);

                PostMsgReq(cReq, GetCmuUri());

                NextState(Ntf);
            }
            else
            {
                //失败
                TASKLOG(MOD_CU, ERROR_LEV, "前端抓拍失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuSnapshotTask::OnRspTimer()
{
    if (GetCurStateHoldTime() > 30)
    {
        TASKLOG(MOD_CU, ERROR_LEV, "pu pic snapshot timeout!\n");
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuSnapshotTask::OnNtf(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_PIC_DOWNLOAD_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CPuPicDownloadRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_CU, EVENT_LEV, "图片包传输成功. picID[%s]-TotalPack[%d]-CurPack[%d]\n",
                    m_strPicID.c_str(), cRsp.GetPicData().GetTotalPack(), cRsp.GetPicData().GetCurPackIndex());

                string strData = Base64Decode(cRsp.GetPicData().GetData());
                //写文件
                int nRet = fwrite(strData.c_str(), 1, strData.length(), m_pPicFile);
                if (0 == nRet)
                {			
                    TASKLOG(MOD_CU, ERROR_LEV, "fwrite失败\n");
                    return PROCMSG_DEL;
                }

                if (cRsp.GetPicData().GetCurPackIndex() < cRsp.GetPicData().GetTotalPack())
                {                
                    //没下载完，继续
                    CPuPicDownloadReq cReq;
                    cReq.SetDevChn(m_tReq.GetDevChn());
                    cReq.SetPicID(m_strPicID);
                    cReq.SetPackIndex(cRsp.GetPicData().GetCurPackIndex()+1);

                    PostMsgReq(cReq, GetCmuUri());

                    NextState(Ntf);
                }
                else
                {
                    TASKLOG(MOD_CU, EVENT_LEV, "图片成功下载. picID[%s]\n",
                        m_strPicID.c_str());

                    fclose(m_pPicFile);
                    m_pPicFile = NULL;

                    return PROCMSG_DEL;
                }
            }
            else
            {
                //失败
                TASKLOG(MOD_CU, ERROR_LEV, "前端抓拍失败\n");
                return PROCMSG_DEL;
            }

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuSnapshotTask::OnNtfTimer()
{
    return TIMERPOLL_DONE;
}


CPuPicQryTask::CPuPicQryTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSipTransId = INVALID_TRANS_ID;
    m_dwTotalNum = 0;
    m_dwRecvNum = 0;
    m_pPicFile = NULL;
}

CPuPicQryTask::~CPuPicQryTask()
{
    if (!m_strQryTag.empty())
    {
        g_cStubQryTaskList.Erase(m_strQryTag);
    }

    if (m_pPicFile != NULL)
    {
        fclose(m_pPicFile);
        m_pPicFile = NULL;
    }
}

void CPuPicQryTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CPuPicQryTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPicQryTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CPuPicQryTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cRspProc;
    cRspProc.ProcMsg = (CTask::PFProcMsg)&CPuPicQryTask::OnRsp;
    cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPicQryTask::OnRsp;
    cRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuPicQryTask::OnRspTimer;
    AddRuleProc(Rsp, cRspProc);

    CStateProc cNtfProc;
    cNtfProc.ProcMsg = (CTask::PFProcMsg)&CPuPicQryTask::OnNtf;
    cNtfProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPicQryTask::OnNtf;
    cNtfProc.TimerPoll = (CTask::PFTimerPoll)&CPuPicQryTask::OnNtfTimer;
    AddRuleProc(Ntf, cNtfProc);

    CStateProc cDownloadProc;
    cDownloadProc.ProcMsg = (CTask::PFProcMsg)&CPuPicQryTask::OnDownload;
    cDownloadProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPicQryTask::OnDownload;
    cDownloadProc.TimerPoll = (CTask::PFTimerPoll)&CPuPicQryTask::OnDownloadTimer;
    AddRuleProc(Download, cDownloadProc);

    NextState(Wait);
}

void CPuPicQryTask::PrintData()
{
    CTask::PrintData();
}

u32 CPuPicQryTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_PIC_QRY_REQ:
        {         
            string strDevUri;
            if (pcMsg->content == NULL)
            {
                strDevUri = TEST_PU_URI;
            }
            else
            {
                const char* szPuUri = (const char*) (pcMsg->content);
                strDevUri = szPuUri;
            }

            CCuLoginTask* ptCuLoginTask = NULL;

            Iterator pPos;
            string strSession;
            CLoginSession* pTask = NULL;
            while (!pPos.End())
            {
                if (g_cCuList.Iterate(pPos, strSession, pTask) && pTask != NULL)
                {
                    ptCuLoginTask = (CCuLoginTask*)(pTask);
                    break;
                }
            }

            if (ptCuLoginTask == NULL)
            {
                TASKLOG(ERROR_LEV, "没有cu用户登录\n");
                return PROCMSG_DEL;
            }

            CPuData* ptPuData = g_cPuList.FindPuData(strDevUri);
            if (ptPuData == NULL)
            {
                TASKLOG(ERROR_LEV, "设备[%s]不存在, 创建新设备\n",
                    strDevUri.c_str());

                CPuData* ptPuData = new CPuData;
                ptPuData->SetDevUri(strDevUri);
                g_cPuList.AddPuData(ptPuData);
            }

            CPuPicQryReq cReq;
            cReq.GetDevChn().SetDevUri(strDevUri);
            cReq.GetDevChn().SetChnNO(0);
            cReq.SetExpectEntryNum(20);

            int nQryWay = PU_PIC_CHAN_QUERY;//|PU_PIC_TIME_QUERY|PU_PIC_EVENT_QUERY;
            cReq.SetQryWay(nQryWay);
            cReq.SetQryEvent(PU_EVENT_TYPE_ALL);
            CCMSTime cStartTime;
            cStartTime.SetLocalTime(2012, 9, 14, 1, 1, 1);
            CCMSTime cEndTime;
            cEndTime.SetLocalTime(2013, 3, 19, 1, 1, 1);
            cReq.GetTimeRange().SetStartTime(cStartTime.ToString());
            cReq.GetTimeRange().SetEndTime(cEndTime.ToString());

            m_tReq = cReq;

            PostMsgReq(cReq, GetCmuUri());

            //等待Rsp
            NextState(Rsp);

            g_cStubQryTaskList[cReq.GetQueryTag()] = this;

            m_strQryTag = cReq.GetQueryTag();

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuPicQryTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuPicQryTask::OnRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_PIC_QRY_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CPuPicQryRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_CU, EVENT_LEV, "前端图片查询成功. 总数[%d]\n",
                    cRsp.GetTotalEntryNum());
                m_dwTotalNum = cRsp.GetTotalEntryNum();

                if (cRsp.GetTotalEntryNum() == 0)
                {
                    dwRet = PROCMSG_DEL;
                }
                else
                {
                    dwRet = PROCMSG_OK;
                    NextState(Ntf);
                }
            }
            else
            {
                //失败
                TASKLOG(MOD_CU, ERROR_LEV, "前端图片查询失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuPicQryTask::OnRspTimer()
{
    if (GetCurStateHoldTime() > 30000)
    {
        TASKLOG(MOD_CU, ERROR_LEV, "query pu pic timeout!\n");
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuPicQryTask::OnNtf(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_PIC_QRY_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CPuPicQryNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            for (int i=0; i<cReq.GetPicInfoList().size(); i++)
            {
                m_cPicInfoList.push_back(cReq.GetPicInfoList().at(i));
            }
            

            m_dwRecvNum += cReq.GetEntryNum();

            CPuPicQryNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            TASKLOG(MOD_CU, EVENT_LEV, "前端图片通知:[%s]\n",
                cReq.ToXml().c_str());

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (cReq.IsLastNtf())
            {
                TASKLOG(MOD_CU, ERROR_LEV, "前端图片通知结束\n");

                if (m_dwTotalNum > m_dwRecvNum)
                {
                    //继续查
                    CPuPicQryReq cReq;
                    cReq.SetDevChn(m_tReq.GetDevChn());
                    cReq.SetExpectEntryNum(20);
                    cReq.SetStartEntryIndex(m_dwRecvNum);

                    int nQryWay = PU_PIC_CHAN_QUERY;//|PU_PIC_TIME_QUERY|PU_PIC_EVENT_QUERY;
                    cReq.SetQryWay(nQryWay);
                    cReq.SetQryEvent(PU_EVENT_TYPE_ALL);
                    CCMSTime cStartTime;
                    cStartTime.SetLocalTime(2012, 9, 14, 1, 1, 1);
                    CCMSTime cEndTime;
                    cEndTime.SetLocalTime(2013, 3, 19, 1, 1, 1);
                    cReq.GetTimeRange().SetStartTime(cStartTime.ToString());
                    cReq.GetTimeRange().SetEndTime(cEndTime.ToString());

                    m_tReq = cReq;

                    PostMsgReq(cReq, GetCmuUri());

                    //等待Rsp
                    NextState(Rsp);

                    g_cStubQryTaskList[cReq.GetQueryTag()] = this;

                    m_strQryTag = cReq.GetQueryTag();
                }
                else
                {
                    TASKLOG(MOD_CU, EVENT_LEV, "前端图片查询全部结束\n");

                    //开始下载
                    TASKLOG(MOD_CU, EVENT_LEV, "开始下载\n");

                    const TPuPicInfo& tPicInfo = m_cPicInfoList.at(0);
                    m_strPicID = tPicInfo.GetPicID();

                    string strPicFileName = m_strPicID;
                    strPicFileName += ".jpg";
                    m_pPicFile = fopen(strPicFileName.c_str(),"wb");
                    if (m_pPicFile == NULL)
                    {
                        TASKLOG(MOD_CU, EVENT_LEV, "创建文件[%s]失败\n",
                            strPicFileName.c_str());
                        return PROCMSG_DEL;
                    }

                    CPuPicDownloadReq cReq;
                    cReq.SetDevChn(m_tReq.GetDevChn());
                    
                    cReq.SetPicID(m_strPicID);
                    cReq.SetPackIndex(1);

                    PostMsgReq(cReq, GetCmuUri());

                    NextState(Download);
                }
            }

            //开始下载

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuPicQryTask::OnNtfTimer()
{
    return TIMERPOLL_DONE;
}


u32 CPuPicQryTask::OnDownload(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_PIC_DOWNLOAD_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CPuPicDownloadRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_CU, EVENT_LEV, "图片包传输成功. picID[%s]-TotalPack[%d]-CurPack[%d]\n",
                    m_strPicID.c_str(), cRsp.GetPicData().GetTotalPack(), cRsp.GetPicData().GetCurPackIndex());

                string strData = Base64Decode(cRsp.GetPicData().GetData());
                //写文件
                int nRet = fwrite(strData.c_str(), 1, strData.length(), m_pPicFile);
                if (0 == nRet)
                {			
                    TASKLOG(MOD_CU, ERROR_LEV, "fwrite失败\n");
                    return PROCMSG_DEL;
                }

                if (cRsp.GetPicData().GetCurPackIndex() < cRsp.GetPicData().GetTotalPack())
                {                
                    //没下载完，继续
                    CPuPicDownloadReq cReq;
                    cReq.SetDevChn(m_tReq.GetDevChn());
                    cReq.SetPicID(m_strPicID);
                    cReq.SetPackIndex(cRsp.GetPicData().GetCurPackIndex()+1);

                    PostMsgReq(cReq, GetCmuUri());

                    NextState(Download);
                }
                else
                {
                    TASKLOG(MOD_CU, EVENT_LEV, "图片成功下载. picID[%s]\n",
                        m_strPicID.c_str());

                    fclose(m_pPicFile);
                    m_pPicFile = NULL;

                    return PROCMSG_DEL;
                }
            }
            else
            {
                //失败
                TASKLOG(MOD_CU, ERROR_LEV, "前端抓拍失败\n");
                return PROCMSG_DEL;
            }

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuPicQryTask::OnDownloadTimer()
{
    return TIMERPOLL_DONE;
}