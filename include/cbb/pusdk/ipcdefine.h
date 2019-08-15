//////////////////////////////////////////////////////////////////////////
// ipcdefine.h 
#ifndef IPCDEFINE_H_
#define IPCDEFINE_H_

#pragma pack(1)

/***************************************************************************************
					���Ͷ���
****************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	  /* Type definition */
/*-----------------------------------------------------------------------------
ϵͳ�����ļ���������Ա�Ͻ��޸�
------------------------------------------------------------------------------*/

typedef int		s32,BOOL32;
typedef unsigned long   u32;
typedef unsigned char	u8;
typedef unsigned short  u16;
typedef short           s16;
typedef char            s8;

#ifdef _MSC_VER
typedef __int64			s64;
#else 
typedef long long		s64;
#endif 

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else 
typedef unsigned long long u64;
#endif

#ifndef _MSC_VER
#ifndef LPSTR
#define LPSTR   char *
#endif
#ifndef LPCSTR
#define LPCSTR  const char *
#endif
#endif



#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif


#ifdef __cplusplus
}
#endif // __cplusplus


#ifndef CDECL
#ifdef WIN32
#define CDECL  __cdecl
#else
#define CDECL
#endif
#endif

/********************************************************************************/

/**********************************MAXMIN VALUE************************************/
#define MAX_USER_NAME                64   // ����û�������(64λ)
#define MAX_USER_PASSWORD            64   // ����û����볤��(64λ)
#define MAX_DEV_NAME				 64   // ����豸���Ƴ���(64λ)
#define MAX_PROTOCOL_NAME			 32   // ���Э�����Ƴ���(32λ)


#ifndef MAX_PATH
#define MAX_PATH 260
#endif

/************************************errcode************************************/					
#define RET_FAILD                       (0)    // ʧ��
#define RET_SUCCESS                     (1)    // �ɹ�����
#define ERR_NODLL						(2)    // û���ҵ�SDKDLL
#define ERR_NOFUN						(3)    // û���ҵ�SDKDLL��Ӧ�ӿ�
#define ERR_INITFAILD                   (4)    // ��ʼ��ʧ��
#define ERR_RELEASE                  	(5)    // �ͷ�ʧ��
#define ERR_AUCFAILD					(6)    // ��ȡ��Ȩ��Ϣʧ��
#define ERR_MEMERYFAILD					(7)    // ��ȡ�ڴ�ʧ��
#define ERR_PARAM						(8)    // ��������
#define ERR_HTTPSEND					(9)	   // http����ʧ��
#define ERR_ANALVZE						(10)   // ������������ʧ��
#define ERR_UPLOADING					(11)   // �豸��������
#define ERR_NOLOGIN					    (12)   // û��ע��
#define ERR_NODEFINE                    (13)   // ����ģ��û�ж���

#define ERR_UNKNOW						(201)  //CGI Appδ֪����
#define ERR_USERNAME					(202)  //�û�������(���ķ�����û�������)
#define ERR_NOT_AUTH					(203)  //�û���Ϣδ��֤
#define ERR_PASS						(204)  //�û��������
#define ERR_AUTHID						(205)  //��ȨID����
#define ERR_NO_POWER					(206)  //û��Ȩ��ִ�в���
#define ERR_IP_DENY					    (207)  //IP���ܾ�
#define ERR_OLDPASS				    	(208)  //���������
#define ERR_USER_EXIST					(209)  //�û��Ѵ���
#define ERR_URL_NO_SUPPORT				(210)  //��CGI����֧��

#define ERR_NEED_BODY                   (211)  //����û��Я����Ϣ��
#define ERR_XML                     	(212)  //CGI����Я����XML��ʽ����
#define ERR_XML_ROOT                	(213)  //CGI����Я��XML���������URL��XML�ڵĸ��ڵ㲻ƥ�䣩
#define ERR_PARAM_LOST                  (214)  //CGI����Я����XML��ȱ�ٱ�Ҫ�Ĳ�����XMLԪ�أ�
#define ERR_PARMM_TOOLONG               (215)  //XML��Я���Ĳ�������
#define ERR_CHECK                   	(216)  //������Ϣ������
#define ERR_FILE                    	(217)  //�����ļ�������, �ط���ǰ�İ�
#define ERR_UPGRADE_FAILURE             (218)  //�豸����ʧ��
#define ERR_XML_PARAM                   (219)  //XML��������
#define ERR_IP                      	(220)  //IP��ַ��ʽ����
#define ERR_MASK                    	(221)  //IP�������
#define ERR_GATEWAY                 	(222)  //IP���ش���
#define ERR_DNS                     	(223)  //DNS��ַ����
#define ERR_MULTICAST               	(224)  //�鲥��ַ����
#define ERR_MTU                     	(225)  //MTU��������
#define ERR_SEQ                     	(226)  //����ʱ�����
#define ERR_URL                     	(227)  //�����URL��ʽ����
#define ERR_INVALID_ARG                 (228)  //��Ч������
#define ERR_DEVICE_BUSY                 (229)  //�豸��æ
#define ERR_RECOVER                     (230)  //�豸�ָ�
#define ERR_USER_BANNED                 (231)  //�û�����ֹ��¼
#define ERR_USER_ACTIVE                 (232)  //�û���Ҫ����
#define ERR_NEED_REQUERY                (233)  //��Ҫ���²�ѯ

#define ERR_NOFORMAT                    (2509) // ǰ������¼���ܸ�ʽ��

/********************************************************************************/

/************************************event************************************/	

#define EVT_PTZ_BEGIN                  	(0)    			// PTZ�¼�������ʼλ��				
#define EVT_PTZ_FOUCUS                  (EVT_PTZ_BEGIN + 1)    // PTZ�����¼�


/********************************************************************************/

/************************************cap************************************/	

