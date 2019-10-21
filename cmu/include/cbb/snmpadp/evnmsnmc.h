/*=============================================================================
模   块   名: 网管服务器
文   件   名: evnmsnmc.h
相 关  文 件: 无
文件实现功能: NMS-NMC消息定义
作        者: 
版        本: V1.0  
-------------------------------------------------------------------------------
修改记录:
日      期  版本    修改人      修改内容
2005/12/20  1.0     任厚平       创建     
2006/02/22  3.9     李洪强       重新整理消及结构定义，为主消息加子消息定义
=============================================================================*/

#ifndef _EVNMSNMC_20050907_H_
#define _EVNMSNMC_20050907_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "osp.h"
#include "eventid.h"
#include "nmsmacro.h"
#include "snmpadp.h"
#include "calladdr.h"

//自定义断言
#define NMS_ASSERT(x)

#define NMS_SYSTEM_VERSION          (u32)0x3932         //3.9_R3.2

#define MATCH_CONDITION_OR              1               // 所有条件满足一个既成立
#define MATCH_CONDITION_AND             2               // 所有条件全部满足才成立

//数据库用字符集
#define DB_LANGID_SC_CHN        0       //简体中文
#define DB_LANGID_TR_CHN        1       //繁体中文
#define DB_LANGID_US_ENG        2       //英语

#define DB_CHARSETID_ISO_1      0       //英语字符集
#define DB_CHARSETID_CP850      1       //英语字符集
#define DB_CHARSETID_EUCGB      2       //中文字符集
#define DB_CHARSETID_GB18030    3       //中文字符集
#define DB_CHARSETID_BIG5       4       //繁体中文字符集
#define DB_CHARSETID_UTF8       5       //通用字符集
//是否生效
#define INVALID_VALUE                   (u8)0           // 无效的u8型值
#define VALID_VALUE                     (u8)1           // 有效值
#define INVALID_PARAM                   (u32)0xFFFFFFFF // 无效的u32参数
#define VALID_PARAM                     (u32)0x00000001 // 有效的u32参数

// 告警状态及级别,既做为类型定义，又做为mask
#define ALARM_NORMAL                    (u8)0x00        // 设备正常
#define ALARM_IMPORTANT                 (u8)0x01        // 重要告警
#define ALARM_GENERAL                   (u8)0x02        // 普通告警
#define ALARM_CRITICAL                  (u8)0x04        // 严重告警
#define ALARM_UNKNOW                    (u8)0x05        // 未知告警

// 操作类型级别,既做为类型定义，又做为mask
#define ACTION_INVALID                  (u8)0x00        // 无效值，不匹配级别
#define ACTION_IMPORTANT                (u8)0x01        // 重要操作
#define ACTION_SECONDARY                (u8)0x02        // 次要操作
#define ACTION_GENERAL                  (u8)0x04        // 普通操作


// 告警过滤条件,既做为类型定义，又做为mask
#define ALARM_FILTER_LEVEL              0x00000001      // 告警级别过滤
#define ALARM_FILTER_CODE               0x00000002      // 告警码过滤
#define ALARM_FILTER_DEV_ID             0x00000004      // 告警设备ID过滤
#define ALARM_FILTER_DEV_TYPE           0x00000008      // 告警设备类型过滤
#define ALARM_FILTER_DEV_ALIAS          0x00000010      // 告警设备别名过滤


// 告警查询条件,既做为类型定义，又做为mask
#define ALARM_QUERY_LEVEL               0x00000001      // 告警级别
#define ALARM_QUERY_CODE                0x00000002      // 告警码
#define ALARM_QUERY_DEV_ID              0x00000004      // 告警设备ID
#define ALARM_QUERY_DEV_TYPE            0x00000008      // 告警设备类型
#define ALARM_QUERY_DEV_ALIAS           0x00000010      // 告警设备别名
#define ALARM_QUERY_DESC                0x00000020      // 告警描述
#define ALARM_QUERY_START_TIME          0x00000040      // 告警产生时间
#define ALARM_QUERY_RECOVER_TIME        0x00000080      // 告警恢复时间
#define ALARM_QUERY_ALARM_STATE         0x00000100      // 按告警状态查询
#define ALARM_QUERY_IP_ADDR             0x00000200      // 按IP地址查询
#define ALARM_QUERY_DEV_LARGE_TYPE      0x00000400    //告警大类型

// 操作查询条件,既做为类型定义，又做为mask
#define ACTION_QUERY_USER_NAME          0x00000001      // 按用户名查
#define ACTION_QUERY_GROUP_NAME         0x00000002      // 按组名查
#define ACTION_QUERY_TIME               0x00000004      // 按时间段查
#define ACTION_QUERY_DEV_ID             0x00000008      // 按访问的设备ID查
#define ACTION_QUERY_ACT_TYPE           0x00000010      // 按操作类型查
#define ACTION_QUERY_ACT_INFO           0x00000020      // 按操作详细描述查
#define ACTION_QUERY_ACT_LEVEL          0x00000040      // 按操作级别查

// 权限范围
#define RIGHT_RANGE_ALL_CHILD           0x01            // 对本设备及所有下属设备拥有权限
#define RIGHT_RANGE_NO_CHILD            0x02            // 对本设备及所有下属设备都没有权限
#define RIGHT_RANGE_PART_CHILD          0x04            // 对平台设备的部分下属设备拥有权限

// 设备操作权限,既做为类型定义，又做为mask
#define DEV_RIGHT_VIEW_CFG              0x00000001      // 查看配置信息
#define DEV_RIGHT_MODIFY_CFG            0x00000002      // 修改配置信息
#define DEV_RIGHT_VIEW_PFM              0x00000004      // 查看性能信息
#define DEV_RIGHT_REBOOT                0x00000008      // 重启设备
#define DEV_RIGHT_PING                  0x00000010      // PING设备
#define DEV_RIGHT_VIEW_LOG              0x00000020      // 查看历史告警信息
#define DEV_RIGHT_RELEASE_ALARM         0x00000040      // 解除设备告警
#define DEV_RIGHT_SELFTEST              0x00000080      // 设备自检
#define DEV_RIGHT_SOFTWARE_UPDATE       0x00000100      // 上传版本
#define DEV_RIGHT_OWNER                 0x00000200      // 设备的拥有权限
#define DEV_RIGHT_VIEW_DEVFORM          0x00000400      // 查看机件构架图

//域权限信息
#define FIELD_RIGHT_ADD_FIELD_GROUP     0x00000001      // 新建域用户组
#define FIELD_RIGHT_MODDEL_FIELD_GROUP  0x00000002      // 修改/删除域用户组
#define FIELD_RIGHT_VIEW_FIELD_GROUP        0x00000004      // 查看域用户组
#define FIELD_RIGHT_VIEW_FIELD_USER     0x00000008      // 查看域用户
#define FIELD_RIGHT_MODDEL_FIELD_USER       0x00000010      // 修改/删除域用户
#define FIELD_RIGHT_CHILD_FIELD_RIGHT       0x00000020      // 是否具有子域权限
#define FIELD_RIGHT_OWNER               0x00000040      // 域的拥有权限


// 系统管理权限,既做为类型定义，又做为mask
#define SYS_RIGHT_USER_MANGE            0x00000020
#define SYS_RIGHT_VIEW_OTHER_GROUP_LOG  0x00000040      // 查看其他用户组操作日志
#define SYS_RIGHT_DEL_SELF_LOG          0x00000080      // 删除自己的操作日志
#define SYS_RIGHT_DEL_OTHER_GROUP_LOG   0x00000100      // 删除其他用户组的操作日志
#define SYS_RIGHT_DEL_HISTORY_ALARM     0x00000200      // 删除历史告警
#define SYS_RIGHT_MODIFY_NMS_AUTO_ACT   0x00000400      // 修改服务器自动备份及清理设置
#define SYS_RIGHT_SET_IP_RULES          0x00000800      // 设置IP受限规则
#define SYS_RIGHT_SET_EMAIL_RULES       0x00001000      // 设置EMAIL发送规则
#define SYS_RIGHT_SET_ALARM_RULES       0x00002000      // 设置告警过滤规则
#define SYS_RIGHT_LOCK_NMCUI            0x00004000      // 使用NMC界面锁定功能
#define SYS_RIGHT_MODIFY_NMS_CFG        0x00008000      // 修改服务器配置

// NMC请求信息类型,既做为类型定义，又做为mask
#define NMC_REQ_BASE_PFM_INFO           0x00000001      // 请求设备基本性能参数
#define NMC_REQ_EXTEND_PFM_INFO         0x00000002      // 请求设备扩展性能参数
#define NMC_REQ_FOR_MOCK_DRAW           0x00000004      // 请求设备性能参数用于画机架构件图

// 设备组ID操作
#define NMC_DEVGROUP_SERIAL_NO  (u32)0x80000000         // 从服务器获取过来的设备组ID要将最高位置1
#define NMS_DEVGROUP_SERIAL_NO  (u32)0x7FFFFFFF         // 送往服务器的设备组ID要将最高位置清0
#define NMS_DEV_TYPE_INVALID            (u8)0x00        //设备为无效类型或是不能识别
#define NMS_DEV_TYPE_PU                 (u8)0x01        //设备大类型为PU
#define NMS_DEV_TYPE_CMU                (u8)0x02        //设备大类型为CMU

// 操作日志类型索引
enum ACT_TYPE_IDX
{
    ACT_TYPE_LOGON_IDX = 0,             //  登录           
    ACT_TYPE_LOGOFF_IDX,                //  注销           
    ACT_TYPE_ADD_GROUP,                 //  新增用户组     
    ACT_TYPE_MODIFY_GROUP,              //  修改用户组     
    ACT_TYPE_DEL_GROUP,                 //  删除用户组     
    ACT_TYPE_ADD_USER,                  //  新增用户       
    ACT_TYPE_MODIFY_USER,               //  修改用户       
    ACT_TYPE_DEL_USER,                  //  删除用户       
    ACT_TYPE_MODIFY_ACCOUNT,            //  修改自身帐户信息       
    ACT_TYPE_SAVE_PREFER,               //  保存个性化信息 
    ACT_TYPE_EXPORT_PREFER,             //  导出个性化信息 
    ACT_TYPE_IMPORT_PREFER,             //  导入个性化信息 
    ACT_TYPE_DEFAULT_VALUE,             //  恢复默认值     
    ACT_TYPE_GET_DEV_CFG,               //  获取设备参数   
    ACT_TYPE_SET_DEV_CFG,               //  修改设备参数   
    ACT_TYPE_REBOOT,                    //  重启设备
    ACT_TYPE_PING,                      //  PING设备
    ACT_TYPE_SELFTEST,                  //  设备自检
    ACT_TYPE_SOFTWARE_UPDATE,           //  版本上传
    ACT_TYPE_ALARM_SETTING,             //  告警设置               
    ACT_TYPE_ALARM_ANALYSE,             //  分析故障               
    ACT_TYPE_ALARM_QUERY,               //  查询告警               
    ACT_TYPE_DEL_ALARM,                 //  清除告警               
    ACT_TYPE_EXPORT_ALARM,              //  导出告警               
    ACT_TYPE_RELEASE_ALARM,             //  解除告警               
    ACT_TYPE_GET_DEV_PFM,               //  查看性参参数           
    ACT_TYPE_PFM_ANALYSE,               //  性能分析               
    ACT_TYPE_SET_IP_RULES,              //  设定受限IP地址规则     
    ACT_TYPE_SET_MAIL_RULES,            //  设定邮件发送规则    
    ACT_TYPE_SET_ALARM_FILTER_RULES,    //  设定告警过滤规则    
    ACT_TYPE_SET_LOGIN_LOCK_TIME,       //  设定登录锁定时间       
    ACT_TYPE_MANUAL_BACKUP_DB,          //  手动备份网管数据库     
    ACT_TYPE_RECOVER_DB,                //  恢复网管数据库         
    ACT_TYPE_QUERY_ACT_LOG,             //  查询操作日志           
    ACT_TYPE_EXPORT_ACT_LOG,            //  导出操作日志           
    ACT_TYPE_DEL_ACT_LOG,               //  删除操作日志           
    ACT_TYPE_LOCK_NMCUI,                //  手动锁定界面
    ACT_TYPE_UNLOCK_NMCUI,              //  解锁界面               
    ACT_TYPE_SET_NMS_CFG,               //  修改服务器配置参数     
    ACT_TYPE_VIEW_DEV_PIC,              //  查看设备机架构件图
    ACT_TYPE_DEV_GROUP_ADD,             //  增加设备组操作
    ACT_TYPE_DEV_GROUP_MODIFY,          //  修改设备组操作
    ACT_TYPE_DEV_GROUP_DEL,             //  删除设备组操作
    ACT_TYPE_NUM                        //  操作类型总数
};


// IP访问限制条件
#define IP_RULES_IP_LIST_MASK           0x00000001      // 根据IP列表限制
#define IP_RULES_IP_SEGMENT_MASK        0x00000002      // 根据网段限制
#define IP_RULES_IP_ZONE_MASK           0x00000004      // 根据地址区域限制

// 邮件发送触发条件
#define MAIL_RULES_DEV_TYPE_MASK        0x00000001      // 根据设备类型列表判断
#define MAIL_RULES_DEV_ID_MASK          0x00000002      // 根据设备ID列表判断
#define MAIL_RULES_DEV_NAME_MASK        0x00000004      // 根据设备别名判断
#define MAIL_RULES_DEV_IP_MASK          0x00000008      // 根据设备IP判断
#define MAIL_RULES_ALARM_CODE_MASK      0x00000010      // 根据告警码判断
#define MAIL_RULES_ALARM_LEVEL_MASK     0x00000020      // 根据告警级别判断

// 自动操作的策略
#define AUTO_ACT_WEEKDAY                (u8)1           // 指定工作日中的某些天
#define AUTO_ACT_PERIODICAL             (u8)2           // 动作周期，最小单位为天

#define WEEK_MONDAY                     0x00000001      // 星期一
#define WEEK_TUESDAY                    0x00000002      // 星期二
#define WEEK_WEDNESDAY                  0x00000004      // 星期三
#define WEEK_THURSDAY                   0x00000008      // 星期四
#define WEEK_FRIDAY                     0x00000010      // 星期五
#define WEEK_SATURDAY                   0x00000020      // 星期六
#define WEEK_SUNDAY                     0x00000040      // 星期天

//域更新消息的更新类型
#define ACTION_ADD      1
#define ACTION_MODIFY   2
#define ACTION_DELETE   3


//////////////////////////////////////////////////////////////////////////
// 服务器和客户端交互消息
// NMS与NMC的交互消息，很多涉及到与数据库操作的，nmcapp收到后直接转发到dbapp处理
// 所以就不再单独定义消息了
//
// 客户端登录
// 客户端登录操作
//    nmc -> nmcapp -> dbapp (req): TNmsLogonReq + u32(客户端版本号)
//    nmc <- nmcapp <- dbapp (ack): TNmsLogonRet + u32(服务器版本号) +u8(数组长度)+s8数组(用户组名字)错误码: ERROR_SYSTEM_VERSION_ERROR
//              |
// serviceapp <-|
// nmc <- nmcapp <- dbapp (nack): TNmsLogonReq + u32(服务器版本号) 错误码: ERROR_PWD/ERROR_USER_NOT_EXIST/ERROR_IP_RESTRICT/ERROR_SYSTEM_VERSION_ERROR
// nmc <- nmcapp (finish):
OSPEVENT( EV_NMC_LOGON,                         EV_NMC_NMS_BGN + 0 );
// 客户端注销操作
// nmc -> nmcapp -> serviceapp (req): 
// nmc <- nmcapp (ack):
OSPEVENT( EV_NMC_LOGOUT,                        EV_NMC_NMS_BGN + 5 );
// 用户权限的信息, 登录时使用
// nmcapp -> dbapp(req): u32(用户流水号)
//     nmc <- nmcapp <- dbapp (notify):u32(用户流水号)+ TTnmMask(系统权限) + u32(域权限数）+ TDeviceRight数组
//              |            + u32(设备权限数) + TDeviceRight数组
// serviceapp <-|  
//     nmc <- nmcapp <- dbapp (finish):u32(用户流水号)
//              |
// serviceapp <-|  

OSPEVENT( EV_NMC_USER_RIGHT,                    EV_NMC_NMS_BGN + 10 );

//////////////////////////////////////////////////////////////////////////
// 用户管理(nmcpp/dbapp共用)
// 请求用户信息列表
// nmc -> nmcapp -> dbapp (req): u32(用户流水号)[ + u8(MSG_TYPE_GROUP/MSG_TYPE_USER)]
// nmc <- nmcapp <- dbapp (notify):u8(MSG_TYPE_GROUP) + u32(用户组列表大小) + TUserGroupInfo用户组列表
// nmc <- nmcapp <- dbapp (notify):u8(MSG_TYPE_USER) +  u32(用户列表大小) + TUserInfo用户列表
// nmc <- nmcapp <- dbapp (finish):u8(MSG_TYPE_GROUP/MSG_TYPE_USER)
OSPEVENT( EV_GET_USER_LIST,                     EV_NMC_NMS_BGN + 40 );

// 新增用户组消息
// nmc -> nmcapp -> dbapp (req): TUserGroupInfo
// nmc <- nmcapp <- dbapp (ack): TUserGroupInfo
// nmc <- nmcapp <- dbapp (nack): TUserGroupInfo    错误码: ERROR_GROUP_EXIST
OSPEVENT( EV_ADD_USER_GROUP,                    EV_NMC_NMS_BGN + 50 );

// 修改用户组
//       nmc -> nmcapp -> dbapp (req): TUserGroupInfo
//       nmc <- nmcapp <- dbapp (ack):TUserGroupInfo
//   serviceapp <-|(notify):TUserGroupInfo
OSPEVENT( EV_MODIFY_USER_GROUP,                 EV_NMC_NMS_BGN + 55 );

// 删除用户组
// 用户组无用户在线
// nmc -> nmcapp -> dbapp (EV_DEL_USER_GROUP::req): u32(用户组ID）
// nmc <- nmcapp <- dbapp (EV_DEL_USER_GROUP::ack): u32(用户组ID）
// nmc <- nmcapp <- dbapp (EV_DEL_USER_GROUP::nack):u32(用户组ID）  错误码: ERROR_GROUP_NOT_EXIST
// 用户组有用户在线
// nmc -> nmcapp (EV_DEL_USER_GROUP::req): u32(用户组ID）
// nmc <- nmcapp(EV_DEL_USER_GROUP_CONFIRM::req): u32(用户组ID）    错误码: ERROR_USER_ONLINE
// nmc -> nmcapp(EV_DEL_USER_GROUP_CONFIRM::ack): u32(用户组ID）
//        nmcapp -> dbapp (EV_DEL_USER_GROUP::req): u32(用户组ID)
// nmc <- nmcapp <- dbapp (EV_DEL_USER_GROUP::ack): u32(用户组ID）
// nmc <- nmcapp <- dbapp (EV_DEL_USER_GROUP::nack):u32(用户组ID）  
OSPEVENT( EV_DEL_USER_GROUP,                    EV_NMC_NMS_BGN + 60 );
OSPEVENT( EV_DEL_USER_GROUP_CONFIRM,            EV_NMC_NMS_BGN + 61 );

// 新增用户
// nmc -> nmcapp -> dbapp (req): TUserInfo
// nmc <- nmcapp <- dbapp (ack): TUserInfo
// nmc <- nmcapp <- dbapp (nack):TUserInfo      错误码: ERROR_USER_EXIST
OSPEVENT( EV_ADD_USER,                          EV_NMC_NMS_BGN + 65 );

// 修改用户
// nmc -> nmcapp -> dbapp (req): TUserInfo
// nmc <- nmcapp <- dbapp (ack):TUserInfo
// nmc <- nmcapp <- dbapp (nack):TUserInfo      错误码: ERROR_USER_NOT_EXIST
OSPEVENT( EV_MODIFY_USER,                       EV_NMC_NMS_BGN + 70 );

// 删除用户
// 用户不在线
// nmc -> nmcapp -> dbapp (EV_DEL_USER::req): u32(用户流水号列表大小) + u32列表(用户流水号列表)
// nmc <- nmcapp <- dbapp (EV_DEL_USER::ack): u32(用户流水号列表大小) + u32列表(用户流水号列表)
// nmc <- nmcapp <- dbapp (EV_DEL_USER::nack):u32(用户流水号列表大小) + u32列表(用户流水号列表)     错误码: ERROR_USER_NOT_EXIST
// 用户在线
//          nmc -> nmcapp (EV_DEL_USER::req):u32(用户流水号列表大小) + u32列表(用户流水号列表)      
//          nmc <- nmcapp (EV_DEL_USER_CONFIRM::req):u32(用户流水号列表大小) + u32列表(用户流水号列表)错误码: ERROR_USER_ONLINE
//          nmc -> nmcapp (EV_DEL_USER_CONFIRM::ack):u32(用户流水号列表大小) + u32列表(用户流水号列表)错误码: ERROR_USER_ONLINE
//                 nmcapp -> dbqpp (EV_DEL_USER::req):u32(用户流水号列表大小) + u32列表(用户流水号列表)
//          nmc <- nmcapp <- dbapp (EV_DEL_USER::ack): u32(用户流水号列表大小) + u32列表(用户流水号列表)
//                   |
//nmc <- sericeapp <-|  (EV_DEL_USER::notify) : u32(用户流水号列表大小) + u32列表(用户流水号列表)
// nmc <- nmcapp <- dbapp (EV_DEL_USER::nack):u32(用户流水号列表大小) + u32列表(用户流水号列表)     错误码: ERROR_USER_NOT_EXIST

OSPEVENT( EV_DEL_USER,                          EV_NMC_NMS_BGN + 75 );
OSPEVENT( EV_DEL_USER_CONFIRM,                  EV_NMC_NMS_BGN + 76 );
// 修改用户帐户信息
// nmc -> nmcapp -> dbapp (req): TUserInfo
// nmc <- nmcapp <- dbapp (ack): TUserInfo
// nmc <- nmcapp <- dbapp (nack): TUserInfo
OSPEVENT( EV_MODIFY_USER_INFO,                  EV_NMC_NMS_BGN + 80 );
// nmc -> nmcapp -> dbapp (req): TChangePwd
// nmc <- nmcapp <- dbapp (ack): TChangePwd 
// nmc <- nmcapp <- dbapp (nack): TChangePwd        错误码:ERROR_PWD
OSPEVENT( EV_MODIFY_USER_PWD,                   EV_NMC_NMS_BGN + 85 );

//用户域  renhouping 2006.08.21
//nmcapp -> serviceapp(req):
//nms -> nmc(notify): u16( 域个数) + TFieldInfo数组
OSPEVENT(EV_GET_FIELD_INFO, EV_NMC_NMS_BGN + 100);

//nms->nmc(notify): u8(操作类型) + u16(个数) + TFieldInfo数组
OSPEVENT(EV_UPDATE_FIELD_INFO, EV_NMC_NMS_BGN + 104); 


//////////////////////////////////////////////////////////////////////////
// 个性化信息操作
//
// NMC获取个性化信息
OSPEVENT(EV_GET_PREFER_INFO,                    EV_NMC_NMS_BGN + 130);

// 保存个性化信息
OSPEVENT(EV_SAVE_PREFER_INFO,                   EV_NMC_NMS_BGN + 135);

// 恢复默认值
OSPEVENT(EV_RESTORE_SYS_PREFER_INFO,            EV_NMC_NMS_BGN + 140);

//////////////////////////////////////////////////////////////////////////
// 设备拓朴图消息
//
// 
// 得到设备拓朴图
// nmc -> nmcapp -> serviceapp (req): 
// nmc <- nmcapp <- serviceapp (notify): u32(在拓扑的设备总数)+u16(数组个数) + TDevNmcTopo数组
// ...
// nmc <- nmcapp <- serviceapp (finish): 
OSPEVENT(EV_GET_DEVICE_TOPO,                    EV_NMC_NMS_BGN + 190);
// 添加设备
// nmc <- nmcapp <- serviceapp (notify): u16(数组个数) + TDevNmcTopo数组
OSPEVENT(EV_DEVICE_TOPO_ADD,                    EV_NMC_NMS_BGN + 191);
// 删除设备
// nmc <- nmcapp <- serviceapp (notify): u16(数组个数) + u32数组（设备ID）
OSPEVENT(EV_DEVICE_TOPO_DEL,                    EV_NMC_NMS_BGN + 192);
// 设备信息更新
// nmc <- nmcapp <- serviceapp (notify): u16(数组个数) + TDevNmcTopo数组
OSPEVENT(EV_DEVICE_TOPO_UPDATE,                 EV_NMC_NMS_BGN + 193);

// 向3AS请求更新拓朴图
// nmc -> nmcapp -> serviceapp:u8 (VALID_VALUE 强制请求拓朴，不做检查; INVALID_VALUE 检查拓朴完整性，如果完整则不请求
//           ... -> serviceapp: 
OSPEVENT(EV_DEVICE_TOPO_REFRESH,                EV_NMC_NMS_BGN + 194);


// 得到设备在线状态
// nmc <- nmcapp <- serviceapp (notify):
//     上线:u8(TNM_STATE_ONLINE) + TDeviceInfo(设备信息)
//     下线 u8(TNM_STATE_OFFLINE) + u32(设备流水号)
OSPEVENT(EV_GET_DEV_ONLINE_INFO,                EV_NMC_NMS_BGN + 195);

