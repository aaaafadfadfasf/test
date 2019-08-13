/*========================================================================================
ģ����    ��cmu
�ļ���    ��domaincatalogssertask.h
����ļ�  ��
ʵ�ֹ���  �����Ŀ¼����������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2013/12/10         V1.0              fanxg                                  �½��ļ�
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

    CRedisDevData data;         // �������Ļ�������
    u32           time;         // ������뻺��ʱ��ʱ��

    bool          puconfig;     // data�е�puconfig�Ƿ���Ч
    bool          puservice;    // data�е�puservice�Ƿ���Ч
    bool          pualarm;      // data�е�pualarm�Ƿ���Ч
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
	// ʵ�ָ����ж���Ĵ��麯��
    virtual TSsMode GetSsMode() const { return CATALOG_SS; }
	virtual string  GetSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo);
	virtual string  GetRefreshSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo);
    virtual void    ProcSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList);
    virtual void    ProcRefreshSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList);
    virtual void    ProcNotifyDevNotExist(const TSsType &tSsType, const CEventReq *pcNtfReq);

public:
    void PopCache(const string &strDevId);

private:
    // �����Ĺ����У������յ�֪ͨ���Ƿ��ָ��豸�����ڣ�cmu֮���֪ͨ��3as���豸���֪֮ͨ���ʱ�����⵼�£�����Ҫ�Ȼ��棬���յ�3as��֪ͨ���ٴ���
    map<string, TDevCacheData> m_cDevCacheData;         // ���滺��ľ������ݣ��൱�ھ�������ݣ�
    u32 m_dwDrop;                                       // ���浽�ں�ɾ��������
    u32 m_dwTop;                                        // ����ķ�ֵ
    u32 m_dwTotal;                                      // �����������
    u32 m_dwSize;                                       // ��ǰ���������
    static const u32 m_sMaxCacheNum  = 1024;            // ��󻺴�����
    static const u16 m_sMaxCacheTime = 30;              // ��󻺴�ʱ�䣨�����������ﵽ����ʱ��������󻺴�ʱ��Ľ���ɾ����
};

#endif  //#ifndef _DOMAIN_CATALOG_SSER_TASK_H
