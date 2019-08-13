/*========================================================================================
模块名    ：cmustubtest
文件名    ：cmustubtestconf.h
相关文件  ：kdvtype.h ospsip.h
实现功能  ：cmustubtest配置信息管理
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_STUB_TEST_CONF_H
#define _CMU_STUB_TEST_CONF_H

#include "kdvtype.h"
#include "cms/ospsip/ospsip.h"

#define cmu_config  (LPCSTR)"./conf/cmustubtest.ini"

#define SECTION_CMU             ( "CMU" )
#define KEY_LOCAL_IP            ( "LocalIP" )         //本地SIP地址
#define KEY_LOCAL_PORT          ( "LocalPort" )       //本地SIP端口

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