#ifndef _KEYDEF_H_
#define _KEYDEF_H_

/************************************************************************
 * redis数据库中用到的key相关的定义放这个头文件                 
 * 2016-03-17 zsy  创建
 * 2016-09-21 hzy  公布KEY字段接口
 * 2016-09-21 hzy  重新编排所有字段和精简注释
 ************************************************************************/

#include "cms/tas/tas_struct.h"
#include "cms/cmu/cmu_data.h"
#include "cms/tas/tas_proto.h"

/************************************************************************/
/*                 key定义                                          */
/************************************************************************/
#define MAKE_DOMAIN_KEY(uuidstring) ("domain:"+uuidstring) //域key生成宏
#define DOMAIN_KEY_LIST		"fastdb:domain_key_list"       //所有域key的集合key
#define MAKE_DEV_KEY(uuidstring) ("dev:"+uuidstring)       //设备key生成宏
#define DEVICE_KEY_LIST		"fastdb:device_key_list"       //所有设备key的集合key
#define MAKE_USER2DEV_KEY(uuidstring) ("user2dev:"+uuidstring)    //用户设备权限key生成宏
#define USERIDSET_KEY "useridset"                                 //用户id集合key
#define MAKE_USERGRP2DEV_KEY(uuidstring) ("ugrp2dev:"+uuidstring) //用户组设备权限key生成宏
#define UGRPIDSET_KEY "ugrpidset"                                 //用户组id集合key
#define SECUREMAIL_KEY  "securemail"                             //安全邮箱的key
#define MOBILE_CONNECT_NUM "onlineMobileNum"                //移动客户端接入的数量
#define MOBILE_CONNECT_TOTAL_NUM "maxMobileNum"     //移动客户端可以接入的总数
#define REDIS_TASDBINFO_KEY   "tasdbinfo"                               //3a数据库信息

/************************************************************************/
/*                 域关系字段定义                                        */
/************************************************************************/
#define ALL_CDomainRelationInfo				"VIRTUAL_CDomainRelationInfo"	//表示整个 CDomainRelationInfo 结构体的虚拟字段
#define CDomainRelationInfo_domainID		"dom"                           // 域编号
#define CDomainRelationInfo_parentDomainID	"parent"                        // 父域编号
#define CDomainRelationInfo_domainName		"name"                          // 域名称
#define CDomainRelationInfo_domainAlias		"alias"                         // 域别名


/************************************************************************/
/*                 设备字段定义                                          */
/************************************************************************/
// CDeviceInfo 
#define ALL_CDeviceInfo					"VIRTUAL_CDeviceInfo"  //表示全部的 CDeviceInfo 字段的虚拟字段
#define CDeviceInfo_deviceId			"devId"                // 设备UUIDString
#define CDeviceInfo_domainId			"dom"                  // 设备所属域编号
#define CDeviceInfo_name				"name"                 // 设备名
#define CDeviceInfo_alias				"alias"                // 设备别名
#define CDeviceInfo_type				"type"                 // 设备类型：参见常量定义
#define CDeviceInfo_modelId				"model"                // 设备型号
#define CDeviceInfo_manufacturer		"manu"                     //设备制造商, 与设备型号相关联，出于方便考虑，3AS根据设备型号动态拼接出来
#define CDeviceInfo_modelCapDetails		"VIRTUAL_modelCapDetails"  //这个字段只取redis中已有的几个字段
#define CDeviceInfo_usedFlag			"use"                  // 启停状态标志：0停用，非0启用
#define CDeviceInfo_netType				"net"                  // 接入线路方式：参见常量定义
#define CDeviceInfo_netAddress			"addr"                 // 设备网络地址，IP或域名	[---胡志云确认，CDeviceInfo结构体里的IP地址就是redis里addr这个字段表示入网IP地址---]
#define CDeviceInfo_expireDate			"expiredate"           // 使用截止日期										[---redis中没有这个字段---,后续胡志云又加入了 2016-3-31加入]
#define CDeviceInfo_creater				"creat"                // 创建者
#define CDeviceInfo_owner				"owner"                // 所属者
#define CDeviceInfo_fixaddr             "fixaddr"              // 安装位置
#define Redis_fixaddr			        CDeviceInfo_fixaddr    // 为了兼容保留此别名
#define CDeviceInfo_kdmno               "kdmno"
#define Redis_kdmno				        CDeviceInfo_kdmno      // 为了兼容保留此别名

