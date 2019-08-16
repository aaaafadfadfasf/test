#ifndef PUSDKSTRUCT_H_
#define PUSDKSTRUCT_H_

#include "pusdkdefine.h"
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// 通用类结构定义

// 设备GUID结构
typedef struct
{
    unsigned long    m_dwData1;
    unsigned short   m_wData2;
    unsigned short   m_wData3;
    unsigned char    m_abyData4[8];
} TPuGuid;

// 设备信息结构
typedef struct
{
    TPuGuid        m_tGuid;                                 // 设备GUID值(只读)
    char           m_achName[PUSDK_MAX_STRING_LEN64+1];     // 设备名
    char           m_achTypeName[PUSDK_MAX_STRING_LEN64+1]; // 设备类型名(只读)
    char           m_achSeriesName[PUSDK_MAX_STRING_LEN16]; // 设备系列名
    unsigned char  m_byDevType;                             // 设备类型(EPuDeviceType)
    unsigned long  m_dwUnitIpAddr;                          // 设备IP地址
    unsigned short m_wVsipPort;                             // VSIP端口号
} TPuUnitInfo;

// 设备搜索回调（内外部唯一公共使用的定义，只能放在这里）
typedef int (*fPuSearch)(TPuUnitInfo *ptPuInfo, void *pContext);

// 搜索设备参数
typedef struct
{
    unsigned long  dwIpAddr;                // 搜索地址
    unsigned short wPort;                   // 搜索端口
} TPuAddressInfo;

//////////////////////////////////////////////////////////////////////////
// 设备状态
//////////////////////////////////////////////////////////////////////////

// 解码器统计信息
typedef struct
{
	unsigned long   m_dwDecSSRC;       /*SSRC100608yuleiadd*/
    unsigned char   m_byVideoFrameRate;//视频解码帧率
    unsigned short  m_wVideoBitRate;   //视频解码码率
    unsigned short  m_wAudioBitRate;   //语音解码码率
    unsigned long   m_dwVideoRecvFrame;//收到的视频帧数
    unsigned long   m_dwAudioRecvFrame;//收到的语音帧数
    unsigned long   m_dwVideoLoseFrame;//丢失的视频帧数
    unsigned long   m_dwAudioLoseFrame;//丢失的语音帧数
    unsigned short  m_wVideoLoseRatio;//视频丢失率,单位是%     
    unsigned short  m_wAudioLoseRatio;//音频丢失率,单位是%     
    unsigned long   m_dwPackError;     //乱帧数 
    unsigned long   m_dwIndexLose;//序号丢帧
    unsigned long   m_dwSizeLose; //大小丢帧
    unsigned long   m_dwFullLose; //满丢帧    
    unsigned long   m_wAvrVideoBitRate;   //1分钟内视频解码平均码率
    unsigned long   m_wAvrAudioBitRate;   //1分钟内语音解码平均码率
    int             m_bVidCompellingIFrm;  //视频强制请求I帧                                  
    unsigned long   m_dwDecdWidth;    //码流的宽
    unsigned long   m_dwDecdHeight;   //码流的高
} TPuDecStatis;

// 系统服务状态
typedef struct 
{
    unsigned char      byNatSend;              // 探测包服务   
    unsigned char      byTimerSyn;             // 时钟同步服务 
    unsigned char      bySocks5Agent;          // socks5代理   
    unsigned char      byDns;                  // DNS服务      
    unsigned char      byPppoe;                // pppoe服务    
    unsigned char      bySnmpAgent;            // 网管代理     
    unsigned char      bySysGuard;             // 系统守卫     
    unsigned char      byDhcp;                 // DHCP         
    unsigned char      byDdns;                 // DDNS         
    unsigned char      byWifi;                 // wifi         
    unsigned char      by3g;                   // 3G           
    unsigned char      byGps;                  // GPS          
    unsigned char      byPWRState;             // 电源状态 0-无 1-AC接入 2-充电    
    unsigned char      byBatteryState;         // 电池电量 0-无 1-低 2-正常 3-满 4-异常 
} TPuSysService;

// 视频编码通道状态    
typedef struct
{        
    unsigned char        bySendEnable;         // 是否发送   
    unsigned char        byEnFormat;           // 编码格式   
    unsigned char        byEnRes;              // 分辨率     
    unsigned short       wEnRtBitRate;         // 实时码率   
    unsigned short       wEnAvrBitRate;        // 平均码率   
    unsigned long       dwPackLose;           // 丢帧率     
    unsigned long       dwPackError;          // 错帧数         
} TPuVidEncState;

// 视频解码通道状态
typedef struct
{ 
    unsigned char        byRecvEnable;         // 是否接收   
    unsigned char        byDecFormat;          // 解码格式   
    unsigned char        byDecRes;             // 分辨率     
    unsigned short       wDecRtBitRate;        // 实时码率   
    unsigned short       wDecAvrBitRate;       // 平均码率   
    unsigned long       dwRcvFrmNum;          // 收到帧数   
    unsigned char        byLoseFrmRatio;       // 丢帧率     
} TPuVidDecState;

// 音频编码状态
typedef struct
{        
    unsigned char        bySendEnable;        // 是否发送 
    unsigned char        byEnFormat;          // 编码格式 
    unsigned char        byAecEnable;         // 是否回声抵消 
    unsigned char        byDumbEnable;        // 是否哑音 
    unsigned char        byEnVolume;          // 音量 
} TPuAudEncState;

// 音频解码状态
typedef struct
{
    unsigned char        byRecvEnable;        // 是否接收 
    unsigned char        byDecFormat;         // 解码格式 
    unsigned char        byMuteEnable;        // 是否静音 
    unsigned char        byDecVolume;         // 音量 
} TPuAudDecState;

// 录像状态
typedef struct 
{
    unsigned char  byIsManualRec;          // 是否在手动录像
    unsigned char  byIsMotiveAlarmRec;     // 是否在移动侦测告警录像
    unsigned char  byIsPinAlarmRec;        // 是否在并口告警录像
    unsigned char  byIsTimelyRec;          // 是否在定时录像
    unsigned char  byIsWeekRec;            // 是否在周期录像
    unsigned char  byIsPreRec;             // 是否在预录
} TPuRecState;

// 放像状态
typedef struct 
{
    unsigned char  byIsPlaying;             // 是否在放像
} TPuPlayState;

// 通道基本数据
typedef struct 
{
    unsigned char  byChanType;               // 通道类型 EPuChanType
    unsigned char  byChanId;                 // 通道编号
} TPuChanId;

// 通道状态查询请求数据
typedef struct 
{
    unsigned char  byChanNum;   // 待查询通道个数
    TPuChanId      atChanId[64]; // 待查询通道基本数据集合
} TPuChanStateReq;

// 通道状态数据
typedef struct 
{
    TPuChanId tChanId;           // 通道基本数据
    
    union
    {
        TPuAudEncState tAudEncState; // 音频编码状态
        TPuAudDecState tAudDecState; // 音频解码状态
        TPuVidEncState tVidEncState; // 视频编码状态
        TPuVidDecState tVidDecState; // 视频解码状态
        TPuRecState    tRecState;    // 录像状态
        TPuPlayState   tPlayState;   // 放像状态
    }u;
} TPuChanState;

// 通道状态查询应答数据
typedef struct 
{
    unsigned char    byChanNum;          // 应答通道个数
    TPuChanState     atChanState[64];    // 应答通道状态数据集合
} TPuChanStateAck;

// 前端设备系统信息
typedef struct 
{
    char   achDevType[PUSDK_MAX_STRING_LEN64 + 1];     // 设备类型
    char   achDevName[PUSDK_MAX_STRING_LEN64 + 1];     // 设备别名
    char   achDevIP  [PUSDK_MAX_STRING_LEN32 + 1];     // IP地址
    char   achDevMac [PUSDK_MAX_STRING_LEN32 + 1];     // MAC地址
    char   achSoftVer[PUSDK_MAX_STRING_LEN64 + 1];     // 软件版本
    char   achHardVer[PUSDK_MAX_STRING_LEN32 + 1];     // 硬件版本
    char   achConnStatus[PUSDK_MAX_STRING_LEN32 + 1];  // 连接状态
} TPuSysInfo;

// 磁盘分区信息
typedef struct 
{
    unsigned char  m_byDiskId;                              // 磁盘ID
    unsigned char  m_byPartId;                              // 分区ID
    unsigned char  m_byState;                               // 磁盘状态，EPuDiskState
    unsigned long  m_dwHealth;                              // 磁盘健康状态，EPuDiskHealth
    unsigned long  m_dwMBSize;                              // 分区大小
    unsigned long  m_dwFreeMBSize;                          // 剩余空间
    char           m_achName[PUSDK_MAX_FILEPATH_LEN + 1];   // 分区mount后的名称
    char           m_achPhyName[PUSDK_MAX_FILEPATH_LEN + 1];// 分区的物理名称
} TPuDiskPartion;

// 系统分区信息
typedef struct 
{
    unsigned char     m_byDiskPartionNum;                          // 系统分区个数(只读)
    TPuDiskPartion    m_atDiskPartionInfo[PUSDK_MAX_DISKPART_NUM]; // 系统分区列表(只读)
} TPuDiskPartionParam;



//! 格式化状态
typedef struct 
{
    unsigned char byCurState;        // 格式化状态(EPuFormatState) 
    unsigned char byErrorNum;        // 失败次数 
}TPuFormatState;

