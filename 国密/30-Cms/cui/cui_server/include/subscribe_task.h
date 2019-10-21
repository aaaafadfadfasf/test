/*****************************************************************************
   ģ����      : cui_server
   �ļ���      : subscribe_task.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ʵ��ģ���б���. ����ģ�����ģʽʵ��
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2014/05/28  1.0         huzhiyun        ����
******************************************************************************/
#ifndef _SUBSCRIBE_TASK_H_
#define _SUBSCRIBE_TASK_H_
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include <map>
using std::map;
#include <set>
using std::set;
#include <list>
using std::list;
#include "cms/cms_proto.h"
class CCuAgent;

struct TSubscribeKey
{
    string strDevUri;
    string strSubType;

    TSubscribeKey(){}
    TSubscribeKey(const string &strDevUri_, const string &strSubType_)
        :strDevUri(strDevUri_), strSubType(strSubType_)
    {

    }

    bool operator<(const TSubscribeKey &rh) const
    {
        return (strDevUri<rh.strDevUri ||
            (strDevUri == rh.strDevUri && strSubType < rh.strSubType));
    }
};

struct TSubscribeData
{
    string strNtf;
    set<CCuAgent*> setUserlist;
};

struct TRefreshSubReq
{
    TSubscribeKey tKey;
    bool bAdd;


    TRefreshSubReq(const TRefreshSubReq &rh)
    :tKey(rh.tKey), bAdd(rh.bAdd){}

    TRefreshSubReq(const TSubscribeKey &tKey_, bool bAdd_)
        :tKey(tKey_), bAdd(bAdd_){}
};

class CModuleListSubscribeTaskBase: public CSipTask
{
public:
    enum
    {
        enSubscribe = UNKNOWN_STATE + 1,
        enRefreshSub,
        enDisconnect
    };
    CModuleListSubscribeTaskBase(CInstExt *pcInst, const string &strModuleUri, const string &strSession);
    void ConstructorData(u16 wSubscribeRspEvent, u16 wRefreshRspEvent, vector<u16> vecSupportedNtfReqEvent, vector<string> vecSupportedSubType);
    ~CModuleListSubscribeTaskBase();
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
    virtual void PrintData() const;

    /* ������������ʵ�ֶ���ʱ�����¶��� */
    void ModuleDisconnect();
    void ModuleReconnect(const string &strSession);

	s32 Refresh(const TSubscribeKey &tKey, CCuAgent *pcUser, bool bAdd);
	s32 RedisRefresh(const TSubscribeKey &tKey, CCuAgent *pcUser, bool bAdd);
    void EraseKeyByDevUri(const string &strDevUri);
    void EraseKeyByDevDomain(const string &strDomainName);
    void EraseUser(CCuAgent *pcUser);
	const TSubscribeData* GetSubDataBySubKey(const TSubscribeKey& tSubscribeKey) const;
    void DumpByDevUri(const string &strDevUri) const;
	void DumpByUser(const CCuAgent *pcUser) const;

	bool IsRedisSubscribe(const string &strSubType);
	bool DoRedisSubscribe(const TSubscribeKey &tKey, bool bAdd);

	// �������ĵ�����
	void DoSubscribe();
private:
    string m_strModuleUri;
    string m_strModuleSession;
    TKDSIP_DIALOG_ID m_tSubscribeID;
    u16 m_wSubscribeRspEvent;
    u16 m_wRefreshRspEvent;
    vector<u16> m_vecSupportedNtfReqEvent;
    vector<string> m_vecSupportedSubType;

    map<TSubscribeKey, TSubscribeData> m_mapSubscribe;  // �豸��Ӧ���û�
    map<CCuAgent*, set<TSubscribeKey> > m_mapUser2Keys; // �û����ĵ��豸

    list<TRefreshSubReq> m_lstRefreshSubReq; // �ȴ�ˢ�µ�������Ŀ
    u32 m_dwRefreshingReqNum;                // ����ˢ�µ�������Ŀ

	string m_strNtf;                         // �ȴ������֪ͨ��C***NtyReq.ToXml()�Ľ�� 

public:
	void SetNtfString(const string& var){m_strNtf = var;}
	string& GetNtyString(){return m_strNtf;}
	const string& GetNtyString() const{return m_strNtf;}

private:
    u32 SubscribeProc(CMessage *const pcMsg);
    u32 SubscribeTimerPoll();
    u32 RefreshSubProc(CMessage *const pcMsg);
    u32 RefreshSubTimerPoll();
    u32 DisconnectProc(CMessage *const pcMsg);
    u32 DisconnectTimerPoll();

    void DoRefreshSub();

