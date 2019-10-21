//////////////////////////////////////////////////////////////////////////
// ipcstruct.h 
#ifndef IPCSTRUCT_H_
#define IPCSTRUCT_H_

#include "ipcdefine.h"

/*************************************************
/	                  ����ǰ��
/*************************************************/
// �������
typedef struct tagIPCNetParam
{
	unsigned long   dwIpAddr;	                                      // IP��ַ(������)
	unsigned short  wPort;		                                      // �˿ں�(1024-65535)
} TIPC_NET_PARAM, *PTIPC_NET_PARAM;

//GUID�ṹ
typedef struct tagIPCVsipGuid
{
	unsigned long	Data1;                                            // �����豸GUID,Data1����Ϊ0
   	unsigned short	Data2;
	unsigned short	Data3;
   	unsigned char	Data4[8];
} TIPC_VSIP_GUID, *PTIPC_VSIP_GUID;

// �����������
typedef struct tagIPCDiscoveryCBInfo
{
	TIPC_VSIP_GUID	tGuid;							                  // �豸GUIDֵ(ֻ��)
	char	        achName[MAX_STR_LEN_64+1];			              // �豸��
	char	        achTypeName[MAX_STR_LEN_64+1];		              // �豸������(ֻ��)
	unsigned char   byDevType;							              // �豸����(IPC_ENCODER��)
	unsigned long	dwUnitIpAddr;						              // �豸IP��ַ
	unsigned short	wVsipPort;							              // VSIP�˿ں�
} TIPC_DISCOVERY_CB_INFO, *PTIPC_DISCOVERY_CB_INFO;

/* ��������ص� */
typedef void (*IPCSDK_SEARCH_CB)(TIPC_DISCOVERY_CB_INFO atUnitParam[], unsigned short wUnitNum, void *pContext);

// ��������
typedef struct tagIPCConnType
{
	unsigned char byUdpUnicast;                                       // ֧��Ϊ1����֧��Ϊ0����ͬ
	unsigned char byUdpMulticast;
	unsigned char byUdpBroadcast;
	unsigned char byTcpServer;
	unsigned char byTcpClient;
	unsigned char bySSL;
} TIPC_CON_TYPE, *PTIPC_CON_TYPE;

/*************************************************
/	                  ��������
/*************************************************/
// ǰ�˻��������ṹ
typedef struct tagIPCSysParam
{
	char		   achName[MAX_STR_LEN_64+1];			              // �豸��
	char		   achTypeName[MAX_STR_LEN_64+1];		              // �豸������(ֻ��)
	unsigned char  byDevType;							              // �豸����(IPC_ENCODER��)
	char 		   abySeriesName[16+1];					              // �豸ϵ����
} TIPC_PU_BASE_PARAM, *PTIPC_PU_BASE_PARAM;

// ϵͳ���������ṹ
typedef struct tagIPCSysPubParam
{
	unsigned char   byDetPacketSend;                                  // �Ƿ���NAT̽���
	unsigned long	dwUnitIpAddr;                                     // �豸IP��ַ
	unsigned short	wVsipPort;                                        // VSIP�˿ں�
	unsigned long	dwSubnetMask;                                     // �豸��������(������)
	unsigned long	dwGateWay;                                        // ���ص�ַ(������)
	char	        achVersion[MAX_VER_STRING_LEN+1];                 // �豸�汾��(ֻ��)
	unsigned long	dwDiscoveryIpAddr;                                // ������ע���ַ
	unsigned long	dwVsipMultiIpAddr;                                // �������鲥��ַ
	char	        achWebUsername[MAX_STR_LEN_64+1];                 // Ƕ��ʽWeb�û���
	char	        achWebPassword[MAX_STR_LEN_64+1];                 // Ƕ��ʽWeb����
	char	        achWebGuestname[MAX_STR_LEN_64+1];                // Ƕ��ʽWeb����Ա�û���
	char	        achWebGuestPassword[MAX_STR_LEN_64+1];            // Ƕ��ʽWeb����Ա����
	unsigned short	wAlarmContinuance;                                // �澯�ָ�ʱ��(��)
	unsigned char   bySubAreaType;                                    // ������ʽ(ֻ�� IPCSDK_SINGLE_SUBAREA ������
	                                                                  // IPCSDK_MULTI_SUBAREA �����)
	unsigned char   byMaxTranferNum;                                  // ���ֱ��·��
	char            abyPuid[MAX_VER_STRING_LEN];                      // ǰ��ID
	char            abyPassword[MAX_VER_STRING_LEN];                  // ��֤����
	unsigned char   byIsDecCombined;                                  // �Ƿ��ǽ���ϳ�ͼ��
	unsigned short  wAppType;							              // Ӧ������ 0-��̬����  1-DHCP
	unsigned char   byMatrixValid;						              // �Ƿ�������
	unsigned char   byDecMPStyle;		                              // �������ģʽ
    char            achSoftVer[MAX_STR_LEN_64 + 1];
    char            achHardVer[MAX_STR_LEN + 1];
	char            achPtzSoftVer[MAX_STR_LEN_64  + 1];
} TIPC_SYS_PUB_PARAM, *PTIPC_SYS_PUB_PARAM;

#ifdef WIN32
#pragma pack(push, 1)
#endif
// ������Ϣ�����ṹ
typedef struct tagIPCDomainNameInfo
{
	unsigned char   byState;                                          // ע��״̬: 0-����; 1-����; 2-���ڸ���
	unsigned char   byDomainNameNum;                                  // ���ص���������
	char            abyDomainName[MAX_STR_LEN][MAX_STR_LEN_64+1];     // ��������
	unsigned long   dwDonator;                                        // �ʻ����ͣ���ѣ��շ�
} TIPC_DOMAIN_NM_INFO, *PTIPC_DOMAIN_NM_INFO;

// ��̬������������ṹ
typedef struct tagIPCDDNSCfg
{
	unsigned char byServiceNum;                                       // ֧�ַ������
	unsigned char abyService[2];                                      // �������ͣ�ex:3322�������ǣ�dyndns
	unsigned char bySrvType;                                          // ��ǰ��������
	char achDnsServer[16];                                            // dns server ip��ַ
	char achUsername[128];                                            // ��֤�û���
	char achPassword[128];                                            // ��֤����
	char achHostname[128];                                            // ����
	char achInterface[16];                                            // �������ڵ�ַ
	TIPC_DOMAIN_NM_INFO  tPuDomain;                                   // ������Ϣ
} TIPC_DDNS_CFG, *PTIPC_DDNS_CFG;

// DNS����
typedef struct tagIPCDnsCfg
{
	unsigned long dwDnsAddr;                                          // DNS��ַ
	unsigned long dwDnsBackupAddr;                                    // DNS���ݵ�ַ
} TIPC_DNS_CFG, *PTIPC_DNS_CFG;

// �����������ò���
typedef struct tagIPCLocalNetCfg
{
	unsigned char   byAddrMode;                                       // ��ַʹ�÷�ʽ���̶�IP����DHCP
	unsigned char   byDhcpEthId;                                      // ��ַDHCP�����ں�
	unsigned char   byUseDhcpGateway;                                 // �Ƿ�ʹ��DHCP������
	unsigned long	dwIpAddr;				                          // �豸IP��ַ(Ĭ��:IPCSDK_DEFCFG_IPADDR)
	unsigned long	dwSubnetMask;			                          // �豸��������(Ĭ��:IPCSDK_DEFCFG_NETMASK)
	unsigned long   dwGateWay;				                          // ���ص�ַ(Ĭ��:IPCSDK_DEFCFG_GATEWAY)
	unsigned char	abyMacAddr[6];			                          // mac��ַ(�ڲ�ʹ��)
	unsigned char   byUseDdns;                                        // �Ƿ�ʹ��DDNS
	TIPC_DDNS_CFG   tDdnsCfg;                                         // ddns��Ϣ
	unsigned char   byChangeMask;			                          // �Ƿ�ı�mask,Ŀǰ���������ṹ�Ƿ��޸�(����ģ��)
	TIPC_DNS_CFG	tDnsCfg;                                          // DNS��Ϣ
} TIPC_LOCAL_NET_CFG, *PTIPC_LOCAL_NET_CFG;
#ifdef WIN32
#pragma pack(pop)
#endif

/*����ͨ�����ӹ�ϵ*/
typedef struct tagIPCVideoEncChanRelationItem
{
    unsigned char       byChannelId;                                  // ����ͨ��Id
    unsigned char       byMainChannelId;                              // ��Ӧ������ͨ��Id
} TIPC_VIDEO_ENC_CHAN_RELATION_ITEM, *PTIPC_VIDEO_ENC_CHAN_RELATION_ITEM;

typedef struct tagIPCVideoEncChanRelation
{
    unsigned char                       byChannelNum;                 // ͨ����
    TIPC_VIDEO_ENC_CHAN_RELATION_ITEM   atChannel[32];                // ͨ����Ϣ	
} TIPC_VIDEO_ENC_CHAN_RELATION, *PTIPC_VIDEO_ENC_CHAN_RELATION;

