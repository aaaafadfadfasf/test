/*****************************************************************************
   ģ����      : cms_const 
   �ļ���      : cms_const.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cms_const.h ��������CMSϵͳ�ĳ���
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/22   1.0         liangli        ����
******************************************************************************/

#ifndef _CMS_CONST_H_
#define _CMS_CONST_H_

#include "kdvtype.h"
#include "cms/cms_landesc.h"
#include <string>


enum EAlarmType
{
	cu__AlarmTypeInvalid,	        // ��Ч�ĸ澯����.
	cu__AlarmMove,			        // �ƶ����
	cu__AlarmInput,	                // �澯����.
	cu__AlarmDiskfull,		        // ������.
	cu__AlarmVideoLost,		        // ��ƵԴ��ʧ.
	cu__AlarmIntelligent,	        // ���ܸ澯.
	cu__AlarmVideo,			        // ������Ѳ�澯��
	cu__AlarmExceptionEvent,		//�쳣�¼���Ϣ
	cu__AlarmDefocus,				//�齹���
	cu__AlarmSceneChange,			//�����任
	cu__AlarmRegionInvasion,		//��������
	cu__AlarmRegionLeaving,			//�����뿪
	cu__AlarmObjectTaken,			//��Ʒ��ȡ
	cu__AlarmObjectLeft,			//��Ʒ����
	cu__AlarmPeopleGather,			//��Ա�ۼ�
	cu__AlarmCorgon,				//������
	cu__AlarmAudioAbnormal,			//�����쳣
	cu__AlarmLumaChange,			//���ȱ仯
};

/* �����������壬ҵ��������Ҫ����ʹ�����ǵĳ���ֵ */
const u32 UUID_STRING_LEN = 32;
const u32 USER_NAME_MAX_LEN = 256;
const u32 USER_PASSWORD_MAX_LEN = 256;
const u32 TMP_BUFFER_LEN = 256;

const u32 NAME_MAX_LEN = 256;
const u32 NAME_SHORT_MAX_LEN = 256;
const u32 COMPANY_NAME_MAX_LEN = 256;
// �û�����μ�USER_NAME_MAX_LEN
const u32 DOMAIN_NAME_MAX_LEN = 256;
const u32 MAXLEN_DESC = 255;    //��������󳤶�

const u32 DESCRIPTION_MAX_LEN = 512;
const u32 DEPARTMENT_NUM_MAXLEN = 256;
const u32 TELEPHONE_MAX_LEN = 256;
const u32 EMAIL_MAX_LEN = 256;

#define NTF_BUF_MAX_SIZE            (25*1024)   //25K
#define MAX_NET_NUM	4   // ƽ̨���֧�ֵ�����
#define TOTAL_STREAMS_IN_EVERY_SWITCH_CHANNEL        ( 4 ) // ÿ������ͨ������������
#define CMS_MSG_TIMEOUT             (35)        //35��
#define CMS_PTZ_MAX_PRI_LEV         (10)        //���PTZ����Ȩ�ޣ�10
#define CMS_PTZ_DEFAULT_HOLD_TIME   (60)        //PTZ����Ĭ�ϰѳ�ʱ�䣺60��

typedef std::string TNatProtocolType;
#define NAT_PROTOCOL_TCP            ("TCP")
#define NAT_PROTOCOL_UDP            ("UDP")

/* �����ļ������ڱ�ʶ32λ����64λ���ַ� */
#define CMS_ARCH_X86_32BIT          ("X86")
#define CMS_ARCH_X86_64BIT          ("X86_64")
#define CMS_ARCH_X86_32BIT_ONDISP   ("32bit")
#define CMS_ARCH_X86_64BIT_ONDISP   ("64bit")


