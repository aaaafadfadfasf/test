/*****************************************************************************
   文件实现功能: 中心App，外部SIP消息的入口和负责总体业务流程控制
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/11   1.0        huzhiyun        创建
******************************************************************************/
#ifndef _CCTRL_H_
#define _CCTRL_H_
#include "inner_common.h"
class CSsnTaskbase;
class CConnTaskbase;


class CCtrlAppData;
class CCtrlInst: public CInstExt
{
    enum ENState
    {
        enStateIdle = 0,
        enStateInit = 1,
        enStateRegProxy = 2,
        enStateServer = 3,
        enStateExit = 4,
    };
public:
    CCtrlInst(){}
    virtual ~CCtrlInst();

    CCtrlAppData& GetAppData();
private:
    void DaemonEntry(CMessage* const pcMsg);
    void NormalEntry(CMessage* const pcMsg){};
    virtual LPCSTR GetStrState() const;
    void DaemonInstanceDump(u32 dwCode);
private:
    void IdleStateHandle(CMessage* const pcMsg);
    void InitStateHandle(CMessage* const pcMsg);
    void RegProxyStateHandle(CMessage* const pcMsg);
    void ServerStateHandle(CMessage* const pcMsg);
    void ExitStateHandle(CMessage* const pcMsg);

private:
    void OnExternSipMsg(CMessage* const pcMsg);  // 外部SIP消息入口
    void OnInnerOspMsg(CMessage* const pcMsg);   // 内部OSP消息入口

    void OnPowerOn();
    void OnTasExit();
    void OnProxyConnect();
    void OnProxyDisconnect();
    void OnLogin(CMessage* const pcMsg);  // 登录处理
    void OnLogout(CMessage* const pcMsg); // 注销处理
    void OnSipDisconnect(CMessage* const pcMsg);   // SIP断链消息
    void OnSipMsgProcFail(CMessage* const pcMsg);  // SIP错误消息
    void OnTaskMsgTransfer(CMessage* const pcMsg); // 传输器消息处理
    void OnDbMsgTransfer(CMessage* const pcMsg);   // 传输器消息处理
    void OnSyncDataChangeNtf(CMessage* const pcMsg); // 处理同步数据变更通知

private:
    set<u32> m_cWaitPoweronReadyModuleIds;   // 对外提供服务前必须完成上电的模块列表
    set<u32> m_cWaitPoweroffReadyModuleIds;  // TAS退出前必须完成去电的模块列表
};

class CCtrlAppData
{
public:
    CCtrlAppData(): m_bProxyConnected(false){}

    bool GetProxyConnectState(){return m_bProxyConnected;}
    void SetProxyConnectState(bool bConnected){m_bProxyConnected = bConnected;}

    /* 客户端模块Uri集合 */
    bool IsClientModuleUriExist(const string &strUri);
    void AddClientModuleUri(const string &strUri);
    void DelClientModuleUri(const string &strUri);

    /* 关注变更通知的任务集合 */
    set<CSsnTaskbase*>& GetChangeNtfSubscribeTasks(){return m_cChangeNtfSubscribeTasks;}
    void HandleChangeNtfReq(u16 wNtfReqId, string strNtfReqXml);

    /* SsnId映射Task管理 */
    CTask* FindTaskBySsnId(const string& strSsnId);
    void AddSsnId2TaskMap(const string& strSsnId, CTask* pcTask);
    void DelSsnId2TaskMap(const string& strSsnId);
    const map<string, CTask*>& GetSsnId2TaskMap() const;

    /* 在线CU用户记录 */
    void AddCuserName(const string &strName, const string &strSessionId, const string &strUserId);
    void DelCuserName(const string &strName, const string *pstrSessionId);
    bool IsCuserNameExist(const string &strName);
    bool GetCuserId(const string &strSessionId, string &strUserId);

    /* 连接其它TAS的连接任务管理 */
    void AddTasConnTask(CConnTaskbase *pcTask);
    void DelTasConnTask(CConnTaskbase *pcTask);
    set<CConnTaskbase*> GetTasConnTaskSet() const;

    /* 服务于其它TAS的会话任务管理 */
    set<CSsnTaskbase*>& TasSessTasks();

    void Dump(u32 dwCode);
private:
    bool m_bProxyConnected; // proxy连接标志
    set<string> m_cClientModuleUris;               // 客户端模块Uri集合, 用于防止重复登录
    set<CSsnTaskbase*> m_cChangeNtfSubscribeTasks; // 关注变更通知的任务集合, 用于变更通知
    map<string, CTask*> m_cSsnId2Task;          // 会话Id到会话相关任务(CSsnTaskbase子类)的映射，用于请求消息的Task定位, 通过请求消息的SsnId定位到Task
    map<string, u32> m_cOnlineCuserNames;       // 在线的cu用户名(不区分大小写)，用于控制用户的多登录属性(第二个字段表示同时在线个数)
    map<string, string> m_cCuserSessId2CuserId; // 在线cu会话号到用户Id的映射, 中心App和DB线程需要访问.
    CXSemLock m_cLock4CuserSessId2CuserId;      // 保护上面的m_cCuserSessId2CuserId成员

    set<CConnTaskbase*> m_cTasConnTasks;        // 本端与TAS之间的连接任务（本端作为客户端）
    set<CSsnTaskbase*> m_cTasSessTasks;         // 本端与TAS之间的会话任务（本端作为服务端）
};

typedef COspApp<CCtrlInst, 1, CCtrlAppData> CCtrlApp;

extern CCtrlApp g_cCtrlApp;

#endif // _CCTRL_H_
