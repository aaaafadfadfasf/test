/*****************************************************************************
   文件实现功能: 监控产线自定义的加密器
   作者        : 胡志云
   版权        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人       走读人      修改记录
   2013/05/25  1.0        胡志云                   移植自1.0
******************************************************************************/

#ifndef _KDM_ENCRYPTOR_H_
#define _KDM_ENCRYPTOR_H_

#include "kdvsys.h"

class CEncryptor
{
public:
    CEncryptor( void );
    virtual ~CEncryptor();
    // 从pachString中取出dwBitPos位置Bit的数值
    static bool GetBitAt(const s8* pachString, u32 dwBitPos);
    // 从pachString中设置dwBitPos位置Bit的数值
    static void SetBitAt(s8* pachString, u32 dwBitPos, bool bSetToOne);
    // 加密。把[pachPlainText+wPlainSize]指定的密文转化为[pachCipherText+wCipherSize]指定的明文
    static bool EncryptString(const s8* pachPlainText, const u16 wPlainSize, s8* pachCipherText, u16& wCipherSize);
    // 解密。把[pachCipherText+wCipherSize]指定的密文转化为[pachPlainText+wPlainSize]指定的明文
    static bool DecryptString(const s8* pachCipherText, const u16 wCipherSize, s8* pachPlainText, u16& wPlainSize);
};

#endif // _KDM_ENCRYPTOR_H_
