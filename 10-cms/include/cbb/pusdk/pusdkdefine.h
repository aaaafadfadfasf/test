//////////////////////////////////////////////////////////////////////////
// pusdkdefine.h 
#ifndef PUSDKDEFINE_H_
#define PUSDKDEFINE_H_

#define PUSDK_DEFCFG_TIMEZONE                8          // ϵͳʱ��
#define PUSDK_MAX_DISKPART_NUM              16          // ����������
#define PUSDK_MAX_FILEPATH_LEN              64          // �ļ�·����󳤶�
#define PUSDK_MAX_MOTION_DETECT_AREAM_NUM   16
#define PUSDK_MAX_MTALIAS_LEN               16
#define PUSDK_MAX_NATSEND_INT               30         // nat�����ͼ��(��) 
#define PUSDK_MAX_PIC_REGION_NUM            6
#define PUSDK_MAX_POLL_PORT_NUM             5
#define PUSDK_MAX_PUID_LEN                  32
#define PUSDK_MAX_STRING_LEN16              16
#define PUSDK_MAX_STRING_LEN32              32
#define PUSDK_MAX_STRING_LEN64              64
#define PUSDK_MAX_USER_NUM                  256
#define PUSDK_MAX_VERSION_STRING_LEN        64
#define PUSDK_MAX_VIDEO_SHADE_AREA_NUM      16
#define PUSDK_MAX_VIDEOIN_PORT_NUM          16          // �����Ƶ����˿���
#define PUSDK_SYNCMD_TIMEOUT                (5*1000)    // ͬ��������ʱʱ��
#define PUSDK_SEARCH_TIMEOUT                (5*1000)    // ������ʱʱ��
#define PUSDK_SYNC_TIME_LOCAL               (-1)        // ͬ��ʱ�����õı���ͬ��ʱ��
#define PUSDK_MAX_DISK_NUM                  4           // ��������Ϣ��
#define PUSDK_MAX_SUBEREA_NUM               4           // ÿ�����̵��ӷ�����
#define MAX_RECIPROCAL_LIST_NUM				30


#ifndef MAX_PATH
#define MAX_PATH                            260
#endif


// ����ͷ���ƺ����ƶ�����
#define PUSDK_CAMERA_MOVE_RANGE              2

#define PUSDK_MAX_TRANFER_NUM                3           // ���ֱ��·��

// �˿ڶ���
#define PUSDK_CMU_PORT                       (unsigned short)1722      // CMU �����˿�
#define PUSDK_VSIP_PORT                      (unsigned short)5510      // VSIP�˿�
#define PUSDK_TEST_LINK                      (unsigned short)10000     // ������ⳬʱ������
#define PUSDK_TEST_LINK_COUNT                (unsigned long)3        // ������ⳬʱ����
#define PUSDK_RTP_PORT                       (unsigned short)60000     // ��Ƶ����˿�
#define PUSDK_LOCAL_REC_PORT                 (unsigned short)10086     // ����¼����ʼ�˿�
#define PUSDK_DEFAULT_MUL_ADDR               (unsigned long)0x012010E0 // Ĭ�ϵ��鲥ip

//////////////////////////////////////////////////////////////////////////
// ����ͷ����
#define PUSDK_CAM_NAME_SONY                  "SONY"
#define PUSDK_CAM_NAME_AD                    "AD"
#define PUSDK_CAM_NAME_PELCO                 "PELCO-D"
#define PUSDK_CAM_NAME_PIH                   "����"
#define PUSDK_CAM_NAME_PELCO_P               "PELCO-P"
#define PUSDK_CAM_NAME_PARCO                 "PARCO�ظ�"
#define PUSDK_CAM_NAME_AB                    "AB"
#define PUSDK_CAM_NAME_YUNTAI                "������̨"
#define PUSDK_CAM_NAME_Samsung               "Samsung����"
#define PUSDK_CAM_NAME_YAAN                  "YUNTAI�ǰ�"
#define PUSDK_CAM_NAME_AD168                 "AD168"
#define PUSDK_CAM_NAME_KALATEL               "KALATEL������"
#define PUSDK_CAM_NAME_JN                    "JNΰ��"
#define PUSDK_CAM_NAME_WV_CS650              "����CS650"
#define PUSDK_CAM_NAME_WV_CS850              "����CS850"
#define PUSDK_CAM_NAME_ALEC                  "ALEC������"
#define PUSDK_CAM_NAME_AB_P                  "AB-P"
#define PUSDK_CAM_NAME_AB_D                  "AB-D"
#define PUSDK_CAM_NAME_PELCO_D_JEC           "JEC PELCO-D"
#define PUSDK_CAM_NAME_PELCO_P_JEC           "JEC PELCO-P"
#define PUSDK_CAM_NAME_TIANDY                 "TIANDY"
#define PUSDK_CAM_NAME_MATRIX                "ͨ�þ���Э��"
#define PUSDK_CAM_NAME_ROBOT                 "ROBOT ����ϳɾ���"

// �豸��ҵ����
#define PUSDK_INDUSTRY_INVALID               (unsigned char)0  // ��Ч���� 
#define PUSDK_INDUSTRY_TELECOM               (unsigned char)1  // �й����� 
#define PUSDK_INDUSTRY_NETCOM                (unsigned char)2  // �й���ͨ 
#define PUSDK_INDUSTRY_UNICOM                (unsigned char)3  // �й���ͨ 
#define PUSDK_INDUSTRY_RAILCOM               (unsigned char)4  // �й���ͨ 
#define PUSDK_INDUSTRY_MOBILE                (unsigned char)5  // �й��ƶ� 
#define PUSDK_INDUSTRY_ENTERPRISE            (unsigned char)6  // ��ҵר�� 

// �豸��ҵ����

//name �澯��������
#define PUSDK_LINKSET_VIDSRCSWT_SUPPORT   (unsigned short)1   // ֧����ƵԴ�л� 
#define PUSDK_LINKSET_CAMPOSSWT_SUPPORT   (unsigned short)2   // ֧������ͷԤ��λ�л� 
#define PUSDK_LINKSET_SNAPSHOT_SUPPORT    (unsigned short)4   // ֧��ץ�� 
#define PUSDK_LINKSET_LOCALREC_SUPPORT    (unsigned short)8   // ֧��ǰ��¼�� 
#define PUSDK_LINKSET_ALARMTIME_SUPPORT   (unsigned short)16  // ֧�ָ澯��ʱ 
#define PUSDK_LINKSET_OSDSHOW_SUPPORT     (unsigned short)32  // ֧��̨����ʾ 
#define PUSDK_LINKSET_PINOUTPUT_SUPPORT   (unsigned short)64  // ֧�ֲ������ 
#define PUSDK_LINKSET_AUDIOOUT_SUPPORT    (unsigned short)128 // ֧�ָ澯��Ƶ��� 
#define PUSDK_LINKSET_CONVIDSRC_SUPPORT   (unsigned short)256 // ֧�ֹ�����ƵԴ 
// �澯��������

// ǰ�˾��
#define PUHANDLE      long
#define LPPUHANDLE    long*

// ���������
#define PLAYERHANDLE long
#define LPPLAYERHANDLE    long*

// ���ض��������
#define ERR_SUCCESS                  (0)     // �ɹ�
#define ERR_INVALID                  (-1)    // �ڲ���̬�����������,û�г�ʼ������,������ͨ��ָ�뷵��
#define ERR_SIZE                     (-2)    // ��С����ȷ     
#define ERR_TYPE                     (-3)    // ���Ͳ���ȷ
#define ERR_RANGE                    (-4)    // ��Χ����ȷ
#define ERR_HANDLE                   (-5)    // ��Ч��handle
#define ERR_OVERCAP                  (-6)    // �豸������֧��
#define ERR_TIMEOUT                  (-7)    // ��ʱ����
#define ERR_STATE                    (-8)    // ״̬����ǰ������������
#define ERR_SPACE                    (-9)    // �ռ䲻��
#define ERR_PATH                     (-10)   // �����·��
#define ERR_LACK_RESOURECE           (-11)   // ��Դ����
#define ERR_MSG_SEQUENCE             (-12)   // ��Ϣ���д���
#define ERR_DLL_LINK                 (-13)   // dll���Ӵ���
#define ERR_LOCALFILE_OPEN           (-14)   // û�д򿪱����ļ�
#define ERR_LOCALFILE_PLAY           (-15)   // û�в��ű����ļ�
#define ERR_PU_ERR                   (-16)   // ǰ�˷��ش���
#define ERR_NO_DLL_CAMERA            (-17)   // û��camera.dll��
#define ERR_CAMERA_TYPE              (-18)   // ����ͷ���Ͳ���ȷ
#define ERR_GET_CAMERA_OPT_CODE      (-19)   // ��ȡ����ͷ������ʧ��
#define ERR_FILE_OPT                 (-20)   // �ļ�����ʧ��
#define ERR_PLAYER_HANDLE            (-21)   // handle�����palyer�������Ӧ
#define ERR_PALYER_INVALID           (-22)   // player��Ч
#define ERR_PALYER_REC               (-23)   // player����¼��
#define ERR_PALYER_PLAY              (-24)   // player���ڲ�����Ƶ
#define ERR_SEARCH                   (-25)   // ��������
#define ERR_CREATE_BMP               (-26)   // ����λͼʧ��
#define ERR_CONNECT_EXIST            (-27)   // �����Ѿ�����
#define ERR_CONNECT_PU               (-28)   // �����豸ʧ��
#define ERR_REQ_RECV_AUDIO           (-29)   // ����ǰ�˽�����Ƶʧ��

#define ERR_OSP                      (-30)   // Osp�齨����
#define ERR_VSIP                     (-31)   // Э��ջ�齨����
#define ERR_SWITCH                   (-32)   // ת������
#define ERR_ENC                      (-40)   // ����������
#define ERR_DEC                      (-41)   // ����������
#define ERR_RPPLAY                   (-50)   // ¼������
#define ERR_RPDOWNLOAD               (-51)   // ¼�����ؿ����
#define ERR_CAMERA                   (-60)   // ץ�Ŀ����
#define ERR_PICISDOWN                (-61)   // ��ǰ��������ͼƬ
#define ERR_PICISUPLOAD              (-62)   // ��ǰ�����ϴ�ͼƬ
#define ERR_DEC_ICOPEN               (-100)  // ���ؽ������쳣

// ��ǰ�˽����Ĵ���
#define ERR_SRART                    (-100)

#define ERR_LOGIN_OTHER              (ERR_SRART+11) // ��½��������
#define ERR_LOGIN_TIMEOUT            (ERR_SRART+12) // ��½��ʱ
#define ERR_LOGIN_PASSWORD           (ERR_SRART+13) // �û��������벻��ȷ
#define ERR_LOGIN_ADMINLOGIN         (ERR_SRART+14) // ����Ա�Ѿ���¼
#define ERR_LOGIN_MAX_USER           (ERR_SRART+15) // USER�û���¼�ѵ����ֵ
#define ERR_LOGIN_MAX_LOGIN          (ERR_SRART+16) // �û���¼�����ѵ����ֵ
#define ERR_LOGIN_NOT_CONN           (ERR_SRART+17) // û������

// �豸����汾���� CAP_VERSION
#define     PUSDK_V39_VERSIONID       0 // 3.9�汾
#define     PUSDK_V40_VERSIONID       1 // 4.0�汾
#define     PUSDK_V50_VERSIONID       2 // 5.0�汾

#define     PUSDK_FILE_TYPE_ASF       0 // asf�ļ�����
//#define 	PUSDK_FILE_TYPE_KSJ       1
//#define 	PUSDK_FILE_TYPE_KDS       2

//! ����ģʽ;�µ�ģʽ��ӣ�ע��ʹ��λ�ķ�ʽ��
#define PUSDK_WIRELESS_MODE_INVALID   (unsigned char)0   // ��Чģʽ 
#define PUSDK_WIRELESS_MODE_WIFI      (unsigned char)1   // WiFiģʽ 
#define PUSDK_WIRELESS_MODE_3G        (unsigned char)2   // 3Gģʽ 

// ��Ļ�������
#define MAX_CAPTION_NUM               100   // ÿ����Ļ��󳤶�
#define MAX_BMP_CAPTION_NUM           12    // �����Ļ����
#define MAX_ROW_SPACE                 1024  // ����м��
#define MAX_SCREEN_OSD_X              720   // ��Ļ�����
#define BLUR_COLOR_TYPE_1             1     // ���߹�һ������
#define BLUR_COLOR_TYPE_2             2     // ���߹���������

// �汾���� �ļ�����
#define PUSDK_UPDATE_TYPE_UBOOT	    1
#define PUSDK_UPDATE_TYPE_IOS		2
#define PUSDK_UPDATE_TYPE_APP		3
#define PUSDK_UPDATE_TYPE_SYS		4
#define PUSDK_UPDATE_TYPE_ALL		5

// �汾���� Ĭ��FTP�˿�
#define PUSDK_DEFAULT_FTP_PORT		54321


// �����Ƶ����������
typedef enum
{
    emPlayMain = 0,
    emPlaySub  = 1,
}EPlayChanType;

// ����״̬
typedef enum
{
    emPlaying,
    emStop,
    emPause,
}EPlayState;

// ��������
typedef enum 
{
    emPlayVideo = 1,
    emPlayRec   = 2
}EPlayType;

// ̨������ //ͼƬ����
typedef enum
{       
    emPuBmpType_Lable               = 0,  // ̨��
    emPuBmpType_MMAlarm,                  // �ƶ����澯
    emPuBmpType_PinAlarm,                 // ���ڸ澯
    emPuBmpType_Banner,                   // ���
} EPuBmpType;
 
// �豸���������
typedef enum
{
    PUSDK_DEVICE_NONE                = 0, // ��Ƶ����붼��֧��
    PUSDK_DEVICE_ENCODER             = 1, // ֻ֧����Ƶ����
    PUSDK_DEVICE_DECODER             = 2, // ֻ֧����Ƶ����
    PUSDK_DEVICE_BOTH                = 3, // ��Ƶ����붼֧��
    PUSDK_DEVICE_SMARTSIGHT_ENCODER  = 4, // SMARTSIGHT��Ƶ����֧��
    PUSDK_DEVICE_SMARTSIGHT_DECODER  = 5, // SMARTSIGHT��Ƶ����֧��
    PUSDK_DEVICE_SMARTSIGHT_BOTH     = 6, // SMARTSIGHT��Ƶ����붼֧��                
} EPuDeviceType;

//��Ƶ���ݸ�ʽ
typedef enum
{
    PUSDK_VIDEO_TYPE_NONE            = 0,
    PUSDK_VIDEO_TYPE_SN4             = 1,//����
    PUSDK_VIDEO_TYPE_MPEG4           = 97,
    PUSDK_VIDEO_TYPE_H261            = 31,
    PUSDK_VIDEO_TYPE_H263            = 34,
    PUSDK_VIDEO_TYPE_H264            = 106,            
} EPuVideoType;

//��Ƶ���ݸ�ʽ
typedef enum
{
    PUSDK_AUDIO_TYPE_PCMA            = 8,
    PUSDK_AUDIO_TYPE_PCMU            = 0,
    PUSDK_AUDIO_TYPE_ADPCM           = 5,
    PUSDK_AUDIO_TYPE_G7221C          = 98,               
} EPuAudioType;

//ͼ��ֱ���
typedef enum
{
    PUSDK_VIDEO_RESOLUTION_AUTO      = 0,
    PUSDK_VIDEO_RESOLUTION_QQCIF     = 0x08,
    PUSDK_VIDEO_RESOLUTION_QCIF      = 0x20,
    PUSDK_VIDEO_RESOLUTION_CIF       = 0x40,
    PUSDK_VIDEO_RESOLUTION_QVGA      = 0x42,   
    PUSDK_VIDEO_RESOLUTION_D1        = 0x82,
    PUSDK_VIDEO_RESOLUTION_VGA       = 0x83,   
    PUSDK_VIDEO_RESOLUTION_2CIF      = 0x60,
    PUSDK_VIDEO_RESOLUTION_XVGA      = 0xA3,   
    PUSDK_VIDEO_RESOLUTION_720P      = 0xD0,   
    PUSDK_VIDEO_RESOLUTION_1080P     = 0xE0,   
} EPuVideoResolution;

//��������Ƶʱ����ʾ
typedef enum
{
    PUSDK_PLAYER_STARVE_MODE_INVALID     = 0, //��Ч(�ر�)
    PUSDK_PLAYER_STARVE_MODE_COLOR_BAR   = 1, //��ɫ��
    PUSDK_PLAYER_STARVE_MODE_LAST_IMAGE  = 2, //��Ļ����
    PUSDK_PLAYER_STARVE_MODE_BLUE_SCR    = 3, //����
    PUSDK_PLAYER_STARVE_MODE_SHOW_OSD    = 4, //��Ļ���Ტ��Ļ��ʾ                
} EPuPlayerStarveMode;

//��Ƶ����˿�����
typedef enum
{
    PUSDK_VIDEO_OUT_TYPE_INVALID         = 0, // ��Ч����
    PUSDK_VIDEO_OUT_TYPE_C               = 1, // ����C����
    PUSDK_VIDEO_OUT_TYPE_S               = 2, // S����
    PUSDK_VIDEO_OUT_TYPE_VGA             = 3, // VGA���
} EPuVideoOutType;

//̨������ģʽ
typedef enum
{
    EPuOsdFontMode_DEFAULT      = 0,
    EPuOsdFontMode_BIG          = 1,
    EPuOsdFontMode_MEDIUM       = 2,
    EPuOsdFontMode_SMALL        = 3,
    EPuOsdFontMode_CUSTOM       = 4,
} EPuOsdFontMode;

//̨�������Զ�����
typedef enum
{
    EPuCustomFontStyle_Normal   = 0x0,
    EPuCustomFontStyle_Bold     = 0x1,
    EPuCustomFontStyle_Italic   = 0x2,
} EPuCustomFontStyle;

