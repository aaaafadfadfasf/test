/*****************************************************************************
   ģ����      : vtdu 
   �ļ���      : vtdulib.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: vtdulib�Ķ���ӿڶ���
   ����        : ������
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��         �汾        �޸���      �޸�����
   2012/01/06     1.0        ������        ����
******************************************************************************/
#ifndef _VTDULIB_H_
#define _VTDULIB_H_

#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_struct.h"
#ifdef _USE_NEW_KDM_NETAGENT_
    #include "cbb/kdmnatagent/kdmnatagent.h"
#else
    #include "cms/natagent/natagent.h"
#endif //_USE_NEW_KDM_NETAGENT_


//VTDUĬ�������ļ�·��
#define DEFAULT_PATH_VTDUCFG    "./conf/vtducfg.ini"

#define   KDM_NAT_DEFAULT_PU_SESSID         0xFFFFFFFF 


//VTDU����ģʽ
enum EMRunMode
{
    RUN_MODE_ATTACH = 1,    //����ģʽ����
    RUN_MODE_ABSOLUTE       //����
};

LPCSTR GetRunModeStr(EMRunMode emValue);

////////////////////////////////////////////////////////////////////////////////////
//���ݽṹ����������
////////////////////////////////////////////////////////////////////////////////////
struct TVtduCfg
{
    CIpList     m_cIpAddrList;
    int         m_nMediaStartPort;      //����������ʼ�˿�
	int         m_nMediaM2PPort;        //��Դ�����˿�
    int         m_nMaxSwitchNum;        //��󽻻�·��
    int         m_nMaxRcvGroup;         //��������
    int         m_nMaxSndMmbPerRcvGrp;  //ÿ�������Ŀ����
    bool        m_bIsTravGAP;           //�Ƿ�Խ��բ
    string      m_strCreditDomainUri;   //������
};

enum EMNatPackMediaTransChnType
{
	NAT_PACK_TYPE_INVALID    = 0,

	NAT_PACK_TYPE_VIDEO_RTP  = 1,
	NAT_PACK_TYPE_VIDEO_RTCP = 2,
	NAT_PACK_TYPE_AUDIO_RTP  = 3,
	NAT_PACK_TYPE_AUDIO_RTCP = 4,
	NAT_PACK_TYPE_DATA_RTP   = 5,
	NAT_PACK_TYPE_DATA_RTCP  = 6,
	NAT_PACK_TYPE_DATA_DATA  = 7, //����ʱ��????

	NAT_PACK_TYPE_NUM,
};

struct TNatData
{
protected:
    u32		m_dwSrcIp;		//ԴIP;
    u16		m_wSrcPort;		//Դ�˿�;
    u32		m_dwNatIp;		//NAT IP��ַ;
    u16		m_wNatPort;		//NAT �˿�;

    string	m_strID;		//�豸����
    u32		m_dwSessID;		//�ỰID;
	u8      m_byReserve;    //����������EMNatPackMediaTransChnType

    u16		m_wBindPort;	//�󶨶˿�
    u8		m_byChangeCnt;	//NAT�仯����

	string  m_strRecvedTime;//�յ�NAT̽�����ʱ��

public:
    TNatData(void);
    u32 GetSrcIp() const;			//��ȡԴIP
    void SetSrcIp(u32 dwSrcIp);		//����ԴIP
    u16 GetSrcPort() const;			//��ȡԴ�˿�
    void SetSrcPort(u16 wSrcPort);	//����Դ�˿�
    u32 GetNatIp() const;			//��ȡNAT IP��ַ
    void SetNatIp(u32 dwNatIp);		//����NAT IP��ַ
    u16 GetNatPort() const;			//��ȡNAT �˿�
    void SetNatPort(u16 wNatPort);	//����NAT �˿�
    const string& GetID() const;	//��ȡ�û����豸����
    void SetID(const string& tID);	//�����û����豸����
    u32 GetSessID() const;			//��ȡ�ỰID
    void SetSessID(u32 dwSessID);	//���ûỰID
    u8 GetReserve() const;			//��ȡ�����ֶ�
    void SetReserve(u8 byReserve);  //���ñ����ֶ�
    u8 GetChangeCnt(void)const;		//��ȡNAT�仯��
    void AddChangeCnt(u8 byChangeCnt);//����NAT�仯��
	string GetRecvedTime() const;
	void SetRecvedTime(string strRecvedTime);
    void PrintData() const;
};

struct TVtduPunchChn
{
    u16                 m_wLocalPort;
    TMediaTransChannel  m_tMediaTransChn;
};

typedef vector<TVtduPunchChn> TVtduPunchChnList;


//VTDU_QUERY_MAPPED_ADDR_NTF
struct TVtduStunResult
{
    EmResult        m_emResult;
    u32             m_dwResultNum;
    TStunResult     m_atResultList[NATAGENT_MAX_PORT_NUM];
};

//VTDU_PUNCH_RESULT_NTF
struct TVtduPunchResult
{
    EmResult        m_emResult;
    u32             m_dwResultNum;
    TPunchResult    m_atResultList[NATAGENT_MAX_PORT_NUM];
};


