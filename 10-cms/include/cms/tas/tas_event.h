/*****************************************************************************
   模块名      : tas_event
   文件名      : tas_event.h
   相关文件    : 
   文件实现功能: tas_event.h 定义了3AS的外部消息ID
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/22   1.0         liangli        创建
******************************************************************************/
#ifndef _TAS_EVENT_H_
#define _TAS_EVENT_H_

#include "cms/cms_eventid.h"
#include "cms/cms_proto.h"

/* 3as协议 */
enum 
{
    TAS_EXTERN_MSG_BEGIN = TAC_TAS_EVENT_BEGIN, // TAS外部消息的起始消息号

    ///// 3AS基本业务功能 /////////////////////////////////////////////////
    TAS_LOGIN_REQ = TAS_EXTERN_MSG_BEGIN, // 消息体 CLoginTasReq  登录管理
    TAS_LOGIN_RSP,  // 消息体 CLoginTasRsp
    TAS_LOGOUT_REQ, // 消息体 CLogoutTasReq
    TAS_LOGOUT_RSP, // 消息体 CLogoutTasRsp
    TAS_CU_ONLINE_REQ,  // 消息体 CCuOnlineReq  cui向tas报告cu的上线与下线行为
    TAS_CU_ONLINE_RSP,  // 消息体 CCuOnlineRsp
    TAS_CU_OFFLINE_REQ, // 消息体 CCuOfflineReq
    TAS_CU_OFFLINE_RSP, // 消息体 CCuOfflineRsp

    TAS_DEVICE_ADD_REQ, // 消息体 CDeviceAddReq 通用设备管理(不包括特殊处理的设备，比如电视墙类设备)
    TAS_DEVICE_ADD_RSP, // 消息体 CDeviceAddRsp
    TAS_DEVICE_ADD_NTF_REQ, // 消息体 CDeviceAddNtfReq
    TAS_DEVICE_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_DEL_REQ, // 消息体 CDeviceDelReq
    TAS_DEVICE_DEL_RSP, // 消息体 CDeviceDelRsp
    TAS_DEVICE_DEL_NTF_REQ, // 消息体 CDeviceDelNtfReq
    TAS_DEVICE_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_MOD_REQ, // 消息体 CDeviceModReq
    TAS_DEVICE_MOD_RSP, // 消息体 CDeviceModRsp
    TAS_DEVICE_MOD_NTF_REQ, // 消息体 CDeviceModNtfReq
    TAS_DEVICE_MOD_NTF_RSP, // 消息体 CNtfRsp
	TAS_DEVICE_BATCH_CHANGE_REQ, //消息体 CTasDeviceBatchChangeReq
	TAS_DEVICE_BATCH_CHANGE_RSP, //消息体 CTasDeviceBatchChangeRsp
    TAS_DEVICE_QRY_REQ, // 消息体 CDeviceQryReq
    TAS_DEVICE_QRY_RSP, // 消息体 CDeviceQryRsp
    TAS_DEVICE_QRY_NTF_REQ, // 消息体 CDeviceQryNtfReq
    TAS_DEVICE_QRY_NTF_RSP, // 消息体 CNtfRsp
    TAS_TVWALL_ADD_REQ, // 消息体 CTvwallAddReq 电视墙设备管理(电视墙中的电视和解码器的绑定关系，由于数据量少不单独定义信令，修改绑定关系时利用电视墙修改信令进行)
    TAS_TVWALL_ADD_RSP, // 消息体 CTvwallAddRsp
    TAS_TVWALL_ADD_NTF_REQ, // 消息体 CTvwallAddNtfReq
    TAS_TVWALL_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_TVWALL_DEL_REQ, // 消息体 CTvwallDelReq
    TAS_TVWALL_DEL_RSP, // 消息体 CTvwallDelRsp
    TAS_TVWALL_DEL_NTF_REQ, // 消息体 CTvwallDelNtfReq
    TAS_TVWALL_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_TVWALL_MOD_REQ, // 消息体 CTvwallModReq
    TAS_TVWALL_MOD_RSP, // 消息体 CTvwallModRsp
    TAS_TVWALL_MOD_NTF_REQ, // 消息体 CTvwallModNtfReq
    TAS_TVWALL_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_TVWALL_QRY_REQ, // 消息体 CTvwallQryReq
    TAS_TVWALL_QRY_RSP, // 消息体 CTvwallQryRsp
    TAS_TVWALL_QRY_NTF_REQ, // 消息体 CTvwallQryNtfReq
    TAS_TVWALL_QRY_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_MODEL_ADD_REQ,     // 消息体 CDeviceModelAddReq  设备型号 不支持修改，因为可能导致一系列相关修改
    TAS_DEVICE_MODEL_ADD_RSP,     // 消息体 CDeviceModelAddRsp
    TAS_DEVICE_MODEL_ADD_NTF_REQ, // 消息体 CDeviceModelAddNtfReq
    TAS_DEVICE_MODEL_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_MODEL_DEL_REQ,     // 消息体 CDeviceModelDelReq
    TAS_DEVICE_MODEL_DEL_RSP,     // 消息体 CDeviceModelDelRsp
    TAS_DEVICE_MODEL_DEL_NTF_REQ, // 消息体 CDeviceModelDelNtfReq
    TAS_DEVICE_MODEL_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_MODEL_QRY_REQ,     // 消息体 CDeviceModelQryReq
    TAS_DEVICE_MODEL_QRY_RSP,     // 消息体 CDeviceModelQryRsp
    TAS_DEVICE_MODEL_QRY_NTF_REQ, // 消息体 CDeviceModelQryNtfReq
    TAS_DEVICE_MODEL_QRY_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_CAP_INDEX_NAME_GET_REQ, // 消息体 CDeviceCapIndexNameGetReq  设备能力索引名称(比如视频源别名)
    TAS_DEVICE_CAP_INDEX_NAME_GET_RSP, // 消息体 CDeviceCapIndexNameGetRsp
    TAS_DEVICE_CAP_INDEX_NAME_SET_REQ, // 消息体 CDeviceCapIndexNameSetReq
    TAS_DEVICE_CAP_INDEX_NAME_SET_RSP, // 消息体 CDeviceCapIndexNameSetRsp
    TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ,  // 消息体 CDeviceCapIndexNameModNtfReq
    TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP,  // 消息体 CNtfRsp
    TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ,     // 消息体 CDeviceCapIndexNameQryReq
    TAS_DEVICE_CAP_INDEX_NAME_QRY_RSP,     // 消息体 CDeviceCapIndexNameQryRsp
    TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ, // 消息体 CDeviceCapIndexNameQryNtfReq
    TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_MANUFACTURER_ADD_REQ,  // 消息体 CDeviceManufacturerAddReq  设备厂商维护
    TAS_DEVICE_MANUFACTURER_ADD_RSP,  // 消息体 CDeviceManufacturerAddRsp
    TAS_DEVICE_MANUFACTURER_DEL_REQ,  // 消息体 CDeviceManufacturerDelReq  
    TAS_DEVICE_MANUFACTURER_DEL_RSP,  // 消息体 CDeviceManufacturerDelRsp
    TAS_DEVICE_MANUFACTURER_QRY_REQ,  // 消息体 CDeviceManufacturerQryReq
    TAS_DEVICE_MANUFACTURER_QRY_RSP,  // 消息体 CDeviceManufacturerQryRsp
    TAS_DEVICE_MANUFACTURER_QRY_NTF_REQ,  // 消息体 CDeviceManufacturerQryNtfReq
    TAS_DEVICE_MANUFACTURER_QRY_NTF_RSP,  // 消息体 CNtfRsp

