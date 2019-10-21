/** 
* @file         coedclib.h 
* @brief        mediasdk 
* @details  CODEC�ϲ�API��װ���� 
* @author       ������ κ�α� 
* @date     date 
* @version  A001 
* @par Copyright (c):  
*      kedacom 
* @par History:          
*   version: author, date, desc\n 
*/ 

#ifndef __CODECLIB_H_
#define __CODECLIB_H_

#include "KdvDef.h"
#include "codeccommon.h"
#include "kdvmedianet.h"
#include "kdmfileinterface.h"

/*=======================================����������ṹ�Ķ���=================================*/

//ģ��汾��Ϣ�ͱ���ʱ�� �� �����Ŀ�İ汾�ͱ���ʱ��
API void kdvencoderver();
API void kdvdecoderver();

//ģ�������Ϣ �� �����Ŀ�İ�����Ϣ
API void kdvencoderhelp();
API void kdvdecoderhelp();

enum 
{
	FILE_PLAY_STOP = 0,///<�ļ�����״̬����ֹͣ
	FILE_PLAY_START,   ///<�ļ�����״̬������ʼ
	FILE_PLAY_PAUSE    ///<�ļ�����״̬������ͣ
}  ;


enum 
{
	FILE_TYPE_ASF = 0,///<�ļ����ͣ�asfm,mp4�ļ�ͳһ�������
	FILE_TYPE_KSJ     ///<�ļ����ͣ�ksj��
} ;

enum
{
	DECODER_TYPE_NET = 0 ,///<�������
	DECODER_TYPE_FILE     ///<�ļ�����
		
} ;


/**���������(Kbps)*/
#define  BIT_RATE_ZERO           (u16)0 

/**I֡�����P֡��Ŀ*/
#define  MAX_P_FARME_INTERNAL_I  (u8)75

/*��������(1-31)*/
#define  MAX_QUANT_PARAM         (u8)31   ///<�����������
#define  MIN_QUANT_PARAM         (u8)1    ///<��С��������
#define  QUANT_PARAM_31          (u8)31   ///<�����������
#define  QUANT_PARAM_1           (u8)1    ///<��С��������

/*ͼ���С*/
#define  VIDEO_WIDTH_720         (u16)720  ///<ͼ��߶�
#define  VIDEO_HEIGHT_576        (u16)576  ///<ͼ����
#define  VIDEO_WIDTH_352         (u16)352  ///<ͼ��߶�
#define  VIDEO_HEIGHT_288        (u16)288  ///<ͼ����
#define  VIDEO_WIDTH_DEFAULT     (u16)640  ///<ͼ��Ĭ�ϸ߶�
#define  VIDEO_HEIGHT_DEFAULT    (u16)480  ///<ͼ��Ĭ�Ͽ��

/*֡��(default:25)*/
#define  FRAME_RATE_ONE          (u8)1    ///<Ĭ��֡��
#define  FRAME_RATE_DEFAULT      (u8)25   ///<Ĭ��֡��

/*ͼ��ѹ������(1-5)*/
#define  IMAGE_ENCODE_QUALITY_TWO  (u8)2   ///<ѹ������2
#define  IMAGE_ENCODE_QUALITY_FOUR (u8)4   ///<ѹ������4     

/*����ģʽ mp3*/
#define	 AUDIO_MODE_WORST        (u8)0///<���
#define	 AUDIO_MODE_BAD          (u8)1///<�� 
#define	 AUDIO_MODE_NORMAL       (u8)2///<һ��
#define	 AUDIO_MODE_FINE         (u8)3///<�� 
#define	 AUDIO_MODE_BEST         (u8)4///<���

//G.711
#define  AUDIO_MODE_PCMA         (u8)5///<a law
#define  AUDIO_MODE_PCMU         (u8)6///<u law 
//G.723
#define AUDIO_MODE_G723_6        (u8)7///<G723_6 
#define AUDIO_MODE_G723_5        (u8)8///<G723_5
//G.728
#define AUDIO_MODE_G728		 (u8)9 ///<G728
//G.722
#define AUDIO_MODE_G722		 (u8)10 ///<G722
//G.729
#define AUDIO_MODE_G729		 (u8)11 ///<G729
//G.7221
#define AUDIO_MODE_G7221		 (u8)12 ///<G7221
//adpcm
#define AUDIO_MODE_ADPCM		 (u8)13 ///<ADPCM

#define  MAX_LOCAL_IPADDR_LEN    (u8)30 ///<����ip��ַ����

#ifndef  MAX_NETSND_DEST_NUM
#define  MAX_NETSND_DEST_NUM     (u8)5  ///<����͵�Ŀ�ĵ���
#endif

#define  CAP_AUDIO_ONLY          (u8)0x01 ///<��Ƶ            ���ݲ���
#define  CAP_CAMERA_ONLY         (u8)0x02 ///<      CAMERA��Ƶ���ݲ���
#define  CAP_AC_BOTH             (u8)0x03 ///<��Ƶ��CAMERA��Ƶ���ݾ����в���
#define  CAP_DESKTOP_ONLY        (u8)0x04 ///<	    DESKTOP������Ƶ���ݲ���
#define  CAP_AD_BOTH             (u8)0x05 ///<��Ƶ��DESKTOP������Ƶ���ݾ����в���
#define  CAP_FILE_AV_BOTH		 (u8)0x08 ///<��ָ���ļ���Ŀǰ֧��kdc¼���������asf�ļ�������Ƶ����Ƶѹ������Ϊ����Դ

#define  CTRL_AUDIO_ONLY          (u8)0x01 ///<��Ƶ(�����ɼ�������Ƶ�Լ��ļ�����Ƶ��)
#define  CTRL_VIDEO_ONLY          (u8)0x02 ///<��Ƶ(�����ɼ�������Ƶ�Լ��ļ�����Ƶ��)
#define  CTRL_AV_BOTH			  (u8)0x03 ///<����Ƶ����Ƶ��

#define  MAX_DECODER_NUM		 (u8)96   ///<����������Ŀ


#define  CAP_FRAME_BMP           (u8)0 ///<RGB24λͼ;
#define  CAP_FRAME_UYVY          (u8)1 ///<yuv4:2:2��ʽ
#define  CAP_FRAME_I420          (u8)2 ///<yuv4:2:0��ʽ
#define  CAP_FRAME_YUY2          (u8)3 ///<yuy2��ʽ
#define  SOFT_DEC_ONLY           (u8)0x01///<�����
#define  HARD_DEC_ONLY           (u8)0x02///<��Ӳ��
#define  SOFTHARD_DEC_BOTH       (u8)0x03///<ͬʱ��Ӳ�� 

#define  AUDIO_MODE_ONLYDEC      (u8)0x01///<��Ƶģʽ������
#define  AUDIO_MODE_ONLYRCV      (u8)0x02///<��Ƶģʽ������
#define  AUDIO_MODE_PLAY         (u8)0x00///<��Ƶģʽ������    
   
#define NAL_CUSTOM_TRANSDATA   30///<͸������NAL����
#define MAX_GPS_INFO_LEN     1024///<GPS��Ϣ��󳤶�

//Err Info
#define ERR_H264_ENC_HANDLE_NULL    8001///<H264������Ϊ��
#define ERR_H264_ENC_BUF_OVER_FLOW  8002///<H264���뻺�����
#define ERR_H264_DEC_HANDLE_NULL    9001///<H264������Ϊ��
#define ERR_H264_DEC_BUF_OVER_FLOW  9002///<H264���뻺�����

//MP4 ENC
#define MP4_CUSTOM_GPS             0xA0///<mpeg4 gpsͷ
#define ERR_MP4_ENC_HANDLE_NULL    6001///<mpeg4������Ϊ��
#define ERR_MP4_DEC_HANDLE_NULL    7001///<mpeg4������Ϊ��

/** �����ṹ*/
typedef struct
{
    void *pvStart;   ///<������������ʼλ��
    u8 *pu8Buffer;   ///<����������
    u32 u32Length;   ////<������
    u8 *pu8Tail;     ///<�����ֽ�λ��
}TBitStream;

