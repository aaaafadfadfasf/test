/*****************************************************************************
   模块名      : Utility
   文件名      : ccfgfile.h
   相关文件    : 
   文件实现功能: 配置文件基类及相关工具类或方法
   作者        : 黄至春
   版权        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人       走读人      修改记录
   2012/04/26  1.0        黄至春                     创建
   2012/12/28  1.0        胡志云                     cfgfile中的SetEntry方法修改配置时会出现配置文件与内存不一致的问题
   2014/12/15  1.0        胡志云                     EntryTable字段内容编解码函数
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
类名        : CCfgFile
功能        : 配置文件公共基类，定义了常用的接口，具体配置类按这个模板去派生实现。
----------------------------------------------------------------------
修改记录    :
日  期         版本        修改人       修改内容
2012/04/26     1.0         黄至春       创建
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
类名        : CEntryTable
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
    函数名      : LoadEntry
    功能        : 加载配置文件中指定Section的所有记录项
    算法实现    : 
    引用全局变量: 
    输入参数说明: TEntryList& tEntryList -- 返回的记录列表
    返回值说明  : TRUE -  成功；FALSE - 失败
    ----------------------------------------------------------------------
    修改记录    :
    日  期         版本        修改人       修改内容
    2012/04/26     1.0         黄至春       创建
    2013/12/16                 胡志云       parse失败返回FALSE
    ====================================================================*/
    BOOL32 LoadEntry(TEntryList& tEntryList)
    {
        u32 dwEntryNum = 0;
        if(!GetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", 0, (s32*)&dwEntryNum ))
        {
            return FALSE;
        }

        //需要最大值保护吗，以防用户填一个很大的值??
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
    函数名      : SetEntry
    功能        : 修改或添加指定的记录
    算法实现    : 
    引用全局变量: 
    输入参数说明: const TEntry& tEntry -- 待修改或添加的记录
    返回值说明  : TRUE -  成功；FALSE - 失败
    ----------------------------------------------------------------------
    修改记录    :
    日  期         版本        修改人       修改内容
    2012/04/26     1.0         黄至春       创建
    ====================================================================*/
    BOOL32 SetEntry(const TEntry& tEntry)
    {
        BOOL32 bRet = FALSE;
        s8 achEntry[255] = { 0 };

        s32 nIdx =0;
        Iterator itr = FindEntry(tEntry, nIdx);
        if(itr != m_tEntryList.end())
        {
            //修改存在的记录
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

            //添加一项
            sprintf( achEntry, "Entry%d", (int)dwEntryNum);    //Entry的索引是从0开始计的。
            bRet = SetRegKeyString( GetCfgFilePath(), GetSectionName(), achEntry, tEntry.ToString().c_str());
            if(!bRet)
            {
                return FALSE;
            }

            //把总的记录数加一
            bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", dwEntryNum + 1);
            if(!bRet)
            {
                return FALSE;
            }

            //内存记录列表中也增加相应的记录
            m_tEntryList.push_back(tEntry);
        }

        return TRUE;
    }

    /*====================================================================
    函数名      : DelEntry
    功能        : 删除指定的记录
    算法实现    : 
    引用全局变量: 
    输入参数说明: const TEntry& tEntry -- 待删除的记录
    返回值说明  : TRUE -  成功；FALSE - 失败
    ----------------------------------------------------------------------
    修改记录    :
    日  期         版本        修改人       修改内容
    2012/04/26     1.0         黄至春       创建
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

        //把当前项之后的记录往前移
        for( u32 dwLoop = nIdx+1; dwLoop < dwEntryNum; dwLoop++)
        {
            sprintf( achEntry, "Entry%d", (int)(dwLoop-1));
            bRet = SetRegKeyString( GetCfgFilePath(), GetSectionName(), achEntry, m_tEntryList[dwLoop].ToString().c_str());
            if(!bRet)
            {
                return FALSE;
            }
        }

        //把总的记录数减一
        bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", dwEntryNum - 1);
        if(!bRet)
        {
            return FALSE;
        }

        //内存记录列表中也删除相应的记录
        m_tEntryList.erase(itr);

        //删除最后一项。因为上面的总数已经修改，这项的返回值可以不判断，失败与否不重要
        sprintf( achEntry, "Entry%d", (int)(dwEntryNum - 1));    //Entry的索引是从0开始计的。
        DeleteRegKey( GetCfgFilePath(), GetSectionName(), achEntry);

        return TRUE;
    }
	/*====================================================================
	函数名      : DelSectionInCfgFile
	功能        : 删除配置文件中的配置节。补充kdvsys的功能，尽量少用
	----------------------------------------------------------------------
	修改记录    :
	日  期         版本        修改人       修改内容
	2016/10/26     1.0         周旋       创建
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
                //缓存下本行
                offsetCurrentLine = offset;
                }
                //下一行就是stream指向的
                offsetNextLine =  ftell(stream);
                fseek(stream, 0, SEEK_END);
                buffLen =  ftell(stream) - offsetNextLine;
                if (buffLen != 0)
                {
                    lpszBuf = (s8 *)malloc(buffLen);
                    //移到下一行准备删除了
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
                //读入真实需要的内容，截取末尾
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
    函数名      : DelEntrySection
    功能         : 从配置文件中删除整个entry，仅能在entry为空时调用，大部分entry不会删除 
                   section的，慎用
    算法实现    : 
    引用全局变量: 
    输入参数说明:
    返回值说明  : TRUE -  成功；FALSE - 失败
    ----------------------------------------------------------------------
    修改记录    :
    日  期         版本        修改人       修改内容
    2016/10/25     1.0         周旋       创建
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
    BOOL32 IsEntryExist(const TEntry& tEntry)
    {
        s32 nIdx;
        FindEntry(tEntry, nIdx);
        return (nIdx != -1)? TRUE: FALSE;
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
    函数名      : SyncToCfg
    功能        : 把内存记录列表同步到配置文件中 (hzytodo批注 不要用编译不过)
    算法实现    : 
    引用全局变量: 
    输入参数说明: 无
    返回值说明  : TRUE -  成功；FALSE - 失败
    ----------------------------------------------------------------------
    修改记录    :
    日  期         版本        修改人       修改内容
    2012/04/26     1.0         黄至春       创建
    ====================================================================*/
    BOOL32 SyncToCfg()
    {
        BOOL32 bRet = FALSE;
        s8 achEntry[255] = { 0 };

        //读取文件中的配置列表
        TEntryList tEntryList;
        LoadEntry(tEntryList);

        //对比内存记录列表与配置文件中的记录列表是否一致
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
            //如果双方共同存在的记录顺序一致且内容相等，则只需进行多删少补

            //如果内存记录数比配置记录数少，则删除配置文件中多余的记录数
            if( nMemListSize < nCfgListSize)
            {
                for( u32 dwLoop = nMemListSize; dwLoop < nCfgListSize; dwLoop++)
                {
                    sprintf( achEntry, "Entry%d", dwLoop);
                    DeleteRegKey( GetCfgFilePath(), GetSectionName(), achEntry);
                }

                bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", nMemListSize);
            }

            //如果内存记录数比配置记录数多，则把多出的记录添加到配置文件中
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
            //如果双方共同存在的记录顺序不一致活内容不相等，则需要同步全部记录
            for( u32 dwLoop = 0; dwLoop < nMemListSize; dwLoop++)
            {
                sprintf( achEntry, "Entry%d", dwLoop);
                bRet = SetRegKeyString( GetCfgFilePath(), GetSectionName(), achEntry, m_tEntryList[dwLoop].ToString().c_str());
            }

            bRet = SetRegKeyInt( GetCfgFilePath(), GetSectionName(), "EntryNum", nMemListSize);

            //如果内存记录数比配置记录数少，则删除配置文件中多余的记录数
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

protected:
    string m_strCfgFilePath;
    string m_strSectionName;
    TEntryList m_tEntryList;

};//class CEntryTable

/* EntryTable字段内容编解码函数 */
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
