//////////////////////////////////////////////////////////////////////////
// ipcdefine.h 
#ifndef IPCDEFINE_H_
#define IPCDEFINE_H_

/* ������ */
#define IPC_ERR_SUCCESS                     (0)    // �ɹ�����
#define IPC_ERR_INIT_OSP                    (-1)   // OSPͨ��ģ���ʼ��ʧ��
#define IPC_ERR_GET_LOC_IP                  (-2)   // ��ȡ����IPʧ��
#define IPC_ERR_CREATE_VSIP                 (-3)   // Э��ջ������󴴽�ʧ��
#define IPC_ERR_INIT_VSIP                   (-4)   // Э��ջ�����ʼ��ʧ��
#define IPC_ERR_GET_ADAPTER_INFO            (-5)   // ��ȡ����������Ϣʧ��
#define IPC_ERR_INIT_DATA_SWITCH            (-6)   // ����ת��ģ���ʼ��ʧ��
#define IPC_ERR_INVALID_PU_HANDLE           (-10)  // ��Ч��ǰ�˾��
#define IPC_ERR_GET_PU_CTRL                 (-11)  // ��ȡǰ�˿��ƶ���ʧ��
#define IPC_ERR_INIT_PU_CTRL                (-12)  // ��ʼ���豸����ʧ��(����ʱ)
#define IPC_ERR_INVALID_CON_LOC_IP          (-13)  // ���ӳɹ���Э��ջ���صı���IPΪ0
#define IPC_ERR_GET_ORIGIN_CAP              (-14)  // ��ȡԭʼ������ʧ��
#define IPC_ERR_GET_EXT_CAP                 (-15)  // ��ȡ��չ������ʧ��
#define IPC_ERR_GET_ADAPTER_CAP             (-16)  // ��ȡ������������ʧ��
#define IPC_ERR_GET_PU_BASE_INFO            (-17)  // ��ȡǰ�˻�����Ϣʧ��
#define IPC_ERR_INVALID_USER_PWD            (-18)  // ����ʱ���û����������
#define IPC_ERR_GET_PLAYER                  (-30)  // ��ȡ����������ʧ��
#define IPC_ERR_INVALID_VD_SRC_ID           (-31)  // ��Ч����ƵԴͨ����
#define IPC_ERR_INIT_DECODER                (-32)  // ��������ʼ��ʧ��
#define IPC_ERR_START_DECODE                (-33)  // ����Ƶ����ʧ��
#define IPC_ERR_GET_VD_ENC_PT_CFG           (-34)  // ��ȡ��Ƶ������ƵԴ�˿ڲ���ʧ��
#define IPC_ERR_INVALID_PLAY_WND            (-35)  // ��Ч�Ĳ��Ŵ��ھ��
#define IPC_ERR_GET_AD_DEC_NET_CFG          (-36)  // ��ȡ��Ƶ�����������ʧ��
#define IPC_ERR_START_DEC_RECV_EX           (-37)  // ��չ����Ƶ��������������ʧ��
#define IPC_ERR_INIT_ENCODER                (-38)  // ��������ʼ��ʧ��
#define IPC_ERR_AD_CALL_UNSUPPORTED         (-39)  // ǰ�˲�֧����������
#define IPC_ERR_PU_STOP_AD_RECV             (-40)  // ֹͣǰ����Ƶ����ʧ��
#define IPC_ERR_START_SEND_AUDIO            (-41)  // ��Ƶ���뷢��ʧ��
#define IPC_ERR_STOP_ENCODE                 (-42)  // ������ֹͣ��Ƶ����ʧ��
#define IPC_ERR_REC_WITH_NO_VD_PLAY         (-43)  // ��û����Ƶ���ŵ�����½��б���¼��
#define IPC_ERR_INIT_LOCAL_RECORDER         (-44)  // ����¼�����ʼ��ʧ��
#define IPC_ERR_START_LOCAL_REC             (-45)  // ��ʼ����¼��ʧ��
#define IPC_ERR_STOP_LOCAL_REC              (-46)  // ֹͣ����¼��ʧ��
#define IPC_ERR_SHOT_WITH_NO_VD_PLAY        (-47)  // ��û����Ƶ���ŵ�����½��б���ץ��
#define IPC_ERR_LOCAL_SNAPSHOT              (-48)  // ����ץ��ʧ��
#define IPC_ERR_LOCAL_RECV_PU_REC           (-49)  // ���ؽ���ǰ��¼��ʧ��
#define IPC_ERR_INVALID_REC_FILE_NAME       (-50)  // ���������ļ���Ϊ��
#define IPC_ERR_LOCAL_OPEN_FILE             (-51)  // ���ؽ��������ļ�ʧ��
#define IPC_ERR_LOCAL_PLAY_FILE             (-52)  // ���ؽ����������ļ�ʧ��
#define IPC_ERR_UPLOAD_TITLE_PIC            (-53)  // �ϴ�̨���ļ�ʧ��
#define IPC_ERR_ADD_VIDEO_SEND_DEST         (-54)  // ǰ�˱�������������
#define IPC_ERR_STREAM_EXCEED_LIMIT         (-55)  // ֱ��·������(��Ƶ�������)
#define IPC_ERR_PU_OVER_CAP                 (-56)  // ǰ��������֧�� 
#define IPC_ERR_CAMCTRL_UNSUPPORTED         (-60)  // ǰ���豸��֧�ִ�����ͷЭ��
#define IPC_ERR_SEND_CAMCTRL                (-61)  // ����ͷ���������ʧ��
#define IPC_ERR_PARSE_CAM_PROTOCOL          (-62)  // ����ͷЭ�����ʧ��
#define IPC_ERR_INVALID_PORT_NUM            (-63)  // ��Ч�Ķ˿ںŻ��ѱ�����Ӧ��ռ��
#define IPC_ERR_GET_PU_PARAM                (-80)  // ��ȡǰ�˲�������ʧ��
#define IPC_ERR_GET_PU_PARAM_EX             (-81)  // ��ȡ͸��ͨ����������ʧ��
#define IPC_ERR_INVALID_PARAM               (-82)  // ��Ч�����ò���
#define IPC_ERR_SET_PU_PARAM                (-83)  // ����ǰ�˲�������ʧ��
#define IPC_ERR_SET_PU_PARAM_EX             (-84)  // ����͸��ͨ����������ʧ��
#define IPC_ERR_SYNC_PU_TIME                (-85)  // ͬ��ǰ���豸ʱ��ʧ��
#define IPC_ERR_REBOOT_PU                   (-86)  // ǰ���豸����ʧ��
#define IPC_ERR_RESET_DEFAULTS              (-87)  // ǰ���豸�ָ�����ʧ��
#define IPC_ERR_LOAD_DEFAULT_CFG            (-88)  // �ָ�Ĭ������ʧ��(��Ҫ����ͼ��λ����)
#define IPC_ERR_START_PU_REC                (-89)  // ��ʼǰ��¼��ʧ��
#define IPC_ERR_STOP_PU_REC                 (-90)  // ֹͣǰ��¼��ʧ��
#define IPC_ERR_PU_SNAPSHOT                 (-91)  // ǰ��ץ��ʧ��
#define IPC_ERR_PU_SNAPSHOT_TIME_OUT        (-92)  // ǰ��ץ�ĳ�ʱ
#define IPC_ERR_SEARCH_PU_PIC               (-93)  // ǰ������ͼƬʧ��
#define IPC_ERR_DOWNLOAD_PU_PIC             (-94)  // ����ǰ��ͼƬʧ��
#define IPC_ERR_DELETE_PU_PIC               (-95)  // ǰ��ɾ��ͼƬʧ��
#define IPC_ERR_SEARCH_PU_REC               (-96)  // ǰ������¼���ļ�ʧ��
#define IPC_ERR_INVALID_PU_REC_INFO         (-97)  // ָ����ǰ��¼����ϢΪ��
#define IPC_ERR_START_PLAY_PU_REC           (-98)  // ǰ�˲���¼���ļ�ʧ��
#define IPC_ERR_PU_EXPORT_REC_FILE          (-99)  // ǰ�˵���¼���ļ�ʧ��
#define IPC_ERR_PU_STOP_EXPORT_REC_FILE     (-100) // ǰ��ֹͣ����¼���ļ�ʧ��
#define IPC_ERR_PU_DELETE_REC_FILE          (-101) // ǰ��ɾ��¼���ļ�ʧ��
#define IPC_ERR_PU_REC_PLAY_OPERATION       (-102) // ǰ��¼���ļ��طŲ���ʧ��
#define IPC_ERR_PU_REC_SEEK_LOCATION        (-103) // ǰ��¼�񲥷��϶�����ʧ��
#define IPC_ERR_ADD_DATA_SWITCH             (-104) // ����������Դת��ʧ��
#define IPC_ERR_REMOVE_ALL_DATA_SWITCH      (-105) // ɾ��ָ��Դ�İ�Դת��ʧ��
#define IPC_ERR_REMOVE_DATA_SWITCH          (-106) // ɾ��ָ��Ŀ�ĵİ�Դת��ʧ��

/* һ�㳣�� */
#define MAX_STR_LEN                  32   // ���������Ƴ���(32λ)
#define MAX_STR_LEN_64               64   // ���������Ƴ���(64λ)
#define MAX_VER_STRING_LEN           64   // ���汾�š����볤��
#define MAX_STRING_OSD_LEN           64   // �����Ļ���ݳ���
#define MAX_USER_NUMBER             256   // ��󲦺��û���/���볤��
#define MAX_SEPA_LEN                  8   // ���ָ�������
#define MAX_NAMERULE_NUM              4   // �����������
#define MAX_PATH_LENGTH             256   // ����ļ�·������
#define MAX_FILTERADDR_NUM		      5	  // �����˵�ַ����
#define MAX_VIDEOIN_PORT_NUMBER      32   // �����Ƶ����˿���
#define MAX_VIDEOIN_PORT_NM_LEN       8   // �����ƵԴ�˿�������
#define MAX_FILE_STREAM_NUM           3   // ���ط��ļ�����Ŀ
#define MAX_MOTION_DETECT            16   // �ƶ�����������
#define SM_DATA_BUFSIZE            1024   // ͼƬ�����ֽڳ���
#define MAX_TM_PRECISION              8   // ¼��ʱ�侫��
#define MAX_REC_FILE_NUM             20   // ���¼���ļ���Ŀ
#define MAX_PIC_FILE_NUM             20   // ���ץ���ļ���Ŀ
#define MAX_DISK_NUM                 16   // �����̷�����Ŀ
#define MAX_CHANNEL_NUM               4   // ���ͨ����Ŀ
#define MAX_VIDCHAN_NUM		         16	  // �������ͨ������
#define MAX_FILE_NAME_LENGTH         64   // ����ļ�����
#define MAX_FILE_PATH_LENGTH         64   // ����ļ�·������
#define MAX_DISK_PARTITION_NAME      64   // ��������С
#define MAX_CONNECTTED_PU			 16   // ���ͬʱ�����PU����
#define MAX_RECIPROCAL_LIST_SIZE	 30   // ֧�ֵĿ��Ų����б�
#define MAX_DAILYTIME_NUM             6   // ÿ�����ʱ��θ���
#define MAX_WEEKLYDAY_NUM             7   // ÿ������

/* ��Ƶ���ݸ�ʽ */
#define IPCSDK_VIDEO_TYPE_NONE					0x00
#define IPCSDK_VIDEO_TYPE_SN4					0x01
#define IPCSDK_VIDEO_TYPE_MPEG4					0x02
#define IPCSDK_VIDEO_TYPE_H261					0x03
#define IPCSDK_VIDEO_TYPE_H263					0x04
#define IPCSDK_VIDEO_TYPE_H264					0x05

/* ��Ƶ���ݸ�ʽ */
#define IPCSDK_AUDIO_TYPE_NONE					0x00
#define IPCSDK_AUDIO_TYPE_PCMA					0x01
#define IPCSDK_AUDIO_TYPE_PCMU					0x02
#define IPCSDK_AUDIO_TYPE_GSM					0x03
#define IPCSDK_AUDIO_TYPE_G729					0x04
#define IPCSDK_AUDIO_TYPE_ADPCM					0x05
#define IPCSDK_AUDIO_TYPE_G7221C				0x06
#define IPCSDK_AUDIO_TYPE_G722                  0x07
#define IPCSDK_AUDIO_TYPE_AACLC                 0x08

/* ͼ��ֱ��� */
#define IPCSDK_VIDEO_RESOLUTION_NONE			0x00
#define IPCSDK_VIDEO_RESOLUTION_AUTO			0x01
#define IPCSDK_VIDEO_RESOLUTION_QCIF			0x02
#define IPCSDK_VIDEO_RESOLUTION_CIF			    0x03
#define IPCSDK_VIDEO_RESOLUTION_2CIF			0x04
#define IPCSDK_VIDEO_RESOLUTION_4CIF			0x05
#define IPCSDK_VIDEO_RESOLUTION_QQCIF			0x06
#define IPCSDK_VIDEO_RESOLUTION_VGA			    0x07
#define IPCSDK_VIDEO_RESOLUTION_QVGA			0x08
#define IPCSDK_VIDEO_RESOLUTION_720P			0x09
#define IPCSDK_VIDEO_RESOLUTION_1080P			0x0A
#define IPCSDK_VIDEO_RESOLUTION_QXGA            0x0B

/* ��Ч���� */
#define IPCSDK_BITRATE_128						0x01
#define IPCSDK_BITRATE_256						0x02
#define IPCSDK_BITRATE_512						0x03
#define IPCSDK_BITRATE_768						0x04
#define IPCSDK_BITRATE_896						0x05
#define IPCSDK_BITRATE_1024						0x06
#define IPCSDK_BITRATE_2048						0x07
#define IPCSDK_BITRATE_3072						0x08
#define IPCSDK_BITRATE_4096						0x09
#define IPCSDK_BITRATE_8192						0x0A

/* ͼ������ */
#define IPCSDK_VIDEO_QUALITY_PRIOR				0x01 // ��������
#define IPCSDK_VIDEO_SPEED_PRIOR				0x02 // �ٶ�����

/* ͼƬ�������� */
#define IPCSDK_PIC_ENC_JPEG                        1 // jpeg��ʽ
#define IPCSDK_PIC_ENC_BMP                         2 // bmp��ʽ(�ݲ�֧��)

/* ������ʽ */
#define IPCSDK_SINGLE_SUBAREA			        0x01 // ������
#define IPCSDK_MULTI_SUBAREA			        0x02 // �����

/* ��Ƶ�������� */
#define IPCSDK_AUDIO_INPUTTYPE_LINEIN			0x00
#define IPCSDK_AUDIO_INPUTTYPE_MIC				0x01

/* ¼���ļ�/�¼�����/ͼƬ�¼� */
#define IPCSDK_QUIEY_ALL_EVENT					0    // ��ѯ�����¼�
#define IPCSDK_TYPE_PIN_ALARM					1    // ���ڸ澯¼��
#define IPCSDK_TYPE_MOTION_DETECT_ALARM			2    // �ƶ����澯¼��
#define IPCSDK_TYPE_MANUAL						3    // �ֶ�¼��
#define IPCSDK_TYPE_TIMELY						4    // ��ʱ¼��
#define IPCSDK_TYPE_PERIODLY					5    // ����¼��
#define IPCSDK_QUIEY_FILE						6    // ���ļ���ѯ

/* �����ļ����� */
#define IPCSDK_FILE_TYPE_ASF                    0    // asf�ļ�

/* ͼƬ��ϲ�ѯ */
#define IPCSDK_PIC_CHAN_QUERY					1	 // ͨ���Ų�ѯ
#define IPCSDK_PIC_TIME_QUERY					2	 // ʱ���ѯ
#define IPCSDK_PIC_EVENT_QUERY					4	 // �¼���ѯ

/* �������� */
#define IPCSDK_CONNECTION_NONE					0x00
#define IPCSDK_CONNECTION_UNICAST				0x01
#define IPCSDK_CONNECTION_MULTICAST				0x02
#define IPCSDK_CONNECTION_BROADCAST				0x03
#define IPCSDK_CONNECTION_TCPSERVER				0x04
#define IPCSDK_CONNECTION_TCPCLIENT				0x05
#define IPCSDK_CONNECTION_TCP					0x06
#define IPCSDK_CONNECTION_SSL					0x07
#define IPCSDK_CONNECTION_UDP					0x08

