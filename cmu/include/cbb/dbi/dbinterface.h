/** 
* @file     dbinterface.h 
* @brief    数据库访问接口  
* @details  
* @author   郑榕 
* @date     2006/07/17
* @version  1.0   
* @par Copyright (c): v1.0  Copyright(C) 2002-2005 KDC, All rights reserved.
* @par History:          
*   v1.0: 郑榕, 2006/07/17, 创建
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

/** 数据库类型宏定义*/ 
#define DBTYPE_SYBASE     1   ///< sybase数据库
#define DBTYPE_MYSQL      2	  ///< mysql 数据库
#define DBTYPE_ORACLE     3   ///< oracle数据库
#define DBTYPE_SQLITE     4   ///< sqlite数据库

#define MAX_RECORDCOL_NUM     64    ///< Table最大列数
#define MAX_COLDATA_WIDTH     2048  ///< 数据项最大字节数(二进制大对象image,BLOB)
#define MAX_DB_CONNECTIONS    512   ///< 最大连接数

/** 语言组定义*/
#define DB_LANG_ENGLISH    "us_english"  ///< 英文
#define DB_LANG_CHINESE    "chinese"     ///< 简体中文
#define DB_LANG_TCHINESE   "tchinese"    ///< 繁体中文

/** 字符集定义*/
#define DB_CHARSET_ISO1       "iso_1"
#define DB_CHARSET_CP850      "cp850"
#define DB_CHARSET_EUCGB      "eucgb"
#define DB_CHARSET_GB18030    "gb18030"
#define DB_CHARSET_UTF8       "utf8"
#define DB_CHARSET_BIG5       "big5"   ///< 繁体中文字符集

/** 处理数据库的结果集的提示信息*/
typedef enum
{
    EOpCode_FindRs,      ///< 发现新的记录集
    EOpCode_BindDataErr, ///< 绑定数据失败
    EOpCode_CmdErr,      ///< 有命令执行失败
    EOpCode_Finish,      ///< 结果集已处理完毕
    EOpCode_OtherErr     ///< 其它错误
}EOpCode;

/** 安装数据库时的提示信息*/
typedef enum
{
    ERR_DBCB_CHANNEL_INVALID = 0,     ///< 无效连接
    ERR_DBCB_OPERATION_TIMEOUT,       ///< 操作超时
    ERR_DBCB_UNKNOWN,                 ///< 未知错误
    ERR_DBCB_TBNOTFIND = 208,	      ///< 找不到对应的表
    ERR_DBCB_DBNOTINUSE = 921,        ///< 数据库没有开启
    ERR_DBCB_OFFLINE = 950,		      ///< 数据库不在线
    ERR_DBCB_CHANGEDB = 5701,	      ///< 数据库切换消息
    ERR_DBCB_TEMPTABLE_INUSE = 12822  ///< 所要使用的临时表已经存在
}EDBCbErr;

typedef enum
{
    EDB_PARAM_CONNECT_TIMEOUT,      ///< 连接超时时间, 参数 u8 * seconds
    EDB_PARAM_READ_TIMEOUT,      ///< 读超时时间，参数  u32 * seconds
    EDB_PARAM_WRITE_TIMEOUT,      ///< 写超时时间，参数  u32 * seconds
    EDB_PARAM_NUM     ///<
}EDbParamType;

/**
*@brief       回调函数，包括服务端回调和客户端回调
*@param[in]   u16 wErrNum     消息错误码
*@param[in]   LPCSTR lpSrvMsg 消息内容
*@param[in]   void *pContext  回调上下文
*@return      无
*@ref
*@see
*@note
*/
typedef void (*DBCbFunc) (u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);

/**
*@brief       初始化系统，分配上下文结构
*@param[in]   u8 byDBType   数据库类型，1 : sybase数据库, 2 : mysql 数据库, 
*                                       3 : oracle数据库, 4 : sqlite数据库
*@return      成功返回true, 失败返回false
*@ref
*@see
*@note        此函数必须在任何数据库对象被创建之前调用
*/
BOOL32 DBInit(u8 byDBType = DBTYPE_SYBASE);

