/*========================================================================================
模块名    ：cmu
文件名    ：domaincatalogssertask.h
相关文件  ：
实现功能  ：域间目录订阅者事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2013/12/10         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _DOMAIN_CATALOG_SSER_TASK_H
#define _DOMAIN_CATALOG_SSER_TASK_H


#include "domainssbasetasktmpl.h"
#include "cms/cms_proto.h"

struct TDevCacheData
{
    TDevCacheData()
    {
        time = 0;
        puconfig  = false;
        puservice = false;
        pualarm   = false;
    }

    CRedisDevData data;         // 保存具体的缓存数据
    u32           time;         // 保存加入缓存时的时间

    bool          puconfig;     // data中的puconfig是否有效
    bool          puservice;    // data中的puservice是否有效
    bool          pualarm;      // data中的pualarm是否有效
};

class CDomainCatalogSserTask : public CDomainSsBaseTaskTmpl<string>
{
public:
	CDomainCatalogSserTask(CInstExt *pcInst, CCmuConnecterTask * const pcConnectTask);
	virtual ~CDomainCatalogSserTask();

public:
	virtual const char* GetObjName() const
	{
		return "CDomainCatalogSserTask";
	}

	virtual void PrintData() const;

public:
	void ReleaseResource();
    void TUnitList2TSsList(const set<CSsUnit<string> > &tUnitList, TSsTypes &tSsTypes);
	void DealSsOpRsp(const TSsOpInfo<string> &tOpInfo, set<CSsUnit<string> > &cErrorList);

public:
	// 实现父类中定义的纯虚函数
    virtual TSsMode GetSsMode() const { return CATALOG_SS; }
	virtual string  GetSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo);
	virtual string  GetRefreshSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo);
    virtual void    ProcSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList);
    virtual void    ProcRefreshSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList);
    virtual void    ProcNotifyDevNotExist(const TSsType &tSsType, const CEventReq *pcNtfReq);

public:
    void PopCache(const string &strDevId);

private:
    // 跨域订阅过程中，可能收到通知但是发现该设备不存在（cmu之间的通知和3as的设备变更通知之间的时序问题导致），需要先缓存，在收到3as的通知后再处理
    map<string, TDevCacheData> m_cDevCacheData;         // 保存缓存的具体数据（相当于具体的数据）
    u32 m_dwDrop;                                       // 缓存到期后删除的数量
    u32 m_dwTop;                                        // 缓存的峰值
    u32 m_dwTotal;                                      // 缓存的总数量
    u32 m_dwSize;                                       // 当前缓存的数量
    static const u32 m_sMaxCacheNum  = 1024;            // 最大缓存数量
    static const u16 m_sMaxCacheTime = 30;              // 最大缓存时间（当缓存数量达到上限时，超过最大缓存时间的将被删除）
};

#endif  //#ifndef _DOMAIN_CATALOG_SSER_TASK_H
