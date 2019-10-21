/**
* @file		rpctrl.h
* @brief		录放像库提供给上层的接口文件
* @author	王小月、方辉
* @date		2007-04-11
* @version	4.0
* @copyright V4.0  Copyright(C) 2003-2005 KDC, All rights reserved.
*/

/**
 *@note								一些规则

 * 一	路径名以 "/"结尾，如 "d:/asf/", 不要给"d:/asf"
 *	只有在开始文件下载接收参数中，文件名为实际的文件名(如a.asf)，其他情况下应为文件前缀名(如a)
 *
 * 二	开始录像,开始时间必须为绝对时间(秒为单位),录小文件时不能为0，录大文件可以为0;
 *			 如果是小文件录象，时间间隔为[MIN_TIME_ITVL,MAX_TIME_ITVL];
 *			 路径和文件名不能为空;
 *			 录小文件时指定的目录不允许存在，录大文件时指定的目录可以存在;
 *
 * 三	开始放像,开始时间必须为绝对时间(秒为单位),放小文件时不能为0，放大文件时可以为0;
 *			 结束时间必须为绝对时间(秒为单位),必须大于开始时间,可以为0，表示播放完整个录象;
 *			 路径和文件名不能为空;
 *
 * 四  开始文件下载发送,开始时间必须为绝对时间(秒为单位),不能为0;
 *			 结束时间必须为绝对时间(秒为单位),必须大于开始时间,可以为0，表示播放完整个录象;
 *			 路径和文件名不能为空;
 *
 * 五	删除文件以目录为单元,目录下所有文件都删除,最后删除目录;
 * 
 * 六  为了简便，TNetAddr中的m_wPort是指rtp端口，程序内部将rtcp端口设置为m_wPort + 1, 所以两个m_wPort间至少相差2
 *
 * 七  放像进度原为百分比，调整之后改为绝对时间（如为大文件，则为文件内相对时间），可以调用RPGetPlayerStatis
 *			得到播放的即时时间和总时间，上层计算百分比;
 *
 */

#ifndef _RPCTRL_H_
#define _RPCTRL_H_

#include "osp.h"
#include "kdvdef.h"
#include "kdvmedianet.h"
#include "vbfslib.h"
#include "kdslib.h"


/** RPCTRL库的版本号*/
#define  VER_RPCTRL								( const char * )"rpctrl_nru20 git 50.10.00.30.140506"

/** 错误码定义*/
#define RP_OK                 					(u16)0					///<操作成功
#define RPERR_BASE								(u16)2000				///<错误码的起始值
#define RPERR_RP_ALREADY_INITIAL					(u16)(RPERR_BASE + 1)		///<库已经初始化
#define RPERR_RP_NOT_INITIAL						(u16)(RPERR_BASE + 2)		///<库未初始化
#define RPERR_NO_MEMORY							(u16)(RPERR_BASE + 3)		///<内存不足
#define RPERR_NULL_POINT							(u16)(RPERR_BASE + 4)		///<指针为空
#define RPERR_PARTION_NUM							(u16)(RPERR_BASE + 5)		///<达到最大允许的分区数量，目前未使用
#define RPERR_PARAM								(u16)(RPERR_BASE + 6)		///<参数错误
#define RPERR_NOT_CREATE							(u16)(RPERR_BASE + 7)		///<资源未创建
#define RPERR_NOT_RELEASE							(u16)(RPERR_BASE + 8)		///<资源未释放
#define RPERR_UNKNOWN								(u16)(RPERR_BASE + 9)		///<未知错误
#define RPERR_REC_NOT_EXIST						(u16)(RPERR_BASE + 10)		///<指定录像机不存在
#define RPERR_REC_STATUS_ERROR						(u16)(RPERR_BASE + 11)		///<录像机状态错误，即某个操作不能在当前的录像机状态进行
#define RPERR_NO_FREE_REC  						(u16)(RPERR_BASE + 12)		///<找不到空闲的录像机
#define RPERR_REC_OVER_CAPACITY						(u16)(RPERR_BASE + 13)		///<超过库允许的最大录像机容量限制
#define RPERR_MEDIATYPE_CHANGE						(u16)(RPERR_BASE + 14)		///<媒体类型改变,当前未使用
#define RPERR_WAIT_KEYFRAME						(u16)(RPERR_BASE + 15)		///<等待关键帧，当前未使用
#define RPERR_TIMESTAMP_ERR						(u16)(RPERR_BASE + 16)		///<时间戳错误，当前未使用
#define RPERR_PREREC_TIME_INVALID					(u16)(RPERR_BASE + 17)		///<无效的预录时间，预录时间的有效范围(0, MAX_PREREC_TIME]之间
#define RPERR_SMALL_FILE_INTERVAL_INVALID			(u16)(RPERR_BASE + 18)		///<无效的小文件时间间隔，有效范围[MIN_TIME_ITVL, MAX_TIME_ITVL]之间
#define RPERR_ALREADY_ALARM_TWO						(u16)(RPERR_BASE + 19)		///<磁盘分区已经处于二级告警，当前未使用
#define RPERR_PLAY_NOT_EXIST						(u16)(RPERR_BASE + 21)		///<指定放像机不存在
#define RPERR_PLAY_STATUS_ERROR						(u16)(RPERR_BASE + 22)		///<放像机状态错误，即某个操作不能在当前的放像机状态进行
#define RPERR_NO_FREE_PLY  						(u16)(RPERR_BASE + 23)		///<找不到空闲的放像机
#define RPERR_PLAY_OVER_CAPACITY					(u16)(RPERR_BASE + 24)		///<超过库允许的最大放像机容量限制
#define RPERR_PLAY_DROG_TIME_INVALID				(u16)(RPERR_BASE + 25)		///<放像时的定位时间无效，即不在文件的开始时间与结束时间之间
#define RPERR_PLAY_GRAN_INVALID						(u16)(RPERR_BASE + 26)		///<放像进度的回调粒度无效，有效范围[MIN_READ_CALLBACK_GRAN, MAX_READ_CALLBACK_GRAN]
#define RPERR_PLAY_RECORD_LENGTH_ZERO				(u16)(RPERR_BASE + 27)		///<录像文件时长为0
#define RPERR_PLAY_REACH_STOP_TIME					(u16)(RPERR_BASE + 28)		///<已经播放到结束时间
#define RPERR_PLAY_MODE_LOCKED                      (u16)(RPERR_BASE + 29)      ///<抽帧模式锁定>
#define RPERR_MEM_PARAM							(u16)(RPERR_BASE + 31)		///<创建循环缓冲时的参数错误
#define RPERR_MEM_NULL								(u16)(RPERR_BASE + 32)		///<循环缓冲空
#define RPERR_MEM_INVALID							(u16)(RPERR_BASE + 33)		///<循环缓冲无效
#define RPERR_MEM_FULL								(u16)(RPERR_BASE + 34)		///<循环缓冲满
#define RPERR_MEM_DATA_ERR							(u16)(RPERR_BASE + 35)		///<循环缓冲数据错误，目前未使用
#define RPERR_MEDIATYPE_UNSUPPORT					(u16)(RPERR_BASE + 41)		///<媒体类型不支持
#define RPERR_STREAM_IDX_INVALID					(u16)(RPERR_BASE + 42)		///<媒体流序号无效，有效范围[0, MAX_STREAM_NUM)
#define RPERR_PARTION_NAME_INVALID					(u16)(RPERR_BASE + 43)		///<分区名太长，有效范围(0, MAX_PARTION_NAME_LEN]
#define RPERR_ALARM_VALUE_INVALID					(u16)(RPERR_BASE + 44)		///<设置的告警阈值无效，有效值必须大于0，且一级告警阈值必须大于二级告警阈值
#define RPERR_START_TIME_INVALID					(u16)(RPERR_BASE + 45)		///<开始时间无效，开始时间必须大于0，且如果结束时间不为0，则开始时间按必须小于结束时间
#define RPERR_PATH_ALREADY_EXIST					(u16)(RPERR_BASE + 51)		///<目录已经存在
#define RPERR_PATH_NOT_EXIST						(u16)(RPERR_BASE + 52)		///<目录不存在
#define RPERR_PATH_CREATE_FAIL						(u16)(RPERR_BASE + 53)		///<目录创建失败
#define RPERR_PATH_DELETE_FAIL						(u16)(RPERR_BASE + 54)		///<目录删除失败
#define RPERR_FILE_PATH_INVALID						(u16)(RPERR_BASE + 55)		///<文件目录名无效，有效目录必须保证以'/'结尾，并且长度在(0, MAX_FILE_PATH_LEN]之间
#define RPERR_TOO_MANY_LINKS						(u16)(RPERR_BASE + 56)		///<linux系统下，目录数量超过了系统允许的最大限制
#define RPERR_NOT_ADD_DISK							(u16)(RPERR_BASE + 57)		///<分区没有加入管理列表
#define	RPERR_DISK_DIED							(u16)(RPERR_BASE + 58)		///<磁盘分区下线
#define RPERR_DISK_MGR_FULL						(u16)(RPERR_BASE + 59)		///<超过可管理的最大磁盘限制
#define RPERR_FILE_ALREADY_EXIST					(u16)(RPERR_BASE + 61)		///<文件已经存在
#define RPERR_FILE_NOT_EXIST						(u16)(RPERR_BASE + 62)		///<	文件不存在
#define RPERR_FILE_DELETE_FAIL						(u16)(RPERR_BASE + 63)		///<文件删除失败
#define RPERR_FILE_RENAME_FAIL						(u16)(RPERR_BASE + 64)		///<文件重命名失败
#define RPERR_FILE_NAME_INVALID						(u16)(RPERR_BASE + 65)		///<文件名长度无效，有效范围(0, MAX_FILE_NAME_LEN]
#define RPERR_RECLOG_FILE_CREATE_FAIL				(u16)(RPERR_BASE + 71)		///<reclog.txt文件创建失败
#define RPERR_RECLOG_FILE_NOT_EXIST					(u16)(RPERR_BASE + 72)		///<reclog.txt文件不存在
#define RPERR_RECLOG_FILE_OP_FAIL					(u16)(RPERR_BASE + 73)		///<reclog.txt文件操作失败
#define RPERR_PREREC_FILE_CREATE_FAIL				(u16)(RPERR_BASE + 74)		///<预录文件创建失败
#define RPERR_NO_FREE_FTPRCV  						(u16)(RPERR_BASE + 81)		///<找不到空闲的下载接收通道
#define RPERR_FTPRCV_NOT_EXIST  					(u16)(RPERR_BASE + 82)		///<指定下载接收通道不存在
#define RPERR_FTPRCV_STATUS_ERROR  					(u16)(RPERR_BASE + 83)		///<不能在该下载接收状态做该操作
#define RPERR_FTPRCV_LISTEN_SOCKET_ERROR			(u16)(RPERR_BASE + 84)		///<下载接收无Socket在监听，目前未使用
#define RPERR_FTPRCV_LISTEN_TIMEOUT					(u16)(RPERR_BASE + 85)		///<监听超时
#define RPERR_IP_OR_PORT_MUST_NO_ZERO				(u16)(RPERR_BASE + 86)		///<监听地址和端口不能为0
#define RPERR_FTPRCV_LISTEN_ERROR  					(u16)(RPERR_BASE + 87)		///<监听错误
#define RPERR_FTPRCV_ACCEPT_ERROR  					(u16)(RPERR_BASE + 88)		///<接收客户端连接错误
#define RPERR_FTPRCV_CLIENT_ERROR  					(u16)(RPERR_BASE + 89)		///<接收客户端错误
#define RPERR_NO_FREE_FTPSND  						(u16)(RPERR_BASE + 91)		///<找不到空闲的下载发送通道
#define RPERR_FTPSND_NOT_EXIST  					(u16)(RPERR_BASE + 92)		///<指定下载发送通道不存在
#define RPERR_FTPSND_STATUS_ERROR  					(u16)(RPERR_BASE + 93)		///<不能在该下载发送状态做该操作
#define RPERR_FTPSND_SOCKET_ERROR  					(u16)(RPERR_BASE + 94)		///<创建下载发送套结字失败
#define RPERR_FTPSND_CONNECT_ERROR  				(u16)(RPERR_BASE + 95)		///<连接下载接收方失败
#define RPERR_RPCTL_BUG								(u16)(RPERR_BASE + 96)		///<下载时rpctrl内部的异常
#define RPERR_INODE_DESTROYED_ERROR					(u16)(RPERR_BASE + 97)		///<Inode表损坏
#define RPERR_UNLOAD_DISK_ABNORMAL					(u16)(RPERR_BASE + 98)		///<卸载磁盘分区异常
#define RPERR_METHOD_NOTSUPPORTED					(u16)(RPERR_BASE + 99)		///<操作不支持
#define RPERR_FILE_LOCKED							(u16)(RPERR_BASE + 100)		///<文件已经加过锁

/** add zhx ftp udp下载有关*/
#define RP_ERR_FTP_PARAM							(u16)(RPERR_BASE+101)		///<下载参数错误
#define RP_ERR_FTP_SOCKET							(u16)(RPERR_BASE+102)		///<套结字相关操作错误
#define RP_ERR_FTP_TASK_CREATE						(u16)(RPERR_BASE+103)		///<创建下载线程失败
#define RP_ERR_SND_SERVICE_NOT_START				(u16)(RPERR_BASE+104)		///<下载的发送线程无创建或者未启动
#define RP_ERR_FTP_NEW_OBJECT						(u16)(RPERR_BASE+105)		///<创建下载对象失败
#define RP_ERR_FTP_MAX_CAPACITY						(u16)(RPERR_BASE+106)		///<超过允许的最大下载并发数MAX_RP_FTP_SND_NUM
#define RP_ERR_FTP_DATA_SCARE						(u16)(RPERR_BASE+107)		///<下载数据错误
#define RP_ERR_FTP_PACK_NO_INDEX					(u16)(RPERR_BASE+108)		///<包乱序，未使用
#define RP_ERR_OBJECT_NOT_EXIST						(u16)(RPERR_BASE+109)		///<下载对象不存在
#define RP_ERR_FTP_STATUS_ERR						(u16)(RPERR_BASE+110)		///<下载状态错误
#define RP_ERR_FTP_TOOMUCH_PROXY_DATA				(u16)(RPERR_BASE+111)		///<Proxy数据超过允许的最大长度MAX_PROXY_USER_DATA

#define RPERR_FILE_UNLOCKED							(u16)(RPERR_BASE + 120)		///<文件本来就未加锁

/** add zhx 06-04-18 文件预录有关*/
//#define RP_ERR_FILE_BUF_FILENULL					(u16)(RPERR_BASE + 120)	///<未使用
//#define RP_ERR_FILE_BUF_PARAM						(u16)(RPERR_BASE + 121)	///<参数错误，未使用
//#define RP_ERR_FILE_BUF_FOPEN						(u16)(RPERR_BASE + 122)	///<fopen错误
//#define RP_ERR_FILE_BUF_NODATA					(u16)(RPERR_BASE + 123)	///<没有数据错误
//#define RP_ERR_FILE_BUF_FREAD						(u16)(RPERR_BASE + 124)	///<fread错误
//#define RP_ERR_FILE_BUF_FWRITE					(u16)(RPERR_BASE + 125)	///<fwrite错误
//#define RP_ERR_FILE_BUF_FSEEK						(u16)(RPERR_BASE + 126)	///<fseek错误
//#define RP_ERR_FILE_BUF_DATAERR					(u16)(RPERR_BASE + 127)	///<数据混乱错误
//#define RP_ERR_FILE_BUF_NEW						(u16)(RPERR_BASE + 131)	///<分配缓冲错误

