/*****************************************************************************
   模块名      : KDM
   文件名      : kdmconst.h
   相关文件    : 
   文件实现功能: KDM公用接口的常量定义头文件
   作者        : 万春雷
   版本        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人        修改内容
   2006/02/20  1.0         万春雷        创建
******************************************************************************/

#ifndef _CONST_KDM_H_
#define _CONST_KDM_H_

///////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "kdvtype.h"
//#include "kdmdef.h"


//1. 编号的相关定义

#define   LEN_KDM_NO                    (u8)32 //AAA中的各类对象的唯一编号的长度

//1.1 编号由三部分组成: CMS + NO类别码 + 剩余

#define   FIELD_LEN_CMS                 (u8)18 //CMS      编码段长度
#define   FIELD_LEN_NOTYPE              (u8)1  //NO类别码 编码段长度
#define   FIELD_LEN_ELSE                (u8)13 //剩余     编码段长度

//1.1.1 CMS编号由两部分组成:  [2*8位]域编码 + [2位]CMU平台序号
//      应用中域与平台一一对应，合在一起整体作为域或CMU平台编号

#define   MAXNUM_DOMAIN_LEVEL           (u8)8  //域最大层级
#define   FIELD_LEN_SINGLE_DOMAIN       (u8)2  //单一域最大长度
#define   FIELD_LEN_DOMAIN              (u8)(MAXNUM_DOMAIN_LEVEL*FIELD_LEN_SINGLE_DOMAIN)
#define   FIELD_LEN_CMU                 (u8)2  //CMU平台序号 编码段长度，2006-09-07改为作为行业序号

//1.1.2 NO类别码包括下面这样一些类型

//
#define   NOFIELD_TYPE_INVALID          (u8)0  //无效编号类型, 对应数字
#define   NOFIELD_CHAR_INVALID          (s8)'0'//无效编号类型, 对应字符
#define   NOFIELD_TYPE_PRECEDINGDEVICE  (u8)1  //前端设备编号类型
#define   NOFIELD_TYPE_PLATFORMDEVICE   (u8)2  //平台设备编号类型
#define   NOFIELD_TYPE_CUSTOMER         (u8)3  //客户编号类型
#define   NOFIELD_TYPE_CUSER            (u8)4  //客户用户编号类型
#define   NOFIELD_TYPE_DOMAINORCMU      (u8)5  //域或CMU平台编号类型
#define   NOFIELD_TYPE_TUSER            (u8)6  //电信用户编号类型
#define   NOFIELD_TYPE_CDGROUP          (u8)7  //客户设备组编号类型
#define   NOFIELD_TYPE_CUGROUP          (u8)8  //客户用户组编号类型

//1.1.3 剩余部分根据不同的NO类别码由各自的组合

//1.1.3.1 前端设备剩余部分编号: PDTYPE + PDSUBTYPE + PDSN, 末尾补零
#define   FIELD_LEN_PDTYPE              (u8)1  //前端设备类型   编码段长度, 具体类型参见4.2
#define   FIELD_LEN_PDSUBTYPE           (u8)1  //前端子设备类型 编码段长度, 具体类型参见4.2
#define   FIELD_LEN_PDSN                (u8)6  //前端设备序号   编码段长度

//1.1.3.2 平台设备剩余部分编号: CDTYPE + CDSN, 末尾补零
#define   FIELD_LEN_CDTYPE              (u8)1  //平台设备类型   编码段长度, 具体类型参见4.1
#define   FIELD_LEN_CDSN                (u8)3  //平台设备序号   编码段长度
// hanwang 2008-8-22 平台设备扩展类型
#define   FIELD_LEN_CDTYPE_EX           (u8)2  // 两位

//1.1.3.3 客户剩余部分编号: CUSTOMERSN, 末尾补零
#define   FIELD_LEN_CUSTOMERSN          (u8)5  //客户序号       编码段长度

