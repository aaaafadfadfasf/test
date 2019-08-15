/*=============================================================================
ģ   ��   ��: ���ܷ�����
��   ��   ��: evnmsnmc.h
�� ��  �� ��: ��
�ļ�ʵ�ֹ���: NMS-NMC��Ϣ����
��        ��: 
��        ��: V1.0  
-------------------------------------------------------------------------------
�޸ļ�¼:
��      ��  �汾    �޸���      �޸�����
2005/12/20  1.0     �κ�ƽ       ����     
2006/02/22  3.9     ���ǿ       �������������ṹ���壬Ϊ����Ϣ������Ϣ����
=============================================================================*/

#ifndef _EVNMSNMC_20050907_H_
#define _EVNMSNMC_20050907_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "osp.h"
#include "eventid.h"
#include "nmsmacro.h"
#include "snmpadp.h"
#include "calladdr.h"

//�Զ������
#define NMS_ASSERT(x)

#define NMS_SYSTEM_VERSION          (u32)0x3932         //3.9_R3.2

#define MATCH_CONDITION_OR              1               // ������������һ���ȳ���
#define MATCH_CONDITION_AND             2               // ��������ȫ������ų���

//���ݿ����ַ���
#define DB_LANGID_SC_CHN        0       //��������
#define DB_LANGID_TR_CHN        1       //��������
#define DB_LANGID_US_ENG        2       //Ӣ��

#define DB_CHARSETID_ISO_1      0       //Ӣ���ַ���
#define DB_CHARSETID_CP850      1       //Ӣ���ַ���
#define DB_CHARSETID_EUCGB      2       //�����ַ���
#define DB_CHARSETID_GB18030    3       //�����ַ���
#define DB_CHARSETID_BIG5       4       //���������ַ���
#define DB_CHARSETID_UTF8       5       //ͨ���ַ���
//�Ƿ���Ч
#define INVALID_VALUE                   (u8)0           // ��Ч��u8��ֵ
#define VALID_VALUE                     (u8)1           // ��Чֵ
#define INVALID_PARAM                   (u32)0xFFFFFFFF // ��Ч��u32����
#define VALID_PARAM                     (u32)0x00000001 // ��Ч��u32����

// �澯״̬������,����Ϊ���Ͷ��壬����Ϊmask
#define ALARM_NORMAL                    (u8)0x00        // �豸����
#define ALARM_IMPORTANT                 (u8)0x01        // ��Ҫ�澯
#define ALARM_GENERAL                   (u8)0x02        // ��ͨ�澯
#define ALARM_CRITICAL                  (u8)0x04        // ���ظ澯
#define ALARM_UNKNOW                    (u8)0x05        // δ֪�澯

// �������ͼ���,����Ϊ���Ͷ��壬����Ϊmask
#define ACTION_INVALID                  (u8)0x00        // ��Чֵ����ƥ�伶��
#define ACTION_IMPORTANT                (u8)0x01        // ��Ҫ����
#define ACTION_SECONDARY                (u8)0x02        // ��Ҫ����
#define ACTION_GENERAL                  (u8)0x04        // ��ͨ����


// �澯��������,����Ϊ���Ͷ��壬����Ϊmask
#define ALARM_FILTER_LEVEL              0x00000001      // �澯�������
#define ALARM_FILTER_CODE               0x00000002      // �澯�����
#define ALARM_FILTER_DEV_ID             0x00000004      // �澯�豸ID����
#define ALARM_FILTER_DEV_TYPE           0x00000008      // �澯�豸���͹���
#define ALARM_FILTER_DEV_ALIAS          0x00000010      // �澯�豸��������


// �澯��ѯ����,����Ϊ���Ͷ��壬����Ϊmask
#define ALARM_QUERY_LEVEL               0x00000001      // �澯����
#define ALARM_QUERY_CODE                0x00000002      // �澯��
#define ALARM_QUERY_DEV_ID              0x00000004      // �澯�豸ID
#define ALARM_QUERY_DEV_TYPE            0x00000008      // �澯�豸����
#define ALARM_QUERY_DEV_ALIAS           0x00000010      // �澯�豸����
#define ALARM_QUERY_DESC                0x00000020      // �澯����
#define ALARM_QUERY_START_TIME          0x00000040      // �澯����ʱ��
#define ALARM_QUERY_RECOVER_TIME        0x00000080      // �澯�ָ�ʱ��
#define ALARM_QUERY_ALARM_STATE         0x00000100      // ���澯״̬��ѯ
#define ALARM_QUERY_IP_ADDR             0x00000200      // ��IP��ַ��ѯ
#define ALARM_QUERY_DEV_LARGE_TYPE      0x00000400    //�澯������

// ������ѯ����,����Ϊ���Ͷ��壬����Ϊmask
#define ACTION_QUERY_USER_NAME          0x00000001      // ���û�����
#define ACTION_QUERY_GROUP_NAME         0x00000002      // ��������
#define ACTION_QUERY_TIME               0x00000004      // ��ʱ��β�
#define ACTION_QUERY_DEV_ID             0x00000008      // �����ʵ��豸ID��
#define ACTION_QUERY_ACT_TYPE           0x00000010      // ���������Ͳ�
#define ACTION_QUERY_ACT_INFO           0x00000020      // ��������ϸ������
#define ACTION_QUERY_ACT_LEVEL          0x00000040      // �����������

// Ȩ�޷�Χ
#define RIGHT_RANGE_ALL_CHILD           0x01            // �Ա��豸�����������豸ӵ��Ȩ��
#define RIGHT_RANGE_NO_CHILD            0x02            // �Ա��豸�����������豸��û��Ȩ��
#define RIGHT_RANGE_PART_CHILD          0x04            // ��ƽ̨�豸�Ĳ��������豸ӵ��Ȩ��

// �豸����Ȩ��,����Ϊ���Ͷ��壬����Ϊmask
#define DEV_RIGHT_VIEW_CFG              0x00000001      // �鿴������Ϣ
#define DEV_RIGHT_MODIFY_CFG            0x00000002      // �޸�������Ϣ
#define DEV_RIGHT_VIEW_PFM              0x00000004      // �鿴������Ϣ
#define DEV_RIGHT_REBOOT                0x00000008      // �����豸
#define DEV_RIGHT_PING                  0x00000010      // PING�豸
#define DEV_RIGHT_VIEW_LOG              0x00000020      // �鿴��ʷ�澯��Ϣ
#define DEV_RIGHT_RELEASE_ALARM         0x00000040      // ����豸�澯
#define DEV_RIGHT_SELFTEST              0x00000080      // �豸�Լ�
#define DEV_RIGHT_SOFTWARE_UPDATE       0x00000100      // �ϴ��汾
#define DEV_RIGHT_OWNER                 0x00000200      // �豸��ӵ��Ȩ��
#define DEV_RIGHT_VIEW_DEVFORM          0x00000400      // �鿴��������ͼ

//��Ȩ����Ϣ
#define FIELD_RIGHT_ADD_FIELD_GROUP     0x00000001      // �½����û���
#define FIELD_RIGHT_MODDEL_FIELD_GROUP  0x00000002      // �޸�/ɾ�����û���
#define FIELD_RIGHT_VIEW_FIELD_GROUP        0x00000004      // �鿴���û���
#define FIELD_RIGHT_VIEW_FIELD_USER     0x00000008      // �鿴���û�
#define FIELD_RIGHT_MODDEL_FIELD_USER       0x00000010      // �޸�/ɾ�����û�
#define FIELD_RIGHT_CHILD_FIELD_RIGHT       0x00000020      // �Ƿ��������Ȩ��
#define FIELD_RIGHT_OWNER               0x00000040      // ���ӵ��Ȩ��


// ϵͳ����Ȩ��,����Ϊ���Ͷ��壬����Ϊmask
#define SYS_RIGHT_USER_MANGE            0x00000020
#define SYS_RIGHT_VIEW_OTHER_GROUP_LOG  0x00000040      // �鿴�����û��������־
#define SYS_RIGHT_DEL_SELF_LOG          0x00000080      // ɾ���Լ��Ĳ�����־
#define SYS_RIGHT_DEL_OTHER_GROUP_LOG   0x00000100      // ɾ�������û���Ĳ�����־
#define SYS_RIGHT_DEL_HISTORY_ALARM     0x00000200      // ɾ����ʷ�澯
#define SYS_RIGHT_MODIFY_NMS_AUTO_ACT   0x00000400      // �޸ķ������Զ����ݼ���������
#define SYS_RIGHT_SET_IP_RULES          0x00000800      // ����IP���޹���
#define SYS_RIGHT_SET_EMAIL_RULES       0x00001000      // ����EMAIL���͹���
#define SYS_RIGHT_SET_ALARM_RULES       0x00002000      // ���ø澯���˹���
#define SYS_RIGHT_LOCK_NMCUI            0x00004000      // ʹ��NMC������������
#define SYS_RIGHT_MODIFY_NMS_CFG        0x00008000      // �޸ķ���������

// NMC������Ϣ����,����Ϊ���Ͷ��壬����Ϊmask
#define NMC_REQ_BASE_PFM_INFO           0x00000001      // �����豸�������ܲ���
#define NMC_REQ_EXTEND_PFM_INFO         0x00000002      // �����豸��չ���ܲ���
#define NMC_REQ_FOR_MOCK_DRAW           0x00000004      // �����豸���ܲ������ڻ����ܹ���ͼ

// �豸��ID����
#define NMC_DEVGROUP_SERIAL_NO  (u32)0x80000000         // �ӷ�������ȡ�������豸��IDҪ�����λ��1
#define NMS_DEVGROUP_SERIAL_NO  (u32)0x7FFFFFFF         // �������������豸��IDҪ�����λ����0
#define NMS_DEV_TYPE_INVALID            (u8)0x00        //�豸Ϊ��Ч���ͻ��ǲ���ʶ��
#define NMS_DEV_TYPE_PU                 (u8)0x01        //�豸������ΪPU
#define NMS_DEV_TYPE_CMU                (u8)0x02        //�豸������ΪCMU

// ������־��������
enum ACT_TYPE_IDX
{
    ACT_TYPE_LOGON_IDX = 0,             //  ��¼           
    ACT_TYPE_LOGOFF_IDX,                //  ע��           
    ACT_TYPE_ADD_GROUP,                 //  �����û���     
    ACT_TYPE_MODIFY_GROUP,              //  �޸��û���     
    ACT_TYPE_DEL_GROUP,                 //  ɾ���û���     
    ACT_TYPE_ADD_USER,                  //  �����û�       
    ACT_TYPE_MODIFY_USER,               //  �޸��û�       
    ACT_TYPE_DEL_USER,                  //  ɾ���û�       
    ACT_TYPE_MODIFY_ACCOUNT,            //  �޸������ʻ���Ϣ       
    ACT_TYPE_SAVE_PREFER,               //  ������Ի���Ϣ 
    ACT_TYPE_EXPORT_PREFER,             //  �������Ի���Ϣ 
    ACT_TYPE_IMPORT_PREFER,             //  ������Ի���Ϣ 
    ACT_TYPE_DEFAULT_VALUE,             //  �ָ�Ĭ��ֵ     
    ACT_TYPE_GET_DEV_CFG,               //  ��ȡ�豸����   
    ACT_TYPE_SET_DEV_CFG,               //  �޸��豸����   
    ACT_TYPE_REBOOT,                    //  �����豸
    ACT_TYPE_PING,                      //  PING�豸
    ACT_TYPE_SELFTEST,                  //  �豸�Լ�
    ACT_TYPE_SOFTWARE_UPDATE,           //  �汾�ϴ�
    ACT_TYPE_ALARM_SETTING,             //  �澯����               
    ACT_TYPE_ALARM_ANALYSE,             //  ��������               
    ACT_TYPE_ALARM_QUERY,               //  ��ѯ�澯               
    ACT_TYPE_DEL_ALARM,                 //  ����澯               
    ACT_TYPE_EXPORT_ALARM,              //  �����澯               
    ACT_TYPE_RELEASE_ALARM,             //  ����澯               
    ACT_TYPE_GET_DEV_PFM,               //  �鿴�Բβ���           
    ACT_TYPE_PFM_ANALYSE,               //  ���ܷ���               
    ACT_TYPE_SET_IP_RULES,              //  �趨����IP��ַ����     
    ACT_TYPE_SET_MAIL_RULES,            //  �趨�ʼ����͹���    
    ACT_TYPE_SET_ALARM_FILTER_RULES,    //  �趨�澯���˹���    
    ACT_TYPE_SET_LOGIN_LOCK_TIME,       //  �趨��¼����ʱ��       
    ACT_TYPE_MANUAL_BACKUP_DB,          //  �ֶ������������ݿ�     
    ACT_TYPE_RECOVER_DB,                //  �ָ��������ݿ�         
    ACT_TYPE_QUERY_ACT_LOG,             //  ��ѯ������־           
    ACT_TYPE_EXPORT_ACT_LOG,            //  ����������־           
    ACT_TYPE_DEL_ACT_LOG,               //  ɾ��������־           
    ACT_TYPE_LOCK_NMCUI,                //  �ֶ���������
    ACT_TYPE_UNLOCK_NMCUI,              //  ��������               
    ACT_TYPE_SET_NMS_CFG,               //  �޸ķ��������ò���     
    ACT_TYPE_VIEW_DEV_PIC,              //  �鿴�豸���ܹ���ͼ
    ACT_TYPE_DEV_GROUP_ADD,             //  �����豸�����
    ACT_TYPE_DEV_GROUP_MODIFY,          //  �޸��豸�����
    ACT_TYPE_DEV_GROUP_DEL,             //  ɾ���豸�����
    ACT_TYPE_NUM                        //  ������������
};


// IP������������
#define IP_RULES_IP_LIST_MASK           0x00000001      // ����IP�б�����
#define IP_RULES_IP_SEGMENT_MASK        0x00000002      // ������������
#define IP_RULES_IP_ZONE_MASK           0x00000004      // ���ݵ�ַ��������

// �ʼ����ʹ�������
#define MAIL_RULES_DEV_TYPE_MASK        0x00000001      // �����豸�����б��ж�
#define MAIL_RULES_DEV_ID_MASK          0x00000002      // �����豸ID�б��ж�
#define MAIL_RULES_DEV_NAME_MASK        0x00000004      // �����豸�����ж�
#define MAIL_RULES_DEV_IP_MASK          0x00000008      // �����豸IP�ж�
#define MAIL_RULES_ALARM_CODE_MASK      0x00000010      // ���ݸ澯���ж�
#define MAIL_RULES_ALARM_LEVEL_MASK     0x00000020      // ���ݸ澯�����ж�

// �Զ������Ĳ���
#define AUTO_ACT_WEEKDAY                (u8)1           // ָ���������е�ĳЩ��
#define AUTO_ACT_PERIODICAL             (u8)2           // �������ڣ���С��λΪ��

#define WEEK_MONDAY                     0x00000001      // ����һ
#define WEEK_TUESDAY                    0x00000002      // ���ڶ�
#define WEEK_WEDNESDAY                  0x00000004      // ������
#define WEEK_THURSDAY                   0x00000008      // ������
#define WEEK_FRIDAY                     0x00000010      // ������
#define WEEK_SATURDAY                   0x00000020      // ������
#define WEEK_SUNDAY                     0x00000040      // ������

//�������Ϣ�ĸ�������
#define ACTION_ADD      1
#define ACTION_MODIFY   2
#define ACTION_DELETE   3


//////////////////////////////////////////////////////////////////////////
// �������Ϳͻ��˽�����Ϣ
// NMS��NMC�Ľ�����Ϣ���ܶ��漰�������ݿ�����ģ�nmcapp�յ���ֱ��ת����dbapp����
// ���ԾͲ��ٵ���������Ϣ��
//
// �ͻ��˵�¼
// �ͻ��˵�¼����
//    nmc -> nmcapp -> dbapp (req): TNmsLogonReq + u32(�ͻ��˰汾��)
//    nmc <- nmcapp <- dbapp (ack): TNmsLogonRet + u32(�������汾��) +u8(���鳤��)+s8����(�û�������)������: ERROR_SYSTEM_VERSION_ERROR
//              |
// serviceapp <-|
// nmc <- nmcapp <- dbapp (nack): TNmsLogonReq + u32(�������汾��) ������: ERROR_PWD/ERROR_USER_NOT_EXIST/ERROR_IP_RESTRICT/ERROR_SYSTEM_VERSION_ERROR
// nmc <- nmcapp (finish):
OSPEVENT( EV_NMC_LOGON,                         EV_NMC_NMS_BGN + 0 );
// �ͻ���ע������
// nmc -> nmcapp -> serviceapp (req): 
// nmc <- nmcapp (ack):
OSPEVENT( EV_NMC_LOGOUT,                        EV_NMC_NMS_BGN + 5 );
// �û�Ȩ�޵���Ϣ, ��¼ʱʹ��
// nmcapp -> dbapp(req): u32(�û���ˮ��)
//     nmc <- nmcapp <- dbapp (notify):u32(�û���ˮ��)+ TTnmMask(ϵͳȨ��) + u32(��Ȩ������+ TDeviceRight����
//              |            + u32(�豸Ȩ����) + TDeviceRight����
// serviceapp <-|  
//     nmc <- nmcapp <- dbapp (finish):u32(�û���ˮ��)
//              |
// serviceapp <-|  

OSPEVENT( EV_NMC_USER_RIGHT,                    EV_NMC_NMS_BGN + 10 );

//////////////////////////////////////////////////////////////////////////
// �û�����(nmcpp/dbapp����)
// �����û���Ϣ�б�
// nmc -> nmcapp -> dbapp (req): u32(�û���ˮ��)[ + u8(MSG_TYPE_GROUP/MSG_TYPE_USER)]
// nmc <- nmcapp <- dbapp (notify):u8(MSG_TYPE_GROUP) + u32(�û����б��С) + TUserGroupInfo�û����б�
// nmc <- nmcapp <- dbapp (notify):u8(MSG_TYPE_USER) +  u32(�û��б��С) + TUserInfo�û��б�
// nmc <- nmcapp <- dbapp (finish):u8(MSG_TYPE_GROUP/MSG_TYPE_USER)
OSPEVENT( EV_GET_USER_LIST,                     EV_NMC_NMS_BGN + 40 );

// �����û�����Ϣ
// nmc -> nmcapp -> dbapp (req): TUserGroupInfo
// nmc <- nmcapp <- dbapp (ack): TUserGroupInfo
// nmc <- nmcapp <- dbapp (nack): TUserGroupInfo    ������: ERROR_GROUP_EXIST
OSPEVENT( EV_ADD_USER_GROUP,                    EV_NMC_NMS_BGN + 50 );

// �޸��û���
//       nmc -> nmcapp -> dbapp (req): TUserGroupInfo
//       nmc <- nmcapp <- dbapp (ack):TUserGroupInfo
//   serviceapp <-|(notify):TUserGroupInfo
OSPEVENT( EV_MODIFY_USER_GROUP,                 EV_NMC_NMS_BGN + 55 );

// ɾ���û���
// �û������û�����
// nmc -> nmcapp -> dbapp (EV_DEL_USER_GROUP::req): u32(�û���ID��
// nmc <- nmcapp <- dbapp (EV_DEL_USER_GROUP::ack): u32(�û���ID��
// nmc <- nmcapp <- dbapp (EV_DEL_USER_GROUP::nack):u32(�û���ID��  ������: ERROR_GROUP_NOT_EXIST
// �û������û�����
// nmc -> nmcapp (EV_DEL_USER_GROUP::req): u32(�û���ID��
// nmc <- nmcapp(EV_DEL_USER_GROUP_CONFIRM::req): u32(�û���ID��    ������: ERROR_USER_ONLINE
// nmc -> nmcapp(EV_DEL_USER_GROUP_CONFIRM::ack): u32(�û���ID��
//        nmcapp -> dbapp (EV_DEL_USER_GROUP::req): u32(�û���ID)
// nmc <- nmcapp <- dbapp (EV_DEL_USER_GROUP::ack): u32(�û���ID��
// nmc <- nmcapp <- dbapp (EV_DEL_USER_GROUP::nack):u32(�û���ID��  
OSPEVENT( EV_DEL_USER_GROUP,                    EV_NMC_NMS_BGN + 60 );
OSPEVENT( EV_DEL_USER_GROUP_CONFIRM,            EV_NMC_NMS_BGN + 61 );

// �����û�
// nmc -> nmcapp -> dbapp (req): TUserInfo
// nmc <- nmcapp <- dbapp (ack): TUserInfo
// nmc <- nmcapp <- dbapp (nack):TUserInfo      ������: ERROR_USER_EXIST
OSPEVENT( EV_ADD_USER,                          EV_NMC_NMS_BGN + 65 );

// �޸��û�
// nmc -> nmcapp -> dbapp (req): TUserInfo
// nmc <- nmcapp <- dbapp (ack):TUserInfo
// nmc <- nmcapp <- dbapp (nack):TUserInfo      ������: ERROR_USER_NOT_EXIST
OSPEVENT( EV_MODIFY_USER,                       EV_NMC_NMS_BGN + 70 );

// ɾ���û�
// �û�������
// nmc -> nmcapp -> dbapp (EV_DEL_USER::req): u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)
// nmc <- nmcapp <- dbapp (EV_DEL_USER::ack): u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)
// nmc <- nmcapp <- dbapp (EV_DEL_USER::nack):u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)     ������: ERROR_USER_NOT_EXIST
// �û�����
//          nmc -> nmcapp (EV_DEL_USER::req):u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)      
//          nmc <- nmcapp (EV_DEL_USER_CONFIRM::req):u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)������: ERROR_USER_ONLINE
//          nmc -> nmcapp (EV_DEL_USER_CONFIRM::ack):u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)������: ERROR_USER_ONLINE
//                 nmcapp -> dbqpp (EV_DEL_USER::req):u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)
//          nmc <- nmcapp <- dbapp (EV_DEL_USER::ack): u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)
//                   |
//nmc <- sericeapp <-|  (EV_DEL_USER::notify) : u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)
// nmc <- nmcapp <- dbapp (EV_DEL_USER::nack):u32(�û���ˮ���б��С) + u32�б�(�û���ˮ���б�)     ������: ERROR_USER_NOT_EXIST

OSPEVENT( EV_DEL_USER,                          EV_NMC_NMS_BGN + 75 );
OSPEVENT( EV_DEL_USER_CONFIRM,                  EV_NMC_NMS_BGN + 76 );
// �޸��û��ʻ���Ϣ
// nmc -> nmcapp -> dbapp (req): TUserInfo
// nmc <- nmcapp <- dbapp (ack): TUserInfo
// nmc <- nmcapp <- dbapp (nack): TUserInfo
OSPEVENT( EV_MODIFY_USER_INFO,                  EV_NMC_NMS_BGN + 80 );
// nmc -> nmcapp -> dbapp (req): TChangePwd
// nmc <- nmcapp <- dbapp (ack): TChangePwd 
// nmc <- nmcapp <- dbapp (nack): TChangePwd        ������:ERROR_PWD
OSPEVENT( EV_MODIFY_USER_PWD,                   EV_NMC_NMS_BGN + 85 );

//�û���  renhouping 2006.08.21
//nmcapp -> serviceapp(req):
//nms -> nmc(notify): u16( �����) + TFieldInfo����
OSPEVENT(EV_GET_FIELD_INFO, EV_NMC_NMS_BGN + 100);

//nms->nmc(notify): u8(��������) + u16(����) + TFieldInfo����
OSPEVENT(EV_UPDATE_FIELD_INFO, EV_NMC_NMS_BGN + 104); 


//////////////////////////////////////////////////////////////////////////
// ���Ի���Ϣ����
//
// NMC��ȡ���Ի���Ϣ
OSPEVENT(EV_GET_PREFER_INFO,                    EV_NMC_NMS_BGN + 130);

// ������Ի���Ϣ
OSPEVENT(EV_SAVE_PREFER_INFO,                   EV_NMC_NMS_BGN + 135);

// �ָ�Ĭ��ֵ
OSPEVENT(EV_RESTORE_SYS_PREFER_INFO,            EV_NMC_NMS_BGN + 140);

//////////////////////////////////////////////////////////////////////////
// �豸����ͼ��Ϣ
//
// 
// �õ��豸����ͼ
// nmc -> nmcapp -> serviceapp (req): 
// nmc <- nmcapp <- serviceapp (notify): u32(�����˵��豸����)+u16(�������) + TDevNmcTopo����
// ...
// nmc <- nmcapp <- serviceapp (finish): 
OSPEVENT(EV_GET_DEVICE_TOPO,                    EV_NMC_NMS_BGN + 190);
// ����豸
// nmc <- nmcapp <- serviceapp (notify): u16(�������) + TDevNmcTopo����
OSPEVENT(EV_DEVICE_TOPO_ADD,                    EV_NMC_NMS_BGN + 191);
// ɾ���豸
// nmc <- nmcapp <- serviceapp (notify): u16(�������) + u32���飨�豸ID��
OSPEVENT(EV_DEVICE_TOPO_DEL,                    EV_NMC_NMS_BGN + 192);
// �豸��Ϣ����
// nmc <- nmcapp <- serviceapp (notify): u16(�������) + TDevNmcTopo����
OSPEVENT(EV_DEVICE_TOPO_UPDATE,                 EV_NMC_NMS_BGN + 193);

// ��3AS�����������ͼ
// nmc -> nmcapp -> serviceapp:u8 (VALID_VALUE ǿ���������ӣ��������; INVALID_VALUE ������������ԣ��������������
//           ... -> serviceapp: 
OSPEVENT(EV_DEVICE_TOPO_REFRESH,                EV_NMC_NMS_BGN + 194);


// �õ��豸����״̬
// nmc <- nmcapp <- serviceapp (notify):
//     ����:u8(TNM_STATE_ONLINE) + TDeviceInfo(�豸��Ϣ)
//     ���� u8(TNM_STATE_OFFLINE) + u32(�豸��ˮ��)
OSPEVENT(EV_GET_DEV_ONLINE_INFO,                EV_NMC_NMS_BGN + 195);

