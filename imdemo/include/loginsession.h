/*========================================================================================
模块名    ：cmu
文件名    ：loginsession.h
相关文件  ：
实现功能  ：所有登录事务抽象基类，包括pu、cu、pui、cui、vtdu、cmu等各种登录事务需要
            从它继承，该基类实现登录事务的子事务的统一管理
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/02/06         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _LOGIN_SESSION_H
#define _LOGIN_SESSION_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xset.h"
#include "cms/cms_struct.h"
#include "cms/uas//uas_struct.h"

class CLoginSession : public CSipTask
{
public:
    CLoginSession(CInstExt *pcInst);
    virtual ~CLoginSession();

    virtual void PrintData() const;
public:
    void AddTask(CTask* pTask);

    void DelTask(CTask* pTask);

    void DestroyChildTasks();

    bool IterateChildTasks(Iterator& rNextPosition, CTask*& pTask) const;

    void CallbackToChildTask(CMessage *const pcMsg);
    void CallbackToChildTask(u16 event, const void* content=0, u16 length=0);

    set<u32> GetChildTaskNO() const;
    void PrintChildTasks() const;

    virtual TDeviceLogInfo GetDevLogInfo(bool bOnOffState);

public:
    //业务处理
    virtual bool IsService() const = 0;

public:
    //数据操作
    const string& GetSession() const
    {
        return m_strSession;
    }
    string& GetSession()
    {
        return m_strSession;
    }
    void SetSession(const string& strSession)
    {
        m_strSession = strSession;
    }

    //m_strDevUri
    void SetDevUri(const string& strDevUri)
    {
        m_strDevUri = strDevUri;
    }
    string& GetDevUri()
    {
        return m_strDevUri;
    }
    const string& GetDevUri() const
    {
        return m_strDevUri;
    }

    //m_strDevType
    void SetDevType(const string& strDevType)
    {
        m_strDevType = strDevType;
    }
    string& GetDevType()
    {
        return m_strDevType;
    }
    const string& GetDevType() const
    {
        return m_strDevType;
    }

    //m_tDevAddrList
    void SetDevAddrList(const TNetAddrList& cDevAddrList)
    {
        m_tDevAddrList = cDevAddrList;
    }
    TNetAddrList& GetDevAddrList()
    {
        return m_tDevAddrList;
    }
    const TNetAddrList& GetDevAddrList() const
    {
        return m_tDevAddrList;
    }

    //m_strServerUri
    void SetServerUri(const string& strServerUri)
    {
        m_strServerUri = strServerUri;
    }
    string& GetServerUri()
    {
        return m_strServerUri;
    }
    const string& GetServerUri() const
    {
        return m_strServerUri;
    }

    //m_tServerAddrList
    void SetServerAddrList(const TNetAddrList& cServerAddrList)
    {
        m_tServerAddrList = cServerAddrList;
    }
    TNetAddrList& GetServerAddrList()
    {
        return m_tServerAddrList;
    }
    const TNetAddrList& GetServerAddrList() const
    {
        return m_tServerAddrList;
    }

    //tUri
    TSipURI GetDevSipUri() const
    {
        TSipURI tUri;
        tUri.SetURIString(GetDevUri());
        return tUri;
    }

    TSipURI GetServerSipUri() const
    {
        TSipURI tUri;
        tUri.SetURIString(GetServerUri());
        return tUri;
    }

    virtual bool IsEnableOfflineSwitch() const
    {
        if (this->GetDevType() == DEV_TYPE_PU 
            || this->GetDevType() == DEV_TYPE_VTDU
            || this->GetDevType() == DEV_TYPE_RCS)
        {
            return true;
        }

        return false;
    }

	bool HasDirectTransferCap()
	{
		if (this->GetCurDirectTransNum() < this->GetSupportDirectTransNum())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

    virtual s32 GetSupportDirectTransNum()
    {
        return 0;
    }

    s32 GetCurDirectTransNum()
    {
        return m_nCurDirectTransNum;
    }
    void AddCurDirectTransNum()
    {
        m_nCurDirectTransNum++;
    }
    void SubCurDirectTransNum()
    {
        m_nCurDirectTransNum--;
    }

     bool GetIsNeedPunch() const
     {
         return m_bIsNeedPunch;
     }
     void SetIsNeedPunch(bool bIsNeedPunch)
     {
         m_bIsNeedPunch = bIsNeedPunch;
     }

     virtual size_t GetDevNetSegNO()
     {
         return 0;
     }

	 virtual string GetJoinInterfaceIp(const string &strSession)
	 {
		 return "";
	 }

private:
    string m_strSession;        //登录session
    CXSet<CTask*> m_tTaskList;  //登录session当前存在的Task列表

    //登录设备信息
    string          m_strDevUri;                   //设备URI
    string          m_strDevType;                  //设备类型
    TNetAddrList    m_tDevAddrList;                //设备地址列表

    //登录服务器信息
    string          m_strServerUri;                //服务器URI
    TNetAddrList    m_tServerAddrList;             //服务器地址列表

    //扩展信息
    s32             m_nCurDirectTransNum;

    bool            m_bIsNeedPunch;                 //是否需要Punch，默认true
};

//20130606 hzc: 设备下线转为改该消息回调通知给所有子任务
struct TDevOnOffCBMsg
{
public:
    TDevOnOffCBMsg()
    {
    }

    //devUri
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    //devType
    void SetDevType(const TDevType& strDevType)
    {
        devType = strDevType;
    }
    TDevType& GetDevType()
    {
        return devType;
    }
    const TDevType& GetDevType() const
    {
        return devType;
    }

private:
    string           devUri;        //设备URI
    TDevType         devType;       //设备类型
};

#endif  //#ifndef _LOGIN_SESSION_H