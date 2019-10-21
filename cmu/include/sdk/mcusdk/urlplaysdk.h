#ifndef _RTSP_PLAY_SDK_H_
#define _RTSP_PLAY_SDK_H_

#ifndef IN
#define IN
#endif 

#ifndef OUT
#define OUT
#endif 

#ifndef INOUT
#define INOUT
#endif 

#ifdef WIN32
#ifdef URL_PLAY_SDK_EXPORTS
#define URL_PLAY_SDK_API  __declspec(dllexport)
#else//URL_PLAY_SDK_EXPORTS
#define URL_PLAY_SDK_API __declspec(dllimport)
#endif//URL_PLAY_SDK_EXPORTS
#define URLCALL __cdecl 

#else//WIN32
#define URLCALL 
#define URL_PLAY_SDK_API 
#endif//WIN32


//生成URL自身的 URL_GetLastInternalErrorCode 错误码
#define URLERROR(p) (URL_PLAY_ERROR)(p)
//生成UNIPLAY的 URL_GetLastInternalErrorCode 错误码
#define UNIERROR(p) (URL_PLAY_ERROR)(0x00100000|(p&0x000fffff))
//生成MVC的 URL_GetLastInternalErrorCode 错误码
#define MVCERROR(p) (URL_PLAY_ERROR)(0x00200000|(p&0x000fffff))
//生成MEDIANET的 URL_GetLastInternalErrorCode 错误码
#define MEDERROR(p) (URL_PLAY_ERROR)(0x00300000|(p&0x000fffff))
//生成MEDIASWITCH的 URL_GetLastInternalErrorCode 错误码
#define MSERROR(p) (URL_PLAY_ERROR)(0x00400000|(p&0x000fffff))

//判断URL_GetLastInternalErrorCode返回的内部错误码类型相关
#define IS_UNIPLAY_ERRORCODE(e) ((e>>20)==1)
#define IS_MVC_ERRORCODE(e) ((e>>20)==2)
#define IS_MED_ERRORCODE(e) ((e>>20)==3)
#define IS_MS_ERRORCODE(e) ((e>>20)==4)
//把错误码转成对应模块的内部错误码，不同模块的内部错误码是可能冲突的，
//所以转之前，你应该已经先判断出了这个错误码是哪个模块的错误码
#define TO_INTERNAL_ERRORCODE(e) (e&0x000fffff)

typedef void * URL_PLAY_HANDLE;
typedef void * URL_PLAY_WND;
typedef void * URL_PARAM;

//错误码
enum URL_PLAY_ERROR
{
	UPE_OK												=0,
	UPE_NOT_IMPLEMENT,//						=1,	//接口或功能未实现
	UPE_NOT_SUPPORT,//							=2,	//不支持该接口或功能
	UPE_TIME_OUT,//									=3,	//通用超时
	UPE_PLAY_HANDLE_EMPTY,//			=4,	//播放器ID为空
	UPE_PLAY_FAILED,//							=5,	//播放失败
	UPE_PARAM_INVALID,//					=6,	//参数有误
	UPE_CONNECT_SERVER_FAILED,// =7,//连接服务器失败（MVS必须要有登录才能用）
	UPE_CONNECT_SERVER_TIME_OUT,// =8,//连接服务器超时（MVS必须要有登录才能用）
	UPE_CREATE_MEDIANET_FAILED,//=9 创建medianet对象失败
	UPE_NOT_EXIST,								//=10 各种不存在的错误
	UPE_GET_DEC_STATIS_FAILED,	//=11获取解码统计信息失败
	UPE_FILE_NAME_IS_EMPTY,			//=12文件名为空
	UPE_SNAPSHOT_FAILED,				//=13抓拍失败
	UPE_START_LOCAL_RECORD_FAILED,				//=14开启本地录像失败
	UPE_STOP_LOCAL_RECORD_FAILED,				//=15停止本地录像失败
	UPE_ENABLE_REGION_ZOOM_FAILED,	//=16开启电子缩放失败
	UPE_DISABLE_REGION_ZOOM_FAILED,	//=17停止电子缩放失败
	UPE_REFRESH_REGION_FAILED,				//=18刷新区域失败
	UPE_SET_AUDIO_MUTE_FAILED,				//=19设置静音失败
	UPE_GET_AUDIO_MUTE_FAILED,				//=20获取静音失败
	UPE_SET_AUDIO_VOLUME_FAILED,		//=21设置音量失败
	UPE_GET_AUDIO_VOLUME_FAILED,		//=22获取音量失败

	UPE_OPEN_FILE_FAILED,							//=23打开文件失败
	UPE_GET_FILE_TOTAL_TIME_FAILED,		//=24获取录像文件总时长（毫秒）失败
	UPE_GET_FILE_TOTAL_FRAMES_FAILED,		//=25获取录像文件总帧数失败
	UPE_GET_FILE_PLAYED_TIME_FAILED,		//=26获取录像文件当前已播放时长（毫秒）失败
	UPE_GET_FILE_PLAYED_FRAMES_FAILED,		//=27获取录像文件当前已播放帧数失败

