/*========================================
模块名	： KdmMtIntTest.h
文件名	： KdmMtIntTest.h
相关文件	： MtCtrl.h,kdvdef.h
实现功能 ： 编解码器集成测试接口
作者	： 凌宏强
版本	： V3.0
日期    	： 2005.04.14
修改记录:
日  期     版本       修改人      修改内容
===========================================*/
#ifndef _MT_INTEGRATION_TEST_H
#define _MT_INTEGRATION_TEST_H

#include "osp.h"
#include "kdvdef.h"
#include "kdvtype.h"
#include "mtctrl.h"

#define VER_MT_TESTLIB "testlib30.10.01.03.00.050414"
API void testlibver();

// 指针判断
#define MT_ASSERT( p ) if ( p == NULL ) { ::OspPrintf( TRUE, FALSE, "[SYSTEM]Invalid Pointer(%s:(%d))!\n", __FILE__, __LINE__ ); return; }
#define MT_ASSERT_RET( p, ret ) if ( p == NULL ) { ::OspPrintf( TRUE, FALSE, "[SYSTEM]Invalid Pointer(%s:(%d))!\n", __FILE__, __LINE__ ); return ret; }

#ifndef _VXWORKS_
#pragma pack(push)
#endif
#pragma pack(1)



//netsend dest addr max num

#define IT_MAX_NETSND_DEST_NUM                   5 

//MSG
#define MSG_IT_MT_CMD                            55//终端集成测试模块向终端发送获取状态或参数的命令消息

//detail state
#define SYS_DEVICE_TYPE							1 //获取设备类型
#define SYS_TIME_GET		              		2 //获取系统时间
#define STATE_CODEC_VENC          				3 //获取视频编码通道状态
#define STATE_CODEC_VDEC						4 //获取视频解码通道状态
#define STATE_CODEC_AENC						5 //获取音频编码通道状态
#define STATE_CODEC_ADEC						6 //获取音频解码通道状态
#define STATE_PERI_SERI	             			7 //获取串口状态
#define STATE_PERI_PARIN 						8 //获取并口输入状态
#define STATE_CODEC_VPOLL						9 //获取视频编码轮询状态
#define STATE_CODEC_VMP							10 //获取视频编码多画面状态
#define STATE_CODEC_VDECRF						11 //获取视频解码重传状态
#define STATE_CODEC_VENCMD						12 //获取视频编码移动侦测状态
#define STATE_CODEC_VDECMD						13 //获取视频解码移动侦测状态
#define STATE_OSD_VENC 							14 //获取视频编码字幕状态
#define STATE_CODEC_VINPUT						15 //获取视频源输入端口状态

//detail param
#define PARAM_SYS_CAPABILITY					100 //获取设备能力集
#define PARAM_SYS       						101 //获取系统参数结构
#define PARAM_SYS_PUB							102 //获取系统公共参数结构
#define PARAM_CODEC_VENCNET						103 //公共网络参数结构
#define PARAM_CODEC_VDECNET						104 //公共网络参数结构
#define PARAM_CODEC_AENCNET						105 //公共网络参数结构
#define PARAM_CODEC_ADECNET						106 //公共网络参数结构
#define PARAM_CODEC_VENC						107 //视频编码参数结构
#define PARAM_CODEC_VENCPUB						108 //视频编码公共参数结构
#define PARAM_OSD_VENC							109 //视频编码字幕参数结构
#define PARAM_CODEC_VMP							110 //视频编码多画面参数结构
#define PARAM_CODEC_VENPORT         			111 //视频编码视频源端口参数结构
#define PARAM_CODEC_VPOLL						112 //视频编码轮询参数结构
#define PARAM_CODEC_VDEC						113 //视频解码参数结构
#define PARAM_CODEC_VDECPUB						114 //视频解码公共参数结构
#define PARAM_CODEC_VDECRF						115 //视频解码重传参数结构
#define PARAM_CODEC_AENC						116 //音频编码参数结构
#define PARAM_CODEC_ADEC						117 //音频解码参数结构
#define PARAM_PERI_SERI							118 //串口参数结构
#define PARAM_PERI_SERIPUB						119 //串口公共参数结构
#define PARAM_PERI_PARIN						120 //并口输入参数结构
#define PARAM_CODEC_VINPORT						121 //视频源输入端口参数结构
#define PARAM_CODEC_VENCMD						122 //视频编码移动侦测参数结构
#define PARAM_CODEC_VDECMD						123 //视频解码移动侦测参数结构

#define PARAM_PPPOE                             124 //获取PPPOE参数结构

// 状态
#define INTTEST_TRUE							1
#define INTTEST_FALSE							0

