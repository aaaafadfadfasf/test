/*****************************************************************************
   模块名      : Utility
   文件名      : xml_entry_table.h
   相关文件    : 
   文件实现功能: xml配置文件表格工具
                相比之前黄至春实现的一个ini的类似功能的工具类，磁盘IO性能提高明显
   作者        : 胡志云
   版权        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人       走读人      修改记录
   2014/04/11  1.0        胡志云                   创建 
******************************************************************************/

#if !defined(_XML_ENTRY_TABLE_H)
#define _XML_ENTRY_TABLE_H

#include "cms/commonconfig/cmcfg.h"
#include "cms/commonconfig/xmlconfig.h"
#include <string>
#include <vector>
#include <cstdio>
#include "mischelper.h"
using std::string;
using std::vector;

/*====================================================================
类名        : CXmlEntryTable
功能        : 对列表配置项进行了封装，方便对记录项的操作。
模板参数    : TEntry模板类需要有定义如下方法
class TEntry
{
    string ToString() const;
    BOOL32 Parse(LPCSTR lpszEntryContent);
    bool MatchInKey(const TEntry& tEntry) const;
};
----------------------------------------------------------------------
修改记录    :
日  期         版本        修改人       修改内容
2012/04/26     1.0         黄至春       创建
====================================================================*/
template<typename TEntry>
class CXmlEntryTable
{
public:
    typedef vector<TEntry> TEntryList;
    typedef typename TEntryList::iterator Iterator;

    CXmlEntryTable()
    {
    }

    void Init(LPCSTR lpszCfgPath, const s8* lpszSectionName)
    {
        m_strCfgFilePath = lpszCfgPath;
        m_strSectionName = lpszSectionName;
        m_strXmlRoot = "XmlRoot";
        m_strEntryNum = "EntryNum";
        m_strEntryPrefix = "Entry";
    }

public:

    /*====================================================================
    函数名      : LoadEntry
    功能        : 加载配置文件中指定Section的所有记录项
    算法实现    : 
    引用全局变量: 
    输入参数说明: TEntryList& tEntryList -- 返回的记录列表
    ====================================================================*/
    s32 LoadEntry(TEntryList& tEntryList)
    {
        m_tEntryList.clear();

        string strXmlContent;
        if (!ReadFileToString(m_strCfgFilePath.c_str(), strXmlContent))
            return 1;

        CXmlConfig cXmlConfig(m_strXmlRoot);
        cXmlConfig.SetXmlString(strXmlContent);

        s32 nEntryNum = 0;
        if (!cXmlConfig.GetRegKeyInt(m_strSectionName, m_strEntryNum, 0, nEntryNum))
        {
            return 2;
        }

        string strKeyVal;
        TEntry tEntry;
        for (s32 i = 0; i < nEntryNum; ++i)
        {
            if (!cXmlConfig.GetRegKeyString(m_strSectionName, GetKeyName(i), string(), strKeyVal))
            {
                return 3;
            }

            if (!tEntry.Parse(strKeyVal.c_str()))
            {
                return 4;
            }

            tEntryList.push_back(tEntry);
        }
        
        return 0;
    }

	/*====================================================================
    函数名      : LoadEntry
    功能        : 加载配置文件中指定Section的记录项
    算法实现    : 
    引用全局变量: 
    输入参数说明: TEntryList& tEntryList -- 返回的记录列表
    ====================================================================*/
    s32 LoadEntrys(TEntryList& tEntryList, const s8* lpszKeyName)
	{
		m_tEntryList.clear();

		string strXmlContent;
		if (!ReadFileToString(m_strCfgFilePath.c_str(), strXmlContent))
			return 1;

		CXmlConfig cXmlConfig(m_strXmlRoot);
		cXmlConfig.SetXmlString(strXmlContent);

		string strKeyVal;
		TEntry tEntry;
		string strKeyName = lpszKeyName;
		if (!cXmlConfig.GetRegKeyString(m_strSectionName, strKeyName, string(), strKeyVal))
		{
			return 3;
		}

		if (!tEntry.Parse(strKeyVal.c_str()))
		{
			return 4;
		}

		tEntryList.push_back(tEntry);

		return 0;
	}