	UPE_SEEK_FAILED,									//=28,seek失败
	UPE_PAUSE_FAILED,								//=29,暂停失败
	UPE_RESUME_FAILED,							//=30，从暂停中恢复播放失败
	UPE_SINGLE_FORWARD_FAILED,		//=31，单帧进失败（注意，单帧操作需要先暂停播放才能进行，否则会失败）
	UPE_SINGLE_BACKWARD_FAILED,		//=32，单帧退失败
	UPE_FAST_PLAY_FAILED,						//=33，快速播放失败
	UPE_SLOW_PLAY_FAILED,						//=34，慢速播放失败
	UPE_MVC_PLAY_DISCONNECTED,		//=35，MVC的某路码流（设备下线）断链了,如果是事件上报，则param1为mvc的播放id，param2为保留
	UPE_HITCP_PLAY_DISCONNECTED = UPE_MVC_PLAY_DISCONNECTED,//35 都是断链，所以值一样

	UPE_RTSP_SET_URL_FAILED,				//36，RTSP设置URL失败了
	UPE_RTSP_START_PLAY_FAILED,		//37，RTSP开始播放失败了
	UPE_RTSP_GET_MULTICAST_PARAM_FAILED,	//38，RTSP获取组播参数失败
	
	UPE_MVC_GENERATE_SESSION_FAILED, //39，MVC获取新session失败
	UPE_UNIPLAY_PLUGIN_NOT_EXIST,				//40，uniplay的插件不存在
	UPE_UNIPLAY_OPEN_STREAM_FAILED,		//41，uniplay OpenStream失败
	UPE_UNIPLAY_NOT_EXIST,								//42，uniplay 自身不存在

	UPE_NO_FREE_PORT, //43,没有可用的端口了

	UPE_MEDIASWITCH_CREATE_FAILED,		//44，创建Mediaswitch失败
	UPE_MEDIASWITCH_SETPARAM_FAILED,	//45，设置Mediaswitch参数失败

	UPE_SET_WATERMARK_FAILED,				//46，设置水印失败
	UPE_CREATE_AUDIO_CAPTURE_FAILED,         //47,创建音频采集失败，确认麦克风
	UPE_HAVE_NO_FRAME,         //48,录像下载超过10s没有码流
	UPE_STREAM_VERIFY_FAILED,					//49，码流验签失败
};

//码流类型
enum EUrlStreamType
{
	StreamTypeNull=0,
	StreamTypeTCP,
	StreamTypeUDP,
	StreamTypeMultiCast,
};

