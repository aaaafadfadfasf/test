#include "subscribe_task.h"
#include "culogintask.h"
#include "cuidbg.h"
#include "cuidata.h"
#include <ostream>

#include "redisdb.h"

extern CRedisDBInterface g_cRedisDb;
extern void RedisSubscribeCallback(CFastDBInterface */*pInter*/, const std::string &/*chnName*/, const std::string &/*strMsgData*/, void */*userdata*/);

static map<string, CModuleListSubscribeTaskBase*> s_mapModuleListSubscribeTask;

CModuleListSubscribeTaskBase::CModuleListSubscribeTaskBase( 
    CInstExt *pcInst, const string &strModuleUri, const string &strSession)
    : CSipTask(pcInst), m_strModuleUri(strModuleUri), m_strModuleSession(strSession), 
    m_tSubscribeID(INVALID_DIALOG_ID), m_dwRefreshingReqNum(0)
{
    CMS_ASSERT(s_mapModuleListSubscribeTask.find(m_strModuleUri) == s_mapModuleListSubscribeTask.end());
    s_mapModuleListSubscribeTask[strModuleUri] = this;
}

void CModuleListSubscribeTaskBase::ConstructorData( u16 wSubscribeRspEvent, u16 wRefreshRspEvent, vector<u16> vecSupportedNtfReqEvent, vector<string> vecSupportedSubType )
{
    m_wSubscribeRspEvent = wSubscribeRspEvent;
    m_wRefreshRspEvent = wRefreshRspEvent;
    m_vecSupportedNtfReqEvent = vecSupportedNtfReqEvent;
    m_vecSupportedSubType = vecSupportedSubType;
}

CModuleListSubscribeTaskBase::~CModuleListSubscribeTaskBase()
{
    CMS_ASSERT(s_mapModuleListSubscribeTask.find(m_strModuleUri) != s_mapModuleListSubscribeTask.end());
    s_mapModuleListSubscribeTask.erase(m_strModuleUri);

    if (INVALID_DIALOG_ID != m_tSubscribeID)
    {
        OspSipReleaseDlg(m_tSubscribeID);
        m_tSubscribeID = INVALID_DIALOG_ID;
    }

    for (map<CCuAgent*, set<TSubscribeKey> >::iterator it = m_mapUser2Keys.begin();
        it != m_mapUser2Keys.end(); ++it)
    {
        it->first->m_setModuleListSubTask.erase(this);
    }
}

const char* CModuleListSubscribeTaskBase::GetStateName( u32 dwState ) const
{
    switch(dwState)
    {
    case enSubscribe:
        return "subscribeState";
    case enRefreshSub:
        return "refreshSubState";
    case enDisconnect:
        return "disconnnectState";
    default:
        return "NA";
    }
}

void CModuleListSubscribeTaskBase::InitStateMachine()
{
    CStateProc cState;
    cState.ProcMsg = (CSipTask::PFProcMsg)&CModuleListSubscribeTaskBase::SubscribeProc;
    cState.TimerPoll = (CSipTask::PFTimerPoll)&CModuleListSubscribeTaskBase::SubscribeTimerPoll;
    cState.ProcErrMsg = cState.ProcMsg;
    AddRuleProc(enSubscribe, cState);

    cState.ProcMsg = (CSipTask::PFProcMsg)&CModuleListSubscribeTaskBase::RefreshSubProc;
    cState.TimerPoll = (CSipTask::PFTimerPoll)&CModuleListSubscribeTaskBase::RefreshSubTimerPoll;
    cState.ProcErrMsg = cState.ProcMsg;
    AddRuleProc(enRefreshSub, cState);

    cState.ProcMsg = (CSipTask::PFProcMsg)&CModuleListSubscribeTaskBase::DisconnectProc;
    cState.TimerPoll = (CSipTask::PFTimerPoll)&CModuleListSubscribeTaskBase::DisconnectTimerPoll;
    cState.ProcErrMsg = cState.ProcMsg;
    AddRuleProc(enDisconnect, cState);

    NextState(enSubscribe);
    //DoSubscribe();
}

bool CModuleListSubscribeTaskBase::IsRedisSubscribe(const string &strSubType)
{
	if (m_strModuleUri == g_cCuiConfig.GetCmuURI().GetURIString()
		|| strSubType == PU_ON_OFF_STATUS
		|| strSubType == PU_CONFIG_STATUS
		|| strSubType == PU_SERVICE_STATUS
		|| strSubType == PU_ALARM_STATUS
		//|| strSubType == PU_GPS_STATUS  //by zzc 剔除按需订阅功能点
		|| strSubType == PU_TRANSDATA_STATUS)
	{
		return true;
	}

	return false;
}
bool CModuleListSubscribeTaskBase::DoRedisSubscribe(const TSubscribeKey &tKey, bool bAdd)
{
	string subChnName = RedisGetSubChn(GetNameWithoutDomainFromURI(tKey.strDevUri), tKey.strSubType);
	//by zzc 剔除按需订阅功能点
    //string subGpsNtf = "GPS_NTF";
	//if(tKey.strSubType == PU_GPS_STATUS)
	//{
	//	EFDB_ErrorCode error = g_cRedisDb.FDBPublish(subGpsNtf,tKey.strDevUri);
	//	if ( !IsFdbResultPositive(error) )
	//	{
	//		TASKLOG(NORMAL_TASK, ERROR_LEV, "Redis Pub Failed DevUri=%s, er=%d\n", tKey.strDevUri.c_str(), error);
	//		return false;
	//	}
	//}
	if (bAdd == true)
	{
		// cui是单个订阅，后续可优化
		EFDB_ErrorCode er = g_cRedisDb.FDBSubscribe(subChnName, RedisSubscribeCallback, NULL/*&g_cRedisDb*/);
		if ( !IsFdbResultPositive(er) )
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "Redis Sub Failed ChnName=%s, er=%d\n", subChnName.c_str(), er);
			return false;
		}

		TASKLOG(NORMAL_TASK, EVENT_LEV, "Redis Sub ChnName=%s, er=%d\n", subChnName.c_str(), er);
	}
	else
	{
		// 取消订阅
		// cui是单个订阅，后续可优化
		EFDB_ErrorCode er = g_cRedisDb.FDBUnsubscribe(subChnName);
		if ( !IsFdbResultPositive(er) )
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "Redis UnSub Failed ChnName=%s, er=%d\n", subChnName.c_str(), er);
			return false;
		}
		TASKLOG(NORMAL_TASK, EVENT_LEV, "Redis UnSub ChnName=%s, er=%d\n", subChnName.c_str(), er);
	}

	return true;
}

