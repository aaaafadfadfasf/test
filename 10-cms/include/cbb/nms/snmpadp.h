/*=======================================================================
ģ����      :SNMP����ģ��
�ļ���      :snmpadp.h
����ļ�    :snmpadp.cpp
�ļ�ʵ�ֹ��� : ʵ��ǰ�˼�ƽ̨�豸�����ܷ�����֮�����Ϣ������

   �ṹ�ֽ���ת��ע��㣨ע����־�Ƶ����˿��ʵ�֣�������ڲ���δ����ת��Ľӿڣ�
      1. ǰ�˼�ƽ̨ҵ�񲻹����ֽ�������⡣���е�ת�������ɴ������ɣ��ϲ�ҵ��
         ȫ��ʹ��������
      2. ����ֻ�������շ��Ľӿڴ������ֽ���ת���ӿڣ�����ʱ��һ�ɲ��ñ�����

����        : ��  ��
�汾        : V0.2  Copyright(C) 2005-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2005/12/22  0.1         ��  ��      ����
2006/02/23  0.2         ���ǿ      ���ݼ��3.9����ƽ��е���
2011/11/05  0.2         ��־��      �����eventid.h������(ֱ������ָ����AppID��EventID��Χ)
2011/11/11  0.2         ��־��      osp.h�޸ĺ�������Ҫ��ʽ����cstdlib��ctime
2013/03/25  0.2         ��־��      ƽ̨2.0�����������
=======================================================================*/
#ifndef _SNMPADP_H_
#define _SNMPADP_H_
/**
 * @defgroup SNMP����ģ��
 * @{
 */
/** \file
*   �豸������ӿڵ�ͷ�ļ�
*/
#include "cbb/osp/osp.h"
#include <cstdlib>
#include <ctime>

#ifdef WIN32
#pragma pack( push, 1 )
#define PACKED 
#endif //WIN32

#ifdef _VXWORKS_
#ifndef PACKED
#define PACKED __attribute__((__packed__))
#endif // PACKED
#endif //_VXWORKS_

#ifdef _LINUX_
#ifndef PACKED
#ifndef _EQUATOR_
#define PACKED  __attribute__((packed))
#else 
#define PACKED
#endif //EQUATOR_
#endif // PACKED 
#endif //_LINUX_

//����AppId
#define AID_TNMS_BGN  11
#define AID_TNM_SNMPADP_CAGTSSN      (AID_TNMS_BGN + 6)
#define AID_TNM_SNMPADP_SAGTSSN      (AID_TNMS_BGN + 7)
/*�豸������NMS�ӿ���Ϣ��52001-53000��*/
#define EV_DEVAGT_NMS_BGN       52001
#define EV_DEVAGT_NMS_END       53000


//SNMP General Errors / Reasons Value Description
#define SNMP_SUCCESS              (u16)0    //Success Status
#define SNMP_FINISHED             (u16)1    //Too Big Data, Finished
#define SNMP_NOFINISHED           (u16)2    //Too Big Data, Not Finished
#define SNMP_GENERAL_ERROR        (u16)3    //General Error
#define SNMP_RESOURCE_UNAVAIL     (u16)4    //New or malloc Failed
#define SNMP_INTERNAL_ERROR       (u16)5    //Unexpected internal error
#define SNMP_UNSUPPORTED          (u16)6    //Unsupported function

//Callback Reasons
#define SNMP_TIMEOUT              (u16)10   //Outstanding request timed out
#define SNMP_NOTIFICATION         (u16)11   //Received notification (trap/inform)
#define SNMP_ASYNC_GETRESPONSE    (u16)12   //Received get-response
#define SNMP_ASYNC_SETRESPONSE    (u16)13   //Received set-response
#define SNMP_ASYNC_RESPONSE_ERROR (u16)14   //Received response error
#define SNMP_SESSION_DESTROYED    (u16)15   //Snmp Object destroyed
//Snmp Class Errors
#define SNMP_INVALID              (u16)20   //Snmp member function called on invalid instance
#define SNMP_INVALID_PDU          (u16)21   //Invalid Pdu passed to mf
#define SNMP_INVALID_TARGET       (u16)22   //Invalid target passed to mf
#define SNMP_INVALID_CALLBACK     (u16)23   //Invalid callback to mf
#define SNMP_INVALID_REQID        (u16)24   //Invalid request id to cancel
#define SNMP_INVALID_NOTIFYID     (u16)25   //Missing trap/inform Oid
#define SNMP_INVALID_OPERATION    (u16)26   //Snmp operation not allowed for specified target
#define SNMP_INVALID_OID          (u16)27   //Invalid Oid passed to mf
#define SNMP_INVALID_ADDRESS      (u16)28   //Invalid address passed to mf
#define SNMP_ERR_STATUS_SET       (u16)29   //Agent returned response Pdu with error status set
#define SNMP_TL_UNSUPPORTED       (u16)30   //Transport not supported
#define SNMP_TL_IN_USE            (u16)31   //Transport in use
#define SNMP_TL_FAILED            (u16)32   //Transport Failed

#define SNMP_ERR_TOO_BIG          (u16)50   //Pdu Too Big, see error index
#define SNMP_ERR_NO_SUCH_NAME     (u16)51   //No Such Variable Binding name, see returned error index
#define SNMP_ERR_BAD_VALUE        (u16)52   //Bad Variable Binding Value, see returned error index
#define SNMP_ERR_READ_ONLY        (u16)53   //Variable Binding is read only, see returned error index
#define SNMP_ERR_GENERAL_VB_ERR   (u16)54   //General Variable Binding error, see returned error index
#define SNMP_ERR_NO_ACCESS        (u16)55   //Operation Failure, No Access
#define SNMP_ERR_WRONG_TYPE       (u16)56   //Operation Failure, Bad Type
#define SNMP_ERR_WRONG_LENGTH     (u16)57   //Operation Failure, Bad Length
#define SNMP_ERR_WRONG_ENCODING   (u16)58   //Operation Failure, Wrong Encoding
#define SNMP_ERR_WRONG_VALUE      (u16)59   //Operation Failure, Wrong Value
#define SNMP_ERR_NO_CREATION      (u16)60   //Operation Failure, No Creation
#define SNMP_ERR_INCONSIST_VAL    (u16)61   //Operation Failure, Inconsistent Value
#define SNMP_ERR_RESOURCE_UNAVAIL (u16)62   //Operation Failure, Resource Unavailable
#define SNMP_ERR_COMITFAIL        (u16)63   //Operation Failure, Commit Failure
#define SNMP_ERR_UNDO_FAIL        (u16)64   //Operation Failure, Undo Failure
#define SNMP_ERR_AUTH_ERR         (u16)65   //Operation Failure, Authorization Error
#define SNMP_ERR_NOT_WRITEABLE    (u16)66   //Operation Failure, Not Writable
#define SNMP_ERR_INCONSIS_NAME    (u16)67   //Operation Failure, Inconsistent Name


#define TNM_DAYS_OF_WEEK                    7       //һ�ܵ�����
#define TNM_MAX_TIME_STRING_LEN             16      //���ʱ���ַ�������
#define TNM_MAX_SERIALPORT_DATA_LEN         1024    //������ݳ���
#define TNM_MAX_STRING_LEN                  16      //����ַ�������
#define TNM_MAX_VERSION_LEN                 32      //���汾�ַ�������
#define TNM_MAX_VERSION_PATH_LEN            128     //���汾·���ַ�������
#define TNM_MAX_USER_NUM                    256     //����û���Ŀ
#define TNM_MAX_VIDEOIN_PORT_NAME_LEN       8       //�����Ƶ����˿�������
#define TNM_MAX_VIDEOIN_PORT_NUM            32      //�����Ƶ����˿�����
#define TNM_MAX_PIC_REGION_NUM              16      //�໭�����������
#define TNM_MAX_POLL_PORT_NUM               4       //��ѯ���˿���
#define TNM_MAX_NET_ADAPTER_NUM             10      //֧�ֵ�������Ŀ
#define TNM_MAX_PANELLED_LEN                16      //������Ƹ���
#define MAX_BOX_ID_LEN                      32      //�������ʶ������
#define TNM_MAX_DEVTYPE_NAME_LEN            31      //�����豸���Ƴ���
#define TNM_MAX_CAMERA_NAME_LEN             32      //��������ͷ��

#define TNM_MAX_ETHERNET_NUM                5       //�����̫������
#define TNM_MAX_FAN_NUM                     5       //��������
#define TNM_MAX_POWER_NUM                   5       //����Դ��
#define TNM_MAX_VTDU_NUM                    16      //���VPU����
#define TNM_MAX_VPUDECCHN_NUM               16      //���VPU����ͨ����

#define TNM_MAX_CHAN_NUM                    16      //���ͨ����
#define TNM_MAX_MAP_NUM                     5       //���MAP��
#define TNM_MAX_VIDEOSRC_NUM                8       //�����Ƶ��
#define TNM_MAX_LPTCHAN_NUM                 8       //��󲢿���

#define TNM_MAX_RECFILEPATHNAME_LEN         128     //���¼���ļ�·��������
#define TNM_MAX_DBPROVIDER_LEN              64      //���OLEDB �ṩ��������
#define TNM_MAX_DBNAME_LEN                  64      //������ݿ�������

#define TNM_MAX_CU_LOG_NUM                  10      //��CU������������־��Ŀ��
#define TNM_MAX_CU_LOG_LEN                  199     //��CU������־��Ŀ��󳤶�

#define TNM_MAX_COMMUNITY_LEN               32

#define TNM_MAX_FIELD_NAME_LEN              32      // ������ID����
#define TNM_MAX_ALIAS_NAME_LEN              32      // ���ı�������
#define TNM_MAX_DEVICE_ID_LEN               32      // �����豸ID����
#define TNM_MAX_USER_ID_LEN                 32      // �����û���¼ID����
#define TNM_MAX_PASSWORD_LEN                32      // ����û����볤��
#define TNM_CMS_ID_LEN                      18      // CMS��ID��ų���

#define TNM_MAX_FIELD_ID_LEN                32
#define TNM_MAX_USER_NAME_LEN               32

#define TNM_MAX_COMPILE_TIME_LEN            32      // ����ʱ���ִ�����󳤶�

// 3A������Ϣ���ȶ���
#define TNM_MAX_DEV_NAME_LEN                32     // �豸����
#define TNM_MAX_DEV_MODEINFO_LEN            16     // �豸�ͺ�
#define TNM_MAX_DEV_NETADDR_LEN             32     // �豸�����ַ��IP������

// ����Ϣ����
#define SUBEV_TYPE_INVALID              0           // ��Чֵ
#define SUBEV_TYPE_REQ                  1           // ������
#define SUBEV_TYPE_ACK                  2           // ȷ����
#define SUBEV_TYPE_NACK                 3           // �ܾ���
#define SUBEV_TYPE_NOTIFY               4           // ֪ͨ��
#define SUBEV_TYPE_FINISH               5           // ������Ϣ���ȷ����

#ifndef DEFAULT_WRITE_COMMUNITY
#define DEFAULT_WRITE_COMMUNITY             "private"
#endif
#ifndef DEFAULT_READ_COMMUNITY
#define DEFAULT_READ_COMMUNITY              "public"
#endif

#define TNM_DEFAULT_LISTENPORT              1727    //����Ĭ�ϼ����˿�

//��Ϣ���͵�Э������
#define PROTO_TCP                           0x00000000
#define PROTO_UDP                           0xF0000000
//Э��MASK
#define PROTO_TCP_MASK                      0xF0000000
#define PROTO_UDP_MASK                      0xF0000000

//��������
#define TNM_CONNECTION_NONE                 0x00
#define TNM_CONNECTION_UNICAST              0x01
#define TNM_CONNECTION_MULTICAST            0x02
#define TNM_CONNECTION_BROADCAST            0x03
#define TNM_CONNECTION_TCPSERVER            0x04
#define TNM_CONNECTION_TCPCLIENT            0x05
#define TNM_CONNECTION_TCP                  0x06
#define TNM_CONNECTION_SSL                  0x07

//��Ƶ���ݸ�ʽ
#define TNM_VIDEO_TYPE_NONE                 0x00
#define TNM_VIDEO_TYPE_SN4                  0x01
#define TNM_VIDEO_TYPE_MPEG4                0x02
#define TNM_VIDEO_TYPE_H261                 0x03
#define TNM_VIDEO_TYPE_H263                 0x04
#define TNM_VIDEO_TYPE_H264                 0x05
#define TNM_VIDEO_TYPE_H262             0x06    /*H.262 (MPEG-2)*/
#define TNM_VIDEO_TYPE_H263PLUS            0x07    /*H.263+*/

//��Ƶ���ݸ�ʽ
#define TNM_AUDIO_TYPE_NONE                 0x00
#define TNM_AUDIO_TYPE_PCMA                 0x01
#define TNM_AUDIO_TYPE_PCMU                 0x02
#define TNM_AUDIO_TYPE_GSM                  0x03
#define TNM_AUDIO_TYPE_G729                 0x04
#define TNM_AUDIO_TYPE_MP3                  0x05
#define TNM_AUDIO_TYPE_ADPCM                0x08

#define TNM_AUDIO_TYPE_G721                 0x09    /*G.721*/
#define TNM_AUDIO_TYPE_G722                 0x0A    /*G.722*/
#define TNM_AUDIO_TYPE_G7231                0x0B    /*G.7231*/
#define TNM_AUDIO_TYPE_G728                 0x0C    /*G.728*/
#define TNM_AUDIO_TYPE_G7221                0x0D    /*G.7221*/
#define TNM_AUDIO_TYPE_G7221C               0x0E    /*G.7221C*/

//ͼ��ֱ���
#define TNM_VIDEO_RESOLUTION_NONE           0x00
#define TNM_VIDEO_RESOLUTION_AUTO           0x01
#define TNM_VIDEO_RESOLUTION_QCIF           0x02
#define TNM_VIDEO_RESOLUTION_CIF            0x03
#define TNM_VIDEO_RESOLUTION_2CIF           0x04
#define TNM_VIDEO_RESOLUTION_4CIF           0x05

//ͼ������
#define TNM_VIDEO_SET_NONE                  0x00 //��֧��ͼ����������
#define TNM_VIDEO_QUALITY_PRIOR             0x01 //��������
#define TNM_VIDEO_SPEED_PRIOR               0x02 //�ٶ�����

//ͼ�������(0��3��,0��ʾû��)
#define TNM_VIDEO_DEALLEV_NONE              0x00
#define TNM_VIDEO_DEALLEV_ONE               0x01
#define TNM_VIDEO_DEALLEV_TWO               0x02
#define TNM_VIDEO_DEALLEV_THREE             0x03

//�໭����
#define TNM_VIDEO_FORMAT_NONE               0x00 //�޶໭��
#define TNM_VIDEO_FORMAT_PIP_4              0x01 //���л�(����1:4)
#define TNM_VIDEO_FORMAT_PIP_9              0x02 //���л�(����1:9)
#define TNM_VIDEO_FORMAT_PIP_16             0x03 //���л�(����1:16)

//��������
#define TNM_SERIAL_LINE_DRIVER_NONE         0x00
#define TNM_SERIAL_LINE_DRIVER_RS232        0x01
#define TNM_SERIAL_LINE_DRIVER_RS422        0x02
#define TNM_SERIAL_LINE_DRIVER_RS485        0x03

//��Ƶ��������
#define TNM_AUDIO_INPUTTYPE_LINEIN          0x00
#define TNM_AUDIO_INPUTTYPE_MIC             0x01
//��Ƶ��������
#define TNM_CAP_AUDIO_INPUT_LINEIN          0x01    //linein
#define TNM_CAP_AUDIO_INPUT_MIC             0x02    //mic

//��������ͨ������
#define TNM_SERIAL_STREAM_TYPE_NONE         0x00 //��ʹ��
#define TNM_SERIAL_STREAM_TYPE_UDP          0x01 //UDPͨ��
#define TNM_SERIAL_STREAM_TYPE_TCP          0x02 //TCPͨ��
#define TNM_SERIAL_STREAM_TYPE_VSIP         0x03 //��Ϣͨ��

//��������Э������
#define TNM_SERIAL_DATA_TYPE_NONE           0x00 //͸��ͨ��
#define TNM_SERIAL_DATA_TYPE_VSIP           0x01 //VSIPЭ��

//������������
#define TNM_MEDIA_STREAM_TYPE_UDP           0x00 //UDPͨ��
#define TNM_MEDIA_STREAM_TYPE_TCP           0x01 //TCPͨ��

//����Ƶʱ����ʾ
#define TNM_STARVE_MODE_COLOR_BAR           0x00 //��ɫ��
#define TNM_STARVE_MODE_LAST_IMAGE          0x01 //��Ļ����
#define TNM_STARVE_MODE_BLACK_SCR           0x02 //����
#define TNM_STARVE_MODE_SHOW_OSD            0x03 //��Ļ���Ტ��Ļ��ʾ

//��Ƶ����˿�����
#define TNM_VIDEO_OUT_TYPE_C                0x01 // ����C����
#define TNM_VIDEO_OUT_TYPE_S                0x02 // S����
#define TNM_VIDEO_OUT_TYPE_VGA              0x03 // VGA���

//�澯���״̬
#define TNM_ALARM_DETECT_NONE               0x00 //�����
#define TNM_ALARM_DETECT_ONCE               0x01 //���һ������
#define TNM_ALARM_DETECT_REPEAT             0x02 //ѭ�����
#define TNM_ALARM_DETECT_ALWAYS             0x03 //ʼ�ռ��

//����ͷ����
#define TNM_CAMERA_TYPE_SONY                0x01    // ���ᣨSONY��
#define TNM_CAMERA_TYPE_AD                  0x02    // AD
#define TNM_CAMERA_TYPE_PELCO               0x03    // PELCO
#define TNM_CAMERA_TYPE_PIH                 0x04    // PIH
#define TNM_CAMERA_TYPE_PELCO_P             0x05    // PELCO_P
#define TNM_CAMERA_TYPE_PARCO               0x06    // �ظߣ�PARCO��
#define TNM_CAMERA_TYPE_AB                  0x07    // ���ȣ�AB��
#define TNM_CAMERA_TYPE_YUNTAI              0x08
#define TNM_CAMERA_TYPE_SAMSUNG             0x09    // ���ǣ�SAMSUNG��
#define TNM_CAMERA_TYPE_YAAN                0x010   // �ǰ�
#define TNM_CAMERA_TYPE_AD168               0x011   // AD168
#define TNM_CAMERA_TYPE_KALATEL             0x012   // ��������KALATEL��
#define TNM_CAMERA_TYPE_JN                  0x013 
#define TNM_CAMERA_TYPE_WV_CS650            0x014   // ���£�CS650����850����Э��
#define TNM_CAMERA_TYPE_WV_CS850            0x015   // ���£�CS850����Э��
#define TNM_CAMERA_TYPE_ALEC                0x016   // �����ˣ�ALEC��
#define TNM_CAMERA_TYPE_AB_P                0x017
#define TNM_CAMERA_TYPE_AB_D                0x018




//VGA���ˢ��Ƶ��
#define TNM_VGA_OUT_FREQUENCY_60            0x01 //ˢ��Ƶ��60
#define TNM_VGA_OUT_FREQUENCY_75            0x02 //ˢ��Ƶ��75
#define TNM_VGA_OUT_FREQUENCY_85            0x03 //ˢ��Ƶ��85

//�豸���������
#define TNM_NONE                            0x0  //��Ƶ����붼��֧��
#define TNM_ENCODER                         0x1  //ֻ֧����Ƶ����
#define TNM_DECODER                         0x2  //ֻ֧����Ƶ����
#define TNM_BOTH                            0x3  //��Ƶ����붼֧��
#define TNM_SMARTSIGHT_ENCODER              0x4  //SMARTSIGHT��Ƶ����֧��
#define TNM_SMARTSIGHT_DECODER              0x5  //SMARTSIGHT��Ƶ����֧��
#define TNM_SMARTSIGHT_BOTH                 0x6  //SMARTSIGHT��Ƶ����붼֧��

//��������¼�������
#define TNM_DISK_POLICY_STOP_REC             0x0  // ֹͣ¼��
#define TNM_DISK_POLICY_REPLACE_ALL          0x1  // ���������ļ�
#define TNM_DISK_POLICY_KEEP_ALARMREC        0x2  // ���ǳ��澯֮����ļ�

// �豸��������
#define TNM_SINGLE_DISK                     0x0     // ������
#define TNM_MULTI_DISK                      0x1     // �����

// ��������
#define TNM_MAX_NRU_DISK_NUM                (u8)255 // ����������
#define TNM_MAX_PU_DISK_NUM                 (u8)32  // ǰ���豸������������

//vpu�ϳɷ��
#define TNM_VPU_STYLE_INVALID               0x0  //��Ч���
#define TNM_VPU_STYLE_HTWO                  0x1  //�����棺���ҷ�
#define TNM_VPU_STYLE_THREE                 0x2  //������
#define TNM_VPU_STYLE_FOUR                  0x3  //�Ļ���
#define TNM_VPU_STYLE_SIX                   0x4  //������
#define TNM_VPU_STYLE_ONE                   0x5  //�����棨�ֳ�9�ݷ��ͣ�
#define TNM_VPU_STYLE_VTWO                  0x6  //�����棺 һ��һС������ֳ�9�ݷ��ͣ�
#define TNM_VPU_STYLE_SPECFOUR              0x7  //�����Ļ��棺һ����С������ֳ�4�ݷ��ͣ�
#define TNM_VPU_STYLE_SEVEN                 0x8  //�߻��棺������С������ֳ�4�ݷ��ͣ�
#define TNM_VPU_STYLE_EIGHT                 0x9  //�˻���
#define TNM_VPU_STYLE_NINE                  0xa //�Ż���
#define TNM_VPU_STYLE_TEN                   0xb //ʮ���棺�����С������ֳ�4�ݷ��ͣ�
#define TNM_VPU_STYLE_THIRTEEN              0xc //ʮ������
#define TNM_VPU_STYLE_SIXTEEN               0xd //ʮ������

// ����������շ�״̬
#define TNM_CODEC_STOP                      0x0 // �����ͨ������
#define TNM_ENCODING                        0x1 // ���ڱ��뷢��
#define TNM_DECODING                        0x2 // ���ڽ��뷢��

// ����Ƶ��ʽ
#define TNM_ONLY_VIDEO           0x1    // ֻ����Ƶ
#define TNM_ONLY_AUDIO           0x2    // ֻ����Ƶ
#define TNM_AUDIO_VIDEO          0x3    // ����Ƶ

// ǰ���豸״̬
#define TNM_PU_STATE_ONLINE      0x01 // 0: �Ƿ����ߣ�0:offline, 1:online
#define TNM_PU_STATE_MUTE        0x02 // 1: �Ƿ�Զ�˾�����0:notmute, 1:mute
#define TNM_PU_STATE_SEND_AUDIO  0x04 // 2: �Ƿ��ڷ�����Ƶ��0:no, 1:yes
#define TNM_PU_STATE_PLAT_REC    0x08 // 3: �Ƿ�����ƽ̨¼��0:no, 1:yes
#define TNM_PU_STATE_PU_REC      0x10 // 4: �Ƿ�����ǰ��¼��0:no, 1:yes
#define TNM_PU_STATE_ALARM       0x20 // 5: �Ƿ����ҵ��澯 0:no,1:yes

#define TNM_STATE_OFFLINE                   0x00    // �豸����
#define TNM_STATE_ONLINE                    0x01    // �豸����

#define TNM_PU_GUID_LEN                     16      // GUID�ĳ���

#define TNM_OSD_COLOR_FONT      (u8)1   //��������Ӧ
#define TNM_OSD_COLOR_16        (u8)2
#define TNM_OSD_COLOR_256       (u8)3

#define TNM_UPGRADE_MODE_NONE   (u8)0   //��������
#define TNM_UPGRADE_MODE_TCP    (u8)1   //TCP��ʽ����
#define TNM_UPGRADE_MODE_FTP    (u8)2   //FTP��ʽ����

//��������
#define TNM_DISABLE             0
#define TNM_ENABLE              1

#define TNM_INVALED_IP          0xFFFFFFFF      //��Ч��IP��ַ
#define TNM_INVALED_PORT        0xFFFF          //��Ч�Ķ˿�
#define TNM_INVALID_CHANNEL_ID  0xFF            // ��Ч��ͨ����

#define TNM_REG_CMU   0              // ֱ��ע�ᵽƽ̨
#define TNM_REG_DNS   1              // ͨ�����ؾ����������ȡƽ̨��ַ

/************************************************************************/
/*                          �澯��Ķ���(��ǰ���ú��룺10027)             */
/************************************************************************/
#define TNM_ALARM_CODE_INVALID             0            // ��Ч�ĸ澯��
#define TNM_ALARM_CODE_DEV_OFFLINE         10001        // �豸����
#define TNM_ALARM_CODE_RECV_NOBITSTREAM    10002        // �����������ϱ�
#define TNM_ALARM_CODE_LOSEPACKET          10003        // �������ն�������ָ����ֵ�澯
#define TNM_ALARM_CODE_MAP_STATUS          10004        // MAPS�쳣�澯
#define TNM_ALARM_CODE_MOTIONDETECT_STATUS 10005        // �ƶ����澯
#define TNM_ALARM_CODE_LPT_STATUS          10006        // ���ڸ澯�ϱ�
#define TNM_ALARM_CODE_POWER_STATUS        10007        // ��Դ��ѹ����ָ����ֵ�澯
#define TNM_ALARM_CODE_CPU_STATUS          10008        // CPUʹ���ʸ���ָ����ֵ�澯
#define TNM_ALARM_CODE_MEMORY_STATUS       10009        // �ڴ�ʹ�ó���ָ����ֵ�澯
#define TNM_ALARM_CODE_DISKFULL_STATUS     10010        // �������澯
#define TNM_ALARM_CODE_FAN_STATUS          10011        // ����ת�ٹ��͸澯
#define TNM_ALARM_CODE_ETHCARD_RESTORE     10012        // ��̫���ָ��澯
#define TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS 10013        // ǰ���豸��ƵԴ��ʧ�澯
#define TNM_ALARM_CODE_LED_STATUS          10014        // ���ƿ�����������״̬�����ı�
#define TNM_ALARM_CODE_SAME_REG_ID         10015        // �����豸��ID���뱾�豸��ͬ��ͬʱ��NMSע��
#define TNM_ALARM_CODE_REG_CMU             10016        // PU������ģ��ע��CMUʧ�ܸ澯
#define TNM_ALARM_CODE_DISKDETECT_ERROR    10017        // �豸����޴��̻�û�д��̿ɹ����澯


#define TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET        10021        //Cuͨ��CMU���͵�CUҵ��澯
#define TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION        10022        //�˸澯��Cuͨ��CMU���͵�CUҵ��澯
#define TNM_ALARM_CU_OVERLOAD              10023        // ��¼CU��������ֵ
#define TNM_ALARM_PU_OVERLOAD              10024        // ��¼PU��������ֵ
#define TNM_ALARM_VTDU_OVERLOAD            10025        // VTDUת��·��������ֵ


//  23  TNM_ALARM_CODE_NODISK_ERROR         ��Ӳ�� - ��IDE���(0-3)
//      u8(Ӳ��IDE���) + u8(Ӳ�̵�״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_NODISK_ERROR        10100       //��Ӳ�� - ��IDE���(0-3)

//  24  TNM_ALARM_CODE_DISK_ERROR           Ӳ�̷����� - ���������(0-15)
//      u8(Ӳ�̷������) + u8(Ӳ�̷�����״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISK_ERROR          10101       //Ӳ�̷����� -���������(0-15)

//  25  TNM_ALARM_CODE_DISK_NOFORMAT        Ӳ�̷���δ��ʽ�� - ���������(0-15)
//      u8(Ӳ�̷������) + u8(Ӳ�̷�����״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISK_NOFORMAT       10102       //Ӳ�̷���δ��ʽ�� - ���������(0-15)

//  26  TNM_ALARM_CODE_STOPREC_LOGERR       ǰ��ͣ¼ - ��־ϵͳ��ͣ¼
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_STOPREC_LOGERR      10103       //ǰ��ͣ¼ - ��־ϵͳ��ͣ¼

//  27  TNM_ALARM_CODE_STOPREC_DISKERR      ǰ��ͣ¼ - Ӳ�̻�ͣ¼
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_STOPREC_DISKERR     10104       //ǰ��ͣ¼ - Ӳ�̻�ͣ¼


//  28  TNM_ALARM_CODE_DISCONNECT_3AS
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_3AS      11001      //δ������3AS

//  29  TNM_ALARM_CODE_DISCONNECT_UAS
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_UAS      11002      //δ������UAS

//  30  TNM_ALARM_CODE_DISCONNECT_PUI
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_PUI      11003      //δ������PUI

//  31  TNM_ALARM_CODE_DISCONNECT_CUI
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_CUI      11004      //δ������CUI

//  32  TNM_ALARM_CODE_DISCONNECT_NRU
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_NRU      11005      //δ������NRU

//  33  TNM_ALARM_CODE_DISCONNECT_VTDU
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_VTDU     11006      //δ������VTDU

//  34  TNM_ALARM_CODE_DISCONNECT_CMU
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_CMU      11007      //δ������CMU

//  35  TNM_ALARM_CODE_DISCONNECT_DATABASE
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_DATABASE 11008      //δ������DATABASE

//  36  TNM_ALARM_CODE_DISCONNECT_USBKEY
//      u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_USBKEY   11009     //δ������USBKEY

//  37  TNM_ALARM_CODE_DISK_PART_R_ONLY   NRU���̷���ֻ���澯
//      u8*(���̷���mount·��---�������80)+u8(�澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISK_PART_R_ONLY			11010		//���̷���ֻ��
#define TNM_ALARM_CODE_DISK_FS_ERROR			11011		//�����ļ�ϵͳ����
#define TNM_ALARM_CODE_DISK_OFFLINE				11012		//��������
#define TNM_ALARM_CODE_STORAGE_ABNORMAL			11013		//�������쳣

#define TNM_ALARM_CODE_PLATFORM_IN_REPLACE_MODE 11014		//ƽ̨�����ڱ����滻ģʽ

#define TNM_ALARM_CODE_RAID_DISK_OFFLINE		11015		 //����Ӳ���𻵻��ȱ
#define TNM_ALARM_CODE_RAID_DISK_HEALTH_BAD		11016		 //����Ӳ�̽���״̬�쳣
#define TNM_ALARM_CODE_RAID_DISKGROUP_STATUS_ABNORMAL	11017//���д�����״̬������
#define TNM_ALARM_CODE_RAID_ISCSI_DROPPED		11018		 //����ISCSI����
#define TNM_ALARM_CODE_RAID_ABNORMAL			11019		 //�����쳣

#define TNM_ALARM_CODE_DETECT_NULL_IP_NIC       11020        //̽�⵽��IP������

