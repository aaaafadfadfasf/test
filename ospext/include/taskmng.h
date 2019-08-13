#ifndef _OSP_TASK_MNG_H_INCLUDED_
#define _OSP_TASK_MNG_H_INCLUDED_

#include "cms/container/xmap.h"
#include "cms/container/xset.h"
#include "cms/container/xqueue.h"
#include "cms/ospext/osptool.h"
#include "ospexevt.h"

#include <vector>
using std::vector;

#define MAX_TASKNO                  ((u32)0x1FFFFFFF)
#define MAX_CHECK_TASK_REF_COUNT    (10000)
#define MAX_TASK_EACH_POLL_COUNT    (500)       //ÿ�η�����ѯ�����������
#define TASK_POLL_INTERVAL          (1000)      //������ѯ���

class CTask;

class CTaskNO
{
protected:
    CTaskNO();
public:
    static CTaskNO& GetTaskNOPool()
    {
        static CTaskNO s_cTaskNOPool;
        return s_cTaskNOPool;
    }
    u32 GenTaskNO();
    void RevokeTaskNO(const u32 dwTaskNO);

    //͵����һ�����õ�TaskNO(��Ӱ������ŵķ�����ͷ�), ���ڴ�ӡ
    u32 PeekNextTaskNO();
protected:
    u32 NextTaskNO();
protected:
/*
* ����֮���Բ�ʹ�ö�����������������, ����Ϊ
��Ȼ�ö��б�����ȻѸ��, �����޷���������д�ŵ������û���ظ�
��Ϊ�����޷���֤�û����ȥ���ñ��ӿ�
һ��������ظ�, ��ҵ��ͻ����
�������ﻹ��������ʵ��, ������±��ʾ��Ӧ�������, Byteֵ��ʾ�Ƿ����
    */
    u32 m_dwTaskNOGenerator;
    //u8  m_abyTaskNOUsed[MAX_TASKNO+1];
    CXSet<u32> m_cUsedTaskNOSet;

    //���ܱ����APPʹ�ã���Ҫ������
    CXSemLock m_cLock;
private:
};

/*
 * CTaskManager���ڵǼ�, ����, �������е�����
 * ͬʱCTaskManagerֻ����Taskָ��, ��Ӱ��ָ������Ӧ���ڴ�, �ڴ��new��delete���û��Լ����
 */

typedef CTask * LPTask;
typedef CXMap<u32, LPTask> TTaskPtrMap;

class CTaskManager
{
public:
    CTaskManager();
    virtual ~CTaskManager() {};
    /*====================================================================
    ������      : RegisterTask
    ����        : ע��һ������
    �㷨ʵ��    : ��������ָ�뱣����ȫ�ֶ�����,�����ṩͳһ��������ҽӿ�
                  �Լ������������ά��
    ����ȫ�ֱ���: 
    �������˵��: CTask *pcTask         Ҫע�������ָ��
                  ע��, ֻ��pcTask�е����������뱾�������Ӧ��, ����ע��ɹ�
    ����ֵ˵��  : �ɹ�TRUE, ʧ��FALSE
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    2010/17/7               liangli       create
    ====================================================================*/
    virtual BOOL32 RegisterTask( CTask *pcTask );

    /*====================================================================
    ������      : FindTask
    ����        : ��������
    �㷨ʵ��    : �������ṩһ��ȫ�ֵ��������, ��������������, ͳһ��Map�в���
                  ÿ���������͵�TaskManager������Ӧ��ʵ���Լ���FindTask
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : �Ҳ����򷵻�NULL, ���򷵻������ָ��
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    2010/17/7               liangli       create
    ====================================================================*/
    LPTask FindTask(const u32 dwTaskNO);

    /*====================================================================
    ������      : UnRegisterTask
    ����        : ע������
    �㷨ʵ��    : ÿ�����񶼻���һ����ʼ״̬, �������������ڵ��˻���������Ҫ����ʱ,
                  �����������ͷ���ռ�е���Դ, ��ת����ʼ״̬��, ��ָ�뽻��TaskManager
                  ��TaskManager��ע������, Ȼ���û�����delete ����, �ͷ��ڴ�

    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    2010/17/7               liangli       create
    ====================================================================*/
    virtual void   UnRegisterTask(CTask * pcTask);

    /*====================================================================
    ������      : OnTimerPollTasks
    ����        : ��ѯ��������, ���������Լ��Ķ�ʱ�¼�
    �㷨ʵ��    : ������ѯ
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    2010/19/7               liangli       create
    ====================================================================*/
    void OnTimerPollTasks();