/**
*@brief       设置本地语言组，字符集(针对sybase数据库)
*@param[in]   LPCSTR lpLang    语言组,英文:"us_english",简体中文:"chinese",繁体中文:"tchinese"
*@param[in]   LPCSTR lpCharSet 字符集,包括:"iso_1","cp850","eucgb","gb18030"
*                                          "utf8","big5"(繁体中文字符集)
*@return      成功返回true，失败返回false
*@ref
*@see
*@note
*/
BOOL32 SetDBLocale(LPCSTR lpLang, LPCSTR lpCharSet);

/**
*@brief       安装CS_Lib消息处理函数，针对所有连接有效(sybase数据库)
*@param[in]   DBCbFunc CbFunc  回调函数
*@param[in]   void *pContext   回调上下文
*@return      成功返回true，失败返回false
*@ref
*@see
*@note
*/
BOOL32 SetupLibErrFuc(DBCbFunc CbFunc, void *pContext = NULL);

/**
*@brief       设置操作超时时间
*@param[in]   u8 bySec  超时时间，单位秒
*@return      成功返回true，失败返回false
*@ref
*@see
*@note
*/
BOOL32 SetDBTimeOut(u8 bySec);

/**
*@brief       设置操作超时时间
*@param[in]   u8 bySec  超时时间，单位秒
*@return      成功返回true，失败返回false
*@ref
*@see
*@note   该超时设置需要在初始化数据库(mysql_init)后，
*  在连接数据库之前(mysql_connect or mysql_real_connect, 即调用ConnecttoSrv接口前)调用才可生效
*/
BOOL32 SetDBParam(EDbParamType eType, void * value);

/**
*@brief       从Buf里获取字符描述
*@param[in]   const void *pBuf  源buf
*@param[in]   u16 wBufLen       Buf长度
*@param[out]  LPSTR lpStr       Buf的字符描述
*@param[out]  u16 wStrInLen     Buf的字符描述长度
*@return      Buf的字符描述
*@ref
*@see
*@note        提供的缓冲区长度为期待数据最大长度*2+1
*/
LPCSTR DBGetStrFromBuf(const void *pBuf, u16 wBufLen, LPSTR lpStr, u16 wStrInLen);

/**
*@brief       退出系统
*@return      成功返回true, 失败返回false
*@ref
*@see
*@note
*/
BOOL32 DBExit();

/**
*@brief       设置DBI日志文件存放位置
*@param[in]   LPCSTR lpcLogDir  日志文件路径
*@return      成功返回true, 失败返回false
*@ref
*@see
*@note        路径长度不能大于500字节,不设置则默认路径为./errorlog
*/
BOOL32 SetDBILogDir(LPCSTR lpcLogDir);

/**
*@brief       设置错误日志写开关
*@param[in]   BOOL32 dwSwitch  是否开启开关，TRUE表示写日志，反之不写
*@return      成功返回true, 失败返回false
*@ref
*@see
*@note        如需要修改默认日志存放路径,则应先修改日志存放路径,默认状态下不写日志
*/
BOOL32 SetFileLog(BOOL32 dwSwitch);

class CDBRs;
/** 操作数据库实现类*/
class CDBHandlerImp;
/** 操作数据库接口类*/
class CDBHandler
{
public:
    CDBHandler();
    ~CDBHandler();

public:
    /**
    *@brief       建立到数据库服务器的连接
    *@param[in]   LPCSTR szUsrName 登陆用户名
    *@param[in]   LPCSTR szPwd     密码
    *@param[in]   LPCSTR szDBName  数据库名
    *@param[in]   LPCSTR szSrvIP   数据库服务器地址(字符串IP)
    *@param[in]   u16 wPort        端口
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note        该接口不能在数据库的错误处理函数中调用
    */
    BOOL32 ConnecttoSrv(LPCSTR szUsrName, LPCSTR szPwd,
        LPCSTR szDBName, LPCSTR szSrvIP, u16 wPort = 5000);

