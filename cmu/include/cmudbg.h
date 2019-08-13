/*========================================================================================
ģ����    ��cmu
�ļ���    ��cmudbg.h
����ļ�  ��ospdbg.h
ʵ�ֹ���  ��cmu���Դ�ӡ
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_DBG_H
#define _CMU_DBG_H

#include "cms/ospext/ospdbg.h"
#include "cms/cmu/cmu_struct.h"
#include "cmuconf.h"

#define   MAX_CMS_URI_LEN    256

struct TPrintDevParam
{
    TPrintDevParam()
    {
        memset(this, 0, sizeof(TPrintDevParam));
    }

    s8 m_achPuId[MAX_CMS_URI_LEN + 1];
    s8 m_achDomainName[MAX_CMS_URI_LEN+1];
    u8 m_byType;
    u8 m_byState;
};

struct TPrintSwitchParam
{
    TPrintSwitchParam()
    {
        memset(this, 0, sizeof(TPrintSwitchParam));
    }

    s8 m_achDevUri[MAX_CMS_URI_LEN+1];
    u8 m_byChn;
};

struct TPrintSsParam
{
    TPrintSsParam()
    {
        memset(this, 0, sizeof(TPrintSsParam));
    }

    s8 m_szLoginSess[MAX_CMS_URI_LEN+1];
    s8 m_szDevUri[MAX_CMS_URI_LEN+1];
};

struct TPrintDomainPuList
{
	TPrintDomainPuList()
	{
		byFlag = 0;
		memset(szDomainName, 0, sizeof(szDomainName));
	}

	u8 byFlag;
	s8 szDomainName[MAX_CMS_URI_LEN+1];
};

struct TDomainSyncParam
{
    TDomainSyncParam()
    {
        memset(szDomainName, 0, sizeof(szDomainName));
        dwSyncNumOnce = DEFAULT_REDIS_DOMAIN_SYNC_NUM_ONCE;
    }

    s8 szDomainName[MAX_CMS_URI_LEN+1];
    u32 dwSyncNumOnce;
};


//cmu��������
API void cmuhelp();

//cmu�汾��Ϣ
API void cmuver();

//cmu������Ϣ
API void cmucfg();

//redissdk��Ϣ
API void redissdk();

//cmu״̬��Ϣ
API void cmustat();

//��ʾlog������Ϣ
API void logpara();

//����log����
API void loglev(u8 byLev);

//����corelog����
API void corelog(u8 byLev);

//����puilog����
API void puilog(u8 byLev);

//����cuilog����
API void cuilog(u8 byLev);

//����pulog����
API void pulog(u8 byLev);

//����culog����
API void culog(u8 byLev);

//����3aslog����
API void taslog(u8 byLev);

//����uaslog����
API void uaslog(u8 byLev);

//����nmslog����
API void nmslog(u8 byLev);

//��������cmulog����
API void domainlog(u8 byLev);

//���ö���log����
API void sslog(u8 byLev);

//����vtdulog����
API void vtdulog(u8 byLev);

//���ý���log����
API void switchlog(u8 byLev);

//����redis��־����
API void redislog(u8 byLev);

//ͬʱ��������ģ��log����
API void logall(u8 byLev);

//�ر�����ģ��log����
API void logoff();

//�򿪵���ģʽ
API void debug(bool bVal);

//��ӡ��������
API void tasklist(LPCSTR szTaskType = NULL);

//��ӡָ��������Ϣ
API void task(u32 dwTaskNO);

//��������Ŵ�ӡָ������������񣬵�ͬ��pnst
API void taskson(u32 dwTaskNO);

//ɱ��ָ����ʬ�������ڹ��ϻָ����߲��ԣ�����ʹ�ö��ĵ����Ʊ����û��������
API void IWillKillZombieTask(u32 dwTaskNO);

//��������Ŵ�ӡָ�������������
API void pnst(u32 dwTaskNO);

//����sesskon�Ŵ�ӡָ�������������
API void psst(LPCSTR szSession);

//trace ָ������
API void tracet(u32 dwTaskNO);

//trace ��һ���½�������
API void tracent();

//���ߵ�pui�б�
API void puilist(LPCSTR szPuiType);

//pu��Ϣͳ��
API void sumpu(LPCSTR szDomainName);

//pu�б�
API void pulist(LPCSTR szDomainName, u8 byState);

//��ӡָ��pu��Ϣ
API void pu(LPCSTR szDevUri);

//domain��Ϣ�б�
API void domainlist();

//������ͬ������
API void syncparam(LPCSTR szDomainName, u32 dwSyncNumOnce);

//{{{{{{{{{{{{{{{ add by Royan 20140513 ���ܸ澯����
//1.0puid��2.0puUriӳ���
API void map12list();
//}}}}}}}}}}}}}}} add by Royan 20140513 ���ܸ澯����

//domain����
API void domain(LPCSTR szDomainName);

//���ӵ�cmu�б�
API void cmulist();

//���ӵ�vtdu�б�
API void vtdulist();

//���ߵ�cui�б�
API void cuilist();

//���ߵ�cu�б�
API void culist();

//���ߵ������б�
API void pelist();

//�����豸���������б�
API void sslist();

//�����豸���������б�
API void dsslist();

//redis���������б�
API void rsslist();

//�����豸�Ķ������б�
API void sserlist(LPCSTR szDevUri);

//SwitchNode�б�,DevChn��ʽ��"7d53960277f341858f00b043fb8636a3@china_0"
API void switchlist(const char* szDevChn=NULL);

API void pusl(const char* szDevChn);

API void cusl(const char* szSession);

//add domain table
API void adt(LPCSTR szPeerDomainName, LPCSTR szPeerIP, LPCSTR szLocalIP, LPCSTR szOpType);
//print domain table
API void pdt();
//set display area info
API void area(u8 byCfg);

//nms���
//��ӡ�����澯�б�
API void nmsdalist();

//��ӡ���ظ澯�б�
API void nmsoalist();

//Natagent����
API void nahelp();

//Natagent�����������
API void nacmd(u16 wCmdNum);

//�˳�cmu
API void cmuquit();

API void SetMemoryReleaseRate(int nRange);
API void GetMemoryReleaseRate();
API void ReleaseFreeMemory();
API void SetMemroyReleaseTimer(int nSeconds);
//��ʼ��cmu����Ϣ����
void InitCmuMsgDesc();

//��ʼ��cmu�ڸ�ģ����־����
void InitCmuModuleLog();

#ifdef _LINUX_
void RegCmuCmd();
#endif


#endif  //#ifndef _CMU_DBG_H