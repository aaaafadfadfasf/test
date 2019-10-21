/*****************************************************************************
   ģ����      : gbs_event
   �ļ���      : gbs_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: gbs_event.h ������gbs���ⲿ��ϢID
   ����        : ��־��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/11/25  1.0         huzhiyun   ����
******************************************************************************/
#ifndef _GBS_EVENT_H_
#define _GBS_EVENT_H_

#include "cms/cms_eventid.h"
#include "cms/cms_proto.h"

/* gbsЭ�� */
enum 
{
    GBS_EXTERN_MSG_BEGIN = GBS_EVENT_BEGIN, 

    /* ģ���¼��ʹ��ͳһ�������¼���� DEV_LOGIN_REQ */

    /* ����֪ͨ��Ϣ */
    GBS_PROGRESS_NTF_REQ,  // ��Ϣ�� CGbsProgressNtfReq
    GBS_PROGRESS_NTF_RSP,  // ��Ϣ�� CNtfRsp

    /* �������� */
    GBS_CONF_SET_REQ,    // ��Ϣ�� CGbsConfSetReq
    GBS_CONF_SET_RSP,    // ��Ϣ�� CGbsConfSetRsp
    GBS_CONF_GET_REQ,    // ��Ϣ�� CGbsConfGetReq
    GBS_CONF_GET_RSP,    // ��Ϣ�� CGbsConfGetRsp

    /* �ƴ���ӳ������ */
    GBS_KDDOMAIN_ADD_REQ, // ��Ϣ�� CGbsKddomainAddReq
    GBS_KDDOMAIN_ADD_RSP, // ��Ϣ�� CGbsKddomainAddRsp
    GBS_KDDOMAIN_DEL_REQ, // ��Ϣ�� CGbsKddomainDelReq
    GBS_KDDOMAIN_DEL_RSP, // ��Ϣ�� CGbsKddomainDelRsp
    GBS_KDDOMAIN_QRY_REQ, // ��Ϣ�� CGbsKddomainQryReq
    GBS_KDDOMAIN_QRY_RSP, // ��Ϣ�� CGbsKddomainQryRsp
    GBS_KDDOMAIN_QRY_NTF_REQ, // ��Ϣ�� CGbsKddomainQryNtfReq
    GBS_KDDOMAIN_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    /* ����PU���� */
    GBS_PU_ADD_REQ,  // ��Ϣ�� CGbsPuAddReq
    GBS_PU_ADD_RSP,  // ��Ϣ�� CGbsPuAddRsp
    GBS_PU_DEL_REQ,  // ��Ϣ�� CGbsPuDelReq
    GBS_PU_DEL_RSP,  // ��Ϣ�� CGbsPuDelRsp
    GBS_PU_MOD_REQ,  // ��Ϣ�� CGbsPuModReq
    GBS_PU_MOD_RSP,  // ��Ϣ�� CGbsPuModRsp
    GBS_PU_QRY_REQ,  // ��Ϣ�� CGbsPuQryReq
    GBS_PU_QRY_RSP,  // ��Ϣ�� CGbsPuQryRsp
    GBS_PU_QRY_NTF_REQ, // ��Ϣ�� CGbsPuQryNtfReq
    GBS_PU_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
	GBS_PU_SUPPORT_IDTYPES_GET_REQ, //��Ϣ�� CGbsPuSupportIdTypesGetReq
	GBS_PU_SUPPORT_IDTYPES_GET_RSP, //��Ϣ�� CGbsPuSupportIdTypesGetRsp
	GBS_DECODE_ADD_REQ,  //��Ϣ�� CGbsDecodeAddReq
	GBS_DECODE_ADD_RSP,  //��Ϣ�� CGbsDecodeAddRsp
	GBS_DECODE_DEL_REQ,  //��Ϣ�� CGbsDecodeDelReq
	GBS_DECODE_DEL_RSP,  //��Ϣ�� CGbsDecodeDelRsp
	GBS_DECODE_MOD_REQ,  //��Ϣ�� CGbsDecodeModReq
	GBS_DECODE_MOD_RSP,  //��Ϣ�� CGbsDecodeModRsp
	GBS_DECODE_QRY_REQ,  //��Ϣ�� CGbsDecodeQryReq
	GBS_DECODE_QRY_RSP,  //��Ϣ�� CGbsDecodeQryRsp
	GBS_DECODE_QRY_NTF_REQ, //��Ϣ�� CGbsDecodeQryNtfReq
	GBS_DECODE_QRY_NTF_RSP, //��Ϣ�� CGbsDecodeQryNtfRsp

	/* ������������ */
	GBS_GBPERIPHERAL_ADD_REQ, // ��Ϣ�� CGbsGbPeripheralAddReq
	GBS_GBPERIPHERAL_ADD_RSP, // ��Ϣ�� CGbsGbPeripheralAddRsp
	GBS_GBPERIPHERAL_DEL_REQ, // ��Ϣ�� CGbsGbPeripheralDelReq
	GBS_GBPERIPHERAL_DEL_RSP, // ��Ϣ�� CGbsGbPeripheralDelRsp
	GBS_GBPERIPHERAL_DEL_ACK, // ��Ϣ�� CGbsGbPeripheralDelAck
	GBS_GBPERIPHERAL_MOD_REQ, // ��Ϣ�� CGbsGbPeripheralModReq
	GBS_GBPERIPHERAL_MOD_RSP, // ��Ϣ�� CGbsGbPeripheralModRsp
	GBS_GBPERIPHERAL_QRY_REQ, // ��Ϣ�� CGbsGbPeripheralQryReq
	GBS_GBPERIPHERAL_QRY_RSP, // ��Ϣ�� CGbsGbPeripheralQryRsp
	GBS_GBPERIPHERAL_QRY_NTF_REQ,  // ��Ϣ�� CGbsGbPeripheralQryNtfReq
	GBS_GBPERIPHERAL_QRY_NTF_RSP,  // ��Ϣ�� CNtfRsp

    /* �����¼����� */
    GBS_GBDOMAIN_CHILD_ADD_REQ, // ��Ϣ�� CGbsGbdomainChildAddReq
    GBS_GBDOMAIN_CHILD_ADD_RSP, // ��Ϣ�� CGbsGbdomainChildAddRsp
    GBS_GBDOMAIN_CHILD_DEL_REQ, // ��Ϣ�� CGbsGbdomainChildDelReq  ����ͨ��GBS_PROGRESS_NTF_REQ֪ͨ
    GBS_GBDOMAIN_CHILD_DEL_RSP, // ��Ϣ�� CGbsGbdomainChildDelRsp
    GBS_GBDOMAIN_CHILD_DEL_ACK, // ��Ϣ�� CGbsGbdomainChildDelAck
    GBS_GBDOMAIN_CHILD_DEL_SYNC_REQ, // ��Ϣ�� CGbsGbdomainChildDelSyncReq ֪ͨgbuɾ���¼�����ֹɾ���������¼�����������ɸ���
    GBS_GBDOMAIN_CHILD_DEL_SYNC_RSP, // ��Ϣ�� CGbsGbdomainChildDelSyncRsp
    GBS_GBDOMAIN_CHILD_MOD_REQ, // ��Ϣ�� CGbsGbdomainChildModReq
    GBS_GBDOMAIN_CHILD_MOD_RSP, // ��Ϣ�� CGbsGbdomainChildModRsp
    GBS_GBDOMAIN_CHILD_QRY_REQ, // ��Ϣ�� CGbsGbdomainChildQryReq
    GBS_GBDOMAIN_CHILD_QRY_RSP, // ��Ϣ�� CGbsGbdomainChildQryRsp
    GBS_GBDOMAIN_CHILD_QRY_NTF_REQ, // ��Ϣ�� CGbsGbdomainChildQryNtfReq
    GBS_GBDOMAIN_CHILD_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    /* �����ϼ����� */
    GBS_GBDOMAIN_PARENT_ADD_REQ, // ��Ϣ�� CGbsGbdomainParentAddReq
    GBS_GBDOMAIN_PARENT_ADD_RSP, // ��Ϣ�� CGbsGbdomainParentAddRsp
    GBS_GBDOMAIN_PARENT_DEL_REQ, // ��Ϣ�� CGbsGbdomainParentDelReq
    GBS_GBDOMAIN_PARENT_DEL_RSP, // ��Ϣ�� CGbsGbdomainParentDelRsp
    GBS_GBDOMAIN_PARENT_MOD_REQ, // ��Ϣ�� CGbsGbdomainParentModReq
    GBS_GBDOMAIN_PARENT_MOD_RSP, // ��Ϣ�� CGbsGbdomainParentModRsp
    GBS_GBDOMAIN_PARENT_QRY_REQ, // ��Ϣ�� CGbsGbdomainParentQryReq
    GBS_GBDOMAIN_PARENT_QRY_RSP, // ��Ϣ�� CGbsGbdomainParentQryRsp
    GBS_GBDOMAIN_PARENT_QRY_NTF_REQ, // ��Ϣ�� CGbsGbdomainParentQryNtfReq
    GBS_GBDOMAIN_PARENT_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    /* �����ϼ�Ȩ�޿��� */
    GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_REQ, // ��Ϣ��: CGbsGbdomainParentPrivilegeGetReq
    GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_RSP, // ��Ϣ��: CGbsGbdomainParentPrivilegeGetRsp
    GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_REQ, // ��Ϣ��: CGbsGbdomainParentPrivilegeSetReq
    GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_RSP, // ��Ϣ��: CGbsGbdomainParentPrivilegeSetRsp