//! 磁盘信息
typedef struct
{
    char achDiskName[PUSDK_MAX_STRING_LEN32 +1];		// 磁盘名称 
    unsigned char byDiskType;   		                // 磁盘类型(EPuDiskType)
    unsigned char byDiskSubareaNum;					    // 磁盘包含的分区个数 
	unsigned long dwDiskSizeMB;                         // 磁盘容量大小(MB) 
    //注: 每个磁盘有几个分区,就在abySubareaType的前几个元素中设置分区类型,分区大小类同
    char abyFileSystemType[PUSDK_MAX_SUBEREA_NUM];   	// 文件系统类型(EPuFileSystemType)
    char abySubareaUseType[PUSDK_MAX_SUBEREA_NUM];		// 分区类型(EPuSubareaUseType) 
    char abySubareaSize[PUSDK_MAX_SUBEREA_NUM];		    //分区大小(百分比,如10%则为10,注意:是本块磁盘的百分比) 
} TPuDiskDetailInfo;

// 磁盘格式化信息(GET)
typedef struct 
{
    unsigned char       byErrorCode;                    // 0 —— 成功，非0 —— 失败
    unsigned char       byDiskNum;                      // 磁盘个数 
    TPuFormatState      tFormatState;                   // 磁盘格式化状态 
    TPuDiskDetailInfo	atDiskInfo[PUSDK_MAX_DISK_NUM]; // 磁盘信息 
} TPuFormatGetDiskInfo;

// 磁盘格式化信息(SET)
typedef struct 
{
    unsigned char     byDiskNum;                       // 磁盘个数 
    TPuDiskDetailInfo atDiskInfo[PUSDK_MAX_DISK_NUM];  // 磁盘信息 
} TPuFormatSetDiskInfo;

typedef struct
{
    unsigned char		byErrCode;		// 错误码,注:当错误码为noerr时才使用格式化进度 
    unsigned char		byMkfsPercent;	// 当前磁盘格式化进度(百分比,如10%则为10) 
} TPuDiskFormatProgressInfo;

// 并口输出状态
typedef struct 
{
    unsigned char      byUsedNum;                  // 使用数，即当前OutputPin个数
    unsigned char      abyState[64];               // 状态 EEventType
} TPuOutputPinState;

// 并口输入状态
typedef struct 
{
    unsigned char      byUsedNum;     // 使用数，即当前InputPin个数
    unsigned char      abyState[64];  // 状态 EEventType
} TPuInputPinState;

//////////////////////////////////////////////////////////////////////////
// 录像信息操作结构

//系统时间参数结构
typedef struct
{
    unsigned short   wYear;    
    unsigned char    byMonth;   //月 1-12
    unsigned char    byDay;     //日 1-31    
    unsigned char    byHour;    //时 0-23    
    unsigned char    byMinute;  //分 0-59
    unsigned char    bySecond;  //秒 0-59
} TPuSysTime;

// 查询录像文件数目请求
typedef struct
{
    unsigned char  byChannelId;
    unsigned char  byType;          // EPuRecQueryType
    TPuSysTime     tStartTime;
    TPuSysTime     tEndTime;
} TPuRecQueryNumReq;

// 查询录像文件数目应答
typedef struct
{
    unsigned short         wNum;
} TPuRecQueryNumAck;

// 播放前端录像
typedef struct
{
    unsigned long  dwIndexId;     // 录像文件索引号
    unsigned char  byType;        // EPuRecQueryType 录像类型
    TPuSysTime     tStartTime;    // 开始时间
    TPuSysTime     tEndTime;      // 结束时间
    unsigned short wIdentifier;   // 录像标识（不使用）    
} TPuPlayStartReq;

typedef struct 
{
    unsigned char     byPuPlayerId;                             // 放像器id
    unsigned long     dwCreateTime;                           // 录象文件创建时间(s)
    unsigned long     dwTotalFileTime;                        // 文件总时间
    unsigned long     dwCurrentPlayTime;                      // 当前播放时间
    unsigned char     byIsFinished;                           // 是否播放结束
    unsigned short    wTransactionNum;                        // 操作流水号（和播放请求中的一致）
} TPuPlayProgress;

// 查询录像文件信息请求
typedef struct
{
    unsigned char     byChannelId;     // 通道号
    unsigned char     byType;          // 前端录像类型 EPuRecQueryType
    TPuSysTime        tStartTime;      // 起始时间
    TPuSysTime        tEndTime;        // 结束时间
    unsigned long     dwIndex;         // 录像文件索引号
    unsigned short    wNum;            // 当前查询数目 (1-20,填0的时候不返回数据)
    unsigned char     byQueryMode;     // 查询模式 EPuRecQueryFileMode
} TPuRecQueryFileReq;

// 单个录像文件信息
typedef struct 
{
    unsigned long   dwRecIndexId;      // 文件事件id号
    unsigned char   byRecFileType;     // 录象文件/事件类型 EPuRecQueryType
    TPuSysTime      tStartTime;        // 起始时间
    TPuSysTime      tEndTime;          // 结束时间
    unsigned char   abyFileName[65];   // 文件别名
    unsigned char   abyFilePath[65];   // 最大文件路径长度
    
} TPuRecFileInfo;

// 查询录像文件信息应答
typedef struct
{
    unsigned short     wNum;               // 录象文件数目
    unsigned char      byType;             // 录象文件/事件类型 EPuRecQueryType
    unsigned char      byIsFinished;       // 是否是所有的录象文件,1为结束，0为未结束
    TPuRecFileInfo     atFileInfo[20];     // 录象文件信息
    unsigned short     wTransactionNum;    // 文件操作流水号
} TPuRecQueryFileAck;

//放像操作请求
typedef struct 
{
    EPuRecPlayType      byOperationType;              // EPuRecPlayType
} TPuRecPlayOperation;

// 删除前端录像文件
typedef struct 
{
    unsigned char     byType;                       // 文件类型 EPuRecQueryType
    unsigned long     dwRecFileIndexId;             // 文件ID
} TPuRecFileDelReq;

// 抓拍图片文件查询请求
typedef struct 
{
    unsigned char   byNum;            // 文件数量
    unsigned char   byType;           // 查询类型 EPuPicFileQueryType
    unsigned char   byChannelId;      // 通道号
    unsigned char   byEvent;          // 事件类型 EPuRecQueryType
    unsigned short  wBeginNum;        // 文件查询起始序号
    TPuSysTime      tTimeStart;       // 起始抓拍时间
    TPuSysTime      tTimeEnd;         // 结束抓拍时间
} TPuPicFileQueryReq;

// 抓拍文件信息
typedef struct 
{
    unsigned char    byType;                // 事件类型
    unsigned char    byChannelId;           // 通道号
    unsigned char    byVideoResolution;     // 图像分辨率 EPuVideoRes
    unsigned char    byFormat;              // 图片格式
    TPuSysTime       tTime;                 // 截图时间
    unsigned long    dwPicID;               // 图片编号
    char             achFileName[PUSDK_MAX_STRING_LEN64+1];    // 文件别名
} TPuPicFileInfo;

// 抓拍图片文件查询应答
typedef struct 
{
    unsigned short   wMaxNum;            // 当前查询文件总数量
    int              nBeginNum;          // 文件查询起始序号, < 0 Invalid, V5 
    unsigned char    byIndex;            // 当前包索引号,从1开始
    unsigned char    byNum;              // 文件数量
    unsigned char    byEnd;              // 是否结束
    TPuPicFileInfo   aPicFileInfo[20];   // 图片文件列表
    unsigned short   wTransactionNum;    // 操作流水号
} TPuPicFileQueryAck;

// 抓拍文件下载
typedef struct 
{
    //unsigned short wIndex;                // 默认1
    TPuPicFileInfo tPicFileInfo;
} TPuPicFileDownloadReq;

// 图片下载结果
typedef struct
{
    unsigned long dwPicID;    // 图片编号
    int           res;        // 操作结果
    void*         pPicDate;   // 图片数据
    int           nLenDate;   // 图片数据长度
} TPuPicFileDownloadRes;

// 删除抓拍文件
typedef struct 
{
    TPuPicFileInfo tPicFileInfo;
} TPuPicFileDelExReq;

// 抓拍文件删除结果
typedef struct 
{
    unsigned short    wError;         // 结果
    unsigned long     dwPicID;        // 图片ID
} TPuPicFileDelAck;

//////////////////////////////////////////////////////////////////////////
// 配置类结构定义

//////////////////////////////////////////////////////////////////////////
// from vsip

// 跨网段修改IP
typedef struct 
{
    unsigned long  dwTargetIP; // 目标IP地址
    unsigned long  dwNewIP;    // 新IP地址
    unsigned long  dwNewMask;  // 新子网掩码
    unsigned long  dwNewGW;    // 新网关地址
    TPuGuid        tGUID;      // GUID
} TPuSetIpInfo;

//系统参数结构
typedef struct
{
    unsigned char    abyName[PUSDK_MAX_STRING_LEN64+1];        // 设备名
    unsigned char    abyTypeName[PUSDK_MAX_STRING_LEN64+1];    // 设备类型名(只读)
    unsigned char    byDevType;                                // 设备类型(EPuDeviceType) 
    unsigned char    abySeriesName[PUSDK_MAX_MTALIAS_LEN+1];   // 设备系列名
} TPuSysParam;