//      s8[33](IPStr)+s8[33](TKDMNO)+u8(TNM_STATUS_ALARM/TNM_STATUS_NORMAL)
#define TNM_ALARM_CODE_DISCONNECT_SUPER_CMU		11021      //δ�������ϼ�CMU
//      s8[33](IPStr)+s8[33](TKDMNO)+u8(TNM_STATUS_ALARM/TNM_STATUS_NORMAL)
#define TNM_ALARM_CODE_DISCONNECT_SUB_CMU		11022		//�¼�CMU�Ͽ�����
//      s8[33](IPStr)+s8[33](TKDMNO)+u8(TNM_STATUS_ALARM/TNM_STATUS_NORMAL)
#define TNM_ALARM_CODE_DISCONNECT_BCMU_CMU		11023		//δ������ƽ��CMU

#define TNM_ALARM_CODE_AT_TERM_REMIND_USBKEY       11024      //USB��������
#define TNM_ALARM_CODE_LICENSE_VALIDATE_NOT_PASS   11026      //LICENSEУ�鲻ͨ��
//      u32(����ƽ̨��������¼�������)+u32(��ǰ���ӵĴ�����)
#define TNM_ALARM_CODE_PLATFORM_DISK_NUM_OVERLOAD	11025		//ƽ̨�������̷�����������¼���֧�ֵ�������


/************************************************************************/
#define MAX_DEV_ALARM_TYPE                 36           // ��һ�豸�������ϱ��ĸ澯����
#define TNM_ROOT_NODE_ID                    "99000000000000000050000000000000"  // ����ƽ̨������

// ID��ų���
// ��������������: CMS + NO����� + ʣ��
#define   FIELD_LEN_CMS                     (u8)18      //  CMS����γ���
#define   FIELD_LEN_ID_TYPE                 (u8)1       //  ID����� ����γ���
#define   FIELD_LEN_ELSE                    (u8)13      //  ʣ��     ����γ���

#define   FIELD_LEN_PU_TYPE                 (u8)1       // ǰ���豸����   ����γ���, ��������enum TnmDevCategory
#define   FIELD_LEN_PU_SUBTYPE              (u8)1       // ǰ�����豸���� ����γ���, ��������enum TnmDevCategory

#define   FIELD_LEN_PLAT_DEV_TYPE           (u8)1       //ƽ̨�豸����   ����γ���

//Hot Backup State
#define TNM_HB_STATE_NORMAL    0     //��ͨģʽ������
#define TNM_HB_STATE_ACTIVE    1     //��ȱ�������
#define TNM_HB_STATE_INACTIVE  2     //�����ȱ�������

//CMU ��ģ�������
#define  TNM_MAX_CMU_MODULE_NUM    128

// �豸�ĸ澯״̬
enum TnmStatusType
{
    TNM_STATUS_NONE = 0,        // ��ʾ��Ч״̬
    TNM_STATUS_ALARM,           // ��ʾ����������ĸ澯״̬(�豸���ߵ�)
    TNM_STATUS_NORMAL,          // ��ʾ����������״̬(�豸���ߵ�)
    TNM_STATUS_CLEAR,           // ������Ȼ���ڣ���������Ա�ֶ�����澯���
    TNM_STATUS_NOCONFIRM = 4,   // δȷ��
    TNM_STATUS_CONFIRM          // ��ȷ��
};
#define MAX_MOUNTPATH_LEN       (s32)80 //�洢������::��װ·������󳤶�

//PCƽ̨�豸�Ĳ�ۺ�
#define PC_PLAT_LAYER 1
#define PC_PLAT_SLOT  1

// �����豸��Ӳ������
enum TnmUnitType
{
    // δ֪�豸
    TNM_UNIT_UNKNOW = 0,

    // ǰ���豸
    TNM_PU_KDM2400 = 1,
    TNM_PU_KDM2400_PLUS,
    TNM_PU_KDM2500,
    TNM_PU_KDM2500_PLUS,
    TNM_PU_KDM2417,
    TNM_PU_KDM2418,
    TNM_PU_KDM2401,
    TNM_PU_KDM2428,
    TNM_PU_KDM2518,
    TNM_PU_KDV8010A,
    TNM_PU_KDV8010C,
    TNM_PU_KDM2401L,
    TNM_PU_KDM2401S,
    TNM_PU_KDM2402S,
    TNM_PU_KDM2402,

    // ������ƽ̨�豸
    TNM_CMU_KDM2100A = 16,

    // ҵ�񵥰�
    TNM_BOARD_MPC = 21,
    TNM_BOARD_CRI,
    TNM_BOARD_DRI,
    TNM_BOARD_VPU,
    TNM_BOARD_VAS,
    TNM_BOARD_DEC5,
   
    // ̨ʽ�����
    TNM_PC_UNIT = 31,
    
    //pu����
    TNM_PU_KDM2461 = 55,
    TNM_PU_KDM2461L,
    TNM_PU_KDM2560,
    TNM_PU_KDM2401LS,
    TNM_PU_KDM2402L,
    TNM_PU_KDM2402LS,

    TNM_PU_KDM2440,
    TNM_PU_KDM2440P,
    TNM_PU_KDM201_ENC_CARD,
    TNM_PU_KDM201_DEC_CARD,
    TNM_PU_KDM2461LS,
    TNM_PU_KDM2462L,
    TNM_PU_KDM2462S,
    TNM_PU_KDM2462LS,
    TNM_PU_KDM2461S,
    TNM_PU_KDM2462,

    TNM_PU_HARDWARE_PU = 100        //ǰ���豸Ӳ������
};

// ҵ�����Ͷ���
enum TnmServiceType
{
    // δ֪ҵ������
    TNM_SERVICE_UNKNOW = 0,

    // ǰ��ҵ��
    TNM_PU_SERVICE_KDM2400 = 1,
    TNM_PU_SERVICE_KDM2400_PLUS,
    TNM_PU_SERVICE_KDM2500,
    TNM_PU_SERVICE_KDM2500_PLUS,
    TNM_PU_SERVICE_KDM2417,
    TNM_PU_SERVICE_KDM2418,
    TNM_PU_SERVICE_KDM2401,
    TNM_PU_SERVICE_KDM2428,
    TNM_PU_SERVICE_KDM2518,    
    TNM_PU_SERVICE_KDV8010A,
    TNM_PU_SERVICE_KDV8010C, 
    TNM_PU_SERVICE_KDMDEC5,
    TNM_PU_SERVICE_PU,                  // ǰ��ҵ����

    // ƽ̨ҵ��
    TNM_CMU_SERVICE_KDM2000 = 15,
    TNM_CMU_SERVICE_KDM2100A,

    // ����ҵ��
    TNM_CMU_SERVICE_PUI = 21,
    TNM_CMU_SERVICE_CUI,
    TNM_CMU_SERVICE_VTDU,
    TNM_CMU_SERVICE_VPU,
    TNM_CMU_SERVICE_DNS,

    // PCҵ��
    TNM_PC_SERVICE_SERVER = 35,  // ƽ̨ҵ����
    TNM_PC_SERVICE_NRU = 31,
    TNM_PC_SERVICE_UAS,
    TNM_PC_SERVICE_3AS,
    TNM_PC_SERVICE_NMS,
    
    //pu����
    TNM_PU_SERVICE_KDM2401L = 50,
    TNM_PU_SERVICE_KDM2401S,
    TNM_PU_SERVICE_KDM2402S,
    TNM_PU_SERVICE_KDM2402,
    TNM_PU_SERVICE_KDM2461,
    TNM_PU_SERVICE_KDM2461L,
    TNM_PU_SERVICE_KDM2560,
    TNM_PU_SERVICE_KDM2401LS,
    TNM_PU_SERVICE_KDM2402L,
    TNM_PU_SERVICE_KDM2402LS,

    TNM_PU_SERVICE_KDM2440,
    TNM_PU_SERVICE_KDM2440P,
    TNM_PU_SERVICE_KDM2461LS,
    TNM_PU_SERVICE_KDM2462L,
    TNM_PU_SERVICE_KDM2462S,
    TNM_PU_SERVICE_KDM2462LS,
    TNM_PU_SERVICE_KDM2461S,
    TNM_PU_SERVICE_KDM201_ENC_CARD,
    TNM_PU_SERVICE_KDM201_DEC_CARD,
    TNM_PU_SERVICE_KDM2462,
  
    //IAS,��ע�͵���, ��ӪҲ��ά����
    //IMU
    TNM_CMU_SERVICE_IMU,

    //VMU
    TNM_CMU_SERVICE_VMU,

    //RMG
    TNM_CMU_SERVICE_RMG,

    //NVR
    TNM_NVR_SERVICE_TRASPARENT_CHANNEL,

    //VRS
    TNM_CMU_SERVICE_VRS,

    //VMTU
    TNM_CMU_SERVICE_VMTU,
    
    TNM_PC_SERVICE_ALL = 100,           // ����ҵ������
    TNM_KDV_SERVICE_ALL = 101,          // ����ҵ������  
};

// �������
enum TnmDevIdType
{
    ID_TYPE_INVALID     = 0,    // ǰ���豸�������
    ID_TYPE_PU          = 1,    // ǰ���豸�������
    ID_TYPE_PLATFORM    = 2,    // ƽ̨�豸�������
    ID_TYPE_DOMAIN_CMU  = 5     // ���CMUƽ̨�������
};

// �豸�Ĺ��ܷ��ඨ��
enum TnmDevCategory
{
    TNM_CAT_INVALID = 0,

    TNM_CAT_CMU     = 1,        // CMU

    TNM_CAT_VTDU    = 11,       // ����ת��
    TNM_CAT_NRU     = 12,       // ����¼����
    TNM_CAT_VPU     = 13,       // ���渴��
    TNM_CAT_UAS     = 14,       // CMU�ĸ��Ի����ݷ�����
    TNM_CAT_PUI     = 15,       // PU����
    TNM_CAT_CUI     = 16,       // CU����
    TNM_CAT_DNS     = 17,       // ���ؾ��������Lds

    TNM_CAT_DVR         = 21,   // DVR���洢�ı������������ش洢����Ƶ��������
    TNM_CAT_ENCODER     = 22,   // DV ���������������ش洢����Ƶ��������
    TNM_CAT_IPCAMERA    = 23,   // IP-Camera
    TNM_CAT_DECODER     = 24,   // ������
	TNM_CAT_MTU			= 26,	//ת�������

    TNM_CAT_VIDICON = 31,       // �����
    TNM_CAT_YT      = 32,       // ��̨
    TNM_CAT_IOIN    = 33,       // ���������豸��������/ģ����������⡢�̸С��Ŵŵ�
    TNM_CAT_INOUT   = 34,       // ��������豸:�羯�ơ�����
    TNM_CAT_AIN     = 35,       // ���������豸
    TNM_CAT_AOUT    = 36,       // ��������豸
    TNM_CAT_MATRIX  = 37        // ����
};

enum TnmPlatDevType
{
    TNM_PLAT_VTDU   = 1,        // ����ת��
    TNM_PLAT_NRU    = 2,        // ����¼����
    TNM_PLAT_VPU    = 3,        // ���渴��
    TNM_PLAT_UAS    = 5,        // CMU�ĸ��Ի����ݷ�����
    TNM_PLAT_DNS    = 6,        // ���ؾ��������
    TNM_PLAT_PUI    = 7,        // PU����
    TNM_PLAT_CUI    = 8,         // CU����
	TNM_PLAT_MTU    = 9,		//MTU
};

// PU�豸����
enum TnmPuDevType
{
    TNM_PU_TYPE_MAIN    = 1,    // ���豸����Ƶ�������ȸ�������Ƶ���벢�����紫�͵��豸
    TNM_PU_TYPE_PERI    = 2     // ��Χ�豸�����豸���ӵ����������̨���豸
};

// PU���豸����
enum TnmPuMainType
{
    TNM_PU_MAIN_DVR         = 1,    // DVR���洢�ı������������ش洢����Ƶ��������
    TNM_PU_MAIN_ENCODER     = 2,    // DV ���������������ش洢����Ƶ��������
    TNM_PU_MAIN_IPCAMERA    = 3,    // IP-Camera
    TNM_PU_MAIN_DECODER     = 4     //������
};

// PU���豸����
enum TnmPuSubType
{
    TNM_PU_SUB_VIDICON      = 1,    //�����
    TNM_PU_SUB_YT           = 2,    //��̨
    TNM_PU_SUB_IOIN         = 3,    //���������豸��������/ģ����������⡢�̸С��Ŵŵ�
    TNM_PU_SUB_INOUT        = 4,    //��������豸:�羯�ơ�����
    TNM_PU_SUB_AIN          = 5,    //���������豸
    TNM_PU_SUB_AOUT         = 6,    //��������豸
    TNM_PU_SUB_MATRIX       = 7     //����
};

//��������
enum AgentType
{
    EM_CENTERAGENT,     //���Ĵ���
    EM_SUBAGENT         //�Ӵ���
};

enum CommunityType
{
    READ_COMMUNITY,
    WRITE_COMMUNITY
};


//�����˸״̬
enum TnmLedStatus
{
    TNM_LED_NONE = 0,        // ��
    TNM_LED_ON,              // ��
    TNM_LED_OFF,             // ��
    TNM_LED_EVERY_0_25_SEC,  // 0.25���л�һ������״̬
    TNM_LED_EVERY_0_5_SEC,   // 0.5���л�һ������״̬
    TNM_LED_EVERY_1_0_SEC,   // 1���л�һ������״̬
    TNM_LED_EVERY_2_0_SEC    // 2���л�һ������״̬
};

//��̫����״̬
enum TnmEnternetStatus
{
    TNM_ETHERNET_NONE = 0,
    TNM_ETHERNET_ON,
    TNM_ETHERNET_OFF
};

//����״̬
enum TnmFanStatus
{
    TNM_FAN_NONE = 0,
    TNM_FAN_ON,
    TNM_FAN_OFF
};

//��Դ״̬
enum TnmPowerStatus
{
    TNM_POWER_NONE = 0,
    TNM_POWER_ON,
    TNM_POWER_OFF
};

//¼����״̬
enum TnmRpState 
{
    RP_STATE_NULL = 0xFF,
    RECORD_RUNNING = 0,     // ����¼��
//    RECORD_WAITING = 1,       // ��ʱ��¼��
    PRERECORDING = 2,       // ����Ԥ¼
    PLAY_RUNNING = 10,      // ���ڲ���
    PLAY_PAUSING = 11, 
    FAST_FORWARDING = 12, 
    SLOW_FORWARDING = 13, 

    RP_STATE_STOP = 20      // ¼����ֹͣ״̬s
};

//��������
enum TnmPlayRate
{
    INVALID_RATE    = 0,    // ��Чֵ
    ONE_TIMES   = 1,        // 1����
    TWO_TIMES   = 2,        // 2����
    THREE_TIMES = 3,        // 3����
    FOUR_TIMES  = 4,        // 4����

    ONE_OF_TWO      = 10,   // 1/2 ����
    ONE_OF_THREE    = 11,   // 1/3 ����
    ONE_OF_FOUR     = 12,   // 1/4 ����
    ONE_OF_EIGHT    = 13,   // 1/8 ����
};

enum TnmPowType
{
    emPowStabilizer,   // ��ѹ��Դ
    emPowEthernet,    // ��̫������
    emPowUps,        // UPS����
    emPowBattery     // ��ع���
};

// �豸��ʼ������
typedef struct tagTnmAdpInitParam
{
    tagTnmAdpInitParam(void)
    {
        dwLocalIp   = TNM_INVALED_IP;
        wLocalPort  = TNM_INVALED_PORT;
        dwRemoteIp  = TNM_INVALED_IP;
        wRemotePort = TNM_INVALED_PORT;
    }
    u32  dwLocalIp;            //[������](NMS���� )NMS�����󶨵�IP��ַ
                               //        (���Ĵ���)���Ĵ�������󶨵�IP��ַ
    
    u32  dwRemoteIp;           //[������](���Ĵ���)Ҫ���ӵ�NMS��IP��ַ
                               //        (�Ӵ���  )Ҫ���ӵ����Ĵ����IP��ַ

    u16  wLocalPort;           //[������](NMS���� )NMS�ļ����󶨵Ķ˿�
                               //        (���Ĵ���)���Ĵ���ļ����󶨵Ķ˿�(Ĭ�ϣ�1727)
    u16  wRemotePort;          //[������](���Ĵ���)Ҫ���ӵ�NMS�Ķ˿�
                               //        (�Ӵ���  )Ҫ���ӵ����Ĵ���Ķ˿�
}PACKED TTnmAdpInitParam;


//�����ɹ��ϱ�3asҵ������ݽṹ
typedef struct tagTnmLinkInfo
{   
    tagTnmLinkInfo()
    {
        dwLinkInsId = 0;
        dwNmsIp = 0;
        wNmsPort = 0;

    }
    u32 dwLinkInsId;    //[������]��·Id;
    u32 dwNmsIp;        //[������]����������Ip;
    u16 wNmsPort;       //[������]���ܶ˿�;
    u8  byReserved1;
    u8  byReserved2;
}PACKED TnmLinkInfo;

#define READ_FLAG     0
#define WRITE_FLAG    1
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*=============================================================================
  ��    �ܣ� Agent��д�ص���������
  ��    ���� u32 dwNodeName --     [IN]��д�ڵ���(NODE_SYSPUBCFGINFO��)
             u8 byRWFlag    --     [IN]��д��־(READ_FLAG WRITE_FLAG)
             void * pBuf    -- [IN/OUT]��д����
             u16 * pwBufLen -- [IN/OUT]��д���ݵĳ���
             u32 dwUserData --     [IN]�Ѷ����ݵ��ۻ�����
                                       ��д���ݵĽ�����־(SNMP_FINISHED/SNMP_NOFINISHED)

  �� �� ֵ�� SNMP_SUCCESS - �ɹ�
             SNMP_NOFINISHED - ����̫���д��û�����
             ���� - ʧ��
=============================================================================*/
typedef u16 (*TTnmAgentCallBack)(u32 dwNodeName, u8 byRWFlag, void * pBuf, u16 * pwBufLen, u32 dwUserData);

//renhouping 2006.08.16 for 3as
typedef u32 (*TTnm3asAgentCallBack)(u32 dwNodeName, u8 byRWFlag, void * pBuf, u16 * pwBufLen, u32 dwUserData, u32 dwInsId);

u16 SnmpAdp_AgentInit(u8 byAgentType, TTnmAdpInitParam * ptAdpInitParam, TTnmAgentCallBack tAgentCallBack);
u16 SnmpAdp_SendTrap(u32 dwTrapType, void * pBuf, u16 wBufLen);
u16 SnmpAdp_CloseAgent(void);

#ifdef __cplusplus
}
#endif

class CTnmSnmpAdapter;

/**
 * �豸������ӿ���.
 */
class CTnmAgentAdapter
{
public:
    CTnmAgentAdapter();
    virtual ~CTnmAgentAdapter();

    /**
     * ��ʼ���豸�����������
     * @param byAgentType - ��������(enum AgentType)
     * @param ptAgentInitParam - �����ʼ������ָ��
     * @param byAgentIndex - ��������ֵ
     * @return SNMP_SUCCESS - �ɹ�  ���� - ʧ��
     */
    u16  Initialize(u8 byAgentType, TTnmAdpInitParam *ptAgentInitParam = NULL, u8 byAgentIndex = 0);

    /**
     * 3ASר�õĳ�ʼ���豸�����������
     * @param byAgentType - ��������(enum AgentType)
     * @param ptAgentInitParam - �����ʼ������ָ��
     * @param byParamNum - ��������Ԫ�ظ���
     * @return SNMP_SUCCESS - �ɹ�  ���� - ʧ��
     */
    u16  Initialize_3asAdp(u8 byAgentType, TTnmAdpInitParam *ptAgentInitParam = NULL, u8 byParamNum = 1);

    /**
     * ���ö�д��ͬ��
     * @param pchCommunity --- ��ͬ��ָ��
     * @param byCommunityType --- ��ͬ������
     * @return SNMP_SUCCESS - �ɹ�  ���� - ʧ��
     */
    u16  SetCommunity(s8 * pchCommunity, u8 byCommunityType);

    /*=============================================================================
      �� �� ���� SetAgentCallBack
      ��    �ܣ� ����Agent�˵Ķ�д�ص�����
      ��    ���� TTnmAgentCallBack tAgentCallBack --- �ص�����
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  SetAgentCallBack(TTnmAgentCallBack tAgentCallBack);
    u16  SetAgentCallBack_3asAdp(TTnm3asAgentCallBack tAgentCallBack);    

    /*=============================================================================
      �� �� ���� SendTrap
      ��    �ܣ� Agent�˷�������NMS������Ϣ�Ľӿں���
      ��    ���� u32 dwTrapType --- Ҫ���͵���Ϣ����, NODE��Ϣ��TRAP��Ϣ
                 void * pBuf --- �澯����
                 u16 wBufLen --- �澯����
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  SendTrap(u32 dwTrapType, void * pBuf, u16 wBufLen);

    /*=============================================================================
      �� �� ���� SendTrap
      ��    �ܣ� 3as Agent�˷�������NMS������Ϣ�Ľӿں���
      ��    ���� u32 dwTrapType --- Ҫ���͵���Ϣ����, NODE��Ϣ��TRAP��Ϣ
                 void * pBuf --- �澯����
                 u16 wBufLen --- �澯����
                 u16 wInsId --- ������Ϣ��3as�˴����ʵ����
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  SendTrap_3asAdp(u32 dwTrapType, void * pBuf, u16 wBufLen, u32 dwInsId);

    ///����֪ͨ
    u16  DisconnectNotify(void);

    /*=============================================================================
      �� �� ���� CloseAgent
      ��    �ܣ� ����Agent�ĺ���
      ��    ���� ��
      �� �� ֵ�� SNMP_SUCCESS - �ɹ�  ���� - ʧ��
    =============================================================================*/
    u16  CloseAgent(void);

private:
    CTnmSnmpAdapter * m_pcSnmpAdp;
};

const s8* GetTimeStr(u32 dwTime);
const s8* GetIpStr(u32 dwIpAddr);
const s8 * GetStatusStr(u8 byState);
inline const s8* GetAlarmCodeDesc(u32 dwAlarmCode);

typedef struct tagTnmDeviceId
{
    enum { INVALID_ID_CHAR = '0' };
    // ID��Ϣ�Ƿ�Ϊ��Ч
    BOOL32 IsValid(void) const;
    //2006/03/13 ���ǿ �Ƿ�Ϊ���ڵ�
    BOOL32 IsRoot() const;
    // 2006/03/21 ���ǿ ��ID��Ϊ��
    void SetNull(void);
    // 2006/03/21 ���ǿ ���صȺ�
    BOOL32 operator == ( const tagTnmDeviceId &tDevId ) const;
    // 2006/03/30 ���ǿ ����IDֵ
    void SetId(s8 *pbyId, s32 nLen);
    void SetId(const s8 *pbyId)
    {
        SetId(const_cast<s8*>(pbyId), strlen(pbyId));
    }
    // 2006/03/30 ���ǿ �õ��豸ID���ִ�ָ��
    s8* GetId() const;
    // 2006/04/13 ���ǿ �����豸ID���õ��豸�ķ���
    u32 GetDevCategory() const;
    // 2006/04/13 ���ǿ �õ��豸���������
    const s8* GetDevCatDesc() const;
    // 2006/04/13 ���ǿ �õ��豸ID����һ������豸
    u8 GetIdType() const;
    // 2006/04/13 ���ǿ �õ�ǰ���豸������
    u8  GetPuType() const;
    // 2006/04/13 ���ǿ �õ�ǰ�����豸������
    u8  GetPuMainType() const;
    // 2006/04/13 ���ǿ �õ�ǰ�˵���Χ�豸����
    u8  GetPuSubType() const;
    // 2006/04/13 ���ǿ �õ�ƽ̨�豸������
    u8  GetPlatType() const;
    // 2006/04/13 ���ǿ ID�ִ�ת����
    u32 StrToNum(const s8 *pchId, s32 nLen) const;
    // 2006/04/26 ���ǿ �Ƿ�Ϊ������ʾ�����ܽ����ϵ��豸
    BOOL32 IsIndependDev() const;

    bool operator < ( const tagTnmDeviceId& tId ) const;

    s8  achID[TNM_MAX_DEVICE_ID_LEN + 4];
}PACKED TTnmDeviceId;

/*========================================================================
�� �� ����IsValid
��    �ܣ��ж�ID�Ƿ�Ϊ��Чֵ
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/21  3.9     ���ǿ  ����
========================================================================*/

inline bool TTnmDeviceId::operator <( const tagTnmDeviceId& tId )const
{
    int nCmp = strcmp( achID, tId.achID );
    return nCmp < 0;
}