    /* �߼����� */
    GBS_ADVANCED_CONF_SET_REQ, // ��Ϣ�� CGbsAdvancedConfSetReq
    GBS_ADVANCED_CONF_SET_RSP, // ��Ϣ�� CGbsAdvancedConfSetRsp
    GBS_ADVANCED_CONF_GET_REQ, // ��Ϣ�� CGbsAdvancedConfGetReq
    GBS_ADVANCED_CONF_GET_RSP, // ��Ϣ�� CGbsAdvancedConfGetRsp

	/* ѡ�����ã�ÿ������1�� */
	GBS_OPTION_CONF_SET_REQ, // ��Ϣ�� CGbsOptionConfSetReq
	GBS_OPTION_CONF_SET_RSP, // ��Ϣ�� CGbsOptionConfSetRsp
	GBS_OPTION_CONF_GET_REQ, // ��Ϣ�� CGbsOptionConfGetReq
	GBS_OPTION_CONF_GET_RSP, // ��Ϣ�� CGbsOptionConfGetRsp

    /* �����豸��ѯ */
    GBS_GBDEV_QRY_REQ, // ��Ϣ�� CGbsGbdevQryReq
    GBS_GBDEV_QRY_RSP, // ��Ϣ�� CGbsGbdevQryRsp
    GBS_GBDEV_QRY_NTF_REQ, // ��Ϣ�� CGbsGbdevQryNtfReq
    GBS_GBDEV_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

	GBS_GBPU_QRY_REQ,  //��Ϣ�� CGbsGbPuQryReq
	GBS_GBPU_QRY_RSP,  //��Ϣ�� CGbsGbPuQryRsp
	GBS_DISC_GBPU_REQ, //��Ϣ�� CGbsDiscGbPuReq
	GBS_DISC_GBPU_RSP, //��Ϣ�� CGbsDiscGbPuRsp

    /* �����Ų�ѯ */
    GBS_DEV_GBID_GET_REQ, // ��Ϣ�� CGbsDevGbidGetReq
    GBS_DEV_GBID_GET_RSP, // ��Ϣ�� CGbsDevGbidGetRsp
	GBS_DEV_GRP_GBID_BATCH_GET_REQ,//��Ϣ�� CGbsDevGrpGbidBatchGetReq
	GBS_DEV_GRP_GBID_BATCH_GET_RSP,//��Ϣ�� CGbsDevGrpGbidBatchGetRsp
	GBS_DEV_GBID_BATCH_GET_REQ,//��Ϣ�� CGbsDevGbidBatchGetReq
	GBS_DEV_GBID_BATCH_GET_RSP,//��Ϣ�� CGbsDevGbidBatchGetRsp
	GBS_DEV_KDID_GET_REQ, // ��Ϣ�� CGbsDevKdidGetReq
	GBS_DEV_KDID_GET_RSP, // ��Ϣ�� CGbsDevKdidGetRsp

	/*�����豸Զ������*/
	GBS_GBDEV_TELEBOOT_REQ, //��Ϣ�壺CGbsGbDevTeleBootReq
	GBS_GBDEV_TELEBOOT_RSP, //��Ϣ�壺CGbsGbDevTeleBootRsp

	GBS_GBDEV_RECCTRL_REQ,  //��Ϣ�壺CGbsGbDevRecCtrlReq
	GBS_GBDEV_RECCTRL_RSP,  //��Ϣ�壺CGbsGbDevRecCtrlRsp

	/*�����ϼ�����Ϣ��ѯ*/ 
	GBS_GBPARENTDOMAIN_CATALOG_LOAD_REQ, //��Ϣ�壺CGbsGbPDCatalogLoadReq
	GBS_GBPARENTDOMAIN_CATALOG_LOAD_RSP, //��Ϣ�壺CGbsGbPDCatalogLoadRsp
	GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_REQ, //��Ϣ�壺CGbsGbPDCLProgressNtfReq
	GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_RSP, //��Ϣ�壺CGbsGbPDCLProgressNtfRsp
	GBS_GBPARENTDOMAIN_GET_REQ, //��Ϣ�壺CGbsGbParentDomainGetReq
	GBS_GBPARENTDOMAIN_GET_RSP, //��Ϣ�壺CGbsGbParentDomainGetRsp

    /* ҵ��������� */
    GBS_BUSINESS_GROUP_ADD_REQ, // ��Ϣ�� CGbsBusinessGroupAddReq
    GBS_BUSINESS_GROUP_ADD_RSP, // ��Ϣ�� CGbsBusinessGroupAddRsp
    GBS_BUSINESS_GROUP_DEL_REQ, // ��Ϣ�� CGbsBusinessGroupDelReq
    GBS_BUSINESS_GROUP_DEL_RSP, // ��Ϣ�� CGbsBusinessGroupDelRsp
    GBS_BUSINESS_GROUP_MOD_REQ, // ��Ϣ�� CGbsBusinessGroupModReq
    GBS_BUSINESS_GROUP_MOD_RSP, // ��Ϣ�� CGbsBusinessGroupModRsp
    GBS_BUSINESS_GROUP_QRY_REQ, // ��Ϣ�� CGbsBusinessGroupQryReq
    GBS_BUSINESS_GROUP_QRY_RSP, // ��Ϣ�� CGbsBusinessGroupQryRsp
    GBS_BUSINESS_GROUP_QRY_NTF_REQ, // ��Ϣ�� CGbsBusinessGroupQryNtfReq
    GBS_BUSINESS_GROUP_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    /* ������֯���� */
    GBS_VIRTUAL_ORGANIZATION_ADD_REQ, // ��Ϣ�� CGbsVirtualOrganizationAddReq
    GBS_VIRTUAL_ORGANIZATION_ADD_RSP, // ��Ϣ�� CGbsVirtualOrganizationAddRsp
    GBS_VIRTUAL_ORGANIZATION_DEL_REQ, // ��Ϣ�� CGbsVirtualOrganizationDelReq
    GBS_VIRTUAL_ORGANIZATION_DEL_RSP, // ��Ϣ�� CGbsVirtualOrganizationDelRsp
    GBS_VIRTUAL_ORGANIZATION_MOD_REQ, // ��Ϣ�� CGbsVirtualOrganizationModReq
    GBS_VIRTUAL_ORGANIZATION_MOD_RSP, // ��Ϣ�� CGbsVirtualOrganizationModRsp
    GBS_VIRTUAL_ORGANIZATION_QRY_REQ, // ��Ϣ�� CGbsVirtualOrganizationQryReq
    GBS_VIRTUAL_ORGANIZATION_QRY_RSP, // ��Ϣ�� CGbsVirtualOrganizationQryRsp
    GBS_VIRTUAL_ORGANIZATION_QRY_NTF_REQ, // ��Ϣ�� CGbsVirtualOrganizationQryNtfReq
    GBS_VIRTUAL_ORGANIZATION_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    /* ���黮������ */
    GBS_ORGANIZATION2DEV_ADD_REQ, // ��Ϣ�� CGbsOrganization2DevAddReq
    GBS_ORGANIZATION2DEV_ADD_RSP, // ��Ϣ�� CGbsOrganization2DevAddRsp
    GBS_ORGANIZATION2DEV_DEL_REQ, // ��Ϣ�� CGbsOrganization2DevDelReq
    GBS_ORGANIZATION2DEV_DEL_RSP, // ��Ϣ�� CGbsOrganization2DevDelRsp
    GBS_ORGANIZATION2DEV_QRY_REQ, // ��Ϣ�� CGbsOrganization2DevQryReq
    GBS_ORGANIZATION2DEV_QRY_RSP, // ��Ϣ�� CGbsOrganization2DevQryRsp
    GBS_ORGANIZATION2DEV_QRY_NTF_REQ, // ��Ϣ�� CGbsOrganization2DevQryNtfReq
    GBS_ORGANIZATION2DEV_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

	//�ֶ�ˢ��Ŀ¼
	GBS_REFRESH_CATALOG_REQ, // ��Ϣ�� CGbsGbRefreshCatalogReq
	GBS_REFRESH_CATALOG_RSP, // ��Ϣ�� CGbsGbRefreshCatalogRsp
	GBS_REFRESH_CATALOG_NTF_REQ, // ��Ϣ�� CGbsGbRefreshCatalogNtfReq
	GBS_REFRESH_CATALOG_NTF_RSP, // ��Ϣ�� CGbsGbRefreshCatalogNtfRsp

	//��ѯ�豸����������
	GBS_DEVGRP_CIVILCODE_GET_REQ, // ��Ϣ�� CGbsDevGrpCivilCodeGetReq
	GBS_DEVGRP_CIVILCODE_GET_RSP, // ��Ϣ�� CGbsDevGrpCivilCodeGetRsp