//////////////////////////////////////////////////////////////////////////
// ���ù���
// 
// �õ��豸���ò���
// NMC -> NMS(req): u32(�豸��ˮ��)
// NMC <- NMs(ack): u32(�豸��ˮ��) + ���ò���
OSPEVENT(EV_GET_DEVICE_CFG_INFO,                EV_NMC_NMS_BGN + 240);

// �����豸�����ò���
// NMC -> NMS(req): u32(�豸��ˮ��) + ���ò���
// NMC <- NMS(ack): u32(�豸��ˮ��)
// NMC <- NMS(nack): u32(�豸��ˮ��)         ������:ERROR_CFG_DEV
OSPEVENT(EV_SET_DEVICE_CFG_INFO,                EV_NMC_NMS_BGN + 245);

// �õ����������ò���
// NMC -> NMS(req): 
// NMC <- NMS(ack): TNmsCfgInfo
OSPEVENT(EV_GET_NMS_CFG_INFO,                   EV_NMC_NMS_BGN + 250);
// ���÷��������ò���
// NMC -> NMS(req): TNmsCfgInfo
// NMC <- NMS(ack): TNmsCfgInfo
OSPEVENT(EV_SET_NMS_CFG_INFO,                   EV_NMC_NMS_BGN + 255);

// �����豸����汾
// nmc->nmcapp��nmcapp->serviceapp����
// NMC -> NMS(req): u8(BlockId�б��С�� + TTnmBlockInfo�б�(BlockId��Ϣ��)
//                  + u32(�豸ID�б��С�� + u32�б�(�豸ID�б�)
// NMC <- NMS(ack): 
OSPEVENT(EV_UPDATE_SOFTWARE,                    EV_NMC_NMS_BGN + 260);

// ����汾�зְ�
// NMC -> NMS(notify): u8(byBlockID) + u32(��Ƭ���) + u32(��Ч���ݳ���) + ��Ч����
// NMC <- NMS(ack): u8(byBlockID) + u32(��Ƭ���)
// NMC <- NMS(finish): u8(byBlockID)
OSPEVENT(EV_SOFTWARE_PACKAGE,                   EV_NMC_NMS_BGN + 270);

// NMC <- NMS(notify): u32(������Ϣ�����С��+ TProgressInfo����
OSPEVENT(EV_SOFTWARE_PROGRESS,                  EV_NMC_NMS_BGN + 275);
//�豸�ϴ��ļ����
//nms->nmc(notify): u32(�豸��ˮ��) + u8(�Ƿ�ɹ�: 0 ʧ�ܣ�1 �ɹ� ) + u8(�Ƿ�����: 1 ������0 ������) 
OSPEVENT(EV_SOFTWARE_COMPLETE,                  EV_NMC_NMS_BGN + 279);
//�ϴ��ļ�������Ϣ֪ͨ
//nms->nmc(notify):u32(�豸��ˮ��)
OSPEVENT(EV_SOFTWARE_UPDATE_ERROR,              EV_NMC_NMS_BGN + 283);
//�������ʧ�ܴ�����Ϣ(�����ϴ�����ʧ��)
OSPEVENT( EV_UPDATE_SOFTWARE_FAIL,              EV_NMC_NMS_BGN + 284);

//////////////////////////////////////////////////////////////////////////
// �豸����
//
// 
// �豸����
// NMC -> NMS(req): u32(�����豸������) + u32����(�豸��ˮ��)
//                   + u8 (�Ƿ��ǽ���������д����־VALID_VALUE/INVALID_VALUE,���򴥷��Ĳ�д������־)
// NMC <- NMS(ack): u32(�豸��ˮ��)
// NMC <- NMS(nack): u32(�豸��ˮ��)
OSPEVENT(EV_DEVICE_REBOOT,                      EV_NMC_NMS_BGN + 330); 

// �豸�Լ�
// NMC -> NMS(req): u32(�Լ��豸������) + u32����(�豸��ˮ��)
// NMC <- NMS(ack): u32(�豸��ˮ��)
// NMC <- NMS(nack): u32(�豸��ˮ��)
OSPEVENT(EV_DEVICE_SELF_TEST,                   EV_NMC_NMS_BGN + 335);

// �豸��·���
// NMC -> NMS(req): TPingReq
// NMC <- NMS(notify): TPingInfo
// NMC <- NMS(finish): u32(�豸��ˮ��)
OSPEVENT(EV_PING_DEVICE,                        EV_NMC_NMS_BGN + 340);
// ��·�����֪ͨ
// NMS <- Thread: TPingInfo
OSPEVENT(EV_PING_ECHO,                          EV_NMC_NMS_BGN + 341);
// ��·������֪ͨ
// NMS <- Thread: u32(�豸��ˮ��) + TPingStat
// NMC <- NMS: u32(�豸��ˮ��) + TPingStat
OSPEVENT(EV_PING_FINISH,                        EV_NMC_NMS_BGN + 342);
// ��ֹ�豸��·���
// NMC -> NMS(req): u32(�豸��ˮ��)
// NMC <- NMS(ack): u32(�豸��ˮ��)
OSPEVENT(EV_STOP_PING_DEVICE,                   EV_NMC_NMS_BGN + 343);

// �豸���ܷ���
OSPEVENT(EV_DEVICE_PFM_ANALYSE,                 EV_NMC_NMS_BGN + 345);

//////////////////////////////////////////////////////////////////////////
// ���Ϲ���ӿ�
//
// �õ��ʼ����͹���
// nmc -> nmcapp -> dbapp (req): 
// nmc <- nmcapp <- dbapp (notify): u32(����)+ TEmailRules����
// nmc <- nmcapp <- dbapp (finish):
//    serviceapp -> dbapp (req): 
//    serviceapp <- dbapp (notify): u32(����)+ TEmailRules����
//    serviceapp <- dbapp (finish):
OSPEVENT(EV_GET_EMAIL_RULES,                    EV_NMC_NMS_BGN + 380);
// �����ʼ����͹���
// nmc -> nmcapp -> dbapp (req): TEmailRules
// nmc <- nmcapp <- dbapp (ack): TEmailRules
//                    |
//       serviceapp <-| (notify):TEmailRules
// nmc <- nmcapp <- dbapp (nack): TEmailRules           ������:ERROR_ADD_MAIL_RULES_ALREADY_EXIST
OSPEVENT(EV_ADD_EMAIL_RULES,                    EV_NMC_NMS_BGN + 381);
// �޸��ʼ����͹���
// nmc -> nmcapp -> dbapp (req): TEmailRules
// nmc <- nmcapp <- dbapp (ack): TEmailRules
//                    |
//       serviceapp <-| (notify):TEmailRules
// nmc <- nmcapp <- dbapp (nack): TEmailRules           ������:ERROR_ADD_MAIL_RULES_ALREADY_EXIST
OSPEVENT(EV_MODIFY_EMAIL_RULES,                 EV_NMC_NMS_BGN + 382);
// ɾ���ʼ����͹���
// nmc -> nmcapp -> dbapp (req): u32(Ҫɾ����EMAIL������) + u32����(�ʼ�����ID����)
// nmc <- nmcapp <- dbapp (ack): u32(Ҫɾ����EMAIL������) + u32����(�ʼ�����ID����)
//                    |
//       serviceapp <-| (notify):u32(Ҫɾ����EMAIL������) + u32����(�ʼ�����ID����)
// nmc <- nmcapp <- dbapp (nack): u32(Ҫɾ����EMAIL������) + u32����(�ʼ�����ID����)            ������:ERROR_ADD_MAIL_RULES_ALREADY_EXIST
OSPEVENT(EV_DEL_EMAIL_RULES,                    EV_NMC_NMS_BGN + 383);

// �õ��澯���˹���
// nmc -> nmcapp -> dbapp (req): 
// nmc <- nmcapp <- dbapp (notify): u32(����)+TAlarmFilterRules����
// nmc <- nmcapp <- dbapp (finish):
//    serviceapp -> dbapp (req): 
//    serviceapp <- dbapp (notify): u32(����)+TAlarmFilterRules����
//    serviceapp <- dbapp (finish):
OSPEVENT(EV_GET_ALARM_FILTER_RULES,             EV_NMC_NMS_BGN + 390);
// ���Ӹ澯���˹���
// nmc -> nmcapp -> dbapp (req): TAlarmFilterRules
// nmc <- nmcapp <- dbapp (ack): TAlarmFilterRules
//                    |
//       serviceapp <-| (notify): TAlarmFilterRules
// nmc <- nmcapp <- dbapp (nack): TAlarmFilterRules     
OSPEVENT(EV_ADD_ALARM_FILTER_RULES,             EV_NMC_NMS_BGN + 391);
// nmc -> nmcapp -> dbapp (req): TAlarmFilterRules
// nmc <- nmcapp <- dbapp (ack): TAlarmFilterRules
//                    |
//       serviceapp <-| (notify): TAlarmFilterRules
// nmc <- nmcapp <- dbapp (nack): TAlarmFilterRules     
OSPEVENT(EV_MODIFY_ALARM_FILTER_RULES,          EV_NMC_NMS_BGN + 392);
// ɾ���澯���˹���
// nmc -> nmcapp -> dbapp (req): u32(Ҫɾ���Ĺ�������)+ u32����(�澯���˹���ID����)
// nmc <- nmcapp <- dbapp (ack): u32(Ҫɾ���Ĺ�������)+ u32����(�澯���˹���ID����)
//                    |
//      serviceapp <- | (notify): u32(Ҫɾ���Ĺ�������)+ u32����(�澯���˹���ID����)
// nmc <- nmcapp <- dbapp (nack): u32(Ҫɾ���Ĺ�������)+ u32����(�澯���˹���ID����)
OSPEVENT(EV_DEL_ALARM_FILTER_RULES,             EV_NMC_NMS_BGN + 393);

// �澯��Ϣ�ϱ�
// NMC <- NMS(notify): TDevAlarm
OSPEVENT(EV_DEVICE_ALARM_INFO,                  EV_NMC_NMS_BGN + 395);

// �澯֪ʶ��
// nmcapp(daemon) -> dbapp(req):u32(�������Ŀ��) + u32(��ʼ��¼��)
// nmcapp(daemon) <- dbapp(notify): u32(����Ŀ��) + u32(�������Ŀ��) + u32(������Ϣ����ʼ��¼��)
//                       + u16(����Ϣ����Ŀ��) + TAlarmKnowItem����
// nmcapp(daemon) <- dbapp(finish):�� 
// nmc <- nmcapp(daemon)(notify): u32(����Ŀ��) + u32(������Ϣ����ʼ��¼��)
//                       + u16(����Ϣ����Ŀ��) + TAlarmKnowItem����
// nmc <- nmcapp(daemon)(finish):��
OSPEVENT(EV_ALARM_KNOWLEDGE_INFO,               EV_NMC_NMS_BGN + 400);

// �澯֪ʶ��ά��3.9�ݲ���
//// ���Ӹ澯֪ʶ����Ŀ
//OSPEVENT(EV_ADD_ALARM_KNOWLEDGE_INFO,         EV_NMC_NMS_BGN + 405);
//
//// �޸ĸ澯֪ʶ����Ŀ
//OSPEVENT(EV_EDIT_ALARM_KNOWLEDGE_INFO,            EV_NMC_NMS_BGN + 410);
//
//// ɾ���澯֪ʶ����Ŀ
//OSPEVENT(EV_DEL_ALARM_KNOWLEDGE_INFO,         EV_NMC_NMS_BGN + 415);

// ��ѯ�澯��Ϣ
// NMC -> NMS(req): TAlarmQueryCondition + u32(�������Ŀ��) + u32(��ʼ��¼��) + u8(�Ƿ񽫲�ѯд����־VALID_VALUE/INVAID_VALUE)
// NMC <- NMS(notify): u32(ƥ����Ŀ������+ u32(�������Ŀ��) + u32(������Ϣ����ʼ��¼��)
//                       + u32(����Ϣ����Ŀ��) + TDevAlarm����
// db->nmcapp->nmc(nack):
// NMC <- NMS(finish):
OSPEVENT(EV_QUERY_ALARM_INFO,                   EV_NMC_NMS_BGN + 420);

// �ֶ�ɾ���澯��¼
// NMC -> NMS(req): u32(Ҫɾ���ļ�¼������+ u32���飨�澯��ID��
// NMC <- NMS(ack): 
// NMC <- NMS(nack):                        �����룺ERROR_ALARM_LOG_NOT_EXIST
OSPEVENT(EV_DEL_QUERY_ALARM_INFO,               EV_NMC_NMS_BGN + 425);

// �ֶ�ɾ������ƥ��ĸ澯��Ŀ
// nmc -> nmcapp(EV_DEL_ALL_QUERY_ALARM_INFO:req): TAlarmQueryCondition
//        nmcapp -> dbapp (EV_DEL_QUERY_ALARM_INFO:req): u32(Ҫɾ���ļ�¼����Ϊ0��+ TAlarmQueryCondition
//        nmcapp <- dbapp (EV_DEL_QUERY_ALARM_INFO:ack): u32(Ҫɾ���ļ�¼����Ϊ0��+ TAlarmQueryCondition
// nmc <- nmcapp (EV_DEL_ALL_QUERY_ALARM_INFO:ack): u32(Ҫɾ���ļ�¼����Ϊ0��+ TAlarmQueryCondition
// nmc <- nmcapp <- dbapp (nack):           �����룺ERROR_ALARM_LOG_NOT_EXIST
OSPEVENT(EV_DEL_ALL_QUERY_ALARM_INFO,           EV_NMC_NMS_BGN + 426);

// �ֶ�����澯��¼
// nmc -> nmcapp -> serviceapp (req): u32(Ҫ����ļ�¼����) + ��¼����[u32 (�豸��ˮ��) + u32(Ҫ����ĸ澯��¼ID)]
// nmc <- nmcapp <- serviceapp (ack): u32(Ҫ����ļ�¼����) + ��¼����[u32 (�豸��ˮ��) + u32(Ҫ����ĸ澯��¼ID)]
//                     |
//           dbapp  <- |
OSPEVENT(EV_CLEAR_ALARM_INFO,                   EV_NMC_NMS_BGN + 430);

//��ѯ�����豸�����¸澯��Ϣ
//��NMC��������ѯ�����豸�����¸澯
//nmc -> nms(req):  u32(�豸��ˮ��)
//nms -> nmc(ack):  u32(�豸��ˮ��) + u8(����) +��TDevAlarmState���� + u8(����) + TCuAlarmInfo����
OSPEVENT(EV_QUERY_ALLDEV_NEW_ALARM ,            EV_NMC_NMS_BGN + 433);

//ͳ��ĳ��ʱ�����ĳ���豸�������ϴ���
//nmc -> nms(req):    TTroubleStatCondition(����ͳ������) + u32(�豸ID)
//nms -> nmc(notify): u32(ͳ��ʱ���) + u32(�������ϴ���) 
OSPEVENT(EV_QUERY_TROUBLE_COUNT,                EV_NMC_NMS_BGN + 434);

//��ѯ�¸澯��Ϣ
//nmc -> nms(req):  u32(�豸��ˮ��)
//nms -> nmc(ack):  u32(�豸��ˮ��) + u8(����) +��TDevAlarmState���� + u8(����) + TCuAlarmInfo����
OSPEVENT(EV_QUERY_NEW_ALARM,                    EV_NMC_NMS_BGN + 435);

//ͳ��ĳ��ʱ����ڹ����豸����ʱ��
//nmc -> nms(req):    TTroubleStatCondition(����ͳ������)
//nms -> nmc(notify): u32(ͳ��ʱ���) + u32(�����豸����ʱ��) 
OSPEVENT(EV_QUERY_TROUBLE_RATE,                 EV_NMC_NMS_BGN + 436);

//������ϡ������ͳ�ƣ�����ͳ���뱨��
//nmc -> nms(req):    TTroubleStatCondition(����ͳ������)
//nms -> nmc(notify): u32(ͳ��ʱ���) + u32(�й����豸����) + 
//                    u32(�澯ͳ�Ƹ���) + �澯ͳ������[u32(�澯��) + u32(����澯�豸����)]
OSPEVENT(EV_QUERY_TROUBLE_STAT,                 EV_NMC_NMS_BGN + 437);

//�澯�Զ�ȷ��֪ͨ
//nms -> nmc(notify): 
OSPEVENT(EV_ALARM_AUTO_CONFIRM,                 EV_NMC_NMS_BGN + 438);

//////////////////////////////////////////////////////////////////////////
//  ���ܹ���
//
// ���ܲ����ϱ�
// NMC <- NMS(notify): u32(�豸ID��+ ���ܲ������б�
OSPEVENT(EV_DEVICE_PFM_INFO,                    EV_NMC_NMS_BGN + 480 );

// �ͻ���ע��������Ϣ
// NMC -> NMS(req): TNmcReqInfo
// NMC <- NMS(ack): TNmcReqInfo
// NMC <- NMS(nack): TNmcReqInfo
OSPEVENT( EV_NMC_REG_PFM_REQ,                   EV_NMC_NMS_BGN + 485 );
// �ͻ���ע��������Ϣ
// NMC -> NMS(req): TNmcReqInfo
// NMC <- NMS(ack): TNmcReqInfo
// NMC <- NMS(nack): TNmcReqInfo
OSPEVENT( EV_NMC_UNREG_PFM_REQ,                 EV_NMC_NMS_BGN + 486 );

// nmc -> nmcapp -> serviceapp (req): u32(CMU���豸ID) + u32(CU�ĻỰID)
// nmc <- nmcapp <- serviceapp (EV_DEVICE_PFM_INFO:notify): u32(CMU���豸ID��+ TTnmCuLogon
OSPEVENT( EV_NMC_CULOG_REQ,                     EV_NMC_NMS_BGN + 487 );

// nmc -> nmcapp -> serviceapp (req): u32(CMU���豸ID) + u32(NODE ID)
OSPEVENT( EV_DEVICE_PFM_LIST_REQ,               EV_NMC_NMS_BGN + 490);

//

//////////////////////////////////////////////////////////////////////////
// ��ȫ����
// 
// �õ�IP���ƹ���
// nmc -> nmcapp -> dbapp (req): 
// nmc <- nmcapp <- dbapp (notify):u32(��������+ TIpRules����
// nmc <- nmcapp <- dbapp (finish):
//
// nmcapp(daemon) -> dbapp (req): 
// nmcapp(daemon) <- dbapp (notify):u32(��������+ TIpRules����
// nmcapp(daemon) <- dbapp (finish):
OSPEVENT( EV_GET_IP_RULES,                      EV_NMC_NMS_BGN + 530);
// ����IP����
// nmc -> nmcapp -> dbapp (req): TIpRules
// nmc <- nmcapp <- dbapp (ack): TIpRules��������Ч��ID��
// nmcapp(daemon) <- dbapp (notify): TIpRules��������Ч��ID��
OSPEVENT( EV_ADD_IP_RULES,                      EV_NMC_NMS_BGN + 531);
// ɾ��IP����
// nmc -> nmcapp -> dbapp (req): u32(Ҫɾ����IP������)+u32���飨IP����ID���飩
// nmc <- nmcapp <- dbapp (ack): u32(Ҫɾ����IP������)+u32���飨IP����ID���飩
// nmcapp(daemon) <- dbapp (notify): u32(Ҫɾ����IP������)+u32���飨IP����ID���飩
OSPEVENT( EV_DEL_IP_RULES,                      EV_NMC_NMS_BGN + 532);
// �޸�IP����
// nmc -> nmcapp -> dbapp (req): TIpRules
// nmc <- nmcapp <- dbapp (ack): TIpRules
// nmcapp(daemon) <- dbapp (notify): TIpRules
OSPEVENT( EV_MODIFY_IP_RULES,                   EV_NMC_NMS_BGN + 533);

// �������ݿ�����
// ������ʽ��NMC���ȷ���EV_BACKUP_DISK_INFO��ʾ���õ�NMS������Ϣ���ٷ���
// Ӧ�Ĳ���ָ��
// �������ݿ���Ϣ
// NMC -> NMS(req): 
// NMC <- NMS(ack): u8(�����ļ���ǰ׺����) + s8����(�ļ���ǰ׺)+ u8(��������) + TDiskInfo����
OSPEVENT(EV_BACKUP_DISK_INFO,                   EV_NMC_NMS_BGN + 540);
// NMC -> NMS(req): u8(����·�������ȣ�+ ����·����
// NMC <- NMS(ack): 
// NMC <- NMS(finish):  
OSPEVENT(EV_BACKUP_NMS_DATA,                    EV_NMC_NMS_BGN + 541);
// ��ȡ�Զ����ݵ�����
// nmc -> nmcapp (req): 
// nmc <- nmcapp (ack): TAutoBackup + u8(����·�������ȣ�+ ����·����
// nmc <- nmcapp (nack):
OSPEVENT(EV_GET_AUTO_BACKUP_CFG,                EV_NMC_NMS_BGN + 542);
// �޸��Զ����ݵ�����
// NMC -> NMS(req): TAutoBackup + u8(����·�������ȣ�+ ����·����
// NMC <- NMS(ack): 
OSPEVENT(EV_AUTO_BACKUP_NMS_DATA,               EV_NMC_NMS_BGN + 543);
// �ָ����ݿ�����
// NMC -> NMS(req): u8(����·�������ȣ�+ ����·����
// NMC <- NMS(ack): 
// NMC <- NMS(finish):  
OSPEVENT(EV_RESTORE_NMS_DATA,                   EV_NMC_NMS_BGN + 545);

// �ֶ���������
// NMC -> NMS(notify): 
OSPEVENT(EV_NMC_LOCK_UI,                        EV_NMC_NMS_BGN + 550);

// ��������
// NMC -> NMS(notify): 
OSPEVENT(EV_NMC_UNLOCK_UI,                      EV_NMC_NMS_BGN + 565);

//////////////////////////////////////////////////////////////////////////
// ������־����
//
// ��¼��־����
// nmc -> nmcapp -> dbapp(req): TActionLogItem
OSPEVENT(EV_REC_ACT_LOG,                        EV_NMC_NMS_BGN + 600);
// ��ѯ������־
// NMC -> NMS(req): TActionQueryCondition + u32(����ļ�¼��) + u32(��ʼ��¼��) + u8(�Ƿ񽫲�ѯд����־VALID_VALUE/INVAID_VALUE)
// NMC <- NMS(notify): u32(ƥ����Ŀ������+ u32(�������Ŀ��) + u32(������Ϣ����ʼ��¼��)
//                       + u32(����Ϣ����Ŀ��) + TActionLogItem����
// NMC <- NMS(finish): 
OSPEVENT(EV_QUERY_ACT_LOG,                      EV_NMC_NMS_BGN + 605);

// ɾ��������־
// nmc -> nmcapp -> dbapp (req): u32(Ҫɾ���ļ�¼������+ u32���飨��־����ˮ�ţ�
// nmc <- nmcapp <- dbapp (ack): u32(ʵ��ɾ���ļ�¼������+ u32���飨��־����ˮ�ţ�
// nmc <- nmcapp <- dbapp(nack):                        
OSPEVENT(EV_DEL_QUERY_ACT_LOG,                  EV_NMC_NMS_BGN + 610);

// ɾ������ƥ��Ĳ�����־
// nmc -> nmcapp (EV_DEL_ALL_QUERY_ACT_LOG:req):  TActionQueryCondition
//        nmcapp -> dbapp (EV_DEL_QUERY_ACT_LOG:req): u32(Ҫɾ���ļ�¼����Ϊ0��+ TActionQueryCondition
//        nmcapp <- dbapp (EV_DEL_QUERY_ACT_LOG:ack): u32(Ҫɾ���ļ�¼����Ϊ0��+ TActionQueryCondition
// nmc <- nmcapp (EV_DEL_ALL_QUERY_ACT_LOG:ack): TActionQueryCondition
// nmc <- nmcapp <- dbapp (nack):                       
OSPEVENT(EV_DEL_ALL_QUERY_ACT_LOG,              EV_NMC_NMS_BGN + 611);


// ����������־
// nmc -> nmcapp (notify): u8(��ʼVALID_VALUE/����INVALID_VALUE)
OSPEVENT(EV_EXPORT_ACT_LOG,                     EV_NMC_NMS_BGN + 615);

// �����澯��־
// nmc -> nmcapp (notify): u8(��ʼVALID_VALUE/����INVALID_VALUE)
OSPEVENT(EV_EXPORT_ALARM_LOG,                   EV_NMC_NMS_BGN + 620);

//�����ܿͻ���ͬ��ʱ��
//nms -> nmc(notify): u32(ʱ��ֵ)
OSPEVENT(EV_SYNTIME_MSG,                        EV_NMC_NMS_BGN + 625);

//////////////////////////////////////////////////////////////////////////
// �豸������Ϣ
//get �豸����
//nms->nmc(notify): u16(�������)+TDevGroup����
//nms->nmc(finish):
OSPEVENT(EV_DEV_GROUP_GET,                      EV_NMC_NMS_BGN  + 630);
//get �豸������Ϣ
//nms->nmc(notify):u16(�������)+TDevGroupMem����
//nms->nmc(finish):
OSPEVENT(EV_DEV_GROUP_MEM_GET,                  EV_NMC_NMS_BGN  + 635);
//����豸��
//nmc->nms(req): TDevGroup
//nms->nmc(ack): TDevGroup
//nms->nmc(nack):
OSPEVENT(EV_DEV_GROUP_ADD,                      EV_NMC_NMS_BGN  + 640);
//�޸��豸����Ϣ
//nmc->nms(req):TDevGroup
//nms->nmc(ack):TDevGroup
//nms->nmc(nack):
OSPEVENT(EV_DEV_GROUP_MODIFY,                   EV_NMC_NMS_BGN  + 645);
//���������豸��Ա
//nmc->nms(req):u16(���ӵ��豸����) + TDevGroupMem����
//nms->nmc(ack):u16(���ӵ��豸����) + TDevGroupMem����
//nms->nmc(nack):
OSPEVENT(EV_DEV_GROUP_MEM_ADD,                  EV_NMC_NMS_BGN  + 650);
//ɾ�������豸��Ա
//nmc->nms(req):u16(ɾ�����豸����) + u32����
//nms->nmc(ack):u16(ɾ�����豸����) + u32����
//nms->nmc(nack):
OSPEVENT(EV_DEV_GROUP_MEM_DEL,                  EV_NMC_NMS_BGN  + 655);
//ɾ���豸��
//nmc->nms(req):u32(DevGroupId)
//nms->nmc(ack):u32(DevGroupId)
//nms->nmc(nack);
OSPEVENT(EV_DEV_GROUP_DEL,                      EV_NMC_NMS_BGN  + 660);
//////////////////////////////////////////////////////////////////////////