// TPuStatus
#define ALL_TPuStatus			"VIRTUAL_TPuStatus"	 //表示所有TPuStatus字段的虚拟字段
#define TPuStatus_isUsed		CDeviceInfo_usedFlag //是否启用
#define TPuStatus_isOnline	    "online"	         //是否在线

// TPuServiceStatus
#define ALL_TPuServiceStatus					"VIRTUAL_TPuServiceStatus"	           //表示所有TPuServiceStatus字段的虚拟字段
#define ALL_TPuServiceStatus_audioCallSrc		"DYNAMIC_TPuServiceStatus_audioCallSrc"//表示所有TPuServiceStatus_audioCallSrc字段的虚拟字段,根据【音频解码通道数】生成
#define TPuServiceStatus_audioCallSrc(idx)		MakeField("chnaudcallstatus", idx)     //音频呼叫的源
#define ALL_TPuServiceStatus_isPuRec			"DYNAMIC_TPuServiceStatus_isPuRec"     //表示所有TPuServiceStatus_isPuRec字段的虚拟字段名,根据【编码通道数】生成
#define TPuServiceStatus_isPuRec(idx)			MakeField("chnpurecstatus", idx)       //是否在前端录像

// TPuBaseCapSet
#define ALL_TPuBaseCapSet				"VIRTUAL_TPuBaseCapSet"	 //表示所有 TPuBaseCapSet 字段的虚拟字段
#define TPuBaseCapSet_videoEncNum		"encode"                 //视频编码通道数量
#define TPuBaseCapSet_videoDecNum		"decode"                 //视频解码通道数量
#define TPuBaseCapSet_audioEncNum		"audencnum"              //音频编码通道数量
#define TPuBaseCapSet_audioDecNum		"auddecnum"              //音频解码通道数量
#define TPuBaseCapSet_serialPortNum		"serialport"             //串口数量
#define TPuBaseCapSet_inputPinNum		"inputpin"               //并口输入数量		[----是不是就是告警输入通道---]
#define TPuBaseCapSet_outputPinNum		"outputpin"              //并口输出数量
#define TPuBaseCapSet_videoInPortNum	"video"                  //视频输入端口数量	[---就是视频源数量--]
#define CDeviceInfo_capLabel            "caplabel"               //设备的能力标签