// ����״̬
typedef enum
{
    EPuDiskState_NODISK         = 0,      // ��Ӳ��
    EPuDiskState_NOPARTION      = 1,      // δ����
    EPuDiskState_NOFORMAT       = 2,      // δ��ʽ��
    EPuDiskState_NORMAL         = 3,      // ����
    EPuDiskState_READONLY       = 4,      // ֻ��
    EPuDiskState_INVALID        = 5,      // ������
} EPuDiskState;

// ���̽���״̬
typedef enum
{
    EPuDiskHeath_Ok             = 0x0,    // ����
    EPuDiskHeath_NeedRepair     = 0x01,   // ����ǿ���޸�
    EPuDiskHeath_NeedFormat     = 0x02,   // �����ʽ��
    EPuDiskHeath_NeedChangeDisk = 0x04,   // ����Ӳ��
    EPuDiskHeath_NeedReboot     = 0x08,   // ��������        
} EPuDiskHealth;

//Event Arguments 
typedef enum
{
    PUSDK_STATE_ON   = 0x00,
    PUSDK_STATE_OFF  = 0x01,
} EEventType;

// ��Ļ����(����)��
typedef  enum
{
	eTextHeightDefault, // eTextHeightNo5
	eTextHeightNo0,     // 56
	eTextHeightNo1,     // 48
	eTextHeightNo2,     // 34
	eTextHeightNo3,     // 32
	eTextHeightNo4,     // 28
	eTextHeightNo5,     // 24
	eTextHeightNo6,     // 22
	eTextHeightNo7      // 20 
}EFontTable;

/************************************************************************/
/* Message interface, message type                                      */
/************************************************************************/

// ǰ�˲����ṹ����
typedef enum 
{
    emPuConfig_Invalid                 = 0,
    emPuConfig_Sys                     = 1,    // TPuSysParam
    emPuConfig_SysPub                  = 2,    // TPuSysPubParam
//  emPuConfig_SysTime                 = 3,    // Dummy, not set by user
    emPuConfig_PPPOE                   = 4,    // TPuPppoeParam V4��֧��
//  emPuConfig_Cap                     = 5,    // TPuCapInfo
    
    emPuConfig_VideoEnc                = 6,    // TPuVideoEncParam
    emPuConfig_VideoEncPub             = 7,    // TPuVideoEncPubParam
    emPuConfig_VideoEncNet             = 8,    // TPuCommonNetParam  
    emPuConfig_VideoEncOSD             = 9,    // TPuVideoEncOsdParam
    emPuConfig_VideoEncMP              = 10,   // TPuVideoEncMpParam
    emPuConfig_VideoEncInputPort       = 11,   // TPuVideoEncInputPortParam
    emPuConfig_VideoEncPoll            = 12,   // TPuVideoEncPollParam
    emPuConfig_VideoEncMultiEncChan    = 13,   // TPuMultiEncChanParam
    
    emPuConfig_VideoDec                = 14,   // TPuVideoDecParam
    emPuConfig_VideoDecPub             = 15,   // TPuVideoDecPubParam
    emPuConfig_VideoDecNet             = 16,   // TPuCommonNetParam
    emPuConfig_VideoDecRetransfer      = 17,   // TPuVideoDecRetransferParam
    
    emPuConfig_AudioEnc                = 18,   // TPuAudioEncParam V4��֧��
    emPuConfig_AudioEncNet             = 19,   // TPuCommonNetParam
    
    emPuConfig_AudioDec                = 20,   // TPuAudioDecParam V4��֧��
    emPuConfig_AudioDecNet             = 21,   // TPuCommonNetParam
    
    emPuConfig_SerialPort              = 22,   // TPuSerialPortParam
    emPuConfig_SerialPortPub           = 23,   // TPuSerialPortPubParam
    
    emPuConfig_VideoInput              = 24,   // TPuVideoInputParam
    emPuConfig_OsdScroll               = 25,   // TPuOsdScroll
    
    emPuConfig_VideoShade              = 26,   // TPuVideoShadeParam
    
//  emPuConfig_AlarmLink               = 27,   // TPuAlarmLinkInfo
    
    emPuConfig_Nms                     = 28,   // TPuNmsParam
    
//  emPuConfig_VideoDestAdd            = 29,   // Dummy, not set by user
//  emPuConfig_VideoDestRemove         = 30,   // Dummy, not set by user
//  emPuConfig_AudioDestAdd            = 31,   // Dummy, not set by user
//  emPuConfig_AudioDestRemove         = 32,   // Dummy, not set by user
    
//  emPuConfig_CamCtrl                 = 33,
//  emPuConfig_PuNms                   = 34,   // TPuNms
//  emPuConfig_OutputPinState          = 35,   // Dummy, not set by user
    emPuConfig_Snapshot                = 36,   // TPuSnapshotConfig
    emPuConfig_PuSnapConf              = 39,   // TPuSnapConfig 

    //5.0 cfg
    emPuConfig_SysExt                  = 51,   // TPuSysExtParam V4��֧��
    emPuConfig_P2p                     = 52,   // TPuVideoDecPpParam    
    emPuConfig_RecChan                 = 53,   // TPuRecChannel V4��֧��
    emPuConfig_RecSys                  = 54,   // TPuRecSys V4��֧��
	emPuConfig_RecChanBase             = 40,   // TPuRecChanBasicInfo(ƽ̨)
	emPuConfig_RecChanPeriodly         = 41,   // TPuRecChannel(ƽ̨)
	emPuConfig_RecChanTimely           = 42,   // TVsipRecChannel(ƽ̨)
	emPuConfig_RecSysInfo              = 43,   // TVsipRecSys(ƽ̨)
    emPuConfig_RecSysEx                = 55,   // TPuRecSysEx V4��֧��
    emPuConfig_SnapshotStore           = 56,   // TPuSnapshotStoreConfig
    emPuConfig_CamCfg                  = 57,   // TPuCameraCfg
    emPuConfig_InputPin                = 59,   // TPuInputPinParam V4��֧��
    emPuConfig_MotionDet               = 60,   // TPuMotionDetectParam V4��֧��
    emPuConfig_OutputPin               = 61,   // TPuOutputPinParam V4��֧��
	emPuConfig_Ddns                    = 62,   // TPuDdnsParam(����)
    emPuConfig_ExtCard                 = 63,   // TPuExtCardParam V4��֧��
    
    emPuConfig_OsdExtension            = 65,   // TPuDAQDeviceParam
    emPuConfig_WirelessMode            = 67,   // TPuWirelessModeParam (����)
    emPuConfig_VerUpdateInfo           = 73,   // TPuVerUpdateInfo (����)

    emPuConfig_WifiApInfo              = 74,   // TPuWifiApInfo ���AP��(����) 
    emPuConfig_WifiCfgGetResp          = 75,   // TPuWifiCfgGetResp ����WIFI�ȵ�(����)
    emPuConfig_WifiConnectionReqParam  = 76,   // TPuWifiConnectionReqParam ����һ���ȵ�(����)
    //emPuConfig_WifiConnectionStatus    = 77,   // TPuWifiConnectionStatus (����)
    emPuConfig_WifiNetParam            = 78,   // TPuWifiNetParam �����ѯ����(����)

    emPuConfig_3gCfgParam              = 79,   // TPu3gCfgParam (����)
    emPuConfig_3gStatus                = 80,   // TPu3gStatus (����)
	emPuConfig_3gNetCfg                = 81,   // TPu3gNetCfg
    emPuConfig_GpsCfgParam             = 82,   // TPuGpsCfgParam (����)
    emPuConfig_LocalImageOutput        = 83,   // TPuLocalImageOutputParam
    
    emPuConfig_ImageDisplayParam       = 84,   // TPuImageDisplayParam
    emPuConfig_VideoDecCompParam       = 85,   // TPuVideoDecCompParam


    emPuConfig_End,
} EPuConfigType;