//系统公共参数结构
typedef struct
{
    unsigned char      byIndustryType;                             // 行业类型(默认:PUSDK_INDUSTRY_TELECOM)
    unsigned char      abyVersion[PUSDK_MAX_VERSION_STRING_LEN+1]; // 设备版本号(只读)
    
    unsigned char      abyMacAddr[6];                          // MAC
    unsigned long      dwUnitIpAddr;                           // 设备IP地址
    
    unsigned long      dwSubnetMask;                           // 设备子网掩码(网络序)
    unsigned long      dwGateWay;                              // 网关地址(网络序)
    
    unsigned char      byAddrMode;                             // 地址使用方式，固定IP或者DHCP   
    unsigned char      byDhcpEthId;                            // 地址DHCP的网口号
    unsigned char      byUseDhcpGateway;                       // 是否使用DHCP的网关
    
    unsigned short     wVsipPort;                              // VSIP端口号,设备监听端口
    unsigned long      dwRegPlatformIpAddr;                    // 注册平台地址
    unsigned short     wRegPlatformPort;                       // 注册平台端口号
        
    unsigned short     wVsipMultiPort;                         // 搜索端口        
    unsigned long      dwVsipMultiIpAddr;                      // 搜索的组播地址        
    
    unsigned char      abyWebUsername[PUSDK_MAX_MTALIAS_LEN+1];    // 嵌入式Web用户名
    unsigned char      abyWebPassword[PUSDK_MAX_MTALIAS_LEN+1];    // 嵌入式Web密码
    unsigned char      abyGeustUsername[PUSDK_MAX_MTALIAS_LEN+1];  // 嵌入式Web用户名
    unsigned char      abyGeustPassword[PUSDK_MAX_MTALIAS_LEN+1];  // 嵌入式Web密码
    
    unsigned char      byDetPacketSend;                        // 是否发送NAT探测包
    unsigned char      byDetPacketInterval;                    // NAT探测包间隔(默认:PUSDK_MAX_NATSEND_INT)
    
    unsigned char      abyPuid[PUSDK_MAX_PUID_LEN+1];                 // puid
    unsigned char      abyPassword[PUSDK_MAX_STRING_LEN64+1];   // 验证密码
    
    unsigned short     wAlarmContinuance;                      // 告警恢复时间(秒)
    unsigned char      bySubAreaType;                          // 分区格式(只读)
    unsigned char      byMaxTranferNum;                        // 最大直传路数
    unsigned char      byMatrixValid;                          // 是否接入矩阵
    char               chSysTimeZone;                          // 系统时区信息(0:UTC,+1~+12:东时区,-1~-12:西时区,默认:PUSDK_DEFCFG_TIMEZONE)
    char               byIsDecCombined;                        // 是否画面合成输出(仅解码器) 
    
    unsigned char      byDoubleFlowEnable;                     // 是否开启双流，0-不开启，1-开启
    unsigned char      byVideoEncCurNum;                       // 视频编码当前使用通道数
    unsigned char      byEncPref;                              // 码流优先方式（速度优先/图像质量优先）
	char               abySupportCharsetEncode[PUSDK_MAX_STRING_LEN64 + 1];  //支持的字符集
    char               abyActiveCharsetEncode[PUSDK_MAX_STRING_LEN64 + 1];    //当前的字符集
} TPuSysPubParam;

//PPPOE参数结构
typedef struct
{
    unsigned char  m_byPPPOEState;                 // PPPOE状态 0-不使用, 1-使用
    char           m_achUsername[PUSDK_MAX_USER_NUM];    // 拨号用户名
    char           m_achPassword[PUSDK_MAX_USER_NUM];    // 拨号密码
    unsigned char  m_byAutoDialState;              // 自动拨号状态 0-不使用, 1-使用
    unsigned long  m_dwAutoDialWait;               // 自动拨号等待时间(默认60秒)
    unsigned long  m_dwAutoDialTry;                // 自动拨号重试次数(默认6次)
    unsigned char  m_byDebugState;                 // Debug状态 0-不使用, 1-使用
    unsigned short m_wLcpEchoSendInterval;         // 链接建立后，发送LCP-ECHO包的时间间隔(默认30秒)
    unsigned short m_wLcpEchoRetryTimes;           // 发送几次LCP-ECHO包没有得到相应则认为链接断开(默认6次)
    unsigned char  m_byDefaultRouteEnable;         // 路由使能, 0-不使用, 1-使用
} TPuPppoeParam;

// 基本能力集
typedef struct
{
    unsigned char        m_byVideoEncNum;              // 视频编码通道数量（反映平台应用需要的通道数）
    unsigned char        m_byVideoDecNum;              // 视频解码通道数量
    unsigned char        m_byAudioEncNum;              // 音频编码通道数量
    unsigned char        m_byAudioDecNum;              // 音频解码通道数量
    
    unsigned char        m_bySerialPortNum;            // 串口数量
    unsigned char        m_byInputPinNum;              // 并口输入数量
    unsigned char        m_byOutputPinNum;             // 并口输出数量
    unsigned char        m_byVideoInputPortNum;        // 视频输入端口数量
    
} TPuBaseCap;

//杂类能力（扩展能力）
typedef struct
{
    int     m_nVGANum;                      // 视频输出VGA接口数量
    int     m_nVidOutNum;                   // 视频输出接口数量
    int     m_nPTZCtrlNum;                  // 可控制云台数量
    int     m_nEtherNetNum;                 // 以太网口数量
    int     m_nWirelessNum;                 // 无线网口数量
    int     m_nXiAnTeleNum;                 // 西安电信告警数量
    int     m_nPlatFormType;                // 接入平台类型
    int     m_nDevPowerType;                // 供电类型
    int     m_nUseSd;                       // 是否使用SD卡
    unsigned char m_bySlotInfoNum;          // 槽位信息数量(网管用)
    
    int     m_nVidAdpterNum;                // 视频转压通道数量
    int     m_nVidCompNum;                  // 图像合成通道数
    int     m_nVidSrcPollNum;               // 视频源轮询通道数量
    int     m_nAudMixNum;                   // 音频混音通道数量
    int     m_nOsdColorNum;                 // 台标字幕支持的颜色数
    int     m_nShadeAreaNum;                // 图像遮蔽区域数量
    int     m_nMaxStreamSendNum;            // 单个通道最大发送能力(一个通道包括视频和音频)
    int     m_nMaxStreamDirSendNum;         // 单个通道的最大直传路数(一个通道包括视频和音频)
    int     m_nStreamEncrypt;               // 是否码流加密
    int     m_nMotionDetectAreaNum;         // 移动侦测区域数量
    int     m_nSnapShotChanNum;             // 抓拍通道数
    int     m_nCBRVBR;                      // 是否支持码率控制,1－支持，0－不支持
    int     m_nMaxEncBitRate;               // 最大有效码率             
    int     m_nSupportMultiChan;            // 是否支持双流，1－支持，0－不支持
    int     m_nMatrix;                      // 是否支持矩阵接入
    
    int     m_nRecorderNum;                 // 录像通道数量
    int     m_nPlayerNum;                   // 放像通道数量
    int     m_nFileDownloadNum;             // 录像文件下载通道数量

    int     m_nShadeAreaSize;               // 最大图像遮蔽面积 
    int     m_nSupportCompStyle;            // 画面合成能力 1,2,4,8位分别代表1,4,9,16画面的能力
    int     m_nImageDisplaySetting;         // 支持的本地图像显示参数配置能力(每个bit表示一种配置项) EPuImageSetting
    unsigned char      m_bySupportImageDisPlay;        // 支持本地图像输出
    unsigned char      m_byRecTypeSupport;             // 支持的录像类型
    unsigned char      m_bySupportSnapShot;            // 支持抓拍类型
    unsigned char      m_bySupportP2P;                 // 支持P2P，1-支持, 0-不支持
    unsigned char      m_bySelfCamera;                 // 自带摄像头，1-支持, 0-不支持
    unsigned char      m_byOtherAlarmNum;              // 扩展告警口数量
    unsigned char      m_byPuVer;                      // 前端版本号指0(v3.9);1(v4.0);2(v5.0)
    unsigned char      m_byVideoEncSlaveChannelNum;    // 视频编码辅流通道数

    unsigned char      m_bySupportAEC;                 // 支持回声消除
    unsigned char      m_bySupportOsdScroll;           // 支持横幅
    unsigned char      m_bySupportOsdExtension;        // 支持扩展

    unsigned char      m_bySupportGps;                 // 支持GPS
    unsigned char      m_bySupportWifi;                // 支持WiFi
    unsigned char      m_bySupport3g;                  // 支持3G
    
    unsigned char      m_byEncQualityOptimize;         // 支持编码质量优化
    unsigned char      m_bySupportBncOut;              // 支持本地BNC输出
    
    int                m_adwVideoAudeoEnc[16];         // 辅流视频和音频的对应关系
    unsigned char      m_byVideoEncPhysicalNum;        // 视频编码物理通道数量
    unsigned char      m_bySupportEmergentButton;      // 支持紧急告警按钮
} TPuMiscCap;

// 设备能力集定义
typedef struct 
{
    TPuBaseCap		  m_tBaseCap;
    TPuMiscCap        m_tExtCap;
} TPuCapInfo;

//Video Type
typedef struct
{
    unsigned char bySN4;
    unsigned char byMPEG4;
    unsigned char byH261;
    unsigned char byH263;
    unsigned char byH264;
} TPuVideoType;

//Video Resolution
typedef struct
{
    unsigned char byAuto;
    unsigned char byQCIF;
    unsigned char byCIF;
    unsigned char by2CIF;
    unsigned char by4CIF;
    unsigned char byVGA;
    unsigned char byQQCIF;
    unsigned char byQVGA;
    unsigned char byXVGA;
    unsigned char by720P;
    unsigned char by1080P;
} TPuVideoResolution;

//视频图象分辨率
typedef struct
{
    TPuVideoResolution tSN4;
    TPuVideoResolution tMEPG4;
    TPuVideoResolution tH261;
    TPuVideoResolution tH263;
    TPuVideoResolution tH264;
} TPuSpVideoResolution;

