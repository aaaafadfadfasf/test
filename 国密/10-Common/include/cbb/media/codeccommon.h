/** 
* @file         CodecCommon.h 
* @brief        mediasdk 
* @details      ͨ�ö��� 
* @author       ��ѩ��
* @date     2004/02/07
* @version  3.0  
* @par Copyright (c):  
*      kedacom 
* @par History:          
*   version: ��ѩ��, 2004/02/07, ͨ�ö���\n 
*/ 
#ifndef __COMMON_H_
#define __COMMON_H_

/** 
 * ������ɫ 
 */ 
typedef struct BackBGDColor
{
	u8 RColor;    ///<R����
	u8 GColor;    ///<G����
	u8 BColor;    ///<B����	
}TBackBGDColor;

/** 
 * �ش����� 
 */ 
typedef struct tagNetRSParam
{
    u16  m_wFirstTimeSpan;	  ///<��һ���ش�����
	u16  m_wSecondTimeSpan;  ///<�ڶ����ش�����
	u16  m_wThirdTimeSpan;   ///<�������ش�����
	u16  m_wRejectTimeSpan;  ///<���ڶ�����ʱ����
} TNetRSParam;

/*==================================������=======================*/

#ifndef DES_ENCRYPT_MODE
#define DES_ENCRYPT_MODE         (u8)0      ///<DES����ģʽ
#define AES_ENCRYPT_MODE         (u8)1      ///<AES����ģʽ
#define ENCRYPT_KEY_SIZE         (u8)32     ///<��Կ���� ȡ���еĽϴ�ֵ
#define DES_ENCRYPT_KEY_SIZE     (u8)8      ///<DES��Կ����
#define AES_ENCRYPT_KEY_SIZE_MODE_A (u8)16  ///<AES Mode-A ��Կ����
#define AES_ENCRYPT_KEY_SIZE_MODE_B (u8)24  ///<AES Mode-B ��Կ����
#define AES_ENCRYPT_KEY_SIZE_MODE_C (u8)32  ///<AES Mode-C ��Կ����
#endif