inline u32 GetAlarmLevel(u32 dwAlarmCode);
inline s8* GetAlarmLevelDesc(u32 dwLevel);
inline s8* GetActTypeDesc(s32 nActIdx);
inline u32 GetCurTime();
// mask��Ϣ
typedef struct tagTnmMask
{   
public:
    tagTnmMask()
    {
        dwMask = 0;
    }
    BOOL32 IsItemValid(u32 dwItemMask) const        // �ж�ĳһ�������Ƿ���Ч
    {
        return (dwMask & dwItemMask) != 0;
    }
    /*========================================================================
    �� �� ����SetItemMask
    ��    �ܣ�����һ�������ֶ���Чλ��
    ��    ����u32 dwItemMask                        [in]�µ�MASKֵ
              BOOL32 bValid = TRUE                  [in]���û������
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/03  3.9     ���ǿ  ����
    ========================================================================*/
    inline void SetItemMask(u32 dwItemMask, BOOL32 bValid = TRUE);
    u32  GetMaskInfo() const { return dwMask; }             // ���ص�ǰ������mask��Ϣ
    
    /*========================================================================
    �� �� ����SetMaskAll
    ��    �ܣ��ֶ�������MASK��ֵ����
    ��    ����u32 dwMask                    [in]�µ�MASKֵ
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/03  3.9     ���ǿ  ����
    ========================================================================*/
    inline void SetMaskAll(u32 dwItemMask);
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
    // 2006/05/22 ���ǿ ���ӵ��Դ�ӡ�ӿ�
    void Print(BOOL32 bNetToHost = FALSE);
protected:
    u32 dwMask;                                 // ��λ�룬��Щ���������á� 1Ϊ��Ч��0��Ч
}PACKED TTnmMask;

inline void PrintDevRight(TTnmMask const *ptDevRight);


//�û���Ϣ�ṹ
typedef struct tagUserInfo
{
    tagUserInfo()
    {
        memset(this, 0, sizeof(tagUserInfo));
        dwSerialNo = INVALID_SERIAL_NO;
        dwGroupId = INVALID_SERIAL_NO;
        byGender = GENDER_MALE;
    }
    u32     dwSerialNo;                         // �û���ˮ��
    u32     dwGroupId;                          // �û�������ID
    s8      achUserId[MAX_USER_ID_LEN + 4];     // �û���¼ID
    s8      achUserPwd[MAX_USER_PWD_LEN + 4];   // �û���¼���� 
    s8      achUserName[MAX_USER_NAME_LEN + 2]; // �û�����
    s8      achDept[MAX_DEPT_NAME_LEN + 4];     // ��������
    s8      achWorkId[MAX_WORK_ID_LEN + 4];     // ����
    s8      achRank[MAX_RANK_LEN + 2];          // ְ��
    s8      achAddress[MAX_ADDR_LEN + 4];       // ��ϵ��ַ
    s8      achTelePhone[MAX_TELEPHONE_LEN + 4];// �̶��绰
    s8      achMobile[MAX_MOBILE_LEN + 4];      // �ƶ��绰
    s8      achEmail[MAX_EMAIL_LEN + 4];        // Email
    s8      achRemark[MAX_REMARK_LEN + 4];      // ��ע
    u8      byGender;                           // �û��Ա�GENDER_MALE/GENDER_FEMALE

    u8      byReserve1;
    u8      byReserve2;
    u8      byReserve3;

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            OspPrintf(TRUE, FALSE, "\tSerialNo: %x; UserId : %s; \n", ntohl(dwSerialNo), achUserId);
            OspPrintf(TRUE, FALSE, "\tGroupId : %x; UserPwd: %s; Gender: %d; \n", 
                        ntohl(dwGroupId), achUserPwd, byGender);            
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tSerialNo: %x; UserId : %s; \n", dwSerialNo, achUserId);
            OspPrintf(TRUE, FALSE, "\tGroupId : %x; UserPwd: %s; Gender: %d; \n", 
                        dwGroupId, achUserPwd, byGender);
        }       
        OspPrintf(TRUE, FALSE, "\tUserName: %s; UserDpt: %s; \n", achUserName, achDept);
        OspPrintf(TRUE, FALSE, "\tWorkId  : %s; UsrRank: %s; \n", achWorkId, achRank);
        OspPrintf(TRUE, FALSE, "\tAddress : %s; TelCall: %s; \n", achAddress, achTelePhone);
        OspPrintf(TRUE, FALSE, "\tMobile  : %s; Email  : %s; \n", achMobile, achEmail);
        OspPrintf(TRUE, FALSE, "\tRemark  : %s; \n", achRemark);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TUserInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TUserInfo::NetToHost()
{
    dwSerialNo = ntohl(dwSerialNo);
    dwGroupId = ntohl(dwGroupId);
}
inline void TUserInfo::HostToNet()
{
    dwSerialNo = htonl(dwSerialNo);
    dwGroupId = htonl(dwGroupId);
}

// �����û���¼���ݣ�ͬʱ���������޸��û�����
typedef struct tagNmsLogonReq
{
    tagNmsLogonReq()
    {
        memset(this, 0, sizeof (tagNmsLogonReq));
    }
    s8   achUserId[TNM_MAX_USER_ID_LEN + 1];            // �û���¼ID
    s8   achUserPwd[TNM_MAX_PASSWORD_LEN + 1];          // �û���¼����
}PACKED TNmsLogonReq;


// �����û���¼�������ݽṹ
typedef struct tagNmsLogonRet
{
    tagNmsLogonRet()
    {
        dwResult = 0;
    }

    u32 dwResult;                       // ��¼���
    TUserInfo tUserInfo;                // ��¼�û�����ϸ��Ϣ               

    void Print(BOOL bNettoHost = FALSE)
    {
        tUserInfo.Print(bNettoHost);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TNmsLogonRet;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TNmsLogonRet::NetToHost()
{
    tUserInfo.NetToHost();
}
inline void TNmsLogonRet::HostToNet()
{
    tUserInfo.HostToNet();
}

// �������ܲ���������
typedef union tagPfmInfoType
{
    TTnmCmuPfmInfo  tCmu;                       // ƽ̨���ܲ���
    TTnmVpuPfmInfo  tVpu;                       // VPU�����ܲ���
    TTnmNruPfmInfo  tNru;                       // NRU���ܲ���
    TTnm3asPfmInfo  t3as;                       // 3AS���ܲ���
    TTnmUasPfmInfo  tUas;                       // UAS���ܲ���
    TTnmPuPfmInfo   tPu;                        // PU���ܲ���
    TTnmVtduPfmInfo tVtdu;                      // VTDU���ܲ���
    TTnmCuiPfmInfo  tCui;                       // CUI���ܲ���
    TTnmPuiPfmInfo  tPui;                       // PUI���ܲ���
    TTnmNmsPfmInfo  tNms;                       // NMS���ܲ���
    TTnmDec5PfmInfo tDec5;                      // DEC5���ܲ��� 
    
    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        memset(this, 0, sizeof(tagPfmInfoType));
    }
}UPfmInfoType;

//renhouping 2006.08.04 �޸ĸ澯ģ��
#define CU_ALARMCODE_NUM                    2
#define MAX_ALARM_CU_NUM_ONE_ALARMCODE      32

#define MAX_ALARM_CHANEL_ONE_ALARMCODE      16
typedef struct tagTAlarmBaseInfo
{
    tagTAlarmBaseInfo()
    {
        byAlarmState = TNM_STATUS_NORMAL;
        dwNmsId = INVALID_SERIAL_NO;
        dwOccurTime = 0;
    }
    u32 dwNmsId;            // ��¼�澯�����ݿ���ˮ�ţ���Ҫ���ڸ澯�ָ�ʱ��ƥ��
    u32 dwOccurTime;        // �澯������ʱ��
    u8  byAlarmState;       // �澯״̬ enum TnmStatusType, ����TNM_STATUS_CLEAR
                        // ״̬ʱ�����ϱ�ͬ��澯ʱ״̬���ı䡣ֻ���յ�
                        // TNM_STATUS_NORMALʱ��״̬���±���Ϊ����  
    u8  byConfirmState;
    u8  byReserved2;
    u8  byReserved3;
    //�ֽ�ת���ӿ�
    void NetToHost();
    void HostToNet();

    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwAlarmSn = dwNmsId;
        if (bNetToHost)
        {
            dwAlarmSn = ntohl(dwAlarmSn);
        }
        OspPrintf(TRUE, FALSE, "AlarmSn: %d; AlarmState: %d;\n", dwAlarmSn, byAlarmState);
    }
}TAlarmBaseInfo;

inline void TAlarmBaseInfo::NetToHost()
{
    dwNmsId = ntohl(dwNmsId);
    dwOccurTime = ntohl(dwOccurTime);
}
inline void TAlarmBaseInfo::HostToNet()
{
    dwNmsId = htonl(dwNmsId);
    dwOccurTime = htonl(dwOccurTime);
}

// �豸�澯״̬
typedef struct tagDevAlarmState : public TTnmDevAlarm
{
    TAlarmBaseInfo tAlarmBase[MAX_ALARM_CHANEL_ONE_ALARMCODE];
    void NetToHost();
    void HostToNet();
    void Print(BOOL32 bNetToHost = FALSE);
    BOOL32 IsAlarmState();             //��ǰ�ø澯���״̬�Ƿ�TNM_STATUS_ALARM
}TDevAlarmState;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TDevAlarmState::NetToHost()
{
    TTnmDevAlarm::NetToHost();
    s32 nIndex = 0;
    for (; nIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nIndex++)
    {
        tAlarmBase[nIndex].NetToHost();
    }
}

inline void TDevAlarmState::HostToNet()
{
    TTnmDevAlarm::HostToNet();
    s32 nIndex = 0;
    for (; nIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nIndex++)
    {
        tAlarmBase[nIndex].HostToNet();
    }
}

inline void TDevAlarmState::Print(BOOL32 bNetToHost)
{
    TTnmDevAlarm::Print(bNetToHost);
    OspPrintf(TRUE, FALSE, "\n");
    s32 nIndex = 0;
    for (; nIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "\t");
        tAlarmBase[nIndex].Print(bNetToHost);
    }
}

inline BOOL32 TDevAlarmState::IsAlarmState()
{
    s32 nIndex = 0;
    for (; nIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nIndex++)
    {
        if (tAlarmBase[nIndex].byAlarmState == TNM_STATUS_ALARM)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//
typedef struct tagCuAlarmInfo
{
    tagCuAlarmInfo()
    {
        dwAlarmSN = 0;
        dwDevAlarmSN = 0;
        dwOccurTime = 0;
        byAlarmState = TNM_STATUS_NORMAL;
        byConfirmState = TNM_STATUS_NOCONFIRM;
        byHasAlarmInfo = TNM_DISABLE;
    }
    u32 dwAlarmSN;          //��¼�����ݿ��еĸ澯���
    u32 dwDevAlarmSN;       //�豸�˷����������
    u32 dwOccurTime;        // �澯������ʱ��
    u8  byAlarmState;       // �澯״̬ enum TnmStatusType
    u8  byConfirmState;
    u8  byHasAlarmInfo;
    u8  byReserved;
    u16 wAlarmCode;
    u8  byCuAlarmLevel;
    u8  byReserved2;
    s8  achAlarmDesc[MAX_ALARM_DESC_LEN+1];//�澯����

    void NetToHost();
    void HostToNet();
}TCuAlarmInfo;

inline void TCuAlarmInfo::NetToHost()
{
    dwAlarmSN = ntohl(dwAlarmSN);
    dwDevAlarmSN = ntohl(dwDevAlarmSN);
    dwOccurTime = ntohl(dwOccurTime);
    wAlarmCode = ntohs(wAlarmCode);
}

inline void TCuAlarmInfo::HostToNet()
{
    dwAlarmSN = htonl(dwAlarmSN);
    dwDevAlarmSN = htonl(dwDevAlarmSN);
    dwOccurTime = htonl(dwOccurTime);
    wAlarmCode = htons(wAlarmCode);
}

// �豸��Ϣ�ṹ
typedef struct tagDeviceInfo : public TTnmDevTopo
{   
    tagDeviceInfo() : tagTnmDevTopo()
    {
        tUnitType.byDeviceType = TNM_UNIT_UNKNOW;
        tUnitType.byServiceType = TNM_SERVICE_UNKNOW;
        dwDevSerialNo = INVALID_SERIAL_NO;
        dwMasterBoardSn = INVALID_SERIAL_NO;
        memset(achAlias, 0, sizeof(achAlias));
        dwIpAddr = 0;
        byField = INVALID_VALUE;
        byRightRange = RIGHT_RANGE_ALL_CHILD;
        byOnline = TNM_STATE_OFFLINE;
        byLayer = 0;
        bySlot = 0;
        byUpgradeMode = TNM_UPGRADE_MODE_NONE;
        byIsOldDevice = TNM_DISABLE;
        memset(achDevTypeName, 0, sizeof(achDevTypeName));
        memset(achBoxID, 0, sizeof(achBoxID));
        ResetAlarm();
        // ��ʼ���澯��
        atDevAlarm[0].dwAlarmCode = TNM_ALARM_CODE_DEV_OFFLINE;
        atDevAlarm[1].dwAlarmCode = TNM_ALARM_CODE_RECV_NOBITSTREAM;
        atDevAlarm[2].dwAlarmCode = TNM_ALARM_CODE_LOSEPACKET;
        atDevAlarm[3].dwAlarmCode = TNM_ALARM_CODE_MAP_STATUS;      
        atDevAlarm[4].dwAlarmCode = TNM_ALARM_CODE_LPT_STATUS;
        atDevAlarm[5].dwAlarmCode = TNM_ALARM_CODE_POWER_STATUS;
        atDevAlarm[6].dwAlarmCode = TNM_ALARM_CODE_CPU_STATUS;
        atDevAlarm[7].dwAlarmCode = TNM_ALARM_CODE_MEMORY_STATUS;
        atDevAlarm[8].dwAlarmCode = TNM_ALARM_CODE_DISKFULL_STATUS;
        atDevAlarm[9].dwAlarmCode = TNM_ALARM_CODE_FAN_STATUS;
        atDevAlarm[10].dwAlarmCode = TNM_ALARM_CODE_ETHCARD_RESTORE;
        atDevAlarm[11].dwAlarmCode = TNM_ALARM_CODE_LED_STATUS;
        atDevAlarm[12].dwAlarmCode = TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS;
        atDevAlarm[13].dwAlarmCode = TNM_ALARM_CODE_SAME_REG_ID;
        atDevAlarm[14].dwAlarmCode = TNM_ALARM_CODE_REG_CMU;
        atDevAlarm[15].dwAlarmCode = TNM_ALARM_CODE_DISKDETECT_ERROR;
        atDevAlarm[16].dwAlarmCode = TNM_ALARM_CU_OVERLOAD;
        atDevAlarm[17].dwAlarmCode = TNM_ALARM_PU_OVERLOAD;
        atDevAlarm[18].dwAlarmCode = TNM_ALARM_VTDU_OVERLOAD;
		atDevAlarm[19].dwAlarmCode = TNM_ALARM_CODE_NODISK_ERROR;
		atDevAlarm[20].dwAlarmCode = TNM_ALARM_CODE_DISK_ERROR;
		atDevAlarm[21].dwAlarmCode = TNM_ALARM_CODE_DISK_NOFORMAT;
		atDevAlarm[22].dwAlarmCode = TNM_ALARM_CODE_STOPREC_LOGERR;
        atDevAlarm[23].dwAlarmCode = TNM_ALARM_CODE_STOPREC_DISKERR;
        atDevAlarm[24].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_3AS;
        atDevAlarm[25].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_UAS;
        atDevAlarm[26].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_PUI;
        atDevAlarm[27].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_CUI;
        atDevAlarm[28].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_NRU;
        atDevAlarm[29].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_VTDU;
        atDevAlarm[30].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_CMU;
        atDevAlarm[31].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_DATABASE;   
    #ifdef BOGUS
        for (u16 wII = 0; wII < CU_ALARMCODE_NUM; wII++)
        {
            for (u16 wIII = 0; wIII < MAX_ALARM_CU_NUM_ONE_ALARMCODE; wIII++)
            {
                if (0 == wII)
                {
                    atCuAlarm[wII][wIII].wAlarmCode = TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET;
                }
                else if (1 == wII)
                {
                    atCuAlarm[wII][wIII].wAlarmCode = TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION;
                }
            }
        }
    #endif /* BOGUS */
        
        InitPfm();
    }
    TTnmUnitType tUnitType;                             //��Ԫ����
    u32 dwDevSerialNo;                                  //�豸��ˮ��.
    u32 dwMasterBoardSn;                                //����������豸��ˮ��
    s8  achAlias[MAX_DEVICE_ALIAS_LEN + 4];             //�豸����  
    u32 dwIpAddr;                                       //�豸��Ip��ַ
    TDevAlarmState atDevAlarm[MAX_DEV_ALARM_TYPE];      //�豸��ǰ�ĸ澯״̬
#ifdef BOGUS
    TCuAlarmInfo    atCuAlarm[CU_ALARMCODE_NUM][MAX_ALARM_CU_NUM_ONE_ALARMCODE];
#endif /* BOGUS */
    UPfmInfoType   tPfmInfo;                            //���ܲ�����Ϣ
    u8  byField;                                        //byField����Ľڵ��Ƿ�����
    u8  byRightRange;                                   //Ȩ�޵����ͣ����ֶζԵ�����NMC�ͻ��������壩
                                                        //RIGHT_RANGE_ALL_CHILD
                                                        //RIGHT_RANGE_NO_CHILD
                                                        //RIGHT_RANGE_PART_CHILD
    u8  byOnline;                                       //��ǰ�豸״̬���Ƿ�����
    u8  byHighestAlarmLevel;                            //��߸澯����
    u8  bySelfAlarmLevel;                               //���豸����߸澯����
    u8  byLayer;                            
    u8  bySlot;
    u8  byUpgradeMode;                                  //TNM_UPGRADE_MODE_TCP/TNM_UPGRADE_MODE_FTP

    u8  byIsOldDevice;                                  //TNM_ENABLE/TNM_DISABLE
    u8  byReserved;
    u16 wReserved;

    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //�豸������
    s8  achBoxID[MAX_BOX_ID_LEN + 4];   //�����ʶ

    /*========================================================================
    �� �� ����UpdateHighestAlarmLevel
    ��    �ܣ�u8 byLevel                    [in]�µĸ澯����
              BOOL32 bSet                   [in]TRUE: ���µ�ֵȡ��ԭ��ֵ
                                            FALSE�����µ�ֵ�����澯����
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/23  3.9     ���ǿ  ����
    ========================================================================*/
    void UpdateHighestAlarmLevel(u8 byLevel, BOOL32 bSet);
    // 2006/03/23 ���ǿ �õ����豸����߸澯����
    u8 GetSelfAlarmLevel() const
    {
        return bySelfAlarmLevel;
    }
    // 2006/03/23 ���ǿ �õ����豸�����������豸����߸�澯����
    u8 GetHighestAlarmLevel() const
    {
        return byHighestAlarmLevel;
    }
    // 2006/03/22 ���ǿ ����/�õ�����״̬
    BOOL32 IsOnline() const
    {
        return byOnline == TNM_STATE_ONLINE;
    }
    void SetOnline(u8 byOnlineState)
    {
        byOnline = byOnlineState;
    }
    /*========================================================================
    �� �� ����IsField
    ��    �ܣ��ж��豸�Ƿ�Ϊ���ƽ̨�豸
    ��    ����BOOL32 bCheckSubBoard = FALSE     [in]�ж�ʱ�Ƿ�������Ӱ�,���Ϊ
                            TRUE����ֻ���¹ҵ���Ĳŷ���TRUE�����ΪFALSE��
                            ֻҪ��ƽ̨ҵ�������TRUE
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/07  3.9     ���ǿ  ʵ��
    ========================================================================*/
    BOOL32 IsField(BOOL32 bCheckSubBoard = FALSE, BOOL32 bCheckById = FALSE) const;
    /*========================================================================
    �� �� ����GetPfmInfo
    ��    �ܣ��õ��������ܲ���
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/21  3.9     ���ǿ  ����
    ========================================================================*/
    inline void*  GetPfmInfo(s32 &nPfmSize) const;                  // �õ��豸�����ܲ�����Ϣ
    /*========================================================================
    �� �� ����GetBoardPfmInfo
    ��    �ܣ��õ�����Ļ������ܲ���
    ��    ����
    �� �� ֵ������Ļ������ܲ���ָ��:   �ɹ�        NULL��ʧ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/21  3.9     ���ǿ  ����
    ========================================================================*/
    const TTnmBoardPfmInfo * GetBoardPfmInfo() const;
    /*========================================================================
    �� �� ����IsBoard
    ��    �ܣ��жϸ��豸�Ƿ�Ϊ�����еĵ���
    ��    ����
    �� �� ֵ��TRUE: �ǵ���          FALSE:  ���������豸
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/22  3.9     ���ǿ  ����
    ========================================================================*/
    BOOL32 IsBoard() const;
    /*========================================================================
    �� �� ����UpdateAlarmState
    ��    �ܣ����±��ر���ĸ澯״̬
    ��    ����TTnmDevAlarm * const ptDevAlarm       [in]�澯������
              u8 byState                            [in]�澯�������ǻָ�
                                                    TNM_STATUS_ALARM/TNM_STATUS_NORMAL
              u8 byAlarmLevel                       [in]Ҫ���µĸ澯����
              u32 dwNmsId = 0                       [in]NMS����ĸ澯ID��
              u32 dwOccurTime = 0                   [in]�澯������ʱ��
    �� �� ֵ���澯״̬�Ƿ����仯����Ҫ��¼��֪ͨ�ͻ���
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/22  3.9     ���ǿ  ����
    ========================================================================*/
    BOOL32 UpdateAlarmState(TTnmDevAlarm * const ptDevAlarm, u8 byState, u8 byAlarmLevel,
                u32 dwNmsId = 0, u32 dwOccurTime = 0, u8 byParam = 0);
    
    /*========================================================================
    �� �� ����RecalcAlarmLevel
    ��    �ܣ����¼��㱾�豸�ĸ澯����
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/26  3.9     ���ǿ  ����
    ========================================================================*/
    void RecalcAlarmLevel();

    /*========================================================================
    �� �� ����ClearAlarm
    ��    �ܣ��ֶ�����澯״̬
    ��    ����u32 dwAlarmSn                         [in]Ҫ����ĸ澯��ˮ��
    �� �� ֵ��TRUE: �ɹ�        FALSE��ʧ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/22  3.9     ���ǿ  ����
    ========================================================================*/
    BOOL32 ClearAlarm(u32 dwAlarmSn, u8 byState);
    
    /*========================================================================
    �� �� ����GetDevDesc
    ��    �ܣ��õ��豸������(�豸����+�豸����)
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/15  3.9     ���ǿ  ����
    ========================================================================*/
    BOOL32 GetDevDesc(s8 *pbyDescBuf, s32 nLen) const;
    /*========================================================================
    �� �� ����InitPfm
    ��    �ܣ���ʼ�����ܲ���
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/03  3.9     ���ǿ  ����
    ========================================================================*/
    void InitPfm();
    
    /*========================================================================
    �� �� ����ResetAlarm
    ��    �ܣ������豸�澯��Ϣ
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/18  3.9     ���ǿ  ����
    ========================================================================*/
    void ResetAlarm();

    /*========================================================================
    �� �� ����SetAlarmNmsId
    ��    �ܣ����ø澯����ˮ��
    ��    ����u32 dwAlarmCode               [in]�澯��
              u32 dwNmsId                   [in]�澯����ˮ��
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/18  3.9     ���ǿ  ����
    ========================================================================*/
    void SetAlarmNmsId(u32 dwAlarmCode, u32 dwNmsId, u8 byParam, u32 dwDevAlarmSN);
    
    void ResetAlarmState(u32 dwAlarmCode, u8 byParam, u32 dwDevAlarmSN);
    /*========================================================================
    �� �� ����GetAlarmNmsId
    ��    �ܣ��õ��澯����ˮ��
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/18  3.9     ���ǿ  ����
    ========================================================================*/
    u32  GetAlarmNmsId(u32 dwAlarmCode, u8 byParam, u32 dwDevAlarmSN);
    /*========================================================================
    �� �� ����GetHighAlarmCode
    ��    �ܣ�����һ����߸澯����ĸ澯��
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/04/26  3.9     ���ǿ  ����
    ========================================================================*/
    u32  GetHighAlarmCode();
    /*========================================================================
    �� �� ����PrintBrief
    ��    �ܣ����Դ�ӡ�ӿ�, ��ӡ��Ҫ�Ĳ�����Ϣ
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/22  3.9     ���ǿ  ����
    ========================================================================*/
    void PrintBrief(BOOL32 bNetToHost = FALSE);
    /*========================================================================
    �� �� ����PrintAll
    ��    �ܣ���ӡ���еĲ�����Ϣ
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/22  3.9     ���ǿ  ����
    ========================================================================*/
    void PrintAll(BOOL32 bNetToHost = FALSE);
    /*========================================================================
    �� �� ����GetServType
    ��    �ܣ��õ��豸��ҵ����������
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/27  3.9     �κ�ƽ  ����
    2006/03/29  3.9     ���ǿ  ��Ϊ��̬��Ա
    ========================================================================*/
    static s8* GetServType(u8 byServType);

    //�õ��豸�����ִ�
    static s8* GetDevType(u8 byDevType);
    // �ж��Ƿ�Ϊǰ���豸
    static BOOL32 IsPuDevice(u8 byServiceType);

    //�����豸ҵ�����ʹ�ӡ�豸���ܲ���
    void PrintPfm(TTnmUnitType tUnitType, BOOL32 bNetToHost = FALSE);

    //��ӡ���ܲ���
    void PrintBasePfm(TTnmBasePfmInfo* ptPfm, BOOL32 bNetToHost = FALSE);
    void PrintBoardPfm(TTnmBoardPfmInfo *ptPfm, BOOL32 bNetToHost = FALSE);
    void PrintHwPfm(TTnmPfmHwInfo *ptPfm, BOOL32 bNetToHost = FALSE);
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}TDeviceInfo;

/*========================================================================
�� �� ����ResetAlarm
��    �ܣ������豸�澯��Ϣ
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/18  3.9     ���ǿ  ����
========================================================================*/
inline void TDeviceInfo::ResetAlarm()
{
    byHighestAlarmLevel = ALARM_NORMAL;
    bySelfAlarmLevel = ALARM_NORMAL;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        TDevAlarmState *ptState = atDevAlarm + nIndex;
        ptState->dwDevSerialNo = INVALID_SERIAL_NO;
        s32 nCount = 0;
        for (; nCount < MAX_ALARM_CHANEL_ONE_ALARMCODE; nCount++)
        {
            TAlarmBaseInfo *ptInfo = &ptState->tAlarmBase[nCount];
            ptInfo->byAlarmState = TNM_STATUS_NORMAL;
            ptInfo->dwNmsId = INVALID_SERIAL_NO;
            ptInfo->dwOccurTime = 0;
        }
    }

#ifdef BOGUS
    u8 byAlarmIdx;
    for(byAlarmIdx = 0; byAlarmIdx < CU_ALARMCODE_NUM; byAlarmIdx++)
    {
        for(s32 nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
        {
            TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
            if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
            {
                continue;
            }

            ptCuAlarmInfo->byAlarmState = TNM_STATUS_NORMAL;
            ptCuAlarmInfo->byHasAlarmInfo = TNM_DISABLE;
            ptCuAlarmInfo->dwAlarmSN = INVALID_SERIAL_NO;
            ptCuAlarmInfo->dwDevAlarmSN = 0;
        }
    }
#endif /* BOGUS */
}

/*========================================================================
�� �� ����SetAlarmNmsId
��    �ܣ����ø澯����ˮ��
��    ����u32 dwAlarmCode               [in]�澯��
          u32 dwNmsId                   [in]�澯����ˮ��
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/18  3.9     ���ǿ  ����
========================================================================*/
inline void TDeviceInfo::SetAlarmNmsId(u32 dwAlarmCode, u32 dwNmsId, u8 byParam, u32 dwDevAlarmSN)
{
    TDevAlarmState *ptState = atDevAlarm;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        if (ptState->dwAlarmCode == dwAlarmCode) 
        {
            TAlarmBaseInfo *ptInfo = &ptState->tAlarmBase[byParam];
            ptInfo->dwNmsId = dwNmsId;
            ptInfo->dwOccurTime = ::GetCurTime();
            return;
        }
        ptState++;
    }
    
#ifdef BOGUS
    u8 byAlarmIdx;
    if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)
    {
        byAlarmIdx = 0;
    }
    else if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)
    {
        byAlarmIdx = 1;
    }
    for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
    {
        TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
        if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
        {
            continue;
        }

        if (dwDevAlarmSN != ptCuAlarmInfo->dwDevAlarmSN)
        {
            continue;
        }

        ptCuAlarmInfo->dwAlarmSN = dwNmsId;
        ptCuAlarmInfo->dwOccurTime = ::GetCurTime();
        return;
    }