s32 CModuleListSubscribeTaskBase::Refresh( const TSubscribeKey &tKey, CCuAgent *pcUser, bool bAdd )
{
	vector<string>::const_iterator it = m_vecSupportedSubType.begin();
    for (; it != m_vecSupportedSubType.end(); ++it)
    {
        if (*it == tKey.strSubType)
        {
            break;
        }
    }
    if (it == m_vecSupportedSubType.end())
    {
        CMS_ASSERT(false);
        return ERR_CUI_INVALID_PARAM;
    }

	// 如果是redis订阅方式
	if ( IsRedisSubscribe(tKey.strSubType) )
	{
		return RedisRefresh(tKey, pcUser, bAdd);
	}

    if (bAdd)
    {
        bool bNeedAddReqLst = false;
        map<TSubscribeKey, TSubscribeData>::iterator itAdd = m_mapSubscribe.find(tKey);
        if (itAdd != m_mapSubscribe.end())
        {
            if (itAdd->second.setUserlist.empty())
                bNeedAddReqLst = true;

            itAdd->second.setUserlist.insert(pcUser);
        }
        else
        {
            bNeedAddReqLst = true;

            TSubscribeData tData;
            tData.setUserlist.insert(pcUser);
            m_mapSubscribe[tKey] = tData;
        }

        if (bNeedAddReqLst)
        {
            TRefreshSubReq tReq(tKey, bAdd);
            m_lstRefreshSubReq.push_back(tReq);

            if (m_dwRefreshingReqNum == 0)
            {
                DoRefreshSub();
            }
        }

        map<CCuAgent*, set<TSubscribeKey> >::iterator itAdd2 = m_mapUser2Keys.find(pcUser);
        if (itAdd2 != m_mapUser2Keys.end())
        {
            itAdd2->second.insert(tKey);
        }
        else
        {
            set<TSubscribeKey> tData;
            tData.insert(tKey);
            m_mapUser2Keys[pcUser] = tData;

            pcUser->m_setModuleListSubTask.insert(this);
        }

        itAdd = m_mapSubscribe.find(tKey);
        if (!itAdd->second.strNtf.empty())
        {
            TSubscribeData tData;
            tData.strNtf = itAdd->second.strNtf;
            tData.setUserlist.insert(pcUser);
            HandleNtf4Output(itAdd->first.strSubType, tData, itAdd->second.strNtf);
        }
    }
    else
    {
        EraseSubscribeMap(tKey, pcUser);
        EraseUser2KeyMap(pcUser, tKey);
    }

    return CMS_SUCCESS;
}

s32 CModuleListSubscribeTaskBase::RedisRefresh(const TSubscribeKey &tKey, CCuAgent *pcUser, bool bAdd)
{
	vector<string>::const_iterator it = m_vecSupportedSubType.begin();
	for (; it != m_vecSupportedSubType.end(); ++it)
	{
		if (*it == tKey.strSubType)
		{
			break;
		}
	}

	if (it == m_vecSupportedSubType.end())
	{
		CMS_ASSERT(false);
		return ERR_CUI_INVALID_PARAM;
	}

	if ( !IsRedisSubscribe(tKey.strSubType) )
	{
		CMS_ASSERT(false);
		return ERR_CUI_INVALID_PARAM;
	}

	if (bAdd)
	{
		string strNtf;
		bool bNeedSub = true;

		map<TSubscribeKey, TSubscribeData>::iterator itAdd = m_mapSubscribe.find(tKey);
		if (itAdd != m_mapSubscribe.end())
		{
			if ( !itAdd->second.setUserlist.empty() )
			{
				bNeedSub = false;
			}

			set<CCuAgent*>::const_iterator itUser = itAdd->second.setUserlist.find(pcUser);
			if (itUser != itAdd->second.setUserlist.end())
			{
				// 重复订阅
				// redis可以直接取数据
				GetNtfByKeyFromRedis(tKey, strNtf);

				if (!strNtf.empty())
				{
					TSubscribeData tData;
					tData.setUserlist.insert(pcUser);
					HandleNtf4Output(tKey.strSubType, tData, strNtf);
				}
				return CMS_SUCCESS;
			}
		}

		if ( bNeedSub && !DoRedisSubscribe(tKey, bAdd) )
		{
			return ERR_CUI_INVALID_PARAM;
		}

		if (itAdd != m_mapSubscribe.end())
		{
			itAdd->second.setUserlist.insert(pcUser);
		}
		else
		{
			TSubscribeData tData;
			tData.setUserlist.insert(pcUser);
			m_mapSubscribe[tKey] = tData;
		}
		
		map<CCuAgent*, set<TSubscribeKey> >::iterator itAgent = m_mapUser2Keys.find(pcUser);
		if (itAgent != m_mapUser2Keys.end())
		{
			if ( itAgent->second.find(tKey) == itAgent->second.end() )
			{
				itAgent->second.insert(tKey);
			}
		}
		else
		{
			set<TSubscribeKey> tSubKey;
			tSubKey.insert(tKey);
			m_mapUser2Keys[pcUser] = tSubKey;

			pcUser->m_setModuleListSubTask.insert(this);
		}

		GetNtfByKeyFromRedis(tKey, strNtf);
		if ( !strNtf.empty() )
		{
			TSubscribeData tData;
			tData.setUserlist.insert(pcUser);
			HandleNtf4Output(tKey.strSubType, tData, strNtf);
		}
	}
	else
	{
		// 取消订阅失败就失败了
		EraseRedisSubscribeMap(tKey, pcUser);
		EraseUser2KeyMap(pcUser, tKey);
	}

	return CMS_SUCCESS;
}

void CModuleListSubscribeTaskBase::EraseKeyByDevUri(const string &strDevUri)
{
    TSubscribeKey tKey;
    tKey.strDevUri = strDevUri;
    for (vector<string>::iterator it = m_vecSupportedSubType.begin(); 
        it != m_vecSupportedSubType.end(); ++it)
    {
        tKey.strSubType = *it;

        map<TSubscribeKey, TSubscribeData>::iterator itData = m_mapSubscribe.find(tKey);
        if (itData != m_mapSubscribe.end())
        {
            TRefreshSubReq tReq(tKey, false);
            m_lstRefreshSubReq.push_back(tReq);

            for (set<CCuAgent*>::iterator itUser = itData->second.setUserlist.begin();
                itUser != itData->second.setUserlist.end(); ++itUser)
            {
                EraseUser2KeyMap(*itUser, tKey);
            }

            m_mapSubscribe.erase(itData);
        }
    }

    if (m_dwRefreshingReqNum == 0)
    {
        DoRefreshSub();
    }
}

void CModuleListSubscribeTaskBase::EraseKeyByDevDomain( const string &strDomainName )
{
    TSipURI tUri;
    for (map<TSubscribeKey, TSubscribeData>::iterator it = m_mapSubscribe.begin(); it != m_mapSubscribe.end();)
    {
        tUri = it->first.strDevUri;
        if (tUri.GetDomain() == strDomainName)
        {
            TRefreshSubReq tReq(it->first, false);
            m_lstRefreshSubReq.push_back(tReq);

            for (set<CCuAgent*>::iterator itUser = it->second.setUserlist.begin();
                itUser != it->second.setUserlist.end(); ++itUser)
            {
                EraseUser2KeyMap(*itUser, it->first);
            }

            m_mapSubscribe.erase(it++);
        }
        else
        {
            ++it;
        }
    }

    if (m_dwRefreshingReqNum == 0)
    {
        DoRefreshSub();
    }
}

void CModuleListSubscribeTaskBase::EraseUser(CCuAgent *pcUser)
{
    map<CCuAgent*, set<TSubscribeKey> >::iterator it = m_mapUser2Keys.find(pcUser);
    if (it != m_mapUser2Keys.end())
    {
        for (set<TSubscribeKey>::iterator itKey = it->second.begin(); 
            itKey != it->second.end(); ++itKey)
        {
			if ( IsRedisSubscribe((*itKey).strSubType) )
			{
				EraseRedisSubscribeMap(*itKey, pcUser);
			}
			else
			{
				EraseSubscribeMap(*itKey, pcUser);
			}
        }

        m_mapUser2Keys.erase(it);
    }
}

