/*****************************************************************************
   ģ����      : Utility
   �ļ���      : ccfgfile.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �����ļ����༰��ع�����򷽷�
   ����        : ������
   ��Ȩ        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���       �߶���      �޸ļ�¼
   2012/04/26  1.0        ������                     ����
   2012/12/28  1.0        ��־��                     cfgfile�е�SetEntry�����޸�����ʱ����������ļ����ڴ治һ�µ�����
   2014/12/15  1.0        ��־��                     EntryTable�ֶ����ݱ���뺯��
******************************************************************************/

#if !defined(_CCFGFILE_H)
#define _CCFGFILE_H

#include "kdvsys.h"
#include <string>
#include <vector>
#include "mischelper.h"
using std::string;
using std::vector;

/*====================================================================
����        : CCfgFile
����        : �����ļ��������࣬�����˳��õĽӿڣ����������ఴ���ģ��ȥ����ʵ�֡�
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��         �汾        �޸���       �޸�����
2012/04/26     1.0         ������       ����
====================================================================*/
class CCfgFile 
{
public:
    void SetCfgFilePath(const char* lpszVar)
    {
        m_strCfgFilePath = lpszVar;
    }

    inline LPCSTR GetCfgFilePath() const
    {
        return m_strCfgFilePath.c_str();
    }

    virtual BOOL32 Load()=0;

protected:
    string m_strCfgFilePath;
};


