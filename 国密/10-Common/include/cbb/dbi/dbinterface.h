/** 
* @file     dbinterface.h 
* @brief    ���ݿ���ʽӿ�  
* @details  
* @author   ֣�� 
* @date     2006/07/17
* @version  1.0   
* @par Copyright (c): v1.0  Copyright(C) 2002-2005 KDC, All rights reserved.
* @par History:          
*   v1.0: ֣��, 2006/07/17, ����
*/

#ifndef _DB_INTERFACE_H
#define _DB_INTERFACE_H

#ifdef _MSC_VER
#include <winsock2.h>	
#include <wtypes.h>	
#endif

#ifndef NULL
#define NULL   0
#endif

#include "kdvtype.h"

#define DBI_UNREFERENCED_PARAM(param)    ( param )   ///< avoid warning

/** ���ݿ����ͺ궨��*/ 
#define DBTYPE_SYBASE     1   ///< sybase���ݿ�
#define DBTYPE_MYSQL      2	  ///< mysql ���ݿ�
#define DBTYPE_ORACLE     3   ///< oracle���ݿ�
#define DBTYPE_SQLITE     4   ///< sqlite���ݿ�

#define MAX_RECORDCOL_NUM     64    ///< Table�������
#define MAX_COLDATA_WIDTH     2048  ///< ����������ֽ���(�����ƴ����image,BLOB)
#define MAX_DB_CONNECTIONS    512   ///< ���������

/** �����鶨��*/
#define DB_LANG_ENGLISH    "us_english"  ///< Ӣ��
#define DB_LANG_CHINESE    "chinese"     ///< ��������
#define DB_LANG_TCHINESE   "tchinese"    ///< ��������

/** �ַ�������*/
#define DB_CHARSET_ISO1       "iso_1"
#define DB_CHARSET_CP850      "cp850"
#define DB_CHARSET_EUCGB      "eucgb"
#define DB_CHARSET_GB18030    "gb18030"
#define DB_CHARSET_UTF8       "utf8"
#define DB_CHARSET_BIG5       "big5"   ///< ���������ַ���

/** �������ݿ�Ľ��������ʾ��Ϣ*/
typedef enum
{
    EOpCode_FindRs,      ///< �����µļ�¼��
    EOpCode_BindDataErr, ///< ������ʧ��
    EOpCode_CmdErr,      ///< ������ִ��ʧ��
    EOpCode_Finish,      ///< ������Ѵ������
    EOpCode_OtherErr     ///< ��������
}EOpCode;

/** ��װ���ݿ�ʱ����ʾ��Ϣ*/
typedef enum
{
    ERR_DBCB_CHANNEL_INVALID = 0,     ///< ��Ч����
    ERR_DBCB_OPERATION_TIMEOUT,       ///< ������ʱ
    ERR_DBCB_UNKNOWN,                 ///< δ֪����
    ERR_DBCB_TBNOTFIND = 208,	      ///< �Ҳ�����Ӧ�ı�
    ERR_DBCB_DBNOTINUSE = 921,        ///< ���ݿ�û�п���
    ERR_DBCB_OFFLINE = 950,		      ///< ���ݿⲻ����
    ERR_DBCB_CHANGEDB = 5701,	      ///< ���ݿ��л���Ϣ
    ERR_DBCB_TEMPTABLE_INUSE = 12822  ///< ��Ҫʹ�õ���ʱ���Ѿ�����
}EDBCbErr;

typedef enum
{
    EDB_PARAM_CONNECT_TIMEOUT,      ///< ���ӳ�ʱʱ��, ���� u8 * seconds
    EDB_PARAM_READ_TIMEOUT,      ///< ����ʱʱ�䣬����  u32 * seconds
    EDB_PARAM_WRITE_TIMEOUT,      ///< д��ʱʱ�䣬����  u32 * seconds
    EDB_PARAM_NUM     ///<
}EDbParamType;

/**
*@brief       �ص���������������˻ص��Ϳͻ��˻ص�
*@param[in]   u16 wErrNum     ��Ϣ������
*@param[in]   LPCSTR lpSrvMsg ��Ϣ����
*@param[in]   void *pContext  �ص�������
*@return      ��
*@ref
*@see
*@note
*/
typedef void (*DBCbFunc) (u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);

