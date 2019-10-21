/*****************************************************************************
   ģ����      : CMSSDK
   �ļ���      : cmssdk_errorcode.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cmssdk�����붨��
   ����        : fanxg
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/09/12  1.0         fanxg        ����
******************************************************************************/

#ifndef __CMS_SDK_ERRORCODE_H__
#define __CMS_SDK_ERRORCODE_H__

//�����붨��
enum CMS_SDK_ERRCODE
{
	CMS_SDK_SUCCESS = 0,                       //�����ɹ�,�ɹ�����Ҫ���ÿ�����������壬���в�������

	//������һЩ������
	CMS_SDK_COMMON_INVALID_DEV_URI = 1,         //��Ч��dev uri
	CMS_SDK_COMMON_DEV_ALREADY_LOGIN= 2,        //��ͬuri��dev�Ѿ���¼
	CMS_SDK_SIP_BODY_NULL = 3,				    //SIP��Ϣ��Ϊ��
	CMS_SDK_POST_MSG_FAILED = 4,                //������Ϣʧ��
	CMS_SDK_INVALIAD_REQ_PARAM = 5,             //��Ч���������
	CMS_SDK_INVALIAD_SDP = 6,                   //��Ч��ý��������Ϣ
	CMS_SDK_DIALOG_TERMINATE = 7,               //�Ի����ж�
	CMS_SDK_MKDIR_FAILED = 8,                   //����Ŀ¼ʧ��
	CMS_SDK_INVALID_SESSIONID = 9,              //��Ч�ĻỰID��ָ���ĻỰ������
	CMS_SDK_REQUIRE_TOO_MUCH = 10,              //ͬ��������࣬����ҵ���޶�
	CMS_SDK_OLD_PASSWORD_INCORRECT = 11,        //���������

	//������sip������룺4xx-6xx��������β���sip��׼���壬���ܱ����ģ��ռ�á�
	//��Щ������ȫ�ֹ�����ģ�������ٴζ���
	//���õ�sip�����붨�����£�������������
	// 4xx Client Error
	CMS_SDK_SIP_403_FORBIDDEN        = 403,  // 403 ��ֹ����
	CMS_SDK_SIP_404_NOT_FOUND        = 404,  // 404 �Ҳ���Ŀ��
	CMS_SDK_SIP_408_REQ_TIMEOUT      = 408,  // 408 ����ʱ
	CMS_SDK_SIP_423_INTERV_TOO_BRIEF = 423,  // 423 �������Ƶ��
	CMS_SDK_SIP_486_BUSY_HERE        = 486,  // 486 Busy Here    
	// 5xx Server Error
	// 6xx Global Error

	//3as������ 10000-10999(��ǰ���ú�Ϊ10040)
	CMS_SDK_3AS_BEGIN = 10000,                  //3as��������ʼֵ

	CMS_SDK_TAS_INVALID_PARM = 10001,        // �����Ƿ�, ��ϸ����μ���̨��ӡ�����������Ҫ���ڳ���Ա
	CMS_SDK_TAS_HANDLE_IN_PROGRESS = 10002,  // ��ʾһ���������ڴ�������У��û������ظ��ύ����
	CMS_SDK_TAS_INVALID_OPERATION = 10003,   // �Ƿ�������ҵ���ϲ�������˲���
	CMS_SDK_TAS_SYSTEM_BUSY = 10024,         // ϵͳ��æ�����Ժ�����
	CMS_SDK_TAS_ULTRA_VIRES = 10028,         // ԽȨ��Ϊ
	CMS_SDK_TAS_EXIST_SAMENAME_RECORD = 10029,  // ����ͬ����¼
	CMS_SDK_TAS_FIELD_CONTENT_EMPTY = 10031,    // �ֶ�����Ϊ��
	CMS_SDK_TAS_CURRENT_USER_NOT_EXIST = 10030, // ��ǰ�����߲����ڣ��Ự�����ڻ����û�������

	CMS_SDK_TAS_SYSTEM_UNINITED = 10004,   // ϵͳδ��ʼ��
	CMS_SDK_TAS_DBTASK_POOL_FULL = 10005,  // DB�������
	CMS_SDK_TAS_DBERROR = 10006,           // ���ݿ��������������һ��
	CMS_SDK_TAS_RECORD_EXISTS = 10007,     // ���ݿ��¼�Ѿ�����
	CMS_SDK_TAS_RECORD_NOT_EXISTS = 10008, // ���ݿ��¼������
	CMS_SDK_TAS_RECORD_FETCH_FAILED = 10009, // ���ݿ��¼��ȡʧ��, �����Ǽ�¼�Ѿ���������

	CMS_SDK_TAS_LOGINED = 10010,           // ��ͬ��ʶ�Ŀͻ����Ѿ���¼TAS
	CMS_SDK_TAS_USER_NOT_EXIST = 10011,    // �û�������
	CMS_SDK_TAS_INVALID_PASSWORD = 10012,  // �������
	CMS_SDK_TAS_USER_EXPIRED = 10025,      // �û�����
	CMS_SDK_TAS_USER_DISABLE_MULTILOGIN = 10026,  // ��ֹ��ε�¼
	CMS_SDK_TAS_USER_TYPE_DISABLE_CHANGE = 10027, // �û����Ͳ������޸�, ������ڶ����û����û������ܸ�Ϊ����Ա

	CMS_SDK_TAS_DECODER_ALREADY_BINDED = 10033,   // �������Ѿ�����

	CMS_SDK_TAS_LICENSE_KEY_ERROR = 10013,  // license key����(licenseδ�������ӻ���license���ݲ���ȷ)
	CMS_SDK_TAS_LICENSE_KEY_EXPIRE = 10014, // license key����
	CMS_SDK_TAS_LICENSE_NOT_ENOUGH = 10015, // license����
	CMS_SDK_TAS_LICENSE_KEY_INVALID_LICENSE_FILE = 10034,  // ��Ч��license�ļ�(��������벻ƥ���������)
	CMS_SDK_TAS_LICENSE_SOME_OTHER_PU_TOO_MANY = 10016,    // ĳ�⳧��PU����̫��
	CMS_SDK_TAS_LICENSE_OTHER_PU_TOTAL_TOO_MANY = 10017,   // �⳧��PU��������̫��
	CMS_SDK_TAS_LICENSE_NO_DIRECT_ACCESS_OTHER_PU = 10018, // ������ֱ�ӽ����⳧��PU
	CMS_SDK_TAS_LICENSE_OTHER_MORE_THAN_ALL = 10037,	   // �⳧��pu����������������ص���

	CMS_SDK_TAS_INVALID_EXPIRE_DATE = 10019,   // �������ڴ���
	CMS_SDK_TAS_INVALID_DOMAINID = 10020,      // ��Ŵ���
	CMS_SDK_TAS_INVALID_DEVICEMODELID = 10021, // �豸�ͺŴ���
	CMS_SDK_TAS_SAME_DEVICEMODEL_INFO = 10038,  // ��ͬ���豸�ͺ���Ϣ
	CMS_SDK_TAS_INVALID_DEVICETYPEID = 10022,  // �豸���ʹ���
	CMS_SDK_TAS_INVALID_DEVICEID = 10023,      // �豸�Ŵ���

	CMS_SDK_TAS_INVALID_SYNCDATA = 10032,   // ��Ч��ͬ�����ݣ�����ʹ��ʱͬ�����ݱ�ʶ��Ӧ�����������Ѿ���������
	CMS_SDK_TAS_DOMAIN_RELATION_VALIDATE_FAILED = 10035, // ���ϵУ��ʧ��

