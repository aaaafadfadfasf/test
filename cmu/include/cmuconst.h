/*========================================================================================
模块名    ：cmu
文件名    ：cmucost.h
相关文件  ：ospext.h
实现功能  ：cmu常量定义
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_CONST_H
#define _CMU_CONST_H

#include "cms/ospext/ospext.h"
#include "cms/cms_const.h"

#define CMU_CORE_APP_ID   138
#define CMU_CORE_DAEMON   (MAKEIID(CMU_CORE_APP_ID, CInstance::DAEMON))

//实际osp只能达到30000，在把pu注册cmu超时重试时间从10改成60后，30000勉强可以处理
//10000个设备的同时上下线并发操作(峰值消息堆积约25000）。 2019/3/15 zhusenyong
#define CMU_CORE_APP_QUEUE_SIZE   100000

enum ECmuInnerEvent
{
    CMU_INNER_EVENT_BEGIN = USER_INNER_MSG_BEGIN,

    //打印消息, cmu所有的打印消息在CMU_PRINT_EVENT_BEGIN和CMU_PRINT_EVENT_END之间添加
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

    //内部业务消息
    CMU_START,
    CMU_QUIT,
    CMU_CMU_DISCONNECT,       //连接成功后断链
    CMU_CMU_DELETE,           //cmu所在域被删除
    CMU_CMU_CONNECT_SUCCESS,  //连接成功通知
    CMU_CMU_CONNECT_FAIL,     //连接失败通知，对端cmu不在线，proxy直接应答404，此时需要立即给cu应答错误，否则cu可能卡死
    CMU_PU_STATUS_UPDATE,
    CMU_SWITCH_FINISH_CB,     //消息体：u32(回调触发者的TaskNO)
    CMU_DEV_ONLINE_NTF,      //消息体：TDevOnOffCBMsg
    CMU_DEV_OFFLINE_NTF,     //消息体：TDevOnOffCBMsg

    // redis相关内部消息
    CMU_FASTDB_SUB_CB,       // 消息体：TRedisSubCB
    CMU_REDISSDK_DISC_CB,       // 消息体：TRedisSdkDiscCB
    CMU_REDISSDK_SUB_CB,       // 消息体：TRedisSdkSubCB
    CMU_DOMAIN_REDIS_SYNC,     //消息体：u32(同步Task的TaskNO)
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

#define CMU_GET_TAS_DEV_TIMEOUT       (120)       //s,两分钟

#define CMU_TAS_DEV_NTF_TIMEOUT       CMS_MSG_TIMEOUT        //s

#define CMU_GET_TAS_CAP_ALIAS_TIMEOUT (120)       //s,两分钟

#define CMU_TAS_CAP_ALIAS_NTF_TIMEOUT (120)       //s,两分钟

#define CMU_GET_TAS_DOMAIN_TIMEOUT    (60)       //s,一分钟

#define CMU_PU_CONFIG_NTF_TIMEOUT     g_cCmuConf.m_nMaxPuRegTime //s,一分钟，NVR含有128通道时，配置获取时间大于之前定义的35s

#define CMU_REQ_RSP_TASK_TIMEOUT      CMS_MSG_TIMEOUT         //s

#define CMU_CONNECT_CMU_TIMEOUT       CMS_MSG_TIMEOUT         //s

#define CMU_SS_PU_STATUS_TIMEOUT      CMS_MSG_TIMEOUT         //s

#define CMU_SWITCH_TIMEOUT            CMS_MSG_TIMEOUT         //s

#define NTF_TRY_SEND_NUM              (1)         //3次

#define NTF_BUF_SIZE_LIMIT            NTF_BUF_MAX_SIZE   //25K

#define CMU_SWITCH_NEED_DLG_NUM      (4)   //一次完整交换最少需要4个InviteDlg资源
#define CMU_DST_SWITCH_NEED_DLG_NUM  (2)   //一次目的交换最少需要2个InviteDlg资源

#define VSIP_ENTITY_G100_SUPPORT_VIDEOINPUTNAME_SYNC             101  //用以标识g100是否支持视频源名称同步

#endif  //#ifndef _CMU_CONST_H