#endif /* BOGUS */
}

inline void TDeviceInfo::ResetAlarmState(u32 dwAlarmCode, u8 byParam, u32 dwDevAlarmSN)
{
    TDevAlarmState *ptState = atDevAlarm;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        if (ptState->dwAlarmCode == dwAlarmCode) 
        {
            TAlarmBaseInfo *ptInfo = &(ptState->tAlarmBase[byParam]);
            ptInfo->dwNmsId = INVALID_SERIAL_NO;
            ptInfo->dwOccurTime = 0;
            if (ptInfo->byAlarmState == TNM_STATUS_ALARM)
            {
                ptInfo->byAlarmState = TNM_STATUS_NORMAL;
                //���¼���澯����
                RecalcAlarmLevel();
            }
            return;
        }
        ptState++;
    }

#ifdef BOGUS
    u8 byAlarmIdx;
    if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)
    {
        byAlarmIdx = 0;
    }
    else if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)
    {
        byAlarmIdx = 1;
    }
    for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
    {
        TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
        if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
        {
            continue;
        }

        if (dwDevAlarmSN != ptCuAlarmInfo->dwDevAlarmSN)
        {
            continue;
        }

        ptCuAlarmInfo->byAlarmState = TNM_STATUS_NORMAL;
        ptCuAlarmInfo->byHasAlarmInfo = TNM_DISABLE;
        ptCuAlarmInfo->dwAlarmSN = INVALID_SERIAL_NO;
        ptCuAlarmInfo->dwDevAlarmSN = 0;
        RecalcAlarmLevel();
    }
#endif /* BOGUS */

    return;
}
/*========================================================================
�� �� ����GetAlarmNmsId
��    �ܣ��õ��澯����ˮ��
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/18  3.9     ���ǿ  ����
========================================================================*/
inline u32 TDeviceInfo::GetAlarmNmsId(u32 dwAlarmCode, u8 byParam, u32 dwDevAlarmSN)
{
    TDevAlarmState *ptState = atDevAlarm;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        if (ptState->dwAlarmCode == dwAlarmCode) 
        {
            TAlarmBaseInfo *ptInfo = &ptState->tAlarmBase[byParam];
            return ptInfo->dwNmsId;
        }
        ptState++;
    }

#ifdef BOGUS
    u8 byAlarmIdx;
    if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)    //cu alarm
    {
        byAlarmIdx = 0;
    }
    else if (TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION == dwAlarmCode)
    {
        byAlarmIdx = 1;
    }

    for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
    {
        TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
        if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
        {
            continue;
        }

        if (dwDevAlarmSN == ptCuAlarmInfo->dwDevAlarmSN)
        {
            return ptCuAlarmInfo->dwAlarmSN;
        }
    }
#endif /* BOGUS */

    return INVALID_SERIAL_NO;
}

/*========================================================================
�� �� ����GetHighAlarmCode
��    �ܣ�����һ����߸澯����ĸ澯��
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/26  3.9     ���ǿ  ����
========================================================================*/
inline u32 TDeviceInfo::GetHighAlarmCode()
{
    TDevAlarmState *ptState = atDevAlarm;

    u32 dwHighCode = TNM_ALARM_CODE_INVALID;
    
    u32 dwHighLevel = ALARM_NORMAL;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        u32 dwLevel = ::GetAlarmLevel(ptState->dwAlarmCode);
        if (dwLevel > dwHighLevel) 
        {
            dwHighCode = ptState->dwAlarmCode;
        }
        ptState++;
    }

    
#ifdef BOGUS
    u8 byAlarmIdx;
    u32 dwAlarmCode;
    for(byAlarmIdx = 0; byAlarmIdx < CU_ALARMCODE_NUM; byAlarmIdx++)
    {
        if (0 == byAlarmIdx)
        {
            dwAlarmCode = TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET;
        }
        else if (1 == byAlarmIdx)
        {
            dwAlarmCode = TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION;
        }
        for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
        {
            TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
            if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
            {
                continue;
            }

            u32 dwLevel = ::GetAlarmLevel(dwAlarmCode);
            if (dwLevel > dwHighLevel) 
            {
                dwHighCode = dwAlarmCode;
            }

            break;
        }
    }
#endif /* BOGUS */

    return dwHighCode;
}


/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TDeviceInfo::NetToHost()
{
    TTnmDevTopo::NetToHost();

    dwDevSerialNo = ntohl(dwDevSerialNo);
    dwMasterBoardSn = ntohl(dwMasterBoardSn);
    dwIpAddr = ntohl(dwIpAddr);
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        atDevAlarm[nIndex].NetToHost();
    }
    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        tPfmInfo.tPu.NetToHost();
    }
    else
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            tPfmInfo.tCmu.NetToHost();
            break;
        case TNM_PC_SERVICE_NRU:
            tPfmInfo.tNru.NetToHost();
            break;
        case TNM_PC_SERVICE_UAS:
            tPfmInfo.tUas.NetToHost();
            break;
        case TNM_CMU_SERVICE_VTDU:
            tPfmInfo.tVtdu.NetToHost();
            break;
        case TNM_PC_SERVICE_3AS:
            tPfmInfo.t3as.NetToHost();
            break;
        case TNM_CMU_SERVICE_PUI:
            tPfmInfo.tPui.NetToHost();
            break;
        case TNM_CMU_SERVICE_CUI:
            tPfmInfo.tCui.NetToHost();
        case TNM_CMU_SERVICE_VPU:
            tPfmInfo.tVpu.NetToHost();
            break;
        case TNM_PC_SERVICE_NMS:
            tPfmInfo.tNms.NetToHost();
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            tPfmInfo.tDec5.NetToHost();
            break;
        default:
            break;
        }
    }
}
inline void TDeviceInfo::HostToNet()
{
    TTnmDevTopo::HostToNet();

    dwDevSerialNo = htonl(dwDevSerialNo);
    dwMasterBoardSn = htonl(dwMasterBoardSn);
    dwIpAddr = htonl(dwIpAddr);
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        atDevAlarm[nIndex].HostToNet();
    }

    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        tPfmInfo.tPu.HostToNet();
    }
    else
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            tPfmInfo.tCmu.HostToNet();
            break;
        case TNM_PC_SERVICE_NRU:
            tPfmInfo.tNru.HostToNet();
            break;
        case TNM_PC_SERVICE_UAS:
            tPfmInfo.tUas.HostToNet();
            break;
        case TNM_CMU_SERVICE_VTDU:
            tPfmInfo.tVtdu.HostToNet();
            break;
        case TNM_PC_SERVICE_3AS:
            tPfmInfo.t3as.HostToNet();
            break;
        case TNM_CMU_SERVICE_PUI:
            tPfmInfo.tPui.HostToNet();
            break;
        case TNM_CMU_SERVICE_CUI:
            tPfmInfo.tCui.HostToNet();
            break;
        case TNM_CMU_SERVICE_VPU:
            tPfmInfo.tVpu.HostToNet();
            break;
        case TNM_PC_SERVICE_NMS:
            tPfmInfo.tNms.HostToNet();
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            tPfmInfo.tDec5.HostToNet();
            break;
        default:
            break;
        }
    }
}

/*========================================================================
�� �� ����IsField
��    �ܣ��ж��豸�Ƿ�Ϊ���ƽ̨�豸
��    ����BOOL32 bCheckSubBoard = FALSE     [in]�ж�ʱ�Ƿ�������Ӱ�,���Ϊ
                            TRUE����ֻ���¹ҵ���Ĳŷ���TRUE�����ΪFALSE��
                            ֻҪ��ƽ̨ҵ�������TRUE
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/07  3.9     ���ǿ  ʵ��
========================================================================*/
inline BOOL32 TDeviceInfo::IsField(BOOL32 bCheckSubBoard, BOOL32 bCheckById) const
{
    if (bCheckById) 
    {
        if (tDevId.achID[18] == '5') 
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    u8 bySericeType = tUnitType.byServiceType;
    if (bySericeType == TNM_CMU_SERVICE_KDM2000) 
    {
        return TRUE;
    }
    else if (bySericeType == TNM_CMU_SERVICE_KDM2100A) 
    {
        if (bCheckSubBoard == TRUE) 
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}


/*========================================================================
�� �� ����UpdateHighestAlarmLevel
��    �ܣ�u8 byLevel                    [in]�µĸ澯����
          BOOL32 bSet                   [in]TRUE: ���µ�ֵȡ��ԭ��ֵ
                                        FALSE�����µ�ֵ�����澯����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/23  3.9     ���ǿ  ����
========================================================================*/
inline void TDeviceInfo::UpdateHighestAlarmLevel(u8 byLevel, BOOL32 bSet)
{
    if (bSet) 
    {
        byHighestAlarmLevel = byLevel;
    }
    else
    {
        byHighestAlarmLevel |= byLevel;
    }
}

/*========================================================================
�� �� ����GetBoardPfmInfo
��    �ܣ��õ�����Ļ������ܲ���
��    ����
�� �� ֵ������Ļ������ܲ���ָ��:   �ɹ�        NULL��ʧ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/21  3.9     ���ǿ  ����
========================================================================*/
inline const TTnmBoardPfmInfo * TDeviceInfo::GetBoardPfmInfo() const
{
    if (IsBoard()) 
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
            {
                return tPfmInfo.tCmu.GetBoardPfmInfo();
            }
            break;
        case TNM_CMU_SERVICE_VTDU:
            {
                return tPfmInfo.tVtdu.GetBoardPfmInfo();
            }
            break;
        case TNM_CMU_SERVICE_PUI:
            {
                return tPfmInfo.tPui.GetBoardPfmInfo();
            }
            break;
        case TNM_CMU_SERVICE_CUI:
            {
                return tPfmInfo.tCui.GetBoardPfmInfo();
            }
            break;
        case TNM_CMU_SERVICE_VPU:
            {
                return tPfmInfo.tVpu.GetBoardPfmInfo();
            }
            break;
//        case TNM_PU_SERVICE_KDMDEC5:
//            {
//                return tPfmInfo.tDec5.GetBoardPfmInfo();
//            }
//            break;
        default:
            {
                return NULL;
            }
            break;
        }
    }
    return NULL;
}

/*========================================================================
�� �� ����IsBoard
��    �ܣ��жϸ��豸�Ƿ�Ϊ�����еĵ���
��    ����
�� �� ֵ��TRUE: �ǵ���          FALSE:  ���������豸
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/22  3.9     ���ǿ  ����
========================================================================*/
inline BOOL32 TDeviceInfo::IsBoard() const
{
    u8 byDeviceType = tUnitType.byDeviceType;
    if (byDeviceType == TNM_BOARD_MPC
        || byDeviceType == TNM_BOARD_CRI
        || byDeviceType == TNM_BOARD_DRI
        || byDeviceType == TNM_BOARD_VPU
        || byDeviceType == TNM_BOARD_VAS)
//      || byDeviceType == TNM_BOARD_DEC5  
    {
        return TRUE;
    }

    return FALSE;
}

/*========================================================================
�� �� ����GetPfmInfo
��    �ܣ��õ����ܲ�����ָ�뼰���ݴ�С
��    ����s32 &nPfmSize                     [in]���ܲ������ݴ�С
�� �� ֵ�����ܲ�����ָ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/20  3.9     ���ǿ  ����
========================================================================*/
inline void*  TDeviceInfo::GetPfmInfo(s32 &nPfmSize) const
{
    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        nPfmSize = sizeof(TTnmPuPfmInfo);
    }
    else
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            nPfmSize = sizeof(TTnmCmuPfmInfo);
            break;
        case TNM_PC_SERVICE_NRU:
            nPfmSize = sizeof(TTnmNruPfmInfo);
            break;
        case TNM_PC_SERVICE_UAS:
            nPfmSize = sizeof(TTnmUasPfmInfo);
            break;
        case TNM_CMU_SERVICE_VTDU:
            nPfmSize = sizeof(TTnmVtduPfmInfo);
            break;
        case TNM_PC_SERVICE_3AS:
            nPfmSize = sizeof(TTnm3asPfmInfo);
            break;
        case TNM_CMU_SERVICE_PUI:
            nPfmSize = sizeof(TTnmPuiPfmInfo);
            break;
        case TNM_CMU_SERVICE_CUI:
            nPfmSize = sizeof(TTnmCuiPfmInfo);
            break;
        case TNM_CMU_SERVICE_VPU:
            nPfmSize = sizeof(TTnmVpuPfmInfo);
            break;
        case TNM_PC_SERVICE_NMS:
            nPfmSize = sizeof(TTnmNmsPfmInfo);
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            nPfmSize = sizeof(TTnmDec5PfmInfo);
            break;
        default:
            nPfmSize = 0;
            break;
        }
    }
    return (void*)&tPfmInfo;
}

/*========================================================================
�� �� ����UpdateAlarmState
��    �ܣ����±��ر���ĸ澯״̬
��    ����TTnmDevAlarm * const ptDevAlarm       [in]�澯������
          u8 byState                            [in]�澯�������ǻָ�
                                                TNM_STATUS_ALARM/TNM_STATUS_NORMAL
          u8 byAlarmLevel                       [in]Ҫ���µĸ澯����
          u32 dwNmsId = 0                       [in]NMS����ĸ澯ID��
          u32 dwOccurTime = 0                   [in]�澯������ʱ��
�� �� ֵ���澯״̬�Ƿ����仯����Ҫ��¼��֪ͨ�ͻ���
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/22  3.9     ���ǿ  ����
2006/08/07  3.9     �κ�ƽ  �޸�Ϊÿ���澯��֧��8·�澯
========================================================================*/
inline BOOL32 TDeviceInfo::UpdateAlarmState(TTnmDevAlarm * const ptDevAlarm,
                            u8 byState, u8 byAlarmLevel, u32 dwNmsId, u32 dwOccurTime, u8 byParam)
{
    NMS_ASSERT(ptDevAlarm);
    if (ptDevAlarm == NULL) 
    {
        return FALSE;
    }
    if (ptDevAlarm->dwAlarmCode == TNM_ALARM_CODE_DEV_OFFLINE) 
    {
        // �����߲�����
        return TRUE;
    }

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        TDevAlarmState *ptState = atDevAlarm + nIndex;
        if (ptState->dwAlarmCode == ptDevAlarm->dwAlarmCode) 
        {
            //renhouping 2006.08.07
            TAlarmBaseInfo *ptInfo = &ptState->tAlarmBase[byParam];

            if (byState == TNM_STATUS_NORMAL) //����Ǹ澯�ָ�
            {                
                //�ж��Ƿ�ı�״̬
                BOOL32 bChange = (ptInfo->byAlarmState != TNM_STATUS_NORMAL);
                
                // �澯�ָ�
                //BOOL32 bChange = (ptState->byAlarmState != TNM_STATUS_NORMAL);

                ptInfo->byAlarmState = TNM_STATUS_NORMAL;
                ptInfo->byConfirmState = TNM_STATUS_NOCONFIRM;
               
                //??�Ƿ�Ҫ�趨�豸��ˮ��
                //ptState->dwDevSerialNo = INVALID_SERIAL_NO;
                ptInfo->dwOccurTime = 0;
                
                //���¼���澯����
                RecalcAlarmLevel();
                return bChange;
            }
            else//����Ǹ澯
            {
                if (dwNmsId != 0)
                {
                    ptInfo->dwNmsId = dwNmsId;
                }
                if (dwOccurTime != 0)//�澯ʱ�� ??�Ƿ���Ҫ
                {
                    ptInfo->dwOccurTime = dwOccurTime;
                }
                // �澯���� ��ptInfo->byAlarmStateΪ
                // TNM_STATUS_CLEAR��TNM_STATUS_ALARMʱ����ԭ��״̬
                if (ptInfo->byAlarmState == TNM_STATUS_NORMAL) 
                {
                    ptInfo->byAlarmState = TNM_STATUS_ALARM;
                    //����ǵ�ǰ�豸����澯��ĵ�һ���澯���趨�豸��ˮ��
                    if (ptState->dwDevSerialNo == INVALID_SERIAL_NO)
                    {
                        ptState->dwDevSerialNo = ptDevAlarm->dwDevSerialNo;
                    }
                    
                    bySelfAlarmLevel |= byAlarmLevel;
                    byHighestAlarmLevel |= bySelfAlarmLevel;
                    return TRUE;
                }
                else if (ptInfo->byAlarmState == TNM_STATUS_CLEAR) 
                {
                    if (ptDevAlarm->dwAlarmCode == TNM_ALARM_CODE_SAME_REG_ID) 
                    {
                        // �ظ�ע���û�лָ�����Ϣ��ͬʱֻ�ϱ�һ�Σ�����CLEAR״̬��ȻҪ�ı�
                        ptInfo->byAlarmState = TNM_STATUS_ALARM;
                        ptState->dwDevSerialNo = ptDevAlarm->dwDevSerialNo;

                        bySelfAlarmLevel |= byAlarmLevel;
                        byHighestAlarmLevel |= bySelfAlarmLevel;
                        return TRUE;
                    }
                }
            }
            return FALSE;
        }
    }

//    if ((TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == ptDevAlarm->dwAlarmCode)
//        ||(TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION == ptDevAlarm->dwAlarmCode))
//    {
//        return TRUE ;
//    }

#ifdef BOGUS
    u8 byAlarmIdx;
    if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == ptDevAlarm->dwAlarmCode)    //cu alarm
    {
        byAlarmIdx = 0;
    }
    else if (TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION == ptDevAlarm->dwAlarmCode)
    {
        byAlarmIdx = 1;
    }

    for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
    {
        //atCuAlarm;
        TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
//        if (0 == ptCuAlarmInfo->dwAlarmSN)
//        {
//            continue;
//        }

        if (byState == TNM_STATUS_NORMAL) //����Ǹ澯�ָ�
        {
            if (ptCuAlarmInfo->dwDevAlarmSN != ptDevAlarm->dwDevSerialNo)
            {
                continue;
            }
            //�ж��Ƿ�ı�״̬
            BOOL32 bChange = (ptCuAlarmInfo->byAlarmState != TNM_STATUS_NORMAL);

            ptCuAlarmInfo->byAlarmState = TNM_STATUS_NORMAL;

            ptCuAlarmInfo->dwOccurTime = 0;

            ptCuAlarmInfo->dwAlarmSN = 0;
            
            //���¼���澯����
            RecalcAlarmLevel();
            return bChange;
        }
        else//����Ǹ澯
        {
            if (TNM_ENABLE == ptCuAlarmInfo->byHasAlarmInfo)
            {
                if (ptCuAlarmInfo->dwDevAlarmSN == ptDevAlarm->dwDevSerialNo)
                {
                    return FALSE;
                }
                continue;
            }

            ptCuAlarmInfo->byHasAlarmInfo = TNM_ENABLE;

            if (dwNmsId != 0)
            {
                ptCuAlarmInfo->dwAlarmSN = dwNmsId;
            }
            if (dwOccurTime != 0)//�澯ʱ�� ??�Ƿ���Ҫ
            {
                ptCuAlarmInfo->dwOccurTime = dwOccurTime;
            }
            // �澯���� ��ptInfo->byAlarmStateΪ
            // TNM_STATUS_CLEAR��TNM_STATUS_ALARMʱ����ԭ��״̬
            //if (ptCuAlarmInfo->byAlarmState == TNM_STATUS_NORMAL) 
            {
                ptCuAlarmInfo->byAlarmState = TNM_STATUS_ALARM;
                ptCuAlarmInfo->dwDevAlarmSN = ptDevAlarm->dwDevSerialNo;
                
                bySelfAlarmLevel |= byAlarmLevel;
                byHighestAlarmLevel |= bySelfAlarmLevel;
                return TRUE;
            }
        }
        return FALSE;
    }
#endif /* BOGUS */

    return FALSE;
}


/*========================================================================
�� �� ����RecalcAlarmLevel
��    �ܣ����¼��㱾�豸�ĸ澯����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/26  3.9     ���ǿ  ����
========================================================================*/
inline void TDeviceInfo::RecalcAlarmLevel()
{
    //���¼��㱾�豸����߸澯����
    bySelfAlarmLevel = ALARM_NORMAL;
    byHighestAlarmLevel = ALARM_NORMAL;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        TDevAlarmState *ptState = atDevAlarm + nIndex;
        if (ptState->IsAlarmState()) 
        {
            bySelfAlarmLevel |= ::GetAlarmLevel(ptState->dwAlarmCode);
        }
    }
    byHighestAlarmLevel |= bySelfAlarmLevel;
}
/*========================================================================
�� �� ����ClearAlarm
��    �ܣ��ֶ�����澯״̬
��    ����u32 dwAlarmSn                         [in]Ҫ����ĸ澯��ˮ��
�� �� ֵ��TRUE: �ɹ�        FALSE��ʧ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/22  3.9     ���ǿ  ����
========================================================================*/
inline BOOL32 TDeviceInfo::ClearAlarm(u32 dwAlarmSn, u8 byState)
{
    s32 nIndex = 0;
    TDevAlarmState *ptState = NULL;
    TAlarmBaseInfo *ptBaseInfo = NULL;
    
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        ptState = atDevAlarm + nIndex;
        s32 nSubIndex = 0;
        for (; nSubIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nSubIndex++)
        {
            ptBaseInfo = &ptState->tAlarmBase[nSubIndex];
            if (ptBaseInfo->dwNmsId != dwAlarmSn) 
            {
                continue;
            }
            if (TNM_STATUS_CONFIRM == byState)
            {
                ptBaseInfo->byConfirmState = byState;
            }
            else
            {
                ptBaseInfo->byAlarmState = byState;
                ptBaseInfo->byConfirmState = TNM_STATUS_NOCONFIRM;
            }
            RecalcAlarmLevel();
            return TRUE;
        }
    }

#ifdef BOGUS
    TCuAlarmInfo *ptCuAlarmInfo = NULL;
    for(nIndex = 0; nIndex < CU_ALARMCODE_NUM; nIndex++)
    {
        s32 nSubIndex = 0;
        for (; nSubIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nSubIndex++)
        {
            ptCuAlarmInfo = &atCuAlarm[nIndex][nSubIndex];
            if (ptCuAlarmInfo->dwAlarmSN != dwAlarmSn)
            {
                continue;
            }
            if (TNM_STATUS_CONFIRM == byState)
            {
                ptCuAlarmInfo->byConfirmState = byState;
            }
            else
            {
                ptCuAlarmInfo->byAlarmState = byState; //TNM_STATUS_CLEAR;
                ptCuAlarmInfo->byConfirmState = TNM_STATUS_NOCONFIRM;
                ptCuAlarmInfo->byHasAlarmInfo = TNM_DISABLE;
            }
            RecalcAlarmLevel();
            return TRUE;
        }
    }
