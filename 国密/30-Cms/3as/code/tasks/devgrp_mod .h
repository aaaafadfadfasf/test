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

	void SendRsp(s32 ErrNo);//��cu��rsp


	void ConstructGbidGetReq();
	bool SendGbidBatchGetReq();
	bool SendDevModReq();
	bool SendBatchOperate();
	void SetProgress(int progress);

private:
	string m_strTreeId;//�������豸��Id
	string m_strGrpId; //�޸ĵ��豸��Id
	int m_dwPriSec;//������ 1.���� 2.���� 3.ȫ��
	string m_strGrpGbid;//�޸ĵ�gbid

	TKDSIP_DIALOG_ID m_TransId;

	TSipURI m_tGbsUri;
	CUUID m_operUuid;
           
	vector<CDeviceGroupDeviceInfo4Gb>  m_vecDevGrpDevInfos; //��tas�鵽���豸��Ϣ 

	vector<TGbDevGbidBatchGetCondition>  m_vecBatchGetConditions;//��Ҫ��ѯgbid���豸
	u32 m_dwBatchIndex;

	vector<TOperateGbDevInfo>  m_vecLastNeedModDevs;
	TOperateGbDevInfo m_vecBatchOperate;//����������Ҫ������ɾ��

	int m_dwProgress;//������ȣ���cuԼ�������cu���ֱ��εĽ���С���ϴεĽ��ȣ�����tas��������ʧ�ܣ����ش���


};

#endif // CDevGrpModTask