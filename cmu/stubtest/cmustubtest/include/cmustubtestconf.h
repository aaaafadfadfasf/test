/*========================================================================================
ģ����    ��cmustubtest
�ļ���    ��cmustubtestconf.h
����ļ�  ��kdvtype.h ospsip.h
ʵ�ֹ���  ��cmustubtest������Ϣ����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_STUB_TEST_CONF_H
#define _CMU_STUB_TEST_CONF_H

#include "kdvtype.h"
#include "cms/ospsip/ospsip.h"

#define cmu_config  (LPCSTR)"./conf/cmustubtest.ini"

#define SECTION_CMU             ( "CMU" )
#define KEY_LOCAL_IP            ( "LocalIP" )         //����SIP��ַ
#define KEY_LOCAL_PORT          ( "LocalPort" )       //����SIP�˿�

#define MAX_READ_BUF_LEN        (256)

class CCmuStubTestConf
{
public:
    CCmuStubTestConf()
    {
        m_nIsAutoTest = 0; 
        m_nNAQryAddrMode = 0;
    }
    BOOL32 ReadConf(const s8* szConfPath);
    void PrintData();
public:
    TSipURI  m_tLocalUri;
    TSipURI  m_tCmuUri;
    TSipURI  m_tProxyUri;
    TSipAddr m_tLocalAddr;
    TSipAddr m_tProxyAddr;
    int m_nIsAutoTest;
    int m_nNAQryAddrMode;
};

extern CCmuStubTestConf g_cCmuConf;

#endif  //#ifndef _CMU_STUB_TEST_CONF_H