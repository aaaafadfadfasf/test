#ifndef _FAKE_CU_H_
#define _FAKE_CU_H_
#include "cms/ospext/ospext.h"

enum cusdk__EPlayMode
{
    cusdk__VideoAudioBoth,
    cusdk__VideoOnly,
    cusdk__AudioOnly
};

enum cusdk__ETransMode
{
    cusdk__UDP,
    cusdk__TCP
};

enum cusdk__EUserType
{
    //cusdk__UserTypeInvalid,
    cusdk__SuperAdmin,
    cusdk__Admin,
    cusdk__Operator,
};

enum cusdk__EUserPri
{
    cusdk__PriUserInvalid,              //��Ч���û�Ȩ��
    cusdk__PriUserCMSRecStart,			// ƽ̨¼��.
    cusdk__PriUserCMSRecPlay,			// ƽ̨¼�񲥷�.
    cusdk__PriUserCMSRecDel,			// ƽ̨¼��ɾ��.
    cusdk__PriUserCMSRecDownload,		// ƽ̨¼������.
    cusdk__PriUserLocalRec,				// ����¼��.

    cusdk__PriUserCMSRecPlayOtherDomain,// ����¼�񲥷�.
    cusdk__PriUserPUCall,				// ǰ�˺���.		ԭem_Cms_Call_Ctrl
    cusdk__PriUserAlarmLog,				// �澯��־����.	ԭem_Cms_Alarm_Manage
};

enum cusdk__EDevicePri
{
    cusdk__PriPUInvalid,        //��Ч���豸Ȩ��
    cusdk__PriPURecStart,		// ��ʼǰ��¼��.
    cusdk__PriPURecPlay,		// ����ǰ��¼��
    cusdk__PriPURecDel,		    // ɾ��ǰ��¼��.
    cusdk__PriPURecDownload,	// ǰ��¼������.

    cusdk__PriPUConfig,	        // ǰ�˲�������. �����ƵԴ�Ĳ�����ô����??
    cusdk__PriPUAlarmOutput,    // ǰ�˿������������.
    cusdk__PriPUAlarmConfig,    // ��������.	
};

enum cusdk__EDeviceType
{
    cusdk__DevTypeUnknown,      // �Ƿ��豸����
    cusdk__DevTypeEncoder,      // �����豸������PUS��ͨ���Ƿ����豸�洢������ʶ
    cusdk__DevTypeDecoder,      // �����豸
    cusdk__DevTypeCodecer,      // ������豸
    cusdk__DevTypeTVWall,       // ����ǽ�豸
    //����B2�汾����
    cusdk__DevTypeNVR,
    cusdk__DevTypeSVR,
    cusdk__DevAlarmHost,
};

enum cusdk__EPuSubscribeType
{
    cusdk__SubscribeOnlineStatus,		// ������.
    cusdk__SubscribeConfigStatus,		// ����.
    cusdk__SubscribeServiceStatus,		// ¼��/����/����/��Ѳ �ȷ����״̬
    cusdk__SubscribeAlarmStatus,		// �澯.
};

enum cusdk__EPtzCmd
{
    cusdk__PtzInvalidCmd,           //��Чָ��
    cusdk__PtzMoveUp,			    // 1	//�����ƶ�
    cusdk__PtzMoveDown,	    	    // 2	//�����ƶ�
    cusdk__PtzMoveLeft,			    // 3	//�����ƶ�
    cusdk__PtzMoveRight,			// 4	//�����ƶ�
    cusdk__PtzMoveHome,			    // 5	//�ع�
    cusdk__PtzMoveStop,			    // 6	//ֹͣ�ƶ�
    cusdk__PtzZoomIn,				// 7	//��������ͷ
    cusdk__PtzZoomOut,				// 8	//��Զ����ͷ
    cusdk__PtzZoomStop,			    // 9	//��Ұ����ֹͣ
    cusdk__PtzFocusFar,		        // 10	//�������Զ
    cusdk__PtzFocusNear,			// 11	//���������
    cusdk__PtzFocusAuto,			// 12	//�Զ�����
    cusdk__PtzFocusStop,			// 13	//����ֹͣ
    cusdk__PtzPresetSave,	    	// 14	//����ͷԤ��
    cusdk__PtzPresetCall,			// 15	//������ͷԤ��
    cusdk__PtzCameraReset,		    // 16	// ��ʼ������ͷ
    cusdk__PtzBrightUp,			    // 17	//�������
    cusdk__PtzBrightDown,			// 18	//�������
    cusdk__PtzBrightStop,			// 19	//ֹͣ����
    cusdk__PtzCommandNew,			// 20	//������	  ������??	 1.0��û���õ�.
    cusdk__PtzCommandAppend,		// 21	//�������� ������?? 1.0��û���õ�.
    cusdk__PtzGotoPoint,			// 22	//���Ķ�λ
    cusdk__PtzZoomArea,			    // 23	//�ֲ��Ŵ�

    cusdk__PtzAlecPanSpeed,	        // 24	//������ˮƽ�ƶ��ٶ�����������ڲ�ʹ�ã� û�õ�??
    cusdk__PtzAlecTiltSpeed,	    // 25	//�����˴�ֱ�ƶ��ٶ�����������ڲ�ʹ�ã� û�õ�??
    cusdk__PtzSonySetBright,	    // 26	//sony���ȵ�����������ڲ�ʹ�ã�		û�õ�??

