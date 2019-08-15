/******************************************************************************
ģ����	�� camera
�ļ���	�� newcamera.h
����ļ��� 
ʵ�ֹ��ܣ� ģ��ͷ�ļ��������ͱ����Ķ���,����ӿ����� 

  -------------------------------------------------------------------------------
  �޸ļ�¼:
  ��  ��		�汾		�޸���		�޸�����
  2005/09/14	3.0		    ���³�      ������ע��
******************************************************************************/
#ifndef _NEWCAMERA_H
#define _NEWCAMERA_H

#include "kdvtype.h"

// for Microsoft c++
#ifdef _MSC_VER

#ifndef API
#define API extern "C"			__declspec(dllexport)
#endif

// for gcc
#else
#ifndef API
#define API extern "C"
#endif

#endif	// _MSC_VER

#define CAM_TYPE_LEN            (u8)32    //Э�����Ƴ���
#define MAX_PROTOCOL_LEN		(u8)30    //Э�����󳤶�
#define MAX_CAM_PRTCL_NUM       (u8)4     //�������������� 
#define	MAX_CAM_CAP_NUM			(u8)40	  //�������ͷ����������
#define	MAX_MATRIX_CAP_NUM		(u8)10	  //����������������
//�����˵��

//����ͷ����
#define CAM_NAME_SONY	                        "SONY"
#define CAM_NAME_AD								"AD"
#define CAM_NAME_PELCO							"PELCO-D"
#define CAM_NAME_PIH							"����"
#define CAM_NAME_PELCO_P						"PELCO-P"
#define CAM_NAME_PARCO							"PARCO�ظ�"
#define CAM_NAME_AB								"AB"
#define CAM_NAME_YUNTAI							"������̨"
#define CAM_NAME_Samsung						"Samsung����"
#define CAM_NAME_YAAN							"YUNTAI�ǰ�"
#define CAM_NAME_AD168							"AD168"
#define CAM_NAME_KALATEL						"KALATEL������"
#define CAM_NAME_JN								"JNΰ��"
#define CAM_NAME_WV_CS650						"����CS650"
#define CAM_NAME_WV_CS850						"����CS850"
#define CAM_NAME_ALEC							"ALEC������"
#define CAM_NAME_AB_P							"AB-P"
#define CAM_NAME_AB_D							"AB-D"
#define CAM_NAME_PELCO_D_JEC					"JEC PELCO-D"
#define CAM_NAME_PELCO_P_JEC					"JEC PELCO-P"
#define CAM_NAME_TIANDY                         "���ΰҵ"
#define CAM_NAME_G100                           "G100"
#define CAM_NAME_368ZT                          "368ZT"
#define CAM_NAME_GaoDeTI                        "GaoDeTI"
#define CAM_NAME_DaliTI                         "DaliTI"

#define CAM_NAME_MATRIX							"ͨ�þ���Э��"
#define CAM_NAME_ROBOT							"ROBOT ����ϳɾ���"

//����ͷ������
enum TCamType{
        CAM_TYPE_BEGIN = 0,
	    CAM_TYPE_SONY,	        //	SONY
		CAM_TYPE_AD,			//	AD
		CAM_TYPE_PELCO,			//	PELCO-D
		CAM_TYPE_PIH,			//	����
		CAM_TYPE_PELCO_P,		//	PELCO-P
		CAM_TYPE_PARCO,			//	PARCO�ظ�
		CAM_TYPE_AB,			//	AB
		CAM_TYPE_YUNTAI,		//	������̨
		CAM_TYPE_Samsung,		//	����
		CAM_TYPE_YAAN,			//	�ǰ�
		CAM_TYPE_AD168,			//	AD168����
		CAM_TYPE_KALATEL,		//	������
		CAM_TYPE_JN,			//	JNΰ��
		CAM_TYPE_WV_CS650,		//	����,��850����Э��
		CAM_TYPE_WV_CS850,		//	����,��850����Э��
		CAM_TYPE_ALEC,		    //	������
		CAM_TYPE_AB_P,          //	AB-P
		CAM_TYPE_AB_D,          //	AB-D
		CAM_TYPE_PELCO_D_JEC,   //	JEC PELCO-D
		CAM_TYPE_PELCO_P_JEC,   //	JEC PELCO-P
        CAM_TYPE_TIANDY,        //  ���ΰҵ
        CAM_TYPE_G100,          //  G100
        CAM_TYPE_368ZT,         //  368ת̨���
        CAM_TYPE_GaoDeTI,       //  GaoDeTI������
        CAM_TYPE_DaliTI,        //  DaliTI������
		CAM_TYPE_2130_PELCO_P,  //  �Ǳ�׼PELCO_PЭ��
		CAM_TYPE_K_PELCO_D,
		CAM_TYPE_JVCTKC676,
		CAM_TYPE_INFINOVA_PELCO_D,
		CAM_TYPE_PEARMAIN_PELCO_D,
		CAM_TYPE_PEARMAIN_PELCO_P,
		CAM_TYPE_YAAN_PELCO_D,
		CAM_TYPE_CAMERA_NUM,

