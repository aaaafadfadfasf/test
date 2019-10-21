/*========================================================================================
模块名    ：ospext.lib
文件名    ：ospapp.h
相关文件  ：osp.h
实现功能  ：OSP APP扩展封装
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/04/20         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _OSP_APP_EXT_H
#define _OSP_APP_EXT_H

#include "cbb/osp/osp.h"

class COspAppImpl
{
public:
    BOOL32 CreateOspApp(CApp* pApp, char *name, u16 aid, u8 pri, u16 queuesize = 80, u32 uStackSize = 200<<10 ) const;
};

template <class I, int maxins=1, class A=CAppNoData, u8 maxAliasLen=0>
class COspApp: public zTemplate<I, maxins, A, maxAliasLen>
{
public:
    typedef zTemplate<I, maxins, A, maxAliasLen> BaseApp;
    typedef A AppDataType;

//将zTemplate继承的一些函数私有化, 防止用户误调用
protected:
    using CApp::CreateApp;

public:

    A& GetAppData()
    {
        return (A&)(*this);
    }

    BOOL32 CreateOspApp( char *name, u16 aid, u8 pri, u16 queuesize = 80, u32 uStackSize = 200<<10 )
    {
        return m_cAppImpl.CreateOspApp(this, name, aid, pri, queuesize, uStackSize);
    }

private:
    COspAppImpl m_cAppImpl;
};

#endif  //#ifndef _OSP_APP_EXT_H
