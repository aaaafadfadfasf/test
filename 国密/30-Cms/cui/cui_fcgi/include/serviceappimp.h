/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : serviceappimp.
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cui_fcgi����
   ����        : ��  ��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/09/23  1.0         ��  ��        ����
******************************************************************************/
#ifndef _SERVICE_APP_IMP_H_
#define _SERVICE_APP_IMP_H_

#include <string>
#include "kdvtype.h"
#include "../../common/cuiproto.h"

string GetCuiServerIpStr();
u16 GetCuiServerTcpListenPort();
u32 GetCuiServerTimeout();  //��λ��
u32 GetCuiServerReadWriteTimeout();
/*====================================================================
��������CuLogin
���ܣ�CU��¼����֤
�㷨ʵ�֣�
����ȫ�ֱ�����
�������˵����CCuLoginMsg &cLoginReq �û���������
             CLoginRsp &cloginRsp ��¼���
����ֵ˵�����ɹ�����TRUE��ʧ�ܷ���FALSE
====================================================================*/
s32 CuLogin(CCuiCuLoginReq &cLoginReq, CCuiCuLoginRsp &cLoginRsp);

/*====================================================================
��������CuLogout
���ܣ�CU�˳���¼
�㷨ʵ�֣�
����ȫ�ֱ�����
�������˵����CLogoutRsp &logoutRsp �˳����
����ֵ˵�����ɹ�����TRUE��ʧ�ܷ���FALSE
====================================================================*/
s32 CuLogout(CCuLogoutReq &cLogoutReq, CCuLogoutRsp &cLogoutRsp);

/*====================================================================
��������CuGetNotify
���ܣ������ж���1��CU���2����ȡƽ̨����֪ͨ��Ϣ
�㷨ʵ�֣�
����ȫ�ֱ�����
�������˵����CGetNotifyReq &cGetNotifyReq  �û���SessionID
             CGetNotifyRsp &cGetNotifyRsp  ���ص�������Ϣ�����û��������Ϣ��Ϊ�սṹ��
����ֵ˵�����ɹ�����TRUE��ʧ�ܷ���FALSE
====================================================================*/
s32 CuGetNotify(CGetNotifyReq &cGetNotifyReq, CGetNotifyRsp &cGetNotifyRsp);

/*====================================================================
������      : CuGetCuiList
����        : ��ȡ��������CUI�б�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/1/7/             liangli
====================================================================*/
s32 CuGetCuiList(CCuGetCuiListReq &cCuGetCuiListReq, CCuGetCuiListRsp &cCuGetCuiListRsp);

/*====================================================================
������      : CuPtzCtrl
����        : ��ǰ������ͷ���п���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/1/31/             liangli
====================================================================*/
s32 CuPtzCtrl(CPuPtzCtrlReq &cCuPtzCtrlReq, CPuPtzCtrlRsp &cCuPtzCtrlRsp);

/*====================================================================
������      : CuPtzLock
����        : ��ǰ������ͷ���м���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/1/             liangli
====================================================================*/
s32 CuPtzLock(CPuPtzLockReq &cCuPtzLockReq, CPuPtzLockRsp &cCuPtzLockRsp);				// for cmu
s32 CuPtzLock(CCuPtzLockAddReq &cCuPtzLockAddReq, CCuPtzLockAddRsp &cCuPtzLockAddRsp);	// for tas

/*====================================================================
������      : CuPtzUnlock
����        : ��ǰ������ͷ���н���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/1/             liangli
====================================================================*/
s32 CuPtzUnlock(CPuPtzUnlockReq &cCuPtzUnlockReq, CPuPtzUnlockRsp &cCuPtzUnlockRsp);		// for cmu
s32 CuPtzUnlock(CCuPtzLockDelReq &cCuPtzLockDelReq, CCuPtzLockDelRsp &cCuPtzLockDelRsp);	// for tas

/*====================================================================
������      : CuSendTransData
����        : ����͸����������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/1/             liangli
====================================================================*/
s32 CuSendTransData(CCuPuTransparentOperateReq &cSendDataReq, CCuPuTransparentOperateRsp &cSendDataRsp);

/*====================================================================
������      : CuQueryTransData
����        : query͸����������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/1/             liangli
====================================================================*/
s32 CuQueryTransData(CCuPuTransparentQryReq &cQryReq, CCuPuTransparentQryRsp &cQryRsp);

/*====================================================================
������      : CuGetSelfUserInfo
����        : CU��ѯ������û���Ϣ
�㷨ʵ��    : CU��ѯ�����û���Ϣ���Լ���ѯ��Ȩ�޵������û���Ϣ����3AS����ͬһ������
             ���Բ�����CuiServer�����������ֲ�ѯ��ֻ����cui_fcgi������
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/2/             liangli
====================================================================*/
s32 CuGetUserInfo(CCuQueryUserReq &cCuQueryUserReq, CCuQueryUserRsp &cCuQueryUserRsp);

/*====================================================================
������      : CuGetSelfUserGroupInfo
����        : CU��ѯ������û�����Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/5/             liangli
====================================================================*/
s32 CuGetUserGroupInfo(CCuUserGroupQryReq &cCuQueryUserGroupReq, CCuUserGroupQryRsp &cCuQueryUserGroupRsp);

/*====================================================================
������      : CuGetUserData
����        : ��ȡCU�Զ�������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/5/             liangli
====================================================================*/
s32 CuGetUserData(CCuUserCustomDataQryReq &cReq, CCuUserCustomDataQryRsp &cRsp);

/*====================================================================
������      : CuSetUserData
����        : ����CU�Զ�������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/5/             liangli
====================================================================*/
s32 CuSetUserData(CCuUserCustomDataAddReq &cReq, CCuUserCustomDataAddRsp &cRsp);

s32 CuDelUserData(CCuUserCustomDataDelReq &cReq, CCuUserCustomDataDelRsp &cRsp);

/*====================================================================
������      : CuGetUserDevGroup
����        : ��ȡ�û�ӵ�е��豸�飬��ֻ��ȡ�û��豸���е����ڵ㣬������Ҷ�ӽڵ�
              ��ȡ���豸�����С�hasDev���ֶΣ���ʶ�Ƿ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/6/             liangli
====================================================================*/
s32 CuGetUserDevGroup(CCuUserDevGroupQryReq &cCuGetUserDevGroupReq, CCuUserDevGroupQryRsp &cCuGetUserDevGroupRsp);

