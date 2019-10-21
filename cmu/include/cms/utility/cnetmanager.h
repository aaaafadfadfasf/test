/*****************************************************************************
    模块名 ：PMS
    文件名 ：
    相关文件 ：
    文件实现功能：
    作者        ：杨新
    版本        ：V1.0  Copyright(C) 2007-2008 KDC, All rights reserved.
------------------------------------------------------------------------------
    修改记录:
    日  期		版本		修改人		走读人      修改内容
    11/30/07    1.0         杨新                     创建
******************************************************************************/

#ifndef _CNETMANAGER_H
#define _CNETMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdvtype.h"
#include "kdvencrypt.h"
#include "../../cbb/osp/osp.h"
#include "mischelper.h"


#ifdef _LINUX_
#include <errno.h>
#include <net/if.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <arpa/inet.h>

#include <sys/socket.h>  // socket
#include <sys/times.h>  // time
#include <sys/select.h> 
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#endif
#include "../cms_const.h"

#ifdef WIN32
/* This should work for both 32 and 64 bit userland. */
struct ethtool_cmd {
    u32	cmd;
    u32	supported;	/* Features this interface supports */
    u32	advertising;	/* Features this interface advertises */
    u16	speed;		/* The forced speed, 10Mb, 100Mb, gigabit */
    u8	duplex;		/* Duplex, half or full */
    u8	port;		/* Which connector port */
    u8	phy_address;
    u8	transceiver;	/* Which transceiver to use */
    u8	autoneg;	/* Enable or disable autonegotiation */
    u32	maxtxpkt;	/* Tx pkts before generating tx int */
    u32	maxrxpkt;	/* Rx pkts before generating rx int */
    u32	reserved[4];
};
#endif

#define ETHTOOL_GSET   	        0x00000001 /* Get settings. */
#define SIOCETHTOOL             0x8946

/* The forced speed, 10Mb, 100Mb, gigabit, 2.5Gb, 10GbE. */
#define NIC_SPEED_10		    (u16)10

#define NIC_SPEED_100		    (u16)100

#define NIC_SPEED_1000		    (u16)1000

#define NIC_SPEED_2500	    	(u16)2500

#define NIC_SPEED_10000 		(u16)10000

#define NIC_DUPLEX_HALF             (0x00)

#define NIC_DUPLEX_FULL             (0x01)

#define CAN_NOT_GET_DEV_SETTING 0

//创建socket失败
#define CREATE_SOCKET_ERROR     0

//最大mac地址长度
#define MAX_MAC_LEN             (u8)64
#define REAL_MAC_LEN			(u8)6

#define MAX_IPADDRESS_LEN       (s32)(32+1)

#define  BUFSIZE                (u32)8192 

#ifndef IF_NAMESIZE
#define  IF_NAMESIZE            (u32)16
#endif

#ifndef __min
#define __min(a,b) ((a)<(b)? a:b)
#endif

//NetManager定义
struct NetConfigure
{
public:
	NetConfigure()
	{
		memset(this,0,sizeof(NetConfigure));
	}
    const s8 *GetNicIp()const
    {
        return m_achLocalIp;
    }
    const s8 *GetNicNetMask()const
    {
        return m_achLocalNetMask;
    }
    const s8 *GetNicGateWay()const
    {
        return m_achLocalGateWay;
    }
    void SetNicIp(const s8 *achIp)
    {
        if (NULL == achIp)
        {
            return;
        }
        strncpy(m_achLocalIp, achIp, sizeof(m_achLocalIp));
        m_achLocalIp[sizeof(m_achLocalIp) - 1] = 0;
    }

    void SetNicNetMask(const s8 *achNetMask)
    {
        if (NULL == achNetMask)
        {
            return;
        }
        strncpy(m_achLocalNetMask, achNetMask, sizeof(m_achLocalNetMask));
        m_achLocalNetMask[sizeof(m_achLocalNetMask) - 1] = 0;
    }

    void SetNicGateWay(const s8 *achGateWay)
    {
        if (NULL == achGateWay)
        {
            return;
        }
        strncpy(m_achLocalGateWay, achGateWay, sizeof(m_achLocalGateWay));
        m_achLocalGateWay[sizeof(m_achLocalGateWay) - 1] = 0;
    }

private:
    s8  m_achLocalIp     [MAX_IPADDRESS_LEN];
    s8  m_achLocalNetMask[MAX_IPADDRESS_LEN];
    s8  m_achLocalGateWay[MAX_IPADDRESS_LEN];
};

