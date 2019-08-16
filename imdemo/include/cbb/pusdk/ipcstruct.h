//////////////////////////////////////////////////////////////////////////
// ipcv7struct.h 

#ifndef IPCSTRUCT_H_
#define IPCSTRUCT_H_

#pragma warning(disable: 4103)
#include "ipcdefine.h"
#pragma pack(push, 1)

#define MAX_USER_NAME                64   // ����û�������(64λ)
#define MAX_USER_PASSWORD           64   // ����û����볤��(64λ)
#define MAX_EXPAND_PATH              1024 // �����չ���ȣ�1024��
// Handle����
typedef struct tagHandleParam
{
	unsigned long dwIpAddr;
	unsigned short wPort;
	char szUserNmae[MAX_USER_NAME];
	char szUserPassword[MAX_USER_PASSWORD];
	u32 dwReserved;
} THANDLEPARAM, *PTHANDLEPARAM;
// �������
typedef struct tagNetParam
{
	unsigned long   dwIpAddr;	                                      // IP��ַ(������)
	unsigned short  wPort;		                                      // �˿ں�(1024-65535)
} TNET_PARAM, *PTNET_PARAM;

// ������ؽṹ�����(UDP)
typedef struct tagPlayVideoInfo
{
	u16	wRtcpVideoPort;	     // ���ص���Ƶrtcp�˿�(��netʹ��)
	u16 wRtcpAudioPort;	     // ���ص���Ƶrtcp�˿�(��netʹ��) 
}TPLAYVIDEOINFO, *PTPLAYVIDEOINFO;


// ����ṹ�����
typedef struct tagPlayVideoParam
{
	u8   byVideoSouce;		// ��ƵԴID
	char szStreamType[17];	// ������������� ---vedio,audio,both,ps
	char szTransType[17];	// ����Э������   ---rtp_udp,rtp_tcp,rtsp_tcp,rtsp_udp
	u8   byStreamChn;		// ����ͨ��
	char szAdmin[32];		// ǰ�˷��ص��˺�
	char szPassword[32];	// ǰ�˷��ص�����
	u32  dwSrcIP;			// ���ؽ���IP
	u16	 wRtspPort;			// ǰ�˼����˿�
	u16  wRtpVideoPort;		// ��Ƶ RTP�˿�
	u16  wRtcpVideoPort;	// ��Ƶ RTCP�˿�
	u16  wRtpAudioPort;		// ��Ƶ RTP�˿�
	u16  wRtcpAudioPort;	// ��Ƶ RTP�˿�
	u16  wRtpAudioPort2;	// ��Ƶ2 RTP�˿�
	u16  wRtcpAudioPort2;	// ��Ƶ2 RTP�˿�
	char szMediaURL[MAX_PATH];// url
	u32	 hPlayWnd;			// ���Ŵ��ھ��
}TPLAYVIDEOPARAM, *PTPLAYVIDEOPARAM;

// ����״̬
typedef struct tagRTCPCallStateInfo
{
	bool bCallState;
}TRTCPCALLSTATEINFO, PRTCPCALLSTATEINFO;

// ׼�����з��ؽṹ�����
typedef struct tagRTCPPrepareCallInfo 
{
	u32	dwID;
	u16	wRtpPort;
	u16 wRtcpPort;
	char szStreamType[17];
}TRTCPPREPARECALLINFO, *PRTCPPREPARECALLINFO;

// RTSP�����Ϣ
typedef struct tagRTSPParam
{
	u8	byVideoSource;	// ��ƵԴID
	u16 wVideoChanID;	// ����ͨ��
}TRTSPPARAM, *PRTSPPARAM;

// RTSP���������Ϣ
typedef struct tagRTSPInfo
{
	char szurl[MAX_PATH];
	u16	wRtspPort;	
	bool bDoubleAudio;  // �Ƿ�֧��˫��Ƶ
}TRTSPINFO, *PRTSPINFO;

// ��������
typedef struct tagRTCPVideoVolumeInfo
{
	bool bAudioEnable;		// �Ƿ�����Ƶ
	u8	 byVolume;			// ����ֵ
}TRTCPVIDEOVOLUMEINFO, *PRTCPVIDEOVOLUMEINFO;

//��Ƶ
typedef struct tagMultiVideoModeInfo
{
	char szMode[17];	//��������˫���������������������� "1,2,3,4"
}TMULTIVIDEOMODEINFO, *PTMULTIVIDEOMODEINFO;

typedef struct tagMultiVideoModeParam
{
	u8	byVideoSource;
	TMULTIVIDEOMODEINFO	tMultiVideoInfo;
}TMUTIVIDEOMODEPARAM, *PTMULTIVIDEOMODEPARAM;

// ��Ƶ����
typedef struct tagVideoFrameCapList
{
	char szFrameRateCap[64];			// �����ʽ����
	u8	 byMin;							// ��С����֡��
	u8	 byMax;							// ������֡��
	u8	 byDef;							// Ĭ�ϱ���֡��
}TVIDEOFRAMECAPLIST, *PVIDEOFRAMECAPLIST;

typedef struct tagVideoCapList
{
	u8	 byStreamTypeCap;				// ��������
	u8	 byStreamIdCap;					// ����ID
	char szResolutionCap[128];			// �ֱ�����������
	char szEncoderComplexityCap[64];	// ���븴�Ӷ�����
	TVIDEOFRAMECAPLIST tVideoCapList[5]; //֡����������
}TVIDEOCAPLIST, *PVIDEOCAPLIST;

typedef struct tagVideoEnc
{
	char		szResolution[17];		// ��ǰ�ֱ���
	char		szDataRateType[17];		// ���ݸ�ʽ����
	u16			wMaxDataRate;			// ��������(�����Сֵ����ӿڵ�szAssistInfo����)
	char		szQuality[17];			// opt=��lowest,lower,low,middle,hight,highest��
	u8			byFrameRate;			// (�������tagVideoFrameCapList)
	char		szVideoType[17];		//(������������ӿڵ�szAssistInfo����)
	char		szEncoderComplexity[17];
	u16			wIFrameSpeed;			//i֡���(�����Сֵ����ӿڵ�szAssistInfo����)
}TVIDEOENC, *PTVIDEOENC;

typedef struct tagVideoEncInfo
{
	TVIDEOCAPLIST	tVideoCapList[10];	// ���10��
	TVIDEOENC		tVideoEnc;
}TVIDEOENCINFO, *PTVIDEOENCINFO;

typedef struct tagVideoEncParam
{
	u8	byVideoSource;
	u8	byStreamId;		// ������(1),������(2),������(3),������(4)
	TVIDEOENC	tVideoEnc;
}TVIDEOENCPARAM, *PTVIDEOENCPARAM;

// ��Ƶ����
typedef struct tagAudioEncInfo
{
	u8	byVolume;	// ��������0-100
	bool  bAec;		// ����¼��
	char  szType[17];	// �����ʽ��PCMA/PCMU/ADPCM/G722/G728/G7221c/AACLC��
	char  szInputMode[17];	//��Ƶ����ģʽ��linein,micin��
	char  szSampling[17];	//��ǰ������
}TAUDIOENCINFO, *PTAUDIOENCINFO;

typedef struct tagAudioEncParam
{
	u8	byVideoSource;
	u8	byStreamId;		// ������(1),������(2),������(3),������(4)
	TAUDIOENCINFO	tAudioEncInfo;
}TAUDIOENCPARAM, *PTAUDIOENCPARAM;

// ��Ƶ����
typedef struct tagAudioDecInfo
{
	u8	byVolume;		// �������� 0-100
	bool bMixerRec;		// ����¼��
	bool bModeEnable;	// ��Ƶ�����Ƿ�����
	char szTypeList[64];	//��������б�, ֻ��
	char szOutputType[17];	//������� lineout,speaker
}TAUDIODECINFO, *PTAUDIODECINFO;

typedef struct tagAudioDecParam
{
	u8	byVideoSource;
	u8	byStreamId;		// ������(1),������(2),������(3),������(4)
	TAUDIODECINFO	tAudioDecInfo;
}TAUDIODECPARAM, *PTAUDIODECPARAM;

// ͼ���ڱ�
typedef struct tagShieldArea
{
	char szName[17];	// ������������ Ŀǰ��֧��
	char szColor[17];	// �������ɫ"gray,red,yellow,blue,orange,green,transparent,half-transparent,mosaic" Ŀǰ��֧��
	char szType[17];	// Ŀǰ֧��rect
	u8	byPointNum;		// ��ĸ�����rectΪ4����
	u16	x[8];			// x
	u16	y[8];			// y
}TSHIELDAREA, *PTSHLELDAREA;

typedef struct tagVideoShield
{
	bool bEnable;		// ����
	u16	 wVgaWidth;		// ���������ȣ�Ĭ��Ϊ10000
	u16	 wVgaHeight;	// ��������߶ȣ�Ĭ��Ϊ10000
	u16	 wMinWidth;		// ��С�����ȣ������޸�
	u16	 wMinHeight;	// ��С����߶ȣ������޸�
	char szAllColor[17];// �����������ɫgray,red,yellow,blue,orange,green,transparent,half-transparent,mosaic
	u8	 byAreaNum;		// ���������Ŀǰ���֧��4��
}TVIDEOSHIELD, *PTVIDEOSHIELD;

typedef struct tagVideoShieldInfo
{	
	TVIDEOSHIELD	tVideoShield;
	TSHIELDAREA		tShieldArea[4];		// Ŀǰ֧��4������
}TVIDEOSHIELDINFO, *PTVIDEOSHIELDINFO;

typedef struct	tagVideoShieldParam
{
	u8		byVideoSource;
	TVIDEOSHIELD	tVideoShield;
	TSHIELDAREA		tShieldArea[4];		// Ŀǰ֧��4������
}TVIDEOSHIELDPARAM, *PVIDEOSHIELDPARAM;