#define RP_ERR_OP_NOT_PERMISSION					(u16)(RPERR_BASE + 140)	///<不允许的操作
#define RP_ERR_NO_SUCH_RP_TYPE						(u16)(RPERR_BASE + 141)	///<无效的数据类型
#define RP_ERR_REACH_MAX_REG						(u16)(RPERR_BASE + 142)	///<达到库允许的最大发送回调注册数MAX_SND_CB_REG_NUM
#define RP_ERR_NOT_FIND_REG						(u16)(RPERR_BASE + 143)	///<没有找到发送回调信息
#define RP_ERR_CB_NOT_REG							(u16)(RPERR_BASE + 144)	///<发送回调没有注册
#define RP_ERR_DS_REG_FAIL							(u16)(RPERR_BASE + 145)	///<注册发送回调失败
#define RP_ERR_CMD_TIME_OUT						(u16)(RPERR_BASE + 146)	///<命令处理超时
#define RP_ERR_FILEVER_TOOLOW						(u16)(RPERR_BASE + 147)	///<文件版本太低
#define RPERR_IO_BUSY								(u16)(RPERR_BASE + 150)	///<IO错误
#define RPERR_FILE_IS_LOCKED						(u16)(RPERR_BASE + 151)	///<文件被锁定
#define RPERR_ADDSTREAM							(u16)(RPERR_BASE + 152)	///<添加流属性失败
#define RPERR_GET_FILEINFO							(u16)(RPERR_BASE + 153)	///<获取文件信息失败
#define RPERR_GET_STREAM_PROPERTY					(u16)(RPERR_BASE + 154)	///<获取流属性失败
#define RPERR_FILE_OPEN							(u16)(RPERR_BASE + 155)	///<文件打开失败

#define     RPCTRL_CREATE_TASK_ERR					(15800+1)				///<创建线程失败
#define     RPCTRL_CREATE_TASK_TIMEOUT				(15000+2)				///<创建线程超时
#define     RPCTRL_CREATE_SEM_ERR					(15800+3)				///<创建信号量超时
//#define     RPCTRL_NEW_ASF_ERR						(15800+4)			
#define     RPCTRL_ASF_NOT_FOUND					(15800+5)				///<录放像任务未找到

//#define     RPCTRL_ASF_MGR_EXIST					(15800+6)				
#define     RPCTRL_ASF_MGR_FULL					(15800+7)				///<超过最大任务管理数量
#define     RPCTRL_ASF_MGR_NOT_FOUND				(15800+8)				///<未找到任务管理对象
#define     RPCTRL_ASF_MGR_NEW_FAIL					(15800+9)				///<任务管理对象创建失败
#define     RPCTRL_STREAM_BUFF_NEW_FAIL				(15800+10)				///<帧缓冲创建失败
#define     RPCTRL_STREAM_BUFF_EMPTY				(15800+11)				///<帧缓冲空
#define     RPCTRL_STREAM_BUFF_FULL					(15800+12)				///<帧缓冲满
#define     RPCTRL_STREAM_BUFF_MALLOC_FAIL			(15800+13)				///<从帧缓冲中分区内存失败
#define     RPCTRL_RECASF_BIND_MGR_FAIL				(15800+14)				///<将任务绑定到管理对象失败

//#define     RPCTRL_RECASF_CMD_TIMEOUT				(15800+15)				
#define     RPCTRL_RECASF_CMD_BUSY					(15800+16)				///<有操作正在处理
#define     RPCTRL_RECASF_NEW_ASFWRITE_FAIL			(15800+17)				///<创建文件库对象失败
#define     RPCTRL_RECASF_NEW_MEMMGR_FAIL			(15800+18)				///<录像内存管理对象创建失败
#define     RPCTRL_RECASF_NEW_RECASFMGR_FAIL			(15800+19)				///<创建录放像任务的管理对象失败
#define     RPCTRL_RECASF_WAIT_FRAME				(15800+20)				///<等待数据帧
#define     RPCTRL_RECASF_WAIT_TIME					(15800+21)				///<等待读取后续的码流数据，即当前的码流数据还没有到读取的时间
#define     RPCTRL_RECASF_STATE_ERROR				(15800+22)				///<录像任务状态错误
#define     RPCTRL_RECASF_STREAMTYPE_ERROR			(15800+23)				///<媒体流类型错误，有效值[0, MAX_STREAM_NUM)
#define     RPCTRL_RECASF_WAIT_KEYFRAME				(15800+24)				///<需要等待关键帧
//#define     RPCTRL_RECASF_WAIT_VIDEOFRAME			(15800+25)				
//#define     RPCTRL_RECASF_BUFF_NOT_CREATE			(15800+26)				
#define     RPCTRL_RECASF_MGR_NOT_SAME				(15800+27)				///<录像管理对象变化
#define     RPCTRL_RECASF_MGR_NOT_REG				(15800+28)				///<录像任务没有管理者
#define     RPCTRL_RECASF_FORMAT_CHANGE				(15800+29)				///<录像码流的媒体属性变化，主要指载荷类型、宽、高
//#define     RPCTRL_RECASF_CMD_ACK_NOTMATCH			(15000+30)				///<
//#define     RPCTRL_PLAYASF_NEW_ASFREAD_FAIL			(15800+40)			
#define     RPCTRL_PLAYASF_NEW_MEMMGR_FAIL			(15800+41)				///<放像内存管理对象创建失败
//#define     RPCTRL_PLAYASF_BIND_MGR_FAIL			(15800+42)				
#define     RPCTRL_PLAYASF_MGR_IS_NOT_SAME			(15800+43)				///<放像管理对象变化
//#define     RPCTRL_PLAYASF_CMD_BUSY					(15800+44)			
//#define     RPCTRL_PLAYASF_CMD_TIMEOUT				(15800+45)				
#define     RPCTRL_PLAYASF_STATE_ERROR				(15800+46)				///<放像任务状态错误
#define     RPCTRL_PLAYASF_WAIT_TIME				(15800+47)				///<等待读取后续的码流进行发送
#define     RPCTRL_PLAYASF_PARAM_ERROR				(15800+48)				///<放像参数错误
#define     RPCTRL_PLAYASF_MGR_NOT_REG				(15800+49)				///<放像任务没有管理者
#define     RPCTRL_RECASF_BIGFILE_SWITCH			(15800+50)				///<大文件切换

/** 录像类型定义*/
#define RP_NORMAL_RECORD					0			///<帧数据，录制为asf文件格式
#define RP_UNKNOWN_KEDA_RECORD				1			///<包数据，支持重传，需要分析关键帧和宽高信息，录制为kds文件格式，
#define RP_UNKNOWN_OTHERNOTRTP_RECORD		100			///<包数据，不支持重传，不分析码流，录制为kds文件格式，盲录
#define	RP_PS_RECORD						101			///<当前未使用
#define	RP_KSJ_RECORD						102			///<帧数据，录制为ksj文件格式
#define RP_MP4_RECORD						103			///<帧数据，录制为mp4文件格式
#define RP_UNKNOWN_OTHERRTP_RECORD			104			///<包数据，支持重传，不分析码流，录制为kds文件格式,盲录

/** 放像类型定义*/
#define RP_NORMAL_PLAY						0			///<播放的文件为asf文件，目前不支持倒放
#define RP_UNKNOWN_KEDA_PLAY				1			///<播放的文件为kds文件，支持所有的播放操作
#define RP_UNKNOWN_OTHERNOTRTP_PLAY			100			///<播放的文件为kds文件，不支持与关键帧有关的任何操作
													///<比如，倒放、定位到关键帧、抽关键帧播放
//#define	RP_PS_PLAY						101
#define RP_KSJ_PLAY						102			///<播放的文件为ksj文件，目前不支持倒放
#define RP_MP4_PLAY						103			///<播放的文件为mp4文件，目前不支持倒放
#define RP_UNKNOWN_OTHERRTP_PLAY			104			///<播放的文件为kds文件，不支持与关键帧有关的任何操作
													///<比如，倒放、定位到关键帧、抽关键帧播放

/** 下载类型定义*/
#define RP_NORMAL_DOWN						0			///<下载的原文件为asf文件，不回调数据给上层业务，rpctrl库内部直接写文件
#define RP_KSJ_DOWN						1			///<下载的原文件为ksj文件，不回调数据给上层业务，rpctrl库内部直接写文件
#define RP_UNKNOWN_KEDA_DOWN				2			///<下载的原文件为kds文件，收到的包数据会回调给上层业务调用udp2asf写成asf或者mp4文件
#define	RP_PS_DOWN						3
#define RP_UNKNOWN_OTHER_DOWN				4			///<下载的原文件为kds文件，收到的包数据会回调给上层业务处理，且数据包为第三方厂商码流，不能使用udp2asf
#define RP_THIRDCOMPANY_FILE_DOWN			5			///<下载任务的原数据由第三方提供，不是下载服务端rpctrl通过直接读取磁盘分区上的文件得到
#define RP_MP4_DOWN						6			///<下载的原文件为mp4文件，不回调数据给上层业务，rpctrl库内部直接写文件

/** 无效的资源句柄*/
#define	RP_INVALID_ID				((u32)-1)

/** RPCTRL库支持的最大分区数*/
#define		MAX_PARTION_NUM						64

/** 磁盘使用空间回调事件定义*/
#define	RP_DISK_RESUME_NORMAL						(u8)0			///<磁盘空间恢复正常，FreeSpaceAlarmCallBack回调函数的第三个参数为磁盘剩余空间大小
#define	RP_DISK_ALARM_LEVEL_ONE					(u8)1			///<磁盘剩余空间达到磁盘一级告警阈值，FreeSpaceAlarmCallBack回调函数的第三个参数为磁盘剩余空间大小
#define	RP_DISK_ALARM_LEVEL_TWO					(u8)2			///<磁盘剩余空间达到磁盘二级告警阈值，FreeSpaceAlarmCallBack回调函数的第三个参数为磁盘剩余空间大小
#define	RP_DISK_SPACE_NOTIFY						(u8)3			///<磁盘空间上报，FreeSpaceAlarmCallBack回调函数的第三个参数为磁盘剩余空间大小

/** 磁盘状态回调事件定义*/
#define	RP_DISK_STATE_DIED						(u8)4			///<磁盘分区下线，FreeSpaceAlarmCallBack回调函数的第三个参数为NULL_REC_NO
#define	RP_DISK_STATE_RECOVER						(u8)5			///<磁盘分区恢复上线，FreeSpaceAlarmCallBack回调函数的第三个参数为NULL_REC_NO

#define	RP_DISK_COVER_RECORD						(u8)6			///<有vbfs分区上的录像被覆盖删除，FreeSpaceAlarmCallBack回调函数的第二个参数为被覆盖的文件名，第三个参数为文件名长度

/** RPCTRL库支持的最大媒体流数量*/
#define MAX_STREAM_NUM								(u8)3

/** 无效的录像机编号*/
#define NULL_REC_NO								(u32)~0

/** 无效的放像机编号，申请放像资源失败时，即调用RPCreatePlay()失败时返回该编号*/
#define NULL_PLY_NO								(u32)~0

/** 无效的下载接收编号，RPCreateFtpRcv()调用失败时返回该编号*/
#define NULL_FTPRCV_NO								(u8)255

/** 无效的下载发送编号，RPCreateFtpSnd()调用失败时返回该编号*/
#define NULL_FTPSND_NO								(u8)255

/** 等待下载发送方建立链接的超时时间*/
#define MAX_FTPRCV_LISTEN_TIMEOUT					(u32)8

/** RPCTRL库允许的最大录像机容量*/
#define MAX_RECORDER_CAPACITY						(u32)1024

/** RPCTRL库允许的最大放像机容量*/
#define MAX_PLAYER_CAPACITY						(u32)1024

/** RPCTRL库允许同时进行的最大TCP下载链路数*/
#define MAX_FTP_CHNS								(u8)4

/** RPCTRL库支持的最大分驱名长度*/
#define  MAX_PARTION_NAME_LEN						(u8)255

/** RPCTRL库支持的最大文件路径长度*/
#define	MAX_FILE_PATH_LEN							(u8)255

/** RPCTRL库支持的最大文件名长度*/
#define	MAX_FILE_NAME_LEN							(u8)255

/** RPCTRL库支持的最大预录时间，单位秒*/
#define MAX_PREREC_TIME							(u32)60

/** RPCTRL库支持的最大预录缓冲大小，目前未使用*/
//#define MAX_PREREC_SIZE							(u32)((MAX_PREREC_TIME * 25 * 8) << 10)

/** RPCTRL库支持的最大预录帧数，目前未使用*/
//#define MAX_PREREC_FRMN_NUM						(u32)10000

/** 最大快放倍数*/
//#define MAX_QUICKPLAY_TIMES						(u8)4

/** 最大慢放倍数*/
//#define MAX_SLOWPLAY_TIMES						(u8)4

/** RPCTRL库文件列表中最多可存放的文件数*/
#define MAX_FILE_NUMBER							(u32)1000

/** RPCTRL库支持的最小读文件回调粒度，单位秒*/
#define MIN_READ_CALLBACK_GRAN						(u8)1

/** RPCTRL库支持的默认读文件回调粒度，单位秒*/
//#define DEFAULT_READCB_GRAN						(u8)5

/** RPCTRL库支持的最大读文件回调粒度，单位秒*/
#define MAX_READ_CALLBACK_GRAN						(u8)60

/** RPCTRL库支持的最小小文件时间间隔，单位秒*/
#define MIN_TIME_ITVL								(u32)10

/** RPCTRL库默认的小文件时间间隔，单位秒*/
//#define DEFAULT_TIME_ITVL						(u32)60

/** RPCTRL库支持的最大小文件时间间隔，单位秒*/
#define MAX_TIME_ITVL								(u32)600

/** 环行缓冲个数*/
//#define MAX_BUF_NUM								(s32)16

//#define BUF_STARTREAD_NUM						(s32)1
//#define BUF_FASTREAD_NUM							(s32)13

/** 最大放像时间，目前主要用在下载中，当上层设置的下载时间为0时，内部使用该值作为结束时间*/
#define MAX_PLAY_TIME								(u32)-1

/** MediaPack库的视频接收缓冲大小，单位字节，目前已废弃不用*/
#define MEDIAPACK_RECV_BUFF						256*1024

/** MediaPack库的音频接收缓冲大小，单位字节，目前已废弃不用*/
#define MEDIAPACK_RECV_BUFF_AUDIO					64* 1024

/** MediaPack库的视频发送缓冲大小，单位字节*/
#define MEDIAPACK_SEND_BUFF						1024*1024		///<由128*1024调整过来 20091117

/** MediaPack库的音频发送缓冲大小，单位字节*/
#define MEDIAPACK_SEND_BUFF_AUDIO					512* 1024

/** MediaPack库的视频发送缓冲大小的关键帧数量*/
#define MEDIAPACK_SEND_BUFF_KEYFRAME_NUM			5
#define MEDIAPACK_SEND_BUFF_KEYFRAME_NUM_MIN		2

/** RPCTRL库允许的最大注册函数数量*/
#define MAX_SND_CB_REG_NUM							256