inline BOOL32 TTnmDeviceId::IsValid(void) const
{
    for (u16 wLoop = 0; wLoop < TNM_MAX_DEVICE_ID_LEN; wLoop++)
    {
        if (INVALID_ID_CHAR != achID[wLoop])
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*========================================================================
�� �� ����IsRoot
��    �ܣ��Ƿ�Ϊ���ڵ�
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/13  3.9     ���ǿ  ����
========================================================================*/
inline BOOL32 TTnmDeviceId::IsRoot() const
{   
    if (GetDevCategory() != TNM_CAT_CMU) 
    {
        // ����ƽ̨�豸
        return FALSE;
    }

    s32 nNum = StrToNum(achID, 2);
    if (nNum < 1) 
    {
        return FALSE;
    }

    for(s32 nIndex = 2; nIndex < FIELD_LEN_CMS; nIndex++)
    {
        s8 chNum = achID[nIndex];
        if (chNum != '0') 
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*========================================================================
�� �� ����SetNull
��    �ܣ���ID��Ϊ��
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/21  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmDeviceId::SetNull(void)
{
    memset(achID, INVALID_ID_CHAR, TNM_MAX_DEVICE_ID_LEN);
    achID[TNM_MAX_DEVICE_ID_LEN] = '\0';
}

/*========================================================================
�� �� ����operator == 
��    �ܣ����صȺ�
��    ����const tagTnmDeviceId &tDevId          [in]Ҫ�Ƚϵ�ID
�� �� ֵ: TRUE��    ��ͬ��  FALSE�� ����ͬ
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/21  3.9     ���ǿ  ����
========================================================================*/
inline BOOL32 TTnmDeviceId:: operator == ( const tagTnmDeviceId &tDevId ) const
{
    if (strncmp(achID, tDevId.achID, TNM_MAX_DEVICE_ID_LEN) == 0) 
    {
        return TRUE;
    }
    return FALSE;
}

/*========================================================================
�� �� ����SetId
��    �ܣ�����IDֵ
��    ����s8 *pbyId                             [in]Ҫ���õ�ֵ
          s32 nLen                              [in]�ִ��ĳ���
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/30  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmDeviceId::SetId(s8 *pbyId, s32 nLen)
{
    assert(pbyId);
    assert(nLen > 0);
    assert(nLen <= TNM_MAX_USER_ID_LEN);

    if (pbyId == NULL || nLen < 1) 
    {
        return;
    }

    if (nLen > TNM_MAX_USER_ID_LEN)
    {
        nLen = TNM_MAX_USER_ID_LEN;
    }
    s32 nCopyLen = nLen;
    memcpy(achID, pbyId, nCopyLen);
    memset(achID + nCopyLen, 0, TNM_MAX_USER_ID_LEN + 1 - nCopyLen);
}
/*========================================================================
�� �� ����GetId
��    �ܣ��õ��豸ID���ִ�ָ��
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/30  3.9     ���ǿ  ����
========================================================================*/
inline s8* TTnmDeviceId::GetId() const
{
    return (s8*)achID;
}

/*========================================================================
�� �� ����GetDevCategory
��    �ܣ������豸ID���õ��豸�ķ���
��    ����
�� �� ֵ���豸�ķ���enum TnmDevCategory
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/13  3.9     ���ǿ  ����
========================================================================*/
inline u32 TTnmDeviceId::GetDevCategory() const
{
    u8 byType = GetIdType();
    switch(byType) 
    {
    case ID_TYPE_PU:
        {
            u8 byPuType = GetPuType();
            if (byPuType == TNM_PU_TYPE_MAIN) 
            {
                u8 byMainType = GetPuMainType();
                switch(byMainType) 
                {
                case TNM_PU_MAIN_DVR:
                    return TNM_CAT_DVR;
                case TNM_PU_MAIN_ENCODER:
                    return TNM_CAT_ENCODER;
                case TNM_PU_MAIN_IPCAMERA:
                    return TNM_CAT_IPCAMERA;
                case TNM_PU_MAIN_DECODER:
                    return TNM_CAT_DECODER;
                default:
                    break;
                }
                return TNM_CAT_INVALID;
            }
            else if (byPuType == TNM_PU_TYPE_PERI) 
            {
                u8 bySubType = GetPuSubType();
                switch(bySubType) 
                {
                case TNM_PU_SUB_VIDICON:
                    return TNM_CAT_VIDICON;
                case TNM_PU_SUB_YT:
                    return TNM_CAT_YT;
                case TNM_PU_SUB_IOIN:
                    return TNM_CAT_IOIN;
                case TNM_PU_SUB_INOUT:
                    return TNM_CAT_INOUT;
                case TNM_PU_SUB_AIN:
                    return TNM_CAT_AIN;
                case TNM_PU_SUB_AOUT:
                    return TNM_CAT_AOUT;
                case TNM_PU_SUB_MATRIX:
                    return TNM_CAT_MATRIX;
                default:
                    break;
                }
                return TNM_CAT_INVALID;
            }
        }
        break;
    case ID_TYPE_DOMAIN_CMU:
        {
            return TNM_CAT_CMU;
        }
        //break;
    case ID_TYPE_PLATFORM:
        {
            u8 byPuType = GetPlatType();
            switch(byPuType) 
            {
            case TNM_PLAT_VTDU:
                return TNM_CAT_VTDU;
            case TNM_PLAT_NRU:
                return TNM_CAT_NRU;
            case TNM_PLAT_VPU:
                return TNM_CAT_VPU;
            case TNM_PLAT_UAS:
                return TNM_CAT_UAS;
            case TNM_PLAT_PUI:
                return TNM_CAT_PUI;
            case TNM_PLAT_CUI:
                return TNM_CAT_CUI;
            case TNM_PLAT_DNS:
                return TNM_CAT_DNS;
			case TNM_PLAT_MTU:
				return TNM_CAT_MTU;
            default:
                break;
            }
            //return TNM_CAT_INVALID;
        }
        break;
    default:
        break;
        //return TNM_CAT_INVALID;
    }

    return TNM_CAT_INVALID;
}


/*========================================================================
�� �� ����GetIdType
��    �ܣ��õ��豸ID����һ������豸
��    ����
�� �� ֵ��TnmDevIdType
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/13  3.9     ���ǿ  ����
========================================================================*/
inline u8 TTnmDeviceId::GetIdType() const
{
    return (u8)StrToNum(achID + FIELD_LEN_CMS, FIELD_LEN_ID_TYPE);
}

/*========================================================================
�� �� ����StrToNum
��    �ܣ�ID�ִ�ת����
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/13  3.9     ���ǿ  ����
========================================================================*/
inline u32 TTnmDeviceId::StrToNum(const s8 *pchId, s32 nLen) const
{
    if (nLen < 0) 
    {
        return 0;
    }
    if (nLen > TNM_MAX_DEVICE_ID_LEN) 
    {
        nLen = TNM_MAX_DEVICE_ID_LEN;
    }
    s8 achId[TNM_MAX_DEVICE_ID_LEN + 1];
    memset(achId, 0, sizeof(achId));
    memcpy(achId, pchId, nLen);
    return atoi(achId);
}

/*========================================================================
�� �� ����GetPuType
��    �ܣ��õ�ǰ���豸������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/13  3.9     ���ǿ  ����
========================================================================*/
inline u8 TTnmDeviceId::GetPuType() const
{
    return (u8)StrToNum(achID + FIELD_LEN_CMS + FIELD_LEN_ID_TYPE, FIELD_LEN_PU_TYPE);
}

/*========================================================================
�� �� ����GetPuMainType
��    �ܣ��õ�ǰ�����豸������
��    ����
�� �� ֵ��enum TnmPuMainType
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/13  3.9     ���ǿ  ����
========================================================================*/
inline u8 TTnmDeviceId::GetPuMainType() const
{
    return (u8)StrToNum(achID + FIELD_LEN_CMS + FIELD_LEN_ID_TYPE + FIELD_LEN_PU_TYPE,
            FIELD_LEN_PU_SUBTYPE);
}

/*========================================================================
�� �� ����GetPuSubType
��    �ܣ��õ�ǰ�˵���Χ�豸����
��    ����
�� �� ֵ��enum TnmPuSubType
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/13  3.9     ���ǿ  ����
========================================================================*/
inline u8 TTnmDeviceId::GetPuSubType() const
{
    return (u8)StrToNum(achID + FIELD_LEN_CMS + FIELD_LEN_ID_TYPE + FIELD_LEN_PU_TYPE,
            FIELD_LEN_PU_SUBTYPE);  
}
// 2006/04/13 ���ǿ �õ�ƽ̨�豸������
inline u8 TTnmDeviceId::GetPlatType() const
{
    u8 byPlatType = GetIdType();
    if (byPlatType == ID_TYPE_PLATFORM) 
    {
        return (u8)StrToNum(achID + FIELD_LEN_CMS + FIELD_LEN_ID_TYPE,
                FIELD_LEN_PLAT_DEV_TYPE);
    }
    return TNM_CAT_INVALID;
}

/*========================================================================
�� �� ����IsIndependDev
��    �ܣ��ж��豸�Ƿ�Ϊ������ʾ�����ܽ����ϵ��豸
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/26  3.9     ���ǿ  ����
========================================================================*/
inline BOOL32 TTnmDeviceId::IsIndependDev() const
{
    u32 dwCategory = GetDevCategory();
    if (dwCategory == TNM_CAT_VTDU
        || dwCategory == TNM_CAT_VPU
        || dwCategory == TNM_CAT_PUI
        || dwCategory == TNM_CAT_CUI) 
    {
        // �����岻ֱ����ʾ�����ܵ���������
        return FALSE;
    }

    return TRUE;
}


typedef struct tagTnmUserId
{
    tagTnmUserId()
    {
        memset(achID, 0, sizeof(achID));
    }
    s8  achID[TNM_MAX_USER_ID_LEN + 1];
    u8  byReserved1;
    u8  byReserved2;
    u8  byReserved3;
}PACKED TTnmUserId;


/******************************************************************************/
/*                                    �ṹ����                                */
/*ע�⣺�������нṹ��ĳ�Ա��������ģ���Ľӿڵ��û����緢��ʱ��Ӧ��Ϊ������*/
/******************************************************************************/
/********************************************************************/
/* 1.   ���ù������ò����Ļ�ȡ������                              */
/********************************************************************/
/*====================================================================
1.1.    �������ò���
====================================================================*/
typedef struct tagTnmUnitType
{
    u8  byDeviceType;       // �豸��Ӳ���ͺţ�ֻ����(enum TnmUnitType)
    u8  byServiceType;      // ҵ�����ͣ�ֻ����(enum TnmServiceType)
    //u8  byReqConfigFile;    // �豸��ע��ʱ�����ϴ������ļ� (TNM_DISABLE/TNM_ENABLE)
    //u8    byPowType;         // ���緽ʽ  (TnmPowType)
    u16 wReserved;
}TTnmUnitType;

typedef struct tagTnmPubCfgInfo
{
    tagTnmPubCfgInfo()
    {
        memset(this, 0, sizeof(tagTnmPubCfgInfo));
    }
    u32 dwIpAddr;               // �豸IP��ַ
    u32 dwSubnetMask;           // �豸��������
    u32 dwGateWay;              // ���ص�ַ
    TTnmUnitType tUnitType;     // �豸����
    u32 dwNmsIpAddr;            // ���ܷ�����IP��ַ���ɶ�д��
    u16 wTrapPort;              // ���ܷ������澯�˿ںţ�ֻ����
    u16 wGetSetPort;            // ����˶�д�����˿ںţ�ֻ����
//    s8  achGetCommunity[TNM_MAX_COMMUNITY_LEN + 1]; //��ѯ��ͬ����(�ɶ�д)
//    s8  achSetCommunity[TNM_MAX_COMMUNITY_LEN + 1]; //���ù�ͬ����(�ɶ�д)
    u8  byMemThreshold;         // ϵͳ�ڴ�ʹ�÷�ֵ���ɶ�д, 0-100��
    u8  byCpuThreshold;         // ϵͳCPUʹ�÷�ֵ���ɶ�д, 0-100��
//  u8  byLostPackThreshold;    // ��Ƶ�����澯��ֵ���ɶ�д��0-100��
    u16 wAcceptBandwidth;           // ������� ��λ kbsp
    /////u32 dwRealIpAddr;          // �豸��ʵIP��ַ(�ӷ�������ȡ�ĵ�ַ,��PPPoE)

    const tagTnmPubCfgInfo * GetPubCfgPtr() const
    {
        return this;
    }
    void Print(BOOL32 bNetToHost = FALSE);
    void NetToHost();
    void HostToNet();
}TTnmPubCfgInfo;

typedef struct tagTnmServerSysCfgInfo
{
    tagTnmServerSysCfgInfo()
    {
        memset(this, 0, sizeof(tagTnmServerSysCfgInfo));
    }

    s8  m_achModuleTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1]; //ģ������
    TTnmDeviceId m_tModuleId;                              //ģ����
    s8  m_achModuleName[TNM_MAX_STRING_LEN + 4];           //ģ������
}TTnmServerSysCfgInfo;

inline void TTnmPubCfgInfo::Print(BOOL32 bNetToHost)
{
    if (bNetToHost) 
    {
        NetToHost();
    }

    OspPrintf(TRUE, FALSE, "IP=%s,", ::GetIpStr(dwIpAddr));
    OspPrintf(TRUE, FALSE, "SMask=%s, ", ::GetIpStr(dwSubnetMask));
    OspPrintf(TRUE, FALSE, "GW: %s;\n", ::GetIpStr(dwGateWay));
    OspPrintf(TRUE, FALSE, "\tDType: %d; SType: %d; NmsIP: %s; TrapPort: 0x%hx;\n",
            tUnitType.byDeviceType, tUnitType.byServiceType,
            ::GetIpStr(dwNmsIpAddr), wTrapPort);
    OspPrintf(TRUE, FALSE, "\tSetPort:0x%hx; MThreshold: %d; CThreshold: %d;\n", 
            wGetSetPort, byMemThreshold, byCpuThreshold);
    if (bNetToHost) 
    {
        HostToNet();
    }
}

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPubCfgInfo::NetToHost()
{
    dwIpAddr = ntohl(dwIpAddr);
    dwSubnetMask = ntohl(dwSubnetMask);
    dwGateWay = ntohl(dwGateWay);
    dwNmsIpAddr = ntohl(dwNmsIpAddr);
    wTrapPort = ntohs(wTrapPort);
    wGetSetPort = ntohs(wGetSetPort);
    wAcceptBandwidth = ntohs(wAcceptBandwidth);
    /////dwRealIpAddr = ntohl(dwRealIpAddr);
}
inline void TTnmPubCfgInfo::HostToNet()
{
    dwIpAddr = htonl(dwIpAddr);
    dwSubnetMask = htonl(dwSubnetMask);
    dwGateWay = htonl(dwGateWay);
    dwNmsIpAddr = htonl(dwNmsIpAddr);
    wTrapPort = htons(wTrapPort);
    wGetSetPort = htons(wGetSetPort);
    wAcceptBandwidth = htons(wAcceptBandwidth);
    /////dwRealIpAddr = htonl(dwRealIpAddr);
}
/*====================================================================
1.2.    ǰ���豸���ò���
====================================================================*/
// 1.2.0  ǰ���豸������
typedef struct tagTnmPuCapSet
{
    tagTnmPuCapSet()
    {
        memset(this, 0, sizeof(tagTnmPuCapSet));
    }
    u8  byVidEncChannelNum;     //��Ƶ����ͨ������
    u8  byVidDecChannelNum;     //��Ƶ����ͨ������
    u8  byAudEncChannelNum;     //��Ƶ����ͨ������
    u8  byAudDecChannelNum;     //��Ƶ����ͨ������
    u8  bySerialPortNum;        //��������
    u8  byLPortInputNum;        //��������ͨ������
    u8  byLPortOutputNum;       //�������ͨ������
    u8  byVideoSrcNum;          //��ƵԴ�˿�����

    u8  byVidAdpterNum;         //��Ƶתѹͨ������
    u8  byVidCompNum;           //ͼ��ϳ�ͨ������
    u8  byVidSrcPollNum;        //��ƵԴ��ѯͨ������
    u8  byAudMixNum;            //��Ƶ����ͨ������
    u8  byPlayerNum;            //����ͨ������
    u8  byFileDwnldNum;         //�ļ���������
    u8  byRecorderNum;          //¼��ͨ������(ͬ��Ƶ����ͨ��)
    u8  byMaxStreamSendNum;     //����ͨ�����������(һ��ͨ��������Ƶ����Ƶ)
    u8  byMaxStreanDirSndNum;   //����ͨ�������ֱ��·��(һ��ͨ��������Ƶ����Ƶ)
    u8  byVGANum;               //��Ƶ���VGA�ӿڸ���
    u8  byVidOutNum;            //��Ƶ����ӿڸ���

    u8  byOsdColorNum;          //̨����ɫ��(TNM_OSD_COLOR_16\TNM_OSD_COLOR_256)
    u8  byMoveMotionAreaNum;    //�ƶ��������������
    u8  byShadeAreaNum;         //ͼ���ڵ�����������
    u8  byStreamEncrypt;        //��������
    u8  byEtherNetNum;          //��̫��������
    u8  byWirelessNum;          //������������
    u8  byDevPowerType;         //�豸���緽ʽ

    u16 wReserved;

    void Print(BOOL32 bNetToHost = FALSE);

}TTnmPuCapSet;

inline void TTnmPuCapSet::Print(BOOL32 bNetToHost)
{
    OspPrintf(TRUE, FALSE, "VECNun: %d; VDCNun: %d; AECNum: %d; ADCNum: %d\n",
                byVidEncChannelNum, byVidDecChannelNum, byAudEncChannelNum, byAudDecChannelNum);
    OspPrintf(TRUE, FALSE, "\tSPNum: %d; LCInNum: %d; LCOutNum: %d; VSrcNum: %d;\n",
                bySerialPortNum, byLPortInputNum, byLPortOutputNum, byVideoSrcNum);
    OspPrintf(TRUE, FALSE, "\tVidAdpterNum: %d, byVidCompNum: %d, byVidSrcPollNum: %d",
                byVidAdpterNum, byVidCompNum, byVidSrcPollNum);
    OspPrintf(TRUE, FALSE, "\tbyAudMixNum: %d, byPlayerNum: %d, byFileDwnldNum: %d\n",
                byAudMixNum, byPlayerNum, byFileDwnldNum);
    OspPrintf(TRUE, FALSE, "\tbyRecorderNum: %d, byMaxStreamSendNum: %d, byMaxStreanDirSndNum: %d",
                byRecorderNum, byMaxStreamSendNum, byMaxStreanDirSndNum);
    OspPrintf(TRUE, FALSE, "\tbyVGANum: %d, byVidOutNum: %d\n", byVGANum, byVidOutNum);

    OspPrintf(TRUE, FALSE, "\tbyOsdColorNum: %d, byMoveMotionAreaNum: %d, byShadeAreaNum: %d",
                byOsdColorNum, byMoveMotionAreaNum, byShadeAreaNum);
    OspPrintf(TRUE, FALSE, "\tbyStreamEncrypt: %d, byEtherNetNum: %d, byWirelessNum: %d, byDevPowerType: %d",
                byStreamEncrypt, byEtherNetNum, byWirelessNum, byDevPowerType);
}

/*====================================================================
1.2.1.  ϵͳ����
====================================================================*/
//ϵͳ�����ṹ
typedef struct tagTnmPuSysCfgInfo : public TTnmPubCfgInfo
{
    tagTnmPuSysCfgInfo()
    {
        memset(achName, 0, sizeof(achName));
        dwRegCMUIpAddr = 0;
        dwSearchMultiIpAddr = 0;
        memset(achWebUsername, 0, sizeof(achWebUsername));
        memset(achWebPassword, 0, sizeof(achWebPassword));
        byNATPacketSend = TNM_DISABLE;
        wAlarmRecoverTime = 0;
        byDiskType = TNM_SINGLE_DISK;
        memset(achBoxID, 0, sizeof(achBoxID));
        memset(achDevTypeName, 0, sizeof(achDevTypeName));
    }
    TTnmDeviceId tDevId;                                //������Ψһ��ʶ
    s8  achName[TNM_MAX_STRING_LEN + 4];                //�豸��
    u32 dwRegCMUIpAddr;                                 //�豸����ע���ƽ̨��ַ�������鲥��ַ������
                                                        //��õ�ַע�ᣬ�ɶ�д��
    u32 dwSearchMultiIpAddr;                            //�������鲥��ַ���ɶ�д��
    s8  achWebUsername[TNM_MAX_USER_ID_LEN + 4];        //Web�����½�û������ɶ�д��
    s8  achWebPassword[TNM_MAX_PASSWORD_LEN + 4];       //Web�����½���루�ɶ�д��
    u16 wAlarmRecoverTime;                              //�澯�ָ�ʱ��(��,�ɶ�д)
    u8  byNATPacketSend;                                //�Ƿ���NAT̽���(�ɶ�д) TNM_ENABLE/TNM_DISABLE
    u8  byDiskType;                                     //�豸�������ͣ��ɶ�д��
                                                        //TNM_SINGLE_DISK/TNM_MULTI_DISK
    u8  byLostPackThreshold;                            //��Ƶ�����澯��ֵ���ɶ�д��0-100��
    s8  achWebGuestname[TNM_MAX_USER_ID_LEN + 4];       //web guest name
    s8  achWebGuestPassword[TNM_MAX_PASSWORD_LEN + 4];  //web guest pwd
    u8  byRegCMUIpType;                                 //��ֱ��ע�ᵽƽ̨������ͨ�����ؾ������������ƽ̨��ַ��
                                                        //TNM_REG_CMU/TNM_REG_DNS
    u8  byHeartBeatSecond;                              //��ƽ̨����������λ���룩��Χ��1--60��
    u8  byHeartBeatLostNum;                             //��ʧ���ٴ�������Ϊ����

    u32 dwVersionServerAddr;                            //�汾��������ַ ��0����Ч��
    u16 wVersionServerPort;                             //�汾�������˿�
    u16 wReserved;
    s8  achVerServUsername[TNM_MAX_USER_ID_LEN + 4];    //�汾��������½�û������ɶ�д��
    s8  achVerServPassword[TNM_MAX_PASSWORD_LEN + 4];   //�汾��������½���루�ɶ�д��
    s8  achVersionPath[TNM_MAX_VERSION_PATH_LEN + 4];   //�汾�ļ����ڵ�·��
    u16 wCheckVersionMinute;                            //�汾�������ڣ���λ������  0����Ч��
    u8  byStreamNum;                                    //�������������
    u8  byReserved;
    s8  achDeviceLocation[TNM_MAX_STRING_LEN + 4];      //�豸���ڵ�λ������

    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //�豸������(ֻ��)

    u32 dwRealIpAddr;                                   //�豸��ʵIP��ַ(�ӷ�������ȡ�ĵ�ַ,��PPPoE)
    u8  byReqConfigFile;                                //�豸��ע��ʱ�����ϴ������ļ� (TNM_DISABLE/TNM_ENABLE)
    u8  byPowType;                                      //���緽ʽ  (TnmPowType)
    u16 wReserved1;

    s8  achBoxID[MAX_BOX_ID_LEN + 4];   //�����ʶ

    u8  abyReserved[28];

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tName=%s; RegIp=%s, ",  achName, ::GetIpStr(dwRegCMUIpAddr));
        OspPrintf(TRUE, FALSE, "MulIp: %s;\n", ::GetIpStr(dwSearchMultiIpAddr));
        OspPrintf(TRUE, FALSE, "\tWevUsrName: %s; WebPwd: %s; NAT: %d;\n", 
            achWebUsername, achWebPassword, byNATPacketSend);
        OspPrintf(TRUE, FALSE, "\tAResumTime: 0x%hx; DiskType: %d; \n",
            wAlarmRecoverTime, byDiskType);
        OspPrintf(TRUE, FALSE, "\tGuestWebName:%s; pwd:%s;\n", achWebGuestPassword, achWebGuestPassword);
        OspPrintf(TRUE, FALSE, "\tachBoxID:%s\n", achBoxID);
        OspPrintf(TRUE, FALSE, "\tachDevTypeName:%s\n", achDevTypeName);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }

    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuSysCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPuSysCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwRegCMUIpAddr = ntohl(dwRegCMUIpAddr);
    dwSearchMultiIpAddr = ntohl(dwSearchMultiIpAddr);
    wAlarmRecoverTime = ntohs(wAlarmRecoverTime);
    dwVersionServerAddr = ntohl(dwVersionServerAddr);
    wCheckVersionMinute = ntohs(wCheckVersionMinute);
    wVersionServerPort = ntohs(wVersionServerPort);
    dwRealIpAddr = ntohl(dwRealIpAddr);
}
inline void TTnmPuSysCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();
    
    dwRegCMUIpAddr = htonl(dwRegCMUIpAddr);
    dwSearchMultiIpAddr = htonl(dwSearchMultiIpAddr);
    wAlarmRecoverTime = htons(wAlarmRecoverTime);
    dwVersionServerAddr = htonl(dwVersionServerAddr);
    wCheckVersionMinute = htons(wCheckVersionMinute);
    wVersionServerPort = htons(wVersionServerPort);
    dwRealIpAddr = htonl(dwRealIpAddr);
}

// ǰ�˴洢ϵͳ����, ֻ��2417/2428��Ч
typedef struct tagTnmPuStoreSysCfgInfo : public TTnmPuSysCfgInfo
{
    tagTnmPuStoreSysCfgInfo()
    {
        byDiskFullPolicy = TNM_DISK_POLICY_STOP_REC;
        wShortFileRecTime = 60;
        byMaxFreeDiskSpaceRate = 10;
    }

    u8  byDiskFullPolicy;                       // �������ĸ��ǲ��ԣ��ɶ�д��
                                                //  TNM_DISK_POLICY_STOP_REC
                                                //  TNM_DISK_POLICY_REPLACE_ALL
                                                //  TNM_DISK_POLICY_KEEP_ALARMREC
    u8  byMaxFreeDiskSpaceRate;                 // �������ʣ��ռ䣨�ɶ�д,100%��
    u16 wShortFileRecTime;                      // С�ļ�¼��ʱ�����ɶ�д,���ӣ�
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPuSysCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tDiskFuPoli: %d; sFileRecTime: 0x%hx; MaxFreeSpace: %d%%;\n", 
                byDiskFullPolicy, ntohs(wShortFileRecTime), byMaxFreeDiskSpaceRate);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tDiskFuPoli: %d; sFileRecTime: 0x%hx; MaxFreeSpace: %d%%;\n", 
                byDiskFullPolicy, wShortFileRecTime, byMaxFreeDiskSpaceRate);
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuStoreSysCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPuStoreSysCfgInfo::NetToHost()
{
    TTnmPuSysCfgInfo::NetToHost();

    wShortFileRecTime = ntohs(wShortFileRecTime);
}
inline void TTnmPuStoreSysCfgInfo::HostToNet()
{
    TTnmPuSysCfgInfo::HostToNet();
    
    wShortFileRecTime = htons(wShortFileRecTime);
}

/*====================================================================
1.2.2.  PPPOE����
====================================================================*/
//PPPOE�����ṹ
typedef struct tagTnmPPPOECfgInfo
{   
    tagTnmPPPOECfgInfo()
    {
        byPPPOEEnable = TNM_ENABLE;
        memset(achADSLUserName, 0, sizeof(achADSLUserName));
        memset(achADSLPassword, 0, sizeof(achADSLPassword));
        byAutoDialEnable = TNM_ENABLE;
        dwAutoDialWait = 60;
        dwAutoDialTry = 3;
        byDebugEnable = TNM_ENABLE;
        wLcpEchoSendInterval = 60;
        wLcpEchoRetryTimes = 3;
    }
    s8  achADSLUserName[TNM_MAX_USER_ID_LEN + 4];   // ADSL�����û������ɶ�д��
    s8  achADSLPassword[TNM_MAX_PASSWORD_LEN + 4];  // ADSL�������루�ɶ�д��
    u8  byPPPOEEnable;              // PPPOEʹ��״̬���ɶ�д��TNM_DISABLE/TNM_ENABLE
    u8  byAutoDialEnable;           // �Զ�����ʹ��״̬���ɶ�д��TNM_DISABLE/TNM_ENABLE
    u8  byDebugEnable;              // Debugʹ��״̬���ɶ�д��TNM_DISABLE/TNM_ENABLE
    u8  byReserved;
    u32 dwAutoDialWait;             // �Զ����ŵȴ�ʱ�䣨�ɶ�д����)
    u32 dwAutoDialTry;              // �Զ��������Դ������ɶ�д��
    u16 wLcpEchoSendInterval;       // �����󣬷���LCP-ECHO����ʱ�������ɶ�д���룩
    u16 wLcpEchoRetryTimes;         // ���ͼ���LCP-ECHO��û�еõ���Ӧ����Ϊ���ӶϿ����ɶ�д��

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "En: %d; UsrName: %s; Pwd: %s;\n", 
            byPPPOEEnable, achADSLUserName, achADSLPassword);

        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tADialEn: %d; ADialWait: 0x%lx; DialTry: 0x%lx;\n",
            byAutoDialEnable, dwAutoDialWait, dwAutoDialTry);
        OspPrintf(TRUE, FALSE, "\tDebugEn: %d; Interval:0x%hx; Times:0x%hx;\n", 
            byDebugEnable, wLcpEchoSendInterval, wLcpEchoRetryTimes);

        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmPPPOECfgInfo;

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPPPOECfgInfo::NetToHost()
{
    dwAutoDialWait = ntohl(dwAutoDialWait);
    dwAutoDialTry = ntohl(dwAutoDialTry);
    wLcpEchoSendInterval = ntohs(wLcpEchoSendInterval);
    wLcpEchoRetryTimes = ntohs(wLcpEchoRetryTimes);
}
inline void TTnmPPPOECfgInfo::HostToNet()
{
    dwAutoDialWait = htonl(dwAutoDialWait);
    dwAutoDialTry = htonl(dwAutoDialTry);
    wLcpEchoSendInterval = htons(wLcpEchoSendInterval);
    wLcpEchoRetryTimes = htons(wLcpEchoRetryTimes);
}

/*====================================================================
1.2.3.  ��Ƶ�����������ͨ����
====================================================================*/
typedef struct tagTnmVideoTypeSet
{
    u8  bySN4;
    u8  byMPEG4;
    u8  byH261;
    u8  byH263;
    u8  byH264;
    u8  byReserved;
    u16 wReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "\tSN4=%d, MPEG4=%d, H261=%d, H263=%d, H264=%d\n",
                bySN4, byMPEG4, byH261, byH263, byH264);
    }
}PACKED TTnmVideoTypeSet;

typedef struct tagTnmVideoResolution
{
    u8  byAuto;
    u8  byQCIF;
    u8  byCIF;
    u8  by2CIF;
    u8  by4CIF;
    u8  byReserved;
    u16 wReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "\tAuto=%d, QCIF=%d, CIF=%d, 2CIF=%d, 4CIF=%d\n",
            byAuto, byQCIF, byCIF, by2CIF, by4CIF);
    }   
}PACKED TTnmVideoResolution;

//��Ƶͼ��ֱ���
typedef struct tagTnmVideoResolutionSet
{
    TTnmVideoResolution tSN4;
    TTnmVideoResolution tMEPG4;
    TTnmVideoResolution tH261;
    TTnmVideoResolution tH263;
    TTnmVideoResolution tH264;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "\tSN4:");
        tSN4.Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tMEPG4:");
        tMEPG4.Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tH261:");
        tH261.Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tH263:");
        tH263.Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tH264:");
        tH264.Print(bNetToHost);
    }
}PACKED TTnmVideoResolutionSet;

typedef struct tagTnmVideoEncCfgInfo
{
    TTnmVideoTypeSet tVideoTypeSet; //�豸֧�ֵ���Ƶ�����ʽ��������ֻ����
    TTnmVideoResolutionSet tVideoResolutionSet; //�豸֧�ֵ���Ƶ�ֱ�����������ֻ����
    u8  byEncodeNo;         // ��Ƶ����ͨ����
    u8  byVideoType;        // ��Ƶ�����ʽ���ɶ�д��
                            // TNM_VIDEO_TYPE_NONE
                            // TNM_VIDEO_TYPE_SN4
                            // TNM_VIDEO_TYPE_MPEG4
                            // TNM_VIDEO_TYPE_H261
                            // TNM_VIDEO_TYPE_H263
                            // TNM_VIDEO_TYPE_H264
    u8  byVideoResolution;  // ��Ƶ�ֱ��ʣ��ɶ�д��
                            // TNM_VIDEO_RESOLUTION_NONE
                            // TNM_VIDEO_RESOLUTION_AUTO
                            // TNM_VIDEO_RESOLUTION_QCIF
                            // TNM_VIDEO_RESOLUTION_CIF
                            // TNM_VIDEO_RESOLUTION_2CIF
                            // TNM_VIDEO_RESOLUTION_4CIF
    u8  byFrameRate;        // ֡�� 1-25���ɶ�д��
    u32 dwBitRate;          // �������� 64K-4M���ɶ�д��kbps
    u8  byQuality;          // ͼ������ ���ɶ�д��TNM_VIDEO_QUALITY_PRIOR/TNM_VIDEO_SPEED_PRIOR
    u8  byMaxQuantization;  // ����������� 1-31���ɶ�д��
    u8  byMinQuantization;  // ��С�������� 1-31���ɶ�д��
    s8  chBrightness;       // ���� -127��128���ɶ�д��
    u16 wInterval;          // �ؼ�֡��� 25-2500���ɶ�д��
    s8  chContrast;         // �Աȶ� -127��128���ɶ�д��
    s8  chSaturation;       // ���Ͷ� -127��128���ɶ�д��
    s8  chColourGrading;      // ɫ��0��255���ɶ�д��
    u8  byReserved;
    u16 wReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "EncNo: %d; VType: %d; VResol: %d; FRate: %d;\n",
            byEncodeNo, byVideoType, byVideoResolution, byFrameRate);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tQuty: %d; BRate: 0x%lx; MaxQu: %d; MinQu: %d;\n",
            byQuality, dwBitRate, byMaxQuantization, byMinQuantization);
        OspPrintf(TRUE, FALSE, "\tInterval: 0x%hx; Bright: %d; Contr: %d; Satur: %d;\n", 
            wInterval, chBrightness, chContrast, chSaturation);
        if (bNetToHost) 
        {
            HostToNet();
        }
        tVideoTypeSet.Print(bNetToHost);
        tVideoResolutionSet.Print(bNetToHost);

    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVideoEncCfgInfo;

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVideoEncCfgInfo::NetToHost()
{
    dwBitRate = ntohl(dwBitRate);
    wInterval = ntohs(wInterval);
}
inline void TTnmVideoEncCfgInfo::HostToNet()
{
    dwBitRate = htonl(dwBitRate);
    wInterval = htons(wInterval);
}

/*====================================================================
1.2.4.  ��Ƶ�����������ͨ����
====================================================================*/
typedef struct tagTnmVideoDecCfgInfo
{
    TTnmVideoTypeSet tVideoTypeSet;     //�豸֧�ֵ���Ƶ�����ʽ��������ֻ����
    u8  byDecodeNo;             // ��Ƶ����ͨ����
    u8  byVideoType;            // ��Ƶ�����ʽ���ɶ�д��TNM_VIDEO_TYPE_H263
    u8  byDecDealLev;           // ͼ�������(0��3��,0��ʾû��)���ɶ�д��
    u8  byVGAOutEnable;         // VGA���ʹ��(ֻ��) TNM_DISABLE, TNM_ENABLE
    u8  byVideoOutputType;      // ��Ƶ����˿����� ���ɶ�д��
                                //  TNM_VIDEO_OUT_TYPE_C
                                //  TNM_VIDEO_OUT_TYPE_S
                                //  TNM_VIDEO_OUT_TYPE_VGA
    u8  byVGAFrequency;         // VGA���ˢ��Ƶ�ʣ��ɶ�д��
                                // TNM_VGA_OUT_FREQUENCY_60
                                // TNM_VGA_OUT_FREQUENCY_75
                                // TNM_VGA_OUT_FREQUENCY_85
    u8  byStarveMode;           // ����Ƶ����ʾ��ʽ (�ɶ�д)
                                //  TNM_STAEVE_MODE_COLOR_BAR
                                //  TNM_STARVE_MODE_LAST_IMAGE
                                //  TNM_STARVE_MODE_BLACK_SCR
                                //  TNM_STARVE_MODE_SHOW_OSD
    u8  byResendEnable;         // �����ش�ʹ��״̬ TNM_DISABLE/TNM_ENABLE
    u16 wResend1stTimeSpan;     // �����1���ش�����
    u16 wResend2ndTimeSpan;     // �����2���ش�����
    u16 wResend3rdTimeSpan;     // �����3���ش�����
    u16 wRejectTimeSpan;        // �������ʱ��

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "DecNo: %d; VType: %d; DecLev: %d; VGAout: %d;\n",
            byDecodeNo, byVideoType, byDecDealLev, byVGAOutEnable);
        OspPrintf(TRUE, FALSE, "\tVOutType: %d; VGAFre: %d; Starve: %d; Resnd: %d;\n",
            byVideoOutputType, byVGAFrequency, byStarveMode, byResendEnable);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tResnd1:0x%hx; Resnd2:0x%hx; Resnd3: 0x%hx;Reject: 0x%hx;\n",
                ntohs(wResend1stTimeSpan), ntohs(wResend2ndTimeSpan), ntohs(wResend3rdTimeSpan),
                ntohs(wRejectTimeSpan));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tResnd1:0x%hx; Resnd2:0x%hx; Resnd3: 0x%hx;Reject: 0x%hx;\n",
                wResend1stTimeSpan, wResend2ndTimeSpan, wResend3rdTimeSpan, wRejectTimeSpan);
        }
        OspPrintf(TRUE, FALSE, "\t");
        tVideoTypeSet.Print(bNetToHost);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVideoDecCfgInfo;

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVideoDecCfgInfo::NetToHost()
{
    wResend1stTimeSpan = ntohs(wResend1stTimeSpan);
    wResend2ndTimeSpan = ntohs(wResend2ndTimeSpan);
    wResend3rdTimeSpan = ntohs(wResend3rdTimeSpan);
    wRejectTimeSpan = ntohs(wRejectTimeSpan);
}
inline void TTnmVideoDecCfgInfo::HostToNet()
{
    wResend1stTimeSpan = htons(wResend1stTimeSpan);
    wResend2ndTimeSpan = htons(wResend2ndTimeSpan);
    wResend3rdTimeSpan = htons(wResend3rdTimeSpan);
    wRejectTimeSpan = htons(wRejectTimeSpan);
}

