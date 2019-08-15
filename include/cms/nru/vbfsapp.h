#ifndef _VBFS_APP_H
#define _VBFS_APP_H

#include "kdvtype.h"

#define MAXLEN_VBFSPATH      256
#define MAXLEN_MANUFACTURE   64


#define VBFSAPP_ERROR                 (-1)
#define VBFSAPP_OK                    (0)
#define VBFSAPP_ERR_INVALID_PARAM     (1)
#define VBFSAPP_ERR_OPENDISK          (2)
#define VBFSAPP_ERR_READDISK          (3)
#define VBFSAPP_ERR_NOTVBFS           (4)
#define VBFSAPP_ERR_FILEDESTROY       (5)
#define VBFSAPP_ERR_FSDESTROY         (6)
#define VBFSAPP_ERR_NOMEMORY          (7)
#define VBFSAPP_ERR_INVALID_FILENAME  (8)


#define DTYPE_DIR                      1
#define DTYPE_FILE                     2

struct vbfsapp_direct
{
	s8 d_type;    //D_DIR or D_FILE
	s8 d_name[MAXLEN_VBFSPATH];
};

struct vbfsapp_stat
{
	u32 st_starttime;
	u32 st_endtime;
	u64 st_filesize;   //bytes
};

struct vbfsapp_fextinfo
{
	s8 fe_format;
	s8 fe_manufacture[MAXLEN_MANUFACTURE];
};


/*====================================================================
函数名      : vbfsapp_opendisk
功能        : 打开磁盘
算法实现    : 
引用全局变量: 
输入参数说明:  [IN] psdiskname -- 需要打开的磁盘名
返回值说明  :  返回打开的磁盘句柄
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2011/01/05             linlifen         创建
====================================================================*/
s32 vbfsapp_opendisk(const s8* psdiskname, s32& hdisk);


/*====================================================================
函数名      : vbfsapp_closedisk
功能        : 关闭磁盘
算法实现    : 
引用全局变量: 
输入参数说明:  [IN] hdisk -- 需要关闭的磁盘句柄
返回值说明  :  0 -- 成功， 其他 -- 失败
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2011/01/05             linlifen         创建
====================================================================*/
s32 vbfsapp_closedisk(const s32 hdisk);


/*====================================================================
函数名      : vbfsapp_scandir
功能        : 扫描磁盘
算法实现    : 
引用全局变量: 
输入参数说明:  [IN]  psdir -- 需要扫描的目录
               [OUT] namelist -- 扫描路径下的路径或文件列表
			   [OUT] nNameNum -- 路径或文件数量
返回值说明  :  0 -- 成功， 其他 -- 失败
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2011/01/05             linlifen         创建
====================================================================*/
s32 vbfsapp_scandir(const s8* psdir, struct vbfsapp_direct ***namelist, s32& nNameNum);


/*====================================================================
函数名      : vbfsapp_fstat
功能        : 获取文件基本信息，创建时间，结束时间，文件大小等
算法实现    : 
引用全局变量: 
输入参数说明:  [IN]  pszfilename -- 文件名
               [OUT] vstat       -- 文件信息
返回值说明  :  0 -- 成功， 其他 -- 失败
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2011/01/05             linlifen         创建
====================================================================*/
s32 vbfsapp_fstat(const s8* pszfilename, struct vbfsapp_stat *vstat);


/*====================================================================
函数名      : vbfsapp_getextinfo
功能        : 获取文件扩展信息信息（文件格式，厂商信息等）
算法实现    : 
引用全局变量: 
输入参数说明:  [IN]  pszfilename -- 文件名
               [OUT] fextinfo    -- 文件扩展信息
返回值说明  :  0 -- 成功， -1 -- 失败
----------------------------------------------------------------------
修改记录    ：
日  期      版本        修改人        修改内容
2011/01/05             linlifen         创建
====================================================================*/
s32 vbfsapp_getextinfo(const s8* pszfilename, struct vbfsapp_fextinfo *fextinfo);

#endif