
//gsoap cusdk service name:   cusdk
//gsoap cusdk service style:  document
//gsoap cusdk service encoding: literal
//gsoap cusdk service namespace: http://www.kedacom.com/cusdk.wsdl
//gsoap cusdk service location:  http://127.0.0.1

//gsoap cusdk schema namespace:	urn:cusdk

/**	cusdk webservice �ӿ�.
*	Created by lizhixing@20111117 
*/

/*************************************************
�����ṹ�嶨��
**************************************************/

#import "stlvector.h"
#import "stlset.h"
#import "kdtype.gsoap"

// 
// enum cusdk__EPlayMode
// {
// 	cusdk__PlayModeInvalid,		// ��Чֵ
// 	cusdk__VideoAudioBoth,
// 	cusdk__VideoOnly,
// 	cusdk__AudioOnly
// };

enum cusdk__ETransMode
{
	cusdk__TransModeInvalid,	// ��Чֵ.
	cusdk__TransModeUDP,
	cusdk__TransModeMRTC,
	//cusdk__TransModeTCP
};


enum cusdk__EUserType
{
	// cusdk__UserTypeInvalid, 3A�ж���0�ǳ�������Ա. ��������������һ��.
	cusdk__SuperAdmin,
	cusdk__Admin,
	cusdk__Operator,
    cusdk__SecurityAudit,           // ��ȫ���
    cusdk__SecurityManage,          // ��ȫ����
    cusdk__SecurityOperator,        // ��ȫ����
};

enum cusdk__EUserPri
{
    cusdk__PriUserInvalid,              // ��Ч���û�Ȩ��
	cusdk__PriUserCMSRecStart,			// ƽ̨¼��.
	cusdk__PriUserCMSRecPlay,			// ƽ̨¼�񲥷�.
	cusdk__PriUserCMSRecDel,			// ƽ̨¼��ɾ��.
	cusdk__PriUserCMSRecDownload,		// ƽ̨¼������.
	cusdk__PriUserLocalRec,				// ����¼��.

	cusdk__PriUserCMSRecPlayOtherDomain,// ����¼�񲥷�.
	cusdk__PriUserPUCall,				// ǰ�˺���.		ԭem_Cms_Call_Ctrl
	cusdk__PriUserAlarmLog,				// �澯��־����.	ԭem_Cms_Alarm_Manage
	cusdk__PriUserWaterMark,			// ˮӡȨ��
};

enum cusdk__EDevicePri
{
    cusdk__PriPUInvalid,        //��Ч���豸Ȩ��
    cusdk__PriPURecStart,		// ��ʼǰ��¼��.		�豸Ȩ��
	cusdk__PriPURecPlay,		// ����ǰ��¼��			�豸Ȩ��
	cusdk__PriPURecDel,		    // ɾ��ǰ��¼��.		�豸Ȩ��
	cusdk__PriPURecDownload,	// ǰ��¼������.		�豸Ȩ��

	cusdk__PriPUConfig,	        // ǰ�˲�������.		�豸Ȩ��
	cusdk__PriPUAlarmOutput,    // ǰ�˿������������.	�豸Ȩ��
	cusdk__PriPUAlarmConfig,    // ��������.			�豸Ȩ��
	cusdk__PriPuAudioCtrl,      // ��Ƶ����.			��ƵԴȨ��.
	cusdk__PriPuSavePreset,		// Ԥ��λ����Ȩ��.		��ƵԴȨ��.
	cusdk__PriPuPtzLock,		// ��̨����Ȩ��.		��ƵԴȨ��.
};

enum cusdk__EDeviceType
{
	cusdk__DevTypeUnknown,      // �Ƿ��豸����
	cusdk__DevTypeEncoder,      // �����豸������PUS��ͨ���Ƿ����豸�洢������ʶ
    cusdk__DevTypeDecoder,      // �����豸
	cusdk__DevTypeCodecer,      // ������豸
	cusdk__DevTypeTVWall,       // ����ǽ�豸
    //����B2�汾����
	//cusdk__DevTypeNVR,
	//cusdk__DevTypeSVR,
	//cusdk__DevAlarmHost,
};

// ֪ͨ���ĵ�����.
enum cusdk__ESubscribeNotifyType
{
	cusdk__SubscribeNotifyInvalid,		// ��Чֵ.
	cusdk__SubscribePuStatus,			// �豸״̬.
	cusdk__SubscribeTVWallStatus,		// ����ǽ״̬.
	cusdk__SubscribeNRUStatus,			// ¼���/���� ״̬.
	// cusdk__SubscribeAlarmLinkConfig,	// �澯����. ƽ̨û��֧�����..
	cusdk__SubscribeSeviceStatus,		// ƽ̨�ϵķ���״̬.

	cusdk__SubscribeDeviceGroupStat,			// �豸��ͳ����Ϣ(�ǵݹ�).
	cusdk__SubscribeDeviceGroupRecursiveStat,	// �豸��ͳ��(�ݹ��豸��)��Ϣ.

};

// �豸״̬���ĵ�������.
enum cusdk__EPuSubscribeType
{
	cusdk__PuSubscribeTypeInvalid,		// ��Чֵ.
	cusdk__PUSubscribeOnlineStatus,		// ������.
    cusdk__PUSubscribeConfigStatus,		// ����.
    cusdk__PUSubscribeServiceStatus,	// ¼��/����/����/��Ѳ �ȷ����״̬
	cusdk__PUSubscribeAlarmStatus,		// �澯.
	cusdk__PUSubscribeGPS,				// GPS��Ϣ����.
	cusdk__PUSubscribeTransdata,		// ǰ��͸������.
};

enum cusdk__EPtzCmd
{
    cusdk__PtzCmdInvalid,           //��Чָ��
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

	// ��ˢ
	cusdk__PtzStartWiper = 160,		//(u8)160	//��ʼ��ˢ����һ������Ϊ��ˢ�ٶȣ�ֵΪ0-15��������������
	cusdk__PtzStopWiper,			//(u8)161	//ֹͣ��ˢ

	// +by lizhixing@2014-11-04
	cusdk__PtzLightOn,            //(u8)162   //�����ƹ�
	cusdk__PtzLightOff,           //(u8)163   //�رյƹ�
	cusdk__PtzStartExt1,           //(u8)164   //��չ����һ�������ǿ�����ˮ��
	cusdk__PtzStopExt1,           //(u8)165   //�ر���ˮ��չ����һ
	cusdk__PtzStartExt2,          //(u8)166   //��չ���ܶ��������ǿ�����˪��
	cusdk__PtzStopExt2,			//(u8)167   //�رճ�˪��չ���ܶ�

	// +by maonanjing@2016-04-06
	cusdk_PtzLock,				//(u8)168	//�ƽ���Ȩ��
};


enum cusdk__EVideoFormat
{
	cusdk__VF_Invalid,	// ��Чֵ.
	cusdk__VF_SN4,
	cusdk__VF_MPEG4,
	cusdk__VF_H261,
	cusdk__VF_H263,
	cusdk__VF_H264,
	cusdk__VF_H265,
	cusdk__VF_SVAC,
	cusdk__VF_MJPEG,
};

enum cusdk__EAudioFormat
{
	cusdk__AF_Invalid,	// ��Чֵ.
	cusdk__AF_PCMA,
	cusdk__AF_PCMU,
	cusdk__AF_GSM,
	cusdk__AF_G729,
	cusdk__AF_ADPCM,
	cusdk__AF_G7221C,
	cusdk__AF_G722,
	cusdk__AF_AACLC,
	cusdk__AF_G711,
	cusdk__AF_G726,
	cusdk__AF_AAC,
};

enum cusdk__EVideoResolution
{
	cusdk__VS_Invalid,	// ��Чֵ.
	cusdk__VS_AUTO,
	cusdk__VS_QCIF,
	cusdk__VS_CIF,
	cusdk__VS_2CIF,
	cusdk__VS_4CIF,
	cusdk__VS_QQCIF,

	cusdk__VS_QVGA,
	cusdk__VS_VGA,

	cusdk__VS_720P,
	cusdk__VS_1080A,
	cusdk__VS_1080P,
	cusdk__VS_QXGA,

	cusdk__VS_960P,
};

enum cusdk__EUserOrUserGroup
{
	cusdk__TypeInvalid,		// ��Чֵ.
	cusdk__TypeUser,
	cusdk__TypeUserGroup,
};


enum cusdk__ERS_CoverPolicy
{
	cusdk__RS_COVERPOLICY_INVALID,			// ��Чֵ.
	cusdk__RS_COVERPOLICY_STOPRECORD,          /*ͨ���ռ���ͣ¼��*/
	cusdk__RS_COVERPOLICY_COVERALL,            /*ͨ���ռ�����������¼��*/
	cusdk__RS_COVERPOLICY_COVERNORMAL,         /*ͨ���ռ���������ͨ¼��*/
	cusdk__RS_COVERPOLICY_UNKNOWN,
};
// enum cusdk__ERS_PartFormat
// {
// 	cusdk__RS_PARTFORMAT_INVALID,	// ��Чֵ.
// 	cusdk__RS_PARTFORMAT_EXT3,
// 	cusdk__RS_PARTFORMAT_VBFS,
// 	cusdk__RS_PARTFORMAT_UNKNOWN,
// };

// ����״̬.
enum cusdk__ERS_DiskState
{
	cusdk__RS_DISKSTATE_INVALID,
	cusdk__RS_DISKSTATE_ONLINE,
	cusdk__RS_DISKSTATE_OFFLINE,
	cusdk__RS_DISKSTATE_UNKNOWN,
};


// ����״̬.
enum cusdk__ERS_PartState
{
	cusdk__RS_PARTSTATE_INVALID,		// ��Чֵ.
	cusdk__RS_PARTSTATE_OFFLINE,		// ����
	cusdk__RS_PARTSTATE_UNFORMATTED,	// û�и�ʽ��
	cusdk__RS_PARTSTATE_FORMATTED,		// ��ʽ����.û����.
	cusdk__RS_PARTSTATE_MOUNTED,		// �ѹ���, ����״̬.
	cusdk__RS_PARTSTATE_UMOUNTEDD,		// δ����.
	cusdk__RS_PARTSTATE_UNKNOWN,		// δ֪״̬.
};

enum cusdk__ERecordType
{
	cusdk__ERecordTypeInvalid,
	cusdk__ERecordTypePlatformRecord,	// ƽ̨¼��ط�.
	cusdk__ERecordTypePURecord,			// ǰ��¼��ط�
};

enum cusdk__ERecordEventType
{
	cusdk__RS_RECTYPE_INVALID,		// ��Чֵ.
	cusdk__RS_RECTYPE_ALL,				// �������͵�¼��.
	cusdk__RS_RECTYPE_ALARM,            /*�澯¼��*/
	cusdk__RS_RECTYPE_HANDLE,               /*�ֶ�¼��*/
	cusdk__RS_RECTYPE_PLAN,                 /*�ƻ�¼�񣬰�����ʱ¼�������¼��*/
	cusdk__RS_RECTYPE_UNKNOWN,              
};

enum cusdk__ERS_RecMode
{
	cusdk__RS_RECMODE_INVALID,		// ��Чֵ.
	cusdk__RS_RECMODE_ALL,			// ������ͨ¼��.
	cusdk__RS_RECMODE_ONLY_VIDEO,		// ֻ¼��Ƶ.
	cusdk__RS_RECMODE_ONLY_KEYFRAME,	// ֻ¼�ؼ�֡.
};


enum cusdk__EVCRCmd
{
	cusdk__VCRCMD_INVALID,		// ��Чֵ.
	cusdk__VCRCMD_CONFIG,		// ����.
	cusdk__VCRCMD_START,        /*��ʼ*/
	cusdk__VCRCMD_STOP,         /*ֹͣ*/ 
	cusdk__VCRCMD_PAUSE,        /*��ͣ*/
	cusdk__VCRCMD_RESUME,       /*�ָ�*/
	cusdk__VCRCMD_SEEK,         /*ʱ�䶨λ*/
	cusdk__VCRCMD_FASTPLAY,     /*��� ������*/		
	cusdk__VCRCMD_SLOWPLAY,     /*���� ������ */
	cusdk__VCRCMD_ONEFRAME,     /*��֡����*/
	cusdk__VCRCMD_KEYFRAME,     /*ֻ�Źؼ�֡*/
	cusdk__VCRCMD_REVERSEPLAY,	/*����*/
	cusdk__VCRCMD_FORWARDPLAY,	/*����*/
	cusdk__VCRCMD_KEYSEEK,		/*��λ���ؼ�֡. */
	cusdk__VCRCMD_AUTO_KEYFRAME,          /*�Զ�ģʽ���Ƿ��֡��NRUȷ�����ͱ���ȫ֡���߱���ֻ�Źؼ�֡*/
	cusdk__VCRCMD_FORCE_KEYFRAME,         /*ǿ��ֻ�Źؼ�֡*/
	cusdk__VCRCMD_FORCE_ALLFRAME,         /*ǿ�ƻط�ȫ֡*/
	cusdk__VCRCMD_UNKNOWN,          
};

enum cusdk__EDeviceChannelType
{
	cusdk__DeviceChannelTypeInvalid,	// ��Чֵ.
	cusdk__DevChnTypeVideoSource,	// ��ƵԴ.
	cusdk__DevChnTypeAlarmInput,	// �澯����.

};

// �豸������ǩ.
enum cusdk__EDeviceCapLabel
{
	cusdk__DEVICE_CAP_INVALID = 0,				// ��Чֵ.
	cusdk__DEVICE_CAP_LOCAL_STORAGE = 2,			 // �豸������ǩ-�洢,����ǰ��¼��.
	cusdk__DEVICE_CAP_LABEL_ENCODE_NVR = 7,          // �豸������ǩ-NVR
	cusdk__DEVICE_CAP_LABEL_ENCODE_SVR = 8,          // �豸������ǩ-SVR
	cusdk__DEVICE_CAP_LABEL_ENCODE_IPC_BULLET = 9,   // �豸������ǩ-IPCǹ��
	cusdk__DEVICE_CAP_LABEL_ENCODE_IPC_DOME = 10,    // �豸������ǩ-IPC���
	cusdk__DEVICE_CAP_LABEL_ENCODE_GENERAL = 11,     // �豸������ǩ-��ͨ������
	cusdk__DEVICE_CAP_LABEL_ENCODE_WIRELESS = 12,    // �豸������ǩ-���߱�����
	cusdk__DEVICE_CAP_LABEL_GBDEVICE = 14,           // �豸������ǩ-�����豸
	cusdk__DEVICE_CAP_LABEL_CONFDEVICE = 15,        // �豸������ǩ-�����ն�
	cusdk__DEVICE_CAP_LABEL_GBRAWSTREAM = 16,       // �豸������ǩ-����ԭʼ����
};

// �豸�����ߵ�����.
enum cusdk__EDeviceCreatorType
{
	cusdk__DevCreatorTypePlat = 0,					// ��ƽ̨�ڲ�����
	cusdk__DevCreatorTypeG300,						// ��G300����
	cusdk__DevCreatorTypeGBS,						// ��gbs������ֻ�ޱ����豸
	cusdk__DevCreatorTypeGBSForeignDomain,			// ��gbs������ֻ�������豸
};

enum cusdk__ERecordStatus
{
	cusdk__RecordStatusInvalid,	// ��Чֵ.
	cusdk__RecordStatusIdel,	// ��ǰû��¼��.
	cusdk__RecordStatusOk,		// ��ǰ��������¼��.
	cusdk__RecordStatusTry,		// ��ǰ���ڳ��Կ���¼��.
	cusdk__RecordStatusStopping,// ��ǰ����ֹͣ¼��.
};

enum cusdk__EAlarmType
{
	cusdk__AlarmTypeInvalid,	// ��Ч�ĸ澯����.
	cusdk__AlarmMove,			// �ƶ����
	cusdk__AlarmInput,	// �澯����.
	cusdk__AlarmDiskfull,		// ������.
	cusdk__AlarmVideoLost,		// ��ƵԴ��ʧ.
	cusdk__AlarmIntelligent,	// ���ܸ澯.
	cusdk__AlarmVideo,			// ������Ѳ�澯��

	// �������µĸ澯,ֻ�� AlarmTypeV2 �д���.
	cusdk__AlarmExceptionEvent,		//�쳣�¼���Ϣ
	cusdk__AlarmDefocus,				//�齹���
	cusdk__AlarmSceneChange,			//�����任
	cusdk__AlarmRegionInvasion,		//��������
	cusdk__AlarmRegionLeaving,			//�����뿪
	cusdk__AlarmObjectTaken,			//��Ʒ��ȡ
	cusdk__AlarmObjectLeft,			//��Ʒ����
	cusdk__AlarmPeopleGather,			//��Ա�ۼ�
	cusdk__AlarmCorgon,				//������
	cusdk__AlarmAudioAbnormal,			//�����쳣
	cusdk__AlarmLumaChange,			//���ȱ仯
	//R4B1֧�ֹ���2016�������� �����
	cusdk__AlarmAntiDismantle,			//�豸���𱨾�
	cusdk__AlarmHighTemperature,		//�豸���±���
	cusdk__AlarmLowTemperature,		//�豸���±���
	cusdk__AlarmPersonVideo,           //�˹���Ƶ
	cusdk__AlarmSportGoalCheck,        //�˶�Ŀ����
	cusdk__AlarmTripwireCheck,         //���߼��
	cusdk__AlarmHoverCheck,             //  �ǻ���ⱨ��
	cusdk__AlarmTrafficStatisticsCheck, //  ����ͳ�Ʊ���
	cusdk__AlarmDensityCheck,           //�ܶȼ��
	cusdk__AlarmVideoException,         //��Ƶ�쳣
	cusdk__AlarmMoveFast,               //�����ƶ�
	cusdk__AlarmRetrogradeCheck,        //���м��
	cusdk__AlarmDiskFault,              //�洢�豸���̹��ϱ���
	cusdk__AlarmFanFault,              // �洢�豸���ȹ��ϱ���
	cusdk__AlarmGPSOverSpeed,           //gps ����
	cusdk__AlarmSIMDataTrafficExceeded,        //sim ������������
	
};

enum cusdk__EAlarmStatus
{
	cusdk__AlarmStatusInvalid,	// ��Чֵ.
	cusdk__AlarmOccur,		// �澯����.
	cusdk__AlarmRestore,	// �澯�ָ�.
};

// ��Ѳ״̬
enum cusdk__EPollStatus
{
	cusdk__PollStatusInvalid,	// ��Чֵ.
	cusdk__PollStarted,	// ��Ѳ�Ѿ���ʼ.
	cusdk__PollStoped,	// ��Ѳ�Ѿ�ֹͣ.
	cusdk__PollPaused,	// ��Ѳ��ͣ.
};

// ����ǽ����/������ ����ϳ�ģʽ/���. 
enum cusdk__ETVDivStyle
{
	cusdk__TVDIV_Invalid = 0,	// ��Чֵ.
	cusdk__TVDIV_1 = 1,
	cusdk__TVDIV_2 = 2,
	cusdk__TVDIV_3 = 3,
	cusdk__TVDIV_4 = 4,
	cusdk__TVDIV_6 = 6,
	cusdk__TVDIV_8 = 8,
	cusdk__TVDIV_9 = 9,
	cusdk__TVDIV_16 = 16,
	cusdk__TVDIV_6_EQUAL = 106,	// ������6�ȷֵ�֧�� +by lzx@2017-07-11
};


// ����ǽ�Ĳ���ģʽ.
enum cusdk__ETVWallPlayMode
{
	cusdk__TVWallPlayModeInvalid,	// ��Чֵ.
	cusdk__TVPlayModeLiveStream,
	cusdk__TVPlayModePlatformRecord,
	cusdk__TVPlayModePuRecord,
};

enum cusdk__ECameraProtocolType{
	cusdk__CAM_TYPE_Invalid = 0,
	cusdk__CAM_TYPE_SONY,	        //	SONY
	cusdk__CAM_TYPE_AD,			//	AD
	cusdk__CAM_TYPE_PELCO,			//	PELCO-D
	cusdk__CAM_TYPE_PIH,			//	����
	cusdk__CAM_TYPE_PELCO_P,		//	PELCO-P
	cusdk__CAM_TYPE_PARCO,			//	PARCO�ظ�
	cusdk__CAM_TYPE_AB,			//	AB
	cusdk__CAM_TYPE_YUNTAI,		//	������̨
	cusdk__CAM_TYPE_Samsung,		//	����
	cusdk__CAM_TYPE_YAAN,			//	�ǰ�
	cusdk__CAM_TYPE_AD168,			//	AD168����
	cusdk__CAM_TYPE_KALATEL,		//	������
	cusdk__CAM_TYPE_JN,			//	JNΰ��
	cusdk__CAM_TYPE_WV_CS650,		//	����,��850����Э��
	cusdk__CAM_TYPE_WV_CS850,		//	����,��850����Э��
	cusdk__CAM_TYPE_ALEC,		    //	������
	cusdk__CAM_TYPE_AB_P,          //	AB-P
	cusdk__CAM_TYPE_AB_D,          //	AB-D
	cusdk__CAM_TYPE_PELCO_D_JEC,   //	JEC PELCO-D
	cusdk__CAM_TYPE_PELCO_P_JEC,   //	JEC PELCO-P
	cusdk__CAM_TYPE_TIANDY,        //  ���ΰҵ
	cusdk__CAM_TYPE_G100,          //  G100
	cusdk__CAM_TYPE_368ZT,         //  368ת̨���
	cusdk__CAM_TYPE_GaoDeTI,       //  GaoDeTI������
	cusdk__CAM_TYPE_DaliTI,        //  DaliTI������
	cusdk__CAM_TYPE_2130_PELCO_P,  //  �Ǳ�׼PELCO_PЭ��
	cusdk__CAM_TYPE_K_PELCO_D,
	cusdk__CAM_TYPE_JVCTKC676,
	cusdk__CAM_TYPE_INFINOVA_PELCO_D,
	cusdk__CAM_TYPE_PEARMAIN_PELCO_D,
	cusdk__CAM_TYPE_PEARMAIN_PELCO_P,
	cusdk__CAM_TYPE_YAAN_PELCO_D,
	cusdk__CAM_TYPE_LANGKE,			// �ʿ�.
	cusdk__CAM_TYPE_CAMERA_NUM,

	// 	cusdk__CAM_TYPE_MATRIX=128,	//	����Э��
	// 	cusdk__MATRIX_TYPE_AB=128,		//  AB ����
	// 	cusdk__MATRIX_TYPE_ROBOT,			//  ROBOT ����ϳɾ���
	// 	cusdk__MATRIX_TYPE_SUNNHO,		//  SUNNHO	����
	// 	cusdk__MATRIX_TYPE_PEARMAIN,   //  ��ƻ��  ����
	// 	cusdk__MATRIX_TYPE_AD,         //  AD ����
	// 	cusdk__MATRIX_TYPE_INFORNOVA,
	// 	cusdk__MATRIX_TYPE_PECLO,      
	// 	cusdk__MATRIX_TYPE_HONEWELL,  //����Τ��
	// 	cusdk__MATRIX_TYPE_TIANDI,
	// 	cusdk__MATRIX_TYPE_HUATONG,//��ͨ����
	// 	cusdk__MATRIX_TYPE_TONGCHUANG,//ͬ������
	// 	cusdk__MATRIX_TYPE_BOCOM,     //��������
	// 	cusdk__MATRIX_TYPE_EXTRON,    //��˼��
	// 	cusdk__MATRIX_TYPE_ST_MS650,  //������
	// 	cusdk__MATRIX_TYPE_LIKEN,     //����
	// 	cusdk__MATRIX_TYPE_VINET,     //΢����
	// 	cusdk__MATRIX_TYPE_WEPUS,     //����ʿ
	// 	cusdk__MATRIX_TYPE_ADKEYBOARD, //AD���̿���
	// 	cusdk__MATRIX_TYPE_XINTAI,    //��̩����
	// 	cusdk__MATRIX_TYPE_JINCHAN,   //��Ӿ���
	// 	cusdk__CAM_TYPE_MATRIX_NUM
};

// ��ͼ������Ϣ. Jpeg �� Google ��ͼ.
// ��ͼ������Ϣ���ǵ�ͼ�ڳ�ʼ����ʱ����Ҫ��һЩ����. ���ֵ�ͼ��������.
// ��ͼ��Ϣ, Jpeg��ͼ�����û������µ�ͼ.
// Google��ͼ�Ĵ���, ֻ�����ȡ��ͼ��Ϣ.

enum cusdk__EMapType
{
	cusdk__MapTypeInvalid,	// ��Чֵ.
	cusdk__MapTypeGoogle,
	cusdk__MapTypeJpeg,
};

// ͼԪ��ɾ��
enum cusdk__EMapElementType
{
	cusdk__MapElementTypeInvalid,	// ��Чֵ.
	// ���ö���ڲ����ļ��ִ���..

	cusdk__MapEleCommon,	// ����???
	cusdk__MapEleMove,		// ????
// 
// 
// 	cusdk__MapEleMainMap,	// ??�Ƿ���Ҫ? ����ͼ.
// 	cusdk__MapEleSubMap,	// �ӵ�ͼ. ?�Ƿ���Ҫ??

};

enum cusdk__EMapCoordType
{
	cusdk__MapCoordTypeInvalid,	// ��Чֵ.
	cusdk__MapCoordEarth,	// ��������.
	cusdk__MapCoordMar,		// ��ƫ�Ļ�������
};




// enum cusdk__EUserOperateType
// {
// 	cusdk__UserOpTypeInvalid,	
// 	cusdk__UserOpTypeLogin,					// ��¼.1
// 	cusdk__UserOpTypeLogout,					// �ǳ�.2
// 	cusdk__UserOpTypeLiveStreamPlay,			// ���ʵʱ����.3
// 	cusdk__UserOpTypePlatformRecordStart,		// ��ʼ�ֶ�ƽ̨¼��.4
// 	cusdk__UserOpTypePlatformRecordStop,		// ֹͣ�ֶ�ƽ̨¼��.5
// 	cusdk__UserOpTypePlatformRecordPlay,		// ƽ̨¼�񲥷�.6
// 	cusdk__UserOpTypePlatformRecordDownload,	// ƽ̨¼������.7
// 	cusdk__UserOpTypePlatformRecordDelete,		// ƽ̨¼��ɾ��.8
// 	cusdk__UserOpTypePuRecordStart,		// ǰ���ֶ�¼��ʼ.9
// 	cusdk__UserOpTypePuRecordStop,		// ǰ���ֶ�¼��ֹͣ.10
// 	cusdk__UserOpTypePuRecordPlay,		// ǰ��¼�񲥷�.11
// 	cusdk__UserOpTypePuRecordDownload,	// ǰ��¼������.12
// 	cusdk__UserOpTypePuRecordDelete,	// ǰ��¼��ɾ��.13
// 
// 	cusdk__UserOpTypeTVWall,			// ����ǽ����,����.14
// 
// 	cusdk__UserOpTypeCall,				// ǰ�˺���.15
// 	cusdk__UserOpTypeHangUpCall,		// ǰ�˺���.16
// 
// 
// 	cusdk__UserOpTypeUserManage,			// �û�����17
// 	cusdk__UserOpTypeLogManage,				// ��־����18
// 	cusdk__UserOpTypeRecordManage,			// ¼���������.19
// 	cusdk__UserOpTypeMapConfig,				// ��ͼ����.20
// 	cusdk__UserOpTypePuConfig,				// ǰ������.21
// 	cusdk__UserOpTypePtzCtrl,				// ptz���� 22
// 	cusdk__UserOpTypeAlarmConfig,			// �澯���� 23
// 	cusdk__UserOpTypeSysParamConfig,		// ϵͳ�������� 24
// 
// 	
// 	cusdk__UserOpTypeOther = 25
// };

enum cusdk__EUserOperateType
{
	cusdk__UserOpTypeInvalid,	
	cusdk__UserOpTypeLogin,				// ��¼/�ǳ�. 1
	cusdk__UserOpTypeLiveStream,		// ���ʵʱ����. 2
	cusdk__UserOpTypePlatformRecord,	// ƽ̨¼�����. 3
	cusdk__UserOpTypePuRecord,			// ǰ��¼��	4
	cusdk__UserOpTypeTVWall,			// ����ǽ����,����.5
	cusdk__UserOpTypeCall,				// ǰ�˺���.6
	cusdk__UserOpTypeUserManage,		// �û�����7
	cusdk__UserOpTypeNRUManage,			// ¼���������. 8
	cusdk__UserOpTypeMapConfig,			// ��ͼ����. 9
	cusdk__UserOpTypePuConfig,			// ǰ������.10
	cusdk__UserOpTypePtzCtrl,			// ptz���� 11
	cusdk__UserOpTypeAlarmConfig,		// �澯���� 12
	cusdk__UserOpTypeSysParamConfig,	// ϵͳ�������� 13
	cusdk__UserOpTypeOther,				// ���� 14

	cusdk__UserOpTypeRecordStart,		// ����¼�� 15
	cusdk__UserOpTypeRecordStop,		// ֹͣ¼�� 16
	cusdk__UserOpTypeRecordPlayback,	// ¼��ط� 17
	cusdk__UserOpTypeRecordDownload,	// ¼������ 18
	cusdk__UserOpTypeCuLocalRecord,		// CU����¼�� 19

	// cusdk__UserOpTypeLiveStreamPlay,		// ʵʱ���(enUserOpTypeVideo Play) 20

};

enum cusdk__EVideoQuality
{
	cusdk__VideoQualityInvalid,		// ��Чֵ.
	cusdk__VIDEO_QUALITY_PRIOR = 1, //��������
	cusdk__VIDEO_SPEED_PRIOR = 2,	//�ٶ�����
};

enum cusdk__EDeviceLogType
{
	cusdk__DeviceLogTypeInvalid,	// ��Чֵ.
	cusdk__DeviceLogTypeOnline,		// ע������.
	cusdk__DeviceLogTypeOffline,	// ����.
	cusdk__DeviceLogTypePuException,		// �豸�쳣. 1.0������������ƽ̨�豸�쳣��ǰ���豸�쳣. 2.0��û��ƽ̨�豸��..
	cusdk__DeviceLogTypePlatformRecordException,	// ƽ̨¼���쳣.
	cusdk__DeviceLogTypePuRecordToPlatform,	// ƽ̨¼��ת��.
	cusdk__DeviceLogTypeOther,
};

enum cusdk__EAlarmLinkActionType
{
	cusdk__AlarmLinkActionTypeInvalid,		// ��Чֵ.
	cusdk__AlarmLinkActionPlatformRecord,	// ƽ̨¼��.
	cusdk__AlarmLinkActionPlayOnTV,		// ͼ���ϵ���ǽ.
	cusdk__AlarmLinkActionReadSecond,	// �澯����.
	cusdk__AlarmLinkActionIOOutput,		// �澯���
	cusdk__AlarmLinkCameraPrePos,		// �澯����ͷת��Ԥ��λ.
};

enum cusdk__EMediaType
{
	cusdk__MediaTypeInvalid,	// ��Чֵ.
	cusdk__MediaTypeVideo,	// ��Ƶ.
	cusdk__MediaTypeAudio,	// ��Ƶ.
	cusdk__MediaTypeData,	// ����.
};

enum cusdk__ETransChannelType
{
	cusdk__TransChannelTypeInvalid,	//��Чֵ.
	cusdk__TransChannelTypeRTP,
	cusdk__TransChannelTypeRTCP,
	cusdk__TransChannelTypeData,
};

enum cusdk__EDeviceEntity
{
	cusdk__VSIP_ENTITY_DEVICE					=0x00,
    cusdk__VSIP_ENTITY_VIDEODECODER             =0x01,
    cusdk__VSIP_ENTITY_VIDEOENCODER             =0x02,
    cusdk__VSIP_ENTITY_AUDIODECODER             =0x03,
    cusdk__VSIP_ENTITY_AUDIOENCODER             =0x04,
    cusdk__VSIP_ENTITY_SERIALPORT               =0x05,
    cusdk__VSIP_ENTITY_INPUTPIN                 =0x06,
    cusdk__VSIP_ENTITY_OUTPUTPIN                =0x07,
    cusdk__VSIP_ENTITY_PTZMOTOR                 =0x08,
    cusdk__VSIP_ENTITY_PTZCONTROL               =0x09,
    cusdk__VSIP_ENTITY_ARCHIVER                 =0x0A,
    cusdk__VSIP_ENTITY_VIDEOIN		            =0x0C,
    cusdk__VSIP_ENTITY_ETHERNET					=0x0D,  //0x01~0x0CΪ����������  Ox0D~0x2F����չ������
    cusdk__VSIP_ENTITY_WIRELESS		            =0x0E,
    cusdk__VSIP_ENTITY_XIANTELE					=0x0F,
    cusdk__VSIP_ENTITY_VIDIOADPTER	            =0x10,
    cusdk__VSIP_ENTITY_VIDIOCOMP	            =0x11,
    cusdk__VSIP_ENTITY_VIDEOSRCPOLL             =0x12,
    cusdk__VSIP_ENTITY_AUDIOMIX                 =0x13,
    cusdk__VSIP_ENTITY_SHADEAREANUM             =0x14,
    cusdk__VSIP_ENTITY_DEVPOWERTYPE             =0x15,
    cusdk__VSIP_ENTITY_MAXSTREAMSEND            =0x16,
    cusdk__VSIP_ENTITY_MAXSTREAMDIRSEND         =0x17,
    cusdk__VSIP_ENTITY_ENCRYPT		            =0x18,
    cusdk__VSIP_ENTITY_RECORDER					=0x19,
    cusdk__VSIP_ENTITY_PLAYER			        =0x1A,
    cusdk__VSIP_ENTITY_FILEDOWNLOAD	            =0x1B,
    cusdk__VSIP_ENTITY_VGAOUT			        =0x1C,
    cusdk__VSIP_ENTITY_VIDEOOUT		            =0x1D, 
    cusdk__VSIP_ENTITY_OSDCOLOR					=0x1E,
    cusdk__VSIP_ENTITY_PLATFORMTYPE				=0x1F,
    cusdk__VSIP_ENTITY_MOTIONDETECTAREANUM		=0x20,
    cusdk__VSIP_ENTITY_SNAPSHOTCHANNUM			=0x21,
    cusdk__VSIP_ENTITY_SLOTINFONUM				=0x22,
    cusdk__VSIP_ENTITY_CBRVBR					=0x23,       // �Ƿ��ܹ�����ͼ������
    cusdk__VSIP_ENTITY_MAXENCBITRATE			=0x24,		// �����Ч����
    cusdk__VSIP_ENTITY_MULTICHAN                =0x25,       // �Ƿ�֧��˫��
    cusdk__VSIP_ENTITY_MATRIX                   =0x26,       // �Ƿ�֧�־������
    cusdk__VSIP_ENTITY_VIDEOENCSLAVECHANNELNUM  =0x2F,       // ��Ƶ���븨��ͨ����  20090509
    cusdk__VSIP_ENTITY_TRANSPORTDATA			=0x29,       // �Ƿ�ʹ��͸�����ݴ���
    cusdk__VSIP_ENTITY_GPS         				=0x35,       // GPS����
    cusdk__VSIP_ENTITY_EXTINPUTPIN      		=0x36,       // ��չ��������(ֻ��NVRʹ��)add G.J. 20091020

    cusdk__VSIP_ENTITY_DECODEEXT                =0x3B,       //�Ƿ�֧�ֽ�������������չ  //add GJ 20100317

	cusdk__VSIP_ENTITY_SUPPORT_RESOLUTION_WH = 0x3D,		 // �Ƿ�֧���ַ�����ʽ���ϱ��ֱ���. 