/*====================================================================
1.2.5.  ��Ƶ������������ͨ����
====================================================================*/
//��Ƶѹ����ʽ
typedef struct tagTnmAudioTypeSet
{
    u8 byPCMA;
    u8 byPCMU;
    u8 byGSM;
    u8 byG729;
    u8 byADPCM;
    u8 byG7221C;
    u8 byReserved1;
    u8 byReserved2;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "PCMA: %d; PCMU: %d; GSM: %d; G729: %d; ADPCM: %d; G7221C: %d\n;",
            byPCMA, byPCMU, byGSM, byG729, byADPCM, byG7221C);
    }
}TTnmAudioTypeSet;

// ��Ƶ�������
typedef struct tagTnmAudioEncCfgInfo
{
    TTnmAudioTypeSet tEncTypeSet;   //�豸֧�ֵ���Ƶ�����ʽ��������ֻ����
    u8  byChannelNo;                //  
    u8  byEncType;                  //��Ƶ�����ʽ���ɶ�д��
                                    //  TNM_AUDIO_TYPE_PCMA
                                    //  TNM_AUDIO_TYPE_PCMU
                                    //  TNM_AUDIO_TYPE_GSM
                                    //  TNM_AUDIO_TYPE_G729
    u8  byInputType;                //��Ƶ�������ͣ��ɶ�д��
                                    //  TNM_AUDIO_INPUTTYPE_LINEIN
                                    //  TNM_AUDIO_INPUTTYPE_MIC
    u8  byEncVolume;                //�����������ɶ�д�� 0-25
    u8  byAECEnable;                //��������ʹ��״̬���ɶ�д��TNM_DISABLE/TNM_ENABLE
    u8  byDumbEnable;               //����ʹ��״̬���ɶ�д�� TNM_DISABLE/TNM_ENABLE
    u8  byCapInput;                 //��Ƶ��������-����Ľ��Ϊ������
                                    //  TNM_CAP_AUDIO_INPUT_LINEIN | TNM_CAP_AUDIO_INPUT_MIC
    u8  byReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "ChNo: %d; EncT: %d; InT: %d; Vol: %d; AEC: %d; Dumb: %d; CapIn: 0x%x\n",
            byChannelNo, byEncType, byInputType, byEncVolume, byAECEnable, byDumbEnable, byCapInput);
        OspPrintf(TRUE, FALSE, "\t");
        tEncTypeSet.Print(bNetToHost);
    }
}TTnmAudioEncCfgInfo;

// ��Ƶ�������
typedef struct tagTnmAudioDecCfgInfo
{
    TTnmAudioTypeSet tDecTypeSet;   // �豸֧�ֵ���Ƶ�����ʽ��������ֻ����
    u8  byChannelNo;                // ��Ƶ����ͨ����
    u8  byDecVolume;                // �����������ɶ�д�� 0-25
    u8  byMuteEnable;               // ����ʹ��״̬���ɶ�д��TNM_DISABLE/TNM_ENABLE
    u8  byReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "ChNo: %d; Vol: %d; Mute: %d;\n",
            byChannelNo, byDecVolume, byMuteEnable);
        OspPrintf(TRUE, FALSE, "\t");
        tDecTypeSet.Print(bNetToHost);
    }
}PACKED TTnmAudioDecCfgInfo;

/*====================================================================
1.2.6.  ��ƵԴ����˿ڲ��������˿ڣ�
====================================================================*/
typedef struct tagTnmVideoInputCfgInfo
{
    u8  byVidInputNo;               // ��ƵԴ����˿ں�
    u8  byCameraType;               // ����ͷ���ͣ��ɶ�д��
    u8  byCameAddrID;               // ����ͷ��ַ�� 1-255���ɶ�д��
    u8  bySerialPortID;             // ����ͷ��Ӧ����ID���ɶ�д��������������
    s8  achVideoPortName[TNM_MAX_VIDEOIN_PORT_NAME_LEN + 4]; //��ƵԴ�˿������ɶ�д��
    s8  achVideoPortOSD[TNM_MAX_STRING_LEN + 4]; //��ƵԴ��Ļ���ݣ��ɶ�д��
    u16 wVideoPortOSDLeftMargin;    // ��ƵԴ��Ļ��߾� 1-200���ɶ�д��
    u16 wVideoPortOSDTopMargin;     // ��ƵԴ��Ļ�ϱ߾� 1-200���ɶ�д��
    u32 dwVideoPortOSDColor;        // ��ƵԴ��Ļ��ɫ���ɶ�д��

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "VidNo: %d; CamType: %d; CamID: %d; SPortID: %d;\n",
            byVidInputNo, byCameraType, byCameAddrID, bySerialPortID);
        OspPrintf(TRUE, FALSE, "\tName: %s; OSD: %s;\n", achVideoPortName, achVideoPortOSD);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tLMar: 0x%hx; TMar: 0x%hx; OSDClr: 0x%x;\n", 
                ntohs(wVideoPortOSDLeftMargin), ntohs(wVideoPortOSDTopMargin),
                ntohl(dwVideoPortOSDColor));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tLMar: 0x%hx; TMar: 0x%hx; OSDClr: 0x%lx;\n", 
                wVideoPortOSDLeftMargin, wVideoPortOSDTopMargin, dwVideoPortOSDColor);
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVideoInputCfgInfo;

//����ͷ����
typedef struct tagTnmCameraInfo
{ 
    u8  byCameraType;               // ����ͷ����
    u8  byReserved;
    u16 wReserved;
    s8  achCameraTypeName[TNM_MAX_CAMERA_NAME_LEN + 4]; //����ͷ������
}TTnmCameraInfo;


/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVideoInputCfgInfo::NetToHost()
{
    wVideoPortOSDLeftMargin = ntohs(wVideoPortOSDLeftMargin);
    wVideoPortOSDTopMargin = ntohs(wVideoPortOSDTopMargin);
    dwVideoPortOSDColor = ntohl(dwVideoPortOSDColor);
}
inline void TTnmVideoInputCfgInfo::HostToNet()
{
    wVideoPortOSDLeftMargin = htons(wVideoPortOSDLeftMargin);
    wVideoPortOSDTopMargin = htons(wVideoPortOSDTopMargin);
    dwVideoPortOSDColor = htonl(dwVideoPortOSDColor);
}

/*====================================================================
1.2.7.  ���ڲ��������˿ڣ�
====================================================================*/
//������������ṹ
typedef struct tagTnmLPTCfgInfo
{
    u8  bySerialNo;             // �˿ں�
    u8  byAlarmDetectState;     // �澯���״̬���ɶ�д��
                                //  TNM_ALARM_DETECT_NONE
                                //  TNM_ALARM_DETECT_ONCE
                                //  TNM_ALARM_DETECT_REPEAT
                                //  TNM_ALARM_DETECT_ALWAYS
    u8  byRelateEncId;          // �澯�����ı���ͨ�� ���ɶ�д���޹�������ͨ����Ϊ TNM_INVALID_CHANNEL_ID
    u8  byOutputLptId;          // �澯�Զ������������ID ���ɶ�д���޿��������Ϊ TNM_INVALID_CHANNEL_ID
    u8  byAlarmOSDEnable;       // �澯��Ļʹ��״̬ TNM_DISABLE-����ʾ, TNM_ENABLE-��ʾ
    u8  byReserved;
    u16 wReserved;
    u32 dwAlarmDetectStartTime[TNM_DAYS_OF_WEEK]; //�澯��⿪ʼʱ�䣨�ɶ�д��
    u32 dwAlarmDetectDuration[TNM_DAYS_OF_WEEK];  //�澯������ʱ�䣨�ɶ�д��
    s8  achAlarmOSDName[TNM_MAX_STRING_LEN + 4];    //�澯��Ļ���ݣ��ɶ�д��

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "Sn: %d; EncId: %d; LptId: %d; OSD: %d\n",
            bySerialNo, byRelateEncId, byOutputLptId, byAlarmOSDEnable);
        OspPrintf(TRUE, FALSE, "\tDetState: %d; OsdName: %s;\n",
            byAlarmDetectState, achAlarmOSDName);
        struct tm tmStime;
        u32 dwTime = 0;
        u32 dwDay = 0;
        for (s32 nIndex = 0; nIndex < TNM_DAYS_OF_WEEK; nIndex++)
        {
            dwTime = dwAlarmDetectStartTime[nIndex];
            dwDay = dwAlarmDetectDuration[nIndex];
            OspPrintf(TRUE, FALSE, "\tDay%d: ", nIndex+1);
            if (bNetToHost) 
            {
                dwTime = ntohl(dwTime);
                dwDay = ntohl(dwDay);
            }

            if (dwTime & 0x80000000)
            {
                OspPrintf(TRUE, FALSE, "\tDur: 0x%lx; �Ƿ�ʱ��ֵ: 0x%lx;\n", dwDay, dwTime);
            }
            else
            {
                tmStime = *localtime((time_t*)&dwTime);
                OspPrintf(TRUE, FALSE, "\tDur: 0x%lx; STime: %s\n", 
                    dwDay, asctime(&tmStime));
            }
        }   
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmLPTCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmLPTCfgInfo::NetToHost()
{
    for(s32 nIndex = 0; nIndex < TNM_DAYS_OF_WEEK; nIndex++)
    {
        dwAlarmDetectStartTime[nIndex] = ntohl(dwAlarmDetectStartTime[nIndex]);
        dwAlarmDetectDuration[nIndex] = ntohl(dwAlarmDetectDuration[nIndex]);
    }
}
inline void TTnmLPTCfgInfo::HostToNet()
{
    for(s32 nIndex = 0; nIndex < TNM_DAYS_OF_WEEK; nIndex++)
    {
        dwAlarmDetectStartTime[nIndex] = htonl(dwAlarmDetectStartTime[nIndex]);
        dwAlarmDetectDuration[nIndex] = htonl(dwAlarmDetectDuration[nIndex]);
    }
}


/*====================================================================
1.2.8.  ���ڲ����������ڣ�
====================================================================*/
//��������
typedef struct tagTnmSerialTypeSet
{
    u8 byRS232;
    u8 byRS422;
    u8 byRS485;
    u8 byReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "RS232: %d; RS422: %d; RS485: %d;\n",
            byRS232, byRS422, byRS485);
    }
}PACKED TTnmSerialTypeSet;

typedef struct tagTnmSerialCfgInfo
{
    TTnmSerialTypeSet tSerialTypeSet;   // ����������������ֻ����
    u32 dwBaudRate;                     // ���ڲ����ʣ��ɶ�д��
    u8  bySerialNo;                     // ���ں�
    u8  byComPortType;                  // �������ͣ��ɶ�д��
                                        //  TNM_SERIAL_LINE_DRIVER_NONE
                                        //  TNM_SERIAL_LINE_DRIVER_RS232
                                        //  TNM_SERIAL_LINE_DRIVER_RS422
                                        //  TNM_SERIAL_LINE_DRIVER_RS485
    u16 wReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "SN: %d; Baud: 0x%x; Comtype: %d;\n",
                bySerialNo, ntohl(dwBaudRate), byComPortType);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "SN: %d; Baud: 0x%lx; Comtype: %d;\n",
                bySerialNo, dwBaudRate, byComPortType);
        }
        OspPrintf(TRUE, FALSE, "\t");
        tSerialTypeSet.Print(bNetToHost);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmSerialCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmSerialCfgInfo::NetToHost()
{
    dwBaudRate = ntohl(dwBaudRate);
}
inline void TTnmSerialCfgInfo::HostToNet()
{
    dwBaudRate = htonl(dwBaudRate);
}
/*====================================================================
1.2.9.  ǰ�˴洢��������ͨ����2417/2428����
====================================================================*/
typedef struct tagTnmStorageCfgInfo
{
    u8  byStorageChanNo;                // �洢ͨ����
    u8  byAlarmRecEnable;               // �澯¼��ʹ�ܣ��ɶ�д��TNM_DISABLE/TNM_ENABLE
    u8  bySkipRecEnable;                // ��֡¼��ʹ�ܣ��ɶ�д��TNM_DISABLE/TNM_ENABLE
    u8  byTimeRecEnable;                // ��ʱ¼��ʹ�ܣ��ɶ�д��TNM_DISABLE/TNM_ENABLE
    u32 dwAlarmPreRecTime;              // �澯Ԥ¼ʱ�䣨0����ʾ��Ԥ¼���ɶ�д��(��)
    u32 dwRecTimeAfterRecover;          // �澯�ָ������¼��ʱ�䣨�ɶ�д��(��)
    u16 wSkipRecInterval;               // ��֡¼��ؼ�֡�����֡�ĸ��������ɶ�д��
    u8  byCycleRecEnable;               // ����¼��ʹ�ܣ��ɶ�д��TNM_DISABLE/TNM_ENABLE
    u8  byReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "ChNo: %d; RecEn: %d; PreRecT: 0x%x; RecT: 0x%x;\n",
                byStorageChanNo, byAlarmRecEnable, ntohl(dwAlarmPreRecTime), ntohl(dwRecTimeAfterRecover));
            OspPrintf(TRUE, FALSE, "\tSkipEn: %d; SkipInt: 0x%hx; TimeREn: %d; Cycle: %d;\n",
                bySkipRecEnable, ntohs(wSkipRecInterval), byTimeRecEnable, byCycleRecEnable);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "ChNo: %d; RecEn: %d; PreRecT: 0x%lx; RecT: 0x%lx;\n",
                byStorageChanNo, byAlarmRecEnable, dwAlarmPreRecTime, dwRecTimeAfterRecover);
            OspPrintf(TRUE, FALSE, "\tSkipEn: %d; SkipInt: 0x%hx; TimeREn: %d; Cycle: %d;\n",
                bySkipRecEnable, wSkipRecInterval, byTimeRecEnable, byCycleRecEnable);
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmStorageCfgInfo;

typedef struct tagPeriodInterval
{
    u32 dwStartTime;        // ��ʼʱ��
    u32 dwEndTime;          // ����ʱ��
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwStime = dwStartTime;
        u32 dwEtime = dwEndTime;
        if (bNetToHost)
        {
            dwStime = ntohl(dwStime);
            dwEtime = ntohl(dwEtime);
        }        
        struct tm *ptm = NULL;
        ptm = localtime((time_t*)&dwStime);
        if (ptm ==NULL)
        {
            OspPrintf(TRUE, FALSE, "\t 1970.01.01.00.00\n");
        }
        OspPrintf(TRUE, FALSE, "\tstart:%s", asctime(ptm));

        ptm = localtime((time_t*)&dwEtime);
        if (ptm ==NULL)
        {
            OspPrintf(TRUE, FALSE, "\t 1970.01.01.00.00\n\n");
        }
        OspPrintf(TRUE, FALSE, "\tend:%s\n", asctime(ptm));
    }
    void NetToHost()
    {
        dwStartTime = ntohl(dwStartTime);
        dwEndTime = ntohl(dwEndTime);
    }
    void HostToNet()
    {
        dwStartTime = htonl(dwStartTime);
        dwEndTime = htonl(dwEndTime);
    }
}PACKED TPeriodInterval;


#define TTNM_PU_RECORD_TIMES_EACH_DAY   3   //ÿ��¼���ʱ����Ŀ

#define PU_RECORD__EACHDAY    0   //ÿ��¼��
#define PU_RECORD__PEROID     1   //����¼��
typedef struct TTnmPuPeriodPolicy
{
    TPeriodInterval tPeriod[TNM_DAYS_OF_WEEK][TTNM_PU_RECORD_TIMES_EACH_DAY];//��byPolicyΪPU_RECORD__EACHDAYʱ
                                                                         //tPeriod[0][0],tPeriod[0][1],tPeriod[0][2]��Ч
                                                                         //��ʾÿ��¼���3��ʱ��
                                                                         //��byPolicyΪPU_RECORD__PEROIDʱtPeriod����Ԫ��
                                                                         //��ʾһ������ÿ��¼���3��ʱ��
    u8 byChanNo;                                //ͨ����
    u8 byPolicy;                                //¼����ԣ�����¼��(PU_RECORD__PEROID)/ÿ��¼��(PU_RECORD__EACHDAY)
    u8 byReserved;
    u8 byReserved1;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "ChanNo:%d; Policy:%d\n", byChanNo, byPolicy);
        OspPrintf(TRUE, FALSE, " Record Time:");
        s32 nIndex = 0;
        
        for (nIndex = 0; nIndex < TNM_DAYS_OF_WEEK;nIndex++)
        {
            OspPrintf(TRUE, FALSE, "\t%d:\n",nIndex);
            tPeriod[nIndex][0].Print(bNetToHost);
            tPeriod[nIndex][1].Print(bNetToHost);
            tPeriod[nIndex][2].Print(bNetToHost);
        }
    }
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuPeriodPolicy;

inline void TTnmPuPeriodPolicy::NetToHost()
{
    u32 dwIndex = 0;
    for (; dwIndex < TNM_DAYS_OF_WEEK; dwIndex++)
    {
        tPeriod[dwIndex][0].NetToHost();
        tPeriod[dwIndex][1].NetToHost();
        tPeriod[dwIndex][2].NetToHost();
    }
}

inline void TTnmPuPeriodPolicy::HostToNet()
{
    u32 dwIndex = 0;
    for (; dwIndex < TNM_DAYS_OF_WEEK; dwIndex++)
    {
        tPeriod[dwIndex][0].HostToNet();
        tPeriod[dwIndex][1].HostToNet();
        tPeriod[dwIndex][2].HostToNet();
    }
}
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmStorageCfgInfo::NetToHost()
{
    dwAlarmPreRecTime = ntohl(dwAlarmPreRecTime);
    dwRecTimeAfterRecover = ntohl(dwRecTimeAfterRecover);
    wSkipRecInterval = ntohs(wSkipRecInterval);
}
inline void TTnmStorageCfgInfo::HostToNet()
{
    dwAlarmPreRecTime = htonl(dwAlarmPreRecTime);
    dwRecTimeAfterRecover = htonl(dwRecTimeAfterRecover);
    wSkipRecInterval = htons(wSkipRecInterval);
}

/*====================================================================
1.3.    ƽ̨�豸���ò���
====================================================================*/
/*====================================================================
1.3.1.  CMU���ò���
====================================================================*/
typedef struct tagTnmCmuCfgInfo : public TTnmPubCfgInfo
{
    TTnmDeviceId tDevId;    // CMU��ID��ֻ����
    u32 dw3asIp;            // 3AS��IP���ɶ�д��
    u32 dwUasIp;            // UAS��IP���ɶ�д��
    u32 dwParentCmuIp;      // �ϼ�CMU��IP��ֻ����
    u16 wPuNatPort;         // PU��ԽNAT�Ķ˿ںţ�0��ʾ������
    u16 wCuNatPort;         // PU��ԽNAT�Ķ˿ںţ�0��ʾ������
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tDevId=%s, 3as=%s, ", tDevId.achID, ::GetIpStr(dw3asIp));
        OspPrintf(TRUE, FALSE, "Uas=%s\n", ::GetIpStr(dwUasIp));
        OspPrintf(TRUE, FALSE, "\tPCmuIp: %s; PuPort: 0x%hx; CuPort: 0x%hx;\n",
            ::GetIpStr(dwParentCmuIp), wPuNatPort, wCuNatPort);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmCmuCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmCmuCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dw3asIp = ntohl(dw3asIp);
    dwUasIp = ntohl(dwUasIp);
    dwParentCmuIp = ntohl(dwParentCmuIp);
    wPuNatPort = ntohs(wPuNatPort);
    wCuNatPort = ntohs(wCuNatPort);
}
inline void TTnmCmuCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    dw3asIp = htonl(dw3asIp);
    dwUasIp = htonl(dwUasIp);
    dwParentCmuIp = htonl(dwParentCmuIp);
    wPuNatPort = htons(wPuNatPort);
    wCuNatPort = htons(wCuNatPort);
}

typedef struct tagTnmLadenCfgInfo : public TTnmPubCfgInfo
{
    TTnmDeviceId tDevId;    //CMU��ID��ֻ����
    u32 dwCmuIp;            //�����CMU��ַ
    u16 dwCmuPort;          //�����CMU�˿ں�
    u16 wListenPort;        //����PU��CU���ӵĶ˿ں�
    u16 wCheckLinkInterval; //��CMU��������ʱ����(��)
    u8  byCheckLinkNum;     //��CMU�������Ĵ���
    u8  byReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tDevId=%s, CmuIp=%s, ", tDevId.achID, ::GetIpStr(dwCmuIp));
        OspPrintf(TRUE, FALSE, "CmuPort=%d ListenPort=%d\n", dwCmuPort, wListenPort);
        OspPrintf(TRUE, FALSE, "\tSynInterval=%d; CheckLinkNum=%d\n", wCheckLinkInterval, byCheckLinkNum);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }

    void NetToHost();
    void HostToNet();
}TTnmLadenCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmLadenCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwCmuIp = ntohl(dwCmuIp);
    dwCmuPort = ntohs(dwCmuPort);
    wListenPort = ntohs(wListenPort);
    wCheckLinkInterval = ntohs(wCheckLinkInterval);
}
inline void TTnmLadenCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    dwCmuIp = htonl(dwCmuIp);
    dwCmuPort = htons(dwCmuPort);
    wListenPort = htons(wListenPort);
    wCheckLinkInterval = htons(wCheckLinkInterval);
}

/*====================================================================
1.3.3.  �������ò���
====================================================================*/
typedef struct tagTnmKeyboardCfgInfo
{
    u8  byIndex; //����������ţ��ɶ�д��
    u8  byId;    //�����豸ID��ֻ����
    u8  byPos;   //���̵ķ���λ�ã��ɶ�д��
    u8  byReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "Index: %d; Id: %d; Pos: %d;\n",
            byId, byId, byPos);
    }
}PACKED TTnmKeyboardCfgInfo;

/*====================================================================
1.3.4.  NRU���ò���
====================================================================*/
typedef struct tagTnmNruCfgInfo : public TTnmPubCfgInfo
{
    TTnmDeviceId    tNruId;     // NRU��ţ�ֻ����
    u32 dwCmuIp;                // CMU������ַ���ɶ�д��
    s8  achRegCode[32];         // NRUע���루�ɶ�д��?? to do
    u16 wCmuPort;               // CMU�����˿ڣ��ɶ�д��
    u16 dwNruListenIp;          // �����ظ�������ʹ��
    TTnmUserId  tUserId;        // ����ͻ���ţ��ɶ�д��
    u8  byMaxPreRecChanNum;     // ���Ԥ¼ͨ���������ɶ�д��
    u8  byReserved;
    u16 wReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tNruId: %s; UsrId: %s; RegCode: %s;\n",
            tNruId.achID, tUserId.achID, achRegCode);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tCIp: %s; CPort: 0x%hx; MaxChNum: %d;\n",
            ::GetIpStr(dwCmuIp), wCmuPort, byMaxPreRecChanNum);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmNruCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmNruCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();
    
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
}

/*====================================================================
1.3.5.  �໭�渴�����ò���
====================================================================*/
typedef struct tagTnmVpuCfgInfo
{
    u16 wSwitchId;       //VPUʹ�õĽ����������ţ��ɶ�д��
    u16 wSwitchPort;     //VPU������������ʼ�˿ںţ��ɶ�д��
    u8  byIndex;         //VPU������ţ��ɶ�д��
    u8  byId;            //VPU�豸ID���ɶ�д��
    u16 wReserved;
    s8  achAlias[TNM_MAX_STRING_LEN + 4]; //VPU�豸�������ɶ�д��

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "Index: %d; Id: %d; SwId: 0x%hx; SwPort: 0x%hx;\n",
                byIndex, byId, ntohs(wSwitchId), ntohs(wSwitchPort));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "Index: %d; Id: %d; SwId: 0x%hx; SwPort: 0x%hx;\n",
                byIndex, byId, wSwitchId, wSwitchPort);
        }
        OspPrintf(TRUE,FALSE, "\tAlias: %s;\n", achAlias);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVpuCfgInfo::NetToHost()
{
    wSwitchId = ntohs(wSwitchId);
    wSwitchPort = ntohs(wSwitchPort);
}
inline void TTnmVpuCfgInfo::HostToNet()
{
    wSwitchId = htons(wSwitchId);
    wSwitchPort = htons(wSwitchPort);
}
/*====================================================================
1.3.6.  ��Ƶ�������ò���
====================================================================*/
typedef struct tagTnmMatrixCfgInfo
{
    u8  byIndex;  //�豸������ţ��ɶ�д��
    u8  byId;     //�豸ID��ֻ����
    u16 wReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "Index: %d; Id: %d;\n", byIndex, byId);
    }
}PACKED TTnmMatrixCfgInfo;


/*====================================================================
1.4.    �໭�渴����(VPU)���ò���
====================================================================*/
/*====================================================================
1.4.1.  ϵͳ���ò���
====================================================================*/
typedef struct tagTnmVpuSysCfgInfo : public TTnmPubCfgInfo
{
    tagTnmVpuSysCfgInfo()
    {
        tVpuId.SetNull();
        dwRegCMUIp = 0;
        wRegCMUPort = 0;
        byVpuStyle = 0;
        byEncodeType = 0;
        byMaxQualValue = 0;
        byMinQualValue = 0;
        dwEcdBitRate = 0;
        dwBandWidth = 0;
        wFrameRate = 0;
        byImageQuality = 0;
        wVideoWidth = 0;
        wVideoHeight = 0;
        byVideoOutput = 0;
        memset(achAlias, 0, sizeof(achAlias));
        
    }
    TTnmDeviceId  tVpuId;                       //�豸ȫ��ΨһID���ɶ�д��
    u32 dwRegCMUIp;                             //ע���ƽ̨IP��ַ���ɶ�д��
    u16 wRegCMUPort;                            //ע���ƽ̨�˿ںţ��ɶ�д��
    u8  byVpuStyle;                             //�໭���񣨿ɶ�д��TNM_VPU_STYLE_HTWO
    u8  byEncodeType;                           //VPU ��������
                                                //TNM_VIDEO_TYPE_MPEG4/TNM_VIDEO_TYPE_H261/TNM_VIDEO_TYPE_H263
    u8  byMaxQualValue;                         //�����������(1-31)
    u8  byMinQualValue;                         //��С��������(1-31)
    u16 wIFramInterval;             //�ؼ�����
    u32 dwEcdBitRate;                           //���������(kbps)
    u32 dwBandWidth;                            //���緢�ʹ���(kbps)
    u16 wFrameRate;                             //����
    u8  byImageQuality;                         //ͼ��ѹ������(1-5)
    u16 wVideoWidth;                            //ͼ����
    u16 wVideoHeight;                           //ͼ��߶�
    s8  achAlias[TNM_MAX_ALIAS_NAME_LEN + 4];       //�豸�������ɶ�д��
    u8  byVideoOutput;                          //�Ƿ񱳰���� 0����� /1���
    u8  byReserved1;
    u8  byReserved2;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tStyle: %d; RegIp: %s; RegPort: %d; EcdType: %d;\n",
                byVpuStyle, ::GetIpStr(ntohl(dwRegCMUIp)), ntohs(wRegCMUPort), byEncodeType);
            OspPrintf(TRUE, FALSE, "\tMaxQual: %d; MinQual: %d; EcdRate: %d; BandWidth: %d;\n", 
                byMaxQualValue, byMinQualValue, ntohl(dwEcdBitRate), ntohl(dwBandWidth));
            OspPrintf(TRUE, FALSE, "\tFrameRate:%d; ImageQual:%d; Vwidth: %d; Vheight:%d;\n",
                ntohs(wFrameRate), byImageQuality, ntohs(wVideoWidth), ntohs(wVideoHeight));

        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tStyle: %d; RegIp: %s; RegPort: %hu; EcdType: %d;\n",
                byVpuStyle, ::GetIpStr(dwRegCMUIp), wRegCMUPort, byEncodeType);
            OspPrintf(TRUE, FALSE, "\tMaxQual: %d; MinQual: %d; EcdRate: %lu; BandWidth: %lu;\n", 
                byMaxQualValue, byMinQualValue, dwEcdBitRate, dwBandWidth);
            OspPrintf(TRUE, FALSE, "\tFrameRate:%d; ImageQual:%d; Vwidth: %hu; Vheight:%hu;\n",
                wFrameRate, byImageQuality, wVideoWidth, wVideoHeight);
        }
        OspPrintf(TRUE, FALSE, "\tUnitNo: %s; Alias: %s; Videoout: %d\n", tVpuId.achID, achAlias, byVideoOutput);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuSysCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVpuSysCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwRegCMUIp = ntohl(dwRegCMUIp);
    wRegCMUPort = ntohs(wRegCMUPort);
    dwEcdBitRate = ntohl(dwEcdBitRate);
    dwBandWidth = ntohl(dwBandWidth);
    wFrameRate = ntohs(wFrameRate);
    wVideoWidth = ntohs(wVideoWidth);
    wVideoHeight = ntohs(wVideoHeight);
    wIFramInterval = ntohs(wIFramInterval);
}
inline void TTnmVpuSysCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    dwRegCMUIp = htonl(dwRegCMUIp);
    wRegCMUPort = htons(wRegCMUPort);
    dwEcdBitRate = htonl(dwEcdBitRate);
    dwBandWidth = htonl(dwBandWidth);
    wFrameRate = htons(wFrameRate);
    wVideoWidth = htons(wVideoWidth);
    wVideoHeight = htons(wVideoHeight);
    wIFramInterval = htons(wIFramInterval);
}
/*====================================================================
1.5.    ��Ƶ����VAS�������ò���
====================================================================*/
/*====================================================================
1.5.1.  ϵͳ���ò���
====================================================================*/
typedef struct tagTnmVasSysCfgInfo : public TTnmPubCfgInfo
{
    u32 dwRegCMUIp;                     //ע���ƽ̨IP��ַ���ɶ�д��
    u16 wRegCMUPort;                    //ע���ƽ̨�˿ںţ��ɶ�д��
    u16 wRegCMUNo;                      //ע���ƽ̨ΨһID���ɶ�д��
    u8  byUnitNo;                       //�豸ȫ��ΨһID���ɶ�д��
    u8  byMaxInputPort;                 //�������˿ڣ��ɶ�д��
    u8  byMaxOutputPort;                //�������˿ڣ��ɶ�д��
    u8  byReserved;
    s8  achAlias[TNM_MAX_STRING_LEN + 4]; //�豸�������ɶ�д��

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tRegIp: %s; RegPort: 0x%hx; RegNo: 0x%hx; UniNo: %d;\n",
                ::GetIpStr(ntohl(dwRegCMUIp)), ntohs(wRegCMUPort), ntohs(wRegCMUNo), byUnitNo);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tRegIp: %s; RegPort: 0x%hx; RegNo: 0x%hx; UniNo: %d;\n",
                ::GetIpStr(dwRegCMUIp), wRegCMUPort, wRegCMUNo, byUnitNo);
        }
        OspPrintf(TRUE, FALSE, "\tMaxInPort: %d; OutPort: %d; Alias: %s;\n",
            byMaxInputPort, byMaxOutputPort, achAlias);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVasSysCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVasSysCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwRegCMUIp = ntohl(dwRegCMUIp);
    wRegCMUPort = ntohs(wRegCMUPort);
    wRegCMUNo = ntohs(wRegCMUNo);
}
inline void TTnmVasSysCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    dwRegCMUIp = htonl(dwRegCMUIp);
    wRegCMUPort = htons(wRegCMUPort);
    wRegCMUNo = htons(wRegCMUNo);
}