//����ͷ����
typedef enum
{
    PUSDK_CAMERA_TYPE_SONY=1,        //  SONY
    PUSDK_CAMERA_TYPE_AD,            //  AD
    PUSDK_CAMERA_TYPE_PELCO,         //  PELCO-D
    PUSDK_CAMERA_TYPE_PIH,           //  ����
    PUSDK_CAMERA_TYPE_PELCO_P,       //  PELCO-P
    PUSDK_CAMERA_TYPE_PARCO,         //  PARCO�ظ�
    PUSDK_CAMERA_TYPE_AB,            //  AB
    PUSDK_CAMERA_TYPE_YUNTAI,        //  ������̨
    PUSDK_CAMERA_TYPE_SAMSUNG,       //  ����
    PUSDK_CAMERA_TYPE_YAAN,          //  �ǰ�
    PUSDK_CAMERA_TYPE_AD168,         //  AD168����
    PUSDK_CAMERA_TYPE_KALATEL,       //  ������
    PUSDK_CAMERA_TYPE_JN,            //  JNΰ��
    PUSDK_CAMERA_TYPE_WV_CS650,      //  ����,��850����Э��
    PUSDK_CAMERA_TYPE_WV_CS850,      //  ����,��850����Э��
    PUSDK_CAMERA_TYPE_ALEC,          //  ������
    PUSDK_CAMERA_TYPE_AB_P,          //  AB-P
    PUSDK_CAMERA_TYPE_AB_D,          //  AB-D
    PUSDK_CAMERA_TYPE_PELCO_D_JEC,   //  JEC PELCO-D
    PUSDK_CAMERA_TYPE_PELCO_P_JEC,   //  JEC PELCO-P
    PUSDK_CAMERA_TYPE_TIANDY,        //  ���ΰҵ
    // PUSDK_CAMERA_TYPE_SUNNHO,     //  SUNNHO    ����
    // PUSDK_CAMERA_TYPE_PEARMAIN,   //  ��ƻ��  ����
	PUSDK_CAMERA_TYPE_PELCO_D_KEDA = 26, // PELCO_D_K
    PUSDK_CAMERA_TYPE_CAMERA_NUM,
    
    PUSDK_CAMERA_TYPE_MATRIX=128,    //  ����Э��
    PUSDK_MATRIX_TYPE_AB=128,        //  AB ����
    PUSDK_CAMERA_TYPE_ROBOT,         //  ROBOT ����ϳɾ���
    PUSDK_MATRIX_TYPE_SUNNHO,        //  SUNNHO    ����
    PUSDK_MATRIX_TYPE_PEARMAIN,      //  ��ƻ��  ����
    PUSDK_CAMERA_TYPE_MATRIX_NUM,
} EPuCameraType;

// �澯���״̬
typedef enum
{
    emPuRunTimeMode_None   = 0,
    emPuRunTimeMode_Always = 0x01,
    emPuRunTimeMode_Fix    = 0x02,
    emPuRunTimeMode_Day    = 0x04,
    emPuRunTimeMode_Week   = 0x08,
} EPuRunTimeMode;


//////////////////////////////////////////////////////////////////////////
// ǰ��¼�񲥷Ų�������
typedef enum 
{
    RECPLAY_NORMAL    = 0,      // ��������
    RECPLAY_STOP,               // ֹͣ����
    RECPLAY_PAUSE,              // ��ͣ����
    RECPLAY_RESUME,             // ��������
    
    RECPLAY_2XFORWARD,          // 2���ٿ������
    RECPLAY_4XFORWARD,          // 4���ٿ������
    
    RECPLAY_2XFORWARD_SLOW,     // 1/2�������ٲ���
    RECPLAY_4XFORWARD_SLOW,     // 1/4�������ٲ���
}EPuRecPlayType;

// ��ѯ¼���ļ���ʽ
typedef enum
{
    TPuRecQueryFileMode_FILEINDEX      = 0,   //���ļ���Ų�ѯ
    TPuRecQueryFileMode_QUERYINDEX     = 1,   //����ѯ��Ų�ѯ
} EPuRecQueryFileMode;

// ��ѯ¼���ļ�����
typedef enum
{
    EPuRecQueryType_ALL                =   0,
    EPuRecQueryType_PIN_ALARM          =   1,
    EPuRecQueryType_MOTIONDETECT_ALARM =   2,
    EPuRecQueryType_MANUAL             =   3,
    EPuRecQueryType_TIMELY             =   4,
    EPuRecQueryType_PERIODLY           =   5,
    EPuRecQueryType_FILE               =   6,
} EPuRecQueryType; 

// ��ѯͼƬ�ļ�����
typedef enum
{
    EPuPicFileRecQueryType_ALL      =   0,  // ����
    EPuPicFileRecQueryType_CHAN     =   1,  // ͨ���Ų�ѯ
    EPuPicFileRecQueryType_TIME     =   2,  // ʱ���ѯ
    EPuPicFileRecQueryType_EVENT    =   4,  // �¼���ѯ
} EPuPicFileQueryType;

// ֧�ֵı���ͼ����ʾ������������
// ��ʹ��ʱʹ��"|"�����������Ӽ���,����ͬʱ֧�ֻ���ϳ�����,�ֱ������� 
// emPuImageSetting_PicComp | emPuImageSetting_Resolution 
//
typedef enum EnumPuImageSetting
{
    emPuImageSetting_PicComp       = 0x01,  // ����ϳ�����
    emPuImageSetting_Resolution    = 0x02,  // �ֱ�������
    emPuImageSetting_RefreshRate   = 0x04,  // ˢ��������
    emPuImageSetting_OutputMode    = 0x08,  // ���ģʽ����
} EPuImageSetting;

// ǰ������������
typedef enum
{
    CAP_VIDEODECODER = 0x01,     // ��Ƶ����ͨ������
    CAP_VIDEOENCODER,            // ��Ƶ����ͨ������
    CAP_AUDIODECODER,            // ��Ƶ����ͨ������
    CAP_AUDIOENCODER,            // ��Ƶ����ͨ������
    CAP_SERIALPORT,              // ��������
    CAP_INPUTPIN,                // ������������
    CAP_OUTPUTPIN,               // �����������
    CAP_PTZMOTOR,                // null
    CAP_PTZCONTROL,              // �ɿ�����̨����
    CAP_ARCHIVER,                // null
    CAP_VIDEOIN    = 0x0C,       // ��ƵԴ����
    CAP_ETHERNET,                // ��̫��������
    CAP_WIRELESS,                // ������������
    CAP_XIANTELE,                // �������Ÿ澯����
    CAP_VIDIOADPTER,             // null
    CAP_VIDIOCOMP,               // ͼ��ϳ�ͨ����
    CAP_VIDEOSRCPOLL,            // ��ƵԴ��ѯͨ������
    CAP_AUDIOMIX,                // ��Ƶ����ͨ������
    CAP_SHADEAREANUM,            // ͼ���ڵ���������
    CAP_DEVPOWERTYPE,            // ��������
    CAP_MAXSTREAMSEND,           // ����ͨ�����������(һ��ͨ��������Ƶ����Ƶ)
    CAP_MAXSTREAMDIRSEND,        // ����ͨ�������ֱ��·��(һ��ͨ��������Ƶ����Ƶ)
    CAP_ENCRYPT,                 // �Ƿ���������
    //    CAP_RECORDER,          // 
    CAP_PLAYER,                  // ����ͨ������
    CAP_FILEDOWNLOAD,            // ¼���ļ�����ͨ������
    CAP_VGAOUT,                  // ��Ƶ���VGA�ӿ�����
    CAP_VIDEOOUT,                // ��Ƶ����ӿ�����
    CAP_OSDCOLOR,                // ̨����Ļ֧�ֵ���ɫ��
    CAP_PLATFORMTYPE,            // ����ƽ̨����
    CAP_MOTIONDETECTAREANUM,     // �ƶ������������
    CAP_SNAPSHOTCHANNUM,         // ץ��ͨ����
    CAP_SLOTINFONUM,             // ��λ��Ϣ����(������)
    CAP_CBRVBR,                  // �Ƿ��ܹ�����ͼ������
    CAP_MAXENCBITRATE,           // �����Ч����
    CAP_MULTI_CHANNEL,           // �Ƿ�֧��˫��
    CAP_MATRIX,                  // �Ƿ�֧�־������
    CAP_AUDIODECODETYPE,         // null
    CAP_SD_CARD,                 // �Ƿ�ʹ��SD��
    CAP_VERSION,                 // ǰ�˰汾��
    CAP_TS = 0x30,               // null
    // ǰ��ץ��ʹ��
    CAP_SNAPSHOT_UNSUPPORTED,    // ǰ���Ƿ�֧��ץ��
    CAP_SNAPSHOT_DISKSTORAGE,    // ǰ���Ƿ�֧�ֱ���Ӳ��ץ��
    CAP_SNAPSHOT_BUFSENTBACK,    // ǰ���Ƿ�֧�����ݻش�ץ��
    CAP_SNAPSHOT_FTPSENTBACK,    // ǰ���Ƿ�֧��FTP�ش�ץ��
    // ǰ��¼��ʹ��
    CAP_RECORDER,                // ǰ���Ƿ�֧��¼�� 
    CAP_PUREC_MANUAL,            // ǰ���Ƿ�֧���ֶ�¼��
    CAP_PUREC_ALARM,             // ǰ���Ƿ�֧�ָ澯¼��
    CAP_PUREC_TIMELY,            // ǰ���Ƿ�֧�ֶ�ʱ¼��
    CAP_PUREC_WEEKLY,            // ǰ���Ƿ�֧������¼��
    CAP_SUPPORT_P2P,             // ǰ���Ƿ�֧��P2P
    CAP_SYS_LOG,                 // ǰ���Ƿ�֧����־���  
    CAP_MAX_SHADE_AREA_SIZE,     // ���ͼ���ڱ����

    CAP_COMPSTYLE,               // ֧�ֵĻ���ϳɷ������
    CAP_IMAGE_DISPLAY_SETTING,   // ֧�ֵı���ͼ����ʾ������������(ÿ��bit��ʾһ��������) EPuImageSetting
    CAP_SUPPORT_IMAGE_DISPLAY,   // ֧�ֱ���ͼ�����

    CAP_AEC,                     // �Ƿ�֧�ֻ�������
    CAP_OSD_SCROLL,              // �Ƿ�֧�ֺ��
    CAP_OSD_EXTENSION,           // �Ƿ�֧�ֶ�̬��Ļ

    CAP_GPS,                     // �Ƿ�֧��GPS
    CAP_WIFI,                    // �Ƿ�֧��WiFi
    CAP_3G,                      // �Ƿ�֧��3G

	CAP_LICENSE,                 // ֧��ƽ̨���
    CAP_WEBFORMAT,               // ֧��WEB��ʽ��

	CAP_WHITE_BALANCE_ONE,       // ��ƽ��1
	CAP_WHITE_BALANCE_TWO,       // ��ƽ��2

	CAP_CHAR_SET_ENCODE,          // �Ƿ�֧�������ַ���

}ECapType; 