u32 CModuleListSubscribeTaskBase::SubscribeProc( CMessage *const pcMsg )
{
    switch(pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            TASKLOG(NORMAL_TASK, ERROR_LEV, "Subscribe %s failed, sipError:%d\n", 
                m_strModuleUri.c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "Subscribe %s failed, dialog terminate\n", m_strModuleUri.c_str());
        }
        break;
    default:
        {
            if (pcMsg->event == m_wSubscribeRspEvent)
            {
                s32 nError = GetSubscribeRspResult(pcMsg);
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

                if (CMS_SUCCESS == nError)
                {
                    m_tSubscribeID = pcOspSipMsg->GetSipDlgID();
                    NextState(enRefreshSub);
                    DoRefreshSub();
                }
                else
                {
                    OspSipReleaseDlg(pcOspSipMsg->GetSipDlgID());
                    TASKLOG(NORMAL_TASK, ERROR_LEV, "Subscribe %s failed, rsp error:%d\n", m_strModuleUri.c_str(), nError);
                }
            }
            else
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "Recv unknown mesg, wEvent=%s[%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CModuleListSubscribeTaskBase::SubscribeTimerPoll()
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
    {
        NextState(enSubscribe);
        DoSubscribe();
    }
    
    return TIMERPOLL_DONE;
}

u32 CModuleListSubscribeTaskBase::RefreshSubProc( CMessage *const pcMsg )
{
    switch(pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            TASKLOG(NORMAL_TASK, ERROR_LEV, "Refresh Subscribe %s failed, sipError:%d\n", 
                m_strModuleUri.c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "Recv Dialog Terminate, mododuleUri=%s\n", 
                m_strModuleUri.c_str());

            // 会话销毁，需要重新订阅
            m_tSubscribeID = INVALID_DIALOG_ID;

            string strSession = m_strModuleSession;
            ModuleDisconnect();
            ModuleReconnect(strSession);
        }
        break;
    default:
        {
            if (pcMsg->event == m_wRefreshRspEvent)
            {
                vector<string> vecFailDevUri;
                s32 nError = GetRefreshRspResult(pcMsg, vecFailDevUri);
                if (CMS_SUCCESS == nError)
                {
                    if (!vecFailDevUri.empty())
                    {
                        // 剔除掉可以忽略的错误，比如设备删除或域删除时请求队列中的过期订阅可能永远失败，
                        // 它们的失败是可以忽略的，由于设备删除时Key不会存在，我们可以利用它判断。
                        if (m_lstRefreshSubReq.front().bAdd)
                        {
                            TSubscribeKey tKey;
                            for (vector<string>::iterator it = vecFailDevUri.begin(); it != vecFailDevUri.end();)
                            {
                                tKey.strDevUri = *it;

                                bool bNoKey = true;
                                for (vector<string>::iterator itSubType = m_vecSupportedSubType.begin();
                                    itSubType != m_vecSupportedSubType.end(); ++itSubType)
                                {
                                    tKey.strSubType = *itSubType;

                                    if (m_mapSubscribe.find(tKey) != m_mapSubscribe.end())
                                    {
                                        bNoKey = false;
                                        break;
                                    }
                                }

                                if (bNoKey)
                                {
                                    it = vecFailDevUri.erase(it);
                                }
                                else
                                {
                                    ++it;
                                }
                            }
                        }
                    }
                    if (!vecFailDevUri.empty())
                    {
                        // 存在失败的条目，后续尝试
                        break;
                    }

                    for (u32 i = 0; i < m_dwRefreshingReqNum; ++i)
                    {
                        m_lstRefreshSubReq.pop_front();
                    }
                    m_dwRefreshingReqNum = 0;

                    if (!m_lstRefreshSubReq.empty())
                    {
                        DoRefreshSub();
                    }
                }
                else
                {
                    TASKLOG(NORMAL_TASK, WARNING_LEV, "refresh %s failed, rsp error:%d\n", m_strModuleUri.c_str(), nError);
                }
            }
            else
            {
                for (vector<u16>::const_iterator it = m_vecSupportedNtfReqEvent.begin();
                    it != m_vecSupportedNtfReqEvent.end(); ++it)
                {
					// 订阅来的通知
                    if (*it == pcMsg->event)
                    {
                        TSubscribeKey tKey;
						string strNtf;
                        HandleNtf4Input(pcMsg, tKey, strNtf);
                        map<TSubscribeKey, TSubscribeData>::iterator itOut = m_mapSubscribe.find(tKey);
                        if (itOut != m_mapSubscribe.end())
                        {
                            HandleNtf4Output(tKey.strSubType, itOut->second, strNtf);
                            HandleNtf4InputAfterOutput(tKey.strSubType, itOut->second, strNtf);
                        }
                        
                        return PROCMSG_OK;
                    }
                }
                
                TASKLOG(NORMAL_TASK, ERROR_LEV, "Recv unknown mesg, wEvent=%s[%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CModuleListSubscribeTaskBase::RefreshSubTimerPoll()
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
    {
        NextState(enRefreshSub);

        if (m_dwRefreshingReqNum > 0 || !m_lstRefreshSubReq.empty())
        {
            DoRefreshSub();
        }
    }

    return TIMERPOLL_DONE;
}

void CModuleListSubscribeTaskBase::DoSubscribe()
{
    string strReq;
    GetSubscribeReqXml(m_strModuleSession, g_cCuiConfig.GetLocalURI().GetURIString(), strReq);

    if(CMS_SUCCESS != PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, strReq, m_strModuleUri))
    {
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "send subscribe fail, module uri:%s\n", m_strModuleUri.c_str());
    }
}

void CModuleListSubscribeTaskBase::DoRefreshSub()
{
    if (GetState() != enRefreshSub)
    {
        return;
    }

    if (m_dwRefreshingReqNum == 0 && !m_lstRefreshSubReq.empty())
    {
        m_dwRefreshingReqNum = 1;

        list<TRefreshSubReq>::const_iterator itChk = m_lstRefreshSubReq.begin();
        ++itChk;
        for (;itChk != m_lstRefreshSubReq.end(); ++itChk)
        {
            if (itChk->bAdd == m_lstRefreshSubReq.begin()->bAdd)
            {
                ++m_dwRefreshingReqNum;
                if (m_dwRefreshingReqNum > 20)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    if (m_dwRefreshingReqNum == 0)
    {
        return;
    }


    TSsDevList tDevLst;
    TSsOpType tOprType;
    
    list<TRefreshSubReq>::const_iterator it = m_lstRefreshSubReq.begin();
    for (u32 i = 0; i < m_dwRefreshingReqNum; ++i, ++it)
    {
        TSsDevList::iterator itDevLst = tDevLst.find(it->tKey.strDevUri);
        if (itDevLst == tDevLst.end())
        {
            set<TSsType> tData;
            tData.insert(it->tKey.strSubType);
            tDevLst[it->tKey.strDevUri] = tData; 
        }
        else
        {
            itDevLst->second.insert(it->tKey.strSubType);
        }
    }

    tOprType = m_lstRefreshSubReq.front().bAdd? SS_OP_TYPE_ADD:SS_OP_TYPE_DEL;

    string strReq;
    GetRefreshSubReqXml(m_strModuleSession, tDevLst, tOprType, strReq);

    if(CMS_SUCCESS != PostInDlgReq(KDSIP_EVENT_SUBSCRIBE_REQ, strReq, m_tSubscribeID))
    {
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "send subscribe fail, module uri:%s\n", m_strModuleUri.c_str());
    }
}

bool CModuleListSubscribeTaskBase::GetSubscribeNtf( const TSubscribeKey &tKey, string &strNtf )
{
    map<TSubscribeKey, TSubscribeData>::const_iterator it = m_mapSubscribe.find(tKey);
    if (it != m_mapSubscribe.end())
    {
        strNtf = it->second.strNtf;
        return true;
    }
    else
    {
        return false;
    }
}

bool CModuleListSubscribeTaskBase::SetSubscribeNtf( const TSubscribeKey &tKey, const string &strNtf )
{
    map<TSubscribeKey, TSubscribeData>::iterator it = m_mapSubscribe.find(tKey);
    if (it != m_mapSubscribe.end())
    {
        it->second.strNtf = strNtf;
        return true;
    }
    else
    {
        return false;
    }
}

void CModuleListSubscribeTaskBase::GetNtfByKeyFromRedis(const TSubscribeKey &tKey, string &strNtf)
{
	strNtf.clear();
	EFDB_ErrorCode er = EC_OK;

	string devId = GetNameWithoutDomainFromURI(tKey.strDevUri);

	TPuStatus tPuStatus;
	CPuStatusNtyReq cPuStatusNtf;

	er = g_cRedisDb.GetFullPuStatus(devId, tPuStatus);
	unsigned char enLogLev = EVENT_LEV;
	if ( !IsFdbResultPositive(er) )
	{
		enLogLev = ERROR_LEV;
	}
	TASKLOG(NORMAL_TASK, enLogLev, "GetPuStatus TPuStatus[%s] er[%d] dev=%s,subtype=%s\n",
		tPuStatus.ToXml().c_str(), er, tKey.strDevUri.c_str(), tKey.strSubType.c_str());

	if (tKey.strSubType == PU_ON_OFF_STATUS)
	{
		cPuStatusNtf.SetDevURI(tKey.strDevUri);
		cPuStatusNtf.SetOnOffStatus(tPuStatus);
		strNtf = cPuStatusNtf.ToXml();
		return;
	}

	strNtf.clear();
	if (tKey.strSubType == PU_CONFIG_STATUS)
	{
		TPuConfig tPuConfig;
		string errMsg;
		er = g_cRedisDb.GetFullPuConfigForCu(devId, tPuConfig, errMsg);
		if ( IsFdbResultPositive(er) )
		{
			tPuConfig.SetDevUri(tKey.strDevUri);
			if (tPuConfig.IsCompleted())
			{
				CPuConfigNtyReq cNtf;
				cNtf.SetDevURI(tKey.strDevUri);

				TPuDevConfig tPuDevConfig = tPuConfig.GetDevConfig();
				cNtf.SetConfig(tPuDevConfig);
				
				strNtf = cNtf.ToXml();
			}
			else
			{
				TASKLOG(NORMAL_TASK, CRITICAL_LEV, "GetFullPuConfigForCu NotCompleted devUri[%s], tPuConfig[%s]\n",
					tKey.strDevUri.c_str(), tPuConfig.ToXml().c_str());
				return;
			}
		}
		else
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "GetFullPuConfigForCu failed devUri[%s] errMsg[%s] er[%d]\n",
				tKey.strDevUri.c_str(), errMsg.c_str(), er);
		}

		return;
	}

	strNtf.clear();
	if (tPuStatus.GetIsOnline() == false
		|| tPuStatus.GetIsUsed() == false
		|| tPuStatus.GetIsInTop() == false)
	{
		TASKLOG(NORMAL_TASK, CRITICAL_LEV, "dev[%s] is offline TPuStatus[%s]!\n", tKey.strDevUri.c_str(), tPuStatus.ToXml().c_str());
		return;
	}

	if (tKey.strSubType == PU_SERVICE_STATUS)
	{
		TPuServiceStatus serverStatus;

		er = g_cRedisDb.GetFullPuServiceStatus(devId, serverStatus);
		if ( IsFdbResultPositive(er) )
		{
			if (serverStatus.GetAudioCallSrc().empty()
				&& serverStatus.GetIsPlatPreRec().empty()
				&& serverStatus.GetIsPuPreRec().empty()
				&& serverStatus.GetIsPuRec().empty()
				&& serverStatus.GetPlatRec().empty() )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "GetRedisData dev=%s,type=%s,Value Is Empty!\n", tKey.strDevUri.c_str(), tKey.strSubType.c_str());
				return;
			}

			CPuServiceStatusNtyReq cNtf;

			cNtf.SetDevURI(tKey.strDevUri);
			cNtf.SetServiceStatus(serverStatus);

			strNtf = cNtf.ToXml();
		}
	}
	else if (tKey.strSubType == PU_ALARM_STATUS)
	{
		CRedisPuData redisPuData;
		std::map<std::string, CAlarmInputList> alarmInfoList;
		er = g_cRedisDb.GetAlarmChnInfo(devId, alarmInfoList);
		if ( IsFdbResultPositive(er) )
		{
			TPuAlarmStatus  alarmStatus;
			// 保险起见过滤掉恢复
			for (std::map<std::string, CAlarmInputList>::const_iterator cAlarmIter = alarmInfoList.begin();
				cAlarmIter != alarmInfoList.end(); cAlarmIter++)
			{
				const CAlarmInputList &alarmChnList = cAlarmIter->second;
				for (CAlarmInputList::const_iterator cAlarmChnIter = alarmChnList.begin();
					cAlarmChnIter != alarmChnList.end(); cAlarmChnIter++)
				{
					// 告警恢复不需要直接给刚刚订阅的CU
					if (cAlarmChnIter->second.GetAlarmFlag() == ALARM_FLAG_OCCUR)
					{
						alarmStatus.AddAlarmItem(cAlarmChnIter->second);
					}
				}
			}

			if ( alarmStatus.GetAlarmArray().empty() )
			{
				// 没有告警产生就直接返回了
				TASKLOG(NORMAL_TASK, EVENT_LEV, "GetRedisData dev=%s,type=%s,Value Is Empty!\n", tKey.strDevUri.c_str(), tKey.strSubType.c_str());
				return;
			}

			CPuAlarmStatusNtyReq cNtf;

			cNtf.SetDevURI(tKey.strDevUri);
			cNtf.SetAlarmStatus(alarmStatus);

			strNtf = cNtf.ToXml();
		}
	}
	else if (tKey.strSubType == PU_TRANSDATA_STATUS)
	{
		CRedisPuData puData;
		string field = CRedisDevData_m_tTransparentData(PU_TRANS_DATA_TYPE_COMDATA);

		er = g_cRedisDb.GetPuData(devId, puData, field);
		if ( IsFdbResultPositive(er) )
		{
			if (puData.GetPuTransparentData().GetType().empty())
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "GetRedisData dev=%s,type=%s,Value Is Empty!\n", tKey.strDevUri.c_str(), tKey.strSubType.c_str());
				return;
			}

			CPuTransDataStatusNtyReq cNtf;

			cNtf.SetDevURI(tKey.strDevUri);
			cNtf.SetData(puData.GetPuTransparentData());

			strNtf = cNtf.ToXml();
		}
	}
	else
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "未知的类型订阅类型 type[%s]!\n", tKey.strSubType.c_str());
	}

	if ( !IsFdbResultPositive(er) )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "GetRedisData dev=%s,type=%s,er=%d\n", tKey.strDevUri.c_str(), tKey.strSubType.c_str(),er);
	}
	return;
}