	cusdk__VSIP_ENTITY_EXT_OSD					=0xF0,       //�Ƿ�֧�ֶ�����Ļ
	cusdk__VSIP_ENTITY_SUPPORT_TOTAL_PICTURE    =0xF2,       //�Ƿ�֧������ͼԪ 
	cusdk__VSIP_ENTITY_MAX_FRAMERATE			=0xF4,       //֧�ֵ�������֡��
	cusdk__VSIP_ENTITY_DEVTYPE                  =0xF5,	   //�豸����������
	cusdk__VSIP_ENTITY_CTRLTYPE                 =0xF6,	   //����̨����������
	cusdk__VSIP_ENTITY_MAX_ENC_CHANNEL			=0xF7,	   //������ͨ����������
	cusdk__VSIP_ENTITY_CHANNEL_CAPS				=0xF8,       //�豸����ͨ������

	cusdk__VSIP_ENTITY_RECLIST_QRY				=0x50,		// �Ƿ�֧��¼���������ѯ.
    cusdk__VSIP_ENTITY_CHANNL_STATE_REPORT		=0x51,		// �Ƿ�֧��ͨ��״̬�ϱ�
	cusdk__VSIPDEF_ENTITY_REPLAY_MAXSPEED       =0x52,      //��������
	cusdk__VSIPDEF_ENTITY_REPLAY_MAXSPEED_ALLFRAME	=0x53,      //�����֡�����ٶ�

	cusdk__VSIP_ENTITY_HMDI_COUNT				=0x60,    //HMDI����
	cusdk__VSIP_ENTITY_HMDI_1_DEC_CHAN_COUNT	=0x61,    //HMDI_1�Ľ���ͨ����
	cusdk__VSIP_ENTITY_HMDI_2_DEC_CHAN_COUNT	=0x62,    //HMDI_2�Ľ���ͨ����
	cusdk__VSIP_ENTITY_HMDI_3_DEC_CHAN_COUNT	=0x63,    //HMDI_3�Ľ���ͨ����
	cusdk__VSIP_ENTITY_HMDI_4_DEC_CHAN_COUNT	=0x64,    //HMDI_4�Ľ���ͨ����

	cusdk__VsipEntitySmartTab					= 0x97,		// �豸֧��ƴ��(ͼ������)����.
	cusdk__VSIP_ENTITY_SUPPORT_BD_TRACK_MODE    = 0xD7,     //֧������ǹ������
    cusdk__VSIP_ENTITY_G100_NOT_SUPPORT_VIDEONAME_SYNC =0x65,   //��֧����ƵԴ����ͬ��������

};

enum cusdk__EDeviceTransparentChannelType
{
	cusdk__TransChnCOMPort,			// �豸COM��͸������
	cusdk__TransChnCameraTaskConfig,// �����������������.
	cusdk__TransChnExtData,			// ͸�����ݣ�֧������Ӧ��֪ͨ��̬ģ�� 2014-07-29��ӡ�
	// cusdk__TransChnVSIP,			// vsip Э��ջͨ��.
	// cusdk__TransChnReqRspNtf,		// ͸������, ����Ӧ��֪ͨ��̬ģ��. 2014��8��14�� ���.
	cusdk__TransChnUserDef,        // ͸�����ݣ��û��Զ���͸����������,��ǰ���������ģ��֧�ֽ��ղ�����
};

enum cusdk__EChnStatus
{
	cusdk__CHNSTATUS_IDEL = 0,      
	cusdk__CHNSTATUS_ONLINE,
	cusdk__CHNSTATUS_OFFLINE ,
};

//��״̬ 
enum cusdk__EDomainStatus 
{ 
    cusdk__DomainStatusInvalid,   // ��Чֵ. 
    cusdk__DomainOnline,          // ����. 
    cusdk__DomainOffline,         // ����. 
};

class cusdk__UserGroup
{
	xsd__string id;			            // id.
	xsd__string name;		            // �û�������.
    xsd__string description;            // ����
	xsd__string creator;	            // ������.
	enum cusdk__EUserType  userType;				// �û�����. ����Ա,����Ա,��������Ա.
	std::set< enum cusdk__EUserPri > userPri;		// �û�Ȩ��. cusdk__PriUserAlarmLog��֮ǰ��Ȩ�޿��Էŵ�����.cusdk__PriUserWaterMark��֮���Ȩ�޷ŵ� userPriV2.
	std::set< int > userPriV2;						// �û�Ȩ��. +by lzx@2018-01-30 V2R4B1


	xsd__boolean maxStreamNumEnable;	// �Ƿ����� maxStreamNum, true ��ʾ����, false��ʾ������. +by lzx@2017-12-14 V2R2B3SP1
	xsd__int maxStreamNum;				// �û���������������Ŀ. +by lzx@2017-12-14 V2R2B3SP1
	// ��Ƶˮӡ����. +by lzx@2017-12-20 V2R4B1
	xsd__boolean waterMarkSupported;	// �Ƿ�֧��ˮӡ����. �������ֶ�, ֻ������ˮӡ����,�����ֶβ���Ч.
	xsd__boolean waterMarkEnabled;		// �Ƿ�����ˮӡ.
	xsd__string waterMarkFont;			// ˮӡ����
	xsd__int waterMarkFontSize;		// �����С.	����� cusdk__EWaterMarkFontSize.
	xsd__int waterMarkRotationAngle;	// ��ת�Ƕ�
	xsd__int waterMarkColor;			// ��ɫ.
	xsd__int waterMarkTransparent;		// ͸����
	xsd__boolean waterMarkTiled;		// �Ƿ�ƽ��.
	// ��Ƶˮӡ���ܽ���. +by lzx@2017-12-20 V2R4B1
};

enum cusdk__EWaterMarkFontSize			// ��Ƶˮӡ�����嶨��.
{
	cusdk__WaterMarkFontSizeInvalid,	// ��Ч
	cusdk__WaterMarkFontSizeSmall,		// С����
	cusdk__WaterMarkFontSizeNormal,	// ��������
	cusdk__WaterMarkFontSizeLarge,		// ������.
};

class cusdk__UserInfo
{
	xsd__string uuid;				// �û�uuid.
    xsd__string userName;			// ��¼ʱ�õ��û���. lee@kedacom 
	xsd__string userPassword;
	xsd__string domainID;			// �û�������ID.
	enum cusdk__EUserType  userType;				// �û�����. ����Ա,����Ա,��������Ա.
	std::set< enum cusdk__EUserPri > userPri;		// �û�Ȩ��. cusdk__PriUserAlarmLog��֮ǰ��Ȩ�޿��Էŵ�����.cusdk__PriUserWaterMark��֮���Ȩ�޷ŵ� userPriV2.
	std::set< int > userPriV2;						// �û�Ȩ��. +by lzx@2018-01-30 V2R4B1
	xsd__string creator;			// ������Id.
	xsd__string userGroup;			// �û���Id.
	xsd__string description;		// ����.
	time_t accountOverdueTime;	// �˻���ֹ����.
	time_t passwordOverdueTime;	// �����������.	
	xsd__boolean enable;					// ����.	
	xsd__boolean allowMultilogin;			// �Ƿ������ε�¼.
	xsd__boolean passwordExpireEnable;			//�Ƿ�����������Ч��
	xsd__int passwordExpire;						//������Ч�ڣ���λΪ��	
	//std::vector < xsd__string > bindIpList;		//�û���Ip���б�
	xsd__boolean  bindIPEnable;             //�Ƿ����ð�IP��ַ
	xsd__string bindingAddress; // �û��󶨵�IP��ַ, Ϊ�ձ�ʾ�����û��ϰ汾CU. +by lzx@2017-07-03
	xsd__boolean IsIPAppliedToMobileEnd; //�󶨵�IPʹ��Ӧ�����ƶ��� +by lzx@2017-08-24

	xsd__boolean maxStreamNumEnable;	// �Ƿ����� maxStreamNum, true ��ʾ����, false��ʾ������. +by lzx@2017-12-14 V2R2B3SP1
	xsd__int maxStreamNum;				// �û���������������Ŀ. +by lzx@2017-12-14 V2R2B3SP1

	// ��Ƶˮӡ����. +by lzx@2017-12-20 V2R4B1
	xsd__boolean waterMarkSupported;	// �Ƿ�֧��ˮӡ����. �������ֶ�, ֻ������ˮӡ����,�����ֶβ���Ч.
	xsd__boolean waterMarkEnabled;		// �Ƿ�����ˮӡ.
	xsd__string waterMarkFont;			// ˮӡ����
	xsd__int waterMarkFontSize;		// �����С.	����� cusdk__EWaterMarkFontSize.
	xsd__int waterMarkRotationAngle;	// ��ת�Ƕ�
	xsd__int waterMarkColor;			// ��ɫ.
	xsd__int waterMarkTransparent;		// ͸����
	xsd__boolean waterMarkTiled;		// �Ƿ�ƽ��.
	// ��Ƶˮӡ���ܽ���. +by lzx@2017-12-20 V2R4B1

	xsd__string passwordAlgorithm;	// ����ʹ�õļ����㷨. �� Ϊ����, "SM3_Base64" ��ʾSM3ȡɢ��ֵ��Base64����Ϊ�ַ���.
};
enum cusdk__EGBDeviceGroupType
{
	cusdk__GBDeviceGroupTypeInvalid,	// ��Ч.
	cusdk__GBDeviceGroupTypeCivilcode,	// ��������.
	cusdk__GBDeviceGroupTypeVitual,		// �������.(215/216)
};

// �豸�������Ϣ.
class cusdk__DeviceGroupInfo
{
	xsd__string id;			// �豸��Id.
	xsd__string parentId;	// �ϼ��豸��Id
	xsd__string domainId;   // �豸�������� UUID.
	xsd__string name;		// ��ʾ���豸����.
	xsd__string description;// ����.
	double      fOrderNumber; //�������
	xsd__string devTreeId;  //�豸��������Id

	xsd__string originalGBId;		// �豸��Ĺ���ID(��������)
	xsd__string newGBID;			// �豸���

	xsd__int GBDeviceGroupType;		// ��������ʱ���������, ����� cusdk__EGBDeviceGroupType. +by lzx@2017-12-14 V2R2B3SP1
	xsd__int CreatorType;           // ����������  ö��Ϊ cusdk__EDeviceCreatorType
};

// �豸��������Ϣ.
class cusdk__DeviceGroup : public cusdk__DeviceGroupInfo
{
	xsd__boolean hasDevice;	// �豸�鱾�����Ƿ����û�ӵ�е��豸

	xsd__boolean recursiveEmpty;	// �豸��ݹ�������¼��Ƿ�Ϊ��.
};

class cusdk__EncoderChn
{
	xsd__int id;
//	xsd__string name;
//	enum cusdk__EVideoFormat videoFormat;			// ��Ƶ��ʽ.
//	enum cusdk__EVideoResolution videoSolution;		// �ֱ���.
};

class cusdk__DevChannel
{
    xsd__string deviceId;     //�豸��Ψһ��ʶ pu uuid@domain
    xsd__int    channelId;      //ͨ���п����Ǳ���ͨ�����п�������ƵԴ�����п����Ƿ���ͨ��, Ҫ����Э�����Ӧ�ó����ж�
};

class cusdk__VideoSrc	// ��ƵԴ.
{
	xsd__int id;									// ��ƵԴ��������0��ʼ
	xsd__string name;								// ��ƵԴ����
	//std::set< xsd__int > chnIdList;					// ��ƵԴͨ���б�(������)
	int ptzLevel;									// ptz���Ƶȼ�.
	std::set< enum cusdk__EDevicePri > priList;	    // Ȩ��. cusdk__EDevicePri
	std::set< int > priListV2;					// �µ�Ȩ�� ����� cusdk__EDevicePri

	// 2015-01-22 ����豸GPS��Ϣ��֧��. by lizhixing@2015-01-22
	double				longitude;				// ����
	double				latitude;				// γ��
};

// class cusdk__AlarmInputChn	// �澯����(����)ͨ��.
// {
// 	xsd__int id;		// ��������ͨ��id.
// //	xsd__string name;	// ����
// };                                                   



class cusdk__DeviceInfo	
{
	xsd__string deviceId;							// uuid.
    xsd__string domainId;							// �豸������.
	xsd__string name;								// �豸����
	xsd__string deviceAlias;                        // �豸����, �̱��.
	//�����豸����˰��豸����, �����԰�����ƵԴ/�澯ͨ���Ȼ��鵥λ����, ����ע�͵���������ID
//	xsd__string devGroupId;							// �����豸��. 

	xsd__string deviceIP;							// �豸��IP��ַ. +by lizhixing@2013-10-21

	enum cusdk__EDeviceType deviceType;				// �豸����. ������, ������, ������� ��.

	xsd__string deviceModel;						// �豸��ϸ�ͺ�. �� IPC112 ��.
	std::vector< enum cusdk__EDeviceCapLabel > deviceCapLabelList;	// �豸��������ǩ�б�.

	xsd__string manufacturer;						// ������.

	int creatorType;								// �豸�Ĵ������� ö��Ϊ cusdk__EDeviceCreatorType

    xsd__string description;                        // �豸����
	std::vector< cusdk__VideoSrc > videoSrcList;	// ��ƵԴ�б�.
	std::set< enum cusdk__EDevicePri > priList;	    // �豸�����Ȩ��. cusdk__EDevicePri
	std::set< int > priListV2;					// �µ�Ȩ�� ����� cusdk__EDevicePri
	//std::vector< cusdk__AlarmInputChn > alarmInputChnList;	// �澯����ͨ���б�. 3A�û���ѯ�����澯����ͨ��.
    xsd__int videoSrcNum;                           //��ƵԴ�ܸ���
    xsd__int encodeChnNum;                          //����ͨ������
    xsd__int decodeChnNum;                          //����ͨ������
    xsd__int alarmInputNum;                         //������������

	xsd__int videoOutputChnNum;						// ��Ƶ���ͨ����Ŀ. 

	xsd__string deviceExtInfo;						// �豸��չ��Ϣ.
};

// �豸��/�豸 ������ϵ. ?? �е㸴��...... ������ÿ��豸���ȡ �豸������Ϣ �ķ�ʽ.
class cusdk__DeviceGroupRelation
{
	xsd__string devGroupId;					// �豸��id.
	xsd__string devId;						// �豸id.
	std::vector< int > videoSrcIdList;		// ��ƵԴ�б�.
	//std::vector< int > alarmInputChnList;	// �澯����ͨ��. �û��鲻���澯����ͨ��.
};

// �����õ���ƵԴ.
class cusdk__AssignVideoSrc
{
	xsd__int vieoSrcId;	// id
	std::set<  int > devPriSet;	// ��ƵԴȨ���б�. ����� cusdk__EDevicePri
	int ptzLevel;		// ptz���Ƶȼ�.
};

// �����õĸ澯����ͨ��. ����֧�ֻ���.
//typedef int cusdk__AssignAlarminputChn;

// ����ʱʹ�õ��豸��Ϣ.
class cusdk__AssignDeviceInfo
{
	xsd__string devId;
	std::set<  int >		devPriSet;		// �豸Ȩ���б�. ����� cusdk__EDevicePri
	std::vector< cusdk__AssignVideoSrc >		videoSrcIdList;	// ��ƵԴ�б�.
//	std::vector< cusdk__AssignAlarminputChn > alarmInputChnList;	// �澯����ͨ��. ��֧�ֻ���
};

// ��Ȩ�豸/��ʱ��Ȩ
class cusdk__AuthorizationDeviceInfo
{
	xsd__string userName;						// ����Ȩ��Ŀ���û���, ��¼�õ��û���.����ligang@suzhou
	cusdk__AssignDeviceInfo devInfo;		    // ����Ȩ���豸��Ϣ.
	xsd__string description;					// ����.
	time_t	createTime;							// ����ʱ��.
};

class cusdk__DeviceChannelNameChangeNotify
{
    xsd__string deviceId;					// �豸Id.
	enum cusdk__EDeviceChannelType chnType;	// ͨ������.
	int chnId;								// ͨ��Id.
	xsd__string chnName;					// ͨ����.
	double	longitude;						// ����
	double	latitude;						// γ��
};

class cusdk__DevOnlineNotify
{
    xsd__string deviceId;  //pu uuid@domain
    xsd__boolean online;
};

class cusdk__PuBaseCapSet
{
    xsd__int videoEncNum;               //��Ƶ����ͨ������;
    xsd__int videoDecNum;               //��Ƶ����ͨ������;
    xsd__int audioEncNum;               //��Ƶ����ͨ������;
    xsd__int audioDecNum;               //��Ƶ����ͨ������;
    xsd__int serialPortNum;             //��������;
    xsd__int alarmInputChnNum;               //������������;
    xsd__int alarmOutputChnNum;              //�����������;
    xsd__int videoInPortNum;            //��Ƶ����˿�����;
};

//Ϊ��ģ����������mapתvector������vector���������Ͷ���id + value
class cusdk__PuExtCap
{
    xsd__int id;            // ����������ID ֵ��cusdk__EDeviceEntity
    xsd__int value;         // ����������ֵ
};

class cusdk__AlarmInputChnEnable
{
    xsd__int id;            //����ͨ��
    xsd__boolean value;     //�Ƿ�ʹ��
};

class cusdk__ChannelVideoFormat
{
    xsd__int id;            //����ͨ��
    xsd__string value;      //������Ƶ��ʽ
};

class cusdk__ChannelVidDynValue
{
    xsd__int id;            //����ͨ��
    xsd__int value;         //��Ƶ�غ�
};

class cusdk__VideoSrcEncoderSet
{
    xsd__int id;            //��ƵԴ
    std::set<xsd__int> value;     //��ǰ��ƵԴ����ʹ�õı���ͨ���б�
};



class cusdk__EncoderAvailableVidSrcSet
{
    xsd__int id;            //����ͨ��
    std::set<xsd__int> value;       //����ͨ����Ӧ�Ŀ�����ƵԴ
};

class cusdk__VideoSrcCamType
{
    xsd__int id;            // ��ƵԴId
    enum cusdk__ECameraProtocolType camType;         //����ͷ����
};

class cusdk__VideoSrcCamID
{
    xsd__int id;            //��ƵԴ
    xsd__int value;         //����ͷ��ַ��
};

class cusdk__EncoderBitrate
{
    xsd__int id;            //����ͨ��
    xsd__int value;         //���������
};

class cusdk__EncoderChnStatus
{
	xsd__int id;            //����ͨ��
	xsd__int status;        //����ͨ��״̬ cusdk__EChnStatus
};

class cusdk__EncoderVideoRes	// ͨ���ֱ���.
{
	xsd__int id;		// ͨ��id
	xsd__int width;		// ��Ƶ���.
	xsd__int height;	// ��Ƶ�߶�.
};

class cusdk__DevConfigNotify
{
    xsd__string         deviceId;             //pu uuid@domain
    xsd__string         devIp;              //ǰ���豸IP
    xsd__string         devType;            //�豸���ͣ����������߽�����
    xsd__string         devMode;            //�豸�ͺţ�CU��ʹ�����ж�һЩ���豸��ƽ̨����ʹ��

    cusdk__PuBaseCapSet                         baseCapSet;     //����������
    std::vector<cusdk__PuExtCap >               extCapSet;      //��չ������
    std::vector<cusdk__AlarmInputChnEnable>     alarmInputChnEnable; //��������ͨ��ʹ������
    std::vector<cusdk__ChannelVideoFormat>      chnVidFormat;   //ͨ����ǰ����Ƶ��ʽ
    std::vector<cusdk__ChannelVidDynValue>      chnVidDynValue; //���ͨ����Ƶ��ʽ��H264�����ø�ͨ���Ķ�̬�غ�
    std::vector<cusdk__VideoSrcEncoderSet >     vidSrcChnSet;   //��ƵԴ��ǰ����ʹ�õ�ͨ���б�
    std::vector<cusdk__EncoderAvailableVidSrcSet > chnVidSrcSet;   //ͨ�����õ���ƵԴ�б�
    std::vector<cusdk__VideoSrcCamType>         camType;        //��ƵԴ������ͷ����
    std::vector<cusdk__VideoSrcCamID>           camId;          //��ƵԴ������ͷ��ַ��
    std::vector<cusdk__EncoderBitrate>          bitRate;        //ͨ���ı����� 

	std::vector<cusdk__EncoderChnStatus>		chnStatus;		// ͨ��״̬.
	std::vector<cusdk__EncoderVideoRes>			chnVideoRes;	// ͨ������Ƶ�ֱ���.

    xsd__int                            freeLicenceNum; //��ѵ�licence����
    xsd__int                            directTransNum; //�豸֧�ֵ�ֱ��·��
    xsd__boolean                        isDecCombined;  //�������Ƿ�ϳ�����
};

class cusdk__VidDecoderToPuChn
{
    xsd__int id;                //��������Ƶ����ͨ��
    cusdk__DevChannel puChn;    //���ŵı���ͨ��
};

class cusdk__AudDecoderToCuChn
{
    xsd__int id;                //��������Ƶ����ͨ��
    cusdk__DevChannel cuChn;    //��Ƶ������
};

class cusdk__EncoderStatusInt
{
    xsd__int id;                //����ͨ��
    xsd__boolean value;         //boolͨ��״̬
};


class cusdk__RecordStatus
{
	xsd__int encoderChannelId;
	enum cusdk__ERecordStatus recordStatus;

	int errorCode;				// ��ǰ¼���������Ĵ���.
};

class cusdk__EncoderStatusStr
{
    xsd__int id;                //����ͨ��
    xsd__string value;          //stringͨ��״̬
};

class cusdk__DecoderStatusStr
{
    xsd__int id;                //�������Ľ���ͨ��
    xsd__string value;          //stringͨ��״̬
};

class cusdk__DevServiceStatusNotify
{
    xsd__string deviceId;             //pu uuid@domain
    // std::vector<cusdk__VidDecoderToPuChn>      decChnVidSrc;    //���������ŵ���ƵԴͨ����ͨ����PU��
    std::vector<cusdk__AudDecoderToCuChn>   audioCallSrc;    //��Ƶ���е�Դ��ͨ����CU��
    std::vector<cusdk__EncoderStatusInt>    isPlatPreRec;    //�Ƿ���ƽ̨Ԥ¼
    std::vector<cusdk__RecordStatus>		platRecStatus;   // ƽ̨¼��״̬
    std::vector<cusdk__EncoderStatusInt>    isPuPreRec;      //�Ƿ���ǰ��Ԥ¼
    std::vector<cusdk__EncoderStatusInt>    isPuRec;         //�Ƿ���ǰ��¼��
    std::vector<cusdk__DecoderStatusStr>    decPoll;         //��������ѯ״̬
};

// class cusdk__DevAlarmData
// {
//     xsd__boolean    isAlarm;      //�Ƿ�澯
//     xsd__string     time;         //����ʱ��
//     xsd__string     desc;         //�澯����
// };

// class cusdk__AlarmChannelToAlarmData
// {
//     xsd__int id;        //�澯ͨ��
//     cusdk__DevAlarmData data;
// };



// Ԥ������.
// enum cusdk__ESchemeType
// {
// 	cusdk__SchemePrivate,	// ˽��Ԥ��.
// 	cusdk__SchemePublic,	// ����Ԥ��.
// };

class cusdk__DevAlarmNotify
{
    xsd__string deviceId;                     //pu uuid@domain
	enum cusdk__EAlarmType alarmType;       //�澯����, ֻ��
	int alarmTypeV2;						// �澯���;��嶨��� cusdk__EAlarmType, �¸澯������.
	int  alarmChnId;						//�澯����ͨ��
	enum cusdk__EAlarmStatus alarmStatus;	//�澯��־: ALARM_STATUS_OCCUR, ALARM_STATUS_RESTORE
	int  detAreaId;							//�ƶ�����ʾ�澯����ţ��ƶ����ʱ��Ч
	time_t time;							//�澯֪ͨ�¼�������ʱ��
	xsd__string alarmData;					//�澯����

	xsd__string alarmName;					// �澯����.

//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmVidMove;    //�ƶ����澯״̬
//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmOutPin;     //��������澯״̬
//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmInPin;      //��������澯״̬
//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmVidLost;    //��ƵԴ��ʧ�澯״̬
//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmInte;       //���ܸ澯״̬
//     xsd__boolean                                     alarmDiskFull;   //�������澯
};

class cusdk__StopPlayNotify
{
    xsd__int playId;        //Invite��CU���Ŵ���
    xsd__int errorCode;     //������
};



// �����ַ.
class cusdk__NetworkAddr
{
	xsd__string ip;
	xsd__int port;
};
class cusdk__NatInfo
{
	xsd__string natData;	// NAT�����͵�����.
	cusdk__NetworkAddr localAddr;	// ������ַ�˿�.
	cusdk__NetworkAddr natAddr;		// NATӳ���ĵ�ַ�˿�.
};
class cusdk__MediaNetInfo
{
	cusdk__NatInfo videoRtpNatInfo;
	cusdk__NatInfo videoRtcpNatInfo;
	cusdk__NatInfo audioRtpNatInfo;
	cusdk__NatInfo audioRtcpNatInfo;
};
class cusdk__MediaFormat
{
    xsd__int payloadType;       // �����ʽID�����嶨���kdvdef.h
	xsd__int encodingType;		// �����ʽ, ���嶨���?
    xsd__string encodingName;   // �����ʽ������Ӧ��payloadType������
    xsd__string manufacture;    // ý��������
	xsd__string mediaStreamType;// ý������ʽ. "ES", "PS", "TS", "RTP_ES", "RTP_PS", "RTP_TS" ��.
	
	xsd__int bitRateType;       // ��������, �ɱ���߹̶�����.1���̶����ʣ�CBR����2���ɱ����ʣ�VBR��
	xsd__int bitRate;			// ���� ��λ Kbps

	xsd__int keyframeInterval;	// �ؼ�֡���, ֡�ĸ���.
	xsd__int frameRate;			// ֡��. ��λΪ fps
	xsd__int videoWidth;		// �ֱ��ʿ��.
	xsd__int videoHeight;		// �ֱ��ʸ߶�.

	xsd__int clockRate;			// ʱ���������.

	xsd__int samplingRate;                      // ��ʵ������	���磬16000��44100��
	xsd__int audioChannels;                     // ������ ���磬1=��������2=˫����
};

class cusdk__OrderMediaFormat
{
	bool needTransMediaFormat;        //�Ƿ�ָ��ת���ʽ, FALSE��ʾ��ת�룬TRUE��ʾת��
	//Ŀ��Ĭ��Ϊkedacom�ı����ʽ, ��֧��תΪ�⳧�̸�ʽ
	enum cusdk__EVideoFormat dstVideoFormat; //Ŀ����Ƶ��ʽ��H264 or MPEG4
	enum cusdk__EVideoResolution dstSolution;    //Ŀ��ֱ��ʣ�Ĭ��D1
	xsd__int dstFrameRate;  //Ŀ����Ƶ֡�ʣ�Ĭ��25 fps
	xsd__int dstBitRate;    //Ŀ�����ʣ�Ĭ��800 kbps
	enum cusdk__EAudioFormat dstAudioFormat; //Ŀǰ�ݲ�֧��
};

class cusdk__TranscodeMediaFormat
{
	bool needTranscode;        //�Ƿ���Ҫת��.

	//Ŀ��Ĭ��Ϊkedacom�ı����ʽ, ��֧��תΪ�⳧�̸�ʽ
	enum cusdk__EVideoFormat dstVideoFormat;	//Ŀ����Ƶ��ʽ
	enum cusdk__EVideoResolution dstSolution;   //Ŀ��ֱ��ʣ�Ĭ��D1
	xsd__int dstFrameRate;  //Ŀ����Ƶ֡��
	xsd__int dstBitRate;    //Ŀ������

	enum cusdk__EAudioFormat dstAudioFormat; //Ŀǰ�ݲ�֧����Ƶת��.
};

// ������Ϣ��
class cusdk__PartitionInfo
{
	int diskId;				// ��������Id.
	int partitionId;		// ����Id.
	xsd__string partName;	// ������.
	int totalSize;			// �ܿռ�, ��λ MB
	int freeSize;			// ���пռ�, ��λ MB.
	enum cusdk__ERS_PartState partState;	// ����״̬.
	xsd__string partType;	// ������ʽ( vbfs, ext3 )
	int abnNum;				// �쳣����.
	xsd__string mountPath;	// ����·��.
	int start;				// ��ʼ��λ��.
	int end;				// ������λ��
	int recordNum;			// ¼����.
};

class cusdk__DiskInfo
{
	int diskId;				// ����Id.
	xsd__string diskName;	// ������.
	int diskSize;			// ����. ��λMB.
	int diskAbnNum;			// �쳣����.
	enum cusdk__ERS_DiskState diskState;	// ����״̬.
	xsd__string diskSource;	// ��Դ.
	int cylinders;			// ������.
	int heads;				// ��ͷ��.
	int sectors;			// ������Ŀ

	std::vector< cusdk__PartitionInfo > partitionList;	// �����б�.
};

// nru��Ϣ��
class cusdk__NruInfo
{
	xsd__string nruId;			// nru id��
	xsd__string devName;		//�豸��
	xsd__string devType;		//�豸����:VS200,KDM2801E��
	xsd__string manuFactory;	//�豸����

	xsd__string ifName;		// ������.
	xsd__string ipAddr;		// IP��ַ.
	xsd__string netMask;	// ��������.
	xsd__string gateway;	// ����.

	xsd__string RMSAddr;	// ע��RMS��������ַ.
	int maxBandWidthKb;		// NRU��������ݴ���, ��λ Kb/S.
	bool online;			// �Ƿ�����.
	bool used;				// �Ƿ�����.
};

// ʱ��Ρ�
class cusdk__TimeRange
{
	time_t         startTime;
	time_t         endTime;
};

// ¼��ʱ���
class cusdk__RecTimeRange
{
	enum cusdk__ERecordEventType        recEventType;   /*¼���¼����ͣ��ֶ�¼�񡢼ƻ�¼�� or �澯¼��*/
	cusdk__TimeRange      timeRange;             /*¼��ʱ�䷶Χ*/
};

// ¼����ʱ���
class cusdk__RecDuration
{
	xsd__string recordDurationId;		// ¼���ʶ(Token)��
	cusdk__TimeRange durationTimeRange;	// ¼���ܵ�ʱ���
	std::vector< cusdk__RecTimeRange > subTimeRangeList;	// ��¼������¼���еĲ�ͬ��¼��ε����ͣ�
	xsd__string recordDomainName;		// ¼���������������Ϊ����

	cusdk__DevChannel encoderChn;		// ¼��ı�����ͨ��.

	xsd__boolean locked;				// ¼���Ƿ�����. +by lzx@2017-09-18
	time_t	lockExpireTime;				// ¼��������ʧЧʱ��. +by lzx@2017-09-18
};

// ¼�񲥷�/����ʱ�Ĳ���.
class cusdk__RecordPlayDuration
{
	xsd__string recordDurationId;		// ¼���ʶ(Token)��
	enum cusdk__ERecordType recordType;	// ǰ��¼��/ƽ̨¼��.

	cusdk__TimeRange durationTimeRange;	// ¼���ܵ�ʱ���	
	xsd__string recordDomainName;		// ¼���������������Ϊ���� 

	cusdk__DevChannel encoderChn;		// ¼��ı�����ͨ��.
};


class cusdk__OnePuChnRecDays
{
	cusdk__DevChannel     puChn;
	cusdk__TimeRange	  timeRange;								// ʱ��Ρ�
	std::vector<bool>     recordStateInDays;                 // ��ʱ�����ĵ�һ�쿪ʼ�� ��¼�������Ϊtrue
};

// class cusdk__CronScedule
// {
// 	xsd__string cronStart;	// ��ʼ��Cron ʱ�䡣
// 	int duration;					// �����೤ʱ�䣬��λ�롣
// };

class cusdk__RecordPlan
{	
	int prerecordDuration;	// Ԥ¼ʱ��. 0 ��ʾ��Ԥ¼.
	enum cusdk__ERS_RecMode recordMode;		// ¼��ģʽ.
	std::vector< xsd__string > planedRecordCronTimeList;	// ¼��ƻ��Ŀ���ʱ��㡣

	int recordDurationAfterAlarm;	// �澯�ָ���ƽ̨¼��ʱ��, ��λ��.
	int recStorageDuration; //¼��洢ʱ����>=1�죩��0-ϵͳĬ��
	bool recDuraionEnabled; //��ǰ�Ƿ�����¼��洢ʱ������
	bool supportSetRecDuration; //�Ƿ�֧������¼��洢ʱ��
};

class cusdk__TVDecoderBind
{
	int tvId;		// �������
	xsd__string decoderId;	// �������豸id.

	int decoderOutputId;	// �������豸��Ƶ���ͨ��Id.
};

class cusdk__TVWall
{
	xsd__string tvwallId;			// ����ǽ���
	xsd__string domainId;		// ����ǽ��������
	xsd__string name;				// ����ǽ����
	int tvNum;						// ���ӻ���Ŀ
	int tvwallStyle;					// ����ǽ���, ��ʹ��ö�٣���ΪTAS�����ľ�������ݣ�����ʱ����������ʱTASҲҪ����
	xsd__string tvwallCustomStyleData; // ����ǽ�Զ���������
	xsd__string desc;          // ����ǽ����
	time_t createTime;		// ��������

	std::vector< cusdk__TVDecoderBind > tvDecoderBind; // ������������󶨹�ϵ
};

// ������Ѳ��һ��
class cusdk__TVWallWindowPollStep
{
	cusdk__DevChannel encoderChn;	// ������/IPC ����ƵԴͨ��.
	int duration;					// ����ƵԴ����������ʱ��.
};

// �����ϵĵ����Ӵ���(����ϳ�)��Ԥ��.
class cusdk__TVWALLTVDivScheme
{
	int divId;	// ������ͨ��id.
	std::vector< cusdk__TVWallWindowPollStep > pollSteps;	// ��Ѳ�ĸ�������. ���ֻ��һ����Ѳ, ������ͨ���.
};

// �������ӵ�Ԥ��.
class cusdk__TVWallTVScheme
{
	int tvId;			// ����Id
	enum cusdk__ETVDivStyle divStyle;	// ���ڵĻ���ϳɷֻ�����.	
	std::vector< cusdk__TVWALLTVDivScheme > tvWallDivList;	// ���ӵĻ���ϳɵ�С����.	
};


// ����ǽ��Ԥ��.
class cusdk__TVWallScheme
{
//	xsd__string schemeId;	// Ԥ��Id.
	xsd__string schemeName;	// Ԥ������.
	xsd__string tvWallId;	// Ԥ����������ǽId.
//	enum cusdk__ESchemeType schemeType;	// ����Ԥ������˽��Ԥ��.
	std::vector< cusdk__TVWallTVScheme > tvSchemeList;	// ����ǽ�����е��ӵ�Ԥ���б�.
};


// Ԥ����Ѳ.
class cusdk__TVWallSchemePollStep
{
	xsd__string tvWallSchemeName;	// Ԥ����
	int duration;					// Ԥ���л�ǰ�ı���ʱ��.
};


// ����ǽ��һ������ϳ��Ӵ��ڵ�״̬.
class cusdk__TVWallTVDivState
{
	//	int tvId;	// ����Id.
	int divId;	// ����ϳ��Ӵ���(ͨ��)Id.

	enum cusdk__ETVWallPlayMode divPlayMode;	// ����ǽ�ֻ���Ĳ���ģʽ. ( ʵʱ���, ƽ̨¼��, ǰ��¼�� ).
	xsd__string recordPlaySession;	// ¼�񲥷ŵ�Session.
	cusdk__RecordPlayDuration recordPlayDuration;	// ��ǰ���ŵ�¼��ʱ���.

