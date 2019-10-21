/*****************************************************************************
   ģ����      : security_module
   �ļ���      : security_module_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ��ȫģ����ⲿ��Ϣ����
   ����        : ��־��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2017/07/19  1.0         huzhiyun   ����
******************************************************************************/
#ifndef _SECURITY_MODULE_EVENT_H
#define _SECURITY_MODULE_EVENT_H
#include "security_module_proto_dependent.h"

enum
{
    SECURITY_MODULE_EXTERN_MSG_BEGIN=29001, // ע��Ϊ���ô�Э���ļ���С����������2.0�����С��ʹ�ã����˴�����ֱ��д����

    SECURITY_MODULE_NEW_VKEK_REQ, // ��Ϣ�� CSecurityModuleNewVkekReq  �����µ�vkek
    SECURITY_MODULE_NEW_VKEK_RSP, // ��Ϣ�� CSecurityModuleNewVkekRsp

    SECURITY_MODULE_VKEK_SAVE_REQ, // ��Ϣ�� CSecurityModuleVkekSaveReq  ����vkek
    SECURITY_MODULE_VKEK_SAVE_RSP, // ��Ϣ�� CSecurityModuleVkekSaveRsp
    SECURITY_MODULE_VKEK_QUERY_REQ, // ��Ϣ�� CSecurityModuleVkekQueryReq  ��ѯvkek
    SECURITY_MODULE_VKEK_QUERY_RSP, // ��Ϣ�� CSecurityModuleVkekQueryRsp
	SECURITY_MODULE_NVKEK_QUERY_REQ, // ��Ϣ�� CSecurityModuleNVkekQueryReq  ��ѯ�����n��vkek
	SECURITY_MODULE_NVKEK_QUERY_RSP, // ��Ϣ�� CSecurityModuleNVkekQueryRsp

    SECURITY_MODULE_CERTIFICATE_QUERY_REQ, // ��Ϣ�� CSecurityModuleCertificateQueryReq ��ѯ֤�� ����ǩ��֤��ͼ���֤��
    SECURITY_MODULE_CERTIFICATE_QUERY_RSP, // ��Ϣ�� CSecurityModuleCertificateQueryRsp

	SECURITY_MODULE_CERTIFICATE_SAVE_REQ,  // ��Ϣ�� CSecurityModuleCertificateSaveReq ����֤�� ����֤����Կ����ֵ���Ӵ�ֵ
	SECURITY_MODULE_CERTIFICATE_SAVE_RSP,  // ��Ϣ�� CSecurityModuleCertificateSaveRsp

	SECURITY_MODULE_CERTIFICATE_GET_REQ,   //  ��Ϣ�� CSecurityModuleCertificateGetReq  ��ȡ֤�� ����֤����Կ����ֵ���Ӵ�ֵ
	SECURITY_MODULE_CERTIFICATE_GET_RSP,    //  ��Ϣ�� CSecurityModuleCertificateGetRsp
};


inline void InitSecurityModuleEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(SECURITY_MODULE_EXTERN_MSG_BEGIN);        // ע��Ϊ���ô�Э���ļ���С����������2.0�����С��ʹ�ã����˴�����ֱ��д����
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_NEW_VKEK_REQ);            // ��Ϣ�� CSecurityModuleNewVkekReq  �����µ�vkek
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_NEW_VKEK_RSP);            // ��Ϣ�� CSecurityModuleNewVkekRsp
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_VKEK_SAVE_REQ);           // ��Ϣ�� CSecurityModuleVkekSaveReq  ����vkek
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_VKEK_SAVE_RSP);           // ��Ϣ�� CSecurityModuleVkekSaveRsp
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_VKEK_QUERY_REQ);          // ��Ϣ�� CSecurityModuleVkekQueryReq  ��ѯvkek
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_VKEK_QUERY_RSP);          // ��Ϣ�� CSecurityModuleVkekQueryRsp
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_NVKEK_QUERY_REQ);         // ��Ϣ�� CSecurityModuleNVkekQueryReq  ��ѯ�����n��vkek
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_NVKEK_QUERY_RSP);         // ��Ϣ�� CSecurityModuleNVkekQueryRsp
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_QUERY_REQ);   // ��Ϣ�� CSecurityModuleCertificateQueryReq ��ѯ֤�� ����ǩ��֤��ͼ���֤��
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_QUERY_RSP);   // ��Ϣ�� CSecurityModuleCertificateQueryRsp
	OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_SAVE_REQ);    // ��Ϣ�� CSecurityModuleCertificateSaveReq ����֤�� ����ǩ��֤��ͼ���֤��
	OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_SAVE_RSP);    // ��Ϣ�� CSecurityModuleCertificateSaveRsp
	OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_GET_REQ);    // ��Ϣ�� CSecurityModuleCertificateGetReq ��ȡ֤�� ����֤����Կ����ֵ���Ӵ�ֵ
	OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_GET_RSP);    // ��Ϣ�� CSecurityModuleCertificateGetRsp
}

#endif // _SECURITY_MODULE_EVENT_H





