#ifndef _DAU_CONST_H_
#define _DAU_CONST_H_

// �����ַ�������
#define DAU_NE_AGENT_CS		    "AgentCap"		// �Ƿ�Ϊ��������Ԫ����
#define DAU_NE_UPGRADE_CS	    "UpgradeCap"	// �Զ���������
#define DAU_NE_TOPO_CS		    "TopoCap"		// TOPO����
#define DAU_NE_ISCSI_CS         "IscsiCap"      // ISCSI����
#define DAU_NE_LOGHARVEST_CS    "LogharvestCap" // ��־�ո�����
#define DAU_NE_AUTO_ADD_BY_NMS_CS      "AutoAddByNmsCap"    // �Զ�������������
#define DAU_NE_NEW_COMMON_MSG_CS    "NewCommonMsgCap"   //ʹ���µ�TCommonMsg��ԭ���ϵ���TOpeNEAgentParam
                                                        //neagent�ڲ���������ʹ�ã��ⲿ�ӿڱ����

#define DAU_TOPODISCOVER_CS "TopodiscoverCap" // topo����������dau��������
// end ����


//MainType define
#define MAIN_TYPE_PLATFORM      "PLATFORM"
#define MAIN_TYPE_PU            "PU"
#define MAIN_TYPE_STORAGE       "STORAGE"
#define MAIN_TYPE_NEAGENT       "NEAGENT"
#define MAIN_TYPE_NETWORKDEV    "NETWORKDEV"
#define MAIN_TYPE_ALARMPU       "ALARMPU"
#define MAIN_TYPE_DECODER       "DECODER"
#define MAIN_TYPE_GENERALSERVER "GENERALSERVER" //ͨ�÷�����
//end MainType define

//SubType define
//////////////////////////////////////////////////////////////////////////
#define SUB_TYPE_CMU        "CMU"
#define SUB_TYPE_VTDU       "VTDU"
#define SUB_TYPE_PUI        "PUI"
#define SUB_TYPE_CUI        "CUI"
#define SUB_TYPE_NRU        "NRU"
#define SUB_TYPE_GAS        "GAS"
#define SUB_TYPE_UAS        "UAS"
#define SUB_TYPE_3AS        "3AS"
#define SUB_TYPE_VPU        "VPU"
#define SUB_TYPE_DNS        "DNS"
#define SUB_TYPE_MTU        "MTU"
#define SUB_TYPE_TVS        "TVS"
#define SUB_TYPE_MPS        "MPS"
#define SUB_TYPE_RCS        "RCS"
#define SUB_TYPE_ALS        "ALS"

//////////////////////////////////////////////////////////////////////////
#define SUB_TYPE_IPC        "IPC"
#define SUB_TYPE_NVR        "NVR"
#define SUB_TYPE_SVR        "SVR"
#define SUB_TYPE_DEC        "DEC"
#define SUB_TYPE_ENC        "ENC"
#define SUB_TYPE_DVR        "DVR"
#define SUB_TYPE_SVR_EDU    "SVR-EDU"


//////////////////////////////////////////////////////////////////////////
#define SUB_TYPE_RAID       "RAID"
#define SUB_TYPE_CLOUD      "CLOUD"
#define SUB_TYPE_MA         "MA"
#define SUB_TYPE_KDNETDISKAG    "KDNETDISKAG"
#define SUB_TYPE_STDNETDISKAG   "STDNETDISKAG"

//////////////////////////////////////////////////////////////////////////
#define SUB_TYPE_PLATV1     "PLATV1.0AG"
#define SUB_TYPE_PLATV2     "PLATV2.0AG"
#define SUB_TYPE_NETDISKAG  "NETDISKAG"
#define SUB_TYPE_PLATHUB    "PLATHUB"
#define SUB_TYPE_LINKGATE   "LINKGATE"

//////////////////////////////////////////////////////////////////////////
#define SUB_TYPE_SWITCH     "SWITCH"
#define SUB_TYPE_ROUTER     "ROUTER"

//////////////////////////////////////////////////////////////////////////
#define SUB_TYPE_INTELLIGENTBOX     "INTELLIGENTBOX"


//////////////////////////////////////////////////////////////////////////
#define SUB_TYPE_VAS            "VAS"               //vas
#define SUB_TYPE_LOGHARVEST     "LOGHARVEST"        //��־�ո������

//end SubType define

//���������䶨��
#define WEB_DAU_ERROR_BEGIN		10000
#define DAU_NE_ERROR_BEGIN		15000
#define NE_AGENT_ERROR_BEGIN	20000
#define SNMP_DAU_ERROR_BEGIN    25000
#define NE_ERROR_BEGIN          30000


#endif //_DAU_CONST_H_
