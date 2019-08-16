/*****************************************************************************
   模块名      : CMSSDK
   文件名      : cmssdk_errorcode.h
   相关文件    : 
   文件实现功能: cmssdk错误码定义
   作者        : fanxg
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/09/12  1.0         fanxg        创建
******************************************************************************/

#ifndef __CMS_SDK_ERRORCODE_H__
#define __CMS_SDK_ERRORCODE_H__

//错误码定义
enum CMS_SDK_ERRCODE
{
	CMS_SDK_SUCCESS = 0,                       //操作成功,成功不需要针对每个操作都定义，所有操作共享

	//公共的一些错误码
	CMS_SDK_COMMON_INVALID_DEV_URI = 1,         //无效的dev uri
	CMS_SDK_COMMON_DEV_ALREADY_LOGIN= 2,        //相同uri的dev已经登录
	CMS_SDK_SIP_BODY_NULL = 3,				    //SIP消息体为空
	CMS_SDK_POST_MSG_FAILED = 4,                //发送消息失败
	CMS_SDK_INVALIAD_REQ_PARAM = 5,             //无效的请求参数
	CMS_SDK_INVALIAD_SDP = 6,                   //无效的媒体描述信息
	CMS_SDK_DIALOG_TERMINATE = 7,               //对话被中断
	CMS_SDK_MKDIR_FAILED = 8,                   //创建目录失败
	CMS_SDK_INVALID_SESSIONID = 9,              //无效的会话ID，指定的会话不存在
	CMS_SDK_REQUIRE_TOO_MUCH = 10,              //同类请求过多，超过业务限额
	CMS_SDK_OLD_PASSWORD_INCORRECT = 11,        //旧密码错误

	//公共的sip层错误码：4xx-6xx，这个区段采用sip标准定义，不能被别的模块占用。
	//这些错误码全局共享，各模块无需再次定义
	//常用的sip错误码定义如下，后续可以增加
	// 4xx Client Error
	CMS_SDK_SIP_403_FORBIDDEN        = 403,  // 403 禁止访问
	CMS_SDK_SIP_404_NOT_FOUND        = 404,  // 404 找不到目标
	CMS_SDK_SIP_408_REQ_TIMEOUT      = 408,  // 408 请求超时
	CMS_SDK_SIP_423_INTERV_TOO_BRIEF = 423,  // 423 请求过于频繁
	CMS_SDK_SIP_486_BUSY_HERE        = 486,  // 486 Busy Here    
	// 5xx Server Error
	// 6xx Global Error

	//3as错误码 10000-10999(当前可用号为10040)
	CMS_SDK_3AS_BEGIN = 10000,                  //3as错误码起始值

	CMS_SDK_TAS_INVALID_PARM = 10001,        // 参数非法, 详细错误参见后台打印，此类错误主要用于程序员
	CMS_SDK_TAS_HANDLE_IN_PROGRESS = 10002,  // 表示一个操作正在处理过程中，用户无需重复提交请求
	CMS_SDK_TAS_INVALID_OPERATION = 10003,   // 非法操作，业务上不允许如此操作
	CMS_SDK_TAS_SYSTEM_BUSY = 10024,         // 系统繁忙，请稍后再试
	CMS_SDK_TAS_ULTRA_VIRES = 10028,         // 越权行为
	CMS_SDK_TAS_EXIST_SAMENAME_RECORD = 10029,  // 存在同名记录
	CMS_SDK_TAS_FIELD_CONTENT_EMPTY = 10031,    // 字段内容为空
	CMS_SDK_TAS_CURRENT_USER_NOT_EXIST = 10030, // 当前操作者不存在，会话不存在或者用户不存在

	CMS_SDK_TAS_SYSTEM_UNINITED = 10004,   // 系统未初始化
	CMS_SDK_TAS_DBTASK_POOL_FULL = 10005,  // DB任务池满
	CMS_SDK_TAS_DBERROR = 10006,           // 数据库操作出错，请重试一次
	CMS_SDK_TAS_RECORD_EXISTS = 10007,     // 数据库记录已经存在
	CMS_SDK_TAS_RECORD_NOT_EXISTS = 10008, // 数据库记录不存在
	CMS_SDK_TAS_RECORD_FETCH_FAILED = 10009, // 数据库记录获取失败, 可能是记录已经不存在了

	CMS_SDK_TAS_LOGINED = 10010,           // 相同标识的客户端已经登录TAS
	CMS_SDK_TAS_USER_NOT_EXIST = 10011,    // 用户不存在
	CMS_SDK_TAS_INVALID_PASSWORD = 10012,  // 密码错误
	CMS_SDK_TAS_USER_EXPIRED = 10025,      // 用户过期
	CMS_SDK_TAS_USER_DISABLE_MULTILOGIN = 10026,  // 禁止多次登录
	CMS_SDK_TAS_USER_TYPE_DISABLE_CHANGE = 10027, // 用户类型不允许修改, 比如存在儿子用户的用户被不能改为操作员

	CMS_SDK_TAS_DECODER_ALREADY_BINDED = 10033,   // 解码器已经被绑定

	CMS_SDK_TAS_LICENSE_KEY_ERROR = 10013,  // license key出错(license未正常连接或者license内容不正确)
	CMS_SDK_TAS_LICENSE_KEY_EXPIRE = 10014, // license key到期
	CMS_SDK_TAS_LICENSE_NOT_ENOUGH = 10015, // license不足
	CMS_SDK_TAS_LICENSE_KEY_INVALID_LICENSE_FILE = 10034,  // 无效的license文件(比如机器码不匹配或内容损坏)
	CMS_SDK_TAS_LICENSE_SOME_OTHER_PU_TOO_MANY = 10016,    // 某外厂商PU接入太多
	CMS_SDK_TAS_LICENSE_OTHER_PU_TOTAL_TOO_MANY = 10017,   // 外厂商PU接入总数太多
	CMS_SDK_TAS_LICENSE_NO_DIRECT_ACCESS_OTHER_PU = 10018, // 不允许直接接入外厂商PU
	CMS_SDK_TAS_LICENSE_OTHER_MORE_THAN_ALL = 10037,	   // 外厂商pu最大接入数大于最大监控点数

	CMS_SDK_TAS_INVALID_EXPIRE_DATE = 10019,   // 过期日期错误
	CMS_SDK_TAS_INVALID_DOMAINID = 10020,      // 域号错误
	CMS_SDK_TAS_INVALID_DEVICEMODELID = 10021, // 设备型号错误
	CMS_SDK_TAS_SAME_DEVICEMODEL_INFO = 10038,  // 相同的设备型号信息
	CMS_SDK_TAS_INVALID_DEVICETYPEID = 10022,  // 设备类型错误
	CMS_SDK_TAS_INVALID_DEVICEID = 10023,      // 设备号错误

	CMS_SDK_TAS_INVALID_SYNCDATA = 10032,   // 无效的同步数据，比如使用时同步数据标识对应的数据内容已经不存在了
	CMS_SDK_TAS_DOMAIN_RELATION_VALIDATE_FAILED = 10035, // 域关系校验失败

