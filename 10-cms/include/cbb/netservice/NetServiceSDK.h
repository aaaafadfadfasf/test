#ifndef __NET_SERVICE_SDK_H__
#define __NET_SERVICE_SDK_H__
#include "kdvtype.h"

#define MAX_DDNS_SERIAL_LEN 64
#define MAX_DDNS_DOMAIN_LEN 64
#define DDNS_RESERVE_LEN	64
#define UPNP_PROTO_LEN 		4
#define UPNP_DEC_LEN		40
#define NET_SERVICE_OK			0
#define NET_SERVICE_ERROR		(-1)
#define MAX_MOD_PORT_MAP_NUM	256
#define NET_SERVICE_PORT		9595

#define UPNP_PROTO_TCP "TCP"
#define UPNP_PROTO_UDP "UDP"

#define ERR_UPNP_INVALID_PARAM		201
#define ERR_UPNP_GATEWAY_IP_ERROR	202
#define	ERR_UPNP_LAN_IP_ERROR		203

#define ERR_DDNS_OK					0
#define ERR_DDNS_NULL_PARAM			301
#define ERR_DDNS_USR_PASSWORD		302
#define ERR_DDNS_BAD_DEVICE			303
#define ERR_DDNS_DOMAIN				304
#define ERR_DDNS_SYS_DOMAIN			305
#define	ERR_DDNS_CLIENT_ADDR		306
#define ERR_DDNS_BAD_DEVICE_REG		307
#define ERR_DDNS_SERIAL_USED		308
#define ERR_DDNS_SERIAL_NO_EXIST	309
#define ERR_DDNS_INVALID_PARAM		310
#define ERR_DDNS_CONN_SVR_FAIL		311
#define ERR_DDNS_SEND_REQ_FAIL		312
#define ERR_DDNS_RECV_FAIL			313
#define ERR_DDNS_UNKNOWN			314
#define ERR_DDNS_STOPPED			315

#define UPNP_NAT_PORT_VALID				(0)
#define UPNP_NAT_PORT_INVALID			(1)
#define UPNP_NAT_PORT_INVALID_PARAM		(2) /* the param is invalid */
#define UPNP_NAT_PORT_NO_MEMORY			(3) /* no memory */
#define UPNP_NAT_PORT_SYS_FAILED		(4) /* system error */
#define UPNP_NAT_PORT_NO_INIT			(5) /* uninitialized */
#define	UPNP_NAT_PORT_PORT_IS_USED		(6) /* the in or ex port has been used */
#define UPNP_NAT_PORT_NO_THE_ENTRY		(7) /* the upnp entry is not existed */
#define UPNP_NAT_PORT_SERVER_ABNORMAL	(8) /* the server's upnp is abnormal : closed, 
											   abnormal, port is used by other device*/									
#define UPNP_NAT_PORT_NO_FOUND_IGD  	(9) /* not found the IGD */
#define UPNP_NAT_PORT_DELETED			(10)



// UPNP�ص��¼�
enum{
	UPNP_EVENT_START_RSP,
	UPNP_EVENT_PORT_MAP_ADD,
	UPNP_EVENT_PORT_MAP_DEL,
	UPNP_EVENT_PORT_MAP_UPDATE,
	UPNP_EVENT_PORT_MAP_IP_CHG,
	UPNP_EVENT_ROUTE_DISCONN,
	UPNP_EVENT_ROUTE_RECOVER,
	UPNP_EVENT_OTHER,
	UPNP_EVENT_DISCONN,
};

//DDNS �ص��¼�
enum{
	DDNS_CB_EVENT_START_RSP,
	DDNS_CB_EVENT_STATUS_UPDATE,
	DDNS_CB_EVENT_STOP_DDNS,
	DDNS_CB_EVENT_SET_PARAM,
	DDNS_CB_EVENT_CHG_HTTP_PORT,
	DDNS_CB_EVENT_QRY_STATUS,
	DDNS_CB_EVNET_DISCONN,
};


//�˿����÷�ʽ
enum{
NAT_MODE_MANUAL, //�ֶ�
NAT_MODE_AUTO,   //�Զ�
};

