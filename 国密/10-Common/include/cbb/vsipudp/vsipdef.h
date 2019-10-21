#include "common.h"

#define     VSIP_CHECK_VERSION              0x0100
#define     VSIP_V50_VERSIONID              2


typedef enum
{
    VSIP_MSG_TYPE_D_PING_REQ                       = 0x01,
    VSIP_MSG_TYPE_D_PING_RESP                      = 0x02,
    VSIP_MSG_TYPE_MC_GETCONFIG_REQ                 = 0x08,
    VSIP_MSG_TYPE_MC_GETCONFIG_RESP                = 0x09,
    VSIP_MSG_TYPE_ER_ERROR                         = 0x0D,
    VSIP_MSG_TYPE_MC_SETPROPCONFIG_REQ             = 0x0E,
    VSIP_MSG_TYPE_MC_GETPROPCONFIG_REQ             = 0x0F,
    VSIP_MSG_TYPE_MC_GETPROPCONFIG_RESP            = 0x10,

    VSIP_MSG_SET_IP_INFO_REQ                       = 0x69,

// expend
    VSIP_MSG_TYPE_MC_GETCONFIG_REQ__SYSINFO        = (0x01<<8)+ VSIP_MSG_TYPE_MC_GETCONFIG_REQ,
    VSIP_MSG_TYPE_MC_GETCONFIG_RESP__SYSINFO       = (0x01<<8)+ VSIP_MSG_TYPE_MC_GETCONFIG_RESP,
                
} EVsipMsgType;

// *********************   types and sizes of item values      *********************//
#define VSIP_VALUE_TYPE_CHAR                           0x01 //1 byte 
#define VSIP_VALUE_TYPE_SHORT                          0x02 //2 bytes 
#define VSIP_VALUE_TYPE_INT                            0x03 //4 bytes 
#define VSIP_VALUE_TYPE_STRING                         0x04 //The first two bytes are for the string size. The remaining bytes are for the string itself. 
#define VSIP_VALUE_TYPE_UINT                           0x05 //4 bytes 
#define VSIP_VALUE_TYPE_GUID                           0x06 //16 bytes

/************************************************************************/
/* CONFIG -> System item                                                */
/************************************************************************/
typedef enum
{
    CONFIG_SYSTEM_REBOOTREQUIRED                   = 0x5001,
    CONFIG_SYSTEM_DEVICE_NAME                      = 0x5002,
    CONFIG_SYSTEM_TYPE_NAME                        = 0x5003,
    CONFIG_SYSTEM_DEVICE_TYPE                      = 0x50FE,
    CONFIG_SYSTEM_DEVICE_TIME                      = 0x50FF,
    CONFIG_SYSTEM_SERIES_NAME                      = 0x50FD,

    // V5 add start, used in pingack
    CONFIG_SYSTEM_NETMASK                          = 0x500A,
    CONFIG_SYSTEM_GATEWAY                          = 0x500B,
    // V5 add end
                
} EVsipConfig_SystemItem;




typedef struct
{
    u16 m_wVersion;
    u16 m_wTransactionNum;
    u8  m_bAck;
    u8  m_byMsgType;
    u16 m_wMsgLen;
    u16 m_wReserved;    //for align
        
} TVsipHeader_P;


typedef struct
{
    u32    m_dwData1;
    u16    m_wData2;
    u16    m_wData3;
    u8     m_abyData4[8];
} TVsipGuid_P;

// Discovery Ping Info typedef struct
typedef struct
{   
    u32  m_dwIpAddress;          // discovery ip 
    u16  m_wPort;                // discovery port
    u8   m_byConnectionType;     // connection type,refer to VSIP Connection Types 
    u8   m_byVerionID;           // Vsip Version ID
} TPingReq_P;

typedef struct
{   
    u32     m_dwIpAddress;          // discovery ip 
    u16     m_wPort;                // discovery port
    u8      m_byConnectionType;     // connection type,refer to VSIP Connection Types 
    TVsipGuid_P m_tGUID;
    u16     m_wManufacturerId;
    u16     m_wProductId;
    u8      m_byStatus;

    u8      m_byVerionID;           // Vsip Version ID
    s8      m_achName[64+1];         //设备名
    s8      m_achTypeName[64+1];     //设备类型名(只读)
    u8      m_byDevType;                         //设备类型(MTI_ENCODER等) 
    s8      m_achSeriesName[16+1];                 //设备系列名

    u32     m_dwNetMask;            //子网掩码
    u32     m_dwGateway;            //网关
} TPingAck_P;


typedef struct 
{
    u16     m_wPropConfigId;
    u16     m_wValueLen;
        
    u8      m_byChannelId;
    u8      m_byType;
    u16     m_wSectionId;
        
    s32     m_nSize;
} TPropConfigItem_P;


typedef struct
{
    s8    m_achName[64+1];           // 设备名
    s8    m_achTypeName[64+1];       // 设备类型名(只读)
    u8    m_byDevType;               // 设备类型(MTI_ENCODER等) 
    s8    m_achSeriesName[16+1];     // 设备系列名
} TSysParam_P;

typedef struct 
{
    u32         m_dwTargetIP;
    u32         m_dwNewIP;
    u32         m_dwNewMask;
    u32         m_dwNewGW;
    TVsipGuid_P m_tGUID;
} TSetIpInfo_P;


typedef struct 
{
    u8  m_byRequestType;
    u16 m_wErrorCode;
} TErrorResponse_P;
