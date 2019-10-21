#ifndef _XML_H_
#define _XML_H_

#include "tinyxml.h"
#include <string>
using std::string;

#define XML_MSG_TYPE_NODE ("event")
#define XML_SEQ_NUM_NODE ("seqNum")
#define XML_SESSION_NODE ("session")
#define XML_ERRORCODE_NODE ("errorCode")
#define XML_QUERY_TAG_NODE ("queryTag")

/*====================================================================
������      : GetSipXmlMsgType
����        : ������XML�в���"event"Ԫ�ص��ı�ֵ
�㷨ʵ��    : GetXmlVal�����ĵڶ�����Ϊ"event"
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:event�ַ�����ʧ��:�մ�
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/10/31/             liangli
====================================================================*/
string GetSipXmlMsgType(const char* szXmlBody);
/*====================================================================
������      : GetXmlVal
����        : ������XML�в���ָ��Ԫ�ص��ı�ֵ
�㷨ʵ��    : �ݹ����
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:Element��Content��ʧ��:���ַ���
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/10/31/             liangli
====================================================================*/
string GetXmlVal(const string strXml, const string &strNodeName);

TiXmlText* GetTextNode(TiXmlElement *ptRoot, const string &strNodeName);

string GetChildXmlVal(TiXmlNode *ptParent, const string &strNodeName);

/**/
//�����������е����⣬ֻ�ܱ���childnodeһ�㣬����Ƕ�ױ�����ֻ�������ض�����
string GetXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName);
string GetXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName, const char* szParentNodeName);
/**/

bool PutXmlVal(TiXmlElement* pXmlElem, const char* szNodeVal);
bool PutXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName, const char* szNodeVal);
bool PutXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName, string strNodeVal);
bool PutXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName, const char* szParentNodeName, const char* szNodeVal);
string GetXmlStr(TiXmlDocument& xmlDoc);

/*====================================================================
������      : GetXmlKeyText
����        : ��ȡxml�ַ����еļ�ֵ
�㷨ʵ��    : �����ַ��� string strXml = "<tcp_listen_port>123</tcp_listen_port>"
             GetXmlKeyText(strXml, "<tcp_listen_port>", "</tcp_listen_port>")
             �᷵��"123"
             �ú������ʹ��tinyxml��������xml��Ҫ���ܶ࣬�����ڼ򵥻�����ʹ��
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/3/2/             liangli
====================================================================*/
string GetXmlKeyText(const string &strXml, LPCSTR lpKeyBegin, LPCSTR lpKeyEnd);
string GetXmlNodeText(const string &strXml, const string& strNodeName);

/*====================================================================
������      : ReplaceXmlKeyText
����        : �滻xml�ַ�����ĳ����ֵ
�㷨ʵ��    : �����ַ��� string strXml = "<tcp_listen_port>123</tcp_listen_port>"
            ReplaceXmlKeyText(strXml, "456", "<tcp_listen_port>", "</tcp_listen_port>");
            �᷵�� "<tcp_listen_port>456</tcp_listen_port>"
            �ú������ʹ��tinyxml��������xml��Ҫ���ܶ࣬�����ڼ򵥻�����ʹ��
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/3/5/             liangli
2012/11/24/           huangzhichun   ����֧��int����ֵ�ýӿ�
====================================================================*/
string ReplaceXmlKeyText(const string &strXml, const string &strReplaceValue, LPCSTR lpKeyBegin, LPCSTR lpKeyEnd);
string ReplaceXmlKeyText(const string &strXml, int nReplaceValue, LPCSTR lpKeyBegin, LPCSTR lpKeyEnd);


string GetSipSeqNO(const char* szXmlBody);

string GetSipSessionID(const char* szXmlBody);

string GetSipErrorcode(const char* szXmlBody);

string GetQueryTag(const char* szXmlBody);


//������ʹ�õĽӿ�  fanxg-20131202
#define GB_CMD_TYPE              "CmdType"
#define GB_SN                    "SN"
#define GB_DEVICE_ID             "DeviceID"

string GetGBMethod(const char* szXmlBody);
string GetGBCmd(const char* szXmlBody);
string GetGBSN(const char* szXmlBody);
string GetGBDeviceID(const char* szXmlBody);
bool FindGBCtrlCmdByName(const char* szXmlBody, const char* szNodeName);

#endif //#ifndef _XML_H_