struct route_info{ 
    in_addr dstAddr; 
    in_addr srcAddr; 
    in_addr gateWay; 
    s8 ifName[IF_NAMESIZE]; 
}; 

inline BOOL32 SetDefaultRoute(const s8 *gateway)
{
    s8 achLine[512] = {0};
    const s8 *pszNetwork = "/etc/sysconfig/network";
    const s8 *pszNetworkTmp = "/etc/sysconfig/network_tmp";
    FILE *ptFile = NULL;
    FILE *ptTmpFile = NULL;
    BOOL32 bRet = FALSE;

    do 
    {
        ptFile = fopen(pszNetwork, "r");
        if (NULL == ptFile)
        {
            break;
        }
        ptTmpFile = fopen(pszNetworkTmp, "w");
        if (NULL == ptTmpFile)
        {
            break;
        }

        bool bGWHandled = false;
        bool bNeedBreak = false;
        while (0 == feof(ptFile))
        {
            if (NULL == fgets(achLine, 512, ptFile))
            {
                if (ferror(ptFile) != 0)
                {// error
                    bNeedBreak = true;
                    break;
                }
                else
                {
                    continue;
                }
            }
            if (NULL != strstr(achLine, "GATEWAY"))
            {
                if (NULL == gateway || 0 == strcmp(gateway, ""))
                {
                    continue;
                }
                else
                {
                    sprintf(achLine, "GATEWAY=%s\n", gateway);
                }

                bGWHandled = true;
            }

            if(-1 == fputs(achLine, ptTmpFile))
            {
                bNeedBreak = true;
                break;
            }
        }
        if (bNeedBreak)
        {
            break;
        }

        if(!bGWHandled && NULL != gateway && 0 != strcmp(gateway, ""))
        {
            sprintf(achLine, "GATEWAY=%s\n", gateway);

            if(-1 == fputs(achLine, ptTmpFile))
            {
                fclose(ptTmpFile);
                return FALSE;
            }
        }

        fclose(ptFile); 
        ptFile = NULL;
        fclose(ptTmpFile); 
        ptTmpFile = NULL;

		remove(pszNetwork);

		if (0 != rename(pszNetworkTmp,pszNetwork))
			break;
        //if (-1 == ExecCmd("mv -f %s %s",pszNetworkTmp, pszNetwork))
        //{
        //    break;
        //}

        bRet = TRUE;
    } while (false);

    if (NULL != ptFile)
    {
        fclose(ptFile);
    }
    if (NULL != ptTmpFile)
    {
        fclose(ptTmpFile);
    }
    return bRet;
}
//设备网络管理
class CNetManager
{
public:
    CNetManager()
    {
        m_wSpeed = 0;
    }
	
    ~CNetManager()
    {
		
    }
    
public:

    /*========================================================================
    函 数 名：GetNicNum
    功    能：获取设备网口数目
    参    数：
    返 回 值：返回得网口数不包含环回地址
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2007/12/04  2800    姜宇晖  创建
    ========================================================================*/
    u8 GetNicNum()
    {
		int nNICs = 0;
#ifdef _LINUX_
		FILE *pFile = popen("ifconfig |grep 'HWaddr'|wc -l", "r");
		if (NULL != pFile)
		{
			fscanf(pFile, "%d", &nNICs);
			pclose(pFile);
		}
#endif 
        return nNICs;
    };

	/*========================================================================
	函 数 名：EthtoolGet
	功    能：获取ethtool设置
	参    数：网口设备名"eth0"/"eth1" ..../"eth16" 现支持到16
	返 回 值：struct ifreq
	--------------------------------------------------------------------------
	修改记录：
	日      期  版本    修改人  修改内容
	2010/06/08	V1R4B3		黄至春	创建
	========================================================================*/
	s32 EthtoolGSet(const s8 *achDevName, ethtool_cmd& tECmd)
    {
        s32  nError = 0;
#ifdef _LINUX_
        ifreq ifr;
        s32 nSocket = 0;
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
			OspPrintf(TRUE, FALSE,"EthtoolGet|Creat socket failed!\n");
            return -1;
        }
        