#define CODEC_NO_ERROR              (u16)0///<�޴�
#define CODEC_ERROR_BASE            (u16)10000///<�������ֵ
#define ERROR_CODEC_UNKNOWN	        (u16)10000///<��֪���ı���������� 
#define ERROR_CODEC_PARAM           (u16)(CODEC_ERROR_BASE + 1)///<��������;  
#define ERROR_DEC_DRAW_HDC	        (u16)(CODEC_ERROR_BASE + 2)///<��������ͼ����DC����
#define ERROR_DEC_DRAW_DIBOPEN      (u16)(CODEC_ERROR_BASE + 3)///<��������ͼ����򿪴���
#define ERROR_DEC_DRAW_DIBBEGIN     (u16)(CODEC_ERROR_BASE + 4)///<��������ͼ��ʽ����
#define ERROR_DEC_DRAW_CLEARWINDOW  (u16)(CODEC_ERROR_BASE + 5)///<��������������
#define ERROR_DEC_ICOPEN            (u16)(CODEC_ERROR_BASE + 6)///<�������򿪽�����������
#define ERROR_DEC_DECOMPRESSBEGIN   (u16)(CODEC_ERROR_BASE + 7)///<���������ý���������ʽ����
#define ERROR_DEC_SETTIMER          (u16)(CODEC_ERROR_BASE + 8)///<���������ý��붨ʱ������
#define ERROR_DEC_NO_VIDEO          (u16)(CODEC_ERROR_BASE + 9)///<������û�д�����Ƶ
#define ERROR_DEC_SOCKET_CALLBACK   (u16)(CODEC_ERROR_BASE + 10)///<���������������¼��ص�����
#define ERROR_DEC_SOCKET_CREATE     (u16)(CODEC_ERROR_BASE + 11)///<���������������׽��ִ���
#define ERROR_DEC_NO_AUDIO          (u16)(CODEC_ERROR_BASE + 12)///<������û�д�����Ƶ 
#define ERROR_DEC_WAVEOPEN          (u16)(CODEC_ERROR_BASE + 13)///<���������ܴ�����*
#define ERROR_DEC_MIXEROPEN         (u16)(CODEC_ERROR_BASE + 14)///<���������ܴ�������������
#define ERROR_DEC_WAVEPREHEADER     (u16)(CODEC_ERROR_BASE + 15)///<����������������Ƶ����ͷ
#define ERROR_DEC_WAVEWRITE         (u16)(CODEC_ERROR_BASE + 16)///<����������������д��Ƶ����
#define ERROR_DEC_WAVE_ENCOPEN      (u16)(CODEC_ERROR_BASE + 17)///<���������ܴ���Ƶ������*
#define ERROR_DEC_WAVE_ENCPREHEADER (u16)(CODEC_ERROR_BASE + 18)///<����������������Ƶ����ͷ
#define ERROR_DEC_WAVE_ENCCONVERT   (u16)(CODEC_ERROR_BASE + 19)///<��������Ƶ�������
#define ERROR_DEC_WAVESTART         (u16)(CODEC_ERROR_BASE + 20)///<��������ʼ��Ƶ���Ŵ���
#define ERROR_DEC_WAVESTOP          (u16)(CODEC_ERROR_BASE + 21)///<������ֹͣ��Ƶ���Ŵ���
#define ERROR_DEC_MIXER_SETVOLUME   (u16)(CODEC_ERROR_BASE + 22)///<������������������
#define ERROR_DEC_MIXER_GETVOLUME   (u16)(CODEC_ERROR_BASE + 23)///<��������ȡ��������
#define ERROR_DEC_MIXERMUTE         (u16)(CODEC_ERROR_BASE + 24)///<���������þ�������
#define ERROR_DEC_AUDIOOUT_EXIT     (u16)(CODEC_ERROR_BASE + 25)///<��������Ƶ�߳��˳�����
#define ERROR_DEC_HARDCREATE        (u16)(CODEC_ERROR_BASE + 26)///<����������Ӳ��������ʧ��
#define ERROR_DEC_CREATE            (u16)(CODEC_ERROR_BASE + 27)///<�������������� 
#define ERROR_DEC_NEW               (u16)(CODEC_ERROR_BASE + 28)///<�������ɳ���
#define ERROR_DEC_SETAUDIO_ADDR     (u16)(CODEC_ERROR_BASE + 29)///<������������Ƶ��ַ����  
#define ERROR_DEC_AUDIO_LEN         (u16)(CODEC_ERROR_BASE + 30)///<��Ƶ���������ݳ��Ȳ���ȷ
#define ERROR_DEC_NO_MASK           (u16)(CODEC_ERROR_BASE + 31)///<�Ȳ����������Ҳ����Ӳ������
#define ERROR_SOCKOBJ_NULL          (u16)(CODEC_ERROR_BASE + 32)///<������ն��󲻴���
#define ERROR_DEC_NET_INVALIDPARAM  (u16)(CODEC_ERROR_BASE + 33)///<��Ч��������ղ��� 
#define EROOR_DEC_NO_IMPLEMENT      (u16)(CODEC_ERROR_BASE + 34)///<�˹��ܻ���֧�� 
#define ERROR_DEC_SETSTATE          (u16)(CODEC_ERROR_BASE + 35)///<���������ý�������״̬����
#define	ERROR_DEC_SEMCREATE			(u16)(CODEC_ERROR_BASE + 36)///<�����ź�����
#define	ERROR_DEC_NOTCREATE			(u16)(CODEC_ERROR_BASE + 37)///<������û�д���
#define	ERROR_DEC_FILEPLAYSTATUS	(u16)(CODEC_ERROR_BASE + 38)///<�ļ�����״̬����
#define ERROR_DEC_FILEEND           (u16)(CODEC_ERROR_BASE + 39)///<�ļ����Ž���

#define ERROR_ENC_CAPWIN            (u16)(CODEC_ERROR_BASE + 100)///<�������������񴰿ڳ���
#define ERROR_ENC_CAPDRIVER         (u16)(CODEC_ERROR_BASE + 101)///<������û�з��ֲ���
#define ERROR_ENC_DIBOPEN           (u16)(CODEC_ERROR_BASE + 102)///<��������ͼ����򿪴���
#define ERROR_ENC_DRAWHDC           (u16)(CODEC_ERROR_BASE + 103)///<��������ͼ���ڴ򲻿�
#define ERROR_ENC_ICOPEN            (u16)(CODEC_ERROR_BASE + 104)///<�������������򿪳���
#define ERROR_ENC_DRAWPOS           (u16)(CODEC_ERROR_BASE + 105)///<�������õ���ͼλ�ó���
#define ERROR_ENC_DIBBEGIN          (u16)(CODEC_ERROR_BASE + 106)///<���������û�ͼ��ʽ����
#define ERROR_ENC_GET_VIDEOBMP      (u16)(CODEC_ERROR_BASE + 107)///<�������õ�ͼ���ʽ����
#define ERROR_ENC_SET_VIDEOBMP      (u16)(CODEC_ERROR_BASE + 108)///<����������ͼ���ʽ����
#define ERROR_ENC_GET_CAPSET        (u16)(CODEC_ERROR_BASE + 109)///<�������õ��������ó���
#define ERROR_ENC_SET_CAPSET        (u16)(CODEC_ERROR_BASE + 110)///<���������ò������ó���
#define ERROR_ENC_SET_AUDIOFORMATE  (u16)(CODEC_ERROR_BASE + 111)///<������������Ƶ�����ʽ����
#define ERROR_ENC_STREAMOPEN        (u16)(CODEC_ERROR_BASE + 112)///<����������Ƶ����������
#define ERROR_ENC_PREHEADER         (u16)(CODEC_ERROR_BASE + 113)///<������׼����Ƶ����ͷ����
#define ERROR_ENC_MIXEROPEN         (u16)(CODEC_ERROR_BASE + 114)///<�������������������������
#define ERROR_ENC_MIXER_SETVOLUME   (u16)(CODEC_ERROR_BASE + 115)///<������������������
#define ERROR_ENC_MIXER_GETVOLUME   (u16)(CODEC_ERROR_BASE + 116)///<�������õ���������
#define ERROR_ENC_ALREADY           (u16)(CODEC_ERROR_BASE + 117)///<������״̬�Ѿ�����
#define ERROR_ENC_STOPCAP           (u16)(CODEC_ERROR_BASE + 118)///<�������޷�ֹͣ����
#define ERROR_ENC_SETSTATE          (u16)(CODEC_ERROR_BASE + 119)///<���������ñ����������
#define ERROR_ENC_ENCBEGIN          (u16)(CODEC_ERROR_BASE + 120)///<���������ñ���֡��ʽ
#define ERROR_ENC_SETTIMER          (u16)(CODEC_ERROR_BASE + 121)///<���������ö�ʱ������
#define ERROR_ENC_STOPVIDEOENC      (u16)(CODEC_ERROR_BASE + 122)///<������ֹͣ��Ƶ�������
#define ERROR_ENC_NO_SETPARAM       (u16)(CODEC_ERROR_BASE + 123)///<������û�����ò���
#define ERROR_ENC_STOPAUDIOENC      (u16)(CODEC_ERROR_BASE + 124)///<��������Ƶ�������
#define ERROR_ENC_CREATESOCK        (u16)(CODEC_ERROR_BASE + 125)///<���������õ��ص�ַ����
#define ERROR_ENC_STOPSENDVIDEO     (u16)(CODEC_ERROR_BASE + 126)///<������ֹͣ������Ƶ����
#define ERROR_ENC_STOPSENDAUDIO     (u16)(CODEC_ERROR_BASE + 127)///<������ֹͣ������Ƶ����
#define ERROR_ENC_AUDIOENC          (u16)(CODEC_ERROR_BASE + 128)///<��������Ƶת������
#define ERROR_ENC_NO_CREATE         (u16)(CODEC_ERROR_BASE + 129)///<�������������ɹ�  
#define ERROR_ENC_SOCK_INIT         (u16)(CODEC_ERROR_BASE + 130)///<�����������׽��ֲ���2.2��
#define ERROR_ENC_STARTCAP          (u16)(CODEC_ERROR_BASE + 131)///<�������޷���ʼ����
#define ERROR_ENC_CAP_WIDTH         (u16)(CODEC_ERROR_BASE + 132)///<����������̫֡��
#define ERROR_DRAW_FORMATE          (u16)(CODEC_ERROR_BASE + 133)///<��ͼ��ʽ��֧��
#define ERROR_HARDENC_FORMATE       (u16)(CODEC_ERROR_BASE + 134)///<Ӳ����֧�ֵĻ�ͼ��ʽ
#define ERROR_ENC_CAP_FORMATE       (u16)(CODEC_ERROR_BASE + 135)///<�ɼ���ʽ�ͱ����ʽ��ƥ��
#define ERROR_ENC_NEW               (u16)(CODEC_ERROR_BASE + 136)///<�������ɳ���
#define ERROR_ENC_INVALID_PARAM     (u16)(CODEC_ERROR_BASE + 137)///<��Ч�Ĳ���
#define ERROR_ENC_CREATE_EVENT      (u16)(CODEC_ERROR_BASE + 138)///<���ܴ����¼� 
#define EROOR_ENC_NO_IMPLEMENT      (u16)(CODEC_ERROR_BASE + 139)///<�˹��ܻ���֧�� 
#define ERROR_ENC_GETSTATE          (u16)(CODEC_ERROR_BASE + 140)///<�������õ������������
#define ERROR_DECENC_CREATE_THREAD  (u16)(CODEC_ERROR_BASE + 141)///</���ܴ����߳�

#define ERROR_ENC_CAP_DESKTOP_FORMAT    (u16)(CODEC_ERROR_BASE + 142)///<Ҫ��ı��������沶���ʽ����֧��
#define ERROR_ENC_CREATE_DESKTOP_MEMDC  (u16)(CODEC_ERROR_BASE + 143)///<���������ڴ�dcʧ��
#define ERROR_ENC_CAP_DESKTOP_WIDTH     (u16)(CODEC_ERROR_BASE + 144)///<��������������ֱ��ʸ��ڵ�ǰ����ʵ�ʷֱ���
#define ERROR_ENC_CAP_DESKTOP_BITCOUNT  (u16)(CODEC_ERROR_BASE + 145)///<��������������λ����ڵ�ǰ����ʵ��λ��
#define ERROR_ENC_INVILAD_FILE          (u16)(CODEC_ERROR_BASE + 146)///<�ṩ���ļ���Ч
#define ERROR_ENC_OPERATE_FILE          (u16)(CODEC_ERROR_BASE + 147)///<�ļ���������ָ�����ļ�����ʧ��
#define ERROR_DEC_SNAP_NO_VIDEO         (u16)(CODEC_ERROR_BASE + 148)///<������ץ����ͼ��
#define ERROR_DEC_SNAP_NOSPACE          (u16)(CODEC_ERROR_BASE + 149) ///<������ץ�Ĵ�����ʣ��ռ�
#define ERROR_ENC_NO_AUDIO_INPUTDEV     (u16)(CODEC_ERROR_BASE + 150) ///<û����Ƶ�����豸

#define  VIDCAP_CAPVIDEO			(u8)1	//�ɼ�video
#define  VIDCAP_CAPVGA				(u8)2	//�ɼ�vga
//====================�ṹ����=====================================//
/*ͼ��ѹ�����ʿ��Ʋ���*/
#define	 RC_MODE_CONST_BITRATE	 (u8)0x0 ///<������
#define  RC_MODE_VAR_BITRATE	 (u8)0x1 ///<������

/** 
 * ��Ƶ������� 
 */ 
typedef struct VideoEncParam
{
	u8  m_byEncType;   ///<ͼ���������
    u8  m_byRcMode;    ///<ͼ��ѹ�����ʿ��Ʋ���
    u16  m_byMaxKeyFrameInterval;///<I֡�����P֡��Ŀ
    u8  m_byMaxQuant;  ///<�����������(1-31)
    u8  m_byMinQuant;  ///<��С��������(1-31)
    u8  m_byReserved1; ///<����
    u8  m_byReserved2; ///<����
    u8  m_byReserved3; ///<����
    u16  m_wBitRate;    ///<���������(Kbps)
    u16  m_wReserved4;  ///<����		
	u32 m_dwSndNetBand;///<���緢�ʹ���(��λ:Kbps,1K=1024)   
    u32 m_dwReserved5; ///<����
	u8  m_byQcMode; ///<ͼ���������ʿ��Ʋ���,�������Ȼ����ٶ�����
    u8  m_byQI; ///< QIֵ
    u8  m_byQP;///< QPֵ

	u8  m_byFrameFmt; ///< �ֱ��� 0:���̶�����, 1: half cif, 2: 1 cif, 3: 2 cif,  4: 4 cif,  5: ����ͼ��ϳɱ���  
	u8	m_byFrmRateCanSet; ///<֡���Ƿ�������趨 ?
	u8  m_byFrmRateLittleThanOne; ///< ֡���Ƿ�С��1 ?
}TVideoEncParam;

/** 
 * ������ͳ����Ϣ 
 */ 
typedef struct KdvDecStatis
{
	u8	  m_byVideoFrameRate;///<��Ƶ����֡��
	u16	  m_wVideoBitRate;   ///<��Ƶ��������
	u16	  m_wAudioBitRate;   ///<������������
	u32   m_dwVideoRecvFrame;///<�յ�����Ƶ֡��
	u32   m_dwAudioRecvFrame;///<�յ�������֡��
	u32   m_dwVideoLoseFrame;///<��ʧ����Ƶ֡��
	u32   m_dwAudioLoseFrame;///<��ʧ������֡��
    u16   m_wVideoLoseRatio;///<��Ƶ��ʧ��,��λ��% 	
	u16   m_wAudioLoseRatio;///<��Ƶ��ʧ��,��λ��% 	
	u32   m_dwPackError;///<��֡��
	u32   m_dwIndexLose;///<��Ŷ�֡
	u32   m_dwSizeLose; ///<��С��֡
	u32   m_dwFullLose; ///<����֡	
	u32   m_wAvrVideoBitRate;   ///<1��������Ƶ����ƽ������
	u32	  m_wAvrAudioBitRate;   ///<1��������������ƽ������
	BOOL32 m_bVidCompellingIFrm;  ///<��Ƶǿ������I֡							  
	u32   m_dwDecdWidth;	///<�����Ŀ�
	u32   m_dwDecdHeight;   ///<�����ĸ�
}TKdvDecStatis; 



#endif





