//视频编码通道参数结构
typedef struct
{  
    TPuVideoType         tSupportedVideoType;       // 支持的视频格式(只读)
    TPuSpVideoResolution tSupportedVideoResolution; // 支持的视频分辨率(只读)
    unsigned char        byVideoType;             // 数据压缩格式，参见 EPuVideoType
    unsigned char        byVideoResolution;       // 图象分辨率，参见 EPuVideoResolution
    unsigned char        byFrameRate;             // 帧率 1-25
    
    unsigned long        dwBitRate;               // 编码比特率 64K-4M
    unsigned char        byMaxQuantization;       // 最大量化因子 1-31
    unsigned char        byMinQuantization;       // 最小量化因子 1-31
    unsigned short       wInterval;               // 关键帧间隔 25-2500
    char                 byBrightness;            // 亮度 -128～127
    char                 byContrast;              // 对比度 -128～127
    char                 bySaturation;            // 饱和度 -128～127
    char                 byHue;                   // 色度 -128～127
    char                 chSharpness;             // 锐度 -128～127
    unsigned char        byBitRateType;           // 0:定码率, 1:变码率
    
    unsigned char        bySendRetrans;           // 是否发送重传(默认:1-是)
    unsigned short       wSendBufTimeSpan;        // 重传时间间隔(默认:200(1~1500毫秒)
    int                  bOpenMultiChan;          // 双流是否开启(不使用)
    unsigned char        byImageQuality;          // 图像质量(默认:1-质量优先, 2-速度优先)
    unsigned char        bSlaveChannel;           // 是否是辅流通道(默认:0-不是)
    
    unsigned char        byH264FrameMode;         // 0: FieldMode, 1:FrameMode
} TPuVideoEncParam;

//视频编码公共参数结构
typedef struct 
{
    unsigned char        byMaxVideoEncNum;      //最大可用视频编码通道数量(只读)
    unsigned char        byVideoEncUsed;        //启用的视频编码通道数
} TPuVideoEncPubParam;

// 码流链接类型
typedef struct
{
    unsigned char byUdpUnicast;                // 组播方式
    unsigned char byUdpMulticast;              // 多播方式
    unsigned char byUdpBroadcast;              // 广播方式
    unsigned char byTcpServer;                 // TCP服务端
    unsigned char byTcpClient;                 // TCP客户端
    unsigned char bySSL;                       // SSL方式
} TPuConnType;

//公共网络参数结构
typedef struct
{
    unsigned char        byInitStreamState;      // 码流初始化状态 0-不发送, 1-发送
    unsigned char        byCurStreamState;       // 码流当前状态 0-未发送, 1-发送
    TPuConnType          tSupportedConnType;     // 支持的码流链接类型
    unsigned char        byConnectionType;       // 码流链接类型
    unsigned long        dwStreamIpAddr;         // 码流传输地址
    unsigned short       wStreamPort;            // 码流传输端口
} TPuCommonNetParam;

//视频编码字幕参数结构
typedef struct 
{
    unsigned char        byShowTimeState;      // 是否显示时间 0-不显示, 1-显示
    unsigned char        byOSDState;           // 编码字幕状态 0-无字幕, 1-有字幕
} TPuVideoEncOsdParam;

//视频编码多画面参数结构
typedef struct 
{
    unsigned char abyEncodeMember[PUSDK_MAX_PIC_REGION_NUM];// 多画面对应的视频源端口号
    unsigned char byEncodeMemberNum;                        // 多画面对应的视频源端口数量
    unsigned char byEncodeStyle;                            // 多画面编码风格类型(0-不合成，1-四画面合成)
} TPuVideoEncMpParam;

//视频编码视频源端口参数结构
typedef struct 
{    
    unsigned char   abyValidVideoInputPort[PUSDK_MAX_VIDEOIN_PORT_NUM];// 可用的视频输入端口号(只读)
    unsigned char   byValidVideoInputPortNum;                          // 可用的视频输入端口数量(只读)
    unsigned char   byVideoCurrentInputPort;                           // 当前使用的视频输入端口号
} TPuVideoEncInputPortParam;

// 视频编码通道关系参数
typedef struct
{
    unsigned char  byChannelId;        // 编码通道Id
    unsigned char  byMainChannelId;    // 对应主编码通道Id
} TPuVideoEncChannelRelation_Item;

// 视频编码通道关系表参数结构
typedef struct 
{
    unsigned char                    byChannelNum;   // 通道数
    TPuVideoEncChannelRelation_Item  atChannel[32];  // 通道信息    
} TPuVideoEncChannelRelation;

//视频编码轮询参数结构
typedef struct 
{
    unsigned char        byPollState;                          // 轮询状态 0-不轮询, 1-有效
    unsigned char        byPollPortNum;                        // 轮询端口数量
    unsigned char        abyPollPort[PUSDK_MAX_POLL_PORT_NUM]; // 轮询端口集
    unsigned short       wPollInternal;                        // 轮询时间间隔 单位秒
} TPuVideoEncPollParam;

typedef struct 
{
    unsigned char  byChanType;                 // 主流还是辅助流, 0:辅流,1:主流
    unsigned char  bySupportSubNum;            // 支持辅助流路数，仅主流有效;
    unsigned char  SubChanId[8];               // 辅助流ID列表，根据辅助流路数，依次填写,仅主流有效;
    unsigned char  byStatus;                   // 当前辅助流是否开启（1－开启，0-未开启），仅主流有；如果是开启双流，则填1，如果停止双流则填0；
    unsigned char  byParentEncId;              // 主流的编码通道号,仅辅助流有效
    unsigned long  m_dwReserved;               // for align
} TPuMultiEncChanParam;

//视频解码参数结构
typedef struct 
{
    TPuVideoType   tSupportedVideoType;      // 支持的视频格式(只读)
    unsigned char  byPlayerBufferNum;        // 播放缓冲区数 6-50
    unsigned char  byPlayerOriginBufNum;     // 播放起始缓冲区号 0-10
    unsigned char  byVideoType;              // 视频解码格式，参见EPuVideoType
    unsigned char  byDecDealLevel;           // 图像后处理级别(0～3级,0表示没有)
    unsigned char  byImageQuality;           // 图像质量(默认:1-质量优先, 2-速度优先)
    unsigned char  byPerpenSync;             // 垂直同步(0-关闭，1-开启)
} TPuVideoDecParam;

//视频解码公共参数结构
typedef struct 
{    
    unsigned char  byVGAOutSupported;        //是否支持VGA输出(只读) 0-不支持, 1-支持
    unsigned char  byVideoOutputType;        //视频输出端口类型 (参见 EPuVideoOutType )
    unsigned char  byVGAFrequency;           //VGA输出刷新频率 (60/75/85)
    unsigned char  byStarveMode;             //无视频的显示方式，参见 EPuPlayerStarveMode
} TPuVideoDecPubParam;

//视频解码重传参数结构
typedef struct 
{
    unsigned short wRetransfer1stTimeSpan;  // 解码第1个重传检测点(默认40)
    unsigned short wRetransfer2ndTimeSpan;  // 解码第2个重传检测点(默认80)
    unsigned short wRetransfer3rdTimeSpan;  // 解码第3个重传检测点(默认160)
    unsigned short wRejectTimeSpan;         // 解码过期时间(默认200)
    unsigned char  byRetransferState;       // 解码重传状态 0-不重传, 1-重传
} TPuVideoDecRetransferParam;

//音频压缩格式
typedef struct
{
    unsigned char byPCMA;
    unsigned char byPCMU;
    unsigned char byGSM;
    unsigned char byG729;
    unsigned char byADPCM;
    unsigned char byG7221C;
} TPuAudioType;

//音频输入类型
typedef struct 
{
    unsigned char      byLineIn;
    unsigned char      byMic;
} TPuAudioInMode;

//音频编码参数结构
typedef struct 
{
    TPuAudioType   tSupportedAudioType; // 支持的数据压缩格式(只读)
    TPuAudioInMode tSupportedAudioIn;   // 支持的音频输入方式
    
    unsigned char  byAudioType;         // 音频数据格式 EPuAudioType
    unsigned char  byAudioInputType;    // 音频输入类型，1-Line In，2-mic
    unsigned char  byVolume;            // 音量 0-25
    unsigned char  byAECState;          // 回声抵消状态 0-无效, 1-有效
    unsigned char  byDumbState;         // 哑音状态 0-无效, 1-有效
    unsigned char  byIsMix;             // 是否混音
} TPuAudioEncParam;

//音频解码参数结构
typedef struct 
{
    TPuAudioType   tSupportedAudioType; // 支持的数据压缩格式(只读)
    unsigned char  byVolume;            // 音量 0-25
    unsigned char  byMuteState;         // 静音状态 0-无效, 1-有效
} TPuAudioDecParam; 

//串口类型
typedef struct
{
    unsigned char byRS232;
    unsigned char byRS422;
    unsigned char byRS485;
} TPuSerialType;

//串口参数结构
typedef struct 
{
    unsigned long   dwBaudRate;       // 串口波特率
    unsigned char   byComPortType;    // 串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485
    TPuSerialType   tSupportedType;   // 支持的串口类型 (只读)
    unsigned char   byStop;           // 停止位，默认1
    unsigned char   byDig;            // 数据位，默认8
    unsigned char   byParityCheck;    // 奇偶校验 0-不校验，1-偶校验，2-奇校验
    unsigned char   byStart;          // 起始位，默认1
    unsigned long   dwTimeOutMS;      // 读串口最大超时时间(ms)，默认200
    unsigned long   dwMaxReadByte;    // 读串口最大字节数，默认8
    unsigned long   dwWriteDelayTime; // 写串口延时时间(ms)
    unsigned char   byIsSlaveMode;    // 串口是否是从模式
} TPuSerialPortParam;

//串口公共参数结构
typedef struct 
{
    unsigned char  byInitStreamState;         // 串口数据收发初始化状态 0-无效, 1-有效
    unsigned short wStreamLocalPort;          // 串口数据本地接收端口
    unsigned long  dwStreamDestIpAddr;        // 串口数据对端地址
    unsigned short wStreamDestPort;           // 串口数据对端端口
    TPuConnType    tSupportedConnType;        // 支持的码流链接类型
    unsigned char  byConnectionType;          // 码流链接类型
    unsigned char  byProtocolType;            // 协议类型 0-透明通道, 1-VSIP协议
} TPuSerialPortPubParam;

