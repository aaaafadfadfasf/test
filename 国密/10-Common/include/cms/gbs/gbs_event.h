/*****************************************************************************
   模块名      : gbs_event
   文件名      : gbs_event.h
   相关文件    : 
   文件实现功能: gbs_event.h 定义了gbs的外部消息ID
   作者        : 胡志云
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/11/25  1.0         huzhiyun   创建
******************************************************************************/
#ifndef _GBS_EVENT_H_
#define _GBS_EVENT_H_

#include "cms/cms_eventid.h"
#include "cms/cms_proto.h"

/* gbs协议 */
enum 
{
    GBS_EXTERN_MSG_BEGIN = GBS_EVENT_BEGIN, 

    /* 模块登录，使用统一的外设登录信令 DEV_LOGIN_REQ */

    /* 进度通知消息 */
    GBS_PROGRESS_NTF_REQ,  // 消息体 CGbsProgressNtfReq
    GBS_PROGRESS_NTF_RSP,  // 消息体 CNtfRsp

    /* 基本配置 */
    GBS_CONF_SET_REQ,    // 消息体 CGbsConfSetReq
    GBS_CONF_SET_RSP,    // 消息体 CGbsConfSetRsp
    GBS_CONF_GET_REQ,    // 消息体 CGbsConfGetReq
    GBS_CONF_GET_RSP,    // 消息体 CGbsConfGetRsp

    /* 科达域映射配置 */
    GBS_KDDOMAIN_ADD_REQ, // 消息体 CGbsKddomainAddReq
    GBS_KDDOMAIN_ADD_RSP, // 消息体 CGbsKddomainAddRsp
    GBS_KDDOMAIN_DEL_REQ, // 消息体 CGbsKddomainDelReq
    GBS_KDDOMAIN_DEL_RSP, // 消息体 CGbsKddomainDelRsp
    GBS_KDDOMAIN_QRY_REQ, // 消息体 CGbsKddomainQryReq
    GBS_KDDOMAIN_QRY_RSP, // 消息体 CGbsKddomainQryRsp
    GBS_KDDOMAIN_QRY_NTF_REQ, // 消息体 CGbsKddomainQryNtfReq
    GBS_KDDOMAIN_QRY_NTF_RSP, // 消息体 CNtfRsp

    /* 国标PU配置 */
    GBS_PU_ADD_REQ,  // 消息体 CGbsPuAddReq
    GBS_PU_ADD_RSP,  // 消息体 CGbsPuAddRsp
    GBS_PU_DEL_REQ,  // 消息体 CGbsPuDelReq
    GBS_PU_DEL_RSP,  // 消息体 CGbsPuDelRsp
    GBS_PU_MOD_REQ,  // 消息体 CGbsPuModReq
    GBS_PU_MOD_RSP,  // 消息体 CGbsPuModRsp
    GBS_PU_QRY_REQ,  // 消息体 CGbsPuQryReq
    GBS_PU_QRY_RSP,  // 消息体 CGbsPuQryRsp
    GBS_PU_QRY_NTF_REQ, // 消息体 CGbsPuQryNtfReq
    GBS_PU_QRY_NTF_RSP, // 消息体 CNtfRsp
	GBS_PU_SUPPORT_IDTYPES_GET_REQ, //消息体 CGbsPuSupportIdTypesGetReq
	GBS_PU_SUPPORT_IDTYPES_GET_RSP, //消息体 CGbsPuSupportIdTypesGetRsp
	GBS_DECODE_ADD_REQ,  //消息体 CGbsDecodeAddReq
	GBS_DECODE_ADD_RSP,  //消息体 CGbsDecodeAddRsp
	GBS_DECODE_DEL_REQ,  //消息体 CGbsDecodeDelReq
	GBS_DECODE_DEL_RSP,  //消息体 CGbsDecodeDelRsp
	GBS_DECODE_MOD_REQ,  //消息体 CGbsDecodeModReq
	GBS_DECODE_MOD_RSP,  //消息体 CGbsDecodeModRsp
	GBS_DECODE_QRY_REQ,  //消息体 CGbsDecodeQryReq
	GBS_DECODE_QRY_RSP,  //消息体 CGbsDecodeQryRsp
	GBS_DECODE_QRY_NTF_REQ, //消息体 CGbsDecodeQryNtfReq
	GBS_DECODE_QRY_NTF_RSP, //消息体 CGbsDecodeQryNtfRsp

	/* 国标外设配置 */
	GBS_GBPERIPHERAL_ADD_REQ, // 消息体 CGbsGbPeripheralAddReq
	GBS_GBPERIPHERAL_ADD_RSP, // 消息体 CGbsGbPeripheralAddRsp
	GBS_GBPERIPHERAL_DEL_REQ, // 消息体 CGbsGbPeripheralDelReq
	GBS_GBPERIPHERAL_DEL_RSP, // 消息体 CGbsGbPeripheralDelRsp
	GBS_GBPERIPHERAL_DEL_ACK, // 消息体 CGbsGbPeripheralDelAck
	GBS_GBPERIPHERAL_MOD_REQ, // 消息体 CGbsGbPeripheralModReq
	GBS_GBPERIPHERAL_MOD_RSP, // 消息体 CGbsGbPeripheralModRsp
	GBS_GBPERIPHERAL_QRY_REQ, // 消息体 CGbsGbPeripheralQryReq
	GBS_GBPERIPHERAL_QRY_RSP, // 消息体 CGbsGbPeripheralQryRsp
	GBS_GBPERIPHERAL_QRY_NTF_REQ,  // 消息体 CGbsGbPeripheralQryNtfReq
	GBS_GBPERIPHERAL_QRY_NTF_RSP,  // 消息体 CNtfRsp

    /* 国标下级配置 */
    GBS_GBDOMAIN_CHILD_ADD_REQ, // 消息体 CGbsGbdomainChildAddReq
    GBS_GBDOMAIN_CHILD_ADD_RSP, // 消息体 CGbsGbdomainChildAddRsp
    GBS_GBDOMAIN_CHILD_DEL_REQ, // 消息体 CGbsGbdomainChildDelReq  进度通过GBS_PROGRESS_NTF_REQ通知
    GBS_GBDOMAIN_CHILD_DEL_RSP, // 消息体 CGbsGbdomainChildDelRsp
    GBS_GBDOMAIN_CHILD_DEL_ACK, // 消息体 CGbsGbdomainChildDelAck
    GBS_GBDOMAIN_CHILD_DEL_SYNC_REQ, // 消息体 CGbsGbdomainChildDelSyncReq 通知gbu删除下级，防止删除过程中下级入网数据造成干扰
    GBS_GBDOMAIN_CHILD_DEL_SYNC_RSP, // 消息体 CGbsGbdomainChildDelSyncRsp
    GBS_GBDOMAIN_CHILD_MOD_REQ, // 消息体 CGbsGbdomainChildModReq
    GBS_GBDOMAIN_CHILD_MOD_RSP, // 消息体 CGbsGbdomainChildModRsp
    GBS_GBDOMAIN_CHILD_QRY_REQ, // 消息体 CGbsGbdomainChildQryReq
    GBS_GBDOMAIN_CHILD_QRY_RSP, // 消息体 CGbsGbdomainChildQryRsp
    GBS_GBDOMAIN_CHILD_QRY_NTF_REQ, // 消息体 CGbsGbdomainChildQryNtfReq
    GBS_GBDOMAIN_CHILD_QRY_NTF_RSP, // 消息体 CNtfRsp

    /* 国标上级配置 */
    GBS_GBDOMAIN_PARENT_ADD_REQ, // 消息体 CGbsGbdomainParentAddReq
    GBS_GBDOMAIN_PARENT_ADD_RSP, // 消息体 CGbsGbdomainParentAddRsp
    GBS_GBDOMAIN_PARENT_DEL_REQ, // 消息体 CGbsGbdomainParentDelReq
    GBS_GBDOMAIN_PARENT_DEL_RSP, // 消息体 CGbsGbdomainParentDelRsp
    GBS_GBDOMAIN_PARENT_MOD_REQ, // 消息体 CGbsGbdomainParentModReq
    GBS_GBDOMAIN_PARENT_MOD_RSP, // 消息体 CGbsGbdomainParentModRsp
    GBS_GBDOMAIN_PARENT_QRY_REQ, // 消息体 CGbsGbdomainParentQryReq
    GBS_GBDOMAIN_PARENT_QRY_RSP, // 消息体 CGbsGbdomainParentQryRsp
    GBS_GBDOMAIN_PARENT_QRY_NTF_REQ, // 消息体 CGbsGbdomainParentQryNtfReq
    GBS_GBDOMAIN_PARENT_QRY_NTF_RSP, // 消息体 CNtfRsp