/**
 *@brief			剩余存储空间告警回调函数定义
 *@param[out]		u8 byAlarmLevel  回调事件，比如RP_DISK_RESUME_NORMAL等
 *@param[out]		s8* pchPartionName 磁盘分区名，在回调事件为RP_DISK_COVER_RECORD时，该参数为覆盖删除的文件名
 *@param[out]		u64 dwValue		具体意义见回调事件时的注释
 *@param[out]		KD_PTR pvContext  回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*FreeSpaceAlarmCallBack)( u8 byAlarmLevel, s8* pchPartionName, u64 dwValue, KD_PTR pvContext );

/**
 *@brief			文件下载进度回调函数定义，仅针对TCP方式下载时
 *@param[out]		u8 byFtpSndID      下载资源句柄
 *@param[out]		u32 dwCurrentFtpTime  当前下载进度，绝对时间，单位秒
 *@param[out]		KD_PTR pvContext  回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*FtpFileRateCallBack)( u8 byFtpSndID, u32 dwCurrentFtpTime, KD_PTR pvContext );

/**
 *@brief			小文件结束回调函数定义
 *@param[out]		u32 dwRecorderID      录像机资源ID
 *@param[out]		s8* pchFileName       结束的小文件名
 *@param[out]		u32 dwStoptime        小文件的结束时间，绝对时间
 *@param[out]		u32 dwRecSize         结束的小文件的大小，单位K
 *@param[out]		KD_PTR pvContext		回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*RecFileEndCallBack)( u32 dwRecorderID, s8* pchFileName, u32 dwStoptime, u32 dwRecSize, KD_PTR pvContext );

/**
 *@brief			大文件切换回调函数定义
 *@param[out]		u32 dwRecorderID      录像机资源ID
 *@param[out]		s8* pchFileName       结束的大文件的文件名
 *@param[out]		KD_PTR pvContext		回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*RecBigFileSwCallBack)( u32 dwRecorderID, s8* pchFileName, KD_PTR pvContext );

/**
 *@brief			读取文件进度回调函数定义
 *@param[out]		u32 dwPlayerID		放像机ID
 *@param[out]		u32 dwCurrentPlayTime	当前放像时间，绝对时间，MAX_PLAY_TIME时表示放像结束
 *@param[out]		KD_PTR pvContext		回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*ReadFileRateCallBack)( u32 dwPlayerID, u32 dwCurrentPlayTime, KD_PTR pvContext );

/** 录像状态回调类型定义*/
typedef enum
{
    RECORDER_REC_START = 1,			///<录像成功开启，回调内容为TRecorderRecStartInfo
    RECORDER_REC_STOP,				///<录像停止，回调内容为TRecorderRecStopInfo
	RECORDER_REC_FILE_START ,			///<录像文件打开成功，无内容
	RECORDER_REC_START_TIME,			///<录像文件开始时间回调，回调内容为TRecorderRecStartTime
    RECORDER_PREREC_START,				///<预录开始，无内容
    RECORDER_PREREC_STOP,				///<预录停止，回调内容为TRecorderPreRecStopInfo
	RECORDER_STREAM_STOP,				///<未使用
	RECORDER_BIGFILE_SWITCH,			///<大文件切换，回调内容为TBigRecFileSwitchInfo
} ERecorderCBType;

/** 放像状态回调类型定义*/
typedef enum
{
	PLAYER_CB_START = 1 ,				///<放像开始，未使用
	PLAYER_CB_STOP ,					///<放像结束，回调内容为TPlayStopInfo
	PLAYER_CB_PAUSE,					///<放像暂停，未使用
	PLAYER_CB_RESUME,					///<放像恢复，未使用
	PLAYER_CB_QUICK,					///<快放，未使用
	PLAYER_CB_SLOW,					///<慢放，未使用
	PLAYER_CB_ONEFRAME ,				///<单帧播放，未使用
} EPlayerCBType ;

/** 录像停止原因*/
typedef enum
{
    STOPREASON_NORMAL = 0,				///<录像正常停止
    STOPREASON_FORMAT_CHG = 1,			///<码流的载荷类型或者分辨率改变引起的停止录像
    STOPREASON_FILE_FAIL = 2,			///<写文件失败引起的录像停止
	STOPREASON_BIGFILE_SWITCH = 3,		///<大文件切换引起的录像停止
	STOPREASON_VBFS_NOMEM = 4,			///<vbfs 没有内存
} ERecorderStopReason;

/*typedef struct  
{

} TRecorderFileStart ;*/

/** 录像开始时间回调事件RECORDER_REC_START_TIME的内容结构体*/
typedef struct 
{
	u32 m_dwStartTime ;				///<录像开始时间，绝对时间
} TRecorderRecStartTime ;

/** 录像开始回调事件RECORDER_REC_START的内容结构体*/
typedef struct
{
    
} TRecorderRecStartInfo;

/** 录像停止事件RECORDER_REC_STOP的内容结构体*/
typedef struct
{
    u16 m_wStopReason;				///<录像停止原因，参见ERecorderStopReason
	u64 m_qwRecTotalSize;				///<录像停止时的文件大小，单位字节
    u64 m_qwTotalDiskUsage;			///<录像停止时占用的磁盘空间大小，单位字节，在小文件录制时，等于m_qwRecTotalSize+reclog.txt文件的大小
	u32 m_dwDuration;					///<录像停止时，录像文件的时长，单位秒
} TRecorderRecStopInfo;

/*typedef struct
{
    
} TRecorderPreRecStartInfo;*/

/** 预录停止事件的内容结构体定义*/
typedef struct
{
    u16 m_wStopReason ;				///<停止原因
} TRecorderPreRecStopInfo;

/** 大文件切换事件的内容结构体定义*/
typedef struct
{
	u16 m_wSwitchResult;							///<大文件切换的结果
	s8 m_achRecFilePath[MAX_FILE_PATH_LEN+1];		///<大文件切换后的新文件路径
    s8 m_achRecFileName[MAX_FILE_NAME_LEN+1];		///<大文件切换后的新文件名
    u32 m_dwStartTime;							///<大文件切换后的新文件的开始时间，绝对时间
}TBigRecFileSwitchInfo;

/** 网络地址结构体定义*/
typedef  struct  tagTRPNetAddr 
{
	u32  m_dwIp;						///<IP地址
	u16  m_wPort;						///<端口号
}TNetAddr;

/** 放像停止事件的内容结构体定义*/
typedef struct tagTPlayStopInfo
{
	u16	m_wReason ;					///<放像停止的原因
} TPlayStopInfo ;

/** RPCTRL库中的丢包及写出延时统计信息*/
typedef struct tagTRpStatics
{
	u32		m_dwFullLost;				///<RPCTRL库内部由于缓冲满导致的丢包数量
	u32		m_dwInLost;				///<网络丢包数量
	u32		m_dwTaskDelay;			///<录像线程的最大延时统计信息
	u32		m_dwIoDelay;				///<数据包写到文件时的最大延时统计信息
	double  m_lfUdpRate;				///<数据包的码率统计信息
}TRpStatics;

/**
 *@brief			网络数据包接收回调函数定义，提供给DATASWITCH使用，从DATASWITCH接收裸包
 *@param[out]		u8* pPackBuf           接收到的网络数据包
 *@param[out]		u16 wPackLen           数据包长度
 *@param[out]		KD_PTR ptDstAddr		数据包发往的目的地址
 *@param[out]		KD_PTR ptSrcAddr		数据包的来源地址
 *@param[out]		u64 qwTimeStamp		数据包的时间戳
 *@param[out]		KD_PTR pvContext		回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void* (*RPUdpPackRecvCallback)(u8* pPackBuf, u16 wPackLen, KD_PTR ptDstAddr, KD_PTR ptSrcAddr, u64 qwTimeStamp, KD_PTR pvContext);

/**
 *@brief			录像状态回调函数定义
 *@param[out]		u32 dwRecorderID		录像机ID
 *@param[out]		u32 dwCBType			回调类型，参见ERecorderCBType
 *@param[out]		void* pData			回调内容，具体参见回调类型的注释说明
 *@param[out]		s32 nDataLen			回调内容的大小
 *@param[out]		KD_PTR hUserHandle	回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*RecorderStateCallback)(u32 dwRecorderID, u32 dwCBType, void* pData, s32 nDataLen, KD_PTR hUserHandle);

/**
 *@brief			放像状态回调函数定义
 *@param[out]		u32 dwPlayerId		放像机ID
 *@param[out]		u32 dwCBType			回调类型，参见EPlayerCBType
 *@param[out]		void* pData			回调内容，具体参见回调类型的注释说明
 *@param[out]		s32 nDataLen			回调内容的大小
 *@param[out]		KD_PTR hUserHandle	回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void (*PlayerStateCallback)(u32 dwPlayerId, u32 dwCBType, void *pData, s32 nDataLen, KD_PTR hUserHandle);

/** TSysGlobalParam结构体中各个参数的默认值定义*/
#define	DEFAULT_CONFIG_VIDEOFROMKEYFRAME					TRUE			///<视频码流是否必须从关键帧开始
#define	DEFAULT_CONFIG_FRAMELOSTWAITKEYFRAME				TRUE			///<在视频丢帧后，是否一定要等到下一个关键帧才开始处理
#define	DEFAULT_CONFIG_RECTASKSWITCHTIME					40			///<录像任务的默认切换时间，单位毫秒
#define	DEFAULT_CONFIG_PLAYTASKSWITCHTIME					15			///<放像任务的默认切换时间，单位毫秒
#define	DEFAULT_CONFIG_COMMANDTIMEOUT						10000		///<RPCTRL库内部命令的默认超时时间，单位毫秒
#define	DEFAULT_CONFIG_VIDEOFRAMEDURATION					40			///<默认的视频帧间隔，单位毫秒
#define	DEFAULT_CONFIG_AUDIOFRAMEDURATION					30			///<默认的音频帧间隔，单位毫秒
#define	DEFAULT_CONFIG_BUFFTIME							2000			///<帧缓冲的最大时长，单位毫秒
#define	DEFAULT_CONFIG_BUFFSIZE							1024 * 1024	///<包缓冲的缓冲默认大小，单位字节
#define	DEFAULT_CONFIG_PLAY_BUFFTIME						100			///<播放帧数据时的最大帧缓冲时长，单位毫秒
#define	DEFAULT_CONFIG_PLAY_BUFFSIZE						64 * 1024	///<播放包数据时的最大包缓冲大小，单位字节
#define	DEFAULT_CONFIG_INSERTNULLFRAME						TRUE			///<当没有帧数据输入时，是否插入空帧
#define	DEFAULT_CONFIG_INPUTFRAMEMAXDELAY					10000		///<默认的最大输入帧延时，超过该延时，帧的时间戳将重新计算
#define	DEFAULT_CONFIG_FRAMEWAITTIME						400			///<默认等待帧的时长，超过该时长，则可能插入空帧
#define	DEFAULT_CONFIG_DURATIONCALCNUM						10			///<帧数统计数量，达到该帧数，则计算一次实际的帧间隔
#define	DEFAULT_CONFIG_USEFIXFRAMERATE						TRUE			///<是否使用固定帧率，TRUE，则内部使用DEFAULT_CONFIG_VIDEOFRAMEDURATION作为视频帧间隔
																	///<						   使用DEFAULT_CONFIG_AUDIOFRAMEDURATION作为音频帧间隔
																	///<					FALSE，则库内部每隔DEFAULT_CONFIG_DURATIONCALCNUM多个帧统计一次媒体流帧间隔
#define	DEFAULT_CONFIG_SWITCHFILEMINSPAN					1000			///<文件切换的最小间隔，目前未使用
#define	DEFAULT_CONFIG_DISK_OP_TIMEOUT						10000		///<磁盘操作超时时间，单位毫秒
#define	DEFAULT_TASK_NUM									0			///<默认的任务数量，该参数目前不用修改
#define	DEFAULT_TASK_NUM_PER_DIR							1			///<每个分区上的任务数量，目前该参数决定RPCTRL内部有多少录像/放像线程

#define	RP_SYS_MAX_DIR_NUM								32			///<最大磁盘分区数量
#define	RP_SYS_MAX_DIR_NAME								256			///<分区名的最大长度
#define	THREAD_NUM_DELFILE								25			///<覆盖线程的数量

#define	DEFAULT_LOG_PATH									("/var/log/rplog")	///<RPCTRL日志的默认存放路径
#define	DEFAULT_LOG_COUNT									(11)					///<RPCTRL日志默认数量
#define	DEFAULT_LOG_SIZE									(4*1024*1024)			///<RPCTRL单个日志文件的大小
#define	DEFAULT_AUDIO_RECVBUFF								(65536)				///<MediaPack中默认的音频接收缓冲大小，目前未使用
#define	DEFAULT_VIDEO_RECVBUFF								(262144)				///<MediaPack中默认的视频接收缓冲大小