	CMS_SDK_TAS_DISABLED_ACCOUNT = 10036,    // 未启用的用户
	CMS_SDK_TAS_EXTDATA_TOO_LONG = 10039,	 // 信息超长

	CMS_SDK_3AS_END = CMS_SDK_3AS_BEGIN + 999,      //3as错误码结束值

	//cmu错误码 11000-11999
	CMS_SDK_CMU_BEGIN = 11000,                  //cmu错误码起始值

	CMS_SDK_CMU_NOT_IN_SERVICE = 11001,         //平台服务器未进入服务态
	CMS_SDK_CMU_SIP_BODY_NULL = 11002,          //SIP消息体为空
	CMS_SDK_CMU_PUI_ALREADY_REG = 11003,        //相同的PUI已经注册
	CMS_SDK_CMU_CUI_ALREADY_REG = 11004,        //相同的CUI已经注册
	CMS_SDK_CMU_PU_NOT_IN_TOP = 11005,          //pu未入网
	CMS_SDK_CMU_PU_NOT_USED = 11006,            //pu未启用
	CMS_SDK_CMU_PU_EXPIRE = 11007,              //pu已经过期
	CMS_SDK_CMU_PU_ALREADY_REG = 11008,         //相同的PU已经注册
	CMS_SDK_CMU_PU_LICENSE_NOT_MATCH = 11009,   //PU的license不匹配
	CMS_SDK_CMU_CU_LOGIN_SESS_ILLEGAL = 11010,  //CU操作中登录Session非法
	CMS_SDK_CMU_SS_OPE_TYPE_INVALID = 11011,    //CU更新订阅的操作类型无效
	CMS_SDK_CMU_CUI_NOT_REG = 11012,            //CUI未向CMU注册
	CMS_SDK_CMU_PUI_NOT_REG = 11013,            //PUI未向CMU注册
	CMS_SDK_CMU_VTDU_ALREADY_REG = 11014,       //相同的VTDU已经注册
	CMS_SDK_CMU_DEV_NO_ROUTER = 11015,          //设备无法路由
	CMS_SDK_CMU_DEV_RSP_TIMEOUT = 11016,        //设备应答超时
	CMS_SDK_CMU_CMU_ALREADY_CONN = 11017,       //相同的CMU已经连接
	CMS_SDK_CMU_CMU_NOT_CONN = 11018,           //CMU未连接
	CMS_SDK_CMU_CMU_INVALID_CMU_URI = 11019,    //无效CMU URI
	CMS_SDK_CMU_PU_NOT_ONLINE = 11020,          //PU不在线
	CMS_SDK_CMU_USER_PTZ_PRI_LOW = 11021,       //用户PTZ控制权限低
	CMS_SDK_CMU_NOT_SUPPORT = 11022,            //该功能不支持
	CMS_SDK_CMU_ALARM_ILLEGAL = 11023,          //不合理的告警
	CMS_SDK_CMU_STOP_BY_BYE = 11024,            //对话被挂断
	CMS_SDK_CMU_DOMAIN_NOT_IN_TOP = 11025,      //Domian未组网
	CMS_SDK_CMU_DEV_ADDR_INVALID = 11026,       //设备地址无效

	//{{SWITCH=11500
	CMS_SDK_CMU_SWITCH_DEV_NOT_AVAILABLE = 11500,       //参与交换的设备不在线
	//CMS_SDK_CMU_SWITCH_DST_NOT_AVAILABLE = 11501,       //交换目的不在线
	//CMS_SDK_CMU_SWITCH_SRC_NOT_AVAILABLE = 11502,       //交换源不在线
	CMS_SDK_CMU_SWITCH_DST_EXIST_SWITCH= 11503,         //交换目的存在交换
	CMS_SDK_CMU_SWITCH_SRC_EXIST_SWITCH= 11504,         //交换源存在交换
	//CMS_SDK_CMU_SWITCH_POST_MSG_TO_SRC_FAILED= 11505,   //交换时发送消息给源失败
	//CMS_SDK_CMU_SWITCH_POST_MSG_TO_VTDU_FAILED= 11506,  //交换时发送消息给目的失败
	//CMS_SDK_CMU_SWITCH_POST_MSG_TO_DST_FAILED= 11507,   //交换时发送消息给VTDU失败
	CMS_SDK_CMU_SWITCH_SRC_NODE_NOT_EXIST = 11508,      //交换源节点不存在
	CMS_SDK_CMU_SWITCH_DST_NODE_NOT_EXIST = 11509,      //交换目的节点不存在
	CMS_SDK_CMU_SWITCH_INVALID_DIALOG_ID = 11510,       //无效的交换会话ID,会话还未建立或已经销毁
	CMS_SDK_CMU_SWITCH_ADDRESS_RULE_SWITCH_FAILED = 11511,      //交换时转为地址规则失败
	CMS_SDK_CMU_SWITCH_VTDU_NOT_AVAILABLE = 11512,      //找不到可用的VTDU
	CMS_SDK_CMU_SWITCH_GAP_VTDU_NOT_AVAILABLE = 11513,  //找不到可用的网闸VTDU
	CMS_SDK_CMU_SWITCH_VTDU_PORT_ALLOC_FAILED = 11514,  //VTDU通道或端口分配失败
	CMS_SDK_CMU_SWITCH_EXCEPTION = 11515,	            //交换时发生诊断性异常
	CMS_SDK_CMU_SWITCH_ADD_BAK_SRC_FAILED = 11516,	    //交换时添加新的备份源交换失败
	CMS_SDK_CMU_SWITCH_STOP_SWITCH_FOR_NEW_BAK_SRC_FAILED = 11517,	    //交换时为构建新的备份源停止原有交换失败
	CMS_SDK_CMU_SWITCH_AGAIN = 11518,                   //资源临时不可得（如VTDU下线等），请重试
	CMS_SDK_CMU_SWITCH_TASK_TIMEOUT = 11519,            //CMU交换任务超时
	CMS_SDK_CMU_SWITCH_STOP_BY_BYE = 11520,             //交换对话被挂断
	CMS_SDK_CMU_SWITCH_VCR_FAILED = 11521,              //VCR控制失败
	//}}SWITCH


	CMS_SDK_CMU_END = CMS_SDK_CMU_BEGIN + 999,      //cmu错误码结束值

	//pui错误码 12000-12999
	CMS_SDK_PUI_BEGIN = 12000,                  //pui错误码起始值