	CMS_SDK_TAS_DISABLED_ACCOUNT = 10036,    // δ���õ��û�
	CMS_SDK_TAS_EXTDATA_TOO_LONG = 10039,	 // ��Ϣ����

	CMS_SDK_3AS_END = CMS_SDK_3AS_BEGIN + 999,      //3as���������ֵ

	//cmu������ 11000-11999
	CMS_SDK_CMU_BEGIN = 11000,                  //cmu��������ʼֵ

	CMS_SDK_CMU_NOT_IN_SERVICE = 11001,         //ƽ̨������δ�������̬
	CMS_SDK_CMU_SIP_BODY_NULL = 11002,          //SIP��Ϣ��Ϊ��
	CMS_SDK_CMU_PUI_ALREADY_REG = 11003,        //��ͬ��PUI�Ѿ�ע��
	CMS_SDK_CMU_CUI_ALREADY_REG = 11004,        //��ͬ��CUI�Ѿ�ע��
	CMS_SDK_CMU_PU_NOT_IN_TOP = 11005,          //puδ����
	CMS_SDK_CMU_PU_NOT_USED = 11006,            //puδ����
	CMS_SDK_CMU_PU_EXPIRE = 11007,              //pu�Ѿ�����
	CMS_SDK_CMU_PU_ALREADY_REG = 11008,         //��ͬ��PU�Ѿ�ע��
	CMS_SDK_CMU_PU_LICENSE_NOT_MATCH = 11009,   //PU��license��ƥ��
	CMS_SDK_CMU_CU_LOGIN_SESS_ILLEGAL = 11010,  //CU�����е�¼Session�Ƿ�
	CMS_SDK_CMU_SS_OPE_TYPE_INVALID = 11011,    //CU���¶��ĵĲ���������Ч
	CMS_SDK_CMU_CUI_NOT_REG = 11012,            //CUIδ��CMUע��
	CMS_SDK_CMU_PUI_NOT_REG = 11013,            //PUIδ��CMUע��
	CMS_SDK_CMU_VTDU_ALREADY_REG = 11014,       //��ͬ��VTDU�Ѿ�ע��
	CMS_SDK_CMU_DEV_NO_ROUTER = 11015,          //�豸�޷�·��
	CMS_SDK_CMU_DEV_RSP_TIMEOUT = 11016,        //�豸Ӧ��ʱ
	CMS_SDK_CMU_CMU_ALREADY_CONN = 11017,       //��ͬ��CMU�Ѿ�����
	CMS_SDK_CMU_CMU_NOT_CONN = 11018,           //CMUδ����
	CMS_SDK_CMU_CMU_INVALID_CMU_URI = 11019,    //��ЧCMU URI
	CMS_SDK_CMU_PU_NOT_ONLINE = 11020,          //PU������
	CMS_SDK_CMU_USER_PTZ_PRI_LOW = 11021,       //�û�PTZ����Ȩ�޵�
	CMS_SDK_CMU_NOT_SUPPORT = 11022,            //�ù��ܲ�֧��
	CMS_SDK_CMU_ALARM_ILLEGAL = 11023,          //������ĸ澯
	CMS_SDK_CMU_STOP_BY_BYE = 11024,            //�Ի����Ҷ�
	CMS_SDK_CMU_DOMAIN_NOT_IN_TOP = 11025,      //Domianδ����
	CMS_SDK_CMU_DEV_ADDR_INVALID = 11026,       //�豸��ַ��Ч

	//{{SWITCH=11500
	CMS_SDK_CMU_SWITCH_DEV_NOT_AVAILABLE = 11500,       //���뽻�����豸������
	//CMS_SDK_CMU_SWITCH_DST_NOT_AVAILABLE = 11501,       //����Ŀ�Ĳ�����
	//CMS_SDK_CMU_SWITCH_SRC_NOT_AVAILABLE = 11502,       //����Դ������
	CMS_SDK_CMU_SWITCH_DST_EXIST_SWITCH= 11503,         //����Ŀ�Ĵ��ڽ���
	CMS_SDK_CMU_SWITCH_SRC_EXIST_SWITCH= 11504,         //����Դ���ڽ���
	//CMS_SDK_CMU_SWITCH_POST_MSG_TO_SRC_FAILED= 11505,   //����ʱ������Ϣ��Դʧ��
	//CMS_SDK_CMU_SWITCH_POST_MSG_TO_VTDU_FAILED= 11506,  //����ʱ������Ϣ��Ŀ��ʧ��
	//CMS_SDK_CMU_SWITCH_POST_MSG_TO_DST_FAILED= 11507,   //����ʱ������Ϣ��VTDUʧ��
	CMS_SDK_CMU_SWITCH_SRC_NODE_NOT_EXIST = 11508,      //����Դ�ڵ㲻����
	CMS_SDK_CMU_SWITCH_DST_NODE_NOT_EXIST = 11509,      //����Ŀ�Ľڵ㲻����
	CMS_SDK_CMU_SWITCH_INVALID_DIALOG_ID = 11510,       //��Ч�Ľ����ỰID,�Ự��δ�������Ѿ�����
	CMS_SDK_CMU_SWITCH_ADDRESS_RULE_SWITCH_FAILED = 11511,      //����ʱתΪ��ַ����ʧ��
	CMS_SDK_CMU_SWITCH_VTDU_NOT_AVAILABLE = 11512,      //�Ҳ������õ�VTDU
	CMS_SDK_CMU_SWITCH_GAP_VTDU_NOT_AVAILABLE = 11513,  //�Ҳ������õ���բVTDU
	CMS_SDK_CMU_SWITCH_VTDU_PORT_ALLOC_FAILED = 11514,  //VTDUͨ����˿ڷ���ʧ��
	CMS_SDK_CMU_SWITCH_EXCEPTION = 11515,	            //����ʱ����������쳣
	CMS_SDK_CMU_SWITCH_ADD_BAK_SRC_FAILED = 11516,	    //����ʱ����µı���Դ����ʧ��
	CMS_SDK_CMU_SWITCH_STOP_SWITCH_FOR_NEW_BAK_SRC_FAILED = 11517,	    //����ʱΪ�����µı���Դֹͣԭ�н���ʧ��
	CMS_SDK_CMU_SWITCH_AGAIN = 11518,                   //��Դ��ʱ���ɵã���VTDU���ߵȣ���������
	CMS_SDK_CMU_SWITCH_TASK_TIMEOUT = 11519,            //CMU��������ʱ
	CMS_SDK_CMU_SWITCH_STOP_BY_BYE = 11520,             //�����Ի����Ҷ�
	CMS_SDK_CMU_SWITCH_VCR_FAILED = 11521,              //VCR����ʧ��
	//}}SWITCH


	CMS_SDK_CMU_END = CMS_SDK_CMU_BEGIN + 999,      //cmu���������ֵ

	//pui������ 12000-12999
	CMS_SDK_PUI_BEGIN = 12000,                  //pui��������ʼֵ