    /*====================================================================
    ������      : GenerateTaskNO
    ����        : ����һ�����е������
    �㷨ʵ��    : ���ܼ򵥵ı���һ��ȫ�������,Ȼ������������, ��Ϊ����ſ��ܻ�һֱ�������ͷ�
                  ���Լ������ᵼ��������ظ�

                  �����ڿ�������ų����õ���һ�����������
                  ע��, �õ�������Ż������ų�����ʧ, �û���Ҫ�ô��������ע������
                  ����û���ʹ�ø������ע��, �����RevokeTaskNO

                  ����Ȳ�ʹ�������ע��, Ҳ���ٻ�, ��ᷢ�������"й¶"
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    2010/17/7               liangli       create
    ====================================================================*/
    static u32 GenerateTaskNO();

    /*====================================================================
    ������      : RevokeTaskNO
    ����        : ����������·������
    �㷨ʵ��    : CTaskManager��ʼ����, �Ὣ���п�������ŷ������
                  �û�����GenerateTaskNO�����ĳ��е�һ�������
                  �û�����RegisterTask�Ὣ����ű������б���
                  �û�����UnRegisterTask���Զ����������
                  ����û�������GenerateTaskNOȴ���ֲ���Ҫʹ��, ������������RevokeTaskNO�����������
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    2010/19/7               liangli       create
    ====================================================================*/
    static void RevokeTaskNO(const u32 dwTaskNO);

    static u32 PeekTaskNO();

    void PrintAllTask();
    void PrintTaskByNO(u32 dwTaskNO);
    void PrintTaskByType(LPCSTR szTaskType);
    void PrintPollTaskByType(LPCSTR szTaskType);

    size_t GetTaskNum();
    size_t GetPollTaskNum();

    void AddTaskToPollMap(u32 dwTaskNO);
    void EraseTaskFromPollMap(u32 dwTaskNO);
    void EmptyTask();

    //��̬��ȡ��ѯ���ʱ��
    u32 GetPollInterval();

    CXMap<u32, CTask*> GetTasksMap(LPCSTR szTaskType = NULL);
    vector<CTask*> GetTasksArray(LPCSTR szTaskType = NULL);



protected:
    //static member
    /*====================================================================
    ������      : InsertTaskToMap, FindTaskOnMap, EraseTaskFromMap
    ����        : TaskManager�е�Map��̬����, �������ʽӿ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    2010/20/7               liangli       create
    ====================================================================*/
    void InsertTaskToMap(const u32 dwTaskNO, LPTask lpTask);
    BOOL32 FindTaskOnMap(const u32 dwTaskNO, LPTask &lpTask);
    void EraseTaskFromMap(const u32 dwTaskNO);

protected:
    TTaskPtrMap  m_tTaskPtrMap;         //�ӿ�����ٶ�, ע��, Value��TTaskPtrList�е�Itemָ��
    //static CTaskNO m_tTaskNOPool;             //��������ų�
#define m_tTaskNOPool CTaskNO::GetTaskNOPool()

protected:
    u32            m_dwTaskCheckRef;    //������ѯ����

    TTaskPtrMap    m_tPollTaskPtrMap;
private:
};

//u16��ŷ���أ���֤�������Ψһ��
class CU16IndexPool
{
public:
    enum
    {
        MIN_INDEX = 0x0000,
        MAX_INDEX = 0xFFFF,
        INVALID_INDEX = 0xFFFF,
    };
public:
    CU16IndexPool(u16 wMinIndex = MIN_INDEX, u16 wMaxIndex = MAX_INDEX)
    {
        m_wMinIndex = wMinIndex;
        m_wMaxIndex = wMaxIndex;
        m_wIndexGenerator = m_wMinIndex;
    }
public:
    u16 PopIndex()
    {
        u16 wPollCnt = 0;
        u16 wRetIndex = INVALID_INDEX;
        while ( wPollCnt
            <=(m_wMaxIndex-m_wMinIndex+1) )
        {
            if ( !m_cUsedIndexSet.Exist(m_wIndexGenerator) )
            {
                wRetIndex = m_wIndexGenerator;
                m_cUsedIndexSet.Insert(m_wIndexGenerator);
                NextIndex();   //������m_wIndexGenerator��������, ��ֹ���һֱ�ظ�
                return wRetIndex;
            }

            NextIndex();
            wPollCnt++;
        }

        return INVALID_INDEX;
    }