//1.1.3.4 客户用户剩余部分编号: CUSTOMERSN + CUSTOMERUSERDSN, 末尾补零
#define   FIELD_LEN_CUSTOMERUSERSN      (u8)3  //客户用户序号   编码段长度

//1.1.3.5 域或CMU平台剩余部分编号: 全零

//1.1.3.6 电信用户剩余部分编号: TELUSERSN, 末尾补零
#define   FIELD_LEN_TELUSERSN           (u8)8  //电信用户序号   编码段长度

//1.1.3.7 客户设备组剩余部分编号: CUSTOMERSN + CDGROUPSN, 末尾补零
#define   FIELD_LEN_CDGROUPSN           (u8)5  //客户设备组序号 编码段长度

//1.1.3.8 客户用户组剩余部分编号: CUSTOMERSN + CUGROUPSN, 末尾补零
#define   FIELD_LEN_CUGROUPSN           (u8)3  //客户用户组序号 编码段长度


//1.2
#define   MAXNUM_SUBDOMAIN_ONEDOAMIN       (u32)99      //一个域下的子域最大数
#define   MAXNUM_SUBCMU_ONEDOAMIN          (u32)99      //一个域下的子CMU最大数
#define   MAXNUM_TUSER_ONEDOAMIN           (u32)99999999//一个域下的电信用户最大数
#define   MAXNUM_PREDEVICE_ONEDOAMIN       (u32)999999  //一个域下的直连前端设备最大数
#define   MAXNUM_PLATDEVICE_ONEDOAMIN      (u32)999     //一个域下的直连平台设备最大数
#define   MAXNUM_CUSTOMER_ONEDOAMIN        (u32)99999   //一个域下的客户最大数
#define   MAXNUM_CUSER_ONECUSTOMER         (u32)999     //一个客户下的客户用户最大数
#define   MAXNUM_CUGROUP_ONECUSTOMER       (u32)999     //一个客户下的客户用户组最大数
#define   MAXNUM_CDGROUP_ONECUSTOMER       (u32)99999   //一个客户下的客户设备组最大数


//2. 编号中的 域/用户/客户/设备名称、密码的相关定义

#define   MAXLEN_WHOLE_DOMAIN_NAME      (u16)64 //域全名的最大长度
#define   MAXLEN_SHORT_DOMAIN_NAME      (u16)8  //域简称的最大长度
#define   MAXLEN_CUSTOMER_NAME          (u16)128//客户名的最大长度(不含域名)
#define   MAXLEN_DEVICE_NAME            (u16)64 //设备名的最大长度(不含域名)
#define   MAXLEN_DGROUP_NAME            (u16)64 //设备组名的最大长度(不含域名)
#define   MAXLEN_TUSER_NAME             (u16)64 //电信用户名的最大长度(不含域名)
#define   MAXLEN_CUSER_NAME             (u16)64 //客户用户名的最大长度(不含域名、客户名)
#define   MAXLEN_CUGROUP_NAME           (u16)64 //客户用户组名的最大长度(不含域名、客户名)
#define   MAXLEN_APOLICY_NAME           (u16)64 //计费策略名的最大长度
#define   MAXLEN_HISTORYFILE_NAME       (u16)64 //自动维护形成的历史数据文件名的最大长度

//登录名：采用@分隔,域采用简称
#define   MAXLEN_DOMAIN_FULLPOSTFIX     (u16)(MAXLEN_SHORT_DOMAIN_NAME+1)*MAXNUM_DOMAIN_LEVEL//域后缀的最大长度(@域简称名List)
#define   MAXLEN_WHOLE_TUSER_NAME       (u16)160//电信用户的登陆用户名的最大长度(TUserName＋@域简称名)
#define   MAXLEN_WHOLE_CUSTOMER_NAME    (u16)208//客户的登陆用户名的最大长度(CustomerName＋@域简称名)
#define   MAXLEN_WHOLE_CUSER_NAME       (u16)256//偏小,客户用户的登陆用户名的最大长度(CUserName＋@客户名＋@域简称名)

