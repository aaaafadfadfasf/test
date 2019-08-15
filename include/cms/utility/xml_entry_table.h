/*****************************************************************************
   ģ����      : Utility
   �ļ���      : xml_entry_table.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: xml�����ļ���񹤾�
                ���֮ǰ������ʵ�ֵ�һ��ini�����ƹ��ܵĹ����࣬����IO�����������
   ����        : ��־��
   ��Ȩ        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���       �߶���      �޸ļ�¼
   2014/04/11  1.0        ��־��                   ���� 
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
����        : CXmlEntryTable
����        : ���б�����������˷�װ������Լ�¼��Ĳ�����
ģ�����    : TEntryģ������Ҫ�ж������·���
class TEntry
{
    string ToString() const;
    BOOL32 Parse(LPCSTR lpszEntryContent);
    bool MatchInKey(const TEntry& tEntry) const;
};
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾        �޸���       �޸�����
2012/04/26     1.0         ������       ����
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
    ������      : LoadEntry
    ����        : ���������ļ���ָ��Section�����м�¼��
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: TEntryList& tEntryList -- ���صļ�¼�б�
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
    ������      : LoadEntry
    ����        : ���������ļ���ָ��Section�ļ�¼��
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: TEntryList& tEntryList -- ���صļ�¼�б�
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
    ������      : SetEntry
    ����        : �޸Ļ����ָ���ļ�¼
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const TEntry& tEntry -- ���޸Ļ���ӵļ�¼
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
                // �޸�
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
    ������      : DelEntry
    ����        : ɾ��ָ���ļ�¼
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const TEntry& tEntry -- ��ɾ���ļ�¼
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

            //�ѵ�ǰ��֮��ļ�¼��ǰ��
            for( s32 nLoop = nIdx+1; nLoop < nEntryNum; nLoop++)
            {
                if (!cXmlConfig.SetRegKeyString(GetSectionName(), GetKeyName(nLoop-1), m_tEntryList[nLoop].ToString()))
                {
                    return 3;
                }
            }

            //���ܵļ�¼����һ
            if (!cXmlConfig.SetRegKeyInt(GetSectionName(), m_strEntryNum, nEntryNum-1))
            {
                return 4;
            }
            //ɾ�����һ��
            if (!cXmlConfig.DeleteRegKey(GetSectionName(), GetKeyName(nEntryNum-1)))
            {
                return 5;
            }
            if (!WriteFileFromString(m_strCfgFilePath.c_str(), cXmlConfig.GetXmlString()))
            {
                return 6;
            }
        }

        //�ڴ��¼�б���Ҳɾ����Ӧ�ļ�¼
        m_tEntryList.erase(itr);
        return 0;
    }

    /*====================================================================
    ������      : IsEntryExist
    ����        : �ж�ָ���ļ�¼�Ƿ����, ���ü�¼��MatchInKey�Ƚ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const TEntry& tEntry -- ���жϵļ�¼
    ����ֵ˵��  : TRUE -  ���ڣ�FALSE - ������
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��         �汾        �޸���       �޸�����
    2014/01/10     1.0         ��־��       ����
    ====================================================================*/
    bool IsEntryExist(const TEntry& tEntry)
    {
        s32 nIdx;
        FindEntry(tEntry, nIdx);
        return (nIdx != -1);
    }

    /*====================================================================
    ������      : FillEntry
    ����        : ���ü�¼�е�keyֵ���ָ���ļ�¼, ���ü�¼��MatchInKey�Ƚ�
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: TEntry& tEntry -- �����ļ�¼
    ����ֵ˵��  : TRUE -  �ҵ�����䣻FALSE - ������
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��         �汾        �޸���       �޸�����
    2014/01/14     1.0         ��־��       ����
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
            //�ҵ�ƥ��ļ�¼
            if(itr->MatchInKey(tEntry))
            {
                return itr;
            }
        }

        //�Ҳ���ƥ��ļ�¼
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
