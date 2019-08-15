/*****************************************************************************
   ģ����      : tas_event
   �ļ���      : tas_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: tas_event.h ������3AS���ⲿ��ϢID
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/22   1.0         liangli        ����
******************************************************************************/
#ifndef _TAS_EVENT_H_
#define _TAS_EVENT_H_

#include "cms/cms_eventid.h"
#include "cms/cms_proto.h"

/* 3asЭ�� */
enum 
{
    TAS_EXTERN_MSG_BEGIN = TAC_TAS_EVENT_BEGIN, // TAS�ⲿ��Ϣ����ʼ��Ϣ��

    ///// 3AS����ҵ���� /////////////////////////////////////////////////
    TAS_LOGIN_REQ = TAS_EXTERN_MSG_BEGIN, // ��Ϣ�� CLoginTasReq  ��¼����
    TAS_LOGIN_RSP,  // ��Ϣ�� CLoginTasRsp
    TAS_LOGOUT_REQ, // ��Ϣ�� CLogoutTasReq
    TAS_LOGOUT_RSP, // ��Ϣ�� CLogoutTasRsp
    TAS_CU_ONLINE_REQ,  // ��Ϣ�� CCuOnlineReq  cui��tas����cu��������������Ϊ
    TAS_CU_ONLINE_RSP,  // ��Ϣ�� CCuOnlineRsp
    TAS_CU_OFFLINE_REQ, // ��Ϣ�� CCuOfflineReq
    TAS_CU_OFFLINE_RSP, // ��Ϣ�� CCuOfflineRsp

    TAS_DEVICE_ADD_REQ, // ��Ϣ�� CDeviceAddReq ͨ���豸����(���������⴦����豸���������ǽ���豸)
    TAS_DEVICE_ADD_RSP, // ��Ϣ�� CDeviceAddRsp
    TAS_DEVICE_ADD_NTF_REQ, // ��Ϣ�� CDeviceAddNtfReq
    TAS_DEVICE_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_DEL_REQ, // ��Ϣ�� CDeviceDelReq
    TAS_DEVICE_DEL_RSP, // ��Ϣ�� CDeviceDelRsp
    TAS_DEVICE_DEL_NTF_REQ, // ��Ϣ�� CDeviceDelNtfReq
    TAS_DEVICE_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_MOD_REQ, // ��Ϣ�� CDeviceModReq
    TAS_DEVICE_MOD_RSP, // ��Ϣ�� CDeviceModRsp
    TAS_DEVICE_MOD_NTF_REQ, // ��Ϣ�� CDeviceModNtfReq
    TAS_DEVICE_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
	TAS_DEVICE_BATCH_CHANGE_REQ, //��Ϣ�� CTasDeviceBatchChangeReq
	TAS_DEVICE_BATCH_CHANGE_RSP, //��Ϣ�� CTasDeviceBatchChangeRsp
    TAS_DEVICE_QRY_REQ, // ��Ϣ�� CDeviceQryReq
    TAS_DEVICE_QRY_RSP, // ��Ϣ�� CDeviceQryRsp
    TAS_DEVICE_QRY_NTF_REQ, // ��Ϣ�� CDeviceQryNtfReq
    TAS_DEVICE_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TVWALL_ADD_REQ, // ��Ϣ�� CTvwallAddReq ����ǽ�豸����(����ǽ�еĵ��Ӻͽ������İ󶨹�ϵ�������������ٲ�������������޸İ󶨹�ϵʱ���õ���ǽ�޸��������)
    TAS_TVWALL_ADD_RSP, // ��Ϣ�� CTvwallAddRsp
    TAS_TVWALL_ADD_NTF_REQ, // ��Ϣ�� CTvwallAddNtfReq
    TAS_TVWALL_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TVWALL_DEL_REQ, // ��Ϣ�� CTvwallDelReq
    TAS_TVWALL_DEL_RSP, // ��Ϣ�� CTvwallDelRsp
    TAS_TVWALL_DEL_NTF_REQ, // ��Ϣ�� CTvwallDelNtfReq
    TAS_TVWALL_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TVWALL_MOD_REQ, // ��Ϣ�� CTvwallModReq
    TAS_TVWALL_MOD_RSP, // ��Ϣ�� CTvwallModRsp
    TAS_TVWALL_MOD_NTF_REQ, // ��Ϣ�� CTvwallModNtfReq
    TAS_TVWALL_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TVWALL_QRY_REQ, // ��Ϣ�� CTvwallQryReq
    TAS_TVWALL_QRY_RSP, // ��Ϣ�� CTvwallQryRsp
    TAS_TVWALL_QRY_NTF_REQ, // ��Ϣ�� CTvwallQryNtfReq
    TAS_TVWALL_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_MODEL_ADD_REQ,     // ��Ϣ�� CDeviceModelAddReq  �豸�ͺ� ��֧���޸ģ���Ϊ���ܵ���һϵ������޸�
    TAS_DEVICE_MODEL_ADD_RSP,     // ��Ϣ�� CDeviceModelAddRsp
    TAS_DEVICE_MODEL_ADD_NTF_REQ, // ��Ϣ�� CDeviceModelAddNtfReq
    TAS_DEVICE_MODEL_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_MODEL_DEL_REQ,     // ��Ϣ�� CDeviceModelDelReq
    TAS_DEVICE_MODEL_DEL_RSP,     // ��Ϣ�� CDeviceModelDelRsp
    TAS_DEVICE_MODEL_DEL_NTF_REQ, // ��Ϣ�� CDeviceModelDelNtfReq
    TAS_DEVICE_MODEL_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_MODEL_QRY_REQ,     // ��Ϣ�� CDeviceModelQryReq
    TAS_DEVICE_MODEL_QRY_RSP,     // ��Ϣ�� CDeviceModelQryRsp
    TAS_DEVICE_MODEL_QRY_NTF_REQ, // ��Ϣ�� CDeviceModelQryNtfReq
    TAS_DEVICE_MODEL_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_CAP_INDEX_NAME_GET_REQ, // ��Ϣ�� CDeviceCapIndexNameGetReq  �豸������������(������ƵԴ����)
    TAS_DEVICE_CAP_INDEX_NAME_GET_RSP, // ��Ϣ�� CDeviceCapIndexNameGetRsp
    TAS_DEVICE_CAP_INDEX_NAME_SET_REQ, // ��Ϣ�� CDeviceCapIndexNameSetReq
    TAS_DEVICE_CAP_INDEX_NAME_SET_RSP, // ��Ϣ�� CDeviceCapIndexNameSetRsp
    TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ,  // ��Ϣ�� CDeviceCapIndexNameModNtfReq
    TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP,  // ��Ϣ�� CNtfRsp
    TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ,     // ��Ϣ�� CDeviceCapIndexNameQryReq
    TAS_DEVICE_CAP_INDEX_NAME_QRY_RSP,     // ��Ϣ�� CDeviceCapIndexNameQryRsp
    TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ, // ��Ϣ�� CDeviceCapIndexNameQryNtfReq
    TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_MANUFACTURER_ADD_REQ,  // ��Ϣ�� CDeviceManufacturerAddReq  �豸����ά��
    TAS_DEVICE_MANUFACTURER_ADD_RSP,  // ��Ϣ�� CDeviceManufacturerAddRsp
    TAS_DEVICE_MANUFACTURER_DEL_REQ,  // ��Ϣ�� CDeviceManufacturerDelReq  
    TAS_DEVICE_MANUFACTURER_DEL_RSP,  // ��Ϣ�� CDeviceManufacturerDelRsp
    TAS_DEVICE_MANUFACTURER_QRY_REQ,  // ��Ϣ�� CDeviceManufacturerQryReq
    TAS_DEVICE_MANUFACTURER_QRY_RSP,  // ��Ϣ�� CDeviceManufacturerQryRsp
    TAS_DEVICE_MANUFACTURER_QRY_NTF_REQ,  // ��Ϣ�� CDeviceManufacturerQryNtfReq
    TAS_DEVICE_MANUFACTURER_QRY_NTF_RSP,  // ��Ϣ�� CNtfRsp

