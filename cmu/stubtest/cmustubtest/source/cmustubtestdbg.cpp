#include "cmustubtestdbg.h"
#include "cmustubtestconst.h"

#include "cbb/dataswitch/datadispatch.h"
#include "cms/cmu/cmu_event.h"
#include "cms/tas/tas_event.h"
#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"
#include "cmustubtestcoreapp.h"
#include "cmustubtestdata.h"
#include "cmustubtestconf.h"
#include "invitetasktest.h"

extern CXSemLock g_cMainSemLock;
extern CDumpStatMap g_cDumpStatMap;

u64 g_TraceAddrID = 0;
u32 g_dwDumpLogLevel = DumpLog_Error;
u32 g_dwNALogLevel = emError;
CXSet<u16> g_cDumpPortSet;


//cmu��������
API void cmuhelp()
{
    OspPrintf(TRUE, FALSE, "%-20s: cmu��������\n","  cmuhelp");
    OspPrintf(TRUE, FALSE, "%-20s: cmu�汾��Ϣ\n","  cmuver");
    OspPrintf(TRUE, FALSE, "%-20s: cmu������Ϣ\n","  cmucfg()");
    OspPrintf(TRUE, FALSE, "%-20s: cmu״̬��Ϣ\n","  cmustat()");
    OspPrintf(TRUE, FALSE, "%-20s: ��ʾlog������Ϣ\n","  logpara()");
    OspPrintf(TRUE, FALSE, "%-20s: ��������ģ���ӡ����\n","  logall(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-20s: �ر����д�ӡ\n","  logoff()");
    OspPrintf(TRUE, FALSE, "%-20s: ���ô�ӡ����\n","  loglev(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-20s: ��ӡ��ǰ��������\n","  tasklist(LPCSTR szTaskType), szTaskTypeΪ��ʱ��ӡ�������񣬷����ӡָ��������б�");
    OspPrintf(TRUE, FALSE, "%-20s: ����taskNO��ӡָ������\n","  task(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-20s: traceָ������\n","  tracet(u16 wTaskNO)");
    OspPrintf(TRUE, FALSE, "%-20s: trace��һ���½�����\n","  tracent()");
    OspPrintf(TRUE, FALSE, "%-20s: ����pu�б�\n","  pulist(u8 byFlag), 0:��ӡ����PU, 1:��ӡ����PU");
    OspPrintf(TRUE, FALSE, "%-20s: ����cui�б�\n","  cuilist()");
    OspPrintf(TRUE, FALSE, "%-20s: ����cu�б�\n","  culist()");


    OspPrintf(TRUE, FALSE, "%-20s: cui��¼\n","  cuion()");
    OspPrintf(TRUE, FALSE, "%-20s: cu��¼\n","  cuon()");
    OspPrintf(TRUE, FALSE, "%-20s: cu�ǳ�\n","  cuoff()");
    OspPrintf(TRUE, FALSE, "%-20s: �����豸״̬\n","  ss()");
    OspPrintf(TRUE, FALSE, "%-20s: �����豸״̬\n","  ssall()");
    OspPrintf(TRUE, FALSE, "%-20s: PTZ����\n","  ptz()");


    OspPrintf(TRUE, FALSE, "%-20s: ��ȡ��Ƶ�������¼\n","  getvp()");
    OspPrintf(TRUE, FALSE, "%-20s: ������Ƶ�������¼\n","  setvp()");
    OspPrintf(TRUE, FALSE, "%-20s: ��ȡǰ��ϵͳ����\n","  getsp()");
    OspPrintf(TRUE, FALSE, "%-20s: ����ǰ��ϵͳ����\n","  setsp()");
    OspPrintf(TRUE, FALSE, "%-20s: ��ʼ��������\n","  invite(const char* szPuUri=NULL, s32 nPuChn=0, s32 nCuChn=0)");
    OspPrintf(TRUE, FALSE, "%-20s: ������·����\n","  ivtnum(u32 nNum)");
    OspPrintf(TRUE, FALSE, "%-20s: ������������\n","  byeinvite(u32 dwTask)");
    OspPrintf(TRUE, FALSE, "%-20s: ������·����\n","  byenum(u32 nNum)");

    OspPrintf(TRUE, FALSE, "%-20s: ����dump����\n","  dumploglev(u32 nLev)");
    OspPrintf(TRUE, FALSE, "%-20s: ����taskNO��MediaType����dump-trace\n","  dumptrace(u32 dwTaskNO, u32 dwMediaType = 0)");
    OspPrintf(TRUE, FALSE, "%-20s: ����RecvPort����dump-trace\n","  dumptraceport(u16 wRecvPort)");
    OspPrintf(TRUE, FALSE, "%-20s: ɾ��dump-trace\n","  dumpdelport(u16 wRecvPort)");
    OspPrintf(TRUE, FALSE, "%-20s: ���dump-trace��dump����\n","  dumptraceclear()");
    OspPrintf(TRUE, FALSE, "%-20s: ��ӡdumpstatͳ����Ϣ\n","  dumpstat()");

    OspPrintf(TRUE, FALSE, "%-20s: ��ʼ���ڵ���������\n","  invite3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn");
    OspPrintf(TRUE, FALSE, "%-20s: ��ʼ����������\n","  start3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn)");
    OspPrintf(TRUE, FALSE, "%-20s: ֹͣ����������\n","  stop3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn)");

    OspPrintf(TRUE, FALSE, "%-20s: natagent��������\n","  nahelp()");
    OspPrintf(TRUE, FALSE, "%-20s: natagent��������ͳһ���\n","  nacmd()");
    OspPrintf(TRUE, FALSE, "%-20s: NA��ӡȫ����Ϣ\n","  NAGlobalInfo()");
    OspPrintf(TRUE, FALSE, "%-20s: NA��ӡ�ص���������\n","  NACBList()");
    OspPrintf(TRUE, FALSE, "%-20s: NA��ӡ������������\n","  NARecvList()");
    OspPrintf(TRUE, FALSE, "%-20s: NA��ӡ������������\n","  NASendList()");
    OspPrintf(TRUE, FALSE, "%-20s: NA��ӡ������������\n","  NAKAList()");
    OspPrintf(TRUE, FALSE, "%-20s: NA��ӡ����\n","  naloglev(u32 dwLev)");

    OspPrintf(TRUE, FALSE, "%-20s: �˳�����\n","  cmuquit()");
}