		/* Setup our control structures. */
		memset(&ifr, 0, sizeof(ifr));
		strcpy(ifr.ifr_name, achDevName);
        tECmd.cmd = ETHTOOL_GSET;
        ifr.ifr_data = (s8 *)&tECmd;
        nError = ioctl(nSocket, SIOCETHTOOL, &ifr);
        
        if (0 == nError) 
        {
			OspPrintf(TRUE, FALSE,"EthtoolGet|ioctl(SIOCETHTOOL) successfully.\n"
			"dev[%s]			cmd[%d]		supported[0x%x]		advertising[%x] \n"
			"speed[%d]			duplex[%d]	port[%d]			phy_address[%d] \n"
			"transceiver[%d]	autoneg[%d] maxtxpkt[%d]		maxrxpkt[%d]\n",
			achDevName, tECmd.cmd, tECmd.supported, tECmd.advertising, 
			tECmd.speed, tECmd.duplex, tECmd.port, tECmd.phy_address, 
			tECmd.transceiver, tECmd.autoneg, tECmd.maxtxpkt, tECmd.maxrxpkt);
        }
        else if (nError != EOPNOTSUPP) 
        {
			OspPrintf(TRUE, FALSE,"EthtoolGet|Cannot get device settings");
        }
        
        //释放socket
        if (nSocket != 0)
        {
            close(nSocket);
        }
#endif 
        return nError;
    };

    /*========================================================================
    函 数 名：GetSpeed
    功    能：获取指定网口带宽
    参    数：网口设备名"eth0"/"eth1" ..../"eth16" 现支持到16
    返 回 值：NIC_SPEED_10/NIC_SPEED_100/NIC_SPEED_1000/NIC_SPEED_2500
              /NIC_SPEED_10000/NIC_SPEED_ERROR
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2007/12/04  2800    姜宇晖  创建
    ========================================================================*/
	u16 GetSpeed(const s8 *achDevName)
    {
        ethtool_cmd tECmd;

		if(0 == EthtoolGSet(achDevName, tECmd))
		{
			if(65535 == tECmd.speed)
			{
				tECmd.speed = 0;
			}
			return tECmd.speed;
		}
		else
		{
			OspPrintf(TRUE, FALSE,"GetSpeed(%s) failed!\n", achDevName);
			return 0;
		}
    };

    /*========================================================================
    函 数 名：GetNicInfo
    功    能：获取设备网口参数
    参    数：网口设备名"eth0"/"eth1" ..../"eth16" 现支持到16
    返 回 值：NetConfigure
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2007/12/04  2800    姜宇晖  创建
    ========================================================================*/
	NetConfigure GetNicInfo(const s8 *achDevName)
    {
        memset(&m_tNetConfigure, 0, sizeof(NetConfigure));
#ifdef _LINUX_
        
        ifreq  chIfreq;
        memset(&chIfreq, 0, sizeof(ifreq));
        sockaddr_in *pAddr = NULL;
        sockaddr_in *pMask = NULL;
		sockaddr_in *pGateway = NULL;
        s32 nSocket = 0;
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
            return m_tNetConfigure;
        } 
        strcpy(chIfreq.ifr_name, achDevName);
        
        if (!(ioctl(nSocket, SIOCGIFADDR, (s8 *)&chIfreq)))
        {
            pAddr = (sockaddr_in *) (&chIfreq.ifr_addr);
            m_tNetConfigure.SetNicIp(inet_ntoa(pAddr->sin_addr));
        }
        if (!(ioctl(nSocket, SIOCGIFNETMASK, (s8 *)&chIfreq)))
        {
            pMask = (sockaddr_in *)(&chIfreq.ifr_netmask);
            m_tNetConfigure.SetNicNetMask(inet_ntoa(pMask->sin_addr));
        }    
//         if (!(ioctl(nSocket, SIOCGIFBRDADDR, (s8 *)&chIfreq)))
//         {
//             pBrdAddr = (sockaddr_in *)(&chIfreq.ifr_broadaddr);
//             m_tNetConfigure.SetNicGateWay(inet_ntoa(pBrdAddr->sin_addr));
//         }
        s8  m_achLocalGateWay[MAX_IPADDRESS_LEN];
        if (GetGateway(m_achLocalGateWay, achDevName))
        {
            m_tNetConfigure.SetNicGateWay(m_achLocalGateWay);
        }
        
        if (nSocket != 0)
        {
            close(nSocket);
        }