	CMS_SDK_PUI_PU_ALREAD_EXIST = 12001,        //相同的PU已经注册
	CMS_SDK_PUI_PU_OVERLORD = 12002,            //pui接入pu数过载
	CMS_SDK_PUI_SIP_BODY_NULL = 12003,          //SIP消息体为空
	CMS_SDK_PUI_NOT_IN_SERVICE = 12004,         //PUI未进入服务态
	CMS_SDK_PUI_ADD_SWITCH_FAIL = 12005,        //PUI添加交换失败
	CMS_SDK_PUI_GET_CAM_PROTO_FAIL = 12006,     //PUI获取摄像头控制协议失败
	CMS_SDK_PUI_GET_PU_CONFIG_FAIL = 12007,     //PUI获取前端配置失败
	CMS_SDK_PUI_SET_PU_CONFIG_FAIL = 12008,     //PUI设置前端配置失败
	CMS_SDK_PUI_PU_NOT_ONLINE = 12009,          //PU不在线
	CMS_SDK_PUI_PU_OPE_TIMEOUT = 12010,         //PU操作超时
	CMS_SDK_PUI_PU_OPE_FAIL = 12011,            //PU操作失败
	CMS_SDK_PUI_PU_TYPE_ILLEGAL = 12012,        //PU类型非法
	CMS_SDK_PUI_PU_NODISK = 12013,              //前端无硬盘
	CMS_SDK_PUI_PU_RECORDING = 12014,           //正在录像或预录
	CMS_SDK_PUI_PU_RECPLAYING = 12015,          //正在放像
	CMS_SDK_PUI_PU_FILE_NOT_EXIT = 12016,		//文件不存在(该路径下无此文件)
	CMS_SDK_PUI_PU_RECLENGTH_ZERO = 12017,		//文件大小为零(录象时无码流)
	CMS_SDK_PUI_PU_RECLOG_NOT_EXIST = 12018,    //reclog文件不存在
	CMS_SDK_PUI_PU_ASF_STATE_ERROR = 12019,     //播放ASF文件状态不正确
	CMS_SDK_PUI_PU_PATH_CREATE_FAIL = 12020,    //目录创建失败,请检查是否磁盘只读
	CMS_SDK_PUI_PU_NO_PIC_FILE = 12021,         //无前端抓拍图片
	CMS_SDK_PUI_PU_PIC_FILE_TOO_MUCH = 12022,   //前端抓拍图片过多

	CMS_SDK_PUI_NVR_NO_BUF_RECORD = 12023,                      //录像缓存记录不存在,请重新搜索录像(NVR前端放像时)
	CMS_SDK_PUI_NVR_INVALID_PU_RECCHN = 12024,                  //录像通道不可用，一般是此NVR通道上未接入设备(NVR前端录像时)
	CMS_SDK_PUI_NVR_NOT_ALLOW_DELETE_PU_REC = 12025,            //无删除NVR前端录像权限
	CMS_SDK_PUI_NVR_BITRATE_OVERFLOW = 12026,                   //比特率过大(NVR2820 BitRate:64 - 2048)
	CMS_SDK_PUI_NVR_PIC_TOO_MUCH = 12027,                       //NVR前端抓拍图片数过多，请缩小搜索范围
	CMS_SDK_PUI_NVR_NO_SNAPSHOT_PIC = 12028,                    //无NVR前端抓拍图片
	CMS_SDK_PUI_NVR_NO_VALID_REC_DISK = 12029,                  //NVR无可用录像磁盘分区
	CMS_SDK_PUI_PU_NOT_SUPPORT = 12030,                         //该前端不支持此功能

	CMS_SDK_PUI_PU_VCR_NOT_SUPPORT_PLAY_RATE = 12031,           //前端不支持的VCR播放速率
	CMS_SDK_PUI_PU_VCR_NOT_SUPPORT_CMD = 12032,                 //前端不支持的VCR控制命令

	CMS_SDK_PUI_QUERY_MAPPED_ADDR_FAILED = 12033,               //查询外网地址失败
	CMS_SDK_PUI_QUERY_MAPPED_ADDR_TIMEOUT = 12034,              //查询外网地址超时
	CMS_SDK_PUI_QUERY_MAPPED_ADDR_CANCELED = 12035,             //查询外网地址操作被取消

	CMS_SDK_PUI_PUNCH_FAILED = 12036,                           //地址探测失败
	CMS_SDK_PUI_PUNCH_TIMEOUT = 12037,                          //地址探测超时
	CMS_SDK_PUI_PUNCH_CANCELED = 12038,                         //地址探测操作被取消
	CMS_SDK_PUI_EXCEED_PLAY_REC_MAX_NUM = 12039,                //超过前端最大放像数目
	CMS_SDK_PUI_GET_LOCAL_ADDR_FAILED = 12040,                  //获取本地地址失败
	CMS_SDK_PUI_MEDIA_TYPE_ERROR = 12041,                       //错误的媒体格式
	CMS_SDK_PUI_PU_VCR_TRY_AGAIN = 12042,                       //正在开启前端放像，稍后进行VCR控制
	CMS_SDK_PUI_TRANS_OPR_TYPE_ERROR = 12043,                   //透明数据操作类型错误

	CMS_SDK_PUI_PU_UNKNOWN = CMS_SDK_PUI_BEGIN + 998,                //前端未知错误
	CMS_SDK_PUI_END = CMS_SDK_PUI_BEGIN + 999,      //pui错误码结束值

	//cui错误码 13000-13999 当前可用号 13028
	CMS_SDK_CUI_BEGIN = 13000,                  //cui错误码起始值

	CMS_SDK_CUI_USERNAME_OR_PASSWORD_WRONG = 13001, //用户名或者密码错误
	CMS_SDK_CUI_CONNECT_CUI_FAIL = 13002,           //无法连接上CUI服务器
	CMS_SDK_CUI_DISCONNECT_CUI = 13003,             //和CUI断开连接
	CMS_SDK_CUI_NOT_IN_SERVICE = 13004,             //CUI未进入服务态
	CMS_SDK_CUI_OPR_TIME_OUT = 13005,               //请求超时
	CMS_SDK_CUI_CU_NOT_ONLINE = 13006,              //CU不在线，请求非法
	CMS_SDK_CUI_CU_NO_PERMISSION = 13007,           //CU没有权限
	CMS_SDK_CUI_QUERYING_PRIVILEGE_DATA = 13026,    //正在查询权限数据，请稍后再试
	CMS_SDK_CUI_QUERYING_INNER_DATA = 13027,        //正在查询内部数据，请稍后再试
	CMS_SDK_CUI_DIALOG_NOT_ESTABLISHED = 13008,     //会话尚未建立，操作被拒绝
	CMS_SDK_CUI_NOT_SUBSCRIBE = 13009,              //设备未订阅过
	CMS_SDK_CUI_PARAMS_INVALID = 13010,             //参数非法，请检查
	CMS_SDK_CUI_DIALOG_TERMINATED = 13011,          //会话被对端主动结束
	CMS_SDK_CUI_SIP_MSG_FAIL = 13012,               //发送SIP请求失败，可能服务器不在线
	CMS_SDK_CUI_TAS_OFFLINE = 13013,                //TAS服务器不在线，操作失败
	CMS_SDK_CUI_INVALID_PARAM = 13014,              //无效请求参数
	CMS_SDK_CUI_IN_SUBSCRIBE = 13015,               //正在订阅中
	CMS_SDK_CUI_UNKNOW_MSG = 13016,                 //不认识的消息
	CMS_SDK_CUI_TASK_DEL = 13017,                   //task消亡
	CMS_SDK_CUI_TCP_TIMEOUT = 13018,                //tcp端接收消息失败
	CMS_SDK_CUI_PE_OFFLINE = 13019,                //外设模块不在线
	CMS_SDK_CUI_RECORD_END = 13020,                //录像播放停止
	CMS_SDK_CUI_PASSWORD_ERROR = 13021,            //密码校验错误
	CMS_SDK_CUI_EXCEED_CU_MAX_NUM = 13022,         //超出最大cu用户数
	CMS_SDK_CUI_CROSS_DOMAIN_LOGIN = 13023,        //暂不允许跨域登录
	CMS_SDK_CUI_STATE_ERROR = 13024,               //任务状态不正确
	CMS_SDK_CUI_RSP_MESSAGE_TOO_LONG = 13025,      //应答消息体过长