/**
*@brief       ��ʼ��ϵͳ�����������Ľṹ
*@param[in]   u8 byDBType   ���ݿ����ͣ�1 : sybase���ݿ�, 2 : mysql ���ݿ�, 
*                                       3 : oracle���ݿ�, 4 : sqlite���ݿ�
*@return      �ɹ�����true, ʧ�ܷ���false
*@ref
*@see
*@note        �˺����������κ����ݿ���󱻴���֮ǰ����
*/
BOOL32 DBInit(u8 byDBType = DBTYPE_SYBASE);

/**
*@brief       ���ñ��������飬�ַ���(���sybase���ݿ�)
*@param[in]   LPCSTR lpLang    ������,Ӣ��:"us_english",��������:"chinese",��������:"tchinese"
*@param[in]   LPCSTR lpCharSet �ַ���,����:"iso_1","cp850","eucgb","gb18030"
*                                          "utf8","big5"(���������ַ���)
*@return      �ɹ�����true��ʧ�ܷ���false
*@ref
*@see
*@note
*/
BOOL32 SetDBLocale(LPCSTR lpLang, LPCSTR lpCharSet);

/**
*@brief       ��װCS_Lib��Ϣ���������������������Ч(sybase���ݿ�)
*@param[in]   DBCbFunc CbFunc  �ص�����
*@param[in]   void *pContext   �ص�������
*@return      �ɹ�����true��ʧ�ܷ���false
*@ref
*@see
*@note
*/
BOOL32 SetupLibErrFuc(DBCbFunc CbFunc, void *pContext = NULL);

/**
*@brief       ���ò�����ʱʱ��
*@param[in]   u8 bySec  ��ʱʱ�䣬��λ��
*@return      �ɹ�����true��ʧ�ܷ���false
*@ref
*@see
*@note
*/
BOOL32 SetDBTimeOut(u8 bySec);

/**
*@brief       ���ò�����ʱʱ��
*@param[in]   u8 bySec  ��ʱʱ�䣬��λ��
*@return      �ɹ�����true��ʧ�ܷ���false
*@ref
*@see
*@note   �ó�ʱ������Ҫ�ڳ�ʼ�����ݿ�(mysql_init)��
*  ���������ݿ�֮ǰ(mysql_connect or mysql_real_connect, ������ConnecttoSrv�ӿ�ǰ)���òſ���Ч
*/
BOOL32 SetDBParam(EDbParamType eType, void * value);

/**
*@brief       ��Buf���ȡ�ַ�����
*@param[in]   const void *pBuf  Դbuf
*@param[in]   u16 wBufLen       Buf����
*@param[out]  LPSTR lpStr       Buf���ַ�����
*@param[out]  u16 wStrInLen     Buf���ַ���������
*@return      Buf���ַ�����
*@ref
*@see
*@note        �ṩ�Ļ���������Ϊ�ڴ�������󳤶�*2+1
*/
LPCSTR DBGetStrFromBuf(const void *pBuf, u16 wBufLen, LPSTR lpStr, u16 wStrInLen);

/**
*@brief       �˳�ϵͳ
*@return      �ɹ�����true, ʧ�ܷ���false
*@ref
*@see
*@note
*/
BOOL32 DBExit();

/**
*@brief       ����DBI��־�ļ����λ��
*@param[in]   LPCSTR lpcLogDir  ��־�ļ�·��
*@return      �ɹ�����true, ʧ�ܷ���false
*@ref
*@see
*@note        ·�����Ȳ��ܴ���500�ֽ�,��������Ĭ��·��Ϊ./errorlog
*/
BOOL32 SetDBILogDir(LPCSTR lpcLogDir);

/**
*@brief       ���ô�����־д����
*@param[in]   BOOL32 dwSwitch  �Ƿ������أ�TRUE��ʾд��־����֮��д
*@return      �ɹ�����true, ʧ�ܷ���false
*@ref
*@see
*@note        ����Ҫ�޸�Ĭ����־���·��,��Ӧ���޸���־���·��,Ĭ��״̬�²�д��־
*/
BOOL32 SetFileLog(BOOL32 dwSwitch);

