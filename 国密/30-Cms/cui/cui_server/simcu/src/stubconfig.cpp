#include <afx.h>
#include <string>
#include "stubconfig.h"


CEvToMsgMap g_cEventToMsgMap;

void LoadMsg(const string &strDir)
{
    CFileFind cFinder;
    CString strWildcard(strDir.c_str());
    strWildcard += _T("\\*.*");
    int nMaxSize = 1024*1024;
    char *szBuf = new char[nMaxSize];

    BOOL bFinished = cFinder.FindFile(strWildcard);
    while ( bFinished )
    {
        bFinished = cFinder.FindNextFile();
        if ( cFinder.IsDots() )
        {
            continue;
        }
        if ( cFinder.IsDirectory() )
        {
            continue;
        }

        CString strFile = cFinder.GetFilePath();
        CString strFileName = cFinder.GetFileName();
        CFile cFile(strFile, CFile::modeRead | CFile::typeBinary);
        int nSize = cFile.Read(szBuf, nMaxSize);
        szBuf[nSize] = '\0';
        cFile.Close();

        int nPos = strFileName.ReverseFind('.');
        strFileName.SetAt(nPos, '\0');
        int nEvent = OspExtEventID(strFileName.GetBuffer());
        string strMsg = szBuf;
        g_cEventToMsgMap.Insert(nEvent, strMsg);
    }

    cFinder.Close();
}