//�ڲ�����Ϣ�����Բ���ת������
struct TVtduCBMsg
{
public:
	TVtduCBMsg()
	{
		memset(this, 0, sizeof(TVtduCBMsg));
	}

	u32 GetEvent() const
	{
		return m_nEvent;
	}
	void SetEvent(u32 var)
	{
		m_nEvent = var;
	}

	u32 GetSeqNO() const
	{
		return m_dwSeqNO;
	}
	void SetSeqNO(u32 var)
	{
		m_dwSeqNO = var;
	}

	u32 GetContext() const
	{
		return m_dwContext;
	}
	void SetContext(u32 var)
	{
		m_dwContext = var;
	}

	const u8* GetMsgBody() const
	{
		return m_abyMsgBody;
	}
	void SetMsgBody(void* pbyBuf, u32 dwBufLen);

	void CatMsgBody(void* pbyBuf, u32 dwBufLen);

	u32 GetMsgBodyLen() const
	{
		return m_dwMsgBodyLen;
	}

	u32 GetSize() const
	{
		return sizeof(TVtduCBMsg) - sizeof(m_abyMsgBody) + m_dwMsgBodyLen;
	}

private:
	s32 m_nEvent;
	u32 m_dwSeqNO;
	u32 m_dwContext;	//ͨ������TaskNo
	u32 m_dwMsgBodyLen;
	u8  m_abyMsgBody[50*1024];
};

typedef void (*pNATDETECTPACKET_CB)(TNatData tNatData);

struct TVtduLibConf
{
	TVtduLibConf()
	{
		m_emRunMode = RUN_MODE_ABSOLUTE;
		m_dwDefaultOspIID = 0;
		m_fpNatDetectCB = NULL;
	}

	void PrintData() const
	{
		OspPrintf(TRUE, FALSE, "  VTDU����ģʽ   :[%s]\n", GetRunModeStr(m_emRunMode));
		OspPrintf(TRUE, FALSE, "  DefaultOspIID  :[%lu]\n", m_dwDefaultOspIID);
		OspPrintf(TRUE, FALSE, "  ConfigPath     :[%s]\n", m_strConfigPath.c_str());
	}

	EMRunMode m_emRunMode;
	u32 m_dwDefaultOspIID;	//VtduLibͶ����Ϣ��OspIID
	string m_strConfigPath;	//�գ���ʹ��DEFAULT_PATH_VTDUCFG
	pNATDETECTPACKET_CB m_fpNatDetectCB;
};
/*====================================================================
������      : VtduStart
����        : ��ʼ��VTDU
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: 
EMRunMode emRunMode --[in]vtdu������ģʽ
LPSTR lpsConfigPath --[in]vtdulib�����ļ�·����ֵΪNULLʱ��ʹ��Ĭ��·��DEFAULT_PATH_VTDUCFG��
����ֵ˵��  : TRUE -  �ɹ�
             FALSE -  ʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���          �޸�����
2012/01/06     1.0        ������        ����
====================================================================*/
BOOL32 VtduLibStart( const TVtduLibConf& tVtduLibConf);

/*====================================================================
������      : AddNatRcvPort
����        : �����Ҫ����Nat���Ķ˿�
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: u16 wPort --[in]����Nat���ݰ��˿�
����ֵ˵��  : TRUE -  �ɹ�
             FALSE -  ʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���          �޸�����
2012/01/06     1.0        ������        ����
====================================================================*/
BOOL32 AddNatRcvPort( u16 wPort);

/*====================================================================
������      : RemoveNatRcvPort
����        : �����Ҫ����Nat���Ķ˿�
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: u16 wPort --[in]����Nat���ݰ��˿�
����ֵ˵��  : TRUE -  �ɹ�
             FALSE -  ʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���          �޸�����
2013/03/07     1.0        ������        ����
====================================================================*/
BOOL32 RemoveNatRcvPort( u16 wPort);

/*====================================================================
// ������:    Remove
// ����:      ɾ��ָ���豸��NatData
// �㷨ʵ��:  ��Ϊ����TNatData::m_strID�������������������IP��ȷƥ�䣬���dwIPΪ0����ȥƥ��
// ����Ȩ��:  public 
// ����˵��:  const string & strDevId--�豸ID����Uri
// ����˵��:  u32 dwIP
// ����ֵ:    void
//-----------------------------------------------------------
// �޸ļ�¼:
// ��      ��     �޸���        �߶���          �޸ļ�¼
// 2013/03/07     huangzhichun huangzhichun    ����
====================================================================*/
void RemoveNatData(const string& strDevId, u32 dwIP);
void RemoveAllNatData();

//******************************************************************************
// ������:    FindNatData
// ����:      
// �㷨ʵ��:  
// ����Ȩ��:  public 
// ����˵��:  const string & strDevId --�豸ID����Uri
// ����˵��:  u16 wSrcPort
// ����ֵ:    const TNatData*
//-----------------------------------------------------------
// �޸ļ�¼:
// ��      ��     �޸���        �߶���          �޸ļ�¼
// 2013/04/09     huangzhichun huangzhichun    ����
//******************************************************************************
const TNatData* FindNatData(const string& strDevId, u16 wSrcPort, u32 dwSessionId = KDM_NAT_DEFAULT_PU_SESSID );

