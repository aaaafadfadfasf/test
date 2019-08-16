#ifndef _IM_CORE_APP_H
#define _IM_CORE_APP_H

#include "../include/cms/ospext/ospext.h"
#include "../include/cms/ospext/ospinst.h"
#include "../include/cbb/osp/osp.h"
#include "../include/cms/ospext/osplog.h"

#include "democonf.h"

class CImCoreInst : public CInstExt
{
public:
	//实例状态
	enum { Idle = 0, RegProxy, Service };

public:
	CImCoreInst();
	virtual ~CImCoreInst();

	void ClearInst();

	//消息入口
	virtual void DaemonEntry(CMessage* const pcMsg);



};




#endif