//////////////////////////////////////////////////////////////////////////
// 配置管理
// 
// 得到设备配置参数
// NMC -> NMS(req): u32(设备流水号)
// NMC <- NMs(ack): u32(设备流水号) + 配置参数
OSPEVENT(EV_GET_DEVICE_CFG_INFO,                EV_NMC_NMS_BGN + 240);

// 设置设备的配置参数
// NMC -> NMS(req): u32(设备流水号) + 配置参数
// NMC <- NMS(ack): u32(设备流水号)
// NMC <- NMS(nack): u32(设备流水号)         错误码:ERROR_CFG_DEV
OSPEVENT(EV_SET_DEVICE_CFG_INFO,                EV_NMC_NMS_BGN + 245);

// 得到服务器配置参数
// NMC -> NMS(req): 
// NMC <- NMS(ack): TNmsCfgInfo
OSPEVENT(EV_GET_NMS_CFG_INFO,                   EV_NMC_NMS_BGN + 250);
// 设置服务器配置参数
// NMC -> NMS(req): TNmsCfgInfo
// NMC <- NMS(ack): TNmsCfgInfo
OSPEVENT(EV_SET_NMS_CFG_INFO,                   EV_NMC_NMS_BGN + 255);

// 更新设备软件版本
// nmc->nmcapp与nmcapp->serviceapp共用
// NMC -> NMS(req): u8(BlockId列表大小） + TTnmBlockInfo列表(BlockId信息表)
//                  + u32(设备ID列表大小） + u32列表(设备ID列表)
// NMC <- NMS(ack): 
OSPEVENT(EV_UPDATE_SOFTWARE,                    EV_NMC_NMS_BGN + 260);

// 软件版本切分包
// NMC -> NMS(notify): u8(byBlockID) + u32(分片序号) + u32(有效数据长度) + 有效数据
// NMC <- NMS(ack): u8(byBlockID) + u32(分片序号)
// NMC <- NMS(finish): u8(byBlockID)
OSPEVENT(EV_SOFTWARE_PACKAGE,                   EV_NMC_NMS_BGN + 270);

// NMC <- NMS(notify): u32(进度信息数组大小）+ TProgressInfo数组
OSPEVENT(EV_SOFTWARE_PROGRESS,                  EV_NMC_NMS_BGN + 275);
//设备上传文件完成
//nms->nmc(notify): u32(设备流水号) + u8(是否成功: 0 失败，1 成功 ) + u8(是否重启: 1 重启，0 不重启) 
OSPEVENT(EV_SOFTWARE_COMPLETE,                  EV_NMC_NMS_BGN + 279);
//上传文件出错消息通知
//nms->nmc(notify):u32(设备流水号)
OSPEVENT(EV_SOFTWARE_UPDATE_ERROR,              EV_NMC_NMS_BGN + 283);
//软件升级失败错误消息(整个上传任务失败)
OSPEVENT( EV_UPDATE_SOFTWARE_FAIL,              EV_NMC_NMS_BGN + 284);

//////////////////////////////////////////////////////////////////////////
// 设备操作
//
// 
// 设备重启
// NMC -> NMS(req): u32(重启设备的数量) + u32数组(设备流水号)
//                   + u8 (是否是将重启操作写入日志VALID_VALUE/INVALID_VALUE,程序触发的不写操作日志)
// NMC <- NMS(ack): u32(设备流水号)
// NMC <- NMS(nack): u32(设备流水号)
OSPEVENT(EV_DEVICE_REBOOT,                      EV_NMC_NMS_BGN + 330); 

// 设备自检
// NMC -> NMS(req): u32(自检设备的数量) + u32数组(设备流水号)
// NMC <- NMS(ack): u32(设备流水号)
// NMC <- NMS(nack): u32(设备流水号)
OSPEVENT(EV_DEVICE_SELF_TEST,                   EV_NMC_NMS_BGN + 335);

// 设备线路检测
// NMC -> NMS(req): TPingReq
// NMC <- NMS(notify): TPingInfo
// NMC <- NMS(finish): u32(设备流水号)
OSPEVENT(EV_PING_DEVICE,                        EV_NMC_NMS_BGN + 340);
// 线路检测结果通知
// NMS <- Thread: TPingInfo
OSPEVENT(EV_PING_ECHO,                          EV_NMC_NMS_BGN + 341);
// 线路检测完成通知
// NMS <- Thread: u32(设备流水号) + TPingStat
// NMC <- NMS: u32(设备流水号) + TPingStat
OSPEVENT(EV_PING_FINISH,                        EV_NMC_NMS_BGN + 342);
// 中止设备线路检测
// NMC -> NMS(req): u32(设备流水号)
// NMC <- NMS(ack): u32(设备流水号)
OSPEVENT(EV_STOP_PING_DEVICE,                   EV_NMC_NMS_BGN + 343);

// 设备性能分析
OSPEVENT(EV_DEVICE_PFM_ANALYSE,                 EV_NMC_NMS_BGN + 345);

//////////////////////////////////////////////////////////////////////////
// 故障管理接口
//
// 得到邮件发送规则
// nmc -> nmcapp -> dbapp (req): 
// nmc <- nmcapp <- dbapp (notify): u32(个数)+ TEmailRules数组
// nmc <- nmcapp <- dbapp (finish):
//    serviceapp -> dbapp (req): 
//    serviceapp <- dbapp (notify): u32(个数)+ TEmailRules数组
//    serviceapp <- dbapp (finish):
OSPEVENT(EV_GET_EMAIL_RULES,                    EV_NMC_NMS_BGN + 380);
// 增加邮件发送规则
// nmc -> nmcapp -> dbapp (req): TEmailRules
// nmc <- nmcapp <- dbapp (ack): TEmailRules
//                    |
//       serviceapp <-| (notify):TEmailRules
// nmc <- nmcapp <- dbapp (nack): TEmailRules           错误码:ERROR_ADD_MAIL_RULES_ALREADY_EXIST
OSPEVENT(EV_ADD_EMAIL_RULES,                    EV_NMC_NMS_BGN + 381);
// 修改邮件发送规则
// nmc -> nmcapp -> dbapp (req): TEmailRules
// nmc <- nmcapp <- dbapp (ack): TEmailRules
//                    |
//       serviceapp <-| (notify):TEmailRules
// nmc <- nmcapp <- dbapp (nack): TEmailRules           错误码:ERROR_ADD_MAIL_RULES_ALREADY_EXIST
OSPEVENT(EV_MODIFY_EMAIL_RULES,                 EV_NMC_NMS_BGN + 382);
// 删除邮件发送规则
// nmc -> nmcapp -> dbapp (req): u32(要删除的EMAIL规则数) + u32数组(邮件规则ID数组)
// nmc <- nmcapp <- dbapp (ack): u32(要删除的EMAIL规则数) + u32数组(邮件规则ID数组)
//                    |
//       serviceapp <-| (notify):u32(要删除的EMAIL规则数) + u32数组(邮件规则ID数组)
// nmc <- nmcapp <- dbapp (nack): u32(要删除的EMAIL规则数) + u32数组(邮件规则ID数组)            错误码:ERROR_ADD_MAIL_RULES_ALREADY_EXIST
OSPEVENT(EV_DEL_EMAIL_RULES,                    EV_NMC_NMS_BGN + 383);

// 得到告警过滤规则
// nmc -> nmcapp -> dbapp (req): 
// nmc <- nmcapp <- dbapp (notify): u32(个数)+TAlarmFilterRules数组
// nmc <- nmcapp <- dbapp (finish):
//    serviceapp -> dbapp (req): 
//    serviceapp <- dbapp (notify): u32(个数)+TAlarmFilterRules数组
//    serviceapp <- dbapp (finish):
OSPEVENT(EV_GET_ALARM_FILTER_RULES,             EV_NMC_NMS_BGN + 390);
// 增加告警过滤规则
// nmc -> nmcapp -> dbapp (req): TAlarmFilterRules
// nmc <- nmcapp <- dbapp (ack): TAlarmFilterRules
//                    |
//       serviceapp <-| (notify): TAlarmFilterRules
// nmc <- nmcapp <- dbapp (nack): TAlarmFilterRules     
OSPEVENT(EV_ADD_ALARM_FILTER_RULES,             EV_NMC_NMS_BGN + 391);
// nmc -> nmcapp -> dbapp (req): TAlarmFilterRules
// nmc <- nmcapp <- dbapp (ack): TAlarmFilterRules
//                    |
//       serviceapp <-| (notify): TAlarmFilterRules
// nmc <- nmcapp <- dbapp (nack): TAlarmFilterRules     
OSPEVENT(EV_MODIFY_ALARM_FILTER_RULES,          EV_NMC_NMS_BGN + 392);
// 删除告警过滤规则
// nmc -> nmcapp -> dbapp (req): u32(要删除的规则数量)+ u32数组(告警过滤规则ID数组)
// nmc <- nmcapp <- dbapp (ack): u32(要删除的规则数量)+ u32数组(告警过滤规则ID数组)
//                    |
//      serviceapp <- | (notify): u32(要删除的规则数量)+ u32数组(告警过滤规则ID数组)
// nmc <- nmcapp <- dbapp (nack): u32(要删除的规则数量)+ u32数组(告警过滤规则ID数组)
OSPEVENT(EV_DEL_ALARM_FILTER_RULES,             EV_NMC_NMS_BGN + 393);

// 告警信息上报
// NMC <- NMS(notify): TDevAlarm
OSPEVENT(EV_DEVICE_ALARM_INFO,                  EV_NMC_NMS_BGN + 395);

// 告警知识库
// nmcapp(daemon) -> dbapp(req):u32(请求的条目数) + u32(起始记录号)
// nmcapp(daemon) <- dbapp(notify): u32(总条目数) + u32(请求的条目数) + u32(本条消息的起始记录数)
//                       + u16(本消息的条目数) + TAlarmKnowItem数组
// nmcapp(daemon) <- dbapp(finish):无 
// nmc <- nmcapp(daemon)(notify): u32(总条目数) + u32(本条消息的起始记录数)
//                       + u16(本消息的条目数) + TAlarmKnowItem数组
// nmc <- nmcapp(daemon)(finish):无
OSPEVENT(EV_ALARM_KNOWLEDGE_INFO,               EV_NMC_NMS_BGN + 400);

// 告警知识库维护3.9暂不做
//// 增加告警知识库条目
//OSPEVENT(EV_ADD_ALARM_KNOWLEDGE_INFO,         EV_NMC_NMS_BGN + 405);
//
//// 修改告警知识库条目
//OSPEVENT(EV_EDIT_ALARM_KNOWLEDGE_INFO,            EV_NMC_NMS_BGN + 410);
//
//// 删除告警知识库条目
//OSPEVENT(EV_DEL_ALARM_KNOWLEDGE_INFO,         EV_NMC_NMS_BGN + 415);

// 查询告警信息
// NMC -> NMS(req): TAlarmQueryCondition + u32(请求的条目数) + u32(起始记录号) + u8(是否将查询写入日志VALID_VALUE/INVAID_VALUE)
// NMC <- NMS(notify): u32(匹配条目总数）+ u32(请求的条目数) + u32(本条消息的起始记录号)
//                       + u32(本消息的条目数) + TDevAlarm数组
// db->nmcapp->nmc(nack):
// NMC <- NMS(finish):
OSPEVENT(EV_QUERY_ALARM_INFO,                   EV_NMC_NMS_BGN + 420);

// 手动删除告警记录
// NMC -> NMS(req): u32(要删除的记录总数）+ u32数组（告警的ID）
// NMC <- NMS(ack): 
// NMC <- NMS(nack):                        错误码：ERROR_ALARM_LOG_NOT_EXIST
OSPEVENT(EV_DEL_QUERY_ALARM_INFO,               EV_NMC_NMS_BGN + 425);

// 手动删除所有匹配的告警条目
// nmc -> nmcapp(EV_DEL_ALL_QUERY_ALARM_INFO:req): TAlarmQueryCondition
//        nmcapp -> dbapp (EV_DEL_QUERY_ALARM_INFO:req): u32(要删除的记录总数为0）+ TAlarmQueryCondition
//        nmcapp <- dbapp (EV_DEL_QUERY_ALARM_INFO:ack): u32(要删除的记录总数为0）+ TAlarmQueryCondition
// nmc <- nmcapp (EV_DEL_ALL_QUERY_ALARM_INFO:ack): u32(要删除的记录总数为0）+ TAlarmQueryCondition
// nmc <- nmcapp <- dbapp (nack):           错误码：ERROR_ALARM_LOG_NOT_EXIST
OSPEVENT(EV_DEL_ALL_QUERY_ALARM_INFO,           EV_NMC_NMS_BGN + 426);

// 手动解除告警记录
// nmc -> nmcapp -> serviceapp (req): u32(要解除的记录总数) + 记录数组[u32 (设备流水号) + u32(要解除的告警记录ID)]
// nmc <- nmcapp <- serviceapp (ack): u32(要解除的记录总数) + 记录数组[u32 (设备流水号) + u32(要解除的告警记录ID)]
//                     |
//           dbapp  <- |
OSPEVENT(EV_CLEAR_ALARM_INFO,                   EV_NMC_NMS_BGN + 430);

//查询所有设备的最新告警消息
//由NMC多次请求查询单个设备的最新告警
//nmc -> nms(req):  u32(设备流水号)
//nms -> nmc(ack):  u32(设备流水号) + u8(个数) +　TDevAlarmState数组 + u8(个数) + TCuAlarmInfo数组
OSPEVENT(EV_QUERY_ALLDEV_NEW_ALARM ,            EV_NMC_NMS_BGN + 433);

//统计某个时间段内某个设备发生故障次数
//nmc -> nms(req):    TTroubleStatCondition(故障统计条件) + u32(设备ID)
//nms -> nmc(notify): u32(统计时间点) + u32(发生故障次数) 
OSPEVENT(EV_QUERY_TROUBLE_COUNT,                EV_NMC_NMS_BGN + 434);

//查询新告警消息
//nmc -> nms(req):  u32(设备流水号)
//nms -> nmc(ack):  u32(设备流水号) + u8(个数) +　TDevAlarmState数组 + u8(个数) + TCuAlarmInfo数组
OSPEVENT(EV_QUERY_NEW_ALARM,                    EV_NMC_NMS_BGN + 435);

//统计某个时间段内故障设备处理及时率
//nmc -> nms(req):    TTroubleStatCondition(故障统计条件)
//nms -> nmc(notify): u32(统计时间点) + u32(故障设备处理及时率) 
OSPEVENT(EV_QUERY_TROUBLE_RATE,                 EV_NMC_NMS_BGN + 436);

//故障诊断、完好率统计＋故障统计与报表
//nmc -> nms(req):    TTroubleStatCondition(故障统计条件)
//nms -> nmc(notify): u32(统计时间点) + u32(有故障设备总数) + 
//                    u32(告警统计个数) + 告警统计数组[u32(告警码) + u32(此类告警设备个数)]
OSPEVENT(EV_QUERY_TROUBLE_STAT,                 EV_NMC_NMS_BGN + 437);

//告警自动确认通知
//nms -> nmc(notify): 
OSPEVENT(EV_ALARM_AUTO_CONFIRM,                 EV_NMC_NMS_BGN + 438);

//////////////////////////////////////////////////////////////////////////
//  性能管理
//
// 性能参数上报
// NMC <- NMS(notify): u32(设备ID）+ 性能参数（列表）
OSPEVENT(EV_DEVICE_PFM_INFO,                    EV_NMC_NMS_BGN + 480 );

// 客户端注册请求信息
// NMC -> NMS(req): TNmcReqInfo
// NMC <- NMS(ack): TNmcReqInfo
// NMC <- NMS(nack): TNmcReqInfo
OSPEVENT( EV_NMC_REG_PFM_REQ,                   EV_NMC_NMS_BGN + 485 );
// 客户端注销请求信息
// NMC -> NMS(req): TNmcReqInfo
// NMC <- NMS(ack): TNmcReqInfo
// NMC <- NMS(nack): TNmcReqInfo
OSPEVENT( EV_NMC_UNREG_PFM_REQ,                 EV_NMC_NMS_BGN + 486 );

// nmc -> nmcapp -> serviceapp (req): u32(CMU的设备ID) + u32(CU的会话ID)
// nmc <- nmcapp <- serviceapp (EV_DEVICE_PFM_INFO:notify): u32(CMU的设备ID）+ TTnmCuLogon
OSPEVENT( EV_NMC_CULOG_REQ,                     EV_NMC_NMS_BGN + 487 );

// nmc -> nmcapp -> serviceapp (req): u32(CMU的设备ID) + u32(NODE ID)
OSPEVENT( EV_DEVICE_PFM_LIST_REQ,               EV_NMC_NMS_BGN + 490);

//

//////////////////////////////////////////////////////////////////////////
// 安全管理
// 
// 得到IP限制规则
// nmc -> nmcapp -> dbapp (req): 
// nmc <- nmcapp <- dbapp (notify):u32(规则数）+ TIpRules数组
// nmc <- nmcapp <- dbapp (finish):
//
// nmcapp(daemon) -> dbapp (req): 
// nmcapp(daemon) <- dbapp (notify):u32(规则数）+ TIpRules数组
// nmcapp(daemon) <- dbapp (finish):
OSPEVENT( EV_GET_IP_RULES,                      EV_NMC_NMS_BGN + 530);
// 增加IP限制
// nmc -> nmcapp -> dbapp (req): TIpRules
// nmc <- nmcapp <- dbapp (ack): TIpRules（返回有效的ID）
// nmcapp(daemon) <- dbapp (notify): TIpRules（返回有效的ID）
OSPEVENT( EV_ADD_IP_RULES,                      EV_NMC_NMS_BGN + 531);
// 删除IP限制
// nmc -> nmcapp -> dbapp (req): u32(要删除的IP规则数)+u32数组（IP规则ID数组）
// nmc <- nmcapp <- dbapp (ack): u32(要删除的IP规则数)+u32数组（IP规则ID数组）
// nmcapp(daemon) <- dbapp (notify): u32(要删除的IP规则数)+u32数组（IP规则ID数组）
OSPEVENT( EV_DEL_IP_RULES,                      EV_NMC_NMS_BGN + 532);
// 修改IP限制
// nmc -> nmcapp -> dbapp (req): TIpRules
// nmc <- nmcapp <- dbapp (ack): TIpRules
// nmcapp(daemon) <- dbapp (notify): TIpRules
OSPEVENT( EV_MODIFY_IP_RULES,                   EV_NMC_NMS_BGN + 533);

// 备份数据库数据
// 工作方式：NMC首先发出EV_BACKUP_DISK_INFO请示，得到NMS磁盘信息后再发相
// 应的操作指令
// 备份数据库信息
// NMC -> NMS(req): 
// NMC <- NMS(ack): u8(备份文件名前缀长度) + s8数组(文件名前缀)+ u8(磁盘数量) + TDiskInfo数组
OSPEVENT(EV_BACKUP_DISK_INFO,                   EV_NMC_NMS_BGN + 540);
// NMC -> NMS(req): u8(备份路径名长度）+ 备份路径名
// NMC <- NMS(ack): 
// NMC <- NMS(finish):  
OSPEVENT(EV_BACKUP_NMS_DATA,                    EV_NMC_NMS_BGN + 541);
// 获取自动备份的设置
// nmc -> nmcapp (req): 
// nmc <- nmcapp (ack): TAutoBackup + u8(备份路径名长度）+ 备份路径名
// nmc <- nmcapp (nack):
OSPEVENT(EV_GET_AUTO_BACKUP_CFG,                EV_NMC_NMS_BGN + 542);
// 修改自动备份的设置
// NMC -> NMS(req): TAutoBackup + u8(备份路径名长度）+ 备份路径名
// NMC <- NMS(ack): 
OSPEVENT(EV_AUTO_BACKUP_NMS_DATA,               EV_NMC_NMS_BGN + 543);
// 恢复数据库数据
// NMC -> NMS(req): u8(备份路径名长度）+ 备份路径名
// NMC <- NMS(ack): 
// NMC <- NMS(finish):  
OSPEVENT(EV_RESTORE_NMS_DATA,                   EV_NMC_NMS_BGN + 545);

// 手动锁定界面
// NMC -> NMS(notify): 
OSPEVENT(EV_NMC_LOCK_UI,                        EV_NMC_NMS_BGN + 550);

// 解锁界面
// NMC -> NMS(notify): 
OSPEVENT(EV_NMC_UNLOCK_UI,                      EV_NMC_NMS_BGN + 565);

//////////////////////////////////////////////////////////////////////////
// 操作日志管理
//
// 记录日志操作
// nmc -> nmcapp -> dbapp(req): TActionLogItem
OSPEVENT(EV_REC_ACT_LOG,                        EV_NMC_NMS_BGN + 600);
// 查询操作日志
// NMC -> NMS(req): TActionQueryCondition + u32(请求的记录数) + u32(起始记录号) + u8(是否将查询写入日志VALID_VALUE/INVAID_VALUE)
// NMC <- NMS(notify): u32(匹配条目总数）+ u32(请求的条目数) + u32(本条消息的起始记录数)
//                       + u32(本消息的条目数) + TActionLogItem数组
// NMC <- NMS(finish): 
OSPEVENT(EV_QUERY_ACT_LOG,                      EV_NMC_NMS_BGN + 605);

// 删除操作日志
// nmc -> nmcapp -> dbapp (req): u32(要删除的记录总数）+ u32数组（日志的流水号）
// nmc <- nmcapp <- dbapp (ack): u32(实际删除的记录总数）+ u32数组（日志的流水号）
// nmc <- nmcapp <- dbapp(nack):                        
OSPEVENT(EV_DEL_QUERY_ACT_LOG,                  EV_NMC_NMS_BGN + 610);

// 删除所有匹配的操作日志
// nmc -> nmcapp (EV_DEL_ALL_QUERY_ACT_LOG:req):  TActionQueryCondition
//        nmcapp -> dbapp (EV_DEL_QUERY_ACT_LOG:req): u32(要删除的记录总数为0）+ TActionQueryCondition
//        nmcapp <- dbapp (EV_DEL_QUERY_ACT_LOG:ack): u32(要删除的记录总数为0）+ TActionQueryCondition
// nmc <- nmcapp (EV_DEL_ALL_QUERY_ACT_LOG:ack): TActionQueryCondition
// nmc <- nmcapp <- dbapp (nack):                       
OSPEVENT(EV_DEL_ALL_QUERY_ACT_LOG,              EV_NMC_NMS_BGN + 611);


// 导出操作日志
// nmc -> nmcapp (notify): u8(开始VALID_VALUE/结束INVALID_VALUE)
OSPEVENT(EV_EXPORT_ACT_LOG,                     EV_NMC_NMS_BGN + 615);

// 导出告警日志
// nmc -> nmcapp (notify): u8(开始VALID_VALUE/结束INVALID_VALUE)
OSPEVENT(EV_EXPORT_ALARM_LOG,                   EV_NMC_NMS_BGN + 620);

//与网管客户端同步时间
//nms -> nmc(notify): u32(时间值)
OSPEVENT(EV_SYNTIME_MSG,                        EV_NMC_NMS_BGN + 625);

//////////////////////////////////////////////////////////////////////////
// 设备分组消息
//get 设备分组
//nms->nmc(notify): u16(数组个数)+TDevGroup数组
//nms->nmc(finish):
OSPEVENT(EV_DEV_GROUP_GET,                      EV_NMC_NMS_BGN  + 630);
//get 设备的组信息
//nms->nmc(notify):u16(数组个数)+TDevGroupMem数组
//nms->nmc(finish):
OSPEVENT(EV_DEV_GROUP_MEM_GET,                  EV_NMC_NMS_BGN  + 635);
//添加设备组
//nmc->nms(req): TDevGroup
//nms->nmc(ack): TDevGroup
//nms->nmc(nack):
OSPEVENT(EV_DEV_GROUP_ADD,                      EV_NMC_NMS_BGN  + 640);
//修改设备组信息
//nmc->nms(req):TDevGroup
//nms->nmc(ack):TDevGroup
//nms->nmc(nack):
OSPEVENT(EV_DEV_GROUP_MODIFY,                   EV_NMC_NMS_BGN  + 645);
//增加组内设备成员
//nmc->nms(req):u16(增加的设备个数) + TDevGroupMem数组
//nms->nmc(ack):u16(增加的设备个数) + TDevGroupMem数组
//nms->nmc(nack):
OSPEVENT(EV_DEV_GROUP_MEM_ADD,                  EV_NMC_NMS_BGN  + 650);
//删除组内设备成员
//nmc->nms(req):u16(删除的设备个数) + u32数组
//nms->nmc(ack):u16(删除的设备个数) + u32数组
//nms->nmc(nack):
OSPEVENT(EV_DEV_GROUP_MEM_DEL,                  EV_NMC_NMS_BGN  + 655);
//删除设备组
//nmc->nms(req):u32(DevGroupId)
//nms->nmc(ack):u32(DevGroupId)
//nms->nmc(nack);
OSPEVENT(EV_DEV_GROUP_DEL,                      EV_NMC_NMS_BGN  + 660);
//////////////////////////////////////////////////////////////////////////