// ROI
typedef struct tagVideoRoi
{
	bool	bEnable;	// ����
	u16		wVgaWidth;	// ���������ȣ�Ĭ��Ϊ10000
	u16		wVgaHeight;	// ��������߶ȣ�Ĭ��Ϊ10000
	u16		wMinWidth;	// ��С�����ȣ������޸�--1250
	u16		wMinHeight;	// ��С����߶ȣ������޸�--1250
	char	szCodeLevel[17]; // low , middle , high
}TVIDEOROI, *PTVIDEOROI;

typedef struct tagRoiArea
{
	char szType[17];	// Ŀǰ֧��rect
	u8	byPointNum;		// ��ĸ�����rectΪ4����
	u16	x[4];			// x
	u16	y[4];			// y
}TROIAREA, *PTROIAREA;

typedef struct tagVideoRoiInfo
{	
	TVIDEOROI		tVideoRoi;
	TROIAREA		tRoiArea;		// Ŀǰ֧��1������
}TVIDEOROIINFO, *PTVIDEOROIINFO;

typedef struct	tagVideoRoiParam
{
	u8		byVideoSource;
	TVIDEOROI	tVideoRoi;
	TROIAREA	tRoiArea;
}TVIDEOROIPARAM, *PVIDEOROIPARAM;

// ����ü�
typedef struct tagVideoEncCut
{
	bool	bEnable;	// ����
	u16		wVgaWidth;	// ���������ȣ�Ĭ��Ϊ10000
	u16		wVgaHeight;	// ��������߶ȣ�Ĭ��Ϊ10000
	u16		wMinWidth;	// ��С�����ȣ������޸�--5500
	u16		wMinHeight;	// ��С����߶ȣ������޸�--5500
}TVIDEOENCCUT, *PTVIDEOENCCUT;

typedef struct tagEncCutArea
{
	u16		wX;			// �󶥵�xֵ
	u16		wY;			// yֵ
	u16		wWidth;		// ���
	u16		wHeight;	// �߶�
}TENCCUTAREA, *PTENCCUTAREA;

typedef struct tagVideoEncCutInfo
{
	TVIDEOENCCUT	tVideoEncCut;
	TENCCUTAREA		tEncCutArea;
}TVIDEOENCCUTINFO, *PTVIDEOENCCUTINFO;

typedef struct tagVideoEncCutParam
{
	u8	byVideoSource;
	TVIDEOENCCUT	tVideoEncCut;
	TENCCUTAREA		tEncCutArea;
}TVIDEOENCCUTPARAM, *PTVIDEOENCCUTPARAM;

// OSD
typedef struct tagOsdStateInfo
{
	char szState[17];		//��prepare,uploading,idle��
	u8	byProcess;			// ����0-100
}TOSDSTATEINFO,*PTOSDSTATEINFO;

typedef struct tagOsdPrepareParam
{
	u8	 byVideoSource;
	char szPicId[17];		// ͼƬID opt=��osd,label,time,alarm, move,shieldalarm��
	bool bIsAlarm;			// �Ƿ�澯����
	u8	 byPicSn;			// ͼƬ��� osd, alarminר��
	u16	 wFileLen;			// ͼƬ�ļ�����
	u16	 wSendLenMax;		// ���η��ͳ�������
}TOSDPREPAREPARAM,*PTOSDPREPAREPARAM;

typedef struct tagOsdCapInfo
{
	char szTimeFormat[33];		// ֧�ֵ�ʱ����ʾ��ʽ��MM-DD-YYYY,YYYY-MM-DD,DD-MM-YYYY��
	char szFontName[33];		// ֧�ֵ����� "blcak song"
	char szFontColor[33];		// ֧�ֵ�������ɫ��black, white,red,blue��
	char szFontBkColor[17];		// ֧�ֵı�����ɫ "black, white,red,blue"
	char szOsdType[65];			// ֧�ֵ���Ļ���͡�osd,lable,time,alarm,ptz,3g��
}TOSDCAPINFO, *PTOSDCAPINFO;

typedef struct tagOsdUploadFileParam
{
	u8	byVideoSource;
	u16	wStartPos;	// ��ʼ����λ��
	u16 wDataLen;	// ���ݳ���
	char* pData;	// ����
}TOSDUPLOADFILEPARAM, *PTOSDUPLOADFILEPARAM;

typedef struct tagOsdList
{
	char	szOsdType[17];		// time,osd,alarm,label,ptz,3g
	bool	bVisiable;
	u8		bySn;				// osd������Ļר��1-6
	u16		wOsdX;				// ��OSD��xֵ��yֵ
	u16		wOsdY;
	u16		wOsdWidth;			// ��OSD�ĸ߶ȺͿ��
	u16		wOsdHeight;
	u8      bynindex;
	char	szContext[65];		// ��Ļ����
}TOSDLIST, *PTOSDLIST;

typedef struct tagOsdListInfo
{
	TOSDLIST	tOsdList[16];
}TOSDLISTINFO, *PTOSDLISTINFO;

typedef struct tagOsdListParam
{
	u8		byVideoSource;
	u8		byOsdNum;			//����OSD����
	TOSDLIST	tOsdList[16];
}TOSDLISTPARAM, *PTOSDLISTPARAM;

typedef struct tagOsdInfo
{
	bool	bUploadOsdPic;			// �Ƿ��ϴ�ͼƬ
	u16		wResolutionWidth;		// �ֱ��ʿ��
	u16		wResolutionHeight;		// �ֱ��ʸ߶�
	char	szTimeFormat[33];		// ʱ����Ļ��ʽMM-DD-YYYY,YYYY-MM-DD,DD-MM-YYYY
	bool	bTimeSingleRow;			// �Ƿ���
	char	szFontName[17];			// ���� song black
	char	szFontColor[17];		// ������ɫĬ��	white�� ֧��black, white,red,blue
	char	szFontBkColor[17];		// ������ɫĬ�� black�� ֧��black, white,red,blue
	bool	bFontTransparent;		// ����͸��
	bool	bFontEdge;				// �������
	u8		byBackGuageTop;			// �ϱ߾ࣨn���ַ��ĸ߶ȣ����ڱ�Ե���룬0��16��
	u8		byBackGuageBotton;		// �±߾�
	u8		byBackGuageLeft;		// ��߾�
	u8		byBackGuageRight;		// �ұ߾�
}TOSDINFO, *PTOSDINFO;

typedef struct tagOsdParam
{
	u8		byVideoSource;
	char	szTimeFormat[33];		// ʱ����Ļ��ʽMM-DD-YYYY,YYYY-MM-DD,DD-MM-YYYY
	bool	bTimeSingleRow;			// �Ƿ���
	char	szFontName[17];			// ���� song black
	char	szFontColor[17];		// ������ɫĬ��	white�� ֧��black, white,red,blue
	char	szFontBkColor[17];		// ������ɫĬ�� black�� ֧��black, white,red,blue
	bool	bFontTransparent;		// ����͸��
	bool	bFontEdge;				// �������
	u8		byBackGuageTop;			// �ϱ߾ࣨn���ַ��ĸ߶ȣ����ڱ�Ե���룬0��16��
	u8		byBackGuageBotton;		// �±߾�
	u8		byBackGuageLeft;		// ��߾�
	u8		byBackGuageRight;		// �ұ߾�
}TOSDPARAM, *PTOSDPARAM;

// ͼ��
typedef struct tagImgDefParam
{
	u8	byVideoSource;		// ��ƵID
	char szcolorrecov[17];	// 1.�ָ�ȫ��:all, 2.�ָ���ɫֵ:color
}TIMGDEFPARAM, *PTIMGDEFPARAM;

// ͼ��--��ɫ����
typedef struct tagColorInfo
{
	u8	byBrightnessLevel;	//����0-100
	u8	bycContrastLevel;	//�Աȶ�0-100
	u8  bySaturationLevel;	//���Ͷ�0-100
	u8	byHueLevel;			//���0-100
}TCOLORINFO, *PTCOLORINFO;

typedef struct tagColorParam
{
	u8	byVideoSource;
	TCOLORINFO tColorInfo;
}TCOLORPARAM, *PTCOLORPARAM;

// ͼ��-�۽�����
typedef struct tagFocusInfo
{
	char szFocusStyle[17];	//single,continu,auto,manual
	char szFocusMin[17];	//��С�۽����� 1m,2m
	char szFocusMax[17];	//���۽����� 1m,2m
}TFOCUSINFO, *PTFOCUSINFO;

typedef struct tagFocusParam
{
	u8	byVideoSource;					//
	TFOCUSINFO tFocusInfo;
}TFOCUSPARAM, *PTFOCUSPARAM;

// ͼ��-��ƽ�����
typedef struct tagWhiteBlanceInfo
{
	char szWhiteBlaneceStyle[17]; // ��ƽ������
	u8	byBlanceRed;			  // ������ֵ0-100
	u8  byBlanceBlue;			  // ������ֵ0-100
}TWHITEBLANCEINFO, *PTWHITEBLANCEINFO;

typedef struct tagWhiteBlanceParam
{
	u8	byVideoSource;
	TWHITEBLANCEINFO tWhiteBlanceInfo;
}TWHITEBLANCEPARAM, *PTWHITEBLANCEPARAM;

// ͼ��-��ҹ�л�����
typedef struct tagIrcutFixTime
{
	char szstarttime[17];
	char szendtime[17];
	bool bEnable;		// ����true, false
}TIRCUTFIXTIME, *PTIRCUTFIXTIME;

typedef struct tagIrcutWeekTime
{
	TIRCUTFIXTIME	tIrcutWeekdayTime[7];	// ���յ�����
}TIRCUTWEEKTIME, PTIRCUTWEEKTIME;

typedef struct tagIrcutFilterInfo
{
	char szircutfiltertype[17];		// ת������ day,night,auto_gain,auto_photo,time
	char sztrigger[17];				// ����״̬day night
	char szdaytonightlevel[17];		// �ȼ� �� �� ��
	char szdaytonighttime[17];		// ʱ�� (5-20)
	u8	 byPhotoLevel;				// 0-100
	//ֻ����szircutfiltertypeΪtime(��ʱ����)ʱ����Ҫ�������µ�ʱ��ṹ��
	TIRCUTFIXTIME	tIrcutTimeAll;	// szEnable����tIrcutTimeAll��tIrcutWeekdayTime����
	TIRCUTWEEKTIME	tIrcutTimeWeekDay;	// ���յ�����
}TRCUTFILTERINFO, *PTIRCUTFILTERINFO;