class CDBRs;
/** �������ݿ�ʵ����*/
class CDBHandlerImp;
/** �������ݿ�ӿ���*/
class CDBHandler
{
public:
    CDBHandler();
    ~CDBHandler();

public:
    /**
    *@brief       ���������ݿ������������
    *@param[in]   LPCSTR szUsrName ��½�û���
    *@param[in]   LPCSTR szPwd     ����
    *@param[in]   LPCSTR szDBName  ���ݿ���
    *@param[in]   LPCSTR szSrvIP   ���ݿ��������ַ(�ַ���IP)
    *@param[in]   u16 wPort        �˿�
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note        �ýӿڲ��������ݿ�Ĵ��������е���
    */
    BOOL32 ConnecttoSrv(LPCSTR szUsrName, LPCSTR szPwd,
        LPCSTR szDBName, LPCSTR szSrvIP, u16 wPort = 5000);

    /**
    *@brief       �Ͽ������ݿ������������
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 CloseConnection();

    /**
    *@brief       ��װ�ͻ��˴�����Ϣ������
    *@param[in]   DBCbFunc CbFunc �������ָ��
    *@param[in]   void *pContext  �ص�������
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 SetupCtErrFuc(DBCbFunc CbFunc, void *pContext = NULL);

    /**
    *@brief       ��װ�������˴�����Ϣ������
    *@param[in]   DBCbFunc CbFunc �������ָ��
    *@param[in]   void *pContext  �ص�������
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 SetupSrvErrFuc(DBCbFunc CbFunc, void *pContext = NULL);

    /**
    *@brief       �Ƴ��������ϵĻص���Ϣ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    void RemoveCallBack();

    /**
    *@brief       �����ݿ�
    *@param[in]   LPCSTR szDBName Ŀ�����ݿ���
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note        ֻ��mysql���ݿ���Ч
    */
    BOOL32 UseDB(LPCSTR szDBName);

    /**
    *@brief       ִ��������sql���
    *@param[in]   LPCSTR szsql  Ҫִ�е�sql�����ִ�
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note        sql���ĩβ�����зֺţ������ִ��ʱ�������֮��Ҫ�÷ֺŸ�����
    *             �������һ�����ĩβ���ܴ��ֺ�
    */
    BOOL32 ExecSql(LPCSTR szsql);

    /**
    *@brief       ִ��������sql���
    *@param[in]   LPCSTR szsql           Ҫִ�е�sql�����ִ�
    *@param[out]  CDBRsImp *pcRecordSet  ִ��sql����Ľ����ָ��
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note        sql���ĩβ�����зֺţ������ִ��ʱ�������֮��Ҫ�÷ֺŸ�����
    *             �������һ�����ĩβ���ܴ��ֺ�
    */
    BOOL32 ExecSql(LPCSTR szsql, CDBRs *pcRecordSet);

    /**
    *@brief       �����û��Զ���ķǽṹ�����ݵ����ݿ����
    *@param[in]   LPCSTR lpszSql         Ҫִ�е�sql�ִ�
    *@param[in]   const void *pBuf       �������׵�ַ
    *@param[in]   s32 sdwBufLen          ����������
    *@param[out]  CDBRsImp *pcRecordSet  �����ָ��
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 InsertBuf(LPCSTR lpszSql, CDBRs *pcRecordSet, const void *pBuf, s32 sdwBufLen);

    /**
    *@brief       ��������(�����ڴ�������¼������)
    *@param[in]   BOOL32 bIsClear �Ƿ������ǰ�����
    *@return      EOpCode_FindRs  �����µļ�¼��
    *             EOpCode_Finish  ������Ѵ������
    *             EOpCode_BindDataErr ���ݰʧ��
    *             EOpCode_CmdErr  ����ĳ������ִ��ʧ��
    *@ref
    *@see
    *@note
    */
    EOpCode GoOnProcRes(); 

    /**
    *@brief       ��ȡ���β���������Ӱ��ļ�¼����
    *@return      ��Ӱ��ļ�¼����
    *@ref
    *@see
    *@note
    */
    u32 GetAffectedRowCount();

    /**
    *@brief       ��ȡ���ݿ�����ַ
    *@return      ���ݿ�����ַ
    *@ref
    *@see
    *@note
    */
    CDBHandlerImp* GetHandleAddr();

protected:
    CDBHandlerImp *m_pcDBHandlerImp;  ///< ���ݿ���
};