// 板卡类型(0:KDM2400;1:KDM2500;2:KDM2418;3:KDM2518;4:KDM2418A;5:KDV8010A;6:KDV8010C;7:DEC5)
#define KDM_BRD_2400	0
#define KDM_BRD_2500	1
#define KDM_BRD_2418	2
#define KDM_BRD_2518	3
#define KDM_BRD_2418A	4
#define KDM_BRD_8010A	5
#define KDM_BRD_8010C	6
#define KDM_BRD_DEC5	7


#ifdef WIN32
#pragma pack( push )
#pragma pack(1)
#define PACKED 
#endif //WIN32
#ifdef _VXWORKS_
#define PACKED __attribute__((__packed__))
#endif //_VXWORKS_
#ifdef _LINUX_
#define PACKED 
#endif //_LINUX_

/////////////////Common group//////////////////////
//网络参数（可以是组播地址或是本地接收地址）
typedef struct tagTKDMNetAddr
{
	u32 	dwRtpIP;  //网络序
	u16 	wRtpPort; //
	u32		dwRtcpIP; //网络序
	u16 	wRtcpPort;//
}PACKED IT_TNETADDR,*IT_PTNETADDR;

//时间格式
typedef TMTI_SYS_TIME    IT_SYSTIME,*IT_PSYSTIME;
//设备能力集
typedef TMTI_CAPABILITY  IT_CAPABILITY,*IT_PCAPABILITY;

/////////////////CodecManage group//////////////////////
//所有编解码通道状态结构(0:关闭,1:打开)
typedef struct tagChannelState
{
	u8	abyVideoSend[MAX_VIDEO_ENC_NUM];//视频发送通道状态
	u8	abyVideoRecv[MAX_VIDEO_DEC_NUM];//视频接收通道状态
	u8	abyAudioSend[MAX_AUDIO_ENC_NUM];//音频发送通道状态
	u8	abyAudioRecv[MAX_AUDIO_DEC_NUM];//音频接收通道状态
}PACKED IT_CHANSTATE,*IT_PCHANSTATE;

//视频编码通道状态
typedef struct tagVEncState
{
	u8				byVideoEnChannel;		//视频编码通道号
	u8				byVideoPort;			//对应视频输入端口号
	u8				byVideoType;			//视频编码类型(MTI_VIDEO_TYPE_MPEG4等)
	u8				byVideoFormat;			//视频编码格式(MTI_VIDEO_RESOLUTION_CIF等)
	u8				byVideoFrameRate;		//视频编码帧率(1-25)
	u16				wVideoBitRate;			//视频编码码率
	u8				byyVideoImageQulity;	//视频编码压缩质量(1-5)
	u8				byVideoEncoding;		//是否视频编码(INTTEST_TRUE/INTTEST_FALSE)
	u8				byVideoSending;			//是否发送视频(INTTEST_TRUE/INTTEST_FALSE)
	IT_TNETADDR		tLocalNetParam;			//本地网络参数
	IT_TNETADDR		atDestNetParam[IT_MAX_NETSND_DEST_NUM];//发送目的地网络参数,最大为5个
}PACKED IT_VENCSTATE,*IT_PVENCSTATE;

//视频解码通道状态
typedef struct tagVDecState
{
	u8				byVideoDecChannel;		//视频解码通道号
	u8				byVideoPort;			//对应视频输出端口号
	u8				byVideoDecoding;		//是否视频解码(INTTEST_TRUE/INTTEST_FALSE)
	u8				byVideoRecving;			//是否接收视频(INTTEST_TRUE/INTTEST_FALSE)	
	IT_TNETADDR		tLocalNetParam;			//本地接收网络参数
}PACKED IT_VDECSTATE,*IT_PVDECSTATE;

//音频编码通道状态
typedef struct tagAEncState
{
	u8			byAudioEncChannel;  //音频编码通道号
	u8			byAudioFormat;	    //音频格式(MTI_AUDIO_TYPE_PCMA等)
	u8			byEncVol;			//编码音量
	u8			byMute;				//是否哑音(INTTEST_TRUE/INTTEST_FALSE)
	u8			byAudioEncoding;    //是否音频编码(INTTEST_TRUE/INTTEST_FALSE)
	u8			byAudioSending;     //是否发送音频(INTTEST_TRUE/INTTEST_FALSE)
	u8			byAecEnable;	    //是否开启回声抵消(INTTEST_TRUE/INTTEST_FALSE)
	IT_TNETADDR tLocalNetParam;		//本地网络参数
	IT_TNETADDR atDestNetParam[IT_MAX_NETSND_DEST_NUM];//发送目的地网络参数,最大为5个
}PACKED IT_AENCSTATE,*IT_PAENCSTATE;