	CMS_SDK_PUI_PU_ALREAD_EXIST = 12001,        //��ͬ��PU�Ѿ�ע��
	CMS_SDK_PUI_PU_OVERLORD = 12002,            //pui����pu������
	CMS_SDK_PUI_SIP_BODY_NULL = 12003,          //SIP��Ϣ��Ϊ��
	CMS_SDK_PUI_NOT_IN_SERVICE = 12004,         //PUIδ�������̬
	CMS_SDK_PUI_ADD_SWITCH_FAIL = 12005,        //PUI��ӽ���ʧ��
	CMS_SDK_PUI_GET_CAM_PROTO_FAIL = 12006,     //PUI��ȡ����ͷ����Э��ʧ��
	CMS_SDK_PUI_GET_PU_CONFIG_FAIL = 12007,     //PUI��ȡǰ������ʧ��
	CMS_SDK_PUI_SET_PU_CONFIG_FAIL = 12008,     //PUI����ǰ������ʧ��
	CMS_SDK_PUI_PU_NOT_ONLINE = 12009,          //PU������
	CMS_SDK_PUI_PU_OPE_TIMEOUT = 12010,         //PU������ʱ
	CMS_SDK_PUI_PU_OPE_FAIL = 12011,            //PU����ʧ��
	CMS_SDK_PUI_PU_TYPE_ILLEGAL = 12012,        //PU���ͷǷ�
	CMS_SDK_PUI_PU_NODISK = 12013,              //ǰ����Ӳ��
	CMS_SDK_PUI_PU_RECORDING = 12014,           //����¼���Ԥ¼
	CMS_SDK_PUI_PU_RECPLAYING = 12015,          //���ڷ���
	CMS_SDK_PUI_PU_FILE_NOT_EXIT = 12016,		//�ļ�������(��·�����޴��ļ�)
	CMS_SDK_PUI_PU_RECLENGTH_ZERO = 12017,		//�ļ���СΪ��(¼��ʱ������)
	CMS_SDK_PUI_PU_RECLOG_NOT_EXIST = 12018,    //reclog�ļ�������
	CMS_SDK_PUI_PU_ASF_STATE_ERROR = 12019,     //����ASF�ļ�״̬����ȷ
	CMS_SDK_PUI_PU_PATH_CREATE_FAIL = 12020,    //Ŀ¼����ʧ��,�����Ƿ����ֻ��
	CMS_SDK_PUI_PU_NO_PIC_FILE = 12021,         //��ǰ��ץ��ͼƬ
	CMS_SDK_PUI_PU_PIC_FILE_TOO_MUCH = 12022,   //ǰ��ץ��ͼƬ����

	CMS_SDK_PUI_NVR_NO_BUF_RECORD = 12023,                      //¼�񻺴��¼������,����������¼��(NVRǰ�˷���ʱ)
	CMS_SDK_PUI_NVR_INVALID_PU_RECCHN = 12024,                  //¼��ͨ�������ã�һ���Ǵ�NVRͨ����δ�����豸(NVRǰ��¼��ʱ)
	CMS_SDK_PUI_NVR_NOT_ALLOW_DELETE_PU_REC = 12025,            //��ɾ��NVRǰ��¼��Ȩ��
	CMS_SDK_PUI_NVR_BITRATE_OVERFLOW = 12026,                   //�����ʹ���(NVR2820 BitRate:64 - 2048)
	CMS_SDK_PUI_NVR_PIC_TOO_MUCH = 12027,                       //NVRǰ��ץ��ͼƬ�����࣬����С������Χ
	CMS_SDK_PUI_NVR_NO_SNAPSHOT_PIC = 12028,                    //��NVRǰ��ץ��ͼƬ
	CMS_SDK_PUI_NVR_NO_VALID_REC_DISK = 12029,                  //NVR�޿���¼����̷���
	CMS_SDK_PUI_PU_NOT_SUPPORT = 12030,                         //��ǰ�˲�֧�ִ˹���

	CMS_SDK_PUI_PU_VCR_NOT_SUPPORT_PLAY_RATE = 12031,           //ǰ�˲�֧�ֵ�VCR��������
	CMS_SDK_PUI_PU_VCR_NOT_SUPPORT_CMD = 12032,                 //ǰ�˲�֧�ֵ�VCR��������

	CMS_SDK_PUI_QUERY_MAPPED_ADDR_FAILED = 12033,               //��ѯ������ַʧ��
	CMS_SDK_PUI_QUERY_MAPPED_ADDR_TIMEOUT = 12034,              //��ѯ������ַ��ʱ
	CMS_SDK_PUI_QUERY_MAPPED_ADDR_CANCELED = 12035,             //��ѯ������ַ������ȡ��

	CMS_SDK_PUI_PUNCH_FAILED = 12036,                           //��ַ̽��ʧ��
	CMS_SDK_PUI_PUNCH_TIMEOUT = 12037,                          //��ַ̽�ⳬʱ
	CMS_SDK_PUI_PUNCH_CANCELED = 12038,                         //��ַ̽�������ȡ��
	CMS_SDK_PUI_EXCEED_PLAY_REC_MAX_NUM = 12039,                //����ǰ����������Ŀ
	CMS_SDK_PUI_GET_LOCAL_ADDR_FAILED = 12040,                  //��ȡ���ص�ַʧ��
	CMS_SDK_PUI_MEDIA_TYPE_ERROR = 12041,                       //�����ý���ʽ
	CMS_SDK_PUI_PU_VCR_TRY_AGAIN = 12042,                       //���ڿ���ǰ�˷����Ժ����VCR����
	CMS_SDK_PUI_TRANS_OPR_TYPE_ERROR = 12043,                   //͸�����ݲ������ʹ���

	CMS_SDK_PUI_PU_UNKNOWN = CMS_SDK_PUI_BEGIN + 998,                //ǰ��δ֪����
	CMS_SDK_PUI_END = CMS_SDK_PUI_BEGIN + 999,      //pui���������ֵ

	//cui������ 13000-13999 ��ǰ���ú� 13028
	CMS_SDK_CUI_BEGIN = 13000,                  //cui��������ʼֵ

	CMS_SDK_CUI_USERNAME_OR_PASSWORD_WRONG = 13001, //�û��������������
	CMS_SDK_CUI_CONNECT_CUI_FAIL = 13002,           //�޷�������CUI������
	CMS_SDK_CUI_DISCONNECT_CUI = 13003,             //��CUI�Ͽ�����
	CMS_SDK_CUI_NOT_IN_SERVICE = 13004,             //CUIδ�������̬
	CMS_SDK_CUI_OPR_TIME_OUT = 13005,               //����ʱ
	CMS_SDK_CUI_CU_NOT_ONLINE = 13006,              //CU�����ߣ�����Ƿ�
	CMS_SDK_CUI_CU_NO_PERMISSION = 13007,           //CUû��Ȩ��
	CMS_SDK_CUI_QUERYING_PRIVILEGE_DATA = 13026,    //���ڲ�ѯȨ�����ݣ����Ժ�����
	CMS_SDK_CUI_QUERYING_INNER_DATA = 13027,        //���ڲ�ѯ�ڲ����ݣ����Ժ�����
	CMS_SDK_CUI_DIALOG_NOT_ESTABLISHED = 13008,     //�Ự��δ�������������ܾ�
	CMS_SDK_CUI_NOT_SUBSCRIBE = 13009,              //�豸δ���Ĺ�
	CMS_SDK_CUI_PARAMS_INVALID = 13010,             //�����Ƿ�������
	CMS_SDK_CUI_DIALOG_TERMINATED = 13011,          //�Ự���Զ���������
	CMS_SDK_CUI_SIP_MSG_FAIL = 13012,               //����SIP����ʧ�ܣ����ܷ�����������
	CMS_SDK_CUI_TAS_OFFLINE = 13013,                //TAS�����������ߣ�����ʧ��
	CMS_SDK_CUI_INVALID_PARAM = 13014,              //��Ч�������
	CMS_SDK_CUI_IN_SUBSCRIBE = 13015,               //���ڶ�����
	CMS_SDK_CUI_UNKNOW_MSG = 13016,                 //����ʶ����Ϣ
	CMS_SDK_CUI_TASK_DEL = 13017,                   //task����
	CMS_SDK_CUI_TCP_TIMEOUT = 13018,                //tcp�˽�����Ϣʧ��
	CMS_SDK_CUI_PE_OFFLINE = 13019,                //����ģ�鲻����
	CMS_SDK_CUI_RECORD_END = 13020,                //¼�񲥷�ֹͣ
	CMS_SDK_CUI_PASSWORD_ERROR = 13021,            //����У�����
	CMS_SDK_CUI_EXCEED_CU_MAX_NUM = 13022,         //�������cu�û���
	CMS_SDK_CUI_CROSS_DOMAIN_LOGIN = 13023,        //�ݲ���������¼
	CMS_SDK_CUI_STATE_ERROR = 13024,               //����״̬����ȷ
	CMS_SDK_CUI_RSP_MESSAGE_TOO_LONG = 13025,      //Ӧ����Ϣ�����

	CMS_SDK_CUI_END = CMS_SDK_CUI_BEGIN + 999,      //cui���������ֵ

	//vtdu������ 14000-14999
	CMS_SDK_VTDU_BEGIN = 14000,                 //vtdu��������ʼֵ

	CMS_SDK_VTDU_ADD_SWITCH_TO_DS_FAILED = 14001,       //[VTDU]��ӽ�����DataSwitchʧ��
	CMS_SDK_VTDU_REMOVE_SWITCH_FROM_DS_FAILED = 14002,  //[VTDU]��DataSwitchɾ������ʧ��
	CMS_SDK_VTDU_ADD_SWITCH_INVALID_ARGUMENT = 14003,   //[VTDU]��ӽ�������Ĳ�������
	CMS_SDK_VTDU_TASK_TIMEOUT = 14004,                  //[VTDU]������ʱ
	CMS_SDK_VTDU_EXCEPTION = 14005,                     //[VTDU]ϵͳ�쳣
	CMS_SDK_VTDU_WAIT_ACK_TIMEOUT = 14006,              //[VTDU]�ȴ�ȷ��Ӧ��ʱ
	CMS_SDK_VTDU_STUN_UPDATE_FAILED = 14007,            //[VTDU]����Stun��ַʧ��
	CMS_SDK_VTDU_WITHOUT_VTDU_ADDR_OF_NETSEG = 14008,   //[VTDU]û����Ӧ���ε�VTDU��ַ

	CMS_SDK_VTDU_QUERY_MAPPED_ADDR_FAILED = 14500,      //[VTDU]��ѯ������ַʧ��
	CMS_SDK_VTDU_QUERY_MAPPED_ADDR_TIMEOUT = 14501,     //[VTDU]��ѯ������ַ��ʱ
	CMS_SDK_VTDU_QUERY_MAPPED_ADDR_CANCELED = 14502,    //[VTDU]��ѯ������ַ������ȡ��

	CMS_SDK_VTDU_PUNCH_FAILED = 14520,                  //[VTDU]��ַ̽��ʧ��
	CMS_SDK_VTDU_PUNCH_TIMEOUT = 14521,                 //[VTDU]��ַ̽�ⳬʱ
	CMS_SDK_VTDU_PUNCH_CANCELED = 14522,                //[VTDU]��ַ̽�������ȡ��
	CMS_SDK_VTDU_INVALID_PUNCH_PARAM = 14523,           //[VTDU]��Ч��Punch����

	CMS_SDK_VTDU_END = CMS_SDK_VTDU_BEGIN + 999,    //vtdu���������ֵ

	//pmcs������ 15000-15999
	CMS_SDK_PMCS_BEGIN = 15000,

	CMS_SDK_PMCS_NOT_IN_SERVICE = 15001,         //pmcsδ�������̬
	CMS_SDK_PMCS_OPR_TIME_OUT = 15002,          //ҵ������ʱ
	CMS_SDK_PMCS_WEB_NOT_ONLINE = 15003,          //pmcs web ������,����Ƿ�
	CMS_SDK_PMCS_DIALOG_TERMINATED = 15004,        //��pms�Ự���Զ���������
	CMS_SDK_PMCS_SIP_MSG_FAIL = 15005,               //����SIP����ʧ�ܣ����ܷ�����������
	CMS_SDK_PMCS_DISCONNECT_PMCS = 15006,             //��PMCS�Ͽ�����
	CMS_SDK_PMCS_PMS_NOT_IN_SERVICE = 15007,          //PMSδ�������
	CMS_SDK_PMCS_3AS_NOT_IN_SERVICE = 15008,      //3asδ�������
	CMS_SDK_PMCS_RCS_NOT_IN_SERVICE = 15009,					//rcs���ڷ���̬
	CMS_SDK_PMCS_RCS_SUBSCRIBE_ALREADY_EXIST = 15010,		//�����Ѵ���
	CMS_SDK_PMCS_RCS_SUBSCRIBE_NOT_EXIST = 15011,			//��δ����
	CMS_SDK_PMCS_PMCAGENT_NOT_EXIST = 15012,		//PMCAGENT������
	CMS_SDK_PMCS_PMC_NOT_IN_SERVICE = 15013,		//PMC�쳣���ܾ�����
	CMS_SDK_PMCS_PMS_PLATFORM_RELATION_ERROR = 15014,    //ƽ̨��ϵ���
	CMS_SDK_PMCS_PMS_OPT_ERROR = 15015,                  //ҵ����ʧ��
	CMS_SDK_PMCS_PMS_SUBPLATFORM_ADD_OPT_ERROR = 15016,  //��Ӵ�ƽ̨���¼�ƽ̨ʱ��ƽ̨���¼�ƽ̨����ʧ�ܣ�����ƽ̨���ϼ�ƽ̨�в���
	CMS_SDK_PMCS_PMS_SUBPLATFORM_DEL_OPT_ERROR = 15017,  //ɾ����ƽ̨���¼�ƽ̨ʱ����ƽ̨���ϼ�ƽ̨����ʧ�ܣ���ƽ̨���¼�ƽ̨�Ѵ���
	CMS_SDK_PMCS_DETECTED_NO_TOPO_INFO = 15018,          //δ̽�⵽��ȷ��topoƽ̨
	CMS_SDK_PMCS_NAT_DATA_ERR = 15019,                   //ӳ���������󣬲����������
	CMS_SDK_PMCS_GBS_NOT_IN_SERVICE = 15020,             //GBSδ�������̬
	CMS_SDK_PMCS_IMPORT_FILE_FAIL = 15021,               //�豸��������ʧ��
	CMS_SDK_PMCS_IMPORT_DEV_INFO_ERR = 15022,            //���������豸�ļ�ģ�����
	CMS_SDK_PMCS_GET_DEV_MOD_FAIL = 15023,        //��ȡ�豸�ͺ�ʧ��
	CMS_SDK_PMCS_DEV_IMPORT_FILE_NOT_EXIST   = 15024,    //���������豸�ļ�������
	CMS_SDK_PMCS_DEV_MODEL_EMPTY             = 15025,    //û�в鵽�豸�ͺ�
	CMS_SDK_PMCS_QRY_DEV_FAIL                = 15026,    //��ѯ�����豸ʧ��
	CMS_SDK_PMCS_QRY_GBDEV_FAIL              = 15027,    //��ѯ�����豸ʧ��
	CMS_SDK_PMCS_QRY_DEVICE_EMPTY            = 15028,    //��ѯ��0���豸

	CMS_SDK_PMCS_END = CMS_SDK_PMCS_BEGIN + 999,    //pmcs���������

	// CU������ 16000 - 16999 
	CMS_SDK_CU_BEGIN = 16000,						// cu������ο�ʼ

