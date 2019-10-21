#ifndef _STRTOK_H_
#define _STRTOK_H_
/*功能：提取字符串为token数组
  作者：胡志云
  日期：20140211创建
*/

#include <string>
using std::string;
#include <vector>
using std::vector;
#include "kdvtype.h"
#include <cstring>
#ifdef WIN32
#define strtok_r(str, sep, lasts) strtok(str, sep)
#endif // WIN32

inline void strtok(const s8 *str, const s8 *delim, vector<string> &tokens)
{
#ifndef WIN32
    s8 *pSepPtr;
#endif
    tokens.clear();

    s8 *pchToken;
    s8 *pszBuf = (s8*)malloc(strlen(str)+1);
    strcpy(pszBuf, str);
    s8 *pszBuf4Del = pszBuf;

    do 
    {
        pchToken = strtok_r( pszBuf, delim, &pSepPtr );
        if (NULL != pszBuf)
        {
            pszBuf = NULL;
        }

        if (NULL == pchToken)
        {
            break;
        }

        tokens.push_back(pchToken);
    } while (true);

    free(pszBuf4Del);
}

inline void strtok(const s8 *str, const s8 *delim, vector<u32> &tokens)
{
    tokens.clear();

    vector<string> res;
    strtok(str, delim, res);
    for (vector<string>::const_iterator it = res.begin(); it != res.end(); ++it)
    {
        tokens.push_back((u32)atoi(it->c_str()));
    }
}

#endif // _STRTOK_H_