        CAM_TYPE_CAMERA_NOCTLR = 63,    //���ɿ��豸
		CAM_TYPE_MATRIX=128,	//	����Э��
		MATRIX_TYPE_AB=128,		//  AB ����
		CAM_TYPE_ROBOT,			//  ROBOT ����ϳɾ���
		MATRIX_TYPE_SUNNHO,		//  SUNNHO	����
		MATRIX_TYPE_PEARMAIN,   //  ��ƻ��  ����
		MATRIX_TYPE_AD,         //  AD ����
		MATRIX_TYPE_INFORNOVA,
		MATRIX_TYPE_PECLO,      
		MATRIX_TYPE_HONEWELL,  //����Τ��
		MATRIX_TYPE_TIANDI,
		MATRIX_TYPE_HUATONG,//��ͨ����
		MATRIX_TYPE_TONGCHUANG,//ͬ������
		MATRIX_TYPE_BOCOM,     //��������
		MATRIX_TYPE_EXTRON,    //��˼��
		MATRIX_TYPE_ST_MS650,  //������
		MATRIX_TYPE_LIKEN,     //����
		MATRIX_TYPE_VINET,     //΢����
		MATRIX_TYPE_WEPUS,     //����ʿ
		CAM_TYPE_MATRIX_NUM
};

//����ͷ�Ŀ�������
enum TCamCommand{
	CAM_COMMAND_MOVEUP=1,			//(u8)1		//�����ƶ�
	CAM_COMMAND_MOVEDOWN,	    	//(u8)2		//�����ƶ�
	CAM_COMMAND_MOVELEFT,			//(u8)3		//�����ƶ�
	CAM_COMMAND_MOVERIGHT,			//(u8)4		//�����ƶ�
	CAM_COMMAND_MOVEHOME,			//(u8)5		//�ع�
	CAM_COMMAND_MOVESTOP,			//(u8)6		//ֹͣ�ƶ�
	CAM_COMMAND_ZOOMTELE,		    //(u8)7		//��������ͷ
	CAM_COMMAND_ZOOMWIDE,			//(u8)8		//��Զ����ͷ
	CAM_COMMAND_ZOOMSTOP,			//(u8)9		//��Ұ����ֹͣ
	CAM_COMMAND_FOCUSFAR,		    //(u8)10	//�������Զ
	CAM_COMMAND_FOCUSNEAR,			//(u8)11	//���������
	CAM_COMMAND_FOCUSAUTO,			//(u8)12	//�Զ�����
	CAM_COMMAND_FOCUSSTOP,			//(u8)13	//����ֹͣ
	CAM_COMMAND_PRESETSET,	    	//(u8)14	//����ͷԤ��
	CAM_COMMAND_PRESETCALL,			//(u8)15	//������ͷԤ��
	CAM_COMMAND_CAMERASET,			//(u8)16	//��ʼ������ͷ
	CAM_COMMAND_BRIGHTUP,			//(u8)17	//�������
	CAM_COMMAND_BRIGHTDOWN,			//(u8)18	//�������
	CAM_COMMAND_BRIGHTSTOP,			//(u8)19	//ֹͣ����
	CAM_COMMAND_NEW,				//(u8)20	//������
	CAM_COMMAND_APPEND,				//(u8)21	//��������
	CAM_COMMAND_GOTOPOINT,			//(u8)22	//���Ķ�λ
	CAM_COMMAND_ZOOMPART,			//(u8)23	//�ֲ��Ŵ�
	
