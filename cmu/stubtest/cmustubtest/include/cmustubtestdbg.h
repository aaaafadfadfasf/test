/*========================================================================================
ģ����    ��cmustubtest
�ļ���    ��cmustubtestdbg.h
����ļ�  ��ospdbg.h
ʵ�ֹ���  ��cmustubtest���Դ�ӡ
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_STUB_TEST_DBG_H
#define _CMU_STUB_TEST_DBG_H

#include "cms/ospext/ospdbg.h"

//cmu��������
API void cmuhelp();

//cmu�汾��Ϣ
API void cmuver();

//cmu������Ϣ
API void cmucfg();

//cmu״̬��Ϣ
API void cmustat();

//��ʾlog������Ϣ
API void logpara();

//����log����
API void loglev(u8 byLev);

//ͬʱ��������ģ��log����
API void logall(u8 byLev);

//�ر�����ģ��log����
API void logoff();

//��ӡ��������
API void tasklist(LPCSTR szTaskType = NULL);

//��ӡָ��������Ϣ
API void task(u32 dwTaskNO);

//��ӡ��������
API void pat();

//��ӡָ��������Ϣ
API void pt(u16 wTaskNO);

//trace ָ������
API void tracet(u16 wTaskNO);

//trace ��һ���½�������
API void tracent();

//���ߵ�pui�б�
API void puilist();

//���ߵ�pu�б�
API void pulist(u8 byFlag);

//���ӵ�cmu�б�
API void cmulist();

//���ߵ�cui�б�
API void cuilist();

//���ߵ�cu�б�
API void culist();

//cui��¼
API void cuion();

//cu��¼
API void cuon(u32 dwCuNum = 0);

//cui�˳�
API void cuioff();

//cu�˳�
API void cuoff(u32 dwTask);

//�����豸״̬
API void ssall(u32 dwInterver = 0);
API void ss(const char* szPuUri = NULL);

//ȡ�������豸״̬
API void unss(u32 dwTask);

//PTZ����
API void ptz(const char* szPuUri = NULL);

//ǰ��ͼƬץ�ġ�����
API void snap(const char* szPuUri = NULL);

//��ȡ��Ƶ�������
API void getvp(const char* szPuUri = NULL);

//������Ƶ�������
API void setvp(const char* szPuUri = NULL);

//��ȡǰ��ϵͳ����
API void getsp(const char* szPuUri = NULL);

//����ǰ��ϵͳ����
API void setsp(const char* szPuUri = NULL);

//��Ƶ���
API void invite(const char* szPuUri=NULL, s32 nPuChn=0, s32 nCuChn=0);
API void byeinvite(u32 dwTask = 0);

API void ivtnum(u32 dwIvtNum, u32 dwInterval = 0);
API void byenum(u32 dwbyeNum, u32 dwInterval = 0);

API void dumploglev(u32 dwLev);
API void dumptrace(u32 dwTaskNO, u32 dwMediaType = 0);
API void dumptraceport(u16 dwRecvPort);
API void dumpdelport(u16 dwRecvPort);
API void dumptraceclear();
API void dumpstat(u16 dwRecvPort);



//���ڵ���������
API void invite3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn);

//��ʼ����������
API void start3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn);

//ֹͣ����������
API void stop3pc(const char* szSrcUri, s32 nSrcChn, const char* szDstUri, s32 nDstChn);

//��¼als
API void onals();

//����ǰ�˸澯����
API void setal(const char* szPuUri = NULL);

//��ȡǰ�˸澯����
API void getal(const char* szPuUri = NULL);

//��ѯǰ�˸澯����
API void qryal(const char* szPuUri = NULL);


//�˳�cmu
API void cmuquit();

//��ʼ��cmu����Ϣ����
void InitCmuMsgDesc();

#ifdef _LINUX_
void RegCmuCmd();
#endif

//
API void naloglev(u32 dwLev);

void dumplog( u32 elevel, const char * fmt, ... );


#endif  //#ifndef _CMU_STUB_TEST_DBG_H