#ifdef _LINUX_
void RegCmuCmd()
{
    OspRegCommand("cmuhelp", (void *)cmuhelp, "");
    OspRegCommand("cmuver", (void *)cmuver, "");
    OspRegCommand("cmucfg", (void *)cmucfg, "");    
    OspRegCommand("cmustat", (void *)cmustat, ""); 
    OspRegCommand("loglev", (void *)loglev, "");
    OspRegCommand("logall", (void *)logall, "");
    OspRegCommand("logoff", (void *)logoff, "");
    OspRegCommand("tasklist", (void *)tasklist, "");
    OspRegCommand("task", (void *)task, "");   
    OspRegCommand("tracet", (void *)tracet, "");
    OspRegCommand("tracent", (void *)tracent, "");
    OspRegCommand("puilist", (void *)puilist,"");
    OspRegCommand("pulist", (void *)pulist,"");
    OspRegCommand("cmulist", (void *)cmulist,"");
    OspRegCommand("cuilist", (void *)cuilist,"");
    OspRegCommand("culist", (void *)culist,"");
    OspRegCommand("cuion", (void *)cuion,"");
    OspRegCommand("cuon", (void *)cuon,"");
    OspRegCommand("cuoff", (void *)cuoff,"");
    OspRegCommand("ss", (void *)ss,"");
    OspRegCommand("ssall", (void *)ssall,"");
    
    OspRegCommand("ptz", (void *)ptz,"");
    OspRegCommand("getvp", (void *)getvp,"");
    OspRegCommand("setvp", (void *)setvp,"");
    OspRegCommand("getsp", (void *)getsp,"");
    OspRegCommand("setsp", (void *)setsp,"");
    OspRegCommand("invite", (void *)invite,"");
    OspRegCommand("ivtnum", (void *)ivtnum,"");
    OspRegCommand("byenum", (void *)byenum,"");

    OspRegCommand("dumploglev", (void *)dumploglev,"");
    OspRegCommand("dumptrace", (void *)dumptrace,"");
    OspRegCommand("dumptraceport", (void *)dumptraceport,"");
    OspRegCommand("dumpdelport", (void *)dumpdelport,"");
    OspRegCommand("dumptraceclear", (void *)dumptraceclear,"");
    OspRegCommand("dumpstat", (void *)dumpstat,"");
    
    OspRegCommand("byeinvite", (void *)byeinvite,"");
    OspRegCommand("invite", (void *)invite,"");
    OspRegCommand("invite3pc", (void *)invite3pc,"");
    OspRegCommand("start3pc", (void *)start3pc,"");
    OspRegCommand("stop3pc", (void *)stop3pc,"");
    OspRegCommand("cmuquit", (void *)cmuquit,"");

    OspRegCommand("nacmd", (void *)NACmd, "");
    OspRegCommand("nahelp", (void *)NAHelp, "");
    OspRegCommand("NAGlobalInfo", (void *)NAGlobalInfo, "");
    OspRegCommand("NACBList", (void *)NACBList, "");
    OspRegCommand("NARecvList", (void *)NARecvList, "");
    OspRegCommand("NASendList", (void *)NASendList, "");
    OspRegCommand("NAKAList", (void *)NAKAList, "");
    OspRegCommand("naloglev", (void *)naloglev, "");
}
#endif

//cmu�汾��Ϣ
API void cmuver()
{

}

//cmu������Ϣ
API void cmucfg()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_CONF);
}

//cmu״̬��Ϣ
API void cmustat()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_STAT);
}

//��ʾlog������Ϣ
API void logpara()
{
    OspShowLogParam();
}

//����log����
API void loglev(u8 byLev)
{
    OspSetLogLev(byLev, CMU_STUB_TEST_CORE_APP_ID);
}