    TAS_CUSER_ADD_REQ, // 消息体 CUserAddReq  CU用户管理
    TAS_CUSER_ADD_RSP, // 消息体 CUserAddRsp
    TAS_CUSER_ADD_NTF_REQ, // 消息体 CUserAddNtfReq
    TAS_CUSER_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_DEL_REQ, // 消息体 CUserDelReq
    TAS_CUSER_DEL_RSP, // 消息体 CUserDelRsp
    TAS_CUSER_DEL_NTF_REQ, // 消息体 CUserDelNtfReq
    TAS_CUSER_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_MOD_REQ, // 消息体 CUserModReq
    TAS_CUSER_MOD_RSP, // 消息体 CUserModRsp
    TAS_CUSER_MOD_NTF_REQ, // 消息体 CUserModNtfReq
    TAS_CUSER_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_QRY_REQ, // 消息体 CUserQryReq
    TAS_CUSER_QRY_RSP, // 消息体 CUserQryRsp
    TAS_CUSER_QRY_NTF_REQ, // 消息体 CUserQryNtfReq
    TAS_CUSER_QRY_NTF_RSP, // 消息体 CNtfRsp

    TAS_CUSER_DEVICE_ADD_REQ, // 消息体 CUserDeviceAddReq CU用户设备划归
    TAS_CUSER_DEVICE_ADD_RSP, // 消息体 CUserDeviceAddRsp
    TAS_CUSER_DEVICE_ADD_NTF_REQ, // 消息体 CUserDeviceAddNtfReq
    TAS_CUSER_DEVICE_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_DEVICE_DEL_REQ, // 消息体 CUserDeviceDelReq
    TAS_CUSER_DEVICE_DEL_RSP, // 消息体 CUserDeviceDelRsp
    TAS_CUSER_DEVICE_DEL_NTF_REQ, // 消息体 CUserDeviceDelNtfReq
    TAS_CUSER_DEVICE_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_DEVICE_MOD_REQ, // 消息体 CUserDeviceModReq
    TAS_CUSER_DEVICE_MOD_RSP, // 消息体 CUserDeviceModRsp
    TAS_CUSER_DEVICE_MOD_NTF_REQ, // 消息体 CUserDeviceModNtfReq
    TAS_CUSER_DEVICE_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_DEVICE_QRY_REQ, // 消息体 CUserDeviceQryReq
    TAS_CUSER_DEVICE_QRY_RSP, // 消息体 CUserDeviceQryRsp
    TAS_CUSER_DEVICE_QRY_NTF_REQ, // 消息体 CUserDeviceQryNtfReq
    TAS_CUSER_DEVICE_QRY_NTF_RSP, // 消息体 CNtfRsp

    TAS_DEVICE_GROUP_ADD_REQ, // 消息体 CDeviceGroupAddReq 设备分组管理（组的层级，由域层级和域内组层级两者决定）
    TAS_DEVICE_GROUP_ADD_RSP, // 消息体 CDeviceGroupAddRsp
    TAS_DEVICE_GROUP_ADD_NTF_REQ, // 消息体 CDeviceGroupAddNtfReq
    TAS_DEVICE_GROUP_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_GROUP_DEL_REQ, // 消息体 CDeviceGroupDelReq
    TAS_DEVICE_GROUP_DEL_RSP, // 消息体 CDeviceGroupDelRsp
    TAS_DEVICE_GROUP_DEL_NTF_REQ, // 消息体 CDeviceGroupDelNtfReq
    TAS_DEVICE_GROUP_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_GROUP_MOD_REQ, // 消息体 CDeviceGroupModReq
    TAS_DEVICE_GROUP_MOD_RSP, // 消息体 CDeviceGroupModRsp
    TAS_DEVICE_GROUP_MOD_NTF_REQ, // 消息体 CDeviceGroupModNtfReq
    TAS_DEVICE_GROUP_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_GROUP_QRY_REQ, // 消息体 CDeviceGroupQryReq
    TAS_DEVICE_GROUP_QRY_RSP, // 消息体 CDeviceGroupQryRsp
    TAS_DEVICE_GROUP_QRY_NTF_REQ, // 消息体 CDeviceGroupQryNtfReq
    TAS_DEVICE_GROUP_QRY_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_GROUP_DEVICE_ADD_REQ, // 消息体 CDeviceGroupDeviceAddReq 设备分组设备划归
    TAS_DEVICE_GROUP_DEVICE_ADD_RSP, // 消息体 CDeviceGroupDeviceAddRsp
    TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ, // 消息体 CDeviceGroupDeviceAddNtfReq
    TAS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP, // 消息体 CNtfRsp
	TAS_DEVICE_GROUP_MOD_4GB_REQ,        //消息体 CDeviceGroupMod4GbReq
	TAS_DEVICE_GROUP_MOD_4GB_RSP,        //消息体 CDeviceGroupMod4GbRsp
	TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ, // 消息体 CDeviceGroupDeviceMod4GbReq 
	TAS_DEVICE_GROUP_DEVICE_MOD_4GB_RSP, // 消息体 CDeviceGroupDeviceMod4GbRsp
    TAS_DEVICE_GROUP_DEVICE_DEL_REQ, // 消息体 CDeviceGroupDeviceDelReq
    TAS_DEVICE_GROUP_DEVICE_DEL_RSP, // 消息体 CDeviceGroupDeviceDelRsp
    TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ, // 消息体 CDeviceGroupDeviceDelNtfReq
    TAS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP, // 消息体 CNtfRsp
	TAS_DEVICE_GROUP_DEVICE_ORDER_REQ,     // 消息体 CDeviceGroupDeviceOrderReq
	TAS_DEVICE_GROUP_DEVICE_ORDER_RSP,     // 消息体 CDeviceGroupDeviceOrderRsp
	TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_REQ, // 消息体 CDeviceGroupDeviceOrderNtfReq
	TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_RSP, // 消息体 CNtfRsp
    TAS_DEVICE_GROUP_DEVICE_QRY_REQ, // 消息体 CDeviceGroupDeviceQryReq
    TAS_DEVICE_GROUP_DEVICE_QRY_RSP, // 消息体 CDeviceGroupDeviceQryRsp
    TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ, // 消息体 CDeviceGroupDeviceQryNtfReq
    TAS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP, // 消息体 CNtfRsp
	TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ, // 消息体 CDeviceGroupDeviceQry4GbReq
	TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP, // 消息体 CDeviceGroupDeviceQry4GbRsp
	TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ, // 消息体 CDeviceGroupDeviceQry4GbNtfReq
	TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_RSP, // 消息体 CNtfRsp

	TAS_DEVICE_GROUP_CHILDREN_QRY_REQ,     // 指定分组下子分组和设备递归查询 消息体：CDeviceGroupChildrenQryReq
	TAS_DEVICE_GROUP_CHILDREN_QRY_RSP,     // 消息体：CDeviceGroupChildrenQryRsp
	TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_REQ, // 消息体：CDeviceGroupChildrenQryNtfReq
	TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_RSP, // 消息体：CNtfRsp

	TAS_DEVICE_GBID_ERROR_GET_REQ,       // 消息体 CDeviceGbidErrGetReq
	TAS_DEVICE_GBID_ERROR_GET_RSP,       // 消息体 CDeviceGbidErrGetRsp

	TAS_DEVICE_MATCH_GBID_REQ,           //消息体 CDeviceMatchGbidReq
	TAS_DEVICE_MATCH_GBID_RSP,           //消息体 CDeviceMatchGbidRsp

	TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ,  //消息体 CDeviceGrpDevSumNumReq
	TAS_DEVICE_GROUP_DEVICE_SUMNUM_RSP,  //消息体 CDeviceGrpDevSumNumRsp