/** 录放像库的系统参数配置结构体*/
typedef struct tagSysGlobalParam
{
    tagSysGlobalParam()
    {
        m_bVideoFromKeyFrame = DEFAULT_CONFIG_VIDEOFROMKEYFRAME;
        m_bFrameLostWaitKeyFrame = DEFAULT_CONFIG_FRAMELOSTWAITKEYFRAME;

        m_nClockRate = OspClkRateGet();

        m_nDefaultRecTaskSwitchTime = DEFAULT_CONFIG_RECTASKSWITCHTIME;
        m_nDefaultPlayTaskSwitchTime = DEFAULT_CONFIG_PLAYTASKSWITCHTIME;
        m_nDefaultCommandTimeout = DEFAULT_CONFIG_COMMANDTIMEOUT;

        m_nDefaultVideoFrameDuration = DEFAULT_CONFIG_VIDEOFRAMEDURATION;
        m_nDefaultAudioFrameDuration = DEFAULT_CONFIG_AUDIOFRAMEDURATION;
        m_nDefaultBuffTime = DEFAULT_CONFIG_BUFFTIME;
        
        m_nDefaultPlayBuffTime = DEFAULT_CONFIG_PLAY_BUFFTIME;

        m_nDefaultIsInsertNullFrame = DEFAULT_CONFIG_INSERTNULLFRAME;
        m_nDefaultInputFrameMaxDelay = DEFAULT_CONFIG_INPUTFRAMEMAXDELAY;
        m_nDefaultFrameWaitTime = DEFAULT_CONFIG_FRAMEWAITTIME;
        m_nDefaultFrameDurationCalcNum = DEFAULT_CONFIG_DURATIONCALCNUM;

        m_nDefaultContiueNullInsertNum = m_nDefaultInputFrameMaxDelay/m_nDefaultFrameWaitTime;

        m_bUseFixedFrameRate = DEFAULT_CONFIG_USEFIXFRAMERATE;

        m_nSwitchFileMinSpan = DEFAULT_CONFIG_SWITCHFILEMINSPAN;

		m_nDefaultDiskOpCommandTimeout = DEFAULT_CONFIG_DISK_OP_TIMEOUT ;
		m_nDefaultPlayBuffSize = DEFAULT_CONFIG_PLAY_BUFFSIZE ;
		m_nDefaultBuffSize = DEFAULT_CONFIG_BUFFSIZE ;

		m_nDefaultTaskNum = DEFAULT_TASK_NUM ;

		m_nDefaultTaskNumPerDir = DEFAULT_TASK_NUM_PER_DIR ;

		m_bDoStatis = TRUE;

		m_bMultiDelete = TRUE ;

		m_dwDelThreadNum = THREAD_NUM_DELFILE;

		strcpy(m_achLogPath, DEFAULT_LOG_PATH);

		m_byMaxLogCount = DEFAULT_LOG_COUNT;

		m_dwMaxSizePerLog = DEFAULT_LOG_SIZE;
		
		m_nDefaultAudioRecvBuff = DEFAULT_AUDIO_RECVBUFF;

		m_nDefaultVideoRecvBuff = DEFAULT_VIDEO_RECVBUFF;

		m_nDefaultMPPlayBufSize = MEDIAPACK_SEND_BUFF;

		m_nDefaultMPPlayBufKeyFrameNum = MEDIAPACK_SEND_BUFF_KEYFRAME_NUM;
		
    }


    s32 m_nClockRate;									///<时间频率，由OspClkRateGet()接口得到

    s32 m_nDefaultRecTaskSwitchTime;					///<默认的录像任务切换时间
    s32 m_nDefaultPlayTaskSwitchTime;					///<默认的放像任务切换时间

    s32 m_nDefaultCommandTimeout;						///<RPCTRL库内部命令的默认超时时间，单位毫秒

	s32 m_nDefaultDiskOpCommandTimeout;					///<磁盘操作超时时间，单位毫秒

    BOOL32 m_bVideoFromKeyFrame;						///<视频码流是否必须从关键帧开始
    BOOL32 m_bFrameLostWaitKeyFrame;					///<在视频丢帧后，是否一定要等到下一个关键帧才开始处理
    
    s32 m_nDefaultVideoFrameDuration;				///<默认的视频帧间隔，单位毫秒
    s32 m_nDefaultAudioFrameDuration;					///<默认的音频帧间隔，单位毫秒
    
    s32 m_nDefaultBuffTime;							///<帧缓冲的最大时长，单位毫秒
	
	s32	m_nDefaultPlayBuffSize ;						///<播放包数据时的最大包缓冲大小，单位字节

	s32 m_nDefaultBuffSize ;							///<包缓冲的缓冲默认大小，单位字节
    s32 m_nDefaultPlayBuffTime;						///<播放帧数据时的最大帧缓冲时长，单位毫秒

    s32 m_nDefaultIsInsertNullFrame;					///<当没有帧数据输入时，是否插入空帧

    s32 m_nDefaultInputFrameMaxDelay;					///<默认的最大输入帧延时，超过该延时，帧的时间戳将重新计算

    s32 m_nDefaultFrameWaitTime;						///<默认等待帧的时长，超过该时长，则可能插入空帧

    s32 m_nDefaultFrameDurationCalcNum;					///<帧数统计数量，达到该帧数，则计算一次实际的帧间隔

    s32 m_nDefaultContiueNullInsertNum;					///<持续插入空帧的数量

	BOOL32 m_bUseFixedFrameRate;						///<是否使用固定帧率，TRUE，则内部使用m_nDefaultVideoFrameDuration作为视频帧间隔
													///<						   使用m_nDefaultAudioFrameDuration作为音频帧间隔
													///<					FALSE，则库内部每隔m_nDefaultFrameDurationCalcNum多个帧统计一次媒体流帧间隔

    s32 m_nSwitchFileMinSpan;							///<文件切换的最小间隔，目前未使用

	s32	m_nDefaultTaskNum ;							///<默认的任务数量，该参数目前不用修改

	s32 m_nDefaultTaskNumPerDir ;						///<每个分区上的任务数量，目前该参数决定RPCTRL内部有多少录像/放像线程

	BOOL32	m_bDoStatis;								///<是否进行码流丢包及写出延时统计

	BOOL32	m_bMultiDelete;							///<是否多线程同时删除文件

	u32		m_dwDelThreadNum;							///<删除文件的线程数量

	s8	m_pchTaskDirName[RP_SYS_MAX_DIR_NUM][RP_SYS_MAX_DIR_NAME];		///<每个线程对应的分区名
	s8 m_achDllPathName[MAX_FILE_PATH_LEN + 1];						///<加载动态库的绝对路径

	s8    m_achLogPath[MAX_FILE_PATH_LEN + 1];						///<RPCTRL内部日志的存放路径
	u8    m_byMaxLogCount;										///<允许的最大日志数量
	u32   m_dwMaxSizePerLog;										///<多个日志文件允许的最大大小，单位字节
	s32   m_nDefaultAudioRecvBuff;									///<MediaPack中的音频接收缓冲大小，目前未使用
	s32   m_nDefaultVideoRecvBuff;									///<MediaPack中默认的视频接收缓冲大小，单位字节
	s32   m_nMpAffCpuNo;                                              ///<MediaPack亲核CPU号>
    s32   m_nDefaultMPPlayBufSize; 	// video send buffer of mediapack
    s32   m_nDefaultMPPlayBufKeyFrameNum;	// video send buffer of mediapack keyframenum	
} TSysGlobalParam;

/** 网络回传地址结构*/
typedef  struct  tagTNetBackAddr
{
	u32  m_dwIp;						///<IP地址
	u16  m_wRtcpPort;					///<rtcp端口号
}TNetBackAddr;

/** 网络接收地址信息结构*/
typedef  struct  tagTRPNetRcvParam
{
	BOOL32 m_bNVR;								///<是否NVR录像
	u8    m_byNum;								///<网络地址数量
	TNetAddr  m_atAddr[MAX_STREAM_NUM];				///<网络地址数组，第0个表示音频，第n个表示第n路视频，目前只支持两路视频
	TNetBackAddr m_atBackAddr[MAX_STREAM_NUM];		///<回传地址数组
}TRPNetRcvParam;

/** 网络发送地址信息结构*/
typedef  struct  tagTRPNetSndParam
{
	u8			m_byNum;							///<网络地址数量
	BOOL32		m_bUseRawSock ;					///<是否使用裸套结字
	TNetAddr	m_atLocalAddr[MAX_STREAM_NUM ];			///<本地地址数组，第0个表示音频，第n个表示第n路视频，目前只支持两路视频
	TNetAddr	m_atRemoteAddr[MAX_STREAM_NUM ];		///<远端地址数组，第0个表示音频，第n个表示第n路视频，目前只支持两路视频
	TNetSession m_atAddrForRawSock[MAX_STREAM_NUM];	///<如果m_bUseRawSock=TRUE,则本字段为src ip/port

	tagTRPNetSndParam()
	{
		m_bUseRawSock = FALSE ;
	}
}TRPNetSndParam;

/** 文件列表*/
typedef struct tagTListInfo
{
	u32 m_dwNum;											///<文件数量
	s8	m_astrFileName[MAX_FILE_NUMBER][MAX_FILE_NAME_LEN];	///<文件名数组
}TListInfo;

/** 载荷信息*/
typedef struct tagPTInfo
{
	u8 m_byRmtActivePT;				///<收到的码流中的载荷类型
	u8 m_byRealPT;					///<码流中的载荷类型对应我司内部定义的载荷类型
}TPTInfo;

/** 载荷信息结构*/
typedef struct tagRemotePTParam
{
	TPTInfo   m_atPTInfo[MAX_STREAM_NUM];		///<载荷值数组
}TRemotePTParam;

/** 本地载荷信息结构*/
typedef struct tagPTParam
{
	u8		  m_abyLocalPt[MAX_STREAM_NUM];		///<本地载荷数组
}TLocalPTParam;

/** 加密（解密）信息*/
typedef struct tagEncDecInfo
{
	s8 *m_pszKeyBuf;					///<加密（解密）key字串
	u16 m_wKeySize;					///<加密（解密）key字串大小
	u8 m_byMode;						///<加密（解密）模式
}TEncDecInfo;

/** 加密（解密）信息结构*/
typedef struct tagTEncDecParam
{
	u8	m_byNum;								///<加密（解密）信息数量
	TEncDecInfo m_atEncDecInfo[MAX_STREAM_NUM];	///<加密（解密）信息数组
}TEncDecParam;

/** 录放像时的媒体统计信息*/
typedef struct tagTMediaStatis
{
	u8  m_byMediaType;						///<媒体类型
	BOOL32 m_bAud;							///<是否为音频
	u32 m_dwNetFrmNum;						///<接收（发送）总帧数
	u32 m_dwNetFrmRate;						///<接收（发送）帧率
	u32 m_dwBufFullLostFrmNum;					///<缓冲满导致的丢包数
	u32 m_dwErrFrmLostFrmNum;					///<错帧导致的丢包数
	u32 m_dwBufEmptyNum;						///<缓冲空的次数
	u32 m_dwLastFrameId;						///<最后一帧的序号
	BOOL32 m_bWaitForKeyFrame;					///<是否在等待关键帧
	u32 m_dwDuration;							///<放像时为媒体流的持续时间，录像时为帧间隔（暂写死为40）
	u32 m_dwTimeStamp;						///<时间戳
	u32 m_dwAsfFrmNum;						///<录像时写入文件的总帧数，放像时未用
	u32 m_dwAsfFrmRate;						///<录像时写入文件的帧率，放像时未用
	u32 m_dwWidth;							///<视频宽度(音频时,把这个字段用作保存模式)
	u32 m_dwHeight;							///<视频高度
	BOOL32 m_bH263Plus;						///<是否263+
	u32 m_dwAverageBitrate;					///<平均波特率
	BOOL32 m_bPlayOut;						///<是否播放，针对上层不给网络地址，则认为那路流是不播放的
}TMediaStatis;

/** 录放像库初始化参数结构*/
typedef struct tagRPInitParam
{
	u32 m_dwInitialRecorderNum;				///<需要的录像机最大路数（不大于最大容量）
	u32 m_dwInitialPlayerNum;					///<需要的放像机最大路数（不大于最大容量）
	u32 m_dwRemainSpaceAlarmLevelOne;			///<一级告警的最小剩余存储空间，目前未使用
	u32	m_dwRemainSpaceAlarmLevelTwo;			///<二级告警的最小剩余存储空间，目前未使用
	FreeSpaceAlarmCallBack	m_pCallback;			///<剩余存储空间及告警回调函数
	KD_PTR m_pvContext;						///<回调函数上下文
}TRPInitParam;

/** 下载接收状态*/
enum EMFtpRcvStatus{
	FTP_RCV_STOP,								///<停止下载接收
	FTP_RCV_RUNNING							///<正在下载接收
};

/** 录象TCP方式下载接收参数*/
typedef struct tagFtpRcvParam
{
	TNetAddr m_tFtpSndAddr;					///<下载发送方地址
	TNetAddr m_tFtpListenAddr;					///<下载接收方地址
	s8 m_achRecFilePath[MAX_FILE_PATH_LEN+1];	///<保存录像文件的路径
    s8 m_achRecFileName[MAX_FILE_NAME_LEN+1];	///<保存录像文件的文件名
}TFtpRcvParam;

/** 下载发送状态*/
enum EMFtpSndStatus{
	FTP_SND_STOP,								///<停止下载发送
	FTP_SND_RUNNING							///<正在下载发送
};

/** 录象TCP方式下载发送参数*/
typedef struct tagFtpSndParam
{
	TNetAddr m_tFtpSndAddr;						///<下载发送方地址，需连接接收地址
	TNetAddr m_tFtpRcvAddr;						///<下载接收方地址
	u32 m_dwStartTime;							///<下载开始时间，绝对时间
	u32 m_dwStopTime;								///<下载结束时间
    s8 m_achPlayFilePath[MAX_FILE_PATH_LEN+1];		///<待下载的文件的路径
    s8 m_achPlayFileName[MAX_FILE_NAME_LEN+1];		///<待下载文件的文件名
	FtpFileRateCallBack m_pCallBack;				///<下载文件进度回调
	u8 m_byRateGran;								///<下载进度回调力度(以秒为单位,应处于[MIN_READ_CALLBACK_GRAN,MAX_READ_CALLBACK_GRAN]范围)
	KD_PTR m_pvContext;							///<回调函数上下文
}TFtpSndParam;

/** 录像状态*/
enum EMRecorderStatus{
	REC_STOP,									///<录像已经停止
	REC_RUNNING,									///<正在录像
	REC_PAUSE,									///<录像处于暂停状态，当前未使用
    REC_STOPING,									///<录像正在停止过程中
};	

/** RPCTRL中允许的最大媒体扩展信息长度*/
#define RP_MAXLEN_MEDIAINFOEXT    1024

/** 媒体扩展信息*/
typedef struct tagMediaInfoExt
{
	u16                 m_wMediaInfoDataLen;							///<媒体扩展信息实际长度
	u8                  m_abyMediaInfoData[RP_MAXLEN_MEDIAINFOEXT];		///<媒体扩展信息
}TMediaInfoExt;

/** 媒体流类型*/
enum ERPStreamType
{
	E_StreamType_Unknown,							///<未知的媒体流类型
	E_StreamType_Audio,							///<音频流
	E_StreamType_Video,							///<视频流
};

/** 媒体属性信息*/
typedef struct tagRPStreamProperty
{
	ERPStreamType m_eStreamType;								///<媒体流类型
	u8            m_byPayloadType;							///<载荷类型
	s8            m_szEncodingName[KDS_MAXLEN_ENCODINGNAME+1];	///<编码名称

	union
	{
		TAudioStreamProperty  m_tAudioStreamProperty;			///<音频媒体流属性
		TVideoStreamProperty  m_tVideoStreamProperty;			///<	视频媒体流属性
	};
}TRPStreamProperty;

/** 媒体流描述信息*/
typedef struct
{
	u64                   m_qwStreamStartTime;					///<媒体流的开始时间，单位毫秒
	u8                    m_byAudioStreamNum;					///<音频流数量
	u8                    m_byVideoStreamNum;					///<视频流数量
	TRPStreamProperty     m_atStreamProperty[MAX_STREAM_NUM];		///<媒体流属性信息
}TMediaStreamDescription;

/** 文件信息*/
typedef struct tagTRPFileInfo
{
	u64		m_qwTotalFileSize;								///<录像文件大小，单位字节
	u32		m_dwFileStartTime;								///<录像文件开始时间，单位秒
	u32		m_dwFileDuration;									///<录像文件播放的总时间长度，单位秒
	TMediaStreamDescription  m_tMediaStreamDesc;					///<录像文件的媒体流描述信息
	TMediaInfoExt           m_tMediaInfoExt;					///<录像文件的媒体扩展信息
}TRPFileInfo;

/** 文件的媒体信息*/
typedef struct tagTRPMediaInfo
{
	TMediaStreamDescription  m_tMediaStreamDesc;					///<录像文件的媒体流描述信息
	TMediaInfoExt           m_tMediaInfoExt;					///<录像文件的媒体扩展信息
}TRPMediaInfo;

/** 录像参数结构*/
typedef struct tagTRecParam
{
	BOOL32 m_bLocalRec;								///<是否为本地录像，本地录像要在codecwrapper中调用
    s8 m_achRecFilePath[MAX_FILE_PATH_LEN+1];			///<保存录像文件的路径，如果为非WINDOWS系统，必须以/mnt/开头
    s8 m_achRecFileName[MAX_FILE_NAME_LEN+1];			///<保存录像文件的文件名
	u32 m_dwStartTime;								///<录像文件的起始时间
	BOOL32 m_bVideoFirst;								///<录像文件是否以视频关键帧开始存
	BOOL32 m_bSmallFile;								///<是否为小文件方式录制录像，如果为非WINDOWS系统，且存储文件系统为通用文件系统，则该参数填TRUE
	u32 m_dwTimeInterval;								///<小文件的时间间隔，单位秒，m_bSmallFile=TRUE时有效
	RecFileEndCallBack m_pRecEndCallBack;				///<小文件结束回调函数，m_bSmallFile=TRUE时需要设置
	RecBigFileSwCallBack m_pRecBigFileSwCB;				///<大文件切换回调函数
	KD_PTR m_pvContext;								///<回调函数上下文
	
	TKdsStreamProperty  m_atStreamProperty[MAX_STREAM_NUM];	///<媒体流属性信息
	TMediaInfoExt       m_tMediaInfoExt;					///<媒体扩展信息
}TRecParam;

