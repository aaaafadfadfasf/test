#ifndef CONVERT_STRUCT_TO_XML_H
#define CONVERT_STRUCT_TO_XML_H

#include <string>
#include <sstream>
#include "cms/cmsassert.h"
#include "system/tinyxml/tinyxml.h"

using namespace std;

namespace ConvertHelper{
namespace ToXml{

namespace Private{

//////////////////////////////////////////////////////////////
// set node's value, type is primitive, int, long, char, short
template <typename Type>
void DoSetPrimitiveNode(TiXmlNode& theNode,const Type& theValue){
    std::ostringstream oss;
	oss.precision(18);
    oss<<theValue;
    TiXmlText* pValue = new TiXmlText(oss.str());
    theNode.LinkEndChild(pValue);
}

/*
很奇怪的是，单测用ostringstream转换bool，结果是正确的
但是单步跟踪下来这里true结果为"204"
所以针对bool实现一个特例
*/
template <>
inline void DoSetPrimitiveNode<bool>(TiXmlNode& theNode,const bool& theValue)
{
    TiXmlText* pValue = new TiXmlText(theValue ? "1" : "0");
    theNode.LinkEndChild(pValue);
}


/////////////////////////////////////////////////////////////////////
//add a primitive node to theParent as a child
template <typename Type>
void DoAddPrimitive(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const Type& theValue){
    TiXmlElement* pElem = new TiXmlElement( szName.c_str() );
    DoSetPrimitiveNode(*pElem,theValue);
    theParent.LinkEndChild(pElem);
}
template <typename Type>
void DoAddPrimitive(TiXmlNode& theParent,const std::string& szName,const Type& theValue){
    TiXmlElement* pElem = new TiXmlElement( szName.c_str() );
    DoSetPrimitiveNode(*pElem,theValue);
    theParent.LinkEndChild(pElem);
}

//add a string node to theParent as a child
template <>
inline void DoAddPrimitive<std::string>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const std::string& theValue){
    if ( ! theValue.empty() )
    {
        TiXmlElement* pElem = new TiXmlElement( szName.c_str() );
        DoSetPrimitiveNode(*pElem,theValue);
        theParent.LinkEndChild(pElem);
    }
}
template <>
inline void DoAddPrimitive<std::string>(TiXmlNode& theParent,const std::string& szName,const std::string& theValue){
    if ( ! theValue.empty() )
    {
        TiXmlElement* pElem = new TiXmlElement( szName.c_str() );
        DoSetPrimitiveNode(*pElem,theValue);
        theParent.LinkEndChild(pElem);
    }
}

template <>
inline void DoAddPrimitive<CTransparentData>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const CTransparentData& theValue){
    if ( ! theValue.empty() )
    {
        TiXmlElement* pElem = new TiXmlElement( szName.c_str() );
        //DoSetPrimitiveNode(*pElem,theValue);
        /*
        为了防止透明数据的CDATA嵌套，添加TextNode前去掉内部所有CDATA标记
        */
        string strTempVal = theValue;
        GetNodeContent(strTempVal, "<![CDATA[", "]]>", 0, true);
        TiXmlText* pValue = new TiXmlText(strTempVal.c_str());
        pValue->SetCDATA(true);
        pElem->LinkEndChild(pValue);
        theParent.LinkEndChild(pElem);
    }
}
template <>
inline void DoAddPrimitive<CTransparentData>(TiXmlNode& theParent,const std::string& szName,const CTransparentData& theValue){
    if ( ! theValue.empty() )
    {
        TiXmlElement* pElem = new TiXmlElement( szName.c_str() );
        //DoSetPrimitiveNode(*pElem,theValue);
        /*
        为了防止透明数据的CDATA嵌套，添加TextNode前去掉内部所有CDATA标记
        */
        string strTempVal = theValue;
        GetNodeContent(strTempVal, "<![CDATA[", "]]>", 0, true);
        TiXmlText* pValue = new TiXmlText(strTempVal.c_str());
        pValue->SetCDATA(true);
        pElem->LinkEndChild(pValue);
        theParent.LinkEndChild(pElem);
    }
}

}//end of Private

////////////////////////////////////////////////////////////
//set node's value, by default, value is a NonPrimitive Type.
template <typename Type>
void SetNode(TiXmlNode& theNode,const std::string& szClassName,const Type& theValue){
    theValue.ToXmlWithoutClassNameNode(theNode);
}

template <>
inline void SetNode<CTransparentData>(TiXmlNode& theNode,const std::string& szClassName,const CTransparentData& theValue){
    //Private::DoSetPrimitiveNode(theNode,theValue);
    /*
    为了防止透明数据的CDATA嵌套，添加TextNode前去掉内部所有CDATA标记
    */
    string strTempVal = theValue;
    GetNodeContent(strTempVal, "<![CDATA[", "]]>", 0, true);
    TiXmlText* pValue = new TiXmlText(strTempVal.c_str());
    pValue->SetCDATA(true);
    theNode.LinkEndChild(pValue);
}

