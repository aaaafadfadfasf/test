/*========================================================================================
模块名    ：dllloader
文件名    ：dllloader.h
相关文件  ：
实现功能  ：dll加载器，屏蔽linux和windows差异性
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2013/12/31         V1.0              fanxg                                  新建文件
=========================================================================================*/
#ifndef _DLL_LOADER_H_
#define _DLL_LOADER_H_

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <string>
using std::string;

#include "cms/container/xmap.h"

#ifdef WIN32
  typedef HINSTANCE TDllHandle;
#else
  typedef void* TDllHandle;
#endif

  typedef void* PFPointer;

struct TDllFunUnit
{
    TDllFunUnit()
    {
        ppFun = NULL;
    }

    TDllFunUnit(PFPointer& pUserFun): ppFun(&pUserFun)
    {
    }
    string strFunName;
    PFPointer* ppFun;
};

struct TDllClient
{
    string strDllPath;
    CStrMap<TDllFunUnit> tFunList;
};

class CDllLoader
{
public:
    CDllLoader()
    {
        m_hDll = NULL;
    }

    ~CDllLoader()
    {
        FreeDll();
    }

private:    
    CDllLoader(const CDllLoader&);
    void operator=(const CDllLoader&);

public:
    bool LoadDll(TDllClient& tClient)
    {
        m_hDll = GetDllHandle(tClient.strDllPath);
        if (m_hDll == NULL)
        {
            printf("LoadDll(): m_hDll == NULL\n");
            return false;
        }

        CStrMap<TDllFunUnit>& tFunUnitList = tClient.tFunList;
        Iterator pPos;
        while(!pPos.End())
        {
            string* pstrFunName = NULL;
            TDllFunUnit* ptDllFunUnit = NULL;
            if (tFunUnitList.Iterate(pPos, pstrFunName, ptDllFunUnit) && pstrFunName != NULL && ptDllFunUnit != NULL)
            {
                *(ptDllFunUnit->ppFun) = GetDllFun(ptDllFunUnit->strFunName);

                if (*(ptDllFunUnit->ppFun) == NULL)
                {
                    printf("LoadDll(): dll[%s] not exist fun[%s]\n", tClient.strDllPath.c_str(), ptDllFunUnit->strFunName.c_str());
                }
            }
        }

        return true;
    }

    void FreeDll()
    {
        if (m_hDll == NULL)
        {
            return;
        }

#ifdef WIN32
        FreeLibrary(m_hDll);
#else
        dlclose(m_hDll);
#endif
    }

    TDllHandle GetDllHandle(const string& strDllPath)
    {
        TDllHandle hDll = NULL;
        if (strDllPath.empty())
        {
            printf("GetDllHandle(): dll path is invalid\n");
            return NULL;
        }

#ifdef WIN32
        hDll = ::LoadLibrary(strDllPath.c_str());
#else
        hDll = dlopen(strDllPath.c_str(), RTLD_LAZY);
        if (NULL == hDll)
        {
            printf("strDllPath:%s, dlerror:%s\n", strDllPath.c_str(), dlerror());
        }
#endif
        return hDll;
    }

    PFPointer GetDllFun(const string& strFunName)
    {
        PFPointer pFun = NULL;
        if (strFunName.empty())
        {
            printf("GetDllFun(): fun name is invalid\n");
            return NULL;
        }

        if (m_hDll == NULL)
        {
            printf("GetDllFun(): m_hDll == NULL\n");
            return NULL;
        }

#ifdef WIN32
        pFun = (PFPointer)GetProcAddress(m_hDll, strFunName.c_str());
#else
        pFun = (PFPointer)dlsym(m_hDll, strFunName.c_str());
#endif
        return pFun;
    }

private:
    TDllHandle m_hDll;
};

#endif  //#ifndef _DLL_LOADER_H_