    s32 LoadEntry()
    {
        return LoadEntry(m_tEntryList);
    }

	s32 LoadEntrys(const s8* lpszKeyName)
	{
		return LoadEntrys(m_tEntryList, lpszKeyName);
	}

    s32 SyncEntry()
    {
        string strXmlContent;
        if (!ReadFileToString(m_strCfgFilePath.c_str(), strXmlContent))
        {
            return 1;
        }

        CXmlConfig cXmlConfig(m_strXmlRoot);
        cXmlConfig.SetXmlString(strXmlContent);

        if (!cXmlConfig.SetRegKeyInt(GetSectionName(), m_strEntryNum, (s32)m_tEntryList.size()))
        {
            return 2;
        }
        Iterator itr = m_tEntryList.begin();
        s32 nIndex = 0;
        for(; itr != m_tEntryList.end(); itr++, ++nIndex)
        {
            if (!cXmlConfig.SetRegKeyString(GetSectionName(), 
                GetKeyName(nIndex), itr->ToString()))
            {
                return 3;
            }
        }
        
        if (!WriteFileFromString(m_strCfgFilePath.c_str(), cXmlConfig.GetXmlString()))
        {
            return 4;
        }

        return 0;
    }

    /*====================================================================
    函数名      : SetEntry
    功能        : 修改或添加指定的记录
    算法实现    : 
    引用全局变量: 
    输入参数说明: const TEntry& tEntry -- 待修改或添加的记录
    ====================================================================*/
    s32 SetEntry(const TEntry& tEntry, bool bSync=true)
    {
        CXmlConfig cXmlConfig(m_strXmlRoot);
        if (bSync)
        {
            string strXmlContent;
            if (!ReadFileToString(m_strCfgFilePath.c_str(), strXmlContent))
            {
                return 1;
            }
            cXmlConfig.SetXmlString(strXmlContent);
        }      

        s32 nIdx =0;
        Iterator itr = FindEntry(tEntry, nIdx);
        if(itr != m_tEntryList.end())
        {
            if (bSync)
            {
                // 修改
                if (!cXmlConfig.SetRegKeyString(GetSectionName(), GetKeyName(nIdx), tEntry.ToString()))
                {
                    return 2;
                }
                if (!WriteFileFromString(m_strCfgFilePath.c_str(), cXmlConfig.GetXmlString()))
                {
                    return 3;
                }
            }

            *itr = tEntry;
        }
        else
        {
            if (bSync)
            {
                if (!cXmlConfig.SetRegKeyInt(GetSectionName(), m_strEntryNum, (s32)m_tEntryList.size()+1))
                {
                    return 4;
                }
                if (!cXmlConfig.SetRegKeyString(GetSectionName(), GetKeyName((s32)m_tEntryList.size()), tEntry.ToString()))
                {
                    return 5;
                }
                if (!WriteFileFromString(m_strCfgFilePath.c_str(), cXmlConfig.GetXmlString()))
                {
                    return 6;
                }
            }

            m_tEntryList.push_back(tEntry);
        }

        return 0;
    }