	CMS_SDK_CUI_END = CMS_SDK_CUI_BEGIN + 999,      //cui错误码结束值

	//vtdu错误码 14000-14999
	CMS_SDK_VTDU_BEGIN = 14000,                 //vtdu错误码起始值

	CMS_SDK_VTDU_ADD_SWITCH_TO_DS_FAILED = 14001,       //[VTDU]添加交换到DataSwitch失败
	CMS_SDK_VTDU_REMOVE_SWITCH_FROM_DS_FAILED = 14002,  //[VTDU]从DataSwitch删除交换失败
	CMS_SDK_VTDU_ADD_SWITCH_INVALID_ARGUMENT = 14003,   //[VTDU]添加交换请求的参数不对
	CMS_SDK_VTDU_TASK_TIMEOUT = 14004,                  //[VTDU]任务处理超时
	CMS_SDK_VTDU_EXCEPTION = 14005,                     //[VTDU]系统异常
	CMS_SDK_VTDU_WAIT_ACK_TIMEOUT = 14006,              //[VTDU]等待确认应答超时
	CMS_SDK_VTDU_STUN_UPDATE_FAILED = 14007,            //[VTDU]更新Stun地址失败
	CMS_SDK_VTDU_WITHOUT_VTDU_ADDR_OF_NETSEG = 14008,   //[VTDU]没有相应网段的VTDU地址

	CMS_SDK_VTDU_QUERY_MAPPED_ADDR_FAILED = 14500,      //[VTDU]查询外网地址失败
	CMS_SDK_VTDU_QUERY_MAPPED_ADDR_TIMEOUT = 14501,     //[VTDU]查询外网地址超时
	CMS_SDK_VTDU_QUERY_MAPPED_ADDR_CANCELED = 14502,    //[VTDU]查询外网地址操作被取消

	CMS_SDK_VTDU_PUNCH_FAILED = 14520,                  //[VTDU]地址探测失败
	CMS_SDK_VTDU_PUNCH_TIMEOUT = 14521,                 //[VTDU]地址探测超时
	CMS_SDK_VTDU_PUNCH_CANCELED = 14522,                //[VTDU]地址探测操作被取消
	CMS_SDK_VTDU_INVALID_PUNCH_PARAM = 14523,           //[VTDU]无效的Punch参数

	CMS_SDK_VTDU_END = CMS_SDK_VTDU_BEGIN + 999,    //vtdu错误码结束值

	//pmcs错误码 15000-15999
	CMS_SDK_PMCS_BEGIN = 15000,

	CMS_SDK_PMCS_NOT_IN_SERVICE = 15001,         //pmcs未进入服务态
	CMS_SDK_PMCS_OPR_TIME_OUT = 15002,          //业务请求超时
	CMS_SDK_PMCS_WEB_NOT_ONLINE = 15003,          //pmcs web 不在线,请求非法
	CMS_SDK_PMCS_DIALOG_TERMINATED = 15004,        //与pms会话被对端主动结束
	CMS_SDK_PMCS_SIP_MSG_FAIL = 15005,               //发送SIP请求失败，可能服务器不在线
	CMS_SDK_PMCS_DISCONNECT_PMCS = 15006,             //和PMCS断开连接
	CMS_SDK_PMCS_PMS_NOT_IN_SERVICE = 15007,          //PMS未进入服务
	CMS_SDK_PMCS_3AS_NOT_IN_SERVICE = 15008,      //3as未进入服务
	CMS_SDK_PMCS_RCS_NOT_IN_SERVICE = 15009,					//rcs不在服务态
	CMS_SDK_PMCS_RCS_SUBSCRIBE_ALREADY_EXIST = 15010,		//订阅已存在
	CMS_SDK_PMCS_RCS_SUBSCRIBE_NOT_EXIST = 15011,			//尚未订阅
	CMS_SDK_PMCS_PMCAGENT_NOT_EXIST = 15012,		//PMCAGENT不存在
	CMS_SDK_PMCS_PMC_NOT_IN_SERVICE = 15013,		//PMC异常，拒绝服务
	CMS_SDK_PMCS_PMS_PLATFORM_RELATION_ERROR = 15014,    //平台关系填错
	CMS_SDK_PMCS_PMS_OPT_ERROR = 15015,                  //业务处理失败
	CMS_SDK_PMCS_PMS_SUBPLATFORM_ADD_OPT_ERROR = 15016,  //添加从平台或下级平台时从平台或下级平台处理失败，且主平台或上级平台有残留
	CMS_SDK_PMCS_PMS_SUBPLATFORM_DEL_OPT_ERROR = 15017,  //删除从平台或下级平台时，主平台或上级平台处理失败，从平台或下级平台已处理
	CMS_SDK_PMCS_DETECTED_NO_TOPO_INFO = 15018,          //未探测到正确到topo平台
	CMS_SDK_PMCS_NAT_DATA_ERR = 15019,                   //映射数据有误，不能完成组网
	CMS_SDK_PMCS_GBS_NOT_IN_SERVICE = 15020,             //GBS未进入服务态
	CMS_SDK_PMCS_IMPORT_FILE_FAIL = 15021,               //设备批量入网失败
	CMS_SDK_PMCS_IMPORT_DEV_INFO_ERR = 15022,            //批量导入设备文件模板错误
	CMS_SDK_PMCS_GET_DEV_MOD_FAIL = 15023,        //获取设备型号失败
	CMS_SDK_PMCS_DEV_IMPORT_FILE_NOT_EXIST   = 15024,    //批量导入设备文件不存在
	CMS_SDK_PMCS_DEV_MODEL_EMPTY             = 15025,    //没有查到设备型号
	CMS_SDK_PMCS_QRY_DEV_FAIL                = 15026,    //查询所有设备失败
	CMS_SDK_PMCS_QRY_GBDEV_FAIL              = 15027,    //查询国标设备失败
	CMS_SDK_PMCS_QRY_DEVICE_EMPTY            = 15028,    //查询到0个设备

	CMS_SDK_PMCS_END = CMS_SDK_PMCS_BEGIN + 999,    //pmcs错误码结束

	// CU错误码 16000 - 16999 
	CMS_SDK_CU_BEGIN = 16000,						// cu错误码段开始

	CMS_SDK_CU_END = CMS_SDK_CU_BEGIN + 999,	// cu错误码结束。