/*====================================================================
����        : CEntryTable
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
class CEntryTable
{
public:
    typedef vector<TEntry> TEntryList;
    typedef typename TEntryList::iterator Iterator;

    CEntryTable()
    {
    }

    void Init(LPCSTR lpszCfgPath, const s8* lpszSectionName)
    {
        m_strCfgFilePath = lpszCfgPath;
        m_strSectionName = lpszSectionName;
    }

public:

    /*====================================================================
    ������      : LoadEntry
    ����        : ���������ļ���ָ��Section�����м�¼��
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: TEntryList& tEntryList -- ���صļ�¼�б�
    ����ֵ˵��  : TRUE -  �ɹ���FALSE - ʧ��
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��         �汾        �޸���       �޸�����
    2012/04/26     1.0         ������       ����
    2013/12/16                 ��־��       parseʧ�ܷ���FALSE
    ====================================================================*/
    BOOL32 LoadEntry(TEntryList& tEntryList)
    {
        u32 dwEntryNum = 0;
        if(!GetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", 0, (s32*)&dwEntryNum ))
        {
            return FALSE;
        }

        //��Ҫ���ֵ�������Է��û���һ���ܴ��ֵ??
        //if (dwEntryNum > MAX_XXX_NUM)
        //{
        //	dwEntryNum = MAX_XXX_NUM;
        //}
        BOOL32 bParseResult = TRUE;

        if (dwEntryNum > 0)
        {
            s8 **lpszTable  = AllocTable(dwEntryNum, 1024);

            if(!GetRegKeyStringTable( GetCfgFilePath(), GetSectionName(), "fail", lpszTable, &dwEntryNum, 1024 ))
            {
                FreeTable(lpszTable, dwEntryNum);
                return FALSE;
            }

            m_tEntryList.clear();

            for (u32 dwLoop = 0; dwLoop < dwEntryNum; dwLoop++)
            {
                TEntry tEntry;
                if (!tEntry.Parse(lpszTable[dwLoop]))
                {
                    bParseResult = FALSE;
                    break;
                }

                tEntryList.push_back(tEntry);
            }

            FreeTable(lpszTable, dwEntryNum);
        }

        return bParseResult;
    }

    BOOL32 LoadEntry()
    {
        return LoadEntry(m_tEntryList);
    }

    /*====================================================================
    ������      : SetEntry
    ����        : �޸Ļ����ָ���ļ�¼
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const TEntry& tEntry -- ���޸Ļ���ӵļ�¼
    ����ֵ˵��  : TRUE -  �ɹ���FALSE - ʧ��
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��         �汾        �޸���       �޸�����
    2012/04/26     1.0         ������       ����
    ====================================================================*/
    BOOL32 SetEntry(const TEntry& tEntry)
    {
        BOOL32 bRet = FALSE;
        s8 achEntry[255] = { 0 };

        s32 nIdx =0;
        Iterator itr = FindEntry(tEntry, nIdx);
        if(itr != m_tEntryList.end())
        {
            //�޸Ĵ��ڵļ�¼
            sprintf( achEntry, "Entry%d", nIdx);
            bRet = SetRegKeyString( GetCfgFilePath(), GetSectionName(), achEntry, tEntry.ToString().c_str());
            if(!bRet)
            {
                return FALSE;
            }

            *itr = tEntry;
        }
        else
        {
            u32 dwEntryNum = m_tEntryList.size();

            //���һ��
            sprintf( achEntry, "Entry%d", (int)dwEntryNum);    //Entry�������Ǵ�0��ʼ�Ƶġ�
            bRet = SetRegKeyString( GetCfgFilePath(), GetSectionName(), achEntry, tEntry.ToString().c_str());
            if(!bRet)
            {
                return FALSE;
            }

            //���ܵļ�¼����һ
            bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", dwEntryNum + 1);
            if(!bRet)
            {
                return FALSE;
            }

            //�ڴ��¼�б���Ҳ������Ӧ�ļ�¼
            m_tEntryList.push_back(tEntry);
        }

        return TRUE;
    }

    /*====================================================================
    ������      : DelEntry
    ����        : ɾ��ָ���ļ�¼
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: const TEntry& tEntry -- ��ɾ���ļ�¼
    ����ֵ˵��  : TRUE -  �ɹ���FALSE - ʧ��
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��         �汾        �޸���       �޸�����
    2012/04/26     1.0         ������       ����
    ====================================================================*/
    BOOL32 DelEntry(const TEntry& tEntry) //LPCSTR lpszKey
    {
        BOOL32 bRet = FALSE;
        s8 achEntry[255] = { 0 };

        s32 nIdx =0;
        Iterator itr = FindEntry(tEntry, nIdx);
        if(itr == m_tEntryList.end())
        {
            return FALSE;
        }

        u32 dwEntryNum = m_tEntryList.size();

        //�ѵ�ǰ��֮��ļ�¼��ǰ��
        for( u32 dwLoop = nIdx+1; dwLoop < dwEntryNum; dwLoop++)
        {
            sprintf( achEntry, "Entry%d", (int)(dwLoop-1));
            bRet = SetRegKeyString( GetCfgFilePath(), GetSectionName(), achEntry, m_tEntryList[dwLoop].ToString().c_str());
            if(!bRet)
            {
                return FALSE;
            }
        }

        //���ܵļ�¼����һ
        bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", dwEntryNum - 1);
        if(!bRet)
        {
            return FALSE;
        }

        //�ڴ��¼�б���Ҳɾ����Ӧ�ļ�¼
        m_tEntryList.erase(itr);

        //ɾ�����һ���Ϊ����������Ѿ��޸ģ�����ķ���ֵ���Բ��жϣ�ʧ�������Ҫ
        sprintf( achEntry, "Entry%d", (int)(dwEntryNum - 1));    //Entry�������Ǵ�0��ʼ�Ƶġ�
        DeleteRegKey( GetCfgFilePath(), GetSectionName(), achEntry);

        return TRUE;
    }
	/*====================================================================
	������      : DelSectionInCfgFile
	����        : ɾ�������ļ��е����ýڡ�����kdvsys�Ĺ��ܣ���������
	----------------------------------------------------------------------
	�޸ļ�¼    :
	��  ��         �汾        �޸���       �޸�����
	2016/10/26     1.0         ����       ����
	====================================================================*/
	s32 DelSectionInCfgFile(const s8* lpszProfileName,const s8* lpszSectionName)
	{


        char    achRow[MAX_LINE_LEN + 1];
        char    achSectionCopy[MAX_SECTIONNAME_LEN + 1];
        u32 dwLen;
        s8 *lpszReturn;
        u32 offset;

        bool isPreLineEmpty=false;
        bool isCurrentLineEmpty=false;
        u32 buffLen;
        u32 offsetPreEmptyLine;
        u32 offsetCurrentLine;
        u32 offsetNextLine;
        u32 offsetFin;
        s8 *lpszBuf = NULL;

        FILE *stream;
        stream = fopen(lpszProfileName, "rb+");
        if (stream == NULL)
        {
	        fclose(stream);
	        return FALSE;
	    }
	    if (fseek(stream, 0, SEEK_SET)!=0)
	    {
	        fclose(stream);
	        return FALSE;
	    }
	    strncpy( achSectionCopy, lpszSectionName, MAX_SECTIONNAME_LEN );
	    achSectionCopy[MAX_SECTIONNAME_LEN] = '\0';
	    do
	    {
	        offset=ftell(stream);
	        if( fgets( achRow, MAX_LINE_LEN, stream ) == NULL )
	        {
	             fclose(stream);
	             return(FALSE);
	        }
      		if(strcmp(achRow, "\r\n")==0 ||strcmp(achRow, "\n")==0 )
   		    {
                offsetPreEmptyLine = offset;
                isCurrentLineEmpty=true;
            }
            else
            {
                isCurrentLineEmpty=false;
            }
	        if( ( lpszReturn = strstr( achRow, "\r\n" ) ) != NULL )
	        {
	            lpszReturn[0] = '\0';
	        }
	        else if( ( lpszReturn = strstr( achRow, "\n" ) ) != NULL )
	        {
	            lpszReturn[0] = '\0';
	        }
        
	        dwLen = strlen( achRow );
	        if( dwLen <= 2 || achRow[0] != '[' || achRow[dwLen - 1] != ']' )
	        {
                isPreLineEmpty = isCurrentLineEmpty;
	            continue;
            }
            strncpy(achRow, achRow + 1, dwLen - 2);
            achRow[dwLen - 2] = '\0';
            if (strcmp(achRow, achSectionCopy) == 0)
            {
                if(isPreLineEmpty)
                {
                    offsetCurrentLine = offsetPreEmptyLine;
                }
                else
                {
                //�����±���
                offsetCurrentLine = offset;
                }
                //��һ�о���streamָ���
                offsetNextLine =  ftell(stream);
                fseek(stream, 0, SEEK_END);
                buffLen =  ftell(stream) - offsetNextLine;
                if (buffLen != 0)
                {
                    lpszBuf = (s8 *)malloc(buffLen);
                    //�Ƶ���һ��׼��ɾ����
                    fseek(stream, offsetNextLine, SEEK_SET);
                    buffLen = fread(lpszBuf, sizeof(char), buffLen, stream);
                    fseek(stream, offsetCurrentLine, SEEK_SET);
                    fwrite(lpszBuf, sizeof(char), buffLen, stream);
                    offsetFin = ftell(stream);
                    free(lpszBuf);
                }
                else
                {
                    fseek(stream, offsetCurrentLine, SEEK_SET);
                    offsetFin = ftell(stream);
                }
                //������ʵ��Ҫ�����ݣ���ȡĩβ
                lpszBuf = (s8 *)malloc(offsetFin);
                fseek(stream, 0, SEEK_SET);
                offsetFin = fread(lpszBuf, sizeof(char), offsetFin, stream);
                fclose(stream);
                stream = fopen(lpszProfileName, "wb");
                fwrite(lpszBuf, sizeof(char), offsetFin, stream);
                free(lpszBuf);
                fclose(stream);
                return TRUE;
            }
            isPreLineEmpty = isCurrentLineEmpty;

        }while (TRUE);
    }


    /*====================================================================
    ������      : DelEntrySection
    ����         : �������ļ���ɾ������entry��������entryΪ��ʱ���ã��󲿷�entry����ɾ�� 
                   section�ģ�����
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��:
    ����ֵ˵��  : TRUE -  �ɹ���FALSE - ʧ��
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��         �汾        �޸���       �޸�����
    2016/10/25     1.0         ����       ����
    ====================================================================*/
    BOOL32 DelEntrySection()
    {
        if (m_tEntryList.size() != 0)
        {
            for (Iterator EntryIter = m_tEntryList.begin(); EntryIter != m_tEntryList.end();  EntryIter++)
            {
                DelEntry(*EntryIter);
            }
        }
        DeleteRegKey( GetCfgFilePath(), GetSectionName(), "EntryNum");
        DelSectionInCfgFile(GetCfgFilePath(), GetSectionName());
        return TRUE;
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
    BOOL32 IsEntryExist(const TEntry& tEntry)
    {
        s32 nIdx;
        FindEntry(tEntry, nIdx);
        return (nIdx != -1)? TRUE: FALSE;
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
    BOOL32 FillEntry(TEntry &tEntry)
    {
        s32 nIdx;
        FindEntry(tEntry, nIdx);
        if (nIdx != -1)
        {
            tEntry = m_tEntryList[nIdx];
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    /*====================================================================
    ������      : SyncToCfg
    ����        : ���ڴ��¼�б�ͬ���������ļ��� (hzytodo��ע ��Ҫ�ñ��벻��)
    �㷨ʵ��    : 
    ����ȫ�ֱ���: 
    �������˵��: ��
    ����ֵ˵��  : TRUE -  �ɹ���FALSE - ʧ��
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��         �汾        �޸���       �޸�����
    2012/04/26     1.0         ������       ����
    ====================================================================*/
    BOOL32 SyncToCfg()
    {
        BOOL32 bRet = FALSE;
        s8 achEntry[255] = { 0 };

        //��ȡ�ļ��е������б�
        TEntryList tEntryList;
        LoadEntry(tEntryList);

        //�Ա��ڴ��¼�б��������ļ��еļ�¼�б��Ƿ�һ��
        BOOL32 bExistEntrySame = TRUE;
        if (m_tEntryList.size() == tEntryList.size())
        {
            Iterator itrMem = m_tEntryList.begin();
            Iterator itrCfg = tEntryList.begin();

            for (; itrMem != m_tEntryList.end() && itrCfg != tEntryList.end(); itrMem++, itrCfg++)
            {
                if (!itrMem->MatchInKey(itrCfg))
                {
                    bExistEntrySame = FALSE;
                    break;
                }
            }
        }

        s32 nMemListSize = m_tEntryList.size();
        s32 nCfgListSize = tEntryList.size();

        if(bExistEntrySame)
        {
            //���˫����ͬ���ڵļ�¼˳��һ����������ȣ���ֻ����ж�ɾ�ٲ�

            //����ڴ��¼�������ü�¼���٣���ɾ�������ļ��ж���ļ�¼��
            if( nMemListSize < nCfgListSize)
            {
                for( u32 dwLoop = nMemListSize; dwLoop < nCfgListSize; dwLoop++)
                {
                    sprintf( achEntry, "Entry%d", dwLoop);
                    DeleteRegKey( GetCfgFilePath(), GetSectionName(), achEntry);
                }

                bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", nMemListSize);
            }

            //����ڴ��¼�������ü�¼���࣬��Ѷ���ļ�¼��ӵ������ļ���
            if( nMemListSize > nCfgListSize)
            {
                for( u32 dwLoop = nCfgListSize; dwLoop < nMemListSize; dwLoop++)
                {
                    sprintf( achEntry, "Entry%d", dwLoop);
                    bRet = SetRegKeyString( GetCfgFilePath(), GetSectionName(), achEntry, m_tEntryList[dwLoop].ToString().c_str());
                }

                bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", nMemListSize);
            }
        }
        else
        {
            //���˫����ͬ���ڵļ�¼˳��һ�»����ݲ���ȣ�����Ҫͬ��ȫ����¼
            for( u32 dwLoop = 0; dwLoop < nMemListSize; dwLoop++)
            {
                sprintf( achEntry, "Entry%d", dwLoop);
                bRet = SetRegKeyString( GetCfgFilePath(), GetSectionName(), achEntry, m_tEntryList[dwLoop].ToString().c_str());
            }

            bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", nMemListSize);

            //����ڴ��¼�������ü�¼���٣���ɾ�������ļ��ж���ļ�¼��
            if( nMemListSize < nCfgListSize)
            {
                for( u32 dwLoop = nMemListSize; dwLoop < nCfgListSize; dwLoop++)
                {
                    sprintf( achEntry, "Entry%d", dwLoop);
                    DeleteRegKey( GetCfgFilePath(), GetSectionName(), achEntry);
                }
            }
        }

        return bRet;
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

protected:
    string m_strCfgFilePath;
    string m_strSectionName;
    TEntryList m_tEntryList;

};//class CEntryTable

/* EntryTable�ֶ����ݱ���뺯�� */
inline bool EntryTableFieldEncode(const string &strFrom, string &strTo)
{
    strTo = strFrom;

    for (size_t i = 0; i < strTo.size(); ++i)
    {
        if (strTo[i] == ' ')
        {
            strTo[i] = '\x1a';
        }
    }

    return true;
}

inline bool EntryTableFieldDecode(const string &strFrom, string &strTo)
{
    strTo = strFrom;

    for (size_t i = 0; i < strTo.size(); ++i)
    {
        if (strTo[i] == '\x1a')
        {
            strTo[i] = ' ';
        }
    }

    return true;
}

#endif  //_CCFGFILE_H