    /* 国标上级权限控制 */
    GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_REQ, // 消息体: CGbsGbdomainParentPrivilegeGetReq
    GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_RSP, // 消息体: CGbsGbdomainParentPrivilegeGetRsp
    GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_REQ, // 消息体: CGbsGbdomainParentPrivilegeSetReq
    GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_RSP, // 消息体: CGbsGbdomainParentPrivilegeSetRsp

    /* 高级配置 */
    GBS_ADVANCED_CONF_SET_REQ, // 消息体 CGbsAdvancedConfSetReq
    GBS_ADVANCED_CONF_SET_RSP, // 消息体 CGbsAdvancedConfSetRsp
    GBS_ADVANCED_CONF_GET_REQ, // 消息体 CGbsAdvancedConfGetReq
    GBS_ADVANCED_CONF_GET_RSP, // 消息体 CGbsAdvancedConfGetRsp

	/* 选项配置，每次配置1类 */
	GBS_OPTION_CONF_SET_REQ, // 消息体 CGbsOptionConfSetReq
	GBS_OPTION_CONF_SET_RSP, // 消息体 CGbsOptionConfSetRsp
	GBS_OPTION_CONF_GET_REQ, // 消息体 CGbsOptionConfGetReq
	GBS_OPTION_CONF_GET_RSP, // 消息体 CGbsOptionConfGetRsp

    /* 国标设备查询 */
    GBS_GBDEV_QRY_REQ, // 消息体 CGbsGbdevQryReq
    GBS_GBDEV_QRY_RSP, // 消息体 CGbsGbdevQryRsp
    GBS_GBDEV_QRY_NTF_REQ, // 消息体 CGbsGbdevQryNtfReq
    GBS_GBDEV_QRY_NTF_RSP, // 消息体 CNtfRsp

	GBS_GBPU_QRY_REQ,  //消息体 CGbsGbPuQryReq
	GBS_GBPU_QRY_RSP,  //消息体 CGbsGbPuQryRsp
	GBS_DISC_GBPU_REQ, //消息体 CGbsDiscGbPuReq
	GBS_DISC_GBPU_RSP, //消息体 CGbsDiscGbPuRsp

    /* 国标编号查询 */
    GBS_DEV_GBID_GET_REQ, // 消息体 CGbsDevGbidGetReq
    GBS_DEV_GBID_GET_RSP, // 消息体 CGbsDevGbidGetRsp
	GBS_DEV_GRP_GBID_BATCH_GET_REQ,//消息体 CGbsDevGrpGbidBatchGetReq
	GBS_DEV_GRP_GBID_BATCH_GET_RSP,//消息体 CGbsDevGrpGbidBatchGetRsp
	GBS_DEV_GBID_BATCH_GET_REQ,//消息体 CGbsDevGbidBatchGetReq
	GBS_DEV_GBID_BATCH_GET_RSP,//消息体 CGbsDevGbidBatchGetRsp
	GBS_DEV_KDID_GET_REQ, // 消息体 CGbsDevKdidGetReq
	GBS_DEV_KDID_GET_RSP, // 消息体 CGbsDevKdidGetRsp

	/*国标设备远程重启*/
	GBS_GBDEV_TELEBOOT_REQ, //消息体：CGbsGbDevTeleBootReq
	GBS_GBDEV_TELEBOOT_RSP, //消息体：CGbsGbDevTeleBootRsp

	GBS_GBDEV_RECCTRL_REQ,  //消息体：CGbsGbDevRecCtrlReq
	GBS_GBDEV_RECCTRL_RSP,  //消息体：CGbsGbDevRecCtrlRsp

	/*国标上级域信息查询*/ 
	GBS_GBPARENTDOMAIN_CATALOG_LOAD_REQ, //消息体：CGbsGbPDCatalogLoadReq
	GBS_GBPARENTDOMAIN_CATALOG_LOAD_RSP, //消息体：CGbsGbPDCatalogLoadRsp
	GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_REQ, //消息体：CGbsGbPDCLProgressNtfReq
	GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_RSP, //消息体：CGbsGbPDCLProgressNtfRsp
	GBS_GBPARENTDOMAIN_GET_REQ, //消息体：CGbsGbParentDomainGetReq
	GBS_GBPARENTDOMAIN_GET_RSP, //消息体：CGbsGbParentDomainGetRsp

    /* 业务分组配置 */
    GBS_BUSINESS_GROUP_ADD_REQ, // 消息体 CGbsBusinessGroupAddReq
    GBS_BUSINESS_GROUP_ADD_RSP, // 消息体 CGbsBusinessGroupAddRsp
    GBS_BUSINESS_GROUP_DEL_REQ, // 消息体 CGbsBusinessGroupDelReq
    GBS_BUSINESS_GROUP_DEL_RSP, // 消息体 CGbsBusinessGroupDelRsp
    GBS_BUSINESS_GROUP_MOD_REQ, // 消息体 CGbsBusinessGroupModReq
    GBS_BUSINESS_GROUP_MOD_RSP, // 消息体 CGbsBusinessGroupModRsp
    GBS_BUSINESS_GROUP_QRY_REQ, // 消息体 CGbsBusinessGroupQryReq
    GBS_BUSINESS_GROUP_QRY_RSP, // 消息体 CGbsBusinessGroupQryRsp
    GBS_BUSINESS_GROUP_QRY_NTF_REQ, // 消息体 CGbsBusinessGroupQryNtfReq
    GBS_BUSINESS_GROUP_QRY_NTF_RSP, // 消息体 CNtfRsp

    /* 虚拟组织配置 */
    GBS_VIRTUAL_ORGANIZATION_ADD_REQ, // 消息体 CGbsVirtualOrganizationAddReq
    GBS_VIRTUAL_ORGANIZATION_ADD_RSP, // 消息体 CGbsVirtualOrganizationAddRsp
    GBS_VIRTUAL_ORGANIZATION_DEL_REQ, // 消息体 CGbsVirtualOrganizationDelReq
    GBS_VIRTUAL_ORGANIZATION_DEL_RSP, // 消息体 CGbsVirtualOrganizationDelRsp
    GBS_VIRTUAL_ORGANIZATION_MOD_REQ, // 消息体 CGbsVirtualOrganizationModReq
    GBS_VIRTUAL_ORGANIZATION_MOD_RSP, // 消息体 CGbsVirtualOrganizationModRsp
    GBS_VIRTUAL_ORGANIZATION_QRY_REQ, // 消息体 CGbsVirtualOrganizationQryReq
    GBS_VIRTUAL_ORGANIZATION_QRY_RSP, // 消息体 CGbsVirtualOrganizationQryRsp
    GBS_VIRTUAL_ORGANIZATION_QRY_NTF_REQ, // 消息体 CGbsVirtualOrganizationQryNtfReq
    GBS_VIRTUAL_ORGANIZATION_QRY_NTF_RSP, // 消息体 CNtfRsp

    /* 分组划归配置 */
    GBS_ORGANIZATION2DEV_ADD_REQ, // 消息体 CGbsOrganization2DevAddReq
    GBS_ORGANIZATION2DEV_ADD_RSP, // 消息体 CGbsOrganization2DevAddRsp
    GBS_ORGANIZATION2DEV_DEL_REQ, // 消息体 CGbsOrganization2DevDelReq
    GBS_ORGANIZATION2DEV_DEL_RSP, // 消息体 CGbsOrganization2DevDelRsp
    GBS_ORGANIZATION2DEV_QRY_REQ, // 消息体 CGbsOrganization2DevQryReq
    GBS_ORGANIZATION2DEV_QRY_RSP, // 消息体 CGbsOrganization2DevQryRsp
    GBS_ORGANIZATION2DEV_QRY_NTF_REQ, // 消息体 CGbsOrganization2DevQryNtfReq
    GBS_ORGANIZATION2DEV_QRY_NTF_RSP, // 消息体 CNtfRsp