void CModuleListSubscribeTaskBase::EraseUser2KeyMap(CCuAgent* pcUser, const TSubscribeKey &tKey)
{
    map<CCuAgent*, set<TSubscribeKey> >::iterator it = m_mapUser2Keys.find(pcUser);
    if (it != m_mapUser2Keys.end())
    {
        it->second.erase(tKey);

        if (it->second.empty())
        {
            m_mapUser2Keys.erase(pcUser);
            pcUser->m_setModuleListSubTask.erase(this);
        }
    }
}

void CModuleListSubscribeTaskBase::EraseSubscribeMap( const TSubscribeKey &tKey, CCuAgent* pcUser )
{
    map<TSubscribeKey, TSubscribeData>::iterator it = m_mapSubscribe.find(tKey);
    if (it != m_mapSubscribe.end())
    {
        it->second.setUserlist.erase(pcUser);

        if (it->second.setUserlist.empty())
        {
            TRefreshSubReq tReq(tKey, false);
            m_lstRefreshSubReq.push_back(tReq);

            if (m_dwRefreshingReqNum == 0)
            {
                DoRefreshSub();
            }

            m_mapSubscribe.erase(it);
        }
    }
}

void CModuleListSubscribeTaskBase::EraseRedisSubscribeMap(const TSubscribeKey &tKey, CCuAgent* pcUser)
{
	map<TSubscribeKey, TSubscribeData>::iterator it = m_mapSubscribe.find(tKey);
	if (it != m_mapSubscribe.end())
	{
		set<CCuAgent*>::iterator iter = it->second.setUserlist.find(pcUser);
		if (iter != it->second.setUserlist.end())
		{
			it->second.setUserlist.erase(iter);
		}

		if (it->second.setUserlist.empty())
		{
			m_mapSubscribe.erase(it);

			DoRedisSubscribe(tKey, false);
		}
	}
}