inline u32 GetAlarmLevel(u32 dwAlarmCode);
inline s8* GetAlarmLevelDesc(u32 dwLevel);
inline s8* GetActTypeDesc(s32 nActIdx);
inline u32 GetCurTime();
// mask信息
typedef struct tagTnmMask
{   
public:
    tagTnmMask()
    {
        dwMask = 0;
    }
    BOOL32 IsItemValid(u32 dwItemMask) const        // 判断某一个条件是否有效
    {
        return (dwMask & dwItemMask) != 0;
    }
    /*========================================================================
    函 数 名：SetItemMask
    功    能：设置一个或多个字段有效位。
    参    数：u32 dwItemMask                        [in]新的MASK值
              BOOL32 bValid = TRUE                  [in]设置还是清除
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/03  3.9     李洪强  创建
    ========================================================================*/
    inline void SetItemMask(u32 dwItemMask, BOOL32 bValid = TRUE);
    u32  GetMaskInfo() const { return dwMask; }             // 返回当前的所有mask信息
    
    /*========================================================================
    函 数 名：SetMaskAll
    功    能：手动重设置MASK的值函数
    参    数：u32 dwMask                    [in]新的MASK值
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/03  3.9     李洪强  创建
    ========================================================================*/
    inline void SetMaskAll(u32 dwItemMask);
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
    // 2006/05/22 李洪强 增加调试打印接口
    void Print(BOOL32 bNetToHost = FALSE);
protected:
    u32 dwMask;                                 // 按位与，哪些条件起作用。 1为有效，0无效
}PACKED TTnmMask;

inline void PrintDevRight(TTnmMask const *ptDevRight);


//用户信息结构
typedef struct tagUserInfo
{
    tagUserInfo()
    {
        memset(this, 0, sizeof(tagUserInfo));
        dwSerialNo = INVALID_SERIAL_NO;
        dwGroupId = INVALID_SERIAL_NO;
        byGender = GENDER_MALE;
    }
    u32     dwSerialNo;                         // 用户流水号
    u32     dwGroupId;                          // 用户所在组ID
    s8      achUserId[MAX_USER_ID_LEN + 4];     // 用户登录ID
    s8      achUserPwd[MAX_USER_PWD_LEN + 4];   // 用户登录密码 
    s8      achUserName[MAX_USER_NAME_LEN + 2]; // 用户姓名
    s8      achDept[MAX_DEPT_NAME_LEN + 4];     // 部门名称
    s8      achWorkId[MAX_WORK_ID_LEN + 4];     // 工号
    s8      achRank[MAX_RANK_LEN + 2];          // 职务
    s8      achAddress[MAX_ADDR_LEN + 4];       // 联系地址
    s8      achTelePhone[MAX_TELEPHONE_LEN + 4];// 固定电话
    s8      achMobile[MAX_MOBILE_LEN + 4];      // 移动电话
    s8      achEmail[MAX_EMAIL_LEN + 4];        // Email
    s8      achRemark[MAX_REMARK_LEN + 4];      // 备注
    u8      byGender;                           // 用户性别GENDER_MALE/GENDER_FEMALE

    u8      byReserve1;
    u8      byReserve2;
    u8      byReserve3;

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            OspPrintf(TRUE, FALSE, "\tSerialNo: %x; UserId : %s; \n", ntohl(dwSerialNo), achUserId);
            OspPrintf(TRUE, FALSE, "\tGroupId : %x; UserPwd: %s; Gender: %d; \n", 
                        ntohl(dwGroupId), achUserPwd, byGender);            
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tSerialNo: %x; UserId : %s; \n", dwSerialNo, achUserId);
            OspPrintf(TRUE, FALSE, "\tGroupId : %x; UserPwd: %s; Gender: %d; \n", 
                        dwGroupId, achUserPwd, byGender);
        }       
        OspPrintf(TRUE, FALSE, "\tUserName: %s; UserDpt: %s; \n", achUserName, achDept);
        OspPrintf(TRUE, FALSE, "\tWorkId  : %s; UsrRank: %s; \n", achWorkId, achRank);
        OspPrintf(TRUE, FALSE, "\tAddress : %s; TelCall: %s; \n", achAddress, achTelePhone);
        OspPrintf(TRUE, FALSE, "\tMobile  : %s; Email  : %s; \n", achMobile, achEmail);
        OspPrintf(TRUE, FALSE, "\tRemark  : %s; \n", achRemark);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TUserInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TUserInfo::NetToHost()
{
    dwSerialNo = ntohl(dwSerialNo);
    dwGroupId = ntohl(dwGroupId);
}
inline void TUserInfo::HostToNet()
{
    dwSerialNo = htonl(dwSerialNo);
    dwGroupId = htonl(dwGroupId);
}

// 定义用户登录数据，同时可以用于修改用户口令
typedef struct tagNmsLogonReq
{
    tagNmsLogonReq()
    {
        memset(this, 0, sizeof (tagNmsLogonReq));
    }
    s8   achUserId[TNM_MAX_USER_ID_LEN + 1];            // 用户登录ID
    s8   achUserPwd[TNM_MAX_PASSWORD_LEN + 1];          // 用户登录口令
}PACKED TNmsLogonReq;


// 定义用户登录返回数据结构
typedef struct tagNmsLogonRet
{
    tagNmsLogonRet()
    {
        dwResult = 0;
    }

    u32 dwResult;                       // 登录结果
    TUserInfo tUserInfo;                // 登录用户的详细信息               

    void Print(BOOL bNettoHost = FALSE)
    {
        tUserInfo.Print(bNettoHost);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TNmsLogonRet;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TNmsLogonRet::NetToHost()
{
    tUserInfo.NetToHost();
}
inline void TNmsLogonRet::HostToNet()
{
    tUserInfo.HostToNet();
}

// 基本性能参数共公体
typedef union tagPfmInfoType
{
    TTnmCmuPfmInfo  tCmu;                       // 平台性能参数
    TTnmVpuPfmInfo  tVpu;                       // VPU板性能参数
    TTnmNruPfmInfo  tNru;                       // NRU性能参数
    TTnm3asPfmInfo  t3as;                       // 3AS性能参数
    TTnmUasPfmInfo  tUas;                       // UAS性能参数
    TTnmPuPfmInfo   tPu;                        // PU性能参数
    TTnmVtduPfmInfo tVtdu;                      // VTDU性能参数
    TTnmCuiPfmInfo  tCui;                       // CUI性能参数
    TTnmPuiPfmInfo  tPui;                       // PUI性能参数
    TTnmNmsPfmInfo  tNms;                       // NMS性能参数
    TTnmDec5PfmInfo tDec5;                      // DEC5性能参数 
    
    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        memset(this, 0, sizeof(tagPfmInfoType));
    }
}UPfmInfoType;

//renhouping 2006.08.04 修改告警模块
#define CU_ALARMCODE_NUM                    2
#define MAX_ALARM_CU_NUM_ONE_ALARMCODE      32

#define MAX_ALARM_CHANEL_ONE_ALARMCODE      16
typedef struct tagTAlarmBaseInfo
{
    tagTAlarmBaseInfo()
    {
        byAlarmState = TNM_STATUS_NORMAL;
        dwNmsId = INVALID_SERIAL_NO;
        dwOccurTime = 0;
    }
    u32 dwNmsId;            // 记录告警的数据库流水号，主要用于告警恢复时的匹配
    u32 dwOccurTime;        // 告警产生的时间
    u8  byAlarmState;       // 告警状态 enum TnmStatusType, 处于TNM_STATUS_CLEAR
                        // 状态时，再上报同类告警时状态不改变。只有收到
                        // TNM_STATUS_NORMAL时，状态重新被置为正常  
    u8  byConfirmState;
    u8  byReserved2;
    u8  byReserved3;
    //字节转换接口
    void NetToHost();
    void HostToNet();

    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwAlarmSn = dwNmsId;
        if (bNetToHost)
        {
            dwAlarmSn = ntohl(dwAlarmSn);
        }
        OspPrintf(TRUE, FALSE, "AlarmSn: %d; AlarmState: %d;\n", dwAlarmSn, byAlarmState);
    }
}TAlarmBaseInfo;

inline void TAlarmBaseInfo::NetToHost()
{
    dwNmsId = ntohl(dwNmsId);
    dwOccurTime = ntohl(dwOccurTime);
}
inline void TAlarmBaseInfo::HostToNet()
{
    dwNmsId = htonl(dwNmsId);
    dwOccurTime = htonl(dwOccurTime);
}

// 设备告警状态
typedef struct tagDevAlarmState : public TTnmDevAlarm
{
    TAlarmBaseInfo tAlarmBase[MAX_ALARM_CHANEL_ONE_ALARMCODE];
    void NetToHost();
    void HostToNet();
    void Print(BOOL32 bNetToHost = FALSE);
    BOOL32 IsAlarmState();             //当前该告警码的状态是否：TNM_STATUS_ALARM
}TDevAlarmState;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TDevAlarmState::NetToHost()
{
    TTnmDevAlarm::NetToHost();
    s32 nIndex = 0;
    for (; nIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nIndex++)
    {
        tAlarmBase[nIndex].NetToHost();
    }
}

inline void TDevAlarmState::HostToNet()
{
    TTnmDevAlarm::HostToNet();
    s32 nIndex = 0;
    for (; nIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nIndex++)
    {
        tAlarmBase[nIndex].HostToNet();
    }
}

inline void TDevAlarmState::Print(BOOL32 bNetToHost)
{
    TTnmDevAlarm::Print(bNetToHost);
    OspPrintf(TRUE, FALSE, "\n");
    s32 nIndex = 0;
    for (; nIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "\t");
        tAlarmBase[nIndex].Print(bNetToHost);
    }
}

inline BOOL32 TDevAlarmState::IsAlarmState()
{
    s32 nIndex = 0;
    for (; nIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nIndex++)
    {
        if (tAlarmBase[nIndex].byAlarmState == TNM_STATUS_ALARM)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//
typedef struct tagCuAlarmInfo
{
    tagCuAlarmInfo()
    {
        dwAlarmSN = 0;
        dwDevAlarmSN = 0;
        dwOccurTime = 0;
        byAlarmState = TNM_STATUS_NORMAL;
        byConfirmState = TNM_STATUS_NOCONFIRM;
        byHasAlarmInfo = TNM_DISABLE;
    }
    u32 dwAlarmSN;          //记录在数据库中的告警序号
    u32 dwDevAlarmSN;       //设备端发过来的序号
    u32 dwOccurTime;        // 告警产生的时间
    u8  byAlarmState;       // 告警状态 enum TnmStatusType
    u8  byConfirmState;
    u8  byHasAlarmInfo;
    u8  byReserved;
    u16 wAlarmCode;
    u8  byCuAlarmLevel;
    u8  byReserved2;
    s8  achAlarmDesc[MAX_ALARM_DESC_LEN+1];//告警描述

    void NetToHost();
    void HostToNet();
}TCuAlarmInfo;

inline void TCuAlarmInfo::NetToHost()
{
    dwAlarmSN = ntohl(dwAlarmSN);
    dwDevAlarmSN = ntohl(dwDevAlarmSN);
    dwOccurTime = ntohl(dwOccurTime);
    wAlarmCode = ntohs(wAlarmCode);
}

inline void TCuAlarmInfo::HostToNet()
{
    dwAlarmSN = htonl(dwAlarmSN);
    dwDevAlarmSN = htonl(dwDevAlarmSN);
    dwOccurTime = htonl(dwOccurTime);
    wAlarmCode = htons(wAlarmCode);
}

// 设备信息结构
typedef struct tagDeviceInfo : public TTnmDevTopo
{   
    tagDeviceInfo() : tagTnmDevTopo()
    {
        tUnitType.byDeviceType = TNM_UNIT_UNKNOW;
        tUnitType.byServiceType = TNM_SERVICE_UNKNOW;
        dwDevSerialNo = INVALID_SERIAL_NO;
        dwMasterBoardSn = INVALID_SERIAL_NO;
        memset(achAlias, 0, sizeof(achAlias));
        dwIpAddr = 0;
        byField = INVALID_VALUE;
        byRightRange = RIGHT_RANGE_ALL_CHILD;
        byOnline = TNM_STATE_OFFLINE;
        byLayer = 0;
        bySlot = 0;
        byUpgradeMode = TNM_UPGRADE_MODE_NONE;
        byIsOldDevice = TNM_DISABLE;
        memset(achDevTypeName, 0, sizeof(achDevTypeName));
        memset(achBoxID, 0, sizeof(achBoxID));
        ResetAlarm();
        // 初始化告警码
        atDevAlarm[0].dwAlarmCode = TNM_ALARM_CODE_DEV_OFFLINE;
        atDevAlarm[1].dwAlarmCode = TNM_ALARM_CODE_RECV_NOBITSTREAM;
        atDevAlarm[2].dwAlarmCode = TNM_ALARM_CODE_LOSEPACKET;
        atDevAlarm[3].dwAlarmCode = TNM_ALARM_CODE_MAP_STATUS;      
        atDevAlarm[4].dwAlarmCode = TNM_ALARM_CODE_LPT_STATUS;
        atDevAlarm[5].dwAlarmCode = TNM_ALARM_CODE_POWER_STATUS;
        atDevAlarm[6].dwAlarmCode = TNM_ALARM_CODE_CPU_STATUS;
        atDevAlarm[7].dwAlarmCode = TNM_ALARM_CODE_MEMORY_STATUS;
        atDevAlarm[8].dwAlarmCode = TNM_ALARM_CODE_DISKFULL_STATUS;
        atDevAlarm[9].dwAlarmCode = TNM_ALARM_CODE_FAN_STATUS;
        atDevAlarm[10].dwAlarmCode = TNM_ALARM_CODE_ETHCARD_RESTORE;
        atDevAlarm[11].dwAlarmCode = TNM_ALARM_CODE_LED_STATUS;
        atDevAlarm[12].dwAlarmCode = TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS;
        atDevAlarm[13].dwAlarmCode = TNM_ALARM_CODE_SAME_REG_ID;
        atDevAlarm[14].dwAlarmCode = TNM_ALARM_CODE_REG_CMU;
        atDevAlarm[15].dwAlarmCode = TNM_ALARM_CODE_DISKDETECT_ERROR;
        atDevAlarm[16].dwAlarmCode = TNM_ALARM_CU_OVERLOAD;
        atDevAlarm[17].dwAlarmCode = TNM_ALARM_PU_OVERLOAD;
        atDevAlarm[18].dwAlarmCode = TNM_ALARM_VTDU_OVERLOAD;
		atDevAlarm[19].dwAlarmCode = TNM_ALARM_CODE_NODISK_ERROR;
		atDevAlarm[20].dwAlarmCode = TNM_ALARM_CODE_DISK_ERROR;
		atDevAlarm[21].dwAlarmCode = TNM_ALARM_CODE_DISK_NOFORMAT;
		atDevAlarm[22].dwAlarmCode = TNM_ALARM_CODE_STOPREC_LOGERR;
        atDevAlarm[23].dwAlarmCode = TNM_ALARM_CODE_STOPREC_DISKERR;
        atDevAlarm[24].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_3AS;
        atDevAlarm[25].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_UAS;
        atDevAlarm[26].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_PUI;
        atDevAlarm[27].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_CUI;
        atDevAlarm[28].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_NRU;
        atDevAlarm[29].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_VTDU;
        atDevAlarm[30].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_CMU;
        atDevAlarm[31].tBaseDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DISCONNECT_DATABASE;   
    #ifdef BOGUS
        for (u16 wII = 0; wII < CU_ALARMCODE_NUM; wII++)
        {
            for (u16 wIII = 0; wIII < MAX_ALARM_CU_NUM_ONE_ALARMCODE; wIII++)
            {
                if (0 == wII)
                {
                    atCuAlarm[wII][wIII].wAlarmCode = TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET;
                }
                else if (1 == wII)
                {
                    atCuAlarm[wII][wIII].wAlarmCode = TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION;
                }
            }
        }
    #endif /* BOGUS */
        
        InitPfm();
    }
    TTnmUnitType tUnitType;                             //单元类型
    u32 dwDevSerialNo;                                  //设备流水号.
    u32 dwMasterBoardSn;                                //单板的主板设备流水号
    s8  achAlias[MAX_DEVICE_ALIAS_LEN + 4];             //设备别名  
    u32 dwIpAddr;                                       //设备的Ip地址
    TDevAlarmState atDevAlarm[MAX_DEV_ALARM_TYPE];      //设备当前的告警状态
#ifdef BOGUS
    TCuAlarmInfo    atCuAlarm[CU_ALARMCODE_NUM][MAX_ALARM_CU_NUM_ONE_ALARMCODE];
#endif /* BOGUS */
    UPfmInfoType   tPfmInfo;                            //性能参数信息
    u8  byField;                                        //byField代表的节点是否是域
    u8  byRightRange;                                   //权限的类型（此字段对单个的NMC客户端有意义）
                                                        //RIGHT_RANGE_ALL_CHILD
                                                        //RIGHT_RANGE_NO_CHILD
                                                        //RIGHT_RANGE_PART_CHILD
    u8  byOnline;                                       //当前设备状态，是否在线
    u8  byHighestAlarmLevel;                            //最高告警级别
    u8  bySelfAlarmLevel;                               //本设备的最高告警级别
    u8  byLayer;                            
    u8  bySlot;
    u8  byUpgradeMode;                                  //TNM_UPGRADE_MODE_TCP/TNM_UPGRADE_MODE_FTP

    u8  byIsOldDevice;                                  //TNM_ENABLE/TNM_DISABLE
    u8  byReserved;
    u16 wReserved;

    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //设备类型名
    s8  achBoxID[MAX_BOX_ID_LEN + 4];   //机框标识

    /*========================================================================
    函 数 名：UpdateHighestAlarmLevel
    功    能：u8 byLevel                    [in]新的告警级别
              BOOL32 bSet                   [in]TRUE: 用新的值取代原有值
                                            FALSE：用新的值提升告警级别
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/23  3.9     李洪强  创建
    ========================================================================*/
    void UpdateHighestAlarmLevel(u8 byLevel, BOOL32 bSet);
    // 2006/03/23 李洪强 得到本设备的最高告警级别
    u8 GetSelfAlarmLevel() const
    {
        return bySelfAlarmLevel;
    }
    // 2006/03/23 李洪强 得到本设备及所有下属设备的最高告告警级别
    u8 GetHighestAlarmLevel() const
    {
        return byHighestAlarmLevel;
    }
    // 2006/03/22 李洪强 设置/得到在线状态
    BOOL32 IsOnline() const
    {
        return byOnline == TNM_STATE_ONLINE;
    }
    void SetOnline(u8 byOnlineState)
    {
        byOnline = byOnlineState;
    }
    /*========================================================================
    函 数 名：IsField
    功    能：判断设备是否为域的平台设备
    参    数：BOOL32 bCheckSubBoard = FALSE     [in]判断时是否检验有子板,如果为
                            TRUE，则只有下挂单板的才返回TRUE。如果为FALSE，
                            只要是平台业务均返回TRUE
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/07  3.9     李洪强  实现
    ========================================================================*/
    BOOL32 IsField(BOOL32 bCheckSubBoard = FALSE, BOOL32 bCheckById = FALSE) const;
    /*========================================================================
    函 数 名：GetPfmInfo
    功    能：得到基本性能参数
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/21  3.9     李洪强  创建
    ========================================================================*/
    inline void*  GetPfmInfo(s32 &nPfmSize) const;                  // 得到设备的性能参数信息
    /*========================================================================
    函 数 名：GetBoardPfmInfo
    功    能：得到单板的基本性能参数
    参    数：
    返 回 值：单板的基本性能参数指针:   成功        NULL：失败
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/21  3.9     李洪强  创建
    ========================================================================*/
    const TTnmBoardPfmInfo * GetBoardPfmInfo() const;
    /*========================================================================
    函 数 名：IsBoard
    功    能：判断该设备是否为机框中的单板
    参    数：
    返 回 值：TRUE: 是单板          FALSE:  是其他的设备
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/22  3.9     李洪强  创建
    ========================================================================*/
    BOOL32 IsBoard() const;
    /*========================================================================
    函 数 名：UpdateAlarmState
    功    能：更新本地保存的告警状态
    参    数：TTnmDevAlarm * const ptDevAlarm       [in]告警的内容
              u8 byState                            [in]告警产生还是恢复
                                                    TNM_STATUS_ALARM/TNM_STATUS_NORMAL
              u8 byAlarmLevel                       [in]要更新的告警级别
              u32 dwNmsId = 0                       [in]NMS分配的告警ID号
              u32 dwOccurTime = 0                   [in]告警产生的时间
    返 回 值：告警状态是否发生变化，需要记录和通知客户端
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/22  3.9     李洪强  创建
    ========================================================================*/
    BOOL32 UpdateAlarmState(TTnmDevAlarm * const ptDevAlarm, u8 byState, u8 byAlarmLevel,
                u32 dwNmsId = 0, u32 dwOccurTime = 0, u8 byParam = 0);
    
    /*========================================================================
    函 数 名：RecalcAlarmLevel
    功    能：重新计算本设备的告警级别
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/26  3.9     李洪强  创建
    ========================================================================*/
    void RecalcAlarmLevel();

    /*========================================================================
    函 数 名：ClearAlarm
    功    能：手动解除告警状态
    参    数：u32 dwAlarmSn                         [in]要解除的告警流水号
    返 回 值：TRUE: 成功        FALSE：失败
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/22  3.9     李洪强  创建
    ========================================================================*/
    BOOL32 ClearAlarm(u32 dwAlarmSn, u8 byState);
    
    /*========================================================================
    函 数 名：GetDevDesc
    功    能：得到设备的描述(设备名称+设备别名)
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/15  3.9     李洪强  创建
    ========================================================================*/
    BOOL32 GetDevDesc(s8 *pbyDescBuf, s32 nLen) const;
    /*========================================================================
    函 数 名：InitPfm
    功    能：初始化性能参数
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/03  3.9     李洪强  创建
    ========================================================================*/
    void InitPfm();
    
    /*========================================================================
    函 数 名：ResetAlarm
    功    能：重置设备告警信息
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/18  3.9     李洪强  创建
    ========================================================================*/
    void ResetAlarm();

    /*========================================================================
    函 数 名：SetAlarmNmsId
    功    能：设置告警的流水号
    参    数：u32 dwAlarmCode               [in]告警码
              u32 dwNmsId                   [in]告警的流水号
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/18  3.9     李洪强  创建
    ========================================================================*/
    void SetAlarmNmsId(u32 dwAlarmCode, u32 dwNmsId, u8 byParam, u32 dwDevAlarmSN);
    
    void ResetAlarmState(u32 dwAlarmCode, u8 byParam, u32 dwDevAlarmSN);
    /*========================================================================
    函 数 名：GetAlarmNmsId
    功    能：得到告警的流水号
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/18  3.9     李洪强  创建
    ========================================================================*/
    u32  GetAlarmNmsId(u32 dwAlarmCode, u8 byParam, u32 dwDevAlarmSN);
    /*========================================================================
    函 数 名：GetHighAlarmCode
    功    能：返回一个最高告警级别的告警码
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/04/26  3.9     李洪强  创建
    ========================================================================*/
    u32  GetHighAlarmCode();
    /*========================================================================
    函 数 名：PrintBrief
    功    能：调试打印接口, 打印主要的参数信息
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/22  3.9     李洪强  创建
    ========================================================================*/
    void PrintBrief(BOOL32 bNetToHost = FALSE);
    /*========================================================================
    函 数 名：PrintAll
    功    能：打印所有的参数信息
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/22  3.9     李洪强  创建
    ========================================================================*/
    void PrintAll(BOOL32 bNetToHost = FALSE);
    /*========================================================================
    函 数 名：GetServType
    功    能：得到设备的业务类型描述
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/27  3.9     任厚平  创建
    2006/03/29  3.9     李洪强  改为静态成员
    ========================================================================*/
    static s8* GetServType(u8 byServType);

    //得到设备类型字串
    static s8* GetDevType(u8 byDevType);
    // 判断是否为前端设备
    static BOOL32 IsPuDevice(u8 byServiceType);

    //根据设备业务类型打印设备性能参数
    void PrintPfm(TTnmUnitType tUnitType, BOOL32 bNetToHost = FALSE);

    //打印性能参数
    void PrintBasePfm(TTnmBasePfmInfo* ptPfm, BOOL32 bNetToHost = FALSE);
    void PrintBoardPfm(TTnmBoardPfmInfo *ptPfm, BOOL32 bNetToHost = FALSE);
    void PrintHwPfm(TTnmPfmHwInfo *ptPfm, BOOL32 bNetToHost = FALSE);
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}TDeviceInfo;

/*========================================================================
函 数 名：ResetAlarm
功    能：重置设备告警信息
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/18  3.9     李洪强  创建
========================================================================*/
inline void TDeviceInfo::ResetAlarm()
{
    byHighestAlarmLevel = ALARM_NORMAL;
    bySelfAlarmLevel = ALARM_NORMAL;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        TDevAlarmState *ptState = atDevAlarm + nIndex;
        ptState->dwDevSerialNo = INVALID_SERIAL_NO;
        s32 nCount = 0;
        for (; nCount < MAX_ALARM_CHANEL_ONE_ALARMCODE; nCount++)
        {
            TAlarmBaseInfo *ptInfo = &ptState->tAlarmBase[nCount];
            ptInfo->byAlarmState = TNM_STATUS_NORMAL;
            ptInfo->dwNmsId = INVALID_SERIAL_NO;
            ptInfo->dwOccurTime = 0;
        }
    }

#ifdef BOGUS
    u8 byAlarmIdx;
    for(byAlarmIdx = 0; byAlarmIdx < CU_ALARMCODE_NUM; byAlarmIdx++)
    {
        for(s32 nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
        {
            TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
            if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
            {
                continue;
            }

            ptCuAlarmInfo->byAlarmState = TNM_STATUS_NORMAL;
            ptCuAlarmInfo->byHasAlarmInfo = TNM_DISABLE;
            ptCuAlarmInfo->dwAlarmSN = INVALID_SERIAL_NO;
            ptCuAlarmInfo->dwDevAlarmSN = 0;
        }
    }
#endif /* BOGUS */
}

/*========================================================================
函 数 名：SetAlarmNmsId
功    能：设置告警的流水号
参    数：u32 dwAlarmCode               [in]告警码
          u32 dwNmsId                   [in]告警的流水号
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/18  3.9     李洪强  创建
========================================================================*/
inline void TDeviceInfo::SetAlarmNmsId(u32 dwAlarmCode, u32 dwNmsId, u8 byParam, u32 dwDevAlarmSN)
{
    TDevAlarmState *ptState = atDevAlarm;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        if (ptState->dwAlarmCode == dwAlarmCode) 
        {
            TAlarmBaseInfo *ptInfo = &ptState->tAlarmBase[byParam];
            ptInfo->dwNmsId = dwNmsId;
            ptInfo->dwOccurTime = ::GetCurTime();
            return;
        }
        ptState++;
    }
    
#ifdef BOGUS
    u8 byAlarmIdx;
    if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)
    {
        byAlarmIdx = 0;
    }
    else if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)
    {
        byAlarmIdx = 1;
    }
    for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
    {
        TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
        if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
        {
            continue;
        }

        if (dwDevAlarmSN != ptCuAlarmInfo->dwDevAlarmSN)
        {
            continue;
        }

        ptCuAlarmInfo->dwAlarmSN = dwNmsId;
        ptCuAlarmInfo->dwOccurTime = ::GetCurTime();
        return;
    }