//1.6.  CUI���ò���
typedef struct tagTnmCuiCfgInfo : public TTnmPubCfgInfo
{
    u16 wListenPort;                            // �����˿�
    u16 wMaxCuLinkNum;                          // �ͻ������������
    TTnmDeviceId tDevId;                        // CUIģ���ID
    TTnmDeviceId tCmuId;                        // ���ӵ�CMU��ID
    u32 dwCmuIp;                                // ���ӵ�CMU��IP
    u16 wCmuPort;                               // ���ӵ�CMU�������˿�
    u16 wReserved;
    s8  achCuiPwd[TNM_MAX_PASSWORD_LEN + 4];    // CUI��CMUע�������

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tLnPort: 0x%hx; LkNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
                ntohs(wListenPort), ntohs(wMaxCuLinkNum), ::GetIpStr(ntohl(dwCmuIp)), ntohs(wCmuPort));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tLnPort: 0x%hx; LkNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
                wListenPort, wMaxCuLinkNum, ::GetIpStr(dwCmuIp), wCmuPort);
        }
        OspPrintf(TRUE, FALSE, "\tDevId: %s; CmuId: %s; CuiPwd: %s;\n",
            tDevId.achID, tCmuId.achID, achCuiPwd);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuiCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmCuiCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    wListenPort = ntohs(wListenPort);
    wMaxCuLinkNum = ntohs(wMaxCuLinkNum);
    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmCuiCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    wListenPort = htons(wListenPort);
    wMaxCuLinkNum = htons(wMaxCuLinkNum);
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
}

//1.7.  PUI���ò���
typedef struct tagTnmPuiCfgInfo : public TTnmPubCfgInfo
{
    u16 wListenPort;                         // ���������˿�
    u16 wMaxPuNum;                           // ���PU��������
    u32 dwCmuIp;                             // ���ӵ�CMU��IP��ַ
    u16 wCmuPort;                            // ����CMU�Ķ˿ں�
    u16 wReserved;
    TTnmDeviceId tCmuId;                     // ���ӵ�CMU���豸ID
    TTnmDeviceId tDevId;                     // PUIģ���ID
    s8  achPuiPwd[TNM_MAX_PASSWORD_LEN + 4]; // PUI��CMUע�������
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tLnPort: 0x%hx; PuNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
                ntohs(wListenPort), ntohs(wMaxPuNum), ::GetIpStr(ntohl(dwCmuIp)), ntohs(wCmuPort));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tLnPort: 0x%hx; PuNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
                wListenPort, wMaxPuNum, ::GetIpStr(dwCmuIp), wCmuPort);
        }
        OspPrintf(TRUE, FALSE, "\tCmuId: %s; DevId: %s; PuiPwd: %s;\n",
            tCmuId.achID, tDevId.achID, achPuiPwd);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuiCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPuiCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    wListenPort = ntohs(wListenPort);
    wMaxPuNum = ntohs(wMaxPuNum);
    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmPuiCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    wListenPort = htons(wListenPort);
    wMaxPuNum = htons(wMaxPuNum);
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
}
// 1.8 VTDU���ò���
typedef struct tagTnmVtduCfgInfo : public TTnmPubCfgInfo
{
    u32 dwCmuIp;                                // ���ӵ�CMU��IP��ַ
    u16 wMaxSwitchNum;                          // ��󽻻���������
    u16 wCmuPort;                               // ����CMU�Ķ˿ں�
    TTnmDeviceId tCmuId;                        // ���ӵ�CMU���豸ID
    TTnmDeviceId tDevId;                        // VTDU���豸ID
    s8  achPwd[TNM_MAX_PASSWORD_LEN + 4];       // VTDU��CMUע�������
    u8  bySwitchNumAlarmRate;                   // ת��·���澯�İٷֱȣ�1--99��
    u8  byReserved;
    u16 wReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tSwNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
            wMaxSwitchNum, ::GetIpStr(dwCmuIp), wCmuPort);

        OspPrintf(TRUE, FALSE, "\tCmuId=%s\n\tDevId=%s, Pwd=%s\n",
            tCmuId.achID, tDevId.achID, achPwd);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVtduCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVtduCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    wMaxSwitchNum = ntohs(wMaxSwitchNum);
    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmVtduCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    wMaxSwitchNum = htons(wMaxSwitchNum);
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
} 
// 1.9 UAS���ò���
typedef struct tagTnmUasCfgInfo : public TTnmPubCfgInfo
{
    u32 dwCmuIp;                                // ���ӵ�CMU��IP��ַ
    u16 wCmuPort;                               // ����CMU�Ķ˿ں�
    u16 wReserved;
    TTnmDeviceId tCmuId;                        // ���ӵ�CMU���豸ID
    TTnmDeviceId tDevId;                        // UAS���豸ID
    s8  achPwd[TNM_MAX_PASSWORD_LEN + 4];       // UAS��CMUע�������

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }

        OspPrintf(TRUE, FALSE, "\tCmuIp=%s, CmuPort=0x%hx\n\tCmuId=%s\n",
            ::GetIpStr(dwCmuIp), wCmuPort, tCmuId.achID);
        
        OspPrintf(TRUE, FALSE, "\tDevId=%s, Pwd=%s\n",
            tDevId.achID, achPwd);

        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmUasCfgInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmUasCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmUasCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();
    
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
} 


// 1.10 3AS���ò���
typedef struct tagTnm3asCfgInfo : public TTnmPubCfgInfo
{
    tagTnm3asCfgInfo()
    {
        memset(achAlias, 0, sizeof(achAlias));
    }
    s8  achAlias[TNM_MAX_ALIAS_NAME_LEN + 4];       // UAS��CMUע�������

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tAlias:%s\n", achAlias);

        }
    }
    void TransByteOrder(BOOL32 bToHost = TRUE);
}PACKED TTnm3asCfgInfo;

/*========================================================================
�� �� ����TransByteOrder
��    �ܣ�ת���ֽ���ӿ�
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/13  3.9     ���ǿ  ����
========================================================================*/
inline void TTnm3asCfgInfo::TransByteOrder(BOOL32 bToHost)
{
    if (bToHost) 
    {
        TTnmPubCfgInfo::NetToHost();
    }
    else
    {
        TTnmPubCfgInfo::HostToNet();
    }
}


/********************************************************************/
/* 2. ���ܹ���ϵͳ״̬�����ܵ�ʵʱ��ѯ                            */
/********************************************************************/
/*====================================================================
2.1. �������ܲ���
====================================================================*/
// ���������ܲ���
typedef struct tagTnmBasePfmInfo
{
    u32 dwDeviceId;         // ��ֵΪNMSΪ�豸�����õ�ID��ǰ���豸����Ҫ��д����
    u32 dwSysTime;          // ϵͳʱ��
    
    u8  byCpuUseRate;       // CPUʹ����(0-100)
    u8  byMemUseRate;       // �ڴ�ʹ����(0-100)
    u8  byMemStatus;        // �ڴ�ʹ��״̬(enum TnmStatusType)
    u8  bySysLoad;          // ϵͳ����(0-100)
    s8  achHardwareVer[TNM_MAX_VERSION_LEN + 4];        // �豸�汾��
    s8  achSoftwareVer[TNM_MAX_VERSION_LEN + 4];        // �豸����汾��
    s8  achCompileTime[TNM_MAX_COMPILE_TIME_LEN + 4];   // �������ʱ��
    /////u8  byTemperature;     // �豸��ǰ���¶ȣ�0--100��
    /////u8  byReserved;
    /////u16 wReserved;

    void Init()
    {
        dwDeviceId  = 0;
        byCpuUseRate = 0;
        byMemUseRate = 0;
        byMemStatus = TNM_STATUS_NONE;
        dwSysTime   = 0;
        bySysLoad   = 0;
        memset(achHardwareVer, 0, sizeof(achHardwareVer));
        memset(achSoftwareVer, 0, sizeof(achSoftwareVer));
        memset(achCompileTime, 0, sizeof(achCompileTime));
        /////byTemperature = 0;
    }
    const tagTnmBasePfmInfo* GetBasePfmInfoPtr() const
    {
        return this;
    }

    void AdjustSysTime(BOOL32 bDecTime = TRUE);

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tDevId: 0x%lx; CpuRate: %d; SysTime: %s;\n", 
            dwDeviceId, byCpuUseRate, ::GetTimeStr(dwSysTime));
            
        OspPrintf(TRUE, FALSE, "\tMemRate: %d; MemStatus: %d; CompileT: %s\n", 
            byMemUseRate, byMemStatus, achCompileTime); 
        OspPrintf(TRUE, FALSE, "\tSysLoad: %d; SoftV: %s; HardV: %s;\n",
            bySysLoad, achSoftwareVer, achHardwareVer);

        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    void NetToHost();
    void HostToNet();
}TTnmBasePfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmBasePfmInfo::NetToHost()
{
    dwSysTime = ntohl(dwSysTime);
    dwDeviceId = ntohl(dwDeviceId);
}
inline void TTnmBasePfmInfo::HostToNet()
{
    dwSysTime = htonl(dwSysTime);
    dwDeviceId = htonl(dwDeviceId);
} 


/*========================================================================
�� �� ����AdjustSysTime
��    �ܣ����ڲ�ͬ����ϵͳ��ʱ���׼��һ���������Ҫ��ϵͳʱ����е���
��    ����BOOL32 bDecTime = TRUE        [in]�Ƿ�Ҫ��ʱ��ֵ��С
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/06/01  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmBasePfmInfo::AdjustSysTime(BOOL32 bDecTime)
{
    if (bDecTime) 
    {
        dwSysTime -= 3600 * 8;
    }
    else
    {
        dwSysTime += 3600 * 8;
    }
}


typedef struct tagTnmNmsPfmInfo : public  TTnmBasePfmInfo
{
}TTnmNmsPfmInfo;

// Ӳ���豸�����ܲ���
typedef struct tagTnmPfmHwInfo : public TTnmBasePfmInfo
{
    u8  abyPanelLed[TNM_MAX_PANELLED_LEN];      // �豸���Ƶ�״̬(һ��u8����һյ��) enum TnmLedStatus
    u8  abyEnternetStatus[TNM_MAX_ETHERNET_NUM + 3];// ��̫����״̬(һ��u8����һ������) enum TnmEnternetStatus
    u8  abyMapStatus[TNM_MAX_MAP_NUM + 3];          // MAP״̬(һ��u8����һ��MAP)enum TnmStatusType
    u8  byWatchDogEnalbe;                       // �豸��Ӳ�����Ź�ʹ��״̬(TNM_DISABLE/TNM_ENABLE)
    u8  byReserved;
    u16 wReserved;

    /////u8  abyEthLostPkRate[TNM_MAX_ETHERNET_NUM + 3];// ��̫���ڶ����ʣ�0--100��
    /////u32 abyEthBps[TNM_MAX_ETHERNET_NUM + 3];       // ��̫������������λbps��

    void Init()
    {
        TTnmBasePfmInfo::Init();
        byWatchDogEnalbe    = TNM_DISABLE;
        s32 nIndex = 0;
		for(nIndex = 0; nIndex < TNM_MAX_PANELLED_LEN; nIndex++)
		{
			abyPanelLed[nIndex] = TNM_LED_NONE;
		}
		for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
		{
			abyEnternetStatus[nIndex] = TNM_ETHERNET_NONE;
			/////abyEthLostPkRate[nIndex] = 0;
			//////abyEthBps[nIndex] = 0;
		}
		for(nIndex = 0; nIndex < TNM_MAX_MAP_NUM; nIndex++)
		{
			abyMapStatus[nIndex] = TNM_STATUS_NONE;
		}
	}

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBasePfmInfo::Print(bNetToHost);

        s32 nIndex;

        OspPrintf(TRUE, FALSE, "\tWDog: %d; MapStatus: ", byWatchDogEnalbe);
        for (nIndex = 0; nIndex < TNM_MAX_MAP_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyMapStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tLed: ");
        for (nIndex = 0; nIndex < TNM_MAX_PANELLED_LEN; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyPanelLed[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
		OspPrintf(TRUE, FALSE, "\tEthNet: ");
        for (nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyEnternetStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmPfmHwInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPfmHwInfo::NetToHost()
{
    /////s32 nIndex = 0;

    TTnmBasePfmInfo::NetToHost();

    /////for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    /////{
        /////abyEthBps[nIndex] = ntohl(abyEthBps[nIndex]);
    /////}
}
inline void TTnmPfmHwInfo::HostToNet()
{
    /////s32 nIndex = 0;

    TTnmBasePfmInfo::HostToNet();

    /////for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    /////{
    /////    abyEthBps[nIndex] = htonl(abyEthBps[nIndex]);
    /////}
} 

typedef struct tagTnmLogonInfo
{
    tagTnmLogonInfo()
    {
        memset(this, 0, sizeof(tagTnmLogonInfo));
    }
    s8  achUserName[TNM_MAX_USER_NAME_LEN + 4];     // ��¼�û���
    u32 dwCuIP;                                 // ��¼IP��ַ
    u32 dwLoginTime;                            // ��¼ʱ��
    void Print(BOOL32 bNetToHost = FALSE)
    {       
        u32 dwIp = dwCuIP;
        u32 dwTime = dwLoginTime;
        if (bNetToHost) 
        {
            dwTime = ntohl(dwTime);
            dwIp = ntohl(dwIp);
        }
        OspPrintf(TRUE, FALSE, "\tUsrname=%s, CuIp=%s, Time=%s\n", 
            achUserName, ::GetIpStr(dwIp), ::GetTimeStr(dwTime));
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmLogonInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmLogonInfo::NetToHost()
{
    dwCuIP = ntohl(dwCuIP);
    dwLoginTime = ntohl(dwLoginTime);
}
inline void TTnmLogonInfo::HostToNet()
{
    dwCuIP = htonl(dwCuIP);
    dwLoginTime = htonl(dwLoginTime);
}

/*====================================================================
2.2. �������ܲ���
====================================================================*/

typedef struct tagTnmBoardPfmInfo : public TTnmPfmHwInfo
{
    u8  byLayer;            //�豸�Ĳ��
    u8  bySlot;             //�豸�Ĳۺ�

    u8  byReserved1;
    u8  byReserved2;

    void Init()
    {
        TTnmPfmHwInfo::Init();
        
        byLayer = 0;
        bySlot = 0;
    }
    /*========================================================================
    �� �� ����GetBoardPfmInfo
    ��    �ܣ��õ���������ܲ���ָ��
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/21  3.9     ���ǿ  ����
    ========================================================================*/
    const tagTnmBoardPfmInfo * GetBoardPfmInfo() const;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPfmHwInfo::Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tLayer: %d; Slot: %d;\n", byLayer, bySlot);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmBoardPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmBoardPfmInfo::NetToHost()
{
    TTnmPfmHwInfo::NetToHost();
}
inline void TTnmBoardPfmInfo::HostToNet()
{
    TTnmPfmHwInfo::HostToNet();
} 
/*========================================================================
�� �� ����GetBoardPfmInfo
��    �ܣ��õ���������ܲ���ָ��
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/21  3.9     ���ǿ  ����
========================================================================*/
inline const TTnmBoardPfmInfo * TTnmBoardPfmInfo::GetBoardPfmInfo() const
{
    return this;
}


/*====================================================================
2.3. ǰ���豸���ܲ���
====================================================================*/
#define INVALID_DISK    (u8)0xFF
typedef struct tagTnmPuPfmInfo : public TTnmPfmHwInfo
{
    u8  abyRcvLosePacketRate[TNM_MAX_CHAN_NUM];     //���ն����ʣ�ָ���1����֮�ڵ�ƽ�������ʣ���ѯ
                                                    //һ��u8����һ��ͨ����100%
                                                    //TNM_INVALID_CHANNEL_ID	��Чֵ
    u8  abyCodecState[TNM_MAX_CHAN_NUM];            //����������շ�״̬��ѯ(һ��u8����һ��ͨ��)
                                                    //TNM_INVALID_CHANNEL_ID	��Чֵ
                                                    //TNM_CODEC_STOP
                                                    //TNM_ENCODING
                                                    //TNM_DECODING
    u8  abyLPTInStatus[TNM_MAX_LPTCHAN_NUM];        //���������ϱ�(һ��u8����һ��ͨ��)��ֻ����enum TnmStatusType
    u8  abyHardDiskUseRate[TNM_MAX_PU_DISK_NUM];    //�д��̵�ǰ�˸������̵�ʹ����(ֵС��100��Ч,ֵΪINVALID_DISK˵���ô��̲�����)

    u8  abyLPTOutStatus[TNM_MAX_LPTCHAN_NUM];       //��������ϱ�(һ��u8����һ��ͨ��)��ֻ����enum TnmStatusType
    u32 abyCodecRate[TNM_MAX_CHAN_NUM];             //ͨ����������(��λbps)
    u8  abyVideoSrcStatus[TNM_MAX_VIDEOSRC_NUM];    //��ƵԴ״̬(һ��u8����һ����ƵԴ)enum TnmStatusType
    u8  abyVideoRecordState[TNM_MAX_CHAN_NUM];      //ÿ��ͨ���Ƿ���¼��(һ��u8����һ��ͨ��) enum TnmRpState
    u8  byVideoPlayState;                           //�豸�Ƿ��ڷ��� enum TnmRpState
    u8  byStreamNum;                                //��ǰ��Ƶ��·��
    u8  byLostPacketRate;                           //ҵ��㶪����
    u8  byReserved;

    u8  abyEthLostPkRate[TNM_MAX_ETHERNET_NUM + 3]; //��̫���ڶ����ʣ�0--100��
    u32 abyEthBps[TNM_MAX_ETHERNET_NUM + 3];        //��̫������������λbps��

    u8  byLayer;                                    //�豸�Ĳ��
    u8  bySlot;                                     //�豸�Ĳۺ�

    u8  abyReserved[30];
    
    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        TTnmPfmHwInfo::Init();
        
        s32 nIndex = 0;
        for(nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            abyRcvLosePacketRate[nIndex] = 0;
            abyCodecState[nIndex] = TNM_INVALID_CHANNEL_ID;
            abyCodecRate[nIndex] = 0;
        }
        for(nIndex = 0; nIndex < TNM_MAX_LPTCHAN_NUM; nIndex++)
        {
            abyLPTInStatus[nIndex] = TNM_STATUS_NONE;
            abyLPTOutStatus[nIndex] = TNM_STATUS_NONE;
        }
        for (nIndex = 0; nIndex < TNM_MAX_PU_DISK_NUM; nIndex++)
        {
            abyHardDiskUseRate[nIndex] = INVALID_DISK;
        }

        for (nIndex = 0; nIndex < TNM_MAX_VIDEOSRC_NUM; nIndex++)
        {
            abyVideoSrcStatus[nIndex] = TNM_STATUS_NONE;
        }
        
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            abyVideoRecordState[nIndex] = RP_STATE_NULL;
        }

        for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
        {
            abyEthBps[nIndex] = 0;
        }

        byVideoPlayState = RP_STATE_STOP;
        byStreamNum = 0;
        byLostPacketRate = 0;
        
        byLayer = 0;                                    //�豸�Ĳ��
    	bySlot = 0;                                     //�豸�Ĳۺ�
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPfmHwInfo::Print(bNetToHost);

        s32 nIndex;
        OspPrintf(TRUE, FALSE, "\tLosRate: ");
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyRcvLosePacketRate[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tCodeState: ");
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyCodecState[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tLPTStatus: ");
        for (nIndex = 0; nIndex < TNM_MAX_LPTCHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyLPTInStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "DiskUseRate:");
        for (nIndex = 0; nIndex < TNM_MAX_PU_DISK_NUM; nIndex++)
        {
            if (abyHardDiskUseRate[nIndex] == INVALID_DISK)
            {
                break;
            }
            OspPrintf(TRUE, FALSE, "%d-", abyHardDiskUseRate[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPuPfmInfo::NetToHost()
{
    s32 nIndex = 0;

    TTnmPfmHwInfo::NetToHost();

    for(nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
    {
        abyCodecRate[nIndex] = ntohl(abyCodecRate[nIndex]);
    }
    
    for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    {
        abyEthBps[nIndex] = ntohl(abyEthBps[nIndex]);
    }
}
inline void TTnmPuPfmInfo::HostToNet()
{
    s32 nIndex = 0;

    TTnmPfmHwInfo::HostToNet();

    for(nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
    {
        abyCodecRate[nIndex] = htonl(abyCodecRate[nIndex]);
    }
    
    for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    {
        abyEthBps[nIndex] = htonl(abyEthBps[nIndex]);
    }
}

/*====================================================================
2.x. Dec5���ܲ���
====================================================================*/
//�������ܲ���
typedef struct tagTnmDec5PfmInfo : public TTnmBoardPfmInfo
{
    u8  abyRcvLosePacketRate[TNM_MAX_CHAN_NUM]; // ���ն����ʣ�ָ���1����֮�ڵ�ƽ�������ʣ���ѯ
                                                //  һ��u8����һ��ͨ����100%
                                                //  TNM_INVALID_CHANNEL_ID  ��Чֵ
    u8  abyCodecState[TNM_MAX_CHAN_NUM];        // ����������շ�״̬��ѯ(һ��u8����һ��ͨ��)
                                                //  TNM_INVALID_CHANNEL_ID  ��Чֵ
                                                //  TNM_CODEC_STOP
                                                //  TNM_ENCODING
                                                //  TNM_DECODING
    u8  abyLPTStatus[TNM_MAX_LPTCHAN_NUM];      //���ڸ澯�ϱ�(һ��u8����һ��ͨ��)��ֻ����enum TnmStatusType
    
    void Init()
    {
        TTnmBoardPfmInfo::Init();
        memset(abyRcvLosePacketRate, 0, sizeof(abyRcvLosePacketRate));
        memset(abyCodecState, 0, sizeof(abyCodecState));
        memset(abyLPTStatus, 0, sizeof(abyLPTStatus));
    }

    void Print(BOOL32 bNetToHost = FALSE )
    {
        TTnmBoardPfmInfo::Print(bNetToHost);
        
        s32 nIndex;
        OspPrintf(TRUE, FALSE, "\tLosRate: ");
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyRcvLosePacketRate[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tCodeState: ");
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyCodecState[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tLPTStatus: ");
        for (nIndex = 0; nIndex < TNM_MAX_LPTCHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyLPTStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
    }

    void NetToHost()
    {
        TTnmBoardPfmInfo::NetToHost();
    }
    void HostToNet()
    {
        TTnmBoardPfmInfo::HostToNet();
    }

}PACKED TTnmDec5PfmInfo;

//////////////////////////////////////////////////////////////////////////
//  ƽ̨�豸������չ����
typedef struct tagTnmCmuExtendablePfmInfo : public TTnmBoardPfmInfo
{
    u32 dwRecTaskNum;           // ¼������·��
    u32 dwRecBitRate;           //¼��������
    u32 dwRecVodNum;            //¼��㲥·��
    u32 dwRecVodTotalBitRate;   // ¼��㲥������       
    u32 dwSwitchNum;            // ת������·��
    u32 dwTotalBitRate;         // ת��������
    u32 dwPreRecChanNum;        //Ԥ¼·��

    void NetToHost();
    void HostToNet();
}PACKED TTnmCmuExtendablePfmInfo;

inline void TTnmCmuExtendablePfmInfo::NetToHost()
{
    dwRecTaskNum = ntohl(dwRecTaskNum);           
    dwRecBitRate = ntohl(dwRecBitRate);           
    dwRecVodNum = ntohl(dwRecVodNum);            
    dwRecVodTotalBitRate = ntohl(dwRecVodTotalBitRate);       
    dwSwitchNum= ntohl(dwSwitchNum);            
    dwTotalBitRate = ntohl(dwTotalBitRate);         
    dwPreRecChanNum = ntohl(dwPreRecChanNum);        
}

inline void TTnmCmuExtendablePfmInfo::HostToNet()
{
    dwRecTaskNum = htonl(dwRecTaskNum);           
    dwRecBitRate = htonl(dwRecBitRate);           
    dwRecVodNum = htonl(dwRecVodNum);            
    dwRecVodTotalBitRate = htonl(dwRecVodTotalBitRate);       
    dwSwitchNum= htonl(dwSwitchNum);            
    dwTotalBitRate = htonl(dwTotalBitRate);         
    dwPreRecChanNum = htonl(dwPreRecChanNum);
}

//��ȡcmu�Ĵ���¼�����
typedef struct tagTnmCmuDiskPfmInfo : public TTnmBoardPfmInfo
{
    //���ط�����
    u8 achMountPath[MAX_MOUNTPATH_LEN];
    
    //¼�����
    u32 dwRecNum;
    
    //�����
    u32 dwAreaID;

    void NetToHost();
    void HostToNet();

}PACKED TTnmCmuDiskPfmInfo;

inline void TTnmCmuDiskPfmInfo::NetToHost()
{
   dwRecNum = ntohl(dwRecNum);
   dwAreaID = ntohl(dwAreaID);
}

inline void TTnmCmuDiskPfmInfo::HostToNet()
{
    dwRecNum = htonl(dwRecNum);
    dwAreaID = htonl(dwAreaID);
}

// 2.4. ƽ̨�豸���ܲ���
// 2.4.1 CMU�������ܲ���
typedef struct tagTnmCmuPfmInfo : public TTnmBoardPfmInfo
{
    u16 wPuDevNum;          // ǰ���豸������
    u16 wCuNum;             // CU������
    u16 wVtduNum;           // VTDU������
    u16 wNruNum;            // NRU������
    u16 wBoardNum;          // ��������
    u16 wReserved;
    u8  abyFanStatus[TNM_MAX_FAN_NUM + 3];      // ����״̬(һ��u8����һ������)enum TnmFanStatus
    u8  abyPowerStatus[TNM_MAX_POWER_NUM + 3];  // ��Դ״̬(һ��u8����һ����Դ)enum TnmPowerStatus

    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        TTnmBoardPfmInfo::Init();
        wPuDevNum = 0;
        wCuNum = 0;
        wVtduNum = 0;
        wNruNum = 0;
        wBoardNum = 0;
        s32 nIndex = 0;
        for(nIndex = 0; nIndex < TNM_MAX_FAN_NUM; nIndex++)
        {
            abyFanStatus[nIndex] = TNM_FAN_NONE;
        }
        for(nIndex = 0; nIndex < TNM_MAX_POWER_NUM; nIndex++)
        {
            abyPowerStatus[nIndex] = TNM_POWER_NONE; 
        }
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBoardPfmInfo::Print(bNetToHost);

        s32 nIndex;
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tPuNum: 0x%hx; CuNum: 0x%hx; NruNum: 0x%hx; \n",
                ntohs(wPuDevNum), ntohs(wCuNum), ntohs(wNruNum));
            OspPrintf(TRUE, FALSE, "\tVtduNum: 0x%hx; BoardNum: 0x%hx;\n",
                ntohs(wVtduNum), ntohs(wBoardNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tPuNum: 0x%hx; CuNum: 0x%hx; NruNum: 0x%hx; \n", wPuDevNum, wCuNum, wNruNum);
            OspPrintf(TRUE, FALSE, "\tVtduNum: 0x%hx; BoardNum: 0x%hx;\n", wVtduNum, wBoardNum);
        }
        OspPrintf(TRUE, FALSE, "\tFan: ");
        for (nIndex = 0; nIndex < TNM_MAX_FAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyFanStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "; Power: ");
        for (nIndex = 0; nIndex < TNM_MAX_POWER_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyPowerStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, ";\n");
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmCmuPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmCmuPfmInfo::NetToHost()
{
    TTnmBoardPfmInfo::NetToHost();

    wPuDevNum = ntohs(wPuDevNum);
    wCuNum = ntohs(wCuNum);
    wVtduNum = ntohs(wVtduNum);
    wNruNum = ntohs(wNruNum);
    wBoardNum = ntohs(wBoardNum);
}
inline void TTnmCmuPfmInfo::HostToNet()
{
    TTnmBoardPfmInfo::HostToNet();

    wPuDevNum = htons(wPuDevNum);
    wCuNum = htons(wCuNum);
    wVtduNum = htons(wVtduNum);
    wNruNum = htons(wNruNum);
    wBoardNum = htons(wBoardNum);
} 

//2.4.2 CMU��չ���ܲ���-CU��¼���

typedef struct tagTnmCuLogon 
{
    tagTnmCuLogon()
    {
        dwSessionId = 0;
    }
    u32 dwSessionId;        // �ͻ����û�Session
    TTnmLogonInfo   tCuLogonInfo;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwSSId = dwSessionId;
        if (bNetToHost) 
        {
            dwSSId = ntohl(dwSSId);
        }
        OspPrintf(TRUE, FALSE, "\tSession=0x%lx\n", dwSSId);
        
        tCuLogonInfo.Print(bNetToHost);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuLogon;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmCuLogon::NetToHost()
{
    tCuLogonInfo.NetToHost();
    dwSessionId = ntohl(dwSessionId);
}
inline void TTnmCuLogon::HostToNet()
{
    tCuLogonInfo.HostToNet();
    dwSessionId = htonl(dwSessionId);
} 

typedef struct tagTnmCuLogItem
{
    tagTnmCuLogItem()
    {
        dwSessionId = 0;
        memset(achRecentLog, 0, sizeof(achRecentLog));
    }
    u32 dwSessionId;        // �ͻ����û�Session
    s8  achRecentLog[TNM_MAX_CU_LOG_NUM][TNM_MAX_CU_LOG_LEN + 1];   // CU���������־��¼��

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "Session=0x%x\n", ntohl(dwSessionId));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "Session=0x%lx\n", dwSessionId);
        }
        s32 nIndex;
        for(nIndex = 0; nIndex < TNM_MAX_CU_LOG_NUM; nIndex++)
        {
            s8 *pchLog = achRecentLog[nIndex];
            if (strlen(pchLog) > 0) 
            {
                OspPrintf(TRUE, FALSE, "%s\n", pchLog);
            }
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuLogItem;

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmCuLogItem::NetToHost()
{
    dwSessionId = ntohl(dwSessionId);
}
inline void TTnmCuLogItem::HostToNet()
{
    dwSessionId = htonl(dwSessionId);
} 
//typedef struct tagTnmCmuCuInfo
//{
//  tagTnmCmuCuInfo()
//  {
//      dwSessionId = 0;
//  }
//    
//  u64 dwSessionId;        // �û�Session
//    TTnmLogonInfo tCuLogonInfo;
//    TTnmCuLogItem tCuLog; // CU���������־��¼��
//}PACKED TTnmCmuCuInfo;

//2.4.3 CMU��չ���ܲ���-PU�������
typedef struct tagTnmCmuPuInfo
{
    tagTnmCmuPuInfo()
    {
        dwDevIp = 0;
        byState = TNM_PU_STATE_ONLINE;
        tDevId.SetNull();
    }
    TTnmDeviceId tDevId;    // ǰ���豸ID
    u32 dwDevIp;            // ǰ���豸IP
    s8  byState;            // �豸״̬
                            //  TNM_PU_STATE_ONLINE
                            //  TNM_PU_STATE_MUTE
                            //  TNM_PU_STATE_SEND_AUDIO
                            //  TNM_PU_STATE_PLAT_REC
                            //  TNM_PU_STATE_PU_REC
                            //  TNM_PU_STATE_ALARM
    u8  byReserved1;        // 2006/04/05 ���ǿ ���ֽڶ���
    u8  byReserved2;
    u8  byReserved3;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; DevIp: %s; State: %d;\n", 
                tDevId.achID, ::GetIpStr(ntohl(dwDevIp)), byState);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; DevIp: %s; State: %d;\n", 
                tDevId.achID, ::GetIpStr(dwDevIp), byState);
        }
        
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmCmuPuInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmCmuPuInfo::NetToHost()
{
    dwDevIp = ntohl(dwDevIp);
}
inline void TTnmCmuPuInfo::HostToNet()
{
    dwDevIp = htonl(dwDevIp);
}


typedef struct tagTnmLaDenPfmInfo : public TTnmBasePfmInfo
{
    u16 wPuiNum;        //PUI��
    u16 wCuiNum;        //CUI��

    void Init()
    {
        TTnmBasePfmInfo::Init();
        wPuiNum = 0;
        wCuiNum = 0;
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBasePfmInfo::Print(bNetToHost);

        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tPuiNum: 0x%hx; wCuiNum: %d;\n", ntohs(wPuiNum), ntohs(wCuiNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tPuiNum: 0x%hx; wCuiNum: %d;\n", wPuiNum, wCuiNum);
        }
        OspPrintf(TRUE, FALSE, ";\n");
    }
    void NetToHost();
    void HostToNet();
}TTnmLaDenPfmInfo;

// 2.4.4 ��ȡƽ̨��ģ������״̬
//  ƽ̨�豸ģ������״̬����
typedef struct tagTnmCmuModuleStateInfo
{
	s8 achModuleIp[TNM_MAX_DEV_NETADDR_LEN + 1]; //ģ��ip��ַ
	s8 achModuleId[TNM_MAX_DEVICE_ID_LEN + 1];   //ģ��id
	u8 byModuleState;			                 //ģ������״̬ 0:��ʾδ���� 1:��ʾ��������
    u8 byModuleType;                             //ģ������
}PACKED TTnmCmuModuleStateInfo;


inline void TTnmLaDenPfmInfo::NetToHost()
{
    TTnmBasePfmInfo::NetToHost();

    wPuiNum = ntohs(wPuiNum);
    wCuiNum = ntohs(wCuiNum);
}
inline void TTnmLaDenPfmInfo::HostToNet()
{
    TTnmBasePfmInfo::HostToNet();

    wPuiNum = htons(wPuiNum);
    wCuiNum = htons(wCuiNum);
}

//Laden��չ���ܲ���-PUI�б�
typedef struct tagTnmLadenPuiInfo
{
    tagTnmLadenPuiInfo()
    {
        tDevId.SetNull();
        dwPuNum = 0;
    }
    TTnmDeviceId tDevId;    //Pui�豸ID
    u32 dwPuNum;            //ǰ���豸����

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; dwPuNum: %u;\n", tDevId.achID, ntohl(dwPuNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; dwPuNum: %lu;\n", tDevId.achID, dwPuNum);
        }
    }

    void NetToHost();
    void HostToNet();

}TTnmLadenPuiInfo;

inline void TTnmLadenPuiInfo::NetToHost()
{
    dwPuNum = ntohl(dwPuNum);
}
inline void TTnmLadenPuiInfo::HostToNet()
{
    dwPuNum = htonl(dwPuNum);
}

//Laden��չ���ܲ���-CUI�б�
typedef struct tagTnmLadenCuiInfo
{
    tagTnmLadenCuiInfo()
    {
        tDevId.SetNull();
        dwCuNum = 0;
    }
    TTnmDeviceId tDevId;    //Pui�豸ID
    u32 dwCuNum;            //CU����

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; dwCuNum: %u;\n", tDevId.achID, ntohl(dwCuNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; dwCuNum: %lu;\n", tDevId.achID, dwCuNum);
        }
    }

    void NetToHost();
    void HostToNet();

}TTnmLadenCuiInfo;

inline void TTnmLadenCuiInfo::NetToHost()
{
    dwCuNum = ntohl(dwCuNum);
}

inline void TTnmLadenCuiInfo::HostToNet()
{
    dwCuNum = htonl(dwCuNum);
}

// 2.5 VPU
typedef struct tagTnmVpuEncState
{
    u8  byEncStatus;        //�Ƿ��ڱ���(TNM_DISABLE TNM_ENABLE:�ڱ���)
    u8  bySendFlag;         //�Ƿ��ڷ���(TNM_DISABLE TNM_ENABLE:�ڷ���)
    u8  byVideoResolution;  //ͼ��ֱ���
    u8  byVideoType;        //�����ʽ TNM_VIDEO_TYPE_H263
    u8  byFrameRate;        //֡�� 1-25
    u8  byVpuStyle;         //�໭����
    u16 wReserved;          //����2
    u32 dwBitRate;          //�������� 64K-4M

    
    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        byEncStatus = TNM_DISABLE;
        bySendFlag  = TNM_DISABLE;
        byVideoResolution   = TNM_VIDEO_RESOLUTION_NONE;
        byVideoType = TNM_VIDEO_TYPE_H263;
        byFrameRate = 25;
        byVpuStyle  = TNM_VPU_STYLE_INVALID;
        dwBitRate   = 64;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "EncStatus: %d; SFlag: %d; VResol: %d; BitRate: 0x%lx;\n", 
                byEncStatus, bySendFlag, byVideoResolution, dwBitRate);
        OspPrintf(TRUE, FALSE, "\tVType: %d; FramRate: %d; VpuStyle: %d;\n",
                byVideoType, byFrameRate, byVpuStyle);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuEncState;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVpuEncState::NetToHost()
{
    dwBitRate = ntohl(dwBitRate);
}
inline void TTnmVpuEncState::HostToNet()
{
    dwBitRate = htonl(dwBitRate);
}
typedef struct tagTnmVpuDecState
{
    u8  byDecStatus;        //�Ƿ��ڽ���(TNM_DISABLE TNM_ENABLE:�ڽ���)
    u8  byRcvFlag;          //�Ƿ��ڽ���(TNM_DISABLE TNM_ENABLE:�ڽ���)
    u8  byLosePkgRate;      //������
    u8  byReserved1;        //����1
    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        byDecStatus = TNM_DISABLE;
        byRcvFlag   = TNM_DISABLE;
        byLosePkgRate   = 0;
        byReserved1 = 0;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "DecStatus: %d; RFlag: %d; LoseRate: %d;\n",
                byDecStatus, byRcvFlag, byLosePkgRate);
    }
}PACKED TTnmVpuDecState;