// ��Ƶ�����������
typedef struct tagIPCVideoEncNetParam
{
	unsigned char			byEncID;				                  // ��Ƶ����ͨ����
	unsigned char			byInitStreamState;		                  // ������ʼ��״̬ 0-������, 1-����
	unsigned char			byCurStreamState;		                  // ������ǰ״̬ 0-δ����, 1-�ѷ���
	TIPC_CON_TYPE	        tSupportedConnType;		                  // ֧�ֵ�������������
	unsigned char			byConnectionType;		                  // ������������(IPCSDK_CONNECTION_UNICAST��)
	unsigned long			dwStreamIpAddr;			                  // ���������ַ
	unsigned short			wStreamPort;			                  // ��������˿�
} TIPC_VIDEC_ENC_NET_PARAM, *PTIPC_VIDEC_ENC_NET_PARAM;

// ��Ƶѹ����ʽ
typedef struct tagIPCVideoType
{
	unsigned char bySN4;                                              // ֧��Ϊ1����֧��Ϊ0����ͬ
	unsigned char byMPEG4;
	unsigned char byH261;
	unsigned char byH263;
	unsigned char byH264;
} TIPC_VIDEO_TYPE, *PTIPC_VIDEO_TYPE;

// ��Ƶͼ��ֱ���
typedef struct tagIPCVideoResolution
{
	unsigned char byAuto;                                             // ֧��Ϊ1����֧��Ϊ0����ͬ
	unsigned char byQCIF;
	unsigned char byCIF;
	unsigned char by2CIF;
	unsigned char by4CIF;
	unsigned char byVGA;
	unsigned char byQQCIF;
	unsigned char byQVGA;
	unsigned char by720P;
	unsigned char by1080P;
	unsigned char byQXGA;
} TVIDEO_IMG_RESOLUTION, *PTVIDEO_IMG_RESOLUTION;

// ��Ƶ�ֱ��ʸ�ʽ
typedef struct tagIPCSPVideoResolution
{
	TVIDEO_IMG_RESOLUTION tSN4;
	TVIDEO_IMG_RESOLUTION tMEPG4;
	TVIDEO_IMG_RESOLUTION tH261;
	TVIDEO_IMG_RESOLUTION tH263;
	TVIDEO_IMG_RESOLUTION tH264;
} TIPC_SP_VIDEO_RESOLUTION, *PTIPC_SP_VIDEO_RESOLUTION;

// ��Ƶ��������ṹ
typedef struct tagIPCVideoEncParam
{
	unsigned char					 byEncID;				          // ��Ƶͨ����
	TIPC_VIDEO_TYPE			         tSupportedVideoType;	          // ֧�ֵ���Ƶ��ʽ(ֻ��)
	TIPC_SP_VIDEO_RESOLUTION         tSupportedVideoResolution;       // ֧�ֵ���Ƶ�ֱ���(ֻ��)
	unsigned char					 byVideoType;			          // ��Ƶ�����ʽ(IPCSDK_VIDEO_TYPE_SN4��)
	unsigned char					 byVideoResolution;		          // ͼ��ֱ���(IPCSDK_VIDEO_RESOLUTION_AUTO��)
	unsigned char					 byFrameRate;			          // ֡�� 1-25
	unsigned char					 byQuality;				          // ͼ������ 1-2 (��������/�ٶ�����)
	unsigned long					 dwBitRate;				          // ��������� 64K-4M
	unsigned char					 byMaxQuantization;		          // ����������� 1-31
	unsigned char					 byMinQuantization;		          // ��С�������� 1-31
	unsigned short					 wInterval;				          // �ؼ�֡��� 25-2500
	char					         chBrightness;			          // ���� -128��127
	char					         chContrast;				      // �Աȶ� -128��127
	char					         chSaturation;			          // ���Ͷ� -128��127
    unsigned char			         byBitRateType;                   // 0:������, 1:������
	unsigned char		             byStreamType;				      // 0:RTP, 1:TS
	unsigned char		             bySharpness;				      // ��� 0: not support, 1~4: available
	unsigned char                    byVideoFrame;                    // ��֡
	unsigned char                    byMaxFrameRate;                  // ֧�ֵ����֡��(ֻ��)
	unsigned char 	                 byEncRankId;                     // ͼ�����Э��ȼ�
	unsigned char                    byEncFormat;                     // ͼ������ʽ
} TIPC_VIDEO_ENC_PARAM, *PTIPC_VIDEO_ENC_PARAM;

// ��Ƶ���빫�������ṹ
typedef struct tagIPCVideoEncPubParam
{
	unsigned char		byMaxVideoEncNum;	                          // ��������Ƶ����ͨ������(ֻ��)
	unsigned char		byVideoEncUsed;		                          // ���õ���Ƶ����ͨ����
	unsigned char		abyVideoEncCap[MAX_VIDEOIN_PORT_NM_LEN];	  // ֧�ֵ���Ƶ��������(ֻ��)
	unsigned char		byMaxVideoEncCapNum;	                      // ֧�ֵı�����������(ֻ��)
} TIPC_VIDEO_ENC_PUB_PARAM, *PTIPC_VIDEO_ENC_PUB_PARAM;

// ��Ƶ������Ļ�����ṹ
typedef struct tagIPCVideoEncOSDParam
{
	unsigned char		byEncID;				                      // ����ͨ����
	unsigned char		byShowTimeState;		                      // �Ƿ���ʾʱ�� 0-����ʾ, 1-��ʾ
	unsigned char		byOSDState;				                      // ������Ļ״̬ 0-����Ļ, 1-����Ļ
} TIPC_VIDEO_ENC_OSD_PARAM, *PTIPC_VIDEO_ENC_OSD_PARAM;

// ��Ƶ������ƵԴ�˿ڲ����ṹ
typedef struct tagIPCVideoEncInputPortParam
{	
	unsigned char	byEncID;				                          // ����ͨ��
															          // �м�����Ƶ����˿���������м�������ã�������ֵ���ʾ�����Ƶ����ID;
															          // ͨ����Ƶ����˿�N��Ӧ��ƵԴIDҲ��N
	unsigned char	abyValidVideoInputPort[MAX_VIDEOIN_PORT_NUMBER];  // ���õ���Ƶ����˿ں�(ֻ��)	
	unsigned char	byValidVideoInputPortNum; 	                      // ���õ���Ƶ����˿�����(ֻ��)
	unsigned char	byVideoInputPort;		                          // ��ǰʹ�õ���Ƶ����˿ں�
} TIPC_VIDEO_ENC_INPUT_PT_PARAM, *PTIPC_VIDEO_ENC_INPUT_PT_PARAM;

// ��Ƶ�����������
typedef struct tagIPCVideoDecNetParam
{
	unsigned char			byDecID;				                  // ����ͨ����
	unsigned char			byInitStreamState;		                  // ������ʼ��״̬ 0-������, 1-����
	unsigned char			byCurStreamState;		                  // ������ǰ״̬ 0-δ����, 1-�ѷ���
	TIPC_CON_TYPE	        tSupportedConnType;		                  // ֧�ֵ�������������
	unsigned char			byConnectionType;		                  // ������������(IPCSDK_CONNECTION_UNICAST��)
	unsigned long			dwStreamIpAddr;			                  // ���������ַ
	unsigned short			wStreamPort;			                  // ��������˿�
} TIPC_VIDEO_DEC_NET_PARAM, *PTIPC_VIDEO_DEC_NET_PARAM;

// ��Ƶ��������ṹ
typedef struct tagIPCVideoDecParam
{
	unsigned char			byDecID;				                  // ����ͨ��
	TIPC_VIDEO_TYPE	        tSupportedVideoType;	                  // ֧�ֵ���Ƶ��ʽ(ֻ��)
	unsigned char			byPlayerBufferNum;		                  // ���Ż������� 6-50
	unsigned char			byPlayerOriginBufNum;	                  // ������ʼ�������� 0-10
	unsigned char			byVideoType;			                  // ��Ƶ�����ʽ(IPCSDK_VIDEO_TYPE_SN4��)
	unsigned char			byDecDealLev;			                  // ͼ�������(0��3��,0��ʾû��)
	unsigned char		    bySharpFactor; 
} TIPC_VIDEO_DEC_PARAM, *PTIPC_VIDEO_DEC_PARAM;

// ��Ƶ���빫�������ṹ
typedef struct tagIPCVideoDecPubParam
{	
	unsigned char		byDecID;					                  // ��Ƶ����ͨ��
	unsigned char		byVGAOutSupported;			                  // �Ƿ�֧��VGA���(ֻ��) 0-��֧��, 1-֧��
	unsigned char		byVideoOutputType;			                  // ��Ƶ����˿�����(IPC_VIDEO_OUT_TYPE_C��)
	unsigned char		byVGAFrequency;				                  // VGA���ˢ��Ƶ��(IPC_VGA_OUT_FREQUENCY_60��)
	unsigned char		byStarveMode;				                  // ����Ƶ����ʾ��ʽ
} TIPC_VIDEO_DEC_PUB_PARAM, *PTIPC_VIDEO_DEC_PUB_PARAM;

// ��Ƶ�����������
typedef struct tagIPCAudioEncNetParam
{
	unsigned char			byAEncID;				                  // ��Ƶ����ͨ����
	unsigned char			byInitStreamState;		                  // ������ʼ��״̬ 0-������, 1-����
	unsigned char			byCurStreamState;		                  // ������ǰ״̬ 0-δ����, 1-�ѷ���
	TIPC_CON_TYPE	        tSupportedConnType;		                  // ֧�ֵ�������������
	unsigned char			byConnectionType;		                  // ������������(IPCSDK_CONNECTION_UNICAST��)
	unsigned long			dwStreamIpAddr;			                  // ���������ַ
	unsigned short			wStreamPort;			                  // ��������˿�
} TIPC_AUDIO_ENC_NET_PARAM, *PTIPC_AUDIO_ENC_NET_PARAM;

