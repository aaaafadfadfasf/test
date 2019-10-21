#ifndef _KEYDEF_H_
#define _KEYDEF_H_

/************************************************************************
 * redis���ݿ����õ���key��صĶ�������ͷ�ļ�                 
 * 2016-03-17 zsy  ����
 * 2016-09-21 hzy  ����KEY�ֶνӿ�
 * 2016-09-21 hzy  ���±��������ֶκ;���ע��
 ************************************************************************/

#include "cms/tas/tas_struct.h"
#include "cms/cmu/cmu_data.h"
#include "cms/tas/tas_proto.h"

/************************************************************************/
/*                 key����                                          */
/************************************************************************/
#define MAKE_DOMAIN_KEY(uuidstring) ("domain:"+uuidstring) //��key���ɺ�
#define DOMAIN_KEY_LIST		"fastdb:domain_key_list"       //������key�ļ���key
#define MAKE_DEV_KEY(uuidstring) ("dev:"+uuidstring)       //�豸key���ɺ�
#define DEVICE_KEY_LIST		"fastdb:device_key_list"       //�����豸key�ļ���key
#define MAKE_USER2DEV_KEY(uuidstring) ("user2dev:"+uuidstring)    //�û��豸Ȩ��key���ɺ�
#define USERIDSET_KEY "useridset"                                 //�û�id����key
#define MAKE_USERGRP2DEV_KEY(uuidstring) ("ugrp2dev:"+uuidstring) //�û����豸Ȩ��key���ɺ�
#define UGRPIDSET_KEY "ugrpidset"                                 //�û���id����key
#define SECUREMAIL_KEY  "securemail"                             //��ȫ�����key
#define MOBILE_CONNECT_NUM "onlineMobileNum"                //�ƶ��ͻ��˽��������
#define MOBILE_CONNECT_TOTAL_NUM "maxMobileNum"     //�ƶ��ͻ��˿��Խ��������
#define REDIS_TASDBINFO_KEY   "tasdbinfo"                               //3a���ݿ���Ϣ

/************************************************************************/
/*                 ���ϵ�ֶζ���                                        */
/************************************************************************/
#define ALL_CDomainRelationInfo				"VIRTUAL_CDomainRelationInfo"	//��ʾ���� CDomainRelationInfo �ṹ��������ֶ�
#define CDomainRelationInfo_domainID		"dom"                           // ����
#define CDomainRelationInfo_parentDomainID	"parent"                        // ������
#define CDomainRelationInfo_domainName		"name"                          // ������
#define CDomainRelationInfo_domainAlias		"alias"                         // �����


/************************************************************************/
/*                 �豸�ֶζ���                                          */
/************************************************************************/
// CDeviceInfo 
#define ALL_CDeviceInfo					"VIRTUAL_CDeviceInfo"  //��ʾȫ���� CDeviceInfo �ֶε������ֶ�
#define CDeviceInfo_deviceId			"devId"                // �豸UUIDString
#define CDeviceInfo_domainId			"dom"                  // �豸��������
#define CDeviceInfo_name				"name"                 // �豸��
#define CDeviceInfo_alias				"alias"                // �豸����
#define CDeviceInfo_type				"type"                 // �豸���ͣ��μ���������
#define CDeviceInfo_modelId				"model"                // �豸�ͺ�
#define CDeviceInfo_manufacturer		"manu"                     //�豸������, ���豸�ͺ�����������ڷ��㿼�ǣ�3AS�����豸�ͺŶ�̬ƴ�ӳ���
#define CDeviceInfo_modelCapDetails		"VIRTUAL_modelCapDetails"  //����ֶ�ֻȡredis�����еļ����ֶ�
#define CDeviceInfo_usedFlag			"use"                  // ��ͣ״̬��־��0ͣ�ã���0����
#define CDeviceInfo_netType				"net"                  // ������·��ʽ���μ���������
#define CDeviceInfo_netAddress			"addr"                 // �豸�����ַ��IP������	[---��־��ȷ�ϣ�CDeviceInfo�ṹ�����IP��ַ����redis��addr����ֶα�ʾ����IP��ַ---]
#define CDeviceInfo_expireDate			"expiredate"           // ʹ�ý�ֹ����										[---redis��û������ֶ�---,������־���ּ����� 2016-3-31����]
#define CDeviceInfo_creater				"creat"                // ������
#define CDeviceInfo_owner				"owner"                // ������
#define CDeviceInfo_fixaddr             "fixaddr"              // ��װλ��
#define Redis_fixaddr			        CDeviceInfo_fixaddr    // Ϊ�˼��ݱ����˱���
#define CDeviceInfo_kdmno               "kdmno"
#define Redis_kdmno				        CDeviceInfo_kdmno      // Ϊ�˼��ݱ����˱���

// TPuStatus
#define ALL_TPuStatus			"VIRTUAL_TPuStatus"	 //��ʾ����TPuStatus�ֶε������ֶ�
#define TPuStatus_isUsed		CDeviceInfo_usedFlag //�Ƿ�����
#define TPuStatus_isOnline	    "online"	         //�Ƿ�����

// TPuServiceStatus
#define ALL_TPuServiceStatus					"VIRTUAL_TPuServiceStatus"	           //��ʾ����TPuServiceStatus�ֶε������ֶ�
#define ALL_TPuServiceStatus_audioCallSrc		"DYNAMIC_TPuServiceStatus_audioCallSrc"//��ʾ����TPuServiceStatus_audioCallSrc�ֶε������ֶ�,���ݡ���Ƶ����ͨ����������
#define TPuServiceStatus_audioCallSrc(idx)		MakeField("chnaudcallstatus", idx)     //��Ƶ���е�Դ
#define ALL_TPuServiceStatus_isPuRec			"DYNAMIC_TPuServiceStatus_isPuRec"     //��ʾ����TPuServiceStatus_isPuRec�ֶε������ֶ���,���ݡ�����ͨ����������
#define TPuServiceStatus_isPuRec(idx)			MakeField("chnpurecstatus", idx)       //�Ƿ���ǰ��¼��

// TPuBaseCapSet
#define ALL_TPuBaseCapSet				"VIRTUAL_TPuBaseCapSet"	 //��ʾ���� TPuBaseCapSet �ֶε������ֶ�
#define TPuBaseCapSet_videoEncNum		"encode"                 //��Ƶ����ͨ������
#define TPuBaseCapSet_videoDecNum		"decode"                 //��Ƶ����ͨ������
#define TPuBaseCapSet_audioEncNum		"audencnum"              //��Ƶ����ͨ������
#define TPuBaseCapSet_audioDecNum		"auddecnum"              //��Ƶ����ͨ������
#define TPuBaseCapSet_serialPortNum		"serialport"             //��������
#define TPuBaseCapSet_inputPinNum		"inputpin"               //������������		[----�ǲ��Ǿ��Ǹ澯����ͨ��---]
#define TPuBaseCapSet_outputPinNum		"outputpin"              //�����������
#define TPuBaseCapSet_videoInPortNum	"video"                  //��Ƶ����˿�����	[---������ƵԴ����--]
#define CDeviceInfo_capLabel            "caplabel"               //�豸��������ǩ

