#ifndef _OSP_SIP_EVT_POLL_H_
#define _OSP_SIP_EVT_POLL_H_

#include "kdvtype.h"

extern bool g_bSipPollRun;
void* SipEventPollThread(void* pArg);

#endif  //#ifndef _OSP_SIP_EVT_POLL_H_