    TAS_CUSER_ADD_REQ, // ��Ϣ�� CUserAddReq  CU�û�����
    TAS_CUSER_ADD_RSP, // ��Ϣ�� CUserAddRsp
    TAS_CUSER_ADD_NTF_REQ, // ��Ϣ�� CUserAddNtfReq
    TAS_CUSER_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_DEL_REQ, // ��Ϣ�� CUserDelReq
    TAS_CUSER_DEL_RSP, // ��Ϣ�� CUserDelRsp
    TAS_CUSER_DEL_NTF_REQ, // ��Ϣ�� CUserDelNtfReq
    TAS_CUSER_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_MOD_REQ, // ��Ϣ�� CUserModReq
    TAS_CUSER_MOD_RSP, // ��Ϣ�� CUserModRsp
    TAS_CUSER_MOD_NTF_REQ, // ��Ϣ�� CUserModNtfReq
    TAS_CUSER_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_QRY_REQ, // ��Ϣ�� CUserQryReq
    TAS_CUSER_QRY_RSP, // ��Ϣ�� CUserQryRsp
    TAS_CUSER_QRY_NTF_REQ, // ��Ϣ�� CUserQryNtfReq
    TAS_CUSER_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    TAS_CUSER_DEVICE_ADD_REQ, // ��Ϣ�� CUserDeviceAddReq CU�û��豸����
    TAS_CUSER_DEVICE_ADD_RSP, // ��Ϣ�� CUserDeviceAddRsp
    TAS_CUSER_DEVICE_ADD_NTF_REQ, // ��Ϣ�� CUserDeviceAddNtfReq
    TAS_CUSER_DEVICE_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_DEVICE_DEL_REQ, // ��Ϣ�� CUserDeviceDelReq
    TAS_CUSER_DEVICE_DEL_RSP, // ��Ϣ�� CUserDeviceDelRsp
    TAS_CUSER_DEVICE_DEL_NTF_REQ, // ��Ϣ�� CUserDeviceDelNtfReq
    TAS_CUSER_DEVICE_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_DEVICE_MOD_REQ, // ��Ϣ�� CUserDeviceModReq
    TAS_CUSER_DEVICE_MOD_RSP, // ��Ϣ�� CUserDeviceModRsp
    TAS_CUSER_DEVICE_MOD_NTF_REQ, // ��Ϣ�� CUserDeviceModNtfReq
    TAS_CUSER_DEVICE_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_DEVICE_QRY_REQ, // ��Ϣ�� CUserDeviceQryReq
    TAS_CUSER_DEVICE_QRY_RSP, // ��Ϣ�� CUserDeviceQryRsp
    TAS_CUSER_DEVICE_QRY_NTF_REQ, // ��Ϣ�� CUserDeviceQryNtfReq
    TAS_CUSER_DEVICE_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    TAS_DEVICE_GROUP_ADD_REQ, // ��Ϣ�� CDeviceGroupAddReq �豸���������Ĳ㼶������㼶��������㼶���߾�����
    TAS_DEVICE_GROUP_ADD_RSP, // ��Ϣ�� CDeviceGroupAddRsp
    TAS_DEVICE_GROUP_ADD_NTF_REQ, // ��Ϣ�� CDeviceGroupAddNtfReq
    TAS_DEVICE_GROUP_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_GROUP_DEL_REQ, // ��Ϣ�� CDeviceGroupDelReq
    TAS_DEVICE_GROUP_DEL_RSP, // ��Ϣ�� CDeviceGroupDelRsp
    TAS_DEVICE_GROUP_DEL_NTF_REQ, // ��Ϣ�� CDeviceGroupDelNtfReq
    TAS_DEVICE_GROUP_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_GROUP_MOD_REQ, // ��Ϣ�� CDeviceGroupModReq
    TAS_DEVICE_GROUP_MOD_RSP, // ��Ϣ�� CDeviceGroupModRsp
    TAS_DEVICE_GROUP_MOD_NTF_REQ, // ��Ϣ�� CDeviceGroupModNtfReq
    TAS_DEVICE_GROUP_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_GROUP_QRY_REQ, // ��Ϣ�� CDeviceGroupQryReq
    TAS_DEVICE_GROUP_QRY_RSP, // ��Ϣ�� CDeviceGroupQryRsp
    TAS_DEVICE_GROUP_QRY_NTF_REQ, // ��Ϣ�� CDeviceGroupQryNtfReq
    TAS_DEVICE_GROUP_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_GROUP_DEVICE_ADD_REQ, // ��Ϣ�� CDeviceGroupDeviceAddReq �豸�����豸����
    TAS_DEVICE_GROUP_DEVICE_ADD_RSP, // ��Ϣ�� CDeviceGroupDeviceAddRsp
    TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ, // ��Ϣ�� CDeviceGroupDeviceAddNtfReq
    TAS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
	TAS_DEVICE_GROUP_MOD_4GB_REQ,        //��Ϣ�� CDeviceGroupMod4GbReq
	TAS_DEVICE_GROUP_MOD_4GB_RSP,        //��Ϣ�� CDeviceGroupMod4GbRsp
	TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ, // ��Ϣ�� CDeviceGroupDeviceMod4GbReq 
	TAS_DEVICE_GROUP_DEVICE_MOD_4GB_RSP, // ��Ϣ�� CDeviceGroupDeviceMod4GbRsp
    TAS_DEVICE_GROUP_DEVICE_DEL_REQ, // ��Ϣ�� CDeviceGroupDeviceDelReq
    TAS_DEVICE_GROUP_DEVICE_DEL_RSP, // ��Ϣ�� CDeviceGroupDeviceDelRsp
    TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ, // ��Ϣ�� CDeviceGroupDeviceDelNtfReq
    TAS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
	TAS_DEVICE_GROUP_DEVICE_ORDER_REQ,     // ��Ϣ�� CDeviceGroupDeviceOrderReq
	TAS_DEVICE_GROUP_DEVICE_ORDER_RSP,     // ��Ϣ�� CDeviceGroupDeviceOrderRsp
	TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_REQ, // ��Ϣ�� CDeviceGroupDeviceOrderNtfReq
	TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DEVICE_GROUP_DEVICE_QRY_REQ, // ��Ϣ�� CDeviceGroupDeviceQryReq
    TAS_DEVICE_GROUP_DEVICE_QRY_RSP, // ��Ϣ�� CDeviceGroupDeviceQryRsp
    TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ, // ��Ϣ�� CDeviceGroupDeviceQryNtfReq
    TAS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
	TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ, // ��Ϣ�� CDeviceGroupDeviceQry4GbReq
	TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP, // ��Ϣ�� CDeviceGroupDeviceQry4GbRsp
	TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ, // ��Ϣ�� CDeviceGroupDeviceQry4GbNtfReq
	TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_RSP, // ��Ϣ�� CNtfRsp

	TAS_DEVICE_GROUP_CHILDREN_QRY_REQ,     // ָ���������ӷ�����豸�ݹ��ѯ ��Ϣ�壺CDeviceGroupChildrenQryReq
	TAS_DEVICE_GROUP_CHILDREN_QRY_RSP,     // ��Ϣ�壺CDeviceGroupChildrenQryRsp
	TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_REQ, // ��Ϣ�壺CDeviceGroupChildrenQryNtfReq
	TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_RSP, // ��Ϣ�壺CNtfRsp

	TAS_DEVICE_GBID_ERROR_GET_REQ,       // ��Ϣ�� CDeviceGbidErrGetReq
	TAS_DEVICE_GBID_ERROR_GET_RSP,       // ��Ϣ�� CDeviceGbidErrGetRsp

	TAS_DEVICE_MATCH_GBID_REQ,           //��Ϣ�� CDeviceMatchGbidReq
	TAS_DEVICE_MATCH_GBID_RSP,           //��Ϣ�� CDeviceMatchGbidRsp

	TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ,  //��Ϣ�� CDeviceGrpDevSumNumReq
	TAS_DEVICE_GROUP_DEVICE_SUMNUM_RSP,  //��Ϣ�� CDeviceGrpDevSumNumRsp

	TAS_DEVICE_GBINFO_QRY_REQ,              //��Ϣ�壺CDeviceGbinfoQryReq
	TAS_DEVICE_GBINFO_QRY_RSP,              //��Ϣ�壺CDeviceGbinfoQryRsp
	TAS_DEVICE_GBINFO_QRY_NTF_REQ,          //��Ϣ�壺CDeviceGbinfoQryNtfReq
	TAS_DEVICE_GBINFO_QRY_NTF_RSP,          //��Ϣ�壺CNtfRsp

