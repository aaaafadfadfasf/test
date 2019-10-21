/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��ospdbg.h
����ļ�  ��osp.h
ʵ�ֹ���  ��ospext���Կ���
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/04/21         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _OSP_DBG_H_INCLUDED_
#define _OSP_DBG_H_INCLUDED_

#include "cbb/osp/osp.h"
#include <string>
using std::string;

#define OSP_EXT_VER "ospext 1.0 Compile Time: %s, %s\r\n", __DATE__, __TIME__

//������������
API void u();

//������������
API void d();

//��ӡ���л����������
API void a();

//ospex ��������
API void OspExtHelp();
API void ospexthelp();

//ospex �汾��Ϣ
API void OspExtVer();
API void ospextver();

//��ȡ��־������ַ�������
const string GetStrLogLev(u8 byLogLev);

//��ʾ��־���Ʋ���
API void OspShowLogParam();

//����ָ��AppId-InstId�Ĵ�ӡ����
API void OspSetLogLev(u8 byLogLev, u16 wAppId, u16 wInstId = 0);

//����ָ��ģ��Ĵ�ӡ����,���ʹ��ģ��ʽ��ӡ���ƣ����������趨ģ��Ĵ�ӡ����
API void OspSetModLogLev(u8 byLogMod, u8 byLogLev, LPCSTR szModName = NULL);
#define OSP_SET_MOD_LOG(LogMod, LogLev) OspSetModLogLev(LogMod, LogLev, #LogMod)

//����ospextģ����־����
API void ospextlog(u8 byLogLev);

//������־���ʱ�䣺//0:����ӡʱ��; 1:��ӡ�뼶ʱ��; 255:��ӡ���뼶ʱ��
API void OspSetLogTimeLev(u8 byLogLev);

//������־���������Ϣ(file-line-class-function)��//0:����ӡ; 1:��ӡ
API void OspLogCompileInfo(u8 byPrint);

//������־���OSPǰ׺(app-inst-task-state)��//0:����ӡ; 1:��ӡ
API void OspLogOspPrefix(u8 byPrint);

//����AppId-InstId��ӡ������Ϣ
API void OspPrintTask(u16 wAppId, u16 wInstId=0, LPCSTR szTaskType=NULL);

//����AppId-InstId��ӡ��ѯ������Ϣ
API void OspPrintPollTask(u16 wAppId, u16 wInstId=0, LPCSTR szTaskType=NULL);

//����AppId-InstId-TaskNO��ӡ������Ϣ
API void OspPrintOneTask(u32 dwTaskNO, u16 wAppId, u16 wInstId =0);

//����AppId-InstId-TaskNO��ָֹ������
API void OspKillTask(u32 dwTaskNO, u16 wAppId, u16 wInstId =0);

//traceָ��AppId-InstId�����ӡ
API void OspTraceInst(u16 wInstId, u16 wAppId);

//traceָ�����������ӡ
API void OspTraceTask(u32 dwTaskNO);

//trace��һ���½����������ӡ
API void OspTraceNextTask();

//��ӡָ��AppId������Inst״̬
API void OspInstState(u16 wAppId);

//�Ƿ����������־�ļ���//0:�����; ��0:���
API void OspWriteRunLog(u8 byIsWriteLog);

//�Ƿ����������־�ļ���//0:�����; ��0:���
API void OspWriteErrLog(u8 byIsWriteLog);

//���ô�����־�ļ�����1-255
API void OspSetErrLogFileNum(u32 dwErrLogFileNum);

//���ô�����־�ļ���С��1M-1024M
API void OspSetErrLogFileSize(u32 dwErrLogFileSize);

//����������־�ļ�����1-255
API void OspSetRunLogFileNum(u32 dwRunLogFileNum);

//����������־�ļ���С��1M-1024M
API void OspSetRunLogFileSize(u32 dwRunLogFileSize);

//��ʾ��־������Ϣ��
API void OspShowLogQue();

#ifdef _LINUX_
//linux�Ƿ����mangle name�Ľ���
API void OspSetParseMangleName(u8 byIsParse);
#endif

//�Ƿ���ʱ�����ܼ�⹦��
API void OspSetTimeMonitor(u8 byOpen);


#endif  //#ifndef _OSP_DBG_H_INCLUDED_
