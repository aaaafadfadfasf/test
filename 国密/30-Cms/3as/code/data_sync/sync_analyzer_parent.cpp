#include "sync_analyzer_parent.h"
#include "../sync_analyzer_mng.h"

void CSyncAnalyzerParent::InitStateMachine()
{
    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncAnalyzerParent::ServerNonblockStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncAnalyzerBase::PollSyncDataLst);
    AddRuleProc(enStateServerNonblock, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncAnalyzerParent::ServerBlockStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncAnalyzerParent::ServerBlockStateTimerPoll);
    AddRuleProc(enStateServerBlock, cProc);

    EnterServerNonblockState();
}

const char* CSyncAnalyzerParent::GetStateName( u32 dwState ) const
{
    if (enStateServerNonblock == dwState)
    {
        return "enStateServerNonblock";
    }
    else if (enStateServerBlock == dwState)
    {
        return "enStateServerBlock";
    }
    else
    {
        return "Unknown";
    }
}

u32 CSyncAnalyzerParent::ServerNonblockStateProcMsg( CMessage *const pcMsg )
{
    return DefaultProc(pcMsg);
}


u32 CSyncAnalyzerParent::ServerBlockStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == msgDbMsgTransfer)
    {
        CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
        if (pcTransBase->GetEventId() == msgSyncDataRecordStoreRsp)
        {
            CSyncAnalyzerBase::DefaultProc(pcMsg);
        }
        else if (pcTransBase->GetEventId() == msgSyncDevInfoRelateDataGetRsp)
        {
            CDbMsgTransfer<TSyncDevInfoRelateDataGetRsp> *pcRsp = static_cast<CDbMsgTransfer<TSyncDevInfoRelateDataGetRsp> *>(pcTransBase);
            if (pcRsp->GetMsgObj().GetErrno() == 0)
            {
                TSyncDevInfoRelateDataGetRsp &cRsp = pcRsp->GetMsgObj();

                CSyncAnalyzerBase::Analyze(m_tQueringDevInfoRelateData);

                TSyncDataAnalyzeInfo tData;
                for (u32 i = 0; i < cRsp.devCapIndexs.size(); ++i)
                {
                    ConstructSyncDataAnalyzeInfo(tData, enSyncDataDeviceCapIndexName, enSyncOperateMod, m_tQueringDevInfoRelateData.domainId, 
                        &cRsp.devId, cRsp.devCapIndexs[i].id, cRsp.devCapIndexs[i].indexs[0]);

                    CSyncAnalyzerBase::Analyze(tData);
                }

                for (u32 i = 0; i < cRsp.devGrpDevKeys.size(); ++i)
                {
                    const CDeviceGroupDeviceInfoKey &cDevGrpDevKey = cRsp.devGrpDevKeys[i];
                    ConstructSyncDataAnalyzeInfo(tData, enSyncDataDeviceGroup2Dev, enSyncOperateAdd, m_tQueringDevInfoRelateData.domainId,
                        &cDevGrpDevKey.GetDeviceGroupID(), &cDevGrpDevKey.GetDeviceID(), 
                        cDevGrpDevKey.GetDeviceCapIndexs()[0].id, cDevGrpDevKey.GetDeviceCapIndexs()[0].indexs[0]);

                    CSyncAnalyzerBase::Analyze(tData);
                }

                EnterServerNonblockState();
            }
            else if (pcRsp->GetMsgObj().GetErrno() == ERR_TAS_INVALID_SYNCDATA)
            {
                // 无效同步数据时直接丢弃
                EnterServerNonblockState();
            }
            else
            {
                // 放开开关，后续自动重试
                m_bForbidQueringDevInfoRelateData = false;
            }
        }
    }

    return PROCMSG_OK;
}

u32 CSyncAnalyzerParent::ServerBlockStateTimerPoll()
{
    QueryDevInfoRelationData();
    return TIMERPOLL_DONE;
}


