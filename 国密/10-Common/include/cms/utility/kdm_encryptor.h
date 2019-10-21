/*****************************************************************************
   �ļ�ʵ�ֹ���: ��ز����Զ���ļ�����
   ����        : ��־��
   ��Ȩ        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���       �߶���      �޸ļ�¼
   2013/05/25  1.0        ��־��                   ��ֲ��1.0
******************************************************************************/

#ifndef _KDM_ENCRYPTOR_H_
#define _KDM_ENCRYPTOR_H_

#include "kdvsys.h"

class CEncryptor
{
public:
    CEncryptor( void );
    virtual ~CEncryptor();
    // ��pachString��ȡ��dwBitPosλ��Bit����ֵ
    static bool GetBitAt(const s8* pachString, u32 dwBitPos);
    // ��pachString������dwBitPosλ��Bit����ֵ
    static void SetBitAt(s8* pachString, u32 dwBitPos, bool bSetToOne);
    // ���ܡ���[pachPlainText+wPlainSize]ָ��������ת��Ϊ[pachCipherText+wCipherSize]ָ��������
    static bool EncryptString(const s8* pachPlainText, const u16 wPlainSize, s8* pachCipherText, u16& wCipherSize);
    // ���ܡ���[pachCipherText+wCipherSize]ָ��������ת��Ϊ[pachPlainText+wPlainSize]ָ��������
    static bool DecryptString(const s8* pachCipherText, const u16 wCipherSize, s8* pachPlainText, u16& wPlainSize);
};

#endif // _KDM_ENCRYPTOR_H_
