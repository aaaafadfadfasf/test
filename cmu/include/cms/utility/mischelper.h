/*****************************************************************************
   ģ����      : Helper
   �ļ���      : MISCHELPER.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ���幫�����������ͺ�
   ����        : ������
   ��Ȩ        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���       �߶���      �޸ļ�¼
   2009/07/18  1.0         ������                     ����
   2012/12/27  1.0         ��־��                     ����ExecCmd�ȵ�ʵ�ֺͷ���ֵ˵��
******************************************************************************/
#ifndef _MISCHELPER_H_
#define _MISCHELPER_H_

#include "helpercommon.h"
#include <string>
using std::string;

//_snprintf_s�Ĳ���������������һ������ʱע��
//#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
//// Microsoft visual studio, version 2005 and higher.
//#define SNPRINTF _snprintf_s
//#define SNSCANF  _snscanf_s
//#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
//#pragma message( "Using _sn* functions." )
#define SNPRINTF _snprintf
#define SNSCANF  _snscanf
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
//#warning( "Using sn* functions." )
#define SNPRINTF snprintf
#define SNSCANF  snscanf
#endif

#ifdef WIN32
    #define POPEN _popen
	#define PCLOSE _pclose
#else
    #define POPEN popen
	#define PCLOSE pclose
#endif

#define StrIsEmpty(lpszPath) (NULL == (lpszPath) || 0 == (lpszPath)[0]) 
char *KdmStrCpyN(char* s1, const char* s2, size_t n);
char* StrCpy_S(char* lpszDst, size_t nDstCnt, const char* lpszSrc);
char* StrTrimR(char* s, int ch = ' ');
char* StrTrimL(char* s, int ch = ' ');
char* StrTrimRL(char* s, int ch = ' ');

s8** AllocTable(u32 dwItemNum, u32 dwItemLen);
void FreeTable(s8** lpszTable, u32 dwItemNum );
/*===========================================================
������    ExecCmd
����      ��ִ��Shell�ű�����
�㷨ʵ��  ��
����˵��  ��
			[in]const char* lpszCmdFormat ��ʽ���ַ���
			[in]...,�ɱ�����б�
����ֵ˵����int: -1 ����ʧ��(ͨ��errno�жϾ������), ���� ��ʾshell�ķ���ֵ
-------------------------------------------------------------
�޸ļ�¼  ��
��  ��		�汾		�޸���		�߶���        �޸ļ�¼
2009/07/17  1.0     ������                    �½�
2012/12/27  1.0     ��־��                    �޸ķ���ֵ˵��  
============================================================*/
#define MAX_SHELL_CMD_LEN 1000
int ExecCmd(const char* lpszCmdFormat, ...);
int PGets(char* pBuf, size_t size, const char* lpszCmdFormat, ...);

int PExecCmd(CBStringOutput pfOutput, void* pUserArg, const char* lpszCmdFormat, ...);

// ���ܣ��ַ����滻���滻src�е�str1Ϊstr2, ���������dst��
void StrReplace(const s8 *src, const s8 *str1, const s8 *str2, s8 *dst);

// ���ܣ������ִ�Сд�ַ����Ƚ�, ����true��ʾ��ȣ������ʾ�����
bool Stricmp(const char * lpszStr1, const char * lpszStr2);

//��ʽ��ʱ�������
s8* GetTimeSpanDes(u32 dwTimeSpan);

//���ܣ��޸��ļ�������(��Linux), ����true��ʾ�ɹ��������ʾ���ɹ�
bool ChangeOwner(const char *lpszUserName, const char *lpszFilePath);

//���ܣ��޸��ļ�Ȩ��Ϊnnn(ʹ������777,520����ʽ)����Linux��,����true��ʾ�ɹ��������ʾ���ɹ�
bool ChangeMod(u32 dwMod, const char *lpszFilePath);



/*===========================================================
������    TruncateUtf8String
����      ������ָ�����ַ�������ȡUtf8�ַ����е�һ�η���Ҫ����Ӵ�
�㷨ʵ��  ��
����˵��  ��
			[in][out]string& strUTF8  utf8��ʽ�ַ���
			[in]u32 dwCharactersNum,utf8�ַ�����(��Ϊ0���򷵻�ԭ�ַ���)
����ֵ˵������
-------------------------------------------------------------
�޸ļ�¼  ��
��  ��		�汾		�޸���		�߶���        �޸ļ�¼
2016/03/01  1.0         ������                    �½� 
============================================================*/
void TruncateUtf8String(string& strUTF8, u32 dwCharactersNum);
bool IsUTF8Begin(u8 ch);

//���ܣ�ʵ��linux�µ�cp  Whui[2016/3/2], 0 ��ʾ�ɹ�
int cp(const char *from, const char *to);
#endif // _MISCHELPER_H_