//音频解码通道状态
typedef struct tagADecState
{
	u8      byAudioDecChannel;		//音频解码通道号
	u8      byDecVol;				//解码音量
	u8		byQuiet;	            //是否静音(INTTEST_TRUE/INTTEST_FALSE)
	u8		byAudioDecoding;		//是否音频解码(INTTEST_TRUE/INTTEST_FALSE)
	u8		byAudioRecving;			//是否接收音频(INTTEST_TRUE/INTTEST_FALSE)
	IT_TNETADDR  tLocalNetParam;	//本地接收网络参数
}PACKED IT_ADECSTATE,*IT_PADECSTATE;


//视频编码轮询状态
typedef TMTI_VIDEOENC_POLL_PARAM		IT_POLLSTATE,*IT_PPOLLSTATE;
//视频编码多画面状态
typedef TMTI_VIDEOENC_MP_PARAM          IT_VENCMPSTATE,*IT_PVENCMPSTATE;
//视频解码重传状态
typedef TMTI_VIDEODEC_RETRANSFER_PARAM  IT_VDECRETRANSFERSTATE,*IT_PVDECRETRANSFERSTATE;
//移动侦测状态
typedef TMTI_MOTION_DETECT_PARAM        IT_MOTIONDETECTSTATE,*IT_PMOTIONDETECTSTATE;

/////////////////OsdManage group///////////////////////
//视频编码字幕状态
typedef TMTI_VIDEOENC_OSD_PARAM         IT_VENCOSDSTATE,*IT_PVENCOSDSTATE;
//视频源输入端口状态
typedef TMTI_VIDEO_INPUT_PARAM          IT_VINPUTSTATE,*IT_PVINPUTSTATE;


/////////////////PeriCtrl group///////////////////////
//串口状态
typedef struct tagSerialState
{
	u8	bySerialSend; //串口发送状态(INTTEST_TRUE/INTTEST_FALSE)
	u8	bySerialRecv; //串口接收状态(INTTEST_TRUE/INTTEST_FALSE)
}PACKED IT_SERIALSTATE,*IT_PSERIALSTATE;

//并口输入状态
typedef struct tagParallelInState
{	
	u8	bPInputState[MAX_INPUT_PIN_NUM];  //告警输入状态(INTTEST_TRUE/INTTEST_FALSE)
}PACKED IT_PARALLELINSTATE,*IT_PPARALLELINSTATE;

//并口输出状态
typedef struct tagParallelOutState
{	
	u8	bPOutputState[MAX_OUTPUT_PIN_NUM];//告警输出状态(INTTEST_TRUE/INTTEST_FALSE)
}PACKED IT_PARALLELOUTSTATE,*IT_PPARALLELOUTSTATE;

/////////////////RWConfig group///////////////////////
//系统参数结构
typedef TMTI_SYS_PARAM                  IT_SYSPARAM,*IT_PSYSPARAM;
//系统公共参数结构
typedef TMTI_SYS_PUB_PARAM              IT_SYSPUBPARAM,*IT_PSYSPUBPARAM;
//公共网络参数结构
typedef TMTI_COMMONNET_PARAM            IT_COMMONNETPARAM,*IT_PCOMMONNETPARAM;
//视频编码参数结构
typedef TMTI_VIDEOENC_PARAM             IT_VENCPARAM,*IT_PVENCPARAM;
//视频编码公共参数结构
typedef TMTI_VIDEOENC_PUB_PARAM         IT_VENCPUBPARAM,*IT_PVENCPUBPARAM;
//视频编码字幕参数结构
typedef TMTI_VIDEOENC_OSD_PARAM         IT_VENCOSDPARAM,*IT_PVENCOSDPARAM;
//视频编码多画面参数结构
typedef TMTI_VIDEOENC_MP_PARAM          IT_VENCMPPARAM,*IT_PVENCMPPARAM;
//视频编码视频源端口参数结构
typedef TMTI_VIDEOENC_INPUT_PORT_PARAM  IT_VENCINPORTPARAM,*IT_PVENCINPORTPARAM;
//视频编码轮询参数结构
typedef TMTI_VIDEOENC_POLL_PARAM		IT_POLLPARAM,*IT_PPOLLPARAM;
//视频解码参数结构
typedef TMTI_VIDEODEC_PARAM             IT_VDECPARAM,*IT_PVDECPARAM;
//视频解码公共参数结构
typedef TMTI_VIDEODEC_PUB_PARAM         IT_VDECPUBPARAM,*IT_PVDECPUBPARAM;
//视频解码重传参数结构
typedef TMTI_VIDEODEC_RETRANSFER_PARAM  IT_VDECRETRANSFERPARAM,*IT_PVDECRETRANSFERPARAM;
//音频编码参数结构
typedef TMTI_AUDIOENC_PARAM             IT_AENCPARAM,*IT_PAENCPARAM;
//音频解码参数结构
typedef TMTI_AUDIODEC_PARAM             IT_ADECPARAM,*IT_PADECPARAM;
//串口参数结构
typedef TMTI_SERIALPORT_PARAM           IT_SERIALPORTPARAM,*IT_PSERIALPORTPARAM;
//串口公共参数结构
typedef TMTI_SERIALPORT_PUB_PARAM       IT_SERIALPORTPUBPARAM,*IT_PSERIALPORTPUBPARAM;
//并口输入参数结构
typedef TMTI_INPUTPIN_PARAM             IT_INPUTPINPARAM,*IT_PINPUTPINPARAM;
//视频源输入端口参数结构
typedef TMTI_VIDEO_INPUT_PARAM          IT_VINPUTPARAM,*IT_PVINPUTPARAM;
//移动侦测参数结构
typedef TMTI_MOTION_DETECT_PARAM        IT_MOTIONDETECTPARAM,*IT_PMOTIONDETECTPARAM;
//PPPOE参数
typedef TMTI_PPPOE_PARAM                IT_PPPOEPARAM,*IT_PPPPOEPARAM;