/* �������� */
#define IPCSDK_SERIAL_LINE_DRIVER_NONE			0x00
#define IPCSDK_SERIAL_LINE_DRIVER_RS232			0x01
#define IPCSDK_SERIAL_LINE_DRIVER_RS422			0x02
#define IPCSDK_SERIAL_LINE_DRIVER_RS485			0x03

/* ��Ƶ�������� */
#define IPCSDK_AUDIO_INPUTTYPE_LINEIN		    0x00
#define IPCSDK_AUDIO_INPUTTYPE_MIC		        0x01

/* ��������ͨ������ */
#define IPCSDK_SERIAL_STREAM_TYPE_NONE		    0x00 // ��ʹ��
#define IPCSDK_SERIAL_STREAM_TYPE_UDP		    0x01 // UDPͨ��
#define IPCSDK_SERIAL_STREAM_TYPE_TCP		    0x02 // TCPͨ��
#define IPCSDK_SERIAL_STREAM_TYPE_VSIP		    0x03 // ��Ϣͨ��

/* ��������Э������ */
#define IPCSDK_SERIAL_DATA_TYPE_NONE		    0x00 // ͸��ͨ��
#define IPCSDK_SERIAL_DATA_TYPE_VSIP		    0x01 // VSIPЭ��

/* ������������ */
#define IPCSDK_MEDIA_STREAM_TYPE_UDP		    0x00 // UDPͨ��
#define IPCSDK_MEDIA_STREAM_TYPE_TCP		    0x01 // TCPͨ��

#define IPCSDK_RELATE_ENCID_DISABLE				0xFF // �澯������Ƶ����ͨ����Чֵ
#define IPCSDK_OUTPUT_DISABLE					0xFF // �澯���������Чֵ

/* �澯������ */
#define IPCSDK_PINALMCHK_NONE			  0	 // �����
#define	IPCSDK_PINALMCHK_TIMER			  1	 // ��ʱ���
#define IPCSDK_PINALMCHK_WEEKLY			  2	 // ���ڼ��
#define IPCSDK_PINALMCHK_ALL			  3	 // ʼ�ռ��

/* ���ڸ澯ģʽ */
#define IPCSDK_PINALARM_INVALID			  0	 // ��Чģʽ
#define IPCSDK_PINALARM_HIGHVOL			  1	 // �ߵ�ƽģʽ(����)
#define IPCSDK_PINALARM_LOWVOL			  2  // �͵�ƽģʽ(����)
#define IPCSDK_PINALARM_CLOSE			  3	 // �պ�ģʽ(���)
#define IPCSDK_PINALARM_OPEN			  4	 // ��ģʽ(���)

/* �澯�������� */
#define IPCSDK_LNK_VIDSRCSWT_SUPPORT      1  // ֧����ƵԴ�л�
#define IPCSDK_LNK_CAMPOSSWT_SUPPORT      2  // ֧������ͷԤ��λ�л�
#define IPCSDK_LNK_SNAPSHOT_SUPPORT       4  // ֧��ץ��
#define IPCSDK_LNK_LOCALREC_SUPPORT       8  // ֧��ǰ��¼��
#define IPCSDK_LNK_ALARMTIME_SUPPORT     16  // ֧�ָ澯��ʱ
#define IPCSDK_LNK_OSDSHOW_SUPPORT       32  // ֧��̨����ʾ
#define IPCSDK_LNK_PINOUTPUT_SUPPORT     64  // ֧�ֲ������
#define IPCSDK_LNK_AUDIOOUT_SUPPORT     128  // ֧�ָ澯��Ƶ���
#define IPCSDK_LNK_CONVIDSRC_SUPPORT    256  // ֧�ֹ�����ƵԴ

/* ����¼����� */
#define IPCSDK_NONE_PERIOD                      0    // ������¼��
#define IPCSDK_PERIOD_DAYLY                     1    // ÿ��¼��
#define IPCSDK_PERIOD_SELECTION                 2    // ����¼��

/* �ļ����ǲ��� */
#define IPCSDK_STRATEGY_STOP_REC                0    // ֹͣ¼��
#define IPCSDK_STRATEGY_COVER_ALL_REC           1    // �������м�¼
#define IPCSDK_STRATEGY_COVER_REC_EXCEPT_ALARM  2    // ���ǳ��澯¼����ļ�¼

/* �������� */
#define SNAPSHOT_NAME_INVALID		0
#define SNAPSHOT_NAME_DATETIME		1	// ����ʱ��
#define SNAPSHOT_NAME_DEVALIAS		2	// �豸����
#define SNAPSHOT_NAME_VIDSRCNAME	3	// ��ƵԴ��
#define SNAPSHOT_NAME_ALARMTYPE		4	// �澯����
#define SNAPSHOT_NAME_USERDEFINE	5	// �û��Զ���

/* ��Ļ���� */
#define IPCSDK_CAPOSDCOLOR_16		1
#define IPCSDK_CAPOSDCOLOR_256		2
#define IPCSDK_CAPOSDCOLOR_FONT		4	// ��������Ӧ

/* ��Ļ��ض��� */
#define IPCSDK_OSDUSE_DEFAULT		0	  // Ĭ�Ϸ�ʽ
#define IPCSDK_OSDUSE_USERDEF		1	  // �û��Զ��巽ʽ
#define IPCSDK_OSDPICTYPE_INVALID	0	  // ��Ч����
#define IPCSDK_OSDPICTYPE_LABEL		1	  // ����̨����ʾ
#define IPCSDK_OSDPICTYPE_PINALARM	2	  // ���ڲ��ڸ澯��ʾ
#define IPCSDK_OSDPICTYPE_NORECV	3	  // �����޽���������Ļ��ʾ
#define IPCSDK_OSDPICTYPE_ALMHINT	4	  // ���ڸ澯��ʾ��ʾ
#define IPCSDK_OSDPICTYPE_MMALARM	5	  // �ƶ����澯
#define IPCSDK_OSDPICTYPE_ALMTIME	6	  // ���ڸ澯��ʱ��ʾ
#define IPCSDK_OSDPICTYPE_EXALARM	7	  // �ⲿ�澯��ʾ
#define IPCSDK_OSDPICTYPE_RECORD    8     // ¼����ʾ

/* ̨����Ļ���� */
#define	IPCSDK_OSD_LABEL0		0		// ��ƵԴ��
#define	IPCSDK_OSD_LABEL1		1		// ��ƵԴ��
#define	IPCSDK_OSD_LABEL2		2		// ��ƵԴ��
#define	IPCSDK_OSD_LABEL3		3		// ��ƵԴ��

#define IPCSDK_ALARM_LABEL0    10       //�澯��Ļ
#define IPCSDK_ALARM_LABEL1    11       //�澯��Ļ
#define IPCSDK_ALARM_LABEL2    12       //�澯��Ļ
#define IPCSDK_ALARM_LABEL3    13       //�澯��Ļ

/* ͼƬ��ʽ */
#define IPCSDK_PIC_DAT		0
#define IPCSDK_PIC_JPG		10

/* ǰ�˴洢start */
#define PARAM_TYPE_TIMELY_INFO              0x18 // ��ʱ¼����Ϣ
#define PARAM_TYPE_PERIODLY_INFO            0x19 // ����¼����Ϣ
#define PARAM_TYPE_CHANNEL_BASIC_INFO       0x20 // ͨ��������Ϣ
#define PARAM_TYPE_REC_SYS_INFO             0x21 // ϵͳ��Ϣ
#define PARAM_NMS_INFO                      0x22 // ���ܲ���

/* ǰ�˴洢end */
#define PARAM_SHADE_INFO                    0x23 // ͼ�����β���
#define PARAM_XIANTELECOM_ALARM_INFO        0X24 // �澯����
#define PARAM_XIANTELECOM_BANNER_INFO       0X25 // �������
#define PARAM_PIC_CONFIG_INFO				0X26 // ͼƬ��Ϣ
#define PARAM_MATRIX_CONFIG_INFO			0x27 // ������Ϣ
#define PARAM_PIC_NAMERULE_INFO				0x28 // ץ����Ϣ
#define PARAM_PUNMS_INFO					0x29 // ������ز���
#define PARAM_PINEXT_INFO					0x31 // ������չ������

/* ����ͷ���� */
#define IPC_CAMERA_TYPE_SONY				1   // SONY
#define IPC_CAMERA_TYPE_AD					2   // AD
#define IPC_CAMERA_TYPE_PELCO				3   // PELCO
#define IPC_CAMERA_TYPE_PIH					4   // ����
#define IPC_CAMERA_TYPE_PELCO_P				5   // PELCO-P
#define IPC_CAMERA_TYPE_PARCO		        6   // PARCO�ظ�
#define IPC_CAMERA_TYPE_AB					7	// AB 
#define IPC_CAMERA_TYPE_YUNTAI			    8   // ������̨
#define IPC_CAMERA_TYPE_SAMSUNG				9   // Samsung����
#define IPC_CAMERA_TYPE_YAAN			    10  // YUNTAI�ǰ�
#define IPC_CAMERA_TYPE_AD168				11	// AD168
#define IPC_CAMERA_TYPE_KALATEL				12	// KALATEL������
#define IPC_CAMERA_TYPE_JN					13  // JNΰ��
#define IPC_CAMERA_TYPE_WV_CS650			14	// ����CS650
#define IPC_CAMERA_TYPE_WV_CS850			15	// ����CS850
#define IPC_CAMERA_TYPE_ALEC				16	// ALEC������
#define IPC_CAMERA_TYPE_AB_P				17	// AB-P
#define IPC_CAMERA_TYPE_AB_D				18	// AB-D
#define IPC_CAMERA_TYPE_PELCO_D_JEC			19	// JEC PELCO-D
#define IPC_CAMERA_TYPE_PELCO_P_JEC			20	// JEC PELCO-P
#define IPC_CAMERA_TYPE_ROBOT				21  // ROBOT
#define IPC_CAMERA_TYPE_KDM2130_PELCO_P		22  // KDM2130����ͷ����Э��
#define IPC_CAMEAR_TYPE_PELCO_D_K			26  // PELCO_D_K

/* ����ͷ�������� */
#define IPC_CAM_NAME_SONY              	"SONY"
#define IPC_CAM_NAME_AD                	"AD"
#define IPC_CAM_NAME_PELCO             	"PELCO-D"
#define IPC_CAM_NAME_PIH                "����"
#define IPC_CAM_NAME_PELCO_P           	"PELCO-P"
#define IPC_CAM_NAME_PARCO             	"PARCO�ظ�"
#define IPC_CAM_NAME_AB                	"AB"
#define IPC_CAM_NAME_YUNTAI           	"������̨"
#define IPC_CAM_NAME_Samsung           	"Samsung����"
#define IPC_CAM_NAME_YAAN             	"YUNTAI�ǰ�"
#define IPC_CAM_NAME_AD168             	"AD168"
#define IPC_CAM_NAME_KALATEL          	"KALATEL������"
#define IPC_CAM_NAME_JN                 "JNΰ��"
#define IPC_CAM_NAME_WV_CS650         	"����CS650"
#define IPC_CAM_NAME_WV_CS850         	"����CS850"
#define IPC_CAM_NAME_ALEC              	"ALEC������"d
#define IPC_CAM_NAME_AB_P              	"AB-P"
#define IPC_CAM_NAME_AB_D              	"AB-D"
#define IPC_CAM_NAME_PELCO_D_JEC      	"JEC PELCO-D"
#define IPC_CAM_NAME_PELCO_P_JEC      	"JEC PELCO-P"
#define IPC_CAM_NAME_TIANDY		   	    "TIANDY"
#define IPC_CAM_NAME_MATRIX           	"ͨ�þ���Э��"
#define IPC_CAM_NAME_ROBOT            	"ROBOT ����ϳɾ���"

/* ��Ƶ����˿����� */
#define IPC_VIDEO_OUT_TYPE_NONE			    0x00 // ��
#define IPC_VIDEO_OUT_TYPE_C			    0x01 // ����C����
#define IPC_VIDEO_OUT_TYPE_VGA			    0x02 // VGA���
#define IPC_VIDEO_OUT_TYPE_HDMI			    0x04 // HDMI
#define IPC_VIDEO_OUT_TYPE_BNCandVGA	    0X08 // C adn VGA
#define IPC_VIDEO_OUT_TYPE_BNCandHDMI	    0x10 // C and HDMI

/* VGA���ˢ��Ƶ�� */
#define IPC_VGA_OUT_FREQUENCY_60			0x01 // ˢ��Ƶ��60
#define IPC_VGA_OUT_FREQUENCY_75			0x02 // ˢ��Ƶ��75
#define IPC_VGA_OUT_FREQUENCY_85			0x03 // ˢ��Ƶ��85

/* �豸��������� */
#define IPC_NONE		                  0	// ��Ƶ����붼��֧��
#define IPC_ENCODER		                  1	// ֻ֧����Ƶ����
#define IPC_DECODER		                  2	// ֻ֧����Ƶ����
#define IPC_BOTH		                  3	// ��Ƶ����붼֧��
#define IPC_SMARTSIGHT_ENCODER            4 // SMARTSIGHT��Ƶ����֧��
#define IPC_SMARTSIGHT_DECODER            5 // SMARTSIGHT��Ƶ����֧��
#define IPC_SMARTSIGHT_BOTH               6 // SMARTSIGHT��Ƶ����붼֧��

/* Э������ */
#define IPC_STACK_INVALID				(BYTE)0 // ��ЧЭ������
#define IPC_STACK_VSIP					(BYTE)1 // VSIPЭ��
#define IPC_STACK_GEP					(BYTE)2 // ȫ����Э��
#define IPC_STACK_OSP					(BYTE)3 // OSPЭ��

/* ����Ĭ�ϲ������� */
#define IPCSDK_DEFCFG_PUID					(LPCSTR)"009900000000000000000100000000  "	// puid or pupwd
#define IPCSDK_DEFCFG_IPADDR				(unsigned long)(inet_addr("192.168.0.99"))	// ����IP
#define IPCSDK_DEFCFG_NETMASK				(unsigned long)(inet_addr("255.255.255.0"))	// ������������
#define IPCSDK_DEFCFG_GATEWAY				0								            // ��������
#define IPCSDK_DEFCFG_STARTPORT				60000							            // ��ʼ�˿�
#define IPCSDK_DEFCFG_TIMEZONE				(char)8								        // ϵͳʱ��
#define IPCSDK_DEFCFG_REGPFIP				(LPCSTR)"224.1.1.1"					        // ע��ƽ̨��ַ
#define IPCSDK_DEFCFG_REGPFPORT				5510							            // ע��ƽ̨�˿�
#define IPCSDK_DEFCFG_VSIPMULIP				(LPCSTR)"224.16.32.1"				        // Э��ջ�����ಥ��ַ
#define IPCSDK_DEFCFG_PPPOE_NAME            (LPCSTR)"root"                              // PPPoE�û���
#define	IPCSDK_DEFCFG_PPPOE_PASSWORD        (LPCSTR)"root"                              // PPPoE����
#define IPCSDK_DEFCFG_PPPOE_SERVERNAME      (LPCSTR)"SERVER_NAME"                       // PPPoE��������         
#define IPCSDK_DEFCFG_SYNTIMEINT			10		                                    // ͬ��ʱ����(����)
#define IPCSDK_DEFCFG_AUTODIALINT			1		                                    // �Զ����ż��(��)
#define IPCSDK_DEFCFG_DIALRETRYTIMES		3		                                    // �������Դ���(��)
#define IPCSDK_DEFCFG_LESNDINT				30		                                    // LP���(��)
#define IPCSDK_DEFCFG_LERETRYTIMES			3		                                    // LP���Դ���(��)
#define IPCSDK_DEFCFG_CPUTHD				95		                                    // cpu������ֵ
#define IPCSDK_DEFCFG_MEMORYTHD				95		                                    // �ڴ汨����ֵ
#define IPCSDK_DEFCFG_VIDLOSTTHD			95		                                    // ��Ƶ���ն���������ֵ
#define IPCSDK_DEFCFG_SNMPNATINT			10		                                    // ����̽������ͼ��
#define IPCSDK_DEFCFG_TRAPRESNDNUM			10		                                    // trap�ط�����
#define IPCSDK_DEFCFG_TRAPRESNDINT			10		                                    // trap�ط����
#define IPCSDK_DEFCFG_VIDBRT				120		                                    // ����
#define IPCSDK_DEFCFG_VIDCTS				120		                                    // �Աȶ�
#define IPCSDK_DEFCFG_VIDSTT				150		                                    // ���Ͷ�
#define IPCSDK_DEFCFG_VIDRESNDTIME			200		                                    // ��Ƶ�����ش����(����)
#define IPCSDK_DEFCFG_VIDSRCPOLLINT			10		                                    // ��ƵԴ��ѯʱ����(��)
#define IPCSDK_DEFCFG_VIDBITRATE			800	                                        // ��Ƶ��������
#define IPCSDK_DEFCFG_VIDMAXKFINT			75		                                    // ��Ƶ���ؼ�֡���
#define IPCSDK_DEFCFG_VIDMP4MAXQ			31		                                    // mp4�����������
#define IPCSDK_DEFCFG_VIDMP4MINQ			3		                                    // mp4��С��������
#define IPCSDK_DEFCFG_VIDH264MAXQ			45		                                    // h264�����������
#define IPCSDK_DEFCFG_VIDH264MINQ			20		                                    // h264��С��������
#define IPCSDK_DEFCFG_VIDFRMRATE			25		                                    // ��Ƶ֡���
#define IPCSDK_DEFCFG_VIDRS1TS				40		                                    // ��Ƶ���յ�һ�ش���
#define IPCSDK_DEFCFG_VIDRS2TS				80		                                    // ��Ƶ���յڶ��ش���
#define IPCSDK_DEFCFG_VIDRS3TS				160	                                        // ��Ƶ���յ����ش���
#define IPCSDK_DEFCFG_VIDRSRTS				200	                                        // ��Ƶ���չ���ʱ��
#define IPCSDK_DEFCFG_AUDIOVOL				12		                                    // ����
#define IPCSDK_DEFCFG_OSDXPOS				50		                                    // osd��ʼλ��x
#define IPCSDK_DEFCFG_OSDYPOS				50		                                    // osd��ʼλ��y
#define IPCSDK_DEFCFG_RECSFILE				2		                                    // ¼��С�ļ�ʱ����(����)
#define IPCSDK_DEFCFG_RECFREESIZE			50		                                    // ����ʣ��ռ�MB
#define IPCSDK_DEFCFG_RECSYSFREESIZE		350	                                        // ϵͳ����ʣ��ռ�MB
#define IPCSDK_DEFCFG_RECPARTMINSIZE		50		                                    // ��¼�����С������СMB
#define IPCSDK_DEFCFG_DLSPEEDLIMIT 		    1024	                                    // ¼�������ٶ�KB
#define IPCSDK_DEFCFG_ALMRESUMETIME			5		                                    // �澯�ָ�ʱ��
#define IPCSDK_DEFCFG_DETSENSE				80		                                    // �ƶ�֡��������(%)
#define IPCSDK_DEFCFG_DETALMRATE			10		                                    // �ƶ�֡��澯������ֵ(%)
#define IPCSDK_DEFCFG_BAUDRATE				9600	                                    // ���ڲ�����
#define IPCSDK_DEFCFG_RECIKEYINT			5		                                    // ¼��ؼ�֡���(��)
#define IPCSDK_DEFCFG_OSDCLRID				145		                                    // Ĭ����ɫID -->��ɫ*/
#define IPCSDK_DEFCFG_TESTSERVERIP			(LPCSTR)"2.1.1.1"				            // ���Է�����IP��ַ
#define IPCSDK_DEFCFG_DOWNLOADPORT			1730	                                    // ¼�����ط���˿�
#define IPCSDK_DEFCFG_DEFAULTPOSINFO        (LPCSTR)"�豸λ����Ϣ"                      // �豸Ĭ��λ����Ϣ
#define IPCSDK_DEFCFG_CFGNULL               (LPCSTR)"NuLL"                              // ����Ϊ��ʱ�����ֵ

/* ����ͷ���� */
typedef enum ECamControl
{	
	CAM_FOCUSAUTO = 1,                      // �Զ��Խ�
	CAM_FOCUSFAR,                           // �ֶ��Խ�(��Զ)
	CAM_FOCUSNEAR,                          // �ֶ��Խ�(�ƽ�)
  	CAM_FOCUSSTOP,                          // ֹͣ�Խ�
	CAM_MOVEUP,                             // ����
	CAM_MOVEDOWN,                           // ����
	CAM_MOVEHOME,                           // ��λ
	CAM_MOVELEFT,	                        // ����
	CAM_MOVERIGHT,                          // ����
	CAM_MOVESTOP,	                        // ֹͣ
	CAM_ZOOMWIDE,                           // ��Ұ�Ŵ�
	CAM_ZOOMTELE,                           // ��Ұ��С
	CAM_ZOOMSTOP,	                        // ֹͣ����
	CAM_PRESETSET,	                        // ����Ԥ��λ
	CAM_PRESETCALL,                         // ��ȡԤ��λ
	CAM_PRESETREMOVE,                       // ���Ԥ��λ
	CAM_BRIGHTUP,                           // �������(��Ȧ)
	CAM_BRIGHTDOWN,                         // �������(��Ȧ)
	CAM_BRIGHTSTOP,                         // ֹͣ����(��Ȧ)
	CAM_TOURSTART,                          // ��ʼԤ��λѲ��  
	CAM_TOUREND,                            // ����Ԥ��λѲ��
	CAM_STARTAUTOSCAN,                      // ��ʼˮƽѲ��
	CAM_STOPAUTOSCAN,                       // ֹͣˮƽѲ��
	CAM_SCANSPEED_SET,                      // ����ˮƽɨ���ٶ�
	CAM_GOTOPT,                             // ���Ķ�λ
	CAM_ZOOMPT,                             // �ֲ��Ŵ�
	CAM_ZMWHOLE,                            // �ֲ���С
	CAM_DEPTHRATESPEED_ON,                  // ������������ٶ�
	CAM_DEPTHRATESPEED_OFF,                 // �رվ�������ٶ�
	CAM_WIPER_ON,                           // ��ˢ����
	CAM_WIPER_OFF,                          // ��ˢ�ر�   
	CAM_IR_ON,                              // ���⿪�� 
	CAM_IR_OFF,                             // ����ر�
	CAM_DEFROST_ON,                         // ��˪����                          
	CAM_DEFROST_OFF,                        // ��˪�ر�
	CAM_LASER_ON,                           // ���⿪��
	CAM_LASER_OFF,                          // ����ر�
	CAM_RESTORE_FACTORY_SETTINGS,           // �ָ���������
	CAM_DAYNIGHTMOD_ON,                     // ��ҹģʽ��
	CAM_DAYNIGHTMOD_OFF,                    // ��ҹģʽ��
	CAM_DAYNIGHTMOD_AUTO,                   // ��ҹģʽ�Զ�
	CAM_WIDEDYNAMIC_ON,                     // ��̬��
	CAM_WIDEDYNAMIC_OFF,                    // ��̬��
	CAM_BACKLIGHT_ON,                       // ���ⲹ����
	CAM_BACKLIGHT_OFF,                      // ���ⲹ����
	CAM_WHITEBALANCEMODE_AUTO,              // ��ƽ��ģʽ�Զ�
	CAM_WHITEBALANCEMODE_MANUAL,            // ��ƽ��ģʽ�ֶ�
	CAM_ALL_AUTO,                           // ��оȫ�Զ�
	CAM_SHUTTERMODE_AUTO,                   // ����ģʽ�Զ�
	CAM_SHUTTERMODE_MANUAL,                 // ����ģʽ�ֶ�
	CAM_PLUSMODE_AUTO,                      // ����ģʽ�Զ�
	CAM_PLUSMODE_MANUAL,                    // ����ģʽ�ֶ�
	CAM_APERTUREMODE_AUTO,                  // ��Ȧģʽ�Զ�     
	CAM_APERTUREMODE_MANUAL_HIGHSPEED,      // ��Ȧģʽ�ֶ�����(��)
	CAM_APERTUREMODE_MANUAL_LOWSPEED,       // ��Ȧģʽ�ֶ�����(С)
	CAM_MENU_SHOW,		                    // �˵���ʾ
	CAM_MENU_HIDE,		                    // �˵�����
	CAM_MENU_ENTER,		                    // �˵�ȷ��
	CAM_MENU_EXIT,		                    // �˵�ȡ��
	CAM_IMAGE_SHOW,		                    // ͼ������ʾ
	CAM_IMAGE_ANTISHOW,	                    // ͼ����ʾ
	CAM_MACHINE_ZERO,                       // ��λУ�� 
	CAM_PANPOSION_QUERY,                    // ��λ����ѯˮƽλ��
	CAM_TILTPOSION_QUERY,                   // ��λ����ѯ����λ��
	CAM_ZOOMPOSION_QUERY,                   // ��λ����ѯ�䱶ֵ
	CAM_PANPOSION_SET,                      // ��λ������ˮƽλ��
	CAM_TILTPOSION_SET,                     // ��λ�����ø���λ��
	CAM_ZOOMPOSION_SET,                     // ��λ�����ñ䱶ֵ
    CAM_FOCUS_SWITCH_TO_MANUAL,             // ֹͣ�Զ��Խ����л�Ϊ�ֶ��Խ�
	CAM_MOVE_LEFTUP,                        // ������
	CAM_MOVE_LEFTDOWN,                      // ������
	CAM_MOVE_RIGHTUP,	                    // ������
	CAM_MOVE_RIGHTDOWN,                     // ������
	CAM_NOOP			                    // �޲���
};

