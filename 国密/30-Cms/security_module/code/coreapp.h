#ifndef _COREAPP_H
#define _COREAPP_H

#include "cms/ospext/ospinst.h"
#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospapp.h"
#include "mesg.h"
#include "cms/dbix/dbresult_callback_imp4osptask.h"
#include "vkek_creat_task.h"
#include "vkek_query_task.h"
#include "vkek_save_task.h"
#include "certificate_query_task.h"
#include "del_vkek_task.h"
#include "nvkek_query_task.h"
#include "certificate_save_task.h"
#include "certificate_get_task.h"

struct TSecurityStat
{
	TSecurityStat()
	{
		bIsRegSipProxy = false;
	}

	void PrintData()
	{
		OspPrintf(TRUE, FALSE, "\nSECURITY状态信息：\n");

		OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
		OspPrintf(TRUE, FALSE, "  security启动时间   :[%s]\n", strStartTime.c_str());
		OspPrintf(TRUE, FALSE, "  security状态       :[%s]\n", strSecurityStat.c_str());
		OspPrintf(TRUE, FALSE, "  是否注册PROXY      :[%d]\n", bIsRegSipProxy);


		OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
	}

	string strStartTime;
	string strSecurityStat;
	bool   bIsRegSipProxy;

};


class CCoreAppData;

class CCoreInst: public CInstExt
{
    enum ENState
    {
        enStateIdle = 0,
        enStateServer = 1,
    };

public:
    CCoreInst();
    ~CCoreInst();
    CCoreAppData& AppData();

private:
    void DaemonEntry(CMessage* const pcMsg);
   // void NormalEntry(CMessage* const pcMsg){};
    virtual LPCSTR GetStrState() const;

    BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg);
    void IdleStateHandle(CMessage* const pcMsg);
  //  void InitStateHandle(CMessage* const pcMsg);
    void ServerStateHandle(CMessage* const pcMsg);
  //  void ExitStateHandle(CMessage* const pcMsg);

	void RegSipProxy(s32 nExpires = 3600);

	//事务创建
	virtual CTask* CreateTask(u16 wEvent);

	void ProcTaskMsg(CTask* pTask, CMessage* const pcMsg);
};

class CCoreAppData
{

};

typedef COspApp<CCoreInst, 1, CCoreAppData> CCoreApp;
extern CCoreApp *g_pcCoreApp;

extern TSecurityStat g_tSecurityStat;


#endif // _COREAPP_H
