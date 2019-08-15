/********************************************************************
	filename: cmszip.h
	purpose : 提供文件的zip压缩、解压 
	author  : pangubing@kedacom.com
    copyright:<Copyright(C) 2003-2013 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------
修改记录：
日 期          版本     修改人   修改记录
2016/03/01     V1.0     潘古兵     创建
*********************************************************************/
#include <string>
#include <vector>
using std::string;
using std::vector;

int ZipCompress(const string& strSrcFileName, const string& strDstFileName, bool bForceFileNameInZipNoDir = false);
int ZipCompress(const vector<string>& vecSrcFileName, const string& strDstFileName, bool bForceFileNameInZipNoDir = false);