	CMS_SDK_CU_END = CMS_SDK_CU_BEGIN + 999,	// cu�����������

	//als������ 17000-17999
	CMS_SDK_ALS_BEGIN = 17000,                  //als��������ʼֵ

	CMS_SDK_ALS_INVALID_ALARM_LINK = 17001,     //��Ч�ĸ澯��������
	CMS_SDK_ALS_DB_OPR_FAIL = 17002,            //���ݿ����ʧ��
	CMS_SDK_ALS_DB_OPR_TIMEOUT = 17003,         //���ݿ������ʱ
	CMS_SDK_ALS_ALARM_LINK_NOT_EXIST = 17004,   //�澯����������

	CMS_SDK_ALS_END = CMS_SDK_ALS_BEGIN + 999,      //als���������ֵ

	//tvs������ 18000-18999
	CMS_SDK_TVS_BEGIN = 18000,                  //tvs��������ʼֵ
	CMS_SDK_TVS_TVWALL_NOT_EXIST = 18001,       //����ǽ������
	CMS_SDK_TVS_TV_PLAY_NOT_EXIST = 18002,      //���Ӳ��Ų�����
	CMS_SDK_TVS_TV_OCCUR_NOT_MATCH = 18003,     //���Ӵ������Ͳ�ƥ��
	CMS_SDK_TVS_TV_SRC_NOT_MATCH = 18004,       //���Ӳ���Դ����ƥ��
	CMS_SDK_TVS_3PC_INVITE_FAIL = 18005,        //3PC����ʧ�ܣ�sip�����
	CMS_SDK_TVS_3PC_INVITE_TIMEOUT = 18006,     //3PC���г�ʱ
	CMS_SDK_TVS_TV_POLL_OPR_FAIL = 18007,       //��ѯ����ʧ��
	CMS_SDK_TVS_DB_OPR_FAIL = 18008,            //���ݲ���ʧ��
	CMS_SDK_TVS_DB_OPR_TIMEOUT = 18009,         //���ݲ�����ʱ
	CMS_SDK_TVS_ILLEGAL_SCHEME_NAME = 18010,    //��Ч��Ԥ������
	CMS_SDK_TVS_ILLEGAL_SCHEME_ID = 18011,      //��Ч��Ԥ��ID
	CMS_SDK_TVS_SCHEME_NOT_EXIST = 18012,       //Ԥ��������
	CMS_SDK_TVS_CUI_LOGIN_SESS_ILLEGAL = 18013, //�Ƿ���cui��¼session
	CMS_SDK_TVS_TV_DEC_BIND_ERROR = 18014,      //���ӻ��󶨽���������
	CMS_SDK_TVS_ILLEGAL_ENC_CHN = 18015,        //��Ч�ı���ͨ��
	CMS_SDK_TVS_ILLEGAL_SHEME_POLL_CFG = 18016, //��Ч��Ԥ����ѯ����
	CMS_SDK_TVS_SET_DEC_STYLE_TIMEOUT = 18017,  //���ý��������ʱ
	CMS_SDK_TVS_NOT_LOGIN_CMU = 18018,          //δ��¼cmu
	CMS_SDK_TVS_LOGIN_CMU_BYE_3PC = 18019,      //cmu����bye 3PC����
	CMS_SDK_TVS_3PC_DIALOG_TERMINATE = 18020,   //3PC�����쳣��ֹ��sip�����
	CMS_SDK_TVS_LOCKED_TVWALL_OR_CHANNEL_NOT_EXIST = 18021,   //�����ڵ���������ͨ��������������ǽԤ��
	CMS_SDK_TVS_UNLOCK_TVWALL_OR_CHANNEL_PASS_ERR = 18022,   //��������ͨ������Ԥ���������
	CMS_SDK_TVS_INVALID_WATCHOINT_SAVE_REQ = 18023, //��Ч�Ĺۿ��㱸������
	CMS_SDK_TVS_SAVED_WATCHPOINT_NOT_EXIST = 18024, //�����ڵı��ݹۿ���
	CMS_SDK_TVS_TVWALL_OR_CHANNEL_LOCKED = 18025, //����ǽԤ������ͨ������
    CMS_SDK_TVS_TVWALL_TVCHN_TASK_EXIST = 18026, //����ͨ����ռ��
    CMS_SDK_TVS_TVWALL_TIMEOUT = 18027,          //������ʱ
	CMS_SDK_TVS_END = CMS_SDK_TVS_BEGIN + 999,      //tvs���������ֵ

	//rcs������ 19000-19999
	CMS_SDK_RCS_BEGIN = 19000,                  //rcs��������ʼֵ
	CMS_SDK_RCS_RMS_NOT_ONLINE = 19001,             //[RCS]rms������
	CMS_SDK_RCS_NRU_NOT_ONLINE = 19002,             //[RCS]nru������
	CMS_SDK_RCS_DB_OPR_FAIL = 19003,                //[RCS]���ݿ����ʧ��
	CMS_SDK_RCS_DB_OPR_TIMEOUT = 19004,             //[RCS]���ݿ������ʱ
	CMS_SDK_RCS_PU_NOT_ONLINE = 19005,              //[RCS]ǰ���豸������
	CMS_SDK_RCS_PU_CONFIG_NOT_AVAILABLE = 19006,    //[RCS]ǰ�����ò������ɵ�
	CMS_SDK_RCS_RECORD_PARAM_NOT_EXIST = 19007,     //[RCS]¼�����������
	CMS_SDK_RCS_RECORD_TASK_ALREADY_EXIST = 19008,  //[RCS]¼�������Ѿ�����
	CMS_SDK_RCS_EXCEPTION = 19009,                  //[RCS]�������쳣

	CMS_SDK_RCS_ALLOC_RECORD_RES_FAILED = 19010,    //[RCS]����¼����Դʧ��
	CMS_SDK_RCS_START_RECORD_FAILED = 19011,        //[RCS]����¼��ʧ��
	CMS_SDK_RCS_RECORD_TASK_NOT_EXIST = 19012,      //[RCS]��������Ӧ��¼������
	CMS_SDK_RCS_UPDATE_RECTYPE_FAILED = 19013,      //[RCS]����¼������ʧ��
	CMS_SDK_RCS_DEV_NOT_REGISTER = 19014,           //[RCS]�豸δע��
	CMS_SDK_RCS_TASK_TIMEOUT = 19015,               //[RCS]������ʱ
	CMS_SDK_RCS_NOT_REG_TO_CMU = 19016,             //[RCS]δע����CMU
	CMS_SDK_RCS_PLAYBACK_TASK_NOT_EXIST = 19017,    //[RCS]�������񲻴���
	CMS_SDK_RCS_DEV_NOT_EXIST = 19018,              //[RCS]�豸������
	CMS_SDK_RCS_SS_OPE_TYPE_INVALID = 19019,        //[RCS]��Ч�Ķ�������
	CMS_SDK_RCS_DEV_NO_ROUTER = 19020,              //[RCS]�����޷���·�ɵ�ָ���豸
	CMS_SDK_RCS_STOP_BY_BYE = 19021,                //[RCS]�Ի����Ҷ�
	CMS_SDK_RCS_NOT_SUPPORT = 19022,                //[RCS]�ù��ܲ�֧��
	CMS_SDK_RCS_CONVERT_URI_DOMAIN_FAILED = 19023,  //[RCS]ת��URI������ʧ�ܣ������Ҳ�����Ӧ������Ϣ
	CMS_SDK_RCS_VCR_TRY_AGAIN = 19024,              //[RCS]���ڿ���¼��طţ��Ժ����VCR����

	CMS_SDK_RCS_END = CMS_SDK_RCS_BEGIN + 999,          //rcs���������ֵ