//ͬʱ��������ģ��log����
API void logall(u8 byLev)
{
    OspSetLogLev(byLev, CMU_STUB_TEST_CORE_APP_ID);

    OspSetModLogLev(OSPEXT, byLev);
    OspSetModLogLev(OSPSIP, byLev);
}

//�ر�����ģ��log����
API void logoff()
{
    logall(0);
}

//��ӡ��������
API void tasklist(LPCSTR szTaskType)
{
    OspPrintTask(CMU_STUB_TEST_CORE_APP_ID, 0, szTaskType);
}

//��ӡָ��������Ϣ
API void task(u32 dwTaskNO)
{
    OspPrintOneTask(dwTaskNO, CMU_STUB_TEST_CORE_APP_ID, 0);
}

//��ӡ��������
API void pat()
{
    OspPrintTask(CMU_STUB_TEST_CORE_APP_ID, 0);
}

//��ӡָ��������Ϣ
API void pt(u16 wTaskNO)
{
    OspPrintOneTask(wTaskNO, CMU_STUB_TEST_CORE_APP_ID, 0);
}

//trace ָ������
API void tracet(u16 wTaskNO)
{
    OspTraceTask(wTaskNO);
}

//trace ��һ���½�������
API void tracent()
{
    OspTraceNextTask();
}

//���ߵ�pui�б�
API void puilist()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_PUI_LIST);
}

//���ߵ�pu�б�
API void pulist(u8 byFlag)
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_PU_LIST, &byFlag, sizeof(byFlag));
}

//���ӵ�cmu�б�
API void cmulist()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_CMU_LIST);
}

//���ߵ�cui�б�
API void cuilist()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_CUI_LIST);
}

//���ߵ�cu�б�
API void culist()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_CU_LIST);
}

//cui��¼
API void cuion()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CUI_REG_REQ);
}

//cu��¼
API void cuon(u32 dwCuNum)
{
    if (dwCuNum == 0)
    {
        dwCuNum = 1;
    }

    for (u32 i=dwCuNum; i>0; i--)
    {
        ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CU_LOGIN_REQ);
        OspDelay(30);
    }  
}

//cu�˳�
API void cuoff(u32 dwTask)
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CU_LOGOUT_REQ, &dwTask, sizeof(dwTask));
}

//�����豸״̬
API void ssall(u32 dwInterver)
{
    if (dwInterver == 0)
    {
        dwInterver = 10;
    }

    u32 dwPuNum = g_pszTestPu.size();
    OspPrintf(TRUE, FALSE, "��Ҫ����[%u]��pu\n", dwPuNum);

    for (u32 dwLoop = 0; dwLoop<dwPuNum; dwLoop++)
    {
        if (g_pszTestPu[dwLoop].size() > 0)
        {
            string strLocalDomain = GetLocalUri().GetDomain().c_str();
            string strTestPuUri = g_pszTestPu[dwLoop];

            strTestPuUri += "@";
            strTestPuUri += strLocalDomain;
            ss(strTestPuUri.c_str());

            OspPrintf(TRUE, FALSE, "����pu[%s]\n", strTestPuUri.c_str());
            OspDelay(dwInterver);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "��Чpu[%s]\n", g_pszTestPu[dwLoop].c_str());
        }
    }
}

API void ss(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_STATUS_SS_REQ, szPuUri, wLen);
}

//ȡ�������豸״̬
API void unss(u32 dwTask)
{
    COspSipMsg cMsg;
    cMsg.SetOspEventID(PU_STATUS_UNSS_REQ);
    cMsg.SetOspTaskNO(dwTask);
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_STATUS_UNSS_REQ, cMsg.GetMsgBuf(), cMsg.GetMsgLen());
}

//PTZ����
API void ptz(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_PTZ_CTRL_REQ, szPuUri, wLen);
}

//ǰ��ץ�ġ�����
API void snap(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_PIC_SNAPSHOT_REQ, szPuUri, wLen);
}

API void qrypupic(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_PIC_QRY_REQ, szPuUri, wLen);
}

//��ȡ��Ƶ�������
API void getvp(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_VID_ENC_PARAM_GET_REQ, szPuUri, wLen);
}

//������Ƶ�������
API void setvp(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_VID_ENC_PARAM_SET_REQ, szPuUri, wLen);
}

//��ȡǰ��ϵͳ����
API void getsp(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_SYS_PARAM_GET_REQ, szPuUri, wLen);
}

//����ǰ��ϵͳ����
API void setsp(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_SYS_PARAM_SET_REQ, szPuUri, wLen);
}

//��ѯǰ��¼�񣬲�����
API void qrypurec(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_REC_QRY_REQ, szPuUri, wLen);
}

//��ѯ�������
API void qryrecprog(u32 dwTask)
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PLAYBACK_PROGRESS_QUERY_REQ, &dwTask, sizeof(dwTask));
}