/* �����������ͼ���Ӧ�ṹ */
typedef enum EParamType
{
	IPCSDK_PARAM_PU_BASE = 0,			// ǰ�˻�������           TIPC_PU_BASE_PARAM
	IPCSDK_PARAM_SYS_PUB,				// ϵͳ��������           TIPC_SYS_PUB_PARAM
	IPCSDK_PARAM_SYS_ALL,               // ϵͳ����(ȫ��)         TIPC_SYS_CFG_PARAM
	IPCSDK_PARAM_LOCAL_NET_CFG,         // �����������ò���       TIPC_LOCAL_NET_CFG
	IPCSDK_PARAM_VIDEO_ENC_NET,			// ��Ƶ�����������       TIPC_VIDEC_ENC_NET_PARAM
	IPCSDK_PARAM_VIDEO_ENC,				// ��Ƶ�������	          TIPC_VIDEO_ENC_PARAM
	IPCSDK_PARAM_VIDEO_ENC_PUB,			// ��Ƶ���빫������       TIPC_VIDEO_ENC_PUB_PARAM
	IPCSDK_PARAM_VIDEO_ENC_OSD,			// ��Ƶ������Ļ����       TIPC_VIDEO_ENC_OSD_PARAM
	IPCSDK_PARAM_VIDEO_ENC_INPUTPORT,	// ��Ƶ������ƵԴ�˿ڲ��� TIPC_VIDEO_ENC_INPUT_PT_PARAM
	IPCSDK_PARAM_VIDEO_DEC_NET,			// ��Ƶ�����������       TIPC_VIDEO_DEC_NET_PARAM
	IPCSDK_PARAM_VIDEO_DEC,				// ��Ƶ���������         TIPC_VIDEO_DEC_PARAM
	IPCSDK_PARAM_VIDEO_DEC_PUB,			// ��Ƶ���빫������       TIPC_VIDEO_DEC_PUB_PARAM
	IPCSDK_PARAM_AUDIO_ENC_NET,			// ��Ƶ�����������       TIPC_AUDIO_ENC_NET_PARAM
	IPCSDK_PARAM_AUDIO_ENC,				// ��Ƶ�������           TIPC_AUDIO_ENC_PARAM
	IPCSDK_PARAM_AUDIO_DEC_NET,			// ��Ƶ�����������       TIPC_AUDIO_DEC_NET_PARAM
	IPCSDK_PARAM_AUDIO_DEC,				// ��Ƶ�������           TIPC_AUDIO_DEC_PARAM
	IPCSDK_PARAM_SERIAL_PORT,			// ���ڲ���               TIPC_SERIAL_PT_PARAM
	IPCSDK_PARAM_SERIAL_PORT_PUB,		// ���ڹ�������           TIPC_SERIAL_PT_PUB_PARAM
	IPCSDK_PARAM_INPUT_PIN,				// �����������           TIPC_INPUT_PIN_PARAM
	IPCSDK_PARAM_VIDEO_INPUT,			// ��ƵԴ�˿ڲ���         TIPC_VIDEO_INPUT_PARAM
	IPCSDK_PARAM_NMS_INFO,				// ���ܲ���               TIPC_NMS_PARAM
	IPCSDK_PARAM_PU_NMS_CFG,			// ����������ò���       TIPC_PU_NMS_CONFIG
	IPCSDK_PARAM_EN_MOTION_DETECT,		// �����ƶ�������       TIPC_MOTION_DETECT
	IPCSDK_PARAM_VIDEO_SHADE,           // ͼ���ڱο��Ʋ���       TIPC_VIDEO_SHADE_PARAM
	IPCSDK_PARAM_PPPOE,				    // PPPOE���Ų���          TIPC_PPPOE_PARAM
	IPCSDK_PARAM_VIDEO_DEC_RETRANSFER,	// ��Ƶ�����ش�����       TIPC_VIDEO_DEC_RETRANSFER_PARAM
	IPCSDK_PARAM_REC_SYS_INFO,			// ¼��ϵͳ����           TIPC_REC_SYS_INFO
	IPCSDK_PARAM_REC_BASIC_INFO,		// ¼���������           TIPC_REC_CHANNEL_BASIC_INFO
	IPCSDK_PARAM_TIMELY_REC,			// ��ʱ¼����Ϣ           TIPC_TIMELY_REC_INFO
	IPCSDK_PARAM_PERIOD_REC,			// ����¼����Ϣ           TIPC_PERIODLY_TIME_INFO
	IPCSDK_PARAM_SNAPSHOT_BASIC_INFO,	// ץ�Ļ�������           TIPC_SNAPSHOT_CONFIG
	IPCSDK_PARAM_SNAPSHOT_NAME_RULE,	// ץ����������           TIPC_SNAPSHOT_NAMING_RULE
	IPCSDK_PARAM_SNAPSHOT_EXT_CFG,      // ���Ų���               TIPC_SNAPSHOT_EXT_CFG
	IPCSDK_PARAM_AGC_INFO,              // �Զ��������(���ֵ)   TIPC_AGC_INFO
	IPCSDK_PARAM_AUTOSHUT_INFO,         // �Զ����Ų���(��ֵ)     TIPC_AUTOSHUT_INFO
	IPCSDK_PARAM_BNC_INFO,              // ���ػ��Բ���           TIPC_BNC_INFO
	IPCSDK_PARAM_WHITE_BALANCE,         // ��ƽ�����             TIPC_WHITE_BALANCE
	IPCSDK_PARAM_SD_CARD_INFO,          // SD����������           TIPC_SD_CARD_INFO
	IPCSDK_PARAM_FORMAT_SD_CARD,        // ��ʽ��SD������         TIPC_FORMAT_SD_CARD
	IPCSDK_PARAM_MULTI_CHAN_INFO,       // ˫������               TIPC_MULTI_CHAN_INFO
	IPCSDK_PARAM_PTZCTRL_INFO           // ��̨������ز���       TIPC_PTZ_CTRL_INFO
};

/* �����Ƶ���������� */
typedef enum EPlayChanType
{
    PLAY_MAIN = 0,
	PLAY_SUB  = 1,
};

/* ����¼��״̬(¼��ص���ʹ��) */
typedef enum ElLocRecordingStat
{
    LOCAL_REC_START = 1,
	LOCAL_REC_STOP,
	LOCAL_REC_FILE_START ,
	LOCAL_REC_START_TIME,
	LOCAL_PREREC_START,
	LOCAL_PREREC_STOP,
	LOCAL_STREAM_STOP,        
};

/* ����¼��ط�״̬ */
typedef enum ElLocRecPlayStat
{
	LOCAL_PLAYBACK_STOP  = 0,
	LOCAL_PLAYBACK_START = 1,
};

/* ǰ��¼��״̬ */
typedef enum EPuRecordingStat
{
	eAll,				    // ��ȡ����״̬
	eManualRec,		        // �ֶ�¼��
	eMotiveAlarmRec,	    // �ƶ����澯¼��
	ePinAlarmRec,	        // ���ڸ澯¼��
	eTimerRec,	    	    // ��ʱ¼��
	eWeekRec,		        // ����¼��
	ePreRec,			    // Ԥ¼
};