    void PushIndex(const u16 wIndex)
    {
        if (wIndex >= m_wMinIndex && wIndex <= m_wMaxIndex )
        {
            m_cUsedIndexSet.Erase(wIndex);
        }
    }

protected:
    u16 NextIndex()
    {
        if (m_wIndexGenerator == m_wMaxIndex)
        {
            m_wIndexGenerator = m_wMinIndex;
        }
        else
        {
            m_wIndexGenerator++;
        }

        return m_wIndexGenerator;
    }


protected:
    u16 m_wIndexGenerator;
    CXSet<u16> m_cUsedIndexSet;
    u16 m_wMinIndex;
    u16 m_wMaxIndex;
};

/*====================================================================
    ����        : CTaskTimerManager
    ����        : ʵ��Task�ж�ʱ�����ܣ�֧��ͬһInstance�в�ͬtask������ͬTIME_ID�Ķ�ʱ������ΪOSP
                  ԭʼ�Ķ�ʱ����֧�ָù��ܣ�OSP��ͬһInstance��TIME_ID�Ķ�ʱ���Ḳ�ǡ�
    �㷨ʵ��    : 
                  1. ��ospext���ڲ���Ϣ���з���һ����[50500-54999]��task��ʱ��ʹ��
                  2. �ϲ�ͨ��task���ö�ʱ��ʱ����TIMER_ID�ᱻӳ�䵽�������Ķ��У�ȷ��ÿ�ε���ʱӳ���TIMER_IDΨһ
                  3. ӳ����TIMER_ID�ᱻ���õ�OSP��ԭʼ��ʱ����
                  4. ��ʱ������ʱ����ӳ����ϲ��TIMER_ID�����׸��ϲ㴦��
                  5. ͬһ��task��������ͬTIME_ID�Ḳ��
                  6. �����Ͼ��ǽ���ʱ���ĸ������Դ�Instance���𽵵�Task����
    ����ȫ�ֱ���: 
    �������˵��: 
    ����ֵ˵��  : 
    ----------------------------------------------------------------------
    �޸ļ�¼    : 
    ��  ��      �汾        �޸���        �޸�����
    2014/03/12              fanxg       create
    ====================================================================*/

#define  INVALID_TASK_TIMER_ID  (0xFFFF)
struct TTaskTimerInfo
{
    TTaskTimerInfo()
    {
        dwTaskNO = INVALID_TASKNO;
        wUserTimerID = INVALID_TASK_TIMER_ID;
    }
    u32 dwTaskNO;            //�����
    u16 wUserTimerID;        //�û���TimerID

    bool operator==(const TTaskTimerInfo& tOther) const
    {
        return (tOther.dwTaskNO == dwTaskNO && tOther.wUserTimerID == wUserTimerID);
    }
};

template <> struct CHashFunctor<TTaskTimerInfo>
{
    size_t operator()(const TTaskTimerInfo& key) const
    {
        unsigned char abyKey[sizeof(key.dwTaskNO) + sizeof(key.wUserTimerID)] = {0};
        memcpy(abyKey, &key.dwTaskNO, sizeof(key.dwTaskNO));
        memcpy(abyKey+sizeof(key.dwTaskNO), &key.wUserTimerID, sizeof(key.wUserTimerID));

        return hash_mysql((LPCSTR)abyKey, sizeof(key.dwTaskNO) + sizeof(key.wUserTimerID));
    }
};

class CTaskTimerManager
{
public:
    CTaskTimerManager() : m_cInnerTimerIDPool(OSP_EXT_EVENT_TASK_TIMER_ID_BEGIN, OSP_EXT_EVENT_TASK_TIMER_ID_END)
    {

    }
    ~CTaskTimerManager()
    {

    }

public:
    u16 GenTaskTimerID(const TTaskTimerInfo& tUserTimerInfo);

    u16 RevokeTaskTimerID(u16 wInnerTimerID);

    u16 FindInnerTimerIDByUserTimer(const TTaskTimerInfo& tUserTimerInfo) const;

    static bool IsTaskTimerID(u16 wEvent);

    void PrintData() const;

private:
    CU16IndexPool m_cInnerTimerIDPool;
    CXMap<u16, TTaskTimerInfo> m_cInnerTimerIDMapUserTimer;  //key��ӳ����ID��value��TTaskTimerInfo(�ϲ��)
    CXMap<TTaskTimerInfo, u16> m_cUserTimerIDMapInnerTimer;  //key��TTaskTimerInfo(�ϲ��), value��ӳ����ID
};


#endif  //#ifndef _OSP_TASK_MNG_H_INCLUDED_