#define   MAXLEN_PWD                    (u16)32 //电信/客户用户的登陆密码的最大长度

#define   MAXLEN_VERSION_STRING         (u16)32 //版本字串的最大长度

#define   SEPARATE_CHAR_LOGINNAME       (s8)'@'//登录名的分隔字符
#define   SEPARATE_CHAR_POSTFIX         (s8)'.'//登录后缀的分隔字符

//3. 编号中的 其余的基本信息的相关定义

#define   TUSER_TYPE_OPERATOR           (u8)0   //电信用户类型：操作员
#define   TUSER_TYPE_COMMONADMIN        (u8)1   //电信用户类型：管理员
#define   TUSER_TYPE_SUPERADMIN         (u8)2   //电信用户类型：超级管理员

#define   CUSER_TYPE_OPERATOR           (u8)0   //客户用户类型：操作员
#define   CUSER_TYPE_COMMONADMIN        (u8)1   //客户用户类型：管理员
#define   CUSER_TYPE_SUPERADMIN         (u8)2   //客户用户类型：超级用户

#define   AUDIOCALL_FLAG_DISABLE        (u8)0   //支持音频呼叫标志：禁用
#define   AUDIOCALL_FLAG_ENABLE         (u8)1   //支持音频呼叫标志：启用

#define   ALARMMANAGE_FLAG_DISABLE      (u8)0   //支持现场告警管理标志：禁用
#define   ALARMMANAGE_FLAG_ENABLE       (u8)1   //支持现场告警管理标志：启用

#define   PARAMCONFIG_FLAG_DISABLE      (u8)0   //支持参数设置标志：禁用
#define   PARAMCONFIG_FLAG_ENABLE       (u8)1   //支持参数设置标志：启用

#define   MAXNUM_CASTOMER_AUDIO_CALL    (u8)16  //客户的音频呼叫最大并发路数

#define   USER_SEX_MALE                 (u8)0   //用户性别：男
#define   USER_SEX_FEMALE               (u8)1   //用户性别：女

#define   USE_FLAG_DISABLE              (u8)0   //启停状态标志：禁用
#define   USE_FLAG_ENABLE               (u8)1   //启停状态标志：启用

#define   MLOGIN_FLAG_DISABLE           (u8)0   //多次登陆标志：不允许
#define   MLOGIN_FLAG_ENABLE            (u8)1   //多次登陆标志：允许

#define   FORCEUNIFORM_FLAG_DISABLE     (u8)0   //强制用户组所属用户权限与组一致标志：不强制
#define   FORCEUNIFORM_FLAG_ENABLE      (u8)1   //强制用户组所属用户权限与组一致标志：强制

//2006-03-27 WANCL ADD
#define   IP_NET_RANGE_NO_LIMIT         (u8)0   //登陆限制策略生效方式：不限制
#define   IP_NET_RANGE_REFUSED          (u8)1   //登陆限制策略生效方式：禁用网段
#define   IP_NET_RANGE_ALLOWED          (u8)2   //登陆限制策略生效方式：允许网段

#define   NET_TYPE_LAN                  (u8)0   //接入线路方式：LAN
#define   NET_TYPE_ADSL                 (u8)1   //接入线路方式：ADSL
#define   NET_TYPE_NAT                  (u8)2   //接入线路方式：NAT