	CAM_COMMAND_PANSPEED,			//(u8)24	//������ˮƽ�ƶ��ٶ�����������ڲ�ʹ�ã�
	CAM_COMMAND_TILTSPEED,			//(u8)25	//�����˴�ֱ�ƶ��ٶ�����������ڲ�ʹ�ã�
	CAM_COMMAND_SETBRIGHT,			//(u8)26	//sony���ȵ�����������ڲ�ʹ�ã�
	CAM_COMMAND_MATRIX_TRANSIO,		//(u8)27
	CAM_COMMAND_MATRIX_SWTICH=27,	//(u8)27	//�л���Ƶ
	CAM_COMMAND_MATRIX_VIDMIX4,		//(u8)28	//����ϳ�2��2
	CAM_COMMAND_MATRIX_VIDMIX9,		//(u8)29	//����ϳ�3��3
	CAM_COMMAND_MATRIX_VIDMIX16,	//(u8)30	//����ϳ�4��4
	CAM_COMMAND_MATRIX_VIDMIXPP,	//(u8)31	//����ϳɻ��л�

	CAM_COMMAND_ZOOMWHOLE,			//(u8)32    //�ֲ���С 

////�˵�//////////////////////////////////////////////////////////////////////	
	CAM_COMMAND_MENU_UP,			//(u8)33	//�˵���
	CAM_COMMAND_MENU_DOWN,			//(u8)34	//�˵���
	CAM_COMMAND_MENU_LEFT,			//(u8)35	//�˵���
	CAM_COMMAND_MENU_RIGHT,			//(u8)36	//�˵���

	CAM_COMMAND_MENU_SHOW,			//(u8)37	//�˵���ʾ
	CAM_COMMAND_MENU_HIDE,			//(u8)38	//�˵�����

	CAM_COMMAND_MENU_ENTER,			//(u8)39	//�˵�ȷ��
	CAM_COMMAND_MENU_EXIT,		    //(u8)40	//�˵�ȡ��

	CAM_COMMAND_PRESETDEL,			//(u8)41	//ɾ������ͷԤ��
////�����л�//////////////////////////////////////////////////////////////////////
	CAM_COMMAND_MATRIX_SWTICHASC,	//(u8)42	//�����л�
	CAM_COMMAND_MATRIX_SWTICHDESC,	//(u8)43	//�����л�
	CAM_COMMAND_MATRIX_IMAGEHOLD,	//(U8)44	//ͼ�񱣳�

	CAM_COMMAND_MOVELEFTUP,			//(u8)45	//�������ƶ�
	CAM_COMMAND_MOVELEFTDOWN,	    //(u8)46	//�������ƶ�
	CAM_COMMAND_MOVERIGHTUP,		//(u8)47	//�������ƶ�
	CAM_COMMAND_MOVERIGHTDOWN,		//(u8)48	//�������ƶ�

	CAM_COMMAND_WIDEDYNAMIC_ON=65,	//(u8)65	//��̬��
	CAM_COMMAND_WIDEDYNAMIC_OFF,	//(u8)66	//��̬��
////Ѳ��//////////////////////////////////////////////////////////////////////	
	CAM_COMMAND_STARTAUTOSCAN=128,  //(u8)128	//��ʼ�Զ�Ѳ��
	CAM_COMMAND_STOPAUTOSCAN,       //(u8)129	//ֹͣ�Զ�Ѳ��	
	CAM_COMMAND_STARTPRESETSCAN,	//(u8)130	//��ʼѲ��
	CAM_COMMAND_STOPPRESETSCAN,     //(u8)131	//ֹͣѲ��
	CAM_COMMAND_SCANSPEED_SET,		//(u8)132	//ˮƽɨ���ٶȣ�Ӱ���Զ�ɨ���ٶȣ�bySpeed(1-40s)

