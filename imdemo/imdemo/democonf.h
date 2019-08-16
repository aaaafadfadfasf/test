#ifndef _DEMO_CONF_H
#define _DEMO_CONF_H

#include "../include/ospsip.h"
#include "../include/cms/commonconfig/cmcfg.h"
#include "../include/kdvtype.h"
#include "../include/kdvsys.h"

#define SECTION_IM              ( "IM" )
#define KEY_LOCAL_IP            ( "LocalIP" )         //����SIP��ַ
#define KEY_LOCAL_PORT          ( "LocalPort" )       //����SIP�˿�

#define MAX_READ_CONF_NUM       64
#define DEAFULT_LOCAL_PORT      9080

#define CONF_FILE_PATH          "./im.ini"

#define IM_CORE_APP_ID         156
#define IM_CORE_DAEMON         (MAKEIID(IM_CORE_APP_ID, CInstance::DAEMON))


#define EVENT_CODE_START       5000

enum EDemoEventCode
{
	IM_REG_REQ = EVENT_CODE_START,     //ע����Ϣ
	IM_REG_RSP,

	IM_SUB_REQ,						  //������Ϣ
	IM_SUB_RSP,

	IM_NOTIFY_REQ,					  //֪ͨ��Ϣ
	IM_NOTIFY_RSP,

	IM_INVITE_REQ,					  //����Ự��Ϣ
	IM_INVITE_RSP,
	IM_INVITE_ACK,

	IM_MESSAGE_REQ,					  //��Ϣת��
	IM_MESSAGE_RSP,

	IM_BYE_REQ,						  //�����Ự��Ϣ
	IM_BYE_RSP,
};

//����demo���̵Ĵ������
enum EDemoErrCode
{
	DEMO_SUCCESS = 0,			      //����һ���ɹ�����

	//���������
	ERR_SIP_BODY_EMPTY,
};

//������־��� server��1  client��2
enum EDemoModule
{
	DEMO_SERVER = 1,				  
	DEMO_CLIENT,
};
class CDemoConf
{
public:
	CDemoConf();
	~CDemoConf();

public:
	BOOL32 ReadDemoConf(const s8 *sDemoConfPath);
	void   PrintData();
public:
	TSipURI  m_tLocalUri;
	TSipURI  m_tProxyUri;
	TSipAddr m_tLocalAddr;
	TSipAddr m_tProxyAddr;

};

CDemoConf::CDemoConf()
{
}

CDemoConf::~CDemoConf()
{
}





#endif