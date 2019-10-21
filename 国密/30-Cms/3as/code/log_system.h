#ifndef _LOG_SYSTEM_H_
#define _LOG_SYSTEM_H_
#include "osp.h"
#include <string>
using std::string;
#include "ospext/osplog.h"

/* ��־��ϵͳ */
enum ENLogSrc  // ��־��Դö��
{
    enLogSrcAll = MOD_ALL, 
    enLogSrcSystem = USER_MODULE_START+1, // ϵͳ����
    enLogSrcNMSConn,    // nmsģ��
    enLogSrcDbTaskPool, // DbTaskPoolģ��
    enLogSrcCCtrl,      // CCtrlģ��
    enLogSrcBackground, // Backgroundģ��
    enLogSrcLicense,    // licenseģ��
    enLogSrcSyncAnalyzer// SyncAnalyzerģ��
};

API void logpara();
API void logall(u32 dwLogLevel); // ��������ģ�����־����
API void logoff();               // �ر�����ģ�����־����

API void taslog(ENLogSrc enSrc, u32 dwLogLevel); // ����ģ����־����

API void tla(); // ���������־����

API void tle(); // ���ô�����־����

API void cctrllog(u32 dwLevel);
API void dblog(u32 dwLevel);
API void nmslog(u32 dwLevel);
API void licenselog(u32 dwLevel);

#endif // _LOG_SYSTEM_H_