/*====================================================================
������      : CuGetUserDevFullInfo
����        : ��ȡ�û��豸������Ϣ�б�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/6/             liangli
====================================================================*/
s32 CuGetUserDevFullInfo(CCuUserDevFullInfoQryReq &cCuGetUserDevFullInfoReq, CCuUserDevFullInfoQryRsp &cCuGetUserDevFullInfoRsp);

/*====================================================================
������      : CuSubscribePuStatus
����        : ����pu״̬��������ܶ���pu�Ĳ�ͬ״̬
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/7/             liangli
====================================================================*/
s32 CuSubscribePuStatus(CCuSubscribeNotifyReq &cCuSubscribeNotifyReq, CCuSubscribeNotifyRsp &cCuSubscribeNotifyRsp);

/*====================================================================
������      : CuUnsubscribePuStatus
����        : ȡ��PU״̬����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/7/             liangli
====================================================================*/
s32 CuUnsubscribePuStatus(CCuUnSubscribeNotifyReq &cCuUnSubscribePuStatusReq, CCuUnSubscribeNotifyRsp &cCuUnSubscribePuStatusRsp);

/*====================================================================
������      : CuGetDeviceGroupOfDeviceInfo
����        : ��ȡ�豸���豸��Ķ�Ӧ��ϵ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
s32 CuGetDeviceGroupOfDeviceInfo(CCuQryDevGroupOfDevInfoReq &cCuQryDevGroupOfDevInfoReq, CCuQryDevGroupOfDevInfoRsp &cCuQryDevGroupOfDevInfoRsp);

/*====================================================================
������      : CuCreateDeviceGroup
����        : �����豸��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
s32 CuCreateDeviceGroup(CCuCreateDeviceGroupReq &cCuCreateDeviceGroupReq, CCuCreateDeviceGroupRsp &cCuCreateDeviceGroupRsp);

/*====================================================================
������      : CuDeleteDeviceGroup
����        : ɾ���豸��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
s32 CuDeleteDeviceGroup(CCuDeleteDeviceGroupReq &cCuDeleteDeviceGroupReq, CCuDeleteDeviceGroupRsp &cCuDeleteDeviceGroupRsp);

/*====================================================================
������      : CuModifyDeviceGroup
����        : �޸��豸����Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
s32 CuModifyDeviceGroup(CCuModifyDeviceGroupReq &cCuModifyDeviceGroupReq, CCuModifyDeviceGroupRsp &cCuModifyDeviceGroupRsp);

/*====================================================================
������      : CuDeviceGroupAddDevice
����        : �豸��������豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
s32 CuDeviceGroupAddDevice(CCuDeviceGroupAddDeviceReq &cCuDeviceGroupAddDeviceReq, CCuDeviceGroupAddDeviceRsp &cCuDeviceGroupAddDeviceRsp);

s32 CuDeviceGroupAddGroup(CCuDevTreeDevGrpAssignReq &cCuDevTreeDevGrpAssignReq, CCuDevTreeDevGrpAssignRsp &cCuDevTreeDevGrpAssignRsp);
/*====================================================================
������      : CuDeviceGroupDelDevice
����        : �豸��������豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/13/             liangli
====================================================================*/
s32 CuDeviceGroupDelDevice(CCuDeviceGroupDelDeviceReq &cCuDeviceGroupDelDeviceReq, CCuDeviceGroupDelDeviceRsp &cCuDeviceGroupDelDeviceRsp);

/*====================================================================
������      : CuCreateUserGroup
����        : �����µ��û���
�㷨ʵ��    : �û���û�����Ĳ㼶�ṹ�����û��������û���ֻ�����û�����
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuCreateUserGroup(CCuCreateUserGroupReq &cCuCreateUserGroupReq, CCuCreateUserGroupRsp &cCuCreateUserGroupRsp);

/*====================================================================
������      : CuDeleteUserGroup
����        : ɾ���û���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuDeleteUserGroup(CCuDeleteUserGroupReq &cCuDeleteUserGroupReq, CCuDeleteUserGroupRsp &cCuDeleteUserGroupRsp);

/*====================================================================
������      : CuModifyUserGroup
����        : �޸��û��������Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuModifyUserGroup(CCuModifyUserGroupReq &cCuModifyUserGroupReq, CCuModifyUserGroupRsp &cCuModifyUserGroupRsp);

/*====================================================================
������      : CuCreateUser
����        : �������û�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuCreateUser(CCuCreateUserReq &cCuCreateUserReq, CCuCreateUserRsp &cCuCreateUserRsp);

/*====================================================================
������      : CuDeleteUser
����        : ɾ�����û�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuDeleteUser(CCuDeleteUserReq &cCuDeleteUserReq, CCuDeleteUserRsp &cCuDeleteUserRsp);

/*====================================================================
������      : CuModifyUser
����        : �޸����û�������Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuModifyUser(CCuModifyUserReq &cCuModifyUserReq, CCuModifyUserRsp &cCuModifyUserRsp);

/*====================================================================
������      : CuChangePassword
����        : �޸����û�������Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuChangePassword(CCuChangePasswordReq &cCuChangePasswordReq, CCuChangePasswordRsp &cCuChangePasswordRsp);

/*====================================================================
������      : CuSendKeyFrame
����        : �޸����û�������Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuSendKeyFrame(CCuKeyFrameReq &cCuKeyFrameReq, CCuKeyFrameRsp &CCuKeyFrameRsp);

/*====================================================================
������      : CuGetUserDevice
����        : ��ȡ�û����豸�����б�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuGetUserDevice(CCuGetUserDeviceReq &cCuGetUserDeviceReq, CCuGetUserDeviceRsp &cCuGetUserDeviceRsp);

/*====================================================================
������      : CuGetUserGroupDevice
����        : ��ȡ�û�����豸�����б�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/14/             liangli
====================================================================*/
s32 CuGetUserGroupDevice(CCuGetUserGroupDeviceReq &cCuGetUserGroupDeviceReq, CCuGetUserGroupDeviceRsp &cCuGetUserGroupDeviceRsp);