#define CAP_ANONYM                  	"anonym"    	// �����������
#define CAP_HELP                  		"help"    		// ����
#define CAP_VIDEOSOURCENUM              "videosourcenum"// ��ƵԴ����
#define CAP_STREAMNUM                  	"streamnum"    	// ����·��
#define CAP_AUDIOENC                  	"audioenc"    	// ��Ƶ��������
#define CAP_AUDIODEC                  	"audiodec"    	// ��Ƶ��������
#define CAP_REC                  		"rec"    		// ֧��¼����
#define CAP_IMAGE                  		"image"    		// ֧��ͼ����ڹ���
#define CAP_BRIGHTNESS                  "brightness"    // ����
#define CAP_CONTRAST                  	"contrast"    	// �Աȶ�
#define CAP_SATURATION                 	"saturation"    // ���Ͷ�
#define CAP_HUE                 		"hue"    		// ���
#define CAP_FOCUS                 		"focus"    		// ֧�־۽�
#define CAP_WHITEBALANCE                "whitebalance"  // ��ƽ�� 
#define CAP_DYNAMICMODE                 "dynamicmode"   // ֧�ֶ�̬����
#define CAP_IMAGEENHANCE                "imageenhance"  // ͼ����ǿ
#define CAP_WDR                 		"wdr"    		// ��̬
#define CAP_BLC                 		"blc"    		// ǿ������
#define CAP_HLC                 		"hlc"    		// ���ⲹ��
#define CAP_FOGTHROUGH                 	"fogthrough"    // ͸��
#define CAP_GAMMA                 		"gamma"    		// Gamma
#define CAP_SMARTIR                 	"smartir"    	// SmartIR
#define CAP_IRIS                 		"iris"    		// ��Ȧ
#define CAP_IRCUTFILTER                 "ircutfilter"   // ��ҹ�л�
#define CAP_SHUTTER                 	"shutter"    	// ֧�ֿ���
#define CAP_GAIN                 		"gain"    		// ����
#define CAP_CORRIDORMODE                "corridormode"  // ֧������ģʽ
#define CAP_IMAGEMODE                 	"imagemode"    	// ����ģʽ
#define CAP_NOISEREDUCE2D               "noisereduce2d" // 2D���� 
#define CAP_NOISEREDUCE3D               "noisereduce3d" // 3D����
#define CAP_LOCALECHO                 	"localecho"    	// ֧�ֱ��ػ���
#define CAP_HDSDI                 		"hdsdi"    		// HDSDI
#define CAP_POWERLINEFREQUENCY          "powerlinefrequency"// ����˸
#define CAP_IMAGETRUN                 	"imagetrun"    	// ͼ��ת
#define CAP_ENCCLIP                 	"encclip"    	// ����ü�
#define CAP_ROI                 		"roi"    		// ��������
#define CAP_VIDEOSHILD                 	"videoshild"    // ��Ƶ�ڱ�
#define CAP_QOS                 		"qos"    		// Qos
#define CAP_AUDIODEC                 	"audiodec"    	// ��Ƶ����
#define CAP_AUDIOENC                 	"audioenc"    	// ��Ƶ����
#define CAP_OSD                 		"osd"    		// ��Ļ
#define CAP_DYNAMICOSD                 	"dynamicosd"    // ��̬OSD
#define CAP_MTCF                 		"mtcf"    		// mtcf ����ģʽ
#define CAP_PTZ                  		"ptz"    		// ��̨
#define CAP_PRESET                 		"preset"    	// Ԥ��λ
#define CAP_WATCHON                 	"watchon"    	// ��̨����
#define CAP_TIMETASK                 	"timetask"    	// ��ʱ����
#define CAP_PATHCRUISE                 	"pathcruise"    // Ѳ��ɨ��
#define CAP_PATTERNS                 	"patterns"    	// ����ɨ��
#define CAP_LIMIT                 		"limit"    		// ��λ
#define CAP_LASER                 		"laser"    		// ����
#define CAP_WIPER                 		"wiper"    		// ��ˢ, 
#define CAP_SHAKE                 		"shake"    		// ����, 
#define CAP_ESHAKE                 		"eshake"    	// ���ӷ���
#define CAP_DEFRONST                 	"defronst"    	// ��˪,
#define CAP_ASSFOCUS                 	"assfocus"    	// ��������
#define CAP_INFRARED                 	"infrared"    	// ���� 
#define CAP_ANALYZE                 	"analyze"    	// ��Ƶ����
#define CAP_DETECT                 		"detect"    	// �ƶ��������������0��֧��
#define CAP_SHIELDALARM                 "shieldalarm"   // �ڱα�������������0��֧��
#define CAP_ALARMIN                 	"alarmin"    	// ��������������0��֧��
#define CAP_ALARMOUT                 	"alarmout"    	// �������������0��֧��
#define CAP_EXCEPTION                 	"exception"    	// �쳣����  
#define CAP_MULCARD                 	"mulcard"    	// ��������
#define CAP_WIRELESS                 	"wireless"    	// ��֧������
#define CAP_PPPOE                 		"pppoe"    		// PPPoE֧��
#define CAP_DDNS                 		"ddns"    		// DDNS֧��
#define CAP_8021X                 		"8021x"    		// 8021X֧��
#define CAP_VISP                 		"visp"    		// Kedavsip
#define CAP_ONVIF                 		"onvif"    		// Onvif
#define CAP_GB28181                 	"gb28181"    	// Gb28181
#define CAP_KSNMP                 		"ksnmp"    		// Ksnmp
#define CAP_IPADDR                 		"ipaddr"    	// ip
#define CAP_WIFI                 		"wifi"    		// wifi
#define CAP_BLUETOOTH                 	"bluetooth"    	// ����
#define CAP_AUTOMAINTAIN                "automaintain"  // �Զ�ά��
#define CAP_FIND                 		"find"    		// ����
#define CAP_RS232                 		"rs232"    		// rs232
#define CAP_RS485                 		"rs485"    		// rs485
#define CAP_STORAGE                 	"storage"    	// �洢������
#define CAP_SNAPPIC                 	"snappic"    	// ץͼ
#define CAP_LOG                			"log"    		// ��־
#define CAP_SECURITY                 	"security"    	// ��ȫ



/********************************************************************************/

/***********************************enum************************************/

// �����ֱ���
enum ERESOLUTION 
{
	ERESOLUTION_1024_768,		// 1024�� 768
	ERESOLUTION_1920_1280,		// 1920�� 1280
};


// UDP��Ƶ�������
enum emStreamType
{
	em_streamtype_vedio = 0,	// ��Ƶ
	em_streamtype_audio,		// ��Ƶ
	em_streamtype_both,			// ��Ƶ + ��Ƶ
	em_streamtype_ps			// PS��
};