#ifdef WIN32
#pragma pack( pop )
#endif //WIN32

#pragma pack()
#ifndef _VXWORKS_
#pragma pack(pop)
#endif

class CKdmMtIntTest
{
public:
	CKdmMtIntTest() : m_dwTesterNode( INVALID_NODE ) {}
	virtual ~CKdmMtIntTest() {}
	
	/////////////////////////////链路建立和撤销////////////////////////////////
	BOOL LinkInstance( u32 dwIp, u16 wPort = PORT_MT );
	void DisLinkInstance( void );
	BOOL IsLinked();
	void SetTesterNode( u32 dwNodeId ) { m_dwTesterNode = dwNodeId; }
	
	/////////////////////////////详细查询接口//////////////////////////////////
    //获取终端硬件类型(0:KDM2400;1:KDM2500;2:KDM2418;3:KDM2518;4:KDM2418A;5:KDV8010A;6:KDV8010C;7:DEC5)
    BOOL GetTerDeviceType( u8* pbyMtBoardType );
        
    //state
    //获取系统时间
	BOOL GetSysTime( IT_PSYSTIME ptSysTime = NULL );
    //获取视频编码通道状态
    BOOL GetVencState( IT_PVENCSTATE ptVencState = NULL, u8 byId = 0 );
    //获取视频解码通道状态
    BOOL GetVdecState( IT_PVDECSTATE ptVdecState = NULL, u8 byId = 0 );
    //获取音频编码通道状态
    BOOL GetAencState( IT_PAENCSTATE ptAencState = NULL, u8 byId = 0 );
    //获取音频解码通道状态
    BOOL GetAdecState( IT_PADECSTATE ptAdecState = NULL, u8 byId = 0 );
    //获取串口状态
    BOOL GetSeriState( IT_PSERIALSTATE ptSeriState = NULL, u8 byId = 0 );
    //获取并口输入状态
    BOOL GetParallelInState( IT_PPARALLELINSTATE ptParallelInState = NULL, u8 byId = 0 );   
    //视频编码轮询状态
    BOOL GetPollState( IT_PPOLLSTATE ptPollState = NULL, u8 byId = 0 );
    //视频编码多画面状态        
    BOOL GetVencMpState( IT_PVENCMPSTATE ptVencMpState = NULL, u8 byId = 0 ); 
    //视频解码重传状态
    BOOL GetVdecRetransferState( IT_PVDECRETRANSFERSTATE ptVdecRetransferState = NULL, u8 byId = 0 ); 
    //视频编码移动侦测状态
    BOOL GetVencMotionDetectState( IT_PMOTIONDETECTSTATE ptVencMotionDetectState = NULL, u8 byId = 0 );
    //视频解码移动侦测状态
    BOOL GetVdecMotionDetectState( IT_PMOTIONDETECTSTATE ptVdecMotionDetectState = NULL, u8 byId = 0 );
    //视频编码字幕状态
    BOOL GetVencOsdState( IT_PVENCOSDSTATE ptVencOsdState = NULL, u8 byId = 0 );
	//视频源输入端口状态
	BOOL GetVencVidSrcState( IT_PVINPUTSTATE ptVSrcState = NULL, u8 byId = 0 );