/*====================================================================
������      : CuUserAddDevice
����        : �û�����֮�û�����豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
s32 CuUserAddDevice(CCuUserAddDeviceReq &cCuUserAddDeviceReq, CCuUserAddDeviceRsp &cCuUserAddDeviceRsp);

/*====================================================================
������      : CuUserDelDevice
����        : �û�����֮�û�ɾ���豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
s32 CuUserDelDevice(CCuUserDelDeviceReq &cCuUserDelDeviceReq, CCuUserDelDeviceRsp &cCuUserDelDeviceRsp);

/*====================================================================
������      : CuUserModDevice
����        : �û�����֮�û��޸��豸Ȩ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
s32 CuUserModDevice(CCuUserModDeviceReq &cCuUserModDeviceReq, CCuUserModDeviceRsp &cCuUserModDeviceRsp);

/*====================================================================
������      : CuUserGroupAddDevice
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
s32 CuUserGroupAddDevice(CCuUserGroupAddDeviceReq &cCuUserGroupAddDeviceReq, CCuUserGroupAddDeviceRsp &cCuUserGroupAddDeviceRsp);

/*====================================================================
������      : CuUserGroupDelDevice
����        : �û���ɾ���豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
s32 CuUserGroupDelDevice(CCuUserGroupDelDeviceReq &cCuUserGroupDelDeviceReq, CCuUserGroupDelDeviceRsp &cCuUserGroupDelDeviceRsp);

/*====================================================================
������      : CuUserGroupModDevice
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/15/             liangli
====================================================================*/
s32 CuUserGroupModDevice(CCuUserGroupModDeviceReq &cCuUserGroupModDeviceReq, CCuUserGroupModDeviceRsp &cCuUserGroupModDeviceRsp);

/*====================================================================
������      : CuGetGetAuthorizationDevice
����        : ��ȡ������ʱ��Ȩ�豸�б�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/16/             liangli
====================================================================*/
s32 CuGetGetAuthorizationDevice(CCuGetAuthorizationDeviceReq &cCuGetAuthorizationDeviceReq, CCuGetAuthorizationDeviceRsp &cCuGetAuthorizationDeviceRsp);

/*====================================================================
������      : CuAddAuthorizationDevice
����        : ��Ŀ�����ָ���û������ʱ��Ȩ�豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/16/             liangli
====================================================================*/
s32 CuAddAuthorizationDevice(CCuAddAuthorizationDeviceReq &cCuAddAuthorizationDeviceReq, CCuAddAuthorizationDeviceRsp &cCuAddAuthorizationDeviceRsp);

/*====================================================================
������      : CuDelAuthorizationDevice
����        : ɾ����ʱ��Ȩ�豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/16/             liangli
====================================================================*/
s32 CuDelAuthorizationDevice(CCuDelAuthorizationDeviceReq &cCuDelAuthorizationDeviceReq, CCuDelAuthorizationDeviceRsp &cCuDelAuthorizationDeviceRsp);

/*====================================================================
������      : CuModifyAuthorizationDevice
����        : �޸���ʱ��Ȩ�豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/16/             liangli
====================================================================*/
s32 CuModifyAuthorizationDevice(CCuModifyAuthorizationDeviceReq &cCuModifyAuthorizationDeviceReq, CCuModifyAuthorizationDeviceRsp &cCuModifyAuthorizationDeviceRsp);

/*====================================================================
������      : CuGetDeviceBaseInfo
����        : ��ȡָ���豸������Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/16/             liangli
====================================================================*/
s32 CuGetDeviceBaseInfo(CCuGetDeviceBaseInfoReq &cCuGetDeviceBaseInfoReq, CCuGetDeviceBaseInfoRsp &cCuGetDeviceBaseInfoRsp);

