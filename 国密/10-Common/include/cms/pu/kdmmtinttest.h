/*========================================
ģ����	�� KdmMtIntTest.h
�ļ���	�� KdmMtIntTest.h
����ļ�	�� MtCtrl.h,kdvdef.h
ʵ�ֹ��� �� ����������ɲ��Խӿ�
����	�� ���ǿ
�汾	�� V3.0
����    	�� 2005.04.14
�޸ļ�¼:
��  ��     �汾       �޸���      �޸�����
===========================================*/
#ifndef _MT_INTEGRATION_TEST_H
#define _MT_INTEGRATION_TEST_H

#include "osp.h"
#include "kdvdef.h"
#include "kdvtype.h"
#include "mtctrl.h"

#define VER_MT_TESTLIB "testlib30.10.01.03.00.050414"
API void testlibver();

// ָ���ж�
#define MT_ASSERT( p ) if ( p == NULL ) { ::OspPrintf( TRUE, FALSE, "[SYSTEM]Invalid Pointer(%s:(%d))!\n", __FILE__, __LINE__ ); return; }
#define MT_ASSERT_RET( p, ret ) if ( p == NULL ) { ::OspPrintf( TRUE, FALSE, "[SYSTEM]Invalid Pointer(%s:(%d))!\n", __FILE__, __LINE__ ); return ret; }

#ifndef _VXWORKS_
#pragma pack(push)
#endif
#pragma pack(1)



//netsend dest addr max num

#define IT_MAX_NETSND_DEST_NUM                   5 

//MSG
#define MSG_IT_MT_CMD                            55//�ն˼��ɲ���ģ�����ն˷��ͻ�ȡ״̬�������������Ϣ

//detail state
#define SYS_DEVICE_TYPE							1 //��ȡ�豸����
#define SYS_TIME_GET		              		2 //��ȡϵͳʱ��
#define STATE_CODEC_VENC          				3 //��ȡ��Ƶ����ͨ��״̬
#define STATE_CODEC_VDEC						4 //��ȡ��Ƶ����ͨ��״̬
#define STATE_CODEC_AENC						5 //��ȡ��Ƶ����ͨ��״̬
#define STATE_CODEC_ADEC						6 //��ȡ��Ƶ����ͨ��״̬
#define STATE_PERI_SERI	             			7 //��ȡ����״̬
#define STATE_PERI_PARIN 						8 //��ȡ��������״̬
#define STATE_CODEC_VPOLL						9 //��ȡ��Ƶ������ѯ״̬
#define STATE_CODEC_VMP							10 //��ȡ��Ƶ����໭��״̬
#define STATE_CODEC_VDECRF						11 //��ȡ��Ƶ�����ش�״̬
#define STATE_CODEC_VENCMD						12 //��ȡ��Ƶ�����ƶ����״̬
#define STATE_CODEC_VDECMD						13 //��ȡ��Ƶ�����ƶ����״̬
#define STATE_OSD_VENC 							14 //��ȡ��Ƶ������Ļ״̬
#define STATE_CODEC_VINPUT						15 //��ȡ��ƵԴ����˿�״̬