#endif /* BOGUS */
}

inline void TDeviceInfo::ResetAlarmState(u32 dwAlarmCode, u8 byParam, u32 dwDevAlarmSN)
{
    TDevAlarmState *ptState = atDevAlarm;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        if (ptState->dwAlarmCode == dwAlarmCode) 
        {
            TAlarmBaseInfo *ptInfo = &(ptState->tAlarmBase[byParam]);
            ptInfo->dwNmsId = INVALID_SERIAL_NO;
            ptInfo->dwOccurTime = 0;
            if (ptInfo->byAlarmState == TNM_STATUS_ALARM)
            {
                ptInfo->byAlarmState = TNM_STATUS_NORMAL;
                //重新计算告警级别
                RecalcAlarmLevel();
            }
            return;
        }
        ptState++;
    }

#ifdef BOGUS
    u8 byAlarmIdx;
    if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)
    {
        byAlarmIdx = 0;
    }
    else if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)
    {
        byAlarmIdx = 1;
    }
    for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
    {
        TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
        if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
        {
            continue;
        }

        if (dwDevAlarmSN != ptCuAlarmInfo->dwDevAlarmSN)
        {
            continue;
        }

        ptCuAlarmInfo->byAlarmState = TNM_STATUS_NORMAL;
        ptCuAlarmInfo->byHasAlarmInfo = TNM_DISABLE;
        ptCuAlarmInfo->dwAlarmSN = INVALID_SERIAL_NO;
        ptCuAlarmInfo->dwDevAlarmSN = 0;
        RecalcAlarmLevel();
    }
#endif /* BOGUS */

    return;
}
/*========================================================================
函 数 名：GetAlarmNmsId
功    能：得到告警的流水号
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/18  3.9     李洪强  创建
========================================================================*/
inline u32 TDeviceInfo::GetAlarmNmsId(u32 dwAlarmCode, u8 byParam, u32 dwDevAlarmSN)
{
    TDevAlarmState *ptState = atDevAlarm;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        if (ptState->dwAlarmCode == dwAlarmCode) 
        {
            TAlarmBaseInfo *ptInfo = &ptState->tAlarmBase[byParam];
            return ptInfo->dwNmsId;
        }
        ptState++;
    }

#ifdef BOGUS
    u8 byAlarmIdx;
    if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == dwAlarmCode)    //cu alarm
    {
        byAlarmIdx = 0;
    }
    else if (TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION == dwAlarmCode)
    {
        byAlarmIdx = 1;
    }

    for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
    {
        TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
        if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
        {
            continue;
        }

        if (dwDevAlarmSN == ptCuAlarmInfo->dwDevAlarmSN)
        {
            return ptCuAlarmInfo->dwAlarmSN;
        }
    }
#endif /* BOGUS */

    return INVALID_SERIAL_NO;
}

/*========================================================================
函 数 名：GetHighAlarmCode
功    能：返回一个最高告警级别的告警码
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/26  3.9     李洪强  创建
========================================================================*/
inline u32 TDeviceInfo::GetHighAlarmCode()
{
    TDevAlarmState *ptState = atDevAlarm;

    u32 dwHighCode = TNM_ALARM_CODE_INVALID;
    
    u32 dwHighLevel = ALARM_NORMAL;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        u32 dwLevel = ::GetAlarmLevel(ptState->dwAlarmCode);
        if (dwLevel > dwHighLevel) 
        {
            dwHighCode = ptState->dwAlarmCode;
        }
        ptState++;
    }

    
#ifdef BOGUS
    u8 byAlarmIdx;
    u32 dwAlarmCode;
    for(byAlarmIdx = 0; byAlarmIdx < CU_ALARMCODE_NUM; byAlarmIdx++)
    {
        if (0 == byAlarmIdx)
        {
            dwAlarmCode = TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET;
        }
        else if (1 == byAlarmIdx)
        {
            dwAlarmCode = TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION;
        }
        for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
        {
            TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
            if (TNM_DISABLE == ptCuAlarmInfo->byHasAlarmInfo)
            {
                continue;
            }

            u32 dwLevel = ::GetAlarmLevel(dwAlarmCode);
            if (dwLevel > dwHighLevel) 
            {
                dwHighCode = dwAlarmCode;
            }

            break;
        }
    }
#endif /* BOGUS */

    return dwHighCode;
}


/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TDeviceInfo::NetToHost()
{
    TTnmDevTopo::NetToHost();

    dwDevSerialNo = ntohl(dwDevSerialNo);
    dwMasterBoardSn = ntohl(dwMasterBoardSn);
    dwIpAddr = ntohl(dwIpAddr);
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        atDevAlarm[nIndex].NetToHost();
    }
    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        tPfmInfo.tPu.NetToHost();
    }
    else
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            tPfmInfo.tCmu.NetToHost();
            break;
        case TNM_PC_SERVICE_NRU:
            tPfmInfo.tNru.NetToHost();
            break;
        case TNM_PC_SERVICE_UAS:
            tPfmInfo.tUas.NetToHost();
            break;
        case TNM_CMU_SERVICE_VTDU:
            tPfmInfo.tVtdu.NetToHost();
            break;
        case TNM_PC_SERVICE_3AS:
            tPfmInfo.t3as.NetToHost();
            break;
        case TNM_CMU_SERVICE_PUI:
            tPfmInfo.tPui.NetToHost();
            break;
        case TNM_CMU_SERVICE_CUI:
            tPfmInfo.tCui.NetToHost();
        case TNM_CMU_SERVICE_VPU:
            tPfmInfo.tVpu.NetToHost();
            break;
        case TNM_PC_SERVICE_NMS:
            tPfmInfo.tNms.NetToHost();
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            tPfmInfo.tDec5.NetToHost();
            break;
        default:
            break;
        }
    }
}
inline void TDeviceInfo::HostToNet()
{
    TTnmDevTopo::HostToNet();

    dwDevSerialNo = htonl(dwDevSerialNo);
    dwMasterBoardSn = htonl(dwMasterBoardSn);
    dwIpAddr = htonl(dwIpAddr);
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        atDevAlarm[nIndex].HostToNet();
    }

    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        tPfmInfo.tPu.HostToNet();
    }
    else
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            tPfmInfo.tCmu.HostToNet();
            break;
        case TNM_PC_SERVICE_NRU:
            tPfmInfo.tNru.HostToNet();
            break;
        case TNM_PC_SERVICE_UAS:
            tPfmInfo.tUas.HostToNet();
            break;
        case TNM_CMU_SERVICE_VTDU:
            tPfmInfo.tVtdu.HostToNet();
            break;
        case TNM_PC_SERVICE_3AS:
            tPfmInfo.t3as.HostToNet();
            break;
        case TNM_CMU_SERVICE_PUI:
            tPfmInfo.tPui.HostToNet();
            break;
        case TNM_CMU_SERVICE_CUI:
            tPfmInfo.tCui.HostToNet();
            break;
        case TNM_CMU_SERVICE_VPU:
            tPfmInfo.tVpu.HostToNet();
            break;
        case TNM_PC_SERVICE_NMS:
            tPfmInfo.tNms.HostToNet();
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            tPfmInfo.tDec5.HostToNet();
            break;
        default:
            break;
        }
    }
}

/*========================================================================
函 数 名：IsField
功    能：判断设备是否为域的平台设备
参    数：BOOL32 bCheckSubBoard = FALSE     [in]判断时是否检验有子板,如果为
                            TRUE，则只有下挂单板的才返回TRUE。如果为FALSE，
                            只要是平台业务均返回TRUE
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/07  3.9     李洪强  实现
========================================================================*/
inline BOOL32 TDeviceInfo::IsField(BOOL32 bCheckSubBoard, BOOL32 bCheckById) const
{
    if (bCheckById) 
    {
        if (tDevId.achID[18] == '5') 
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    u8 bySericeType = tUnitType.byServiceType;
    if (bySericeType == TNM_CMU_SERVICE_KDM2000) 
    {
        return TRUE;
    }
    else if (bySericeType == TNM_CMU_SERVICE_KDM2100A) 
    {
        if (bCheckSubBoard == TRUE) 
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}


/*========================================================================
函 数 名：UpdateHighestAlarmLevel
功    能：u8 byLevel                    [in]新的告警级别
          BOOL32 bSet                   [in]TRUE: 用新的值取代原有值
                                        FALSE：用新的值提升告警级别
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/23  3.9     李洪强  创建
========================================================================*/
inline void TDeviceInfo::UpdateHighestAlarmLevel(u8 byLevel, BOOL32 bSet)
{
    if (bSet) 
    {
        byHighestAlarmLevel = byLevel;
    }
    else
    {
        byHighestAlarmLevel |= byLevel;
    }
}

/*========================================================================
函 数 名：GetBoardPfmInfo
功    能：得到单板的基本性能参数
参    数：
返 回 值：单板的基本性能参数指针:   成功        NULL：失败
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/21  3.9     李洪强  创建
========================================================================*/
inline const TTnmBoardPfmInfo * TDeviceInfo::GetBoardPfmInfo() const
{
    if (IsBoard()) 
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
            {
                return tPfmInfo.tCmu.GetBoardPfmInfo();
            }
            break;
        case TNM_CMU_SERVICE_VTDU:
            {
                return tPfmInfo.tVtdu.GetBoardPfmInfo();
            }
            break;
        case TNM_CMU_SERVICE_PUI:
            {
                return tPfmInfo.tPui.GetBoardPfmInfo();
            }
            break;
        case TNM_CMU_SERVICE_CUI:
            {
                return tPfmInfo.tCui.GetBoardPfmInfo();
            }
            break;
        case TNM_CMU_SERVICE_VPU:
            {
                return tPfmInfo.tVpu.GetBoardPfmInfo();
            }
            break;
//        case TNM_PU_SERVICE_KDMDEC5:
//            {
//                return tPfmInfo.tDec5.GetBoardPfmInfo();
//            }
//            break;
        default:
            {
                return NULL;
            }
            break;
        }
    }
    return NULL;
}

/*========================================================================
函 数 名：IsBoard
功    能：判断该设备是否为机框中的单板
参    数：
返 回 值：TRUE: 是单板          FALSE:  是其他的设备
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/22  3.9     李洪强  创建
========================================================================*/
inline BOOL32 TDeviceInfo::IsBoard() const
{
    u8 byDeviceType = tUnitType.byDeviceType;
    if (byDeviceType == TNM_BOARD_MPC
        || byDeviceType == TNM_BOARD_CRI
        || byDeviceType == TNM_BOARD_DRI
        || byDeviceType == TNM_BOARD_VPU
        || byDeviceType == TNM_BOARD_VAS)
//      || byDeviceType == TNM_BOARD_DEC5  
    {
        return TRUE;
    }

    return FALSE;
}

/*========================================================================
函 数 名：GetPfmInfo
功    能：得到性能参数的指针及内容大小
参    数：s32 &nPfmSize                     [in]性能参数内容大小
返 回 值：性能参数的指针
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/20  3.9     李洪强  创建
========================================================================*/
inline void*  TDeviceInfo::GetPfmInfo(s32 &nPfmSize) const
{
    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        nPfmSize = sizeof(TTnmPuPfmInfo);
    }
    else
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            nPfmSize = sizeof(TTnmCmuPfmInfo);
            break;
        case TNM_PC_SERVICE_NRU:
            nPfmSize = sizeof(TTnmNruPfmInfo);
            break;
        case TNM_PC_SERVICE_UAS:
            nPfmSize = sizeof(TTnmUasPfmInfo);
            break;
        case TNM_CMU_SERVICE_VTDU:
            nPfmSize = sizeof(TTnmVtduPfmInfo);
            break;
        case TNM_PC_SERVICE_3AS:
            nPfmSize = sizeof(TTnm3asPfmInfo);
            break;
        case TNM_CMU_SERVICE_PUI:
            nPfmSize = sizeof(TTnmPuiPfmInfo);
            break;
        case TNM_CMU_SERVICE_CUI:
            nPfmSize = sizeof(TTnmCuiPfmInfo);
            break;
        case TNM_CMU_SERVICE_VPU:
            nPfmSize = sizeof(TTnmVpuPfmInfo);
            break;
        case TNM_PC_SERVICE_NMS:
            nPfmSize = sizeof(TTnmNmsPfmInfo);
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            nPfmSize = sizeof(TTnmDec5PfmInfo);
            break;
        default:
            nPfmSize = 0;
            break;
        }
    }
    return (void*)&tPfmInfo;
}

/*========================================================================
函 数 名：UpdateAlarmState
功    能：更新本地保存的告警状态
参    数：TTnmDevAlarm * const ptDevAlarm       [in]告警的内容
          u8 byState                            [in]告警产生还是恢复
                                                TNM_STATUS_ALARM/TNM_STATUS_NORMAL
          u8 byAlarmLevel                       [in]要更新的告警级别
          u32 dwNmsId = 0                       [in]NMS分配的告警ID号
          u32 dwOccurTime = 0                   [in]告警产生的时间
返 回 值：告警状态是否发生变化，需要记录和通知客户端
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/22  3.9     李洪强  创建
2006/08/07  3.9     任厚平  修改为每个告警码支持8路告警
========================================================================*/
inline BOOL32 TDeviceInfo::UpdateAlarmState(TTnmDevAlarm * const ptDevAlarm,
                            u8 byState, u8 byAlarmLevel, u32 dwNmsId, u32 dwOccurTime, u8 byParam)
{
    NMS_ASSERT(ptDevAlarm);
    if (ptDevAlarm == NULL) 
    {
        return FALSE;
    }
    if (ptDevAlarm->dwAlarmCode == TNM_ALARM_CODE_DEV_OFFLINE) 
    {
        // 上下线不处理
        return TRUE;
    }

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        TDevAlarmState *ptState = atDevAlarm + nIndex;
        if (ptState->dwAlarmCode == ptDevAlarm->dwAlarmCode) 
        {
            //renhouping 2006.08.07
            TAlarmBaseInfo *ptInfo = &ptState->tAlarmBase[byParam];

            if (byState == TNM_STATUS_NORMAL) //如果是告警恢复
            {                
                //判断是否改变状态
                BOOL32 bChange = (ptInfo->byAlarmState != TNM_STATUS_NORMAL);
                
                // 告警恢复
                //BOOL32 bChange = (ptState->byAlarmState != TNM_STATUS_NORMAL);

                ptInfo->byAlarmState = TNM_STATUS_NORMAL;
                ptInfo->byConfirmState = TNM_STATUS_NOCONFIRM;
               
                //??是否要设定设备流水号
                //ptState->dwDevSerialNo = INVALID_SERIAL_NO;
                ptInfo->dwOccurTime = 0;
                
                //重新计算告警级别
                RecalcAlarmLevel();
                return bChange;
            }
            else//如果是告警
            {
                if (dwNmsId != 0)
                {
                    ptInfo->dwNmsId = dwNmsId;
                }
                if (dwOccurTime != 0)//告警时间 ??是否需要
                {
                    ptInfo->dwOccurTime = dwOccurTime;
                }
                // 告警产生 当ptInfo->byAlarmState为
                // TNM_STATUS_CLEAR和TNM_STATUS_ALARM时保持原有状态
                if (ptInfo->byAlarmState == TNM_STATUS_NORMAL) 
                {
                    ptInfo->byAlarmState = TNM_STATUS_ALARM;
                    //如果是当前设备这个告警码的第一个告警，设定设备流水号
                    if (ptState->dwDevSerialNo == INVALID_SERIAL_NO)
                    {
                        ptState->dwDevSerialNo = ptDevAlarm->dwDevSerialNo;
                    }
                    
                    bySelfAlarmLevel |= byAlarmLevel;
                    byHighestAlarmLevel |= bySelfAlarmLevel;
                    return TRUE;
                }
                else if (ptInfo->byAlarmState == TNM_STATUS_CLEAR) 
                {
                    if (ptDevAlarm->dwAlarmCode == TNM_ALARM_CODE_SAME_REG_ID) 
                    {
                        // 重复注册的没有恢复的消息，同时只上报一次，所以CLEAR状态仍然要改变
                        ptInfo->byAlarmState = TNM_STATUS_ALARM;
                        ptState->dwDevSerialNo = ptDevAlarm->dwDevSerialNo;

                        bySelfAlarmLevel |= byAlarmLevel;
                        byHighestAlarmLevel |= bySelfAlarmLevel;
                        return TRUE;
                    }
                }
            }
            return FALSE;
        }
    }

//    if ((TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == ptDevAlarm->dwAlarmCode)
//        ||(TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION == ptDevAlarm->dwAlarmCode))
//    {
//        return TRUE ;
//    }

#ifdef BOGUS
    u8 byAlarmIdx;
    if (TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET == ptDevAlarm->dwAlarmCode)    //cu alarm
    {
        byAlarmIdx = 0;
    }
    else if (TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION == ptDevAlarm->dwAlarmCode)
    {
        byAlarmIdx = 1;
    }

    for(nIndex = 0; nIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nIndex++)
    {
        //atCuAlarm;
        TCuAlarmInfo *ptCuAlarmInfo = &atCuAlarm[byAlarmIdx][nIndex];
//        if (0 == ptCuAlarmInfo->dwAlarmSN)
//        {
//            continue;
//        }

        if (byState == TNM_STATUS_NORMAL) //如果是告警恢复
        {
            if (ptCuAlarmInfo->dwDevAlarmSN != ptDevAlarm->dwDevSerialNo)
            {
                continue;
            }
            //判断是否改变状态
            BOOL32 bChange = (ptCuAlarmInfo->byAlarmState != TNM_STATUS_NORMAL);

            ptCuAlarmInfo->byAlarmState = TNM_STATUS_NORMAL;

            ptCuAlarmInfo->dwOccurTime = 0;

            ptCuAlarmInfo->dwAlarmSN = 0;
            
            //重新计算告警级别
            RecalcAlarmLevel();
            return bChange;
        }
        else//如果是告警
        {
            if (TNM_ENABLE == ptCuAlarmInfo->byHasAlarmInfo)
            {
                if (ptCuAlarmInfo->dwDevAlarmSN == ptDevAlarm->dwDevSerialNo)
                {
                    return FALSE;
                }
                continue;
            }

            ptCuAlarmInfo->byHasAlarmInfo = TNM_ENABLE;

            if (dwNmsId != 0)
            {
                ptCuAlarmInfo->dwAlarmSN = dwNmsId;
            }
            if (dwOccurTime != 0)//告警时间 ??是否需要
            {
                ptCuAlarmInfo->dwOccurTime = dwOccurTime;
            }
            // 告警产生 当ptInfo->byAlarmState为
            // TNM_STATUS_CLEAR和TNM_STATUS_ALARM时保持原有状态
            //if (ptCuAlarmInfo->byAlarmState == TNM_STATUS_NORMAL) 
            {
                ptCuAlarmInfo->byAlarmState = TNM_STATUS_ALARM;
                ptCuAlarmInfo->dwDevAlarmSN = ptDevAlarm->dwDevSerialNo;
                
                bySelfAlarmLevel |= byAlarmLevel;
                byHighestAlarmLevel |= bySelfAlarmLevel;
                return TRUE;
            }
        }
        return FALSE;
    }
#endif /* BOGUS */

    return FALSE;
}


/*========================================================================
函 数 名：RecalcAlarmLevel
功    能：重新计算本设备的告警级别
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/26  3.9     李洪强  创建
========================================================================*/
inline void TDeviceInfo::RecalcAlarmLevel()
{
    //重新计算本设备的最高告警级别
    bySelfAlarmLevel = ALARM_NORMAL;
    byHighestAlarmLevel = ALARM_NORMAL;
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        TDevAlarmState *ptState = atDevAlarm + nIndex;
        if (ptState->IsAlarmState()) 
        {
            bySelfAlarmLevel |= ::GetAlarmLevel(ptState->dwAlarmCode);
        }
    }
    byHighestAlarmLevel |= bySelfAlarmLevel;
}
/*========================================================================
函 数 名：ClearAlarm
功    能：手动解除告警状态
参    数：u32 dwAlarmSn                         [in]要解除的告警流水号
返 回 值：TRUE: 成功        FALSE：失败
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/22  3.9     李洪强  创建
========================================================================*/
inline BOOL32 TDeviceInfo::ClearAlarm(u32 dwAlarmSn, u8 byState)
{
    s32 nIndex = 0;
    TDevAlarmState *ptState = NULL;
    TAlarmBaseInfo *ptBaseInfo = NULL;
    
    for(nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        ptState = atDevAlarm + nIndex;
        s32 nSubIndex = 0;
        for (; nSubIndex < MAX_ALARM_CHANEL_ONE_ALARMCODE; nSubIndex++)
        {
            ptBaseInfo = &ptState->tAlarmBase[nSubIndex];
            if (ptBaseInfo->dwNmsId != dwAlarmSn) 
            {
                continue;
            }
            if (TNM_STATUS_CONFIRM == byState)
            {
                ptBaseInfo->byConfirmState = byState;
            }
            else
            {
                ptBaseInfo->byAlarmState = byState;
                ptBaseInfo->byConfirmState = TNM_STATUS_NOCONFIRM;
            }
            RecalcAlarmLevel();
            return TRUE;
        }
    }

#ifdef BOGUS
    TCuAlarmInfo *ptCuAlarmInfo = NULL;
    for(nIndex = 0; nIndex < CU_ALARMCODE_NUM; nIndex++)
    {
        s32 nSubIndex = 0;
        for (; nSubIndex < MAX_ALARM_CU_NUM_ONE_ALARMCODE; nSubIndex++)
        {
            ptCuAlarmInfo = &atCuAlarm[nIndex][nSubIndex];
            if (ptCuAlarmInfo->dwAlarmSN != dwAlarmSn)
            {
                continue;
            }
            if (TNM_STATUS_CONFIRM == byState)
            {
                ptCuAlarmInfo->byConfirmState = byState;
            }
            else
            {
                ptCuAlarmInfo->byAlarmState = byState; //TNM_STATUS_CLEAR;
                ptCuAlarmInfo->byConfirmState = TNM_STATUS_NOCONFIRM;
                ptCuAlarmInfo->byHasAlarmInfo = TNM_DISABLE;
            }
            RecalcAlarmLevel();
            return TRUE;
        }
    }
#endif /* BOGUS */
    return FALSE;
}

/*========================================================================
函 数 名：GetDevDesc
功    能：得到设备的描述(设备名称+设备别名)
参    数：s8 *pbyDescBuf            [in]设备描述的长度
          s32 nLen                  [in]描述BUF的大小
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/15  3.9     李洪强  创建
========================================================================*/
inline BOOL32 TDeviceInfo::GetDevDesc(s8 *pbyDescBuf, s32 nLen) const
{
    NMS_ASSERT(pbyDescBuf);
    if (pbyDescBuf == NULL) 
    {
        return FALSE;
    }

    s32 nNameLen = strlen(achDevName);
    s32 nCopyLen = 0;
    if (nNameLen > 0 && nNameLen < nLen) 
    {
        strcpy(pbyDescBuf, achDevName);
        nCopyLen = nNameLen;
    }

    s32 nAliasLen = strlen(achAlias);

    if (nCopyLen + nAliasLen < nLen && nAliasLen > 0) 
    {
        if (nCopyLen > 0) 
        {
            strcat(pbyDescBuf, "(");
            strcat(pbyDescBuf, achAlias);
            strcat(pbyDescBuf, ")");
        }
        else
        {
            strcpy(pbyDescBuf, achAlias);
        }
        return TRUE;
    }
    return FALSE;
}