/*====================================================================
������      : CuSetupStream
����        : Cu����Invite����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetupStream(CCuSetupStreamReq &cCuSetupStreamReq, CCuSetupStreamRsp &cCuSetupStreamRsp);

/*====================================================================
������      : CuPlayStream
����        : CU����InviteAck����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuPlayStream(CCuPlayStreamReq &cCuPlayStreamReq, CCuPlayStreamRsp &cCuPlayStreamRsp);

/*====================================================================
������      : CuStopStream
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuStopStream(CCuStopStreamReq &cCuStopStreamReq, CCuStopInviteRsp &cCuStopStreamRsp);

/*====================================================================
������      : CuGetRCSSysParam
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetRCSSysParam(CCuRcsGetSysParamReq &cCuRcsGetSysParamReq, CCuRcsGetSysParamRsp &cCuRcsGetSysParamRsp);

/*====================================================================
������      : CuGetRCSSysParam
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetRCSSysParam(CCuRcsSetSysParamReq &cCuRcsSetSysParamReq, CCuRcsSetSysParamRsp &cCuRcsSetSysParamRsp);

/*====================================================================
������      : CuGetNruList
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetNruList(CCuRcsQueryNruReq &cCuRcsQueryNruReq, CCuRcsQueryNruRsp &cCuRcsQueryNruRsp);

/*====================================================================
������      : CuGetPuChnBinding
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetPuChnBinding(CCuRcsQueryPuChnBindingReq &cCuRcsQueryPuChnBindingReq, CCuRcsQueryPuChnBindingRsp &cCuRcsQueryPuChnBindingRsp);

/*====================================================================
������      : CuBindPu
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuBindPu(CCuRcsBindingPuChnReq &cCuRcsBindingPuChnReq, CCuRcsBindingPuChnRsp &cCuRcsBindingPuChnRsp);

/*====================================================================
������      : CuUnBindPu
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuUnBindPu(CCuRcsUnbindingPuChnReq &cCuRcsUnbindingPuChnReq, CCuRcsUnbindingPuChnRsp &CCuRcsUnbindingPuChnRsp);

/*====================================================================
������      : CuGetPlatformRecordDays
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetPlatformRecordDays(CCuRcsQueryRecordDaysReq &cCuRcsQueryRecordDaysReq, CCuRcsQueryRecordDaysRsp &CCuRcsQueryRecordDaysRsp);

s32 CuGetPuRecordDays(CCuPuRecDaysQryReq &cCuiPuRecDaysQryReq, CCuPuRecDaysQryRsp &cCuiPuRecDaysQryRsp);

s32 CuStartExport(CCuFileExportReq &cCuFileExportReq, CCuFileExportRsp &cCuFileExportRsp);
s32 CuGetExportProcess(CCuGetExportProcessReq &cCuGetExportUserLogProcessReq, CCuGetExportProcessRsp &cCuGetExportUserLogProcessRsp);
s32 CuStopExport(CCuStopExportReq &cCuStopExportUserLogReq, CCuStopExportRsp &cCuStopExportUserLogRsp);

s32 CuStartExportEx(CCuFileExportStartExReq &cReq, CCuFileExportStartExRsp &cRsp);
s32 CuGetExportProcessEx(CCuFileExportGetProcessExReq &cReq, CCuFileExportGetProcessExRsp &cRsp);
s32 CuStopExportEx(CCuFileExportStopExReq &cReq, CCuFileExportStopExRsp &cRsp);

s32 CuStartImportEx(CCuFileImportStartExReq &cReq, CCuFileImportStartExRsp &cRsp);
s32 CuSetImportProcessEx(CCuFileImportSetProcessExReq &cReq, CCuFileImportSetProcessExRsp &cRsp);
s32 CuStopImportEx(CCuFileImportStopExReq &cReq, CCuFileImportStopExRsp &cRsp);

/*====================================================================
������      : CuGetPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetPlatformRecord(CCuRcsQueryRecordReq &cCuRcsQueryRecordReq, CCuRcsQueryRecordRsp &cCuRcsQueryRecordRsp);



/*====================================================================
������      : CuStartPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuStartPlatformRecord(CCuRcsSetRecordTaskReq &cCuRcsSetRecordTaskReq, CCuRcsSetRecordTaskRsp &cCuRcsSetRecordTaskRsp);

/*====================================================================
������      : CuStartPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuStopPlatformRecord(CCuRcsStopRecordTaskReq &cCuRcsStopRecordTaskReq, CCuRcsStopRecordTaskRsp &cCuRcsStopRecordTaskRsp);

/*====================================================================
������      : CuSetupPlayPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetupPlayPlatformRecord(CCuSetupPlayPlatformRecordReq &cCuSetupPlayPlatformRecordReq, CCuSetupPlayPlatformRecordRsp &cCuSetupPlayPlatformRecordRsp);

/*====================================================================
������      : CuPlayPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuPlayPlatformRecord(CCuPlayPlatformRecordReq &cCuPlayPlatformRecordReq, CCuPlayPlatformRecordRsp &cCuPlayPlatformRecordRsp);

/*====================================================================
������      : CuVcrCtrl
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuVcrCtrl(CCuVcrCtrlReq &cCuVcrCtrlReq, CCuVcrCtrlRsp &cCuVcrCtrlRsp);

/*====================================================================
������      : CuGetCurRecordPlayPosition
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetCurRecordPlayPosition(CCuPlaybackProgressQueryReq &cCuReplayProgressQueryReq, CCuPlaybackProgressQueryRsp &cCuReplayProgressQueryRsp);

/*====================================================================
������      : CuStopPlayPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuStopPlayPlatformRecord(CCuStopPlatformRecordReq &cCuStopPlatformRecordReq, CCuStopPlatformRecordRsp &cCuStopPlatformRecordRsp);

/*====================================================================
������      : CuSetupDownloadPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetupDownloadPlatformRecord(CCuDownloadPlatformRecordReq &cCuDownloadPlatformRecordReq, CCuDownloadPlatformRecordRsp &cCuDownloadPlatformRecordRsp );

/*====================================================================
������      : CuStartDownloadPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuStartDownloadPlatformRecord(CCuStartDownloadPlatformRecordReq &cCuStartDownloadPlatformRecordReq, CCuStartDownloadPlatformRecordRsp &cCuStartDownloadPlatformRecordRsp);

/*====================================================================
������      : CuStopDownloadPlatformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuStopDownloadPlatformRecord(CCuStopDownloadPlatformRecordReq &cCuStopDownloadPlatformRecordReq, CCuStopDownloadPlatformRecordRsp &cCuStopDownloadPlatformRecordRsp);

/*====================================================================
������      : CuQueryGpsTrece
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2014/5/20/             qiqianqian
====================================================================*/
s32 CuQueryGpsTrece(CCuGpsTraceQryReq &cCuGpsTraceQryReq, CCuGpsTraceQryRsp &cCuGpsTraceQryRsp);

/*====================================================================
������      : CuStartPuformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2014/5/19/             qiqianqian
====================================================================*/
s32 CuStartPuRecord(CCuPuRecordStartReq &cCuPuRecordStartReq, CCuPuRecordStartRsp &cCuPuRecordStartRsp);


s32 CuGetDeviceSerialConfig(CCuPuSerialParamCfgGetReq &cReq, CCuPuSerialParamCfgGetRsp &cRsp);
s32 CuSetDeviceSerialConfig(CCuPuSerialParamCfgSetReq &cReq, CCuPuSerialParamCfgSetRsp &cRsp);

/*====================================================================
������      : CuStopPuformRecord
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2014/5/19/             qiqianqian
====================================================================*/
s32 CuStopPuRecord(CCuPuRecordStopReq &cCuPuRecordStopReq, CCuPuRecordStopRsp &cCuPuRecordStopRsp);

