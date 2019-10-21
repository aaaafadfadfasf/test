#ifndef CONVERT_XML_TO_STRUCT_H
#define CONVERT_XML_TO_STRUCT_H

#include <string>
#include <sstream>
#include <limits>
#include "cms/cmsassert.h"
#include "system/tinyxml/tinyxml.h"
#include "adpfromstdsettovector.h"

using namespace std;
namespace ConvertHelper{
namespace ToStruct{

namespace Private{

static inline const std::string DoParseValue(const TiXmlNode& theNode){
    const TiXmlNode *ptContent = theNode.FirstChild();
    if(!ptContent)
    {
        //CMS_ASSERT(0&&"Field Content is empty");
        return "";
    }
    return ptContent->Value();
}

static inline int GetContainerSize(const TiXmlElement& theNode){
    int count = 0;
    theNode.Attribute("size",&count);
    return count;
}

static inline int GetContainerSizeGBStyle(const TiXmlElement& theNode){
    int count = 0;
    theNode.Attribute("Num",&count);
    return count;
}

static inline int GetContainerSizeMTStyle(const TiXmlElement& theNode){
	int count = 0;
	theNode.Attribute("num",&count);
	return count;
}

/////////////////////////////////
// template <class T>
// T StringToInt(const char * pStr)
// {
// 
// }

template <typename Type>
void DoParsePrimitiveChildNode(const TiXmlNode& theParent,const std::string& szFieldName,Type& output){
    const TiXmlNode* pElem = theParent.FirstChild(szFieldName);
    if(!pElem)
    {
        return;
    }
    const std::string szValue = Private::DoParseValue(*pElem);

	std::stringstream ssParser;
	ssParser.precision(18);
	ssParser << szValue;
	ssParser >> output;

    // output = static_cast<Type>(atol(szValue.c_str()));
}

//CTransparentData
template <>
inline void DoParsePrimitiveChildNode<CTransparentData>(const TiXmlNode& theParent,const std::string& szFieldName, CTransparentData& output){
    const TiXmlNode* pElem = theParent.FirstChild(szFieldName);
    if(!pElem)
    {
        return;
    }
    output = Private::DoParseValue(*pElem);
}

template <>
inline void DoParsePrimitiveChildNode<std::string>(const TiXmlNode& theParent,const std::string& szFieldName, std::string& output){
    const TiXmlNode* pElem = theParent.FirstChild(szFieldName);
    if(!pElem)
    {
        return;
    }
    output = Private::DoParseValue(*pElem);
}

template <>
inline void DoParsePrimitiveChildNode<bool>(const TiXmlNode& theParent,const std::string& szFieldName, bool& output){
	const TiXmlNode* pElem = theParent.FirstChild(szFieldName);
	if(!pElem)
	{
		return;
	}
	const std::string szValue = Private::DoParseValue(*pElem);
    std::stringstream ssParser;
    ssParser << szValue;
    ssParser >> output;
	//output = (0 != (atol(szValue.c_str())));
}



template <typename Type>
void DoParsePrimitiveCurNode(const TiXmlNode& theNode,Type& output){
    const std::string szValue = Private::DoParseValue(theNode);
    //output = static_cast<Type>(atol(szValue.c_str()));
    std::stringstream ssParser;
	ssParser.precision(18);
    ssParser << szValue;
    ssParser >> output;
}

template <>
inline void DoParsePrimitiveCurNode<CTransparentData>(const TiXmlNode& theNode, CTransparentData& output){
    output = Private::DoParseValue(theNode);
}

template <>
inline void DoParsePrimitiveCurNode<std::string>(const TiXmlNode& theNode, std::string& output){
    output = Private::DoParseValue(theNode);
}

template <>
inline void DoParsePrimitiveCurNode<bool>(const TiXmlNode& theNode, bool& output){
    const std::string szValue = Private::DoParseValue(theNode);
    //output = (0 != atol(szValue.c_str()));
    std::stringstream ssParser;
    ssParser << szValue;
    ssParser >> output;
}

} //end of namespace Private



//////////////////////////////////////////////////////////////////
// parse child node, node is nonprimitive by default.
template <typename Type>
void ParseChildNode(const TiXmlNode& theParent,const std::string& szFieldName,Type& output){
    const TiXmlNode* pElem = theParent.FirstChild(szFieldName);
    if(!pElem)
    {
        return;
    }
    output.FromXml(*pElem);
}

//对CTransparentData做特殊处理
template <>
inline void ParseChildNode<CTransparentData>(const TiXmlNode& theParent,const std::string& szFieldName,CTransparentData& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<unsigned char>(const TiXmlNode& theParent,const std::string& szFieldName,unsigned char& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<signed char>(const TiXmlNode& theParent,const std::string& szFieldName,signed char& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<unsigned short>(const TiXmlNode& theParent,const std::string& szFieldName,unsigned short& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<signed short>(const TiXmlNode& theParent,const std::string& szFieldName,signed short& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<unsigned int>(const TiXmlNode& theParent,const std::string& szFieldName,unsigned int& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<signed int>(const TiXmlNode& theParent,const std::string& szFieldName,signed int& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<unsigned long>(const TiXmlNode& theParent,const std::string& szFieldName,unsigned long& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<signed long>(const TiXmlNode& theParent,const std::string& szFieldName,signed long& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<std::string>(const TiXmlNode& theParent,const std::string& szFieldName,std::string& output){
    Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<bool>(const TiXmlNode& theParent,const std::string& szFieldName,bool& output){
	Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<float>(const TiXmlNode& theParent,const std::string& szFieldName,float& output){
	Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}

template <>
inline void ParseChildNode<double>(const TiXmlNode& theParent,const std::string& szFieldName,double& output){
	Private::DoParsePrimitiveChildNode(theParent,szFieldName,output);
}
////////////////////////////////////////////////////////////
//parse current node
template <typename Type>
void ParseCurNode(const TiXmlNode& xmlNode,Type& output){
    output.FromXml(xmlNode);
}

template <>
inline void ParseCurNode<CTransparentData>(const TiXmlNode& xmlNode,CTransparentData& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}


template <>
inline void ParseCurNode<unsigned char>(const TiXmlNode& xmlNode,unsigned char& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<signed char>(const TiXmlNode& xmlNode,signed char& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<unsigned short>(const TiXmlNode& xmlNode,unsigned short& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<signed short>(const TiXmlNode& xmlNode,signed short& output){ 
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}


template <>
inline void ParseCurNode<unsigned int>(const TiXmlNode& xmlNode,unsigned int& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<signed int>(const TiXmlNode& xmlNode,signed int& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<unsigned long>(const TiXmlNode& xmlNode,unsigned long& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<signed long>(const TiXmlNode& xmlNode,signed long& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<std::string>(const TiXmlNode& xmlNode,std::string& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<bool>(const TiXmlNode& xmlNode,bool& output){
    Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<float>(const TiXmlNode& xmlNode,float& output){
	Private::DoParsePrimitiveCurNode(xmlNode,output);
}

template <>
inline void ParseCurNode<double>(const TiXmlNode& xmlNode,double& output){
	Private::DoParsePrimitiveCurNode(xmlNode,output);
}
////list,vector,set
template <typename ContainerType>
void ParseContainer(const TiXmlNode& xmlNode,
        ContainerType& container,
        const std::string& szFieldName,
        const std::string& szElemTypeName){
    const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
    if(!pNode){
        return;
    }
    int count = Private::GetContainerSize(*pNode);
    if(count <= 0){
        return;
    }
    const TiXmlNode* pItem = pNode->FirstChild("item");
    for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
        if(!pItem){
            break;
        }
        typename ContainerType::value_type v;
        ParseCurNode(*pItem,v);
        container.push_back(v);
    }
}

//国标格式的xml容器解析
template <typename ContainerType>
void ParseContainerGBStyle(const TiXmlNode& xmlNode,
                            ContainerType& container,
                            const std::string& szFieldName,
                            const std::string& szElemTypeName)
{
    const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
    if(!pNode){
        return;
    }
    int count = Private::GetContainerSizeGBStyle(*pNode);
    if(count <= 0){
        return;
    }
    const TiXmlNode* pItem = pNode->FirstChild("Item");
    for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
        if(!pItem){
            break;
        }
        typename ContainerType::value_type v;
        ParseCurNode(*pItem,v);
        container.push_back(v);
    }
}

//MTS格式的xml容器解析
template <typename ContainerType>
void ParseContainerMTStyle(const TiXmlNode& xmlNode,
	ContainerType& container,
	const std::string& szFieldName,
	const std::string& szElemTypeName)
{
	const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
	if(!pNode){
		return;
	}
	int count = Private::GetContainerSizeMTStyle(*pNode);
	if(count <= 0){
		return;
	}
	const TiXmlNode* pItem = pNode->FirstChild("item");
	for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
		if(!pItem){
			break;
		}
		typename ContainerType::value_type v;
		ParseCurNode(*pItem,v);
		container.push_back(v);
	}
}

////list,vector,set
template <typename ContainerType>
void ParseContainer(const TiXmlNode& xmlNode,
    ContainerType& container,
    const std::string& szElemTypeName){

        const TiXmlElement*  pNode = xmlNode.ToElement();
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }

        const TiXmlNode* pItem = pNode->FirstChild("item");
        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }
            typename ContainerType::value_type v;
            ParseCurNode(*pItem,v);
            container.push_back(v);
        }
}


////list,vector,set
template <typename ContainerType>
void ParseMapInContainer(const TiXmlNode& xmlNode,
    ContainerType& container,
    const std::string& szFieldName,
    const std::string& szElemTypeName){
        const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        const TiXmlNode* pItem = pNode->FirstChild("item");

        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }
            
            typename ContainerType::value_type adp_Map;
            //ContainerType::value_type adp_Map;
            ParseMap(*pItem, adp_Map);
            
            container.push_back(adp_Map);
        }
}

////list,vector,set
template <typename ContainerType>
void ParseSetInContainer(const TiXmlNode& xmlNode,
    ContainerType& container,
    const std::string& szFieldName,
    const std::string& szElemTypeName){
        const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        const TiXmlNode* pItem = pNode->FirstChild("item");

        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }

            typename ContainerType::value_type adp_Set;
            CAdpFromStdSetToVector< typename ContainerType::value_type::value_type > adp_resVec = CreateAdpFromStdSetToVector( adp_Set );
            
            ParseContainer(*pItem, adp_resVec, szElemTypeName);

            container.push_back(adp_Set);
        }
}

////list,vector,set
template <typename ContainerType>
void ParseSetInContainer(const TiXmlNode& xmlNode,
    ContainerType& container,
    const std::string& szElemTypeName){

        const TiXmlElement* pNode = xmlNode.ToElement();

        const TiXmlNode* pItem = pNode->FirstChild("item");
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }

            typename ContainerType::value_type adp_Set;
            CAdpFromStdSetToVector<typename ContainerType::value_type::value_type > adp_resVec = CreateAdpFromStdSetToVector( adp_Set );

            ParseContainer(*pItem, adp_resVec, "Container", szElemTypeName);

            container.push_back(adp_Set);
        }
}

////list,vector,set
template <typename ContainerType>
void ParseContainerInContainer(const TiXmlNode& xmlNode,
    ContainerType& container,
    const std::string& szFieldName,
    const std::string& szElemTypeName){
        const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        const TiXmlNode* pItem = pNode->FirstChild("item");

        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }
            
            typename ContainerType::value_type adp_Vec;
            ParseContainer(*pItem, adp_Vec, szElemTypeName);
 
            container.push_back(adp_Vec);
        }
}
////
template <typename Type>
void ParseArray(const TiXmlNode& xmlNode,
        Type* pArray,
        size_t nLength,
        const std::string& szFieldName,
        const std::string& szElemTypeName){
    const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
    if(!pNode){
        return;
    }
    
    int count = Private::GetContainerSize(*pNode);
    if(count <= 0){
        return;
    }
    
    const TiXmlNode* pItem = pNode->FirstChild("item");
    for(size_t index = 0;index < nLength;++index,pItem = pItem->NextSibling()){
        if(!pItem){
            break;
        }
        
        Type& t = pArray[index];
        ParseCurNode(*pItem,t);
    }
}

////char*
template <>
inline void ParseArray(const TiXmlNode& xmlNode,
        char* pArray,
        size_t nLength,
        const std::string& szFieldName,
        const std::string& szElemTypeName){
    std::string szValue;
    ParseChildNode(xmlNode,szFieldName,szValue);
    strncpy(pArray,szValue.c_str(),nLength);
    pArray[nLength - 1] = 0;
}

template <typename ContainerType>
void ParseMap(const TiXmlNode& xmlNode,
    ContainerType& container,
    const char* szFieldName,
    const char* szKeyTypeName,
    const char* szValueTypeName
    ){
        const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        const TiXmlNode* pItem = pNode->FirstChild("item");
        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }

            const TiXmlNode* pKeyNode = pItem->FirstChild("key");
            if(!pKeyNode){
                continue;
            }
            typename ContainerType::key_type theKey;
            ParseCurNode(*pKeyNode,theKey);

            const TiXmlNode* pValueNode = pItem->FirstChild("value");
            if(!pValueNode){
                continue;
            }
            typename ContainerType::value_type::second_type theValue;
            ParseCurNode(*pValueNode,theValue);
            container.insert(std::make_pair(theKey,theValue));
        }
}

template <typename ContainerType>
void ParseMap(const TiXmlNode& xmlNode,
    ContainerType& container,
    const char* szFieldName
    ){
        const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        const TiXmlNode* pItem = pNode->FirstChild("item");
        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }

            const TiXmlNode* pKeyNode = pItem->FirstChild("key");
            if(!pKeyNode){
                continue;
            }
            typename ContainerType::key_type theKey;
            ParseCurNode(*pKeyNode,theKey);

            const TiXmlNode* pValueNode = pItem->FirstChild("value");
            if(!pValueNode){
                continue;
            }
            typename ContainerType::value_type::second_type theValue;
            ParseCurNode(*pValueNode,theValue);
            container.insert(std::make_pair(theKey,theValue));
        }
}

template <typename ContainerType>
void ParseMap(const TiXmlNode& xmlNode,
    ContainerType& container
    ){
        const TiXmlElement* pNode = xmlNode.ToElement();
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }

        const TiXmlNode* pItem = pNode->FirstChild("item");
        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }

            const TiXmlNode* pKeyNode = pItem->FirstChild("key");
            if(!pKeyNode){
                continue;
            }
            typename ContainerType::key_type theKey;
            ParseCurNode(*pKeyNode,theKey);

            const TiXmlNode* pValueNode = pItem->FirstChild("value");
            if(!pValueNode){
                continue;
            }
            typename ContainerType::value_type::second_type theValue;
            ParseCurNode(*pValueNode,theValue);
            container.insert(std::make_pair(theKey,theValue));
        }
}


template <typename ContainerType>
void ParseSetInMap(const TiXmlNode& xmlNode,
    ContainerType& container,
    const char* szFieldName,
    const char* szKeyTypeName,
    const char* szValueTypeName
    ){
        const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        const TiXmlNode* pItem = pNode->FirstChild("item");

        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }

            const TiXmlNode* pKeyNode = pItem->FirstChild("key");
            if(!pKeyNode){
                continue;
            }
            typename ContainerType::key_type theKey;
            ParseCurNode(*pKeyNode,theKey);

            typename ContainerType::value_type::second_type adp_resSet;           
            
            CAdpFromStdSetToVector<typename ContainerType::value_type::second_type::value_type > adp_resVec = CreateAdpFromStdSetToVector( adp_resSet );
            ParseContainer(*pItem, adp_resVec , "value", szValueTypeName);
            container.insert(std::make_pair(theKey,adp_resSet));
        }
}

template <typename ContainerType>
void ParseMapInMap(const TiXmlNode& xmlNode,
    ContainerType& container,
    const char* szFieldName,
    const char* szKeyTypeName,
    const char* szValueTypeName
    ){
        const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        const TiXmlNode* pItem = pNode->FirstChild("item");
 
        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }

            const TiXmlNode* pKeyNode = pItem->FirstChild("key");
            if(!pKeyNode){
                continue;
            }
            typename ContainerType::key_type theKey;
            ParseCurNode(*pKeyNode,theKey);

            typename ContainerType::value_type::second_type adp_Map;
            ParseMap(*pItem, adp_Map, "value");
      
            container.insert(std::make_pair(theKey,adp_Map));
        }
}

template <typename ContainerType>
void ParseContainerInMap(const TiXmlNode& xmlNode,
    ContainerType& container,
    const char* szFieldName,
    const char* szKeyTypeName,
    const char* szValueTypeName
    ){
        const TiXmlElement* pNode = dynamic_cast<const TiXmlElement*>(xmlNode.FirstChild(szFieldName));
        if(!pNode){
            return;
        }
        int count = Private::GetContainerSize(*pNode);
        if(count <= 0){
            return;
        }
        const TiXmlNode* pItem = pNode->FirstChild("item");

        for(size_t index = 0; index < static_cast<size_t>(count); ++index,pItem = pItem->NextSibling()){
            if(!pItem){
                break;
            }

            const TiXmlNode* pKeyNode = pItem->FirstChild("key");
            if(!pKeyNode){
                continue;
            }
            typename ContainerType::key_type theKey;
            ParseCurNode(*pKeyNode,theKey);
           
            typename ContainerType::value_type::second_type adp_Vec; 
            ParseContainer(*pItem, adp_Vec, "value", szValueTypeName);

            container.insert(std::make_pair(theKey,adp_Vec));
        }
}

}//end of ToStruct namespace
}//end of converthelper namespace

#endif
//end of file