typedef struct tagIrcutFilterParam
{
	u8 byVideoSource;
	TRCUTFILTERINFO tImageIrcutFilterInfo;
}TTRCUTFILTERPARAM, *PTIRCUTFILTERPARAM;

// ͼ��--��Ȧ
typedef struct tagIrisInfo
{
	char szIrisMode[17];		//��Ȧģʽ��auto, manual,dc_irisauto, dc_irismanual, p_irisauto, p_irismanual��
	u8	 byIrisSize;			//��Ȧ��С
	u8	 byIrisLevel;			//��Ȧ�ȼ�
}TIRISINFO, *PTIRISINFO;

typedef struct tagIrisParam
{
	u8	byVideoSource;
	TIRISINFO	tIrisInfo;
}TIRISPARAM, *PTIRISPARAM;

// ͼ��--����
typedef struct tagShutterInfo
{
	char szshutermode[17]; // ����ģʽ
	char szshutermin[17];  // ��������
	char szshuterlevel[17];// ������-�ֶ�
}TSHUTTERINFO, *PTSHUTTERINFO;

typedef struct tagShutterParam
{
	u8	byVideoSource;
	TSHUTTERINFO	tShutterInfo;
}TSHUTTERPARAM, *PTSHUTTERPARAM;

// ͼ��-- ����
typedef struct tagGainInfo
{
	char szgainmode[17];	// ����ģʽ
	u8	 byGainMax;			// �������� 0-100
	u8	 byGainLevel;		// ����ȼ� 0-100
}TGAININFO, *PTGAININFO;

typedef struct tagGainParam
{
	u8 byVideoSource;
	TGAININFO tGainInfo;
}TGAINPARAM, PTGAINPARAM;

// ͼ��-- ����
typedef struct tagInfraredInfo
{
	char szControlMode[17];	// ����ģʽ--�رգ��Զ�������Զ�ƿ������еƿ��������ƿ����������ƿ���
	u8	byInfraredLevel;	// ����ǿ��
}TINFRAREDINFO, *PTINFRAREDINFO;

typedef struct tagInfraredParam
{
	u8	byVideoSource;
	TINFRAREDINFO	tInfraredInfo;
}TINFRAREDPARAM, *PTINFRAREDPARAM;

//ͼ��-- ����
typedef struct tagLaserInfo
{
	char szControlMode[17];	 // open, close
	u8	byLaserLevel;		 // ����ǿ��
	char szSpotMode[17];	 // �۹�ǿ�� Ĭ�ϣ����Сģʽ����ߴ�ģʽ���Զ���ģʽ1���Զ���ģʽ2
}TLASERINFO, *PTLASERINFO;

typedef struct tagLaserParam
{
	u8	byVideoSource;
	TLASERINFO	tLaserInfo;
}TLASERPARAM, *PTLASERPARAM;

// ͼ�� --����ģʽ
typedef struct tagCorridoModeInfo
{
	char	szCorridoMode[17];	// ����ģʽclose, left,right
}TCORRIDOMODEINFO, *PTCORRIDOMODEINFO;

typedef struct tagCorridoModeParam
{
	u8	byVideoSource;
	TCORRIDOMODEINFO	tCorridoModeInfo;
}TCORRIDOMODEPARAM, *PTCORRIDOMODEPARAM;

// ͼ�� --����ģʽ
typedef struct tagImageModeInfo
{
	char szImageMode[17];	//����ģʽclose, left,right,topbottom
}TIMAGEMODEINFO, *PTIMAGEMODEINFO;

typedef struct tagImageModeParam
{
	u8	byVideoSource;
	TIMAGEMODEINFO	tImageModeInfo;
}TIMAGEMODEPARAM, *PTIMAGEMODEPARAM;

// ͼ�� --2D����
typedef struct tag2DNoise
{
	char szNoiseReduce2DMode[17]; //2D����ģʽauto,open,close
	u8	 byNoiseReduce2DLevel;
}T2DDENOISE, *PT2DDENOISE;

typedef struct tag2DNoiseParam
{
	u8	byVideoSource;
	T2DDENOISE	tNoiseReduceInfo;
}T2DDENOISEPARAM, *PT2DDENOISEPARAM;

// ͼ�� --3D����
typedef struct tagNoiseReduce3DInfo
{
	char	szNoiseReduce3DMode[17];	// 3D����ģʽauto,open,close
	u8		byNoiseReduce3DLevel;
}T3DDENOISE, *PT3DDENOISE;

typedef struct	tagNoiseReduce3DParam
{
	u8	byVideoSource;
	T3DDENOISE	tNoiseReduceInfo;
}T3DDENOISEPARAM, *PT3DDENOISEPARAM;

// ͼ�� -- ���ػ���
typedef struct tagLocalEchoInfo
{
	char	szLocalEchoMode[17];	// close, pal,ntsc,line,interleave
}TLOCALECHOINFO, *PTLOCALECHOINFO;

typedef struct tagLocalEchoParam
{
	u8	byVideoSource;
	TLOCALECHOINFO	tLocalEchoInfo;
}TLOCALECHOPARAM, *PTLOCALECHOPARAM;

// ͼ�� -- ����˸
typedef struct tagPowerLineFrequencyInfo
{
	char szPowerLineFrequencyMode[17];	// 50hz,60hz,auto
}TPOWERLINEFREQUENCYINFO, PTPOWERLINEFREQUENCYINFO;

typedef struct tagPowerLineFrequencyParam
{
	u8	byVideoSource;
	TPOWERLINEFREQUENCYINFO	tPowerLineFrequencyInfo;
}TPOWERLINEFREQUENCYPARAM, *PTPOWERLINEFREQUENCYPARAM;

// ͼ�� -- ��̬���
//��̬��������
typedef struct tagDynamicArea
{
	char szType[17];	// �������ͣ�����rect
	u8	byPointNum;		// ����Ķ���
	u16	x[16];			// x����	�������꣬˳ʱ��д��
	u16	y[16];			// y����
}TDYNAMICAREA, *PTDYNAMICAREA;

typedef struct tagDynamicModeInfo
{
	char szMode[17];		// ��̬����ģʽ���ر�(close)/��̬(wdr)/ǿ������(hlc)/���ⲹ���Զ�(autoblc)/
							// ���ⲹ���ֶ�(regionblc)/͸��(fogthrough)/Gamma(gamma)/smartIR(smartir)
	u8	nLevel;				// ������
	char szPosition[17];	// ���ڷ�λ �Զ���custom,������������ top,left,right,bottom,center
	u16	wMinWidth;			// ��С���
	u16 wMinHeight;			// ��С�߶�
	u16	wVGAWidth;			// ����Ŀ��
	u16	wVGAHeight;			// ����ĸ߶�
	TDYNAMICAREA	tDynamicArea;
}TDYNAMICMODEINFO, *PTDYNAMICMODEINFO;

typedef struct tagDynamicModeParam
{
	u8	byVideoSource;
	TDYNAMICMODEINFO	tDynamicModeInfo;
}TDYNAMICMODEPARAM, *PTDYNAMICMODEPARAM;

// ���ⲹ���
typedef struct tagFillLight
{
	char szMode[7];   // �����ģʽ: �ر�(close),�Զ�(auto),�ֶ�(manual)
	u16  wBrightness; // ����(0~100)
	u16  wThreshold;  // ��ֵ(0~48)
}TDFILLLIGHT, *PTDFILLLIGHT;

// ��̬���״̬
typedef struct tagDynPlugin
{
	bool bEnable;   // true-����,false-�ر�
}TDDYNMAICPLUGIN, *PTDDYNMAICPLUGIN;

// �豸��Ϣ
typedef struct tagDeviceInfo
{
	char szDevName[MAX_DEV_NAME+1];						// �豸����
	char szDevType[MAX_DEV_NAME+1];						// �豸����
	char szSerial[MAX_DEV_NAME+1];						// ���к�
	char szVerHard[MAX_DEV_NAME+1];						// Ӳ���汾��
	char szVerSoft[MAX_DEV_NAME+1];						// ����汾��
	u8   byNumVideo;									// ��ƵԴ����
	u8   byCardNum;										// �����(MSP1000���뿨��ĸ���ϵı��,����ֻ��)
	u8   bySlotNum;                                     // �۵���(MSP1000���뿨ĸ���ڻ����ϵĲ۵���,����ֻ��)
	u8   byCaseNum;                                     // �����(MSP1000���뿨ĸ�������,����ֻ��)
}TDEVICEINFO,  *PTDEVICEINFO;

// ʱ���ȡ
typedef struct tagSysTimeInfo
{
	char szTimeZone[8];									//ʱ��, ��Ӧ��ʱ��ƫ��ֵ,��λ��
	u32  dwYear;
	u32  dwMonth;
	u32  dwDay;
	u32  dwHour;
	u32  dwMinute;
	u32  dwSecond;
}TSYSTIMEINFO,  *PSYSTIMEINFO;

typedef struct tagTimeAutoInfo
{
	bool  bAuto;
	u8   byMode;										// ntp, inputprotocol,Ҫ�����
	char szNtpUrl[MAX_PATH];
	u16  wNtpPort;
	u16  wNtpTimeSpeed;									// ��
}TTIMEAUTOINFO,  *PTIMEAUTOINFO;

// ����ʱ
typedef struct tagDSTInfo
{
	bool	bEnable;									// ����
	u8  byStartMonth;									// ��ʼ��
	u8  byStartWeek;									// ��ʼ�ܣ������£�1��5��
	u8  byStartDay;										// ��ʼ�죬�����ܣ�1��7��
	u8  byStartHour;									// ��ʼСʱ��1��24��
	u8  byEndMonth;										// ������
	u8  byEndWeek;										// �����ܣ������£�1��5��
	u8  byEndDay;										// �����죬�����ܣ�1��7��
	u8  byEndHour;										// ����Сʱ��1��24
	u8	byOffMinute;									// ƫ��ʱ��,��(0~120)
} TDSTINFO,  * PTDSTINFO;