//视频源输入端口参数结构
typedef struct 
{
    unsigned char  byCameraType;                 // 摄像头类型，参见 EPuCameraType
    unsigned char  byCameraID;                   // 摄像头地址码
    unsigned char  bySerialPortID;               // 对应的串口ID
    unsigned char  abyVideoPortName[PUSDK_MAX_STRING_LEN16+1];    // 视频源端口名
    unsigned char  abyVideoPortOSD[PUSDK_MAX_STRING_LEN64+1];    // 视频源字幕内容
    unsigned short wVideoPortOSDLeftMargin;      // 视频源字幕左边距
    unsigned short wVideoPortOSDTopMargin;       // 视频源字幕上边距
    unsigned long  dwVideoPortOSDColor;          // 视频源字幕颜色，RGB值
    
    unsigned char  byOsdFontMode;                // 字体模式 EPuOsdFontMode
    unsigned char  abyOsdCustomFontName[32+1];   // 自定义字体名称
    unsigned long  dwOsdCustomFontSize;          // 自定义字体大小
    unsigned long  dwOsdCustomFontStyle;         // 自定义字体风格 (EPuCustomFontStyle)
    
    char           byLighteness;                 // 亮度-127~128 (默认:0)
    char           byContrast;                   // 对比度-127~128 (默认:0)
    char           bySaturation;                 // 饱和度-127~128 (默认:0)
    char           byHue;                        // 色度-127~128 (默认:0)
    
    // add
    unsigned char  byAutoResetEnable;            // 是否自动归位             
    char           szResetCmd[PUSDK_MAX_STRING_LEN64+1]; // 自动归位字符串
    unsigned short wAutoResetTime;                // 自动归位时间，默认60秒
    
    unsigned short byNoVideoAlarmEnable;         // 是否上报视频源丢失告警

    unsigned short wTimeOsdLeftMargin;           // 时间左边距
    unsigned short wTimeOsdTopMargin;            // 时间上边距
    
    unsigned short wAlarmTimeOsdLeftMargin;      // 告警计时左边距
    unsigned short wAlarmTimeOsdTopMargin;       // 告警计时上边距
    
    unsigned short wAlarmOsdLeftMargin;          // 告警字幕左边距
    unsigned short wAlarmOsdTopMargin;           // 告警字幕上边距
} TPuVideoInputParam;

//本地视频图像输出配置
typedef struct 
{
    unsigned char byOutputChanId;                         // 图像输出通道ID
    unsigned char byImageOutputEnable;                    // 图像本地输出使能
    unsigned char byCompStyle;                            // 画面合成编码风格 (0-1画面,1-4画面)
    unsigned char byCompMemberNum;                        // 画面合成视频源端口数量
    unsigned char abyCompMember[PUSDK_MAX_PIC_REGION_NUM];// 画面合成视频源端口号
} TPuLocalImageOutputParam;

//横幅参数结构
typedef struct {
    unsigned long       m_dwBackgroundColor;  // 背景颜色 （0:透明,1:半透明,2:不透明）
    unsigned long       m_dwFontColor;        // 字体颜色（默认：0xFFFFFF白色）RGB(r,g,b)
    unsigned long       m_dwHaltTime;         // 停留时间
    unsigned short      m_wLeftMargin;        // X坐标
    unsigned short      m_wTopMargin;         // Y坐标
    unsigned short      m_wOnelineHigh;       // 字幕单行高度
    unsigned short      m_wScrollTimes;       // 滚动次数(默认:1)
    unsigned char       m_byFontSize;         // 字体大小(默认:28)
    unsigned char       m_byArrangeMode;      // 对齐模式(0-左对齐 1-中对齐 2-右对齐)
    unsigned char       m_byScrollMode;       // 滚动模式(0-不动 1-下上 2-上下 3-右左 4-左右)
    unsigned char       m_byScrollSpeed;      // 滚动速度(0-快 1-中 2-慢)
    unsigned char       m_byScrollType;       // 滚动方式(0始终，1指定次数)
    unsigned char       m_byScrollLineNum;    // 横幅行数(默认:1)
    unsigned char       m_byTransparency;     // 横幅背景(0-透明，1-半透明，2-不透明)
    unsigned char       m_byOsdScrollEnable;  // 横幅功能使能(0-无效, 1-有效)
    char                m_achFontType[16+1];  // 字体类型(默认：宋体)
    char                m_achContent[256+1];  // 内容 多行字幕的时候用■分割，如两行字幕AAA■BBB
} TPuOsdScroll;

//图像遮蔽控制区域
typedef struct 
{
    unsigned char  byShadeEnable;         // 遮蔽使能 1-有效 0-无效
    unsigned long  dwXPos;                // 遮蔽区域顶点横坐标
    unsigned long  dwYPos;                // 遮蔽区域顶点纵坐标
    unsigned long  dwWidth;               // 遮蔽区域宽度，范围 0-720
    unsigned long  dwHeigth;              // 遮蔽区域高度，范围 0-576
} TPuVideoShadeArea;

//图像遮蔽控制结构体
typedef struct 
{
    unsigned short     wAreaNum;                                    // 区域数目
    TPuVideoShadeArea  atShadeArea[PUSDK_MAX_VIDEO_SHADE_AREA_NUM]; // 图像遮蔽控制区域
} TPuVideoShadeParam;

// 告警联动参数
typedef struct 
{
    // 视频源相关
    unsigned char        byVidSrcId;            // 关联视频源id(物理)(有效视频源id列表)
    unsigned char        byVidSrcSwtEnable;     // 视频源切换使能(默认:否)
    unsigned char        byCamPosSwtEnable;     // 视频源预置位切换(默认:否)(控制台无需显示)
    unsigned long        dwCamPosId;            // 摄像头预置位id(默认:0)(控制台无需显示)    
    unsigned char        bySnapShotEnable;      // 图象抓拍(默认0:否)
    unsigned char        byLocalRecEnable;      // 本地录像使能(默认0:否)
    unsigned char        byAlarmTimeEnable;     // 告警计时使能(默认0:否)
    unsigned char        bySoundAlarm;          // 告警声音使能(默认0:否)
    unsigned char        byOsdShowEnable;       // 字幕显示(默认0:否)
    unsigned char        byOsdUseType;          // 字幕类型(默认:0)
    unsigned char        byColorId;             // 颜色
    char                 achOsdName[PUSDK_MAX_STRING_LEN64 + 1];     // 字幕内容(默认:alarm+id)
    
    // 并口相关
    unsigned char        byPinOutputEnable;     // 并口输出通道使能(默认0:否)
    unsigned char        byOutputPinId;         // 关联的告警输出通道(有效并口输出通道id列表)
    unsigned short       wLinkFuncSupport;      // 告警联动支持操作参数，PUSDK_LINKSET_VIDSRCSWT_SUPPORT 等多选(按位或)
} TPuAlarmLinkInfo;

//网管参数
typedef struct 
{    
    unsigned char  byMemThreshold;             // 系统内存使用阀值
    unsigned char  byMaxVideoLoseRatio;        // 视频丢包上报阀值
    unsigned long  dwServerIP;                 // 网管服务器IP地址
    unsigned short wServerPort;                // 网管服务器Port
    unsigned char  byCpuUseRatio;              // cpu利用率
    
    char           achGetCommunity[ 32 + 1 ];  // 查询共同体名(默认:public)
    char           achSetCommunity[ 32 + 1 ];  // 设置共同体名(默认:private)
    unsigned char  byTrapServerNum;            // trap服务器个数(默认:1个)
    unsigned short wLocalPort;                 // SNMP本地端口号，默认161
} TPuNmsParam;

// 前端抓拍参数
typedef struct 
{
    unsigned char  byPinAlarmEnable;        // 并口告警抓图使能 0关闭/1开启
    unsigned char  byMotionDetectEnable;    // 移动侦测抓图使能 0关闭/1开启
    unsigned long  dwInterval;              // 抓拍时间间隔(s)
    unsigned long  dwMaxSnapNum;            // 最大抓拍数量
} TPuSnapshotConfig;

// 命名规则
typedef struct 
{
    unsigned char abyNameOrder[4];                 // 字段顺序，从0开始，依次表示第i+1个字段
    unsigned char byNameNum;                       // 实际使用的字段个数
    char          achDateSeparator[8];             // 日期之间的分隔符(年月日)(有效可打印字符)
    char          achTimeSeparator[8];             // 时间之间的分隔符(时分秒)(有效可打印字符)
    char          achNameSeparator[8];             // 命名字段之间的分隔符(有效可打印字符)
    char          achUserDefine[64+1];             // 用户自定义字段内容
} TPuNamingRule;

// FTP配置
typedef struct 
{
    unsigned long   m_dwFtpSvrIpAddr;                           // FTP服务器地址
    unsigned short  m_wFtpSvrPort;                              // FTP服务器端口
    char            m_achFtpSvrPath[PUSDK_MAX_FILEPATH_LEN+1];  // FTP服务器存放路径
    char            m_achUserName[64 + 1];                      // 用户名
    char            m_achUserPassWord[64 + 1];                  // 用户密码
} TPuFtpServer;

// 文件存储配置
typedef struct 
{
    unsigned char byStoreType;      // 存储类型，0-不存储,2-本地存储
    TPuFtpServer  tPuFtpCfg;        // FTP配置
    
    // 本地存储配置
    unsigned char              byUsedDiskPartionhNum;                          // 用户使用的分区个数
    unsigned char              abyUsedDiskPartionId[ PUSDK_MAX_DISKPART_NUM ]; // 用户使用的分区路径列表(分区硬件ID)
} TPuFileStoreParam;