//����VCR����
API void vcr(u32 dwTask, const char* szVcrCmd)
{
    if (szVcrCmd == NULL)
    {
        OspPrintf(TRUE, FALSE, "szVcrCmd == NULL\n");
        return;
    }

    TVcrCtrlParam tParam;
    tParam.m_nTaskNO = dwTask;
    strcpy(tParam.m_szVcrCmd, szVcrCmd);
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, VCR_CTRL_REQ, &tParam, sizeof(tParam));
}

//����ǰ�˷���
API void byepurec(u32 dwTask)
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, BYE_REQ, &dwTask, sizeof(dwTask));
}

//��ʼ��Ƶ���
//invite "55000000000000000011200019800000@china" 0
API void invite(const char* szPuUri, s32 nPuChn, s32 nCuChn)
{
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
        return;
    }

    TInviteReqMsg tInviteReqMsg;
    strcpy(tInviteReqMsg.m_szSrcUri, szPuUri);
    tInviteReqMsg.m_nSrcChn = nPuChn;
    tInviteReqMsg.m_nDstChn = nCuChn;

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, INVITE_REQ, &tInviteReqMsg, sizeof(tInviteReqMsg));
}

API void ivtnum(u32 dwIvtNum, u32 dwInterval)
{
    if (dwIvtNum == 0)
    {
        dwIvtNum = 1;
    }

    if (dwInterval == 0)
    {
        dwInterval = 100;
    }

    u32 dwRemainIvtNum = dwIvtNum;

    static s32 s_CuChn = 0;

    //�������������pu
    CPuDataMap cOnlineNoInvitePulist = g_cPuList.GetOnLineNoInvitePuDataList();

    OspPrintf(TRUE, FALSE, "��������pu��[%d]\n", cOnlineNoInvitePulist.GetSize());

    if (cOnlineNoInvitePulist.GetSize() > 0)
    {
        for (u32 i=0; i<dwIvtNum;)
        {
            Iterator pPos; 
            while (!pPos.End())
            {
                string strPu;
                CPuData* pPuData = NULL;
                if (cOnlineNoInvitePulist.Iterate(pPos, strPu, pPuData) && pPuData != NULL)
                {
                    //ֻ���pu�ĵ�0��ͨ��
                    invite(strPu.c_str(), 0, s_CuChn);

                    OspPrintf(TRUE, FALSE, "invite pu[%s-%d] to CuChn[%d]\n", strPu.c_str(), 0, s_CuChn);

                    s_CuChn++;
                    i++;

                    if (dwRemainIvtNum != 0)
                    {
                        dwRemainIvtNum--;
                    }

                    if (i == dwIvtNum)
                    {
                        return;
                    }
                    OspDelay(dwInterval);
                }
            }

            break;
        } 
    }
    
    if (dwRemainIvtNum == 0)
    {
        return;
    }

    //�ٰ����ƽ�����䵽��������pu��
    CPuDataMap cOnlinePulist = g_cPuList.GetOnLinePuDataList();
    if (cOnlinePulist.GetSize() > 0)
    {
        for (u32 i=0; i<dwRemainIvtNum;)
        {
            Iterator pPos; 
            while (!pPos.End())
            {
                string strPu;
                CPuData* pPuData = NULL;
                if (cOnlinePulist.Iterate(pPos, strPu, pPuData) && pPuData != NULL)
                {
                    invite(strPu.c_str(), 0, s_CuChn);

                    OspPrintf(TRUE, FALSE, "invite pu[%s-%d] to CuChn[%d]\n", strPu.c_str(), 0, s_CuChn);

                    s_CuChn++;
                    i++;
                    if (i == dwRemainIvtNum)
                    {
                        return;
                    }
                    OspDelay(dwInterval);
                }
            }
        } 
    }  
}

API void byenum(u32 dwbyeNum, u32 dwInterval)
{
    if (dwbyeNum == 0)
    {
        dwbyeNum = 1;
    }

    if (dwInterval == 0)
    {
        dwInterval = 50;
    }

    CApp* ptApp = (CApp*)(&g_cCmuStubTestCoreApp);
    CCmuStubTestCoreInst* pInst = (CCmuStubTestCoreInst*)ptApp->GetInstance(CInstance::DAEMON);

    vector<CTask*> tIvtList = pInst->GetTasksArray("CInviteTask");

    size_t nHasAByeNum = 0;
    size_t nIvtNum = tIvtList.size();
    for (size_t i=0; i<nIvtNum; i++)
    {
        CTask* pcTask = tIvtList.at(i);
        if (pcTask != NULL && pcTask->GetState() == CInviteTask::Service)
        {
            byeinvite(pcTask->GetTaskNO());

            nHasAByeNum++;

            if (nHasAByeNum == dwbyeNum)
            {
                return;
            }

            OspDelay(dwInterval);           
        }      
    }
}

char *GetDumpLogLevStr(char *szLevel, int nBuffLen)
{
    memset(szLevel, 0, nBuffLen);
    if ( DumpLog_None == g_dwDumpLogLevel )
    {
        strcpy(szLevel, "None ");
    }
    else if ( DumpLog_All == g_dwDumpLogLevel )
    {
        strcpy(szLevel, "All ");
    }
    else
    {
        if ( g_dwDumpLogLevel & (1 << DumpLog_Error) )
        {
            strcat(szLevel, "Error ");
        }
        if ( g_dwDumpLogLevel & (1 << DumpLog_Warning) )
        {
            strcat(szLevel, "Warn ");
        }
        if ( g_dwDumpLogLevel & (1 << DumpLog_Info) )
        {
            strcat(szLevel, "Info ");
        }
    }

    return szLevel;
}