//播放额外配置选项，定义的顺序不要改，只允许往后添加不能在中间插入
enum EUrlPlayCfgType
{
	ConfigTypeNull=0,							//未知		
	ConfigTypeFrameCallBack,		//设置音视频帧回调	[设置时参数：param1：为pfUrlFrameCallBack回调函数地址，param2：用户自定义数据]	
	ConfigTypeForceSoftDecode,	//设置强制软解标记 [设置时参数：param1：保留，param2：非0表示强制软解，否则表示默认（如果能硬解会优先硬解）]	
	ConfigTypeYUVCallBack,			//设置视频YUV数据回调	[设置时参数：param1：为pfUrlYUVCallBack回调函数地址，param2：用户自定义数据] 该回调会降低硬解解码的性能
	ConfigTypeEventCallBack,			//设置事件回调函数 [设置时参数：param1：为pfUrlEventCallBack回调函数地址，param2：用户自定义数据]	
	ConfigTypeManufactor,				//设置码流厂商名[设置时参数：param1：保留，param2：utf8字符串]	，不填代表默认是科达设备
	ConfigTypeAudioSampleRate,	//设置音频采样率[设置时参数：param1：保留，param2：采样率数值]
	ConfigTypeVideoSampleRate,	//设置视频采样率[设置时参数：param1：保留，param2：采样率数值]
	ConfigTypeUDPParam,				//更新UDP播放额外参数[设置时参数：param1：保留，param2：参数字符串，参数格式类似"manufactor=kedacom&localvideortpport=50000&localvideortcpport=50001&localaudiortpport=50002&localaudiortcpport=50003&rsflag=1&rs1=80&rs2=120&rs3=200&reject=400&remotevideortpport=40004&remotevideortcpport=40005&remoteaudiortpport=40006&remoteaudiortcpport=40007"]
	ConfigTypeYUVFrameInfoCallBack,	//与 ConfigTypeYUVCallBack 回调函数类型和设置方法完全一致，除了回调结构体了不带实际的YUV数据以外。当上层仅需要解码后数据的信息，而不是解码后的YUV数据时，尽量使用这个回调，当使用硬解码时与ConfigTypeYUVCallBack比，这会显著提升解码性能。
	ConfigTypeGPSDataCallBack,			//设置码流GPS数据回调函数 [设置时参数：param1：为pfUrlGPSDataCallBack回调函数地址，param2：用户自定义数据]	
	ConfigTypeEnableNAT,					//废弃		使用新的 ConfigTypeVideoNATData ConfigTypeAudioNATData
	ConfigTypeAddNATData,				//废弃		使用新的 ConfigTypeVideoNATData ConfigTypeAudioNATData
	ConfigTypeSupportLargeFrame,	//设置是否支持大帧（通常是指4K画面及以上的帧）[设置时参数：param1：bool，param2：保留]	，不填代表默认不支持
	ConfigTypeVideoNATData,			//设置视频NAT探测包信息，[param1为指向 URL_NATInfo 的指针， param2为sizeof(URL_NATInfo)] 必须在StartPlay之前设置好
	ConfigTypeAudioNATData,			//设置音频NAT探测包信息，[param1为指向 URL_NATInfo 的指针， param2为sizeof(URL_NATInfo)] 必须在StartPlay之前设置好
	ConfigTypeDrawCallBack,	//设置作画后的回调，[设置时参数：param1：为pfUrlDrawCallBack回调函数地址，param2：用户自定义数据]	
	ConfigType_MVS_Port,		//连接MVS的端口 [设置时参数：param1：为端口号，param2：保留]
	ConfigType_NO_Decode,	//不解码（但是码流回调还是要的）[设置时参数：param1：为bool,是否不解码，param2：保留]
	ConfigType_AllocUDPPort,			//申请UDP端口（4个连续的可用udp端口，返回第一个）[设置时参数：param1：为1表示用于1.0平台，0表示2.0平台，param2：指向int *的指针用于接收端口号]
	ConfigType_UseMediaswitch,		//设置播放码流接收是否采用mediaswitch方式。[设置时参数：param1：bool，param2：保留]	，不填代表默认不使用
	ConfigType_IsRecordPlay,	//由于UDP录像播放和UDP实施流播放对于urlplaysdk来说是没有区别的，所以，需要上层在UDP录像回放前手工设置这个标记。[设置时参数：param1：bool，param2：保留]	
	ConfigTypeVideoBufferTime,	//设置视频缓存时长。[设置时参数：param1：int(时常毫秒数)，param2：保留]	
	ConfigTypeAudioBufferTime,	//设置音频缓存时长。[设置时参数：param1：int(时常毫秒数)，param2：保留]	，如果音频不支持单设，则统一使用视频的参数值
	ConfigTypeStreamType,		//这里保存是否是PS流的标记 [设置时参数：param1：bool（是否是PS流），param2：保留]
	ConfigType_Safe_Mode,		//本路是否采用安全模式[设置时参数：param1：为bool,是否启用安全模式，param2：保留]	=====目前只是占位
	ConfigType_PubKey,		//.pem文件名[设置时参数：param1：为utf8 const char *指针，表示.pem文件全路径，param2：为utf8 const char *指针,表示保护密码]	=====目前只是占位
	ConfigTypeWaterMark,		//设置本地播放图像加入水印，[param1为指向 URL_WaterMark 的指针， param2为sizeof(URL_WaterMark)] 必须在StartPlay之前设置好]
	ConfigType_IsRecordDownload,	//如果是录像下载，设置一个标记，内部可以做一些特别处理[设置时参数：param1：bool，param2：保留]	
	ConfigType_RecFile_AudioTrackNum,	//获取录像文件里的音轨数。[获取时参数：param1：(int *)，param2：保留]
	ConfigType_FirstTimeStamp,					//录像回放的时候，通过这个参数告诉urlplaysdk，本次播放录像【文件的第一帧】时间戳[param1:时间戳, param2:保留]
	ConfigType_Device_Cert,	//设备的签名和加密证书，[设置时参数：param1：为utf8 const char *指针表示签名证书原始内容，param2：为utf8 const char *指针表示密钥证书原始内容]
	ConfigType_Base_UserDef=0x400,		//用户自定义配置,这个值以及大于这个值的类型作为用户自定义的配置数据
	ConfigType_Userdef_PlayID,						//把Mcusdk分配的PlayID也存起来很多地方用得到
	ConfigType_Userdef_Frame_CB,				//Mcusdk的 Frame信息回调的两个参数
	ConfigType_Userdef_YUVData_CB,			//Mcusdk的 YUV数据回调的两个参数
	ConfigType_Userdef_YUVInfo_CB,			//Mcusdk的 YUV信息回调的两个参数
	ConfigType_Userdef_EventCallBackInfo,	//本工程里表示通过param1和param2传递Mcusdk自己的事件回调函数指针和用户参数
	ConfigType_Userdef_IsKeyframeReported,	//本工程里表示通过param2传递一个bool型，表示首个关键帧到来事件是否已经上报过	
	ConfigType_Userdef_Draw_CB,			//Mcusdk的 DrawCallBack信息回调的两个参数
	ConfigType_MaxRecFileSize,					//设置本地录像的最大文件大小（MB） [设置时参数：param1：int（以MB为单位的大小），param2：保留]
	ConfigTypeAudioCallBack,		//设置音频呼叫时的帧回调	[设置时参数：param1：为 pfUrlAudioPowerCallBack 回调函数地址，param2：用户自定义数据]
	ConfigType_IsRecordReversePlay,	//如果是录像倒放，设置一个标记，内部可以做一些特别处理[设置时参数：param1：bool，param2：保留]
	ConfigType_RecordDownloadTimeoutValue, //设置录像下载无码流超时结束时间 [设置时参数：param1：int（超时时间单位毫秒），param2：保留]
	ConfigTypeRSParamData, //设置重传参数信息，[param1为指向 URL_RSParam 的指针， param2为sizeof(URL_RSParam)]
};