void CModuleListSubscribeTaskBase::PrintData() const
{
    OspPrintf(TRUE, FALSE, "\n");
    CTask::PrintData();

    OspPrintf(TRUE, FALSE, "ModuleUri:%s, ModuleSession:%s, SubscribeRspEvent:%s, RefreshRspEvent:%s,"
        "RefreshingReqNum:%d, RefreshingReqSize:%d, SubscribeKeySize:%d, User2KeySize:%d\n", 
        m_strModuleUri.c_str(), m_strModuleSession.c_str(), OspExtEventDesc(m_wSubscribeRspEvent).c_str(), 
        OspExtEventDesc(m_wRefreshRspEvent).c_str(),
        (s32)m_dwRefreshingReqNum, (s32)m_lstRefreshSubReq.size(),
        (s32)m_mapSubscribe.size(), (s32)m_mapUser2Keys.size());

    std::ostringstream oss;
    for (vector<u16>::const_iterator it = m_vecSupportedNtfReqEvent.begin(); 
        it != m_vecSupportedNtfReqEvent.end(); ++it)
    {
        oss << OspExtEventDesc(*it);
        oss << " ";
    }
    OspPrintf(TRUE, FALSE, "\nSupportedNtfReqEvent List:%s\n", oss.str().c_str());

    oss.str("");
    for (vector<string>::const_iterator it = m_vecSupportedSubType.begin();
        it != m_vecSupportedSubType.end(); ++it)
    {
        oss << *it;
        oss << " ";
    }
    OspPrintf(TRUE, FALSE, "\nSupportedSubType List:%s\n", oss.str().c_str());

    OspPrintf(TRUE, FALSE, "\nSubscribe Key List:\n");
    oss.str("");
    for (map<TSubscribeKey, TSubscribeData>::const_iterator it = m_mapSubscribe.begin();
        it != m_mapSubscribe.end(); ++it)
    {
        oss << it->first.strDevUri;
        oss << ":";
        oss << it->first.strSubType;
        oss << " ";

        if (oss.str().size() > 800)
        {
            OspPrintf(TRUE, FALSE, "%s", oss.str().c_str());
            oss.str("");
        }
    }
    if (!oss.str().empty())
    {
        OspPrintf(TRUE, FALSE, "%s", oss.str().c_str());
    }

    OspPrintf(TRUE, FALSE, "\nSubscribe User List:\n");

    oss.str("");
    for (map<CCuAgent*, set<TSubscribeKey> >::const_iterator it = m_mapUser2Keys.begin();
        it != m_mapUser2Keys.end(); ++it)
    {
        oss << it->first->m_strSession;
        oss << ":";
        oss << it->second.size();
        oss << " ";

        if (oss.str().size() > 800)
        {
            OspPrintf(TRUE, FALSE, "%s", oss.str().c_str());
            oss.str("");
        }
    }
    if (!oss.str().empty())
    {
        OspPrintf(TRUE, FALSE, "%s", oss.str().c_str());
    }

    OspPrintf(TRUE, FALSE, "\n");
}

const TSubscribeData* CModuleListSubscribeTaskBase::GetSubDataBySubKey(const TSubscribeKey& tSubscribeKey) const
{
	map<TSubscribeKey, TSubscribeData>::const_iterator it = m_mapSubscribe.find(tSubscribeKey);
	if (m_mapSubscribe.end() != it)
	{
		return &it->second;
	}
	else
	{
		return NULL;
	}
}

void CModuleListSubscribeTaskBase::DumpByDevUri( const string &strDevUri ) const
{
    TSubscribeKey tKey;
    tKey.strDevUri = strDevUri;
    for (vector<string>::const_iterator it = m_vecSupportedSubType.begin();
        it != m_vecSupportedSubType.end(); ++it)
    {
        tKey.strSubType = *it;

        map<TSubscribeKey, TSubscribeData>::const_iterator itVal = m_mapSubscribe.find(tKey);
        if (itVal == m_mapSubscribe.end())
        {
            continue;
        }
        
        OspPrintf(TRUE, FALSE, "ModuleUri:%s, DevUri:%s, SubscribeType:%s——Userlist:",
            m_strModuleUri.c_str(), tKey.strDevUri.c_str(), tKey.strSubType.c_str());
        for (set<CCuAgent*>::const_iterator itUser = itVal->second.setUserlist.begin();
            itUser != itVal->second.setUserlist.end(); ++itUser)
        {
            OspPrintf(TRUE, FALSE, "%s ", (*itUser)->m_strSession.c_str());
        }
        OspPrintf(TRUE, FALSE, "\nNtfContent:%s\n", itVal->second.strNtf.c_str());
    }
}

void CModuleListSubscribeTaskBase::DumpByUser( const CCuAgent *pcUser ) const
{
    map<CCuAgent*, set<TSubscribeKey> >::const_iterator it = m_mapUser2Keys.find(const_cast<CCuAgent*>(pcUser));
    if (it == m_mapUser2Keys.end())
    {
        return;
    }

    for (set<TSubscribeKey>::const_iterator itKey = it->second.begin();
        itKey != it->second.end(); ++itKey)
    {
        OspPrintf(TRUE, FALSE, "ModuleUri:%s, DevUri:%s, SubscribeType:%s\n", 
            m_strModuleUri.c_str(), itKey->strDevUri.c_str(), itKey->strSubType.c_str());
    }
}

void CModuleListSubscribeTaskBase::ModuleDisconnect()
{
    CMS_ASSERT(GetState() != enDisconnect);

    if (m_tSubscribeID != INVALID_DIALOG_ID)
    {
        OspSipReleaseDlg(m_tSubscribeID);
        m_tSubscribeID = INVALID_DIALOG_ID;
    }
    m_strModuleSession.clear();

    NextState(enDisconnect);
}

void CModuleListSubscribeTaskBase::ModuleReconnect( const string &strSession )
{
    CMS_ASSERT(GetState() == enDisconnect);

    m_strModuleSession = strSession;

    m_lstRefreshSubReq.clear();
    m_dwRefreshingReqNum = 0;

    for (map<TSubscribeKey, TSubscribeData>::iterator it = m_mapSubscribe.begin();
        it != m_mapSubscribe.end();)
    {
        if (!it->second.setUserlist.empty())
        {
            TRefreshSubReq tReq(it->first, true);
            m_lstRefreshSubReq.push_back(tReq);

            ++it;
        }
        else
        {
            m_mapSubscribe.erase(it++);
        }
    }

    NextState(enSubscribe);
    DoSubscribe();
}

u32 CModuleListSubscribeTaskBase::DisconnectProc(CMessage *const pcMsg)
{
    return PROCMSG_OK;
}