void CSyncAnalyzerParent::Analyze( const TSyncDataAnalyzeInfo &tData )
{
    CSyncAnalyzerAppData& cAppData = static_cast<CSyncAnalyzerInst*>(GetInstance())->GetAppData();

    do 
    {
        /* 通过域过滤同步数据 */
        if (!(tData.domainId == CurrentDomainId() || cAppData.IsSubDomainData(tData.domainId)))
        {
            break;
        }

        /* 通过数据类型过滤同步数据 */
        if (tData.dataType == enSyncDataUserDevice)
        {
            break;
        }

        /* 过滤本域私有设备的相关数据 */
        if ((tData.domainId == CurrentDomainId()) && 
            (tData.dataType == enSyncDataDeviceCapIndexName
            || tData.dataType == enSyncDataDeviceGroup2Dev))
        {
            s8 szDevId[DEVID_LEN+1] = {'\0'};
            if (tData.dataType == enSyncDataDeviceCapIndexName)
            {
                sscanf(tData.dataId.c_str(), "%s", szDevId);
            }
            else // enSyncDataDeviceGroup2Dev
            {
                sscanf(tData.dataId.c_str(), "%*s%s", szDevId);
            }

            if (cAppData.IsPrivateDev(szDevId))
            {
                break;
            }
        } 

        if (enStateServerNonblock == GetState())
        {
            if (tData.dataType == enSyncDataDeviceInfo)
            {
                AnalyzeDevInfo(tData);
            }
            else
            {
                CSyncAnalyzerBase::Analyze(tData);
            }
        }
        else
        {
            // 请求暂时缓存起来
            m_tTempSyncDataLst.push_back(tData);
        }
    } while (false);
}

void CSyncAnalyzerParent::AnalyzeDevInfo( const TSyncDataAnalyzeInfo &tData )
{
    s8 szDevId[DEVID_LEN+1] = {'\0'};
    u32 dwDevType, dwDevPrivateNow, dwDevPrivatePrev;
    sscanf(tData.dataId.c_str(), "%s%u%u%u", szDevId, &dwDevType, &dwDevPrivateNow, &dwDevPrivatePrev);
    
    if (tData.operateType == enSyncOperateAdd)
    {
        if (dwDevPrivateNow)
        {
            return;
        }

        NextState(enStateServerBlock);
        QueryDevInfoRelationData(&tData);
    }
    else if (tData.operateType == enSyncOperateDel)
    {
        if (dwDevPrivateNow)
        {
            return;
        }

        CSyncAnalyzerBase::Analyze(tData);
    }
    else // enSyncOperateMod
    {
        TSyncDataAnalyzeInfo tNewData = tData;
        if (dwDevPrivatePrev && !dwDevPrivateNow)
        { // 私有变为非私有需要发起添加同步
            NextState(enStateServerBlock);
            tNewData.operateType = enSyncOperateAdd;
            QueryDevInfoRelationData(&tNewData);
        }
        else if (!dwDevPrivatePrev && dwDevPrivateNow)
        { // 非私有变为私有需要发起删除同步 hzytodo2 与跨域授权一起工作时存在问题
            tNewData.operateType = enSyncOperateDel;
            CSyncAnalyzerBase::Analyze(tNewData);
        }
        else if (!dwDevPrivatePrev)
        { // 改前与改后都是非私有发起修改同步
            CSyncAnalyzerBase::Analyze(tNewData);
        }
        else
        {
            // 私有设备无需同步
        }
    }
}

void CSyncAnalyzerParent::QueryDevInfoRelationData( const TSyncDataAnalyzeInfo *ptSyncData /*= NULL*/ )
{
    // 新的分析
    if (NULL != ptSyncData)
    {
        m_tQueringDevInfoRelateData = *ptSyncData;
        m_bForbidQueringDevInfoRelateData = false;
    }

    if (m_bForbidQueringDevInfoRelateData)
    {
        return;
    }

    s8 szDevId[DEVID_LEN+1] = {'\0'};
    sscanf(m_tQueringDevInfoRelateData.dataId.c_str(), "%s", szDevId);
    CDbMsgTransfer<string> cReq(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSyncDevInfoRelateDataGetReq);
    cReq.GetMsgObj() = szDevId;
    m_bForbidQueringDevInfoRelateData = PostDbMsgTransfer2DbTaskPool(cReq);
}

void CSyncAnalyzerParent::HandleTempSyncDataList()
{
    CMS_ASSERT(GetState() == enStateServerNonblock);

    while(!m_tTempSyncDataLst.empty())
    {
        TSyncDataAnalyzeInfo tData = m_tTempSyncDataLst.front();
        m_tTempSyncDataLst.pop_front();

        if (tData.dataType == enSyncDataDeviceInfo)
        {
            AnalyzeDevInfo(tData);
            if (GetState() != enStateServerNonblock) break;
        }
        else
        {
            CSyncAnalyzerBase::Analyze(tData);
        }
    }
}

void CSyncAnalyzerParent::EnterServerNonblockState()
{
    NextState(enStateServerNonblock);
    HandleTempSyncDataList();
}