typedef struct tagTnmVpuRcvChnStatus
{
    tagTnmVpuRcvChnStatus()
    {
        dwChnIp = 0;
        wChnPort = 0;
        dwRecvBitRate = 0;
        dwRecvLosePackNum = 0;
        dwRecvPackNum = 0;
    }
    u32     dwChnIp;                //������ͨ����Ip
    u16     wChnPort;               //������ͨ����Port
    u32     dwRecvBitRate;          //��������
    u32     dwRecvPackNum;          //�յ��İ���
    u32     dwRecvLosePackNum;      //�������ȱ�ٵİ���
    u16     wReserved;
    void Print(BOOL32 bNetToHost = FALSE);
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuRcvChnStatus;

inline void TTnmVpuRcvChnStatus::Print(BOOL32 bNetToHost)
{
    if (bNetToHost)
    {
        OspPrintf(TRUE, FALSE, "\tChnIp:%s; ChnPort:%d; RcvRate: %d;\n", 
            ::GetIpStr(ntohl(dwChnIp)), ntohs(wChnPort), ntohl(dwRecvBitRate));
        OspPrintf(TRUE, FALSE, "\tRcvPackNum: %d; LosePackNum: %d;\n", 
            ntohl(dwRecvPackNum), ntohl(dwRecvLosePackNum));
    }
    else
    {        
        OspPrintf(TRUE, FALSE, "\tChnIp:%s; ChnPort:%hu; RcvRate: %lu;\n", 
            ::GetIpStr(dwChnIp), wChnPort, dwRecvBitRate);
        OspPrintf(TRUE, FALSE, "\tRcvPackNum: %lu; LosePackNum: %lu;\n", 
            dwRecvPackNum, dwRecvLosePackNum);
    }
}

inline void TTnmVpuRcvChnStatus::NetToHost()
{
    dwChnIp = ntohl(dwChnIp);
    wChnPort = ntohs(wChnPort);
    dwRecvBitRate = ntohl(dwRecvBitRate);
    dwRecvPackNum = ntohl(dwRecvPackNum);
    dwRecvLosePackNum = ntohl(dwRecvLosePackNum);
}

inline void TTnmVpuRcvChnStatus::HostToNet()
{
    dwChnIp = htonl(dwChnIp);
    wChnPort = htons(wChnPort);
    dwRecvBitRate = htonl(dwRecvBitRate);
    dwRecvPackNum = htonl(dwRecvPackNum);
    dwRecvLosePackNum = htonl(dwRecvLosePackNum);
}


typedef struct tagTnmVpuPfmInfo : public TTnmBoardPfmInfo
{   
    u8          byMergeStart;           //�Ƿ�ʼ����ϳ� 0 ��/1 ��
    u8          byType;                 //����ϳ�����    
    u8          byCurChnNum;            //��ǰ����ϳɵ�ͨ����
    u32         dwTotalMapNum;          //Mapʹ�ñ�־
    u32         dwNetSndIp;             //���緢�͵�ַIp
    u16         wNetSndPort;            //���緢�͵�ַport
    u32         dwSendBitRate;          //��������
    u32         dwSendPackNum;          //���͵İ���
    u32         dwSendDiscardPackNum;   //���Ͷ�����
    u8          byReserved;
    u16         wReserved2;
    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�  2006.09.14 renhouping �޸�
    void Init()
    {
        byMergeStart = TNM_DISABLE;
        byType = 0;
        byCurChnNum = 0;
        dwTotalMapNum = 0;
        dwNetSndIp = 0;
        wNetSndPort = 0;
        dwSendBitRate = 0;
        dwSendPackNum = 0;
        dwSendDiscardPackNum = 0;        
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            OspPrintf(TRUE, FALSE, "\tMapNum:%d; SndIp:0x%x; SndPort:%d;\n", 
                    ntohl(dwTotalMapNum), ntohl(dwNetSndIp), ntohs(wNetSndPort));
            OspPrintf(TRUE, FALSE, "\tSndRate:%d; SndPackNum:%d; SndLostNum:%d;\n", 
                    ntohl(dwSendBitRate), ntohl(dwSendPackNum), ntohl(dwSendDiscardPackNum));            
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tMapNum:%lu; SndIp:0x%lx; SndPort:%hu;\n", 
                    dwTotalMapNum, dwNetSndIp, wNetSndPort);
            OspPrintf(TRUE, FALSE, "\tSndRate:%lu; SndPackNum:%lu; SndLostNum:%lu;\n", 
                    dwSendBitRate, dwSendPackNum, dwSendDiscardPackNum);
        }
        OspPrintf(TRUE, FALSE, "\tMegStart: %d; Type: %d; ChnNum: %d; \n",
                byMergeStart, byType, byCurChnNum);

    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVpuPfmInfo::NetToHost()
{
    TTnmBoardPfmInfo::NetToHost();
    dwTotalMapNum = ntohl(dwTotalMapNum);
    dwNetSndIp = ntohl(dwNetSndIp);
    wNetSndPort = ntohs(wNetSndPort);
    dwSendBitRate = ntohl(dwSendBitRate);
    dwSendPackNum = ntohl(dwSendPackNum);
    dwSendDiscardPackNum = ntohl(dwSendDiscardPackNum);
}
inline void TTnmVpuPfmInfo::HostToNet()
{
    TTnmBoardPfmInfo::NetToHost();
    dwTotalMapNum = htonl(dwTotalMapNum);
    dwNetSndIp = htonl(dwNetSndIp);
    wNetSndPort = htons(wNetSndPort);
    dwSendBitRate = htonl(dwSendBitRate);
    dwSendPackNum = htonl(dwSendPackNum);
    dwSendDiscardPackNum = htonl(dwSendDiscardPackNum);
}

/*====================================================================
2.6. NRU���ܲ���
====================================================================*/
#define MAX_REC_DIR_LEN             128     // ¼�����Ŀ¼����
#define MAX_REC_FILE_NAME_LEN       128      // ¼���ļ�����󳤶�
#define MAX_NRU_CUID_LEN            24      // �ͻ���ID��󳤶�
#define MAX_NRU_MAX_ADDR_NUM        10      // NRU���IP��ַ��Ŀ
#define MAX_STREAM_PORT_NUM         4       // ý�������˿���Ŀ

// ¼�����������Ϣ
typedef struct tagTnmRecTask
{
    tagTnmRecTask()
    {
        memset(achFileDir, 0, sizeof(achFileDir));
        memset(achFileName, 0, sizeof(achFileName));
        dwStartTime = 0;
        dwEndTime = 0;
    }
    s8  achFileDir[MAX_REC_DIR_LEN + 4];            // ¼��Ŀ¼
    s8  achFileName[MAX_REC_FILE_NAME_LEN + 4];     // ¼���ļ���
    u32 dwStartTime;                            // ¼��ʼʱ��
    u32 dwEndTime;                              // ¼�����ʱ��
    TTnmUserId tUserId;                         // �ͻ����û�ID
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwStart = dwStartTime;
        u32 dwEnd = dwEndTime;
        if (bNetToHost)
        {
            dwStart = ntohl(dwStart);
            dwEnd = ntohl(dwEnd);
        }
        
        OspPrintf(TRUE, FALSE, "\tFDir=%s\n\tFName=%s\n", achFileDir, achFileName);
        OspPrintf(TRUE, FALSE, "\tSTime=%s\n", ::GetTimeStr(dwStart));
        OspPrintf(TRUE, FALSE, "\tETime=%s\n", ::GetTimeStr(dwEnd));
        OspPrintf(TRUE, FALSE, "\tUsrId=%s\n", tUserId.achID);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmRecTask;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmRecTask::NetToHost()
{
    dwStartTime = ntohl(dwStartTime);
    dwEndTime = ntohl(dwEndTime);
}
inline void TTnmRecTask::HostToNet()
{
    dwStartTime = htonl(dwStartTime);
    dwEndTime = htonl(dwEndTime);
}
// ¼��ý�������Ϣ
typedef struct tagTnmNruMediaInfo
{
    tagTnmNruMediaInfo()
    {
        memset(this, 0, sizeof(tagTnmNruMediaInfo));
    }
    u32 dwListenIpAddr;                         // ������ַ
    u16 awLocalListenPort[MAX_STREAM_PORT_NUM]; // �����˿�
    u8  byStreamPortNum;                        // �����˿�����
    u8  byReserve1;     // 2006/04/05 ���ǿ ���ֽڶ���
    u8  byReserve2;
    u8  byReserve3;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "PortNum: %d; LsnIp: %s; \n",
                byStreamPortNum, ::GetIpStr(ntohl(dwListenIpAddr)));
            OspPrintf(TRUE, FALSE, "\tLsnPort1: 0x%hx; LsnPort2: 0x%hx; LsnPort3: 0x%hx;\n",
                ntohs(awLocalListenPort[0]), ntohs(awLocalListenPort[1]), ntohs(awLocalListenPort[2]));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "PortNum: %d; LsnIp: %s; \n",
                byStreamPortNum, ::GetIpStr(dwListenIpAddr));
            OspPrintf(TRUE, FALSE, "\tLsnPort1: 0x%hx; LsnPort2: 0x%hx; LsnPort3: 0x%hx;\n",
                awLocalListenPort[0], awLocalListenPort[1], awLocalListenPort[2]);
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruMediaInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmNruMediaInfo::NetToHost()
{
    s32 nIndex;
    dwListenIpAddr = ntohl(dwListenIpAddr);
    for(nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
    {
        awLocalListenPort[nIndex] = ntohs(awLocalListenPort[nIndex]);
    }
}
inline void TTnmNruMediaInfo::HostToNet()
{
    s32 nIndex;
    dwListenIpAddr = htonl(dwListenIpAddr);
    for(nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
    {
        awLocalListenPort[nIndex] = htons(awLocalListenPort[nIndex]);
    }
}

// 2.6.1 ¼���������(��չ����)
typedef struct tagTnmNruRecTask : public TTnmRecTask
{
    tagTnmNruRecTask()
    {
        tDevId.SetNull();
        dwDevChannelId = 0;
        byHasAudio = TNM_AUDIO_VIDEO;
        tDevId.SetNull();
        dwCurRecSpace = 0;
    }
    TTnmDeviceId    tDevId;                 // �豸 id
    u32             dwDevChannelId;         // �豸ͨ��id
    u32             dwCurRecSpace;          // ��ǰ¼���������¼�ռ�(M)
    TTnmNruMediaInfo tMediaInfo;            // ý����Ϣ
    u8              byHasAudio;             // �Ƿ������Ƶ TNM_ONLY_VIDEO/TNM_AUDIO_VIDEO

    u8  byReserve1;     // 2006/04/05 ���ǿ ���ֽڶ���
    u8  byReserve2;
    u8  byReserve3;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmRecTask::Print(bNetToHost);
        
        u32 dwChnnlId = dwDevChannelId;
        u32 dwSpace = dwCurRecSpace;
        if (bNetToHost) 
        {
            dwChnnlId = ntohl(dwDevChannelId);
            dwSpace = ntohl(dwSpace);
        }
        OspPrintf(TRUE, FALSE, "\tDevId=%s\n\tChId=0x%lx, Space=0x%lx, Audio=%d\n",
            tDevId.achID, dwChnnlId, dwSpace, byHasAudio);
        OspPrintf(TRUE, FALSE, "\t");
        tMediaInfo.Print(bNetToHost);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruRecTask;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmNruRecTask::NetToHost()
{
    TTnmRecTask::NetToHost();

    dwDevChannelId = ntohl(dwDevChannelId);
    dwCurRecSpace = ntohl(dwCurRecSpace);
    tMediaInfo.NetToHost();
}
inline void TTnmNruRecTask::HostToNet()
{
    TTnmRecTask::HostToNet();

    dwDevChannelId = htonl(dwDevChannelId);
    dwCurRecSpace = htonl(dwCurRecSpace);
    tMediaInfo.HostToNet();
}
// 2.6.2 ��������(��չ����)
// ¼�������
typedef struct tagTnmRecProg
{
    tagTnmRecProg()
    {
        dwCurProg = 0;
        dwTotalTime = 0;
    }
    u32 dwCurProg;                          // ��ǰ���ȣ���λ���룩;
    u32 dwTotalTime;                        // �ܳ��ȣ����ڷ���ʱ��Ч����λ���룩;    
    
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwCur = dwCurProg;
        u32 dwTotal = dwTotalTime;
        if (bNetToHost) 
        {
            dwCur = ntohl(dwCurProg);
            dwTotal =  ntohl(dwTotalTime);
        }
        OspPrintf(TRUE, FALSE, "CurProg=0x%lx, TotalTime=0x%lx\n",dwCur, dwTotal);
        
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmRecProg;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmRecProg::NetToHost()
{
    dwCurProg = ntohl(dwCurProg);
    dwTotalTime = ntohl(dwTotalTime);
}
inline void TTnmRecProg::HostToNet()
{
    dwCurProg = htonl(dwCurProg);
    dwTotalTime = htonl(dwTotalTime);
}
typedef struct tagTnmNruPlayTask : public TTnmRecTask
{
    tagTnmNruPlayTask()
    {
        s32 nIndex;
        dwLocAddr   = 0;
        dwRemoteAddr    = 0;
        for(nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
        {
            awLocalPort[nIndex] = 0;
            awRemotePort[nIndex] = 0;
        }
        byPlayState = RP_STATE_NULL;
        byPlayRate  = ONE_TIMES;
        dwPlayChannel   = 0;
    }
    u32 dwLocAddr;                          // ���ص�ַ
    u32 dwRemoteAddr;                       // �Զ˵�ַ
    u16 awLocalPort[MAX_STREAM_PORT_NUM + 2];   // ���ض˿��б�
    u16 awRemotePort[MAX_STREAM_PORT_NUM + 2];  // �Զ˶˿��б�
    TTnmRecProg m_tRecProg;                 // �������;
    u32 dwPlayChannel;                      // ����ͨ��;
    u8  byPlayState;                        // ����״̬ enum TnmRpState 
    u8  byPlayRate;                         // ��������;enum TnmPlayRate
    u8  byReserve1;     // 2006/04/05 ���ǿ ���ֽڶ���
    u8  byReserve2;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmRecTask::Print(bNetToHost);
        u32 dwLocalIp = dwLocAddr;
        u32 dwRIp = dwRemoteAddr;
        u32 dwChnnl = dwPlayChannel;
        if (bNetToHost) 
        {
            dwLocalIp = ntohl(dwLocAddr);
            dwRIp = ntohl(dwRemoteAddr);
            dwChnnl = ntohl(dwPlayChannel);
        }
        OspPrintf(TRUE, FALSE, "\tLIp=%s, ", ::GetIpStr(dwLocalIp));
        OspPrintf(TRUE, FALSE, "RIp=%s\n", ::GetIpStr(dwRIp));
        OspPrintf(TRUE, FALSE, "\tPlayState=%d, PlayRate=%d, PlayChannel=0x%lx;\n",
            byPlayState, byPlayRate, dwChnnl);
        OspPrintf(TRUE, FALSE, "\t");
        m_tRecProg.Print(bNetToHost);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruPlayTask;

/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmNruPlayTask::NetToHost()
{
    TTnmRecTask::NetToHost();

    dwLocAddr = ntohl(dwLocAddr);
    dwRemoteAddr = ntohl(dwRemoteAddr);
    for(s32 nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
    {
        awLocalPort[nIndex] = ntohs(awLocalPort[nIndex]);
        awRemotePort[nIndex] = ntohs(awRemotePort[nIndex]);
    }
    m_tRecProg.NetToHost();
    dwPlayChannel = ntohl(dwPlayChannel);
}
inline void TTnmNruPlayTask::HostToNet()
{
    TTnmRecTask::HostToNet();

    dwLocAddr = htonl(dwLocAddr);
    dwRemoteAddr = htonl(dwRemoteAddr);
    for(s32 nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
    {
        awLocalPort[nIndex] = htons(awLocalPort[nIndex]);
        awRemotePort[nIndex] = htons(awRemotePort[nIndex]);
    }
    m_tRecProg.HostToNet();
    dwPlayChannel = htonl(dwPlayChannel);
}

// 2.6.3 ��������(��չ����)
typedef struct tagTnmNruDownLoadTask : public TTnmRecTask
{
    tagTnmNruDownLoadTask()
    {
        dwSendIP = 0;
        wSendPort = 0;
        dwRcvIP = 0;
        wRcvPort = 0;
    }

    u32 dwSendIP;                               // ���Ͷ�IP
    u32 dwRcvIP;                                // ���ն�IP
    u16 wSendPort;                              // ���Ͷ˶˿�
    u16 wRcvPort;                               // ���ն˶˿�

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmRecTask::Print(bNetToHost);
        
        u32 dwSend = dwSendIP;
        u32 dwRcv = dwRcvIP;
        u16 wSPort = wSendPort;
        u16 wRPort = wRcvPort;
        if (bNetToHost) 
        {
            dwSend =ntohl(dwSendIP);
            dwRcv = ntohl(dwRcvIP);
            wSPort = ntohs(wSendPort);
            wRPort = ntohs(wRcvPort);
        }
        OspPrintf(TRUE, FALSE, "\tSIp=%s, ", ::GetIpStr(dwSend));
        OspPrintf(TRUE, FALSE, "RIp=%s, SPort=0x%hx, RPort=0x%hx\n",
            ::GetIpStr(dwRcv), wSPort, wRPort);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruDownLoadTask;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmNruDownLoadTask::NetToHost()
{
    TTnmRecTask::NetToHost();

    dwSendIP = ntohl(dwSendIP);
    dwRcvIP = ntohl(dwRcvIP);
    wSendPort = ntohs(wSendPort);
    wRcvPort = ntohs(wRcvPort);
}
inline void TTnmNruDownLoadTask::HostToNet()
{
    TTnmRecTask::HostToNet();

    dwSendIP = htonl(dwSendIP);
    dwRcvIP = htonl(dwRcvIP);
    wSendPort = htons(wSendPort);
    wRcvPort = htons(wRcvPort);
}
// 2.6.4 Ԥ¼����(��չ����)
typedef struct tagTnmNruPreRecTask 
{
    tagTnmNruPreRecTask()
    {
        tDevId.SetNull();
        dwDevChannelId  = 0;
        dwPreRecTime = 0;
        byHasAudio = TNM_AUDIO_VIDEO;
        tDevId.SetNull();
    }

    TTnmDeviceId tDevId;                        // �豸 id
    u32 dwDevChannelId;                         // �豸ͨ��id
    u32 dwPreRecTime;                           // Ԥ¼ʱ�䳤��

    TTnmNruMediaInfo tMediaInfo;                // ý����Ϣ
    TTnmUserId      tUserId;                    // �ͻ����û�ID
    u8  byHasAudio;                             // �Ƿ������Ƶ
    u8  byReserve1;     // 2006/04/05 ���ǿ ���ֽڶ���
    u8  byReserve2;
    u8  byReserve3;
    
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwChnnlId = dwDevChannelId;
        u32 dwRecTime = dwPreRecTime;
        if (bNetToHost) 
        {
            dwChnnlId = ntohl(dwDevChannelId);
            dwRecTime = ntohl(dwPreRecTime);
        }
        OspPrintf(TRUE, FALSE, "\tDevId=%s, ChId=0x%lx, PRecTime=0x%lx\n",
            tDevId.achID, dwChnnlId, dwRecTime);
        OspPrintf(TRUE, FALSE, "\tUsrId=%s, Audio=%d\n", tUserId.achID, byHasAudio);
        OspPrintf(TRUE, FALSE, "\t");
        tMediaInfo.Print(bNetToHost);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruPreRecTask;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmNruPreRecTask::NetToHost()
{
    dwDevChannelId = ntohl(dwDevChannelId);
    dwPreRecTime = ntohl(dwPreRecTime);
    tMediaInfo.NetToHost();
}
inline void TTnmNruPreRecTask::HostToNet()
{
    dwDevChannelId = htonl(dwDevChannelId);
    dwPreRecTime = htonl(dwPreRecTime);
    tMediaInfo.HostToNet();
}
     
// 2.6.5 NRU�������ܲ���
#define DISK_VALID      (u8)1
#define DISK_INVALID    (u8)0
typedef struct tagTnmNruPfmInfo : public TTnmPfmHwInfo
{
    u32 adwRemainDisk[TNM_MAX_NRU_DISK_NUM +1];   // ������ʣ��������ѯ��ֻ������M��
    u8  abyDiskState[TNM_MAX_NRU_DISK_NUM + 1];       // �������Ƿ���ã������������һһ��ӦDISK_VALID/DISK_INVALID 
    u32 dwRecTaskNum;       // ¼������·�� 
    u32 dwRecBitRate;       // ¼������(bps), ��ʱδ�ṩ
    u8  byCurDiskNum;       // ��ǰʹ���˶��ٴ���
    u8  byCpuAlarm;         // CPU״̬��ֻ���� enum TnmStatusType
    u8  byDiskFullAlarm;    // ����״̬��ֻ����enum TnmStatusType
    
    u8  byReserve1;     //
    u32 adwFullDisk[TNM_MAX_NRU_DISK_NUM +1];   // ������������ֻ������M��
    
    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        byCurDiskNum    = 0;
        for(s32 nIndex = 0; nIndex < TNM_MAX_NRU_DISK_NUM+1; nIndex++)
        {
            adwRemainDisk[nIndex] = 0;
            adwFullDisk[nIndex] = 0;
            abyDiskState[nIndex] = 0;
        }

        byCpuAlarm  = TNM_STATUS_NONE;
        byDiskFullAlarm = TNM_STATUS_NONE;
        dwRecTaskNum    = 0;
        dwRecBitRate    = 0;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPfmHwInfo::Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tDiskNum: %d; CpuAlarm: %d; DiskFull: %d;\n", 
            byCurDiskNum, byCpuAlarm, byDiskFullAlarm);
        OspPrintf(TRUE, FALSE, "\tDiskRemain: ");
        for (s32 nIndex = 0; nIndex < TNM_MAX_NRU_DISK_NUM; nIndex++)
        {
            u32 dwRemain = adwRemainDisk[nIndex];
            u32 dwFull = adwFullDisk[nIndex];
            if (bNetToHost) 
            {
                dwRemain = ntohl(dwRemain);
                dwFull = ntohl(dwFull);
            }
            if (dwRemain > 0) 
            {
                OspPrintf(TRUE, FALSE, "(%d, RemainDisk: 0x%lx), State: %d", nIndex, dwRemain, abyDiskState[nIndex]);
            }
            if (dwFull > 0)
            {
                OspPrintf(TRUE, FALSE, "(%d, FullDisk: 0x%lx), State: %d", nIndex, dwFull, abyDiskState[nIndex]);
            }
        }
        OspPrintf(TRUE, FALSE, "\n");
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tRecTaskNum: 0x%x; RecBitRate: 0x%x;\n",
                ntohl(dwRecTaskNum), ntohl(dwRecBitRate));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tRecTaskNum: 0x%lx; RecBitRate: 0x%lx;\n", dwRecTaskNum, dwRecBitRate);
        }
        
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmNruPfmInfo::NetToHost()
{
    TTnmPfmHwInfo::NetToHost();

    for(s32 nIndex = 0; nIndex < TNM_MAX_NRU_DISK_NUM; nIndex++)
    {
        adwRemainDisk[nIndex] = ntohl(adwRemainDisk[nIndex]);
        adwFullDisk[nIndex] = ntohl(adwFullDisk[nIndex]);
    }
    dwRecTaskNum = ntohl(dwRecTaskNum);
    dwRecBitRate = ntohl(dwRecBitRate);
}
inline void TTnmNruPfmInfo::HostToNet()
{
    TTnmPfmHwInfo::HostToNet();

    for(s32 nIndex = 0; nIndex < TNM_MAX_NRU_DISK_NUM; nIndex++)
    {
        adwRemainDisk[nIndex] = htonl(adwRemainDisk[nIndex]);
        adwFullDisk[nIndex] = htonl(adwFullDisk[nIndex]);
    }
    dwRecTaskNum = htonl(dwRecTaskNum);
    dwRecBitRate = htonl(dwRecBitRate);
}

//��ȡNru�Ĵ���¼�����
typedef struct tagTnmNruDiskPfmInfo
{
    tagTnmNruDiskPfmInfo()
    {
        memset(achMountPath, 0, sizeof(achMountPath));
        dwRecNum =0;
		dwTotalSize = 0;
		dwFreeSize = 0;
    }

    s8 achMountPath[MAX_MOUNTPATH_LEN];	//���ط�����
    u32 dwRecNum;						//¼�����
	u32 dwTotalSize;					//������������MΪ��λ
	u32 dwFreeSize;						//��������������MΪ��λ

    
    void NetToHost();
    void HostToNet();
    
}PACKED TTnmNruDiskPfmInfo;

inline void tagTnmNruDiskPfmInfo::NetToHost()
{
    dwRecNum = ntohl(dwRecNum);
	dwTotalSize = ntohl(dwTotalSize);
	dwFreeSize = ntohl(dwFreeSize);
}

inline void tagTnmNruDiskPfmInfo::HostToNet()
{
    dwRecNum = htonl(dwRecNum);
	dwTotalSize = htonl(dwTotalSize);
	dwFreeSize = htonl(dwFreeSize);
}
/*====================================================================
2.7. 3AS���ܲ���
====================================================================*/
typedef struct tagTnm3asPfmInfo : public TTnmBasePfmInfo
{
    u32 dwUserLogonNum;     // ��¼�û���
    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        dwUserLogonNum  = 0;
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBasePfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tUsrLogonNum: 0x%x; \n", ntohl(dwUserLogonNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tUsrLogonNum: 0x%lx; \n", dwUserLogonNum);
        }
        
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnm3asPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnm3asPfmInfo::NetToHost()
{
    TTnmBasePfmInfo::NetToHost();

    dwUserLogonNum = ntohl(dwUserLogonNum);
}
inline void TTnm3asPfmInfo::HostToNet()
{
    TTnmBasePfmInfo::HostToNet();

    dwUserLogonNum = htonl(dwUserLogonNum);
}
/*====================================================================
2.8. UAS���ܲ���
====================================================================*/
typedef struct tagTnmUasPfmInfo : public TTnmBasePfmInfo
{
    u32 dwCmuLinkNum;       // CMU������
    u8  byNetUseRate;       // ������Դʹ���� 100%
    u8  byDBStatus;         // ���ݿ�����״̬ enum TnmStatusType

    u8  byReserve1;     // 2006/04/05 ���ǿ ���ֽڶ���
    u8  byReserve2;

    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        byNetUseRate    = 0;
        byDBStatus  = TNM_STATUS_NONE;
        dwCmuLinkNum    = 0;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBasePfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tNUseRate: %d; DbStatus: %d; LinkNum: 0x%x;\n",
                byNetUseRate, byDBStatus, ntohl(dwCmuLinkNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tNUseRate: %d; DbStatus: %d; LinkNum: 0x%lx;\n",
                byNetUseRate, byDBStatus, dwCmuLinkNum);
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmUasPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmUasPfmInfo::NetToHost()
{
    TTnmBasePfmInfo::NetToHost();

    dwCmuLinkNum = ntohl(dwCmuLinkNum);
}
inline void TTnmUasPfmInfo::HostToNet()
{
    TTnmBasePfmInfo::HostToNet();

    dwCmuLinkNum = htonl(dwCmuLinkNum);
}

// 2.9 CUI ���ܲ���
// 2.9.1 CUI �������ܲ���
typedef struct tagTnmCuiPfmInfo : public TTnmBoardPfmInfo
{
    u16 wCuLinkNum;             // ��ǰ�������ӵ�CU��Ŀ

    u8  byHotBackupState;       //��ȱ�������TNM_HB_STATE_ACTIVE
    u8  byReserve2;

    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        wCuLinkNum = 0;
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBoardPfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tCuLinkNum: 0x%hx;\n", ntohs(wCuLinkNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tCuLinkNum: 0x%hx;\n", wCuLinkNum);
        }
        
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuiPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmCuiPfmInfo::NetToHost()
{
    TTnmBasePfmInfo::NetToHost();

    wCuLinkNum = ntohs(wCuLinkNum);
}
inline void TTnmCuiPfmInfo::HostToNet()
{
    TTnmBasePfmInfo::HostToNet();

    wCuLinkNum = htons(wCuLinkNum);
}

// 2.9.2 CUI ��չ���ܲ���
typedef struct tagTnmCuiPfmInfoEx
{
    tagTnmCuiPfmInfoEx()
    {
        dwCuIp = 0;
        dwLoginTime = 0;
    }
    TTnmUserId tUserId;         // CU�û���¼��
    u32 dwCuIp;                 // CU�ĵ�¼IP
    u32 dwLoginTime;            // CU��¼��ʱ��
    
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwTime = dwLoginTime;
        u32 dwIp = dwCuIp;
        if (bNetToHost) 
        {
            dwTime = ntohl(dwTime);
            dwIp = ntohl(dwIp);
        }
        
        OspPrintf(TRUE, FALSE, "UsrId: %s; CuIp: %s;\n",
                          tUserId.achID, ::GetIpStr(dwIp)); 
        if (dwTime & 0x80000000) //������λΪ 1 ˵���ǷǷ�ʱ�����
        {
            OspPrintf(TRUE, FALSE, "\tʱ��ֵ�Ƿ�: %lx;\n", dwTime);
        }
        else
        {
            struct tm tmTime = *localtime((time_t*)&dwTime);
            OspPrintf(TRUE, FALSE, "\tLoginTime: %s;\n", asctime(&tmTime));
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuiPfmInfoEx;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmCuiPfmInfoEx::NetToHost()
{
    dwCuIp = ntohl(dwCuIp);
    dwLoginTime = ntohl(dwLoginTime);
}
inline void TTnmCuiPfmInfoEx::HostToNet()
{
    dwCuIp = htonl(dwCuIp);
    dwLoginTime = htonl(dwLoginTime);
}

// 2.10 PUI ���ܲ���
// 2.10.1 PUI �������ܲ���
typedef struct tagTnmPuiPfmInfo : public TTnmBoardPfmInfo
{
    u16 wCurPuNum;              // ��ǰ���ӵ�PU����
    
    u8  byHotBackupState; //��ȱ�������TNM_HB_STATE_ACTIVE
    u8  byReserve2;

    // 2006/04/03 ���ǿ ���ӳ�ʼ���ӿ�
    void Init()
    {
        wCurPuNum   = 0;
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBoardPfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tCurPuNum: 0x%hx; \n", ntohs(wCurPuNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tCurPuNum: 0x%hx; \n", wCurPuNum);
        }
        
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuiPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPuiPfmInfo::NetToHost()
{
    TTnmBoardPfmInfo::NetToHost();

    wCurPuNum = ntohs(wCurPuNum);
}
inline void TTnmPuiPfmInfo::HostToNet()
{
    TTnmBoardPfmInfo::HostToNet();

    wCurPuNum = htons(wCurPuNum);
}
// 2.10.2 PUI ��չ���ܲ���
typedef struct tagTnmPuiPfmInfoEx
{
    tagTnmPuiPfmInfoEx()
    {
        dwPuIp = 0;
        byServiceType = TNM_SERVICE_UNKNOW;
        memset(abyGuid, 0, sizeof(abyGuid));
        tPuId.SetNull();
    }
    u32 dwPuIp;                     // PU��IP��ַ
    TTnmDeviceId tPuId;             // PU�ĵ��豸ID
    u8  abyGuid[TNM_PU_GUID_LEN];   // �豸��GUID
    u8  byServiceType;              // �豸��ҵ������enum TnmServiceType
    u8  byReserve1;     // 2006/04/05 ���ǿ ���ֽڶ���
    u8  byReserve2;
    u8  byReserve3;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        tPuId.achID[TNM_MAX_DEVICE_ID_LEN+3] = '\0';
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "PuIp: %s; PuId: %s; SType: %d;\n",
                ::GetIpStr(ntohl(dwPuIp)), tPuId.achID, byServiceType);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "PuIp: %s; PuId: %s; SType: %d;\n",
                ::GetIpStr(dwPuIp), tPuId.achID, byServiceType);
        }
        OspPrintf(TRUE, FALSE, "\tGuid: ");
        for (s32 nIndex = 0; nIndex < TNM_PU_GUID_LEN; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyGuid[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuiPfmInfoEx;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmPuiPfmInfoEx::NetToHost()
{
    dwPuIp = ntohl(dwPuIp);
}
inline void TTnmPuiPfmInfoEx::HostToNet()
{
    dwPuIp = htonl(dwPuIp);
}

// 2.11 VTDU
// 2.11.1 VTDU�������ܲ���
typedef struct tagTnmVtduPfmInfo : public TTnmBoardPfmInfo
{
    TTnmDeviceId tDevId;    // VTDU�豸ID
    u16 wRcvStreamCurNum;                     // ������Ƶ��·��
    u16 wSndStreamCurNum;                     // ������Ƶ��·��
    u16 wRcvLostPackNum;    // �����ܶ�����
    u16 wTaskNum;           // ת��������
    u16 wRcvStreamMaxNum;                    // ��������Ƶ��·��
    u16 wSndStreamMaxNum;                    // �������Ƶ��·��
    u32 dwRcvStreamMaxNumTime;              // ������������Ƶ��·����ʱ��
    u32 dwSndStreamMaxNumTime;              // �����������Ƶ��·����ʱ��
    void Init()
    {
        tDevId.SetNull();
        wRcvStreamCurNum    = 0;
        wSndStreamCurNum    = 0;
        wRcvLostPackNum = 0;
        wTaskNum        = 0;
        wRcvStreamMaxNum = 0;
        wSndStreamMaxNum = 0;
        dwRcvStreamMaxNumTime = 0;
        dwSndStreamMaxNumTime = 0;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBoardPfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tDevId: %s; RcvNum: 0x%hx; SndNum: 0x%hx;\n",
                tDevId.achID, ntohs(wRcvStreamCurNum), ntohs(wSndStreamCurNum));
            OspPrintf(TRUE, FALSE, "\tLstNum: 0x%hx; TaskNum: 0x%hx;\n", 
                ntohs(wRcvLostPackNum), ntohs(wTaskNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tDevId: %s; RcvNum: 0x%hx; SndNum: 0x%hx;\n",
                tDevId.achID, wRcvStreamCurNum, wSndStreamCurNum);
            OspPrintf(TRUE, FALSE, "\tLstNum: 0x%hx; TaskNum: 0x%hx;\n",  wRcvLostPackNum, wTaskNum);
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVtduPfmInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVtduPfmInfo::NetToHost()
{
    TTnmBoardPfmInfo::NetToHost();

    wRcvStreamCurNum = ntohs(wRcvStreamCurNum);
    wSndStreamCurNum = ntohs(wSndStreamCurNum);
    wRcvLostPackNum = ntohs(wRcvLostPackNum);
    wTaskNum = ntohs(wTaskNum);
    wRcvStreamMaxNum = ntohs(wRcvStreamMaxNum);
    wSndStreamMaxNum = ntohs(wSndStreamMaxNum);
    dwRcvStreamMaxNumTime = ntohl(dwRcvStreamMaxNumTime);
    dwSndStreamMaxNumTime = ntohl(dwSndStreamMaxNumTime);
}
inline void TTnmVtduPfmInfo::HostToNet()
{
    TTnmBoardPfmInfo::HostToNet();

    wRcvStreamCurNum = htons(wRcvStreamCurNum);
    wSndStreamCurNum = htons(wSndStreamCurNum);
    wRcvLostPackNum = htons(wRcvLostPackNum);
    wTaskNum = htons(wTaskNum);
    wRcvStreamMaxNum = htons(wRcvStreamMaxNum);
    wSndStreamMaxNum = htons(wSndStreamMaxNum);
    dwRcvStreamMaxNumTime = htonl(dwRcvStreamMaxNumTime);
    dwSndStreamMaxNumTime = htonl(dwSndStreamMaxNumTime);
}
// 2.11.2 VTDU����չ���ܲ���
typedef struct tagTnmVtduTask
{
    tagTnmVtduTask()
    {
        tDevId.SetNull();
        dwSrcDevIp      = 0;
        wSrcDevPort     = 0;
        dwRcvBitRate    = 0;
        dwRcvPackNum    = 0;
        dwRcvBytes      = 0;
        dwDestDevIp     = 0;
        wDestDevPort    = 0;
        dwSendBitRate   = 0;
        dwSendPacknum   = 0;
        dwSendBytes     = 0;
        byVideoType     = TNM_VIDEO_TYPE_NONE;
        byAudioType     = TNM_AUDIO_TYPE_NONE;
        byProtocolType  = TNM_MEDIA_STREAM_TYPE_TCP;
        byMediaType     = TNM_AUDIO_VIDEO;
    }
    TTnmDeviceId tDevId;    // VTDU�豸ID
    u32 dwSrcDevIp;         // Դ�豸IP
    u32 dwRcvBitRate;       // ��������(kbps)
    u32 dwRcvPackNum;       // ���հ���
    u32 dwRcvBytes;         // �������ֽ�����byte��
    u32 dwDestDevIp;        // Ŀ���豸IP
    u32 dwSendBitRate;      // ��������(kbps)
    u32 dwSendPacknum;      // ���Ͱ���
    u32 dwSendBytes;        // �����ֽ���(byte)
    u16 wSrcDevPort;        // Դ�豸�˿�
    u16 wDestDevPort;       // Ŀ���豸�˿�
    u8  byVideoType;        // ��Ƶ������ʽ
                            //  TNM_VIDEO_TYPE_SN4
                            //  TNM_VIDEO_TYPE_MPEG4
                            //  TNM_VIDEO_TYPE_H261
                            //  TNM_VIDEO_TYPE_H263
                            //  TNM_VIDEO_TYPE_H264
    u8  byAudioType;        // ��Ƶ������ʽ
                            //  TNM_AUDIO_TYPE_PCMA
                            //  TNM_AUDIO_TYPE_PCMU
                            //  TNM_AUDIO_TYPE_GSM
                            //  TNM_AUDIO_TYPE_G729
                            //  TNM_AUDIO_TYPE_MP3
    u8 byProtocolType;      // Э������
                            //  TNM_MEDIA_STREAM_TYPE_UDP
                            //  TNM_MEDIA_STREAM_TYPE_TCP
    u8 byMediaType;         // ����ģ��
                            //  TNM_ONLY_VIDEO
                            //  TNM_ONLY_AUDIO
                            //  TNM_AUDIO_VIDEO
    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; SrcIp: %s; SPort:0x%x; RcvRate:0x%x;\n",
                tDevId.achID, ::GetIpStr(ntohl(dwSrcDevIp)), ntohs(wSrcDevPort), ntohl(dwRcvBitRate));
            OspPrintf(TRUE, FALSE, "\tRcvNum: 0x%x; RcvByte: 0x%x; DstIp: %s; DstPort:0x%x;\n",
                ntohl(dwRcvPackNum), ntohl(dwRcvBytes), ::GetIpStr(ntohl(dwDestDevIp)), ntohs(wDestDevPort));
            OspPrintf(TRUE, FALSE, "\tSndRate: 0x%x; SndNum: 0x%x; SndByte:0x%x; VType:%d;\n",
                ntohl(dwSendBitRate), ntohl(dwSendPacknum), ntohl(dwSendBytes), byVideoType);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; SrcIp: %s; SPort:0x%hx; RcvRate:0x%lx;\n",
                tDevId.achID, ::GetIpStr(dwSrcDevIp), wSrcDevPort, dwRcvBitRate);
            OspPrintf(TRUE, FALSE, "\tRcvNum: 0x%lx; RcvByte: 0x%lx; DstIp: %s; DstPort:0x%hx;\n",
                dwRcvPackNum, dwRcvBytes, ::GetIpStr(dwDestDevIp), wDestDevPort);
            OspPrintf(TRUE, FALSE, "\tSndRate: 0x%lx; SndNum: 0x%lx; SndByte:0x%lx; VType:%d;\n",
                dwSendBitRate, dwSendPacknum, dwSendBytes, byVideoType);
        }
        OspPrintf(TRUE, FALSE, "\tAuType: %d; ProType: %d; MediaType: %d;\n",
            byAudioType, byProtocolType, byMediaType);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmVtduTask;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmVtduTask::NetToHost()
{
    dwSrcDevIp = ntohl(dwSrcDevIp);
    dwRcvBitRate = ntohl(dwRcvBitRate);
    dwRcvPackNum = ntohl(dwRcvPackNum);
    dwRcvBytes = ntohl(dwRcvBytes);
    dwDestDevIp = ntohl(dwDestDevIp);
    dwSendBitRate = ntohl(dwSendBitRate);
    dwSendPacknum = ntohl(dwSendPacknum);
    dwSendBytes = ntohl(dwSendBytes);
    wSrcDevPort = ntohs(wSrcDevPort);
    wDestDevPort = ntohs(wDestDevPort);
}
inline void TTnmVtduTask::HostToNet()
{
    dwSrcDevIp = htonl(dwSrcDevIp);
    dwRcvBitRate = htonl(dwRcvBitRate);
    dwRcvPackNum = htonl(dwRcvPackNum);
    dwRcvBytes = htonl(dwRcvBytes);
    dwDestDevIp = htonl(dwDestDevIp);
    dwSendBitRate = htonl(dwSendBitRate);
    dwSendPacknum = htonl(dwSendPacknum);
    dwSendBytes = htonl(dwSendBytes);
    wSrcDevPort = htons(wSrcDevPort);
    wDestDevPort = htons(wDestDevPort);
}
//////////////////////////////////////////////////////////////////////////
// 3.  �豸����ͼ
// 3.1 �豸������Ŀ
typedef struct tagTnmDevTopo
{
    tagTnmDevTopo()
    {
        tDevId.SetNull();
        memset(achDevName, 0, sizeof(achDevName));
        memset(achDevModeInfo, 0, sizeof(achDevModeInfo));
        memset(achNetAddress, 0, sizeof(achNetAddress));
    }

    TTnmDeviceId tDevId;                                // �豸��ID
    s8  achDevName[TNM_MAX_DEV_NAME_LEN + 4];           // �豸����
    s8  achDevModeInfo[TNM_MAX_DEV_MODEINFO_LEN + 4];   // �豸�ͺ�
    s8  achNetAddress[TNM_MAX_DEV_NETADDR_LEN + 4];     // �豸�����ַ��IP������(www.**)
}PACKED TTnmDevTopo;

// 4.�豸�澯
// �澯������Ϣ
typedef struct tagTnmDevAlarm
{
    tagTnmDevAlarm()
    {
        dwDevSerialNo = 0;
        dwAlarmCode = 0;
    }
    u32 dwDevSerialNo;          // �豸�����澯��ˮ�ţ����豸��̬���ɡ���Ҫ���ڸ澯�����ͻָ�
                                // ��ƥ�䡣ͬһ��澯��û�лָ�֮ǰ�����ϱ�����ˮ����ͬһ��
    u32 dwAlarmCode;            // �澯�룬���ڱ�ʶ�澯������,�μ��澯�궨��

    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwSn = dwDevSerialNo;
        u32 dwCode = dwAlarmCode;
        if (bNetToHost) 
        {
            dwSn = ntohl(dwDevSerialNo);
            dwCode = ntohl(dwAlarmCode);
        }
        
        OspPrintf(TRUE, FALSE, "Sn=0x%lx, code=%s(0x%lx), ", 
            dwSn, ::GetAlarmCodeDesc(dwCode), dwCode);
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmDevAlarm;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmDevAlarm::NetToHost()
{
    dwDevSerialNo = ntohl(dwDevSerialNo);
    dwAlarmCode = ntohl(dwAlarmCode);
}

inline void TTnmDevAlarm::HostToNet()
{
    dwDevSerialNo = htonl(dwDevSerialNo);
    dwAlarmCode = htonl(dwAlarmCode);
}

typedef struct tagTnmCuBrowseInfo
{
    tagTnmCuBrowseInfo()
    {
        m_startTime = 0;
        m_endTime = 0;
        memset(m_cuName, 0, sizeof(m_cuName));
        m_connectReqCount = 0;
        m_connectSuccessCount = 0;
        m_connectFailureCount = 0;
        m_releaseReqCount = 0;
        m_releaseSuccessCount = 0;
        m_releaseFailureCount = 0;
    }
    
    u32 m_startTime;
    u32 m_endTime;
    s8  m_cuName[TNM_MAX_STRING_LEN*2 + 4];
    u16 m_connectReqCount;
    u16 m_connectSuccessCount;
    u16 m_connectFailureCount;
    u16 m_releaseReqCount;
    u16 m_releaseSuccessCount;
    u16 m_releaseFailureCount;

    void NetToHost();
    void HostToNet();
}PACKED TTnmCuBrowseInfo;

inline void tagTnmCuBrowseInfo::NetToHost()
{
    m_startTime = ntohl(m_startTime);
    m_endTime = ntohl(m_endTime);
    m_connectReqCount = ntohs(m_connectReqCount);
    m_connectSuccessCount = ntohs(m_connectSuccessCount);
    m_connectFailureCount = ntohs(m_connectFailureCount);
    m_releaseReqCount = ntohs(m_releaseReqCount);
    m_releaseSuccessCount = ntohs(m_releaseSuccessCount);
    m_releaseFailureCount = ntohs(m_releaseFailureCount);
}

inline void tagTnmCuBrowseInfo::HostToNet()
{
    m_startTime = htonl(m_startTime);
    m_endTime = htonl(m_endTime);
    m_connectReqCount = htons(m_connectReqCount);
    m_connectSuccessCount = htons(m_connectSuccessCount);
    m_connectFailureCount = htons(m_connectFailureCount);
    m_releaseReqCount = htons(m_releaseReqCount);
    m_releaseSuccessCount = htons(m_releaseSuccessCount);
    m_releaseFailureCount = htons(m_releaseFailureCount);
}

//���������Ϣ
typedef struct tagTnmVersionInfo
{
    u32 dwVersionServerAddr;                                //�汾��������ַ
    u16 wVersionServerPort;                                 //�汾�������˿�
    u16 wReserved;
    s8  achVerServUsername[TNM_MAX_USER_ID_LEN + 4];        //�汾��������½�û���
    s8  achVerServPassword[TNM_MAX_PASSWORD_LEN + 4];       //�汾��������½����
    s8  achVersionFullPath[TNM_MAX_VERSION_PATH_LEN + 4];   //�汾�ļ����ڵ�ȫ·��(�����ļ���)
    void NetToHost();
    void HostToNet();
}TTnmVersionInfo;

inline void tagTnmVersionInfo::NetToHost()
{
    dwVersionServerAddr = ntohl(dwVersionServerAddr);
    wVersionServerPort = ntohs(wVersionServerPort);
}

inline void tagTnmVersionInfo::HostToNet()
{
    dwVersionServerAddr = htonl(dwVersionServerAddr);
    wVersionServerPort = htons(wVersionServerPort);
}

/************************************************************************/
/*                          ����������                                */
/************************************************************************/
#define TNM_MAX_UPDATEFILE_LEN  32    //�����ļ�����󳤶�
#define TNM_MAX_UPDATEPATH_LEN  128   //�����ļ�·����󳤶�

#define TNM_MAX_DEVICE_BLOCKID_NUM      8       // һ���豸һ�β������ͬʱ�ϴ���ģ�����

// ����汾�ļ�����
#define BLOCK_ID_INVALID            (u8)0xFF    // ��Ч���ڴ��ID
#define BLOCK_ID_MDOS_Z             (u8)1       // mdos.z
#define BLOCK_ID_MEDIACTRLKDM       (u8)2       // Mediactrlkdm
#define BLOCK_ID_FONT               (u8)3       // FONT
#define BLOCK_ID_KDMVS_Z            (u8)4       // KDMVS.z
#define BLOCK_ID_KDM2417Z_IOS       (u8)5       // kdm2417z.ios
#define BLOCK_ID_MEDIACTRL          (u8)6       // mediactrl
#define BLOCK_ID_KERNEL_UPACK       (u8)7       // kernel.flashsei.upack
#define BLOCK_ID_KDMAPP_TGZ         (u8)8       // kdmapp.tgz
#define BLOCK_ID_MODULES_TGZ        (u8)9       // modules.tgz
#define BLOCK_ID_KDM2428Z_IOS       (u8)10      // kdm2428z.ios
#define BLOCK_ID_KDMVS_BIN          (u8)11      // KDMVS.bin

//  KDM2400/KDM2500��   mdos.z,Mediactrlkdm, FONT, KDMVS.z
//  KDM2417��           kdm2417z.ios,  KdmMediaCtrl, FONT, KDMVS.z
//  KDM2428��           kdm2428z.ios, KdmMediaCtrl, FONT, KDMVS.z
//  KDM2418/KDM2518��   kernel.flashsei.upack, kdmapp.tgz, modules.tgz


//cmu
#define BLOCK_ID_MCU_IOS            (u8)20    //mcu.ios
#define BLOCK_ID_MCU_Z              (u8)21    //mcu.z
//cri
#define BLOCK_ID_IOS                (u8)30    //ios
#define BLOCK_ID_KDVMPCRI_Z         (u8)31    //kdvmpcri.z
//2100a
#define BLOCK_ID_KDM2100A_IOS       (u8)40    //KDM2100A.ios
#define BLOCK_ID_KDMMEDIAGW_Z       (u8)41    //kdvmediagw.z

#define BLOCK_ID_MMP_Z              (u8)50    //mmp.z
//#define BLOCK_ID_KDMVS_Z          (u8)51    //kdmvs.z

#define BLOCK_ID_VASOS              (u8)53    //vasos
#define BLOCK_ID_VASCTRL_Z          (u8)54    //VasCtrl.z

#define BLOCK_ID_RPSERVER_EXE       (u8)60    //rpserver.exe

// rnu
#define BLOCK_ID_NRU_WIN32          (u8)70      // win32 -> nru.exe

// cui
#define BLOCK_ID_CUI_VXWORKS        (u8)71      // VxWorks -> cui
#define BLOCK_ID_CUI_LINUXS         (u8)72      // Linuxs -> cui
// pui
#define BLOCK_ID_PUI_VXWORKS        (u8)73      // VxWorks -> pui
#define BLOCK_ID_PUI_LINUXS         (u8)74      // Linuxs -> pui

// uas
#define BLOCK_ID_UAS_WIN32          (u8)75      // win32 -> uas.exe
#define BLOCK_ID_UAS_LINUXS         (u8)76      // Linuxs -> uas

// vtdu
#define BLOCK_ID_VTDU_VXWORKS       (u8)77      // VxWorks -> vtdu
#define BLOCK_ID_VTDU_LINUXS        (u8)78      // Linuxs -> vtdu

// cmu
#define BLOCK_ID_CMU_VXWORKS        (u8)80      // VxWorks -> cmu
#define BLOCK_ID_CMU_LINUXS         (u8)81      // Linuxs -> cmu
#define BLOCK_ID_CMU_WIN32          (u8)82      // Linuxs -> cmu.exe



// �����ڴ�������
#define TNM_TRANS_FILE                      (u8)0x1     // �����ļ�
#define TNM_TRANS_BUFFER                    (u8)0x2     // ����һ���ڴ�

// ȱʡ�ڴ����Ƭ��С
#define TNM_DEF_SLICE_SIZE                  (16 * 1024) // 16K�ֽ�

// �ڴ�������Ϣ
typedef struct tagTnmBlockInfo
{
    tagTnmBlockInfo()
    {
        memset(this, 0, sizeof(tagTnmBlockInfo));
    }

    /*========================================================================
    �� �� ����GetSliceNum
    ��    �ܣ��õ���Ƭ������
    ��    ����
    �� �� ֵ��
    --------------------------------------------------------------------------
    �޸ļ�¼��
    ��      ��  �汾    �޸���  �޸�����
    2006/03/21  3.9     ���ǿ  ����
    ========================================================================*/
    u32 GetSliceNum() const;
    
    u32 dwTotalSize;            // �ڴ��ȫ����С
    u32 dwSliceSize;            // ������Ƭ�Ĵ�С, ����Ч����

    u8  byTransType;        // �ڴ�����ͣ���buffer,�����ļ�
                            // TNM_TRANS_FILE
                            // TNM_TRANS_BUFFER
    u8  byBlockId;          // ��ʶ�ļ����ڴ�����ݣ�������汾�ļ��ĺ궨��
    u8  byReserve1;         // 2006/04/05 ���ǿ ���ֽڶ���
    u8  byReserve2;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "TrsType: %d; BlkId: %d; TalSize: 0x%x;\n", 
                byTransType, byBlockId, ntohl(dwTotalSize));
            OspPrintf(TRUE, FALSE, "\tSlicSize: 0x%x; SlicNum: 0x%x;\n",
                ntohl(dwSliceSize), ntohl(GetSliceNum()));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "TrsType: %d; BlkId: %d; TalSize: 0x%lx;\n", 
                byTransType, byBlockId, dwTotalSize);
            OspPrintf(TRUE, FALSE, "\tSlicSize: 0x%lx; SlicNum: 0x%lx;\n",
                dwSliceSize, GetSliceNum());
        }
    }
    // 2006/04/06 ���ǿ �����ֽ���ת���Ľӿ�
    void NetToHost();
    void HostToNet();
}PACKED TTnmBlockInfo;
/*========================================================================
�� �� ����NetToHost/HostToNet
��    �ܣ��ֽ���ת���ӿڣ������շ���Ϣʱ���ã�ǰ���ϲ�ҵ����ʹ�ûص�����ʱҪ����HostToNetתΪ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/04/06  3.9     ���ǿ  ����
========================================================================*/
inline void TTnmBlockInfo::NetToHost()
{
    dwTotalSize = ntohl(dwTotalSize);
    dwSliceSize = ntohl(dwSliceSize);
}

inline void TTnmBlockInfo::HostToNet()
{
    dwTotalSize = htonl(dwTotalSize);
    dwSliceSize = htonl(dwSliceSize);
}
/*========================================================================
�� �� ����GetSliceNum
��    �ܣ��õ���Ƭ������
��    ����
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/21  3.9     ���ǿ  ����
========================================================================*/
inline u32 TTnmBlockInfo::GetSliceNum() const
{
    assert(dwSliceSize > 0);
    if (dwSliceSize < 1) 
    {
        return 0;
    }
    u32 dwSliceNum = dwTotalSize/dwSliceSize;

    if (dwTotalSize%dwSliceSize != 0)
    {
        dwSliceNum++;
    }
    return dwSliceNum;
}

/********************************************************************/
/*                         ������Ϣ��Ϣ����                         */
/********************************************************************/
#define KDMCONF_BGN        5001
#define KDMCONF_END        KDMCONF_BGN + 999


//���������Ϲ���˵�֪ͨ
#define NODE_CONNECTED_NOTIFY           KDMCONF_BGN
//���������˶Ͽ���֪ͨ
#define NODE_DISCONNECTED_NOTIFY        KDMCONF_BGN + 1

//3as�����ܽ����ɹ�֪ͨ��Ϣ
//��Ϣ�壺TnmLinkInfo
#define NODE_CONNECTED_NMS_NOTIFY       KDMCONF_BGN + 2

/********************************************************************/
/* 1.   ���ù������ò����Ļ�ȡ������                              */
/********************************************************************/
/*====================================================================
1.1.    �������ò���
====================================================================*/

// ����汾����
// ֪ͨ�������: TTnmVersionInfo
// nms -> agent(req):  TTnmVersionInfo
// nms <- agent(ack):  
// nms <- agent(nack): 
#define NODE_SOFTWARE_UPGRADE           KDMCONF_BGN + 3

// ��������汾��Ϣ
// ֪ͨ������ݣ�TTnmBlockInfo
// nms -> agent(req): TTnmBlockInfo
// nms <- agent(ack): 
#define NODE_SOFTWARE_INFO              KDMCONF_BGN + 4
// ����汾�зְ�
// nms -> agent(notify): u8(byBlockID) + u32(��Ƭ���) + u32(��Ч���ݳ���) + ��Ч����
// nms <- agent(ack): u8(byBlockID) + u32(��Ƭ���)
// nms <- agent(nack):
// nms <- agent(finish):u8(byBlockID) + u32(��Ƭ���)
#define NODE_SOFTWARE_PACKAGE           KDMCONF_BGN + 5
//��������汾�ϴ����
//nms->agent:
#define NODE_SOFTWARE_UPLOAD_COMPLETE   KDMCONF_BGN + 6
        
//ϵͳӲ����λ
//��ѯ�������������: ��
#define NODE_SYSTEM_RESET               KDMCONF_BGN + 7

//ϵͳ�Լ�
//��ѯ�������������: ��
#define NODE_SELF_TEST                  KDMCONF_BGN + 8

//��ע����Ϣ
//��ѯ�������������: ��
#define NODE_REPEAT_REGISTER            KDMCONF_BGN + 10

//�����ļ�
//��ѯ�������������: u8[65535]�ļ�����
#define NODE_SYSPUBCFG_FILE             KDMCONF_BGN + 11

//�������ò���
//��ѯ�������������: TTnmPubCfgInfo
#define NODE_SYSPUBCFGINFO              KDMCONF_BGN + 15

//ƽ̨�豸ϵͳ������TTnmServerSysCfgInfo
#define NODE_SERVER_SYSCFGINFO          KDMCONF_BGN + 16

// 1.2. ǰ���豸���ò���
// 1.2.1.   ϵͳ����
//��ѯ�������������: TTnmPuCapSet
#define NODE_PU_CAPSETINFO              KDMCONF_BGN + 17

// 1.2.1.   ǰ���豸��DEC5ϵͳ����
//��ѯ�������������: TTnmPuSysCfgInfo
#define NODE_PU_SYSCFGINFO             KDMCONF_BGN + 20
//��ѯ���������ǰ�˴洢�豸(2417/2428)������: TTnmPuStoreSysCfgInfo
#define NODE_PU_STORE_SYSCFGINFO       KDMCONF_BGN + 22

// 1.2.2.   PPPOE����(pu & Dec5)
//��ѯ�������������: TTnmPPPOECfgInfo
#define NODE_PU_PPPOECFGINFO           KDMCONF_BGN + 25

// 1.2.3.   ��Ƶ�����������ͨ����
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(��Ƶ����ͨ������) + TTnmVideoEncCfgInfo����
#define NODE_PU_VIDEOENCINFO            KDMCONF_BGN + 30

// 1.2.4.   ��Ƶ�����������ͨ����(pu & dec5)
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(��Ƶ����ͨ������) + TTnmVideoDecCfgInfo����
#define NODE_PU_VIDEODECINFO            KDMCONF_BGN + 35

// 1.2.5.   ��Ƶ������������ͨ����
//  ��Ƶ���������(pu & dec5)
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(��Ƶ����ͨ������) + TTnmAudioEncCfgInfo����
#define NODE_PU_AUDIO_ENC_INFO          KDMCONF_BGN + 40
//��Ƶ���������
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(��Ƶ����ͨ������) + TTnmAudioDecCfgInfo����
#define NODE_PU_AUDIO_DEC_INFO          KDMCONF_BGN + 41

// 1.2.6.   ��ƵԴ����˿ڲ��������˿ڣ�
//��ѯ���: u8(��ƵԴ����˿ڸ���) + TTnmVideoInputCfgInfo���� + u8(֧�ֵ�����ͷ����) + TTnmCameraInfo����
//��������: u8(��ƵԴ����˿ڸ���) + TTnmVideoInputCfgInfo����
#define NODE_PU_VIDEOINPUTINFO          KDMCONF_BGN + 45

// 1.2.7.   ���ڲ��������˿ڣ�
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(�˿ڸ���) + TTnmLPTCfgInfo����
#define NODE_LPTCFGINFO                 KDMCONF_BGN + 50

// 1.2.8.   ���ڲ����������ڣ�
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(���ڸ���) + TTnmSerialCfgInfo����
#define NODE_SERIALCFGINFO              KDMCONF_BGN + 55

// 1.2.9.   ǰ�˴洢��������ͨ����
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(ͨ������) + TTnmStorageCfgInfo����
#define NODE_PU_STORAGECFGINFO          KDMCONF_BGN + 60

// 1.2.10  ǰ���豸��ʱ��ÿ��¼�����
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(ͨ������) + TTnmPuPeriodPolicy����
#define NODE_PU_PERIOD_WOR_POLICY       KDMCONF_BGN + 62

// 1.3. ƽ̨�豸���ò���
// 1.3.1.   ϵͳ���ò���
//��ѯ�������������: TTnmCmuCfgInfo
#define NODE_CMU_SYSCFGINFO             KDMCONF_BGN + 65

// 1.3.2.   �������ò���
//��ѯ�������������: TTnmCmuNetCfgInfo + u8(��������) + TTnmEthernetCard���� +
//                    u8(���������) + TTnmSwitchBoard���� +
//                    u8(���������) + TTnmSubSwitchBoard����
//#define NODE_CMU_NETCFGINFO             KDMCONF_BGN + 70


// 1.3.3.   �������ò���
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(������) + TTnmKeyboardCfgInfo����
#define NODE_CMU_KEYBOARDINFO           KDMCONF_BGN + 75

// 1.3.4.   NRU���ò���
//��ѯ���[������]����������[�޸ĵ���Ŀ]: TTnmNruCfgInfo
#define NODE_CMU_NRUINFO                KDMCONF_BGN + 80

// 1.3.5.   �໭�渴�����ò���
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(VPU����) + TTnmVpuCfgInfo����
#define NODE_CMU_VPUINFO                KDMCONF_BGN + 85

// 1.3.6.   ��Ƶ�������ò���
//��ѯ���[������]����������[�޸ĵ���Ŀ]: u8(��Ƶ�������) + TTnmMatrixCfgInfo����
#define NODE_CMU_MATRIXINFO             KDMCONF_BGN + 90


// 1.4. �໭�渴����(VPU)���ò���
// 1.4.1.   ϵͳ���ò���
//��ѯ�������������: TTnmVpuSysCfgInfo
#define NODE_VPU_SYSTEMINFO             KDMCONF_BGN + 95

// 1.5. ��Ƶ����VAS�������ò���
// 1.5.1.   ϵͳ���ò���
//��ѯ�������������: TTnmVasSysCfgInfo
#define NODE_VAS_SYSTEMINFO             KDMCONF_BGN + 100

// 1.6. ����ҵ�����ò���
// 1.6.1.   ϵͳ���ò���
//��ѯ�������������: TTnmMainServSysCfgInfo
#define NODE_MP_SYSTEMINFO              KDMCONF_BGN + 105

//Laden���ò���
//��ѯ�������������: TTnmLadenCfgInfo
#define NODE_LADEN_CFGINFO              KDMCONF_BGN + 107

// 1.7. CUI����
//��ѯ�������������: TTnmCuiCfgInfo
#define NODE_CUI_SYSTEMINFO             KDMCONF_BGN + 110

// 1.7.2.   PUI���ò���
//��ѯ�������������: TTnmPuiCfgInfo
#define NODE_PUI_CFGINFO                KDMCONF_BGN + 115

// 1.8 VTDU���ò���
//��ѯ�������������: TTnmVtduCfgInfo
#define NODE_VTDU_CFGINFO               KDMCONF_BGN + 117

// 1.9 UAS���ò���
//��ѯ�������������: TTnmUasCfgInfo
#define NODE_UAS_CFGINFO                KDMCONF_BGN + 118

// 1.10 3AS���ò���
//��ѯ�������������: TTnm3asCfgInfo
#define NODE_3AS_CFGINFO                KDMCONF_BGN + 119


/* 2. ���ܹ���ϵͳ״̬�����ܵ�ʵʱ��ѯ                            */
// 2.1. �������ܲ���
// 2.2 ���ܷ��������ܲ���  TTnmNmsPfmInfo
#define NODE_NMS_PFMINFO                KDMCONF_BGN + 120

// 2.3. ǰ���豸���ܲ���
//��ѯ�������������: TTnmPuPfmInfo
#define NODE_PU_PFMINFO                 KDMCONF_BGN + 122

// 2.4.1. ƽ̨�豸�������ܲ���
//��ѯ�������������: TTnmCmuPfmInfo
#define NODE_CMU_PFMINFO                KDMCONF_BGN + 125

// 2.4.1.1 ƽ̨�豸��չ���ܲ���
//��ѯ�������������: TTnmCmuExtendablePfmInfo
#define NODE_CMU_EXTENDABLE_PFMINFO     KDMCONF_BGN + 126

// 2.4.1.2 ƽ̨�豸��չ���ܲ���
//��ѯ�������������: u16 ��������+TTnmCmuDiskPfmInfo����
#define NODE_CMU_DISK_PFMINFO           KDMCONF_BGN + 127

// 2.4.2 ƽ̨�豸��չ���ܲ��� CU��¼��� 
//��ѯ�������������: u16 CU���� + TTnmCuLogon����
#define NODE_CMU_PFMINFO_CU_LIST        KDMCONF_BGN + 130
//nms->agt��ȡCU��־����: u32(�ͻ����û�Session��ID)
//nms<-agt֪ͨCU��־����: TTnmCuLogItem
#define NODE_CMU_PFMINFO_CU_LOG         KDMCONF_BGN + 131

// 2.4.3 ��ѯƽ̨��ĳ��CU�û�����Ƶ���ͳ����Ϣ
#define NODE_CMU_CU_BROWSE_INFO         KDMCONF_BGN + 132

// 2.4.3 ƽ̨�豸��չ���ܲ��� PU��¼��� 
//��ѯ�������������: u16 PU���� + TTnmCmuPuInfo����
#define NODE_CMU_PFMINFO_PU_LIST        KDMCONF_BGN + 133

// 2.4.4 ��ȡƽ̨��ģ������״̬
// ��ѯ������������ݣ�u16 ģ�����(TNM_MAX_CMU_MODULE_NUM = 128) �� TTnmCmuModuleState����
#define NODE_CMU_MODULE_STATE           KDMCONF_BGN + 134

// 2.5. ƽ̨�豸VPU���ܲ���
//��ѯ�������������: TTnmVpuPfmInfo
#define NODE_CMU_VPUPFMINFO             KDMCONF_BGN + 135
//��ѯ������������ݣ�u16(ͨ������) + TTnmVpuRcvChnStatus����
#define NODE_CMU_VPUPFMINFOEX             KDMCONF_BGN + 136


// 2.6. NRU���ܲ���
// 2.6.1 ��������
//��ѯ�������������: TTnmNruPfmInfo
#define NODE_NRU_PFMINFO                KDMCONF_BGN + 137
// 2.6.2 ¼���������(��չ����) u16(������) + TTnmNruRecTask
#define NODE_NRU_REC_TASK               KDMCONF_BGN + 138
// 2.6.3 �����������(��չ����) u16(������) + TTnmNruPlayTask
#define NODE_NRU_PLAY_TASK              KDMCONF_BGN + 139
// 2.6.4 �����������(��չ����) u16(������) + TTnmNruDownLoadTask
#define NODE_NRU_DOWNLOAD_TASK          KDMCONF_BGN + 140
// 2.6.5 Ԥ¼�������(��չ����) u16(������) + TTnmNruPreRecTask
#define NODE_NRU_PREREC_TASK            KDMCONF_BGN + 141
// 2.6.6 ������չ����  u16(������)+TTnmNruDiskPfmInfo����
#define NODE_NRU_DISK_PFMINFO           KDMCONF_BGN + 142

// 2.7. 3AS���ܲ���
// 2.7.1 �������ܲ���
//��ѯ�������������: TTnm3asPfmInfo
#define NODE_3AS_PFMINFO                KDMCONF_BGN + 145
// 2.7.2 ��չ���ܲ���
//��ѯ�������������: u16(����Ϣ�������С) + TTnmLogonInfo����
#define NODE_3AS_PFMINFOEX              KDMCONF_BGN + 146

// 2.8. UAS���ܲ���
//��ѯ�������������: TTnmUasPfmInfo
#define NODE_UAS_PFMINFO                KDMCONF_BGN + 150

// 2.9. CUI���ܲ���
// �������ܲ���  TTnmCuiPfmInfo
#define NODE_CUI_PFMINFO                KDMCONF_BGN + 155
// ��չ���ܲ���  u16(����Ϣ�����������С) + TTnmCuiPfmInfoEx
#define NODE_CUI_PFMINFOEX              KDMCONF_BGN + 160

// 2.10. PUI���ܲ���
// �������ܲ���  TTnmPuiPfmInfo
#define NODE_PUI_PFMINFO                KDMCONF_BGN + 165
// ��չ���ܲ���  u16(����Ϣ��PU���������С) + TTnmPuiPfmInfoEx
#define NODE_PUI_PFMINFOEX              KDMCONF_BGN + 170

// 2.11 VDTU���ܲ���
// �������ܲ��� TTnmVtduPfmInfo
#define NODE_VTDU_PFMINFO               KDMCONF_BGN + 175
// ��չ���ܲ��� u16(����Ϣ�����������С��+ TTnmVtduTask����
#define NODE_VTDU_PFMINFOEX             KDMCONF_BGN + 180

//2.12 DEC5�������ܲ���
//�������ܲ��� TTnmDec5PfmInfo
#define NODE_DEC5_PFM_INFO              KDMCONF_BGN + 185


//landen�������ܲ���
//��ѯ�������������: TTnmLadenPfmInfo
#define NODE_LADEN_PFMINFO              KDMCONF_BGN + 190

//landen��չ���ܲ��� - PUI�б���Ϣ
//��ѯ�������������: u16(PUI����) + TTnmLadenPuiInfo����
#define NODE_LADEN_PFMINFO_PUI_LIST     KDMCONF_BGN + 191

//landen��չ���ܲ��� - CUI�б���Ϣ
//��ѯ�������������: u16(CUI����) + TTnmLadenCuiInfo����
#define NODE_LADEN_PFMINFO_CUI_LIST     KDMCONF_BGN + 192

// 3.1 �豸������Ϣ
// agent -> 3as(req)��ѯ��Ϣ��BUF����Ϊ��
// ��Ϊ��ЧIDʱ���Ҹ��ڵ��µ������豸
// agent <- 3as(notify):
// ���ӱ�: u32(ȫ���ڵ����Ŀ��) + u32(��ǰ��Ϣ��һ���ڵ�����)
//          + u16(������Ϣ����Ŀ��) + TTnmDevTopo����
// agent <- 3as(finish);��
#define NODE_3AS_TOPO_INFO              KDMCONF_BGN + 200
// 3.2 ���������豸
// agent <- 3as(notify)���ӱ�: u16(��Ŀ��) + TTnmDevTopo
#define NODE_3AS_TOPO_ADD_DEV           KDMCONF_BGN + 205
// 3.3 ɾ�������豸
// agent <- 3as(notify)���ӱ�: u16(��Ŀ��) + TTnmDevTopo
#define NODE_3AS_TOPO_DEL_DEV           KDMCONF_BGN + 210
// 3.3 ���������豸
// agent <- 3as(notify)���ӱ�: u16(��Ŀ��) + TTnmDevTopo
#define NODE_3AS_TOPO_UPDATE_DEV        KDMCONF_BGN + 215
// ������Ϣ�������ʱ���ͱ���Ϣ��ǰ������Ϣ�ţ�����������Ϣ����
// agent <- ǰ��(notify):u16(NODE��Ϣ��) + u16(����Ϣ����SUBEV_TYPE_FINISH)
#define NODE_EVSTATUS_NOTIFY            KDMCONF_BGN + 220

/********************************************************************/
/*                        ��  ��  ��  ��                            */
/********************************************************************/
#define KDMTRAP_BGN             EV_DEVAGT_NMS_BGN + 1000
#define KDMTRAP_END             KDMTRAP_BGN + 999
#define KDMTRAP_MAX_IPADDR_LEN  32+1
#define KDMTRAP_MAX_DEV_NO      32+1
// �澯��Ϣ�ϱ�
// ��Ϣ��:TTnmDevAlarm + �������Ϣ���ݡ����ݲ�ͬ����ĸ澯�룬�澯������
//      ������ͬ����������
//  1. TNM_ALARM_CODE_DEV_OFFLINE  �˸澯��NMS������ǰ���豸������
//      u8(�澯״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  2.  TNM_ALARM_CODE_DEV_ONLINE
//  3.  TNM_ALARM_CODE_RECV_NOBITSTREAM
//      u8(ͨ����) + u8(ͨ����״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  4.  TNM_ALARM_CODE_LOSEPACKET
//      u8(ͨ����) + u8(ͨ����״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  5.  TNM_ALARM_CODE_MAP_STATUS
//      u8(MAP��) + u8(MAP��״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  6.  TNM_ALARM_CODE_MOTIONDETECT_STATUS
//      u8(��ƵԴ��) + u8(��ƵԴ��״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  7.  TNM_ALARM_CODE_LPT_STATUS
//      u8(ͨ����) + u8(ͨ����״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  8.  TNM_ALARM_CODE_POWER_STATUS
//      u8(�Ǹ���Դ) + u8(��Դ��״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  9.  TNM_ALARM_CODE_CPU_STATUS
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  10. TNM_ALARM_CODE_MEMORY_STATUS
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  11. TNM_ALARM_CODE_DISKFULL_STATUS
//      u8(״̬���澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  12. TNM_ALARM_CODE_FAN_STATUS
//      u8(�Ǹ�����) + u8(����״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  13. TNM_ALARM_CODE_ETHCARD_RESTORE
//      u8(��̫��ID) + u8(��̫����״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  14. TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS
//      u8(��ƵԴ��) + u8(��ƵԴ��״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  15. TNM_ALARM_CODE_LED_STATUS
//      u8(���Ƹ���) + u8����(enum TnmLedStatus)
//  16. TNM_ALARM_CODE_SAME_REG_ID  �˸澯��NMS������ǰ���豸������
//      TTnmAgentRegReq + u8(�澯: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  17. TNM_ALARM_CODE_REG_CMU  PU������ģ��ע��CMUʧ�ܸ澯
//      u8(�澯(TNM_STATUS_ALARM,ע��ʧ��) �ָ�(TNM_STATUS_NORMAL��ע��ɹ�))
//  18. TNM_ALARM_CODE_DISKDETECT_ERROR    �˸澯���豸���� �豸����޴��̻�û�д��̿ɹ����澯
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL))
//  19. TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET        �˸澯��Cuͨ��CMU���͵�CUҵ��澯
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL)) + �澯����
//  20. TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION        �˸澯��Cuͨ��CMU���͵�CUҵ��澯
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL)) + �澯����
//  21  TNM_ALARM_CU_OVERLOAD  ��¼��CMU�ϵ�CU��������ֵ
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL)) + �澯����
//  22  TNM_ALARM_PU_OVERLOAD  ��¼��CMU�ϵ�PU��������ֵ
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL)) + �澯����
//  23  TNM_ALARM_VTDU_OVERLOAD  VTDU��ת��·��������ֵ
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL)) + �澯����
//  24  TNM_ALARM_CODE_DISCONNECT_VTDU  VTDUδ������
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL)) + 
//      s8 achSlaveIp[KDMTRAP_MAX_IPADDR_LEN](Vtdu Ip) +
//      s8 achSlaveId[KDMTRAP_MAX_DEV_NO](Vtdu Id)
//  25  TNM_ALARM_CODE_DISCONNECT_NRU  NRUδ������
//      u8(״̬: �澯(TNM_STATUS_ALARM) �ָ�(TNM_STATUS_NORMAL)) + 
//      s8 achSlaveIp[KDMTRAP_MAX_IPADDR_LEN](Nru Ip) +
//      s8 achSlaveId[KDMTRAP_MAX_DEV_NO](Nru Id)