/** �������ݿ�����ʵ����*/
class CDBRsImp;
/** �������ݿ������ӿ���*/
class CDBRs
{
public:
    CDBRs();
    ~CDBRs();

public:    
    /**
    *@brief       ָ����������һ����¼
    *@return      �ɹ�������ǰ�з���true, ���򷵻�false
    *@ref
    *@see
    *@note
    */
    BOOL32 MoveNext();

    /**
    *@brief       �жϽ�����Ƿ񵽴�ĩβ
    *@return      ����ĩβ����true, ����false
    *@ref
    *@see
    *@note
    */
    BOOL32 IsEof();

    /**
    *@brief       ��ȡ��������� 
    *@return      ���������
    *@ref
    *@see
    *@note
    */
    const u16 GetFieldCount();

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  double Value      ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, double& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  float Value       ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, float& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  s64 Value         ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, s64& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  u64 Value         ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, u64& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  s32 Value         ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, s32& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  u32 Value         ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, u32& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  s16 Value         ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, s16& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  u16 Value         ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, u16& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  s8 Value          ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, s8& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[out]  u8 Value          ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, u8& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[in]   u16 wBufLen       ������Ҳ����pstr����󳤶�
    *@param[out]  LPSTR pstr        �����ִ�ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, LPSTR pstr, u16 wBufLen);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[in]   u16 wBufLen       ������Ҳ����pBuf����󳤶�
    *@param[out]  void *pBuf        ������, ������Ÿ����ִ�ֵ
    *@param[out]  u16 wOutLen       �����ִ�ֵ��С
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, void *pBuf, u16 wBufLen, u16 &wOutLen);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   LPCSTR lpColName  ����
    *@param[in]   s32 sdwBufLen       ������Ҳ����pBuf����󳤶�
    *@param[out]  void *pBuf        ������, ������Ÿ����ִ�ֵ
    *@param[out]  s32 sdwOutLen       �����ִ�ֵ��С
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, void *pBuf, s32 sdwBufLen, s32 &sdwOutLen);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex   ������, ������1��ʼ
    *@param[out]  double Value ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, double& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex   ������, ������1��ʼ
    *@param[out]  float Value  ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, float& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex  ������, ������1��ʼ
    *@param[out]  s64 Value   ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, s64& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex  ������, ������1��ʼ
    *@param[out]  u64 Value   ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, u64& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex  ������, ������1��ʼ
    *@param[out]  s32 Value   ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, s32& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex  ������, ������1��ʼ
    *@param[out]  u32 Value   ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, u32& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex  ������, ������1��ʼ
    *@param[out]  s16 Value   ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, s16& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex  ������, ������1��ʼ
    *@param[out]  u16 Value   ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, u16& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex  ������, ������1��ʼ
    *@param[out]  s8 Value    ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, s8& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex  ������, ������1��ʼ
    *@param[out]  u8 Value    ����ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, u8& Value);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex   ������, ������1��ʼ
    *@param[in]   u16 wBufLen  ������Ҳ����pstr����󳤶�
    *@param[out]  LPSTR pstr   �����ִ�ֵ
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, LPSTR pstr, u16 wBufLen);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex   ������, ������1��ʼ
    *@param[in]   u16 wBufLen  ������Ҳ����pBuf����󳤶�
    *@param[out]  void *pBuf   ������, ������Ÿ����ִ�ֵ
    *@param[out]  u16 wOutLen  �����ִ�ֵ��С
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, void *pBuf, u16 wBufLen, u16 &wOutLen);

    /**
    *@brief       ��ȡĳ��ֵ
    *@param[in]   u8 byIndex   ������, ������1��ʼ
    *@param[in]   s32 sdwBufLen  ������Ҳ����pBuf����󳤶�
    *@param[out]  void *pBuf   ������, ������Ÿ����ִ�ֵ
    *@param[out]  s32 sdwOutLen  �����ִ�ֵ��С
    *@return      �ɹ�true, ʧ��false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, void *pBuf, s32 sdwBufLen, s32 &sdwOutLen);

    /**
    *@brief       ��ȡ���ݿ����������ַ
    *@return      ���ݿ����������ַ
    *@ref
    *@see
    *@note
    */
    CDBRsImp* GetRsImp();

protected:
    CDBRsImp *m_pcDBRsImp;  ///< ���ݿ��������
};

#endif
