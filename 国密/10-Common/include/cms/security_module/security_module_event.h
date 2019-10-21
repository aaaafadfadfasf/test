/*****************************************************************************
   模块名      : security_module
   文件名      : security_module_event.h
   相关文件    : 
   文件实现功能: 安全模块的外部消息定义
   作者        : 胡志云
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2017/07/19  1.0         huzhiyun   创建
******************************************************************************/
#ifndef _SECURITY_MODULE_EVENT_H
#define _SECURITY_MODULE_EVENT_H
#include "security_module_proto_dependent.h"

enum
{
    SECURITY_MODULE_EXTERN_MSG_BEGIN=29001, // 注：为了让此协议文件最小依赖（便于2.0以外的小组使用），此处故意直接写整数

    SECURITY_MODULE_NEW_VKEK_REQ, // 消息体 CSecurityModuleNewVkekReq  生成新的vkek
    SECURITY_MODULE_NEW_VKEK_RSP, // 消息体 CSecurityModuleNewVkekRsp

    SECURITY_MODULE_VKEK_SAVE_REQ, // 消息体 CSecurityModuleVkekSaveReq  保存vkek
    SECURITY_MODULE_VKEK_SAVE_RSP, // 消息体 CSecurityModuleVkekSaveRsp
    SECURITY_MODULE_VKEK_QUERY_REQ, // 消息体 CSecurityModuleVkekQueryReq  查询vkek
    SECURITY_MODULE_VKEK_QUERY_RSP, // 消息体 CSecurityModuleVkekQueryRsp
	SECURITY_MODULE_NVKEK_QUERY_REQ, // 消息体 CSecurityModuleNVkekQueryReq  查询最近的n个vkek
	SECURITY_MODULE_NVKEK_QUERY_RSP, // 消息体 CSecurityModuleNVkekQueryRsp

    SECURITY_MODULE_CERTIFICATE_QUERY_REQ, // 消息体 CSecurityModuleCertificateQueryReq 查询证书 包括签名证书和加密证书
    SECURITY_MODULE_CERTIFICATE_QUERY_RSP, // 消息体 CSecurityModuleCertificateQueryRsp

	SECURITY_MODULE_CERTIFICATE_SAVE_REQ,  // 消息体 CSecurityModuleCertificateSaveReq 保存证书 包含证书密钥加密值和杂凑值
	SECURITY_MODULE_CERTIFICATE_SAVE_RSP,  // 消息体 CSecurityModuleCertificateSaveRsp

	SECURITY_MODULE_CERTIFICATE_GET_REQ,   //  消息体 CSecurityModuleCertificateGetReq  获取证书 包含证书密钥加密值和杂凑值
	SECURITY_MODULE_CERTIFICATE_GET_RSP,    //  消息体 CSecurityModuleCertificateGetRsp
};


inline void InitSecurityModuleEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(SECURITY_MODULE_EXTERN_MSG_BEGIN);        // 注：为了让此协议文件最小依赖（便于2.0以外的小组使用），此处故意直接写整数
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_NEW_VKEK_REQ);            // 消息体 CSecurityModuleNewVkekReq  生成新的vkek
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_NEW_VKEK_RSP);            // 消息体 CSecurityModuleNewVkekRsp
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_VKEK_SAVE_REQ);           // 消息体 CSecurityModuleVkekSaveReq  保存vkek
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_VKEK_SAVE_RSP);           // 消息体 CSecurityModuleVkekSaveRsp
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_VKEK_QUERY_REQ);          // 消息体 CSecurityModuleVkekQueryReq  查询vkek
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_VKEK_QUERY_RSP);          // 消息体 CSecurityModuleVkekQueryRsp
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_NVKEK_QUERY_REQ);         // 消息体 CSecurityModuleNVkekQueryReq  查询最近的n个vkek
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_NVKEK_QUERY_RSP);         // 消息体 CSecurityModuleNVkekQueryRsp
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_QUERY_REQ);   // 消息体 CSecurityModuleCertificateQueryReq 查询证书 包括签名证书和加密证书
    OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_QUERY_RSP);   // 消息体 CSecurityModuleCertificateQueryRsp
	OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_SAVE_REQ);    // 消息体 CSecurityModuleCertificateSaveReq 保存证书 包括签名证书和加密证书
	OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_SAVE_RSP);    // 消息体 CSecurityModuleCertificateSaveRsp
	OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_GET_REQ);    // 消息体 CSecurityModuleCertificateGetReq 获取证书 包含证书密钥加密值和杂凑值
	OSP_ADD_EVENT_DESC(SECURITY_MODULE_CERTIFICATE_GET_RSP);    // 消息体 CSecurityModuleCertificateGetRsp
}

#endif // _SECURITY_MODULE_EVENT_H