// ���д�������
enum emTransType
{
	em_transtype_rtp_udp = 0,	// rtp-udp
	em_transtype_rtp_tcp,		// rtp-tcp
	em_transtype_rtsp_tcp,		// rtsp-tcp
	em_transtype_rtsp_udp		// rtsp-udp
};

// ������������
enum emPlayVideoType
{
	type_udp =  0,     // udp
	type_tcp,          // tcp
	type_unknow,       
};


// ����״̬
enum emConnectState
{
	em_connect_ok = 0,	// ���ӳɹ�
	em_connect_no,		// ����ʧ��
};

// IP���䷽ʽ
enum emIpAddrType
{
	em_ipaddr_static = 0,//��̬
	em_ipaddr_dymatic,   //��̬
};

// IP��ַ������
enum emIPAddressType
{
	em_ipaddrtype_ip = 0,		// ���ͣ�dwordֵ
	em_ipaddrtype_url			// url��ַ���ַ���
};
// ����¼���ļ������ʽ
enum emLocalRecType
{
	em_localrectype_asf = 0,     //asf��ʽ
};
// ͼƬ�������� 
enum emPictureType
{
	
	em_picture_jpeg = 0,// jpeg��ʽ
	em_picture_bmp,		// bmp��ʽ
};
// ¼���¼�����
enum emRecType
{
	em_record_alarm = 0, //�¼�¼��
	em_record_time,      //ʱ��¼��
	em_record_manual,    //�ֶ�¼��
};
// ¼�񲥷��ٶ�
enum emRecPlayRate
{
	em_recplay_rate1 = 0,// ��ʼ�ٶ�
	em_recplay_rate2,    // 2���ٶ�
	em_recplay_rate4,    
	em_recplay_rate8,
	em_recplay_rate16,
	em_recplay_rate1_2,  // 1/2���ٶ�
	em_recplay_rate1_4,
	em_recplay_rate1_8,
};

// �Զ�Уʱģʽ
enum EAUTOTIMEMODE 
{
	em_autotime_ntp = 0,			// 1024�� 768
	em_autotime_inputprotocol,			// 1920�� 1280
};
/********************************************************************************/

/**********************************����״̬t************************************/
enum emUploadState
{
	em_uploadstate_idle = 0,     // ����
	em_uploadstate_checkdata,    // У��
	em_uploadstate_pre,			 // ׼������
	em_uploadstate_uploading,	 // ��������
};

/********************************************************************************/

/***********************************port************************************/
#define PORT_HTTP                  (80)   // http�˿�
#define PORT_RTSP                  (554)  // http�˿�
#define PORT_PRINT                 (3700)  // ���ش�ӡ�˿�


/********************************************************************************/

/**********************************default************************************/
#define DEF_                  (80)   // 


/********************************************************************************/


/**********************************Normal define***********************************/
#define NORMAL_BEGIN            0   // 
#define VIEW_TCP               	(NORMAL_BEGIN + 2)   // TCP
#define VIEW_UDP                (NORMAL_BEGIN + 3)   // UDP���
#define REC_TCP               	(NORMAL_BEGIN + 2)   // TCP
#define VIEW_UDP                (NORMAL_BEGIN + 3)   // UDP���

/*********************************IPCSDK�ص�ԭ�Ͷ���e***********************************/


/*=================================================================
��������: cbfConnectDetect
��    ��: ����̽��ص��ص�    ��IPC_AddConnectDetect
����˵��: 
		  dwIP		-- �豸IP 
		  wPort		-- �豸http�˿� 
		  dwCBconnectType	-- ����״̬ emConnectState
		  dwData	-- �ش�����
		  dwDataLen -- �ش����ݳ���
		  pContext	-- ������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
#ifdef WIN32
typedef void (__stdcall *cbfConnectDetect)(u32 dwIP, u16 wPort, u32 dwHandle,u32 dwCBconnectType, u32 dwDataLen, u32 dwData, void* pContext);
#else
typedef void (CDECL *cbfConnectDetect)(u32 dwIP, u16 wPort, u32 dwHandle,u32 dwCBconnectType, u32 dwDataLen, u32 dwData, void* pContext);
#endif



#pragma pack()

#endif