// 前端抓拍图片存储配置
typedef struct 
{
    TPuNamingRule      tNamingRule;       // 抓拍图片命名规则
    TPuFileStoreParam  tFileStoreCfg;     // 存储配置
} TPuSnapshotStoreConfig;

// 系统扩展参数
typedef struct
{
    unsigned char  byLinkChkInterval;                      // 链路检测间隔(单位:秒,默认:10秒(0~60s)
    unsigned char  byLinkChkDisNum;                        // 链路检测最大尝试次数(单位:次,默认:3次(1~255次)
    
    // 时间同步
    unsigned char  byTimeSynMode;                          // 时钟同步方式 
    unsigned long  dwSynTimeServerIp;                      // 时钟同步服务器,0表示不同步(默认:ip=0)
    unsigned short wSynTimeServerPort;                     // (默认port=123)
    unsigned long  dwSynTimeInterval;                      // 同步时间间隔1~72*60分钟(默认:1)
    
    // socks5
    unsigned long  dwSocks5ServerIp;                       // SOCKS5 代理服务器,0表示不使用(默认:ip=0)
    unsigned short wSocks5ServerPort;                      // (默认port=1080)
    char           achSocks5UserName[ PUSDK_MAX_MTALIAS_LEN + 1 ];    // 用户名
    char           achSocks5Password[ PUSDK_MAX_MTALIAS_LEN + 1 ];    // 用户密码
    
    // Domain
    unsigned char  byUseDomain;                            // 是否使用域名
    char           achPlatDomain[PUSDK_MAX_STRING_LEN64+1];// 平台域名
    unsigned char  byLdsMode;                              // LDS使用模式，0：no use，1：IP， 2：domain
    char           achLdsDomain[PUSDK_MAX_STRING_LEN64+1]; // LDS域名
    unsigned long  dwLdsServerIp;                          // LDS 地址
    unsigned short wLdsServerPort;                         // LDS 端口
    
    // other
    unsigned char  byStreamMaxSendNum;             // 当前单个通道的最大发送路数(默认:6)
    
    int            bUseIndiLight;                  // 是否开启指示灯
    int            bSupportBuildNet;               // 是否支持组网 TURE:支持
    
    unsigned short wStartPort;                     // 本地发送或接收起始端口(默认: 60000)
    char           achDeviceIPosInfo[32+1];        // 前端位置信息
    unsigned long  dwDnsAddr;                      // 主要DNS
    unsigned long  dwDnsBackupAddr;                // 次要DNS
    
    unsigned char  byCameraInfoNotifyPlantformEnable;// 摄像头数据上报平台使能
    
} TPuSysExtParam;

// 前端点对点信息
typedef struct 
{
    int            bOpenP2PConnect;            // 是否开启点对点
    unsigned long  dwPuDestAddr;               // 点对点对端IP
    unsigned short wPuDestPort;                // 点对点对端Port
    int            bOpenRmtSerial;             // 点对点是否远端读串口
    unsigned char  byDestEncChan;              // 点对点对端编码通道
    unsigned char  bySendAud;                  // 是否发送音频
} TPuVideoDecPpParam;

// 按天定义的时间段
typedef struct 
{
    unsigned long dwStartTime;                // 开始时间, hh*3600+mm*60+ss
    unsigned long dwEndTime;                  // 结束时间, hh*3600+mm*60+ss
} TPuDayPeriodTime;

// 按绝对时间定义的时间段, 1970起始
typedef struct 
{
    unsigned long dwStartTime;                // 开始时间, 1970起始
    unsigned long dwEndTime;                  // 结束时间, 1970起始
} TPuFixPeriodTime;

// 周期时间表
typedef struct 
{
    unsigned char     m_byFixUsedNum;              // 固定时间表中使用的个数
    unsigned char     m_byDayUsedNum;              // 日时间表中使用的个数
    unsigned char     m_byWeekUsedNum;             // 周时间表中使用的个数
    
    TPuFixPeriodTime  m_atFixTimerTable[6];        // 定时
    TPuDayPeriodTime  m_atDayTimerlyTable[6];      // 每天定时
    TPuDayPeriodTime  m_atWeekTimerlyTable[7*6];   // 每周排列顺序为[7*loop+week]
} TPuPeriodTable;

// 前端录像通道参数
typedef struct 
{
    unsigned char      byChanId;                   // 录像通道
    unsigned short     wAlarmPreRecTime;           // 告警预录时间，默认0秒
    unsigned short     wAlarmResumeRecTime;        // 告警恢复后继续录制时间，默认5秒
    unsigned char      byIsAbstractFrame;          // 是否抽帧录象 1为是，0为否
    unsigned char      byIsRecWhenAlarmOccur;      // 告警是否录象,1为是,0为否
    unsigned char      byAbstractRecInterval;      // 抽帧录像间隔,指关键帧个数，默认5
    unsigned char      byIsAudioRecorded;          // 是否录音频，默认1启用
    unsigned char      byRecChanNum;               // 0-录制主码流，其它-录制辅助码流号,默认为0
    
    unsigned char      m_byRecTimeMode;            // 录像时间模式，EPuRunTimeMode
    TPuPeriodTable     tPeriodTable;               // 周期时间表
    
} TPuRecChannel;

// 前端录像配置参数
typedef struct 
{
    unsigned char      byDiskFullStrategy;                 // 磁盘满录象策略 0-停止 1-全覆盖 2-仅覆盖告警录像
    unsigned char      abyRecFileStorePath[16];            // 录象文件存放路径
    unsigned long      dwDiskRemainSpace;                  // 磁盘剩余空间(50MB)
    unsigned short     wLittleFileRecTime;                 // 小文件录象时长(2MB)
    unsigned char      byDiskPartitionNum;                 // 文件分区数目,用户不可设置
    unsigned char      abyRecDiskPartitionInfo[16][64];    // 磁盘分区信息，用户不可设置
    unsigned char      byUsedPartitionNum;                 // 用户使用了的分区数目,默认1
    unsigned long      dwDownloadSpeedLimit;               // 录像下载速度KB(默认:1024)     
    unsigned char      byIsPltRecWhenRecErr;               // 录像出错时是否平台转录(默认0不启用)
    unsigned char      byIsAutoFixWhenRecErr;              // 录像出错是否自动修复(默认1启用)
    unsigned char      byRecOsdState;                      // 录像图标状态(默认1启用)
    unsigned char      byPuDisConnRecEnable;               // 开启前端转录，0-不开启，1-开启
    unsigned char      byIsAlarmWhenDiskErr;               // 磁盘异常告警上报
} TPuRecSys;

// 前端录像基本配置
typedef struct 
{
    unsigned char      byChannelId;
	unsigned short     wAlarmPreRecTime;          // 告警预录时间
	unsigned short     wAlarmResumeRecTime;       // 告警恢复后继续录制时间
	unsigned char      byIsAbstractFrame;         // 是否抽帧录象 1为是，0为否
	unsigned char      byIsRecWhenAlarmOccur;     // 告警是否录象,1为是,0为否
	unsigned char      byAbstractRecInterval;     // 抽帧录像间隔,指关键帧个数
	unsigned char      byIsAudioRecorded;         // 是否录音频
    unsigned char      byRecChanNum;              // 0-录制主码流，其它－录制辅助码流号,默认为0
} TPuRecChanBasicInfo;

// 前端录像扩展参数
typedef struct 
{
    unsigned long         dwDownloadSpeedLimit;      // (冗余)
    TPuFileStoreParam     tRecFileStoreCfg;          // 录像存储信息
    
} TPuRecSysEx;

// 摄像头预置位
typedef struct 
{ 
    unsigned char byUsePreset;                // 预置位使能    
    char          achPresetAlias[16];         // 预置位别名
} TPuCameraPresetInfo;

// 摄像头配置
typedef struct 
{
    unsigned char      byCurPresetNo;   // 当前预置位
    unsigned char      byInverseMode;   // 当前反转类型
    unsigned char      byUseAutoBack;   // 是否自动归位
    unsigned short     wAutoBackTime;   // 自动归位时间
    unsigned char      byScanMode;      // 自动扫描方式
    
    unsigned char      byScanTimeMode;  // 扫描时间模式, EPuRunTimeMode
    TPuPeriodTable     tScanPeriodTalbe;// 自动扫描时间表
    
    TPuCameraPresetInfo  tPresetInfo[8];   // 预置位信息
} TPuCameraCfg;


//并口输入参数结构
typedef struct 
{
    unsigned char    byAlarmDetectState;    // EPuRunTimeMode
    
    unsigned char    byRelateEncId;         // 告警关联的编码通道 默认0
    unsigned char    byAlarmCheckState;     // 告警状态 0-无效 1-低电平 2-高电平 3-闭合 4-打开
    char             achChanAlais[32 + 1];  // 别名
    TPuAlarmLinkInfo tAlarmLinkInfo;        // 告警联动参数
    TPuPeriodTable   tCheckPeriodTable;     // 检测时间表
	
} TPuInputPinParam;

// 并口输出通道参数
typedef struct 
{
    unsigned char m_byAlarmMode;                      // 告警模式(默认:3-闭合 4-打开)
    char          m_achOutputPinChanAlais[32 + 1 ];   // 并口输出通道别名(默认:并口输出通道+id)
} TPuOutputPinParam;

// DNS配置
typedef struct 
{
    unsigned long dwDnsAddr;          // DNS地址
    unsigned long dwDnsBackupAddr;    // DNS备份地址
} TPuDnsParam ;
//! DDNS配置
typedef struct 
{
    unsigned char   byState;                              // 注册状态
    unsigned char   byDomainNameNum;                      // 返回的域名数量
    char            abyDomainName[32][64+1];              // 域名名称
    unsigned long   dwDonator;                            // 帐户类型：免费，收费
    TPuDnsParam     tDnsCfg;
} TPuDomainName;