	//����ѡ�����͡�ȡ��
	GBS_CATALOG_SELECT_REPORT_START_REQ,//��Ϣ�� CGbsCatalogSelectReportStartReq
	GBS_CATALOG_SELECT_REPORT_START_RSP,//��Ϣ�� CGbsCatalogSelectReportStartRsp
	GBS_CATALOG_SELECT_REPORT_CANCEL_REQ,//��Ϣ�� CGbsCatalogSelectReportCancelReq
	GBS_CATALOG_SELECT_REPORT_CANCEL_RSP,//��Ϣ�� CGbsCatalogSelectReportCancelRsp
    //GBNat����
    GBS_NAT_TABLE_ADD_REQ, // ��Ϣ�� CGbsNatTableAddReq
    GBS_NAT_TABLE_ADD_RSP, // ��Ϣ�� CGbsNatTableAddRsp
    GBS_NAT_TABLE_DEL_REQ, // ��Ϣ�� CGbsNatTableDelReq
    GBS_NAT_TABLE_DEL_RSP, // ��Ϣ�� CGbsNatTableDelRsp
    GBS_NAT_TABLE_MOD_REQ, // ��Ϣ�� CGbsNatTableModReq
    GBS_NAT_TABLE_MOD_RSP, // ��Ϣ�� CGbsNatTableModRsp
    GBS_NAT_TABLE_GET_REQ, // ��Ϣ�� CGbsNatTableGetReq
    GBS_NAT_TABLE_GET_RSP, // ��Ϣ�� CGbsNatTableGetRsp
    GBS_NAT_DATA_LIST_GET_REQ, // ��Ϣ�� CGbsNatDataListGetReq 
    GBS_NAT_DATA_LIST_GET_RSP, // ��Ϣ�� CGbsNatDataListGetRsp
    GBS_NAT_DATA_SET_REQ, // ��Ϣ�� CGbsNatDataAddReq  ������ͬ���ֶ��飨Э��,net�ڵ�ip����ʼ�˿�������˿ڣ�ʱ��Ϊ���޸�
    GBS_NAT_DATA_SET_RSP, // ��Ϣ�� CGbsNatDataAddRsp
    GBS_NAT_DATA_DEL_REQ, // ��Ϣ�� CGbsNatDataDelReq
    GBS_NAT_DATA_DEL_RSP, // ��Ϣ�� CGbsNatDataDelRsp
    GBS_NAT_DATA_CONTENT_QRY_REQ, // ��Ϣ�� CGbsNatDataContentQryReq
    GBS_NAT_DATA_CONTENT_QRY_RSP, // ��Ϣ�� CGbsNatDataContentQryRsp
    GBS_NAT_DATA_CONTENT_QRY_NTF_REQ, // ��Ϣ�� CGbsNatDataContentQryNtfReq
    GBS_NAT_DATA_CONTENT_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    GBS_GBPU_NAT_SET_REQ, // ��Ϣ�� CGbsGbpuNatSetReq
    GBS_GBPU_NAT_SET_RSP, // ��Ϣ�� CGbsGbpuNatSetRsp
    GBS_GBPU_NAT_QRY_REQ, // ��Ϣ�� CGbsGbpuNatQryReq
    GBS_GBPU_NAT_QRY_RSP, // ��Ϣ�� CGbsGbpuNatQryRsp
    GBS_GBPU_NAT_QRY_NTF_REQ, // ��Ϣ�� CGbsGbpuNatQryNtfReq
    GBS_GBPU_NAT_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
    GBS_GBPU_NAT_DEL_REQ, // ��Ϣ�� CGbsGbpuNatDelReq
    GBS_GBPU_NAT_DEL_RSP, // ��Ϣ�� CGbsGbpuNatDelRsp

	//gbs�������ӵ�puigb��mts
	GBS_SET_PUIGB_MTS_REQ,      //��Ϣ��: CGbsSetPuiGbMtsReq
    GBS_SET_PUIGB_MTS_RSP,      //��Ϣ��: CGbsSetPuiGbMtsRsp
    
	//gbs ��ȫcu˫����֤Э��
	GBS_SAFE_AUTHINFO_CHECK_FIRST_REQ, //��Ϣ�壺CGbsSafeAuthInfoFirstReq
	GBS_SAFE_AUTHINFO_CHECK_FIRST_RSP, //��Ϣ�壺CGbsSafeAuthInfoFirstRsp
	GBS_SAFE_AUTHINFO_CHECK_SECOND_REQ,//��Ϣ�壺CGbsSafeAuthInfoSecondReq
	GBS_SAFE_AUTHINFO_CHECK_SECOND_RSP,//��Ϣ�壺CGbsSafeAuthInfoSecondRsp

    //֤�����
    GBS_CERTIFICATE_UPLOAD_REQ, //��Ϣ�壺CGbsCertificateUploadReq
    GBS_CERTIFICATE_UPLOAD_RSP, //��Ϣ�壺CGbsCertificateUploadRsp
    GBS_CERTIFICATE_GET_REQ, //��Ϣ�壺CGbsCertificateGetReq
    GBS_CERTIFICATE_GET_RSP, //��Ϣ�壺CGbsCertificateGetRsp
    
	//GBS֧�ֲַ�ʽ����
	GBS_REG_UAS_INFO_QRY_REQ, //��Ϣ�壺CGbsRegUasInfoQryReq
	GBS_REG_UAS_INFO_QRY_RSP, //��Ϣ�壺CGbsRegUasInfoQryRsp
	GBS_REG_UAS_INFO_QRY_NTF_REQ, //��Ϣ�壺CGbsRegUasInfoQryNtfReq
	GBS_REG_UAS_INFO_QRY_NTF_RSP, //��Ϣ�壺CNtfRsp
	GBS_PRIMARYGBS_CATALOG_QRY_REQ, //��Ϣ�壺CPrimaryGbsCatalogQryReq
	GBS_PRIMARYGBS_CATALOG_QRY_RSP, //��Ϣ�壺CPrimaryGbsCatalogQryRsp


    //����ΪGB28181���¼�����: fanxg-20131203
    GB28181_NULL_200OK,   //��Ϣ��: ��

    /*
      ���ܣ�����ע��
      SIP������REGISTER��401, 403, 200OK
      SIP-ContentType: ��
      Ӧ����ƣ�SIPӦ��
      ҵ�����̣�
      uac--REGISTER-->uas
      uac<--401--uas
      uac--REGISTER-->uas
      uac<--200OK--uas
    */
    GB28181_REGISTER_REQ,           //��Ϣ��: ��
    GB28181_REGISTER_RSP,           //��Ϣ��: �ޣ�ֱ��200 OK

    /*
      ���ܣ�����ע��
      SIP������REGISTER��401, 403, 200OK
      SIP-ContentType: ��
      Ӧ����ƣ�SIPӦ��
      ҵ�����̣�
      uac--REGISTER-->uas
      uac<--401--uas
      uac--REGISTER-->uas
      uac<--200OK--uas
    */
    GB28181_UNREGISTER_REQ,         //��Ϣ��: ��
    GB28181_UNREGISTER_RSP,         //��Ϣ��: �ޣ�ֱ��200 OK

    /*
      ���ܣ���������
      SIP������MESSAGE��200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�SIPӦ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
    GB28181_KEEP_LIVE_REQ,          //��Ϣ��: CGBKeepaliveReq
    //GB28181_KEEP_LIVE_RSP = GB28181_NULL_200OK,          //��Ϣ��:  �ޣ�ֱ��200 OK

    /*
      ���ܣ�����INVITE��������ʵʱ�����¼��طš�¼������
      SIP������INVITE, 200OK��ACK
      SIP-ContentType: SDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--INVITE-->uas
      uac<--200OK--uas
      uac--ACK-->uas
    */
    GB28181_INVITE_REQ,             //��Ϣ��: TGBSdp���
    GB28181_INVITE_RSP,             //��Ϣ��: TGBSdp���
    GB28181_INVITE_ACK,             //��Ϣ��: TGBSdp���

    /*
      ���ܣ�����BYE��������ʵʱ�����¼��طš�¼������
      SIP������BYE, 200OK
      SIP-ContentType:��
      Ӧ����ƣ�SIPӦ��
      ҵ�����̣�
      uac--BYE-->uas
      uac<--200OK--uas
    */
    GB28181_BYE_REQ,             //��Ϣ��: ��
    //GB28181_BYE_RSP = GB28181_NULL_200OK,             //��Ϣ��: �ޣ�ֱ��200 OK

    /*
      ���ܣ�����PTZ����
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�SIPӦ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
    GB28181_PTZ_CTRL_REQ,          //��Ϣ��: CGBPTZCmd
    //GB28181_PTZ_CTRL_RSP = GB28181_NULL_200OK,          //��Ϣ��:  �ޣ�ֱ��200 OK

	/*
      ���ܣ�����ǰ��Զ���������ɿ���
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�SIPӦ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
	GB28181_TELEBOOT_CMD,          //��Ϣ�壺CTeleBootCmd
	//GB28181_PTZ_CTRL_RSP = GB28181_NULL_200OK,          //��Ϣ��:  �ޣ�ֱ��200 OK

    /*
      ���ܣ�����RECORD����
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_REC_CTRL_REQ,          //��Ϣ��: CGBRecordReq
    GB28181_REC_CTRL_RSP,          //��Ϣ��: CGBRecordRsp

	/*
	���ܣ�����ؼ�֡����
	SIP������MESSAGE,200OK
	SIP-ContentType:MANSCDP
	Ӧ����ƣ�ҵ��Ӧ��
	ҵ�����̣�
	uac--MESSAGE-->uas
	uac<--200OK--uas
	uac<--MESSAGE--uas
	uac--200OK-->uas

	*/
	GB28181_KEYFRAME_CTRL_REQ,     //��Ϣ�壺CKeyFrameCmd (��˾��չ)
	//GB28181_KEYFRAME_CTRL_RSP == GB28181_NULL_200OK,          //��Ϣ��:�ޣ�ֱ��200OK

