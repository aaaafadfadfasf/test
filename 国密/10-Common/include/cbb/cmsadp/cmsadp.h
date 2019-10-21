#ifndef CMS_ADP_H
#define CMS_ADP_H
#include "kdvtype.h"
#include "string.h"

#ifndef API
#define API extern "C"
#endif

#define  MAX_STR_LEN          (255)
#define  MAX_IPADDR_LEN       (32)
struct TCmsParamInfo
{
private:
	s8	   m_szCmsIp[MAX_IPADDR_LEN];	    //ƽ̨��ַ
	u16    m_wCmsPort;	    //ƽ̨�˿�
	s8     m_szCmsClientId[MAX_STR_LEN]; //��ý��ID
	s8     m_szCmsServerId[MAX_STR_LEN]; //ƽ̨ID
	s8     m_szUsrName[MAX_STR_LEN];     //ƽ̨�û���
	s8     m_szPassWord[MAX_STR_LEN];    //ƽ̨����
	u8     m_byCmsType;     //ƽ̨����
public:
	TCmsParamInfo()
	{
		memset(this, 0, sizeof(TCmsParamInfo));
	}

	s8*  GetCmsIp(void) {return m_szCmsIp;}
	void SetCmsIp(const s8* lpszCmsIp)
	{
		if(lpszCmsIp == NULL) return;        
        strncpy(m_szCmsIp, lpszCmsIp, sizeof(m_szCmsIp) - 1); 
        m_szCmsIp[sizeof(m_szCmsIp) - 1] = 0;
	}
	u16  GetCmsPort() { return m_wCmsPort; }
	void SetCmsPort(u16 dwCmsPort){ m_wCmsPort = dwCmsPort; }
	s8*  GetCmsClientId(void) {return m_szCmsClientId;}
	void SetCmsClientId(const s8* lpszClientId)
	{
		if(lpszClientId == NULL) return;        
        strncpy(m_szCmsClientId, lpszClientId, sizeof(m_szCmsClientId) - 1); 
        m_szCmsClientId[sizeof(m_szCmsClientId) - 1] = 0;
	}
	s8*  GetCmsServerId(void) {return m_szCmsServerId;}
	void SetCmsServerId(const s8* lpszServerId)
	{
		if(lpszServerId == NULL) return;        
        strncpy(m_szCmsServerId, lpszServerId, sizeof(m_szCmsServerId) - 1); 
        m_szCmsServerId[sizeof(m_szCmsServerId) - 1] = 0;
	}
	s8*  GetUsrName(void) {return m_szUsrName;}
	void SetUsrName(const s8* lpszUsrName)
	{
		if(lpszUsrName == NULL) return;        
        strncpy(m_szUsrName, lpszUsrName, sizeof(m_szUsrName) - 1); 
        m_szUsrName[sizeof(m_szUsrName) - 1] = 0;
	}
	s8*  GetPassWord(void) {return m_szPassWord;}
	void SetPassWord(const s8* lpszPassWord)
	{
		if(lpszPassWord == NULL) return;        
        strncpy(m_szPassWord, lpszPassWord, sizeof(m_szPassWord) - 1); 
        m_szPassWord[sizeof(m_szPassWord) - 1] = 0;
	}
	u8   GetCmsType(){ return m_byCmsType; }
	void SetCmsType( u8 byCmsType ){ m_byCmsType = byCmsType; }
};

/*=============================================================================
    �� �� ���� CmsAdpInit
    ��    �ܣ� ��ʼ��
    �㷨ʵ�֣� bIsUpnpΪfalseʱ����ʹ��upnp��CmsAdpSetNatInfo��CmsAdpDelNatInfo���ӿڲ���ʹ��
    ��    ���� lpszCfg    �����ļ�·��,Ĭ����/opt/kdm/mss
	           bIsUpnp    �Ƿ��ʼ��upnp��Ĭ��Ϊtrue
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/11/13  1.0		    ������                  ����
=============================================================================*/
API s32 CmsAdpInit(const s8* lpszCfg = NULL, const BOOL32 bIsUpnp = TRUE);

#ifdef CMS_SUPPORT_UPNP
/*=============================================================================
    �� �� ���� CmsAdpSetNatInfo
    ��    �ܣ� ����nat��Ϣ
    �㷨ʵ�֣� 
    ��    ���� dwLocalIp nat�ڵ�ַ
	           wLocalPort nat�ڶ˿�
			   bTcp	trueΪtcp��falseΪudp
			   dwNatIp nat���ַ
			   wNatPort nat��˿�
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/11/13  1.0		    ������                  ����
=============================================================================*/
API s32 CmsAdpSetNatInfo(u32 dwLocalIp, u16 wLocalPort, BOOL32 bTcp, u32 dwNatIp, u16 wNatPort);

/*=============================================================================
    �� �� ���� CmsAdpDelNatInfo
    ��    �ܣ� ɾ��nat��Ϣ
    �㷨ʵ�֣� 
    ��    ���� dwLocalIp nat�ڵ�ַ
	           wLocalPort nat�ڶ˿�
			   bTcp	trueΪtcp��falseΪudp
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/11/13  1.0		    ������                  ����
=============================================================================*/
API s32 CmsAdpDelNatInfo(u32 dwLocalIp, u16 wLocalPort, BOOL32 bTcp);
#endif
/*=============================================================================
    �� �� ���� CmsAdpSetSysInfo
    ��    �ܣ� ϵͳIP�޸�����
    �㷨ʵ�֣� ��������������������Ϣ������ƽ̨IP(Ĭ��Ϊeth0)
    ��    ���� dwOriginIp ԭIP
			   dwNewIp ��IP
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2014/11/13  1.0		    ������                  ����
=============================================================================*/
API s32 CmsAdpSetSysInfo(u32 dwOriginIp, u32 dwNewIp);

/*=============================================================================
    �� �� ���� CmsAdpSetPlatFormInfo
    ��    �ܣ� ����ƽ̨���������Ϣ������ƽ̨���͡�ƽ̨��ַ��ƽ̨�˿ڡ���ý��ID��ƽ̨ID��ƽ̨�û��������룬�ɹ�������smu
    �㷨ʵ�֣� 
    ��    ���� TCmsParamInfo &tCmsParamInfo
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2015/07/15  1.0		     ����                   ����
=============================================================================*/
API s32 CmsAdpSetPlatFormInfo( TCmsParamInfo &tCmsParamInfo);

/*=============================================================================
    �� �� ���� CmsAdpGetPlatFormInfo
    ��    �ܣ� ��ȡƽ̨���������Ϣ������ƽ̨���͡�ƽ̨��ַ��ƽ̨�˿ڡ���ý��ID��ƽ̨ID��ƽ̨�û���������
    �㷨ʵ�֣� 
    ��    ���� TCmsParamInfo &tCmsParamInfo
    �� �� ֵ�� s32 0-�ɹ� ����0-ʧ��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2015/07/15  1.0		     ����                   ����
=============================================================================*/
API s32 CmsAdpGetPlatFormInfo( TCmsParamInfo &tCmsParamInfo);


#endif