// TPuConfig
#define ALL_TPuConfig					"VIRTUAL_TPuConfig"	//表示所有TPuConfig字段的虚拟字段
#define TPuConfig_devIp					"connectip"         //前端设备IP				[---大师确认 TPuConfig里的IP就是这个connectip---]
#define TPuConfig_devType				"devtype"          //设备类型：编码器或解码器
#define TPuConfig_devMode				"devmode"          //设备型号，CU会使用它判断一些老前端的能力，平台不会使用
#define TPuConfig_devName				CDeviceInfo_name   //设备名称
#define TPuConfig_manufacturer			"manu"         //设备厂商				[---和CDeviceInfo里的是重复的，到时候，直接再复制一份给这里就好了---]
#define TPuConfig_extCapSet				"extcapset"    //扩展能力集				[---确定需要序列化---]
#define TPuConfig_inputPinEnable		"inputpinen"   //并口输入通道使能配置
#define TPuConfig_freeLicenceNum		"freelicense"  //免费的licence点数		[----是否就是这个---]
#define TPuConfig_directTransNum		"directtrans"  //设备支持的直传路数
#define TPuConfig_isDecCombined		    "deccombined"  //解码器是否合成码流
#define TPuConfig_isGBDevice			"isgb"         //是否是国标设备
#define TPuConfig_baseCapSet			ALL_TPuBaseCapSet//基本能力集			[---是一个TPuBaseCapSet结构体---]
#define ALL_TPuConfig_hdmiVidDecCfg		"DYNAMIC_hdmistyle"		//表示所有 TPuConfig_hdmiVidDecCfg字段的虚拟字段,根据【视频解码通道数】生成
#define TPuConfig_hdmiVidDecCfg(idx)	MakeField("hdmistyle", idx)//解码器hdmi输出风格 ，总数就是 Redis_hdmioutnum，idx代表hdmi输出口索引
#define ALL_TPuConfig_chnVidFormat		"DYNAMIC_TPuConfig_chnVidFormat"	//表示所有TPuConfig_chnVidFormat字段的虚拟字段名,根据【编码通道数】生成
#define TPuConfig_chnVidFormat(idx)		MakeField("chnvidformat", idx)      //通道当前的视频格式
#define ALL_TPuConfig_supportMediaType	"DYNAMIC_TPuConfig_supportMediaType"   //表示所有TPuConfig_supportMediaType字段的虚拟字段,根据【编码通道数】生成
#define TPuConfig_supportMediaType(idx)	 MakeField("chnsupportmediatype", idx) //通道当前支持的所有媒体格式(只读)
#define ALL_TPuConfig_chnVidEncParam	"DYNAMIC_TPuConfig_chnVidEncParam"	// 表示所有TPuConfig_chnVidEncParam字段的虚拟字段,根据【编码通道数】生成
#define TPuConfig_chnVidEncParam(idx)	MakeField("chnvidencparam", idx)    //通道当前的视频编码参数	[---已确定每一项是一个序列化---]
#define ALL_TPuConfig_chnAudEncParam	"DYNAMIC_TPuConfig_chnAudEncParam"	// 表示所有TPuConfig_chnAudEncParam字段的虚拟字段,根据【编码通道数】生成
#define TPuConfig_chnAudEncParam(idx)	MakeField("chnaudencparam", idx)    //通道当前的音频编码参数 [---已确定每一项是一个序列化---]
#define ALL_TPuConfig_chnVidSrcSet		"DYNAMIC_TPuConfig_chnVidSrcSet"// 表示所有TPuConfig_chnVidSrcSet字段的虚拟字段,根据【编码通道数】生成
#define TPuConfig_chnVidSrcSet(idx)		MakeField("chnvid", idx)        //通道可用的视频源列表							[---已确定每一项是一个序列化---]
#define ALL_TPuConfig_chnStatusSet		"DYNAMIC_TPuConfig_chnStatusSet"// 表示所有TPuConfig_chnStatusSet字段的虚拟字段,根据【编码通道数】生成
#define TPuConfig_chnStatusSet(idx)		MakeField("chnstatus", idx)     //通道状态集 add by ywy for  channel state report
#define TPuConfig_chnStatusSet_Q(idx, buf)		MakeFieldQ("chnstatus", idx, buf)     //通道状态集 add by ywy for  channel state report
#define ALL_TPuConfig_vidSrcName		"DYNAMIC_TPuConfig_vidSrcName" // 表示所有TPuConfig_vidSrcName字段的虚拟字段,根据【视频源数】生成
#define TPuConfig_vidSrcName(idx)		MakeField("vidname", idx)      // 视频源名称，pui收集到pu上报的名称 PuConfig结构体中使用的是此字段
#define ALL_TPuConfig_vidSrcAlias		"DYNAMIC_TPuConfig_vidSrcAlias"// 表示所有TPuConfig_vidSrcAlias字段的虚拟字段,根据【视频源数】生成
#define TPuConfig_vidSrcAlias(idx)      MakeField("vidalias", idx)     // 视频源别名，平台生效的视频源别名           
#define ALL_TPuConfig_vidSrcChnSet		"DYNAMIC_TPuConfig_vidSrcChnSet"  // 表示所有TPuConfig_vidSrcChnSet字段的虚拟字段,根据【视频源数】生成
#define TPuConfig_vidSrcChnSet(idx)		MakeField("vidchnset", idx)   // 视频源当前正在使用的通道列表		[---已确定每一项是一个序列化---]
#define ALL_TPuConfig_camType			"DYNAMIC_TPuConfig_camType"   // 表示所有TPuConfig_camType字段的虚拟字段,根据【视频源数】生成
#define TPuConfig_camType(idx)			MakeField("camtype", idx)     //视频源的摄像头类型
#define ALL_TPuConfig_camId				"DYNAMIC_TPuConfig_camId"     // 表示所有TPuConfig_camId字段的虚拟字段,根据【视频源数】生成
#define TPuConfig_camId(idx)			MakeField("camid", idx)       //视频源的摄像头地址码
#define ALL_TPuConfig_azimuth			"DYNAMIC_TPuConfig_azimuth"   // 表示所有TPuConfig_azimuth字段的虚拟字段,根据【视频源数】生成
#define TPuConfig_azimuth(idx)          MakeField("azimuth", idx)     //视频源的方位角数据
#define TPuConfig_puiId                 "puiid"                       //接入pui的uuid