void dumplog( u32 elevel, const char * fmt, ... )
{
    if (g_dwDumpLogLevel & (1 << elevel) || DumpLog_All == elevel)
    {
        char szLevel[256] = {0};
        if ( DumpLog_Error == elevel )
        {
            strcpy(szLevel, " Error");
        }
        else if ( DumpLog_Warning == elevel )
        {
            strcpy(szLevel, " Warn");
        }
        else if ( DumpLog_Info == elevel )
        {
            strcpy(szLevel, " Info");
        }
        else
        {
            strcpy(szLevel, " All");
        }

        va_list argptr;
        va_start(argptr, fmt);
        char szPrintBuf[512] = {0};
        vsprintf(szPrintBuf, fmt, argptr);
        va_end(argptr);
        ::OspPrintf(TRUE, FALSE, "[DUMP%s] %s", szLevel, szPrintBuf);
    }
}

API void dumploglev(u32 dwLev)
{
    if (DumpLog_None == dwLev)
    {
        g_dwDumpLogLevel = DumpLog_None;
    }
    else if (DumpLog_Info < dwLev)
    {
        g_dwDumpLogLevel = DumpLog_All;
    }
    else
    {
        g_dwDumpLogLevel |= (1 << dwLev);
    }
    char szLevel[256] = {0};
    GetDumpLogLevStr(szLevel, sizeof(szLevel));
    OspPrintf(TRUE, FALSE, "[DS] log level : %s\n", szLevel);

    OspPrintf(TRUE, FALSE, "dumploglev = %u\n", g_dwDumpLogLevel);
}

API void dumptrace(u32 dwTaskNO, u32 dwMediaType)
{
    CApp* ptApp = (CApp*)(&g_cCmuStubTestCoreApp);
    CCmuStubTestCoreInst* pInst = (CCmuStubTestCoreInst*)ptApp->GetInstance(CInstance::DAEMON);

    CTask* pcTask = pInst->FindTask(dwTaskNO);
    if (pcTask != NULL)
    {
        CInviteTask* pcIvtTask = dynamic_cast<CInviteTask*>(pcTask);
        if (pcIvtTask != NULL)
        {
            if (dwMediaType == EDumpMediaBoth)
            {
                dumptraceport(pcIvtTask->GetVidPort());
                dumptraceport(pcIvtTask->GetAudPort());
            }
            else if (dwMediaType == EDumpMediaVideo)
            {
                dumptraceport(pcIvtTask->GetAudPort());
            }
            else if (dwMediaType == EDumpMediaAudio)
            {
                dumptraceport(pcIvtTask->GetAudPort());
            }
            else
            {
                OspPrintf(TRUE, FALSE, "��Ч��ý������\n");
            }
        }
        else
        {
            OspPrintf(TRUE, FALSE, "TaskNO[%u] is not CInviteTask\n");
        }
    }
    else
    {
        OspPrintf(TRUE, FALSE, "TaskNO[%s]û�ж�Ӧ������\n");
    }  
}

API void dumptraceport(u16 dwRecvPort)
{
    g_cDumpPortSet.Insert(dwRecvPort);
}

API void dumpdelport(u16 dwRecvPort)
{
    g_cDumpPortSet.Erase(dwRecvPort);
}

API void dumptraceclear()
{
    g_cDumpPortSet.Empty();
}

#include <algorithm>
#include <functional>
using namespace std;
typedef vector<TDumpStat> CDumpStatList;

void SortDumpStatList(const CDumpStatMap& cDumpStatMap, CDumpStatList &cDumpStatList )
{  
    Iterator pPos;
    while (!pPos.End())
    {
        u64 ddwKey = 0;
        TDumpStat tDumpStat;
        if (cDumpStatMap.Iterate(pPos, ddwKey, tDumpStat))
        {
            cDumpStatList.push_back(tDumpStat);
        }
    }

    //��Ŀ�Ķ˿ڴ�С��������DumpStat�б�
    sort(cDumpStatList.begin(), cDumpStatList.end(), less<TDumpStat>());   
}