    /*====================================================================
    函数名      : DelEntry
    功能        : 删除指定的记录
    算法实现    : 
    引用全局变量: 
    输入参数说明: const TEntry& tEntry -- 待删除的记录
    ====================================================================*/
    s32 DelEntry(const TEntry& tEntry, bool bSync=true) //LPCSTR lpszKey
    {
        s32 nIdx =0;
        Iterator itr = FindEntry(tEntry, nIdx);
        if(itr == m_tEntryList.end())
        {
            return 1;
        }

        if (bSync)
        {
            CXmlConfig cXmlConfig(m_strXmlRoot);

            string strXmlContent;
            if (!ReadFileToString(m_strCfgFilePath.c_str(), strXmlContent))
            {
                return 2;
            }

            cXmlConfig.SetXmlString(strXmlContent);

            s32 nEntryNum = (s32)m_tEntryList.size();

            //把当前项之后的记录往前移
            for( s32 nLoop = nIdx+1; nLoop < nEntryNum; nLoop++)
            {
                if (!cXmlConfig.SetRegKeyString(GetSectionName(), GetKeyName(nLoop-1), m_tEntryList[nLoop].ToString()))
                {
                    return 3;
                }
            }

            //把总的记录数减一
            if (!cXmlConfig.SetRegKeyInt(GetSectionName(), m_strEntryNum, nEntryNum-1))
            {
                return 4;
            }
            //删除最后一项
            if (!cXmlConfig.DeleteRegKey(GetSectionName(), GetKeyName(nEntryNum-1)))
            {
                return 5;
            }
            if (!WriteFileFromString(m_strCfgFilePath.c_str(), cXmlConfig.GetXmlString()))
            {
                return 6;
            }
        }

        //内存记录列表中也删除相应的记录
        m_tEntryList.erase(itr);
        return 0;
    }

    /*====================================================================
    函数名      : IsEntryExist
    功能        : 判断指定的记录是否存在, 利用记录的MatchInKey比较
    算法实现    : 
    引用全局变量: 
    输入参数说明: const TEntry& tEntry -- 待判断的记录
    返回值说明  : TRUE -  存在；FALSE - 不存在
    ----------------------------------------------------------------------
    修改记录    :
    日  期         版本        修改人       修改内容
    2014/01/10     1.0         胡志云       创建
    ====================================================================*/
    bool IsEntryExist(const TEntry& tEntry)
    {
        s32 nIdx;
        FindEntry(tEntry, nIdx);
        return (nIdx != -1);
    }

    /*====================================================================
    函数名      : FillEntry
    功能        : 利用记录中的key值填充指定的记录, 利用记录的MatchInKey比较
    算法实现    : 
    引用全局变量: 
    输入参数说明: TEntry& tEntry -- 待填充的记录
    返回值说明  : TRUE -  找到并填充；FALSE - 不存在
    ----------------------------------------------------------------------
    修改记录    :
    日  期         版本        修改人       修改内容
    2014/01/14     1.0         胡志云       创建
    ====================================================================*/
    bool FillEntry(TEntry &tEntry)
    {
        s32 nIdx;
        FindEntry(tEntry, nIdx);
        if (nIdx != -1)
        {
            tEntry = m_tEntryList[nIdx];
            return true;
        }
        else
        {
            return false;
        }
    }

    TEntryList& GetEntryList()
    {
        return m_tEntryList;
    }

protected:
    Iterator FindEntry(const TEntry& tEntry, s32& nIdx)
    {
        nIdx = 0;
        Iterator itr = m_tEntryList.begin();
        for(; itr != m_tEntryList.end(); itr++, nIdx++)
        {
            //找到匹配的记录
            if(itr->MatchInKey(tEntry))
            {
                return itr;
            }
        }

        //找不到匹配的记录
        nIdx = -1;
        return m_tEntryList.end();
    }

    inline LPCSTR GetCfgFilePath() const
    {
        return m_strCfgFilePath.c_str();
    }

    inline LPCSTR GetSectionName() const
    {
        return m_strSectionName.c_str();
    }
    inline string GetKeyName(s32 EntryIndex) const
    {
        s8 szKey[100];
        sprintf(szKey, "%s%d", m_strEntryPrefix.c_str(), EntryIndex);
        szKey[sizeof(szKey)-1] = '\0';
        return szKey;
    }

protected:
    string m_strCfgFilePath;
    string m_strXmlRoot;
    string m_strEntryNum;
    string m_strEntryPrefix;
    string m_strSectionName;
    TEntryList m_tEntryList;

};//class CXmlEntryTable

#endif  //_XML_ENTRY_TABLE_H