	//手动刷新目录
	GBS_REFRESH_CATALOG_REQ, // 消息体 CGbsGbRefreshCatalogReq
	GBS_REFRESH_CATALOG_RSP, // 消息体 CGbsGbRefreshCatalogRsp
	GBS_REFRESH_CATALOG_NTF_REQ, // 消息体 CGbsGbRefreshCatalogNtfReq
	GBS_REFRESH_CATALOG_NTF_RSP, // 消息体 CGbsGbRefreshCatalogNtfRsp

	//查询设备组行政区划
	GBS_DEVGRP_CIVILCODE_GET_REQ, // 消息体 CGbsDevGrpCivilCodeGetReq
	GBS_DEVGRP_CIVILCODE_GET_RSP, // 消息体 CGbsDevGrpCivilCodeGetRsp

	//国标选择推送、取消
	GBS_CATALOG_SELECT_REPORT_START_REQ,//消息体 CGbsCatalogSelectReportStartReq
	GBS_CATALOG_SELECT_REPORT_START_RSP,//消息体 CGbsCatalogSelectReportStartRsp
	GBS_CATALOG_SELECT_REPORT_CANCEL_REQ,//消息体 CGbsCatalogSelectReportCancelReq
	GBS_CATALOG_SELECT_REPORT_CANCEL_RSP,//消息体 CGbsCatalogSelectReportCancelRsp
    //GBNat配置
    GBS_NAT_TABLE_ADD_REQ, // 消息体 CGbsNatTableAddReq
    GBS_NAT_TABLE_ADD_RSP, // 消息体 CGbsNatTableAddRsp
    GBS_NAT_TABLE_DEL_REQ, // 消息体 CGbsNatTableDelReq
    GBS_NAT_TABLE_DEL_RSP, // 消息体 CGbsNatTableDelRsp
    GBS_NAT_TABLE_MOD_REQ, // 消息体 CGbsNatTableModReq
    GBS_NAT_TABLE_MOD_RSP, // 消息体 CGbsNatTableModRsp
    GBS_NAT_TABLE_GET_REQ, // 消息体 CGbsNatTableGetReq
    GBS_NAT_TABLE_GET_RSP, // 消息体 CGbsNatTableGetRsp
    GBS_NAT_DATA_LIST_GET_REQ, // 消息体 CGbsNatDataListGetReq 
    GBS_NAT_DATA_LIST_GET_RSP, // 消息体 CGbsNatDataListGetRsp
    GBS_NAT_DATA_SET_REQ, // 消息体 CGbsNatDataAddReq  存在相同的字段组（协议,net内的ip和起始端口与结束端口）时认为是修改
    GBS_NAT_DATA_SET_RSP, // 消息体 CGbsNatDataAddRsp
    GBS_NAT_DATA_DEL_REQ, // 消息体 CGbsNatDataDelReq
    GBS_NAT_DATA_DEL_RSP, // 消息体 CGbsNatDataDelRsp
    GBS_NAT_DATA_CONTENT_QRY_REQ, // 消息体 CGbsNatDataContentQryReq
    GBS_NAT_DATA_CONTENT_QRY_RSP, // 消息体 CGbsNatDataContentQryRsp
    GBS_NAT_DATA_CONTENT_QRY_NTF_REQ, // 消息体 CGbsNatDataContentQryNtfReq
    GBS_NAT_DATA_CONTENT_QRY_NTF_RSP, // 消息体 CNtfRsp
    GBS_GBPU_NAT_SET_REQ, // 消息体 CGbsGbpuNatSetReq
    GBS_GBPU_NAT_SET_RSP, // 消息体 CGbsGbpuNatSetRsp
    GBS_GBPU_NAT_QRY_REQ, // 消息体 CGbsGbpuNatQryReq
    GBS_GBPU_NAT_QRY_RSP, // 消息体 CGbsGbpuNatQryRsp
    GBS_GBPU_NAT_QRY_NTF_REQ, // 消息体 CGbsGbpuNatQryNtfReq
    GBS_GBPU_NAT_QRY_NTF_RSP, // 消息体 CNtfRsp
    GBS_GBPU_NAT_DEL_REQ, // 消息体 CGbsGbpuNatDelReq
    GBS_GBPU_NAT_DEL_RSP, // 消息体 CGbsGbpuNatDelRsp

	//gbs配置连接的puigb的mts
	GBS_SET_PUIGB_MTS_REQ,      //消息体: CGbsSetPuiGbMtsReq
    GBS_SET_PUIGB_MTS_RSP,      //消息体: CGbsSetPuiGbMtsRsp
    
	//gbs 安全cu双向认证协议
	GBS_SAFE_AUTHINFO_CHECK_FIRST_REQ, //消息体：CGbsSafeAuthInfoFirstReq
	GBS_SAFE_AUTHINFO_CHECK_FIRST_RSP, //消息体：CGbsSafeAuthInfoFirstRsp
	GBS_SAFE_AUTHINFO_CHECK_SECOND_REQ,//消息体：CGbsSafeAuthInfoSecondReq
	GBS_SAFE_AUTHINFO_CHECK_SECOND_RSP,//消息体：CGbsSafeAuthInfoSecondRsp

    //证书管理
    GBS_CERTIFICATE_UPLOAD_REQ, //消息体：CGbsCertificateUploadReq
    GBS_CERTIFICATE_UPLOAD_RSP, //消息体：CGbsCertificateUploadRsp
    GBS_CERTIFICATE_GET_REQ, //消息体：CGbsCertificateGetReq
    GBS_CERTIFICATE_GET_RSP, //消息体：CGbsCertificateGetRsp
    
	//GBS支持分布式部署
	GBS_REG_UAS_INFO_QRY_REQ, //消息体：CGbsRegUasInfoQryReq
	GBS_REG_UAS_INFO_QRY_RSP, //消息体：CGbsRegUasInfoQryRsp
	GBS_REG_UAS_INFO_QRY_NTF_REQ, //消息体：CGbsRegUasInfoQryNtfReq
	GBS_REG_UAS_INFO_QRY_NTF_RSP, //消息体：CNtfRsp
	GBS_PRIMARYGBS_CATALOG_QRY_REQ, //消息体：CPrimaryGbsCatalogQryReq
	GBS_PRIMARYGBS_CATALOG_QRY_RSP, //消息体：CPrimaryGbsCatalogQryRsp


    //以下为GB28181的事件定义: fanxg-20131203
    GB28181_NULL_200OK,   //消息体: 无

    /*
      功能：国标注册
      SIP方法：REGISTER，401, 403, 200OK
      SIP-ContentType: 无
      应答机制：SIP应答
      业务流程：
      uac--REGISTER-->uas
      uac<--401--uas
      uac--REGISTER-->uas
      uac<--200OK--uas
    */
    GB28181_REGISTER_REQ,           //消息体: 无
    GB28181_REGISTER_RSP,           //消息体: 无，直接200 OK

    /*
      功能：国标注销
      SIP方法：REGISTER，401, 403, 200OK
      SIP-ContentType: 无
      应答机制：SIP应答
      业务流程：
      uac--REGISTER-->uas
      uac<--401--uas
      uac--REGISTER-->uas
      uac<--200OK--uas
    */
    GB28181_UNREGISTER_REQ,         //消息体: 无
    GB28181_UNREGISTER_RSP,         //消息体: 无，直接200 OK

    /*
      功能：国标心跳
      SIP方法：MESSAGE，200OK
      SIP-ContentType: MANSCDP
      应答机制：SIP应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
    GB28181_KEEP_LIVE_REQ,          //消息体: CGBKeepaliveReq
    //GB28181_KEEP_LIVE_RSP = GB28181_NULL_200OK,          //消息体:  无，直接200 OK

    /*
      功能：国标INVITE，包括：实时浏览、录像回放、录像下载
      SIP方法：INVITE, 200OK，ACK
      SIP-ContentType: SDP
      应答机制：业务应答
      业务流程：
      uac--INVITE-->uas
      uac<--200OK--uas
      uac--ACK-->uas
    */
    GB28181_INVITE_REQ,             //消息体: TGBSdp或空
    GB28181_INVITE_RSP,             //消息体: TGBSdp或空
    GB28181_INVITE_ACK,             //消息体: TGBSdp或空

