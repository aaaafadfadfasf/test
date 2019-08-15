/*=======================================================================
模块名      :
文件名      : nmsmacro.h
相关文件    :
文件实现功能 :
作者        : 李洪强
版本        : V3.9  Copyright(C) 2003-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2006/02/22  3.9         李洪强      根据3.9的设计重新整理
=======================================================================*/
#ifndef NMS_MACRO_H
#define NMS_MACRO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "kdmdef.h"
#include "eventid.h"

#define NMS_APP_NAME            "NmsServer 0xbe8e2ce1"
#define NMS_WATCHDOG_APP_NAME   "NmsWatchDog 0xbe8e2ce1"
#define NMS_WATCHDOG_PORT       (u16)7777

//////////////////////////////////////////////////////////////////////////
// NMS看门狗消息
// 向看门狗注册进程ID
OSPEVENT(EV_WATCHDOG_REG_PROCESSID,             EV_NMS_BGN + 300);
// 向看门狗注销
OSPEVENT(EV_WATCHDOG_UNREG_NMS,                 EV_NMS_BGN + 302);
// 看门狗索取食物消息
OSPEVENT(EV_WATCHDOG_ASK_FOOD,                  EV_NMS_BGN + 304);
// NMS喂狗消息
OSPEVENT(EV_WATCHDOG_FEED_DOG,                  EV_NMS_BGN + 305);
// 启动看门狗定时消息
OSPEVENT(EV_WATCHDOG_START_DOG_TIMER,           EV_NMS_BGN + 306);
// 向看门狗注销返回ACK消息定时
OSPEVENT(EV_WATCHDOG_UNREG_ACK_TIMER,           EV_NMS_BGN + 307);
// 上传版本的进试定时
OSPEVENT(EV_UPDATE_PROGRESS_TIMER,              EV_NMS_BGN + 310);
// 公用单元定时器的定时
OSPEVENT(EV_PUB_UNIT_TIMER,                     EV_NMS_BGN + 315);
//取网管服务器CPU信息定时器
OSPEVENT(EV_GET_CPUINFO_TIMER,                  EV_NMS_BGN + 318);
//向看门狗注册消息号
OSPEVENT(EV_WATCHDOG_REG_MSG,                   EV_NMS_BGN + 320);
//看门狗初始化成功消息
OSPEVENT(EV_DOG_INIT_MSG,                       EV_NMS_BGN + 323);
//请求topo消息定时器
OSPEVENT(EV_ASK_TOPO_TIMER,                     EV_NMS_BGN + 325);
#ifdef _PRIVATENET_
//内存数据库中内容导入磁盘文件定时器
OSPEVENT(EV_LOADOUT_MEMDB_TIMER,                EV_NMS_BGN + 330);
#endif

//喂狗的app的类型
#define NMC_APP_TYPE                            (u8)0
#define SVR_APP_TYPE                            (u8)1

//服务器向看门狗的注册结构
typedef struct tagNmsRegDog
{
    tagNmsRegDog()
    {
        m_dwProcessId = 0;
        m_dwFeedInterval = 0;
        m_wStaveCount = 0;
    }
    u32 m_dwProcessId;
    u32 m_dwFeedInterval;
    u16 m_wStaveCount;
    u16 wReserved;
}TNmsRegDog;


#define START_DOG_TIMER_INTERVAL                1000        // 每秒钟尝试启动和连接看门狗一次
#ifdef WIN32
#define DOG_ASK_FOOD_INTERVAL                   10000       // 每秒钟需要喂一次
#endif
#ifdef _LINUX_
#define DOG_ASK_FOOD_INTERVAL                   10000       // 每秒钟需要喂一次 LINUX下等街时间长一些
#endif // _LINUX_
#define MAX_TRY_TIMES_ASK_FOOD                  3           // 最多多少次得不到食物就重启NMS
#define MAX_TIME_FROM_UNREG_ACK                 1000        //注销返回ACK超时时间
#define PUB_UNIT_TIME_INTERVAL                  1000        // 公用定时器一秒回调一次

#define UPDATE_PROGRESS_TIMEOUT                 1000        // 每1秒上报一次进度

#define GET_CPUINFO_TIMEOUT                     3000        // 每隔3秒取一次cpu信息
#define ASK_TOPO_TIMEOUT                        15000       // 15秒去拓扑超时

