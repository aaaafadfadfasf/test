/**
* @file		vbfslib.h
* @brief		访问vbfs文件系统的接口文件
* @author	林莉芬
* @date		2016-06-16
* @version	1.0
* @copyright V1.0  Copyright(C) 2016 CBB All rights reserved.
*/
#ifndef _VBFSLIB_20100104_H_
#define _VBFSLIB_20100104_H_

/** vbfs文件系统中允许的最大录像文件名*/
#define MAXLEN_RECNAME   256

/** vbfs文件系统中允许的最大前端通道名*/
#define MAXLEN_CHANNAME  32

/** vbfs文件系统中允许的最大前端厂商名*/
#define MAXLEN_PRODUCT   8

/** vbfs文件系统中允许的最大编码格式名*/
#define MAXLEN_FORMAT    4

/** vbfs文件系统中允许的最大前端通道编号长度*/
#define LEN_CID          36

/** vbfs文件系统中允许的最大录像文件数量*/
#define MAXNUM_REC       (1<<20)

#define STRACPY(dst,src) {s32 len=sizeof(dst)>sizeof(src)?sizeof(src):sizeof(dst); strncpy(dst,src,len); dst[sizeof(dst)-1]=0;}

/** 文件句柄定义*/
typedef int   hs;

/** 无效的文件句柄值定义*/
#define INVALID_HS -1

/** 错误码定义*/
#define VBFS_OK                 					(u16)0						///<VBFS操作成功
#define VBFSERR_BASE								(u16)3000					///<VBFS操作失败的起始错误码值
#define VBFSERR_FILE_NOT_EXIST                     	(u16)(VBFSERR_BASE + 1)		///<获取RIA失败
#define VBFSERR_GET_DISK_FAIL                      	(u16)(VBFSERR_BASE + 2)		///<获取磁盘失败
#define VBFSERR_WRITE_TO_RIA_ERR                   	(u16)(VBFSERR_BASE + 3)		///<将RIA信息写入磁盘失败
#define VBFSERR_UNKNOW                            	(u16)(VBFSERR_BASE + 4)		///<未知错误
#define VBFSERR_FILE_IS_LOCKED                     	(u16)(VBFSERR_BASE + 5)		///<文件已经锁定
#define VBFSERR_NO_MEMORY		                    (u16)(VBFSERR_BASE + 6)		///<无法分配到内存

/** 回调事件定义*/
#define EVENT_RECDELETE							1							///<录像删除

/** vbfs系统中的文件信息结构体定义*/
struct vbfs_statinfo
{
	u32 recid;								///<录像文件编号，目前未使用
	s8	recname[MAXLEN_RECNAME+4];				///<录像文件名，带路径的完整文件名
	u32 starttime;							///<录像文件的开始时间，绝对时间
	u32 endtime;								///<录像文件的结束时间，绝对时间
	s8	cid[LEN_CID+4];						///<录像文件所属的前端编号+通道号，前端编号占用32位，通道号占用4，目前未使用
	s8  cnm[MAXLEN_CHANNAME+4];				///<录像文件所属的前端通道名称，目前未使用
	u64 recbytes;								///<录像文件的总大小，单位字节
    s8  product[MAXLEN_PRODUCT+4];				///<录像文件所属的前端所在的厂商名，目前未使用
	s8  format[MAXLEN_FORMAT+4];				///<录像文件中的视频编码名称，比如H264、MP4...，目前未使用
};

/** 库的初始化参数*/
struct TVbfsInitParam
{
	u64 llPreAllocMem;						///<预分配给vbfs文件系统用于缓存数据的内存大小，单位字节，该内存在第一块vbfs分区加载时进行分配
	u8  byDiskWriteMode;						///<磁盘写入方式，0 : 使用系统缓存, 1 : 使用O_DIRECT方式写入
	u8  byDiskAccessMode;						///<磁盘分区的接入模式，0 : ISCSI， 1 : 阵列
};

/**
 *@brief			库的初始化函数
 *@param[in]		TVbfsInitParam* ptInitParm  库的初始化参数
 *@return		TRUE:成功 FALSE:失败
 *@ref
 *@see
 *@note			该接口必须在使用vbfslib库的其他任何功能之前被成功调用
 */
BOOL32  vbfs_init(TVbfsInitParam* ptInitParm);

/**
 *@brief			库的反初始化函数
 *@return		TRUE:成功 FALSE:失败
 *@ref
 *@see
 *@note			该接口将释放所有的资源
 */
BOOL32  vbfs_free();

/**
 *@brief			将分区从内存中卸载
 *@param[in]		const s8 *filename  需要卸载的磁盘分区名称
 *@param[out]		BOOL32 *bNormalUnLoad  TRUE:正常卸载成功，FALSE:非正常卸载
 *@return		TRUE:成功 FALSE:失败
 *@ref
 *@see
 *@note			使用该接口之后，将不能再通过vbfslib库访问到相应的分区
 */