//detail param
#define PARAM_SYS_CAPABILITY					100 //��ȡ�豸������
#define PARAM_SYS       						101 //��ȡϵͳ�����ṹ
#define PARAM_SYS_PUB							102 //��ȡϵͳ���������ṹ
#define PARAM_CODEC_VENCNET						103 //������������ṹ
#define PARAM_CODEC_VDECNET						104 //������������ṹ
#define PARAM_CODEC_AENCNET						105 //������������ṹ
#define PARAM_CODEC_ADECNET						106 //������������ṹ
#define PARAM_CODEC_VENC						107 //��Ƶ��������ṹ
#define PARAM_CODEC_VENCPUB						108 //��Ƶ���빫�������ṹ
#define PARAM_OSD_VENC							109 //��Ƶ������Ļ�����ṹ
#define PARAM_CODEC_VMP							110 //��Ƶ����໭������ṹ
#define PARAM_CODEC_VENPORT         			111 //��Ƶ������ƵԴ�˿ڲ����ṹ
#define PARAM_CODEC_VPOLL						112 //��Ƶ������ѯ�����ṹ
#define PARAM_CODEC_VDEC						113 //��Ƶ��������ṹ
#define PARAM_CODEC_VDECPUB						114 //��Ƶ���빫�������ṹ
#define PARAM_CODEC_VDECRF						115 //��Ƶ�����ش������ṹ
#define PARAM_CODEC_AENC						116 //��Ƶ��������ṹ
#define PARAM_CODEC_ADEC						117 //��Ƶ��������ṹ
#define PARAM_PERI_SERI							118 //���ڲ����ṹ
#define PARAM_PERI_SERIPUB						119 //���ڹ��������ṹ
#define PARAM_PERI_PARIN						120 //������������ṹ
#define PARAM_CODEC_VINPORT						121 //��ƵԴ����˿ڲ����ṹ
#define PARAM_CODEC_VENCMD						122 //��Ƶ�����ƶ��������ṹ
#define PARAM_CODEC_VDECMD						123 //��Ƶ�����ƶ��������ṹ

#define PARAM_PPPOE                             124 //��ȡPPPOE�����ṹ

// ״̬
#define INTTEST_TRUE							1
#define INTTEST_FALSE							0

// �忨����(0:KDM2400;1:KDM2500;2:KDM2418;3:KDM2518;4:KDM2418A;5:KDV8010A;6:KDV8010C;7:DEC5)
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
//����������������鲥��ַ���Ǳ��ؽ��յ�ַ��
typedef struct tagTKDMNetAddr
{
	u32 	dwRtpIP;  //������
	u16 	wRtpPort; //
	u32		dwRtcpIP; //������
	u16 	wRtcpPort;//
}PACKED IT_TNETADDR,*IT_PTNETADDR;

//ʱ���ʽ
typedef TMTI_SYS_TIME    IT_SYSTIME,*IT_PSYSTIME;
//�豸������
typedef TMTI_CAPABILITY  IT_CAPABILITY,*IT_PCAPABILITY;

/////////////////CodecManage group//////////////////////
//���б����ͨ��״̬�ṹ(0:�ر�,1:��)
typedef struct tagChannelState
{
	u8	abyVideoSend[MAX_VIDEO_ENC_NUM];//��Ƶ����ͨ��״̬
	u8	abyVideoRecv[MAX_VIDEO_DEC_NUM];//��Ƶ����ͨ��״̬
	u8	abyAudioSend[MAX_AUDIO_ENC_NUM];//��Ƶ����ͨ��״̬
	u8	abyAudioRecv[MAX_AUDIO_DEC_NUM];//��Ƶ����ͨ��״̬
}PACKED IT_CHANSTATE,*IT_PCHANSTATE;

//��Ƶ����ͨ��״̬
typedef struct tagVEncState
{
	u8				byVideoEnChannel;		//��Ƶ����ͨ����
	u8				byVideoPort;			//��Ӧ��Ƶ����˿ں�
	u8				byVideoType;			//��Ƶ��������(MTI_VIDEO_TYPE_MPEG4��)
	u8				byVideoFormat;			//��Ƶ�����ʽ(MTI_VIDEO_RESOLUTION_CIF��)
	u8				byVideoFrameRate;		//��Ƶ����֡��(1-25)
	u16				wVideoBitRate;			//��Ƶ��������
	u8				byyVideoImageQulity;	//��Ƶ����ѹ������(1-5)
	u8				byVideoEncoding;		//�Ƿ���Ƶ����(INTTEST_TRUE/INTTEST_FALSE)
	u8				byVideoSending;			//�Ƿ�����Ƶ(INTTEST_TRUE/INTTEST_FALSE)
	IT_TNETADDR		tLocalNetParam;			//�����������
	IT_TNETADDR		atDestNetParam[IT_MAX_NETSND_DEST_NUM];//����Ŀ�ĵ��������,���Ϊ5��
}PACKED IT_VENCSTATE,*IT_PVENCSTATE;

