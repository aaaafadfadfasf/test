/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��ospinst.h
����ļ�  ��osp.h
ʵ�ֹ���  ����չOSP�Ĺ���
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/04/18         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _OSP_INSTANCE_EXT_H
#define _OSP_INSTANCE_EXT_H

#include <vector>
using std::vector;

#include "cbb/osp/osp.h"
#include "cms/container/xmap.h"
#include "cms/ospext/osplog.h"


#ifdef _DEBUG
  #ifndef DEBUG
    #define DEBUG
  #endif
#endif

#define SELF_IID     (0xFFFFFFFF) //��INSTANCE��IID
#define SELF_NID     (0)          //����osp���

class CTask;

class CTaskManager;

class CTaskTimerManager;

/*
  ��չOSP�Ĺ���:
  1. ֧���������
  2. ֧����־�������
*/
class  CInstExt : public CInstance
{
public:
    friend class CTask;
public:
    CInstExt();
    virtual ~CInstExt();

private:
    CInstExt(const CInstExt&);
    void operator=(const CInstExt&);

private:
    //daemonʵ�����
    virtual void DaemonInstanceEntry(CMessage *const, CApp*);

    //normalʵ�����
    virtual void InstanceEntry(CMessage* const pcMsg);

    //�ڲ���Ϣ���
    BOOL32 OnInnerMsg(CMessage* const pcMsg);

    //�û��Զ����ͨ���ڲ���Ϣ�����ɸ��Ǵ˺�����һ������Daemon��Inst����������״̬����Ҫ�������Ϣ
    virtual BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg){return FALSE;};

    //ͨ��private��access��ֹ�ϲ�ֱ��ʹ�����·���, ��������������Ĳ��ɿ�

    //��ʵ�����������
    BOOL32 AddTask(CTask* pTask);

    //��ʵ����ɾ������
    BOOL32 DelTask(CTask* pTask);

    //��ʵ���������������ѯ����
    void OnTimerPollTasks();

public:
    //��ȡʵ��״̬���ƣ���Ҫ��������״̬ʵ�ָú���
    virtual LPCSTR GetStrState() const
    {
        return "NA";
    }

    //��ȡ��ǰAPP����
    LPCSTR GetStrApp() const;

    //��ȡAPPָ��
    CApp* GetAppPt() const;

    //������������������osp����ķ�constʵ��
    //��ѯ��ǰʵ��״̬��
    u32 CurState() const;

    //��ѯӦ�ú�
    u16 GetAppID() const;

    //��ѯʵ����
    u16 GetInsID() const;

    //����APPָ��
    void SetAppPt(CApp* pApp);

    //��ȡAPP��DATA������, APPTYPE������COspAppģ�������ɵ���
#define GETAPPDATA(APPTYPE) ((APPTYPE::AppDataType&)((APPTYPE*)(GetAppPt()))->GetAppData())

    //��ȡOSPIID
    u32 GetOspIID() const;

    //daemonʵ�����
    virtual void DaemonEntry(CMessage* const pcMsg);

    //normalʵ�����
    virtual void NormalEntry(CMessage* const pcMsg){};

    //��Ϣ����, ��� dwDstNode = SELF_IID, dwDstIId = 0, ��Ϣ������ʵ��
    virtual BOOL32 PostMsg(u16 wMsgId, const void* pMsgBuf = 0, u16 wMsgLen = 0, u32 wDstIId = SELF_IID, u32 dwDstNode = SELF_NID);

    //�������
    //�����¼���������,�����������Ĺ���ʽ�����Ƿ񸲸Ǹ÷���
    virtual CTask* CreateTask(u16 wEvent) {return NULL;};

    //������Ϣ����ͳһ��������ķ��ؽ��
    virtual void ProcTaskMsg(CTask* pTask, CMessage* const pcMsg);

    //���ʵ������������
    void EmptyTask();

    //��ӡʵ���е�������Ϣ
    void PrintTask(LPCSTR szTaskType=NULL) const;

    //��ӡʵ���е���ѯ������Ϣ
    void PrintPollTask(LPCSTR szTaskType=NULL) const;

    //�����������INST�е�����ָ��
    CTask* FindTask(u32 dwTaskNO) const;

    //��������Ŵӱ�ʵ��������APP��������ָ��
    CTask* FindTaskFromApp(u32 dwTaskNO) const;

    //��ȡ��ʵ������������
    u32 GetTaskNum() const;

    //��ȡ��ʵ����Ҫ��ѯ����������
    u32 GetPollTaskNum() const;

    //
    void AddTaskToPollMap(u32 dwTaskNO);
    void EraseTaskFromPollMap(u32 dwTaskNO);


    //��ȡָ������task��ָ�뼯�ϵ�map, szTaskTypeΪtask��������szTaskTypeΪ��ʱ��ȡ��������ָ��
    CXMap<u32, CTask*> GetTasksMap(LPCSTR szTaskType = NULL);

    //��ȡָ������task��ָ�뼯�ϵ�array, szTaskTypeΪtask��������szTaskTypeΪ��ʱ��ȡ��������ָ��
    vector<CTask*> GetTasksArray(LPCSTR szTaskType = NULL);

    //����Task��ʱ��
    bool SetTaskTimer(u32 dwTimerEvent, long dwMilliSeconds, u32 dwTaskNO);

    //ɾ��Task��ʱ��
    bool KillTaskTimer(u32 dwTimerEvent, u32 dwTaskNO);

    //��־���

    //��ģ�����
    void Log(u8 byLogLevel, const char* szFormat, ...) const;

    //��ģ�����
    void Log(u8 byLogMod, u8 byLogLevel, const char* szFormat, ...) const;

    typedef CXMemFunLog<CInstExt> CInstLog;

    #define INSTLOG (CInstLog(this, FILE_NAME, FILE_LINE, TYPE_NAME, FUNC_NAME))

private:

    //��instance����������
    CTaskManager* m_pTaskMng;

    //task����Ķ�ʱ��������
    CTaskTimerManager* m_pTaskTimerMng;

    //������ѯ��ʱ��״̬
    BOOL32 m_bTimerIsRunning;

    //ʵ���Ĵ�ӡ����
    u8 m_byLogLev;

    //ʵ��������APP��ָ��
    CApp* m_pApp;

    //���ʵ������
    void ClearData();
};

#endif  //#ifndef _OSP_INSTANCE_EXT_H
