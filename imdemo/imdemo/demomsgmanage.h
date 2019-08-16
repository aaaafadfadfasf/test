#ifndef _DEMO_MSG_MANAGE_H
#define	_DEMO_MSG_MANAGE_H

#include "../include/cms/cms_proto.h"
#include "../include/cms/ospext/osptool.h"
#include "democonf.h"

class CDemoMsgReq : public CEventReq
{
public:
	CDemoMsgReq()
	{
		event = OspExtEventDesc(IM_REG_REQ);
	}

public:
	string& GetDevUri()
	{
		return devUri;
	}

private:
	string devUri;
};

class CDemoMsgRsp : public CEventRsp
{
public:
	CDemoMsgRsp()
	{
		event = OspExtEventDesc(IM_REG_RSP);
	}

};


#endif