struct TTransparentBuffer
{
    u16 wLength;    ///<���Ȱ���wLength��wType��dwSubType��pBuf���ܳ���
	u16 wType;      ///<����
	u32 dwSubType;  ///<����byType��Լ�������ʽ������ʱӦ��Ϊ0
	s8* pBuf;       ///<����
};

/** ��ȡ��߶ȣ���ʹ�ñ�׼����Ƶ��ʽ���μ� videodefines.h �����ΪD1*/
struct TMixVidBgParam
{
	s32 nBgWidth;                             ///<����ͼ��Ŀ��
	s32 nBgHeight;                            ///<����ͼ��ĸ߶�
	s32 nDrawBoundaryFlag;                    ///<�����Ƿ񹴻��߽��ߵı�ǣ�1�������߽��ߣ�0���������߽��ߣ�
    s32 nBoundaryRGB;                         ///<����߽�����ɫRGB��������ʽΪ0x00RRGGBB��
    s32 nBoundaryWidth;                       ///<�߽���ͳһ��ȣ���Ϊ2�ı�����
	s32 nDrawBackgroundFlag;                  ///<������ͼ���Ƿ���䱳��ɫ
	s32 nBackgroundRGB;	                      ///<����ϳɱ������ɫRGB��������ʽΪ0x00RRGGBB��
};


#ifndef TNETSTRUCT
#define TNETSTRUCT
#define MAX_USERDATA_LEN    16   ///<����û����ݳ���

/** �������*/
typedef struct tagNetSession
{
    tagNetSession(){m_dwRTPAddr=0; m_wRTPPort=0; m_dwRTCPAddr=0; m_wRTCPPort=0; \
                    m_dwRtpUserDataLen = 0; m_dwRtcpUserDataLen = 0;}
    
    u32   m_dwRTPAddr;  ///<RTP��ַ(������)
    u16   m_wRTPPort;   ///<RTP�˿�(������)
    u32   m_dwRTCPAddr; ///<RTCP��ַ(������)
    u16   m_wRTCPPort;  ///<RTCP�˿�(������)
    u32   m_dwRtpUserDataLen;   ///<Rtp�û����ݳ���,�����ڴ��������,��Ҫ��ÿ��udpͷǰ��ӹ̶�ͷ���ݵ����
    u8    m_abyRtpUserData[MAX_USERDATA_LEN]; ///<�û�����ָ��
    u32   m_dwRtcpUserDataLen;   ///<Rtcp�û����ݳ���,�����ڴ��������,��Ҫ��ÿ��udpͷǰ��ӹ̶�ͷ���ݵ����
    u8    m_abyRtcpUserData[MAX_USERDATA_LEN]; ///<�û�����ָ��
}TNetSession;

/** �����������*/
typedef struct tagLocalNetParam
{
	TNetSession  m_tLocalNet;   ///<���������ַ
	u32        m_dwRtcpBackAddr;///<RTCP�ط���ַ
	u16         m_wRtcpBackPort; ///<RTCP�ط��˿�      
}TLocalNetParam;

/** ���緢�Ͳ���*/
typedef struct tagNetSndParam
{
	u8 m_byNum;                                     ///<���͵�ַ����
	TNetSession m_tLocalNet;                        ///<���������ַ
	TNetSession m_tRemoteNet[MAX_NETSND_DEST_NUM];  ///<�Ŷ������ַ
}TNetSndParam;

#endif//TNETSTRUCT

/** ����������*/
typedef struct Encoder
{  
	u32   m_dwCpuId;///<CPU��ʶ
	s32   m_nPriority;///<���ȼ�
	HWND    m_hwVideoPreview;///<��ƵԤ�����
}TEncoder;

/** ��Ƶ����������*/
typedef struct VideoDecoder
{
	s32     m_nPriority;///<��Ƶ�����߳����ȼ�
	u32     m_dwCpuId;  ///<��Ƶ�����CPU��ʶ
	s32     m_nVideoPlayBufNum;     ///<��Ƶ�طŻ������
	s32     m_nVideoStartPlayBufNum;///<��Ƶ��ʼ�طŻ������
	HWND    m_hwPlayVideo;///<��Ƶ���Ŵ��ھ��
	BOOL32  m_bOverlay;///<�Ƿ�Overlay;
	u8      m_byMask;  ///<0x01�������,0x02Ӳ������,0x03����ͬʱ����
    double  m_dbDecTimer;  ///<��ʱ���Ʋ��ŵ�ʱ��������λms��default 0������Ӧ������ʱ��
	BOOL32  m_bNodeinterlacing;///<�Ƿ��deinterlacing���أ�default 0	 
	//Ӳ�����벿�� 
	s32 	m_nBsp15Freq;//MHZ
	s32 	m_nMemFreq;  //MHZ
	u8      m_byChipType;
	u8	    m_byBoardId;//index from 0 
	u8	    m_byBufNum; //queue length, default 5 
	CHAR	m_achDemoFilePath[MAX_PATH];	
	u32	  m_dwPostPrcLvl;  /*mpv4����ļ���*/		 
}TVideoDecoder;

/** ��Ƶ����������*/
typedef struct AudioDecoder
{
	s32   m_nPriority;       ///<��Ƶ�����߳����ȼ�
	u32   m_dwCpuId;         ///<��Ƶ�����CPU��ʶ
	s32   m_nAudioPlayBufNum;///<��Ƶ���Ż�����
	s32   m_nAudioStartPlayBufNum;///<��Ƶ��ʼ���Ż�����
    u8    m_byAudioMode; ///<��Ƶģʽ
}TAudioDecoder;

#define FRMRATE_CAL_BY_FPS  0 ///<֡�ʲɼ�ģʽFPS
#define FRMRATE_CAL_BY_SPF  1 ///<֡�ʲɼ�ģʽSPF

/** ��Ƶ�ɼ�����*/
typedef struct VideoCapParam
{
	u16	m_wVideoWidth; ///<��Ƶ����ͼ����(default:640)
	u16	m_wVideoHeight;///<��Ƶ����ͼ��߶�(default:480)
	u8  m_byFrmRateLittleThanOne; ///< ֡���Ƿ�С��1 ?  
	u8	m_byFrameRate; ///<��Ƶ����֡��(default:25)   ��֡��>=1���� m_byFrameRate �� fps, ��֡��<1���� m_byFrameRate �� spf  
	u8  m_byFOURCC;    ///<��Ƶ����֡��ʽ(default:CAP_FRAME_BMP)
}TVideoCapParam;

/** ������״̬*/
typedef struct KdvEncStatus
{
	BOOL32 			m_bVideoSignal;	  ///<�Ƿ�����Ƶ�ź�
	BOOL32	        m_bVideoCapStart; ///<�Ƿ�ʼ��Ƶ����
	BOOL32			m_bAudioCapStart; ///<�Ƿ�ʼ��Ƶ����
	BOOL32			m_bVideoEncStart; ///<�Ƿ�ʼ��Ƶ����
	BOOL32			m_bAudioEncStart; ///<�Ƿ�ʼ��Ƶ����
	BOOL32			m_bNetSndVideoStart;///<�Ƿ�ʼ��Ƶ����
	BOOL32			m_bNetSndAudioStart;///<�Ƿ�ʼ��Ƶ����
	u32				m_dwVideoBufLen;  ///<��Ƶ���峤��
	u32				m_dwAudioBufLen;  ///<��Ƶ���峤��
	TVideoEncParam	m_tVideoEncParam; ///<��Ƶ�������
	u8				m_byAudioEncParam;///<��Ƶ����ģʽ
	u8				m_byAudEncMediaType; ///<��Ƶ����ý������	
	TNetSndParam    m_tVideoSendAddr; ///<��Ƶ���͵�ַ
	TNetSndParam    m_tAudioSendAddr; ///<��Ƶ���͵�ַ
	u32				m_dwSetEncParam; ///<����Ӳ�������������
	u32				m_dwVideoSend;   ///<�ѷ��͵���Ƶ����
	u32				m_dwAudioSend;   ///<�ѷ��͵���Ƶ����
	u32				m_dwHardFrameNum;///<Ӳ���ص�֡��
	u32				m_dwPrevFrameNum;///<Ԥ��֡��	
}TKdvEncStatus;