API void dumpstat(u16 wRecvPort)
{
    OspPrintf(TRUE, FALSE, "dumploglev = %u\n", g_dwDumpLogLevel);

    if (g_cDumpPortSet.GetSize() > 0)
    {
        OspPrintf(TRUE, FALSE, "Current Trace Port list [%u]:\n", g_cDumpPortSet.GetSize());
        OspPrintf(TRUE, FALSE, "[");
        Iterator pPos; 
        while (!pPos.End())
        {
            u16 wPort = 0;
            if (g_cDumpPortSet.Iterate(pPos, wPort))
            {
                OspPrintf(TRUE, FALSE, "%d  ", wPort);
            }
        }
        OspPrintf(TRUE, FALSE, "]\n");
    }

    if (wRecvPort != 0)
    {
        TNetAddr tDstAddr;
        tDstAddr.SetNetIp(g_cCmuConf.m_tLocalAddr.GetIpStr());
        tDstAddr.SetNetPort(wRecvPort);
        u64 ddwKey = MakeAddrID(tDstAddr.GetNetIpv4(), tDstAddr.GetNetPort());
        TDumpStat tDumpStat;
        bool bFind = g_cDumpStatMap.Find(ddwKey, tDumpStat);
        if (bFind)
        {
            u32 dwPackDelay = (tDumpStat.m_ddwProcDataTime == 0) ? ( ~0) : u32(tDumpStat.m_ddwProcDataTime - tDumpStat.m_ddwPowerUpTime);
            OspPrintf(TRUE, FALSE, "[%04lu]. total:%010lu,lost:%012lu,repeat:%08lu,seq:%05u,SSRC:0x%08x,delay[%010lu],[%s:%d]->[%s:%d]\n",
                1, tDumpStat.m_dwTotal, tDumpStat.m_dwRTPLost, tDumpStat.m_dwRTPRepeat,
                tDumpStat.m_wExpectedSeqNO, tDumpStat.m_dwSSRC, dwPackDelay,
                tDumpStat.m_tSrcAddr.GetNetIp().c_str(), tDumpStat.m_tSrcAddr.GetNetPort(),
                tDstAddr.GetNetIp().c_str(), tDstAddr.GetNetPort());
        }
        else
        {
            OspPrintf(TRUE, FALSE, "[%s:%d] is not EXIST dump\n",
                tDstAddr.GetNetIp().c_str(), tDstAddr.GetNetPort());
        }

        return;
    }
 
    if (g_cDumpStatMap.GetSize() > 0)
    {
        u32 dwCount = 0;

        CDumpStatList cDumpStatList;
        SortDumpStatList(g_cDumpStatMap, cDumpStatList);
        CDumpStatList::iterator pItem = cDumpStatList.begin();
        while ( pItem != cDumpStatList.end() )
        {
            dwCount++;
            const TDumpStat& tDumpStat = *pItem;            
            const TNetAddr& tDstAddr = tDumpStat.m_tDstAddr;

            u32 dwPackDelay = (tDumpStat.m_ddwProcDataTime == 0) ? ( ~0) : u32(tDumpStat.m_ddwProcDataTime - tDumpStat.m_ddwPowerUpTime);
            OspPrintf(TRUE, FALSE, "[%04lu]. total:%010lu,lost:%012lu,repeat:%08lu,seq:%05u,SSRC:0x%08x,delay[%010lu],[%s:%d]->[%s:%d]\n",
                dwCount, tDumpStat.m_dwTotal, tDumpStat.m_dwRTPLost, tDumpStat.m_dwRTPRepeat,
                tDumpStat.m_wExpectedSeqNO, tDumpStat.m_dwSSRC, dwPackDelay,
                tDumpStat.m_tSrcAddr.GetNetIp().c_str(), tDumpStat.m_tSrcAddr.GetNetPort(),
                tDstAddr.GetNetIp().c_str(), tDstAddr.GetNetPort());

            pItem++;
        }       
    }
}

//������Ƶ���
API void byeinvite(u32 dwTask)
{
   ::OspPost(CMU_STUB_TEST_CORE_DAEMON, BYE_REQ, &dwTask, sizeof(dwTask));
}

//���ڵ���������
API void invite3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn)
{
	if (szSrcUri == NULL)
	{
		OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
	}

	TInviteReqMsg tInviteReqMsg;
	strcpy(tInviteReqMsg.m_szSrcUri, szSrcUri);
	tInviteReqMsg.m_nSrcChn = nSrcChn;
	strcpy(tInviteReqMsg.m_szDstUri, szDstUri);
	tInviteReqMsg.m_nDstChn = nDstChn;

	::OspPost(CMU_STUB_TEST_CORE_DAEMON, INVITE_THIRD_PARTY_CALL_REQ, &tInviteReqMsg, sizeof(tInviteReqMsg));
}

//��ʼ����������
//start3pc "55000000000000000011200019800000@china" 0 "55010000000000000011400000100000@china" 0
API void start3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn)
{
	if (szSrcUri == NULL)
	{
		OspPrintf(TRUE, FALSE, "szSrcUri == NULL\n");
	}
	if (szDstUri == NULL)
	{
		OspPrintf(TRUE, FALSE, "szDstUri == NULL\n");
	}

    TChannel tSrcChn(szSrcUri, nSrcChn);
    TChannel tDstChn(szDstUri, nDstChn);
    TSdp tSdp;
    tSdp.SetSessionName(SDP_SESSION_PLAY);

    CStartThirdPartyCallReq cReq;
    cReq.SetSrcChn(tSrcChn);
    cReq.SetDstChn(tDstChn);
    cReq.SetSdp(tSdp);

    string strMsgBody = cReq.ToXml();

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, START_THIRD_PARTY_CALL_REQ, strMsgBody.c_str(), strMsgBody.size());
}