/*====================================================================
������      : CuGetDomainList
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetDomainList(CCuDomainRelationQryReq &cCuDomainRelationQryReq, CCuDomainRelationQryRsp& cCuDomainRelationQryRsp);

/*====================================================================
������      : CuGetRecordPlan
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetRecordPlan(CCuRcsQueryRecordParamReq &cCuRcsQueryRecordParamReq, CCuRcsQueryRecordParamRsp &cCuRcsQueryRecordParamRsp);

/*====================================================================
������      : CuSetRecordPlan
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetRecordPlan(CCuRcsSetRecordParamReq &cCuRcsSetRecordParamReq, CCuRcsSetRecordParamRsp &cCuRcsSetRecordParamRsp);

/*====================================================================
������      : CuGetAllTVWall
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetAllTVWall(CCuTvwallQryReq &cCuTvwallQryReq, CCuTvwallQryRsp &cCuTvwallQryRsp);

/*====================================================================
������      : CCuCreatTVWall
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CCuCreatTVWall(CCuTvwallAddReq &cCuTvwallAddReq, CCuTvwallAddRsp &cCuTvwallAddRsp);

/*====================================================================
������      : CuDelTVWall
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuDelTVWall(CCuTvwallDelReq &cCuTvwallDelReq, CCuTvwallDelRsp &cCuTvwallDelRsp);

/*====================================================================
������      : CuModifyTVWall
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuModifyTVWall(CCuTvwallModReq &cCuTvwallModReq, CCuTvwallModRsp &cCuTvwallModRsp);

/*====================================================================
������      : CuPlayRecordOnTVWall
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuPlayRecordOnTVWall(CCuRecordOnTvReq &cCuPlatRecOnTvReq, CCuRecordOnTvRsp &cCuPlatRecOnTvRsp);

/*====================================================================
������      : CuStopPlayRecordOnTVWall
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuStopPlayRecordOnTVWall(CCuRecordOffTvReq &cCuPlatRecOffTvReq, CCuRecordOffTvRsp &cCuPlatRecOffTvRsp);
/*====================================================================
������      : CuTVWallStartPlay
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallStartPlay(CCuRealMediaOnTvReq &cCuRealMediaOnTvReq, CCuRealMediaOnTvRsp &cCuRealMediaOnTvRsp);

/*====================================================================
������      : CuTVWallStopPlay
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallStopPlay(CCuRealMediaOffTvReq &cCuRealMediaOffTvReq, CCuRealMediaOffTvRsp &cCuRealMediaOffTvRsp);

/*====================================================================
������      : CuTVWallWindowPollContrl
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallWindowPollContrl(CCuTvPollOperateReq &cCuTvPollOperateReq, CCuTvPollOperateRsp &cCuTvPollOperateRsp);

/*====================================================================
������      : CuTVWallSaveScheme
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallSaveScheme(CCuTvWallSchemeSaveReq &cCuTvWallSchemeSaveReq, CCuTvWallSchemeSaveRsp &cCuTvWallSchemeSaveRsp);

/*====================================================================
������      : CuTVWallDelScheme
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallDelScheme(CCuTvWallSchemeDelReq &cCuTvWallSchemeDelReq, CCuTvWallSchemeDelRsp &cCuTvWallSchemeDelRsp);

/*====================================================================
������      : CuTVWallLoadScheme
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallLoadScheme(CCuTvWallSchemeLoadReq &cCuTvWallSchemeLoadReq, CCuTvWallSchemeLoadRsp &cCuTvWallSchemeLoadRsp);

/*====================================================================
������      : CuTVWallQueryScheme
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallQueryScheme(CCuTvWallSchemeQueryReq &cCuTvWallSchemeQueryReq, CCuTvWallSchemeQueryRsp &cCuTvWallSchemeQueryRsp);

/*====================================================================
������      : CuTVWallSetSchemePoll
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallSetSchemePoll(CCuTvWallSchemeGroupSetReq &cCuTvWallSchemeGroupSetReq, CCuTvWallSchemeGroupSetRsp &cCuTvWallSchemeGroupSetRsp);

/*====================================================================
������      : CuTVWallGetSchemePoll
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallGetSchemePoll(CCuTvWallSchemeGroupGetReq &cCuTvWallSchemeGroupGetReq, CCuTvWallSchemeGroupGetRsp &cCuTvWallSchemeGroupGetRsp);

/*====================================================================
������      : CuTVWallSchemePollControl
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallSchemePollControl(CCuTvWallSchemePollOperateReq &cCuTvWallSchemePollOperateReq, CCuTvWallSchemePollOperateRsp &cCuTvWallSchemePollOperateRsp);

/*====================================================================
������      : CuCreateMap
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuCreateMap(CCuMapCreateReq &cCuMapCreateReq, CCuMapCreateRsp &cCuMapCreateRsp);

/*====================================================================
������      : CuModifyMap
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuModifyMap(CCuMapModReq &cCuMapModReq, CCuMapModRsp &cCuMapModRsp);

/*====================================================================
������      : CuDelMap
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuDelMap(CCuMapDelReq &cCuMapDelReq, CCuMapDelRsp &cCuMapDelRsp);

/*====================================================================
������      : CuQueryMap
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuQueryMap(CCuMapQryReq &cCuMapQryReq, CCuMapQryRsp &cCuMapQryRsp);

/*====================================================================
������      : CreateMapElement
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuCreateMapElement(CCuMapElementCreateReq &cCuMapElementCreateReq, CCuMapElementCreateRsp &cCuMapElementCreateRsp);

/*====================================================================
������      : CuModifyMapElement
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuModifyMapElement(CCuMapElementModReq &cCuMapElementModReq, CCuMapElementModRsp &cCuMapElementModRsp);

/*====================================================================
������      : CuDelMapElement
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuDelMapElement(CCuMapElementDelReq &cCuMapElementDelReq, CCuMapElementDelRsp &cCuMapElementDelRsp);

/*====================================================================
������      : CuQueryMapElement
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuQueryMapElement(CCuMapElementQryReq &cCuMapElementQryReq, CCuMapElementQryRsp &cCuMapElementQryRsp);

s32 CuSearchMapElement(CCuMapElementSearchReq &cCuMapElementSearchReq, CCuMapElementSearchRsp &cCuMapElementSearchRsp);
/*====================================================================
������      : CuMapPositionMar2Earth
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuMapPositionMar2Earth(CCuPosCorrectMarsToEarthReq &cCuPosCorrectMarsToEarthReq, CCuPosCorrectMarsToEarthRsp &cCuPosCorrectMarsToEarthRsp);

/*====================================================================
������      : CuMapPositionMar2Earth
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuMapPositionEarth2Mar(CCuPosCorrectEarthToMarsReq &cCuPosCorrectEarthToMarsReq, CCuPosCorrectEarthToMarsRsp &cCuPosCorrectEarthToMarsRsp);

s32 CuEMapDataVersion(CCuMpsDataVersionGetReq &cReq, CCuMpsDataVersionGetRsp &cRsp);

s32 CuEGpsDataBackUp(CCuGpsDataBakeUpReq &cReq,CCuGpsDataBakeUpRsp &cRsp);

s32 CuGpsStoreCfgGet(CCuMpsGpsStoreCfgGetReq &cReq,CCuMpsGpsStoreCfgGetRsp &cRsp);
/*====================================================================
������      : CuQueryAlarmLinkConfig
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuQueryAlarmLinkConfig(CCuAlarmLinkQueryReq &cCuAlarmLinkQueryReq, CCuAlarmLinkQueryRsp &cCuAlarmLinkQueryRsp);

s32 CuGetAlarmLinkDataVersion(CCuAlarmLinkVersionGetReq &cCuAlarmLinkQueryReq, CCuAlarmLinkVersionGetRsp &cCuAlarmLinkQueryRsp);

/*====================================================================
������      : CuGetAlarmLinkConfig
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetAlarmLinkConfig(CCuAlarmLinkCfgGetReq &cCuAlarmLinkCfgGetReq, CCuAlarmLinkCfgGetRsp &cCuAlarmLinkCfgGetRsp);

/*====================================================================
������      : CuSetAlarmLinkConfig
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetAlarmLinkConfig(CCuAlarmLinkCfgSetReq &cCuAlarmLinkCfgSetReq, CCuAlarmLinkCfgSetRsp &cCuAlarmLinkCfgSetRsp);

/*====================================================================
������      : CuQueryUserLog
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuQueryUserLog(CCuOperateLogQryReq &cCuOperateLogQryReq, CCuOperateLogQryRsp &cCuOperateLogQryRsp);

/*====================================================================
������      : CuQueryDeviceLog
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuQueryDeviceLog(CCuDeviceLogQryReq &cCuDeviceLogQryReq, CCuDeviceLogQryRsp &cCuDeviceLogQryRsp);

/*====================================================================
������      : CuQueryAlarmLog
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuQueryAlarmLog(CCuAlarmLogQryReq &cCuAlarmLogQryReq, CCuAlarmLogQryRsp &cCuAlarmLogQryRsp);

/*====================================================================
������      : CuGetLogConfig
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetLogConfig(CCuConfigUasGetReq &cCuConfigUasGetReq, CCuConfigUasGetRsp &cCuConfigUasGetRsp);

/*====================================================================
������      : CuSetLogConfig
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetLogConfig(CCuConfigUasSetReq &cCuConfigUasSetReq, CCuConfigUasSetRsp &cCuConfigUasSetRsp);

/*====================================================================
������      : CuGetDeviceGBID
����        : ��ȡ�豸����id
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2014/08/04/             qiqianqian
====================================================================*/
s32 CuGetDeviceGBID(CCuGbsDevGbidGetReq &cCuGbsDevGbidGetReq, CCuGbsDevGbidGetRsp &cCuGbsDevGbidGetRsp);