/* �豸���� */
const u32 DEVICE_NAME_MAX_LEN = NAME_MAX_LEN;
const u32 DEVICE_ALIAS_MAX_LEN = NAME_SHORT_MAX_LEN;
const s32 DEVICE_TYPE_INVALID = 0; // �Ƿ��豸����  �豸���Ͷ��� ?������Щ����
const s32 DEVICE_TYPE_ENCODER = 1; // �����豸
const s32 DEVICE_TYPE_DECODER = 2; // �����豸
const s32 DEVICE_TYPE_CODECER = 3; // ����豸
const s32 DEVICE_TYPE_TV_WALL = 4; // ����ǽ�豸
const s32 DEVICE_CAP_INVALID = 0;                   // �Ƿ��豸��������
// α������;: a. ��ʾ����ע�����豸�������豸���͵��豸������ b. ��ʾ���豸Ϊ��λ���л�����豸���͵Ļ�������
const s32 DEVICE_CAP_DUMMY = 6;                     // �豸����-α����(������ֵ�̶�Ϊ1)
const s32 DEVICE_CAP_VIDEOSOURCE = 1;               // �豸����-��ƵԴ
const s32 DEVICE_CAP_LOCAL_STORAGE = 2;             // �豸����-���ش洢(����һЩ���������ܾ��д洢��������һЩ������)
const s32 DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT = 3; // �豸����-���ڸ澯
const s32 DEVICE_CAP_ENCODE_CHANNEL = 4;            // �豸����-����ͨ��
const s32 DEVICE_CAP_DECODE_CHANNEL = 5;            // �豸����-����ͨ��
const s32 DEVICE_CAP_DECODE_OUTPUTCHANNEL = 13;     // �豸����-�������ͨ�� ���繫˾��D04F�豸,�;��ж����ͨ��
const s32 DEVICE_CAP_LABEL_ENCODE_NVR = 7;          // �豸������ǩ-NVR
const s32 DEVICE_CAP_LABEL_ENCODE_SVR = 8;          // �豸������ǩ-SVR
const s32 DEVICE_CAP_LABEL_ENCODE_IPC_BULLET = 9;   // �豸������ǩ-IPCǹ��
const s32 DEVICE_CAP_LABEL_ENCODE_IPC_DOME = 10;    // �豸������ǩ-IPC���
const s32 DEVICE_CAP_LABEL_ENCODE_GENERAL = 11;     // �豸������ǩ-��ͨ������
const s32 DEVICE_CAP_LABEL_ENCODE_WIRELESS = 12;    // �豸������ǩ-���߱�����
const s32 DEVICE_CAP_LABEL_GBDEVICE = 14;           // �豸������ǩ-�����豸
const s32 DEVICE_CAP_LABEL_CONFDEVICE = 15;         // �豸������ǩ-�����ն�
const s32 DEVICE_CAP_LABEL_GBRAWSTREAM = 16;        // �豸������ǩ-����ԭʼ����

const int DEVICE_LINEMODE_INTRANET = 0;    //CMU��PU��ͬһ����
const int DEVICE_LINEMODE_EXTRANET = 1;    //CMU��PU�ڲ�ͬ����
const int DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT = 0;    //CMU��NAT�⣬PU��NAT����
const int DEVICE_LINEMODE_CMU_OUTNAT_PU_INNAT = 1;     //CMU��NAT�⣬PU��NAT�ڱ��
const int DEVICE_LINEMODE_CMU_INNAT_PU_OUTNAT = 2;     //CMU��NAT�ڣ�PU��NAT����
const int DEVICE_LINEMODE_CMU_INNAT_PU_INNAT = 4;      //CMU��NAT�ڣ�PU��NAT�ڱ��

const u32 DEVICE_NETADDRESS_MAX_LEN = 40;
const u32 DEVICE_FIXADDRESS_MAX_LEN = 128;
const u32 DEVICE_MANUFACTURE_NAME_MAX_LEN = 64;
const u32 DEVICE_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/* �豸�鳣�� */
const u32 DEVICEGROUP_NAME_MAX_LEN = NAME_MAX_LEN;
const u32 DEVICEGROUP_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/* CU�û����� */
const u32 CUSER_NAME_MAX_LEN = USER_NAME_MAX_LEN;
const u32 CUSER_PASSWORD_MAX_LEN = USER_PASSWORD_MAX_LEN;
const u32 CUSER_TYPE_SUPERADMIN   = 0;   // ��������Ա
const u32 CUSER_TYPE_COMMONADMIN  = 1;   // ����Ա
const u32 CUSER_TYPE_OPERATOR     = 2;   // ����Ա CU�û�����
const u32 CUSER_COMPANY_NAME_MAX_LEN = COMPANY_NAME_MAX_LEN;
const u32 CUSER_DEPARTMENT_NUM_MAXLEN = DEPARTMENT_NUM_MAXLEN;
const u32 CUSER_TELEPHONE_MAX_LEN = TELEPHONE_MAX_LEN;
const u32 CUSER_EMAIL_MAX_LEN = EMAIL_MAX_LEN;
const u32 CUSER_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/* CU�û��鳣�� */
const u32 CUSERGROUP_NAME_MAX_LEN = USER_NAME_MAX_LEN;
const u32 CUSERGROUP_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/* Tas�û����� */
const u32 TASUSER_NAME_MAX_LEN = USER_NAME_MAX_LEN;
const u32 TASUSER_PASSWORD_MAX_LEN = USER_PASSWORD_MAX_LEN;
const u32 TASUSER_COMPANY_NAME_MAX_LEN = COMPANY_NAME_MAX_LEN;
const u32 TASUSER_DEPARTMENT_NUM_MAXLEN = DEPARTMENT_NUM_MAXLEN;
const u32 TASUSER_TELEPHONE_MAX_LEN = TELEPHONE_MAX_LEN;
const u32 TASUSER_EMAIL_MAX_LEN = EMAIL_MAX_LEN;
const u32 TASUSER_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/*��������������*/
const u32 DECODER_MAX_DEC_CAP_PER_HDMI = 16;   //��������ÿ��HDMI������������

//�û���֤��Ϣ����
#define AUTH_DEFAULT_AES_KEY        (s8 *)"kedacom.shanghai"
#define AUTH_DEFAULT_AES_MODE       (2)
#define AUTH_DEFAULT_AES_INITKEY    (s8 *)"2003091919190900"

//{{����������أ�huangzhichun
typedef std::string TMidaType;
#define MEDIA_TYPE_VIDEO      "VIDEO"
#define MEDIA_TYPE_AUDIO      "AUDIO"
#define MEDIA_TYPE_DATA       "DATA"
#define MEDIA_TYPE_VIDEO_LOW  "video"
#define MEDIA_TYPE_AUDIO_LOW  "audio"
#define MEDIA_TYPE_DATA_LOW   "data"
#define MEDIA_TYPE_UNKNOWN    "UNKNOWN"


//{{TMediaTransProto
typedef std::string TMediaTransport;
#define MEDIA_TRANS_RTP       "RTP"         //ʵʱ����Э��,  MediaType[VIDEO] TransProto[RTP] TransChannelList[RTP,RTCP]
                                            //              MediaType[AUDIO] TransProto[RTP] TransChannelList[RTP,RTCP]
#define MEDIA_TRANS_RTSP      "RTSP"
#define MEDIA_TRANS_DATA      "DATA"        //��ͨ�����ݴ���Э�飬MediaType[DATA] TransProto[DATA] TransChannelList[DATA]
#define MEDIA_TRANS_DATARTP   "DATARTP"     //˫ͨ�����ݴ���Э�飬MediaType[DATA] TransProto[DATARTP] TransChannelList[RTP,RTCP]

typedef std::string TLowerTransport;
#define LOWER_TRANS_UDP    "UDP"
#define LOWER_TRANS_TCP    "TCP"
#define LOWER_TRANS_UNKNOWN    "UNKNOWN"

typedef std::string TTransCastMode;
#define TRANS_CAST_MODE_UNICAST   "unicast"
#define TRANS_CAST_MODE_MULTICAST "multicast"
#define TRANS_CAST_MODE_UNKNOWN   "unknown"
//}}TMediaTransProto

typedef std::string TTransChannel;
#define MEDIA_TRANS_CHN_RTP       "RTP"
#define MEDIA_TRANS_CHN_RTCP      "RTCP"
#define MEDIA_TRANS_CHN_DATA      "DATA"
#define MEDIA_TRANS_CHN_UNKNOWN   "UNKNOWN"

//��������
typedef std::string TSwitchType;
#define  SWITCH_TYPE_M2P    "M2P"
#define  SWITCH_TYPE_P2P    "P2P"