	TAS_DEVICE_GBINFO_QRY_REQ,              //消息体：CDeviceGbinfoQryReq
	TAS_DEVICE_GBINFO_QRY_RSP,              //消息体：CDeviceGbinfoQryRsp
	TAS_DEVICE_GBINFO_QRY_NTF_REQ,          //消息体：CDeviceGbinfoQryNtfReq
	TAS_DEVICE_GBINFO_QRY_NTF_RSP,          //消息体：CNtfRsp

	TAS_DEVGRP_GBINFO_QRY_REQ,              //消息体：CDevgrpGbinfoQryReq
	TAS_DEVGRP_GBINFO_QRY_RSP,              //消息体：CDevgrpGbinfoQryRsp
	TAS_DEVGRP_GBINFO_QRY_NTF_REQ,          //消息体：CDevgrpGbinfoQryNtfReq
	TAS_DEVGRP_GBINFO_QRY_NTF_RSP,          //消息体：CNtfRsp

    TAS_CUSER_GROUP_ADD_REQ, // 消息体 CUserGroupAddReq  CU用户分组
    TAS_CUSER_GROUP_ADD_RSP, // 消息体 CUserGroupAddRsp
    TAS_CUSER_GROUP_ADD_NTF_REQ, // 消息体 CUserGroupAddNtfReq
    TAS_CUSER_GROUP_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_GROUP_DEL_REQ, // 消息体 CUserGroupDelReq
    TAS_CUSER_GROUP_DEL_RSP, // 消息体 CUserGroupDelRsp
    TAS_CUSER_GROUP_DEL_NTF_REQ, // 消息体 CUserGroupDelNtfReq
    TAS_CUSER_GROUP_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_GROUP_MOD_REQ, // 消息体 CUserGroupModReq
    TAS_CUSER_GROUP_MOD_RSP, // 消息体 CUserGroupModRsp
    TAS_CUSER_GROUP_MOD_NTF_REQ, // 消息体 CUserGroupModNtfReq
    TAS_CUSER_GROUP_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_GROUP_QRY_REQ, // 消息体 CUserGroupQryReq
    TAS_CUSER_GROUP_QRY_RSP, // 消息体 CUserGroupQryRsp
    TAS_CUSER_GROUP_QRY_NTF_REQ, // 消息体 CUserGroupQryNtfReq
    TAS_CUSER_GROUP_QRY_NTF_RSP, // 消息体 CNtfRsp
    // CU用户分组用户划归  直接修改CU用户的所属组Id即可，无需要额外设置信令
    TAS_CUSER_GROUP_DEVICE_ADD_REQ, // 消息体 CUserGroupDeviceAddReq  CU用户分组设备划归
    TAS_CUSER_GROUP_DEVICE_ADD_RSP, // 消息体 CUserGroupDeviceAddRsp
    TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ, // 消息体 CUserGroupDeviceAddNtfReq
    TAS_CUSER_GROUP_DEVICE_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_GROUP_DEVICE_DEL_REQ, // 消息体 CUserGroupDeviceDelReq
    TAS_CUSER_GROUP_DEVICE_DEL_RSP, // 消息体 CUserGroupDeviceDelRsp
    TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ, // 消息体 CUserGroupDeviceDelNtfReq
    TAS_CUSER_GROUP_DEVICE_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_GROUP_DEVICE_MOD_REQ, // 消息体 CUserGroupDeviceModReq
    TAS_CUSER_GROUP_DEVICE_MOD_RSP, // 消息体 CUserGroupDeviceModRsp
    TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ, // 消息体 CUserGroupDeviceModNtfReq
    TAS_CUSER_GROUP_DEVICE_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_CUSER_GROUP_DEVICE_QRY_REQ, // 消息体 CUserGroupDeviceQryReq
    TAS_CUSER_GROUP_DEVICE_QRY_RSP, // 消息体 CUserGroupDeviceQryRsp
    TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ, // 消息体 CUserGroupDeviceQryNtfReq
    TAS_CUSER_GROUP_DEVICE_QRY_NTF_RSP, // 消息体 CNtfRsp

	// 获取指定用户指定设备组下所有视频源信息
	TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ,      //消息体 CUserDevGrpVidsrcQryReq
	TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP,      //消息体 CUserDevGrpVidsrcQryRsp
	TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ,  //消息体 CUserDevGrpVidsrcQryNtfReq
	TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_RSP,  //消息体 CNtfRsp

	// 指定（系统设备树）设备组下视频源总数查询
	TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ,   //消息体 CDevGrpVidsrcTotalnumGetReq
	TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP,   //消息体 CDevGrpVidsrcTotalnumGetRsp

    TAS_LICENSE_UPDATE_REQ, // 消息体 CLicenseUpdateReq  license管理
    TAS_LICENSE_UPDATE_RSP, // 消息体 CLicenseUpdateRsp
    TAS_LICENSE_UPDATE_NTF_REQ, // 消息体 CLicenseUpdateNtfReq
    TAS_LICENSE_UPDATE_NTF_RSP, // 消息体 CNtfRsp
    TAS_LICENSE_SET_REQ,    //消息体 CLicenseSetReq
    TAS_LICENSE_SET_RSP,    //消息体 CLicenseSetRsp
    TAS_LICENSE_GET_REQ,    // 消息体 CLicenseGetReq
    TAS_LICENSE_GET_RSP,    // 消息体 CLicenseGetRsp

    TAS_DOMAIN_RELATION_QRY_REQ,     // 消息体 CDomainRelationQryReq  域关系查询
    TAS_DOMAIN_RELATION_QRY_RSP,     // 消息体 CDomainRelationQryRsp
    TAS_DOMAIN_RELATION_QRY_NTF_REQ, // 消息体 CDomainRelationQryNtfReq
    TAS_DOMAIN_RELATION_QRY_NTF_RSP, // 消息体 CNtfRsp
    TAS_DOMAIN_RELATION_ADD_NTF_REQ, // 消息体 CDomainRelationAddNtfReq
    TAS_DOMAIN_RELATION_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_DOMAIN_RELATION_DEL_NTF_REQ, // 消息体 CDomainRelationDelNtfReq
    TAS_DOMAIN_RELATION_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_DOMAIN_RELATION_MOD_NTF_REQ, // 消息体 CDomainRelationModNtfReq
    TAS_DOMAIN_RELATION_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_DOMAIN_NAME_GET_REQ,    // 消息体 CDomainNameGetReq  域名查询
    TAS_DOMAIN_NAME_GET_RSP,    // 消息体 CDomainNameGetRsp

	TAS_GATEWAY_DEL_REQ,        // 消息体 CGatewayDelReq
	TAS_GATEWAY_DEL_RSP,        // 消息体 CGatewayDelRsp  错误码为ERR_TAS_NOT_FINISHED时，通过RedisGetProcessValue4Tas轮循获取进度


	// v2r2b4功能：分组划归设备
	// 将指定的设备组划归给指定用户
	TAS_CUSER_DEVICE_GROUP_ADD_REQ,							// 消息体 CUserDeviceGroupAddReq
	TAS_CUSER_DEVICE_GROUP_ADD_RSP,							// 消息体 CUserDeviceGroupAddRsp
	TAS_CUSER_DEVICE_GROUP_DEL_REQ,							// 消息体 CUserDeviceGroupDelReq
	TAS_CUSER_DEVICE_GROUP_DEL_RSP,							// 消息体 CUserDeviceGroupDelRsp

	// 将指定的设备组划归给指定用户组
	TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ,					// 消息体 CUserGroupDeviceGroupAddReq
	TAS_CUSER_GROUP_DEVICE_GROUP_ADD_RSP,					// 消息体 CUserGroupDeviceGroupAddRsp
	TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ,					// 消息体 CUserGroupDeviceGroupDelReq
	TAS_CUSER_GROUP_DEVICE_GROUP_DEL_RSP,					// 消息体 CUserGroupDeviceGroupDelRsp

