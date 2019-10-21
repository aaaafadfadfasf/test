#ifndef _OSP_EXT_H_
#define _OSP_EXT_H_

/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��ospext.h
����ļ�  ��osp.h ospexevt.h osplog.h osptool.h osptask.h ospinst.h ospapp.h
ʵ�ֹ���  ����չOSP�Ĺ���
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/04/21         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#include "cbb/osp/osp.h"
#include "cms/cms_eventid.h"
#include "cms/cms_const.h"
#include "cms/ospext/osplog.h"
#include "cms/ospext/ospdbg.h"
#include "cms/ospext/osptool.h"
#include "cms/ospext/osptask.h"
#include "cms/ospext/ospinst.h"
#include "cms/ospext/ospapp.h"

//�źŴ���ص�����
typedef void (*PfProcSignal)(int);

//��ȡkdsip��Ҫ��������
#define SECTION_OSPEXT         ( "OSPEXT" )            //OSPEXT������
#define KEY_WRITE_RUNLOG_LEV   ( "WriteRunLogLev" )    //���������־�ļ�������0:�����; ��0:���ָ������
#define KEY_RUNLOG_FILENUM     ( "RunLogFileNum" )     //������־�ļ�����1-255
#define KEY_RUNLOG_FILESIZE    ( "RunLogFileSize" )    //������־�ļ���С��1M-1024M
#define KEY_ERRLOG_FILENUM     ( "ErrLogFileNum" )     //������־�ļ�����1-255
#define KEY_ERRLOG_FILESIZE    ( "ErrLogFileSize" )    //������־�ļ���С��1M-1024M

#define MIN_LOG_FILE_NUM   (1)       //��С��־�ļ���1
#define MAX_LOG_FILE_NUM   (255)     //�����־�ļ���255
#define MIN_LOG_FILE_SIZE  (1)       //ÿ����־�ļ���СΪ1M
#define MAX_LOG_FILE_SIZE  (1024)    //ÿ����־�ļ����Ϊ1024M

//�ļ���־Ĭ�ϲ���
//runlog
#define RUN_LOG_DIR_NAME      "./runlog/"
#define RUN_LOG_FILE_SIZE_kb  (1*1024)     //ÿ��������־�ļ����Ϊ1M
#define RUN_LOG_FILE_NUM      (10)         //10�ļ�ѭ������

//errlog
#define ERR_LOG_DIR_NAME      "./errlog/"
#define ERR_LOG_FILE_SIZE_kb  (1*1024)    //ÿ��������־�ļ����Ϊ1M
#define ERR_LOG_FILE_NUM      (5)         //5�ļ�ѭ������

struct TOspExtConf 
{
    TOspExtConf()
    {
        dwMaxNodeNum = 32;  //���ó�1�ᵼ��osp�ڲ����������app(OspNodeMan)�̵߳���Ϣ����̫С�����׶���Ϣ�������ʵ��Ŵ�һ��
        dwMaxDispatchMsg = 1024;

        strRunLogDir = RUN_LOG_DIR_NAME;
        dwRunLogFileSize_kb = RUN_LOG_FILE_SIZE_kb;
        dwRunLogFileNum = RUN_LOG_FILE_NUM;

        strErrLogDir = ERR_LOG_DIR_NAME;
        dwErrLogFileSize_kb = ERR_LOG_FILE_SIZE_kb;
        dwErrLogFileNum = ERR_LOG_FILE_NUM;

		byRunLogFileLev=0;
    }

    string strModuleName;    //ģ��������������ʹ�ð���osp��dllʱ����ָ��dll��telnet���ڡ�Ϊ��ʱֱ��ȡ��������

    u32 dwMaxNodeNum;        //osp���������Node��, �����ʹ��ospͨ�ţ�����Ϊ1����
    u32 dwMaxDispatchMsg;    //osp�����Ϣ�ַ���,Ĭ��Ϊ1024

    string strRunLogDir;     //������־���Ŀ¼
    u32 dwRunLogFileSize_kb; //������־�ļ���С
    u32 dwRunLogFileNum;     //������־�ļ���Ŀ

    string strErrLogDir;     //������־���Ŀ¼
    u32 dwErrLogFileSize_kb; //������־�ļ���С
    u32 dwErrLogFileNum;     //������־�ļ���Ŀ

	// ����runlog��־���� [11/16/2015 pangubing]
	u8 byRunLogFileLev;

	/*pangubing  [11/16/2015 pangubing]
	  ��ȡkeda��ini��ʽ�������ļ�ֱ��ʹ�øýӿ�
	 ��ע�⣩������ʽ�������ļ����ϲ����ж����ȡ������������Ϣ��ȡ��TOspExtConf��  */
	bool ReadKeyCfg(const s8* szConfPath);

	
};



/*====================================================================
������      : OspExtInit
����        : OspExt��ʼ����
              1.osp��ʼ��
              2.ospext��־�ļ���ʼ��
              3.nipdebug��ʼ��
�㷨ʵ��    : ÿ������ֻ��Ҫ����һ��
����ȫ�ֱ���:
�������˵��:
             dwMaxNodeNum : [in] osp���������Node��, �����ʹ��ospͨ�ţ�����Ϊ1����
             dwMaxDispatchMsg : [in] osp�����Ϣ�ַ���
����ֵ˵��  : �ɹ�:TRUE��ʧ��:FALSE
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/02/20  0.1         fanxg
====================================================================*/
BOOL32 OspExtInit(const TOspExtConf& tOspExtConf = TOspExtConf());


/*====================================================================
������      : OspExtQuit
����        : OspExt�˳�
�㷨ʵ��    : ÿ������ֻ��Ҫ����һ��
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/02/20  0.1         fanxg
====================================================================*/
void OspExtQuit();

/*====================================================================
������      : OspExtSetSignalProcCB
����        : �����źŴ���ص�
�㷨ʵ��    : 
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/11/19  0.1         fanxg
====================================================================*/
void OspExtSetSignalProcCB(PfProcSignal pfSigProcCB);


#endif //#ifndef _OSP_EXT_H_