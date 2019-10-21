#ifndef _DBIX_FACE_H
#define _DBIX_FACE_H

#include "cms/ospext/osptask.h"
#include "cms/dbix/dbix.h"
#include "cms/dbix/dbresult_callback_imp4osptask.h"

#define  SECURITY_DBIX_PROC_TIMEOUT	 (10)	//s

#define VKEK_INFO_TABLE "VkekInfo"

#define VKIT_MODULEID  "ModuleId"
#define VKIT_PUID      "PuId"
#define VKIT_CHANNELID "ChnId"
#define VKIT_TMFROM    "TmFrom"
#define VKIT_VKEKS     "Vkeks"

#define CRTIT_INFO_TABLE "CrtInfo"   //Crt:证书

#define CRTIT_TYPE     "Type"
#define CRTIT_OWNERID  "OwnerId"
#define CRTIT_SIGN     "Sign"       // 签名证书
#define CRTIT_ENCRY    "Encry"      // 加密证书


class CVkekInfoTable: public CDBEntitySchema
{
public:
	CVkekInfoTable();
};

class CCertificateInfoTable :public CDBEntitySchema
{
public:
	CCertificateInfoTable();
};


extern CVkekInfoTable g_cVkekInfoTable;
extern CCertificateInfoTable g_cCertificateInfoTable;












#endif