#define   MAXLEN_COMPANY_NAME           (u16)128//工作单位的最大长度
#define   MAXLEN_DEPT_NUM               (u16)16 //工号的最大长度
#define   MAXLEN_TELEPHONE              (u16)32 //联系电话的最大长度
#define   MAXLEN_EMAIL                  (u16)32 //邮件的最大长度
#define   MAXLEN_DESCRIPTION            (u16)256//描述的最大长度
#define   MAXLEN_ADDRESS                (u16)128//住址的最大长度
#define   MAXLEN_POSTALCODE             (u16)32 //邮编的最大长度
#define   MAXLEN_REGION                 (u16)32 //区域的最大长度
#define   MAXLEN_LINKMAN                (u16)64 //联系人的最大长度
#define   MAXLEN_FAXCODE                (u16)32 //传真号码的最大长度
#define   MAXLEN_BANKNAME               (u16)128//开户银行的最大长度
#define   MAXLEN_BANKACCOUNT            (u16)64 //银行帐号的最大长度

#define   MAXLEN_DEVICE_STATE           (u16)32 //设备维护状态的最大长度(正常/报修/其他自定义。。)
#define   MAXLEN_DEVICE_NETADDRESS      (u16)32 //设备网络地址的最大长度(IP或域名)
#define   MAXLEN_DEVICE_FIXADDRESS      (u16)32 //设备安装位置的最大长度
#define   MAXLEN_DEVICE_MANUFACTURER    (u16)32 //制造商信息的最大长度
#define   MAXLEN_DEVICE_OWNER           (u16)32 //设备资产拥有者信息的最大长度
#define   MAXLEN_DEVICE_BUSINESSTYPE    (u16)16 //设备业务类别属性的最大长度
#define   MAXLEN_POWERSUPPLY_MODE       (u16)16 //供电方式
#define   MAXLEN_DEVICE_MODEINFO        (u16)16 //型号
#define   MAXLEN_DEVICE_DEFINITION      (u16)16 //清晰度
#define   MAXLEN_DEVICE_SORT            (u16)16 //类别
#define   MAXLEN_FENSE_GRADE            (u16)16 //防护等级
#define   MAXLEN_FIXFORMAT              (u16)32 //安装形式
#define   MAXLEN_DEVICE_LEVEL           (u16)16 //标高
#define   MAXLEN_LOG_CONTENT            (u16)256//日志描述的最大长度

#define   MAXLEN_POLICY_NAME            (u16)64 //计费策略名的最大长度
#define   MAXLEN_DEVICE_PASSWORD        (u16)64 //设备口令最大长度

//2006-04-15 WANCL ADD
#define   UNUSED_TIMELINE_NO_LIMIT      (u8)0   //使用截止时间标志：不限制

#define   QUERY_SUBDOMAIN_DISABLE       (u8)0   //查询时是否包含子域：不包含
#define   QUERY_SUBDOMAIN_ENABLE        (u8)1   //查询时是否包含子域：不过滤

//4. 编号中的 设备类型定义

#define   DEVICE_TYPE_INVALID           (u8)0   //无效设备, 对应数字

//4. 设备分为前端设备与平台设备两大类：

#define   DEVICE_TYPE_PRECEDING         NOFIELD_TYPE_PRECEDINGDEVICE  //(u8)1 前端设备
#define   DEVICE_TYPE_PLATFORM          NOFIELD_TYPE_PLATFORMDEVICE   //(u8)2 平台设备

//4.1. 平台设备分为下面几类：
#define   PLATFORM_DEVICE_TYPE_VTDU     (u8)1   //码流转发
#define   PLATFORM_DEVICE_TYPE_NRU      (u8)2   //网络录放像
#define   PLATFORM_DEVICE_TYPE_VPU      (u8)3   //画面复合 4??
#define   PLATFORM_DEVICE_TYPE_GAS      (u8)4   //GPS应用服务器
#define   PLATFORM_DEVICE_TYPE_UAS      (u8)5   //CMU的个性化数据服务器
#define   PLATFORM_DEVICE_TYPE_DNS      (u8)6   //DNS设备
//wdl
#define   PLATFORM_DEVICE_TYPE_PUI      (u8)7   //PU接入
#define   PLATFORM_DEVICE_TYPE_CUI      (u8)8   //CU接入
//
// 2009-5-19
#define   PLATFORM_DEVICE_TYPE_EX		(u8)9   //扩展设备类型
// 以下平台设备使用扩展编号。从11开始到99。
#define   PLATFORM_DEVICE_TYPE_EX_IMU   (u8)11	// IMU
#define   PLATFORM_DEVICE_TYPE_EX_IAS   (u8)12	// IAS
#define   PLATFORM_DEVICE_TYPE_EX_NPU   (u8)13  // NPU
#define   PLATFORM_DEVICE_TYPE_EX_TAS   (u8)14  // 3AS
#define   PLATFORM_DEVICE_TYPE_EX_MTU   (u8)15  // MTU


