/*========================================================================================
ģ����    ��cmustubtest
�ļ���    ��cmustubtestdata.h
����ļ�  ��
ʵ�ֹ���  ��cmu�豸���ݹ���
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_STUB_TEST_DATA_H
#define _CMU_STUB_TEST_DATA_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xset.h"
#include "cms/tas/tas_struct.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/natagent/natagent.h"
#include "cmustubtestconst.h"

#include <string>
using std::string;
#include <set>
using std::set;

string GetUriFromDevId(const string& strDevId);
bool IsLocalDomain(const string& strDevUri);
bool IsLocalDomain(const TSipURI& tDevUri);
const TSipURI& GetLocalUri();
TSipURI GetCmuUriFromDevUri(const string& strDevUri);
TSipURI GetCmuUriFromDevUri(const TSipURI& tDevUri);
TSipURI GetCmuUri();
TSipURI GetAlsUri();
TSipURI GetTvsUri();

#define Kdm_IsValidPort(port) (port!=0 && port!=0xffff)

bool Kdm_IsValidIPStr(const char* lpszIp);

void Cms2Na_NetAddr(const TNetAddr& tNetAddr, TNatAddr& tNatAddr);

bool SamStartPunch(TSdp& tPeerSdp, const TNetAddr& tLocalAddr, void* pvContext);

u64 MakeAddrID( const u32 &dwIP, const u16 &wPort );

u64 GetTimeMsByTicks();

struct TCmuStat
{
    TCmuStat()
    {
        bIsRegSipProxy = false;
        nSsNum = 0;
        nIvtNum = 0;
    }

    void PrintData()
    {
        OspPrintf(TRUE, FALSE, "\n״̬��Ϣ��\n");

        OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
        OspPrintf(TRUE, FALSE, "  ����ʱ��      :[%s]\n", strStartTime.c_str());
        OspPrintf(TRUE, FALSE, "  �Ƿ�ע��PROXY :[%d]\n", bIsRegSipProxy);
        OspPrintf(TRUE, FALSE, "  ע��CMU״̬   :[%s]\n", strRegCmuState.c_str());
        OspPrintf(TRUE, FALSE, "  ������Ŀ      :[%d]\n", nSsNum);
        OspPrintf(TRUE, FALSE, "  �����Ŀ      :[%d]\n", nIvtNum);

        OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
    }

    string strStartTime;
    string strRegCmuState;
    bool   bIsRegSipProxy;
    int    nSsNum;
    int    nIvtNum;
};

class CPuDataList;

class CPuData : public COspObj
{
public:
    friend class CPuDataList;
public:
    CPuData();
    ~CPuData();
    void Clear();

public:
    //���ݲ���
    const string& GetDevUri() const
    {
        return m_tPuConfig.GetDevUri();
    }
    string& GetDevUri()
    {
        return m_tPuConfig.GetDevUri();
    }
    void SetDevUri(const string& strDevUri)
    {
        m_tPuConfig.SetDevUri(strDevUri);
    }

    const TPuStatus& GetPuStatus() const
    {
        return m_tPuOnOff;
    }
    TPuStatus& GetPuStatus()
    {
        return m_tPuOnOff;
    }
    void SetPuStatus(const TPuStatus& tPuStatus)
    {
        m_tPuOnOff = tPuStatus;
    }

    const TPuConfig& GetPuConfig() const
    {
        return m_tPuConfig;
    }
    TPuConfig& GetPuConfig()
    {
        return m_tPuConfig;
    }
    void SetPuConfig(const TPuDevConfig& tPuConfig)
    {
        m_tPuConfig.MergeDevConfig(tPuConfig);
    }

    const TPuServiceStatus& GetPuServiceStatus() const
    {
        return m_tPuServiceStatus;
    }
    TPuServiceStatus& GetPuServiceStatus()
    {
        return m_tPuServiceStatus;
    }
    void SetPuServiceStatus(const TPuServiceStatus& tPuServiceStatus)
    {
        m_tPuServiceStatus = tPuServiceStatus;
    }

    const TPuAlarmStatus& GetPuAlarmStatus() const
    {
        return m_tPuAlarmStatus;
    }
    TPuAlarmStatus& GetPuAlarmStatus()
    {
        return m_tPuAlarmStatus;
    }
    void SetPuAlarmStatus(const TPuAlarmStatus& tPuAlarmStatus)
    {
        m_tPuAlarmStatus = tPuAlarmStatus;
    }

    void AddInvitedNum()
    {
        m_nHasInvitedNum++;
    }

    void SubInvitedNum()
    {
        if (m_nHasInvitedNum > 0)
        {
            m_nHasInvitedNum--;
        }
    }

    s32 GetInvitedNum() const
    {
        return m_nHasInvitedNum;
    }

private:
    TPuConfig m_tPuConfig;               //pu������Ϣ
    TPuStatus m_tPuOnOff;                //pu����״̬
    TPuServiceStatus m_tPuServiceStatus; //puҵ��״̬
    TPuAlarmStatus m_tPuAlarmStatus;     //pu�澯״̬

public:
    TVideoEncodeParam  m_tVideoEncodeParam;         //��Ƶ�������
    TSupportMediaType  m_tSurportedMediaType;  //����֧�ֵ�ý���ʽ
    TPuSysParam        m_tSysParam;  //ϵͳ����

    s32                m_nHasInvitedNum;
};

typedef CStrMap<CPuData*> CPuDataMap;    //keyΪPuUri


class CLoginSession;
class CPuDataList
{
public:
    void AddPuData(CPuData* pPuData);
    void DelPuData(CPuData* pPuData);
    void DelPuData(const string& strUri);
    CPuData* FindPuData(const string& strUri);

    CPuDataMap GetOnLinePuDataList();

    CPuDataMap GetOnLineNoInvitePuDataList();

public:
    void PrintData(u8 byFlag = 0);
    u32 GetOnlinePuNum();
    u32 GetSize();
private:
    CPuDataMap m_cPuList;
};
extern CPuDataList g_cPuList;

extern vector<string> g_pszTestPu;

inline string GetTestPuUri(size_t i=0)
{
    string strLocalDomain = GetLocalUri().GetDomain().c_str();
    //string strTestPuUri = "55010100000000000011200011200000";
    string strTestPuUri = g_pszTestPu[i];//"a7786f080a8a4f38bcd78bbe3d98db55";
    
    strTestPuUri += "@";
    strTestPuUri += strLocalDomain;
    return strTestPuUri;
}



#define TEST_PU_URI GetTestPuUri(0)

struct TInviteReqMsg
{
    TInviteReqMsg()
    {
        memset(this, 0, sizeof(TInviteReqMsg));
    }

    s8 m_szSrcUri[255];
    s32 m_nSrcChn;
    s8 m_szDstUri[255];
    s32 m_nDstChn;
};

extern CStrMap<CTask*>   g_cStubQryTaskList;

struct TTwSchemeId
{
    TTwSchemeId()
    {
        memset(this, 0, sizeof(TTwSchemeId));
    }

    s8 m_szTwId[255];
    s8 m_szSchemeName[255];
    int m_nTvNum;
    int m_nTvDivNum;
    int m_nPollNum;
};

struct TVcrCtrlParam
{
    TVcrCtrlParam()
    {
        memset(this, 0, sizeof(TVcrCtrlParam));
    }

    u32 m_nTaskNO;
    s8  m_szVcrCmd[255];
};

struct TPunchResultParam
{
    TPunchResultParam()
    {
        memset(this, 0, sizeof(TPunchResultParam));
    }

    EmResult        m_emResult;
    u32             m_dwResultNum;
    TPunchResult    m_atResultList[NATAGENT_MAX_PORT_NUM];

    u32 m_dwSeqNum;
    u32 m_dwTaskNO;
};

struct TStunResultParam
{
    TStunResultParam()
    {
        memset(this, 0, sizeof(TStunResultParam));
    }

    EmResult        m_emResult;

    u32                  m_dwResultNum;                      //��ַ����Ŀ
    TStunResult         m_atResultList[NATAGENT_MAX_NETSEG];  //��ַ���б�
    u32 m_dwSeqNum;
    u32 m_dwTaskNO;
};



struct TDumpStat
{
    TDumpStat()
    {
        m_dwTotal = 0;
        m_dwRTPRepeat = 0;
        m_dwRTPLost = 0;
        m_wExpectedSeqNO = 0;
        m_bIsFirstRun = TRUE;
        m_dwSSRC = 0;          
        m_ddwPowerUpTime = 0;  
        m_ddwProcDataTime = 0; 

        m_dwTaskNO = INVALID_TASKNO;
        m_dwMediaType = EDumpMediaBoth;
    }

    bool operator < (const TDumpStat& tOther) const 
    {            
        return m_tDstAddr.GetNetPort() < tOther.m_tDstAddr.GetNetPort();
    }

    u32 m_dwTotal;          //�յ�RTP�ܰ����������ش���
    u32 m_dwRTPRepeat;      //�ش�����
    u32 m_dwRTPLost;        //�����������������İ��ۼ���

    u16                         m_wExpectedSeqNO;
    BOOL32                      m_bIsFirstRun;
    u32                         m_dwSSRC;             //��¼��һ���յ�������SSRC������Ժ�SSRC�仯��������ӡ
    u64                         m_ddwPowerUpTime;     //��¼������ʱ��㣬�Ա�ͳ�Ƶ�һ�հ���ʱ���
    u64                         m_ddwProcDataTime;    //��¼�յ���һ����ʱ���

    u32                         m_dwTaskNO;
    u32                         m_dwMediaType;

    TNetAddr m_tSrcAddr;
    TNetAddr m_tDstAddr;
};
typedef CXMap<u64, TDumpStat> CDumpStatMap;



#endif  //#ifndef _CMU_STUB_TEST_DATA_H