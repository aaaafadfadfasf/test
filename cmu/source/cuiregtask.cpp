
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cmuconst.h"
#include "culogintask.h"
#include "cuiregtask.h"
#include "cmucoreapp.h"

#include "cmuconf.h"

CCuiList g_cCuiList;
extern CCmuConf g_cCmuConf;
extern TMappingTable g_tMappingTable;

CCuiRegTask::CCuiRegTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
	m_tNetSegAllInfo.Clear();
}

CCuiRegTask::~CCuiRegTask()
{
    ReleaseResource();
}

void CCuiRegTask::InitStateMachine()
{
    CStateProc cWaitRegProc;
    cWaitRegProc.ProcMsg = (CTask::PFProcMsg)&CCuiRegTask::OnWaitReg;
    cWaitRegProc.ProcErrMsg = (CTask::PFProcMsg)&CCuiRegTask::OnWaitReg;
    cWaitRegProc.TimerPoll = (CTask::PFTimerPoll)&CCuiRegTask::OnWaitRegTimer;
    AddRuleProc(WaitReg, cWaitRegProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CCuiRegTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CCuiRegTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CCuiRegTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitReg);
}

void CCuiRegTask::ReleaseResource()
{
    TASKLOG(CMU_MOD_CU, EVENT_LEV, "cui[%s]��¼��������\n",
        GetDevUri().c_str());

    switch(GetState())
    {
    case WaitReg:
        {
        }
        break;

    case Service:
        {
            //��uasд�豸������־
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

            /* 
            cui������֪ͨ��cui������cu����
            Ŀǰֱ�ӱ���ɾ�����еĵ�¼����
            ��������������������⣬����ͨ������Ϣ�����ַ�ʽ���cu��¼����
            */
            DestroyChildTasks();

            //���б���ɾ��
            g_cCuiList.Erase(GetDevUri());

        }
        break;

    default:
        {
            TASKLOG(ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    }

	m_tNetSegAllInfo.Clear();
}

u32 CCuiRegTask::OnWaitReg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case CUI_REG_REQ:
        {
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_CUI, ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CCuiRegReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

			//add by Royan 20130828 ����punch����
			if (cReq.GetIsSupportNA())
			{
				SetIsNeedPunch(true);

				//���������ȫ������pu��ƽ̨�������ϵ
			}
			else
			{
				SetIsNeedPunch(false);
			}

            SetDevUri(cReq.GetDevUri());
            if (g_cCuiList.Exist(GetDevUri()))
            {
                TASKLOG(CMU_MOD_CUI, ERROR_LEV, "�Ѵ��ڵ�cui[%s]��¼cmu\n",
                    GetDevUri().c_str());
                dwErrorCode = ERR_CMU_CUI_ALREADY_REG;
            }

            CCuiRegRsp cRsp;
            cRsp.SetErrorCode(dwErrorCode);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            //����ΨһUUID���豸����session�����豸URI��cmu���ú���rsp�ش�����¼�豸
            //Ŀǰֻ��cu��session����cmu��̬����ġ�
            //cRsp.SetSession(cReq.GetSession());
            cRsp.SetSession(GetDevUri());
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (dwErrorCode == CMS_SUCCESS)
            {
                //ע��ɹ�
                TASKLOG(CMU_MOD_CUI, CRITICAL_LEV, "cui[%s]��¼cmu�ɹ�!\n",
                    GetDevUri().c_str());

                SetSession(GetDevUri());
                SetDevType(DEV_TYPE_CUI);
                SetServerUri(OspSipGetLocalURI().GetURIString());

                //��ȡipaddlist
                SetIpAddrList(cReq.GetIpAddrList());

				//����������Ϣ
				SetNetSegAllInfo(cReq.GetNetSegmentInfo());

                //��ʼ����
                SetHBParam(GetDevSipUri());

                //��ӵ��б���
                g_cCuiList.Insert(GetDevUri(), this);

                //�������̬
                NextState(Service);

                //֪ͨstun��ַ
                ((CCmuCoreInst*)GetInstance())->UpdateDevStunAddr(GetDevSipUri());

                //��uasд�豸��־
                 ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));

				//֪ͨCUIӳ�����Ϣ
				NotifyCuiMappingTable();

                dwRet = PROCMSG_OK;
            }
            else
            {
                //ע��ʧ��
                TASKLOG(CMU_MOD_CUI, ERROR_LEV, "cui[%s]��¼cmuʧ��!\n",
                    GetDevUri().c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CUI, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuiRegTask::OnWaitRegTimer()
{
    return TIMERPOLL_DONE;
}

u32 CCuiRegTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
	case MAPPING_TABLE_NTF_RSP:
		{
			TASKLOG(CMU_MOD_VTDU, EVENT_LEV, "MAPPING_TABLE_NTF_RSP\n");
		}
		break;

	case UPDATE_REG_NETSEG_REQ:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_VTDU, ERROR_LEV, "ospsip ��ϢΪ��\n");
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			CUpdateRegNetSegReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
			m_tNetSegAllInfo = cReq.GetNetSegmentAllInfo();

			//����Ӧ��
			CUpdateRegNetSegRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			cRsp.SetErrorCode(CMS_SUCCESS);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			dwRet= PROCMSG_OK;
		}
		break;

    case OSP_SIP_DISCONNECT:
        {
            if (DisProc(pcMsg))
            {
                dwRet = PROCMSG_DEL;
            }
            else
            {
                dwRet = PROCMSG_FAIL;
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CUI, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuiRegTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

//֪ͨCUIӳ�����Ϣ
void CCuiRegTask::NotifyCuiMappingTable()
{
	//TODO Ӧ������NotifyTask����֤CUI�յ�
	CMappingTableNtfReq cReq;
	cReq.SetSession(g_cCmuConf.m_tLocalUri.GetURIString());
	cReq.SetMappingTable(g_tMappingTable);

	PostMsgReq(cReq, GetDevSipUri());
}


bool CCuiRegTask::DisProc(CMessage *const pcMsg)
{
    //��������

    COspSipMsg* pOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_CUI, ERROR_LEV, "ospsip ��ϢΪ��\n");
        return false;
    }

    //�Ƿ������
    TSipURI tDevUri;
    tDevUri.SetNOString(pOspSipMsg->GetMsgBody());
    if (tDevUri != GetDevSipUri())
    {
        TASKLOG(CMU_MOD_CUI, WARNING_LEV, "�յ���Ч������Ϣ�������豸[%s]\n", tDevUri.GetNOString());
        return false;
    }

    TASKLOG(CMU_MOD_CUI, CRITICAL_LEV, "cui[%s]��cmu����\n", tDevUri.GetNOString());
    return true;
}

void CCuiList::PrintData()
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cui��[%u]��\n", GetSize());    
    u32 i = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strUri, pTask))
        {
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. cui[%s]-session[%s]-taskNO[%u]\n", i, 
                strUri.c_str(), pTask->GetSession().c_str(), pTask->GetTaskNO());

			CCuiRegTask *pCuiRegTask=(CCuiRegTask*)pTask;
			OspPrintf(TRUE, FALSE, "\tCUI�����б�Size[%d]��\n",
				pCuiRegTask->GetNetSegAllInfo().GetNetSegmentList().size());

			TNetSegmentList::const_iterator it = pCuiRegTask->GetNetSegAllInfo().GetNetSegmentList().begin();
			for(; it != pCuiRegTask->GetNetSegAllInfo().GetNetSegmentList().end(); it++)
			{
				OspPrintf(TRUE, FALSE, "\t\tNic[%s] IP[%s] Mask[%s]\n",
					it->GetNicName().c_str(), it->GetNetIp().c_str(), it->GetNetMask().c_str());
			}
			OspPrintf(TRUE, FALSE, "\t\tDefaultRouteIP:[%s]\n", pCuiRegTask->GetNetSegAllInfo().GetDafaultRoute().c_str());
        }
    }

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cui��[%u]��\n", GetSize());
}
