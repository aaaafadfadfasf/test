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


//cmu帮助命令
API void cmuhelp()
{
    OspPrintf(TRUE, FALSE, "%-20s: cmu帮助命令\n","  cmuhelp");
    OspPrintf(TRUE, FALSE, "%-20s: cmu版本信息\n","  cmuver");
    OspPrintf(TRUE, FALSE, "%-20s: cmu配置信息\n","  cmucfg()");
    OspPrintf(TRUE, FALSE, "%-20s: cmu状态信息\n","  cmustat()");
    OspPrintf(TRUE, FALSE, "%-20s: 显示log参数信息\n","  logpara()");
    OspPrintf(TRUE, FALSE, "%-20s: 设置所有模块打印级别\n","  logall(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-20s: 关闭所有打印\n","  logoff()");
    OspPrintf(TRUE, FALSE, "%-20s: 设置打印级别\n","  loglev(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-20s: 打印当前所有任务\n","  tasklist(LPCSTR szTaskType), szTaskType为空时打印所有事务，否则打印指定事务的列表");
    OspPrintf(TRUE, FALSE, "%-20s: 根据taskNO打印指定任务\n","  task(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-20s: trace指定事务\n","  tracet(u16 wTaskNO)");
    OspPrintf(TRUE, FALSE, "%-20s: trace下一个新建事务\n","  tracent()");
    OspPrintf(TRUE, FALSE, "%-20s: 在线pu列表\n","  pulist(u8 byFlag), 0:打印所有PU, 1:打印在线PU");
    OspPrintf(TRUE, FALSE, "%-20s: 在线cui列表\n","  cuilist()");
    OspPrintf(TRUE, FALSE, "%-20s: 在线cu列表\n","  culist()");


    OspPrintf(TRUE, FALSE, "%-20s: cui登录\n","  cuion()");
    OspPrintf(TRUE, FALSE, "%-20s: cu登录\n","  cuon()");
    OspPrintf(TRUE, FALSE, "%-20s: cu登出\n","  cuoff()");
    OspPrintf(TRUE, FALSE, "%-20s: 订阅设备状态\n","  ss()");
    OspPrintf(TRUE, FALSE, "%-20s: 订阅设备状态\n","  ssall()");
    OspPrintf(TRUE, FALSE, "%-20s: PTZ控制\n","  ptz()");


    OspPrintf(TRUE, FALSE, "%-20s: 获取视频编码参数录\n","  getvp()");
    OspPrintf(TRUE, FALSE, "%-20s: 设置视频编码参数录\n","  setvp()");
    OspPrintf(TRUE, FALSE, "%-20s: 获取前端系统参数\n","  getsp()");
    OspPrintf(TRUE, FALSE, "%-20s: 设置前端系统参数\n","  setsp()");
    OspPrintf(TRUE, FALSE, "%-20s: 开始请求码流\n","  invite(const char* szPuUri=NULL, s32 nPuChn=0, s32 nCuChn=0)");
    OspPrintf(TRUE, FALSE, "%-20s: 开启多路码流\n","  ivtnum(u32 nNum)");
    OspPrintf(TRUE, FALSE, "%-20s: 结束请求码流\n","  byeinvite(u32 dwTask)");
    OspPrintf(TRUE, FALSE, "%-20s: 结束多路码流\n","  byenum(u32 nNum)");

    OspPrintf(TRUE, FALSE, "%-20s: 设置dump级别\n","  dumploglev(u32 nLev)");
    OspPrintf(TRUE, FALSE, "%-20s: 根据taskNO和MediaType设置dump-trace\n","  dumptrace(u32 dwTaskNO, u32 dwMediaType = 0)");
    OspPrintf(TRUE, FALSE, "%-20s: 根据RecvPort设置dump-trace\n","  dumptraceport(u16 wRecvPort)");
    OspPrintf(TRUE, FALSE, "%-20s: 删除dump-trace\n","  dumpdelport(u16 wRecvPort)");
    OspPrintf(TRUE, FALSE, "%-20s: 清空dump-trace，dump所有\n","  dumptraceclear()");
    OspPrintf(TRUE, FALSE, "%-20s: 打印dumpstat统计信息\n","  dumpstat()");

    OspPrintf(TRUE, FALSE, "%-20s: 开始长期第三方交换\n","  invite3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn");
    OspPrintf(TRUE, FALSE, "%-20s: 开始第三方交换\n","  start3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn)");
    OspPrintf(TRUE, FALSE, "%-20s: 停止第三方交换\n","  stop3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn)");

    OspPrintf(TRUE, FALSE, "%-20s: natagent帮助命令\n","  nahelp()");
    OspPrintf(TRUE, FALSE, "%-20s: natagent调试命令统一入口\n","  nacmd()");
    OspPrintf(TRUE, FALSE, "%-20s: NA打印全局信息\n","  NAGlobalInfo()");
    OspPrintf(TRUE, FALSE, "%-20s: NA打印回调事务链表\n","  NACBList()");
    OspPrintf(TRUE, FALSE, "%-20s: NA打印接收事务链表\n","  NARecvList()");
    OspPrintf(TRUE, FALSE, "%-20s: NA打印发送事务链表\n","  NASendList()");
    OspPrintf(TRUE, FALSE, "%-20s: NA打印保活事务链表\n","  NAKAList()");
    OspPrintf(TRUE, FALSE, "%-20s: NA打印级别\n","  naloglev(u32 dwLev)");

    OspPrintf(TRUE, FALSE, "%-20s: 退出进程\n","  cmuquit()");
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

//cmu版本信息
API void cmuver()
{

}

//cmu配置信息
API void cmucfg()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_CONF);
}

