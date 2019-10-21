/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : xmlconfig.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: xmlconfig ��װxml�����ö�д
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/14   1.0         liangli        ����
******************************************************************************/
#ifndef _XML_CONFIG_H_
#define _XML_CONFIG_H_
#include "kdvtype.h"
#include "system/tinyxml/xml.h"

/*
    �����ļ��е�KeyName�ڲ�ͬ��Section�п����ظ���������Section�ڲ��������ظ�
    SectionNameȫ�ֶ��������ظ�
    ���Ի�ȡKeyValue�ķ����ǣ����ȸ���SectionName�ҵ�Node�ڵ�
    Ȼ���ڸ�Node�ڵ�����Ψһ��KeyName
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
                    const string &strSectionName, /* Profile�еĶ���   */
                    const string &strKeyName,     /* ���ڵĹؼ�����     */
                    const s32 nDefaultValue,      /* ʧ��ʱ���ص�Ĭ��ֵ */
                    s32 &nReturnValue);          /* ����ֵ            */
    bool GetRegKeyString(
                    const string &strSectionName, /* Profile�еĶ���   */
                    const string &strKeyName,     /* ���ڵĹؼ�����     */
                    const string &strDefaultValue,/* ʧ��ʱ���ص�Ĭ��ֵ */
                    string &strReturnValue        /* ����ֵ            */
                        );

    bool SetRegKeyInt(
                    const string &strSectionName, /* Profile�еĶ���   */
                    const string &strKeyName,     /* ���ڵĹؼ�����     */
                    const s32 nValue              /* �µ�����ֵ */
                    );
    bool SetRegKeyString(
                    const string &strSectionName, /* Profile�еĶ���   */
                    const string &strKeyName,     /* ���ڵĹؼ�����     */
                    const string &strValue        /* �µ��ִ�ֵ, NULL���ʾɾ������ */
                    );
    bool DeleteRegKey(
                    const string &strSectionName, /* Profile�еĶ���   */
                    const string &strKeyName      /* ���ڵĹؼ�����     */
                    );
protected:
    bool GetRegKeyValue(
                    const string &strSectionName, /* Profile�еĶ���   */
                    const string &strKeyName,     /* ���ڵĹؼ�����     */
                    string &strReturnValue);      /* ����ֵ            */
    bool SetRegKeyValue(
                    const string &strSectionName, /* Profile�еĶ���   */
                    const string &strKeyName,     /* ���ڵĹؼ�����     */
                    const string &strValue        /* �µ�ֵ */
                    );
protected:
    //string m_strXml;
    TiXmlDocument m_xmlDoc;
    string m_strRootName;
};



#endif