	GB28181_IFAMECMD_CTRL_REQ,    //��Ϣ�壺CCGbIFameCmd(�����׼�ؼ�֡��������)

    /*
      ���ܣ�����澯֪ͨ
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_ALARM_OCCUR_NTF_REQ,          //��Ϣ��: CAlarmOccurNotify
    GB28181_ALARM_OCCUR_NTF_RSP,          //��Ϣ��: CGBAlarmOccurRsp

    /*
      ���ܣ������豸Ŀ¼��ѯ
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
      uac<--MESSAGE--uas  (�����ж��֪ͨӦ��)
      uac--200OK-->uas
    */
    GB28181_CATALOG_QRY_REQ,         //��Ϣ��: CGBCatalogQryReq
    GB28181_CATALOG_QRY_RSP,         //��Ϣ��: CGBCataLogQryRsp

    /*
      ���ܣ������豸��Ϣ��ѯ
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_DEV_INFO_QRY_REQ,         //��Ϣ��: CGBDevInfoQryReq
    GB28181_DEV_INFO_QRY_RSP,         //��Ϣ��: CGBDevInfoQryRsp

    /*
      ���ܣ������豸״̬��ѯ
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_DEV_STATUS_QRY_REQ,         //��Ϣ��: CGBDevStatusQryReq
    GB28181_DEV_STATUS_QRY_RSP,         //��Ϣ��: CGBDevStatusQryRsp

    /*
      ���ܣ������豸����Ƶ�ļ�����
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
      uac<--MESSAGE--uas  (�����ж��֪ͨӦ��)
      uac--200OK-->uas
    */
    GB28181_REC_INFO_QRY_REQ,         //��Ϣ��: CGBRecordInfoQryReq
    GB28181_REC_INFO_QRY_RSP,         //��Ϣ��: CGBRecordInfoQryRsp

    /*
      ���ܣ�����VCR����
      SIP������INFO, 200OK
      SIP-ContentType: MANSRTSP
      Ӧ����ƣ�SIPӦ��
      ҵ�����̣�
      uac--INFO-->uas
      uac<--200OK--uas
    */
    GB28181_VCR_CTRL_REQ,          //��Ϣ��: TGBRtsp
    //GB28181_VCR_CTRL_RSP = GB28181_NULL_200OK,          //��Ϣ��:  �ޣ�ֱ��200 OK

    /*
      ���ܣ�����ý��״̬֪ͨ
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�SIPӦ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
    GB28181_MEDIA_STATUS_NTF_REQ,          //��Ϣ��: CGBMediaStatusNotifyReq
    //GB28181_MEDIA_STATUS_NTF_RSP = GB28181_NULL_200OK,          //��Ϣ��:  �ޣ�ֱ��200 OK

     /*
      ���ܣ�����Ŀ¼����
      SIP������SUBSCRIBE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--SUBSCRIBE-->uas
      uac<--200OK--uas
    */
    GB28181_CATALOG_SUBSCRIBE_REQ,          //��Ϣ��: CGBCatalogQryReq
    GB28181_CATALOG_SUBSCRIBE_RSP,          //��Ϣ��: 200OKʱΪCGBCataLogQryRecvRsp��400ʱΪ��
    GB28181_CATALOG_UNSUBSCRIBE_REQ,        //��Ϣ��: CGBCatalogQryReq
    GB28181_CATALOG_UNSUBSCRIBE_RSP,        //��Ϣ��: 200OKʱΪ��


    /*
      ���ܣ�����Ŀ¼֪ͨ
      SIP������NOTIFY, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--NOTIFY-->uas
      uac<--200OK--uas
    */
    GB28181_CATALOG_NTF_REQ,          //��Ϣ��: CGBCataLogQryRsp
    GB28181_CATALOG_NTF_RSP,          //��Ϣ��: CGBCataLogQryRecvRsp

    /*
      ���ܣ�����澯����
      SIP������SUBSCRIBE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--SUBSCRIBE-->uas
      uac<--200OK--uas
    */
    GB28181_ALARM_SUBSCRIBE_REQ,          //��Ϣ��: CGBAlarmSubscribeReq
    GB28181_ALARM_SUBSCRIBE_RSP,          //��Ϣ��: CGBAlarmSubscribeRsp

    /*
      ���ܣ�����澯֪ͨ
      SIP������NOTIFY, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--NOTIFY-->uas
      uac<--200OK--uas
    */
    GB28181_ALARM_NTF_REQ,          //��Ϣ��: CGBAlarmNtfReq
    GB28181_ALARM_NTF_RSP,          //��Ϣ��: CGBAlarmNtfRsp

	/*
      ���ܣ��豸���ò�����ѯ
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
      uac<--MESSAGE--uas  (�����ж��֪ͨӦ��)
      uac--200OK-->uas
    */
	GB28181_PU_CONFIG_DOWNLOAD_REQ,   //��Ϣ�壺CGBConfigDownloadReq
	GB28181_PU_CONFIG_DOWNLOAD_RSP,   //��Ϣ�壺CGBConfigDownloadRsp
	
	/*
      ���ܣ�����͸�����ݶ���
      SIP������SUBSCRIBE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--SUBSCRIBE-->uas
      uac<--200OK--uas
    */
    GB28181_TRANSDATA_SUBSCRIBE_REQ,          //��Ϣ��: CGBTransDataSubscribeReq
    GB28181_TRANSDATA_SUBSCRIBE_RSP,          //��Ϣ��: 200OKʱΪCGBTransDataSubscribeRsp��400ʱΪ��

	/* ���ܣ�͸�������
	   SIP������MESSAGE,200OK
	   SIP-ContentType:MANSCDP
	   Ӧ����ƣ�ҵ��Ӧ��
	   ҵ�����̣�
	   uas--Control-->uac
	   uac--200OK-->uas
	*/
	GB28181_PU_TRANSOPERATE_REQ,  //��Ϣ�壺CGBTransOperateReq
	GB28181_PU_TRANSOPERATE_RSP,  //��Ϣ�壺CGBTransOperateRsp

	/*���ܣ�͸������֪ͨ
	  SIP������NOTIFY, 200OK
	  SIP-ContentType:MANSCDP
	  Ӧ����ƣ�ҵ��Ӧ��
	  ҵ�����̣�
	  uac--NOTIFY-->uas
	  uas--200OK-->uac
	*/
	GB28181_PU_TRANSDATA_NTF_REQ,  //��Ϣ�壺CGBTransDataNtfReq
	GB28181_PU_TRANSDATA_NTF_RSP,  //��Ϣ�壺CGBTransDataNtfRsp

	/*���ܣ������㲥
	  SIP������MESSAGE,200 OK,INVITE,200 OK, ACK
	  SIP-ContentType:MANSCDP  sdp
	  Ӧ����ƣ�ҵ��Ӧ��
	  ҵ�����̣�
	  uac--MESSAGE-->uas
	  uas--200OK-->uac
	  uac--INVITE-->uas
	  uas--200OK-->uac
	  uac--ACK-->uas
	*/
	GB28181_AUDIOCALL_MESSAGE_REQ,	//��Ϣ�壺
	GB28181_AUDIOCALL_MESSAGE_RSP,	//��Ϣ�壺

	/*
      ���ܣ�����GPS���ݶ���
      SIP������SUBSCRIBE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--SUBSCRIBE-->uas
      uac<--200OK--uas
    */
    GB28181_GPSDATA_SUBSCRIBE_REQ,          //��Ϣ��: CGBGpsSubcribeReq
    GB28181_GPSDATA_SUBSCRIBE_RSP,          //��Ϣ��: 200OKʱΪCGBGpsSubscribeRsp��400ʱΪ��

	/*���ܣ�͸������֪ͨ
	  SIP������NOTIFY, 200OK
	  SIP-ContentType:MANSCDP
	  Ӧ����ƣ�ҵ��Ӧ��
	  ҵ�����̣�
	  uac--NOTIFY-->uas
	  uas--200OK-->uac
	*/
	GB28181_GPSDATA_NTF_REQ,  //��Ϣ�壺CGBGpsDataNtfReq
	GB28181_GPSDATA_NTF_RSP,  //��Ϣ�壺CGBGpsDataNtfRsp