/*========================================================================
函 数 名：InitPfm
功    能：初始化性能参数
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/03  3.9     李洪强  创建
========================================================================*/
inline void TDeviceInfo::InitPfm()
{
    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        tPfmInfo.tPu.Init();
    }
    else
    {
        switch(tUnitType.byServiceType) 
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            tPfmInfo.tCmu.Init();
            break;
        case TNM_PC_SERVICE_NRU:
            tPfmInfo.tNru.Init();
            break;
        case TNM_PC_SERVICE_UAS:
            tPfmInfo.tUas.Init();
            break;
        case TNM_CMU_SERVICE_VTDU:
            tPfmInfo.tVtdu.Init();
            break;
        case TNM_PC_SERVICE_3AS:
            tPfmInfo.t3as.Init();
            break;
        case TNM_CMU_SERVICE_PUI:
            tPfmInfo.tPui.Init();
            break;
        case TNM_CMU_SERVICE_CUI:
            tPfmInfo.tCui.Init();
            break;
        case TNM_CMU_SERVICE_VPU:
            tPfmInfo.tVpu.Init();
            break;
        case TNM_PC_SERVICE_NMS:
            tPfmInfo.tNms.Init();
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            tPfmInfo.tDec5.Init();
            break;
        default:
            tPfmInfo.Init();
            break;
        }
    }
    return;
}

/*========================================================================
函 数 名：PrintBrief
功    能：调试打印接口, 打印主要的参数信息
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/22  3.9     李洪强  创建
2006/03/29  3.9     任厚平  实现
========================================================================*/
inline void TDeviceInfo::PrintBrief(BOOL32 bNetToHost)
{
    //设备简要信息
    if (bNetToHost) 
    {
        OspPrintf(TRUE, FALSE, "\tSn=%d, ", ntohl(dwDevSerialNo));
    }
    else
    {
        OspPrintf(TRUE, FALSE, "\tSn=%d, ", dwDevSerialNo);
    }
    
    OspPrintf(TRUE, FALSE, "Alias=%s, ", achAlias);

    //设备硬件类型和业务类型
    s8 *pchTemp = GetDevType(tUnitType.byDeviceType);
    OspPrintf(TRUE, FALSE, "Dev=%s", pchTemp);
    pchTemp = GetServType(tUnitType.byServiceType);
    OspPrintf(TRUE, FALSE, ", Srv=%s", pchTemp);
    OspPrintf(TRUE, FALSE, ", DevTypeName=%s\n", achDevTypeName);
    
    if (bNetToHost) 
    {
        //单板设备的主板流水号
        OspPrintf(TRUE, FALSE, "\tMBSn=%d, ", ntohl(dwMasterBoardSn));
        
        //设备的Ip地址
        CString csIp = CCallAddr::GetIpStr(ntohl(dwIpAddr));
        OspPrintf(TRUE, FALSE, "Ip=%s, ", ((LPCTSTR)csIp));
    }
    else
    {
        //单板设备的主板流水号
        OspPrintf(TRUE, FALSE, "\tMBSn=%d, ", dwMasterBoardSn);
        
        //设备的Ip地址
        CString csIp = CCallAddr::GetIpStr(dwIpAddr);
        OspPrintf(TRUE, FALSE, "Ip=%s, ", ((LPCTSTR)csIp));
    }

    //是否是域 
    OspPrintf(TRUE, FALSE, "IsField=%d, ", byField);

    //设备是否在线
    OspPrintf(TRUE, FALSE, "Online=%d\n", byOnline);
}
/*========================================================================
函 数 名：PrintAll
功    能：打印所有的参数信息
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/22  3.9     李洪强  创建
2006/03/29  3.9     任厚平  实现
========================================================================*/
inline void TDeviceInfo::PrintAll(BOOL32 bNetToHost)
{
    //首先打印基本信息
    PrintBrief(bNetToHost);
    //层
    OspPrintf(TRUE, FALSE, "\tbyLayer=%d, ", byLayer);
    //槽
    OspPrintf(TRUE, FALSE, "bySlot=%d\n", bySlot);

    //打印权限类型
    OspPrintf(TRUE, FALSE, "\tRigntRange=(%d)", byRightRange);
    switch(byRightRange)
    {
    case RIGHT_RANGE_ALL_CHILD:
        {
            OspPrintf(TRUE, FALSE, "%s\n", "RIGHT_RANGE_ALL_CHILD");
        }
        break;
    case RIGHT_RANGE_NO_CHILD:
        {
            OspPrintf(TRUE, FALSE, "%s\n", "RIGHT_RANGE_NO_CHILD");
        }
        break;
    case RIGHT_RANGE_PART_CHILD:
        {
            OspPrintf(TRUE, FALSE, "%s\n", "RIGHT_RANGE_PART_CHILD");
        }
        break;  
    default:break;
    }
    
    //最高告警级别 本设备的最高告警级别
    OspPrintf(TRUE, FALSE, "\tHighAlarmLevel= %d, SelfAlarmLevel= %d\n", byHighestAlarmLevel,
                    bySelfAlarmLevel);
    //  设备当前的告警状态
    OspPrintf(TRUE, FALSE, "\tCurrent Alarm:");
    s32 nIndex = 0;
    for (nIndex = 0; nIndex < MAX_DEV_ALARM_TYPE; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "\n\t%(%d.) ", nIndex);
        TDevAlarmState *ptState = atDevAlarm + nIndex;
        ptState->Print(bNetToHost);
    
    }
    OspPrintf(TRUE, FALSE, "\n");
    //打印设备性能参数
    OspPrintf(TRUE, FALSE, "\tDevicePfmInfo: \n");
    PrintPfm(tUnitType, bNetToHost);    
}

/*========================================================================
函 数 名：PrintPfm
功    能：根据设备业务类型打印设备性能参数
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/29  3.9     李洪强  创建
2006/03/29  3.9     任厚平  实现
========================================================================*/
inline void TDeviceInfo::PrintPfm(TTnmUnitType tUnitType, BOOL32 bNetToHost)
{
    if (IsPuDevice(tUnitType.byServiceType)) 
    {
        //打印PU性能参数
        TTnmPuPfmInfo *pt = (TTnmPuPfmInfo*)&tPfmInfo.tPu;
        if (pt) 
        {
            pt->Print(bNetToHost);
        }
    }
    else
    {
        switch(tUnitType.byServiceType)
        {
        case TNM_CMU_SERVICE_KDM2000:
        case TNM_CMU_SERVICE_KDM2100A:
            {//打印平台设备性能参数   
                TTnmCmuPfmInfo *pt = (TTnmCmuPfmInfo*)&tPfmInfo.tCmu;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_CMU_SERVICE_PUI:
            {//打印PUI性能参数
                TTnmPuiPfmInfo *pt = (TTnmPuiPfmInfo *)&tPfmInfo.tPui;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_CMU_SERVICE_CUI:
            {//打印CUI性能参数
                TTnmCuiPfmInfo *pt = (TTnmCuiPfmInfo*)&tPfmInfo.tCui;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_CMU_SERVICE_VPU:
            {//打印CUI性能参数
                TTnmVpuPfmInfo *pt = (TTnmVpuPfmInfo*)&tPfmInfo.tVpu;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_CMU_SERVICE_VTDU:
            {//打印VTDU性能参数
                TTnmVtduPfmInfo *pt = (TTnmVtduPfmInfo*)&tPfmInfo.tVtdu;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_PC_SERVICE_NRU:
            {//打印NRU性能参数
                TTnmNruPfmInfo *pt = (TTnmNruPfmInfo*)&tPfmInfo.tNru;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_PC_SERVICE_UAS:
            {//打印UAS性能参数
                TTnmUasPfmInfo *pt = (TTnmUasPfmInfo*)&tPfmInfo.tUas;
                if (pt) 
                {
                    pt->Print(bNetToHost);
                }
            }
            break;
        case TNM_PC_SERVICE_3AS:
            {//打印3AS性能参数
                TTnm3asPfmInfo *pt3as = (TTnm3asPfmInfo*)&tPfmInfo.t3as;
                if (pt3as) 
                {
                    pt3as->Print(bNetToHost);
                }
            }
            break;
        case TNM_PC_SERVICE_NMS:
            {
                // 打印NMS的性能参数
                TTnmNmsPfmInfo *ptNms = (TTnmNmsPfmInfo*)&tPfmInfo.tNms;
                if (ptNms) 
                {
                    ptNms->Print(bNetToHost);
                }
            }
            break;
        case TNM_PU_SERVICE_KDMDEC5:
            {
                TTnmDec5PfmInfo *ptDec5 = (TTnmDec5PfmInfo *)&tPfmInfo.tDec5;
                if(ptDec5)
                {
                    ptDec5->Print(bNetToHost);
                }
            }
            break;
        default:
            break;
        }
    }
}

/*========================================================================
函 数 名：PrintBasePfm
功    能：打印基本性能参数
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/29  3.9     李洪强  创建
2006/03/29  3.9     任厚平  实现
========================================================================*/
inline void TDeviceInfo::PrintBasePfm(TTnmBasePfmInfo* ptPfm, BOOL32 bNetToHost)
{
    OspPrintf(TRUE, FALSE, "\tCpu=%d, Mem=%d, MemStatus=%d, SysLoad:%d\n", 
                ptPfm->byCpuUseRate, ptPfm->byMemUseRate, 
                ptPfm->byMemStatus, ptPfm->bySysLoad);
    OspPrintf(TRUE, FALSE, "\tSysTime=%s, Compile=%s\n",
                GetTimeStr(ptPfm->dwSysTime), ptPfm->achCompileTime);
    OspPrintf(TRUE, FALSE, "\thwver=%s, swver=%s\n",ptPfm->achHardwareVer, ptPfm->achSoftwareVer);
}
/*========================================================================
函 数 名：PrintBoardPfm
功    能：打印单板性能参数
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/29  3.9     李洪强  创建
2006/03/29  3.9     任厚平  实现
========================================================================*/
inline void TDeviceInfo::PrintBoardPfm(TTnmBoardPfmInfo *ptPfm, BOOL32 bNetToHost)
{
    TTnmPfmHwInfo *pt = (TTnmPfmHwInfo*)ptPfm;
    PrintHwPfm(pt, bNetToHost);
    OspPrintf(TRUE, FALSE, "\tLayer : %d      Slot : %d\n", ptPfm->byLayer, ptPfm->bySlot);
}
/*========================================================================
函 数 名：PrintHwPfm
功    能：打印性能参数
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/29  3.9     李洪强  创建
2006/03/29  3.9     任厚平  实现
========================================================================*/
inline void TDeviceInfo::PrintHwPfm(TTnmPfmHwInfo *ptPfm, BOOL32 bNetToHost)
{
    TTnmBasePfmInfo *pt = (TTnmBasePfmInfo*)ptPfm;
    PrintBasePfm(pt, bNetToHost);

    OspPrintf(TRUE, FALSE, "\tWatchDogEnalbe : %d\n", ptPfm->byWatchDogEnalbe);
    OspPrintf(TRUE, FALSE, "\tPannelLed:");
    s32 nIndex = 0;
    for (nIndex = 0; nIndex < TNM_MAX_PANELLED_LEN; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "-%d", ptPfm->abyPanelLed[nIndex]);
    }
    OspPrintf(TRUE, FALSE, "\n");

    OspPrintf(TRUE, FALSE, "\tEnternetStatus: ");
    for (nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "-%d", ptPfm->abyPanelLed[nIndex]);
    }
    OspPrintf(TRUE, FALSE, "\n");

    OspPrintf(TRUE, FALSE, "\tMapStatus  :");
    for (nIndex = 0; nIndex < TNM_MAX_MAP_NUM; nIndex++)
    {
        OspPrintf(TRUE, FALSE, "-%d", ptPfm->abyMapStatus[nIndex]);
    }
    OspPrintf(TRUE, FALSE, "\n");
}

/*========================================================================
函 数 名：GetServType
功    能：得到设备业务类型的描述
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/29  3.9     李洪强  创建
2006/03/29  3.9     任厚平  实现
2006/03/29  3.9     李洪强  改为静态成员
========================================================================*/
inline s8* TDeviceInfo::GetServType(u8 byServType)
{
    switch(byServType)
    {
    case TNM_PU_SERVICE_KDM2400:    return "PU_SERV_KDM2400";
    case TNM_PU_SERVICE_KDM2400_PLUS:   return "PU_SERV_KDM2400P";
    case TNM_PU_SERVICE_KDM2500:    return "PU_SERV_KDM2500";
    case TNM_PU_SERVICE_KDM2500_PLUS:   return "PU_SERV_KDM2500P";
    case TNM_PU_SERVICE_KDM2417:    return "PU_SERV_KDM2417";
    case TNM_PU_SERVICE_KDM2418:    return "PU_SERV_KDM2418";
    case TNM_PU_SERVICE_KDM2428:    return "PU_SERV_KDM2428";
    case TNM_PU_SERVICE_KDM2518:    return "PU_SERV_KDM2518";
    case TNM_PU_SERVICE_KDM2401:    return "PU_SERV_KDM2401";
    case TNM_PU_SERVICE_KDM2401L:   return "PU_SERV_KDM2401L";
    case TNM_PU_SERVICE_KDM2401LS:  return "PU_SERV_KDM2401LS";
    case TNM_PU_SERVICE_KDM2402LS:  return "PU_SERV_KDM2402LS";
    case TNM_PU_SERVICE_KDM2402L:   return "PU_SERV_KDM2402L";
    case TNM_PU_SERVICE_KDM2401S:   return "PU_SERV_KDM2401S";
    case TNM_PU_SERVICE_KDM2461:    return "PU_SERV_KDM2461";
    case TNM_PU_SERVICE_KDM2461L:   return "PU_SERV_KDM2461L";
    case TNM_PU_SERVICE_KDM2560:    return "PU_SERV_KDM2560";
    case TNM_PU_SERVICE_KDM2402S:   return "PU_SERV_KDM2402S";
    case TNM_PU_SERVICE_KDM2440:    return "PU_SERV_KDM2440";
    case TNM_PU_SERVICE_KDM2440P:    return "PU_SERV_KDM2440P";
    case TNM_PU_SERVICE_KDM2402:    return "PU_SERV_KDM2402";
    case TNM_PU_SERVICE_KDV8010A:   return "PU_SERV_KDM8010A";
    case TNM_PU_SERVICE_KDV8010C:   return "PU_SERV_KDM8010C";
    case TNM_PU_SERVICE_PU:         return "PU_SERV_PU";

    case TNM_CMU_SERVICE_KDM2000:   return "CMU_SERV_KDM2000";
    case TNM_CMU_SERVICE_KDM2100A: return "CMU_SERV_KDM2100A";

    case TNM_CMU_SERVICE_PUI:   return "CMU_SERV_PUI";
    case TNM_CMU_SERVICE_CUI:   return "CMU_SERV_CUI";
    case TNM_CMU_SERVICE_VPU:   return "CMU_SERV_VPU";
    case TNM_CMU_SERVICE_VTDU:  return "CMU_SERV_VTDU";
    case TNM_PU_SERVICE_KDMDEC5: return "PU_SERV_DEC5";

    case TNM_PC_SERVICE_NRU:    return "PC_SERV_NRU";
    case TNM_PC_SERVICE_UAS:    return "PC_SERV_UAS";
    case TNM_PC_SERVICE_3AS:    return "PC_SERV_3AS";
    case TNM_PC_SERVICE_ALL:    return "PC_SERV_ALL";
    case TNM_PC_SERVICE_NMS:    return "PC_SERV_NMS";
    default:
        return "SERV_UNKNOW";
    }
}

/*========================================================================
函 数 名：GetDevType
功    能：得到设备硬件类型描述
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/29  3.9     李洪强  创建
2006/03/29  3.9     任厚平  实现
========================================================================*/
inline s8* TDeviceInfo::GetDevType(u8 byDevType)
{
    switch(byDevType)
    {

    case TNM_PU_KDM2400:return "PU_KDM2400";
    case TNM_PU_KDM2400_PLUS:return "PU_KDM2400P";
    case TNM_PU_KDM2500:return "PU_KDM2500";
    case TNM_PU_KDM2500_PLUS:return "PU_KDM2500P";        
    case TNM_PU_KDM2417:return "PU_KDM2417";
    case TNM_PU_KDM2418:return "PU_KDM2418";
    case TNM_PU_KDM2428:return "PU_KDM2428";
    case TNM_PU_KDM2518:return "PU_KDM2518";
    case TNM_PU_KDM2401L:return "PU_KDM2401L";
    case TNM_PU_KDM2401LS:return "PU_KDM2401LS";
    case TNM_PU_KDM2402LS:return "PU_KDM2402LS";
    case TNM_PU_KDM2401S:return "PU_KDM2401S";
    case TNM_PU_KDM2402S:return "PU_KDM2402S";
    case TNM_PU_KDM2402:return "PU_KDM2402";
    case TNM_PU_KDM2401:return "PU_KDM2401";
    case TNM_PU_KDM2461:return "PU_KDM2461";
    case TNM_PU_KDM2461L:return "PU_KDM2461L";
    case TNM_PU_KDM2560:return "PU_KDM2560";
    case TNM_PU_KDV8010A:return "PU_KDV8010A";
    case TNM_PU_KDV8010C:return "PU_KDV8010C";
    case TNM_PU_KDM2440:return "PU_KDM2440";
    case TNM_PU_KDM2440P:return "PU_KDM2440P";
    case TNM_CMU_KDM2100A:return "CMU_KDM2100A";
    case TNM_BOARD_MPC:return "BOARD_MPC";
    case TNM_BOARD_CRI:return "BOARD_CRI";
    case TNM_BOARD_DRI:return "BOARD_DRI";
    case TNM_BOARD_VPU:return "BOARD_VPU";
    case TNM_BOARD_VAS:return "BOARD_VAS";
    case TNM_BOARD_DEC5:return "BOARD_DEC5";
    case TNM_PC_UNIT:return "PC_UNIT";
    default:
        return "UNIT_UNKNOW";
    }
}

/*========================================================================
函 数 名：IsPuDevice
功    能：判断是否为前端设备
参    数：u8 
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/11  3.9     李洪强  创建
========================================================================*/
inline BOOL32 TDeviceInfo::IsPuDevice(u8 byServiceType)
{
    if (byServiceType == TNM_PU_SERVICE_KDM2400
        || byServiceType == TNM_PU_SERVICE_KDM2400_PLUS
        || byServiceType == TNM_PU_SERVICE_KDM2500
        || byServiceType == TNM_PU_SERVICE_KDM2500_PLUS
        || byServiceType == TNM_PU_SERVICE_KDM2417
        || byServiceType == TNM_PU_SERVICE_KDM2418
        || byServiceType == TNM_PU_SERVICE_KDM2428
        || byServiceType == TNM_PU_SERVICE_KDM2518
        || byServiceType == TNM_PU_SERVICE_KDM2401
        || byServiceType == TNM_PU_SERVICE_KDM2401L
        || byServiceType == TNM_PU_SERVICE_KDM2401LS
        || byServiceType == TNM_PU_SERVICE_KDM2402LS
        || byServiceType == TNM_PU_SERVICE_KDM2402L
        || byServiceType == TNM_PU_SERVICE_KDM2401S
        || byServiceType == TNM_PU_SERVICE_KDM2461
        || byServiceType == TNM_PU_SERVICE_KDM2461L
        || byServiceType == TNM_PU_SERVICE_KDM2560
        || byServiceType == TNM_PU_SERVICE_KDM2402S
        || byServiceType == TNM_PU_SERVICE_KDM2402
        || byServiceType == TNM_PU_SERVICE_KDM2440
        || byServiceType == TNM_PU_SERVICE_KDM2440P
        || byServiceType == TNM_PU_SERVICE_KDV8010A
        || byServiceType == TNM_PU_SERVICE_KDV8010C
        || byServiceType == TNM_PU_SERVICE_PU)
    {
        return TRUE;
    }

    return FALSE;
}

//用户信息结构
typedef struct tagChangePwd
{
    tagChangePwd()
    {
        dwSerialNo = INVALID_SERIAL_NO;
        memset(achOldPwd, 0, sizeof(achOldPwd));
        memset(achNewPwd, 0, sizeof(achNewPwd));
    }
    u32     dwSerialNo;                         // 用户流水号
    s8      achOldPwd[MAX_USER_PWD_LEN + 4];    // 用户登录密码 
    s8      achNewPwd[MAX_USER_PWD_LEN + 4];    // 用户登录密码 

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            OspPrintf(TRUE, FALSE, "\tSerialNo: %x; OldPwd: %s; NewPwd: %s;\n", 
                    ntohl(dwSerialNo), achOldPwd, achNewPwd);
            return;
        }
        OspPrintf(TRUE, FALSE, "\tSerialNo: %x; OldPwd: %s; NewPwd: %s;\n", 
                dwSerialNo, achOldPwd, achNewPwd);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TChangePwd;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TChangePwd::NetToHost()
{
    dwSerialNo = ntohl(dwSerialNo);
}
inline void TChangePwd::HostToNet()
{
    dwSerialNo = htonl(dwSerialNo);
}



/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmMask::NetToHost()
{
    dwMask = ntohl(dwMask);
}
inline void TTnmMask::HostToNet()
{
    dwMask = htonl(dwMask);
}

/*========================================================================
函 数 名：Print
功    能：调试打印接口
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/05/22  3.9     李洪强  创建
========================================================================*/
inline void TTnmMask::Print(BOOL32 bNetToHost)
{
    if (bNetToHost) 
    {
        NetToHost();
    }

    ::OspPrintf(TRUE, FALSE, "\tMask=0x%x\n", dwMask);

    if (bNetToHost) 
    {
        HostToNet();
    }
}



/*========================================================================
函 数 名：SetMaskAll
功    能：重载赋值函数
参    数：u32 dwItemMask                    [in]新的MASK值
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/03  3.9     李洪强  创建
========================================================================*/
void TTnmMask::SetMaskAll(u32 dwItemMask)
{
    dwMask = dwItemMask;
}

/*========================================================================
函 数 名：SetItemMask
功    能：设置一个或多个字段有效位。
参    数：u32 dwItemMask                        [in]新的MASK值
          BOOL32 bValid = TRUE                  [in]设置还是清除
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/03  3.9     李洪强  创建
========================================================================*/
inline void TTnmMask::SetItemMask(u32 dwItemMask, BOOL32 bValid)
{
    if (bValid) 
    {
        dwMask |= dwItemMask;
    }
    else
    {
        dwMask &= ~dwItemMask;
    }
}

// 域权限信息
//typedef struct tagFieldRight : public TTnmMask
//{ 
//  TTnmDeviceId tFieldId;
//}PACKED TFieldRight;

// 设备(域)权限信息，域当做为平台设备使用同一结构
typedef struct tagDeviceRight : public TTnmMask
{   
    TTnmDeviceId    tDevId;         // 设备的ID，平台、前端及各类设备
    u8              byRightRange;   // 平台设备对所辖设备的权限类型,该字段只在在NMS发送设备列表时有用
                                    //  RIGHT_RANGE_ALL_CHILD
                                    //  RIGHT_RANGE_NO_CHILD
                                    //  RIGHT_RANGE_PART_CHILD
    u8      byReserve1;
    u8      byReserve2;
    u8      byReserve3;

    // 2006/04/04 李洪强 增加调试接口
    void Print(BOOL32 bNetToHost = FALSE);
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}TDeviceRight;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TDeviceRight::NetToHost()
{
    TTnmMask::NetToHost();
}
inline void TDeviceRight::HostToNet()
{
    TTnmMask::HostToNet();
}

inline void TDeviceRight::Print(BOOL32 bNetToHost)
{
    u32 dwMask = GetMaskInfo();
    if (bNetToHost) 
    {
        dwMask = ntohl(dwMask);
    }
    OspPrintf(TRUE, FALSE, "\tDevId=%s, Range=%x\n", tDevId.achID, byRightRange);

    PrintDevRight(this);
}

// 用户组信息
typedef struct tagUserGroupInfo
{
    tagUserGroupInfo()
    {
        memset(this, 0, sizeof(tagUserGroupInfo));
        dwGroupId = INVALID_SERIAL_NO;
        tFieldId.SetNull();
    } 
    u32             dwGroupId;                              // 用户组Id
    TTnmDeviceId    tFieldId;                       // 父亲用户组的ID
    s8              achGroupName[MAX_GROUP_NAME_LEN + 4];   // 用户组名称
    s8              achGroupDesc[MAX_GROUP_DESC_LEN + 4];   // 用户组名称
    TDeviceRight    atFieldRight[MAX_FIELD_RIGHT_NUM];      // 域权限
    TDeviceRight    atDevRight[MAX_DEVICE_RIGHT_NUM];       // 设备权限
    TTnmMask        tSysRight;                              // 系统管理权限

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            NetToHost();
        }
        
        OspPrintf(TRUE, FALSE, "GroupId=%x, GroupName=%s, Field=%s\n", dwGroupId, achGroupName, tFieldId.achID);
        OspPrintf(TRUE, FALSE, "\tGroupFieldRight: \n");
        s32 nIndex = 0;
        for (nIndex = 0; nIndex < MAX_FIELD_RIGHT_NUM; nIndex++)
        {
            if (atFieldRight[nIndex].tDevId.IsValid()) 
            {
                atFieldRight[nIndex].Print();
            }
            
        }

        OspPrintf(TRUE, FALSE, "\tGroupDevRight : \n");
        for (nIndex = 0; nIndex < MAX_DEVICE_RIGHT_NUM; nIndex++)
        {
            if (atDevRight[nIndex].tDevId.IsValid()) 
            {
                atDevRight[nIndex].Print();
            }
        }
        OspPrintf(TRUE, FALSE, "\tSysRight=0x%x\n", tSysRight.GetMaskInfo());
        if (bNetToHost) 
        {
            HostToNet();
        }
    
    }

    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}TUserGroupInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TUserGroupInfo::NetToHost()
{
    dwGroupId = ntohl(dwGroupId);
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_FIELD_RIGHT_NUM; nIndex++)
    {
        atFieldRight[nIndex].NetToHost();
    }
    for(nIndex = 0; nIndex < MAX_DEVICE_RIGHT_NUM; nIndex++)
    {
        atDevRight[nIndex].NetToHost();
    }
    tSysRight.NetToHost();
}
inline void TUserGroupInfo::HostToNet()
{
    dwGroupId = htonl(dwGroupId);
    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_FIELD_RIGHT_NUM; nIndex++)
    {
        atFieldRight[nIndex].HostToNet();
    }
    for(nIndex = 0; nIndex < MAX_DEVICE_RIGHT_NUM; nIndex++)
    {
        atDevRight[nIndex].HostToNet();
    }
    tSysRight.HostToNet();
}