#endif    
        
        return m_tNetConfigure;
    };

    /*========================================================================
    函 数 名：NicIsConnet
    功    能：网口是否连接
    参    数：网口设备名"eth0"/"eth1" ..../"eth16" 现支持到16
    返 回 值：TRUE为连接状态 FALSE为断开状态
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2007/12/04  2800    姜宇晖  创建
    ========================================================================*/
	BOOL32 NicIsConnet(const s8 *achDevName)
    {
#ifdef _LINUX_
        ifreq ifr;
        ethtool_value tIsConnect;
        
        memset(&ifr, 0, sizeof(ifr));
        tIsConnect.cmd = ETHTOOL_GLINK;
        
        s32 nSocket = 0;
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
			OspPrintf(TRUE, FALSE,"Create Socket Failed\n");
            return FALSE;
        } 
        strncpy(ifr.ifr_name, achDevName, sizeof(ifr.ifr_name)-1);
        ifr.ifr_data = (char *) &tIsConnect;
        
        if (ioctl(nSocket, SIOCETHTOOL, &ifr) == -1)
        {

            //OspPrintf(TRUE, FALSE,"ETHTOOL_GLINK failed: %s\n", strerror(errno));
            close(nSocket);

            return FALSE;
        }
        
        close(nSocket);
        return (tIsConnect.data ? TRUE : FALSE);