#define TRAP_TNM_DEV_ALARM                  KDMTRAP_BGN

//�豸д���Trap��Ϣ, �豸�汾����ʱд���豸��ɺ󷢸����ܵ���Ϣ
//      ��Ϣ�壺u8(0:дʧ��  1:�ɹ�) + u8(0:������  1:����)
#define TRAP_SOFTWARE_COMPLETE              KDMTRAP_BGN+1

//�������ó�ͻ
#define TRAP_MASTER_NMS_EXIT_ALREADY        KDMTRAP_BGN+5

//���澯������Ϣ����
#define MAX_DESC_INFO_LEN  256

typedef struct tagAlarmTrapInfo
{
	tagAlarmTrapInfo()
	{	
		dwAgentID = 0;
		byParam = 0;
		byState = 0;
		wDescInfoLen = 0;
		wSubDevId = 0;
		memset(achDescInfo, 0, sizeof(achDescInfo));
	}
	void NetToHost();
	void HostToNet();
	TTnmDevAlarm tDevAlarm;   	//��ˮ�š�AlarmCode
	u32 dwAgentID;  			//���ڲ��ҵ��豸SN������豸��Ϣ
	u16 wSubDevId;
	u8  byParam;    			//���ںŻ��Դ�Ż���ȺŻ������Ż���ƵԴ�Ż�Ӳ�̷�����Ż�¼��ͨ����
	u8  byState;    			//�澯״̬
	u16 wDescInfoLen;			//�澯������Ϣ����
	s8 achDescInfo[MAX_DESC_INFO_LEN + 4];       //�澯������Ϣ
}PACKED TAlarmTrapInfo;

