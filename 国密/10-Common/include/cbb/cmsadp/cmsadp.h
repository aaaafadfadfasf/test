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
	s8	   m_szCmsIp[MAX_IPADDR_LEN];	    //平台地址
	u16    m_wCmsPort;	    //平台端口
	s8     m_szCmsClientId[MAX_STR_LEN]; //流媒体ID
	s8     m_szCmsServerId[MAX_STR_LEN]; //平台ID
	s8     m_szUsrName[MAX_STR_LEN];     //平台用户名
	s8     m_szPassWord[MAX_STR_LEN];    //平台密码
	u8     m_byCmsType;     //平台类型
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
    函 数 名： CmsAdpInit
    功    能： 初始化
    算法实现： bIsUpnp为false时，不使用upnp，CmsAdpSetNatInfo，CmsAdpDelNatInfo两接口不可使用
    参    数： lpszCfg    配置文件路径,默认是/opt/kdm/mss
	           bIsUpnp    是否初始化upnp，默认为true
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/11/13  1.0		    张若玺                  创建
=============================================================================*/
API s32 CmsAdpInit(const s8* lpszCfg = NULL, const BOOL32 bIsUpnp = TRUE);

#ifdef CMS_SUPPORT_UPNP
/*=============================================================================
    函 数 名： CmsAdpSetNatInfo
    功    能： 设置nat信息
    算法实现： 
    参    数： dwLocalIp nat内地址
	           wLocalPort nat内端口
			   bTcp	true为tcp，false为udp
			   dwNatIp nat外地址
			   wNatPort nat外端口
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/11/13  1.0		    张若玺                  创建
=============================================================================*/
API s32 CmsAdpSetNatInfo(u32 dwLocalIp, u16 wLocalPort, BOOL32 bTcp, u32 dwNatIp, u16 wNatPort);

/*=============================================================================
    函 数 名： CmsAdpDelNatInfo
    功    能： 删除nat信息
    算法实现： 
    参    数： dwLocalIp nat内地址
	           wLocalPort nat内端口
			   bTcp	true为tcp，false为udp
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/11/13  1.0		    张若玺                  创建
=============================================================================*/
API s32 CmsAdpDelNatInfo(u32 dwLocalIp, u16 wLocalPort, BOOL32 bTcp);
#endif
/*=============================================================================
    函 数 名： CmsAdpSetSysInfo
    功    能： 系统IP修改设置
    算法实现： 根据网卡名更新网段信息和连接平台IP(默认为eth0)
    参    数： dwOriginIp 原IP
			   dwNewIp 新IP
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2014/11/13  1.0		    张若玺                  创建
=============================================================================*/
API s32 CmsAdpSetSysInfo(u32 dwOriginIp, u32 dwNewIp);

/*=============================================================================
    函 数 名： CmsAdpSetPlatFormInfo
    功    能： 设置平台相关配置信息：包括平台类型、平台地址、平台端口、流媒体ID、平台ID、平台用户名和密码，成功后重启smu
    算法实现： 
    参    数： TCmsParamInfo &tCmsParamInfo
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2015/07/15  1.0		     王军                   创建
=============================================================================*/
API s32 CmsAdpSetPlatFormInfo( TCmsParamInfo &tCmsParamInfo);

/*=============================================================================
    函 数 名： CmsAdpGetPlatFormInfo
    功    能： 获取平台相关配置信息：包括平台类型、平台地址、平台端口、流媒体ID、平台ID、平台用户名和密码
    算法实现： 
    参    数： TCmsParamInfo &tCmsParamInfo
    返 回 值： s32 0-成功 ，非0-失败
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2015/07/15  1.0		     王军                   创建
=============================================================================*/
API s32 CmsAdpGetPlatFormInfo( TCmsParamInfo &tCmsParamInfo);


#endif