u32 CModuleListSubscribeTaskBase::DisconnectTimerPoll()
{
    return TIMERPOLL_DONE;
}


CCmuListSubscribeTask::CCmuListSubscribeTask( CInstExt *pcInst, const string &strModuleUri, const string &strSession )
    : CModuleListSubscribeTaskBase(pcInst, strModuleUri, strSession)
{
    vector<u16> vecSupportedNtfReqEvent;
    vecSupportedNtfReqEvent.push_back(PU_STATUS_NTF_REQ);
	vecSupportedNtfReqEvent.push_back(PU_CONFIG_NTF_REQ);
    vecSupportedNtfReqEvent.push_back(PU_SERVICE_STATUS_NTF_REQ);
    vecSupportedNtfReqEvent.push_back(PU_ALARM_STATUS_NTF_REQ);
    vecSupportedNtfReqEvent.push_back(PU_TRANSDATA_STATUS_NTF_REQ);
    vector<string> vecSupportedSubType;
    vecSupportedSubType.push_back(PU_ON_OFF_STATUS);
    vecSupportedSubType.push_back(PU_CONFIG_STATUS);
    vecSupportedSubType.push_back(PU_SERVICE_STATUS);
    vecSupportedSubType.push_back(PU_ALARM_STATUS);
    vecSupportedSubType.push_back(PU_TRANSDATA_STATUS);

    ConstructorData(PULIST_STATUS_SS_RSP, PULIST_STATUS_REFRESH_SS_RSP, 
        vecSupportedNtfReqEvent, vecSupportedSubType);
}


void CCmuListSubscribeTask::GetSubscribeReqXml( const string &strSession, const string &strUser, string &strResult ) const
{
    CPuListStatusSsReq cReq;
    cReq.SetSession(strSession);
    cReq.SetUserUri(strUser);
    
    strResult = cReq.ToXml();
}

void CCmuListSubscribeTask::GetRefreshSubReqXml( const string &strSession, const TSsDevList &tDevLst, const TSsOpType &tOprType, string &strResult ) const
{
    CPuListStatusRefreshSsReq cReq;
    cReq.SetSession(strSession);
    cReq.SetSsDevList(tDevLst);
    cReq.SetSsOpType(tOprType);

    strResult = cReq.ToXml();
}

s32 CCmuListSubscribeTask::GetSubscribeRspResult( CMessage *const pcMsg ) const
{
    CPuListStatusSsRsp cRsp;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    pcOspSipMsg->GetMsgBody(cRsp);

    return cRsp.GetErrorCode();
}

s32 CCmuListSubscribeTask::GetRefreshRspResult(CMessage *const pcMsg, vector<string> vecFailDevUri) const
{
    vecFailDevUri.clear();

    CPuListStatusRefreshSsRsp cRsp;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    pcOspSipMsg->GetMsgBody(cRsp);

    if (cRsp.GetErrorCode() != CMS_SUCCESS)
    {
        return cRsp.GetErrorCode();
    }
    for (TSsErrorList::const_iterator it = cRsp.GetSsErrorList().begin();
        it != cRsp.GetSsErrorList().end(); ++it)
    {
        vecFailDevUri.push_back(it->first);
    }

    return CMS_SUCCESS;
}

void CCmuListSubscribeTask::HandleNtf4Input( CMessage *const pcMsg, TSubscribeKey &tKey, string &strNtf )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	string strEmptyNtf;
    switch (pcMsg->event)
    {
    case PU_STATUS_NTF_REQ:
        {
            CPuStatusNtyReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);
            tKey.strDevUri = cNtf.GetDevURI();
            tKey.strSubType = PU_ON_OFF_STATUS;
			SetSubscribeNtf(tKey, strEmptyNtf);
			strNtf = cNtf.ToXml();
            if (!cNtf.GetOnOffStatus().GetIsOnline())
            {
                TSubscribeKey tConfNtfKey(tKey.strDevUri, PU_CONFIG_STATUS);
                SetSubscribeNtf(tConfNtfKey, strEmptyNtf);

                TSubscribeKey tServiceNtfKey(tKey.strDevUri, PU_SERVICE_STATUS);
                SetSubscribeNtf(tServiceNtfKey, strEmptyNtf);

                TSubscribeKey tAlarmNtfKey(tKey.strDevUri, PU_ALARM_STATUS);
                SetSubscribeNtf(tAlarmNtfKey, strEmptyNtf);

                TSubscribeKey tTransNtfKey(tKey.strDevUri, PU_TRANSDATA_STATUS);
                SetSubscribeNtf(tTransNtfKey, strEmptyNtf);
            }
        }
        break;
    case PU_CONFIG_NTF_REQ:
        {
            CPuConfigNtyReq cNtf;
            cNtf.FromXml(GetNtyString());
			tKey.strDevUri = cNtf.GetDevURI();
			tKey.strSubType = PU_CONFIG_STATUS;
			SetSubscribeNtf(tKey, "");
			strNtf = GetNtyString();
        }
        break;
    case PU_SERVICE_STATUS_NTF_REQ:
        {
            CPuServiceStatusNtyReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);
            tKey.strDevUri = cNtf.GetDevURI();
            tKey.strSubType = PU_SERVICE_STATUS;
			SetSubscribeNtf(tKey, "");
			strNtf = cNtf.ToXml();
        }
        break;
    case PU_ALARM_STATUS_NTF_REQ:
        {
            CPuAlarmStatusNtyReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);
            tKey.strDevUri = cNtf.GetDevURI();
            tKey.strSubType = PU_ALARM_STATUS;
			SetSubscribeNtf(tKey, "");
			strNtf = cNtf.ToXml();
        }
        break;
    case PU_TRANSDATA_STATUS_NTF_REQ:
        {
            CPuTransDataStatusNtyReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);
            tKey.strDevUri = cNtf.GetDevURI();
            tKey.strSubType = PU_TRANSDATA_STATUS;
			SetSubscribeNtf(tKey, "");
			strNtf = cNtf.ToXml();
        }
        break;
    default:
        CMS_ASSERT(false);
        break;
    }
}