#define MAX_PROTOCOL_NAME			 32					// ���Э�����Ƴ���(32λ)
typedef struct tagRSInfo
{
	char  	szType[33];									// ��������, rs232,rs485
	u32  	dwBaudrate;									// �����ʣ�2400 4800, 9600, 19200, 38400, 57600 , 76800 ,  115200
	u32  	dwDataBit;									// ����λ��5,6,7,8
	u32  	dwStopBit;;									// ֹͣλ��1,2
	u8  	byAddressNum;								// ��ַ��(1~255)
	char  	szParityBit[17];							// У��λnone , even , odd
	char  	szStreamControl[17];						// ���أ�none, hardctrl, softctrl
	char	szControlProtocol[MAX_PROTOCOL_NAME + 1];	// ����Э�� ����
} TRSINFO,  * PTRSINFO;

typedef struct tagLedInfo
{
	bool  	bOpen;										// �Ƿ�����
	char* 	szTime;										// ��˸��ʱ�䣬5s,10s,30s,1m,30m,60m,24h
} TLEDINFO,  * PTLEDINFO;

typedef struct tagUserActiveInfo
{
	char	szUserNmae[MAX_USER_NAME];              // �û���
	char	szUserPassword[MAX_USER_PASSWORD];		// �û�����
	char    szMailAddr[128];						// ���������ַ
} TUSERACTIVEINFO,  *PTUSERACTIVEINFO;

typedef struct tagUploadInfo
{
	u32  nLenFile;										// �Ƿ�����
	u32  nMaxLen_Send;									// �������͵���󳤶�
	char    szFilePath[MAX_PATH];						// �ļ�·��
} TUPLOADINFO,  *PTUPLOADINFO;

typedef struct tagDownloadInfo
{
	u32  nLenFile;										// �Ƿ�����
	u32  nMaxLen_Send;									// �������͵���󳤶�
	char    szFilePath[MAX_PATH];						// �����ļ�·��
} TDOWNLOADINFO,  *PTDOWNLOADINFO;

typedef struct tagDdnsInfo
{
	bool  bDdnsEnable;									//����DDNS
	char  szDdnsType[8];								//DDNS����
	char  szDevurl[MAX_PATH] ;							//�豸���� 
	char  szUserName[MAX_USER_NAME + 1];				//�û���
	char  szPassWord[MAX_USER_PASSWORD + 1];			//����
}TDDNSINFO, *PTDDNSINFO;

typedef struct tagUploadState
{
	char  szState[17];									// ����״̬ "prepare,uploading,idel"integer 
	u32	  nProgress;									// �������� 0��100
} TUPLOADSTATE, *PTUPLOADSTATE;

// Ȩ��
typedef struct tagPowerInfo
{
	char	szType[17];		// �û�����: administrator, operator, viewer
	bool	bView;			// ���
	bool	bRecanPic;		// �ط�/ͼƬץ��
	bool	bPtz;			// ��̨����
	bool	bParamSet;		// ��������
	bool	bMemory;		// �洢
	bool	bReboot;		// ���������֡��Զ�ά��
	bool	bSystem;		// ϵͳ����-���������õ��뵼��
}TPOWERINFO,PTPOWERINFO;

// �û�Ȩ����Ϣ
typedef struct tagUserInfo
{
	char	szUserName[MAX_USER_NAME + 1];				// �û�����
	char	szOldUserName[MAX_USER_NAME + 1];			// ���û�����(��ȡ�û���ϢʱΪ��)
	char	szUserPassword[MAX_USER_PASSWORD+ 1];		// �û�����
	char	szOldUserPassword[MAX_USER_PASSWORD+ 1];	// ���û�����(��ȡ�û���ϢʱΪ��)
	char	szUserType[17];								// �û�����"administrator, operator, viewer"
	bool	bView;										// ���
	bool	bRecanPic;									// �ط�/ͼƬץ��
	bool	bPtz;										// ��̨����
	bool	bParamSet;									// ��������
	bool	bMemory;									// �洢
	bool	bReboot;									// ���������֡��Զ�ά��
	bool	bSystem;									// ϵͳ����-���������õ��뵼��
}TUSERINFO,PTUSERINFO;

// ɾ���û�
typedef struct tagUserDelInfo
{
    char    szUserName[MAX_USER_NAME  + 1];				// �û�����
	char	szUserPassword[MAX_USER_PASSWORD+ 1];		// �û�����
	char	szUserType[17];								// �û�����"administrator, operator, viewer"
}TUSERDELINFO,PTUSERDELINFO;

// �޸��û�
typedef struct tagUserUpdateInfo
{
    char    szUserName_Old[MAX_USER_NAME  + 1];			// ���û�����
	char	szUserPassword_Old[MAX_USER_PASSWORD+ 1];	// ���û�����
    char    szUserName_New[MAX_USER_NAME  + 1];			// ���û�����
	char	szUserPassword_New[MAX_USER_PASSWORD+ 1];	// ���û�����
	char	szUserType[17];								// �û�����"administrator, operator, viewer"
	bool	bView;										// ���
	bool	bRecanPic;									// �ط�/ͼƬץ��
	bool	bPtz;										// ��̨����
	bool	bParamSet;									// ��������
	bool	bMemory;									// �洢
	bool	bReboot;									// ���������֡��Զ�ά��
	bool	bSystem;									// ϵͳ����-���������õ��뵼��
}TUSERUPDATEINFO,PTUSERUPDATEINFO;


typedef struct tagAdvancedInfo
{
	char  	szKVispTimeStamp[33];	// KEDAVISIP����ʱ�����׼����׼(normal)���ɱ�(old)
	char  	szKVispKeepLive[33];	// KEDAVISIP����Ƶ����Զ�(auto)������(open)���ر�(close)
	char  	szOnvifKeepLive[33];	// Onvif����Ƶ����Զ�(auto)������(open)���ر�(close)
	char  	szGB28181KeepLive[33];	// GB28181����Ƶ����Զ�(auto)������(open)���ر�(close)
	char  	szCGIKeepLive[33];		// CGI����Ƶ����Զ�(auto)������(open)���ر�(close)
	char  	szKVispOSDPicSource[33];	// KEDAVISIP OSDͼƬ��Դ���ͻ���(client)���豸(dev)
	char  	szKVispOSDFontName[33];	// KEDAVISIP OSD���壺����(song)������(black)
	bool	bTCControl;			// TC����
	char  	szMTCFMode[33];		// ����ģʽ������̬ģʽ(hdr)�� 60֡ģʽ(fps60)������ģʽ(stream4)��4k 4Kģʽ(4k)��Div4 4K�ķ���ģʽ(div4),��ͨ��normal����HDSDI-��ͨģʽ��sdsdi��
	char  	szAccMode[33];		// ����ģʽ���Զ�����ʱģʽ(autolow)����ͨģʽ(normal)��ǿ�Ƶ���ʱģʽ(low)
} TADVANCEDINFO,  * PTADVANCEDINFO;

//DDNS״̬����
typedef struct tagDdnsState
{
	char szDdnsState[10];  //״̬
}TDDNSSTATE, *PTDDNSSTATE;

//TCP\IP��Ϣ
typedef struct tagTCPIPv6Info
{
	u8    byIpMode;            //IPģʽ  0-�Զ�auto 1-�ֶ�manual 3-·��route
	bool   bDnsAutoEnable;     //�����Զ�DNS����(TRUE,FALSE)
	u32   dwSubNetMask;        //��������
	u32   dwDefGateway;        //ȱʡ����
	char   szIpAddr[MAX_PATH];       //IP��ַ
	char   szMultCastAddr[MAX_PATH]; //�ಥ��ַ(���64)
	char   szDNSFirst[MAX_PATH];     //��ѡDNS������
	char   szDNSAlter[MAX_PATH];     //��ѡDNS������(���64�ֽ�)
}TIPV6INFO, *PTIPV6INFO;

typedef struct tagTCPIPv4Info
{
	u8   byAddrType;           //��ַ����  0-��̬��1-��̬
	bool   bDnsAutoEnable;     //�����Զ�DNS����(TRUE,FALSE)
	u32   dwSubNetMask;        //��������
	u32   dwDefGateway;        //ȱʡ����
	u32   dwIP;       		   //IP��ַ
	char   szMultCastAddr[MAX_PATH]; //�ಥ��ַ
	char   szDNSFirst[MAX_PATH];     //��ѡDNS������(���64�ֽ�)
	char   szDNSAlter[MAX_PATH];     //��ѡDNS������(���64�ֽ�)
}TIPV4INFO, *PTIPV4INFO;

typedef struct tagIPInfo
{
	char        szMac[18];      //Mac��ַ  
	u32         dwMtu;	        //Mtu(500~4000)
	TIPV4INFO  	tIpv4Info;	    //Ipv4�����ṹ
	TIPV6INFO   tIpv6Info;      //Ipv6�����ṹ
}TIPINFO, *PTIPINFO;


//�˿�
typedef struct tagPortInfo
{
	u16  wHttpPort;            //http�˿�(0~65535)
	u16  wHttpsPort;           //https�˿�(0~65535)
	u16  wRtspPort;			   //rstp�˿ڣ�0~65535��
	u16  wMulticastPort;       //�ಥ�˿� (0~65535)
}TPORTINFO, *PTPORTINFO;

//Wlan��Ϣ
typedef struct tagWlanInfo
{
	char        szMode[5];      //ģʽ(wlan)  
	char        szSSid[33];	    //ssid
	char        szSecurity[5];  //��ȫģʽ(wpk2)  
	char        szEncrypt[5];   //����ģʽ(aes) 
	char        szPassword[17];	//����
	u16         wChannel;       //�ŵ�(1~11)  
}TWLANINFO, *PTWLANINFO;