// 操作日志条目数据结构
typedef struct tagActionLogItem
{
    tagActionLogItem()
    {
        memset(this, 0, sizeof(tagActionLogItem));
        dwSerialNo = INVALID_SERIAL_NO;
        dwUserId = INVALID_SERIAL_NO;
    }
    u32 dwSerialNo;                     // 日志流水号
    u32 dwUserId;                       // 用户登录Id
    s8  achUserId[TNM_MAX_USER_ID_LEN + 4]; // 用户登录ID
    u32 dwFailReason;                   // 操作失败原因
    u32 dwActTime;                      // 操作发生时间
    s8  achActInfo[MAX_LOG_INFO_LEN + 4];// 操作相关信息(详细描述)
    u8  byActType;                      // 操作类型
    u8  byResult;                       // 操作结果SUCESS_VALUE/FAILE_VALUE
    
    u8  byReserve1;
    u8  byReserve2;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwTime = dwActTime;
        u32 dwSn = dwSerialNo;
        u32 dwUId = dwUserId;
        u32 dwFail = dwFailReason;

        if (bNetToHost) 
        {
            dwSn = ntohl(dwSerialNo);
            dwUId = ntohl(dwUserId);
            dwFail = ntohl(dwFailReason);
            dwTime = ntohl(dwTime);
        }
        OspPrintf(TRUE, FALSE, "\tSn=%d, UserId=%s(%d), FRsn=%d, Type=%s(%d), Rlt= %d;\n",
            dwSn, achUserId, dwUId, dwFail, GetActTypeDesc(byActType), byActType, byResult);
            
        if (dwTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tInfo=%s\n\t非法时间值:%x\n", achActInfo, dwTime);
            return;
        }
        struct tm tmTime = *localtime((time_t*)&dwTime);
        OspPrintf(TRUE, FALSE, "\tInfo=%s\n\tTime=%s\n", achActInfo, asctime(&tmTime));
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TActionLogItem;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TActionLogItem::NetToHost()
{
    dwSerialNo = ntohl(dwSerialNo);
    dwUserId = ntohl(dwUserId);
    dwFailReason = ntohl(dwFailReason);
    dwActTime = ntohl(dwActTime);
}
inline void TActionLogItem::HostToNet()
{
    dwSerialNo = htonl(dwSerialNo);
    dwUserId = htonl(dwUserId);
    dwFailReason = htonl(dwFailReason);
    dwActTime = htonl(dwActTime);
}

//告警信息结构
typedef struct tagDevAlarm : public TTnmDevAlarm
{
    tagDevAlarm()
    {
        dwNmsId = INVALID_SERIAL_NO;
        byAlarmLevel = ALARM_NORMAL;
        dwDevId = INVALID_SERIAL_NO;
        //wDevServiceType = TNM_SERVICE_UNKNOW;
        dwDevIp = 0;
        memset(achDevAlias, 0, sizeof(achDevAlias));
        memset(achDesc, 0, sizeof(achDesc));
        memset(achConfirmor, 0, sizeof(achConfirmor));
        memset(achProcessor, 0, sizeof(achProcessor));
        dwAlarmTime = 0;
        dwRecoverTime = 0;
        dwConfirmTime = 0;
        byAlarmState = TNM_STATUS_ALARM;
        byConfirmState = TNM_STATUS_NOCONFIRM;
    }
    u32 dwNmsId;                                        //告警记录的ID，由NMS生成并维护
    TTnmDeviceId tDevId;                                //告警设备的Id, 记录设备的ID信息,主要写日志与显示
    u32 dwDevId;                                        //设备的流水号，用于比较
    u32 dwDevIp;                                        //设备的IP地址
    u32 dwAlarmTime;                                    //告警产生时间  
    u32 dwRecoverTime;                                  //告警恢复时间
    u32 dwConfirmTime;                                  //告警的确认时间
    //u16   wDevServiceType;                                //设备业务类型enum TnmServiceType
    u8  byAlarmState;                                   //告警的状态enum TnmStatusType
    u8  byConfirmState;                                 //告警的确认状态
    u16 wReserved;
    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //设备类型名
    s8  achDevAlias[MAX_DEVICE_ALIAS_LEN + 4];          //设备别名
    s8  achDesc[MAX_ALARM_DESC_LEN + 1];                //告警描述
    s8  achConfirmor[MAX_USER_ID_LEN + 4];              //确认者
    s8  achProcessor[MAX_USER_ID_LEN + 4];              //处理者
    u8  byAlarmLevel;                                   //告警级别
    u8  byParam;                                        //用于告警细分的变量
    u8  byDevLargeType;                                 //设备大类型（区别CMU与PU）
    u8  byReserved;

    void PrintDevAlarm(BOOL bNetToHost = FALSE)
    {
        TTnmDevAlarm::Print(bNetToHost);

        u32 dwSN = dwDevId;
        u32 dwAlarmSn = dwNmsId;
        //u16 wSrvType = wDevServiceType;
        u32 dwIp = dwDevIp;
        u32 dwGenTime = dwAlarmTime;
        u32 dwRecTime = dwRecoverTime;

        if (bNetToHost)
        {       
            dwSN = ntohl(dwSN);
            dwAlarmSn = ntohl(dwAlarmSn);
            dwIp = ntohl(dwIp);
            dwGenTime = ntohl(dwGenTime);
            dwRecTime = ntohl(dwRecTime);
        }
        OspPrintf(TRUE, FALSE, "\tAlarmLevel=%s(%d)\n\tDevId: %s;\n", 
            ::GetAlarmLevelDesc(byAlarmLevel), byAlarmLevel, tDevId.achID);
        OspPrintf(TRUE, FALSE, "\tNmsId=0x%x; DevSn=0x%x; DevIp=%s;\n\tDevType=%s\n",
            dwAlarmSn, dwSN, ((LPCTSTR)CCallAddr::GetIpStr(dwIp)), achDevTypeName);
        s8 achGenTime[24] = {0};
        s8 achRecTime[24] = {0};
        if (dwGenTime & 0x80000000)
        {
            sprintf(achGenTime, "非法时间值: %x;\n", dwGenTime);
        }
        else
        {
            struct tm tmGenTime = *localtime((time_t*)&dwGenTime);
            memcpy(achGenTime, asctime(&tmGenTime), sizeof(achGenTime));
        }
        if (dwRecTime & 0x80000000)
        {
            sprintf(achRecTime, "非法时间值: %x;\n", dwRecTime);
        }
        else
        {
            struct tm tmResTime = *localtime((time_t*)&dwRecTime);
            memcpy(achRecTime, asctime(&tmResTime), sizeof(achRecTime));
        }
                
        OspPrintf(TRUE, FALSE, "\tAlarmState=%s(%d); byParam=%d;\n\tAlarmTime=%s\n\tResumTime: %s\n", 
                ::GetStatusStr(byAlarmState), byAlarmState, byParam, achGenTime, achRecTime);
        OspPrintf(TRUE, FALSE, "\tDevAlias: %s; Desc: %s; \n", achDevAlias, achDesc);

    }
    void NetToHost();
    void HostToNet();
}TDevAlarm;

inline void TDevAlarm::NetToHost()
{
    TTnmDevAlarm::NetToHost();

    dwNmsId = ntohl(dwNmsId);
    dwDevId = ntohl(dwDevId);
    dwDevIp = ntohl(dwDevIp);
    dwAlarmTime = ntohl(dwAlarmTime);
    dwRecoverTime = ntohl(dwRecoverTime);
    dwConfirmTime = ntohl(dwConfirmTime);
    //wDevServiceType = ntohs(wDevServiceType);
}
inline void TDevAlarm::HostToNet()
{
    TTnmDevAlarm::HostToNet();

    dwNmsId = htonl(dwNmsId);
    dwDevId = htonl(dwDevId);
    dwDevIp = htonl(dwDevIp);
    dwAlarmTime = htonl(dwAlarmTime);
    dwRecoverTime = htonl(dwRecoverTime);
    dwConfirmTime = htonl(dwConfirmTime);
    //wDevServiceType = htons(wDevServiceType);
}

//告警知识库条目
typedef struct tagAlarmKnowItem
{
    tagAlarmKnowItem()
    {
        memset(this, 0, sizeof(tagAlarmKnowItem));
        dwSerialNo = INVALID_SERIAL_NO;
        dwDevServiceType = TNM_SERVICE_UNKNOW;
    }
    u32 dwSerialNo;                                 // 流水号
    u32 dwAlarmCode;                                // 告警码
    u32 dwDevServiceType;                           // 设备类型(设备服务类型）
    s8  achDecription[MAX_KNOW_ITEM_DESC_LEN + 4];  // 告警描述
    s8  achCause[MAX_KNOW_ITEM_CAUSE_LEN + 4];      // 告警原因
    s8  achSolution[MAX_KNOW_ITEM_SOLUTION_LEN + 4];// 提示的解决办法
    s8  achRemark[MAX_KNOW_ITEM_REMARK_LEN + 4];    // 备注
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TAlarmKnowItem;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TAlarmKnowItem::NetToHost()
{
    dwSerialNo = ntohl(dwSerialNo);
    dwAlarmCode = ntohl(dwAlarmCode);
    dwDevServiceType = ntohl(dwDevServiceType);
}
inline void TAlarmKnowItem::HostToNet()
{
    dwSerialNo = htonl(dwSerialNo);
    dwAlarmCode = htonl(dwAlarmCode);
    dwDevServiceType = htonl(dwDevServiceType);
}


// 条件匹配的方式
typedef struct tagMatchCondition : public TTnmMask
{   
public:
    tagMatchCondition()
    {
        byMatchType = MATCH_CONDITION_OR;
    }
    void SetMatchType(u8 byType) { byMatchType = byType; }  // 设置匹配类型
    u8  GetMatchType() const { return byMatchType; }        // 得到当前配置类型
protected:
    u8  byMatchType;                // MATCH_CONDITION_OR/MATCH_CONDITION_AND







    
    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TMatchCondition;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TMatchCondition::NetToHost()
{
    TTnmMask::NetToHost();
}
inline void TMatchCondition::HostToNet()
{
    TTnmMask::HostToNet();
}


// 规则
typedef struct tagRules : public TMatchCondition
{
    tagRules()
    {
        dwRulesId = INVALID_SERIAL_NO;
        memset(achRulesName, 0, sizeof(achRulesName));
        byEnable = TNM_DISABLE;
    }
    u32 dwRulesId;                              // 规则ID，由NMS生成
    s8  achRulesName[MAX_RULES_NAME_LEN + 3];   // 规则名称
    u8  byEnable;                               // 是否启用该规则 TNM_DISABLE/TNM_ENABLE
    void PrintRuleInfo(BOOL32 bNetToHost = FALSE)
    {   
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "RulesId: 0x%x; EN: %d; MacT: %d; Mask: 0x%x;\n", 
                ntohl(dwRulesId), byEnable, GetMatchType(), ntohl(GetMaskInfo()));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "RulesId: 0x%x; EN: %d; MacT: %d; Mask: 0x%x;\n", 
                dwRulesId, byEnable, GetMatchType(), GetMaskInfo());
        }
        OspPrintf(TRUE, FALSE, "\tRulName: %s;\n", achRulesName);
    }

    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TRules;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TRules::NetToHost()
{
    TMatchCondition::NetToHost();

    dwRulesId = ntohl(dwRulesId);
}
inline void TRules::HostToNet()
{
    TMatchCondition::HostToNet();

    dwRulesId = htonl(dwRulesId);
}

//告警过滤规则
typedef struct tagAlarmFilterRules: public TRules
{   
    tagAlarmFilterRules()
    {
        byAlarmLevel = ALARM_NORMAL;
        memset(adwAlarmCode, 0, sizeof(adwAlarmCode));
        memset(achDevAlias, 0, sizeof(achDevAlias));
        s32 nIndex = 0;
        //for(nIndex = 0; nIndex < MAX_FILTER_RULES_SERVICE_TYPE_NUM; nIndex++)
        //{
        //  awServiceType[nIndex] = TNM_SERVICE_UNKNOW;
        //}
        for(nIndex = 0; nIndex < MAX_FILTER_RULES_DEVID_NUM; nIndex++)
        {
            atDevId[nIndex].SetNull();
        }
        memset(achDevTypeNameList, 0, sizeof(achDevTypeNameList));
    }
    u32 adwAlarmCode[MAX_FILTER_RULES_CODE_NUM];                // 告警码
    s8  achDevAlias[MAX_FILTER_RULES_ALIAS_NUM][MAX_DEVICE_ALIAS_LEN + 1];  // 设备别名
    TTnmDeviceId atDevId[MAX_FILTER_RULES_DEVID_NUM];           // 设备的ID
    //u16   awServiceType[MAX_FILTER_RULES_SERVICE_TYPE_NUM];       // 设备的业务类型
    s8  achDevTypeNameList[MAX_FILTER_RULES_SERVICE_TYPE_NUM][TNM_MAX_DEVTYPE_NAME_LEN + 1];    //设备类型名列表
    u8  byAlarmLevel;               // 告警级别,按位与
                                    // ALARM_CRITICAL
                                    // ALARM_IMPORTANT
                                    // ALARM_GENERAL
    u8  byReserve;
    u16 wReserve;

    void NetToHost();
    void HostToNet();
}TAlarmFilterRules;

inline void TAlarmFilterRules::NetToHost()
{
    TRules::NetToHost();

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_FILTER_RULES_CODE_NUM; nIndex++)
    {
        adwAlarmCode[nIndex] = ntohl(adwAlarmCode[nIndex]);
    }
    //for(nIndex = 0; nIndex < MAX_FILTER_RULES_SERVICE_TYPE_NUM; nIndex++)
    //{
    //  awServiceType[nIndex] = ntohs(awServiceType[nIndex]);
    //}
}
inline void TAlarmFilterRules::HostToNet()
{
    TRules::HostToNet();

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_FILTER_RULES_CODE_NUM; nIndex++)
    {
        adwAlarmCode[nIndex] = htonl(adwAlarmCode[nIndex]);
    }
    //for(nIndex = 0; nIndex < MAX_FILTER_RULES_SERVICE_TYPE_NUM; nIndex++)
    //{
    //  awServiceType[nIndex] = htons(awServiceType[nIndex]);
    //}
}

//告警查询条件
typedef struct tagAlarmQueryCondition : public TMatchCondition
{   
    tagAlarmQueryCondition ()
    {
        byAlarmLevel = ALARM_NORMAL;
        dwAlarmCode = 0;
        dwDevIp = 0;
        //wServiceType = TNM_SERVICE_UNKNOW;
        byAlarmState = TNM_STATUS_NORMAL;           // 告警状态已恢复
        memset(achDevTypeName, 0, sizeof(achDevTypeName));
        memset(achDeviceAlias, 0, sizeof(achDeviceAlias));
        memset(achAlarmDesc, 0, sizeof(achAlarmDesc));
        dwStartOccurTime = 0;
        dwEndOccurTime = 0;
        dwStartRecoverTime = 0;
        dwEndRecoverTime = 0;
        tDevID.SetNull();
    }
    u32 dwAlarmCode;        // 告警码
    TTnmDeviceId tDevID;    // 设备的ID

    u32 dwDevIp;            // 设备的IP地址
    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //设备类型名
    u8  byAlarmLevel;       // 告警级别, 按位与 ALARM_CRITICAL/ALARM_IMPORTANT/ALARM_GENERAL
    u8  byAlarmState;       // 告警状态 enum TnmStatusType
    u8  byDevLargeType;     //设备大类型（区别CMU与PU）
    u8  byReserved;  
    s8  achDeviceAlias[MAX_DEVICE_ALIAS_LEN + 4];   // 设备别名
    s8  achAlarmDesc[MAX_ALARM_DESC_LEN + 1];       // 告警描述

    u32 dwStartOccurTime;       // 告警产生的最早时间，
    u32 dwEndOccurTime;         // 告警产生的最晚时间

    u32 dwStartRecoverTime;     // 告警恢复的最早时间，
    u32 dwEndRecoverTime;       // 告警恢复的最晚时间

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            NetToHost();        
        }

        OspPrintf(TRUE, FALSE, "Code=%d, DevId=%s, DevIp=%s\n",
            dwAlarmCode, tDevID.achID, ::GetIpStr(dwDevIp));
        OspPrintf(TRUE, FALSE, "\tSerType=%s, Level=%s(%d)\n",
            achDevTypeName, ::GetAlarmLevelDesc(byAlarmLevel), byAlarmLevel);
        OspPrintf(TRUE, FALSE, "\tState=%d; Alias=%s, Dsc=%s\n", 
            byAlarmState, achDeviceAlias, achAlarmDesc);
        if (dwStartOccurTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tStartOcurTime: 非法时间值 %x\n", dwStartOccurTime);
        }
        else
        {
            struct tm tmStartGenTime = *localtime((time_t*)&dwStartOccurTime);
            OspPrintf(TRUE, FALSE, "\tStartOcurTime: %s", asctime(&tmStartGenTime));
        }
        if (dwEndOccurTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tEndOcurTime: 非法时间值 %x\n", dwEndOccurTime);
        }
        else
        {
            struct tm tmEndOccurTime = *localtime((time_t*)&dwEndOccurTime);
            OspPrintf(TRUE, FALSE, "\tEndOcurTime: %s", asctime(&tmEndOccurTime));
        }
        if (dwStartRecoverTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tStartResumTime: 非法时间值 %x\n",dwStartRecoverTime);
        }
        else
        {
            struct tm tmStatrResumTime = *localtime((time_t*)&dwStartRecoverTime);
            OspPrintf(TRUE, FALSE, "\tStartResumTime: %s",asctime(&tmStatrResumTime));
        }
        if (dwEndRecoverTime & 0x80000000)
        {
            OspPrintf(TRUE, FALSE, "\tEndResumTime: 非法时间值 %x", dwEndRecoverTime);
        }
        else
        {
            struct tm tmEndResumTime = *localtime((time_t*)&dwEndRecoverTime);
            OspPrintf(TRUE, FALSE, "\tEndResumTime: %s", asctime(&tmEndResumTime));
        }
        
        if (bNetToHost) 
        {
            HostToNet();
        }
    }

    void NetToHost();
    void HostToNet();
}TAlarmQueryCondition;

inline void TAlarmQueryCondition::NetToHost()
{
    TMatchCondition::NetToHost();

    dwAlarmCode = ntohl(dwAlarmCode);
    //wServiceType = ntohs(wServiceType);
    dwDevIp = ntohl(dwDevIp);
    dwStartOccurTime = ntohl(dwStartOccurTime);
    dwEndOccurTime = ntohl(dwEndOccurTime);
    dwStartRecoverTime = ntohl(dwStartRecoverTime);
    dwEndRecoverTime = ntohl(dwEndRecoverTime);
}
inline void TAlarmQueryCondition::HostToNet()
{
    TMatchCondition::HostToNet();

    dwAlarmCode = htonl(dwAlarmCode);
    dwDevIp = htonl(dwDevIp);
    //wServiceType = htons(wServiceType);
    dwStartOccurTime = htonl(dwStartOccurTime);
    dwEndOccurTime = htonl(dwEndOccurTime);
    dwStartRecoverTime = htonl(dwStartRecoverTime);
    dwEndRecoverTime = htonl(dwEndRecoverTime);
}

//故障统计条件
typedef struct tagTroubleStatCondition
{   
    tagTroubleStatCondition()
    {
        dwStartStatTime = 0;
        dwEndStatTime = 0;
    }
    u32 dwStartStatTime;        //开始统计时间
    u32 dwEndStatTime;          //结束统计时间
    u32 dwStatIntervalTime;     //统计时间间隔(单位: 秒)

    void Print(BOOL32 bNetToHost = FALSE)
    {
    }
    void NetToHost();
    void HostToNet();

}TTroubleStatCondition;

inline void TTroubleStatCondition::NetToHost()
{
    dwStartStatTime = ntohl(dwStartStatTime);
    dwEndStatTime = ntohl(dwEndStatTime);
    dwStatIntervalTime = ntohl(dwStatIntervalTime);
}
inline void TTroubleStatCondition::HostToNet()
{
    dwStartStatTime = htonl(dwStartStatTime);
    dwEndStatTime = htonl(dwEndStatTime);
    dwStatIntervalTime = htonl(dwStatIntervalTime);
}