//������ʽ
typedef std::string TSwitchMode;
#define  SWITCH_MODE_SNG    "SNG"   //����,��ֱ��
#define  SWITCH_MODE_MUL    "MUL"   //�鲥
#define  SWITCH_MODE_DRT    "DRT"   //����,ֱ��

//ֱ��ģʽ[�豸��ֱ�����������],Ŀǰֻ�������ʵʱ������Ч
typedef int TDirectTransMode;
#define  DIRECTTRANS_MODE_ONDEMAND     0   //����ֱ��
#define  DIRECTTRANS_MODE_ALWAYS       1   //ʼ��ֱ��

typedef std::string TChannelType;
#define  CHANNEL_TYPE_RECVONLY    "recvonly"
#define  CHANNEL_TYPE_SENDONLY    "sendonly"
//#define  CHANNEL_TYPE_SENDRECV    "sendrecv"
//#define  CHANNEL_TYPE_INACTIVE    "inactive"
//#define  CHANNEL_TYPE_UNDEFINED   "undefined"

#define MEDIA_MANUFACTURE_KEDACOM "kedacom"

typedef std::string TMediaStreamType;
#define MEDIA_STREAM_TYPE_ES    "ES"
#define MEDIA_STREAM_TYPE_PS    "PS"
#define MEDIA_STREAM_TYPE_TS    "TS"
#define MEDIA_STREAM_TYPE_RTP_ES "RTP_ES"
#define MEDIA_STREAM_TYPE_RTP_PS "RTP_PS"
#define MEDIA_STREAM_TYPE_RTP_TS "RTP_TS"
//}}�����������

#define INVALID_CHN_NO  0xFFFFFFFF
#define INVALID_NETSEG_NO 0xFFFFFFFF

//�澯��س���
#define    ALARM_RESUME_TIME_INVALIDVAL  "2038-01-01T10:15:14+08:00"  // �澯�ָ�ʱ��Ƿ�ֵ��������ʱ��ʾ�澯�ָ�ʱ����Ч

//�澯��־
#define    ALARM_FLAG_OCCUR         "occur"           //����
#define    ALARM_FLAG_RESTORE       "restore"         //�ָ�

//�澯����
#define    ALARM_TYPE_PIN             "pin"          //����
#define    ALARM_TYPE_MOVE            "move"         //�ƶ����
#define    ALARM_TYPE_VIDLOST         "vid lost"     //��ƵԴ��ʧ
#define    ALARM_TYPE_DISKFULL        "disk full"    //������
//#define    ALARM_TYPE_CHN_UNAVAIL     "chn unavail"  //ͨ��������
#define    ALARM_TYPE_INTELLIGENT     "intelligent"  //����
#define    ALARM_TYPE_VIDEOPOLL       "video poll"  //������ѯ
//��ӻ������ܸ澯������
#define    ALARM_TYPE_EXCEPTION_EVENT "exception event" //�쳣�¼���Ϣ
#define    ALARM_TYPE_DEFOCUS         "defocus"         //�齹���
#define    ALARM_TYPE_SCENE_CHANGE    "sceme change"    //�����任
#define    ALARM_TYPE_REGION_INVASION "region invasion" //��������
#define    ALARM_TYPE_REGION_LEAVING  "region leaving"  //�����뿪
#define    ALARM_TYPE_OBJECT_TAKEN    "object taken"    //��Ʒ��ȡ
#define    ALARM_TYPE_OBJECT_LEFT     "object left"     //��Ʒ����
#define    ALARM_TYPE_PEOPLE_GATHER   "people gather"   //��Ա�ۼ�
#define    ALARM_TYPE_CORGON          "corgon"          //������
#define    ALARM_TYPE_AUDIO_ABNORMAL  "audio abnormal"  //�����쳣
#define    ALARM_TYPE_LUMA_CHANGE     "luma change"     //���ȱ仯