    /**
    *@brief       断开到数据库服务器的连接
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 CloseConnection();

    /**
    *@brief       安装客户端错误消息处理函数
    *@param[in]   DBCbFunc CbFunc 函数入口指针
    *@param[in]   void *pContext  回调上下文
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 SetupCtErrFuc(DBCbFunc CbFunc, void *pContext = NULL);

    /**
    *@brief       安装服务器端错误消息处理函数
    *@param[in]   DBCbFunc CbFunc 函数入口指针
    *@param[in]   void *pContext  回调上下文
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 SetupSrvErrFuc(DBCbFunc CbFunc, void *pContext = NULL);

    /**
    *@brief       移除该连接上的回调信息
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    void RemoveCallBack();

    /**
    *@brief       打开数据库
    *@param[in]   LPCSTR szDBName 目标数据库名
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note        只有mysql数据库有效
    */
    BOOL32 UseDB(LPCSTR szDBName);

    /**
    *@brief       执行命令类sql语句
    *@param[in]   LPCSTR szsql  要执行的sql命令字串
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note        sql语句末尾不能有分号，多语句执行时各个语句之间要用分号隔开，
    *             但是最后一条语句末尾不能带分号
    */
    BOOL32 ExecSql(LPCSTR szsql);

    /**
    *@brief       执行命令类sql语句
    *@param[in]   LPCSTR szsql           要执行的sql命令字串
    *@param[out]  CDBRsImp *pcRecordSet  执行sql命令的结果集指针
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note        sql语句末尾不能有分号，多语句执行时各个语句之间要用分号隔开，
    *             但是最后一条语句末尾不能带分号
    */
    BOOL32 ExecSql(LPCSTR szsql, CDBRs *pcRecordSet);

    /**
    *@brief       插入用户自定义的非结构化数据到数据库表中
    *@param[in]   LPCSTR lpszSql         要执行的sql字串
    *@param[in]   const void *pBuf       缓冲区首地址
    *@param[in]   s32 sdwBufLen          缓冲区长度
    *@param[out]  CDBRsImp *pcRecordSet  结果集指针
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 InsertBuf(LPCSTR lpszSql, CDBRs *pcRecordSet, const void *pBuf, s32 sdwBufLen);

    /**
    *@brief       处理结果集(适用于处理多个记录集返回)
    *@param[in]   BOOL32 bIsClear 是否清除当前结果集
    *@return      EOpCode_FindRs  发现新的记录集
    *             EOpCode_Finish  结果集已处理完毕
    *             EOpCode_BindDataErr 数据邦定失败
    *             EOpCode_CmdErr  发现某条命令执行失败
    *@ref
    *@see
    *@note
    */
    EOpCode GoOnProcRes(); 

    /**
    *@brief       获取本次操作命令所影响的记录行数
    *@return      所影响的记录行数
    *@ref
    *@see
    *@note
    */
    u32 GetAffectedRowCount();

    /**
    *@brief       获取数据库句柄地址
    *@return      数据库句柄地址
    *@ref
    *@see
    *@note
    */
    CDBHandlerImp* GetHandleAddr();

protected:
    CDBHandlerImp *m_pcDBHandlerImp;  ///< 数据库句柄
};

/** 操作数据库结果集实现类*/
class CDBRsImp;
/** 操作数据库结果集接口类*/
class CDBRs
{
public:
    CDBRs();
    ~CDBRs();

public:    
    /**
    *@brief       指向结果集的下一条记录
    *@return      成功读出当前行返回true, 否则返回false
    *@ref
    *@see
    *@note
    */
    BOOL32 MoveNext();

