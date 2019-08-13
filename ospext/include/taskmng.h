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
#define MAX_TASK_EACH_POLL_COUNT    (500)       //每次分批轮询的最大任务数
#define TASK_POLL_INTERVAL          (1000)      //任务轮询间隔

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

    //偷看下一个可用的TaskNO(不影响事务号的分配和释放), 用于打印
    u32 PeekNextTaskNO();
protected:
    u32 NextTaskNO();
protected:
/*
* 这里之所以不使用队列来保存空闲任务号, 是因为
虽然用队列保存虽然迅捷, 但是无法保存队列中存放的任务号没有重复
因为我们无法保证用户如何去调用本接口
一旦任务号重复, 则业务就会出错
所以这里还是用数组实现, 数组的下标表示对应的任务号, Byte值表示是否可用
    */
    u32 m_dwTaskNOGenerator;
    //u8  m_abyTaskNOUsed[MAX_TASKNO+1];
    CXSet<u32> m_cUsedTaskNOSet;

    //可能被多个APP使用，需要锁保护
    CXSemLock m_cLock;
private:
};

/*
 * CTaskManager用于登记, 管理, 销毁所有的任务
 * 同时CTaskManager只管理Task指针, 不影响指针所对应的内存, 内存的new和delete由用户自己完成
 */

typedef CTask * LPTask;
typedef CXMap<u32, LPTask> TTaskPtrMap;

class CTaskManager
{
public:
    CTaskManager();
    virtual ~CTaskManager() {};
    /*====================================================================
    函数名      : RegisterTask
    功能        : 注册一个任务
    算法实现    : 将该任务指针保存至全局队列中,便于提供统一的任务查找接口
                  以及后续对任务的维护
    引用全局变量: 
    输入参数说明: CTask *pcTask         要注册的任务指针
                  注意, 只有pcTask中的任务类型与本管理类对应了, 才能注册成功
    返回值说明  : 成功TRUE, 失败FALSE
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    2010/17/7               liangli       create
    ====================================================================*/
    virtual BOOL32 RegisterTask( CTask *pcTask );

    /*====================================================================
    函数名      : FindTask
    功能        : 查找任务
    算法实现    : 本函数提供一个全局的任务查找, 不区分任务类型, 统一在Map中查找
                  每个任务类型的TaskManager的子类应该实现自己的FindTask
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 找不到则返回NULL, 否则返回任务的指针
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    2010/17/7               liangli       create
    ====================================================================*/
    LPTask FindTask(const u32 dwTaskNO);

    /*====================================================================
    函数名      : UnRegisterTask
    功能        : 注销任务
    算法实现    : 每个任务都会有一个初始状态, 当任务生命周期到了或者任务需要销毁时,
                  必须由任务释放其占有的资源, 并转至初始状态后, 将指针交给TaskManager
                  由TaskManager来注销任务, 然后用户才能delete 任务, 释放内存

    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    2010/17/7               liangli       create
    ====================================================================*/
    virtual void   UnRegisterTask(CTask * pcTask);

    /*====================================================================
    函数名      : OnTimerPollTasks
    功能        : 轮询所有任务, 让任务处理自己的定时事件
    算法实现    : 分批轮询
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    2010/19/7               liangli       create
    ====================================================================*/
    void OnTimerPollTasks();

    /*====================================================================
    函数名      : GenerateTaskNO
    功能        : 产生一个空闲的任务号
    算法实现    : 不能简单的保存一个全局任务号,然后做自增操作, 因为任务号可能会一直保留不释放
                  所以简单自增会导致任务号重复

                  这里在空闲任务号池中拿到第一个可用任务号
                  注意, 拿到的任务号会从任务号池中消失, 用户需要用此任务号来注册任务
                  如果用户不使用该任务号注册, 请调用RevokeTaskNO

                  如果既不使用任务号注册, 也不召回, 则会发生任务号"泄露"
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    2010/17/7               liangli       create
    ====================================================================*/
    static u32 GenerateTaskNO();

    /*====================================================================
    函数名      : RevokeTaskNO
    功能        : 将任务号重新放入池中
    算法实现    : CTaskManager初始化后, 会将所有空闲任务号放入池中
                  用户调用GenerateTaskNO会消耗池中的一个任务号
                  用户调用RegisterTask会将任务号保存至列表中
                  用户调用UnRegisterTask会自动回收任务号
                  如果用户调用了GenerateTaskNO却发现不需要使用, 则请主动调用RevokeTaskNO来回收任务号
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
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

    //动态获取轮询间隔时间
    u32 GetPollInterval();

    CXMap<u32, CTask*> GetTasksMap(LPCSTR szTaskType = NULL);
    vector<CTask*> GetTasksArray(LPCSTR szTaskType = NULL);



protected:
    //static member
    /*====================================================================
    函数名      : InsertTaskToMap, FindTaskOnMap, EraseTaskFromMap
    功能        : TaskManager中的Map静态函数, 公共访问接口
    算法实现    : 
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
    2010/20/7               liangli       create
    ====================================================================*/
    void InsertTaskToMap(const u32 dwTaskNO, LPTask lpTask);
    BOOL32 FindTaskOnMap(const u32 dwTaskNO, LPTask &lpTask);
    void EraseTaskFromMap(const u32 dwTaskNO);

protected:
    TTaskPtrMap  m_tTaskPtrMap;         //加快查找速度, 注意, Value是TTaskPtrList中的Item指针
    //static CTaskNO m_tTaskNOPool;             //空闲任务号池
#define m_tTaskNOPool CTaskNO::GetTaskNOPool()

protected:
    u32            m_dwTaskCheckRef;    //任务轮询计数

    TTaskPtrMap    m_tPollTaskPtrMap;
private:
};

//u16序号分配池，保证分配序号唯一性
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
                NextIndex();   //这里让m_wIndexGenerator不断自增, 防止序号一直重复
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
    类名        : CTaskTimerManager
    功能        : 实现Task中定时器功能，支持同一Instance中不同task设置相同TIME_ID的定时器，因为OSP
                  原始的定时器不支持该功能，OSP在同一Instance对TIME_ID的定时器会覆盖。
    算法实现    : 
                  1. 在ospext的内部消息段中分配一个段[50500-54999]给task定时器使用
                  2. 上层通过task设置定时器时，其TIMER_ID会被映射到上面分配的段中，确保每次调用时映射的TIMER_ID唯一
                  3. 映射后的TIMER_ID会被设置到OSP的原始定时器中
                  4. 定时器到来时，反映射出上层的TIMER_ID，并抛给上层处理
                  5. 同一个task中设置相同TIME_ID会覆盖
                  6. 本质上就是将定时器的覆盖特性从Instance级别降到Task级别
    引用全局变量: 
    输入参数说明: 
    返回值说明  : 
    ----------------------------------------------------------------------
    修改记录    : 
    日  期      版本        修改人        修改内容
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
    u32 dwTaskNO;            //事务号
    u16 wUserTimerID;        //用户层TimerID

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
    CXMap<u16, TTaskTimerInfo> m_cInnerTimerIDMapUserTimer;  //key是映射后的ID，value是TTaskTimerInfo(上层的)
    CXMap<TTaskTimerInfo, u16> m_cUserTimerIDMapInnerTimer;  //key是TTaskTimerInfo(上层的), value是映射后的ID
};


#endif  //#ifndef _OSP_TASK_MNG_H_INCLUDED_