//4.2. 前端设备分为下面几类：main/periphery
#define   PRECEDING_DEVICE_TYPE_MAIN    (u8)1   //主设备：视频服务器等负责音视频编码并向网络传送的设备
#define   PRECEDING_DEVICE_TYPE_PER     (u8)2   //外围设备：主设备连接的摄像机、云台等设备

//4.2.1. 主设备分为下面几类：
#define   MAIN_DEVICE_TYPE_DVR          (u8)1   //DVR带存储的编码器（带本地存储的视频服务器）
#define   MAIN_DEVICE_TYPE_ENCODER      (u8)2   //DV 编码器（不带本地存储的视频服务器）
#define   MAIN_DEVICE_TYPE_IPCAMERA     (u8)3   //IP-Camera
#define   MAIN_DEVICE_TYPE_DECODER      (u8)4   //解码器

//4.2.2. 外围设备分为下面几类：
#define   PER_DEVICE_TYPE_VIDICON       (u8)1   //摄像机
#define   PER_DEVICE_TYPE_YT            (u8)2   //云台
#define   PER_DEVICE_TYPE_IOIN          (u8)3   //报警输入设备－开关量/模拟量，如红外、烟感、门磁等
#define   PER_DEVICE_TYPE_INOUT         (u8)4   //报警输出设备:如警灯、警铃
#define   PER_DEVICE_TYPE_AIN           (u8)5   //语音输入设备
#define   PER_DEVICE_TYPE_AOUT          (u8)6   //语音输出设备
#define   PER_DEVICE_TYPE_MATRIX        (u8)7   //矩阵

//5. 设备类型统一定义
//   (编号中的设备类型分三层，这里用一层统一定义，用于接口通讯交互，
//    3A中保障编号与该统一类型指代的一致性)

#define   DEVICE_GLOBAL_TYPE_INVALID    (u8)0   //无效设备

#define   DEVICE_GLOBAL_TYPE_CMU        (u8)1   //CMU

#define   DEVICE_GLOBAL_TYPE_VTDU       (u8)11  //码流转发
#define   DEVICE_GLOBAL_TYPE_NRU        (u8)12  //网络录放像
#define   DEVICE_GLOBAL_TYPE_VPU        (u8)13  //画面复合
#define   DEVICE_GLOBAL_TYPE_UAS        (u8)14  //CMU的个性化数据服务器
#define   DEVICE_GLOBAL_TYPE_PUI        (u8)15  //PU接入
#define   DEVICE_GLOBAL_TYPE_CUI        (u8)16  //CU接入
#define   DEVICE_GLOBAL_TYPE_NPU        (u8)17
#define	  DEVICE_GLOBAL_TYPE_IMU		(u8)18 
#define	  DEVICE_GLOBAL_TYPE_IAS		(u8)19
#define	  DEVICE_GLOBAL_TYPE_TAS		(u8)20  // 3AS

#define   DEVICE_GLOBAL_TYPE_DVR        (u8)21  //DVR带存储的编码器（带本地存储的视频服务器）
#define   DEVICE_GLOBAL_TYPE_ENCODER    (u8)22  //DV 编码器（不带本地存储的视频服务器）
#define   DEVICE_GLOBAL_TYPE_IPCAMERA   (u8)23  //IP-Camera
#define   DEVICE_GLOBAL_TYPE_DECODER    (u8)24  //解码器
#define   DEVICE_GLOBAL_TYPE_DNS        (u8)25  //DNS设备