// ����ͷ��������
typedef enum 
{
    CAM_FOCUSAUTO = 1,                           // �۽�-�Զ�
    CAM_FOCUSFAR,                                // �۽�-�ֶ�-��Զ
    CAM_FOCUSNEAR,                               // �۽�-�ֶ�-����
    CAM_FOCUSSTOP,                               // �۽�-�ֶ�-ֹͣ
    CAM_MOVEUP,                                  // ����
    CAM_MOVEDOWN,                                // ����
    CAM_MOVEHOME,                                // ��λ
    CAM_MOVELEFT,                                // ����
    CAM_MOVERIGHT,                               // ����
    CAM_MOVESTOP,                                // ֹͣ�˶�
    CAM_ZOOMWIDE,                                // �䱶-��Ұ�Ŵ�
    CAM_ZOOMTELE,                                // �䱶-��Ұ��С
    CAM_ZOOMSTOP,                                // �䱶-ֹͣ
    CAM_PRESETSET,                               // ���Ԥ��λ
    CAM_BRIGHTUP,                                // �������
    CAM_BRIGHTDOWN,                              // �������
    CAM_BRIGHTSTOP,                              // ֹͣ�������
    CAM_PRESETCALL,                              // ��ȡԤ��λ
    
	CAM_PRESETREMOVE                   = 59,     // ɾ��Ԥ��λ
	CAM_TOURSTART                      = 48,     // ��ʼԤ��λѲ��
	CAM_TOUREND                        = 109,    // ����Ԥ��λѲ��
	CAM_STARTAUTOSCAN                  = 24,     // ��ʼˮƽѲ��
	CAM_STOPAUTOSCAN                   = 25,     // ֹͣˮƽѲ��
	CAM_SCANSPEED_SET                  = 72,     // Ѳ���ٶ�
	CAM_COMMAND_GOTOPT                 = 22,     // ���Ķ�λ
	CAM_COMMAND_ZOOMPT                 = 23,     // �ֲ��Ŵ�
	CAM_COMMAND_ZMWHOLE                = 34,     // �ֲ���С
	CAM_DEPTHRATESPEED_ON              = 70,     // ������������ٶ�
	CAM_DEPTHRATESPEED_OFF             = 71,     // �رվ�������ٶ�
	CAM_WIPER_ON                       = 105,    // ��ˢ����
	CAM_WIPER_OFF                      = 106,    // ��ˢ�ر�
	CAM_IR_ON                          = 107,    // ���⿪��
	CAM_IR_OFF                         = 108,    // ����ر�
	CAM_DEFROST_ON                     = 111,    // ��˪����
	CAM_DEFROST_OFF                    = 112,    // ��˪�ر�
	CAM_LASER_ON                       = 113,    // ���⿪��
	CAM_LASER_OFF                      = 114,    // ����ر�
	CAM_RESTORE_FACTORY_SETTINGS       = 110,    // �ָ���������

	CAM_DAYNIGHTMOD_ON                 = 52,     // ��ҹģʽ��
	CAM_DAYNIGHTMOD_OFF                = 53,     // ��ҹģʽ��
	CAM_DAYNIGHTMOD_AUTO               = 54,     // ��ҹģʽ�Զ�
	CAM_WIDEDYNAMIC_ON                 = 50,     // ��̬��
	CAM_WIDEDYNAMIC_OFF                = 51,     // ��̬��
	CAM_BACKLIGHT_ON                   = 87,     // ���ⲹ����
	CAM_BACKLIGHT_OFF                  = 88,     // ���ⲹ����
	CAM_WHITEBALANCEMODE_AUTO          = 102,    // ��ƽ��ģʽ�Զ�
	CAM_WHITEBALANCEMODE_MANUAL        = 103,    // ��ƽ��ģʽ�ֶ�
	CAM_ALL_AUTO                       = 104,    // ��оȫ�Զ�
	CAM_SHUTTERMODE_AUTO               = 98,     // ����ģʽ�Զ�
	CAM_SHUTTERMODE_MANUAL             = 99,     // ����ģʽ�ֶ�
	CAM_PLUSMODE_AUTO                  = 100,    // ����ģʽ�Զ�
	CAM_PLUSMODE_MANUAL                = 101,    // ����ģʽ�ֶ�
	CAM_APERTUREMODE_AUTO              = 94,     // ��Ȧģʽ�Զ�
	CAM_APERTUREMODE_MANUAL_HIGHSPEED  = 95,     // ��Ȧģʽ�ֶ�����(��)
	CAM_APERTUREMODE_MANUAL_LOWSPEED   = 97,     // ��Ȧģʽ�ֶ�����(С)

	CAM_NOOP                                     // �޲���
}ECamControl;

//����״̬,¼�����ػص����
typedef enum
{
    DWL_STATUS_START = 1,             // 1��ʼ����
    DWL_STATUS_ADD_STREAM_ERROR,      // 2д������
    DWL_STASTU_ADD_FRAME_ERROR,       // 3д֡����
    DWL_STATUS_SND_REQ,               // 4������������
    DWL_STATUS_WAIT_STREAM_PROPERTY,  // 5�ȴ�������
    DWL_STATUS_SND_PROPERTY_ACK,      // 6���������Ի���
    DWL_STATUS_WAIT_FRAME_DATA,       // 7�ȴ�֡����
    DWL_STATUS_SND_FRAME_DATA_ACK,    // 8����֡����
    DWL_STATUS_TIMEOUT,               // 9��ʱ
    DWL_STATUS_CLOSE,                 // 10�ر�
    DWL_STATUS_RCV_END,               // 11���ؽ���
    DWL_STATUS_REQ_INVALID,           // 12������ز���(���ͷ���Ϊ��������������ܲ��Ϸ�)
    DWL_STATUS_PROBE,                 //
    DWL_DATA_TYPE,
    DWL_EXCEED_NUM
}EDownloadPuRecStatus;

// ����ԭ��
typedef enum
{
    EPuDropType_PeerDrop,             // ǰ�������Ͽ�
    EPuDropType_NetDrop,              // ������Ч
} EPuDropType;

// ץ��ͼƬ�ֱ���
typedef enum
{
    EPuVideoRes_NONE      =   0x00,
    EPuVideoRes_AUTO      =   0x01,
    EPuVideoRes_QCIF      =   0x02,
    EPuVideoRes_CIF       =   0x03,
    EPuVideoRes_2CIF      =   0x04,
    EPuVideoRes_4CIF      =   0x05,
    EPuVideoRes_QQCIF     =   0x06,
    EPuVideoRes_VGA       =   0x07,
    
    EPuVideoRes_QVGA      =   0x08,
    EPuVideoRes_XVGA      =   0x09,
} EPuVideoRes;

// ǰ��ͨ������
typedef enum
{
    EPuChanType_AudioEnc = 1,
    EPuChanType_AudioDec = 2,
    EPuChanType_VideoEnc = 3,
    EPuChanType_VideoDec = 4,
    EPuChanType_Rec      = 5,
    EPuChanType_Play     = 6,
        
} EPuChanType;

typedef enum
{
    emPuFormatState_IDLE       = 1,
	emPuFormatState_IN_PROCESS = 2,
	emPuFormatState_END        = 3,
} EPuFormatState;

typedef enum
{
    emPuDiskType_HARD_DISK = 1,
	emPuDiskType_SCSI_DISK = 2,
	emPuDiskType_SD_DISK   = 3,
} EPuDiskType;

typedef enum
{
    emPuSubareaUseType_SnapShot = 1,
	emPuSubareaUseType_Record   = 2
} EPuSubareaUseType;

typedef enum
{
    emPuFileSystemType_EXT3 = 1,
	emPuFileSystemType_KFS  = 2,
} EPuFileSystemType;


typedef enum 
{
    EPuLoadDefualt_VideoImageValue = 1,       // ��Ƶͼ������ȡ��Աȶȵ�ֵ
} EPuLoadDefualt;

// PUSDK �ڲ�ʹ�ýṹ�붨��
// ��ʽ����������
typedef enum
{
    EPuDiskFormatType_Invalid          = 0,
	EPuDiskFormatType_FormatAllDisk    = 1, //��������ʽ��ȫ������
	EPuDiskFormatType_FormatDisk       = 2, //��������ʽ��ĳ������
	EPuDiskFormatType_FormatPartition  = 3, //��ʽ������
	EPuDiskFormatType_RepairDisk       = 4, //�����޸�
	EPuDiskFormatType_Defragment       = 5, //������Ƭ����		
} EPuDiskFormatType;