	//als错误码 17000-17999
	CMS_SDK_ALS_BEGIN = 17000,                  //als错误码起始值

	CMS_SDK_ALS_INVALID_ALARM_LINK = 17001,     //无效的告警联动配置
	CMS_SDK_ALS_DB_OPR_FAIL = 17002,            //数据库操作失败
	CMS_SDK_ALS_DB_OPR_TIMEOUT = 17003,         //数据库操作超时
	CMS_SDK_ALS_ALARM_LINK_NOT_EXIST = 17004,   //告警联动不存在

	CMS_SDK_ALS_END = CMS_SDK_ALS_BEGIN + 999,      //als错误码结束值

	//tvs错误码 18000-18999
	CMS_SDK_TVS_BEGIN = 18000,                  //tvs错误码起始值
	CMS_SDK_TVS_TVWALL_NOT_EXIST = 18001,       //电视墙不存在
	CMS_SDK_TVS_TV_PLAY_NOT_EXIST = 18002,      //电视播放不存在
	CMS_SDK_TVS_TV_OCCUR_NOT_MATCH = 18003,     //电视触发类型不匹配
	CMS_SDK_TVS_TV_SRC_NOT_MATCH = 18004,       //电视播放源不不匹配
	CMS_SDK_TVS_3PC_INVITE_FAIL = 18005,        //3PC呼叫失败，sip层错误
	CMS_SDK_TVS_3PC_INVITE_TIMEOUT = 18006,     //3PC呼叫超时
	CMS_SDK_TVS_TV_POLL_OPR_FAIL = 18007,       //轮询操作失败
	CMS_SDK_TVS_DB_OPR_FAIL = 18008,            //数据操作失败
	CMS_SDK_TVS_DB_OPR_TIMEOUT = 18009,         //数据操作超时
	CMS_SDK_TVS_ILLEGAL_SCHEME_NAME = 18010,    //无效的预案名称
	CMS_SDK_TVS_ILLEGAL_SCHEME_ID = 18011,      //无效的预案ID
	CMS_SDK_TVS_SCHEME_NOT_EXIST = 18012,       //预案不存在
	CMS_SDK_TVS_CUI_LOGIN_SESS_ILLEGAL = 18013, //非法的cui登录session
	CMS_SDK_TVS_TV_DEC_BIND_ERROR = 18014,      //电视机绑定解码器错误
	CMS_SDK_TVS_ILLEGAL_ENC_CHN = 18015,        //无效的编码通道
	CMS_SDK_TVS_ILLEGAL_SHEME_POLL_CFG = 18016, //无效的预案轮询配置
	CMS_SDK_TVS_SET_DEC_STYLE_TIMEOUT = 18017,  //设置解码器风格超时
	CMS_SDK_TVS_NOT_LOGIN_CMU = 18018,          //未登录cmu
	CMS_SDK_TVS_LOGIN_CMU_BYE_3PC = 18019,      //cmu主动bye 3PC呼叫
	CMS_SDK_TVS_3PC_DIALOG_TERMINATE = 18020,   //3PC呼叫异常终止，sip层错误
	CMS_SDK_TVS_LOCKED_TVWALL_OR_CHANNEL_NOT_EXIST = 18021,   //不存在的锁定电视通道或者锁定电视墙预案
	CMS_SDK_TVS_UNLOCK_TVWALL_OR_CHANNEL_PASS_ERR = 18022,   //解锁电视通道或者预案密码错误
	CMS_SDK_TVS_INVALID_WATCHOINT_SAVE_REQ = 18023, //无效的观看点备份请求
	CMS_SDK_TVS_SAVED_WATCHPOINT_NOT_EXIST = 18024, //不存在的备份观看点
	CMS_SDK_TVS_TVWALL_OR_CHANNEL_LOCKED = 18025, //电视墙预案或者通道被锁
    CMS_SDK_TVS_TVWALL_TVCHN_TASK_EXIST = 18026, //电视通道已占用
    CMS_SDK_TVS_TVWALL_TIMEOUT = 18027,          //操作超时
	CMS_SDK_TVS_END = CMS_SDK_TVS_BEGIN + 999,      //tvs错误码结束值

	//rcs错误码 19000-19999
	CMS_SDK_RCS_BEGIN = 19000,                  //rcs错误码起始值
	CMS_SDK_RCS_RMS_NOT_ONLINE = 19001,             //[RCS]rms不在线
	CMS_SDK_RCS_NRU_NOT_ONLINE = 19002,             //[RCS]nru不在线
	CMS_SDK_RCS_DB_OPR_FAIL = 19003,                //[RCS]数据库操作失败
	CMS_SDK_RCS_DB_OPR_TIMEOUT = 19004,             //[RCS]数据库操作超时
	CMS_SDK_RCS_PU_NOT_ONLINE = 19005,              //[RCS]前端设备不在线
	CMS_SDK_RCS_PU_CONFIG_NOT_AVAILABLE = 19006,    //[RCS]前端配置参数不可得
	CMS_SDK_RCS_RECORD_PARAM_NOT_EXIST = 19007,     //[RCS]录像参数不存在
	CMS_SDK_RCS_RECORD_TASK_ALREADY_EXIST = 19008,  //[RCS]录像任务已经存在
	CMS_SDK_RCS_EXCEPTION = 19009,                  //[RCS]服务器异常

	CMS_SDK_RCS_ALLOC_RECORD_RES_FAILED = 19010,    //[RCS]分配录像资源失败
	CMS_SDK_RCS_START_RECORD_FAILED = 19011,        //[RCS]开启录像失败
	CMS_SDK_RCS_RECORD_TASK_NOT_EXIST = 19012,      //[RCS]不存在相应的录像任务
	CMS_SDK_RCS_UPDATE_RECTYPE_FAILED = 19013,      //[RCS]更新录像类型失败
	CMS_SDK_RCS_DEV_NOT_REGISTER = 19014,           //[RCS]设备未注册
	CMS_SDK_RCS_TASK_TIMEOUT = 19015,               //[RCS]任务处理超时
	CMS_SDK_RCS_NOT_REG_TO_CMU = 19016,             //[RCS]未注册上CMU
	CMS_SDK_RCS_PLAYBACK_TASK_NOT_EXIST = 19017,    //[RCS]放像任务不存在
	CMS_SDK_RCS_DEV_NOT_EXIST = 19018,              //[RCS]设备不存在
	CMS_SDK_RCS_SS_OPE_TYPE_INVALID = 19019,        //[RCS]无效的订阅类型
	CMS_SDK_RCS_DEV_NO_ROUTER = 19020,              //[RCS]请求无法被路由到指定设备
	CMS_SDK_RCS_STOP_BY_BYE = 19021,                //[RCS]对话被挂断
	CMS_SDK_RCS_NOT_SUPPORT = 19022,                //[RCS]该功能不支持
	CMS_SDK_RCS_CONVERT_URI_DOMAIN_FAILED = 19023,  //[RCS]转换URI的域名失败，可能找不到相应的域信息
	CMS_SDK_RCS_VCR_TRY_AGAIN = 19024,              //[RCS]正在开启录像回放，稍后进行VCR控制