//播放额外控制类型
enum EUrlPlayCtrlType
{
	ControlTypeNull=0,	//
	ControlTypeDecStatis,					//获取解码统计信息  [参数：param1：为指向已分配 URL_DecStatis 的指针，param2：URL_DecStatis的大小sizeof(URL_DecStatis)]
	ControlTypeSnapshot,					//抓拍图片 					[参数：param1：为ESnapshotFormat，param2：utf8格式的图片文件全路径]
	ControlTypeStartLocalRecord,	//开启本地录像			[参数：param1：保留填0，param2：utf8格式的录像文件全路径，根据扩展名自动决定录像格式]
	ControlTypeStopLocalRecord,	//停止本地录像			[参数：无]
	ControlTypeEnableRegionZoom,			//开启将指定区域缩放到另外的窗口 [参数：param1：指向 URL_RegionZoomInfo 指针，param2：URL_RegionZoomInfo 大小sizeof(URL_RegionZoomInfo)]
	ControlTypeDisableRegionZoom,			//停止将指定区域缩放到另外的窗口 [参数：param1：为区域号，param2：保留]
	ControlTypeRefreshRegion,						//相当于drawlastframe，但是是对指定区域进行的 [参数：param1：为区域号（0表示刷新全部），param2：保留]
	ControlTypeSetMute,							//设置静音			[参数：param1：1表示静音，0表示取消静音, param2：保留]
	ControlTypeGetMute,							//获取静音标记，uniplay还没这样的接口！！！！[参数：param1指向int的指针，用来返回静音标记，param2：保留]
	ControlTypeSetVolume,						//设置音量			[参数：param1：int类型的音量值[0,0xffff]，param2：保留]
	ControlTypeGetVolume,						//获取音量			[参数：param1：指向int类型的指针，用来返回音量值，param2：保留]
	ControlTypeGetPlayedTime,				//获取当前已经播放的时间单位毫秒[获取时参数：param1：指向uint32_t类型的指针，用来取回已播放时长（毫秒），param2：保留0）]
	ControlTypeGetPlayedFrames,			//获取当前已经播放的帧数[获取时参数：param1：指向uint32_t类型的指针，用来取回已播放帧数，param2：保留0）]
	ControlTypeGetFileTotalTime,				//获取录像文件总时间单位毫秒[获取时参数：param1：指向uint32_t类型的指针，用来取回总时长(毫秒），param2：保留0]
	ControlTypeGetFileTotalFrames,		//>>>该接口目前不可用<<<  获取录像文件总帧数[获取时参数：param1：指向uint32_t类型的指针，用来取回总帧数，param2：保留0]
	ControlTypeSeek,							//跳转指定位置		[参数：param1：相对时间单位毫秒，param2：保留0]
	ControlTypePause,						//暂停或恢复暂停的播放[参数：param1：bool型，1表示暂停，0表示恢复，param2：保留0]
	ControlTypeSingleForward,			//单帧播放，每调用一次播放一帧	[参数保留]
	ControlTypeSingleBackward,				//单帧倒放，每调用一次倒放一帧 [参数保留]
	ControlTypeFastPlay,					//快放，[参数：param1：倍数值1,2,4,8,16等分别表示需要加快的播放倍数，param2：保留0]
	ControlTypeSlowPlay,					//慢放，[参数：param1：倍数分母值1,2,4,8,16等分别表示需要减小到X分之一速，param2：保留0]
	ControlTypeReserve1,					//暂时用不到了
	ControlTypeDrawOneFrame,			//这个是专门给IOS下使用的，在界面需要update的时候，调用一次，以便解码图像能够更新出来。[参数：无]
	ControlTypeDropAudio,					//丢弃音频数据包[参数：param1：bool型，1丢弃，0表示恢复，param2：保留0]，不论播放还是本地录像，都将没有声音。
	ControlTypeStartMemoryRecord,	//开始内存录像[参数：param1：保留0，param2：保留0]
	ControlTypeStopMemoryRecord,	//停止内存录像[参数：param1：保留0，param2：保留0]
	ControlTypeSaveMemoryRecord,	//将内存录像保存到本地文件 [参数：param1：保留填0，param2：utf8格式的录像文件全路径]
	ControlTypeGetMemoryRecordBufferSize,	//获取内存录像已经缓存的帧数量， [参数：param1：指向int *的指针用来返回帧数值，param2：保留]
	ControlTypeDropADPCM,		//播放时丢弃ADPCM混音。[设置时参数：param1：bool(是否丢弃标记)，param2：保留]	，但是对码流本地录像时还是会将音频录下来。
	ControlTypeVerticalSync,			//设置视频垂直同步。[设置时参数：param1：bool(是否启用标记)，param2：保留]	
	ControlTypeAntiRip,					//设置视频反错位。	 [设置时参数：param1：bool(是否启用标记)，param2：保留]		
	ControlTypeSmooth,					//设置视频平滑处理。[设置时参数：param1：bool(是否启用标记)，param2：保留]	
	ControlTypeSelectAudioStreamID,	//当有多音频流时，选择要使用的流ID号。[设置时参数：param1：int(流ID号)，param2：bool(是否启用)]	，如果启用了，则仅使用选择的这个音频流，否则全部启用
	ControlTypeStartSend,	//开始将本路码流转发到另外一个地址， 注意本路码流应该已经开始接收解码。[设置时参数：param1：URL_SenderAddrInfo *，param2：sizeof(URL_SenderAddrInfo)]	
	ControlTypeStopSend,	//停止转发 [设置时参数：param1：保留0，param2：保留0]	
	ControlTypeRestartMVCPlay, //不关闭uniplay重新请求一路mvc播放（这个功能主要用于录像下载）[设置时参数：param1：const char *url：新请求的mvc录像回放url，param2：不为0表示下载倍速]	
	ControlTypeResetNewPlay,		//不释放playhandle，重置各种与播放相关的标记参数（比如时间戳）[设置时参数：param1：保留，param2：保留]	
	ControlTypeSetKeyFramePlay, //设置是否关键帧解码（针对文件播放），[设置时参数：param1：bool 开/关，param2：保留]	
	ControlTypeReversePlay,				//设置uniplay是否开启倒放模式，[设置时参数：param1：bool 开/关，param2：uint32 *: 同时返回uniplay返回本段录像当前已经播放了的相对时间毫秒数
	ControlTypeClearUniplayBuffer,	//清一下uniplay的播放缓存（比如录像播放seek后需要调用一下）[设置时参数：param1：保留，param2：保留]	
	ControlTypeSetCapVolume,		//设置采集音量[param1:音量值]
	ControlTypeGetCapVolume,		//获取采集音量[param1:音量值指针]
	ControlTypeSetCapMute,				//设置采集哑音状态[param1:哑音bool]
	ControlTypeGetCapMute,			//获取采集哑音状态[param1:哑音bool指针]
	ControlTypeVideoBufToFile,		//将yuv转成图片[param1:指向 URL_VideoBufToFileReq 结构的指针， param2:保留]
	ControlTypePauseDecode,			//暂停或恢复解码，仅本地解码，不影响码流传输[参数：param1：bool型，1表示暂停，0表示恢复，param2：保留0]
};


