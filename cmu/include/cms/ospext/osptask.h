/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��osptask.h
����ļ�  ��cospobj.h
ʵ�ֹ���  ������OSP��������ģ��
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/04/21         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _OSP_TASK_H_INCLUDED_
#define _OSP_TASK_H_INCLUDED_
#include "cms/ospext/osptool.h"
#include "cms/container/cospobj.h"
#include "cms/container/xmap.h"
#include "cms/ospext/osplog.h"

#define INVALID_IID     (0)          //��ЧIID
#define INVALID_TASKNO  (0)          //��Ч�����
#define INVALID_SEQ_NO  (0)          //��Ч���к�

//��ʱ��ѯ���ؽ��, 0��ʾ������ѯ������, 1��ʾ�����ѯ, 2��ʾ��������ѯ��ɾ��

//#define TIMERPOLL_SKIP 0    //ɾ��TIMERPOLL_SKIP��TIMERPOLL_SKIP�����TIMERPOLL_DONEһ��
#define TIMERPOLL_DONE 1
#define TIMERPOLL_DEL  2

//�����״̬��Ϣ�����ؽ��, 0��ʾ��Ϣ����ɹ�, 1��ʾ��Ϣ����ʧ��, 2��ʾ��������Ϣ�����ɾ��
typedef u32 PROCMSG_RESULT;
#define PROCMSG_OK   0
#define PROCMSG_FAIL 1
#define PROCMSG_DEL  2

class CInstExt;

struct TOspTransaction
{
	u32 dwIID;    //appid,instid
	u32 dwTaskID;  //task id
	u32 dwSeqNO;  //seq no

	TOspTransaction()
	{
		dwIID = INVALID_IID;
		dwTaskID = INVALID_TASKNO;
		dwSeqNO = INVALID_SEQ_NO;
	}

	bool operator==(const TOspTransaction& tOther) const
	{
		return (tOther.dwIID == dwIID && tOther.dwTaskID == dwTaskID);  //ע�⣬���ﲻ�Ƚ�SeqNO
	}
};

typedef TOspTransaction TOspTransation;

template <> struct CHashFunctor<TOspTransation>
{
	size_t operator()(const TOspTransation& key) const
	{
		unsigned char abyKey[sizeof(key.dwIID) + sizeof(key.dwTaskID)] = {0};
		memcpy(abyKey, &key.dwIID, sizeof(key.dwIID));
		memcpy(abyKey+sizeof(key.dwIID), &key.dwTaskID, sizeof(key.dwTaskID));

		return hash_mysql((LPCSTR)abyKey, sizeof(key.dwIID) + sizeof(key.dwTaskID));
	}
};


/*
 * CTask����������Ļ���, ���ﶨ����һ������Ļ�������
 */
class CTask : public COspObj
{
public:
    enum //�������������Զ����״̬ö��
    {
        UNKNOWN_STATE = 0,
    };
    typedef u32 (CTask::*PFProcMsg)(CMessage *const pcMsg);
    typedef u32 (CTask::*PFTimerPoll)(void);
    class CStateProc
    {
    public:
        CStateProc()
        {
            ProcMsg = NULL;
            TimerPoll = NULL;
            ProcErrMsg = NULL;
        }
        PFProcMsg ProcMsg;
        PFTimerPoll TimerPoll;
        PFProcMsg ProcErrMsg;
    };
    typedef CXMap<u32, CStateProc> CTaskRuleMap;
    

    CTask(CInstExt *pcInst);
    virtual ~CTask();

private:
    CTask(const CTask& cTask);
    CTask& operator=(const CTask& cTask);

public:
    virtual u32 ProcMsg(CMessage *const pcMsg)
    {
        CStateProc cProc;
        if ( !GetStateProc(GetState(), cProc) )
        {
            CMS_ASSERT( 0 && "[CTask::ProcMsg]��ȡCStateProcʧ�ܣ��Ƿ����Ǹ���InitStateMachine" );
            return PROCMSG_FAIL;
        }
        CMS_ASSERT( cProc.ProcMsg && "[CTask::ProcMsg]ProcMsg����ָ��Ϊ�գ��Ƿ����Ǹ���InitStateMachine" );

        PFProcMsg pfProcMsg = (PFProcMsg)cProc.ProcMsg;
        return (this->*pfProcMsg)(pcMsg);
    }

    /*====================================================================
    ������      : TimerPoll
    ����        : ÿ��������һ����ʱ��ѯ�¼�, ���¼�����������ഥ��
                  ÿ�����񶼿����ڴ˺����ڴ�������ʱ��������ά����Ϣ
    �㷨ʵ��    : �������ڱ�����������ʵ���ϵĴ���, ���緢�͹�������Ϣ, ��Ӧ�÷���TRUE
                  ������û�����κδ���, ���߳��Դ�����û�гɹ�, ��Ӧ�÷���FALSE

                  Ϊ�˷�ֹ��Ϣͻ��, �����������ڶ�ʱ������ʱ����������,
                  ���Ի���������TimerPoll�ж��Ƿ����������һ������
    ����ȫ�ֱ���:
    �������˵��:
    ����ֵ˵��  : û������������0, ����ж�������1, ��������������������2
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2010/19/7               liangli       create
    ====================================================================*/
    virtual u32 TimerPoll()
    {
        CStateProc cProc;
        if ( !GetStateProc(GetState(), cProc) )
        {
            CMS_ASSERT( 0 && "[CTask::TimerPoll]��ȡCStateProcʧ�ܣ��Ƿ����Ǹ���InitStateMachine" );
            return PROCMSG_FAIL;
        }
        CMS_ASSERT( cProc.TimerPoll && "[CTask::TimerPoll]TimerPoll����ָ��Ϊ�գ��Ƿ����Ǹ���InitStateMachine" );

        PFTimerPoll pfTimerPoll = (PFTimerPoll)cProc.TimerPoll;
        return (this->*pfTimerPoll)();
    }
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const = 0; //���븲�ǵķ���
    virtual const char* GetStateName(u32 dwState) const = 0; //���븲�ǵķ��� 
    virtual u32 TimerPollSkip();

