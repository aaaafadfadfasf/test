/*****************************************************************************
   ģ����      : cms_errorcode
   �ļ���      : cms_errorcode.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cms_errorcode.h �������д�����
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/22   1.0         liangli        ����
******************************************************************************/

#ifndef _CMS_ERRORCODE_H_
#define _CMS_ERRORCODE_H_

/************************************************************************/
//�����붨����֪
//1���������������������д�������ע�ͣ����Ҿ������꾡
//2����������δ֪����ϵͳ����Ⱥ�������Ĵ�����
//3��һ�ִ�����ֻ��������һ�ֳ�����������CU������PU�����������ִ����룬��ֹ����
//4��Ϊ�˷�ֹ����������ϸ��ͬʱ�û�Ҳ�����ľ�����󣬿ɶ�ĳһ������Ĵ�������й��࣬ȡһ�������Դ���
//5. Ϊ���Ǵ����벻�ܱ������޸ģ�ö��ֵ����Ҫֱ��д��Ӧ����ֵ
/************************************************************************/

typedef int CMS_RESULT;

enum ECMS_ERRORCODE
{
    CMS_SUCCESS = 0,                       //�����ɹ�,�ɹ�����Ҫ���ÿ�����������壬���в�������

    //������һЩ������
    ERR_COMMON_INVALID_DEV_URI = 1,         //��Ч��dev uri
    ERR_COMMON_DEV_ALREADY_LOGIN= 2,        //��ͬuri��dev�Ѿ���¼
	ERR_SIP_BODY_NULL = 3,				    //SIP��Ϣ��Ϊ��
    ERR_POST_MSG_FAILED = 4,                //������Ϣʧ��
    ERR_INVALIAD_REQ_PARAM = 5,             //��Ч���������
    ERR_INVALIAD_SDP = 6,                   //��Ч��ý��������Ϣ
    ERR_DIALOG_TERMINATE = 7,               //�Ի����ж�
    ERR_MKDIR_FAILED = 8,                   //����Ŀ¼ʧ��
    ERR_INVALID_SESSIONID = 9,              //��Ч�ĻỰID��ָ���ĻỰ������
    ERR_REQUIRE_TOO_MUCH = 10,              //ͬ��������࣬����ҵ���޶�
    ERR_OLD_PASSWORD_INCORRECT = 11,        //���������
	ERR_OLD_PASSWORD_EQUAL_NEW_PASSWORD = 12, //�¾�������ͬ
	

    //������sip������룺4xx-6xx��������β���sip��׼���壬���ܱ����ģ��ռ�á�
    //��Щ������ȫ�ֹ�����ģ�������ٴζ���
    //���õ�sip�����붨�����£�������������
    // 4xx Client Error
    ERR_SIP_403_FORBIDDEN        = 403,  // 403 ��ֹ����
    ERR_SIP_404_NOT_FOUND        = 404,  // 404 �Ҳ���Ŀ��
    ERR_SIP_408_REQ_TIMEOUT      = 408,  // 408 ����ʱ
    ERR_SIP_423_INTERV_TOO_BRIEF = 423,  // 423 �������Ƶ��
    ERR_SIP_486_BUSY_HERE        = 486,  // 486 Busy Here    
    // 5xx Server Error
    // 6xx Global Error

    //3as������ 10000-10999(��ǰ���ú�Ϊ10056)
    ERR_3AS_BEGIN = 10000,                  //3as��������ʼֵ

    ERR_TAS_INVALID_PARM = 10001,        // �����Ƿ�, ��ϸ����μ���̨��ӡ�����������Ҫ���ڳ���Ա
    ERR_TAS_SHELL_CMD_FAILED = 10042,    // shell����ִ��ʧ��
    ERR_TAS_HANDLE_IN_PROGRESS = 10002,  // ��ʾһ���������ڴ�������У��û������ظ��ύ����
    ERR_TAS_NOT_FINISHED = 10055,        // ������δ��ɣ�����ͨ����ؽӿڻ�ȡ���� 
    ERR_TAS_INVALID_OPERATION = 10003,   // �Ƿ�������ҵ���ϲ�������˲���
    ERR_TAS_SYSTEM_BUSY = 10024,         // ϵͳ��æ�����Ժ�����
    ERR_TAS_ULTRA_VIRES = 10028,         // ԽȨ��Ϊ
    ERR_TAS_EXIST_SAMENAME_RECORD = 10029,  // ����ͬ����¼
    ERR_TAS_FIELD_CONTENT_EMPTY = 10031,    // �ֶ�����Ϊ��
    ERR_TAS_CURRENT_USER_NOT_EXIST = 10030, // ��ǰ�����߲����ڣ��Ự�����ڻ����û�������

    ERR_TAS_SYSTEM_UNINITED = 10004,   // ϵͳδ��ʼ��
    ERR_TAS_DBTASK_POOL_FULL = 10005,  // DB�������
    ERR_TAS_DBERROR = 10006,           // ���ݿ��������������һ��
    ERR_TAS_RECORD_EXISTS = 10007,     // ���ݿ��¼�Ѿ�����
    ERR_TAS_RECORD_NOT_EXISTS = 10008, // ���ݿ��¼������
    ERR_TAS_RECORD_FETCH_FAILED = 10009, // ���ݿ��¼��ȡʧ��, �����Ǽ�¼�Ѿ���������

    ERR_TAS_LOGINED = 10010,           // ��ͬ��ʶ�Ŀͻ����Ѿ���¼TAS
    ERR_TAS_USER_NOT_EXIST = 10011,    // �û�������
    ERR_TAS_INVALID_PASSWORD = 10012,  // �������
    ERR_TAS_USER_EXPIRED = 10025,      // �û�����
    ERR_TAS_USER_DISABLE_MULTILOGIN = 10026,  // ��ֹ��ε�¼
    ERR_TAS_USER_TYPE_DISABLE_CHANGE = 10027, // �û����Ͳ������޸�, ������ڶ����û����û������ܸ�Ϊ����Ա

    ERR_TAS_DECODER_ALREADY_BINDED = 10033,   // �������Ѿ�����

    ERR_TAS_LICENSE_KEY_ERROR = 10013,  // license key����(licenseδ�������ӻ���license���ݲ���ȷ)
    ERR_TAS_LICENSE_KEY_EXPIRE = 10014, // license key����
    ERR_TAS_LICENSE_NOT_ENOUGH = 10015, // license����
    ERR_TAS_LICENSE_KEY_INVALID_LICENSE_FILE = 10034,  // ��Ч��license�ļ�(��������벻ƥ���������)
    ERR_TAS_LICENSE_SOME_OTHER_PU_TOO_MANY = 10016,    // ĳ�⳧��PU����̫��
    ERR_TAS_LICENSE_OTHER_PU_TOTAL_TOO_MANY = 10017,   // �⳧��PU��������̫��
    ERR_TAS_LICENSE_NO_DIRECT_ACCESS_OTHER_PU = 10018, // ������ֱ�ӽ����⳧��PU
	ERR_TAS_LICENSE_OTHER_MORE_THAN_ALL = 10037,	   // �⳧��pu����������������ص���
    ERR_TAS_LICENSE_WRITE_USB_KEY_ERROR = 10043,       // д��USB KEYʧ��

    ERR_TAS_INVALID_EXPIRE_DATE = 10019,   // �������ڴ���
    ERR_TAS_INVALID_DOMAINID = 10020,      // ��Ŵ���
    ERR_TAS_INVALID_DEVICEMODELID = 10021, // �豸�ͺŴ���
	ERR_TAS_SAME_DEVICEMODEL_INFO = 10038,  // ��ͬ���豸�ͺ���Ϣ
    ERR_TAS_INVALID_DEVICETYPEID = 10022,  // �豸���ʹ���
    ERR_TAS_INVALID_DEVICEID = 10023,      // �豸�Ŵ���

    ERR_TAS_INVALID_SYNCDATA = 10032,   // ��Ч��ͬ�����ݣ�����ʹ��ʱͬ�����ݱ�ʶ��Ӧ�����������Ѿ���������
    ERR_TAS_DOMAIN_RELATION_VALIDATE_FAILED = 10035, // ���ϵУ��ʧ��

    ERR_TAS_DISABLED_ACCOUNT = 10036,    // δ���õ��û�
	ERR_TAS_EXTDATA_TOO_LONG = 10039,	 // ��Ϣ����
	ERR_TAS_TIME_OUT = 10040,            // ����ʱ
	ERR_TAS_OP_PART_FINISH = 10041,      // ����������ɣ�ͨ���������������в�������˵����
	ERR_TAS_DEVICE_MODEL_USING = 10044,      // �豸�ͺ�ʹ����
	ERR_TAS_LICENSE_ID_DIFF = 10045,			 // ����UsbKeyʱ��UsbKey��FileKey���֤ID��ͬ
	ERR_TAS_OP_MUST_ADMIN = 10046,			 // ������admin�û�������Ĳ���
	ERR_TAS_DEV_GROUP_IS_AUTO_UPDATE = 10047,	// �豸������Ϊ�Զ�ͬ���飬��ֹ�ò���
	ERR_TAS_GROUP_MUST_TOP_AUTO_UPDATE = 10048,	// ֻ����ȡ�������Զ�ͬ����
	ERR_TAS_GROUP_IS_NOT_AUTO_UPDATE = 10049,	// ���Զ�ͬ���飬��ֹ�ò���
    ERR_TAS_LICENSE_SDK_INIT_ERROR = 10050,       // license sdk��ʼ��ʧ��
	ERR_TAS_LICENSE_MANAGE_PU_TOO_MANY = 10051,        // ������ƽ̨�Ŀɹ����ص���
	ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP = 10052, // �豸������㼶���ӷ�����������������ƣ���ֹ�ò���
	ERR_TAS_DEVTREE_TOTALNUM_TOO_MUCH = 10053,     // �Զ����豸��������������
	ERR_TAS_RELYMODULE_DEALTIMEOUT_NOSTART = 10054, //ҵ������ģ��δ��������ʱ
	ERR_TAS_INPUT_ERROR_PASSWORD_TOO_MUCH = 10056, //�������������δ������Ƶ�½
	ERR_TAS_IP_LIMIT_LOGIN = 10057,                //��ǰIP�����Ƶ�½
	ERR_TAS_PASSWORD_EXPIRED = 10058,              //�����ѹ���

    ERR_3AS_END = ERR_3AS_BEGIN + 999,      //3as���������ֵ

    //cmu������ 11000-11999
    ERR_CMU_BEGIN = 11000,                  //cmu��������ʼֵ