void CCmuListSubscribeTask::HandleNtf4Output( const string &strSubType, const TSubscribeData &tData, const string &strNtf )
{
    if (strNtf.empty())
    {
        return;
    }

    if (strSubType == PU_ALARM_STATUS)
    {
        CPuAlarmStatusNtyReq cNtf;
        cNtf.FromXml(strNtf);
        if (cNtf.GetAlarmStatus().GetAlarmArray().empty())
        {
            return;
        }

        for (set<CCuAgent*>::iterator it = tData.setUserlist.begin(); 
            it != tData.setUserlist.end(); ++it)
        {
            CCuAgent* pcAgent = *it;
            pcAgent->m_cNotifyPool.InsertSubNotify(cNtf);
        }
    }
    else if (strSubType == PU_ON_OFF_STATUS)
    {
        CPuStatusNtyReq cNtf;
        cNtf.FromXml(strNtf);

		CPuConfigNtyReq cPuConfigNtf;

		if ( !cNtf.GetOnOffStatus().GetIsOnline() )
		{
			// 设备下线 cmu不会发布设备通道状态信息，需要cui发布一下
			string uuid = GetNameWithoutDomainFromURI(cNtf.GetDevURI());
			TPuConfig tPuConfig;
			string strErrMsg;
			EFDB_ErrorCode er = g_cRedisDb.GetFullPuConfigForCu(uuid, tPuConfig, strErrMsg);
			if ( IsFdbResultPositive(er) )
			{
				cPuConfigNtf.SetDevURI(cNtf.GetDevURI());
				cPuConfigNtf.SetConfig(tPuConfig.GetDevConfig());
			}
			if ( !strErrMsg.empty() )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "GetFullPuConfigForCu strErrMsg[%s]\n", strErrMsg.c_str());
			}
		}

        for (set<CCuAgent*>::iterator it = tData.setUserlist.begin(); 
            it != tData.setUserlist.end(); ++it)
        {
            CCuAgent* pcAgent = *it;
            pcAgent->m_cNotifyPool.InsertSubNotify(cNtf);

			if ( !cPuConfigNtf.GetDevURI().empty() )
			{
				pcAgent->m_cNotifyPool.InsertSubNotify(cPuConfigNtf);
			}
        }
    }
    else if (strSubType == PU_SERVICE_STATUS)
    {
        CPuServiceStatusNtyReq cNtf;
        cNtf.FromXml(strNtf);

        for (set<CCuAgent*>::iterator it = tData.setUserlist.begin(); 
            it != tData.setUserlist.end(); ++it)
        {
            CCuAgent* pcAgent = *it;
            pcAgent->m_cNotifyPool.InsertSubNotify(cNtf);
        }
    }
    else if (strSubType == PU_CONFIG_STATUS)
    {
        CPuConfigNtyReq cNtf;
        cNtf.FromXml(strNtf);
        if (!cNtf.GetConfig().IsCompleted())
        {
			TASKLOG(NORMAL_TASK, ERROR_LEV, "PU_CONFIG_STATUS IsCompleted() == false ntf[%s]\n", cNtf.ToXml().c_str());
            return;
        }

        for (set<CCuAgent*>::iterator it = tData.setUserlist.begin(); 
            it != tData.setUserlist.end(); ++it)
        {
            CCuAgent* pcAgent = *it;
            pcAgent->m_cNotifyPool.InsertSubNotify(cNtf);
        }
    }
    else if (strSubType == PU_TRANSDATA_STATUS)
    {
        CPuTransDataStatusNtyReq cNtf;
        cNtf.FromXml(strNtf);

        for (set<CCuAgent*>::iterator it = tData.setUserlist.begin(); 
            it != tData.setUserlist.end(); ++it)
        {
            CCuAgent* pcAgent = *it;
            pcAgent->m_cNotifyPool.InsertSubNotify(cNtf);
        }
    }
    else
    {
        CMS_ASSERT(false && "Not implement");
    }
}

void CCmuListSubscribeTask::HandleNtf4InputAfterOutput( const string &strSubType, TSubscribeData &tData, string &strNtf )
{
    if (strSubType == PU_ALARM_STATUS)
    {
        CPuAlarmStatusNtyReq tNtf;
        tNtf.FromXml(strNtf);
        for (CAlarmInputArray::iterator it = tNtf.GetAlarmStatus().GetAlarmArray().begin();
            it != tNtf.GetAlarmStatus().GetAlarmArray().end();)
        {
            if( ALARM_FLAG_RESTORE == it->GetAlarmFlag())
            {
                it = tNtf.GetAlarmStatus().GetAlarmArray().erase(it);
            }
            else
            {
                ++it;
            }
        }        
    }
    else if (strSubType == PU_TRANSDATA_STATUS)
    {
        tData.strNtf.clear();
    }
}

void CCmuListSubscribeTask::SetStateRefreshSub()
{
	NextState(enRefreshSub);
}

CRcsListSubscribeTask::CRcsListSubscribeTask(CInstExt *pcInst, const string &strModuleUri, const string &strSession)
    : CModuleListSubscribeTaskBase(pcInst, strModuleUri, strSession)
{
    vector<u16> vecSupportedNtfReqEvent;
    vecSupportedNtfReqEvent.push_back(RCS_PU_RECORD_STATUS_NTF_REQ);
    vector<string> vecSupportedSubType;
    vecSupportedSubType.push_back(SS_TYPE_PU_PLATFORM_RECORD_STATUS);

    ConstructorData(RCS_LIST_SUBSCRIBE_PU_STATUS_RSP, RCS_LIST_REFRESH_SS_PU_STATUS_RSP, 
        vecSupportedNtfReqEvent, vecSupportedSubType);
}

void CRcsListSubscribeTask::GetSubscribeReqXml( const string &strSession, const string &strUser, string &strResult ) const
{
    CRcsListSubscribePuStatusReq cReq;
    cReq.SetSession(strSession);
    cReq.SetUserUri(strUser);

    strResult = cReq.ToXml();
}

void CRcsListSubscribeTask::GetRefreshSubReqXml( const string &strSession, const TSsDevList &tDevLst, const TSsOpType &tOprType, string &strResult ) const
{
    CRcsListRefreshSsPuStatusReq cReq;
    cReq.SetSession(strSession);
    cReq.SetSsDevList(tDevLst);
    cReq.SetSsOpType(tOprType);

    strResult = cReq.ToXml();
}

s32 CRcsListSubscribeTask::GetSubscribeRspResult( CMessage *const pcMsg ) const
{
    CRcsListSubscribePuStatusRsp cRsp;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    pcOspSipMsg->GetMsgBody(cRsp);

    return cRsp.GetErrorCode();
}

s32 CRcsListSubscribeTask::GetRefreshRspResult( CMessage *const pcMsg, vector<string> vecFailDevUri ) const
{
    vecFailDevUri.clear();

    CRcsListRefreshSsPuStatusRsp cRsp;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    pcOspSipMsg->GetMsgBody(cRsp);

    if (cRsp.GetErrorCode() != CMS_SUCCESS)
    {
        return cRsp.GetErrorCode();
    }
    for (TSsErrorList::const_iterator it = cRsp.GetSsErrorList().begin();
        it != cRsp.GetSsErrorList().end(); ++it)
    {
        vecFailDevUri.push_back(it->first);
    }

    return CMS_SUCCESS;
}

void CRcsListSubscribeTask::HandleNtf4Input( CMessage *const pcMsg, TSubscribeKey &tKey, string &strNtf )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case RCS_PU_RECORD_STATUS_NTF_REQ:
        {
            CRcsPuRecordStatusNtyReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);

            CRcsPuRecordStatusNtyRsp cRsp;
            cRsp.SetEvent(RCS_PU_RECORD_STATUS_NTF_RSP);
            cRsp.SetSession(cNtf.GetSession());
            cRsp.SetSeqNum(cNtf.GetSeqNum());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            tKey.strDevUri = cNtf.GetDevURI();
            tKey.strSubType = SS_TYPE_PU_PLATFORM_RECORD_STATUS;

            SetSubscribeNtf(tKey, cNtf.ToXml());
        }
        break;
    default:
        CMS_ASSERT(false && "Not Implement");
        break;
    }
}

void CRcsListSubscribeTask::HandleNtf4Output( const string &strSubType, const TSubscribeData &tData, const string &/*strNtf*/ )
{
    if (tData.strNtf.empty())
    {
        return;
    }

    if (strSubType == SS_TYPE_PU_PLATFORM_RECORD_STATUS)
    {
        CRcsPuRecordStatusNtyReq cNtf;
        cNtf.FromXml(tData.strNtf);

        for (set<CCuAgent*>::iterator it = tData.setUserlist.begin(); 
            it != tData.setUserlist.end(); ++it)
        {
            CCuAgent* pcAgent = *it;
            pcAgent->m_cNotifyPool.InsertSubNotify(cNtf);
        }
    }
    else
    {
        CMS_ASSERT(false && "Not Implement");
    }
}