    /*
    ���ܣ��豸���ò�������
    SIP������MESSAGE, 200OK
    SIP-ContentType: MANSCDP
    Ӧ����ƣ�ҵ��Ӧ��
    ҵ�����̣�
    uac--MESSAGE-->uas
    uac<--200OK--uas
    uac<--MESSAGE--uas
    uac--200OK-->uas
    */
    GB28181_PU_CONFIG_SET_REQ,  //��Ϣ�壺CGBConfigSetReq    ��˾�ӿ�  ��Query���Ͷ�����Ϊcontrol����
    GB28181_PU_CONFIG_SET_RSP,  //��Ϣ�壺CGBConfigSetRsp
    GB28181_DEVICE_CONFIG_REQ,  //��Ϣ��: CGBDeviceConfigReq ��׼����  Ӧ��ΪGB28181_PU_CONFIG_SET_RSP������result

	/*
      ���ܣ�������ƾ�������
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_GUARD_CTRL_REQ,          //��Ϣ��: CGBAlarmGuardReq
	
	/*
      ���ܣ�������ƾ�������
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
      uac<--MESSAGE--uas
      uac--200OK-->uas
    */
    GB28181_ALARM_RESET_CTRL_REQ,          //��Ϣ��: CGBAlarmResetReq
    GB28181_CTRL_RSP,			           //��Ϣ��: CGBCommonRsp
	
	/*
      ���ܣ�ʵʩ������Կ���֪ͨ
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uac--MESSAGE-->uas
      uac<--200OK--uas
    */
	GB28181_CLIENT_VKEK_NOTIFY_REQ,        //��Ϣ��:CGBClientVkeyNtfReq

	/*
	���ܣ���ȡ����֤��
	SIP������MESSAGE, 200OK
	SIP-ContentType: MANSCDP
	Ӧ����ƣ�ҵ��Ӧ��
	ҵ�����̣�
	uac--MESSAGE-->uas
	uac<--200OK--uas
	*/
	GB28181_GET_CERTIFICATE_REQ,          //��Ϣ�壺CGbGetCertificateReq
	GB28181_GET_CERTIFICATE_RSP,          //��Ϣ�壺CGbGetCertificateRsp

	/*
	���ܣ���ͣ��ȫǰ��ǩ������
	SIP������MESSAGE, 200OK
	SIP-ContentType: MANSCDP
	Ӧ����ƣ�ҵ��Ӧ��
	ҵ�����̣�
	uas--MESSAGE-->uac
	uas<--200OK--uac
	uas<--MESSAGE--uac
	uas--->200OK-->uac
	*/
	GB28181_SIGNATURE_CTRL_REQ,            //��Ϣ�壺CGbSignatrueCtrlReq
	GB28181_SIGNATURE_CTRL_RSP,            //��Ϣ�壺CGbSignatureCtrlRsp

	/*
	���ܣ���ͣ��ȫǰ�˼��ܹ���
	SIP������MESSAGE, 200OK
	SIP-ContentType: MANSCDP
	Ӧ����ƣ�ҵ��Ӧ��
	ҵ�����̣�
	uas--MESSAGE-->uac
	uas<--200OK--uac
	uas<--MESSAGE--uac
	uas--->200OK-->uac
	*/
	GB28181_ENCRYPTION_CTRL_REQ,          //��Ϣ�壺CGbEncryptionCtrlReq
	GB28181_ENCRYPTION_CTRL_RSP,          //��Ϣ�壺CGbEncryptionCtrlRsp
	
	/*
      ���ܣ���׼����GPS���ݶ�����֪ͨ
      SIP������MESSAGE, 200OK
      SIP-ContentType: MANSCDP
      Ӧ����ƣ�ҵ��Ӧ��
      ҵ�����̣�
      uas--SUBSCRIBE-->uac
      uas<--200OK--uac
      uas<--NOTIFY--uac
      uas--200OK-->uac
    */
	GB28181_MOBILEPOS_SUBSCRIBE_REQ,       //��Ϣ�壺CGbMobilePositionSubscribeReq��Ӧ��200OK
	GB28181_MOBILEPOS_NOTIFY_REQ,          //��Ϣ�壺CGbMobilePositionDataNtfReq��Ӧ��200 OK

	GB28181_KDMCAPABILITY_REQ,  // ��Ϣ��: CGbKdmCapabilityReq
	GB28181_KDMCAPABILITY_RSP,  // ��Ϣ��: CGbKdmCapabilityRsp

    GBS_EXTERN_MSG_END
};