	//add by Royan 20140305 �����ˢ����
////��ˢ//////////////////////////////////////////////////////////////////////
	CAM_COMMAND_STARTWIPER = 160,	//(u8)160	//��ʼ��ˢ����һ������Ϊ��ˢ�ٶȣ�ֵΪ0-15��������������
	CAM_COMMAND_STOPWIPER,			//(u8)161	//ֹͣ��ˢ
	CAM_COMMAND_LIGHTON,            //(u8)162   //�����ƹ�
	CAM_COMMAND_LIGHTOFF,           //(u8)163   //�رյƹ�
	CAM_COMMAND_START_EXTEND1,      //(u8)164   //��չ����һ���ݶ�������ˮ��
	CAM_COMMAND_STOP_EXTEND1,       //(u8)165   //�ر���ˮ
	CAM_COMMAND_START_EXTEND2,      //(u8)166   //��չ���ܶ����ݶ�������˪��
	CAM_COMMAND_STOP_EXTEND2,       //(u8)167   //�رճ�˪
};

//#define MAX_TYPE_DISCRIBE		(u8)10    //���Э�鳤��
#define MAX_PAN_SPEED			(u8)15    //pan speed 0-MAX_PAN_SPEED
#define MAX_TILT_SPEED			(u8)15    //tilt speed 0-MAX_TILT_SPEED
#define MAX_ZOOM_SPEED			(u8)15    //zoom speed 0-MAX_ZOOM_SPEED
#define MAX_PREPOS_RANGE		(u8)16    //Ԥ��λ�ŷ�Χ
#define MAX_ALEC_WIDE           (u8)254   //�����˾ֲ��Ŵ���������
#define MAX_ALEC_HIGH           (u8)254   //�����˾ֲ��Ŵ�������߶�
#define MIN_ALEC_WIDE           (u8)11    //�����˾ֲ��Ŵ������С���
#define MIN_ALEC_HIGH           (u8)11    //�����˾ֲ��Ŵ������С�߶�
#define MAX_MATRIX_VIDMIX_NUM   (u8)16    //���������ϳ���������


#ifdef WIN32
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct tagCamInfo
{
	u8	m_byCamType;			//����ͷ�����ͣ����ڹ���13��
	u16 m_wCamId;				//����ͷ�ĵ�ַ
	u8	m_byCommandId;			//��������
    u8	m_byParam1;				//����1����������Ĳ������岻ͬ,������������
	u8	m_byParam2;				//����2
    u8	m_byParam3;				//����3������3��4Ϊ�����˾ֲ��Ŵ���ʹ��
	u8	m_byParam4;				//����4
	u8	m_byCamCtrlWay;			//����ͷ��������Ʒ�ʽ
}
#ifndef WIN32
__attribute__((packed))
#endif
TCamInfo;

typedef struct tagMatrixInfo 
{
	u8 m_byCamType;    //��������
	u8 m_byCommandID;  //��������
	u8 m_byInputNum;  //ѡ�������˿�����, camera
	//����˿��б���m_byCommID=Switchʱ��Ĭ�Ͻ�m_awInputList[0]��Ч
	u16 m_awInputList[MAX_MATRIX_VIDMIX_NUM];
	u8 m_byOutputPort;  //��������˿�, monitor
}
#ifndef WIN32
__attribute__((packed))
#endif
TMatrixInfo;



typedef struct tagCamData
{
	u8	m_abyCamData[MAX_PROTOCOL_LEN];//Э������
    s32 m_iNumOfData;			//Э���ʵ�ʳ���
	public:
		tagCamData()
		{
			memset(this, 0, sizeof(tagCamData));
		};
}
#ifndef WIN32
__attribute__((packed))
#endif
TCamData;

typedef struct tagCamPrtcl
{
	TCamData m_atCamPrtcl[MAX_CAM_PRTCL_NUM];	//Э������
	u8 m_byInterval;            //����ʱ��
    u8 m_byNumPrtcl;			//Э����Ŀ
	public:
		tagCamPrtcl()
		{
			memset(this, 0, sizeof(tagCamPrtcl));
		};
}
#ifndef WIN32
__attribute__((packed))
#endif
TCamPrtcl;

