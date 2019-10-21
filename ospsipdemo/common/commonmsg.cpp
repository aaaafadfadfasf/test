/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : commonmsg.cpp
   相关文件    : 
   文件实现功能: 公共头文件的xml_struct转换实现
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
******************************************************************************/
#include "commonmsg.h"
#include "system/converthelper/toxml.h"
#include "system/converthelper/tostruct.h"
#include "system/converthelper/adpfromstdsettovector.h"

const std::string CEventReq::ToXml()const
{
    TiXmlElement xmlNode("CEventReq");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CEventReq::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CEventReq");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CEventReq::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{

    if(!event.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"event","string",event);
    }

    ConvertHelper::ToXml::AddChildNode(xmlNode,"seqNum","int",seqNum);

    if(!session.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"session","string",session);
    }
}

void CEventReq::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CEventReq");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CEventReq::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CEventReq")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"event",event);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"seqNum",seqNum);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"session",session);
}


const std::string CEventRsp::ToXml()const
{
    TiXmlElement xmlNode("CEventRsp");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CEventRsp::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CEventRsp");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CEventRsp::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{

    if(!event.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"event","string",event);
    }

    ConvertHelper::ToXml::AddChildNode(xmlNode,"seqNum","int",seqNum);

    if(!session.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"session","string",session);
    }

    ConvertHelper::ToXml::AddChildNode(xmlNode,"errorCode","int",errorCode);
}

void CEventRsp::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CEventRsp");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CEventRsp::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CEventRsp")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"event",event);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"seqNum",seqNum);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"session",session);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"errorCode",errorCode);
}


const std::string CRegReq::ToXml()const
{
    TiXmlElement xmlNode("CRegReq");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CRegReq::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CRegReq");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CRegReq::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventReq = new TiXmlElement("CEventReq");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventReq);
    CEventReq::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventReq);

    if(!userName.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"userName","string",userName);
    }

    if(!passWord.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"passWord","string",passWord);
    }

    if(!clientURI.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"clientURI","string",clientURI);
    }
}

void CRegReq::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CRegReq");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CRegReq::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CRegReq")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventReq = xmlNode.FirstChild("CEventReq");
    CEventReq::FromXml(*pBaseClassNodeOfCEventReq);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"userName",userName);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"passWord",passWord);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"clientURI",clientURI);
}


const std::string CRegRsp::ToXml()const
{
    TiXmlElement xmlNode("CRegRsp");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CRegRsp::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CRegRsp");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CRegRsp::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventRsp = new TiXmlElement("CEventRsp");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventRsp);
    CEventRsp::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventRsp);
}

void CRegRsp::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CRegRsp");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CRegRsp::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CRegRsp")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventRsp = xmlNode.FirstChild("CEventRsp");
    CEventRsp::FromXml(*pBaseClassNodeOfCEventRsp);
}


const std::string CSendMsgReq::ToXml()const
{
    TiXmlElement xmlNode("CSendMsgReq");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CSendMsgReq::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CSendMsgReq");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CSendMsgReq::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventReq = new TiXmlElement("CEventReq");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventReq);
    CEventReq::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventReq);

    if(!msg.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"msg","string",msg);
    }
}

void CSendMsgReq::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CSendMsgReq");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CSendMsgReq::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CSendMsgReq")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventReq = xmlNode.FirstChild("CEventReq");
    CEventReq::FromXml(*pBaseClassNodeOfCEventReq);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"msg",msg);
}


const std::string CSendMsgRsp::ToXml()const
{
    TiXmlElement xmlNode("CSendMsgRsp");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CSendMsgRsp::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CSendMsgRsp");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CSendMsgRsp::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventRsp = new TiXmlElement("CEventRsp");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventRsp);
    CEventRsp::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventRsp);
}

void CSendMsgRsp::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CSendMsgRsp");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CSendMsgRsp::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CSendMsgRsp")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventRsp = xmlNode.FirstChild("CEventRsp");
    CEventRsp::FromXml(*pBaseClassNodeOfCEventRsp);
}


const std::string CUnRegReq::ToXml()const
{
    TiXmlElement xmlNode("CUnRegReq");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CUnRegReq::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CUnRegReq");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CUnRegReq::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventReq = new TiXmlElement("CEventReq");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventReq);
    CEventReq::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventReq);
}

void CUnRegReq::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CUnRegReq");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CUnRegReq::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CUnRegReq")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventReq = xmlNode.FirstChild("CEventReq");
    CEventReq::FromXml(*pBaseClassNodeOfCEventReq);
}


const std::string CUnRegRsp::ToXml()const
{
    TiXmlElement xmlNode("CUnRegRsp");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CUnRegRsp::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CUnRegRsp");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CUnRegRsp::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventRsp = new TiXmlElement("CEventRsp");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventRsp);
    CEventRsp::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventRsp);
}

