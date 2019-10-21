/*****************************************************************************
  ģ����      : �ն�Э��ӿ�ģ��
  �ļ���      : MtCtrl.h
  ����ļ�    :
  �ļ�ʵ�ֹ���:
  ����        : ���
  �汾        : V3.0  Copyright(C) 1997-2005 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2005/6/14    3.0         ���      ����
******************************************************************************/
#ifndef MT_CTRL_H
#define MT_CTRL_H

#include "cbb/osp/osp.h"
#include "kdvtype.h"
#include "cms/pu/vsipdefine.h"
#include "cms/natagent/natagent.h"
//{{{{{{{{{{{{{{{ add by Royan 20140328 �ֱ�����ֵ��
#include <map>
//#include <set>
//}}}}}}}}}}}}}}} add by Royan 20140328 �ֱ�����ֵ��
#ifdef linux
#include <pthread.h>
#endif // _LINUX

using std::map;

//��Ϣ���Ͷ���
#define OPERATION_RESULT_RESP				0x01 //�������Ӧ��[msgbody:TMTIOPResultResp]
#define EVENT_INPUTPIN_STATE_CHANGED		0x02 //���ڸ澯����״̬֪ͨ[msgbody:TMTIEntityState]
#define EVENT_EN_MOTD_STATE_CHANGED			0x03 //�����ƶ����澯״̬֪ͨ[msgbody:TMTIMotDetState]
#define EVENT_CONFIG_CHANGED				0x04 //��������֪ͨ[msgbody:TMTIConfigInfo]
#define EVENT_CONNECTION_LOST				0x05 //����֪ͨ[mtc��mcu�� msgbody:NULL]
#define EVENT_SERIAL_DATA					0x06 //�ն˴�������[msgbody:TMTIDataSend]
#define EVENT_OUTPUTPIN_STATE_CHANGED       0x07 //���ڸ澯���״̬֪ͨ[msgbody:TMTIEntityState]
#define EVENT_SYS_STATE_NOTIFY		        0x08 //ϵͳ״̬֪ͨ[msgbody:u32]
#define EVENT_DEC_SERIAL_DATA				0x09 //��������������[msgbody:TMTISerialDataSend]
#define EVENT_VIDEO_IN_STATE_CHANGED	    0x0A //��ƵԴ��ʧ�澯
#define EVENT_CHANNEL_STATE_NOTIFY			0x0B //ͨ��״̬֪ͨ
#define EVENT_VIDEO_IN_NOTIFY   			0x0C //��Ƶ����֪ͨ
#define EVENT_REC_STATE_NOTIFY		        0x10 //¼���쳣֪ͨ[msgbody:u32 ����Ϊ������,��Ϊ�ָ�����]
#define EVENT_DE_MOTD_STATE_CHANGED			0x13 //�����ƶ����澯״̬֪ͨ[msgbody:TMTIMotDetState]
#define EVENT_VIENC_TYPE_CHANGED			0x14 //��Ƶ�����ʽ����H264�ı�[msgbody:TMTIConfigInfo]
#define EVENT_PINENABLE_CHANGED				0x20	//body:TMTIInputPinState
//NAT��͸���
#define EVENT_START_NAT_DETECT              0x40    //20100317 
#define EVENT_STOP_NAT_DETECT               0x41    
#define CMD_REBOOT_DEVICE					0x50 //�����豸
#define MSG_SET_SYS_TIME_REQ				0x51 //����ϵͳʱ��
#define MSG_SET_CONFIG_REQ					0x52 //���ò�������
#define MSG_START_VIDEO_SEND_REQ			0x53 //��ʼ��Ƶ����
#define MSG_START_VIDEO_RECV_REQ			0x54 //��ʼ��Ƶ����
#define MSG_START_AUDIO_SEND_REQ			0x55 //��ʼ��Ƶ����
#define MSG_START_AUDIO_RECV_REQ			0x56 //��ʼ��Ƶ����
#define MSG_ADD_VIDEO_SEND_DEST_REQ			0x57 //������Ƶ����Ŀ��
#define MSG_REMOVE_VIDEO_SEND_DEST_REQ		0x58 //ɾ����Ƶ����Ŀ��
#define MSG_STOP_VIDEO_SEND_REQ				0x59 //ֹͣ��Ƶ����
#define MSG_STOP_VIDEO_RECV_REQ				0x5A //ֹͣ��Ƶ����
#define MSG_STOP_AUDIO_SEND_REQ				0x5B //ֹͣ��Ƶ����
#define MSG_STOP_AUDIO_RECV_REQ				0x5C //ֹͣ��Ƶ����
#define CMD_SEND_SERIAL_PORT_DATA			0x5D //���ʹ�������
#define CMD_SEND_CAMERA_CTRL_DATA			0x5E //��������ͷ��������
#define CMD_SET_VIDEO_INPUT_PORT			0x5F //������Ƶ����˿�
#define CMD_SET_OUTPUT_PIN_STATE			0x60 //���ø澯���
#define CMD_START_ALARM_TIMER				0x61 //��ʼ�澯��ʱ
#define CMD_STOP_ALARM_TIMER				0x62 //ֹͣ�澯��ʱ
#define CMD_HIDE_ALARM_TIMER				0x63 //���ظ澯��ʱ
#define CMD_LOAD_DEFAULT_SETTINGS			0x64 //�ָ�Ĭ�ϲ���
#define CMD_SAVE_SETTINGS					0x65 //�������
#define MSG_USER_CUSTOM						0x66 //�û��Զ�������
#define MSG_ADD_AUDIO_SEND_DEST_REQ         0x67 //������Ƶ����Ŀ��
#define MSG_REMOVE_AUDIO_SEND_DEST_REQ      0x68 //ɾ����Ƶ����Ŀ��
//�û��Զ�����Ϣ�������ͺ궨��
#define MSG_CONNECT_SUCCESS					0x69    //MCU���ն˽����ɹ�
#define CMD_SEND_KEY_FRAME					0x6A //���͹ؼ�֡
#define MSG_REAL_CONN_IP					0x6B //��ȡ�ն�����ʵ������IP
#define CMD_START_SEND_COM_DATA				0x6C //���û��˷�������ͷ���ش�������
#define MSG_LOAD_FILE_OVER                  0x6D    //�ϴ��ļ����
#define MSG_SET_DEV_GUID                    0x6E //�����豸guid��Ϣ
//#define CMD_SET_VIDEO_DYN_VALUE				0x6F //���ý�������̬�غ�ֵ
#define EVENT_BITCURRENT_STATE_CHANGED      0x6F    //������������״̬֪ͨ
//��Ϣ��Ϊu8,����Ϊͨ����
#define EVENT_START_REC_REQ                 0x70    //StartRec[msgbody:channelid(u8)]
////��Ϣ��Ϊu8,����Ϊͨ����
#define EVENT_STOP_REC_REQ                  0x71    //StopRec[msgbody:channelid(u8)]
//��Ϣ��Ϊ(u8)+(TimelyRecInfo),����Ϊͨ����+��ʱ����
#define EVENT_SET_TIMELY_INFO_REQ           0x72    //SetTimelyRecInfo[msgbody:channelid(u8)+TimelyRecInfo]
//��Ϣ��Ϊ(u8)+(PeriodlyTimeInfo),����Ϊͨ����+���ڲ���
#define EVENT_SET_PERIODLY_INFO_REQ         0x73    //SetPeriodlyRecInfo[msgbody:channelid(u8)+PeriodlyTimeInfo]
//��Ϣ��Ϊ(u8)+(RecChannelBasicInfo),����Ϊͨ����+ͨ������
#define EVENT_SET_CHANNEL_BASIC_INFO_REQ    0x74    // SetChannelRecBasicInfo[msgbody:channelid(u8)+RecChannelBasicInfo]
//��Ϣ��Ϊ(RecSysInfo),����Ϊ¼��ϵͳ
#define EVENT_SET_SYS_INFO_REQ              0x75    //SetRecSysInfo[msgbody:RecSysInfo]
//��Ϣ��Ϊ(u8+ RecStartEndTimeInfo+u8 +u16) ����Ϊͨ����+��ʼ����ʱ��+��ѯ����+��ѯ��Ŀ
#define EVENT_QUIEY_REC_FILE_REQ            0x76    //QueryRecFile[MsgBody:TQueryParam]
//��Ϣ��Ϊu32 ,�������ļ�id
#define EVENT_START_EXPORT_REC_FILE_REQ     0x77    //MsgBody[u32 dwRecFileIndexId+u32 dwIpAddress+u16 wPort]
//��Ϣ��Ϊu8+u8������Ϊ������id�Ͳ�������
#define EVENT_REC_FILE_OPERATION_REQ        0x78    //RecFileOperation[byPlayerId(u8)+byOperationType(u8)]
//��Ϣ��Ϊu32+u8������Ϊ�ļ����¼�id��������
#define EVENT_DEL_REC_FILE_REQ              0x79    //DelRecFile[dwRecFileIndexId(u32+byType(u8)]
//��Ϣ��Ϊu8+u32������Ϊ������id������ʱ��
#define EVENT_SEEK_LOCATION_REQ             0x7A    //RecSeekLocation
//��Ϣ��Ϊu32+u32+u16+u8, ����Ϊ�ļ�id�ţ�ip��ַ���˿ں�,����
#define EVENT_REC_FILE_START_PLAY_REQ       0x7B    //RecFileStartPlay
//��Ϣ��Ϊu8������Ϊ����
#define EVENT_DEL_REC_RECORD_REQ            0x7C    //DelRecRecord
//MCU��MTC
/*��Ϣ��QueryRecFileInfo������ҲΪ��ѯ���*/
#define EVENT_QUIRY_REC_FILE_ACK            0x7D      //��ѯ¼���ļ�Ӧ��[MsgBody:QueryRecFileInfo]
//��Ϣ��ProgressParaInfo����Ϣ���ݽ���
#define EVENT_REC_PROGRESS_NOTIFY           0x7E      //����֪ͨ[MsgBody:ProgressParaInfo]
/*��Ϣ��TMTIConfigInfo�����ֶ�byConfigType��
PARAM_TYPE_TIMELY_INFO ��Ӧ��ʱ¼����Ϣ
PARAM_TYPE_PERIODLY_INFO ��Ӧ����¼����Ϣ
PARAM_TYPE_CHANNEL_BASIC_INFO ��Ӧͨ��������Ϣ
PARAM_TYPE_REC_SYS_INFO    ��Ӧϵͳ��Ϣ  */
#define EVENT_REC_INFO_CHANGED				   0x7F     //¼����Ϣ����֪ͨ[MsgBody:TMTIConfigInfo]
#define EVENT_REC_STATUS_CHANGED               0x80     //¼����״̬�仯֪ͨ[MsgBody:u8+RecPlayerState]ͨ����+״̬
#define EVENT_REC_FILE_START_PLAYERID_ACK      0X81     //������id֪ͨ[MsgBody:TPlayerRsp]
#define EVENT_REC_FILE_EXPORT_PROGRESS_NOTIFY  0X82     //�����ļ���Ϣ֪ͨ[MsgBody:TRecFileExportInfo]
#define EVENT_STOP_EXPORT_REC_FILE_REQ         0x83     //ֹͣ�����ļ�֪ͨ[MsgBody:NULL]
#define EVENT_TIMER_DEAD                       0x84     //��ʱ������֪ͨ[MsgBody:NULL]
#define EVENT_SET_NMS_PARAM_REQ                0x85     //�������ܲ�������[msgbody:TNmsParam]
#define EVENT_VIDEO_SOURCE_STATE_NOTIFY        0x86     //�ն���ƵԴ״̬֪ͨ[MsgBody:channelId(u8)+state(u8)]
//mt
#define EVENT_QUIEY_REC_FILE_NUM_REQ           0x87     //��ѯ���������ĸ���[MsgBody:TQueryParam]
#define EVENT_QUIEY_REC_FILE_NUM_ACK           0x88     //�������������ĸ���(msgbody:u16)
#define EVENT_NEW_CONNECTION_COME              0x89     //�����ӽ���[msgbody:ip(32)+port(16)]
#define EVENT_CONNECTION_PING_ACK              0X90     //��·���pingӦ��[msgbody:ip(32)+port(16)]
#define EVENT_MT_REGEDIT_REQ                   0x91     //�ն�ע������(TDevRegeditInfo)
#define EVENT_MT_REGEDIT_RSP                   0x92     //�ն�ע��Ӧ��(u16)
//mtc��mcu
#define EVENT_FILE_TRANS_ACK                   0x93     //TFileTransRspInfo
#define EVENT_START_FILE_TRANS_REQ             0x94     //TFileTransInfo
#define EVENT_STOP_FILE_TRANS_REQ              0x95     //NULL
#define EVENT_FILE_TRANS_DATA                  0x96     //TFileTransInfo
#define EVENT_TELECOM_ALARM_NOTIFY             0X97     // TMTIAlarmInfo
#define EVENT_TRANSPARENT_MESSAGE              0X98     // TMTITransparentInfo
//ͼƬ��ѯ�������
#define EVENT_QUERY_PIC_FILE_NUM_REQ		   0x99	    //body:TMTIQueryPicFileParam
#define EVENT_QUERY_PIC_FILE_REQ			   0x9A	    //body:TMTIQueryPicFileParam
#define EVENT_DOWNLOAD_PIC_FILE_REQ			   0x9B	    //body:u16 windex+TMTIPicFileInfo
#define EVENT_DEL_PIC_FILE_REQ				   0x9C	    //body:TMTIQueryPicFileInfo
#define EVENT_DEL_PIC_FILEEX_REQ			   0x9D	    //body:TMTIPicFileInfo
#define EVENT_START_SNAPSHOT_REQ			   0x9E	    //body:u8
#define EVENT_STOP_SNAPSHOT_REQ				   0x9F	    //body:u8
#ifdef VSIP_MULTI_CHAN_SUPPORT
#define EVENT_SET_DEV_MULTIENCCHAN_PARAM_REQ	0xA0	//body:TMTISetConfig
#endif
#define MSG_START_VIDEO_RECV_EXREQ			   0xA0     //��ʼ��Ƶ����(��չ����)
#define MSG_START_AUDIO_RECV_EXREQ	           0XA1     //��ʼ��Ƶ����(��չ����)
//GPS���
#define EVENT_SET_TRANSDATA_ACK                0xA3
#define EVENT_GET_TRANSDATA_ACK				   0xA4
#define EVENT_GET_TRANSCFG_ACK				   0xAE
#define EVENT_CREATE_SUBSCRIBE_ACK             0xAF
#define EVENT_QUERY_PIC_FILE_NACK              0xB0    //body:u16 add G.J. 20091222
#define EVENT_GET_SUBSCRIBE_ACK                0xB1
#define EVENT_TRANSPARENT_CHANNEL_TYPE_EX      0xB2
//hzc_Transparent_Chan_20110324{{
#define EVENT_TRANSPARENT_MESSAGE_RESP		   0xB3
//}}
#define EVENT_CU_OPERTION_RESP				   0xB4
#define EVENT_CU_OPERTION_NOTIFY			   0xB5
#define EVENT_PLAY_GPS_RESP					   0xB6
#define EVENT_PLAY_GPS_NOTIFY				   0xB7
#define EVENT_GET_DECODER_STYLE_RESP		   0xB8
#define EVENT_GET_CONFIG_RESP       		   0xB9    //add by Royan 20130131
/*��Ϣ��TMTIRecListQryRsp������Ϊ��ѯ���*/
#define EVENT_QUIRY_REC_LIST_RSP               0xBA      //��ѯ¼������Ӧ��[MsgBody:TMTIRecListQryRsp]
#define EVENT_GET_CUSTOM_EXT_CAPS_RESP  	   0xBB    //�Զ�����չ������
#define EVENT_STREAM_PROP_RSP                  0xBC    
#define EVENT_TRANSPARENT_QUERY_NOTIFY		   0xBE
//shenxin_rec_file_down_notify{{
#define EVENT_REC_FILE_DOWN_NOTIFY			   0xBF
//}}

#define EVENT_SET_OSD_VIDSRC_ACK			   0xC0
#define EVENT_GET_OSD_VIDSRC_ACK			   0xC1
#define EVENT_SET_OSD_CHN_ACK				   0xC2
#define EVENT_GET_OSD_CHN_ACK				   0xC3
#define EVENT_TRANSPARENT_SUBSCRIBE_RESP       0xD1
#define EVENT_TRANSPARENT_SUBSCRIBE_NTF        0xD2
#define EVENT_GET_MATRIX_ACK				   0xE0	//body:TMTIMatrixInfo
#define EVENT_SET_SNAPSHOTNAME_ACK			   0xF1	//body:u16 werror
#define EVENT_CHANGEPASSWORD_ACK			   0xF2	//zlq add �޸�����Ӧ��
#define EVENT_SETBMPOSDINFO_ACK				   0xF3	//zlq add
#define EVENT_TRANSPARENTCHAN_CFG_MSG		   0xF4	//zlq add
#define EVENT_TRANSPARENTCHAN_CTRL_MSG		   0xF5	//zlq add
#define EVENT_DIRECTTRANSCHAN_CHANGED		   0xF6	//zlq add	�ն�ֱ��·���ı�֪ͨ
#define EVENT_CAPBILITY_CHANGED				   0xF7	//zlq add �ն��������ı�֪ͨ
#define EVENT_QUERY_PIC_FILE_NUM_ACK		   0xF8	//body:u16 wnum
#define EVENT_QUERY_PIC_FILE_ACK			   0xF9	//body:TMTIQueryPicFileInfo
#define EVENT_DOWNLOAD_PIC_FILE_ACK			   0xFA	//body:TMTIDownloadPicFileInfo
#define EVENT_DEL_PIC_FILE_ACK				   0xFB	//body:u16 TMTISnapShotResult
#define EVENT_SET_SNAPSHOT_CONFIG_ACK		   0xFC	//body:u16 werror
#define EVENT_SET_MATRIX_CONFIG_ACK			   0xFD	//body:u16 werror
#define EVENT_START_SNAPSHOT_ACK			   0xFE	//body:u16 werror
#define EVENT_STOP_SNAPSHOT_ACK				   0xFF	//body:u16 werror

#define MSG_FILE_TRANS_BEGIN                    0
#define MSG_FILE_TRANS_END                      0xffff

#define MTI_AUDIO_TYPE 0
#define MTI_VIDEO_TYPE 1
#define MTI_BOTH_MEDIA_TYPE  2
#define MSG_SET_BMPOSDINFO						0xF0    //����bmposd��Ϣ zlq add
#define MSG_UPDATE_USERPSW                      0xF1    //�����û����� zlq add
#define MSG_TRANSPARENTCHAN_CFG					0xF2    //͸��ͨ��������Ϣ zlq add
#define MSG_TRANSPARENTCHAN_CTRL				0xF3    //͸��ͨ��������Ϣ zlq add
// �豸��������
#define MTI_DEV_KDM2401			(const char*)"KDM2401"
#define MTI_DEV_KDM2401S		(const char*)"KDM2401S"
#define MTI_DEV_KDM2401L		(const char*)"KDM2401L"
#define MTI_DEV_KDM2402			(const char*)"KDM2402"
#define MTI_DEV_KDM2402S		(const char*)"KDM2402S"
#define MTI_DEV_KDM2400			(const char*)"KDM2400"
#define MTI_DEV_KDM2500			(const char*)"KDM2500"
#define MTI_DEV_KDM2400P		(const char*)"KDM2400P"
#define MTI_DEV_KDM2500P		(const char*)"KDM2500P"
#define MTI_DEV_KDM2418			(const char*)"KDM2418"
#define MTI_DEV_KDM2518			(const char*)"KDM2518"
#define MTI_DEV_KDM2417			(const char*)"KDM2417"
#define MTI_DEV_KDM2428			(const char*)"KDM2404LS"
#define MTI_DEV_KDV8010A		(const char*)"KDV8010A"
#define MTI_DEV_KDV8010C		(const char*)"KDV8010C"
#define MTI_DEV_DEC5			(const char*)"DEC5"
#define MTI_DEV_KDM2560			(const char*)"KDM2560"
#define MTI_DEV_KDM2461			(const char*)"KDM2461"
#define MTI_DEV_KDM2461L		(const char*)"KDM2461L"
#define MTI_DEV_KDM2401LS		(const char*)"KDM2401LS"
#define MTI_DEV_KDM2402LS		(const char*)"KDM2402LS"
#define MTI_DEV_KDM2402L		(const char*)"KDM2402L"
#define MTI_DEV_KDM2440			(const char*)"KDM2440"
#define MTI_DEV_KDM2440P		(const char*)"KDM2440P"
#define MTI_DEV_KDM201_C04		(const char*)"KDM201-C04"
#define MTI_DEV_KDM201_C04P		(const char*)"KDM201-C04P"
#define MTI_DEV_KDM201_C04L		(const char*)"KDM201-C04L"
#define MTI_DEV_KDM201_D04		(const char*)"KDM201-D04"
#define MTI_DEV_KDM2100			(const char*)"KDM2100"
#define MTI_DEV_KDM2401ES		(const char*)"KDM2401ES"
#define MTI_DEV_KDM2404S		(const char*)"KDM2404S"
#define MTI_DEV_KDM2501			(const char*)"KDM2501"
#define MTI_DEV_KDM2462			(const char*)"KDM2462"
#define MTI_DEV_KDM2462L		(const char*)"KDM2462L"
#define MTI_DEV_KDM2462S		(const char*)"KDM2462S"
#define MTI_DEV_KDM2462LS		(const char*)"KDM2462LS"
#define MTI_DEV_KDM2461LS		(const char*)"KDM2461LS"
#define MTI_DEV_KDM2461S		(const char*)"KDM2461S"
//daiyi Bug00003637 2008-07-04 add start
#define MTI_DEV_KDM2110			(const char*)"KDM2110"
#define MTI_DEV_KDM2110E		(const char*)"KDM2110E"
#define MTI_DEV_KDM2121			(const char*)"KDM2121"
//daiyi Bug00003637 2008-07-04 add end
//2009/09/03 G.J. add
#define MTI_DEV_NVR2820         (const char*)"NVR2820"
#define MTI_DEV_NVR2820E        (const char*)"NVR2820E"
#define MTI_DEV_NVR2860         (const char*)"NVR2860"
#define MTI_DEV_KDM201S         (const char*)"KDM201S"
//end add
//add by shenxin 2011-6-20 ���֧��SVR2810[E]�豸����
#define MTI_DEV_SVR2810			(const char*)"SVR2810"
#define MTI_DEV_SVR2810E		(const char*)"SVR2810E"
//end add

//��������
#define MTI_CAPABILITY_ENABLE	1
#define MTI_CAPABILITY_DISABLE	0
#define MTI_CAPOSDCOLOR_16		1
#define MTI_CAPOSDCOLOR_256		2
#define MTI_CAPOSDCOLOR_FONT	4	//��������Ӧ
#define MTI_CAPPRIVTENETWORK	1
#define MTI_CAPOPERATOR			2

//ͼ��ץ����������
#define SNAPSHOT_NAME_INVALID		0
#define SNAPSHOT_NAME_DATETIME		1	// ����ʱ��
#define SNAPSHOT_NAME_DEVALIAS		2	// �豸����
#define SNAPSHOT_NAME_VIDSRCNAME	3	// ��ƵԴ��
#define SNAPSHOT_NAME_ALARMTYPE		4	// �澯����
#define SNAPSHOT_NAME_USERDEFINE	5	// �û��Զ���

//�����豸����
#define MTI_DEV_VIRMT			(const char *)"KDMVMT"//�����ն�
#define MTI_DEV_SMARTSIGHT		(const char *)"S1600e"//smartsight�豸

//��������
#define MTI_CONNECTION_NONE					0x00
#define MTI_CONNECTION_UNICAST				0x01
#define MTI_CONNECTION_MULTICAST			0x02
#define MTI_CONNECTION_BROADCAST			0x03
#define MTI_CONNECTION_TCPSERVER			0x04
#define MTI_CONNECTION_TCPCLIENT			0x05
#define MTI_CONNECTION_TCP					0x06
#define MTI_CONNECTION_SSL					0x07
#define MTI_CONNECTION_UDP                  0x08

//��Ƶ���ݸ�ʽ
#define MTI_VIDEO_TYPE_NONE					0x00
#define MTI_VIDEO_TYPE_MPEG4				0x01
#define MTI_VIDEO_TYPE_H264					0x02
#define MTI_VIDEO_TYPE_MJPEG                0x03
#define MTI_VIDEO_TYPE_H265					0x04
#define MTI_VIDEO_TYPE_SVAC					0x05
#define MTI_VIDEO_TYPE_SN4					0x11
#define MTI_VIDEO_TYPE_H261					0x12
#define MTI_VIDEO_TYPE_H263					0x13

//��Ƶ���ݸ�ʽ
#define MTI_AUDIO_TYPE_NONE					0x00
#define MTI_AUDIO_TYPE_PCMA					0x01
#define MTI_AUDIO_TYPE_PCMU					0x02
#define MTI_AUDIO_TYPE_GSM					0x03
#define MTI_AUDIO_TYPE_G729					0x04
#define MTI_AUDIO_TYPE_ADPCM                0x05
#define MTI_AUDIO_TYPE_G7221C               0x06
#define MTI_AUDIO_TYPE_G722                 0x07 
#define MTI_AUDIO_TYPE_AACLC                0x08 

//ͼ��ֱ���
#define MTI_VIDEO_RESOLUTION_NONE			0x00
#define MTI_VIDEO_RESOLUTION_AUTO			0x01
#define MTI_VIDEO_RESOLUTION_QCIF			0x02
#define MTI_VIDEO_RESOLUTION_CIF			0x03
#define MTI_VIDEO_RESOLUTION_2CIF			0x04
#define MTI_VIDEO_RESOLUTION_4CIF			0x05
#ifdef VSIP_ADD_NEW_RESOLUTION
#define MTI_VIDEO_RESOLUTION_QQCIF			0x06
#define MTI_VIDEO_RESOLUTION_VGA			0x07
#define MTI_VIDEO_RESOLUTION_QVGA			0x08 
//addbeg G.J. 20090901
#define MTI_VIDEO_RESOLUTION_720P           0x09
#define MTI_VIDEO_RESOLUTION_1080P          0x0A
//addend20090901
//add by shenxin
#define MTI_VIDEO_RESOLUTION_QXGA          0x0B
//end add
#define MTI_VIDEO_RESOLUTION_UXGA		   0x0C
#define MTI_VIDEO_RESOLUTION_960P	       0x0D
#endif

#define MTI_VIDEO_RESLUTION_VAL_72         72
#define MTI_VIDEO_RESLUTION_VAL_88         88
#define MTI_VIDEO_RESLUTION_VAL_144        144
#define MTI_VIDEO_RESLUTION_VAL_176        176
#define MTI_VIDEO_RESLUTION_VAL_240        240
#define MTI_VIDEO_RESLUTION_VAL_288        288
#define MTI_VIDEO_RESLUTION_VAL_320        320
#define MTI_VIDEO_RESLUTION_VAL_352        352
#define MTI_VIDEO_RESLUTION_VAL_480        480
#define MTI_VIDEO_RESLUTION_VAL_576        576
#define MTI_VIDEO_RESLUTION_VAL_640        640
#define MTI_VIDEO_RESLUTION_VAL_704        704
#define MTI_VIDEO_RESLUTION_VAL_720        720
#define MTI_VIDEO_RESLUTION_VAL_960        960
#define MTI_VIDEO_RESLUTION_VAL_1080       1080
#define MTI_VIDEO_RESLUTION_VAL_1280       1280
#define MTI_VIDEO_RESLUTION_VAL_1536       1536
#define MTI_VIDEO_RESLUTION_VAL_1920       1920
#define MTI_VIDEO_RESLUTION_VAL_2048       2048
#define MTI_VIDEO_RESLUTION_VAL_2160       2160
#define MTI_VIDEO_RESLUTION_VAL_3480       3480
#define MTI_VIDEO_RESLUTION_VAL_4096       4096

//ͼ������
#define MTI_VIDEO_QUALITY_PRIOR             0x01 //��������
#define MTI_VIDEO_SPEED_PRIOR               0x02 //�ٶ�����

//�������Ͷ���
#define PARAM_TYPE_SYS						0x00 //ϵͳ����
#define PARAM_TYPE_SYS_PUB					0x01 //ϵͳ��������
#define PARAM_TYPE_VIDEO_ENC_NET			0x02 //��Ƶ�����������
#define PARAM_TYPE_VIDEO_ENC				0x03 //��Ƶ�������
#define PARAM_TYPE_VIDEO_ENC_PUB			0x04 //��Ƶ���빫������
#define PARAM_TYPE_VIDEO_ENC_OSD			0x05 //��Ƶ������Ļ����
#define PARAM_TYPE_VIDEO_ENC_MP				0x06 //��Ƶ����໭�����
#define PARAM_TYPE_VIDEO_ENC_INPUTPORT		0x07 //��Ƶ������ƵԴ�˿ڲ���
#define PARAM_TYPE_VIDEO_ENC_POLL			0x08 //��Ƶ������ѯ����
#define PARAM_TYPE_VIDEO_DEC_NET			0x09 //��Ƶ�����������
#define PARAM_TYPE_VIDEO_DEC				0x0A //��Ƶ�������
#define PARAM_TYPE_VIDEO_DEC_PUB			0x0B //��Ƶ���빫������
#define PARAM_TYPE_VIDEO_DEC_RETRANSFER		0x0C //��Ƶ�����ش�����
#define PARAM_TYPE_AUDIO_ENC_NET			0x0D //��Ƶ�����������
#define PARAM_TYPE_AUDIO_ENC				0x0E //��Ƶ�������
#define PARAM_TYPE_AUDIO_DEC_NET			0x0F //��Ƶ�����������
#define PARAM_TYPE_AUDIO_DEC				0x10 //��Ƶ�������
#define PARAM_TYPE_SERIAL_PORT				0x11 //���ڲ���
#define PARAM_TYPE_SERIAL_PORT_PUB			0x12 //���ڹ�������
#define PARAM_TYPE_INPUT_PIN				0x13 //���ڲ���
#define PARAM_TYPE_VIDEO_INPUT				0x14 //��ƵԴ�˿ڲ���
#define PARAM_TYPE_EN_MOTION_DETECT			0x15 //�����ƶ�������
#define PARAM_TYPE_DE_MOTION_DETECT			0x16 //�����ƶ�������
#define PARAM_TYPE_PPPOE					0x17 //PPPOE���Ų���
//ǰ�˴洢start
#define PARAM_TYPE_TIMELY_INFO              0x18 //��ʱ¼����Ϣ
#define PARAM_TYPE_PERIODLY_INFO            0x19 //����¼����Ϣ
#define PARAM_TYPE_CHANNEL_BASIC_INFO       0x20 //ͨ��������Ϣ
#define PARAM_TYPE_REC_SYS_INFO             0x21 //ϵͳ��Ϣ
#define PARAM_NMS_INFO                      0x22 //���ܲ���
//ǰ�˴洢end
#define PARAM_SHADE_INFO                    0x23 //ͼ�����β���
#define PARAM_XIANTELECOM_ALARM_INFO        0X24 //�澯����
#define PARAM_XIANTELECOM_BANNER_INFO       0X25 //�������
#define PARAM_PIC_CONFIG_INFO				0X26 //ͼƬ��Ϣ
#define PARAM_MATRIX_CONFIG_INFO			0x27 //������Ϣ
#define PARAM_PIC_NAMERULE_INFO				0x28 //ץ����Ϣ
#define PARAM_PUNMS_INFO					0x29 //������ز���

//add by shenxin 
//�豸����̨����������
#define MTI_CTRLTYPE_IPCCtrl    0x00001000
#define MTI_CTRLTYPE_NVRStation 0x00002000 
#define MTI_CTRLTYPE_SVR		0x00003000 
#define MTI_CTRLTYPE_WebCtrl    0x00004000 //���������DVS����̨��
#define MTI_CTRLTYPE_IPCWebCtrl 0x00008000 //���������IPCv7����̨��

//�豸����������
#define MTI_DEVTYPE_IPC       0x00001000     
#define MTI_DEVTYPE_NVR       0x00002000 
#define MTI_DEVTYPE_SVR       0x00003000 
#define MTI_DEVTYPE_DVS       0x00004000   
//end

#ifdef VSIP_MULTI_CHAN_SUPPORT
#define PARAM_MULTI_ENCCHAN_INFO            0x30 //˫����ز���
#endif

#define PARAM_PIN_EXT_INFO					0x31 //������չ��
#define PARAM_EXTINPUTPIN_ENABLE_STATUS     0x32 //����ʹ��״̬

#define PARAM_TYPE_MAX_VALUE				0x40 //����������

#define PARAM_TYPE_VIDEO_DECCOMP			0xB5 //���������ı�֪ͨ

//�໭����
#define VSIP_VIDEO_FORMAT_NONE			    0x00 //�޶໭��
#define VSIP_VIDEO_FORMAT_PIP_4			    0x01 //���л�(����1:4)
#define VSIP_VIDEO_FORMAT_PIP_9			    0x02 //���л�(����1:9)
#define VSIP_VIDEO_FORMAT_PIP_16		    0x03 //���л�(����1:16)

//��չ��Ϣ����֪ͨ
#define MTI_XIAN_ALARM_TYPE				    1
#define MTI_TRANSPARENT_CHANNEL_TYPE	    2//[2-30]
#define MTI_CAPABILITY_TYPE				    3
#define MTI_VMT_ALARM_TYPE				    4
#define MTI_TRANSPARENT_DEC2ENC_TYPE	    5
#define MTI_TRANSPARENT_TYPE			    6
#define MTI_TRANSPARENT_CHANNEL_TYPE_EX	    7
//{{{{{{{{{{{{{{{ add by Royan 20140515 ���ܸ澯����
#define MTI_VMT_ALARM_TYPE_EX			    11//todo ��ϵͳ��ȷ�Ϸ���
//}}}}}}}}}}}}}}} add by Royan 20140515 ���ܸ澯����
#define MTI_PINENABLE_CHANGED				10	//����ʹ��״̬�ĸı䣬��չ��Ϣ����Ϊ������״̬

//BEGIN
#define MTI_XIANTELCOM_TYPE  2
//END
//

//��������
#define VSIP_SERIAL_LINE_DRIVER_NONE        0x00
#define VSIP_SERIAL_LINE_DRIVER_RS232       0x01
#define VSIP_SERIAL_LINE_DRIVER_RS422       0x02
#define VSIP_SERIAL_LINE_DRIVER_RS485       0x03

//��Ƶ��������
#define VSIP_AUDIO_INPUTTYPE_LINEIN		    0x00
#define VSIP_AUDIO_INPUTTYPE_MIC		    0x01

//��������ͨ������
#define MTI_SERIAL_STREAM_TYPE_NONE		    0x00 //��ʹ��
#define MTI_SERIAL_STREAM_TYPE_UDP		    0x01 //UDPͨ��
#define MTI_SERIAL_STREAM_TYPE_TCP		    0x02 //TCPͨ��
#define MTI_SERIAL_STREAM_TYPE_VSIP		    0x03 //��Ϣͨ��

//��������Э������
#define MTI_SERIAL_DATA_TYPE_NONE		    0x00 //͸��ͨ��
#define MTI_SERIAL_DATA_TYPE_VSIP		    0x01 //VSIPЭ��

//������������
#define MTI_MEDIA_STREAM_TYPE_UDP		    0x00 //UDPͨ��
#define MTI_MEDIA_STREAM_TYPE_TCP		    0x01 //TCPͨ��

//��������Ƶʱ����ʾ
#define VSIP_PLAYER_STAEVE_MODE_COLOR_BAR	0x00 //��ɫ��
#define VSIP_PLAYER_STAEVE_MODE_LAST_IMAGE	0x01 //��Ļ����
#define VSIP_PLAYER_STAEVE_MODE_BLACK_SCR   0x02 //����
#define VSIP_PLAYER_STAEVE_MODE_SHOW_OSD	0x03 //��Ļ���Ტ��Ļ��ʾ

//��Ƶ����˿�����
#define MTI_VIDEO_OUT_TYPE_C				0x01 //����C����
#define MTI_VIDEO_OUT_TYPE_VGA				0x02 //VGA���

//�澯���״̬
#define MTI_ALARM_DETECT_NONE				0x00 //�����
#define MTI_ALARM_DETECT_ONCE				0x01 //���һ������
#define MTI_ALARM_DETECT_REPEAT				0x02 //ѭ�����
#define MTI_ALARM_DETECT_ALWAYS				0x03 //ʼ�ռ��

//����ͷ����
#define MTI_CAMERA_TYPE_SONY				0x01 //SONY
#define MTI_CAMERA_TYPE_AD					0x02 //AD
#define MTI_CAMERA_TYPE_PELCO				0x03 //PELCO
#define MTI_CAMERA_TYPE_PIH					0x04 //PIH
#define MTI_CAMERA_TYPE_PELCO_P				0x05 //PELCO_P
#define MTI_CAMERA_TYPE_PARCO		        0x06
#define MTI_CAMERA_TYPE_AB					0x07 //
#define MTI_CAMERA_TYPE_YUNTAI			    0x08
#define MTI_CAMERA_TYPE_SAMSUNG				0x09 //SAMSUNG
#define MTI_CAMERA_TYPE_YAAN			    0x0A

#define MTI_CAMERA_TYPE_AD168				0x011// AD168
#define MTI_CAMERA_TYPE_KALATEL				0x012// ������
#define MTI_CAMERA_TYPE_JN					0x013
#define MTI_CAMERA_TYPE_WV_CS650			0x014// ����
#define MTI_CAMERA_TYPE_WV_CS850			0x015// ����
#define MTI_CAMERA_TYPE_ALEC				0x016// ������
#define MTI_CAMERA_TYPE_PANASONIC_O         0x0E //PANASONIC-OLD
#define MTI_CAMERA_TYPE_PANASONIC_N         0x0F //PANASONIC-NEW

//VGA���ˢ��Ƶ��
#define MTI_VGA_OUT_FREQUENCY_60			0x01 //ˢ��Ƶ��60
#define MTI_VGA_OUT_FREQUENCY_75			0x02 //ˢ��Ƶ��75
#define MTI_VGA_OUT_FREQUENCY_85			0x03 //ˢ��Ƶ��85

//�豸���������
#define MTI_NONE		                  0	//��Ƶ����붼��֧��
#define MTI_ENCODER		                  1	//ֻ֧����Ƶ����
#define MTI_DECODER		                  2	//ֻ֧����Ƶ����
#define MTI_BOTH		                  3	//��Ƶ����붼֧��
#define MTI_SMARTSIGHT_ENCODER            4 //SMARTSIGHT��Ƶ����֧��
#define MTI_SMARTSIGHT_DECODER            5 //SMARTSIGHT��Ƶ����֧��
#define MTI_SMARTSIGHT_BOTH               6 //SMARTSIGHT��Ƶ����붼֧��

//��������
#define MTI_DISABLE							0
#define MTI_ENABLE							1
#define MTI_FAIL							0
#define MTI_SUCCESS							1
#define MTI_ERR_NODISK						2    //��Ӳ��
#define MTI_ERR_RECORDING					3    //����¼��
#define MTI_ERR_RECPLAYING					4    //����ͨ������,����������
#define MTI_ERR_FILE_NOT_EXIT				5    //�ļ�������(��·�����޴��ļ�)
#define MTI_ERR_RECLENGTH_ZERO				6    //�ļ���СΪ��(¼��ʱ������)
#define MTI_ERR_UNKNOWN_FAIL				7    //�ļ���СΪ��(¼��ʱ������)
#define MTI_ERR_NVR_NO_BUF_RECORD           8    //NVR¼�񻺴��¼������,����������¼��(ǰ�˷���ʱ)
#define MTI_ERR_NVR_BITRATE_OVERFLOW        9    //�����ʹ���(NVR2820������ֻ֧��64 - 2048������NVR�豸�޴�����)
#define MTI_ERR_INVALID_PU_RECCHN           10   //NVRͨ��������,һ��������NVR��ͨ��������ƵԴ
#define MTI_ERR_NO_VALID_REC_DISK           11   //NVR�޿��õ�¼����̷���
#define MTI_ERR_NOT_SUPPORT                 12   //��֧�ֵĹ���
#define MTI_ERR_PIC_TOO_MUSH				13	 //NVR��ѯץ��ͼƬ���࣬��Ҫ��С��ѯ��Χ
#define MTI_ERR_PU_OVERLOAD                 1000 //ǰ�˳�����
#define MTI_ERR_CREATE_PATH_FAIL            2053 //Ŀ¼����ʧ��(�������Ǵ���ֻ��)
#define MTI_ERR_RECLOG_NOT_EXIST            2072 //reclog�ļ�������
#define MTI_ERR_ASF_STATE_ERROR             15846//����ASF�ļ�״̬����ȷ
#define MTI_ERR_DELETE_NVR_REC              13002//ɾ��NVRǰ��¼��ʧ��(VSƽ̨��֧��ɾ��NVRǰ��¼��)
#define MTI_ERR_NO_GPSDATA					3000  //û���ҵ�gps���� 
#define MTI_ERR_EXIST_PLAY_GPS				3001  //�Ѿ��ڲ���gps����
#define MTI_ERR_PLTFM_FLOWMAX               30001 //3g�����������ֵ

#define MTI_FTP_SERVER_PORT       1730
#define MTI_NAT_LOCAL_SOURCE_PORT 60000

//�豸���緽ʽ
#define MTI_DEVPOWERTYPE_INVALID			(u8)0	/*!< ��Ч���緽ʽ */
#define MTI_DEVPOWERTYPE_STABLE				(u8)1	/*!< ��ѹ��Դ���� */
#define MTI_DEVPOWERTYPE_ETHNET				(u8)2	/*!< ��̫������ */
#define MTI_DEVPOWERTYPE_UPS				(u8)3	/*!< UPS���� */
#define MTI_DEVPOWERTYPE_BATTERY			(u8)4	/*!< ��ع��� */

//��Ļ
#define		MTI_OSD_LABEL0		0			// ��ƵԴ��
#define		MTI_OSD_LABEL1		1			// ��ƵԴ��
#define		MTI_OSD_LABEL2		2			// ��ƵԴ��
#define		MTI_OSD_LABEL3		3			// ��ƵԴ��


#define     MTI_ALARM_LABEL0    10           //�澯��Ļ
#define     MTI_ALARM_LABEL1    11           //�澯��Ļ
#define     MTI_ALARM_LABEL2    12           //�澯��Ļ
#define     MTI_ALARM_LABEL3    13           //�澯��Ļ

#define		MTI_ALARM_PIN		0			//���ڸ澯
#define		MTI_ALARM_MOTION	1			//�ƶ����澯

// �ܾ�ԭ��
#define		MTI_NACK_REASON_OK			0		// ����ɹ�
#define		MTI_NACK_REASON_LOSTPACK	1		// ����
#define		MTI_NACK_REASON_TIMEOUT		2		// ��ʱ
#define		MTI_NACK_REASON_TYPEERR		3		// ���Ͳ�ƥ��
#define		MTI_NACK_REASON_LENERR		4		// ��Ϣͷ���ȳ���
#define		MTI_NACK_REASON_PROTOCOL	6		// Э���ų���
#define		MTI_NACK_REASON_RECVING		7		// ���ڴ���
#define		MTI_NACK_REASON_FILEERR		8		// �ļ�����ȷ
#define     MTI_NACK_REASON_FINISH      9

#define     MTI_MAX_ALARM_ALIAS_LENGTH  32     //���澯������С
#define     MTI_MAX_ALARM_NUM           10      //���澯����
#define     MTI_MAX_SENSOR_NUM          4     //���ɼ���Ԫ����
#define		MTI_MAX_CU_OPERTION_NUM     8192   //cu��pu֮��͸��ͨ��������ݵĳ���
#define     MTI_MAX_STREAM_PROP_LENGTH  128    //��ý�������ֵ����

#define   MAXLEN_DEVICE_MANUFACTURER    (u16)32 //��������Ϣ����󳤶�

#define MTI_RELATE_ENCID_DISABLE	0xFF    //�澯������Ƶ����ͨ����Чֵ
#define MTI_OUTPUT_DISABLE			0xFF	//�澯���������Чֵ
#define DAYS_OF_WEEK				7		//һ�ܵ�����
#define CONN_CHEK_INTERNAL			30000	//��·���Ĭ�ϼ��ʱ��(30s)
#define MAX_SERIALPORT_DATA_LEN		1024	//������ݳ���
#define MAX_STRING_LEN				64		//����ַ�������
#define MAX_VERSION_STRING_LEN      64      //���汾�ַ�������
#define MAX_USER_NUM                256     //����û���Ŀ
#define MAX_VIDEOIN_PORT_NAME_LEN	8		//�����Ƶ����˿�������
#define MAX_VIDEOIN_PORT_LONGNAME_LEN   64		//�����Ƶ������
#define MAX_VIDEOIN_PORT_NUM		32		//�����Ƶ����˿�����
#define MAX_PIC_REGION_NUM			16		//�໭�����������
#define MAX_POLL_PORT_NUM			4		//��ѯ���˿���
#define MAX_NET_ADAPTER_NUM         10      //֧�ֵ�������Ŀ
#define MAX_TIME_PRECISION          8       //ʱ�侫��
#define MAX_MATRIX_PORT_NUM			256		//������˿�����
#define MAX_SEPARATOR_LEN			8		//���ָ�������
#ifdef WIN32
#define MAX_UNIT_NUM				1024	//����豸��,windows
#else
#define MAX_UNIT_NUM                5000    //����豸��,linux
#endif
//�����ƵԴ������չUNIT�� add by Royan 20130628
#define MAX_UNIT_NUM_EXTEND          512     //MAX_UNIT_NUM_EXTEND+g_cPuiConf.m_wMaxVidSrcNum=tMtiStackConf.wMaxPuNum

#define MAX_PATH_LEN				256		//���·������
#define MAX_BMPINFOBUF_LEN			1024	//���buf���� zlq add
#define MAX_ENTITY_TYPE				16	    //�����������
#define MAX_ENTITY_TOTAL_NUM		1800  	//�����������modify G.J.   20091020 (512->1500) 20150127��1500->1800��
#define MAX_OSD_FONT_WIDTH          18      //����ַ����  2014.03.14 ywy
//nvr128
#define MAX_ENTITY_NUM				128      //���ͨ���� modify G.J.  20090903 16 -> 64,20120705 64 -> 128
#define MAX_VIDEO_ENC_NUM			128      //�����Ƶ����ͨ����modify G.J. 20090903 16 -> 64,20120705 64 -> 128
#define MAX_VIDEO_DEC_NUM			64	    //�����Ƶ����ͨ���� modify by Royan 20131031 16 -> 64
//nvr128
#define MAX_AUDIO_ENC_NUM			128      //�����Ƶ����ͨ����modify G.J.  20090903 16 -> 64,20120705 64 -> 128
#define MAX_AUDIO_DEC_NUM			128   	 //�����Ƶ����ͨ����modify G.J.  20090903 16 -> 64,20120705 64 -> 128
#define MAX_VIDEO_INPUT_PORT_NUM	128	    //�����Ƶ����˿���modify G.J.  20090903 16 -> 64, modify by Royan 20140912 64->128
#define MAX_INPUT_PIN_NUM			1024	//��󲢿�����ͨ����modify G.J.  20091020 16 -> 1024
//#define MAX_ONE_CHN_INPUTPIN_NUM    16      //һ���豸ͨ�������Խ���Ĳ��������� add G.J. 20091020
#define MAX_OUTPUT_PIN_NUM			16	    //��󲢿����ͨ����
#define MAX_SERIAL_PORT_NUM			4	    //��󴮿���
#define MAX_PLAYER_NUM              4       //������������
#define MAX_CUSTOMCAP_NUM			256	    //����Զ�����������
#define MAX_BACKENDCAP_NUM          256     //���������������
#define MAX_STREAMPROP_NUM          256     //���������������


//ǰ��¼��Э��ӿں궨��start
#define MAX_REC_FILE_NUM            20    //���¼���ļ���Ŀ
#define MTI_DISK_NUM                16    //�����̷�����Ŀ modify G.J.20090903 MAX_DISK_NUM -> MTI_DISK_NUM ������cmsconst.h�ظ�����
#define MAX_CHANNEL_NUM             128   //���ͨ����Ŀ modify by Royan 20140912 64->128
#define MTI_BUFFER_LENGTH           2048  //buffer��С
#define MAX_FILE_NAME_LENGTH        64    //����ļ�����
#define MAX_FILE_PATH_LENGTH        64    //����ļ�·������
#define MAX_DISK_PARTITION_NAME     64    //��������С
#define MAX_THIRDPARTY_NAME_LEN     512   //���������������б��� 20100317
//�ļ����ǲ���
#define MTI_STRATEGY_STOP_REC                0        //ֹͣ¼��
#define MTI_STRATEGY_COVER_ALL_REC           1        //�������м�¼
#define MTI_STRATEGY_COVER_REC_EXCEPT_ALARM  2        //���ǳ��澯¼����ļ�¼
//����¼�����
#define MTI_NONE_PERIOD                      0           //������¼��
#define MTI_PERIOD_DAYLY                     1           //ÿ��¼��
#define MTI_PERIOD_SELECTION                 2           //����¼��
//¼���ļ���������
#define MTI_OPERATION_NORMAL                 0         //��������
#define MTI_OPERATION_STOP                   1         //ֹͣ����
#define MTI_OPERATION_PAUSE                  2         //��ͣ����
#define MTI_OPERATION_RESUME                 3         //��������
#define MTI_OPERATION_2XFORWARD              4         //2���ٿ������
#define MTI_OPERATION_4XFORWARD              5         //4���ٿ������
#define MTI_OPERATION_2XBACKWARD             6         //2���ٿ��˲���
#define MTI_OPERATION_4XBACKWARD             7         //4���ٿ��˲���
#define MTI_OPERATION_8XFORWARD				 8		   //8���ٿ������
#define MTI_OPERATION_8XBACKWARD			 9		   //8���ٿ��˲���
#define MTI_OPERATION_FRAME					10		   //��֡����
#define MTI_OPERATION_16XFORWARD			11		   // 16���
#define MTI_OPERATION_16XBACKWARD			12		   // 16����

//¼���ļ�/�¼�����/ͼƬ�¼�
#define MTI_TYPE_H264					   0x10 //h264�ļ�����
#define MTI_QUIEY_ALL_EVENT                0    //��ѯ�����¼�
#define MTI_TYPE_PIN_ALARM                 1    //���ڸ澯¼��
#define MTI_TYPE_MOTION_DETECT_ALARM       2    //�ƶ����澯¼��
#define MTI_TYPE_MANUAL                    3    //�ֶ�¼��
#define MTI_TYPE_TIMELY                    4    //��ʱ¼��
#define MTI_TYPE_PERIODLY                  5    //����¼��
#define MTI_QUIEY_FILE                     6    //���ļ���ѯ
//¼����״̬
#define MTI_INITIALIZED                    0     //��ʼ��״̬��0
#define MTI_REC_NORUNNING                  1     //δ¼��
#define MTI_REC_RUNNING                    2     //��¼��
#define MTI_REC_FINISHED                   3     //¼�����
//ͼƬ��ϲ�ѯ
#define MTI_PIC_CHAN_QUERY					1	//ͨ���Ų�ѯ
#define MTI_PIC_TIME_QUERY					2	//ʱ���ѯ
#define MTI_PIC_EVENT_QUERY					4	//�¼���ѯ

//ǰ��¼��Э��ӿں궨��end

#define MAX_DET_AREA_NUM			16  //����ƶ����������
#define MAX_SHADE_AREA_NUM			16  //���ͼ������������
#define MAX_IP_LEN					15  //IP��ַ�ַ�������

//������ʽ
#define MTI_SINGLE_SUBAREA			0x01 //������
#define MTI_MULTI_SUBAREA			0x02 //�����

//�ն���·���ʱ����
#define MT_CONNCTION_CHECK_INTERVAL         10*1000

//operation type
#define OPERATION_START_FILE_TRANS_REQ     0
#define OPERATION_STOP_FILE_TRANS_REQ      1
#define OPERATION_FILE_TRANS_DATA          2
#define OPERATION_START_FILE_TRANS_ACK     3
#define OPERATION_GET_GUID_REQ             4
#define OPERATION_GET_GUID_ACK             5
#define OPERATION_UDP_TRANSPARENT_DATA     6
#define OPERATION_START_RECV_EXTEND_REQ    7
#define OPERATION_INDENTIFY_USERPASSWORD_REQ   8
#define OPERATION_INDENTIFY_USERPASSWORD_ACK   9
//zlq add
#define OPERATION_CHANGE_PASSWORD_REQ   10
#define OPERATION_CHANGE_PASSWORD_ACK   11
#define OPERATION_SETBMPOSD_DATA_REQ	12
#define OPERATION_SETBMPOSD_DATA_ACK	13
#define OPERATION_TRANSPARENT_CFG_MSG	14
#define OPERATION_TRANSPARENT_CTRL_MSG	15
#define OPERATION_DIRECTTRANSCHAN_MSG	16
#define OPERATION_QUERY_PICFILE_REQ		17
#define OPERATION_QUERY_PICFILE_ACK		18
#define OPERATION_QUERY_PICFILE_NUM_REQ	19
#define OPERATION_QUERY_PICFILE_NUM_ACK	20
#define OPERATION_DOWNLOAD_PICFILE_REQ	21
#define OPERATION_DOWNLOAD_PICFILE_ACK	22
#define OPERATION_DEL_PICFILE_REQ		23
#define OPERATION_DEL_PICFILE_ACK		24
#define OPERATION_SET_SNAPSHOTCFG_REQ	25
#define OPERATION_SET_SNAPSHOTCFG_ACK	26
#define OPERATION_GET_SNAPSHOTCFG_REQ	27
#define OPERATION_GET_SNAPSHOTCFG_ACK	28
#define OPERATION_DEL_PICFILEEX_REQ		29
#define OPERATION_SET_MATRIX_REQ		30
#define OPERATION_SET_MATRIX_ACK		31
#define OPERATION_GET_MATRIX_REQ		32
#define OPERATION_GET_MATRIX_ACK		33
#define OPERATION_START_SNAPSHOT_REQ	34
#define OPERATION_START_SNAPSHOT_ACK	35
#define OPERATION_STOP_SNAPSHOT_REQ		36
#define OPERATION_STOP_SNAPSHOT_ACK		37
#define OPERATION_GET_SNAPSHOTNAME_REQ	38
#define OPERATION_GET_SNAPSHOTNAME_ACK	39
#define OPERATION_SET_SNAPSHOTNAME_REQ	40
#define OPERATION_SET_SNAPSHOTNAME_ACK	41
#define OPERATION_SET_PUNMS_REQ			42
#define OPERATION_SET_PUNMS_ACK			43
#define OPERATION_GET_PUNMS_REQ			44
#define OPERATION_GET_PUNMS_ACK			45
#define OPERATION_QUERY_PICFILE_NACK    73

//GPS
#define OPERATION_CREATE_SUBSCRIBE_REQ   67
#define OPERATION_CREATE_SUBSCRIBE_ACK   68
#define OPERATION_GET_SUBSCRIBE_REQ      69
#define OPERATION_GET_SUBSCRIBE_ACK      70
#define OPERATION_START_THIRDPARTY_RECV_REQ 80 //20100317

#define OPERATION_SET_SERIALBMP_DATA_REQ                     0xE1
#define OPERATION_SET_SERIALBMP_DATA_ACK                     0xE2

#define OPERATION_SET_OSD_VIDSRC_REQ	87
#define OPERATION_SET_OSD_VIDSRC_ACK	88
#define OPERATION_GET_OSD_VIDSRC_REQ	89
#define OPERATION_GET_OSD_VIDSRC_ACK	90
#define OPERATION_SET_OSD_CHANNEL_REQ	91
#define OPERATION_SET_OSD_CHANNEL_ACK	92
#define OPERATION_GET_OSD_CHANNEL_REQ	93
#define OPERATION_GET_OSD_CHANNEL_ACK	94

//͸��ͨ������
#define TRANSPARENT_TYPE_CFG			0
#define TRANSPARENT_TYPE_CTRL			1
//add end

//�����ٶ�
#define MTI_RUNSPEED_VERYSLOW    0
#define MTI_RUNSPEED_SLOW        1
#define MTI_RUNSPEED_NORMAL      2
#define MTI_RUNSPEED_FAST        3
#define MTI_RUNSPEED_VERYFAST    4
//������ʽ
#define MTI_RUNMODE_DOWN2UP      0
#define MTI_RUNMODE_LEFT2RIGHT   1
#define MTI_RUNMODE_STILL        2
#define MTI_RUNMODE_STILL_LALIGN    2     //����뾲ֹ
#define MTI_RUNMODE_STILL_RALIGN    3     //�Ҷ��뾲ֹ
#define MTI_RUNMODE_STILL_MIDDLE    4     //���ж��뾲ֹ


//���뷽ʽ
#define MTI_ALIGN_TOLEFT         0
#define MTI_ALIGN_TORIGHT        1
#define MTI_ALIGN_TOCENTER       2

#define  MTI_PU_TRANS_DATA_LEN 2048

#define MAX_RES_NUM             32   //֧�ֵķֱ��ʸ��� add by ywy
#define MAX_VID_FORMAT_NUM      16   //֧�ֵ������Ƶ�ļ���ʽ�� add by ywy

#ifdef WIN32
#pragma pack( push, 1 )
#define PACKED
#endif //WIN32
#ifndef PACKED
#ifdef _VXWORKS_
#define PACKED __attribute__((__packed__))
#endif //_VXWORKS_
#ifdef _LINUX_
#ifndef _LINUXEQUATOR_
#define PACKED __attribute__((packed))
#endif //_LINUXEQUATOR_
#endif //_LINUX_
#ifdef _LINUXEQUATOR_
#define PACKED
#endif //_LINUXEQUATOR_
#endif //#ifndef PACKED


//{{{{{{{{{{{{{{{ add by Royan 20140328 �ֱ�����ֵ��
struct TVidResItem
{
public:
	TVidResItem()
	{
		Clear();
	}
	void Clear()
	{
		resolutionType = MTI_VIDEO_RESOLUTION_NONE;
		height = 0xffff;
		width = 0xffff;
	}
	void SetResolutionType(const u8 byType)
	{
		resolutionType = byType;
	}
	u8 GetResolutionType() const
	{
		return resolutionType;
	}
	void SetHeight(const u16 wHeight)
	{
		//ASSERT(resolutionType == 0);
		height = wHeight;
	}
	u16 GetHeight() const
	{
		return height;
	}
	void SetWidth(const u16 wWidth)
	{
		//ASSERT(resolutionType == 0);
		width = wWidth;
	}
	u16 GetWidth() const
	{
		return width;
	}
	//1.�Ϸ���������ö��ֵ�ȴ�С
	//2.�Ϸ�����С���·���
	//3.�·����ȿ������ʱ�ȸ�
	bool operator<(const TVidResItem&tItem) const
	{
		if (resolutionType == MTI_VIDEO_RESOLUTION_NONE && tItem.resolutionType == MTI_VIDEO_RESOLUTION_NONE)
		{
			return (width < tItem.width) || (width == tItem.width && height < tItem.height); 
		}
		if (resolutionType == MTI_VIDEO_RESOLUTION_NONE)
			return false;
		if (tItem.resolutionType == MTI_VIDEO_RESOLUTION_NONE)
			return true;
		return resolutionType < tItem.resolutionType;
	}

protected:
	//������ǰ�����������ֵ�� VIDEO_RESOLUTION_NONE �����Ը�ֵΪ׼��
	u8 resolutionType;		//�ֱ������ͣ�VIDEO_RESOLUTION_**
	u16 width;				//��
	u16 height;				//��
}PACKED;


#ifdef _NEW_HW //�·ֱ��ʷ���
typedef std::set<TVidResItem> TVidResSet;
typedef std::map<CVidType, TVidResSet> TVidTypeResMap;
//}}}}}}}}}}}}}}} add by Royan 20140328 �ֱ�����ֵ��
#endif

typedef u8 CVidType;
struct TVidResFormatInfo
{
	CVidType byVidFormatType;
	TVidResItem abyVidResItem[MAX_RES_NUM];
	u8 byResItemNum;     //��ǰ�����ʽ֧�ֵķֱ��ʸ�������abyVidResItem����ЧԪ�ظ���
}PACKED;


//֧��¼��������ѯ
/*
1����ѯȫ��ÿ��¼��ֲ���������磬��=2013����=0����=0��
2����ѯָ���·�¼��ֲ���������磬��=2013����=1����=0��
3����ѯָ������¼��ֲ���������磬��=2013����=1����=3��
��Ϊ��Ԫ��ݺţ���ȡֵ��ΧΪ0~12����ȡֵ��ΧΪ0~31��
*/
struct TMTIRecListQryReq
{
    u8 byChanId;       //����ͨ��
    u16 wYear;         //��ѯȫ��ÿ��¼��ֲ���������磬��=2013����=0����=0��
    u8 byMonth;        //��ѯָ���·�¼��ֲ���������磬��=2013����=1����=0��
    u8 byDay;          //��ѯָ������¼��ֲ���������磬��=2013����=1����=3��
    u8 byType;         //�¼����ͣ�MTI_QUIEY_ALL_EVENT��

    void SwapToNetwork()
    {
        wYear = htons(wYear);
    }
    void SwapFromNetwork()
    {
        wYear = ntohs(wYear);
    }
    BOOL PackedToBuf( char * Buf, unsigned short BufLen )
    {
        if( PackedLen() > BufLen)
        {
            return FALSE;
        }

        memcpy( Buf, &byChanId, sizeof(byChanId));
        Buf += sizeof(byChanId);
        memcpy( Buf, &wYear, sizeof(wYear));
        Buf += sizeof(wYear);
        memcpy( Buf, &byMonth, sizeof(byMonth));
        Buf += sizeof(byMonth);
        memcpy( Buf, &byDay, sizeof(byDay));
        Buf += sizeof(byDay);
        memcpy( Buf, &byType,sizeof(byType));

        return TRUE;
    }

    BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
    {
        if( PackedLen() > BufLen)
        {
            return FALSE;
        }

        memcpy( &byChanId, Buf, sizeof(byChanId));
        Buf += sizeof(byChanId);
        memcpy( &wYear, Buf, sizeof(wYear));
        Buf += sizeof(wYear);
        memcpy( &byMonth, Buf, sizeof(byMonth));
        Buf += sizeof(byMonth);
        memcpy( &byDay, Buf, sizeof(byDay));
        Buf += sizeof(byDay);
        memcpy( &byType,Buf, sizeof(byType));

        return TRUE;
    }
    unsigned short PackedLen()
    {
        return (sizeof(byChanId)+sizeof(wYear)+sizeof(byMonth)+sizeof(byDay)+sizeof(byType));
    }
}PACKED;

/*
���ڲ�ѯ1��ÿλ����1���£���0λ����1�·ݣ���1λ����2�·ݣ��Դ����ƣ����ĳλΪ0��������¼�񣻷�����¼�񣩡�����λĬ��Ϊ0��
���ڲ�ѯ2��ÿλ����1�죬��0λ����1�գ���1λ����2�գ��Դ����ƣ����ĳλΪ0��������¼�񣻷�����¼�񣩡�����λĬ��Ϊ0��
���ڲ�ѯ3��ÿλ����1Сʱ����0λ����1ʱ����1λ����2ʱ���Դ����ƣ����ĳλΪ0��������¼�񣻷�����¼�񣩡�����λĬ��Ϊ0��
*/
struct TMTIRecListQryRsp
{
    u32 dwQryResult;       //��ѯ���
    
    void SwapToNetwork()
    {
        dwQryResult = htonl(dwQryResult);
    }
    void SwapFromNetwork()
    {
        dwQryResult = ntohl(dwQryResult);
    }
    BOOL PackedToBuf( char * Buf, unsigned short BufLen )
    {
        if( PackedLen() > BufLen)
        {
            return FALSE;
        }

        memcpy( Buf, &dwQryResult, sizeof(dwQryResult));

        return TRUE;
    }

    BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
    {
        if( PackedLen() > BufLen)
        {
            return FALSE;
        }

        memcpy( &dwQryResult, Buf, sizeof(dwQryResult));

        return TRUE;
    }
    unsigned short PackedLen()
    {
        return (sizeof(dwQryResult));
    }
}PACKED;

//modify by Royan 20140304 ͨ��״̬֪ͨ��Ϣ
typedef struct TMTIChnStatInfo
{
	u16		wEntityID;		//ͨ��id��ID modify G.J. 20091020 u8->u16
	u32 	dwChnStat;		//ͨ��״̬��0��unused��1��online��2��offline
}PACKED TMTI_CHAN_STAT_INFO, *PTMTI_CHAN_STAT_INFO;

//֧��natagentǰ���������ƽ��
#define   VSIP_MAX_MEDIA_ADDR_NUM       8
#define   VSIP_MAX_MEDIA_TYPE_NUM       2

struct TMTIMediaAddr
{
    u32     dwIp;
    u16     wPort;
}PACKED;

struct TMTIMediaChn
{
    TPunchKey       tPunchKey;                                 //��Ψһ��ʶ
    u8				byAddrNum;                                 //��ַ��Ŀ
    TMTIMediaAddr	atAddrList[VSIP_MAX_MEDIA_ADDR_NUM];       //��ַ�б�

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memset( Buf, VSIP_VALUE_TYPE_STRING, sizeof(VSIP_VALUE_TYPE_STRING) );
		Buf += sizeof(VSIP_VALUE_TYPE_STRING);
		memcpy( Buf, &(tPunchKey.m_wLen), sizeof(tPunchKey.m_wLen) );
		Buf += sizeof(tPunchKey.m_wLen);
		memcpy( Buf, &(tPunchKey.m_achKey), sizeof(tPunchKey.m_achKey) );
		Buf += sizeof(tPunchKey.m_achKey);
		memcpy( Buf, &(byAddrNum), sizeof(byAddrNum) );
		Buf += sizeof(byAddrNum);
		for ( s32 nAddrIndex=0; nAddrIndex<byAddrNum; nAddrIndex++ )
		{
			memcpy( Buf, &(atAddrList[nAddrIndex].dwIp), sizeof(atAddrList[nAddrIndex].dwIp) );
			Buf += sizeof(atAddrList[nAddrIndex].dwIp);
			memcpy( Buf, &(atAddrList[nAddrIndex].wPort), sizeof(atAddrList[nAddrIndex].wPort) );
			Buf += sizeof(atAddrList[nAddrIndex].wPort);
		}

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		//memcpy( &VSIP_VALUE_TYPE_STRING, Buf, sizeof(VSIP_VALUE_TYPE_STRING) );
		Buf += sizeof(VSIP_VALUE_TYPE_STRING);
		memcpy( &(tPunchKey.m_wLen), Buf, sizeof(tPunchKey.m_wLen) );
		Buf += sizeof(tPunchKey.m_wLen);
		memcpy( &(tPunchKey.m_achKey), Buf, sizeof(tPunchKey.m_achKey) );
		Buf += sizeof(tPunchKey.m_achKey);
		memcpy( &(byAddrNum), Buf, sizeof(byAddrNum) );
		Buf += sizeof(byAddrNum);
		for ( s32 nAddrIndex=0; nAddrIndex<byAddrNum; nAddrIndex++ )
		{
			memcpy( &(atAddrList[nAddrIndex].dwIp), Buf, sizeof(atAddrList[nAddrIndex].dwIp) );
			Buf += sizeof(atAddrList[nAddrIndex].dwIp);
			memcpy( &(atAddrList[nAddrIndex].wPort), Buf, sizeof(atAddrList[nAddrIndex].wPort) );
			Buf += sizeof(atAddrList[nAddrIndex].wPort);
		}

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(VSIP_VALUE_TYPE_STRING) + sizeof(tPunchKey.m_wLen) + tPunchKey.m_wLen
			+ sizeof(byAddrNum) + (sizeof(u32) + sizeof(u16))*byAddrNum;
		return wPacketLen;
	}
}PACKED;

struct TMTIMediaDesc
{
    TMTIMediaChn    tRtpAddrList;                              //rtp��ַ�б�
    TMTIMediaChn    tRtcpAddrList;                             //rtcp��ַ�б�

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		tRtpAddrList.PackedToBuf(Buf, BufLen);
		Buf += tRtpAddrList.PackedLen();
		tRtcpAddrList.PackedToBuf(Buf, BufLen-tRtpAddrList.PackedLen());

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		tRtpAddrList.UnpackedFromBuf(Buf, BufLen);
		Buf += tRtpAddrList.PackedLen();
		tRtcpAddrList.UnpackedFromBuf(Buf, BufLen-tRtpAddrList.PackedLen());

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = tRtpAddrList.PackedLen() + tRtcpAddrList.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIDevMediaParam
{
    TMTIMediaDesc	tVideoMediaDesc;                           //��Ƶý����Ϣ
    TMTIMediaDesc	tAudioMediaDesc;                           //��Ƶý����Ϣ
}PACKED;

//ý�巢�ͽӿ�
struct TMTIStartSendMediaReq
{
    u16             wChnId;                                    //�豸ͨ��ID
    u8              byMediaType;                               //ý������:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    u8              byTransType;                               //��������:MTI_MEDIA_STREAM_TYPE_UDP/MTI_MEDIA_STREAM_TYPE_TCP
    TMTIMediaDesc	tVideoMediaDesc;                           //��Ƶý����Ϣ
    TMTIMediaDesc	tAudioMediaDesc;                           //��Ƶý����Ϣ

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIStartSendMediaRsp
{
    u16             wChnId;                                    //�豸ͨ��ID
    u8              byMediaType;                               //ý������:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    u8              byTransType;                               //��������:MTI_MEDIA_STREAM_TYPE_UDP/MTI_MEDIA_STREAM_TYPE_TCP    
    TMTIMediaDesc	tVideoMediaDesc;                           //��Ƶý����Ϣ
    TMTIMediaDesc	tAudioMediaDesc;                           //��Ƶý����Ϣ

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIStartSendMediaAck
{
    u16             wChnId;                                    //�豸ͨ��ID
    u8              byMediaType;                               //ý������:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType);
		return wPacketLen;
	}
}PACKED;

struct TMTIStopSendMediaReq
{
    u16             wChnId;                                    //�豸ͨ��ID
    u8              byMediaType;                               //ý������:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType);
		return wPacketLen;
	}
}PACKED;

//typedef    TMTIOPResultResp   TMTIStopSendMediaRsp;            //ʹ��ͨ��Ӧ��

//ý����սӿ�
struct TMTIStartRecvMediaReq
{
    u16             wChnId;                                    //�豸ͨ��ID
    u8              byMediaType;                               //ý������:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    u8              byTransType;                               //��������:MTI_MEDIA_STREAM_TYPE_UDP/MTI_MEDIA_STREAM_TYPE_TCP

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);
		memcpy( Buf, &byTransType, sizeof(byTransType) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);
		memcpy( &byTransType, Buf, sizeof(byTransType) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType) + sizeof(byTransType);
		return wPacketLen;
	}
}PACKED;

struct TMTIStartRecvMediaRsp
{
    u16             wChnId;                                    //�豸ͨ��ID
    u8              byMediaType;                               //ý������:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    TMTIMediaDesc	tVideoMediaDesc;                           //��Ƶý����Ϣ
    TMTIMediaDesc	tAudioMediaDesc;                           //��Ƶý����Ϣ

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIStartRecvMediaAck
{
    u16             wChnId;                                    //�豸ͨ��ID
    u8              byMediaType;                               //ý������:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    TMTIMediaDesc	tVideoMediaDesc;                           //��Ƶý����Ϣ
    TMTIMediaDesc	tAudioMediaDesc;                           //��Ƶý����Ϣ

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIStopRecvMediaReq
{
    u16             wChnId;                                    //�豸ͨ��ID
    u8              byMediaType;                               //ý������:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType);
		return wPacketLen;
	}
}PACKED;

//typedef    TMTIOPResultResp   TMTIStopRecvMediaRsp;            //ʹ��ͨ��Ӧ��

//¼�񲥷�
struct TMTIRecStartPlayReq
{
    u32 dwIndexId;                                             //�ļ����
    u8  byType;                                                //�ļ�����
    u32 dwStartTime;                                           //��ʼʱ��
    u32 dwEndTime;                                             //����ʱ��
    u8              byTransType;                               //��������:MTI_MEDIA_STREAM_TYPE_UDP/MTI_MEDIA_STREAM_TYPE_TCP
    TMTIMediaDesc	tVideoMediaDesc;                           //��Ƶý����Ϣ
    TMTIMediaDesc	tAudioMediaDesc;                           //��Ƶý����Ϣ

	void SwapToNetwork()
	{
		dwIndexId = htonl(dwIndexId);
		dwStartTime = htonl(dwStartTime);
		dwEndTime = htonl(dwEndTime);
	}

	void SwapFromNetwork()
	{
		dwIndexId = ntohl(dwIndexId);
		dwStartTime = ntohl(dwStartTime);
		dwEndTime = ntohl(dwEndTime);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwIndexId, sizeof(dwIndexId) );
		Buf += sizeof(dwIndexId);
		memcpy( Buf, &byType, sizeof(byType) );
		Buf += sizeof(byType);
		memcpy( Buf, &dwStartTime, sizeof(dwStartTime) );
		Buf += sizeof(dwStartTime);
		memcpy( Buf, &dwEndTime, sizeof(dwEndTime) );
		Buf += sizeof(dwEndTime);

		u16 wLen = BufLen - sizeof(dwIndexId) - sizeof(byType) - sizeof(dwStartTime) - sizeof(dwEndTime);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwIndexId, Buf, sizeof(dwIndexId) );
		Buf += sizeof(dwIndexId);
		memcpy( &byType, Buf, sizeof(byType) );
		Buf += sizeof(byType);
		memcpy( &dwStartTime, Buf, sizeof(dwStartTime) );
		Buf += sizeof(dwStartTime);
		memcpy( &dwEndTime, Buf, sizeof(dwEndTime) );
		Buf += sizeof(dwEndTime);

		u16 wLen = BufLen - sizeof(dwIndexId) - sizeof(byType) - sizeof(dwStartTime) - sizeof(dwEndTime);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(dwIndexId) + sizeof(byType) + sizeof(dwStartTime) + sizeof(dwEndTime)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIRecStartPlayRsp
{
    u32             dwPlayId;                                  //����ID
    TMTIMediaDesc	tVideoMediaDesc;                           //��Ƶý����Ϣ
    TMTIMediaDesc	tAudioMediaDesc;                           //��Ƶý����Ϣ

	void SwapToNetwork()
	{
		dwPlayId = htonl(dwPlayId);
	}

	void SwapFromNetwork()
	{
		dwPlayId = ntohl(dwPlayId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwPlayId, sizeof(dwPlayId) );
		Buf += sizeof(dwPlayId);

		u16 wLen = BufLen - sizeof(dwPlayId);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwPlayId, Buf, sizeof(dwPlayId) );
		Buf += sizeof(dwPlayId);

		u16 wLen = BufLen - sizeof(dwPlayId);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(dwPlayId) + tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIRecStartPlayAck
{
    u32             dwPlayId;                                  //����ID

	void SwapToNetwork()
	{
		dwPlayId = htonl(dwPlayId);
	}

	void SwapFromNetwork()
	{
		dwPlayId = ntohl(dwPlayId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwPlayId, sizeof(dwPlayId) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwPlayId, Buf, sizeof(dwPlayId) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(dwPlayId);
		return wPacketLen;
	}
}PACKED;

//¼������
struct TMTIRecStartExportReq
{
    u32 dwIndexId;                                             //�ļ����
    u8  byConnectType;                                         //TCP/UDP
    TMTIMediaChn tAddrList;                                    //��ַ�б�

	void SwapToNetwork()
	{
		dwIndexId = htonl(dwIndexId);
	}

	void SwapFromNetwork()
	{
		dwIndexId = ntohl(dwIndexId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwIndexId, sizeof(dwIndexId) );
		Buf += sizeof(dwIndexId);
		memcpy( Buf, &byConnectType, sizeof(byConnectType) );
		Buf += sizeof(byConnectType);

		u16 wLen = BufLen - sizeof(dwIndexId) - sizeof(byConnectType);
		tAddrList.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwIndexId, Buf, sizeof(dwIndexId) );
		Buf += sizeof(dwIndexId);
		memcpy( &byConnectType, Buf, sizeof(byConnectType) );
		Buf += sizeof(byConnectType);

		u16 wLen = BufLen - sizeof(dwIndexId) - sizeof(byConnectType);
		tAddrList.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(dwIndexId) + sizeof(byConnectType) + tAddrList.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIRecStartExportRsp
{
    TMTIMediaChn tAddrList;                                    //��ַ�б�

	void SwapToNetwork()
	{
	}

	void SwapFromNetwork()
	{
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		tAddrList.PackedToBuf(Buf, BufLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		tAddrList.UnpackedFromBuf(Buf, BufLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = tAddrList.PackedLen();
		return wPacketLen;
	}
}PACKED;


//ϵͳʱ������ṹ
typedef struct TMTISysTime
{
	u16		wYear;		//��
	u8		byMonth;	//�� 1-12
	u8		byDay;		//�� 1-31
	u8		byHour;		//ʱ 0-23
	u8		byMinute;	//�� 0-59
	u8		bySecond;	//�� 0-59

	void SwapToNetwork()
	{
		wYear = htons(wYear);
	}

	void SwapFromNetwork()
	{
		wYear = ntohs(wYear);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wYear, sizeof(wYear) );
		Buf += sizeof(wYear);
		memcpy( Buf, &byMonth, sizeof(byMonth) );
		Buf += sizeof(byMonth);
		memcpy( Buf, &byDay, sizeof(byDay) );
		Buf += sizeof(byDay);
		memcpy( Buf, &byHour, sizeof(byHour) );
		Buf += sizeof(byHour);
		memcpy( Buf, &byMinute, sizeof(byMinute) );
		Buf += sizeof(byMinute);
		memcpy( Buf, &bySecond, sizeof(bySecond) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wYear, Buf, sizeof(wYear) );
		Buf += sizeof(wYear);
		memcpy( &byMonth, Buf, sizeof(byMonth) );
		Buf += sizeof(byMonth);
		memcpy( &byDay, Buf, sizeof(byDay) );
		Buf += sizeof(byDay);
		memcpy( &byHour, Buf, sizeof(byHour) );
		Buf += sizeof(byHour);
		memcpy( &byMinute, Buf, sizeof(byMinute) );
		Buf += sizeof(byMinute);
        memcpy( &bySecond, Buf, sizeof(bySecond) );
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(u16)+5*sizeof(u8));
	}

}PACKED TMTI_SYS_TIME, *PTMTI_SYS_TIME;

//��Ƶͼ��ֱ���
typedef struct TVideoResolution
{
	u8 byAuto;
	u8 byQCIF;
	u8 byCIF;
	u8 by2CIF;
	u8 by4CIF;
#ifdef VSIP_ADD_NEW_RESOLUTION
	u8 byVGA;
	u8 byQQCIF;
	u8 byQVGA; 
	u8 by720P;
	u8 by1080P;
	u8 byQXGA;
	u8 byUXGA;
	u8 byQuadVGA;
#endif
}PACKED TVIDEO_RESOLUTION, *PTVIDEO_RESOLUTION;

//addbeg NAT��͸�ṹ�� 20100317
typedef struct TVsipStartNatDetectParam
{
    u32 dwSourceIP;
	u32 dwDestIP;
	u16 wDestPort;
    u16 dwSourcePort;
	u8  byDeviceType;
} PACKED TVSIP_STARTNATDETECTPARAM, *PTVSIP_STARTNATDETECTPARAM;
			
typedef struct TVsipStopNatDetectParam
{
    u32 dwIPAddr;
} PACKED TVSIP_STOPNATDETECTPARAM, *PTVSIP_STOPNATDETECTPARAM;
//addend 20100317

//��Ƶѹ����ʽ
typedef struct TVideoType
{
	u8 bySN4;
	u8 byMPEG4;
	u8 byH261;
	u8 byH263;
	u8 byH264;
	u8 byH265;
	u8 bySvac;
}PACKED TVIDEO_TYPE, *PTVIDEO_TYPE;

//��Ƶͼ��ֱ���
typedef struct TSPVideoResolution
{
	TVideoResolution tSN4;
	TVideoResolution tMEPG4;
	TVideoResolution tH261;
	TVideoResolution tH263;
	TVideoResolution tH264;
	TVideoResolution tH265;
	TVideoResolution tSvac;
}PACKED TSP_VIDEO_RESOLUTION, *PTSP_VIDEO_RESOLUTION;

//��Ƶѹ����ʽ
typedef struct TAudioType
{
	u8 byPCMA;
	u8 byPCMU;
	u8 byGSM;
	u8 byG729;
	u8 byADPCM;
	u8 byG7221C;
	u8 byG722;
	u8 byAACLC;
}PACKED TAUDIO_TYPE, *PTAUDIO_TYPE;

//��������
typedef struct TSerialType
{
	u8 byRS232;
	u8 byRS422;
	u8 byRS485;
}PACKED TSERIAL_TYPE, *PTSERIAL_TYPE;

//��������
typedef struct TConnType
{
	u8 byUdpUnicast;
	u8 byUdpMulticast;
	u8 byUdpBroadcast;
	u8 byTcpServer;
	u8 byTcpClient;
	u8 bySSL;
}PACKED TCONNECTION_TYPE, *PTCONNECTION_TYPE;

//�����������
typedef struct TMTIDiscoveryCBInfo
{
	TVsipGuid	tGuid;							//�豸GUIDֵ(ֻ��)
	char	achName[MAX_STRING_LEN+1];			//�豸��
	char	achTypeName[MAX_STRING_LEN+1];		//�豸������(ֻ��)
	u8		byDevType;							//�豸����(MTI_ENCODER��)
	u32		dwUnitIpAddr;						//�豸IP��ַ
	u16		wVsipPort;							//VSIP�˿ں�
}PACKED TMTI_DISCOVERY_CB_INFO, *PTMTI_DISCOVERY_CB_INFO;

//ϵͳ�����ṹ
typedef struct TMTISysParam
{
	char	achName[MAX_STRING_LEN+1];			//�豸��
	char	achTypeName[MAX_STRING_LEN+1];		//�豸������(ֻ��)
	u8		byDevType;							//�豸����(MTI_ENCODER��)
}PACKED TMTI_SYS_PARAM, *PTMTI_SYS_PARAM;

//PPPOE�����ṹ
typedef struct TMTIPPPOEParam
{
	u8		byPPPOEState;						//PPPOE״̬ MTI_DISABLE-��ʹ��, MTI_ENABLE-ʹ��
	char	achUsername[MAX_USER_NUM];			//�����û���
	char	achPassword[MAX_USER_NUM];			//��������
	u8		byAutoDialState;					//�Զ�����״̬ MTI_DISABLE-��ʹ��, MTI_ENABLE-ʹ��
	u32		dwAutoDialWait;						//�Զ����ŵȴ�ʱ��(��)
	u32		dwAutoDialTry;						//�Զ��������Դ���
	u8		byDebugState;						//Debug״̬ MTI_DISABLE-��ʹ��, MTI_ENABLE-ʹ��
    u16     wLcpEchoSendInterval;				//���ӽ����󣬷���LCP-ECHO����ʱ����
    u16     wLcpEchoRetryTimes;					//���ͼ���LCP-ECHO��û�еõ���Ӧ����Ϊ���ӶϿ�
}PACKED TMTI_PPPOE_PARAM, *PTMTI_PPPOE_PARAM;

//ϵͳ���������ṹ
typedef struct TMTISysPubParam
{
	u8      byDetPacketSend;                    //�Ƿ���NAT̽���
	u32		dwUnitIpAddr;                       //�豸IP��ַ
	u16		wVsipPort;                          //VSIP�˿ں�
	u32		dwSubnetMask;                       //�豸��������(������)
	u32		dwGateWay;                          //���ص�ַ(������)
	char	achVersion[MAX_VERSION_STRING_LEN+1];       //�豸�汾��(ֻ��)
	u32		dwDiscoveryIpAddr;                  //������ע���ַ
	u32		dwVsipMultiIpAddr;                  //�������鲥��ַ
	char	achWebUsername[MAX_STRING_LEN+1];   //Ƕ��ʽWeb�û���
	char	achWebPassword[MAX_STRING_LEN+1];   //Ƕ��ʽWeb����
	char	achWebGuestname[MAX_STRING_LEN+1];   //Ƕ��ʽWeb����Ա�û���	 zlq add
	char	achWebGuestPassword[MAX_STRING_LEN+1];   //Ƕ��ʽWeb����Ա����	 zlq add
	u16		wAlarmContinuance;                  //�澯�ָ�ʱ��(��)
	u8      bySubAreaType;                      //������ʽ(ֻ�� MTI_SINGLE_SUBAREA ������
	                                            //	       MTI_MULTI_SUBAREA�����)
	u8      byMaxTranferNum;                    //���ֱ��·��
	char    abyPuid[MAX_VERSION_STRING_LEN];    //puid
	char    abyPassword[MAX_VERSION_STRING_LEN]; //��֤����
	u8      byIsDecCombined;                     //�Ƿ��ǽ���ϳ�ͼ��
	u16		wAppType;							//Ӧ������
	u8		byMatrixValid;						//�Ƿ�������
}PACKED TMTI_SYS_PUB_PARAM, *PTMTI_SYS_PUB_PARAM;

typedef struct TMTIUserPassword
{
    char abyUserName[MAX_STRING_LEN];
	char abyPassWord[MAX_STRING_LEN];
}PACKED TMTI_USER_PASSWORD,*PTMTI_USER_PASSWORD;

//������������ṹ
typedef struct TMTICommonNetParam
{
	u8		byInitStreamState;		//������ʼ��״̬ MTI_DISABLE-������, MTI_ENABLE-����
	u8		byCurStreamState;		//������ǰ״̬ MTI_DISABLE-δ����, MTI_ENABLE-����
	TConnType	tSupportedConnType;	//֧�ֵ�������������
	u8		byConnectionType;		//������������
	u32		dwStreamIpAddr;			//���������ַ
	u16		wStreamPort;			//��������˿�
}PACKED TMTI_COMMONNET_PARAM, *PTMTI_COMMONNET_PARAM;

//��Ƶ��������ṹ
typedef struct TMTIVideoEncParam
{
	TMTIVideoEncParam()
	{
		memset(this,0,sizeof(TMTIVideoEncParam));
	}
	TVideoType tSupportedVideoType;	//֧�ֵ���Ƶ��ʽ(ֻ��)
	TSPVideoResolution tSupportedVideoResolution;	//֧�ֵ���Ƶ�ֱ���(ֻ��)
	u8		byVideoType;			//����ѹ����ʽ
	u8		byVideoResolution;		//ͼ��ֱ���
	//{{{{{{{{{{{{{{{ add by Royan 20140328 �ֱ�����ֵ��
#ifdef _ROYAN	
	TVidTypeResMap tVidTypeResMap;  // ֧�ֵ�ѹ����ʽ�ͷֱ��� //delete by yuwenying 2014.4.15
#endif
	TVidResFormatInfo abyVidResFormatInfo[MAX_VID_FORMAT_NUM];

	TVidResItem tVidResItem;        // ��ǰ�ֱ���
	
	//}}}}}}}}}}}}}}} add by Royan 20140328 �ֱ�����ֵ��

	u8      bySupportVidFormatNum; //��ǰͨ��֧�ֵı����ʽ������	
	u8		byFrameRate;			//֡�� 1-25
	u8		byMaxFrameRate;			//���֡��
	u8		byQuality;				//ͼ������ 1-2 (��������/�ٶ�����)
	u32		dwBitRate;				//��������� 64K-4M
	u8		byMaxQuantization;		//����������� 1-31
	u8		byMinQuantization;		//��С�������� 1-31
	u16		wInterval;				//�ؼ�֡��� 25-2500
	char	chBrightness;			//���� -128��127
	char	chContrast;				//�Աȶ� -128��127
	char	chSaturation;			//���Ͷ� -128��127
	u8		byDnyValue;				//��̬�غ�ֵ
    u8		bySharpness;			//��ȣ�0: not support; 1-4: available
	//{{{{{{{{{{{{{{{ add by Royan 20140528 �߼��������-����ȼ�
    u8		byMpValue;			    //ͼ�����Э��ȼ�
	//}}}}}}}}}}}}}}} add by Royan 20140528 �߼��������-����ȼ�
	u32     dwClockRate;            //��Ƶ������
}PACKED TMTI_VIDEOENC_PARAM, *PTMTI_VIDEOENC_PARAM;

//��Ƶ���빫�������ṹ
typedef struct TMTIVideoEncPubParam
{
	u8		byMaxVideoEncNum;	//��������Ƶ����ͨ������(ֻ��)
	u8		byVideoEncUsed;		//���õ���Ƶ����ͨ����
	u8		abyVideoEncCap[MAX_VIDEOIN_PORT_NAME_LEN];	//֧�ֵ���Ƶ��������(ֻ��)
	u8		byMaxVideoEncCapNum;	//֧�ֵı�����������(ֻ��)
}PACKED TMTI_VIDEOENC_PUB_PARAM, *PTMTI_VIDEOENC_PUB_PARAM;

//��Ƶ������Ļ�����ṹ
typedef struct TMTIVideoEncOSDParam
{
	u8		byShowTimeState;		//�Ƿ���ʾʱ�� MTI_DISABLE-����ʾ, MTI_ENABLE-��ʾ
	u8		byOSDState;				//������Ļ״̬ MTI_DISABLE-����Ļ, MTI_ENABLE-����Ļ
}PACKED TMTI_VIDEOENC_OSD_PARAM, *PTMTI_VIDEOENC_OSD_PARAM;

//��Ƶ����໭������ṹ
typedef struct TMTIVideoEncMPParam
{
	u8		abyEncodeMember[MAX_PIC_REGION_NUM]; //�໭���Ӧ����ƵԴ�˿ں�
	u8		byEncodeMemberNum; 		//�໭���Ӧ����ƵԴ�˿�����
	u8		byEncodeStyle;			//�໭�����������
}PACKED TMTI_VIDEOENC_MP_PARAM, *PTMTI_VIDEOENC_MP_PARAM;

//��Ƶ������ƵԴ�˿ڲ����ṹ
typedef struct TMTIVideoEncInputPortParam
{
	u8		abyValidVideoInputPort[MAX_VIDEO_INPUT_PORT_NUM]; //���õ���Ƶ����˿ں�(ֻ��)
	u8		byValidVideoInputPortNum; 	//���õ���Ƶ����˿�����(ֻ��)
	u8		byVideoInputPort;		//��ǰʹ�õ���Ƶ����˿ں�
}PACKED TMTI_VIDEOENC_INPUT_PORT_PARAM, *PTMTI_VIDEOENC_INPUT_PORT_PARAM;

//��Ƶ������ѯ�����ṹ
typedef struct TMTIVideoEncPollParam
{
	u8		byPollState;		//��ѯ״̬ MTI_DISABLE-����ѯ, MTI_ENABLE-��Ч
	u8		byPollPortNum; 		//��ѯ�˿�����
	u8		abyPollPort[MAX_POLL_PORT_NUM];	//��ѯ�˿ڼ�
	u16		wPollInternal;	//��ѯʱ���� ��λ��
}PACKED TMTI_VIDEOENC_POLL_PARAM, *PTMTI_VIDEOENC_POLL_PARAM;

//��Ƶ��������ṹ
typedef struct TMTIVideoDecParam
{
	TVideoType tSupportedVideoType;	//֧�ֵ���Ƶ��ʽ(ֻ��)
	u8		byPlayerBufferNum;		//���Ż������� 6-50
	u8		byPlayerOriginBufNum;	//������ʼ�������� 0-10
	u8		byVideoType;			//��Ƶ�����ʽ
	u8		byDecDealLev;			//ͼ�������(0��3��,0��ʾû��)
}PACKED TMTI_VIDEODEC_PARAM, *PTMTI_VIDEODEC_PARAM;

//��Ƶ���빫�������ṹ
typedef struct TMTIVideoDecPubParam
{
	u8		byVGAOutSupported;		//�Ƿ�֧��VGA���(ֻ��) MTI_DISABLE-��֧��, MTI_ENABLE-֧��
	u8		byVideoOutputType;		//��Ƶ����˿����� (MTI_VIDEO_OUT_TYPE_C ��)
	u8		byVGAFrequency;			//VGA���ˢ��Ƶ�� (MTI_VGA_OUT_FREQUENCY_60 ��)
	u8		byStarveMode;			//����Ƶ����ʾ��ʽ (VSIP_PLAYER_STAEVE_MODE_COLOR_BAR ��)
}PACKED TMTI_VIDEODEC_PUB_PARAM, *PTMTI_VIDEODEC_PUB_PARAM;

//��Ƶ�����ش������ṹ
typedef struct TMTIVideoDecRetransferParam
{
	u16		wRetransfer1stTimeSpan;//�����1���ش�����
	u16		wRetransfer2ndTimeSpan;//�����2���ش�����
	u16		wRetransfer3rdTimeSpan;//�����3���ش�����
	u16		wRejectTimeSpan;		//�������ʱ��
	u8		byRetransferState;		//�����ش�״̬ MTI_DISABLE-���ش�, MTI_ENABLE-�ش�
}PACKED TMTI_VIDEODEC_RETRANSFER_PARAM, *PTMTI_VIDEODEC_RETRANSFER_PARAM;

//��Ƶ��������ṹ
typedef struct TMTIAudioEncParam
{
	TAudioType	tSupportedAudioType;//֧�ֵ�����ѹ����ʽ(ֻ��)
	u8		byAudioType;			//��Ƶ���ݸ�ʽ
	u8      byAudioTypeEx;          //��չ����Ƶ��ʽ
	u8		byAudioInputType;		//��Ƶ��������(VSIP_AUDIO_INPUTTYPE_LINEIN ��)
	u8		abyAudioInputType[MAX_VIDEOIN_PORT_NAME_LEN];	//֧�ֵ���������
	u8		byAudioMaxInputType;	//������������
	u8		byVolume;				//���� 0-25
	u8		byAECState;				//��������״̬ MTI_DISABLE-��Ч, MTI_ENABLE-��Ч
	u8		byDumbState;			//����״̬ MTI_DISABLE-��Ч, MTI_ENABLE-��Ч

	u32		dwSmpRate;				//������
	u8		bySmpBits;				//����λ��
	u8		byChannel;				//����
	u8		bySample;				//ÿ֡������
	u32     dwClockRate;            //ʱ��Ƶ��
	u32     dwBitRate;              //��Ƶ����
}PACKED TMTI_AUDIOENC_PARAM, *PTMTI_AUDIOENC_PARAM;


//��Ƶ��������ṹ
typedef struct TMTIAudioDecParam
{
	TAudioType	tSupportedAudioType;//֧�ֵ�����ѹ����ʽ(ֻ��)
	u8		byVolume;				//���� 0-25
	u8		byMuteState;			//����״̬ MTI_DISABLE-��Ч, MTI_ENABLE-��Ч
}PACKED TMTI_AUDIODEC_PARAM, *PTMTI_AUDIODEC_PARAM;

//�������Ÿ澯���ò���
typedef struct TMTIAlarmParam
{
    u8 byDetectEnable;  //���״̬ 1-Enable 0-disable
	u8 byRelatedSerialId;//�澯�ɼ��Ĵ���ͨ��������ͨ����������
    u8 byRelatedPinId;  //�澯�����Ĳ��ڸ澯ͨ��������ͨ����������
    u8 byDisplayBannereEnable; //��ʾ��� 1-Enable 0-disable
    u8 byMaxTemperature; //�¶����ޣ���Χ0��100�ȣ�
	u8 byMinTemperature; //�¶����ޣ���Χ0��100�ȣ�
	u8 byMaxHumidity;    //ʪ�����ޣ���Χ0��100����
	u8 byMinHumidity;    //ʪ�����ޣ���Χ0��100����
    u16 wReportInterval; //��ʪ���ϱ�ʱ������Ĭ��30s��15��300s��
	char achAlarmAlias[MTI_MAX_ALARM_NUM*MTI_MAX_SENSOR_NUM][MTI_MAX_ALARM_ALIAS_LENGTH]; //�澯���ر�������(���8������������32���ֽڣ�����Ϊ�գ�Ĭ��"�澯����x" )	
	u8 byMaxUnitNum;     //�ɼ���Ԫ����,�������MTI_MAX_SENSOR_NUM
	char achUnitName[MTI_MAX_SENSOR_NUM][MTI_MAX_ALARM_ALIAS_LENGTH];  //�ɼ���Ԫ������
	u8 byPinNum[MTI_MAX_ALARM_NUM*MTI_MAX_SENSOR_NUM];	//�ɼ���Ԫ��Ӧ�Ĳ��ں�
}PACKED TMTI_ALARM_PARAM,*PTMTI_ALARM_PARAM;

typedef struct TMTIBackBGDColor
{
	u8 RColor;    //R����
	u8 GColor;    //G����
	u8 BColor;    //B����
}PACKED TMTI_BackBGD_COLOR,*PTMTI_BackBGD_COLOR;

typedef struct TMTIAlarmInfo
{
	u16 wPinID;  //modify G.J. 20091020 u8->u16
	u8 byState;
	u16 wAlarmInfoLength;
	char *pAlarmInfoBuf;
}PACKED TMTI_ALARM_INFO,*PTMTI_ALARM_INFO;

typedef struct TMTITransparentInfo
{
    u8 byMessageType;
	u8 byNeedWriteToDB;
	u16 wTransparentInfoLength;
    char transparentInfoBuf[MTI_PU_TRANS_DATA_LEN];
}PACKED TMTI_TRANSPARENT_INFO,*PTMTI_TRANSPARENT_INFO;

typedef struct TMTIBannerParam
{
	u32 dwXPos;//��ʾ�����X����
    u32 dwYPos;//��ʾ�����Y����
    u32 dwWidth;//��ʾ����Ŀ�
    u32 dwHeight;//��ʾ����ĸ�
    u32 dwBMPWidth;//BMPͼ���
    u32 dwBMPHeight;//BMPͼ���
    u32 dwBannerFontHeight;//����߶ȣ�����Ҫ�������¹���ʱ��ȷ��ͣ�ٵ�λ��
	u32 dwBannerFontWidth;//�����ȣ�����Ҫ�������¹���ʱ��ȷ��ͣ�ٵ�λ��
    TMTIBackBGDColor tBackBGDColor;//������ɫ
    u32 dwBGDClarity;//������ɫ͸����
    u32 dwPicClarity;//����ͼƬ͸����
    u32 dwRunMode;//����ģʽ������or����or��ֹ���궨�����ϣ�
    u32 dwRunSpeed;//�����ٶ� �ĸ��ȼ����궨�����ϣ�
    u32 dwRunTimes;//�������� 0Ϊ�����ƹ���
	u32 dwRunForever;//�����ƹ���
	u32 dwHaltTime;//ͣ��ʱ��(��)�����¹���ʱ��ÿ����һ���ֵ�ͣ�ټ����0ʼ��ͣ�����뾲ֹ��ͬ��
	u8  byAlignStyle;//���뷽ʽ
	u8 byRelatedPinId;//�����Ĳ���id
	u8  byCharBkColor;//���屳����ɫ
}PACKED TMTI_BANNER_PARAM,*PTMTI_BANNER_PARAM;

//���ڲ����ṹ
typedef struct TMTISerialPortParam
{
	u32		dwBaudRate;				//���ڲ�����
	u8		byComPortType;			//�������� 0:��ʹ��, 1:RS232, 2:RS422, 3:RS485
	TSerialType tSupportedType;		//֧�ֵĴ������� (ֻ��)
}PACKED TMTI_SERIALPORT_PARAM, *PTMTI_SERIALPORT_PARAM;

//���ڹ��������ṹ
typedef struct TMTISerialPortPubParam
{
	u8		byInitStreamState;		//���������շ���ʼ��״̬ MTI_DISABLE-��Ч, MTI_ENABLE-��Ч
	u16		wStreamLocalPort;		//�������ݱ��ؽ��ն˿�
	u32		dwStreamDestIpAddr;		//�������ݶԶ˵�ַ
	u16		wStreamDestPort;		//�������ݶԶ˶˿�
	TConnType	tSupportedConnType;	//֧�ֵ�������������
	u8		byConnectionType;		//������������
	u8		byProtocolType;			//Э������ 0-͸��ͨ��, 1-VSIPЭ��
	u8      bySerialId;             //�Զ˵Ĵ���id
	u8		byDecCtrlEncEnable;		//���������Ʊ�����ʹ��
	u8		byDecCtrlChanId;		//������ͨ����
}PACKED TMTI_SERIALPORT_PUB_PARAM, *PTMTI_SERIALPORT_PUB_PARAM;

//������������ṹ
typedef struct TMTIInputPinParam
{
	u8		byAlarmDetectState;		//�澯���״̬ 0-�����, 1-ֻ���һ������, 2-ÿ��ѭ�����, 3-ʼ�ռ��
	u8		byAlarmInitState;		//����澯�ͳ�̬��ѡ zlq add
	TMTISysTime atAlarmDetectStartTime[DAYS_OF_WEEK]; //�澯��⿪ʼʱ��
	TMTISysTime atAlarmDetectDuration[DAYS_OF_WEEK];  //�澯������ʱ��
	u8		byRelateEncId;			//�澯�����ı���ͨ�� (�޹�������ͨ����Ϊ MTI_RELATE_ENCID_DISABLE)
	u8		byOutputPinID;			//�澯�Զ��������ID (�޿��������Ϊ MTI_OUTPUT_DISABLE)
	u8		byAlarmOSDState;		//�澯��Ļ״̬ MTI_DISABLE-����ʾ, MTI_ENABLE-��ʾ
	u16		wAlarmOSDLeftMargin;	//�澯��Ļ��߾� 1-200
	u16		wAlarmOSDTopMargin;		//�澯��Ļ�ϱ߾� 1-200
	char	achAlarmOSDName[MAX_STRING_LEN+1];//�澯��Ļ����
	u32		dwAlarmOSDColor;		//�澯��Ļ��ɫ(RGB)
	u8		byEthAlarmState;			//�Ƿ���ETH1���ڸ澯
}PACKED TMTI_INPUTPIN_PARAM, *PTMTI_INPUTPIN_PARAM;

//��������״̬�ṹ 20091020
typedef struct TMTIInputPinState
{
	u8 adwInputPinState[128]; //������NVR64·�豸ÿ��ͨ����Ӧһλ

}PACKED TMTI_INPUTPIN_STATE, *PTMTI_INPUTPIN_STATE;

//��ƵԴ����˿ڲ����ṹ
typedef struct TMTIVideoInputParam
{
	u8		byCameraType;			//����ͷ����
	u8		byCameraID;				//����ͷ��ַ��
	u8		bySerialPortID;			//��Ӧ�Ĵ���ID
	char	achVideoPortName[MAX_VIDEOIN_PORT_NAME_LEN+1];	//��ƵԴ�˿���
	char	achVideoSrcName[MAX_VIDEOIN_PORT_LONGNAME_LEN+1];	//��ƵԴ��
	char	achVideoPortOSD[MAX_STRING_LEN+1];	//��ƵԴ��Ļ����
	u16		wVideoPortOSDLeftMargin;	//��ƵԴ��Ļ��߾� 1-200
	u16		wVideoPortOSDTopMargin;		//��ƵԴ��Ļ�ϱ߾� 1-200
	u32		dwVideoPortOSDColor;		//��ƵԴ��Ļ��ɫ
}PACKED TMTI_VIDEO_INPUT_PARAM, *PTMTI_VIDEO_INPUT_PARAM;

//����������add 20100317
typedef struct TMTIThirdPartyDecParam
{
	char achThirdPartyDecParam[MAX_THIRDPARTY_NAME_LEN+1];

}PACKED TMTI_THIRDPARTY_DEC_PARAM, *PTMTI_THIRDPARTY_DEC_PARAM;

//�ƶ��������ṹ��
typedef struct TMTIMotionDetectParam
{
	u8   byDetectEnable;			//���ʹ�� MTI_ENABLE-��Ч MTI_DISABLE-��Ч
	u32  dwXPos;					//������򶥵������
	u32  dwYPos;					//������򶥵�������
	u32  dwWidth;					//���������
	u32  dwHeigth;					//�������߶�
	u8   byAlarmRate;				//�������澯�����ٷֱȣ����ڴ��ٽ�ֵ�澯,0-100��
	u8   byResumeRate;				//�����澯���˶���Χ�ٷֱ�С�ڸ�ֵ��ָ��澯(��ֵС��byAlarmRate,0-100)
}PACKED TMTI_MOTION_DETECT_PARAM, *PTMTI_MOTION_DETECT_PARAM;

//add by Royan 20130201
typedef struct TMTIMotionDetectParamList
{
	u8                     byMotionDetectCount;	              		  //�ƶ������
	TMTIMotionDetectParam  atMtiMotionDetectParam[MAX_DET_AREA_NUM];  //�ƶ��������б�
}PACKED TMTI_MOTION_DETECT_PARAM_LIST, *PTMTI_MOTION_DETECT_PARAM_LIST;


//ͼ���ڱο��ƽṹ��
typedef struct TMTIShadeParam
{
	u8   byShadeEnable;			//�ڱ�ʹ�� MTI_ENABLE-��Ч MTI_DISABLE-��Ч
	u32  dwXPos;				//�ڱ����򶥵������
	u32  dwYPos;				//�ڱ����򶥵�������
	u32  dwWidth;				//�ڱ�������
	u32  dwHeigth;				//�ڱ�����߶�
}PACKED TMTI_SHADE_PARAM, *PTMTI_SHADE_PARAM;

//add by Royan 20130201
typedef struct TMTIShadeParamList
{
	u8              byShadeCount;		                	//ͼ���ڱ���
	TMTIShadeParam  atMtiShadeParam[MAX_SHADE_AREA_NUM];    //ͼ���ڱβ����б�
}PACKED TMTI_SHADE_PARAM_LIST, *PTMTI_SHADE_PARAM_LIST;

typedef struct TMTIMediaCapability
{
	u8 byVidEncNum; 				//��Ƶ����ͨ������
	u8 byVidDecNum; 				//��Ƶ����ͨ������
	u8 byAudEncNum; 				//��Ƶ����ͨ������
	u8 byAudDecNum; 				//��Ƶ����ͨ������
	u8 byVidAdpterNum; 				//��Ƶתѹͨ������
	u8 byVidCompNum; 				//ͼ��ϳ�ͨ����
	u8 byVidSrcPollNum;				//��ƵԴ��ѯͨ������
	u8 byAudMixNum;					//��Ƶ����ͨ������
	u8 byOsdColorNum;				//̨����Ļ֧�ֵ���ɫ��
	u8 byShadeAreaNum;				//ͼ���ڱ���������
	u8 byMaxStreamSendNum;			//����ͨ�����������(һ��ͨ��������Ƶ����Ƶ)
	u8 byMaxStreanDirSendNum;		//����ͨ�������ֱ��·��(һ��ͨ��������Ƶ����Ƶ)
	u8 byStreamEncrypt;				//�Ƿ���������
	u8 byMotionDetectAreaNum;		//�ƶ������������
	u8 bySnapShotChanNum;			//ץ��ͨ����
    u8 byCBRVBR;				    // �Ƿ��ܹ�����ͼ������,1��֧�֣�0����֧��
    u8 byMaxEncBitRate;				// �����Ч����             
#ifdef VSIP_MULTI_CHAN_SUPPORT
    u8 bySupportMultiChan;		    //�Ƿ�֧��˫����1��֧�֣�0����֧��
#endif
    u8 byMatrix;                            // �Ƿ�֧�־������
    u8 byVideoEncSlaveChannelNum;   //��Ƶ���븨��ͨ���� 20090509
}PACKED TMTI_MEDIACAPABILITY, *PTMTI_MEDIACAPABILITY;

typedef struct TMTIRecCapability
{
	u8 byRecorderNum;				//¼��ͨ������
	u8 byPlayerNum;					//����ͨ������
	u8 byFileDownloadNum;			//¼���ļ�����ͨ������
}PACKED TMTI_RECCAPABILITY, *PTMTI_RECCAPABILITY;

typedef struct TMTIDevCapability
{
	u8 byVGANum;					//��Ƶ���VGA�ӿ�����
	u8 byVidOutNum;					//��Ƶ����ӿ�����
	u8 byVideoInputPortNum;			//��ƵԴ��������
	u8 bySerialPortNum;				//��������
	u16	wInputPinNum;			    //��������ͨ������  modify G.J. 20091020 u8->u16
	u8 byOutputPinNum;				//�������ͨ������
	u8 byPTZCtrlNum;				//�ɿ�����̨����
	u8 byEtherNetnum;				//��̫��������
	u8 byWirelessNum;				//������������
	u8 byXiAnTeleNum;				//�������Ÿ澯����
	u8 byPlatFormType;				//����ƽ̨����
	u8 byDevPowerType;				//��������
}PACKED TMTI_DEVCAPABILITY, *PTMTI_DEVCAPABILITY;

//�豸������
typedef struct TMTIPUCapability
{
	TMTIMediaCapability		tMediaCap;								//��ý��������
	TMTIDevCapability		tDevCap;								//�豸Ӳ��������
	TMTIRecCapability		tRecCap;								//¼����������
}PACKED TMTI_PUCAPABILITY, *PTMTI_PUCAPABILITY;

// ƽ̨�����������
typedef struct TMTICapability
{
	u8		byVideoEncNum;			//��Ƶ����ͨ������
	u8		byVideoDecNum;			//��Ƶ����ͨ������
	u8		byAudioEncNum;			//��Ƶ����ͨ������
	u8		byAudioDecNum;			//��Ƶ����ͨ������
	u8		bySerialPortNum;		//��������
	u16		wInputPinNum;			//������������  //modify G.J. 20091020 u8->u16
	u8		byOutputPinNum;			//�����������
	u8		byVideoInputPortNum;	//��Ƶ����˿�����
}PACKED TMTI_CAPABILITY,*pTMTI_CAPABILITY;

// ��������
typedef struct TMTICapSetItem
{
	u8 byCapItemId;		// ����������ID
	u16 byCapItemValue;	    // ����������ֵ

	u8 GetCapItemId()
	{
		return byCapItemId;
	}

	void SetCapItemId(u8 byID)
	{
		byCapItemId = byID;
	}
	u16 GetCapItemValue()
	{
		return ntohs(byCapItemValue);
	}

	void SetCapItemValue(u16 wValue)
	{
		byCapItemValue = htons(wValue);
	}
}PACKED TMTICAPSETITEM;

//��չ������
typedef struct TMTICapSet
{
	u8				byCapItemNum;
	TMTICapSetItem	atCustomCap[MAX_CUSTOMCAP_NUM];
	//ͨ��������ȡ����������
	u8 GetCapType(u8 byIndex)
	{
		if (byIndex>byCapItemNum)
		{
			return VSIP_INVALID_CAP_TYPE;
		}
		return atCustomCap[byIndex].byCapItemId;
	}
	//ͨ�����������ͻ�ȡ������ȡֵ
	u16 GetCapValue(u8 byCapType)
	{
		u8 i = 0;
		while (i < byCapItemNum && atCustomCap[i].byCapItemId != byCapType)
		{
			i++;
		}
		if (i >= byCapItemNum)
		{
			return VSIP_INVALID_CAP_VALUE;
		}
		return atCustomCap[i].byCapItemValue;
	}

}PACKED TMTICAPSET;

//�ն˲�������ظ�
typedef struct TMTIOPResultResp
{
	u16		wTransactionNum;	//������ˮ��
	u16		wErrCode;			//�������(MTI_SUCCESS, MTI_FAIL)

	void SwapToNetwork()
	{
        wTransactionNum = htons(wTransactionNum);
		wErrCode = htons(wErrCode);
	}

	void SwapFromNetwork()
	{
		wTransactionNum = ntohs(wTransactionNum);
		wErrCode = ntohs(wErrCode);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wTransactionNum, sizeof(wTransactionNum));
		Buf += sizeof(wTransactionNum);
		memcpy( Buf, &wErrCode, sizeof(wErrCode));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy(  &wTransactionNum, Buf, sizeof(wTransactionNum));
		Buf += sizeof(wTransactionNum);
		memcpy( &wErrCode, Buf, sizeof(wErrCode));
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(wTransactionNum)+sizeof(wErrCode));
	}

}PACKED TMTI_OP_RESULT_RESP, *PTMTI_OP_RESULT_RESP;

typedef struct TMTIMediaSwitchAddr
{
	 u32 dwDestIpAddr;
	 u16 wDestRtpPort;
	 u16 wLocalPort;
	 u16 wDestRtcpPort;
	 u8  byConnectType;
	 u32 dwSSRC;
	 u16 wSrcRtpPort;
	 u16 wSrcRtcpPort;

	 void SwapToNetwork()
	 {
		 wDestRtpPort = htons(wDestRtpPort);
		 wDestRtcpPort = htons(wDestRtcpPort);
		 wLocalPort = htons(wLocalPort);
		 dwSSRC = htonl(dwSSRC);
		 wSrcRtpPort = htons(wSrcRtpPort);
		 wSrcRtcpPort = htons(wSrcRtcpPort);
	 }

	 void SwapFromNetwork()
	 {
		 wDestRtpPort = ntohs(wDestRtpPort);
		 wDestRtcpPort = ntohs(wDestRtcpPort);
		 wLocalPort = ntohs(wLocalPort);
		 dwSSRC = ntohl(dwSSRC);
		 wSrcRtpPort = ntohs(wSrcRtpPort);
		 wSrcRtcpPort = ntohs(wSrcRtcpPort);
	 }

	 void PackedToBuf(char* Buf)
	 {
		 memcpy(Buf,&dwDestIpAddr,sizeof(dwDestIpAddr));
		 Buf += sizeof(dwDestIpAddr);
		 memcpy(Buf,&wDestRtpPort,sizeof(wDestRtpPort));
		 Buf += sizeof(wDestRtpPort);
		 memcpy(Buf,&wDestRtpPort,sizeof(wDestRtpPort));   //���ض˿ڣ��ֲ��ã�����ΪĿ��Э��˿�
		 Buf += sizeof(wDestRtpPort);
		 memcpy(Buf,&byConnectType,sizeof(byConnectType));
		 Buf += sizeof(byConnectType);
		 memcpy(Buf,&dwSSRC,sizeof(dwSSRC));
		 Buf += sizeof(dwSSRC);
		 memcpy(Buf,&wSrcRtpPort,sizeof(wSrcRtpPort));
		 Buf += sizeof(wSrcRtpPort);
		 memcpy(Buf,&wSrcRtcpPort,sizeof(wSrcRtcpPort));
		 Buf += sizeof(wSrcRtcpPort);
		 memcpy(Buf,&wDestRtcpPort,sizeof(wDestRtcpPort));
	 }
}PACKED TMTI_MEDIA_SWITCH_ADDR,*PTMTI_MEDIA_SWITCH_ADDR;

typedef struct TFileTransInfo
{
	u16 wSegNumber;
	u8 byMessageType;
	u16 wLen;
	char abyBuf[SMALL_DATA_BUFSIZE];
}PACKED TMTI_FILE_TRANS_INFO,*PTMTI_FILE_TRANS_INFO;

typedef struct TFileTransRspInfo
{
	u16 wResultCode;
	u8 byOsdType;
}PACKED TFILE_TRANS_RSP_INFO,*PTFILE_TRANS_RSP_INFO;

//¼���ļ������ظ�
typedef struct TQueryRecFileNum
{
	u16		wTransactionNum;	//������ˮ��
	u16		wNum;			   //�ļ�����
	unsigned short PackedLen()
	{
		return (sizeof(wTransactionNum)+sizeof(wNum));
	}
}PACKED TQueryRecFileNum, *PTQueryRecFileNum;
//�����ն˲���
typedef struct TMTISetConfig
{
	u16		wEntityID;		//ID modify G.J. 20091020 u8->u16 
	u8		byConfigType;	//�ṹ������
	void *	pConfigData;	//�ṹ��ָ��
}PACKED TMTI_SET_CONFIG, *PTMTI_SET_CONFIG;

//�����ն��ƶ�������
typedef struct TMTISetMotDetCfg
{
	u16     wAreaNum;		//������Ŀ
	void *	pConfigData;	//�ƶ��������ṹ������
}PACKED TMTI_SET_MOT_DET_CONFIG, *PTMTI_SET_MOT_DET_CONFIG;

//�����ն�ͼ�����β���
typedef struct TMTISetShadeCfg
{
	u16     wShadeAreaNum;		//������Ŀ
	void *	pConfigData;	//�ƶ��������ṹ������
}PACKED TMTI_SET_SHADE_CONFIG, *PTMTI_SET_SHADE_CONFIG;


//�ն���������
typedef struct TMTIMediaCtrl
{
	u16		wEntityID;		//ͨ��ID   modify G.J. 20091020 u8->u16
	u8		byStreamType;	//������������
	u16		wStreamPort;	//��������˿�
	u32		dwStreamIpAddr;	//���������ַ
}PACKED TMTI_MEDIA_CTRL, *PTMTI_MEDIA_CTRL;


//��չ�ն���������(������)
typedef struct TMTIMediaCtrlEX
{
	u16		wEntityID;		              //ͨ��ID modify G.J. 20091020 u8->u16 
	u8      byMediaCtrlType;              //video--MTI_VIDEO_TYPE,audio-- MTI_AUDIO_TYPE
	u8		byStreamType;	              //������������MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP
	u16		wLocalAcceptStreamPort;       //�����������ն˿�
	u32		dwLocalAcceptStreamIpAddr;	  //�����������յ�ַ
   	u16		wRTCPStreamPort;              //�Զ˵�RTCP�˿�
	u32		dwRTCPStreamIpAddr;	          //�Զ˵�RTCP�˿ڵ�ַ
}PACKED TMTI_MEDIA_CTRLEX, *PTMTI_MEDIA_CTRLEX;

//��������
typedef struct TMTISerialPortData
{
	u16		wEntityID;		    //���ں� modify G.J. 20091020 u8->u16 
	u16		wDataBufLen;		//���ݳ���
	u8 *	pbyDataBuf;			//����ָ��
}PACKED TMTI_SERIAL_PORT_DATA, *PTMTI_SERIAL_PORT_DATA;

//����ͷ��������
typedef struct TMTICameraCtrlData
{
	u16		wEntityID;		    //��Ƶ����˿ں� modify G.J. 20091020 u8->u16 
	u16		wDataBufLen;		//���ݳ���
	u8 *	pbyDataBuf;			//����ָ��
}PACKED TMTI_CAMERA_CTRL_DATA, *PTMTI_CAMERA_CTRL_DATA;

//��������֪ͨ��Ϣ
typedef struct TMTIConfigInfo
{
	u16		wEntityID;		//ID modify G.J. 20091020 u8->u16 
	u8		byConfigType;	//��������
}PACKED TMTI_CONFIG_INFO, *PTMTI_CONFIG_INFO;

//͸������Ӧ��
typedef struct TMTITransDataRsp
{
	u16     wMsgType;		    //��Ϣ����
	u8      bIsHaveNtf;         //�Ƿ���Ntf
	u32     dwReserved;         //�����ֶ�
	u16	    wDataBufLen;	    //���ݳ���
	u8      abyTransDataBuf[MTI_MAX_CU_OPERTION_NUM];   //͸�����ݽṹ

	void SwapToNetwork()
	{
		wMsgType = htons(wMsgType);
		wDataBufLen = htons(wDataBufLen);
		dwReserved = htonl(dwReserved);
	}

	void SwapFromNetwork()
	{
		wMsgType = ntohs(wMsgType);
		wDataBufLen = ntohs(wDataBufLen);
		dwReserved = ntohl(dwReserved);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		memcpy( Buf, &wMsgType, sizeof(wMsgType));
		Buf += sizeof(wMsgType);
		memcpy( Buf, &bIsHaveNtf, sizeof(bIsHaveNtf));
		Buf += sizeof(bIsHaveNtf);
		memcpy(Buf,&dwReserved,sizeof(dwReserved));
		Buf += sizeof(dwReserved);
		memcpy( Buf, &wDataBufLen, sizeof(wDataBufLen));
		Buf += sizeof(wDataBufLen);
		memcpy( Buf, abyTransDataBuf, sizeof(abyTransDataBuf));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		memcpy( &wMsgType, Buf, sizeof(wMsgType));
		Buf += sizeof(wMsgType);
		memcpy( &bIsHaveNtf, Buf, sizeof(bIsHaveNtf));
		Buf += sizeof(bIsHaveNtf);
		memcpy(&dwReserved,Buf,sizeof(dwReserved));
		Buf += sizeof(dwReserved);
		memcpy( &wDataBufLen, Buf, sizeof(wDataBufLen));
		Buf += sizeof(wDataBufLen);
		memcpy( abyTransDataBuf, Buf, sizeof(abyTransDataBuf));

		return TRUE;
	}

	unsigned short PackedLen() const
	{
		return (sizeof(bIsHaveNtf)+sizeof(wMsgType)+sizeof(dwReserved)+sizeof(wDataBufLen)+sizeof(abyTransDataBuf));
	}
}PACKED TMTI_TRANS_DATA_RSP, *PTMTI_TRANS_DATA_RSP;


typedef struct TMTIStreamProp
{
	u16 wStreamPropId;
	u8  byDataType;
	u16 wValueLength;
	u8  abyValue[MTI_MAX_STREAM_PROP_LENGTH];

	TMTIStreamProp()
	{
		memset(abyValue,0,sizeof(abyValue));
	}

	void SwapToNetwork()
	{
		wStreamPropId = htons(wStreamPropId);
		wValueLength = htons(wValueLength);
	}

	void SwapFromNetwork()
	{
		wStreamPropId = ntohs(wStreamPropId);
		wValueLength = ntohs(wValueLength);
	}

	u16 UnpackedFromBuf(char * Buf, unsigned short BufLen)
	{
		u16 wBufLen = 0;

		memcpy( &wStreamPropId ,Buf, sizeof(wStreamPropId));
		wBufLen += sizeof(wStreamPropId);

		memcpy(&byDataType,Buf+wBufLen,sizeof(byDataType));
		wBufLen += sizeof(byDataType);

		memcpy(&wValueLength,Buf+wBufLen,sizeof(wValueLength));
		wBufLen += sizeof(wValueLength);

		switch(byDataType)
		{
		case VSIP_VALUE_TYPE_CHAR:
			{
				memcpy(abyValue, Buf+wBufLen, sizeof(char));
				wBufLen += sizeof(char);
			}
			break;

		case VSIP_VALUE_TYPE_SHORT:
			{	
				memcpy( abyValue, Buf+wBufLen, sizeof(short));
				wBufLen += sizeof(short);
			}
			break;

		case VSIP_VALUE_TYPE_INT:
			{
				memcpy( abyValue, Buf+wBufLen, sizeof(int));
				wBufLen += sizeof(int);
			}
			break;

		case VSIP_VALUE_TYPE_UINT:
			{
				memcpy(abyValue, Buf+wBufLen, sizeof(u32));
				wBufLen += sizeof(u32);
			}
			break;
		case VSIP_VALUE_TYPE_STRING:
		default:
			{
				memcpy(abyValue,Buf+wBufLen,wValueLength);
				wBufLen += wValueLength;
			}
			break;
		}
		return wBufLen;
	}

	u16 PackedToBuf( char * Buf, unsigned short BufLen )
	{
		u16 wBufLen = 0;

		memcpy(Buf+wBufLen,&wStreamPropId,sizeof(wStreamPropId));
		wBufLen += sizeof(wStreamPropId);

		memcpy(Buf+wBufLen,&byDataType,sizeof(byDataType));
		wBufLen += sizeof(byDataType);

		memcpy( Buf+wBufLen, &wValueLength, sizeof(wValueLength));
		wBufLen += sizeof(wValueLength);

		switch(byDataType)
		{
		case VSIP_VALUE_TYPE_CHAR:
			{
				memcpy( Buf+wBufLen, abyValue, sizeof(char));
				wBufLen += sizeof(char);
			}
			break;

		case VSIP_VALUE_TYPE_SHORT:
			{	
				memcpy( Buf+wBufLen, abyValue, sizeof(short));
				wBufLen += sizeof(short);
			}
			break;

		case VSIP_VALUE_TYPE_INT:
			{
				memcpy( Buf+wBufLen, abyValue, sizeof(int));
				wBufLen += sizeof(int);
			}
			break;

		case VSIP_VALUE_TYPE_UINT:
			{
				memcpy( Buf+wBufLen, abyValue, sizeof(u32));
				wBufLen += sizeof(u32);
			}
			break;
		case VSIP_VALUE_TYPE_STRING:
		default:
			{
				memcpy( Buf+wBufLen, abyValue, sizeof(wValueLength));
				wBufLen += wValueLength;
			}
			break;
		}
		return wBufLen;
	}

}PACKED TMTI_STREAM_PROP,*PTMTI_STREAM_PROP;

typedef struct TMTIStreamPropList
{
	u16 wErrCode;			//�������(MTI_SUCCESS, MTI_FAIL)
	u16 wStreamPropNum;
	TMTIStreamProp tStreamProp[MAX_STREAMPROP_NUM];

	void SwapToNetwork()
	{
		for (int i=0;i<wStreamPropNum;i++)
		{
			tStreamProp[i].SwapToNetwork();
		}
		wStreamPropNum = htons(wStreamPropNum);
	}

	void SwapFromNetwork()
	{
		for (int i=0;i<wStreamPropNum;i++)
		{
			tStreamProp[i].SwapFromNetwork();
		}
		wStreamPropNum = ntohs(wStreamPropNum);
	}

	u16 UnpackedFromBuf(char * Buf, unsigned short BufLen)
	{
		u16 wBufLen;
		for (int i=0;i<wStreamPropNum;i++)
		{
			wBufLen += tStreamProp[i].UnpackedFromBuf(Buf+wBufLen,BufLen);
		}
		return wBufLen;
	}

	u16 PackedToBuf( char * Buf, unsigned short BufLen )
	{
		u16 wBufLen;
		for (int i=0;i<wStreamPropNum;i++)
		{
			wBufLen += tStreamProp[i].PackedToBuf(Buf+wBufLen,BufLen);
		}
		return wBufLen;
	}
}PACKED TMTI_STREAM_PROPLIST,*PTMTI_STREAM_PROPLIST;


typedef struct TMTIVideoAttribute
{
    u32     dwMediaType;      // �����ʽ VSIP_VIDEO_TYPE_, 
    u32     dwClockRate;      // ������ 1000.90000��
    u16     wWidth;           // ��
    u16     wHeigth;          // ��
    u8      byFramerate;      // ֡��
    u8      byStreamType;     // 1:�̶����ʣ�2:�ɱ�����
    u32     dwBitRate;        // ����
    u32     dwParam1;         //��չ����
    u32     dwParam2;

	void SwapToNetwork()
	{
		dwMediaType = htonl(dwMediaType);
		dwClockRate = htonl(dwClockRate);
		wWidth = htons(wWidth);
		wHeigth = htons(wHeigth);
		dwBitRate = htonl(dwBitRate);
		dwParam1 = htonl(dwParam1);
		dwParam2 = htonl(dwParam2);
	}
	void SwapFromNetwork()
	{
		dwMediaType = ntohl(dwMediaType);
		dwClockRate = ntohl(dwClockRate);
		wWidth = ntohs(wWidth);
		wHeigth = ntohs(wHeigth);
		dwBitRate = ntohl(dwBitRate);
		dwParam1 = ntohl(dwParam1);
		dwParam2 = ntohl(dwParam2);
	}
    TMTIVideoAttribute()
    {
        memset(this, 0 , sizeof(TMTIVideoAttribute));
    }

    void PrintData()
    {
        OspPrintf(TRUE, FALSE, "\t\t��Ƶ�������------\n");
        OspPrintf(TRUE, FALSE, "\t\t�����ʽ:%d\n", dwMediaType);
        OspPrintf(TRUE, FALSE, "\t\t������:%d\n", dwClockRate);
        OspPrintf(TRUE, FALSE, "\t\t��:%d\n", wWidth);
        OspPrintf(TRUE, FALSE, "\t\t��:%d\n", wHeigth);
        OspPrintf(TRUE, FALSE, "\t\t֡��:%d\n", byFramerate);
        OspPrintf(TRUE, FALSE, "\t\t1:�̶����ʣ�2:�ɱ�����:%d\n", byStreamType);
        OspPrintf(TRUE, FALSE, "\t\t����:%d\n", dwBitRate);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam1:%d\n", dwParam1);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam2:%d\n", dwParam2);
    }
}PACKED TMTI_VIDEO_ATTRI,*PTMTI_VIDEO_ATTRI;

typedef struct TMTIAudioAttribute
{  
    u32     dwAudioFormat;        // �����ʽ VISP_AUDIO_FORMAT_�� 
    u32     dwSamplingRate;       // ������ 160000,44100 
    u8      byChannels;           // ������ 1=��������2=˫����
    u32     dwClockRate;          // ʱ��Ƶ�� ��m_dwSamplingRate��ȣ�����1000(����1000ms�����)
    u32     dwBitRate;            // ���� 48000��ʾ48kps

/*   u8      m_bySamplingBits;       // ����λ�� 8��ʾλ��Ϊ8���������
    u32     m_dwSaples;             // ÿ֡������ 160��ʾÿ֡����160������
    u8      m_byFramHeadFlag;       // ��AAC_LCʹ�ã�0=�ޣ�1=��
 */  
    u32     dwParam1;
    u32     dwParam2;

	void SwapToNetwork()
	{
		dwAudioFormat = htonl(dwAudioFormat);
		dwSamplingRate = htonl(dwSamplingRate);
		dwClockRate = htonl(dwClockRate);
		dwBitRate = htonl(dwBitRate);
		dwParam1 = htonl(dwParam1);
		dwParam2 = htonl(dwParam2);
	}
	void SwapFromNetwork()
	{
		dwAudioFormat = ntohl(dwAudioFormat);
		dwSamplingRate = ntohl(dwSamplingRate);
		dwClockRate = ntohl(dwClockRate);
		dwBitRate = ntohl(dwBitRate);
		dwParam1 = ntohl(dwParam1);
		dwParam2 = ntohl(dwParam2);
	}

    TMTIAudioAttribute()
    {
        memset(this, 0 , sizeof(TMTIAudioAttribute));
    }

    void PrintData()
    {
        OspPrintf(TRUE, FALSE, "\t\t��Ƶ�������------\n");
        OspPrintf(TRUE, FALSE, "\t\t�����ʽ:%d\n", dwAudioFormat);
        OspPrintf(TRUE, FALSE, "\t\t������:%d\n", dwSamplingRate);
        OspPrintf(TRUE, FALSE, "\t\t������:%d\n", byChannels);
        OspPrintf(TRUE, FALSE, "\t\tʱ��Ƶ��:%d\n", dwClockRate);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam1:%d\n", dwParam1);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam2:%d\n", dwParam2);

    }
}PACKED TMTI_AUDIO_ATTRI,*PTMTI_AUDIO_ATTRI;

enum
{
    // ��Ƶ�����ʽ
    VIDEO_FORMAT=100,           //u32 ͬVSIP_VIDEO_TYPE_* ������ 
    VIDEO_CLOCK_RATE=101,       // u32 ���磬1000��90000�� �� 
    VIDEO_WIDTH=102,            // u16 ��λΪ���� �� 
    VIDEO_HEIGHT=103,           //u16 ��λΪ���� ֡�� 
    VIDEO_FRAMERATE=104,        // u8 ��λΪfps �������� 
    VIDEO_BITRATETYPE=105,      //u8 1���̶����ʣ�CBR���� 2���ɱ����ʣ�VBR�� ���� 
    VIDEO_BITRATE=106,          // u32 ��λΪKbps

    //��Ƶ�����ʽ 
    AUDIO_FORMAT=200,           // u32 ͬVSIP_AUDIO_FORMAT_* ��ʵ������ 
    AUDIO_SAMPLING_RATE=201,    //u32 ���磬16000��44100�� ������ 
    AUDIO_CHANNELS=202,         // u8 ���磬1=��������2=˫���� ʱ��������� 
    AUDIO_CLOCK_RATE=203,       //u32 ���磬��AUDIO_SAMPLING_RATE��ȣ�ȡֵ��Χ����1000������1000ms������� ���� 
    AUDIO_BITRATE=204,          //u32  ��λΪKBPS
};

typedef struct TMTIStreamAttribute
{
    enum AttributeType
    {
        Nty_Type_None = 0,	    // ���߶���
        Nty_Type_Video,     // ��Ƶ����
        Nty_Type_Audio,		// ��Ƶ����
        Nty_Type_Both,	    // ���߶���
    };
    AttributeType   eType;    // ��������
    TMTIVideoAttribute tMediaAttr;   // ��Ƶ����
    TMTIAudioAttribute tAudioAttr;   // ��Ƶ����
    s8              szManufacturer[MAXLEN_DEVICE_MANUFACTURER + 1]; //�豸������Ϣ
    u32             dwParam1;         //��չ�ֶ�1��
    u32             dwParam2;         //��չ�ֶ�2��

    TMTIStreamAttribute()
    {
        memset(this, 0, sizeof(TMTIStreamAttribute));
    }
	void SwapToNetwork()
	{
		tMediaAttr.SwapToNetwork();
		tAudioAttr.SwapToNetwork();
		dwParam1 = htonl(dwParam1);
		dwParam2 = htonl(dwParam2);
	}
	void SwapFromNetwork()
	{
		tMediaAttr.SwapFromNetwork();
		tAudioAttr.SwapFromNetwork();
		dwParam1 = ntohl(dwParam1);
		dwParam2 = ntohl(dwParam2);
	}
    void PrintData() 
    {
        if (Nty_Type_None == eType)
        {
            OspPrintf(TRUE, FALSE, "\t\t��ͨ��δ��ȡRTP������\n");
            return;
        }
        OspPrintf(TRUE, FALSE, "\t\t��������:%d(0-���ޣ�1-��Ƶ��2-��Ƶ��3-���߶���)\n", eType);
        OspPrintf(TRUE, FALSE, "\t\t����:%s\n", szManufacturer);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam1:%d\n", dwParam1);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam2:%d\n", dwParam2);
        tMediaAttr.PrintData();
        tAudioAttr.PrintData();
    }
    u32 PackedToBuf(s8* pszBuff)
    {
        // �˴�ע��pszBuff�����Ƿ���а�ȫ����
        s16 wNum = 0;
        s16 wTypeID = 0;
        s8  szType = 0;
        s16 wDataLeng = 0;
        s16 wTotalLen = 0;

        if (NULL == pszBuff || Nty_Type_None == eType)
        {
            return wTotalLen;
        }

        //��Ƶ�������
        /*u32     m_dwMediaType;      // �����ʽ VSIP_VIDEO_TYPE_, 
        u32     m_dwClockRate;      // ������ 1000.90000��
        u16     m_wWidth;           // ��
        u16     m_wHeigth;          // ��
        u8      m_byFramerate;      // ֡��
        u8      m_byStreamType;     // 1:�̶����ʣ�2:�ɱ�����
    u32     m_dwBitRate;        // ����

  // ��Ƶ�����ʽ
  VIDEO_FORMAT=100,           //u32 ͬVSIP_VIDEO_TYPE_* ������ 
  VIDEO_CLOCK_RATE=101,       // u32 ���磬1000��90000�� �� 
  VIDEO_WIDTH=102,            // u16 ��λΪ���� �� 
  VIDEO_HEIGHT=103,           //u16 ��λΪ���� ֡�� 
  VIDEO_FRAMERATE=104,        // u8 ��λΪfps �������� 
  VIDEO_BITRATETYPE=105,      //u8 1���̶����ʣ�CBR���� 2���ɱ����ʣ�VBR�� ���� 
    VIDEO_BITRATE=106,          // u32 ��λΪKbps*/

        /*����������n(2)+n*(������id(2)+��������(1)+���ݳ���(2)+����������)*/

        memcpy(pszBuff, &wNum, sizeof(s16)); //����������������޸�
        s8* pszTmp = pszBuff; //����BUFF�ĳ�ʼ��ַ
        wTotalLen += sizeof(s16);

        if (0 != tMediaAttr.dwMediaType)
        {
            wNum++; // ���Ը���
            wTypeID = VIDEO_FORMAT; //����ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tMediaAttr.dwMediaType);

			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);

            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);

			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);

            memcpy(pszBuff+wTotalLen, &tMediaAttr.dwMediaType, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tMediaAttr.dwClockRate)
        {
            wNum++; // ���Ը���
            wTypeID = VIDEO_CLOCK_RATE; //����ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tMediaAttr.dwClockRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.dwClockRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tMediaAttr.wWidth)
        {
            wNum++; // ���Ը���
            wTypeID = VIDEO_WIDTH; //����ID
            szType = VSIP_VALUE_TYPE_SHORT; 
            wDataLeng = sizeof(tMediaAttr.wWidth);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.wWidth, sizeof(u16));
            wTotalLen += sizeof(u16);
        }

        if (0 != tMediaAttr.wHeigth)
        {
            wNum++; // ���Ը���
            wTypeID = VIDEO_HEIGHT; //����ID
            szType = VSIP_VALUE_TYPE_SHORT;
            wDataLeng = sizeof(tMediaAttr.wHeigth);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.wHeigth, sizeof(u16));
            wTotalLen += sizeof(u16);
        }

        if (0 != tMediaAttr.byFramerate)
        {
            wNum++; // ���Ը���
            wTypeID = VIDEO_FRAMERATE; //����ID
            szType = VSIP_VALUE_TYPE_CHAR;
            wDataLeng = sizeof(tMediaAttr.byFramerate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.byFramerate, sizeof(u8));
            wTotalLen += sizeof(u8);
        }

        if (0 != tMediaAttr.byStreamType)
        {
            wNum++; // ���Ը���
            wTypeID = VIDEO_BITRATETYPE; //����ID
            szType = VSIP_VALUE_TYPE_CHAR;
            wDataLeng = sizeof(tMediaAttr.byStreamType);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.byStreamType, sizeof(u8));
            wTotalLen += sizeof(u8);
        }

        if (0 != tMediaAttr.dwBitRate)
        {
            wNum++; // ���Ը���
            wTypeID = VIDEO_BITRATE; //����ID
            szType =VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tMediaAttr.dwBitRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.dwBitRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        /////////////////////////��Ƶ����////////////////////////////////////////////////
        if (0 != tAudioAttr.dwSamplingRate)
        {
            wNum++; // ���Ը���
            wTypeID = AUDIO_SAMPLING_RATE; //����ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tAudioAttr.dwSamplingRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.dwSamplingRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tAudioAttr.dwAudioFormat)
        {
            wNum++; // ���Ը���
            wTypeID = AUDIO_FORMAT; //����ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tAudioAttr.dwAudioFormat);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.dwAudioFormat, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tAudioAttr.byChannels)
        {
            wNum++; // ���Ը���
            wTypeID = AUDIO_CHANNELS; //����ID
            szType = VSIP_VALUE_TYPE_CHAR;
            wDataLeng = sizeof(tAudioAttr.byChannels);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.byChannels, sizeof(u8));
            wTotalLen += sizeof(u8);
        }

        if (0 != tAudioAttr.dwClockRate)
        {
            wNum++; // ���Ը���
            wTypeID = AUDIO_CLOCK_RATE; //����ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tAudioAttr.dwClockRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.dwClockRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tAudioAttr.dwBitRate)
        {
            wNum++; // ���Ը���
            wTypeID = AUDIO_BITRATE; //����ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tAudioAttr.dwBitRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.dwBitRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

		wNum = htons(wNum);
        memcpy(pszTmp, &wNum, sizeof(s16)); //����������������޸�
        return wTotalLen;
    }

    BOOL32 UnpackedFromBuf(char* pszMsg, const u32& dwMsgLen )
    {
        s16 wNum = 0;
        s16 wTypeID = 0;
        s8  szType = 0;
        s16 wDataLeng = 0;
        s8 szData[sizeof(TMTIStreamAttribute)] = {0};
        s16 wTotalLen = 0;

        char* pszTmp = pszMsg;
        memcpy(&wNum, pszTmp, sizeof(s16));
        wNum = ntohs(wNum);

        pszTmp += sizeof(s16);
        wTotalLen += sizeof(s16);

        BOOL32 bIsMedia = FALSE;
        BOOL32 bIsAudio = FALSE;

        while((u32)wTotalLen < dwMsgLen)
        {
            memcpy(&wTypeID, pszTmp, sizeof(s16));
			wTypeID = ntohs(wTypeID);
            pszTmp += sizeof(s16);
            wTotalLen += sizeof(s16);

            pszTmp += sizeof(s8);
            wTotalLen += sizeof(s8);

            memcpy(&wDataLeng, pszTmp, sizeof(s16));
			wDataLeng = ntohs(wDataLeng);

            pszTmp += sizeof(s16);
            wTotalLen += sizeof(s16);

            if (wDataLeng > sizeof(TMTIStreamAttribute))
            {
                return FALSE;
            }

            memcpy(&szData, pszTmp, wDataLeng);
            pszTmp +=wDataLeng;
            wTotalLen += wDataLeng;
            switch(wTypeID)
            {
            case VIDEO_FORMAT:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.dwMediaType, szData, wDataLeng);
					tMediaAttr.dwMediaType = ntohl(tMediaAttr.dwMediaType);
                    break;
                }
            case VIDEO_CLOCK_RATE:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.dwClockRate, szData, wDataLeng);
					tMediaAttr.dwClockRate = ntohl(tMediaAttr.dwClockRate);
                    break;
                }
            case VIDEO_WIDTH:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.wWidth, szData, wDataLeng);
					tMediaAttr.wWidth = ntohs(tMediaAttr.wWidth);
                    break;
                }
            case VIDEO_HEIGHT:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.wHeigth, szData, wDataLeng);
					tMediaAttr.wHeigth = ntohs(tMediaAttr.wHeigth);
                    break;
                }
            case VIDEO_FRAMERATE:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.byFramerate, szData, wDataLeng);

                    break;
                }
            case VIDEO_BITRATETYPE:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.byStreamType, szData, wDataLeng);

                    break;
                }
            case VIDEO_BITRATE:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.dwBitRate, szData, wDataLeng);
					tMediaAttr.dwBitRate = ntohl(tMediaAttr.dwBitRate);
                    break;
                }

                //��Ƶ�����ʽ 
            case AUDIO_FORMAT:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.dwAudioFormat, szData, wDataLeng);
					tAudioAttr.dwAudioFormat = ntohl(tAudioAttr.dwAudioFormat);
                    break;
                }
            case AUDIO_SAMPLING_RATE:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.dwSamplingRate, szData, wDataLeng);
					tAudioAttr.dwSamplingRate = ntohl(tAudioAttr.dwSamplingRate);
                    break;
                }
            case AUDIO_CHANNELS:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.byChannels, szData, wDataLeng);
                    break;
                }
            case AUDIO_CLOCK_RATE:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.dwClockRate, szData, wDataLeng);
					tAudioAttr.dwClockRate = ntohl(tAudioAttr.dwClockRate);
                    break;
                }
            case AUDIO_BITRATE:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.dwBitRate, szData, wDataLeng);
					tAudioAttr.dwBitRate = ntohl(tAudioAttr.dwBitRate);
                    break;
                }
            }
        }
        if (TRUE == bIsMedia && TRUE == bIsAudio)
        {
            eType = Nty_Type_Both;
        }
        else if (TRUE == bIsMedia && FALSE == bIsAudio)
        {
            eType = Nty_Type_Video;
        }
        else if (FALSE == bIsMedia && TRUE == bIsAudio)
        {
            eType = Nty_Type_Audio;
        }
        else
        {
            eType = Nty_Type_None;
        }
		return TRUE;
    }
}PACKED TMTI_STREAM_ATTRI,*PTMTI_STREAM_ATTRI;

typedef struct TMTIStartDevRspEx
{
	u16 wErrCode;
	TMTIStreamAttribute tStreamAttri;

	void PrintData()
	{
		tStreamAttri.PrintData();
	}

	void SwapToNetwork()
	{
		wErrCode = htons(wErrCode);
		tStreamAttri.SwapToNetwork();
	}

	void SwapFromNetwork()
	{
		wErrCode = ntohs(wErrCode);
		tStreamAttri.SwapFromNetwork();
	}

	u32 PackedToBuf(s8 * Buf)
	{
		u32 dwBufLen = tStreamAttri.PackedToBuf(Buf);
		return dwBufLen;
	}

	BOOL32 UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		tStreamAttri.UnpackedFromBuf(Buf,BufLen);
		return TRUE;
	}
}PACKED TMTI_STARTDEV_RSPEX,*PTMTI_STARTDEV_RSPEX;

typedef struct TMTIRecFilePlayNtf
{
	u8 byPlayId;
	TMTIStreamAttribute tStreamAttri;

	void PrintData()
	{
		tStreamAttri.PrintData();
	}

	void SwapToNetwork()
	{
		tStreamAttri.SwapToNetwork();
	}

	void SwapFromNetwork()
	{
		tStreamAttri.SwapFromNetwork();
	}

	u32 PackedToBuf(s8 * Buf)
	{
		u32 dwBufLen = tStreamAttri.PackedToBuf(Buf);
		return dwBufLen;
	}

	BOOL32 UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		tStreamAttri.UnpackedFromBuf(Buf,BufLen);
		return TRUE;
	}
}PACKED TMTI_RECFILEPLAY_NTF,*PTMTI_RECFILEPLAY_NTF;

//͸������֪ͨ
typedef struct TMTITransDataNtf
{
	u16     wMsgType;		    //��Ϣ����
	u8      bIsLast;            //�Ƿ������һ��Ntf
	u32     dwReserved;         //�����ֶ�
	u16	    wDataBufLen;	    //���ݳ���
	u8      abyTransDataBuf[MTI_MAX_CU_OPERTION_NUM];   //͸�����ݽṹ

	void SwapToNetwork()
	{
		wMsgType = htons(wMsgType);
		wDataBufLen = htons(wDataBufLen);
		dwReserved = htonl(dwReserved);
	}

	void SwapFromNetwork()
	{
		wMsgType = ntohs(wMsgType);
		wDataBufLen = ntohs(wDataBufLen);
		dwReserved = ntohl(dwReserved);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		memcpy( Buf, &wMsgType, sizeof(wMsgType));
		Buf += sizeof(wMsgType);
		memcpy( Buf, &bIsLast, sizeof(bIsLast));
		Buf += sizeof(bIsLast);
		memcpy(Buf,&dwReserved,sizeof(dwReserved));
		Buf += sizeof(dwReserved);
		memcpy( Buf, &wDataBufLen, sizeof(wDataBufLen));
		Buf += sizeof(wDataBufLen);
		memcpy( Buf, abyTransDataBuf, sizeof(abyTransDataBuf));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		memcpy( &wMsgType, Buf, sizeof(wMsgType));
		Buf += sizeof(wMsgType);
		memcpy( &bIsLast, Buf, sizeof(bIsLast));
		Buf += sizeof(bIsLast);
		memcpy(&dwReserved,Buf,sizeof(dwReserved));
		Buf += sizeof(dwReserved);
		memcpy( &wDataBufLen, Buf, sizeof(wDataBufLen));
		Buf += sizeof(wDataBufLen);
		memcpy( abyTransDataBuf, Buf, sizeof(abyTransDataBuf));

		return TRUE;
	}

	unsigned short PackedLen() const
	{
		return (sizeof(bIsLast)+sizeof(wMsgType)+sizeof(dwReserved)+sizeof(wDataBufLen)+sizeof(abyTransDataBuf));
	}
}PACKED TMTI_TRANS_DATA_NTF, *PTMTI_TRANS_DATA_NTF;

//�澯״̬����
typedef struct TMTIEntityState
{
	u16		wEntityID;		//ID modify G.J. 20091020 u8->u16
	BOOL	bIsEnable;		//״̬ FALSE-��Ч; TRUE-��Ч
}PACKED TMTI_ENTITY_STATE, *PTMTI_ENTITY_STATE;

//�ƶ����澯����
typedef struct TMTIMotDetState
{
	u16		wEntityID;		//ID modify G.J. 20091020 u8->u16
	u16		wDetAreaNum;	//�澯�����
	BOOL	bIsEnable;		//״̬ FALSE-��Ч;TRUE-��Ч
}PACKED TMTI_MOT_DET_STATE, *PTMTI_MOT_DET_STATE;

typedef struct TMTIDataSend
{
	u16 wDataLen;			//���ݳ���
	u8  abyDataBuf[DATA_BUFSIZE];			//����Buffer
}PACKED TMTI_DATA_SEND, *PTMTI_DATA_SEND;

typedef struct TMTISerialDataSend
{
	u16 wDataLen;			//���ݳ���
	u8 *pbyDataBuf;			//����ָ��
	u8	byChanId;			//����ͨ��
}PACKED TMTI_SERIAL_DATA_SEND, *PTMTI_SERIAL_DATA_SEND;

//��ƵԴ����˿�
typedef struct TMTIVideoInputPort
{
	u8		byVideoEncoderID;	//��Ƶ����ͨ����
	u8		byVideoInputPortID;	//��Ƶ����˿ں�
}PACKED TMTI_VIDEO_IN_PORT, *PTMTI_VIDEO_IN_PORT;

//��ʼ�澯��ʱ
typedef struct TMTIStartAlarmTimer
{
	u16		wSeconds;			//�澯��ʱ��ʾʱ��
	u8		byVideoEncoderID;	//��Ƶ����ͨ����
	u16		wAlarmInputId;		//�澯ͨ���� 	//zlq add  modify 20091020
	u8		byType;				//����	MTI_ALARM_PIN:���ڸ澯 MTI_ALARM_MOTION:�ƶ����澯	//zlq add
}PACKED TMTI_START_ALARM_TIMER, *PTMTI_START_ALARM_TIMER;

//��������ͷ���ش�������
typedef struct TMTISendComData
{
	u8		byVideoInputId;		//��ƵԴ�˿ں�
	u8		bySendAble;			//����ʹ��(MTI_ENBALE-��ʼ����,MTI_DISABLE��ֹͣ����)
}PACKED TMTI_SEND_COM_DATA, *PTMTI_SEND_COM_DATA;

//�û��Զ�����Ϣ
typedef struct TMTIUserCustomMsg
{
	u16		wMsgLen;			//��Ϣ����
	u8	*	pbyMsgContent;		//��������ָ��
}PACKED TMTI_USER_CUSTOM_MSG, *PTMTI_USER_CUSTOM_MSG;

//�ص���Ϣ�����Ϣ
typedef struct TMTIMsgHandle
{
	u8			byMsgType;
	VSIP_HANDLE	tVsipHandle;
}PACKED TMTI_MSG_HANDLE, *PTMTI_MSG_HANDLE;

//�豸��������Ϣ
typedef struct TMTICapItemInfo
{
	u8			byEntityType;	 //��������
	u16			wEntityID;		 //����ID    modify 20091020 u8->u16
	TVsipGuid	tEntityGuid;	 //����GUID
	u16         wFabricant;      //����
}PACKED TMTI_CAP_ITEM_INFO, *PTMTI_CAP_ITEM_INFO;

//�豸���Ƴ�ʼ����Ϣ
typedef struct TMTIInitInfo
{
	TVsipGuid	tUnitGuid;		//�豸GUID
	TVsipGuid	tAppGuid;		//Ӧ�ó���GUID
	u32			dwLocalIpAddr;	//�豸IP��ַ
	u16			wVsipPort;		//�豸VSIP�˿�
	char abyUserName[MAX_STRING_LEN]; //��֤�û���
	char abyPassWord[MAX_STRING_LEN]; //��֤�û�����
}PACKED TMTI_INIT_INFO, *PTMTI_INIT_INFO;

enum eDay
{
	MONDAY,TUESDAY,WEDNESDAY,THURSDAY,FRIDAY,SATURDAY,SUNDAY
};

enum emOsdType  //add by ywy 2014.03.14
{
	LOGO = 0,
	ALARM_CAPTION,
	ALARM_PROMPT
};

//bmposd��Ϣ zlq add
typedef struct TBmpOsdInfo
{
	u8		byVideoPort;		              //��ƵԴ�˿�
	u8		byType;			              	  //0:̨�� 1:�澯��Ļ 2:�澯��ʾ
	u16		wWidth;				              //ͼ����
	u16		wHeigth;			              //ͼ��߶�
	u16		wTotalPacket;	                  //�ܰ�����
	u16		wCurPacketIdx;	                  //��ǰ����������,��1��ʼ
	u16		wCurLen;			              //��ǰ����
	u8      abyBuf[MAX_BMPINFOBUF_LEN];       //����
	u8		byFontNum;			              //�ַ�����
	u8		byFontHeight;		              //�ַ���
	u8		abyFontWidth[MAX_OSD_FONT_WIDTH]; //�ַ����
	void SwapToNetWork()
	{
		wWidth = htons(wWidth);
		wHeigth = htons(wHeigth);
		wTotalPacket = htons(wTotalPacket);
		wCurPacketIdx = htons(wCurPacketIdx);
		wCurLen = htons(wCurLen);
	}
	void SwapFromNetwork()
	{
		wWidth = ntohs(wWidth);
		wHeigth = ntohs(wHeigth);
		wTotalPacket = ntohs(wTotalPacket);
		wCurPacketIdx = ntohs(wCurPacketIdx);
		wCurLen = ntohs(wCurLen);
	}
}PACKED TMT_BMPOSD_INFO, *PTMT_BMPOSD_INFO;

typedef struct TBmpOsdAckInfo
{
	u16		wTransactionNum;	//��ˮ��
	u16		wErrorCode;			//������� 0:ʧ�� 1:�ɹ�
	u16		wCurPacketIdx;		//������
}PACKED TMT_BMPOSDACK_INFO, *PTMT_BMPOSDACK_INFO;
//add end

//�������� ǰ�˴洢 zlq add
typedef struct TMTIMatrixInfo
{
	u8		byMatrixType;						        //��������
	u8		byMatrixInputPort;					        //������ʼ����˿�
	u8		byMatrixOutputPort;					        //������ʼ����˿�
	u8		byMatrixInputNum;					        //��������˿���
	u8		byMatrixOutputNum;					        //��������˿���
	u8		abyMatrixPort[MAX_VIDEO_INPUT_PORT_NUM];	//����ӳ���
}PACKED PAKTMT_MATRIX_INFO,*PTMT_MATRIX_INFO;

typedef struct TRecStartPlayInfo
{
	u32 dwIndexId;
	u32 dwIpAddress;
	u16 wVideoPort;
	u16 wAudioPort;
	u8  byType;
	u32 dwStartTime;
	u32 dwEndTime;
	u32 dwSSRC;
	u16 wVidSrcRtpPort;
	u16 wAudSrcRtpPort;
	u16 wVidSrcRtcpPort;
	u16 wAudSrcRtcpPort;
	u16 wVidDestRtcpPort;
	u16 wAudDestRtcpPort;

	void SwapToNetwork()
	{
		dwIndexId = htonl(dwIndexId);
		wVideoPort = htons(wVideoPort);
		wAudioPort = htons(wAudioPort);
		dwStartTime = htonl(dwStartTime);
	    dwEndTime= htonl(dwEndTime);
		dwSSRC = htonl(dwSSRC);
		wVidSrcRtpPort = htons(wVidSrcRtpPort);
		wAudSrcRtpPort = htons(wAudSrcRtpPort);
		wVidSrcRtcpPort = htons(wVidSrcRtcpPort);
		wAudSrcRtcpPort = htons(wAudSrcRtcpPort);
		wVidDestRtcpPort = htons(wVidDestRtcpPort);
		wAudDestRtcpPort = htons(wAudDestRtcpPort);
	}
	void SwapFromNetwork()
	{
		dwIndexId = ntohl(dwIndexId);
		wVideoPort = ntohs(wVideoPort);
		wAudioPort = ntohs(wAudioPort);
		dwStartTime = ntohl(dwStartTime);
	    dwEndTime= ntohl(dwEndTime);
		dwSSRC = ntohl(dwSSRC);
		wVidSrcRtpPort = ntohs(wVidSrcRtpPort);
		wAudSrcRtpPort = ntohs(wAudSrcRtpPort);
		wVidSrcRtcpPort = ntohs(wVidSrcRtcpPort);
		wAudSrcRtcpPort = ntohs(wAudSrcRtcpPort);
		wVidDestRtcpPort = ntohs(wVidDestRtcpPort);
		wAudDestRtcpPort = ntohs(wAudDestRtcpPort);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwIndexId, sizeof(dwIndexId));
		Buf += sizeof(dwIndexId);
		memcpy( Buf, &dwIpAddress, sizeof(dwIpAddress));
		Buf += sizeof(dwIpAddress);
		memcpy( Buf, &wVideoPort,sizeof(wVideoPort));
		Buf += sizeof(wVideoPort);
		memcpy( Buf,&wAudioPort,sizeof(wAudioPort));
		Buf += sizeof(wAudioPort);
		memcpy( Buf, &byType, sizeof(byType));
		Buf += sizeof(byType);
		memcpy( Buf, &dwStartTime,sizeof(dwStartTime));
		Buf += sizeof(dwStartTime);
		memcpy( Buf,&dwEndTime,sizeof(dwEndTime));
		Buf += sizeof(dwEndTime);
		memcpy( Buf,&dwSSRC,sizeof(dwSSRC));
		Buf += sizeof(dwSSRC);
		memcpy( Buf,&wVidSrcRtpPort,sizeof(wVidSrcRtpPort));
		Buf += sizeof(wVidSrcRtpPort);
		memcpy( Buf,&wAudSrcRtpPort,sizeof(wAudSrcRtpPort));
		Buf += sizeof(wAudSrcRtpPort);
		memcpy( Buf,&wVidSrcRtcpPort,sizeof(wVidSrcRtcpPort));
		Buf += sizeof(wVidSrcRtcpPort);
		memcpy( Buf,&wAudSrcRtcpPort,sizeof(wAudSrcRtcpPort));
		Buf += sizeof(wAudSrcRtcpPort);
		memcpy( Buf,&wVidDestRtcpPort,sizeof(wVidDestRtcpPort));
		Buf += sizeof(wVidDestRtcpPort);
		memcpy( Buf,&wAudDestRtcpPort,sizeof(wAudDestRtcpPort));
		Buf += sizeof(wAudDestRtcpPort);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwIndexId, Buf, sizeof(dwIndexId));
		Buf += sizeof(dwIndexId);
		memcpy( &dwIpAddress, Buf, sizeof(dwIpAddress));
		Buf += sizeof(dwIpAddress);
		memcpy( &wVideoPort,Buf, sizeof(wVideoPort));
		Buf += sizeof(wVideoPort);
		memcpy( &wAudioPort,Buf,sizeof(wAudioPort));
		Buf += sizeof(wAudioPort);
		memcpy( &byType, Buf, sizeof(byType));
		Buf += sizeof(byType);
		memcpy( &dwStartTime,Buf, sizeof(dwStartTime));
		Buf += sizeof(dwStartTime);
		memcpy( &dwEndTime,Buf,sizeof(dwEndTime));
		Buf += sizeof(dwEndTime);
		memcpy( &dwSSRC,Buf,sizeof(dwSSRC));
		Buf += sizeof(dwSSRC);
		memcpy( &wVidSrcRtpPort,Buf,sizeof(wVidSrcRtpPort));
		Buf += sizeof(wVidSrcRtpPort);
		memcpy( &wAudSrcRtpPort,Buf,sizeof(wAudSrcRtpPort));
		Buf += sizeof(wAudSrcRtpPort);
		memcpy(&wVidSrcRtcpPort,Buf,sizeof(wVidSrcRtcpPort));
		Buf += sizeof(wVidSrcRtcpPort);
		memcpy( &wAudSrcRtcpPort,Buf,sizeof(wAudSrcRtcpPort));
		Buf += sizeof(wAudSrcRtcpPort);
		memcpy( &wVidDestRtcpPort,Buf,sizeof(wVidDestRtcpPort));
		Buf += sizeof(wVidDestRtcpPort);
		memcpy( &wAudDestRtcpPort,Buf,sizeof(wAudDestRtcpPort));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(dwIndexId)+sizeof(dwIpAddress)+sizeof(wVideoPort)+
			sizeof(wAudioPort)+sizeof(byType)+sizeof(dwStartTime)+sizeof(dwEndTime)+
			sizeof(dwSSRC)+sizeof(wVidSrcRtpPort)+sizeof(wAudSrcRtpPort)+sizeof(wVidSrcRtcpPort)+
			sizeof(wAudSrcRtcpPort)+sizeof(wVidDestRtcpPort)+sizeof(wAudDestRtcpPort));
	}

}PACKED TREC_START_PLAY_INFO,*PTREC_START_PLAY_INFO;

//���ܲ���
typedef struct TNmsParam
{
	u8		byMemThreshold;        //ϵͳ�ڴ�ʹ�÷�ֵ
	u8      byMaxVideoLoseRatio;   //��Ƶ�����ϱ���ֵ
	u32     dwServerIP;            //���ܷ�����IP��ַ
	u8      byCpuUseRatio;         //cpu������

	void SwapToNetwork()
	{
		//dwServerIP = htonl(dwServerIP);
	}
	void SwapFromNetwork()
	{
		//dwServerIP = ntohl(dwServerIP);
	}
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &byMemThreshold, sizeof(byMemThreshold));
		Buf += sizeof(byMemThreshold);
		memcpy( Buf, &byMaxVideoLoseRatio, sizeof(byMaxVideoLoseRatio));
		Buf += sizeof(byMaxVideoLoseRatio);
		memcpy( Buf, &dwServerIP,sizeof(dwServerIP));
		Buf += sizeof(dwServerIP);
		memcpy( Buf,&byCpuUseRatio,sizeof(byCpuUseRatio));
		return TRUE;
	}
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &byMemThreshold, Buf, sizeof(byMemThreshold));
		Buf += sizeof(byMemThreshold);
		memcpy( &byMaxVideoLoseRatio, Buf, sizeof(byMaxVideoLoseRatio));
		Buf += sizeof(byMaxVideoLoseRatio);
		memcpy( &dwServerIP,Buf, sizeof(dwServerIP));
		Buf += sizeof(dwServerIP);
		memcpy( &byCpuUseRatio,Buf,sizeof(byCpuUseRatio));
		return TRUE;
	}
	unsigned short PackedLen()
	{
		return (sizeof(byMemThreshold)+sizeof(byMaxVideoLoseRatio)+sizeof(dwServerIP)+sizeof(byCpuUseRatio));
	}
}PACKED TNMS_PARAM,*PTNMS_PARAM;


//������Ӧ��
typedef struct TPlayerRsp
{
	u16		wTransactionNum;	//������ˮ��
	u8		byPlayerId;			//player id

}PACKED TPLAYER_RSP, *PTPLAYER_RSP;

typedef struct TDevRegeditInfo
{
	u8  abyPuid[MAX_VERSION_STRING_LEN];
	u8  abyPassword[MAX_VERSION_STRING_LEN];
	u16 wPort;
	void SwapToNetwork()
	{
		wPort = htons(wPort);
	}

	void SwapFromNetwork()
	{
		wPort = ntohs(wPort);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, abyPuid, sizeof(abyPuid));
		Buf += sizeof(abyPuid);
		memcpy( Buf, abyPassword, sizeof(abyPassword));
		Buf += sizeof(abyPassword);
		memcpy( Buf, &wPort,sizeof(wPort));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( abyPuid, Buf, sizeof(abyPuid));
		Buf += sizeof(abyPuid);
		memcpy( abyPassword, Buf, sizeof(abyPassword));
		Buf += sizeof(abyPassword);
		memcpy( &wPort,Buf,sizeof(wPort));
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(abyPuid)+sizeof(abyPassword)+sizeof(wPort));
	}

}PACKED TDEV_REGEDIT_INFO, *PTDEV_REGEDIT_INFO;



//��ʼֹͣ¼��ʱ�����ýṹ
typedef struct RecStartEndTimeInfo
{
	TMTISysTime StartTime;     //��ʼ¼��ʱ��
	TMTISysTime EndTime;       //ֹͣ¼��ʱ��

	void SwapToNetwork()
	{
		StartTime.SwapToNetwork();
		EndTime.SwapToNetwork();
	}

	void SwapFromNetwork()
	{
		StartTime.SwapFromNetwork();
		EndTime.SwapFromNetwork();
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		StartTime.PackedToBuf(Buf,BufLen);
		Buf += StartTime.PackedLen();
		EndTime.PackedToBuf(Buf,BufLen-StartTime.PackedLen());
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
        StartTime.UnpackedFromBuf(Buf,BufLen);
		Buf += StartTime.PackedLen();
		EndTime.UnpackedFromBuf(Buf,BufLen-StartTime.PackedLen());
		return TRUE;
	}


	unsigned short PackedLen()
	{
		return (StartTime.PackedLen()+EndTime.PackedLen());
	}

}PACKED REC_START_END_TIME_INFO, *PREC_START_END_TIME_INFO;

typedef struct TQueryParam
{
	u8 byChanId;
	RecStartEndTimeInfo RStartEndTimeInfo;
	u8 byType;
	u32 dwFileIndex;
	u16 wNum;

	void SwapToNetwork()
	{
		RStartEndTimeInfo.SwapToNetwork();
		dwFileIndex = htonl(dwFileIndex);
		wNum = htons(wNum);
	}
	void SwapFromNetwork()
	{
		RStartEndTimeInfo.SwapFromNetwork();
		dwFileIndex = ntohl(dwFileIndex);
		wNum = ntohs(wNum);
	}
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &byChanId, sizeof(byChanId));
		Buf += sizeof(byChanId);

        RStartEndTimeInfo.PackedToBuf(Buf,BufLen-sizeof(byChanId));
		Buf += RStartEndTimeInfo.PackedLen();

		memcpy( Buf, &byType,sizeof(byType));
		Buf += sizeof(byType);
        memcpy( Buf, &dwFileIndex,sizeof(dwFileIndex));
		Buf += sizeof(dwFileIndex);
        memcpy( Buf, &wNum,sizeof(wNum));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &byChanId, Buf, sizeof(byChanId));
		Buf += sizeof(byChanId);

        RStartEndTimeInfo.UnpackedFromBuf(Buf,BufLen-sizeof(byChanId));
		Buf += RStartEndTimeInfo.PackedLen();

		memcpy( &byType,Buf, sizeof(byType));
		Buf += sizeof(byType);
        memcpy( &dwFileIndex,Buf, sizeof(dwFileIndex));
		Buf += sizeof(dwFileIndex);
        memcpy( &wNum,Buf, sizeof(wNum));
		return TRUE;
	}
	unsigned short PackedLen()
	{
		return (sizeof(byChanId)+RStartEndTimeInfo.PackedLen()+sizeof(dwFileIndex)+sizeof(byType)+sizeof(wNum));
	}
}PACKED TQueryParam,*PTQueryParam;

//��ʱ�䡢¼����Ϣ
typedef struct TimelyRecInfo
{
    u8 byEnable;
	RecStartEndTimeInfo RecSETimeInfo;
	void SwapToNetwork()
	{
		RecSETimeInfo.SwapToNetwork();
	}
	void SwapFromNetwork()
	{
		RecSETimeInfo.SwapFromNetwork();
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &byEnable, sizeof(byEnable));
		Buf += sizeof(byEnable);
		RecSETimeInfo.PackedToBuf(Buf,BufLen-sizeof(byEnable));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
	    memcpy( &byEnable, Buf, sizeof(byEnable));
		Buf += sizeof(byEnable);
		RecSETimeInfo.UnpackedFromBuf(Buf,BufLen-sizeof(byEnable));
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(byEnable)+RecSETimeInfo.PackedLen());
	}

}PACKED TIMELY_REC_INFO, *PTIMELY_REC_INFO;

//¼���ļ�/�¼���Ϣ
typedef struct RecFileInfo
{
	u32 dwRecIndexId;                             //�ļ��¼�id��
	u8 byRecFileType;                             //¼���ļ�/�¼�����
	RecStartEndTimeInfo RecSETimeSetting;         //��ʼֹͣ¼��ʱ��
	u8 abyFileName[MAX_FILE_NAME_LENGTH+1];       //�ļ�����
	u8 abyFilePath[MAX_FILE_PATH_LENGTH+1];       //����ļ�·������

	void SetH264Type()
	{
		byRecFileType += MTI_TYPE_H264;
	}
	BOOL IsH264Type()
	{
		if (byRecFileType > MTI_TYPE_H264)
		{
			byRecFileType -= MTI_TYPE_H264;
			return TRUE;
		}
		return FALSE;
	}
	void SwapToNetwork()
	{
		dwRecIndexId = htonl(dwRecIndexId);
    	RecSETimeSetting.SwapToNetwork();
	}
	void SwapFromNetwork()
	{
	   dwRecIndexId = ntohl(dwRecIndexId);
       RecSETimeSetting.SwapFromNetwork();
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwRecIndexId, sizeof(dwRecIndexId));
		Buf += sizeof(dwRecIndexId);
		memcpy( Buf,&byRecFileType,sizeof(byRecFileType));
		Buf += sizeof(byRecFileType);

        RecSETimeSetting.PackedToBuf(Buf,BufLen-sizeof(dwRecIndexId)-sizeof(byRecFileType));
		Buf += RecSETimeSetting.PackedLen();

		memcpy(Buf,abyFileName,sizeof(abyFileName));
		Buf += sizeof(abyFileName);
		memcpy(Buf,abyFilePath,sizeof(abyFilePath));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
	   	memcpy( &dwRecIndexId, Buf, sizeof(dwRecIndexId));
		Buf += sizeof(dwRecIndexId);
		memcpy( &byRecFileType,Buf, sizeof(byRecFileType));
		Buf += sizeof(byRecFileType);

        RecSETimeSetting.UnpackedFromBuf(Buf,BufLen-sizeof(dwRecIndexId)-sizeof(byRecFileType));
		Buf += RecSETimeSetting.PackedLen();
		memcpy(abyFileName,Buf,sizeof(abyFileName));
	    Buf += sizeof(abyFileName);
		memcpy(abyFilePath,Buf,sizeof(abyFilePath));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(dwRecIndexId)+sizeof(byRecFileType)
			+RecSETimeSetting.PackedLen()+sizeof(abyFileName)+sizeof(abyFilePath));
	}
}PACKED REC_FILE_INFO,* PREC_FILE_INFO;

//¼����״̬��Ϣ
typedef struct RecPlayerState
{
	u8 bIsManualRec;		// �Ƿ����ֶ�¼��
	u8 bIsMotiveAlarmRec;	// �Ƿ����ƶ����澯¼��
	u8 bIsPinAlarmRec;	    // �Ƿ��ڲ��ڸ澯¼��
    u8 bIsTimerRec;	    	// �Ƿ��ڶ�ʱ¼��
	u8 bIsWeekRec;		    // �Ƿ�������¼��
	u8 bIsPreRec;			// �Ƿ���Ԥ¼

    void SwapToNetwork()
	{
	}
	void SwapFromNetwork()
	{
	}
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &bIsManualRec, sizeof(bIsManualRec));
		Buf += sizeof(bIsManualRec);
		memcpy(Buf, &bIsMotiveAlarmRec,sizeof(bIsMotiveAlarmRec));
		Buf += sizeof(bIsMotiveAlarmRec);
		memcpy( Buf, &bIsPinAlarmRec, sizeof(bIsPinAlarmRec));
        Buf += sizeof(bIsPinAlarmRec);
		memcpy( Buf, &bIsTimerRec, sizeof(bIsTimerRec));
		Buf += sizeof(bIsTimerRec);
		memcpy( Buf, &bIsWeekRec, sizeof(bIsWeekRec));
		Buf += sizeof(bIsWeekRec);
	    memcpy( Buf, &bIsPreRec, sizeof(bIsPreRec));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &bIsManualRec,Buf, sizeof(bIsManualRec));
		Buf += sizeof(bIsManualRec);
		memcpy(&bIsMotiveAlarmRec,Buf, sizeof(bIsMotiveAlarmRec));
		Buf += sizeof(bIsMotiveAlarmRec);
		memcpy( &bIsPinAlarmRec, Buf, sizeof(bIsPinAlarmRec));
        Buf += sizeof(bIsPinAlarmRec);
		memcpy( &bIsTimerRec, Buf, sizeof(bIsTimerRec));
		Buf += sizeof(bIsTimerRec);
		memcpy( &bIsWeekRec,Buf,  sizeof(bIsWeekRec));
		Buf += sizeof(bIsWeekRec);
	    memcpy( &bIsPreRec, Buf,sizeof(bIsPreRec));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (6*sizeof(u8));
	}


}PACKED REC_PLAYER_STATE,* PREC_PLAYER_STATE;

//���Ȳ�����Ϣ
typedef struct ProgressParaInfo
{
	u8  abyCreateTime[MAX_TIME_PRECISION];      //¼���ļ�����ʱ��,��������
	u32 dwTotalFileTime;                        //�ļ���ʱ��
	u32 dwCurrentPlayTime;                      //��ǰ����ʱ��
	u8 byPlayerId;                              //������id
	u8 byIsFinished;                            //�Ƿ񲥷Ž���
	u16 wTransactionNum;                        //������ˮ��

    void SwapToNetwork()
	{
		dwTotalFileTime = htonl(dwTotalFileTime);
		dwCurrentPlayTime = htonl(dwCurrentPlayTime);
		wTransactionNum  = htons(wTransactionNum);
	}
	void SwapFromNetwork()
	{
        dwTotalFileTime = ntohl(dwTotalFileTime);
		dwCurrentPlayTime = ntohl(dwCurrentPlayTime);
		wTransactionNum = ntohs(wTransactionNum);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, abyCreateTime, sizeof(abyCreateTime));
		Buf += sizeof(abyCreateTime);
		memcpy(Buf, &dwTotalFileTime,sizeof(dwTotalFileTime));
		Buf += sizeof(dwTotalFileTime);
		memcpy( Buf, &dwCurrentPlayTime, sizeof(dwCurrentPlayTime));
        Buf += sizeof(dwCurrentPlayTime);
		memcpy( Buf, &byPlayerId, sizeof(byPlayerId));
		Buf += sizeof(byPlayerId);
		memcpy( Buf, &byIsFinished,sizeof(byIsFinished));
		Buf += sizeof(byIsFinished);
		memcpy( Buf,&wTransactionNum,sizeof(wTransactionNum));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( abyCreateTime, Buf, sizeof(abyCreateTime));
		Buf += sizeof(abyCreateTime);
		memcpy( &dwTotalFileTime,Buf,sizeof(dwTotalFileTime));
		Buf += sizeof(dwTotalFileTime);
		memcpy( &dwCurrentPlayTime,Buf, sizeof(dwCurrentPlayTime));
		Buf += sizeof(dwCurrentPlayTime);
		memcpy( &byPlayerId, Buf,sizeof(byPlayerId));
		Buf += sizeof(byPlayerId);
		memcpy( &byIsFinished,Buf, sizeof(byIsFinished));
		Buf += sizeof(byIsFinished);
		memcpy( &wTransactionNum,Buf,sizeof(wTransactionNum));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(abyCreateTime)+sizeof(dwTotalFileTime)+sizeof(dwCurrentPlayTime)+sizeof(byPlayerId)+sizeof(byIsFinished)+sizeof(wTransactionNum));
	}

}PACKED PROGRESS_PARA_INFO,*PPROGRESS_PARA_INFO;

//��ѯ�ļ���Ϣ��
typedef struct QueryRecFileInfo
{
	u16 wNum;                                   //¼���ļ���Ŀ
	u8 byType;                                  //¼���ļ�/�¼�����
	u8 bIsFinished;                             //�Ƿ������е�¼���ļ�,1Ϊ������0Ϊδ����
	RecFileInfo aRFileInfo[MAX_REC_FILE_NUM];   //¼���ļ���Ϣ
	u16	wTransactionNum;                        //�ļ�������ˮ��

	void SwapToNetwork()
	{
		wNum = htons(wNum);
		for(int i=0;i<MAX_REC_FILE_NUM;++i)
		{
			aRFileInfo[i].SwapToNetwork();
		}
        wTransactionNum = htons(wTransactionNum);
	}
	void SwapFromNetwork()
	{
		wNum = ntohs(wNum);
		for(int i=0;i<MAX_REC_FILE_NUM;++i)
		{
			aRFileInfo[i].SwapFromNetwork();
		}
		wTransactionNum = ntohs(wTransactionNum);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		int i=0;
		int len = BufLen;
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wNum, sizeof(wNum));
		Buf += sizeof(wNum);
		memcpy(Buf, &byType,sizeof(byType));
		Buf += sizeof(byType);
		memcpy( Buf, &bIsFinished, sizeof(bIsFinished));
		Buf += sizeof(bIsFinished);
		len = len - sizeof(wNum)-sizeof(byType)-sizeof(bIsFinished);

		for(i=0;i<MAX_REC_FILE_NUM;i++)
		{
		   aRFileInfo[i].PackedToBuf(Buf,len);
		   Buf += aRFileInfo[i].PackedLen();
           len = len - aRFileInfo[i].PackedLen();
		}
		memcpy(Buf,&wTransactionNum,sizeof(wTransactionNum));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		int i=0;
		int len = BufLen;
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &wNum, Buf, sizeof(wNum));
		Buf += sizeof(wNum);
		memcpy( &byType,Buf,sizeof(byType));
		Buf += sizeof(byType);
		memcpy( &bIsFinished,Buf, sizeof(bIsFinished));
		Buf += sizeof(bIsFinished);
		len = len - sizeof(wNum)-sizeof(byType)-sizeof(bIsFinished);

		for(i=0;i<MAX_REC_FILE_NUM;i++)
		{
		   aRFileInfo[i].UnpackedFromBuf(Buf,len);
		   Buf += aRFileInfo[i].PackedLen();
           len = len - aRFileInfo[i].PackedLen();
		}
		memcpy(&wTransactionNum, Buf, sizeof(wTransactionNum));
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(wNum)+sizeof(byType)+sizeof(bIsFinished)+MAX_REC_FILE_NUM*aRFileInfo[0].PackedLen())
			+sizeof(wTransactionNum);
	}

}PACKED QUERY_REC_FILE_INFO,*PQUERY_REC_FILE_INFO;

//����¼��ʱ������
typedef struct PeriodlyTimeInfo
{
	u8 byPeriodType;      //MTI_NONE_PERIOD,MTI_PERIOD_DAYLY,MTI_PERIOD_SELECTION
	RecStartEndTimeInfo aRTimeInfo[ 3*(DAYS_OF_WEEK+1)];

	void SwapToNetwork()
	{
		for(int i=0; i<3*(DAYS_OF_WEEK+1);++i)
		{
			aRTimeInfo[i].SwapToNetwork();
		}
	}

	void SwapFromNetwork()
	{
		for(int i=0; i<3*(DAYS_OF_WEEK+1); ++i)
		{
			aRTimeInfo[i].SwapFromNetwork();
		}
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		u16 len = BufLen;
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( Buf, &byPeriodType, sizeof(byPeriodType));
		Buf += sizeof(byPeriodType);
		len = BufLen - sizeof(byPeriodType);
		for (int i = 0; i<3*(DAYS_OF_WEEK+1); i++)
		{
			aRTimeInfo[i].PackedToBuf(Buf,len);
			Buf += aRTimeInfo[i].PackedLen();
			len = BufLen - aRTimeInfo[i].PackedLen();

		}
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		u16 len = BufLen;
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &byPeriodType, Buf, sizeof(byPeriodType));
		Buf += sizeof(byPeriodType);
		len = BufLen - sizeof(byPeriodType);
		for (int i = 0; i<3*(DAYS_OF_WEEK+1); i++)
		{
			aRTimeInfo[i].UnpackedFromBuf(Buf,len);
			Buf += aRTimeInfo[i].PackedLen();
			len = BufLen - aRTimeInfo[i].PackedLen();

		}

		return TRUE;
	}

	unsigned short PackedLen()
	{
    	return (sizeof(byPeriodType)+3*(DAYS_OF_WEEK+1)*aRTimeInfo[0].PackedLen());
	}
}PACKED PERIODLY_TIME_INFO,*PPERIODLY_TIME_INFO;

typedef struct RecChannelBasicInfo
{
	u16 wAlarmPreRecTime;        //�澯Ԥ¼ʱ��
	u16 wAlarmResumeRecTime;     //�澯�ָ������¼��ʱ��
	u8 byIsAbstractFrame;        //�Ƿ��֡¼�� 1Ϊ�ǣ�0Ϊ��
	u8 byIsRecWhenAlarmOccur;    //�澯�Ƿ�¼��,1Ϊ��,0Ϊ��
	u8 byAbstractRecInterval;    //��֡¼����,ָ�ؼ�֡����
	u8 byIsAudioRecorded;        //�Ƿ�¼��Ƶ
#ifdef VSIP_MULTI_CHAN_SUPPORT
    u8 byRecChanNum;             //0-¼����������������¼�Ƹ���������,Ĭ��Ϊ0
#endif

	void SwapToNetwork()
	{
		wAlarmPreRecTime = htons(wAlarmPreRecTime);
		wAlarmResumeRecTime = htons(wAlarmResumeRecTime);
	}

	void SwapFromNetwork()
	{
		wAlarmPreRecTime = ntohs(wAlarmPreRecTime);
		wAlarmResumeRecTime = ntohs(wAlarmResumeRecTime);
	}


	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wAlarmPreRecTime, sizeof(wAlarmPreRecTime));
		Buf += sizeof(wAlarmPreRecTime);
		memcpy( Buf, &wAlarmResumeRecTime, sizeof(wAlarmResumeRecTime));
		Buf += sizeof(wAlarmResumeRecTime);
		memcpy( Buf, &byIsAbstractFrame, sizeof(byIsAbstractFrame));
		Buf += sizeof(byIsAbstractFrame);
		memcpy( Buf, &byIsRecWhenAlarmOccur, sizeof(byIsRecWhenAlarmOccur));
		Buf += sizeof(byIsRecWhenAlarmOccur);
		memcpy( Buf, &byAbstractRecInterval, sizeof(byAbstractRecInterval));
        Buf += sizeof(byAbstractRecInterval);
        memcpy( Buf ,&byIsAudioRecorded,sizeof(byIsAudioRecorded));
#ifdef VSIP_MULTI_CHAN_SUPPORT
        Buf += sizeof(byIsAudioRecorded);
        memcpy( Buf ,&byRecChanNum,sizeof(byRecChanNum));	
#endif
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &wAlarmPreRecTime, Buf, sizeof(wAlarmPreRecTime));
		Buf += sizeof(wAlarmPreRecTime);
		memcpy( &wAlarmResumeRecTime, Buf, sizeof(wAlarmResumeRecTime));
		Buf += sizeof(wAlarmResumeRecTime);
		memcpy( &byIsAbstractFrame, Buf, sizeof(byIsAbstractFrame));
		Buf += sizeof(byIsAbstractFrame);
		memcpy(&byIsRecWhenAlarmOccur, Buf, sizeof(byIsRecWhenAlarmOccur));
		Buf += sizeof(byIsRecWhenAlarmOccur);
		memcpy(&byAbstractRecInterval, Buf, sizeof(byAbstractRecInterval));
		Buf += sizeof(byAbstractRecInterval);
        memcpy(&byIsAudioRecorded, Buf,sizeof(byIsAudioRecorded));
#ifdef VSIP_MULTI_CHAN_SUPPORT
        Buf += sizeof(byIsAudioRecorded);
        memcpy( &byRecChanNum, Buf, sizeof(byRecChanNum));
#endif
                
		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 len;
		len = sizeof(wAlarmPreRecTime)+sizeof(wAlarmResumeRecTime)+sizeof(byIsAbstractFrame)+
                        sizeof(byIsRecWhenAlarmOccur)+sizeof(byAbstractRecInterval)+sizeof(byIsAudioRecorded);
#ifdef VSIP_MULTI_CHAN_SUPPORT
		len = len + sizeof (byRecChanNum);
#endif
		return len;
	}
}PACKED REC_CHANNEL_BASIC_INFO,*PREC_CHANNEL_BASIC_INFO;

typedef struct RecSysInfo
{
	u8 byDiskFullStrategy;                                                 // ������¼�����
	u8 abyRecFileStorePath[MTI_DISK_NUM];                                  // ¼���ļ����·��
	u32 dwDiskRemainSpace;                                                 // ����ʣ��ռ䣨M��
	u16 wLittleFileRecTime;                                                // С�ļ�¼��ʱ��
	u8 abyRecDiskPartitionInfo[MTI_DISK_NUM][MAX_DISK_PARTITION_NAME];     // ���̷�����Ϣ���û���������
	u8 byDiskPartitionNum;                                                 // �ļ�������Ŀ,�û���������
	u8 byUsedPartitionNum;                                                 // �û�ʹ���˵ķ�����Ŀ

    void SwapToNetwork()
	{
		dwDiskRemainSpace = htonl(dwDiskRemainSpace);
		wLittleFileRecTime = htons(wLittleFileRecTime);
	}

	void SwapFromNetwork()
	{
		dwDiskRemainSpace = ntohl(dwDiskRemainSpace);
		wLittleFileRecTime = ntohs(wLittleFileRecTime);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &byDiskFullStrategy, sizeof(byDiskFullStrategy));
		Buf += sizeof(byDiskFullStrategy);
		memcpy( Buf, abyRecFileStorePath, sizeof(abyRecFileStorePath));
		Buf += sizeof(abyRecFileStorePath);
		memcpy( Buf, &dwDiskRemainSpace, sizeof(dwDiskRemainSpace));
		Buf += sizeof(dwDiskRemainSpace);
		memcpy( Buf, &wLittleFileRecTime, sizeof(wLittleFileRecTime));
		Buf += sizeof(wLittleFileRecTime);
		memcpy(Buf,abyRecDiskPartitionInfo,sizeof(abyRecDiskPartitionInfo));
		Buf += sizeof(abyRecDiskPartitionInfo);
		memcpy(Buf,&byDiskPartitionNum,sizeof(byDiskPartitionNum));
        Buf += sizeof(byDiskPartitionNum);
		memcpy(Buf,&byUsedPartitionNum,sizeof(byUsedPartitionNum));


		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &byDiskFullStrategy, Buf, sizeof(byDiskFullStrategy));
		Buf += sizeof(byDiskFullStrategy);
		memcpy( abyRecFileStorePath, Buf, sizeof(abyRecFileStorePath));
		Buf += sizeof(abyRecFileStorePath);
		memcpy( &dwDiskRemainSpace, Buf, sizeof(dwDiskRemainSpace));
		Buf += sizeof(dwDiskRemainSpace);
		memcpy( &wLittleFileRecTime, Buf, sizeof(wLittleFileRecTime));
		Buf += sizeof(wLittleFileRecTime);
		memcpy(abyRecDiskPartitionInfo,Buf,sizeof(abyRecDiskPartitionInfo));
		Buf += sizeof(abyRecDiskPartitionInfo);
		memcpy(&byDiskPartitionNum,Buf,sizeof(byDiskPartitionNum));
		 Buf += sizeof(byDiskPartitionNum);
		memcpy(&byUsedPartitionNum,Buf,sizeof(byUsedPartitionNum));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(byDiskFullStrategy)+sizeof(abyRecFileStorePath)+sizeof(dwDiskRemainSpace)+sizeof(wLittleFileRecTime)+
			sizeof(abyRecDiskPartitionInfo)+sizeof(byDiskPartitionNum)+sizeof(byUsedPartitionNum));
	}

}PACKED REC_SYS_INFO,*PREC_SYS_INFO;

//zlq add ץ�Ĺ�������
typedef struct TMTISnapShotConfig
{
	u8 byPinAlarmEnable;		//���ڸ澯ץͼʹ�� 0�ر�/1����
	u8 byMotionDetectEnable;	//�ƶ����ץͼʹ�� 0�ر�/1����
	u32	dwInterval;				//ץ��ʱ����(s)
	u32 dwMaxSnapNum;			//���ץ������
}PACKED SNAP_SHOT_CONFIG,*PSNAP_SHOT_CONFIG;

typedef struct TMTISnapShotNamingRule
{
	// �ֶ�˳�򣬴�0��ʼ�����α�ʾ��i+1���ֶ�
	u8 abyNameOrder[MAX_SEPARATOR_LEN];
	// ʵ��ʹ�õ��ֶθ���
	u8 byNameNum;
	//  ����֮��ķָ���(������)
	char achDateSeparator[MAX_SEPARATOR_LEN];
	// ʱ��֮��ķָ���(ʱ����)
	char achTimeSeparator[MAX_SEPARATOR_LEN];
	// �����ֶ�֮��ķָ���
	char achNameSeparator[MAX_SEPARATOR_LEN];
	// �û��Զ����ֶ�����
	char achUserDefine[MAX_STRING_LEN+1];
	// �Ƿ�Զ�˴洢
	BOOL bIsRemoteStore;
	// ����������·������ʽ��172.16.3.108:/usr/share)
	char achSvrSharePath[MAX_PATH_LEN + 1];
}PACKED TSNAPSHOT_NAMINGRULE,*PTSNAPSHOT_NAMINGRULE;

//zlq add ͼƬ�ļ���Ϣ
typedef struct TMTIPicFileInfo
{
	u8 byType;			//�¼�����
	u8 bychanId;		//ͨ����
	u8 byVideoResolution;//ͼ��ֱ���
	u8 byFormat;		//ͼƬ��ʽ
	TMTISysTime tTime;	//��ͼʱ��
	u32 dwPicID;		//ͼƬ���
	char achFileName[MAX_FILE_NAME_LENGTH+1];	//�ļ�����
	void SwapToNetwork()
	{
		tTime.SwapToNetwork();
		dwPicID = htonl(dwPicID);
	}

	void SwapFromNetwork()
	{
		tTime.SwapFromNetwork();
		dwPicID = ntohl(dwPicID);
	}
}PACKED TPIC_FILE_INFO,*PTPIC_FILE_INFO;

typedef struct TMTIQueryPicFileInfo
{
	u16 wMaxNum;		//��ǰ��ѯ�ļ�������
	u8 byIndex;			//��ǰ��������,��1��ʼ
	u8 byNum;			//�ļ�����
	u8 byEnd;			//�Ƿ����
	TMTIPicFileInfo aPicFileInfo[MAX_REC_FILE_NUM];	//ͼƬ�ļ��б�
	void SwapToNetwork()
	{
		wMaxNum = htons(wMaxNum);
		for (int i = 0; i < MAX_REC_FILE_NUM; i++)
		{
			aPicFileInfo[i].SwapToNetwork();
		}
	}

	void SwapFromNetwork()
	{
		wMaxNum = ntohs(wMaxNum);
		for (int i = 0; i < MAX_REC_FILE_NUM; i++)
		{
			aPicFileInfo[i].SwapFromNetwork();
		}
	}
}PACKED QUERY_PIC_FILE_INFO,*PQUERY_PIC_FILE_INFO;

typedef struct TMTIQueryPicFileParam
{
	u8 byNum;			//�ļ�����
	u8 byType;			//��ѯ����
	u8 byChanId;		//ͨ����
	u8 byEvent;			//�¼�����
	u16 wBeginNum;			//�ļ���ѯ��ʼ���
	TMTISysTime tTimeStart;	//��ʼץ��ʱ��
	TMTISysTime tTimeEnd;	//����ץ��ʱ��
//	char achFileName[MAX_FILE_NAME_LENGTH+1];	//�ļ�����
	void SwapToNetwork()
	{
		wBeginNum = htons(wBeginNum);
		tTimeStart.SwapToNetwork();
		tTimeEnd.SwapToNetwork();
	}
	void SwapFromNetwork()
	{
		wBeginNum = ntohs(wBeginNum);
		tTimeStart.SwapFromNetwork();
		tTimeEnd.SwapFromNetwork();
	}
}PACKED QUERY_PIC_FILE_PARAM,*PQUERY_PIC_FILE_PARAM;

typedef struct TMTIDownloadPicFileInfo
{
	u16		wResult;			/*�������*/		
	u16		wTotalPacket;		/*�ܰ�����*/
	u16		wCurPacketIdx;	    /*��ǰ����������,��1��ʼ*/
	u16		wCurLen;			/*��ǰ����*/
	u32		dwPicID;			//ͼƬ���
	u8      abyBuf[SMALL_DATA_BUFSIZE];       /*����*/
	void SwapToNetwork()
	{
		wResult = htons(wResult);
		wTotalPacket = htons(wTotalPacket);
		wCurPacketIdx = htons(wCurPacketIdx);
		wCurLen = htons(wCurLen);
		dwPicID = htonl(dwPicID);

	}
	void SwapFromNetwork()
	{
		wResult = ntohs(wResult);
		wTotalPacket = ntohs(wTotalPacket);
		wCurPacketIdx = ntohs(wCurPacketIdx);
		wCurLen = ntohs(wCurLen);
		dwPicID = ntohl(dwPicID);

	}
}PACKED DOWNLOAD_PICFILE_INFO,*PDOWNLOAD_PICFILE_INFO;

//zlq add ͼƬ�������
typedef struct TMTISnapShotResult
{
	u16 wError;		//���
	u32 dwPicID;	//ͼƬID
	void SwapToNetwork()
	{
		wError = htons(wError);
		dwPicID = htonl(dwPicID);
	}
	void SwapFromNetwork()
	{
		wError = ntohs(wError);
		dwPicID = ntohl(dwPicID);
	}
}PACKED SNAP_SHOT_RESULT, *PSNAP_SHOT_RESULT;
#define SNAP_SHOT_RESULT_LEN  sizeof(TMTISnapShotResult)

//zlq add ͼƬ�������
typedef struct TMTISnapShotResultEx
{
	u16 wError;		//���
	u32 dwPicID;	//ͼƬID
	char achPicName[MAX_FILE_NAME_LENGTH + 1]; //�ýṹ������������һ��
	void SwapToNetwork()
	{
		wError = htons(wError);
		dwPicID = htonl(dwPicID);
	}

	void SwapFromNetwork()
	{
		wError = ntohs(wError);
		dwPicID = ntohl(dwPicID);
	}
}PACKED SNAP_SHOT_RESULT_EX, *PSNAP_SHOT_RESULT_EX;

//zlq add �����������
typedef struct TMTIPUNMSCFG
{
	u8	byRegType;								//��������:DNS,PUI
	u32	dwDNSIp;								//DNS��ip��ַ
	u16 wDNSPort;								//DNS�˿�
	u16 wPingCheckTimes;						//����������
	u16	wPingCheckInterval;						//�������ʱ����
	u32	dwVersionServerIp;						//�汾������IP
	u16 wVersionServerPort;						//�汾�������˿�
	u32 dwVersionCheckInterval;					//�汾������� ��λ:����
	s8	achVersionPath[MAX_PATH_LEN+1];			//���ذ汾�ļ���Ϣ
	s8	achVersionUserName[MAX_STRING_LEN+1];	//�汾�������û���
	s8	achVersionUserPW[MAX_STRING_LEN+1];		//�汾����������
	u8	bySlotId;								//�ۺ�
	s8	achBoxInfo[MAX_STRING_LEN+1];			//��λ��Ϣ
	s8	achLocationInfo[MAX_STRING_LEN+1];		//λ����Ϣ
	u16     DownloadStartPort;                  // ������ʼ�˿�
	u16     BrowseStartPort;                    // �����ʼ�˿�
}PACKED PU_NMS_CFG,*PPU_NMS_CFG;

typedef struct TConnectionLostList
{
	u32 dwIpAddr;
	u16 wPort;
	u32 dwCheckTimerId;
	BOOL bEnable;
}PACKED TCONNECTION_LOST_LIST,*PTCONNECTION_LOST_LIST;

//¼���ļ�����֪ͨ
typedef struct TRecFileDownNty
{
	u16		wTransactionNum;	//������ˮ��
	u16		wErrorCode;
	u16		wFtpPort;
}PACKED TRECFILEDOWNNTY;

#ifdef VSIP_MULTI_CHAN_SUPPORT
// ����״̬��ͨ�����ϵ
typedef struct TMultiChanInfo
{
        u8 byChanType;                          //�������Ǹ�����,1��������0������
        u8 bySupportSubNum;             //֧�ָ�����·����������Ϊ0;����ǿ����ڶ�·�����������������š�
        u8 SubChanId[8];				//������ID�б����ݸ�����·����������д
        u8 byStatus;                            //��ǰ�������Ƿ�����1��������0-δ���������������У�����ǿ���˫��������1�����ֹͣ˫������0��
        u8 byParentEncId;                       //����Ϊ0��������Ϊ�����ı���ͨ����

        void SwapToNetwork()
        {
                //dwServerIP = htonl(dwServerIP);
        }
        void SwapFromNetwork()
        {
                //dwServerIP = ntohl(dwServerIP);
        }
        BOOL PackedToBuf( char * Buf, unsigned short BufLen )
        {
                if( PackedLen() > BufLen)
                {
                        return FALSE;
                }
                memcpy( Buf, &byChanType, sizeof(byChanType));
                Buf += sizeof(byChanType);
                memcpy( Buf, &bySupportSubNum, sizeof(bySupportSubNum));
                Buf += sizeof(bySupportSubNum);
                memcpy( Buf, &SubChanId, sizeof(SubChanId));
                Buf += sizeof(SubChanId);				
                memcpy( Buf, &byStatus,sizeof(byStatus));
                Buf += sizeof(byStatus);
                memcpy( Buf, &byParentEncId, sizeof(byParentEncId));
                return TRUE;
        }
        BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
        {
                if( PackedLen() > BufLen)
                {
                        return FALSE;
                }
                memcpy( &byChanType, Buf, sizeof(byChanType));
                Buf += sizeof(byChanType);
                memcpy( &bySupportSubNum, Buf, sizeof(bySupportSubNum));
                Buf += sizeof(bySupportSubNum);
                memcpy( &SubChanId, Buf, sizeof(SubChanId));
                Buf += sizeof(SubChanId);
                memcpy( &byStatus,Buf, sizeof(byStatus));
                Buf += sizeof(byStatus);
                memcpy( &byParentEncId,Buf,sizeof(byParentEncId));
                return TRUE;
        }
        unsigned short PackedLen()
        {
                return (sizeof(byChanType)+sizeof(bySupportSubNum)+sizeof(SubChanId)+sizeof(byStatus)+sizeof(byParentEncId));
        }
}PACKED TMULTICHANINFO, *PTMULTICHANINFO;

// ���������ã��粻�������ã�����Բ��ô˽ṹ
typedef struct TDevMultiChanInfo
{
        u8 byTotalNum;                          //��Ƶ����ͨ������
        TMultiChanInfo tMultiChanInfo[MAX_VIDEO_ENC_NUM];       //����״̬��ͨ�����ϵ��
}PACKED TDEVMULTICHANINFO, *PTDEVMULTICHANINFO;

#endif

//cu͸��ͨ����Ϣ�ṹ��
typedef struct TMTICuOpertionMsg
{
private:
	u16    m_wBufLen;     //͸�����ݽṹ����
	u8     m_abyTransDataBuf[MTI_MAX_CU_OPERTION_NUM];   //͸�����ݽṹ
	
	void SetBufLen(u16 wBufLen) 
	{
		m_wBufLen = htons(wBufLen);
	}
public:
	u16 GetBufLen() const
	{
		return ntohs(m_wBufLen);
	}
	
    void SetData(const u8 *pbyData, u16 wBufLen)
    {
        if(pbyData == NULL)
		{
			return;
		}
		
        memset(m_abyTransDataBuf, 0, sizeof(m_abyTransDataBuf));
        wBufLen = wBufLen > MTI_MAX_CU_OPERTION_NUM ? MTI_MAX_CU_OPERTION_NUM : wBufLen;
        memcpy(m_abyTransDataBuf, pbyData, wBufLen);
        SetBufLen(wBufLen);
    }
    const u8* GetData() const
    {
        return m_abyTransDataBuf;
    }
	
	u16 PackedLen() const
	{
		return sizeof(m_wBufLen)+GetBufLen();
	}
	
} PACKED TMTICuOpertionMsg, *PTMTICuOpertionMsg;

typedef struct TGpsStartPlayInfo
{
	u32  dwBeginTime; //��ʼʱ��,��λ(�룩 
	u32  dwEndTime;   //����ʱ�䣬��λ(��) 
	u16  wPlayRate;   //��������,��λ(��) 
    u32  dwReserve1;  //�����ֶ� 
	
	void SwapToNetwork()
	{
		dwBeginTime = htonl(dwBeginTime);
		dwEndTime = htonl(dwEndTime);
		wPlayRate = htons(wPlayRate);
		dwReserve1 = htonl(dwReserve1);
	}
	void SwapFromNetwork()
	{
		dwBeginTime = ntohl(dwBeginTime);
		dwEndTime = ntohl(dwEndTime);
		wPlayRate = ntohs(wPlayRate);
		dwReserve1 = ntohl(dwReserve1);
	}
	
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwBeginTime, sizeof(dwBeginTime));
		Buf += sizeof(dwBeginTime);
		memcpy( Buf, &dwEndTime, sizeof(dwEndTime));
		Buf += sizeof(dwEndTime);
		memcpy( Buf, &wPlayRate,sizeof(wPlayRate));
		Buf += sizeof(wPlayRate);
		memcpy( Buf,&dwReserve1,sizeof(dwReserve1));
		Buf += sizeof(dwReserve1);
		
		return TRUE;
	}
	
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwBeginTime, Buf, sizeof(dwBeginTime));
		Buf += sizeof(dwBeginTime);
		memcpy( &dwEndTime, Buf, sizeof(dwEndTime));
		Buf += sizeof(dwEndTime);
		memcpy( &wPlayRate,Buf, sizeof(wPlayRate));
		Buf += sizeof(wPlayRate);
		memcpy( &dwReserve1,Buf,sizeof(dwReserve1));
		Buf += sizeof(dwReserve1);
		return TRUE;
	}
	
	unsigned short PackedLen()
	{
		return (sizeof(dwBeginTime)+sizeof(dwEndTime)+sizeof(wPlayRate)+sizeof(dwReserve1));
	}
	
}PACKED TGPS_START_PLAY_INFO,*PTGPS_START_PLAY_INFO;

typedef struct TGpsStartPlayRsp
{
public:
	u32 dwErrorCode;
	u8  byPlayId;
	u32 dwReserve;
}PACKED TGPS_START_PLAY_RSP,*PTGPS_START_PLAY_RSP;

typedef struct TPlayGpsNotify
{
	u8 byPlayerId;                              //������id 
	u8 byIsFinished;                            //�Ƿ񲥷Ž��� 
	u32 dwErrorCode;                            //���Ž����Ĵ����� 
	u16 wNum;                                   //��������GpsData�������wNum��GpsData���� 
	u32 dwRecId;								//¼���ļ���Ψһ��ʶ
    u32 dwReserve1;                             //�����ֶ�
	u8  achGpsData[DATA_BUFSIZE];				//Gps���ݲ���

	void SwapFromNetwork()
	{
		dwErrorCode = ntohl(dwErrorCode);
		wNum = ntohs(wNum);
		dwReserve1 = ntohl(dwReserve1);
		dwRecId = ntohl(dwRecId);
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy(&byPlayerId,Buf,sizeof(byPlayerId));
		Buf += 1;
		memcpy(&byIsFinished,Buf,sizeof(byIsFinished));
		Buf += 1;
		memcpy( &dwErrorCode, Buf, sizeof(dwErrorCode));
		Buf += sizeof(dwErrorCode);
		memcpy( &wNum,Buf, sizeof(wNum));
		Buf += sizeof(wNum);
		memcpy( &dwRecId, Buf, sizeof(dwRecId) );
		Buf += sizeof(dwRecId);
		memcpy( &dwReserve1,Buf,sizeof(dwReserve1));
		Buf += sizeof(dwReserve1);
		return TRUE;
	}
	
	unsigned short PackedLen()
	{
		return (sizeof(byPlayerId)+sizeof(byIsFinished)+sizeof(dwErrorCode)+sizeof(wNum)+sizeof(dwRecId)+sizeof(dwReserve1));
	}

}PACKED TPLAY_GPS_NOTIFY,*PTPLAY_GPS_NOTIFY;

typedef struct TTransSubsBase
{
private:
	u32 m_dwSubscribeType;
	u32 m_dwUserDefine;
	u32 m_dwReserved;
	u16 m_wBufLen;     //͸�����ݽṹ����
	u8  m_abyTransDataBuf[DATA_BUFSIZE];   //͸�����ݽṹ //DATA_BUFSIZE vsip�����Ϣ����
public:

	void SwapToNetwork()
	{
		m_dwSubscribeType = htonl(m_dwSubscribeType);
		m_dwUserDefine = htonl(m_dwUserDefine);
		m_dwReserved = htonl(m_dwReserved);
		m_wBufLen = htons(m_wBufLen);
	}
	void SwapFromNetwork()
	{
		m_dwSubscribeType = ntohl(m_dwSubscribeType);
		m_dwUserDefine = ntohl(m_dwUserDefine);
		m_dwReserved = ntohl(m_dwReserved);
		m_wBufLen = ntohs(m_wBufLen);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( Buf, &m_dwSubscribeType, sizeof(m_dwSubscribeType) );
		Buf += sizeof(m_dwSubscribeType);
		memcpy( Buf, &m_dwUserDefine, sizeof(m_dwUserDefine) );
		Buf += sizeof(m_dwUserDefine);
		memcpy( Buf, &m_dwReserved, sizeof(m_dwReserved) );
		Buf += sizeof(m_dwReserved);	
		memcpy( Buf, &m_wBufLen, sizeof(m_wBufLen) );
		Buf += sizeof(m_wBufLen);

		memcpy( Buf, m_abyTransDataBuf, ntohs(m_wBufLen) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{

		memcpy( &m_dwSubscribeType, Buf, sizeof(m_dwSubscribeType) );
		Buf += sizeof(m_dwSubscribeType);
		memcpy( &m_dwUserDefine, Buf, sizeof(m_dwUserDefine) );
		Buf += sizeof(m_dwUserDefine);
		memcpy( &m_dwReserved, Buf, sizeof(m_dwReserved) );
		Buf += sizeof(m_dwReserved);
		memcpy( &m_wBufLen, Buf, sizeof(m_wBufLen) );
		Buf += sizeof(m_wBufLen);

		if( sizeof(m_abyTransDataBuf) < ntohs(m_wBufLen))
		{
			return FALSE;
		}

		memcpy( m_abyTransDataBuf, Buf, ntohs(m_wBufLen) );

		return TRUE;
	}



	u16 GetBufLen() const
	{
		return ntohs(m_wBufLen);
	}


	void SetBufLen(u16 wBufLen) 
	{
		m_wBufLen = htons(wBufLen);
	}

	u32 GetSubscribeType() const
	{

		return ntohl(m_dwSubscribeType);
	}

	void SetSubscribeType(u32 dwSubscribeType)
	{
		m_dwSubscribeType = htonl(dwSubscribeType);
	}

	void SetUserDefine(u32 dwUserDefine)
	{
		m_dwUserDefine = htonl(dwUserDefine);
	}

	u32 GetUserDefine() const
	{

		return ntohl(m_dwUserDefine);
	}

	void SetReserved(u32 dwReserved)
	{
		m_dwReserved = htonl(dwReserved);
	}

	u32 GetReserved() const
	{

		return ntohl(m_dwReserved);
	}


	void SetData(const u8 *pbyData, u16 wBufLen)
	{
		if(pbyData == NULL)
		{
			return;
		}

		memset(m_abyTransDataBuf, 0, sizeof(m_abyTransDataBuf));
		wBufLen = wBufLen > DATA_BUFSIZE ? DATA_BUFSIZE : wBufLen;
		memcpy(m_abyTransDataBuf, pbyData, wBufLen);
		SetBufLen(wBufLen);
	}
	const u8* GetData() const
	{
		return m_abyTransDataBuf;
	}

	u16 PackedLen() const
	{
		return sizeof(m_dwSubscribeType)+sizeof(m_dwUserDefine)+sizeof(m_dwReserved)+sizeof(m_wBufLen)+GetBufLen();
	}

	u16 StructLen() const
	{
		return sizeof(m_dwSubscribeType)+sizeof(m_dwUserDefine)+sizeof(m_dwReserved)+sizeof(m_wBufLen)+sizeof(m_abyTransDataBuf);
	}

} PACKED TTransSubsBase, *PTTransSubsBase;

typedef struct TTransSubsReq:public TTransSubsBase
{
	void SetExpire(u32 dwUserDefine)
	{
		SetUserDefine(dwUserDefine);
	}

	u32 GetExpire() const
	{

		return GetUserDefine();
	}

}PACKED TTransSubsReq, *PTTransSubsReq;

typedef struct TTransSubsRsp:public TTransSubsBase
{
	void SetErrorResult(u32 dwUserDefine)
	{
		SetUserDefine(dwUserDefine);
	}

	u32 GetErrorResult() const
	{

		return GetUserDefine();
	}

}PACKED TTransSubsRsp, *PTTransSubsRsp;

typedef struct TTransSubsNtf:public TTransSubsBase
{
	void SetSequenceNumber(u32 dwUserDefine)
	{
		SetUserDefine(dwUserDefine);
	}

	u32 GetSequenceNumber() const
	{

		return GetUserDefine();
	}

}PACKED TTransSubsNtf, *PTTransSubsNtf;

typedef struct TMtiDecStyleParam
{
	u16 wOutputId;
	u16 wDecStyle;
}PACKED TMTI_DEC_STYLE_PARAM,*PTMTI_DEC_STYLE_PARAM;

typedef struct TMtiCustomExtCapSet
{
	u8 byCapNum;
	u8 abyCapItem[MAX_CUSTOMCAP_NUM];
}PACKED TMTI_CUSTOM_EXT_CAP_SET,*PTMTI_CUSTOM_EXT_CAP_SET;

typedef struct TMtiGpsData 
{ 
	u32 dwTime;  //��GpsData��ʱ�� 
	char cNSIndicator; //�ϱ�γ 
	char cEWIndicator; //������ 
	u8   byLatitudeHigh; //γ�ȸ�λֵ 
	u32  dwLatitudeLow; //γ�ȵ�λֵ 
	u8   byLongitudeHigh;    //���ȸ�λֵ 
	u32  dwLongitudeLow;   //���ȵ�λֵ 
	u16  wSpeedHigh;       //�ٶȸ�λֵ 
	u16  wSpeedLow;        //�ٶȵ�λֵ

	void SwapFromNetwork()
	{
		dwTime = ntohl(dwTime);
		dwLatitudeLow = ntohl(dwLatitudeLow);
		dwLongitudeLow = ntohl(dwLongitudeLow);
		wSpeedHigh = ntohs(wSpeedHigh);
		wSpeedLow = ntohs(wSpeedLow);
	}
	
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy(&dwTime,Buf,sizeof(dwTime));
		Buf += sizeof(dwTime);
		memcpy(&cNSIndicator,Buf,sizeof(cNSIndicator));
		Buf += 1;
		memcpy(&cEWIndicator,Buf,sizeof(cEWIndicator));
		Buf += 1;
		memcpy(&byLatitudeHigh,Buf,sizeof(byLatitudeHigh));
		Buf += 1;
		memcpy( &dwLatitudeLow, Buf, sizeof(dwLatitudeLow));
		Buf += sizeof(dwLatitudeLow);
		memcpy(&byLongitudeHigh,Buf,sizeof(byLongitudeHigh));
		Buf += 1;
		memcpy( &dwLongitudeLow,Buf, sizeof(dwLongitudeLow));
		Buf += sizeof(dwLongitudeLow);
		memcpy( &wSpeedHigh,Buf,sizeof(wSpeedHigh));
		Buf += sizeof(wSpeedHigh);
		memcpy( &wSpeedLow,Buf,sizeof(wSpeedLow));
		Buf += sizeof(wSpeedLow);
		return TRUE;
	}
	
	unsigned short PackedLen()
	{
		return (sizeof(dwTime)+sizeof(cNSIndicator)+sizeof(cEWIndicator)+sizeof(byLatitudeHigh)+sizeof(dwLatitudeLow)+
			sizeof(byLongitudeHigh)+sizeof(dwLongitudeLow)+sizeof(wSpeedHigh)+sizeof(wSpeedLow));
	}

}PACKED TMTI_GPSDATA,*PTMTIGPSDATA;

//bmpserial��Ϣ
typedef struct TBmpSerialInfo
{
    u8      byVideoPort;          /*��ƵԴ�˿�*/
    u8      byType;               /*0:̨��,1:�澯��Ļ 2:�澯��ʾ 4:����ͼƬ*/
    u16     wWidth;              //ͼ����
    u16     wHeigth;             //ͼ��߶�
    u16     wTotalPacket;         /*�ܰ�����*/
    u16     wCurPacketIdx;        /*��ǰ����������,��1��ʼ*/
    u16     wCurLen;              /*��ǰ����*/
    u8      abyBuf[MAX_BMPINFOBUF_LEN];       /*����*/
    u8      byFontNum;            //�ַ�����
    u8      byFontHeight;         //�ַ���
    u8      abyFontWidth[18];     //�ַ����
    u16     wLeftMargin; //������Ļ��߾� 1-200;
    u16     wTopMargin; //������Ļ�ϱ߾� 1-200;
}PACKED TMT_BMPSERIAL_INFO, *PTMT_BMPSERIAL_INFO;


//����ϱ���ǰ�˵���������Ŀǰ���ϱ��Ƿ�֧�ֲ�����Э��
typedef enum EnumVsipBackendEntity
{
	emVideoClockRateNego            = 1,    // MTI_ENABLE=֧�ֲ�����Э�̣�MTI_DISABLE=��֧�ֲ�����Э�� 
} EVsipBackendEntity;

typedef struct TMTIBackendCap
{
	u16 wEntityId;                  // EVsipBackendEntity
	u32 dwEntityValue;
}PACKED TMTI_BACKEND_CAP,*PTMTI_BACKEND_CAP;

typedef struct TMTIBackendCapSets
{
	u16 wCapNum;                         
	TMTIBackendCap EntityValue[MAX_BACKENDCAP_NUM];
}PACKED TMTI_BACKEND_CAPSETS,*PTMTI_BACKEND_CAPSETS;

#ifdef _LINUX_ARM_D6446_	//zhuzhm modify 070917
typedef void (*pDISCOVERY_CB)( TMTIDiscoveryCBInfo *atDiscvInfo, u16 wUnitNum, void* pContext );
#else
typedef void (*pDISCOVERY_CB)( TMTIDiscoveryCBInfo atDiscvInfo[], u16 wUnitNum, void* pContext );
#endif
typedef void (*pMESSAGE_CB)( TMTIMsgHandle tMsgHandle, u8 * pbyMsgBuf, u16 wBufLen, void* pContext );
typedef void (*pCONNECTED_CB)( u32 dwPuiIp, TVsipGuid tDevGuid, u32 dwUnitIpAddr, u16 wPort, void* pContext );
typedef void (*pCONNECTIONCHECK_CB)( TVsipGuid tDevGuid,  void* pContext);
typedef void (*pGetGuidByIp_CB)(TVsipGuid tDevGuid,u8 byLocalSerialId,u8 byTargetSerialId,void* pContext);
typedef void (*pTERMINATED_CB)(void);

typedef void (*pNEWDISCOVERY_CB)(TMTIDiscoveryCBInfo atDiscvInfo, void* pContext);

//ǰ���豸���� add G.J.20090903
enum
{
    DEVICE_PU_KDM = 0,      //��ͨPU�豸
    DEVICE_PU_NVR,          //NVR�豸
	DEVICE_PU_THIRD,        //�⳧���豸
	DEVICE_PU_NVR128,       //nvr128
    DEVICE_PU_3G,           //�����豸
    DEVICE_PU_UNKNOWN,      //δ֪�豸
	DEVICE_PU_G100,
};
//addend

//GPS͸�����ݽṹ
struct TMTISubscribeTransData
{
    u16 m_wType;                   //͸����������
    u32 m_dwSubType;                //͸������������
    
    void SwapToNetwork()
    {
        m_wType = htons(m_wType);
        m_dwSubType = htonl(m_dwSubType);
    }
    
    void SwapFromNetwork()
    {
        m_wType = ntohs(m_wType);
        m_dwSubType = ntohl(m_dwSubType);
    }
}PACKED;

class CMTIUnitCtrl
{
public:
	CMTIUnitCtrl();
	~CMTIUnitCtrl();

	u16 SendUserOpertionMsg(u8 byChnId,char* pbyBuf,u16 wBufLen,BOOL32 bUseDevGuid = FALSE);//�û�͸��ͨ��

	u16 GpsDataStartPlay(TGpsStartPlayInfo tPlayParam,u8 byVideoEncID);//��ʼ����Gps����
	u16 GpsDataStopPlay(u8 byPlayId);//ֹͣ����Gps����

	u16 SendTransSubs(TTransSubsReq & tTransSubsReq);
	u16 SendTransSubsCancle(u16 wTransactionNum, TTransSubsReq & tTransSubsReq);

	u16 SetDecoderStyleParam(TMtiDecStyleParam tSetDecStyleParam);
	u16 GetDecoderStyleParam(TMtiDecStyleParam& tDecStyleParam);

	u16 SetBmpSerialData(const TMT_BMPSERIAL_INFO &tBmpData);

	u16 SetVidEncOsdCfgEx(u16 wDataLen, char *achBuf);
	u16 GetVidEncOsdCfgEx(u16 wDataLen, char *achBuf);
	u16 SetVidSrcOsdCfgEx(u16 wDataLen, char *achBuf);
	u16 GetVidSrcOsdCfgEx(u16 wDataLen, char *achBuf);

    //GPS���
    u16 SetTransReq(u16 wType, const char* pBuf, u16 wBufLen);
    u16 GetTransReq(u16 wType);
    u16 SubscribeTransReq(u8 byOpetype, TMTISubscribeTransData tTans);
    u16 SubscribeTransReq(u8 byOpetype, const char* pBuf, u16 wBufLen);
    u16 InquiryTransReq(TMTISubscribeTransData tTans);
	u16 InquiryTransReq(const char* pBuf, u16 wBufLen);

    u16 FileTransData(u8 byOsdType,u16 wLen,char *abyBuf);
    u16 StartFileTransReq(u8 byOsdType);
    u16 StopFileTransReq(u8 byOsdType);
    u16 StartSmartSightCamera(u8 bySerialID,char *achBuf,u16 wLen);
    void SetConnectStatus(VSIP_HANDLE VsipHandle, eConnState byStatus);
	void SetConnectPui(VSIP_HANDLE VsipHandle, u8 *abyPuid);
    eConnState GetConnectStatus(VSIP_HANDLE VsipHandle);
	void PrintUnitInfo();


	//hzc_Transparent_Chan_20110324{{
	u16 SendUserTcpMsgEx(u8 byVideoEncID,char *pbyBuf, u16 wpbyBufLen);
	//}}

	//����͸������
	u16 SendUserTcpMsgEx2(u8 byVideeInID,u16 wMsgType,char *pbyBuf, u16 wpbyBufLen);

	/*==================================================================
	����        : ��չ����Ƶ��������������(������Ƶ����Ƶ������)
	�������˵��: ��չý�������Ϣ�ṹ
	              s8* pName=NULL -- ���������볧������Ĭ��ΪNULL
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	==================================================================*/
    u16 StartDecoderRecvEx(TMTIMediaCtrlEX tCfgParam, TMTIStreamAttribute tStreamAttri, s8* pName=NULL);
	/*==================================================================
	����        : ���ø澯����(�������ţ�
	�������˵��: �澯����
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	==================================================================*/
	u16 SetAlarmParam(TMTIAlarmParam tCfgParam);
	/*==================================================================
	����        : ���ú������(�������ţ�
	�������˵��: �������
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	==================================================================*/
	u16 SetBannerParam(TMTIBannerParam tCfgParam);
	/*==================================================================
	����        : ��ȡ�澯����(�������ţ�
	�������˵��: �澯����
	����ֵ˵��  : �ɹ�����MTI_SUCCESS��ʧ�ܷ���MTI_FAIL
	==================================================================*/
    u16 GetAlarmParam(TMTIAlarmParam &tCfgParam);
	/*==================================================================
	����        : ��ȡ�������(�������ţ�
	�������˵��: �������
	����ֵ˵��  : �ɹ�����MTI_SUCCESS��ʧ�ܷ���MTI_FAIL
	==================================================================*/
    u16 GetBannerParam(TMTIBannerParam &tCfgParam);
	/*======================ǰ��¼��ҵ��ӿ�start==========================*/

	/*==================================================================
	����        : ��ӡ¼����Ϣ
	�������˵��: ͨ��id��
	����ֵ˵��  : ��
	==================================================================*/
    void PrintRecInfo(u8 byChannel);

	/*==================================================================
	����        : ָ��ĳ���ն˿�ʼ¼��
	�������˵��: ͨ��id��
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
    u16 StartRec(u8 byChannel);
	/*==================================================================
	����        : ָ��ĳ���ն�ֹͣ¼��
	�������˵��: ͨ��id��
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
    u16 StopRec(u8 byChannel);
   /*==================================================================
	����        : ����ĳ���ն˶�ʱ¼�����
	�������˵��: ͨ��id��
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
	u16 SetTimelyRecInfo(u8 byChannel, TimelyRecInfo TRecInfo);
	/*==================================================================
	����        : ��ȡĳ���ն˶�ʱ¼�����
	�������˵��: ͨ��id��,��ʱ¼�����
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetTimelyRecInfo(u8 byChannel, TimelyRecInfo & TRecInfo);
   /*==================================================================
	����        : ����ĳ�ն�����¼�����
	�������˵��: ͨ��id��,�������ͣ�����ʱ����Ϣ
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
	u16 SetPeriodlyRecInfo (u8 byChannel, PeriodlyTimeInfo PTimeInfo);
	/*==================================================================
	����        : ��ȡĳ�ն�����¼�����
	�������˵��: ͨ��id��,����ʱ����Ϣ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetPeriodlyRecInfo (u8 byChannel,PeriodlyTimeInfo & PTimeInfo);
	/*==================================================================
	����        : ����ͨ��¼���������
	�������˵��: ͨ��id��,¼�������Ϣ
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
	u16 SetChannelRecBasicInfo(u8 byChannel, RecChannelBasicInfo RChannelBasicInfo);
	/*==================================================================
	����        : ��ȡͨ��¼���������
	�������˵��: ͨ��id��,¼�������Ϣ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetChannelRecBasicInfo(u8 byChannel, RecChannelBasicInfo & RChannelBasicInfo);

	/*==================================================================
	����        : ����¼��ϵͳ����
	�������˵��: ¼��ϵͳ��Ϣ
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
	u16 SetRecSysInfo(RecSysInfo RSysInfo);

	/*==================================================================
	����        : ��ȡ¼��ϵͳ����
	�������˵��: ¼��ϵͳ��Ϣ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetRecSysInfo(RecSysInfo & RSysInfo);
	/*==================================================================
	����        : ��ѯͨ����¼���ļ�
	�������˵��: ��ѯ��Ϣ
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	==================================================================*/
	u16 QueryRecFile(TQueryParam QueryParam);
	/*==================================================================
	����        : ��ѯ����������¼���ļ��ĸ���
	�������˵��: ��ѯ��Ϣ
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	==================================================================*/
	u16 QueryRecFileNum(TQueryParam QueryParam);
	/*==================================================================
	����        : ¼���ļ�����
	�������˵��: (�ļ����¼�)id�ţ�ip��ַ���˿ں�,���ͼ�MTI_QUIEY_ALL_EVENT
	����ֵ˵��  :  �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
    ����˵����    �������֪ͨ
	==================================================================*/
	u16 RecFileStartPlay(TRecStartPlayInfo tCfgParam);
	/*==================================================================
	����        : ¼���ļ�����
	�������˵��: ������id���������ͼ�MTI_OPERATION_NORMAL
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
	u16 RecFileOperation(u8 byPlayerId, u8 byOperationType);
	/*==================================================================
	����        : ¼���ļ�����λ
	�������˵��: ������id������ʱ��
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
    ����˵����    �������֪ͨ
	==================================================================*/
	u16 RecSeekLocation(u8 byPlayerId,u32 dwDragTime);
	/*==================================================================
	����        : ɾ��¼���ļ����¼��������������жϣ�
                  ��Ϊ�ļ���dwIndexIdΪ�ļ�id
				  ��Ϊ�¼���dwIndexIdΪ�¼�id
	�������˵��: id�ţ����ͼ�MTI_QUIEY_ALL_EVENT
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
	u16 DelRecFile(u32 dwIndexId,u8 byType);
    /*==================================================================
	����        : ɾ������¼���¼(�¼�)��澯¼���ֶ�¼��
	              ��MTI_QUIEY_ALL_EVENT
	�������˵��: �ļ�id��
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
    u16 DelRecRecord(u8 byType);

	/*==================================================================
	����        : ��ȡ¼����״̬
	�������˵��: ͨ��id�ţ�¼����״̬
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	==================================================================*/
	u16 GetRecStatus(u8 byChannelId, RecPlayerState & byStatus);
	/*==================================================================
	����        : ����¼���ļ�
	�������˵��: �ļ�id��(MTI_CONNECTION_UDP,MTI_CONNECTION_TCP)
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
	u16 StartExportRecFile(u32 dwRecFileIndexId,u32 dwIpAddress,u16 wPort,u8 byConnectionType = MTI_CONNECTION_TCP);
	/*==================================================================
	����        : ֹͣ����¼���ļ�
	�������˵��: ��
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    �������֪ͨ
	==================================================================*/
	u16 StopExportRecFile();
	/*==================================================================
	����        : �������ܲ���
	�������˵��: ���ܲ���
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	����˵����    ״̬����֪ͨ
	==================================================================*/
    u16 SetNmsParam(TNmsParam NmsParam);
	/*==================================================================
	����        : ��ȡ���ܲ���
	�������˵��: ���ܲ���
	����ֵ˵��  : �ɹ����ز�����ˮ�ţ�ʧ�ܷ���MTI_FAIL
	==================================================================*/
	u16 GetNmsParam(TNmsParam & NmsParam);
	/*======================ǰ��¼��ҵ��ӿ�end============================*/

   /*==================================================================
	����        : ��ӡʵ����Ϣ
	�������˵��:
	����ֵ˵��  :
	==================================================================*/
    void PrintEntityInfo();
	/*==================================================================
	����        : ��ӡ��������Ϣ
	�������˵��:
	����ֵ˵��  :
	==================================================================*/
    void PrintCapability();
	void PrintInfo();
	void PrintExtCapability();
	void PrintBasicParamInfo();
//#ifndef _LINUXEQUATOR_
	/*==================================================================
	����        : ��ʼ���豸����
	�������˵��: const TMTIInitInfo & tUnitInfo �豸��ʼ����
				  u8 byConnectionType ��Ϣ��·����
				  u8 byIdentify �Ƿ���֤����
				  (����֤����TMTIInitInfo�Ľṹ���ֶ���д�û��������ֶΣ�

	����ֵ˵��  : �ɹ����� TRUE; ʧ�ܷ��� FALSE
	==================================================================*/
	BOOL Initialize( const TMTIInitInfo & tUnitInfo, u32 &dwConIP, u8 byConnectionType = MTI_CONNECTION_TCP,u8 byIdentify = 0);
	//zlq add
	BOOL InitializeEx( const TMTIInitInfo & tUnitInfo, u32 &dwConIP, u8 byConnectionType = MTI_CONNECTION_TCP,u8 byIdentify = 0);
	u16 SetBmpOsdData(TMT_BMPOSD_INFO &tBmpData);
	u16 QueryPicFile(TMTIQueryPicFileParam QueryParam);
	u16 QueryPicFileNum(TMTIQueryPicFileParam QueryParam);
	u16 DownLoadPicFile(TMTIPicFileInfo tPicInfo, u16 wIndex, u8 byHighSpeed  = 1);
	u16 DelPicFile(TMTIQueryPicFileInfo tPicFileInfo);//zlq add ɾ��ͼƬ�ļ� ��������ɾ
	u16 DelPicFileEx(TMTIPicFileInfo tPicFileInfo);//zlq add 07-01-27 //ɾ�������ļ�
	u16 SetSnapShotConfig(u8 byChannel, TMTISnapShotConfig tConfig);//ץͼ����
	u16 GetSnapShotConfig(u8 byChannel, TMTISnapShotConfig &tConfig);
	u16 SetSnapShotNameRule(const TMTISnapShotNamingRule &tConfig);
	u16 GetSnapShotNameRule(TMTISnapShotNamingRule &tConfig);
	u16 StartSnapShot(u8 byChannel);
	u16 StopSnapShot(u8 byChannel);
	u16 SetMatrixParam(const TMTIMatrixInfo &tConfig);
    u16 GetMatrixParam(TMTIMatrixInfo &tConfig);  //mtcͬ��,pui�첽
	u16 SetPuNmsParam(TMTIPUNMSCFG tConfig);
	u16 GetPuNmsParam(TMTIPUNMSCFG &tConfig);//mtcʹ��,ͬ����ʽ
	u16 SendUserTcpMSg(u8 byType, char *pbyBuf, u16 wpbyBufLen);
	u16 SendUserUdpMSg(u32 dwDstIpaddr, u16 wDstPort, u8 byType, char *pbyBuf, u16 wpbyBufLen);
//#endif
	u16 ChangePassword( const TMTIUserPassword &tUserInfo);
	u16 SetVideoDynVal(u8 byChanId,u8 byVidFmt, u8 byDynVal);
	//add end
	/*==================================================================
	����        : ������Ϣ�ص�����
	�������˵��: pMESSAGE_CB pFunc �ص�����ָ��
				  void* pContext �ص�����������
	����ֵ˵��  : �ɹ����� TRUE; ʧ�ܷ��� FALSE
	==================================================================*/
	BOOL SetMessageCallBack( pMESSAGE_CB pFunc, void* pContext = NULL );

	/*==================================================================
	����        : ��ȡ�豸������
	�������˵��: TMTICapability & tCapability �����õ��������ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetCapabilities( TMTICapability & tCapability);
	u16 GetExtCapabilities(TMTICAPSET & tCapability);
	u16 GetCustomExtCapabilities();
	u16 GetAdpCapabilities(TMTICAPSET & tCapability,const char* achMtName);
	/*==================================================================
	����        : ��ȡ�豸ϵͳ����
	�������˵��: TMTISysParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetSysCfg( TMTISysParam & tCfgParam);

	u16 GetSysCfgEx( TMTISysParam & tCfgParam);

	/*==================================================================
	����        : ��ȡPPPOE����
	�������˵��: TMTIPPPOEParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetPPPOECfg( TMTIPPPOEParam & tCfgParam);

	/*==================================================================
	����        : ��ȡϵͳ��������
	�������˵��: TMTISysPubParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetSysPubCfg( TMTISysPubParam & tCfgParam);

	/*==================================================================
	����        : ��ȡ��Ƶ�����������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTICommonNetParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoEncNetCfg( u16 wVideoEncID, TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ��ȡ��Ƶ�����������
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ����(��0��ʼ)
				  TMTICommonNetParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoDecNetCfg( u16 wVideoDecID, TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ��ȡ��Ƶ�����������
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTICommonNetParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetAudioEncNetCfg( u16 wAudioEncID, TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ��ȡ��Ƶ�����������
	�������˵��: u8 byAudioDecID ��Ƶ����ͨ����(��0��ʼ)
				  TMTICommonNetParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetAudioDecNetCfg( u16 wAudioDecID, TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ��ȡ��Ƶ�������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIVideoEncParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoEncCfg( u8 byVideoEncID, TMTIVideoEncParam & tCfgParam, bool bSupportAdvancedEncParam = true );
	//{{{{{{{{{{{{{{{ add by Royan 20140328 �ֱ�����ֵ��
	u16 GetVideoEncCfgEx( u8 byVideoEncID, TMTIVideoEncParam & tCfgParam );
	//}}}}}}}}}}}}}}} add by Royan 20140328 �ֱ�����ֵ��

	/*==================================================================
	����        : ��ȡ��Ƶ���빫������
	�������˵��: TMTIVideoEncPubParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoEncPubCfg( TMTIVideoEncPubParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ��Ƶ������Ļ����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIVideoEncOSDParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoEncOSDCfg( u16 wVideoEncID, TMTIVideoEncOSDParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ��Ƶ���뻭��ϳɲ���
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIVideoEncMPParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoEncMPCfg( u16 byVideoEncID, TMTIVideoEncMPParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ��Ƶ������ƵԴ�˿ڲ���
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIVideoEncInputPortParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoEncInputPortCfg( u16 wVideoEncID, TMTIVideoEncInputPortParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ��Ƶ������ѯ����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIVideoEncPollParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoEncPollCfg( u16 wVideoEncID, TMTIVideoEncPollParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ��Ƶ�������
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIVideoDecParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoDecCfg( u16 wVideoDecID, TMTIVideoDecParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ��Ƶ���빫������
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIVideoDecPubParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoDecPubCfg( u16 wVideoDecID, TMTIVideoDecPubParam & tCfgParam, BOOL32 bExtend = FALSE );

	/*==================================================================
	����        : ��ȡ��Ƶ�����ش�����
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIVideoDecRetransferParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoDecRetransferCfg( u16 wVideoDecID, TMTIVideoDecRetransferParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ��Ƶ������� �첽�ӿ�
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIAudioEncParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� ��ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetAudioEncCfg( u16 wAudioEncID, TMTIAudioEncParam & tCfgParam ); 

	/*==================================================================
	����        : ��ȡ��Ƶ�������
	�������˵��: u8 byAudioDecID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIAudioDecParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetAudioDecCfg( u16 wAudioDecID, TMTIAudioDecParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ���ڲ���
	�������˵��: u8 bySerialPortID ���ں�(��0��ʼ)
				  TMTISerialPortParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetSerialPortCfg( u16 wSerialPortID, TMTISerialPortParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ���ڹ�������
	�������˵��: u8 bySerialPortID ���ں�(��0��ʼ)
				  TMTISerialPortPubParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetSerialPortPubCfg( u16 wSerialPortID, TMTISerialPortPubParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ�����������
	�������˵��: u8 bySerialPortID ��������˿ں�(��0��ʼ)
				  TMTIInputPinParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetInputPinCfg( u16 wInputPinID, TMTIInputPinParam & tCfgParam );

	/*==================================================================
	����        : ��ȡ��������ͨ��ʹ��״̬��Ϣ
	�������˵��: TMTIInputPinParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetInputPinEnableStateCfg(TMTIInputPinState& tInputPinState); //20091020

	/*==========================================================================
	����        : ���ò�������ʹ��״̬
	�������˵��: TMTIInputPinState& tEnableState �����õĲ����ṹ
	����ֵ˵��  : void
	==========================================================================*/
	void SetInputPinEnableState(const TMTIInputPinState& tEnableState );

	/*==================================================================
	����        : ��ȡ��ƵԴ�˿ڲ���
	�������˵��: u8 byVideoInputID ��ƵԴ�˿ں�(��0��ʼ)
				  TMTIVideoInputParam & tCfgParam �����õĲ����ṹ
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetVideoInputCfg( u16 wVideoInputID, TMTIVideoInputParam & tCfgParam, u32 dwTimeOut = 1000, bool bSendToPu = true, bool bPuReg = true);

	/*==================================================================
	����        : ��ȡͼ�����β���
	�������˵��: u16 wEntityID ��ƵԴ�˿ں�
				  TMTIShadeParam *ptCfgParam �ƶ�ͼ�����β���
				  u16 & wDetAreaNum ����ͼ������������
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
    u16 GetShadeCfg(u16 wEntityID, TMTIShadeParam *ptCfgParam, u16 & wShadeAreaNum );

	/*==================================================================
	����        : ��ȡ�ƶ�������
	�������˵��: u8 byType �ƶ��������(MTI_ENCODER-����;MTI_DECODER-����)
				  u16 wEntityID �ƶ����ʵ��ID(����Ϊ��ƵԴ�˿ں�, ����Ϊ������ͨ����)
				  TMTIMotionDetectParamNew *ptCfgParam �ƶ�����������(����Ϊʮ����Ԫ������)
				  u16 & wDetAreaNum �����ƶ����������
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetMotionDetectCfg( u8 byType, u16 wEntityID, TMTIMotionDetectParam *ptCfgParam, u16 &wDetAreaNum );

	/*==================================================================
	����        : �����豸ϵͳ����
	�������˵��: const TMTISysParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetSysCfg( const TMTISysParam & tCfgParam );

	/*==================================================================
	����        : ����PPPOE����
	�������˵��: const TMTIPPPOEParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetPPPOECfg( const TMTIPPPOEParam & tCfgParam );

	/*==================================================================
	����        : ����ϵͳ��������
	�������˵��: const TMTISysPubParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetSysPubCfg( const TMTISysPubParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ�����������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTICommonNetParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoEncNetCfg( u8 byVideoEncID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�����������
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTICommonNetParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoDecNetCfg( u8 byVideoDecID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�����������
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTICommonNetParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetAudioEncNetCfg( u8 byAudioEncID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�����������
	�������˵��: u8 byAudioDecID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTICommonNetParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetAudioDecNetCfg( u8 byAudioDecID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIVideoEncParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoEncCfg( u8 byVideoEncID, const TMTIVideoEncParam & tCfgParam );
	//{{{{{{{{{{{{{{{ add by Royan 20140328 �ֱ�����ֵ��
	u16 SetVideoEncCfgEx(u8 byVideoEncID, const TMTIVideoEncParam & tCfgParam );
	//}}}}}}}}}}}}}}} add by Royan 20140328 �ֱ�����ֵ��

	/*==================================================================
	����        : ������Ƶ���빫������
	�������˵��: const TMTIVideoEncPubParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoEncPubCfg( const TMTIVideoEncPubParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ������Ļ����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIVideoEncOSDParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoEncOSDCfg( u8 byVideoEncID, const TMTIVideoEncOSDParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ���뻭��ϳɲ���
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIVideoEncMPParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoEncMPCfg( u8 byVideoEncID, const TMTIVideoEncMPParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ������ƵԴ�˿ڲ���
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIVideoEncInputPortParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoEncInputPortCfg( u8 byVideoEncID, const TMTIVideoEncInputPortParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ������ѯ����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIVideoEncPollParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoEncPollCfg( u8 byVideoEncID, const TMTIVideoEncPollParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ�������
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIVideoDecParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoDecCfg( u8 byVideoDecID, const TMTIVideoDecParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ���빫������
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIVideoDecPubParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoDecPubCfg( u8 byVideoDecID, const TMTIVideoDecPubParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ�����ش�����
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIVideoDecRetransferParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoDecRetransferCfg( u8 byVideoDecID, const TMTIVideoDecRetransferParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ�������
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIAudioEncParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetAudioEncCfg( u8 byAudioEncID, const TMTIAudioEncParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ�������
	�������˵��: u8 byAudioDecID ��Ƶ����ͨ��ID(��0��ʼ)
				  const TMTIAudioDecParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetAudioDecCfg( u8 byAudioDecID, const TMTIAudioDecParam & tCfgParam );

	/*==================================================================
	����        : ���ô��ڲ���
	�������˵��: u8 bySerialPortID ����ID(��0��ʼ)
				  const TMTISerialPortParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetSerialPortCfg( u8 bySerialPortID, const TMTISerialPortParam & tCfgParam );
    	/*==================================================================
	����        : �����豸guid
	�������˵��: u32 McuId
				  u32 MtId
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetDevGuidCfg(u32 McuId, u32 MtId);

	/*==================================================================
	����        : ���ú��������
	�������˵��: TMTIBackendCapSets
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetBackendCap(const TMTIBackendCapSets& tBackEndCap);

 	/*==================================================================
	����        :  ��ȡ�豸mcuid��Ҳ����guid��data1�ֶ�
	�������˵��:  ��

	����ֵ˵��  : �ɹ�����mcuid; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u32 GetDevMcuId();
	/*==================================================================
	����        :  ��ȡ�豸mtid��Ҳ����guid��data2��data3�ϲ��ֶ�
	�������˵��:  ��

	����ֵ˵��  : �ɹ�����mtid; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u32 GetDevMtId();

	/*==================================================================
	����        : ���ô��ڹ�������
	�������˵��: u8 bySerialPortID ����ID(��0��ʼ)
				  const TMTISerialPortPubParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetSerialPortPubCfg( u8 bySerialPortID, const TMTISerialPortPubParam & tCfgParam );

	/*==================================================================
	����        : ���ò����������
	�������˵��: u16 wInputPinID ��������˿�ID(��0��ʼ)
				  const TMTIInputPinParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetInputPinCfg( u16 wInputPinID, const TMTIInputPinParam & tCfgParam );

	/*==================================================================
	����        : GetThirdPartyVideoDecCfg
	�������˵��: ��ȡ֧�ֵ�������������
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 GetThirdPartyVideoDecCfg(TMTIThirdPartyDecParam & tCfgParam );

	/*==================================================================
	����        : ��������ͷЭ������
	�������˵��: u8 byVideoInputID ��������˿�ID(��0��ʼ)
				  const u8 byCameraType ����ͷЭ������
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	//{{{{{{{{{{{{{{{ modify by Royan 20140520 SetCameraType����CameraId��
	//u16 SetCameraType( u8 byVideoInputID, const u8 byCameraType );
	u16 SetCameraType( u8 byVideoInputID, const u8 byCameraType, const u8 byCameraId );
	//}}}}}}}}}}}}}}} modify by Royan 20140520 SetCameraType����CameraId��

	/*==================================================================
	����        : ������ƵԴ�˿ڲ���
	�������˵��: u8 byVideoInputID ��������˿�ID(��0��ʼ)
				  const TMTIVideoInputParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoInputCfg( u8 byVideoInputID, const TMTIVideoInputParam & tCfgParam );

	/*==================================================================
	����        : ����ͼ�����β���
	�������˵��: u16 wEntityID ��ƵԴ�˿�ID(��0��ʼ)
				  const TMTIShadeParam & tCfgParam ���õĲ���
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
    u16 SetShadeCfg( u16 wEntityID, const TMTIShadeParam * ptCfgParam, u16 wAreaNum );

	/*==================================================================
	����        : �����ƶ�������
	�������˵��: u8 byType �ƶ��������(MTI_ENCODER-����;MTI_DECODER-����)
				  u16 wEntityID �ƶ����ʵ��ID(����Ϊ��ƵԴ�˿ں�, ����Ϊ������ͨ����)
				  TMTIMotionDetectParamNew * ptCfgParam �ƶ�����������
				  u16 wAreaNum  �ƶ����������
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetMotionDetectCfg( u8 byType, u16 wEntityID, const TMTIMotionDetectParam * ptCfgParam, u16 wAreaNum );

	/*==================================================================
	����        : ��ʼ��Ƶ����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  TMTIMediaSwitchAddr tMediaSwitchAddr ��������ip port��
				  u8 byType ������������(MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StartVideoSend( u8 byVideoEncID,TMTIMediaSwitchAddr& tMediaSwitchAddr, u8 byType );

	/*==================================================================
	����        : ��ʼ��Ƶ����
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ����(��0��ʼ)
				  u32 dwDestIpAddr ����Ŀ��IP��ַ
				  u16 wDestPort ����Ŀ�Ķ˿ں�
				  u8 byType ������������(MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StartAudioSend( u8 byAudioEncID,TMTIMediaSwitchAddr& tMediaSwitchAddr, u8 byType );

	/*==================================================================
	����        : ��ʼ��Ƶ����
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ����(��0��ʼ)
				  u32 dwDestIpAddr ԴIP��ַ
				  u16 wLocalPort ���ն˿ں�
				  u8 byType ������������(MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StartVideoRecv( u8 byVideoDecID, TMTIMediaSwitchAddr& tMediaSwitchAddr, u8 byType );

	/*==================================================================
	����        : ��ʼ��Ƶ����
	�������˵��: u8 byAudioDecID ��Ƶ����ͨ����(��0��ʼ)
				  u32 dwDestIpAddr ԴIP��ַ
				  u16 wLocalPort ���ն˿ں�
				  u8 byType ������������(MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StartAudioRecv( u8 byAudioDecID, TMTIMediaSwitchAddr& tMediaSwitchAddr, u8 byType );

	/*==================================================================
	����        : ������Ƶ����Ŀ��
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  u32 dwDestIpAddr ����Ŀ��IP��ַ
				  u16 wDestPort ����Ŀ�Ķ˿ں�
				  u8 byType ������������(MTI_MEDIA_STREAM_TYPE_UDP ��)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 AddVideoSendDest( u8 byVideoEncID, u32 dwDestIpAddr, u16 wDestPort, u8 byType );

	/*==================================================================
	����        : ɾ����Ƶ����Ŀ��
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
				  u32 dwDestIpAddr ����Ŀ��IP��ַ
				  u16 wDestPort ����Ŀ�Ķ˿ں�
				  u8 byType ������������(MTI_MEDIA_STREAM_TYPE_UDP ��)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 RemoveVideoSendDest( u8 byVideoEncID, u32 dwDestIpAddr, u16 wDestPort, u8 byType );

	/*==================================================================
	����        : ������Ƶ����Ŀ��
	�������˵��: u8 byAEncID ��Ƶ����ͨ����(��0��ʼ)
				  u32 dwDestIpAddr ����Ŀ��IP��ַ
				  u16 wDestPort ����Ŀ�Ķ˿ں�
				  u8 byType ������������(MTI_MEDIA_STREAM_TYPE_UDP ��)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 AddAudioSendDest( u8 byAEncID, u32 dwDestIpAddr, u16 wDestPort, u8 byType );

	/*==================================================================
	����        : ɾ����Ƶ����Ŀ��
	�������˵��: u8 byAEncID ��Ƶ����ͨ����(��0��ʼ)
				  u32 dwDestIpAddr ����Ŀ��IP��ַ
				  u16 wDestPort ����Ŀ�Ķ˿ں�
				  u8 byType ������������(MTI_MEDIA_STREAM_TYPE_UDP ��)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 RemoveAudioSendDest( u8 byAEncID, u32 dwDestIpAddr, u16 wDestPort, u8 byType );

	/*==================================================================
	����        : ֹͣ��Ƶ����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����(��0��ʼ)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StopVideoSend( u8 byVideoEncID );

	/*==================================================================
	����        : ֹͣ��Ƶ����
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ����(��0��ʼ)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StopAudioSend( u8 byAudioEncID );

	/*==================================================================
	����        : ֹͣ��Ƶ����
	�������˵��: u8 byVideoDecID ��Ƶ����ͨ����(��0��ʼ)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StopVideoRecv( u8 byVideoDecID );

	/*==================================================================
	����        : ֹͣ��Ƶ����
	�������˵��: u8 byAudioDecID ��Ƶ����ͨ����(��0��ʼ)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StopAudioRecv( u8 byAudioDecID );

	/*==================================================================
	����        : ������Ƶ����������
	�������˵��: u8 byAudioDecID ��Ƶ����ͨ����(��0��ʼ)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetMediaStreamProp(u8 byChnDecID,TMTIStreamAttribute& tStreamPropList);

	/*==================================================================
	����        : ������������ͨ��
	�������˵��: u8 byComPortID ���ں�(��0��ʼ)
				  u8 byType ��������ͨ������(MTI_SERIAL_STREAM_TYPE_UDP ��)
	����ֵ˵��  : �ɹ����� TRUE; ʧ�ܷ��� FALSE
	==================================================================*/
	BOOL CreateSerialDataChannel( u8 byComPortID, u8 byType );

	/*==================================================================
	����        : ɾ����������ͨ��
	�������˵��: byComPortID ���ں�(��0��ʼ)
	����ֵ˵��  : �ɹ����� TRUE; ʧ�ܷ��� FALSE
	==================================================================*/
	BOOL RemoveSerialDataChannel( u8 byComPortID );

	/*==================================================================
	����        : ��ȡ��ǰ��������ͨ������
	�������˵��: u8 byComPortID ���ں�(��0��ʼ)
	����ֵ˵��  : ��������ͨ������(MTI_SERIAL_STREAM_TYPE_UDP ��)
	==================================================================*/
	u8  GetSerialDataChannelType( u8 byComPortID );

	/*==================================================================
	����        : ���ʹ�������
	�������˵��: u8 byComPortID ���ں�(��0��ʼ)
				  u8* pbyDataBuf ����ָ��
				  u16 wBufLen ���ݳ���
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SendSerialData( u8 byComPortID, u8* pbyDataBuf, u16 wBufLen );

	/*==================================================================
	����        : ��������ͷ��������
	�������˵��: u8 byVideoInputID ��Ƶ����˿ں�(��0��ʼ)
				  u8* pbyDataBuf ����ָ��
				  u16 wBufLen ���ݳ���
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SendCameraCtrlData( u8 byVideoInputID, u8* pbyDataBuf, u16 wBufLen );

	/*==================================================================
	����        : ���ò������״̬
	�������˵��: u16 wOutputPinID ����˿ں�(��0��ʼ)
				  BOOL bIsEnable ���״̬ TRUE��Ч, FALSE��Ч
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetOutputPinState( u16 wOutputPinID, BOOL bIsEnable );

	/*==================================================================
	����        : ������Ƶ����˿�
	�������˵��: u8 byVideoEncID ����ͨ����(��0��ʼ)
				  u8 byVideoInputPortID ��Ƶ����˿ں�(��0��ʼ)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetVideoInputPort( u8 byVideoEncID, u8 byVideoInputPortID );

	/*==================================================================
	����        : ��ʼ�ƶ����
	�������˵��: u8 byType �ƶ��������(MTI_ENCODER-����;MTI_DECODER-����)
				  u16 wEntityID �ƶ����ʵ��ID(����Ϊ��ƵԴ�˿ں�, ����Ϊ������ͨ����)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StartMotionDetect( u8 byType, u16 wEntityID );

	/*==================================================================
	����        : ֹͣ�ƶ����
	�������˵��: u8 byType �ƶ��������(MTI_ENCODER-����;MTI_DECODER-����)
				  u16 wEntityID �ƶ����ʵ��ID(����Ϊ��ƵԴ�˿ں�, ����Ϊ������ͨ����)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StopMotionDetect( u8 byType, u16 wEntityID );

	/*==================================================================
	����        : ��ʼ�澯��ʱ
	�������˵��: u8 byVideoEncID ����ͨ����(��0��ʼ)
				  u16 wSeconds �澯��ʱʱ��
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StartAlarmTimer( TMTIStartAlarmTimer tShowAlarmTimer);

	/*==================================================================
	����        : ֹͣ�澯��ʱ
	�������˵��: u8 byVideoEncID ����ͨ����(��0��ʼ)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 StopAlarmTimer( TMTIStartAlarmTimer tShowAlarmTimer);

	/*==================================================================
	����        : ���ظ澯��ʱ
	�������˵��: u8 byVideoEncID ����ͨ����(��0��ʼ)
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 HideAlarmTimer( u8 byVideoEncID);

	/*==================================================================
	����        : �ָ�Ĭ�ϲ�������
	�������˵��: ��
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 LoadDefaultSettings();

	/*==================================================================
	����        : �������
	�������˵��: ��
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SaveSettings();

	/*==================================================================
	����        : �Ƿ���Ҫ�����豸
	�������˵��: ��
	����ֵ˵��  : TRUE ��Ҫ�����豸; FALSE ����Ҫ�����豸
	==================================================================*/
	BOOL IsRebootRequired( );

	/*==================================================================
	����        : �����豸
	�������˵��: ��
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 Reboot();

	/*==================================================================
	����        : �����豸ϵͳʱ��
	�������˵��: const TMTISysTime & tSysTime ���õ�ʱ��
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SetDevSysTime( const TMTISysTime & tSysTime);

	/*==================================================================
	����        : �����Զ�����Ϣ
	�������˵��: u16 wMsgLen ��Ϣ����
				  u8 * pbyMsgContent ��Ϣ����
	����ֵ˵��  : �ɹ����� ������ˮ��; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	u16 SendCustomMsg( u16 wMsgLen, u8 * pbyMsgContent);

	/*==================================================================
	����        : �����ն˿�ʼ��Ŀ��IP����NAT̽���
	�������˵��: u32 dwMpIPAddr ������IP��dwMcuIPAddr��Mcu��IP
    			  wAudCallDetPort����Ƶ���ж˿ڣ���������Ƶ����̽�����Ŀ�Ķ˿�
				  wRTPNATPort �� RTP̽��˿�
	����ֵ˵��  : ������ˮ��
	==================================================================*/
	u16 StartNATDetect(u32 dwMcuIPAddr, u32 dwMpAddr,u16 wSourcePort, u16 wDesPort, u8 byDeviceType );

	/*==================================================================
	����        : �����ն�ֹͣ��Ŀ��IP����NAT̽���
	�������˵��: dwMpIPAddr ������IP
	����ֵ˵��  : ������ˮ��
	==================================================================*/
	u16 StopNATDetect( u32 dwMpIPAddr);

	/*====================================================================
	����		: �����ն˷��͹ؼ�֡
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
	����ֵ˵��  : �ɹ����ز�����ˮ��, ʧ�ܷ���MTI_FAIL
	====================================================================*/
	u16 SendKeyFrame(u8 byVideoEncID);

	/*====================================================================
	����		: �����ն˷�������ͷ���ش�������
	�������˵��: u8 bySendAble ���ʹ�������ʹ��(MTI_ENABLE����ʼ����,
								��ָ����ƵԴ�˿ں�;MTI_DISABLE��ֹͣ����,
								����ָ����ƵԴ�˿ں�)
				  u8 byVideoInpId ��ƵԴ�˿ں�
	����ֵ˵��  : �ɹ����ز�����ˮ��, ʧ�ܷ���MTI_FAIL
	====================================================================*/
	u16 StartSendComData(u8 bySendAble, u8 byVideoInpId = 0);

	/*====================================================================
	����		: �����豸ע��Ӧ��
	�������˵��: u16 ������
				  u8 byVideoInpId ��ƵԴ�˿ں�
	����ֵ˵��  : �ɹ����ز�����ˮ��, ʧ�ܷ���MTI_FAIL
	====================================================================*/
	u16 SendDevRegeditRsp(u16 wResultRsp);

#ifdef VSIP_MULTI_CHAN_SUPPORT
	u16 SetDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
	u16 GetDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
	u16 SaveDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
#endif

#ifdef VSIP_PERFORM_IMPROVE
	BOOL ConnectionCheck(TVsipGuid tVsipGuid);
#endif

    //Natagent-PU �������Ƚӿ�
	//����������ʵʱ���
    u16 NAStartSendMediaReq(TMTIStartSendMediaReq& tReq);
    u16 NAStartSendMediaAck(TMTIStartSendMediaAck& tAck);
    u16 NAStopSendMediaReq(TMTIStopSendMediaReq& tReq);
	//������������Ƶ���к�ǰ�˽���
    u16 NAStartRecvMediaReq(TMTIStartRecvMediaReq& tReq);
    u16 NAStartRecvMediaAck(TMTIStartRecvMediaAck& tAck);
    u16 NAStopRecvMediaReq(TMTIStopRecvMediaReq& tReq);
	//¼�񲥷�
    u16 NARecStartPlayReq(TMTIRecStartPlayReq& tReq);
    u16 NARecStartPlayAck(TMTIRecStartPlayAck& tAck);
	//¼������
    u16 NARecStartExportReq(TMTIRecStartExportReq& tReq);

    //¼��������ѯ
	u16 QueryRecList(TMTIRecListQryReq& tReq);
	
public :
	u8 m_byCurUserType;		//��ǰ�û���¼���� zlq add
	u8 m_byIdentifyResult;//��֤���
	void ClearUnitCtrl(); //�ͷ��豸���Ƶ����ر�����
	void ReleaseUnitCtrl( ); //�ͷ��豸���ƹر�����
	BOOL GetEntityType( TVsipGuid tVsipGuid, u8 & byType);	//��ȡ��������
	BOOL GetEntityId( TVsipGuid tVsipGuid, u16 * byID);		//��ȡ����ID
	BOOL GetEntityGuid( u8 byType, u16 wId, TVsipGuid & tVsipGuid );//��ȡ����GUID
	void PrintData();

	void SetConnectStatus(BOOL32 bStatus)
	{
		m_bIsConnected = bStatus;
	}
	
	BOOL32 IsIdenticalApp(u16 wInstanceId);

	void SetClientUnitHandle( VSIP_HANDLE tClientHandle)
	{
		m_bIsConnected = TRUE;
		m_tVsipComHandle = tClientHandle;
	}

	VSIP_HANDLE GetVsipHandle( )
	{
		return m_tVsipComHandle;
	}
    u8  GetDeviceType();        //��ȡ�豸����addby 20090903 G.J.
	u16 SendMessageBack( TMTIMsgHandle tMsgHandle, u8 * pbyMsgBuf, u16 wBufLen ); //������Ϣ���û�
	void GetInterConfig(TMTIConfigInfo tConfInfo);//zlq add �ڲ����û�ȡ����,���pui���ò���ʱ��Ϊ�첽������ɵĴ���.
	u8 GetCurConnetction(){return m_tVsipComHandle.ConnectionType;};//zlq add ��ȡ��ǰ��������
//	void SetCapability(TMTICapability tCap){m_tCapability = tCap;};//zlq add �޸�������.����������Ŷ�̬�ı䲢����������.��������½���!
protected:

	void SetConfigState( u8 byConfigType, u16 wEntityId, BOOL bIsValid);

	BOOL GetConfigState( u8 byConfigType, u16 wEntityId);

	void SaveCapabilities( u16 CapListCount, VSIP_IC_CAPABILITIES *pCapList );	//����������

	void SaveConfigData( TVsipGuid tVsipGuid, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//�������ò���

	void SaveDeviceConfigData( u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	// �����豸����

	//{{{{{{{{{{{{{{{ add by Royan 20140328 �ֱ�����ֵ��
	void SaveVideoEncConfigData_WH( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//������Ƶ�������
	//}}}}}}}}}}}}}}} add by Royan 20140328 �ֱ�����ֵ��

	void SaveVideoEncConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//������Ƶ�������

	void SaveVideoDecConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//������Ƶ�������

	void SaveAudioEncConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//������Ƶ�������

	void SaveAudioDecConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//������Ƶ�������

	void SaveSerialPortConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//���洮�ڲ���

	void SaveInputPinConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//���沢������˿ڲ���

	void SaveInputPinStateData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//���沢������״̬ʹ�ܲ���

	void SaveVideoInputConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//������Ƶ����˿ڲ���

	u16 GetConfigID(u16 wChnID); //��ȡ��������addby 20090903 G.J.

	//{{{{{{{{{{{{{{{ add by Royan 20140723 ��ȡ��Ƶ�������ӻص���ʽ
	//void SavePropConfigData( u16 ConfigItemCount, VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo );//�����������ò���
	void SavePropConfigData( u16 ConfigItemCount, VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo, bool& bNeedSendBack);//�����������ò���
	//}}}}}}}}}}}}}}} add by Royan 20140723 ��ȡ��Ƶ�������ӻص���ʽ

	void SavePropNetworkConfig( u16 wConfigId, u8 * pConfData );//�����������ò���

	void SavePropVsipConfig( u16 wConfigId, u8 * pConfData );//����VSIP���ò���

	void SavePropSerialPortConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData );

	void SavePropAudioConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData );

	void SavePropVideoEncConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData );

	void SavePropVideoDecConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData );

	void SavePropWebConfig( u16 wConfigId, u8 * pConfData );

	void SavePropVINConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData, bool& bNeedSendBack );
	friend int DiscoveryRspCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo,
							   TVsipGuid tVsipGuid,u16 Fabricant, u16 Product, u8 StatusChanged );

	friend int GetConfigRspCB( VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, u16 ConfigItemCount,
							   VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );

	friend int GetCapabilityRspCB( VSIP_HANDLE VsipHandle, u16 CapListCount,
								   VSIP_IC_CAPABILITIES *pCapList );

	friend int GetProprietaryRspCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 ConfigItemCount,
									 VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo );

	friend int OperationRspCB( VSIP_HANDLE VsipHandle, u16 ErrorCode );

	friend int EventNotifyCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 EventType, u32 EventArg );

	friend int DisconnectedCB( VSIP_HANDLE VsipHandle, int nSockListNum );

	friend void ConnectionCheckCB(void * pArg);

	friend int CommandExReqCB(VSIP_HANDLE VsipHandle, VSIP_GUID tVsipGuid, u8 CommandCode, u32 Arg1,
		u32 Arg2, u16 AdditionalArgCount, VSIP_MEDIA_CONTROL_ADDITIONAL_ARG *pAddArg);

	//ǰ��¼��洢start
    friend int GetRecStatusRspCB( VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannelId,RecPlayerState byRecStatus);
    friend int GetNmsParamRspCB( VSIP_HANDLE VsipHandle,VSIP_GUID guid,TNmsParam NmsParam);
	friend int GetTimelyRecInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,TimelyRecInfo TRecInfo);
	friend int GetPeriodlyRecInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,PeriodlyTimeInfo PTimeInfo);
	friend int GetChannelRecBasicInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,RecChannelBasicInfo & RChanBasicInfo);
	friend int GetRecSysInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,RecSysInfo RSysInfo);
	friend int QueryRecFileAckCB(VSIP_HANDLE VsipHandle, QueryRecFileInfo QRecFileinfo);
    friend int QueryRecListRspCB(VSIP_HANDLE VsipHandle, TMTIRecListQryRsp tRecListRsp);
	friend int QueryRecFileNumAckCB(VSIP_HANDLE VsipHandle, u16 wNum);
	friend int ProgressCallCB(VSIP_HANDLE VsipHandle, ProgressParaInfo ParaInfo);
	friend int RecStatusChangeCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannel,RecPlayerState byRecStatus);
	friend int RecFileStartPlayidCB(VSIP_HANDLE VsipHandle,TMTIRecFilePlayNtf tPlayNtf);
	friend int FileExportProgressCB(VSIP_HANDLE VsipHandle,ProgressParaInfo TProgress);
	friend int VideoSourceStateCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannel,u8 byState);
	friend int DevRegeditReqCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,TDevRegeditInfo DevRegInfoPara);
	friend int StackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

    //daiyi transdata start
    friend int SetTransDataAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
    friend int GetTransDataAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
    //daiyi transdata end
    
    //2009/08/28 fanxg gps
	friend int GetTransCfgAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);

	//hzc_Transparent_Chan_20110324{{
	friend int StackOperationRspExCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	//}}

	friend int StackOperationRspEx2CB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTITransDataRsp tMTITransDataRsp);

	friend int StackOperationNtfEx2CB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTITransDataNtf tMTITransDataNtf);

	friend int GetCuOperationCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	
	friend int NotifyCuOperationCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);

	friend int DevStreamPropRsp(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTIStartDevRspEx tMTIStreamProps);

	//shenxin_rec_file_down_notify{{
	friend int RecFileDownNotifyCB(VSIP_HANDLE VsipHandle,u16 wErrorCode,u16 wFtpPort);
	//}}

	friend int PlayGpsDataCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwErrcode,u8 byPlayId,u32 dwReserve);
	friend int PlayGpsDataNotifyCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TPlayGpsNotify tPlayGpsDataNotify);

	friend int StackTransSubsRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TTransSubsRsp &tReq);
	friend int StackTransSubsNtfCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TTransSubsNtf &tNtf);

	//kedacom potocal begin
	friend int GetDecoderStyleCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	friend int GetCustomExtCapsCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	//kedacom potocal end

	void SaveNmsParam(TVsipGuid guid,TNmsParam NmsParam);
	void SaveRecStatus(TVsipGuid guid,u8 byChannelId,RecPlayerState byRecStatus);
	void SaveTimelyRecInfo(TVsipGuid guid,u8 byChannelId,TimelyRecInfo TRecInfo);
	void SavePeriodlyRecInfo(TVsipGuid guid,u8 byChannelId,PeriodlyTimeInfo PTimeInfo);
	void SaveChannelRecBasicInfo(TVsipGuid guid,u8 byChannelId,RecChannelBasicInfo RChanBasicInfo);
	void SaveRecSysInfo(TVsipGuid guid,RecSysInfo RSysInfo);
	u16 SaveSnapShotConfig(u8 byChannel, TMTISnapShotConfig tConfig) {m_aSnapShotConfig[byChannel] = tConfig; return MTI_SUCCESS;};
	u16 SaveMatrixConfig(const TMTIMatrixInfo &tConfig){m_MatrixParam = tConfig; return MTI_SUCCESS;};
	u16 SaveSnapShotNameRule(TMTISnapShotNamingRule tConfig) { m_tSnapNameRule = tConfig; return MTI_SUCCESS;};
	u16 SavePuNmsParam(TMTIPUNMSCFG tConfig){m_tPuNmsParam = tConfig; return MTI_SUCCESS;};
	//ǰ��¼��洢end

#ifdef VSIP_MULTI_CHAN_SUPPORT
	friend void GetDevMultiEncchanParamRespCB (VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, TMultiChanInfo &MultiChanInfo);
#endif

	//add by Royan 20130201
public:
	u16 GetEnMotionDetectAreaNum(u16 wEntityId);
	u16 GetShadeAreaNum(u16 wEntityId);
	TMTIMotionDetectParam* GetEnMotionDetectParam(u16 wEntityId, u16 wIndex);
	TMTIShadeParam* GetShadeParam(u16 wEntityId, u16 wIndex);
	TMTISerialPortParam* GetSerialPortParam(u16 wEntityId);
	TMTIAudioEncParam* GetAudioEncParam(u16 wEntityId);
	TMTISysParam* GetSysParam();
	TMTIVideoInputParam* GetVideoInputParam(u16 wEntityId);


#ifdef linux  
	void LockMutex();
	void UnlockMutex();
#endif

private:
#ifdef linux                     //add 20150124����ֹpui��mtiͬʱ�ͷ���Դ����coredump
	pthread_mutex_t m_mutex;
#endif
	//ǰ�˴洢start
    //TimelyRecInfo m_aTimelyRecInfo[MAX_CHANNEL_NUM];
    map<u16, TimelyRecInfo> m_aTimelyRecInfo;
	//PeriodlyTimeInfo m_aPeriodTimeInfo[MAX_CHANNEL_NUM];
    map<u16, PeriodlyTimeInfo> m_aPeriodTimeInfo;
    //RecChannelBasicInfo m_aRecChannelBasicInfo[MAX_CHANNEL_NUM];
    map<u16, RecChannelBasicInfo> m_aRecChannelBasicInfo;
    RecSysInfo m_RecSysInfo;
	//RecPlayerState m_abyRecStatus[MAX_CHANNEL_NUM];
    map<u16, RecPlayerState> m_abyRecStatus;
    TNmsParam m_NmsParam;
	TMTIAlarmParam m_tAlarmParam;
	TMTIBannerParam m_tBannerParam;
	//TMTISnapShotConfig m_aSnapShotConfig[MAX_CHANNEL_NUM];
    map<u16, TMTISnapShotConfig> m_aSnapShotConfig;
	TMTISnapShotNamingRule m_tSnapNameRule;
	//ǰ�˴洢end

	pMESSAGE_CB					m_pfuncMessageCB;
	void					*	m_pCallBackContext;
	TMTIInitInfo				m_tUnitInfo;
	//TMTICapItemInfo				m_aptEntityInfo[MAX_ENTITY_TOTAL_NUM];
    map<u16, TMTICapItemInfo>				m_aptEntityInfo;
    u16                         m_wEntityCount; //modify G.J. 20090903 m_byEntityCount -> m_wEntityCount
	TMTICapability				m_tCapability;
	TMTICAPSET					m_tExtCapablity;
	BOOL						m_bIsRebootRequired;
	BOOL						m_bIsConnected;
	//NEW
	//u16							m_awMotEnDetAreaNum[MAX_VIDEO_INPUT_PORT_NUM];
    map<u16, u16>							m_awMotEnDetAreaNum;
	//u16							m_awMotDeDetAreaNum[MAX_VIDEO_ENC_NUM];
    map<u16, u16>							m_awMotDeDetAreaNum;

	//u64							m_addConfigState[PARAM_TYPE_MAX_VALUE]; //modify  G.J. 20090903 u32 --> u64
    map<u16, u64>							m_addConfigState;
	//u64							m_addConfigStateNvr128[PARAM_TYPE_MAX_VALUE]; //nvr128//modify by shenxin,���ڱ���64-128·״̬
    map<u16, u64>							m_addConfigStateNvr128;
	VSIP_HANDLE					m_tVsipComHandle;	//zhuzhm:��Ҫ��Ա
													//���з��ͽ���API����ͨ���˾��
													//��GetUnitCtrlʱ��װ�ؾ��
	VSIP_HANDLE					m_tSerialDataHandle;
	u8							m_bySerialDataChnType;

	TMTISysParam				m_tSysParam;
	TMTISysPubParam				m_tSysPubParam;
	TMTIPPPOEParam				m_tPPPOEParam;

	//TMTICommonNetParam			m_atVideoEncNetParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTICommonNetParam>			m_atVideoEncNetParam;
	//TMTIVideoEncParam			m_atVideoEncParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncParam>			m_atVideoEncParam;
	TMTIVideoEncPubParam		m_tVideoEncPubParam;
	//TMTIVideoEncOSDParam		m_atVideoEncOSDParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncOSDParam>		m_atVideoEncOSDParam;
	//TMTIVideoEncMPParam			m_atVideoEncMPParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncMPParam>			m_atVideoEncMPParam;
	//TMTIVideoEncInputPortParam	m_atVideoEncInputPortParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncInputPortParam>	m_atVideoEncInputPortParam;
	//TMTIVideoEncPollParam		m_atVideoEncPollParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncPollParam>		m_atVideoEncPollParam;

	//TMTICommonNetParam			m_atVideoDecNetParam[MAX_VIDEO_DEC_NUM];
    map<u16, TMTICommonNetParam>			m_atVideoDecNetParam;
	//TMTIVideoDecParam			m_atVideoDecParam[MAX_VIDEO_DEC_NUM];
    map<u16, TMTIVideoDecParam>			m_atVideoDecParam;
	//TMTIVideoDecPubParam		m_atVideoDecPubParam[MAX_VIDEO_DEC_NUM];
    map<u16, TMTIVideoDecPubParam>		m_atVideoDecPubParam;
	//TMTIVideoDecRetransferParam	m_atVideoDecRetransferParam[MAX_VIDEO_DEC_NUM];
    map<u16, TMTIVideoDecRetransferParam>	m_atVideoDecRetransferParam;
	//NEW
	//TMTIMotionDetectParam       m_atDeMotionDetParamNew[MAX_VIDEO_DEC_NUM][MAX_DET_AREA_NUM];

    map<u16, map<u16, TMTIMotionDetectParam> >      m_atDeMotionDetParamNew;

	//TMTICommonNetParam			m_atAudioEncNetParam[MAX_AUDIO_ENC_NUM];
    map<u16, TMTICommonNetParam>			m_atAudioEncNetParam;
	//TMTIAudioEncParam			m_atAudioEncParam[MAX_AUDIO_ENC_NUM];
    map<u16, TMTIAudioEncParam>			m_atAudioEncParam;
	//TMTICommonNetParam			m_atAudioDecNetParam[MAX_AUDIO_DEC_NUM];
    map<u16, TMTICommonNetParam>			m_atAudioDecNetParam;
	//TMTIAudioDecParam			m_atAudioDecParam[MAX_AUDIO_DEC_NUM];
    map<u16, TMTIAudioDecParam>			m_atAudioDecParam;

	//TMTISerialPortParam			m_atSerialPortParam[MAX_SERIAL_PORT_NUM];
    map<u16, TMTISerialPortParam>			m_atSerialPortParam;
	//TMTISerialPortPubParam		m_atSerialPortPubParam[MAX_SERIAL_PORT_NUM];
    map<u16, TMTISerialPortPubParam>		m_atSerialPortPubParam;

	//TMTIInputPinParam			m_atInputPinParam[MAX_INPUT_PIN_NUM];
    map<u16, TMTIInputPinParam>			m_atInputPinParam;
	TMTIInputPinState           m_tInputPinEnableState; //20091020
	//TMTIVideoInputParam			m_atVideoInputParam[MAX_VIDEO_INPUT_PORT_NUM];
    map<u16, TMTIVideoInputParam>			m_atVideoInputParam;
	//NEW
	//TMTIMotionDetectParam       m_atEnMotionDetParamNew[MAX_VIDEO_INPUT_PORT_NUM][MAX_DET_AREA_NUM];
    map<u16, map<u16, TMTIMotionDetectParam> > m_atEnMotionDetParamNew;
    //TMTIShadeParam              m_atShadeParam[MAX_VIDEO_INPUT_PORT_NUM][MAX_SHADE_AREA_NUM];
    map<u16, map<u16, TMTIShadeParam> > m_atShadeParam;
	//u16				            m_awShadeAreaNum[MAX_VIDEO_INPUT_PORT_NUM];
    map<u16, u16>				            m_awShadeAreaNum;
	//���� zlq add
	TMTIMatrixInfo				m_MatrixParam;
	//�����������
	TMTIPUNMSCFG				m_tPuNmsParam;
	
	BOOL32                      m_bIsNewVideoParamVersion; //�°汾ǰ����Ƶ��������ϱ�������, Ϊ�����¾�ǰ�˹ʼ����ж�addby G.J.20090901
	//{{{{{{{{{{{{{{{ add by Royan 20140328 �ֱ�����ֵ��
	BOOL32                      m_bIsNewVideoParamVersionWH;    //20130904
	//}}}}}}}}}}}}}}} add by Royan 20140328 �ֱ�����ֵ��

	BOOL32                      m_bIsRcvInputPinEntity; //�Ƿ��Ѿ����յ����������ϱ� 20091020
	u16                         m_wInputPinEntityStartID; //���ڸ澯��ʼGUID 20091020

#ifdef VSIP_MULTI_CHAN_SUPPORT
	TDevMultiChanInfo			m_tDevMultiChanInfo;
#endif
	TMTIThirdPartyDecParam      m_tThirdPartyDecParam; //add 20100317
};

class CMTIClientCtrl
{
	//��·�����Ϣ
	typedef struct TMTIClientConnCheckInfo
	{
		u8				byCounter;		//��������
		SOCKHANDLE		SocketId;		//��·�׽���
	}PACKED TMTI_CLIENT_CONN_CHECK_INF, *PTMTI_CLIENT_CONN_CHECK_INF;

public:
	CMTIClientCtrl();
	~CMTIClientCtrl();

    //�̼߳��
//#ifndef _LINUXEQUATOR_
	u16 StartFileTransAck(VSIP_HANDLE VsipHandle,TFileTransRspInfo tFileTransRspInfo);
//#endif
    /*==================================================================
	����        : ��ȡ��Ϣ�����̼߳���
	�������˵��: ��
	����ֵ˵��  : ��Ϣ�����̼߳���
	==================================================================*/
    u32 GetMsgRcvCheckCount();
   /*==================================================================
	����        : ��ȡ��ʱ���̼߳���
	�������˵��: ��
	����ֵ˵��  : ��Ϣ��ʱ���̼߳���
	==================================================================*/
    u32 GetTimerCheckCount();
	/*==================================================================
	����        : �ر���·
	�������˵��: ip+�˿�
	����ֵ˵��  : ��
	==================================================================*/
	void CloseConnection(u32 dwIpAddr, unsigned short wPort);
//#ifndef _LINUXEQUATOR_
	void PrintDebug(int selection,int index);
	void PrintConnectionInfo();
	void PrintUsedCapBuf();
	void PrintCapBuf(int index);
	void PrintStackConnection(int index);
	void PrintEntityBuf(int index);
//#endif//_LINUXEQUATOR_
	/*==================================================================
	����        : linuxϵͳ�»�ȡ��ʱ��pid
	�������˵��: ��
	����ֵ˵��  : ��ʱ��pid
	==================================================================*/
	u32 GetTimerThreadId();
	/*==================================================================
	����        : linuxϵͳ�»�ȡ��Ϣ�����߳�pid
	�������˵��: ��
	����ֵ˵��  : ��Ϣ�����߳�pid
	==================================================================*/
	u32 GetMsgRcvThreadId();

	/*==================================================================
	����        : ��·���
	�������˵��: ip��ַ�Ͷ˿�
	����ֵ˵��  : MTI_SUCCESS-���ʾ�ɹ�����ping��Ϣ; MTI_FAIL-���ʾ����·������

	==================================================================*/
    BOOL ConnectionCheck(u32 dwIpAddr, unsigned short wPort);

	/*==================================================================
	����        : �ж�ָ��ip�Ƿ�����·����
	�������˵��: ip��ַ
	����ֵ˵��  : MTI_SUCCESS-���ʾ��·����; MTI_FAIL-���ʾ����·������

	==================================================================*/
	BOOL FindConnection(u32 dwIpAddr);

	/*====================================================================
	����		: �����豸ע��
	�������˵��: u32 (mcu��ַ������ע���룩
				  u8 byVideoInpId ��ƵԴ�˿ں�
	����ֵ˵��  : �ɹ�����TRUE, ʧ�ܷ���FALSE
	====================================================================*/
	BOOL DevRegedit(u32 dwIpAddr,TDevRegeditInfo DevRegInfoPara);

	//ǰ�˴洢�ӿ�start
	int SetNmsParam(TNmsParam NmsParam);
	/*==================================================================
	����        : ����¼����״̬
	�������˵��: ͨ��id��¼��״̬
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
//#ifndef _LINUXEQUATOR_
    u16 SetRecStatus(u8 byChannelId,RecPlayerState byRecStatus);
	/*==================================================================
	����        : ���ö�ʱ¼����Ϣ״̬
	�������˵��: ͨ��id����ʱ¼����Ϣ
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	 u16 SetTimelyRecInfo(u8 byChannel, TimelyRecInfo TRecInfo);
	/*==================================================================
	����        : ��������¼����Ϣ״̬
	�������˵��: ͨ��id������¼����Ϣ
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
     u16 SetPeriodlyRecInfo (u8 byChannel, PeriodlyTimeInfo PTimeInfo);
	 /*==================================================================
	����        : ����ͨ����Ϣ
	�������˵��: ͨ��id��ͨ��������Ϣ
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
     u16 SetChannelRecBasicInfo(u8 byChannel, RecChannelBasicInfo RChannelBasicInfo);
	 /*==================================================================
	����        : ����¼��ϵͳϵͳ
	�������˵��: ¼��ϵͳϵͳ
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	 u16 SetRecSysInfo(RecSysInfo RSysInfo);
	 /*==================================================================
	����        : ϵͳ��Ϣ֪ͨ
	�������˵��: 
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	 u16 SysStateNotify(u32 nSysArg);
	 /*==================================================================
	����        : ���Ͳ�ѯ��Ϣ
	�������˵��: ��ѯ����
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
    int QueryAck(VSIP_HANDLE VsipHandle, QueryRecFileInfo QRecFileinfo);
    /*==================================================================
	����        : ���Ͳ�ѯ���ĸ�����Ϣ
	�������˵��: ����
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	int QueryNumAck(VSIP_HANDLE VsipHandle,u16 wNum);    
	/*==================================================================
	����        : �����ϱ�
	�������˵��: ���Ȳ���
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	int ProgressResult(VSIP_HANDLE VsipHandle,ProgressParaInfo ParaInfo);
	/*==================================================================
	����        : ��ӡ¼������Ϣ
	�������˵��: ͨ����
	����ֵ˵��  : ��
	==================================================================*/
	void PrintRecInfo(u8 byChannel);
	/*==================================================================
	����        : ����playerid
	�������˵��: handle�� id��
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
    int SendPlayerId(VSIP_HANDLE VsipHandle , u8 byPlayerId);
	/*==================================================================
	����        : ����mt���ļ����ص�ip��ַ�Ͷ˿ں�
	�������˵��: handle�� id��
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
    int FileExportProgress(VSIP_HANDLE VsipHandle,ProgressParaInfo TProgress);
//#endif
	/*==================================================================
	����        : ��������ƵԴ֪ͨ
	�������˵��: ͨ����
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	u16 SetVideoSoureState(u8 byChannel,u8 byState);
	//ǰ�˴洢�ӿ�end
	/*==================================================================
	����        : ��ʼ��
	�������˵��: ��
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	u16 Init(TMTISysPubParam tSysPubPara,u16 wMaxOnlineMtNum = 5,u16 wSendPort = 60000, u16 wFtpPort = 1730 );

	BOOL ForwordConnection(u32 dwIpAddr, u16 wPort);

	/*==================================================================
	����        : ������·�����ʱ����
	�������˵��: u16 wCheckInterval ��·�����ʱ���� ��λms
	����ֵ˵��  : ��
	����˵��	: �ڳ�ʼ���ɹ������
	==================================================================*/
	void SetConnectionCheckInterval( u16 wCheckInterval = 30000 ){};

	/*==================================================================
	����        : ������Ϣ�ص�����
	�������˵��: u32 dwLocalIpAddr ����IP��ַ(������)
				  u32 dwMultiIpAddr VSIP�鲥��ַ(������)
				  u16 wVsipPort VSIP�˿ں�
	����ֵ˵��  : TRUE-�ɹ�; FALSE-ʧ��
	==================================================================*/
	BOOL SetMessageCallBack( pMESSAGE_CB pFunc, void* pContext = NULL );

	/*==================================================================
	����        : �����豸������
	�������˵��: const TMTICapability & tCapability �豸������
	����ֵ˵��  : ��
	==================================================================*/
	void SetCapabilities( const TMTIPUCapability & tCapability, const TVsipGuid & tDeviceGuid);

	/*==================================================================
	����        : �����豸ϵͳ����
	�������˵��: const TMTISysParam & tCfgParam �豸ϵͳ����
	����ֵ˵��  : ��
	==================================================================*/
	void SetSysCfg( const TMTISysParam & tCfgParam);
//#ifndef _LINUXEQUATOR_
	/*==================================================================
	����        : ���ø澯���ò���(��������)
	�������˵��: const TMTISysParam & tCfgParam �豸ϵͳ����
	����ֵ˵��  : ��
	==================================================================*/
	void SetAlarmParamCfg( const TMTIAlarmParam & tCfgParam);
	/*==================================================================
	����        : ���ú������(��������)
	�������˵��: const TMTISysParam & tCfgParam �豸ϵͳ����
	����ֵ˵��  : ��
	==================================================================*/
	void SetBannerParamCfg( const TMTIBannerParam & tCfgParam);
//#endif //_LINUXEQUATOR_
	/*==================================================================
	����        : ����PPPOE����
	�������˵��: const TMTIPPPOEParam & tCfgParam �豸ϵͳ����
	����ֵ˵��  : ��
	==================================================================*/
	void SetPPPOECfg(const TMTIPPPOEParam & tCfgParam);

	/*==================================================================
	����        : ����ϵͳ��������
	�������˵��: const TMTISysPubParam & tCfgParam ϵͳ��������
	����ֵ˵��  : ��
	==================================================================*/
	void SetSysPubCfg( const TMTISysPubParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�����������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTICommonNetParam & tCfgParam ��Ƶ�����������
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoEncNetCfg( u8 byVideoEncID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoEncParam & tCfgParam ��Ƶ�������
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoEncCfg( u8 byVideoEncID, const TMTIVideoEncParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ���빫������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoEncPubParam & tCfgParam ��Ƶ���빫������
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoEncPubCfg( const TMTIVideoEncPubParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ������Ļ����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoEncOSDParam & tCfgParam ��Ƶ������Ļ����
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoEncOSDCfg( u8 byVideoEncID, const TMTIVideoEncOSDParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ���뻭��ϳɲ���
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoEncMPParam & tCfgParam ��Ƶ���뻭��ϳɲ���
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoEncMPCfg( u8 byVideoEncID, const TMTIVideoEncMPParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ������ƵԴ�˿ڲ���
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoEncInputPortParam & tCfgParam ��Ƶ������ƵԴ�˿ڲ���
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoEncInputPortCfg( u8 byVideoEncID, const TMTIVideoEncInputPortParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ������ѯ����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoEncPollParam & tCfgParam ��Ƶ������ѯ����
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoEncPollCfg( u8 byVideoEncID, const TMTIVideoEncPollParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ�����������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTICommonNetParam & tCfgParam ��Ƶ�����������
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoDecNetCfg( u8 byVideoDecID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoDecParam & tCfgParam ��Ƶ�������
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoDecCfg( u8 byVideoDecID, const TMTIVideoDecParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ���빫������
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoDecPubParam & tCfgParam ��Ƶ���빫������
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoDecPubCfg( u8 byVideoDecID, const TMTIVideoDecPubParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ�����ش�����
	�������˵��: u8 byVideoEncID ��Ƶ����ͨ����
				  const TMTIVideoDecRetransferParam & tCfgParam ��Ƶ�����ش�����
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoDecRetransferCfg( u8 byVideoDecID, const TMTIVideoDecRetransferParam & tCfgParam );

	/*==================================================================
	����        : ������Ƶ�����������
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ����
				  const TMTICommonNetParam & tCfgParam ��Ƶ�����������
	����ֵ˵��  : ��
	==================================================================*/
	void SetAudioEncNetCfg( u8 byAudioEncID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�������
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ����
				  const TMTIAudioEncParam & tCfgParam ��Ƶ�������
	����ֵ˵��  : ��
	==================================================================*/
	void SetAudioEncCfg( u8 byAudioEncID, const TMTIAudioEncParam & tCfgParam ); //������Ƶ�������

	/*==================================================================
	����        : ������Ƶ�����������
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ����
				  const TMTICommonNetParam & tCfgParam ��Ƶ�����������
	����ֵ˵��  : ��
	==================================================================*/
	void SetAudioDecNetCfg( u8 byAudioDecID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	����        : ������Ƶ�������
	�������˵��: u8 byAudioEncID ��Ƶ����ͨ����
				  const TMTIAudioDecParam & tCfgParam ��Ƶ�������
	����ֵ˵��  : ��
	==================================================================*/
	void SetAudioDecCfg( u8 byAudioDecID, const TMTIAudioDecParam & tCfgParam );

	/*==================================================================
	����        : ���ô��ڲ���
	�������˵��: u8 bySerialPortID ���ں�
				  const TMTISerialPortParam & tCfgParam ���ڲ���
	����ֵ˵��  : ��
	==================================================================*/
	void SetSerialPortCfg( u8 bySerialPortID, const TMTISerialPortParam & tCfgParam );

	/*==================================================================
	����        : ���ô��ڹ�������
	�������˵��: u8 bySerialPortID ���ں�
				  const TMTISerialPortPubParam & tCfgParam ���ڹ�������
	����ֵ˵��  : ��
	==================================================================*/
	void SetSerialPortPubCfg( u8 bySerialPortID, const TMTISerialPortPubParam & tCfgParam );

	/*==================================================================
	����        : ���ò����������
	�������˵��: u16 wInputPinID ��������˿ں�
				  const TMTIInputPinParam & tCfgParam �����������
	����ֵ˵��  : ��
	==================================================================*/
	void SetInputPinCfg( u16 wInputPinID, const TMTIInputPinParam & tCfgParam );

	/*==================================================================
	����        : ������ƵԴ�˿ڲ���
	�������˵��: u8 byVideoInputID ��ƵԴ�˿ں�
				  const TMTIVideoInputParam & tCfgParam ��ƵԴ�˿ڲ���
	����ֵ˵��  : ��
	==================================================================*/
	void SetVideoInputCfg( u8 byVideoInputID, const TMTIVideoInputParam & tCfgParam );

	/*==================================================================
	����        : �����ƶ�������
	�������˵��: u8 byType �ƶ��������(MTI_ENCODER-����;MTI_DECODER-����)
				  u16 wEntityID �ƶ����ʵ��ID(����Ϊ��ƵԴ�˿ں�, ����Ϊ������ͨ����)
				  TMTIMotionDetectParam * tCfgParam �ƶ�������
				  u16 wDetAreaNum ���������
	����ֵ˵��  : �ɹ����� MTI_SUCCESS; ʧ�ܷ��� MTI_FAIL
	==================================================================*/
	void SetMotionDetectCfg( u8 byType, u16 wEntityID, u16 wDetAreaNum, const TMTIMotionDetectParam *tCfgParam );


	/*==================================================================
	����        : ����ͼ�����β���
	�������˵��: u16 wEntityID ��ƵԴ�˿�ID(��0��ʼ)
				  const TMTIShadeParam & tCfgParam ���õĲ���
	����ֵ˵��  : ��
	==================================================================*/
	void SetShadeCfg(u16 wEntityID, u16 wShadeAreaNum, const TMTIShadeParam *tCfgParam );
	/*==================================================================
	����        : �����豸�Ƿ���Ҫ����
	�������˵��: BOOL bIsRebootRequired TRUE-��Ҫ����; FALSE-����Ҫ����
	����ֵ˵��  : ��
	==================================================================*/
	void SetIsRebootRequired( BOOL bIsRebootRequired );

	/*==================================================================
	����        : ���Ͳ�������״̬
	�������˵��: u16 wInputPinID ��������˿ں�
				  BOOL bIsEnable ��������״̬
	����ֵ˵��  : ��
	==================================================================*/
	void SendInputPinState( u16 wInputPinID, BOOL bIsEnable );

	void SendInputPinExtendInfo(u16 wInputPinID,u8 byEventType,u16 wLength,char *pabyBuf);

	u16 SendTransparentData(VSIP_HANDLE tVsipHandle,char *Buf,u16 wBufLen);

	/*==================================================================
	����        : ���Ͳ������״̬
	�������˵��: u16 wOutputPinID ��������˿ں�
				  BOOL bIsEnable �������״̬
	����ֵ˵��  : ��
	==================================================================*/
	void SendOutputPinState( u16  wOutputPinID, BOOL bIsEnable );
	/*==================================================================
	����        : ���ͽ���ͨ����������״̬
	�������˵��: u8 DecodeChannelId ����ͨ���˿ں�
				  BOOL bActive ��������
	����ֵ˵��  : ��
	==================================================================*/
	void SendIsBitCurrentState(u8 DecodeChannelId, BOOL bActive);

	/*==================================================================
	����        : �����ƶ����澯״̬
	�������˵��: u8 byType �ƶ��������(MTI_ENCODER-����;MTI_DECODER-����)
				  u16 wEntityID �ƶ����ʵ��ID(����Ϊ��ƵԴ�˿ں�, ����Ϊ������ͨ����)
				  u16 wAlarmAreaNum �ƶ����澯�������
				  BOOL bIsEnable �ƶ����״̬
	����ֵ˵��  : ��
	==================================================================*/
	void SendMotionDetectState( u8 byType, u16 wEntityID, u16 wAlarmAreaNum ,BOOL bIsEnable );

	/*==================================================================
	����        : ���ز������
	�������˵��: const TMTIMsgHandle & tMsgHandle ��Ϣ���
				  u16 wResult �������(MTI_SUCCESS, MTI_FAIL)
	����ֵ˵��  : ��
	==================================================================*/
	void SendOperationResult( const TMTIMsgHandle & tMsgHandle, u16 wResult );

	/*==================================================================
	����        : ���ش�������
	�������˵��: u8 byVideoInputID ��ƵԴ�˿ں�
				  u8 *pbySerData	��������
				  u16 wDataLen		���ݳ���
	����ֵ˵��  : ��
	==================================================================*/
	void SendSerialDataNotify( u8 byVideoInputID, u8 *pbySerData, u16 wDataLen);
	void SendSerialDataNotifyEX( u8 byEntityId, u8 byEntityType, u8 *pbySerData, u16 wDataLen);
	/*==================================================================
	����        : �Ƿ���������
	�������˵��:
	����ֵ˵��  : ��
	==================================================================*/
	BOOL IsConnectForwardly();

	void CloseStack();

	u16 GetGuidReq(u32 dwIp,u8 byLocalSerialId,u8 byTargetSerialId);

	void SetGetGuidCallBack( pGetGuidByIp_CB pFunc);

	BOOL CreatTransparentChannel(u32 dwIp,TVsipGuid tGuid,u8 byConnectionType);

	u16 SendVideoDynVal(u8 byChanId,u8 byVidFmt, u8 byDynVal );

	u16 ChangePassWordAck(VSIP_HANDLE tVsipHandle,unsigned short ErrorCode);	//zlq add �޸�����
//#ifndef _LINUXEQUATOR_
	u16 SetBmpOsdAck(VSIP_HANDLE tVsipHandle,TMT_BMPOSDACK_INFO tBmpAckInfo);	//zlq add
	int QueryPicFileAck(VSIP_HANDLE tVsipHandle, TMTIQueryPicFileInfo QueryInfo); //zlq add
	int QueryPicFileNumAck(VSIP_HANDLE tVsipHandle, u16 wNum);
	int DownLoadPicFileAck(VSIP_HANDLE tVsipHandle, TMTIDownloadPicFileInfo tPicInfo);
	int DelPicFileAck(VSIP_HANDLE tVsipHandle, TMTISnapShotResult tCfg);
	int SetSnapShotConfigAck(VSIP_HANDLE tVsipHandle, u16 wError);
	int SetSnapShotConfig(u8 byChanId,TMTISnapShotConfig tConfig);
	int SetSnapShotNameRule(TMTISnapShotNamingRule tConfig);
	u16 StartSnapShotAck(VSIP_HANDLE tVsipHandle, u16 wError);
	u16 StopSnapShotAck(VSIP_HANDLE tVsipHandle, u16 wError);
	u16 SendUserTcpMSg(VSIP_HANDLE tVsipHandle, u8 byType, char *pbyBuf, u16 wpbyBufLen);
	u16 SendUserUdpMSg(u32 dwDstIpaddr, u16 wDstPort, u8 byType, char *pbyBuf, u16 wpbyBufLen);
	u16 DirectTransChanChangeMsg(VSIP_HANDLE tVsipHandle);
	u16 CapbilityChangeMsg(TMTICapability tCapability);
	u16 SetMatrixParam(const TMTIMatrixInfo &tConfig);
	u16 SetMatrixConfigAck(VSIP_HANDLE tVsipHandle, u16 wError);
	void SendRecStateAlarm(u8 ChanID, u16 ErrorCode);
	void SendVideoInputState(u8 ChanID, BOOL bError);
	VSIP_HANDLE GetForwordVsipHandle();//zlq add
//#endif //_LINUXEQUATOR_
	VSIP_HANDLE GetTransparentHandle();

#ifdef VSIP_MULTI_CHAN_SUPPORT
	u16 SetDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
	u16 GetDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
#endif

protected:
	void SetConfigState( u8 byConfigType, u16 wEntityId, BOOL bIsValid);
	BOOL GetConfigState( u8 byConfigType, u16 wEntityId);

	u16 SendMessageBack( const TMTIMsgHandle & tMsgHandle, u8 * pbyMsgBuf, u16 wBufLen ); //������Ϣ���û�

	BOOL GetEntityType( TVsipGuid tVsipGuid, u8 & byType);	//��ȡ��������
	BOOL GetEntityId( TVsipGuid tVsipGuid, u16 * wID);		//��ȡ����ID
	BOOL GetEntityGuid( u8 byType, u16 wId, TVsipGuid & tVsipGuid );//��ȡ����GUID

	//ǰ�˴洢�ӿ�start
    u16 GetRecStatus(TVsipGuid tVsipGuid,u8 byChannelId,RecPlayerState & byRecStatus);
	u16 GetTimelyRecInfo(TVsipGuid tVsipGuid,u8 byChannelId,TimelyRecInfo & TRecInfo);
	u16 GetPeriodlyRecInfo(TVsipGuid tVsipGuid,u8 byChannelId,PeriodlyTimeInfo & PTimeInfo);
	u16 GetChannelRecBasicInfo(TVsipGuid tVsipGuid,u8 byChannelId,RecChannelBasicInfo & RChanBasicInfo);
	u16 GetRecSysInfo(TVsipGuid tVsipGuid,RecSysInfo & RSysInfo);
	u16 GetNmsParam(TVsipGuid tVsipGuid,TNmsParam & NmsParam);
	TMTISnapShotConfig GetSnapShotConfig(u8 byChanId){return m_aSnapShotConfig[byChanId];}; //zlq add
	TMTISnapShotNamingRule GetSnapShotNameRule(){return m_tSnapShotNameRule;};
	TMTIMatrixInfo GetMatrixConfig(){return m_MatrixParam;};
	//ǰ�˴洢�ӿ�end

	u16 GetConfig( TVsipGuid tVsipGuid, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen); //��ȡ�豸����

	u16 GetDeviceConfigData( u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//��ȡ�豸����

	u16 GetVideoEncConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//��ȡ��Ƶ�������

	u16 GetVideoDecConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
		   		  char achBuf[], u16 wBufLen );	//��ȡ��Ƶ�������

	u16 GetAudioEncConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//��ȡ��Ƶ�������

	u16 GetAudioDecConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//��ȡ��Ƶ�������

	u16 GetSerialPortConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//��ȡ���ڲ���

	u16 GetInputPinConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//��ȡ��������˿ڲ���

	u16 GetVideoInputConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//��ȡ��Ƶ����˿ڲ���

	u16 GetPropConfig( u16 ConfigItemCount, VSIP_MEDIA_CONTROL_GETPROPRIETARY_CONFIG *pConfigItemId,
				   	  char achBuf[], u16 wBufLen); //��ȡ�豸���Բ���

	u16 GetPropNetworkCfg( u16 wConfId, char * pchBuf );

	u16 GetPropVsipCfg( u16 wConfId, char * pchBuf );

	u16 GetPropSysCfg( u16 wConfId, char * pchBuf );

	u16 GetPropSerialPortCfg( u16 wConfId, char * pchBuf );

	u16 GetPropAudioCfg( u16 wConfId, char * pchBuf );

	u16 GetPropVideoEncCfg( u16 wConfId, char * pchBuf );

	u16 GetPropVideoDecCfg( u16 wConfId, char * pchBuf );

	u16 GetPropOsdCfg( u16 wConfId, char * pchBuf );

	u16 GetPropWebCfg( u16 wConfId, char * pchBuf );

	u16 GetPropVideoInCfg( u16 wConfId, char * pchBuf );

	//�����豸����
	u16 SetConfigReq( TMTIMsgHandle & tMsgHandle, TVsipGuid tVsipGuid, u16 wConfigItemCount,
					  VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo);

	BOOL SetDevConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetVENConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetAENConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetADEConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetSPConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetINPConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetOUTPConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetVINConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetVDEConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	u16 SetDevGuidReq(TMTIMsgHandle & tMsgHandle, TVsipGuid tVsipGuid);

	BOOL SetDevGuidConfig(TMTIMsgHandle & tMsgHandle, TVsipGuid tVsipGuid);


	//�����豸���Բ���
	u16 SetPropConfigReq( TMTIMsgHandle tMsgHandle, VSIP_GUID tVsipGuid, u16 wConfigItemCount,
						 VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo);
	BOOL SetPropSysCfg( TMTIMsgHandle tMsgHandle, TMTISysPubParam &tSysPubParam, u16 wConfigId, u8 byConfigtype, u16 &wResult, u8 *pConfData);
	BOOL SetPropNetworkCfg( TMTISysPubParam & tSysPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVsipCfg( TMTISysPubParam &tSysPubParam, u16 wConfigId, u8 *pConfData);
	BOOL SetPropSerialPortCfg( TMTISerialPortPubParam & tSerialPortPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropAudioEncNetCfg( TMTICommonNetParam & tAudioEncNetParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropAudioDecNetCfg( TMTICommonNetParam & tAudioDecNetParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncNetCfg( TMTICommonNetParam & tVideoEncNetParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncPubCfg( TMTIVideoEncPubParam & tVideoEncPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncOSDCfg( TMTIVideoEncOSDParam & tVideoEncOSDParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncMPCfg( TMTIVideoEncMPParam & tVideoEncMPParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncPollCfg( TMTIVideoEncPollParam & tVideoEncPollParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoDecNetCfg( TMTICommonNetParam & tVideoDecNetParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoDecRTCfg( TMTIVideoDecRetransferParam & tVideoDecRetransferParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoDecPubCfg( TMTIVideoDecPubParam & tVideoDecPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropWebCfg( TMTISysPubParam & tSysPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoInCfg( TMTIVideoInputParam & tVideoInputParam, u16 wConfigId, u8 * pConfData );


	u16 GetCapabilities( VSIP_IC_CAPABILITIES *pCapList, u16 wMaxCapCount); //��ȡ�豸������
	void GetCapability( VSIP_IC_CAPABILITIES *pCapList, u16 wCapCount, u8 byCapType ); //��ȡ�豸������

	//ǰ��ҵ��洢�ӿ�
	friend int StartRecReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int StopRecReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int SetTimelyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,TimelyRecInfo TRecInfo);
	friend int GetTimelyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int GetChannelRecBasicInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int GetRecSysInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
	friend int GetPeriodlyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int SetPeriodlyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,PeriodlyTimeInfo PTimeInfo);
	friend int SetChannelRecBasicInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,RecChannelBasicInfo RChannelBasicInfo);
	friend int SetRecSysInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,RecSysInfo RSysInfo);
	friend int QueryRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TQueryParam QueryParam);
    friend int QueryRecFileNumReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TQueryParam QueryParam);
	friend int StartExportRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwRecFileIndexId,u32 dwIpAddress, u16 wPort,u8 byConnectionType);
	friend int StopExportRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
	friend int RecFileOperationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byPlayerId,u8 byOperationType);
	friend int DelRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwRecFileIndexId,u8 byType);
	friend int DelRecRecordReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byType);
	friend int SeekLocationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byPlayerId, u32 dwDragTime);
	friend int GetRecStatusReqCB( VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannelId);
	friend int RecFileStartPlayReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TRecStartPlayInfo tCfgParam);
	friend int SetNmsParamReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TNmsParam NmsParam);
	friend int GetNmsParamReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
	friend int DevRegeditRspCB(VSIP_HANDLE VsipHandle,u16 wResultRsp);
	friend int MtStackOperationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,
		u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);
	//ǰ�˴洢end

	friend int AppContentTypeRspCB( VSIP_HANDLE VsipHandle, TVsipGuid SrcGuid, u8 SwitchResult );
	friend int TimerDeadCB(VSIP_HANDLE VsipHandle);
    friend int SetDevGuidReqCB(VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid);
	friend int ClientDisconnectedCB( VSIP_HANDLE VsipHandle, int nSockListNum );
	friend int ClientReConnectCB( VSIP_HANDLE VsipHandle );
	friend int DiscoveryReqCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo );
	friend int ContentTypeReqCB( VSIP_HANDLE VsipHandle, TVsipGuid SrcGuid, TVsipGuid DstGuid, u8 ContentType );
	friend int GetConfigReqCB( VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, u16 ConfigItemCount, u16 *pConfigItemId );
	friend int GetCapabilityReqCB( VSIP_HANDLE VsipHandle );
	friend int GetProprietaryReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 ConfigItemCount,
						VSIP_MEDIA_CONTROL_GETPROPRIETARY_CONFIG *pConfigItemId );
	friend int SetConfigReqCB( VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, u16 wConfigItemCount,
						VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );
	friend int SetProprietaryReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID tVsipGuid, u16 wConfigItemCount,
						VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo );

	friend int StartDeviceReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u32 TxIpAddress,
								 u16 TxPort, u16 RxPort, u8 ConnectType );

	friend int StopDeviceReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid );

	friend int SendCommandReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u8 CommandCode,
								 u32 Arg1, u32 Arg2 );

	friend int SendCommandExReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u8 CommandCode,
								   u32 Arg1, u32 Arg2, u16 AdditionalArgCount,
								   VSIP_MEDIA_CONTROL_ADDITIONAL_ARG *pAddArg);

	friend int SerialPortDataCB( VSIP_HANDLE VsipHandle, u8 * Buf, u16 BufLen );

	friend void CheckConnectionLostlist(void * pParam);

	friend int MtStackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

	friend int ClientDiscoveryRspCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo, TVsipGuid tVsipGuid,
						  u16 Fabricant, u16 Product, u8 StatusChanged );

	void AddCapabilitiesInfo( u16 wEntityNum, u8 byEntityType);

#ifdef VSIP_MULTI_CHAN_SUPPORT
	friend void GetDevMultiEncchanParamReqCB (VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid);
	friend void SetDevMultiEncchanParamReqCB (VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, TMultiChanInfo &MultiChanInfo);
#endif

public:
	u16 GetFtpPort() { return m_wFtpPort; }

private:

	u32                         m_dwRealConnIP; //�ն���MCU�Ƚ���ʵ������IP��ַ
	u16                         m_wPort;//�ն���MCU�Ƚ���ʵ������port
	u16							m_wFtpPort;//����
	pMESSAGE_CB					m_pfuncMessageCB;
	pGetGuidByIp_CB             m_pfuncGetGuidCB;
	void					*	m_pCallBackContext;
	TVsipGuid					m_tDeviceGuid;
	VSIP_HANDLE					m_tAllClientHandle;
	VSIP_HANDLE					m_tClientHandle;
	TMTICapItemInfo			*	m_aptEntityInfo[MAX_ENTITY_TOTAL_NUM];
    u16						 	m_wEntityCount; //modify  G.J. 20090903 m_byEntityCount ->m_wEntityCount
	TMTIPUCapability			m_tCapability;
	BOOL						m_bIsRebootRequired;
	VSIP_HANDLE                 m_tTransparentHandle;

	BOOL                        m_bConnThreadOn;
	u16							m_awMotEnDetAreaNum[MAX_VIDEO_INPUT_PORT_NUM];
	u16							m_awMotDeDetAreaNum[MAX_VIDEO_ENC_NUM];

	u32							m_adwConfigState[PARAM_TYPE_MAX_VALUE];

	TMTISysParam				m_tSysParam;
	TMTISysPubParam				m_tSysPubParam;
	TMTIPPPOEParam				m_tPPPOEParam;

	TMTICommonNetParam			m_atVideoEncNetParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncParam			m_atVideoEncParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncPubParam		m_tVideoEncPubParam;
	TMTIVideoEncOSDParam		m_atVideoEncOSDParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncMPParam			m_atVideoEncMPParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncInputPortParam	m_atVideoEncInputPortParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncPollParam		m_atVideoEncPollParam[MAX_VIDEO_ENC_NUM];

	TMTICommonNetParam			m_atVideoDecNetParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoDecParam			m_atVideoDecParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoDecPubParam		m_atVideoDecPubParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoDecRetransferParam	m_atVideoDecRetransferParam[MAX_VIDEO_ENC_NUM];

	TMTIMotionDetectParam    m_atDeMotionDetParamNew[MAX_VIDEO_DEC_NUM][MAX_DET_AREA_NUM];

	TMTICommonNetParam			m_atAudioEncNetParam[MAX_VIDEO_ENC_NUM];
	TMTIAudioEncParam			m_atAudioEncParam[MAX_VIDEO_ENC_NUM];
	TMTICommonNetParam			m_atAudioDecNetParam[MAX_VIDEO_ENC_NUM];
	TMTIAudioDecParam			m_atAudioDecParam[MAX_VIDEO_ENC_NUM];

	TMTISerialPortParam			m_atSerialPortParam[MAX_SERIAL_PORT_NUM];
	TMTISerialPortPubParam		m_atSerialPortPubParam[MAX_SERIAL_PORT_NUM];

	TMTIInputPinParam			m_atInputPinParam[MAX_INPUT_PIN_NUM];
	TMTIVideoInputParam			m_atVideoInputParam[MAX_VIDEO_INPUT_PORT_NUM];

	TMTIMotionDetectParam    m_atEnMotionDetParamNew[MAX_VIDEO_INPUT_PORT_NUM][MAX_DET_AREA_NUM];
    TMTIShadeParam           m_atShadeParam[MAX_VIDEO_INPUT_PORT_NUM][MAX_SHADE_AREA_NUM];
    u16				m_awShadeAreaNum[MAX_VIDEO_INPUT_PORT_NUM];

	u32 m_dwConnectionCheckTimerId;
	u32 m_dwCheckSyncTimerId;

	//ǰ�˴洢start
    TimelyRecInfo m_aTimelyRecInfo[MAX_CHANNEL_NUM];
	PeriodlyTimeInfo m_aPeriodTimeInfo[MAX_CHANNEL_NUM];
    RecChannelBasicInfo m_aRecChannelBasicInfo[MAX_CHANNEL_NUM];
    RecSysInfo m_RecSysInfo;
	RecPlayerState m_abyRecStatus[MAX_CHANNEL_NUM];
	TNmsParam m_NmsParam;
	TConnectionLostList *m_ptConnetionLostList;
	TMTIAlarmParam m_tAlarmParam;
	TMTIBannerParam m_tBannerParam;
	TMTISnapShotConfig m_aSnapShotConfig[MAX_CHANNEL_NUM];
	TMTISnapShotNamingRule m_tSnapShotNameRule;
	//ǰ�˴洢end
	TMTIMatrixInfo				m_MatrixParam;

#ifdef VSIP_MULTI_CHAN_SUPPORT
	TDevMultiChanInfo			m_tDevMultiChanInfo;
#endif
};


typedef struct TMTINetParam
{
	u32 dwIpAddr;	//IP��ַ(������)
	u16 wPort;		//�˿ں�(1024-65535)
}PACKED TMTI_NET_PARAM, *PTMTI_NET_PARAM;

class CMTIUnitManager
{
	typedef struct TMTIUnitListInfo
	{
		eUnitState		emUnitState; //�豸״̬
		TMTIDiscoveryCBInfo	tDiscoveryInfo;	//�豸ϵͳ����
	}PACKED TMTI_UNIT_LIST_INF, *PTMTI_UNIT_LIST_INF;


	/*
	typedef struct TMTIClientUnitListInfo
	{ //client
		u16             wContentTime;   //�����󽻻���·����ʱ��
		eConnState		emConnState;	//����״̬
		TVsipGuid		tDevGuid;		//�豸GUID
		VSIP_HANDLE		VsipHandle;
	}PACKED TMTI_CLIENT_UNIT_LIST_INF, *PTMTI_CLIENT_UNIT_LIST_INF;
    */
	//��·�����Ϣ
	typedef struct TMTIConnCheckInfo
	{
		u8				byCounter;		//��������
		TVsipGuid		tDevGuid;		//�豸GUID
		SOCKHANDLE      ConnSock;       //��·�׽���
	}PACKED TMTI_CONN_CHECK_INF, *PTMTI_CONN_CHECK_INF;

public:

	/*==================================================================
	����        : ���캯��
	�������˵��: ��
	����ֵ˵��  : ��
	==================================================================*/
	CMTIUnitManager();

	/*==================================================================
	����        : ��������
	�������˵��: ��
	����ֵ˵��  : ��
	==================================================================*/
	~CMTIUnitManager();

	/*==================================================================
	����        : ��ʼ��
	�������˵��: u32 dwConnIpAddr ��������IP��ַ
	              u32 dwBindIPAddr ����IP��ַ
				  u32 dwMultiIpAddr �������鲥��ַ
				  u16 wVsipPort VSIP�˿�
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	u16 Init( u32 dwConnIpAddr, u32 dwBindIPAddr, u32 dwMultiIpAddr = 0, u16 wVsipPort = DEFAULT_VSIP_PORT,u16 wMaxOnlineMtNum = MAX_UNIT_NUM);

	void Close();

	void Terminate();
    void PrintCapBuf(int i);

	void PrintUsedCapBuf();

    void PrintEntityBuf(int k);

    u32 GetMsgRcvCheckCount();

    u32 GetTimerCheckCount();

	/*==================================================================
	����        : ������Ϣ�����߳�
	�������˵��: unsigned int dwMaxNum��Ϣ���г���
				  time_t dwTimeout��Ϣ���ͳ�ʱʱ��
				  void (*pfunCB)(const void *,const void *)�д�����ʱ�Ļص�������
				                                           ��һ��ָ��ָ��TMTIMsgHandle �����������ź�ԭ��Ϣ���;��(����������Ϣ����)
														   �ڶ���ָ��ָ��pContent
				  void *pContent�ص�ʱ����ҵ���ָ��
	����ֵ˵��  : ��
	==================================================================*/
	void StartSendThread(unsigned int dwMaxNum,time_t dwTimeout,void (*pfunCB)(const void *,const void *),void *pContent);

	/*==================================================================
	����        : ������������ص�
	�������˵��: pDISCOVERY_CB pFunc ��������ص�����
				  void* pContext �����ص�������
	����ֵ˵��  : ��
	==================================================================*/
	void SetDiscoveryCallBack( pDISCOVERY_CB pFunc, void* pContext = NULL );

	void SetNewDiscoveryCallBack(pNEWDISCOVERY_CB pFunc, void* pContext = NULL );

	/*==================================================================
	����        : ע���ر�Э��ջ�ص�
	����ֵ˵��  : ��
	==================================================================*/

    void SetTerminateMTCallBack(pTERMINATED_CB pFunc);
	/*==================================================================
	����        : �����ն˽����ص�
	�������˵��: pCONNECTED_CB pFunc �ն˽����ص�����
				  void* pContext �ص�����������
	����ֵ˵��  : ��
	==================================================================*/
	void SetConnectedCallBack( pCONNECTED_CB pFunc, void* pContext = NULL );
    /*==================================================================
	����        : ������·���ص�
	�������˵��: pCONNECTIONCHECK_CB pFunc ��·���ص�����
				  void* pContext �ص�����������
	����ֵ˵��  : ��
	==================================================================*/
    void SetConnectionCheckCallBack( pCONNECTIONCHECK_CB pFunc, void* pContext = NULL );
    /*==================================================================
	����        : ��·���
	�������˵��: guid

	����ֵ˵��  : �ɹ�����MTI_SUCCESS,ʧ�ܷ���MTI_FAIL
	==================================================================*/
    BOOL ConnectionCheck(TVsipGuid tVsipGuid);

	BOOL ConnectionCheck(TVsipGuid tVsipGuid, u16 wInstanceId);

	/*==================================================================
	����        : ������·�����ʱ����
	�������˵��: u32 wCheckInterval ��·�����ʱ��(��λms)
				  u8 byDisconnectTime ����ȷ�ϴ���
	����ֵ˵��  : ��
	==================================================================*/
	void SetConnectionCheckInterval( u32 dwCheckInterval = 30000, u8 byDisconnectTime = 3 );

	/*==================================================================
	����        : �����豸
	�������˵��: TMTINetParam tDiscoveryDest[] ����Ŀ��IP��ַ��
				  u16 wDestCount ����Ŀ��IP��ַ����
				  u16 wDiscoveryTimeout �����ȴ���ʱʱ��(����)
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	u16 Discover( TMTINetParam tDiscoveryDest[], u16 wDestCount, u16 wDiscoveryTimeout = 5000 );

	u16 GetGuid(u32 dwIp);

	/*==================================================================
	����        : ����ȫ���豸
	�������˵��: pDISCOVERY_CB pFuncCB ��������ص�����
				  u16 wDiscoveryTimeout �����ȴ���ʱʱ��(����)
	����ֵ˵��  : MTI_SUCCESS-�ɹ�; MTI_FAIL-ʧ��
	==================================================================*/
	u16 DiscoverAll( pDISCOVERY_CB pFuncCB, u16 wDiscoveryTimeout = 5000, void * pContext = NULL );

	/*==================================================================
	����        : ����豸�Ƿ�����
	�������˵��: u32 dwUnitIpAddr �豸IP��ַ
	����ֵ˵��  : TRUE-����; FALSE-������
	==================================================================*/
	BOOL IsUnitOnline( u32 dwUnitIpAddr);

	/*==================================================================
	����        : ��ȡ�豸
	�������˵��: const TVsipGuid & tDevGuid �豸GUID
	����ֵ˵��  : �����豸����ָ��, ʧ�ܷ���NULL
	==================================================================*/
	CMTIUnitCtrl * GetUnitCtrl( const TVsipGuid & tDevGuid);
	//zlq add
	//CMTIUnitCtrl * GetUnitCtrl();
	//add end

	//daiyi add start for "app create connection"
	CMTIUnitCtrl * GetUnitCtrl(u32 dwIpAddr, u16 wPort);
	//daiyi add end for for "app create connection"
	/*==================================================================
	����        : ɾ���豸
	�������˵��: const TVsipGuid & tVsipGuid �豸GUID
	����ֵ˵��  : TRUE-�ɹ�; FALSE-ʧ��
	==================================================================*/
	BOOL DelUnitCtrl( const TVsipGuid & tVsipGuid);
	BOOL DelUnitCtrl( const TVsipGuid & tVsipGuid, u16 wInstId);
    BOOL ClearUnitCtrl( const TVsipGuid & tVsipGuid);
    /*==================================================================
	����        : �ж�ĳ�豸�Ƿ����豸���Ʊ��У�������������������δ��ʼ��
	�������˵��: const TVsipGuid & tVsipGuid �豸GUID
	����ֵ˵��  : TRUE-��; FALSE-��
	==================================================================*/
	BOOL DevIsInUnitCtrlInfo(const TVsipGuid & tVsipGuid);

	/*==================================================================
	����        : �Ƿ���������
	�������˵��: ��
	����ֵ˵��  : TRUE-��������; FALSE-δ������
	==================================================================*/
	BOOL IsDiscovering( ){ return m_bIsDiscovering;}

	/*=============================================================================
	�� �� ��:  PrintDiscovered
	��    ��:  ��ӡҵ��������Ϣ
	�� �� ֵ:
	==============================================================================*/
	void PrintDiscovered();

	/*=============================================================================
	�� �� ��:  PrintDiscoverAll
	��    ��:  ��ӡOS������Ϣ
	�� �� ֵ:
	==============================================================================*/
	void PrintDiscoverAll();

	/*=============================================================================
	�� �� ��:  PrintUnitCapability
	��    ��:  ��ӡ�ն���������Ϣ
	�� �� ֵ:
	==============================================================================*/
	void PrintUnitCapability();
	void PrintUnitCapability(const TVsipGuid tVsipGuid);
	/*=============================================================================
	�� �� ��:  PrintUnitEntityinfo
	��    ��:  ��ӡ�ն���������Ϣ
	�� �� ֵ:
	==============================================================================*/
	void PrintUnitEntityinfo();
	void PrintUnitEntityinfo(const TVsipGuid tVsipGuid);
	/*=============================================================================
	�� �� ��:  PrintUnitCtrlInfo
	��    ��:  ��ӡ�豸���ƾ����Ϣ
	�� �� ֵ:
	==============================================================================*/
	void PrintUnitCtrlInfo();
	void PrintStackConnection(int index);
	void PrintConnectionInfo();
	void PrintConListInfo(u16 wIndex=0, u16 wRange=0, u8 byLevel=0);
	void PrintUnitListInfo(u16 wIndex=0, u16 wRange=0, u8 byLevel=0);
	/*=============================================================================
	�� �� ��:   PrintUnitInfo
	��    ��:  ��ӡ�豸������Ϣ
	�� �� ֵ:
	==============================================================================*/
	void PrintUnitInfo();


	/*=============================================================================
	�� �� ��:  PrintClientInfo
	��    ��:  ��ӡ���������豸��Ϣ
	�� �� ֵ:
	==============================================================================*/
	void PrintClientUnitInfo();

	/*=============================================================================
	�� �� ��:  ClearClientUnitInfo
	��    ��:  ��Ϣ�����߳̽��մ������ʱѰ�Ҳ�������������ն���Ϣ
	�� �� ֵ:
	==============================================================================*/
	void ClearClientUnitInfo(VSIP_HANDLE tVsipHandle);


	/*=============================================================================
    �� �� ��:  GetExistUnitCtrl
    ��    ��:  ��ȡ�Ѵ��ڵ��豸����ָ��
    �� �� ֵ:
    ==========================================================================*/
#ifdef VSIP_PERFORM_IMPROVE
    CMTIUnitCtrl * GetExistUnitCtrl( const VSIP_HANDLE & VsipHandle);
#else
    CMTIUnitCtrl * GetExistUnitCtrl( const TVsipGuid & tGuid);
#endif
//#ifndef _LINUXEQUATOR_
	u16 SendUserUdpMSg(u32 dwDstIpaddr, u16 wDstPort, u8 byType, char *pbyBuf, u16 wpbyBufLen);
//#endif
	void SetIsPuiRegCmuOK(BOOL32 bOK);
protected:

	void AddClientUnit( const VSIP_HANDLE & VsipHandle );
	void SendDiscoveryResult( );
	void ClearDiscoveryResult( );

	BOOL AddDiscoveredUnit( TVsipGuid tVsipGuid, u32 dwUnitIpAddr, u16 wVsipPort, u16 wProduct = 0 );
	void CheckPingRespMsg( u8 * pBuf, u16 wBufLen);
	BOOL CheckConfigRespMsg( TVsipGuid tVsipGuid, u16 wConfigCount, VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );
	friend int DevDiscoveryRspCB( u8 DevTypeId, u32 DevIpAddr, void * pContext );
    friend void CheckSyncObjectCB(void * pParam);
	friend void DiscoveryTimeoutCB(void * pParam);
	friend void DiscoveryAidTimeoutCB(void * pParam);
	friend void DevDiscoveryTimeoutCB(void * pParam);
	friend void ConnectionCheckCB(void * pParam);
	friend int ClientConnectedCB( const VSIP_HANDLE & VsipHandle );

	friend int ClientContentTypeReqCB( VSIP_HANDLE VsipHandle, TVsipGuid SrcGuid,
									   TVsipGuid DstGuid, u8 ContentType );

	friend int ClientDiscoveryReqCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo );

	friend int DiscoveryRspCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo,
							   TVsipGuid tVsipGuid, u16 Fabricant, u16 Product, u8 StatusChanged );

	friend int ContentTypeRspCB( VSIP_HANDLE VsipHandle, TVsipGuid SrcGuid, u8 SwitchResult );

	friend int GetCapabilityRspCB( VSIP_HANDLE VsipHandle, u16 CapListCount,
								   VSIP_IC_CAPABILITIES *pCapList );

	friend int GetConfigRspCB( VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, u16 ConfigItemCount,
							   VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );

	friend int GetProprietaryRspCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 ConfigItemCount,
									 VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo );

	friend int OperationRspCB( VSIP_HANDLE VsipHandle, u16 ErrorCode );

	friend int StackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

	friend int EventNotifyCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 EventType, u32 EventArg );

	friend int EventNotifyExtendCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid,unsigned short EventType,u16 wpbyBufLen,u8 *pbyBuf);

	friend int DisconnectedCB( VSIP_HANDLE VsipHandle, int nSockListNum );

	friend int CommandExReqCB(VSIP_HANDLE VsipHandle, VSIP_GUID tVsipGuid, u8 CommandCode, u32 Arg1,
		u32 Arg2, u16 AdditionalArgCount, VSIP_MEDIA_CONTROL_ADDITIONAL_ARG *pAddArg);

	//kedacom potocal begin
	friend int GetDecoderStyleCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	friend int GetCustomExtCapsCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	//kedacom potocal end
private:

	BOOL bIsInited;
	u32					m_dwLocalIpAddr;//����IP��ַ
	u16					m_wVsipPort;	//VSIP�˿�
	TMTIUnitListInfo	m_atUnitInfo[MAX_UNIT_NUM];	//�豸��Ϣ

#ifndef VSIP_PERFORM_IMPROVE	//zhuzhm:��·������UNITCTRL�������
	TMTIConnCheckInfo	m_atConnCheckInfo[MAX_UNIT_NUM];//��·�����Ϣ
#endif

	TMTIDiscoveryCBInfo	m_atDevDiscvResult[MAX_UNIT_NUM];
	pDISCOVERY_CB		m_pfuncDevDiscvCB;
	void *				m_pDevDiscvCBContext;
	u32					m_dwDevDiscvCBTimerId;
	u32					m_dwDevDiscvCount;
	u32					m_dwLastDevDiscvCount;

	pDISCOVERY_CB	m_pfuncUnitListCallBack;
	void *			m_pUnitListCallBackContext;

	pNEWDISCOVERY_CB m_pNewDiscoveryCB;
	void *			 m_pNewDiscoveryContext;

	pCONNECTED_CB	m_pfuncUnitConnectedCallBack;
	void *			m_pUnitConnectedCallBackContext;

    pCONNECTIONCHECK_CB m_pfuncConnectionCheckCallBack;
    void *          m_pConnectionCheckContext;

	u32             m_dwMsgTimerId;
	u32				m_dwConnCheckCBTimerId;
	BOOL			m_bIsConnCheckCBEnable;
	u8				m_byDisconnectTime;

	BOOL			m_bIsDiscovering;
	BOOL			m_bIsDiscoverAllUnit;
	TMTINetParam	m_atDiscoveryDest[MAX_UNIT_NUM];
	u16				m_wDiscoveryDestCount;
	u32				m_dwDiscoveryCBTimerId;
	u16             m_wMaxOnlineMtNum;
	u32             m_dwDiscoveryCBAidTimerId;
	u32             m_wAidTimerout;
	u32             m_dwCheckSyncObjectTimerId;
};

//070813 zhuzhm add begin : for update
// ��������
#define VSIP_UPDATE_TYPE_INVALID		0   // ��Ч��������
#define VSIP_UPDATE_TYPE_FTP			1	// ��׼ftp
#define VSIP_UPDATE_TYPE_USERDEF		2	// �Զ���

#define MAX_UPDATE_FILE_NUM                 16
#define UPDATE_FILE_STRING_LEN              32

typedef struct VsipUpdateFileInfo
{
    char achName[UPDATE_FILE_STRING_LEN+1];  // �����ļ���
    char achPath[UPDATE_FILE_STRING_LEN+1];	 // �����ļ����·��
}VSIP_UPDATE_FILE_INFO;

typedef struct VsipUpdateInfo
{
    u8 byPolice;     				// ��������
    u8 byFileNum;			// �����ļ�����������ṹ����Ч������
    VSIP_UPDATE_FILE_INFO FileInfo[MAX_UPDATE_FILE_NUM];	// ��Ӧ�����ļ���Ϣ
}VSIP_UPDATE_INFO;

s32 DeviceUpdateInfo (s8 *DeviceType, VSIP_UPDATE_INFO &Info);
// zhuzhm add end

//������Ϣ�ӿں���
API void mtiver();
API void mtihelp();
API void mtilogall();
API void mtilogon();
API void mtilogoff();
API void mtidebuglogon();
API void mtidebuglogoff();
API void mtisockdataon();
API void mtisockdataoff();
API void mtishowsockinfo();
API void mtishowcapability();
API void mtimiddlelogon();
API void mtimiddlelogoff();
API void mtitimeron();
API void mtitimeroff();
API void mtishowtimerstate();
API void vidparamoff();
API void vidparamon();

int GetRecStatusRspCB( VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannelId,RecPlayerState byRecStatus);
int GetNmsParamRspCB( VSIP_HANDLE VsipHandle,VSIP_GUID guid,TNmsParam NmsParam);
int GetTimelyRecInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,TimelyRecInfo TRecInfo);
int GetPeriodlyRecInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,PeriodlyTimeInfo PTimeInfo);
int GetChannelRecBasicInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,RecChannelBasicInfo & RChanBasicInfo);
int GetRecSysInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,RecSysInfo RSysInfo);
int QueryRecFileAckCB(VSIP_HANDLE VsipHandle, QueryRecFileInfo QRecFileinfo);
int QueryRecListRspCB(VSIP_HANDLE VsipHandle, TMTIRecListQryRsp tRecListRsp);
int QueryRecFileNumAckCB(VSIP_HANDLE VsipHandle, u16 wNum);
int ProgressCallCB(VSIP_HANDLE VsipHandle, ProgressParaInfo ParaInfo);
int RecStatusChangeCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannel,RecPlayerState byRecStatus);
int RecFileStartPlayidCB(VSIP_HANDLE VsipHandle,TMTIRecFilePlayNtf tPlayNtf);
int FileExportProgressCB(VSIP_HANDLE VsipHandle,ProgressParaInfo TProgress);
int VideoSourceStateCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannel,u8 byState);
int DevRegeditReqCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,TDevRegeditInfo DevRegInfoPara);
int StackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

int RecFileDownNotifyCB(VSIP_HANDLE VsipHandle,u16 wErrorCode,u16 wFtpPort);
int PlayGpsDataCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwErrcode,u8 byPlayId,u32 dwReserve);
int PlayGpsDataNotifyCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TPlayGpsNotify tPlayGpsDataNotify);

int StackTransSubsRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TTransSubsRsp &tReq);
int StackTransSubsNtfCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TTransSubsNtf &tNtf);

int SetTransDataAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int GetTransDataAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int GetTransCfgAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int StackOperationRspExCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int StackOperationRspEx2CB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTITransDataRsp tMTITransDataRsp);
int StackOperationNtfEx2CB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTITransDataNtf tMTITransDataNtf);
int GetCuOperationCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int NotifyCuOperationCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int DevStreamPropRsp(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTIStartDevRspEx tMTIStreamProps);

//////////////////////////////////////////////////////////////////////////
//NA-PU
//ʵʱ���
int NAStartSendMediaRspCB(VSIP_HANDLE VsipHandle, TMTIStartSendMediaRsp& tRsp);
//��Ƶ���к�ǰ�˽���
int NAStartRecvMediaRspCB(VSIP_HANDLE VsipHandle, TMTIStartRecvMediaRsp& tRsp);
//¼�񲥷�
int NARecStartPlayRspCB(VSIP_HANDLE VsipHandle, TMTIRecStartPlayRsp& tRsp);
//¼������
int NARecStartExportRspCB(VSIP_HANDLE VsipHandle, TMTIRecStartExportRsp& tRsp);

void ConnectionCheckCB(void * pParam);
void DiscoveryTimeoutCB(void * pParam);
void DiscoveryAidTimeoutCB(void * pParam);
void DevDiscoveryTimeoutCB(void * pParam);

void CheckConnectionLostlist(void * pParam);

int StartRecReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int StopRecReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int SetTimelyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,TimelyRecInfo TRecInfo);
int GetTimelyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int GetChannelRecBasicInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int GetRecSysInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
int GetPeriodlyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int SetPeriodlyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,PeriodlyTimeInfo PTimeInfo);
int SetChannelRecBasicInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,RecChannelBasicInfo RChannelBasicInfo);
int SetRecSysInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,RecSysInfo RSysInfo);
int QueryRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TQueryParam QueryParam);
int QueryRecFileNumReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TQueryParam QueryParam);
int StartExportRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwRecFileIndexId,u32 dwIpAddress, u16 wPort,u8 byConnectionType);
int StopExportRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
int RecFileOperationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byPlayerId,u8 byOperationType);
int DelRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwRecFileIndexId,u8 byType);
int DelRecRecordReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byType);
int SeekLocationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byPlayerId, u32 dwDragTime);
int GetRecStatusReqCB( VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannelId);
int RecFileStartPlayReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TRecStartPlayInfo tCfgParam);
int SetNmsParamReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TNmsParam NmsParam);
int GetNmsParamReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
int DevRegeditRspCB(VSIP_HANDLE VsipHandle,u16 wResultRsp);
int MtStackOperationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

int TimerDeadCB(VSIP_HANDLE VsipHandle);
int MtStackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

void SendNATOnce(void *pSendNATInfo);

#ifdef _LINUX_
void CheckContentRsp(void *pArg);
#endif

#ifdef WIN32
API int GetVmtNum();
#endif // WIN32


#ifdef WIN32
#pragma pack( pop )
#endif //WIN32
#endif //MT_CTRL_H