	TAS_DEVGRP_GBINFO_QRY_REQ,              //��Ϣ�壺CDevgrpGbinfoQryReq
	TAS_DEVGRP_GBINFO_QRY_RSP,              //��Ϣ�壺CDevgrpGbinfoQryRsp
	TAS_DEVGRP_GBINFO_QRY_NTF_REQ,          //��Ϣ�壺CDevgrpGbinfoQryNtfReq
	TAS_DEVGRP_GBINFO_QRY_NTF_RSP,          //��Ϣ�壺CNtfRsp

    TAS_CUSER_GROUP_ADD_REQ, // ��Ϣ�� CUserGroupAddReq  CU�û�����
    TAS_CUSER_GROUP_ADD_RSP, // ��Ϣ�� CUserGroupAddRsp
    TAS_CUSER_GROUP_ADD_NTF_REQ, // ��Ϣ�� CUserGroupAddNtfReq
    TAS_CUSER_GROUP_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_GROUP_DEL_REQ, // ��Ϣ�� CUserGroupDelReq
    TAS_CUSER_GROUP_DEL_RSP, // ��Ϣ�� CUserGroupDelRsp
    TAS_CUSER_GROUP_DEL_NTF_REQ, // ��Ϣ�� CUserGroupDelNtfReq
    TAS_CUSER_GROUP_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_GROUP_MOD_REQ, // ��Ϣ�� CUserGroupModReq
    TAS_CUSER_GROUP_MOD_RSP, // ��Ϣ�� CUserGroupModRsp
    TAS_CUSER_GROUP_MOD_NTF_REQ, // ��Ϣ�� CUserGroupModNtfReq
    TAS_CUSER_GROUP_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_GROUP_QRY_REQ, // ��Ϣ�� CUserGroupQryReq
    TAS_CUSER_GROUP_QRY_RSP, // ��Ϣ�� CUserGroupQryRsp
    TAS_CUSER_GROUP_QRY_NTF_REQ, // ��Ϣ�� CUserGroupQryNtfReq
    TAS_CUSER_GROUP_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    // CU�û������û�����  ֱ���޸�CU�û���������Id���ɣ�����Ҫ������������
    TAS_CUSER_GROUP_DEVICE_ADD_REQ, // ��Ϣ�� CUserGroupDeviceAddReq  CU�û������豸����
    TAS_CUSER_GROUP_DEVICE_ADD_RSP, // ��Ϣ�� CUserGroupDeviceAddRsp
    TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ, // ��Ϣ�� CUserGroupDeviceAddNtfReq
    TAS_CUSER_GROUP_DEVICE_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_GROUP_DEVICE_DEL_REQ, // ��Ϣ�� CUserGroupDeviceDelReq
    TAS_CUSER_GROUP_DEVICE_DEL_RSP, // ��Ϣ�� CUserGroupDeviceDelRsp
    TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ, // ��Ϣ�� CUserGroupDeviceDelNtfReq
    TAS_CUSER_GROUP_DEVICE_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_GROUP_DEVICE_MOD_REQ, // ��Ϣ�� CUserGroupDeviceModReq
    TAS_CUSER_GROUP_DEVICE_MOD_RSP, // ��Ϣ�� CUserGroupDeviceModRsp
    TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ, // ��Ϣ�� CUserGroupDeviceModNtfReq
    TAS_CUSER_GROUP_DEVICE_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_CUSER_GROUP_DEVICE_QRY_REQ, // ��Ϣ�� CUserGroupDeviceQryReq
    TAS_CUSER_GROUP_DEVICE_QRY_RSP, // ��Ϣ�� CUserGroupDeviceQryRsp
    TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ, // ��Ϣ�� CUserGroupDeviceQryNtfReq
    TAS_CUSER_GROUP_DEVICE_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

	// ��ȡָ���û�ָ���豸����������ƵԴ��Ϣ
	TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ,      //��Ϣ�� CUserDevGrpVidsrcQryReq
	TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP,      //��Ϣ�� CUserDevGrpVidsrcQryRsp
	TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ,  //��Ϣ�� CUserDevGrpVidsrcQryNtfReq
	TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_RSP,  //��Ϣ�� CNtfRsp

	// ָ����ϵͳ�豸�����豸������ƵԴ������ѯ
	TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ,   //��Ϣ�� CDevGrpVidsrcTotalnumGetReq
	TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP,   //��Ϣ�� CDevGrpVidsrcTotalnumGetRsp

    TAS_LICENSE_UPDATE_REQ, // ��Ϣ�� CLicenseUpdateReq  license����
    TAS_LICENSE_UPDATE_RSP, // ��Ϣ�� CLicenseUpdateRsp
    TAS_LICENSE_UPDATE_NTF_REQ, // ��Ϣ�� CLicenseUpdateNtfReq
    TAS_LICENSE_UPDATE_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_LICENSE_SET_REQ,    //��Ϣ�� CLicenseSetReq
    TAS_LICENSE_SET_RSP,    //��Ϣ�� CLicenseSetRsp
    TAS_LICENSE_GET_REQ,    // ��Ϣ�� CLicenseGetReq
    TAS_LICENSE_GET_RSP,    // ��Ϣ�� CLicenseGetRsp

    TAS_DOMAIN_RELATION_QRY_REQ,     // ��Ϣ�� CDomainRelationQryReq  ���ϵ��ѯ
    TAS_DOMAIN_RELATION_QRY_RSP,     // ��Ϣ�� CDomainRelationQryRsp
    TAS_DOMAIN_RELATION_QRY_NTF_REQ, // ��Ϣ�� CDomainRelationQryNtfReq
    TAS_DOMAIN_RELATION_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DOMAIN_RELATION_ADD_NTF_REQ, // ��Ϣ�� CDomainRelationAddNtfReq
    TAS_DOMAIN_RELATION_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DOMAIN_RELATION_DEL_NTF_REQ, // ��Ϣ�� CDomainRelationDelNtfReq
    TAS_DOMAIN_RELATION_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DOMAIN_RELATION_MOD_NTF_REQ, // ��Ϣ�� CDomainRelationModNtfReq
    TAS_DOMAIN_RELATION_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_DOMAIN_NAME_GET_REQ,    // ��Ϣ�� CDomainNameGetReq  ������ѯ
    TAS_DOMAIN_NAME_GET_RSP,    // ��Ϣ�� CDomainNameGetRsp

	TAS_GATEWAY_DEL_REQ,        // ��Ϣ�� CGatewayDelReq
	TAS_GATEWAY_DEL_RSP,        // ��Ϣ�� CGatewayDelRsp  ������ΪERR_TAS_NOT_FINISHEDʱ��ͨ��RedisGetProcessValue4Tas��ѭ��ȡ����


	// v2r2b4���ܣ����黮���豸
	// ��ָ�����豸�黮���ָ���û�
	TAS_CUSER_DEVICE_GROUP_ADD_REQ,							// ��Ϣ�� CUserDeviceGroupAddReq
	TAS_CUSER_DEVICE_GROUP_ADD_RSP,							// ��Ϣ�� CUserDeviceGroupAddRsp
	TAS_CUSER_DEVICE_GROUP_DEL_REQ,							// ��Ϣ�� CUserDeviceGroupDelReq
	TAS_CUSER_DEVICE_GROUP_DEL_RSP,							// ��Ϣ�� CUserDeviceGroupDelRsp

	// ��ָ�����豸�黮���ָ���û���
	TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ,					// ��Ϣ�� CUserGroupDeviceGroupAddReq
	TAS_CUSER_GROUP_DEVICE_GROUP_ADD_RSP,					// ��Ϣ�� CUserGroupDeviceGroupAddRsp
	TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ,					// ��Ϣ�� CUserGroupDeviceGroupDelReq
	TAS_CUSER_GROUP_DEVICE_GROUP_DEL_RSP,					// ��Ϣ�� CUserGroupDeviceGroupDelRsp

	// ��ָ�����豸������Ϊ�Զ�������
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ,					// ��Ϣ�� CDeviceAutoAssignToUserSetReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_RSP,					// ��Ϣ�� CDeviceAutoAssignToUserSetRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ,					// ��Ϣ�� CDeviceAutoAssignToUserQryReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP,					// ��Ϣ�� CDeviceAutoAssignToUserQryRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_REQ,				// ��Ϣ�� CDeviceAutoAssignToUserQryNtfReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_RSP,				// ��Ϣ�� CNtfRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ,			// ��Ϣ�� CDeviceAutoAssignToUserGroupSetReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_RSP,			// ��Ϣ�� CDeviceAutoAssignToUserGroupSetRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ,			// ��Ϣ�� CDeviceAutoAssignToUserGroupQryReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP,			// ��Ϣ�� CDeviceAutoAssignToUserGroupQryRsp
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_REQ,		// ��Ϣ�� CDeviceAutoAssignToUserGroupQryNtfReq
	TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_RSP,		// ��Ϣ�� CNtfRsp