// Ӳ���쳣�ϱ�
typedef enum
{
	EPuRecErrNo                      = 0,       // �޴���
	EPuRecErrWrite		             = 18319,   // д�����
	EPuRecErrRead				     = 18320,   // ������
	EPuRecErrFind				     = 18321,   // ����ʧ��
	EPuRecErrRename			         = 18322,   // ����ʧ��
	EPuRecErrNoformat			     = 18323,   // ����δ��ʽ��
	EPuRecErrReadonly			     = 18324,   // �ļ�ϵͳֻ��
	EPuRecErrPartinvalid		     = 18325    // ����������
	// ��������벻�ڴ˷�Χ��Ϊδ֪����
}EPuRecError;

typedef enum
{
	EPuLoginPuErrNo                  = 0,
	EPuLoginPuErrLoginFailed		 = 18457,   // �û��������벻��ȷ 
	EPuLoginPuErrNoRight			 = 18458,	// û��Ȩ�� 
	EPuLoginPuErrNoLogin			 = 18459,	// �û�δ��¼ 
	EPuLoginPuErrUserlogNumOverload	 = 18460,	// USER�û���¼�ѵ����ֵ 
	EPuLoginPuErrAdminLogin			 = 18461,	// ����Ա�Ѿ���¼ 
	EPuLoginPuErrTimeout			 = 18463,	// ��ʱ 
	// ��������벻�ڴ˷�Χ��Ϊδ֪����
}EPuLoginPuError;


typedef enum 
{
    emPuWifiApInfo_byEncryMode_NONE       = 0,
	emPuWifiApInfo_byEncryMode_WEP        = 1,
	emPuWifiApInfo_byEncryMode_WPA_PSK    = 2,
	emPuWifiApInfo_byEncryMode_WPA2_PSK   = 3,
} EPuWifiApInfo_byEncryMode;

typedef enum 
{
	emPuWifiApInfo_byArithmetic_WPM_Share     = 1,
	emPuWifiApInfo_byArithmetic_WPM_Open      = 2,
	emPuWifiApInfo_byArithmetic_WPM_Auto      = 3,
	emPuWifiApInfo_byArithmetic_WPM_Invalid   = 4,
	emPuWifiApInfo_byArithmetic_AR_None       = 5,
	emPuWifiApInfo_byArithmetic_AR_TKIP       = 6,
	emPuWifiApInfo_byArithmetic_AR_CCMP       = 7,
	emPuWifiApInfo_byArithmetic_AR_WRAP       = 8,
	emPuWifiApInfo_byArithmetic_AR_Invalid    = 9,

} EPuWifiApInfo_byArithmetic;

typedef enum 
{
    emPuWifiApInfo_byVerifyTpye_Invalid       = 0,
	emPuWifiApInfo_byVerifyTpye_PSK           = 1,
	emPuWifiApInfo_byVerifyTpye_Enterprise    = 2,
} EPuWifiApInfo_byVerifyTpye;

typedef enum 
{
    emPuWifiApInfo_byBSSType_Infra    = 1,
	emPuWifiApInfo_byBSSType_Ad_Hoc   = 2,
} EPuWifiApInfo_byBSSType;

typedef enum
{
    emPu3gNetType_EVDO                = 0x01,
	emPu3gNetType_WCDMA               = 0x02,
	emPu3gNetType_TDSCDMA             = 0x04,
} EPu3gNetType;

// ����¼����Ȼص�
typedef void (*PUSDK_LOC_REC_PRO_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned long dwCurrentRectime, void* pContext );

// ����¼��״̬�ص�����Ӧ�Ļص����ͺͽṹ������ʾ    
typedef void (*PUSDK_LOC_REC_STATE_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned long dwCBType, void* pData, int nDataLen, void* pContext );

// �����ļ����Żص�
typedef void (*PUSDK_FILE_STAT_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned char byFileState, unsigned long dwSec, unsigned long dwContext) ;

// PUSDKȫ��ͨ�ûص�
typedef void (*FUNC_MSG_ACK_CB)( PUHANDLE hHandle, int nMsgType, unsigned char byChanId, 
                                    void* pData, int nSize, void* pContext );

// ��Ƶ����ص�
typedef void (*PUSDK_AUDIO_PCM_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned char* pBuf, unsigned long dwPcmLen, 
                                    unsigned long dwSamplerate, unsigned long dwBitWidth, 
                                    unsigned long dwContext );

// ��Ƶ����ص����ص�YUV420���ݸ��û�,�����ֱ�Ϊ:���ݡ����ߣ�pBuf�ĳ���=nWidth*nHeight*3/2
typedef void (*PUSDK_DRAW_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                               unsigned char* pBuf, int nWidth, int nHeight, unsigned long dwContext);

// �ص���������YUV420���ݡ�
// �����ֱ�Ϊ:λͼ����ָ�롢λͼ���ߡ�λ�YUV����ÿ������ռ����λ���û��������Ĳ���
// pBitmapBuf �����ݳ���=dwWidth*dwHeight*dwbitcount/8
typedef void (*PUSDK_FRAMEINFO_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned char *pBitmapBuf, unsigned long dwWidth, unsigned long dwHeight, 
                                    unsigned long dwbitcount,  unsigned long dwContext);

// ������pusdkstruct.h �� 
// typedef void (*PUSDK_FRAME_CB)( PUHANDLE hHandle,PLAYERHANDLE hPlayer, TFrameHeadInfo* pFrmHdr, unsigned long dwContext );

// ���û������PCM���ݱ����Ļص�,�����ֱ�Ϊ:ָ���������ݡ��������ݳ��ȡ����øûص����û������Ĳ��� 
typedef void (*PUSDK_INPUT_PCM_BIT_STREAM_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    void* pbyBitStream, unsigned long dwLen, unsigned long dwContext);

// ���û������PCM���ݱ���󣬴��RTP����Ļص�,�����ֱ�Ϊ:ָ��RTP���ݡ������ݳ��ȡ����øûص����û������Ĳ���
typedef void (*PUSDK_INPUT_PCM_RTP_PACK_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    void* pbyRtpPack, unsigned long dwLen, unsigned long dwContext);

// ¼�����ػص�
typedef void (*PUSDK_DOWN_REC_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned long dwStatusCode, /* EDownloadPuRecStatus */
                                    unsigned long dwCurTime, void* pContext);
// ¼���������ݻص�
typedef void (*PUSDK_DOWN_REC_DATA_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                        unsigned char* pBuf, unsigned long dwLen, void* pContext );

// CMU �汾����ת���ص�
typedef void (*PUSDK_STREAMCB)( PUHANDLE hHandle, unsigned char byChanId, unsigned char* pBuffer,
							    long lLength, void* pContext );

///////////////////////////////////////////////////////////////
// PUSDK ͨ�ûص�֪ͨ���� �� FUNC_MSG_ACK_CB �е� nMsgType ��

// ���ͻ�׼ֵ
#define PUSDK_MSG_CB_BASE                       2000

// ���ǰ��ͼƬ����ص�������ͼƬ������Ϣ 
#define PUSDK_MSG_CB_BROWSE_PUPIC               (PUSDK_MSG_CB_BASE + 1)  // TPuPicFileDownloadRes

// ������Ϣ
#define PUSDK_MSG_CB_DISCONNECTED               2  // TPuUnitInfo

// ������Ϣ�ȴ�Ӧ��ʱ֪ͨ
#define PUSDK_MSG_CB_WAITACK_TIMEOUT            5

// �ϴ�ǰ��ͼƬ����ص�������ͼƬ�ϴ���Ϣ
#define PUSDK_MSG_CB_UPLOAD_PUPIC               728  // TPuBmpOsdAck

// ����ǰ��ͼƬ����ص�������ͼƬ������Ϣ 
#define PUSDK_MSG_CB_DOWNLOAD_PUPIC             754  // TPuPicFileDownloadAck

// ���������Ƶ����Ӧ��
#define PUSDK_MSG_CB_VIDEO_SEND_ADD_ACK         451  // TPuResult

// ���������Ƶ����Ӧ��
#define PUSDK_MSG_CB_AUDIO_SEND_ADD_ACK         453  // TPuResult

// ����ɾ����Ƶ����Ӧ��
#define PUSDK_MSG_CB_VIDEO_SEND_REMOVE_ACK      452  // TPuResult

// ����ɾ����Ƶ����Ӧ��
#define PUSDK_MSG_CB_AUDIO_SEND_REMOVE_ACK      454 // TPuResult

// ǰ��������Ϣ
#define PUSDK_INFORM_REBOOT                    1000 //TPuRebootInfo