/* ¼��طſ��� */
typedef enum EPuRecPlayType
{
	RECPLAY_NORMAL	= 0,        // ��������
	RECPLAY_STOP,				// ֹͣ����
	RECPLAY_PAUSE,		        // ��ͣ����
	RECPLAY_RESUME,		        // ��������

	RECPLAY_2XFORWARD,          // 2���ٿ������
	RECPLAY_4XFORWARD,          // 4���ٿ������
	RECPLAY_8XFORWARD,          // 8���ٿ������  (�ݲ�֧��)
	RECPLAY_16XFORWARD,         // 16���ٿ������ (�ݲ�֧��)

	RECPLAY_2XFORWARD_SLOW,     // 1/2�������ٲ���
	RECPLAY_4XFORWARD_SLOW,     // 1/4�������ٲ���
	RECPLAY_8XFORWARD_SLOW,     // 1/8�������ٲ���
	RECPLAY_16XFORWARD_SLOW,    // 1/16�������ٲ���

	RECPLAY_FORWARD_FRAME,      // ��֡���Ų��� (�ݲ�֧��)
	
	RECPLAY_2XBACKWARD,         // 2���ٿ��˲��� (�ݲ�֧��)
	RECPLAY_4XBACKWARD,         // 4���ٿ��˲��� (�ݲ�֧��)
	RECPLAY_8XBACKWARD,         // 8���ٿ��˲��� (�ݲ�֧��)
	RECPLAY_16XBACKWARD,        // 16���ٿ��˲��� (�ݲ�֧��)

	RECPLAY_2XBACKWARD_SLOW,    // 1/2�������˲��� (�ݲ�֧��)
	RECPLAY_4XBACKWARD_SLOW,    // 1/4�������˲��� (�ݲ�֧��)

	RECPLAY_BACKWARD_FRAME,     // ��֡���Ų��� (�ݲ�֧��)
};

/* ǰ��¼��طŽ�� */
typedef enum EPuRecPlayRet
{
	RECRET_FAIL,				// ʧ��
	RECRET_SUCCESS,				// �ɹ�
	RECRET_ERR_NODISK,			// ��Ӳ��
	RECRET_ERR_RECORDING,		// ����¼��
	RECRET_ERR_RECPLAYING,		// ����ͨ������,����������
	RECRET_ERR_FILE_NOT_EXIT,	// �ļ�������(��·�����޴��ļ�)
	RECRET_ERR_RECLENGTH_ZERO,	// �ļ�����Ϊ0 
};

// ��ѯ¼���ļ���ʽ
typedef enum EPuRecQueryFileMode
{
    PuRecQueryFileMode_FILEINDEX      = 0,   //���ļ���Ų�ѯ
	PuRecQueryFileMode_QUERYINDEX     = 1,   //����ѯ��Ų�ѯ
};

// ��ѯ¼���ļ�����
typedef enum EPuRecQueryType
{
    PuRecQueryType_ALL                =   0,  // ����
	PuRecQueryType_PIN_ALARM          =   1,  // ���ڸ澯
	PuRecQueryType_MOTIONDETECT_ALARM =   2,  // �ƶ����澯
	PuRecQueryType_MANUAL             =   3,  // �ֶ�¼��
	PuRecQueryType_TIMELY             =   4,  // ��ʱ¼��
	PuRecQueryType_PERIODLY           =   5,  // ����¼��
	PuRecQueryType_FILE               =   6,  // ¼���ļ�
};

// ��ѯͼƬ�ļ�����
typedef enum EPuPicFileQueryType
{
    PuPicFileRecQueryType_ALL         =   0,  // ����
	PuPicFileRecQueryType_CHAN        =   1,  // ͨ���Ų�ѯ
	PuPicFileRecQueryType_TIME        =   2,  // ʱ���ѯ
	PuPicFileRecQueryType_EVENT       =   4,  // �¼���ѯ
};

// ¼���ļ�����״̬
typedef enum EDwRecStatus
{
	RECV_STATUS_START = 1,					  // ��ʼ����
	RECV_STATUS_ADD_STREAM_ERROR,			  // д������
	RECV_STASTU_ADD_FRAME_ERROR,			  // д֡����
	RECV_STATUS_SND_REQ,					  // ������������
	RECV_STATUS_WAIT_STREAM_PROPERTY,		  // �ȴ�������
	RECV_STATUS_SND_PROPERTY_ACK,			  // ���������Ի���
	RECV_STATUS_WAIT_FRAME_DATA,			  // �ȴ�֡���� 
	RECV_STATUS_SND_FRAME_DATA_ACK,			  // ����֡����
	RECV_STATUS_TIMEOUT,					  // ��ʱ
	RECV_STATUS_CLOSE,						  // �ر�
	RECV_STATUS_RCV_END,					  // ���ؽ���
	RECV_STATUS_REQ_INVALID,				  // ������ز���(���ͷ���Ϊ��������������ܲ��Ϸ�)
	RECV_STATUS_PROBE,						
	RECV_DATA_TYPE, 
	RECV_STATUS_SERVER_REFUSE                 //���ӱ��������ܾ�
};

// ̨��ͼƬ����
typedef enum EPuBmpType
{       
    emPuBmpType_Lable = 0,       // ̨��
    emPuBmpType_MMAlarm,         // �ƶ����澯
    emPuBmpType_PinAlarm,        // ���ڸ澯
    emPuBmpType_Banner,          // ���
};

// ̨������ģʽ
typedef enum EPuOsdFontMode
{
    PuOsdFontMode_DEFAULT             =   0,
	PuOsdFontMode_BIG                 =   1,
	PuOsdFontMode_MEDIUM              =   2,
	PuOsdFontMode_SMALL               =   3,
	PuOsdFontMode_CUSTOM              =   4,
};

//̨�������Զ�����
typedef enum EPuCustomFontStyle
{
    PuCustomFontStyle_Normal          = 0x0,
	PuCustomFontStyle_Bold            = 0x1,
	PuCustomFontStyle_Italic          = 0x2,
};