// ��Ƶѹ����ʽ
typedef struct tagIPCAudioType
{
	unsigned char byPCMA;                                             // ֧��Ϊ1����֧��Ϊ0����ͬ
	unsigned char byPCMU;
	unsigned char byGSM;
	unsigned char byG729;
	unsigned char byADPCM;
	unsigned char byG722;
    unsigned char byG7221C;
	unsigned char byAACLC;
} TIPC_AUDIO_TYPE, *PTIPC_AUDIO_TYPE;

// ��Ƶ��������ṹ
typedef struct tagIPCAudioEncParam
{
	unsigned char		byAEncID;				                      // ��Ƶ����ͨ��
	TIPC_AUDIO_TYPE	    tSupportedAudioType;	                      // ֧�ֵ�����ѹ����ʽ(ֻ��)
	unsigned char		byAudioType;			                      // ��Ƶ���ݸ�ʽ(IPCSDK_AUDIO_TYPE_PCMA��)
	unsigned char		byAudioInputType;		                      // ��Ƶ��������(IPCSDK_AUDIO_INPUTTYPE_LINEIN��)
	unsigned char		abyAudioInputType[MAX_VIDEOIN_PORT_NM_LEN];   // ֧�ֵ���������
	unsigned char		byAudioMaxInputType;	                      // ������������
	unsigned char		byVolume;				                      // ���� 0-25
	unsigned char		byAECState;				                      // ��������״̬ 0-��Ч, 1-��Ч
	unsigned char		byDumbState;			                      // ����״̬ 0-��Ч, 1-��Ч
} TIPC_AUDIO_ENC_PARAM, *PTIPC_AUDIO_ENC_PARAM;

// ��Ƶ�����������
typedef struct tagIPCAudioDecNetParam
{
	unsigned char			byADecID;				                  // ��Ƶ����ͨ����
	unsigned char			byInitStreamState;		                  // ������ʼ��״̬ 0-������, 1-����
	unsigned char			byCurStreamState;		                  // ������ǰ״̬ 0-δ����, 1-�ѷ���
	TIPC_CON_TYPE	        tSupportedConnType;		                  // ֧�ֵ�������������
	unsigned char			byConnectionType;		                  // ������������(IPCSDK_CONNECTION_UNICAST��)
	unsigned long			dwStreamIpAddr;			                  // ���������ַ
	unsigned short			wStreamPort;			                  // ��������˿�
} TIPC_AUDIO_DEC_NET_PARAM, *PTIPC_AUDIO_DEC_NET_PARAM;

// ��Ƶ��������ṹ
typedef struct tagIPCAudioDecParam
{
	unsigned char			byADecID;				                  // ��Ƶ����ͨ��
	TIPC_AUDIO_TYPE	        tSupportedAudioType;	                  // ֧�ֵ�����ѹ����ʽ(ֻ��)
	unsigned char			byVolume;				                  // ���� 0-25
	unsigned char			byMuteState;			                  // ����״̬ 0-��Ч, 1-��Ч
	unsigned char	        byMixRecord;			                  // ����¼�����
} TIPC_AUDIO_DEC_PARAM, *PTIPC_AUDIO_DEC_PARAM;

// ��������
typedef struct tagIPCSerialType
{
	unsigned char byRS232;                                            // ֧��Ϊ1����֧��Ϊ0����ͬ
	unsigned char byRS422;
	unsigned char byRS485;
} TIPC_SERIAL_TYPE, *PTIPC_SERIAL_TYPE;

// ���ڲ����ṹ
typedef struct tagIPCSerialPortParam
{
	unsigned char			 bySerialPortID;			              // ����ID
	unsigned long			 dwBaudRate;				              // ��ǰʹ�õĴ��ڲ�����
	unsigned char			 byComPortType;			                  // ��ǰʹ�õĴ������� 0:��ʹ��, 1:RS232, 2:RS422, 3:RS485
	TIPC_SERIAL_TYPE         tSupportedType;		                  // ֧�ֵĴ������� (ֻ��)
	unsigned char			 byStop;					              // ֹͣλ
	unsigned char			 byDig;					                  // ����λ
	unsigned char			 byParityCheck;			                  // ��żУ��
	unsigned char			 byStart;				                  // ��ʼλ
} TIPC_SERIAL_PT_PARAM, *PTIPC_SERIAL_PT_PARAM;

// ���ڹ��������ṹ
typedef struct tagIPCSerialPortPubParam
{
	unsigned char			bySerialPortID;			                  // �����Ĵ���ID
	unsigned char			byInitStreamState;		                  // ���������շ���ʼ��״̬ 0-��Ч, 1-��Ч
	unsigned short			wStreamLocalPort;		                  // �������ݱ��ؽ��ն˿�
	unsigned long			dwStreamDestIpAddr;		                  // �������ݶԶ˵�ַ
	unsigned short			wStreamDestPort;		                  // �������ݶԶ˶˿�
	TIPC_CON_TYPE	        tSupportedConnType;		                  // ֧�ֵ�������������
	unsigned char			byConnectionType;		                  // ������������(IPCSDK_CONNECTION_UNICAST��)
	unsigned char			byProtocolType;			                  // Э������ 0-͸��ͨ��, 1-VSIPЭ��
	unsigned char			bySerialId;                               // �Զ˵Ĵ���id
} TIPC_SERIAL_PT_PUB_PARAM, *PTIPC_SERIAL_PT_PUB_PARAM;

// ϵͳʱ������ṹ
typedef struct tagIPCSysTime
{
	unsigned short		wYear;		                                  // ��
	unsigned char		byMonth;	                                  // �� 1-12
	unsigned char		byDay;		                                  // �� 1-31
	unsigned char		byHour;		                                  // ʱ 0-23
	unsigned char		byMinute;	                                  // �� 0-59
	unsigned char		bySecond;	                                  // �� 0-59
} TIPC_SYS_TIME, *PTIPC_SYS_TIME;

// ������������ṹ
typedef struct tagIPCInputPinParam
{
	unsigned char		  byPinID;					                  // ����ID
	unsigned char		  byAlarmInitState;		                      // ����澯�ͳ�̬��ѡ
	unsigned char		  byAlarmDetectState;			              // �澯���״̬ 0-�����, 1-ֻ���һ������, 2-ÿ��ѭ�����, 3-ʼ�ռ��
	TIPC_SYS_TIME         atAlarmDetectStartTime[MAX_WEEKLYDAY_NUM];  // �澯��⿪ʼʱ��,һ�ܣ���
	TIPC_SYS_TIME         atAlarmDetectDuration[MAX_WEEKLYDAY_NUM];	  // �澯������ʱ��,һ�ܣ���
	unsigned char		  byRelateEncId;				              // �澯�����ı���ͨ�� (�޹�������ͨ����Ϊ IPCSDK_RELATE_ENCID_DISABLE)
	unsigned char		  byOutputPinID;				              // �澯�Զ��������ID (�޿��������Ϊ IPCSDK_OUTPUT_DISABLE)
	unsigned char		  byAlarmOSDState;			                  // �澯��Ļ״̬ 0-����ʾ, 1-��ʾ
	unsigned short		  wAlarmOSDLeftMargin;		                  // �澯��Ļ��߾� 1-200
	unsigned short		  wAlarmOSDTopMargin;			              // �澯��Ļ�ϱ߾� 1-200
	char		          achAlarmOSDName[MAX_STR_LEN_64+1];          // �澯��Ļ����
	unsigned long		  dwAlarmOSDColor;			                  // �澯��Ļ��ɫ(RGB)
	unsigned char		  byEthAlarmState;			                  // �Ƿ���ETH1���ڸ澯
} TIPC_INPUT_PIN_PARAM, *PTIPC_INPUT_PIN_PARAM;

// ��ƵԴ����˿ڲ����ṹ
typedef struct tagIPCVideoInputParam
{
	unsigned char		byVideoInputID;				                  // ��ƵԴ�˿ں�
	unsigned char		byCameraType;				                  // ����ͷ����(IPC_CAMERA_TYPE_SONY��)
	unsigned char		byCameraID;					                  // ����ͷ��ַ��
	unsigned char		bySerialPortID;				                  // ��Ӧ�Ĵ���ID
	char		        achVideoPortName[MAX_VIDEOIN_PORT_NM_LEN+1];  // ��ƵԴ�˿���
	char		        achVideoPortOSD[MAX_STRING_OSD_LEN+1];        // ��ƵԴ��Ļ����
	unsigned short		wVideoPortOSDLeftMargin;	                  // ��ƵԴ��Ļ��߾� 1-200
	unsigned short		wVideoPortOSDTopMargin;		                  // ��ƵԴ��Ļ�ϱ߾� 1-200
	unsigned long		dwVideoPortOSDColor;		                  // ��ƵԴ��Ļ��ɫ
	unsigned char       byInputType;	
	unsigned short      wVideoPortTimeLeftMargin;                     // ��ƵԴ��Ļ��߾� 1-200
    unsigned short      wVideoPortTimeTopMargin;                      // ��ƵԴ��Ļ�ϱ߾� 1-200
    unsigned short      wVideoPortAlarmLeftMargin;                    // ��ƵԴ�澯��Ļ��߾� 1-200
    unsigned short      wVideoPortAlarmTopMargin;                     // ��ƵԴ�澯��Ļ�ϱ߾� 1-200
} TIPC_VIDEO_INPUT_PARAM, *PTIPC_VIDEO_INPUT_PARAM;