//��Ƶ����ͨ��״̬
typedef struct tagVDecState
{
	u8				byVideoDecChannel;		//��Ƶ����ͨ����
	u8				byVideoPort;			//��Ӧ��Ƶ����˿ں�
	u8				byVideoDecoding;		//�Ƿ���Ƶ����(INTTEST_TRUE/INTTEST_FALSE)
	u8				byVideoRecving;			//�Ƿ������Ƶ(INTTEST_TRUE/INTTEST_FALSE)	
	IT_TNETADDR		tLocalNetParam;			//���ؽ����������
}PACKED IT_VDECSTATE,*IT_PVDECSTATE;

//��Ƶ����ͨ��״̬
typedef struct tagAEncState
{
	u8			byAudioEncChannel;  //��Ƶ����ͨ����
	u8			byAudioFormat;	    //��Ƶ��ʽ(MTI_AUDIO_TYPE_PCMA��)
	u8			byEncVol;			//��������
	u8			byMute;				//�Ƿ�����(INTTEST_TRUE/INTTEST_FALSE)
	u8			byAudioEncoding;    //�Ƿ���Ƶ����(INTTEST_TRUE/INTTEST_FALSE)
	u8			byAudioSending;     //�Ƿ�����Ƶ(INTTEST_TRUE/INTTEST_FALSE)
	u8			byAecEnable;	    //�Ƿ�����������(INTTEST_TRUE/INTTEST_FALSE)
	IT_TNETADDR tLocalNetParam;		//�����������
	IT_TNETADDR atDestNetParam[IT_MAX_NETSND_DEST_NUM];//����Ŀ�ĵ��������,���Ϊ5��
}PACKED IT_AENCSTATE,*IT_PAENCSTATE;

//��Ƶ����ͨ��״̬
typedef struct tagADecState
{
	u8      byAudioDecChannel;		//��Ƶ����ͨ����
	u8      byDecVol;				//��������
	u8		byQuiet;	            //�Ƿ���(INTTEST_TRUE/INTTEST_FALSE)
	u8		byAudioDecoding;		//�Ƿ���Ƶ����(INTTEST_TRUE/INTTEST_FALSE)
	u8		byAudioRecving;			//�Ƿ������Ƶ(INTTEST_TRUE/INTTEST_FALSE)
	IT_TNETADDR  tLocalNetParam;	//���ؽ����������
}PACKED IT_ADECSTATE,*IT_PADECSTATE;


//��Ƶ������ѯ״̬
typedef TMTI_VIDEOENC_POLL_PARAM		IT_POLLSTATE,*IT_PPOLLSTATE;
//��Ƶ����໭��״̬
typedef TMTI_VIDEOENC_MP_PARAM          IT_VENCMPSTATE,*IT_PVENCMPSTATE;
//��Ƶ�����ش�״̬
typedef TMTI_VIDEODEC_RETRANSFER_PARAM  IT_VDECRETRANSFERSTATE,*IT_PVDECRETRANSFERSTATE;
//�ƶ����״̬
typedef TMTI_MOTION_DETECT_PARAM        IT_MOTIONDETECTSTATE,*IT_PMOTIONDETECTSTATE;

/////////////////OsdManage group///////////////////////
//��Ƶ������Ļ״̬
typedef TMTI_VIDEOENC_OSD_PARAM         IT_VENCOSDSTATE,*IT_PVENCOSDSTATE;
//��ƵԴ����˿�״̬
typedef TMTI_VIDEO_INPUT_PARAM          IT_VINPUTSTATE,*IT_PVINPUTSTATE;


/////////////////PeriCtrl group///////////////////////
//����״̬
typedef struct tagSerialState
{
	u8	bySerialSend; //���ڷ���״̬(INTTEST_TRUE/INTTEST_FALSE)
	u8	bySerialRecv; //���ڽ���״̬(INTTEST_TRUE/INTTEST_FALSE)
}PACKED IT_SERIALSTATE,*IT_PSERIALSTATE;