/** 录像的视频流码率*/
typedef struct tagStreamAverageBitrate
{
	u32 m_adwStreamAverageBitrate[MAX_STREAM_NUM];		///<流对应的平均码率，其中0为音频，1，为视频，2为视频
}TStreamAverageBitrate;

/** 录像机运行状态*/
typedef struct tagTRPRecStatus
{
	BOOL32 m_bPreRecStart;								///<预录是否开始
	EMRecorderStatus m_tRecState;						///<录像状态
}TRPRecStatus;

/** 录像机状态信息*/
typedef struct tagTRecorderStatus
{
	TRPRecStatus  m_tRecStatus;						///<录像状态
	TRecParam  m_tRecParam;							///<录像参数
	TRPNetRcvParam	m_tNetparam;						///<网络接收参数
	BOOL32 m_bOnlyRecIFrm;								///<是否只录关键帧
	u32	m_dwFrmItvl;									///<录关键帧的间隔
}TRecorderStatus;

/** 录像机统计*/
typedef struct tagTRecorderStatis
{
	u8	m_byNum;										///<媒体流数量
	u32 m_dwRecTime;									///<录像时长，单位秒
	TMediaStatis m_atMediaRecStatis[MAX_STREAM_NUM];		///<录像媒体统计信息
}TRecorderStatis;

/** 放像状态*/
enum EmPlayerStatus
{
	PLAYER_PLAY_STOP,						///<放像停止
	PLAYER_PLAY_START,					///<放像开始
	PLAYER_PLAY_PAUSE,					///<放像暂停
	PLAYER_PLAY_QUICK,					///<快放
	PLAYER_PLAY_SLOW,						///<慢放
    PLAYER_PLAY_STOPING,					///<放像正在停止
};

/** 放像效果参数*/
enum EmPlayerEffect
{
	PLAYER_EFFECT_ONLY_KEY_FRAME = 1 ,		///<播放时只发送关键帧

	PLAYER_EFFECT_ONLY_AUDIO = 1 << 2,		///<播放时只发送音频

	PLAYER_EFFECT_ONLY_VIDEO = 1 << 3,		///<播放时只发送视频

	PLAYER_EFFECT_SINGE_FRAME = 1 << 4,		///<单帧播放，未使用

	PLAYER_EFFECT_MAX_VALUE ,				///<无效值

} ;

/** 放像参数结构*/
typedef struct tagTPlayParam	
{
	BOOL32 m_bNVR ;								///<是否本地放像
    s8 m_achPlayFilePath[MAX_FILE_PATH_LEN+1];		///<待放像的文件所在路径，非WINDOWS系统，必须以/mnt/开头
    s8 m_achPlayFileName[MAX_FILE_NAME_LEN+1];		///<待放像的文件的文件名
	u32 m_dwStartTime;							///<放像开始时间，绝对时间，不能为0
	u32 m_dwStopTime;								///<放像结束时间，绝对时间，0表示播放到文件尾
	BOOL32 m_bSmallFile;							///<播放的文件是否为小文件，对于通用文件系统，m_achPlayFilePath+m_achPlayFileName为路径，且路径下面存在reclog.txt，则该参数设置为TRUE
	ReadFileRateCallBack m_pCallBack;				///<文件放像进度回调
	u8 m_byRateGran;								///<放像进度回调粒度(以秒为单位,应处于[MIN_READ_CALLBACK_GRAN,MAX_READ_CALLBACK_GRAN]范围)
	KD_PTR m_pvContext;							///<回调上下文
	TRPNetSndParam	m_tNetSndParam;				///<放像时的网络发送地址
	BOOL32 m_bPlyOnlyIFrm;							///<是否只发送关键帧
	BOOL32 m_bReversePlay;							///<是否倒放
	u32    m_dwPlaySSRC;							///<放像时码流包中的SSRC，0:表示不指定，内部自己生成；非0:上层业务指定SSRC，发送码流时必须使用该SSRC
}TPlayParam;

/** 放像机状态*/
typedef struct tagTPlayerStatus
{
	EmPlayerStatus	m_tPlyStatus;					///<播放状态
	u32             m_dwPlyRate;                    ///<播放速率
	u32				m_dwPlyEffect ;				///<放像效果参数，参见EmPlayerEffect(PLAYER_EFFECT_ONLY_KEY_FRAME...)
	TPlayParam		m_tPlayParam;					///<放像参数
}TPlayerStatus;

/** 放像机统计*/
typedef struct tagTPlayerStatis
{
	u8	m_byNum;										///<媒体流数量
	u32 m_dwPlyTime;									///<放像时长，单位秒
	u32 m_dwTotalPlyTime;								///<文件总时长，单位秒
	TMediaStatis m_atMediaPlyStatis[MAX_STREAM_NUM];		///<放像时媒体流统计信息
	u8  m_abyStreamNoArr[MAX_STREAM_NUM];				///<流序号数组，因为一个文件中有2个流，序号可能为2，3, 不是1，2，未使用
}TPlayerStatis;

/** 录像时回调上下文结构体*/
typedef struct tagTCBContext 
{
	u8 m_byIdx;									///<录像媒体流编号，0音频，其他视频
	u64 m_llAddr;									///<录像机对象地址
}TCBContext; 

/** 录像重传参数*/
typedef struct tagTRPRecRSParam
{
	TRSParam m_atRsParam[MAX_STREAM_NUM];			///<重传参数，0为音频，其他视频
	BOOL32	 m_abRepeatSnd[MAX_STREAM_NUM];			///<是否支持重传
}TRPRecRSParam;

/** 放像重传参数*/
typedef struct tagTRPPlyRSParam
{
	u16		 m_awBufTimeSpan[MAX_STREAM_NUM];		///<重传间隔，0为音频，其他视频
	BOOL32	 m_abRepeatSnd[MAX_STREAM_NUM];			///<是否支持重传
}TRPPlyRSParam;


/** UDP下载方同时下载数量*/
#define			MAX_RP_FTP_RCV_NUM			32

/** UDP下载发送服务最大的服务数*/
#define			MAX_RP_FTP_SND_NUM			32

/** 下载接收方的用户ID长度*/
#define			MAX_FTP_RCV_USER_ID_LEN		32

/** 下载消息版本号*/
#define	        RP_VER_DOWNLOAD				3		///<该版本号2开始支持媒体信息
													///<该版本号3开始支持打包ACK

/** UDP下载发送端任务状态*/
typedef enum
{
		SND_STATUS_START = 1,							///<下载开始
		SND_STATUS_STATUS_PROBE,						///<收到探测包，并已经回应
		SND_STATUS_WAITING_RCV_REQ,					///<等待接收端请求，未使用
		SND_STATUS_WAITED_RCV_REQ,						///<收到接收端的请求
		SND_STATUS_SEND_STREAM_PROPERTY,				///<等待发送流属性
		SND_STATUS_WAIT_PROPERTY_ACK,					///<等待接收端对流属性的回应消息
		SND_STATUS_SEND_FRAME_DATA,					///<等待发送帧数据
		SND_STATUS_WAIT_FRAME_DATA_ACK,					///<等待接收端对帧数据的回应
		SND_STATUS_STOP								///<下载停止
}TSndStatus;

/** UDP下载接收端状态,重点对有(!!!及0)的进行处理*/
typedef enum
{
		RCV_STATUS_START = 1,							///<1开始下载，未使用
		RCV_STATUS_ADD_STREAM_ERROR,					///<2写流错误，未使用
		RCV_STASTU_ADD_FRAME_ERROR,					///<3写帧错误
		RCV_STATUS_SND_REQ,							///<4发送下载请求，未使用
		RCV_STATUS_WAIT_STREAM_PROPERTY,				///<5等待流属性
		RCV_STATUS_SND_PROPERTY_ACK,					///<6发送流属性回馈
		RCV_STATUS_WAIT_FRAME_DATA,					///<7等待帧数据
		RCV_STATUS_SND_FRAME_DATA_ACK,					///<8发送帧回馈，未使用
		RCV_STATUS_TIMEOUT,							///<!!!9下载任务超时
		RCV_STATUS_CLOSE,								///<10关闭，未使用
		Rcv_STATUS_RCV_END,							///<!!!11下载结束
		RCV_STATUS_REQ_INVALID,						///<!!!12下载请求无效，检查下载参数(发送方认为下载请求参数可能不合法)，RpFtpRcvCallBack回调函数的第三个参数为错误原因，第四个参数无意义
		RCV_STATUS_PROBE,								///<已经发送探测包
		RCV_DATA_TYPE									///<下载数据类型，RpFtpRcvCallBack回调函数函数的第三个参数为TRcvDataType类型的值，第四个参数无效
}TRcvStatus;

/** 下载接收端的数据类型*/
typedef enum
{
	RCV_DATA_KEDA_ASF = 1,								///<下载的文件为ASF或者MP4文件，该类型的下载，后续将不会有数据包回调给上层，由RPCTRL内部写文件 
	RCV_DATA_KEDA_KDS,								///<下载的文件为Kds文件，且为KEDA码流，数据包回调给上层业务后，可以使用Udp2asf库写成asf或者mp4文件
	RCV_DATA_OTHER_KDS,								///<下载的文件为kds文件，且为外厂商码流，数据包回调给上层业务后，不用使用udp2asf
	RCV_DATA_KEDA_PS,									///<未使用
	RCV_DATA_THIRDCOMPANY,								///<2011.07.14 add by llf 表示下载的为第三方厂商的非标准码流数据，数据包回调给上层业务处理
	RCV_DATA_THIRDCOMPANY_STANDARD						///<2011.11.21 add by llf 表示下载的为第三方厂商的标准码流数据，数据包不回调给上层业务，由RPCTRL直接写文件
} TRcvDataType;

/** 文件系统类型*/
typedef enum
{
	FS_TYPE_EXT3 = 0,									///<通用文件系统
	FS_TYPE_VBFS,										///<vbfs文件系统
	FS_TYPE_CLOUD,									///<云存储
	FS_TYPE_UNKNOWN									///<未知类型
} TFsType;

/**
 *@brief			下载状态回调函数定义
 *@param[out]		u32 dwStatusCode		状态码，参见TRcvStatus
 *@param[out]		u32 dwSSid			下载任务的唯一标识，即使用RpFtpRcvCreate()接口返回的ssid值
 *@param[out]		u32 dwCurTime			当前下载的进度，绝对时间，在某些特殊的TRcvStatus值下有其他的含义，具体见TRcvStatus的说明
 *@param[out]		u32 dwCurSpeed		下载的速度，在某些特殊的TRcvStatus值下有无意义，具体见TRcvStatus的说明
 *@param[out]		KD_PTR pvContext		回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void(*RpFtpRcvCallBack)(u32 dwStatusCode, u32 dwSSid, u32 dwCurTime, u32 dwCurSpeed, KD_PTR pvContext);

/**
 *@brief			下载数据包回调函数定义
 *@param[out]		u32 dwSSid			下载任务的唯一标识，即使用RpFtpRcvCreate()接口返回的ssid值
 *@param[out]		u8 * pbData            回调上来的数据包
 *@param[out]		u32 dwDataSize		数据包大小
 *@param[out]		u64 qwTimeStamp		数据包对应的时间戳，相对时间戳，从0开始，单位毫秒
 *@param[out]		u8 byStreamId			数据包对应的媒体流编号，0为音频，其他为视频
 *@param[out]		KD_PTR pvContext		回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void(*RpFtpDataCallback)(u32 dwSSid, u8 * pbData, u32 dwDataSize, u64 qwTimeStamp, u8 byStreamId, KD_PTR pvContext);

/** 下载服务端的发送缓冲大小*/
#define DEFAULT_RP_DOWNLOAD_SERVER_BUFF		(128 * 1024)

/** 下载客户端的接收缓冲大小*/
#define DEFAULT_RP_DOWNLOAD_CLIENT_BUFF		(128 * 1024)

/** 发送服务端参数结构*/
typedef struct tagFtpSndServParam
{
	tagFtpSndServParam()
	{
		m_tLocalAddr.m_dwIp = 0;
		m_tLocalAddr.m_wPort = 1723;
		m_dwSndBuf = DEFAULT_RP_DOWNLOAD_SERVER_BUFF;
		m_nSpeedLimit = -1;
	}

	TNetAddr		m_tLocalAddr;				///<发送服务端的本地地址
	s32			m_dwSndBuf;				///<发送缓冲大小，单位字节，取值范围[8KB, 2000KB], 0则表示使用默认值DEFAULT_RP_DOWNLOAD_SERVER_BUFF
	s32			m_nSpeedLimit;			///<下载速度限制，单位bps，取值大于等于20KB/s, 小于等于0表示不限速
}TFtpSndServParam;

/** 下载发送参数*/
typedef struct tagFtpUdpSndParam
{
	u32 m_dwStartTime;					///<开始时间，绝对时间
	u32 m_dwStopTime;						///<结束时间，绝对时间，0表示下载到文件结束，否则取值必须大于开始时间
	s8 m_abyFilePath[MAX_FILE_PATH_LEN];	///<文件路径，非WINDOWS操作系统下的通用文件系统，比如LINUX下的EXT3分区，必须以/mnt/开头
	s8 m_abyFileName[MAX_FILE_NAME_LEN];	///<文件名
	BOOL32 m_bSmallFile;					///<是否小文件
	u16	m_wDownloadType ;					///<下载的文件类型，比如RP_NORMAL_DOWN等
}TFtpUdpSndParam;

/** 下载客户端的参数*/
typedef struct tagFtpSndAllParam
{
	u32				m_dwSSid;							///<下载客户端的ssid
	TSndStatus		m_emSndStatus;						///<下载发送端状态
	TNetAddr			m_tRemoteAddr;						///<下载客户端ip/port
	s8				m_abyUserId[MAX_FTP_RCV_USER_ID_LEN+1];	///<下载接收端的用户编号
}TFtpSndAllParam;