#define MAX_LOADOUT_MEMDB_TIMEOUT               1800000     //每隔三十分钟备份一次内存数据库


//向客户端发送数据的数组长度
#define ARRAY_DEVICE_LEN        (u32)40
#define ARRAY_USER_LEN          (u32)20
#define ARRAY_ALARM_LEN         (u32)50

// 操作结果
#define SUCESS_VALUE            (u8)0           // 操作成功
#define FAILE_VALUE             (u8)1           // 操作失败

//用户操作消息类型
#define MSG_TYPE_GROUP          (u8)1           // 用户组消息
#define MSG_TYPE_USER           (u8)2           // 用户消息

#define INVALID_INDEX           (u32)0xFFFFFFFF  // 无效的索引号

#define ROOT_PARENTDEVICE_SN    (u32)0           //根节点的父设备流水号

#define NMS_DEVICE_SN           (u32)1           // 网管服务器本身的设备流水号

#define INVALID_SERIAL_NO       (u32)0xFFFFFFFF  // 非法的ID(用于用户,用户组, 设备等所有由NMS生成的流水号等)
#define SUPER_ADMIN_SN          (u32)0x00000001  // 超级管理员的用户流水号
#define SUPER_ADMIN_GROUP_ID    (u32)0x00000000  // 超级管理员的用户组号



#define MASTER_NMS              (u8)0x1         //主网管(管理3as)
#define SLAVE_NMS               (u8)0x0         //非主网管

//系统运行的网络类型
#define PUBLIC_NET              (u8)0x0         //公网
#define PRIVATE_NET             (u8)0x1         //专网


//EMAIL服务器认证方式
#define NMS_EMAIL_NO_AUTH               (u8)0   //不需认证
#define NMS_EMAIL_AUTH_LOGIN            (u8)1   //login
#define NMS_EMAIL_AUTH_PLAIN            (u8)2   //plain
#define NMS_EMAIL_AUTH_CRAM_MD5         (u8)3   //cram_md5
#define NMS_EMAIL_AUTH_DIGEST_MD5       (u8)4   //digest_md5


// 错误码
enum
{
    ERROR_CMD_SUCCESS = 0,                  // 操功成功
    // 登录
    ERROR_NMS_INIT_FAIL,                    // 网管服务器初始化失败
    ERROR_USER_ID_PWD,                      // 登录用户名或密码错误
    ERROR_IP_RESTRICT,                      // 登录IP被限制
    ERROR_EXCEED_MAX_NMC,                   // 客户端登录数达到上限

    // 用户管理
    ERROR_USER_ONLINE,                      // 用户正在登录
    
    ERROR_ADD_GROUP_ALREADY_EXIST,          // 新增用户组已存在
    ERROR_ADD_GROUP_BEYOND_LIMIT,           // 系统用户组数已达上限
    ERROR_MODIFY_GROUP,                     // 修改用户组出错
    ERROR_DEL_GROUP_NOT_EXIST,              // 删除用户组不存在

    ERROR_ADD_USER_ALREADY_EXIST,           // 新增用户已存大
    ERROR_ADD_USER_BEYOND_LIMIT,            // 系统用户数已经达到上限
    ERROR_MODIFY_USER,                      // 用户资料修改出错
    ERROR_CHANGE_PWD,                       // 修改密码失败
    ERROR_DEL_USER_NOT_EXIST,               // 删除的用户不存在

    // 配置管理
    ERROR_GET_DEV_CFG,                      // 获取设备配置参数失败
    ERROR_SET_DEV_CFG,                      // 设置设备配置参数失败 

    // 告警
    // IP规则
    ERROR_ADD_IPRULES_ALREADY_EXIST,        // 添加的IP规则已存在
    ERROR_MODIFY_IPRULES,                   // 修改IP规则出错
    ERROR_DEL_IPRULES_NOT_EXIST,            // 删除IP规则不存在

    // 告警过滤规则
    ERROR_ADD_ALARM_FILETER_ALREADY_EXIST,  // 添加的告警过滤规则已存在
    ERROR_MODIFY_ALARM_FILETER,             // 修改告警过滤规则出错
    ERROR_DEL_ALARM_FILETER_NOT_EXIST,      // 删除告警过滤规则不存在

    // 邮件发送规则
    ERROR_ADD_MAIL_RULES_ALREADY_EXIST,     // 添加的邮件发送规则已存在
    ERROR_MODIFY_MAIL_RULES,                // 修改邮件发送规则出错
    ERROR_DEL_MAIL_RULES_NOT_EXIST,         // 删除邮件发送规则不存在