    /*
      功能：国标BYE，包括：实时浏览、录像回放、录像下载
      SIP方法：BYE, 200OK
      SIP-ContentType:无
      应答机制：SIP应答
      业务流程：
      uac--BYE-->uas
      uac<--200OK--uas
    */
    GB28181_BYE_REQ,             //消息体: 无
    //GB28181_BYE_RSP = GB28181_NULL_200OK,             //消息体: 无，直接200 OK

    /*
      功能：国标PTZ控制
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：SIP应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
    GB28181_PTZ_CTRL_REQ,          //消息体: CGBPTZCmd
    //GB28181_PTZ_CTRL_RSP = GB28181_NULL_200OK,          //消息体:  无，直接200 OK

	/*
      功能：国标前端远程重启（可跨域）
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：SIP应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
	GB28181_TELEBOOT_CMD,          //消息体：CTeleBootCmd
	//GB28181_PTZ_CTRL_RSP = GB28181_NULL_200OK,          //消息体:  无，直接200 OK

    /*
      功能：国标RECORD控制
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_REC_CTRL_REQ,          //消息体: CGBRecordReq
    GB28181_REC_CTRL_RSP,          //消息体: CGBRecordRsp

	/*
	功能：国标关键帧请求
	SIP方法：MESSAGE,200OK
	SIP-ContentType:MANSCDP
	应答机制：业务应答
	业务流程：
	uac--MESSAGE-->uas
	uac<--200OK--uas
	uac<--MESSAGE--uas
	uac--200OK-->uas

	*/
	GB28181_KEYFRAME_CTRL_REQ,     //消息体：CKeyFrameCmd (我司扩展)
	//GB28181_KEYFRAME_CTRL_RSP == GB28181_NULL_200OK,          //消息体:无，直接200OK

	GB28181_IFAMECMD_CTRL_REQ,    //消息体：CCGbIFameCmd(国标标准关键帧请求命令)

    /*
      功能：国标告警通知
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_ALARM_OCCUR_NTF_REQ,          //消息体: CAlarmOccurNotify
    GB28181_ALARM_OCCUR_NTF_RSP,          //消息体: CGBAlarmOccurRsp

    /*
      功能：国标设备目录查询
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
      uac<--MESSAGE--uas  (可能有多次通知应答)
      uac--200OK-->uas
    */
    GB28181_CATALOG_QRY_REQ,         //消息体: CGBCatalogQryReq
    GB28181_CATALOG_QRY_RSP,         //消息体: CGBCataLogQryRsp

    /*
      功能：国标设备信息查询
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_DEV_INFO_QRY_REQ,         //消息体: CGBDevInfoQryReq
    GB28181_DEV_INFO_QRY_RSP,         //消息体: CGBDevInfoQryRsp

    /*
      功能：国标设备状态查询
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_DEV_STATUS_QRY_REQ,         //消息体: CGBDevStatusQryReq
    GB28181_DEV_STATUS_QRY_RSP,         //消息体: CGBDevStatusQryRsp

    /*
      功能：国标设备视音频文件检索
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
      uac<--MESSAGE--uas  (可能有多次通知应答)
      uac--200OK-->uas
    */
    GB28181_REC_INFO_QRY_REQ,         //消息体: CGBRecordInfoQryReq
    GB28181_REC_INFO_QRY_RSP,         //消息体: CGBRecordInfoQryRsp

    /*
      功能：国标VCR控制
      SIP方法：INFO, 200OK
      SIP-ContentType: MANSRTSP
      应答机制：SIP应答
      业务流程：
      uac--INFO-->uas
      uac<--200OK--uas
    */
    GB28181_VCR_CTRL_REQ,          //消息体: TGBRtsp
    //GB28181_VCR_CTRL_RSP = GB28181_NULL_200OK,          //消息体:  无，直接200 OK

    /*
      功能：国标媒体状态通知
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：SIP应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
    GB28181_MEDIA_STATUS_NTF_REQ,          //消息体: CGBMediaStatusNotifyReq
    //GB28181_MEDIA_STATUS_NTF_RSP = GB28181_NULL_200OK,          //消息体:  无，直接200 OK

     /*
      功能：国标目录订阅
      SIP方法：SUBSCRIBE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--SUBSCRIBE-->uas
      uac<--200OK--uas
    */
    GB28181_CATALOG_SUBSCRIBE_REQ,          //消息体: CGBCatalogQryReq
    GB28181_CATALOG_SUBSCRIBE_RSP,          //消息体: 200OK时为CGBCataLogQryRecvRsp，400时为空
    GB28181_CATALOG_UNSUBSCRIBE_REQ,        //消息体: CGBCatalogQryReq
    GB28181_CATALOG_UNSUBSCRIBE_RSP,        //消息体: 200OK时为空


    /*
      功能：国标目录通知
      SIP方法：NOTIFY, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--NOTIFY-->uas
      uac<--200OK--uas
    */
    GB28181_CATALOG_NTF_REQ,          //消息体: CGBCataLogQryRsp
    GB28181_CATALOG_NTF_RSP,          //消息体: CGBCataLogQryRecvRsp

    /*
      功能：国标告警订阅
      SIP方法：SUBSCRIBE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--SUBSCRIBE-->uas
      uac<--200OK--uas
    */
    GB28181_ALARM_SUBSCRIBE_REQ,          //消息体: CGBAlarmSubscribeReq
    GB28181_ALARM_SUBSCRIBE_RSP,          //消息体: CGBAlarmSubscribeRsp

    /*
      功能：国标告警通知
      SIP方法：NOTIFY, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--NOTIFY-->uas
      uac<--200OK--uas
    */
    GB28181_ALARM_NTF_REQ,          //消息体: CGBAlarmNtfReq
    GB28181_ALARM_NTF_RSP,          //消息体: CGBAlarmNtfRsp

	/*
      功能：设备配置参数查询
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
      uac<--MESSAGE--uas  (可能有多次通知应答)
      uac--200OK-->uas
    */
	GB28181_PU_CONFIG_DOWNLOAD_REQ,   //消息体：CGBConfigDownloadReq
	GB28181_PU_CONFIG_DOWNLOAD_RSP,   //消息体：CGBConfigDownloadRsp
	
	/*
      功能：国标透明数据订阅
      SIP方法：SUBSCRIBE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--SUBSCRIBE-->uas
      uac<--200OK--uas
    */
    GB28181_TRANSDATA_SUBSCRIBE_REQ,          //消息体: CGBTransDataSubscribeReq
    GB28181_TRANSDATA_SUBSCRIBE_RSP,          //消息体: 200OK时为CGBTransDataSubscribeRsp，400时为空

	/* 功能：透明命令传输
	   SIP方法：MESSAGE,200OK
	   SIP-ContentType:MANSCDP
	   应答机制：业务应答
	   业务流程：
	   uas--Control-->uac
	   uac--200OK-->uas
	*/
	GB28181_PU_TRANSOPERATE_REQ,  //消息体：CGBTransOperateReq
	GB28181_PU_TRANSOPERATE_RSP,  //消息体：CGBTransOperateRsp

	/*功能：透明数据通知
	  SIP方法：NOTIFY, 200OK
	  SIP-ContentType:MANSCDP
	  应答机制：业务应答
	  业务流程：
	  uac--NOTIFY-->uas
	  uas--200OK-->uac
	*/
	GB28181_PU_TRANSDATA_NTF_REQ,  //消息体：CGBTransDataNtfReq
	GB28181_PU_TRANSDATA_NTF_RSP,  //消息体：CGBTransDataNtfRsp

	/*功能：语音广播
	  SIP方法：MESSAGE,200 OK,INVITE,200 OK, ACK
	  SIP-ContentType:MANSCDP  sdp
	  应答机制：业务应答
	  业务流程：
	  uac--MESSAGE-->uas
	  uas--200OK-->uac
	  uac--INVITE-->uas
	  uas--200OK-->uac
	  uac--ACK-->uas
	*/
	GB28181_AUDIOCALL_MESSAGE_REQ,	//消息体：
	GB28181_AUDIOCALL_MESSAGE_RSP,	//消息体：

	/*
      功能：国标GPS数据订阅
      SIP方法：SUBSCRIBE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--SUBSCRIBE-->uas
      uac<--200OK--uas
    */
    GB28181_GPSDATA_SUBSCRIBE_REQ,          //消息体: CGBGpsSubcribeReq
    GB28181_GPSDATA_SUBSCRIBE_RSP,          //消息体: 200OK时为CGBGpsSubscribeRsp，400时为空

