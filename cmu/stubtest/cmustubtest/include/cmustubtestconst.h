/*========================================================================================
ģ����    ��cmustubtest
�ļ���    ��cmustubtestcost.h
����ļ�  ��ospext.h
ʵ�ֹ���  ��cmustubtest��������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_STUB_TEST_CONST_H
#define _CMU_STUB_TEST_CONST_H

#include "cms/ospext/ospext.h"

#define CMU_STUB_TEST_CORE_APP_ID   138
#define CMU_STUB_TEST_CORE_DAEMON   (MAKEIID(CMU_STUB_TEST_CORE_APP_ID, CInstance::DAEMON))

enum ECmuStubTestInnerEvent
{
    CMU_INNER_EVENT_BEGIN = USER_INNER_MSG_BEGIN,

    //��ӡ��Ϣ, cmu���еĴ�ӡ��Ϣ��CMU_PRINT_EVENT_BEGIN��CMU_PRINT_EVENT_END֮�����
    CMU_PRINT_EVENT_BEGIN,

    CMU_PRINT_CONF,
    CMU_PRINT_STAT,
    CMU_PRINT_PUI_LIST,
    CMU_PRINT_PU_LIST,
    CMU_PRINT_CUI_LIST,
    CMU_PRINT_CU_LIST,
    CMU_PRINT_VTDU_LIST,
    CMU_PRINT_CMU_LIST,

    CMU_PRINT_EVENT_END,

    //�ڲ�ҵ����Ϣ
    CMU_START,
    CMU_PUI_DISCONNECT,
    CMU_CUI_DISCONNECT,
    CMU_CMU_DISCONNECT,
    CMU_CMU_CONNECT_SUCCESS,
    CMU_CMU_CONNECT_FAIL,
    CMU_PU_STATUS_UPDATE,
    CMU_STUN_QRY_ADDR_NTF,
    CMU_PUNCH_RESULT_NTF,

    //��ʱ����Ϣ
    CMU_TASK_TIMER_TEST,
    CMU_TASK_TIMER_UPDATE,
    CMU_TASK_TIMER_KILL,

};

enum ECmuModule
{
    MOD_CUI = USER_MODULE_START,
    MOD_CU,
    MOD_SS,
    MOD_PTZ,
    MOD_ALS,
    MOD_TVS,
    MOD_SWITCH,

};

enum
{
    EDumpMediaBoth = 0,
    EDumpMediaVideo,
    EDumpMediaAudio,
};

//DUMP��־��������
enum 
{
    DumpLog_None                  = 0,        //�ر����д�ӡ
    DumpLog_Error                 = 1,        //���󼶱�
    DumpLog_Warning               = 2,        //���漶��
    DumpLog_Info                  = 3,        //��Ϣ����ͨ������
    DumpLog_All                   = (u32)(~0),//�ſ����д�ӡ
};

#define CMU_REG_TAS_INTERVAL     (60*1000)   //ms

#define CMU_REQ_RSP_TASK_TIMEOUT  (3)        //s

#define CMU_CONNECT_CMU_TIMEOUT   (3)        //s

#define CMU_SS_PU_STATUS_TIMEOUT  (3)        //s

#define LOCAL_INVITE_START_PORT 50128


#endif  //#ifndef _CMU_STUB_TEST_CONST_H