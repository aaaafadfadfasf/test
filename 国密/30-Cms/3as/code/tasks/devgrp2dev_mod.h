#ifndef _DEVGRP2DEV_MOD_TASK_H_
#define _DEVGRP2DEV_MOD_TASK_H_
#include "inner_common.h"
#include "cms/gbs/gbs_proto.h"
#include "cms/gbs/gbs_struct.h"
class CSsnTaskbase;


class CDevGrp2DevModTask: public CSipTask
{
public:
    enum
    {
        enStateIdle  = UNKNOWN_STATE + 1,
		enStateGetGbid,
		enStateMod,
    };

    CDevGrp2DevModTask(CSsnTaskbase* pcSsnTask);
    ~CDevGrp2DevModTask();
public:
    virtual const char* GetObjName() const{return "CDevGrp2DevModTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    u32 OnIdle(CMessage *const pcMsg);
	u32 OnGetGbid(CMessage* const pcMsg);
	u32 OnGetGbidTimePoll();
	u32 OnMod(CMessage* const pcMsg);
	u32 OnModTimePoll();

	void AccordingTypeToStart();
	void SendGetGbid();
	bool SendDevModReq();
	void SendRsp(s32 ErrNo,const string& strGbid="");

private:
	CDeviceGroupDeviceMod4GbReq m_cModReq;

	TSipURI m_tGbsUri;
	TKDSIP_DIALOG_ID m_TransId;


	TGbDevGbidBatchGetCondition  m_BatchGetCondition;//需要查询gbid的设备

	TOperateGbDevInfo m_tOperateGbInfo;//需要操作的gb设备


	int m_operateType;//操作类型
};

#endif // _DEVGRP2DEV_MOD_TASK_H_