	/*功能：透明数据通知
	  SIP方法：NOTIFY, 200OK
	  SIP-ContentType:MANSCDP
	  应答机制：业务应答
	  业务流程：
	  uac--NOTIFY-->uas
	  uas--200OK-->uac
	*/
	GB28181_GPSDATA_NTF_REQ,  //消息体：CGBGpsDataNtfReq
	GB28181_GPSDATA_NTF_RSP,  //消息体：CGBGpsDataNtfRsp

    /*
    功能：设备配置参数配置
    SIP方法：MESSAGE, 200OK
    SIP-ContentType: MANSCDP
    应答机制：业务应答
    业务流程：
    uac--MESSAGE-->uas
    uac<--200OK--uas
    uac<--MESSAGE--uas
    uac--200OK-->uas
    */
    GB28181_PU_CONFIG_SET_REQ,  //消息体：CGBConfigSetReq    我司接口  是Query类型而国标为control类型
    GB28181_PU_CONFIG_SET_RSP,  //消息体：CGBConfigSetRsp
    GB28181_DEVICE_CONFIG_REQ,  //消息体: CGBDeviceConfigReq 标准国标  应答为GB28181_PU_CONFIG_SET_RSP且设置result

	/*
      功能：国标控制警布撤防
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_GUARD_CTRL_REQ,          //消息体: CGBAlarmGuardReq
	
	/*
      功能：国标控制警布撤防
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_ALARM_RESET_CTRL_REQ,          //消息体: CGBAlarmResetReq
    GB28181_CTRL_RSP,			           //消息体: CGBCommonRsp
	
	/*
      功能：实施码流密钥变更通知
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
	GB28181_CLIENT_VKEK_NOTIFY_REQ,        //消息体:CGBClientVkeyNtfReq

	/*
	功能：获取数字证书
	SIP方法：MESSAGE, 200OK
	SIP-ContentType: MANSCDP
	应答机制：业务应答
	业务流程：
	uac--MESSAGE-->uas
	uac<--200OK--uas
	*/
	GB28181_GET_CERTIFICATE_REQ,          //消息体：CGbGetCertificateReq
	GB28181_GET_CERTIFICATE_RSP,          //消息体：CGbGetCertificateRsp

	/*
	功能：启停安全前端签名功能
	SIP方法：MESSAGE, 200OK
	SIP-ContentType: MANSCDP
	应答机制：业务应答
	业务流程：
	uas--MESSAGE-->uac
	uas<--200OK--uac
	uas<--MESSAGE--uac
	uas--->200OK-->uac
	*/
	GB28181_SIGNATURE_CTRL_REQ,            //消息体：CGbSignatrueCtrlReq
	GB28181_SIGNATURE_CTRL_RSP,            //消息体：CGbSignatureCtrlRsp

	/*
	功能：启停安全前端加密功能
	SIP方法：MESSAGE, 200OK
	SIP-ContentType: MANSCDP
	应答机制：业务应答
	业务流程：
	uas--MESSAGE-->uac
	uas<--200OK--uac
	uas<--MESSAGE--uac
	uas--->200OK-->uac
	*/
	GB28181_ENCRYPTION_CTRL_REQ,          //消息体：CGbEncryptionCtrlReq
	GB28181_ENCRYPTION_CTRL_RSP,          //消息体：CGbEncryptionCtrlRsp
	
	/*
      功能：标准国标GPS数据订阅与通知
      SIP方法：MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      应答机制：业务应答
      业务流程：
      uas--SUBSCRIBE-->uac
      uas<--200OK--uac
      uas<--NOTIFY--uac
      uas--200OK-->uac
    */
	GB28181_MOBILEPOS_SUBSCRIBE_REQ,       //消息体：CGbMobilePositionSubscribeReq，应答200OK
	GB28181_MOBILEPOS_NOTIFY_REQ,          //消息体：CGbMobilePositionDataNtfReq，应答200 OK

	GB28181_KDMCAPABILITY_REQ,  // 消息体: CGbKdmCapabilityReq
	GB28181_KDMCAPABILITY_RSP,  // 消息体: CGbKdmCapabilityRsp

    GBS_EXTERN_MSG_END
};