//upnp��Ϣ
typedef struct tagUPnPInfo
{
	bool 	bEnable;               		// �Ƿ�֧��
	char  	szAlias[MAX_USER_NAME];  	// ����
	char  	szPortMapMode[17];          // �˿�ӳ�䷽ʽ 0-auto 1-manual
}TUPNPINFO, *PTQUPNPINFO;

//upnp�˿�ӳ����Ϣ
typedef struct tagUPnPPortMap
{
	bool 	bCheck;    		    // �Ƿ�ѡ��
	char  	szProtocolName[17]; // Э������ http/rtsp/sdk
	char    szState[17]; 		// ״̬valid/invalid
	char    szMode[5];          // ����ģʽ tcp/udp
	u16     wPort;			    // �ⲿ�˿� 0~65535
	u32     dwIp;		 	    // ip
} TUPNPPORTMAP, *PTUPNPPORTMAP;

//pppoe
typedef struct tagPPPoEInfo
{
	bool 	bEnable;          		              // ����PPPOE
	u32  	dwDynamicIp;            	          // �豸��̬IP(ֻ��) 
	char  	szUserName[MAX_USER_NAME + 1]; 		  // �û���
	char  	szPassWord[MAX_USER_PASSWORD + 1];    // ����
}TPPPOEINFO, * PTPPPOEINFO;

//K-SNMP
typedef struct tagKSnmpInfo
{
	u16  wSvrPort;      	// ���ܷ������˿ڣ�ֻ����(0~65535)
	u32  dwSvrIP;     		// ���ܷ�������ַ
	u32  dwCpuVpt;    	    // cpu�����ʷ�ֵ(0~100)
	u32  dwMemeryVPT; 	    // �ڴ������ʷ�ֵ(0~100)
	u32  dwLostVPT;     	// ��Ƶ�����ʷ�ֵ(0~100)
	char szDevAdress[33]; 	// �豸λ����Ϣ(���32)
}TKSNMPINFO, *PTKSNMPINFO;

//802.1X
typedef struct tag8021xInfo
{
	bool 	bEnable;    		               // ����
	char  szProtocoltype[17];                  // Э������(ֻ��),eap-tls,eap-ttls,eap-peap,eap-leap,eap-fast,eap-md5
	char  szEapolVersion[17];  	               // eapol�汾��1, 2��
	char  szUserName[MAX_USER_NAME+1];    	   // �û���
	char  szPassWord[MAX_USER_PASSWORD+1];       // ����
}T8021XINFO, *PT8021XINFO;

//Qos
typedef struct tagQosInfo
{
	bool 	bEnable;    		// �Ƿ�֧��
	u32  	dwVideoDscpValue; 	// ����Ƶdscp(0~63)
	u32     dwManagerDscpValue; // ����dscp(0~63)
}TQOSINFO, *PTQOSINFO;

//Vsip
typedef struct tagKVispInfo
{
	bool	bEnable;			        // ���� 	
	bool	bSendNatPac;			    // �Ƿ���nat̽���
	bool	bSelfNetWork;		        // �Ƿ�֧���Զ�����
	u8	    byAddrtype;			        // ��������(ip,url)	
	u16	    wIpPort;			        // �˿�
	u16	    wLinkCheckTimeSpeed;	    // ��·���ʱ��(10~1000)	
	u32	    dwIpv4;			            // ipv4��ַ	
	char	szUuid[33];			        // �����豸uuid
	char	szNetPassWord[33];		    // �����豸����
	char	szUrl[MAX_PATH];		    // url(���260)
	char	szIpv6[MAX_PATH];			// ipv6��ַ
}TKVISPINFO, *PTKVISPINFO;

//Onvif
typedef struct tagONVIFInfo
{
	bool	byOnvifEnable;		        // ���� 
	char	szUrl[MAX_PATH];		    // ������url
}TONVIFINFO,PTONVIFINFO;

//AuthMode
typedef struct tagAuthModeInfo
{
	char	szAuthtication [8];		// ��Ȩ��ʽ none,digest
}TAUTHMODEINFO,*PTAUTMODEINFO;

// ����ͨ����Ϣ
typedef struct tagGBEncChnInfo
{
	u32	    dwIndex;		          // �ڵ��
	u8	    byStramChn;		          // ����ͨ��
	char	szId[33];		          // ͨ��id(���32)
	char	szName[33];		          // ����(���32)
}TGBENCCHNINFO,*PTGBENCCHNINFO;
// �澯ͨ����Ϣ
typedef struct tagGBAlarmChnInfo
{
	u32	    dwIndex;		          // �ڵ��
	char	szId[33];		          // ͨ��id(���32)
	char	szName[33];		          // ����(���32)
}TGBALARMCHNINFO,*PTGBALARMCHNINFO;
// GB28181��Ϣ
typedef struct tagGB28181Info
{
	bool	 bEnable;		              // ���� 
	u16	    wLocalPort;		          // ���ض˿�  (0~65535)
	u16  	wPlatPort;		              // ƽ̨�˿�(0~65535)
	u32	    dwReletTime;		          // ����ʱ��	(1~1440)
	u32	    dwHeartBeat;		          // �������	(1~100)
	u32	    dwIpv4;		                  // ƽ̨ipv4��ַ
	char	szPlatId[33];		          // ƽ̨ID  (1~16)
	char	szNetid[33];		          // ����ID  (���32)	
	char	szDevOwner[33];	              // �豸����	(���32)
	char	szCivilCode[33];	          // ��������   (���32)
	char	szPoliceRgn[33];	          // ����       (���32)
	char	szSetupAddress[33];           // ��װ��ַ   (���32)
	char	szExpand[MAX_EXPAND_PATH];	  // ����       (���1024)
	char	szIpv6[MAX_PATH];             // ƽ̨ipv6��ַ
	char	szDevName[MAX_DEV_NAME];      // �豸����  (���32)
	char	szUserName[MAX_USER_NAME];	  // ע���û���
	char	szPassWord[MAX_USER_PASSWORD];// ע������
}TGB28181INFO, *PTGB28181INFO;

//DPSS
typedef struct tagDpssInfo
{
	bool	bEnable;			        // ���� 	
	u16	    wIpPort;			        // �˿�(0~65535)
	u32	    dwIpv4;			            // ipv4��ַ	
	char	szUuid[35];			        // �����豸uuid(6~32)
	char	szIpv6[MAX_PATH];			// ipv6��ַ
}TDPSSINFO, *PTDPSSINFO;

//4g
typedef struct tagMobilenetwork
{
	bool	bEnable;			    // ���� 
	bool	bUseDefaultApn;         // ����Ĭ��apn
	char	szApn[33];
	char	szNum[33];
	char	szUser[MAX_USER_NAME];
	char	szPassword[MAX_USER_PASSWORD];
	char	szCarrieroperator[16];//��Ӫ��   "none,chinamobile,chinaunicom,chinatelecom "
	char	szAuthentication[16]; //��֤Э�� "none,pap,chap,auto"
	char	szNetworksystem[16];  //������ʽ
	char	szNetworkstatus[16];  //����״̬ "link,unlink"
	char	szSigstatus[16];      //�ź�ǿ�� "good,nomal,bad,none"
	char	szImei[16];           //imei��
	char	szIpaddress[16];
	char	szSubnetmask[16];
	char	szDefaultgateway[16];
	char	szDns1[16];
	char	szDns2[16];
	char	szSimCard[16];        //sim��״̬ "yes,no"
}TMOBILENETWORK, *PTMOBILENETWORK;

//��ͼ��
typedef struct tagPubsec
{
	bool	bEnable;			         // ���� 
	char	szNetid[33];                 // ����id
	char	szUser[MAX_USER_NAME];
	char	szPassword[MAX_USER_PASSWORD];
	char	szSetupaddress[MAX_PATH];   //��װ��ַ
	char	szCivilcode[MAX_PATH];      //��������
	char	szIpaddress[16];            //ip
	u16     wPort;                      //�˿�1024~65535
	u16     wHeartbeat;                 //������� 1~90
}TPUBSECPARAM, *PTPUBSECPARAM;

//����״̬��ȡ
typedef struct tagStorageState
{
	char	szState[33];		// ����(null),����ʹ����(disker),�ٶ���(diskrslowish), ����(idel), 
	                            // û�и�ʽ��(noformat),��ʽ����(formatting),ʶ����(identifying)
}TSTORAGESTATE,*PTSTORAGESTATE;

// �洢����
// ������Ϣ
typedef struct tagDiskInfo
{
	u8	bySN;			          // �������
	u32	dwDiskCapacity;	          // ��������(KB)
	u32	dwRisidual;		          // ʣ��ռ�
	char	szState[17];		  // ����(null),����ʹ����(disker),�ٶ���(diskrslowish), ����(idel), û�и�ʽ��(noformat),��ʽ����(formatting),ʶ����(identifying)
	char	szDiskType[17];	      // ���ش���(stlocal),����(net)
	char	szAttribute[17];	  // ֻ��(readonly),���в���(all)
}TDISKINFO,*PTDISKINFO;
// ���̲���
typedef struct tagStorageManager
{
	char	szFullMode[17];	      // ������ģʽ(stop),����(over)
	int     nDiskMun;             // ���̲ۿ���
	TDISKINFO tDiskInfo[5];
}TSTORAGEMANAGER,*PTSTORAGEMANAGER;

// ��ʽ������
typedef struct tagStorageFomat
{
	u8	byDiskSn;		// �������
}TSTORAGEFOMAT,*PTSTORAGEFOMAT;

// ��ʽ������
typedef struct tagFomatState
{
	bool	bFomating;		// �Ƿ����ڸ�ʽ����
	u16	    wProgress;		// ��ʽ������
}TFOMATSTATE,*PTFOMATSTATE;

// ¼��ƻ�
typedef struct tagRecSchedule
{
	bool	bEnable;		         // ����¼��ƻ�
	char	szPrepareTime[9];	     // Ԥ¼ʱ�䣬�룻
	char	szDelayTime[9];	         // �ӳ�ʱ�䣬�룻
	char	szRecType[7];		     // ¼�����ͣ�(����������������������)��
	char	szESPS [17];             // �����ʽ��es��ʽ(es)��ps��ʽ(ps)
}TRECSCHEDULE,*PTRECSCHEDULE;