	CMS_SDK_RCS_END = CMS_SDK_RCS_BEGIN + 999,          //rcs错误码结束值

	//uas错误码 20000-20999 (当前可用号为20003)
	CMS_SDK_UAS_BEGIN = 20000,                  //uas错误码起始值
	CMS_SDK_UAS_DBERROR = 20001,                // 数据库操作出错，请重试一次
	CMS_SDK_UAS_SAVE_CONFIG_FAILED = 20002,     // 保存配置失败
	CMS_SDK_UAS_END = CMS_SDK_UAS_BEGIN + 999,      //uas错误码结束值

	//mps错误码 21000-21999
	CMS_SDK_MPS_BEGIN = 21000,                  //mps错误码起始值
	CMS_SDK_MPS_INVAILD_MSG  = 21001,			//错误的消息体
	CMS_SDK_MPS_MSG_TIME_OUT	 = 21002,		//消息处理超时
	CMS_SDK_MPS_DB_CONNECT_ERROR = 21003,		//连接数据库失败
	CMS_SDK_MPS_DB_OPR_ERROR	= 21004,		//操作数据库失败
	CMS_SDK_MPS_DB_OPR_TIMEOUT	= 21005,		//操作数据库超时
	CMS_SDK_MPS_DB_NO_RESULT	= 21006,		//数据查询返回无内容
	CMS_SDK_MPS_MAP_EXISTS		= 21007,		//地图已存在
	CMS_SDK_MPS_MAP_ELEMENT_EXISTS	= 21008,	//图元已存在
	CMS_SDK_MPS_ICON_PATH_NOT_EXISTS = 21009,	//地图图元目录不存在
	CMS_SDK_MPS_PARAMS_INVAILD		= 21010,	//参数不合法
	CMS_SDK_MPS_POINT_OUT_OF_RANG	= 21011,	//校正点在范围之外
	CMS_SDK_MPS_NOT_CONFIG_GPS_STORE	= 21012,	//未配置GPS存储
	CMS_SDK_MPS_END = CMS_SDK_MPS_BEGIN + 999,      //mps错误码结束值

	//pms错误码 22000-22999 当前可用错误码为22057
	CMS_SDK_PMS_BEGIN = 22000,                  //pms错误码起始值
	CMS_SDK_PMS_INVALID_PARAM = 22012,          //无效的请求参数
	CMS_SDK_PMS_DATA_NOT_EXIST = 22019,         //数据不存在
	CMS_SDK_PMS_NOT_IN_SERVICE = 22001,         //PMS未进入服务态
	CMS_SDK_PMS_INVALID_OPERATION = 22020,      //非法操作，业务上不允许如此操作
	CMS_SDK_PMS_EXECUTE_TIMEOUT = 22039,        //执行超时

	CMS_SDK_PMS_3AS_NOT_IN_SERVICE = 22002,     //3as未进入服务态
	CMS_SDK_PMS_INVALID_PMC_URI = 22003,        //无效的pmc uri
	CMS_SDK_PMS_PMS_ALREADY_CONN = 22004,       //重复登录
	CMS_SDK_PMS_DB_OPR_FAIL = 22005,            //数据库操作错误
	CMS_SDK_PMS_USERNAME_OR_PASSWORD_WRONG = 22006, //用户名或密码错误
	CMS_SDK_PMS_INVALID_SERVER_URI = 22007,         //无效的SERVER URI
	CMS_SDK_PMS_DB_OPR_TIMEOUT = 22008,         //数据库操作超时
	CMS_SDK_PMS_WEB_ONT_ONLINE = 22009,			//WEB不在线
	CMS_SDK_PMS_PMS_UNKNOW_DEVTYPE = 22010,     //无效的设备类型
	CMS_SDK_PMS_DB_DATA_NOT_EXIST = 22011,		//数据库中不存在要找的数据
	CMS_SDK_PMS_OPR_FAIL = 22015,               //操作失败

	CMS_SDK_PMS_PLATFORM_ALREADY_IN_TOPO = 22014,//待入网平台已经参与组网，请先退出再组网
	CMS_SDK_PMS_REPEATED_PLATFORM_RELATION = 22021, //重复的平台关系
	CMS_SDK_PMS_GET_MOSTADAPTIVEIP_FAILED = 22051,  //获取网段最长匹配IP失败, 请检查是否需要配置NAT映射
	CMS_SDK_PMS_DETECTED_NOT_CONSISTENCE_TOPO_CONF = 22017, //探测到不一致的TOPO配置 
	CMS_SDK_PMS_MODIFY_CONFIG_FAILED = 22016,               //修改PMS的配置文件失败
	CMS_SDK_PMS_MODIFY_COMMONCONFIG_FAILED = 22018,         //修改公共配置文件失败

	CMS_SDK_PMS_GET_ROUTEINFO_ERR = 22022,              //获取网络路由信息失败
	CMS_SDK_PMS_SET_ROUTEINFO_ERR = 22023,              //设置网络路由失败
	CMS_SDK_PMS_DEL_ROUTEINFO_ERR = 22024,              //删除网络路由失败
	CMS_SDK_PMS_ADD_ROUTEINFO_ERR = 22025,              //添加网络路由失败
	CMS_SDK_PMS_RESET_ERR = 22026,                      //还原出厂设置失败
	CMS_SDK_PMS_SOFTWAREINFO_QRY_ERR = 22027,           //软件信息获取失败
	CMS_SDK_PMS_SOFTWAREINFO_MOD_ERR = 22028,           //软件信息修改失败
	CMS_SDK_PMS_GET_DEFAULTROUTE_ERR = 22029,           //获取默认网关失败
	CMS_SDK_PMS_SET_DEFAULTROUTE_ERR = 22030,           //设置默认网关失败
	CMS_SDK_PMS_MOD_PLATFORMTIME_ERR = 22031,           //修改平台时间失败
	CMS_SDK_PMS_CTRL_PMS_ERR = 22032,                   //PMS平台控制失败
	CMS_SDK_PMS_MOD_NETCARDINFO_ERR = 22033,            //修改网卡信息失败
	CMS_SDK_PMS_QRY_PLATFORMTIME_ERR = 22034,           //查询平台时间失败
	CMS_SDK_PMS_ADD_NETNAT_ERR = 22035,                 //添加网络NAT失败
	CMS_SDK_PMS_DEL_NETNAT_ERR = 22036,                 //删除网络NAT失败
	CMS_SDK_PMS_ADD_REPEATED_DATA_ERR = 22037,          //重复添加数据
	CMS_SDK_PMS_NETWORK_RESTART_ERR = 22038,            //网络服务重启失败
	CMS_SDK_PMS_INVTASK_LIMITED = 22040,                //升级、镜像导入、恢复出厂设置同时只能存在一个操作,若有两个以上相应请求则错误
	CMS_SDK_PMS_ADD_USER_ALREADY_EXIST = 22041,         //添加的用户已经存在
	CMS_SDK_PMS_MOD_USER_NOT_EXIST = 22042,             //要修改的用户不存在
	CMS_SDK_PMS_DEL_NTP_SERVER_ERR = 22043,             //删除NTP服务失败
	CMS_SDK_PMS_SET_NTP_SERVER_ERR = 22045,             //设置NTP服务失败
	CMS_SDK_PMS_REMOVE_USBBAK_FAILED = 22044,           //移除USB备份盘失败，可能是备份盘正在使用，请稍候重试
	CMS_SDK_PMS_RECORD_USERLOG_FAILED = 22046,          //记录用户日志失败
	CMS_SDK_PMS_EXPORTMIRROR_TASK_ALREADY_EXIST = 22047,//导出镜像的任务已经存在
	CMS_SDK_PMS_ABNORMAL_STATUS = 22048,                //PMS处于异常状态，只提供有限服务
	CMS_SDK_PMS_INVALID_NTP_SERVER_ADDR = 22049,        //无效的NTP服务地址
	CMS_SDK_PMS_DOMAINNAME_ALREADY_EXIST = 22050,       //要修改的域名与其上下级平台冲突，操作失败
	CMS_SDK_PMS_SET_MAIL_CFG_ERR = 22052,               //邮箱配置失败
	CMS_SDK_PMS_QRY_MAIL_CFG_ERR = 22053,               //邮配置查询失败
	CMS_SDK_PMS_SEND_TEST_MAIL_ERR = 22054,             //测试邮件发送失败,发邮件命令执行失败
	CMS_SDK_PMS_MAILTO_LIST_IS_NULL = 22055,            //邮件接收列表为空，测试邮件发送失败
	CMS_SDK_PMS_MAILSERVER_CFG_ERR = 22056,             //邮件服务器配置有误，不能为空

