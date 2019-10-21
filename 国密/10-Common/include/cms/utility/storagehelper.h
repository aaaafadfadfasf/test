/*****************************************************************************
   模块名      : Helper
   文件名      : storagehelper.h
   相关文件    : 
   文件实现功能: 定义了一些文件系统操作辅助函数和宏
   作者        : 黄至春
   版权        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人       走读人      修改记录
   2009/06/20  1.0         黄至春                     创建
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
函数名    ScanMnt
功能      ：根据自定义的过滤回调函数，枚举出mtab/ftab的信息.
算法实现  ：
参数说明  ：
			[out]tMntList, 返回的枚举列表
			[in]pszMtabPath, mtab/ftab路径；
			[in]pfFilter,过滤回调函数，NULL表示枚举所有
返回值说明：0：成功；非0，失败
-------------------------------------------------------------
修改记录  ：
日  期		版本		修改人		走读人        修改记录
2009/04/09  1.0         黄至春                      新建
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
函数名    ScanDir
功能      ：根据自定义的过滤回调函数，枚举出指定目录的文件信息列表.
算法实现  ：
参数说明  ：
			[out]tDirList, 返回的枚举列表
			[in]pszDirpath, 待枚举目录路径；
			[in]pfFilter,过滤回调函数，NULL表示枚举所有
返回值说明：0：成功；非0，失败
-------------------------------------------------------------
修改记录  ：
日  期		版本		修改人		走读人        修改记录
2009/04/09  1.0         黄至春                      新建
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
//文本配置文件操作
/////////////////////////////////////////////////////////////////////////////////////////////
// 功能：读取文件行内容（利用正则表达式定位读取）
s32 ReadFileLine(LPCSTR lpcstrFile, LPCSTR lpcstrRegex, size_t size, s8* pBuf);

// 写入文件行内容（先利用正则表达式删除老的内容，然后再写入新的内容）
s32 WriteFileLine(LPCSTR lpcstrFile, LPCSTR lpcstrRegex, LPCSTR lpcstrContent);

// 删除文件行内容（利用正则表达式定位删除）
s32 DeleteFileLine(LPCSTR lpcstrFile, LPCSTR lpcstrRegex);

//删除文件中满足条件的行
typedef BOOL32 (*CBDeleteLine)(const s8 achLine[], void* pUserArg);
int FDeleteLine(const char* lpszPath, CBDeleteLine pfDeleteLine, void* pUserArg);


/////////////////////////////////////////////////////////////////////////////////////////////
//文件路径操作
/////////////////////////////////////////////////////////////////////////////////////////////

//分区索引可能到达2位,0表示没有找到
int GetPartIndex(const char* lpszPartName);
char* SplitPartName(const char* lpszPartName, char* lpszDiskName, u8* byIndex);
s8* TrimPath(s8* lpszPath);
void SplitPath(const char *path, char *dir, char *fname);

/////////////////////////////////////////////////////////////////////////////////////////////
//进程管理
/////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_SERVICE_EXE_NUM    30
u8 GetServicePid(s32* anPid, const s8 *achServiceName);
BOOL32 IsFileUsedByPid(s32 nPid, const s8* lpszFilePath);
BOOL32 IsFileUsedByPName(const s8* lpszPName, const s8* lpszFilePath);

// 返回值说明：0表示成功，-1表示失败 具体查看errno
s32 TestExistOfProcess(const s8* cstrProcessName, bool &bExist);

/////////////////////////////////////////////////////////////////////////////////////////////
//存储空间操作
/////////////////////////////////////////////////////////////////////////////////////////////
// 获取某文件(或者文件夹)所处文件系统的可用空间是否足够，足够返回TRUE，否则返回FALSE（包括出错和空间不足）
BOOL32 IsFSFreeSpaceSizeEnough(LPCSTR lpcstrFileName, u32 dwNeedBytes);

// 检测cpconf文件夹写入能力，有能力时返回PMS_SUCCESS,否则返回错误码
// 文件夹不存在时自动创建
int CheckCpconfWriteAbility(u32 dwNeedWritingBytes);

#endif // _STORAGEHELPER_H_
