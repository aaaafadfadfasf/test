/*****************************************************************************
   ģ����      : CMSSDK
   �ļ���      : cmssdk.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ���ƽ̨2.0���뿪��sdk
   ����        : fanxg
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/09/03  1.0         fanxg        ����
******************************************************************************/

#ifndef __CMS_SDK_H__
#define __CMS_SDK_H__

#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define CMS_API extern "C"			__declspec(dllexport)
#else
#define CMS_API extern "C"
#endif	// _MSC_VER

#ifdef SetPort
#undef SetPort
#endif

class CCmsMsg;
struct TCmsSdkConf;


/*====================================================================
������      : CmsSdk_Init
����        : ��ʼ��cmssdk
�㷨ʵ��    : ͬ���ӿڣ�ȫ��ֻ�ܵ���һ��
�������˵��:
             tCfg: CmsSdk��ʼ������
����ֵ˵��  : 
             �ɹ�:CMS_SDK_SUCCESS��
             ʧ��:������
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/09/03  0.1         fanxg
====================================================================*/
CMS_API int CmsSdk_Init(const TCmsSdkConf& tCfg);
 

/*====================================================================
������      : CmsSdk_Quit
����        : �˳�cmssdk
�㷨ʵ��    : ͬ���ӿڣ�ȫ��ֻ�ܵ���һ��
�������˵��:
             
����ֵ˵��  : 
             �ɹ�:CMS_SDK_SUCCESS��
             ʧ��:������
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/09/03  0.1         fanxg
====================================================================*/
CMS_API int CmsSdk_Quit();


/*====================================================================
������      : CmsSdk_SendMsgToPlat
����        : G������ƽ̨������Ϣ����������������֪ͨ��Ϣ��������Ӧ����Ϣ
�㷨ʵ��    : �첽�ӿ�
�������˵��:
             cMsg������Ϣͷ����Ϣ���װ��CCmsMsg��
             
����ֵ˵��  : 
             �ɹ�:CMS_SDK_SUCCESS��
             ʧ��:������
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/09/03  0.1         fanxg
====================================================================*/
CMS_API int CmsSdk_SendMsgToPlat(const CCmsMsg& cMsg);

/*====================================================================
������      : PCmsEventCallBack
����        : ƽ̨��G���ط��ͻص���Ϣ����������������֪ͨ��Ϣ��������Ӧ����Ϣ
�㷨ʵ��    : �ûص�����Ӧ����Ƴ��첽�ӿڣ����������ײ�ĵ����߳�
����ȫ�ֱ���:
�������˵��:
              cMsg������Ϣͷ����Ϣ���װ��CCmsMsg��
����ֵ˵��  : 
             �ɹ�:CMS_SDK_SUCCESS��
             ʧ��:������
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/09/03  0.1         fanxg
====================================================================*/
typedef int (*PCmsEventCallBack)(const CCmsMsg& cMsg);


//��̬��ӿ��в�ʹ��stl, ����ʵ��һ���򵥵�string��
class CCmsString
{
public:
    CCmsString(const char* pStr=0) 
    { 
        CopyStrToSelf(pStr); 
    }

    CCmsString(const CCmsString & other) 
    { 
        CopyStrToSelf(other.m_pData); 
    }

    ~CCmsString() 
    { 
        FreeStrBuffer(m_pData);
    }

    bool operator==(const CCmsString& other) const 
    { 
        return (strcmp(m_pData, other.m_pData) == 0); 
    }

    bool operator==(const char* pStr) const 
    { 
        return (pStr == 0) ? false : (strcmp(m_pData, pStr) == 0); 
    }

    CCmsString& operator=(const CCmsString& other) 
    { 
        return ((this != &other) ? AssignStrToSelf(other.m_pData) : *this); 
    }

    CCmsString& operator=(const char* pStr) 
    { 
        return AssignStrToSelf(pStr); 
    }

    CCmsString& operator+=(const CCmsString& other) 
    { 
        return AddStrToSelf(other.m_pData); 
    }

    CCmsString& operator+=(const char* pStr)
    { 
        return AddStrToSelf(pStr); 
    }

    CCmsString operator+(const CCmsString &other) const 
    { 
        CCmsString newStr; 
        return((newStr += *this) += other); 
    }

    size_t size() const 
    { 
        return strlen(m_pData); 
    }

    const char* c_str() const
    { 
        return m_pData; 
    }

    void clear()
    {
        AssignStrToSelf("");
    }

    bool  empty() const
    {
        return ((m_pData == 0) || operator==(""));
    }

private:
    void CopyStrToSelf(const char* pStr)
    {
        int nLen = (pStr==0) ? 0 : strlen(pStr);
        pStr = (pStr==0) ? "" : pStr;
        m_pData = AllocStrBuffer(nLen+1);
        strcpy(m_pData, pStr);
    }

