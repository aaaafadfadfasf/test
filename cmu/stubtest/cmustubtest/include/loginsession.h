/*========================================================================================
ģ����    ��cmu
�ļ���    ��loginsession.h
����ļ�  ��
ʵ�ֹ���  �����е�¼���������࣬����pu��cu��pui��cui��vtdu��cmu�ȸ��ֵ�¼������Ҫ
            �����̳У��û���ʵ�ֵ�¼������������ͳһ����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/02/06         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _LOGIN_SESSION_H
#define _LOGIN_SESSION_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xset.h"

class CLoginSession : public CSipTask
{
public:
    CLoginSession(CInstExt *pcInst);
    virtual ~CLoginSession();


public:
    void AddTask(CTask* pTask);

    void DelTask(CTask* pTask);

    void DestroyChildTasks();

    bool IterateChildTasks(Iterator& rNextPosition, CTask*& pTask) const;

public:
    //ҵ����    
public:
    //���ݲ���
    const string& GetSession() const
    {
        return m_strSession;
    }
    string& GetSession()
    {
        return m_strSession;
    }
    void SetSession(const string& strSession)
    {
        m_strSession = strSession;
    }

private:
    string m_strSession;        //��¼session
    CXSet<CTask*> m_tTaskList;  //��¼session��ǰ���ڵ�Task�б�
};

#endif  //#ifndef _LOGIN_SESSION_H