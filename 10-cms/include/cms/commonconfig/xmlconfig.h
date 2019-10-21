/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : xmlconfig.h
   相关文件    : 
   文件实现功能: xmlconfig 封装xml的配置读写
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/14   1.0         liangli        创建
******************************************************************************/
#ifndef _XML_CONFIG_H_
#define _XML_CONFIG_H_
#include "kdvtype.h"
#include "system/tinyxml/xml.h"

/*
    配置文件中的KeyName在不同的Section中可以重复，但是在Section内部不允许重复
    SectionName全局都不允许重复
    所以获取KeyValue的方法是，首先根据SectionName找到Node节点
    然后在该Node节点中找唯一的KeyName
*/

class CXmlConfig
{
public:
    CXmlConfig(const string &strRootName);
    const string GetXmlString();
    void SetXmlString(const string &strXml);
public:
    void Clear();
    bool Declare();
    bool GetRegKeyInt(
                    const string &strSectionName, /* Profile中的段名   */
                    const string &strKeyName,     /* 段内的关键字名     */
                    const s32 nDefaultValue,      /* 失败时返回的默认值 */
                    s32 &nReturnValue);          /* 返回值            */
    bool GetRegKeyString(
                    const string &strSectionName, /* Profile中的段名   */
                    const string &strKeyName,     /* 段内的关键字名     */
                    const string &strDefaultValue,/* 失败时返回的默认值 */
                    string &strReturnValue        /* 返回值            */
                        );

    bool SetRegKeyInt(
                    const string &strSectionName, /* Profile中的段名   */
                    const string &strKeyName,     /* 段内的关键字名     */
                    const s32 nValue              /* 新的整数值 */
                    );
    bool SetRegKeyString(
                    const string &strSectionName, /* Profile中的段名   */
                    const string &strKeyName,     /* 段内的关键字名     */
                    const string &strValue        /* 新的字串值, NULL则表示删除该行 */
                    );
    bool DeleteRegKey(
                    const string &strSectionName, /* Profile中的段名   */
                    const string &strKeyName      /* 段内的关键字名     */
                    );
protected:
    bool GetRegKeyValue(
                    const string &strSectionName, /* Profile中的段名   */
                    const string &strKeyName,     /* 段内的关键字名     */
                    string &strReturnValue);      /* 返回值            */
    bool SetRegKeyValue(
                    const string &strSectionName, /* Profile中的段名   */
                    const string &strKeyName,     /* 段内的关键字名     */
                    const string &strValue        /* 新的值 */
                    );
protected:
    //string m_strXml;
    TiXmlDocument m_xmlDoc;
    string m_strRootName;
};



#endif