typedef struct 
{
    unsigned char byUseDdns;                                 // 是否使用DDNS
    unsigned char byServiceNum;                              // 支持服务个数
    unsigned char abyService[2];                             // 服务类型，3322，dyndns
    unsigned char bySrvType;                                 // 当前服务类型
    char          achDnsServer[16];                          // dns server ip地址
    char          achUsername[128];                          // 认证用户名 
    char          achPassword[128];                          // 认证密码 
    char          achHostname[128];                          // 域名
    char          achInterface[16];                          // 本地网口地址
    TPuDomainName tPuDomain;                                 // 返回的域名信息
} TPuDdnsParam;

//移动侦测区域结构体
typedef struct 
{
    unsigned long  dwXPos;                 // 侦测区域顶点横坐标
    unsigned long  dwYPos;                 // 侦测区域顶点纵坐标
    unsigned long  dwWidth;                // 侦测区域宽度，范围 0-720
    unsigned long  dwHeigth;               // 侦测区域高度，范围 0-576
    
    unsigned char   byDetectEnable;         // 侦测使能 1-有效 0-无效
    unsigned char   byAlarmRate;            // 侦测区域告警产生百分比（大于此临界值告警,0-100）
} TPuMotionDetectArea;

//设置终端移动侦测参数
typedef struct 
{
    unsigned short      wAreaNum;         //区域数目
    TPuMotionDetectArea atDetectArea[PUSDK_MAX_MOTION_DETECT_AREAM_NUM];      //移动侦测参数结构体数组
    
    unsigned char       byDetectTimeMode;      // 监测时间模式, EPuRunTimeMode
    TPuPeriodTable      tCheckPeriodTable;     // 监测时间周期表
    
    unsigned char       byDetectSense;        // 帧测灵敏度(默认:60,范围:0~99%)
    TPuAlarmLinkInfo    tAlarmLinkInfo;       // 告警联动
} TPuMotionDetectParam;

// 扩展卡参数
typedef struct 
{
    unsigned char        byRelateSerialId;   // 扩展卡关联串口Id号         
    unsigned char        byPinExtSerialAddr; // 扩展卡侧串口地址码 
} TPuExtCard;

// 前端扩展卡配置
typedef struct 
{
    unsigned char  byExtCardNum;
    TPuExtCard     atExtCard[8];
} TPuExtCardParam;

//视频解码图像分辨率
typedef struct 
{
    unsigned short wWidth;      // 图象宽
    unsigned short wHeight;     // 图象高
    unsigned short wRefreshRate;// 刷新率
} TPuResolution;

//视频解码图像显示参数结构
typedef struct 
{
    TPuResolution atSupportResType[64];  // 目前最多支持64种
    unsigned char bySupportResTypeNum;   // 支持的分辨率种类数量
    unsigned char byCurrentResType;      // 当前使用的分辨率
    unsigned char bySupportOutputMode;   // 支持的输出模式(只读) 按位 0-无 1-BNC 2-VGA 4-HDMI 8-BNC+VGA 16-BNC+HDMI
    unsigned char byCurrentOutputMode;   // 当前使用的输出模式 
} TPuImageDisplayParam;

//视频解码多画面参数结构
typedef struct 
{
    unsigned char byCompStyle;       // 合成风格 1-1画面 2-4画面 4-9画面 8-16画面
    unsigned char byCompMemberNum;   // 合成数量
    char achCompMember[64];          // 成员列表
} TPuVideoDecCompParam;

// ------------------------------------------------------- //
// 外部数据采集设备
typedef struct
{
    unsigned char  byOsdVideoEncId;       //关联的编码通道
    char           achDeviceCode[32 + 1]; //设备码（设备标识符，MN字段）
    unsigned long  dwOsdColor;            //字体颜色(RGB)
    unsigned short wOsdXpos;              //X坐标
    unsigned short wOsdYpos;              //Y坐标
    unsigned long  dwReserved;            //保留字段
} TPuDAQDevice;

typedef struct 
{
    unsigned char  byState;        //用于标识是否启用
    unsigned char  byProtocolId;   //协议ID（协议类型）对于前端设备只有一个解析协议
    unsigned char  byNum;          //有效个数
    TPuDAQDevice   atDevice[8];    //设备列表
} TPuDAQDeviceParam;

//////////////////////////////////////////////////////////////////////////
// 文件解码器相关 //
#ifdef WIN32
typedef struct 
{
    unsigned __int64        m_qwTotalSize;		// 大小
    unsigned __int64        m_qwCreateDate;     // 创建时间
    unsigned long           m_dwTotalTime; 		// 播放时间
} TPuPlayFileInfo;
#endif
typedef struct 
{
    int            m_bAudio ;           // 是否是音频流
    unsigned short m_wWidth;            // 图像宽度
    unsigned short m_wHeight ;          // 图像高度
    unsigned char  m_byStreamId ;       // 码流编号
    unsigned char  m_byMediaType ;      // 编码格式
} TPuPlayFileStreamInfo ;

typedef struct 
{
    unsigned long m_dwCurrentPlayPos;   // 播放进度
    unsigned char m_byPlayMode;         // 播放方式
    char          m_byPlayRate;         // 播放速度
    unsigned char m_byPlayState;        // 播放状态 1-开始 0-停止
    unsigned char m_byStreamNum ;       // 码流数
    unsigned long m_dwVFps;             // 视频帧率
    unsigned long m_dwAFps;             // 音频帧率
    TPuPlayFileStreamInfo  m_atFileStreamInfo[3]; // 码流信息
} TPuPlayFileStatus;

//////////////////////////////////////////////////////////////////////////
// 应答消息

// PING包应答
typedef struct 
{
    unsigned long      m_dwIpAddress;
    unsigned short     m_wPort;
    
    unsigned long      m_dwVersionID;
    unsigned char      m_abyName[PUSDK_MAX_STRING_LEN64+1];       // 设备名
    unsigned char      m_abyTypeName[PUSDK_MAX_STRING_LEN64+1];   // 设备类型名(只读)
    unsigned char      m_byDevType;                               // 设备类型(EPuDeviceType) 
    unsigned char      m_abySeriesName[PUSDK_MAX_MTALIAS_LEN+1];  // 设备系列名
} TPuPingAck;

// 断链原因
typedef struct
{
    EPuDropType m_eType;
} TPuDropReason;

// 异步消息应答
typedef struct 
{
    unsigned short     m_wErrorCode;
} TPuResult;

// 重启消息
typedef struct 
{
    unsigned char   byRebootType;                    // 重启类型
    unsigned long   dwRebootTime;                    // 重启时间
    char            achRebootHint[128];              // 重启提示
} TPuRebootInfo;

// 无线配置数据结构
// AP点信息
typedef struct
{
    unsigned char  byIsActive;       // 此AP点是否是活动的 
    unsigned char  byHiddenAP;       // 是否为隐藏节点 
    unsigned char  byChannelId;      // 频道号 
    unsigned char  byBSSType;        // 组网方式:EVsipWifiApInfo_byBSSType 
    unsigned char  byVerifyTpye;     // 身份验证方式: EVsipWifiApInfo_byVerifyTpye
    unsigned char  byArithmetic;     // 加密算法:EVsipWifiApInfo_byArithmetic 
    int            dwMaxRate;        // AP可连接最大速度, 单位: kbps 
    unsigned char  byRSSI;           // 信号强度:0到100 
    unsigned char  abyBSSIDMac[6];   // AP点的MAC地址 
    char           achSSID[32+1];    // AP点的名字 
    unsigned char  abyPwdsValid[4];  // 密钥有效性 WEP_KEY_NUM = 4
    char           achPasswd[4][64]; // 密钥表 
    unsigned char  byEncryMode;      // 用于标示加密模式: EVsipWifiApInfo_byEncryMode 
} TPuWifiApInfo;

// 获取配置应答
typedef struct
{
    unsigned char  byAPNum;            // 扫描到的AP点个数 
} TPuWifiCfgGetResp;

// 连接/断开请求
typedef struct
{
    unsigned char  byConnect;          // 连接或断开 
    char           achSSID[32+1];      // AP点的名字 
    unsigned char  byChannelId;        // 频道号 
} TPuWifiConnectionReqParam;

// 上报WIFI连接状态
typedef struct
{
    unsigned char  byWifiState;        // Wifi状态; 连接、断开、无设备等
    unsigned char  byBSSType;          // Infra还是Ad-Hoc 
    unsigned char  abyBSSIDMac[6];     // AP点的MAC地址 
    char           achSSID[32+1];      // AP点的名字 
    unsigned char  byChannelId;        // 频道号 
    unsigned char  byRSSI;             // 信号强度：0到100 
    unsigned long  dwMaxRate;          // AP连接速度, 单位: kbps 
} TPuWifiConnectionStatus;

typedef struct
{
    unsigned char byAddrMode;    // 地址使用方式，固定IP或者DHCP 
    unsigned long dwUnitIpAddr;  // 设备IP地址
    unsigned long dwSubnetMask;  // 设备子网掩码(网络序)
    unsigned long dwGateWay;     // 网关地址(网络序)
    unsigned long dwDNSaddr;     // DNS地址(网络序)
} TPuWifiNetParam;

// 3G配置结构
typedef struct
{ 
    unsigned char  byAutoDisConn;      // 自动断开连接 
    unsigned char  byPhoneListNum;     // 电话白名单长度 
    char           achPhoneNum[5][32]; // 电话号码 
    unsigned char  byMsgListNum;       // 消息白名单长度 
    char           achMsgNum[5][32];   // 消息白名单 
    unsigned long  dwRmgIp;
    unsigned short wRmgPort;
} TPu3gCfgParam;