// ���ܲ���
typedef struct tagIPCNmsParam
{
	unsigned char	 byMemThreshold;			                      // ϵͳ�ڴ�ʹ�÷�ֵ
	unsigned char    byMaxVideoLoseRatio;	                          // ��Ƶ�����ϱ���ֵ
	unsigned char    byCpuUseRatio;			                          // cpu������
	unsigned long	 dwServerIP;				                      // ���ܷ�����IP��ַ
} TIPC_NMS_PARAM, *PTIPC_NMS_PARAM;

// �����������
typedef struct TIPCPuNmsConfig
{
	unsigned char	byRegType;								          // ��������:DNS,PUI
	unsigned long	dwDNSIp;								          // DNS��ip��ַ
	unsigned short  wDNSPort;								          // DNS�˿�
	unsigned short  wPingCheckTimes;					 	          // ����������
	unsigned short 	wPingCheckInterval;						          // �������ʱ����
	unsigned long	dwVersionServerIp;						          // �汾������IP
	unsigned short  wVersionServerPort;						          // �汾�������˿�
	unsigned long   dwVersionCheckInterval;					          // �汾������� ��λ:����
	char	        achVersionPath[MAX_PATH_LENGTH+1];			      // ���ذ汾�ļ���Ϣ
	char	        achVersionUserName[MAX_STR_LEN_64+1];	          // �汾�������û���
	char	        achVersionUserPW[MAX_STR_LEN_64+1];		          // �汾����������
	unsigned char	bySlotId;							         	  // �ۺ�
	char	        achBoxInfo[MAX_STR_LEN_64+1];			          // ��λ��Ϣ
	char	        achLocationInfo[MAX_STR_LEN_64+1];		          // λ����Ϣ
	unsigned short  DownloadStartPort;                                // ������ʼ�˿�
	unsigned short  BrowseStartPort;                                  // �����ʼ�˿�
} TIPC_PU_NMS_CONFIG,*PTIPC_PU_NMS_CONFIG;

// PPPOE�����ṹ
typedef struct tagIPCPPPOEParam
{
	unsigned char		byPPPOEState;				                  // PPPOE״̬ 0-��ʹ��, 1-ʹ��
	char		        achUsername[MAX_USER_NUMBER];	              // �����û���
	char		        achPassword[MAX_USER_NUMBER];	              // ��������
	unsigned char		byAutoDialState;			                  // �Զ�����״̬ 0-��ʹ��, 1-ʹ��
	unsigned long		dwAutoDialWait;				                  // �Զ����ŵȴ�ʱ��(��)
	unsigned long		dwAutoDialTry;				                  // �Զ��������Դ���
	unsigned char		byDebugState;				                  // Debug״̬ 0-��ʹ��, 1-ʹ��
    unsigned short		wLcpEchoSendInterval;		                  // ���ӽ����󣬷���LCP-ECHO����ʱ����
    unsigned short		wLcpEchoRetryTimes;			                  // ���ͼ���LCP-ECHO��û�еõ���Ӧ����Ϊ���ӶϿ�
} TIPC_PPPOE_PARAM, *PTIPC_PPPOE_PARAM;

// ��Ƶ�����ش������ṹ
typedef struct tagIPCVideoDecRetransferParam
{
	unsigned char		byDecID;					                  // ��Ƶ����ͨ��
	unsigned short		wRetransfer1stTimeSpan;		                  // �����1���ش�����
	unsigned short		wRetransfer2ndTimeSpan;		                  // �����2���ش�����
	unsigned short		wRetransfer3rdTimeSpan;		                  // �����3���ش�����
	unsigned short		wRejectTimeSpan;			                  // �������ʱ��
	unsigned char		byRetransferState;			                  // �����ش�״̬ 0-���ش�, 1-�ش�
} TIPC_VIDEO_DEC_RETRANSFER_PARAM, *PTIPC_VIDEO_DEC_RETRANSFER_PARAM;

// ¼��ϵͳ����
typedef struct tagIPCRecSysInfo
{
	unsigned char	byDiskFullStrategy;                               // ������¼�����
	unsigned char	abyRecFileStorePath[MAX_DISK_NUM];                // ¼���ļ����·��
	unsigned long	dwDiskRemainSpace;                                // ����ʣ��ռ�(MB)
	unsigned short	wLittleFileRecTime;                               // С�ļ�¼��ʱ��
	unsigned char	abyRecDiskPartitionInfo[MAX_DISK_NUM][MAX_DISK_PARTITION_NAME]; // ���̷�����Ϣ���û���������
	unsigned char	byDiskPartitionNum;                               // �ļ�������Ŀ,�û���������
	unsigned char	byUsedPartitionNum;                               // �û�ʹ���˵ķ�����Ŀ
} TIPC_REC_SYS_INFO, *PTIPC_REC_SYS_INFO;

// ¼���������
typedef struct tagIPCRecChannelBasicInfo
{
	unsigned char	byChannel;
	unsigned short	wAlarmPreRecTime;				                  // �澯Ԥ¼ʱ��
	unsigned short	wAlarmResumeRecTime;			                  // �澯�ָ������¼��ʱ��
	unsigned char	byIsAbstractFrame;				                  // �Ƿ��֡¼�� 1Ϊ�ǣ�0Ϊ��
	unsigned char	byIsRecWhenAlarmOccur;			                  // �澯�Ƿ�¼��,1Ϊ��,0Ϊ��
	unsigned char	byAbstractRecInterval;			                  // ��֡¼����,ָ�ؼ�֡����
	unsigned char	byIsAudioRecorded;				                  // �Ƿ�¼��Ƶ
	unsigned char   byRecChanNum;                                     // 0-¼����������������¼�Ƹ���������,Ĭ��Ϊ0
} TIPC_REC_CHANNEL_BASIC_INFO, *PTIPC_REC_CHANNEL_BASIC_INFO;

// ��ʼֹͣ¼��ʱ�����ýṹ
typedef struct tagIPCRecStartEndTimeInfo
{
	TIPC_SYS_TIME StartTime;				                          // ��ʼ¼��ʱ��
	TIPC_SYS_TIME EndTime;					                          // ֹͣ¼��ʱ��
} TIPC_REC_START_END_TIME_INFO, *PTIPC_REC_START_END_TIME_INFO;

// ��ʱ¼��
typedef struct tagIPCTimelyRecInfo
{
	unsigned char	byChannel;
    unsigned char	byEnable;
	TIPC_REC_START_END_TIME_INFO RecSETimeInfo;
} TIPC_TIMELY_REC_INFO, *PTIPC_TIMELY_REC_INFO;

// ����¼��
typedef struct tagIPCPeriodlyTimeInfo
{
	unsigned char	byChannel;
	unsigned char	byPeriodType;					                  // ����¼�����(IPCSDK_PERIOD_DAYLY��)
	TIPC_REC_START_END_TIME_INFO aRTimeInfo[3*(MAX_WEEKLYDAY_NUM+1)];
} TIPC_PERIODLY_TIME_INFO, *PTIPC_PERIODLY_TIME_INFO;

// ץ�Ļ�������
typedef struct tagIPCSnapShotConfig
{
	unsigned char	byChannel;
	unsigned char	byPinAlarmEnable;				                  // ���ڸ澯ץͼʹ�� 0�ر�/1����
	unsigned char	byMotionDetectEnable;			                  // �ƶ����ץͼʹ�� 0�ر�/1����
	unsigned long	dwInterval;						                  // ץ��ʱ����(s)
	unsigned long	dwMaxSnapNum;					                  // ���ץ������
} TIPC_SNAPSHOT_CONFIG,*PTIPC_SNAPSHOT_CONFIG;

// ץ����������
typedef struct tagIPCSnapShotNamingRule
{
	unsigned char abyNameOrder[MAX_SEPA_LEN];                         // �ֶ�˳�򣬴�0��ʼ�����α�ʾ��i+1���ֶ�
	unsigned char byNameNum;                                          // ʵ��ʹ�õ��ֶθ���	
	char          achDateSeparator[MAX_SEPA_LEN];                     // ����֮��ķָ���(������)	
	char          achTimeSeparator[MAX_SEPA_LEN];                     // ʱ��֮��ķָ���(ʱ����)	
	char          achNameSeparator[MAX_SEPA_LEN];                     // �����ֶ�֮��ķָ��� 	
	char          achUserDefine[MAX_STR_LEN_64+1];                    // �û��Զ����ֶ�����	
	int           bIsRemoteStore;                                     // �Ƿ�Զ�˴洢	
	char          achSvrSharePath[MAX_PATH_LENGTH + 1];               // ����������·������ʽ��172.16.3.108:/usr/share) 
} TIPC_SNAPSHOT_NAMING_RULE,*PTIPC_SNAPSHOT_NAMING_RULE;

// ���Ų���
typedef struct tagIPCSnapShotExtCfg
{
	unsigned char  byChannel;                                         // ͨ���� 
	unsigned short wReciprocal;	                                      // ��ǰ�Ŀ��Ų�������
	unsigned short wSupportListNum; 	                              // ֧�ֵĿ��Ų����б���(ֻ��)
	unsigned short awSupportList[MAX_RECIPROCAL_LIST_SIZE];	          // ֧�ֵĿ��Ų����б�(ֻ��)	
} TIPC_SNAPSHOT_EXT_CFG, *PTIPC_SNAPSHOT_EXT_CFG;