CMpsListSubscribeTask::CMpsListSubscribeTask( CInstExt *pcInst, const string &strModuleUri, const string &strSession )
    : CModuleListSubscribeTaskBase(pcInst, strModuleUri, strSession)
{
    vector<u16> vecSupportedNtfReqEvent;
    vecSupportedNtfReqEvent.push_back(MPS_PU_GPS_NTF_REQ);
    vector<string> vecSupportedSubType;
    vecSupportedSubType.push_back(PU_GPS_STATUS);

    ConstructorData(MPS_PU_GPS_SS_RSP, MPS_PU_GPS_REFRESH_SS_RSP, 
        vecSupportedNtfReqEvent, vecSupportedSubType);
}

void CMpsListSubscribeTask::GetSubscribeReqXml( const string &strSession, const string &strUser, string &strResult ) const
{
    CPuGpsSsReq cReq;
    cReq.SetSession(strSession);
    cReq.SetUserUri(strUser);

    strResult = cReq.ToXml();
}

void CMpsListSubscribeTask::GetRefreshSubReqXml( const string &strSession, const TSsDevList &tDevLst, const TSsOpType &tOprType, string &strResult ) const
{
    CPuGpsRefreshSsReq cReq;
    cReq.SetSession(strSession);
    cReq.SetSsDevList(tDevLst);
    cReq.SetSsOpType(tOprType);

    strResult = cReq.ToXml();
}

s32 CMpsListSubscribeTask::GetSubscribeRspResult( CMessage *const pcMsg ) const
{
    CPuGpsSsRsp cRsp;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    pcOspSipMsg->GetMsgBody(cRsp);

    return cRsp.GetErrorCode();
}

s32 CMpsListSubscribeTask::GetRefreshRspResult( CMessage *const pcMsg, vector<string> vecFailDevUri ) const
{
    vecFailDevUri.clear();

    CPuGpsRefreshSsRsp cRsp;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    pcOspSipMsg->GetMsgBody(cRsp);

    if (cRsp.GetErrorCode() != CMS_SUCCESS)
    {
        return cRsp.GetErrorCode();
    }
    for (TSsErrorList::const_iterator it = cRsp.GetSsErrorList().begin();
        it != cRsp.GetSsErrorList().end(); ++it)
    {
        vecFailDevUri.push_back(it->first);
    }

    return CMS_SUCCESS;
}

void CMpsListSubscribeTask::HandleNtf4Input( CMessage *const pcMsg, TSubscribeKey &tKey, string &strNtf )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case MPS_PU_GPS_NTF_REQ:
        {
            CPuGpsNtyReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);
            CPuGpsNtyRsp cRsp;
            cRsp.SetEvent(MPS_PU_GPS_NTF_RSP);
            cRsp.SetSession(cNtf.GetSession());
            cRsp.SetSeqNum(cNtf.GetSeqNum());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            tKey.strDevUri = cNtf.GetDevURI();
            tKey.strSubType = PU_GPS_STATUS;

            SetSubscribeNtf(tKey, cNtf.ToXml());
        }
        break;
    default:
        CMS_ASSERT(false && "Not Implement");
        break;
    }
}

void CMpsListSubscribeTask::HandleNtf4Output( const string &strSubType, const TSubscribeData &tData, const string &/*strNtf*/ )
{
    if (tData.strNtf.empty())
    {
        return;
    }

    if (strSubType == PU_GPS_STATUS)
    {
        CPuGpsNtyReq cNtf;
        cNtf.FromXml(tData.strNtf);

        for (set<CCuAgent*>::iterator it = tData.setUserlist.begin(); 
            it != tData.setUserlist.end(); ++it)
        {
            CCuAgent* pcAgent = *it;
            pcAgent->m_cNotifyPool.InsertSubNotify(cNtf);
        }
    }
    else
    {
        CMS_ASSERT(false && "Not Implement");
    }
}

s32 ModuleListSubscribe( const string &strModuleUri, const string &strDevUri, const string &strSubType, CCuAgent *pcUser )
{
    map<string, CModuleListSubscribeTaskBase*>::iterator it = s_mapModuleListSubscribeTask.find(strModuleUri);

    if (it == s_mapModuleListSubscribeTask.end())
    {
        return ERR_CUI_SUBSCRIBE_SERVER_NOT_ONLINE;
    }

    TSubscribeKey tKey;
    tKey.strDevUri = strDevUri;
    tKey.strSubType = strSubType;
    return it->second->Refresh(tKey, pcUser, true);
}

s32 ModuleListUnsubscribe( const string &strModuleUri, const string &strDevUri, const string &strSubType, CCuAgent *pcUser )
{
    map<string, CModuleListSubscribeTaskBase*>::iterator it = s_mapModuleListSubscribeTask.find(strModuleUri);

    if (it == s_mapModuleListSubscribeTask.end())
    {
        return CMS_SUCCESS;
    }

    TSubscribeKey tKey;
    tKey.strDevUri = strDevUri;
    tKey.strSubType = strSubType;
    return it->second->Refresh(tKey, pcUser, false);
}

void EraseSubscribeKeyByDevUri( const string &strDevUri )
{
    for (map<string, CModuleListSubscribeTaskBase*>::iterator it = s_mapModuleListSubscribeTask.begin();
        it != s_mapModuleListSubscribeTask.end(); ++it)
    {
        it->second->EraseKeyByDevUri(strDevUri);
    }
}

void NotifyCuPuOffline(const string &strDevUri)
{
	TSubscribeKey tKey;
	tKey.strDevUri = strDevUri;
	tKey.strSubType = PU_ON_OFF_STATUS;

	CPuStatusNtyReq cNtf;
	cNtf.SetDevURI(strDevUri);
	cNtf.GetOnOffStatus().SetIsInTop(false);
	cNtf.GetOnOffStatus().SetIsOnline(false);
	cNtf.GetOnOffStatus().SetIsUsed(true);

	for (map<string, CModuleListSubscribeTaskBase*>::iterator it = s_mapModuleListSubscribeTask.begin();
		it != s_mapModuleListSubscribeTask.end(); ++it)
	{
		const TSubscribeData* pSubscribeData = it->second->GetSubDataBySubKey(tKey);
		if (NULL != pSubscribeData)
		{
			for (set<CCuAgent*>::const_iterator it = pSubscribeData->setUserlist.begin(); 
				it != pSubscribeData->setUserlist.end(); ++it)
			{
				CCuAgent* pcAgent = *it;
				pcAgent->m_cNotifyPool.InsertSubNotify(cNtf);
			}
		}
	}
}

void DeleteSubscribeKeyByDevDomain( const string &strDomainName )
{
    for (map<string, CModuleListSubscribeTaskBase*>::iterator it = s_mapModuleListSubscribeTask.begin();
        it != s_mapModuleListSubscribeTask.end(); ++it)
    {
        it->second->EraseKeyByDevDomain(strDomainName);
    }
}

void DumpModuleListSubscribeByDevUri( const string &strDevUri )
{
    OspPrintf(TRUE, FALSE, "\nDevice subscribe list-----------\n");
    for (map<string, CModuleListSubscribeTaskBase*>::const_iterator it = s_mapModuleListSubscribeTask.begin();
        it != s_mapModuleListSubscribeTask.end(); ++it)
    {
        it->second->DumpByDevUri(strDevUri);
    }
    OspPrintf(TRUE, FALSE, "\nDevice subscribe list-----------\n");
}

void DumpModuleListSubscribeByUser( const CCuAgent *pcUser )
{
    OspPrintf(TRUE, FALSE, "\nDevice subscribe list-----------\n");
    for (map<string, CModuleListSubscribeTaskBase*>::const_iterator it = s_mapModuleListSubscribeTask.begin();
        it != s_mapModuleListSubscribeTask.end(); ++it)
    {
        it->second->DumpByUser(pcUser);
    }
    OspPrintf(TRUE, FALSE, "\nDevice subscribe list-----------\n");
}