    CCmsString& AssignStrToSelf(const char* pStr)
    {
        FreeStrBuffer(m_pData);
        CopyStrToSelf(pStr);
        return *this;
    }

    CCmsString& AddStrToSelf(const char* pStr)
    {
        if (pStr)
        {
            char* pNewBuf = AllocStrBuffer(strlen(m_pData) + strlen(pStr) + 1);
            strcpy(pNewBuf, m_pData);
            strcat(pNewBuf, pStr);
            FreeStrBuffer(m_pData);
            m_pData = pNewBuf;
        }
        return *this;
    }

    char* AllocStrBuffer(size_t nSize)
    {
        return (char*)(malloc(nSize));
    }

    void FreeStrBuffer(void* pStr)
    {
        free(pStr);
    }

private:
    char* m_pData;
};

class CCmsNetAddr
{
private:
    CCmsString ip;            
    int        port;                  

public:
    CCmsNetAddr() 
    {
        Clear();
    }

    void Clear()
    {
        ip.clear();
        port = 0;
    }
public:
    void SetIp(const CCmsString& strIp)
    {
        ip = strIp;
    }
    CCmsString& GetIp()
    {
        return ip;
    }
    const CCmsString& GetIp() const
    {
        return ip;
    }

    void SetPort(int nPort)
    {
        port = nPort;
    }
    int GetPort() const
    {
        return port;
    }
};

// ��������
typedef CCmsString TGateWayType;
#define CMS_DEV_TYPE_GBS    "gbs"
#define CMS_DEV_TYPE_CUI1   "cui1"
#define CMS_DEV_TYPE_GXX    "gxx"

//�豸���֪ͨ����
#define CmsEmpty						0x0000		// �������κα��֪ͨ
#define CmsGeneralDev					0x0002      // ͨ���豸(���������⴦����豸���ͣ��������ǽ�豸)
#define CmsTvwallDev					0x0004      // ����ǽ�豸

#define CmsDevCapIndexName				0x0010		// �豸������������
#define CmsDevGrp						0x0020      // �豸����
#define CmsDevGrp2Dev					0x0040      // �豸�����豸����
#define CmsDomainRelation				0x0080		// ���ϵ
#define CmsCUserInfo				    0x0100		// �û���Ϣ
#define CmsIgnoreGbChildDomainDevChg    0x100000  // ���Թ���������豸���0x0001 0000  

typedef unsigned char TLogLev;
#define LOGLEV_NOLOG_LEV                0
#define LOGLEV_ERROR_LEV                1
#define LOGLEV_WARNING_LEV              2
#define LOGLEV_CRITICAL_LEV             3
#define LOGLEV_EVENT_LEV                4
#define LOGLEV_PROGRESS_LEV             5
#define LOGLEV_TIMER_LEV                6
#define LOGLEV_ALL_LEV                  7
#define LOGLEV_TRIVIAL_LEV              255


//����ṹ���е��ֶ���Ҫ��G���ص������ļ��ж�ȡ
struct TCmsSdkConf
{
    TCmsSdkConf()
    {
        ptCmsEventCB = NULL;
        strLogDir = "./";
        bIsSupportNA = false;
		nInterestChangeNtf = CmsEmpty;

		nMaxRegTime  = 1;
		nMaxMediaTransNum = 2048;
		nMaxSubsDevNum    = 4096;
        nMaxNtfPerSs      = 5;
		nMaxTransNum      = 4096;
		bDisableCheckSum  = false;
		bSpeedPriority    = true;
        bIsLanFileExsit   = false;
		bNeedGrpGbid      = false;
        bIsSupportRedis   = false;

		tRunlogLevel = LOGLEV_NOLOG_LEV;	//Ĭ�Ϲر�������־
		nRunlogFileSize_KB = 1024;			//Ĭ��������־��СΪ1M
		nRunlogFileNum     = 10;			//Ĭ�Ͽ���10��������־
		nErrlogFileSize_KB = 1024;			//Ĭ�ϴ�����־��СΪ1M
		nErrlogFileNum     = 5;				//Ĭ�Ͽ���5��������־
    }
    TGateWayType strGateWayType;    //�����豸����
    CCmsString strGateWayUUID;      //�����豸��Ψһ��ʶ��������32���ֽ�
    CCmsNetAddr tGateWayAddr;       //���ص�ַ
    CCmsString strPlatDomainName;   //���ƽ̨����
    CCmsNetAddr tPlatAddr;          //���ƽ̨��ַ
    CCmsString strLogDir;           //cmssdk��־Ŀ¼
    bool bIsSupportNA;              //�Ƿ�֧��NA
	int nInterestChangeNtf;			//��Ҫ������豸���֪ͨ���ͣ���ֵΪCmsGeneralDev��CmsTvwallDev�ȼ��䰴λ��Ľ������CmsGeneralDev | CmsTvwallDev
	bool bNeedGrpGbid;				//�Ƿ���Ҫ�������

	int nMaxRegTime;                //��ע�������ע�����������������Ĭ��Ϊ1
	int nMaxMediaTransNum;          //���ý��ת��·������������/����/����·����Ĭ��2048
	int nMaxSubsDevNum;             //������豸��Ϣ������Ĭ��4096
    int nMaxNtfPerSs;               //���ĶԻ������ntf���д�С��Ĭ��5
	int nMaxTransNum;               //��󲢷���������Ĭ��4096
	bool bDisableCheckSum;          //����У��ͣ�Ĭ��Ϊfalse��·����������ʱ����Ϊtrue
    bool bSpeedPriority;            //�ٶ����ȣ�Ĭ��true(����)
    bool bIsLanFileExsit;
    bool bIsSupportRedis;

	TLogLev tRunlogLevel;			//������־�ļ���
	int nRunlogFileSize_KB;		//����������־���ļ���С����λΪKB
	int nRunlogFileNum;			//������־���������ﵽ���޺󸲸��������־��
	int nErrlogFileSize_KB;		//����������־���ļ���С����λΪKB
	int nErrlogFileNum;			//������־���������ﵽ���޺󸲸��������־��

    PCmsEventCallBack ptCmsEventCB; //�¼��ص�
};

#define  CMS_SDK_INVALID_TASK_NO      (0)
#define  CMS_SDK_INVALID_EVENT        (0)

struct TCmsMsgHead
{
public:
    TCmsMsgHead()
    {
        Clear();
    }

    ~TCmsMsgHead()
    {
        Clear();
    }

    void Clear()
    {      
        m_wEventId = CMS_SDK_INVALID_EVENT;
        m_dwSdkTask = CMS_SDK_INVALID_TASK_NO;
        m_dwGwTask = CMS_SDK_INVALID_TASK_NO;
    }

public:
    
    unsigned short m_wEventId;    //�¼�id
    unsigned long  m_dwGwTask;    //gw��taskNO
    unsigned long  m_dwSdkTask;   //sdk��taskNO
};


/*
  CCmsMsgע�����
  1. �ڲ�ͬ�̼߳䴫�ݸ���Ϣʱʹ��ָ�룬����������Ϣ�Ƕ�̬�ڴ���䡣
  2. new/delete�ڸ��Ե�ģ������н��С�
*/
class CCmsMsg
{
public:
    CCmsMsg() {}

public:

    TCmsMsgHead& GetMsgHead()
    {
        return m_tMsgHeader;
    }
    const TCmsMsgHead& GetMsgHead() const
    {
        return m_tMsgHeader;
    }
    void SetMsgHead(const TCmsMsgHead& tMsgHeader)
    {
        m_tMsgHeader = tMsgHeader;
    }

    CCmsString& GetMsgBody()
    {
        return m_strMsgBody;
    }
    const CCmsString& GetMsgBody() const
    {
        return m_strMsgBody;
    }
    void SetMsgBody(const CCmsString& strMsgBody)
    {
        m_strMsgBody = strMsgBody;
    }
    void ClearMsgBody(void)
    {
        m_strMsgBody.clear();
    }
    size_t GetMsgBodySize() const
    {
        return m_strMsgBody.size();
    }

    unsigned short GetEventID() const
    {
        return m_tMsgHeader.m_wEventId;
    }
    void SetEventID(unsigned short wEventID)
    {
        m_tMsgHeader.m_wEventId = wEventID;
    }

    unsigned long GetSdkTask() const
    {
        return m_tMsgHeader.m_dwSdkTask;
    }
    void SetSdkTask(unsigned long dwTaskNO)
    {
        m_tMsgHeader.m_dwSdkTask = dwTaskNO;
    }

    unsigned long GetGwTask() const
    {
        return m_tMsgHeader.m_dwGwTask;
    }
    void SetGwTask(unsigned long dwTaskNO)
    {
        m_tMsgHeader.m_dwGwTask = dwTaskNO;
    }

    template<class CMsg>
    void GetMsgBody(CMsg& rMsg) const
    {
        rMsg.FromXml(m_strMsgBody.c_str());
    }

protected:
    TCmsMsgHead m_tMsgHeader;
    CCmsString m_strMsgBody;
};


#endif // __CMS_SDK_H__
