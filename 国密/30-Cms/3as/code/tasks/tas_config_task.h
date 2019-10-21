/*
tas配置任务
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
        enStateConfigQuery,            // 获取tas配置
        enStateConfigDomainName,       // 域名配置
        enStateConfigParentDomain,     // 父域配置
        enStateConfigChildDomainAdd,   // 添加子域配置
        enStateConfigChildDomainDel,   // 删除子域配置
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
    /* 用于配置上级域 */
    UUIDString m_tParentDomainId;
    string m_strParentDomainName;
    UUIDString m_tOldParentDomainId;
    string m_strOldParentDomainName;

    /*用于添加或删除下级域*/
    UUIDString m_tSubDomainId;
	TKDSIP_DIALOG_ID m_tDelChildTasDlgId;
	bool m_bDelChildTasHasRsped;
};

#endif // _TAS_CONFIG_TASK_H_