	CMS_SDK_PMS_END = CMS_SDK_PMS_BEGIN + 999,      //pms错误码结束值

	//rmssdk错误码 23000-23999
	CMS_SDK_RMSSDK_BEGIN = 23000,                   //rmssdk错误码结束值
	CMS_SDK_RMSSDK_END = CMS_SDK_RMSSDK_BEGIN + 999,    //rmssdk错误码结束值

	//scsdk错误码 24000-24999
	CMS_SDK_SCSSDK_BEGIN = 24000,                   //scsdk错误码结束值
	CMS_SDK_SCSSDK_END = CMS_SDK_SCSSDK_BEGIN + 999,    //scsdk错误码结束值

	//gbs错误码 25000-25999， 当前可用号为25025
	CMS_SDK_GBS_BEGIN = 25000,
	CMS_SDK_GBS_INVALID_PARAM = 25007,              //参数非法
	CMS_SDK_GBS_INVALID_OPERATE = 25008,            //非法操作
	CMS_SDK_GBS_CMSSDK_SENDMSG_FAILED = 25001,      //cmssdk发送消息失败
	CMS_SDK_GBS_CMSSDK_RESPONSE_TIMEOUT = 25002,    //cmssdk回复超时
	CMS_SDK_GBS_SAVE_CONFIG_FAILED = 25003,         //配置文件保存失败
	CMS_SDK_GBS_DBOPR_FAILED = 25004,               //数据库操作失败
	CMS_SDK_GBS_DBOPR_TIMEOUT = 25005,              //数据库操作超时
	CMS_SDK_GBS_EXIST_REPEATED_DATA = 25006,        //存在重复数据
	CMS_SDK_GBS_DETECTED_CIRCLE_DATA = 25017,       //探测到环数据
	CMS_SDK_GBS_INVALID_GBID = 25009,               //国标编号非法
	CMS_SDK_GBS_SAME_GBID_WITH_LOCAL = 25016,       //与本域国标编号相同
	CMS_SDK_GBS_GBDOMAIN_ID_NOT_AVAILABLE = 25020,  //国标域号不可用，也许存在相同的域号或存在环
	CMS_SDK_GBS_NOT_AVAILABLE_GBID = 25021,         //国标编号不可用，也许是国标编号非法或者非本域域号或者类型不支持或者编号已经被占用
	CMS_SDK_GBS_NOT_ENOUGH_GBID = 25022,            //提供的国标编号不够，要求设备和通道都需要指定
	CMS_SDK_GBS_DATA_NOT_EXIST = 25010,             //数据不存在
	CMS_SDK_GBS_DATA_NOT_INTEGRITY = 25011,         //数据不完整，建议删除相应数据
	CMS_SDK_GBS_PU_NOT_ONLINE = 25012,              //前端不在线
	CMS_SDK_GBS_OSP_SIP_POST_FAIL = 25013,          //OSPSIP发送消息失败
	CMS_SDK_GBS_PU_VCR_NOT_SUPPORT_CMD = 25014,     //前端不支持的命令
	CMS_SDK_GBS_PU_VCR_NOT_SUPPORT_PLAY_RATE = 25015,//前端不支持的播放速度
	CMS_SDK_GBS_G800_ERROR = 25018,                 //G800错误
	CMS_SDK_GBS_486_SIP_ERROR = 25019,              //486SIP错误
    CMS_SDK_GBS_SUBGBDMOAIN_NOT_ONLINE = 25023,     //国标下级域不在线
    CMS_SDK_GBS_LOWER_DOMAIN_BUSY = 25024,          //下级国标域忙
	CMS_SDK_GBS_END = 25999,

