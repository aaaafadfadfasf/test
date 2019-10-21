/*****************************************************************************
   模块名      : cui_server
   文件名      : subscribe_task.h
   相关文件    : 
   文件实现功能: 实现模块列表订阅. 采用模板设计模式实现
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2014/05/28  1.0         huzhiyun        创建
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

    /* 下面两个函数实现断链时的重新订阅 */
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

	// 创建订阅的连接
	void DoSubscribe();
private:
    string m_strModuleUri;
    string m_strModuleSession;
    TKDSIP_DIALOG_ID m_tSubscribeID;
    u16 m_wSubscribeRspEvent;
    u16 m_wRefreshRspEvent;
    vector<u16> m_vecSupportedNtfReqEvent;
    vector<string> m_vecSupportedSubType;

    map<TSubscribeKey, TSubscribeData> m_mapSubscribe;  // 设备对应的用户
    map<CCuAgent*, set<TSubscribeKey> > m_mapUser2Keys; // 用户订阅的设备

    list<TRefreshSubReq> m_lstRefreshSubReq; // 等待刷新的请求条目
    u32 m_dwRefreshingReqNum;                // 正在刷新的请求数目

	string m_strNtf;                         // 等待处理的通知，C***NtyReq.ToXml()的结果 

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
    // 返回值为刷新订阅的rsp错误码, vecFailDevUri为不能忽略的失败设备uri列表
    virtual s32 GetRefreshRspResult(CMessage *const pcMsg, vector<string> vecFailDevUri) const = 0; 
	// redis通知不进行缓存，需要的时候直接从redis读取，收到发布的通知后，发布完了就丢弃。
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
