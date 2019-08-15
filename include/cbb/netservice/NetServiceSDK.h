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



// UPNP回调事件
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

//DDNS 回调事件
enum{
	DDNS_CB_EVENT_START_RSP,
	DDNS_CB_EVENT_STATUS_UPDATE,
	DDNS_CB_EVENT_STOP_DDNS,
	DDNS_CB_EVENT_SET_PARAM,
	DDNS_CB_EVENT_CHG_HTTP_PORT,
	DDNS_CB_EVENT_QRY_STATUS,
	DDNS_CB_EVNET_DISCONN,
};


//端口设置方式
enum{
NAT_MODE_MANUAL, //手动
NAT_MODE_AUTO,   //自动
};

//端口映射项
struct TUPNP_NAT_PORT_MAP{
u32	dwLANIP;  //LAN IP
u32	dwWANIP;  //WAN IP
u16	wInPort;  //内部端口
u16	wExPort;  //外部端口
s8	szProto[UPNP_PROTO_LEN];  //协议
u32	dwStatus; //状态
u32	dwNatMode; //端口设置方式
s8 szDec[UPNP_DEC_LEN];
s32 nResv;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;


//DDNS类型
enum {
DDNS_EASYDDNS = 1, //easyDDNS
};

enum{
	DDNS_STATUS_OFFLINE,
	DDNS_STATUS_ONLINE,
};
//DDNS 配置参数
struct TDDNS_PARAM{
u16	wWebPort; 			//web端口
s8	szDomain[MAX_DDNS_DOMAIN_LEN ]; //域名
s8	szSerial[MAX_DDNS_SERIAL_LEN ]; //序列号
s8	szResv[DDNS_RESERVE_LEN];			//保留字节
};

struct TDDNS_STATUS
{
	u32 dwDdnsState;
	u32 dwErrNO;
	u32 dwIPAddr;
};


typedef void (*NET_SERVICE_CALLBACK)(s32 nEvent, u32 dwSn, void* pParam, s32 nLen);

/*=============================================================================
    函 数 名： NetServiceInit
    功    能： NetService模块初始化
    算法实现： 
    参    数： NET_SERVICE_CALLBACK cbProcNatEvent    UPNP 事件回调
			   NET_SERVICE_CALLBACK cbProcDDNSEvent   DDNS 事件回调
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceInit(NET_SERVICE_CALLBACK cbProcNatEvent, NET_SERVICE_CALLBACK cbProcDDNSEvent, u32 dwIP, u16 wPort);


/*=============================================================================
    函 数 名： NetServiceDestroy
    功    能： NetService模块反初始化
    算法实现： 
    参    数： void
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceDestroy();


/*=============================================================================
    函 数 名：  NetServiceStartUpnpNat
    功    能：  开启upnp端口映射
    算法实现： 
    参    数：  
				u32				dwGatewayIP     网关地址
				u32				dwLanIP			本机IP
				u32				dwSn		操作序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceStartUpnpNat(u32 dwGatewayIP, u32 dwLanIP, u32 dwSn);



/*=============================================================================
    函 数 名： NetServiceStopUpnpNat
    功    能： 停止upnp端口映射
    算法实现： 
    参    数： u32 dwSn		操作序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceStopUpnpNat(u32 dwSn);


/*=============================================================================
    函 数 名： NetServiceSetUpnpNatPort
    功    能： 设置upnp端口映射
    算法实现： 
    参    数： 	u32 dwNatMode			端口设置模式
				UPNP_NAT_PORT_MAP* pPortMapList	端口映射列表
				u32 dwCount			端口映射数
				u32 dwSn		操作序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceSetUpnpNatPort(u32 dwNetMode, TUPNP_NAT_PORT_MAP* pPortMapList, u32 dwCount, u32 dwSn);



/*=============================================================================
    函 数 名： NetServiceDelUpnpNatPort
    功    能： 根据内部IP、内部端口、协议删除端口映射
    算法实现： 
	参    数：	TUPNP_NAT_PORT_MAP* pPortMapList 待删除列表
				u16 wCount						 删除数目
				u32 dwSn		操作序列号
	返 回 值： s32 0-成功 ，非0-失败
	-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceDelUpnpNatPortList(TUPNP_NAT_PORT_MAP* pPortMapList, u16 wCount, u32 dwSn);



/*=============================================================================
    函 数 名： NetServiceGetAllUpnpNatPortMap
    功    能： 查询NetService上所有映射的端口
    算法实现： 
    参    数：	TUPNP_NAT_PORT_MAP** pPortMapList 返回端口映射列表
				u32* pCount						  返回列表项的个数
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceGetAllUpnpNatPortMap(TUPNP_NAT_PORT_MAP** pPortMapList, u32* pCount);


/*=============================================================================
    函 数 名： NetServiceGetUpnpNatPortMapBys32ernalPort
    功    能： 根据端口映射索引查询端口映射
    算法实现： 
    参    数：	u32 dwLanIP	内网 IP
				u16 dwInPort 内部端口
				s8* pProto			映射协议
				TUPNP_NAT_PORT_MAP* pPortMap 返回的端口映射
				
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceGetUpnpNatPortMapByInternalPort(u32 dwLanIP, u16 dwInPort, s8* pProto, TUPNP_NAT_PORT_MAP* pPortMap);


/*=============================================================================
    函 数 名： NetServiceUpnpNatDeviceIPChanged
    功    能： 修改映射的LanIP
    算法实现： 
    参    数：	u32 dwLanIP	LanIP
				u32 dwSn		操作序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceUpnpNatDeviceIPChanged(u32 dwLanIP, u32 dwSn);



/*=============================================================================
    函 数 名： NetServiceChgUpnpNatGatewayIP
    功    能： 修改映射的LanIP
    算法实现： 
    参    数：	u32 dwGatewayIP	网关IP
				u32 dwSn		操作序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceUpnpNatGatewayIPChanged(u32 dwGatewayIP, u32 dwSn);



/*=============================================================================
    函 数 名： NetServiceStartDDNS
    功    能： 开启DDNS服务
    算法实现： 
    参    数：	u32 dwDDNSType						DDNS类型
				s8* pSeverAddr						DDNS服务器地址
				u16 wPort							DDNS服务器端口
				u32 dwSn							操作序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceStartDDNS(u32 dwDDNSType, const s8* pSeverAddr, u16 wPort, u32 dwSn);


/*=============================================================================
    函 数 名： NetServiceStopDDNS
    功    能： 停止DDNS服务
    算法实现： 
    参    数： u32 dwSn		命令序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceStopDDNS(u32 dwSn);


/*=============================================================================
    函 数 名： NetServiceSetDDNS
    功    能： 设置DDNS参数
    算法实现： 
    参    数：	TDDNS_PARAM* pDDNSParam		ddns参数
				u32 dwSn					操作序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceSetDDNS(TDDNS_PARAM* pDDNSParam, u32 dwSn);


/*=============================================================================
    函 数 名： NetServiceChgDDNSWebPort
    功    能： 修改web端口
    算法实现： 
    参    数：	u16 wWebPort
				u32 dwSn		操作序列号
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceChgDDNSWebPort(u16 wWebPort, u32 dwSn);


/*=============================================================================
    函 数 名： NetServiceGetDDNSStatus
    功    能： 查询ddns状态
    算法实现： 
    参    数：  TDDNS_STATUS* pStatus		 设备状态
			  
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/10/14  	1.0		李镜波                    创建
=============================================================================*/
s32 NetServiceGetDDNSStatus(TDDNS_STATUS* pStatus);


/*=============================================================================
    函 数 名： NetServiceGetSdkVersion
    功    能： 查询ddns状态
    算法实现： 
    参    数：  void
			  
    返 回 值： s8* sdk版本号
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/12/19  	1.0		李镜波                    创建
=============================================================================*/
s8* NetServiceGetSdkVersion(void);



/*=============================================================================
    函 数 名： NetServiceGetServerVersion
    功    能： 查询ddns状态
    算法实现： 
    参    数：  void
			  
    返 回 值： s8* 服务端版本号
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/12/19  	1.0		李镜波                    创建
=============================================================================*/
s8* NetServiceGetServerVersion(void);

#endif