// �������
#define PUSDK_5_0_START                        900 
//
#define PUSDK_GET_SYS_ACK                      201         // TPuSysParam
#define PUSDK_SET_SYS_ACK                      401         // TPuResult
#define PUSDK_CONFIG_INFORM_SYS                501
//
#define PUSDK_GET_SYSPUB_ACK                   202         // TPuSysPubParam
#define PUSDK_SET_SYSPUB_ACK                   402
#define PUSDK_CONFIG_INFORM_SYSPUB             502
// 
#define PUSDK_GET_PPPOE_ACK                    204         // TPuPppoeParam
#define PUSDK_SET_PPPOE_ACK                    404 
#define PUSDK_CONFIG_INFORM_PPPOE              504
//
#define PUSDK_GET_VIDEOENC_ACK                 211         // TPuVideoEncParam                
#define PUSDK_SET_VIDEOENC_ACK                 411
#define PUSDK_CONFIG_INFORM_VIDEOENC           511
//
#define PUSDK_GET_VIDEOENC_PUB_ACK             212         // TPuVideoEncPubParam
#define PUSDK_SET_VIDEOENC_PUB_ACK             412
#define PUSDK_CONFIG_INFORM_VIDEOENC_PUB       512
//
#define PUSDK_GET_VIDEOENC_NET_ACK             213         // TPuCommonNetParam
#define PUSDK_SET_VIDEOENC_NET_ACK             413
#define PUSDK_CONFIG_INFORM_VIDEOENC_NET       513
//
#define PUSDK_GET_VIDEOENC_OSD_ACK             214         // TPuVideoEncOsdParam
#define PUSDK_SET_VIDEOENC_OSD_ACK             414
#define PUSDK_CONFIG_INFORM_VIDEOENC_OSD       514
//
#define PUSDK_GET_VIDEOENC_MP_ACK              215         // TPuVideoEncMpParam
#define PUSDK_SET_VIDEOENC_MP_ACK              415
#define PUSDK_CONFIG__CONFIG_INFORM_VIDEOENC_MP  515
//�л���ƵԴ���ش�����
#define PUSDK_GET_VIDEOENC_INPUTPORT_ACK       216         // TPuVideoEncInputPortParam
#define PUSDK_SET_VIDEOENC_INPUTPORT_ACK       416
#define PUSDK_CONFIG_INFORM_VIDEOENC_INPUTPORT 516
//
#define PUSDK_GET_VIDEOENC_POLL_ACK            217         // TPuVideoEncPollParam
#define PUSDK_SET_VIDEOENC_POLL_ACK            417
#define PUSDK__CONFIG_INFORM_VIDEOENC_POLL     517
//
#define PUSDK_GET_VIDEOENC_MULTIENCCHAN_ACK    218          // TPuMultiEncChanParam
#define PUSDK_SET_VIDEOENC_MULTIENCCHAN_ACK    418
#define PUSDK_CONFIG_INFORM_MULTIENCCHAN       518
//
#define PUSDK_GET_VIDEODEC_ACK                 220          // TPuVideoDecParam
#define PUSDK_SET_VIDEODEC_ACK                 421
#define PUSDK_CONFIG_INFORM_VIDEODEC           521
//
#define PUSDK_GET_VIDEODEC_PUB_ACK             221          // TPuVideoDecPubParam
#define PUSDK_SET_VIDEODEC_PUB_ACK             422
#define PUSDK_CONFIG_INFORM_VIDEODEC_PUB       522 
// 
#define PUSDK_GET_VIDEODEC_NET_ACK             222         // TPuCommonNetParam
#define PUSDK_SET_VIDEODEC_NET_ACK             423
#define PUSDK_CONFIG_INFORM_VIDEODEC_NET       523
//
#define PUSDK_GET_VIDEODEC_RETRANSFER_ACK      223         // TPuVideoDecRetransferParam 
#define PUSDK_SET_VIDEODEC_RETRANSFER_ACK      424
#define PUSDK_CONFIG_INFORM_VIDEODEC_RETRANSFER 524
//
#define PUSDK_GET_AUDIOENC_ACK                 231         // TPuAudioEncParam
#define PUSDK_SET_AUDIOENC_ACK                 431
#define PUSDK_CONFIG_INFORM_AUDIOENC           531
//
#define PUSDK_GET_AUDIOENC_NET_ACK             232         // TPuCommonNetParam
#define PUSDK_SET_AUDIOENC_NET_ACK             432
#define PUSDK_CONFIG_INFORM_AUDIOENC_NET       532
//
#define PUSDK_GET_AUDIODEC_ACK                 233         // TPuAudioDecParam
#define PUSDK_SET_AUDIODEC_ACK                 433
#define PUSDK_CONFIG_INFORM_AUDIODEC           533
//
#define PUSDK_GET_AUDIODEC_NET_ACK             234        // TPuCommonNetParam
#define PUSDK_SET_AUDIODEC_NET_ACK             434
#define PUSDK_CONFIG_INFORM_AUDIODEC_NET       534
//
#define PUSDK_GET_SERIALPORT_ACK               241         // TPuSerialPortParam
#define PUSDK_SET_SERIALPORT_ACK               441
#define PUSDK_CONFIG_INFORM_SERIALPORT         541
//
#define PUSDK_GET_SERIALPORT_PUB_ACK          242        // TPuSerialPortPubParam
#define PUSDK_SET_SERIALPORT_PUB_ACK          442
#define PUSDK_CONFIG_INFORM_SERIALPORT_PUB    542
//
#define PUSDK_GET_VIDEOINPUT_ACK              245        // TPuVideoInputParam
#define PUSDK_SET_VIDEOINPUT_ACK              445
#define PUSDK_CONFIG_INFORM_INPUTPIN          543
//
#define PUSDK_GET_OSD_SCROLL_ACK              248        // TPuOsdScroll
#define PUSDK_SET_OSD_SCROLL_ACK              461
#define PUSDK_CONFIG_INFORM_OSDSCROLL         561
//
#define PUSDK_GET_VIDEOSHADE_ACK              247        // TPuVideoShadeParam 
#define PUSDK_SET_VIDEOSHADE_ACK              447
#define PUSDK_CONFIG_INFORM_SHADE             547
//
#define PUSDK_GET_NMS_ACK                     42         // TPuNmsParam
#define PUSDK_SET_NMS_ACK                     44
#define PUSDK_CONFIG_INFORM_NMS               548
//
#define PUSDK_GET_SNAPSHOT_CFG_ACK            716        // TPuSnapshotConfig
#define PUSDK_SET_SNAPSHOT_CFG_ACK            718
#define PUSDK_CONFIG_INFORM_SNAPSHOT          549
//
#define PUSDK_GET_SYSEXT_ACK                  (2  + PUSDK_5_0_START) // TPuSysExtParam
#define PUSDK_SET_SYSEXT_ACK                  (4  + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_SYSEXT            (5  + PUSDK_5_0_START)
//
#define PUSDK_GET_P2P_INFO_ACK                (7  + PUSDK_5_0_START) // TPuVideoDecPpParam
#define PUSDK_SET_P2P_INFO_ACK                (9  + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_P2P               (10 + PUSDK_5_0_START)
//
#define PUSDK_GET_RECCHAN_ACK                 (12 + PUSDK_5_0_START) // TPuRecChannel
#define PUSDK_SET_RECCHAN_ACK                 (14 + PUSDK_5_0_START) 
#define PUSDK_CONFIG_INFORM_RECCHAN           (15 + PUSDK_5_0_START)
//
#define PUSDK_GET_RECSYS_ACK                  (17 + PUSDK_5_0_START) // TPuRecSys
#define PUSDK_SET_RECSYS_ACK                  (19 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_RECSYS            (20 + PUSDK_5_0_START)

// �������ƽ̨ʹ�� begin
#define PUSDK_RECCHAN_BASICINFO_GET_ACK       46 // TPuRecChanBasicInfo
// ������Set��ʱ��ʹ�õ����ݽṹ��TPuRecChannel,��������emPuConfig_RecChan
#define PUSDK_RECCHAN_BASICINFO_SET_ACK       48 // TPuResult

#define PUSDK_RECCHAN_TIMELYINFO_GET_ACK      50 // TPuRecChannel
#define PUSDK_RECCHAN_TIMELYINFO_SET_ACK      52 // TPuResult
 
#define PUSDK_RECSYS_INFO_GET_ACK             58 // TPuRecSys
#define PUSDK_RECSYS_INFO_SET_ACK             60 // TPuResult

// 
#define PUSDK__RECCHAN_PERIODLYINFO_GET_ACK   54 // TPuRecChannel
#define PUSDK__RECCHAN_PERIODLYINFO_SET_ACK   56 // TPuResult
// end

