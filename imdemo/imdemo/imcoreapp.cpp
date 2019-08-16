#include "imcoreapp.h"

void CImCoreInst::DaemonEntry(CMessage *const pcMsg)
{
	u8 byLogLev = EVENT_LEV;
	switch (pcMsg->event)
	{
	case IM_REG_CB:

		break;
	case IM_SUB_CB:

		break;
	case IM_NOTIFY_CB:

		break;
	case IM_INVITE_CB:

		break;
	case IM_MESSAGE_CB:

		break;
	case IM_BYE_CB:

		break;
	default:
		break;
	}

}