template <>
inline void SetNode<unsigned char>(TiXmlNode& theNode,const std::string& szClassName,const unsigned char& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<signed char>(TiXmlNode& theNode,const std::string& szClassName,const signed char& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<unsigned short>(TiXmlNode& theNode,const std::string& szClassName,const unsigned short& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<signed short>(TiXmlNode& theNode,const std::string& szClassName,const signed short& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<unsigned int>(TiXmlNode& theNode,const std::string& szClassName,const unsigned int& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<signed int>(TiXmlNode& theNode,const std::string& szClassName,const signed int& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<unsigned long>(TiXmlNode& theNode,const std::string& szClassName,const unsigned long& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<signed long>(TiXmlNode& theNode,const std::string& szClassName,const signed long& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<std::string>(TiXmlNode& theNode,const std::string& szClassName,const std::string& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<bool>(TiXmlNode& theNode,const std::string& szClassName,const bool& theValue){
    Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<float>(TiXmlNode& theNode,const std::string& szClassName,const float& theValue){
	Private::DoSetPrimitiveNode(theNode,theValue);
}

template <>
inline void SetNode<double>(TiXmlNode& theNode,const std::string& szClassName,const double& theValue){
	Private::DoSetPrimitiveNode(theNode,theValue);
}
/////////////////////////////////////////////////////////////////////
//add a node to theParent as a child, the node is non primitive by default.
template <typename Type>
void AddChildNode(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const Type& theValue){
    TiXmlElement* pMember = new TiXmlElement(szName);
    theValue.ToXmlWithoutClassNameNode(*pMember);
    theParent.LinkEndChild(pMember);
}

//对CTransparentData做特殊处理
template <>
inline void AddChildNode<CTransparentData>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const CTransparentData& theValue)
{
    if ( ! theValue.empty() )
    {
        TiXmlElement* pMember = new TiXmlElement(szName);
        /*
        为了防止透明数据的CDATA嵌套，添加TextNode前去掉内部所有CDATA标记
        */
        string strTempVal = theValue;
        GetNodeContent(strTempVal, "<![CDATA[", "]]>", 0, true);
        TiXmlText* pValue = new TiXmlText(strTempVal.c_str());
        pValue->SetCDATA(true);
        pMember->LinkEndChild(pValue);
        theParent.LinkEndChild(pMember);
    }
}

template <>
inline void AddChildNode<unsigned char>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const unsigned char& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<signed char>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const signed char& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<unsigned short>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const unsigned short& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<signed short>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const signed short& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<unsigned int>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const unsigned int& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<signed int>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const signed int& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<unsigned long>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const unsigned long& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<signed long>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const signed long& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<std::string>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const std::string& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<bool>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const bool& theValue){
    Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<float>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const float& theValue){
	Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <>
inline void AddChildNode<double>(TiXmlNode& theParent,const std::string& szName,const std::string& szClassName,const double& theValue){
	Private::DoAddPrimitive(theParent,szName,szClassName,theValue);
}

template <typename Type>
inline void AddChildNode(TiXmlNode& theParent,const std::string& szName,const Type& theValue){
    TiXmlElement* pMember = new TiXmlElement(szName);
    theValue.ToXmlWithoutClassNameNode(*pMember);
    theParent.LinkEndChild(pMember);
}

template <>
inline void AddChildNode<CTransparentData>(TiXmlNode& theParent,const std::string& szName,const CTransparentData& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<unsigned char>(TiXmlNode& theParent,const std::string& szName,const unsigned char& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<signed char>(TiXmlNode& theParent,const std::string& szName,const signed char& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<unsigned short>(TiXmlNode& theParent,const std::string& szName,const unsigned short& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<signed short>(TiXmlNode& theParent,const std::string& szName,const signed short& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<unsigned int>(TiXmlNode& theParent,const std::string& szName,const unsigned int& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<signed int>(TiXmlNode& theParent,const std::string& szName,const signed int& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<unsigned long>(TiXmlNode& theParent,const std::string& szName,const unsigned long& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<signed long>(TiXmlNode& theParent,const std::string& szName,const signed long& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<std::string>(TiXmlNode& theParent,const std::string& szName,const std::string& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<bool>(TiXmlNode& theParent,const std::string& szName,const bool& theValue){
    Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<float>(TiXmlNode& theParent,const std::string& szName,const float& theValue){
	Private::DoAddPrimitive(theParent,szName,theValue);
}

template <>
inline void AddChildNode<double>(TiXmlNode& theParent,const std::string& szName,const double& theValue){
	Private::DoAddPrimitive(theParent,szName,theValue);
}
////list,vector,set 增加一个Node层次，szElemTypeName实际上没用
template <typename ContainerType>
void AddContainer(TiXmlNode& theParent,
        const ContainerType& container,
        const std::string& szFieldName,
        const std::string& szElemTypeName){
    size_t count = container.size();
    if(count == 0)
    {
        return;
    }

    TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
    theParent.LinkEndChild(pContainerNode);


    pContainerNode->SetAttribute("size",count);
    typename ContainerType::const_iterator iter = container.begin();
    typename ContainerType::const_iterator end  = container.end();
    for(size_t index = 0; iter != end; ++iter,++index){
        TiXmlElement* pElem = new TiXmlElement("item");
        pElem->SetAttribute("index",index);
        pContainerNode->LinkEndChild(pElem);        
        SetNode(*pElem,szElemTypeName,*iter);
    }
}

//国标格式的xml生成样式
template <typename ContainerType>
void AddContainerGBStyle(TiXmlNode& theParent,
                        const ContainerType& container,
                        const std::string& szFieldName,
                        const std::string& szElemTypeName)
{
    size_t count = container.size();
    if(count == 0)
    {
        return;
    }

    TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
    theParent.LinkEndChild(pContainerNode);


    pContainerNode->SetAttribute("Num",count);
    typename ContainerType::const_iterator iter = container.begin();
    typename ContainerType::const_iterator end  = container.end();
    for(size_t index = 0; iter != end; ++iter,++index){
        TiXmlElement* pElem = new TiXmlElement("Item");
        //pElem->SetAttribute("index",index);
        pContainerNode->LinkEndChild(pElem);        
        SetNode(*pElem,szElemTypeName,*iter);
    }
}

////list,vector,set 不增加额外层次
template <typename ContainerType>
void AddContainer(TiXmlNode& theParent,
    const ContainerType& container,
    const std::string& szElemTypeName){
        size_t count = container.size();
        if(count == 0)
        {
            return;
        }

        TiXmlElement* pContainerNode = theParent.ToElement();
        pContainerNode->SetAttribute("size",count);

        typename ContainerType::const_iterator iter = container.begin();
        typename ContainerType::const_iterator end  = container.end();
        for(size_t index = 0; iter != end; ++iter,++index){
            TiXmlElement* pElem = new TiXmlElement("item");
            pElem->SetAttribute("index",index);
            pContainerNode->LinkEndChild(pElem);        
            SetNode(*pElem,szElemTypeName,*iter);
        }
}

////list,vector,set
template <typename ContainerType>
void AddContainerInContainer(TiXmlNode& theParent,
    const ContainerType& container,
    const std::string& szFieldName,
    const std::string& szElemTypeName){
        size_t count = container.size();
        if(count == 0)
        {
            return;
        }

        TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
        theParent.LinkEndChild(pContainerNode);


        pContainerNode->SetAttribute("size",count);
        typename ContainerType::const_iterator iter = container.begin();
        typename ContainerType::const_iterator end  = container.end();
        for(size_t index = 0; iter != end; ++iter,++index){
             TiXmlElement* pElem = new TiXmlElement("item");
             pElem->SetAttribute("index",index);
             pContainerNode->LinkEndChild(pElem); 
             AddContainer(*pElem, *iter, szElemTypeName);
        }
}
////list,vector,set
template <typename ContainerType>
void AddMapInContainer(TiXmlNode& theParent,
    const ContainerType& container,
    const std::string& szFieldName,
    const std::string& szElemTypeName){
        size_t count = container.size();
        if(count == 0)
        {
            return;
        }

        TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
        theParent.LinkEndChild(pContainerNode);


        pContainerNode->SetAttribute("size",count);
        typename ContainerType::const_iterator iter = container.begin();
        typename ContainerType::const_iterator end  = container.end();
        for(size_t index = 0; iter != end; ++iter,++index){
            TiXmlElement* pElem = new TiXmlElement("item");
            pElem->SetAttribute("index",index);
            pContainerNode->LinkEndChild(pElem);        
            AddMap(*pElem,*iter);
        }
}
////array
template <typename Type>
void AddArray(TiXmlNode& theParent,
    const Type* pArray,
    size_t nLength,
    const std::string& szFieldName,
    const std::string& szElemTypeName){
        TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
        theParent.LinkEndChild(pContainerNode);
        pContainerNode->SetAttribute("size",nLength);
        for(const Type* pIndex = pArray; pIndex != pArray + nLength; ++pIndex){
            TiXmlElement* pElem = new TiXmlElement("item");
            pElem->SetAttribute("index",pIndex-pArray);
            pContainerNode->LinkEndChild(pElem);
            SetNode(*pElem,szElemTypeName,*pIndex);
        }
}

////const char*
template <>
inline void AddArray(TiXmlNode& theParent,
    const char* pArray,
    size_t nLength,
    const std::string& szFieldName,
    const std::string& szElemTypeName){
        std::string szTemp(pArray);
        if(szTemp.empty()){//no need to transport empty string
            return;
        }
        AddChildNode(theParent,szFieldName,szElemTypeName,szTemp);
}

template <typename ContainerType>
void AddMap(TiXmlNode& theParent,
    const ContainerType& container,
    const std::string& szFieldName,
    const std::string& szKeyTypeName,
    const std::string& szValueTypeName
    ){
        size_t count = container.size();
        if(count == 0)
        {
            return;
        }

        TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
        theParent.LinkEndChild(pContainerNode);


        pContainerNode->SetAttribute("size",count);
        typename ContainerType::const_iterator iter = container.begin();
        typename ContainerType::const_iterator end  = container.end();
        for(size_t index = 0; iter != end; ++iter,++index){
            TiXmlElement* pElem = new TiXmlElement("item");
            pElem->SetAttribute("index",index);
            pContainerNode->LinkEndChild(pElem);

            AddChildNode(*pElem,"key",szKeyTypeName,iter->first);
            AddChildNode(*pElem,"value",szValueTypeName,iter->second);
            }
        }

template <typename ContainerType>
void AddMap(TiXmlNode& theParent,
    const ContainerType& container,
    const std::string& szFieldName
    ){
        size_t count = container.size();
        if(count == 0)
        {
            return;
        }

        TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
        theParent.LinkEndChild(pContainerNode);

        pContainerNode->SetAttribute("size",count);
        typename ContainerType::const_iterator iter = container.begin();
        typename ContainerType::const_iterator end  = container.end();
        for(size_t index = 0; iter != end; ++iter,++index){
            TiXmlElement* pElem = new TiXmlElement("item");
            pElem->SetAttribute("index",index);
            pContainerNode->LinkEndChild(pElem);

            AddChildNode(*pElem,"key",iter->first);
            AddChildNode(*pElem,"value",iter->second);
        }
}

template <typename ContainerType>
void AddMap(TiXmlNode& theParent,
    const ContainerType& container
    ){
        size_t count = container.size();
        if(count == 0)
        {
            return;
        }

        TiXmlElement* pContainerNode = theParent.ToElement();

        pContainerNode->SetAttribute("size",count);
        typename ContainerType::const_iterator iter = container.begin();
        typename ContainerType::const_iterator end  = container.end();
        for(size_t index = 0; iter != end; ++iter,++index){
            TiXmlElement* pElem = new TiXmlElement("item");
            pElem->SetAttribute("index",index);
            pContainerNode->LinkEndChild(pElem);

            AddChildNode(*pElem,"key",iter->first);
            AddChildNode(*pElem,"value",iter->second);
        }
}

template <typename ContainerType>
void AddContainerMap(TiXmlNode& theParent,
    const ContainerType& container,
    const std::string& szFieldName,
    const std::string& szKeyTypeName,
    const std::string& szValueTypeName
    ){
        size_t count = container.size();
        if(count == 0)
        {
            return;
        }

        TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
        theParent.LinkEndChild(pContainerNode);


        pContainerNode->SetAttribute("size",count);
        typename ContainerType::const_iterator iter = container.begin();
        typename ContainerType::const_iterator end  = container.end();
        for(size_t index = 0; iter != end; ++iter,++index){
            TiXmlElement* pElem = new TiXmlElement("item");
            pElem->SetAttribute("index",index);
            pContainerNode->LinkEndChild(pElem);

            AddChildNode(*pElem,"key",szKeyTypeName,iter->first);
            AddContainer(*pElem, iter->second, "value", szValueTypeName); 
        }

}
template <typename ContainerType>
void AddMapInMap(TiXmlNode& theParent,
    const ContainerType& container,
    const std::string& szFieldName,
    const std::string& szKeyTypeName,
    const std::string& szValueTypeName
    ){
        size_t count = container.size();
        if(count == 0)
        {
            return;
        }

        TiXmlElement* pContainerNode = new TiXmlElement( szFieldName );
        theParent.LinkEndChild(pContainerNode);


        pContainerNode->SetAttribute("size",count);
        typename ContainerType::const_iterator iter = container.begin();
        typename ContainerType::const_iterator end  = container.end();
        for(size_t index = 0; iter != end; ++iter,++index){
            TiXmlElement* pElem = new TiXmlElement("item");
            pElem->SetAttribute("index",index);
            pContainerNode->LinkEndChild(pElem);

            AddChildNode(*pElem,"key",szKeyTypeName,iter->first);
            AddMap(*pElem, iter->second, "value"); 
        }

}
}//end of toxml namespace
}//end of converthelper namespace

#endif
//end of file