//
#define PUSDK_GET_RECSYSEX_ACK                (22 + PUSDK_5_0_START) // TPuRecSysEx
#define PUSDK_SET_RECSYSEX_ACK                (24 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_RECSYSEX          (25 + PUSDK_5_0_START)
//
#define PUSDK_GET_SNAPSHOTSTORE_CFG_ACK       (112+ PUSDK_5_0_START) // TPuSnapshotStoreConfig
#define PUSDK_SET_SNAPSHOTSTORE_CFG_ACK       (114+ PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_SNAPSHOTSTORE_CFG  550
//
#define PUSDK_GET_CAM_CFG_ACK                 (32 + PUSDK_5_0_START) // TPuCameraCfg
#define PUSDK_SET_CAM_CFG_ACK                 (34 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_CAM_CFG            35
//
#define PUSDK_GET_INPUTPIN_ACK                (42 + PUSDK_5_0_START) // TPuInputPinParam
#define PUSDK_SET_INPUTPIN_ACK                (44 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_INPUTPIN           543
//
#define PUSDK_GET_MOTIONDETECT_ACK            (47 + PUSDK_5_0_START) // TPuMotionDetectParam
#define PUSDK_SET_MOTIONDETECT_ACK            (49 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_MOTIONDETECT       546
//   
#define PUSDK_GET_OUTPUTPIN_ACK               (52 + PUSDK_5_0_START) // TPuOutputPinParam
#define PUSDK_SET_OUTPUTPIN_ACK               (54 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_OUTPUTPIN          544

#define PUSDK_GET_DDNS_ACK                    (57 + PUSDK_5_0_START) // TPuDdnsParam
#define PUSDK_SET_DDNS_ACK                    (59 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_DDNS               60
//
#define PUSDK_GET_EXTCARD_ACK                 (62 + PUSDK_5_0_START) // TPuExtCardParam
#define PUSDK_SET_EXTCARD_ACK                 (64 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_EXTCARD            65
//
#define PUSDK_GET_DAQDEV_ACK                  (123+ PUSDK_5_0_START)        // TPuDAQDeviceParam
#define PUSDK_SET_DAQDEV_ACK                  (125+ PUSDK_5_0_START)        // TPuResult
#define PUSDK_CONFIG_INFORM_DAQDEV             563
//
#define PUSDK_GET_WIRELESS_MODE_ACK           249    // TPuWirelessModeParam
#define PUSDK_SET_WIRELESS_MODE_ACK           462    // TPuResult
#define PUSDK_CONFIG_INFORM_WIRELESS_MODE     562
//
#define PUSDK_GET_VERUPDATEINFO_ACK           (96+ PUSDK_5_0_START) // TPuVerUpdateInfo
#define PUSDK_SET_VERUPDATEINFO_ACK           (98+ PUSDK_5_0_START) // TPuResult

// �����ȵ���Ӧ���������������ȵ����
#define PUSDK_GET_WIFI_CFG_RESP                (2 + 1100) // TPuWifiCfgGetResp
// �����ȵ���Ϣ�ϱ� 
#define PUSDK_REPORT_WIFI_AP_CFG               (3 + 1100) // TPuWifiApInfo 
// ���AP�㷵�ؽ��
#define PUSDK_ADD_WIFI_AP_RESP                 (5 + 1100) // TPuResult
// ɾ��AP�㷵�ؽ��
#define PUSDK_REMOVE_WIFI_AP_RESP              (7 + 1100) // TPuResult
// ���ӷ��ؽ��
#define PUSDK_SET_WIFI_CONNECTION_ACK          (9 + 1100) // TPuResult
// ���ӷ���״̬��Ϣ�ϱ�
#define PUSDK_REPORT_WIFI_CONNECTION_STATE     (10 + 1100) // TPuWifiConnectionStatus
// ��ȡWIFI�������ý��
#define PUSDK_GET_WIFI_NET_CFG_RESP            (12 + 1100) // TPuWifiNetParam
// ����WIFI�������ý��
#define PUSDK_SET_WIFI_NET_CFG_RESP            (14 + 1100) // TPuResult
// WIFI���øı��ϱ�
#define PUSDK_CONFIG_INFORM_WIFI_NET_CFG        567

//
#define PUSDK_GET_3G_CFG_RESP                 (2 + 1150)     // TPu3gCfgParam
#define PUSDK_SET_3G_CFG_RESP                 (4 + 1150)     // TPuResult
#define PUSDK_CONFIG_INFORM_3G_CFG             578
// 
#define PUSDK_GET_3G_NET_CFG_RESP             (7 + 1150)  // TPu3gNetCfg
#define PUSDK_SET_3G_NET_CFG_RESP             (9 + 1150)  // TPuResult
#define PUSDK_CONFIG_INFORM_3G_NET_CFG        568

//
#define PUSDK_GET_GPS_CFG_ACK                 (2 + 1200)     // TPuGpsCfgParam
#define PUSDK_SET_GPS_CFG_ACK                 (4 + 1200)     // TPuResult
#define PUSDK_CONFIG_INFORM_GPS_CFG            579
//
#define PUSDK_GET_LOCALIMAGE_OUTPUT_PARAM_ACK (136 + PUSDK_5_0_START) // TPuLocalImageOutputParam
#define PUSDK_SET_LOCALIMAGE_OUTPUT_PARAM_ACK (138 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_LOCALIMAGE_OUTPUT  564
//
#define PUSDK_GET_IMAGE_DISPLAY_PARAM_ACK     (140 + PUSDK_5_0_START) // TPuImageDisplayParam
#define PUSDK_SET_IMAGE_DISPLAY_PARAM_ACK     (142 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_IMAGE_DISPLAY      565
//
#define PUSDK_GET_VIDEO_DECCOMP_PARAM_ACK     (144 + PUSDK_5_0_START) // TPuVideoDecCompParam
#define PUSDK_SET_VIDEO_DECCOMP_PARAM         (146 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_VIDEO_DECCOMP      566
// end ����

// ��ȡǰ��¼����
#define  PUSDK_REC_QUERY_NUM_ACK                 82      // TPuRecQueryNumAck

//¼���ļ����� KDM_SearchPuRec
#define  PUSDK_REC_QUERY_FILE_ACK                84      // TPuRecQueryFileAck

// ����¼���ʱ��ǰ�˷��ص�¼���ID pleyerid
#define  PUSDK_PLAY_PLAYERID_INFORM              93       // BYTE

// ����ǰ��¼��Ӧ�� KDM_StartPlayPuRec
#define  PUSDK_PLAY_START_ACK                    92       // TpuResult

// ǰ��¼�񲥷Ž�����Ϣ KDM_StartPlayPuRec
#define PUSDK_MSG_CB_PLAY_PRG                    94       // TPuPlayProgress

// ǰ��¼����Ʋ���Ӧ�� KDM_PuRecPlayOperation ��StopPlayPuRec
#define PUSDK_PLAY_OPERATION_ACK                 98       // TPuResult    

//ǰ��¼�񲥷��϶�����Ӧ�� KDM_PuRecSeekLocation
#define PUSDK_PLAY_SEEK_ACK                      96       // TPuResult

// ��ȡϵͳ��ϢӦ�� KDM_GetSysInfo
#define PUSDK_GET_SYSINFO_ACK                    1010     // TPuSysInfo

// ʱ��ͬ��Ӧ�� KDM_SyncPuTime
#define PUSDK_SET_SYSTIME_ACK                    403      // TPuResult

// ��ѯǰ��ͼƬӦ�� KDM_SearchPuPic
#define PUSDK_PICFILE_QUERY_ACK                  742      // TPuPicFileQueryAck

// ɾ��ǰ��ͼƬӦ�� KDM_DeletePuPic
#define PUSDK_PICFILE_DEL_ACK                    745      // TPuPicFileDelAck

// ǰ��ͼƬ����Ӧ�� KDM_DownloadPuPic
#define PUSDK_PICFILE_DOWNLOAD_ACK              20000     // TPuResult

// �ϴ�ͼƬӦ�� UploadTitlePic
#define PUSDK_UPLOAD_TITLE_PIC_ACK              20010     // TPuResult

// ��ȡǰ��״̬ KDM_GetChanState
#define PUSDK_GET_CHANSTATE_ACK                 1008      // TPuChanStateAck

// ��ʼǰ���ֶ�¼��Ӧ�� KDM_StartPuRec
#define PUSDK_REC_START_ACK                     74       // TPuResult

// ֹͣǰ���ֶ�¼��Ӧ�� KDM_StopPuRec
#define PUSDK_REC_STOP_ACK                      76       // TPuResult

// ��Ȩ��������KDM_LoginPu
#define PUSDK_IDENTIFY_USERPASSWORD_ACK         702      // TPuUserPasswordIdentifyAck

// ���� GPS ����ACK KDM_GpsSubScribe
#define PUSDK_CREATE_SUBSCRIBE_ACK              782      // TPuResult

// ��Ƶ��ʧ�ϱ�
#define PUSDK_VIDEO_IN_STATE_CHANGED            649      // TPuVideoInState

// �ƶ�����ϱ�
#define PUSDK_EN_MOTD_STATE_CHANGED             642      // TPuMotionDetAlarm

// Ӳ���쳣�ϱ� 
#define PUSDK_REC_ERROR_NOTIFY                  653      // TPuRecErrorNotify

// �������� 
#define PUSDK_INPUTPIN_STATE_CHANGED            641      // TPuInputPinAlarm

// ���������ϱ�
#define PUSDK_COM_DATA_INFORM                   693      // TPuComData

// GPS �ϱ�����
#define PUSDK_GPS_DATA_NOTIFY_EX                655  // TPuGpsNotifyParam

#endif



