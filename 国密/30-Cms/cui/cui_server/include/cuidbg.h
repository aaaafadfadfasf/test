/*****************************************************************************
   ģ����      : cui_server
   �ļ���      : cuidbg.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cuidbg.h ʵ��CUI��������
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/7   1.0         liangli        ����
******************************************************************************/

#ifndef _CUI_DEBUG_H_
#define _CUI_DEBUG_H_

#include "cms/ospext/ospext.h"
#include "cms/cms_proto.h"

#define CUI_VER FORMAT_VER("cui")

enum ECuiMod
{
    CUI_APP = USER_MODULE_START,
    CUI_DATA,
    LOGIN_TASK,
    NORMAL_TASK,
    TCP_THREAD,
	REDIS_LOG
};

enum ECuiCmd
{
    CUI_INNER_COMMAND_BEGIN = USER_INNER_MSG_BEGIN + 1000,
    CUI_QUIT,
    CUI_REDISSDK_DISC_CB,
    CUI_REDISSDK_SUB_CB,
    CUI_REDISSDK_MODULE_START,
    CUI_REDISSDK_MODULE_STOP,
    CUI_REDISSDK_MODULE_RESTART,
    CUI_REDISSDK_MODULE_UNKNOWN,

    CUI_PRINT_CU_LIST,
    CUI_PRINT_TAS_LIST,
    CUI_PRINT_DOMAIN_LIST,
    CUI_PRINT_CONFIG,
    CUI_PRINT_STATUS,
    CUI_PRINT_TASK_MAP,
    CUI_PRINT_MAIL_SIZE,
    CUI_PRINT_PE_AGENT,
    CUI_PRINT_USER_DEVINFO,
    CUI_PRINT_PUDATA,
    CUI_PRINT_CUREQ,         // ��ӡCU��������Ϣ����Ϣ��Ϊ�Ự�ַ����� ���ݷǿ�ʱ��ʾ��ӡ����ֻ��ӡָ���Ự�ģ������ʾ����ӡ
    CUI_EMULATE_SIPMSG,      // ģ��SIP��Ϣ�� ��Ϣ��ΪTCuiEmulateSipMsg
	CUI_INNER_COMMAND_END,
	CUI_PRINT_VS_STATUS_LIST, // ��ӡ��ƵԴ״̬�б�
	CUI_SET_CU_NTF_MAX_SIZE,  // CU֪ͨ��Ϣ�����������
	CUI_PRINT_PTZ_LOCK_DEV_VS_LIST,	// ��ӡ����ptz����ƵԴ
	CUI_PRINT_CU_BAT_OPT_LIST, // ��ӡ��������������б�
};

struct TCuiEmulateSipMsg
{
    TCuiEmulateSipMsg()
    {
        szMsgBody[0] = '\0';
    }
    u32 dwDstTaskno;
    s32 nSipErrorno;
    s32 nEvent;
    s8 szMsgBody[10240];
};

struct TUserDevData
{
	TUserDevData()
	{
		memset(szUserId, 0, sizeof(s8) * 33);
		memset(szUserId, 0, sizeof(s8) * 33);
	}
	s8 szUserId[33];
	s8 szDevUUid[33];
};

void CuiRegHelpCmd();

API void cuiquit(int nSignal = 0);

API void cuihelp();

API void cuiver();

API void logpara();

API void logall(u8 bLogAll = 0);
void liblog(u8 byLog);

API void logoff();

API void cuilogvc(u32 bLogErr = 0);

API void cuilogerr();

API void cuilogtask(u32 bLogErr = 0);

//��ӡ��������
API void tasklist(LPCSTR szTaskType = NULL);

//��ӡָ��������Ϣ
API void task(u32 dwTaskNO);

//��������Ŵ�ӡָ������������񣬵�ͬ��pnst
API void taskson(u32 dwTaskNO);

//ɱ��ָ����ʬ�������ڹ��ϻָ����߲��ԣ�����ʹ�ö��ĵ����Ʊ����û��������
API void IWillKillZombieTask(u32 dwTaskNO);

//trace ָ������
API void tracet(u32 dwTaskNO);

//trace ��һ���½�������
API void tracent();

API void culist(u32 bPrintSubTask = 0);

API void cuitaskshow();
API void ts();

API void taslist(u32 bPrintUserDevList = 0);

API void domain();

API void ds();

API void hb();

API void cuicfg();

API void logparam();

API void pt(u32 dwTaskNO);

API void tt(u32 dwTaskNO);

API void cuistat();

API void taskmap();

API void mailsize();

API void pe();

API void userdevinfo(LPCSTR szUserSess, LPCSTR szDevUri = NULL);

API void pudata(LPCSTR szDevUrl);

API void toptime();

API void settoptime(u32 dwNum);

API void pcq(LPCSTR szCuSession); // ��ӡcu��������Ϣ, �Ự��Ϊ��ʱ��ʾ����ӡ

API void emulate_sipmsg(u32 dwTaskNo, s32 nSipError, s32 nEvent, LPCSTR szEventContent = NULL);

API void vtdunatclear(LPCSTR szUserSess); //����û�NAT̽���

API void getvslist();

API void setcuntfsize(s32 nCuNtfMaxSize = 0);

API void getptzlock(LPCSTR szDevId); // ��ӡ�豸�±���������ƵԴ

API void getbatopt();

API void GetRedis();
#endif


