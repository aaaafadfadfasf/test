/*
其它tas域名变更处理任务
*/
#ifndef _OTHER_TAS_DOMAINNAME_CHANGENTF_H_
#define _OTHER_TAS_DOMAINNAME_CHANGENTF_H_

#include "inner_common.h"

class COtherTasDomainnameChangeNtf: public CSipTask
{
public:
    enum
    {
        enStateServer = UNKNOWN_STATE + 1,
    };

    COtherTasDomainnameChangeNtf(CInstExt *pcInst): CSipTask(pcInst){}
    ~COtherTasDomainnameChangeNtf(){}

    virtual const char* GetObjName() const{return "COtherTasDomainnameChangeNtf";}
    virtual const char* GetStateName(u32 dwState) const{return "ServerState";}
    virtual void InitStateMachine();

private:
    u32 OnServer(CMessage *const pcMsg);
};

#endif // _OTHER_TAS_DOMAINNAME_CHANGENTF_H_
