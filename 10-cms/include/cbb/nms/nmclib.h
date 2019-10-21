/*****************************************************************************
   文件实现功能: 网管客户端库的接口封装，用于平台类型网元
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/03/30   1.0        huzhiyun        创建
******************************************************************************/
#ifndef _NMCLIB_H_
#define _NMCLIB_H_

#include "kdvtype.h"
#include <string>
using std::string;
#include "snmp_msg.h"

class CNmcLib
{
public:
    CNmcLib();
    virtual ~CNmcLib();
    
    // strNmcId网元编号（模块的uuid，没有uuid的使用模块名，比如cmu），strNmcSubType网元子类型,strNmcName网元名称（一般与类型相同）
    // strNmsIP网管服务器IP，wNmsPort网管服务器Port, strNmcIP网元IP
    bool Init(const string &strNmcId, const TNmcSubType &strNmcSubType, const string &strNmcName, 
              const string &strNmsIP, u16 wNmsPort, const string &strNmcIP);
    void Quit();

    // 发送网管消息接口, dwMsgType消息号(参考snmp_msg.h), pMsgContent为消息体内容，wMsgContentLen为消息体长度
    bool SendMsg(u32 dwMsgType, const void *pMsgContent, u16 wMsgContentLen);
    bool HasConnectedNms() const;
    void EnableDebug();
    void DisableDebug();


    // dwMsgType：消息号, bRWFlag: true时表示读取否则表示写入
    // (读取时业务程序往缓存中写入数据同时更新pwBufLen，写入时业务程序从缓存中读取数据)
    // pMsgContent为消息体缓存，pwMsgContentLen为消息体缓存长度
    virtual bool CBNmcMsg(u32 dwMsgType, bool bRWFlag, void *pMsgContent, u16 *pwMsgContentLen){return false;}
};

#endif // _NMCLIB_H_