// ǰ��ץ��
typedef struct tagSnapPicConfig
{
	char	szPicType[9];		 // ͼƬ��ʽ, ��jpeg, jpg,bmp��
	char    szPicResolution[10];  // ͼƬ�ֱ��� ��1280_720��1920_1280��
	char	szPicQuality[17];	 // ͼƬ������low, middle, hight��
}TSNAPPICCONFIG,*PTSNAPPICCONFIG;

typedef struct tagSnapTime
{
	bool	bEnable;		// ����
	char   szSnapType[5];     // ��ʱץ������ "time,num,"
	u16	   wSnapSpeed;		// ץ�ļ��
	u16	   wSnapNum;		// ץ������
}TSNAPTIME,PTSNAPTIME;

typedef struct tagSnapEvent
{
	bool	bEnable;		// ����
	u16	   wSnapSpeed;		// ץ�ļ��
	u16	   wSnapNum;		// ץ������
}TSNAPEVENT,PTSNAPEVENT;

//����������ʽ
typedef struct tagLinkMode
{
	bool	bLinkUploadCenter;	// �ϴ�����
	bool	bLinkShowOsd;	    // ��ʾ��Ļ
	bool    bLinkAudioOut;      // ��������
	bool	bLinkRec;		    // ¼������ 
	bool	bLinkSnap;		    // ץ��
	bool    bLinkSmtp;
	bool    bLinkPtzEnb;
	u8      byLinkPtzPreset;
}TLINKMODE,*PTLINKMODE;
// a->1����
typedef struct tagA1List
{
	bool bEnable; //
	u16  wIndex;  //
}TA1LIST,*PTA1LIST;
//����������ʽ
typedef struct tagOtherLinkMode
{
	TA1LIST tA1List[5];		// ����������� 
	u32     dwA1Num;        // �����������
	bool	bPtz;		    //
	u8	    byPreset;		// Ԥ��λ��ptz������Ч
}TOTHERLINKMODE,*PTOTHERLINKMODE;

//�¼���������
typedef struct tagDetectPoint
{
	u32	x[4];			// x
	u32	y[4];			// y
}TDETECTPOINT,*PTDETECTPOINT;