/** ������ͳ����Ϣ*/
typedef struct KdvEncStatis
{
    u8   m_byVideoFrameRate;///<��Ƶ֡��
    u32  m_dwVideoBitRate;  ///<��Ƶ�����ٶ�
    u32  m_dwAudioBitRate;  ///<���������ٶ�
    u32  m_dwVideoPackLose; ///<��Ƶ��֡��
    u32  m_dwVideoPackError;///<��Ƶ��֡��
    u32  m_dwAudioPackLose; ///<������֡��
    u32  m_dwAudioPackError;///<������֡��
	u32  m_wAvrVideoBitRate;///<1��������Ƶ����ƽ������
	u32	 m_wAvrAudioBitRate;///<1��������������ƽ������
}TKdvEncStatis;

/** �غɱ�ṹ*/
typedef struct tagRtpPayload
{
    u8 m_byRmtActivePT;///<����ʵ���غ�
    u8 m_byRealPT;///<���ǹ�˾Լ�����غ�
}TRtpPayload;

/** ������״̬*/
typedef struct KdvDecStatus
{
	BOOL32	m_bVideoDecStart;///<�Ƿ�ʼ��Ƶ����
	BOOL32	m_bAudioDecStart;///<�Ƿ�ʼ��Ƶ����
	BOOL32	m_bRcvVideoStart;///<�Ƿ�ʼ��Ƶ����
	BOOL32	m_bRcvAudioStart;///<�Ƿ�ʼ��Ƶ����
	u32	m_dwAudioBufLen; ///<��Ƶ���Ż��峤��
	u32	m_dwVideoBufLen; ///<��Ƶ���Ż��峤��
	TLocalNetParam   m_tAudioRcvAddr;///<��Ƶ���ص�ַ
	TLocalNetParam   m_tVideoRcvAddr;///<��Ƶ���ص�ַ
	BOOL32	m_bAudioOutThread;///<�Ƿ����������̴߳��
	BOOL32	m_bMp4CodecThread;///<�Ƿ�Mp4�����̴߳��
	u32   m_dwAudioDecType;     ///<��Ƶ��������
	u32   m_dwVideoDecType;     ///<��Ƶ��������
}TKdvDecStatus;

/*������ͳ����Ϣ   remove to codeccommon.h*/

/*========================================��������붨��======================================*/

/*��Ƶ�����������*/
#define VID_DROP_PREFERRED_QUALITY      0    ///<�������ȣ�Ϊ��I֡����
#define VID_DROP_PREFERRED_FAST_SPEED   1    ///<�ٶ����ȿ쵵���Իָ������ж�����ֻ�ⲻ����֡��Ϊ1����
#define VID_DROP_PREFERRED_MID_SPEED    2    ///<�ٶ������е����Իָ������ж�����ֻ�ⲻ����֡��Ϊ2����
#define VID_DROP_PREFERRED_SLOW_SPEED   3    ///<�ٶ������������Իָ������ж�����ֻ�ⲻ����֡��Ϊ4����

struct TEncoderRef;
struct TEncoderStatus;
class  CSoftEncoder;

typedef void (*PAUDIOCALLBACK)(u8 buAudioMode, u8 *pBuf, s32 nSize, KD_PTR pvContext);
typedef void (*PVOICECHECK)(CHAR cMode, KD_PTR pvContext);
typedef void (*PDRAWCALLBACK)(void* hAddDrawDC, KD_PTR pvParam);
typedef void (*PSTATICPICCALLBACK)(u8 *pBuf, u32 dwSize, BITMAPINFOHEADER* pHead, KD_PTR pvParam);
typedef void (*pfFileStatCB)(u8 byFileState, u32 dwSec, KD_PTR pvContext) ;
typedef void (*PFRAMEINFOCALLBACK)(u8 *pBitmapBuf, u32 dwWidth, u32 dwHeight, u32 dwbitcount, u32 dwTimeStamp, KD_PTR pvParam);//�����ֱ�Ϊλͼ����ָ�롢λͼ���ߡ�λ�
typedef void (*PAUDIOFRAMEINFOCALLBACK)(u8 *pPcmBuf, u32 dwPcmLen, u32 dwTimeStamp, KD_PTR pvParam);//�����ֱ�Ϊλͼ����ָ�롢λͼ���ߡ�λ�
typedef void (*PFRAMECALLBACK)(PFRAMEHDR pFrmHdr, KD_PTR pvParam);
//pDataBuf  - ���ݻ�����
//nDataSize - �������ݳ���
//dwContext - ������
typedef void (*PDATAINFOCALLBACK)(u8 *pDataBuf, s32 nDataSize, KD_PTR pvParam);
typedef void (*PDRAWTIMESTAMPSCALLBACK)(u32 dwTimestamps, KD_PTR pvParam);

/** �������*/
typedef struct tagVoiceCheck
{
	s32			m_nBGTime;///<ime window of background voice sampling (ms is one unit)
	s32			m_nThreshold;///<��ֵ
	PVOICECHECK	m_pCallBack;///<�ص�����
	void*       m_pvContext;///<������
}TVoiceCheck;

/** �ļ����������*/
typedef struct tagPlayFileParam
{
	u32				m_dwGranuity;///<�ļ���������
	void*			m_pvContext;///<������
	pfFileStatCB	m_pfCB;///<�ص�����
	u8				m_byFileType;///<�ļ�����
	s8				m_achFileName[512];///<�ļ���
} TPlayFileParam ;

/** �����ļ���Ϣ*/
typedef struct tagPlayFileInfo
{
	u64				m_qwTotalSize;///<�ļ��ܴ�С
	u64				m_qwCreateDate;///<�ļ�����ʱ��
	u32				m_dwTotalTime;///<�ļ���ʱ��
} TPlayFileInfo;

/** �ļ���������Ϣ*/
typedef struct tagFileStreamInfo
{
	BOOL32			m_bAudio ;///<�Ƿ���Ƶ
	u16				m_wWidth;///<��
	u16				m_wHeight ;///<��
	u8				m_byStreamId ;///<��id
	u8				m_byMediaType ;///<ý���ʽpayload
} TPlayFileStreamInfo ;

#define MAX_FILE_STREAM		3	///<����ļ�����Ŀ

/** �ļ�����״̬*/
typedef struct tagPlayFileStatus
{
	u32				m_dwCurrentPlayPos;///<��ǰ����λ��
	u8				m_byPlayMode;///<����ģʽ
	s8				m_byPlayRate;///<��������
	u8				m_byPlayState;///<����״̬
	u8				m_byStreamNum ;///<��id
	u32				m_dwVFps;///<��Ƶ֡��
	u32				m_dwAFps;///<��Ƶ֡��
	TPlayFileStreamInfo		m_atFileStreamInfo[MAX_FILE_STREAM];///<����Ϣ
} TPlayFileStatus;

/** �����ļ���Ϣ*/
typedef struct tagTReserveFileInfo
{
	u32           m_dwFirstPackTimeStamps;///<�ļ��е�һ����ʱ���
	u32           m_dwBitRate;///<����
	u32           m_dwKeyFrameInterval;///<�ؼ�֡���
	u8           m_byFrameRate;///<֡��
}TReserveFileInfo;

typedef void (*PGETAUDIOPOWER)(u8 byAudioPower, KD_PTR pvContext);

#define PIC_ENCODE_JPEG   (u8)1///<ͼƬ�����ʽJPEG
#define PIC_ENCODE_BMP    (u8)2///<ͼƬ�����ʽbmp

#define MAX_LOGO_NAME_LEN (u8)32///<���̨�����Ƴ���

/** ̨�����������������Ϣ����*/
typedef struct tagLogoSetting
{
	u32 m_dwXPos;    ///<̨��λ��
	u32 m_dwYPos;    ///<̨��λ��
	u8  m_byClarity; ///<̨��͸����(0-255)
	u8  m_byLogoName[MAX_LOGO_NAME_LEN+3]; ///<�ն˱���
}TLogoSetting;

/** �����建������*/
typedef enum
{
	PLAY_TO_RESERVE = 0,///<����ת����
	RESERVE_TO_PLAY,    ///<����ת����
	SEEKTO,             ///<��ת
}eResetBufType;

/** �ļ�������Ϣ*/
typedef struct tagKdmPlayFileInfo 
{
	u8   m_byVideoStreamNum;///<�ļ�����Ƶ����Ŀ
	u8   m_byAudioStreamNum;///<�ļ�����Ƶ����Ŀ
}TKdmPlayFileInfo;

class CKdvEncoder
{	
public:
    CKdvEncoder();
    virtual ~CKdvEncoder();
public:     
/** 
* Create 
* ���������� 
* @param[in]   HWND previewHwnd Ԥ�����ھ�� 
* @param[in]   s32 nPriority    �߳����ȼ�
* @param[in]   u8 byBindCpuId   ʹ��CPUID
* @param[in]   u8 byCapType     �ɼ�����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    Create( HWND previewHwnd, 
		           s32 nPriority = THREAD_PRIORITY_NORMAL,
		           u8 byBindCpuId = 0, u8 byCapType = CAP_AC_BOTH );

/** 
* Free 
* ���ٱ����� 
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨�� 
* @par ���� 
*      �ͷ���Դ�������ϲ��ڳ����˳�֮ǰ������ã��������ɷ������������е��ã�����   
*/
    u16	   Free();

/** 
* SetVoiceCheck 
* �����������ʼ�� 
* @param[in]   BOOL32 bCheck                    �Ƿ�������
* @param[in]   const TVoiceCheck *ptVoiceCheck  ����������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
   u16    SetVoiceCheck(BOOL32 bCheck, const TVoiceCheck* ptVoiceCheck);
	
/** 
* StartCap 
* ��ʼ�ɼ�
* @param[in]   u8 byCapType  �ɼ�����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    StartCap(u8 byCapType = CTRL_AV_BOTH);

/** 
* StopCap 
* ֹͣ�ɼ�
* @param[in]   u8 byCapType  �ɼ�����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopCap(u8 byCapType =  CTRL_AV_BOTH);

/** 
* SetPreviewHwnd 
* ����Ԥ������
* @param[in]   HWND hPrevWnd  Ԥ�����ھ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetPreviewHwnd( HWND hPrevWnd );

/** 
* SelectVideoSource 
* ѡ����ƵԴ
* @param[in]   const char *szFileName �ļ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SelectVideoSource(const char *szFileName = NULL);
	
/** 
* SetVideoIcon 
* ������Ƶ̨��
* @param[in]   BOOL32 bEnable �Ƿ����ã�TRUEΪ���ã�FALSE����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetVideoIcon( BOOL32 bEnable );

/** 
* SetVideoCapParam 
* ������Ƶ�ɼ�����
* @param[in]  const TVideoCapParam  *ptVideoCapParam ��Ƶ�ɼ������ṹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetVideoCapParam(const TVideoCapParam  *ptVideoCapParam );

/** 
* SetAudioCapParam 
* ������Ƶ�ɼ�ģʽ
* @param[in]  u8 byMode 
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudioCapParam( u8 byMode );

/** 
* SaveAsPic 
* ����ͼƬ
* @param[in]  LPCSTR lpszFileName ͼƬ���� 
* @param[in]  u8 byEncodeMode     ͼƬ�����ʽ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SaveAsPic(LPCSTR lpszFileName, u8 byEncodeMode);

/** 
* SetAudioCallBack 
* ������Ƶ�ص�
* @param[in]  PAUDIOCALLBACK pAudioCallBack �ص�����
* @param[in]  KD_PTR pvContext              �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetAudioCallBack(PAUDIOCALLBACK pAudioCallBack, KD_PTR pvContext);

/** 
* SetPlaySpeed 
* ���ò�������
* @param[in]  float fSpeed ���ű���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetPlaySpeed(float fSpeed);

/** 
* IsStepPlay 
* �Ƿ�֡����
* @param[in]  BOOL32 bIsStepPlay TRUEΪ��֡���ţ�FALSE�ָ�ԭ��ģʽ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    IsStepPlay(BOOL32 bIsStepPlay);

/** 
* NextFrame 
* ������һ֡
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16		NextFrame();

/** 
* SetInputAudioPowerCB 
* ���û�ȡ������Ƶ���ʵĻص�
* @param[in]  PGETAUDIOPOWER pGetInputAudioPowerCB �ص�����
* @param[in]  KD_PTR pvContext                     �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetInputAudioPowerCB(PGETAUDIOPOWER pGetInputAudioPowerCB, KD_PTR pvContext);

/** 
* RegAudioFrameCallBack 
* ��Ƶ�����֡�ص�
* @param[in]  PFRAMECALLBACK pFrameCallback �ص�����
* @param[in]  KD_PTR pvParam                �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    RegAudioFrameCallBack(PFRAMECALLBACK pFrameCallback, KD_PTR pvParam);

/** 
* SetAddLogoIntoEncStream 
* ���� ̨���������� �Ŀ���
* @param[in]  TLogoSetting *ptLogoSetting   ����̨�����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetAddLogoIntoEncStream(TLogoSetting *ptLogoSetting);
	
/** 
* SetTransData 
* ������Ƶ֡��Ϣ��Я����͸������
* @param[in]  s8 *pszData   ͸������
* @param[in]  u32 dwDataLen ͸�����ݳ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16 SetTransData(s8 *pszData, u32 dwDataLen);
	
/** 
* ShowPreview 
* ���� �������ı���Ԥ�� �Ŀ���
* @param[in]  BOOL32 bPreview    TRUE ��Ԥ����FALSE �ر�Ԥ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    ShowPreview(BOOL32 bPreview);

/** 
* GetAudioMute 
* ����Ƿ���
* @param[out]   BOOL32& bMute    TRUE ������FALSE �Ǿ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetAudioMute( BOOL32& bMute );     // �Ƿ��� [zhuyr][4/20/2005]

/** 
* GetEncoderStatus																																		
* ��ȡ������״̬
* @param[out]  TKdvEncStatus &tKdvEncStatus ������״̬�ṹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetEncoderStatus(TKdvEncStatus &tKdvEncStatus); 	
	
/** 
* GetEncoderStatis																																		
* ��ȡ��������ͳ����Ϣ
* @param[out]  TKdvEncStatis &tKdvEncStatis ������ͳ����Ϣ�ṹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetEncoderStatis(TKdvEncStatis &tKdvEncStatis );/*��ȡ��������ͳ����Ϣ*/

/** 
* StartVideoCap																																		
* ��ʼ�ɼ�ͼ��
* @param[in]  u8 byCapType �ɼ�ģʽ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartVideoCap(u8 byCapType = VIDCAP_CAPVIDEO);

/** 
* StopVideoCap																																		
* ֹͣ�ɼ�ͼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopVideoCap(); /*ֹͣ�ɼ�ͼ��*/

/** 
* SetVideoEncParam																																		
* ������Ƶ�������
* @param[in] const TVideoEncParam *ptVideoEncParam ��Ƶ��������ṹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetVideoEncParam(const TVideoEncParam *ptVideoEncParam );

/** 
* GetVideoEncParam																																		
* �õ���Ƶ�������
* @param[out] TVideoEncParam &tVideoEncParam  ��Ƶ��������ṹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetVideoEncParam(TVideoEncParam &tVideoEncParam );    
/** 
* StartVideoEnc																																		
*��ʼѹ��ͼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartVideoEnc();

/** 
* StopVideoEnc																																		
* ֹͣѹ��ͼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopVideoEnc(); 

/** 
* StartAudioCap																																		
*��ʼ�����ɼ�
* @param[in]  BOOL32 bAinMic �����壬�ɲ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartAudioCap(BOOL32 bAinMic = FALSE);

/** 
* StopAudioCap																																		
* ֹͣ�����ɼ�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopAudioCap();

/** 
* SetAudioEncParam																																		
* ��������ѹ������
* @param[in]  u8 byAudioEncParam  ��Ƶģʽ
* @param[in]  u8 byMediaType      ��Ƶpayload
* @param[in]  u16 wAudioDuration  ��Ƶ֡ʱ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudioEncParam(u8 byAudioEncParam, 
                            u8 byMediaType=MEDIA_TYPE_MP3,
                            u16 wAudioDuration = 30); 
/** 
* GetAudioEncParam																																		
* �õ�����ѹ������
* @param[out]  u8 &byAudioEncParam  ��Ƶģʽ
* @param[out]  u8 *pbyMediaType   ��Ƶpayload
* @param[out]  u16 *pwAudioDuration  ��Ƶ֡ʱ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetAudioEncParam(u8 &byAudioEncParam, 
                            u8 *pbyMediaType = NULL,
                            u16 *pwAudioDuration = NULL);

/** 
* StartAudioEnc																																		
* ��ʼѹ������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    StartAudioEnc();

/** 
* StopAudioEnc																																		
* ֹͣѹ������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopAudioEnc(); 

/** 
* SetAudioMute																																		
* �����Ƿ���
* @param[in]  BOOL32 bMute TRUE��ʾ������FALSE��ʾ������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudioMute( BOOL32 bMute ); 

/** 
* SetAudioVolume																																		
* ���òɼ�����
* @param[in]  u8 byVolume ���� ��Χ0~255
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudioVolume(u8 byVolume ); 

/** 
* GetAudioVolume																																		
* ��ȡ�ɼ�����
* @param[out]  u8 &byVolume  ����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetAudioVolume(u8 &byVolume );

/** 
* SetNetFeedbackVideoParam																																		
* ����ͼ��������ش�����
* @param[in]  u16 wBufTimeSpan  ����ʱ��
* @param[in] BOOL32 bRepeatSnd  �Ƿ����ش�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetNetFeedbackVideoParam(u16 wBufTimeSpan, BOOL32 bRepeatSnd = FALSE);

/** 
* SetNetFeedbackAudioParam																																		
* ���������������ش�����
* @param[in]  u16 wBufTimeSpan  ����ʱ��
* @param[in] BOOL32 bRepeatSnd  �Ƿ����ش�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetNetFeedbackAudioParam(u16 wBufTimeSpan, BOOL32 bRepeatSnd = FALSE);

/** 
* SetNetSndVideoParam																																		
* ����ͼ������紫�Ͳ���
* @param[in]  const TNetSndParam *ptNetSndParam ���緢�Ͳ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetNetSndVideoParam(const TNetSndParam *ptNetSndParam);
	
/** 
* SetNetSndAudioParam																																		
* �������������紫�Ͳ���
* @param[in]  const TNetSndParam *ptNetSndParam ���緢�Ͳ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetNetSndAudioParam(const TNetSndParam *ptNetSndParam);
/** 
* GetNetSndVideoParam																																		
* �õ�ͼ������紫�Ͳ���
* @param[out]  TNetSndParam *ptNetSndParam ���緢�Ͳ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	BOOL32 GetNetSndVideoParam(TNetSndParam *ptNetSndParam);

/** 
* GetNetSndAudioParam																																		
* �õ����������紫�Ͳ���
* @param[out]  TNetSndParam *ptNetSndParam ���緢�Ͳ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    BOOL32 GetNetSndAudioParam(TNetSndParam *ptNetSndParam);

/** 
* StartSendVideo																																		
* ��ʼ����ͼ��
* @param[in]  int  dwSSRC ���͵�SSRCֵ��0��ʾ����Ҫ�޸ķ��͵�SSRCֵ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartSendVideo(int  dwSSRC = 0);
	
/** 
* StopSendVideo																																		
* ֹͣ����ͼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopSendVideo();

/** 
* StartSendAudio																																		
* ��ʼ��������
* @param[in]  int  dwSSRC ���͵�SSRCֵ��0��ʾ����Ҫ�޸ķ��͵�SSRCֵ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartSendAudio(int  dwSSRC = 0);

/** 
* StopSendAudio																																		
* ֹͣ��������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopSendAudio();
	
/** 
* SetAudioDumb																																		
* �����Ƿ�����
* @param[in]  BOOL32 bDumb TRUE��ʾ����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16	   SetAudioDumb(BOOL32 bDumb); 
	
/** 
* SetFastUpata																																		
* ����FastUpdata��MediaCtrl��75֡�ڱ�һ��I֡����
* @param[in]  BOOL32 bIsNeedProtect �Ƿ���Ҫǿ�Ʊ�I֡
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetFastUpata(BOOL32 bIsNeedProtect = TRUE);
	
/** 
* SetVideoActivePT																																		
* ��Ƶ��̬�غɵ�PTֵ���趨
* @param[in]  u8 byLocalActivePT ��̬�غ�ֵ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16 SetAudioActivePT(u8 byLocalActivePT);
	
/** 
* SetVideoActivePT																																		
* ��Ƶ��̬�غɵ�PTֵ���趨
* @param[in]  u8 byLocalActivePT ��̬�غ�ֵ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16 SetVideoActivePT(u8 byLocalActivePT );
	
/** 
* SetAudEncryptKey																																		
* ������Ƶ�������key�ִ��Լ� ����ģʽ Aes or Des
* @param[in]  s8 *pszKeyBuf  key�ַ���
* @param[in]  u16 wKeySize   key�ַ�������
* @param[in]  u8 byDecryptMode   ����ģʽ Aes or Des
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetAudEncryptKey( s8 *pszKeyBuf, u16 wKeySize, u8 byEncryptMode );
	
/** 
* SetAudEncryptKey																																		
* ������Ƶ�������key�ִ��Լ� ����ģʽ Aes or Des
* @param[in]  s8 *pszKeyBuf  key�ַ���
* @param[in]  u16 wKeySize   key�ַ�������
* @param[in]  u8 byDecryptMode   ����ģʽ Aes or Des
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetVidEncryptKey( s8 *pszKeyBuf, u16 wKeySize, u8 byEncryptMode ); 
	
/** 
* GetLastError 
* ��ȡ����������
* @param[out]  u32 &dwErrorId  ������   
*/
    VOID    GetLastError( u32 &dwErrorId );
	
private:
	s64    m_qwLastFastupdataTime;     //���һ��FastUpdata��ʱ��

protected:
    CSoftEncoder     *m_pcKdvInterEnc;
    TEncoderRef      *m_ptRef;
    TEncoderStatus   *m_ptStatus;	
};