//cmu状态信息
API void cmustat()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_STAT);
}

//显示log参数信息
API void logpara()
{
    OspShowLogParam();
}

//设置log级别
API void loglev(u8 byLev)
{
    OspSetLogLev(byLev, CMU_STUB_TEST_CORE_APP_ID);
}

//同时设置所有模块log级别
API void logall(u8 byLev)
{
    OspSetLogLev(byLev, CMU_STUB_TEST_CORE_APP_ID);

    OspSetModLogLev(OSPEXT, byLev);
    OspSetModLogLev(OSPSIP, byLev);
}

//关闭所有模块log级别
API void logoff()
{
    logall(0);
}

//打印所有事务
API void tasklist(LPCSTR szTaskType)
{
    OspPrintTask(CMU_STUB_TEST_CORE_APP_ID, 0, szTaskType);
}

//打印指定事务信息
API void task(u32 dwTaskNO)
{
    OspPrintOneTask(dwTaskNO, CMU_STUB_TEST_CORE_APP_ID, 0);
}

//打印所有事务
API void pat()
{
    OspPrintTask(CMU_STUB_TEST_CORE_APP_ID, 0);
}

//打印指定事务信息
API void pt(u16 wTaskNO)
{
    OspPrintOneTask(wTaskNO, CMU_STUB_TEST_CORE_APP_ID, 0);
}

//trace 指定事务
API void tracet(u16 wTaskNO)
{
    OspTraceTask(wTaskNO);
}

//trace 下一个新建的事务
API void tracent()
{
    OspTraceNextTask();
}

//在线的pui列表
API void puilist()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_PUI_LIST);
}

//在线的pu列表
API void pulist(u8 byFlag)
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_PU_LIST, &byFlag, sizeof(byFlag));
}

//连接的cmu列表
API void cmulist()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_CMU_LIST);
}

//在线的cui列表
API void cuilist()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_CUI_LIST);
}

//在线的cu列表
API void culist()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CMU_PRINT_CU_LIST);
}

//cui登录
API void cuion()
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CUI_REG_REQ);
}

//cu登录
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

//cu退出
API void cuoff(u32 dwTask)
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, CU_LOGOUT_REQ, &dwTask, sizeof(dwTask));
}