// �ƶ������Ϣ
typedef struct tagDetectInfo
{
	bool bEnable;			        // �����ƶ����
	u32	dwVgaWidth;			        // ���������ȣ�Ĭ��10000, 
	u32	dwVgaHeight;			    // ��������߶ȣ�Ĭ��10000
	u8	byLevel;				    // ������
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TDETECTINFO,*PTDETECTINFO;

// �ڱα�����Ϣ
typedef struct tagAlarmInfo
{
	bool 	bEnable;		        // �����ڱ�
	u32	dwVgaWidth;		            // ���������ȣ�Ĭ��10000, 
	u32	dwVgaHeight;		        // ��������߶ȣ�Ĭ��10000
	u8	byLevel;		            // ������
	u8  byAreaLevel;                // ��������ȼ�
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TALARMINFO,*PTALARMINFO;

// ��������Ϣ
typedef struct tagWarningLineParam
{
	u16 wIndex;                     // �����߱��
	u32	dwLevel;		            // ������
	u32 dwRatio;					// ��������ռ��
	u32	dwRatioLeftPtX;				// �����������ϵ�x����
	u32	dwRatioLeftPtY;             // �����������ϵ�y����
	u32	dwRatioRightPtX;			// �����������µ�x����
	u32	dwRatioRightPtY;            // �����������µ�y����
	char szDirectionType[5];		// ��ʱץ������ "ab,ba,abba"
	bool bMediaMode;                // �Ƿ�֧�ֳ���ģʽ
	char szMediaMode[5];			// ����ģʽ���� "hor,ver"
	u32	dwLineLeftPtX;				// ���������ϵ�x����
	u32	dwLineLeftPtY;              // ���������ϵ�y����
	u32	dwLineRightPtX;				// ���������µ�x����
	u32	dwLineRightPtY;             // ���������µ�y����
	u32	dwPtAX;						// A��x����
	u32	dwPtAY;						// A��y����
	u32	dwPtBX;						// B��x����
	u32	dwPtBY;						// B��y����
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TWARNINGLINEPARAM,*PTWARNINGLINEPARAM;

typedef struct tagWarningLineInfo
{
	bool 	bEnable;		        // ���þ�����
	u32	dwVgaWidth;		            // ���������ȣ�Ĭ��10000, 
	u32	dwVgaHeight;		        // ��������߶ȣ�Ĭ��10000
	u32 dwLevelMax;					// ���������ֵ
    u32 dwLevelMin;					// ��������Сֵ
	u32 dwRatioMax;					// �����������ռ��
	u32 dwRatioMin;					// ����������Сռ��
	u32 dwSelected;                 // ��ǰѡ�񾯽���(1,2,3,4)
	TWARNINGLINEPARAM tWarningLine[4];// �����߲���
}TWARNINGLINEINFO,*PTWARNINGLINEINFO;

// ���ܴ����������������
typedef struct tagAreaPointList
{
	u32	dwPtX;		        //  ��x����
	u32	dwPtY;		        //  ��y����
}TAREAPOINTLIST,*PTAREAPOINTLIST;
// ��������Ϣ(����,����,�뿪)
typedef struct tagAreaDetectParam
{
	u16 wIndex;                     // ������
	u32	dwLevel;		            // ������
	u32 dwRatio;					// ��������ռ��
	u32 dwThreshold;                // ʱ����ֵ
	u32	dwRatioLeftPtX;				// �����������ϵ�x����
	u32	dwRatioLeftPtY;             // �����������ϵ�y����
	u32	dwRatioRightPtX;			// �����������µ�x����
	u32	dwRatioRightPtY;            // �����������µ�y����
	bool bMediaMode;                // �Ƿ�֧�ֳ���ģʽ
	char szMediaMode[5];			// ����ģʽ���� "hor,ver"
	u32 dwPiontNum;                 // Χ������ĵ���Ŀ(���6����,����3����)
	TAREAPOINTLIST tPtList[6];      // Χ������ĵ���������
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TAREADETECTPARAM,*PTAREADETECTPARAM;

typedef struct tagAreaDetectInfo
{
	bool 	bEnable;		        // ����������
	u32	dwVgaWidth;		            // ���������ȣ�Ĭ��10000, 
	u32	dwVgaHeight;		        // ��������߶ȣ�Ĭ��10000
	u32 dwLevelMax;					// ���������ֵ
	u32 dwLevelMin;					// ��������Сֵ
	u32 dwRatioMax;					// �����������ռ��
	u32 dwRatioMin;					// ����������Сռ��
	u32 dwThresholdMax;				// ʱ����ֵ���ֵ(Ϊ0ʱ����֧���������)
	u32 dwThresholdMin;				// ʱ����ֵ��Сֵ
	u32 dwSelected;                 // ��ǰѡ����������(1,2,3,4)
	TAREADETECTPARAM tAreaDetect[4];// �����������
}TAREADETECTINFO,*PTAREADETECTINFO;
// ��Ʒ���(����,��ȡ)
typedef struct tagObjectDetectParam
{
	u16 wIndex;                     // ������
	u32	dwLevel;		            // ������
	u32 dwRatio;					// ��������ռ��
	u32 dwThreshold;                // ʱ����ֵ
	u32	dwRatioLeftPtX;				// �����������ϵ�x����
	u32	dwRatioLeftPtY;             // �����������ϵ�y����
	u32	dwRatioRightPtX;			// �����������µ�x����
	u32	dwRatioRightPtY;            // �����������µ�y����
	u32 dwPiontNum;                 // Χ������ĵ���Ŀ(���6����,����3����)
	TAREAPOINTLIST tPtList[6];      // Χ������ĵ���������
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TOBJECTDETECTPARAM,*PTOBJECTDETECTPARAM;

typedef struct tagObjectDetectInfo
{
	bool 	bEnable;		        // ������Ʒ���
	u32	dwVgaWidth;		            // ���������ȣ�Ĭ��10000, 
	u32	dwVgaHeight;		        // ��������߶ȣ�Ĭ��10000
	u32 dwLevelMax;					// ���������ֵ
	u32 dwLevelMin;					// ��������Сֵ
	u32 dwRatioMax;					// �����������ռ��
	u32 dwRatioMin;					// ����������Сռ��
	u32 dwThresholdMax;				// ʱ����ֵ���ֵ(Ϊ0ʱ����֧���������)
	u32 dwThresholdMin;				// ʱ����ֵ��Сֵ
	u32 dwSelected;                 // ��ǰѡ��������(1,2,3,4)
	TOBJECTDETECTPARAM tObjectDetect[4];// ��Ʒ����������
}TOBJECTDETECTINFO,*PTOBJECTDETECTINFO;
// ��Ա���(�ۼ�)
typedef struct tagPersontDetectParam
{
	u16 wIndex;                     // ������
	u32 dwRatio;					// ��������ռ��
	u32 dwPiontNum;                 // Χ������ĵ���Ŀ(���6����,����3����)
	TAREAPOINTLIST tPtList[6];      // Χ������ĵ���������
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TPERSONDETECTPARAM,*PTPERSONDETECTPARAM;

typedef struct tagPersonDetectInfo
{
	bool 	bEnable;		        // ������Ա���
	u32	dwVgaWidth;		            // ���������ȣ�Ĭ��10000, 
	u32	dwVgaHeight;		        // ��������߶ȣ�Ĭ��10000
	u32 dwRatioMax;					// �����������ռ��
	u32 dwRatioMin;					// ����������Сռ��
	u32 dwSelected;                 // ��ǰѡ��������(1,2,3,4)
	TPERSONDETECTPARAM tPersonDetect[4];// ��Ա����������
}TPERSONDETECTINFO,*PTPERSONDETECTINFO;
// �齹���
typedef struct tagVirtualFocusInfo
{
	bool 	bEnable;		        // �����齹���
	u32 dwLevelMax;					// ���������ֵ
	u32 dwLevelMin;					// ��������Сֵ
	u32	dwLevel;		            // ������
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TVIRTUALFOCUSINFO,*PTVIRTUALFOCUSINFO;
// �������
typedef struct tagSightChangeInfo
{
	bool 	bEnable;		        // ���ó������
	u32 dwLevelMax;					// ���������ֵ
	u32 dwLevelMin;					// ��������Сֵ
	u32	dwLevel;		            // ������
	char szChkMode[16];             // ���ģʽ(software)
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TSIGHTCHANGEINFO,*PTSIGHTCHANGEINFO;
// �����쳣
typedef struct tagAudioExceptInfo
{
	bool 	bEnable;		        // ���������쳣
	u32 dwThresholdMax;				// ����ǿ����ֵ���ֵ
	u32 dwThresholdMin;				// ����ǿ����ֵ��Сֵ
	u32	dwThreshold;		        // ����ǿ����ֵ
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TAUDIOEXCEPTINFO,*PTAUDIOEXCEPTINFO;
// �������
typedef struct tagFaceDetectInfo
{
	bool 	bEnable;		        // �����������
}TFACEDETECTINFO,*PTFACEDETECTINFO;
// ����������Ϣ
typedef struct tagAlarmInInfo
{
	bool 	bEnable;		        // ���ñ�������
	u32 	dwAlarmNum;		        // ����������
	char	szAlarmName[33];	    // ��������
	char	szAlarmType[17];	    // �������ͣ�����(open)������(close)
	char    szCopyToAlarm[4];       // ���Ƶ�����(all)
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TALARMININFO,*PTALARMININFO;

// ���������Ϣ
typedef struct tagAlarmOutInfo
{
	char   szType[4];               // ������� all,0~1
	char   szDelyTime[17];		    // �ӳ�ʱ�� 5s/10s/30s/1m/2m/5min/10min
}TALARMOUTINFO,*PTALARMOUTINFO;

// �쳣��������
typedef struct  tagExceptType 
{
	char szType[32];               //  �쳣����  ��diskfull,diskerr,disconnect,iperr,illegalaccess��
}TEXCEPTTYPE,*PTEXCEPTTYPE;

// �쳣������Ϣ
typedef struct tagExceptInfo
{
	bool bEnable;		            // �����쳣����
	char szType[32];                // �쳣����  ��diskfull,diskerr,disconnect,iperr,illegalaccess��
	TLINKMODE tLinkMode;	        // ����������ʽ
	TOTHERLINKMODE tOtherLinkMode;	// ����������ʽ
}TEXCEPTINFO,*PTEXCEPTINFO;

// ��̨�ƶ�
typedef struct tagPtzMove
{
	char	szType[17];		// ��(moveup)����(movedown)����(moveleft)����(moveright)����λ(ptz_reset)
							// ����(move_rightup)������(move_rightdown)������(move_leftup)������(move_leftdown)
							// ֹͣ�ƶ�(movestop)
							// ��Ұ����(zoomtele)����Ұ��Զ(zoomwide)��ֹͣ����(zoomstop)
	                        // �۽�(focusnear)����Զ(focusfar)��ֹͣ�۽�(focusstop)���Զ��۽�(focusauto)
	int	    nSpeed;		    // �ٶ�1��100
}TPTZMOVE,*PTPTZMOVE;

// ��̨��������
typedef struct tagPtzZoneZoom
{
	char	szType[17];		 // ����Ŵ�(zoompart)��������С(zoomwhole)
	u32	    dwWidth;		 // �����
	u32     dwHeight;        // �����
	u32     dwX;             // ��ʼ�����--x
	u32     dwY;             // ��ʼ�����--y
}TPTZZONEZOOM,*PTPTZZONEZOOM;

// ��̨�����ƶ�
typedef struct tagPtzGotoPoint
{
	char	szType[17];		 // ����(gotopoint)
	u8      byX;             // �����--x(0,255)
	u8      byY;             // �����--y(0,255)
							 // ˵��:��ʵ�����������ĵ�Ϊ0��,�������귶Χ����(-127~127),�����ڽṹ���д��ݵ���u8����
							 // ��Ҫתһ�£���-1����������254,��-127������������128
}TPTZGOTOPOINT,*PTPTZGOTOPOINT;

// ��ˢ����/�ر�
typedef struct tagPtzWiper
{
	bool bEnable;           // ����/�ر�
}TPTZWIPER,*PTPTZWIPER;

// Ԥ��λ
typedef struct tagPtzPreset
{
	u16	    wSn;		    // Ԥ��λ���,
	bool	bEnable;	    // �Ƿ�����
	char	szName[33];	    // Ԥ��λ����
	char    szEvtType[33];  // Ԥλ���¼����� ����(preset_set),����(preset_load),ɾ��(preset_remove)
}TPTZPRESET,*PTPTZPRESET;
// ��̨��������
typedef struct tagPtzBase
{
	char    szPtzVer[32];             // ��̨�汾(ֻ��)
	u16     wScanRate;               // ɨ���ٶȵȼ�(0~40)
	bool     bEnableDepth;           // �����������
}TPTZBASE,*PTPTZBASE;
// ��̨�ϵ�ά��
typedef struct tagPtzMainTain
{
	bool   bEnableOutage;         //�ϵ�ָ�����
	char   szOutageMemery[7];     //�ָ���ʽoutage, preset
	u16    wOutageTime;          // �ָ�ʱ��(10~360)
	u16    wPreSet;               // Ԥ��λ(1~256)
}TPTZMAINTAIN,*PTPTZMAINTAIN;
// ��̨�ָ���������
typedef struct tagPtzRefactory
{
	char    szEvtType[32];  // �ָ����������¼�(restore_factory_settings)
}TPTZREFACTORY,*PTPTZREFACTORY;
// ��̨��������
typedef struct tagPtzWatchOn
{
	bool   bEnable;                //������������
	char   szWatchmode[16];        //����ģʽoutage, preset
								   //��hscan,vscan,preset,pathcruise,framescan,
								   //randscan,fullviewscan,syncscan��
	u32      dwWaitTime;           // �ȴ�ʱ��(1~36000)
	u16       wPreset;             // Ԥ��λ(1~256)
	u8        byPathCruise;        // ɨ��·��(1~8)
	u8        bySyncScan;          // ����ɨ��·��(1~4)
}TPTZWATCHON,*PTPTZWATCHON;
// ��̨��λ
typedef struct tagPtzLimit
{
	bool  bKeyLimit;             // �Ƿ�����ɨ����λ
	bool  bScanLimit;            // �Ƿ�����������λ
	char  szEvtType[32];         // ��λ�¼����� :����λ(limit_horizonalleft) ����λ(limit_horizonalright) 
								 //	������λ(manuallimitswitch_set) ɨ����λ(scanlimitswitch_set)
}TPTZLIMIT,*PTPTZLIMIT;
// ��̨Ѳ��ɨ��
typedef struct tagPtzPathCruise
{
	int	nIndex;	                // ɨ��·������(1~8)
	u16	wStayTime;	            // ͣ��ʱ�� (5~1800s)
	bool	bEnable;	        // �Ƿ�����
	char	szName[33];	        // Ԥ��λ����
}TPTZPATHCRUISE,*PTPTZPATHCRUISE;
// ��̨Ѳ��ɨ���¼�
typedef struct tagPtzPathCruiseEvt
{
	int	    nIndex;	                // ɨ��·������(1~8)
	char    szEvtType[32];          // Ѳ��ɨ���¼�  ����(path_cruise_start) ֹͣ(path_cruise_end)
}TPTZPATHCRUISEEVT,*PTPTZPATHCRUISEEVT;

// ��ʱ����
typedef struct tagTimeTask
{
	bool  bEnable;            // �Ƿ�������ʱ����
	u32   dwWaitTime;       // ʱ����ʾ(1~36000)
}TTIMETASK,*PTTIMETASK;

typedef struct tagTimeTaskList
{
	u8	      byWeekDay;	     // 1��7�����ա�����
	u8	      byTimeIndex;	     // ʱ�������
	bool      bEnable;           // �Ƿ�����ʱ���
	char      szStartTime[32];   // ��ʼʱ��
	char      szEndTime[32];     // ����ʱ��
	char      szCruiseMode[16];  // ����ģʽ
	u16       wPreset;           // Ԥ��λ(1~256)
	u8        byPathCruise;      // ɨ��·��(1~8)
	u8        bySyncScan;        // ����ɨ��·��(1~4)
	char      szAssParam1[17];   // ��������1,�硱alarm,manual,time,trigger�������沼������Ҫ��д  
}TTIMETASKLIST,*PTTIMETASKLIST;

//��λ
typedef struct tagPtzCoorDinate
{
	u32  dwHorizontal;          // ˮƽ����  min="0" max="36000"
	int  nVertical;            // ��ֱ���� min="-1500" max="9000"
	u8   bymutiple;             // 10���ݴη��� 1,2,3,4,5,6......
	char szEvtType[32];         // ��λ�¼� ��λУ��(turnto_machinezero),ˮƽ��λ(panposion_set),��ֱ��λ(tiltposion_set)
								// ˮƽ��ֱͬʱ��λ(ptposion_set)
}TPTZCOORDINATE,*PTPTZCOORDINATE;
//��̨����
typedef struct tagPtzRatio
{
	u32  dwRatio;              // ����
	int  nRatioMin;            // ������Сֵ
	int  nRatioMax;            // �������ֵ
	char szEvtType[32];        // �����¼� (set_zoomratio)
}TPTZRATIO,*PTPTZRATIO;

//��λ+��̨���� (ͬʱ����)
typedef struct tagPtzCoorDinateEx
{
	u32  dwHorizontal;          // ˮƽ����  min="0" max="36000"
	int  nVertical;             // ��ֱ���� min="-1500" max="9000"
	u32  dwRatio;               // ����	
	u8   bymutiple;             // 10���ݴη��� 1,2,3,4,5,6......
	char szEvtType[32];         // ptzͬʱ��λ(ptz_set)
}TPTZCOORDINATEEX,*PTPTZCOORDINATEEX;

//��̨�ӳ���
typedef struct tagPtzHVangle
{
	u32  dwX;                    // ˮƽ��λ��Ϣ 
	u32  dwY;                    // ��ֱ��λ��Ϣ
	int  nHangleMin;            // �ӳ���ˮƽ��Сֵ
	int  nHangleMax;            // �ӳ���ˮƽ���ֵ
	int  nVangleMin;            // �ӳ��Ǵ�ֱ��Сֵ
	int  nVangleMax;            // �ӳ��Ǵ�ֱ���ֵ
}TPTZHVANGLE,*PTPTZHVANGLE;

//websocket�������ϴ�(����ptz��Ϣ�Զ���ȡ)
typedef struct tagPtzWsInfo
{
	char szIP[32];					//websocket������ip
	u16  wPort;						//websocket�������˿� 	
	char szUrl[MAX_PATH];			//websocket������url
}TPTZWSINFO,*PTPTZWSINFO;

// ����ɨ��
typedef struct tagPtzPatternsInfo
{
	char  szPatternState[10];         //״̬ ��set,scan,idel��
	u16	   wIndex;	                   //ɨ���Ԥ��������
	bool    patternslist[4];          //ʹ�õ�·���б�
}TPTZPATTERNSINFO,PTPTZPATTERNSINFO;
// ����ɨ���¼�
typedef struct tagPtzPatternEvt
{
	u8   byPatternPath;          // ����ɨ��·�� 1,2,3,4
	char szEvtType[32];          // ����ɨ���¼� ��ʼ��¼(syncscan_rec),ֹͣ��¼(syncscan_stoprec),�����¼(syncscan_delete)
								 //              Ԥ��(syncscan_preview),ֹͣ(syncscan_stoppreview)
}TPTZPATTERNEVT,*PTPTZPATTERNEVT;

// ����ʱ����Ϣ
typedef struct tagEventTime
{
	u8	   byWeekDay;		// 1��7�����ա�����
	u8	   byTimeIndex;		// ʱ�������
	bool   bEnable;         // �Ƿ�����ʱ���
	u16	   wStartTime_Year;	// ��ʼʱ��
	u16	   wStartTime_Month;// 
	u16	   wStartTime_Day;	// 
	char   szStartTime[32];
	u16	   wEndTime_Year;	// ����ʱ��
	u16	   wEndTime_Month;	// 
	u16	   wEndTime_Day;	// 
	char   szEndTime[32];
	char   szAssParam1[17];	// ��������1,�硱 alarm,manual,time,trigger�������沼������Ҫ��д
}TEVENTTIME,*PTEVENTTIME;

//
typedef struct tagTimeEx
{
	char   szTime[32];// ��-��-�� ʱ-��-��(utcʱ��)
}TTIMEEX,*PTTIMEEX;
// ����ͼ�ṹ��
typedef struct tagRecMonthInfo
{
	bool   byHadRec[32];		// �Ƿ���¼��
} TRECMONTHINFO, *PTRECMONTHINFO;

// ��ȡ¼�����ݲ���
typedef struct tagRecParam
{
	u32  dwRecType[12];	// ¼�����ͣ� emRecType,�������12��ָ�����ͣ�Ŀǰ֧��:��all,alarm,time,manual��
	TTIMEEX	tStartTime;	// ��ʼʱ��
	TTIMEEX 	tEndTime;	// ֹͣʱ��
} TRECPARAM, * PTRECPARAM;
// ¼�����ݽṹ��
typedef struct tagRecData
{
	u32  dwIndex;		// ����
	u32  dwRecId;		// ¼��id
	u32  dwRecType;		// ¼������
	TTIMEEX	tStartTime;	// ��ʼʱ��
	TTIMEEX 	tEndTime;	// ֹͣʱ��
} TRECDATA, * PTRECDATA;

// ��ʼ�������
typedef struct tagRecStartPlayParam
{
	u32	dwStartID;
	u32	dwRcvIp;
	u16	wRtpVideoPort;
	u16	wRtcpVideoPort;
	u16	wRtpAudioPort;
	u16	wRtcpAudioPort;
	u16	wRtpAudioPort2;
	u16	wRtcpAudioPort2;
	TTIMEEX	tStartTime;	// ��ʼʱ��
	TTIMEEX 	tEndTime;	// ֹͣʱ��
	TTIMEEX 	tPlayTime;	// ����ʱ��
} TRECSTARTPLAYPARAM, * PTRECSTARTPLAYPARAM;

// ������Ϣ
typedef struct tagRecStartPlayInfo
{
	u32	dwPlayID;
	u16	wRtcpVideoPort;
	u16	wRtcpAudioPort;
	u16	wRtcpAudioPort2;
} TRECSTARTPLAYINFO, * PTRECSTARTPLAYINFO;

// ���������Ϣ
typedef struct tagRecPlayStateInfo
{
	char       szState [10]; // ����״̬
	TTIMEEX    tPlayTime;	// ����ʱ��
	u32        dwTimeStamp; //ʱ���
} TRECPLAYSTATEINFO, * PTRECPLAYSTATEINFO;

//
typedef struct KedaTeleZoomAlgoParam
{	
	u32 dwTeleZoomInRatio;  //���ű���
	u32 dwTeleCurvLevel;     //�ϲ�����
	u32 dwTeleMultVal;       //�ϲ�б�ʳ���
	u32 dwWideRatio;        //�²�����
	u32 dwMidWidth;        //����У��������
	u32 dwMidHeight;        //����У������߶�
	u32 dwAdjHeight;       //Զ�˷Ŵ������ʼ��
	float f32WidthScale;       //�����Ӧ���ű���
}KedaTeleZoomAlgoParam_T;

typedef struct tagTeleZoomParam
{	
	u8    bOpen;
	u8    bLevel;
	KedaTeleZoomAlgoParam_T tTeleZoom;
}TTELEZOOMPARAM, *PTTELEZOOMPARAM;

// ���ν���
typedef struct tagMalfAlgoParam
{	
	u8 bOpen;			    // 0-�رգ���0-����
	u32 dwMalfParam1;		 // ���ν���������
	u32 dwMalfParam2;		// ���ν���������

	tagMalfAlgoParam ()
	{
		memset (this, 0, sizeof(tagMalfAlgoParam));
	}
}TMALFPARAM, *PTMALFPARAM;

// ����ģʽ
typedef struct tagMtcfInfoParam
{
	u8 bMtcf;              // 0-stream2 ��ͨ������ģʽ 1-mtcf�������Զ�˷Ŵ�
}TMTCFPARAM, *PTMTCFPARAM;

// ���״̬
typedef struct tagBatStatusInfo
{
	u16 wBatteryStatus;        // ��ص���(0 ~ 100)
}TBATSTATUSINFO, *PTBATSTATUSINFO;

// wifi̽��
typedef struct tagWifiProbeInfo
{
	bool	bEnable;		              // ����wifi̽��
	bool	bReportPlatform;		      // �Ƿ��ϴ�ƽ̨
	bool	bDisable;					  // ���ֹ����Ƿ��������(��ֵ���ɸ���)
										  // Ϊtrue��bMacEnable,bSignalEnable,bTimeEnable,dwLongitude,dwLatitude����������

	bool	bMacEnable;					  // �Ƿ���ʾ�ƶ��ն�Mac��ַ
	bool	bSignalEnable;		          // �Ƿ���ʾ�ź�ǿ��
	bool	bTimeEnable;		          // �Ƿ���ʾ̽��ʱ��
	int	    nLongitude;		              // ����
	int	    nLongitudeMax;		          // �������ֵ
	int	    nLongitudeMin;		          // ������Сֵ
	int	    nLatitude;					  // γ��
	int	    nLatitudeMax;				  // γ�����ֵ
	int	    nLatitudeMin;				  // γ����Сֵ
	char	szProvinceCode[MAX_PATH];     // ����ʡ����(ֻ�������ֺ���ĸ)
	char	szCityCode[MAX_PATH];		  // �����д���(ֻ�������ֺ���ĸ)
	char	szAreaCode[MAX_PATH];		  // ���ڵ�������(ֻ�������ֺ���ĸ)
	char	szAddress[MAX_PATH];		  // ���ڵص�����(����Ϊ�����ַ�)
	int	    dwDeviceNum;			      // ���������ն���Ŀ(��������tagWifiProbeListInfo���ڴ�)
}TWIFIPROBEINFO, *PTWIFIPROBEINFO;

// wifi̽���豸�б�
typedef struct tagWifiProbeListInfo
{
	char	szMac[MAX_PATH];          // �ƶ��ն�Mac��ַ
	char	szSignal[MAX_PATH];		  // �ź�ǿ��("bad,middle,good")
	char	szTime[MAX_PATH];		  // ̽��ʱ��
}TWIFIPROBELISTINFO, *PTWIFIPROBELISTINFO;

// ��ɫ��
typedef struct tagColorSize
{
	char	szColor[64];			// whitehot,blackhot,light,rainbow,north,hotiron,delaminate,medical,red
}TCOLORSIZE, *PTCOLORSIZE;

// �ȵ�׷��״̬
typedef struct tagHotState
{
	char	szState[64];			// open,close
}THOTSTATE, *PTHOTSTATE;

// ����λ����Ϣ
typedef struct tagPositionInfo
{
	bool	bEnable;		              // ����
	char	szPosmode[32];				  // ����λ��ģʽ
	int	    nRtpInterval;			      // �����ϴ����
	char	szEWindicador[32];			  // ����������
	char	szNSindicador[32];			  // ��γ����γ	
	char	szSpdUnit[8];		          // �ٶȵ�λ
	char	szLocation[32];		          // ��ǰλ��
	int     nSpeedMile;
	int     nSpeedKm;
}TPOSITIONINFO, *PTPOSITIONINFO;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif
