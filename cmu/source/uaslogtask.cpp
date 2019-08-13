/*****************************************************************************
模块名      : cmu 
文件名      : uaslogtask.h
相关文件    : 
文件实现功能: CUasLogTask定义
作者        : 
版本        : vs_phoenix  Copyright(C) 2009-2016 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2016/7/6       1.0         zhou        创建
******************************************************************************/

#include "uaslogtask.h"
#include "cmuconst.h"
#include "cmuconf.h"
#include "loginsession.h"
#include "devlogintask.h"
#include "cms/uas/uas_proto.h"
#include "cms/cms_errorcode.h"
#include "cmudata.h"
#include "cbb/nms/snmpadp.h"
#include "cbb/nms/snmp_msg.h"


#define WAIT_UAS_LOG_RSP_TIMEOUT (20)

CUasLogTask::CUasLogTask(CInstExt* pInst) : CSipTask(pInst)
{
	m_strUasUri = g_cCmuConf.m_tLocalUri.GetUasURI().GetURIString();
	m_dwAlarmLogTop = m_dwDeviceLogTop = m_dwOperateLogTop = 0;
	m_wReqSendedTime = 0;
}

CUasLogTask::~CUasLogTask()
{
	m_dwAlarmLogTop = m_dwDeviceLogTop = m_dwOperateLogTop = 0;
	m_wReqSendedTime = 0;
    ReleaseResource();
}

void CUasLogTask::InitStateMachine()
{
    CStateProc cWaitRegProc;
    cWaitRegProc.ProcMsg = (CTask::PFProcMsg)&CUasLogTask::OnWaitRsp;
    cWaitRegProc.ProcErrMsg = (CTask::PFProcMsg)&CUasLogTask::OnWaitRsp;
    cWaitRegProc.TimerPoll = (CTask::PFTimerPoll)&CUasLogTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRegProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CUasLogTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CUasLogTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CUasLogTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    SendLog();
}

void CUasLogTask::ReleaseResource()
{
    switch(GetState())
    {
    case WaitRsp:
        {
        }
        break;

    case Service:
        {
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_UAS, ERROR_LEV, "未知事务状态[%d]\n", GetState());
        }
        break;
    }
}

u32 CUasLogTask::OnWaitRsp(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_FAIL;

	switch (pcMsg->event)
	{
	case BATCH_OPERATE_RSP:
		{
			CBatchOperateRsp cRsp;
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			pcOspSipMsg->GetMsgBody(cRsp);
			if (CMS_SUCCESS == cRsp.GetErrorCode())//整体判断，如果写入uas数据库成功清除m_cBatchReq的数据，失败则重新发送。
			{
				m_cBatchReq.GetData().clear();
				m_wReqSendedTime = 0;
			}
		}
		break;
	case UAS_ALARM_LOG_ADD_RSP:
		{
			CAlarmLogAddRsp cRsp;
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			pcOspSipMsg->GetMsgBody(cRsp);
			if (CMS_SUCCESS == cRsp.GetErrorCode())
			{
				m_tAlarmLogList.pop();
			}
		}
		break;
	case UAS_DEVICE_LOG_ADD_RSP:
		{
			CDeviceLogAddRsp cRsp;
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			pcOspSipMsg->GetMsgBody(cRsp);
			if (CMS_SUCCESS == cRsp.GetErrorCode())
			{
				m_tDeviceLogList.pop();
			}
		}
		break;
	case UAS_OPERATE_LOG_ADD_RSP:
		{
			COperateLogAddRsp cRsp;
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			pcOspSipMsg->GetMsgBody(cRsp);
			if (CMS_SUCCESS == cRsp.GetErrorCode())
			{
				m_tOperateLogList.pop();
			}
		}
		break;
	default:
		{
			TASKLOG(CMU_MOD_UAS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
	}

	SendLog();
	dwRet = PROCMSG_OK;

	return dwRet;
}

u32 CUasLogTask::OnWaitRspTimer()
{
	if (GetCurStateHoldTime() > WAIT_UAS_LOG_RSP_TIMEOUT)
	{
		SendLog();
	}

	return TIMERPOLL_DONE;
}

u32 CUasLogTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    return dwRet;
}

u32 CUasLogTask::OnServiceTimer()
{
	SendLog();

    return TIMERPOLL_DONE;
}

void CUasLogTask::PrintData() const
{
    CSipTask::PrintData();

	OspPrintf(TRUE, FALSE, "  AlarmLogList Size[%lu], Top[%lu]\n", m_tAlarmLogList.size(), m_dwAlarmLogTop);
	OspPrintf(TRUE, FALSE, "  DeviceLogList Size[%lu], Top[%lu]\n", m_tDeviceLogList.size(), m_dwDeviceLogTop);
	OspPrintf(TRUE, FALSE, "  OperateLogList Size[%lu], Top[%lu]\n", m_tOperateLogList.size(), m_dwOperateLogTop);    
}

void CUasLogTask::AddAlarmLog(const TAlarmLogInfo& var)
{
	m_tAlarmLogList.push(var);

	if (m_tAlarmLogList.size() > m_dwAlarmLogTop)
	{
		m_dwAlarmLogTop = m_tAlarmLogList.size();
	}
}

void CUasLogTask::AddDeviceLog(const TDeviceLogInfo& var)
{
	//向nms发送平台设备的上下线告警
	//enDeviceLogTypeOnline : enDeviceLogTypeOffline
	TSipURI tUri(var.devName);				// cmu和cmu之间的上下线日志，devName为cmu@xxx
	if (IsNeedNmsAlarmDevType(var.devName) || (tUri.IsValidURI() && tUri.GetUser() == DEV_TYPE_CMU))
	{
		TNmsAlarm tAlarm;
		tAlarm.byAlarmStatus = (var.logType==enDeviceLogTypeOffline) ? TNM_STATUS_ALARM : TNM_STATUS_NORMAL;
		tAlarm.dwAlarmCode = TNM_ALARM_CODE_MODULE_DISCONNECT;
		tAlarm.strModuleId = var.devUri;
		tAlarm.strModuleName = var.devName;
		tAlarm.strModuleIp = var.netAddr;

		((CCmuCoreInst*)GetInstance())->SendAlarmToNms(tAlarm);
	}
	
	m_tDeviceLogList.push(var);

	if (m_tDeviceLogList.size() > m_dwDeviceLogTop)
	{
		m_dwDeviceLogTop = m_tDeviceLogList.size();
	}
}

void CUasLogTask::AddPuLog(const TDeviceLogInfo& var)
{
	m_tDeviceLogList.push(var);
	
	if (m_tDeviceLogList.size() > m_dwDeviceLogTop)
	{
		m_dwDeviceLogTop = m_tDeviceLogList.size();
	}
}

void CUasLogTask::AddOperateLog(const TOperateLogInfo& var)
{
	m_tOperateLogList.push(var);
	
	if (m_tOperateLogList.size() > m_dwOperateLogTop)
	{
		m_dwOperateLogTop = m_tOperateLogList.size();
	}
}

void CUasLogTask::SendLog()
{
	//判断是否连接uas
	CLoginSession* pcUasLoginTask = NULL;
	g_cGeneralLoginList.Find(m_strUasUri, pcUasLoginTask);
	if (pcUasLoginTask == NULL)
	{
		TASKLOG(CMU_MOD_UAS, WARNING_LEV, "cmu not login uas, can't write log to uas\n");
		NextState(Service);
		return;
	}

	if (pcUasLoginTask->GetState() != CLoginSenderTask::Service)
	{
		TASKLOG(CMU_MOD_UAS, WARNING_LEV, "cmu login uas not in service, can't write log to uas\n");
		NextState(Service);
		return;
	}

	//请求重复发送三次，丢弃
	if (m_wReqSendedTime >= 3)
	{
		TASKLOG(CMU_MOD_UAS, ERROR_LEV, "[SendLog] req sended 3 times, discard!\n");
		m_cBatchReq.GetData().clear();
		m_wReqSendedTime = 0;
	}

	//没有收到应答，发送并清空DataList
	if (!m_cBatchReq.GetData().empty())
	{
		m_wReqSendedTime++;
		PostMsgReq(m_cBatchReq, pcUasLoginTask->GetServerSipUri());
		NextState(WaitRsp);
		return;
	}

	//判断是否有待发日志
	if (m_tDeviceLogList.empty() && m_tAlarmLogList.empty() && m_tOperateLogList.empty())
	{
		NextState(Service);
		return;
	}
	
	//if (!m_tAlarmLogList.empty())
	//{
	//	CAlarmLogAddReq cReq;
	//	cReq.SetSession(pcUasLoginTask->GetSession());
	//	cReq.SetInfo(m_tAlarmLogList.front());
	//	PostMsgReq(cReq, pcUasLoginTask->GetServerSipUri());
	//} 
	//else if (!m_tDeviceLogList.empty())
	//{
	//	CDeviceLogAddReq cReq;
	//	cReq.SetSession(pcUasLoginTask->GetSession());
	//	cReq.SetInfo(m_tDeviceLogList.front());
	//	PostMsgReq(cReq, pcUasLoginTask->GetServerSipUri());
	//} 
	//else if (!m_tOperateLogList.empty())
	//{
	//	COperateLogAddReq cReq;
	//	cReq.SetSession(pcUasLoginTask->GetSession());
	//	cReq.SetInfo(m_tOperateLogList.front());
	//	PostMsgReq(cReq, pcUasLoginTask->GetServerSipUri());
	//}
	//else{}

	bool bLongEnough = false;
	m_cBatchReq.SetSession(pcUasLoginTask->GetSession());
	u32 dwReqLen = m_cBatchReq.ToXml().size();
	vector<TGeneralData>& vctData = m_cBatchReq.GetData();
	TGeneralData tData;
	tData.type = BATCH_OPERATE_TYPE_LOG_ALARM;
	while ((!bLongEnough) && (!m_tAlarmLogList.empty()))
	{
		if (dwReqLen > NTF_BUF_MAX_SIZE)
		{
			bLongEnough = true;
			break;
		}
		tData.content = m_tAlarmLogList.front().ToXml();
		m_tAlarmLogList.pop();
		//53是<data><type>alarmlog</type><content></content></data>的长度
		//+100是为了规避无法准确取长度值，模糊计算
		dwReqLen += (tData.content.size()+100);
		vctData.push_back(tData);
	}

	tData.type = BATCH_OPERATE_TYPE_LOG_DEVICE;
	while ((!bLongEnough) && (!m_tDeviceLogList.empty()))
	{
		if (dwReqLen > NTF_BUF_MAX_SIZE)
		{
			bLongEnough = true;
			break;
		}
		tData.content = m_tDeviceLogList.front().ToXml();
		m_tDeviceLogList.pop();
		//54是<data><type>devicelog</type><content></content></data>的长度
		//+100是为了规避无法准确取长度值，模糊计算
		dwReqLen += (tData.content.size()+100);
		vctData.push_back(tData);
	}

	tData.type = BATCH_OPERATE_TYPE_LOG_OPERATE;
	while ((!bLongEnough) && (!m_tOperateLogList.empty()))
	{
		if (dwReqLen > NTF_BUF_MAX_SIZE)
		{
			bLongEnough = true;
			break;
		}
		tData.content = m_tOperateLogList.front().ToXml();
		m_tOperateLogList.pop();
		//50是<data><type>operatelog</type><content></content></data>的长度
		//+100是为了规避无法准确取长度值，模糊计算
		dwReqLen += (tData.content.size()+100);
		vctData.push_back(tData);
	}

	m_wReqSendedTime++;
	PostMsgReq(m_cBatchReq, pcUasLoginTask->GetServerSipUri());

	NextState(WaitRsp);
}
