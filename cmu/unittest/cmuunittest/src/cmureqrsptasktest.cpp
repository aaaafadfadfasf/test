#include "cppunitlite/cppunitlite.h"
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cms/tas/tas_event.h"
#include "cms/tas/tas_struct.h"
#include "cms/tas/tas_proto.h"

#include "cmudata.h"
#include "cmucoreapp.h"
#include "puregtask.h"
#include "pureqrsptask.h"

CCmuCoreInst* GetCmuCoreInst()
{
    CApp* ptApp = (CApp*)(&g_cCmuCoreApp);
    CInstance* pInst = ptApp->GetInstance(CInstance::DAEMON);
    CCmuCoreInst* pCmuInst = dynamic_cast<CCmuCoreInst*>(pInst);
    return pCmuInst;
}

TEST_GROUP( CGetPuSysParamTask )
{

};


TEST( CGetPuSysParamTask, BasicFun )
{
    CCmuCoreInst* pCmuInst = GetCmuCoreInst();
    CGetPuSysParamTask cTask(pCmuInst);  

    CK_EQ( INVALID_TRANS_ID, cTask.GetSipTransID() );

    CK_STR_EQ("CPuReqRspTask", cTask.GetObjName());

    CK_STR_EQ("Req", cTask.GetStateName(CGetPuSysParamTask::Req));
    CK_STR_EQ("WaitRsp", cTask.GetStateName(CGetPuSysParamTask::WaitRsp));

    cTask.InitStateMachine();
    CTask::CStateProc cProc;
    CK_EQ( true, cTask.GetStateProc(CGetPuSysParamTask::Req, cProc) );
    CK_EQ( true, cTask.GetStateProc(CGetPuSysParamTask::WaitRsp, cProc) );
}


TEST( CGetPuSysParamTask, EventDrive )
{
    CCmuCoreInst* pCmuInst = GetCmuCoreInst();

    CGetPuSysParamTask* pTask = (CGetPuSysParamTask*)pCmuInst->CreateTask(PU_SYS_PARAM_GET_REQ);
    CK_NOT_EQ( NULL, pTask );
    CTask::CStateProc cProc;
    CK_EQ( true, pTask->GetStateProc(CGetPuSysParamTask::Req, cProc) );
    CK_EQ( true, pTask->GetStateProc(CGetPuSysParamTask::WaitRsp, cProc) );


    delete pTask;

}