#define    ALARM_TYPE_GPS_OVERSPEED                "gps overspeed"      //gps ����
#define    ALARM_TYPE_SIM_DATA_SERVICE_OVERUSED    "sim �������� ����"    //������������
//����2016
#define    ALARM_TYPE_ILLEGAL_DEV_DISASSEMBLY       "illegal device dissassembly" //�豸�Ƿ����
#define    ALARM_TYPE_DEVICE_OVER_HEAT              "device over heat"            //�豸����
#define    ALARM_TYPE_DEVICE_OVER_COLD              "device over cold"            //�豸����
#define    ALARM_TYPE_ARTIFICIAL_SCENE              "artificial scene"            //�˹���Ƶ
#define    ALARM_TYPE_OBJECT_MOTION                 "object motion"               //�ƶ����
#define    ALARM_TYPE_TRIP_WIRE                     "trip wire"                   //����
#define    ALARM_TYPE_MOVING_AROUND                 "moving around"               //�ǻ����
#define    ALARM_TYPE_INDIVIDUAL_FLOW_STATISTICS    "individual flow statistcs"    //�������
#define    ALARM_TYPE_INDIVIDUAL_THICKNESS_STATISTICS "individual thickness detect" //�ܶ�ͳ��
#define    ALARM_TYPE_VIDEO_EXCEPTION               "video exception"              //��Ƶ�쳣
#define    ALARM_TYPE_MOVING_SWIFTLY                "moving swiftly"               //�����ƶ�
#define    ALARM_TYPE_STORAGE_DEV_DISK_FAULT        "storage device disk fault"    //�洢�豸���̹���
#define    ALARM_TYPE_STORAGE_DEV_FAN_FAULT         "storage device fan fault"    //�洢�豸���ȹ���
#define    ALARM_TYPE_GO_AGAINST_TRAFFIC            "go against traffic"            //������ʻ


//���̼��źų�������
//�û��Զ����ź���ʼֵ
#ifndef USER_SIGRTMIN
#define USER_SIGRTMIN  34
#endif

//�û��Զ����źŽ���ֵ
#ifndef USER_SIGRTMAX
#define USER_SIGRTMAX  64
#endif
//cmsʹ��CMS_SIGNAL_START(44)��CMS_SIGNAL_END(64)��Χ�ڵ��Զ��峣���ź�
#define    CMS_SIGNAL_START           USER_SIGRTMIN+10    //34-44���ܱ�ϵͳ��ʹ��
#define    CMS_SIGNAL_END             USER_SIGRTMAX       //
#define    CMS_SIGNAL_RELAOD_CFG      CMS_SIGNAL_START    //֪ͨ���¼��������ļ�

//����һ����Ч��γ�ȣ�����ѡ��������xml
#define INVALID_LONGITUDE         65533.0
#define INVALID_LATITUDE          65533.0


//��ȡ�澯�������ƶ�Ӧֵ�ӿ�
static u32 ConverAlarmTypeStrToInt( const std::string& AlarmType )
{
	if (ALARM_TYPE_PIN == AlarmType)
	{
		return cu__AlarmInput;
	}
	else if (ALARM_TYPE_MOVE == AlarmType)
	{
		return cu__AlarmMove;
	}
	else if (ALARM_TYPE_VIDLOST == AlarmType)
	{
		return cu__AlarmVideoLost;
	}
	else if (ALARM_TYPE_DISKFULL == AlarmType)
	{
		return cu__AlarmDiskfull;
	}
	else if (ALARM_TYPE_INTELLIGENT == AlarmType)
	{
		return cu__AlarmIntelligent;
	}
	else if (ALARM_TYPE_VIDEOPOLL == AlarmType)
	{
		return cu__AlarmVideo;
	}
	else if (ALARM_TYPE_EXCEPTION_EVENT == AlarmType)
	{
		return cu__AlarmExceptionEvent;
	}
	else if (ALARM_TYPE_DEFOCUS == AlarmType)
	{
		return cu__AlarmDefocus;
	}
	else if (ALARM_TYPE_SCENE_CHANGE == AlarmType)
	{
		return cu__AlarmSceneChange;
	}
	else if (ALARM_TYPE_REGION_INVASION == AlarmType)
	{
		return cu__AlarmRegionInvasion;
	}
	else if (ALARM_TYPE_REGION_LEAVING == AlarmType)
	{
		return cu__AlarmRegionLeaving;
	}
	else if (ALARM_TYPE_OBJECT_TAKEN == AlarmType)
	{
		return cu__AlarmObjectTaken;
	}
	else if (ALARM_TYPE_OBJECT_LEFT == AlarmType)
	{
		return cu__AlarmObjectLeft;
	}
	else if (ALARM_TYPE_PEOPLE_GATHER == AlarmType)
	{
		return cu__AlarmPeopleGather;
	}
	else if (ALARM_TYPE_CORGON == AlarmType)
	{
		return cu__AlarmCorgon;
	}
	else if (ALARM_TYPE_AUDIO_ABNORMAL == AlarmType)
	{
		return cu__AlarmAudioAbnormal;
	}
	else if (ALARM_TYPE_LUMA_CHANGE == AlarmType)
	{
		return cu__AlarmLumaChange;
	}
	else 
	{
		return cu__AlarmTypeInvalid;
	}
}