/*====================================================================
������      : CuCallPu
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuCallPu(CCuStartThirdPartyCallReq &cCuStartThirdPartyCallReq, CCuStartThirdPartyCallRsp &cCuStartThirdPartyCallRsp );

/*====================================================================
������      : CuHangUpPu
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuHangUpPu(CCuStopThirdPartyCallReq &cCuStopThirdPartyCallReq, CCuStopThirdPartyCallRsp &cCuStopThirdPartyCallRsp);
/*====================================================================
������      : CuGetDeviceEncoderParam
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetDeviceVidEncoderParam(CCuPuVidEncParamGetReq &cPuVidEncParamGetReq, CCuPuVidEncParamGetRsp &cPuVidEncParamGetRsp);

s32 CuGetDeviceAudEncoderParam(CCuPuAudEncParamGetReq &cPuAudEncParamGetReq, CCuPuAudEncParamGetRsp &cPuAudEncParamGetRsp);
/*====================================================================
������      : CuSetDeviceEncoderParam
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetDeviceEncoderParam(CCuPuVidEncParamSetReq &cCuPuVidEncParamSetReq, CCuPuVidEncParamSetRsp &cCuPuVidEncParamSetRsp);

/*====================================================================
������      : CuGetDiskList
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetDiskList(CCuRcsQueryDiskInfoReq &cCuRcsQueryDiskInfoReq, CCuRcsQueryDiskInfoRsp &cCuRcsQueryDiskInfoRsp);

/*====================================================================
������      : CuSetDeviceName
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetDeviceName(CCuDeviceNameModReq &cCuDeviceNameModReq, CCuDeviceNameModRsp &cCuDeviceNameModRsp);

/*====================================================================
������      : CuSetVideoSrcName
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuSetVideoSrcName(CCuDeviceCapIndexNameSetReq &cCuDeviceCapIndexNameSetReq, CCuDeviceCapIndexNameSetRsp &cCuDeviceCapIndexNameSetRsp);

/*====================================================================
������      : CuGetMotionDetectConfig
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/2/22/             liangli
====================================================================*/
s32 CuGetMotionDetectConfig(CCuPuMotionDetectGetReq &cReq, CCuPuMotionDetectGetRsp &cRsp);

s32 CuSetMotionDetectConfig(CCuPuMotionDetectSetReq &cReq, CCuPuMotionDetectSetRsp &cRsp);

s32 CuGetShadeConfig(CCuPuAreaShadeGetReq &cReq, CCuPuAreaShadeGetRsp &cRsp);

s32 CuSetShadeConfig(CCuPuAreaShadeSetReq& cReq, CCuPuAreaShadeSetRsp& cRsp);

s32 CuSetTVDivNum(CTvWinStyleSetReq& cReq, CTvWinStyleSetRsp& cRsp);

s32 CuGetDeviceChannelAlias(CCuDeviceCapIndexNameGetReq& cReq, CCuDeviceCapIndexNameGetRsp& cRsp);

s32 CuSetDeviceChannelAlias(CCuDeviceCapIndexNameSetReq& cReq, CCuDeviceCapIndexNameSetRsp& cRsp);

s32 CuGetSyncVideoSrcNameEnableAlias(CCuGetSyncVideoSrcNameEnableReq& cReq, CCuGetSyncVideoSrcNameEnableRsp& cRsp);

s32 CuSetSyncVideoSrcNameEnableAlias(CCuSetSyncVideoSrcNameEnableReq& cReq, CCuSetSyncVideoSrcNameEnableRsp& cRsp);

s32 CuGetSupportMapTypes(CCuGetSupportMapTypesReq& cReq, CCuGetSupportMapTypesRsp& cRsp);