BOOL32  vbfs_unloaddisk(const s8 *filename, BOOL32 *bNormalUnLoad);

/**
 *@brief			将分区加载到内存中
 *@param[in]		const s8 *filename  需要加载的磁盘分区名称，一般为/vbfs/UUID的形式
 *@param[in]		const s8* diskname  需要加载的磁盘分区对应的盘符名，一般为/dev/sda1这样的形式
 *@return		TRUE:成功 FALSE:失败
 *@ref
 *@see
 *@note			在需要通过vbfslib库访问某个vbfs分区前，必须调用该接口对分区进行加载
 *                该接口的加载是将vbfs分区加载到用户空间的内存中，因此一定要避免在多个进程中同时
 *                加载同一个vbfs分区进行写入操作，如果只是读取，则可以多进程同时加载
 */
BOOL32  vbfs_loaddisk(s8 *filename, const s8* diskname);

/**
 *@brief			以只写方式创建并打开文件
 *@param[in]		const s8 *filename  需要写入的文件的完整文件名
 *@param[in]		const u32 dwFileStartAbsTime  文件的开始时间，绝对时间，如果填0，则由vbfs库取当前本地时间作为文件开始时间
 *@param[out]		u32* pdwFileHeadSize  返回的vbfs文件系统中允许的最大文件头大小
 *@return		(0,2047]:成功，有效的文件句柄 INVALID_HS:失败
 *@ref
 *@see
 *@note
 */
hs		vbfs_openforwrite( const s8 *filename, const u32 dwFileStartAbsTime = 0, u32* pdwFileHeadSize = NULL);

/**
 *@brief			以只读方式打开文件
 *@param[in]		const s8 *filename  需要读取的文件的完整文件名
 *@param[out]		u32* pdwFileHeadSize  返回vbfs文件系统中允许的最大文件头大小
 *@return		(0,2047]:成功，有效的文件句柄 INVALID_HS:失败
 *@ref
 *@see
 *@note			
 */
hs		vbfs_openforread( const s8 *filename, u32* pdwFileHeadSize = NULL);

/**
 *@brief			定位到文件的某个位置进行操作
 *@param[in]		hs stream  文件句柄，由vbfs_openforwrite或者vbfs_openforread函数返回的值
 *@param[in]		s64 offset  定位的偏移量，单位字节
 *@param[in]		s32 origin  定位的方式，取值：SEEK_SET，SEEK_CUR，SEEK_END
 *@return		0 : 成功，<0：失败
 *@ref
 *@see
 *@note
 */
s32		vbfs_seek(hs stream, s64 offset, s32 origin);

/**
 *@brief			向文件中写入数据
 *@param[in]		const void *buffer  需要写入的数据缓冲
 *@param[in]		u32 size  数据缓冲大小
 *@param[in]		u32 count  需要写入的缓冲数量，一般都设置为1
 *@param[in]		const u32 dwRecTime  写入本次缓冲中的数据后，文件的总时长，单位秒
 *@param[in]		hs stream  有效的文件句柄
 *@return		size*count : 成功，其他值：失败
 *@ref
 *@see
 *@note
 */
u32		vbfs_write(const void *buffer, u32 size, u32 count, const u32 dwRecTime, hs stream, u16 *pwErr = NULL);

/**
 *@brief			获取给定文件句柄的文件的当前读写位置
 *@param[in]		hs stream  有效的文件句柄
 *@return		>0 : 文件当前读写位置，<0：失败
 *@ref
 *@see
 *@note
 */
s64		vbfs_tell( hs stream );

/**
 *@brief			读取文件中数据
 *@param[in]		void *buffer  存放读取数据的缓冲，由上层提供
 *@param[in]		u32 size		数据缓冲大小
 *@param[in]		u32 count	需要读取的数据块个数，一般该值都设置为1
 *@param[in]		hs stream	有效的文件句柄
 *@return		>0 : 成功，<=0：失败
 *@ref
 *@see
 *@note
 */
u32		vbfs_read( void *buffer, u32 size, u32 count, hs stream );

/**
 *@brief			判断文件是否结束
 *@param[in]		hs stream	有效的文件句柄
 *@return		1 : 文件结束，0：文件未结束，<0：失败
 *@ref
 *@see
 *@note			在vbfs_read返回的读取大小小于期望读取的数据大小时，使用该接口判断是否读取到文件尾
 */
s32		vbfs_eof( hs stream );

/**
 *@brief			将文件所在磁盘分区上的所有数据同步到磁盘分区
 *@param[in]		hs stream	有效的文件句柄
 *@return		0：成功，<0：失败
 *@ref
 *@see
 *@note			在Linux操作系统中，将执行fdatasync操作，可能会影响性能，请不要轻易使用该接口
 */
