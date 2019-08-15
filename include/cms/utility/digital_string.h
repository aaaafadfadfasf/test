/*========================================================================================
ģ����    ��digital_string
�ļ���    ��digital_string.h
����ļ�  ��
ʵ�ֹ���  ������-�ַ�������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2013/12/30         V1.0              fanxg                                  �½��ļ�
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

