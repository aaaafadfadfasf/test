/*
CUI模块服务端会话任务
*/

#ifndef _CUI_SSN_TASK_H_
#define _CUI_SSN_TASK_H_
#include "common/ssn_taskbase.h"

struct TCuserOnlineKey
{
    TCuserOnlineKey(){}
    TCuserOnlineKey(const string &name, u32 seq):Cusername(name), OnlineSeq(seq){}
    bool operator<(const TCuserOnlineKey &rh) const
    {
        int nCmpRet = Cusername.compare(rh.Cusername);
        if (nCmpRet < 0) return true;
        if (0 == nCmpRet && OnlineSeq < rh.OnlineSeq) return true;

        return false;
    }
    bool operator==(const TCuserOnlineKey &rh) const
    {
        return Cusername==rh.Cusername && OnlineSeq==rh.OnlineSeq;
    }

    string Cusername; // 上线请求的用户名称
    u32 OnlineSeq;    // 上线请求的序列号
};

class CCtrlAppData;
class CCuiSsnTask;
class CuserOnlineData
{
public:
    CuserOnlineData(CCtrlAppData &cAppData);
    ~CuserOnlineData();
    void PrintData() const;

    void AddCuserOnlineKey(const TCuserOnlineKey &cKey, const string& cuiAssignSessId);
    void DeleteCuserOnlineKey(const TCuserOnlineKey &cKey);
    void DeleteCuserOnlineKey(const string &strSessId);
    void AssignCuserSessId(const TCuserOnlineKey &cKey, const string &strSessId, const string &strCuserId, CCuiSsnTask *pcSsnTask);
    
    // 返回false表示未找到 （正在登录的sessId为空）
    bool GetCuserSessId(const TCuserOnlineKey &cKey, string &strSessId);

private:
    map<TCuserOnlineKey, string> m_cCuserOnlineInfos; // CU用户上线记录，成功上线后map的值将是SessId,上线过程中它的值是空的
    map<string, TCuserOnlineKey> m_cCuserSsnId2CuserOnlineKey; // 在线CU用户会话ID到CU用户上线记录Key的映射，用于下线时快速删除CU用户上线记录
    map<string, TCuserOnlineKey> m_cCuserSsnId2CuserOnlineKey4Logining;// 登录过程中的，数据库那边还没返回时保存于此,用于cui提前取消登录
    CCtrlAppData &m_cCtrlApp; // 中心app的数据对象
};

class CCuiSsnTask : public CSsnTaskbase
{
public:
    enum
    {
        enStateInit   = UNKNOWN_STATE + 1,
        enStateServer
    };
public:
    CCuiSsnTask(CInstExt *pcInst);
    ~CCuiSsnTask();
    void PrintData() const;
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const{return "CCuiSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 ServerStateProcMsg(CMessage *const pcMsg);
private:
    void OnCuOnlineReq(CMessage* const pcMsg);
    void OnCuOfflineReq(CMessage* const pcMsg);
    void OnTaskMsgTransfer(CMessage* const pcMsg); // 传输器消息处理
    void OnExportCuserDevData(CMessage* const pcMsg);
private: 
    CuserOnlineData m_cCuserOnlineData; // cu用户上线记录
};

#endif // _CUI_SSN_TASK_H_
