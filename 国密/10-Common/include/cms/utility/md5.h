/*****************************************************************************
   �ļ�ʵ�ֹ���: MD5���ܹ���
   ����        : ��־��
   ��Ȩ        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���       �߶���      �޸ļ�¼
   2013/05/30  1.0        ��־��                   �������ϰ汾����
******************************************************************************/

#ifndef MD5_H
#define MD5_H
#include <string>
using std::string;

// �����ַ�����MD5ֵ
void MD5(const string &strEncrypt, string &strDecrypt);

#ifdef _LINUX_
    int MD5Sum(const string& strFilePath, string& strMD5Sum);
#endif
#endif