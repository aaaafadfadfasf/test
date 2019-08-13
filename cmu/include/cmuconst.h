/*========================================================================================
ģ����    ��cmu
�ļ���    ��cmucost.h
����ļ�  ��ospext.h
ʵ�ֹ���  ��cmu��������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_CONST_H
#define _CMU_CONST_H

#include "cms/ospext/ospext.h"
#include "cms/cms_const.h"

#define CMU_CORE_APP_ID   138
#define CMU_CORE_DAEMON   (MAKEIID(CMU_CORE_APP_ID, CInstance::DAEMON))

//ʵ��ospֻ�ܴﵽ30000���ڰ�puע��cmu��ʱ����ʱ���10�ĳ�60��30000��ǿ���Դ���
//10000���豸��ͬʱ�����߲�������(��ֵ��Ϣ�ѻ�Լ25000���� 2019/3/15 zhusenyong
#define CMU_CORE_APP_QUEUE_SIZE   100000

enum ECmuInnerEvent
{
    CMU_INNER_EVENT_BEGIN = USER_INNER_MSG_BEGIN,

    //��ӡ��Ϣ, cmu���еĴ�ӡ��Ϣ��CMU_PRINT_EVENT_BEGIN��CMU_PRINT_EVENT_END֮�����
    CMU_PRINT_EVENT_BEGIN,

    CMU_PRINT_CONF,
    CMU_PRINT_REDISSDK,
    CMU_PRINT_STAT,
    CMU_PRINT_PUI_LIST,
	CMU_PRINT_SUM_PU,
    CMU_PRINT_DEV_LIST,
    CMU_PRINT_PU_INFO,
    CMU_PRINT_DOMAIN_LIST,
    CMU_PRINT_DOMAIN_SYNC_PARAM,
	CMU_PRINT_PUID_URI_MAP_LIST,
    CMU_PRINT_DOMAIN_DATA,
    CMU_PRINT_CUI_LIST,
    CMU_PRINT_CU_LIST,
    CMU_PRINT_VTDU_LIST,
    CMU_PRINT_CMU_LIST,
    CMU_PRINT_PE_LIST,
    CMU_PRINT_SS_LIST,
    CMU_PRINT_DSS_LIST,
    CMU_PRINT_RSS_LIST,
    CMU_PRINT_SSER_LIST,
    CMU_PRINT_SWITCH_NODE_LIST,
    CMU_PRINT_NMS_DIS_ALARM_LIST,
    CMU_PRINT_NMS_OVERLOAD_ALARM_LIST,
    CMU_PRINT_SON_TASK_LIST_BY_NO,
    CMU_PRINT_SON_TASK_LIST_BY_SESS,
	CMU_PRINT_CERTAIN_PU_SWITCH_INFO,
    CMU_PRINT_CERTAIN_CU_SWITCH_INFO,

    CMU_PRINT_EVENT_END,

    //�ڲ�ҵ����Ϣ
    CMU_START,
    CMU_QUIT,
    CMU_CMU_DISCONNECT,       //���ӳɹ������
    CMU_CMU_DELETE,           //cmu������ɾ��
    CMU_CMU_CONNECT_SUCCESS,  //���ӳɹ�֪ͨ
    CMU_CMU_CONNECT_FAIL,     //����ʧ��֪ͨ���Զ�cmu�����ߣ�proxyֱ��Ӧ��404����ʱ��Ҫ������cuӦ����󣬷���cu���ܿ���
    CMU_PU_STATUS_UPDATE,
    CMU_SWITCH_FINISH_CB,     //��Ϣ�壺u32(�ص������ߵ�TaskNO)
    CMU_DEV_ONLINE_NTF,      //��Ϣ�壺TDevOnOffCBMsg
    CMU_DEV_OFFLINE_NTF,     //��Ϣ�壺TDevOnOffCBMsg

    // redis����ڲ���Ϣ
    CMU_FASTDB_SUB_CB,       // ��Ϣ�壺TRedisSubCB
    CMU_REDISSDK_DISC_CB,       // ��Ϣ�壺TRedisSdkDiscCB
    CMU_REDISSDK_SUB_CB,       // ��Ϣ�壺TRedisSdkSubCB
    CMU_DOMAIN_REDIS_SYNC,     //��Ϣ�壺u32(ͬ��Task��TaskNO)
    CMU_SLIDING_WINDOW_PROC,

    CMU_INNER_EVENT_END,
};

enum ECmuModule
{
    CMU_MOD_START = USER_MODULE_START,

    CMU_MOD_INS,
    CMU_MOD_CUI,
    CMU_MOD_CU,
    CMU_MOD_PUI,
    CMU_MOD_PU,
    CMU_MOD_BATCH,
    CMU_MOD_3AS,
    CMU_MOD_UAS,
    CMU_MOD_CMU,
    CMU_MOD_VTDU,
    CMU_MOD_NMS,
    CMU_MOD_SWITCH,
    CMU_MOD_SS,
    CMU_MOD_REDIS,

    CMU_MOD_END,
};

#define CMU_REG_TAS_TIMEOUT           (20)        //s

#define CMU_GET_TAS_DEV_TIMEOUT       (120)       //s,������

#define CMU_TAS_DEV_NTF_TIMEOUT       CMS_MSG_TIMEOUT        //s

#define CMU_GET_TAS_CAP_ALIAS_TIMEOUT (120)       //s,������

#define CMU_TAS_CAP_ALIAS_NTF_TIMEOUT (120)       //s,������

#define CMU_GET_TAS_DOMAIN_TIMEOUT    (60)       //s,һ����

#define CMU_PU_CONFIG_NTF_TIMEOUT     g_cCmuConf.m_nMaxPuRegTime //s,һ���ӣ�NVR����128ͨ��ʱ�����û�ȡʱ�����֮ǰ�����35s

#define CMU_REQ_RSP_TASK_TIMEOUT      CMS_MSG_TIMEOUT         //s

#define CMU_CONNECT_CMU_TIMEOUT       CMS_MSG_TIMEOUT         //s

#define CMU_SS_PU_STATUS_TIMEOUT      CMS_MSG_TIMEOUT         //s

#define CMU_SWITCH_TIMEOUT            CMS_MSG_TIMEOUT         //s

#define NTF_TRY_SEND_NUM              (1)         //3��

#define NTF_BUF_SIZE_LIMIT            NTF_BUF_MAX_SIZE   //25K

#define CMU_SWITCH_NEED_DLG_NUM      (4)   //һ����������������Ҫ4��InviteDlg��Դ
#define CMU_DST_SWITCH_NEED_DLG_NUM  (2)   //һ��Ŀ�Ľ���������Ҫ2��InviteDlg��Դ

#define VSIP_ENTITY_G100_SUPPORT_VIDEOINPUTNAME_SYNC             101  //���Ա�ʶg100�Ƿ�֧����ƵԴ����ͬ��

#endif  //#ifndef _CMU_CONST_H