// ��ƽ�����
typedef struct tagIPCWhiteBalance
{
	unsigned short wBluePlus;                                         // ������
	unsigned short wRedPlus;                                          // ������
} TIPC_WHITE_BALANCE, *PTIPC_WHITE_BALANCE;

// ˫������
typedef struct tagIPCMultiChanInfo
{
	unsigned char byChanType;                                         // �������Ǹ�����,1��������0������
	unsigned char bySupportSubNum;                                    // ֧�ָ�����·����������Ϊ0;����ǿ����ڶ�·�����������������š�
	unsigned char SubChanId[8];				                          // ������ID�б����ݸ�����·����������д
	unsigned char byStatus;                                           // ��ǰ�������Ƿ�����1��������0-δ���������������У�����ǿ���˫��������1�����ֹͣ˫������0��
    unsigned char byParentEncId;                                      // ����Ϊ0��������Ϊ�����ı���ͨ����
} TIPC_MULTI_CHAN_INFO, *PTIPC_MULTI_CHAN_INFO;

// ��̨������ز���
typedef struct tagIPCPtzCtrlInfo
{
	unsigned char  bySupportIMageFlip;                                 // �Ƿ�֧��ͼ��ת 0-��֧�� 1-֧��
	unsigned char  bySupportDevFlip;                                   // �Ƿ�֧����̨��ת 0-��֧�� 1-֧��
	unsigned char  bySupportBackLight;                                 // �Ƿ�֧�ֱ��ⲹ�� 0-��֧�� 1-֧��
	unsigned char  bySupportDayNight;                                  // �Ƿ�֧����ҹģʽ 0-��֧�� 1-֧��
	unsigned char  bySupportWdr;                                       // �Ƿ�֧�ֿ�̬   0-��֧�� 1-֧��
	unsigned char  bySupportWB;                                        // �Ƿ�֧�ְ�ƽ��   0-��֧�� 1-֧��
	unsigned char  bySupportShutter;                                   // �Ƿ�֧�ֿ���     0-��֧�� 1-֧��
	unsigned char  bySupportGain;                                      // �Ƿ�֧������     0-��֧�� 1-֧��
	unsigned char  bySupportImgScale;                                  // �Ƿ�֧�־������ 0-��֧�� 1-֧��
	unsigned char  bySupportIR;                                        // �Ƿ�֧�ֺ���     0-��֧�� 1-֧��
	unsigned char  bySupportLaser;                                     // �Ƿ�֧�ּ���     0-��֧�� 1-֧��
	unsigned char  bySupportWiper;                                     // �Ƿ�֧����ˢ     0-��֧�� 1-֧��
	unsigned char  bySupportDefrost;                                   // �Ƿ�֧�ֳ�˪     0-��֧�� 1-֧��
	unsigned char  bySupportFocus;                                     // �Ƿ�֧�־۽�     0-��֧�� 1-֧�� 

	unsigned char  byDayNightSetMode;                                  // ��ҹģʽ�趨��ʽ 0-�Զ�   1-�ֶ�
	unsigned char  byWBSetMode;                                        // ��ƽ���趨��ʽ   0-�Զ�   1-�ֶ�
	unsigned char  byShutterSetMode;                                   // �����趨��ʽ     0-�Զ�   1-�ֶ�
	unsigned char  byGainSetMode;                                      // �����趨��ʽ     0-�Զ�   1-�ֶ�
	unsigned char  byFocusMode;                                        // �۽��趨��ʽ     0-�Զ�   1-�ֶ�

	unsigned char  byBackLightState;                                   // ���ⲹ��״̬     0-��     1-��
	unsigned char  byDayNightState;                                    // ��ҹģʽ״̬     0-��ģʽ 1-ҹģʽ
	unsigned char  byWdrState;                                         // ��̬״̬       0-��     1-��
	unsigned char  byWBValueMode;                                      // ��ƽ��ֵģʽ     0-��ͨ   1-�߼�(��������)
	unsigned char  byWBValue;                                          // ��ƽ��-��ֵͨ
	unsigned short wWBRedPlus;                                         // ��ƽ��-������ֵ
	unsigned short wWBBluePlus;                                        // ��ƽ��-������ֵ
	unsigned long  dwShutterValue;                                     // ����ȡֵ(��ĸֵ)
	unsigned long  dwGainValue;                                        // ����ȡֵ(��Ϊ6�ı���)
	unsigned long  dwMaxGainVal;                                       // �������ֵ(��Ϊ6�ı���)
	unsigned long  dwFocusVal;                                         // �۽�ֵ(�ֶ�ģʽ)
} TIPC_PTZ_CTRL_INFO, *pTIPC_PTZ_CTRL_INFO;

/*************************************************
/	                   OSD
/*************************************************/
// OSD̨��
typedef struct tagIPCAdjustPicInfo
{
	unsigned short wStartX;				                              // ͼƬС����ʼ����X��
	unsigned short wStartY;                                           // ͼƬС����ʼ����Y��
	unsigned char  byRegionHeight;		                              // ͼƬС��ĸߣ�ÿ��С�鶼��ͬ)
	unsigned char  byRegionWidthSize;		                          // ��������ṹ������
	unsigned char  abyRegionWidth[16];	                              // ͼƬ����С��Ŀ�
} TIPC_ADJUST_PIC_INFO, *PTIPC_ADJUST_PIC_INFO;

// ̨����Ļ�ϴ�
typedef struct tagTitleUpload
{
	unsigned long dwPackInd;                                          // �ļ����ݰ�ID
	unsigned char byMsgType;                                          // ̨����Ļ����
	unsigned char *pTitleData;                                        // �ļ�����
	unsigned long dwDataSize;                                         // �ļ���С
	unsigned char byAlarmPinId;                                       // ���ڸ澯ID
} TIPC_TITLE_UPLOAD, *PTIPC_TITLE_UPLOAD;

/*************************************************
/	                 �ƶ����
/*************************************************/
// �ƶ��������ṹ��
typedef struct tagIPCMotionDetectParam
{
	unsigned char   byDetectEnable;			                          // ���ʹ�� 1-��Ч 0-��Ч
	unsigned long   dwXPos;					                          // ������򶥵������
	unsigned long   dwYPos;					                          // ������򶥵�������
	unsigned long   dwWidth;					                      // ���������
	unsigned long   dwHeigth;			                              // �������߶�
	unsigned char   byAlarmRate;				                      // �������澯�����ٷֱȣ����ڴ��ٽ�ֵ�澯,0-100��
	unsigned char   byResumeRate;			                          // �����澯���˶���Χ�ٷֱ�С�ڸ�ֵ��ָ��澯(��ֵС��byAlarmRate,0-100)
	short           swDetSense;				                          // ������ -10~10
} TIPC_MOTION_DETECT_PARAM, *PTIPC_MOTION_DETECT_PARAM;

typedef struct tagIPCMotionDetect
{
	unsigned char            byType;							      // �ƶ��������(IPC_ENCODER-����;IPC_DECODER-����)
	unsigned char            byEntityID;						      // �ƶ����ʵ��ID(����Ϊ��ƵԴ�˿ں�, ����Ϊ������ͨ����)
	TIPC_MOTION_DETECT_PARAM aMotionDetectParam[MAX_MOTION_DETECT];   // �ƶ�����������(����Ϊʮ����Ԫ������)
	unsigned short           wDetAreaNum;						      // �����ƶ����������
} TIPC_MOTION_DETECT, *PTIPC_MOTION_DETECT;

/*************************************************
/	                 ͼ���ڱ�
/*************************************************/
// ͼ���ڱο��ƽṹ��
typedef struct tagIPCShadeParam
{
	unsigned char          byShadeEnable;			                  // �ڱ�ʹ�� 1-��Ч 0-��Ч
	unsigned long          dwXPos;				                      // �ڱ����򶥵������
	unsigned long          dwYPos;				                      // �ڱ����򶥵�������
	unsigned long          dwWidth;				                      // �ڱ�������
	unsigned long          dwHeigth;				                  // �ڱ�����߶�
} TIPC_SHADE_PARAM, *PTIPC_SHADE_PARAM;

// ͼ���ڱο�������
typedef struct tagIPCVideoShadeParam
{
	unsigned char       byEntityID;                                   // ��ƵԴ�˿�ID
	TIPC_SHADE_PARAM    atSDArea[MAX_MOTION_DETECT];                  // ͼ���ڱο�������
	unsigned short      wAreaNum;                                     // ������Ŀ
} TIPC_VIDEO_SHADE_PARAM, *PTIPC_VIDEO_SHADE_PARAM;

/*************************************************
/	                 ��Ƶ���
/*************************************************/
// ��������ʼ�������ṹ
typedef struct tagIPCDecWrapInitParam
{
	unsigned long		dwConnectIP;
	unsigned short		wStartPort;		                              // ��ʼ�˿�
		   					                                          // �������������Լ���ID,ʹ������ʼ�˿ں���ĵ�(wStartPort+ID*4)���˿�
	unsigned long		dwChnID;
	unsigned long		dwVideoInputPort;
	unsigned long		dwRecFileID;
    unsigned char       byRecPlayId;
	unsigned short		wDecID;
	unsigned long    	lWnd;
} TIPC_DEC_WRAP_INIT_PARAM, *PTIPC_DEC_WRAP_INIT_PARAM;