#define	  DEVICE_GLOBAL_TYPE_MTU		(u8)26  // MTU // 平台设备

#define   DEVICE_GLOBAL_TYPE_VIDICON    (u8)31  //摄像机
#define   DEVICE_GLOBAL_TYPE_YT         (u8)32  //云台
#define   DEVICE_GLOBAL_TYPE_IOIN       (u8)33  //报警输入设备－开关量/模拟量，如红外、烟感、门磁等
#define   DEVICE_GLOBAL_TYPE_INOUT      (u8)34  //报警输出设备:如警灯、警铃
#define   DEVICE_GLOBAL_TYPE_AIN        (u8)35  //语音输入设备
#define   DEVICE_GLOBAL_TYPE_AOUT       (u8)36  //语音输出设备
#define   DEVICE_GLOBAL_TYPE_MATRIX     (u8)37  //矩阵

#define   DEVICE_GLOBAL_TYPE_GAS        (u8)41  //GAS


//6. 客户用户/用户组的系统权限定义 (Customer.User.System.Privilege)
enum EmCUserSPrivilege
{
	em_StartCUserSPrivilege     = 0, //..

	em_Cms_Vod_Rec              = 1, //平台录像 第一位
	em_Cms_Vod_Play             = 2, //平台放像
	em_Cms_Vod_Del              = 3, //平台录像删除
	em_Cms_Vod_Download         = 4, //平台录像下载
	em_Cms_Vod_Local_Rec        = 5, //本地录像，本地抓拍
	em_Cms_Rec_Query_Remote     = 6, //跨域录像查询

	em_Cms_Call_Ctrl            = 9, //呼叫控制
	em_Cms_Alarm_Manage         = 10,//现场告警管理

	//四个Bit表示用户的干线权限
	em_Cms_Route_Pri	    	= 16,   //用户的干线权限
	em_Cms_Route_Pri_bit2       = 17,   //干线权限Bit2
	em_Cms_Route_Pri_bit3       = 18,   //干线权限Bit3
	em_Cms_Route_Pri_bit4       = 19,   //干线权限Bit4

    em_Cms_Explore              = 20,   //视频浏览权限
    em_Cms_Cycle_Explore        = 21,   //轮巡权限

    cm_Cms_DevGroup_Inherit	= 27,	//开户时设备分组继承权限。20110624 huangzhichun

	//后三位表示Usbkey登陆认证方式
	em_Cms_Login_type			= 29, 
	em_Cms_Login_type_bit2		= 30,
	em_Cms_Login_type_bit3		= 31,//登录认证方式,从第30位开始到第32位,共3个bit位,记录5种登录状态

	em_EndCUserSPrivilege       = 33//..
};

//7. 设备的权限定义信息 (Device.Privilege)

//7.1. 前端设备的权限定义信息 (Preceding.Device.Privilege)

//7.1.1. 编码器
enum EmPUPrivilege
{
	em_StartPUPrivilege        = 0, //..
	
	em_PU_YJ_Ctrl              = 1, //云镜控制(1-10)级 第一位开始，共占用4位
    em_PU_Audio                = 8, //前端音频
	
	em_PU_Vod_Rec              = 9, //前端录像
	em_PU_Vod_Play             = 10,//前端放像
	em_PU_Vod_Del              = 11,//前端录像删除
	em_PU_Vod_Download         = 12,//前端录像下载

	em_PU_Param_Config         = 17,//前端参数设置
	em_PU_IO_Ctrl              = 18,//前端开关量输出控制
	em_PU_Defence_Ctrl         = 19,//布防/撤防
	em_PU_Alarm_Host		   = 20,//告警主机
	
	em_EndPUPrivilege          = 33//..
};