s32 CuQueryMapArea(CCuMapAreaQryReq& cReq, CCuMapAreaQryRsp& cRsp);

s32 CuCreateMapArea(CCuMapAreaCreateReq& cReq, CCuMapAreaCreateRsp& cRsp);

s32 CuModifyMapArea(CCuMapAreaModReq& cReq, CCuMapAreaModRsp& cRsp);

s32 CuVsipChannelData(CCuPuVsipOperateReq& cReq, CCuPuVsipOperateRsp& cRsp);

s32 CuGetMapImageUploadUrl(CCuMapUploadReq& cReq, CCuMapUploadRsp& cRsp);

s32 CuGetMapElementImageUploadUrl(CCuElementIconUploadReq& cReq, CCuElementIconUploadRsp& cRsp);

s32 CuQueryMapElementImageList(CCuElementImageQryReq& cReq, CCuElementImageQryRsp& cRsp);

s32 CuSetCameraProtocolType(CCuPuCameraTypeSetReq& cReq, CCuPuCameraTypeSetRsp& cRsp);

s32 CuResetAlarm(CCuPuAlarmResetReq& cReq, CCuPuAlarmResetRsp& cRsp);

s32 CuDeviceRecordSvrQry(CCuDeviceRecordSvrQryReq& cReq, CCuDeviceRecordSvrQryRsp& cRsp);

s32 CuPuRecord(CCuPuRecQryReq &cReq, CCuPuRecQryRsp &cRsp);

s32 CuGetPtzPresetName(CCuVideosourcePresetcfgQryReq &cReq, CCuVideosourcePresetcfgQryRsp &cRsp);

s32 CuSetPtzPresetName(CCuVideosourcePresetcfgAddReq &cReq, CCuVideosourcePresetcfgAddRsp &cRsp);

s32 CuDelPtzPresetName(CCuVideosourcePresetcfgDelReq &cReq, CCuVideosourcePresetcfgDelRsp &cRsp);

s32 CuGetCAPSConfig(CCuGetCAPSConfigReq cReq, CCuGetCAPSConfigRsp &cRsp);

s32 CuSetCAPSConfig(CCuSetCAPSConfigReq cReq, CCuSetCAPSConfigRsp &cRsp);

s32 CuQueryCAPSDeviceList(CCuQueryCAPSDeviceListReq cReq, CCuQueryCAPSDeviceListRsp &cRsp);

s32 CuCAPSAddDevice(CCuCAPSAddDeviceReq cReq, CCuCAPSAddDeviceRsp &cRsp);

s32 CuCAPSModifyDevice(CCuCAPSModifyDeviceReq cReq, CCuCAPSModifyDeviceRsp &cRsp);

s32 CuCAPSDelDevice(CCuCAPSDelDeviceReq cReq, CCuCAPSDelDeviceRsp &cRsp);

s32 CuGetOfflineDeviceStat(CCuOffLineDevQryReq cReq, CCuOffLineDevQryRsp &cRsp);

s32 CuUpdateCUProperty(CCuUpdateCUPropertyReq cReq, CCuUpdateCUPropertyRsp &cRsp);

s32 CuGbsGbDevTeleBoot( CCuGbsGbDevTeleBootReq cReq, CCuGbsGbDevTeleBootRsp &cRsp );

s32 CuUserDeviceGroupAdd(CCuUserDeviceGroupAddReq cReq, CCuUserDeviceGroupAddRsp &cRsp);

s32 CuUserDeviceGroupDel(CCuUserDeviceGroupDelReq cReq, CCuUserDeviceGroupDelRsp &cRsp);

s32 CuUserGroupAddDeviceGroup(CCuUserGroupDeviceGroupAddReq cReq, CCuUserGroupDeviceGroupAddRsp &cRsp);

s32 CuUserGroupDelDeviceGroup(CCuUserGroupDeviceGroupDelReq cReq, CCuUserGroupDeviceGroupDelRsp &cRsp);

s32 CuUserSetAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserSetReq cReq, CCuDeviceAutoAssignToUserSetRsp &cRsp);

s32 CuUserGroupSetAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserGroupSetReq cReq, CCuDeviceAutoAssignToUserGroupSetRsp &cRsp);

s32 CuUserQueryAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserQryReq cReq, CCuDeviceAutoAssignToUserQryRsp &cRsp);

s32 CuUserGroupQueryAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserGroupQryReq cReq, CCuDeviceAutoAssignToUserGroupQryRsp &cRsp);

s32 CuQueryOperationProgree(CCuTransactionProgressGetReq cReq, CCuTransactionProgressGetRsp &cRsp);

s32 CuGetSuperGBDomainList(CCuGbsGbParentDomainGetReq cReq, CCuGbsGbParentDomainGetRsp &cRsp);

s32 CuPushDevice(CCuGbsGbPDCatalogLoadReq cReq, CCuGbsGbPDCatalogLoadRsp &cRsp);

s32 CuGetUserDataVersion(CCuUserDataVersionGetReq cReq, CCuUserDataVersionGetRsp &cRsp);

s32 CuGetCivilcode(CCuGbsDevGrpCivilCodeGetReq cReq, CCuGbsDevGrpCivilCodeGetRsp &cRsp);

s32 CuGetGbsDevKdid(CCuGbsDevKdidGetReq cReq, CCuGbsDevKdidGetRsp &cRsp);

s32 CuCreateCustomTreeView(CCuDevTreeAddReq cReq, CCuDevTreeAddRsp &cRsp);

s32 CuModifyCustomTreeViewName(CCuDevTreeModReq cReq, CCuDevTreeModRsp &cRsp);

s32 CuDeleteCustomTreeView(CCuDevTreeDelReq cReq, CCuDevTreeDelRsp &cRsp);

s32 CuGetCustomTreeViewList(CCuDevTreeQryReq cReq, CCuDevTreeQryRsp &cRsp);

s32 CuGetDeviceGroupPath(CCuDeviceGrpPathQryReq cReq, CCuDeviceGrpPathQryRsp &cRsp);

s32 CuSearchDevice(CCuDPSSearchReq cReq, CCuDPSSearchRsp &cRsp);