// 操作日志查询条件
typedef struct tagActionQueryCondition : public TMatchCondition
{   
    tagActionQueryCondition ()
    {
        memset(achUserName, 0, sizeof(achUserName));
        memset(achGroupName, 0, sizeof(achGroupName));
        dwStartTime = 0;
        dwEndTime = 0;
        memset(achActInfo, 0, sizeof(achActInfo));
        memset(abyActType, 0, sizeof(abyActType));
        byActLevelMask = ACTION_INVALID;
    }
    s8  achUserName[MAX_USER_NAME_LEN + 2];     // 用户名
    s8  achGroupName[MAX_GROUP_NAME_LEN + 4];   // 用户组名
    u32 dwStartTime;                        // 匹配的最早时间
    u32 dwEndTime;                          // 匹配的最晚时间
    s8  achActInfo[MAX_LOG_INFO_LEN + 4];   // 操作相关信息(详细描述)
    u8  abyActType[ACT_TYPE_NUM];           // 按操作类型查询 INVALID_VALUE/VALID_VALUE
    u8  byActLevelMask;                     // 按操作级别查询,按位与
                                            // ACTION_IMPORTANT/ACTION_SECONDARY/ACTION_GENERAL

    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwSTime = dwStartTime;
        u32 dwETime = dwEndTime;
        if (bNetToHost) 
        {
            dwSTime = ntohl(dwSTime);
            dwETime = ntohl(dwEndTime);
        }
        s8 achStmp[24] ={0};
        s8 achEtmp[24] = {0};
        if (dwSTime & 0x80000000)
        {
            sprintf(achStmp, "非法时间值: %x\n", dwSTime);
        }
        else
        {
            struct tm tmStime = *localtime((time_t*)&dwSTime);
            memcpy(achStmp, asctime(&tmStime), sizeof(achStmp));
        }
        if (dwETime & 0x80000000)
        {
            sprintf(achEtmp, "非法时间值: %x\n", dwETime);
        }
        else
        {
            struct tm tmEtime = *localtime((time_t*)&dwETime);
            memcpy(achEtmp, asctime(&tmEtime), sizeof(achEtmp));
        }
        
        OspPrintf(TRUE, FALSE, "UName=%s, GName=%s\n", achUserName, achGroupName);
        OspPrintf(TRUE, FALSE, "\tSTime=(%d)%s\n\tETime=(%d)%s\n", 
            dwSTime, achStmp, dwETime, achEtmp);

        OspPrintf(TRUE, FALSE, "\tActInfo: %s; ActLev=", achActInfo);
        if (byActLevelMask & ACTION_IMPORTANT) 
        {
            OspPrintf(TRUE, FALSE, "IMPORTANT, ");
        }
        if (byActLevelMask & ACTION_SECONDARY) 
        {
            OspPrintf(TRUE, FALSE, "SECONDARY, ");
        }
        if (byActLevelMask & ACTION_GENERAL) 
        {
            OspPrintf(TRUE, FALSE, "GENERAL, ");
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tActType: ");  
        u8 byIndex = 0;
        for (byIndex = 0; byIndex < ACT_TYPE_NUM; byIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d", abyActType[byIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        for(byIndex = 0; byIndex < ACT_TYPE_NUM; byIndex++)
        {
            if (abyActType[byIndex] == VALID_VALUE) 
            {
                OspPrintf(TRUE, FALSE, "\t%s\n", ::GetActTypeDesc(byIndex));
            }
            
        }

        u32 dwMaskInfo = GetMaskInfo();
        if (bNetToHost) 
        {
            dwMaskInfo = ntohl(dwMaskInfo);
        }
        u8 byMatch = GetMatchType();
        OspPrintf(TRUE, FALSE, "\tMatch=%d, Mask=0x%x, Findby: \n", byMatch, dwMaskInfo);
        if (dwMaskInfo & ACTION_QUERY_USER_NAME) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_USER_NAME\n");
        }
        if (dwMaskInfo & ACTION_QUERY_GROUP_NAME) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_GROUP_NAME\n");
        }
        if (dwMaskInfo & ACTION_QUERY_TIME) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_TIME\n");
        }
        if (dwMaskInfo & ACTION_QUERY_DEV_ID) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_DEV_ID\n");
        }
        if (dwMaskInfo & ACTION_QUERY_ACT_TYPE) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_ACT_TYPE\n");
        }
        if (dwMaskInfo & ACTION_QUERY_ACT_INFO) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_ACT_INFO\n");
        }
        if (dwMaskInfo & ACTION_QUERY_ACT_LEVEL) 
        {
            OspPrintf(TRUE, FALSE, "\tACTION_QUERY_ACT_LEVEL\n");
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TActionQueryCondition;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TActionQueryCondition::NetToHost()
{
    TMatchCondition::NetToHost();

    dwStartTime = ntohl(dwStartTime);
    dwEndTime = ntohl(dwEndTime);
}
inline void TActionQueryCondition::HostToNet()
{
    TMatchCondition::HostToNet();

    dwStartTime = htonl(dwStartTime);
    dwEndTime = htonl(dwEndTime);
}

// IP地址区域
typedef struct tagIpZone
{
    tagIpZone()
    {
         dwStartIP = 0;
         dwEndIP = 0;
    }
    u32 dwStartIP;              // 起始IP
    u32 dwEndIP;                // 结束IP
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TIpZone;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TIpZone::NetToHost()
{
    dwStartIP = ntohl(dwStartIP);
    dwEndIP = ntohl(dwEndIP);
}
inline void TIpZone::HostToNet()
{
    dwStartIP = htonl(dwStartIP);
    dwEndIP = htonl(dwEndIP);
}

//网段结构
typedef struct tagIpSegment
{
    tagIpSegment()
    {
        dwIpSegment = 0;
        dwNetMask = 0;
    }
    u32 dwIpSegment;
    u32 dwNetMask;
    void HostToNet();
    void NetToHost();
}TIpSegment;

inline void TIpSegment::HostToNet()
{
    dwIpSegment = htonl(dwIpSegment);
    dwNetMask = htonl(dwNetMask);
}

inline void TIpSegment::NetToHost()
{
    dwIpSegment = ntohl(dwIpSegment);
    dwNetMask = ntohl(dwNetMask);
}


// IP受限规则
typedef struct tagIpRules : public TRules
{
    tagIpRules()
    {
        byAccessType = INVALID_VALUE;
        memset(adwIPList, 0, sizeof(adwIPList));
    }
    u32 adwIPList[MAX_IP_RULES_IPLIST_SIZE];            // IP列表
    TIpSegment tIPSegment[MAX_IP_RULES_IPSEGMENT_SIZE]; // IP网段
    TIpZone tIpZone[MAX_IP_RULES_IPZOOM_SIZE];      // IP地址区域
    u8  byAccessType;               // 该规则的访问类型:
                                    // TNM_ENABLE允许访问/TNM_DISABLE 被禁止访问
    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    void PrintIpRules(BOOL32 bNetToHost = FALSE)
    {   
        PrintRuleInfo(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tAccessType: %d;\n", byAccessType);
        OspPrintf(TRUE, FALSE, "\tIpList: ");
        s32 nIndex = 0;
        for (nIndex = 0; nIndex < MAX_IP_RULES_IPLIST_SIZE; nIndex++)
        {
            if ((nIndex + 1)%6 == 0)
            {
                OspPrintf(TRUE, FALSE, "\n\t");
            }
            if (bNetToHost) 
            {
                OspPrintf(TRUE, FALSE, "0x%x-", ntohl(adwIPList[nIndex]));
            }
            else
            {
                OspPrintf(TRUE, FALSE, "0x%x-", adwIPList[nIndex]);
            }
        }
        OspPrintf(TRUE, FALSE, "\n");

        OspPrintf(TRUE, FALSE, "\tIpSeg: ");
        for (nIndex = 0; nIndex < MAX_IP_RULES_IPSEGMENT_SIZE; nIndex++)
        {
            if ((nIndex+1)%6 == 0)
            {
                OspPrintf(TRUE, FALSE, "\n\t");
            }
            if (bNetToHost) 
            {
                OspPrintf(TRUE,FALSE, "(0x%x:0x%x)-", ntohl(tIPSegment[nIndex].dwIpSegment), ntohl(tIPSegment[nIndex].dwNetMask));
            }
            else
            {
                OspPrintf(TRUE,FALSE, "(0x%x:0x%x)-", tIPSegment[nIndex].dwIpSegment, tIPSegment[nIndex].dwNetMask);
            }
            
        }
        OspPrintf(TRUE, FALSE, "\n");

        OspPrintf(TRUE, FALSE, "\tIpZone: ");
        for (nIndex = 0; nIndex < MAX_IP_RULES_IPZOOM_SIZE; nIndex++)
        {
            if ((nIndex+1)%2 == 0)
            {
                OspPrintf(TRUE, FALSE, "\n\t");
            }
            if (bNetToHost) 
            {
                OspPrintf(TRUE, FALSE, "(0x%x, 0x%x)-", 
                    ntohl(tIpZone[nIndex].dwStartIP), ntohl(tIpZone[nIndex].dwEndIP));
            }
            else
            {
                OspPrintf(TRUE, FALSE, "(0x%x, 0x%x)-", 
                    tIpZone[nIndex].dwStartIP, tIpZone[nIndex].dwEndIP);
            }
        }
        OspPrintf(TRUE, FALSE, "\n");

    }

    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}TIpRules;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TIpRules::NetToHost()
{
    TRules::NetToHost();

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPLIST_SIZE; nIndex++)
    {
        adwIPList[nIndex] = ntohl(adwIPList[nIndex]);
    }
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPSEGMENT_SIZE; nIndex++)
    {
        tIPSegment[nIndex].NetToHost();
    }
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPZOOM_SIZE; nIndex++)
    {
        tIpZone[nIndex].NetToHost();
    }
}
inline void TIpRules::HostToNet()
{
    TRules::HostToNet();

    s32 nIndex = 0;
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPLIST_SIZE; nIndex++)
    {
        adwIPList[nIndex] = htonl(adwIPList[nIndex]);
    }
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPSEGMENT_SIZE; nIndex++)
    {
        tIPSegment[nIndex].HostToNet();
    }
    for(nIndex = 0; nIndex < MAX_IP_RULES_IPZOOM_SIZE; nIndex++)
    {
        tIpZone[nIndex].HostToNet();
    }
}

//发送Email的规则
typedef struct tagTEmailRules : public TRules
{
    tagTEmailRules()
    {
        //memset(awServiceTypeList, 0, sizeof(awServiceTypeList));
        memset(achDevTypeNameList, 0, sizeof(achDevTypeNameList));
        memset(achDevNameList, 0, sizeof(achDevNameList));
        memset(adwDevIpList, 0, sizeof(adwDevIpList));
        memset(adwAlarmCodeList, 0, sizeof(adwAlarmCodeList));
        byAlarmLevel = ALARM_NORMAL;
        memset(achMailList, 0, sizeof(achMailList));
        memset(achSmsList, 0, sizeof(achMailList));
    }

    //u16   awServiceTypeList[MAX_MAIL_RULES_DEV_TYPE_SIZE];    // 设备类型名列表
    s8  achDevTypeNameList[MAX_MAIL_RULES_DEV_TYPE_SIZE][TNM_MAX_DEVTYPE_NAME_LEN + 1]; //设备类型名列表
    TTnmDeviceId atDevIdList[MAX_MAIL_RULES_DEV_ID_SIZE];   // 设备Id 列表
    s8  achDevNameList[MAX_MAIL_RULES_DEV_NAME_SIZE][MAX_DEVICE_ALIAS_LEN + 4]; // 设备别名列表
    u32 adwDevIpList[MAX_MAIL_RULES_DEV_IP_SIZE];               // 设备IP列表
    u32 adwAlarmCodeList[MAX_MAIL_RULES_ALARM_CODE_SIZE];       // 告警码列表
    s8  achMailList[MAX_MAIL_RULES_ADDR_NUM][MAX_EMAIL_LEN + 4];    // 发送邮件列表
    s8  achSmsList[MAX_SMS_RULES_ADDR_NUM][MAX_SMS_LEN + 4];    // 发送短信列表
    u8  byAlarmLevel;           // 告警级别， 按位与，
                                // ALARM_CRITICAL/ALARM_IMPORTANT/ALARM_GENERAL
    u8  byReserved;
    u16 wReserved;
    void NetToHost();
    void HostToNet();
}TEmailRules;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TEmailRules::NetToHost()
{
    TRules::NetToHost();

    s32 nIndex = 0;
//  for(nIndex = 0; nIndex < MAX_MAIL_RULES_DEV_TYPE_SIZE; nIndex++)
//  {
//      awServiceTypeList[nIndex] = ntohs(awServiceTypeList[nIndex]);
//  }
    for(nIndex = 0; nIndex < MAX_MAIL_RULES_DEV_IP_SIZE; nIndex++)
    {
        adwDevIpList[nIndex] = ntohl(adwDevIpList[nIndex]);
    }
    for(nIndex = 0; nIndex < MAX_MAIL_RULES_ALARM_CODE_SIZE; nIndex++)
    {
        adwAlarmCodeList[nIndex] = ntohl(adwAlarmCodeList[nIndex]);
    }
}
inline void TEmailRules::HostToNet()
{
    TRules::HostToNet();

    s32 nIndex = 0;
//  for(nIndex = 0; nIndex < MAX_MAIL_RULES_DEV_TYPE_SIZE; nIndex++)
//  {
//      awServiceTypeList[nIndex] = htons(awServiceTypeList[nIndex]);
//  }
    for(nIndex = 0; nIndex < MAX_MAIL_RULES_DEV_IP_SIZE; nIndex++)
    {
        adwDevIpList[nIndex] = htonl(adwDevIpList[nIndex]);
    }
    for(nIndex = 0; nIndex < MAX_MAIL_RULES_ALARM_CODE_SIZE; nIndex++)
    {
        adwAlarmCodeList[nIndex] = htonl(adwAlarmCodeList[nIndex]);
    }
}

//定义服务器配置变量
typedef struct tagNmsCfgInfo
{
    tagNmsCfgInfo()
    {
        memset(this, 0, sizeof (tagNmsCfgInfo));
    }

    u32 dwNmsIp;                                    //网管服务器地址
    u32 dwOnlinePeriod;                             //离线呼叫的定时时间
    u32 dwStatusPeriod;                             //状态轮询的定时时间
    u32 dwViewPfmPeriod;                            //用户查看设备的性能轮询定时时间
    u32 dwCheckTopoPeriod;                          //检查拓朴完整性的定时时间
    u32 dwMaxDevCount;                              //网管服务器接受注册的最大设备个数  
    TTnmDeviceId tFieldId;                          //所要管理的域Id
    u8  byIsMaster;                                 //是否主网管
    u8  byIfChild;                                  //是否包括所有子域
    u8  byDbType;                                   //数据库类型 #define DBTYPE_SYBASE     1   //sybase数据库
                                                    //           #define DBTYPE_MYSQL      2   //mysql 数据库
    u8  byHBNum;                                    //设备断链的检测次数

    u32 dwDBIpAddr;                                 //服务器连接的数据库地址
    u32 dwDbLinkNum;                                //数据库连接数
    u16 wDBPort;                                    //连接数据库的端口号
    u16 wOspConnPort;                               //服务器Osp侦听端口
    s8  achDBName[MAX_DB_NAME_LEN + 4];             //服务器连接的数据库名
    s8  achDBUser[TNM_MAX_USER_ID_LEN + 4];         //服务器连接的用户名
    s8  achDBPwd[TNM_MAX_PASSWORD_LEN + 4];         //服务器连接的用户密码
    u16 wDbLangId;                                  //DB_LANG_SC_CHN
    u16 wDbCharSetId;                               //DB_CHARSET_GB18030

    u32 dwAutoClrLogClock;                          //自动清除日志的时刻
    u32 dwAutoClrAlarmClock;                        //自动清除告警的时刻

    u32 dwMaxActLogSaveDay;                         //操作日志最大保存天数
    u32 dwMaxAlarmSaveDay;                          //告警日志最大保存天数

    u8  byIsAutoConfirm;                            //是否自动确认告警
    u8  byIsAutoBackupDevCfg;                       //是否自动备份设备配置文件
    u16 wAutoConfirmTime;                           //自动确认告警的时间(分钟)
    u16 wAutoBackupDevCfgTime;                      //自动备份设备配置文件的时间(分钟)
    u8  byReserved;

    u8  byMemThreshold;                             //系统内存使用阈值（0-100）
    u8  byCpuThreshold;                             //系统CPU使用阈值（0-100）
    u16 wReserved;

    //mail
    u8  byMailServEnable;                           //是否启用邮件服务器配置  1 启用.0 没启用
    u32 dwMailServerIP;                             //邮件服务器的IP地址
    u16 wMailServerPort;                            //邮件服务器的端口(默认为：25)
    u8  byMailServerAuthMode;                       //邮件服务器认证方式 0:无认证 1:login 2:plain 3:cram_md5 4:digest_md5
    u8  byReserved1;
    s8  achEmailAuthUser[TNM_MAX_USER_ID_LEN + 4];  //邮件服务器认证用户名
    s8  achEmailAuthPwd[TNM_MAX_PASSWORD_LEN + 4];  //邮件服务器认证密码
    u32 dwMailInterval;                             //邮件发送间隔(分钟)

    s8  achBkFileName[MAX_BK_FILE_NAME_LEN + 4];    // 数据库备份的文件名前缀
    u16 wHBTime;                                    // 设备断链的检测时间
    u8  byLogSwitch;                                //系统是否记录日志的开关

    //SMS
    u8  bySmsServEnable;                            //是否启用SMS服务器配置  1 启用.0 没启用
    u32 dwSmsServerIp;                              //服务器IP
    u16 wSmsServerPort;                             //服务器端口，默认为7777，如果填0，表示使用默认值
    u16 wReserved2;
    s8  achSmsSrcAddr[MAX_SMS_LEN + 4];             //本地服务号
    s8  achSmsAccount[TNM_MAX_USER_ID_LEN + 4];     //用户名
    s8  achSmsPassword[TNM_MAX_PASSWORD_LEN + 4];   //密码

    //北向接口参数
    u8  byNorthEnable;                              //1 启用.0 没启用
    u8  byReserved3;
    u16 wNorthListenPort;
    u32 dwNorthProxyIp;
    u16 wNorthProxyPort;
    u16 wReserved3;

    void Print(BOOL bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tNmsIp=%s, OspPort=0x%x, Online=0x%x，ViewPfm=0x%x, CheckTopo=%d\n",
            ((LPCTSTR)CCallAddr::GetIpStr(dwNmsIp)), wOspConnPort, dwOnlinePeriod, dwViewPfmPeriod, dwCheckTopoPeriod);
        OspPrintf(TRUE, FALSE, "\tStatusPeriod=0x%x, DbIp=%s, DbPort=0x%x, LinkNum=0x%x, DbType=0x%x;\n", 
            dwStatusPeriod, ((LPCTSTR)CCallAddr::GetIpStr(dwDBIpAddr)), wDBPort, dwDbLinkNum, byDbType);
        OspPrintf(TRUE, FALSE, "\tDbName: %s; DbUser: %s; DbPwd: %s; DbLanId: %d; DbCharSet:%d\n", 
            achDBName, achDBUser, achDBPwd, wDbLangId, wDbCharSetId);
        OspPrintf(TRUE, FALSE, "\tClsLogClk=0x%x, ClsAlrClk=0x%x, MaxLogDay=0x%x, MaxAlrDay=0x%x\n",
            dwAutoClrLogClock, dwAutoClrAlarmClock, dwMaxActLogSaveDay, 
            dwMaxAlarmSaveDay);
        OspPrintf(TRUE, FALSE, "\tMailSvrIp: %s; MailInterval=0x%x; BkFileName: %s\n",
            ((LPCTSTR)CCallAddr::GetIpStr(dwMailServerIP)), dwMailInterval, achBkFileName);
        OspPrintf(TRUE, FALSE, "\tHB=0x%x, HBNum=%d, MaxDevCount=%d, IsMaster=%d\n", wHBTime, byHBNum, dwMaxDevCount, byIsMaster);
        OspPrintf(TRUE, FALSE, "\tFieldId=%s, IfChild=%d, logswitch=%d\n", tFieldId.achID, byIfChild, byLogSwitch);
        OspPrintf(TRUE, FALSE, "\tNorthEnable=%d, ListenPort=%d, NorthProxyIp=%s, NorthProxyPort=%d\n",
                                byNorthEnable, wNorthListenPort, ((LPCTSTR)CCallAddr::GetIpStr(dwNorthProxyIp)), wNorthProxyPort);

        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}TNmsCfgInfo;

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TNmsCfgInfo::NetToHost()
{
    dwNmsIp = ntohl(dwNmsIp);
    dwOnlinePeriod = ntohl(dwOnlinePeriod);
    dwStatusPeriod = ntohl(dwStatusPeriod);
    dwViewPfmPeriod = ntohl(dwViewPfmPeriod);
    dwCheckTopoPeriod = ntohl(dwCheckTopoPeriod);
    dwDBIpAddr = ntohl(dwDBIpAddr);
    dwDbLinkNum = ntohl(dwDbLinkNum);
    wDBPort = ntohs(wDBPort);
    wOspConnPort = ntohs(wOspConnPort);
    dwAutoClrLogClock = ntohl(dwAutoClrLogClock);
    dwAutoClrAlarmClock = ntohl(dwAutoClrAlarmClock);
    dwMaxActLogSaveDay = ntohl(dwMaxActLogSaveDay);
    dwMaxAlarmSaveDay = ntohl(dwMaxAlarmSaveDay);
    dwMailServerIP = ntohl(dwMailServerIP);
    wMailServerPort = ntohs(wMailServerPort);
    dwMailInterval = ntohl(dwMailInterval);
    wHBTime = ntohs(wHBTime);
    wAutoConfirmTime = ntohs(wAutoConfirmTime);
    wAutoBackupDevCfgTime = ntohs(wAutoBackupDevCfgTime);

    dwSmsServerIp = ntohl(dwSmsServerIp);
    wSmsServerPort = ntohs(wSmsServerPort);

    wNorthListenPort = ntohs(wNorthListenPort);
}

inline void TNmsCfgInfo::HostToNet()
{
    dwNmsIp = htonl(dwNmsIp);
    dwOnlinePeriod = htonl(dwOnlinePeriod);
    dwStatusPeriod = htonl(dwStatusPeriod);
    dwViewPfmPeriod = htonl(dwViewPfmPeriod);
    dwCheckTopoPeriod = htonl(dwCheckTopoPeriod);
    dwDBIpAddr = htonl(dwDBIpAddr);
    dwDbLinkNum = htonl(dwDbLinkNum);
    wDBPort = htons(wDBPort);
    wOspConnPort = htons(wOspConnPort);
    dwAutoClrLogClock = htonl(dwAutoClrLogClock);
    dwAutoClrAlarmClock = htonl(dwAutoClrAlarmClock);
    dwMaxActLogSaveDay = htonl(dwMaxActLogSaveDay);
    dwMaxAlarmSaveDay = htonl(dwMaxAlarmSaveDay);
    dwMailServerIP = htonl(dwMailServerIP);
    wMailServerPort = htons(wMailServerPort);
    dwMailInterval = htonl(dwMailInterval);
    wHBTime = htons(wHBTime);
    wAutoConfirmTime = htons(wAutoConfirmTime);
    wAutoBackupDevCfgTime = htons(wAutoBackupDevCfgTime);

    dwSmsServerIp = htonl(dwSmsServerIp);
    wSmsServerPort = htons(wSmsServerPort);

    wNorthListenPort = htons(wNorthListenPort);
}

