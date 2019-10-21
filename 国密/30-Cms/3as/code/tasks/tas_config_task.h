/*
tas��������
*/
#ifndef _TAS_CONFIG_TASK_H_
#define _TAS_CONFIG_TASK_H_

#include "inner_common.h"

class CTasConfigTask: public CSipTask
{
public:
    enum
    {
        enStateIdle = UNKNOWN_STATE + 1,
        enStateConfigQuery,            // ��ȡtas����
        enStateConfigDomainName,       // ��������
        enStateConfigParentDomain,     // ��������
        enStateConfigChildDomainAdd,   // �����������
        enStateConfigChildDomainDel,   // ɾ����������
    };

    CTasConfigTask(CInstExt *pcInst);
    ~CTasConfigTask();

    virtual const char* GetObjName() const{return "CTasConfigTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();

private:
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnConfigQuery(CMessage *const pcMsg);
    u32 OnConfigDomainName(CMessage *const pcMsg);
    u32 OnConfigParentDomain(CMessage *const pcMsg);
    u32 OnConfigChildDomainAdd(CMessage *const pcMsg);
    u32 OnConfigChildDomainDel(CMessage *const pcMsg);

    void DelOldParentTasConnTask();
    void AddNewParentTasConnTask();
    void ResumeOldParentTasConnTask();

private:
    /* ���������ϼ��� */
    UUIDString m_tParentDomainId;
    string m_strParentDomainName;
    UUIDString m_tOldParentDomainId;
    string m_strOldParentDomainName;

    /*������ӻ�ɾ���¼���*/
    UUIDString m_tSubDomainId;
	TKDSIP_DIALOG_ID m_tDelChildTasDlgId;
	bool m_bDelChildTasHasRsped;
};

#endif // _TAS_CONFIG_TASK_H_