// ������������ṹ
typedef struct tagIPCCommonNetParam
{
	unsigned char   byInitStreamState;		                          // ������ʼ��״̬ 0-������, 1-����
	unsigned char	byCurStreamState;		                          // ������ǰ״̬ 0-δ����, 1-����
	TIPC_CON_TYPE	tSupportedConnType;	                              // ֧�ֵ�������������
	unsigned char	byConnectionType;		                          // ������������(IPCSDK_CONNECTION_UNICAST��)
	unsigned long	dwStreamIpAddr;			                          // ���������ַ
	unsigned short	wStreamPort;			                          // ��������˿�
} TIPC_COMMON_NET_PARAM, *PTIPC_COMMON_NET_PARAM;

// ������ͳ����Ϣ
typedef struct tagIPCDecStatics
{
	unsigned char	m_byVideoFrameRate;                               // ��Ƶ����֡��
	unsigned short	m_wVideoBitRate;                                  // ��Ƶ��������
	unsigned short	m_wAudioBitRate;                                  // ������������
	unsigned long   m_dwVideoRecvFrame;                               // �յ�����Ƶ֡��
	unsigned long   m_dwAudioRecvFrame;                               // �յ�������֡��
	unsigned long   m_dwVideoLoseFrame;                               // ��ʧ����Ƶ֡��
	unsigned long   m_dwAudioLoseFrame;                               // ��ʧ������֡��
    unsigned short	m_wVideoLoseRatio;                                // ��Ƶ��ʧ��,��λ��% 	
	unsigned short	m_wAudioLoseRatio;                                // ��Ƶ��ʧ��,��λ��% 	
	unsigned long   m_dwPackError;                                    // ��֡��
	unsigned long   m_dwIndexLose;                                    // ��Ŷ�֡
	unsigned long   m_dwSizeLose;                                     // ��С��֡
	unsigned long   m_dwFullLose;                                     // ����֡
	unsigned long   m_wAvrVideoBitRate;                               // 1��������Ƶ����ƽ������
	unsigned long	m_wAvrAudioBitRate;                               // 1��������������ƽ������
	int	            m_bVidCompellingIFrm;                             // ��Ƶǿ������I֡
	unsigned long   m_dwDecdWidth;	                                  // �����Ŀ�
	unsigned long   m_dwDecdHeight;                                   // �����ĸ�
} TIPC_DEC_STATICS, *PTIPC_DEC_STATICS;

// ��Ƶ֡ͷ�ṹ
typedef struct tagIPCFrameHdr
{
    unsigned char    byMediaType;                                     // ý������
    unsigned char    *pData;                                          // ���ݻ���
    unsigned long    dwPreBufSize;                                    // pData����ǰԤ���˶��ٿռ䣬���ڼ�
	                                                                  // RTP option��ʱ��ƫ��ָ��һ��Ϊ12+4+12
	                                                                  // (FIXED HEADER + Extence option + Extence bit)
    unsigned long    dwDataSize;                                      // pDataָ���ʵ�ʻ����С�����С
    unsigned char    byFrameRate;                                     // ����֡��,���ڽ��ն�
    unsigned long    dwFrameID;                                       // ֡��ʶ�����ڽ��ն�
    unsigned long    dwTimeStamp;                                     // ʱ���, ���ڽ��ն�
    unsigned long    dwSSRC;                                          // ͬ��Դ, ���ڽ��ն�
    union
    {
        struct{
			int               bKeyFrame;                              // Ƶ֡����(I֡ or P֡)
			unsigned short    wVideoWidth;                            // ��Ƶ֡��
			unsigned short    wVideoHeight;                           // ��Ƶ֡��
		}tVideoParam;
        unsigned char    byAudioMode;                                 // ��Ƶģʽ
    };
} TIPC_FRAMEHDR,*PTIPC_FRAMEHDR;

// ��Ƶ֡�ص�
typedef void (*IPCSDK_FRAME_CB)(unsigned long hHandle, unsigned long hPlayHandle, PTIPC_FRAMEHDR pFrmHdr, unsigned long dwParam);

// ����ת��֡���ݻص�
typedef void (*IPCSDK_DATA_SWITCH_FRM_CB)(unsigned long hHandle, unsigned char byChanId, PTIPC_FRAMEHDR *pFrmHdr, void* pContext);

// YUV����������Ϣ�ص�
// �����ֱ�Ϊλͼ����ָ�롢λͼ���ߡ�λ��
typedef void (*IPCSDK_FRAMEINFO_CB)(unsigned long hHandle, unsigned long hPlayHandle, unsigned char *pBitmapBuf, 
									unsigned long dwWidth, unsigned long dwHeight, unsigned long dwbitcount, unsigned long dwParam);

/*************************************************
/	                 ���кͼ���
/*************************************************/
// ��չ�ն���������(������)
typedef struct tagIPCMediaCtrlEX
{
	unsigned char	  byEntityID;		                              // ͨ��ID
	unsigned char     byMediaCtrlType;                                // Video--IPCSDK_VIDEO_TYPE_XXX, Audio--IPCSDK_AUDIO_TYPE_XXX
	unsigned char     byStreamType;	                                  // ������������IPCSDK_MEDIA_STREAM_TYPE_UDP, IPCSDK_MEDIA_STREAM_TYPE_TCP
	unsigned short    wLocalAcceptStreamPort;                         // �����������ն˿�
	unsigned long  	  dwLocalAcceptStreamIpAddr;                      // �����������յ�ַ
   	unsigned short	  wRTCPStreamPort;                                // �Զ˵�RTCP�˿�
	unsigned long	  dwRTCPStreamIpAddr;	                          // �Զ˵�RTCP�˿ڵ�ַ
} TIPC_MEDIA_CTRL_EX, *PTIPC_MEDIA_CTRL_EX;

/*************************************************
/	              ͼ��ץ�ĺ�����
/*************************************************/
// ͼƬ��ѯ�ṹ
typedef struct tagIPCQueryPicFileParam39
{
	unsigned char	      byNum;									  // �ļ�����
	unsigned char	      byType;									  // ��ѯ����
	unsigned char	      byChanId;								      // ͨ����
	unsigned char	      byEvent;								      // �¼�����
	unsigned short	      wBeginNum;								  // �ļ���ѯ��ʼ���
	TIPC_SYS_TIME         tTimeStart;						          // ��ʼץ��ʱ��
	TIPC_SYS_TIME         tTimeEnd;							          // ����ץ��ʱ��
} TIPC_QUERY_PIC_FILE_PARAM, *PTIPC_QUERY_PIC_FILE_PARAM;

// ��ѯ�ļ�����
typedef struct tagIPCPicFileInfo
{
	unsigned char	byType;									          // �¼�����(IPCSDK_QUIEY_ALL_EVENT��)
	unsigned char	bychanId;								          // ͨ����
	unsigned char	byVideoResolution;						          // ͼ��ֱ���
	unsigned char	byFormat;								          // ͼƬ��ʽ
	TIPC_SYS_TIME   tTime;							                  // ��ͼʱ��
	unsigned long	dwPicID;								          // ͼƬ���
	char	        achFileName[MAX_FILE_NAME_LENGTH+1];	          // �ļ�����
} TIPC_PIC_FILE_INFO, *PTIPC_PIC_FILE_INFO;

typedef struct tagIPCPicFileAck
{
	unsigned char byType;			                                  // �¼�����(IPCSDK_QUIEY_ALL_EVENT��)
	unsigned char bychanId;		                                      // ͨ����
	unsigned char byVideoResolution;                                  // ͼ��ֱ���
	unsigned char byFormat;		                                      // ͼƬ��ʽ
	unsigned long dwTime;			                                  // ��ͼʱ��
	unsigned long dwPicID;		                                      // ͼƬ���
	unsigned long dwSize;			                                  // �ļ���С
} TIPC_PIC_FILE_ACK, *PTIPC_PIC_FILE_ACK;

typedef struct tagIPCPicFileAckEx
{
	unsigned char byType;			                                  // �¼�����(IPCSDK_QUIEY_ALL_EVENT��)
	unsigned char bychanId;		                                      // ͨ����
	unsigned char byVideoResolution;                                  // ͼ��ֱ���
	unsigned char byFormat;		                                      // ͼƬ��ʽ
	TIPC_SYS_TIME tTime;	                                          // ��ͼʱ��
	unsigned long dwPicID;		                                      // ͼƬ���
	char          achFileName[MAX_FILE_NAME_LENGTH+1];	              // �ļ�����
} TIPC_PIC_FILE_ACK_EX, *PTIPC_PIC_FILE_ACK_EX;

// ͼƬ��ѯ���
typedef struct tagIPCQueryPicFileInfo
{
    unsigned short wMaxNum;                                           // ��ǰ��ѯ�ļ�������
    unsigned short wIndex;                                            // ��ǰ��������,��1��ʼ
    unsigned char byNum;                                              // �ļ�����
    unsigned char byEnd;                                              // �Ƿ����
    TIPC_PIC_FILE_ACK aPicFileInfo[MAX_PIC_FILE_NUM];                 // ͼƬ�ļ��б�
} TIPC_QUERY_PIC_FILE_INFO, *PTIPC_QUERY_PIC_FILE_INFO;