s32 CuGetSearchResult(CCuDPSGetResultReq cReq, CCuDPSGetResultRsp &cRsp);

s32 CuGetSearchResultByGroup(CCuDPSGetResultByGroupReq cReq, CCuDPSGetResultByGroupRsp &cRsp);

s32 CuEndOperation(CCuDPSSearchStopReq cReq, CCuDPSSearchStopRsp &cRsp);

s32 CuGetDeviceStatus(CCuDPSGetDeviceStatusReq cReq, CCuDPSGetDeviceStatusRsp &cRsp);

s32 CuGetDevInfoAndStatus(CCuGetDevInfoAndStatusReq cReq, CCuGetDevInfoAndStatusRsp &cRsp);

s32 CuGetUserDeviceInGroupRecursive(CCuUserDevGrpVidsrcQryReq cReq, CCuUserDevGrpVidsrcQryRsp &cRsp);

s32 CuBatchOperation(CCuBatOptReq cReq, CCuBatOptRsp &cRsp);

s32 CuGetDeviceGroupVideoSrcNum(CCuDevGrpVidsrcTotalnumGetReq cReq, CCuDevGrpVidsrcTotalnumGetRsp &cRsp);

s32 CuGetVSOfflineInfo(CCuVidsrcInfoQryReq cReq, CCuVidsrcInfoQryRsp &cRsp);

s32 CuGetAzimuth(CCuGetAzimuthReq cReq, CCuGetAzimuthRsp &cRsp);

s32 CuGetDeviceGroupOfGBDeviceInfo(CCuDeviceGroupDeviceQry4GbReq &cReq, CCuDeviceGroupDeviceQry4GbRsp &cRsp);

s32 CuDeviceMatchGbid(CCuDeviceMatchGbidReq cReq, CCuDeviceMatchGbidRsp &cRsp);

s32 CuDeviceGrpDevSumNum(CCuDeviceGrpDevSumNumReq cReq, CCuDeviceGrpDevSumNumRsp &cRsp);

s32 CuDeviceGroupDeviceMod4Gb(CDeviceGroupDeviceMod4GbReq cReq, CDeviceGroupDeviceMod4GbRsp &cRsp);

s32 CuDeviceGbidErrGet(CCuDeviceGbidErrGetReq cReq, CCuDeviceGbidErrGetRsp &cRsp);

s32 CuGbsCatalogSelectReportStart(CCuGbsCatalogSelectReportStartReq cReq, CCuGbsCatalogSelectReportStartRsp &cRsp);

s32 CCuDeviceGbinfoQry(CCuDeviceGbinfoQryReq cReq, CCuDeviceGbinfoQryRsp &cRsp);

s32 CCuDevgrpGbinfoQry(CCuDevgrpGbinfoQryReq cReq, CCuDevgrpGbinfoQryRsp &cRsp);

s32 CCuSetPuVidSrcOSD(CCuPuVidSrcOsdSetReq cReq, CCuPuVidSrcOsdSetRsp &cRsp);

s32 CCuGetPuVidSrcOSD(CCuPuVidSrcOsdGetReq cReq, CCuPuVidSrcOsdGetRsp &cRsp);

s32 CuModifyGBDeviceGroup(CCuModifyGBDeviceGroupReq cReq, CCuModifyGBDeviceGroupRsp &cRsp);

s32 CuPuLongOsdGet(CCuPuLongOsdGetReq cReq, CCuPuLongOsdGetRsp &cRsp);

s32 CuPuLongOsdSet(CCuPuLongOsdSetReq cReq, CCuPuLongOsdSetRsp &cRsp);

s32 CuAdjustVideoSrcOrder(CCuAdjustVideoSrcOrderReq cReq, CCuAdjustVideoSrcOrderRsp &cRsp);

s32 CuGetAlarmTypes(CCuGetAlarmTypesReq cReq, CCuGetAlarmTypesRsp &cRsp);

s32 CuPlatformRecordLock(CCuRcsRecordLockOprReq cCuRcsRecordLockOprReq, CCuRcsRecordLockOprRsp &cCuRcsRecordLockOprRsp);

s32 CuPlatformRecordUnLock(CCuRcsRecordUnLockOprReq cCuRcsRecordUnLockOprReq, CCuRcsRecordUnLockOprRsp &cCuRcsRecordUnLockOprRsp);

s32 CuRealMediaSwitchOccurTypeReq(CCuRealMediaSwitchOccurTypeReq cReq, CCuRealMediaSwitchOccurTypeRsp &cRsp);

s32 CuGetGBDomainPrivilege(CCuGbsGbdomainParentPrivilegeGetReq cReq, CCuGbsGbdomainParentPrivilegeGetRsp &cRsp);
s32 CuSetGBDomainPrivilege(CCuGbsGbdomainParentPrivilegeSetReq cReq, CCuGbsGbdomainParentPrivilegeSetRsp &cRsp);

s32 CuGetGbsOptionConf(CCuGbsOptionConfGetReq cReq, CCuGbsOptionConfGetRsp &cRsp);
s32 CuSetGbsOptionConf(CCuGbsOptionConfSetReq cReq, CCuGbsOptionConfSetRsp &cRsp);
//֤���ϴ�����
s32 CuUploadGbsCertificate(CCuGbsCertificateUploadReq cReq, CCuGbsCertificateUploadRsp &cRsp);
s32 CuGetGbsCertificate(CCuGbsCertificateGetReq cReq, CCuGbsCertificateGetRsp &cRsp);

s32 CuCertificateQry(CCuCertificateQryReq cReq, CCuCertificateQryRsp &cRsp);
s32 CuDeviceSignatrueCtrl(CCuDeviceSignatrueCtrlReq cReq, CCuDeviceSignatrueCtrlRsp &cRsp);
s32 CuDeviceEncryptionCtrl(CCuDeviceEncryptionCtrlReq cReq, CCuDeviceEncryptionCtrlRsp &cRsp);
s32 CuSecurityAuthenticateFirstTime(CCuSecurityAuthorizationFirstReq  &cReq, CCuSecurityAuthorizationFirstRsp  &cRsp);
s32 CuSecurityAuthenticateSecondTime(CCuSecurityAuthorizationSecondReq &cReq, CCuSecurityAuthorizationSecondRsp &cRsp);

#endif