inline void InitGbsEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(GBS_PROGRESS_NTF_REQ);                    // ��Ϣ�� CGbsProgressNtfReq
    OSP_ADD_EVENT_DESC(GBS_PROGRESS_NTF_RSP);                    // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_CONF_SET_REQ);                        // ��Ϣ�� CGbsConfSetReq
    OSP_ADD_EVENT_DESC(GBS_CONF_SET_RSP);                        // ��Ϣ�� CGbsConfSetRsp
    OSP_ADD_EVENT_DESC(GBS_CONF_GET_REQ);                        // ��Ϣ�� CGbsConfGetReq
    OSP_ADD_EVENT_DESC(GBS_CONF_GET_RSP);                        // ��Ϣ�� CGbsConfGetRsp
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_ADD_REQ);                    // ��Ϣ�� CGbsKddomainAddReq
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_ADD_RSP);                    // ��Ϣ�� CGbsKddomainAddRsp
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_DEL_REQ);                    // ��Ϣ�� CGbsKddomainDelReq
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_DEL_RSP);                    // ��Ϣ�� CGbsKddomainDelRsp
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_QRY_REQ);                    // ��Ϣ�� CGbsKddomainQryReq
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_QRY_RSP);                    // ��Ϣ�� CGbsKddomainQryRsp
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_QRY_NTF_REQ);                // ��Ϣ�� CGbsKddomainQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_KDDOMAIN_QRY_NTF_RSP);                // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_PU_ADD_REQ);                          // ��Ϣ�� CGbsPuAddReq
    OSP_ADD_EVENT_DESC(GBS_PU_ADD_RSP);                          // ��Ϣ�� CGbsPuAddRsp
    OSP_ADD_EVENT_DESC(GBS_PU_DEL_REQ);                          // ��Ϣ�� CGbsPuDelReq
    OSP_ADD_EVENT_DESC(GBS_PU_DEL_RSP);                          // ��Ϣ�� CGbsPuDelRsp
    OSP_ADD_EVENT_DESC(GBS_PU_MOD_REQ);                          // ��Ϣ�� CGbsPuModReq
    OSP_ADD_EVENT_DESC(GBS_PU_MOD_RSP);                          // ��Ϣ�� CGbsPuModRsp
    OSP_ADD_EVENT_DESC(GBS_PU_QRY_REQ);                          // ��Ϣ�� CGbsPuQryReq
    OSP_ADD_EVENT_DESC(GBS_PU_QRY_RSP);                          // ��Ϣ�� CGbsPuQryRsp
    OSP_ADD_EVENT_DESC(GBS_PU_QRY_NTF_REQ);                      // ��Ϣ�� CGbsPuQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_PU_QRY_NTF_RSP);                      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_PU_SUPPORT_IDTYPES_GET_REQ);          //��Ϣ�� CGbsPuSupportIdTypesGetReq
    OSP_ADD_EVENT_DESC(GBS_PU_SUPPORT_IDTYPES_GET_RSP);          //��Ϣ�� CGbsPuSupportIdTypesGetRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_ADD_REQ);                      //��Ϣ�� CGbsDecodeAddReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_ADD_RSP);                      //��Ϣ�� CGbsDecodeAddRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_DEL_REQ);                      //��Ϣ�� CGbsDecodeDelReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_DEL_RSP);                      //��Ϣ�� CGbsDecodeDelRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_MOD_REQ);                      //��Ϣ�� CGbsDecodeModReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_MOD_RSP);                      //��Ϣ�� CGbsDecodeModRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_QRY_REQ);                      //��Ϣ�� CGbsDecodeQryReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_QRY_RSP);                      //��Ϣ�� CGbsDecodeQryRsp
    OSP_ADD_EVENT_DESC(GBS_DECODE_QRY_NTF_REQ);                  //��Ϣ�� CGbsDecodeQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_DECODE_QRY_NTF_RSP);                  //��Ϣ�� CGbsDecodeQryNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_ADD_REQ);                // ��Ϣ�� CGbsGbPeripheralAddReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_ADD_RSP);                // ��Ϣ�� CGbsGbPeripheralAddRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_DEL_REQ);                // ��Ϣ�� CGbsGbPeripheralDelReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_DEL_RSP);                // ��Ϣ�� CGbsGbPeripheralDelRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_DEL_ACK);                // ��Ϣ�� CGbsGbPeripheralDelAck
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_MOD_REQ);                // ��Ϣ�� CGbsGbPeripheralModReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_MOD_RSP);                // ��Ϣ�� CGbsGbPeripheralModRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_QRY_REQ);                // ��Ϣ�� CGbsGbPeripheralQryReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_QRY_RSP);                // ��Ϣ�� CGbsGbPeripheralQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_QRY_NTF_REQ);            // ��Ϣ�� CGbsGbPeripheralQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBPERIPHERAL_QRY_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_ADD_REQ);              // ��Ϣ�� CGbsGbdomainChildAddReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_ADD_RSP);              // ��Ϣ�� CGbsGbdomainChildAddRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_REQ);              // ��Ϣ�� CGbsGbdomainChildDelReq  ����ͨ��GBS_PROGRESS_NTF_REQ֪ͨ
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_RSP);              // ��Ϣ�� CGbsGbdomainChildDelRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_ACK);              // ��Ϣ�� CGbsGbdomainChildDelAck
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_SYNC_REQ);         // ��Ϣ�� CGbsGbdomainChildDelSyncReq ֪ͨgbuɾ���¼�����ֹɾ���������¼�����������ɸ���
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_DEL_SYNC_RSP);         // ��Ϣ�� CGbsGbdomainChildDelSyncRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_MOD_REQ);              // ��Ϣ�� CGbsGbdomainChildModReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_MOD_RSP);              // ��Ϣ�� CGbsGbdomainChildModRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_QRY_REQ);              // ��Ϣ�� CGbsGbdomainChildQryReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_QRY_RSP);              // ��Ϣ�� CGbsGbdomainChildQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_QRY_NTF_REQ);          // ��Ϣ�� CGbsGbdomainChildQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_CHILD_QRY_NTF_RSP);          // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_ADD_REQ);             // ��Ϣ�� CGbsGbdomainParentAddReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_ADD_RSP);             // ��Ϣ�� CGbsGbdomainParentAddRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_DEL_REQ);             // ��Ϣ�� CGbsGbdomainParentDelReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_DEL_RSP);             // ��Ϣ�� CGbsGbdomainParentDelRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_MOD_REQ);             // ��Ϣ�� CGbsGbdomainParentModReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_MOD_RSP);             // ��Ϣ�� CGbsGbdomainParentModRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_QRY_REQ);             // ��Ϣ�� CGbsGbdomainParentQryReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_QRY_RSP);             // ��Ϣ�� CGbsGbdomainParentQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_QRY_NTF_REQ);         // ��Ϣ�� CGbsGbdomainParentQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_QRY_NTF_RSP);         // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_REQ);   // ��Ϣ��: CGbsGbdomainParentPrivilegeGetReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_RSP);   // ��Ϣ��: CGbsGbdomainParentPrivilegeGetRsp
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_REQ);   // ��Ϣ��: CGbsGbdomainParentPrivilegeSetReq
    OSP_ADD_EVENT_DESC(GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_RSP);   // ��Ϣ��: CGbsGbdomainParentPrivilegeSetRsp
    OSP_ADD_EVENT_DESC(GBS_ADVANCED_CONF_SET_REQ);               // ��Ϣ�� CGbsAdvancedConfSetReq
    OSP_ADD_EVENT_DESC(GBS_ADVANCED_CONF_SET_RSP);               // ��Ϣ�� CGbsAdvancedConfSetRsp
    OSP_ADD_EVENT_DESC(GBS_ADVANCED_CONF_GET_REQ);               // ��Ϣ�� CGbsAdvancedConfGetReq
    OSP_ADD_EVENT_DESC(GBS_ADVANCED_CONF_GET_RSP);               // ��Ϣ�� CGbsAdvancedConfGetRsp
    OSP_ADD_EVENT_DESC(GBS_OPTION_CONF_SET_REQ);                 // ��Ϣ�� CGbsOptionConfSetReq
    OSP_ADD_EVENT_DESC(GBS_OPTION_CONF_SET_RSP);                 // ��Ϣ�� CGbsOptionConfSetRsp
    OSP_ADD_EVENT_DESC(GBS_OPTION_CONF_GET_REQ);                 // ��Ϣ�� CGbsOptionConfGetReq
    OSP_ADD_EVENT_DESC(GBS_OPTION_CONF_GET_RSP);                 // ��Ϣ�� CGbsOptionConfGetRsp
    OSP_ADD_EVENT_DESC(GBS_GBDEV_QRY_REQ);                       // ��Ϣ�� CGbsGbdevQryReq
    OSP_ADD_EVENT_DESC(GBS_GBDEV_QRY_RSP);                       // ��Ϣ�� CGbsGbdevQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBDEV_QRY_NTF_REQ);                   // ��Ϣ�� CGbsGbdevQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBDEV_QRY_NTF_RSP);                   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_QRY_REQ);                        //��Ϣ�� CGbsGbPuQryReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_QRY_RSP);                        //��Ϣ�� CGbsGbPuQryRsp
    OSP_ADD_EVENT_DESC(GBS_DISC_GBPU_REQ);                       //��Ϣ�� CGbsDiscGbPuReq
    OSP_ADD_EVENT_DESC(GBS_DISC_GBPU_RSP);                       //��Ϣ�� CGbsDiscGbPuRsp
    OSP_ADD_EVENT_DESC(GBS_DEV_GBID_GET_REQ);                    // ��Ϣ�� CGbsDevGbidGetReq
    OSP_ADD_EVENT_DESC(GBS_DEV_GBID_GET_RSP);                    // ��Ϣ�� CGbsDevGbidGetRsp
    OSP_ADD_EVENT_DESC(GBS_DEV_GRP_GBID_BATCH_GET_REQ);          //��Ϣ�� CGbsDevGrpGbidBatchGetReq
    OSP_ADD_EVENT_DESC(GBS_DEV_GRP_GBID_BATCH_GET_RSP);          //��Ϣ�� CGbsDevGrpGbidBatchGetRsp
    OSP_ADD_EVENT_DESC(GBS_DEV_GBID_BATCH_GET_REQ);              //��Ϣ�� CGbsDevGbidBatchGetReq
    OSP_ADD_EVENT_DESC(GBS_DEV_GBID_BATCH_GET_RSP);              //��Ϣ�� CGbsDevGbidBatchGetRsp
    OSP_ADD_EVENT_DESC(GBS_DEV_KDID_GET_REQ);                    // ��Ϣ�� CGbsDevKdidGetReq
    OSP_ADD_EVENT_DESC(GBS_DEV_KDID_GET_RSP);                    // ��Ϣ�� CGbsDevKdidGetRsp
    OSP_ADD_EVENT_DESC(GBS_GBDEV_TELEBOOT_REQ);                  //��Ϣ�壺CGbsGbDevTeleBootReq
    OSP_ADD_EVENT_DESC(GBS_GBDEV_TELEBOOT_RSP);                  //��Ϣ�壺CGbsGbDevTeleBootRsp
    OSP_ADD_EVENT_DESC(GBS_GBDEV_RECCTRL_REQ);                   //��Ϣ�壺CGbsGbDevRecCtrlReq
    OSP_ADD_EVENT_DESC(GBS_GBDEV_RECCTRL_RSP);                   //��Ϣ�壺CGbsGbDevRecCtrlRsp
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_CATALOG_LOAD_REQ);     //��Ϣ�壺CGbsGbPDCatalogLoadReq
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_CATALOG_LOAD_RSP);     //��Ϣ�壺CGbsGbPDCatalogLoadRsp
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_REQ);  //��Ϣ�壺CGbsGbPDCLProgressNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_CL_PROGRESS_NTF_RSP);  //��Ϣ�壺CGbsGbPDCLProgressNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_GET_REQ);              //��Ϣ�壺CGbsGbParentDomainGetReq
    OSP_ADD_EVENT_DESC(GBS_GBPARENTDOMAIN_GET_RSP);              //��Ϣ�壺CGbsGbParentDomainGetRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_ADD_REQ);              // ��Ϣ�� CGbsBusinessGroupAddReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_ADD_RSP);              // ��Ϣ�� CGbsBusinessGroupAddRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_DEL_REQ);              // ��Ϣ�� CGbsBusinessGroupDelReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_DEL_RSP);              // ��Ϣ�� CGbsBusinessGroupDelRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_MOD_REQ);              // ��Ϣ�� CGbsBusinessGroupModReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_MOD_RSP);              // ��Ϣ�� CGbsBusinessGroupModRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_QRY_REQ);              // ��Ϣ�� CGbsBusinessGroupQryReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_QRY_RSP);              // ��Ϣ�� CGbsBusinessGroupQryRsp
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_QRY_NTF_REQ);          // ��Ϣ�� CGbsBusinessGroupQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_BUSINESS_GROUP_QRY_NTF_RSP);          // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_ADD_REQ);        // ��Ϣ�� CGbsVirtualOrganizationAddReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_ADD_RSP);        // ��Ϣ�� CGbsVirtualOrganizationAddRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_DEL_REQ);        // ��Ϣ�� CGbsVirtualOrganizationDelReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_DEL_RSP);        // ��Ϣ�� CGbsVirtualOrganizationDelRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_MOD_REQ);        // ��Ϣ�� CGbsVirtualOrganizationModReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_MOD_RSP);        // ��Ϣ�� CGbsVirtualOrganizationModRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_QRY_REQ);        // ��Ϣ�� CGbsVirtualOrganizationQryReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_QRY_RSP);        // ��Ϣ�� CGbsVirtualOrganizationQryRsp
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_QRY_NTF_REQ);    // ��Ϣ�� CGbsVirtualOrganizationQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_VIRTUAL_ORGANIZATION_QRY_NTF_RSP);    // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_ADD_REQ);            // ��Ϣ�� CGbsOrganization2DevAddReq
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_ADD_RSP);            // ��Ϣ�� CGbsOrganization2DevAddRsp
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_DEL_REQ);            // ��Ϣ�� CGbsOrganization2DevDelReq
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_DEL_RSP);            // ��Ϣ�� CGbsOrganization2DevDelRsp
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_QRY_REQ);            // ��Ϣ�� CGbsOrganization2DevQryReq
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_QRY_RSP);            // ��Ϣ�� CGbsOrganization2DevQryRsp
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_QRY_NTF_REQ);        // ��Ϣ�� CGbsOrganization2DevQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_ORGANIZATION2DEV_QRY_NTF_RSP);        // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_REFRESH_CATALOG_REQ);                 // ��Ϣ�� CGbsGbRefreshCatalogReq
    OSP_ADD_EVENT_DESC(GBS_REFRESH_CATALOG_RSP);                 // ��Ϣ�� CGbsGbRefreshCatalogRsp
    OSP_ADD_EVENT_DESC(GBS_REFRESH_CATALOG_NTF_REQ);             // ��Ϣ�� CGbsGbRefreshCatalogNtfReq
    OSP_ADD_EVENT_DESC(GBS_REFRESH_CATALOG_NTF_RSP);             // ��Ϣ�� CGbsGbRefreshCatalogNtfRsp
    OSP_ADD_EVENT_DESC(GBS_DEVGRP_CIVILCODE_GET_REQ);            // ��Ϣ�� CGbsDevGrpCivilCodeGetReq
    OSP_ADD_EVENT_DESC(GBS_DEVGRP_CIVILCODE_GET_RSP);            // ��Ϣ�� CGbsDevGrpCivilCodeGetRsp
    OSP_ADD_EVENT_DESC(GBS_CATALOG_SELECT_REPORT_START_REQ);     //��Ϣ�� CGbsCatalogSelectReportStartReq
    OSP_ADD_EVENT_DESC(GBS_CATALOG_SELECT_REPORT_START_RSP);     //��Ϣ�� CGbsCatalogSelectReportStartRsp
    OSP_ADD_EVENT_DESC(GBS_CATALOG_SELECT_REPORT_CANCEL_REQ);    //��Ϣ�� CGbsCatalogSelectReportCancelReq
    OSP_ADD_EVENT_DESC(GBS_CATALOG_SELECT_REPORT_CANCEL_RSP);    //��Ϣ�� CGbsCatalogSelectReportCancelRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_ADD_REQ);                   // ��Ϣ�� CGbsNatTableAddReq
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_ADD_RSP);                   // ��Ϣ�� CGbsNatTableAddRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_DEL_REQ);                   // ��Ϣ�� CGbsNatTableDelReq
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_DEL_RSP);                   // ��Ϣ�� CGbsNatTableDelRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_MOD_REQ);                   // ��Ϣ�� CGbsNatTableModReq
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_MOD_RSP);                   // ��Ϣ�� CGbsNatTableModRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_GET_REQ);                   // ��Ϣ�� CGbsNatTableGetReq
    OSP_ADD_EVENT_DESC(GBS_NAT_TABLE_GET_RSP);                   // ��Ϣ�� CGbsNatTableGetRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_LIST_GET_REQ);               // ��Ϣ�� CGbsNatDataListGetReq 
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_LIST_GET_RSP);               // ��Ϣ�� CGbsNatDataListGetRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_SET_REQ);                    // ��Ϣ�� CGbsNatDataAddReq  ������ͬ���ֶ��飨Э��,net�ڵ�ip����ʼ�˿�������˿ڣ�ʱ��Ϊ���޸�
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_SET_RSP);                    // ��Ϣ�� CGbsNatDataAddRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_DEL_REQ);                    // ��Ϣ�� CGbsNatDataDelReq
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_DEL_RSP);                    // ��Ϣ�� CGbsNatDataDelRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_CONTENT_QRY_REQ);            // ��Ϣ�� CGbsNatDataContentQryReq
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_CONTENT_QRY_RSP);            // ��Ϣ�� CGbsNatDataContentQryRsp
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_CONTENT_QRY_NTF_REQ);        // ��Ϣ�� CGbsNatDataContentQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_NAT_DATA_CONTENT_QRY_NTF_RSP);        // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_SET_REQ);                    // ��Ϣ�� CGbsGbpuNatSetReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_SET_RSP);                    // ��Ϣ�� CGbsGbpuNatSetRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_QRY_REQ);                    // ��Ϣ�� CGbsGbpuNatQryReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_QRY_RSP);                    // ��Ϣ�� CGbsGbpuNatQryRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_QRY_NTF_REQ);                // ��Ϣ�� CGbsGbpuNatQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_QRY_NTF_RSP);                // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_DEL_REQ);                    // ��Ϣ�� CGbsGbpuNatDelReq
    OSP_ADD_EVENT_DESC(GBS_GBPU_NAT_DEL_RSP);                    // ��Ϣ�� CGbsGbpuNatDelRsp
    OSP_ADD_EVENT_DESC(GBS_SET_PUIGB_MTS_REQ);                   //��Ϣ��: CGbsSetPuiGbMtsReq
    OSP_ADD_EVENT_DESC(GBS_SET_PUIGB_MTS_RSP);                   //��Ϣ��: CGbsSetPuiGbMtsRsp
    OSP_ADD_EVENT_DESC(GBS_SAFE_AUTHINFO_CHECK_FIRST_REQ);       //��Ϣ�壺CGbsSafeAuthInfoFirstReq
    OSP_ADD_EVENT_DESC(GBS_SAFE_AUTHINFO_CHECK_FIRST_RSP);       //��Ϣ�壺CGbsSafeAuthInfoFirstRsp
    OSP_ADD_EVENT_DESC(GBS_SAFE_AUTHINFO_CHECK_SECOND_REQ);      //��Ϣ�壺CGbsSafeAuthInfoSecondReq
    OSP_ADD_EVENT_DESC(GBS_SAFE_AUTHINFO_CHECK_SECOND_RSP);      //��Ϣ�壺CGbsSafeAuthInfoSecondRsp
    OSP_ADD_EVENT_DESC(GBS_CERTIFICATE_UPLOAD_REQ);              //��Ϣ�壺CGbsCertificateUploadReq
    OSP_ADD_EVENT_DESC(GBS_CERTIFICATE_UPLOAD_RSP);              //��Ϣ�壺CGbsCertificateUploadRsp
    OSP_ADD_EVENT_DESC(GBS_CERTIFICATE_GET_REQ);                 //��Ϣ�壺CGbsCertificateGetReq
    OSP_ADD_EVENT_DESC(GBS_CERTIFICATE_GET_RSP);                 //��Ϣ�壺CGbsCertificateGetRsp
    OSP_ADD_EVENT_DESC(GBS_REG_UAS_INFO_QRY_REQ);                //��Ϣ�壺CGbsRegUasInfoQryReq
    OSP_ADD_EVENT_DESC(GBS_REG_UAS_INFO_QRY_RSP);                //��Ϣ�壺CGbsRegUasInfoQryRsp
    OSP_ADD_EVENT_DESC(GBS_REG_UAS_INFO_QRY_NTF_REQ);            //��Ϣ�壺CGbsRegUasInfoQryNtfReq
    OSP_ADD_EVENT_DESC(GBS_REG_UAS_INFO_QRY_NTF_RSP);            //��Ϣ�壺CNtfRsp
    OSP_ADD_EVENT_DESC(GBS_PRIMARYGBS_CATALOG_QRY_REQ);          //��Ϣ�壺CPrimaryGbsCatalogQryReq
    OSP_ADD_EVENT_DESC(GBS_PRIMARYGBS_CATALOG_QRY_RSP);          //��Ϣ�壺CPrimaryGbsCatalogQryRsp
    OSP_ADD_EVENT_DESC(GB28181_NULL_200OK);                      //��Ϣ��: ��
    OSP_ADD_EVENT_DESC(GB28181_REGISTER_REQ);                    //��Ϣ��: ��
    OSP_ADD_EVENT_DESC(GB28181_REGISTER_RSP);                    //��Ϣ��: �ޣ�ֱ��200 OK
    OSP_ADD_EVENT_DESC(GB28181_UNREGISTER_REQ);                  //��Ϣ��: ��
    OSP_ADD_EVENT_DESC(GB28181_UNREGISTER_RSP);                  //��Ϣ��: �ޣ�ֱ��200 OK
    OSP_ADD_EVENT_DESC(GB28181_KEEP_LIVE_REQ);                   //��Ϣ��: CGBKeepaliveReq
    OSP_ADD_EVENT_DESC(GB28181_INVITE_REQ);                      //��Ϣ��: TGBSdp���
    OSP_ADD_EVENT_DESC(GB28181_INVITE_RSP);                      //��Ϣ��: TGBSdp���
    OSP_ADD_EVENT_DESC(GB28181_INVITE_ACK);                      //��Ϣ��: TGBSdp���
    OSP_ADD_EVENT_DESC(GB28181_BYE_REQ);                         //��Ϣ��: ��
    OSP_ADD_EVENT_DESC(GB28181_PTZ_CTRL_REQ);                    //��Ϣ��: CGBPTZCmd
    OSP_ADD_EVENT_DESC(GB28181_TELEBOOT_CMD);                    //��Ϣ�壺CTeleBootCmd
    OSP_ADD_EVENT_DESC(GB28181_REC_CTRL_REQ);                    //��Ϣ��: CGBRecordReq
    OSP_ADD_EVENT_DESC(GB28181_REC_CTRL_RSP);                    //��Ϣ��: CGBRecordRsp
    OSP_ADD_EVENT_DESC(GB28181_KEYFRAME_CTRL_REQ);               //��Ϣ�壺CKeyFrameCmd (��˾��չ)
    OSP_ADD_EVENT_DESC(GB28181_IFAMECMD_CTRL_REQ);               //��Ϣ�壺CCGbIFameCmd(�����׼�ؼ�֡��������)
    OSP_ADD_EVENT_DESC(GB28181_ALARM_OCCUR_NTF_REQ);             //��Ϣ��: CAlarmOccurNotify
    OSP_ADD_EVENT_DESC(GB28181_ALARM_OCCUR_NTF_RSP);             //��Ϣ��: CGBAlarmOccurRsp
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_QRY_REQ);                 //��Ϣ��: CGBCatalogQryReq
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_QRY_RSP);                 //��Ϣ��: CGBCataLogQryRsp
    OSP_ADD_EVENT_DESC(GB28181_DEV_INFO_QRY_REQ);                //��Ϣ��: CGBDevInfoQryReq
    OSP_ADD_EVENT_DESC(GB28181_DEV_INFO_QRY_RSP);                //��Ϣ��: CGBDevInfoQryRsp
    OSP_ADD_EVENT_DESC(GB28181_DEV_STATUS_QRY_REQ);              //��Ϣ��: CGBDevStatusQryReq
    OSP_ADD_EVENT_DESC(GB28181_DEV_STATUS_QRY_RSP);              //��Ϣ��: CGBDevStatusQryRsp
    OSP_ADD_EVENT_DESC(GB28181_REC_INFO_QRY_REQ);                //��Ϣ��: CGBRecordInfoQryReq
    OSP_ADD_EVENT_DESC(GB28181_REC_INFO_QRY_RSP);                //��Ϣ��: CGBRecordInfoQryRsp
    OSP_ADD_EVENT_DESC(GB28181_VCR_CTRL_REQ);                    //��Ϣ��: TGBRtsp
    OSP_ADD_EVENT_DESC(GB28181_MEDIA_STATUS_NTF_REQ);            //��Ϣ��: CGBMediaStatusNotifyReq
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_SUBSCRIBE_REQ);           //��Ϣ��: CGBCatalogQryReq
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_SUBSCRIBE_RSP);           //��Ϣ��: 200OKʱΪCGBCataLogQryRecvRsp��400ʱΪ��
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_UNSUBSCRIBE_REQ);         //��Ϣ��: CGBCatalogQryReq
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_UNSUBSCRIBE_RSP);         //��Ϣ��: 200OKʱΪ��
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_NTF_REQ);                 //��Ϣ��: CGBCataLogQryRsp
    OSP_ADD_EVENT_DESC(GB28181_CATALOG_NTF_RSP);                 //��Ϣ��: CGBCataLogQryRecvRsp
    OSP_ADD_EVENT_DESC(GB28181_ALARM_SUBSCRIBE_REQ);             //��Ϣ��: CGBAlarmSubscribeReq
    OSP_ADD_EVENT_DESC(GB28181_ALARM_SUBSCRIBE_RSP);             //��Ϣ��: CGBAlarmSubscribeRsp
    OSP_ADD_EVENT_DESC(GB28181_ALARM_NTF_REQ);                   //��Ϣ��: CGBAlarmNtfReq
    OSP_ADD_EVENT_DESC(GB28181_ALARM_NTF_RSP);                   //��Ϣ��: CGBAlarmNtfRsp
    OSP_ADD_EVENT_DESC(GB28181_PU_CONFIG_DOWNLOAD_REQ);          //��Ϣ�壺CGBConfigDownloadReq
    OSP_ADD_EVENT_DESC(GB28181_PU_CONFIG_DOWNLOAD_RSP);          //��Ϣ�壺CGBConfigDownloadRsp
    OSP_ADD_EVENT_DESC(GB28181_TRANSDATA_SUBSCRIBE_REQ);         //��Ϣ��: CGBTransDataSubscribeReq
    OSP_ADD_EVENT_DESC(GB28181_TRANSDATA_SUBSCRIBE_RSP);         //��Ϣ��: 200OKʱΪCGBTransDataSubscribeRsp��400ʱΪ��
    OSP_ADD_EVENT_DESC(GB28181_PU_TRANSOPERATE_REQ);             //��Ϣ�壺CGBTransOperateReq
    OSP_ADD_EVENT_DESC(GB28181_PU_TRANSOPERATE_RSP);             //��Ϣ�壺CGBTransOperateRsp
    OSP_ADD_EVENT_DESC(GB28181_PU_TRANSDATA_NTF_REQ);            //��Ϣ�壺CGBTransDataNtfReq
    OSP_ADD_EVENT_DESC(GB28181_PU_TRANSDATA_NTF_RSP);            //��Ϣ�壺CGBTransDataNtfRsp
    OSP_ADD_EVENT_DESC(GB28181_AUDIOCALL_MESSAGE_REQ);           //��Ϣ�壺
    OSP_ADD_EVENT_DESC(GB28181_AUDIOCALL_MESSAGE_RSP);           //��Ϣ�壺
    OSP_ADD_EVENT_DESC(GB28181_GPSDATA_SUBSCRIBE_REQ);           //��Ϣ��: CGBGpsSubcribeReq
    OSP_ADD_EVENT_DESC(GB28181_GPSDATA_SUBSCRIBE_RSP);           //��Ϣ��: 200OKʱΪCGBGpsSubscribeRsp��400ʱΪ��
    OSP_ADD_EVENT_DESC(GB28181_GPSDATA_NTF_REQ);                 //��Ϣ�壺CGBGpsDataNtfReq
    OSP_ADD_EVENT_DESC(GB28181_GPSDATA_NTF_RSP);                 //��Ϣ�壺CGBGpsDataNtfRsp
    OSP_ADD_EVENT_DESC(GB28181_PU_CONFIG_SET_REQ);               //��Ϣ�壺CGBConfigSetReq    ��˾�ӿ�  ��Query���Ͷ�����Ϊcontrol����
    OSP_ADD_EVENT_DESC(GB28181_PU_CONFIG_SET_RSP);               //��Ϣ�壺CGBConfigSetRsp
    OSP_ADD_EVENT_DESC(GB28181_DEVICE_CONFIG_REQ);               //��Ϣ��: CGBDeviceConfigReq ��׼����  Ӧ��ΪGB28181_PU_CONFIG_SET_RSP������result
    OSP_ADD_EVENT_DESC(GB28181_GUARD_CTRL_REQ);                  //��Ϣ��: CGBAlarmGuardReq
    OSP_ADD_EVENT_DESC(GB28181_ALARM_RESET_CTRL_REQ);            //��Ϣ��: CGBAlarmResetReq
    OSP_ADD_EVENT_DESC(GB28181_CTRL_RSP);                        //��Ϣ��: CGBCommonRsp
    OSP_ADD_EVENT_DESC(GB28181_CLIENT_VKEK_NOTIFY_REQ);          //��Ϣ��:CGBClientVkeyNtfReq
    OSP_ADD_EVENT_DESC(GB28181_GET_CERTIFICATE_REQ);             //��Ϣ�壺CGbGetCertificateReq
    OSP_ADD_EVENT_DESC(GB28181_GET_CERTIFICATE_RSP);             //��Ϣ�壺CGbGetCertificateRsp
    OSP_ADD_EVENT_DESC(GB28181_SIGNATURE_CTRL_REQ);              //��Ϣ�壺CGbSignatrueCtrlReq
    OSP_ADD_EVENT_DESC(GB28181_SIGNATURE_CTRL_RSP);              //��Ϣ�壺CGbSignatureCtrlRsp
    OSP_ADD_EVENT_DESC(GB28181_ENCRYPTION_CTRL_REQ);             //��Ϣ�壺CGbEncryptionCtrlReq
    OSP_ADD_EVENT_DESC(GB28181_ENCRYPTION_CTRL_RSP);             //��Ϣ�壺CGbEncryptionCtrlRsp
    OSP_ADD_EVENT_DESC(GB28181_MOBILEPOS_SUBSCRIBE_REQ);         //��Ϣ�壺CGbMobilePositionSubscribeReq��Ӧ��200OK
    OSP_ADD_EVENT_DESC(GB28181_MOBILEPOS_NOTIFY_REQ);            //��Ϣ�壺CGbMobilePositionDataNtfReq��Ӧ��200 OK
    OSP_ADD_EVENT_DESC(GB28181_KDMCAPABILITY_REQ);               // ��Ϣ��: CGbKdmCapabilityReq
    OSP_ADD_EVENT_DESC(GB28181_KDMCAPABILITY_RSP);               // ��Ϣ��: CGbKdmCapabilityRsp
}

#endif // _GBS_EVENT_H_