s32		vbfs_fsync( hs stream );

/**
 *@brief			关闭相关的文件句柄
 *@param[in]		hs stream	有效的文件句柄
 *@param[in]		BOOL32 bFlushBuf TRUE:将文件所在磁盘上的数据同步到磁盘分区上
 *@return		0：成功，<0：失败
 *@ref
 *@see
 *@note			如果bFlushBuf设置为TRUE，则会执行与vbfs_fsync相同的操作
 */
s32		vbfs_close( hs stream, BOOL32 bFlushBuf=FALSE );//

/**
 *@brief			为相应文件分配磁盘空间
 *@param[in]		u32 size  需要分配的大小
 *@param[in]		u32 count 需要分区多少块size大小的空间
 *@param[in]		hs stream 有效的文件句柄
 *@return		size*count：成功，其他值：失败
 *@ref
 *@see
 *@note			分配的总大小为size*count
 */
u32		vbfs_allocspace(u32 size, u32 count, hs stream );

/**
 *@brief			判断分区是否为vbfs分区
 *@param[in]		const s8 *uuidname  分区名，一般为带Uuid的名称
 *@param[in]		s32 nReadDisk  是否读取磁盘分区进行判断
 *@return		TRUE：是vbfs分区，FALSE：不是vbfs分区
 *@ref
 *@see
 *@note			
 */
BOOL32  vbfs_isvbfsdisk(const s8 *uuidname, s32 nReadDisk=0);

/**
 *@brief			获取磁盘分区的空间使用信息
 *@param[in]		const s8 *diskname  分区名，一般为带Uuid的名称
 *@param[out]		u64* pfreebytes  剩余可用的磁盘分区大小，单位字节
 *@param[out]		u64* ptotalbytes  磁盘分区总大小，单位字节
 *@param[out]		u64* plockbytes  磁盘分区上被锁定的文件占用的总空间，单位字节
 *@return		TRUE：成功，FALSE：失败
 *@ref
 *@see
 *@note			
 */
BOOL32  vbfs_getdiskfreespace(const s8 *diskname, u64* pfreebytes, u64* ptotalbytes, u64* plockbytes);

/**
 *@brief			获取文件信息
 *@param[in]		const s8 *filename  完整文件名
 *@param[out]		vbfs_statinfo* pvbfs_stat  返回的文件信息
 *@return		TRUE：成功，FALSE：失败
 *@ref
 *@see
 *@note			
 */
BOOL32  vbfs_stat(const s8 *filename, vbfs_statinfo* pvbfs_stat);

/**
 *@brief			创建目录
 *@param[in]		const s8 *dirname 需要创建的目录名称
 *@return		TRUE：成功，FALSE：失败
 *@ref
 *@see
 *@note			该接口目前未实现
 */
BOOL32  vbfs_mkdir(const s8 *dirname);

/**
 *@brief			删除给定的文件
 *@param[in]		const s8 *filename 需要删除的文件的完成文件名
 *@return		0：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
s32  vbfs_remove(const s8 *filename);

/**
 *@brief			重命名文件
 *@param[in]		const char *oldname 原文件名，带路径的完整文件名
 *@param[in]		const char *newname 新文件名，带路径的完整文件名
 *@return		TRUE：成功，FALSE：失败
 *@ref
 *@see
 *@note			oldname与newname必须保持前面的路径一致
 */
BOOL32  vbfs_rename(const char *oldname, const char *newname );

/**
 *@brief			锁定或者解锁相关文件
 *@param[in]		const s8 *filename 需要锁定或者解锁的文件全名
 *@param[in]		BOOL32 bLock  TRUE：锁定相关文件，FALSE：解锁相关文件
 *@return		0：成功，其他值：失败
 *@ref
 *@see
 *@note			文件在锁定状态，不能被删除，只有解锁之后才能被删除	
 */
s32  vbfs_lockfile(const s8 *filename, BOOL32 bLock);

/**
 *@brief			状态回调函数定义
 *@param[out]		u32 dwEvent   事件名
 *@param[out]		u8 *pbyBuf    事件内容
 *@param[out]		s32 nLen      事件内容长度
 *@param[out]		KD_PTR pvContext  回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void(*PStatCallBack)(u32 dwEvent, u8 *pbyBuf, s32 nLen, KD_PTR pvContext);

/**
 *@brief			设置状态回调函数
 *@param[in]		PStatCallBack pfstat  回调函数指针
 *@param[in]		KD_PTR pvContext      回调上下文
 *@return		TRUE：成功，FALSE：失败
 *@ref
 *@see
 *@note				
 */
BOOL32  vbfs_setstatcallback(PStatCallBack pfstat, KD_PTR pvContext);

#endif//_VBFSLIB_20100104_H_

