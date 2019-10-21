#ifndef _SSN_TASKBASE_H_
#define _SSN_TASKBASE_H_
#include "inner_common.h"
class CCtrlAppData;
class CQueryTask;
class CLongQueryTask;
class CUserDevGrpVidsrcQryTask;
class CDevGrpVidsrcTotalnumGetTask;

typedef string TChangeNtfItem; // string����Ϊ��Ϣ���л�֮��Ľ������

class CSsnTaskbase: public CSipTask
{
    friend class CQueryTask;
	friend class CLongQueryTask;
	friend class CUserDevGrpVidsrcQryTask;
	friend class CDevGrpVidsrcTotalnumGetTask;
public:
    CSsnTaskbase(CInstExt *pcInst);
    ~CSsnTaskbase();
    void PrintData() const;
    u32 PollChangeNotifyLst(); // ��ʱ��ѭ��������һ������֪ͨ��Ϣ
    void SaveNewChangeNotify(u16 wMsgId,TChangeNtfItem &cItem);

    virtual const char* GetObjName() const{return "ssn_taskbase";}
    virtual const char* GetStateName(u32 dwState) const{return "NA";}

    CCtrlAppData& AppData();
    TSipURI GetClientSipURI() const;

    u32 DefaultProc(CMessage *const pcMsg);
protected:
    void SetClientSipURI(const TSipURI& tUri);
    const CUUID& GetSessionId() const{return m_tSessionId;}
    bool SetHb(); // ��������

private:
    // ���֪ͨ������������true��ʾ��Ҫ֪ͨ
    virtual bool ChangeNotifyFilter(u16 /*wMsgId*/, TChangeNtfItem &cItem);
    void ReverseDeleteQueryTaskRecord(CQueryTask* pcTask); // ��ѯ��������ʱ������ô˺���
	void ReverseDeleteQueryTaskRecord(CLongQueryTask* pcTask);
	void ReverseDeleteQueryTaskNoNumRecord(CUserDevGrpVidsrcQryTask* pcTask);
	void ReverseDelGetDevGrpVidSrcNumRecord(CDevGrpVidsrcTotalnumGetTask* pcTask);
    u32 DoPollChangeNotifyLst();
public:
	bool GetIsNeedDevGrpGbId(){  return m_bNeedDevGrpGbId; }
protected:
    set<ENTasResourceTypes> m_cNtfFilterTypes;  // ָ֪ͨ�����͵ı��
	bool	m_bNeedDevGrpGbId;
private:
    bool m_bDestroying; // �Ự��������������־������ʱ��ѯ�������跴��ɾ������Ĺ�������

    TSipURI m_tClientUri; // �ͻ���ģ���URI
    bool m_bHbSetResult;  // �������ý��, �ɹ�֮����������������
    CUUID m_tSessionId;   // ����ĻỰ��
    // ���ڲ�ѯ����ʱ��̬֪ͨ�ݻ����ͣ������ѯ����붯̬֪ͨ��ͬ������(����ʱ���ܵõ�������)
    set<CQueryTask*> m_cQueryTaskSet;        // ��ѯ���񼯺�
	set<CLongQueryTask*> m_cLongQueryTaskSet; //����ѯ���񼯺�
	set<CUserDevGrpVidsrcQryTask*> m_cUserDevGrpVidsrcQryTaskSet; //Ӧ���Ҽ�¼����Ŀ�Ĳ�ѯ���񼯺�
	set<CDevGrpVidsrcTotalnumGetTask*> m_cDevGrpVidsrcNumGetTaskSet; 
    list<TChangeNtfItem> m_cChangeNotifyLst; // ���֪ͨ����(�������ӣ�ɾ�����޸ĵ�֪ͨ����ѯ�Ľ��֪ͨ�ڲ�ѯ�������)
    u32 m_dwPollCount; // poll������, ÿ�յ�һ�����֪ͨӦ����������㣬ÿ��poll����ʱ������������1��poll����������һ����ֵ֮��ͳ��Խ���һ�η���
    u32 m_dwPollDetectCount; // poll��ⶨʱ��

private: /* ҵ����ʵ�� */
    void OnQryReq(CMessage *const pcMsg);
    void OnChangeNtfReq(const CTaskMsgTransferBase* pcMsg); // ���ݿ�����ı��֪ͨ(������ɾ��)
    void OnChangeNtfRsp(CMessage *const pcMsg); // �Զ˶Ա��֪ͨ��Rsp
    void HandleChangeNtfRsp(s32 nSeqNo);

    virtual u32 OnLogout(CMessage *const pcMsg);
    virtual u32 OnDisconnect(CMessage *const pcMsg);
    virtual u32 OnSipMsgProcFail(CMessage *const pcMsg); // ����ʱ��ע����ø�ʵ�֣���������ȷ�Լ�������
};
#endif // _SSN_TASKBASE_H_
