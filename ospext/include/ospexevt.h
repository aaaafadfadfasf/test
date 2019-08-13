/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��ospexevt.h
����ļ�  ��
ʵ�ֹ���  ��ospext�ڲ���Ϣ���壬�ϲ㲻��ֱ��ʹ��
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/04/19         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _OSP_EX_INNER_EVENT_H
#define _OSP_EX_INNER_EVENT_H

#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"

enum
{
    OSP_EXT_INNER_EVENT_BEGIN = OSP_EXT_INNER_MSG_BEGIN,

    //����ָ��APP-INST�Ĵ�ӡ����
    OSP_EXT_EVENT_SET_LOG_LEV,

    //����ָ��APP�Ĵ�ӡ����
    OSP_EXT_EVENT_SET_APP_LOG_LEV,

    //INST����ѯ����Ķ�ʱ��
    OSP_EXT_EVENT_POLL_TASK,

    //��ӡָ��APP��������Ϣ
    OSP_EXT_EVENT_PRINT_APP_TASK,

    //��ӡָ��APP-INST��������Ϣ
    OSP_EXT_EVENT_PRINT_INST_TASK,

    //��ӡָ��APP����ѯ������Ϣ
    OSP_EXT_EVENT_PRINT_APP_POLL_TASK,

    //��ӡָ��APP-INST����ѯ������Ϣ
    OSP_EXT_EVENT_PRINT_INST_POLL_TASK,

    //��ӡָ��APP-INST-TaskNO��������Ϣ
    OSP_EXT_EVENT_PRINT_ONE_TASK,

    //��ָֹ��APP-INST-TaskNO������
    OSP_EXT_EVENT_KILL_TASK,

    //TRACEָ��INST
    OSP_EXT_EVENT_TRACE_INST,

    //��ӡָ��AppId������Inst״̬
    OSP_EXT_EVENT_PRINT_INST_STATE,

    //OSPEXT��task�Ķ�ʱ��ID
    OSP_EXT_EVENT_TASK_TIMER_ID_BEGIN = OSP_EXT_INNER_EVENT_BEGIN + 500,  //50500
    OSP_EXT_EVENT_TASK_TIMER_ID_END = USER_INNER_MSG_BEGIN - 1,           //54999


    OSP_EXT_INNER_EVENT_END = USER_INNER_MSG_BEGIN - 1,

};

#endif //#ifndef _OSP_EX_INNER_EVENT_H