// TPuConfig
#define ALL_TPuConfig					"VIRTUAL_TPuConfig"	//��ʾ����TPuConfig�ֶε������ֶ�
#define TPuConfig_devIp					"connectip"         //ǰ���豸IP				[---��ʦȷ�� TPuConfig���IP�������connectip---]
#define TPuConfig_devType				"devtype"          //�豸���ͣ��������������
#define TPuConfig_devMode				"devmode"          //�豸�ͺţ�CU��ʹ�����ж�һЩ��ǰ�˵�������ƽ̨����ʹ��
#define TPuConfig_devName				CDeviceInfo_name   //�豸����
#define TPuConfig_manufacturer			"manu"         //�豸����				[---��CDeviceInfo������ظ��ģ���ʱ��ֱ���ٸ���һ�ݸ�����ͺ���---]
#define TPuConfig_extCapSet				"extcapset"    //��չ������				[---ȷ����Ҫ���л�---]
#define TPuConfig_inputPinEnable		"inputpinen"   //��������ͨ��ʹ������
#define TPuConfig_freeLicenceNum		"freelicense"  //��ѵ�licence����		[----�Ƿ�������---]
#define TPuConfig_directTransNum		"directtrans"  //�豸֧�ֵ�ֱ��·��
#define TPuConfig_isDecCombined		    "deccombined"  //�������Ƿ�ϳ�����
#define TPuConfig_isGBDevice			"isgb"         //�Ƿ��ǹ����豸
#define TPuConfig_baseCapSet			ALL_TPuBaseCapSet//����������			[---��һ��TPuBaseCapSet�ṹ��---]
#define ALL_TPuConfig_hdmiVidDecCfg		"DYNAMIC_hdmistyle"		//��ʾ���� TPuConfig_hdmiVidDecCfg�ֶε������ֶ�,���ݡ���Ƶ����ͨ����������
#define TPuConfig_hdmiVidDecCfg(idx)	MakeField("hdmistyle", idx)//������hdmi������ ���������� Redis_hdmioutnum��idx����hdmi���������
#define ALL_TPuConfig_chnVidFormat		"DYNAMIC_TPuConfig_chnVidFormat"	//��ʾ����TPuConfig_chnVidFormat�ֶε������ֶ���,���ݡ�����ͨ����������
#define TPuConfig_chnVidFormat(idx)		MakeField("chnvidformat", idx)      //ͨ����ǰ����Ƶ��ʽ
#define ALL_TPuConfig_supportMediaType	"DYNAMIC_TPuConfig_supportMediaType"   //��ʾ����TPuConfig_supportMediaType�ֶε������ֶ�,���ݡ�����ͨ����������
#define TPuConfig_supportMediaType(idx)	 MakeField("chnsupportmediatype", idx) //ͨ����ǰ֧�ֵ�����ý���ʽ(ֻ��)
#define ALL_TPuConfig_chnVidEncParam	"DYNAMIC_TPuConfig_chnVidEncParam"	// ��ʾ����TPuConfig_chnVidEncParam�ֶε������ֶ�,���ݡ�����ͨ����������
#define TPuConfig_chnVidEncParam(idx)	MakeField("chnvidencparam", idx)    //ͨ����ǰ����Ƶ�������	[---��ȷ��ÿһ����һ�����л�---]
#define ALL_TPuConfig_chnAudEncParam	"DYNAMIC_TPuConfig_chnAudEncParam"	// ��ʾ����TPuConfig_chnAudEncParam�ֶε������ֶ�,���ݡ�����ͨ����������
#define TPuConfig_chnAudEncParam(idx)	MakeField("chnaudencparam", idx)    //ͨ����ǰ����Ƶ������� [---��ȷ��ÿһ����һ�����л�---]
#define ALL_TPuConfig_chnVidSrcSet		"DYNAMIC_TPuConfig_chnVidSrcSet"// ��ʾ����TPuConfig_chnVidSrcSet�ֶε������ֶ�,���ݡ�����ͨ����������
#define TPuConfig_chnVidSrcSet(idx)		MakeField("chnvid", idx)        //ͨ�����õ���ƵԴ�б�							[---��ȷ��ÿһ����һ�����л�---]
#define ALL_TPuConfig_chnStatusSet		"DYNAMIC_TPuConfig_chnStatusSet"// ��ʾ����TPuConfig_chnStatusSet�ֶε������ֶ�,���ݡ�����ͨ����������
#define TPuConfig_chnStatusSet(idx)		MakeField("chnstatus", idx)     //ͨ��״̬�� add by ywy for  channel state report
#define TPuConfig_chnStatusSet_Q(idx, buf)		MakeFieldQ("chnstatus", idx, buf)     //ͨ��״̬�� add by ywy for  channel state report
#define ALL_TPuConfig_vidSrcName		"DYNAMIC_TPuConfig_vidSrcName" // ��ʾ����TPuConfig_vidSrcName�ֶε������ֶ�,���ݡ���ƵԴ��������
#define TPuConfig_vidSrcName(idx)		MakeField("vidname", idx)      // ��ƵԴ���ƣ�pui�ռ���pu�ϱ������� PuConfig�ṹ����ʹ�õ��Ǵ��ֶ�
#define ALL_TPuConfig_vidSrcAlias		"DYNAMIC_TPuConfig_vidSrcAlias"// ��ʾ����TPuConfig_vidSrcAlias�ֶε������ֶ�,���ݡ���ƵԴ��������
#define TPuConfig_vidSrcAlias(idx)      MakeField("vidalias", idx)     // ��ƵԴ������ƽ̨��Ч����ƵԴ����           
#define ALL_TPuConfig_vidSrcChnSet		"DYNAMIC_TPuConfig_vidSrcChnSet"  // ��ʾ����TPuConfig_vidSrcChnSet�ֶε������ֶ�,���ݡ���ƵԴ��������
#define TPuConfig_vidSrcChnSet(idx)		MakeField("vidchnset", idx)   // ��ƵԴ��ǰ����ʹ�õ�ͨ���б�		[---��ȷ��ÿһ����һ�����л�---]
#define ALL_TPuConfig_camType			"DYNAMIC_TPuConfig_camType"   // ��ʾ����TPuConfig_camType�ֶε������ֶ�,���ݡ���ƵԴ��������
#define TPuConfig_camType(idx)			MakeField("camtype", idx)     //��ƵԴ������ͷ����
#define ALL_TPuConfig_camId				"DYNAMIC_TPuConfig_camId"     // ��ʾ����TPuConfig_camId�ֶε������ֶ�,���ݡ���ƵԴ��������
#define TPuConfig_camId(idx)			MakeField("camid", idx)       //��ƵԴ������ͷ��ַ��
#define ALL_TPuConfig_azimuth			"DYNAMIC_TPuConfig_azimuth"   // ��ʾ����TPuConfig_azimuth�ֶε������ֶ�,���ݡ���ƵԴ��������
#define TPuConfig_azimuth(idx)          MakeField("azimuth", idx)     //��ƵԴ�ķ�λ������
#define TPuConfig_puiId                 "puiid"                       //����pui��uuid