class CKdvVideoDec;
class CKdvAudioDec;
class CKdvASFFileReader;
struct TDecoderRef;
struct TDecoderStatus;

class CKdvDecoder
{
public:
    CKdvDecoder();
    virtual ~CKdvDecoder();
	
public:
/** 
* CreateDecoder 
* ����������
* @param[in]  const TVideoDecoder *ptVideoDecoder ��Ƶ����������
* @param[in]  const TAudioDecoder *ptAudioDecoder ��Ƶ����������
* @param[in]  BOOL32 bSaveBuff                    �Ƿ񻺳����ݣ��Ƿ񱣴��������ݣ�����ʱ��Ҫ����Ӱ�����ܣ�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    CreateDecoder(const TVideoDecoder *ptVideoDecoder,const TAudioDecoder *ptAudioDecoder, BOOL32 bSaveBuff = FALSE);

/**  
* GetDecoderStatus																																		
* ��ȡ������״̬
* @param[out]  TKdvDecStatus &tKdvDecStatus ������״̬�ṹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetDecoderStatus(TKdvDecStatus &tKdvDecStatus );

/** 
* GetDecoderStatis																																		
* ��ȡ������ͳ����Ϣ
* @param[out]  TKdvDecStatus &tKdvDecStatus ������ͳ����Ϣ�ṹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetDecoderStatis(TKdvDecStatis &tKdvDecStatis );	

	
/** 
* StartVideoDec																																		
* ��ʼͼ�����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartVideoDec();

/** 
* StopVideoDec																																		
* ֹͣͼ�����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopVideoDec();

/** 
* StartAudioDec																																		
* ��ʼ��������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartAudioDec();

/** 
* StopAudioDec																																		
* ֹͣ��������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopAudioDec();

/** 
* SetAudioVolume																																		
* ���������������
* @param[in]  u8 byVolume ������С 0��С��255���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudioVolume(u8 byVolume );

/** 
* GetAudioVolume																																		
* �õ������������
* @param[out]  u8 &pbyVolume ������С 0��С��255���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetAudioVolume(u8 &pbyVolume );	

/** 
* SetAudioMute																																		
* ���þ���
* @param[in]  BOOL32 bMute �Ƿ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudioMute(BOOL32 bMute);
	
/** 
* StartRcvVideo																																		
* ��ʼ�������ͼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartRcvVideo();

/** 
* StopRcvVideo																																		
* ֹͣ�������ͼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopRcvVideo();

/** 
* StartRcvAudio																																		
* ��ʼ�����������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StartRcvAudio();
	
/** 
* StopRcvAudio																																		
* ֹͣ�����������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    StopRcvAudio();

/** 
* SetVideoNetRcvParam																																		
* ����ͼ���������ղ���
* @param[in]  const TLocalNetParam *ptLocalNetParam ������ղ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetVideoNetRcvParam(const TLocalNetParam *ptLocalNetParam );
	
/** 
* SetAudioNetRcvParam																																		
* ����������������ղ���
* @param[in]  const TLocalNetParam *ptLocalNetParam ������ղ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudioNetRcvParam(const TLocalNetParam *ptLocalNetParam );
	
/** 
* SetNetFeedbackVideoParam																																		
* ����ͼ��������ش�����
* @param[in]  TNetRSParam tNetRSParam �����ش�����
* @param[in]  BOOL32 bRepeatSnd       �Ƿ����ش�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetNetFeedbackVideoParam(TNetRSParam tNetRSParam, BOOL32 bRepeatSnd = FALSE);

/** 
* SetNetFeedbackAudioParam																																		
* ���������������ش�����
* @param[in]  TNetRSParam tNetRSParam �����ش�����
* @param[in]  BOOL32 bRepeatSnd       �Ƿ����ش�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetNetFeedbackAudioParam(TNetRSParam tNetRSParam, BOOL32 bRepeatSnd = FALSE);

/** 
* SetVidDecDblSend																																		
* ���÷��͵ڶ�·��Ƶ
* @param[in]  const TNetSndParam *ptNetSndVideoParam ��Ƶ���Ͳ���������NULL��ʾ�ر�
* @param[in]  u16 wBitRate  ����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetVidDecDblSend(const TNetSndParam *ptNetSndVideoParam, u16 wBitRate = 4<<10);

/** 
* SetAudDecDblSend																																		
* ���÷��͵ڶ�·��Ƶ
* @param[in]  const TNetSndParam *ptNetSndAudioParam ��Ƶ���Ͳ���������NULL��ʾ�ر�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudDecDblSend(const TNetSndParam *ptNetSndAudioParam); 

/** 
* ClearWindow																																		
* ����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    ClearWindow(); 

/** 
* FreezeVideo																																		
* ��Ƶ����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    FreezeVideo();

/** 
* UnFreezeVideo																																		
* ȡ����Ƶ����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    UnFreezeVideo();

/** 
* SetVideoActivePT																																		
* ������Ƶ��̬�غ�
* @param[in]  u8 byRmtActivePT  ���յ��Ķ�̬�غɵ�Playloadֵ
* @param[in]  u8 byRealPT       �ö�̬�غ�ʵ�ʴ���ĵ�Playload���ͣ���ͬ�����ǹ�˾��PayloadԼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetVideoActivePT( u8 byRmtActivePT, u8 byRealPT );
	//������Ƶ��̬�غɲ��

/** 
* SetVideoPayloadTable																																		
* ���ò�����Ƶ��̬�غɱ�
* @param[in]  TRtpPayload atPayloadTable[]   ��̬�غɱ�
* @param[in]  u32 dwPayloadNum               ����ĸ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetVideoPayloadTable(TRtpPayload atPayloadTable[], u32 dwPayloadNum);

/** 
* SetVideoCallBackWithoutSPSPPS																																		
* ͸��kdvmedainet �ӿڣ�����kdvmedianet �Ƿ�����SPSPPS�������Ҳ�ص���Ĭ��ֵΪfalse.���ص�
* @param[in]  BOOL32 bNoPPSSPSStillCallback �Ƿ�ص�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16 SetVideoCallBackWithoutSPSPPS(BOOL32 bNoPPSSPSStillCallback);

/** 
* SetVidDecryptKey																																		
* ���� ��Ƶ����key�ִ� �Լ� ����ģʽ
* @param[in]  s8 *pszKeyBuf  key�ַ���
* @param[in]  u16 wKeySize   key�ַ�������
* @param[in]  u8 byDecryptMode   ����ģʽ Aes or Des
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetVidDecryptKey(s8 *pszKeyBuf, u16 wKeySize, u8 byDecryptMode); 

/** 
* SetAudioActivePT																																		
* ������Ƶ��̬�غ�
* @param[in]  u8 byRmtActivePT  ���յ��Ķ�̬�غɵ�Playloadֵ
* @param[in]  u8 byRealPT       �ö�̬�غ�ʵ�ʴ���ĵ�Playload���ͣ���ͬ�����ǹ�˾��PayloadԼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetAudioActivePT( u8 byRmtActivePT, u8 byRealPT );

/** 
* SetAudioPayloadTable																																		
* ���ò�����Ƶ��̬�غɱ�
* @param[in]  TRtpPayload atPayloadTable[]   ��̬�غɱ�
* @param[in]  u32 dwPayloadNum               ����ĸ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetAudioPayloadTable(TRtpPayload atPayloadTable[], u32 dwPayloadNum);

/** 
* SetAudDecryptKey																																		
* ���� ��Ƶ����key�ִ� �Լ� ����ģʽ
* @param[in]  s8 *pszKeyBuf  key�ַ���
* @param[in]  u16 wKeySize   key�ַ�������
* @param[in]  u8 byDecryptMode   ����ģʽ Aes or Des
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudDecryptKey(s8 *pszKeyBuf, u16 wKeySize, u8 byDecryptMode); 

/** 
* SetVidDropPolicy																																		
* ������Ƶ�����������
* @param[in]  u8 byDropPolicy  ������Ƶ�����������
* @param[in]  u16 wKeySize   key�ַ�������
VID_DROP_PREFERRED_QUALITY     �������ȣ�Ϊ��I֡����
VID_DROP_PREFERRED_FAST_SPEED  �ٶ����ȿ쵵���Իָ������ж�����ֻ�ⲻ����֡��Ϊ1����
VID_DROP_PREFERRED_MID_SPEED   �ٶ������е����Իָ������ж�����ֻ�ⲻ����֡��Ϊ2����
VID_DROP_PREFERRED_SLOW_SPEED  �ٶ������������Իָ������ж�����ֻ�ⲻ����֡��Ϊ4����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetVidDropPolicy(u8 byDropPolicy);

/** 
* SetVideoRtpPublicAddr																																		
* ������Ƶ���յĹ�����ַ����
* @param[in]  u32 dwRtpPublicIp  ����IP
* @param[in]  u16 wRtpPublicPort  �����˿�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16	   SetVideoRtpPublicAddr(u32 dwRtpPublicIp, u16 wRtpPublicPort);

/** 
* SetAudioRtpPublicAddr																																		
* ������Ƶ���յĹ�����ַ����
* @param[in]  u32 dwRtpPublicIp  ����IP
* @param[in]  u16 wRtpPublicPort  �����˿�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16	   SetAudioRtpPublicAddr(u32 dwRtpPublicIp, u16 wRtpPublicPort);
	
/** 
* SetDecoderMode																																		
* ���ý���ģʽ
* @param[in]  BOOL32 bUserTimer TRUE��ʾ��ʱ���룬FALSE��ʾ����ʱ����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetDecoderMode(BOOL32 bUserTimer);

/** 
* SetNoDeinterlacing																																		
* ����deinterlacing����
* @param[in]  BOOL32 bNoDeinterlacing TRUE��ʾdeinterlacing�ر� FALSE��ʾ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetNoDeinterlacing(BOOL32 bNoDeinterlacing);

/** 
* SetWatchMoveRect																																		
* �����ƶ����۲����
* @param[in]  LPRECT pRect �������
* @param[in]  u8 bySize    ��С
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetWatchMoveRect(LPRECT pRect, u8 bySize);

/** 
* SetWatchColor																																		
* ���ù۲���ɫ
* @param[in]  COLORREF crWatch  ��ɫ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetWatchColor(COLORREF crWatch);

/** 
* SetNetDithering																																		
* ��������綶�������������ٶȣ���ʱ������Ч��
* @param[in]  BOOL32 bUser �Ƿ�ʹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetNetDithering(BOOL32 bUser);

/** 
* RegStaticPicCallBack																																		
* ��Ƶ�����֡��Ϣ�ص�
* @param[in]  PSTATICPICCALLBACK pCallBack �ص�����
* @param[in]  KD_PTR pvParam               �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    RegStaticPicCallBack(PSTATICPICCALLBACK pCallBack, KD_PTR pvParam);

/** 
* RegFrameInfoCallBack																																		
* ��Ƶ����ǰ֡��Ϣ�ص�
* @param[in]  PFRAMEINFOCALLBACK pFrameInfo �ص�����
* @param[in]  KD_PTR pvParam                �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    RegFrameInfoCallBack(PFRAMEINFOCALLBACK pFrameInfo, KD_PTR pvParam);

/** 
* RegFrameCallBack																																		
* ��Ƶ������֡��֡��Ϣ�ص�����RegFrameInfoCallBack�ص�λ�ò�ͬ��������֡����ܻᾭ�����ſ��ƴ����п��ܻᶪ��һ����֡���ݣ�֮��Ż���룩
* @param[in]  PFRAMECALLBACK pFrame �ص�����
* @param[in]  KD_PTR pvParam        �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    RegFrameCallBack(PFRAMECALLBACK pFrame, KD_PTR pvParam);

/** 
* RegAudioFrameInfoCallBack																																		
* ��Ƶ�����֡��Ϣ�ص�
* @param[in]  PAUDIOFRAMEINFOCALLBACK pFrameInfo �ص�����
* @param[in]  KD_PTR pvParam                     �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    RegAudioFrameInfoCallBack(PAUDIOFRAMEINFOCALLBACK pFrameInfo, KD_PTR pvParam);

/** 
* RegAudioFrameCallBack																																		
* ��Ƶ����ǰ֡��Ϣ�ص�
* @param[in]  PFRAMECALLBACK pFrame �ص�����
* @param[in]  KD_PTR pvParam        �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    RegAudioFrameCallBack(PFRAMECALLBACK pFrame, KD_PTR pvParam);

/** 
* SetTransDataCallBack																																		
* ������Ƶ֡��Ϣ��Я����͸�����ݣ�GPS��Ϣ���ص�������ÿ֡�ص�
* @param[in]  PDATAINFOCALLBACK pFun �ص�����
* @param[in]  KD_PTR pvContext       �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16 SetTransDataCallBack(PDATAINFOCALLBACK pFun, KD_PTR pvContext);

/** 
* SetDrawCallBack																																		
* ���û�ͼ�ص�
* @param[in]  PDRAWCALLBACK pFun   �ص�����
* @param[in]  KD_PTR pvParam       �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetDrawCallBack(PDRAWCALLBACK pFun, KD_PTR pvParam);

/** 
* GetAudioMute																																		
* �õ�����״̬
* @param[out]  BOOL32& bMute   �Ƿ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetAudioMute(BOOL32& bMute);

/** 
* SetAudioAdpcmDiscardStereo																																		
* ����adpcm��Ƶ����˫����
* @param[in]  BOOL32 bDiscard �Ƿ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetAudioAdpcmDiscardStereo(BOOL32 bDiscard);

/** 
* GetAudioAdpcmDiscardStereo																																		
* ���adpcm��Ƶ�Ƿ���˫����
* @param[out]  BOOL32 bDiscard �Ƿ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetAudioAdpcmDiscardStereo(BOOL32& bDiscard);

/** 
* GetWrapperVer																																		
* �õ�Wrapper�İ汾�ͱ�����Ϣ
* @param[out]  s8 *pszVerBuffer �汾��Ϣ�ַ���
* @param[out]  u8 bySize        �ַ������ȣ�bysize��ò�С��128 
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    GetWrapperVer(s8 *pszVerBuffer, u8 bySize);
    
/** 
* SetShowMargins																																	
* ����ͼ��ñ�
* @param[in]  s32 nLeft    ��
* @param[in]  s32 nTop     ��
* @param[in]  s32 nRight   ��
* @param[in]  s32 nBottom  ��
* @param[in]  BOOL32 bAuto bAutoΪTRUEʱ��ǰ�ĸ�������ʾ�ĸ������ϱ��õĿ������CIFʱ��������,�������ͼ���С�ı�,bAutoΪFALSeʱ��ǰ�ĸ������趨���������ͼ���С�ı�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SetShowMargins(s32 nLeft, s32 nTop, s32 nRight, s32 nBottom, BOOL32 bAuto = TRUE);

/** 
* SetPlayHwnd																																		
* ���ò��Ŵ��ھ��
* @param[in]  HWND hPlayWnd         ���ھ��
* @param[in]  BOOL32 bSaveLastFrame �Ƿ񱣴����һ֡
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetPlayHwnd( HWND hPlayWnd, BOOL32 bSaveLastFrame = FALSE );

/** 
* SetZoomWnd																																		
* �������Ŵ��ں�����
* @param[in]  HWND hPlayWnd         ���ھ����NULL��ʾȡ�����Ŵ���
* @param[in]  LPRECT pImgRect       �������꣬pImgRect��Ӧ����ԭʼͼ���һ���ֲ���Χ�����������Ŵ��ڵľֲ���Χ,ԭʼͼ��Ŀ�ߴӺ���GetDecoderStatis��ȡ
*                                   ���þ��ε������������ͼ��ķ��أ��������н�ȡ�������ȫ����ͼ��ķ�Χ�ڣ���ܾ���
*                                   ���Ŵ��ں������ڵ�ǰ��˳��Ҫ�趨�ã���ͼʱ�Ȼ������ţ��ٻ���������
*                                   ���������˸��ȥMSDN�鿴WS_CLIPSIBLINGS��SetWindowPos.����һ�´������Ժ�ǰ��˳��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��   
* @par �޸���־ 
*      wxh��2010-12-30����  
*/
    BOOL32 SetZoomWnd(HWND hPlayWnd, LPRECT pImgRect);
    
/** 
* RedrawLastFrame																																		
* �����ػ�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    RedrawLastFrame();
	
/** 
* SaveAsPic																																		
* ͼ�񱣴�
* @param[in]  LPCSTR lpszFileName   �ļ���
* @param[in]  u8 byEncodeMode       ����ģʽ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
    u16    SaveAsPic(LPCSTR lpszFileName, u8 byEncodeMode);

/** 
* SetOnlyKeyFrame																																		
* �����Ƿ�ֻ��ؼ�֡ 
* @param[in]  BOOL32 bKeyFrame   TRUEΪ���ã�FALSEΪȡ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetOnlyKeyFrame(BOOL32 bKeyFrame);

/** 
* SetAudioCallBack																																		
* ������Ƶ������֡��ص� 
* @param[in]  PAUDIOCALLBACK pAudioCallBack  �ص�����
* @param[in]  KD_PTR pvContext               �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetAudioCallBack(PAUDIOCALLBACK pAudioCallBack, KD_PTR pvContext);

/** 
* SetOutputAudioPowerCB																																		
* ���û�ȡ�����Ƶ���ʵĻص� 
* @param[in]  PGETAUDIOPOWER pGetOutputAudioPowerCB  �ص�����
* @param[in]  KD_PTR pvContext                       �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	u16    SetOutputAudioPowerCB(PGETAUDIOPOWER pGetOutputAudioPowerCB, KD_PTR pvContext);

/** 
* SetWaitForVB																																		
* ���ô�ֱͬ��
* @param[in]  BOOL32 bEnable     �Ƿ�ֱͬ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��    
*/
	VOID   SetWaitForVB(BOOL32 bEnable);

/** 
* SetZoomNum																																		
* �������Ŵ���
* @param[in]  s8 byZoomNum ���Ŵ��� ��0~3���������ã��ڲ������Զ����Ų��ԣ���0�����޸�ǰһ��������һ�ε�λ�����ô���3��ֵ��3��Ч
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨�� 
* @par �޸���־ 
*      gxk��2011-06-01����    
*/
	VOID    SetZoomNum(s8 byZoomNum);

/** 
* InputFrame																																		
* ����֡����
* @param[in]  PFRAMEHDR pFrame  ֡�ṹ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16    InputFrame(PFRAMEHDR pFrame);

/** 
* SetVideoSample																																		
* ������Ƶʱ���������
* @param[in]  u32 dwSample ʱ���������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16 SetVideoSample(u32 dwSample);
	
/** 
* SetAudioSample																																		
* ������Ƶʱ���������
* @param[in]  u32 dwSample ʱ���������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16 SetAudioSample(u32 dwSample);

/** 
* GetLastError																																		
* ��ȡ���µĴ�����
* @param[out]  u32 &dwErrorId    ������
*/
    VOID   GetLastError( u32 &dwErrorId );

/** 
* OpenFile																																		
* ���ļ�
* @param[in]  TPlayFileParam *ptParam �ļ�����
* @param[out]  TKdmPlayFileInfo* ptPlayFileInfo �ļ�������Ϣ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		OpenFile(/*IN*/TPlayFileParam *ptParam, /*OUT*/ TKdmPlayFileInfo* ptPlayFileInfo);

