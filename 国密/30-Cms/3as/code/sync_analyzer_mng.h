/*
同步数据分析子系统
功能：
1. 负责同步需求的分析
2. 对于需要同步的入库暂时保存于同步表中
3. 通知同步传输系统的发送部分
*/

#ifndef _SYNC_ANALYZER_MANAGER_H_
#define _SYNC_ANALYZER_MANAGER_H_
#include "inner_common.h"
#include "cms/ospext/osptool.h"
#include "data_sync/sync_analyzer_base.h"

class CSyncAnalyzerAppData;

/* 注意：下面几个函数中变参字段为non-POD type时利用指针传递参数,内部不释放地址值 */

// strDataDomainId表示被同步数据的所属域。
// 变参部分中的参数字段是由SyncDataType决定的，enDataType与字段对应关系如下(字符串类型字段采用string)：
// 补充说明：[]括起来的字段只对本域数据的同步分析有效
// enSyncDataDomainInfo         —— 域(包括下级域和本域)信息 变参为无
// enSyncDataDeviceModel        —— 设备型号信息 变参为型号编号[、设备类型]
// enSyncDataDeviceInfo         —— 设备信息 变参为设备编号、设备类型、设备当前私有属性、设备之前私有属性
// enSyncDataDeviceCapIndexName —— 设备能力索引名称 变参为设备编号、能力标识、能力索引[、设备类型]
// enSyncDataDeviceGroup        —— 设备分组信息 变参为设备分组编号
// enSyncDataDeviceGroup2Dev    —— 设备分组划归 变参为设备分组编号、设备编号、设备能力标识、设备能力索引[、设备类型]
// enSyncDataUserDevice         —— 设备跨域授权 变参为授权设备编号、CU用户名、CU用户域编号
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
    // 调用任务类处理数据库传输消息，释放传输消息内存和返回task处理结果，处理结果为PROCMSG_DEL时自动删除相应task
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
    void SubDomainIdDel(const UUIDString &tDomainId); // 自动删除自身和自身之后的域号
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
    set<string> m_cPrivateDevIds;                // 私有设备编号表
    vector<CDomainRelationInfo> m_cSubDomainIds; // 下级域编号表, CDomainRelationInfo域名字段无效
    CXSemLock m_cLock4SubDomainIds;              // 保护上面的m_cSubDomainIds成员

    CSyncAnalyzerBase *m_pcSyncAnalyzerParent;           // 父平台同步分析器
    vector<CSyncAnalyzerBase*> m_cSyncAnalyzerP2PLimit;  // 跨域授权同步分析器
};

typedef COspApp<CSyncAnalyzerInst, 1, CSyncAnalyzerAppData> CSyncAnalyzerApp;

extern CSyncAnalyzerApp g_cSyncAnalyzerApp;


void DelParentAnalyzer();
void AddParentAnalyzer(const UUIDString &tParentDomainId);

#endif // _SYNC_ANALYZER_MANAGER_H_