//事件上报类型
enum EUrlEventType
{
	EventTypeNull = 0,
	EventTypePlay,			//播放结果通知(失败)
	EventTypeUPDReady,	//UDP播放本地已经准备好接收码流
	EventTypeRecordPlayEnd,//录像回放结束通知
	EventTypeRecordPlayBufferEmpty,		//单帧录像回放码流缓存为空，param1：缓存还剩余的帧数，此时上层应该让服务端发送码流过来
	EventTypeRecordPlayBufferOverflow,	//单帧录像回放码流缓存溢出，param1：缓存还剩余的帧数，此时上层应该停止服务端发送码流过来
	EventTypeMultiAudioStream,		//当发现码流存在多个音频流时会上报次事件，param1：第一个收到的音频流ID，param2第二个收到的音频流ID。
	EventTypeUniplay,						//uniplay新增的错误码回调，param1：参看 EUniplayModule，param2：参看uniplay的errorcode.h,KDVD_ERR_IN_BUF_FULL表示性能不足
	EventTypeMediaSender,				//所有与发送媒体相关的错误码， param1暂时还没用，param2就是medianet/mediaswitch返回的错误码
	EventTypeNoVkek,			//当前浏览的码流没有vkek
};

//视频帧类型
enum EVideoFrameType
{
	VFrameTypeI= 0,				//是关键帧
	VFrameTypeP,
	VFrameTypeB,
};

