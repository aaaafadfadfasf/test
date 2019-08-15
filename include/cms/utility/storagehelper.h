/*****************************************************************************
   ģ����      : Helper
   �ļ���      : storagehelper.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ������һЩ�ļ�ϵͳ�������������ͺ�
   ����        : ������
   ��Ȩ        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���       �߶���      �޸ļ�¼
   2009/06/20  1.0         ������                     ����
******************************************************************************/
#ifndef _STORAGEHELPER_H_
#define _STORAGEHELPER_H_

#include "helpercommon.h"
#ifdef _LINUX_
#include <mntent.h> // setmntent(),...
#include <dirent.h> //opendir,readdir,...
#endif//_LINUX_

#ifdef WIN32
#include <direct.h>
#endif

#include <list>
#include <map>
#include <string>

#ifdef WIN32
#define MKDIR(pathname, mode) _mkdir(pathname)
#else
#define MKDIR(pathname, mode) mkdir(pathname, mode)
#endif

#ifndef INVALID_PART_INDEX
#define INVALID_PART_INDEX 0
#endif

#ifndef MAX_PATH
#define MAX_PATH            (u16)256
#endif
#ifndef MAX_FNAME
#define MAX_FNAME           (u16)64
#endif

#define IsDev(lpszPath)  (((lpszPath) !=NULL && NULL!=strstr(lpszPath, "/dev/") ) ? TRUE :FALSE)
inline bool IsDotDir(const char* lpszFileName)
{
	return 0==strcmp(lpszFileName, ".") || 0==strcmp(lpszFileName, "..");
}

/*===========================================================
������    ScanMnt
����      �������Զ���Ĺ��˻ص�������ö�ٳ�mtab/ftab����Ϣ.
�㷨ʵ��  ��
����˵��  ��
			[out]tMntList, ���ص�ö���б�
			[in]pszMtabPath, mtab/ftab·����
			[in]pfFilter,���˻ص�������NULL��ʾö������
����ֵ˵����0���ɹ�����0��ʧ��
-------------------------------------------------------------
�޸ļ�¼  ��
��  ��		�汾		�޸���		�߶���        �޸ļ�¼
2009/04/09  1.0         ������                      �½�
============================================================*/
#define MAX_FSNAME_LEN 11
#define MAX_DIR_LEN 126
#define MAX_FS_TYPE_LEN 16
#define MAX_MNT_OPT_LEN 11
struct TMntent
{
	TMntent()
	{
		memset(this, 0, sizeof(*this));
	}
	char    mnt_fsname[MAX_FSNAME_LEN];    /* name of mounted file system */
	char    mnt_dir[MAX_DIR_LEN];       /* file system path prefix */
	char    mnt_type[MAX_FS_TYPE_LEN];      /* mount type (see mntent.h) */
	char    mnt_opts[MAX_MNT_OPT_LEN];      /* mount options (see mntent.h) */
	int     mnt_freq;       /* dump frequency in days */
	int     mnt_passno;     /* pass number on parallel fsck */
};
typedef std::list<TMntent> TMntList;
typedef std::map<std::string, TMntent> TMntMap;

typedef bool(*CBScanMntFilter)(struct TMntent * ptMnt);
int ScanMnt(TMntList &tMntList, const char *pszMtabPath, CBScanMntFilter pfFilter);
int ScanMnt(TMntMap &tMntMap, const char *pszMtabPath, CBScanMntFilter pfFilter);
/*===========================================================
������    ScanDir
����      �������Զ���Ĺ��˻ص�������ö�ٳ�ָ��Ŀ¼���ļ���Ϣ�б�.
�㷨ʵ��  ��
����˵��  ��
			[out]tDirList, ���ص�ö���б�
			[in]pszDirpath, ��ö��Ŀ¼·����
			[in]pfFilter,���˻ص�������NULL��ʾö������
����ֵ˵����0���ɹ�����0��ʧ��
-------------------------------------------------------------
�޸ļ�¼  ��
��  ��		�汾		�޸���		�߶���        �޸ļ�¼
2009/04/09  1.0         ������                      �½�
============================================================*/
#ifdef _LINUX_
typedef std::list<dirent> TDirList;
#else
typedef std::list<std::string/*dummy*/> TDirList;
#endif//_LINUX_

typedef bool(*CBScanDirFilter)(const struct dirent *pdirent);
int ScanDir(TDirList &tDirList, const char* pszDirpath, CBScanDirFilter pfFilter, bool bIncludeDotDir=false);

int RemoveDirRecursively(const char *pathname);
int FGetString(const char* lpszPath, char* pBuff, int nSize);
int FOutputLine(CBStringOutput pfOutput, void* pUserArg, const char* lpszPath);
int FScanf(const char* lpszPath, const char* lpszFormat, ...);

/////////////////////////////////////////////////////////////////////////////////////////////
//�ı������ļ�����
/////////////////////////////////////////////////////////////////////////////////////////////
// ���ܣ���ȡ�ļ������ݣ�����������ʽ��λ��ȡ��
s32 ReadFileLine(LPCSTR lpcstrFile, LPCSTR lpcstrRegex, size_t size, s8* pBuf);

// д���ļ������ݣ�������������ʽɾ���ϵ����ݣ�Ȼ����д���µ����ݣ�
s32 WriteFileLine(LPCSTR lpcstrFile, LPCSTR lpcstrRegex, LPCSTR lpcstrContent);

// ɾ���ļ������ݣ�����������ʽ��λɾ����
s32 DeleteFileLine(LPCSTR lpcstrFile, LPCSTR lpcstrRegex);

//ɾ���ļ���������������
typedef BOOL32 (*CBDeleteLine)(const s8 achLine[], void* pUserArg);
int FDeleteLine(const char* lpszPath, CBDeleteLine pfDeleteLine, void* pUserArg);


/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�·������
/////////////////////////////////////////////////////////////////////////////////////////////

//�����������ܵ���2λ,0��ʾû���ҵ�
int GetPartIndex(const char* lpszPartName);
char* SplitPartName(const char* lpszPartName, char* lpszDiskName, u8* byIndex);
s8* TrimPath(s8* lpszPath);
void SplitPath(const char *path, char *dir, char *fname);

/////////////////////////////////////////////////////////////////////////////////////////////
//���̹���
/////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_SERVICE_EXE_NUM    30
u8 GetServicePid(s32* anPid, const s8 *achServiceName);
BOOL32 IsFileUsedByPid(s32 nPid, const s8* lpszFilePath);
BOOL32 IsFileUsedByPName(const s8* lpszPName, const s8* lpszFilePath);

// ����ֵ˵����0��ʾ�ɹ���-1��ʾʧ�� ����鿴errno
s32 TestExistOfProcess(const s8* cstrProcessName, bool &bExist);

/////////////////////////////////////////////////////////////////////////////////////////////
//�洢�ռ����
/////////////////////////////////////////////////////////////////////////////////////////////
// ��ȡĳ�ļ�(�����ļ���)�����ļ�ϵͳ�Ŀ��ÿռ��Ƿ��㹻���㹻����TRUE�����򷵻�FALSE����������Ϳռ䲻�㣩
BOOL32 IsFSFreeSpaceSizeEnough(LPCSTR lpcstrFileName, u32 dwNeedBytes);

// ���cpconf�ļ���д��������������ʱ����PMS_SUCCESS,���򷵻ش�����
// �ļ��в�����ʱ�Զ�����
int CheckCpconfWriteAbility(u32 dwNeedWritingBytes);

#endif // _STORAGEHELPER_H_