//�õ�ĳһ����ĳһ���������ͷЭ��
//������ֵΪTRUEʱ�õ���ȷ��Э��;
//��ΪFALSEʱ����ʱ�޴�����ͷ���ͻ��������
//ptCamInfo��֪������ptCamData����Э�飬pbyInterval//��������=0��ͣ,=0xffֹͣ����,��0,0xff���������
BOOL32 GetCamProtocol(const TCamInfo *ptCamInfo,TCamPrtcl *ptCamPrtcl);
BOOL32 GetCamProtocol(TMatrixInfo *ptMatrixInfo,TCamPrtcl *ptCamPrtcl);

/*���������*/
/*
����:CAM_COMMAND_MOVEUP,CAM_COMMAND_MOVEDOWN,CAM_COMMAND_MOVELEFT,
CAM_COMMAND_MOVERIGHT,CAM_COMMAND_MOVEHOME,CAM_COMMAND_MOVESTOP
����:(m_byParam1,m_byParam2��������)
m_byParam1/��Χ:0-MAX_PAN_SPEED
m_byParam2/��Χ:0-MAX_TILT_SPEED

����:CAM_COMMAND_ZOOMTELE,CAM_COMMAND_ZOOMWIDE
����:(m_byParam1������)
m_byParam1/��Χ:0-MAX_ZOOM_SPEED

����:CAM_COMMAND_ZOOMSTOP,CAM_COMMAND_FOCUSFAR,CAM_COMMAND_FOCUSNEAR,
CAM_COMMAND_FOCUSAUTO,CAM_COMMAND_FOCUSSTOP
����:(m_byParam1,m_byParam2��������)

����:CAM_COMMAND_PRESETSET,CAM_COMMAND_PRESETCALL
����:(m_byParam1������)
m_byParam1/��Χ:1-16

����:CAM_COMMAND_CAMERASET
����:(m_byParam1,m_byParam2��������)
*/
struct TProtoclInfo
{
	u8 byID;  //����id
	s8 ptachType[CAM_TYPE_LEN+1]; //��������
public:
	TProtoclInfo()
	{
		memset(this, 0, sizeof(TProtoclInfo));
	};
}
#ifndef WIN32
__attribute__((packed))
#endif
;

#ifdef WIN32
#pragma pack(pop)
#endif


/*====================================================================
������      : GetCamCapSet
����        : ��ȡ֧��Э������
�������˵��: TProtoclInfo tCam[]
����ֵ˵��  : Э����Ŀ
====================================================================*/
API u8 GetCamCapSet(TProtoclInfo tCam[]);

/*====================================================================
������      : GetStrCamType
����        : ��ȡ�ַ�����ʽ��Э������
�������˵��: dwCamType
����ֵ˵��  : StrCamType
====================================================================*/
API LPCSTR GetStrCamType(u32 dwCamType);

/*====================================================================
������      : GetStrCamCmd
����        : ��ȡ�ַ�����ʽ��Э������
�������˵��: dwCamCmd
����ֵ˵��  : StrCamCmd
====================================================================*/
API LPCSTR GetStrCamCmd(u32 dwCamCmd);  //����Ӣ��
API LPCSTR GetStrCommand(u8 byCmd);      //��������

/*====================================================================
������      : GetCamCapSet
����        : ��ȡ֧��Э����Ŀ
����ֵ˵��  : Э����Ŀ
====================================================================*/
API u8 GetCamCapSetNum();
/*====================================================================
������      : GetMatrCapSet
����        : ��ȡ֧��Э������
�������˵��: TProtoclInfo tMatr[]
����ֵ˵��  : Э����Ŀ
====================================================================*/
API u8 GetMatrCapSet(TProtoclInfo tMatr[]);

/*====================================================================
������      : GetCamCapSet
����        : ��ȡ֧��Э����Ŀ
����ֵ˵��  : Э����Ŀ
====================================================================*/
API u8 GetMatrCapSetNum();

#ifdef _LINUX_
void CamRegHelpCmd(); 
#endif

#endif
