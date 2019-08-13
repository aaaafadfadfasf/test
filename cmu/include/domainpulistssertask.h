/*========================================================================================
ģ����    ��cmu
�ļ���    ��domainpulistssertask.h
����ļ�  ��
ʵ�ֹ���  ������豸�б���������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2013/12/12         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _DOMAIN_PULIST_SSER_TASK_H
#define _DOMAIN_PULIST_SSER_TASK_H

#include <string>
using std::string;

#include "domainssbasetasktmpl.h"
#include "cms/cms_proto.h"


class CDomainPuListSserTask : public CDomainSsBaseTaskTmpl<string>
{
public:
	CDomainPuListSserTask(CInstExt *pcInst, CCmuConnecterTask * const pcConnectTask);
	virtual ~CDomainPuListSserTask();

public:
	virtual const char* GetObjName() const
	{
		return "CDomainPuListSserTask";
	}

    virtual void PrintData() const;

protected:
    void ReleaseResource();
	void TUnitList2TSsList(const set<CSsUnit<string> > &tUnitList, map<string, TSsTypes> &tSsList);
	void DealSsOpRsp(const TSsOpInfo<string> &tOpInfo, const TSsErrorList &tErrorList, set<CSsUnit<string> > &cErrorList);

	// ʵ�ָ����ж���Ĵ��麯��
    virtual TSsMode GetSsMode() const { return LIST_SS; }
	virtual string  GetSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo);
	virtual string  GetRefreshSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo);
	virtual void    ProcSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList);
	virtual void    ProcRefreshSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList);
	
// 
// private:
// 
// 	string m_strDomainId;
};

#endif  //#ifndef _DOMAIN_PULIST_SSER_TASK_H