// CRedisDevData
#define ALL_CRedisDevData							"VIRTUAL_CRedisDevData"	       //��ʾ����CRedisDevData�ṹ��������ֶ�
#define CRedisDevData_m_tGpsData					"gpsdata"                 	   // pu gps����
#define CRedisDevData_m_tTransparentData(type)		MakeField("chntransdata", type)// pu ͸�����ݣ�Ŀǰʵ����ֻ��һ���ֶΣ�δ��Ҳ��̫���ܸı䣬�������Ŀǰ�ǵ��ɹ̶��ֶ����õ�	[---��ȷ��ÿһ����һ�����л�---]
#define CRedisDevData_m_tPuOnOff					ALL_TPuStatus		// pu ����״̬		[---��һ�� TPuStatus �ṹ��--]
#define CRedisDevData_m_tPuConfig					ALL_TPuConfig	    // pu ������Ϣ		[---��һ��TPuConfig�ṹ��---]
#define CRedisDevData_m_tPuServiceStatus			ALL_TPuServiceStatus// pu ҵ��״̬		[---��һ��TPuServiceStatus�ṹ��---]
#define ALL_CRedisDevData_m_tPuAlarmCollector			"DYNAMIC_CRedisDevData_m_tPuAlarmCollector"	//��ʾ���е� CRedisDevData_m_tPuAlarmCollector �ֶ�
#define CRedisDevData_m_tPuAlarmCollector(idx, type)	MakeField("chnalarm", idx, type)	        // pu�澯��Ϣ	[---��ȷ��ÿһ����һ�����л�---]
#define CHNS_WITH_ALARM_TYPE(type)	MakeField("fastdb:alarm_type", type) // �澯type��ͨ�����б�value��һ��set<int>������fastdb�Ĺ����ֶ�
// CRedisPuData �̳���CRedisDevData
#define ALL_CRedisPuData							"VIRTUAL_CRedisPuData"	//��ʾ����CRedisPuData�ṹ��������ֶ�
#define CRedisPuData_m_tDevInfo						ALL_CDeviceInfo		    //��3AS��ȡ���豸��Ϣ			[---ʵ����һ��CDeviceInfo�ṹ---]

// �����ֶ� ���ṹ����û�У���redis���е��ֶ�
#define Redis_LicenceNum		"license"	        //�����CDevice��ģ������ǽṹ���Ա���ĳ��ֵ������û�г�Ա��
#define Redis_localip			"localip"	        //����ip
#define Redis_devmode			"devmode"	        //-- CU��ʹ�����ж�һЩ��ǰ��
#define Redis_alarm				"alarm"	
#define Redis_localstore		"store"		        //�Ƿ���б��ش洢
#define Redis_hdmioutnum		"hdmioutput"	    //�����hdmi�����
#define ALL_vidgps				"DYNAMIC_vidgps"	// ��ʾ���� Redis_vidgps�ֶε������ֶ�,���ݡ���ƵԴ��������
#define Redis_vidgps(idx)		MakeField("vidgps", idx) // --�洢��ƵԴ��γ�ȣ�lat, lon�ṹ������л�  vidgps : 1��
#define Redis_gbid              "gbid"                   // �豸��gbid
#define ALL_encgbid             "DYNAMIC_encgbid"         // ��ʾ���� Redis_encgbid�ֶε������ֶΣ����ݡ�����ͨ����������
#define Redis_encgbid(idx)      MakeField("encgbid", idx) // �洢����ͨ���Ĺ�����
#define Redis_puiip             "puiip"                  // �����pui ip
#define Redis_chnplatrecstatus(idx)		MakeField("chnplatrecstatus", idx)   //�Ƿ���ƽ̨¼��
#define Redis_GB_Security       "pms_gb_security_enable" //ƽ̨�Ƿ������˰�ȫģʽ

