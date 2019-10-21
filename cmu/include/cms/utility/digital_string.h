/*========================================================================================
模块名    ：digital_string
文件名    ：digital_string.h
相关文件  ：
实现功能  ：数字-字符串类型
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2013/12/30         V1.0              fanxg                                  新建文件
=========================================================================================*/
#ifndef _DIGITAL_STRING_H_
#define _DIGITAL_STRING_H_

#include <stdlib.h>
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include "kdvtype.h"

class digital_string : public string
{
public:
    digital_string(const string& strVal)
    {
        string::operator =(strVal);
    }

    digital_string(s32 nVal)
    {
        SetS32(nVal);
    }

    digital_string(u32 dwVal)
    {
        SetU32(dwVal);
    }

    bool operator==(s32 nVal)
    {
        return (GetS32() == nVal);
    }

    bool operator==(u32 dwVal)
    {
        return (GetU32() == dwVal);
    }

public:

    s32 GetS32() const
    {
        return atoi(this->c_str());
    }
    u32 GetU32() const
    {
        return strtoul(this->c_str(), NULL, 10);
    }

    void SetS32(s32 nVal)
    {
        SetVal(nVal);
    }
    void SetU32(u32 dwVal)
    {
        SetVal(dwVal);
    }

    template<class T>
    void SetVal(T tVal)
    {
        ostringstream oss;
        oss<<tVal;
        this->operator =(oss.str());
    }
};

#endif  //#ifndef _DIGITAL_STRING_H_