/** 下载媒体信息*/
typedef struct tagDLMediaInfo
{
public:
	tagDLMediaInfo()
	{
		memset(this, 0, sizeof(tagDLMediaInfo));
	}

	u32 GetVersion()
	{
		return ntohl(m_dwVersion);
	}

	void SetVersion(const u32 dwVersion)
	{
		m_dwVersion = htonl(dwVersion);
	}

	u8  GetStreamID()
	{
		return m_byStreamID;
	}

	void SetStreamID(const u8 byStreamID)
	{
		m_byStreamID = byStreamID;
	}

	u8 GetPayLoad()
	{
		return m_byPayLoad;
	}

	void SetPayLoad(const u8 byPayLoad)
	{
		m_byPayLoad = byPayLoad;
	}

	s32 GetWidth()
	{
		return ntohl(m_tVideoInfo.m_nWidth);
	}

	void SetWidth(s32 nWidth)
	{
		m_tVideoInfo.m_nWidth = htonl(nWidth);
	}

	s32 GetHeigth()
	{
		return ntohl(m_tVideoInfo.m_nHeight);
	}

	void SetHeigth(s32 nHeigth)
	{
		m_tVideoInfo.m_nHeight = htonl(nHeigth);
	}

	s32 GetVideoClockRate()
	{
		return ntohl(m_tVideoInfo.m_nClockRate);
	}

	void SetVideoClockRate(s32 nClockRate)
	{
		m_tVideoInfo.m_nClockRate = htonl(nClockRate);
	}

	s32 GetBitRate()
	{
		return ntohl(m_tVideoInfo.m_nBitRate);
	}

	void SetBitRate(s32 nBitRate)
	{
		m_tVideoInfo.m_nBitRate = htonl(nBitRate);
	}

	s32 GetFrameRate()
	{
		return ntohl(m_tVideoInfo.m_nFrameRate);
	}

	void SetFrameRate(s32 nFrameRate)
	{
		m_tVideoInfo.m_nFrameRate = htonl(nFrameRate);
	}

	s32 GetKeyFrameIntvl()
	{
		return ntohl(m_tVideoInfo.m_nKeyFrameIntvl);
	}

	void SetKeyFrameIntvl(s32 nKeyFrameIntvl)
	{
		m_tVideoInfo.m_nKeyFrameIntvl = htonl(nKeyFrameIntvl);
	}

	s32 GetAudioClockRate()
	{
		return ntohl(m_tAudioInfo.m_nClockRate);
	}

	void SetAudioClockRate(s32 nClockRate)
	{
		m_tAudioInfo.m_nClockRate = htonl(nClockRate);
	}

	s32 GetSampleRate()
	{
		return ntohl(m_tAudioInfo.m_nSampleRate);
	}

	void SetSampleRate(s32 nSampleRate)
	{
		m_tAudioInfo.m_nSampleRate = htonl(nSampleRate);
	}

	s32 GetChannelNum()
	{
		return ntohl(m_tAudioInfo.m_nChannelsNum);
	}

	void SetChannelNum(s32 nChannelNum)
	{
		m_tAudioInfo.m_nChannelsNum = htonl(nChannelNum);
	}

private:
	u32     m_dwVersion;								///<下载的版本号
	u8      m_byStreamID;								///<媒体流编号
	u8      m_byPayLoad;								///<媒体的载荷类型
	u16     m_wReserved;								///<保留字段

	union
	{
		TVideoStreamProperty   m_tVideoInfo;			///<视频媒体属性信息
		TAudioStreamProperty   m_tAudioInfo;			///<音频媒体属性信息
	};

}TRPDLMediaInfo;

/** 下载媒体流属性集*/
typedef struct tagRPDLMediaInfoSet
{
	u8              m_byStreamNum;				///<媒体流数量
	TRPDLMediaInfo  m_atMediaInfo[MAX_STREAM_NUM];	///<媒体流信息
}TRPDLMediaInfoSet;

/**
 *@brief			媒体信息回调函数定义
 *@param[out]		u32 dwSSid			下载任务的唯一标识，即使用RpFtpRcvCreate()接口返回的ssid值
 *@param[out]		TRPDLMediaInfoSet *ptMediaInfoSet	媒体信息集
 *@param[out]		KD_PTR pvContext		回调上下文
 *@return		
 *@ref
 *@see
 *@note				
 */
typedef void(*RpFtpMediaInfoCallBack)(u32 dwSSid, TRPDLMediaInfoSet *ptMediaInfoSet, KD_PTR pvContext);

/** PROXY数据的最大长度*/
#define		MAX_PROXY_USER_DATA		16

typedef enum
{
	ERP_FileType_Unknown = 0,
	ERP_FileType_Asf,
	ERP_FileType_Mp4,
}ERPFileType;

/** ftp客户端接收参数结构*/
typedef struct tagFtpUdpRcvParam
{
	s8	m_abyUserId[MAX_FTP_RCV_USER_ID_LEN+1];				///<下载客户端用户编号，可以不填
	TNetAddr m_tRcvLocalAddr;								///<下载客户端的本地地址
	TNetAddr m_tRcvRemoteAddr;								///<下载服务端的地址
	RpFtpRcvCallBack m_pRcvCallBack;						///<下载状态回调函数
	KD_PTR m_pvContext;									///<回调上下文
	s8 m_abyLocalFilePath[MAX_FILE_PATH_LEN];				///<下载客户端本地文件存储路径
	s8 m_abyLocalFileName[MAX_FILE_NAME_LEN];				///<下载客户端本地存储文件名
	ERPFileType  m_eLocalFileType;							///<下载到本地的文件保存的格式，如果不设置，即为ERP_FileType_Unknown，
														///<则通过m_abyLocalFileName参数中的后缀确定文件格式
	u32 m_dwGran;											///<下载进度的回调粒度，单位秒
	u32 m_dwStartTime;									///<下载开始时间，绝对时间
	u32 m_dwStopTime;										///<下载结束时间，绝对时间，0表示下载到文件尾，非0时，必须大于下载开始时间
	s8 m_abyRequestFilePath[MAX_FILE_PATH_LEN];				///<请求下载的服务器端文件所在路径,Linux系统下的通用文件系统，必须以/mnt/开头
	s8 m_abyRequestFileName[MAX_FILE_NAME_LEN];				///<请求下载的服务器端文件的文件名
	BOOL32 m_bSmallFile;									///<是否小文件，如果m_abyRequestFilePath目录下面存在reclog.txt文件，则设置为TRUE
	u8	m_abyProxyUserData[MAX_PROXY_USER_DATA];				///<代理用户数据
	u32 m_dwProxyUserDataLen;								///<代理用户数据长度

	RpFtpDataCallback m_pfCallback ;						///<下载数据回调
	RpFtpMediaInfoCallBack  m_pfMediaInfoCallBack;			///<下载媒体信息回调
	s32			m_dwRcvBuf;								///<下载接收端缓冲大小，单位字节，取值范围[8KB,2000KB],0表示使用默认值DEFAULT_RP_DOWNLOAD_CLIENT_BUFF
	s32			m_nSpeedLimit;							///<接收端速度限制，目前无效
	u8          m_byDownLoadType;							///<下载的文件类型，比如RP_NORMAL_DOWN等，可以不填
	tagFtpUdpRcvParam()
	{
		m_dwProxyUserDataLen = 0;
		m_dwRcvBuf = DEFAULT_RP_DOWNLOAD_CLIENT_BUFF;
		m_nSpeedLimit = -1;
		m_pRcvCallBack = NULL;
		m_pfMediaInfoCallBack = NULL;
		m_pfCallback = NULL;
		m_eLocalFileType = ERP_FileType_Unknown;
	}
}TFtpUdpRcvParam;

/** ftp发送服务端查询结构*/
typedef struct tagFtpSndQuery
{
	u32 dwClientNum;										///<客户端数,也即下列数组元素个数
	TFtpSndAllParam m_atFtpSndAllParam[MAX_RP_FTP_SND_NUM];	///<客户端请求下载的相应参数
	TFtpUdpSndParam m_tFtpUdpSndParam[MAX_RP_FTP_SND_NUM];	///<客户端请求下载的相应参数
}TFtpSndQuery;

/** ftp客户端查询结构*/
typedef struct tagFtpRcvQuery
{
	u32 dwStauts;											///<下载接收端目前状态
}TFtpRcvQuery;

/** 磁盘信息*/
typedef struct tagRPDiskInfo
{
	u8 m_byFsType;							///<文件系统类型
	u64 m_llPhysicsSize;						///<磁盘分区大小，单位MB
	u64 m_llUsedSize;							///<已使用磁盘分区大小，单位MB
	u64 m_llFreeSize;							///<可用磁盘分区大小，单位MB
	u64 m_llLockSize;							///<磁盘锁定空间大小，单位MB
}TRPDiskInfo;

/**
 *@brief			录放像库初始化函数
 *@param[in]		const TRPInitParam *ptRPInitParam	初始化参数
 *@param[in]		TSysGlobalParam	*ptRPSysParam		系统全局参数，设置为NULL时，将使用默认值
 *@param[in]		BOOL32 bUsedRawSocket				是否使用裸套结字
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该参数必须在使用RPCTRL库的所有其他功能前被调用	
 */
u16 RPInit( const TRPInitParam *ptRPInitParam, TSysGlobalParam	*ptRPSysParam = NULL, BOOL32 bUsedRawSocket = FALSE);

/**
 *@brief			录放像库反初始化函数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口被调用后，将不能再使用RPCTRL库的其他任何功能	
 */
u16 RPRelease( );

/**
 *@brief			添加需要检测的磁盘分区信息
 *@param[in]		s8 *pchDiskMountName	磁盘分区的挂载点，比如ext3分区：/mnt/UUID，vbfs分区：/vbfs/UUID,
 *@param[in]		u32 dwAlarmLvlOne		磁盘分区剩余空间的一级告警阈值
 *@param[in]		u32 dwAlarmLvlTwo		磁盘分区剩余空间的二级告警阈值
 *@param[in]		const s8* pchDiskName	磁盘分区对应的盘符名，比如/dev/sda1
 *@param[in]		BOOL32 bMount			磁盘分区是否为挂载分区，针对linux系统的通用文件系统有用
 *@param[in]		BOOL32 bCheckDisk       是否检测磁盘分区
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16 RPSetCheckPartion( s8 *pchDiskMountName , u32 dwAlarmLvlOne, u32 dwAlarmLvlTwo, const s8* pchDiskName = NULL, BOOL32 bMount = FALSE, BOOL32 bCheckDisk = TRUE);

/**
 *@brief			设置磁盘检测间隔
 *@param[in]		u32 dwInterval	磁盘分区检测间隔，单位秒
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			如果不使用该接口设置检测间隔，内部默认5s检测一次	
 */
u16 RPSetDiskCheckInterval(  u32 dwInterval );

/**
 *@brief			设置磁盘断链超时时间，超过此时间磁盘无响应上报磁盘断链
 *@param[in]		s8 * pchPartitionName	磁盘分区名，与RPSetCheckPartion接口传入的第一个参数保持一致
 *@param[in]		u32 dwSec			磁盘检测超时时间，单位秒
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			如果不使用该接口设置检测超时时间，内部默认15s超时	
 */
u16 RPSetDiskCheckTimeOut(s8 * pchPartitionName, u32 dwSec);

/**
 *@brief			检查磁盘状态，确认磁盘是否在线
 *@param[in]		s8 * pchPartitionName	磁盘分区名，与RPSetCheckPartion接口传入的第一个参数保持一致
 *@param[in]		u32 dwWaitMillSec		等待检测时间，单位毫秒
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16 RPCheckDiskState(s8 * pchPartitionName, u32 dwWaitMillSec) ;

/**
 *@brief			删除不需要再检测的磁盘分区，比如分区已经下线
 *@param[in]		s8 * pchPartitionName	磁盘分区名，与RPSetCheckPartion接口传入的第一个参数保持一致
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16  RPDeleteCheckPartion( s8 *pchPartionName );
	
/**
 *@brief			创建录像机，得到可用的录像机编号
 *@param[out]		u32 *pdwRecoderID		可用的录像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16 RPCreateRec( u32 *pdwRecoderID );

/**
 *@brief			释放录像机
 *@param[in]		u32 dwRecoderID		需要释放的录像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16 RPReleaseRec( u32 dwRecoderID );

/**
 *@brief			设置录像机的网络接收参数
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		const TRPNetRcvParam *ptNetRcvParam		网络接收参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16 RPSetRecNetRcvParam( u32 dwRecorderID, const TRPNetRcvParam *ptNetRcvParam );

/**
 *@brief			设置码流平均码率
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		const TStreamAverageBitrate& tStreamAverageBitrate	平均码率
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			目前无效	
 */
u16 RPSetAverageBitrate(u32 dwRecorderID, const TStreamAverageBitrate& tStreamAverageBitrate);

/**
 *@brief			设置接收码流的载荷信息
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		const TRemotePTParam *ptPTParam		码流的载荷值
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口只在使用kdvmedianet库接收码流时有效	
 */
u16 RPSetRemotePT( u32 dwRecoderID, const TRemotePTParam *ptPTParam );

/**
 *@brief			设置解密参数
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		const TEncDecParam *ptDecryptParam	解密参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口只在使用kdvmedianet库接收码流时有效	
 */
u16 RPSetDecryptKey( u32 dwRecorderID, const TEncDecParam *ptDecryptParam );

/**
 *@brief			设置录像类型
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		u8 RecordType			录像类型，比如RP_NORMAL_RECORD，RP_UNKNOWN_KEDA_RECORD等
 *@param[in]		const s8* pbyCompanyName	厂商信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16 RPSetRecordType(u32 dwRecorderID , u8 RecordType, const s8* pbyCompanyName = NULL/*add by jh*/);

/**
 *@brief			使用给定录像机ID开始录像
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		const TRecParam *ptRecParam	录像参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16 RPStartRec( u32 dwRecorderID , const TRecParam *ptRecParam );

/**
 *@brief			请求切换录像文件进行继续录像，不停止码流
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		const s8 * pszPartitionName	录像文件切换的目的分区，NULL表示在原分区上继续录像
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note				
 */
u16 RPSwitchRec( u32 dwRecorderID, const s8 * pszPartitionName = NULL);

/**
 *@brief			选择媒体流进行录像
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		u8 byStreamId			媒体流编号，1：音频，2/3：视频
 *@param[in]		BOOL32 bSave			是否录制
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			默认是所有流都录制，因此如果不想录像某条流，可以使用该接口设置不录制，
 *                但是必须保证录制一条视频流
 */
u16 RPSetRecStreamSave( u32 dwRecorderID, u8 byStreamId, BOOL32 bSave );

/**
 *@brief			暂停录像
 *@param[in]		u32 dwRecoderID		录像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			调用该接口后，将暂停码流的接收，从而达到暂停录像的目的
 */
u16 RPPauseRec( u32  dwRecorderID );

/**
 *@brief			继续录像
 *@param[in]		u32 dwRecoderID		录像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			调用该接口后，将恢复码流的接收，从而达到继续录像的目的
 */
u16 RPResumeRec( u32 dwRecorderID );

/**
 *@brief			设置录像机的录制方法：抽帧录像或者正常录制,针对视频帧而言
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		BOOL32 bOnlySaveKeyFrame	是否只录关键帧
 *@param[in]		u32 dwSaveFrameInterval	关键帧间隔，即几个关键帧录制一个关键帧，目前该参数无效，即会录制所有关键帧
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			默认行为是录制所有的帧数据，除非使用该接口改变录制方法
 */
u16 RPSetRecorderSaveMethod( u32 dwRecorderID, BOOL32 bOnlySaveKeyFrame = FALSE, u32 dwSaveFrameInterval = 1 );

/**
 *@brief			停止指定录像机的录像
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		u32 dwStopTime		停止时间，目前无效
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口调用后，会立即停止网络接收，但是文件会等到缓冲中的数据都写入之后再关闭
 *                因此不要在该接口返回后，立即使用RPReleaseRec接口释放录像资源，否则会丢失部分
 *                录像数据，请收到RECORDER_REC_STOP回调消息之后，再释放录像资源
 */
u16 RPStopRec( u32 dwRecorderID, u32 dwStopTime = 0 );