    ERR_CMU_NOT_IN_SERVICE = 11001,         //ƽ̨������δ�������̬
    ERR_CMU_SIP_BODY_NULL = 11002,          //SIP��Ϣ��Ϊ��
    ERR_CMU_PUI_ALREADY_REG = 11003,        //��ͬ��PUI�Ѿ�ע��
    ERR_CMU_CUI_ALREADY_REG = 11004,        //��ͬ��CUI�Ѿ�ע��
    ERR_CMU_PU_NOT_IN_TOP = 11005,          //puδ����
    ERR_CMU_PU_NOT_USED = 11006,            //puδ����
    ERR_CMU_PU_EXPIRE = 11007,              //pu�Ѿ�����
    ERR_CMU_PU_ALREADY_REG = 11008,         //��ͬ��PU�Ѿ�ע��
    ERR_CMU_PU_LICENSE_NOT_MATCH = 11009,   //PU��license��ƥ��
    ERR_CMU_CU_LOGIN_SESS_ILLEGAL = 11010,  //CU�����е�¼Session�Ƿ�
    ERR_CMU_SS_OPE_TYPE_INVALID = 11011,    //CU���¶��ĵĲ���������Ч
    ERR_CMU_CUI_NOT_REG = 11012,            //CUIδ��CMUע��
    ERR_CMU_PUI_NOT_REG = 11013,            //PUIδ��CMUע��
    ERR_CMU_VTDU_ALREADY_REG = 11014,       //��ͬ��VTDU�Ѿ�ע��
    ERR_CMU_DEV_NO_ROUTER = 11015,          //�豸�޷�·��
    ERR_CMU_DEV_RSP_TIMEOUT = 11016,        //�豸Ӧ��ʱ
    ERR_CMU_CMU_ALREADY_CONN = 11017,       //��ͬ��CMU�Ѿ�����
    ERR_CMU_CMU_NOT_CONN = 11018,           //CMUδ����
    ERR_CMU_CMU_INVALID_CMU_URI = 11019,    //��ЧCMU URI
    ERR_CMU_PU_NOT_ONLINE = 11020,          //PU������
    ERR_CMU_USER_PTZ_PRI_LOW = 11021,       //�û�PTZ����Ȩ�޵�
    ERR_CMU_NOT_SUPPORT = 11022,            //�ù��ܲ�֧��
    ERR_CMU_ALARM_ILLEGAL = 11023,          //������ĸ澯
    ERR_CMU_STOP_BY_BYE = 11024,            //�Ի����Ҷ�
    ERR_CMU_DOMAIN_NOT_IN_TOP = 11025,      //Domianδ����
    ERR_CMU_DEV_ADDR_INVALID = 11026,       //�豸��ַ��Ч
	ERR_CMU_SUBDOMAIN_OFFLINE = 11027,      //�¼�������
	ERR_CMU_DIALOG_TERMINATE = 11028,       //CMU�Ի����ж�
	ERR_CMU_DIALOG_TERMINATE_BY_CUI = 11029,  //CMU�Ի���CUI�ж�
	ERR_CMU_DIALOG_TERMINATE_BY_VTDU = 11030, //CMU�Ի���VTDU�ж�
	ERR_CMU_DIALOG_TERMINATE_BY_PUI = 11031,  //CMU�Ի���PUI�ж�
	ERR_CMU_DIALOG_TERMINATE_BY_GBS = 11032,  //CMU�Ի���GBS�ж�
	ERR_CMU_DIALOG_TERMINATE_BY_SUBDOMAIN_CMU = 11033,  //CMU�Ի����¼�CMU�ж�
    ERR_CMU_SS_TYPES_INVALID = 11034,                   //����������Ч
    ERR_CMU_BATCH_REG_TYPE_INVALID = 11035,             //����ע���е�type��Ч
    ERR_CMU_UNCOMPATIBLE_REQ = 11036,             //�����ݵ�����
    ERR_CMU_NOT_SERVICE_TEMPORARILY = 11037,      //��ʱ���ɽ��ܷ���
    ERR_CMU_DATA_UNAVAILABLE_TEMPORARILY = 11038,   //������ʱ������

    //{{SWITCH=11500
    ERR_CMU_SWITCH_DEV_NOT_AVAILABLE = 11500,       //���뽻�����豸������
    //ERR_CMU_SWITCH_DST_NOT_AVAILABLE = 11501,       //����Ŀ�Ĳ�����
    //ERR_CMU_SWITCH_SRC_NOT_AVAILABLE = 11502,       //����Դ������
    ERR_CMU_SWITCH_DST_EXIST_SWITCH= 11503,         //����Ŀ�Ĵ��ڽ���
	ERR_CMU_SWITCH_SRC_EXIST_SWITCH= 11504,         //����Դ���ڽ���
    //ERR_CMU_SWITCH_POST_MSG_TO_SRC_FAILED= 11505,   //����ʱ������Ϣ��Դʧ��
    //ERR_CMU_SWITCH_POST_MSG_TO_VTDU_FAILED= 11506,  //����ʱ������Ϣ��Ŀ��ʧ��
    //ERR_CMU_SWITCH_POST_MSG_TO_DST_FAILED= 11507,   //����ʱ������Ϣ��VTDUʧ��
    ERR_CMU_SWITCH_SRC_NODE_NOT_EXIST = 11508,      //����Դ�ڵ㲻����
    ERR_CMU_SWITCH_DST_NODE_NOT_EXIST = 11509,      //����Ŀ�Ľڵ㲻����
    ERR_CMU_SWITCH_INVALID_DIALOG_ID = 11510,       //��Ч�Ľ����ỰID,�Ự��δ�������Ѿ�����
    ERR_CMU_SWITCH_ADDRESS_RULE_SWITCH_FAILED = 11511,      //����ʱתΪ��ַ����ʧ��
    ERR_CMU_SWITCH_VTDU_NOT_AVAILABLE = 11512,      //�Ҳ������õ�VTDU
    ERR_CMU_SWITCH_GAP_VTDU_NOT_AVAILABLE = 11513,  //�Ҳ������õ���բVTDU
    ERR_CMU_SWITCH_VTDU_PORT_ALLOC_FAILED = 11514,  //VTDUͨ����˿ڷ���ʧ��
    ERR_CMU_SWITCH_EXCEPTION = 11515,	            //����ʱ����������쳣
    ERR_CMU_SWITCH_ADD_BAK_SRC_FAILED = 11516,	    //����ʱ����µı���Դ����ʧ��
    ERR_CMU_SWITCH_STOP_SWITCH_FOR_NEW_BAK_SRC_FAILED = 11517,	    //����ʱΪ�����µı���Դֹͣԭ�н���ʧ��
    ERR_CMU_SWITCH_AGAIN = 11518,                   //��Դ��ʱ���ɵã���VTDU���ߵȣ���������
    ERR_CMU_SWITCH_TASK_TIMEOUT = 11519,            //CMU��������ʱ
    ERR_CMU_SWITCH_STOP_BY_BYE = 11520,             //�����Ի����Ҷ�
    ERR_CMU_SWITCH_VCR_FAILED = 11521,              //VCR����ʧ��
    ERR_CMU_SWITCH_DLG_RES_NOT_ENOUGH = 11522,      //�����Ự��Դ����
    //}}SWITCH

    
    ERR_CMU_END = ERR_CMU_BEGIN + 999,      //cmu���������ֵ

    //pui������ 12000-12999
    ERR_PUI_BEGIN = 12000,                  //pui��������ʼֵ

    ERR_PUI_PU_ALREAD_EXIST = 12001,        //��ͬ��PU�Ѿ�ע��
    ERR_PUI_PU_OVERLORD = 12002,            //pui����pu������
    ERR_PUI_SIP_BODY_NULL = 12003,          //SIP��Ϣ��Ϊ��
    ERR_PUI_NOT_IN_SERVICE = 12004,         //PUIδ�������̬
    ERR_PUI_ADD_SWITCH_FAIL = 12005,        //PUI��ӽ���ʧ��
    ERR_PUI_GET_CAM_PROTO_FAIL = 12006,     //PUI��ȡ����ͷ����Э��ʧ��
    ERR_PUI_GET_PU_CONFIG_FAIL = 12007,     //PUI��ȡǰ������ʧ��
    ERR_PUI_SET_PU_CONFIG_FAIL = 12008,     //PUI����ǰ������ʧ��
    ERR_PUI_PU_NOT_ONLINE = 12009,          //PU������
    ERR_PUI_PU_OPE_TIMEOUT = 12010,         //PU������ʱ
    ERR_PUI_PU_OPE_FAIL = 12011,            //PU����ʧ��
    ERR_PUI_PU_TYPE_ILLEGAL = 12012,        //PU���ͷǷ�
    ERR_PUI_PU_NODISK = 12013,              //ǰ����Ӳ��
    ERR_PUI_PU_RECORDING = 12014,           //����¼���Ԥ¼
    ERR_PUI_PU_RECPLAYING = 12015,          //���ڷ���
    ERR_PUI_PU_FILE_NOT_EXIT = 12016,		//�ļ�������(��·�����޴��ļ�)
    ERR_PUI_PU_RECLENGTH_ZERO = 12017,		//�ļ���СΪ��(¼��ʱ������)
    ERR_PUI_PU_RECLOG_NOT_EXIST = 12018,    //reclog�ļ�������
    ERR_PUI_PU_ASF_STATE_ERROR = 12019,     //����ASF�ļ�״̬����ȷ
    ERR_PUI_PU_PATH_CREATE_FAIL = 12020,    //Ŀ¼����ʧ��,�����Ƿ����ֻ��
    ERR_PUI_PU_NO_PIC_FILE = 12021,         //��ǰ��ץ��ͼƬ
    ERR_PUI_PU_PIC_FILE_TOO_MUCH = 12022,   //ǰ��ץ��ͼƬ����
    ERR_PUI_NVR_NO_BUF_RECORD = 12023,                      //¼�񻺴��¼������,����������¼��(NVRǰ�˷���ʱ)
    ERR_PUI_NVR_INVALID_PU_RECCHN = 12024,                  //¼��ͨ�������ã�һ���Ǵ�NVRͨ����δ�����豸(NVRǰ��¼��ʱ)
    ERR_PUI_NVR_NOT_ALLOW_DELETE_PU_REC = 12025,            //��ɾ��NVRǰ��¼��Ȩ��
    ERR_PUI_NVR_BITRATE_OVERFLOW = 12026,                   //�����ʹ���(NVR2820 BitRate:64 - 2048)
    ERR_PUI_NVR_PIC_TOO_MUCH = 12027,                       //NVRǰ��ץ��ͼƬ�����࣬����С������Χ
    ERR_PUI_NVR_NO_SNAPSHOT_PIC = 12028,                    //��NVRǰ��ץ��ͼƬ
    ERR_PUI_NVR_NO_VALID_REC_DISK = 12029,                  //NVR�޿���¼����̷���
    ERR_PUI_PU_NOT_SUPPORT = 12030,                         //��ǰ�˲�֧�ִ˹���
    ERR_PUI_PU_VCR_NOT_SUPPORT_PLAY_RATE = 12031,           //ǰ�˲�֧�ֵ�VCR��������
    ERR_PUI_PU_VCR_NOT_SUPPORT_CMD = 12032,                 //ǰ�˲�֧�ֵ�VCR��������
    ERR_PUI_QUERY_MAPPED_ADDR_FAILED = 12033,               //��ѯ������ַʧ��
    ERR_PUI_QUERY_MAPPED_ADDR_TIMEOUT = 12034,              //��ѯ������ַ��ʱ
    ERR_PUI_QUERY_MAPPED_ADDR_CANCELED = 12035,             //��ѯ������ַ������ȡ��
    ERR_PUI_PUNCH_FAILED = 12036,                           //��ַ̽��ʧ��
    ERR_PUI_PUNCH_TIMEOUT = 12037,                          //��ַ̽�ⳬʱ
    ERR_PUI_PUNCH_CANCELED = 12038,                         //��ַ̽�������ȡ��
    ERR_PUI_EXCEED_PLAY_REC_MAX_NUM = 12039,                //����ǰ����������Ŀ
    ERR_PUI_GET_LOCAL_ADDR_FAILED = 12040,                  //��ȡ���ص�ַʧ��
    ERR_PUI_MEDIA_TYPE_ERROR = 12041,                       //�����ý���ʽ
    ERR_PUI_PU_VCR_TRY_AGAIN = 12042,                       //���ڿ���ǰ�˷����Ժ����VCR����
    ERR_PUI_TRANS_OPR_TYPE_ERROR = 12043,                   //͸�����ݲ������ʹ���
	ERR_PUI_OSDBMP_SIZE_ERROR = 12044,                      //OSDBMP�Ĵ�С�������MTI����
	ERR_PUI_OSDFONT_WIDTH_ERROR = 12045,                    //�ַ���ȴ���MTI��󻺴�
	ERR_PUI_UNKNOW_MTI_AUDIO_TYPE_ERROR = 12046,            //��ȡ��MTI��Ƶ��ʽCMU��֧�� 
	ERR_PUI_UNKNOW_CMS_AUDIO_TYPE_ERROR = 12047,            //���յ���CMU��Ƶ��ʽMTI��֧��
	ERR_PUI_LONG_OSDCAPTION_SIZE_ERROR = 12048,             //long osd��ĸ���ȳ������ƴ�С
	ERR_PUI_GET_SNAPSHOT_CFG_FAIL = 12049,                  //��ȡǰ��ץ�Ĳ���ʧ��
	ERR_PUI_SET_SNAPSHOT_CFG_FAIL = 12050,                  //����ǰ��ץ�Ĳ���ʧ��
	ERR_PUI_GET_SERIALPORT_CFG_FAIL = 12051,                //��ȡ���ڲ���ʧ��
	ERR_PUI_SET_SERIALPORT_CFG_FAIL = 12052,                //���ô��ڲ���ʧ��
	ERR_PUI_PU_CPU_OVERLOAD = 12053,                        //ǰ�˳�����
	ERR_PUI_NOT_AUTHORIZED = 12054,                         //û��Ȩ��
	ERR_PUI_CRITICAL_ERROR = 12055,                         //�ؼ��Դ�����Ҫ��������
	ERR_PUI_NO_PROFILE     = 12056,                         //û��profile��onvifǰ�ˣ�
	ERR_PUI_PU_PLTFM_FLOWMAX = 12057,                       //3g�����������ֵ
    ERR_PUI_TASK_NOT_FOUND  = 12058,                        //����δ�ҵ�
    ERR_PUI_PU_UNKNOWN = ERR_PUI_BEGIN + 998,               //ǰ��δ֪����
    ERR_PUI_END = ERR_PUI_BEGIN + 999,      //pui���������ֵ