    /**
    *@brief       判断结果集是否到达末尾
    *@return      到达末尾返回true, 否则false
    *@ref
    *@see
    *@note
    */
    BOOL32 IsEof();

    /**
    *@brief       获取结果集列数 
    *@return      结果集列数
    *@ref
    *@see
    *@note
    */
    const u16 GetFieldCount();

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  double Value      该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, double& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  float Value       该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, float& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  s64 Value         该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, s64& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  u64 Value         该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, u64& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  s32 Value         该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, s32& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  u32 Value         该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, u32& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  s16 Value         该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, s16& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  u16 Value         该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, u16& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  s8 Value          该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, s8& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[out]  u8 Value          该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, u8& Value);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[in]   u16 wBufLen       缓冲区也就是pstr的最大长度
    *@param[out]  LPSTR pstr        该列字串值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, LPSTR pstr, u16 wBufLen);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[in]   u16 wBufLen       缓冲区也就是pBuf的最大长度
    *@param[out]  void *pBuf        缓冲区, 用来存放该列字串值
    *@param[out]  u16 wOutLen       该列字串值大小
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, void *pBuf, u16 wBufLen, u16 &wOutLen);

    /**
    *@brief       获取某列值
    *@param[in]   LPCSTR lpColName  列名
    *@param[in]   s32 sdwBufLen       缓冲区也就是pBuf的最大长度
    *@param[out]  void *pBuf        缓冲区, 用来存放该列字串值
    *@param[out]  s32 sdwOutLen       该列字串值大小
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(LPCSTR lpColName, void *pBuf, s32 sdwBufLen, s32 &sdwOutLen);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex   列索引, 索引从1开始
    *@param[out]  double Value 该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, double& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex   列索引, 索引从1开始
    *@param[out]  float Value  该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, float& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex  列索引, 索引从1开始
    *@param[out]  s64 Value   该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, s64& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex  列索引, 索引从1开始
    *@param[out]  u64 Value   该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, u64& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex  列索引, 索引从1开始
    *@param[out]  s32 Value   该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, s32& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex  列索引, 索引从1开始
    *@param[out]  u32 Value   该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, u32& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex  列索引, 索引从1开始
    *@param[out]  s16 Value   该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, s16& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex  列索引, 索引从1开始
    *@param[out]  u16 Value   该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, u16& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex  列索引, 索引从1开始
    *@param[out]  s8 Value    该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, s8& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex  列索引, 索引从1开始
    *@param[out]  u8 Value    该列值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, u8& Value);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex   列索引, 索引从1开始
    *@param[in]   u16 wBufLen  缓冲区也就是pstr的最大长度
    *@param[out]  LPSTR pstr   该列字串值
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, LPSTR pstr, u16 wBufLen);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex   列索引, 索引从1开始
    *@param[in]   u16 wBufLen  缓冲区也就是pBuf的最大长度
    *@param[out]  void *pBuf   缓冲区, 用来存放该列字串值
    *@param[out]  u16 wOutLen  该列字串值大小
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, void *pBuf, u16 wBufLen, u16 &wOutLen);

    /**
    *@brief       获取某列值
    *@param[in]   u8 byIndex   列索引, 索引从1开始
    *@param[in]   s32 sdwBufLen  缓冲区也就是pBuf的最大长度
    *@param[out]  void *pBuf   缓冲区, 用来存放该列字串值
    *@param[out]  s32 sdwOutLen  该列字串值大小
    *@return      成功true, 失败false
    *@ref
    *@see
    *@note
    */
    BOOL32 GetFieldValue(u8 byIndex, void *pBuf, s32 sdwBufLen, s32 &sdwOutLen);

    /**
    *@brief       获取数据库结果集句柄地址
    *@return      数据库结果集句柄地址
    *@ref
    *@see
    *@note
    */
    CDBRsImp* GetRsImp();

protected:
    CDBRsImp *m_pcDBRsImp;  ///< 数据库结果集句柄
};

#endif