//YUV原始数据回调里的帧类型
enum  ERawFrmType
{
	RFrameVideo = 0,				//是视频
	RFrameAudio,						//是音频
	RFrameDirectVideo,			//????
	RFrameUnknow = 255,		//未知
};

//抓拍图片类型
enum ESnapshotFormat
{
	SFormatBMP32 = 0,		//BMP32类型
	SFormatJPEG,				//JPEG类型
	SFormatJPEG_70,			//70%质量的JPEG类型
	SFormatJPEG_50,			//50%质量的JPEG类型
	SFormatJPEG_30,			//30%质量的JPEG类型
	SFormatJPEG_10,			//10%质量的JPEG类型
	SFormatBMP24,            //BMP24类型
} ;

//uniplay错误码回调模块类型
enum EUniplayModule
{
	UMDEFAULT = 0,		// 
	UMVIDEODEC = 2,	// 视频解码模块
	UMVIDEOPLAY = 3,	// 视频显示模块
	UMVIDEOREENC = 4,	// 视频重编码模块
	UMVIDEOENC = 5,	// 视频编码模块
	UMVIDEOCAP = 6,	// 视频采集模块
	UMAUDIODEC = 7,	// 音频解码模块
	UMAUDIOPLAY = 8,	// 音频播放模块
	UMAUDIOCAP = 9,	// 音频采集模块
	UMAUDIOENC = 10,	// 音频编码模块
};


//帧回调里的帧结构体
typedef struct tagURL_FrameInfo
{
	void *pData;       //数据缓冲
	unsigned int dwPreBufSize;//pData缓冲前预留了多少空间，用于加// RTP option的时候偏移指针一般为12+4+12	// (FIXED HEADER + Extence option + Extence bit)
	unsigned int dwMediaEncode; //帧编码类型MPEG4(97), H264(106),H265(111),ADPCM(5)
	unsigned int dwDataSize;  //pData指向的实际缓冲大小缓冲大小
	unsigned int dwFrameID;   //帧标识，用于接收端
	unsigned int dwTimeStamp; //时间戳, 用于接收端
	unsigned int dwSSRC;      //同步源, 用于接收端
	unsigned long long qwRawTimeStamp;		//绝对时间戳（视频上面时间）
	union
	{
		struct
		{			
			EVideoFrameType FrameType;    //频帧类型
			unsigned int dwFrameRate;		//发送帧率,用于接收端
			unsigned int dwVideoWidth;  //视频帧宽
			unsigned int dwVideoHeight; //视频帧宽
		}tVideoParam;
		struct
		{
			unsigned int dwChannel;		//音频通道
			unsigned int dwSampleRate;	//音频采样率
			unsigned int dwSamplebit; //音频采样位数
		}tAudioParam;		
	};	
	unsigned int dwAudioMode;	//额外保存老的音频参数（为兼容）
}URL_FrameInfo;
//YUV回调里的帧原始数据结构体
typedef struct tagURL_RawFrmInfo
{
	void * pbyRawBuf;
	unsigned int dwRawlen;
	ERawFrmType dwMediaType;					//数据类型(ERawFrmType)
	EVideoFrameType dwSubMediaType;		//当 dwMediaType 为 视频时 RFrameVideo 有效
	unsigned int dwFrameIndex;			//帧序号
	unsigned long long dwNetTimeStamp;			//相对时间戳（RTP内部时间戳）
	unsigned long long dwMediaTimeStamp;		//绝对时间戳（视频上面时间）
	unsigned int dwMediaEncode;			//在kdvdef.h 内部定义或者FRAME_DATA_FORMAT里面定义
	union
	{
		struct
		{
			unsigned int		dwFrameRate;
			unsigned int		dwWidth;     //画面宽
			unsigned int		dwHeight;    //画面高
			unsigned int		strike[3];
		}tVideo;

		struct
		{
			unsigned int		dwChannels;   //声道数
			unsigned int		dwBitWidth;   //位宽
			unsigned int		dwSampleRate; //采样率
		}tAudio;
	};
	unsigned int		dwBitRate;			//比特率，单位为1028字节
	unsigned int		dwReverse[3];
}URL_RawFrmInfo;

