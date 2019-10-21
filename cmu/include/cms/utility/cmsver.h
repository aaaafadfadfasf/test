#ifndef __CMS_VERSION_H__
#define __CMS_VERSION_H__

/*****************************************************************************
   ģ����      : cms2.0
   �ļ���      : cmsver.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ƽ̨2.0�汾�Ŷ���
   ����        : 
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���				�޸�����
   2012/12/17   1.0         ��С��			����
******************************************************************************/

#include <stdio.h>

#include <string>
using std::string;

#if defined( _DEBUG ) && !defined( DEBUG )
#define DEBUG
#endif

//CU2.0��ǰ�汾��
#define CU_VERSION     "CU 2.3.3.2.4 2019.08.02"

//ƽ̨2.0��ǰ�汾��
#define CMS_VERSION     "Phoenix 2.3.3.2.4 2019.08.02"
#define VER_NAME        "Phoenix"
#define VER_DATE        "2019.08.02"

//ModuleName��c_str()��ʽ
#if defined(_WIN64) || defined(WIN64) ||defined (__LP64__) || defined (__64BIT__) || defined (_LP64) || (__WORDSIZE == 64) 
#ifdef DEBUG
#define FORMAT_VER(ModuleName) "%s: %s(debug), 64bits, Compile Time: %s, %s\r\n", CMS_VERSION, ModuleName, __DATE__, __TIME__
#else
#define FORMAT_VER(ModuleName) "%s: %s, 64bits, Compile Time: %s, %s\r\n", CMS_VERSION, ModuleName, __DATE__, __TIME__
#endif
#else
#ifdef DEBUG
#define FORMAT_VER(ModuleName) "%s: %s(debug), 32bits, Compile Time: %s, %s\r\n", CMS_VERSION, ModuleName, __DATE__, __TIME__
#else
#define FORMAT_VER(ModuleName) "%s: %s, 32bits, Compile Time: %s, %s\r\n", CMS_VERSION, ModuleName, __DATE__, __TIME__
#endif
#endif

/* ʹ��ʾ��
#define CMU_VER  FORMAT_VER("cmu")
*/

#define VER_ITEM_MAX_LEN    (64)

//CCmsVer������Ϣ����ֱ��ʹ�ã���Ϣ����ʹ���ַ���
class CCmsVer
{
public:
    CCmsVer()
    {
        Clear();
    }

    CCmsVer(const string& strCmsVer)
    {
        FromString(strCmsVer);
    }

    ~CCmsVer()
    {
        Clear();
    }

    void Clear()
    {
        name.clear();
        version = 0;
        release = 0;
        build = 0;
        sp = 0;
        fix = 0;
        date.clear();
    }

public:
    void FromString(const string& strCmsVer)
    {
        Clear();

        if (strCmsVer.size() == 0)
        {
            return;
        }

        char szVerName[VER_ITEM_MAX_LEN+1] = {0};
        char szVerDate[VER_ITEM_MAX_LEN+1] = {0};
        sscanf(strCmsVer.c_str(), "%s %d.%d.%d.%d.%d %s", szVerName, &version, &release, &build, &sp, &fix, szVerDate);

        name = szVerName;
        date = szVerDate;
    }
    string ToString() const
    {
        string strCmsVer;

        char szCmsVer[VER_ITEM_MAX_LEN*7 +1] = {0};
        sprintf(szCmsVer, "%s %d.%d.%d.%d.%d %s", name.c_str(), version, release, build, sp, fix, date.c_str());

        strCmsVer = szCmsVer;
        return strCmsVer;
    }

    bool operator < ( const CCmsVer& tOther ) const
    {
        //ֻ�Ƚϰ汾�ţ��汾���ƺͰ汾���ں���

        if (version < tOther.version)
        {
            return true;
        }
        if (version > tOther.version)
        {
            return false;
        }

        if (release < tOther.release)
        {
            return true;
        }
        if (release > tOther.release)
        {
            return false;
        }

        if (build < tOther.build)
        {
            return true;
        }
        if (build > tOther.build)
        {
            return false;
        }

        if (sp < tOther.sp)
        {
            return true;
        }
        if (sp > tOther.sp)
        {
            return false;
        }

        if (fix < tOther.fix)
        {
            return true;
        }
        if (fix > tOther.fix)
        {
            return false;
        }

        return false;
    }
    bool operator > ( const CCmsVer& tOther ) const
    {
        //ֻ�Ƚϰ汾�ţ��汾���ƺͰ汾���ں���
        if (version > tOther.version)
        {
            return true;
        }
        if (version < tOther.version)
        {
            return false;
        }

        if (release > tOther.release)
        {
            return true;
        }
        if (release < tOther.release)
        {
            return false;
        }

        if (build > tOther.build)
        {
            return true;
        }
        if (build < tOther.build)
        {
            return false;
        }

        if (sp > tOther.sp)
        {
            return true;
        }
        if (sp < tOther.sp)
        {
            return false;
        }

        if (fix > tOther.fix)
        {
            return true;
        }
        if (fix < tOther.fix)
        {
            return false;
        }

        return false;
    }

    bool operator == ( const CCmsVer& tOther ) const
    {
        if (name != tOther.name)
        {
            return false;
        }

        if (date != tOther.date)
        {
            return false;
        }

        if (*this>(tOther))
        {
            return false;
        }

        if (*this<(tOther))
        {
            return false;
        }

        return true;
    }
    bool operator != ( const CCmsVer& tOther ) const
    {
        return !(operator==(tOther));
    }

public:
    const string& GetName() const
    {
        return name;
    }
    string& GetName()
    {
        return name;
    }
    void SetName(const string& strName)
    {
        name = strName;
    }

    int GetVersion() const
    {
        return version;
    }
    int GetVersion()
    {
        return version;
    }
    void SetVersion(const int nVersion)
    {
        version = nVersion;
    }

    int GetRelease() const
    {
        return release;
    }
    int GetRelease()
    {
        return release;
    }
    void SetRelease(const int nRelease)
    {
        release = nRelease;
    }

    int GetBuild() const
    {
        return build;
    }
    int GetBuild()
    {
        return build;
    }
    void SetBuild(const int nBuild)
    {
        build = nBuild;
    }

    int GetSp() const
    {
        return sp;
    }
    int GetSp()
    {
        return sp;
    }
    void SetSp(const int nSp)
    {
        sp = nSp;
    }

    int GetFix() const
    {
        return fix;
    }
    int GetFix()
    {
        return fix;
    }
    void SetFix(const int nFix)
    {
        fix = nFix;
    }

    const string& GetDate() const
    {
        return date;
    }
    string& GetDate()
    {
        return date;
    }
    void SetDate(const string& strDate)
    {
        date = strDate;
    }

private:
    string name;
    int    version;
    int    release;
    int    build;
    int    sp;
    int    fix;
    string date;
};

#endif  //#ifndef __CMS_VERSION_H__


// end of the file


