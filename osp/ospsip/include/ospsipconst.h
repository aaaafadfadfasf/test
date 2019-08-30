#ifndef _OSP_SIP_CONST_H_
#define _OSP_SIP_CONST_H_

enum EOspSipInnerEvent
{    
    SIP_HB_TIMER = OSP_SIP_EV_END+1,
    SIP_HB_ADD_NODE,
    SIP_HB_DEL_NODE,
    SIP_HB_REQ,
    SIP_HB_RSP,

    SIP_REG_PROXY_REQ,
    SIP_REG_PROXY_RSP,
    SIP_REG_PROXY_TIMEOUT,
    SIP_REG_PROXY_UPDATE_TIMER,

    SIP_TRANS_CHECK_TIMER,
};

#define SIP_HB_INTERVAL            (10*1000) //ms
#define SIP_HB_COUNT               (3)

#define SIP_REG_TIMEOUT_INTERVAL   (5*1000)  //ms
#define SIP_REG_UPDATE_INTERVAL    (60)      //s
#define SIP_TRANS_CHECK_INTERVAL   (12*1000) //ms

#define OSP_SIP_APP_ID    (190)

#define SIP_HB_NODE_MAX   (1024)


#define OSP_SIP_DAEMON   (MAKEIID(OSP_SIP_APP_ID, CInstance::DAEMON))
#define OSP_SIP_PENDING  (MAKEIID(OSP_SIP_APP_ID, CInstance::PENDING))

#endif //#ifndef _OSP_SIP_CONST_H_