// CRedisDevData
#define ALL_CRedisDevData							"VIRTUAL_CRedisDevData"	       //表示整个CRedisDevData结构体的虚拟字段
#define CRedisDevData_m_tGpsData					"gpsdata"                 	   // pu gps数据
#define CRedisDevData_m_tTransparentData(type)		MakeField("chntransdata", type)// pu 透明数据，目前实际上只有一个字段，未来也不太可能改变，所以这个目前是当成固定字段来用的	[---已确定每一项是一个序列化---]
#define CRedisDevData_m_tPuOnOff					ALL_TPuStatus		// pu 在线状态		[---是一个 TPuStatus 结构体--]
#define CRedisDevData_m_tPuConfig					ALL_TPuConfig	    // pu 配置信息		[---是一个TPuConfig结构体---]
#define CRedisDevData_m_tPuServiceStatus			ALL_TPuServiceStatus// pu 业务状态		[---是一个TPuServiceStatus结构体---]
#define ALL_CRedisDevData_m_tPuAlarmCollector			"DYNAMIC_CRedisDevData_m_tPuAlarmCollector"	//表示所有的 CRedisDevData_m_tPuAlarmCollector 字段
#define CRedisDevData_m_tPuAlarmCollector(idx, type)	MakeField("chnalarm", idx, type)	        // pu告警信息	[---已确定每一项是一个序列化---]
#define CHNS_WITH_ALARM_TYPE(type)	MakeField("fastdb:alarm_type", type) // 告警type的通道号列表（value是一个set<int>），是fastdb的管理字段
// CRedisPuData 继承自CRedisDevData
#define ALL_CRedisPuData							"VIRTUAL_CRedisPuData"	//表示整个CRedisPuData结构体的虚拟字段
#define CRedisPuData_m_tDevInfo						ALL_CDeviceInfo		    //从3AS获取的设备信息			[---实际是一个CDeviceInfo结构---]

// 其他字段 ，结构体里没有，但redis中有的字段
#define Redis_LicenceNum		"license"	        //这个是CDevice里的，但是是结构体成员里的某个值，所以没有成员名
#define Redis_localip			"localip"	        //本地ip
#define Redis_devmode			"devmode"	        //-- CU会使用它判断一些老前端
#define Redis_alarm				"alarm"	
#define Redis_localstore		"store"		        //是否带有本地存储
#define Redis_hdmioutnum		"hdmioutput"	    //这个是hdmi输出数
#define ALL_vidgps				"DYNAMIC_vidgps"	// 表示所有 Redis_vidgps字段的虚拟字段,根据【视频源数】生成
#define Redis_vidgps(idx)		MakeField("vidgps", idx) // --存储视频源经纬度，lat, lon结构体的序列化  vidgps : 1等
#define Redis_gbid              "gbid"                   // 设备的gbid
#define ALL_encgbid             "DYNAMIC_encgbid"         // 表示所有 Redis_encgbid字段的虚拟字段，根据【编码通道数】生成
#define Redis_encgbid(idx)      MakeField("encgbid", idx) // 存储编码通道的国标编号
#define Redis_puiip             "puiip"                  // 接入的pui ip
#define Redis_chnplatrecstatus(idx)		MakeField("chnplatrecstatus", idx)   //是否在平台录像
#define Redis_GB_Security       "pms_gb_security_enable" //平台是否启用了安全模式

#define REDIS_TASDBINFO_IP "ip"
#define REDIS_TASDBINFO_PORT "port"
#define REDIS_TASDBINFO_PASSWORD "password"
static const size_t DevKeyLen = 36;		// 设备Key的大小.
static const size_t MaxFieldLen = 64;	// 最大的字段长度.
static const size_t MaxCmdSize = 2048;	// 最大的Redis命令大小.
static const size_t MaxCmdSize2 = (1024 * 64);	// 最大的Redis命令大小（确保可以存储一条puconfig，这种字符串比xml序列化冗余数据要少一些，应该够了）


