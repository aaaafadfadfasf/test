/********************************************************************
	ע������:	2010/07/29
	author:		��Ӧ��
	
	purpose:	ʵ�� CMS_ASSERT �� CMS_ASSERT_WITH_MSG
                CMS_ASSERT �� ASSERT ��Чһ��
                CMS_ASSERT_WITH_MSG , ������һ����ʾ����,�����п���д��ʾ.
                �� OspOpenLogFile �˵�ʱ��,�Ὣ Assertion д��log�ļ���.
                ��Щ������ Debug �汾��Ч,������ ALPHA_RELEASE �� Release �汾Ҳ��Ч.
                �� Release �汾��Ч.
                �� UNIT_TEST �汾��Ч,�Զ�����Ԫ����ʱͨ��Ҫ���Ժ�����ǿ׳��.

                _CMS_ASSERT_WITH_MSG_IMPL ���ڲ�ʵ��,�ⲿ��Ҫ����.
    modify:     liangli
                Ϊ�˷�ֹ�û���exp�����ֱ��д���в�������ı��ʽ, ������Release�汾��û��ִ�в���
                �޸���CMS_ASSERT�Ķ���, ִ��exp;
                lizhixing 2012.2.17
                ԭ�ȵĶ��Դ�����ֱ��exit(1)�˳����򣬵����޷�����������Ϣ�������ó���Ա��λ��������ԭ��
                �����޸�Ϊthrow��ʽ����windows��linux��ͬ����Ч
                �������˰汾�����Զ��˳�����������½��̣����Է�release�汾�Ͻ�������ʽ����
    ��Ȩ        : Copyright(C) 2003-2009 Suzhou Keda Technology Co., Ltd. All rights reserved
*********************************************************************/

#ifndef CMS_ASSERT_H
#define CMS_ASSERT_H

#include <cassert>
//#include "osp.h"
#include "kdvtype.h"
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _MSC_VER
#include <WinSock2.h>
#include <Windows.h>
#include <time.h>

// ����֧�ֶ������������Ĵ�ӡ��
#ifndef __FUNCTION__
#define __FUNCTION__ ""
#endif

#endif

#ifdef _LINUX_
#include <unistd.h>
#include <string.h>
#endif

/*====================================================================
��������TaskDelay
���ܣ�������ʱ
�㷨ʵ�֣�����ѡ�
����ȫ�ֱ�����
�������˵����uMs: ʱ����(ms)

����ֵ˵����
====================================================================*/
inline void TaskDelay(int dwMs)
{
#ifdef WIN32
    Sleep(dwMs);
#endif

#ifdef _LINUX_
    usleep(dwMs*1000);
#endif
}

/** дAssertԭ���ļ���־�� */
inline void DumpAssertMsg( const char* exp, const char* pFileName, int lineNum, const char* funName, const char* szMsg )
{
	std::stringstream ssAssert;
	ssAssert << "Assertion: " << exp 
		<< " , Fun: " << funName
		<< " , File: " << pFileName
		<< " , Line: " << lineNum
		<< " , Msg: " << szMsg
		<< std::endl;
	
	// ��ӡ�������С�
	std::cout << ssAssert.str();
	// ��ӡ�����������
	std::cerr << ssAssert.str();
	// д�������ļ���
	std::stringstream ssAssertFileName;
	tm localT;
	memset( &localT, 0, sizeof( localT ) );
	time_t now = time( 0 );
#ifdef _LINUX_
    localtime_r(&now, &localT);
#else
    tm *pLocalT = ::localtime( &now );
    if ( pLocalT )
    {
        localT = *pLocalT;
    }
#endif
		
	ssAssertFileName << "Assertion_" << localT.tm_year + 1900 << localT.tm_mon + 1 << 
		localT.tm_mday << localT.tm_hour << localT.tm_min << localT.tm_sec
		<< ".txt";
	std::ofstream fAssert( ssAssertFileName.str().c_str() );
	fAssert << ssAssert.str() << std::endl;
	fAssert.close();
}

//// _CMS_ASSERT_WITH_MSG_IMPL �ڲ�ʵ��,�ⲿ�����.
// ʹ�� throw �׳��쳣�ķ�ʽ, ����ͨ���źŽ� gdb��windows����������������. 
// Windows����ʱ�ڵ�� "����" ��������ܼ�������. linux�³����˳�..
// TaskDelay �����ȴ������첽����־��ӡ��д��־�ļ���ɡ�
#define _CMS_ASSERT_WITH_MSG_IMPL(exp,msg) do{\
            if(!(exp)){\
			DumpAssertMsg( #exp , __FILE__, __LINE__, __FUNCTION__, static_cast<const char*>(msg) );\
			TaskDelay( 100 );\
			throw( msg );\
            }\
        }while(0)

//// Release �汾�� Alpha Release �汾.
#ifdef NDEBUG    //release version

    #ifdef ALPHA_RELEASE ////enable assert in Alpha Release Version

        #define CMS_ASSERT_WITH_MSG _CMS_ASSERT_WITH_MSG_IMPL

        #define CMS_ASSERT(exp) do{\
            CMS_ASSERT_WITH_MSG(exp,"");\
        }while(0)
        
    #else    ////disable assert in Release Version

        #define CMS_ASSERT_WITH_MSG(exp,msg) do {  \
                                            } while(0)
        #define CMS_ASSERT(exp)     do {  \
                                    } while(0)
        
    #endif

#else 
////debug version
#define CMS_ASSERT_WITH_MSG _CMS_ASSERT_WITH_MSG_IMPL

#define CMS_ASSERT(exp) do{\
            CMS_ASSERT_WITH_MSG(exp,"");\
        }while(0)

#endif 

////disable assert in automatic unittest
#ifdef UNIT_TEST
    #undef CMS_ASSERT_WITH_MSG
    #define CMS_ASSERT_WITH_MSG(exp,msg)     do { exp; \
                                            } while(0)

    #undef CMS_ASSERT
    #define CMS_ASSERT(exp)     do { exp; \
                                } while(0)
#endif

#endif

/////////////////////////////////////// END OF FILE /////////////////////////////////////