#endif /* BOGUS */
    return FALSE;
}

/*========================================================================
�� �� ����GetDevDesc
��    �ܣ��õ��豸������(�豸����+�豸����)
��    ����s8 *pbyDescBuf            [in]�豸�����ĳ���
          s32 nLen                  [in]����BUF�Ĵ�С
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/15  3.9     ���ǿ  ����
========================================================================*/
inline BOOL32 TDeviceInfo::GetDevDesc(s8 *pbyDescBuf, s32 nLen) const
{
    NMS_ASSERT(pbyDescBuf);
    if (pbyDescBuf == NULL) 
    {
        return FALSE;
    }

    s32 nNameLen = strlen(achDevName);
    s32 nCopyLen = 0;
    if (nNameLen > 0 && nNameLen < nLen) 
    {
        strcpy(pbyDescBuf, achDevName);
        nCopyLen = nNameLen;
    }

    s32 nAliasLen = strlen(achAlias);

    if (nCopyLen + nAliasLen < nLen && nAliasLen > 0) 
    {
        if (nCopyLen > 0) 
        {
            strcat(pbyDescBuf, "(");
            strcat(pbyDescBuf, achAlias);
            strcat(pbyDescBuf, ")");
        }
        else
        {
            strcpy(pbyDescBuf, achAlias);
        }
        return TRUE;
    }
    return FALSE;
}

/*========================================================================
�� �� ����InitPfm
��    �ܣ���ʼ�����ܲ���
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/03  3.9     ���ǿ  ����
========================================================================*/
inline void TDeviceInfo::InitPfm()
{
    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        tPfmInfo.tPu.Init();
    }
    else
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            tPfmInfo.tCmu.Init();
            break;
        case TNM_PC_SERVICE_NRU:
            tPfmInfo.tNru.Init();
            break;
        case TNM_PC_SERVICE_UAS:
            tPfmInfo.tUas.Init();
            break;
        case TNM_CMU_SERVICE_VTDU:
            tPfmInfo.tVtdu.Init();
            break;
        case TNM_PC_SERVICE_3AS:
            tPfmInfo.t3as.Init();
            break;
        case TNM_CMU_SERVICE_PUI:
            tPfmInfo.tPui.Init();
            break;
        case TNM_CMU_SERVICE_CUI:
            tPfmInfo.tCui.Init();
            break;
        case TNM_CMU_SERVICE_VPU:
            tPfmInfo.tVpu.Init();
            break;
        case TNM_PC_SERVICE_NMS:
            tPfmInfo.tNms.Init();
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            tPfmInfo.tDec5.Init();
            break;
        default:
            tPfmInfo.Init();
            break;
        }
    }
    return;
}

/*========================================================================
�� �� ����PrintBrief
��    �ܣ����Դ�ӡ�ӿ�, ��ӡ��Ҫ�Ĳ�����Ϣ
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/22  3.9     ���ǿ  ����
2006/03/29  3.9     �κ�ƽ  ʵ��
========================================================================*/
inline void TDeviceInfo::PrintBrief(BOOL32 bNetToHost)
{
    //�豸��Ҫ��Ϣ
    if (bNetToHost) 
    {
        OspPrintf(TRUE, FALSE, "\tSn=%d, ", ntohl(dwDevSerialNo));
    }
    else
    {
        OspPrintf(TRUE, FALSE, "\tSn=%d, ", dwDevSerialNo);
    }
    
    OspPrintf(TRUE, FALSE, "Alias=%s, ", achAlias);

    //�豸Ӳ�����ͺ�ҵ������
    s8 *pchTemp = GetDevType(tUnitType.byDeviceType);
    OspPrintf(TRUE, FALSE, "Dev=%s", pchTemp);
    pchTemp = GetServType(tUnitType.byServiceType);
    OspPrintf(TRUE, FALSE, ", Srv=%s", pchTemp);
    OspPrintf(TRUE, FALSE, ", DevTypeName=%s\n", achDevTypeName);
    
    if (bNetToHost) 
    {
        //�����豸��������ˮ��
        OspPrintf(TRUE, FALSE, "\tMBSn=%d, ", ntohl(dwMasterBoardSn));
        
        //�豸��Ip��ַ
        CString csIp = CCallAddr::GetIpStr(ntohl(dwIpAddr));
        OspPrintf(TRUE, FALSE, "Ip=%s, ", ((LPCTSTR)csIp));
    }
    else
    {
        //�����豸��������ˮ��
        OspPrintf(TRUE, FALSE, "\tMBSn=%d, ", dwMasterBoardSn);
        
        //�豸��Ip��ַ
        CString csIp = CCallAddr::GetIpStr(dwIpAddr);
        OspPrintf(TRUE, FALSE, "Ip=%s, ", ((LPCTSTR)csIp));
    }

    //�Ƿ����� 
    OspPrintf(TRUE, FALSE, "IsField=%d, ", byField);

    //�豸�Ƿ�����
    OspPrintf(TRUE, FALSE, "Online=%d\n", byOnline);
}
/*========================================================================
�� �� ����PrintAll
��    �ܣ���ӡ���еĲ�����Ϣ
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/22  3.9     ���ǿ  ����
2006/03/29  3.9     �κ�ƽ  ʵ��
========================================================================*/
inline void TDeviceInfo::PrintAll(BOOL32 bNetToHost)
{
    //���ȴ�ӡ������Ϣ
    PrintBrief(bNetToHost);
    //��
    OspPrintf(TRUE, FALSE, "\tbyLayer=%d, ", byLayer);
    //��
    OspPrintf(TRUE, FALSE, "bySlot=%d\n", bySlot);

    //��ӡȨ������
    OspPrintf(TRUE, FALSE, "\tRigntRange=(%d)", byRightRange);
    switch(byRightRange)
    {
    case RIGHT_RANGE_ALL_CHILD:
        {
            OspPrintf(TRUE, FALSE, "%s\n", "RIGHT_RANGE_ALL_CHILD");
        }
        break;
    case RIGHT_RANGE_NO_CHILD:
        {
            OspPrintf(TRUE, FALSE, "%s\n", "RIGHT_RANGE_NO_CHILD");
        }
        break;
    case RIGHT_RANGE_PART_CHILD:
        {
            OspPrintf(TRUE, FALSE, "%s\n", "RIGHT_RANGE_PART_CHILD");
        }
        break;  
    default:break;
    }
    
    //��߸澯���� ���豸����߸澯����
    OspPrintf(TRUE, FALSE, "\tHighAlarmLevel= %d, SelfAlarmLevel= %d\n", byHighestAlarmLevel,
                    bySelfAlarmLevel);
    //  �豸��ǰ�ĸ澯״̬
    OspPrintf(TRUE, FALSE, "\tCurrent Alarm:");
    s32 nIndex = 0;
    for (nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "\n\t%(%d.) ", nIndex);
        TDevAlarmState *ptState = atDevAlarm + nIndex;
        ptState->Print(bNetToHost);
    
    }
    OspPrintf(TRUE, FALSE, "\n");
    //��ӡ�豸���ܲ���
    OspPrintf(TRUE, FALSE, "\tDevicePfmInfo: \n");
    PrintPfm(tUnitType, bNetToHost);    
}