	// 将指定的设备组设置为自动划归组
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ,					// 消息体 CDeviceAutoAssignToUserSetReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_RSP,					// 消息体 CDeviceAutoAssignToUserSetRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ,					// 消息体 CDeviceAutoAssignToUserQryReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP,					// 消息体 CDeviceAutoAssignToUserQryRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_REQ,				// 消息体 CDeviceAutoAssignToUserQryNtfReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_RSP,				// 消息体 CNtfRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ,			// 消息体 CDeviceAutoAssignToUserGroupSetReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_RSP,			// 消息体 CDeviceAutoAssignToUserGroupSetRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ,			// 消息体 CDeviceAutoAssignToUserGroupQryReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP,			// 消息体 CDeviceAutoAssignToUserGroupQryRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_REQ,		// 消息体 CDeviceAutoAssignToUserGroupQryNtfReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_RSP,		// 消息体 CNtfRsp

	// 查询事务的处理进度
	TAS_TRANSACTION_PROGRESS_GET_REQ,						// 消息体 CTransactionProgressGetReq
	TAS_TRANSACTION_PROGRESS_GET_RSP,						// 消息体 CTransactionProgressGetRsp

    ///////// 侧重3AS自身业务功能 ////////////////////////////////////////////////

    TAS_CONFIG_TAS_SET_REQ,   // 消息体 CConfigTasSetReq   Tas配置(包括: a. 本域的域名 b. 父域配置；注意：一次只允许配置一项)
    TAS_CONFIG_TAS_SET_RSP,   // 消息体 CConfigTasSetRsp
    TAS_CONFIG_TAS_GET_REQ,   // 消息体 CConfigTasGetReq
    TAS_CONFIG_TAS_GET_RSP,   // 消息体 CConfigTasGetRsp
    TAS_CHILDTAS_ADD_REQ,  // 消息体 CChildtasAddReq 子域配置
    TAS_CHILDTAS_ADD_RSP,  // 消息体 CChildtasAddRsp
    TAS_CHILDTAS_DEL_REQ,  // 消息体 CChildtasDelReq
    TAS_CHILDTAS_DEL_RSP,  // 消息体 CChildtasDelRsp
	TAS_CHILDTAS_DEL_ACK,  // 消息体 空

    TAS_CTRL_TAS_REQ,        // 消息体 CCtrlTasReq  Tas控制(包括：a. 重做数据同步；注意：一次只允许控制一项)
    TAS_CTRL_TAS_RSP,        // 消息体 CCtrlTasRsp

    TAS_TACUSER_ADD_REQ,     // 消息体 CTacUserAddReq  Tac用户管理
    TAS_TACUSER_ADD_RSP,     // 消息体 CTacUserAddRsp
    TAS_TACUSER_ADD_NTF_REQ, // 消息体 CTacUserAddNtfReq
    TAS_TACUSER_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_TACUSER_DEL_REQ,     // 消息体 CTacUserDelReq
    TAS_TACUSER_DEL_RSP,     // 消息体 CTacUserDelRsp
    TAS_TACUSER_DEL_NTF_REQ, // 消息体 CTacUserDelNtfReq
    TAS_TACUSER_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_TACUSER_MOD_REQ,     // 消息体 CTacUserModReq
    TAS_TACUSER_MOD_RSP,     // 消息体 CTacUserModRsp
    TAS_TACUSER_MOD_NTF_REQ, // 消息体 CTacUserModNtfReq
    TAS_TACUSER_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_TACUSER_QRY_REQ,     // 消息体 CTacUserQryReq
    TAS_TACUSER_QRY_RSP,     // 消息体 CTacUserQryRsp
    TAS_TACUSER_QRY_NTF_REQ, // 消息体 CTacUserQryNtfReq
    TAS_TACUSER_QRY_NTF_RSP, // 消息体 CNtfRsp

    TAS_SYNCDSTDOMAIN_ADD_REQ,     // 消息体 CSyncDstDomainAddReq   数据同步目的域管理
    TAS_SYNCDSTDOMAIN_ADD_RSP,     // 消息体 CSyncDstDomainAddRsp
    TAS_SYNCDSTDOMAIN_ADD_NTF_REQ, // 消息体 CSyncDstDomainAddNtfReq
    TAS_SYNCDSTDOMAIN_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_SYNCDSTDOMAIN_DEL_REQ,     // 消息体 CSyncDstDomainDelReq
    TAS_SYNCDSTDOMAIN_DEL_RSP,     // 消息体 CSyncDstDomainDelRsp
    TAS_SYNCDSTDOMAIN_DEL_NTF_REQ, // 消息体 CSyncDstDomainDelNtfReq
    TAS_SYNCDSTDOMAIN_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_SYNCDSTDOMAIN_MOD_REQ,     // 消息体 CSyncDstDomainModReq
    TAS_SYNCDSTDOMAIN_MOD_RSP,     // 消息体 CSyncDstDomainModRsp
    TAS_SYNCDSTDOMAIN_MOD_NTF_REQ, // 消息体 CSyncDstDomainModNtfReq
    TAS_SYNCDSTDOMAIN_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_SYNCDSTDOMAIN_QRY_REQ,     // 消息体 CSyncDstDomainQryReq
    TAS_SYNCDSTDOMAIN_QRY_RSP,     // 消息体 CSyncDstDomainQryRsp
    TAS_SYNCDSTDOMAIN_QRY_NTF_REQ, // 消息体 CSyncDstDomainQryNtfReq
    TAS_SYNCDSTDOMAIN_QRY_NTF_RSP, // 消息体 CNtfRsp

    // 删除本域中指定点对点同步源域的所有数据，动态通知通过域关系删除通知实现
    TAS_SYNCSRCDOMAIN_DEL_REQ,     // 消息体 CSyncSrcDomainDelReq  数据同步来源域管理
    TAS_SYNCSRCDOMAIN_DEL_RSP,     // 消息体 CSyncSrcDomainDelRsp

    TAS_TEMP_AUTHORIZATION_ADD_REQ,     // 消息体 CTempAuthorizationAddReq  跨域授权管理(禁止二次划归)
    TAS_TEMP_AUTHORIZATION_ADD_RSP,     // 消息体 CTempAuthorizationAddRsp
    TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ, // 消息体 CTempAuthorizationAddNtfReq
    TAS_TEMP_AUTHORIZATION_ADD_NTF_RSP, // 消息体 CNtfRsp
    TAS_TEMP_AUTHORIZATION_DEL_REQ,     // 消息体 CTempAuthorizationDelReq
    TAS_TEMP_AUTHORIZATION_DEL_RSP,     // 消息体 CTempAuthorizationDelRsp
    TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ, // 消息体 CTempAuthorizationDelNtfReq
    TAS_TEMP_AUTHORIZATION_DEL_NTF_RSP, // 消息体 CNtfRsp
    TAS_TEMP_AUTHORIZATION_MOD_REQ,     // 消息体 CTempAuthorizationModReq
    TAS_TEMP_AUTHORIZATION_MOD_RSP,     // 消息体 CTempAuthorizationModRsp
    TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ, // 消息体 CTempAuthorizationModNtfReq
    TAS_TEMP_AUTHORIZATION_MOD_NTF_RSP, // 消息体 CNtfRsp
    TAS_TEMP_AUTHORIZATION_QRY_REQ,     // 消息体 CTempAuthorizationQryReq
    TAS_TEMP_AUTHORIZATION_QRY_RSP,     // 消息体 CTempAuthorizationQryRsp
    TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ, // 消息体 CTempAuthorizationQryNtfReq
    TAS_TEMP_AUTHORIZATION_QRY_NTF_RSP, // 消息体 CNtfRsp