	//cmssdk内部错误码：26000-26499，由于CMSSDK的错误码已经在使用了，就不增加CMS_前缀了
	CMS_SDK_BEGIN                                     = 26000,
	CMS_SDK_REINIT                                    = 26001,          //重复初始化
	CMS_SDK_CALLBACK_INVALID                          = 26002,          //回调函数无效
	CMS_SDK_GW_UUID_INVALID                           = 26003,          //网关UUID无效
	CMS_SDK_DOMAIN_NAME_INVALID                       = 26004,          //平台域名无效
	CMS_SDK_GW_ADDR_INVALID                           = 26005,          //网关地址无效
	CMS_SDK_PLAT_ADDR_INVALID                         = 26006,          //平台地址无效
	CMS_SDK_OSPEXT_INIT_FAIL                          = 26007,          //ospext初始化失败
	CMS_SDK_OSPSIP_INIT_FAIL                          = 26008,          //ospsip初始化失败
	CMS_SDK_OSPAPP_CREATE_FAIL                        = 26009,          //ospapp创建失败
	CMS_SDK_GW_MSG_INVALID                            = 26010,          //网关消息无效
	CMS_SDK_OSP_POST_FAIL                             = 26011,          //OspPost失败
	CMS_SDK_TIMEOUT                                   = 26012,          //超时错误
	CMS_SDK_SEND_MSG_FAIL                             = 26013,          //向平台发送请求失败
	CMS_SDK_CMU_DIS_CONN                              = 26014,          //未连接到CMU
	CMS_SDK_3AS_DIS_CONN                              = 26015,          //未连接到3as
	CMS_SDK_PU_REG_TIMEOUT                            = 26016,          //pu注册超时
	CMS_SDK_DEVICE_UUID_INVALID                       = 26017,          //设备ID无效
	CMS_SDK_DEVICE_NAME_INVALID                       = 26018,          //设备名无效
	CMS_SDK_GROUP_UUID_INVALID                        = 26019,          //设备组组ID无效
	CMS_SDK_GROUP_NAME_INVALID                        = 26020,          //设备组组名无效
	CMS_SDK_DOMAIN_UUID_INVALID                       = 26021,          //域ID无效
	CMS_SDK_DEVICE_NOT_EXIST                          = 26022,          //设备不存在
	CMS_SDK_RCS_DIS_CONN                              = 26023,          //未连接到rcs
	CMS_SDK_TVS_DIS_CONN                              = 26024,          //未连接到tvs
	CMS_SDK_HAVE_NOT_SS                               = 26025,		   //尚未订阅（用于取消订阅前检查是否已订阅）
	CMS_SDK_ALREADY_SS                                = 26026,		   //重复订阅（用于订阅前检查是否重复订阅）
	CMS_SDK_SS_OPE_TYPE_INVALID                       = 26027,          //无效的订阅操作类型
	CMS_SDK_TRANSCODE_TERMINATE                       = 26028,          //G800转码异常终止
	CMS_SDK_PU_CONF_NTF_TIMEOUT                       = 26029,          //pu配置通知超时
	CMS_SDK_PU_UNREG_TIMEOUT                          = 26030,          //pu下线通知超时
	CMS_SDK_PU_ALREADY_REG                            = 26031,			//pu已经注册
	CMS_SDK_ERR_DIALOG_TERMINATE                      = 26032,			//会话异常终止
	CMS_SDK_ERR_TASK_DEL                              = 26033,			//Task已经删除
	CMS_SDK_DIALOG_NOT_ESTABLISHED                    = 26034,			//会话尚未建立
	CMS_SDK_UAS_DIS_CONN                              = 26035,          //未连接到uas
	CMS_SDK_VCR_CTRL_TIMEOUT                          = 26036,          //VCR控制请求消息超时
	CMS_SDK_PLAT_REC_PLAYBACK_FINISHED                = 26037,          //平台放像正常结束
    CMS_SDK_COMMONCONFIG_INIT_FAIL                    = 26038,          //公共配置文件初始化失败
    CMS_SDK_PU_REG_CONFIG_NOT_COMPLETED               = 26039,          //pu注册信息不完整

	CMS_SDK_CMS_SDK_END                                   = 26499,

	//GW错误码：26500-26999
	CMS_SDK_GW_BEGIN                                      = 26500,
	CMS_SDK_GLB_ERR_NOINIT_ADAPTER                        = 26501,          //初始化ADAPTER
	CMS_SDK_GLB_ERR_LOAD_ADATER_FAIL                      = 26502,          //加载adapter失败
	CMS_SDK_GLB_ERR_INIT_ADAPTER_FAIL                     = 26503,          //初始化adapter失败
	CMS_SDK_GLB_ERR_NOINIT_PLAT                           = 26504,          //没有初始化平台
	CMS_SDK_GLB_ERR_USERNAME_ERR                          = 26505,          //用户信息错误
	CMS_SDK_GLB_ERR_PASSWORD_ERR                          = 26506,          //用户密码错误
	CMS_SDK_GLB_ERR_PARA_EMPTY                            = 26507,          //参数传递错误
	CMS_SDK_GLB_ERR_DLL_NOOBJ                             = 26508,          //底层dll为空
	CMS_SDK_GLB_ERR_UNKNOWN_REASON                        = 26509,          //未知原因的错误
	CMS_SDK_GLB_ERR_OPR_TIMEOUT                           = 26510,          //操作超时
	CMS_SDK_GLB_ERR_OPR_FAIL                              = 26511,		    //操作失败
	CMS_SDK_GLB_ERR_TRANS_LIMIT                           = 26512,          //转发超过限制       
	CMS_SDK_GLB_ERR_ALREADY_REALPLAY                      = 26513,          //针对该设备的接收地址已经开启(实时浏览)
	CMS_SDK_BLB_ERR_DEV_RECDOWN_EXIST                     = 26514,          //针对该设备通道的录像回放已经存在
	CMS_SDK_GLB_ERR_RESOURCE_LACK                         = 26515,          //系统资源不足
	CMS_SDK_GLB_ERR_NOSUPP_FUNC                           = 26516,          //不支持该功能
	CMS_SDK_GLB_ERR_NOSTREAM                              = 26517,          //无码流的通知
	CMS_SDK_GLB_ERR_NOINIT_NAT                            = 26518,          //NAT穿透未初始化
	CMS_SDK_GLB_ERR_DAU_REG_FAIL                          = 26519,          //DAU向GMU注册失败
	// G100相关错误码
	CMS_SDK_GLB_G100_SDKFAILED                            = 26520,	        //调用SDK接口失败
	CMS_SDK_GLB_G100_RECODERNOTFOUND                      = 26521,		    //查询不到录像
	CMS_SDK_GLB_G100_NOENOUGHPRI                          = 26522,			//权限不足
	CMS_SDK_GLB_G100_DVRNORESOURCE                        = 26523,			//DVR资源不足，系统忙
	CMS_SDK_GLB_G100_DVROPRATEFAILED                      = 26524,		    //DVR操作失败
	CMS_SDK_GLB_G100_TIMEINPUTERROR                       = 26525,		    //时间输入不正确
	CMS_SDK_GLB_G100_NOSPECFILE                           = 26526,			//回放时服务器没有指定的文件
	CMS_SDK_GLB_G100_OPERNOTFINISH                        = 26527,			//上次操作还未完成
	CMS_SDK_GLB_G100_COMMANDTIMEOUT                       = 26528,		    //执行命令超时
	CMS_SDK_GLB_G100_NOSUPPORT                            = 26529,			//服务器不支持
	CMS_SDK_GLB_G100_FILEFORMATERROR                      = 26530,		    //文件格式不正确
	CMS_SDK_GLB_G100_NODOWNLOADCHN                        = 26531,			//没有更多的下载通道可用(录像回放与下载不能同时进行)
	CMS_SDK_GLB_G100_DVRHASQUERYTASK                      = 26532,          //DVR已经存在查询任务，请稍后再查
	CMS_SDK_GLB_G100_DVRQUERYTIMEOUT                      = 26533,          //DVR录像查询超时 请等待
	CMS_SDK_GLB_G100_DVRQUERYUPLOADING                    = 26534,          //DVR其他用户正在回传数据 请等待
	CMS_SDK_GLB_G100_DHQUERYTIMEOUT                       = 26535, 		    //大华查询录像超时
	CMS_SDK_GLB_G100_DHPLAYBACKLIMIT                      = 26536,		    //大华录像回放只能一路

	CMS_SDK_GW_END                                        = 26999,

	//未知错误码，尽量不使用未知错误
	CMS_SDK_UNKNOWN = 0xFFFFFFFF,
};

#endif // __CMS_SDK_ERRORCODE_H__