/*====================================================================
�� �� ����  NatPkgCallback
��    �ܣ�  ʹ��̽����Ļص�����
�㷨ʵ�֣�
ȫ�ֱ�����
��    ����u32 dwRecvIP,   ̽������յ�ַ��Ϣ
u16 wRecvPort,  ̽������յ�ַ��Ϣ
u32 dwSrcIP     ̽���Դ��ַ��Ϣ
u16 wSrcPort    ̽���Դ��ַ��Ϣ
u8* pData       ̽�����Ϣ����
u32 uLen        ̽�����Ϣ����
�� �� ֵ�� 0, �����ݰ���Ч�� ����ֵ�������ݰ���Ч��
==============================================================*/
//typedef u32 (*VtduNatPkgCB)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 uLen);

/*====================================================================
������      : UpdateStun
����        : ����Stun��ַ
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: const TNetAddr& tStunAddr --[in]������Ϣ

����ֵ˵��  : CMS_SUCCESS - �ɹ�
             ����ֵ - ʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
����        �汾       �޸���       �޸�����
2013/05/17  1.0       ������        ����
====================================================================*/
BOOL32 UpdateStun(const TNetAddr& tStunAddr);

/*====================================================================
������      : StartPunch
����        : ��ʼPunch
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: const TSwitchChannel& tSwitchChn --[in]������Ϣ

����ֵ˵��  : CMS_SUCCESS - �ɹ�
             ����ֵ - ʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
����        �汾       �޸���       �޸�����
2013/02/01  1.0        ������        ����
====================================================================*/
CMS_RESULT StartPunch(const TVtduPunchChnList& tVtduPunchChnList, const TIpAddr& tLocalIp, u32 dwContext, u32 *pdwSeqNum);

/*====================================================================
������      : AddSwitch
����        : ��ӽ���
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: const TSwitchChannel& tSwitchChn --[in]������Ϣ

����ֵ˵��  : CMS_SUCCESS - �ɹ�
             ����ֵ - ʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
����        �汾       �޸���       �޸�����
2012/01/06  1.0        ������        ����
====================================================================*/
CMS_RESULT AddSwitch( const TSwitchChannel& tSwitchChn);

/*====================================================================
������      : RemoveSwitch
����        : ɾ������
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: const TSwitchSsnID& tSwitchSsnID --[in]���������

����ֵ˵��  : CMS_RESULT -- CMS_SUCCESS��Ϊ�ɹ�������ֵ��Ϊʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
����        �汾       �޸���       �޸�����
2012/01/06  1.0        ������        ����
====================================================================*/
CMS_RESULT RemoveSwitch( const TSwitchSsnID& tSwitchSsnID);

/*====================================================================
������      : VtduLibQuit
����        : �˳�VTDU
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: ��
����ֵ˵��  : ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���          �޸�����
2012/01/06     1.0        ������        ����
====================================================================*/
void VtduLibQuit();

/*====================================================================
������      : GetVtduCfg
����        : ��ȡvtduģ���������Ϣ
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: ��

����ֵ˵��  : TVtduCfg
----------------------------------------------------------------------
�޸ļ�¼    :
����        �汾       �޸���       �޸�����
2012/04/18  1.0        ������        ����
====================================================================*/
TVtduCfg GetVtduCfg();

extern TVtduLibConf g_tVtduLibConf;


/*=============================��������(��ʼ)===============================*/
//---------NAT̽�������
API void vtdunatid(LPCSTR pszDevId);
API void vtdunatlist();

//---------Filter�������
API void pflist(u32 dwPort = 0);
API void vtduportfilterlist(u32 dwPort = 0);

//---------�����������
API void vtduloadcfg();

//---------��־�����������
//��ʾlog������Ϣ
API void vtdulogpara();
//��������ģ��Ĵ�ӡ����
API void vtdulogall(u8 byLev);
//�ر����д�ӡ���൱��logall(0)
API void vtdulogoff();

//---------����������� 
//��ӡ��������
API void vtdutasklist(LPCSTR szTaskType);
//��ӡָ��������Ϣ
API void vtdutask(u32 dwTaskNO);
//ɱ��ָ����ʬ�������ڹ��ϻָ����߲��ԣ�����ʹ�ö��ĵ����Ʊ����û��������
API void vtduIWillKillZombieTask(u32 dwTaskNO);
//��������Ŵ�ӡָ�������������
API void vtdupnst(u32 dwTaskNO);
//��������Ŵ�ӡָ������������񣬵�ͬ��pnst
API void vtdutaskson(u32 dwTaskNO);
//����sesskon�Ŵ�ӡָ�������������
API void vtdupsst(LPCSTR szSession);
//trace ָ������
API void vtdutracet(u32 dwTaskNO);
//trace ��һ���½�������
API void vtdutracent();

//��ӡredissdk��Ϣ
API void redissdk();
/*=============================��������(����)===============================*/


#endif     //#ifndef _VTDULIB_H_

/////////////////////////////END OF THIS FILE///////////////////////////////