	// ��ѯ����Ĵ������
	TAS_TRANSACTION_PROGRESS_GET_REQ,						// ��Ϣ�� CTransactionProgressGetReq
	TAS_TRANSACTION_PROGRESS_GET_RSP,						// ��Ϣ�� CTransactionProgressGetRsp

    ///////// ����3AS����ҵ���� ////////////////////////////////////////////////

    TAS_CONFIG_TAS_SET_REQ,   // ��Ϣ�� CConfigTasSetReq   Tas����(����: a. ��������� b. �������ã�ע�⣺һ��ֻ��������һ��)
    TAS_CONFIG_TAS_SET_RSP,   // ��Ϣ�� CConfigTasSetRsp
    TAS_CONFIG_TAS_GET_REQ,   // ��Ϣ�� CConfigTasGetReq
    TAS_CONFIG_TAS_GET_RSP,   // ��Ϣ�� CConfigTasGetRsp
    TAS_CHILDTAS_ADD_REQ,  // ��Ϣ�� CChildtasAddReq ��������
    TAS_CHILDTAS_ADD_RSP,  // ��Ϣ�� CChildtasAddRsp
    TAS_CHILDTAS_DEL_REQ,  // ��Ϣ�� CChildtasDelReq
    TAS_CHILDTAS_DEL_RSP,  // ��Ϣ�� CChildtasDelRsp
	TAS_CHILDTAS_DEL_ACK,  // ��Ϣ�� ��

    TAS_CTRL_TAS_REQ,        // ��Ϣ�� CCtrlTasReq  Tas����(������a. ��������ͬ����ע�⣺һ��ֻ�������һ��)
    TAS_CTRL_TAS_RSP,        // ��Ϣ�� CCtrlTasRsp

    TAS_TACUSER_ADD_REQ,     // ��Ϣ�� CTacUserAddReq  Tac�û�����
    TAS_TACUSER_ADD_RSP,     // ��Ϣ�� CTacUserAddRsp
    TAS_TACUSER_ADD_NTF_REQ, // ��Ϣ�� CTacUserAddNtfReq
    TAS_TACUSER_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TACUSER_DEL_REQ,     // ��Ϣ�� CTacUserDelReq
    TAS_TACUSER_DEL_RSP,     // ��Ϣ�� CTacUserDelRsp
    TAS_TACUSER_DEL_NTF_REQ, // ��Ϣ�� CTacUserDelNtfReq
    TAS_TACUSER_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TACUSER_MOD_REQ,     // ��Ϣ�� CTacUserModReq
    TAS_TACUSER_MOD_RSP,     // ��Ϣ�� CTacUserModRsp
    TAS_TACUSER_MOD_NTF_REQ, // ��Ϣ�� CTacUserModNtfReq
    TAS_TACUSER_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TACUSER_QRY_REQ,     // ��Ϣ�� CTacUserQryReq
    TAS_TACUSER_QRY_RSP,     // ��Ϣ�� CTacUserQryRsp
    TAS_TACUSER_QRY_NTF_REQ, // ��Ϣ�� CTacUserQryNtfReq
    TAS_TACUSER_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    TAS_SYNCDSTDOMAIN_ADD_REQ,     // ��Ϣ�� CSyncDstDomainAddReq   ����ͬ��Ŀ�������
    TAS_SYNCDSTDOMAIN_ADD_RSP,     // ��Ϣ�� CSyncDstDomainAddRsp
    TAS_SYNCDSTDOMAIN_ADD_NTF_REQ, // ��Ϣ�� CSyncDstDomainAddNtfReq
    TAS_SYNCDSTDOMAIN_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_SYNCDSTDOMAIN_DEL_REQ,     // ��Ϣ�� CSyncDstDomainDelReq
    TAS_SYNCDSTDOMAIN_DEL_RSP,     // ��Ϣ�� CSyncDstDomainDelRsp
    TAS_SYNCDSTDOMAIN_DEL_NTF_REQ, // ��Ϣ�� CSyncDstDomainDelNtfReq
    TAS_SYNCDSTDOMAIN_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_SYNCDSTDOMAIN_MOD_REQ,     // ��Ϣ�� CSyncDstDomainModReq
    TAS_SYNCDSTDOMAIN_MOD_RSP,     // ��Ϣ�� CSyncDstDomainModRsp
    TAS_SYNCDSTDOMAIN_MOD_NTF_REQ, // ��Ϣ�� CSyncDstDomainModNtfReq
    TAS_SYNCDSTDOMAIN_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_SYNCDSTDOMAIN_QRY_REQ,     // ��Ϣ�� CSyncDstDomainQryReq
    TAS_SYNCDSTDOMAIN_QRY_RSP,     // ��Ϣ�� CSyncDstDomainQryRsp
    TAS_SYNCDSTDOMAIN_QRY_NTF_REQ, // ��Ϣ�� CSyncDstDomainQryNtfReq
    TAS_SYNCDSTDOMAIN_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    // ɾ��������ָ����Ե�ͬ��Դ����������ݣ���̬֪ͨͨ�����ϵɾ��֪ͨʵ��
    TAS_SYNCSRCDOMAIN_DEL_REQ,     // ��Ϣ�� CSyncSrcDomainDelReq  ����ͬ����Դ�����
    TAS_SYNCSRCDOMAIN_DEL_RSP,     // ��Ϣ�� CSyncSrcDomainDelRsp

    TAS_TEMP_AUTHORIZATION_ADD_REQ,     // ��Ϣ�� CTempAuthorizationAddReq  ������Ȩ����(��ֹ���λ���)
    TAS_TEMP_AUTHORIZATION_ADD_RSP,     // ��Ϣ�� CTempAuthorizationAddRsp
    TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ, // ��Ϣ�� CTempAuthorizationAddNtfReq
    TAS_TEMP_AUTHORIZATION_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TEMP_AUTHORIZATION_DEL_REQ,     // ��Ϣ�� CTempAuthorizationDelReq
    TAS_TEMP_AUTHORIZATION_DEL_RSP,     // ��Ϣ�� CTempAuthorizationDelRsp
    TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ, // ��Ϣ�� CTempAuthorizationDelNtfReq
    TAS_TEMP_AUTHORIZATION_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TEMP_AUTHORIZATION_MOD_REQ,     // ��Ϣ�� CTempAuthorizationModReq
    TAS_TEMP_AUTHORIZATION_MOD_RSP,     // ��Ϣ�� CTempAuthorizationModRsp
    TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ, // ��Ϣ�� CTempAuthorizationModNtfReq
    TAS_TEMP_AUTHORIZATION_MOD_NTF_RSP, // ��Ϣ�� CNtfRsp
    TAS_TEMP_AUTHORIZATION_QRY_REQ,     // ��Ϣ�� CTempAuthorizationQryReq
    TAS_TEMP_AUTHORIZATION_QRY_RSP,     // ��Ϣ�� CTempAuthorizationQryRsp
    TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ, // ��Ϣ�� CTempAuthorizationQryNtfReq
    TAS_TEMP_AUTHORIZATION_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    TAS_SYNC_TAS_DATA_REQ,  // ��Ϣ�� CSyncTasDataReq   3AS֮������ͬ��(�������¼�ͬ���͵�Ե�ͬ��)
    TAS_SYNC_TAS_DATA_RSP,  // ��Ϣ�� CSyncTasDataRsp

    ///////// ���ڷ�������ܿ���3AS��չ��ҵ��ӿ� ///////////////////////////////////////
    TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ,     // ��Ϣ�� CCuserDevicefullinfoAndPrivilegeQryReq   ��ȡCU�û��豸������Ϣ���豸Ȩ����Ϣ
    TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP,     // ��Ϣ�� CCuserDevicefullinfoAndPrivilegeQryRsp
    TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ, // ��Ϣ�� CCuserDevicefullinfoAndPrivilegeQryNtfReq
    TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

	// ��ȡָ����ƵԴ������ID����������ƵԴ������ip��ַ����γ����Ϣ(��֧�ַ�ҳ����������)
	TAS_VIDSRC_INFO_QRY_REQ,              //��Ϣ�� CVidsrcInfoQryReq
	TAS_VIDSRC_INFO_QRY_RSP,              //��Ϣ�� CVidsrcInfoQryRsp
	TAS_VIDSRC_INFO_QRY_NTF_REQ,          //��Ϣ�� CVidsrcInfoQryNtfReq
	TAS_VIDSRC_INFO_QRY_NTF_RSP,          //��Ϣ�� CNtfRsp

    TAS_CUSER_DATA_VERSION_GET_REQ,  // ��Ϣ�� CCuserDataVersionGetReq ��ȡCU�û����豸���汾��
    TAS_CUSER_DATA_VERSION_GET_RSP,  // ��Ϣ�� CCuserDataVersionGetRsp

	//�Զ����豸����� [3/10/2016 pangubing]
	TAS_DEVTREE_ADD_REQ,     // ��Ϣ�� CDevTreeAddReq 
	TAS_DEVTREE_ADD_RSP,     // ��Ϣ�� CDevTreeAddRsp
	TAS_DEVTREE_MOD_REQ,     // ��Ϣ�� CDevTreeModReq 
	TAS_DEVTREE_MOD_RSP,     // ��Ϣ�� CDevTreeModRsp
	TAS_DEVTREE_DEL_REQ,     // ��Ϣ�� CDevTreeDelReq 
	TAS_DEVTREE_DEL_RSP,     // ��Ϣ�� CDevTreeDelRsp
	TAS_DEVTREE_QRY_REQ,     // ��Ϣ�� CDevTreeQryReq
	TAS_DEVTREE_QRY_RSP,     // ��Ϣ�� CDevTreeQryRsp
	TAS_DEVTREE_QRY_NTF_REQ, // ��Ϣ�� CDevTreeQryNtfReq
	TAS_DEVTREE_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

	TAS_DEVTREE_DEVGROUP_ASSIGN_REQ,     // ��Ϣ�� CDevTreeDevGrpAssignReq 
	TAS_DEVTREE_DEVGROUP_ASSIGN_RSP,     // ��Ϣ�� CDevTreeDevGrpAssignRsp

	// �豸�豸��·����ѯ [3/18/2016 pangubing]
	TAS_DEVICE_GROUPPATH_QRY_REQ,     // ��Ϣ�� CDeviceGrpPathQryReq
	TAS_DEVICE_GROUPPATH_QRY_RSP,     // ��Ϣ�� CDeviceGrpPathQryRsp
	TAS_DEVICE_GROUPPATH_QRY_NTF_REQ, // ��Ϣ�� CDeviceGrpPathQryNtfReq
	TAS_DEVICE_GROUPPATH_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

	// �豸�����޸� [3/30/2016 pangubing]
	TAS_DEV_CAPINFO_MOD_REQ,     // ��Ϣ�� CDevCapInfoModReq 
	TAS_DEV_CAPINFO_MOD_RSP,     // ��Ϣ�� CDevCapInfoModRsp

	// ��̨������� [4/11/2016 pangubing]
	TAS_PTZLOCK_ADD_REQ,     // ��Ϣ�� CPtzLockAddReq 
	TAS_PTZLOCK_ADD_RSP,     // ��Ϣ�� CPtzLockAddRsp
	TAS_PTZLOCK_ADD_NTF_REQ, // ��Ϣ�� CPtzLockAddNtfReq
	TAS_PTZLOCK_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
	TAS_PTZLOCK_DEL_REQ,     // ��Ϣ�� CPtzLockDelReq 
	TAS_PTZLOCK_DEL_RSP,     // ��Ϣ�� CPtzLockDelRsp
	TAS_PTZLOCK_DEL_NTF_REQ, // ��Ϣ�� CPtzLockDelNtfReq
	TAS_PTZLOCK_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
	TAS_PTZLOCK_QRY_REQ,     // ��Ϣ�� CPtzLockQryReq
	TAS_PTZLOCK_QRY_RSP,     // ��Ϣ�� CPtzLockQryRsp
	TAS_PTZLOCK_QRY_NTF_REQ, // ��Ϣ�� CPtzLockQryNtfReq
	TAS_PTZLOCK_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    TAS_EXTERN_MSG_END = TAC_TAS_EVENT_END
};