typedef struct tagIPCDownloadPicFileInfo
{
	unsigned short		wResult;			                          // �������
	unsigned short		wTotalPacket;		                          // �ܰ�����
	unsigned short		wCurPacketIdx;	                              // ��ǰ����������,��1��ʼ
	unsigned short		wCurLen;			                          // ��ǰ����
	unsigned long		dwPicID;			                          // ͼƬ���
	unsigned char       abyBuf[SM_DATA_BUFSIZE];                      // ����
} TIPC_DOWN_PIC_FILE_INFO, *PTIPC_DOWN_PIC_FILE_INFO;

// ǰ��ͼƬ�������
typedef struct tagIPCDownPuPic
{
	unsigned long 		dwCurPkgNum;			                      // ��ǰ������
	unsigned char		*pPicFileBuf;			                      // ͼƬ�ļ�������
	unsigned long		dwBufRecvedBytes;		                      // ���յ��Ĵ�С
	int					nDownloadPicRet;		                      // -1:����ʧ�� 0:��δ���� 1:���سɹ�
	TIPC_PIC_FILE_ACK_EX tPicFileInfo;			                      // �����ص�ͼƬ�ļ���Ϣ
	char				szSavePath[MAX_PATH_LENGTH];                  // ����·��
} TIPC_DOWN_PU_PIC, *PTIPC_DOWN_PU_PIC;

/*************************************************
/	               ǰ��¼�����
/*************************************************/
// ¼���ļ���ѯ�ṹ
typedef struct tagIPCQueryParam
{
	unsigned char                byChanId;
	TIPC_REC_START_END_TIME_INFO RStartEndTimeInfo;
	unsigned char                byType;
	unsigned long                dwFileIndex;
	unsigned short               wNum;
} TIPC_QUERY_PARAM, *PTIPC_QUERY_PARAM;

// ¼���ļ���Ϣ
typedef struct tagIPCRecFileInfo
{
	unsigned char   byChanID;			                              // ͨ��ID
	unsigned long   dwStartTime;		                              // ��ʼʱ��
	unsigned long   dwEndTime;		                                  // ����ʱ��
	unsigned long   dwRecSize;		                                  // �ļ���С
	unsigned char   byRecFileType;		                              // ¼���ļ�����(EPuRecQueryType����)
	unsigned long   dwFileID;
} TIPC_REC_FILE_INFO, *PTIPC_REC_FILE_INFO;

// ¼���ļ�/�¼���Ϣ
typedef struct tagIPCRecFileInfoEx
{
	unsigned long	dwRecIndexId;							          // �ļ��¼�id��
	unsigned char	byRecFileType;                                    // ¼���ļ�/�¼�����(EPuRecQueryType����)
	TIPC_REC_START_END_TIME_INFO RecSETimeSetting;                    // ��ʼֹͣ¼��ʱ��
	unsigned char	abyFileName[MAX_FILE_NAME_LENGTH+1];              // �ļ�����
	unsigned char	abyFilePath[MAX_FILE_PATH_LENGTH+1];              // ����ļ�·������
} TIPC_REC_FILE_INFO_EX, *PTIPC_REC_FILE_INFO_EX;

// ��ѯ�ļ���Ϣ��
typedef struct tagIPCQueryRecFileInfo
{
	unsigned short wNum;                                              // ¼���ļ���Ŀ
	unsigned char  byType;                                            // ¼���ļ�/�¼�����
	unsigned char  bIsFinished;                                       // �Ƿ������е�¼���ļ�,1Ϊ������0Ϊδ����
	TIPC_REC_FILE_INFO_EX aRFileInfo[MAX_REC_FILE_NUM];               // ¼���ļ���Ϣ
	unsigned short wTransactionNum;                                   // �ļ�������ˮ��
} TIPC_QUERY_REC_INFO, *PTIPC_QUERY_REC_INFO;

// ���Ȳ�����Ϣ
typedef struct tagIPCProgressInfo
{
	unsigned char  abyCreateTime[MAX_TM_PRECISION];                   // ¼���ļ�����ʱ��,��������
	unsigned long  dwTotalFileTime;                                   // �ļ���ʱ��
	unsigned long  dwCurrentPlayTime;                                 // ��ǰ����ʱ��
	unsigned char  byPlayerId;                                        // ������id
	unsigned char  byIsFinished;                                      // �Ƿ񲥷Ž���
	unsigned short wTransactionNum;                                   // ������ˮ��
} TIPC_PROGRESS_INFO, *PTIPC_PROGRESS_INFO;

/*************************************************
/	               ¼���ļ��ط�
/*************************************************/
// �ļ����������
typedef struct tagIPCPlayFileParam
{
    unsigned long        dwGranuity;
    unsigned long        dwContext;
    IPCSDK_FILE_STAT_CB  pfCB;
    unsigned char        byFileType;
    char                 achFileName[512];
} TIPC_PLAY_FILE_PARAM, *PTIPC_PLAY_FILE_PARAM;

// �ط��ļ�����Ϣ
typedef struct tagIPCPlayFileInfo
{
    unsigned __int64   qwTotalSize;
    unsigned __int64   qwCreateDate;
    unsigned long      dwTotalTime;
} TIPC_PLAY_FILE_INFO, *PTIPC_PLAY_FILE_INFO;

// �ط��ļ�������Ϣ
typedef struct tagIPCPlayFileStreamInfo
{
    unsigned long          dwAudio;                                   // �Ƿ�����Ƶ��
    unsigned short         wWidth;                                    // ͼ����
    unsigned short         wHeight;                                   // ͼ��߶�
    unsigned char          byStreamId;                                // �������
    unsigned char          byMediaType;                               // �����ʽ
} TIPC_PLAY_FILE_STREAM_INFO, *PTIPC_PLAY_FILE_STREAM_INFO;

// �ط��ļ���״̬
typedef struct tagIPCPlayFileStatus
{
    unsigned long                      dwCurrentPlayPos;
    unsigned char                      byPlayMode;
    unsigned char                      byPlayRate;
    unsigned char                      byPlayState;
    unsigned char                      byStreamNum;
    unsigned long                      dwVFps;
    unsigned long                      dwAFps;
    TIPC_PLAY_FILE_STREAM_INFO atFileStreamInfo[MAX_FILE_STREAM_NUM];
} TIPC_PLAY_FILE_STATUS, *PTIPC_PLAY_FILE_STATUS;

/*************************************************
/	             ͸��ͨ����չ����
/*************************************************/
// �Զ��������(���ֵ)
typedef struct tagIPCAgcInfo
{
	unsigned long dwAgcMaxHigh;                                       // �Զ��������ֵ����
	unsigned long dwAgcMaxLow;                                        // �Զ��������ֵ���� 
	unsigned long dwAgcMaxCurVal;                                     // �Զ��������ֵ��ǰֵ 
} TIPC_AGC_INFO, *PTIPC_AGC_INFO;

// �Զ����Ų���(��ֵ)
typedef struct tagIPCAutoShutterInfo
{
	unsigned long dwAutoShutterMax;                                   // �Զ����ŷ�Χ���ֵ
	unsigned long dwAutoShutterMin;                                   // �Զ����ŷ�Χ��Сֵ 
	unsigned long dwAutoShutterCurMaxVal;                             // �Զ����ŵ�ǰ���趨�����ֵ
	unsigned long dwAutoShutterCurMinVal;                             // �Զ����ŵ�ǰ���趨����Сֵ
} TIPC_AUTOSHUT_INFO, *PTIPC_AUTOSHUT_INFO;

// ���ػ��Բ���
typedef struct tagIPCBncInfo
{
	int bSupportBNC;                                                  // �Ƿ�֧�ֱ��ػ���
	int nBNCStatus;                                                   // ���ػ���״̬ 0-�ر� 1-����
} TIPC_BNC_INFO, *PTIPC_BNC_INFO;

/*************************************************
/	                  ǰ������
/*************************************************/
#ifdef WIN32
#pragma pack(push, 1)
#endif
// ������ַ��Ϣ
typedef struct tagIPCAddrInfo
{
	unsigned long   dwIp;				                              // ip��ַ(������)
	unsigned short	wPort;				                              // �˿ں�
} TIPC_ADDR_INFO,*PTIPC_ADDR_INFO;

// �û���Ϣ
typedef struct tagIPCUserInfo
{
	char	achName[ MAX_USER_NUMBER + 1 ];		                      // �û���
	char	achPassWord[ MAX_USER_NUMBER + 1 ];	                      // �û�����
} TIPC_USER_INFO,*PTIPC_USER_INFO;

// �汾����
typedef struct tagIPCVerUpdateInfo
{
	unsigned char	 byFileType;									  // ���������ļ�����
	char			 achFileInfo[MAX_STR_LEN_64+1];			          // ���������ļ�·�����ļ���
	TIPC_ADDR_INFO   tFtpAddr;									      // FTP��������ַ���˿�
	TIPC_USER_INFO	 tUserInfo;								          // �û���¼��Ϣ
	    
} TIPC_VER_UPD_INFO, *PTIPC_VER_UPD_INFO;