//ƽ̨2.0�˿�ʹ�ù滮
//STUN (UDP)
const u32 DEFAULT_STUN_SVR_PORT = 3478;

//Natagent�ڲ�socket�˿�
//һ��kedaplayer��Ҫ��װһ��nalib��3���˿ڣ����ַ���1000���˿ڸ�NATAGENTʹ��
//const u32 NATAGENT_INNER_SOCK_PORT_START = 3500;
//const u32 NATAGENT_INNER_SOCK_PORT_END   = 4499;

//PROXY
const u32 PROXY_SIP_UDP_PORT = 5060;
const u32 PROXY_SIP_TCP_PORT = 5060;

//3AS
const u32 TAS_SIP_UDP_PORT = 5061;
const u32 TAS_SIP_TCP_PORT = 5061;

//PMS
const u32 PMS_SIP_UDP_PORT = 5062;
const u32 PMS_SIP_TCP_PORT = 5062;
const u32 PMS_PU_LISTEN_PORT = 5510;
const u32 PMS_REC_LISTEN_PORT = 10086;
const u32 PMS_NMS_LISTEN_PORT = 1727;

//PMCS
const u32 PMCS_SIP_UDP_PORT = 5063;
const u32 PMCS_SIP_TCP_PORT = 5063;

//CMSSDK_GW
const u32 CMSSDK_SIP_UDP_PORT = 5069;
const u32 CMSSDK_SIP_TCP_PORT = 5069;

//CUI 1.0
const u32 CUI_1_OSP_TCP_PORT = 1722;

//CMU
const u32 CMU_SIP_UDP_PORT = 5070;
const u32 CMU_SIP_TCP_PORT = 5070;

//ALS
const u32 ALS_SIP_UDP_PORT = 5071;
const u32 ALS_SIP_TCP_PORT = 5071;

//TVS
const u32 TVS_SIP_UDP_PORT = 5072;
const u32 TVS_SIP_TCP_PORT = 5072;

//UAS
const u32 UAS_SIP_UDP_PORT = 5073;
const u32 UAS_SIP_TCP_PORT = 5073;

//RCS
const u32 RCS_SIP_UDP_PORT = 5074;
const u32 RCS_SIP_TCP_PORT = 5074;

//MPS
const u32 MPS_SIP_UDP_PORT = 5075;
const u32 MPS_SIP_TCP_PORT = 5075;

//NRU
const u32 NRU_SIP_UDP_PORT = 5076;
const u32 NRU_SIP_TCP_PORT = 5076;

//RMS
const u32 RMS_OSP_TCP_PORT = 5080;

// CAPS ͼƬץ��/΢��ƽ̨ ������.
const u32 CAPS_SIP_UDP_PORT = 5090;

// DPS
const u32 DPS_SIP_UDP_PORT = 5095;
const u32 DPS_SIP_CMSSDK_PORT = 5096;
#define  DPS_ROUTER_TCP_URL "tcp://*:5097"
#define  DPS_DEALER_TCP_URL "tcp://*:5098"



//GBS
const u32 GBS_SIP_UDP_PORT = 5511;
const u32 GBS_SIP_TCP_PORT = 5511;
const u32 GBS_SIP_UDP_PORT2 = 5082;
const u32 GBS_SIP_TCP_PORT2 = 5082;

//SECURITY_MODULE
const u32 SECURITY_MODULE_SIP_UDP_PORT = 5083;