    virtual void GetSubscribeReqXml(const string &strSession, const string &strUser, string &strResult) const = 0;
    virtual void GetRefreshSubReqXml(const string &strSession, const TSsDevList &tDevLst, const TSsOpType &tOprType, string &strResult) const = 0;
    virtual s32 GetSubscribeRspResult(CMessage *const pcMsg) const = 0;
    // ����ֵΪˢ�¶��ĵ�rsp������, vecFailDevUriΪ���ܺ��Ե�ʧ���豸uri�б�
    virtual s32 GetRefreshRspResult(CMessage *const pcMsg, vector<string> vecFailDevUri) const = 0; 
	// redis֪ͨ�����л��棬��Ҫ��ʱ��ֱ�Ӵ�redis��ȡ���յ�������֪ͨ�󣬷������˾Ͷ�����
    virtual void HandleNtf4Input(CMessage *const pcMsg, TSubscribeKey &tKey, string &strNtf) = 0;
    virtual void HandleNtf4Output(const string &strSubType, const TSubscribeData &tData, const string &strNtf) = 0;
    virtual void HandleNtf4InputAfterOutput(const string &strSubType, TSubscribeData &tData, string &strNtf){}
	void GetNtfByKeyFromRedis(const TSubscribeKey &tKey, string &strNtf);

    void EraseUser2KeyMap(CCuAgent* pcUser, const TSubscribeKey &tKey);
	void EraseSubscribeMap(const TSubscribeKey &tKey, CCuAgent* pcUser);
	void EraseRedisSubscribeMap(const TSubscribeKey &tKey, CCuAgent* pcUser);

protected:
    bool GetSubscribeNtf(const TSubscribeKey &tKey, string &strNtf);
    bool SetSubscribeNtf(const TSubscribeKey &tKey, const string &strNtf);
};

class CCmuListSubscribeTask: public CModuleListSubscribeTaskBase
{
public:
    CCmuListSubscribeTask(CInstExt *pcInst, const string &strModuleUri, const string &strSession);

    virtual const char* GetObjName() const
    {
        return "CCmuListSubscribeTask";
    }

    virtual void GetSubscribeReqXml(const string &strSession, const string &strUser, string &strResult) const;
    virtual void GetRefreshSubReqXml(const string &strSession, const TSsDevList &tDevLst, const TSsOpType &tOprType, string &strResult) const;
    virtual s32 GetSubscribeRspResult(CMessage *const pcMsg) const;
    virtual s32 GetRefreshRspResult(CMessage *const pcMsg, vector<string> vecFailDevUri) const;
    virtual void HandleNtf4Input(CMessage *const pcMsg, TSubscribeKey &tKey, string &strNtf);
    virtual void HandleNtf4Output(const string &strSubType, const TSubscribeData &tData, const string &strNtf);
    virtual void HandleNtf4InputAfterOutput(const string &strSubType, TSubscribeData &tData, string &strNtf);

public:
	void SetStateRefreshSub();
};

class CRcsListSubscribeTask: public CModuleListSubscribeTaskBase
{
public:
    CRcsListSubscribeTask(CInstExt *pcInst, const string &strModuleUri, const string &strSession);

    virtual const char* GetObjName() const
    {
        return "CRcsListSubscribeTask";
    }

    virtual void GetSubscribeReqXml(const string &strSession, const string &strUser, string &strResult) const;
    virtual void GetRefreshSubReqXml(const string &strSession, const TSsDevList &tDevLst, const TSsOpType &tOprType, string &strResult) const;
    virtual s32 GetSubscribeRspResult(CMessage *const pcMsg) const;
    virtual s32 GetRefreshRspResult(CMessage *const pcMsg, vector<string> vecFailDevUri) const;
    virtual void HandleNtf4Input(CMessage *const pcMsg, TSubscribeKey &tKey, string &strNtf);
    virtual void HandleNtf4Output(const string &strSubType, const TSubscribeData &tData, const string &strNtf);
};

class CMpsListSubscribeTask: public CModuleListSubscribeTaskBase
{
public:
    CMpsListSubscribeTask(CInstExt *pcInst, const string &strModuleUri, const string &strSession);

    virtual const char* GetObjName() const
    {
        return "CMpsListSubscribeTask";
    }

    virtual void GetSubscribeReqXml(const string &strSession, const string &strUser, string &strResult) const;
    virtual void GetRefreshSubReqXml(const string &strSession, const TSsDevList &tDevLst, const TSsOpType &tOprType, string &strResult) const;
    virtual s32 GetSubscribeRspResult(CMessage *const pcMsg) const;
    virtual s32 GetRefreshRspResult(CMessage *const pcMsg, vector<string> vecFailDevUri) const;
    virtual void HandleNtf4Input(CMessage *const pcMsg, TSubscribeKey &tKey, string &strNtf);
    virtual void HandleNtf4Output(const string &strSubType, const TSubscribeData &tData, const string &strNtf);
};

s32 ModuleListSubscribe(const string &strModuleUri, const string &strDevUri, const string &strSubType, CCuAgent *pcUser);
s32 ModuleListUnsubscribe(const string &strModuleUri, const string &strDevUri, const string &strSubType, CCuAgent *pcUser);
void EraseSubscribeKeyByDevUri(const string &strDevUri);
void DeleteSubscribeKeyByDevDomain(const string &strDomainName);
void DumpModuleListSubscribeByDevUri(const string &strDevUri);
void DumpModuleListSubscribeByUser(const CCuAgent *pcUser);
void NotifyCuPuOffline(const string &strDevUri);

#endif // _SUBSCRIBE_TASK_H_