inline void TAlarmTrapInfo::NetToHost()
{
	tDevAlarm.NetToHost();
	wDescInfoLen = ntohs(wDescInfoLen);
	dwAgentID = ntohl(dwAgentID);
	wSubDevId = ntohs(wSubDevId);
}

inline void TAlarmTrapInfo::HostToNet()
{
	tDevAlarm.HostToNet();
	wDescInfoLen = htons(wDescInfoLen);
	dwAgentID = htonl(dwAgentID);
	wSubDevId = htons(wSubDevId);
}


/*========================================================================
�� �� ����GetAlarmCodeDesc
��    �ܣ����Խӿڣ��õ��澯�����������
��    ����u32 dwAlarmCode               [in]Ҫ�õ������ĸ澯��
�� �� ֵ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2006/03/30  3.9     ���ǿ  ����
========================================================================*/
inline const s8* GetAlarmCodeDesc(u32 dwAlarmCode)
{
    switch(dwAlarmCode)
    {
    case TNM_ALARM_CODE_DEV_OFFLINE:            return "DEV_OFFLINE";
    case TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS:    return "LOSEVIDEOSRC_STATUS";
    case TNM_ALARM_CODE_RECV_NOBITSTREAM:       return "RECV_NOBITSTREAM";
    case TNM_ALARM_CODE_MAP_STATUS:             return "MAP_STATUS";
    case TNM_ALARM_CODE_CPU_STATUS:             return "CPU_STATUS";
    case TNM_ALARM_CODE_MEMORY_STATUS:          return "MEMORY_STATUS";
    case TNM_ALARM_CODE_DISKFULL_STATUS:        return "DISKFULL_STATUS";
    case TNM_ALARM_CODE_LOSEPACKET:             return "LOSEPACKET";
    case TNM_ALARM_CODE_LPT_STATUS:             return "LPT_STATUS";
    case TNM_ALARM_CODE_POWER_STATUS:           return "POWER_STATUS";
    case TNM_ALARM_CODE_FAN_STATUS:             return "FAN_STATUS";
    case TNM_ALARM_CODE_MOTIONDETECT_STATUS:    return "MOTIONDETECT_STATUS";
    case TNM_ALARM_CODE_ETHCARD_RESTORE:        return "ETHCARD_RESTORE";
    case TNM_ALARM_CODE_LED_STATUS:             return "LED_STATUS";
    case TNM_ALARM_CODE_SAME_REG_ID:            return "SAME_REG_ID";
    case TNM_ALARM_CODE_REG_CMU:                return "REG_CMU";
    case TNM_ALARM_CODE_DISKDETECT_ERROR:       return "DISK_DETECT";
    case TNM_ALARM_CU_OVERLOAD:                 return "CU_OVERLOAD";
    case TNM_ALARM_PU_OVERLOAD:                 return "PU_OVERLOAD";
    case TNM_ALARM_VTDU_OVERLOAD:               return "VTDU_OVERLOAD";
    case TNM_ALARM_CODE_NODISK_ERROR:			   return "NODISK_ERROR";
	case TNM_ALARM_CODE_DISK_ERROR:				   return "DISK_ERROR";
	case TNM_ALARM_CODE_DISK_NOFORMAT:			   return "DISK_NOFORMAT";
	case TNM_ALARM_CODE_STOPREC_LOGERR:			   return "STOPREC_LOGERR";
	case TNM_ALARM_CODE_STOPREC_DISKERR:        return "STOPREC_DISKERR";
    case TNM_ALARM_CODE_DISCONNECT_3AS:         return "DISCONNECT_3AS";
    case TNM_ALARM_CODE_DISCONNECT_UAS:         return "DISCONNECT_UAS";
    case TNM_ALARM_CODE_DISCONNECT_PUI:         return "DISCONNECT_PUI";
    case TNM_ALARM_CODE_DISCONNECT_CUI:         return "DISCONNECT_CUI";
    case TNM_ALARM_CODE_DISCONNECT_NRU:         return "DISCONNECT_NRU";
    case TNM_ALARM_CODE_DISCONNECT_VTDU:        return "DISCONNECT_VTDU";
    case TNM_ALARM_CODE_DISCONNECT_CMU:         return "DISCONNECT_CMU";
    case TNM_ALARM_CODE_DISCONNECT_DATABASE:    return "DISCONNECT_DATABASE";
    case TNM_ALARM_CODE_DISCONNECT_USBKEY:      return "DISCONNECT_USBKEY";
    case TNM_ALARM_CODE_DISK_PART_R_ONLY:       return "DISK_PART_R_ONLY";
	case TNM_ALARM_CODE_DISK_FS_ERROR:			return "DISK_FS_ERROR";
	case TNM_ALARM_CODE_DISK_OFFLINE:			return "DISK_OFFLINE";
	case TNM_ALARM_CODE_STORAGE_ABNORMAL:		return "STORAGE_ABNORMAL";

	case TNM_ALARM_CODE_PLATFORM_IN_REPLACE_MODE: return "PLATFORM_IN_REPLACE_MODE";

	case TNM_ALARM_CODE_RAID_DISK_OFFLINE:		return "RAID_DISK_OFFLINE";
	case TNM_ALARM_CODE_RAID_DISK_HEALTH_BAD:	return "RAID_DISK_HEALTH_BAD";
	case TNM_ALARM_CODE_RAID_DISKGROUP_STATUS_ABNORMAL:	return "RAID_DISKGROUP_STATUS_ABNORMAL";
	case TNM_ALARM_CODE_RAID_ISCSI_DROPPED:		return "RAID_ISCSI_DROPPED";
	case TNM_ALARM_CODE_RAID_ABNORMAL:			return "RAID_ABNORMAL";
	case TNM_ALARM_CODE_DETECT_NULL_IP_NIC:     return "NULL_IP_NIC";
	case TNM_ALARM_CODE_DISCONNECT_SUPER_CMU:	return "TNM_ALARM_CODE_DISCONNECT_SUPER_CMU";
	case TNM_ALARM_CODE_DISCONNECT_SUB_CMU:		return "TNM_ALARM_CODE_DISCONNECT_SUB_CMU";
	case TNM_ALARM_CODE_DISCONNECT_BCMU_CMU:	return "TNM_ALARM_CODE_DISCONNECT_BCMU_CMU";
	case TNM_ALARM_CODE_PLATFORM_DISK_NUM_OVERLOAD:	return "TNM_ALARM_CODE_PLATFORM_DISK_NUM_OVERLOAD";

    default:
    	OspPrintf(TRUE,FALSE,"unknow alarmcode %lu\n",dwAlarmCode);
        break;
    }
    return "TNM_ALARM_CODE_INVALID";
}

#ifdef WIN32
#pragma pack(pop)
#endif //WIN32
/**
 * @}
 */
#endif  //!_SNMP_AGENTADP_H_


