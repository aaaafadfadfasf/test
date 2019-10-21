/*
CUIģ�����˻Ự����
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

    string Cusername; // ����������û�����
    u32 OnlineSeq;    // ������������к�
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
    
    // ����false��ʾδ�ҵ� �����ڵ�¼��sessIdΪ�գ�
    bool GetCuserSessId(const TCuserOnlineKey &cKey, string &strSessId);

private:
    map<TCuserOnlineKey, string> m_cCuserOnlineInfos; // CU�û����߼�¼���ɹ����ߺ�map��ֵ����SessId,���߹���������ֵ�ǿյ�
    map<string, TCuserOnlineKey> m_cCuserSsnId2CuserOnlineKey; // ����CU�û��ỰID��CU�û����߼�¼Key��ӳ�䣬��������ʱ����ɾ��CU�û����߼�¼
    map<string, TCuserOnlineKey> m_cCuserSsnId2CuserOnlineKey4Logining;// ��¼�����еģ����ݿ��Ǳ߻�û����ʱ�����ڴ�,����cui��ǰȡ����¼
    CCtrlAppData &m_cCtrlApp; // ����app�����ݶ���
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
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const{return "CCuiSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 ServerStateProcMsg(CMessage *const pcMsg);
private:
    void OnCuOnlineReq(CMessage* const pcMsg);
    void OnCuOfflineReq(CMessage* const pcMsg);
    void OnTaskMsgTransfer(CMessage* const pcMsg); // ��������Ϣ����
    void OnExportCuserDevData(CMessage* const pcMsg);
private: 
    CuserOnlineData m_cCuserOnlineData; // cu�û����߼�¼
};

#endif // _CUI_SSN_TASK_H_