//G800 �����˿�
const u32 G800_MMU_PORT = 5600; // Mmu��MmuPort ��ǰ��PUI�ķ�Χ���ڳ�ͻ���գ���������һ�������ϲ��᲼����ô��PUI�ʲ���������
const u32 G800_HTTP_PORT = 5084;  // Mtu��HttpServicePort
const u32 G800_MTU_PORT = 6600; // Mtu��MtuPort

//VTDU SIP�˿ڷ�Χ��5101-5200
const u32 VTDU_SIP_UDP_PORT = 5101;
const u32 VTDU_SIP_TCP_PORT = 5101;
const u32 VTDU_SIP_PORT_END = 5200;
const u32 VTDU_MEDIA_UDP_START_PORT = 40000;
const u32 VTDU_MEDIA_TCP_START_PORT = 40000;
const u32 VTDU_MEDIA_UDP_M2P_PORT = 8002;
const u32 VTDU_MEDIA_TCP_M2P_PORT = 8002;

//CUI SIP�˿ڷ�Χ��5201-5300
const u32 CUI_SIP_UDP_PORT = 5201;
const u32 CUI_SIP_TCP_PORT = 5201;
const u32 CUI_TCP_DAT_RECV_PORT = 5300;
const u32 CUI_SIP_PORT_END = 5300;
const u32 CUI_WS_PORT = 9080;
const u32 CUI_CU_MAX_NUM = 100;

//PUI SIP�˿ڷ�Χ��5301-5400  5511��5600�Ѿ��ֱ�GBS��G800ռ����
const u32 PUI_SIP_UDP_PORT = 5301;
const u32 PUI_SIP_TCP_PORT = 5301;
const u32 PUI_SIP_PORT_END = 5400;
const u32 PUI_VSIP_LISTEN_PORT = 5510;
const u32 PUI_VSIP_PORT_END = 5600;
const u32 PUI_MEDIA_UDP_PORT = 7000;
const u32 PUI_MEDIA_TCP_PORT = 7000;

const u32 OVF_SIP_UDP_PORT = 5401;
const u32 OVF_SIP_TCP_PORT = 5401;
const u32 PUI_OVFUDP_LISTEN_PORT = 3702;  //onvif�豸��udp�˿ڣ�Ĭ��3702
const u32 OVF_MEDIA_UDP_PORT = 7100;
const u32 OVF_MEDIA_TCP_PORT = 7100;

//PUI_GB SIP�˿ڷ�Χ��5800-5899  
const u32 PUIGB_SIP_UDP_PORT = 5800;
const u32 PUIGB_SIP_TCP_PORT = 5800;

//PMC_fcgi SIP�˿ڷ�Χ��5601-5700
const u32 PMC_FCGI_SIP_UDP_PORT = 5601;
const u32 PMC_FCGI_SIP_TCP_PORT = 5601;
const u32 PMC_FCGI_SIP_PORT_END = 5700;


//CBB�ײ�ģ��˿�ʹ���������Χ: 10001-39999,�ϲ�ҵ����ʹ�ø÷�Χ�ڵĶ˿�
const u32 KDM_CBB_PORT_START = 10001;

const u32 KD_DEV_FINDER_UDP_PORT = 10001;//¼����ϵͳ�е��鲥�����˿ڣ�ԭ5690���ָ�Ϊ10001��
const u32 SCS_OSP_TCP_PORT = 10002;

const u32 KDM_CBB_PORT_END = 39999;