    //cui������ 13000-13999 ��ǰ���ú� 13030
    ERR_CUI_BEGIN = 13000,                  //cui��������ʼֵ

    ERR_CUI_USERNAME_OR_PASSWORD_WRONG = 13001,  //�û��������������
    ERR_CUI_CONNECT_CUI_FAIL = 13002,            //�޷�������CUI������
    ERR_CUI_DISCONNECT_CUI = 13003,              //��CUI�Ͽ�����
    ERR_CUI_NOT_IN_SERVICE = 13004,              //CUIδ�������̬
    ERR_CUI_OPR_TIME_OUT = 13005,                //����ʱ
    ERR_CUI_CU_NOT_ONLINE = 13006,               //CU�����ߣ�����Ƿ�
    ERR_CUI_CU_NO_PERMISSION = 13007,            //CUû��Ȩ��
    ERR_CUI_QUERYING_PRIVILEGE_DATA = 13026,     //���ڲ�ѯȨ�����ݣ����Ժ�����
    ERR_CUI_QUERYING_INNER_DATA = 13027,         //���ڲ�ѯ�ڲ����ݣ����Ժ�����
    ERR_CUI_DIALOG_NOT_ESTABLISHED = 13008,      //�Ự��δ�������������ܾ�
    ERR_CUI_NOT_SUBSCRIBE = 13009,               //�豸δ���Ĺ�
    ERR_CUI_PARAMS_INVALID = 13010,              //�����Ƿ�������
    ERR_CUI_DIALOG_TERMINATED = 13011,           //CUI�Ự���Զ���������
    ERR_CUI_SIP_MSG_FAIL = 13012,                //����SIP����ʧ�ܣ����ܷ�����������
    ERR_CUI_TAS_OFFLINE = 13013,                 //TAS�����������ߣ�����ʧ��
    ERR_CUI_INVALID_PARAM = 13014,               //��Ч�������
	ERR_CUI_IN_SUBSCRIBE = 13015,                //���ڶ�����
    ERR_CUI_UNKNOW_MSG = 13016,                  //����ʶ����Ϣ
    ERR_CUI_TASK_DEL = 13017,                    //task����
    ERR_CUI_TCP_TIMEOUT = 13018,                 //tcp�˽�����Ϣʧ��
    ERR_CUI_PE_OFFLINE = 13019,                  //����ģ�鲻����
    ERR_CUI_RECORD_END = 13020,                  //¼�񲥷�ֹͣ
    ERR_CUI_PASSWORD_ERROR = 13021,              //����У�����
    ERR_CUI_EXCEED_CU_MAX_NUM = 13022,           //�������cu�û���
    ERR_CUI_CROSS_DOMAIN_LOGIN = 13023,          //�ݲ���������¼
    ERR_CUI_STATE_ERROR = 13024,                 //����״̬����ȷ
    ERR_CUI_RSP_MESSAGE_TOO_LONG = 13025,        //Ӧ����Ϣ�����
	ERR_CUI_SUBSCRIBE_SERVER_NOT_ONLINE = 13028, //���ķ�����������
	ERR_CUI_EXCEED_MOBILE_CU_MAX_NUM = 13029,    //��������ƶ��ͻ�������ֵ
	ERR_CUI_DIALOG_TERMINATED_BY_CMU = 13030,    //CUI�Ự��CMU��������
	ERR_CUI_ADD_SWITCH_FAILED = 13031,           //CUI������ʧ��
	ERR_CUI_DIALOG_TERMINATED_BY_RCS = 13032,    //CUI�Ự��RCS��������
	ERR_CUI_PU_PTZ_LOCKED = 13033,				 //ǰ��PTZ�ѱ�����
	ERR_CUI_GET_REDIS_DATA_FAILED = 13034,       //��ȡ����ʧ��
	ERR_CUI_USER_EXCEEDED_MAX_STREAM = 13035,    //�û�������󲢷���
	ERR_CUI_CU_VERSION_UNSUPPORTIVE = 13036,     //��֧�ִ��û��汾
	ERR_CUI_TASK_NOT_SUPPORT = 13037,            //��֧�ִ�ҵ��
	ERR_CUI_REDIS_SUBS_FAILED = 13038,           //redis����ʧ��
	ERR_CUI_CU_REQ_NUM_EXCEEDED = 13039,		 //�������������
    ERR_CUI_END = ERR_CUI_BEGIN + 999,      //cui���������ֵ

    //vtdu������ 14000-14999
    ERR_VTDU_BEGIN = 14000,                 //vtdu��������ʼֵ

    ERR_VTDU_ADD_SWITCH_TO_DS_FAILED = 14001,       //[VTDU]��ӽ�����DataSwitchʧ��
    ERR_VTDU_REMOVE_SWITCH_FROM_DS_FAILED = 14002,  //[VTDU]��DataSwitchɾ������ʧ��
	ERR_VTDU_ADD_SWITCH_INVALID_ARGUMENT = 14003,   //[VTDU]��ӽ�������Ĳ�������
    ERR_VTDU_TASK_TIMEOUT = 14004,                  //[VTDU]������ʱ
    ERR_VTDU_EXCEPTION = 14005,                     //[VTDU]ϵͳ�쳣
    ERR_VTDU_WAIT_ACK_TIMEOUT = 14006,              //[VTDU]�ȴ�ȷ��Ӧ��ʱ
    ERR_VTDU_STUN_UPDATE_FAILED = 14007,            //[VTDU]����Stun��ַʧ��
    ERR_VTDU_WITHOUT_VTDU_ADDR_OF_NETSEG = 14008,   //[VTDU]û����Ӧ���ε�VTDU��ַ
    ERR_VTDU_UPDATE_SWITCH_REMOVE_FAILED = 14009,   //[VTDU]���½�����ַʱɾ���Ͻ���ʧ��
    ERR_VTDU_UPDATE_SWITCH_ADD_FAILED = 14010,      //[VTDU]���½�����ַʱ����½���ʧ��
    ERR_VTDU_UPDATE_SWITCH_DUMP_FAILED = 14011,     //[VTDU]���½�����ַʱ���Dumpʧ��

    ERR_VTDU_QUERY_MAPPED_ADDR_FAILED = 14500,      //[VTDU]��ѯ������ַʧ��
    ERR_VTDU_QUERY_MAPPED_ADDR_TIMEOUT = 14501,     //[VTDU]��ѯ������ַ��ʱ
    ERR_VTDU_QUERY_MAPPED_ADDR_CANCELED = 14502,    //[VTDU]��ѯ������ַ������ȡ��

    ERR_VTDU_PUNCH_FAILED = 14520,                  //[VTDU]��ַ̽��ʧ��
    ERR_VTDU_PUNCH_TIMEOUT = 14521,                 //[VTDU]��ַ̽�ⳬʱ
    ERR_VTDU_PUNCH_CANCELED = 14522,                //[VTDU]��ַ̽�������ȡ��
    ERR_VTDU_INVALID_PUNCH_PARAM = 14523,           //[VTDU]��Ч��Punch����
    ERR_VTDU_INVALID_SWITCH_RESOURCE = 14524,       //[VTDU]��Ч�Ľ�����Դ
    ERR_VTDU_INVALID_SWITCH_OWNER = 14525,          //[VTDU]�����е�Owner��Ч
    ERR_VTDU_CACULATE_PORT_FAILED = 14526,          //[VTDU]����˿ڳ���
    ERR_VTDU_REDIS_FORWARD_REQUEST_FAILED = 14527,  //[VTDU]redis��¼�����������
    ERR_VTDU_REDIS_FORWARD_RESPONSE_FAILED = 14528, //[VTDU]redis��¼����Ӧ�����
    ERR_VTDU_REDIS_FORWARD_ACKNOWLEDGE_FAILED = 14529,//[VTDU]redis��¼ȷ�Ͻ�������
    ERR_VTDU_REDIS_FORWARD_DESTORY_FAILED = 14530,  //[VTDU]redis��¼�����������
	ERR_VTDU_SWITCH_VCR_FAILED = 14531,             //[VTDU]VCR����ʧ��


    ERR_VTDU_END = ERR_VTDU_BEGIN + 999,    //vtdu���������ֵ

    //pmcs������ 15000-15999
    ERR_PMCS_BEGIN = 15000,