	enum cusdk__EPollStatus pollState;	// ������Ѳ��״̬.	

	cusdk__DevChannel encoderChn;	// ��ǰ����ı�����ͨ��.
	xsd__int errorCode;		// ��ǰ���������ֵĴ���, ������.
};

// ����ǽ�ϵ�һ�����ӵ�״̬.
class cusdk__TVWallTVState
{
	int tvId;	// ����Id.
	std::vector< cusdk__TVWallTVDivState > divStateList;	// ����ϳ��ӻ���״̬. û�л���ϳɵĴ���ֻ��һ��.
};

class cusdk__TVWallStateNotify
{
	xsd__string tvWallId;	// ����ǽId.

	enum cusdk__EPollStatus schemePollState;				// Ԥ����Ѳ��״̬.
	std::vector< cusdk__TVWallSchemePollStep > pollSteps;	// ����ǽԤ����Ѳ�Ĳ���.

	cusdk__TVWallScheme curSchemeInfo;						// ��ǰ����ǽ����ʱԤ��.(�����Ǳ������, Ҳ������δ�������ʱԤ��)

	std::vector< cusdk__TVWallTVState > tvStateList;		// ����ǽ�ϵĵ��ӵ�״̬.
};

class cusdk__DomainInfo
{
	xsd__string domainId;	// ��Id
	xsd__string domainName;	// ����
	xsd__string parentDomainId;	// �ϼ���Id.
    xsd__int    status;         // ��״̬cusdk__EDomainStatus
};


class cusdk__TransChannel
{
	enum cusdk__ETransChannelType transChannelType;	// ����ͨ��������.

	xsd__string natData;	// NAT�����͵�����.( ֻ��UDPģʽ����Ч )

	std::vector< cusdk__NetworkAddr > netAddrList;	// �����ַ�б�, ������ַ��ǰ, ����(NAT)��ַ�ں�.

	//2013-11-01 ƽ̨δʵ�֣��ݲ��޸�// 2013-09-10 ��ƽ̨��Ҫ��������ַ��NATӳ���ַ�ֿ�.
// 	std::vector< cusdk__NetworkAddr > localAddrList;	// ������ַ�˿�.
// 	std::vector< cusdk__NetworkAddr > natAddrList;		// NATӳ���ĵ�ַ�˿�.
};

class cusdk__MediaDescription
{
	enum cusdk__ETransMode transMode;	// TCP/UDP
	enum cusdk__EMediaType mediaType;  //ý�����ͣ�VIDEO��AUDIO��DATA

	std::vector< cusdk__TransChannel > transChannelList;  // ý�崫��ͨ����UDP����ʱ��ͨ����RTP��RTCPͨ�������ݴ����TCP����ʱ����ͨ���Ϳ���	

	std::vector< cusdk__MediaFormat > supportedFormats;	// ��֧�ֵ���Ƶ�����ʽ. ����Ҫ��չ. �ֱ���, ���ʵȱ�Ҫ�ı��������Ϣ, �ο���׼.
	std::vector< enum cusdk__EVideoResolution > supportedResolution;	// ��֧�ֵ���Ƶ�ֱ���. ֻ����Ƶʱ��Ч.

};

// ý����չ��Ϣ.
class cusdk__MediaExtInfo
{
	xsd__string extInfoName;	
	xsd__string extInfoValue;
};

class cusdk__BaseReq
{
	xsd__string session;
};
//
//class cusdk__BaseRsp
//{
//	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
//	long errorCode;
//};




/*************************************************
���巽������
**************************************************/
class cusdk__DetectServerReq
{
};

class cusdk__DetectServerRsp /* : public cusdk__BaseRsp */
{
	xsd__string domainName;		// ƽ̨����
	
	xsd__string cuNatIp;        // CU��Nat��ַ.
	xsd__int    cuNatPort;		// CU��Nat�˿�.
	
	xsd__string session;		// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;

};

//gsoap cusdk service method-documentation: ̽��������Ƿ������ṩ����. .
int cusdk__DetectServer( cusdk__DetectServerReq detectReq, cusdk__DetectServerRsp &detectRsp );

class cusdk__STUNServerInfo
{
    xsd__string ip;        //����ϵͳ���õ�ȫ��Ψһ��STUN������
    xsd__int    port;
};

enum cusdk__ENATMode
{
	cusdk__NATModeAuto,		// �Զ�ģʽ,�ɷ�����Զ����
	cusdk__NATModeInner,	// λ��NAT����,������ַӳ��.
	cusdk__NATModeOutside,	// λ��NAT����,�����ַӳ��.
};

class cusdk__LoginReq
{
	xsd__string userName;
	xsd__string password;	
	xsd__string cusdkVersion;	// �ͻ���ʹ��sdk�İ汾.

	xsd__string clientType;		// �ͻ�������,(ѡ��,Ĭ��ΪCU).

	xsd__boolean isSupportNA;	// �Ƿ�֧��NA��.

	xsd__string webCUClientAddr;	// WebCU�ͻ��˵�ַ(WebCUͨ��ƽ̨������������WebCU����˵�¼CUI,��ҳ��ַ��ҪWebCU������ṩ.)

	xsd__boolean isSupportSecure;	// CU�Ƿ�֧�ְ�ȫģʽ.

	xsd__string passwordAlgorithm;	// ����ʹ�õļ����㷨. �� Ϊ����, "SM3_Base64" ��ʾSM3ȡɢ��ֵ��Base64����Ϊ�ַ���.
};

class cusdk__LoginRsp /* : public cusdk__BaseRsp */
{
//	xsd__string session;
	xsd__string cmsVersion;     // ƽ̨�汾, ����CU�ж�����
	xsd__string cuVersion;		// ƽ̨�ϵĿͻ��˰汾.
	xsd__string cuUpdateUrl;	// CU��������URL.
	cusdk__STUNServerInfo stunInfo;
	xsd__string serverDataVersion; // ����˵����ݰ汾, ���ڱ������ݻ����Ż�.
    xsd__int    userLockRemainTime;    //�û�����ʣ��ʱ�䣬��λΪ��
	xsd__int    passwordRemainTime;   //������뵽�ڻ�ʣ���ʱ�䣬��λΪ�롣Ϊ0�͸�ֵʱ����ʾ�Ѿ�����

	xsd__boolean isSecureEnabled;	// ƽ̨�Ƿ����õİ�ȫģʽ.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};

//gsoap cusdk service method-documentation: ��¼ .
int cusdk__Login( cusdk__LoginReq loginReq, cusdk__LoginRsp &loginRsp );

class cusdk__LogoutReq : public cusdk__BaseReq
{
};
class cusdk__LogoutRsp /* : public cusdk__BaseRsp */
{
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};

//gsoap cusdk service method-documentation: �ǳ� .
int cusdk__Logout( cusdk__LogoutReq logoutReq, cusdk__LogoutRsp& logoutRsp );


class cusdk__NotifySubscription
{
	enum cusdk__ESubscribeNotifyType subscribeNotifyType;	// ֪ͨ���ĵ�����.

	xsd__string deviceId;									// ���ĵ�����豸URI.
	std::set<enum cusdk__EPuSubscribeType> subscribeTypes;	// �豸״̬���ĵ�������.

	xsd__string deviceTreeId;								// ���ĵ��豸��ID, ֻ�ڶ���������cusdk__SubscribeDeviceGroupStatʱ��Ч.
	xsd__string deviceGroupId;								// ���ĵ��豸��ID, ֻ�ڶ���������cusdk__SubscribeDeviceGroupStatʱ��Ч.
};

class cusdk__SubscribeNotifyReq : public cusdk__BaseReq
{
	std::vector< cusdk__NotifySubscription > subscriptionList;
};
class cusdk__SubscribeNotifyRsp /* : public cusdk__BaseRsp */
{
    std::vector< cusdk__NotifySubscription > failedList;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};


//gsoap cusdk service method-documentation: ֪ͨ���� .
int cusdk__SubscribeNotify( cusdk__SubscribeNotifyReq req, cusdk__SubscribeNotifyRsp& rsp );

class cusdk__UnsubscribeNotifyReq : public cusdk__BaseReq
{
	std::vector< cusdk__NotifySubscription > subscriptionList;
};
class cusdk__UnsubscribeNotifyRsp /* : public cusdk__BaseRsp */
{
    std::vector< cusdk__NotifySubscription > failedList;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};

//gsoap cusdk service method-documentation: ȡ��״̬���� .
int cusdk__UnsubscribeNotify( cusdk__UnsubscribeNotifyReq req, cusdk__UnsubscribeNotifyRsp& rsp );

class cusdk__DeviceFullInfo : public cusdk__DeviceInfo
{
    std::vector< cusdk__DeviceGroupRelation > devGroupToChannel;    //�豸���ͨ���Ķ�Ӧ��ϵ������devIdΪ��
};

// �澯��������.
class cusdk__AlarmLinkAction
{
	std::vector< enum cusdk__EAlarmLinkActionType > actionType;	// ��������������, ���Զ���.
	cusdk__DevChannel encoderChn;	// �澯�����ı���ͨ��.

	//int recordDurationAfterAlarm;	// �澯�ָ���ƽ̨¼��ʱ��.

	int camPrePos;	// ����ͷԤ��λ

	// ����ǽ
	xsd__string tvWallId;	// ����ǽId.
	int tvId;				// �������.
	int tvDivId;			// ���ӷֻ������.

	// �澯���.
	int alarmIOOutputChnId;	// �澯���ͨ��Id.

	// 
	// 	std::vector< xsd__string > SMSPhoneNumList;		// ���ŵ绰�����б�.
	// 	std::vector< xsd__string > voicePhoneNumList;	// ��Ѷ֪ͨ�����б�.
	// 	std::vector< xsd__string > mailList;			// �����ַ�б�.
};

// �澯Դ.
class cusdk__AlarmSource
{
	xsd__string alarmDeviceId;			// �澯�豸Id.
	int alarmInputChnId;				// �����澯��ͨ��Id. ��ͬ�澯�����µĺ��岻ͬ.
	enum cusdk__EAlarmType alarmType;	// �澯����.
};

// �澯��������
class cusdk__AlarmLinkConfig
{
	cusdk__AlarmSource alarmSource;	// �澯Դ

	//	xsd__string alarmName;			// �澯����.

	bool enabled;	// �Ƿ�����.
	std::vector< xsd__string > enabledCronTimeList;	// ���ø澯������ʱ���.

	std::vector< cusdk__AlarmLinkAction > alarmLinkActionList;	// �澯���������б�.	
};

// ����ʧ��֪ͨ.
class cusdk__ExportFail
{
	xsd__string exportFileUrl;	// ������Url.
	int errorCode;				// ������ʧ��ԭ��.
};

// ��������
enum cusdk__EServiceType
{
	/*¼���ܣ�RCS�����澯���ܣ�ALS��������ǽ���ܣ�TVS�������ӵ�ͼ���ܣ�MPS����
	��־���ܣ�UAS����ƽ̨���ô洢����UAS �����û��Զ������ݺ�ǰ���������ݣ�*/
	cusdk__ServiceRecord,
	cusdk__ServiceAlarmLink,
	cusdk__ServiceTVWall,
	cusdk__ServiceMap,
	cusdk__ServiceLog,
	cusdk__ServiceConfig,
};
// ����״̬.
class cusdk__ServiceStatus
{
	enum cusdk__EServiceType serviceType;
	bool serviceOk;
};

// GPS��Ϣ
class cusdk__GPSInfo	// �ο��󻪵�.
{
	time_t				revTime;				// ��λʱ��
	xsd__string			deviceId;				// �豸���к�
	double				longitude;				// ����
	double				latitude;				// γ��

	double				marLongitude;			// ��ƫ��ľ���.
	double				marLatitude;			// ��ƫ���γ��.

	double				height;					// �߶�(��)
	double				angle;					// �����(��������Ϊԭ�㣬˳ʱ��Ϊ��)
	double				speed;					// �ٶ�(����/Сʱ)
//	int					starCount;				// ��λ����
//	xsd__boolean		antennaState;			// ����״̬(true �ã�false ��)
//	xsd__boolean		orientationState;		// ��λ״̬(true ��λ��false ����λ)
};

// �豸͸������֪ͨ.
class cusdk__TransDataNotify
{
	xsd__string deviceId;	// �豸Id.
	xsd__int chnId;			// �豸ͨ��ID.

	xsd__int transChnType;	// ͸��ͨ������. ��enum cusdk__EDeviceTransparentChannelType

	xsd__int dataMsgType;	// ͸��ͨ��Э���о���涨.
	xsd__boolean isEnd;				// �Ƿ������һ��֪ͨ.
	xsd__string transDataBase64;	// ����,base64����.
    xsd__string dataMsgTypeUserDef;    //���ͣ�transChnTypeΪcusdk_TransChnUserDefʱ��ʵ�����������ﶨ��

};

//�豸���ͽ���֪ͨ
class cusdk__DevicePushProgressNotify
{
	xsd__int pos;              //��ǰ���ȣ�����������0~100������-1������� 
	xsd__string errorDsc;      //��������            
};

// ƽ̨֪ͨ��Ϣ.
class cusdk__CMSMessageNotify
{
	time_t messageTime;			// �¼�����ʱ��.
	xsd__string messageType;	// ֪ͨ��Ϣ����
	xsd__string messageContent;	// ֪ͨ��Ϣ����
};

// ��ʼ���봫��֪ͨ.
class cusdk__StartEncoderTransNotify
{
	xsd__int encodeChnId;	// �ͻ��˵�����Ƶ����ͨ��Id. 

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// Э�̺��ý����Ϣ����. ���Ϊ�վͱ�ʾ����Ҫ��������.
	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// ý����չ��Ϣ.
};

// �豸��ͳ����Ϣ֪ͨ.
class cusdk__DeviceGroupStatNotify
{
	xsd__string treeId;		// �豸��Id
	xsd__string groupId;	// �豸��Id
	xsd__boolean recursive;	// �Ƿ�ݹ�ͳ��.

	int totalNum;			// ����
	int onlineNum;			// ������Ŀ
	int errorCode;			// ͳ�ƹ����г���Ĵ�����
};

class cusdk__GetNotifyReq : public cusdk__BaseReq
{
	xsd__int timeoutMSec;	// ��ʱʱ��, ��λ����. Ϊ0��ʾ��������, ���������֪ͨ��Ϣ���ʱ�󷵻�.
};
class cusdk__GetNotifyRsp /* : public cusdk__BaseRsp */
{
    std::vector< cusdk__DeviceFullInfo > deviceAddNotify;       //1 �豸���֪ͨ.
    std::vector< cusdk__AssignDeviceInfo > deviceDelNotify;     //2 �豸 �� ��ص�ɾ��֪ͨ. 
    std::vector< cusdk__DeviceFullInfo > deviceModNotify;       //3 �豸�޸�֪ͨ.

    std::vector< cusdk__DeviceChannelNameChangeNotify> deviceChannelNameChangeNotify;   //4 �豸ͨ�������޸�֪ͨ.

    std::vector< cusdk__DeviceGroupInfo > deviceGroupAddNotify;     //5 �豸�����֪ͨ.
    std::vector< xsd__string > deviceGroupDelNotify;            //6 �豸��ɾ��֪ͨ.
    std::vector< cusdk__DeviceGroupInfo > deviceGroupModNotify;     //7 �豸���޸�֪ͨ.

	std::vector< cusdk__DevOnlineNotify > devOnlineNotify;      //8 �豸������֪ͨ.
    std::vector< cusdk__DevConfigNotify > devConfigNotify;      //9 �豸�����޸�֪ͨ.
    std::vector< cusdk__DevServiceStatusNotify > devServiceStatusNotify;	//10 �豸 ƽ̨¼��/ǰ��¼��/����/��Ѳ(������)�ȷ���״̬.
    std::vector< cusdk__DevAlarmNotify > devAlarmNotify;        //11 �豸�澯֪ͨ.

    std::vector< cusdk__StopPlayNotify > stopPlayNotify;        //12 �����쳣�ж�֪ͨ.

	std::vector< cusdk__TVWallStateNotify > tvWallStateNotify;	//13 ����ǽ״̬֪ͨ.

	std::vector< cusdk__UserInfo > userAddNotify;			//14 ����û�֪ͨ.
	std::vector< xsd__string > userDelNotify;				//15 ɾ���û�֪ͨ.
	std::vector< cusdk__UserInfo > userModNotify;			//16 �޸��û�֪ͨ.

	std::vector< cusdk__UserGroup > userGroupAddNotify;     //17 ����û���֪ͨ.
	std::vector< xsd__string > userGroupDelNotify;          //18 ɾ���û���֪ͨ.
	std::vector< cusdk__UserGroup > userGroupModNotify;     //19 �޸��û���֪ͨ.

	std::vector< cusdk__TVWall > tvWallAddNotify;     //20 ��ӵ���ǽ֪ͨ.
	std::vector< xsd__string > tvWallDelNotify;       //21 ɾ������ǽ֪ͨ.
	std::vector< cusdk__TVWall > tvWallModNotify;     //22 �޸ĵ���ǽ֪ͨ.

	std::vector< cusdk__DeviceGroupRelation > deviceGroupAddDeviceNotify;     //23 �豸���л����豸֪ͨ.
	std::vector< cusdk__DeviceGroupRelation > deviceGroupDelDeviceNotify;     //24 �豸����ɾ���豸֪ͨ.

	std::vector< cusdk__AlarmLinkConfig > alarmLinkConfigChangeNotify;		// 25 �澯�������ø���֪ͨ.

	std::vector< cusdk__STUNServerInfo > stunChangedNotify;		// 26 stun��ַ���ø���֪ͨ.
	std::vector< cusdk__ExportFail > exportFailNotify;		// 27 ����ʧ��֪ͨ.

	std::vector< cusdk__ServiceStatus > serviceStatusNotify;	// 28 ƽ̨����.

	std::vector< cusdk__GPSInfo > gpsNotify;	// 29 GPS��Ϣ֪ͨ.

	std::vector< cusdk__TransDataNotify > transDataNotify;	// 30 ͸������֪ͨ.

	std::vector< cusdk__DevicePushProgressNotify > devPushProgressNotify;  // 31�豸����֪ͨ

	std::vector< cusdk__CMSMessageNotify > cmsMessageNotify;	// 32 ƽ̨������֪ͨ��Ϣ.

	std::vector< cusdk__StartEncoderTransNotify > startEncoderTransNotify;	// 33 ƽ̨Ҫ��CU��ʼ���봫��(����).

	std::vector< cusdk__DeviceGroupStatNotify > deviceGroupStatNotify;		// 34 �豸��ͳ��֪ͨ.

    std::vector< xsd__int > seqMarks;   //֪ͨ��Ϣ�����б�ǣ���Ǵ�1��ʼ����ʶ���ݳ�Ա���ֵ�λ��
    xsd__boolean hasDataLeft;           //�Ƿ���ʣ��֪ͨ����

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ֪ͨ. �������������.
int cusdk__GetNotify( cusdk__GetNotifyReq req, cusdk__GetNotifyRsp& rsp );


class cusdk__GetCUIListReq : public cusdk__BaseReq
{
};
class cusdk__GetCUIListRsp /* : public cusdk__BaseRsp */
{
	std::vector< xsd__string > cuiList;	    //����ÿ��CUI��URL, ����˿ڲ�Ϊ80, ��Ҫ���϶˿���Ϣ

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ����CUI��ַ�б�, ���ڶ�·���ٺ͸��ؾ���.
int cusdk__GetCUIList( cusdk__GetCUIListReq req, cusdk__GetCUIListRsp& rsp );


class cusdk__GetDomainListReq : public cusdk__BaseReq
{
};
class cusdk__GetDomainListRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DomainInfo > domainList;	    // �������б�

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ������.
int cusdk__GetDomainList( cusdk__GetDomainListReq req, cusdk__GetDomainListRsp& rsp );


class cusdk__SetupStreamReq : public cusdk__BaseReq
{
	xsd__int playId;

	xsd__string devId;
	xsd__int devChnId;

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// ������󷽵�ý����Ϣ����.

	cusdk__TranscodeMediaFormat transcodeMeidaFormat;		// Ҫ��ƽ̨ת��

	xsd__boolean directTrans;								// �Ƿ�ֱ��, ��ǰ��ֱ�ӷ���.
	xsd__boolean directTransFromDeviceDomain;				// ֱ��, ���豸�����򷢳�����.

// 	enum cusdk__EPlayMode playMode;	        // Video/Audio/Both
 	int transMode;		// ����μ� cusdk__ETransMode
// 	std::vector<cusdk__MediaNetInfo> localNetInfoList;  //֮������vector, ����CU��IP���
// 	cusdk__OrderMediaFormat mediaInfo;	// Ҫ��ƽ̨ת��.
};
class cusdk__SetupStreamRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// ����������ý����Ϣ����.

	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// ý����չ��Ϣ.
// 	cusdk__MediaFormat videoFormat;
// 	cusdk__MediaFormat audioFormat;
// 	std::vector<cusdk__MediaNetInfo> remoteNetInfoList; //ƽ̨Ҳ��IP���

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����������Դ, ��������.
int cusdk__SetupStream( cusdk__SetupStreamReq req, cusdk__SetupStreamRsp& rsp );

class cusdk__PlayStreamReq : public cusdk__BaseReq
{
	xsd__int playId;
};
class cusdk__PlayStreamRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ��������.
int cusdk__PlayStream( cusdk__PlayStreamReq req, cusdk__PlayStreamRsp& rsp );

class cusdk__StopStreamReq : public cusdk__BaseReq
{
	xsd__int playId;	
};
class cusdk__StopStreamRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ֹͣ����.
int cusdk__StopStream( cusdk__StopStreamReq req, cusdk__StopStreamRsp& rsp );

class cusdk__SendKeyFrameReq : public cusdk__BaseReq
{
	xsd__string devId;
	xsd__int encoderChannelId;
};
class cusdk__SendKeyFrameRsp /* : public cusdk__BaseRsp */
{
	xsd__string errorDesc;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ǿ��ǰ���豸���͹ؼ�֡.
int cusdk__SendKeyFrame( cusdk__SendKeyFrameReq req, cusdk__SendKeyFrameRsp& rsp );

class cusdk__PtzCtrlReq : public cusdk__BaseReq
{
	xsd__string devId;
	xsd__int videoSrcId;
	enum cusdk__EPtzCmd ptzCmd;
	std::vector< int > ptzParam;
};
class cusdk__PtzCtrlRsp /* : public cusdk__BaseRsp */
{
    xsd__string errorDesc;

	xsd__string userName;	//����PTZ���û����������ɹ�ֵΪ�գ�
	xsd__int	ptzLevel;	//����PTZ���û�ptz���Ƶȼ��������ɹ�Ϊ�գ�
	time_t		beginTime;	//������ʼʱ��
	time_t		endTime;	//�Զ�����ʱ��

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: PTZ����.
int cusdk__PtzCtrl( cusdk__PtzCtrlReq req, cusdk__PtzCtrlRsp& rsp );

class cusdk__PtzLockReq : public cusdk__BaseReq
{
	xsd__string devId;
	xsd__int videoSrcId;
	time_t beginTime;		// ������ʼʱ��.
	time_t endTime;		    // �Զ�����ʱ��.
	xsd__string password;	// �ֶ���������.
};
class cusdk__PtzLockRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	xsd__string userName;	//����PTZ���û����������ɹ�ֵΪ�գ�
	xsd__int	ptzLevel;	//����PTZ���û�ptz���Ƶȼ��������ɹ�Ϊ�գ�
	time_t		beginTime;	//������ʼʱ��
	time_t		endTime;	//�Զ�����ʱ��
	long errorCode;			//������

};
//gsoap cusdk service method-documentation: PTZ����.
int cusdk__PtzLock( cusdk__PtzLockReq req, cusdk__PtzLockRsp& rsp );

class cusdk__PtzUnlockReq : public cusdk__BaseReq
{
	xsd__string devId;
	xsd__int videoSrcId;	
	xsd__string password;	// ��������.
};
class cusdk__PtzUnlockRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	xsd__string	userName;	//����PTZ���û����������ɹ�ֵΪ�գ�
	xsd__int	ptzLevel;	//����PTZ���û�ptz���Ƶȼ��������ɹ�ֵΪ�գ�
	time_t		beginTime;	//������ʼʱ��
	time_t		endTime;	//��������ʱ��
	long errorCode;
};
//gsoap cusdk service method-documentation: PTZ����.
int cusdk__PtzUnlock( cusdk__PtzUnlockReq req, cusdk__PtzUnlockRsp& rsp );

class cusdk__PtzUnlockAllReq : public cusdk__BaseReq
{
};
class cusdk__PtzUnlockAllRsp /* : public cusdk__BaseRsp */
{	
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: PTZȫ������.
int cusdk__PtzUnlockAll( cusdk__PtzUnlockAllReq req, cusdk__PtzUnlockAllRsp& rsp );

class cusdk__SendTransDataReq : public cusdk__BaseReq
{
	enum cusdk__EDeviceTransparentChannelType transChnType;	// ͸��ͨ������.

	xsd__string devId;			// �豸Id.
	xsd__int channelId;			// �豸ͨ��Id.

	xsd__int dataMsgType;			// �������͡�
	xsd__string base64TransDataReq;	// ʹ��base64��׼�����͸������.
    xsd__string dataMsgTypeUserDef;    //�û��Զ�������ʵ��Я�����������ͣ�dataMsgTypeΪcusdk__TransChnUserDefʱ��ʹ�ø��ֶδ���

};
class cusdk__SendTransDataRsp /* : public cusdk__BaseRsp */
{
	xsd__int dataMsgType;			// ���ص��������͡�
	xsd__string base64TransDataRsp;	// ���ص�base64��׼�����͸������.

	xsd__boolean hasNotify;			// �Ƿ���Notify֪ͨ. cusdk__TransDataNotify.

	// PTZ����ԭ����ɵ�ʧ��.
	xsd__string errorDesc;

	xsd__string userName;	//����PTZ���û����������ɹ�ֵΪ�գ�
	xsd__int	ptzLevel;	//����PTZ���û�ptz���Ƶȼ��������ɹ�Ϊ�գ�
	time_t		beginTime;	//������ʼʱ��
	time_t		endTime;	//�Զ�����ʱ��

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
    xsd__string dataMsgTypeUserDef;    //�û��Զ�������ʵ��Я�����������ͣ�dataMsgTypeΪcusdk__TransChnUserDefʱ��ʹ�ø��ֶδ���


};
//gsoap cusdk service method-documentation: ����͸������.
int cusdk__SendTransData( cusdk__SendTransDataReq req, cusdk__SendTransDataRsp& rsp );


class cusdk__GetUserInfoReq : public cusdk__BaseReq
{
	xsd__string userName;
};
class cusdk__GetUserInfoRsp /* : public cusdk__BaseRsp */
{
	cusdk__UserInfo userInfo;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��ǰ��¼CU���û���Ϣ, ������CU�����¼�û�.
int cusdk__GetUserInfo( cusdk__GetUserInfoReq req, cusdk__GetUserInfoRsp& rsp );


class cusdk__GetUserGroupInfoReq : public cusdk__BaseReq
{
	xsd__string userGroupId;	// �û���id.
};
class cusdk__GetUserGroupInfoRsp /* : public cusdk__BaseRsp */
{
	cusdk__UserGroup userGroupInfo;	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��ǰ��¼CU���û�����Ϣ, ������CU�����¼�û�.
int cusdk__GetUserGroupInfo( cusdk__GetUserGroupInfoReq req, cusdk__GetUserGroupInfoRsp& rsp );

class cusdk__GetUserDataReq : public cusdk__BaseReq
{
	xsd__string key;	// �Զ������ݼ�ֵ �û�id + ��keyֵ��ΪΨһ��ʶ
	int dataType;		// ��������.
	int fragNo;			// �ֿ����. ( ��Ϊ���ݿ��ܳ���һ����������С, ���Ի�ְ� )
};
class cusdk__GetUserDataRsp /* : public cusdk__BaseRsp */
{
	xsd__string key;	// �Զ������ݼ�ֵ �û�id + ��keyֵ��ΪΨһ��ʶ ��������,�����ⷽ��
	int dataType;		// ��������.
	int fragNo;			// �ֿ����. ( ��Ϊ���ݿ��ܳ���һ����������С, ���Ի�ְ�  ��������,�����ⷽ�� )

	int allFragNum;		// ��Ƭ����.

	xsd__string value;	// �Զ�������. ������ܴܺ�, �����Զ����豸��

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�û��Զ�������.
int cusdk__GetUserData( cusdk__GetUserDataReq req, cusdk__GetUserDataRsp& rsp );

class cusdk__SetUserDataReq : public cusdk__BaseReq
{
	xsd__string key;	// �Զ������ݼ�ֵ
	int dataType;		// ��������.

	int fragNo;			// �ֿ����. ( ��Ϊ���ݿ��ܳ���һ����������С, ���Ի�ְ�
	int allFragNum;		// ��Ƭ����.

	xsd__string value;	// �Զ�������.
};
class cusdk__SetUserDataRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����û��Զ�������.
int cusdk__SetUserData( cusdk__SetUserDataReq req, cusdk__SetUserDataRsp& rsp );

class cusdk__DelUserDataReq : public cusdk__BaseReq
{
	xsd__string key;	// �Զ������ݼ�ֵ
	int dataType;		// ��������.
};
class cusdk__DelUserDataRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ɾ���û��Զ�������.
int cusdk__DelUserData( cusdk__DelUserDataReq req, cusdk__DelUserDataRsp& rsp );

// �豸��/�豸���ݵ��ļ������ӿ�.
class cusdk__StartExportDeviceDataReq : public cusdk__BaseReq
{	
};

class cusdk__StartExportDeviceDataRsp /* : public cusdk__BaseRsp */
{	
	xsd__string exportFileUrl;	// ����������Url.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ�����û�����/�豸��/�豸 ����.
int cusdk__StartExportDeviceData( cusdk__StartExportDeviceDataReq req, cusdk__StartExportDeviceDataRsp& rsp );

class cusdk__StopExportDeviceDataReq : public cusdk__BaseReq
{	
};

class cusdk__StopExportDeviceDataRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �Ѿ���ɻ�ȡ�����ݵ���,ɾ���ļ�.
int cusdk__StopExportDeviceData( cusdk__StopExportDeviceDataReq req, cusdk__StopExportDeviceDataRsp& rsp );



class cusdk__GetUserDevGroupByGroupReq : public cusdk__BaseReq
{
	xsd__string userId;		// �û�Id. ����ID��Ϊ��ʱ����ѯ���豸����Ϣ��hasDev�Ǹ��û���, �������豸���
	xsd__string parentDevGroupId;// ���豸��Id. ����IDΪ��ʱ, ��ѯ���ڵ��µ����豸�飬��Ϊ��ʱ��ѯָ�������µ��豸�飬ÿ�ν���ѯһ��
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
	xsd__string devTreeId;  //�豸��������Id
};
class cusdk__GetUserDevGroupByGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceGroup > devGroupList;	    // �豸�б�.
	int totalNum;										// �����������豸�������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����û�/�豸����������ȡ��һ���豸���б�.ΪCU������չ���豸���Ź����豸���������ڵ�ʱ�õ��˽ӿ�.�����ο����������豸��,���õ���
int cusdk__GetUserDevGroupByGroup( cusdk__GetUserDevGroupByGroupReq req, cusdk__GetUserDevGroupByGroupRsp& rsp );


class cusdk__GetUserDevByGroupReq : public cusdk__BaseReq
{
	xsd__string userId;		// �û�Id.
	xsd__string devGroupId;	// �豸��Id.
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;		// ��������Ҫ������.
	xsd__string devTreeId; //�豸��Id

	xsd__string beginQueryHint;	// ��ѯ����ʼ��, ��ѯ��־, ���ڷ�ҳ��ѯ. ��beginTokenΪ��ʱ, ����������ŷ�ҳ. 

};
class cusdk__GetUserDevByGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceInfo > devList;	        // �豸�б�.
	int totalNum;								        // �����������豸������.

	xsd__string nextQueryHint;	// ��ѯ��־, ���ڷ�ҳ��ѯ. ��nextQueryHintΪ��ʱ, ����������ŷ�ҳ. 

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����û�/�豸����������ȡ�豸�б�.��CU��������Ҫ�㿪ĳ���豸��֮����ʾ�豸ʱ�����øýӿ�
int cusdk__GetUserDevByGroup( cusdk__GetUserDevByGroupReq req, cusdk__GetUserDevByGroupRsp& rsp );


class cusdk__GetUserDevGroupReq : public cusdk__BaseReq
{
	xsd__string userId;		// �û�Id.
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;		    // ��������Ҫ����Ŀ.
	xsd__string devTreeId;  //�豸��������Id
};
class cusdk__GetUserDevGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceGroup > devGroupList;	    // �豸���б�.
	int totalNum;										// ���������豸�������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: һ���Ի�ȡ�û��������豸���б�.
int cusdk__GetUserDevGroup( cusdk__GetUserDevGroupReq req, cusdk__GetUserDevGroupRsp& rsp );

class cusdk__GetDomainDevGroupReq : public cusdk__BaseReq
{
    xsd__string userId;		// �û�Id.
	xsd__string domainId;	// ��Id.
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;		    // ��������Ҫ����Ŀ.
};
class cusdk__GetDomainDevGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceGroup > devGroupList;	    // �豸�б�.
	int totalNum;								        // �����������豸������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ���������е��豸���б�, ������ͬ���������豸��.. 
int cusdk__GetDomainDevGroup( cusdk__GetDomainDevGroupReq req, cusdk__GetDomainDevGroupRsp& rsp );

class cusdk__GetUserDevReq : public cusdk__BaseReq
{
	xsd__string userId;		// �û�Id.
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;		    // ��������Ҫ����Ŀ.
	xsd__string devTreeId; //�豸��Id
};
class cusdk__GetUserDevRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceInfo > devList;	        // �豸�б�.
	int totalNum;										// �����������豸������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�û����е��豸�б�. ���ο���ʱʹ��, �ó�������Ա�û����Ի�ȡ����ƽ̨��Ȩ�޵������豸.
int cusdk__GetUserDev( cusdk__GetUserDevReq req, cusdk__GetUserDevRsp& rsp );


class cusdk__GetDevByDomainReq : public cusdk__BaseReq
{
	xsd__string domainId;	// ��Id
    xsd__string userId;		// �û�Id.
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;		    // ��������Ҫ������.
};
class cusdk__GetDevByDomainRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceInfo > devList;	        // �豸�б�.
	int totalNum;								        // �����������豸������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����������ȡ�û��豸�б�.
int cusdk__GetUserDevByDomain( cusdk__GetDevByDomainReq req, cusdk__GetDevByDomainRsp& rsp );

// ��ѯ �豸�� - �豸(��ƵԴ/�澯ͨ��) �Ĺ�ϵ ���ο���ֱ�ӻ�ȡ�豸�б��ʱ����Ҫ������ܸ㵽�豸����Ϣ..
class cusdk__GetDeviceGroupDeviceRelationsReq : public cusdk__BaseReq
{
    xsd__string userId;		// �û�Id.
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};

class cusdk__GetDeviceGroupDeviceRelationsRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceGroupRelation > devGroupRelationList;	// �豸��ϵ�б�.
	int totalNum;									// �����������豸���豸��ϵ������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�����豸��/�豸��ϵ. ���ο���ֱ�ӻ�ȡ�豸�б���������Ҫ�豸������, ��������Ҫ���.
int cusdk__GetDeviceGroupDeviceRelations( cusdk__GetDeviceGroupDeviceRelationsReq req, cusdk__GetDeviceGroupDeviceRelationsRsp& rsp );

// �豸��������.
class cusdk__CreateDeviceGroupReq : public cusdk__BaseReq
{
	cusdk__DeviceGroupInfo deviceGroupInfo;	// id����Ҫ��, �ɷ���������.
};

class cusdk__CreateDeviceGroupRsp /* : public cusdk__BaseRsp */
{
	xsd__string deviceGroupId;	        // �豸��ID

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����豸��, .
int cusdk__CreateDeviceGroup( cusdk__CreateDeviceGroupReq req, cusdk__CreateDeviceGroupRsp& rsp );

class cusdk__DelDeviceGroupReq : public cusdk__BaseReq
{
	xsd__string deviceGroupId;//��ɾ���ķ���Id
	xsd__string devTreeId; //�豸��Id
};
class cusdk__DelDeviceGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< xsd__string > conflictedNames;        //ɾ���豸��ʱ�����������������б�, �����ʾ10��. ������ΪERR_TAS_EXIST_SAMENAME_RECORDʱ��Ч

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ɾ���豸��.
int cusdk__DelDeviceGroup( cusdk__DelDeviceGroupReq req, cusdk__DelDeviceGroupRsp& rsp );

class cusdk__ModifyDeviceGroupReq : public cusdk__BaseReq
{
	cusdk__DeviceGroupInfo deviceGroupInfo;
};
class cusdk__ModifyDeviceGroupRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸��豸��, �����豸������˽ṹ.
int cusdk__ModifyDeviceGroup( cusdk__ModifyDeviceGroupReq req, cusdk__ModifyDeviceGroupRsp& rsp );

class cusdk__DeviceGroupAddDeviceReq : public cusdk__BaseReq
{
	std::vector<cusdk__DeviceGroupRelation> relationList;
	xsd__string srcDevTreeId; //��ʶ���֮ǰ����ƵԴ�������豸��Id
	xsd__string dstDevTreeId; //��ʶ���֮����ƵԴ�������豸��Id
};
class cusdk__DeviceGroupAddDeviceRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �豸������豸/��ƵԴ/�澯����ͨ��.
int cusdk__DeviceGroupAddDevice( cusdk__DeviceGroupAddDeviceReq req, cusdk__DeviceGroupAddDeviceRsp& rsp );

class cusdk__DeviceGroupAddGroupReq : public cusdk__BaseReq
{
	xsd__string srcGrpId; //��ʶ���֮ǰ,��ƵԴ�����豸����
	xsd__string dstGrpId; //��ʶ���֮��,��ƵԴ�����豸����
	xsd__string srcDevTreeId; //��ʶ���֮ǰ����ƵԴ�������豸��Id
	xsd__string dstDevTreeId; //��ʶ���֮����ƵԴ�������豸��Id