// 磁盘信息
typedef struct tagDiskInfo
{
    tagDiskInfo()
    {
        byDiskLabel = 0;
        dwFreeSpace = 0;
    }
    u32 dwFreeSpace;        // 剩余磁盘空间(M)
    u8  byDiskLabel;        // 盘符 a-z

    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE,FALSE, "FreeSpace: 0x%x; DiskLabel: %c\n", ntohl(dwFreeSpace), byDiskLabel);
        }
        else
        {
            OspPrintf(TRUE,FALSE, "FreeSpace: 0x%x; DiskLabel: %c\n", dwFreeSpace, byDiskLabel);
        }
        
    }

    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TDiskInfo;

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TDiskInfo::NetToHost()
{
    dwFreeSpace = ntohl(dwFreeSpace);
}
inline void TDiskInfo::HostToNet()
{
    dwFreeSpace = htonl(dwFreeSpace);
}
typedef struct tagAutoAction
{
    tagAutoAction()
    {
        byPolicy = AUTO_ACT_PERIODICAL;
        byActDay = 1;
        dwActTime = 0;
    }
    u32 dwActTime;          // 备份的时间(min)
    u8  byActDay;           // 在哪一天备份 当byPolicy为AUTO_ACT_WEEKDAY时取值为
                            // WEEK_MONDAY
                            // WEEK_TUESDAY
                            // WEEK_WEDNESDAY
                            // WEEK_THURSDAY
                            // WEEK_FRIDAY
                            // WEEK_SATURDAY
                            // WEEK_SUNDAY
                            // 当byPolicy为AUTO_ACT_PERIODICAL时取值为天数
    u8  byPolicy;           // 自动动作的策略
                            // AUTO_ACT_WEEKDAY
                            // AUTO_ACT_PERIODICAL

    u8  byReserve1;
    u8  byReserve2;
    
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwTime = dwActTime;
        if (bNetToHost) 
        {
            dwTime = ntohl(dwTime);
        }
        
        OspPrintf(TRUE, FALSE, "Day: %d; Poli: %d; Timevalue: %x;\n", byActDay, byPolicy, dwTime);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TAutoAction;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TAutoAction::NetToHost()
{
    dwActTime = ntohl(dwActTime);

}
inline void TAutoAction::HostToNet()
{
    dwActTime = htonl(dwActTime);
}

typedef struct tagAutoBackup : public TAutoAction
{
    tagAutoBackup()
    {
        byEnable = TNM_ENABLE;
    }
    u8  byEnable;           // 是否启用自动备份功能 TNM_ENABLE/TNM_DISABLE

    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;

    void PrintAutoBkup(BOOL32 bNetToHost = FALSE)
    {
        Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tEnable: %d; \n", byEnable);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TAutoBackup;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TAutoBackup::NetToHost()
{
    TAutoAction::NetToHost();

}
inline void TAutoBackup::HostToNet()
{
    TAutoAction::HostToNet();
}

typedef struct tagAutoClearAct : public TAutoAction
{
    tagAutoClearAct()
    {
        byEnable = TNM_ENABLE;
    }
    u8  byEnable;           // 是否启用自动清除操作日志功能 TNM_ENABLE/TNM_DISABLE

    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TAutoClearAct;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TAutoClearAct::NetToHost()
{
    TAutoAction::NetToHost();

}
inline void TAutoClearAct::HostToNet()
{
    TAutoAction::HostToNet();
}

typedef struct tagAutoClearAlarm : public TAutoAction
{
    tagAutoClearAlarm()
    {
        byEnable = TNM_ENABLE;
    }
    u8  byEnable;           // 是否启用自动清除告警功能 TNM_ENABLE/TNM_DISABLE

    u8  byReserve1;
    u8  byReserve2;
    u8  byReserve3;
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TAutoClearAlarm;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TAutoClearAlarm::NetToHost()
{
    TAutoAction::NetToHost();

}
inline void TAutoClearAlarm::HostToNet()
{
    TAutoAction::HostToNet();
}


// 个性化信息
typedef struct tagTPreferInfo
{
    tagTPreferInfo()
    {
        memset(this, 0, sizeof (tagTPreferInfo));
    }
    // to do
}PACKED TPreferInfo;    

// 客户端请求信息,请求内容参见前面宏定义NMC_REQ_BASE_PFM_INFO|NMC_REQ_EXTEND_PFM_INFO
typedef struct tagTNmcReqInfo : public TTnmMask
{
    tagTNmcReqInfo()
    {
        dwNmcInsId = 0;
        dwDevId = INVALID_SERIAL_NO;
    }
    u32 dwNmcInsId;     // NMC的实例号
    u32 dwDevId;        // 设备的ID
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmMask::Print(bNetToHost);

        if (bNetToHost) 
        {
            NetToHost();
        }

        u32 dwMaskInfo = GetMaskInfo();

        OspPrintf(TRUE, FALSE, "\tNmcInsId: 0x%x; DevId: 0x%x;\n", dwNmcInsId, dwDevId);

        OspPrintf(TRUE, FALSE, "\t");
        if (dwMaskInfo & NMC_REQ_EXTEND_PFM_INFO)
        {
            OspPrintf(TRUE, FALSE, "NMC_REQ_EXTEND_PFM_INFO | ");
        }
        
        if (dwMaskInfo & NMC_REQ_BASE_PFM_INFO)
        {
            OspPrintf(TRUE, FALSE, "NMC_REQ_BASE_PFM_INFO | ");
        }

        if (dwMaskInfo & NMC_REQ_FOR_MOCK_DRAW) 
        {
            OspPrintf(TRUE, FALSE, "NMC_REQ_FOR_MOCK_DRAW | ");
        }

        if (bNetToHost) 
        {
            HostToNet();
        }

    }

    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TNmcReqInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TNmcReqInfo::NetToHost()
{
    TTnmMask::NetToHost();

    dwNmcInsId = ntohl(dwNmcInsId);
    dwDevId = ntohl(dwDevId);
}
inline void TNmcReqInfo::HostToNet()
{
    TTnmMask::HostToNet();

    dwNmcInsId = htonl(dwNmcInsId);
    dwDevId = htonl(dwDevId);
}

// PING结构
typedef struct tagPingInfo
{
    tagPingInfo()
    {
        nEchTime = 0;
        dwTTL = 0;
        dwDevId = INVALID_SERIAL_NO;
        dwDestIp = 0;
        dwPingThreadId = 0;
    }
    s32 nEchTime;
    u32 dwTTL;
    u32 dwDevId;        // 设备ID
    u32 dwDestIp;       // 目标设备IP
    u32 dwPingThreadId;    // 每个ping线程的ID

    void Print(BOOL bNetToHost = FALSE) //renhouping 2006.04.08
    {
        if (bNetToHost)
        {
            OspPrintf(TRUE, FALSE, "EachTime: %d; TTL: %x; DevId: %x; DestIp: %s;\n",
                    ntohl(nEchTime), ntohl(dwTTL), ntohl(dwDevId), 
                    ((LPCTSTR)CCallAddr::GetIpStr(ntohl(dwDestIp))));
            return;             
        }
        OspPrintf(TRUE, FALSE, "EachTime: %d; TTL: %x; DevId: %x; DestIp: %s;\n",
                    nEchTime, dwTTL, dwDevId, ((LPCTSTR)CCallAddr::GetIpStr(dwDestIp)));
                    
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TPingInfo;

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TPingInfo::NetToHost()
{
    nEchTime = ntohl(nEchTime);
    dwTTL = ntohl(dwTTL);
    dwDevId = ntohl(dwDevId);
    dwDestIp = ntohl(dwDestIp);
    dwPingThreadId = ntohl(dwPingThreadId);
}
inline void TPingInfo::HostToNet()
{
    nEchTime = htonl(nEchTime);
    dwTTL = htonl(dwTTL);
    dwDevId = htonl(dwDevId);
    dwDestIp = htonl(dwDestIp);
    dwPingThreadId = htonl(dwPingThreadId);
}

/*========================================================================
函 数 名：GetAlarmLevel
功    能: 通过告警码得到告警级别
参    数：u32 dwAlarmCode               [in]告警码
返 回 值：告警级别
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/08  3.9     李洪强  创建
2006/03/13  3.9     任厚平  实现
========================================================================*/
inline u32 GetAlarmLevel(u32 dwAlarmCode)
{
    switch(dwAlarmCode)
    {
        //严重告警
    case TNM_ALARM_CODE_DEV_OFFLINE:           //10001       设备下线   
    case TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS:   //10013       前端设备视频源丢失告警 
    case TNM_ALARM_CODE_RECV_NOBITSTREAM:      //10002       接收无码流上报 
    case TNM_ALARM_CODE_MAP_STATUS:            //10004       MAP死、异常告警    
    case TNM_ALARM_CODE_CPU_STATUS:            //10008       CPU使用率高于指定阈值告警          
    case TNM_ALARM_CODE_MEMORY_STATUS:         //10009       内存使用超过指定阈值告警       
    case TNM_ALARM_CODE_DISKFULL_STATUS:       //10010       磁盘满告警
    case TNM_ALARM_CODE_SAME_REG_ID:           //10015       设备ID冲突
    case TNM_ALARM_CODE_REG_CMU:               //10016       PU或其他模块注册CMU失败告警
    case TNM_ALARM_CODE_DISKDETECT_ERROR:      //10017       设备检测无磁盘或无可用磁盘告警
    case TNM_ALARM_CU_OVERLOAD:
    case TNM_ALARM_PU_OVERLOAD:
    case TNM_ALARM_VTDU_OVERLOAD:
	case TNM_ALARM_CODE_NODISK_ERROR:			//10100       无硬盘 - 按IDE编号(0-3)  
	case TNM_ALARM_CODE_DISK_ERROR:				//10101       硬盘分区坏 -按分区编号(0-16)
    case TNM_ALARM_CODE_DISK_NOFORMAT:			//10102       硬盘分区未格式化 - 按分区编号(0-16)
    case TNM_ALARM_CODE_STOPREC_LOGERR:			//10103       前端停录 - 日志系统坏停录
    case TNM_ALARM_CODE_STOPREC_DISKERR:		//10104       前端停录 - 硬盘坏停录
    case TNM_ALARM_CODE_DISCONNECT_3AS:         //11001       未连接上3AS
    case TNM_ALARM_CODE_DISCONNECT_UAS:         //11002       未连接上UAS
    case TNM_ALARM_CODE_DISCONNECT_PUI:         //11003       未连接上PUI
    case TNM_ALARM_CODE_DISCONNECT_CUI:         //11004       未连接上CUI
    case TNM_ALARM_CODE_DISCONNECT_NRU:         //11005       未连接上NRU
    case TNM_ALARM_CODE_DISCONNECT_VTDU:        //11006       未连接上VTDU
    case TNM_ALARM_CODE_DISCONNECT_CMU:         //11007       未连接上CMU
    case TNM_ALARM_CODE_DISCONNECT_DATABASE:    //11008       未连接上DATABASE

        {   
            return ALARM_CRITICAL;
        }

        //重要告警
    case TNM_ALARM_CODE_LOSEPACKET:            //10003       码流接收丢包超过指定阈值告警       
    case TNM_ALARM_CODE_LPT_STATUS:            //10006       并口告警上报       
    case TNM_ALARM_CODE_POWER_STATUS:          //10007       电源电压低于指定阈值告警
    case TNM_ALARM_CODE_FAN_STATUS:            //10011       风扇转速过低告警
        {
            return ALARM_IMPORTANT;
        }

        //普通告警
    case TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET:            
    case TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION:
    case TNM_ALARM_CODE_MOTIONDETECT_STATUS:   //10005       移动侦测告警   
    case TNM_ALARM_CODE_ETHCARD_RESTORE:       //10012       以太网恢复告警
        {
            return ALARM_GENERAL;
        }   
    
    default:
        break;
    }
    return ALARM_CRITICAL;
}


/*========================================================================
函 数 名：GetAlarmLevelDesc
功    能：调试接口：得到告警级别的文字描述
参    数：u32 dwLevel                   [in]告警级别
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/30  3.9     李洪强  创建
========================================================================*/
inline s8* GetAlarmLevelDesc(u32 dwLevel)
{
    switch(dwLevel) 
    {
    case ALARM_NORMAL:      return "ALARM_NORMAL";
    case ALARM_IMPORTANT:   return "ALARM_IMPORTANT";
    case ALARM_GENERAL:     return "ALARM_GENERAL";
    case ALARM_CRITICAL:    return "ALARM_CRITICAL";
    default:
        break;
    }
    return "ALARM_NORMAL";
}

/*========================================================================
函 数 名：GetSubevDesc
功    能：调试接口：得到消息子类型的描述
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/30  3.9     李洪强  创建
========================================================================*/
inline s8* GetSubevDesc(u8 bySubev)
{
    switch(bySubev) 
    {
    case SUBEV_TYPE_REQ:        return "REQ";
    case SUBEV_TYPE_ACK:        return "ACK";
    case SUBEV_TYPE_NACK:       return "NACK";
    case SUBEV_TYPE_NOTIFY:     return "NOTIFY";
    case SUBEV_TYPE_FINISH:     return "FINISH";
    case SUBEV_TYPE_INVALID:    return "INVALID";
    default:
        break;
    }
    return "INVALID";
}

/*========================================================================
函 数 名：GetActTypeDesc
功    能：调试接口： 得到操作类型
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/13  3.9     李洪强  创建
========================================================================*/
inline s8* GetActTypeDesc(s32 nActIdx)
{
    switch(nActIdx) 
    {
    case ACT_TYPE_LOGON_IDX:            return "LOGON";
    case ACT_TYPE_LOGOFF_IDX:           return "LOGOFF";
    case ACT_TYPE_ADD_GROUP:            return "ADD_GROUP";
    case ACT_TYPE_MODIFY_GROUP:         return "MODIFY_GROUP";
    case ACT_TYPE_DEL_GROUP:            return "DEL_GROUP";
    case ACT_TYPE_ADD_USER:             return "ADD_USER";
    case ACT_TYPE_MODIFY_USER:          return "MODIFY_USER";
    case ACT_TYPE_DEL_USER:             return "DEL_USER";
    case ACT_TYPE_MODIFY_ACCOUNT:       return "MODIFY_ACCOUNT";
    case ACT_TYPE_SAVE_PREFER:          return "SAVE_PREFER";
    case ACT_TYPE_EXPORT_PREFER:        return "EXPORT_PREFER";
    case ACT_TYPE_IMPORT_PREFER:        return "IMPORT_PREFER";
    case ACT_TYPE_DEFAULT_VALUE:        return "DEFAULT_VALUE";
    case ACT_TYPE_GET_DEV_CFG:          return "GET_DEV_CFG";
    case ACT_TYPE_SET_DEV_CFG:          return "SET_DEV_CFG";
    case ACT_TYPE_REBOOT:               return "REBOOT";
    case ACT_TYPE_PING:                 return "PING";
    case ACT_TYPE_SELFTEST:             return "SELFTEST";
    case ACT_TYPE_SOFTWARE_UPDATE:      return "SOFTWARE_UPDATE";
    case ACT_TYPE_ALARM_SETTING:        return "ALARM_SETTING";
    case ACT_TYPE_ALARM_ANALYSE:        return "ALARM_ANALYSE";
    case ACT_TYPE_ALARM_QUERY:          return "ALARM_QUERY";
    case ACT_TYPE_DEL_ALARM:            return "DEL_ALARM";
    case ACT_TYPE_EXPORT_ALARM:         return "EXPORT_ALARM";
    case ACT_TYPE_RELEASE_ALARM:        return "RELEASE_ALARM";
    case ACT_TYPE_GET_DEV_PFM:          return "GET_DEV_PFM";
    case ACT_TYPE_PFM_ANALYSE:          return "PFM_ANALYSE";
    case ACT_TYPE_SET_IP_RULES:         return "SET_IP_RULES";
    case ACT_TYPE_SET_LOGIN_LOCK_TIME:  return "SET_LOGIN_LOCK_TIME";
    case ACT_TYPE_MANUAL_BACKUP_DB:     return "MANUAL_BACKUP_DB";
    case ACT_TYPE_RECOVER_DB:           return "RECOVER_DB";
    case ACT_TYPE_QUERY_ACT_LOG:        return "QUERY_ACT_LOG";
    case ACT_TYPE_EXPORT_ACT_LOG:       return "EXPORT_ACT_LOG";
    case ACT_TYPE_DEL_ACT_LOG:          return "DEL_ACT_LOG";
    case ACT_TYPE_LOCK_NMCUI:           return "LOCK_NMCUI";
    case ACT_TYPE_UNLOCK_NMCUI:         return "UNLOCK_NMCUI";
    case ACT_TYPE_SET_NMS_CFG:          return "SET_NMS_CFG";
    case ACT_TYPE_VIEW_DEV_PIC:         return "VIEW_DEV_PIC";
    default:
        break;
    }
    return NULL;
}

/*========================================================================
函 数 名：PrintDevRight
功    能：打印设备权限的信息
参    数：TTnmMask const *ptDevRight        [in]设备权限
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/19  3.9     李洪强  创建
========================================================================*/
inline void PrintDevRight(TTnmMask const *ptDevRight)
{
    OspPrintf(TRUE, FALSE, "\tMask=0x%x\n", ptDevRight->GetMaskInfo());

    if (ptDevRight->IsItemValid(DEV_RIGHT_VIEW_CFG)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_VIEW_CFG\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_VIEW_PFM)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_VIEW_PFM\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_MODIFY_CFG)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_MODIFY_CFG\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_REBOOT)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_REBOOT\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_VIEW_CFG)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_PING\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_VIEW_LOG)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_VIEW_LOG\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_SELFTEST)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_SELFTEST\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_SOFTWARE_UPDATE)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_SOFTWARE_UPDATE\n");
    }
    if (ptDevRight->IsItemValid(DEV_RIGHT_OWNER)) 
    {
        OspPrintf(TRUE, FALSE, "\tDEV_RIGHT_OWNER\n");
    }
}

/*========================================================================
函 数 名：GetCurTime
功    能：得到当前的时间值
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/05/19  3.9     李洪强  创建
========================================================================*/
inline u32 GetCurTime()
{
    u32 dwCurTime = 0;
#ifdef WIN32

    CTime tCurTime = CTime::GetCurrentTime();
    dwCurTime = (u32)tCurTime.GetTime();
#else
    dwCurTime = time(NULL);
#endif // WIN32

    return dwCurTime;
}

//向看门狗注册消息结构
typedef struct tagMsgInfo
{
    u16 wMsgEventId;        //消息值
    u8 byMsgSubEvent;       //子消息值
    u8 byApp;               //接收消息的App
}TMsgInfo;

//域的TFieldInfo结构
typedef struct tagTFieldInfo
{
        tagTFieldInfo()
        {
            memset(achAlias, 0, sizeof(achAlias));
            tFieldId.SetNull();
            tParentField.SetNull();
        }   
        TTnmDeviceId    tFieldId;           //域Id (对应平台的设备Id)
        TTnmDeviceId    tParentField;   //上级域Id (对应平台的设备Id)
        s8              achAlias[MAX_DEVICE_ALIAS_LEN + 4]; // 域名
        void Print(BOOL32 bNetToHost = FALSE);
        void NetToHost(){};
        void HostToNet(){};
        void Reset();
}PACKED TFieldInfo;

inline void TFieldInfo::Print(BOOL32 bNetToHost)
{
    OspPrintf(TRUE, FALSE, "\tFId=%s; Alias=%s;\n", tFieldId.achID, achAlias);
    OspPrintf(TRUE, FALSE, "\tPId=%s;\n", tParentField.achID);
}

inline void TFieldInfo::Reset()
{
    memset(achAlias, 0, sizeof(achAlias));
    tFieldId.SetNull();
    tParentField.SetNull();
}


//记录写告警信息入数据库失败的数据结构  (网管服务器内部使用无字节序问题)
typedef struct tagAlarmDrop
{
    tagAlarmDrop()
    {
        m_dwAlarmCode = 0;
        m_byState = 0;
        m_tRecTime = 0;
        m_byReserved1 = 0;
        m_byReserved2 = 0;
        m_byReserved3 = 0;
    }
    u32 m_dwAlarmCode;
    u32 m_dwDevId;                              // 设备的流水号，用于比较
    time_t m_tRecTime;
    u8  m_byState;
    u8  m_byReserved1;
    u8  m_byReserved2;
    u8  m_byReserved3;

    //打印函数
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "\tAlarmCode=%d; DevSn=%d; AlarmState=%d;\n", m_dwAlarmCode, m_dwDevId, m_byState);
        struct tm *ptmtime = localtime(&m_tRecTime);
        if (!ptmtime)
        {
            OspPrintf(TRUE, FALSE, "\tRecTime: 1970.00.00 08:00:00\n");
            return;
        }
        OspPrintf(TRUE, FALSE, "\tRecTime: %s", asctime(ptmtime));
    }
}PACKED TAlarmDroped;

//网管服务器看门狗配置
typedef struct tagNmsDogCfg
{
    tagNmsDogCfg()
    {
        m_dwTimeInterval = 0;
        m_wStaveCount = 0;
        m_byEnable = 0;
    }
    u32 m_dwTimeInterval;
    u16 m_wStaveCount;
    u8 m_byEnable;
    u8 m_byNetType;
}PACKED TNmsDogCfg;

//设备组数据结构
typedef struct tagDevGroup
{
    tagDevGroup()
    {
        dwDevGroupId = 0;
        dwUserId = 0;
        memset(achPDevId, 0, sizeof(achPDevId));
        memset(achGroupName, 0, sizeof(achGroupName));
    }
    u32 dwDevGroupId;
    u32 dwUserId;
    s8 achPDevId[TNM_MAX_DEVICE_ID_LEN+4];
    s8 achGroupName[MAX_DEVICE_ALIAS_LEN+4];

    void Print(BOOL32 bNetToHost = FALSE);
    void NetToHost();
    void HostToNet();
}PACKED TDevGroup;

inline void TDevGroup::Print(BOOL32 bNetToHost)
{
    if(bNetToHost)
    {
        OspPrintf(TRUE, FALSE, "\tDGroupID=%d; PDevID=%s;\n", ntohl(dwDevGroupId), achPDevId);
        OspPrintf(TRUE, FALSE, "\tUserId = %d; DGroupName=%s;\n", ntohl(dwUserId), achGroupName);
    }
    OspPrintf(TRUE, FALSE, "\tDGroupID=%d; PDevID=%s;\n", dwDevGroupId, achPDevId);
    OspPrintf(TRUE, FALSE, "\tUserId = %d; DGroupName=%s;\n", dwUserId, achGroupName);
}

inline void TDevGroup::NetToHost()
{
    dwDevGroupId = ntohl(dwDevGroupId);
    dwUserId = ntohl(dwUserId);
}

inline void TDevGroup::HostToNet()
{
    dwDevGroupId = htonl(dwDevGroupId);
    dwUserId = htonl(dwUserId);
}

typedef struct tagDevGroupMem
{
    tagDevGroupMem()
    {
        dwItemSn = 0;
        dwGroupId = 0;
        dwUserId = 0;
        memset(achDevId, 0, sizeof(achDevId));
    }
    u32 dwItemSn;
    u32 dwGroupId;
    u32 dwUserId;
    s8 achDevId[TNM_MAX_DEVICE_ID_LEN+4];

    void Print(BOOL32 bNetToHost);
    void NetToHost();
    void HostToNet();
}PACKED TDevGroupMem;

inline void TDevGroupMem::Print(BOOL32 bNetToHost)
{
    if (bNetToHost)
    {
        OspPrintf(TRUE, FALSE, "\tItemSn=%d;GroupId=%d; UserId=%d; DevId=%s;\n",ntohl(dwItemSn), ntohl(dwGroupId), ntohl(dwUserId), achDevId);
    }
    OspPrintf(TRUE, FALSE, "\tItemSn=%d;GroupId=%d; UserId=%d; DevId=%s;\n",dwItemSn, dwGroupId, dwUserId, achDevId);
}

inline void TDevGroupMem::NetToHost()
{
    dwItemSn = ntohl(dwItemSn);
    dwGroupId = ntohl(dwGroupId);
    dwUserId = ntohl(dwUserId);
}

inline void TDevGroupMem::HostToNet()
{
    dwItemSn = htonl(dwItemSn);
    dwGroupId = htonl(dwGroupId);
    dwUserId = htonl(dwUserId);
}

typedef struct tagDevNmcTopo : public TTnmDevTopo
{   
    tagDevNmcTopo() : tagTnmDevTopo()
    {
        tUnitType.byDeviceType = TNM_UNIT_UNKNOW;
        tUnitType.byServiceType = TNM_SERVICE_UNKNOW;
        dwDevSerialNo = INVALID_SERIAL_NO;
        dwMasterBoardSn = INVALID_SERIAL_NO;
        memset(achAlias, 0, sizeof(achAlias));
        dwIpAddr = 0;
        byField = INVALID_VALUE;
        byRightRange = RIGHT_RANGE_ALL_CHILD;
        byOnline = TNM_STATE_OFFLINE;
        byLayer = 0;
        bySlot = 0;
        byUpgradeMode = TNM_UPGRADE_MODE_NONE;
        byIsOldDevice = TNM_DISABLE;
        memset(achDevTypeName, 0, sizeof(achDevTypeName));
        memset(achBoxID, 0, sizeof(achBoxID));
    }
    TTnmUnitType tUnitType;                          // 单元类型
    u32 dwDevSerialNo;                      // 设备流水号.
    u32 dwMasterBoardSn;                    // 单板的主板设备流水号
    s8  achAlias[MAX_DEVICE_ALIAS_LEN + 4]; // 设备别名     
    u32 dwIpAddr;                           // 设备的Ip地址
    u8  byField;                            // byField代表的节点是否是域
    u8  byRightRange;                       // 权限的类型（此字段对单个的NMC客户端有意义）
                                                        // RIGHT_RANGE_ALL_CHILD
                                                        // RIGHT_RANGE_NO_CHILD
                                                        // RIGHT_RANGE_PART_CHILD
    u8  byOnline;                           // 当前设备状态，是否在线
    u8  byHighestAlarmLevel;                // 最高告警级别
    u8  bySelfAlarmLevel;                   // 本设备的最高告警级别

    u8  byLayer;
    u8  bySlot; 
    u8  byUpgradeMode;                                  //TNM_UPGRADE_MODE_TCP/TNM_UPGRADE_MODE_FTP

    u8  byIsOldDevice;                                  //TNM_ENABLE/TNM_DISABLE
    u8  byReserved;
    u16 wReserved;

    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //设备类型名
    s8  achBoxID[MAX_BOX_ID_LEN + 4];   //机框标识

    void  CopyDeviceInfo(const TDeviceInfo *ptDevInfo)
    {
        memcpy((TTnmDevTopo*)this, (TTnmDevTopo*)ptDevInfo, sizeof(TTnmDevTopo));
        strcpy(achAlias, ptDevInfo->achAlias);

        tUnitType.byDeviceType = ptDevInfo->tUnitType.byDeviceType;
        tUnitType.byServiceType = ptDevInfo->tUnitType.byServiceType;

        dwDevSerialNo = ptDevInfo->dwDevSerialNo;
        dwMasterBoardSn = ptDevInfo->dwMasterBoardSn;
        dwIpAddr = ptDevInfo->dwIpAddr;
        byField = ptDevInfo->byField;
        byRightRange = ptDevInfo->byRightRange;
        byOnline = ptDevInfo->byOnline;
        byHighestAlarmLevel = ptDevInfo->byHighestAlarmLevel;
        bySelfAlarmLevel = ptDevInfo->bySelfAlarmLevel;
        byLayer = ptDevInfo->byLayer;
        bySlot = ptDevInfo->bySlot;
        byUpgradeMode = ptDevInfo->byUpgradeMode;
        byIsOldDevice = ptDevInfo->byIsOldDevice;

        strcpy(achDevTypeName, ptDevInfo->achDevTypeName);
        strcpy(achBoxID, ptDevInfo->achBoxID);
    }
    void  GiveDeviceInfo(TDeviceInfo *ptDevInfo)
    {
        memcpy((TTnmDevTopo*)ptDevInfo, (TTnmDevTopo*)this, sizeof(TTnmDevTopo));
        strcpy(ptDevInfo->achAlias, achAlias);

        ptDevInfo->tUnitType.byDeviceType = tUnitType.byDeviceType;
        ptDevInfo->tUnitType.byServiceType = tUnitType.byServiceType;

        ptDevInfo->dwDevSerialNo = dwDevSerialNo;
        ptDevInfo->dwMasterBoardSn = dwMasterBoardSn;
        ptDevInfo->dwIpAddr = dwIpAddr;
        ptDevInfo->byField = byField;
        ptDevInfo->byRightRange = byRightRange;
        ptDevInfo->byOnline = byOnline;
        ptDevInfo->byHighestAlarmLevel = byHighestAlarmLevel;
        ptDevInfo->bySelfAlarmLevel = bySelfAlarmLevel;
        ptDevInfo->byLayer = byLayer;
        ptDevInfo->bySlot = bySlot;
        ptDevInfo->byUpgradeMode = byUpgradeMode;
        ptDevInfo->byIsOldDevice = byIsOldDevice;

        strcpy(ptDevInfo->achDevTypeName, achDevTypeName);
        strcpy(ptDevInfo->achBoxID, achBoxID);
    }

    void NetToHost()
    {
        TTnmDevTopo::NetToHost();

        dwDevSerialNo = ntohl(dwDevSerialNo);
        dwMasterBoardSn = ntohl(dwMasterBoardSn);
        dwIpAddr = ntohl(dwIpAddr);
    }
    void HostToNet()
    {
        TTnmDevTopo::HostToNet();

        dwDevSerialNo = htonl(dwDevSerialNo);
        dwMasterBoardSn = htonl(dwMasterBoardSn);
        dwIpAddr = htonl(dwIpAddr);
    }

}PACKED TDevNmcTopo;

#endif  //_EVNMSNMC_20050907_H_