    ERR_PMCS_NOT_IN_SERVICE = 15001,         //pmcsδ�������̬
    ERR_PMCS_OPR_TIME_OUT = 15002,          //ҵ������ʱ
    ERR_PMCS_WEB_NOT_ONLINE = 15003,          //pmcs web ������,����Ƿ�
    ERR_PMCS_DIALOG_TERMINATED = 15004,        //��pms�Ự���Զ���������
    ERR_PMCS_SIP_MSG_FAIL = 15005,               //����SIP����ʧ�ܣ����ܷ�����������
    ERR_PMCS_DISCONNECT_PMCS = 15006,             //��PMCS�Ͽ�����
	ERR_PMCS_PMS_NOT_IN_SERVICE = 15007,          //PMSδ�������
	ERR_PMCS_3AS_NOT_IN_SERVICE = 15008,      //3asδ�������
	ERR_PMCS_RCS_NOT_IN_SERVICE = 15009,					//rcs���ڷ���̬
	ERR_PMCS_RCS_SUBSCRIBE_ALREADY_EXIST = 15010,		//�����Ѵ���
	ERR_PMCS_RCS_SUBSCRIBE_NOT_EXIST = 15011,			//��δ����
	ERR_PMCS_PMCAGENT_NOT_EXIST = 15012,		//PMCAGENT������
	ERR_PMCS_PMC_NOT_IN_SERVICE = 15013,		//PMC�쳣���ܾ�����
    ERR_PMCS_PMS_PLATFORM_RELATION_ERROR = 15014,    //ƽ̨��ϵ���
    ERR_PMCS_PMS_OPT_ERROR = 15015,                  //ҵ����ʧ��
    ERR_PMCS_PMS_SUBPLATFORM_ADD_OPT_ERROR = 15016,  //��Ӵ�ƽ̨���¼�ƽ̨ʱ��ƽ̨���¼�ƽ̨����ʧ�ܣ�����ƽ̨���ϼ�ƽ̨�в���
    ERR_PMCS_PMS_SUBPLATFORM_DEL_OPT_ERROR = 15017,  //ɾ����ƽ̨���¼�ƽ̨ʱ����ƽ̨���ϼ�ƽ̨����ʧ�ܣ���ƽ̨���¼�ƽ̨�Ѵ���
    ERR_PMCS_DETECTED_NO_TOPO_INFO = 15018,          //δ̽�⵽��ȷ��topoƽ̨
    ERR_PMCS_NAT_DATA_ERR = 15019,                   //ӳ���������󣬲����������
    ERR_PMCS_GBS_NOT_IN_SERVICE = 15020,             //GBSδ�������̬
    ERR_PMCS_IMPORT_FILE_FAIL = 15021,               //�豸��������ʧ��
    ERR_PMCS_IMPORT_DEV_INFO_ERR = 15022,            //���������豸�ļ�ģ�����
    ERR_PMCS_GET_DEV_MOD_FAIL = 15023,        //��ȡ�豸�ͺ�ʧ��
    ERR_PMCS_DEV_IMPORT_FILE_NOT_EXIST   = 15024,    //���������豸�ļ�������
    ERR_PMCS_DEV_MODEL_EMPTY             = 15025,    //û�в鵽�豸�ͺ�
    ERR_PMCS_QRY_DEV_FAIL                = 15026,    //��ѯ�����豸ʧ��
    ERR_PMCS_QRY_GBDEV_FAIL              = 15027,    //��ѯ�����豸ʧ��
    ERR_PMCS_QRY_DEVICE_EMPTY            = 15028,    //��ѯ��0���豸
	ERR_PMCS_UNZIP_FAIL                  = 15029,    //��ѹ��ʧ��
	ERR_PMCS_MPS_NOT_IN_SERVICE          = 15030,    //MPSδ�������̬
	ERR_PMCS_UNZIP_FILE_TOO_MUCH         = 15031,    //��ǰ����ѹ���ļ��Ѵﵽ�޶�����3��
	//upload_fcgi �����룬�ӵ�pmcs��
	ERR_UPLOAD_FCGI_OPEN_FILE_FAIL       = 15032,    //���ļ�ʧ��
	ERR_UPLOAD_FCGI_NOBOUNDARY           = 15033,    //û�ҵ��߽�
	ERR_UPLOAD_FCGI_UNCOMPLETE           = 15034,    //�ļ�������
	ERR_UPLOAD_FCGI_WRITE_FILE_FAIL      = 15035,    //д���ļ�ʧ��
	//upload_fcgi ������ ����
	ERR_PMCS_IP_IS_LOCAL_IP              = 15036,    //ip����Ϊ����ip


    ERR_PMCS_END = ERR_PMCS_BEGIN + 999,    //pmcs���������

	// CU������ 16000 - 16999 
	ERR_CU_BEGIN = 16000,						// cu������ο�ʼ

	ERR_CU_END = ERR_CU_BEGIN + 999,	// cu�����������

    //als������ 17000-17999
    ERR_ALS_BEGIN = 17000,                  //als��������ʼֵ

    ERR_ALS_INVALID_ALARM_LINK = 17001,     //��Ч�ĸ澯��������
    ERR_ALS_DB_OPR_FAIL = 17002,            //���ݿ����ʧ��
    ERR_ALS_DB_OPR_TIMEOUT = 17003,         //���ݿ������ʱ
    ERR_ALS_ALARM_LINK_NOT_EXIST = 17004,   //�澯����������

    ERR_ALS_END = ERR_ALS_BEGIN + 999,      //als���������ֵ

    //tvs������ 18000-18999
    ERR_TVS_BEGIN = 18000,                  //tvs��������ʼֵ
    ERR_TVS_TVWALL_NOT_EXIST = 18001,       //����ǽ������
    ERR_TVS_TV_PLAY_NOT_EXIST = 18002,      //���Ӳ��Ų�����
    ERR_TVS_TV_OCCUR_NOT_MATCH = 18003,     //���Ӵ������Ͳ�ƥ��
    ERR_TVS_TV_SRC_NOT_MATCH = 18004,       //���Ӳ���Դ����ƥ��
    ERR_TVS_3PC_INVITE_FAIL = 18005,        //3PC����ʧ�ܣ�sip�����
    ERR_TVS_3PC_INVITE_TIMEOUT = 18006,     //3PC���г�ʱ
    ERR_TVS_TV_POLL_OPR_FAIL = 18007,       //��ѯ����ʧ��
    ERR_TVS_DB_OPR_FAIL = 18008,            //���ݲ���ʧ��
    ERR_TVS_DB_OPR_TIMEOUT = 18009,         //���ݲ�����ʱ
    ERR_TVS_ILLEGAL_SCHEME_NAME = 18010,    //��Ч��Ԥ������
    ERR_TVS_ILLEGAL_SCHEME_ID = 18011,      //��Ч��Ԥ��ID
    ERR_TVS_SCHEME_NOT_EXIST = 18012,       //Ԥ��������
    ERR_TVS_CUI_LOGIN_SESS_ILLEGAL = 18013, //�Ƿ���cui��¼session
    ERR_TVS_TV_DEC_BIND_ERROR = 18014,      //���ӻ��󶨽���������
    ERR_TVS_ILLEGAL_ENC_CHN = 18015,        //��Ч�ı���ͨ��
    ERR_TVS_ILLEGAL_SHEME_POLL_CFG = 18016, //��Ч��Ԥ����ѯ����
    ERR_TVS_SET_DEC_STYLE_TIMEOUT = 18017,  //���ý��������ʱ
    ERR_TVS_NOT_LOGIN_CMU = 18018,          //δ��¼cmu
    ERR_TVS_LOGIN_CMU_BYE_3PC = 18019,      //cmu����bye 3PC����
    ERR_TVS_3PC_DIALOG_TERMINATE = 18020,   //3PC�����쳣��ֹ��sip�����
	ERR_TVS_LOCKED_TVWALL_OR_CHANNEL_NOT_EXIST = 18021,   //�����ڵ���������ͨ��������������ǽԤ��
	ERR_TVS_UNLOCK_TVWALL_OR_CHANNEL_PASS_ERR = 18022,   //��������ͨ������Ԥ���������
	ERR_TVS_INVALID_WATCHOINT_SAVE_REQ = 18023, //��Ч�Ĺۿ��㱸������
	ERR_TVS_SAVED_WATCHPOINT_NOT_EXIST = 18024, //�����ڵı��ݹۿ���
	ERR_TVS_TVWALL_OR_CHANNEL_LOCKED = 18025, //����ǽԤ������ͨ������
	ERR_TVS_TVWALL_TVCHN_TASK_EXIST = 18026, //����ͨ����ռ��
    ERR_TVS_TVWALL_TIMEOUT = 18027,          //������ʱ
    ERR_TVS_TV_SAME_DIV_OCCUR_TYPE = 18028,     // ��ͬ�Ĵ��� ��������
    ERR_TVS_SAME_ENC_CHANNEL_IN_USE= 18029,     // ��ͬ����Ƶ��Ƶͨ����ʹ����
    ERR_TVS_ERR_CONFIG_DISABLE_ALARM_OFF_WALL = 18030, // TVS ���ó� �ܾ� �澯��ǽ
    ERR_TVS_DIV_EXEC_TASK_NOT_FOUND   = 18031,  // δ�ҵ�ָ���� ���� ��ѯ����
    ERR_TVS_END = ERR_TVS_BEGIN + 999,      //tvs���������ֵ

    //rcs������ 19000-19999
    ERR_RCS_BEGIN = 19000,                  //rcs��������ʼֵ
    ERR_RCS_RMS_NOT_ONLINE = 19001,             //[RCS]rms������
    ERR_RCS_NRU_NOT_ONLINE = 19002,             //[RCS]nru������
    ERR_RCS_DB_OPR_FAIL = 19003,                //[RCS]���ݿ����ʧ��
    ERR_RCS_DB_OPR_TIMEOUT = 19004,             //[RCS]���ݿ������ʱ
    ERR_RCS_PU_NOT_ONLINE = 19005,              //[RCS]ǰ���豸������
    ERR_RCS_PU_CONFIG_NOT_AVAILABLE = 19006,    //[RCS]ǰ�����ò������ɵ�
    ERR_RCS_RECORD_PARAM_NOT_EXIST = 19007,     //[RCS]¼�����������
    ERR_RCS_RECORD_TASK_ALREADY_EXIST = 19008,  //[RCS]¼�������Ѿ�����
    ERR_RCS_EXCEPTION = 19009,                  //[RCS]�������쳣
    ERR_RCS_ALLOC_RECORD_RES_FAILED = 19010,    //[RCS]����¼����Դʧ��
    ERR_RCS_START_RECORD_FAILED = 19011,        //[RCS]����¼��ʧ��
    ERR_RCS_RECORD_TASK_NOT_EXIST = 19012,      //[RCS]��������Ӧ��¼������
    ERR_RCS_UPDATE_RECTYPE_FAILED = 19013,      //[RCS]����¼������ʧ��
    ERR_RCS_DEV_NOT_REGISTER = 19014,           //[RCS]�豸δע��
    ERR_RCS_TASK_TIMEOUT = 19015,               //[RCS]������ʱ
    ERR_RCS_NOT_REG_TO_CMU = 19016,             //[RCS]δע����CMU
    ERR_RCS_PLAYBACK_TASK_NOT_EXIST = 19017,    //[RCS]�������񲻴���
    ERR_RCS_DEV_NOT_EXIST = 19018,              //[RCS]�豸������
    ERR_RCS_SS_OPE_TYPE_INVALID = 19019,        //[RCS]��Ч�Ķ�������
    ERR_RCS_DEV_NO_ROUTER = 19020,              //[RCS]�����޷���·�ɵ�ָ���豸
    ERR_RCS_STOP_BY_BYE = 19021,                //[RCS]�Ի����Ҷ�
    ERR_RCS_NOT_SUPPORT = 19022,                //[RCS]�ù��ܲ�֧��
    ERR_RCS_CONVERT_URI_DOMAIN_FAILED = 19023,  //[RCS]ת��URI������ʧ�ܣ������Ҳ�����Ӧ������Ϣ
    ERR_RCS_VCR_TRY_AGAIN = 19024,              //[RCS]���ڿ���¼��طţ��Ժ����VCR����
	ERR_RCS_REPEAT_SS = 19025,                  //[RCS]�ظ�����
	ERR_RCS_FOREIGN_DOMAIN_SS = 19026,          //[RCS]��֧�ֿ�����
	ERR_RCS_CHANNEL_UNAVALIABLE = 19027,        //[RCS]ͨ��������
	ERR_RCS_REC_CANNOT_STOPPED_MANUALLY = 19028,//[RCS]����¼���澯¼�����ֶ�ֹͣ
	ERR_RCS_NRU_NOT_EXIST = 19029,				//[RCS]nru������
	ERR_RCS_DISK_PART_TOO_BIG = 19030,			//[RCS]��֧�ֳ���16T�Ĵ���
	ERR_RCS_DIALOG_TERMINATE = 19031,           //[RCS]�Ի����ж�
	ERR_RCS_INVALID_PARAM = 19032,              //[RCS]��Ч����
	ERR_RCS_RESOURCE_CALL_FAIL = 19033,         //[RCS]��Դ����ʧ��

    ERR_RCS_END = ERR_RCS_BEGIN + 999,          //rcs���������ֵ

    //uas������ 20000-20999 (��ǰ���ú�Ϊ20006)
    ERR_UAS_BEGIN = 20000,                  //uas��������ʼֵ
    ERR_UAS_DBERROR = 20001,                // ���ݿ��������������һ��
    ERR_UAS_SAVE_CONFIG_FAILED = 20002,     // ��������ʧ��
	ERR_UAS_TIMEOUT = 20003,                // ����ʱ
	ERR_UAS_PROC_FAIL = 20004,              // ����ʧ��
	ERR_UAS_UNKNOWN_TYPE = 20005,           // δ֪����
    ERR_UAS_END = ERR_UAS_BEGIN + 999,      //uas���������ֵ

    //mps������ 21000-21999
    ERR_MPS_BEGIN = 21000,                  //mps��������ʼֵ
	ERR_MPS_INVAILD_MSG  = 21001,			//�������Ϣ��
	ERR_MPS_MSG_TIME_OUT	 = 21002,		//��Ϣ����ʱ
	ERR_MPS_DB_CONNECT_ERROR = 21003,		//�������ݿ�ʧ��
	ERR_MPS_DB_OPR_ERROR	= 21004,		//�������ݿ�ʧ��
	ERR_MPS_DB_OPR_TIMEOUT	= 21005,		//�������ݿⳬʱ
	ERR_MPS_DB_NO_RESULT	= 21006,		//���ݲ�ѯ����������
	ERR_MPS_MAP_EXISTS		= 21007,		//��ͼ�Ѵ���
	ERR_MPS_MAP_ELEMENT_EXISTS	= 21008,	//ͼԪ�Ѵ���
	ERR_MPS_ICON_PATH_NOT_EXISTS = 21009,	//��ͼͼԪĿ¼������
	ERR_MPS_PARAMS_INVAILD		= 21010,	//�������Ϸ�
	ERR_MPS_POINT_OUT_OF_RANG	= 21011,	//У�����ڷ�Χ֮��
	ERR_MPS_NOT_CONFIG_GPS_STORE	= 21012,	//δ����GPS�洢
	ERR_MPS_GOOGLE_MAIN_MAP_DEL_ILLEGAL	= 21013,	//Google����ͼ������ɾ��
    ERR_MPS_END = ERR_MPS_BEGIN + 999,      //mps���������ֵ

    //pms������ 22000-22999 ��ǰ���ô�����Ϊ22080
    ERR_PMS_BEGIN = 22000,                  //pms��������ʼֵ
    ERR_PMS_INVALID_PARAM = 22012,          //��Ч���������
    ERR_PMS_DATA_NOT_EXIST = 22019,         //���ݲ�����
    ERR_PMS_NOT_IN_SERVICE = 22001,         //PMSδ�������̬
    ERR_PMS_INVALID_OPERATION = 22020,      //�Ƿ�������ҵ���ϲ�������˲���
    ERR_PMS_EXECUTE_TIMEOUT = 22039,        //ִ�г�ʱ

    ERR_PMS_3AS_NOT_IN_SERVICE = 22002,     //3asδ�������̬
    ERR_PMS_INVALID_PMC_URI = 22003,        //��Ч��pmc uri
    ERR_PMS_PMS_ALREADY_CONN = 22004,       //�ظ���¼
    ERR_PMS_DB_OPR_FAIL = 22005,            //���ݿ��������
    ERR_PMS_USERNAME_OR_PASSWORD_WRONG = 22006, //�û������������
    ERR_PMS_INVALID_SERVER_URI = 22007,         //��Ч��SERVER URI
    ERR_PMS_DB_OPR_TIMEOUT = 22008,         //���ݿ������ʱ
	ERR_PMS_WEB_ONT_ONLINE = 22009,			//WEB������
	ERR_PMS_PMS_UNKNOW_DEVTYPE = 22010,     //��Ч���豸����
	ERR_PMS_DB_DATA_NOT_EXIST = 22011,		//���ݿ��в�����Ҫ�ҵ�����
    ERR_PMS_OPR_FAIL = 22015,               //����ʧ��
    
    ERR_PMS_PLATFORM_ALREADY_IN_TOPO = 22014,//������ƽ̨�Ѿ����������������˳�������
    ERR_PMS_REPEATED_PLATFORM_RELATION = 22021, //�ظ���ƽ̨��ϵ
    ERR_PMS_GET_MOSTADAPTIVEIP_FAILED = 22051,  //��ȡ�����ƥ��IPʧ��, �����Ƿ���Ҫ����NATӳ��
    ERR_PMS_DETECTED_NOT_CONSISTENCE_TOPO_CONF = 22017, //̽�⵽��һ�µ�TOPO���� 
    ERR_PMS_MODIFY_CONFIG_FAILED = 22016,               //�޸�PMS�������ļ�ʧ��
    ERR_PMS_MODIFY_COMMONCONFIG_FAILED = 22018,         //�޸Ĺ��������ļ�ʧ��

    ERR_PMS_GET_ROUTEINFO_ERR = 22022,              //��ȡ����·����Ϣʧ��
    ERR_PMS_SET_ROUTEINFO_ERR = 22023,              //��������·��ʧ��
    ERR_PMS_DEL_ROUTEINFO_ERR = 22024,              //ɾ������·��ʧ��
    ERR_PMS_ADD_ROUTEINFO_ERR = 22025,              //�������·��ʧ��
    ERR_PMS_RESET_ERR = 22026,                      //��ԭ��������ʧ��
    ERR_PMS_SOFTWAREINFO_QRY_ERR = 22027,           //�����Ϣ��ȡʧ��
    ERR_PMS_SOFTWAREINFO_MOD_ERR = 22028,           //�����Ϣ�޸�ʧ��
    ERR_PMS_GET_DEFAULTROUTE_ERR = 22029,           //��ȡĬ������ʧ��
    ERR_PMS_SET_DEFAULTROUTE_ERR = 22030,           //����Ĭ������ʧ��
    ERR_PMS_MOD_PLATFORMTIME_ERR = 22031,           //�޸�ƽ̨ʱ��ʧ��
    ERR_PMS_CTRL_PMS_ERR = 22032,                   //PMSƽ̨����ʧ��
    ERR_PMS_MOD_NETCARDINFO_ERR = 22033,            //�޸�������Ϣʧ��
    ERR_PMS_QRY_PLATFORMTIME_ERR = 22034,           //��ѯƽ̨ʱ��ʧ��
    ERR_PMS_ADD_NETNAT_ERR = 22035,                 //�������NATʧ��
    ERR_PMS_DEL_NETNAT_ERR = 22036,                 //ɾ������NATʧ��
    ERR_PMS_ADD_REPEATED_DATA_ERR = 22037,          //�ظ��������
    ERR_PMS_NETWORK_RESTART_ERR = 22038,            //�����������ʧ��
    ERR_PMS_INVTASK_LIMITED = 22040,                //�����������롢�ָ���������ͬʱֻ�ܴ���һ������,��������������Ӧ���������
    ERR_PMS_ADD_USER_ALREADY_EXIST = 22041,         //��ӵ��û��Ѿ�����
    ERR_PMS_MOD_USER_NOT_EXIST = 22042,             //Ҫ�޸ĵ��û�������
    ERR_PMS_DEL_NTP_SERVER_ERR = 22043,             //ɾ��NTP����ʧ��
    ERR_PMS_SET_NTP_SERVER_ERR = 22045,             //����NTP����ʧ��
    ERR_PMS_REMOVE_USBBAK_FAILED = 22044,           //�Ƴ�USB������ʧ�ܣ������Ǳ���������ʹ�ã����Ժ�����
    ERR_PMS_RECORD_USERLOG_FAILED = 22046,          //��¼�û���־ʧ��
    ERR_PMS_EXPORTMIRROR_TASK_ALREADY_EXIST = 22047,//��������������Ѿ�����
    ERR_PMS_ABNORMAL_STATUS = 22048,                //PMS�����쳣״̬��ֻ�ṩ���޷���
    ERR_PMS_INVALID_NTP_SERVER_ADDR = 22049,        //��Ч��NTP�����ַ
    ERR_PMS_DOMAINNAME_ALREADY_EXIST = 22050,       //Ҫ�޸ĵ������������¼�ƽ̨��ͻ������ʧ��
    ERR_PMS_SET_MAIL_CFG_ERR = 22052,               //��������ʧ��
    ERR_PMS_QRY_MAIL_CFG_ERR = 22053,               //�����ò�ѯʧ��
    ERR_PMS_SEND_TEST_MAIL_ERR = 22054,             //�����ʼ�����ʧ��,���ʼ�����ִ��ʧ��
    ERR_PMS_MAILTO_LIST_IS_NULL = 22055,            //�ʼ������б�Ϊ�գ������ʼ�����ʧ��
    ERR_PMS_MAILSERVER_CFG_ERR = 22056,             //�ʼ��������������󣬲���Ϊ��
	ERR_PMS_DOMAIN_CONPLICT = 22057,                //������ͻ
	ERR_PMS_EXPORT_TASK_LIMIT = 22058,              //��������ﵽ����ֵ
	ERR_PMS_UPNP_EXIST = 22059,						//�Ѵ�����ͬ��ӳ��
	ERR_PMS_UPNP_NOT_EXIST = 22060,					//�����ڵ�ӳ��
	ERR_PMS_UPNP_NOT_CONNECT = 22061,				//UPnP��δ���ӳɹ�
	ERR_PMS_DDNS_NOT_CONNECT = 22062,				//DDNS��δ���ӳɹ�
	ERR_PMS_NET_SERVICE_TIMEOUT = 22063,			//NetService������ʱ
	ERR_PMS_NET_DONOT_ALLOW_DEL_PRESET = 22064,		//��ֹɾ������ӳ��
	ERR_PMS_DDNS_DOMAIN_EXIST = 22065,				//DDNS�����Ѿ���ռ��
	ERR_PMS_UPNP_ROUTER_DISABLE = 22066,			//·����δ����UPnP����
	ERR_PMS_UPNP_PORT_INVALID = 22067,				//UPnP�˿���Ч
	ERR_PMS_UPNP_ERROR_UNKNOWN = 22068,				//δ֪��UPnP����
	ERR_PMS_DDNS_ERROR_UNKNOWN = 22069,				//δ֪��DDNS����
	ERR_PMS_UPNP_PARAM_INVALID = 22070,				//UPnP������Ч
	ERR_PMS_DDNS_SYSTEM_DOMAIN = 22071,				//ϵͳ������DDNS����
	ERR_PMS_DDNS_SERVICE_DISABLE = 22072,           //DDNS����δ����
	ERR_PMS_UPNP_PORT_IS_USED = 22073,				//UPnP�˿ڱ�ռ��
	ERR_PMS_UPNP_NETCARD_OR_MODE_OVERDUE = 22074,	//UPnP������id���߲���ģʽ����
    ERR_PMS_UPNP_PORT_USING_BY_DDNS = 22075,		//��ӳ�����ڱ�DDNSʹ����
	ERR_PMS_UPNP_SWITCH_NETCARD_FAIL = 22076,		//�л������������·����ʧ��
	ERR_PMS_HTTP_PORT_USED = 22077,					//Http�˿ڱ�ռ��
	ERR_PMS_UPNP_IS_INEFFECTIVE = 22078,			//UPnP����δ��Ч
	ERR_PMS_UPNP_PLAT_NOT_ACTIVATE = 22079,			//ƽ̨UPnP����δ����
    ERR_PMS_SET_MSS_SERVER_CFG_ERR = 22080,        //����Mss��������ַʧ��
    ERR_PMS_DEL_MSS_SERVER_CFG_ERR = 22081,        //����Mss��������ַʧ��
    ERR_PMS_SET_PUIGB_MTS_IP_CFG_ERR = 22082,     //pms����puigb��mts
	ERR_PMS_SUB_DOMAIN_REDEL = 22083,         //ƽ̨˽�����������ظ�����
    ERR_PMS_SUB_DOMAIN_DEL_PROGRESS_QRY = 22084,         //ƽ̨˽�������������Ȳ�ѯʧ��
    ERR_PMS_CROSS_DEFENSE_STREAM_MOD_ERR = 22085,  //�޸ķ���������ʧ��
    ERR_PMS_SET_GBS_MTS_IP_CFG_ERR = 22086,     //pms����gbs��mts
    ERR_PMS_RESET_CU_PASSWORD_FAIL_ERR = 22087,          //pms�ָ�cu����ʧ��
    ERR_PMS_GET_SUCUREMAIL_ERR = 22088,                  //pmc��ȡ��ȫ����ʧ��
    ERR_PMS_GET_GEN_SERIALNUM_ERR = 22089,                //pms�������к�ʧ��
    ERR_PMS_SAFECODE_SN_NOT_PAIR_ERR = 22090,             //�ָ�Ĭ������ʧ��
	ERR_PMS_NETWORK_SEGMENT_SAME_ERR = 22091,       //��ֹ����������������ͬ����
	ERR_PMS_NOT_CONNECTED_TO_REDIS_ERR = 22092,     //pmsδ������redis
    ERR_PMS_MOD_SECUREMAIL_ERR = 22093,                   //�޸İ�ȫ����ʧ��
	ERR_PMS_INPUT_ERROR_PASSWORD_TOO_MUCH = 22094, //�������������δ������Ƶ�½
	ERR_PMS_IP_LIMIT_LOGIN = 22095,                //��ǰIP�����Ƶ�½
	ERR_PMS_HEARTBEART_CONF_FAIL_ERR = 22096,      //����������ʧ��
	ERR_PMS_IPTABLES_CONF_FAIL_ERR = 22097,        //����������ʧ��
	ERR_PMS_HEARTBEAT_BAKCONF_EXIST = 22098,      //�������������Ѵ���
	ERR_PMS_TCPDUMP_FILE_NOT_EXIST = 22099,       //ץ���ļ�������
	ERR_PMS_PASSWORD_EXPIRED = 22100,             //�����ѹ���
	ERR_PMS_GET_MODULE_LIST_ERR = 22101,          //��ȡģ���б�ʧ��
	ERR_PMS_TCPDUMP_EXIST = 22102,                //ץ�������Ѵ���
	ERR_PMS_ZIP_MODULE_LOG_ERR = 22103,           //ѹ��ģ���ռ�ʧ��
	ERR_PMS_GET_MOBILE_CONNECT_NUM_ERR = 22104,   //��ȡ�ƶ��ͻ��˽�������ʧ��
	ERR_PMS_NOT_SET_MIRROR_BAK_FILE = 22105,      //δ���ñ��ݾ���·��
	ERR_PMS_MIRROR_BAK_PATH_NOT_EXIST = 22106,    //���񱸷�·��������
	ERR_PMS_MIRROR_BAK_FILE_NOT_EXIST = 22107,    //���񱸷��ļ�������
	ERR_PMS_HEARTBEAT_CONF_GET_ERR = 22108,       //������������Ϣ��ȡʧ��
	ERR_PMS_SPACE_NOT_ENOUGH = 22109,             //�ռ䲻��
	ERR_PMS_EXPORT_LOG_TASK_EXIST = 22110,        //�����ռ������Ѵ���
	ERR_PMS_MOD_USER_NAME_EXIST = 22111,          //Ҫ�޸ĵ��û����û����Ѿ�����
	ERR_PMS_LICENSE_NOT_EXIST_AND_HEARTBEAT_CONF_ERR = 22112,     //license������,��������ʧ��

    ERR_PMS_END = ERR_PMS_BEGIN + 999,      //pms���������ֵ

    //rmssdk������ 23000-23999
    ERR_RMSSDK_BEGIN = 23000,                   //rmssdk���������ֵ
    ERR_RMSSDK_END = ERR_RMSSDK_BEGIN + 999,    //rmssdk���������ֵ

    //scsdk������ 24000-24999
    ERR_SCSSDK_BEGIN = 24000,                   //scsdk���������ֵ
    ERR_SCSSDK_END = ERR_SCSSDK_BEGIN + 999,    //scsdk���������ֵ

    //gbs������ 25000-25999�� ��ǰ���ú�Ϊ25033
    ERR_GBS_BEGIN = 25000,
    ERR_GBS_INVALID_PARAM = 25007,              //�����Ƿ�
    ERR_GBS_INVALID_OPERATE = 25008,            //�Ƿ�����
    ERR_GBS_CMSSDK_SENDMSG_FAILED = 25001,      //cmssdk������Ϣʧ��
    ERR_GBS_CMSSDK_RESPONSE_TIMEOUT = 25002,    //cmssdk�ظ���ʱ
    ERR_GBS_SAVE_CONFIG_FAILED = 25003,         //�����ļ�����ʧ��
    ERR_GBS_DBOPR_FAILED = 25004,               //���ݿ����ʧ��
    ERR_GBS_DBOPR_TIMEOUT = 25005,              //���ݿ������ʱ
    ERR_GBS_EXIST_REPEATED_DATA = 25006,        //�����ظ�����
    ERR_GBS_DETECTED_CIRCLE_DATA = 25017,       //̽�⵽������
    ERR_GBS_INVALID_GBID = 25009,               //�����ŷǷ�
    ERR_GBS_SAME_GBID_WITH_LOCAL = 25016,       //�뱾���������ͬ
    ERR_GBS_GBDOMAIN_ID_NOT_AVAILABLE = 25020,  //������Ų����ã�Ҳ�������ͬ����Ż���ڻ�
    ERR_GBS_NOT_AVAILABLE_GBID = 25021,         //�����Ų����ã�Ҳ���ǹ����ŷǷ����߷Ǳ�����Ż������Ͳ�֧�ֻ��߱���Ѿ���ռ��
    ERR_GBS_NOT_ENOUGH_GBID = 25022,            //�ṩ�Ĺ����Ų�����Ҫ���豸��ͨ������Ҫָ��
    ERR_GBS_DATA_NOT_EXIST = 25010,             //���ݲ�����
    ERR_GBS_DATA_NOT_INTEGRITY = 25011,         //���ݲ�����������ɾ����Ӧ����
	ERR_GBS_PU_NOT_ONLINE = 25012,              //ǰ�˲�����
	ERR_GBS_OSP_SIP_POST_FAIL = 25013,          //OSPSIP������Ϣʧ��
	ERR_GBS_PU_VCR_NOT_SUPPORT_CMD = 25014,     //ǰ�˲�֧�ֵ�����
	ERR_GBS_PU_VCR_NOT_SUPPORT_PLAY_RATE = 25015,//ǰ�˲�֧�ֵĲ����ٶ�
	ERR_GBS_G800_ERROR = 25018,                 //G800����
	ERR_GBS_486_SIP_ERROR = 25019,              //486SIP����
	ERR_GBS_SUBGBDMOAIN_NOT_ONLINE = 25023,     //�����¼�������
    ERR_GBS_LOWER_DOMAIN_BUSY = 25024,      //�¼�������æ
	ERR_GBS_INNER_PERIPHERAL_TYPE = 25025,		// ������������
    ERR_INVALID_GBS_GBNATID = 25026,               //���Ϸ��Ĺ���NatID
    ERR_GBS_GB_NATID_NOT_EXIST = 25027,       //����NATID������
    ERR_GBS_GB_NATID_ALREADY_IN_USE = 25028, //����NATID��ǰ����ʹ��
    ERR_GBS_GB_NATDATA_CONTENT_PORTRANGE_INVALID=25029, //����NAT�������ã��˿ڷ�Χ����
    ERR_GBS_GB_NATDATA_CONTENT_ALREADY_EXIST=25030,   //��ǰNAT�����Ѵ���
	ERR_GBS_GB_LOWER_DOMAIN_TERMINATE_DIALOG=25031,   //�����¼�������ǰ����ֹ�Ự
	ERR_GBS_PRIOR_OPERTAE_UNFINISHED_NEED_WAIT=25032,       //ǰ�β�����δ���������Ժ�
	ERR_GBS_NEED_ADD_REPORT_INTER_DEV_CFG_FIRST=25033,      //�������ӡ��ϱ������豸���á�
	ERR_GBS_UP_CAPACITY_LACK = 25034,                 //ƽ̨�������������������GBSģ��δ������������GBSδע�ᵽ��GBS
	ERR_GBS_NEED_SELECT_INTER_OPTION_FIRST = 25035,         //��Ҫ��ѡ���齨����ƽ̨��ѡ��
	ERR_GBS_WRITE_DATA_TO_REDIS_FAIL = 25036,         //����д��redisʧ��
    ERR_GBS_IMPORT_FILE_NOT_EXIST = 25037,               //�ļ�������
    ERR_GBS_END = 25999,
	