//��������״̬
typedef struct tagParallelInState
{	
	u8	bPInputState[MAX_INPUT_PIN_NUM];  //�澯����״̬(INTTEST_TRUE/INTTEST_FALSE)
}PACKED IT_PARALLELINSTATE,*IT_PPARALLELINSTATE;

//�������״̬
typedef struct tagParallelOutState
{	
	u8	bPOutputState[MAX_OUTPUT_PIN_NUM];//�澯���״̬(INTTEST_TRUE/INTTEST_FALSE)
}PACKED IT_PARALLELOUTSTATE,*IT_PPARALLELOUTSTATE;

/////////////////RWConfig group///////////////////////
//ϵͳ�����ṹ
typedef TMTI_SYS_PARAM                  IT_SYSPARAM,*IT_PSYSPARAM;
//ϵͳ���������ṹ
typedef TMTI_SYS_PUB_PARAM              IT_SYSPUBPARAM,*IT_PSYSPUBPARAM;
//������������ṹ
typedef TMTI_COMMONNET_PARAM            IT_COMMONNETPARAM,*IT_PCOMMONNETPARAM;
//��Ƶ��������ṹ
typedef TMTI_VIDEOENC_PARAM             IT_VENCPARAM,*IT_PVENCPARAM;
//��Ƶ���빫�������ṹ
typedef TMTI_VIDEOENC_PUB_PARAM         IT_VENCPUBPARAM,*IT_PVENCPUBPARAM;
//��Ƶ������Ļ�����ṹ
typedef TMTI_VIDEOENC_OSD_PARAM         IT_VENCOSDPARAM,*IT_PVENCOSDPARAM;
//��Ƶ����໭������ṹ
typedef TMTI_VIDEOENC_MP_PARAM          IT_VENCMPPARAM,*IT_PVENCMPPARAM;
//��Ƶ������ƵԴ�˿ڲ����ṹ
typedef TMTI_VIDEOENC_INPUT_PORT_PARAM  IT_VENCINPORTPARAM,*IT_PVENCINPORTPARAM;
//��Ƶ������ѯ�����ṹ
typedef TMTI_VIDEOENC_POLL_PARAM		IT_POLLPARAM,*IT_PPOLLPARAM;
//��Ƶ��������ṹ
typedef TMTI_VIDEODEC_PARAM             IT_VDECPARAM,*IT_PVDECPARAM;
//��Ƶ���빫�������ṹ
typedef TMTI_VIDEODEC_PUB_PARAM         IT_VDECPUBPARAM,*IT_PVDECPUBPARAM;
//��Ƶ�����ش������ṹ
typedef TMTI_VIDEODEC_RETRANSFER_PARAM  IT_VDECRETRANSFERPARAM,*IT_PVDECRETRANSFERPARAM;
//��Ƶ��������ṹ
typedef TMTI_AUDIOENC_PARAM             IT_AENCPARAM,*IT_PAENCPARAM;
//��Ƶ��������ṹ
typedef TMTI_AUDIODEC_PARAM             IT_ADECPARAM,*IT_PADECPARAM;
//���ڲ����ṹ
typedef TMTI_SERIALPORT_PARAM           IT_SERIALPORTPARAM,*IT_PSERIALPORTPARAM;
//���ڹ��������ṹ
typedef TMTI_SERIALPORT_PUB_PARAM       IT_SERIALPORTPUBPARAM,*IT_PSERIALPORTPUBPARAM;
//������������ṹ
typedef TMTI_INPUTPIN_PARAM             IT_INPUTPINPARAM,*IT_PINPUTPINPARAM;
//��ƵԴ����˿ڲ����ṹ
typedef TMTI_VIDEO_INPUT_PARAM          IT_VINPUTPARAM,*IT_PVINPUTPARAM;
//�ƶ��������ṹ
typedef TMTI_MOTION_DETECT_PARAM        IT_MOTIONDETECTPARAM,*IT_PMOTIONDETECTPARAM;
//PPPOE����
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
	
	/////////////////////////////��·�����ͳ���////////////////////////////////
	BOOL LinkInstance( u32 dwIp, u16 wPort = PORT_MT );
	void DisLinkInstance( void );
	BOOL IsLinked();
	void SetTesterNode( u32 dwNodeId ) { m_dwTesterNode = dwNodeId; }
	
	/////////////////////////////��ϸ��ѯ�ӿ�//////////////////////////////////
    //��ȡ�ն�Ӳ������(0:KDM2400;1:KDM2500;2:KDM2418;3:KDM2518;4:KDM2418A;5:KDV8010A;6:KDV8010C;7:DEC5)
    BOOL GetTerDeviceType( u8* pbyMtBoardType );
        
    //state
    //��ȡϵͳʱ��
	BOOL GetSysTime( IT_PSYSTIME ptSysTime = NULL );
    //��ȡ��Ƶ����ͨ��״̬
    BOOL GetVencState( IT_PVENCSTATE ptVencState = NULL, u8 byId = 0 );
    //��ȡ��Ƶ����ͨ��״̬
    BOOL GetVdecState( IT_PVDECSTATE ptVdecState = NULL, u8 byId = 0 );
    //��ȡ��Ƶ����ͨ��״̬
    BOOL GetAencState( IT_PAENCSTATE ptAencState = NULL, u8 byId = 0 );
    //��ȡ��Ƶ����ͨ��״̬
    BOOL GetAdecState( IT_PADECSTATE ptAdecState = NULL, u8 byId = 0 );
    //��ȡ����״̬
    BOOL GetSeriState( IT_PSERIALSTATE ptSeriState = NULL, u8 byId = 0 );
    //��ȡ��������״̬
    BOOL GetParallelInState( IT_PPARALLELINSTATE ptParallelInState = NULL, u8 byId = 0 );   
    //��Ƶ������ѯ״̬
    BOOL GetPollState( IT_PPOLLSTATE ptPollState = NULL, u8 byId = 0 );
    //��Ƶ����໭��״̬        
    BOOL GetVencMpState( IT_PVENCMPSTATE ptVencMpState = NULL, u8 byId = 0 ); 
    //��Ƶ�����ش�״̬
    BOOL GetVdecRetransferState( IT_PVDECRETRANSFERSTATE ptVdecRetransferState = NULL, u8 byId = 0 ); 
    //��Ƶ�����ƶ����״̬
    BOOL GetVencMotionDetectState( IT_PMOTIONDETECTSTATE ptVencMotionDetectState = NULL, u8 byId = 0 );
    //��Ƶ�����ƶ����״̬
    BOOL GetVdecMotionDetectState( IT_PMOTIONDETECTSTATE ptVdecMotionDetectState = NULL, u8 byId = 0 );
    //��Ƶ������Ļ״̬
    BOOL GetVencOsdState( IT_PVENCOSDSTATE ptVencOsdState = NULL, u8 byId = 0 );
	//��ƵԴ����˿�״̬
	BOOL GetVencVidSrcState( IT_PVINPUTSTATE ptVSrcState = NULL, u8 byId = 0 );

    //param
	//��ȡ�豸������
	BOOL GetCapability( IT_PCAPABILITY ptCapability = NULL ) ;
	//ϵͳ�����ṹ
    BOOL GetSysParam( IT_PSYSPARAM ptSysParam = NULL ); 
    //ϵͳ���������ṹ
    BOOL GetSysPubParam( IT_PSYSPUBPARAM ptSysPubParam = NULL ); 
    //��Ƶ�����������
    BOOL GetVideoEncNetParam( IT_PCOMMONNETPARAM ptCommonNetParam = NULL, u8 byId = 0 ); 
   //��Ƶ�����������
    BOOL GetVideoDecNetParam( IT_PCOMMONNETPARAM ptCommonNetParam = NULL, u8 byId = 0 ); 
	//��Ƶ�����������
    BOOL GetAudioEncNetParam( IT_PCOMMONNETPARAM ptCommonNetParam = NULL, u8 byId = 0 ); 
    //��Ƶ�����������
    BOOL GetAudioDecNetParam( IT_PCOMMONNETPARAM ptCommonNetParam = NULL, u8 byId = 0 ); 
    //��Ƶ��������ṹ
    BOOL GetVencParam( IT_PVENCPARAM ptVencParam = NULL, u8 byId = 0 ); 
    //��Ƶ���빫�������ṹ
    BOOL GetVencPubParam( IT_PVENCPUBPARAM ptVencPubParam = NULL, u8 byId = 0  ); 
    //��Ƶ������Ļ�����ṹ
    BOOL GetVencOsdParam( IT_PVENCOSDPARAM ptVencOsdParam = NULL, u8 byId = 0 ); 
    //��Ƶ����໭������ṹ        
    BOOL GetVencMpParam( IT_PVENCMPPARAM ptVencMpParam = NULL, u8 byId = 0 ); 
    //��Ƶ������ƵԴ�˿ڲ����ṹ
    BOOL GetVencInportParam( IT_PVENCINPORTPARAM ptVencInportParam = NULL, u8 byId = 0 ); 
    //��Ƶ������ѯ�����ṹ
    BOOL GetPollParam( IT_PPOLLPARAM ptPollParam = NULL, u8 byId = 0 ); 
    //��Ƶ��������ṹ
    BOOL GetVdecParam( IT_PVDECPARAM ptVdecParam = NULL, u8 byId = 0 ); 
    //��Ƶ���빫�������ṹ        
    BOOL GetVdecPubParam( IT_PVDECPUBPARAM ptVdecPubParam = NULL, u8 byId = 0 ); 
    //��Ƶ�����ش������ṹ
    BOOL GetVdecRetransferParam( IT_PVDECRETRANSFERPARAM ptVdecRetransferParam = NULL, u8 byId = 0 ); 
    //��Ƶ��������ṹ
    BOOL GetAencParam( IT_PAENCPARAM ptAencParam = NULL, u8 byId = 0 );         
    //��Ƶ��������ṹ
    BOOL GetAdecParam( IT_PADECPARAM ptAdecParam = NULL, u8 byId = 0 );         
    //���ڲ����ṹ
    BOOL GetSerialportParam( IT_PSERIALPORTPARAM ptSerialportParam = NULL, u8 byId = 0 ); 
    //���ڹ��������ṹ
    BOOL GetSerialportPubParam( IT_PSERIALPORTPUBPARAM ptSerialportPubParam = NULL, u8 byId = 0 ); 
    //������������ṹ
    BOOL GetInputPinParam( IT_PINPUTPINPARAM ptInputPinParam = NULL, u8 byId = 0 ); 
    //��ƵԴ����˿ڲ����ṹ
    BOOL GetVinputParam( IT_PVINPUTPARAM ptVinputParam = NULL, u8 byId = 0 ); 
    //��Ƶ�����ƶ��������ṹ
    BOOL GetVencMotionDetectParam( IT_PMOTIONDETECTPARAM ptVencMotionDetectParam = NULL, u8 byId = 0 );
    //��Ƶ�����ƶ��������ṹ
    BOOL GetVdecMotionDetectParam( IT_PMOTIONDETECTPARAM ptVdecMotionDetectParam = NULL, u8 byId = 0 );
    //PPPOE�����ṹ
	BOOL GetPPPOEParam(IT_PPPPOEPARAM ptPPPOEParam=NULL,u8 byId = 0);
 	
protected:
	BOOL	IntTestQuery( u16 wEvent, void* buf, u16  wBufLen, u16* pwRetLen, 
				const void* pParam = NULL, u16  wParamLen = 0, u16  wAppId = 32 );
protected:
	u32	m_dwTesterNode;

};

#endif //_MT_INTEGRATION_TEST_H
