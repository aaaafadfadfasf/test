
#include "cms/ospext/osplog.h"
#include "cms/commonconfig/cmcfg.h"
#include "kdvsys.h"
#include "cmustubtestconf.h"
#include "cmustubtestdata.h"

CCmuStubTestConf g_cCmuConf;

BOOL32 CCmuStubTestConf::ReadConf(const s8* szConfPath)
{
    if (szConfPath == NULL)
    {
        CLASSLOG(ERROR_LEV, "�����ļ�·��Ϊ��\n");
        return FALSE;
    }

    //�����豸�ļ���
//     if (!CreateDeviceMark())
//     {
//         CLASSLOG(ERROR_LEV, "�����豸�ļ���ʧ��\n");
//         return FALSE;
//     }


    //��ȡ���������ļ�
//     if (!ReadCommonConfig())
//     {
//         CLASSLOG(ERROR_LEV, "���������ļ���ʼ��ʧ��\n");
//         return FALSE;
//     }


    
    //�������ļ�
    s8 szStr[MAX_READ_BUF_LEN] = {0};
    int nPort;

    GetRegKeyString(szConfPath, SECTION_CMU, "LocalUUID", "", szStr, MAX_READ_BUF_LEN);
    m_tLocalUri.SetUser(szStr);

    GetRegKeyString(szConfPath, SECTION_CMU, "DomainName", "", szStr, MAX_READ_BUF_LEN);

    m_tLocalUri.SetDomain(szStr);
    
    m_tCmuUri.SetDomain(szStr);
    m_tCmuUri.SetUser(UNIQUE_SIP_UA_CMU);
    
    m_tProxyUri.SetDomain(szStr);
    m_tProxyUri.SetUser(UNIQUE_SIP_UA_PROXY);
    
    GetRegKeyString(szConfPath, SECTION_CMU, KEY_LOCAL_IP, "", szStr, MAX_READ_BUF_LEN);
    m_tLocalAddr.tIpAddr = szStr;

    GetRegKeyInt(szConfPath, SECTION_CMU, KEY_LOCAL_PORT, CMU_SIP_UDP_PORT, &nPort);
    m_tLocalAddr.wPort = (u16)nPort;

    GetRegKeyString(szConfPath, SECTION_CMU, "ProxyIP", "", szStr, MAX_READ_BUF_LEN);
    m_tProxyAddr.tIpAddr = szStr;

    GetRegKeyInt(szConfPath, SECTION_CMU, "ProxyPort", PROXY_SIP_UDP_PORT, &nPort);
    m_tProxyAddr.wPort = (u16)nPort;

    GetRegKeyInt(szConfPath, SECTION_CMU, "AutoTest", 0, &nPort);
    m_nIsAutoTest = nPort;

    GetRegKeyInt(szConfPath, SECTION_CMU, "NAQryMode", 0, &nPort);
    m_nNAQryAddrMode = nPort;

    int nPuNum = 0;
    GetRegKeyInt(szConfPath, SECTION_CMU, "MtNum", 1, &nPuNum);

    int nMtType = 0;
    GetRegKeyInt(szConfPath, SECTION_CMU, "MtIdType", 1, &nMtType);

    if (nMtType == 0)
    {
        char   strRead[64+1] = {0};
        memset(strRead, 0 , sizeof(strRead));
        GetRegKeyString(szConfPath, SECTION_CMU, "mtStartId", "", strRead, sizeof(strRead)-1);
        if ( strRead == "" )
        {	
            OspPrintf(TRUE, FALSE, "Wrong profile while reading PUID!\n");
            return FALSE;
        }
        else
        {	
            s32	dwLoop = 0;
            s8 szAlias[10] = { 0 };
            u32 dwPuNum = 0;
            memcpy(szAlias, &strRead[27-5], 5);

            dwPuNum = atoi((char*)szAlias);

            for( dwLoop = 0; dwLoop < nPuNum; dwLoop++ )
            {
                sprintf(szAlias, "VMT%05d", dwPuNum);
                memcpy(&strRead[27-5], &szAlias[3], 5);

                g_pszTestPu.push_back(strRead);
                dwPuNum++;
            }
        }
    }
    else
    {      
        for (s32 dwLoop = 1; dwLoop<=nPuNum; dwLoop++)
        {
            char   strRead[64+1] = {0};
            s8 achKey[64] = {0};
            sprintf(achKey, "%s%d\0", "mt", dwLoop);
            GetRegKeyString(szConfPath, SECTION_CMU, achKey, "", strRead, sizeof(strRead));

            g_pszTestPu.push_back(strRead);
        }
    }
    

    return TRUE;
}


void CCmuStubTestConf::PrintData()
{
    OspPrintf(TRUE, FALSE, "\nCMU������Ϣ��\n");
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "  ����URI        [%s]\n", m_tLocalUri.GetURIString().c_str());
    OspPrintf(TRUE, FALSE, "  ���ص�ַ��Ϣ   [%s]\n", m_tLocalAddr.GetSipAddrStr().c_str());
    OspPrintf(TRUE, FALSE, "  PROXY��ַ��Ϣ  [%s]\n", m_tProxyAddr.GetSipAddrStr().c_str());
    OspPrintf(TRUE, FALSE, "  NA��ѯ��ַ��ʽ [%d]\n", m_nNAQryAddrMode);
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");  

    for (u32 dwLoop = 0; dwLoop<g_pszTestPu.size(); dwLoop++)
    {
        OspPrintf(TRUE, FALSE, "%u. puid:%s\n", dwLoop+1, g_pszTestPu[dwLoop].c_str());
    }

    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
}