/*========================================================================
�� �� ����PrintPfm
��    �ܣ������豸ҵ�����ʹ�ӡ�豸���ܲ���
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/29  3.9     ���ǿ  ����
2006/03/29  3.9     �κ�ƽ  ʵ��
========================================================================*/
inline void TDeviceInfo::PrintPfm(TTnmUnitType tUnitType, BOOL32 bNetToHost)
{
    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        //��ӡPU���ܲ���
        TTnmPuPfmInfo *pt = (TTnmPuPfmInfo*)&tPfmInfo.tPu;
        if (pt) 
        {
            pt->Print(bNetToHost);
        }
    }
    else
    {
        switch(tUnitType.byServiceType)
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            {//��ӡƽ̨�豸���ܲ���   
                TTnmCmuPfmInfo *pt = (TTnmCmuPfmInfo*)&tPfmInfo.tCmu;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_CMU_SERVICE_PUI:
            {//��ӡPUI���ܲ���
                TTnmPuiPfmInfo *pt = (TTnmPuiPfmInfo *)&tPfmInfo.tPui;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_CMU_SERVICE_CUI:
            {//��ӡCUI���ܲ���
                TTnmCuiPfmInfo *pt = (TTnmCuiPfmInfo*)&tPfmInfo.tCui;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_CMU_SERVICE_VPU:
            {//��ӡCUI���ܲ���
                TTnmVpuPfmInfo *pt = (TTnmVpuPfmInfo*)&tPfmInfo.tVpu;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_CMU_SERVICE_VTDU:
            {//��ӡVTDU���ܲ���
                TTnmVtduPfmInfo *pt = (TTnmVtduPfmInfo*)&tPfmInfo.tVtdu;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_PC_SERVICE_NRU:
            {//��ӡNRU���ܲ���
                TTnmNruPfmInfo *pt = (TTnmNruPfmInfo*)&tPfmInfo.tNru;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_PC_SERVICE_UAS:
            {//��ӡUAS���ܲ���
                TTnmUasPfmInfo *pt = (TTnmUasPfmInfo*)&tPfmInfo.tUas;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_PC_SERVICE_3AS:
            {//��ӡ3AS���ܲ���
                TTnm3asPfmInfo *pt3as = (TTnm3asPfmInfo*)&tPfmInfo.t3as;
                if (pt3as) 
                {
                    pt3as->Print(bNetToHost);
                }
            }
            break;
        case TNM_PC_SERVICE_NMS:
            {
                // ��ӡNMS�����ܲ���
                TTnmNmsPfmInfo *ptNms = (TTnmNmsPfmInfo*)&tPfmInfo.tNms;
                if (ptNms) 
                {
                    ptNms->Print(bNetToHost);
                }
            }
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            {
                TTnmDec5PfmInfo *ptDec5 = (TTnmDec5PfmInfo *)&tPfmInfo.tDec5;
                if(ptDec5)
                {
                    ptDec5->Print(bNetToHost);
                }
            }
            break;
        default:
            break;
        }
    }
}

/*========================================================================
�� �� ����PrintBasePfm
��    �ܣ���ӡ�������ܲ���
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/29  3.9     ���ǿ  ����
2006/03/29  3.9     �κ�ƽ  ʵ��
========================================================================*/
inline void TDeviceInfo::PrintBasePfm(TTnmBasePfmInfo* ptPfm, BOOL32 bNetToHost)
{
    OspPrintf(TRUE, FALSE, "\tCpu=%d, Mem=%d, MemStatus=%d, SysLoad:%d\n", 
                ptPfm->byCpuUseRate, ptPfm->byMemUseRate, 
                ptPfm->byMemStatus, ptPfm->bySysLoad);
    OspPrintf(TRUE, FALSE, "\tSysTime=%s, Compile=%s\n",
                GetTimeStr(ptPfm->dwSysTime), ptPfm->achCompileTime);
    OspPrintf(TRUE, FALSE, "\thwver=%s, swver=%s\n",ptPfm->achHardwareVer, ptPfm->achSoftwareVer);
}
/*========================================================================
�� �� ����PrintBoardPfm
��    �ܣ���ӡ�������ܲ���
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/29  3.9     ���ǿ  ����
2006/03/29  3.9     �κ�ƽ  ʵ��
========================================================================*/
inline void TDeviceInfo::PrintBoardPfm(TTnmBoardPfmInfo *ptPfm, BOOL32 bNetToHost)
{
    TTnmPfmHwInfo *pt = (TTnmPfmHwInfo*)ptPfm;
    PrintHwPfm(pt, bNetToHost);
    OspPrintf(TRUE, FALSE, "\tLayer : %d      Slot : %d\n", ptPfm->byLayer, ptPfm->bySlot);
}
/*========================================================================
�� �� ����PrintHwPfm
��    �ܣ���ӡ���ܲ���
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/29  3.9     ���ǿ  ����
2006/03/29  3.9     �κ�ƽ  ʵ��
========================================================================*/
inline void TDeviceInfo::PrintHwPfm(TTnmPfmHwInfo *ptPfm, BOOL32 bNetToHost)
{
    TTnmBasePfmInfo *pt = (TTnmBasePfmInfo*)ptPfm;
    PrintBasePfm(pt, bNetToHost);

    OspPrintf(TRUE, FALSE, "\tWatchDogEnalbe : %d\n", ptPfm->byWatchDogEnalbe);
    OspPrintf(TRUE, FALSE, "\tPannelLed:");
    s32 nIndex = 0;
    for (nIndex = 0; nIndex < TNM_MAX_PANELLED_LEN; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "-%d", ptPfm->abyPanelLed[nIndex]);
    }
    OspPrintf(TRUE, FALSE, "\n");

    OspPrintf(TRUE, FALSE, "\tEnternetStatus: ");
    for (nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "-%d", ptPfm->abyPanelLed[nIndex]);
    }
    OspPrintf(TRUE, FALSE, "\n");

    OspPrintf(TRUE, FALSE, "\tMapStatus  :");
    for (nIndex = 0; nIndex < TNM_MAX_MAP_NUM; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "-%d", ptPfm->abyMapStatus[nIndex]);
    }
    OspPrintf(TRUE, FALSE, "\n");
}

/*========================================================================
�� �� ����GetServType
��    �ܣ��õ��豸ҵ�����͵�����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/29  3.9     ���ǿ  ����
2006/03/29  3.9     �κ�ƽ  ʵ��
2006/03/29  3.9     ���ǿ  ��Ϊ��̬��Ա
========================================================================*/
inline s8* TDeviceInfo::GetServType(u8 byServType)
{
    switch(byServType)
    {
    case TNM_PU_SERVICE_KDM2400:    return "PU_SERV_KDM2400";
    case TNM_PU_SERVICE_KDM2400_PLUS:   return "PU_SERV_KDM2400P";
    case TNM_PU_SERVICE_KDM2500:    return "PU_SERV_KDM2500";
    case TNM_PU_SERVICE_KDM2500_PLUS:   return "PU_SERV_KDM2500P";
    case TNM_PU_SERVICE_KDM2417:    return "PU_SERV_KDM2417";
    case TNM_PU_SERVICE_KDM2418:    return "PU_SERV_KDM2418";
    case TNM_PU_SERVICE_KDM2428:    return "PU_SERV_KDM2428";
    case TNM_PU_SERVICE_KDM2518:    return "PU_SERV_KDM2518";
    case TNM_PU_SERVICE_KDM2401:    return "PU_SERV_KDM2401";
    case TNM_PU_SERVICE_KDM2401L:   return "PU_SERV_KDM2401L";
    case TNM_PU_SERVICE_KDM2401LS:  return "PU_SERV_KDM2401LS";
    case TNM_PU_SERVICE_KDM2402LS:  return "PU_SERV_KDM2402LS";
    case TNM_PU_SERVICE_KDM2402L:   return "PU_SERV_KDM2402L";
    case TNM_PU_SERVICE_KDM2401S:   return "PU_SERV_KDM2401S";
    case TNM_PU_SERVICE_KDM2461:    return "PU_SERV_KDM2461";
    case TNM_PU_SERVICE_KDM2461L:   return "PU_SERV_KDM2461L";
    case TNM_PU_SERVICE_KDM2560:    return "PU_SERV_KDM2560";
    case TNM_PU_SERVICE_KDM2402S:   return "PU_SERV_KDM2402S";
    case TNM_PU_SERVICE_KDM2440:    return "PU_SERV_KDM2440";
    case TNM_PU_SERVICE_KDM2440P:    return "PU_SERV_KDM2440P";
    case TNM_PU_SERVICE_KDM2402:    return "PU_SERV_KDM2402";
    case TNM_PU_SERVICE_KDV8010A:   return "PU_SERV_KDM8010A";
    case TNM_PU_SERVICE_KDV8010C:   return "PU_SERV_KDM8010C";
    case TNM_PU_SERVICE_PU:         return "PU_SERV_PU";

    case TNM_CMU_SERVICE_KDM2000:   return "CMU_SERV_KDM2000";
    case TNM_CMU_SERVICE_KDM2100A: return "CMU_SERV_KDM2100A";

    case TNM_CMU_SERVICE_PUI:   return "CMU_SERV_PUI";
    case TNM_CMU_SERVICE_CUI:   return "CMU_SERV_CUI";
    case TNM_CMU_SERVICE_VPU:   return "CMU_SERV_VPU";
    case TNM_CMU_SERVICE_VTDU:  return "CMU_SERV_VTDU";
    case TNM_PU_SERVICE_KDMDEC5: return "PU_SERV_DEC5";

    case TNM_PC_SERVICE_NRU:    return "PC_SERV_NRU";
    case TNM_PC_SERVICE_UAS:    return "PC_SERV_UAS";
    case TNM_PC_SERVICE_3AS:    return "PC_SERV_3AS";
    case TNM_PC_SERVICE_ALL:    return "PC_SERV_ALL";
    case TNM_PC_SERVICE_NMS:    return "PC_SERV_NMS";
    default:
        return "SERV_UNKNOW";
    }
}

/*========================================================================
�� �� ����GetDevType
��    �ܣ��õ��豸Ӳ����������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/29  3.9     ���ǿ  ����
2006/03/29  3.9     �κ�ƽ  ʵ��
========================================================================*/
inline s8* TDeviceInfo::GetDevType(u8 byDevType)
{
    switch(byDevType)
    {

    case TNM_PU_KDM2400:return "PU_KDM2400";
    case TNM_PU_KDM2400_PLUS:return "PU_KDM2400P";
    case TNM_PU_KDM2500:return "PU_KDM2500";
    case TNM_PU_KDM2500_PLUS:return "PU_KDM2500P";        
    case TNM_PU_KDM2417:return "PU_KDM2417";
    case TNM_PU_KDM2418:return "PU_KDM2418";
    case TNM_PU_KDM2428:return "PU_KDM2428";
    case TNM_PU_KDM2518:return "PU_KDM2518";
    case TNM_PU_KDM2401L:return "PU_KDM2401L";
    case TNM_PU_KDM2401LS:return "PU_KDM2401LS";
    case TNM_PU_KDM2402LS:return "PU_KDM2402LS";
    case TNM_PU_KDM2401S:return "PU_KDM2401S";
    case TNM_PU_KDM2402S:return "PU_KDM2402S";
    case TNM_PU_KDM2402:return "PU_KDM2402";
    case TNM_PU_KDM2401:return "PU_KDM2401";
    case TNM_PU_KDM2461:return "PU_KDM2461";
    case TNM_PU_KDM2461L:return "PU_KDM2461L";
    case TNM_PU_KDM2560:return "PU_KDM2560";
    case TNM_PU_KDV8010A:return "PU_KDV8010A";
    case TNM_PU_KDV8010C:return "PU_KDV8010C";
    case TNM_PU_KDM2440:return "PU_KDM2440";
    case TNM_PU_KDM2440P:return "PU_KDM2440P";
    case TNM_CMU_KDM2100A:return "CMU_KDM2100A";
    case TNM_BOARD_MPC:return "BOARD_MPC";
    case TNM_BOARD_CRI:return "BOARD_CRI";
    case TNM_BOARD_DRI:return "BOARD_DRI";
    case TNM_BOARD_VPU:return "BOARD_VPU";
    case TNM_BOARD_VAS:return "BOARD_VAS";
    case TNM_BOARD_DEC5:return "BOARD_DEC5";
    case TNM_PC_UNIT:return "PC_UNIT";
    default:
        return "UNIT_UNKNOW";
    }
}

/*========================================================================
�� �� ����IsPuDevice
��    �ܣ��ж��Ƿ�Ϊǰ���豸
��    ����u8 
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/11  3.9     ���ǿ  ����
========================================================================*/
inline BOOL32 TDeviceInfo::IsPuDevice(u8 byServiceType)
{
    if (byServiceType == TNM_PU_SERVICE_KDM2400
        || byServiceType == TNM_PU_SERVICE_KDM2400_PLUS
        || byServiceType == TNM_PU_SERVICE_KDM2500
        || byServiceType == TNM_PU_SERVICE_KDM2500_PLUS
        || byServiceType == TNM_PU_SERVICE_KDM2417
        || byServiceType == TNM_PU_SERVICE_KDM2418
        || byServiceType == TNM_PU_SERVICE_KDM2428
        || byServiceType == TNM_PU_SERVICE_KDM2518
        || byServiceType == TNM_PU_SERVICE_KDM2401
        || byServiceType == TNM_PU_SERVICE_KDM2401L
        || byServiceType == TNM_PU_SERVICE_KDM2401LS
        || byServiceType == TNM_PU_SERVICE_KDM2402LS
        || byServiceType == TNM_PU_SERVICE_KDM2402L
        || byServiceType == TNM_PU_SERVICE_KDM2401S
        || byServiceType == TNM_PU_SERVICE_KDM2461
        || byServiceType == TNM_PU_SERVICE_KDM2461L
        || byServiceType == TNM_PU_SERVICE_KDM2560
        || byServiceType == TNM_PU_SERVICE_KDM2402S
        || byServiceType == TNM_PU_SERVICE_KDM2402
        || byServiceType == TNM_PU_SERVICE_KDM2440
        || byServiceType == TNM_PU_SERVICE_KDM2440P
        || byServiceType == TNM_PU_SERVICE_KDV8010A
        || byServiceType == TNM_PU_SERVICE_KDV8010C
        || byServiceType == TNM_PU_SERVICE_PU)
    {
        return TRUE;
    }

    return FALSE;
}

//�û���Ϣ�ṹ
typedef struct tagChangePwd
{
    tagChangePwd()
    {
        dwSerialNo = INVALID_SERIAL_NO;
        memset(achOldPwd, 0, sizeof(achOldPwd));
        memset(achNewPwd, 0, sizeof(achNewPwd));
    }
    u32     dwSerialNo;                         // �û���ˮ��
    s8      achOldPwd[MAX_USER_PWD_LEN + 4];    // �û���¼���� 
    s8      achNewPwd[MAX_USER_PWD_LEN + 4];    // �û���¼���� 

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            OspPrintf(TRUE, FALSE, "\tSerialNo: %x; OldPwd: %s; NewPwd: %s;\n", 
                    ntohl(dwSerialNo), achOldPwd, achNewPwd);
            return;
        }
        OspPrintf(TRUE, FALSE, "\tSerialNo: %x; OldPwd: %s; NewPwd: %s;\n", 
                dwSerialNo, achOldPwd, achNewPwd);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TChangePwd;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TChangePwd::NetToHost()
{
    dwSerialNo = ntohl(dwSerialNo);
}
inline void TChangePwd::HostToNet()
{
    dwSerialNo = htonl(dwSerialNo);
}



/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmMask::NetToHost()
{
    dwMask = ntohl(dwMask);
}
inline void TTnmMask::HostToNet()
{
    dwMask = htonl(dwMask);
}

/*========================================================================
�� �� ����Print
��    �ܣ����Դ�ӡ�ӿ�
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/05/22  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmMask::Print(BOOL32 bNetToHost)
{
    if (bNetToHost) 
    {
        NetToHost();
    }

    ::OspPrintf(TRUE, FALSE, "\tMask=0x%x\n", dwMask);

    if (bNetToHost) 
    {
        HostToNet();
    }
}



/*========================================================================
�� �� ����SetMaskAll
��    �ܣ����ظ�ֵ����
��    ����u32 dwItemMask                    [in]�µ�MASKֵ
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/03  3.9     ���ǿ  ����
========================================================================*/
void TTnmMask::SetMaskAll(u32 dwItemMask)
{
    dwMask = dwItemMask;
}

/*========================================================================
�� �� ����SetItemMask
��    �ܣ�����һ�������ֶ���Чλ��
��    ����u32 dwItemMask                        [in]�µ�MASKֵ
          BOOL32 bValid = TRUE                  [in]���û������
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/03  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmMask::SetItemMask(u32 dwItemMask, BOOL32 bValid)
{
    if (bValid) 
    {
        dwMask |= dwItemMask;
    }
    else
    {
        dwMask &= ~dwItemMask;
    }
}

// ��Ȩ����Ϣ
//typedef struct tagFieldRight : public TTnmMask
//{ 
//  TTnmDeviceId tFieldId;
//}PACKED TFieldRight;

// �豸(��)Ȩ����Ϣ������Ϊƽ̨�豸ʹ��ͬһ�ṹ
typedef struct tagDeviceRight : public TTnmMask
{   
    TTnmDeviceId    tDevId;         // �豸��ID��ƽ̨��ǰ�˼������豸
    u8              byRightRange;   // ƽ̨�豸����Ͻ�豸��Ȩ������,���ֶ�ֻ����NMS�����豸�б�ʱ����
                                    //  RIGHT_RANGE_ALL_CHILD
                                    //  RIGHT_RANGE_NO_CHILD
                                    //  RIGHT_RANGE_PART_CHILD
    u8      byReserve1;
    u8      byReserve2;
    u8      byReserve3;

    // 2006/04/04 ���ǿ ���ӵ��Խӿ�
    void Print(BOOL32 bNetToHost = FALSE);
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}TDeviceRight;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TDeviceRight::NetToHost()
{
    TTnmMask::NetToHost();
}
inline void TDeviceRight::HostToNet()
{
    TTnmMask::HostToNet();
}

inline void TDeviceRight::Print(BOOL32 bNetToHost)
{
    u32 dwMask = GetMaskInfo();
    if (bNetToHost) 
    {
        dwMask = ntohl(dwMask);
    }
    OspPrintf(TRUE, FALSE, "\tDevId=%s, Range=%x\n", tDevId.achID, byRightRange);

    PrintDevRight(this);
}

// �û�����Ϣ
typedef struct tagUserGroupInfo
{
    tagUserGroupInfo()
    {
        memset(this, 0, sizeof(tagUserGroupInfo));
        dwGroupId = INVALID_SERIAL_NO;
        tFieldId.SetNull();
    } 
    u32             dwGroupId;                              // �û���Id
    TTnmDeviceId    tFieldId;                       // �����û����ID
    s8              achGroupName[MAX_GROUP_NAME_LEN + 4];   // �û�������
    s8              achGroupDesc[MAX_GROUP_DESC_LEN + 4];   // �û�������
    TDeviceRight    atFieldRight[MAX_FIELD_RIGHT_NUM];      // ��Ȩ��
    TDeviceRight    atDevRight[MAX_DEVICE_RIGHT_NUM];       // �豸Ȩ��
    TTnmMask        tSysRight;                              // ϵͳ����Ȩ��

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            NetToHost();
        }
        
        OspPrintf(TRUE, FALSE, "GroupId=%x, GroupName=%s, Field=%s\n", dwGroupId, achGroupName, tFieldId.achID);
        OspPrintf(TRUE, FALSE, "\tGroupFieldRight: \n");
        s32 nIndex = 0;
        for (nIndex = 0; nIndex < MAX_FIELD_RIGHT_NUM; nIndex++)
        {
            if (atFieldRight[nIndex].tDevId.IsValid()) 
            {
                atFieldRight[nIndex].Print();
            }
            
        }

        OspPrintf(TRUE, FALSE, "\tGroupDevRight : \n");
        for (nIndex = 0; nIndex < MAX_DEVICE_RIGHT_NUM; nIndex++)
        {
            if (atDevRight[nIndex].tDevId.IsValid()) 
            {
                atDevRight[nIndex].Print();
            }
        }
        OspPrintf(TRUE, FALSE, "\tSysRight=0x%x\n", tSysRight.GetMaskInfo());
        if (bNetToHost) 
        {
            HostToNet();
        }
    
    }

    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}TUserGroupInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TUserGroupInfo::NetToHost()
{
    dwGroupId = ntohl(dwGroupId);
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_FIELD_RIGHT_NUM; nIndex++)
    {
        atFieldRight[nIndex].NetToHost();
    }
    for(nIndex = 0; nIndex < MAX_DEVICE_RIGHT_NUM; nIndex++)
    {
        atDevRight[nIndex].NetToHost();
    }
    tSysRight.NetToHost();
}
inline void TUserGroupInfo::HostToNet()
{
    dwGroupId = htonl(dwGroupId);
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_FIELD_RIGHT_NUM; nIndex++)
    {
        atFieldRight[nIndex].HostToNet();
    }
    for(nIndex = 0; nIndex < MAX_DEVICE_RIGHT_NUM; nIndex++)
    {
        atDevRight[nIndex].HostToNet();
    }
    tSysRight.HostToNet();
}

// ������־��Ŀ���ݽṹ
typedef struct tagActionLogItem
{
    tagActionLogItem()
    {
        memset(this, 0, sizeof(tagActionLogItem));
        dwSerialNo = INVALID_SERIAL_NO;
        dwUserId = INVALID_SERIAL_NO;
    }
    u32 dwSerialNo;                     // ��־��ˮ��
    u32 dwUserId;                       // �û���¼Id
    s8  achUserId[TNM_MAX_USER_ID_LEN + 4]; // �û���¼ID
    u32 dwFailReason;                   // ����ʧ��ԭ��
    u32 dwActTime;                      // ��������ʱ��
    s8  achActInfo[MAX_LOG_INFO_LEN + 4];// ���������Ϣ(��ϸ����)
    u8  byActType;                      // ��������
    u8  byResult;                       // �������SUCESS_VALUE/FAILE_VALUE
    
    u8  byReserve1;
    u8  byReserve2;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwTime = dwActTime;
        u32 dwSn = dwSerialNo;
        u32 dwUId = dwUserId;
        u32 dwFail = dwFailReason;

        if (bNetToHost) 
        {
            dwSn = ntohl(dwSerialNo);
            dwUId = ntohl(dwUserId);
            dwFail = ntohl(dwFailReason);
            dwTime = ntohl(dwTime);
        }
        OspPrintf(TRUE, FALSE, "\tSn=%d, UserId=%s(%d), FRsn=%d, Type=%s(%d), Rlt= %d;\n",
            dwSn, achUserId, dwUId, dwFail, GetActTypeDesc(byActType), byActType, byResult);
            
        if (dwTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tInfo=%s\n\t�Ƿ�ʱ��ֵ:%x\n", achActInfo, dwTime);
            return;
        }
        struct tm tmTime = *localtime((time_t*)&dwTime);
        OspPrintf(TRUE, FALSE, "\tInfo=%s\n\tTime=%s\n", achActInfo, asctime(&tmTime));
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TActionLogItem;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TActionLogItem::NetToHost()
{
    dwSerialNo = ntohl(dwSerialNo);
    dwUserId = ntohl(dwUserId);
    dwFailReason = ntohl(dwFailReason);
    dwActTime = ntohl(dwActTime);
}
inline void TActionLogItem::HostToNet()
{
    dwSerialNo = htonl(dwSerialNo);
    dwUserId = htonl(dwUserId);
    dwFailReason = htonl(dwFailReason);
    dwActTime = htonl(dwActTime);
}

//�澯��Ϣ�ṹ
typedef struct tagDevAlarm : public TTnmDevAlarm
{
    tagDevAlarm()
    {
        dwNmsId = INVALID_SERIAL_NO;
        byAlarmLevel = ALARM_NORMAL;
        dwDevId = INVALID_SERIAL_NO;
        //wDevServiceType = TNM_SERVICE_UNKNOW;
        dwDevIp = 0;
        memset(achDevAlias, 0, sizeof(achDevAlias));
        memset(achDesc, 0, sizeof(achDesc));
        memset(achConfirmor, 0, sizeof(achConfirmor));
        memset(achProcessor, 0, sizeof(achProcessor));
        dwAlarmTime = 0;
        dwRecoverTime = 0;
        dwConfirmTime = 0;
        byAlarmState = TNM_STATUS_ALARM;
        byConfirmState = TNM_STATUS_NOCONFIRM;
    }
    u32 dwNmsId;                                        //�澯��¼��ID����NMS���ɲ�ά��
    TTnmDeviceId tDevId;                                //�澯�豸��Id, ��¼�豸��ID��Ϣ,��Ҫд��־����ʾ
    u32 dwDevId;                                        //�豸����ˮ�ţ����ڱȽ�
    u32 dwDevIp;                                        //�豸��IP��ַ
    u32 dwAlarmTime;                                    //�澯����ʱ��  
    u32 dwRecoverTime;                                  //�澯�ָ�ʱ��
    u32 dwConfirmTime;                                  //�澯��ȷ��ʱ��
    //u16   wDevServiceType;                                //�豸ҵ������enum TnmServiceType
    u8  byAlarmState;                                   //�澯��״̬enum TnmStatusType
    u8  byConfirmState;                                 //�澯��ȷ��״̬
    u16 wReserved;
    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //�豸������
    s8  achDevAlias[MAX_DEVICE_ALIAS_LEN + 4];          //�豸����
    s8  achDesc[MAX_ALARM_DESC_LEN + 1];                //�澯����
    s8  achConfirmor[MAX_USER_ID_LEN + 4];              //ȷ����
    s8  achProcessor[MAX_USER_ID_LEN + 4];              //������
    u8  byAlarmLevel;                                   //�澯����
    u8  byParam;                                        //���ڸ澯ϸ�ֵı���
    u8  byDevLargeType;                                 //�豸�����ͣ�����CMU��PU��
    u8  byReserved;

    void PrintDevAlarm(BOOL bNetToHost = FALSE)
    {
        TTnmDevAlarm::Print(bNetToHost);

        u32 dwSN = dwDevId;
        u32 dwAlarmSn = dwNmsId;
        //u16 wSrvType = wDevServiceType;
        u32 dwIp = dwDevIp;
        u32 dwGenTime = dwAlarmTime;
        u32 dwRecTime = dwRecoverTime;

        if (bNetToHost)
        {       
            dwSN = ntohl(dwSN);
            dwAlarmSn = ntohl(dwAlarmSn);
            dwIp = ntohl(dwIp);
            dwGenTime = ntohl(dwGenTime);
            dwRecTime = ntohl(dwRecTime);
        }
        OspPrintf(TRUE, FALSE, "\tAlarmLevel=%s(%d)\n\tDevId: %s;\n", 
            ::GetAlarmLevelDesc(byAlarmLevel), byAlarmLevel, tDevId.achID);
        OspPrintf(TRUE, FALSE, "\tNmsId=0x%x; DevSn=0x%x; DevIp=%s;\n\tDevType=%s\n",
            dwAlarmSn, dwSN, ((LPCTSTR)CCallAddr::GetIpStr(dwIp)), achDevTypeName);
        s8 achGenTime[24] = {0};
        s8 achRecTime[24] = {0};
        if (dwGenTime & 0x80000000)
        {
            sprintf(achGenTime, "�Ƿ�ʱ��ֵ: %x;\n", dwGenTime);
        }
        else
        {
            struct tm tmGenTime = *localtime((time_t*)&dwGenTime);
            memcpy(achGenTime, asctime(&tmGenTime), sizeof(achGenTime));
        }
        if (dwRecTime & 0x80000000)
        {
            sprintf(achRecTime, "�Ƿ�ʱ��ֵ: %x;\n", dwRecTime);
        }
        else
        {
            struct tm tmResTime = *localtime((time_t*)&dwRecTime);
            memcpy(achRecTime, asctime(&tmResTime), sizeof(achRecTime));
        }
                
        OspPrintf(TRUE, FALSE, "\tAlarmState=%s(%d); byParam=%d;\n\tAlarmTime=%s\n\tResumTime: %s\n", 
                ::GetStatusStr(byAlarmState), byAlarmState, byParam, achGenTime, achRecTime);
        OspPrintf(TRUE, FALSE, "\tDevAlias: %s; Desc: %s; \n", achDevAlias, achDesc);

    }
    void NetToHost();
    void HostToNet();
}TDevAlarm;

inline void TDevAlarm::NetToHost()
{
    TTnmDevAlarm::NetToHost();

    dwNmsId = ntohl(dwNmsId);
    dwDevId = ntohl(dwDevId);
    dwDevIp = ntohl(dwDevIp);
    dwAlarmTime = ntohl(dwAlarmTime);
    dwRecoverTime = ntohl(dwRecoverTime);
    dwConfirmTime = ntohl(dwConfirmTime);
    //wDevServiceType = ntohs(wDevServiceType);
}
inline void TDevAlarm::HostToNet()
{
    TTnmDevAlarm::HostToNet();

    dwNmsId = htonl(dwNmsId);
    dwDevId = htonl(dwDevId);
    dwDevIp = htonl(dwDevIp);
    dwAlarmTime = htonl(dwAlarmTime);
    dwRecoverTime = htonl(dwRecoverTime);
    dwConfirmTime = htonl(dwConfirmTime);
    //wDevServiceType = htons(wDevServiceType);
}

//�澯֪ʶ����Ŀ
typedef struct tagAlarmKnowItem
{
    tagAlarmKnowItem()
    {
        memset(this, 0, sizeof(tagAlarmKnowItem));
        dwSerialNo = INVALID_SERIAL_NO;
        dwDevServiceType = TNM_SERVICE_UNKNOW;
    }
    u32 dwSerialNo;                                 // ��ˮ��
    u32 dwAlarmCode;                                // �澯��
    u32 dwDevServiceType;                           // �豸����(�豸�������ͣ�
    s8  achDecription[MAX_KNOW_ITEM_DESC_LEN + 4];  // �澯����
    s8  achCause[MAX_KNOW_ITEM_CAUSE_LEN + 4];      // �澯ԭ��
    s8  achSolution[MAX_KNOW_ITEM_SOLUTION_LEN + 4];// ��ʾ�Ľ���취
    s8  achRemark[MAX_KNOW_ITEM_REMARK_LEN + 4];    // ��ע
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TAlarmKnowItem;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TAlarmKnowItem::NetToHost()
{
    dwSerialNo = ntohl(dwSerialNo);
    dwAlarmCode = ntohl(dwAlarmCode);
    dwDevServiceType = ntohl(dwDevServiceType);
}
inline void TAlarmKnowItem::HostToNet()
{
    dwSerialNo = htonl(dwSerialNo);
    dwAlarmCode = htonl(dwAlarmCode);
    dwDevServiceType = htonl(dwDevServiceType);
}


// ����ƥ��ķ�ʽ
typedef struct tagMatchCondition : public TTnmMask
{   
public:
    tagMatchCondition()
    {
        byMatchType = MATCH_CONDITION_OR;
    }
    void SetMatchType(u8 byType) { byMatchType = byType; }  // ����ƥ������
    u8  GetMatchType() const { return byMatchType; }        // �õ���ǰ��������
protected:
    u8  byMatchType;                // MATCH_CONDITION_OR/MATCH_CONDITION_AND







    
    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TMatchCondition;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TMatchCondition::NetToHost()
{
    TTnmMask::NetToHost();
}
inline void TMatchCondition::HostToNet()
{
    TTnmMask::HostToNet();
}


// ����
typedef struct tagRules : public TMatchCondition
{
    tagRules()
    {
        dwRulesId = INVALID_SERIAL_NO;
        memset(achRulesName, 0, sizeof(achRulesName));
        byEnable = TNM_DISABLE;
    }
    u32 dwRulesId;                              // ����ID����NMS����
    s8  achRulesName[MAX_RULES_NAME_LEN + 3];   // ��������
    u8  byEnable;                               // �Ƿ����øù��� TNM_DISABLE/TNM_ENABLE
    void PrintRuleInfo(BOOL32 bNetToHost = FALSE)
    {   
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "RulesId: 0x%x; EN: %d; MacT: %d; Mask: 0x%x;\n", 
                ntohl(dwRulesId), byEnable, GetMatchType(), ntohl(GetMaskInfo()));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "RulesId: 0x%x; EN: %d; MacT: %d; Mask: 0x%x;\n", 
                dwRulesId, byEnable, GetMatchType(), GetMaskInfo());
        }
        OspPrintf(TRUE, FALSE, "\tRulName: %s;\n", achRulesName);
    }

    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TRules;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TRules::NetToHost()
{
    TMatchCondition::NetToHost();

    dwRulesId = ntohl(dwRulesId);
}
inline void TRules::HostToNet()
{
    TMatchCondition::HostToNet();

    dwRulesId = htonl(dwRulesId);
}

//�澯���˹���
typedef struct tagAlarmFilterRules: public TRules
{   
    tagAlarmFilterRules()
    {
        byAlarmLevel = ALARM_NORMAL;
        memset(adwAlarmCode, 0, sizeof(adwAlarmCode));
        memset(achDevAlias, 0, sizeof(achDevAlias));
        s32 nIndex = 0;
        //for(nIndex = 0; nIndex < MAX_FILTER_RULES_SERVICE_TYPE_NUM; nIndex++)
        //{
        //  awServiceType[nIndex] = TNM_SERVICE_UNKNOW;
        //}
        for(nIndex = 0; nIndex < MAX_FILTER_RULES_DEVID_NUM; nIndex++)
        {
            atDevId[nIndex].SetNull();
        }
        memset(achDevTypeNameList, 0, sizeof(achDevTypeNameList));
    }
    u32 adwAlarmCode[MAX_FILTER_RULES_CODE_NUM];                // �澯��
    s8  achDevAlias[MAX_FILTER_RULES_ALIAS_NUM][MAX_DEVICE_ALIAS_LEN + 1];  // �豸����
    TTnmDeviceId atDevId[MAX_FILTER_RULES_DEVID_NUM];           // �豸��ID
    //u16   awServiceType[MAX_FILTER_RULES_SERVICE_TYPE_NUM];       // �豸��ҵ������
    s8  achDevTypeNameList[MAX_FILTER_RULES_SERVICE_TYPE_NUM][TNM_MAX_DEVTYPE_NAME_LEN + 1];    //�豸�������б�
    u8  byAlarmLevel;               // �澯����,��λ��
                                    // ALARM_CRITICAL
                                    // ALARM_IMPORTANT
                                    // ALARM_GENERAL
    u8  byReserve;
    u16 wReserve;

    void NetToHost();
    void HostToNet();
}TAlarmFilterRules;

inline void TAlarmFilterRules::NetToHost()
{
    TRules::NetToHost();

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_FILTER_RULES_CODE_NUM; nIndex++)
    {
        adwAlarmCode[nIndex] = ntohl(adwAlarmCode[nIndex]);
    }
    //for(nIndex = 0; nIndex < MAX_FILTER_RULES_SERVICE_TYPE_NUM; nIndex++)
    //{
    //  awServiceType[nIndex] = ntohs(awServiceType[nIndex]);
    //}
}
inline void TAlarmFilterRules::HostToNet()
{
    TRules::HostToNet();

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_FILTER_RULES_CODE_NUM; nIndex++)
    {
        adwAlarmCode[nIndex] = htonl(adwAlarmCode[nIndex]);
    }
    //for(nIndex = 0; nIndex < MAX_FILTER_RULES_SERVICE_TYPE_NUM; nIndex++)
    //{
    //  awServiceType[nIndex] = htons(awServiceType[nIndex]);
    //}
}

//�澯��ѯ����
typedef struct tagAlarmQueryCondition : public TMatchCondition
{   
    tagAlarmQueryCondition ()
    {
        byAlarmLevel = ALARM_NORMAL;
        dwAlarmCode = 0;
        dwDevIp = 0;
        //wServiceType = TNM_SERVICE_UNKNOW;
        byAlarmState = TNM_STATUS_NORMAL;           // �澯״̬�ѻָ�
        memset(achDevTypeName, 0, sizeof(achDevTypeName));
        memset(achDeviceAlias, 0, sizeof(achDeviceAlias));
        memset(achAlarmDesc, 0, sizeof(achAlarmDesc));
        dwStartOccurTime = 0;
        dwEndOccurTime = 0;
        dwStartRecoverTime = 0;
        dwEndRecoverTime = 0;
        tDevID.SetNull();
    }
    u32 dwAlarmCode;        // �澯��
    TTnmDeviceId tDevID;    // �豸��ID

    u32 dwDevIp;            // �豸��IP��ַ
    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //�豸������
    u8  byAlarmLevel;       // �澯����, ��λ�� ALARM_CRITICAL/ALARM_IMPORTANT/ALARM_GENERAL
    u8  byAlarmState;       // �澯״̬ enum TnmStatusType
    u8  byDevLargeType;     //�豸�����ͣ�����CMU��PU��
    u8  byReserved;  
    s8  achDeviceAlias[MAX_DEVICE_ALIAS_LEN + 4];   // �豸����
    s8  achAlarmDesc[MAX_ALARM_DESC_LEN + 1];       // �澯����

    u32 dwStartOccurTime;       // �澯����������ʱ�䣬
    u32 dwEndOccurTime;         // �澯����������ʱ��

    u32 dwStartRecoverTime;     // �澯�ָ�������ʱ�䣬
    u32 dwEndRecoverTime;       // �澯�ָ�������ʱ��

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            NetToHost();        
        }

        OspPrintf(TRUE, FALSE, "Code=%d, DevId=%s, DevIp=%s\n",
            dwAlarmCode, tDevID.achID, ::GetIpStr(dwDevIp));
        OspPrintf(TRUE, FALSE, "\tSerType=%s, Level=%s(%d)\n",
            achDevTypeName, ::GetAlarmLevelDesc(byAlarmLevel), byAlarmLevel);
        OspPrintf(TRUE, FALSE, "\tState=%d; Alias=%s, Dsc=%s\n", 
            byAlarmState, achDeviceAlias, achAlarmDesc);
        if (dwStartOccurTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tStartOcurTime: �Ƿ�ʱ��ֵ %x\n", dwStartOccurTime);
        }
        else
        {
            struct tm tmStartGenTime = *localtime((time_t*)&dwStartOccurTime);
            OspPrintf(TRUE, FALSE, "\tStartOcurTime: %s", asctime(&tmStartGenTime));
        }
        if (dwEndOccurTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tEndOcurTime: �Ƿ�ʱ��ֵ %x\n", dwEndOccurTime);
        }
        else
        {
            struct tm tmEndOccurTime = *localtime((time_t*)&dwEndOccurTime);
            OspPrintf(TRUE, FALSE, "\tEndOcurTime: %s", asctime(&tmEndOccurTime));
        }
        if (dwStartRecoverTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tStartResumTime: �Ƿ�ʱ��ֵ %x\n",dwStartRecoverTime);
        }
        else
        {
            struct tm tmStatrResumTime = *localtime((time_t*)&dwStartRecoverTime);
            OspPrintf(TRUE, FALSE, "\tStartResumTime: %s",asctime(&tmStatrResumTime));
        }
        if (dwEndRecoverTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tEndResumTime: �Ƿ�ʱ��ֵ %x", dwEndRecoverTime);
        }
        else
        {
            struct tm tmEndResumTime = *localtime((time_t*)&dwEndRecoverTime);
            OspPrintf(TRUE, FALSE, "\tEndResumTime: %s", asctime(&tmEndResumTime));
        }
        
        if (bNetToHost) 
        {
            HostToNet();
        }
    }

    void NetToHost();
    void HostToNet();
}TAlarmQueryCondition;

inline void TAlarmQueryCondition::NetToHost()
{
    TMatchCondition::NetToHost();

    dwAlarmCode = ntohl(dwAlarmCode);
    //wServiceType = ntohs(wServiceType);
    dwDevIp = ntohl(dwDevIp);
    dwStartOccurTime = ntohl(dwStartOccurTime);
    dwEndOccurTime = ntohl(dwEndOccurTime);
    dwStartRecoverTime = ntohl(dwStartRecoverTime);
    dwEndRecoverTime = ntohl(dwEndRecoverTime);
}
inline void TAlarmQueryCondition::HostToNet()
{
    TMatchCondition::HostToNet();

    dwAlarmCode = htonl(dwAlarmCode);
    dwDevIp = htonl(dwDevIp);
    //wServiceType = htons(wServiceType);
    dwStartOccurTime = htonl(dwStartOccurTime);
    dwEndOccurTime = htonl(dwEndOccurTime);
    dwStartRecoverTime = htonl(dwStartRecoverTime);
    dwEndRecoverTime = htonl(dwEndRecoverTime);
}

//����ͳ������
typedef struct tagTroubleStatCondition
{   
    tagTroubleStatCondition()
    {
        dwStartStatTime = 0;
        dwEndStatTime = 0;
    }
    u32 dwStartStatTime;        //��ʼͳ��ʱ��
    u32 dwEndStatTime;          //����ͳ��ʱ��
    u32 dwStatIntervalTime;     //ͳ��ʱ����(��λ: ��)

    void Print(BOOL32 bNetToHost = FALSE)
    {
    }
    void NetToHost();
    void HostToNet();

}TTroubleStatCondition;

inline void TTroubleStatCondition::NetToHost()
{
    dwStartStatTime = ntohl(dwStartStatTime);
    dwEndStatTime = ntohl(dwEndStatTime);
    dwStatIntervalTime = ntohl(dwStatIntervalTime);
}
inline void TTroubleStatCondition::HostToNet()
{
    dwStartStatTime = htonl(dwStartStatTime);
    dwEndStatTime = htonl(dwEndStatTime);
    dwStatIntervalTime = htonl(dwStatIntervalTime);
}

// ������־��ѯ����
typedef struct tagActionQueryCondition : public TMatchCondition
{   
    tagActionQueryCondition ()
    {
        memset(achUserName, 0, sizeof(achUserName));
        memset(achGroupName, 0, sizeof(achGroupName));
        dwStartTime = 0;
        dwEndTime = 0;
        memset(achActInfo, 0, sizeof(achActInfo));
        memset(abyActType, 0, sizeof(abyActType));
        byActLevelMask = ACTION_INVALID;
    }
    s8  achUserName[MAX_USER_NAME_LEN + 2];     // �û���
    s8  achGroupName[MAX_GROUP_NAME_LEN + 4];   // �û�����
    u32 dwStartTime;                        // ƥ�������ʱ��
    u32 dwEndTime;                          // ƥ�������ʱ��
    s8  achActInfo[MAX_LOG_INFO_LEN + 4];   // ���������Ϣ(��ϸ����)
    u8  abyActType[ACT_TYPE_NUM];           // ���������Ͳ�ѯ INVALID_VALUE/VALID_VALUE
    u8  byActLevelMask;                     // �����������ѯ,��λ��
                                            // ACTION_IMPORTANT/ACTION_SECONDARY/ACTION_GENERAL

    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwSTime = dwStartTime;
        u32 dwETime = dwEndTime;
        if (bNetToHost) 
        {
            dwSTime = ntohl(dwSTime);
            dwETime = ntohl(dwEndTime);
        }
        s8 achStmp[24] ={0};
        s8 achEtmp[24] = {0};
        if (dwSTime & 0x80000000)
        {
            sprintf(achStmp, "�Ƿ�ʱ��ֵ: %x\n", dwSTime);
        }
        else
        {
            struct tm tmStime = *localtime((time_t*)&dwSTime);
            memcpy(achStmp, asctime(&tmStime), sizeof(achStmp));
        }
        if (dwETime & 0x80000000)
        {
            sprintf(achEtmp, "�Ƿ�ʱ��ֵ: %x\n", dwETime);
        }
        else
        {
            struct tm tmEtime = *localtime((time_t*)&dwETime);
            memcpy(achEtmp, asctime(&tmEtime), sizeof(achEtmp));
        }
        
        OspPrintf(TRUE, FALSE, "UName=%s, GName=%s\n", achUserName, achGroupName);
        OspPrintf(TRUE, FALSE, "\tSTime=(%d)%s\n\tETime=(%d)%s\n", 
            dwSTime, achStmp, dwETime, achEtmp);

        OspPrintf(TRUE, FALSE, "\tActInfo: %s; ActLev=", achActInfo);
        if (byActLevelMask & ACTION_IMPORTANT) 
        {
            OspPrintf(TRUE, FALSE, "IMPORTANT, ");
        }
        if (byActLevelMask & ACTION_SECONDARY) 
        {
            OspPrintf(TRUE, FALSE, "SECONDARY, ");
        }
        if (byActLevelMask & ACTION_GENERAL) 
        {
            OspPrintf(TRUE, FALSE, "GENERAL, ");
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tActType: ");  
        u8 byIndex = 0;
        for (byIndex = 0; byIndex < ACT_TYPE_NUM; byIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d", abyActType[byIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        for(byIndex = 0; byIndex < ACT_TYPE_NUM; byIndex++)
        {
            if (abyActType[byIndex] == VALID_VALUE) 
            {
                OspPrintf(TRUE, FALSE, "\t%s\n", ::GetActTypeDesc(byIndex));
            }
            
        }

        u32 dwMaskInfo = GetMaskInfo();
        if (bNetToHost) 
        {
            dwMaskInfo = ntohl(dwMaskInfo);
        }
        u8 byMatch = GetMatchType();
        OspPrintf(TRUE, FALSE, "\tMatch=%d, Mask=0x%x, Findby: \n", byMatch, dwMaskInfo);
        if (dwMaskInfo & ACTION_QUERY_USER_NAME) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_USER_NAME\n");
        }
        if (dwMaskInfo & ACTION_QUERY_GROUP_NAME) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_GROUP_NAME\n");
        }
        if (dwMaskInfo & ACTION_QUERY_TIME) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_TIME\n");
        }
        if (dwMaskInfo & ACTION_QUERY_DEV_ID) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_DEV_ID\n");
        }
        if (dwMaskInfo & ACTION_QUERY_ACT_TYPE) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_ACT_TYPE\n");
        }
        if (dwMaskInfo & ACTION_QUERY_ACT_INFO) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_ACT_INFO\n");
        }
        if (dwMaskInfo & ACTION_QUERY_ACT_LEVEL) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_ACT_LEVEL\n");
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TActionQueryCondition;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TActionQueryCondition::NetToHost()
{
    TMatchCondition::NetToHost();

    dwStartTime = ntohl(dwStartTime);
    dwEndTime = ntohl(dwEndTime);
}
inline void TActionQueryCondition::HostToNet()
{
    TMatchCondition::HostToNet();

    dwStartTime = htonl(dwStartTime);
    dwEndTime = htonl(dwEndTime);
}

// IP��ַ����
typedef struct tagIpZone
{
    tagIpZone()
    {
         dwStartIP = 0;
         dwEndIP = 0;
    }
    u32 dwStartIP;              // ��ʼIP
    u32 dwEndIP;                // ����IP
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TIpZone;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TIpZone::NetToHost()
{
    dwStartIP = ntohl(dwStartIP);
    dwEndIP = ntohl(dwEndIP);
}
inline void TIpZone::HostToNet()
{
    dwStartIP = htonl(dwStartIP);
    dwEndIP = htonl(dwEndIP);
}

//���νṹ
typedef struct tagIpSegment
{
    tagIpSegment()
    {
        dwIpSegment = 0;
        dwNetMask = 0;
    }
    u32 dwIpSegment;
    u32 dwNetMask;
    void HostToNet();
    void NetToHost();
}TIpSegment;

inline void TIpSegment::HostToNet()
{
    dwIpSegment = htonl(dwIpSegment);
    dwNetMask = htonl(dwNetMask);
}

inline void TIpSegment::NetToHost()
{
    dwIpSegment = ntohl(dwIpSegment);
    dwNetMask = ntohl(dwNetMask);
}


// IP���޹���
typedef struct tagIpRules : public TRules
{
    tagIpRules()
    {
        byAccessType = INVALID_VALUE;
        memset(adwIPList, 0, sizeof(adwIPList));
    }
    u32 adwIPList[MAX_IP_RULES_IPLIST_SIZE];            // IP�б�
    TIpSegment tIPSegment[MAX_IP_RULES_IPSEGMENT_SIZE]; // IP����
    TIpZone tIpZone[MAX_IP_RULES_IPZOOM_SIZE];      // IP��ַ����
    u8  byAccessType;               // �ù���ķ�������:
                                    // TNM_ENABLE�������/TNM_DISABLE ����ֹ����
    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    void PrintIpRules(BOOL32 bNetToHost = FALSE)
    {   
        PrintRuleInfo(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tAccessType: %d;\n", byAccessType);
        OspPrintf(TRUE, FALSE, "\tIpList: ");
        s32 nIndex = 0;
        for (nIndex = 0; nIndex < MAX_IP_RULES_IPLIST_SIZE; nIndex++)
        {
            if ((nIndex + 1)%6 == 0)
            {
                OspPrintf(TRUE, FALSE, "\n\t");
            }
            if (bNetToHost) 
            {
                OspPrintf(TRUE, FALSE, "0x%x-", ntohl(adwIPList[nIndex]));
            }
            else
            {
                OspPrintf(TRUE, FALSE, "0x%x-", adwIPList[nIndex]);
            }
        }
        OspPrintf(TRUE, FALSE, "\n");

        OspPrintf(TRUE, FALSE, "\tIpSeg: ");
        for (nIndex = 0; nIndex < MAX_IP_RULES_IPSEGMENT_SIZE; nIndex++)
        {
            if ((nIndex+1)%6 == 0)
            {
                OspPrintf(TRUE, FALSE, "\n\t");
            }
            if (bNetToHost) 
            {
                OspPrintf(TRUE,FALSE, "(0x%x:0x%x)-", ntohl(tIPSegment[nIndex].dwIpSegment), ntohl(tIPSegment[nIndex].dwNetMask));
            }
            else
            {
                OspPrintf(TRUE,FALSE, "(0x%x:0x%x)-", tIPSegment[nIndex].dwIpSegment, tIPSegment[nIndex].dwNetMask);
            }
            
        }
        OspPrintf(TRUE, FALSE, "\n");

        OspPrintf(TRUE, FALSE, "\tIpZone: ");
        for (nIndex = 0; nIndex < MAX_IP_RULES_IPZOOM_SIZE; nIndex++)
        {
            if ((nIndex+1)%2 == 0)
            {
                OspPrintf(TRUE, FALSE, "\n\t");
            }
            if (bNetToHost) 
            {
                OspPrintf(TRUE, FALSE, "(0x%x, 0x%x)-", 
                    ntohl(tIpZone[nIndex].dwStartIP), ntohl(tIpZone[nIndex].dwEndIP));
            }
            else
            {
                OspPrintf(TRUE, FALSE, "(0x%x, 0x%x)-", 
                    tIpZone[nIndex].dwStartIP, tIpZone[nIndex].dwEndIP);
            }
        }
        OspPrintf(TRUE, FALSE, "\n");

    }

    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}TIpRules;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TIpRules::NetToHost()
{
    TRules::NetToHost();

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPLIST_SIZE; nIndex++)
    {
        adwIPList[nIndex] = ntohl(adwIPList[nIndex]);
    }
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPSEGMENT_SIZE; nIndex++)
    {
        tIPSegment[nIndex].NetToHost();
    }
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPZOOM_SIZE; nIndex++)
    {
        tIpZone[nIndex].NetToHost();
    }
}
inline void TIpRules::HostToNet()
{
    TRules::HostToNet();

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPLIST_SIZE; nIndex++)
    {
        adwIPList[nIndex] = htonl(adwIPList[nIndex]);
    }
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPSEGMENT_SIZE; nIndex++)
    {
        tIPSegment[nIndex].HostToNet();
    }
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPZOOM_SIZE; nIndex++)
    {
        tIpZone[nIndex].HostToNet();
    }
}

//����Email�Ĺ���
typedef struct tagTEmailRules : public TRules
{
    tagTEmailRules()
    {
        //memset(awServiceTypeList, 0, sizeof(awServiceTypeList));
        memset(achDevTypeNameList, 0, sizeof(achDevTypeNameList));
        memset(achDevNameList, 0, sizeof(achDevNameList));
        memset(adwDevIpList, 0, sizeof(adwDevIpList));
        memset(adwAlarmCodeList, 0, sizeof(adwAlarmCodeList));
        byAlarmLevel = ALARM_NORMAL;
        memset(achMailList, 0, sizeof(achMailList));
        memset(achSmsList, 0, sizeof(achMailList));
    }

    //u16   awServiceTypeList[MAX_MAIL_RULES_DEV_TYPE_SIZE];    // �豸�������б�
    s8  achDevTypeNameList[MAX_MAIL_RULES_DEV_TYPE_SIZE][TNM_MAX_DEVTYPE_NAME_LEN + 1]; //�豸�������б�
    TTnmDeviceId atDevIdList[MAX_MAIL_RULES_DEV_ID_SIZE];   // �豸Id �б�
    s8  achDevNameList[MAX_MAIL_RULES_DEV_NAME_SIZE][MAX_DEVICE_ALIAS_LEN + 4]; // �豸�����б�
    u32 adwDevIpList[MAX_MAIL_RULES_DEV_IP_SIZE];               // �豸IP�б�
    u32 adwAlarmCodeList[MAX_MAIL_RULES_ALARM_CODE_SIZE];       // �澯���б�
    s8  achMailList[MAX_MAIL_RULES_ADDR_NUM][MAX_EMAIL_LEN + 4];    // �����ʼ��б�
    s8  achSmsList[MAX_SMS_RULES_ADDR_NUM][MAX_SMS_LEN + 4];    // ���Ͷ����б�
    u8  byAlarmLevel;           // �澯���� ��λ�룬
                                // ALARM_CRITICAL/ALARM_IMPORTANT/ALARM_GENERAL
    u8  byReserved;
    u16 wReserved;
    void NetToHost();
    void HostToNet();
}TEmailRules;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TEmailRules::NetToHost()
{
    TRules::NetToHost();

    s32 nIndex = 0;
//  for(nIndex = 0; nIndex < MAX_MAIL_RULES_DEV_TYPE_SIZE; nIndex++)
//  {
//      awServiceTypeList[nIndex] = ntohs(awServiceTypeList[nIndex]);
//  }
    for(nIndex = 0; nIndex < MAX_MAIL_RULES_DEV_IP_SIZE; nIndex++)
    {
        adwDevIpList[nIndex] = ntohl(adwDevIpList[nIndex]);
    }
    for(nIndex = 0; nIndex < MAX_MAIL_RULES_ALARM_CODE_SIZE; nIndex++)
    {
        adwAlarmCodeList[nIndex] = ntohl(adwAlarmCodeList[nIndex]);
    }
}
inline void TEmailRules::HostToNet()
{
    TRules::HostToNet();

    s32 nIndex = 0;
//  for(nIndex = 0; nIndex < MAX_MAIL_RULES_DEV_TYPE_SIZE; nIndex++)
//  {
//      awServiceTypeList[nIndex] = htons(awServiceTypeList[nIndex]);
//  }
    for(nIndex = 0; nIndex < MAX_MAIL_RULES_DEV_IP_SIZE; nIndex++)
    {
        adwDevIpList[nIndex] = htonl(adwDevIpList[nIndex]);
    }
    for(nIndex = 0; nIndex < MAX_MAIL_RULES_ALARM_CODE_SIZE; nIndex++)
    {
        adwAlarmCodeList[nIndex] = htonl(adwAlarmCodeList[nIndex]);
    }
}

//������������ñ���
typedef struct tagNmsCfgInfo
{
    tagNmsCfgInfo()
    {
        memset(this, 0, sizeof (tagNmsCfgInfo));
    }

    u32 dwNmsIp;                                    //���ܷ�������ַ
    u32 dwOnlinePeriod;                             //���ߺ��еĶ�ʱʱ��
    u32 dwStatusPeriod;                             //״̬��ѯ�Ķ�ʱʱ��
    u32 dwViewPfmPeriod;                            //�û��鿴�豸��������ѯ��ʱʱ��
    u32 dwCheckTopoPeriod;                          //������������ԵĶ�ʱʱ��
    u32 dwMaxDevCount;                              //���ܷ���������ע�������豸����  
    TTnmDeviceId tFieldId;                          //��Ҫ�������Id
    u8  byIsMaster;                                 //�Ƿ�������
    u8  byIfChild;                                  //�Ƿ������������
    u8  byDbType;                                   //���ݿ����� #define DBTYPE_SYBASE     1   //sybase���ݿ�
                                                    //           #define DBTYPE_MYSQL      2   //mysql ���ݿ�
    u8  byHBNum;                                    //�豸�����ļ�����

    u32 dwDBIpAddr;                                 //���������ӵ����ݿ��ַ
    u32 dwDbLinkNum;                                //���ݿ�������
    u16 wDBPort;                                    //�������ݿ�Ķ˿ں�
    u16 wOspConnPort;                               //������Osp�����˿�
    s8  achDBName[MAX_DB_NAME_LEN + 4];             //���������ӵ����ݿ���
    s8  achDBUser[TNM_MAX_USER_ID_LEN + 4];         //���������ӵ��û���
    s8  achDBPwd[TNM_MAX_PASSWORD_LEN + 4];         //���������ӵ��û�����
    u16 wDbLangId;                                  //DB_LANG_SC_CHN
    u16 wDbCharSetId;                               //DB_CHARSET_GB18030

    u32 dwAutoClrLogClock;                          //�Զ������־��ʱ��
    u32 dwAutoClrAlarmClock;                        //�Զ�����澯��ʱ��

    u32 dwMaxActLogSaveDay;                         //������־��󱣴�����
    u32 dwMaxAlarmSaveDay;                          //�澯��־��󱣴�����

    u8  byIsAutoConfirm;                            //�Ƿ��Զ�ȷ�ϸ澯
    u8  byIsAutoBackupDevCfg;                       //�Ƿ��Զ������豸�����ļ�
    u16 wAutoConfirmTime;                           //�Զ�ȷ�ϸ澯��ʱ��(����)
    u16 wAutoBackupDevCfgTime;                      //�Զ������豸�����ļ���ʱ��(����)
    u8  byReserved;

    u8  byMemThreshold;                             //ϵͳ�ڴ�ʹ����ֵ��0-100��
    u8  byCpuThreshold;                             //ϵͳCPUʹ����ֵ��0-100��
    u16 wReserved;

    //mail
    u8  byMailServEnable;                           //�Ƿ������ʼ�����������  1 ����.0 û����
    u32 dwMailServerIP;                             //�ʼ���������IP��ַ
    u16 wMailServerPort;                            //�ʼ��������Ķ˿�(Ĭ��Ϊ��25)
    u8  byMailServerAuthMode;                       //�ʼ���������֤��ʽ 0:����֤ 1:login 2:plain 3:cram_md5 4:digest_md5
    u8  byReserved1;
    s8  achEmailAuthUser[TNM_MAX_USER_ID_LEN + 4];  //�ʼ���������֤�û���
    s8  achEmailAuthPwd[TNM_MAX_PASSWORD_LEN + 4];  //�ʼ���������֤����
    u32 dwMailInterval;                             //�ʼ����ͼ��(����)

    s8  achBkFileName[MAX_BK_FILE_NAME_LEN + 4];    // ���ݿⱸ�ݵ��ļ���ǰ׺
    u16 wHBTime;                                    // �豸�����ļ��ʱ��
    u8  byLogSwitch;                                //ϵͳ�Ƿ��¼��־�Ŀ���

    //SMS
    u8  bySmsServEnable;                            //�Ƿ�����SMS����������  1 ����.0 û����
    u32 dwSmsServerIp;                              //������IP
    u16 wSmsServerPort;                             //�������˿ڣ�Ĭ��Ϊ7777�������0����ʾʹ��Ĭ��ֵ
    u16 wReserved2;
    s8  achSmsSrcAddr[MAX_SMS_LEN + 4];             //���ط����
    s8  achSmsAccount[TNM_MAX_USER_ID_LEN + 4];     //�û���
    s8  achSmsPassword[TNM_MAX_PASSWORD_LEN + 4];   //����

    //����ӿڲ���
    u8  byNorthEnable;                              //1 ����.0 û����
    u8  byReserved3;
    u16 wNorthListenPort;
    u32 dwNorthProxyIp;
    u16 wNorthProxyPort;
    u16 wReserved3;

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tNmsIp=%s, OspPort=0x%x, Online=0x%x��ViewPfm=0x%x, CheckTopo=%d\n",
            ((LPCTSTR)CCallAddr::GetIpStr(dwNmsIp)), wOspConnPort, dwOnlinePeriod, dwViewPfmPeriod, dwCheckTopoPeriod);
        OspPrintf(TRUE, FALSE, "\tStatusPeriod=0x%x, DbIp=%s, DbPort=0x%x, LinkNum=0x%x, DbType=0x%x;\n", 
            dwStatusPeriod, ((LPCTSTR)CCallAddr::GetIpStr(dwDBIpAddr)), wDBPort, dwDbLinkNum, byDbType);
        OspPrintf(TRUE, FALSE, "\tDbName: %s; DbUser: %s; DbPwd: %s; DbLanId: %d; DbCharSet:%d\n", 
            achDBName, achDBUser, achDBPwd, wDbLangId, wDbCharSetId);
        OspPrintf(TRUE, FALSE, "\tClsLogClk=0x%x, ClsAlrClk=0x%x, MaxLogDay=0x%x, MaxAlrDay=0x%x\n",
            dwAutoClrLogClock, dwAutoClrAlarmClock, dwMaxActLogSaveDay, 
            dwMaxAlarmSaveDay);
        OspPrintf(TRUE, FALSE, "\tMailSvrIp: %s; MailInterval=0x%x; BkFileName: %s\n",
            ((LPCTSTR)CCallAddr::GetIpStr(dwMailServerIP)), dwMailInterval, achBkFileName);
        OspPrintf(TRUE, FALSE, "\tHB=0x%x, HBNum=%d, MaxDevCount=%d, IsMaster=%d\n", wHBTime, byHBNum, dwMaxDevCount, byIsMaster);
        OspPrintf(TRUE, FALSE, "\tFieldId=%s, IfChild=%d, logswitch=%d\n", tFieldId.achID, byIfChild, byLogSwitch);
        OspPrintf(TRUE, FALSE, "\tNorthEnable=%d, ListenPort=%d, NorthProxyIp=%s, NorthProxyPort=%d\n",
                                byNorthEnable, wNorthListenPort, ((LPCTSTR)CCallAddr::GetIpStr(dwNorthProxyIp)), wNorthProxyPort);

        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}TNmsCfgInfo;

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TNmsCfgInfo::NetToHost()
{
    dwNmsIp = ntohl(dwNmsIp);
    dwOnlinePeriod = ntohl(dwOnlinePeriod);
    dwStatusPeriod = ntohl(dwStatusPeriod);
    dwViewPfmPeriod = ntohl(dwViewPfmPeriod);
    dwCheckTopoPeriod = ntohl(dwCheckTopoPeriod);
    dwDBIpAddr = ntohl(dwDBIpAddr);
    dwDbLinkNum = ntohl(dwDbLinkNum);
    wDBPort = ntohs(wDBPort);
    wOspConnPort = ntohs(wOspConnPort);
    dwAutoClrLogClock = ntohl(dwAutoClrLogClock);
    dwAutoClrAlarmClock = ntohl(dwAutoClrAlarmClock);
    dwMaxActLogSaveDay = ntohl(dwMaxActLogSaveDay);
    dwMaxAlarmSaveDay = ntohl(dwMaxAlarmSaveDay);
    dwMailServerIP = ntohl(dwMailServerIP);
    wMailServerPort = ntohs(wMailServerPort);
    dwMailInterval = ntohl(dwMailInterval);
    wHBTime = ntohs(wHBTime);
    wAutoConfirmTime = ntohs(wAutoConfirmTime);
    wAutoBackupDevCfgTime = ntohs(wAutoBackupDevCfgTime);

    dwSmsServerIp = ntohl(dwSmsServerIp);
    wSmsServerPort = ntohs(wSmsServerPort);

    wNorthListenPort = ntohs(wNorthListenPort);
}

inline void TNmsCfgInfo::HostToNet()
{
    dwNmsIp = htonl(dwNmsIp);
    dwOnlinePeriod = htonl(dwOnlinePeriod);
    dwStatusPeriod = htonl(dwStatusPeriod);
    dwViewPfmPeriod = htonl(dwViewPfmPeriod);
    dwCheckTopoPeriod = htonl(dwCheckTopoPeriod);
    dwDBIpAddr = htonl(dwDBIpAddr);
    dwDbLinkNum = htonl(dwDbLinkNum);
    wDBPort = htons(wDBPort);
    wOspConnPort = htons(wOspConnPort);
    dwAutoClrLogClock = htonl(dwAutoClrLogClock);
    dwAutoClrAlarmClock = htonl(dwAutoClrAlarmClock);
    dwMaxActLogSaveDay = htonl(dwMaxActLogSaveDay);
    dwMaxAlarmSaveDay = htonl(dwMaxAlarmSaveDay);
    dwMailServerIP = htonl(dwMailServerIP);
    wMailServerPort = htons(wMailServerPort);
    dwMailInterval = htonl(dwMailInterval);
    wHBTime = htons(wHBTime);
    wAutoConfirmTime = htons(wAutoConfirmTime);
    wAutoBackupDevCfgTime = htons(wAutoBackupDevCfgTime);

    dwSmsServerIp = htonl(dwSmsServerIp);
    wSmsServerPort = htons(wSmsServerPort);

    wNorthListenPort = htons(wNorthListenPort);
}

// ������Ϣ
typedef struct tagDiskInfo
{
    tagDiskInfo()
    {
        byDiskLabel = 0;
        dwFreeSpace = 0;
    }
    u32 dwFreeSpace;        // ʣ����̿ռ�(M)
    u8  byDiskLabel;        // �̷� a-z

    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE,FALSE, "FreeSpace: 0x%x; DiskLabel: %c\n", ntohl(dwFreeSpace), byDiskLabel);
        }
        else
        {
            OspPrintf(TRUE,FALSE, "FreeSpace: 0x%x; DiskLabel: %c\n", dwFreeSpace, byDiskLabel);
        }
        
    }

    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TDiskInfo;

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TDiskInfo::NetToHost()
{
    dwFreeSpace = ntohl(dwFreeSpace);
}
inline void TDiskInfo::HostToNet()
{
    dwFreeSpace = htonl(dwFreeSpace);
}
typedef struct tagAutoAction
{
    tagAutoAction()
    {
        byPolicy = AUTO_ACT_PERIODICAL;
        byActDay = 1;
        dwActTime = 0;
    }
    u32 dwActTime;          // ���ݵ�ʱ��(min)
    u8  byActDay;           // ����һ�챸�� ��byPolicyΪAUTO_ACT_WEEKDAYʱȡֵΪ
                            // WEEK_MONDAY
                            // WEEK_TUESDAY
                            // WEEK_WEDNESDAY
                            // WEEK_THURSDAY
                            // WEEK_FRIDAY
                            // WEEK_SATURDAY
                            // WEEK_SUNDAY
                            // ��byPolicyΪAUTO_ACT_PERIODICALʱȡֵΪ����
    u8  byPolicy;           // �Զ������Ĳ���
                            // AUTO_ACT_WEEKDAY
                            // AUTO_ACT_PERIODICAL

    u8  byReserve1;
    u8  byReserve2;
    
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwTime = dwActTime;
        if (bNetToHost) 
        {
            dwTime = ntohl(dwTime);
        }
        
        OspPrintf(TRUE, FALSE, "Day: %d; Poli: %d; Timevalue: %x;\n", byActDay, byPolicy, dwTime);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TAutoAction;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TAutoAction::NetToHost()
{
    dwActTime = ntohl(dwActTime);

}
inline void TAutoAction::HostToNet()
{
    dwActTime = htonl(dwActTime);
}

typedef struct tagAutoBackup : public TAutoAction
{
    tagAutoBackup()
    {
        byEnable = TNM_ENABLE;
    }
    u8  byEnable;           // �Ƿ������Զ����ݹ��� TNM_ENABLE/TNM_DISABLE

    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;

    void PrintAutoBkup(BOOL32 bNetToHost = FALSE)
    {
        Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tEnable: %d; \n", byEnable);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TAutoBackup;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TAutoBackup::NetToHost()
{
    TAutoAction::NetToHost();

}
inline void TAutoBackup::HostToNet()
{
    TAutoAction::HostToNet();
}

typedef struct tagAutoClearAct : public TAutoAction
{
    tagAutoClearAct()
    {
        byEnable = TNM_ENABLE;
    }
    u8  byEnable;           // �Ƿ������Զ����������־���� TNM_ENABLE/TNM_DISABLE

    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TAutoClearAct;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TAutoClearAct::NetToHost()
{
    TAutoAction::NetToHost();

}
inline void TAutoClearAct::HostToNet()
{
    TAutoAction::HostToNet();
}

typedef struct tagAutoClearAlarm : public TAutoAction
{
    tagAutoClearAlarm()
    {
        byEnable = TNM_ENABLE;
    }
    u8  byEnable;           // �Ƿ������Զ�����澯���� TNM_ENABLE/TNM_DISABLE

    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TAutoClearAlarm;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TAutoClearAlarm::NetToHost()
{
    TAutoAction::NetToHost();

}
inline void TAutoClearAlarm::HostToNet()
{
    TAutoAction::HostToNet();
}


// ���Ի���Ϣ
typedef struct tagTPreferInfo
{
    tagTPreferInfo()
    {
        memset(this, 0, sizeof (tagTPreferInfo));
    }
    // to do
}PACKED TPreferInfo;    

// �ͻ���������Ϣ,�������ݲμ�ǰ��궨��NMC_REQ_BASE_PFM_INFO|NMC_REQ_EXTEND_PFM_INFO
typedef struct tagTNmcReqInfo : public TTnmMask
{
    tagTNmcReqInfo()
    {
        dwNmcInsId = 0;
        dwDevId = INVALID_SERIAL_NO;
    }
    u32 dwNmcInsId;     // NMC��ʵ����
    u32 dwDevId;        // �豸��ID
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmMask::Print(bNetToHost);

        if (bNetToHost) 
        {
            NetToHost();
        }

        u32 dwMaskInfo = GetMaskInfo();

        OspPrintf(TRUE, FALSE, "\tNmcInsId: 0x%x; DevId: 0x%x;\n", dwNmcInsId, dwDevId);

        OspPrintf(TRUE, FALSE, "\t");
        if (dwMaskInfo & NMC_REQ_EXTEND_PFM_INFO)
        {
            OspPrintf(TRUE, FALSE, "NMC_REQ_EXTEND_PFM_INFO | ");
        }
        
        if (dwMaskInfo & NMC_REQ_BASE_PFM_INFO)
        {
            OspPrintf(TRUE, FALSE, "NMC_REQ_BASE_PFM_INFO | ");
        }

        if (dwMaskInfo & NMC_REQ_FOR_MOCK_DRAW) 
        {
            OspPrintf(TRUE, FALSE, "NMC_REQ_FOR_MOCK_DRAW | ");
        }

        if (bNetToHost) 
        {
            HostToNet();
        }

    }

    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TNmcReqInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TNmcReqInfo::NetToHost()
{
    TTnmMask::NetToHost();

    dwNmcInsId = ntohl(dwNmcInsId);
    dwDevId = ntohl(dwDevId);
}
inline void TNmcReqInfo::HostToNet()
{
    TTnmMask::HostToNet();

    dwNmcInsId = htonl(dwNmcInsId);
    dwDevId = htonl(dwDevId);
}

// PING�ṹ
typedef struct tagPingInfo
{
    tagPingInfo()
    {
        nEchTime = 0;
        dwTTL = 0;
        dwDevId = INVALID_SERIAL_NO;
        dwDestIp = 0;
        dwPingThreadId = 0;
    }
    s32 nEchTime;
    u32 dwTTL;
    u32 dwDevId;        // �豸ID
    u32 dwDestIp;       // Ŀ���豸IP
    u32 dwPingThreadId;    // ÿ��ping�̵߳�ID

    void Print(BOOL bNetToHost = FALSE) //renhouping 2006.04.08
    {
        if (bNetToHost)
        {
            OspPrintf(TRUE, FALSE, "EachTime: %d; TTL: %x; DevId: %x; DestIp: %s;\n",
                    ntohl(nEchTime), ntohl(dwTTL), ntohl(dwDevId), 
                    ((LPCTSTR)CCallAddr::GetIpStr(ntohl(dwDestIp))));
            return;             
        }
        OspPrintf(TRUE, FALSE, "EachTime: %d; TTL: %x; DevId: %x; DestIp: %s;\n",
                    nEchTime, dwTTL, dwDevId, ((LPCTSTR)CCallAddr::GetIpStr(dwDestIp)));
                    
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TPingInfo;

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TPingInfo::NetToHost()
{
    nEchTime = ntohl(nEchTime);
    dwTTL = ntohl(dwTTL);
    dwDevId = ntohl(dwDevId);
    dwDestIp = ntohl(dwDestIp);
    dwPingThreadId = ntohl(dwPingThreadId);
}
inline void TPingInfo::HostToNet()
{
    nEchTime = htonl(nEchTime);
    dwTTL = htonl(dwTTL);
    dwDevId = htonl(dwDevId);
    dwDestIp = htonl(dwDestIp);
    dwPingThreadId = htonl(dwPingThreadId);
}

/*========================================================================
�� �� ����GetAlarmLevel
��    ��: ͨ���澯��õ��澯����
��    ����u32 dwAlarmCode               [in]�澯��
�� �� ֵ���澯����
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/08  3.9     ���ǿ  ����
2006/03/13  3.9     �κ�ƽ  ʵ��
========================================================================*/
inline u32 GetAlarmLevel(u32 dwAlarmCode)
{
    switch(dwAlarmCode)
    {
        //���ظ澯
    case TNM_ALARM_CODE_DEV_OFFLINE:           //10001       �豸����   
    case TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS:   //10013       ǰ���豸��ƵԴ��ʧ�澯 
    case TNM_ALARM_CODE_RECV_NOBITSTREAM:      //10002       �����������ϱ� 
    case TNM_ALARM_CODE_MAP_STATUS:            //10004       MAP�����쳣�澯    
    case TNM_ALARM_CODE_CPU_STATUS:            //10008       CPUʹ���ʸ���ָ����ֵ�澯          
    case TNM_ALARM_CODE_MEMORY_STATUS:         //10009       �ڴ�ʹ�ó���ָ����ֵ�澯       
    case TNM_ALARM_CODE_DISKFULL_STATUS:       //10010       �������澯
    case TNM_ALARM_CODE_SAME_REG_ID:           //10015       �豸ID��ͻ
    case TNM_ALARM_CODE_REG_CMU:               //10016       PU������ģ��ע��CMUʧ�ܸ澯
    case TNM_ALARM_CODE_DISKDETECT_ERROR:      //10017       �豸����޴��̻��޿��ô��̸澯
    case TNM_ALARM_CU_OVERLOAD:
    case TNM_ALARM_PU_OVERLOAD:
    case TNM_ALARM_VTDU_OVERLOAD:
	case TNM_ALARM_CODE_NODISK_ERROR:			//10100       ��Ӳ�� - ��IDE���(0-3)  
	case TNM_ALARM_CODE_DISK_ERROR:				//10101       Ӳ�̷����� -���������(0-16)
    case TNM_ALARM_CODE_DISK_NOFORMAT:			//10102       Ӳ�̷���δ��ʽ�� - ���������(0-16)
    case TNM_ALARM_CODE_STOPREC_LOGERR:			//10103       ǰ��ͣ¼ - ��־ϵͳ��ͣ¼
    case TNM_ALARM_CODE_STOPREC_DISKERR:		//10104       ǰ��ͣ¼ - Ӳ�̻�ͣ¼
    case TNM_ALARM_CODE_DISCONNECT_3AS:         //11001       δ������3AS
    case TNM_ALARM_CODE_DISCONNECT_UAS:         //11002       δ������UAS
    case TNM_ALARM_CODE_DISCONNECT_PUI:         //11003       δ������PUI
    case TNM_ALARM_CODE_DISCONNECT_CUI:         //11004       δ������CUI
    case TNM_ALARM_CODE_DISCONNECT_NRU:         //11005       δ������NRU
    case TNM_ALARM_CODE_DISCONNECT_VTDU:        //11006       δ������VTDU
    case TNM_ALARM_CODE_DISCONNECT_CMU:         //11007       δ������CMU
    case TNM_ALARM_CODE_DISCONNECT_DATABASE:    //11008       δ������DATABASE

        {   
            return ALARM_CRITICAL;
        }

        //��Ҫ�澯
    case TNM_ALARM_CODE_LOSEPACKET:            //10003       �������ն�������ָ����ֵ�澯       
    case TNM_ALARM_CODE_LPT_STATUS:            //10006       ���ڸ澯�ϱ�       
    case TNM_ALARM_CODE_POWER_STATUS:          //10007       ��Դ��ѹ����ָ����ֵ�澯
    case TNM_ALARM_CODE_FAN_STATUS:            //10011       ����ת�ٹ��͸澯
        {
            return ALARM_IMPORTANT;
        }

        //��ͨ�澯
    case TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET:            
    case TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION:
    case TNM_ALARM_CODE_MOTIONDETECT_STATUS:   //10005       �ƶ����澯   
    case TNM_ALARM_CODE_ETHCARD_RESTORE:       //10012       ��̫���ָ��澯
        {
            return ALARM_GENERAL;
        }   
    
    default:
        break;
    }
    return ALARM_CRITICAL;
}


/*========================================================================
�� �� ����GetAlarmLevelDesc
��    �ܣ����Խӿڣ��õ��澯�������������
��    ����u32 dwLevel                   [in]�澯����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/30  3.9     ���ǿ  ����
========================================================================*/
inline s8* GetAlarmLevelDesc(u32 dwLevel)
{
    switch(dwLevel) 
    {
    case ALARM_NORMAL:      return "ALARM_NORMAL";
    case ALARM_IMPORTANT:   return "ALARM_IMPORTANT";
    case ALARM_GENERAL:     return "ALARM_GENERAL";
    case ALARM_CRITICAL:    return "ALARM_CRITICAL";
    default:
        break;
    }
    return "ALARM_NORMAL";
}

/*========================================================================
�� �� ����GetSubevDesc
��    �ܣ����Խӿڣ��õ���Ϣ�����͵�����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/30  3.9     ���ǿ  ����
========================================================================*/
inline s8* GetSubevDesc(u8 bySubev)
{
    switch(bySubev) 
    {
    case SUBEV_TYPE_REQ:        return "REQ";
    case SUBEV_TYPE_ACK:        return "ACK";
    case SUBEV_TYPE_NACK:       return "NACK";
    case SUBEV_TYPE_NOTIFY:     return "NOTIFY";
    case SUBEV_TYPE_FINISH:     return "FINISH";
    case SUBEV_TYPE_INVALID:    return "INVALID";
    default:
        break;
    }
    return "INVALID";
}

/*========================================================================
�� �� ����GetActTypeDesc
��    �ܣ����Խӿڣ� �õ���������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/13  3.9     ���ǿ  ����
========================================================================*/
inline s8* GetActTypeDesc(s32 nActIdx)
{
    switch(nActIdx) 
    {
    case ACT_TYPE_LOGON_IDX:            return "LOGON";
    case ACT_TYPE_LOGOFF_IDX:           return "LOGOFF";
    case ACT_TYPE_ADD_GROUP:            return "ADD_GROUP";
    case ACT_TYPE_MODIFY_GROUP:         return "MODIFY_GROUP";
    case ACT_TYPE_DEL_GROUP:            return "DEL_GROUP";
    case ACT_TYPE_ADD_USER:             return "ADD_USER";
    case ACT_TYPE_MODIFY_USER:          return "MODIFY_USER";
    case ACT_TYPE_DEL_USER:             return "DEL_USER";
    case ACT_TYPE_MODIFY_ACCOUNT:       return "MODIFY_ACCOUNT";
    case ACT_TYPE_SAVE_PREFER:          return "SAVE_PREFER";
    case ACT_TYPE_EXPORT_PREFER:        return "EXPORT_PREFER";
    case ACT_TYPE_IMPORT_PREFER:        return "IMPORT_PREFER";
    case ACT_TYPE_DEFAULT_VALUE:        return "DEFAULT_VALUE";
    case ACT_TYPE_GET_DEV_CFG:          return "GET_DEV_CFG";
    case ACT_TYPE_SET_DEV_CFG:          return "SET_DEV_CFG";
    case ACT_TYPE_REBOOT:               return "REBOOT";
    case ACT_TYPE_PING:                 return "PING";
    case ACT_TYPE_SELFTEST:             return "SELFTEST";
    case ACT_TYPE_SOFTWARE_UPDATE:      return "SOFTWARE_UPDATE";
    case ACT_TYPE_ALARM_SETTING:        return "ALARM_SETTING";
    case ACT_TYPE_ALARM_ANALYSE:        return "ALARM_ANALYSE";
    case ACT_TYPE_ALARM_QUERY:          return "ALARM_QUERY";
    case ACT_TYPE_DEL_ALARM:            return "DEL_ALARM";
    case ACT_TYPE_EXPORT_ALARM:         return "EXPORT_ALARM";
    case ACT_TYPE_RELEASE_ALARM:        return "RELEASE_ALARM";
    case ACT_TYPE_GET_DEV_PFM:          return "GET_DEV_PFM";
    case ACT_TYPE_PFM_ANALYSE:          return "PFM_ANALYSE";
    case ACT_TYPE_SET_IP_RULES:         return "SET_IP_RULES";
    case ACT_TYPE_SET_LOGIN_LOCK_TIME:  return "SET_LOGIN_LOCK_TIME";
    case ACT_TYPE_MANUAL_BACKUP_DB:     return "MANUAL_BACKUP_DB";
    case ACT_TYPE_RECOVER_DB:           return "RECOVER_DB";
    case ACT_TYPE_QUERY_ACT_LOG:        return "QUERY_ACT_LOG";
    case ACT_TYPE_EXPORT_ACT_LOG:       return "EXPORT_ACT_LOG";
    case ACT_TYPE_DEL_ACT_LOG:          return "DEL_ACT_LOG";
    case ACT_TYPE_LOCK_NMCUI:           return "LOCK_NMCUI";
    case ACT_TYPE_UNLOCK_NMCUI:         return "UNLOCK_NMCUI";
    case ACT_TYPE_SET_NMS_CFG:          return "SET_NMS_CFG";
    case ACT_TYPE_VIEW_DEV_PIC:         return "VIEW_DEV_PIC";
    default:
        break;
    }
    return NULL;
}

/*========================================================================
�� �� ����PrintDevRight
��    �ܣ���ӡ�豸Ȩ�޵���Ϣ
��    ����TTnmMask const *ptDevRight        [in]�豸Ȩ��
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/19  3.9     ���ǿ  ����
========================================================================*/
inline void PrintDevRight(TTnmMask const *ptDevRight)
{
    OspPrintf(TRUE, FALSE, "\tMask=0x%x\n", ptDevRight->GetMaskInfo());

    if (ptDevRight->IsItemValid(DEV_RIGHT_VIEW_CFG)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_VIEW_CFG\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_VIEW_PFM)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_VIEW_PFM\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_MODIFY_CFG)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_MODIFY_CFG\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_REBOOT)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_REBOOT\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_VIEW_CFG)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_PING\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_VIEW_LOG)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_VIEW_LOG\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_SELFTEST)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_SELFTEST\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_SOFTWARE_UPDATE)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_SOFTWARE_UPDATE\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_OWNER)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_OWNER\n");
    }
}