    ERROR_ALARM_QUERY_NOMATCH,              // 没有符合查询条件的告警
    ERROR_ALARM_QUERY_FAIL,                 // 告警查询失败

    ERROR_ACTION_QUERY_NOMATCH,             // 没有符合查询条件的操作日志
    ERROR_ACTION_QUERY_FAIL,                // 日志查询失败

    ERROR_PING_DEVICE_CANNOT_ARRIVE,        //达到不了要Ping的设备
    ERROR_PING_DEVICE_TIMEOUT,              //ping超时

    //数据库备份与恢复
    ERROR_BACKUP_DB_PATH_NOTEXIT,               //备份文件路径不存在
    ERROR_RESTORE_DB_FILE_NOTEXIT,              //要恢复的数据文件不存在 
    ERROR_DBCHARSET_NOMATCH,                 //数据库字符集不匹配
    ERROR_DB_FAULT_OR_BUSY,                     //数据库操作超时

    //系统错误
    ERROR_NMS_START_ERR_OR_FAIL,             //服务器启动失败或出错

    //新增设备组
    ERROR_ADD_DEV_GROUP_ALREADY_EXIST,          // 新增设备组名已存在
    ERROR_MODIFY_DEV_GROUP_ALREADY_EXIST,      // 修改设备组名已存在
    
    //客户端与服务器版本不一致
    ERROR_SYSTEM_VERSION_ERROR
};


// Session 模块App ID定义
#define NMC_APP_AID                 AID_TNMS_BGN           // CNmcAppIns的ID
#define SERVICE_APP_AID             (AID_TNMS_BGN + 1)     // CServiceAppIns的ID    
#define DEVAGENT_APP_AID            (AID_TNMS_BGN + 2)     // CAgentAppIns的ID
#define DB_MANAGER_APP_AID          (AID_TNMS_BGN + 3)     // CDbAppIns的ID
#define NMS_WATCHDOG_APP_AID        (AID_TNMS_BGN + 4)     // 看门狗程序应用的ID
#define REGDOG_APP_AID              (AID_TNMS_BGN + 5)     // 网管注册看门狗应用ID

#define NMS_DBAGENT_AID             (AID_TNMS_BGN + 20)     // 内实应用ID


#define SYNC_TIME_LAP               (u8)15                  // 与3AS时钟同步的最大误差秒数

//////////////////////////////////////////////////////////////////////////
//Ipv4的地址长度
const u8 MAX_IPV4_LEN = 16;
//IPv6的地址长度
const u8 MAX_IPV6_LEN = 32;

// 网管服务器系统容量
const s32 MAX_FIELD_NUM             = 128;      // 最大域个数
const s32 MAX_GROUP_NUM             = 1024;     // 最大用户组数
const s32 MAX_GROUP_USER_NUM        = 1024;     // 每个组最大用户数
const s32 MAX_USER_NUM              = 1024;     // 系统最大用户数
const s32 MAX_NMC_SESSION_NUM       = 256;      // 最大同时在线客户端数量
const s32 MAX_AGENT_CMD_NUM         = 256;      // 最大一次操作设备代理的数量

//用户信息长度
const s32 MAX_USER_ID_LEN           = 32;
const s32 MAX_USER_PWD_LEN          = 32;
const s32 MAX_USER_NAME_LEN         = 10;
const s32 MAX_DEPT_NAME_LEN         = 32;
const s32 MAX_WORK_ID_LEN           = 12;
const s32 MAX_RANK_LEN              = 10;
const s32 MAX_ADDR_LEN              = 32;
const s32 MAX_TELEPHONE_LEN         = 32;
const s32 MAX_MOBILE_LEN            = 32;
const s32 MAX_EMAIL_LEN             = 32;
const s32 MAX_REMARK_LEN            = 128;
const s32 MAX_SMS_LEN               = 32;

//用户组信息
const s32 MAX_GROUP_NAME_LEN        = 32;
const s32 MAX_GROUP_DESC_LEN        = 128;


// 告警信息描述长度
const s32 MAX_ALARM_DESC_LEN        = 255;

// 日志信息条目的长度
const s32 MAX_LOG_INFO_LEN          = 128;