//7.2. 平台设备的权限定义信息 (Platform.Device.Privilege)

//7.2.1. VPU
enum EmVPUPrivilege
{
	em_StartVPUPrivilege        = 0, //..

	em_VPU_Explore              = 1,//VPU浏览
	em_VPU_Defence_Ctrl         = 2,//VPU控制

	em_EndVPUPrivilege          = 33//..
};


//8. 其他业务常量定义

#define   QUERY_RESULT_ALL_ENTRY        (u32)0 //查询请求的查询返回结果 为所有记录

#define   LASTPACK_IN_QRYNTF_FLAG       (u8)1  //查询结果中该包是否为结束包的标志

#define   MAXENTRYNUM_CMUQRYCUSTOMER_ONEPACK  (u32)36 //CMU查询客户开通信息时返回的单包最大记录数目
#define   MAXENTRYNUM_QRYDEVICE_ONEPACK       (u32)20 //查询设备列表信息时返回的单包最大记录数目
#define   MAXENTRYNUM_QRYCUSER_ONEPACK        (u32)12 //查询客户用户列表信息时返回的单包最大记录数目
#define   MAXENTRYNUM_QRYDGROUP_ONEPACK       (u32)28 //查询客户设备组列表信息时返回的单包最大记录数目
#define   MAXENTRYNUM_QRYDGROUP2DEV_ONEPACK   (u32)50 //查询客户设备组与设备关系列表信息时返回的单包最大记录数目
#define   MAXENTRYNUM_QRYUGROUP_ONEPACK       (u32)28 //查询客户用户组列表信息时返回的单包最大记录数目
#define   MAXENTRYNUM_QRYCUDPRIVILEGE_ONEPACK (u32)18//查询客户用户/用户组编号-设备编号-设备权限信息时返回的单包最大记录数目

#define   QUERY_ALL_DEVICE              (u8)0 //查询所有  设备记录(已分配+未分配)
#define   QUERY_ASSIGNED_DEVICE         (u8)1 //查询已分配设备记录(已分配)
#define   QUERY_UNASSIGNED_DEVICE       (u8)2 //查询未分配设备记录(零散)


//9. 制造商字符串定义
#define		MANU_STR_DC         (LPCSTR)"dacheng"
#define		MANU_STR_KDC        (LPCSTR)"kedacom"
#define		MANU_STR_HAIKANG    (LPCSTR)"haikang"
#define		MANU_STR_DH			(LPCSTR)"dahua"
#define		MANU_STR_VCOM		(LPCSTR)"vcom"
#define		MANU_STR_SANTACHI	(LPCSTR)"santachi"
#define		MANU_STR_CHUANGSHI  (LPCSTR)"chuangshi"  //创世
#define		MANU_STR_DALI       (LPCSTR)"dali"       //大力
#define		MANU_STR_ELSE       (LPCSTR)"other"
#define     MANU_STR_GBKDC      (LPCSTR)"GB_kedacom"
#define     MANU_STR_GBOTHER    (LPCSTR)"GB_other"

#define     MANU_STR_GB_PREFIX  (LPCSTR)"GB_"

//10.密钥字串定义(有效长度16字节,不得改动)
//#define     KDM_EDCRYPT_KEY     (LPCSTR)"**KDM3.9KDM3.9**"

//10.  用户认证信息定义
#define AUTH_DEFAULT_AES_KEY        (s8 *)"kedacom.shanghai"
//#define AUTH_DEFAULT_AES_MODE       MODE_CBC
#define AUTH_DEFAULT_AES_INITKEY    (s8 *)"2003091919190900"

//11. 区域ID无效值
#define REGION_ID_INVALITY         (u32)0    //区域ID无效值

//12. 设备型号字符串定义
#define	MODE_STR_ALARMHOST         (LPCSTR)"AlarmHost"

#endif /* _CONST_KDM_H_ */
//END OF FILE