//订阅设备状态
API void ssall(u32 dwInterver)
{
    if (dwInterver == 0)
    {
        dwInterver = 10;
    }

    u32 dwPuNum = g_pszTestPu.size();
    OspPrintf(TRUE, FALSE, "需要订阅[%u]个pu\n", dwPuNum);

    for (u32 dwLoop = 0; dwLoop<dwPuNum; dwLoop++)
    {
        if (g_pszTestPu[dwLoop].size() > 0)
        {
            string strLocalDomain = GetLocalUri().GetDomain().c_str();
            string strTestPuUri = g_pszTestPu[dwLoop];

            strTestPuUri += "@";
            strTestPuUri += strLocalDomain;
            ss(strTestPuUri.c_str());

            OspPrintf(TRUE, FALSE, "订阅pu[%s]\n", strTestPuUri.c_str());
            OspDelay(dwInterver);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "无效pu[%s]\n", g_pszTestPu[dwLoop].c_str());
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

//取消订阅设备状态
API void unss(u32 dwTask)
{
    COspSipMsg cMsg;
    cMsg.SetOspEventID(PU_STATUS_UNSS_REQ);
    cMsg.SetOspTaskNO(dwTask);
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PU_STATUS_UNSS_REQ, cMsg.GetMsgBuf(), cMsg.GetMsgLen());
}

//PTZ控制
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

//前端抓拍、下载
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

//获取视频编码参数
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

//设置视频编码参数
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

//获取前端系统参数
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

//设置前端系统参数
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

//查询前端录像，并播放
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

//查询放像进度
API void qryrecprog(u32 dwTask)
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, PLAYBACK_PROGRESS_QUERY_REQ, &dwTask, sizeof(dwTask));
}

//放像VCR控制
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

//结束前端放像
API void byepurec(u32 dwTask)
{
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, BYE_REQ, &dwTask, sizeof(dwTask));
}

//开始视频浏览
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

    //先浏览空闲在线pu
    CPuDataMap cOnlineNoInvitePulist = g_cPuList.GetOnLineNoInvitePuDataList();

    OspPrintf(TRUE, FALSE, "空闲在线pu数[%d]\n", cOnlineNoInvitePulist.GetSize());

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
                    //只浏览pu的第0个通道
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

    //再把浏览平均分配到所有在线pu上
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
                OspPrintf(TRUE, FALSE, "无效的媒体类型\n");
            }
        }
        else
        {
            OspPrintf(TRUE, FALSE, "TaskNO[%u] is not CInviteTask\n");
        }
    }
    else
    {
        OspPrintf(TRUE, FALSE, "TaskNO[%s]没有对应的任务\n");
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

    //按目的端口从小到大排序DumpStat列表
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

//结束视频浏览
API void byeinvite(u32 dwTask)
{
   ::OspPost(CMU_STUB_TEST_CORE_DAEMON, BYE_REQ, &dwTask, sizeof(dwTask));
}

//长期第三方交换
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

//开始第三方交换
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

//停止第三方交换
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

//登录als
API void onals()
{ 
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, DEV_LOGIN_REQ, UNIQUE_SIP_UA_ALS, strlen(UNIQUE_SIP_UA_ALS)+1);
}

//设置告警联动参数
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

//获取前端告警联动
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

//查询前端告警联动
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

//登录tvs
API void ontvs()
{ 
    ::OspPost(CMU_STUB_TEST_CORE_DAEMON, DEV_LOGIN_REQ, UNIQUE_SIP_UA_TVS, strlen(UNIQUE_SIP_UA_TVS)+1);
}

//设置预案
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

//删除预案
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

//加载预案
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

//查询预案
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

//实时码流上墙
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

//实时码流上墙
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

//窗口轮询操作
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

//设置预案组
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

//预案轮询操作
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

//电视墙状态订阅
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

//移动侦测设置
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

//移动侦测获取
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

//图像遮蔽设置
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

//图像遮蔽获取
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


//退出cmu
API void cmuquit()
{
    g_cMainSemLock.Give();
}

API void naloglev(u32 dwLev)
{
    g_dwNALogLevel = dwLev;
}

//初始化消息描述
void InitCmuMsgDesc()
{
    //内部消息
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
    

    //网络消息
    InitCmsEventDesc();
    InitTacTasEventDesc();
}