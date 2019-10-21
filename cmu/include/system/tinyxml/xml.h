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
函数名      : GetSipXmlMsgType
功能        : 在整个XML中查找"event"元素的文本值
算法实现    : GetXmlVal函数的第二参数为"event"
引用全局变量:
输入参数说明:
返回值说明  : 成功:event字符串，失败:空串
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/10/31/             liangli
====================================================================*/
string GetSipXmlMsgType(const char* szXmlBody);
/*====================================================================
函数名      : GetXmlVal
功能        : 在整个XML中查找指定元素的文本值
算法实现    : 递归遍历
引用全局变量:
输入参数说明:
返回值说明  : 成功:Element的Content，失败:空字符串
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/10/31/             liangli
====================================================================*/
string GetXmlVal(const string strXml, const string &strNodeName);

TiXmlText* GetTextNode(TiXmlElement *ptRoot, const string &strNodeName);

string GetChildXmlVal(TiXmlNode *ptParent, const string &strNodeName);

/**/
//这两个函数有点问题，只能遍历childnode一层，不能嵌套遍历，只能用于特定场合
string GetXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName);
string GetXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName, const char* szParentNodeName);
/**/

bool PutXmlVal(TiXmlElement* pXmlElem, const char* szNodeVal);
bool PutXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName, const char* szNodeVal);
bool PutXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName, string strNodeVal);
bool PutXmlVal(TiXmlDocument& xmlDoc, const char* szNodeName, const char* szParentNodeName, const char* szNodeVal);
string GetXmlStr(TiXmlDocument& xmlDoc);

/*====================================================================
函数名      : GetXmlKeyText
功能        : 获取xml字符串中的键值
算法实现    : 例如字符串 string strXml = "<tcp_listen_port>123</tcp_listen_port>"
             GetXmlKeyText(strXml, "<tcp_listen_port>", "</tcp_listen_port>")
             会返回"123"
             该函数相比使用tinyxml解析整棵xml树要轻便很多，建议在简单环境下使用
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/3/2/             liangli
====================================================================*/
string GetXmlKeyText(const string &strXml, LPCSTR lpKeyBegin, LPCSTR lpKeyEnd);
string GetXmlNodeText(const string &strXml, const string& strNodeName);

/*====================================================================
函数名      : ReplaceXmlKeyText
功能        : 替换xml字符串中某个键值
算法实现    : 例如字符串 string strXml = "<tcp_listen_port>123</tcp_listen_port>"
            ReplaceXmlKeyText(strXml, "456", "<tcp_listen_port>", "</tcp_listen_port>");
            会返回 "<tcp_listen_port>456</tcp_listen_port>"
            该函数相比使用tinyxml解析整棵xml树要轻便很多，建议在简单环境下使用
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/3/5/             liangli
2012/11/24/           huangzhichun   增加支持int类型值得接口
====================================================================*/
string ReplaceXmlKeyText(const string &strXml, const string &strReplaceValue, LPCSTR lpKeyBegin, LPCSTR lpKeyEnd);
string ReplaceXmlKeyText(const string &strXml, int nReplaceValue, LPCSTR lpKeyBegin, LPCSTR lpKeyEnd);


string GetSipSeqNO(const char* szXmlBody);

string GetSipSessionID(const char* szXmlBody);

string GetSipErrorcode(const char* szXmlBody);

string GetQueryTag(const char* szXmlBody);


//国标中使用的接口  fanxg-20131202
#define GB_CMD_TYPE              "CmdType"
#define GB_SN                    "SN"
#define GB_DEVICE_ID             "DeviceID"

string GetGBMethod(const char* szXmlBody);
string GetGBCmd(const char* szXmlBody);
string GetGBSN(const char* szXmlBody);
string GetGBDeviceID(const char* szXmlBody);
bool FindGBCtrlCmdByName(const char* szXmlBody, const char* szNodeName);

#endif //#ifndef _XML_H_