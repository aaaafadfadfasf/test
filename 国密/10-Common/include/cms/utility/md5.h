/*****************************************************************************
   文件实现功能: MD5加密功能
   作者        : 胡志云
   版权        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人       走读人      修改记录
   2013/05/30  1.0        胡志云                   根据网上版本调整
******************************************************************************/

#ifndef MD5_H
#define MD5_H
#include <string>
using std::string;

// 计算字符串的MD5值
void MD5(const string &strEncrypt, string &strDecrypt);

#ifdef _LINUX_
    int MD5Sum(const string& strFilePath, string& strMD5Sum);
#endif
#endif