#endif
        return TRUE;
    };

    /*========================================================================
    函 数 名：GetMacAddr
    功    能：获取MAC地址
    参    数：
			achDevName，网卡设备名（如eth0）
			pchTextContent,接受结果的缓存地址
			nBufLen，缓冲区大小
			bIsEncode,是否返回加密MAC地址
			lpszSeparator，MAC地址格式化分隔符（如:）
    返 回 值：TRUE为获取成功 FALSE为获取失败
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2007/12/04  2800    姜宇晖  创建
    ========================================================================*/
	BOOL32 GetMacAddr(const s8 *achDevName, s8 *pchTextContent, s32 nBufLen, BOOL32 bIsEncode=TRUE, const s8 chSepChar=0)
    {
        u8 achTextInContent[MAX_MAC_LEN];
        u8 achTextOutContent[MAX_MAC_LEN];
        memset(achTextInContent, 0, sizeof(achTextInContent));
        memset(achTextOutContent, 0, sizeof(achTextInContent));
		memset(pchTextContent, 0, nBufLen);
        
#ifdef _LINUX_
        ifreq ifr;
        u8 wII = 0;
        s32 nSocket = 0;
        
        /* Open control socket. */
        nSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (nSocket < 0)
        {
            return FALSE;
        } 
        
        strncpy(ifr.ifr_name, achDevName, sizeof(ifr.ifr_name)-1);
        
        if (!(ioctl (nSocket, SIOCGIFHWADDR, (s8 *)&ifr)))
        {
            //将网卡MAC地址取出存放在achTextInContent中
            while ( wII < REAL_MAC_LEN )
            {
                achTextInContent[wII] = ifr.ifr_hwaddr.sa_data[wII];
                wII++;
            }
        }
        else
        {
            close(nSocket);
            return FALSE;
        }
        
		s32 nOrigBufLen = nBufLen;
		if(bIsEncode)
		{
			//数据加密
			int nAesRet;
			nAesRet = KdvAES(AUTH_DEFAULT_AES_KEY , strlen(AUTH_DEFAULT_AES_KEY),
				AUTH_DEFAULT_AES_MODE , DIR_ENCRYPT, AUTH_DEFAULT_AES_INITKEY,
				(u8 *)achTextInContent, MAX_MAC_LEN, (u8 *)achTextOutContent);

			if (nAesRet < 0)
			{
				close(nSocket);
				return FALSE;
			}

			if(0 == chSepChar)
			{
				s32 nMaxLenCpy=__min(nBufLen/2, MAX_MAC_LEN);
				for (wII=0; wII<nMaxLenCpy; wII++)
				{
					snprintf(pchTextContent + wII*2, nBufLen, "%02x", achTextOutContent[wII]);
					nBufLen -=2;
				}
			}
			else
			{
				s32 nMaxLenCpy=__min((nBufLen+1)/3, MAX_MAC_LEN);
				for (wII=0; wII<nMaxLenCpy-1; wII++)
				{
					snprintf(pchTextContent + wII*3, nBufLen, "%02x%c", achTextOutContent[wII], chSepChar);
					nBufLen -=3;
				}
				snprintf(pchTextContent + wII*3, nBufLen, "%02x", achTextOutContent[wII]);
			}
			pchTextContent[nOrigBufLen - 1]= 0;
		}
		else
		{
			if(0 == chSepChar)
			{
				s32 nMaxLenCpy=__min(nBufLen/2, REAL_MAC_LEN);
				for (wII=0; wII<nMaxLenCpy; wII++)
				{
					snprintf(pchTextContent + wII*2, nBufLen, "%02x", achTextInContent[wII]);
					nBufLen -=2;
				}
			}
			else
			{
				s32 nMaxLenCpy=__min((nBufLen+1)/3, REAL_MAC_LEN);
				for (wII=0; wII<nMaxLenCpy-1; wII++)
				{
					snprintf(pchTextContent + wII*3, nBufLen, "%02x%c", achTextInContent[wII], chSepChar);
					nBufLen -=3;
				}
				snprintf(pchTextContent + wII*3, nBufLen, "%02x", achTextInContent[wII]);
			}
			pchTextContent[nOrigBufLen - 1]= 0;
		}

        close(nSocket);
#endif
        
        return TRUE;
    };
    
    /*========================================================================
    函 数 名：SetNicInfo
    功    能：设置网卡参数
    参    数：需要设置的网络参数
    返 回 值：TRUE为设置成功 FALSE为设置失败
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2007/12/04  2800    姜宇晖  创建
    ========================================================================*/
    BOOL32 SetNicInfo(const s8 *achDevname, const s8 *achIp, const s8 *achMask, const s8 *achGateWay)
    {
#ifdef _LINUX_
/*
        char szSysCmd[512];
		if(NULL == achGateWay || 0 == achGateWay[0])
		{
			sprintf(szSysCmd, "netconfig -d %s --ip %s --netmask %s > /dev/null 2>&1",
				achDevname, achIp, achMask);
		}
		else
		{
			sprintf(szSysCmd, "netconfig -d %s --ip %s --netmask %s --gateway %s > /dev/null 2>&1",
				achDevname, achIp, achMask, achGateWay);
		}
        
        u32 dwRet = OspSystem(szSysCmd);
#ifdef _LINUX_
		dwRet = WIFEXITED(dwRet)? WEXITSTATUS(dwRet):-1;
#endif
		OspPrintf(TRUE, FALSE, "CNetManager::SetNicInfo| cmd=[%s], wShellRet=%d\n", szSysCmd, dwRet);
        
        if (0 == dwRet)
        {
            return TRUE;
        }
        return FALSE;
*/

        s8 achCfgFilePath[50] = {0};
        s8 achCfgFilePathNew[50] = {0};
        FILE* pCfgFile;
        FILE* pCfgFileNew;
        s8 achLine[512] = {0};
        s8 achTemp[512] = {0};
        BOOL bFoundIp = FALSE;
        BOOL bFoundMask = FALSE;
        BOOL bFoundGateWay = FALSE;
        
        sprintf(achCfgFilePath, "/etc/sysconfig/network-scripts/ifcfg-%s", achDevname);
        sprintf(achCfgFilePathNew, "%s.new", achCfgFilePath);

        pCfgFile = fopen(achCfgFilePath, "r" );
        if (NULL == pCfgFile)
        {
            return FALSE;
        }
        
        pCfgFileNew = fopen(achCfgFilePathNew, "w");
        if (NULL == pCfgFileNew)
        {
            fclose(pCfgFile);
            return FALSE;
        }
        
        while (0 == feof(pCfgFile))
        {
            if (NULL == fgets(achLine, 512, pCfgFile))
            {
                if (ferror(pCfgFile) != 0)
                {// error
                    fclose(pCfgFile);
                    fclose(pCfgFileNew);
                    return FALSE;
                }
                else
                {
                    continue;
                }
            }
            
            //此行有 IPADDR
            if ('#'  != achLine[0] && NULL != strstr(achLine, "IPADDR") && NULL != achIp)  
            {
                sprintf(achTemp, "IPADDR=%s\n", achIp);
                if (-1 == fputs(achTemp, pCfgFileNew))
                {
                    fclose(pCfgFile);
                    fclose(pCfgFileNew);
                    return FALSE;
                }
                bFoundIp = TRUE;
                continue;
            }
            //此行有 NETMASK
            if ('#'  != achLine[0] && NULL != strstr(achLine, "NETMASK") && NULL != achMask)  
            {
                sprintf(achTemp, "NETMASK=%s\n", achMask);
                if (-1 == fputs(achTemp, pCfgFileNew))
                {
                    fclose(pCfgFile);
                    fclose(pCfgFileNew);
                    return FALSE;
                }
                bFoundMask = TRUE;
                continue;
            }
            //此行有 GATEWAY 不用写入ifcfg文件,写入network文件
            if ('#'  != achLine[0] && NULL != strstr(achLine, "GATEWAY"))  
            {
                continue;
            }

            //其他项原样写入
            if(-1 == fputs(achLine, pCfgFileNew))
            {
                fclose(pCfgFile);
                fclose(pCfgFileNew);
                return FALSE;
            }
        }

        //不存在 IPADDR ，添加一项
        if(!bFoundIp && NULL != achMask)
        {
            sprintf(achTemp, "IPADDR=%s\n", achIp);
            if (-1 == fputs(achTemp, pCfgFileNew))
            {
                fclose(pCfgFile);
                fclose(pCfgFileNew);
                return FALSE;
            }	
        }
        //不存在 NETMASK ，添加一项
        if(!bFoundMask && NULL != achIp)
        {
            sprintf(achTemp, "NETMASK=%s\n", achMask);
            if (-1 == fputs(achTemp, pCfgFileNew))
            {
                fclose(pCfgFile);
                fclose(pCfgFileNew);
                return FALSE;
            }
        }
        
        fclose(pCfgFile);
        fclose(pCfgFileNew);
        
        //替换文件
		remove(achCfgFilePath);

		if ( 0 != rename(achCfgFilePathNew,achCfgFilePath))
		{
			return FALSE;
		}
        //if (-1 == ExecCmd("mv -f %s %s",achCfgFilePathNew, achCfgFilePath))
        //{
        //    return FALSE;
        //}

        if (FALSE == SetDefaultRoute(achGateWay))
        {
            return FALSE;
        }
#endif        
        return TRUE;
    };
    

    s32 ReadNlSock(s32 sockFd, s8 *bufPtr, s32 seqNum, s32 pId)
    {
        s32 msgLen = 0; 
 #ifdef _LINUX_
        struct nlmsghdr *nlHdr; 
        s32 readLen = 0;
        
        do
        { 
            if((readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0)
            { 
                perror("SOCK READ: "); 
                return -1; 
            } 
            
            nlHdr = (struct nlmsghdr *)bufPtr; 
            
            if((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR)) 
            { 
                perror("Error in recieved packet"); 
                return -1; 
            } 
            
            if(nlHdr->nlmsg_type == NLMSG_DONE) 
            { 
                break; 
            } 
            else
            { 
                bufPtr += readLen; 
                msgLen += readLen; 
            } 
            
            if((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) 
            { 
                break; 
            } 
        } while((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId)); 
#endif
        return msgLen; 
    } 
#ifdef _LINUX_    
    /* For parsing the route info returned */ 
    BOOL32 ParseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo,s8 *gateway, s8 *devname) 
    { 
        struct rtmsg *rtMsg; 
        struct rtattr *rtAttr; 
        s32 rtLen; 
        s8 *tempBuf = NULL; 
        
        tempBuf = (s8 *)malloc(100); 
        rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr); 
        
		/* If the route is not for AF_INET or does not belong to main routing table 
		then return. */ 
        if((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN)) 
        {
            free(tempBuf);
            return FALSE;
        }
        
		/* get the rtattr field */ 
        rtAttr = (struct rtattr *)RTM_RTA(rtMsg); 
        rtLen = RTM_PAYLOAD(nlHdr); 
        for(;RTA_OK(rtAttr,rtLen);rtAttr = RTA_NEXT(rtAttr,rtLen))
        { 
            switch(rtAttr->rta_type) 
            { 
            case RTA_OIF:
                if_indextoname(*(s8 *)RTA_DATA(rtAttr), rtInfo->ifName);
                break; 
                
            case RTA_GATEWAY: 
                rtInfo->gateWay = *(in_addr *)RTA_DATA(rtAttr); 
                break; 
            } 
        } 
		// ADDED BY BOB - ALSO COMMENTED prs32Route 
        
        if (NULL != strstr((s8 *)inet_ntoa(rtInfo->dstAddr), "0.0.0.0")) 
        {
            sprintf(gateway,"%s", (s8 *)inet_ntoa(rtInfo->gateWay));
            sprintf(devname, "%s",rtInfo->ifName);
            
            free(tempBuf); 
            return TRUE;
        }
        free(tempBuf); 
        return FALSE; 
    } 