/**
 *@brief			设置录像机的重传处理参数
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		TRPRecRSParam *ptRecRSParam	重传参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPSetRecRsParam( u32 dwRecorderID, TRPRecRSParam *ptRecRSParam );

/**
 *@brief			使用指定的录像机开始预录
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		BOOL32 bPreRecInMem	是否使用内存预录
 *@param[in]		u32 dwPreRecTimeInterval	预录时长，单位秒
 *@param[in]		const TRecParam *ptRecParam	录像参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPStartPreRec( u32 dwRecorderID , BOOL32 bPreRecInMem, u32 dwPreRecTimeInterval, const TRecParam *ptRecParam );

/**
 *@brief			停止预录
 *@param[in]		u32 dwRecoderID		录像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPStopPreRec( u32 dwRecorderID );

/**
 *@brief			得到当前预录总时间
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[out]		u32& dwPreRecTime		预录时长，单位秒
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetPreRecTime( u32 dwRecorderID , u32& dwPreRecTime );

/**
 *@brief			获取当前正在录制的录像文件的总长度
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[out]		u64& qwRecordTotalLength	录像文件的长度，单位字节
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetRecordTotalLength( u32 dwRecorderID , u64& qwRecordTotalLength );

/**
 *@brief			获取正在录制的录像文件的总时长
 *@param[in]		u32 dwRecoderID		录像机ID
 *@param[in]		const s8* szFileName	录像文件名
 *@param[out]		u32& dwPlayDuration	录像文件的时长，单位毫秒
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetRecDuration(u32 dwRecorderID,const s8* szFileName, u32& dwPlayDuration);

/**
 *@brief			停止录像并释放录像资源
 *@param[in]		u32 dwRecorderID		录像机ID
 *@param[in]		BOOL32 bDelete		是否同时删除录象文件夹 尽量不要将这个参数设为TRUE！！可能会导致主任务较大的延迟！！
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPStopRecAndRelease(u32 dwRecorderID , BOOL32 bDelete = FALSE ) ;

/**
 *@brief			设置录像机状态回调函数
 *@param[in]		u32 dwRecorderID		录像机ID
 *@param[in]		RecorderStateCallback pfStatCB	录像状态回调函数
 *@param[in]		KD_PTR hUserHandle	回调上下文
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPSetRecStateCallback(u32 dwRecorderID, RecorderStateCallback pfStatCB, KD_PTR hUserHandle);

/**
 *@brief			创建放像机，得到可用的放像机编号
 *@param[out]		u32 *pdwPlayerID		有效的放像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPCreatePlay(u32 *pdwPlayerID);

/**
 *@brief			设置放像机状态回调函数
 *@param[in]		u32 dwPlayerId		放像机ID
 *@param[in]		PlayerStateCallback pfStateCB	放像状态回调函数
 *@param[in]		KD_PTR hUserHandle	回调上下文
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPSetPlayStateCallback(u32 dwPlayerId, PlayerStateCallback pfStateCB, KD_PTR hUserHandle);

/**
 *@brief			释放放像机
 *@param[in]		u32 dwPlayerId		放像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPReleasePlay( u32 dwPlayerID );

/**
 *@brief			停止放像并释放放像资源
 *@param[in]		u32 dwPlayerID		放像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPStopPlayAndRelease(u32 dwPlayerID ) ;
	
/**
 *@brief			设置文件播放进度回调的粒度
 *@param[in]		u32 dwPlayerID		放像机ID
 *@param[in]		u8 byGran			播放文件的进度回调粒度，单位秒，取值范围[MIN_READ_CALLBACK_GRAN,MAX_READ_CALLBACK_GRAN]
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			内部默认1秒回调一次进度，如果想改变回调粒度，可以使用该接口设置
 */
u16 RPSetReadCallBackGran( u32 dwPlayerID, u8 byGran );

/**
 *@brief			设置发送出去的码流使用的载荷值
 *@param[in]		u32 dwPlayerID		放像机ID
 *@param[in]		const TLocalPTParam *ptPTParam	载荷参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			在使用Kdvmedianet发送码流时有效
 */
u16 RPSetLocalPT (u32 dwPlayerID, const TLocalPTParam *ptPTParam );

/**
 *@brief			设置加密参数
 *@param[in]		u32 dwPlayerID		放像机ID
 *@param[in]		TEncDecParam *ptEncryptParam	加密参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			在使用Kdvmedianet发送码流时有效
 */
u16 RPSetEncryptKey( u32 dwPlayerID, TEncDecParam *ptEncryptParam );

/**
 *@brief			获取文件的媒体信息
 *@param[in]		u32 dwPlayerID		放像机ID
 *@param[in]		s8 *pchFullFileName	文件名
 *@param[out]		TRPMediaInfo *ptMediaInfo	媒体信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口一般在RPCreatePlay接口之后使用，用于获取文件的媒体信息
 */
u16 RPGetFileMediaInfo( u32 dwPlayerID, s8 *pchFullFileName, TRPMediaInfo *ptMediaInfo );

/**
 *@brief			获取文件信息
 *@param[in]		s8 *pchFullFileName	文件名
 *@param[out]		TRPFileInfo *ptFileInfo	文件信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetFileInfo( s8 *pchFullFileName, TRPFileInfo *ptFileInfo );

/**
 *@brief			获取文件的扩展信息
 *@param[in]		s8 *pchFullFileName	文件的完整文件名
 *@param[out]		TMediaInfoExt *ptMediaInfoExt	文件扩展信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetFileExtInfo(s8 *pchFullFileName, TMediaInfoExt *ptMediaInfoExt);

/**
 *@brief			锁定或者解锁文件
 *@param[in]		const s8* pchRecFilePath	文件所在路径
 *@param[in]		const s8* pchRecFileName	文件名
 *@param[in]		BOOL32 bLock				TRUE：锁定，FALSE：解锁
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			当希望某个录像文件不被覆盖或者人为删除时，可以使用该接口锁定录像文件
 */
u16 RPLockFile(const s8* pchRecFilePath, const s8* pchRecFileName, BOOL32 bLock, u64 *pqwFileSize = 0);

/**
 *@brief			使用指定的放像机进行网络放像
 *@param[in]		u32 dwPlayerID		放像机ID
 *@param[in]		TPlayParam *ptPlayParam	放像参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPStartPlay( u32 dwPlayerID , TPlayParam *ptPlayParam);

/**
 *@brief			设置放像模式
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		u32 byPlayMode	放像模式，取值参见EmPlayerEffect
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPSetPlayMode(u32 dwPlayerID, u32 byPlayMode);

/**
 *@brief			锁定放像模式
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		u32 byPlayMode	放像模式，取值参见EmPlayerEffect
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
//u16 RPLockPlayMode(u32 dwPlayerID, u32 byPlayMode);

/**
 *@brief			锁定放像模式
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		u32 byPlayMode	放像模式，取值参见EmPlayerEffect
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
//u16 RPUnLockPlayMode(u32 dwPlayerID,u8 byOnlyKeyframeRate=4);

/**
 *@brief			设置放像类型
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		u8 PlayType		放像类型，比如RP_NORMAL_PLAY等
 *@param[in]		const s8* pbyCompanyName	厂商名
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPSetPlayType(u32 dwPlayerID, u8 PlayType, const s8* pbyCompanyName = NULL/*add by jh*/);

/**
 *@brief			播放下一帧
 *@param[in]		u32 dwPlayerID	放像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			调用该接口后，将进入单帧播放模式
 */
u16 RPPlayNextFrame(u32 dwPlayerID) ;

/**
 *@brief			注册发送回调函数，kdvmedianet调用
 *@param[in]		TNetAddr* ptRcvAddr	网络接收地址
 *@param[in]		RPUdpPackRecvCallback pUdpCallBack	数据报接口回调函数
 *@param[in]		void* pvContext		回调上下文
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note		
 */
u16 RPRegSndChannel(TNetAddr* ptRcvAddr, RPUdpPackRecvCallback pUdpCallBack, void* pvContext) ;

/**
 *@brief			注销发送回调函数
 *@param[in]		TNetAddr* ptRcvAddr	网络接收地址
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note		
 */
u16 RPUnRegSndChannel(TNetAddr* ptRcvAddr) ;

/**
 *@brief			暂停播放
 *@param[in]		u32 dwPlayerID	放像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPPausePlay( u32 dwPlayerID );

/**
 *@brief			恢复播放
 *@param[in]		u32 dwPlayerID	放像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			在调用了RPPausePlay之后必须调用该接口才能恢复录像的播放
 */
u16 RPResumePlay( u32 dwPlayerID );

/**
 *@brief			停止指定放像机的网络放像
 *@param[in]		u32 dwPlayerID	放像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPStopPlay( u32 dwPlayerID );

/**
 *@brief			设置放像机的重传处理参数
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		TRPPlyRSParam *ptPlyRSParam	重传参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPSetPlyRsParam( u32 dwPlayerID, TRPPlyRSParam *ptPlyRSParam );

/**
 *@brief			设置快放速率
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		u8 byQuickTimes	快放速率
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPQuickPlay( u32 dwPlayerID , u8 byQuickTimes );

/**
 *@brief			设置慢放速率
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		u8 bySlowTimes	慢放速率
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPSlowPlay( u32 dwPlayerID , u8 bySlowTimes );

/**
 *@brief			拖拉到给定时间开始放像
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		s32 nDragDropTime	拖放目标时间，相对于RPStartPlay中给出的开始播放时间的偏移时间，单位秒
 *@param[in]		BOOL32 bSeekToKeyFrame		是否定位到关键帧开始播放
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPDragDropPlay( u32 dwPlayerID , s32 nDragDropTime, BOOL32 bSeekToKeyFrame = TRUE );

/**
 *@brief			从给定时间开始倒放
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		u32 dwReversePlayTime	倒放开始时间，在文件中的相对时间，单位毫秒
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPReversePlay( u32 dwPlayerID, u32 dwReversePlayTime);

/**
 *@brief			从给定时间开始正放
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[in]		u32 dwForwardPlayTime	正放开始时间，在文件中的相对时间，单位毫秒
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPForwardPlay( u32 dwPlayerID, u32 dwForwardPlayTime);

/**
 *@brief			查询指定录像机的录像状态
 *@param[in]		u32 dwRecorderID	录像机ID
 *@param[out]		TRecorderStatus *ptRecorderStatus	录像机的录像状态
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetRecorderStatus( u32 dwRecorderID, TRecorderStatus *ptRecorderStatus );

/**
 *@brief			查询指定放像机的放像状态
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[out]		TPlayerStatus *ptPlayerStatus	放像机的放像状态
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetPlayerStatus( u32 dwPlayerID, TPlayerStatus *ptPlayerStatus );

/**
 *@brief			查询指定录像机的录像统计信息
 *@param[in]		u32 dwRecorderID	录像机ID
 *@param[out]		TRecorderStatis *ptRecorderStatis	录像机的统计信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetRecorderStatis( u32 dwRecorderID, TRecorderStatis *ptRecorderStatis );

/**
 *@brief			查询指定放像机的放像统计信息
 *@param[in]		u32 dwPlayerID	放像机ID
 *@param[out]		TPlayerStatis *ptPlayerStatis	放像机的统计信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetPlayerStatis( u32 dwPlayerID, TPlayerStatis *ptPlayerStatis );

/**
 *@brief			打印指定录像机的录像状态
 *@param[in]		u32 dwRecorderID	录像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			目前该接口无效
 */
API u16 recstatus( u32 dwRecorderID );

/**
 *@brief			打印指定放像机的放像状态
 *@param[in]		u32 dwPlayerID	放像机ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			目前该接口无效
 */
API u16 plystatus( u32 dwPlayerID );

/**
 *@brief			设置mediapack的接收缓冲大小
 *@param[in]		u32 dwVideoBuffSize	Mediapack发送缓冲大小，对于非云存储上的文件播放有效
 *@param[in]		u32 dwKeyFrameNum		Mediapack中可以缓冲的最大关键帧数，对于云存储上的文件播放有效
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口为调试命令，主要在测试时使用，设置的参数在调用该命令之后开启的放像生效
 */
API void rpsmpvb(u32 dwVideoBuffSize, u32 dwKeyFrameNum);

/**
 *@brief			打印录放像库版本信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口为调试命令
 */
API u16 rpver();

/**
 *@brief			录放像库帮助命令
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
API u16 rphelp();

/**
 *@brief			获取文件的播放时长及开始时间
 *@param[in]		s8* pchPath		文件名	
 *@param[out]		u32& dwPlayDuration	文件的播放总时长
 *@param[out]		u32& dwFileStartTime	文件的开始时间，绝对时间
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetPlayDuration(s8* pchPath, u32& dwPlayDuration, u32& dwFileStartTime );

/**
 *@brief			列举指定目录下的文件
 *@param[in]		s8* pchPath		指定目录	
 *@param[out]		TListInfo *ptListInfo	文件列表
 *@param[in]		BOOL32 bListAll		是否列出目录下的所有文件，
 *									TRUE：列表中包含目录下所有存在的文件，包括非媒体文件
 *									FALSE：列表中只包含目录下的所有媒体文件
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPListFile( s8 *pchPath, TListInfo *ptListInfo, BOOL32 bListAll = FALSE );

/**
 *@brief			删除录像文件
 *@param[in]		s8* pchPath		文件目录	
 *@param[in]		s8* pchName		文件名
 *@param[in]		BOOL32 bBigFile	是否为大文件删除，TRUE：pchName不能设置为NULL
 *@param[in]		BOOL32 bDeleteAll	是否删除所有的文件，TRUE：删除所有文件，包括非媒体文件，FALSE：只删除媒体文件
 *@param[in]		u32 dwWaitTime	等待删除时间，单位毫秒，0表示使用系统默认的超时时间
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPDeleteFile( s8 *pchPath, s8* pchName = NULL, BOOL32 bBigFile = FALSE, BOOL32 bDeleteAll = FALSE, u32 dwWaitTime = 0);

/**
 *@brief			重命名指定文件
 *@param[in]		s8 *pchOldName	原文件名	
 *@param[in]		s8 *pchNewName	新文件名
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPRenameFile(s8 *pchOldName, s8 *pchNewName);

/**
 *@brief			查找文件或目录是否存在
 *@param[in]		s8 * pchFileName	文件名或目录名
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPIsFileExist(s8 * pchFileName);

/**
 *@brief			创建目录
 *@param[in]		s8 *pchDirName	目录名
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPCreateDir(s8 *pchDirName);

/**
 *@brief			设置媒体流发送时的IP数据包的TOS值
 *@param[in]		int nTOS		TOS值
 *@param[in]		int nType	媒体流类型，0:全部流 1:音频 2:视频 3: 数据
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			使用Kdvmedianet库，并且是裸套接字发送时有效
 */
int SetRPMediaTOS( int nTOS, int nType );

/**
 *@brief			获取IP数据包的TOS值
 *@param[in]		int nType	媒体流类型，0:全部流 1:音频 2:视频 3: 数据
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
int GetRPMediaTOS( int nType );

/**
 *@brief			创建下载接收对象，得到可用的下载接收编号
 *@param[out]		u8 *pbyFtpRcvID	有效的下载接受ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			使用TCP下载方式时，必须先使用该接口分配下载接收资源，不推荐使用TCP下载方式的所有接口
 */
u16 RPCreateFtpRcv( u8 *pbyFtpRcvID );

/**
 *@brief			释放下载接收对象
 *@param[in]		u8 byFtpRcvID		有效的下载接收资源ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			适用于TCP方式下载
 */
u16 RPReleaseFtpRcv( u8 byFtpRcvID );

/**
 *@brief			开始下载接收
 *@param[in]		u8 byFtpRcvID		有效的下载接收资源ID
 *@param[in]		TFtpRcvParam *ptFtpRcvParam	下载接收参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			适用于TCP方式下载
 */
u16 RPStartFtpRcv( u8 byFtpRcvID , TFtpRcvParam *ptFtpRcvParam );

/**
 *@brief			停止下载接收
 *@param[in]		u8 byFtpRcvID		有效的下载接收资源ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			适用于TCP方式下载
 */
u16 RPStopFtpRcv( u8 byFtpRcvID );
	
/**
 *@brief			创建下载发送对象，得到可用的下载发送编号
 *@param[out]		u8 *pbyFtpSndID	有效的下载发送ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			适用于TCP下载方式，下载发送端必须先使用该接口分配下载资源
 */
