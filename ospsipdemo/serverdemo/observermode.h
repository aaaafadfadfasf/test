/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : observermode.h
   相关文件    : 
   文件实现功能: 观察者模式
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/26   1.0         liangli        创建
******************************************************************************/
#ifndef _OBSERVER_MODE_H_
#define _OBSERVER_MODE_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

typedef std::string CObsMessage;
class CSubject;


class CObserver
{
public:
    friend CSubject;
    CObserver(CSubject *pSub);
    virtual ~CObserver();
private:
    CObserver(){};
    CObserver(const CObserver& cObsvr){};
    CObserver& operator=(const CObserver& cObsvr){};
public:
    virtual void Update(CObsMessage &cMsg) = 0;   //该接口用于观察者更新信息
    void Subscribe();
    void UnsubscribeReq();         //该接口用于观察者退订
protected:
    /*
    由于观察者模式在多线程环境下存在线程同步问题
    在Subject删除了一个观察者之后，用异步消息通知给Observer所在线程
    Observer处理此消息后再删除对象
    */
    void UnsubscribeNtf();         //该接口用于被观察者通知退订成功, (以上两个接口是异步应答机制)

protected:
    CSubject *m_pSubject;
    bool      m_bSubscribed;
private:
};

class CSubject
{
public:
    CSubject();
    virtual ~CSubject();
    typedef vector<CObserver *> CObsvrVector;
public:
    void RegisterObserver(CObserver *pObsvr); //注册一个观察者
    void RemoveObserver(CObserver *pObsvr);   //注销一个观察者
    /*
    由于观察者模式在多线程运行情况下有线程同步问题，所以这里采用异步消息的方式退订
    */
    virtual void NotifyObserver(CObsMessage &cMsg);    //通知所有在册观察者
public:
    /*====================================================================
    函数名      : ProcUnsubscribeReq
    功能        : 由于观察者模式在多线程运行情况下有线程同步问题，所以这里采用异步消息的方式退订
    算法实现    :
    引用全局变量:
    输入参数说明:
    返回值说明  : 成功:true，失败:false
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2011/10/26/             liangli
    ====================================================================*/
    void ProcUnsubscribeReq();
protected:

protected:
    CObsvrVector m_tObsvrArray;
private:
};


#endif //_OBSERVER_MODE_H_

//end of file