	bool filterOffline;		// �Ƿ��˳��������豸.
	bool filterExist;		// �Ƿ��˳��Ѿ���Ŀ�����ϴ��ڵ��豸.

	xsd__int GBDeviceGroupType;		// ��������ʱ���������, ����� cusdk__EGBDeviceGroupType. +by lzx@2017-12-14 V2R2B3SP1
};

class cusdk__DeviceGroupAddGroupRsp /* : public cusdk__BaseRsp */
{
	xsd__string session;
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ������
};

//�Է������ʽ�����������������ƵԴ
int cusdk__DeviceGroupAddGroup( cusdk__DeviceGroupAddGroupReq req, cusdk__DeviceGroupAddGroupRsp& rsp );

class cusdk__DeviceGroupDelDeviceReq : public cusdk__BaseReq
{
	std::vector<cusdk__DeviceGroupRelation> relationList;
	xsd__string devTreeId; //��ƵԴ�������豸��Id
};
class cusdk__DeviceGroupDelDeviceRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �豸��ɾ���豸/��ƵԴ/�澯����ͨ��.
int cusdk__DeviceGroupDelDevice( cusdk__DeviceGroupDelDeviceReq req, cusdk__DeviceGroupDelDeviceRsp& rsp );


// �û�������..


class cusdk__GetUserListReq : public cusdk__BaseReq
{
    xsd__string userId;		// �û�Id.    ���ڴ˽ӿ���cusdk__GetUserInfo��ӦTASͬһ����ѯ�û��ӿڣ����Ա�����CUָ����ѯ����
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__GetUserListRsp /* : public cusdk__BaseRsp */
{
	std::vector<cusdk__UserInfo> userInfoList;
	int totalNum;										// �����������û�������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��ǰ�û�ӵ�й���Ȩ�޵����û��б�.�����û�����
int cusdk__GetUserList( cusdk__GetUserListReq req, cusdk__GetUserListRsp& rsp );

class cusdk__GetUserGroupListReq : public cusdk__BaseReq
{
    xsd__string userId;		// �û�Id.    ���ڴ˽ӿ���cusdk__GetUserGroupInfo��ӦTASͬһ����ѯ�û��ӿڣ����Ա�����CUָ����ѯ����
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__GetUserGroupListRsp/* : public cusdk__BaseRsp*/
{
	std::vector<cusdk__UserGroup> userGroupList;
	int totalNum;										// �����������û��������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��ǰ�û�ӵ�й���Ȩ�޵��û����б�.
int cusdk__GetUserGroupList( cusdk__GetUserGroupListReq req, cusdk__GetUserGroupListRsp& rsp );

// �û���.
class cusdk__CreateUserGroupReq : public cusdk__BaseReq
{
	cusdk__UserGroup userGroupInfo;	// id����Ҫ��, �ɷ���������.
};
class cusdk__CreateUserGroupRsp/* : public cusdk__BaseRsp*/
{
	xsd__string userGroupId;	    // �û�����Ϣ

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����û���, .
int cusdk__CreateUserGroup( cusdk__CreateUserGroupReq req, cusdk__CreateUserGroupRsp& rsp );

class cusdk__DelUserGroupReq : public cusdk__BaseReq
{
	xsd__string userGroupId;	
};
class cusdk__DelUserGroupRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ������,��Ӧ���Ȳ�ѯ�ӿڣ�QueryOperationProgress
};
//gsoap cusdk service method-documentation: ɾ���û���, .
int cusdk__DelUserGroup( cusdk__DelUserGroupReq req, cusdk__DelUserGroupRsp& rsp );

class cusdk__ModifyUserGroupReq : public cusdk__BaseReq
{
	cusdk__UserGroup userGroupInfo;
};
class cusdk__ModifyUserGroupRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸��û�����Ϣ .
int cusdk__ModifyUserGroup( cusdk__ModifyUserGroupReq req, cusdk__ModifyUserGroupRsp& rsp );

// �û�
class cusdk__CreateUserReq : public cusdk__BaseReq
{
	cusdk__UserInfo userInfo;	// id����Ҫ��, �ɷ���������.
};
class cusdk__CreateUserRsp/* : public cusdk__BaseRsp*/
{
	xsd__string userId;		    // ������Ϣ

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����û� .
int cusdk__CreateUser( cusdk__CreateUserReq req, cusdk__CreateUserRsp& rsp );

class cusdk__DelUserReq : public cusdk__BaseReq
{
	xsd__string userId;	
};
class cusdk__DelUserRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ����������Ӧ���Ȳ�ѯ�ӿڣ�QueryOperationProgress
};
//gsoap cusdk service method-documentation: ɾ���û� .
int cusdk__DelUser( cusdk__DelUserReq req, cusdk__DelUserRsp& rsp );

class cusdk__ModifyUserReq : public cusdk__BaseReq
{
	cusdk__UserInfo userInfo;	
};
class cusdk__ModifyUserRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸��û���Ϣ .
int cusdk__ModifyUser( cusdk__ModifyUserReq req, cusdk__ModifyUserRsp& rsp );

class cusdk__ChangePasswordReq : public cusdk__BaseReq
{
	xsd__string oldPassword;		// ������.
	xsd__string newPassword;		// ������.
	time_t passwordOverdueTime;		// �����ֹʱ��.

	xsd__string passwordAlgorithm;	// ����ʹ�õļ����㷨. �� Ϊ����, "SM3_Base64" ��ʾSM3ȡɢ��ֵ��Base64����Ϊ�ַ���.
};
class cusdk__ChangePasswordRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸��û����� .
int cusdk__ChangePassword( cusdk__ChangePasswordReq req, cusdk__ChangePasswordRsp& rsp );

// �û���/�û� ���豸����.
// �����ǵ�ǰ�û��������û�/�û�����в�����Ϊ�����ֲ����������û������飬������cusdk__EUserOrUserGroup
class cusdk__UserGetDeviceByGroupReq : public cusdk__BaseReq
{
	xsd__string id;						// �û����û����Id. ����û����û����Id�����ظ�, �ǿ���ֻ��һ��.
	enum cusdk__EUserOrUserGroup type;	// ������Ŀ�����û������û���.
	xsd__string deviceGroupId;			// �豸��Id.
	xsd__int beginIndex;				// ��ʼ���.
	xsd__int num;						// ��������Ҫ������.
};
class cusdk__UserGetDeviceByGroupRsp/* : public cusdk__BaseRsp*/
{
	std::vector<cusdk__AssignDeviceInfo> deviceList;	// �豸������Ϣ.
	int totalNum;										// �����������豸������Ϣ������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�û�/�û��黮����豸/��ƵԴ/�澯����ͨ��.�����û������ܵ��û��豸����ʱʹ��
int cusdk__UserGetDeviceByGroup( cusdk__UserGetDeviceByGroupReq req, cusdk__UserGetDeviceByGroupRsp& rsp );

class cusdk__UserAddDeviceReq : public cusdk__BaseReq
{
	xsd__string id;		// �û����û����Id. ����û����û����Id�����ظ�, �ǿ���ֻ��һ��.
	enum cusdk__EUserOrUserGroup type;	// ������Ŀ�����û������û���.
	std::vector<cusdk__AssignDeviceInfo> deviceList;
};
class cusdk__UserAddDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �û�/�û��黮���豸/��ƵԴ/�澯����ͨ��.
int cusdk__UserAddDevice( cusdk__UserAddDeviceReq req, cusdk__UserAddDeviceRsp& rsp );

class cusdk__UserDelDeviceReq : public cusdk__BaseReq
{
	xsd__string id;		// �û����û����Id. ����û����û����Id�����ظ�, �ǿ���ֻ��һ��.
	enum cusdk__EUserOrUserGroup type;	// ������Ŀ�����û������û���.
	std::vector<cusdk__AssignDeviceInfo> deviceList;
};
class cusdk__UserDelDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �û�/�û���ɾ���豸/��ƵԴ/�澯����ͨ��.
int cusdk__UserDelDevice( cusdk__UserDelDeviceReq req, cusdk__UserDelDeviceRsp& rsp );

class cusdk__UserGetTVWallReq : public cusdk__BaseReq
{
	xsd__string id;						// �û����û����Id. ����û����û����Id�����ظ�, �ǿ���ֻ��һ��.
	enum cusdk__EUserOrUserGroup type;	// ������Ŀ�����û������û���.
	xsd__int beginIndex;				// ��ʼ���.
	xsd__int num;						// ��������Ҫ������.
};
class cusdk__UserGetTVWallRsp/* : public cusdk__BaseRsp*/
{
	std::vector<xsd__string> tvWallIdList;	// �Ѿ������˵ĵ���ǽId.
	int totalNum;										// ���������ĵ���ǽ���������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�û�/�û��黮��ĵ���ǽ.�����û������ܵ��û��豸����ʱʹ��
int cusdk__UserGetTVWall( cusdk__UserGetTVWallReq req, cusdk__UserGetTVWallRsp& rsp );

class cusdk__UserAddTVWallReq : public cusdk__BaseReq
{
	xsd__string id;		// �û����û����Id. ����û����û����Id�����ظ�, �ǿ���ֻ��һ��.
	enum cusdk__EUserOrUserGroup type;	// ������Ŀ�����û������û���.
	std::vector<xsd__string> tvwallList;
};
class cusdk__UserAddTVWallRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �û�/�û��黮���豸/��ƵԴ/�澯����ͨ��.
int cusdk__UserAddTVWall( cusdk__UserAddTVWallReq req, cusdk__UserAddTVWallRsp& rsp );

class cusdk__UserDelTVWallReq : public cusdk__BaseReq
{
	xsd__string id;		// �û����û����Id. ����û����û����Id�����ظ�, �ǿ���ֻ��һ��.
	enum cusdk__EUserOrUserGroup type;	// ������Ŀ�����û������û���.
	xsd__string tvwallId;
};
class cusdk__UserDelTVWallRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �û�/�û���ɾ���豸/��ƵԴ/�澯����ͨ��.
int cusdk__UserDelTVWall( cusdk__UserDelTVWallReq req, cusdk__UserDelTVWallRsp& rsp );

class cusdk__UserModifyDeviceReq : public cusdk__BaseReq
{
	xsd__string id;		// �û����û����Id. ����û����û����Id�����ظ�, �ǿ���ֻ��һ���ӿ�.
	enum cusdk__EUserOrUserGroup type;	// ������Ŀ�����û������û���.
	std::vector<cusdk__AssignDeviceInfo> deviceList;
};
class cusdk__UserModifyDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �û�/�û����޸��豸/��ƵԴ/�澯����ͨ����Ȩ��.
int cusdk__UserModifyDevice( cusdk__UserModifyDeviceReq req, cusdk__UserModifyDeviceRsp& rsp );

//������ʱ��Ȩ����ֻ�г�������Ա����ʹ�ã�������ʱ��Ȩ������򣬲���������û�
class cusdk__GetAuthorizationDeviceReq : public cusdk__BaseReq
{
	xsd__int beginIndex;				// ��ʼ���.
	xsd__int num;						// ��������Ҫ������.
};
class cusdk__GetAuthorizationDeviceRsp/* : public cusdk__BaseRsp*/
{
	std::vector<cusdk__AuthorizationDeviceInfo> authorizedDeviceList;		// ����Ȩ�豸�б�
	int totalNum;															// �����������豸�������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��ʱ��Ȩ�豸�б�.
int cusdk__GetAuthorizationDevice( cusdk__GetAuthorizationDeviceReq req, cusdk__GetAuthorizationDeviceRsp& rsp );

class cusdk__AddAuthorizationDeviceReq : public cusdk__BaseReq
{
	cusdk__AuthorizationDeviceInfo authorizedDev;		// ����Ȩ�豸. ���еĴ���ʱ����ƽ̨����
};
class cusdk__AddAuthorizationDeviceRsp/* : public cusdk__BaseRsp*/
{
    time_t	createTime;							        // ����ʱ��.��TAS����

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����ʱ��Ȩ�豸.
int cusdk__AddAuthorizationDevice( cusdk__AddAuthorizationDeviceReq req, cusdk__AddAuthorizationDeviceRsp& rsp );

class cusdk__DelAuthorizationDeviceReq : public cusdk__BaseReq
{
	cusdk__AuthorizationDeviceInfo authorizedDev;		// ����Ȩ�豸.
};
class cusdk__DelAuthorizationDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ɾ����ʱ��Ȩ�豸.
int cusdk__DelAuthorizationDevice( cusdk__DelAuthorizationDeviceReq req, cusdk__DelAuthorizationDeviceRsp& rsp );

class cusdk__ModifyAuthorizationDeviceReq : public cusdk__BaseReq
{
	cusdk__AuthorizationDeviceInfo authorizedDev;		// ����Ȩ�豸.
};
class cusdk__ModifyAuthorizationDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸���ʱ��Ȩ�豸.
int cusdk__ModifyAuthorizationDevice( cusdk__ModifyAuthorizationDeviceReq req, cusdk__ModifyAuthorizationDeviceRsp& rsp );


class cusdk__GetDeviceBaseInfoReq : public cusdk__BaseReq
{
	xsd__string devId;
};
class cusdk__GetDeviceBaseInfoRsp/* : public cusdk__BaseRsp*/
{
	cusdk__DeviceInfo devInfo;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�����豸����Ϣ. ��Ȩ����Ϣ.
int cusdk__GetDeviceBaseInfo( cusdk__GetDeviceBaseInfoReq req, cusdk__GetDeviceBaseInfoRsp& rsp );



class cusdk__GetRCSSysParamReq : public cusdk__BaseReq
{	
};
class cusdk__GetRCSSysParamRsp/* : public cusdk__BaseRsp*/
{
	enum cusdk__ERS_CoverPolicy coverPolicy;		// ¼�񸲸ǲ��ԡ�

	bool maxLockedRecordDiskSpaceCapacitySupported;	// �Ƿ�֧�� maxLockedRecordDiskSpaceCapacity ����, ���ڼ�����. +by lzx@2018-03-06 V2R4B1
	int maxLockedRecordDiskSpaceCapacity;			// ����¼���ڴ����е����ռ��. +by lzx@2018-03-06 V2R4B1

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ���ǲ���.
int cusdk__GetRCSSysParam( cusdk__GetRCSSysParamReq req, cusdk__GetRCSSysParamRsp& rsp );

class cusdk__SetRCSSysParamReq : public cusdk__BaseReq
{
	enum cusdk__ERS_CoverPolicy coverPolicy;

	//bool maxLockedRecordDiskSpaceCapacitySupported;	// �Ƿ�֧�� maxLockedRecordDiskSpaceCapacity ����, ���ڼ�����. +by lzx@2018-03-06 V2R4B1
	//int maxLockedRecordDiskSpaceCapacity;			// ����¼���ڴ����е����ռ��. +by lzx@2018-03-06 V2R4B1
};
class cusdk__SetRCSSysParamRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ���ǲ���.
int cusdk__SetRCSSysParam( cusdk__SetRCSSysParamReq req, cusdk__SetRCSSysParamRsp& rsp );

class cusdk__GetNruListReq : public cusdk__BaseReq
{
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__GetNruListRsp/* : public cusdk__BaseRsp*/
{
	int totalNum;										// ����������Nru������.
	std::vector<cusdk__NruInfo> nruList;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ҳ��ȡNRU/�����б�.
int cusdk__GetNruList( cusdk__GetNruListReq req, cusdk__GetNruListRsp& rsp );

class cusdk__GetDiskListReq : public cusdk__BaseReq
{
	xsd__string nruId;		// ��������NRU Id.

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__GetDiskListRsp/* : public cusdk__BaseRsp*/
{
	std::vector<cusdk__DiskInfo> diskList;	// �����б�.
	int totalNum;							// ���������Ĵ��̵�����.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ҳ��ȡ�����б�.
int cusdk__GetDiskList( cusdk__GetDiskListReq req, cusdk__GetDiskListRsp& rsp );

class cusdk__GetPuChnBindingReq : public cusdk__BaseReq
{
	xsd__string nruId;	// NRU��UUID
	xsd__string diskMountPath;	// ������mount·����

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__GetPuChnBindingRsp/* : public cusdk__BaseRsp*/
{	
	xsd__string nruId;	// NRU��UUID	����
	xsd__string diskMountPath;	// ������mount·��������

	int totalNum;	// �����ϰ󶨵��豸����.
	std::vector<cusdk__DevChannel> channelList;	// �󶨵�ͨ���б�	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�󶨹�ϵ�б�.
int cusdk__GetPuChnBinding( cusdk__GetPuChnBindingReq req, cusdk__GetPuChnBindingRsp& rsp );

class cusdk__BindPuReq : public cusdk__BaseReq
{
	xsd__string nruId;	// NRU��UUID
	xsd__string diskMountPath;	// ������mount·����
	std::vector<cusdk__DevChannel> channelList;	// �󶨵�ͨ���б�
};
class cusdk__BindPuRsp/* : public cusdk__BaseRsp*/
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation:  ��Ӱ󶨹�ϵ
int cusdk__BindPu( cusdk__BindPuReq req, cusdk__BindPuRsp& rsp );

class cusdk__UnbindPuReq : public cusdk__BaseReq
{
	xsd__string nruId;	// NRU��UUID
	xsd__string diskMountPath;	// ������mount·����
	std::vector<cusdk__DevChannel> channelList;	// ����󶨵�ͨ���б�
};
class cusdk__UnbindPuRsp/* : public cusdk__BaseRsp*/
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation:  ɾ���󶨹�ϵ
int cusdk__UnbindPu( cusdk__UnbindPuReq req, cusdk__UnbindPuRsp& rsp );

class cusdk__QueryRecordDaysReq : public cusdk__BaseReq
{
	std::vector<cusdk__DevChannel> devChnList;		// �豸ͨ��
	enum cusdk__ERecordType recordType;		// �ط�����, ƽ̨/ǰ�� ¼��.
	cusdk__TimeRange timeRange;						// ��ѯʱ��Ρ�
	xsd__string recordDomainName;					// ¼���������������Ϊ����
};
class cusdk__QueryRecordDaysRsp /* : public cusdk__BaseRsp */
{		
	std::vector<cusdk__OnePuChnRecDays> recordStateInDaysList;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation:  ��ѯһ��ʱ����������ƽ̨¼��
int cusdk__QueryRecordDays( cusdk__QueryRecordDaysReq req, cusdk__QueryRecordDaysRsp& rsp );

class cusdk__QueryRecordReq : public cusdk__BaseReq
{
	enum cusdk__ERecordType recordType;		// �ط�����, ƽ̨/ǰ�� ¼��.
	cusdk__DevChannel devChn;				// �豸ͨ��
	enum cusdk__ERecordEventType recEventType;		// ¼���¼�����
	cusdk__TimeRange timeRange;				// ��ѯʱ��Ρ�

	xsd__string queryHandle;				// ��ѯ���.Ϊ�ձ�ʾ�²�ѯ

	xsd__string recordDomainName;			// ֻ��ƽ̨¼����Ч. ¼������������������Ϊ����


	bool useVSIPTimeAsGMTTime;				// VSIPǰ��¼���ѯʱ���־, true��Ҫ���VSIP������ʱ�䵱��GMTʱ��.
};
class cusdk__QueryRecordRsp /* : public cusdk__BaseRsp */
{		
	cusdk__DevChannel devChn;								// �豸ͨ��(���࣬������ץ�������⡣��
	std::vector< cusdk__RecDuration > recordDurationList;	// �����

	xsd__string queryHandle;				// ��ѯ���.
	xsd__boolean hasLeft;					// �Ƿ���������¼.

	bool useVSIPTimeAsGMTTime;				// VSIPǰ��¼���ѯʱ���־, true��Ҫ���VSIP������ʱ�䵱��GMTʱ��.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation:  ��ѯ¼��
int cusdk__QueryRecord( cusdk__QueryRecordReq req, cusdk__QueryRecordRsp& rsp );

class cusdk__StartPlatformRecordReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	
};
class cusdk__StartPlatformRecordRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation:  ��ʼ�ֶ�ƽ̨¼��
int cusdk__StartPlatformRecord( cusdk__StartPlatformRecordReq req, cusdk__StartPlatformRecordRsp& rsp );

class cusdk__StopPlatformRecordReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	
};
class cusdk__StopPlatformRecordRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation:  ֹͣ�ֶ�ƽ̨¼��
int cusdk__StopPlatformRecord( cusdk__StopPlatformRecordReq req, cusdk__StopPlatformRecordRsp& rsp );

class cusdk__StartPuRecordReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	
};
class cusdk__StartPuRecordRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation:  ��ʼ�ֶ�ǰ��¼��
int cusdk__StartPuRecord( cusdk__StartPuRecordReq req, cusdk__StartPuRecordRsp& rsp );

class cusdk__StopPuRecordReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	
};
class cusdk__StopPuRecordRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation:  ֹͣ�ֶ�ǰ��¼��
int cusdk__StopPuRecord( cusdk__StopPuRecordReq req, cusdk__StopPuRecordRsp& rsp );


class cusdk__SetupPlayRecordReq : public cusdk__BaseReq
{
	xsd__int playId;								// �ͻ��˵Ĳ�����id��
	cusdk__RecordPlayDuration recordPlayDuration;	// Ҫ���ŵ�¼���.

	xsd__boolean directTrans;								// �Ƿ�ֱ��, ��ǰ��ֱ�ӷ���.
	xsd__boolean directTransFromDeviceDomain;				// ֱ��, ���豸�����򷢳�����.

	int transMode;		// ����μ� cusdk__ETransMode

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// ������󷽵�ý����Ϣ����.
	cusdk__TranscodeMediaFormat transcodeMeidaFormat;		// Ҫ��ƽ̨ת��

//	cusdk__RecDuration recordFileDuration;			// Ҫ���ŵ�¼���ļ���Ϣ. +@2014-08-20
};
class cusdk__SetupPlayRecordRsp /* : public cusdk__BaseRsp */
{
	xsd__string recordPlaySession;				// ¼�񲥷�session, ϵͳȫ�֡�
	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// ����������ý����Ϣ����.

	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// ý����չ��Ϣ.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����ʼ����¼��, ����������Դ, ��������.
int cusdk__SetupPlayRecord( cusdk__SetupPlayRecordReq req, cusdk__SetupPlayRecordRsp& rsp );


class cusdk__PlayRecordReq : public cusdk__BaseReq
{
	xsd__int playId;								// �ͻ��˵Ĳ�����id��
};
class cusdk__PlayRecordRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ����¼�������
int cusdk__PlayRecord( cusdk__PlayRecordReq req, cusdk__PlayRecordRsp& rsp );

class cusdk__VcrCtrlReq : public cusdk__BaseReq
{
	xsd__int playId;				// �ͻ��˵Ĳ�����id��
//	xsd__string recordPlaySession;	// ¼�񲥷�session��

	enum cusdk__EVCRCmd vcrCmd;		// vcr ���
	int seekTime;					// Ҫ��λ��ʱ��㣬���ʱ��, ��λ�롣
	int playSpeed;					// �����ٶȡ�
};
class cusdk__VcrCtrlRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ¼�񲥷�VCR����
int cusdk__VcrCtrl( cusdk__VcrCtrlReq req, cusdk__VcrCtrlRsp& rsp );

class cusdk__GetCurRecordPlayPositionReq : public cusdk__BaseReq
{
	xsd__int playId;							// �ͻ��˵Ĳ�����id��
//	xsd__string recordPlaySession;				// ¼�񲥷�session��
};
class cusdk__GetCurRecordPlayPositionRsp /* : public cusdk__BaseRsp */
{
	xsd__int playId;								// �ͻ��˵Ĳ�����id��
//	xsd__string recordPlaySession;				// ¼�񲥷�session�����࣬Ϊ�˲����ⷽ�㡣

	time_t curTime;			// ¼�񲥷Ž��ȵľ���ʱ�䡣
	cusdk__TimeRange      recordTimeRange;             /*¼��ʱ�䷶Χ*/

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ¼�񲥷ŵĵ�ǰ����
int cusdk__GetCurRecordPlayPosition( cusdk__GetCurRecordPlayPositionReq req, cusdk__GetCurRecordPlayPositionRsp& rsp );

class cusdk__StopPlayRecordReq : public cusdk__BaseReq
{
	xsd__int playId;								// �ͻ��˵Ĳ�����id��

	//xsd__string recordPlaySession;				// ¼�񲥷�session��
};
class cusdk__StopPlayRecordRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ֹͣ����¼��
int cusdk__StopPlayRecord( cusdk__StopPlayRecordReq req, cusdk__StopPlayRecordRsp& rsp );

class cusdk__SetupDownloadRecordReq : public cusdk__BaseReq
{
	xsd__int playId;								// �ͻ��˵Ĳ�����id��

	cusdk__RecordPlayDuration recordPlayDuration;	// Ҫ���ص�¼���.

	xsd__boolean directTrans;						// �Ƿ�ֱ��, ��ǰ��ֱ�ӷ���.
	xsd__boolean directTransFromDeviceDomain;				// ֱ��, ���豸�����򷢳�����.

	int transMode;		// ����μ� cusdk__ETransMode

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// ���ط���ý����Ϣ����.	
};
class cusdk__SetupDownloadRecordRsp /* : public cusdk__BaseRsp */
{	
	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// ����������ý����Ϣ����.

	xsd__string recordFilePath;	// rpctrl��Ҫ�Ĳ����ļ�·��.
	xsd__string recordFileName;	// rpctrl��Ҫ�Ĳ����ļ���.

	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// ý����չ��Ϣ.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��������ʼ¼������, ����������Դ, ��������.
int cusdk__SetupDownloadRecord( cusdk__SetupDownloadRecordReq req, cusdk__SetupDownloadRecordRsp& rsp );

class cusdk__StartDownloadRecordReq : public cusdk__BaseReq
{
	xsd__int playId;
};
class cusdk__StartDownloadRecordRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ����¼�������
int cusdk__StartDownloadRecord( cusdk__StartDownloadRecordReq req, cusdk__StartDownloadRecordRsp& rsp );

class cusdk__StopDownloadRecordReq : public cusdk__BaseReq
{
	xsd__int playId;
};
class cusdk__StopDownloadRecordRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ֹͣ¼������
int cusdk__StopDownloadRecord( cusdk__StopDownloadRecordReq req, cusdk__StopDownloadRecordRsp& rsp );

class cusdk__GetRecordPlanReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;			// �豸ͨ����
};
class cusdk__GetRecordPlanRsp /* : public cusdk__BaseRsp */
{
	cusdk__DevChannel devChn;			// �豸ͨ�������࣬Ϊ�˲����ⷽ�㡣
	cusdk__RecordPlan recordPlan;		// ¼��ƻ���
	int recStreamType;                  // ¼���������͡�0-������1-����1��2-����2��3-����3
	bool supportChangeStreamType;       // ֧���л�¼����������
	xsd__string session;	            // ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ¼��ƻ�
int cusdk__GetRecordPlan( cusdk__GetRecordPlanReq req, cusdk__GetRecordPlanRsp& rsp );

class cusdk__SetRecordPlanReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;			// �豸��ƵԴ��
	int recStreamType;                  // ¼���������͡�0-������1-����1��2-����2��3-����3
	bool supportChangeStreamType;       // ֧���л�¼����������
	cusdk__RecordPlan recordPlan;		// ¼��ƻ���
};
class cusdk__SetRecordPlanRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����¼��ƻ�
int cusdk__SetRecordPlan( cusdk__SetRecordPlanReq req, cusdk__SetRecordPlanRsp& rsp );



// ��ѯ����ǽ�б�
class cusdk__GetAllTVWallReq : public cusdk__BaseReq
{	
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};

class cusdk__GetAllTVWallRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__TVWall > tvWallList;	// ����ǽ�б�.
	int totalNum;											// ����ǽ������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ���е���ǽ.
int cusdk__GetAllTVWall( cusdk__GetAllTVWallReq req, cusdk__GetAllTVWallRsp& rsp );

class cusdk__GetTVWallReq : public cusdk__BaseReq
{	
	xsd__string tvwallId;			// ����ǽId
};

class cusdk__GetTVWallRsp /* : public cusdk__BaseRsp */
{
	cusdk__TVWall tvWallInfo;	// ����ǽ�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ����ǽ��ϸ��Ϣ.
int cusdk__GetTVWall( cusdk__GetTVWallReq req, cusdk__GetTVWallRsp& rsp );

class cusdk__CreateTVWallReq : public cusdk__BaseReq
{
	cusdk__TVWall TVWallInfo;	// id����Ҫ��, �ɷ���������.
};

class cusdk__CreateTVWallRsp /* : public cusdk__BaseRsp */
{
	xsd__string TVWallId;	        // ����ǽID
	time_t creatTime;					// ����ʱ��.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��������ǽ, .
int cusdk__CreateTVWall( cusdk__CreateTVWallReq req, cusdk__CreateTVWallRsp& rsp );

class cusdk__DelTVWallReq : public cusdk__BaseReq
{
	xsd__string TVWallId;
};
class cusdk__DelTVWallRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ɾ������ǽ.
int cusdk__DelTVWall( cusdk__DelTVWallReq req, cusdk__DelTVWallRsp& rsp );

class cusdk__ModifyTVWallReq : public cusdk__BaseReq
{
	cusdk__TVWall TVWallInfo;
};
class cusdk__ModifyTVWallRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸ĵ���ǽ.
int cusdk__ModifyTVWall( cusdk__ModifyTVWallReq req, cusdk__ModifyTVWallRsp& rsp );


// ����.
class cusdk__TVWallStartPlayReq : public cusdk__BaseReq
{	
	//cusdk__DevChannel decoderChn;	// ������ ����ͨ��.
	xsd__string TVWallId;			// ����ǽId.
	int tvId;						// ���ӻ�Id.
	int tvDivId;					// ���ӻ��ֻ���Id.
	cusdk__DevChannel encoderChn;	// ������/IPC ����ƵԴͨ��.	
};

class cusdk__TVWallStartPlayRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	
};
//gsoap cusdk service method-documentation: ����ǽ/������ �ϲ���ͼ��.
int cusdk__TVWallStartPlay( cusdk__TVWallStartPlayReq req, cusdk__TVWallStartPlayRsp& rsp );

class cusdk__TVWallStopPlayReq : public cusdk__BaseReq
{		
	cusdk__DevChannel encoderChn;	// ��ǰ����ͨ��.
	xsd__string TVWallId;			// ����ǽId.
	int tvId;						// ���ӻ�Id.
	int tvDivId;					// ���ӻ��ֻ���Id.
};

class cusdk__TVWallStopPlayRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����ǽ/������ �ϲ���ͼ��.
int cusdk__TVWallStopPlay( cusdk__TVWallStopPlayReq req, cusdk__TVWallStopPlayRsp& rsp );

class cusdk__TVWallWindowPollContrlReq : public cusdk__BaseReq
{	
	//cusdk__DevChannel encoderChn;	// ������/IPC ����ƵԴͨ��.	

	xsd__string TVWallId;			// ����ǽId.
	int tvId;						// ���ӻ�Id.
	int tvDivId;					// ���ӻ��ֻ���Id.

	std::vector< cusdk__TVWallWindowPollStep > pollSteps;	// ��Ѳ�ĸ�������.

	enum cusdk__EVCRCmd pollCmd;	// ��Ѳ��������. ֧�� ��ʼ/ֹͣ/��ͣ/�ָ�.  �Ƿ���Ҫ�½�һ��ö�ٻ�������¼���?
};

class cusdk__TVWallWindowPollContrlRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����ǽ/������ ������Ѳ����.
int cusdk__TVWallWindowPollContrl( cusdk__TVWallWindowPollContrlReq req, cusdk__TVWallWindowPollContrlRsp& rsp );

class cusdk__TVWallSaveSchemeReq : public cusdk__BaseReq
{
	cusdk__TVWallScheme scheme;	// Ҫ�����Ԥ��. id����Ҫ��, �ɷ���������.
};

class cusdk__TVWallSaveSchemeRsp /* : public cusdk__BaseRsp */
{
//	xsd__string schemeId;	        // Ԥ��Id

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����Ԥ��. ͬ��Ԥ������.
int cusdk__TVWallSaveScheme( cusdk__TVWallSaveSchemeReq req, cusdk__TVWallSaveSchemeRsp& rsp );

class cusdk__TVWallDelSchemeReq : public cusdk__BaseReq
{
	xsd__string tvwallId;		// ��������ǽId.
	xsd__string schemeName;		// Ԥ������.
//	xsd__string schemeId;	        // Ԥ��Id
};

class cusdk__TVWallDelSchemeRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ɾ��Ԥ��.
int cusdk__TVWallDelScheme( cusdk__TVWallDelSchemeReq req, cusdk__TVWallDelSchemeRsp& rsp );

class cusdk__TVWallLoadSchemeReq : public cusdk__BaseReq
{
	xsd__string tvwallId;		// ��������ǽId.
	xsd__string schemeName;		// Ԥ������.
//	xsd__string schemeId;	        // Ԥ��Id
};

class cusdk__TVWallLoadSchemeRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����Ԥ��.
int cusdk__TVWallLoadScheme( cusdk__TVWallLoadSchemeReq req, cusdk__TVWallLoadSchemeRsp& rsp );

class cusdk__TVWallQuerySchemeReq : public cusdk__BaseReq
{
	xsd__string tvWallId;	// ����ǽId.
};

class cusdk__TVWallQuerySchemeRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__TVWallScheme > schemeList;	// ����ǽ���е�Ԥ���б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ����ǽ��Ԥ��..
int cusdk__TVWallQueryScheme( cusdk__TVWallQuerySchemeReq req, cusdk__TVWallQuerySchemeRsp& rsp );

class cusdk__TVWallSetSchemePollReq : public cusdk__BaseReq
{
	xsd__string tvWallId;
	std::vector< cusdk__TVWallSchemePollStep > pollSteps;	// ����ǽԤ����Ѳ�Ĳ���.
};

class cusdk__TVWallSetSchemePollRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ���õ���ǽԤ����Ѳ�Ĳ���..
int cusdk__TVWallSetSchemePoll( cusdk__TVWallSetSchemePollReq req, cusdk__TVWallSetSchemePollRsp& rsp );

class cusdk__TVWallGetSchemePollReq : public cusdk__BaseReq
{
	xsd__string tvWallId;	// ����ǽId.
};

class cusdk__TVWallGetSchemePollRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__TVWallSchemePollStep > pollSteps;	// ����ǽԤ����Ѳ�Ĳ���.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ����ǽ��Ԥ����Ѳ����..
int cusdk__TVWallGetSchemePoll( cusdk__TVWallGetSchemePollReq req, cusdk__TVWallGetSchemePollRsp& rsp );

class cusdk__TVWallSchemePollControlReq : public cusdk__BaseReq
{
	xsd__string tvWallId;	// ����ǽId.
	enum cusdk__EVCRCmd pollCmd;	// ��Ѳ��������. ֧�� ��ʼ/ֹͣ/��ͣ/�ָ�.  �Ƿ���Ҫ�½�һ��ö�ٻ�������¼���?
};

class cusdk__TVWallSchemePollControlRsp /* : public cusdk__BaseRsp */
{
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����ǽ��Ԥ����Ѳ����.
int cusdk__TVWallSchemePollControl( cusdk__TVWallSchemePollControlReq req, cusdk__TVWallSchemePollControlRsp& rsp );

class cusdk__SetTVDivNumReq : public cusdk__BaseReq
{	
	xsd__string tvwallId;	// ����ǽId
	int tvId;		// �������.
	int tvDivNum;	// ���ӷֻ�����Ŀ	����� cusdk__ETVDivStyle
};

class cusdk__SetTVDivNumRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����豸ͨ������.
int cusdk__SetTVDivNum( cusdk__SetTVDivNumReq req, cusdk__SetTVDivNumRsp& rsp );

/*�������������ն˿���Ϣ*/
class cusdk__DecMediaPortInfo
{
	xsd__string deviceIP;      //�豸IP������������CU���������յ�ַ��
	int rtpVideoPort;          //������RTP��Ƶ���ն˿�
	int rtpAudioPort;          //������RTP��Ƶ���ն˿�
	int rtcpVideoPort;         //������RTCP��Ƶ���Ͷ˿�
	int rtcpAudioPort;         //������RTCP��Ƶ���Ͷ˿�
};
/*���ص��������ն˿���Ϣ*/
class cusdk__LocalMediaPortInfo : public cusdk__DecMediaPortInfo
{
	xsd__string mediaStreamType; //�������ͣ�ES��PS��TS�ȵȣ�
	xsd__string manufacture;	// ����.
};
class cusdk__PlayRecordOnTVWallReq : public cusdk__BaseReq
{
	xsd__string TVWallId;			// ����ǽId.
	int tvId;						// ���ӻ�Id.
	int tvDivId;					// ���ӻ��ֻ���Id.

	cusdk__RecordPlayDuration recordPlayDuration;	// Ҫ���ŵ�¼����б�.
	cusdk__LocalMediaPortInfo localMediaPortInfo;   //�����������ն˿���Ϣ
};
class cusdk__PlayRecordOnTVWallRsp /* : public cusdk__BaseRsp */
{
	
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.

	cusdk__DecMediaPortInfo decMediaPortInfo;   //�������������ն˿���Ϣ

	long errorCode;
};
//gsoap cusdk service method-documentation: ����¼�񵽵���ǽ.
int cusdk__PlayRecordOnTVWall( cusdk__PlayRecordOnTVWallReq req, cusdk__PlayRecordOnTVWallRsp& rsp );


class cusdk__StopPlayRecordOnTVWallReq : public cusdk__BaseReq
{
	xsd__string TVWallId;			// ����ǽId.
	int tvId;						// ���ӻ�Id.
	int tvDivId;					// ���ӻ��ֻ���Id.
};
class cusdk__StopPlayRecordOnTVWallRsp /* : public cusdk__BaseRsp */
{
	
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ֹͣ����ǽ�ϲ��ŵ�¼��.
int cusdk__StopPlayRecordOnTVWall( cusdk__StopPlayRecordOnTVWallReq req, cusdk__StopPlayRecordOnTVWallRsp& rsp );

// class cusdk__GetPlayRecordOnTVWallStateReq : public cusdk__BaseReq
// {
// 	xsd__string TVWallId;			// ����ǽId.
// 	int tvId;						// ���ӻ�Id.
// 	int tvDivId;					// ���ӻ��ֻ���Id.
// 
// 	//cusdk__DevChannel decoderChn;														// Ҫ��ȡ�Ľ�����ͨ��.
// };
// class cusdk__GetPlayRecordOnTVWallStateRsp /* : public cusdk__BaseRsp */
// {
// 	std::vector< cusdk__RecordPlayDuration > recordPlayDurationList;	// �ƻ����ŵ�¼����б�.
// 	cusdk__RecordPlayDuration curRecordDuration;						// ��ǰ���ڲ��ŵ�¼���.
// 	xsd__string recordPlaySession;				// ��ǰ��¼�񲥷�session�������� cusdk__GetCurRecordPlayPosition ��ȡ����.
//
//xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
//long errorCode;
// };
// //gsoap cusdk service method-documentation: ��ȡ����ǽ��¼�񲥷�״̬. ���Ҳ������֪ͨ, ����д�ɲ�ѯ..
// int cusdk__GetPlayRecordOnTVWallState( cusdk__GetPlayRecordOnTVWallStateReq req, cusdk__GetPlayRecordOnTVWallStateRsp& rsp );

// 
// class cusdk__PlayRecordOnTVWallVCRCtrlReq : public cusdk__BaseReq
// {
// 	xsd__string recordPlaySession;				// ¼�񲥷�session��
// 
// 	enum cusdk__EVCRCmd vcrCmd;					// vcr ���
// 	int seekTime;								// Ҫ��λ��ʱ��㣬���ʱ��, ��λ�롣
// 	int playSpeed;								// �����ٶȡ������һ��֧��, ��ȷ��.
// };
// class cusdk__PlayRecordOnTVWallVCRCtrlRsp /* : public cusdk__BaseRsp */
// {
//
//xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
//long errorCode;
// };
// //gsoap cusdk service method-documentation: ����¼�񵽵���ǽ.
// int cusdk__PlayRecordOnTVWallVCRCtrl( cusdk__PlayRecordOnTVWallVCRCtrlReq req, cusdk__PlayRecordOnTVWallVCRCtrlRsp& rsp );

class cusdk__MapModuleAttrs
{
	xsd__string key;
	xsd__string value;
};
class cusdk__MapTypeInfo
{
	enum cusdk__EMapType mapType;	// ��ͼ����.
	xsd__string mapVersion;			// �汾.
	xsd__string accessUrl;			// ���ʷ����ַ
	std::vector< cusdk__MapModuleAttrs > moduleAttrs;	// ��ͼģ������
};
class cusdk__GetSupportMapTypesReq : public cusdk__BaseReq
{
};
class cusdk__GetSupportMapTypesRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__MapTypeInfo > supportMapTypes;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��ǰ������֧�ֵĵ�ͼ����.
int cusdk__GetSupportMapTypes( cusdk__GetSupportMapTypesReq req, cusdk__GetSupportMapTypesRsp& rsp );

class cusdk__Map
{
	xsd__string mapId;		// ��ͼId
	xsd__string mapName;	// ��ͼ����.

	enum cusdk__EMapType mapType;	// ��ͼ����.

	xsd__string mapUrl;		// ��ͼͼƬ�ļ�url. ( �ɵ�ͼ��������ʽ ) jpeg��ͼҪ���ϴ�ͼƬ.

	xsd__string parentMapId;	// ����ͼId.

	xsd__string description;	// ����

	// ��ͼ�߽�.
	double minLongitude;	
	double maxLongitude;
	double minLatitude;
	double maxLatitude;

	// ���ŷ�Χ.
	int minScaleLevel;
	int maxScaleLevel;

	double screenCenterLongitude;		// ��Ļ(��ͼ�ؼ�)�м����ʾ��Ĭ�Ͼ���
	double screenCenterLatitude;		// ��Ļ(��ͼ�ؼ�)�м����ʾ��Ĭ��ά��.
	int defaultScaleLevel;				// Ĭ�ϵ����ż���

	enum cusdk__EMapCoordType coordType;// ��������.
	xsd__string version;	// ��ͼ�汾.?

	// ��ͼ����?? �Ƿ���Ҫ?? Google��ͼ�Ƿ�������??
};

class cusdk__GetMapImageUploadUrlReq : public cusdk__BaseReq
{
	xsd__string parentMapId;		// �ϼ���ͼId.
};
class cusdk__GetMapImageUploadUrlRsp /* : public cusdk__BaseRsp */
{
	xsd__string imageUploadUrl;		// ��ͼͼƬ�ϴ�URL

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�ϴ���ͼͼƬ��url.
int cusdk__GetMapImageUploadUrl( cusdk__GetMapImageUploadUrlReq req, cusdk__GetMapImageUploadUrlRsp& rsp );

class cusdk__CreateMapReq : public cusdk__BaseReq
{
	cusdk__Map mapInfo;		// Ҫ�����ĵ�ͼ��Ϣ.
};
class cusdk__CreateMapRsp /* : public cusdk__BaseRsp */
{
	xsd__string mapId;		// �´����ĵ�ͼ��id.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ������ͼ. Google��ͼ����������?. Jpeg��ͼҪ���ϴ�ͼƬ, Ȼ�󴴽�. 
int cusdk__CreateMap( cusdk__CreateMapReq req, cusdk__CreateMapRsp& rsp );

class cusdk__ModifyMapReq : public cusdk__BaseReq
{
	cusdk__Map mapInfo;		// Ҫ�޸ĵĵ�ͼ��Ϣ.
};
class cusdk__ModifyMapRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸ĵ�ͼ.
int cusdk__ModifyMap( cusdk__ModifyMapReq req, cusdk__ModifyMapRsp& rsp );

class cusdk__DelMapReq : public cusdk__BaseReq
{
	xsd__string mapId;
};
class cusdk__DelMapRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ɾ����ͼ.
int cusdk__DelMap( cusdk__DelMapReq req, cusdk__DelMapRsp& rsp );

class cusdk__QueryMapReq : public cusdk__BaseReq
{
	xsd__string mapId;	// ָ����ͼId.
	bool mapIdEnabled;	// �Ƿ���Ч.

	xsd__string parentMapId;	// ����ͼ
	bool parentMapIdEnabled;	// �Ƿ񰴸���ͼ��ѯ.

	enum cusdk__EMapType mapType;	// ��ͼ����.
	bool mapTypeEnabled;

	xsd__string mapName;	// ��ͼ����
	bool mapNameEnabled;	// �Ƿ񰴵�ͼ���Ʋ�ѯ.

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__QueryMapRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;										// ͼԪ��Ŀ.
	std::vector< cusdk__Map > mapList;	// ���е�ͼ�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ��ͼ�б�
int cusdk__QueryMap( cusdk__QueryMapReq req, cusdk__QueryMapRsp& rsp );

class cusdk__MapPosition
{
	double longitude;	// ����.
	double latitude;	// γ��.
};

class cusdk__MapArea
{
	xsd__string areaId;	             	//����ID
	xsd__string parentMapId;	        //����������ͼID
	xsd__string refMapId;	            //�����Ӧ���ӵ�ͼID
	xsd__string layerId;	            //��ͼͼ��ID Ԥ���ֶ�.
	xsd__string layerColor;				// �������ɫ.
	//int styleId;		             	//�������ʽ
	xsd__string customStyle;	        //�����Զ�����ʽ
	double opacity;		             	//������ʾ͸����
	std::vector<cusdk__MapPosition> trace;	//�������������
};

class cusdk__QueryMapAreaReq : public cusdk__BaseReq
{
	xsd__string areaId;	             	//����ID
	bool areaIdEnabled;

	xsd__string parentMapId;
	bool parentMapIdEnabled;

	xsd__string refMapId;
	bool refMapIdEnabled;


	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__QueryMapAreaRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;							// ͼԪ��Ŀ.
	std::vector< cusdk__MapArea > areaList;	// ���е�ͼ�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ��ͼ�б�
int cusdk__QueryMapArea( cusdk__QueryMapAreaReq req, cusdk__QueryMapAreaRsp& rsp );

class cusdk__CreateMapAreaReq : public cusdk__BaseReq
{
	cusdk__MapArea mapAreaInfo;		// Ҫ������������Ϣ.
};
class cusdk__CreateMapAreaRsp /* : public cusdk__BaseRsp */
{
	xsd__string mapAreaId;		// �´����ĵ�ͼ�����id.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ������ͼ����
int cusdk__CreateMapArea( cusdk__CreateMapAreaReq req, cusdk__CreateMapAreaRsp& rsp );

class cusdk__ModifyMapAreaReq : public cusdk__BaseReq
{
	cusdk__MapArea mapAreaInfo;		// Ҫ�޸ĵĵ�ͼ������Ϣ.
};
class cusdk__ModifyMapAreaRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸ĵ�ͼ������Ϣ
int cusdk__ModifyMapArea( cusdk__ModifyMapAreaReq req, cusdk__ModifyMapAreaRsp& rsp );

class cusdk__MapElement
{
	xsd__string id;
	xsd__string name;						// ͼԪ����.
	xsd__string mapId;						// �����ĵ�ͼ��Id.
//	xsd__string elementGroupId;				//����ͼԪ��id.(ͼԪ�鹦�ܲ�һ��ʵ��.)
	xsd__string layerId;					//��ͼͼ��ID Ԥ���ֶ�.


	enum cusdk__EMapElementType type;		// ͼԪ����.

	xsd__string deviceId;					// ͼԪ��Ӧ�豸Id.
	int deviceChannel;						// ͼԪ��Ӧ�豸ͨ��.
	enum cusdk__EDeviceChannelType channelType;	// ͼԪ��Ӧ���豸ͨ��������.
	
	cusdk__MapPosition position;			// ͼԪ����.
	enum cusdk__EMapCoordType coordType;	// ��������.
	
	xsd__string picUrl;						// ͼԪͼƬ·��(����ͼƬ�ļ���?),��һ����Ҫ..

	xsd__string description;				// ����.

	bool storeGPSTrace;						// �Ƿ�洢�켣��Ϣ.

	cusdk__MapPosition marPosition;			// ���������λ��. (����/�޸ĵ�ʱ����Ч)
};

class cusdk__MpsDataVersion
{
	int elementVersion;                //ͼԪ���ݰ汾��
};

class cusdk__CreateMapElementReq : public cusdk__BaseReq
{
	cusdk__MapElement mapEleInfo;		// Ҫ������ͼԪ��Ϣ.
};

class cusdk__CreateMapElementRsp /* : public cusdk__BaseRsp */
{
	xsd__string mapEleId;		// �´����ĵ�ͼͼԪ��id.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ������ͼͼԪ
int cusdk__CreateMapElement( cusdk__CreateMapElementReq req, cusdk__CreateMapElementRsp& rsp );

class cusdk__ModifyMapElementReq : public cusdk__BaseReq
{
	cusdk__MapElement mapElementInfo;		// Ҫ�޸ĵĵ�ͼͼԪ��Ϣ.
};
class cusdk__ModifyMapElementRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸ĵ�ͼͼԪ.
int cusdk__ModifyMapElement( cusdk__ModifyMapElementReq req, cusdk__ModifyMapElementRsp& rsp );

class cusdk__DelMapElementReq : public cusdk__BaseReq
{
	xsd__string mapElementId;
};
class cusdk__DelMapElementRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ɾ����ͼͼԪ.
int cusdk__DelMapElement( cusdk__DelMapElementReq req, cusdk__DelMapElementRsp& rsp );

class cusdk__QueryMapElementReq : public cusdk__BaseReq
{
	xsd__string mapId;		// ͼԪ������ͼId.
	bool mapIdEnabled;


 	xsd__string elementId;	// ͼԪId
 	bool elementIdEnabled;
 
	enum cusdk__EMapElementType elementType;		// ͼԪ����.
	bool elementTypeEnabled;

	xsd__string deviceId;					// ͼԪ��Ӧ�豸Id.
	int deviceChannel;						// ͼԪ��Ӧ�豸ͨ��.
	enum cusdk__EDeviceChannelType channelType;	// ͼԪ��Ӧ���豸ͨ��������.
	bool deviceChannelEnabled;	// �Ƿ����������ѯ.

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__QueryMapElementRsp /* : public cusdk__BaseRsp */
{
	int totalNum;										// ͼԪ��Ŀ.
	std::vector< cusdk__MapElement > mapElementList;	// ���е�ͼ�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ��ͼͼԪ�б�
int cusdk__QueryMapElement( cusdk__QueryMapElementReq req, cusdk__QueryMapElementRsp& rsp );


class cusdk__SearchMapElementReq : public cusdk__BaseReq
{
	xsd__string mapId;		// ͼԪ������ͼId. �ݲ�֧��jpeg��ͼ

	// searchPosEnable �� searchPosEnable����ͬʱΪtrue;

	cusdk__MapPosition upperLeft;	// ��������.
	cusdk__MapPosition bottomRight;	// ��������.
	bool searchPosEnable;

	xsd__string searchKey;	// �����ؼ��� ģ��ƥ��
	bool searchKeyEnabled;

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};

class cusdk__SearchMapElementRsp /* : public cusdk__BaseRsp */
{
	int totalNum;										// ͼԪ��Ŀ.
	std::vector< cusdk__MapElement > mapElementList;	// ���е�ͼ�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����ѯͼԪ/����ͼԪ�ؼ���
int cusdk__SearchMapElement( cusdk__SearchMapElementReq req, cusdk__SearchMapElementRsp& rsp );


class cusdk__GetMapElementImageUploadUrlReq : public cusdk__BaseReq
{
	xsd__string imageName;		// ͼƬ����
};
class cusdk__GetMapElementImageUploadUrlRsp /* : public cusdk__BaseRsp */
{
	xsd__string imageUploadUrl;		// ��ͼͼƬ�ϴ�URL

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�ϴ���ͼͼƬ��url.
int cusdk__GetMapElementImageUploadUrl( cusdk__GetMapElementImageUploadUrlReq req, cusdk__GetMapElementImageUploadUrlRsp& rsp );

class cusdk__QueryMapElementImageListReq : public cusdk__BaseReq
{
	
};
class cusdk__QueryMapElementImageListRsp /* : public cusdk__BaseRsp */
{
	std::vector< xsd__string > mapElementImageUrlList;	// ���е�ͼͼԪͼƬurl�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ��ͼͼԪ�б�
int cusdk__QueryMapElementImageList( cusdk__QueryMapElementImageListReq req, cusdk__QueryMapElementImageListRsp& rsp );

// ��ƫ

class cusdk__MapPositionMar2EarthReq : public cusdk__BaseReq
{
	cusdk__MapPosition marPos;	// ��������.
};
class cusdk__MapPositionMar2EarthRsp /* : public cusdk__BaseRsp */
{
	cusdk__MapPosition earthPos;	// ��������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ƫ�������ת��Ϊ��ȡ����.
int cusdk__MapPositionMar2Earth( cusdk__MapPositionMar2EarthReq req, cusdk__MapPositionMar2EarthRsp& rsp );

class cusdk__MapPositionEarth2MarReq : public cusdk__BaseReq
{
	cusdk__MapPosition earthPos;	// ��������.
};
class cusdk__MapPositionEarth2MarRsp /* : public cusdk__BaseRsp */
{
	cusdk__MapPosition marPos;	// ��������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȷ����ת��Ϊ��ƫ����.
int cusdk__MapPositionEarth2Mar( cusdk__MapPositionEarth2MarReq req, cusdk__MapPositionEarth2MarRsp& rsp );

// ��ȡ�澯�������ݰ汾.
class cusdk__GetEMapDataVersionReq : public cusdk__BaseReq
{
};
class cusdk__GetEMapDataVersionRsp /* : public cusdk__BaseRsp */
{
	cusdk__MpsDataVersion dataVersion;	// ͼԪ���ݰ汾.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ���ӵ�ͼ���ݰ汾
int cusdk__GetEMapDataVersion( cusdk__GetEMapDataVersionReq req, cusdk__GetEMapDataVersionRsp& rsp );

// ��ȡ�澯�������ݰ汾.
class cusdk__GetAlarmLinkDataVersionReq : public cusdk__BaseReq
{
};
class cusdk__GetAlarmLinkDataVersionRsp /* : public cusdk__BaseRsp */
{
	xsd__string dataVersion;	// ��ǰ�����ݰ汾.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�澯�������ݰ汾
int cusdk__GetAlarmLinkDataVersion( cusdk__GetAlarmLinkDataVersionReq req, cusdk__GetAlarmLinkDataVersionRsp& rsp );

// ��ȡȫ����������.
class cusdk__QueryAlarmLinkConfigReq : public cusdk__BaseReq
{
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__QueryAlarmLinkConfigRsp /* : public cusdk__BaseRsp */
{
	int totalNum;												// ����������Ŀ.
	std::vector< cusdk__AlarmLinkConfig > alarmLinkConfigList;	// ���������б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�����б�
int cusdk__QueryAlarmLinkConfig( cusdk__QueryAlarmLinkConfigReq req, cusdk__QueryAlarmLinkConfigRsp& rsp );

// ��ȡ��������
class cusdk__GetAlarmLinkConfigReq : public cusdk__BaseReq
{
	cusdk__AlarmSource alarmSource;	// �澯Դ
};
class cusdk__GetAlarmLinkConfigRsp /* : public cusdk__BaseRsp */
{												
	cusdk__AlarmLinkConfig alarmLinkCoonfig;	// ��������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡĳ���澯Դ�Ĳ�������.
int cusdk__GetAlarmLinkConfig( cusdk__GetAlarmLinkConfigReq req, cusdk__GetAlarmLinkConfigRsp& rsp );

// ������������.
class cusdk__SetAlarmLinkConfigReq : public cusdk__BaseReq
{
	cusdk__AlarmLinkConfig alarmLinkCoonfig;	// ��������.
};
class cusdk__SetAlarmLinkConfigRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ���ò�������
int cusdk__SetAlarmLinkConfig( cusdk__SetAlarmLinkConfigReq req, cusdk__SetAlarmLinkConfigRsp& rsp );

// ���򲼷�.
class cusdk__AlarmLinkArea
{
	xsd__string id;	// ���򲼷�id.
	xsd__string name;	// ������.
	std::vector< cusdk__AlarmSource > alarmLinkSourceList;	// �����еĸ澯Դ�б�.
};

// ��ȡȫ�����򲼷��б�.
class cusdk__QueryAlarmLinkAreaReq : public cusdk__BaseReq
{
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__QueryAlarmLinkAreaRsp /* : public cusdk__BaseRsp */
{
	int totalNum;											// ����������Ŀ.
	std::vector< cusdk__AlarmLinkArea > alarmLinkAreaList;	// ���������б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ����������б�
int cusdk__QueryAlarmLinkArea( cusdk__QueryAlarmLinkAreaReq req, cusdk__QueryAlarmLinkAreaRsp& rsp );

class cusdk__CreateAlarmLinkAreaReq : public cusdk__BaseReq
{
	cusdk__AlarmLinkArea alarmLinkAreaInfo;	// id����Ҫ��, �ɷ���������.
};

class cusdk__CreateAlarmLinkAreaRsp /* : public cusdk__BaseRsp */
{
	xsd__string alarmLinkAreaId;	    // ��������ID

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ������������, .
int cusdk__CreateAlarmLinkArea( cusdk__CreateAlarmLinkAreaReq req, cusdk__CreateAlarmLinkAreaRsp& rsp );

class cusdk__DelAlarmLinkAreaReq : public cusdk__BaseReq
{
	xsd__string alarmLinkAreaId;
};
class cusdk__DelAlarmLinkAreaRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ɾ����������.
int cusdk__DelAlarmLinkArea( cusdk__DelAlarmLinkAreaReq req, cusdk__DelAlarmLinkAreaRsp& rsp );

class cusdk__ModifyAlarmLinkAreaReq : public cusdk__BaseReq
{
	cusdk__AlarmLinkArea alarmLinkAreaInfo;
};
class cusdk__ModifyAlarmLinkAreaRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸���������.
int cusdk__ModifyAlarmLinkArea( cusdk__ModifyAlarmLinkAreaReq req, cusdk__ModifyAlarmLinkAreaRsp& rsp );


// �û���־


class cusdk__UserLog
{
	xsd__string logId;			// ��־���
	xsd__string loginSession;	// ��¼Session.
	xsd__string userId;			// �û����
	xsd__string domainName;		// �û�����
	xsd__string userName;		// �û���
	xsd__string ipAddr;			// ip��ַ. ? uas��û��.
	time_t happenedTime;	// ����ʱ��
	enum cusdk__EUserOperateType operateType;	// ��������. ��Ϊ�����Կ���, ֻ֧�� cusdk__UserOpTypeCuLocalRecord ��֮ǰ����־����, ֮�����־����ʹ�� operatorTypeV2.
	int operateTypeV2;			// ��������, ����� cusdk__UserOpTypeCuLocalRecord. 
	int operateResult;			// �������(������?)
	xsd__string operateDesc;	// ��������
};



class cusdk__QueryUserLogCondition
{	
	xsd__string userId;
	bool userIdEnabled;

	// 	xsd__string userName;
	// 	bool userNameEnabled;

	xsd__string userLoginSession;
	bool userLoginSessionEnabled;

	xsd__string domainName;
	bool domainNameEnabled;

	time_t timeBegin;
	time_t timeEnd;
	bool timeEnabled;

	enum cusdk__EUserOperateType operateType;	// ��������. ��Ϊ�����Կ���, ֻ֧�� cusdk__UserOpTypeCuLocalRecord ��֮ǰ����־����, ֮�����־����ʹ�� operatorTypeV2.
	int operateTypeV2;			// ��������, ����� cusdk__UserOpTypeCuLocalRecord.
	bool operateTypeEnabled;
};

class cusdk__QueryUserLogReq : public cusdk__BaseReq
{	
	cusdk__QueryUserLogCondition condition;

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};

class cusdk__QueryUserLogRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;			// ��־��Ŀ.

	std::vector< cusdk__UserLog > userLogList;	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�û���־�б�.
int cusdk__QueryUserLog( cusdk__QueryUserLogReq req, cusdk__QueryUserLogRsp& rsp );

class cusdk__StartExportUserLogReq : public cusdk__BaseReq
{	
	cusdk__QueryUserLogCondition condition;
	xsd__string characterSet;				// �ַ���.

	xsd__string timeZone;			// ʱ��, ʾ��: ������������ 8,  +8 �����Сʱ, 8.5 +8.5
};

class cusdk__StartExportUserLogRsp /* : public cusdk__BaseRsp */
{	
	xsd__string exportFileUrl;	// ����������Url.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ�����û���־�б�.
int cusdk__StartExportUserLog( cusdk__StartExportUserLogReq req, cusdk__StartExportUserLogRsp& rsp );

class cusdk__GetExportUserLogProcessReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// ����������Url.
};

class cusdk__GetExportUserLogProcessRsp /* : public cusdk__BaseRsp */
{	
	int percentage;		// �������Ȱٷֱ� 1-100%

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�û���־�����Ľ���.
int cusdk__GetExportUserLogProcess( cusdk__GetExportUserLogProcessReq req, cusdk__GetExportUserLogProcessRsp& rsp );

class cusdk__StopExportUserLogReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// ����������Url.
};

class cusdk__StopExportUserLogRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �Ѿ���ɻ�ȡ���û���־����,ɾ���ļ�.
int cusdk__StopExportUserLog( cusdk__StopExportUserLogReq req, cusdk__StopExportUserLogRsp& rsp );

class cusdk__DeviceLog
{
	xsd__string logId;						// ��־���
	xsd__string deviceId;						// �豸���
//	xsd__string deviceDomainName;				// �豸��������
	xsd__string deviceName;					// �豸����
	xsd__string deviceIp;					// �豸IP��ַ.
	xsd__string deviceAlias;					// �豸����
	time_t happenedTime;					// ����ʱ��
	enum cusdk__EDeviceLogType logType;		// ��־����
	xsd__string logDesc;					// ��־����
};

class cusdk__QueryDeviceLogCondition
{	
	xsd__string deviceId;		// �豸Id.
	bool deviceIdEnabled;

	xsd__string deviceName;		// �豸����.
	bool deviceNameEnabled;

	time_t timeBegin;			// ʱ���.
	time_t timeEnd;
	bool timeEnabled;

	enum cusdk__EDeviceLogType logType;	// ��־����.
	bool logTypeEnabled;
};

class cusdk__QueryDeviceLogReq : public cusdk__BaseReq
{	
	cusdk__QueryDeviceLogCondition condition;

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};

class cusdk__QueryDeviceLogRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;			// ����������Ŀ.

	std::vector< cusdk__DeviceLog > deviceLogList;	// �豸��־�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�豸��־.
int cusdk__QueryDeviceLog( cusdk__QueryDeviceLogReq req, cusdk__QueryDeviceLogRsp& rsp );

class cusdk__StartExportDeviceLogReq : public cusdk__BaseReq
{	
	cusdk__QueryDeviceLogCondition condition;
	xsd__string characterSet;				// �ַ���.

	xsd__string timeZone;			// ʱ��, ʾ��: ������������ 8,  +8 �����Сʱ, 8.5 +8.5
};

class cusdk__StartExportDeviceLogRsp /* : public cusdk__BaseRsp */
{	
	xsd__string exportFileUrl;	// ����������Url.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ�����豸��־�б�.
int cusdk__StartExportDeviceLog( cusdk__StartExportDeviceLogReq req, cusdk__StartExportDeviceLogRsp& rsp );

class cusdk__GetExportDeviceLogProcessReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// ����������Url.
};

class cusdk__GetExportDeviceLogProcessRsp /* : public cusdk__BaseRsp */
{	
	int percentage;		// �������Ȱٷֱ� 1-100%

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�豸��־�����Ľ���.
int cusdk__GetExportDeviceLogProcess( cusdk__GetExportDeviceLogProcessReq req, cusdk__GetExportDeviceLogProcessRsp& rsp );

class cusdk__StopExportDeviceLogReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// ����������Url.
};

class cusdk__StopExportDeviceLogRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �Ѿ���ɻ�ȡ���豸��־����,ɾ���ļ�.
int cusdk__StopExportDeviceLog( cusdk__StopExportDeviceLogReq req, cusdk__StopExportDeviceLogRsp& rsp );

//class cusdk__DeleteDeviceLogReq : public cusdk__BaseReq
//{	
//	std::vector< xsd__string > alarmLogIdList;	// Ҫɾ������־Id�б�.
//};
//
//class cusdk__DeleteDeviceLogRsp /* : public cusdk__BaseRsp */
//{	
//
//xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
//long errorCode;
//};
////gsoap cusdk service method-documentation: ɾ���豸��־��־.
//int cusdk__DeleteDeviceLog( cusdk__DeleteDeviceLogReq req, cusdk__DeleteDeviceLogRsp& rsp );

class cusdk__AlarmLog
{
	xsd__string logId;			// ��־���
	xsd__string deviceId;       // �豸���
	xsd__string deviceDomainName; // �豸��������
	xsd__string deviceName;         // �豸����
	xsd__string deviceAlias;        // �豸����
	int alarmChannelId;            // �澯ͨ��
	enum cusdk__EAlarmType alarmType;               // �澯���� 
	enum cusdk__EAlarmStatus alarmStatus;           // �澯״̬
	time_t occurTime;       // �澯ʱ��
	time_t resumeTime;      // �ָ�ʱ��
	xsd__string alarmName;           // �澯����
	xsd__string description;          // �澯����
};

class cusdk__QueryAlarmLogCondition
{	
	xsd__string deviceId;
	bool deviceIdEnabled;

	int alarmChannelId;
	bool alarmChannelIdEnabled;

	xsd__string alarmName;
	bool alarmNameEnabled;

	xsd__string deviceDomainName;
	bool deviceDomainNameEnabled;

	time_t timeBegin;
	time_t timeEnd;
	bool timeEnabled;

	enum cusdk__EAlarmType alarmType;
	bool alarmTypeEnabled;
};

class cusdk__QueryAlarmLogReq : public cusdk__BaseReq
{	
	cusdk__QueryAlarmLogCondition condition;

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};

class cusdk__QueryAlarmLogRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;			// �澯��־��Ŀ.

	std::vector< cusdk__AlarmLog > alarmLogList;	// �豸��־�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�澯��־.
int cusdk__QueryAlarmLog( cusdk__QueryAlarmLogReq req, cusdk__QueryAlarmLogRsp& rsp );

class cusdk__StartExportAlarmLogReq : public cusdk__BaseReq
{	
	cusdk__QueryAlarmLogCondition condition;
	xsd__string characterSet;				// �ַ���.

	xsd__string timeZone;			// ʱ��, ʾ��: ������������ 8,  +8 �����Сʱ, 8.5 +8.5
};

class cusdk__StartExportAlarmLogRsp /* : public cusdk__BaseRsp */
{	
	xsd__string exportFileUrl;	// ����������Url.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ�����澯��־.
int cusdk__StartExportAlarmLog( cusdk__StartExportAlarmLogReq req, cusdk__StartExportAlarmLogRsp& rsp );

class cusdk__GetExportAlarmLogProcessReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// ����������Url.
};

class cusdk__GetExportAlarmLogProcessRsp /* : public cusdk__BaseRsp */
{	
	int percentage;		// �������Ȱٷֱ� 1-100%

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�澯��־�����Ľ���.
int cusdk__GetExportAlarmLogProcess( cusdk__GetExportAlarmLogProcessReq req, cusdk__GetExportAlarmLogProcessRsp& rsp );

class cusdk__StopExportAlarmLogReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// ����������Url.
};

class cusdk__StopExportAlarmLogRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �Ѿ���ɻ�ȡ���澯��־����,ɾ����־�ļ�.
int cusdk__StopExportAlarmLog( cusdk__StopExportAlarmLogReq req, cusdk__StopExportAlarmLogRsp& rsp );


//class cusdk__DeleteAlarmLogReq : public cusdk__BaseReq
//{	
//	std::vector< xsd__string > alarmLogIdList;	// Ҫɾ������־Id�б�.
//};
//
//class cusdk__DeleteAlarmLogRsp /* : public cusdk__BaseRsp */
//{	
//
//xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
//long errorCode;
//};
////gsoap cusdk service method-documentation: ɾ���澯��־.
//int cusdk__DeleteAlarmLog( cusdk__DeleteAlarmLogReq req, cusdk__DeleteAlarmLogRsp& rsp );

class cusdk__GetLogConfigReq : public cusdk__BaseReq
{		
};

class cusdk__GetLogConfigRsp /* : public cusdk__BaseRsp */
{	
	int maxUserLogSavedDays;	// �����û���־��������.
	int maxUserLogSavedNumber;	// �����û���־��������.
	bool enableUserLog;			// �Ƿ������û���־.

	int maxDeviceLogSavedNumber;// �����豸��־��������.
	int maxDeviceLogSavedDays;	// �����豸��־��������.
	bool enableDeviceLog;		// �Ƿ������豸��־.

	int maxAlarmLogSavedNumber;	// ���ĸ澯��־��������.
	int maxAlarmLogSavedDays;	// ���ĸ澯��־��������.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��־����.
int cusdk__GetLogConfig( cusdk__GetLogConfigReq req, cusdk__GetLogConfigRsp& rsp );

class cusdk__SetLogConfigReq : public cusdk__BaseReq
{	
	int maxUserLogSavedDays;	// �����û���־��������.
	int maxUserLogSavedNumber;	// �����û���־��������.
	bool enableUserLog;			// �Ƿ������û���־.

	int maxDeviceLogSavedNumber;// �����豸��־��������.
	int maxDeviceLogSavedDays;	// �����豸��־��������.
	bool enableDeviceLog;		// �Ƿ������豸��־.

	int maxAlarmLogSavedNumber;	// ���ĸ澯��־��������.
	int maxAlarmLogSavedDays;	// ���ĸ澯��־��������.
};
class cusdk__SetLogConfigRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ������־����.
int cusdk__SetLogConfig( cusdk__SetLogConfigReq req, cusdk__SetLogConfigRsp& rsp );




// class cusdk__MediaNetInfo
// {
// 	cusdk__NatInfo videoRtpNatInfo;
// 	cusdk__NatInfo videoRtcpNatInfo;
// 	cusdk__NatInfo audioRtpNatInfo;
// 	cusdk__NatInfo audioRtcpNatInfo;
// };

class cusdk__CallPuReq : public cusdk__BaseReq
{	
	xsd__int encodeChnId;	// �ͻ��˵�����Ƶ����ͨ��Id. 


	xsd__string devId;		// Ҫ���е��豸Id.
	xsd__int devDecChnId;	// Ҫ���е��豸����ͨ��Id.

// 	xsd__int devVideoDecChnId;	// Ҫ���е��豸��Ƶ����ͨ��Id.
// 	xsd__int devAudioDecChnId;	// Ҫ���е��豸ͨ��Id.
		
	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// ���з���ý����Ϣ����.
	
	int transMode;		// ����μ� cusdk__ETransMode
// 	
// 	std::vector< enum cusdk__EPlayMode > supportedCallMode;		// ��֧�ֵĺ���ģʽ. ����������ģʽ����ǰ��
// 
// 	std::vector< cusdk__MediaFormat > supportedVideoFormat;		// ��֧�ֵ���Ƶ�����ʽ. ����Ҫ��չ. �ֱ���, ���ʵȱ�Ҫ�ı��������Ϣ, �ο���׼.
// 	std::vector< cusdk__MediaFormat > supportedAudioFormat;		// ��֧�ֵ���Ƶ�����ʽ.
// 	std::vector<cusdk__MediaNetInfo> localNetInfoList;  // ��ƵԴ�Ķ�IP���	
};

class cusdk__CallPuRsp /* : public cusdk__BaseRsp */
{		
	//std::vector<cusdk__MediaNetInfo> remoteNetInfoList; //ƽ̨Ҳ��IP���

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// Э�̺��ý����Ϣ����. ���Ϊ�վͱ�ʾ����Ҫ��������.
	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// ý����չ��Ϣ.


// 	enum cusdk__EPlayMode callMode;	// Э�̺�ȷ��������Ƶ����ģʽ. ����Ƶ���� or ��Ƶ���� or ��Ƶ����?
// 	cusdk__MediaFormat supportedVideoFormat;		// Э�̺�ȷ������Ƶ�����ʽ. ����Ҫ��չ. �ֱ���, ���ʵȱ�Ҫ�ı��������Ϣ, Ӧ�òο���׼.
// 	cusdk__MediaFormat supportedAudioFormat;		// Э�̺�ȷ������Ƶ�����ʽ.
// 	bool needCreateNewVideoSwitch;		// �Ƿ���Ҫ�����µ���Ƶ.
// 	bool needCreateNewAudioSwitch;		// �Ƿ���Ҫ������Ƶ.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����Ƶ���� ��Ƶ����ǰ��, ��������Ƶ���� CU.
int cusdk__CallPu( cusdk__CallPuReq req, cusdk__CallPuRsp& rsp );

class cusdk__HangUpPuReq : public cusdk__BaseReq
{	
	xsd__int encodeChnId;		// �ͻ��˵���Ƶ����ͨ��Id. 
	
	xsd__string devId;			// ���е��豸Id.
	xsd__int devDecChnId;	// Ҫ���е��豸����ͨ��Id.

// 	xsd__int devVideoDecChnId;	// Ҫ���е��豸��Ƶ����ͨ��Id.
// 	xsd__int devAudioDecChnId;	// ���е��豸ͨ��Id.

//	enum cusdk__EPlayMode callMode;		// Ҫ�Ҷϵ�������Ƶ or ��Ƶ or ��Ƶ? ����ͨ��Id����Чֵ����ʾ��Ч??
};

class cusdk__HangUpPuRsp /* : public cusdk__BaseRsp */
{		
	//int remainAudioSwitch;	// ����Ƶͨ��ʣ�����Ƶ������. ���Ϊ0, ��ֹͣ��Ƶ����ͷ���.
// 	bool needStopVideoSwitch;	// �Ƿ��ֹͣ������Ƶ.
// 	bool needStopAudioSwitch;	// �Ƿ��ֹͣ������Ƶ.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �Ҷ�����Ƶ����
int cusdk__HangUpPu( cusdk__HangUpPuReq req, cusdk__HangUpPuRsp& rsp );



class cusdk__VideoMode
{
	enum cusdk__EVideoFormat videoFormat;	// ��Ƶ�����ʽ. ֻ�������� cusdk__VF_H265 ֮ǰ��ֵ.
	int videoFormatV2;						// ��Ƶ�����ʽ. Ϊ�˼�����,��Ϊint, ֵ�Ķ���μ� cusdk__EVideoFormat.
	enum cusdk__EVideoResolution videoRes;	//ͼ��ֱ���;
	xsd__string videoResStr;				//�ַ���ʾ�ķֱ���. ���#define VSIP_ENTITY_SUPPORT_RESOLUTION_WH 0x3D �������������,��ʹ���ַ����ķֱ���. +by lizhixing@2014��3��28��
};

class cusdk__VideoEncodeParam
{
	cusdk__VideoMode videoMode;	// �����ʽ�ͷֱ���
	int frameRate;			//֡�� 1-25;
	enum cusdk__EVideoQuality quality;		//ͼ������; <��pu\MtCtrl.h>
	int bitRate;			//��������� 64K-4M;
	int interval;			//�ؼ�֡��� 25-2500;
	int brightness;			//���� -127��127
	int contrast;			//�Աȶ� -127��127
	int saturation;			//���Ͷ� -127��127
	int sharpness;			// ���. -127-127
};

class cusdk__AudioMode
{
	int audioFormat;	// ��Ƶ�����ʽ,����μ� cusdk__EAudioFormat
};

class cusdk__AudioEncoderParam
{
	int audioForamt;	// ��Ƶ�����ʽ. ����� cusdk__EAudioFormat
	int volume;			// ����,��Χ 0-25
	bool AECState;		// ���������Ƿ���
	int sampleRate;		// ������.
	int sampleBits;		// ����λ��
	int channleNum;		// ������Ŀ.
	int samplePerFrame;	// ÿ֡����������
};

class cusdk__GetDeviceEncoderParamReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int	encodeChnId;		// ����ͨ��Id.
};

class cusdk__GetDeviceEncoderParamRsp /* : public cusdk__BaseRsp */
{		
	cusdk__VideoEncodeParam  videoEncParam;   //��ǰ��Ƶ�������
	std::vector< cusdk__VideoMode >  suportedVideoModeList;  //����֧�ֵ���Ƶģʽ

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�豸�������.
int cusdk__GetDeviceEncoderParam( cusdk__GetDeviceEncoderParamReq req, cusdk__GetDeviceEncoderParamRsp& rsp );


class cusdk__GetDeviceAudioEncoderParamReq : public cusdk__BaseReq
{
	xsd__string deviceId;	// �豸Id
	int	encodeChnId;		// ����ͨ��Id.
};

class cusdk__GetDeviceAudioEncoderParamRsp /* : public cusdk__BaseRsp */
{
	cusdk__AudioEncoderParam audioEncParam;	// ��ǰ����Ƶ�������.
	std::vector< cusdk__AudioMode > supportedAudioModeList;	// ֧�ֵ���Ƶ��ʽ.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�豸�������.
int cusdk__GetDeviceAudioEncoderParam(cusdk__GetDeviceAudioEncoderParamReq req, cusdk__GetDeviceAudioEncoderParamRsp& rsp);


class cusdk__SetDeviceEncoderParamReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int	encodeChnId;		// ����ͨ��Id.

	cusdk__VideoEncodeParam  videoParam;      //��ǰ��Ƶ�������
};

class cusdk__SetDeviceEncoderParamRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����豸�������.
int cusdk__SetDeviceEncoderParam( cusdk__SetDeviceEncoderParamReq req, cusdk__SetDeviceEncoderParamRsp& rsp );

class cusdk__PuVidSrcOSDItem
{
	int			type;			//��Ļ���ͣ��μ�emPuVidSrcOsdCfgType
	int			id;				//��Ļ��ţ���0��ʼ������Ļ����Ϊ̨��ͺ��ʱ����ʶ̨�������ţ���Ϊ���ڸ澯��Ļʱ����ʶ���ں�
	bool		enabled;		//��Ļʹ��
	xsd__string content;		//��Ļ���ݣ���󳤶�128
};
typedef std::vector< cusdk__PuVidSrcOSDItem > cusdk__PuVidSrcOSDList;

class cusdk__SetPuVidSrcOSDReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int videoSrcId;			// ��ƵԴId.

	cusdk__PuVidSrcOSDList vidSrcOSDList;	// OSD �����б�.
};

class cusdk__SetPuVidSrcOSDRsp /* : public cusdk__BaseRsp */
{		
	std::vector< int > OSDItemErrors;	// ÿ�� cusdk__PuVidSrcOSDItem ����ʱ�Ĵ���.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����豸OSD��Ϣ.
int cusdk__SetPuVidSrcOSD( cusdk__SetPuVidSrcOSDReq req, cusdk__SetPuVidSrcOSDRsp& rsp );

class cusdk__GetPuVidSrcOSDReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int videoSrcId;			// ��ƵԴId.
};

class cusdk__GetPuVidSrcOSDRsp /* : public cusdk__BaseRsp */
{	
	cusdk__PuVidSrcOSDList vidSrcOSDList;	// OSD �����б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�豸OSD��Ϣ.
int cusdk__GetPuVidSrcOSD( cusdk__GetPuVidSrcOSDReq req, cusdk__GetPuVidSrcOSDRsp& rsp );

class cusdk__SetDeviceNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	
	xsd__string deviceName;	// �������.
};

class cusdk__SetDeviceNameRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸��豸������
int cusdk__SetDeviceName( cusdk__SetDeviceNameReq req, cusdk__SetDeviceNameRsp& rsp );

class cusdk__GetCameraProtocolTypeReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int videoSrcId;			// ��ƵԴId.
};

class cusdk__GetCameraProtocolTypeRsp /* : public cusdk__BaseRsp */
{		
	enum cusdk__ECameraProtocolType cameraProtocolType;	// ����ͷЭ������.
	xsd__int cameraId;	// ����ͷ��ַ��/ID

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��ƵԴ������ͷЭ������
int cusdk__GetCameraProtocolType( cusdk__GetCameraProtocolTypeReq req, cusdk__GetCameraProtocolTypeRsp& rsp );

class cusdk__SetCameraProtocolTypeReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int videoSrcId;			// ��ƵԴId.

	enum cusdk__ECameraProtocolType cameraProtocolType;	// ����ͷЭ������.
	xsd__int cameraId;	// ����ͷ��ַ��/ID
};

class cusdk__SetCameraProtocolTypeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �޸���ƵԴ������
int cusdk__SetCameraProtocolType( cusdk__SetCameraProtocolTypeReq req, cusdk__SetCameraProtocolTypeRsp& rsp );

// ����.
class cusdk__Area
{
	int x;
	int y;
	int width;
	int height;
};

// �ƶ�������
class cusdk__MotionDetectAreaConfig
{
	bool enable;			// �Ƿ�����.
	cusdk__Area detectArea;	// ��������.
	int alarmRate;			// 1-100 �澯����������.
	int restoreRate;		// 1-100 �ָ�������.
};

class cusdk__GetMotionDetectConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int videoSrcId;			// ��ƵԴId.
};

class cusdk__GetMotionDetectConfigRsp /* : public cusdk__BaseRsp */
{		
	std::vector< cusdk__MotionDetectAreaConfig > motionDetectAreaConfigList;	// ���е���������б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�ƶ��������.
int cusdk__GetMotionDetectConfig( cusdk__GetMotionDetectConfigReq req, cusdk__GetMotionDetectConfigRsp& rsp );

class cusdk__SetMotionDetectConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int videoSrcId;			// ��ƵԴId.

	std::vector< cusdk__MotionDetectAreaConfig > motionDetectAreaConfigList;	// ���е���������б�.
};

class cusdk__SetMotionDetectConfigRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����ƶ��������.
int cusdk__SetMotionDetectConfig( cusdk__SetMotionDetectConfigReq req, cusdk__SetMotionDetectConfigRsp& rsp );

// ͼ���ڱβ���
class cusdk__ShadeAreaConfig
{
	bool enable;			// �Ƿ�����.
	cusdk__Area shadeArea;	// �ڱε�����.
};

class cusdk__GetShadeConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int videoSrcId;			// ��ƵԴId.
};

class cusdk__GetShadeConfigRsp /* : public cusdk__BaseRsp */
{		
	std::vector< cusdk__ShadeAreaConfig > shadeAreaConfigList;	// ���е��ڱ������б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�ƶ��������.
int cusdk__GetShadeConfig( cusdk__GetShadeConfigReq req, cusdk__GetShadeConfigRsp& rsp );

class cusdk__SetShadeConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int videoSrcId;			// ��ƵԴId.

	std::vector< cusdk__ShadeAreaConfig > shadeAreaConfigList;	// ���е���������б�.
};

class cusdk__SetShadeConfigRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����ƶ��������.
int cusdk__SetShadeConfig( cusdk__SetShadeConfigReq req, cusdk__SetShadeConfigRsp& rsp );

class cusdk__GetDeviceChannelNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	enum cusdk__EDeviceChannelType chnType;	// ͨ������.
	int chnId;	// ͨ��Id.	// -1 ���������ͨ���ı���, �����Ǿ���ĳһ��ͨ��.
};

class cusdk__ChannelName
{
	int chnId;				// ͨ��Id.
	xsd__string chnName;	// ͨ������.
	double longitude;		//��ƵԴ����
	double latitude;		//��ƵԴγ��
};

class cusdk__GetDeviceChannelNameRsp /* : public cusdk__BaseRsp */
{		
	std::vector< cusdk__ChannelName > chnNameList;	// �豸�����и�����

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�豸ͨ������.
int cusdk__GetDeviceChannelName( cusdk__GetDeviceChannelNameReq req, cusdk__GetDeviceChannelNameRsp& rsp );


class cusdk__SetDeviceChannelNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	enum cusdk__EDeviceChannelType chnType;	// ͨ������.
	int chnId;				// ͨ��Id.
	xsd__string chnName;	// ͨ������.
	double longitude;		//��ƵԴ����
	double latitude;		//��ƵԴγ��
};

class cusdk__SetDeviceChannelNameRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	
};
//gsoap cusdk service method-documentation: �����豸ͨ������.
int cusdk__SetDeviceChannelName( cusdk__SetDeviceChannelNameReq req, cusdk__SetDeviceChannelNameRsp& rsp );

//ͬ����ƵԴ����
class cusdk__GetSyncVideoSrcNameEnableReq
{
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
};

class cusdk__GetSyncVideoSrcNameEnableRsp
{
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	bool enablePu2Platform; //ǰ��ͬ����ƽ̨
	bool enablePlatform2Pu; //ƽ̨ͬ����ǰ��
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ��ƵԴ����ͬ��ʹ��.
int cusdk__GetSyncVideoSrcNameEnable(cusdk__GetSyncVideoSrcNameEnableReq req, cusdk__GetSyncVideoSrcNameEnableRsp& rsp);

class cusdk__SetSyncVideoSrcNameEnableReq
{
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	bool enablePu2Platform; //ǰ��ͬ����ƽ̨
	bool enablePlatform2Pu; //ƽ̨ͬ����ǰ��
};

class cusdk__SetSyncVideoSrcNameEnableRsp
{
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ������ƵԴ����ͬ��ʹ��.
int cusdk__SetSyncVideoSrcNameEnable(cusdk__SetSyncVideoSrcNameEnableReq req, cusdk__SetSyncVideoSrcNameEnableRsp& rsp);

class cusdk__GetPtzPresetNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int vieoSrcId;			// ��ƵԴId.
};

class cusdk__PresetName
{
	int presetId;			// Ԥ��λId.
	xsd__string presetName;	// Ԥ��λ����.
};

class cusdk__GetPtzPresetNameRsp /* : public cusdk__BaseRsp */
{		
	std::vector< cusdk__PresetName > presetNameList;	// ��ƵԴ�����������Ԥ��λ����.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ����ͷԤ��λ����.
int cusdk__GetPtzPresetName( cusdk__GetPtzPresetNameReq req, cusdk__GetPtzPresetNameRsp& rsp );


class cusdk__SetPtzPresetNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int vieoSrcId;			// ��ƵԴId.
	int presetId;			// Ԥ��λId.

	xsd__string presetName;	// Ԥ��λ����.
};

class cusdk__SetPtzPresetNameRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ������ƵԴ����.
int cusdk__SetPtzPresetName( cusdk__SetPtzPresetNameReq req, cusdk__SetPtzPresetNameRsp& rsp );

class cusdk__DelPtzPresetNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int vieoSrcId;			// ��ƵԴId.
	int presetId;			// Ԥ��λId.
};

class cusdk__DelPtzPresetNameRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ������ƵԴ����.
int cusdk__DelPtzPresetName( cusdk__DelPtzPresetNameReq req, cusdk__DelPtzPresetNameRsp& rsp );


class cusdk__VsipChannelDataReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int deviceIndex;		// �豸���. ��NVRSVR�豸��ʱ����Ч, ��ͨ�豸����0.
	xsd__string vsipBase64DataReq;	// vsip���ݰ�.
};

class cusdk__VsipChannelDataRsp /* : public cusdk__BaseRsp */
{		
	xsd__string vsipBase64DataRsp;	// vsip���ݰ�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: VSIP͸������ͨ��
int cusdk__VsipChannelData( cusdk__VsipChannelDataReq req, cusdk__VsipChannelDataRsp& rsp );

// �豸 GPS ����.
class cusdk__TransDataParam
{
	bool       enable;             	                   //�Ƿ���
	xsd__string     gpsParam;                          //gps���Ĳ���
};

class cusdk__SetTransDataParamReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int        type;                                   //͸����������
	int        subType;                                //͸������������
	cusdk__TransDataParam param;	// ����.
};

class cusdk__SetTransDataParamRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����豸͸��ͨ������.
int cusdk__SetTransDataParam( cusdk__SetTransDataParamReq req, cusdk__SetTransDataParamRsp& rsp );

class cusdk__GetTransDataParamReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id
	int        type;                                   //͸����������
	int        subType;                                //͸������������
};

class cusdk__GetTransDataParamRsp /* : public cusdk__BaseRsp */
{	
	cusdk__TransDataParam param;	// ����.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�豸͸��ͨ������.
int cusdk__GetTransDataParam( cusdk__GetTransDataParamReq req, cusdk__GetTransDataParamRsp& rsp );

// �켣��Ϣ ��ѯ
class cusdk__QueryGPSTraceReq : public cusdk__BaseReq
{
	xsd__string deviceId;	// �豸Id.

	cusdk__TimeRange timeRange;	// ʱ����.

	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__QueryGPSTraceRsp /* : public cusdk__BaseRsp */
{
	int totalNum;								// ����.
	std::vector< cusdk__GPSInfo > gpsInfoList;	// GPS�豸�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ�豸��GPS�켣.
int cusdk__QueryGPSTrace( cusdk__QueryGPSTraceReq req, cusdk__QueryGPSTraceRsp& rsp );
// 

class cusdk__GetMapConfigReq : public cusdk__BaseReq
{	
};

class cusdk__GetMapConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__int maxGPSInfoSavedDays;	// GPS��Ϣ���洢����
	xsd__int maxGPSInfoSavedSize;	// ����GPS�洢ռ�ÿռ�,��λ(M).
	xsd__int maxGPSInfoSavedDeviceNumber;	// ����GPS�洢·��.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ���ӵ�ͼ��������Ϣ.
int cusdk__GetMapConfig( cusdk__GetMapConfigReq req, cusdk__GetMapConfigRsp& rsp );


class cusdk__SetMapConfigReq : public cusdk__BaseReq
{	
	xsd__int maxGPSInfoSavedDays;	// GPS��Ϣ���洢����
	xsd__int maxGPSInfoSavedSize;	// ����GPS�洢ռ�ÿռ�,��λ(M).
	xsd__int maxGPSInfoSavedDeviceNumber;	// ����GPS�洢·��.

};

class cusdk__SetMapConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ���õ��ӵ�ͼ��������Ϣ.
int cusdk__SetMapConfig( cusdk__SetMapConfigReq req, cusdk__SetMapConfigRsp& rsp );

class cusdk__GetDeviceSerialConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id.
	xsd__int serialPortID; //���ں�
};

class cusdk__GetDeviceSerialConfigRsp 
{		
	xsd__string deviceId;	// �豸Id.
	xsd__int serialPortID; //���ں�

	xsd__int baudRate; //���ڲ�����	
	xsd__int comPortType; //�������� 0:��ʹ��, 1:RS232, 2:RS422, 3:RS485

	xsd__boolean rS232Enable;
	xsd__boolean rS422Enable;
	xsd__boolean rS485Enable;

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�豸�Ĵ���������Ϣ.
int cusdk__GetDeviceSerialConfig( cusdk__GetDeviceSerialConfigReq req, cusdk__GetDeviceSerialConfigRsp& rsp );


class cusdk__SetDeviceSerialConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// �豸Id.
	xsd__int serialPortID; //���ں�

	xsd__int baudRate; //���ڲ�����
	xsd__int comPortType; //�������� 0:��ʹ��, 1:RS232, 2:RS422, 3:RS485
};

class cusdk__SetDeviceSerialConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �����豸�ĵĴ���������Ϣ.
int cusdk__SetDeviceSerialConfig( cusdk__SetDeviceSerialConfigReq req, cusdk__SetDeviceSerialConfigRsp& rsp );

class cusdk__GetDeviceGBIDReq : public cusdk__BaseReq
{	
	cusdk__DevChannel deviceChn;	// �豸�ı���ͨ����
};

class cusdk__GetDeviceGBIDRsp /* : public cusdk__BaseRsp */
{		
	xsd__string deviceGBID;	// ��ͨ����Ӧ��GB�����꣩ ID��
	xsd__string gbsID;		// ͨ�ŵ�GBS���  Ҳ��ϵͳĿ¼���

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡһ���豸ͨ���Ĺ���ID
int cusdk__GetDeviceGBID( cusdk__GetDeviceGBIDReq req, cusdk__GetDeviceGBIDRsp& rsp );

// class cusdk__GetDeviceGBIDBatchReq : public cusdk__BaseReq
// {	
// 	std::vector<cusdk__DevChannel> deviceChns;	// �豸�ı���ͨ����
// };
// 
// class cusdk__DeviceChnGBIDInfo
// {
// 	cusdk__DevChannel deviceChn;	// �豸ͨ����Ϣ
// 	xsd__string deviceGBID;			// ͨ������ID
// 	xsd__string gbsID;				// ͨ����GBS����,Ҳ��ϵͳĿ¼���
// };
// 
// class cusdk__GetDeviceGBIDBatchRsp /* : public cusdk__BaseRsp */
// {		
// 	std::vector< cusdk__DeviceChnGBIDInfo > deviceGBIDInfos;
// 
// 	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
// 	long errorCode;
// };
// 
// //gsoap cusdk service method-documentation: ������ȡ�豸ͨ���Ĺ���ID
// int cusdk__GetDeviceGBIDBatch( cusdk__GetDeviceGBIDBatchReq req, cusdk__GetDeviceGBIDBatchRsp& rsp );

class cusdk__GetCivilcodeReq : public cusdk__BaseReq
{	
	xsd__string deviceGroupId;	// �豸���ID��
};

class cusdk__GetCivilcodeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string civilcode;	// �豸���Ӧ����������civilcode.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡһ���豸��������������
int cusdk__GetCivilcode( cusdk__GetCivilcodeReq req, cusdk__GetCivilcodeRsp& rsp );

class cusdk__GetDeviceChnByGBIDReq : public cusdk__BaseReq
{	
	xsd__string deviceGBID;	// �豸�Ĺ�����롣
};

class cusdk__GetDeviceChnByGBIDRsp /* : public cusdk__BaseRsp */
{		
	xsd__string deviceID;	// �豸��ID,��ʽ�� uuid@���� 
	xsd__int chnId;			// �豸��ͨ��id.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡ�豸��GBID��ȡ�豸��ID��ͨ��.
int cusdk__GetDeviceChnByGBID( cusdk__GetDeviceChnByGBIDReq req, cusdk__GetDeviceChnByGBIDRsp& rsp );


class cusdk__GetCAPSConfigReq : public cusdk__BaseReq
{	
};

class cusdk__GetCAPSConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__int captureIntervalSecs;	// ץ��ʱ����,��λ��.
	xsd__string wechatUrl;			// ΢��ƽ̨��ҳURL.
	xsd__string wechatTitle;		// ΢��ƽ̨�ı���.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ȡץ������
int cusdk__GetCAPSConfig( cusdk__GetCAPSConfigReq req, cusdk__GetCAPSConfigRsp& rsp );

class cusdk__SetCAPSConfigReq : public cusdk__BaseReq
{	
	xsd__int captureIntervalSecs;	// ץ��ʱ����,��λ��.
	xsd__string wechatTitle;		// ΢��ƽ̨�ı���.
};

class cusdk__SetCAPSConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ����ץ������
int cusdk__SetCAPSConfig( cusdk__SetCAPSConfigReq req, cusdk__SetCAPSConfigRsp& rsp );

class cusdk__CAPSDevice
{
	xsd__string devId;		// �豸ԭʼID.
	xsd__int chnId;			// �豸ͨ��.
	xsd__string name;		// ����.

	xsd__string devGBId;	// �豸�Ĺ���.
	xsd__string domainGBId;	// ��Ĺ���ID.

	//xsd__int videosrcId;		// ��ƵԴID. ?
};

class cusdk__QueryCAPSDeviceListReq : public cusdk__BaseReq
{
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.
};
class cusdk__QueryCAPSDeviceListRsp /* : public cusdk__BaseRsp */
{
	int totalNum;								// ����.
	std::vector<cusdk__CAPSDevice> devList;		// �豸�б�.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ѯ������豸.
int cusdk__QueryCAPSDeviceList( cusdk__QueryCAPSDeviceListReq req, cusdk__QueryCAPSDeviceListRsp& rsp );

class cusdk__CAPSAddDeviceReq : public cusdk__BaseReq
{	
	std::vector<cusdk__CAPSDevice> devList;		// �豸�б�.
};

class cusdk__CAPSAddDeviceRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
// gsoap cusdk service method-documentation: ����һ���豸
int cusdk__CAPSAddDevice( cusdk__CAPSAddDeviceReq req, cusdk__CAPSAddDeviceRsp& rsp );

// gsoap cusdk service method-documentation: �޸�һ���豸.
class cusdk__CAPSModifyDeviceReq : public cusdk__BaseReq
{	
	cusdk__CAPSDevice device;	// �޸ĵ��豸.
};

class cusdk__CAPSModifyDeviceRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
// gsoap cusdk service method-documentation: ����һ���豸
int cusdk__CAPSModifyDevice( cusdk__CAPSModifyDeviceReq req, cusdk__CAPSModifyDeviceRsp& rsp );

class cusdk__CAPSDelDeviceReq : public cusdk__BaseReq
{	
	cusdk__DevChannel device;	// Ҫɾ�����豸.
};

class cusdk__CAPSDelDeviceRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
// gsoap cusdk service method-documentation: ɾ��һ���豸.
int cusdk__CAPSDelDevice( cusdk__CAPSDelDeviceReq req, cusdk__CAPSDelDeviceRsp& rsp );


class cusdk__GetOfflineDeviceStatReq : public cusdk__BaseReq
{	
	xsd__int beginIndex;	// ��ʼ���.
	xsd__int num;			// ��������Ҫ������.

	xsd__string domanName;	// Ҫͳ�Ƶ���.

	xsd__string queryTag;	// һ�β�ѯ����ı�ǣ��������ֲ�ͬ�Ĳ�ѯ��
};

class cusdk__DeviceVideosrcId
{
	xsd__string deviceId;	// �豸ID.
	xsd__int videoSrcId;	// ��ƵԴID.
	xsd__boolean deviceOnline;      // ��ƵԴ���ڵ��豸�Ƿ�����
};

class cusdk__GetOfflineDeviceStatRsp /* : public cusdk__BaseRsp */
{		
//	vector< xsd__string > offlineDeviceList;	// ���豸��ͳ��.

	std::vector< cusdk__DeviceVideosrcId > offlineDeviceVideosrcList;	// �����ߵ���ƵԴ�б�.

	std::vector< cusdk__DeviceVideosrcId > idleStatVideoSrcList;    //����Idle״̬����ƵԴ�б�

	xsd__int totalOfflineDeviceVideosrcNum;	// ����,���ڷ�ҳ.

	xsd__int onlineDeviceNum;

	xsd__int idleDeviceNum; //idle״̬

	xsd__string queryTag;	// ���� , Ϊ�˲����ⷽ��
	xsd__string domanName;	// ���� , Ϊ�˲����ⷽ��

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
// gsoap cusdk service method-documentation: ��ȡ�������豸��Ϣ
int cusdk__GetOfflineDeviceStat( cusdk__GetOfflineDeviceStatReq req, cusdk__GetOfflineDeviceStatRsp& rsp );

class cusdk__UpdateCUPropertyReq : public cusdk__BaseReq
{	
	xsd__string clientLocalIP;	// CU�ı���IP.
	enum cusdk__ENATMode platNatMode;	// �ֹ�ָ��ƽ̨��NATģʽ.
	xsd__boolean clientSupportNatProbe;  //��ʶ�ͻ����Ƿ�֧��NAT��͸
};

class cusdk__UpdateCUPropertyRsp /* : public cusdk__BaseRsp */
{		
	enum cusdk__ENATMode cuNatMode;		// ��⵽��CU��NATģʽ.
	enum cusdk__ENATMode platNatMode;	// ��⵽��ƽ̨��NATģʽ.
	xsd__string platNATIP;		// ƽ̨����NAT����IP.
	xsd__int platNATPort;		// ƽ̨���յ�NAT���Ķ˿�
	xsd__boolean platSupportForward; //ƽ̨�Ƿ�֧�ִ���ת������(��Ҫ���ڶԳ�NAT�����£�ƽ̨¼��Ĳ��ż�����)

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
// gsoap cusdk service method-documentation: ��ȡ�������豸��Ϣ
int cusdk__UpdateCUProperty( cusdk__UpdateCUPropertyReq req, cusdk__UpdateCUPropertyRsp& rsp );

class cusdk__RestartDeviceReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	 // �豸ID
};

class cusdk__RestartDeviceRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
// gsoap cusdk service method-documentation: �����豸(Ŀǰֻ֧�ֹ����豸)
int cusdk__RestartDevice( cusdk__RestartDeviceReq req, cusdk__RestartDeviceRsp& rsp );


////////////////////////������û������ȡ�������豸///////////////////////////////////
class cusdk__UserAddDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string userId;			//�û�id
	xsd__string devgrpid;		//�豸��id	

	bool usePri;						// �Ƿ�ʹ������ָ���豸Ȩ��.
	std::set<  int > devPriSet;			// �豸Ȩ���б�. ����� cusdk__EDevicePri
	std::set<  int > videoSrcPriSet;	// ��ƵԴȨ���б�. ����� cusdk__EDevicePri
	int ptzLevel;						// ptz���Ƶȼ�.
};

class cusdk__UserAddDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ������
};
// gsoap cusdk service method-documentation: ����һ���豸����û�
int cusdk__UserAddDeviceGroup( cusdk__UserAddDeviceGroupReq req, cusdk__UserAddDeviceGroupRsp& rsp );

class cusdk__UserDelDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string userId;			//�û�id
	xsd__string devgrpid;		//�豸��id	
};

class cusdk__UserDelDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ������
};
// gsoap cusdk service method-documentation: ȡ������һ���û����豸��
int cusdk__UserDelDeviceGroup( cusdk__UserDelDeviceGroupReq req, cusdk__UserDelDeviceGroupRsp& rsp );
////////////////////////������û��黮���ȡ�������豸///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
class cusdk__UserGroupAddDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string usergrpId;		//�û���id
	xsd__string devgrpid;		//�豸��id	

	bool usePri;						// �Ƿ�ʹ������ָ���豸Ȩ��.
	std::set<  int > devPriSet;			// �豸Ȩ���б�. ����� cusdk__EDevicePri
	std::set<  int > videoSrcPriSet;	// ��ƵԴȨ���б�. ����� cusdk__EDevicePri
	int ptzLevel;						// ptz���Ƶȼ�.
};

class cusdk__UserGroupAddDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ������
};
// gsoap cusdk service method-documentation: ����һ���豸����û���
int cusdk__UserGroupAddDeviceGroup( cusdk__UserGroupAddDeviceGroupReq req, cusdk__UserGroupAddDeviceGroupRsp& rsp );

class cusdk__UserGroupDelDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string usergrpId;		//�û���id
	xsd__string devgrpid;		//�豸��id	
};

class cusdk__UserGroupDelDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ������
};
// gsoap cusdk service method-documentation: ȡ������һ���û�����豸��
int cusdk__UserGroupDelDeviceGroup( cusdk__UserGroupDelDeviceGroupReq req, cusdk__UserGroupDelDeviceGroupRsp& rsp );

////////////////////////����ȡ���û����û����豸���Զ�ͬ�����///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
class cusdk__UserSetAutoSyncDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string userId;			//�û�id
	xsd__string devgrpid;		//�豸��id
	xsd__boolean bSync;			//���û�ȡ���Զ�ͬ�����	
};

class cusdk__UserSetAutoSyncDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ������
};
// gsoap cusdk service method-documentation: �Զ�ͬ��/ȡ��ͬ��һ���û����豸��
int cusdk__UserSetAutoSyncDeviceGroup( cusdk__UserSetAutoSyncDeviceGroupReq req, cusdk__UserSetAutoSyncDeviceGroupRsp& rsp );


class cusdk__UserGroupSetAutoSyncDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string usergrpId;		//�û���id
	xsd__string devgrpid;		//�豸��id
	xsd__boolean bSync;			//���û�ȡ���Զ�ͬ�����	
};

class cusdk__UserGroupSetAutoSyncDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__string	operationId;	//��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ������
};
// gsoap cusdk service method-documentation: �Զ�ͬ��/ȡ��ͬ��һ���û�����豸��
int cusdk__UserGroupSetAutoSyncDeviceGroup( cusdk__UserGroupSetAutoSyncDeviceGroupReq req, cusdk__UserGroupSetAutoSyncDeviceGroupRsp& rsp );
////////////////////////��ѯ�û����û����豸���Զ�ͬ�����///////////////////////////////////


//��ѯָ������Id(operationId)�ĵ�ǰ��������(����ӿ���Ϊcuiʵ�ֵ�ԭ��,ֻ�����豸��������Ľ��Ȳ�ѯ!)
class cusdk__QueryOperationProgreeReq : public cusdk__BaseReq
{	
	xsd__string operationId;	//����Id����ƽ̨���ص��Ǹ�	
};

class cusdk__QueryOperationProgreeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__int progress;		//����ֵ
	xsd__int total;			//����

};
// gsoap cusdk service method-documentation: ��ѯָ������Id�ĵ�ǰִ�н���
int cusdk__QueryOperationProgress( cusdk__QueryOperationProgreeReq req, cusdk__QueryOperationProgreeRsp& rsp );

//////////////////////////////////////////////////////////////////////////////////////////////
class cusdk__UserQueryAutoSyncDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string userId;			//�û�id
	xsd__int beginIndex;		// ��ʼ���.
	xsd__int num;		    	// ��������Ҫ����Ŀ.
};

class cusdk__UserQueryAutoSyncDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	long totalnum;		//����
	std::vector< xsd__string > devgroupIds;
};
// gsoap cusdk service method-documentation: ��ҳ��ȡ�û�������ͬ���豸���б�
int cusdk__UserQueryAutoSyncDeviceGroup( cusdk__UserQueryAutoSyncDeviceGroupReq req, cusdk__UserQueryAutoSyncDeviceGroupRsp& rsp );


class cusdk__UserGroupQueryAutoSyncDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string usergrpId;		//�û���id
	xsd__int beginIndex;		// ��ʼ���.
	xsd__int num;		    	// ��������Ҫ����Ŀ.
};

class cusdk__UserGroupQueryAutoSyncDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__int totalnum;		//����
	std::vector< xsd__string > devgroupIds;
};
// gsoap cusdk service method-documentation: ��ҳ��ȡ�û���������ͬ���豸���б�
int cusdk__UserGroupQueryAutoSyncDeviceGroup( cusdk__UserGroupQueryAutoSyncDeviceGroupReq req, cusdk__UserGroupQueryAutoSyncDeviceGroupRsp& rsp );



class cusdk__GetGBDomainListReq : public cusdk__BaseReq
{
};

class cusdk__GBDomainInfo
{
	xsd__string gbDomainId;          //������ID
	xsd__string domainName;	         // ����

//	xsd__string userName;            //���͵��û����������ĸ��û����豸��

	xsd__string devTreeViewId;		// ���͵��豸��ͼId
	xsd__string devTreeVersion;		// ���͵��豸��ͼ�汾��.
};

class cusdk__GetGBDomainListRsp
{
	xsd__string session;
	std::vector< cusdk__GBDomainInfo > gbDomainList;
	long errorCode;
};
// gsoap cusdk service method-documentation: ��ȡ�ϼ��������б����������ϼ����͵�״̬
int cusdk__GetSuperGBDomainList(cusdk__GetGBDomainListReq req, cusdk__GetGBDomainListRsp& rsp);



class cusdk__DevicePushReq : public cusdk__BaseReq
{
	xsd__string userName;      //Ҫ�����ĸ��û����豸
	xsd__string gbDomainId;    //Ҫ���͵��Ĺ�����
};

class cusdk__DevicePushRsp
{
	xsd__string session;
	long errorCode;
};
// gsoap cusdk service method-documentation: �����豸���ϼ�������
int cusdk__PushDevice(cusdk__DevicePushReq req, cusdk__DevicePushRsp& rsp);

enum cusdk__EDataType
{
	cusdk__DataTypeDevice,	// �豸����.
	cusdk__DataTypeEMap,	// ���ӵ�ͼ,ͼԪ��Ϣ.
	cusdk__DataTypeAlarmLink,	// �澯����.

	cusdk__DataTypeGBSCatalog,		// ����GBS�е��豸��Ϣ/״̬. �޲���.
	cusdk__DataTypeGBSDevicePush,	// ����/���� GBS���ϼ���������豸. ����1: �ϼ���Ĺ���ID. 

	cusdk__DataDeviceStatistics,	// �����豸ͳ����Ϣcsv. ����1: �豸��ѯ����.
};

class cusdk__GetDataVersionReq : public cusdk__BaseReq
{
	xsd__int dataType;	// ��������. ���ͼ� cusdk__EDataType
};

class cusdk__GetDataVersionRsp
{
	xsd__string session;
	long errorCode;

	xsd__string dataVersion;
};
// gsoap cusdk service method-documentation: ��ȡƽ̨�����ݰ汾.
int cusdk__GetDataVersion(cusdk__GetDataVersionReq req, cusdk__GetDataVersionRsp& rsp);


// ͨ�õ��ļ������ӿ�.
class cusdk__StartDataFileExportReq : public cusdk__BaseReq
{	
	int exportedDataType;	// Ҫ�������������. ���嶨��μ� cusdk__EDataType

	std::vector< xsd__string > params;	// ����, ÿ�ֵ�����ͬ, ����μ� cusdk__EDataType ������..
};

class cusdk__StartDataFileExportRsp /* : public cusdk__BaseRsp */
{	
	xsd__string fileUrl;	// ����/���������Url.

	xsd__string operationId;	// �첽��ѯ�����õĲ���ID. ���� cusdk__QueryOperationProgress �ӿڲ�ѯ����.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ��������,�첽,��Ҫ���� cusdk__QueryOperationProgress ��ȡ����.
int cusdk__StartDataFileExport( cusdk__StartDataFileExportReq req, cusdk__StartDataFileExportRsp& rsp );

class cusdk__EndDataFileExportReq : public cusdk__BaseReq
{	
	xsd__string operationId;	// ���صĲ���ID.
	std::vector< xsd__string > params;	// ����, ÿ�ֵ�����ͬ, ����μ� cusdk__EDataType ������..
};

class cusdk__EndDataFileExportRsp /* : public cusdk__BaseRsp */
{	
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �Ѿ���ɻ�ȡ�����ݵ���.
int cusdk__EndDataFileExport( cusdk__EndDataFileExportReq req, cusdk__EndDataFileExportRsp& rsp );


// ͨ�õ��ļ�����ӿ�.
class cusdk__StartDataFileImportReq : public cusdk__BaseReq
{	
	int importedDataType;	// Ҫ�������������. ���嶨��μ� cusdk__EDataType

	std::vector< xsd__string > params;	// ����, ÿ�����ݲ�ͬ, ����μ� cusdk__EDataType ������..
};

class cusdk__StartDataFileImportRsp /* : public cusdk__BaseRsp */
{	
	xsd__string fileUrl;	// ����/���������Url.

	xsd__string operationId;	// �첽��ѯ�����õĲ���ID. ���ں�������, cusdk__QueryOperationProgress �ӿڲ�ѯ����.

	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: ��ʼ���� ����.
int cusdk__StartDataFileImport( cusdk__StartDataFileImportReq req, cusdk__StartDataFileImportRsp& rsp );

class cusdk__EndDataFileImportReq : public cusdk__BaseReq
{	
	xsd__string operationId;	// ���صĲ���ID.

	xsd__boolean cancelOperation;	// �Ƿ�ȡ������.

	std::vector< xsd__string > params;	// ����, ÿ�����ݲ�ͬ, ����μ� cusdk__EDataType ������..
};

class cusdk__EndDataFileImportRsp /* : public cusdk__BaseRsp */
{	
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
//gsoap cusdk service method-documentation: �Ѿ���ɻ�ȡ�����ݵ���.
int cusdk__EndDataFileImport( cusdk__EndDataFileImportReq req, cusdk__EndDataFileImportRsp& rsp );

//��ѯָ������Id(operationId)�ĵ�ǰ��������
class cusdk__GetOperationProgreeReq : public cusdk__BaseReq
{	
	xsd__string operationId;	//����Id����ƽ̨���ص��Ǹ�	
};

class cusdk__GetOperationProgreeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
	xsd__int progress;		//����ֵ
	xsd__int total;		//����

};
// gsoap cusdk service method-documentation: ��ѯָ������Id�ĵ�ǰִ�н���
int cusdk__GetOperationProgress( cusdk__GetOperationProgreeReq req, cusdk__GetOperationProgreeRsp& rsp );


// ������Ϣ
class cusdk__ChildrenGBDomainInfo
{
	xsd__string gbDomainId;          //������ID
	xsd__string domainName;	         // ����
    xsd__int    status;              // ��״̬cusdk__EDomainStatus
};

// ��ȡ���е�������Ϣ
class cusdk__GetChildrenGBDomainListReq : public cusdk__BaseReq
{
};

class cusdk__GetChildrenGBDomainListRsp
{
 std::vector< cusdk__ChildrenGBDomainInfo > domainIdList;	// ���������б�.
 xsd__string session;
 long errorCode;
};

// ��ȡ��ǰ���õ����б�.
class cusdk__GetGBDevicePushDomainReq : public cusdk__BaseReq
{
 xsd__string superDomainId;	// ��ȡ���ϼ�������ID
};

class cusdk__GetGBDevicePushDomainRsp
{
 std::vector< cusdk__ChildrenGBDomainInfo > domainIdList;	// ���õ������б�.
 xsd__string session;
 long errorCode;
};


// ���õ�ǰ�����б�.
class cusdk__SetGBDevicePushDomainReq : public cusdk__BaseReq
{
 std::vector< xsd__string > domainIdList;	// ���õ������б�.
 xsd__string superDomainId;	// ���õ��ϼ�������ID
};

class cusdk__SetGBDevicePushDomainRsp
{
 xsd__string session;
 long errorCode;
};



// ��ȡ�������б�
int cusdk__GetChildrenGBDomainList( cusdk__GetChildrenGBDomainListReq req, cusdk__GetChildrenGBDomainListRsp& rsp);

// ��ȡ��ǰ���͵����б�
int cusdk__GetGBDevciePushDomain(cusdk__GetGBDevicePushDomainReq req, cusdk__GetGBDevicePushDomainRsp& rsp);

// ���õ�ǰ���͵����б�.

// ����ָ������Id(operationId)�ĵ�ǰ��������
class cusdk__UpdateOperationProgreeReq : public cusdk__BaseReq
{	
	xsd__string operationId;	//����Id����ƽ̨���ص��Ǹ�	

	xsd__int progress;		//����ֵ
	xsd__int total;		//����
};

class cusdk__UpdateOperationProgreeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;
};
// gsoap cusdk service method-documentation: ����ָ������Id�ĵ�ǰִ�н���
int cusdk__UpdateOperationProgress( cusdk__UpdateOperationProgreeReq req, cusdk__UpdateOperationProgreeRsp& rsp );

 
 class cusdk__ResInfoReq
 {
 	//�������ͣ�0osp��1rtsp��2rtmp
 	xsd__int     PlayMode;
 	//��������IP�����Ϊnat���nat��url��nat�ڸ�nat��url����������������Σ�nat����url
 	xsd__int     ServerIp;
 	//����
 	xsd__int     ServerPort;
 	//ƽ̨��ID��20λ����ID��ʽ����KDMNO
 	xsd__string  DomainId;
 	//�豸ͨ��ID��20λ����ID��ʽ����KDMNO
 	xsd__string  DeviceId;
 	//����KDMNOʹ�ã����ʹ�ù���ID������,ʹ��KDMNOΪͨ����
 	xsd__int     ChannelId;
 	//����
 	xsd__int     ScreenWidth;
 	//����
 	xsd__int     ScreenHeight;
 	//����
 	xsd__int     Quality;
 	//����
 	xsd__string  Url;
 	//�����ģ���ͬ�������Ļ�ѡ��ͬһ̨�ӻ�ȥ�������Ҫ����osp���
 	xsd__string  Context;
 	//¼��ʼʱ�䣬������Ϊ��ֱ��
 	xsd__int	 StartTime;
 	//¼�����ʱ�䣬������Ϊ��ֱ��
 	xsd__int	 EndTime;
 	//¼�����ͣ�1Ϊƽ̨¼��2Ϊǰ��¼��
 	xsd__int	 VodType;
 };
 
 class cusdk__ResInfo
 {
 	xsd__string  Url;
 	//�ֱ���
 	xsd__int     Width;
 	xsd__int     Height;
 	xsd__int     FrameRate;
 	//OriginalStreamΪԭʼ������TransCodeHidefitionΪת��������TransCodeFluentΪת������
 	xsd__string  Desc;	
 	//��������������Ƿ���ת������ѡ��URL	
 	xsd__string  ManuFactory;
 	//ospģʽ��Ϊtcpsendport������ģʽ��ʱ����
 	xsd__string  Reserve;
 	//��Ƶ��ʽ
 	xsd__string  VideoType;
 	//��Ƶ������
 	xsd__int	 VideoSmpRate;
 	//��Ƶ��ʽ
 	xsd__string  AudioType;
 	//��Ƶ������
 	xsd__int	 AudioSmpRate;
 	//��Ƶͨ����
 	xsd__int	 AudioChnlNum;
 };
 
 class cusdk__ResInfoRsp
 {
 	std::vector<cusdk__ResInfo> ResList;	
 };
 int cusdk__GetResInfo(cusdk__ResInfoReq cResInfoReq, cusdk__ResInfoRsp &cResInfoRsp);

 // �豸����/ͳ�� ���������ؼ���֧������, ��������֮�䶼�� AND ��ϵ.
 // userid		ָ��Ҫ�������û�id, δָ��ʱ������ǰ��¼�û�.
 // key			�����ؼ���. ģ��ƥ��. ƴ������ĸ?
 // state=online/offline	�豸״̬, ����/������, idle״̬���Ǻ��Ե���.
 // tree		ָ��Ҫ��������id(��ͼ), ���δָ��,����ϵͳĬ���豸��.
 // rootgroup	ָ�����豸��, δָ��ʱΪ���ĸ���.
 // devid		ָ��Ҫ�������豸Id.
 // filterEmptyGroup	ָ���豸�����Ƿ����豸,Ĭ��ֻ�������豸���豸��.
 // type=group/device/videosrc		ָ��Ҫ�������������豸��, �豸,��ƵԴ. ���Զ�ѡ, ��������� / �ָ�.
 // location=yes/no ָ����ƵԴ�Ƿ��о�γ������, yes��, noû��. ����ʱ���Դ�����.
 // devOnTree=id	�豸ͬʱ������һ����(�Զ�����ͼ)��, Ϊ��ʱ���Դ˲���. 
 // devOnTree!=id	�豸��������һ����(�Զ�����ͼ)��, Ϊ��ʱ���Դ˲���.	
 // parentgroup		����,���ݹ�.
 // statVS=yes/no		�Ƿ�ͳ���豸�����������豸��Ŀ. Ĭ�ϲ�ͳ��. ����ͳ�ƹ���, �� GetSearchResultByGroup ���Ի�ȡ���豸������������.
 class cusdk__SearchDeviceReq : public cusdk__BaseReq
 {
	 xsd__string keywords;	// �����ؼ���. 
	 xsd__int maxResultItemNum;	// ���Ľ������С,�����ĺ���. -1 ��ʾ������.
	 xsd__int maxSearchTimeMSec;// ��������ʱ,�������ʱ���ֹͣ����.
 };

 enum cusdk__SearchResultType
 {
	 cusdk__SearchResultInvalid,		// ������.
	 cusdk__SearchResultDeviceGroup,	// ����.
	 cusdk__SearchResultDevice,			// �豸.
	 cusdk__SearchResultVideosrc,		// Videosrc.
 };

 class cusdk__SearchDeviceRsp
 {
	 xsd__string operationId;	// ����Id, ���ڻ�ȡ����. cusdk__GetOperationProgress

	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;		// ������.
 };

 // �豸����/ͳ��.
 int cusdk__SearchDevice( cusdk__SearchDeviceReq req, cusdk__SearchDeviceRsp& rsp );

 // ��ȡ�������.
 class cusdk__GetSearchResultReq : public cusdk__BaseReq
 {
	 xsd__string operationId;

	 xsd__int beginIndex;		// ��ʼ���.
	 xsd__int num;		    	// ��������Ҫ����Ŀ.
 };

 class cusdk__DeviceGroupPathItem
 {
	 xsd__string groupId;
	 xsd__string groupName;
 };

 class cusdk__SearchResultItem
 {
	 xsd__int type;		// ����, ����μ�cusdk__SearchResultType, �豸��/�豸/��ƵԴ
	 xsd__string id;	// Id
	 xsd__int vsId;		// ��ƵԴid. ���Ϊ�豸��/�豸ʱ������.
	 xsd__string name;	// ����

	 std::vector<cusdk__DeviceGroupPathItem> fullDeviceGroupPath;	// ����ڵ�֮�ϵ������豸��ڵ�Id�б�. 

	 xsd__boolean online;	// �Ƿ�����.
	 std::vector< int > deviceCapLabelList;	// �豸��������ǩ�б�. (���/ǹ��) cusdk__EDeviceCapLabel

	 xsd__string domainId;	// �豸������Id.
	 xsd__string manufacturer;						// ������.
 };

 class cusdk__GetSearchResultRsp
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;		// ������.

	 xsd__boolean lastPage;	// �Ƿ������һҳ��.

	 xsd__int totalNum;		// �������.
	 std::vector< cusdk__SearchResultItem > result;	// ����б�.
 };

 int cusdk__GetSearchResult( cusdk__GetSearchResultReq req, cusdk__GetSearchResultRsp& rsp );

 class cusdk__GetSearchResultByGroupReq : public cusdk__BaseReq
 {
	 xsd__string operationId;

	 xsd__int beginIndex;		// ��ʼ���.
	 xsd__int num;		    	// ��������Ҫ����Ŀ.

	 xsd__string deviceGroupId;
	 xsd__int type;		// ����, ����μ�cusdk__SearchResultType, �豸��/�豸/��ƵԴ
 };

 class cusdk__GetSearchResultByGroupRsp
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;		// ������.

	 xsd__boolean lastPage;	// �Ƿ������һҳ��.
	 std::vector< cusdk__SearchResultItem > result;	// ����б�.

	 xsd__int allVideoSrcNum;		// �������.  statVS=yes ʱ���ô��ֶ�, ���򷵻�0.
	 xsd__int onlineVideoSrcNum;	// ������ƵԴ��Ŀ. statVS=yes ʱ���ô��ֶ�, ���򷵻�0.
 };

 int cusdk__GetSearchResultByGroup(cusdk__GetSearchResultByGroupReq req, cusdk__GetSearchResultByGroupRsp& rsp);
 

 // ��������.
 class cusdk__EndOperationReq : public cusdk__BaseReq
 {
	 xsd__string operationId;	// ���صĲ���ID.

	 std::vector< xsd__string > params;	// ����, 
 };


 class cusdk__EndOperationRsp /* : public cusdk__BaseRsp */
 {	
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ����һ������.
 int cusdk__EndOperation( cusdk__EndOperationReq req, cusdk__EndOperationRsp& rsp );


 // ͨ��ID��ȡһ���豸�����Ϣ.
 class cusdk__GetDeviceGroupReq : public cusdk__BaseReq
 {
	 xsd__string id;	// �豸��id.
 };

 class cusdk__GetDeviceGroupRsp
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;		// ������.

	 cusdk__DeviceGroup group;

	 // std::vector<xsd__string> fullDeviceGroupPath;	// �����ڵ�֮�ϵ������豸��ڵ�Id�б�. 
 };

 int cusdk__GetDeviceGroup( cusdk__GetDeviceGroupReq req, cusdk__GetDeviceGroupRsp& rsp );

 // ͨ��ID��ȡһ���豸����Ϣ.
 class cusdk__GetDeviceReq : public cusdk__BaseReq
 {
	 xsd__string id;		// �豸Id.
 };

 class cusdk__GetDeviceRsp 
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;		// ������.

	 cusdk__DeviceInfo devInfo;		// �豸��Ϣ.
	 //cusdk__DeviceStatus devStatus;	// �豸״̬

	// std::vector<cusdk__DeviceGroupPathItem> fullDeviceGroupPath;	// ����ڵ�֮�ϵ������豸��ڵ�Id�б�. ֻ���ص�һ������.

	 cusdk__DevOnlineNotify online;
	 cusdk__DevConfigNotify config;
	 cusdk__DevServiceStatusNotify service;
	 std::vector< cusdk__DevAlarmNotify > alarm;        // �豸�澯֪ͨ.

 };

 int cusdk__GetDevice( cusdk__GetDeviceReq req, cusdk__GetDeviceRsp& rsp );

 class cusdk__GetDeviceGroupPathReq : public cusdk__BaseReq
 {
     xsd__string devTreeId; //�豸��������Id����ͬһ��ƵԴ���ܱ��������ͬ����ͼ
     xsd__string devId;   //�豸Id
	 xsd__int  videoSrcId; //��ƵԴId
 };

 class cusdk__GetDeviceGroupPathRsp
 {
	 std::vector<cusdk__DeviceGroupInfo> devGroupPath;  //����·�����������ϲ���鵽�²��������
	 xsd__string session;
	 long errorCode;
 };

 //������ƵԴ��Ϣ����ȡ����������·��
 int cusdk__GetDeviceGroupPath(cusdk__GetDeviceGroupPathReq req, cusdk__GetDeviceGroupPathRsp& rsp);


 class cusdk__CreateCustomTreeViewReq : public cusdk__BaseReq
 {
	 xsd__string name; //�Զ�����ͼ����

	 bool cloneFromAnotherTreeView;		// �Ƿ������һ����clone.
	 xsd__string cloneSrcTreeViewId;	// ������һ������clone, Դ��id, Ϊ��Ϊϵͳ�豸��.

	 bool canUsedForGBPush;			// �Ƿ�֧����Ϊ����ѡ�������͵������豸��
 };

 class cusdk__CreateCustomTreeViewRsp
 {
	 xsd__string id; //��ʶ�´�������ͼid

	 xsd__string operationId;	// ����ID, ���ڲ�ѯ����. (cusdk__QueryOperationProgress).

	 xsd__string session;
	 long errorCode;
 };
 
 //�����Զ����豸����ͼ
 int cusdk__CreateCustomTreeView(cusdk__CreateCustomTreeViewReq req, cusdk__CreateCustomTreeViewRsp& rsp);

 class cusdk__ModifyCustomTreeViewNameReq : public cusdk__BaseReq
 {
	 xsd__string id; //��ͼid����ʶҪ�޸ĵ���ͼ
	 xsd__string name; //�µ���ͼ����
 };


 class cusdk__ModifyCustomTreeViewNameRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 //�޸��Զ����豸����ͼ����
 int cusdk__ModifyCustomTreeViewName(cusdk__ModifyCustomTreeViewNameReq req, cusdk__ModifyCustomTreeViewNameRsp& rsp);


 class cusdk__DeleteCustomTreeViewReq : public cusdk__BaseReq
 {
	 xsd__string id; //��ͼid
 };

 class cusdk__DeleteCustomTreeViewRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 //ɾ���Զ����豸����ͼ
 int cusdk__DeleteCustomTreeView(cusdk__DeleteCustomTreeViewReq req, cusdk__DeleteCustomTreeViewRsp& rsp);


 enum cusdk__TreeViewType
 {
	 cusdk__TreeViewType_System = 0, //ϵͳ������ͼ�����û�����
	 cusdk__TreeViewType_User, //�û�������ͼ���û���ռ
 };

 //�Զ����豸����ͼ��Ϣ
 class cusdk__CustomTreeViewInfo
 {
	 xsd__string id; //��ͼid
	 xsd__string name; //��ͼ����
	 xsd__string dataVersion; //���ݰ汾[ֻ��]

	 bool canUsedForGBPush;	// �Ƿ�֧����Ϊ����ѡ�������͵������豸��
	 int  treeViewType; //�����cusdk__TreeViewType
 };

 class cusdk__GetCustomTreeViewListReq : public cusdk__BaseReq
 {
	  bool includeFavorite; // ���������Ƿ�Ҫ�����û�˽�е��豸�������ղؼУ�
 };

 class cusdk__GetCustomTreeViewListRsp
 {
	 std::vector<cusdk__CustomTreeViewInfo> treeViewInfoList;
	 xsd__string session;
	 long errorCode;
 };

 //��ȡ�Զ����豸����ͼ�б�
 int cusdk__GetCustomTreeViewList(cusdk__GetCustomTreeViewListReq req, cusdk__GetCustomTreeViewListRsp& rsp);

 // �ݹ��ѯ�����µ��豸.
 class cusdk__GetUserDeviceInGroupReq : public cusdk__BaseReq
 {
	 xsd__string userId;	// �û�ID, Ϊ��ʱΪ��ǰ�������û�.
	 xsd__string treeId;	// ��ID, Ϊ����ΪϵͳĬ���豸��.
	 xsd__string groupId;	// �豸��Id.
	 xsd__boolean recursive;// �Ƿ���¼��������ݹ����.
	 xsd__string queryTag;	// ��ѯId, ���ڷ�ҳ��ѯ.

	 xsd__int num;			// ������ѯ������.
 };

 class cusdk__SimpleUserDevice
 {
	 xsd__string devId;
	 std::vector<int> videoSrcIdList;
 };

 class cusdk__GetUserDeviceInGroupRsp
 {
	 xsd__string queryTag;	// ��ѯId, ���ڷ�ҳ��ѯ.
	 xsd__boolean lastPage;	// �Ƿ����һҳ.

	 std::vector< cusdk__SimpleUserDevice > devList;

	 xsd__string session;
	 long errorCode;
 };

 int cusdk__GetUserDeviceInGroup(cusdk__GetUserDeviceInGroupReq req, cusdk__GetUserDeviceInGroupRsp& rsp);

 // ���������ӿ�
 enum cusdk__EOperationType
 {
	 cusdk__OpStartPlatRecord,	// ��ʼƽ̨¼��
	 cusdk__OpStopPlatRecord,	// ֹͣƽ̨¼��
	 cusdk__OpStartPuRecord,	// ��ʼǰ��¼��
	 cusdk__OpStopPuRecrod,		// ֹͣǰ��¼��
	 cusdk__OpStartWiper,		// ��ʼ��ˢ
	 cusdk__OpStopWiper,		// ֹͣ��ˢ
	 cusdk__OpStopPuCall,		// ֹͣ��������, ����1:�û����е�PlayId
	 cusdk__OpLoadPreset,		// ǰ������Ԥ��λ ����1: Ԥ��λID.

	 cusdk__OpLockRecord,		// ����ƽ̨¼��. ����1: ¼����ʼʱ��. ����2: ¼�����ʱ��. ʱ���ʽ��Ϊ��׼ʱ���ַ���. +by lzx@2018-03-06 V2R4B1
 };
 class cusdk__BatchOperationReq : public cusdk__BaseReq
 {
	 xsd__string treeId;	// ��ID, Ϊ����ΪϵͳĬ���豸��.
	 xsd__string groupId;	// ��ID.
	 xsd__boolean recursive;// �Ƿ���¼��������ݹ����.
	 xsd__int operationType;// ��������, ����μ� cusdk__EOperationType.

	 std::vector< xsd__string > paramList;	// �����б�, ���ݲ������Ͳ�ͬ,��Ҫ��ͬ�Ĳ���.
 };

 class cusdk__BatchOperationRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 int cusdk__BatchOperation(cusdk__BatchOperationReq req, cusdk__BatchOperationRsp& rsp);

// class cusdk__GetSearchDvcReq
// {
// 	xsd__string			m_SearchDevName;
// 	xsd__string			m_FeatureCode;//
// 	xsd__int			m_ReceivedNum;		//?channel count
// 	xsd__int 	       m_SerchResLimit;
// 	xsd__int 				m_dwserver;
// };
// 
// class cusdk_SearchVidSrc
// {
// 	xsd__int			m_nSn;				// Video Source id(No.)
// 	xsd__string			m_szSrcName;		// video source alias
// 	xsd__int			m_nPtzLevel;		// video source ptz level
// 	xsd__int			m_IsSearchShow;		// 0 no show 1 show
// };
// 
// class cusdk__SearchDvcInfo
// {
// 	//devices
// 	xsd__int			m_nSn;
// 	xsd__string			m_szdeviceID;
// 	xsd__string			m_szdomainID;
// 	xsd__int			m_nDevSrcNum;
// 	xsd__int			m_nEncoderChnNum;
// 	xsd__string			m_szDevSrcAlias;
// 	xsd__int			m_nDevType;
// 	xsd__int			m_nDevCap;
// 	xsd__int			m_nCallType;
// 	xsd__string			m_szManufacturer;
// 	cusdk_SearchVidSrc	m_aDevSrcChn[64];	// device video source infomation
// 
// 	//group
// 	xsd__string			m_szSelfGroupId;
// 	xsd__string			m_szGroupName;
// 	xsd__int			m_bHashDevice;
// 
// 	//
// 	xsd__string			m_szParentId;	
// 	xsd__int			m_SearDvcType;		//1:group;2:devices;
// 
// };
// 
// class cusdk__GetSearchDvcRsp
// {
// 	xsd__int			m_totalChannelNum;			//?channel count
// 	xsd__int			m_hasReceiveChannelNum;		//?channel count
// 	xsd__int			m_CurrentDvcNum;			//?Dvc count
// 	std::vector<cusdk__SearchDvcInfo> SearchDvcResult;
// };
// int cusdk__GetSearchDvcRes(cusdk__GetSearchDvcReq cSearchDvcReq, cusdk__GetSearchDvcRsp &cSearchDvcRsp);
// 
// 

class cusdk__GroupVSNum
{
	xsd__string groupId;			//�豸��ID
    xsd__int num;                //��ƵԴ����
};

 class cusdk__GetDeviceGroupVideoSrcNumReq : public cusdk__BaseReq
 {
	 xsd__string treeId;						// ��ID, Ϊ����ΪϵͳĬ���豸��.������Ŀǰ����ӿ�ֻ������ͳ���ã����ֵ��ֻ����գ�
	 std::vector< xsd__string > groupIdList;	// ��Ҫ��ѯ��ƵԴ�������豸��ID�б�
 };

 class cusdk__GetDeviceGroupVideoSrcNumRsp
 {
	 std::vector< cusdk__GroupVSNum > grpvsNums;	//�豸�����ƵԴ����
	 xsd__string session;
	 long errorCode;
 };
//������ѯָ���豸���µ�������ƵԴ����
 int cusdk__GetDeviceGroupVideoSrcNum(cusdk__GetDeviceGroupVideoSrcNumReq req, cusdk__GetDeviceGroupVideoSrcNumRsp& rsp);

 
 class cusdk__VideoSrcId
 {
	 xsd__string deviceId;
	 xsd__int vsId;
 };
 
 class cusdk__VSOfflineInfo
 {
	cusdk__VideoSrcId videosrcId;	//��ƵԴID
	xsd__string groupId;		//���ڵĸ�����ID���豸����ʹ��ϵͳ�豸��
	xsd__string devName;	//�����豸���豸����
	xsd__string name;		//����
	xsd__string deviceIP;	//ip��ַ
	double longitude;		// ����
	double latitude;		// γ��	
 };
 
 class cusdk__GetVSOfflineInfoReq : public cusdk__BaseReq
 {	 
 //�豸����ʹ��ϵͳ�豸��
	 std::vector< cusdk__VideoSrcId > videosrclist;	// ��Ҫ��ѯ��ƵԴ�������豸��ID�б�
 };

 class cusdk__GetVSOfflineInfoRsp
 {
	 std::vector< cusdk__VSOfflineInfo > vsOfflineInfo;	//������ƵԴ��Ϣ
	 xsd__string session;
	 long errorCode;
 };
//������ѯ������ƵԴ�������Ϣ
 int cusdk__GetVSOfflineInfo(cusdk__GetVSOfflineInfoReq req, cusdk__GetVSOfflineInfoRsp& rsp);

// 2016-07-18 ��ʱ�汾����, ������: ����,����. 
// int cusdk__GetChildrenGBDomainList(cusdk__GetGBDomainListReq req, cusdk__GetGBDomainListRsp& rsp);

//  class cusdk__GetGBDevicePushDomainReq : public cusdk__BaseReq
//  {
//  };
// 
//  class cusdk__GetGBDevicePushDomainRsp
//  {
// 	 std::vector< xsd__string > domainIdList;	// ѡ������б�.
// 
// 	 xsd__string session;
// 	 long errorCode;
//  };
// 
//  // ��ȡ��ǰ���͵����б�.
//  int cusdk__GetGBDevciePushDomain(cusdk__GetGBDevicePushDomainReq req, cusdk__GetGBDevicePushDomainRsp& rsp);
// 
//  class cusdk__SetGBDevicePushDomainReq : public cusdk__BaseReq
//  {
// 	 std::vector< xsd__string > domainIdList;	// ѡ������б�.
//  };
// 
//  class cusdk__SetGBDevicePushDomainRsp
//  {
// 	 xsd__string session;
// 	 long errorCode;
//  };
// 
//  // ��ȡ��ǰ���͵����б�.
//  int cusdk__SetGBDevciePushDomain(cusdk__SetGBDevicePushDomainReq req, cusdk__SetGBDevicePushDomainRsp& rsp);
// 

 class cusdk__GetDevAzimuthalAngleReq : public cusdk__BaseReq
 {
	 xsd__string deviceId;	// �豸Id
	 int videosrcId;		// ��ƵԴId
 };

 class cusdk__GetDevAzimuthalAngleRsp
 {
	 xsd__string azimuthalAngleData;

	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: ��ȡ��ƵԴ�ķ�λ��
 int cusdk__GetDevAzimuthalAngle(cusdk__GetDevAzimuthalAngleReq req, cusdk__GetDevAzimuthalAngleRsp& rsp);

 class cusdk__DeleteRecordReq : public cusdk__BaseReq
 {
	 //xsd__string recordId;	// ¼��Id.
	 xsd__string deviceId;	// �豸ID.
	 int		 chnId;		// �豸ͨ��ID.
	 cusdk__TimeRange timeRange;	// ʱ���.
	 //xsd__string domainName;	// ¼��������Id. ?
 };

 class cusdk__DeleteRecordRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: ɾ��¼���ļ�
 int cusdk__DeleteRecord(cusdk__DeleteRecordReq req, cusdk__DeleteRecordRsp& rsp);



 // �Զ�ƥ��GBID
 class cusdk__AutoMatchGBIdReq : public cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// �豸��ͼId
	 xsd__string devGroupId;	// �豸��Id, �ݹ��Զ�ƥ���豸�������豸��GBId.
	 //xsd__string devGrpgbid;    // ����gbid

	 xsd__string deviceId;		// �豸Id,����Ϊ��,Ϊ��ʱ�Զ�ƥ������������豸�Ĺ���Id.
	 int devVidIndex;
	 int devMatchType;
	 bool matchLastBits;        // �Ƿ�ƥ��� 5 λ
	 //xsd__string devGbid;
 };

 class cusdk__AutoMatchGBIdRsp
 {
	 xsd__string gbid;
	 xsd__string session;
	 long errorCode;
	 xsd__string operationId; //��չ�ֶ��ɿͻ������ɵ�һ��ΨһId����������ͨ�����Id��ѯ��������Ĵ����������Ӧ���Ȳ�ѯ�ӿڣ�QueryOperationProgress
 };

 //gsoap cusdk service method-documentation: �Զ�ƥ��GBID
 int cusdk__AutoMatchGBId( cusdk__AutoMatchGBIdReq req, cusdk__AutoMatchGBIdRsp& rsp );

 // ��ѯ�����µ�����ͨ����Ŀ.
 class cusdk__GetGroupDevSumNumReq : public cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// �豸����ͼID.
	 xsd__string devGroupId;	// �豸��ID.
 };

 class cusdk__GetGroupDevSumNumRsp
 {
	 int chnNum;			// �豸�������͵�ͨ������.

	xsd__string session;
	long errorCode;
 };

 //gsoap cusdk service method-documentation: ͳ�����͵�ͨ������
 int cusdk__GetGroupDevSumNum( cusdk__GetGroupDevSumNumReq req, cusdk__GetGroupDevSumNumRsp& rsp );

 enum cusdk__EDevicePushChnSelectType
 {
	 cusdk__ModifyDeviceGroupGBID = 0,
	 cusdk__ModifyDeviceGroupPrimary = 1,
	 cusdk__ModifyDeviceGroupSecond = 2,
	 cusdk__ModifyDeviceGroupBoth = 3,
 };

 // CDeviceGroupModReq �޸� priSec ����
 class cusdk__ModifyGBDeviceGroupReq : public cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// �豸����ͼID.
	 xsd__string devGroupId;	// �豸��ID.

	 xsd__string devGroupGBID; // �޸ķ��� gbid
	 int devicePushChnSelectType;	// ������ѡ������, ����� cusdk__EDevicePushChnSelectType. 
 };

 class cusdk__ModifyGBDeviceGroupRsp
 {
	 xsd__string operationId;	// ����ID, ���ڲ�ѯ����. QueryOperationProgress.
	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: һ���豸�����������(ֻ��������, ֻ���͸���, ������������)
 int cusdk__ModifyGBDeviceGroup( cusdk__ModifyGBDeviceGroupReq req, cusdk__ModifyGBDeviceGroupRsp& rsp );

 enum cusdk__EDevicePushChnModifyType
 {
	 cusdk__DevicePushChnModifyTypeNil = 0,
	 cusdk__DevicePushChnModifyTypeAddprimary,
	 cusdk__DevicePushChnModifyTypeAddsecond,
	 cusdk__DevicePushChnModifyTypeDelprimary,
	 cusdk__DevicePushChnModifyTypeDelsecond,
	 cusdk__DevicePushChnModifyTypeModifyprimary,
	 cusdk__DevicePushChnModifyTypeModifysecond
 };
 // �޸�ͨ��GBID ( CDeviceGroupDeviceMod4GbReq )
 class cusdk__ModifyGBDeviceReq : cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// �豸����ͼID.
	 xsd__string devGroupId;	// �豸��ID.

	 xsd__string deviceId;		// �豸ID, 
	 int videoSrcId;			// ��ƵԴID

	 int operationType;			// cusdk__EDevicePushChnModifyType.
	 xsd__string gbID;			// Ҫ�޸ĵĹ���ID.
 };

 class cusdk__ModifyGBDeviceRsp
 {
	 xsd__string devtreeId;               
	 xsd__string devgroupId;               
	 xsd__string deviceId;               
	 int vidIndex; 

	 // CDeviceGroupDeviceMod4GbNtfReq �� gbid
	 xsd__string primaryOriginalGBId;	// ͨ��0(����)ԭGBId.
	 xsd__string secondOriginalGBId;	// ͨ��1(����)ԭGBId.
	 xsd__string primaryNewGBId;		// ͨ��0(����)��GBId.
	 xsd__string secondNewGBId;		// ͨ��1(����)��GBId.

	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: �޸�ͨ��GBID
 int cusdk__ModifyGBDevice(cusdk__ModifyGBDeviceReq req, cusdk__ModifyGBDeviceRsp& rsp);


 // ����һ��������ѡ��������.
 class cusdk__StartDevicePushReq : public cusdk__BaseReq
 {
	 xsd__string parentDomainId;	// Ҫ�����豸����ID
	 xsd__string devTreeViewId;		// Ҫ���͵��豸��ͼID. ��Ϊ��ʱ,Ϊȡ��ѡ��������,ʹ��ȫ����(����ϵͳĬ����).
 };

 class cusdk__StartDevicePushRsp
 {
	 xsd__string session;
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ����һ��������ѡ��������
 int cusdk__StartDevicePush( cusdk__StartDevicePushReq req, cusdk__StartDevicePushRsp& rsp );


 // ����ѡ���������豸��Ϣ. 
 class cusdk__CVideoSrcInfoGB		// (TDevGrpFullGbInfos)
 {
	 int videoSrcId;				// ��ƵԴId
	 xsd__string deviceId;			// �����豸Id
	 xsd__string deviceName;		// �����豸��
	 xsd__string videoSrcName;		// ��ƵԴ��

	 xsd__string primaryOriginalGBId;	// ͨ��0(����)ԭGBId.
	 xsd__string secondOriginalGBId;	// ͨ��1(����)ԭGBId.
	 xsd__string primaryNewGBId;		// ͨ��0(����)��GBId.
	 xsd__string secondNewGBId;		// ͨ��1(����)��GBId.

 };

 class cusdk__CVideoSrcInfoGBInGroup : public cusdk__CVideoSrcInfoGB
 {
	 std::vector< cusdk__DeviceGroupInfo > deviceGroupPath;	// �豸��·��.
 };

 class cusdk__CDeviceInfoGB			// (CDeviceGroupDeviceInfo4Gb)
 {
	 xsd__string deviceTreeViewId;	// �豸��Id
	 xsd__string deviceGroupId;		// �豸��Id
	 xsd__string deviceName;		// �豸��
	 xsd__string deviceId;			// �豸Id(KedaID)
	 std::vector< cusdk__CVideoSrcInfoGB > videoSrcList;	// ��ƵԴ��Ϣ.(TDeviceInfo4Gb)
 };

 // ��ȡ�豸�б�. CDeviceGroupDeviceQry4GbReq
 class cusdk__GetGBDeviceByGroupReq : cusdk__BaseReq
 {
	 //xsd__string userId;	// �û�ID, Ϊ��ʱΪ��ǰ�������û�.
	 xsd__string treeId;	// ��ID, Ϊ����ΪϵͳĬ���豸��.
	 xsd__string groupId;	// �豸��Id.

	 xsd__int beginIndex;	// ��ʼ���.
	 xsd__int num;			// ��������Ҫ������.
 };

 class cusdk__GetGBDeviceByGroupRsp
 {
	 int totalNum;										// �����������豸�������.

	 std::vector< cusdk__CDeviceInfoGB > devList;		// �豸�б�.

	 xsd__string session;
	 long errorCode;
 };
 int cusdk__GetGBDeviceByGroup(cusdk__GetGBDeviceByGroupReq req, cusdk__GetGBDeviceByGroupRsp& rsp);


 // ����GBID�����豸.
 //gsoap cusdk service method-documentation: ����GBID�����豸
 class cusdk__SearchGBDeviceReq : cusdk__BaseReq
 {
	 xsd__string deviceTreeViewId;	// �����豸��ID.
	 xsd__string deviceGBID;		// Ҫ������GBID��ֻ֧�־�ȷ������.
	 xsd__string deviceName;        // Ҫ�������豸����(֧��ģ������).
	 xsd__int    beginIndex;        // ��ʼ����
	 xsd__int    requestNum;        // ���󷵻صĽ������
 };

 class cusdk__SearchGBDeviceRsp
 {
	 std::vector< cusdk__CVideoSrcInfoGBInGroup > searchResult;	// �������.
	 xsd__int totalNum; //�������
	 xsd__string session;
	 long errorCode;
 };

 int cusdk__SearchGBDevice(cusdk__SearchGBDeviceReq req, cusdk__SearchGBDeviceRsp& rsp);

class cusdk__SearchGBDeviceGroupInfo
{
	std::vector< cusdk__DeviceGroupInfo > groupInfo;
};
 // ����Cvilcode�����豸����
 //gsoap cusdk service method-documentation: ����Cvilcode�����豸����
 class cusdk__SearchGBDeviceGroupReq : cusdk__BaseReq
 {
	 xsd__string deviceTreeViewId;	// �����豸��ID.
	 xsd__string civilcode;			// �豸����������.
	 xsd__string groupName;         // Ҫ�������豸����(֧��ģ������).
	 xsd__int    beginIndex;        // ��ʼ����
	 xsd__int    requestNum;        // ���󷵻صĽ������
 };

 class cusdk__SearchGBDeviceGroupRsp
 {
	 std::vector< cusdk__SearchGBDeviceGroupInfo > searchResult;	// �������.
	 xsd__int totalNum; //�������
	 xsd__string session;
	 long errorCode;
 };
 int cusdk__SearchGBDeviceGroup(cusdk__SearchGBDeviceGroupReq req, cusdk__SearchGBDeviceGroupRsp& rsp);


 // ѡ�������ʹ�������
 enum cusdk__DevicePushErrorType
 {
	 DevicePushErrorOk,				// �޴���
	 DevicePushErrorPrimaryGBIdRepeat,		// ��������ID�ظ�
	 DevicePushErrorSecondGBIdRepeat,		// ��������ID�ظ�
 };
 class cusdk__DevicePushErrorInfo
 {
	 int errorType;					// ��������, ����� cusdk__DevicePushErrorType

// 	 xsd__string domainId;			// �豸������Id
// 	 xsd__string devId;				// �豸Id.
// 	 xsd__string devName;			// �豸��
// 	 xsd__string videoSrcName;		// ��ƵԴ��
// 	 int videoSrcId;				// ��ƵԴId
// 	 xsd__string primaryOriginalGBId;	// ͨ��0(����)ԭGBId.
// 	 xsd__string secondOriginalGBId;	// ͨ��1(����)ԭGBId.
// 	 xsd__string primaryNewGBId;		// ͨ��0(����)��GBId.
// 	 xsd__string secondNewGBId;		// ͨ��1(����)��GBId.

	 cusdk__CVideoSrcInfoGBInGroup videoSrc;	// �д������ƵԴ��Ϣ.

	 xsd__string deviceGroupId;			// �������ڵ��豸��.
	 int indexInDeviceGroup;			// ��ƵԴ�����豸������.
	 int errIndex;                         // Ҫ���ҵĴ�����
 };

 class cusdk__GetNextDeviceGBIDErrorReq : cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// �豸��ͼId
	 xsd__string groupId;		// �豸��Id, Ϊ��ʱ��ѯ������.
	 int errIndex;                   // �ϴβ��ҵĴ�����
 };

 class cusdk__GetNextDeviceGBIDErrorRsp
 {
	 std::vector< cusdk__DevicePushErrorInfo > nextError;	// ��һ������, Ϊ�ձ�ʾû�д���.

	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: ��ȡѡ�������͵Ĵ����б�
 int cusdk__GetNextDeviceGBIDError(cusdk__GetNextDeviceGBIDErrorReq req, cusdk__GetNextDeviceGBIDErrorRsp& rsp);

 class cusdk__GetServerConfigReq : cusdk__BaseReq
 {
	 
 };

 class cusdk__GetServerConfigRsp
 {
	 xsd__boolean enableDecoderShowAreaInfo;	// �������Ƿ���ʾ�豸Ͻ����Ϣ.

	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: ��ȡƽ̨�����������Ϣ
 int cusdk__GetServerConfig(cusdk__GetServerConfigReq req, cusdk__GetServerConfigRsp& rsp);


 class cusdk__SetServerConfigReq : cusdk__BaseReq
 {
	 xsd__boolean enableDecoderShowAreaInfo;	// �������Ƿ���ʾ�豸Ͻ����Ϣ.
 };

 class cusdk__SetServerConfigRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: ����ƽ̨�����������Ϣ
 int cusdk__SetServerConfig(cusdk__SetServerConfigReq req, cusdk__SetServerConfigRsp& rsp);

 class cusdk__TVWallClearAlarmReq : public cusdk__BaseReq
 {	
	 //cusdk__DevChannel decoderChn;	// ������ ����ͨ��.
	 xsd__string TVWallId;			// ����ǽId.
	 int tvId;						// ���ӻ�Id.
	 int tvDivId;					// ���ӻ��ֻ���Id.

 };

 class cusdk__TVWallClearAlarmRsp /* : public cusdk__BaseRsp */
 {

	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;

 };
 //gsoap cusdk service method-documentation: ����ǽ�����澯��ǽ�Ļ���, �ָ���ԭ����.
 int cusdk__TVWallClearAlarm( cusdk__TVWallClearAlarmReq req, cusdk__TVWallClearAlarmRsp& rsp );

 // ����ƽ̨¼��
 class cusdk__LockRecordReq : public cusdk__BaseReq
 {	
	 std::vector< cusdk__RecDuration > recordDurationList;	// ��Ҫ������¼��
 };

 class cusdk__LockRecordRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;

 };
 //gsoap cusdk service method-documentation: ����¼��, ��֤¼���¼��������..
 int cusdk__LockRecord( cusdk__LockRecordReq req, cusdk__LockRecordRsp& rsp );

 // ����������ƽ̨¼��
 class cusdk__UnlockRecordReq : public cusdk__BaseReq
 {	
	 std::vector< cusdk__RecDuration > recordDurationList;	// ��Ҫ������¼��
 };

 class cusdk__UnlockRecordRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;

 };
 //gsoap cusdk service method-documentation: ����������¼��, �ͷ�¼��ռ�õĿռ�
 int cusdk__UnlockRecord( cusdk__UnlockRecordReq req, cusdk__UnlockRecordRsp& rsp );


 // ��Ļ��ȡ
 enum cusdk__EOSDState
 {
	 cusdk__OSDStateInvalid,	// 0 ��Чֵ
	 cusdk__OSDStateDisable,	// 1 ����
	 cusdk__OSDStateText,		// 2 �ı�ģʽ����Ļ.
 };

 class cusdk__LongOSDParam
 {
	 int state;                         //	������Ļ״̬ cusdk__EOSDState
	 int leftMargin;                    //	������Ļ��߾� 1-200;
	 int topMargin;                     //	������Ļ�ϱ߾� 1-200;
	 int color;                         //	��ƵԴ��Ļ��ɫ;
	 xsd__string text;                  //	��ƵԴ��Ļ����, UTF8����
 };

 class cusdk__GetPULongOSDReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// �豸ID
	 int videoSrcId;		// ��ƵԴID
 };

 class cusdk__GetPULongOSDRsp /* : public cusdk__BaseRsp */
 {
	 cusdk__LongOSDParam longOSDParam;	//	OSD��Ļ����.

	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ��Ļ��ȡ.
 int cusdk__GetPULongOSD( cusdk__GetPULongOSDReq req, cusdk__GetPULongOSDRsp& rsp );

 // ��Ļ����
 class cusdk__SetPULongOSDReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// �豸ID
	 int videoSrcId;		// ��ƵԴID

	 cusdk__LongOSDParam longOSDParam;	//	OSD��Ļ����.
 };

 class cusdk__SetPULongOSDRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ��Ļ����
 int cusdk__SetPULongOSD( cusdk__SetPULongOSDReq req, cusdk__SetPULongOSDRsp& rsp );


 // ��ȡ�豸��֤��.
 class cusdk__GetDeviceCertificateReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// �豸ID	
 };

 class cusdk__GetDeviceCertificateRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string signatureCert;	// ǩ��֤�� 
	 xsd__string encryptCert;	// ����֤��

	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ��ȡ�豸��֤��.
 int cusdk__GetDeviceCertificate( cusdk__GetDeviceCertificateReq req, cusdk__GetDeviceCertificateRsp& rsp );
 
 // �����豸����ǩ������
 class cusdk__DeviceSignatrueCtrlReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// �豸ID	
	 xsd__boolean enabled;	// �Ƿ�����ǩ��.
 };
 class cusdk__DeviceSignatrueCtrlRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: �����豸����ǩ������.
 int cusdk__DeviceSignatrueCtrl( cusdk__DeviceSignatrueCtrlReq req, cusdk__DeviceSignatrueCtrlRsp& rsp );

 // �����豸�������ܿ���.
 class cusdk__DeviceEncryptionCtrlReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// �豸ID	
	 xsd__boolean enabled;	// �Ƿ�������������
 };
 class cusdk__DeviceEncryptionCtrlRsp /* : public cusdk__BaseRsp */
 {
	
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: �����豸�������ܿ���.
 int cusdk__DeviceEncryptionCtrl( cusdk__DeviceEncryptionCtrlReq req, cusdk__DeviceEncryptionCtrlRsp& rsp );


 // ˫����֤��һ������.
 class cusdk__BidirectionalAuth1Req : public cusdk__BaseReq
 {	
	 xsd__string algorithm;
 };
 class cusdk__BidirectionalAuth1Rsp /* : public cusdk__BaseRsp */
 {
	 xsd__string algorithm;
	 xsd__string random1;

	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ˫����֤��һ������.
 int cusdk__BidirectionalAuth1( cusdk__BidirectionalAuth1Req req, cusdk__BidirectionalAuth1Rsp& rsp );


 // ˫����֤�ڶ�������.
 class cusdk__BidirectionalAuth2Req : public cusdk__BaseReq
 {	
	 xsd__string random1;
	 xsd__string random2;
	 xsd__string serverId;
	 xsd__string sign1;

	 xsd__string algorithm;
 };
 class cusdk__BidirectionalAuth2Rsp /* : public cusdk__BaseRsp */
 {
	 xsd__string random1;
	 xsd__string random2;
	 xsd__string cryptkey;
	 xsd__string sign2;

	 xsd__string algorithm;

	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ˫����֤�ڶ�������.
 int cusdk__BidirectionalAuth2( cusdk__BidirectionalAuth2Req req, cusdk__BidirectionalAuth2Rsp& rsp );

 //˳���������
 enum cusdk__MoveOperation
 {
	 cusdk__MoveInvalid = 0, // ��Чֵ
	 cusdk__InDevice_MoveAfter, //�豸���ƶ�
	 cusdk__InDevice_MoveTop, //�豸���ö�
	 cusdk__InDevice_MoveBottom,//�豸���õ�
	 cusdk__InGroup_MoveAfter, //�������ƶ�
	 cusdk__InGroup_MoveTop, //�������ö�
	 cusdk__InGroup_MoveBottom, //�������õ�
 };

 class cusdk_DeviceVideoSrc
 {   
	 xsd__string deviceId; //�豸Id
	 int videoSrcId; //��ƵԴId��-1ʱ��ʾ�����豸
 };

 //������ƵԴ˳������
 class cusdk__AdjustVideoSrcOrderReq : public cusdk__BaseReq
 {
	 xsd__string treeId; //�豸��Id
	 xsd__string groupId; //��������Id
	 int moveOp; //�������� cusdk__MoveOperation
	 cusdk_DeviceVideoSrc vsMove; //��Ҫ��������ƵԴ
	 cusdk_DeviceVideoSrc vsRefer; /*���ο���׼����ƵԴ��������Ϊcusdk__InDevice_MoveAfter��cusdk__InGroup_MoveAfterʱ��
	 ��Ҫ��д��vsRefer�ֶΣ���ʾ��vsMoved�ƶ���vsRefer֮���ö����õ�ʱ���ֶ���Ч*/
 };

 //������ƵԴ˳��Ӧ��
 class cusdk__AdjustVideoSrcOrderRsp
 {   
	 xsd__string session;
	 long errorCode;
 };

 //������ƵԴ˳��
 int cusdk__AdjustVideoSrcOrder( cusdk__AdjustVideoSrcOrderReq req, cusdk__AdjustVideoSrcOrderRsp& rsp );

 // ��ȡ�澯���ͺ��䶨�� 
 class cusdk__AlarmType
 {
	 int alarmType;				// �澯���Ͷ���. �μ� cusdk__EAlarmType
	 xsd__string alarmTypeName;	// �澯��������.
 };
 class cusdk__GetAlarmTypesReq : public cusdk__BaseReq
 {	
 };

 class cusdk__GetAlarmTypesRsp /* : public cusdk__BaseRsp */
 {
	 std::vector< cusdk__AlarmType > alarmTypes;	// ƽ̨֧�ֵ����и澯���ͼ�������.

	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ��Ļ����
 int cusdk__GetAlarmTypes( cusdk__GetAlarmTypesReq req, cusdk__GetAlarmTypesRsp& rsp );

 class cusdk__EliminateAlarmReq : public cusdk__BaseReq
 {	 
	 xsd__string deviceId;	// �豸ID.
	 int vsId;				// ��ƵԴID, ��vsIdΪ-1ʱ, ��ʾ���������豸�ĸ澯.
 };

 class cusdk__EliminateAlarmRsp
 {
	 xsd__string session;
	 long errorCode;
 };
 // �����豸�澯
 int cusdk__EliminateAlarm(cusdk__EliminateAlarmReq req, cusdk__EliminateAlarmRsp& rsp);

 // ��ȡһ��һ���ķ�������ַ
 class cusdk__GetDeviceRecordsServerUrlReq : public cusdk__BaseReq
 {	 
	 
 };

 class cusdk__GetDeviceRecordsServerUrlRsp
 {
	 std::vector< xsd__string > deviceRecordsServerUrls;

	 xsd__string session;
	 long errorCode;
 };
 // ��ȡһ��һ���ķ�������ַ
 int cusdk__GetDeviceRecordsServerUrl(cusdk__GetDeviceRecordsServerUrlReq req, cusdk__GetDeviceRecordsServerUrlRsp& rsp);


 // ����ƽ̨¼��  +by lzx@2018-03-06 V2R4B1
 class cusdk__LockRecordBatchReq : public cusdk__BaseReq
 {	
	 cusdk__VideoSrcId devVideoSrc;					// ����¼��ļ�ص�. �����豸ID����Ϊ��.
	 cusdk__TimeRange timeRange;						// ����ʱ��Ρ�
 };

 class cusdk__LockRecordBatchRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ����¼��, ��֤¼���¼��������..  +by lzx@2018-03-06 V2R4B1
 int cusdk__LockRecordBatch( cusdk__LockRecordBatchReq req, cusdk__LockRecordBatchRsp& rsp );

 // ����ƽ̨¼��.  +by lzx@2018-03-06 V2R4B1
 class cusdk__UnlockRecordBatchReq : public cusdk__BaseReq
 {	
	 cusdk__VideoSrcId devVideoSrc;					// ����¼��ļ�ص�. �����豸ID����Ϊ��, ��ʾ������������¼��.

	 bool enableTimeRange;								// �Ƿ�ʹ��ʱ��β���. �����õĻ���������ʱ��ε�����¼��.
	 cusdk__TimeRange timeRange;						// ����ʱ��Ρ�
 };

 class cusdk__UnlockRecordBatchRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;

 };
 //gsoap cusdk service method-documentation: ����������¼��,   +by lzx@2018-03-06 V2R4B1
 int cusdk__UnlockRecordBatch( cusdk__UnlockRecordBatchReq req, cusdk__UnlockRecordBatchRsp& rsp );


 // ��ѯ������ƽ̨¼��.  +by lzx@2018-03-06 V2R4B1
 class cusdk__QueryLockedRecordReq : public cusdk__BaseReq
 {	
	 cusdk__VideoSrcId devVideoSrc;					// ����¼��ļ�ص�. �����豸ID����Ϊ��, ��ʾ������������¼��.
	 bool enableTimeRange;								// �Ƿ�ʹ��ʱ��β���. �����õĻ���������ʱ��ε�����¼��.
	 cusdk__TimeRange timeRange;						// ����ʱ��Ρ�

	 int maxResultNum;									// ���ص��������.
 };

 class cusdk__QueryLockedRecordRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;

	 std::vector< cusdk__RecDuration > recordDurationList;	// �����
	 bool hasMoreResult;									// �г������������¼���¼δ����.
 };
 //gsoap cusdk service method-documentation: ��ѯ������ƽ̨¼��  +by lzx@2018-03-06 V2R4B1
 int cusdk__QueryLockedRecord( cusdk__QueryLockedRecordReq req, cusdk__QueryLockedRecordRsp& rsp );


 // ����Ԥ�����û�ȡ.  +by lzx@2018-03-06 V2R4B1
 class cusdk__GetStreamPreDispatchConfigReq : public cusdk__BaseReq
 {	
	 
 };

 class cusdk__GetStreamPreDispatchConfigRsp /* : public cusdk__BaseRsp */
 {
	xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	long errorCode;

	bool enabled;			// �Ƿ���������Ԥ��.
	int streamType;         // ¼���������͡�0-������1-����1��2-����2��3-����3
 };
 //gsoap cusdk service method-documentation: ��ȡ����Ԥ��������  +by lzx@2018-03-06 V2R4B1
 int cusdk__GetStreamPreDispatchConfig( cusdk__GetStreamPreDispatchConfigReq req, cusdk__GetStreamPreDispatchConfigRsp& rsp );


 // ����Ԥ����������.  +by lzx@2018-03-06 V2R4B1
 class cusdk__SetStreamPreDispatchConfigReq : public cusdk__BaseReq
 {	
	 bool enabled;			// �Ƿ���������Ԥ��.
	 int streamType;         // ¼���������͡�0-������1-����1��2-����2��3-����3
 };

 class cusdk__SetStreamPreDispatchConfigRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ��������Ԥ��������  +by lzx@2018-03-06 V2R4B1
 int cusdk__SetStreamPreDispatchConfig( cusdk__SetStreamPreDispatchConfigReq req, cusdk__SetStreamPreDispatchConfigRsp& rsp );

 class cusdk__QueryStreamPreDispatchDeviceReq : public cusdk__BaseReq
 {
	 xsd__int beginIndex;	// ��ʼ���.
	 xsd__int num;			// ��������Ҫ������.
 };
 class cusdk__QueryStreamPreDispatchDeviceRsp /* : public cusdk__BaseRsp */
 {
	 int totalNum;								// ����.
	 std::vector< cusdk__VideoSrcId > videoSrcList;		// ������Ԥ������ƵԴ�б�.

	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: ��ѯ����Ԥ�����豸.  +by lzx@2018-03-06 V2R4B1
 int cusdk__QueryStreamPreDispatchDevice( cusdk__QueryStreamPreDispatchDeviceReq req, cusdk__QueryStreamPreDispatchDeviceRsp& rsp );

 class cusdk__StreamPreDispatchAddDeviceReq : public cusdk__BaseReq
 {	
	 std::vector< cusdk__VideoSrcId > videoSrcList;		// ������Ԥ������ƵԴ�б�.
 };

 class cusdk__StreamPreDispatchAddDeviceRsp /* : public cusdk__BaseRsp */
 {		
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;

	 std::vector< cusdk__VideoSrcId > failedVideoSrcList;		// ���ʧ�ܵ�������Ԥ������ƵԴ�б�.
 };
 // gsoap cusdk service method-documentation: ����Ԥ�����һ���豸  +by lzx@2018-03-06 V2R4B1
 int cusdk__StreamPreDispatchAddDevice( cusdk__StreamPreDispatchAddDeviceReq req, cusdk__StreamPreDispatchAddDeviceRsp& rsp );

 class cusdk__StreamPreDispatchDelDeviceReq : public cusdk__BaseReq
 {	
	 cusdk__VideoSrcId deviceVideoSrc;	// Ҫɾ�����豸��ƵԴ.
 };

 class cusdk__StreamPreDispatchDelDeviceRsp /* : public cusdk__BaseRsp */
 {		
	 xsd__string session;	// ���� , Ϊ�˲����ⷽ�����.
	 long errorCode;
 };
 // gsoap cusdk service method-documentation: ����Ԥ��ɾ��һ���豸.  +by lzx@2018-03-06 V2R4B1
 int cusdk__StreamPreDispatchDelDevice( cusdk__StreamPreDispatchDelDeviceReq req, cusdk__StreamPreDispatchDelDeviceRsp& rsp );

/*
  ����������ͨ����Դ��Ϣ���ɹ���
  1.�ṩ�豸��ƵԴ��Ӧ����ͨ��������Ϣ��
  2.����1080P�ֱ��ʣ��������һ�������������һ��1080P����Դ������ͨ��Ϊԭͨ����
  3.�⳧�̶�������һ�׿ƴ����Դ����ͨ��Ϊԭͨ����
*/
//����������ͨ����Դ��Ϣ
class cusdk__EncChnRes
{
	cusdk__DevChannel                    encChn;         //����������ͨ��

	xsd__string                          manuFactory;    //������
	xsd__int                             videoWidth;     //��Ƶ���
	xsd__int                             videoHeight;    //��Ƶ�߶�
	xsd__string                          videoFormat;    //��Ƶ��ʽ

	xsd__string                          desc;           //������Ϣ
	xsd__string                          reserve;        //Ԥ��
};

class cusdk__GetEncChnResReq : public cusdk__BaseReq
{	
	cusdk__DevChannel                    vidSrcChn;      //��������ƵԴͨ��
};
class cusdk__GetEncChnResRsp /* : public cusdk__BaseRsp */
{
	std::vector<cusdk__EncChnRes>        encChnResList;  //����������ͨ����Դ��Ϣ�б�
	xsd__string                          session;	     //���� , Ϊ�˲����ⷽ�����.
	long                                 errorCode;      //������
};
// gsoap cusdk service method-documentation: ��ȡ��������ƵԴͨ����Դ��Ϣ.  +by Royan@2018-03-13 V2R4B1
int cusdk__GetEncChnRes( cusdk__GetEncChnResReq req, cusdk__GetEncChnResRsp& rsp );

class cusdk__AllocEncChnResReq : public cusdk__BaseReq
{
	xsd__int playId;

	cusdk__EncChnRes                     encChnRes;      //����������ͨ����Դ��Ϣ

	std::vector< cusdk__MediaDescription > mediaDescriptionList; //������󷽵�ý����Ϣ����.
};
class cusdk__AllocEncChnResRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__MediaDescription > mediaDescriptionList; //����������ý����Ϣ����.
	std::vector< cusdk__MediaExtInfo >     mediaExtInfo;	     //ý����չ��Ϣ.
	xsd__string                            session;	             //���� , Ϊ�˲����ⷽ�����.
	long                                   errorCode;            //������
};
//gsoap cusdk service method-documentation: ����������Դ, ��������.  +by Royan@2018-03-13 V2R4B1
int cusdk__AllocEncChnRes( cusdk__AllocEncChnResReq req, cusdk__AllocEncChnResRsp& rsp );

class cusdk__ReleaseEncChnResReq : public cusdk__BaseReq
{
	xsd__int playId;
};
class cusdk__ReleaseEncChnResRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;   //���� , Ϊ�˲����ⷽ�����.
	long        errorCode; //������
};
//gsoap cusdk service method-documentation: ֹͣ����.  +by Royan@2018-03-13 V2R4B1
int cusdk__ReleaseEncChnRes( cusdk__ReleaseEncChnResReq req, cusdk__ReleaseEncChnResRsp& rsp );


class cusdk__GBPuTransdataSsSetReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	// �豸ͨ��.
};
class cusdk__GBPuTransdataSsSetRsp /* : public cusdk__BaseRsp */
{	
	xsd__string session;   //���� , Ϊ�˲����ⷽ�����.
	long        errorCode; //������
};
//gsoap cusdk service method-documentation: ֹͣ����.  +by Royan@2018-03-13 V2R4B1
int cusdk__GBPuTransdataSsSet( cusdk__GBPuTransdataSsSetReq req, cusdk__GBPuTransdataSsSetRsp& rsp );

// ��ȡ�������͵�Ȩ��
class cusdk__GetGBDomainPrivilegeReq : public cusdk__BaseReq
{	 
	xsd__string domainId;		// GB��ID.
};

class cusdk__GetGBDomainPrivilegeRsp
{
	xsd__string domainId;		// ����,����ץ������.

	bool priRecordPlay;		// �Ƿ���¼�񲥷�Ȩ��
	bool priRecordDownload;	// �Ƿ���¼������Ȩ��.
	bool priPTZ;				// �Ƿ���PTZȨ��.

	xsd__string session;
	long errorCode;
};
// ��ȡ�������͵�Ȩ��
int cusdk__GetGBDomainPrivilege(cusdk__GetGBDomainPrivilegeReq req, cusdk__GetGBDomainPrivilegeRsp& rsp);

// ���ù������͵�Ȩ��
class cusdk__SetGBDomainPrivilegeReq : public cusdk__BaseReq
{	 
	xsd__string domainId;

	bool priRecordPlay;		// �Ƿ���¼�񲥷�Ȩ��
	bool priRecordDownload;	// �Ƿ���¼������Ȩ��.
	bool priPTZ;				// �Ƿ���PTZȨ��.
};

class cusdk__SetGBDomainPrivilegeRsp
{
	xsd__string session;
	long errorCode;
};
// ���ù������͵�Ȩ��
int cusdk__SetGBDomainPrivilege(cusdk__SetGBDomainPrivilegeReq req, cusdk__SetGBDomainPrivilegeRsp& rsp);

// ��ȡ���������
class cusdk__GetGBConfigReq : public cusdk__BaseReq
{	 
};

class cusdk__GetGBConfigRsp
{
	xsd__boolean syncLatitudeAndLongitudeFromDevice;			// �Ƿ�ӹ���ǰ��ͬ����γ������

	xsd__string session;
	long errorCode;
};
// ��ȡ���������
int cusdk__GetGBConfig(cusdk__GetGBConfigReq req, cusdk__GetGBConfigRsp& rsp);

// ���ù��������
class cusdk__SetGBConfigReq : public cusdk__BaseReq
{	 
	xsd__boolean syncLatitudeAndLongitudeFromDevice;			// �Ƿ�ӹ���ǰ��ͬ����γ������
};

class cusdk__SetGBConfigRsp
{
	xsd__string session;
	long errorCode;
};
// ���ù��������
int cusdk__SetGBConfig(cusdk__SetGBConfigReq req, cusdk__SetGBConfigRsp& rsp);

// �ϴ�֤��
class cusdk__SetCertificateReq : public cusdk__BaseReq
{
	int type; // 1:�豸֤�� 2���û�֤�� ����ֵ���Ƿ�                
	xsd__string ownerid; // ֤��ӵ����id  �豸֤��ʱΪ�豸��ţ��û�֤��ʱΪ�û����                
	xsd__string validField; // У���ֶ�  �豸֤��ʱΪ����ģ��id���û�֤��ʱΪ�û����֤����                

	xsd__string cert4sign;  // ǩ��֤��PEM��ʽ����                
	xsd__string cert4encry; // ����֤��PEM��ʽ����    
};

class cusdk__SetCertificateRsp
{
	xsd__string session;
	long errorCode;
};

int cusdk__SetCertificate(cusdk__SetCertificateReq req, cusdk__SetCertificateRsp& rsp);

// ��ȡ֤��
class cusdk__GetCertificateReq : public cusdk__BaseReq
{
	xsd__string ownerid; // ֤��ӵ����id  �豸֤��ʱΪ�豸��ţ��û�֤��ʱΪ�û����                
};

class cusdk__GetCertificateRsp
{
	int type; // 1:�豸֤�� 2���û�֤�� ����ֵ���Ƿ�                
	xsd__string ownerid; // ֤��ӵ����id  �豸֤��ʱΪ�豸��ţ��û�֤��ʱΪ�û����                
	xsd__string validField; // У���ֶ�  �豸֤��ʱΪ����ģ��id���û�֤��ʱΪ�û����֤����                

	xsd__string cert4sign;  // ǩ��֤��PEM��ʽ����                
	xsd__string cert4encry; // ����֤��PEM��ʽ����    

	xsd__string session;
	long errorCode;
};

int cusdk__GetCertificate(cusdk__GetCertificateReq req, cusdk__GetCertificateRsp& rsp);
