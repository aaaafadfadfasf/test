/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��ospapp.h
����ļ�  ��osp.h
ʵ�ֹ���  ��OSP APP��չ��װ
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/04/20         V1.0              fanxg                                  �½��ļ�
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

//��zTemplate�̳е�һЩ����˽�л�, ��ֹ�û������
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