/*========================================================================
�� �� ����GetCurTime
��    �ܣ��õ���ǰ��ʱ��ֵ
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/05/19  3.9     ���ǿ  ����
========================================================================*/
inline u32 GetCurTime()
{
    u32 dwCurTime = 0;
#ifdef WIN32

    CTime tCurTime = CTime::GetCurrentTime();
    dwCurTime = (u32)tCurTime.GetTime();
#else
    dwCurTime = time(NULL);
#endif // WIN32

    return dwCurTime;
}

//���Ź�ע����Ϣ�ṹ
typedef struct tagMsgInfo
{
    u16 wMsgEventId;        //��Ϣֵ
    u8 byMsgSubEvent;       //����Ϣֵ
    u8 byApp;               //������Ϣ��App
}TMsgInfo;

//���TFieldInfo�ṹ
typedef struct tagTFieldInfo
{
        tagTFieldInfo()
        {
            memset(achAlias, 0, sizeof(achAlias));
            tFieldId.SetNull();
            tParentField.SetNull();
        }   
        TTnmDeviceId    tFieldId;           //��Id (��Ӧƽ̨���豸Id)
        TTnmDeviceId    tParentField;   //�ϼ���Id (��Ӧƽ̨���豸Id)
        s8              achAlias[MAX_DEVICE_ALIAS_LEN + 4]; // ����
        void Print(BOOL32 bNetToHost = FALSE);
        void NetToHost(){};
        void HostToNet(){};
        void Reset();
}PACKED TFieldInfo;