    cusdk__PtzMatrixSwitch,		    // 27	//�л���Ƶ
    cusdk__PtzMatrixVideoMix4,		// 28	//����ϳ�2��2
    cusdk__PtzMatrixVideoMix9,		// 29	//����ϳ�3��3
    cusdk__PtzMatrixVideoMix16,	    // 30	//����ϳ�4��4
    cusdk__PtzMatrixVideoMixPP,	    // 31	//����ϳɻ��л�

    cusdk__PtzZoomAreaStop,		    // 32    // ȡ���ֲ��Ŵ� 

    ////�˵�//////////////////////////////////////////////////////////////////////	
    cusdk__PtzMenuUp,				// 33	//�˵���
    cusdk__PtzMenuDown,			    // 34	//�˵���
    cusdk__PtzMenuLeft,			    // 35	//�˵���
    cusdk__PtzMenuRight,			// 36	//�˵���

    cusdk__PtzMenuShow,			    // 37	//�˵���ʾ
    cusdk__PtzMenuHide,			    // 38	//�˵�����

    cusdk__PtzMenuEnter,			// 39	//�˵�ȷ��
    cusdk__PtzMenuCancel,		    // 40	//�˵�ȡ��

    cusdk__PtzPresetDel,			// 41	//ɾ������ͷԤ�� (û�õ�?)
    ////�����л�//////////////////////////////////////////////////////////////////////
    cusdk__PtzMatrixSwitchASC,	    // 42	//�����л�
    cusdk__PtzMatrixSwitchESC,	    // 43	//�����л�
    cusdk__PtzMatrixHoldImage,	    // 44	//ͼ�񱣳�

    cusdk__PtzMoveLeftUp,			//(u8)45		//�������ƶ�
    cusdk__PtzMoveLeftDown,	        //(u8)46		//�������ƶ�
    cusdk__PtzMoveRightUp,		    //(u8)47		//�������ƶ�
    cusdk__PtzMoveRightDown,		//(u8)48		//�������ƶ�

    ////Ѳ��//////////////////////////////////////////////////////////////////////	
    cusdk__PtzAutoScanStart=128,    // 128	//��ʼ�Զ�Ѳ��
    cusdk__PtzAutoScanStop,         // 129	//ֹͣ�Զ�Ѳ��	
    cusdk__PtzPresetScanStart,		// 130	//��ʼѲ��(Ԥ��λ��Ѳ?)
    cusdk__PtzPresetScanStop,       // 131	//ֹͣѲ��(Ԥ��λ��Ѳ?)
    cusdk__PtzScanSpeedSet,			// (u8)132	//ˮƽɨ���ٶȣ�Ӱ���Զ�ɨ���ٶȣ�bySpeed(1-40s)	
};

enum cusdk__EUserOrUserGroup
{
    cusdk__TypeUser,
    cusdk__TypeUserGroup,
};

class cusdk__NetworkAddr
{
public:
    std::string ip;	/* required element of type xsd:string */
    int port;	/* required element of type xsd:int */
};

class cusdk__NatInfo
{
public:
    std::string natData;	/* required element of type xsd:string */
    cusdk__NetworkAddr localAddr;	/* required element of type cusdk:NetworkAddr */
    cusdk__NetworkAddr natAddr;	/* required element of type cusdk:NetworkAddr */
};

class cusdk__MediaNetInfo
{
public:
    cusdk__NatInfo videoRtpNatInfo;	/* required element of type cusdk:NatInfo */
    cusdk__NatInfo videoRtcpNatInfo;	/* required element of type cusdk:NatInfo */
    cusdk__NatInfo audioRtpNatInfo;	/* required element of type cusdk:NatInfo */
    cusdk__NatInfo audioRtcpNatInfo;	/* required element of type cusdk:NatInfo */
};



API void login();

API void logout();

API void SubscribeTvsStatus();

API void UnsubscribePuStatus();

API void GetNotify();

API void GetCUIList();

API void SetupStream();

API void PlayStream();

API void StopStream();

API void PtzCtrl();

API void PtzLock();

API void PtzUnlock();

API void SendTransData();

API void GetUserInfo();

API void GetUserGroupInfo();

API void GetUserData();

API void SetUserData();

API void GetUserDevGroupByGroup();

API void GetUserDevByGroup();

API void GetUserDevGroup();

API void GetDomainDevGroup();

API void GetUserDev();

API void GetUserDevByDomain();

API void GetDeviceGroupDeviceRelations();

API void CreateDeviceGroup();

API void DelDeviceGroup();

API void ModifyDeviceGroup();

API void DeviceGroupAddDevice();

API void DeviceGroupDelDevice();

API void GetUserList();

API void GetUserGroupList();

API void CreateUserGroup();

API void DelUserGroup();

API void ModifyUserGroup();

API void CreateUser();

API void DelUser();

API void ModifyUser();

API void UserGetDeviceByGroup();

API void UserAddDevice();

API void UserDelDevice();

API void UserModifyDevice();

API void GetAuthorizationDevice();

API void AddAuthorizationDevice();

API void DelAuthorizationDevice();

API void ModifyAuthorizationDevice();

API void GetDeviceBaseInfo();

//API void GetRCSSysParam();

API void GetAlarmLinkConfig();

API void SetAlarmLinkConfig();

API void QueryAlarmLinkConfig();

API void TVWallDelScheme();

API void TVWallQueryScheme();

API void TVWallSaveScheme();

API void TVWallLoadScheme();

API void TVWallStartPlay();

API void TVWallStopPlay();

API void TVWallWindowPollContrl();

API void CallPu();

API void QueryMap();

API void CreateMap();

API void ModifyMap();

API void DelMap();

API void CreateMapElement();

API void ModifyMapElement();

API void DelMapElement();

API void QueryMapElement();

#endif