typedef struct tagURL_RegionZoomInfo
{
	int nRegionIndex;	//区域号（从0开始，最多6个[0,5]，其中0又表示源区域，行为会有差异，建议最好不要用0，所以索引号从1开始作为新投放区域的索引号）
	//从原播放窗口上取得的一个窗口区域作为图像源（取完后原窗口大小变化了，也会实时的从新原窗口的这个区域取图像）
	struct 
	{
		int left;
		int top;
		int right;
		int bottom;
	}Rect;
	//需要将取得的这块区域播放到的窗口句柄
	URL_PLAY_WND hDestWnd;
}URL_RegionZoomInfo;
/*解码器统计信息*/
typedef struct tagURL_DecStatis
{
	unsigned int		dwVideoFrameRate;	/*视频解码帧率*/
	unsigned int		dwVideoBitRate;   	/*视频解码码率*/
	unsigned int		dwAudioBitRate;   	/*语音解码码率*/
	unsigned int		dwVideoRecvFrame;	/*收到的视频帧数*/
	unsigned int		dwAudioRecvFrame;	/*收到的语音帧数*/
	unsigned int		dwVideoLoseFrame;	/*丢失的视频帧数*/
	unsigned int		dwAudioLoseFrame;	/*丢失的语音帧数*/
	unsigned int		dwVideoLoseRatio;	//视频丢失率,单位是% 	
	unsigned int		dwAudioLoseRatio;	//音频丢失率,单位是% 	
	unsigned int		dwPackError;		/*乱帧数*/
	unsigned int		dwIndexLose;		/*序号丢帧*/
	unsigned int		dwVideoSizeLose; 	/*大小丢帧*/
	unsigned int		dwVideoFullLose; 	/*满丢帧*/
	unsigned int		dwAudioSizeLose; 	/*大小丢帧*/
	unsigned int		dwAudioFullLose; 	/*满丢帧*/
	unsigned int		dwAvrVideoBitRate;   /*1分钟内视频解码平均码率*/
	unsigned int	  	dwAvrAudioBitRate;   /*1分钟内语音解码平均码率*/
	bool                bVidCompellingIFrm;  /*视频强制请求I帧*/
	unsigned int		dwDecdWidth;		/*码流的宽*/
	unsigned int		dwDecdHeight;   	/*码流的高*/
	unsigned int        dwCodecType;        /*解码类型,soft:0;hw:1;yuvcb:2;*/
	unsigned int        dwDrawMode;         /*绘图方式,d3d:0;ddraw:1;drawdib:2;direct:3*/
	bool                bIsSafeFrame;       /*是否安全码流*/
}URL_DecStatis; 

/*UDP播放时设置NAT探测包数据*/
typedef struct tagURL_NATData
{
	unsigned int dwRemoteAddr;	//探测包发往的目的地
	int nRemotePort;						//探测包发往的目的端口
	unsigned int dwLocalAddr;		//作为本机的地址
	int nLocalPort;							//作为本机的端口
	void *pNATPkg;							//探测包内容
	int nPkgLen;								//探测包内容长度字节数
}URL_NATData;

//音频或视频探测包所需的全部信息
typedef struct tagURL_NATInfo
{
	int nInterval;	//探测包发送间隔（秒）
	URL_NATData rtpData;
	URL_NATData rtcpData;
}URL_NATInfo;

//重传参数结构体
typedef struct tagURL_RSParam
{
	int  m_wFirstTimeSpan;      //第一个重传检测点
	int  m_wSecondTimeSpan;     //第二个重传检测点
	int  m_wThirdTimeSpan;      //第三个重传检测点
	int  m_wRejectTimeSpan;     //过期丢弃的时间跨度
	bool m_bRSFlag;             //是否开启丢包重传
}URL_RSParam;

typedef struct tagURL_SenderAddrInfo
{	
	int LocalVidRtpPort;
	int LocalVidRtcpPort;
	int LocalAudRtpPort;
	int LocalAudRtcpPort;
	char LocalStreamIP[16+1];
	int RemoteVidRtpPort;
	int RemoteVidRtcpPort;
	int RemoteAudRtpPort;
	int RemoteAudRtcpPort;
	char RemoteStreamIP[16+1];
}URL_SenderAddrInfo;

//给本路码流增加水印
typedef struct tagURL_WaterMark
{
	unsigned int eWMMode;	   //选择添加水印模式，可以用 或运算 一次设置
	bool         bEnable;      //是否开启水印;默认关闭，TRUE开启
	unsigned int uiARGB;       //字体颜色和透明度0xAARRGGBB
	unsigned int uiFontSize;   //字体大小
	unsigned int uiFontRotate; //旋转角度【0~360】
	unsigned int uiLayerMode;  //1：平铺，类似“井”字形均匀平铺，不会拉伸文字。文字太长，将被截断。
	char *strText;             //utf8文字内容，由于uniplay的限制，实际只能传递最多60个字母或30个汉字，超过的会被截断
	char *pstrFontPath;        //utf8字库路径，如果为NULL，内部会选择一个默认的字库
	unsigned int uiWinWidth;//水印窗口宽高
	unsigned int uiWinHeight;//水印窗口宽高
	bool         bAutoAdjustFont;//是否根据窗口自动调整字体大小 保证水印排布一致
}URL_WaterMark;