/* ǰ������������ */
typedef enum ECapability
{
	CAP_VIDEODECODER = 0x01,            // 0x01 ��Ƶ����ͨ������
	CAP_VIDEOENCODER,                   // 0x02 ��Ƶ����ͨ������
	CAP_AUDIODECODER,                   // 0x03 ��Ƶ����ͨ������
	CAP_AUDIOENCODER,	                // 0x04 ��Ƶ����ͨ������
	CAP_SERIALPORT,                     // 0x05 ��������
	CAP_INPUTPIN,	                    // 0x06 ������������
	CAP_OUTPUTPIN,	                    // 0x07 ����������� 
	CAP_PTZMOTOR,                       // 0x08 null
	CAP_PTZCONTROL,                     // 0x09 �ɿ�����̨����
	CAP_ARCHIVER,                       // 0x0A null
	CAP_VIDEOIN	= 0x0C,		            // 0x0C ��ƵԴ����
	CAP_ETHERNET,	                    // 0x0D ��̫��������
	CAP_WIRELESS,	                    // 0x0E ������������
	CAP_XIANTELE,	                    // 0x0F �������Ÿ澯����
	CAP_VIDIOADPTER,	                // 0x10 null
	CAP_VIDIOCOMP,						// 0x11 ͼ��ϳ�ͨ����
	CAP_VIDEOSRCPOLL,                   // 0x12 ��ƵԴ��ѯͨ������
	CAP_AUDIOMIX,                       // 0x13 ��Ƶ����ͨ������
	CAP_SHADEAREANUM,                   // 0x14 ͼ���ڵ���������
	CAP_DEVPOWERTYPE,                   // 0x15 ��������
	CAP_MAXSTREAMSEND,                  // 0x16 ����ͨ�����������(һ��ͨ��������Ƶ����Ƶ)
	CAP_MAXSTREAMDIRSEND,               // 0x17 ����ͨ�������ֱ��·��(һ��ͨ��������Ƶ����Ƶ)
	CAP_ENCRYPT,		                // 0x18 �Ƿ���������
	CAP_RECORDER,	                    // 0x19 ¼��ͨ������
	CAP_PLAYER,			                // 0x1A ����ͨ������
	CAP_FILEDOWNLOAD,	                // 0x1B ¼���ļ�����ͨ������
	CAP_VGAOUT,			                // 0x1C ��Ƶ���VGA�ӿ�����
	CAP_VIDEOOUT,		                // 0x1D ��Ƶ����ӿ�����
	CAP_OSDCOLOR,						// 0x1E ̨����Ļ֧�ֵ���ɫ��
	CAP_PLATFORMTYPE,					// 0x1F ����ƽ̨����
	CAP_MOTIONDETECTAREANUM,			// 0x20 �ƶ������������
	CAP_SNAPSHOTCHANNUM,				// 0x21 ץ��ͨ����
	CAP_SLOTINFONUM,					// 0x22 ��λ��Ϣ����(������)
	CAP_CBRVBR,							// 0x23 �Ƿ��ܹ�����ͼ������
	CAP_MAXENCBITRATE,					// 0x24	�����Ч����
	CAP_MULTI_CHANNEL,					// 0x25 �Ƿ�֧��˫��
	CAP_MATRIX,							// 0x26 �Ƿ�֧�־������
	CAP_AUDIODECODETYPE,				// 0x27 ��Ƶ��������
	CAP_SD_CARD,						// 0x28 �Ƿ�ʹ��SD��
	CAP_VERSION,						// 0x29 ǰ�˰汾��
	CAP_TS = 0x30,                      // 0x30 null
	CAP_PUSNAPSHOT,                     // 0x31 ǰ���Ƿ�֧��ץ��
	CAP_WIFI = 0xE0,					// 0xE0 �Ƿ�֧��WIFI
	CAP_PUSNAPSHOT_SHUTTER,             // VSIP_ENTITY_SNAPSHOT_FLASH
	CAP_VIDEO_FRAME_MODE,               // VSIP_ENTITY_VIDEO_FRAM_MODE
	CAP_NEED_REBOOT,
	CAP_SUPPORT_RTSP,
	CAP_AUDIOENCMIC,
	CAP_CAMERATASK,
	CAP_UPGRADEYUNTAI,
	CAP_WATCHONTIME,
	CAP_AUDIOENCAEC,
	CAP_VIDEOENCRANK,
	CAP_VIDEOD1RES,
    CAP_WHITEBALANCELOW,
	CAP_WHITEBALANCEHIGH,
	CAP_DEVTYPE = 0xEE,
    CAP_TIME_ALARM_OSD
};

/* ACK��Ϣ���Ͷ���(�û���Ϣ�ص�) */
#define EVENT_CONNECTION_LOST				    0x05       // ����֪ͨ
#define EVENT_CONNECTION_PING_ACK               0X90       // ��·���pingӦ��
#define EVENT_DDNS_UPDATE						0xA8       // DDNS����֪ͨ
#define EVENT_WIFI_SCAN_ACK						0xAE	   // ǰ��wifiɨ������Ӧ��
#define EVENT_WIFI_CONNECT_ACK					0xAF	   // ǰ������/�Ͽ�wifi������
#define EVENT_WIFI_SET_AP_ACK					0xB7       // ����ǰ�˱����AP�б���
#define EVENT_WIFI_REP_STATE					0xBA       // ǰ��WIFI����״̬�ϱ�
#define EVENT_WIFI_SET_NET_ACK					0xB5       // ����ǰ��WIFI����������
#define EVENT_TRANSPARENTCHAN_CFG_MSG			0xF4	   // ͨ������UDP��Ϣ�õ��İ汾��Ϣ
#define EVENT_TRANSPARENTCHAN_CTRL_MSG			0xF5	   // ͨ������UDP��Ϣ�õ��İ汾��Ϣ
#define EVENT_IPC_QUERY_MONTH_REC_RSP			0xAB	   // ��ѯ¼��������Ϣ
#define EVENT_QUIEY_REC_FILE_NUM_ACK            0x88       // �������������ĸ���
#define EVENT_QUIRY_REC_FILE_ACK                0x7D       // ��ѯ¼���ļ�Ӧ��
#define EVENT_IPC_QUIRY_REC_FILE_RSP			0xA5       // ��ѯIPC¼�����ص�
#define EVENT_REC_FILE_START_PLAYERID_ACK       0X81       // ������id֪ͨ
#define EVENT_REC_PROGRESS_NOTIFY               0x7E       // ����֪ͨ
#define EVENT_RECFILE_UPDATE					0xAC       // ¼�񲥷�ʱ����¼���¼֪ͨ
#define EVENT_IPC_QUERY_PIC_RSP					0xA7       // ͼƬ��ѯ�������
#define EVENT_DOWNLOAD_PIC_FILE_ACK				0xFA	   // ͼƬ���ػ�Ӧ
#define EVENT_DEL_PIC_FILEEX_ACK				0xA9       // ɾ��ͼƬ��Ӧ
#define EVENT_SETBMPOSDINFO_ACK					0xF3	   // ����̨��ͼƬ�ļ����ص�
#define EVENT_EN_MOTD_STATE_CHANGED			    0x03       // �����ƶ����澯״̬֪ͨ
#define EVENT_INPUTPIN_STATE_CHANGED		    0x02       // ���ڸ澯����״̬֪ͨ
#define EVENT_OUTPUTPIN_STATE_CHANGED           0x07       // ���ڸ澯���״̬֪ͨ
#define EVENT_TRANSPARENT_MESSAGE_RESP		    0xB0	   // ͨ��ͨ��buffer
#define EVENT_CONFIG_CHANGED				    0x04       // ��������֪ͨ
#define EVENT_PUSTARTSNAPSHOT_ACK				0xA2	   // ��ʼǰ��ץ��Ӧ��
#define EVENT_SERIAL_DATA					    0x06       // �ն˴�������
#define EVENT_YUNTAI_UPGRADE_ACK                0x77       // ��̨�������Ӧ��

/* �˿ڶ��� */
#define IPCSDK_CMU_PORT                       (unsigned short)1722      // CMU �����˿�
#define IPCSDK_VSIP_PORT                      (unsigned short)5510      // VSIP�˿�
#define IPCSDK_TEST_LINK                      (unsigned short)10000     // ������ⳬʱ������
#define IPCSDK_TEST_LINK_COUNT                (unsigned long)3          // ������ⳬʱ����
#define IPCSDK_RTP_PORT                       (unsigned short)60000     // ��Ƶ����˿�
#define IPCSDK_LOCAL_REC_PORT                 (unsigned short)10086     // ����¼����ʼ�˿�
#define IPCSDK_DEFAULT_MUL_ADDR               (unsigned long)0x012010E0 // Ĭ�ϵ��鲥ip
#define IPCSDK_DOWNLOAD_PORT				  (unsigned short)10000     // ���ر��ؽ��ն˿�
#define IPCSDK_DOWNLOAD_PUPORT				  (unsigned short)1730	    // ǰ�����ط��Ͷ˿�
#define IPCSDK_RECPLAY_STARTPORT			  (unsigned short)65066     // ����˿�

/*************************************************
/	            IPCSDK�ص�ԭ�Ͷ���
/*************************************************/
// �豸��Ϣ�ص�
typedef void (*IPCSDK_MSG_ACK_CB)(unsigned long hHandle, unsigned char uMsgType, void* pData, int nSize, void* pContext);

// ��Ƶ����ص�(YUV420����)
typedef void (*IPCSDK_DRAW_CB)(unsigned long hHandle, unsigned long hPlayHandle, unsigned long dwPaintDC, unsigned long dwParam);

// ¼��״̬�ص�
typedef void (*IPCSDK_REC_STATE_CB)(unsigned long hHandle, unsigned long hPlayHandle, unsigned long dwRecorderID, unsigned long dwCBType,
									void* pData, int nDataLen, void* pContext);

// ¼�����ػص�
typedef void (*IPCSDK_DOWN_REC_CB)(unsigned long hHandle, unsigned long hPlayHandle, unsigned long dwStatusCode, unsigned long dwCurTime,
								   void* pContext);

// �����ļ����Żص�
typedef void (*IPCSDK_FILE_STAT_CB)(unsigned long hPlayHandle, unsigned char byFileState, unsigned long dwSec, unsigned long dwContext);

// ����ת���ص�
typedef void (*IPCSDK_STREAMCB)(unsigned long hHandle, unsigned char byChanId, unsigned char* pBuffer, long lLength, void* pContext);

#endif