//�˿�ӳ����
struct TUPNP_NAT_PORT_MAP{
u32	dwLANIP;  //LAN IP
u32	dwWANIP;  //WAN IP
u16	wInPort;  //�ڲ��˿�
u16	wExPort;  //�ⲿ�˿�
s8	szProto[UPNP_PROTO_LEN];  //Э��
u32	dwStatus; //״̬
u32	dwNatMode; //�˿����÷�ʽ
s8 szDec[UPNP_DEC_LEN];
s32 nResv;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;


//DDNS����
enum {
DDNS_EASYDDNS = 1, //easyDDNS
};

enum{
	DDNS_STATUS_OFFLINE,
	DDNS_STATUS_ONLINE,
};
//DDNS ���ò���
struct TDDNS_PARAM{
u16	wWebPort; 			//web�˿�
s8	szDomain[MAX_DDNS_DOMAIN_LEN ]; //����
s8	szSerial[MAX_DDNS_SERIAL_LEN ]; //���к�
s8	szResv[DDNS_RESERVE_LEN];			//�����ֽ�
};

struct TDDNS_STATUS
{
	u32 dwDdnsState;
	u32 dwErrNO;
	u32 dwIPAddr;
};


typedef void (*NET_SERVICE_CALLBACK)(s32 nEvent, u32 dwSn, void* pParam, s32 nLen);

/*=============================================================================
    �� �� ���� NetServiceInit
    ��    �ܣ� NetServiceģ���ʼ��
    �㷨ʵ�֣� 
    ��    ���� NET_SERVICE_CALLBACK cbProcNatEvent    UPNP �¼��ص�
			   NET_SERVICE_CALLBACK cbProcDDNSEvent   DDNS �¼��ص�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceInit(NET_SERVICE_CALLBACK cbProcNatEvent, NET_SERVICE_CALLBACK cbProcDDNSEvent, u32 dwIP, u16 wPort);


/*=============================================================================
    �� �� ���� NetServiceDestroy
    ��    �ܣ� NetServiceģ�鷴��ʼ��
    �㷨ʵ�֣� 
    ��    ���� void
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceDestroy();


/*=============================================================================
    �� �� ����  NetServiceStartUpnpNat
    ��    �ܣ�  ����upnp�˿�ӳ��
    �㷨ʵ�֣� 
    ��    ����  
				u32				dwGatewayIP     ���ص�ַ
				u32				dwLanIP			����IP
				u32				dwSn		�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceStartUpnpNat(u32 dwGatewayIP, u32 dwLanIP, u32 dwSn);



/*=============================================================================
    �� �� ���� NetServiceStopUpnpNat
    ��    �ܣ� ֹͣupnp�˿�ӳ��
    �㷨ʵ�֣� 
    ��    ���� u32 dwSn		�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceStopUpnpNat(u32 dwSn);


/*=============================================================================
    �� �� ���� NetServiceSetUpnpNatPort
    ��    �ܣ� ����upnp�˿�ӳ��
    �㷨ʵ�֣� 
    ��    ���� 	u32 dwNatMode			�˿�����ģʽ
				UPNP_NAT_PORT_MAP* pPortMapList	�˿�ӳ���б�
				u32 dwCount			�˿�ӳ����
				u32 dwSn		�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceSetUpnpNatPort(u32 dwNetMode, TUPNP_NAT_PORT_MAP* pPortMapList, u32 dwCount, u32 dwSn);



/*=============================================================================
    �� �� ���� NetServiceDelUpnpNatPort
    ��    �ܣ� �����ڲ�IP���ڲ��˿ڡ�Э��ɾ���˿�ӳ��
    �㷨ʵ�֣� 
	��    ����	TUPNP_NAT_PORT_MAP* pPortMapList ��ɾ���б�
				u16 wCount						 ɾ����Ŀ
				u32 dwSn		�������к�
	�� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
	-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceDelUpnpNatPortList(TUPNP_NAT_PORT_MAP* pPortMapList, u16 wCount, u32 dwSn);



/*=============================================================================
    �� �� ���� NetServiceGetAllUpnpNatPortMap
    ��    �ܣ� ��ѯNetService������ӳ��Ķ˿�
    �㷨ʵ�֣� 
    ��    ����	TUPNP_NAT_PORT_MAP** pPortMapList ���ض˿�ӳ���б�
				u32* pCount						  �����б���ĸ���
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceGetAllUpnpNatPortMap(TUPNP_NAT_PORT_MAP** pPortMapList, u32* pCount);


/*=============================================================================
    �� �� ���� NetServiceGetUpnpNatPortMapBys32ernalPort
    ��    �ܣ� ���ݶ˿�ӳ��������ѯ�˿�ӳ��
    �㷨ʵ�֣� 
    ��    ����	u32 dwLanIP	���� IP
				u16 dwInPort �ڲ��˿�
				s8* pProto			ӳ��Э��
				TUPNP_NAT_PORT_MAP* pPortMap ���صĶ˿�ӳ��
				
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceGetUpnpNatPortMapByInternalPort(u32 dwLanIP, u16 dwInPort, s8* pProto, TUPNP_NAT_PORT_MAP* pPortMap);


/*=============================================================================
    �� �� ���� NetServiceUpnpNatDeviceIPChanged
    ��    �ܣ� �޸�ӳ���LanIP
    �㷨ʵ�֣� 
    ��    ����	u32 dwLanIP	LanIP
				u32 dwSn		�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceUpnpNatDeviceIPChanged(u32 dwLanIP, u32 dwSn);



/*=============================================================================
    �� �� ���� NetServiceChgUpnpNatGatewayIP
    ��    �ܣ� �޸�ӳ���LanIP
    �㷨ʵ�֣� 
    ��    ����	u32 dwGatewayIP	����IP
				u32 dwSn		�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceUpnpNatGatewayIPChanged(u32 dwGatewayIP, u32 dwSn);



/*=============================================================================
    �� �� ���� NetServiceStartDDNS
    ��    �ܣ� ����DDNS����
    �㷨ʵ�֣� 
    ��    ����	u32 dwDDNSType						DDNS����
				s8* pSeverAddr						DDNS��������ַ
				u16 wPort							DDNS�������˿�
				u32 dwSn							�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceStartDDNS(u32 dwDDNSType, const s8* pSeverAddr, u16 wPort, u32 dwSn);


/*=============================================================================
    �� �� ���� NetServiceStopDDNS
    ��    �ܣ� ֹͣDDNS����
    �㷨ʵ�֣� 
    ��    ���� u32 dwSn		�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceStopDDNS(u32 dwSn);


/*=============================================================================
    �� �� ���� NetServiceSetDDNS
    ��    �ܣ� ����DDNS����
    �㷨ʵ�֣� 
    ��    ����	TDDNS_PARAM* pDDNSParam		ddns����
				u32 dwSn					�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceSetDDNS(TDDNS_PARAM* pDDNSParam, u32 dwSn);


/*=============================================================================
    �� �� ���� NetServiceChgDDNSWebPort
    ��    �ܣ� �޸�web�˿�
    �㷨ʵ�֣� 
    ��    ����	u16 wWebPort
				u32 dwSn		�������к�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceChgDDNSWebPort(u16 wWebPort, u32 dwSn);


/*=============================================================================
    �� �� ���� NetServiceGetDDNSStatus
    ��    �ܣ� ��ѯddns״̬
    �㷨ʵ�֣� 
    ��    ����  TDDNS_STATUS* pStatus		 �豸״̬
			  
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/10/14  	1.0		���                    ����
=============================================================================*/
s32 NetServiceGetDDNSStatus(TDDNS_STATUS* pStatus);


/*=============================================================================
    �� �� ���� NetServiceGetSdkVersion
    ��    �ܣ� ��ѯddns״̬
    �㷨ʵ�֣� 
    ��    ����  void
			  
    �� �� ֵ�� s8* sdk�汾��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/12/19  	1.0		���                    ����
=============================================================================*/
s8* NetServiceGetSdkVersion(void);



/*=============================================================================
    �� �� ���� NetServiceGetServerVersion
    ��    �ܣ� ��ѯddns״̬
    �㷨ʵ�֣� 
    ��    ����  void
			  
    �� �� ֵ�� s8* ����˰汾��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/12/19  	1.0		���                    ����
=============================================================================*/
s8* NetServiceGetServerVersion(void);

#endif