u16 RPCreateFtpSnd( u8 *pbyFtpSndID );
	
/**
 *@brief			释放下载发送对象
 *@param[in]		u8 byFtpSndID		有效的下载发送ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			适用于TCP下载方式
 */
u16 RPReleaseFtpSnd( u8 byFtpSndID );

/**
 *@brief			设置文件下载进度回调的粒度
 *@param[in]		u8 byFtpSndID		有效的下载发送ID
 *@param[in]		u8 byGran		下载进度回调粒度，单位秒，取值范围[MIN_READ_CALLBACK_GRAN,MAX_READ_CALLBACK_GRAN]
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			适用于TCP下载方式
 */
u16 RPSetFtpReadCallBackGran( u8 byFtpSndID, u8 byGran );

/**
 *@brief			开始下载发送
 *@param[in]		u8 byFtpSndID		有效的下载发送ID
 *@param[in]		TFtpSndParam *ptFtpSndParam	下载发送参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			适用于TCP下载方式
 */
u16 RPStartFtpSnd( u8 byFtpSndID , TFtpSndParam *ptFtpSndParam );

/**
 *@brief			停止下载发送
 *@param[in]		u8 byFtpSndID		有效的下载发送ID
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			适用于TCP下载方式
 */
u16 RPStopFtpSnd( u8 byFtpSndID );

/**
 *@brief			查询所有正在进行的TCP下载任务的信息
 *@param[out]		TFtpSndQuery& tFtpSndQuery	下载任务信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			使用于TCP下载方式
 */
u16 RpFtpSndQuery(TFtpSndQuery& tFtpSndQuery);

/** 注意：下面接口仅供codecwrapper调用，其它模块不要调用 !!! */

/**
 *@brief			根据录像机ID获取录像机地址
 *@param[in]		u32 dwRecId		录像机ID，通过RPCreateRec()接口得到
 *@param[out]		u64 *pllAddr		录像机地址
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 GetRecAddr( u32 dwRecId, u64 *pllAddr );

/**
 *@brief			录像数据输入函数
 *@param[in]		PFRAMEHDR pFrmHdr		需要录像的帧数据
 *@param[in]		void* pvContext		录像机上下文，TCBContext结构体指针，
 *                                       其中的m_llAddr值为通过GetRecAddr接口得到的录像机地址
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
void RecDataPrc( PFRAMEHDR pFrmHdr, void* pvContext );

/**
 *@brief			录像数据输入函数，针对PS流
 *@param[in]		u8 * pbPes		PES包
 *@param[in]		u32 dwLen		PES包长度
 *@param[in]		void* pvContext		录像机上下文，TCBContext结构体指针，
 *                                       其中的m_llAddr值为通过GetRecAddr接口得到的录像机地址
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			目前该接口未实现
 */
//void RecDataPrc( u8 * pbPes, u32 dwLen, void* pvContext);

/**
 *@brief			添加一路PS流
 *@param[in]		u32 dwRecorderID	录像机ID
 *@param[in]		u8 byStreamID		媒体流ID，PES包中的StreamID，0xE0,0xC0一类
 *@param[in]		u8 byStreamType	媒体流类型，见kdmpsts.h
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			目前该接口未实现
 */
//u16 RPAddPSStream( u32 dwRecorderID, u8 byStreamID, u8 byStreamType);

/**
 *@note	以下为udp方式下载的相关接口
 *       对于下载服务端: 先调用RpFtpStartSndService启动下载发送服务,
 *       再调用RpFtpSndProbePack发送探测包
*/

/**
 *@brief			启动下载的服务端服务
 *@param[in]		TFtpSndServParam tFtpSndServParam	下载服务端参数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口只需要在程序起来后，调用一次即可，不需要每执行一次下载任务就调用一次
 */
u16 RpFtpStartSndService(TFtpSndServParam tFtpSndServParam);

/**
 *@brief			停止下载的服务端服务
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口调用后，服务端将不再提供下载服务，建议在程序退出时调用即可
 */
u16 RpFtpStopSndService();

/**
 *@brief			服务端发送探测包接口
 *@param[in]		u8* pBuf		要发送的探测包数据
 *@param[in]		u32 dwBufLen	探测包数据长度
 *@param[in]		TNetAddr tRemoteAddr	发送的目的地址
 *@param[in]		u32 dwNatIntvl	发送探测包的频率，单位秒，0表示按上层的缓冲发送，且只发送一次
 *                                  非0表示RPCTRL库自动发送探测包，且忽略pBuf的内容
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RpFtpSndProbePack(u8* pBuf,	u32 dwBufLen, TNetAddr tRemoteAddr, u32 dwNatIntvl = 0);

/**
 *@brief			服务端过滤函数定义
 *@param[in]		u32 dwRecvIP	本地接收IP地址
 *@param[in]		u16 wRecvPort	本地接收端口
 *@param[in]		u32 dwSrcIP	数据源IP地址
 *@param[in]		u16 wSrcPort	数据源端口
 *@param[in]		u8* pData	数据内容缓冲
 *@param[in]		u32 uLen		数据内容长度
 *@return		0：表示需要继续处理，非0：数据包被过滤
 *@ref
 *@see
 *@note			
 */
typedef u32 (*FilterFunc)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 uLen);

/**
 *@brief			设置服务端过滤函数
 *@param[in]		FilterFunc ptFunc		过滤函数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RpSetFilterFun(FilterFunc ptFunc);

/**
 *@brief			创建下载客户端
 *@param[in]		TNetAddr& tLocalAddr	下载客户端本地使用的网络地址
 *@param[out]		u32* pdwSsid			有效的下载任务唯一标识
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RpFtpRcvCreate(TNetAddr& tLocalAddr, u32* pdwSsid );

/**
 *@brief			客户端发送探测包
 *@param[in]		u32 dwSsid		下载任务唯一标识
 *@param[in]		TNetAddr& tRemoteAddr	探测包发送的目的地址
 *@param[in]		u8* pBuf			探测包内容
 *@param[in]		u32 dwBufLen		探测包内容长度
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RpFtpRcvSndProbe(u32 dwSsid, TNetAddr& tRemoteAddr, u8* pBuf, u32 dwBufLen );

/**
 *@brief			设置下载类型
 *@param[in]		u8 DownloadType	下载类型
 *@param[in]		u32 dwSsid		下载任务唯一标识
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			该接口不再使用
 */
//u16 RpFtpDownloadType(u8 DownloadType, u32 dwSsid);

/**
 *@brief			客户端开始下载
 *@param[in]		TFtpUdpRcvParam tFtpRcvParam	下载接收参数
 *@param[in]		u32 dwSsid		下载任务唯一标识
 *@param[in]		u32 dwNatIntvl	探测包发送间隔，0表示不发送探测包，非0值表示发送探测包
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RpFtpStartRcv(TFtpUdpRcvParam tFtpRcvParam, u32 dwSsid, u32 dwNatIntvl/*=0*/);

/**
 *@brief			设置发送端的速率限制
 *@param[in]		s32 value	下载发送速率，bytes per sec，必须大于等于20480,0或者-1表示不限速
 *@return		
 *@ref
 *@see
 *@note			
 */
API void RpFtpSetSndSpeedLimit(s32 value );

/**
 *@brief			客户端停止下载
 *@param[in]		u32 dwSsid	下载任务唯一标识
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RpFtpStopRcv(u32 dwSsid);

/**
 *@brief			下载客户端状态查询
 *@param[in]		u32 dwSsid	下载任务唯一标识
 *@param[out]		TFtpRcvQuery& tFtpRcvQuery		客户端状态信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RpFtpRcvQuery(u32 dwSsid, TFtpRcvQuery& tFtpRcvQuery);

/**
 *@brief			设置下载客户端的重传时间间隔及重传次数
 *@param[in]		u32 dwRepeatInterval	重传时间间隔，单位毫秒
 *@param[in]		u32 dwRepeatCount		重传次数
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
API u16 RPSetFtpRepeatPara(u32 dwRepeatInterval , u32 dwRepeatCount);

/**
 *@brief			获取RPCTRL库网络丢包及IO延时统计信息
 *@param[out]		TRpStatics* pStaics	统计信息
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
u16 RPGetStatics(TRpStatics* pStaics);

/**
 *@brief			获取磁盘信息
 *@param[in]		s8* szDiskName	磁盘分区名
 *@param[out]		TRPDiskInfo *ptInfo	磁盘信息
 *@return		TRUE：成功，FALSE：失败
 *@ref
 *@see
 *@note			
 */
BOOL32 RPGetDiskInfo(s8* szDiskName, TRPDiskInfo *ptInfo);

/**
 *@brief			设置接收端的速率限制
 *@param[in]		u32 dwSsid	下载任务唯一标识
 *@param[in]		s32 nSpeed	速率，大于等于10240，0或者-1表示不限速
 *@return		
 *@ref
 *@see
 *@note			
 */
API u16 RpFtpSetRcvSpeedLimit(u32 dwSsid, s32 nSpeed);

/**
 *@brief			下载功能的调试命令开关
 *@param[in]		u32 nDebug	调试命令级别
 *							1--除了其他级别的打印信息基本所有的打印都会打开
 *							2--重传请求相关打印
 *							3--下载数据的时间戳及进度相关打印
 *							4--客户端限速打印
 *							5--下载参数打印
 *							6--服务端重传包发送打印
 *@return		
 *@ref
 *@see
 *@note			
 */
API void rpftpdebug(u32 nDebug);

API void rpdlsto(u32 dwDownloadSndTimeout);

API void rpdlrto(u32 dwDownloadRecvTimeout);

/**
 *@brief			下载服务端服务信息打印，包括下载任务数，下载线程状态，下载服务端地址
 *@return		
 *@ref
 *@see
 *@note			telnet调试命令
 */
API void rpftpsnd();

/**
 *@brief			下载客户端信息打印，包括下载任务数，下载线程状态，下载客户端线程的WATCH套结字地址
 *@return		
 *@ref
 *@see
 *@note			telnet调试命令
 */
API void rpftprcv();

/**
 *@brief			dataswitch注册接收函数定义
 *@param[in]		void* ptNetAddr	网络接收地址	
 *@param[in]		RPUdpPackRecvCallback pUdpCallBack	网络数据包接收回调函数
 *@param[in]		KD_PTR pvContext	回调上下文
 *@return		1：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
typedef u32 (*dsRegRcvChannelP)(void* ptNetAddr, RPUdpPackRecvCallback pUdpCallBack, KD_PTR pvContext);

/**
 *@brief			dataswitch反注册接收函数定义
 *@param[in]		void* ptNetAddr	网络接收地址	
 *@param[in]		RPUdpPackRecvCallback pUdpCallBack	网络数据包接收回调函数
 *@param[in]		KD_PTR pvContext	回调上下文
 *@return		1：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
typedef u32 (*dsUnRegRcvChannelP)(void* ptNetAddr, RPUdpPackRecvCallback pUdpCallBack, KD_PTR pvContext);

/**
 *@brief			设置注册函数
 *@param[in]		dsRegRcvChannelP p	注册函数	
 *@return		
 *@ref
 *@see
 *@note			
 */
void RPSetDSRegPointer(dsRegRcvChannelP p);

/**
 *@brief			设置反注册函数
 *@param[in]		dsUnRegRcvChannelP p	反注册函数	
 *@return		
 *@ref
 *@see
 *@note			
 */
void RPSetDSUnRegPointer(dsUnRegRcvChannelP p);

/**
 *@note 以下接口在下载第三方厂商的前端录像时使用，目前主要是G网关使用
 */

/** 第三方下载文件信息*/
typedef struct tag3rdComDlFileInfo
{
	u32 m_dwStartTime;						///<下载开始时间，绝对时间
	u32 m_dwStopTime;							///<下载结束时间，绝对时间
	s8 m_abyFilePath[MAX_FILE_PATH_LEN];		///<待下载的文件所在路径
	s8 m_abyFileName[MAX_FILE_NAME_LEN];		///<待下载的文件的文件名
}T3rdComDlFileInfo;

/** 第三方文件下载时的媒体属性信息*/
typedef struct tag3rdStreamProperty
{
	u8 m_byStreamID;							///<媒体流序号	
	u8 m_byMediaType;							///<媒体类型	97:MPEG4, 106:H264
	union
	{
		TVideoStreamProperty   m_tVideoInfo;	///<视频流属性
		TAudioStreamProperty   m_tAudioInfo;	///<音频流属性
	};
}T3rdStreamPropery;

/** 第三方文件下载时的媒体属性信息集*/
typedef struct tag3rdStreamPropertySet
{
	u8                  m_byTotalStreamNum;					///<媒体流数量
	T3rdStreamPropery		m_at3rdStreamProperty[MAX_STREAM_NUM];	///<媒体流属性信息
}T3rdStreamPropertySet;

/** 第三方文件下载时的媒体流数据*/
typedef struct tag3rdComStreamPayLoad
{
	u8		m_byStreamID;					///<媒体流序号
	BOOL32	m_bIsKeyFrame;					///<载荷数据是否为关键帧
	u32		m_dwPresentationTime;				///<载荷数据的时间戳，单位毫秒
	u32		m_dwPayLoadSize;					///<载荷大小
	u8		*m_pbyPayLoad;					///<载荷实际数据，内存由提供数据者管理
}T3rdComStreamPayLoad;

/**
 *@brief			开始第三方录像下载函数定义
 *@param[in]		const u32 dwSsid		下载任务唯一标识	
 *@param[in]		T3rdComDlFileInfo t3ComDlFileInfo	下载的文件信息
 *@param[out]		T3rdStreamPropertySet *pt3rdStreamPropertySet	文件媒体属性信息集合
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
typedef u16 (*RP3rdComStartDownLoad)(const u32 dwSsid, T3rdComDlFileInfo t3ComDlFileInfo, T3rdStreamPropertySet *pt3rdStreamPropertySet);

/**
 *@brief			获取下载文件的媒体数据
 *@param[in]		const u32 dwSsid		下载任务唯一标识	
 *@param[out]		BOOL32* pbReadFileEnd	是否到达文件尾
 *@param[out]		T3rdComStreamPayLoad *pt3ComStreamPayLoad	媒体数据
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			
 */
typedef u16 (*RP3rdComGetStreamData)(const u32 dwSsid, BOOL32* pbReadFileEnd, T3rdComStreamPayLoad *pt3ComStreamPayLoad);

/**
 *@brief			停止第三方录像下载
 *@param[in]		const u32 dwSsid		下载任务唯一标识	
 *@return		
 *@ref
 *@see
 *@note			
 */
typedef void (*RP3rdComStopDownLoad)(const u32 dwSsid);

/** 第三方下载的操作函数集*/
typedef struct tag3rdComDlOperSet
{
	RP3rdComStartDownLoad  m_pf3rdComStartDownLoad;		///<开始下载函数
	RP3rdComGetStreamData  m_pf3rdComGetStreamData;		///<获取下载数据函数
	RP3rdComStopDownLoad   m_pf3rdComStopDownLoad;		///<停止下载函数
}T3rdComDlOperSet;

/**
 *@brief			注册下载第三方录像文件的操作方法
 *@param[in]		T3rdComDlOperSet t3rdComDlOperSet	下载操作集	
 *@return		RP_OK：成功，其他值：失败
 *@ref
 *@see
 *@note			出于多路下载的考虑，请保证方法集中各方法的可重入性
 */
u16 RP3rdComDlOperSetRegister(T3rdComDlOperSet t3rdComDlOperSet);

#endif  //_RPCtrl_H_