inline void TFieldInfo::Print(BOOL32 bNetToHost)
{
    OspPrintf(TRUE, FALSE, "\tFId=%s; Alias=%s;\n", tFieldId.achID, achAlias);
    OspPrintf(TRUE, FALSE, "\tPId=%s;\n", tParentField.achID);
}

inline void TFieldInfo::Reset()
{
    memset(achAlias, 0, sizeof(achAlias));
    tFieldId.SetNull();
    tParentField.SetNull();
}


//��¼д�澯��Ϣ�����ݿ�ʧ�ܵ����ݽṹ  (���ܷ������ڲ�ʹ�����ֽ�������)
typedef struct tagAlarmDrop
{
    tagAlarmDrop()
    {
        m_dwAlarmCode = 0;
        m_byState = 0;
        m_tRecTime = 0;
        m_byReserved1 = 0;
        m_byReserved2 = 0;
        m_byReserved3 = 0;
    }
    u32 m_dwAlarmCode;
    u32 m_dwDevId;                              // �豸����ˮ�ţ����ڱȽ�
    time_t m_tRecTime;
    u8  m_byState;
    u8  m_byReserved1;
    u8  m_byReserved2;
    u8  m_byReserved3;

    //��ӡ����
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "\tAlarmCode=%d; DevSn=%d; AlarmState=%d;\n", m_dwAlarmCode, m_dwDevId, m_byState);
        struct tm *ptmtime = localtime(&m_tRecTime);
        if (!ptmtime)
        {
            OspPrintf(TRUE, FALSE, "\tRecTime: 1970.00.00 08:00:00\n");
            return;
        }
        OspPrintf(TRUE, FALSE, "\tRecTime: %s", asctime(ptmtime));
    }
}PACKED TAlarmDroped;

//���ܷ��������Ź�����
typedef struct tagNmsDogCfg
{
    tagNmsDogCfg()
    {
        m_dwTimeInterval = 0;
        m_wStaveCount = 0;
        m_byEnable = 0;
    }
    u32 m_dwTimeInterval;
    u16 m_wStaveCount;
    u8 m_byEnable;
    u8 m_byNetType;
}PACKED TNmsDogCfg;

//�豸�����ݽṹ
typedef struct tagDevGroup
{
    tagDevGroup()
    {
        dwDevGroupId = 0;
        dwUserId = 0;
        memset(achPDevId, 0, sizeof(achPDevId));
        memset(achGroupName, 0, sizeof(achGroupName));
    }
    u32 dwDevGroupId;
    u32 dwUserId;
    s8 achPDevId[TNM_MAX_DEVICE_ID_LEN+4];
    s8 achGroupName[MAX_DEVICE_ALIAS_LEN+4];

    void Print(BOOL32 bNetToHost = FALSE);
    void NetToHost();
    void HostToNet();
}PACKED TDevGroup;

inline void TDevGroup::Print(BOOL32 bNetToHost)
{
    if(bNetToHost)
    {
        OspPrintf(TRUE, FALSE, "\tDGroupID=%d; PDevID=%s;\n", ntohl(dwDevGroupId), achPDevId);
        OspPrintf(TRUE, FALSE, "\tUserId = %d; DGroupName=%s;\n", ntohl(dwUserId), achGroupName);
    }
    OspPrintf(TRUE, FALSE, "\tDGroupID=%d; PDevID=%s;\n", dwDevGroupId, achPDevId);
    OspPrintf(TRUE, FALSE, "\tUserId = %d; DGroupName=%s;\n", dwUserId, achGroupName);
}

inline void TDevGroup::NetToHost()
{
    dwDevGroupId = ntohl(dwDevGroupId);
    dwUserId = ntohl(dwUserId);
}

inline void TDevGroup::HostToNet()
{
    dwDevGroupId = htonl(dwDevGroupId);
    dwUserId = htonl(dwUserId);
}

typedef struct tagDevGroupMem
{
    tagDevGroupMem()
    {
        dwItemSn = 0;
        dwGroupId = 0;
        dwUserId = 0;
        memset(achDevId, 0, sizeof(achDevId));
    }
    u32 dwItemSn;
    u32 dwGroupId;
    u32 dwUserId;
    s8 achDevId[TNM_MAX_DEVICE_ID_LEN+4];

    void Print(BOOL32 bNetToHost);
    void NetToHost();
    void HostToNet();
}PACKED TDevGroupMem;

inline void TDevGroupMem::Print(BOOL32 bNetToHost)
{
    if (bNetToHost)
    {
        OspPrintf(TRUE, FALSE, "\tItemSn=%d;GroupId=%d; UserId=%d; DevId=%s;\n",ntohl(dwItemSn), ntohl(dwGroupId), ntohl(dwUserId), achDevId);
    }
    OspPrintf(TRUE, FALSE, "\tItemSn=%d;GroupId=%d; UserId=%d; DevId=%s;\n",dwItemSn, dwGroupId, dwUserId, achDevId);
}

inline void TDevGroupMem::NetToHost()
{
    dwItemSn = ntohl(dwItemSn);
    dwGroupId = ntohl(dwGroupId);
    dwUserId = ntohl(dwUserId);
}

inline void TDevGroupMem::HostToNet()
{
    dwItemSn = htonl(dwItemSn);
    dwGroupId = htonl(dwGroupId);
    dwUserId = htonl(dwUserId);
}

typedef struct tagDevNmcTopo : public TTnmDevTopo
{   
    tagDevNmcTopo() : tagTnmDevTopo()
    {
        tUnitType.byDeviceType = TNM_UNIT_UNKNOW;
        tUnitType.byServiceType = TNM_SERVICE_UNKNOW;
        dwDevSerialNo = INVALID_SERIAL_NO;
        dwMasterBoardSn = INVALID_SERIAL_NO;
        memset(achAlias, 0, sizeof(achAlias));
        dwIpAddr = 0;
        byField = INVALID_VALUE;
        byRightRange = RIGHT_RANGE_ALL_CHILD;
        byOnline = TNM_STATE_OFFLINE;
        byLayer = 0;
        bySlot = 0;
        byUpgradeMode = TNM_UPGRADE_MODE_NONE;
        byIsOldDevice = TNM_DISABLE;
        memset(achDevTypeName, 0, sizeof(achDevTypeName));
        memset(achBoxID, 0, sizeof(achBoxID));
    }
    TTnmUnitType tUnitType;                          // ��Ԫ����
    u32 dwDevSerialNo;                      // �豸��ˮ��.
    u32 dwMasterBoardSn;                    // ����������豸��ˮ��
    s8  achAlias[MAX_DEVICE_ALIAS_LEN + 4]; // �豸����     
    u32 dwIpAddr;                           // �豸��Ip��ַ
    u8  byField;                            // byField����Ľڵ��Ƿ�����
    u8  byRightRange;                       // Ȩ�޵����ͣ����ֶζԵ�����NMC�ͻ��������壩
                                                        // RIGHT_RANGE_ALL_CHILD
                                                        // RIGHT_RANGE_NO_CHILD
                                                        // RIGHT_RANGE_PART_CHILD
    u8  byOnline;                           // ��ǰ�豸״̬���Ƿ�����
    u8  byHighestAlarmLevel;                // ��߸澯����
    u8  bySelfAlarmLevel;                   // ���豸����߸澯����

    u8  byLayer;
    u8  bySlot; 
    u8  byUpgradeMode;                                  //TNM_UPGRADE_MODE_TCP/TNM_UPGRADE_MODE_FTP

    u8  byIsOldDevice;                                  //TNM_ENABLE/TNM_DISABLE
    u8  byReserved;
    u16 wReserved;

    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //�豸������
    s8  achBoxID[MAX_BOX_ID_LEN + 4];   //�����ʶ

    void  CopyDeviceInfo(const TDeviceInfo *ptDevInfo)
    {
        memcpy((TTnmDevTopo*)this, (TTnmDevTopo*)ptDevInfo, sizeof(TTnmDevTopo));
        strcpy(achAlias, ptDevInfo->achAlias);

        tUnitType.byDeviceType = ptDevInfo->tUnitType.byDeviceType;
        tUnitType.byServiceType = ptDevInfo->tUnitType.byServiceType;

        dwDevSerialNo = ptDevInfo->dwDevSerialNo;
        dwMasterBoardSn = ptDevInfo->dwMasterBoardSn;
        dwIpAddr = ptDevInfo->dwIpAddr;
        byField = ptDevInfo->byField;
        byRightRange = ptDevInfo->byRightRange;
        byOnline = ptDevInfo->byOnline;
        byHighestAlarmLevel = ptDevInfo->byHighestAlarmLevel;
        bySelfAlarmLevel = ptDevInfo->bySelfAlarmLevel;
        byLayer = ptDevInfo->byLayer;
        bySlot = ptDevInfo->bySlot;
        byUpgradeMode = ptDevInfo->byUpgradeMode;
        byIsOldDevice = ptDevInfo->byIsOldDevice;

        strcpy(achDevTypeName, ptDevInfo->achDevTypeName);
        strcpy(achBoxID, ptDevInfo->achBoxID);
    }
    void  GiveDeviceInfo(TDeviceInfo *ptDevInfo)
    {
        memcpy((TTnmDevTopo*)ptDevInfo, (TTnmDevTopo*)this, sizeof(TTnmDevTopo));
        strcpy(ptDevInfo->achAlias, achAlias);

        ptDevInfo->tUnitType.byDeviceType = tUnitType.byDeviceType;
        ptDevInfo->tUnitType.byServiceType = tUnitType.byServiceType;

        ptDevInfo->dwDevSerialNo = dwDevSerialNo;
        ptDevInfo->dwMasterBoardSn = dwMasterBoardSn;
        ptDevInfo->dwIpAddr = dwIpAddr;
        ptDevInfo->byField = byField;
        ptDevInfo->byRightRange = byRightRange;
        ptDevInfo->byOnline = byOnline;
        ptDevInfo->byHighestAlarmLevel = byHighestAlarmLevel;
        ptDevInfo->bySelfAlarmLevel = bySelfAlarmLevel;
        ptDevInfo->byLayer = byLayer;
        ptDevInfo->bySlot = bySlot;
        ptDevInfo->byUpgradeMode = byUpgradeMode;
        ptDevInfo->byIsOldDevice = byIsOldDevice;

        strcpy(ptDevInfo->achDevTypeName, achDevTypeName);
        strcpy(ptDevInfo->achBoxID, achBoxID);
    }

    void NetToHost()
    {
        TTnmDevTopo::NetToHost();

        dwDevSerialNo = ntohl(dwDevSerialNo);
        dwMasterBoardSn = ntohl(dwMasterBoardSn);
        dwIpAddr = ntohl(dwIpAddr);
    }
    void HostToNet()
    {
        TTnmDevTopo::HostToNet();

        dwDevSerialNo = htonl(dwDevSerialNo);
        dwMasterBoardSn = htonl(dwMasterBoardSn);
        dwIpAddr = htonl(dwIpAddr);
    }

}PACKED TDevNmcTopo;

#endif  //_EVNMSNMC_20050907_H_