//ֹͣ����������
//stop3pc "55000000000000000011200019800000@china" 0 "55010000000000000011400000100000@china" 0
API void stop3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn)
{
	if (szSrcUri == NULL)
	{
		OspPrintf(TRUE, FALSE, "szSrcUri == NULL\n");
	}
	if (szDstUri == NULL)
	{
		OspPrintf(TRUE, FALSE, "szDstUri == NULL\n");
	}

	TChannel tSrcChn(szSrcUri, nSrcChn);
	TChannel tDstChn(szDstUri, nDstChn);
	TSdp tSdp;
	tSdp.SetSessionName(SDP_SESSION_PLAY);

	CStopThirdPartyCallReq cReq;
	cReq.SetSrcChn(tSrcChn);
	cReq.SetDstChn(tDstChn);

	string strMsgBody = cReq.ToXml();

	::OspPost(CMU_STUB_TEST_CORE_DAEMON, STOP_THIRD_PARTY_CALL_REQ, strMsgBody.c_str(), strMsgBody.size());
}

//��¼als
API void onals()
{ 
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, DEV_LOGIN_REQ, UNIQUE_SIP_UA_ALS, strlen(UNIQUE_SIP_UA_ALS)+1);
}

//���ø澯��������
API void setal(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, ALARMLINK_CFG_SET_REQ, szPuUri, wLen);
}

//��ȡǰ�˸澯����
API void getal(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, ALARMLINK_CFG_GET_REQ, szPuUri, wLen);
}

//��ѯǰ�˸澯����
API void qryal(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, ALARMLINK_QUERY_REQ, szPuUri, wLen);
}

//��¼tvs
API void ontvs()
{ 
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, DEV_LOGIN_REQ, UNIQUE_SIP_UA_TVS, strlen(UNIQUE_SIP_UA_TVS)+1);
}

//����Ԥ��
API void savescm(const char* szTwId, const char* szSchemeName, int nTvNum = 0, int nTvDivNum = 0, int nPollNum = 0)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    if (szSchemeName == NULL)
    {
        OspPrintf(TRUE, FALSE, "szSchemeName == NULL\n");
        return;
    }


    TTwSchemeId tSchemeId;
    strcpy(tSchemeId.m_szTwId, szTwId);
    strcpy(tSchemeId.m_szSchemeName, szSchemeName);
    tSchemeId.m_nTvNum = nTvNum;
    tSchemeId.m_nTvDivNum = nTvDivNum;
    tSchemeId.m_nPollNum = nPollNum;

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, TVWALL_SCHEME_SAVE_REQ, &tSchemeId, sizeof(tSchemeId));
}

//ɾ��Ԥ��
API void delscm(const char* szTwId, const char* szSchemeName)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    if (szSchemeName == NULL)
    {
        OspPrintf(TRUE, FALSE, "szSchemeName == NULL\n");
        return;
    }


    TTwSchemeId tSchemeId;
    strcpy(tSchemeId.m_szTwId, szTwId);
    strcpy(tSchemeId.m_szSchemeName, szSchemeName);

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, TVWALL_SCHEME_DEL_REQ, &tSchemeId, sizeof(tSchemeId));
}

//����Ԥ��
API void loadscm(const char* szTwId, const char* szSchemeName)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    if (szSchemeName == NULL)
    {
        OspPrintf(TRUE, FALSE, "szSchemeName == NULL\n");
        return;
    }


    TTwSchemeId tSchemeId;
    strcpy(tSchemeId.m_szTwId, szTwId);
    strcpy(tSchemeId.m_szSchemeName, szSchemeName);

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, TVWALL_SCHEME_LOAD_REQ, &tSchemeId, sizeof(tSchemeId));
}

//��ѯԤ��
API void qryscm(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, ALARMLINK_QUERY_REQ, szPuUri, wLen);
}

//ʵʱ������ǽ
API void ontv(const char* szTwId, int nTvId, int nTvDivId)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    TTvChannel tTvChn;
    tTvChn.SetTvWallId(szTwId);
    tTvChn.SetTvId(nTvId);
    tTvChn.SetTvDivId(nTvDivId);

    string strTvChn = tTvChn.ToXml();

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, REAL_MEDIA_ON_TV_REQ, strTvChn.c_str(), strTvChn.size()+1);
}

//ʵʱ������ǽ
API void offtv(const char* szTwId, int nTvId, int nTvDivId)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    TTvChannel tTvChn;
    tTvChn.SetTvWallId(szTwId);
    tTvChn.SetTvId(nTvId);
    tTvChn.SetTvDivId(nTvDivId);

    string strTvChn = tTvChn.ToXml();

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, REAL_MEDIA_OFF_TV_REQ, strTvChn.c_str(), strTvChn.size()+1);
}

