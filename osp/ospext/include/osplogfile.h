/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��osplogfile.h
����ļ�  ��osp.h osptool.h
ʵ�ֹ���  ��д��־�ļ�
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/03/01         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _OSP_LOG_FILE_H_INCLUDED_
#define _OSP_LOG_FILE_H_INCLUDED_

#include "cbb/osp/osp.h"
#include "cms/ospext/osptool.h"
#include <string>
using std::string;

#define DEFAULT_LOG_DIR_NAME       ("./cmslog/")
#define DEFAULT_LOG_FILE_SIZE_kb   (1024)   //1024*1kb = 1M
#define DEFAULT_LOG_FILE_NUM       (10)
#define DEFAULT_LOG_SUFFIX         (".log")

enum EOspExtLogFileType
{
    LOG_FILE_NONE = 0,      //������־
    LOG_FILE_ERR = 1,       //������־
    LOG_FILE_RUN = 1<<1,    //������־
    //LOG_FILE_XXX = 1<<2,        //XXXX��־

};

/*====================================================================
������      : OspExtOpenLogFile
����        : ����ospext��־�ļ�����
�㷨ʵ��    : ��־���Է�����
����ȫ�ֱ���:
�������˵��:
             strLogDir: ��־�ļ������Ŀ¼����ʽ: "./log/" �� "/opt/kdm/cmu/log/"
             nLogSizeMax_kb: ��־�ļ�����������λ��kb
             nLogNumMax: ��־�ļ��������Ŀ
             dwLogType: ��־�ļ�����: LOG_FILE_ERR �� LOG_FILE_RUN
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/03/01  0.1         fanxg
====================================================================*/
bool OspExtOpenLogFile(const string& strLogDir, u32 nLogSizeMax_kb, u32 nLogNumMax, u32 dwLogType);

class CXLogFile
{
public:
    CXLogFile();
    ~CXLogFile();

private:
    CXLogFile(const CXLogFile&);  
    void operator=(const CXLogFile&);
public: 
    //��0.1.2.3.4.5...�ķ�ʽΪ��־�ļ�����
    void WriteLogFile(const string& strLog);

    //����־����ʱ��Ϊ��־�ļ�����
    void WriteLogFileNew(const string& strLog);

    //������־�ļ�����
    void SetLogFileParam(const string& strDir, u32 nLogSizeMax_kb, u32 nLogNumMax, u32 dwLogType);

    //��ȡ�������͵�ǰʱ����ɵ��ļ���
    static string GetFileStamp();

    //���ļ�����ȡ��׺��������û��
    static string GetSuffixFromFile(const string& strFileName);

private:
    string GetCurFile() const;
    string GetFirstFile() const;
    string GetLastFile() const;
    u32 GetFileNum() const;
    string GetNewFile() const;
    string GetSuffixName() const;
public:
	u32 GetLogSizeMax() const { return m_nLogSizeMax; }
	void SetLogSizeMax(u32 val) { m_nLogSizeMax = val; }
	u32 GetLogNumMax() const { return m_nLogNumMax; }
	void SetLogNumMax(u32 val) { m_nLogNumMax = val; }
private:
    u32 m_dwLogType;     //��־����
    string m_strLogDir;  //��־Ŀ¼
    string m_strLogName; //��־�ļ���ʼ����
    u32 m_nLogSizeMax;   //��־�ļ����ֵ����λ��kb
	u32 m_nLogNumMax;    //��־�ļ���Ŀ
	u32 m_nCurLogIdx;    //��ǰ��־�ļ�����
    string m_strCurFile; //��ǰ��д�ļ�
    FILE* m_pFile;       //��ǰ�򿪵��ļ�����������������ļ�һֱ�򿪣�����ÿ��д��־�����ļ��͹��ļ�
    CXSemLock m_cLock;   //���ܶ��߳�д�ļ�����������
};

struct TLogData
{
    TLogData()
    {
        dwLogType = 0;
    }

    string strLog;
    u32    dwLogType;
};

//д��־�ļ��ӿ�
void WriteLogFile(TLogData* ptLogData);

//��ʾ��־������Ϣ
void LogQuePrint();

//д��־�߳�
void* WriteLogFileThread(void* pArg);



#endif //#ifndef _OSP_LOG_FILE_H_INCLUDED_