	//cmssdk�ڲ������룺26000-26499
	ERR_CMS_SDK_BEGIN                                 = 26000,
	ERR_CMS_SDK_REINIT                                = 26001,          //�ظ���ʼ��
	ERR_CMS_SDK_CALLBACK_INVALID                      = 26002,          //�ص�������Ч
	ERR_CMS_SDK_GW_UUID_INVALID                       = 26003,          //����UUID��Ч
	ERR_CMS_SDK_DOMAIN_NAME_INVALID                   = 26004,          //ƽ̨������Ч
	ERR_CMS_SDK_GW_ADDR_INVALID                       = 26005,          //���ص�ַ��Ч
	ERR_CMS_SDK_PLAT_ADDR_INVALID                     = 26006,          //ƽ̨��ַ��Ч
	ERR_CMS_SDK_OSPEXT_INIT_FAIL                      = 26007,          //ospext��ʼ��ʧ��
	ERR_CMS_SDK_OSPSIP_INIT_FAIL                      = 26008,          //ospsip��ʼ��ʧ��
	ERR_CMS_SDK_OSPAPP_CREATE_FAIL                    = 26009,          //ospapp����ʧ��
	ERR_CMS_SDK_GW_MSG_INVALID                        = 26010,          //������Ϣ��Ч
	ERR_CMS_SDK_OSP_POST_FAIL                         = 26011,          //OspPostʧ��
	ERR_CMS_SDK_TIMEOUT                               = 26012,          //��ʱ����
	ERR_CMS_SDK_SEND_MSG_FAIL                         = 26013,          //��ƽ̨��������ʧ��
	ERR_CMS_SDK_CMU_DIS_CONN                          = 26014,          //δ���ӵ�CMU
	ERR_CMS_SDK_3AS_DIS_CONN                          = 26015,          //δ���ӵ�3as
	ERR_CMS_SDK_PU_REG_TIMEOUT                        = 26016,          //puע�ᳬʱ
	ERR_CMS_SDK_DEVICE_UUID_INVALID                   = 26017,          //�豸ID��Ч
	ERR_CMS_SDK_DEVICE_NAME_INVALID                   = 26018,          //�豸����Ч
	ERR_CMS_SDK_GROUP_UUID_INVALID                    = 26019,          //�豸����ID��Ч
	ERR_CMS_SDK_GROUP_NAME_INVALID                    = 26020,          //�豸��������Ч
	ERR_CMS_SDK_DOMAIN_UUID_INVALID                   = 26021,          //��ID��Ч
	ERR_CMS_SDK_DEVICE_NOT_EXIST                      = 26022,          //�豸������
	ERR_CMS_SDK_RCS_DIS_CONN                          = 26023,          //δ���ӵ�rcs
	ERR_CMS_SDK_TVS_DIS_CONN                          = 26024,          //δ���ӵ�tvs
	ERR_CMS_SDK_HAVE_NOT_SS                           = 26025,		   //��δ���ģ�����ȡ������ǰ����Ƿ��Ѷ��ģ�
	ERR_CMS_SDK_ALREADY_SS                            = 26026,		   //�ظ����ģ����ڶ���ǰ����Ƿ��ظ����ģ�
	ERR_CMS_SDK_SS_OPE_TYPE_INVALID                   = 26027,          //��Ч�Ķ��Ĳ�������
	ERR_CMS_SDK_TRANSCODE_TERMINATE                   = 26028,          //G800ת���쳣��ֹ
	ERR_CMS_SDK_PU_CONF_NTF_TIMEOUT                   = 26029,          //pu����֪ͨ��ʱ
	ERR_CMS_SDK_PU_UNREG_TIMEOUT                      = 26030,          //pu����֪ͨ��ʱ
	ERR_CMS_SDK_PU_ALREADY_REG                        = 26031,			//pu�Ѿ�ע��
	ERR_CMS_SDK_ERR_DIALOG_TERMINATE                  = 26032,			//�Ự�쳣��ֹ
	ERR_CMS_SDK_ERR_TASK_DEL                          = 26033,			//Task�Ѿ�ɾ��
	ERR_CMS_SDK_DIALOG_NOT_ESTABLISHED                = 26034,			//�Ự��δ����
	ERR_CMS_SDK_UAS_DIS_CONN                          = 26035,          //δ���ӵ�uas
	ERR_CMS_SDK_VCR_CTRL_TIMEOUT                      = 26036,          //VCR����������Ϣ��ʱ
	ERR_CMS_SDK_PLAT_REC_PLAYBACK_FINISHED            = 26037,          //ƽ̨������������
    ERR_CMS_SDK_COMMONCONFIG_INIT_FAIL                = 26038,          //���������ļ���ʼ��ʧ��
    ERR_CMS_SDK_PU_REG_CONFIG_NOT_COMPLETED           = 26039,          //puע����Ϣ������
    ERR_CMS_SDK_PU_ALARM_RESET_FAIL                   = 26040,          //ǰ�˱�����λʧ��

	ERR_CMS_SDK_END                                   = 26499,

	//GW�����룺26500-26999
	ERR_GW_BEGIN                                      = 26500,
	ERR_GLB_ERR_NOINIT_ADAPTER                        = 26501,          //��ʼ��ADAPTER
	ERR_GLB_ERR_LOAD_ADATER_FAIL                      = 26502,          //����adapterʧ��
	ERR_GLB_ERR_INIT_ADAPTER_FAIL                     = 26503,          //��ʼ��adapterʧ��
	ERR_GLB_ERR_NOINIT_PLAT                           = 26504,          //û�г�ʼ��ƽ̨
	ERR_GLB_ERR_USERNAME_ERR                          = 26505,          //�û���Ϣ����
	ERR_GLB_ERR_PASSWORD_ERR                          = 26506,          //�û��������
	ERR_GLB_ERR_PARA_EMPTY                            = 26507,          //�������ݴ���
	ERR_GLB_ERR_DLL_NOOBJ                             = 26508,          //�ײ�dllΪ��
	ERR_GLB_ERR_UNKNOWN_REASON                        = 26509,          //δ֪ԭ��Ĵ���
	ERR_GLB_ERR_OPR_TIMEOUT                           = 26510,          //������ʱ
	ERR_GLB_ERR_OPR_FAIL                              = 26511,		    //����ʧ��
	ERR_GLB_ERR_TRANS_LIMIT                           = 26512,          //ת����������       
	ERR_GLB_ERR_ALREADY_REALPLAY                      = 26513,          //��Ը��豸�Ľ��յ�ַ�Ѿ�����(ʵʱ���)
	ERR_BLB_ERR_DEV_RECDOWN_EXIST                     = 26514,          //��Ը��豸ͨ����¼��ط��Ѿ�����
	ERR_GLB_ERR_RESOURCE_LACK                         = 26515,          //ϵͳ��Դ����
	ERR_GLB_ERR_NOSUPP_FUNC                           = 26516,          //��֧�ָù���
	ERR_GLB_ERR_NOSTREAM                              = 26517,          //��������֪ͨ
	ERR_GLB_ERR_NOINIT_NAT                            = 26518,          //NAT��͸δ��ʼ��
	ERR_GLB_ERR_DAU_REG_FAIL                          = 26519,          //DAU��GMUע��ʧ��
	// G100��ش�����
	ERR_GLB_G100_SDKFAILED                            = 26520,	        //����SDK�ӿ�ʧ��
	ERR_GLB_G100_RECODERNOTFOUND                      = 26521,		    //��ѯ����¼��
	ERR_GLB_G100_NOENOUGHPRI                          = 26522,			//Ȩ�޲���
	ERR_GLB_G100_DVRNORESOURCE                        = 26523,			//DVR��Դ���㣬ϵͳæ
	ERR_GLB_G100_DVROPRATEFAILED                      = 26524,		    //DVR����ʧ��
	ERR_GLB_G100_TIMEINPUTERROR                       = 26525,		    //ʱ�����벻��ȷ
	ERR_GLB_G100_NOSPECFILE                           = 26526,			//�ط�ʱ������û��ָ�����ļ�
	ERR_GLB_G100_OPERNOTFINISH                        = 26527,			//�ϴβ�����δ���
	ERR_GLB_G100_COMMANDTIMEOUT                       = 26528,		    //ִ�����ʱ
	ERR_GLB_G100_NOSUPPORT                            = 26529,			//��������֧��
	ERR_GLB_G100_FILEFORMATERROR                      = 26530,		    //�ļ���ʽ����ȷ
	ERR_GLB_G100_NODOWNLOADCHN                        = 26531,			//û�и��������ͨ������(¼��ط������ز���ͬʱ����)
	ERR_GLB_G100_DVRHASQUERYTASK                      = 26532,          //DVR�Ѿ����ڲ�ѯ�������Ժ��ٲ�
	ERR_GLB_G100_DVRQUERYTIMEOUT                      = 26533,          //DVR¼���ѯ��ʱ ��ȴ�
	ERR_GLB_G100_DVRQUERYUPLOADING                    = 26534,          //DVR�����û����ڻش����� ��ȴ�
	ERR_GLB_G100_DHQUERYTIMEOUT                       = 26535, 		    //�󻪲�ѯ¼��ʱ
	ERR_GLB_G100_DHPLAYBACKLIMIT                      = 26536,		    //��¼��ط�ֻ��һ·

	ERR_GW_END                                        = 26999,

	ERR_CAPS_BEGIN                                    =27000,			//CAPSģ������뿪ʼ
	ERR_CAPS_MCUSDK_INIT_FAILED	                      =27001,			//ץ�Ľ���ģ���ʼ��ʧ��
	ERR_CAPS_START_PLAY_FAILED                        =27002,			//ץ�Ŀ�ʼ���벥��ʧ��
	ERR_CAPS_TAKE_PICTURE_FAILED                      =27003,			//ץ��ʧ��
	ERR_CAPS_MAKE_THUMB_FAILED                        =27004,			//��������ͼʧ��
	ERR_CAPS_TIME_OUT                                 =27005,			//ץ�ĳ�ʱ
	ERR_CAPS_INVALID_CUI_URI                          =27006,           //��ЧCUI URI
	ERR_CAPS_INVALID_CUI_ALREADY_CONN                 =27007,           //��ͬ��CUI�Ѿ�����
	ERR_CAPS_CHANNUM_EXCEED_LIMIT                     =27008,           //ץ��·����������
	ERR_CAPS_DEV_ALREADY_EXIST                        =27009,           //ץ���豸�Ѵ���
	ERR_CAPS_DEV_NOT_EXIST                            =27010,           //ץ���豸������
	ERR_CAPS_END                                      =27499,			//CAPSģ����������

	ERR_DPS_BEGIN									  =27500,			// DPS�����뿪ʼ.
	ERR_DPS_ZMQ_CONNECT_FAIL,						// 27501			// ����ʧ��
	ERR_DPS_ZMQ_SEND_FAIL,							// 27502			// ����ʧ��
	ERR_DPS_CACHE_ERROR,							// 27503			// Cache��Ч
	ERR_DPS_INVALID_SEARCH_KEY,						// 27504			// ����Key��Ч.
	ERR_DPS_WORKER_BUSY,							// 27505			// ģ��æ.
	ERR_DPS_INVALID_URI,							// 27506			// �Ƿ���URI
	ERR_DPS_INVALID_CLIENT_ALREADY_LOGIN,			// 27507			// �ظ���¼.
	ERR_DPS_DB_CONNECT_FAIL,						// 27508			// �������ݿ�ʧ��.
	ERR_DPS_DB_OP_FAIL,								// 27509			// ���ݲ���ʧ��.
	ERR_DPS_REDIS_CONNECT_FAIL,						// 27510			// ����Redisʧ��
	ERR_DPS_REDIS_OP_FAIL,							// 27511			// REDIS���ݿ����ʧ��.
	ERR_DPS_WAIT_IDLE_WORKER_TIMEOUT,				// 27512			// �ȴ�������Դ��ʱ, ���Ժ�����
	ERR_DPS_INVALID_DATA,							// 27513			// ���ݲ��Ϸ�
	ERR_DPS_ZMQ_RECV_FAIL,							// 27513			// ZMQ����ʧ��
	ERR_DPS_TIME_OUT,								// 27514			// ����ʱ


