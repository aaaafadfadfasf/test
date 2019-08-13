/*****************************************************************************
   ģ����      : vtdu 
   �ļ���      : ssmng.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ��������������
   ����        : xiongxh
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��         �汾        �޸���      �޸�����
   2017/08/10     1.0         xiongxh     ����
******************************************************************************/
#ifndef _REDIS_SS_MNG_H_
#define _REDIS_SS_MNG_H_

#include <map>
#include <set>
#include <sstream>
using namespace std;

#include "kdvtype.h"

class CRedisSdk;
class CRedisSsChannel;

class CRedisSsMng
{
public:
    static CRedisSsMng *GetInstance();

private:
    CRedisSsMng(CRedisSdk *pcRedisSdk);

public:
    void AddSs(const CRedisSsChannel &cSsChn, u32 dwTaskNO);
    void DelSs(const CRedisSsChannel &cSsChn, u32 dwTaskNO);
    void ClearSs(const CRedisSsChannel &cSsChn);
    bool IsSsChnExist(const CRedisSsChannel &cSsChn) const;
    void GetSsers(const CRedisSsChannel &cSsChn, set<u32> &cTasks);
    void PrintData(const CRedisSsChannel &cSsChn) const;
    void PrintData(bool bVerbose = false) const;

private:
    CRedisSdk *m_pcRedisSdk;
    map<CRedisSsChannel, set<u32> > m_cSsList;

    static CRedisSsMng *m_pcInstance;
};


#endif