/** 
* CloseFile																																		
* �ر��ļ�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		CloseFile();

/** 
* PlayFile																																		
* �����ļ�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		PlayFile();

/** 
* StopFile																																		
* ֹͣ�����ļ�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		StopFile();

/** 
* SetPlayPos																																		
* ���ò����ļ�λ��
* @param[in]  u32	dwSec  ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		SetPlayPos(u32	dwSec );

/** 
* PauseFile																																		
* ��ͣ�����ļ�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		PauseFile();

/** 
* ResumeFile																																		
* ��ͣ�����»ָ������ļ�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		ResumeFile();

/** 
* SingleFrame																																		
* ��֡����ǰ�����ţ�ֻ����Ƶ֡��������Ƶ֡
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		SingleFrame();

/** 
* SingleFrameForWmv																																		
* ��֡����ǰ�����ţ���asfתwmv�����ã���֡��ǰ���ţ�������Ƶ����Ƶ֡��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     SingleFrameForWmv();

/** 
* GetStreamPropertyByIndex																																		
* ��������Ż����������Ϣ
* @param[in]  u8 byStreamIndex  �����
* @param[out]  void * pioStreamProperty
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
    u16     GetStreamPropertyByIndex(u8 byStreamIndex, void * pioStreamProperty);//(TStreamProperty)

/** 
* FastPlay																																		
* ��ţ�Ŀǰ���ã�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		FastPlay();

/** 
* FastPlay																																		
* ���
* @param[in]  u8 rate �������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		FastPlay(u8 rate);

/** 
* SlowPlay																																		
* ���ţ�Ŀǰ���ã�
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		SlowPlay();

/** 
* SlowPlay																																		
* ����
* @param[in]  u8 rate ��������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		SlowPlay(u8 rate);

/** 
* SetPlayAudioStreamNum																																		
* ���ò�����Ƶ��
* @param[in]  u8 byAudioStreamNum Ĭ�ϲ��ŵ�һ·��Ƶ��byAudioStreamNumΪ1��byAudioStreamNumΪ2ʱ��ʾ���ŵ�2·��Ƶ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     SetPlayAudioStreamNum(u8 byAudioStreamNum);

/** 
* GetFileInfo																																		
* ��ȡ�ļ���Ϣ
* @param[out]  TPlayFileInfo *ptFileInfo �ļ���Ϣ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		GetFileInfo(TPlayFileInfo *ptFileInfo );
	
/** 
* GetPlayStatus																																		
* ��ȡ����״̬
* @param[out]  TPlayFileStatus *ptPlayFileStatus  ����״̬
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16		GetPlayStatus(TPlayFileStatus *ptPlayFileStatus);

/** 
* AsfReadCallBackProc																																		
* ���ļ��ص�
* @param[in]  u32 dwStatus                  ����״̬
* @param[in]  u32 dwNextFrameTimeStamp      ��һ֡ʱ���
* @param[in]  TStreamDataPayload *ptPayload �ص�������Ϣ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	void	AsfReadCallBackProc(u32 dwStatus, u32 dwNextFrameTimeStamp, TStreamDataPayload *ptPayload);

//�����ýӿ�
/** 
* ResetAllBuffers																																		
* ���û���
* @param[in]  u8 byResetType �������ͣ��μ�eResetBufType
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     ResetAllBuffers(u8 byResetType);

/** 
* SetReversePlay																																		
* ���õ���
* @param[in]  BOOL32 bReversePlay  �Ƿ񵹷�
* @param[out] u32* pdwTimestamps   ���ص�ǰ����ʱ��ȷ��ʱ���
* @param[in]  BOOL32 bAllFrames    �Ƿ�ȫ֡�ʲ���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     SetReversePlay(BOOL32 bReversePlay, u32* pdwTimestamps, BOOL32 bAllFrames = FALSE);

/** 
* SetDrawTimestampsCallback																																		
* ���û�ͼʱ����ص�
* @param[in]  PDRAWTIMESTAMPSCALLBACK pDrawTimestampsCallBack �ص�����
* @param[in]  KD_PTR pvContext                                �ص�������
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     SetDrawTimestampsCallback(PDRAWTIMESTAMPSCALLBACK pDrawTimestampsCallBack, KD_PTR pvContext);

/** 
* SeekTo																																		
* ����ʱ��ת
* @param[in]  u32  dwSeekToTime   ��תʱ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     SeekTo(u32  dwSeekToTime);

/** 
* PauseReversePlay																																		
* ��ͣת����
* @param[in]  BOOL32 bPause �Ƿ���ͣ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     PauseReversePlay(BOOL32 bPause);

/** 
* SetPlayOver																																		
* ���ò��Ž���
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     SetPlayOver();

/** 
* SetFileInfo																																		
* ����ʱ���ò����ļ���Ϣ
* @param[in]  TReserveFileInfo *ptFileInfo �����ļ���Ϣ
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     SetFileInfo(TReserveFileInfo *ptFileInfo);

/** 
* GetPlayTime																																		
* ��ȡ���ŵĽ���
* @param[out]  s32 * pnTimeStamps ���ȣ���λms��������ʾδ�յ�����
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
	u16     GetPlayTime(s32 * pnTimeStamps);

protected:
public:
    CKdvVideoDec       *m_pcKdvVideoDec;//����
    CKdvAudioDec       *m_pcKdvAudioDec;//����
    TDecoderRef	       *m_ptRef;   //����
    TDecoderStatus     *m_ptStatus;//����


	TPlayFileParam		m_tFilePlayParam ;
	TPlayFileStatus		m_tFilePlayStatus ;
	TPlayFileInfo		m_tFilePlayInfo ;

	u32					m_dwVideoFrameNum ;
	u32					m_dwAudioFrameNum ;
	u32					m_dwVFrameNum;
	u32					m_dwAFrameNum;
	u32					m_dwCurTime;
	u32					m_dwLastTime;
	u8                  m_byAudioStreamNum;
	IKdmFileReader	*m_pcAsfReader ;
protected:

	BOOL32         m_bKeyFrame;
    u8		       m_byFlag;
	//	��������������
	SEMHANDLE			m_hSem ;
	BOOL32				m_bSemCreate ;
	BOOL32				m_bCreate;

	BOOL32				m_bDecoderMode ;
};


class CAVCapabilityList;
class CAVCapCheck  
{
public:
    CAVCapCheck();
    ~CAVCapCheck();
public:
	BOOL32 CheckAudInCap();
	BOOL32 CheckVidInCap();
	BOOL32 CheckAudOutCap();
	BOOL32 CheckVidOutCap();
	BOOL32 CheckAudEncCap(u8 byMediaType);
	BOOL32 CheckVidEncCap(u8 byMediaType);
	BOOL32 CheckAudDecCap(u8 byMediaType);
	BOOL32 CheckVidDecCap(u8 byMediaType);	

private:
	CAVCapabilityList *m_ptCAVCapList;
};

/** 
* SoftDecoderInit																																		
* ���������ʼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
u16		SoftDecoderInit();

/** 
* SoftDecoderRelease																																		
* �����������ʼ��
* @retval  CODEC_NO_ERROR  �ɹ� 
* @retval  �μ������붨��     
*/
u16		SoftDecoderRelease();

#endif /*__CODECLIB_H*/