//yuv转jpeg/bmp请求参数
typedef struct tagURL_VideoBufToFileReq
{
	unsigned char *pBuf;
	int nBufSize;
	int width;
	int height;
	int videotype;
	char *savefilename;
	int filetype;//0:bmp,1:jpeg
}URL_VideoBufToFileReq;

/* 该回调函数的声明定义都直接复制和透传给uniplay
pAddDrawDC 是一个结构体
win下：
typedef struct
{
void *hdc;
void *hwnd;
void *rendertype;//渲染类型，D3D直接显示，D3D显示，DDRAW显示，DRAWDIB显示。
}DrawDC;
*/
//画完一帧后的回调，第二个参数是回调上来的参数，因为是透传uniplay的，所以它的含义由uniplay决定，参看上面的定义
typedef int(URLCALL *pfUrlDrawCallBack)(URL_PLAY_HANDLE, void *, void *);

//帧回调函数类型, 返回值，请务必返回0，其他值未来可能会有扩展
typedef int (URLCALL *pfUrlFrameCallBack)(URL_PLAY_HANDLE, URL_FrameInfo *, void *);

//YUV(包括音频pmc)回调函数类型，返回值，请务必返回0，其他值未来可能会有扩展
typedef int (URLCALL *pfUrlYUVCallBack)(URL_PLAY_HANDLE, URL_RawFrmInfo *, void *);

//码流GPS解析回调函数
typedef int (URLCALL *pfUrlGPSDataCallBack)(URL_PLAY_HANDLE, void * /*pGpsDataBuf*/, int /*nDataLen*/, void *);

//事件上报回调函数,句柄，事件类型, 错误码，保留参数1，保留参数2，userdata
typedef int (URLCALL *pfUrlEventCallBack)(URL_PLAY_HANDLE, EUrlEventType, URL_PLAY_ERROR, URL_PARAM, URL_PARAM, void *);

//语音呼叫回调函数，直接按照mcusdk对外接口的回调类型来设置了，省事
typedef void(*pfUrlAudioPowerCallBack)(unsigned short, void *);

#ifdef __cplusplus
extern "C"
{
#endif

//Uniplay,MVC,以及所有下面使用的库都有Init,和DeInit,Init我可以在调用urlplaysdk的任何接口后调用一次,但是DeInit,就没有任何办法了,所以只能为他们加这两个函数
//函数内部会使用引用计数,只要成对调用就行. 为了兼容老的,即便不调用这两个接口,也能使用.只是所有库函数的DeInit函数就不会再调了,原先是放在一个全局静态对象
//CGlobalInit 的析构里调用,但是测试发现,这个时机实在是太迟了,某些线程都已经被操作系统强制退出了,才开始调用这个全局静态对象的析构,造成一些问题.
URL_PLAY_SDK_API void  URLCALL URL_Init();
URL_PLAY_SDK_API void  URLCALL URL_DeInit();

URL_PLAY_SDK_API URL_PLAY_HANDLE URLCALL URL_CreatePlayID();
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_StartPlay(IN URL_PLAY_HANDLE playHandle, IN const char *utf8_url, IN EUrlStreamType streamType, IN URL_PLAY_WND playWnd);
URL_PLAY_SDK_API void URLCALL URL_StopPlay(IN URL_PLAY_HANDLE playHandle);
URL_PLAY_SDK_API void URLCALL URL_ReleasePlayID(IN URL_PLAY_HANDLE playHandle);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_SetConfig(IN URL_PLAY_HANDLE playHandle,  IN EUrlPlayCfgType cfgType, IN URL_PARAM param1, IN URL_PARAM param2);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_GetConfig(IN URL_PLAY_HANDLE playHandle,  IN EUrlPlayCfgType cfgType, INOUT URL_PARAM *pParam1Out, INOUT URL_PARAM *pParam2Out);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_PlayControl(IN URL_PLAY_HANDLE playHandle, IN EUrlPlayCtrlType ctrlType, INOUT URL_PARAM param1, INOUT URL_PARAM param2);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_SetLogFile(IN bool bEnable, IN const char *utf8_filename);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_SetLogFileV2(IN int fileLev, IN const char *utf8_filename, IN const char* utf8_logname, IN int maxSizeKB, IN int maxFiles);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_SetLogLevel(IN int urlplaysdkLev, IN int uniplayLev);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_SetLogScreen(IN bool bEnable);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_SetLogScreenV2(IN int screenLev);
URL_PLAY_SDK_API unsigned int URL_GetLastInternalErrorCode(IN URL_PLAY_HANDLE playHandle);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_GetDecoderPluginVer(IN const char *utf8_manufactor, OUT char *outVerBuf, IN int bufSize);
URL_PLAY_SDK_API URL_PLAY_ERROR URLCALL URL_CreateSomePlayResource(IN URL_PLAY_HANDLE playHandle, IN bool bNeedNat);
#ifdef __cplusplus
}
#endif

#endif