/**	高性能的设备Key生成函数.
*	说明:
*	devId 必须为32字节长度.
*	传入buf 不为0时 内存大小要 >= 37字节. 函数支持重入/多线程.
*	传入buf为0时返回内部静态内存, 函数不支持重入/多线程. 这种模式性能更好, 推荐单线程程序使用.
*	成功返回buf, 失败返回0
*	+by lzx@2017-03-02
*/
char* MakeDevKey(const char* devId, char* buf = 0);

//字段名拼接函数
std::string MakeField(const std::string &strBaseName, int nIdx);

/** 高性能的字段拼接函数. 
*	传入buf为0时返回内部静态内存, 函数不支持重入/多线程 
*	成功返回buf, 失败返回0
*	+by lzx@2017-03-02
*/
char* MakeFieldQ(const char* strBaseName, int nIdx, char* buf=0);	
std::string MakeField(const std::string &strBaseName, int nIdx, const std::string &strType);
std::string MakeField(const std::string &strBaseName, const std::string &strType);
//从 CHNS_WITH_ALARM_TYPE(type)生成的字段名利提取出告警名部分
std::string GetAlarmTypeByField(const std::string &strFieldWithType);
/***********************************************************************
* 以下函数只返回结构体本级可直接在redis中表示的字段
* 不包括父类的所有字段
* 不包括成员本身也是本系列函数可以获得的结构体
* 不包括成员是可变的动态字段
************************************************************************/
//结构体 TPuStatus 的所有字段名
int Get_TPuStatus_FieldNames(std::vector<std::string> &fields);
//结构体 TPuConfig 的所有字段名
int Get_TPuConfig_FieldNames(std::vector<std::string> &fields, bool bRead);
//结构体 TPuBaseCapSet 的所有字段名
int Get_TPuBaseCapSet_FieldNames(std::vector<std::string> &fields);
//结构体 TPuServiceStatus 的所有字段名
int Get_TPuServiceStatus_FieldNames(std::vector<std::string> &fields);
//结构体 CDeviceInfo 的所有字段名
int Get_CDeviceInfo_FieldNames(std::vector<std::string> &fields);
//结构体 CRedisDevData 的所有字段名
int Get_CRedisDevData_FieldNames(std::vector<std::string> &fields);
//结构体 CRedisPuData 的所有字段名
int Get_CRedisPuData_FieldNames(std::vector<std::string> &fields);
//结构体 CDomainRelationInfo 的所有字段名
int Get_CDomainRelationInfo_FieldNames(std::vector<std::string> &fields);
//获取所有告警类型通道集合的字段名
int Get_All_AlaramChannelSet_FieldNames(std::vector<std::string> &fields);
/***********************************************************************
* 以下的函数提供具体的redis字段到结构体成员的互换
* 把values里有的字段全部匹配到data中对应成员里去
* 返回值，如果是False就只表示双方的字段无法对应
************************************************************************/
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT TPuStatus &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT TPuConfig &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT TPuBaseCapSet &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT TPuServiceStatus &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT CDeviceInfo &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT CRedisDevData &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT CRedisPuData &data);
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT CDomainRelationInfo &data);
//values是既包含了字段CHNS_WITH_ALARM_TYPE()也包含了字段CRedisDevData_m_tPuAlarmCollector()的一个map
bool Redis_To_Struct(IN const std::map<std::string, std::string> &values, OUT std::map<std::string, CAlarmInputList> &data);

bool Struct_To_Redis(IN const TPuStatus &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const TPuConfig &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const TPuBaseCapSet &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const TPuServiceStatus &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const CDeviceInfo &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const CRedisDevData &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const CRedisPuData &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const CDomainRelationInfo &data, IN const std::string &field, OUT std::string &value);
bool Struct_To_Redis(IN const std::map<std::string, CAlarmInputList> &data, IN const std::string &field, OUT std::string &value);

#endif // _KEYDEF_H_
