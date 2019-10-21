/*
ͬ�����ݷ�����ϵͳ
���ܣ�
1. ����ͬ������ķ���
2. ������Ҫͬ���������ʱ������ͬ������
3. ֪ͨͬ������ϵͳ�ķ��Ͳ���
*/

#ifndef _SYNC_ANALYZER_MANAGER_H_
#define _SYNC_ANALYZER_MANAGER_H_
#include "inner_common.h"
#include "cms/ospext/osptool.h"
#include "data_sync/sync_analyzer_base.h"

class CSyncAnalyzerAppData;

/* ע�⣺���漸�������б���ֶ�Ϊnon-POD typeʱ����ָ�봫�ݲ���,�ڲ����ͷŵ�ֵַ */

// strDataDomainId��ʾ��ͬ�����ݵ�������
// ��β����еĲ����ֶ�����SyncDataType�����ģ�enDataType���ֶζ�Ӧ��ϵ����(�ַ��������ֶβ���string)��
// ����˵����[]���������ֶ�ֻ�Ա������ݵ�ͬ��������Ч
// enSyncDataDomainInfo         ���� ��(�����¼���ͱ���)��Ϣ ���Ϊ��
// enSyncDataDeviceModel        ���� �豸�ͺ���Ϣ ���Ϊ�ͺű��[���豸����]
// enSyncDataDeviceInfo         ���� �豸��Ϣ ���Ϊ�豸��š��豸���͡��豸��ǰ˽�����ԡ��豸֮ǰ˽������
// enSyncDataDeviceCapIndexName ���� �豸������������ ���Ϊ�豸��š�������ʶ����������[���豸����]
// enSyncDataDeviceGroup        ���� �豸������Ϣ ���Ϊ�豸������
// enSyncDataDeviceGroup2Dev    ���� �豸���黮�� ���Ϊ�豸�����š��豸��š��豸������ʶ���豸��������[���豸����]
// enSyncDataUserDevice         ���� �豸������Ȩ ���Ϊ��Ȩ�豸��š�CU�û�����CU�û�����
void SyncAnalyse(ENSyncDataType enDataType, ENSyncOperateType enOperateType, const string &strDataDomainId, ...);

void ConstructSyncDataAnalyzeInfo(TSyncDataAnalyzeInfo &tData, ENSyncDataType enDataType, ENSyncOperateType enOperateType, 
                                  const string &strDataDomainId, va_list parmList);
void ConstructSyncDataAnalyzeInfo(TSyncDataAnalyzeInfo &tData, ENSyncDataType enDataType, ENSyncOperateType enOperateType, 
                                  const string &strDataDomainId, ...);

class CSyncAnalyzerInst: public CInstExt
{
public:
    enum ENState
    {
        enStateIdle = 0,
        enStateInit = 1, 
        enStateServer = 2,
        enStateExit = 3
    };
    CSyncAnalyzerInst(): CInstExt(){}

    CSyncAnalyzerAppData& GetAppData();
    void DaemonEntry(CMessage* const pcMsg);
    void NormalEntry(CMessage* const pcMsg){};
    virtual LPCSTR GetStrState() const;
    void DaemonInstanceDump(u32 dwCode);

private:
    void IdleStateHandle(CMessage* const pcMsg);
    void InitStateHandle(CMessage* const pcMsg);
    void ServerStateHandle(CMessage* const pcMsg);
    void ExitStateHandle(CMessage* const pcMsg);

private:
    void Analyze(const TSyncDataAnalyzeInfo &tData);
    void OnPowerOn();
    void OnPowerOff();

private:
    void StartReadyExitDetectTimer();
    // ���������ദ�����ݿ⴫����Ϣ���ͷŴ�����Ϣ�ڴ�ͷ���task��������������ΪPROCMSG_DELʱ�Զ�ɾ����Ӧtask
    u32 ProcDbMsgTransfer(CMessage* const pcMsg);
};

class CSyncAnalyzerAppData
{
public:
    CSyncAnalyzerAppData(): m_pcSyncAnalyzerParent(NULL){}
    void Dump(u32 dwCode);

    set<string>& PrivateDevIds()
    {
        return m_cPrivateDevIds;
    }
    bool IsPrivateDev(const string &strDevId) const
    {
        return m_cPrivateDevIds.find(strDevId) != m_cPrivateDevIds.end();
    }

    void SubDomainIdInit(const vector<CDomainRelationInfo> &tInfo)
    {
        m_cLock4SubDomainIds.Lock();
        m_cSubDomainIds = tInfo;
        m_cLock4SubDomainIds.UnLock();
    }
    void SubDomainIdAdd(const CDomainRelationInfo &cInfo);
    void SubDomainIdDel(const UUIDString &tDomainId); // �Զ�ɾ�����������֮������
    bool IsSubDomainData(const string &strDomainId)
    {
        m_cLock4SubDomainIds.Lock();
        for (vector<CDomainRelationInfo>::const_iterator it = m_cSubDomainIds.begin(); it != m_cSubDomainIds.end(); ++it)
        {
            if (it->GetDomainID() == strDomainId)
            {
                m_cLock4SubDomainIds.UnLock();
                return true;
            }
        }

        m_cLock4SubDomainIds.UnLock();
        return false;
    }

    CSyncAnalyzerBase*& SyncAnalyzerParent()
    {
        return m_pcSyncAnalyzerParent;
    }
    vector<CSyncAnalyzerBase*>& SyncAnalyzerP2PLimit()
    {
        return m_cSyncAnalyzerP2PLimit;
    }

    u32 WaitingSyncRecordNumber();

private:
    set<string> m_cPrivateDevIds;                // ˽���豸��ű�
    vector<CDomainRelationInfo> m_cSubDomainIds; // �¼����ű�, CDomainRelationInfo�����ֶ���Ч
    CXSemLock m_cLock4SubDomainIds;              // ���������m_cSubDomainIds��Ա

    CSyncAnalyzerBase *m_pcSyncAnalyzerParent;           // ��ƽ̨ͬ��������
    vector<CSyncAnalyzerBase*> m_cSyncAnalyzerP2PLimit;  // ������Ȩͬ��������
};

typedef COspApp<CSyncAnalyzerInst, 1, CSyncAnalyzerAppData> CSyncAnalyzerApp;

extern CSyncAnalyzerApp g_cSyncAnalyzerApp;


void DelParentAnalyzer();
void AddParentAnalyzer(const UUIDString &tParentDomainId);

#endif // _SYNC_ANALYZER_MANAGER_H_