/*************************************************
/	               ϵͳ��Ϣ(ȫ)
/*************************************************/
// FTP����
typedef struct tagIPCFtpSvrCfg
{
	unsigned long    dwFtpSvrIpAddr;                                  // FTP��������ַ
	unsigned short   wFtpSvrPort;                                     // FTP�������˿�
	char             achFtpSvrPath[MAX_STR_LEN_64 + 1];               // FTP���������·��
	TIPC_USER_INFO   tFtpUserInfo;                                    // FTP��½�û���Ϣ
} TIPC_FTP_SVR_CFG, *PTIPC_FTP_SVR_CFG;

// �ļ��洢����
typedef struct tagIPCFileStoreCfg
{	
	int		         bIsRemoteStore;							      // �Ƿ�Զ�˴洢
	unsigned long	 dwNfsSvrIpAddr;								  // NFS��������ַ
	char			 achSvrSharePath[MAX_STR_LEN_64 + 1];	          // ����������·��	
	int              bIsFtpUpLoad;                                    // �Ƿ�FTP�洢
	TIPC_FTP_SVR_CFG tPuFtpCfg;                                       // FTP����	
	unsigned char 	 abyUsedDiskPartionId[ MAX_DISK_NUM ];            // �û�ʹ�õķ���·���б�(����Ӳ��ID)
	unsigned char 	 byUsedDiskPartionhNum;						      // �û�ʹ�õķ�������	
} TIPC_FILE_STORE_CFG, *PTIPC_FILE_STORE_CFG;

// ��������
typedef struct tagIPCNamingRule
{
	unsigned char abyNameOrder[MAX_NAMERULE_NUM];		              // �ֶ�˳�򣬴�0��ʼ�����α�ʾ��i+1���ֶ�
	unsigned char byNameNum;								          // ʵ��ʹ�õ��ֶθ���
	char achDateSeparator[MAX_SEPA_LEN];		                      // ����֮��ķָ���(������)(��Ч�ɴ�ӡ�ַ�)
	char achTimeSeparator[MAX_SEPA_LEN];		                      // ʱ��֮��ķָ���(ʱ����)(��Ч�ɴ�ӡ�ַ�)
	char achNameSeparator[MAX_SEPA_LEN];		                      // �����ֶ�֮��ķָ���(��Ч�ɴ�ӡ�ַ�)
	char achUserDefine[MAX_STR_LEN_64 + 1];		                      // �û��Զ����ֶ�����	
} TIPC_NAMING_RULE, *PTIPC_NAMING_RULE;

// ץ������
typedef struct tagIPCSnapShotCfg
{
	unsigned short		    wSnapInterval;			                  // ͼ��ץ��ʱ����(��λ:��,Ĭ��5��,��Χ:5~300s)
	unsigned char			byOneAlmPicMaxNum;		                  // ���θ澯ץ��ͼƬ����(��λ:��,Ĭ��5��,��Χ:5~50��)
	unsigned long			dwUseSize;				                  // ץ��ʹ�ÿռ��С(��λ:MB,Ĭ��50MB(50M~Ӳ�̷������ռ�))
	TIPC_NAMING_RULE		tNamingRule;			                  // ץ��ͼƬ��������
	TIPC_FILE_STORE_CFG		tFileStoreCfg;			                  // �洢����	
} TIPC_SNAPSHOT_CFG, *PTIPC_SNAPSHOT_CFG;

// ������Ϣ
typedef struct tagIPCDiskPartionInfo
{
	int                     bFormated;								  // �Ƿ��ʽ��
	int                     bValid;								      // �Ƿ���Ч
	unsigned long           dwMBSize;								  // ������С
	unsigned long           dwFreeMBSize;							  // ʣ��ռ�
	char                    achName[ MAX_STR_LEN_64 + 1 ];		      // ����mount�������
	char                    achPhyName[ MAX_STR_LEN_64 + 1 ];         // ��������������
	unsigned long           dwLogicId;					              // �߼����				
} TIPC_DISK_PARTION_INFO, *PTIPC_DISK_PARTION_INFO;

// socks5�ų���ַ��Ϣ
typedef struct tagIPCFilterAddr
{
	unsigned long		    dwIpAddr;		                          // ip��ַ
	unsigned long		    dwMask;			                          // ��������	
} TIPC_FILTER_ADDR, *PTIPC_FILTER_ADDR;

// ϵͳ����(ȫ��)
typedef struct tagPuSysCfg
{
	unsigned char				byStackType;						  // Э������(Ĭ��:IPC_STACK_VSIP)
	TIPC_ADDR_INFO		        tDnsServer;							  // DNS��������ַ,0��ʾ������
	unsigned long 			    dwRegPlatformIp;					  // ע���ƽ̨IP(Ĭ��:IPCSDK_DEFCFG_REGPFIP)
	unsigned short				wRegPlatformPort;					  // ע���ƽ̨PORT(Ĭ��:IPCSDK_DEFCFG_REGPFPORT)
	unsigned long				dwVsipMultiIpAddr;					  // �������鲥��ַ(Ĭ��:IPCSDK_DEFCFG_VSIPMULIP)
	unsigned short				wVsipPort;							  // �����Ķ˿ں�
	unsigned char				byLinkChkInterval;					  // ��·�����(��λ:��,Ĭ��:10��(0~60s)
	unsigned char				byLinkChkDisNum;					  // ��·�������Դ���(��λ:��,Ĭ��:3��(1~255��)
	
	int			                bNatPacketSend;						  // �Ƿ���NAT̽���(Ĭ��:��)
	unsigned char				byNatSndInterval;					  // NAT̽������(Ĭ��:5)
	unsigned char				byCurStrSendNum;					  // ��ǰ����ͨ���������·��(Ĭ��:5)
	unsigned char				byCurDirSendNum;					  // ��ǰ����ͨ�������ֱ��·��(Ĭ��:0·)
		
	unsigned char				abyVidEncCapSet[MAX_CHANNEL_NUM];	  // ֧�ֵ���Ƶ��������(ֻ��)
	unsigned char				byVidEncCapSetNum;					  // ֧�ֵı�����������(ֻ��)
	unsigned char				byMaxVidEncNum;						  // ��ǰ�����Ƶ��������(Ĭ��ͬ��ǰ�豸��С����)
	int			                bIsVidCompOutput;					  // �Ƿ���ϳ����(��������)
	unsigned char               byTimeSynMode;                        // ʱ��ͬ����ʽ
	TIPC_ADDR_INFO		        tSynTimeServer;						  // ʱ��ͬ��������,0��ʾ��ͬ��
	unsigned long				dwSynTimeInt;						  // ͬ��ʱ����(Ĭ��:IPCSDK_DEFCFG_SYNTIMEINT(1~72*60����)
	TIPC_USER_INFO		        tAdminUserInfo;						  // ����Ա��Ϣ(Ĭ��:admin,admin)
	TIPC_USER_INFO		        tNormalUserInfo;					  // ��ͨ�û���Ϣ(Ĭ��:user,user)
	int			                bGuardEnable;						  // ϵͳ����ʹ��(�ڲ�ʹ��,Ĭ��:����)
	TIPC_SNAPSHOT_CFG	        tSnapShotCfg;						  // ͼ��ץ������
	
	// ϵͳ������Ϣ
	TIPC_DISK_PARTION_INFO 	    atDiskPartionInfo[MAX_DISK_NUM];	  // ϵͳ�����б�(ֻ��)
	unsigned char				byDiskPartionNum;					  // ϵͳ��������(ֻ��)
	
	TIPC_ADDR_INFO		        tSocks5Agent;						  // SOCKS5 ���������,0��ʾ��ʹ��
	TIPC_USER_INFO		        tSocks5AuthInfo;					  // SOCKS5��֤��Ϣ(Ĭ��:Ĭ��:admin,admin)
	TIPC_FILTER_ADDR	        atSocks5Filter[MAX_FILTERADDR_NUM];	  // SOCKS5���˵�ַ�б�(Ĭ��:��)
	
	unsigned short              wAlarmResumeTime;                     // Ϊ�˼���3.9�����澯�ָ�ʱ���ֶΣ�ֻ��vsipstack��
	int                         bUseIndiLight;                        // �Ƿ���ָʾ��
	unsigned char				byChangeMask;						  // �Ƿ�ı�mask,Ŀǰ���������ṹ�Ƿ��޸�(����ģ��)
	unsigned char               byUseDomain;                          // �Ƿ�ʹ������
	char                        achPlatDomain[MAX_STR_LEN_64+1];      // ƽ̨����
	int                         bSupportBuildNet;                     // �Ƿ�֧������ TURE:֧��
} TIPC_SYS_CFG_PARAM, *PTIPC_SYS_CFG_PARAM;

/*************************************************
/	               SD����ز���
/*************************************************/
// ǰ��SD��������Ϣ
typedef struct tagIPCSDCardInfo
{
	unsigned char bySdCardExist;                                      // sd���Ƿ���ڣ�1:���ڣ�0:������
	unsigned long dwTotalMBSize;                                      // ������
	unsigned long dwTotalRecMBSize;                                   // �ܵ�¼��ռ�
	unsigned long dwFreeRecMBSize;                                    // ʣ��¼��ռ�
} TIPC_SD_CARD_INFO, *PTIPC_SD_CARD_INFO;

// SD����ʽ������
typedef struct tagIPCFormatSDCard
{
	int           bFormatted;                                         // �Ƿ��Ѿ���ʽ��
} TIPC_FORMAT_SD_CARD, *PTIPC_FORMAT_SD_CARD;
#ifdef WIN32
#pragma pack(pop)
#endif

#endif