// 3G状态
typedef struct
{
    unsigned short wLinkState;        // 连接状态；使用unsigned short错误码，0-正常 
    unsigned long dwSignalStrength;   // 信号强度最强为100，最弱为0 
    unsigned long dwOnLineTime;       // 上线时长，单位为分钟 
    unsigned long dwInDataCount;      // 下行数据总量，单位为Kb 
    unsigned long dwOutDataCount;     // 上行数据总量，单位为Kb 
    unsigned long dwLocalIpAddr;      // 自身IP 
    unsigned long dwDns1Addr;         // DNS 1 
    unsigned long dwDns2Addr;         // DNS 2 
    unsigned long dwNetType;          // 网络类型：1->EVDO, 2->WCDMA, 3->TD-WCDMA
    char          achVendor[16 + 1];  // 3G芯片提供商：HUAWEI, QUALCOMM
    char          achProduct[16 + 1]; // 3G芯片类型: EM660, MI600
} TPu3gStatus;

// 无线制式
typedef struct
{
       unsigned char byWirelessMode;     // 无线方式 
} TPuWirelessModeParam;

// GPS订阅设置请求/查询应答
typedef struct
{
    unsigned char  byEnabled;      // 1－创建，0－取消
    unsigned short wType;          // 透明数据类型
    unsigned long  dwSubType;      // 透明数据子类型
} TPuGpsSubScribeParam;

// GPS订阅设置应答
typedef struct
{
    unsigned short wError;    //结果，1-成功 0-失败
} TPuGpsSetSubScribeAckParam;

// GPS订阅查询请求
typedef struct
{
    unsigned short wType;              //透明数据类型
    unsigned long  dwSubType;          //透明数据子类型
} TPuGpsGetSubScribeParam;

// GPS配置
typedef struct
{
    unsigned char  byEnabled;        //是否使能
    unsigned short wCollectInterval; //采集间隔
    unsigned short wNotifyInterval;  //上报间隔
} TPuGpsCfgParam;

// GPS通知信息
typedef struct
{
    unsigned long  dwTime;                 //UTC时间
    unsigned char  byAvailability;         //有效性
    char           chNSIndicator;          //南北纬度指示
    unsigned char  byLatitudeHigh;         //纬度，度
    unsigned long  dwLatitudeLow;          //纬度，百万分之一度
    char           chEWIndicator;          //东西经指示
    unsigned char  byLongitudeHigh;        //经度，度
    unsigned long  dwLongitudeLow;         //经度，百万分之一度
    unsigned short wSpeed;                 //速度，公里/小时
    unsigned short wSpeedLow;              //速度，千分之一公里/小时
    unsigned short wDirectionHigh;         //方向，度
    unsigned short wDirectionLow;          //方向，千分之一度
    unsigned long  dwAltitudeHigh;         //海拔，米
    unsigned char  byAltitudeLow;          //海拔，厘米
    unsigned char  bySatelliteNum;         //正在解算位置的卫星数量
} TPuGpsNotifyParam;


// 本地视频文件播放
typedef struct 
{
    unsigned long      m_dwGranuity;
    unsigned long      m_dwContext;
    PUSDK_FILE_STAT_CB m_pfCB;
    unsigned char      m_byFileType;   // PUSDK_FILE_TYPE_ASF
    char               m_achFileName[512];
}PUSDK_TPuPlayFileParam ;

// 前端录像下载请求
typedef struct  
{    
    unsigned long      dwRecFileIndexId;          // 前端录像文件索引值
    TPuSysTime         tStartTime;                // 下载文件开始时间
    TPuSysTime         tEndTime;                  // 下载文件结束时间
    char               achRecFilePath[MAX_PATH];  // 下载文件路径
    char               achSavePath[MAX_PATH];     // 本地保存路径
    PUSDK_DOWN_REC_CB  fRcvCallBack;              // 录像下载进度回调函数
    void*              pCallBackContext;          // 回调函数上下文参数
    unsigned short     wGran;                     // 录像下载进度回调粒度(单位:秒)
    PUSDK_DOWN_REC_DATA_CB fDownDataCB;           // 录像下载数据回调函数
    void*              pDownDataCBContext;        // 录像下载数据回调参数
}PUSDK_TPuRecFileDownloadReq;


// 前端注册回调函数指针
typedef void (*FUNC_CONNECTED_ACK)( PUHANDLE hHandle, TPuUnitInfo *ptPuInfo, void *pContext );


// 生产测试通讯
typedef struct
{
    unsigned short wType;       // 测试类型
    unsigned short wLen;        // 通讯数据长度
    char           abyBuf[64+1];// 通讯数据
}TProTestInfo;

// 版本升级 
typedef struct 
{   
    unsigned char    byFileType;                         // 需升级的文件类型 PUSDK_UPDATE_TYPE_ALL
    char             achFileInfo[64 +1];                 // 需升级的文件路径及文件名
    unsigned long    dwFtpIp;                            // FTP服务器地址
    unsigned short   wFtpPort;                           // FTP服务器端口 PUSDK_DEFAULT_FTP_PORT
    char             achFtpName[ 256 + 1 ];              // FTP用户名
    char             achFtpPassWord[ 256 + 1 ];          // FTP用户密码
} TPuVerUpdateInfo;

typedef struct
{ 
	char achCaption[MAX_CAPTION_NUM+1]; // 字幕内容
}TBmpCaption;

// 字幕内容字
typedef struct  
{
	int  nNum;                                 // 字幕行数
	int  nMaxLen;                              // 字幕内容每行最大使用长度
	TBmpCaption tCaption[MAX_BMP_CAPTION_NUM]; // 字幕内容
}TOsdBmpCaption, *PTOsdBmpCaption;

// 鉴权返回结果 
typedef struct 
{
    unsigned short wResult; // 错误码定义:EPuLoginPuError
} TPuUserPasswordIdentifyAck;


// 帧回调结构
typedef struct 
{
	unsigned char    m_byMediaType;   // 媒体类型
    unsigned char*   m_pbyData;       // 数据缓冲
    unsigned long    m_dwPreBufSize;  // m_pData缓冲前预留了多少空间，用于加
	// RTP option的时候偏移指针一般为12+4+12
	// (FIXED HEADER + Extence option + Extence bit)
    unsigned long    m_dwDataSize;    // m_pData指向的实际缓冲大小缓冲大小
    unsigned char    m_byFrameRate;   // 发送帧率,用于接收端
    unsigned long    m_dwFrameID;     // 帧标识，用于接收端
    unsigned long    m_dwTimeStamp;   // 时间戳, 用于接收端
    unsigned long    m_dwSSRC;        // 同步源, 用于接收端
    union
    {	
        struct{
			int              m_bKeyFrame;    // 频帧类型（I or P）
			unsigned short   m_wVideoWidth;  // 视频帧宽
			unsigned short   m_wVideoHeight; // 视频帧宽
		}m_tVideoParam;
        unsigned char        m_byAudioMode;  // 音频模式
    }x;
}TFrameHeadInfo, *pTFrameHeadInfo;
// 回调收到的一帧数据。
// pFrmHdr：指向一个FRAMEHDR结构体，该结构体用来描述该帧，包括宽高，是否关键帧，以及指向帧数据的指针等
// dwParam：用户的上下文参数
typedef void (*PUSDK_FRAME_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, TFrameHeadInfo* pFrmHdr, unsigned long dwContext );

//CMU 版本真数据回调
typedef void (*PUSDKCMU_FRAME_CB)( PUHANDLE hHandle, unsigned char byChanId, TFrameHeadInfo* pFrmHdr, void* dwContext );

//解码YUV数据回调
typedef void (*PUSDK_YUV_DATA_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, unsigned char* pBuf, unsigned long nBufSize, 
								   int nWidth, int nHeight, TFrameHeadInfo* pFrmHdr, unsigned long dwContext );

// 视频源状态
typedef struct 
{
    EEventType eState; // 状态
}TPuVideoInState;

// 移动侦测上报
typedef struct 
{
	unsigned short   wAlarmAreaNum; // 检测区编号(无效)
    EEventType       eState;        // 状态
} TPuMotionDetAlarm;

// 硬盘异常上报
typedef struct 
{
    unsigned short    wRecError; // 错误码:EPuRecError
} TPuRecErrorNotify;

// 报警输入上报
typedef struct 
{
    EEventType   eState; // 状态
} TPuInputPinAlarm;

// 串口上报结构
typedef struct 
{
    unsigned char     byComId;              // 串口通道号，一般只有一个串口，则为0
    int               dwReserved;           // 保留，后续可作为关联数据标识
    unsigned short    wMsgLen;              // 消息长度
    char              abyMsgBuff[256];
} TPuComData;

typedef struct
{
    EPu3gNetType e3gNetType;                  // 3G
    char achUsername[32];     // 用户名
    char achPassword[32];     // 密码
    char achPhoneNumber[16];  // 连接号码
} TPu3gNetParam;

typedef struct
{
    unsigned char	by3gNetNum;         // 3G网络个数
    TPu3gNetParam	at3gNetParam[5];
    EPu3gNetType	eCurUsedNetType;    // 当前使用的网络类型
} TPu3gNetCfg;

// for ipc
typedef struct
{
    unsigned short	wReciprocal;            // 当前的快门参数倒数
    unsigned short	wSupportListNum;        // 支持的快门参数列表数(只读)
    unsigned short	awSupportList[MAX_RECIPROCAL_LIST_NUM]; // (只读)
    unsigned long	dwReserved1;
    unsigned long	dwReserved2;
} TPuSnapConfig;

// for ipc
typedef struct
{
    unsigned short  byCmdType;
    unsigned long   dwValue;
} TPuCameraCtrl;

typedef struct 
{
  unsigned char      byConfigType;   // EVsipLoadDefualt
} TPuLoadDefaultConfig;

#endif