inline void InitGbsEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(GBS_PROGRESS_NTF_REQ);                    // 消息体 CGbsProgressNtfReq
    OSP_ADD_EVENT_DESC(GBS_PROGRESS_NTF_RSP);                    // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_CONF_SET_REQ);                        // 消息体 CGbsConfSetReq
    OSP_ADD_EVENT_DESC(GBS_CONF_SET_RSP);                        // 消息体 CGbsConfSetRsp
    OSP_ADD_EVENT_DESC(GBS_CONF_GET_REQ);                        // 消息体 CGbsConfGetReq
    OSP_ADD_EVENT_DESC(GBS_CONF_GET_RSP);                        // 消息体 CGbsConfGetRsp
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_ADD_REQ);                    // 消息体 CGbsKddomainAddReq
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_ADD_RSP);                    // 消息体 CGbsKddomainAddRsp
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_DEL_REQ);                    // 消息体 CGbsKddomainDelReq
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_DEL_RSP);                    // 消息体 CGbsKddomainDelRsp
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_QRY_REQ);                    // 消息体 CGbsKddomainQryReq
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_QRY_RSP);                    // 消息体 CGbsKddomainQryRsp
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_QRY_NTF_REQ);                // 消息体 CGbsKddomainQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_QRY_NTF_RSP);                // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_PU_ADD_REQ);                          // 消息体 CGbsPuAddReq
    OSP_ADD_EVENT_DESC(GBS_PU_ADD_RSP);                          // 消息体 CGbsPuAddRsp
    OSP_ADD_EVENT_DESC(GBS_PU_DEL_REQ);                          // 消息体 CGbsPuDelReq
    OSP_ADD_EVENT_DESC(GBS_PU_DEL_RSP);                          // 消息体 CGbsPuDelRsp
    OSP_ADD_EVENT_DESC(GBS_PU_MOD_REQ);                          // 消息体 CGbsPuModReq
    OSP_ADD_EVENT_DESC(GBS_PU_MOD_RSP);                          // 消息体 CGbsPuModRsp
    OSP_ADD_EVENT_DESC(GBS_PU_QRY_REQ);                          // 消息体 CGbsPuQryReq
    OSP_ADD_EVENT_DESC(GBS_PU_QRY_RSP);                          // 消息体 CGbsPuQryRsp
    OSP_ADD_EVENT_DESC(GBS_PU_QRY_NTF_REQ);                      // 消息体 CGbsPuQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_PU_QRY_NTF_RSP);                      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_PU_SUPPORT_IDTYPES_GET_REQ);          //消息体 CGbsPuSupportIdTypesGetReq
    OSP_ADD_EVENT_DESC(GBS_PU_SUPPORT_IDTYPES_GET_RSP);          //消息体 CGbsPuSupportIdTypesGetRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_ADD_REQ);                      //消息体 CGbsDecodeAddReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_ADD_RSP);                      //消息体 CGbsDecodeAddRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_DEL_REQ);                      //消息体 CGbsDecodeDelReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_DEL_RSP);                      //消息体 CGbsDecodeDelRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_MOD_REQ);                      //消息体 CGbsDecodeModReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_MOD_RSP);                      //消息体 CGbsDecodeModRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_QRY_REQ);                      //消息体 CGbsDecodeQryReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_QRY_RSP);                      //消息体 CGbsDecodeQryRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_QRY_NTF_REQ);                  //消息体 CGbsDecodeQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_QRY_NTF_RSP);                  //消息体 CGbsDecodeQryNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_ADD_REQ);                // 消息体 CGbsGbPeripheralAddReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_ADD_RSP);                // 消息体 CGbsGbPeripheralAddRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_DEL_REQ);                // 消息体 CGbsGbPeripheralDelReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_DEL_RSP);                // 消息体 CGbsGbPeripheralDelRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_DEL_ACK);                // 消息体 CGbsGbPeripheralDelAck
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_MOD_REQ);                // 消息体 CGbsGbPeripheralModReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_MOD_RSP);                // 消息体 CGbsGbPeripheralModRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_QRY_REQ);                // 消息体 CGbsGbPeripheralQryReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_QRY_RSP);                // 消息体 CGbsGbPeripheralQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_QRY_NTF_REQ);            // 消息体 CGbsGbPeripheralQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_QRY_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_ADD_REQ);              // 消息体 CGbsGbdomainChildAddReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_ADD_RSP);              // 消息体 CGbsGbdomainChildAddRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_REQ);              // 消息体 CGbsGbdomainChildDelReq  进度通过GBS_PROGRESS_NTF_REQ通知
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_RSP);              // 消息体 CGbsGbdomainChildDelRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_ACK);              // 消息体 CGbsGbdomainChildDelAck
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_SYNC_REQ);         // 消息体 CGbsGbdomainChildDelSyncReq 通知gbu删除下级，防止删除过程中下级入网数据造成干扰
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_SYNC_RSP);         // 消息体 CGbsGbdomainChildDelSyncRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_MOD_REQ);              // 消息体 CGbsGbdomainChildModReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_MOD_RSP);              // 消息体 CGbsGbdomainChildModRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_QRY_REQ);              // 消息体 CGbsGbdomainChildQryReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_QRY_RSP);              // 消息体 CGbsGbdomainChildQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_QRY_NTF_REQ);          // 消息体 CGbsGbdomainChildQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_QRY_NTF_RSP);          // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_ADD_REQ);             // 消息体 CGbsGbdomainParentAddReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_ADD_RSP);             // 消息体 CGbsGbdomainParentAddRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_DEL_REQ);             // 消息体 CGbsGbdomainParentDelReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_DEL_RSP);             // 消息体 CGbsGbdomainParentDelRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_MOD_REQ);             // 消息体 CGbsGbdomainParentModReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_MOD_RSP);             // 消息体 CGbsGbdomainParentModRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_QRY_REQ);             // 消息体 CGbsGbdomainParentQryReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_QRY_RSP);             // 消息体 CGbsGbdomainParentQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_QRY_NTF_REQ);         // 消息体 CGbsGbdomainParentQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_QRY_NTF_RSP);         // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_REQ);   // 消息体: CGbsGbdomainParentPrivilegeGetReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_RSP);   // 消息体: CGbsGbdomainParentPrivilegeGetRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_REQ);   // 消息体: CGbsGbdomainParentPrivilegeSetReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_RSP);   // 消息体: CGbsGbdomainParentPrivilegeSetRsp
    OSP_ADD_EVENT_DESC(GBS_ADVANCED_CONF_SET_REQ);               // 消息体 CGbsAdvancedConfSetReq
    OSP_ADD_EVENT_DESC(GBS_ADVANCED_CONF_SET_RSP);               // 消息体 CGbsAdvancedConfSetRsp
    OSP_ADD_EVENT_DESC(GBS_ADVANCED_CONF_GET_REQ);               // 消息体 CGbsAdvancedConfGetReq
    OSP_ADD_EVENT_DESC(GBS_ADVANCED_CONF_GET_RSP);               // 消息体 CGbsAdvancedConfGetRsp
    OSP_ADD_EVENT_DESC(GBS_OPTION_CONF_SET_REQ);                 // 消息体 CGbsOptionConfSetReq
    OSP_ADD_EVENT_DESC(GBS_OPTION_CONF_SET_RSP);                 // 消息体 CGbsOptionConfSetRsp
    OSP_ADD_EVENT_DESC(GBS_OPTION_CONF_GET_REQ);                 // 消息体 CGbsOptionConfGetReq
    OSP_ADD_EVENT_DESC(GBS_OPTION_CONF_GET_RSP);                 // 消息体 CGbsOptionConfGetRsp
    OSP_ADD_EVENT_DESC(GBS_GBDEV_QRY_REQ);                       // 消息体 CGbsGbdevQryReq
    OSP_ADD_EVENT_DESC(GBS_GBDEV_QRY_RSP);                       // 消息体 CGbsGbdevQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBDEV_QRY_NTF_REQ);                   // 消息体 CGbsGbdevQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBDEV_QRY_NTF_RSP);                   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_QRY_REQ);                        //消息体 CGbsGbPuQryReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_QRY_RSP);                        //消息体 CGbsGbPuQryRsp
    OSP_ADD_EVENT_DESC(GBS_DISC_GBPU_REQ);                       //消息体 CGbsDiscGbPuReq
    OSP_ADD_EVENT_DESC(GBS_DISC_GBPU_RSP);                       //消息体 CGbsDiscGbPuRsp
    OSP_ADD_EVENT_DESC(GBS_DEV_GBID_GET_REQ);                    // 消息体 CGbsDevGbidGetReq
    OSP_ADD_EVENT_DESC(GBS_DEV_GBID_GET_RSP);                    // 消息体 CGbsDevGbidGetRsp
    OSP_ADD_EVENT_DESC(GBS_DEV_GRP_GBID_BATCH_GET_REQ);          //消息体 CGbsDevGrpGbidBatchGetReq
    OSP_ADD_EVENT_DESC(GBS_DEV_GRP_GBID_BATCH_GET_RSP);          //消息体 CGbsDevGrpGbidBatchGetRsp
    OSP_ADD_EVENT_DESC(GBS_DEV_GBID_BATCH_GET_REQ);              //消息体 CGbsDevGbidBatchGetReq
    OSP_ADD_EVENT_DESC(GBS_DEV_GBID_BATCH_GET_RSP);              //消息体 CGbsDevGbidBatchGetRsp
    OSP_ADD_EVENT_DESC(GBS_DEV_KDID_GET_REQ);                    // 消息体 CGbsDevKdidGetReq
    OSP_ADD_EVENT_DESC(GBS_DEV_KDID_GET_RSP);                    // 消息体 CGbsDevKdidGetRsp
    OSP_ADD_EVENT_DESC(GBS_GBDEV_TELEBOOT_REQ);                  //消息体：CGbsGbDevTeleBootReq
    OSP_ADD_EVENT_DESC(GBS_GBDEV_TELEBOOT_RSP);                  //消息体：CGbsGbDevTeleBootRsp
    OSP_ADD_EVENT_DESC(GBS_GBDEV_RECCTRL_REQ);                   //消息体：CGbsGbDevRecCtrlReq
    OSP_ADD_EVENT_DESC(GBS_GBDEV_RECCTRL_RSP);                   //消息体：CGbsGbDevRecCtrlRsp
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_CATALOG_LOAD_REQ);     //消息体：CGbsGbPDCatalogLoadReq
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_CATALOG_LOAD_RSP);     //消息体：CGbsGbPDCatalogLoadRsp
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_REQ);  //消息体：CGbsGbPDCLProgressNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_RSP);  //消息体：CGbsGbPDCLProgressNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_GET_REQ);              //消息体：CGbsGbParentDomainGetReq
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_GET_RSP);              //消息体：CGbsGbParentDomainGetRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_ADD_REQ);              // 消息体 CGbsBusinessGroupAddReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_ADD_RSP);              // 消息体 CGbsBusinessGroupAddRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_DEL_REQ);              // 消息体 CGbsBusinessGroupDelReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_DEL_RSP);              // 消息体 CGbsBusinessGroupDelRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_MOD_REQ);              // 消息体 CGbsBusinessGroupModReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_MOD_RSP);              // 消息体 CGbsBusinessGroupModRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_QRY_REQ);              // 消息体 CGbsBusinessGroupQryReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_QRY_RSP);              // 消息体 CGbsBusinessGroupQryRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_QRY_NTF_REQ);          // 消息体 CGbsBusinessGroupQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_QRY_NTF_RSP);          // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_ADD_REQ);        // 消息体 CGbsVirtualOrganizationAddReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_ADD_RSP);        // 消息体 CGbsVirtualOrganizationAddRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_DEL_REQ);        // 消息体 CGbsVirtualOrganizationDelReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_DEL_RSP);        // 消息体 CGbsVirtualOrganizationDelRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_MOD_REQ);        // 消息体 CGbsVirtualOrganizationModReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_MOD_RSP);        // 消息体 CGbsVirtualOrganizationModRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_QRY_REQ);        // 消息体 CGbsVirtualOrganizationQryReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_QRY_RSP);        // 消息体 CGbsVirtualOrganizationQryRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_QRY_NTF_REQ);    // 消息体 CGbsVirtualOrganizationQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_QRY_NTF_RSP);    // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_ADD_REQ);            // 消息体 CGbsOrganization2DevAddReq
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_ADD_RSP);            // 消息体 CGbsOrganization2DevAddRsp
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_DEL_REQ);            // 消息体 CGbsOrganization2DevDelReq
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_DEL_RSP);            // 消息体 CGbsOrganization2DevDelRsp
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_QRY_REQ);            // 消息体 CGbsOrganization2DevQryReq
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_QRY_RSP);            // 消息体 CGbsOrganization2DevQryRsp
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_QRY_NTF_REQ);        // 消息体 CGbsOrganization2DevQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_QRY_NTF_RSP);        // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_REFRESH_CATALOG_REQ);                 // 消息体 CGbsGbRefreshCatalogReq
    OSP_ADD_EVENT_DESC(GBS_REFRESH_CATALOG_RSP);                 // 消息体 CGbsGbRefreshCatalogRsp
    OSP_ADD_EVENT_DESC(GBS_REFRESH_CATALOG_NTF_REQ);             // 消息体 CGbsGbRefreshCatalogNtfReq
    OSP_ADD_EVENT_DESC(GBS_REFRESH_CATALOG_NTF_RSP);             // 消息体 CGbsGbRefreshCatalogNtfRsp
    OSP_ADD_EVENT_DESC(GBS_DEVGRP_CIVILCODE_GET_REQ);            // 消息体 CGbsDevGrpCivilCodeGetReq
    OSP_ADD_EVENT_DESC(GBS_DEVGRP_CIVILCODE_GET_RSP);            // 消息体 CGbsDevGrpCivilCodeGetRsp
    OSP_ADD_EVENT_DESC(GBS_CATALOG_SELECT_REPORT_START_REQ);     //消息体 CGbsCatalogSelectReportStartReq
    OSP_ADD_EVENT_DESC(GBS_CATALOG_SELECT_REPORT_START_RSP);     //消息体 CGbsCatalogSelectReportStartRsp
    OSP_ADD_EVENT_DESC(GBS_CATALOG_SELECT_REPORT_CANCEL_REQ);    //消息体 CGbsCatalogSelectReportCancelReq
    OSP_ADD_EVENT_DESC(GBS_CATALOG_SELECT_REPORT_CANCEL_RSP);    //消息体 CGbsCatalogSelectReportCancelRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_ADD_REQ);                   // 消息体 CGbsNatTableAddReq
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_ADD_RSP);                   // 消息体 CGbsNatTableAddRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_DEL_REQ);                   // 消息体 CGbsNatTableDelReq
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_DEL_RSP);                   // 消息体 CGbsNatTableDelRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_MOD_REQ);                   // 消息体 CGbsNatTableModReq
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_MOD_RSP);                   // 消息体 CGbsNatTableModRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_GET_REQ);                   // 消息体 CGbsNatTableGetReq
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_GET_RSP);                   // 消息体 CGbsNatTableGetRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_LIST_GET_REQ);               // 消息体 CGbsNatDataListGetReq 
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_LIST_GET_RSP);               // 消息体 CGbsNatDataListGetRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_SET_REQ);                    // 消息体 CGbsNatDataAddReq  存在相同的字段组（协议,net内的ip和起始端口与结束端口）时认为是修改
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_SET_RSP);                    // 消息体 CGbsNatDataAddRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_DEL_REQ);                    // 消息体 CGbsNatDataDelReq
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_DEL_RSP);                    // 消息体 CGbsNatDataDelRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_CONTENT_QRY_REQ);            // 消息体 CGbsNatDataContentQryReq
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_CONTENT_QRY_RSP);            // 消息体 CGbsNatDataContentQryRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_CONTENT_QRY_NTF_REQ);        // 消息体 CGbsNatDataContentQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_CONTENT_QRY_NTF_RSP);        // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_SET_REQ);                    // 消息体 CGbsGbpuNatSetReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_SET_RSP);                    // 消息体 CGbsGbpuNatSetRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_QRY_REQ);                    // 消息体 CGbsGbpuNatQryReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_QRY_RSP);                    // 消息体 CGbsGbpuNatQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_QRY_NTF_REQ);                // 消息体 CGbsGbpuNatQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_QRY_NTF_RSP);                // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_DEL_REQ);                    // 消息体 CGbsGbpuNatDelReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_DEL_RSP);                    // 消息体 CGbsGbpuNatDelRsp
    OSP_ADD_EVENT_DESC(GBS_SET_PUIGB_MTS_REQ);                   //消息体: CGbsSetPuiGbMtsReq
    OSP_ADD_EVENT_DESC(GBS_SET_PUIGB_MTS_RSP);                   //消息体: CGbsSetPuiGbMtsRsp
    OSP_ADD_EVENT_DESC(GBS_SAFE_AUTHINFO_CHECK_FIRST_REQ);       //消息体：CGbsSafeAuthInfoFirstReq
    OSP_ADD_EVENT_DESC(GBS_SAFE_AUTHINFO_CHECK_FIRST_RSP);       //消息体：CGbsSafeAuthInfoFirstRsp
    OSP_ADD_EVENT_DESC(GBS_SAFE_AUTHINFO_CHECK_SECOND_REQ);      //消息体：CGbsSafeAuthInfoSecondReq
    OSP_ADD_EVENT_DESC(GBS_SAFE_AUTHINFO_CHECK_SECOND_RSP);      //消息体：CGbsSafeAuthInfoSecondRsp
    OSP_ADD_EVENT_DESC(GBS_CERTIFICATE_UPLOAD_REQ);              //消息体：CGbsCertificateUploadReq
    OSP_ADD_EVENT_DESC(GBS_CERTIFICATE_UPLOAD_RSP);              //消息体：CGbsCertificateUploadRsp
    OSP_ADD_EVENT_DESC(GBS_CERTIFICATE_GET_REQ);                 //消息体：CGbsCertificateGetReq
    OSP_ADD_EVENT_DESC(GBS_CERTIFICATE_GET_RSP);                 //消息体：CGbsCertificateGetRsp
    OSP_ADD_EVENT_DESC(GBS_REG_UAS_INFO_QRY_REQ);                //消息体：CGbsRegUasInfoQryReq
    OSP_ADD_EVENT_DESC(GBS_REG_UAS_INFO_QRY_RSP);                //消息体：CGbsRegUasInfoQryRsp
    OSP_ADD_EVENT_DESC(GBS_REG_UAS_INFO_QRY_NTF_REQ);            //消息体：CGbsRegUasInfoQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_REG_UAS_INFO_QRY_NTF_RSP);            //消息体：CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_PRIMARYGBS_CATALOG_QRY_REQ);          //消息体：CPrimaryGbsCatalogQryReq
    OSP_ADD_EVENT_DESC(GBS_PRIMARYGBS_CATALOG_QRY_RSP);          //消息体：CPrimaryGbsCatalogQryRsp
    OSP_ADD_EVENT_DESC(GB28181_NULL_200OK);                      //消息体: 无
    OSP_ADD_EVENT_DESC(GB28181_REGISTER_REQ);                    //消息体: 无
    OSP_ADD_EVENT_DESC(GB28181_REGISTER_RSP);                    //消息体: 无，直接200 OK
    OSP_ADD_EVENT_DESC(GB28181_UNREGISTER_REQ);                  //消息体: 无
    OSP_ADD_EVENT_DESC(GB28181_UNREGISTER_RSP);                  //消息体: 无，直接200 OK
    OSP_ADD_EVENT_DESC(GB28181_KEEP_LIVE_REQ);                   //消息体: CGBKeepaliveReq
    OSP_ADD_EVENT_DESC(GB28181_INVITE_REQ);                      //消息体: TGBSdp或空
    OSP_ADD_EVENT_DESC(GB28181_INVITE_RSP);                      //消息体: TGBSdp或空
    OSP_ADD_EVENT_DESC(GB28181_INVITE_ACK);                      //消息体: TGBSdp或空
    OSP_ADD_EVENT_DESC(GB28181_BYE_REQ);                         //消息体: 无
    OSP_ADD_EVENT_DESC(GB28181_PTZ_CTRL_REQ);                    //消息体: CGBPTZCmd
    OSP_ADD_EVENT_DESC(GB28181_TELEBOOT_CMD);                    //消息体：CTeleBootCmd
    OSP_ADD_EVENT_DESC(GB28181_REC_CTRL_REQ);                    //消息体: CGBRecordReq
    OSP_ADD_EVENT_DESC(GB28181_REC_CTRL_RSP);                    //消息体: CGBRecordRsp
    OSP_ADD_EVENT_DESC(GB28181_KEYFRAME_CTRL_REQ);               //消息体：CKeyFrameCmd (我司扩展)
    OSP_ADD_EVENT_DESC(GB28181_IFAMECMD_CTRL_REQ);               //消息体：CCGbIFameCmd(国标标准关键帧请求命令)
    OSP_ADD_EVENT_DESC(GB28181_ALARM_OCCUR_NTF_REQ);             //消息体: CAlarmOccurNotify
    OSP_ADD_EVENT_DESC(GB28181_ALARM_OCCUR_NTF_RSP);             //消息体: CGBAlarmOccurRsp
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_QRY_REQ);                 //消息体: CGBCatalogQryReq
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_QRY_RSP);                 //消息体: CGBCataLogQryRsp
    OSP_ADD_EVENT_DESC(GB28181_DEV_INFO_QRY_REQ);                //消息体: CGBDevInfoQryReq
    OSP_ADD_EVENT_DESC(GB28181_DEV_INFO_QRY_RSP);                //消息体: CGBDevInfoQryRsp
    OSP_ADD_EVENT_DESC(GB28181_DEV_STATUS_QRY_REQ);              //消息体: CGBDevStatusQryReq
    OSP_ADD_EVENT_DESC(GB28181_DEV_STATUS_QRY_RSP);              //消息体: CGBDevStatusQryRsp
    OSP_ADD_EVENT_DESC(GB28181_REC_INFO_QRY_REQ);                //消息体: CGBRecordInfoQryReq
    OSP_ADD_EVENT_DESC(GB28181_REC_INFO_QRY_RSP);                //消息体: CGBRecordInfoQryRsp
    OSP_ADD_EVENT_DESC(GB28181_VCR_CTRL_REQ);                    //消息体: TGBRtsp
    OSP_ADD_EVENT_DESC(GB28181_MEDIA_STATUS_NTF_REQ);            //消息体: CGBMediaStatusNotifyReq
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_SUBSCRIBE_REQ);           //消息体: CGBCatalogQryReq
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_SUBSCRIBE_RSP);           //消息体: 200OK时为CGBCataLogQryRecvRsp，400时为空
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_UNSUBSCRIBE_REQ);         //消息体: CGBCatalogQryReq
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_UNSUBSCRIBE_RSP);         //消息体: 200OK时为空
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_NTF_REQ);                 //消息体: CGBCataLogQryRsp
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_NTF_RSP);                 //消息体: CGBCataLogQryRecvRsp
    OSP_ADD_EVENT_DESC(GB28181_ALARM_SUBSCRIBE_REQ);             //消息体: CGBAlarmSubscribeReq
    OSP_ADD_EVENT_DESC(GB28181_ALARM_SUBSCRIBE_RSP);             //消息体: CGBAlarmSubscribeRsp
    OSP_ADD_EVENT_DESC(GB28181_ALARM_NTF_REQ);                   //消息体: CGBAlarmNtfReq
    OSP_ADD_EVENT_DESC(GB28181_ALARM_NTF_RSP);                   //消息体: CGBAlarmNtfRsp
    OSP_ADD_EVENT_DESC(GB28181_PU_CONFIG_DOWNLOAD_REQ);          //消息体：CGBConfigDownloadReq
    OSP_ADD_EVENT_DESC(GB28181_PU_CONFIG_DOWNLOAD_RSP);          //消息体：CGBConfigDownloadRsp
    OSP_ADD_EVENT_DESC(GB28181_TRANSDATA_SUBSCRIBE_REQ);         //消息体: CGBTransDataSubscribeReq
    OSP_ADD_EVENT_DESC(GB28181_TRANSDATA_SUBSCRIBE_RSP);         //消息体: 200OK时为CGBTransDataSubscribeRsp，400时为空
    OSP_ADD_EVENT_DESC(GB28181_PU_TRANSOPERATE_REQ);             //消息体：CGBTransOperateReq
    OSP_ADD_EVENT_DESC(GB28181_PU_TRANSOPERATE_RSP);             //消息体：CGBTransOperateRsp
    OSP_ADD_EVENT_DESC(GB28181_PU_TRANSDATA_NTF_REQ);            //消息体：CGBTransDataNtfReq
    OSP_ADD_EVENT_DESC(GB28181_PU_TRANSDATA_NTF_RSP);            //消息体：CGBTransDataNtfRsp
    OSP_ADD_EVENT_DESC(GB28181_AUDIOCALL_MESSAGE_REQ);           //消息体：
    OSP_ADD_EVENT_DESC(GB28181_AUDIOCALL_MESSAGE_RSP);           //消息体：
    OSP_ADD_EVENT_DESC(GB28181_GPSDATA_SUBSCRIBE_REQ);           //消息体: CGBGpsSubcribeReq
    OSP_ADD_EVENT_DESC(GB28181_GPSDATA_SUBSCRIBE_RSP);           //消息体: 200OK时为CGBGpsSubscribeRsp，400时为空
    OSP_ADD_EVENT_DESC(GB28181_GPSDATA_NTF_REQ);                 //消息体：CGBGpsDataNtfReq
    OSP_ADD_EVENT_DESC(GB28181_GPSDATA_NTF_RSP);                 //消息体：CGBGpsDataNtfRsp
    OSP_ADD_EVENT_DESC(GB28181_PU_CONFIG_SET_REQ);               //消息体：CGBConfigSetReq    我司接口  是Query类型而国标为control类型
    OSP_ADD_EVENT_DESC(GB28181_PU_CONFIG_SET_RSP);               //消息体：CGBConfigSetRsp
    OSP_ADD_EVENT_DESC(GB28181_DEVICE_CONFIG_REQ);               //消息体: CGBDeviceConfigReq 标准国标  应答为GB28181_PU_CONFIG_SET_RSP且设置result
    OSP_ADD_EVENT_DESC(GB28181_GUARD_CTRL_REQ);                  //消息体: CGBAlarmGuardReq
    OSP_ADD_EVENT_DESC(GB28181_ALARM_RESET_CTRL_REQ);            //消息体: CGBAlarmResetReq
    OSP_ADD_EVENT_DESC(GB28181_CTRL_RSP);                        //消息体: CGBCommonRsp
    OSP_ADD_EVENT_DESC(GB28181_CLIENT_VKEK_NOTIFY_REQ);          //消息体:CGBClientVkeyNtfReq
    OSP_ADD_EVENT_DESC(GB28181_GET_CERTIFICATE_REQ);             //消息体：CGbGetCertificateReq
    OSP_ADD_EVENT_DESC(GB28181_GET_CERTIFICATE_RSP);             //消息体：CGbGetCertificateRsp
    OSP_ADD_EVENT_DESC(GB28181_SIGNATURE_CTRL_REQ);              //消息体：CGbSignatrueCtrlReq
    OSP_ADD_EVENT_DESC(GB28181_SIGNATURE_CTRL_RSP);              //消息体：CGbSignatureCtrlRsp
    OSP_ADD_EVENT_DESC(GB28181_ENCRYPTION_CTRL_REQ);             //消息体：CGbEncryptionCtrlReq
    OSP_ADD_EVENT_DESC(GB28181_ENCRYPTION_CTRL_RSP);             //消息体：CGbEncryptionCtrlRsp
    OSP_ADD_EVENT_DESC(GB28181_MOBILEPOS_SUBSCRIBE_REQ);         //消息体：CGbMobilePositionSubscribeReq，应答200OK
    OSP_ADD_EVENT_DESC(GB28181_MOBILEPOS_NOTIFY_REQ);            //消息体：CGbMobilePositionDataNtfReq，应答200 OK
    OSP_ADD_EVENT_DESC(GB28181_KDMCAPABILITY_REQ);               // 消息体: CGbKdmCapabilityReq
    OSP_ADD_EVENT_DESC(GB28181_KDMCAPABILITY_RSP);               // 消息体: CGbKdmCapabilityRsp
}

#endif // _GBS_EVENT_H_