void CUnRegRsp::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CUnRegRsp");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CUnRegRsp::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CUnRegRsp")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventRsp = xmlNode.FirstChild("CEventRsp");
    CEventRsp::FromXml(*pBaseClassNodeOfCEventRsp);
}


const std::string CSubscribeReq::ToXml()const
{
    TiXmlElement xmlNode("CSubscribeReq");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CSubscribeReq::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CSubscribeReq");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CSubscribeReq::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventReq = new TiXmlElement("CEventReq");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventReq);
    CEventReq::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventReq);
}

void CSubscribeReq::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CSubscribeReq");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CSubscribeReq::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CSubscribeReq")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventReq = xmlNode.FirstChild("CEventReq");
    CEventReq::FromXml(*pBaseClassNodeOfCEventReq);
}


const std::string CSubscribeRsp::ToXml()const
{
    TiXmlElement xmlNode("CSubscribeRsp");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CSubscribeRsp::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CSubscribeRsp");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CSubscribeRsp::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventRsp = new TiXmlElement("CEventRsp");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventRsp);
    CEventRsp::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventRsp);
}

void CSubscribeRsp::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CSubscribeRsp");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CSubscribeRsp::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CSubscribeRsp")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventRsp = xmlNode.FirstChild("CEventRsp");
    CEventRsp::FromXml(*pBaseClassNodeOfCEventRsp);
}


const std::string CUnSubscribeReq::ToXml()const
{
    TiXmlElement xmlNode("CUnSubscribeReq");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CUnSubscribeReq::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CUnSubscribeReq");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CUnSubscribeReq::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventReq = new TiXmlElement("CEventReq");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventReq);
    CEventReq::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventReq);
}

void CUnSubscribeReq::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CUnSubscribeReq");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CUnSubscribeReq::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CUnSubscribeReq")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventReq = xmlNode.FirstChild("CEventReq");
    CEventReq::FromXml(*pBaseClassNodeOfCEventReq);
}


const std::string CUnSubscribeRsp::ToXml()const
{
    TiXmlElement xmlNode("CUnSubscribeRsp");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CUnSubscribeRsp::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CUnSubscribeRsp");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CUnSubscribeRsp::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventRsp = new TiXmlElement("CEventRsp");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventRsp);
    CEventRsp::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventRsp);
}

void CUnSubscribeRsp::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CUnSubscribeRsp");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CUnSubscribeRsp::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CUnSubscribeRsp")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventRsp = xmlNode.FirstChild("CEventRsp");
    CEventRsp::FromXml(*pBaseClassNodeOfCEventRsp);
}


const std::string CBroadcastMsgNtfReq::ToXml()const
{
    TiXmlElement xmlNode("CBroadcastMsgNtfReq");
    ToXmlWithoutClassNameNode(xmlNode);
    TiXmlPrinter printer;
    xmlNode.Accept(&printer);
    return printer.CStr();
}
void CBroadcastMsgNtfReq::ToXml(TiXmlDocument& xmlDoc)const
{
    TiXmlElement* pclass_nameNode = new TiXmlElement("CBroadcastMsgNtfReq");
    xmlDoc.LinkEndChild(pclass_nameNode);
    ToXmlWithoutClassNameNode(*pclass_nameNode);
}
void CBroadcastMsgNtfReq::ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
{
    TiXmlElement* pBaseClassNodeOfCEventReq = new TiXmlElement("CEventReq");
    xmlNode.LinkEndChild(pBaseClassNodeOfCEventReq);
    CEventReq::ToXmlWithoutClassNameNode(*pBaseClassNodeOfCEventReq);

    if(!msg.empty())
    {
        ConvertHelper::ToXml::AddChildNode(xmlNode,"msg","string",msg);
    }
}

void CBroadcastMsgNtfReq::FromXml(const std::string& xml_string)
{
    TiXmlDocument xmlNode;
    xmlNode.Parse(xml_string.c_str());
    TiXmlNode* pRoot = xmlNode.FirstChild("CBroadcastMsgNtfReq");
    if(!pRoot)
    {
        CMS_ASSERT(0&&"error xml format, not root of class");
        return;
    }
    FromXml(*pRoot);
}
void CBroadcastMsgNtfReq::FromXml(const TiXmlNode& xmlNode)
{
    if(xmlNode.ValueStr() != "CBroadcastMsgNtfReq")
    {
        CMS_ASSERT(0 && "error xmlNode not my class");
        return;
    }

    const TiXmlNode* pBaseClassNodeOfCEventReq = xmlNode.FirstChild("CEventReq");
    CEventReq::FromXml(*pBaseClassNodeOfCEventReq);

    ConvertHelper::ToStruct::ParseChildNode(xmlNode,"msg",msg);
}


//end of file