	//uas������ 20000-20999 (��ǰ���ú�Ϊ20003)
	CMS_SDK_UAS_BEGIN = 20000,                  //uas��������ʼֵ
	CMS_SDK_UAS_DBERROR = 20001,                // ���ݿ��������������һ��
	CMS_SDK_UAS_SAVE_CONFIG_FAILED = 20002,     // ��������ʧ��
	CMS_SDK_UAS_END = CMS_SDK_UAS_BEGIN + 999,      //uas���������ֵ

	//mps������ 21000-21999
	CMS_SDK_MPS_BEGIN = 21000,                  //mps��������ʼֵ
	CMS_SDK_MPS_INVAILD_MSG  = 21001,			//�������Ϣ��
	CMS_SDK_MPS_MSG_TIME_OUT	 = 21002,		//��Ϣ����ʱ
	CMS_SDK_MPS_DB_CONNECT_ERROR = 21003,		//�������ݿ�ʧ��
	CMS_SDK_MPS_DB_OPR_ERROR	= 21004,		//�������ݿ�ʧ��
	CMS_SDK_MPS_DB_OPR_TIMEOUT	= 21005,		//�������ݿⳬʱ
	CMS_SDK_MPS_DB_NO_RESULT	= 21006,		//���ݲ�ѯ����������
	CMS_SDK_MPS_MAP_EXISTS		= 21007,		//��ͼ�Ѵ���
	CMS_SDK_MPS_MAP_ELEMENT_EXISTS	= 21008,	//ͼԪ�Ѵ���
	CMS_SDK_MPS_ICON_PATH_NOT_EXISTS = 21009,	//��ͼͼԪĿ¼������
	CMS_SDK_MPS_PARAMS_INVAILD		= 21010,	//�������Ϸ�
	CMS_SDK_MPS_POINT_OUT_OF_RANG	= 21011,	//У�����ڷ�Χ֮��
	CMS_SDK_MPS_NOT_CONFIG_GPS_STORE	= 21012,	//δ����GPS�洢
	CMS_SDK_MPS_END = CMS_SDK_MPS_BEGIN + 999,      //mps���������ֵ

	//pms������ 22000-22999 ��ǰ���ô�����Ϊ22057
	CMS_SDK_PMS_BEGIN = 22000,                  //pms��������ʼֵ
	CMS_SDK_PMS_INVALID_PARAM = 22012,          //��Ч���������
	CMS_SDK_PMS_DATA_NOT_EXIST = 22019,         //���ݲ�����
	CMS_SDK_PMS_NOT_IN_SERVICE = 22001,         //PMSδ�������̬
	CMS_SDK_PMS_INVALID_OPERATION = 22020,      //�Ƿ�������ҵ���ϲ�������˲���
	CMS_SDK_PMS_EXECUTE_TIMEOUT = 22039,        //ִ�г�ʱ

	CMS_SDK_PMS_3AS_NOT_IN_SERVICE = 22002,     //3asδ�������̬
	CMS_SDK_PMS_INVALID_PMC_URI = 22003,        //��Ч��pmc uri
	CMS_SDK_PMS_PMS_ALREADY_CONN = 22004,       //�ظ���¼
	CMS_SDK_PMS_DB_OPR_FAIL = 22005,            //���ݿ��������
	CMS_SDK_PMS_USERNAME_OR_PASSWORD_WRONG = 22006, //�û������������
	CMS_SDK_PMS_INVALID_SERVER_URI = 22007,         //��Ч��SERVER URI
	CMS_SDK_PMS_DB_OPR_TIMEOUT = 22008,         //���ݿ������ʱ
	CMS_SDK_PMS_WEB_ONT_ONLINE = 22009,			//WEB������
	CMS_SDK_PMS_PMS_UNKNOW_DEVTYPE = 22010,     //��Ч���豸����
	CMS_SDK_PMS_DB_DATA_NOT_EXIST = 22011,		//���ݿ��в�����Ҫ�ҵ�����
	CMS_SDK_PMS_OPR_FAIL = 22015,               //����ʧ��

	CMS_SDK_PMS_PLATFORM_ALREADY_IN_TOPO = 22014,//������ƽ̨�Ѿ����������������˳�������
	CMS_SDK_PMS_REPEATED_PLATFORM_RELATION = 22021, //�ظ���ƽ̨��ϵ
	CMS_SDK_PMS_GET_MOSTADAPTIVEIP_FAILED = 22051,  //��ȡ�����ƥ��IPʧ��, �����Ƿ���Ҫ����NATӳ��
	CMS_SDK_PMS_DETECTED_NOT_CONSISTENCE_TOPO_CONF = 22017, //̽�⵽��һ�µ�TOPO���� 
	CMS_SDK_PMS_MODIFY_CONFIG_FAILED = 22016,               //�޸�PMS�������ļ�ʧ��
	CMS_SDK_PMS_MODIFY_COMMONCONFIG_FAILED = 22018,         //�޸Ĺ��������ļ�ʧ��

	CMS_SDK_PMS_GET_ROUTEINFO_ERR = 22022,              //��ȡ����·����Ϣʧ��
	CMS_SDK_PMS_SET_ROUTEINFO_ERR = 22023,              //��������·��ʧ��
	CMS_SDK_PMS_DEL_ROUTEINFO_ERR = 22024,              //ɾ������·��ʧ��
	CMS_SDK_PMS_ADD_ROUTEINFO_ERR = 22025,              //�������·��ʧ��
	CMS_SDK_PMS_RESET_ERR = 22026,                      //��ԭ��������ʧ��
	CMS_SDK_PMS_SOFTWAREINFO_QRY_ERR = 22027,           //�����Ϣ��ȡʧ��
	CMS_SDK_PMS_SOFTWAREINFO_MOD_ERR = 22028,           //�����Ϣ�޸�ʧ��
	CMS_SDK_PMS_GET_DEFAULTROUTE_ERR = 22029,           //��ȡĬ������ʧ��
	CMS_SDK_PMS_SET_DEFAULTROUTE_ERR = 22030,           //����Ĭ������ʧ��
	CMS_SDK_PMS_MOD_PLATFORMTIME_ERR = 22031,           //�޸�ƽ̨ʱ��ʧ��
	CMS_SDK_PMS_CTRL_PMS_ERR = 22032,                   //PMSƽ̨����ʧ��
	CMS_SDK_PMS_MOD_NETCARDINFO_ERR = 22033,            //�޸�������Ϣʧ��
	CMS_SDK_PMS_QRY_PLATFORMTIME_ERR = 22034,           //��ѯƽ̨ʱ��ʧ��
	CMS_SDK_PMS_ADD_NETNAT_ERR = 22035,                 //�������NATʧ��
	CMS_SDK_PMS_DEL_NETNAT_ERR = 22036,                 //ɾ������NATʧ��
	CMS_SDK_PMS_ADD_REPEATED_DATA_ERR = 22037,          //�ظ��������
	CMS_SDK_PMS_NETWORK_RESTART_ERR = 22038,            //�����������ʧ��
	CMS_SDK_PMS_INVTASK_LIMITED = 22040,                //�����������롢�ָ���������ͬʱֻ�ܴ���һ������,��������������Ӧ���������
	CMS_SDK_PMS_ADD_USER_ALREADY_EXIST = 22041,         //��ӵ��û��Ѿ�����
	CMS_SDK_PMS_MOD_USER_NOT_EXIST = 22042,             //Ҫ�޸ĵ��û�������
	CMS_SDK_PMS_DEL_NTP_SERVER_ERR = 22043,             //ɾ��NTP����ʧ��
	CMS_SDK_PMS_SET_NTP_SERVER_ERR = 22045,             //����NTP����ʧ��
	CMS_SDK_PMS_REMOVE_USBBAK_FAILED = 22044,           //�Ƴ�USB������ʧ�ܣ������Ǳ���������ʹ�ã����Ժ�����
	CMS_SDK_PMS_RECORD_USERLOG_FAILED = 22046,          //��¼�û���־ʧ��
	CMS_SDK_PMS_EXPORTMIRROR_TASK_ALREADY_EXIST = 22047,//��������������Ѿ�����
	CMS_SDK_PMS_ABNORMAL_STATUS = 22048,                //PMS�����쳣״̬��ֻ�ṩ���޷���
	CMS_SDK_PMS_INVALID_NTP_SERVER_ADDR = 22049,        //��Ч��NTP�����ַ
	CMS_SDK_PMS_DOMAINNAME_ALREADY_EXIST = 22050,       //Ҫ�޸ĵ������������¼�ƽ̨��ͻ������ʧ��
	CMS_SDK_PMS_SET_MAIL_CFG_ERR = 22052,               //��������ʧ��
	CMS_SDK_PMS_QRY_MAIL_CFG_ERR = 22053,               //�����ò�ѯʧ��
	CMS_SDK_PMS_SEND_TEST_MAIL_ERR = 22054,             //�����ʼ�����ʧ��,���ʼ�����ִ��ʧ��
	CMS_SDK_PMS_MAILTO_LIST_IS_NULL = 22055,            //�ʼ������б�Ϊ�գ������ʼ�����ʧ��
	CMS_SDK_PMS_MAILSERVER_CFG_ERR = 22056,             //�ʼ��������������󣬲���Ϊ��

	CMS_SDK_PMS_END = CMS_SDK_PMS_BEGIN + 999,      //pms���������ֵ

	//rmssdk������ 23000-23999
	CMS_SDK_RMSSDK_BEGIN = 23000,                   //rmssdk���������ֵ
	CMS_SDK_RMSSDK_END = CMS_SDK_RMSSDK_BEGIN + 999,    //rmssdk���������ֵ

	//scsdk������ 24000-24999
	CMS_SDK_SCSSDK_BEGIN = 24000,                   //scsdk���������ֵ
	CMS_SDK_SCSSDK_END = CMS_SDK_SCSSDK_BEGIN + 999,    //scsdk���������ֵ

	//gbs������ 25000-25999�� ��ǰ���ú�Ϊ25025
	CMS_SDK_GBS_BEGIN = 25000,
	CMS_SDK_GBS_INVALID_PARAM = 25007,              //�����Ƿ�
	CMS_SDK_GBS_INVALID_OPERATE = 25008,            //�Ƿ�����
	CMS_SDK_GBS_CMSSDK_SENDMSG_FAILED = 25001,      //cmssdk������Ϣʧ��
	CMS_SDK_GBS_CMSSDK_RESPONSE_TIMEOUT = 25002,    //cmssdk�ظ���ʱ
	CMS_SDK_GBS_SAVE_CONFIG_FAILED = 25003,         //�����ļ�����ʧ��
	CMS_SDK_GBS_DBOPR_FAILED = 25004,               //���ݿ����ʧ��
	CMS_SDK_GBS_DBOPR_TIMEOUT = 25005,              //���ݿ������ʱ
	CMS_SDK_GBS_EXIST_REPEATED_DATA = 25006,        //�����ظ�����
	CMS_SDK_GBS_DETECTED_CIRCLE_DATA = 25017,       //̽�⵽������
	CMS_SDK_GBS_INVALID_GBID = 25009,               //�����ŷǷ�
	CMS_SDK_GBS_SAME_GBID_WITH_LOCAL = 25016,       //�뱾���������ͬ
	CMS_SDK_GBS_GBDOMAIN_ID_NOT_AVAILABLE = 25020,  //������Ų����ã�Ҳ�������ͬ����Ż���ڻ�
	CMS_SDK_GBS_NOT_AVAILABLE_GBID = 25021,         //�����Ų����ã�Ҳ���ǹ����ŷǷ����߷Ǳ�����Ż������Ͳ�֧�ֻ��߱���Ѿ���ռ��
	CMS_SDK_GBS_NOT_ENOUGH_GBID = 25022,            //�ṩ�Ĺ����Ų�����Ҫ���豸��ͨ������Ҫָ��
	CMS_SDK_GBS_DATA_NOT_EXIST = 25010,             //���ݲ�����
	CMS_SDK_GBS_DATA_NOT_INTEGRITY = 25011,         //���ݲ�����������ɾ����Ӧ����
	CMS_SDK_GBS_PU_NOT_ONLINE = 25012,              //ǰ�˲�����
	CMS_SDK_GBS_OSP_SIP_POST_FAIL = 25013,          //OSPSIP������Ϣʧ��
	CMS_SDK_GBS_PU_VCR_NOT_SUPPORT_CMD = 25014,     //ǰ�˲�֧�ֵ�����
	CMS_SDK_GBS_PU_VCR_NOT_SUPPORT_PLAY_RATE = 25015,//ǰ�˲�֧�ֵĲ����ٶ�
	CMS_SDK_GBS_G800_ERROR = 25018,                 //G800����
	CMS_SDK_GBS_486_SIP_ERROR = 25019,              //486SIP����
    CMS_SDK_GBS_SUBGBDMOAIN_NOT_ONLINE = 25023,     //�����¼�������
    CMS_SDK_GBS_LOWER_DOMAIN_BUSY = 25024,          //�¼�������æ
	CMS_SDK_GBS_END = 25999,