// 告警知识库
const s32 MAX_KNOW_ITEM_DESC_LEN        = 64;
const s32 MAX_KNOW_ITEM_CAUSE_LEN       = 64;
const s32 MAX_KNOW_ITEM_SOLUTION_LEN    = 128;
const s32 MAX_KNOW_ITEM_REMARK_LEN      = 128;


// 设备别名长度
const s32 MAX_DEVICE_ALIAS_LEN      = 32;

// 规则名称长度
const s32 MAX_RULES_NAME_LEN            = 32;   // 规则名称长度
// IP访问规则
const s32 MAX_IP_RULES_IPLIST_SIZE      = 32;   // 单条IP访问规则的IP列表大小
const s32 MAX_IP_RULES_IPSEGMENT_SIZE   = 10;   // 单条IP访问规则的受限网段多少
const s32 MAX_IP_RULES_IPZOOM_SIZE      = 5;    // 单条IP访问规则的受限区域多少

// 邮件发送规则
const s32 MAX_MAIL_RULES_DEV_TYPE_SIZE  = 20;       // 单条Email规则中设备类型的最大数目
const s32 MAX_MAIL_RULES_DEV_ID_SIZE    = 15;       // 单条Email规则中设备ID的最大数目
const s32 MAX_MAIL_RULES_DEV_NAME_SIZE  = 15;       // 单条Email规则中设备别名的最大数目
const s32 MAX_MAIL_RULES_DEV_IP_SIZE    = 20;       // 单条Email规则中设备IP的最大数目
const s32 MAX_MAIL_RULES_ALARM_CODE_SIZE = 20;  // 单条Email规则中告警码的最大数目
const s32 MAX_MAIL_RULES_ADDR_NUM   = 15;           // 单条Email规则中发送邮件的最大地址数
const s32 MAX_MAIL_RULES_NUM            = 256;      // 最大的Email规则中数目
const s32 MAX_SMS_RULES_ADDR_NUM    = 15;           // 单条Email规则中发送短信的最大地址数

// 告警过滤规则
const s32 MAX_FILTER_RULES_CODE_NUM         = 20;   // 单条过滤规则中最大告警码数目
const s32 MAX_FILTER_RULES_SERVICE_TYPE_NUM = 20;   // 单条过滤规则中最多业务类型数目
const s32 MAX_FILTER_RULES_ALIAS_NUM        = 8;    // 单条过滤规则中最多设备别名
const s32 MAX_FILTER_RULES_DEVID_NUM        = 8;    // 单条过滤规则中最多设备的ID数

// 权限
const s32 MAX_FIELD_RIGHT_NUM   = 8;                // 用户组最大权限域数目
const s32 MAX_DEVICE_RIGHT_NUM  = 20;               // 用户组最大权限域数目


//数据库名长度
const s32 MAX_DB_NAME_LEN = 16; 
// 数据库最大连接数
const s32 MAX_DB_LINK_NUM = MAX_NMC_SESSION_NUM;    

//数据库名长度
const s32 MAX_BK_FILE_NAME_LEN = 32;            // 数据库备份的文件名前缀最大长度   

//指示灯状态长度        
const s32 LED_STATUS_LEN  = 24; 

const u8 MAX_DBCONNECT_TIME     = 30;

//服务器的磁盘文件相关信息长度
const u16 MAX_FILEPATH_LEN = 256;
//windows message
#define WM_INITAPP_OVER    WM_USER+1//
#define WM_INITAPP_FIN     WM_USER+2//
#define WM_NMSAPP_QUIT     WM_USER+3

// 性别
#define GENDER_MALE                 (u8)1       // 男性
#define GENDER_FEMALE               (u8)2       // 女性

// 断链检测
//#define AGENT_HB_TIME             (u32)20     // 断链检测的时间
//#define AGENT_HB_NUM              (u32)3      // 断链检测的次数
#define ROOT_VIRDEV_DEVICE_ID       "11111111111111111111111111111111"      // NMS作为网管系统的根节点的ID
#define AAA_DEVICE_ID               "11111111111111111111111111111112"      // 3AS指定特殊的设备ID

//AES加密密钥
#define NMS_AES_PUB_KEY         "BBECD8EF49244540"      //公共密钥
#define NMS_AES_INIT_KEY        "1111111111111111"                      //初始密钥
#define SMS_TIME_OUT                    (u8)60          // 短信超时时间60s
#endif // NMS_MACRO_H