#endif
	    /*========================================================================
    函 数 名：GetGateway
    功    能：获取网卡网关
    参    数：
    返 回 值：TRUE为指定网卡获取网关成功 FALSE为失败
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2007/12/04  2800    姜宇晖  创建
    ========================================================================*/
    BOOL32 GetGateway(s8 *gateway, const s8* devname) 
    {
#ifdef _LINUX_
        gateway[0] = '\0';
        struct nlmsghdr *nlMsg; 
        struct rtmsg *rtMsg; 
        struct route_info *rtInfo; 
        s8 msgBuf[BUFSIZE]; 
        
        s32 sock, len, msgSeq = 0; 
        s8 buff[1024]; 
        
        
        /* Create Socket */ 
        if((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0) 
            perror("Socket Creation: "); 
        
		/* Initialize the buffer */ 
        memset(msgBuf, 0, BUFSIZE); 
        
		/* pos32 the header and the msg structure pos32ers s32o the buffer */ 
        nlMsg = (struct nlmsghdr *)msgBuf; 
        rtMsg = (struct rtmsg *)NLMSG_DATA(nlMsg); 
        
        /* Fill in the nlmsg header*/ 
		nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message. 
		nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table . 
		
		nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump. 
		nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet. 
		nlMsg->nlmsg_pid = getpid(); // PID of process sending the request. 
        
        /* Send the request */ 
        if(send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0)
        { 
            close(sock);
            return FALSE; 
        } 
        
		/* Read the response */ 
        if((len = ReadNlSock(sock, msgBuf, msgSeq, getpid())) < 0) 
        { 
            close(sock);
            return FALSE; 
        } 
		/* Parse and prs32 the response */ 
        rtInfo = (struct route_info *)malloc(sizeof(struct route_info)); 
        
		/* THIS IS THE NETTSTAT -RL code I commented out the prs32ing here and in parse routes */ 
		//fprs32f(stdout, "Destination\tGateway\ts32erface\tSource\n"); 
        s8 achdevname[IF_NAMESIZE] = {0};
        for(;NLMSG_OK(nlMsg,len);nlMsg = NLMSG_NEXT(nlMsg,len))
        { 
            memset(rtInfo, 0, sizeof(struct route_info));
            
            ParseRoutes(nlMsg, rtInfo, gateway, achdevname);
            
        }  
        if (0 == strcmp(achdevname, devname))
        {
            free(rtInfo); 
            close(sock);
            return TRUE;
        }
        
        free(rtInfo); 
        close(sock);
#endif
        return FALSE; 
    } 

	s32 GetNullIPNICs(void)
	{
		s32 nNullIPNICs = 0;
#ifdef _LINUX_
		FILE *pFile = popen("ifconfig |grep -P 'HWaddr.+\\n(?!\\s+inet\\s+addr)' |wc -l", "r");
		if (NULL != pFile)
		{
			fscanf(pFile, "%d", &nNullIPNICs);
			pclose(pFile);
			nNullIPNICs >>= 1; // 除2
		}
#endif // _LINUX_
		return nNullIPNICs;
	}

private:
    //网口网络配置
	NetConfigure m_tNetConfigure;

    //网口带宽
    u16 m_wSpeed;

};



#endif
