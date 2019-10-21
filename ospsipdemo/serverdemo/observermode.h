/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : observermode.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �۲���ģʽ
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/26   1.0         liangli        ����
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
    virtual void Update(CObsMessage &cMsg) = 0;   //�ýӿ����ڹ۲��߸�����Ϣ
    void Subscribe();
    void UnsubscribeReq();         //�ýӿ����ڹ۲����˶�
protected:
    /*
    ���ڹ۲���ģʽ�ڶ��̻߳����´����߳�ͬ������
    ��Subjectɾ����һ���۲���֮�����첽��Ϣ֪ͨ��Observer�����߳�
    Observer�������Ϣ����ɾ������
    */
    void UnsubscribeNtf();         //�ýӿ����ڱ��۲���֪ͨ�˶��ɹ�, (���������ӿ����첽Ӧ�����)

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
    void RegisterObserver(CObserver *pObsvr); //ע��һ���۲���
    void RemoveObserver(CObserver *pObsvr);   //ע��һ���۲���
    /*
    ���ڹ۲���ģʽ�ڶ��߳�������������߳�ͬ�����⣬������������첽��Ϣ�ķ�ʽ�˶�
    */
    virtual void NotifyObserver(CObsMessage &cMsg);    //֪ͨ�����ڲ�۲���
public:
    /*====================================================================
    ������      : ProcUnsubscribeReq
    ����        : ���ڹ۲���ģʽ�ڶ��߳�������������߳�ͬ�����⣬������������첽��Ϣ�ķ�ʽ�˶�
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��:
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
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