	//cmssdk�ڲ������룺26000-26499������CMSSDK�Ĵ������Ѿ���ʹ���ˣ��Ͳ�����CMS_ǰ׺��
	CMS_SDK_BEGIN                                     = 26000,
	CMS_SDK_REINIT                                    = 26001,          //�ظ���ʼ��
	CMS_SDK_CALLBACK_INVALID                          = 26002,          //�ص�������Ч
	CMS_SDK_GW_UUID_INVALID                           = 26003,          //����UUID��Ч
	CMS_SDK_DOMAIN_NAME_INVALID                       = 26004,          //ƽ̨������Ч
	CMS_SDK_GW_ADDR_INVALID                           = 26005,          //���ص�ַ��Ч
	CMS_SDK_PLAT_ADDR_INVALID                         = 26006,          //ƽ̨��ַ��Ч
	CMS_SDK_OSPEXT_INIT_FAIL                          = 26007,          //ospext��ʼ��ʧ��
	CMS_SDK_OSPSIP_INIT_FAIL                          = 26008,          //ospsip��ʼ��ʧ��
	CMS_SDK_OSPAPP_CREATE_FAIL                        = 26009,          //ospapp����ʧ��
	CMS_SDK_GW_MSG_INVALID                            = 26010,          //������Ϣ��Ч
	CMS_SDK_OSP_POST_FAIL                             = 26011,          //OspPostʧ��
	CMS_SDK_TIMEOUT                                   = 26012,          //��ʱ����
	CMS_SDK_SEND_MSG_FAIL                             = 26013,          //��ƽ̨��������ʧ��
	CMS_SDK_CMU_DIS_CONN                              = 26014,          //δ���ӵ�CMU
	CMS_SDK_3AS_DIS_CONN                              = 26015,          //δ���ӵ�3as
	CMS_SDK_PU_REG_TIMEOUT                            = 26016,          //puע�ᳬʱ
	CMS_SDK_DEVICE_UUID_INVALID                       = 26017,          //�豸ID��Ч
	CMS_SDK_DEVICE_NAME_INVALID                       = 26018,          //�豸����Ч
	CMS_SDK_GROUP_UUID_INVALID                        = 26019,          //�豸����ID��Ч
	CMS_SDK_GROUP_NAME_INVALID                        = 26020,          //�豸��������Ч
	CMS_SDK_DOMAIN_UUID_INVALID                       = 26021,          //��ID��Ч
	CMS_SDK_DEVICE_NOT_EXIST                          = 26022,          //�豸������
	CMS_SDK_RCS_DIS_CONN                              = 26023,          //δ���ӵ�rcs
	CMS_SDK_TVS_DIS_CONN                              = 26024,          //δ���ӵ�tvs
	CMS_SDK_HAVE_NOT_SS                               = 26025,		   //��δ���ģ�����ȡ������ǰ����Ƿ��Ѷ��ģ�
	CMS_SDK_ALREADY_SS                                = 26026,		   //�ظ����ģ����ڶ���ǰ����Ƿ��ظ����ģ�
	CMS_SDK_SS_OPE_TYPE_INVALID                       = 26027,          //��Ч�Ķ��Ĳ�������
	CMS_SDK_TRANSCODE_TERMINATE                       = 26028,          //G800ת���쳣��ֹ
	CMS_SDK_PU_CONF_NTF_TIMEOUT                       = 26029,          //pu����֪ͨ��ʱ
	CMS_SDK_PU_UNREG_TIMEOUT                          = 26030,          //pu����֪ͨ��ʱ
	CMS_SDK_PU_ALREADY_REG                            = 26031,			//pu�Ѿ�ע��
	CMS_SDK_ERR_DIALOG_TERMINATE                      = 26032,			//�Ự�쳣��ֹ
	CMS_SDK_ERR_TASK_DEL                              = 26033,			//Task�Ѿ�ɾ��
	CMS_SDK_DIALOG_NOT_ESTABLISHED                    = 26034,			//�Ự��δ����
	CMS_SDK_UAS_DIS_CONN                              = 26035,          //δ���ӵ�uas
	CMS_SDK_VCR_CTRL_TIMEOUT                          = 26036,          //VCR����������Ϣ��ʱ
	CMS_SDK_PLAT_REC_PLAYBACK_FINISHED                = 26037,          //ƽ̨������������
    CMS_SDK_COMMONCONFIG_INIT_FAIL                    = 26038,          //���������ļ���ʼ��ʧ��
    CMS_SDK_PU_REG_CONFIG_NOT_COMPLETED               = 26039,          //puע����Ϣ������

	CMS_SDK_CMS_SDK_END                                   = 26499,

	//GW�����룺26500-26999
	CMS_SDK_GW_BEGIN                                      = 26500,
	CMS_SDK_GLB_ERR_NOINIT_ADAPTER                        = 26501,          //��ʼ��ADAPTER
	CMS_SDK_GLB_ERR_LOAD_ADATER_FAIL                      = 26502,          //����adapterʧ��
	CMS_SDK_GLB_ERR_INIT_ADAPTER_FAIL                     = 26503,          //��ʼ��adapterʧ��
	CMS_SDK_GLB_ERR_NOINIT_PLAT                           = 26504,          //û�г�ʼ��ƽ̨
	CMS_SDK_GLB_ERR_USERNAME_ERR                          = 26505,          //�û���Ϣ����
	CMS_SDK_GLB_ERR_PASSWORD_ERR                          = 26506,          //�û��������
	CMS_SDK_GLB_ERR_PARA_EMPTY                            = 26507,          //�������ݴ���
	CMS_SDK_GLB_ERR_DLL_NOOBJ                             = 26508,          //�ײ�dllΪ��
	CMS_SDK_GLB_ERR_UNKNOWN_REASON                        = 26509,          //δ֪ԭ��Ĵ���
	CMS_SDK_GLB_ERR_OPR_TIMEOUT                           = 26510,          //������ʱ
	CMS_SDK_GLB_ERR_OPR_FAIL                              = 26511,		    //����ʧ��
	CMS_SDK_GLB_ERR_TRANS_LIMIT                           = 26512,          //ת����������       
	CMS_SDK_GLB_ERR_ALREADY_REALPLAY                      = 26513,          //��Ը��豸�Ľ��յ�ַ�Ѿ�����(ʵʱ���)
	CMS_SDK_BLB_ERR_DEV_RECDOWN_EXIST                     = 26514,          //��Ը��豸ͨ����¼��ط��Ѿ�����
	CMS_SDK_GLB_ERR_RESOURCE_LACK                         = 26515,          //ϵͳ��Դ����
	CMS_SDK_GLB_ERR_NOSUPP_FUNC                           = 26516,          //��֧�ָù���
	CMS_SDK_GLB_ERR_NOSTREAM                              = 26517,          //��������֪ͨ
	CMS_SDK_GLB_ERR_NOINIT_NAT                            = 26518,          //NAT��͸δ��ʼ��
	CMS_SDK_GLB_ERR_DAU_REG_FAIL                          = 26519,          //DAU��GMUע��ʧ��
	// G100��ش�����
	CMS_SDK_GLB_G100_SDKFAILED                            = 26520,	        //����SDK�ӿ�ʧ��
	CMS_SDK_GLB_G100_RECODERNOTFOUND                      = 26521,		    //��ѯ����¼��
	CMS_SDK_GLB_G100_NOENOUGHPRI                          = 26522,			//Ȩ�޲���
	CMS_SDK_GLB_G100_DVRNORESOURCE                        = 26523,			//DVR��Դ���㣬ϵͳæ
	CMS_SDK_GLB_G100_DVROPRATEFAILED                      = 26524,		    //DVR����ʧ��
	CMS_SDK_GLB_G100_TIMEINPUTERROR                       = 26525,		    //ʱ�����벻��ȷ
	CMS_SDK_GLB_G100_NOSPECFILE                           = 26526,			//�ط�ʱ������û��ָ�����ļ�
	CMS_SDK_GLB_G100_OPERNOTFINISH                        = 26527,			//�ϴβ�����δ���
	CMS_SDK_GLB_G100_COMMANDTIMEOUT                       = 26528,		    //ִ�����ʱ
	CMS_SDK_GLB_G100_NOSUPPORT                            = 26529,			//��������֧��
	CMS_SDK_GLB_G100_FILEFORMATERROR                      = 26530,		    //�ļ���ʽ����ȷ
	CMS_SDK_GLB_G100_NODOWNLOADCHN                        = 26531,			//û�и��������ͨ������(¼��ط������ز���ͬʱ����)
	CMS_SDK_GLB_G100_DVRHASQUERYTASK                      = 26532,          //DVR�Ѿ����ڲ�ѯ�������Ժ��ٲ�
	CMS_SDK_GLB_G100_DVRQUERYTIMEOUT                      = 26533,          //DVR¼���ѯ��ʱ ��ȴ�
	CMS_SDK_GLB_G100_DVRQUERYUPLOADING                    = 26534,          //DVR�����û����ڻش����� ��ȴ�
	CMS_SDK_GLB_G100_DHQUERYTIMEOUT                       = 26535, 		    //�󻪲�ѯ¼��ʱ
	CMS_SDK_GLB_G100_DHPLAYBACKLIMIT                      = 26536,		    //��¼��ط�ֻ��һ·

	CMS_SDK_GW_END                                        = 26999,

	//δ֪�����룬������ʹ��δ֪����
	CMS_SDK_UNKNOWN = 0xFFFFFFFF,
};

#endif // __CMS_SDK_ERRORCODE_H__