    //param
	//获取设备能力集
	BOOL GetCapability( IT_PCAPABILITY ptCapability = NULL ) ;
	//系统参数结构
    BOOL GetSysParam( IT_PSYSPARAM ptSysParam = NULL ); 
    //系统公共参数结构
    BOOL GetSysPubParam( IT_PSYSPUBPARAM ptSysPubParam = NULL ); 
    //视频编码网络参数
    BOOL GetVideoEncNetParam( IT_PCOMMONNETPARAM ptCommonNetParam = NULL, u8 byId = 0 ); 
   //视频解码网络参数
    BOOL GetVideoDecNetParam( IT_PCOMMONNETPARAM ptCommonNetParam = NULL, u8 byId = 0 ); 
	//音频编码网络参数
    BOOL GetAudioEncNetParam( IT_PCOMMONNETPARAM ptCommonNetParam = NULL, u8 byId = 0 ); 
    //音频解码网络参数
    BOOL GetAudioDecNetParam( IT_PCOMMONNETPARAM ptCommonNetParam = NULL, u8 byId = 0 ); 
    //视频编码参数结构
    BOOL GetVencParam( IT_PVENCPARAM ptVencParam = NULL, u8 byId = 0 ); 
    //视频编码公共参数结构
    BOOL GetVencPubParam( IT_PVENCPUBPARAM ptVencPubParam = NULL, u8 byId = 0  ); 
    //视频编码字幕参数结构
    BOOL GetVencOsdParam( IT_PVENCOSDPARAM ptVencOsdParam = NULL, u8 byId = 0 ); 
    //视频编码多画面参数结构        
    BOOL GetVencMpParam( IT_PVENCMPPARAM ptVencMpParam = NULL, u8 byId = 0 ); 
    //视频编码视频源端口参数结构
    BOOL GetVencInportParam( IT_PVENCINPORTPARAM ptVencInportParam = NULL, u8 byId = 0 ); 
    //视频编码轮询参数结构
    BOOL GetPollParam( IT_PPOLLPARAM ptPollParam = NULL, u8 byId = 0 ); 
    //视频解码参数结构
    BOOL GetVdecParam( IT_PVDECPARAM ptVdecParam = NULL, u8 byId = 0 ); 
    //视频解码公共参数结构        
    BOOL GetVdecPubParam( IT_PVDECPUBPARAM ptVdecPubParam = NULL, u8 byId = 0 ); 
    //视频解码重传参数结构
    BOOL GetVdecRetransferParam( IT_PVDECRETRANSFERPARAM ptVdecRetransferParam = NULL, u8 byId = 0 ); 
    //音频编码参数结构
    BOOL GetAencParam( IT_PAENCPARAM ptAencParam = NULL, u8 byId = 0 );         
    //音频解码参数结构
    BOOL GetAdecParam( IT_PADECPARAM ptAdecParam = NULL, u8 byId = 0 );         
    //串口参数结构
    BOOL GetSerialportParam( IT_PSERIALPORTPARAM ptSerialportParam = NULL, u8 byId = 0 ); 
    //串口公共参数结构
    BOOL GetSerialportPubParam( IT_PSERIALPORTPUBPARAM ptSerialportPubParam = NULL, u8 byId = 0 ); 
    //并口输入参数结构
    BOOL GetInputPinParam( IT_PINPUTPINPARAM ptInputPinParam = NULL, u8 byId = 0 ); 
    //视频源输入端口参数结构
    BOOL GetVinputParam( IT_PVINPUTPARAM ptVinputParam = NULL, u8 byId = 0 ); 
    //视频编码移动侦测参数结构
    BOOL GetVencMotionDetectParam( IT_PMOTIONDETECTPARAM ptVencMotionDetectParam = NULL, u8 byId = 0 );
    //视频解码移动侦测参数结构
    BOOL GetVdecMotionDetectParam( IT_PMOTIONDETECTPARAM ptVdecMotionDetectParam = NULL, u8 byId = 0 );
    //PPPOE参数结构
	BOOL GetPPPOEParam(IT_PPPPOEPARAM ptPPPOEParam=NULL,u8 byId = 0);
 	
protected:
	BOOL	IntTestQuery( u16 wEvent, void* buf, u16  wBufLen, u16* pwRetLen, 
				const void* pParam = NULL, u16  wParamLen = 0, u16  wAppId = 32 );
protected:
	u32	m_dwTesterNode;

};

#endif //_MT_INTEGRATION_TEST_H
