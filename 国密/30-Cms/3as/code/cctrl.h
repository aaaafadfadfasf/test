/*****************************************************************************
   �ļ�ʵ�ֹ���: ����App���ⲿSIP��Ϣ����ں͸�������ҵ�����̿���
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/11   1.0        huzhiyun        ����
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
    void OnExternSipMsg(CMessage* const pcMsg);  // �ⲿSIP��Ϣ���
    void OnInnerOspMsg(CMessage* const pcMsg);   // �ڲ�OSP��Ϣ���

    void OnPowerOn();
    void OnTasExit();
    void OnProxyConnect();
    void OnProxyDisconnect();
    void OnLogin(CMessage* const pcMsg);  // ��¼����
    void OnLogout(CMessage* const pcMsg); // ע������
    void OnSipDisconnect(CMessage* const pcMsg);   // SIP������Ϣ
    void OnSipMsgProcFail(CMessage* const pcMsg);  // SIP������Ϣ
    void OnTaskMsgTransfer(CMessage* const pcMsg); // ��������Ϣ����
    void OnDbMsgTransfer(CMessage* const pcMsg);   // ��������Ϣ����
    void OnSyncDataChangeNtf(CMessage* const pcMsg); // ����ͬ�����ݱ��֪ͨ

private:
    set<u32> m_cWaitPoweronReadyModuleIds;   // �����ṩ����ǰ��������ϵ��ģ���б�
    set<u32> m_cWaitPoweroffReadyModuleIds;  // TAS�˳�ǰ�������ȥ���ģ���б�
};

class CCtrlAppData
{
public:
    CCtrlAppData(): m_bProxyConnected(false){}

    bool GetProxyConnectState(){return m_bProxyConnected;}
    void SetProxyConnectState(bool bConnected){m_bProxyConnected = bConnected;}

    /* �ͻ���ģ��Uri���� */
    bool IsClientModuleUriExist(const string &strUri);
    void AddClientModuleUri(const string &strUri);
    void DelClientModuleUri(const string &strUri);

    /* ��ע���֪ͨ�����񼯺� */
    set<CSsnTaskbase*>& GetChangeNtfSubscribeTasks(){return m_cChangeNtfSubscribeTasks;}
    void HandleChangeNtfReq(u16 wNtfReqId, string strNtfReqXml);

    /* SsnIdӳ��Task���� */
    CTask* FindTaskBySsnId(const string& strSsnId);
    void AddSsnId2TaskMap(const string& strSsnId, CTask* pcTask);
    void DelSsnId2TaskMap(const string& strSsnId);
    const map<string, CTask*>& GetSsnId2TaskMap() const;

    /* ����CU�û���¼ */
    void AddCuserName(const string &strName, const string &strSessionId, const string &strUserId);
    void DelCuserName(const string &strName, const string *pstrSessionId);
    bool IsCuserNameExist(const string &strName);
    bool GetCuserId(const string &strSessionId, string &strUserId);

    /* ��������TAS������������� */
    void AddTasConnTask(CConnTaskbase *pcTask);
    void DelTasConnTask(CConnTaskbase *pcTask);
    set<CConnTaskbase*> GetTasConnTaskSet() const;

    /* ����������TAS�ĻỰ������� */
    set<CSsnTaskbase*>& TasSessTasks();

    void Dump(u32 dwCode);
private:
    bool m_bProxyConnected; // proxy���ӱ�־
    set<string> m_cClientModuleUris;               // �ͻ���ģ��Uri����, ���ڷ�ֹ�ظ���¼
    set<CSsnTaskbase*> m_cChangeNtfSubscribeTasks; // ��ע���֪ͨ�����񼯺�, ���ڱ��֪ͨ
    map<string, CTask*> m_cSsnId2Task;          // �ỰId���Ự�������(CSsnTaskbase����)��ӳ�䣬����������Ϣ��Task��λ, ͨ��������Ϣ��SsnId��λ��Task
    map<string, u32> m_cOnlineCuserNames;       // ���ߵ�cu�û���(�����ִ�Сд)�����ڿ����û��Ķ��¼����(�ڶ����ֶα�ʾͬʱ���߸���)
    map<string, string> m_cCuserSessId2CuserId; // ����cu�Ự�ŵ��û�Id��ӳ��, ����App��DB�߳���Ҫ����.
    CXSemLock m_cLock4CuserSessId2CuserId;      // ���������m_cCuserSessId2CuserId��Ա

    set<CConnTaskbase*> m_cTasConnTasks;        // ������TAS֮����������񣨱�����Ϊ�ͻ��ˣ�
    set<CSsnTaskbase*> m_cTasSessTasks;         // ������TAS֮��ĻỰ���񣨱�����Ϊ����ˣ�
};

typedef COspApp<CCtrlInst, 1, CCtrlAppData> CCtrlApp;

extern CCtrlApp g_cCtrlApp;

#endif // _CCTRL_H_