inline void InitTacTasEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(TAS_LOGIN_REQ);                           // ��Ϣ�� CLoginTasReq  ��¼����
    OSP_ADD_EVENT_DESC(TAS_LOGIN_RSP);                           // ��Ϣ�� CLoginTasRsp
    OSP_ADD_EVENT_DESC(TAS_LOGOUT_REQ);                          // ��Ϣ�� CLogoutTasReq
    OSP_ADD_EVENT_DESC(TAS_LOGOUT_RSP);                          // ��Ϣ�� CLogoutTasRsp
    OSP_ADD_EVENT_DESC(TAS_CU_ONLINE_REQ);                       // ��Ϣ�� CCuOnlineReq  cui��tas����cu��������������Ϊ
    OSP_ADD_EVENT_DESC(TAS_CU_ONLINE_RSP);                       // ��Ϣ�� CCuOnlineRsp
    OSP_ADD_EVENT_DESC(TAS_CU_OFFLINE_REQ);                      // ��Ϣ�� CCuOfflineReq
    OSP_ADD_EVENT_DESC(TAS_CU_OFFLINE_RSP);                      // ��Ϣ�� CCuOfflineRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_ADD_REQ);                      // ��Ϣ�� CDeviceAddReq ͨ���豸����(���������⴦����豸���������ǽ���豸)
    OSP_ADD_EVENT_DESC(TAS_DEVICE_ADD_RSP);                      // ��Ϣ�� CDeviceAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_ADD_NTF_REQ);                  // ��Ϣ�� CDeviceAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_ADD_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_DEL_REQ);                      // ��Ϣ�� CDeviceDelReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_DEL_RSP);                      // ��Ϣ�� CDeviceDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_DEL_NTF_REQ);                  // ��Ϣ�� CDeviceDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_DEL_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MOD_REQ);                      // ��Ϣ�� CDeviceModReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MOD_RSP);                      // ��Ϣ�� CDeviceModRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MOD_NTF_REQ);                  // ��Ϣ�� CDeviceModNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MOD_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_BATCH_CHANGE_REQ);             //��Ϣ�� CTasDeviceBatchChangeReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_BATCH_CHANGE_RSP);             //��Ϣ�� CTasDeviceBatchChangeRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_QRY_REQ);                      // ��Ϣ�� CDeviceQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_QRY_RSP);                      // ��Ϣ�� CDeviceQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_QRY_NTF_REQ);                  // ��Ϣ�� CDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_QRY_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_ADD_REQ);                      // ��Ϣ�� CTvwallAddReq ����ǽ�豸����(����ǽ�еĵ��Ӻͽ������İ󶨹�ϵ�������������ٲ�������������޸İ󶨹�ϵʱ���õ���ǽ�޸��������)
    OSP_ADD_EVENT_DESC(TAS_TVWALL_ADD_RSP);                      // ��Ϣ�� CTvwallAddRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_ADD_NTF_REQ);                  // ��Ϣ�� CTvwallAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_ADD_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_DEL_REQ);                      // ��Ϣ�� CTvwallDelReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_DEL_RSP);                      // ��Ϣ�� CTvwallDelRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_DEL_NTF_REQ);                  // ��Ϣ�� CTvwallDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_DEL_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_MOD_REQ);                      // ��Ϣ�� CTvwallModReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_MOD_RSP);                      // ��Ϣ�� CTvwallModRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_MOD_NTF_REQ);                  // ��Ϣ�� CTvwallModNtfReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_MOD_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_QRY_REQ);                      // ��Ϣ�� CTvwallQryReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_QRY_RSP);                      // ��Ϣ�� CTvwallQryRsp
    OSP_ADD_EVENT_DESC(TAS_TVWALL_QRY_NTF_REQ);                  // ��Ϣ�� CTvwallQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_TVWALL_QRY_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_ADD_REQ);                // ��Ϣ�� CDeviceModelAddReq  �豸�ͺ� ��֧���޸ģ���Ϊ���ܵ���һϵ������޸�
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_ADD_RSP);                // ��Ϣ�� CDeviceModelAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_ADD_NTF_REQ);            // ��Ϣ�� CDeviceModelAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_ADD_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_DEL_REQ);                // ��Ϣ�� CDeviceModelDelReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_DEL_RSP);                // ��Ϣ�� CDeviceModelDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_DEL_NTF_REQ);            // ��Ϣ�� CDeviceModelDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_DEL_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_QRY_REQ);                // ��Ϣ�� CDeviceModelQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_QRY_RSP);                // ��Ϣ�� CDeviceModelQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_QRY_NTF_REQ);            // ��Ϣ�� CDeviceModelQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MODEL_QRY_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_GET_REQ);       // ��Ϣ�� CDeviceCapIndexNameGetReq  �豸������������(������ƵԴ����)
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_GET_RSP);       // ��Ϣ�� CDeviceCapIndexNameGetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_SET_REQ);       // ��Ϣ�� CDeviceCapIndexNameSetReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_SET_RSP);       // ��Ϣ�� CDeviceCapIndexNameSetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ);   // ��Ϣ�� CDeviceCapIndexNameModNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP);   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ);       // ��Ϣ�� CDeviceCapIndexNameQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_QRY_RSP);       // ��Ϣ�� CDeviceCapIndexNameQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ);   // ��Ϣ�� CDeviceCapIndexNameQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP);   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_ADD_REQ);         // ��Ϣ�� CDeviceManufacturerAddReq  �豸����ά��
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_ADD_RSP);         // ��Ϣ�� CDeviceManufacturerAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_DEL_REQ);         // ��Ϣ�� CDeviceManufacturerDelReq  
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_DEL_RSP);         // ��Ϣ�� CDeviceManufacturerDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_QRY_REQ);         // ��Ϣ�� CDeviceManufacturerQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_QRY_RSP);         // ��Ϣ�� CDeviceManufacturerQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_QRY_NTF_REQ);     // ��Ϣ�� CDeviceManufacturerQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MANUFACTURER_QRY_NTF_RSP);     // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_ADD_REQ);                       // ��Ϣ�� CUserAddReq  CU�û�����
    OSP_ADD_EVENT_DESC(TAS_CUSER_ADD_RSP);                       // ��Ϣ�� CUserAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_ADD_NTF_REQ);                   // ��Ϣ�� CUserAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_ADD_NTF_RSP);                   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEL_REQ);                       // ��Ϣ�� CUserDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEL_RSP);                       // ��Ϣ�� CUserDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEL_NTF_REQ);                   // ��Ϣ�� CUserDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEL_NTF_RSP);                   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_MOD_REQ);                       // ��Ϣ�� CUserModReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_MOD_RSP);                       // ��Ϣ�� CUserModRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_MOD_NTF_REQ);                   // ��Ϣ�� CUserModNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_MOD_NTF_RSP);                   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_QRY_REQ);                       // ��Ϣ�� CUserQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_QRY_RSP);                       // ��Ϣ�� CUserQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_QRY_NTF_REQ);                   // ��Ϣ�� CUserQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_QRY_NTF_RSP);                   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_ADD_REQ);                // ��Ϣ�� CUserDeviceAddReq CU�û��豸����
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_ADD_RSP);                // ��Ϣ�� CUserDeviceAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_ADD_NTF_REQ);            // ��Ϣ�� CUserDeviceAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_ADD_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_DEL_REQ);                // ��Ϣ�� CUserDeviceDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_DEL_RSP);                // ��Ϣ�� CUserDeviceDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_DEL_NTF_REQ);            // ��Ϣ�� CUserDeviceDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_DEL_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_MOD_REQ);                // ��Ϣ�� CUserDeviceModReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_MOD_RSP);                // ��Ϣ�� CUserDeviceModRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_MOD_NTF_REQ);            // ��Ϣ�� CUserDeviceModNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_MOD_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_QRY_REQ);                // ��Ϣ�� CUserDeviceQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_QRY_RSP);                // ��Ϣ�� CUserDeviceQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_QRY_NTF_REQ);            // ��Ϣ�� CUserDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_QRY_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_ADD_REQ);                // ��Ϣ�� CDeviceGroupAddReq �豸���������Ĳ㼶������㼶��������㼶���߾�����
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_ADD_RSP);                // ��Ϣ�� CDeviceGroupAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_ADD_NTF_REQ);            // ��Ϣ�� CDeviceGroupAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_ADD_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEL_REQ);                // ��Ϣ�� CDeviceGroupDelReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEL_RSP);                // ��Ϣ�� CDeviceGroupDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEL_NTF_REQ);            // ��Ϣ�� CDeviceGroupDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEL_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_REQ);                // ��Ϣ�� CDeviceGroupModReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_RSP);                // ��Ϣ�� CDeviceGroupModRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_NTF_REQ);            // ��Ϣ�� CDeviceGroupModNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_QRY_REQ);                // ��Ϣ�� CDeviceGroupQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_QRY_RSP);                // ��Ϣ�� CDeviceGroupQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_QRY_NTF_REQ);            // ��Ϣ�� CDeviceGroupQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_QRY_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ADD_REQ);         // ��Ϣ�� CDeviceGroupDeviceAddReq �豸�����豸����
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ADD_RSP);         // ��Ϣ�� CDeviceGroupDeviceAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ);     // ��Ϣ�� CDeviceGroupDeviceAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP);     // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_4GB_REQ);            //��Ϣ�� CDeviceGroupMod4GbReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_MOD_4GB_RSP);            //��Ϣ�� CDeviceGroupMod4GbRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ);     // ��Ϣ�� CDeviceGroupDeviceMod4GbReq 
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_MOD_4GB_RSP);     // ��Ϣ�� CDeviceGroupDeviceMod4GbRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_DEL_REQ);         // ��Ϣ�� CDeviceGroupDeviceDelReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_DEL_RSP);         // ��Ϣ�� CDeviceGroupDeviceDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ);     // ��Ϣ�� CDeviceGroupDeviceDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP);     // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ORDER_REQ);       // ��Ϣ�� CDeviceGroupDeviceOrderReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ORDER_RSP);       // ��Ϣ�� CDeviceGroupDeviceOrderRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_REQ);   // ��Ϣ�� CDeviceGroupDeviceOrderNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_ORDER_NTF_RSP);   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_REQ);         // ��Ϣ�� CDeviceGroupDeviceQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_RSP);         // ��Ϣ�� CDeviceGroupDeviceQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ);     // ��Ϣ�� CDeviceGroupDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP);     // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ);     // ��Ϣ�� CDeviceGroupDeviceQry4GbReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP);     // ��Ϣ�� CDeviceGroupDeviceQry4GbRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ); // ��Ϣ�� CDeviceGroupDeviceQry4GbNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_RSP); // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_CHILDREN_QRY_REQ);       // ָ���������ӷ�����豸�ݹ��ѯ ��Ϣ�壺CDeviceGroupChildrenQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_CHILDREN_QRY_RSP);       // ��Ϣ�壺CDeviceGroupChildrenQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_REQ);   // ��Ϣ�壺CDeviceGroupChildrenQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_RSP);   // ��Ϣ�壺CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBID_ERROR_GET_REQ);           // ��Ϣ�� CDeviceGbidErrGetReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBID_ERROR_GET_RSP);           // ��Ϣ�� CDeviceGbidErrGetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MATCH_GBID_REQ);               //��Ϣ�� CDeviceMatchGbidReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_MATCH_GBID_RSP);               //��Ϣ�� CDeviceMatchGbidRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ);      //��Ϣ�� CDeviceGrpDevSumNumReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUP_DEVICE_SUMNUM_RSP);      //��Ϣ�� CDeviceGrpDevSumNumRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBINFO_QRY_REQ);               //��Ϣ�壺CDeviceGbinfoQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBINFO_QRY_RSP);               //��Ϣ�壺CDeviceGbinfoQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBINFO_QRY_NTF_REQ);           //��Ϣ�壺CDeviceGbinfoQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GBINFO_QRY_NTF_RSP);           //��Ϣ�壺CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_GBINFO_QRY_REQ);               //��Ϣ�壺CDevgrpGbinfoQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_GBINFO_QRY_RSP);               //��Ϣ�壺CDevgrpGbinfoQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_GBINFO_QRY_NTF_REQ);           //��Ϣ�壺CDevgrpGbinfoQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_GBINFO_QRY_NTF_RSP);           //��Ϣ�壺CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_ADD_REQ);                 // ��Ϣ�� CUserGroupAddReq  CU�û�����
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_ADD_RSP);                 // ��Ϣ�� CUserGroupAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_ADD_NTF_REQ);             // ��Ϣ�� CUserGroupAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_ADD_NTF_RSP);             // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEL_REQ);                 // ��Ϣ�� CUserGroupDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEL_RSP);                 // ��Ϣ�� CUserGroupDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEL_NTF_REQ);             // ��Ϣ�� CUserGroupDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEL_NTF_RSP);             // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_MOD_REQ);                 // ��Ϣ�� CUserGroupModReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_MOD_RSP);                 // ��Ϣ�� CUserGroupModRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_MOD_NTF_REQ);             // ��Ϣ�� CUserGroupModNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_MOD_NTF_RSP);             // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_QRY_REQ);                 // ��Ϣ�� CUserGroupQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_QRY_RSP);                 // ��Ϣ�� CUserGroupQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_QRY_NTF_REQ);             // ��Ϣ�� CUserGroupQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_QRY_NTF_RSP);             // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_ADD_REQ);          // ��Ϣ�� CUserGroupDeviceAddReq  CU�û������豸����
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_ADD_RSP);          // ��Ϣ�� CUserGroupDeviceAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ);      // ��Ϣ�� CUserGroupDeviceAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_ADD_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_DEL_REQ);          // ��Ϣ�� CUserGroupDeviceDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_DEL_RSP);          // ��Ϣ�� CUserGroupDeviceDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ);      // ��Ϣ�� CUserGroupDeviceDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_DEL_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_MOD_REQ);          // ��Ϣ�� CUserGroupDeviceModReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_MOD_RSP);          // ��Ϣ�� CUserGroupDeviceModRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ);      // ��Ϣ�� CUserGroupDeviceModNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_MOD_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_QRY_REQ);          // ��Ϣ�� CUserGroupDeviceQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_QRY_RSP);          // ��Ϣ�� CUserGroupDeviceQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ);      // ��Ϣ�� CUserGroupDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_QRY_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ);         //��Ϣ�� CUserDevGrpVidsrcQryReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP);         //��Ϣ�� CUserDevGrpVidsrcQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ);     //��Ϣ�� CUserDevGrpVidsrcQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_RSP);     //��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ);      //��Ϣ�� CDevGrpVidsrcTotalnumGetReq
    OSP_ADD_EVENT_DESC(TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP);      //��Ϣ�� CDevGrpVidsrcTotalnumGetRsp
    OSP_ADD_EVENT_DESC(TAS_LICENSE_UPDATE_REQ);                  // ��Ϣ�� CLicenseUpdateReq  license����
    OSP_ADD_EVENT_DESC(TAS_LICENSE_UPDATE_RSP);                  // ��Ϣ�� CLicenseUpdateRsp
    OSP_ADD_EVENT_DESC(TAS_LICENSE_UPDATE_NTF_REQ);              // ��Ϣ�� CLicenseUpdateNtfReq
    OSP_ADD_EVENT_DESC(TAS_LICENSE_UPDATE_NTF_RSP);              // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_LICENSE_SET_REQ);                     //��Ϣ�� CLicenseSetReq
    OSP_ADD_EVENT_DESC(TAS_LICENSE_SET_RSP);                     //��Ϣ�� CLicenseSetRsp
    OSP_ADD_EVENT_DESC(TAS_LICENSE_GET_REQ);                     // ��Ϣ�� CLicenseGetReq
    OSP_ADD_EVENT_DESC(TAS_LICENSE_GET_RSP);                     // ��Ϣ�� CLicenseGetRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_QRY_REQ);             // ��Ϣ�� CDomainRelationQryReq  ���ϵ��ѯ
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_QRY_RSP);             // ��Ϣ�� CDomainRelationQryRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_QRY_NTF_REQ);         // ��Ϣ�� CDomainRelationQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_QRY_NTF_RSP);         // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_ADD_NTF_REQ);         // ��Ϣ�� CDomainRelationAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_ADD_NTF_RSP);         // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_DEL_NTF_REQ);         // ��Ϣ�� CDomainRelationDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_DEL_NTF_RSP);         // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_MOD_NTF_REQ);         // ��Ϣ�� CDomainRelationModNtfReq
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_RELATION_MOD_NTF_RSP);         // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_NAME_GET_REQ);                 // ��Ϣ�� CDomainNameGetReq  ������ѯ
    OSP_ADD_EVENT_DESC(TAS_DOMAIN_NAME_GET_RSP);                 // ��Ϣ�� CDomainNameGetRsp
    OSP_ADD_EVENT_DESC(TAS_GATEWAY_DEL_REQ);                     // ��Ϣ�� CGatewayDelReq
    OSP_ADD_EVENT_DESC(TAS_GATEWAY_DEL_RSP);                     // ��Ϣ�� CGatewayDelRsp  ������ΪERR_TAS_NOT_FINISHEDʱ��ͨ��RedisGetProcessValue4Tas��ѭ��ȡ����
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_GROUP_ADD_REQ);          // ��Ϣ�� CUserDeviceGroupAddReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_GROUP_ADD_RSP);          // ��Ϣ�� CUserDeviceGroupAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_GROUP_DEL_REQ);          // ��Ϣ�� CUserDeviceGroupDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICE_GROUP_DEL_RSP);          // ��Ϣ�� CUserDeviceGroupDelRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ);    // ��Ϣ�� CUserGroupDeviceGroupAddReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_GROUP_ADD_RSP);    // ��Ϣ�� CUserGroupDeviceGroupAddRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ);    // ��Ϣ�� CUserGroupDeviceGroupDelReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_GROUP_DEVICE_GROUP_DEL_RSP);    // ��Ϣ�� CUserGroupDeviceGroupDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ);  // ��Ϣ�� CDeviceAutoAssignToUserSetReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_RSP);  // ��Ϣ�� CDeviceAutoAssignToUserSetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ);  // ��Ϣ�� CDeviceAutoAssignToUserQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP);  // ��Ϣ�� CDeviceAutoAssignToUserQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_REQ);// ��Ϣ�� CDeviceAutoAssignToUserQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_RSP);// ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ);// ��Ϣ�� CDeviceAutoAssignToUserGroupSetReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_RSP);// ��Ϣ�� CDeviceAutoAssignToUserGroupSetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ);// ��Ϣ�� CDeviceAutoAssignToUserGroupQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP);// ��Ϣ�� CDeviceAutoAssignToUserGroupQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_REQ);// ��Ϣ�� CDeviceAutoAssignToUserGroupQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_RSP);// ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TRANSACTION_PROGRESS_GET_REQ);        // ��Ϣ�� CTransactionProgressGetReq
    OSP_ADD_EVENT_DESC(TAS_TRANSACTION_PROGRESS_GET_RSP);        // ��Ϣ�� CTransactionProgressGetRsp
    OSP_ADD_EVENT_DESC(TAS_CONFIG_TAS_SET_REQ);                  // ��Ϣ�� CConfigTasSetReq   Tas����(����: a. ��������� b. �������ã�ע�⣺һ��ֻ��������һ��)
    OSP_ADD_EVENT_DESC(TAS_CONFIG_TAS_SET_RSP);                  // ��Ϣ�� CConfigTasSetRsp
    OSP_ADD_EVENT_DESC(TAS_CONFIG_TAS_GET_REQ);                  // ��Ϣ�� CConfigTasGetReq
    OSP_ADD_EVENT_DESC(TAS_CONFIG_TAS_GET_RSP);                  // ��Ϣ�� CConfigTasGetRsp
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_ADD_REQ);                    // ��Ϣ�� CChildtasAddReq ��������
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_ADD_RSP);                    // ��Ϣ�� CChildtasAddRsp
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_DEL_REQ);                    // ��Ϣ�� CChildtasDelReq
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_DEL_RSP);                    // ��Ϣ�� CChildtasDelRsp
    OSP_ADD_EVENT_DESC(TAS_CHILDTAS_DEL_ACK);                    // ��Ϣ�� ��
    OSP_ADD_EVENT_DESC(TAS_CTRL_TAS_REQ);                        // ��Ϣ�� CCtrlTasReq  Tas����(������a. ��������ͬ����ע�⣺һ��ֻ�������һ��)
    OSP_ADD_EVENT_DESC(TAS_CTRL_TAS_RSP);                        // ��Ϣ�� CCtrlTasRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_ADD_REQ);                     // ��Ϣ�� CTacUserAddReq  Tac�û�����
    OSP_ADD_EVENT_DESC(TAS_TACUSER_ADD_RSP);                     // ��Ϣ�� CTacUserAddRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_ADD_NTF_REQ);                 // ��Ϣ�� CTacUserAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_ADD_NTF_RSP);                 // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_DEL_REQ);                     // ��Ϣ�� CTacUserDelReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_DEL_RSP);                     // ��Ϣ�� CTacUserDelRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_DEL_NTF_REQ);                 // ��Ϣ�� CTacUserDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_DEL_NTF_RSP);                 // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_MOD_REQ);                     // ��Ϣ�� CTacUserModReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_MOD_RSP);                     // ��Ϣ�� CTacUserModRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_MOD_NTF_REQ);                 // ��Ϣ�� CTacUserModNtfReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_MOD_NTF_RSP);                 // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_QRY_REQ);                     // ��Ϣ�� CTacUserQryReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_QRY_RSP);                     // ��Ϣ�� CTacUserQryRsp
    OSP_ADD_EVENT_DESC(TAS_TACUSER_QRY_NTF_REQ);                 // ��Ϣ�� CTacUserQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_TACUSER_QRY_NTF_RSP);                 // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_ADD_REQ);               // ��Ϣ�� CSyncDstDomainAddReq   ����ͬ��Ŀ�������
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_ADD_RSP);               // ��Ϣ�� CSyncDstDomainAddRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_ADD_NTF_REQ);           // ��Ϣ�� CSyncDstDomainAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_ADD_NTF_RSP);           // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_DEL_REQ);               // ��Ϣ�� CSyncDstDomainDelReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_DEL_RSP);               // ��Ϣ�� CSyncDstDomainDelRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_DEL_NTF_REQ);           // ��Ϣ�� CSyncDstDomainDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_DEL_NTF_RSP);           // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_MOD_REQ);               // ��Ϣ�� CSyncDstDomainModReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_MOD_RSP);               // ��Ϣ�� CSyncDstDomainModRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_MOD_NTF_REQ);           // ��Ϣ�� CSyncDstDomainModNtfReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_MOD_NTF_RSP);           // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_QRY_REQ);               // ��Ϣ�� CSyncDstDomainQryReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_QRY_RSP);               // ��Ϣ�� CSyncDstDomainQryRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_QRY_NTF_REQ);           // ��Ϣ�� CSyncDstDomainQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_SYNCDSTDOMAIN_QRY_NTF_RSP);           // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNCSRCDOMAIN_DEL_REQ);               // ��Ϣ�� CSyncSrcDomainDelReq  ����ͬ����Դ�����
    OSP_ADD_EVENT_DESC(TAS_SYNCSRCDOMAIN_DEL_RSP);               // ��Ϣ�� CSyncSrcDomainDelRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_ADD_REQ);          // ��Ϣ�� CTempAuthorizationAddReq  ������Ȩ����(��ֹ���λ���)
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_ADD_RSP);          // ��Ϣ�� CTempAuthorizationAddRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ);      // ��Ϣ�� CTempAuthorizationAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_ADD_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_DEL_REQ);          // ��Ϣ�� CTempAuthorizationDelReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_DEL_RSP);          // ��Ϣ�� CTempAuthorizationDelRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ);      // ��Ϣ�� CTempAuthorizationDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_DEL_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_MOD_REQ);          // ��Ϣ�� CTempAuthorizationModReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_MOD_RSP);          // ��Ϣ�� CTempAuthorizationModRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ);      // ��Ϣ�� CTempAuthorizationModNtfReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_MOD_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_QRY_REQ);          // ��Ϣ�� CTempAuthorizationQryReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_QRY_RSP);          // ��Ϣ�� CTempAuthorizationQryRsp
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ);      // ��Ϣ�� CTempAuthorizationQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_TEMP_AUTHORIZATION_QRY_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_SYNC_TAS_DATA_REQ);                   // ��Ϣ�� CSyncTasDataReq   3AS֮������ͬ��(�������¼�ͬ���͵�Ե�ͬ��)
    OSP_ADD_EVENT_DESC(TAS_SYNC_TAS_DATA_RSP);                   // ��Ϣ�� CSyncTasDataRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ);// ��Ϣ�� CCuserDevicefullinfoAndPrivilegeQryReq   ��ȡCU�û��豸������Ϣ���豸Ȩ����Ϣ
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP);// ��Ϣ�� CCuserDevicefullinfoAndPrivilegeQryRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ);// ��Ϣ�� CCuserDevicefullinfoAndPrivilegeQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_RSP);// ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_VIDSRC_INFO_QRY_REQ);                 //��Ϣ�� CVidsrcInfoQryReq
    OSP_ADD_EVENT_DESC(TAS_VIDSRC_INFO_QRY_RSP);                 //��Ϣ�� CVidsrcInfoQryRsp
    OSP_ADD_EVENT_DESC(TAS_VIDSRC_INFO_QRY_NTF_REQ);             //��Ϣ�� CVidsrcInfoQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_VIDSRC_INFO_QRY_NTF_RSP);             //��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_CUSER_DATA_VERSION_GET_REQ);          // ��Ϣ�� CCuserDataVersionGetReq ��ȡCU�û����豸���汾��
    OSP_ADD_EVENT_DESC(TAS_CUSER_DATA_VERSION_GET_RSP);          // ��Ϣ�� CCuserDataVersionGetRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_ADD_REQ);                     // ��Ϣ�� CDevTreeAddReq 
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_ADD_RSP);                     // ��Ϣ�� CDevTreeAddRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_MOD_REQ);                     // ��Ϣ�� CDevTreeModReq 
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_MOD_RSP);                     // ��Ϣ�� CDevTreeModRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_DEL_REQ);                     // ��Ϣ�� CDevTreeDelReq 
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_DEL_RSP);                     // ��Ϣ�� CDevTreeDelRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_QRY_REQ);                     // ��Ϣ�� CDevTreeQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_QRY_RSP);                     // ��Ϣ�� CDevTreeQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_QRY_NTF_REQ);                 // ��Ϣ�� CDevTreeQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_QRY_NTF_RSP);                 // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_DEVGROUP_ASSIGN_REQ);         // ��Ϣ�� CDevTreeDevGrpAssignReq 
    OSP_ADD_EVENT_DESC(TAS_DEVTREE_DEVGROUP_ASSIGN_RSP);         // ��Ϣ�� CDevTreeDevGrpAssignRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUPPATH_QRY_REQ);            // ��Ϣ�� CDeviceGrpPathQryReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUPPATH_QRY_RSP);            // ��Ϣ�� CDeviceGrpPathQryRsp
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUPPATH_QRY_NTF_REQ);        // ��Ϣ�� CDeviceGrpPathQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_DEVICE_GROUPPATH_QRY_NTF_RSP);        // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_DEV_CAPINFO_MOD_REQ);                 // ��Ϣ�� CDevCapInfoModReq 
    OSP_ADD_EVENT_DESC(TAS_DEV_CAPINFO_MOD_RSP);                 // ��Ϣ�� CDevCapInfoModRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_ADD_REQ);                     // ��Ϣ�� CPtzLockAddReq 
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_ADD_RSP);                     // ��Ϣ�� CPtzLockAddRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_ADD_NTF_REQ);                 // ��Ϣ�� CPtzLockAddNtfReq
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_ADD_NTF_RSP);                 // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_DEL_REQ);                     // ��Ϣ�� CPtzLockDelReq 
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_DEL_RSP);                     // ��Ϣ�� CPtzLockDelRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_DEL_NTF_REQ);                 // ��Ϣ�� CPtzLockDelNtfReq
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_DEL_NTF_RSP);                 // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_QRY_REQ);                     // ��Ϣ�� CPtzLockQryReq
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_QRY_RSP);                     // ��Ϣ�� CPtzLockQryRsp
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_QRY_NTF_REQ);                 // ��Ϣ�� CPtzLockQryNtfReq
    OSP_ADD_EVENT_DESC(TAS_PTZLOCK_QRY_NTF_RSP);                 // ��Ϣ�� CNtfRsp
}

#endif // _TAS_EVENT_H_





























