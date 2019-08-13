/*****************************************************************************
   模块名      : vtdu 
   文件名      : ssmng.h
   相关文件    : 
   文件实现功能: 用来管理订阅事务
   作者        : xiongxh
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期         版本        修改人      修改内容
   2017/08/10     1.0         xiongxh     创建
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