    //�ϲ㶨��־�����ʱ����ʵ�ָ÷������÷���ָ����Щ״̬�������ǲ���Ҫ��ѯ�ġ�
    virtual bool IsNotNeedPollState(){return false;}

    /*====================================================================
    ������      : InitStateMachine
    ����        : ��״̬ö��+����ȡ��CState������CTask���ñ�������ʽ
                  �˷�����ʼ��״̬��
    �㷨ʵ��    : ��CTask���캯���е��ô˷���
    ����ȫ�ֱ���:
    �������˵��:
    ����ֵ˵��  : û������������0, ����ж�������1, ��������������������2
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2010/19/7               liangli       create
    ====================================================================*/
    virtual void InitStateMachine() = 0; //���븲�ǵķ���
public:
    //��־���

    //��ģ�����
    void Log(u8 byLogLevel, const char* szFormat, ...) const;

    //��ģ�����
    void Log(u8 byLogMod, u8 byLogLevel, const char* szFormat, ...) const;

    typedef CXMemFunLog<CTask> CTaskLog;

    #define TASKLOG (CTaskLog(this, FILE_NAME, FILE_LINE, TYPE_NAME, FUNC_NAME))

    //
    virtual void PrintData() const;

public:

    CTask *GetThisTask(){return this;}  //�����ڵ���STL����ʱ��������ת��

    //
    u32 GetState() const;

    //
    u32 GetCurStateHoldTime() const;

    bool GetStateProc(const u32 dwState, CStateProc &cProc);

    //
    u32 GetTaskNO() const;

    //
    void SetTaskNO(const u32 dwTaskNO);

    //
    bool IsTask(const char* szTaskName) const;

    //
    CInstExt* GetInstance() const;

    //
    u16 GetAppID() const;

    //
    u16 GetInsID() const;

	//
	TOspTransaction GetOspTrans() const;

    //
    u32 GetCheckRef() const;

    //
    void SetCheckRef(const u32 dwCheckRef);

    //
    void ClearPollCounter();

    //
    s32 GetPollCounter() const;

    //
    void AddPollCounter();

    //
    time_t GetCreateTime() const;

    //
    void SetCreateTime(time_t tCreateTime);

public:
    /*
      ����ʱ����
      ע�����ѯ��ʱ������
      ��ѯ��ʱ������instance��������ʵ���µ���������ʱִ��ĳ�ֲ�����
      ����������һֱ���ڡ�������ѯ��ʱ����ʱ��������������һ�롣

      ����ʱ����ÿ��������У�������һ���Եġ�����������ָ����ʱ��
      ʱ�����Լ���ʱ���¼��������ڵ�Ŀ���ǿ�����������Ծ�ȷ��ִ��
      ��ʱ��������������ͷ��������Ĳ���ʽ���ƣ�����ѯ��ʱ�����޷���ɡ�
    */

    /*====================================================================
    ������      : SetTimer
    ����        : ���ö�ʱ��
    �㷨ʵ��    : ����Osp�Ķ�ʱ��ʵ��
    ����ȫ�ֱ���:
                  dwTimerEvent��  ��ʱ���¼�
                  dwMilliSeconds����ʱ��ʱ��������λ�ǣ�ms
    �������˵��:
    ����ֵ˵��  : �ɹ�Ϊtrue��ʧ��Ϊfasle
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/02/23              fanxg          create
    ====================================================================*/
    bool SetTimer(u32 dwTimerEvent, long dwMilliSeconds);
    
    /*====================================================================
    ������      : KillTimer
    ����        : ɾ����ʱ��
    �㷨ʵ��    : ����Osp�Ķ�ʱ��ʵ��
    ����ȫ�ֱ���:
                  dwTimerEvent��  ��ʱ���¼�
    �������˵��:
    ����ֵ˵��  : �ɹ�Ϊtrue��ʧ��Ϊfasle
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2012/02/23              fanxg          create
    ====================================================================*/
    bool KillTimer(u32 dwTimerEvent);
    


protected:
    /*  
      �����״̬�л���
      1.����״̬����ʱ��;
      2.�����ѯ����
      3.�����ظ��л�����ǰ״̬�����ڶ�ͬһ��Ϣ(����NtfReq)�Ķ�γ�ʱ���
    */
    void NextState(const u32 dwState);

    //
    void InitCreateTime();

    //
    void AddRuleProc(u32 dwState, const CStateProc &cProc);

private:
    u32       m_dwTaskNO;          //������ˮ��, ��ЧID�� 1 �� 0xFFFF �޷���
    u32       m_dwCurState;       //��ǰ״̬
    u32       m_dwCheckRef;       //��ѯ�������ü���
    time_t    m_tCreateTime;      //����Ĵ���ʱ��
    s32       m_nPollCounter;     //��ѯ����, �����ж�����ʱ
    u64    m_tCurStateCreateTime;  //��ǰ״̬����ʱ�䣬��������״̬�ĳ���ʱ����г�ʱ�ж�
    CInstExt* m_pcInstance;       //���ڷ���Appʵ���ӿ�
    CTaskRuleMap m_mapRule;       //��¼����״̬ö���Լ����������
private:
};

#endif //#ifndef _OSP_TASK_H_INCLUDED_