/*
CBB�ײ�ģ��Ŀǰ���󶨵Ķ˿���cms1.0���ֲ��䡣

#define NSMNRUPORT      9005    //NRU��NSMͨ�Ŷ˿�
#define NSMVTDUPORT     9006    //VTDU��NSMͨ�Ŷ˿�

osp�ڲ�socket�˿ڣ�20010~20200

medianet 20400~20599

mediapack 11000~12000

rpctrl   20600~20799


֮ǰ����󶨵Ķ˿ڸ�Ϊʹ�ù̶��˿����󶨡�

asf��ʱ����20800~20849

kds��ʱ����20850~20899

kdmfile��ʱ����20900~20949

rpctrl��ʱ����20950~20999

dataswitch�ڲ�ʹ�ö˿�������󶨸�Ϊʹ�ù̶�����Ķ˿�

��ʼ�˿ڣ�21050

�����˿ڣ�21099


nru���ض˿ڣ�24000

nru������ʼ�˿ڣ�25000

����˿�Ϊ¼��˿ڼ�10000����̨nru����2500·����ء�

����ǰ��������ƽ̨�ϵ�ģ��˿ڷ�Χ: 12000 ~ 12500
proxyserver ����ǰ�������˿� 12000��ps ������һֱ�����ö˿ڣ������ű������޸ĸö˿ڣ�
���� gmss �˿� 12001��ps ������һֱ�����ö˿ڣ��ڲ������߼���ǰ�������˿�+1��
iperf ���ٶ˿�  12500������д��  �ö˿�ֻ�ڲ���ʱ��ռ�ã����ٽ�����ռ�øö˿ڣ�

��ǰ�豸���� ProxyServer ʱ��ProxyServer Ϊǰ���豸����50���˿ڣ�����
> �豸��ʲô����������
> 
> ���磺��n��ǰ�������� ProxyServer ��ProxyServer �� 35000 + ( n -
> 1 ) * 50 �� 35000 + n * 50 �˿ڷ������ǰ�ˡ���35000 �� ProxyServer ����ʼ
> �˿ڣ�
> 
> ע��ǰ���豸������е�������ͨ����Ϊ16��ǰ����ʼ�˿�60000����Ƶ����
> ͨ����Ϊ0����Ƶ����ֻ��һ��ͨ������¼�����Ƶͨ����鿴�����豸��������
>                                                                                                                                        
>           |               ǰ�˶˿ڼ���               |  ProxyServer�˿ڼ���                                                            
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   ʵʱ��  |       ��ʼ�˿� + 4 * ͨ���� - 1000       |   ProxyServer ����ǰ����ʼ�˿ںţ���ǰ���豸��������ߵļ��㷽ʽ��ÿ�����ǰ��  
>           |                                          |                                                                                 
>     Ƶ    |                                          |                 һ���˿ڣ�����50���˿��ڣ�����һ����˳����䣩                  
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   ʵʱ��  |           ʵʱ��Ƶ ����Port + 2          |                                                                                 
>           |                                          |                                                                                 
>     Ƶ    |                                          |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   ��Ƶ��  |     ��ʼ�˿� + 4 * 16 + 4 *ͨ���� + 2    |                                                                                 
>           |                                          |                                                                                 
>     ��    |                                          |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   ¼����  |  ��ʼ�˿� + 4 * (������ͨ��+1 +ͨ����) |                                                                                 
>           |                                          |                                                                                 
>     Ƶ    |                  - 1000                  |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>           |                                          |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   ¼����  |             ¼����ƵPort + 2             |                                                                                 
>           |                                          |                                                                                 
>     Ƶ    |                                          |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>           |                                          |                                                                                 
>                                                                                                                                        

*/

///////////////////////////////////////////////////////////////////////////////
//huangzhichun:���ߺ�

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define CMS_MIN(x,y) ((x)<(y)?(x):(y))
#define CMS_MAX(x,y) ((x)>(y)?(x):(y))

#ifndef TRUE_STR
#define TRUE_STR "true"
#endif

#ifndef FALSE_STR
#define FALSE_STR "false"
#endif

#define BoolStr(bVar) (bVar? TRUE_STR : FALSE_STR)
#define BOOL32ToBool(bBool32) (bBool32 ? true:false)
#define BoolToBOOL32(bBool) (bBool ? TRUE:FALSE)

#define VERIFY_AND_BREAK(bResult) \
    if(!(bResult)) \
{ \
    break; \
}

#define VERIFY_AND_RETURN_RET(bResult, nRtnError) \
    if(!(bResult)) \
{ \
    return nRtnError; \
}

#define SAFE_DEL_PTR(ptr) do{ delete (ptr); (ptr) = NULL; } while(false)
#define SAFE_DEL_ARRAY_PTR(ptr) do{ delete[] (ptr); (ptr) = NULL; } while(false)
#endif // _CMS_CONST_H_

#define ARRAY_SIZE(ar) sizeof(ar)/sizeof(ar[0])
//end of file
