#ifndef CERTIFICATE_QUERY_TASK_H
#define CERTIFICATE_QUERY_TASK_H


#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospext.h"
#include "cms/ospext/osptask.h"
#include "cms/security_module/security_module_proto.h"
#include <string>  
#include <fstream>  
#include <streambuf> 
#include "cms/cms_errorcode.h"
#include "cms/utility/mischelper.h"
#include "cms/cms_errorcode.h"



class CCertificateQueryTask:public CSipTask
{
public:
	enum
	{
		SERVER = UNKNOWN_STATE + 1,
	};
public:
	CCertificateQueryTask(CInstExt *pcInst);
	~CCertificateQueryTask();
public:
	virtual const char* GetObjName() const
	{
		return "CCertificateQueryTask";
	}
	virtual const char* GetStateName(u32 dwState) const
	{
		switch(dwState)
		{
		case SERVER:
			return "SERVER";
		default:
			return "Unknow State";
		}
		return "Uknow State";
	}
	virtual void InitStateMachine();
public:
	u32 ProcServerMsg(CMessage *const pcMsg);

protected:
private:
};


#endif