	ERR_DPS_END										  =27999,			// DPSģ����������

    // RedisSdk������
    ERR_REDISSDK_BEGIN                                = 28000,

    ERR_REDISSDK_UNINITIALIZED                        = 28001,          // RedisSdkδ��ʼ��
    ERR_REDISSDK_REINITIALIZED                        = 28002,          // �ظ���ʼ��
    ERR_REDISSDK_CONNECT_FAILED                       = 28003,          // ����redisʧ��
    ERR_REDISSDK_REDIS_UNAVALIABLE_TEMPORARILY        = 28004,          // redis-server�����У���ʱ������
    ERR_REDISSDK_LOAD_LUA2SHA_FAILED                  = 28005,          // ����lua�ű���sha1����ӳ���ϵʧ��
    ERR_REDISSDK_LOAD_DOMAIN_MAP_FAILED               = 28006,          // ������ӳ���ϵʧ��
    ERR_REDISSDK_SCRIPT_PARAM_ERROR                   = 28007,          // ��������
    ERR_REDISSDK_LUA2SHA_ERROR                        = 28008,          // lua�ű�ӳ�䵽sha1����ʱ����
    ERR_REDISSDK_LUA_EXECUTE_ERROR                    = 28009,          // lua�ű�ִ��ʱ����
    ERR_REDISSDK_DEV_ID_IS_EMPTY                      = 28010,          // DevIdΪ��
    ERR_REDISSDK_SESSION_IS_EMPTY                     = 28011,          // SessionΪ��
    ERR_REDISSDK_SCRIPT_RETURN_ERROR                  = 28012,          // �ű����ؽ������
    ERR_REDISSDK_NO_REGISTERED_MODULE                 = 28013,          // û��ע���ģ��
    ERR_REDISSDK_SUBSCRIBE_FAILED                     = 28014,          // redis����ʧ��
    ERR_REDISSDK_PATTERN_SUBSCRIBE_FAILED             = 28015,          // redisģʽ����ʧ��
    ERR_REDISSDK_UNSUBSCRIBE_FAILED                   = 28016,          // redisȡ������ʧ��
    ERR_REDISSDK_PATTERN_UNSUBSCRIBE_FAILED           = 28017,          // redisȡ��ģʽ����ʧ��
    ERR_REDISSDK_GET_PLAT_REC_PARAM_FAILED            = 28018,          // ��ȡƽ̨¼���������
    ERR_REDISSDK_NO_ADAPTED_MODULE_CMU                = 28019,          // û���ҵ����ʵ�cmu
    ERR_REDISSDK_NO_ADAPTED_MODULE_RCS                = 28020,          // û���ҵ����ʵ�rcs
    ERR_REDISSDK_NO_ADAPTED_MODULE_VTDU               = 28021,          // û���ҵ����ʵ�vtdu
    ERR_REDISSDK_NO_ADAPTED_MODULE_PUI                = 28022,          // û���ҵ����ʵ�pui
    ERR_REDISSDK_NO_ADAPTED_MODULE_CUI                = 28023,          // û���ҵ����ʵ�cui
    ERR_REDISSDK_NO_ADAPTED_MODULE_NRU                = 28024,          // û���ҵ����ʵ�nru
    ERR_REDISSDK_GET_VTDU_RESOURCE_FAILED             = 28025,          // ��ȡvtdu��Դ��Ϣʧ��
    ERR_REDISSDK_MODULE_IS_OFFLINE                    = 28026,          // ģ�鲻����
    ERR_REDISSDK_CU_IS_OFFLINE                        = 28027,          // CU������
    ERR_REDISSDK_PU_IS_OFFLINE                        = 28028,          // ǰ�˲�����
    ERR_REDISSDK_INVALID_BASE_INFO                    = 28029,          // ��Ч����������
    ERR_REDISSDK_NRU_NOT_IN_TOPO                      = 28030,          // nruδ����
    ERR_REDISSDK_ALLOC_VTDU_RESOURCE                  = 28031,          // ��ȡvtduת����Դʧ��
    ERR_REDISSDK_FORBID_STOP_PLAN_ALARM_REC           = 28032,          // ��ֹ�ֶ�ֹͣ�ƻ����߸澯¼��
    ERR_REDISSDK_RECORD_TASK_NOT_EXIST                = 28033,          // ¼�����񲻴���
    ERR_REDISSDK_RECORD_TASK_NOT_BELONG_NRU           = 28034,          // ¼����������ָ����nru
    ERR_REDISSDK_RECORD_TASK_ALREAY_EXIST             = 28035,          // ¼�������Ѵ���
    ERR_REDISSDK_RECORD_HAS_SAVED                     = 28036,          // ¼�������ѱ��棬���ǵ�ǰû���ʺϵĴ��̻��߷���
    ERR_REDISSDK_DEVICE_NOT_IN_TOPO                   = 28037,          // �豸δ����

    ERR_REDISSDK_END                                  = 28499,


    // NRU������
    ERR_NRU_BEGIN                              	      = 29000,
	ERR_NRU_GET_VTDU                       	          = 29001,          // [NRU]��ȡVTDUʧ��
    ERR_NRU_DEV_OFFLINE                    	          = 29002,          // [NRU]�豸����
    ERR_NRU_PARAM							          = 29003,          // [NRU]NRU������
	ERR_NRU_REACH_MAX_TASK						      = 29004,          // [NRU]NRU�ﵽ���������
	ERR_NRU_TASK_TIMEOUT							  = 29005,          // [NRU]NRU����ʱ
    ERR_NRU_GET_REC_TASK_PARAM						  = 29006,          // [NRU]��ȡ¼��������Ϣʧ��
    ERR_NRU_PU_NOT_ONLINE                             = 29007,          // [NRU]ǰ���豸������
    ERR_NRU_PU_CHANNEL_NOT_ONLINE                     = 29008,          // [NRU]ǰ��ͨ��������
    ERR_NRU_PU_MANU_INVALID                           = 29009,          // [NRU]ǰ���豸���̷Ƿ�
    ERR_NRU_PU_CONFIG_NOT_AVAILABLE                   = 29010,          // [NRU]ǰ�����ò������ɵ�
    ERR_NRU_RECORD_PARAM_NOT_EXIST                    = 29011,          // [NRU]¼�����������
    ERR_NRU_RECORD_TASK_ALREADY_EXIST                 = 29012,          // [NRU]¼�������Ѿ�����
    ERR_NRU_EXCEPTION                                 = 29013,          // [NRU]�������쳣
    ERR_NRU_ALLOC_RECORD_RES_FAILED                   = 29014,          // [NRU]����¼����Դʧ��
    ERR_NRU_START_RECORD_FAILED                       = 29015,          // [NRU]����¼��ʧ��
    ERR_NRU_PLAN_REC_NOT_NOW                          = 29016,          // [NRU]�ƻ�¼��δ������ʱ��

	ERR_NRU_UNKNOWN								= 29099,

	//nru err(by rmserr)
	ERR_NRU_RSERR_RPCTRL_NOT_INITIAL  				= 29100,
	ERR_NRU_RSERR_NO_MEMORY							= 29101,
	ERR_NRU_RSERR_RECORD_FILE_NOT_EXIST				= 29102,
	ERR_NRU_RSERR_REC_STATUS_ERROR  				= 29103,
    ERR_NRU_RSERR_RECTASK_TOO_MUCH					= 29104,
    ERR_NRU_RSERR_REC_OVER_CAPACITY					= 29105,
    ERR_NRU_RSERR_RESOLV_CHANGED					= 29106,
	ERR_NRU_RSERR_WAIT_KEYFRAME						= 29107,
    ERR_NRU_RSERR_TIMESTAMP_ERR						= 29108,
	ERR_NRU_RSERR_PREREC_TIME_INVALID				= 29109,
    ERR_NRU_RSERR_SMALL_FILE_INTERVAL_INVALID		= 29110,
	ERR_NRU_RSERR_ALREADY_ALARM_TWO					= 29111,
    ERR_NRU_RSERR_PLAYTASK_NOTEXIST					= 29112,
	ERR_NRU_RSERR_NO_PLAY_RESOURCE       			= 29113,
	ERR_NRU_RSERR_PLAY_OVER_CAPACITY    			= 29114,
	ERR_NRU_RSERR_PLAY_DROG_TIME_INVALID    		= 29115,
	ERR_NRU_RSERR_PLAY_GRAN_INVALID    				= 29116,
	ERR_NRU_RSERR_PLAY_RECORD_LENGTH_ZERO			= 29117,
    ERR_NRU_RSERR_PLAY_REACH_STOP_TIME				= 29118,
	ERR_NRU_RSERR_NO_DOWNLOAD_RESOURCE				= 29119,
	ERR_NRU_RSERR_DOWNLOADTASK_NOTEXIST    			= 29120,
	ERR_NRU_RSERR_DOWNLOAD_STATUS_ERROR				= 29121,
	ERR_NRU_RSERR_SOCKET_ERROR						= 29122,
    ERR_NRU_RSERR_CONNECT_ERROR  					= 29123,
	ERR_NRU_RSERR_PATH_ALREADY_EXIST				= 29124,
    ERR_NRU_RSERR_PATH_NOT_EXIST 					= 29125,
	ERR_NRU_RSERR_PATH_CREATE_FAIL_RPC				= 29126,
    ERR_NRU_RSERR_PATH_DELETE_FAIL 					= 29127,
	ERR_NRU_RSERR_FILE_PATH_INVALID					= 29128,
	ERR_NRU_RSERR_FILE_ALREADY_EXIST  				= 29129,
	ERR_NRU_RSERR_FILE_NOT_EXIST					= 29130,
	ERR_NRU_RSERR_FILE_DELETE_FAIL					= 29131,
	ERR_NRU_RSERR_FILE_RENAME_FAIL					= 29132,
    ERR_NRU_RSERR_FILE_NAME_INVALID					= 29133,
	ERR_NRU_RSERR_MEDIATYPE_UNSUPPORT    			= 29134,
	ERR_NRU_RSERR_START_TIME_INVALID    			= 29135,
	ERR_NRU_RSERR_RECLOG_FILE_NOT_EXIST				= 29136,
	ERR_NRU_RSERR_RPCMD_IS_BUSY						= 29137,
	ERR_NRU_RSERR_INODE_DESTROYED					= 29138,
	ERR_NRU_RSERR_FORMAT_CHANGE						= 29139,
    ERR_NRU_RSERR_FILEVER_TOOLOW					= 29140,
	ERR_NRU_RSERR_FILE_PROPERTY						= 29141,
	ERR_NRU_RSERR_FILE_FAIL							= 29142,
	ERR_NRU_RSERR_METHOD_NOTSUPPORT      			= 29143,
	
	ERR_NRU_END                                		= 29999,

    //δ֪�����룬������ʹ��δ֪����
    ERR_UNKNOWN = 0xFFFFFFFF,
};

#endif // _CMS_ERRORCODE_H_
