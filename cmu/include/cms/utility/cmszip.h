/********************************************************************
	filename: cmszip.h
	purpose : �ṩ�ļ���zipѹ������ѹ 
	author  : pangubing@kedacom.com
    copyright:<Copyright(C) 2003-2013 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------
�޸ļ�¼��
�� ��          �汾     �޸���   �޸ļ�¼
2016/03/01     V1.0     �˹ű�     ����
*********************************************************************/
#include <string>
#include <vector>
using std::string;
using std::vector;

int ZipCompress(const string& strSrcFileName, const string& strDstFileName, bool bForceFileNameInZipNoDir = false);
int ZipCompress(const vector<string>& vecSrcFileName, const string& strDstFileName, bool bForceFileNameInZipNoDir = false);