#define REDIS_TASDBINFO_IP "ip"
#define REDIS_TASDBINFO_PORT "port"
#define REDIS_TASDBINFO_PASSWORD "password"
static const size_t DevKeyLen = 36;		// �豸Key�Ĵ�С.
static const size_t MaxFieldLen = 64;	// �����ֶγ���.
static const size_t MaxCmdSize = 2048;	// ����Redis�����С.
static const size_t MaxCmdSize2 = (1024 * 64);	// ����Redis�����С��ȷ�����Դ洢һ��puconfig�������ַ�����xml���л���������Ҫ��һЩ��Ӧ�ù��ˣ�


/**	�����ܵ��豸Key���ɺ���.
*	˵��:
*	devId ����Ϊ32�ֽڳ���.
*	����buf ��Ϊ0ʱ �ڴ��СҪ >= 37�ֽ�. ����֧������/���߳�.
*	����bufΪ0ʱ�����ڲ���̬�ڴ�, ������֧������/���߳�. ����ģʽ���ܸ���, �Ƽ����̳߳���ʹ��.
*	�ɹ�����buf, ʧ�ܷ���0
*	+by lzx@2017-03-02
*/
char* MakeDevKey(const char* devId, char* buf = 0);

//�ֶ���ƴ�Ӻ���
std::string MakeField(const std::string &strBaseName, int nIdx);

/** �����ܵ��ֶ�ƴ�Ӻ���. 
*	����bufΪ0ʱ�����ڲ���̬�ڴ�, ������֧������/���߳� 
*	�ɹ�����buf, ʧ�ܷ���0
*	+by lzx@2017-03-02
*/
char* MakeFieldQ(const char* strBaseName, int nIdx, char* buf=0);	
std::string MakeField(const std::string &strBaseName, int nIdx, const std::string &strType);
std::string MakeField(const std::string &strBaseName, const std::string &strType);
//�� CHNS_WITH_ALARM_TYPE(type)���ɵ��ֶ�������ȡ���澯������
std::string GetAlarmTypeByField(const std::string &strFieldWithType);
/***********************************************************************
* ���º���ֻ���ؽṹ�屾����ֱ����redis�б�ʾ���ֶ�
* ����������������ֶ�
* ��������Ա����Ҳ�Ǳ�ϵ�к������Ի�õĽṹ��
* ��������Ա�ǿɱ�Ķ�̬�ֶ�
************************************************************************/
//�ṹ�� TPuStatus �������ֶ���
int Get_TPuStatus_FieldNames(std::vector<std::string> &fields);
//�ṹ�� TPuConfig �������ֶ���
int Get_TPuConfig_FieldNames(std::vector<std::string> &fields, bool bRead);
//�ṹ�� TPuBaseCapSet �������ֶ���
int Get_TPuBaseCapSet_FieldNames(std::vector<std::string> &fields);
//�ṹ�� TPuServiceStatus �������ֶ���
int Get_TPuServiceStatus_FieldNames(std::vector<std::string> &fields);
//�ṹ�� CDeviceInfo �������ֶ���
int Get_CDeviceInfo_FieldNames(std::vector<std::string> &fields);
//�ṹ�� CRedisDevData �������ֶ���
int Get_CRedisDevData_FieldNames(std::vector<std::string> &fields);
//�ṹ�� CRedisPuData �������ֶ���
int Get_CRedisPuData_FieldNames(std::vector<std::string> &fields);
//�ṹ�� CDomainRelationInfo �������ֶ���
int Get_CDomainRelationInfo_FieldNames(std::vector<std::string> &fields);
//��ȡ���и澯����ͨ�����ϵ��ֶ���
int Get_All_AlaramChannelSet_FieldNames(std::vector<std::string> &fields);
/***********************************************************************
* ���µĺ����ṩ�����redis�ֶε��ṹ���Ա�Ļ���
* ��values���е��ֶ�ȫ��ƥ�䵽data�ж�Ӧ��Ա��ȥ
* ����ֵ�������False��ֻ��ʾ˫�����ֶ��޷���Ӧ
************************************************************************/
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT TPuStatus &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT TPuConfig &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT TPuBaseCapSet &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT TPuServiceStatus &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT CDeviceInfo &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT CRedisDevData &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT CRedisPuData &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT CDomainRelationInfo &data);
//values�ǼȰ������ֶ�CHNS_WITH_ALARM_TYPE()Ҳ�������ֶ�CRedisDevData_m_tPuAlarmCollector()��һ��map
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT std::map<std::string, CAlarmInputList> &data);

bool Struct_To_Redis(IN const TPuStatus &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const TPuConfig &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const TPuBaseCapSet &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const TPuServiceStatus &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const CDeviceInfo &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const CRedisDevData &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const CRedisPuData &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const CDomainRelationInfo &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const std::map<std::string, CAlarmInputList> &data, IN const std::string &field, OUT std::string &value);

#endif // _KEYDEF_H_