    TAS_SYNC_TAS_DATA_REQ,  // 消息体 CSyncTasDataReq   3AS之间数据同步(包括上下级同步和点对点同步)
    TAS_SYNC_TAS_DATA_RSP,  // 消息体 CSyncTasDataRsp

    ///////// 出于方便和性能考虑3AS扩展的业务接口 ///////////////////////////////////////
    TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ,     // 消息体 CCuserDevicefullinfoAndPrivilegeQryReq   获取CU用户设备完整信息和设备权限信息
    TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP,     // 消息体 CCuserDevicefullinfoAndPrivilegeQryRsp
    TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ, // 消息体 CCuserDevicefullinfoAndPrivilegeQryNtfReq
    TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_RSP, // 消息体 CNtfRsp

	// 获取指定视频源所属组ID，组名，视频源别名，ip地址，经纬度信息(不支持分页和总数限制)
	TAS_VIDSRC_INFO_QRY_REQ,              //消息体 CVidsrcInfoQryReq
	TAS_VIDSRC_INFO_QRY_RSP,              //消息体 CVidsrcInfoQryRsp
	TAS_VIDSRC_INFO_QRY_NTF_REQ,          //消息体 CVidsrcInfoQryNtfReq
	TAS_VIDSRC_INFO_QRY_NTF_RSP,          //消息体 CNtfRsp

    TAS_CUSER_DATA_VERSION_GET_REQ,  // 消息体 CCuserDataVersionGetReq 获取CU用户的设备树版本号
    TAS_CUSER_DATA_VERSION_GET_RSP,  // 消息体 CCuserDataVersionGetRsp

	//自定义设备树相关 [3/10/2016 pangubing]
	TAS_DEVTREE_ADD_REQ,     // 消息体 CDevTreeAddReq 
	TAS_DEVTREE_ADD_RSP,     // 消息体 CDevTreeAddRsp
	TAS_DEVTREE_MOD_REQ,     // 消息体 CDevTreeModReq 
	TAS_DEVTREE_MOD_RSP,     // 消息体 CDevTreeModRsp
	TAS_DEVTREE_DEL_REQ,     // 消息体 CDevTreeDelReq 
	TAS_DEVTREE_DEL_RSP,     // 消息体 CDevTreeDelRsp
	TAS_DEVTREE_QRY_REQ,     // 消息体 CDevTreeQryReq
	TAS_DEVTREE_QRY_RSP,     // 消息体 CDevTreeQryRsp
	TAS_DEVTREE_QRY_NTF_REQ, // 消息体 CDevTreeQryNtfReq
	TAS_DEVTREE_QRY_NTF_RSP, // 消息体 CNtfRsp

	TAS_DEVTREE_DEVGROUP_ASSIGN_REQ,     // 消息体 CDevTreeDevGrpAssignReq 
	TAS_DEVTREE_DEVGROUP_ASSIGN_RSP,     // 消息体 CDevTreeDevGrpAssignRsp

	// 设备设备组路径查询 [3/18/2016 pangubing]
	TAS_DEVICE_GROUPPATH_QRY_REQ,     // 消息体 CDeviceGrpPathQryReq
	TAS_DEVICE_GROUPPATH_QRY_RSP,     // 消息体 CDeviceGrpPathQryRsp
	TAS_DEVICE_GROUPPATH_QRY_NTF_REQ, // 消息体 CDeviceGrpPathQryNtfReq
	TAS_DEVICE_GROUPPATH_QRY_NTF_RSP, // 消息体 CNtfRsp

	// 设备能力修改 [3/30/2016 pangubing]
	TAS_DEV_CAPINFO_MOD_REQ,     // 消息体 CDevCapInfoModReq 
	TAS_DEV_CAPINFO_MOD_RSP,     // 消息体 CDevCapInfoModRsp

	// 云台锁定相关 [4/11/2016 pangubing]
	TAS_PTZLOCK_ADD_REQ,     // 消息体 CPtzLockAddReq 
	TAS_PTZLOCK_ADD_RSP,     // 消息体 CPtzLockAddRsp
	TAS_PTZLOCK_ADD_NTF_REQ, // 消息体 CPtzLockAddNtfReq
	TAS_PTZLOCK_ADD_NTF_RSP, // 消息体 CNtfRsp
	TAS_PTZLOCK_DEL_REQ,     // 消息体 CPtzLockDelReq 
	TAS_PTZLOCK_DEL_RSP,     // 消息体 CPtzLockDelRsp
	TAS_PTZLOCK_DEL_NTF_REQ, // 消息体 CPtzLockDelNtfReq
	TAS_PTZLOCK_DEL_NTF_RSP, // 消息体 CNtfRsp
	TAS_PTZLOCK_QRY_REQ,     // 消息体 CPtzLockQryReq
	TAS_PTZLOCK_QRY_RSP,     // 消息体 CPtzLockQryRsp
	TAS_PTZLOCK_QRY_NTF_REQ, // 消息体 CPtzLockQryNtfReq
	TAS_PTZLOCK_QRY_NTF_RSP, // 消息体 CNtfRsp

    TAS_EXTERN_MSG_END = TAC_TAS_EVENT_END
};

inline void InitTacTasEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(TAS_LOGIN_REQ);                           // 消息体 CLoginTasReq  登录管理
    OSP_ADD_EVENT_DESC(TAS_LOGIN_RSP);                           // 消息体 CLoginTasRsp
    OSP_ADD_EVENT_DESC(TAS_LOGOUT_REQ);                          // 消息体 CLogoutTasReq
    OSP_ADD_EVENT_DESC(TAS_LOGOUT_RSP);                          // 消息体 CLogoutTasRsp
    OSP_ADD_EVENT_DESC(TAS_CU_ONLINE_REQ);                       // 消息体 CCuOnlineReq  cui向tas报告cu的上线与下线行为
    OSP_ADD_EVENT_DESC(TAS_CU_ONLINE_RSP);                       // 消息体 CCuOnlineRsp
    OSP_ADD_EVENT_DESC(TAS_CU_OFFLINE_REQ);                      // 消息体 CCuOfflineReq
    OSP_ADD_EVENT_DESC(TAS_CU_OFFLINE_RSP);                      // 消息体 CCuOfflineRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_ADD_REQ);                      // 消息体 CDeviceAddReq 通用设备管理(不包括特殊处理的设备，比如电视墙类设备)
    OSP_ADD_EVENT_DESC(TAS_DEVICE_ADD_RSP);                      // 消息体 CDeviceAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_ADD_NTF_REQ);                  // 消息体 CDeviceAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_ADD_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_DEL_REQ);                      // 消息体 CDeviceDelReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_DEL_RSP);                      // 消息体 CDeviceDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_DEL_NTF_REQ);                  // 消息体 CDeviceDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_DEL_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MOD_REQ);                      // 消息体 CDeviceModReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MOD_RSP);                      // 消息体 CDeviceModRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MOD_NTF_REQ);                  // 消息体 CDeviceModNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MOD_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_BATCH_CHANGE_REQ);             //消息体 CTasDeviceBatchChangeReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_BATCH_CHANGE_RSP);             //消息体 CTasDeviceBatchChangeRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_QRY_REQ);                      // 消息体 CDeviceQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_QRY_RSP);                      // 消息体 CDeviceQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_QRY_NTF_REQ);                  // 消息体 CDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_QRY_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_ADD_REQ);                      // 消息体 CTvwallAddReq 电视墙设备管理(电视墙中的电视和解码器的绑定关系，由于数据量少不单独定义信令，修改绑定关系时利用电视墙修改信令进行)
    OSP_ADD_EVENT_DESC(TAS_TVWALL_ADD_RSP);                      // 消息体 CTvwallAddRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_ADD_NTF_REQ);                  // 消息体 CTvwallAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_ADD_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_DEL_REQ);                      // 消息体 CTvwallDelReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_DEL_RSP);                      // 消息体 CTvwallDelRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_DEL_NTF_REQ);                  // 消息体 CTvwallDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_DEL_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_MOD_REQ);                      // 消息体 CTvwallModReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_MOD_RSP);                      // 消息体 CTvwallModRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_MOD_NTF_REQ);                  // 消息体 CTvwallModNtfReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_MOD_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_QRY_REQ);                      // 消息体 CTvwallQryReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_QRY_RSP);                      // 消息体 CTvwallQryRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_QRY_NTF_REQ);                  // 消息体 CTvwallQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_QRY_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_ADD_REQ);                // 消息体 CDeviceModelAddReq  设备型号 不支持修改，因为可能导致一系列相关修改
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_ADD_RSP);                // 消息体 CDeviceModelAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_ADD_NTF_REQ);            // 消息体 CDeviceModelAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_ADD_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_DEL_REQ);                // 消息体 CDeviceModelDelReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_DEL_RSP);                // 消息体 CDeviceModelDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_DEL_NTF_REQ);            // 消息体 CDeviceModelDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_DEL_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_QRY_REQ);                // 消息体 CDeviceModelQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_QRY_RSP);                // 消息体 CDeviceModelQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_QRY_NTF_REQ);            // 消息体 CDeviceModelQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_QRY_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_GET_REQ);       // 消息体 CDeviceCapIndexNameGetReq  设备能力索引名称(比如视频源别名)
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_GET_RSP);       // 消息体 CDeviceCapIndexNameGetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_SET_REQ);       // 消息体 CDeviceCapIndexNameSetReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_SET_RSP);       // 消息体 CDeviceCapIndexNameSetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ);   // 消息体 CDeviceCapIndexNameModNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP);   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ);       // 消息体 CDeviceCapIndexNameQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_QRY_RSP);       // 消息体 CDeviceCapIndexNameQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ);   // 消息体 CDeviceCapIndexNameQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP);   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_ADD_REQ);         // 消息体 CDeviceManufacturerAddReq  设备厂商维护
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_ADD_RSP);         // 消息体 CDeviceManufacturerAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_DEL_REQ);         // 消息体 CDeviceManufacturerDelReq  
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_DEL_RSP);         // 消息体 CDeviceManufacturerDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_QRY_REQ);         // 消息体 CDeviceManufacturerQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_QRY_RSP);         // 消息体 CDeviceManufacturerQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_QRY_NTF_REQ);     // 消息体 CDeviceManufacturerQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_QRY_NTF_RSP);     // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_ADD_REQ);                       // 消息体 CUserAddReq  CU用户管理
    OSP_ADD_EVENT_DESC(TAS_CUSER_ADD_RSP);                       // 消息体 CUserAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_ADD_NTF_REQ);                   // 消息体 CUserAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_ADD_NTF_RSP);                   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEL_REQ);                       // 消息体 CUserDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEL_RSP);                       // 消息体 CUserDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEL_NTF_REQ);                   // 消息体 CUserDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEL_NTF_RSP);                   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_MOD_REQ);                       // 消息体 CUserModReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_MOD_RSP);                       // 消息体 CUserModRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_MOD_NTF_REQ);                   // 消息体 CUserModNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_MOD_NTF_RSP);                   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_QRY_REQ);                       // 消息体 CUserQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_QRY_RSP);                       // 消息体 CUserQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_QRY_NTF_REQ);                   // 消息体 CUserQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_QRY_NTF_RSP);                   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_ADD_REQ);                // 消息体 CUserDeviceAddReq CU用户设备划归
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_ADD_RSP);                // 消息体 CUserDeviceAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_ADD_NTF_REQ);            // 消息体 CUserDeviceAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_ADD_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_DEL_REQ);                // 消息体 CUserDeviceDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_DEL_RSP);                // 消息体 CUserDeviceDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_DEL_NTF_REQ);            // 消息体 CUserDeviceDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_DEL_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_MOD_REQ);                // 消息体 CUserDeviceModReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_MOD_RSP);                // 消息体 CUserDeviceModRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_MOD_NTF_REQ);            // 消息体 CUserDeviceModNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_MOD_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_QRY_REQ);                // 消息体 CUserDeviceQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_QRY_RSP);                // 消息体 CUserDeviceQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_QRY_NTF_REQ);            // 消息体 CUserDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_QRY_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_ADD_REQ);                // 消息体 CDeviceGroupAddReq 设备分组管理（组的层级，由域层级和域内组层级两者决定）
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_ADD_RSP);                // 消息体 CDeviceGroupAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_ADD_NTF_REQ);            // 消息体 CDeviceGroupAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_ADD_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEL_REQ);                // 消息体 CDeviceGroupDelReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEL_RSP);                // 消息体 CDeviceGroupDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEL_NTF_REQ);            // 消息体 CDeviceGroupDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEL_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_REQ);                // 消息体 CDeviceGroupModReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_RSP);                // 消息体 CDeviceGroupModRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_NTF_REQ);            // 消息体 CDeviceGroupModNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_QRY_REQ);                // 消息体 CDeviceGroupQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_QRY_RSP);                // 消息体 CDeviceGroupQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_QRY_NTF_REQ);            // 消息体 CDeviceGroupQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_QRY_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ADD_REQ);         // 消息体 CDeviceGroupDeviceAddReq 设备分组设备划归
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ADD_RSP);         // 消息体 CDeviceGroupDeviceAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ);     // 消息体 CDeviceGroupDeviceAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP);     // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_4GB_REQ);            //消息体 CDeviceGroupMod4GbReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_4GB_RSP);            //消息体 CDeviceGroupMod4GbRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ);     // 消息体 CDeviceGroupDeviceMod4GbReq 
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_MOD_4GB_RSP);     // 消息体 CDeviceGroupDeviceMod4GbRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_DEL_REQ);         // 消息体 CDeviceGroupDeviceDelReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_DEL_RSP);         // 消息体 CDeviceGroupDeviceDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ);     // 消息体 CDeviceGroupDeviceDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP);     // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ORDER_REQ);       // 消息体 CDeviceGroupDeviceOrderReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ORDER_RSP);       // 消息体 CDeviceGroupDeviceOrderRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_REQ);   // 消息体 CDeviceGroupDeviceOrderNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_RSP);   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_REQ);         // 消息体 CDeviceGroupDeviceQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_RSP);         // 消息体 CDeviceGroupDeviceQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ);     // 消息体 CDeviceGroupDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP);     // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ);     // 消息体 CDeviceGroupDeviceQry4GbReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP);     // 消息体 CDeviceGroupDeviceQry4GbRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ); // 消息体 CDeviceGroupDeviceQry4GbNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_RSP); // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_CHILDREN_QRY_REQ);       // 指定分组下子分组和设备递归查询 消息体：CDeviceGroupChildrenQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_CHILDREN_QRY_RSP);       // 消息体：CDeviceGroupChildrenQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_REQ);   // 消息体：CDeviceGroupChildrenQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_RSP);   // 消息体：CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBID_ERROR_GET_REQ);           // 消息体 CDeviceGbidErrGetReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBID_ERROR_GET_RSP);           // 消息体 CDeviceGbidErrGetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MATCH_GBID_REQ);               //消息体 CDeviceMatchGbidReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MATCH_GBID_RSP);               //消息体 CDeviceMatchGbidRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ);      //消息体 CDeviceGrpDevSumNumReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_SUMNUM_RSP);      //消息体 CDeviceGrpDevSumNumRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBINFO_QRY_REQ);               //消息体：CDeviceGbinfoQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBINFO_QRY_RSP);               //消息体：CDeviceGbinfoQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBINFO_QRY_NTF_REQ);           //消息体：CDeviceGbinfoQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBINFO_QRY_NTF_RSP);           //消息体：CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_GBINFO_QRY_REQ);               //消息体：CDevgrpGbinfoQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_GBINFO_QRY_RSP);               //消息体：CDevgrpGbinfoQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_GBINFO_QRY_NTF_REQ);           //消息体：CDevgrpGbinfoQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_GBINFO_QRY_NTF_RSP);           //消息体：CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_ADD_REQ);                 // 消息体 CUserGroupAddReq  CU用户分组
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_ADD_RSP);                 // 消息体 CUserGroupAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_ADD_NTF_REQ);             // 消息体 CUserGroupAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_ADD_NTF_RSP);             // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEL_REQ);                 // 消息体 CUserGroupDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEL_RSP);                 // 消息体 CUserGroupDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEL_NTF_REQ);             // 消息体 CUserGroupDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEL_NTF_RSP);             // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_MOD_REQ);                 // 消息体 CUserGroupModReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_MOD_RSP);                 // 消息体 CUserGroupModRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_MOD_NTF_REQ);             // 消息体 CUserGroupModNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_MOD_NTF_RSP);             // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_QRY_REQ);                 // 消息体 CUserGroupQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_QRY_RSP);                 // 消息体 CUserGroupQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_QRY_NTF_REQ);             // 消息体 CUserGroupQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_QRY_NTF_RSP);             // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_ADD_REQ);          // 消息体 CUserGroupDeviceAddReq  CU用户分组设备划归
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_ADD_RSP);          // 消息体 CUserGroupDeviceAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ);      // 消息体 CUserGroupDeviceAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_ADD_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_DEL_REQ);          // 消息体 CUserGroupDeviceDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_DEL_RSP);          // 消息体 CUserGroupDeviceDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ);      // 消息体 CUserGroupDeviceDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_DEL_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_MOD_REQ);          // 消息体 CUserGroupDeviceModReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_MOD_RSP);          // 消息体 CUserGroupDeviceModRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ);      // 消息体 CUserGroupDeviceModNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_MOD_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_QRY_REQ);          // 消息体 CUserGroupDeviceQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_QRY_RSP);          // 消息体 CUserGroupDeviceQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ);      // 消息体 CUserGroupDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_QRY_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ);         //消息体 CUserDevGrpVidsrcQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP);         //消息体 CUserDevGrpVidsrcQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ);     //消息体 CUserDevGrpVidsrcQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_RSP);     //消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ);      //消息体 CDevGrpVidsrcTotalnumGetReq
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP);      //消息体 CDevGrpVidsrcTotalnumGetRsp
    OSP_ADD_EVENT_DESC(TAS_LICENSE_UPDATE_REQ);                  // 消息体 CLicenseUpdateReq  license管理
    OSP_ADD_EVENT_DESC(TAS_LICENSE_UPDATE_RSP);                  // 消息体 CLicenseUpdateRsp
    OSP_ADD_EVENT_DESC(TAS_LICENSE_UPDATE_NTF_REQ);              // 消息体 CLicenseUpdateNtfReq
    OSP_ADD_EVENT_DESC(TAS_LICENSE_UPDATE_NTF_RSP);              // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_LICENSE_SET_REQ);                     //消息体 CLicenseSetReq
    OSP_ADD_EVENT_DESC(TAS_LICENSE_SET_RSP);                     //消息体 CLicenseSetRsp
    OSP_ADD_EVENT_DESC(TAS_LICENSE_GET_REQ);                     // 消息体 CLicenseGetReq
    OSP_ADD_EVENT_DESC(TAS_LICENSE_GET_RSP);                     // 消息体 CLicenseGetRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_QRY_REQ);             // 消息体 CDomainRelationQryReq  域关系查询
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_QRY_RSP);             // 消息体 CDomainRelationQryRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_QRY_NTF_REQ);         // 消息体 CDomainRelationQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_QRY_NTF_RSP);         // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_ADD_NTF_REQ);         // 消息体 CDomainRelationAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_ADD_NTF_RSP);         // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_DEL_NTF_REQ);         // 消息体 CDomainRelationDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_DEL_NTF_RSP);         // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_MOD_NTF_REQ);         // 消息体 CDomainRelationModNtfReq
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_MOD_NTF_RSP);         // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_NAME_GET_REQ);                 // 消息体 CDomainNameGetReq  域名查询
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_NAME_GET_RSP);                 // 消息体 CDomainNameGetRsp
    OSP_ADD_EVENT_DESC(TAS_GATEWAY_DEL_REQ);                     // 消息体 CGatewayDelReq
    OSP_ADD_EVENT_DESC(TAS_GATEWAY_DEL_RSP);                     // 消息体 CGatewayDelRsp  错误码为ERR_TAS_NOT_FINISHED时，通过RedisGetProcessValue4Tas轮循获取进度
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_GROUP_ADD_REQ);          // 消息体 CUserDeviceGroupAddReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_GROUP_ADD_RSP);          // 消息体 CUserDeviceGroupAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_GROUP_DEL_REQ);          // 消息体 CUserDeviceGroupDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_GROUP_DEL_RSP);          // 消息体 CUserDeviceGroupDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ);    // 消息体 CUserGroupDeviceGroupAddReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_GROUP_ADD_RSP);    // 消息体 CUserGroupDeviceGroupAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ);    // 消息体 CUserGroupDeviceGroupDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_GROUP_DEL_RSP);    // 消息体 CUserGroupDeviceGroupDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ);  // 消息体 CDeviceAutoAssignToUserSetReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_RSP);  // 消息体 CDeviceAutoAssignToUserSetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ);  // 消息体 CDeviceAutoAssignToUserQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP);  // 消息体 CDeviceAutoAssignToUserQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_REQ);// 消息体 CDeviceAutoAssignToUserQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_RSP);// 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ);// 消息体 CDeviceAutoAssignToUserGroupSetReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_RSP);// 消息体 CDeviceAutoAssignToUserGroupSetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ);// 消息体 CDeviceAutoAssignToUserGroupQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP);// 消息体 CDeviceAutoAssignToUserGroupQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_REQ);// 消息体 CDeviceAutoAssignToUserGroupQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_RSP);// 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TRANSACTION_PROGRESS_GET_REQ);        // 消息体 CTransactionProgressGetReq
    OSP_ADD_EVENT_DESC(TAS_TRANSACTION_PROGRESS_GET_RSP);        // 消息体 CTransactionProgressGetRsp
    OSP_ADD_EVENT_DESC(TAS_CONFIG_TAS_SET_REQ);                  // 消息体 CConfigTasSetReq   Tas配置(包括: a. 本域的域名 b. 父域配置；注意：一次只允许配置一项)
    OSP_ADD_EVENT_DESC(TAS_CONFIG_TAS_SET_RSP);                  // 消息体 CConfigTasSetRsp
    OSP_ADD_EVENT_DESC(TAS_CONFIG_TAS_GET_REQ);                  // 消息体 CConfigTasGetReq
    OSP_ADD_EVENT_DESC(TAS_CONFIG_TAS_GET_RSP);                  // 消息体 CConfigTasGetRsp
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_ADD_REQ);                    // 消息体 CChildtasAddReq 子域配置
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_ADD_RSP);                    // 消息体 CChildtasAddRsp
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_DEL_REQ);                    // 消息体 CChildtasDelReq
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_DEL_RSP);                    // 消息体 CChildtasDelRsp
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_DEL_ACK);                    // 消息体 空
    OSP_ADD_EVENT_DESC(TAS_CTRL_TAS_REQ);                        // 消息体 CCtrlTasReq  Tas控制(包括：a. 重做数据同步；注意：一次只允许控制一项)
    OSP_ADD_EVENT_DESC(TAS_CTRL_TAS_RSP);                        // 消息体 CCtrlTasRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_ADD_REQ);                     // 消息体 CTacUserAddReq  Tac用户管理
    OSP_ADD_EVENT_DESC(TAS_TACUSER_ADD_RSP);                     // 消息体 CTacUserAddRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_ADD_NTF_REQ);                 // 消息体 CTacUserAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_ADD_NTF_RSP);                 // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_DEL_REQ);                     // 消息体 CTacUserDelReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_DEL_RSP);                     // 消息体 CTacUserDelRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_DEL_NTF_REQ);                 // 消息体 CTacUserDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_DEL_NTF_RSP);                 // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_MOD_REQ);                     // 消息体 CTacUserModReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_MOD_RSP);                     // 消息体 CTacUserModRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_MOD_NTF_REQ);                 // 消息体 CTacUserModNtfReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_MOD_NTF_RSP);                 // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_QRY_REQ);                     // 消息体 CTacUserQryReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_QRY_RSP);                     // 消息体 CTacUserQryRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_QRY_NTF_REQ);                 // 消息体 CTacUserQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_QRY_NTF_RSP);                 // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_ADD_REQ);               // 消息体 CSyncDstDomainAddReq   数据同步目的域管理
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_ADD_RSP);               // 消息体 CSyncDstDomainAddRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_ADD_NTF_REQ);           // 消息体 CSyncDstDomainAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_ADD_NTF_RSP);           // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_DEL_REQ);               // 消息体 CSyncDstDomainDelReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_DEL_RSP);               // 消息体 CSyncDstDomainDelRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_DEL_NTF_REQ);           // 消息体 CSyncDstDomainDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_DEL_NTF_RSP);           // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_MOD_REQ);               // 消息体 CSyncDstDomainModReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_MOD_RSP);               // 消息体 CSyncDstDomainModRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_MOD_NTF_REQ);           // 消息体 CSyncDstDomainModNtfReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_MOD_NTF_RSP);           // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_QRY_REQ);               // 消息体 CSyncDstDomainQryReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_QRY_RSP);               // 消息体 CSyncDstDomainQryRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_QRY_NTF_REQ);           // 消息体 CSyncDstDomainQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_QRY_NTF_RSP);           // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCSRCDOMAIN_DEL_REQ);               // 消息体 CSyncSrcDomainDelReq  数据同步来源域管理
    OSP_ADD_EVENT_DESC(TAS_SYNCSRCDOMAIN_DEL_RSP);               // 消息体 CSyncSrcDomainDelRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_ADD_REQ);          // 消息体 CTempAuthorizationAddReq  跨域授权管理(禁止二次划归)
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_ADD_RSP);          // 消息体 CTempAuthorizationAddRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ);      // 消息体 CTempAuthorizationAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_ADD_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_DEL_REQ);          // 消息体 CTempAuthorizationDelReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_DEL_RSP);          // 消息体 CTempAuthorizationDelRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ);      // 消息体 CTempAuthorizationDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_DEL_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_MOD_REQ);          // 消息体 CTempAuthorizationModReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_MOD_RSP);          // 消息体 CTempAuthorizationModRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ);      // 消息体 CTempAuthorizationModNtfReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_MOD_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_QRY_REQ);          // 消息体 CTempAuthorizationQryReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_QRY_RSP);          // 消息体 CTempAuthorizationQryRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ);      // 消息体 CTempAuthorizationQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_QRY_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNC_TAS_DATA_REQ);                   // 消息体 CSyncTasDataReq   3AS之间数据同步(包括上下级同步和点对点同步)
    OSP_ADD_EVENT_DESC(TAS_SYNC_TAS_DATA_RSP);                   // 消息体 CSyncTasDataRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ);// 消息体 CCuserDevicefullinfoAndPrivilegeQryReq   获取CU用户设备完整信息和设备权限信息
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP);// 消息体 CCuserDevicefullinfoAndPrivilegeQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ);// 消息体 CCuserDevicefullinfoAndPrivilegeQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_RSP);// 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_VIDSRC_INFO_QRY_REQ);                 //消息体 CVidsrcInfoQryReq
    OSP_ADD_EVENT_DESC(TAS_VIDSRC_INFO_QRY_RSP);                 //消息体 CVidsrcInfoQryRsp
    OSP_ADD_EVENT_DESC(TAS_VIDSRC_INFO_QRY_NTF_REQ);             //消息体 CVidsrcInfoQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_VIDSRC_INFO_QRY_NTF_RSP);             //消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DATA_VERSION_GET_REQ);          // 消息体 CCuserDataVersionGetReq 获取CU用户的设备树版本号
    OSP_ADD_EVENT_DESC(TAS_CUSER_DATA_VERSION_GET_RSP);          // 消息体 CCuserDataVersionGetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_ADD_REQ);                     // 消息体 CDevTreeAddReq 
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_ADD_RSP);                     // 消息体 CDevTreeAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_MOD_REQ);                     // 消息体 CDevTreeModReq 
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_MOD_RSP);                     // 消息体 CDevTreeModRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_DEL_REQ);                     // 消息体 CDevTreeDelReq 
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_DEL_RSP);                     // 消息体 CDevTreeDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_QRY_REQ);                     // 消息体 CDevTreeQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_QRY_RSP);                     // 消息体 CDevTreeQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_QRY_NTF_REQ);                 // 消息体 CDevTreeQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_QRY_NTF_RSP);                 // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_DEVGROUP_ASSIGN_REQ);         // 消息体 CDevTreeDevGrpAssignReq 
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_DEVGROUP_ASSIGN_RSP);         // 消息体 CDevTreeDevGrpAssignRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUPPATH_QRY_REQ);            // 消息体 CDeviceGrpPathQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUPPATH_QRY_RSP);            // 消息体 CDeviceGrpPathQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUPPATH_QRY_NTF_REQ);        // 消息体 CDeviceGrpPathQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUPPATH_QRY_NTF_RSP);        // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEV_CAPINFO_MOD_REQ);                 // 消息体 CDevCapInfoModReq 
    OSP_ADD_EVENT_DESC(TAS_DEV_CAPINFO_MOD_RSP);                 // 消息体 CDevCapInfoModRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_ADD_REQ);                     // 消息体 CPtzLockAddReq 
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_ADD_RSP);                     // 消息体 CPtzLockAddRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_ADD_NTF_REQ);                 // 消息体 CPtzLockAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_ADD_NTF_RSP);                 // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_DEL_REQ);                     // 消息体 CPtzLockDelReq 
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_DEL_RSP);                     // 消息体 CPtzLockDelRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_DEL_NTF_REQ);                 // 消息体 CPtzLockDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_DEL_NTF_RSP);                 // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_QRY_REQ);                     // 消息体 CPtzLockQryReq
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_QRY_RSP);                     // 消息体 CPtzLockQryRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_QRY_NTF_REQ);                 // 消息体 CPtzLockQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_QRY_NTF_RSP);                 // 消息体 CNtfRsp
}

#endif // _TAS_EVENT_H_





