//������ѯ����
API void tvpoll(const char* szTwId, int nTvId, int nTvDivId, const char* szOprType)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    if (szOprType == NULL)
    {
        OspPrintf(TRUE, FALSE, "szOprType == NULL\n");
        return;
    }

    CTvPollOperateReq cReq;

    TTvChannel tTvChn;
    tTvChn.SetTvWallId(szTwId);
    tTvChn.SetTvId(nTvId);
    tTvChn.SetTvDivId(nTvDivId);

    cReq.SetTvChn(tTvChn);
    cReq.SetOprType(szOprType);

    string strReq = cReq.ToXml();

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, TV_POLL_OPERATE_REQ, strReq.c_str(), strReq.size()+1);
}

//����Ԥ����
API void setscmgrp(const char* szTwId)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    CTvWallSchemeGroupSetReq cReq;
    TTvWallSchemeGroup& tSchemeGroup = cReq.GetSchemeGroup();

    tSchemeGroup.SetTvWallId(szTwId);
    CTTvWallSchemePollList& tPollList = tSchemeGroup.GetPollList();

    for (size_t i=0; i<2; i++)
    {
        char szSchemeName[32] = {0};
        sprintf(szSchemeName, "s%u", i+1);
        TTvWallSchemePollUnit tUnit;
        tUnit.SetSchemeName(szSchemeName);
        tUnit.SetPollInex(i);
        tUnit.SetPollTime(i+10);

        tPollList.push_back(tUnit);
    }

    string strReq = cReq.ToXml();

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, TVWALL_SCHEME_GROUP_SET_REQ, strReq.c_str(), strReq.size()+1);
}

API void getscmgrp(const char* szTwId)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    CTvWallSchemeGroupGetReq cReq;
    cReq.SetTvWallId(szTwId);
    string strReq = cReq.ToXml();

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, TVWALL_SCHEME_GROUP_GET_REQ, strReq.c_str(), strReq.size()+1);
}

//Ԥ����ѯ����
API void scmpoll(const char* szTwId, const char* szOprType)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    if (szOprType == NULL)
    {
        OspPrintf(TRUE, FALSE, "szOprType == NULL\n");
        return;
    }

    CTvWallSchemePollOperateReq cReq;
    cReq.SetTvWallId(szTwId);
    cReq.SetOprType(szOprType);

    string strReq = cReq.ToXml();

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, TVWALL_SCHEME_POLL_OPERATE_REQ, strReq.c_str(), strReq.size()+1);
}

//����ǽ״̬����
API void twss(const char* szTwId)
{
    u16 wLen = 0;
    if (szTwId == NULL)
    {
        OspPrintf(TRUE, FALSE, "szTwId == NULL\n");
        return;
    }

    CTvWallStatusSsReq cReq;
    cReq.SetSession(g_cCmuConf.m_tLocalUri.GetURIString());
    cReq.SetDevUri(szTwId);
    cReq.SetUserUri(g_cCmuConf.m_tLocalUri.GetURIString());

    string strReq = cReq.ToXml();

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, TVWALL_STATUS_SS_REQ, strReq.c_str(), strReq.size()+1);
}

//�ƶ��������
API void setmd(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_MOTION_DETECT_SET_REQ, szPuUri, wLen);
}

//�ƶ�����ȡ
API void getmd(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_MOTION_DETECT_GET_REQ, szPuUri, wLen);
}

//ͼ���ڱ�����
API void setas(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_AREA_SHADE_SET_REQ, szPuUri, wLen);
}

//ͼ���ڱλ�ȡ
API void getas(const char* szPuUri)
{
    u16 wLen = 0;
    if (szPuUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szPuUri == NULL\n");
    }
    else
    {
        wLen = strlen(szPuUri)+1;
    }

    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_AREA_SHADE_GET_REQ, szPuUri, wLen);
}


//�˳�cmu
API void cmuquit()
{
    g_cMainSemLock.Give();
}

API void naloglev(u32 dwLev)
{
    g_dwNALogLevel = dwLev;
}

//��ʼ����Ϣ����
void InitCmuMsgDesc()
{
    //�ڲ���Ϣ
    OSP_ADD_EVENT_DESC(CMU_START);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CONF);
    OSP_ADD_EVENT_DESC(CMU_PRINT_STAT);
    OSP_ADD_EVENT_DESC(CMU_PRINT_PU_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CUI_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CU_LIST);
    OSP_ADD_EVENT_DESC(CMU_CMU_DISCONNECT);
    OSP_ADD_EVENT_DESC(CMU_CMU_CONNECT_SUCCESS);
    OSP_ADD_EVENT_DESC(CMU_CMU_CONNECT_FAIL);
    OSP_ADD_EVENT_DESC(CMU_PU_STATUS_UPDATE);
    OSP_ADD_EVENT_DESC(CMU_PUNCH_RESULT_NTF);
    OSP_ADD_EVENT_DESC(CMU_STUN_QRY_ADDR_NTF);
    OSP_ADD_EVENT_DESC(CMU_TASK_TIMER_TEST);
    OSP_ADD_EVENT_DESC(CMU_TASK_TIMER_UPDATE);
    OSP_ADD_EVENT_DESC(CMU_TASK_TIMER_KILL);
    

    //������Ϣ
    InitCmsEventDesc();
    InitTacTasEventDesc();
}