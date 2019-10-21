#ifndef _DEVGRP_MOD_TASK_H_
#define _DEVGRP_MOD_TASK_H_
#include "inner_common.h"
#include "cms/gbs/gbs_proto.h"
#include "cms/gbs/gbs_struct.h"
class CSsnTaskbase;

#define MAX_BATCH_DEAL_NUM 50

class CDevGrpModTask: public CSipTask
{
public:
    enum
    {
        enStateIdle  = UNKNOWN_STATE + 1,
		enStateQryTas,
		enStateQryGbid,
		enStateBatchMod,
		enStateMod,
    };

    CDevGrpModTask(CSsnTaskbase* pcSsnTask);
    ~CDevGrpModTask();
public:
	virtual void PrintData() const;
    virtual const char* GetObjName() const{return "CDevGrpModTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    u32 OnIdle(CMessage *const pcMsg);
	u32 OnQryTas(CMessage* const pcMsg);
	u32 OnQryTasTimePoll();
	u32 OnQryGbid(CMessage* const pcMsg);
	u32 OnQryGbidTimePoll();
	u32 OnBatchMod(CMessage* const pcMsg);
	u32 OnMod(CMessage* const pcMsg);
	u32 OnModTimePoll();

	void SendRsp(s32 ErrNo);//给cu的rsp


	void ConstructGbidGetReq();
	bool SendGbidBatchGetReq();
	bool SendDevModReq();
	bool SendBatchOperate();
	void SetProgress(int progress);

private:
	string m_strTreeId;//操作的设备树Id
	string m_strGrpId; //修改的设备组Id
	int m_dwPriSec;//主辅流 1.主流 2.辅流 3.全部
	string m_strGrpGbid;//修改的gbid

	TKDSIP_DIALOG_ID m_TransId;

	TSipURI m_tGbsUri;
	CUUID m_operUuid;
           
	vector<CDeviceGroupDeviceInfo4Gb>  m_vecDevGrpDevInfos; //从tas查到的设备信息 

	vector<TGbDevGbidBatchGetCondition>  m_vecBatchGetConditions;//需要查询gbid的设备
	u32 m_dwBatchIndex;

	vector<TOperateGbDevInfo>  m_vecLastNeedModDevs;
	TOperateGbDevInfo m_vecBatchOperate;//批量处理，主要是批量删除

	int m_dwProgress;//处理进度，和cu约定，如果cu发现本次的进度小于上次的进度，任务tas处理事务失败，返回错误


};

#endif // CDevGrpModTask