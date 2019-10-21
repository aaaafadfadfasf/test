/*****************************************************************************
   模块名      : cms_errorcode
   文件名      : cms_errorcode.h
   相关文件    : 
   文件实现功能: cms_errorcode.h 定义所有错误码
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/22   1.0         liangli        创建
******************************************************************************/

#ifndef _CMS_ERRORCODE_H_
#define _CMS_ERRORCODE_H_

/************************************************************************/
//错误码定义须知
//1，错误描述必须完整，有错误描述注释，并且尽可能详尽
//2，尽量避免未知错误，系统错误等含糊不清的错误码
//3，一种错误码只用于描述一种出错场景，例如CU断链和PU断链就是两种错误码，防止歧义
//4，为了防止错误码过多过细，同时用户也不关心具体错误，可对某一块操作的错误码进行归类，取一个代表性错误
//5. 为了是错误码不能被随意修改，枚举值后需要直接写对应的数值
/************************************************************************/

typedef int CMS_RESULT;

enum ECMS_ERRORCODE
{
    CMS_SUCCESS = 0,                       //操作成功,成功不需要针对每个操作都定义，所有操作共享

    //公共的一些错误码
    ERR_COMMON_INVALID_DEV_URI = 1,         //无效的dev uri
    ERR_COMMON_DEV_ALREADY_LOGIN= 2,        //相同uri的dev已经登录
	ERR_SIP_BODY_NULL = 3,				    //SIP消息体为空
    ERR_POST_MSG_FAILED = 4,                //发送消息失败
    ERR_INVALIAD_REQ_PARAM = 5,             //无效的请求参数
    ERR_INVALIAD_SDP = 6,                   //无效的媒体描述信息
    ERR_DIALOG_TERMINATE = 7,               //对话被中断
    ERR_MKDIR_FAILED = 8,                   //创建目录失败
    ERR_INVALID_SESSIONID = 9,              //无效的会话ID，指定的会话不存在
    ERR_REQUIRE_TOO_MUCH = 10,              //同类请求过多，超过业务限额
    ERR_OLD_PASSWORD_INCORRECT = 11,        //旧密码错误
	ERR_OLD_PASSWORD_EQUAL_NEW_PASSWORD = 12, //新旧密码相同
	

    //公共的sip层错误码：4xx-6xx，这个区段采用sip标准定义，不能被别的模块占用。
    //这些错误码全局共享，各模块无需再次定义
    //常用的sip错误码定义如下，后续可以增加
    // 4xx Client Error
    ERR_SIP_403_FORBIDDEN        = 403,  // 403 禁止访问
    ERR_SIP_404_NOT_FOUND        = 404,  // 404 找不到目标
    ERR_SIP_408_REQ_TIMEOUT      = 408,  // 408 请求超时
    ERR_SIP_423_INTERV_TOO_BRIEF = 423,  // 423 请求过于频繁
    ERR_SIP_486_BUSY_HERE        = 486,  // 486 Busy Here    
    // 5xx Server Error
    // 6xx Global Error

    //3as错误码 10000-10999(当前可用号为10056)
    ERR_3AS_BEGIN = 10000,                  //3as错误码起始值

    ERR_TAS_INVALID_PARM = 10001,        // 参数非法, 详细错误参见后台打印，此类错误主要用于程序员
    ERR_TAS_SHELL_CMD_FAILED = 10042,    // shell命令执行失败
    ERR_TAS_HANDLE_IN_PROGRESS = 10002,  // 表示一个操作正在处理过程中，用户无需重复提交请求
    ERR_TAS_NOT_FINISHED = 10055,        // 操作尚未完成，可以通过相关接口获取进度 
    ERR_TAS_INVALID_OPERATION = 10003,   // 非法操作，业务上不允许如此操作
    ERR_TAS_SYSTEM_BUSY = 10024,         // 系统繁忙，请稍后再试
    ERR_TAS_ULTRA_VIRES = 10028,         // 越权行为
    ERR_TAS_EXIST_SAMENAME_RECORD = 10029,  // 存在同名记录
    ERR_TAS_FIELD_CONTENT_EMPTY = 10031,    // 字段内容为空
    ERR_TAS_CURRENT_USER_NOT_EXIST = 10030, // 当前操作者不存在，会话不存在或者用户不存在

    ERR_TAS_SYSTEM_UNINITED = 10004,   // 系统未初始化
    ERR_TAS_DBTASK_POOL_FULL = 10005,  // DB任务池满
    ERR_TAS_DBERROR = 10006,           // 数据库操作出错，请重试一次
    ERR_TAS_RECORD_EXISTS = 10007,     // 数据库记录已经存在
    ERR_TAS_RECORD_NOT_EXISTS = 10008, // 数据库记录不存在
    ERR_TAS_RECORD_FETCH_FAILED = 10009, // 数据库记录获取失败, 可能是记录已经不存在了

    ERR_TAS_LOGINED = 10010,           // 相同标识的客户端已经登录TAS
    ERR_TAS_USER_NOT_EXIST = 10011,    // 用户不存在
    ERR_TAS_INVALID_PASSWORD = 10012,  // 密码错误
    ERR_TAS_USER_EXPIRED = 10025,      // 用户过期
    ERR_TAS_USER_DISABLE_MULTILOGIN = 10026,  // 禁止多次登录
    ERR_TAS_USER_TYPE_DISABLE_CHANGE = 10027, // 用户类型不允许修改, 比如存在儿子用户的用户被不能改为操作员

    ERR_TAS_DECODER_ALREADY_BINDED = 10033,   // 解码器已经被绑定

    ERR_TAS_LICENSE_KEY_ERROR = 10013,  // license key出错(license未正常连接或者license内容不正确)
    ERR_TAS_LICENSE_KEY_EXPIRE = 10014, // license key到期
    ERR_TAS_LICENSE_NOT_ENOUGH = 10015, // license不足
    ERR_TAS_LICENSE_KEY_INVALID_LICENSE_FILE = 10034,  // 无效的license文件(比如机器码不匹配或内容损坏)
    ERR_TAS_LICENSE_SOME_OTHER_PU_TOO_MANY = 10016,    // 某外厂商PU接入太多
    ERR_TAS_LICENSE_OTHER_PU_TOTAL_TOO_MANY = 10017,   // 外厂商PU接入总数太多
    ERR_TAS_LICENSE_NO_DIRECT_ACCESS_OTHER_PU = 10018, // 不允许直接接入外厂商PU
	ERR_TAS_LICENSE_OTHER_MORE_THAN_ALL = 10037,	   // 外厂商pu最大接入数大于最大监控点数
    ERR_TAS_LICENSE_WRITE_USB_KEY_ERROR = 10043,       // 写入USB KEY失败

    ERR_TAS_INVALID_EXPIRE_DATE = 10019,   // 过期日期错误
    ERR_TAS_INVALID_DOMAINID = 10020,      // 域号错误
    ERR_TAS_INVALID_DEVICEMODELID = 10021, // 设备型号错误
	ERR_TAS_SAME_DEVICEMODEL_INFO = 10038,  // 相同的设备型号信息
    ERR_TAS_INVALID_DEVICETYPEID = 10022,  // 设备类型错误
    ERR_TAS_INVALID_DEVICEID = 10023,      // 设备号错误

    ERR_TAS_INVALID_SYNCDATA = 10032,   // 无效的同步数据，比如使用时同步数据标识对应的数据内容已经不存在了
    ERR_TAS_DOMAIN_RELATION_VALIDATE_FAILED = 10035, // 域关系校验失败

    ERR_TAS_DISABLED_ACCOUNT = 10036,    // 未启用的用户
	ERR_TAS_EXTDATA_TOO_LONG = 10039,	 // 信息超长
	ERR_TAS_TIME_OUT = 10040,            // 处理超时
	ERR_TAS_OP_PART_FINISH = 10041,      // 操作部分完成，通常用于批量请求中部分完成了的情况
	ERR_TAS_DEVICE_MODEL_USING = 10044,      // 设备型号使用中
	ERR_TAS_LICENSE_ID_DIFF = 10045,			 // 扩容UsbKey时，UsbKey和FileKey许可证ID不同
	ERR_TAS_OP_MUST_ADMIN = 10046,			 // 必须是admin用户才允许的操作
	ERR_TAS_DEV_GROUP_IS_AUTO_UPDATE = 10047,	// 设备所在组为自动同步组，禁止该操作
	ERR_TAS_GROUP_MUST_TOP_AUTO_UPDATE = 10048,	// 只允许取消最顶层的自动同步组
	ERR_TAS_GROUP_IS_NOT_AUTO_UPDATE = 10049,	// 非自动同步组，禁止该操作
    ERR_TAS_LICENSE_SDK_INIT_ERROR = 10050,       // license sdk初始化失败
	ERR_TAS_LICENSE_MANAGE_PU_TOO_MANY = 10051,        // 超过了平台的可管理监控点数
	ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP = 10052, // 设备树分组层级或子分组个数超出国标限制，禁止该操作
	ERR_TAS_DEVTREE_TOTALNUM_TOO_MUCH = 10053,     // 自定义设备树总数超出限制
	ERR_TAS_RELYMODULE_DEALTIMEOUT_NOSTART = 10054, //业务依赖模块未开启或处理超时
	ERR_TAS_INPUT_ERROR_PASSWORD_TOO_MUCH = 10056, //连续输入密码多次错误被限制登陆
	ERR_TAS_IP_LIMIT_LOGIN = 10057,                //当前IP被限制登陆
	ERR_TAS_PASSWORD_EXPIRED = 10058,              //密码已过期

    ERR_3AS_END = ERR_3AS_BEGIN + 999,      //3as错误码结束值

    //cmu错误码 11000-11999
    ERR_CMU_BEGIN = 11000,                  //cmu错误码起始值

    ERR_CMU_NOT_IN_SERVICE = 11001,         //平台服务器未进入服务态
    ERR_CMU_SIP_BODY_NULL = 11002,          //SIP消息体为空
    ERR_CMU_PUI_ALREADY_REG = 11003,        //相同的PUI已经注册
    ERR_CMU_CUI_ALREADY_REG = 11004,        //相同的CUI已经注册
    ERR_CMU_PU_NOT_IN_TOP = 11005,          //pu未入网
    ERR_CMU_PU_NOT_USED = 11006,            //pu未启用
    ERR_CMU_PU_EXPIRE = 11007,              //pu已经过期
    ERR_CMU_PU_ALREADY_REG = 11008,         //相同的PU已经注册
    ERR_CMU_PU_LICENSE_NOT_MATCH = 11009,   //PU的license不匹配
    ERR_CMU_CU_LOGIN_SESS_ILLEGAL = 11010,  //CU操作中登录Session非法
    ERR_CMU_SS_OPE_TYPE_INVALID = 11011,    //CU更新订阅的操作类型无效
    ERR_CMU_CUI_NOT_REG = 11012,            //CUI未向CMU注册
    ERR_CMU_PUI_NOT_REG = 11013,            //PUI未向CMU注册
    ERR_CMU_VTDU_ALREADY_REG = 11014,       //相同的VTDU已经注册
    ERR_CMU_DEV_NO_ROUTER = 11015,          //设备无法路由
    ERR_CMU_DEV_RSP_TIMEOUT = 11016,        //设备应答超时
    ERR_CMU_CMU_ALREADY_CONN = 11017,       //相同的CMU已经连接
    ERR_CMU_CMU_NOT_CONN = 11018,           //CMU未连接
    ERR_CMU_CMU_INVALID_CMU_URI = 11019,    //无效CMU URI
    ERR_CMU_PU_NOT_ONLINE = 11020,          //PU不在线
    ERR_CMU_USER_PTZ_PRI_LOW = 11021,       //用户PTZ控制权限低
    ERR_CMU_NOT_SUPPORT = 11022,            //该功能不支持
    ERR_CMU_ALARM_ILLEGAL = 11023,          //不合理的告警
    ERR_CMU_STOP_BY_BYE = 11024,            //对话被挂断
    ERR_CMU_DOMAIN_NOT_IN_TOP = 11025,      //Domian未组网
    ERR_CMU_DEV_ADDR_INVALID = 11026,       //设备地址无效
	ERR_CMU_SUBDOMAIN_OFFLINE = 11027,      //下级域不在线
	ERR_CMU_DIALOG_TERMINATE = 11028,       //CMU对话被中断
	ERR_CMU_DIALOG_TERMINATE_BY_CUI = 11029,  //CMU对话被CUI中断
	ERR_CMU_DIALOG_TERMINATE_BY_VTDU = 11030, //CMU对话被VTDU中断
	ERR_CMU_DIALOG_TERMINATE_BY_PUI = 11031,  //CMU对话被PUI中断
	ERR_CMU_DIALOG_TERMINATE_BY_GBS = 11032,  //CMU对话被GBS中断
	ERR_CMU_DIALOG_TERMINATE_BY_SUBDOMAIN_CMU = 11033,  //CMU对话被下级CMU中断
    ERR_CMU_SS_TYPES_INVALID = 11034,                   //订阅类型无效
    ERR_CMU_BATCH_REG_TYPE_INVALID = 11035,             //批量注册中的type无效
    ERR_CMU_UNCOMPATIBLE_REQ = 11036,             //不兼容的请求
    ERR_CMU_NOT_SERVICE_TEMPORARILY = 11037,      //临时不可接受服务
    ERR_CMU_DATA_UNAVAILABLE_TEMPORARILY = 11038,   //数据临时不可用

    //{{SWITCH=11500
    ERR_CMU_SWITCH_DEV_NOT_AVAILABLE = 11500,       //参与交换的设备不在线
    //ERR_CMU_SWITCH_DST_NOT_AVAILABLE = 11501,       //交换目的不在线
    //ERR_CMU_SWITCH_SRC_NOT_AVAILABLE = 11502,       //交换源不在线
    ERR_CMU_SWITCH_DST_EXIST_SWITCH= 11503,         //交换目的存在交换
	ERR_CMU_SWITCH_SRC_EXIST_SWITCH= 11504,         //交换源存在交换
    //ERR_CMU_SWITCH_POST_MSG_TO_SRC_FAILED= 11505,   //交换时发送消息给源失败
    //ERR_CMU_SWITCH_POST_MSG_TO_VTDU_FAILED= 11506,  //交换时发送消息给目的失败
    //ERR_CMU_SWITCH_POST_MSG_TO_DST_FAILED= 11507,   //交换时发送消息给VTDU失败
    ERR_CMU_SWITCH_SRC_NODE_NOT_EXIST = 11508,      //交换源节点不存在
    ERR_CMU_SWITCH_DST_NODE_NOT_EXIST = 11509,      //交换目的节点不存在
    ERR_CMU_SWITCH_INVALID_DIALOG_ID = 11510,       //无效的交换会话ID,会话还未建立或已经销毁
    ERR_CMU_SWITCH_ADDRESS_RULE_SWITCH_FAILED = 11511,      //交换时转为地址规则失败
    ERR_CMU_SWITCH_VTDU_NOT_AVAILABLE = 11512,      //找不到可用的VTDU
    ERR_CMU_SWITCH_GAP_VTDU_NOT_AVAILABLE = 11513,  //找不到可用的网闸VTDU
    ERR_CMU_SWITCH_VTDU_PORT_ALLOC_FAILED = 11514,  //VTDU通道或端口分配失败
    ERR_CMU_SWITCH_EXCEPTION = 11515,	            //交换时发生诊断性异常
    ERR_CMU_SWITCH_ADD_BAK_SRC_FAILED = 11516,	    //交换时添加新的备份源交换失败
    ERR_CMU_SWITCH_STOP_SWITCH_FOR_NEW_BAK_SRC_FAILED = 11517,	    //交换时为构建新的备份源停止原有交换失败
    ERR_CMU_SWITCH_AGAIN = 11518,                   //资源临时不可得（如VTDU下线等），请重试
    ERR_CMU_SWITCH_TASK_TIMEOUT = 11519,            //CMU交换任务超时
    ERR_CMU_SWITCH_STOP_BY_BYE = 11520,             //交换对话被挂断
    ERR_CMU_SWITCH_VCR_FAILED = 11521,              //VCR控制失败
    ERR_CMU_SWITCH_DLG_RES_NOT_ENOUGH = 11522,      //交换会话资源不足
    //}}SWITCH

    
    ERR_CMU_END = ERR_CMU_BEGIN + 999,      //cmu错误码结束值

    //pui错误码 12000-12999
    ERR_PUI_BEGIN = 12000,                  //pui错误码起始值

    ERR_PUI_PU_ALREAD_EXIST = 12001,        //相同的PU已经注册
    ERR_PUI_PU_OVERLORD = 12002,            //pui接入pu数过载
    ERR_PUI_SIP_BODY_NULL = 12003,          //SIP消息体为空
    ERR_PUI_NOT_IN_SERVICE = 12004,         //PUI未进入服务态
    ERR_PUI_ADD_SWITCH_FAIL = 12005,        //PUI添加交换失败
    ERR_PUI_GET_CAM_PROTO_FAIL = 12006,     //PUI获取摄像头控制协议失败
    ERR_PUI_GET_PU_CONFIG_FAIL = 12007,     //PUI获取前端配置失败
    ERR_PUI_SET_PU_CONFIG_FAIL = 12008,     //PUI设置前端配置失败
    ERR_PUI_PU_NOT_ONLINE = 12009,          //PU不在线
    ERR_PUI_PU_OPE_TIMEOUT = 12010,         //PU操作超时
    ERR_PUI_PU_OPE_FAIL = 12011,            //PU操作失败
    ERR_PUI_PU_TYPE_ILLEGAL = 12012,        //PU类型非法
    ERR_PUI_PU_NODISK = 12013,              //前端无硬盘
    ERR_PUI_PU_RECORDING = 12014,           //正在录像或预录
    ERR_PUI_PU_RECPLAYING = 12015,          //正在放像
    ERR_PUI_PU_FILE_NOT_EXIT = 12016,		//文件不存在(该路径下无此文件)
    ERR_PUI_PU_RECLENGTH_ZERO = 12017,		//文件大小为零(录象时无码流)
    ERR_PUI_PU_RECLOG_NOT_EXIST = 12018,    //reclog文件不存在
    ERR_PUI_PU_ASF_STATE_ERROR = 12019,     //播放ASF文件状态不正确
    ERR_PUI_PU_PATH_CREATE_FAIL = 12020,    //目录创建失败,请检查是否磁盘只读
    ERR_PUI_PU_NO_PIC_FILE = 12021,         //无前端抓拍图片
    ERR_PUI_PU_PIC_FILE_TOO_MUCH = 12022,   //前端抓拍图片过多
    ERR_PUI_NVR_NO_BUF_RECORD = 12023,                      //录像缓存记录不存在,请重新搜索录像(NVR前端放像时)
    ERR_PUI_NVR_INVALID_PU_RECCHN = 12024,                  //录像通道不可用，一般是此NVR通道上未接入设备(NVR前端录像时)
    ERR_PUI_NVR_NOT_ALLOW_DELETE_PU_REC = 12025,            //无删除NVR前端录像权限
    ERR_PUI_NVR_BITRATE_OVERFLOW = 12026,                   //比特率过大(NVR2820 BitRate:64 - 2048)
    ERR_PUI_NVR_PIC_TOO_MUCH = 12027,                       //NVR前端抓拍图片数过多，请缩小搜索范围
    ERR_PUI_NVR_NO_SNAPSHOT_PIC = 12028,                    //无NVR前端抓拍图片
    ERR_PUI_NVR_NO_VALID_REC_DISK = 12029,                  //NVR无可用录像磁盘分区
    ERR_PUI_PU_NOT_SUPPORT = 12030,                         //该前端不支持此功能
    ERR_PUI_PU_VCR_NOT_SUPPORT_PLAY_RATE = 12031,           //前端不支持的VCR播放速率
    ERR_PUI_PU_VCR_NOT_SUPPORT_CMD = 12032,                 //前端不支持的VCR控制命令
    ERR_PUI_QUERY_MAPPED_ADDR_FAILED = 12033,               //查询外网地址失败
    ERR_PUI_QUERY_MAPPED_ADDR_TIMEOUT = 12034,              //查询外网地址超时
    ERR_PUI_QUERY_MAPPED_ADDR_CANCELED = 12035,             //查询外网地址操作被取消
    ERR_PUI_PUNCH_FAILED = 12036,                           //地址探测失败
    ERR_PUI_PUNCH_TIMEOUT = 12037,                          //地址探测超时
    ERR_PUI_PUNCH_CANCELED = 12038,                         //地址探测操作被取消
    ERR_PUI_EXCEED_PLAY_REC_MAX_NUM = 12039,                //超过前端最大放像数目
    ERR_PUI_GET_LOCAL_ADDR_FAILED = 12040,                  //获取本地地址失败
    ERR_PUI_MEDIA_TYPE_ERROR = 12041,                       //错误的媒体格式
    ERR_PUI_PU_VCR_TRY_AGAIN = 12042,                       //正在开启前端放像，稍后进行VCR控制
    ERR_PUI_TRANS_OPR_TYPE_ERROR = 12043,                   //透明数据操作类型错误
	ERR_PUI_OSDBMP_SIZE_ERROR = 12044,                      //OSDBMP的大小大于最大MTI缓存
	ERR_PUI_OSDFONT_WIDTH_ERROR = 12045,                    //字符宽度大于MTI最大缓存
	ERR_PUI_UNKNOW_MTI_AUDIO_TYPE_ERROR = 12046,            //获取的MTI音频格式CMU不支持 
	ERR_PUI_UNKNOW_CMS_AUDIO_TYPE_ERROR = 12047,            //接收到的CMU音频格式MTI不支持
	ERR_PUI_LONG_OSDCAPTION_SIZE_ERROR = 12048,             //long osd字母长度超过限制大小
	ERR_PUI_GET_SNAPSHOT_CFG_FAIL = 12049,                  //获取前端抓拍参数失败
	ERR_PUI_SET_SNAPSHOT_CFG_FAIL = 12050,                  //设置前端抓拍参数失败
	ERR_PUI_GET_SERIALPORT_CFG_FAIL = 12051,                //获取串口参数失败
	ERR_PUI_SET_SERIALPORT_CFG_FAIL = 12052,                //设置串口参数失败
	ERR_PUI_PU_CPU_OVERLOAD = 12053,                        //前端超负载
	ERR_PUI_NOT_AUTHORIZED = 12054,                         //没有权限
	ERR_PUI_CRITICAL_ERROR = 12055,                         //关键性错误，需要重启服务
	ERR_PUI_NO_PROFILE     = 12056,                         //没有profile（onvif前端）
	ERR_PUI_PU_PLTFM_FLOWMAX = 12057,                       //3g流量超过最大值
    ERR_PUI_TASK_NOT_FOUND  = 12058,                        //事务未找到
    ERR_PUI_PU_UNKNOWN = ERR_PUI_BEGIN + 998,               //前端未知错误
    ERR_PUI_END = ERR_PUI_BEGIN + 999,      //pui错误码结束值

    //cui错误码 13000-13999 当前可用号 13030
    ERR_CUI_BEGIN = 13000,                  //cui错误码起始值

    ERR_CUI_USERNAME_OR_PASSWORD_WRONG = 13001,  //用户名或者密码错误
    ERR_CUI_CONNECT_CUI_FAIL = 13002,            //无法连接上CUI服务器
    ERR_CUI_DISCONNECT_CUI = 13003,              //和CUI断开连接
    ERR_CUI_NOT_IN_SERVICE = 13004,              //CUI未进入服务态
    ERR_CUI_OPR_TIME_OUT = 13005,                //请求超时
    ERR_CUI_CU_NOT_ONLINE = 13006,               //CU不在线，请求非法
    ERR_CUI_CU_NO_PERMISSION = 13007,            //CU没有权限
    ERR_CUI_QUERYING_PRIVILEGE_DATA = 13026,     //正在查询权限数据，请稍后再试
    ERR_CUI_QUERYING_INNER_DATA = 13027,         //正在查询内部数据，请稍后再试
    ERR_CUI_DIALOG_NOT_ESTABLISHED = 13008,      //会话尚未建立，操作被拒绝
    ERR_CUI_NOT_SUBSCRIBE = 13009,               //设备未订阅过
    ERR_CUI_PARAMS_INVALID = 13010,              //参数非法，请检查
    ERR_CUI_DIALOG_TERMINATED = 13011,           //CUI会话被对端主动结束
    ERR_CUI_SIP_MSG_FAIL = 13012,                //发送SIP请求失败，可能服务器不在线
    ERR_CUI_TAS_OFFLINE = 13013,                 //TAS服务器不在线，操作失败
    ERR_CUI_INVALID_PARAM = 13014,               //无效请求参数
	ERR_CUI_IN_SUBSCRIBE = 13015,                //正在订阅中
    ERR_CUI_UNKNOW_MSG = 13016,                  //不认识的消息
    ERR_CUI_TASK_DEL = 13017,                    //task消亡
    ERR_CUI_TCP_TIMEOUT = 13018,                 //tcp端接收消息失败
    ERR_CUI_PE_OFFLINE = 13019,                  //外设模块不在线
    ERR_CUI_RECORD_END = 13020,                  //录像播放停止
    ERR_CUI_PASSWORD_ERROR = 13021,              //密码校验错误
    ERR_CUI_EXCEED_CU_MAX_NUM = 13022,           //超出最大cu用户数
    ERR_CUI_CROSS_DOMAIN_LOGIN = 13023,          //暂不允许跨域登录
    ERR_CUI_STATE_ERROR = 13024,                 //任务状态不正确
    ERR_CUI_RSP_MESSAGE_TOO_LONG = 13025,        //应答消息体过长
	ERR_CUI_SUBSCRIBE_SERVER_NOT_ONLINE = 13028, //订阅服务器不在线
	ERR_CUI_EXCEED_MOBILE_CU_MAX_NUM = 13029,    //超出最大移动客户端数阈值
	ERR_CUI_DIALOG_TERMINATED_BY_CMU = 13030,    //CUI会话被CMU主动结束
	ERR_CUI_ADD_SWITCH_FAILED = 13031,           //CUI建交换失败
	ERR_CUI_DIALOG_TERMINATED_BY_RCS = 13032,    //CUI会话被RCS主动结束
	ERR_CUI_PU_PTZ_LOCKED = 13033,				 //前端PTZ已被锁定
	ERR_CUI_GET_REDIS_DATA_FAILED = 13034,       //获取数据失败
	ERR_CUI_USER_EXCEEDED_MAX_STREAM = 13035,    //用户超过最大并发数
	ERR_CUI_CU_VERSION_UNSUPPORTIVE = 13036,     //不支持此用户版本
	ERR_CUI_TASK_NOT_SUPPORT = 13037,            //不支持此业务
	ERR_CUI_REDIS_SUBS_FAILED = 13038,           //redis订阅失败
	ERR_CUI_CU_REQ_NUM_EXCEEDED = 13039,		 //请求的数量超限
    ERR_CUI_END = ERR_CUI_BEGIN + 999,      //cui错误码结束值

    //vtdu错误码 14000-14999
    ERR_VTDU_BEGIN = 14000,                 //vtdu错误码起始值

    ERR_VTDU_ADD_SWITCH_TO_DS_FAILED = 14001,       //[VTDU]添加交换到DataSwitch失败
    ERR_VTDU_REMOVE_SWITCH_FROM_DS_FAILED = 14002,  //[VTDU]从DataSwitch删除交换失败
	ERR_VTDU_ADD_SWITCH_INVALID_ARGUMENT = 14003,   //[VTDU]添加交换请求的参数不对
    ERR_VTDU_TASK_TIMEOUT = 14004,                  //[VTDU]任务处理超时
    ERR_VTDU_EXCEPTION = 14005,                     //[VTDU]系统异常
    ERR_VTDU_WAIT_ACK_TIMEOUT = 14006,              //[VTDU]等待确认应答超时
    ERR_VTDU_STUN_UPDATE_FAILED = 14007,            //[VTDU]更新Stun地址失败
    ERR_VTDU_WITHOUT_VTDU_ADDR_OF_NETSEG = 14008,   //[VTDU]没有相应网段的VTDU地址
    ERR_VTDU_UPDATE_SWITCH_REMOVE_FAILED = 14009,   //[VTDU]更新交换地址时删除老交换失败
    ERR_VTDU_UPDATE_SWITCH_ADD_FAILED = 14010,      //[VTDU]更新交换地址时添加新交换失败
    ERR_VTDU_UPDATE_SWITCH_DUMP_FAILED = 14011,     //[VTDU]更新交换地址时添加Dump失败

    ERR_VTDU_QUERY_MAPPED_ADDR_FAILED = 14500,      //[VTDU]查询外网地址失败
    ERR_VTDU_QUERY_MAPPED_ADDR_TIMEOUT = 14501,     //[VTDU]查询外网地址超时
    ERR_VTDU_QUERY_MAPPED_ADDR_CANCELED = 14502,    //[VTDU]查询外网地址操作被取消

    ERR_VTDU_PUNCH_FAILED = 14520,                  //[VTDU]地址探测失败
    ERR_VTDU_PUNCH_TIMEOUT = 14521,                 //[VTDU]地址探测超时
    ERR_VTDU_PUNCH_CANCELED = 14522,                //[VTDU]地址探测操作被取消
    ERR_VTDU_INVALID_PUNCH_PARAM = 14523,           //[VTDU]无效的Punch参数
    ERR_VTDU_INVALID_SWITCH_RESOURCE = 14524,       //[VTDU]无效的交换资源
    ERR_VTDU_INVALID_SWITCH_OWNER = 14525,          //[VTDU]交换中的Owner无效
    ERR_VTDU_CACULATE_PORT_FAILED = 14526,          //[VTDU]计算端口出错
    ERR_VTDU_REDIS_FORWARD_REQUEST_FAILED = 14527,  //[VTDU]redis记录交换请求出错
    ERR_VTDU_REDIS_FORWARD_RESPONSE_FAILED = 14528, //[VTDU]redis记录交换应答出错
    ERR_VTDU_REDIS_FORWARD_ACKNOWLEDGE_FAILED = 14529,//[VTDU]redis记录确认交换出错
    ERR_VTDU_REDIS_FORWARD_DESTORY_FAILED = 14530,  //[VTDU]redis记录拆除交换出错
	ERR_VTDU_SWITCH_VCR_FAILED = 14531,             //[VTDU]VCR控制失败


    ERR_VTDU_END = ERR_VTDU_BEGIN + 999,    //vtdu错误码结束值

    //pmcs错误码 15000-15999
    ERR_PMCS_BEGIN = 15000,

    ERR_PMCS_NOT_IN_SERVICE = 15001,         //pmcs未进入服务态
    ERR_PMCS_OPR_TIME_OUT = 15002,          //业务请求超时
    ERR_PMCS_WEB_NOT_ONLINE = 15003,          //pmcs web 不在线,请求非法
    ERR_PMCS_DIALOG_TERMINATED = 15004,        //与pms会话被对端主动结束
    ERR_PMCS_SIP_MSG_FAIL = 15005,               //发送SIP请求失败，可能服务器不在线
    ERR_PMCS_DISCONNECT_PMCS = 15006,             //和PMCS断开连接
	ERR_PMCS_PMS_NOT_IN_SERVICE = 15007,          //PMS未进入服务
	ERR_PMCS_3AS_NOT_IN_SERVICE = 15008,      //3as未进入服务
	ERR_PMCS_RCS_NOT_IN_SERVICE = 15009,					//rcs不在服务态
	ERR_PMCS_RCS_SUBSCRIBE_ALREADY_EXIST = 15010,		//订阅已存在
	ERR_PMCS_RCS_SUBSCRIBE_NOT_EXIST = 15011,			//尚未订阅
	ERR_PMCS_PMCAGENT_NOT_EXIST = 15012,		//PMCAGENT不存在
	ERR_PMCS_PMC_NOT_IN_SERVICE = 15013,		//PMC异常，拒绝服务
    ERR_PMCS_PMS_PLATFORM_RELATION_ERROR = 15014,    //平台关系填错
    ERR_PMCS_PMS_OPT_ERROR = 15015,                  //业务处理失败
    ERR_PMCS_PMS_SUBPLATFORM_ADD_OPT_ERROR = 15016,  //添加从平台或下级平台时从平台或下级平台处理失败，且主平台或上级平台有残留
    ERR_PMCS_PMS_SUBPLATFORM_DEL_OPT_ERROR = 15017,  //删除从平台或下级平台时，主平台或上级平台处理失败，从平台或下级平台已处理
    ERR_PMCS_DETECTED_NO_TOPO_INFO = 15018,          //未探测到正确到topo平台
    ERR_PMCS_NAT_DATA_ERR = 15019,                   //映射数据有误，不能完成组网
    ERR_PMCS_GBS_NOT_IN_SERVICE = 15020,             //GBS未进入服务态
    ERR_PMCS_IMPORT_FILE_FAIL = 15021,               //设备批量入网失败
    ERR_PMCS_IMPORT_DEV_INFO_ERR = 15022,            //批量导入设备文件模板错误
    ERR_PMCS_GET_DEV_MOD_FAIL = 15023,        //获取设备型号失败
    ERR_PMCS_DEV_IMPORT_FILE_NOT_EXIST   = 15024,    //批量导入设备文件不存在
    ERR_PMCS_DEV_MODEL_EMPTY             = 15025,    //没有查到设备型号
    ERR_PMCS_QRY_DEV_FAIL                = 15026,    //查询所有设备失败
    ERR_PMCS_QRY_GBDEV_FAIL              = 15027,    //查询国标设备失败
    ERR_PMCS_QRY_DEVICE_EMPTY            = 15028,    //查询到0个设备
	ERR_PMCS_UNZIP_FAIL                  = 15029,    //解压缩失败
	ERR_PMCS_MPS_NOT_IN_SERVICE          = 15030,    //MPS未进入服务态
	ERR_PMCS_UNZIP_FILE_TOO_MUCH         = 15031,    //当前正解压缩文件已达到限定数量3个
	//upload_fcgi 错误码，加到pmcs中
	ERR_UPLOAD_FCGI_OPEN_FILE_FAIL       = 15032,    //打开文件失败
	ERR_UPLOAD_FCGI_NOBOUNDARY           = 15033,    //没找到边界
	ERR_UPLOAD_FCGI_UNCOMPLETE           = 15034,    //文件不完整
	ERR_UPLOAD_FCGI_WRITE_FILE_FAIL      = 15035,    //写入文件失败
	//upload_fcgi 错误码 结束
	ERR_PMCS_IP_IS_LOCAL_IP              = 15036,    //ip不能为本地ip


    ERR_PMCS_END = ERR_PMCS_BEGIN + 999,    //pmcs错误码结束

	// CU错误码 16000 - 16999 
	ERR_CU_BEGIN = 16000,						// cu错误码段开始

	ERR_CU_END = ERR_CU_BEGIN + 999,	// cu错误码结束。

    //als错误码 17000-17999
    ERR_ALS_BEGIN = 17000,                  //als错误码起始值

    ERR_ALS_INVALID_ALARM_LINK = 17001,     //无效的告警联动配置
    ERR_ALS_DB_OPR_FAIL = 17002,            //数据库操作失败
    ERR_ALS_DB_OPR_TIMEOUT = 17003,         //数据库操作超时
    ERR_ALS_ALARM_LINK_NOT_EXIST = 17004,   //告警联动不存在

    ERR_ALS_END = ERR_ALS_BEGIN + 999,      //als错误码结束值

    //tvs错误码 18000-18999
    ERR_TVS_BEGIN = 18000,                  //tvs错误码起始值
    ERR_TVS_TVWALL_NOT_EXIST = 18001,       //电视墙不存在
    ERR_TVS_TV_PLAY_NOT_EXIST = 18002,      //电视播放不存在
    ERR_TVS_TV_OCCUR_NOT_MATCH = 18003,     //电视触发类型不匹配
    ERR_TVS_TV_SRC_NOT_MATCH = 18004,       //电视播放源不不匹配
    ERR_TVS_3PC_INVITE_FAIL = 18005,        //3PC呼叫失败，sip层错误
    ERR_TVS_3PC_INVITE_TIMEOUT = 18006,     //3PC呼叫超时
    ERR_TVS_TV_POLL_OPR_FAIL = 18007,       //轮询操作失败
    ERR_TVS_DB_OPR_FAIL = 18008,            //数据操作失败
    ERR_TVS_DB_OPR_TIMEOUT = 18009,         //数据操作超时
    ERR_TVS_ILLEGAL_SCHEME_NAME = 18010,    //无效的预案名称
    ERR_TVS_ILLEGAL_SCHEME_ID = 18011,      //无效的预案ID
    ERR_TVS_SCHEME_NOT_EXIST = 18012,       //预案不存在
    ERR_TVS_CUI_LOGIN_SESS_ILLEGAL = 18013, //非法的cui登录session
    ERR_TVS_TV_DEC_BIND_ERROR = 18014,      //电视机绑定解码器错误
    ERR_TVS_ILLEGAL_ENC_CHN = 18015,        //无效的编码通道
    ERR_TVS_ILLEGAL_SHEME_POLL_CFG = 18016, //无效的预案轮询配置
    ERR_TVS_SET_DEC_STYLE_TIMEOUT = 18017,  //设置解码器风格超时
    ERR_TVS_NOT_LOGIN_CMU = 18018,          //未登录cmu
    ERR_TVS_LOGIN_CMU_BYE_3PC = 18019,      //cmu主动bye 3PC呼叫
    ERR_TVS_3PC_DIALOG_TERMINATE = 18020,   //3PC呼叫异常终止，sip层错误
	ERR_TVS_LOCKED_TVWALL_OR_CHANNEL_NOT_EXIST = 18021,   //不存在的锁定电视通道或者锁定电视墙预案
	ERR_TVS_UNLOCK_TVWALL_OR_CHANNEL_PASS_ERR = 18022,   //解锁电视通道或者预案密码错误
	ERR_TVS_INVALID_WATCHOINT_SAVE_REQ = 18023, //无效的观看点备份请求
	ERR_TVS_SAVED_WATCHPOINT_NOT_EXIST = 18024, //不存在的备份观看点
	ERR_TVS_TVWALL_OR_CHANNEL_LOCKED = 18025, //电视墙预案或者通道被锁
	ERR_TVS_TVWALL_TVCHN_TASK_EXIST = 18026, //电视通道已占用
    ERR_TVS_TVWALL_TIMEOUT = 18027,          //操作超时
    ERR_TVS_TV_SAME_DIV_OCCUR_TYPE = 18028,     // 相同的窗口 操作类型
    ERR_TVS_SAME_ENC_CHANNEL_IN_USE= 18029,     // 相同的视频视频通道已使用中
    ERR_TVS_ERR_CONFIG_DISABLE_ALARM_OFF_WALL = 18030, // TVS 配置成 拒绝 告警下墙
    ERR_TVS_DIV_EXEC_TASK_NOT_FOUND   = 18031,  // 未找到指定的 分屏 轮询任务
    ERR_TVS_END = ERR_TVS_BEGIN + 999,      //tvs错误码结束值

    //rcs错误码 19000-19999
    ERR_RCS_BEGIN = 19000,                  //rcs错误码起始值
    ERR_RCS_RMS_NOT_ONLINE = 19001,             //[RCS]rms不在线
    ERR_RCS_NRU_NOT_ONLINE = 19002,             //[RCS]nru不在线
    ERR_RCS_DB_OPR_FAIL = 19003,                //[RCS]数据库操作失败
    ERR_RCS_DB_OPR_TIMEOUT = 19004,             //[RCS]数据库操作超时
    ERR_RCS_PU_NOT_ONLINE = 19005,              //[RCS]前端设备不在线
    ERR_RCS_PU_CONFIG_NOT_AVAILABLE = 19006,    //[RCS]前端配置参数不可得
    ERR_RCS_RECORD_PARAM_NOT_EXIST = 19007,     //[RCS]录像参数不存在
    ERR_RCS_RECORD_TASK_ALREADY_EXIST = 19008,  //[RCS]录像任务已经存在
    ERR_RCS_EXCEPTION = 19009,                  //[RCS]服务器异常
    ERR_RCS_ALLOC_RECORD_RES_FAILED = 19010,    //[RCS]分配录像资源失败
    ERR_RCS_START_RECORD_FAILED = 19011,        //[RCS]开启录像失败
    ERR_RCS_RECORD_TASK_NOT_EXIST = 19012,      //[RCS]不存在相应的录像任务
    ERR_RCS_UPDATE_RECTYPE_FAILED = 19013,      //[RCS]更新录像类型失败
    ERR_RCS_DEV_NOT_REGISTER = 19014,           //[RCS]设备未注册
    ERR_RCS_TASK_TIMEOUT = 19015,               //[RCS]任务处理超时
    ERR_RCS_NOT_REG_TO_CMU = 19016,             //[RCS]未注册上CMU
    ERR_RCS_PLAYBACK_TASK_NOT_EXIST = 19017,    //[RCS]放像任务不存在
    ERR_RCS_DEV_NOT_EXIST = 19018,              //[RCS]设备不存在
    ERR_RCS_SS_OPE_TYPE_INVALID = 19019,        //[RCS]无效的订阅类型
    ERR_RCS_DEV_NO_ROUTER = 19020,              //[RCS]请求无法被路由到指定设备
    ERR_RCS_STOP_BY_BYE = 19021,                //[RCS]对话被挂断
    ERR_RCS_NOT_SUPPORT = 19022,                //[RCS]该功能不支持
    ERR_RCS_CONVERT_URI_DOMAIN_FAILED = 19023,  //[RCS]转换URI的域名失败，可能找不到相应的域信息
    ERR_RCS_VCR_TRY_AGAIN = 19024,              //[RCS]正在开启录像回放，稍后进行VCR控制
	ERR_RCS_REPEAT_SS = 19025,                  //[RCS]重复订阅
	ERR_RCS_FOREIGN_DOMAIN_SS = 19026,          //[RCS]不支持跨域订阅
	ERR_RCS_CHANNEL_UNAVALIABLE = 19027,        //[RCS]通道不可用
	ERR_RCS_REC_CANNOT_STOPPED_MANUALLY = 19028,//[RCS]周期录像或告警录像不能手动停止
	ERR_RCS_NRU_NOT_EXIST = 19029,				//[RCS]nru不存在
	ERR_RCS_DISK_PART_TOO_BIG = 19030,			//[RCS]不支持超过16T的磁盘
	ERR_RCS_DIALOG_TERMINATE = 19031,           //[RCS]对话被中断
	ERR_RCS_INVALID_PARAM = 19032,              //[RCS]无效参数
	ERR_RCS_RESOURCE_CALL_FAIL = 19033,         //[RCS]资源调度失败

    ERR_RCS_END = ERR_RCS_BEGIN + 999,          //rcs错误码结束值

    //uas错误码 20000-20999 (当前可用号为20006)
    ERR_UAS_BEGIN = 20000,                  //uas错误码起始值
    ERR_UAS_DBERROR = 20001,                // 数据库操作出错，请重试一次
    ERR_UAS_SAVE_CONFIG_FAILED = 20002,     // 保存配置失败
	ERR_UAS_TIMEOUT = 20003,                // 处理超时
	ERR_UAS_PROC_FAIL = 20004,              // 处理失败
	ERR_UAS_UNKNOWN_TYPE = 20005,           // 未知类型
    ERR_UAS_END = ERR_UAS_BEGIN + 999,      //uas错误码结束值

    //mps错误码 21000-21999
    ERR_MPS_BEGIN = 21000,                  //mps错误码起始值
	ERR_MPS_INVAILD_MSG  = 21001,			//错误的消息体
	ERR_MPS_MSG_TIME_OUT	 = 21002,		//消息处理超时
	ERR_MPS_DB_CONNECT_ERROR = 21003,		//连接数据库失败
	ERR_MPS_DB_OPR_ERROR	= 21004,		//操作数据库失败
	ERR_MPS_DB_OPR_TIMEOUT	= 21005,		//操作数据库超时
	ERR_MPS_DB_NO_RESULT	= 21006,		//数据查询返回无内容
	ERR_MPS_MAP_EXISTS		= 21007,		//地图已存在
	ERR_MPS_MAP_ELEMENT_EXISTS	= 21008,	//图元已存在
	ERR_MPS_ICON_PATH_NOT_EXISTS = 21009,	//地图图元目录不存在
	ERR_MPS_PARAMS_INVAILD		= 21010,	//参数不合法
	ERR_MPS_POINT_OUT_OF_RANG	= 21011,	//校正点在范围之外
	ERR_MPS_NOT_CONFIG_GPS_STORE	= 21012,	//未配置GPS存储
	ERR_MPS_GOOGLE_MAIN_MAP_DEL_ILLEGAL	= 21013,	//Google主地图不允许删除
    ERR_MPS_END = ERR_MPS_BEGIN + 999,      //mps错误码结束值

    //pms错误码 22000-22999 当前可用错误码为22080
    ERR_PMS_BEGIN = 22000,                  //pms错误码起始值
    ERR_PMS_INVALID_PARAM = 22012,          //无效的请求参数
    ERR_PMS_DATA_NOT_EXIST = 22019,         //数据不存在
    ERR_PMS_NOT_IN_SERVICE = 22001,         //PMS未进入服务态
    ERR_PMS_INVALID_OPERATION = 22020,      //非法操作，业务上不允许如此操作
    ERR_PMS_EXECUTE_TIMEOUT = 22039,        //执行超时

    ERR_PMS_3AS_NOT_IN_SERVICE = 22002,     //3as未进入服务态
    ERR_PMS_INVALID_PMC_URI = 22003,        //无效的pmc uri
    ERR_PMS_PMS_ALREADY_CONN = 22004,       //重复登录
    ERR_PMS_DB_OPR_FAIL = 22005,            //数据库操作错误
    ERR_PMS_USERNAME_OR_PASSWORD_WRONG = 22006, //用户名或密码错误
    ERR_PMS_INVALID_SERVER_URI = 22007,         //无效的SERVER URI
    ERR_PMS_DB_OPR_TIMEOUT = 22008,         //数据库操作超时
	ERR_PMS_WEB_ONT_ONLINE = 22009,			//WEB不在线
	ERR_PMS_PMS_UNKNOW_DEVTYPE = 22010,     //无效的设备类型
	ERR_PMS_DB_DATA_NOT_EXIST = 22011,		//数据库中不存在要找的数据
    ERR_PMS_OPR_FAIL = 22015,               //操作失败
    
    ERR_PMS_PLATFORM_ALREADY_IN_TOPO = 22014,//待入网平台已经参与组网，请先退出再组网
    ERR_PMS_REPEATED_PLATFORM_RELATION = 22021, //重复的平台关系
    ERR_PMS_GET_MOSTADAPTIVEIP_FAILED = 22051,  //获取网段最长匹配IP失败, 请检查是否需要配置NAT映射
    ERR_PMS_DETECTED_NOT_CONSISTENCE_TOPO_CONF = 22017, //探测到不一致的TOPO配置 
    ERR_PMS_MODIFY_CONFIG_FAILED = 22016,               //修改PMS的配置文件失败
    ERR_PMS_MODIFY_COMMONCONFIG_FAILED = 22018,         //修改公共配置文件失败

    ERR_PMS_GET_ROUTEINFO_ERR = 22022,              //获取网络路由信息失败
    ERR_PMS_SET_ROUTEINFO_ERR = 22023,              //设置网络路由失败
    ERR_PMS_DEL_ROUTEINFO_ERR = 22024,              //删除网络路由失败
    ERR_PMS_ADD_ROUTEINFO_ERR = 22025,              //添加网络路由失败
    ERR_PMS_RESET_ERR = 22026,                      //还原出厂设置失败
    ERR_PMS_SOFTWAREINFO_QRY_ERR = 22027,           //软件信息获取失败
    ERR_PMS_SOFTWAREINFO_MOD_ERR = 22028,           //软件信息修改失败
    ERR_PMS_GET_DEFAULTROUTE_ERR = 22029,           //获取默认网关失败
    ERR_PMS_SET_DEFAULTROUTE_ERR = 22030,           //设置默认网关失败
    ERR_PMS_MOD_PLATFORMTIME_ERR = 22031,           //修改平台时间失败
    ERR_PMS_CTRL_PMS_ERR = 22032,                   //PMS平台控制失败
    ERR_PMS_MOD_NETCARDINFO_ERR = 22033,            //修改网卡信息失败
    ERR_PMS_QRY_PLATFORMTIME_ERR = 22034,           //查询平台时间失败
    ERR_PMS_ADD_NETNAT_ERR = 22035,                 //添加网络NAT失败
    ERR_PMS_DEL_NETNAT_ERR = 22036,                 //删除网络NAT失败
    ERR_PMS_ADD_REPEATED_DATA_ERR = 22037,          //重复添加数据
    ERR_PMS_NETWORK_RESTART_ERR = 22038,            //网络服务重启失败
    ERR_PMS_INVTASK_LIMITED = 22040,                //升级、镜像导入、恢复出厂设置同时只能存在一个操作,若有两个以上相应请求则错误
    ERR_PMS_ADD_USER_ALREADY_EXIST = 22041,         //添加的用户已经存在
    ERR_PMS_MOD_USER_NOT_EXIST = 22042,             //要修改的用户不存在
    ERR_PMS_DEL_NTP_SERVER_ERR = 22043,             //删除NTP服务失败
    ERR_PMS_SET_NTP_SERVER_ERR = 22045,             //设置NTP服务失败
    ERR_PMS_REMOVE_USBBAK_FAILED = 22044,           //移除USB备份盘失败，可能是备份盘正在使用，请稍候重试
    ERR_PMS_RECORD_USERLOG_FAILED = 22046,          //记录用户日志失败
    ERR_PMS_EXPORTMIRROR_TASK_ALREADY_EXIST = 22047,//导出镜像的任务已经存在
    ERR_PMS_ABNORMAL_STATUS = 22048,                //PMS处于异常状态，只提供有限服务
    ERR_PMS_INVALID_NTP_SERVER_ADDR = 22049,        //无效的NTP服务地址
    ERR_PMS_DOMAINNAME_ALREADY_EXIST = 22050,       //要修改的域名与其上下级平台冲突，操作失败
    ERR_PMS_SET_MAIL_CFG_ERR = 22052,               //邮箱配置失败
    ERR_PMS_QRY_MAIL_CFG_ERR = 22053,               //邮配置查询失败
    ERR_PMS_SEND_TEST_MAIL_ERR = 22054,             //测试邮件发送失败,发邮件命令执行失败
    ERR_PMS_MAILTO_LIST_IS_NULL = 22055,            //邮件接收列表为空，测试邮件发送失败
    ERR_PMS_MAILSERVER_CFG_ERR = 22056,             //邮件服务器配置有误，不能为空
	ERR_PMS_DOMAIN_CONPLICT = 22057,                //域名冲突
	ERR_PMS_EXPORT_TASK_LIMIT = 22058,              //导出任务达到限制值
	ERR_PMS_UPNP_EXIST = 22059,						//已存在相同的映射
	ERR_PMS_UPNP_NOT_EXIST = 22060,					//不存在的映射
	ERR_PMS_UPNP_NOT_CONNECT = 22061,				//UPnP尚未连接成功
	ERR_PMS_DDNS_NOT_CONNECT = 22062,				//DDNS尚未连接成功
	ERR_PMS_NET_SERVICE_TIMEOUT = 22063,			//NetService操作超时
	ERR_PMS_NET_DONOT_ALLOW_DEL_PRESET = 22064,		//禁止删除内置映射
	ERR_PMS_DDNS_DOMAIN_EXIST = 22065,				//DDNS域名已经被占用
	ERR_PMS_UPNP_ROUTER_DISABLE = 22066,			//路由器未开启UPnP服务
	ERR_PMS_UPNP_PORT_INVALID = 22067,				//UPnP端口无效
	ERR_PMS_UPNP_ERROR_UNKNOWN = 22068,				//未知的UPnP错误
	ERR_PMS_DDNS_ERROR_UNKNOWN = 22069,				//未知的DDNS错误
	ERR_PMS_UPNP_PARAM_INVALID = 22070,				//UPnP参数无效
	ERR_PMS_DDNS_SYSTEM_DOMAIN = 22071,				//系统保留的DDNS域名
	ERR_PMS_DDNS_SERVICE_DISABLE = 22072,           //DDNS服务未开启
	ERR_PMS_UPNP_PORT_IS_USED = 22073,				//UPnP端口被占用
	ERR_PMS_UPNP_NETCARD_OR_MODE_OVERDUE = 22074,	//UPnP的网卡id或者操作模式过期
    ERR_PMS_UPNP_PORT_USING_BY_DDNS = 22075,		//该映射正在被DDNS使用中
	ERR_PMS_UPNP_SWITCH_NETCARD_FAIL = 22076,		//切换后的网卡连接路由器失败
	ERR_PMS_HTTP_PORT_USED = 22077,					//Http端口被占用
	ERR_PMS_UPNP_IS_INEFFECTIVE = 22078,			//UPnP配置未生效
	ERR_PMS_UPNP_PLAT_NOT_ACTIVATE = 22079,			//平台UPnP功能未启用
    ERR_PMS_SET_MSS_SERVER_CFG_ERR = 22080,        //配置Mss服务器地址失败
    ERR_PMS_DEL_MSS_SERVER_CFG_ERR = 22081,        //配置Mss服务器地址失败
    ERR_PMS_SET_PUIGB_MTS_IP_CFG_ERR = 22082,     //pms配置puigb的mts
	ERR_PMS_SUB_DOMAIN_REDEL = 22083,         //平台私有子域退网重复操作
    ERR_PMS_SUB_DOMAIN_DEL_PROGRESS_QRY = 22084,         //平台私有子域退网进度查询失败
    ERR_PMS_CROSS_DEFENSE_STREAM_MOD_ERR = 22085,  //修改防御穿码流失败
    ERR_PMS_SET_GBS_MTS_IP_CFG_ERR = 22086,     //pms配置gbs的mts
    ERR_PMS_RESET_CU_PASSWORD_FAIL_ERR = 22087,          //pms恢复cu密码失败
    ERR_PMS_GET_SUCUREMAIL_ERR = 22088,                  //pmc获取安全邮箱失败
    ERR_PMS_GET_GEN_SERIALNUM_ERR = 22089,                //pms生成序列号失败
    ERR_PMS_SAFECODE_SN_NOT_PAIR_ERR = 22090,             //恢复默认密码失败
	ERR_PMS_NETWORK_SEGMENT_SAME_ERR = 22091,       //禁止与其他网卡处于相同网段
	ERR_PMS_NOT_CONNECTED_TO_REDIS_ERR = 22092,     //pms未连接上redis
    ERR_PMS_MOD_SECUREMAIL_ERR = 22093,                   //修改安全邮箱失败
	ERR_PMS_INPUT_ERROR_PASSWORD_TOO_MUCH = 22094, //连续输入密码多次错误被限制登陆
	ERR_PMS_IP_LIMIT_LOGIN = 22095,                //当前IP被限制登陆
	ERR_PMS_HEARTBEART_CONF_FAIL_ERR = 22096,      //主备机配置失败
	ERR_PMS_IPTABLES_CONF_FAIL_ERR = 22097,        //白名单配置失败
	ERR_PMS_HEARTBEAT_BAKCONF_EXIST = 22098,      //备机配置任务已存在
	ERR_PMS_TCPDUMP_FILE_NOT_EXIST = 22099,       //抓包文件不存在
	ERR_PMS_PASSWORD_EXPIRED = 22100,             //密码已过期
	ERR_PMS_GET_MODULE_LIST_ERR = 22101,          //获取模块列表失败
	ERR_PMS_TCPDUMP_EXIST = 22102,                //抓包任务已存在
	ERR_PMS_ZIP_MODULE_LOG_ERR = 22103,           //压缩模块日记失败
	ERR_PMS_GET_MOBILE_CONNECT_NUM_ERR = 22104,   //获取移动客户端接入数量失败
	ERR_PMS_NOT_SET_MIRROR_BAK_FILE = 22105,      //未设置备份镜像路径
	ERR_PMS_MIRROR_BAK_PATH_NOT_EXIST = 22106,    //镜像备份路径不存在
	ERR_PMS_MIRROR_BAK_FILE_NOT_EXIST = 22107,    //镜像备份文件不存在
	ERR_PMS_HEARTBEAT_CONF_GET_ERR = 22108,       //主备机配置信息获取失败
	ERR_PMS_SPACE_NOT_ENOUGH = 22109,             //空间不足
	ERR_PMS_EXPORT_LOG_TASK_EXIST = 22110,        //导出日记任务已存在
	ERR_PMS_MOD_USER_NAME_EXIST = 22111,          //要修改的用户的用户名已经存在
	ERR_PMS_LICENSE_NOT_EXIST_AND_HEARTBEAT_CONF_ERR = 22112,     //license不存在,备机配置失败

    ERR_PMS_END = ERR_PMS_BEGIN + 999,      //pms错误码结束值

    //rmssdk错误码 23000-23999
    ERR_RMSSDK_BEGIN = 23000,                   //rmssdk错误码结束值
    ERR_RMSSDK_END = ERR_RMSSDK_BEGIN + 999,    //rmssdk错误码结束值

    //scsdk错误码 24000-24999
    ERR_SCSSDK_BEGIN = 24000,                   //scsdk错误码结束值
    ERR_SCSSDK_END = ERR_SCSSDK_BEGIN + 999,    //scsdk错误码结束值

    //gbs错误码 25000-25999， 当前可用号为25033
    ERR_GBS_BEGIN = 25000,
    ERR_GBS_INVALID_PARAM = 25007,              //参数非法
    ERR_GBS_INVALID_OPERATE = 25008,            //非法操作
    ERR_GBS_CMSSDK_SENDMSG_FAILED = 25001,      //cmssdk发送消息失败
    ERR_GBS_CMSSDK_RESPONSE_TIMEOUT = 25002,    //cmssdk回复超时
    ERR_GBS_SAVE_CONFIG_FAILED = 25003,         //配置文件保存失败
    ERR_GBS_DBOPR_FAILED = 25004,               //数据库操作失败
    ERR_GBS_DBOPR_TIMEOUT = 25005,              //数据库操作超时
    ERR_GBS_EXIST_REPEATED_DATA = 25006,        //存在重复数据
    ERR_GBS_DETECTED_CIRCLE_DATA = 25017,       //探测到环数据
    ERR_GBS_INVALID_GBID = 25009,               //国标编号非法
    ERR_GBS_SAME_GBID_WITH_LOCAL = 25016,       //与本域国标编号相同
    ERR_GBS_GBDOMAIN_ID_NOT_AVAILABLE = 25020,  //国标域号不可用，也许存在相同的域号或存在环
    ERR_GBS_NOT_AVAILABLE_GBID = 25021,         //国标编号不可用，也许是国标编号非法或者非本域域号或者类型不支持或者编号已经被占用
    ERR_GBS_NOT_ENOUGH_GBID = 25022,            //提供的国标编号不够，要求设备和通道都需要指定
    ERR_GBS_DATA_NOT_EXIST = 25010,             //数据不存在
    ERR_GBS_DATA_NOT_INTEGRITY = 25011,         //数据不完整，建议删除相应数据
	ERR_GBS_PU_NOT_ONLINE = 25012,              //前端不在线
	ERR_GBS_OSP_SIP_POST_FAIL = 25013,          //OSPSIP发送消息失败
	ERR_GBS_PU_VCR_NOT_SUPPORT_CMD = 25014,     //前端不支持的命令
	ERR_GBS_PU_VCR_NOT_SUPPORT_PLAY_RATE = 25015,//前端不支持的播放速度
	ERR_GBS_G800_ERROR = 25018,                 //G800错误
	ERR_GBS_486_SIP_ERROR = 25019,              //486SIP错误
	ERR_GBS_SUBGBDMOAIN_NOT_ONLINE = 25023,     //国标下级域不在线
    ERR_GBS_LOWER_DOMAIN_BUSY = 25024,      //下级国标域忙
	ERR_GBS_INNER_PERIPHERAL_TYPE = 25025,		// 内置外设类型
    ERR_INVALID_GBS_GBNATID = 25026,               //不合法的国标NatID
    ERR_GBS_GB_NATID_NOT_EXIST = 25027,       //国标NATID不存在
    ERR_GBS_GB_NATID_ALREADY_IN_USE = 25028, //国标NATID当前正在使用
    ERR_GBS_GB_NATDATA_CONTENT_PORTRANGE_INVALID=25029, //国标NAT内容配置，端口范围错误
    ERR_GBS_GB_NATDATA_CONTENT_ALREADY_EXIST=25030,   //当前NAT配置已存在
	ERR_GBS_GB_LOWER_DOMAIN_TERMINATE_DIALOG=25031,   //国标下级域或国标前端中止会话
	ERR_GBS_PRIOR_OPERTAE_UNFINISHED_NEED_WAIT=25032,       //前次操作尚未结束，请稍后
	ERR_GBS_NEED_ADD_REPORT_INTER_DEV_CFG_FIRST=25033,      //需先增加“上报互联设备配置”
	ERR_GBS_UP_CAPACITY_LACK = 25034,                 //平台国标上联能力不足或有GBS模块未正常启动或有GBS未注册到主GBS
	ERR_GBS_NEED_SELECT_INTER_OPTION_FIRST = 25035,         //需要勾选“组建互联平台”选项
	ERR_GBS_WRITE_DATA_TO_REDIS_FAIL = 25036,         //数据写入redis失败
    ERR_GBS_IMPORT_FILE_NOT_EXIST = 25037,               //文件不存在
    ERR_GBS_END = 25999,
	
	//cmssdk内部错误码：26000-26499
	ERR_CMS_SDK_BEGIN                                 = 26000,
	ERR_CMS_SDK_REINIT                                = 26001,          //重复初始化
	ERR_CMS_SDK_CALLBACK_INVALID                      = 26002,          //回调函数无效
	ERR_CMS_SDK_GW_UUID_INVALID                       = 26003,          //网关UUID无效
	ERR_CMS_SDK_DOMAIN_NAME_INVALID                   = 26004,          //平台域名无效
	ERR_CMS_SDK_GW_ADDR_INVALID                       = 26005,          //网关地址无效
	ERR_CMS_SDK_PLAT_ADDR_INVALID                     = 26006,          //平台地址无效
	ERR_CMS_SDK_OSPEXT_INIT_FAIL                      = 26007,          //ospext初始化失败
	ERR_CMS_SDK_OSPSIP_INIT_FAIL                      = 26008,          //ospsip初始化失败
	ERR_CMS_SDK_OSPAPP_CREATE_FAIL                    = 26009,          //ospapp创建失败
	ERR_CMS_SDK_GW_MSG_INVALID                        = 26010,          //网关消息无效
	ERR_CMS_SDK_OSP_POST_FAIL                         = 26011,          //OspPost失败
	ERR_CMS_SDK_TIMEOUT                               = 26012,          //超时错误
	ERR_CMS_SDK_SEND_MSG_FAIL                         = 26013,          //向平台发送请求失败
	ERR_CMS_SDK_CMU_DIS_CONN                          = 26014,          //未连接到CMU
	ERR_CMS_SDK_3AS_DIS_CONN                          = 26015,          //未连接到3as
	ERR_CMS_SDK_PU_REG_TIMEOUT                        = 26016,          //pu注册超时
	ERR_CMS_SDK_DEVICE_UUID_INVALID                   = 26017,          //设备ID无效
	ERR_CMS_SDK_DEVICE_NAME_INVALID                   = 26018,          //设备名无效
	ERR_CMS_SDK_GROUP_UUID_INVALID                    = 26019,          //设备组组ID无效
	ERR_CMS_SDK_GROUP_NAME_INVALID                    = 26020,          //设备组组名无效
	ERR_CMS_SDK_DOMAIN_UUID_INVALID                   = 26021,          //域ID无效
	ERR_CMS_SDK_DEVICE_NOT_EXIST                      = 26022,          //设备不存在
	ERR_CMS_SDK_RCS_DIS_CONN                          = 26023,          //未连接到rcs
	ERR_CMS_SDK_TVS_DIS_CONN                          = 26024,          //未连接到tvs
	ERR_CMS_SDK_HAVE_NOT_SS                           = 26025,		   //尚未订阅（用于取消订阅前检查是否已订阅）
	ERR_CMS_SDK_ALREADY_SS                            = 26026,		   //重复订阅（用于订阅前检查是否重复订阅）
	ERR_CMS_SDK_SS_OPE_TYPE_INVALID                   = 26027,          //无效的订阅操作类型
	ERR_CMS_SDK_TRANSCODE_TERMINATE                   = 26028,          //G800转码异常终止
	ERR_CMS_SDK_PU_CONF_NTF_TIMEOUT                   = 26029,          //pu配置通知超时
	ERR_CMS_SDK_PU_UNREG_TIMEOUT                      = 26030,          //pu下线通知超时
	ERR_CMS_SDK_PU_ALREADY_REG                        = 26031,			//pu已经注册
	ERR_CMS_SDK_ERR_DIALOG_TERMINATE                  = 26032,			//会话异常终止
	ERR_CMS_SDK_ERR_TASK_DEL                          = 26033,			//Task已经删除
	ERR_CMS_SDK_DIALOG_NOT_ESTABLISHED                = 26034,			//会话尚未建立
	ERR_CMS_SDK_UAS_DIS_CONN                          = 26035,          //未连接到uas
	ERR_CMS_SDK_VCR_CTRL_TIMEOUT                      = 26036,          //VCR控制请求消息超时
	ERR_CMS_SDK_PLAT_REC_PLAYBACK_FINISHED            = 26037,          //平台放像正常结束
    ERR_CMS_SDK_COMMONCONFIG_INIT_FAIL                = 26038,          //公共配置文件初始化失败
    ERR_CMS_SDK_PU_REG_CONFIG_NOT_COMPLETED           = 26039,          //pu注册信息不完整
    ERR_CMS_SDK_PU_ALARM_RESET_FAIL                   = 26040,          //前端报警复位失败

	ERR_CMS_SDK_END                                   = 26499,

	//GW错误码：26500-26999
	ERR_GW_BEGIN                                      = 26500,
	ERR_GLB_ERR_NOINIT_ADAPTER                        = 26501,          //初始化ADAPTER
	ERR_GLB_ERR_LOAD_ADATER_FAIL                      = 26502,          //加载adapter失败
	ERR_GLB_ERR_INIT_ADAPTER_FAIL                     = 26503,          //初始化adapter失败
	ERR_GLB_ERR_NOINIT_PLAT                           = 26504,          //没有初始化平台
	ERR_GLB_ERR_USERNAME_ERR                          = 26505,          //用户信息错误
	ERR_GLB_ERR_PASSWORD_ERR                          = 26506,          //用户密码错误
	ERR_GLB_ERR_PARA_EMPTY                            = 26507,          //参数传递错误
	ERR_GLB_ERR_DLL_NOOBJ                             = 26508,          //底层dll为空
	ERR_GLB_ERR_UNKNOWN_REASON                        = 26509,          //未知原因的错误
	ERR_GLB_ERR_OPR_TIMEOUT                           = 26510,          //操作超时
	ERR_GLB_ERR_OPR_FAIL                              = 26511,		    //操作失败
	ERR_GLB_ERR_TRANS_LIMIT                           = 26512,          //转发超过限制       
	ERR_GLB_ERR_ALREADY_REALPLAY                      = 26513,          //针对该设备的接收地址已经开启(实时浏览)
	ERR_BLB_ERR_DEV_RECDOWN_EXIST                     = 26514,          //针对该设备通道的录像回放已经存在
	ERR_GLB_ERR_RESOURCE_LACK                         = 26515,          //系统资源不足
	ERR_GLB_ERR_NOSUPP_FUNC                           = 26516,          //不支持该功能
	ERR_GLB_ERR_NOSTREAM                              = 26517,          //无码流的通知
	ERR_GLB_ERR_NOINIT_NAT                            = 26518,          //NAT穿透未初始化
	ERR_GLB_ERR_DAU_REG_FAIL                          = 26519,          //DAU向GMU注册失败
	// G100相关错误码
	ERR_GLB_G100_SDKFAILED                            = 26520,	        //调用SDK接口失败
	ERR_GLB_G100_RECODERNOTFOUND                      = 26521,		    //查询不到录像
	ERR_GLB_G100_NOENOUGHPRI                          = 26522,			//权限不足
	ERR_GLB_G100_DVRNORESOURCE                        = 26523,			//DVR资源不足，系统忙
	ERR_GLB_G100_DVROPRATEFAILED                      = 26524,		    //DVR操作失败
	ERR_GLB_G100_TIMEINPUTERROR                       = 26525,		    //时间输入不正确
	ERR_GLB_G100_NOSPECFILE                           = 26526,			//回放时服务器没有指定的文件
	ERR_GLB_G100_OPERNOTFINISH                        = 26527,			//上次操作还未完成
	ERR_GLB_G100_COMMANDTIMEOUT                       = 26528,		    //执行命令超时
	ERR_GLB_G100_NOSUPPORT                            = 26529,			//服务器不支持
	ERR_GLB_G100_FILEFORMATERROR                      = 26530,		    //文件格式不正确
	ERR_GLB_G100_NODOWNLOADCHN                        = 26531,			//没有更多的下载通道可用(录像回放与下载不能同时进行)
	ERR_GLB_G100_DVRHASQUERYTASK                      = 26532,          //DVR已经存在查询任务，请稍后再查
	ERR_GLB_G100_DVRQUERYTIMEOUT                      = 26533,          //DVR录像查询超时 请等待
	ERR_GLB_G100_DVRQUERYUPLOADING                    = 26534,          //DVR其他用户正在回传数据 请等待
	ERR_GLB_G100_DHQUERYTIMEOUT                       = 26535, 		    //大华查询录像超时
	ERR_GLB_G100_DHPLAYBACKLIMIT                      = 26536,		    //大华录像回放只能一路

	ERR_GW_END                                        = 26999,

	ERR_CAPS_BEGIN                                    =27000,			//CAPS模块错误码开始
	ERR_CAPS_MCUSDK_INIT_FAILED	                      =27001,			//抓拍解码模块初始化失败
	ERR_CAPS_START_PLAY_FAILED                        =27002,			//抓拍开始解码播放失败
	ERR_CAPS_TAKE_PICTURE_FAILED                      =27003,			//抓拍失败
	ERR_CAPS_MAKE_THUMB_FAILED                        =27004,			//生成缩略图失败
	ERR_CAPS_TIME_OUT                                 =27005,			//抓拍超时
	ERR_CAPS_INVALID_CUI_URI                          =27006,           //无效CUI URI
	ERR_CAPS_INVALID_CUI_ALREADY_CONN                 =27007,           //相同的CUI已经连接
	ERR_CAPS_CHANNUM_EXCEED_LIMIT                     =27008,           //抓拍路数超出上限
	ERR_CAPS_DEV_ALREADY_EXIST                        =27009,           //抓拍设备已存在
	ERR_CAPS_DEV_NOT_EXIST                            =27010,           //抓拍设备不存在
	ERR_CAPS_END                                      =27499,			//CAPS模块错误码结束

	ERR_DPS_BEGIN									  =27500,			// DPS错误码开始.
	ERR_DPS_ZMQ_CONNECT_FAIL,						// 27501			// 连接失败
	ERR_DPS_ZMQ_SEND_FAIL,							// 27502			// 发送失败
	ERR_DPS_CACHE_ERROR,							// 27503			// Cache无效
	ERR_DPS_INVALID_SEARCH_KEY,						// 27504			// 搜索Key无效.
	ERR_DPS_WORKER_BUSY,							// 27505			// 模块忙.
	ERR_DPS_INVALID_URI,							// 27506			// 非法的URI
	ERR_DPS_INVALID_CLIENT_ALREADY_LOGIN,			// 27507			// 重复登录.
	ERR_DPS_DB_CONNECT_FAIL,						// 27508			// 连接数据库失败.
	ERR_DPS_DB_OP_FAIL,								// 27509			// 数据操作失败.
	ERR_DPS_REDIS_CONNECT_FAIL,						// 27510			// 连接Redis失败
	ERR_DPS_REDIS_OP_FAIL,							// 27511			// REDIS数据库操作失败.
	ERR_DPS_WAIT_IDLE_WORKER_TIMEOUT,				// 27512			// 等待空闲资源超时, 请稍后再试
	ERR_DPS_INVALID_DATA,							// 27513			// 数据不合法
	ERR_DPS_ZMQ_RECV_FAIL,							// 27513			// ZMQ接收失败
	ERR_DPS_TIME_OUT,								// 27514			// 处理超时


	ERR_DPS_END										  =27999,			// DPS模块错误码结束

    // RedisSdk错误码
    ERR_REDISSDK_BEGIN                                = 28000,

    ERR_REDISSDK_UNINITIALIZED                        = 28001,          // RedisSdk未初始化
    ERR_REDISSDK_REINITIALIZED                        = 28002,          // 重复初始化
    ERR_REDISSDK_CONNECT_FAILED                       = 28003,          // 连接redis失败
    ERR_REDISSDK_REDIS_UNAVALIABLE_TEMPORARILY        = 28004,          // redis-server锁定中，暂时不可用
    ERR_REDISSDK_LOAD_LUA2SHA_FAILED                  = 28005,          // 加载lua脚本和sha1编码映射关系失败
    ERR_REDISSDK_LOAD_DOMAIN_MAP_FAILED               = 28006,          // 加载域映射关系失败
    ERR_REDISSDK_SCRIPT_PARAM_ERROR                   = 28007,          // 参数错误
    ERR_REDISSDK_LUA2SHA_ERROR                        = 28008,          // lua脚本映射到sha1编码时出错
    ERR_REDISSDK_LUA_EXECUTE_ERROR                    = 28009,          // lua脚本执行时出错
    ERR_REDISSDK_DEV_ID_IS_EMPTY                      = 28010,          // DevId为空
    ERR_REDISSDK_SESSION_IS_EMPTY                     = 28011,          // Session为空
    ERR_REDISSDK_SCRIPT_RETURN_ERROR                  = 28012,          // 脚本返回结果出错
    ERR_REDISSDK_NO_REGISTERED_MODULE                 = 28013,          // 没有注册的模块
    ERR_REDISSDK_SUBSCRIBE_FAILED                     = 28014,          // redis订阅失败
    ERR_REDISSDK_PATTERN_SUBSCRIBE_FAILED             = 28015,          // redis模式订阅失败
    ERR_REDISSDK_UNSUBSCRIBE_FAILED                   = 28016,          // redis取消订阅失败
    ERR_REDISSDK_PATTERN_UNSUBSCRIBE_FAILED           = 28017,          // redis取消模式订阅失败
    ERR_REDISSDK_GET_PLAT_REC_PARAM_FAILED            = 28018,          // 获取平台录像参数出错
    ERR_REDISSDK_NO_ADAPTED_MODULE_CMU                = 28019,          // 没有找到合适的cmu
    ERR_REDISSDK_NO_ADAPTED_MODULE_RCS                = 28020,          // 没有找到合适的rcs
    ERR_REDISSDK_NO_ADAPTED_MODULE_VTDU               = 28021,          // 没有找到合适的vtdu
    ERR_REDISSDK_NO_ADAPTED_MODULE_PUI                = 28022,          // 没有找到合适的pui
    ERR_REDISSDK_NO_ADAPTED_MODULE_CUI                = 28023,          // 没有找到合适的cui
    ERR_REDISSDK_NO_ADAPTED_MODULE_NRU                = 28024,          // 没有找到合适的nru
    ERR_REDISSDK_GET_VTDU_RESOURCE_FAILED             = 28025,          // 获取vtdu资源信息失败
    ERR_REDISSDK_MODULE_IS_OFFLINE                    = 28026,          // 模块不在线
    ERR_REDISSDK_CU_IS_OFFLINE                        = 28027,          // CU不在线
    ERR_REDISSDK_PU_IS_OFFLINE                        = 28028,          // 前端不在线
    ERR_REDISSDK_INVALID_BASE_INFO                    = 28029,          // 无效的入网数据
    ERR_REDISSDK_NRU_NOT_IN_TOPO                      = 28030,          // nru未入网
    ERR_REDISSDK_ALLOC_VTDU_RESOURCE                  = 28031,          // 获取vtdu转发资源失败
    ERR_REDISSDK_FORBID_STOP_PLAN_ALARM_REC           = 28032,          // 禁止手动停止计划或者告警录像
    ERR_REDISSDK_RECORD_TASK_NOT_EXIST                = 28033,          // 录像任务不存在
    ERR_REDISSDK_RECORD_TASK_NOT_BELONG_NRU           = 28034,          // 录像任务不属于指定的nru
    ERR_REDISSDK_RECORD_TASK_ALREAY_EXIST             = 28035,          // 录像任务已存在
    ERR_REDISSDK_RECORD_HAS_SAVED                     = 28036,          // 录像任务已保存，但是当前没有适合的磁盘或者分区
    ERR_REDISSDK_DEVICE_NOT_IN_TOPO                   = 28037,          // 设备未入网

    ERR_REDISSDK_END                                  = 28499,


    // NRU错误码
    ERR_NRU_BEGIN                              	      = 29000,
	ERR_NRU_GET_VTDU                       	          = 29001,          // [NRU]获取VTDU失败
    ERR_NRU_DEV_OFFLINE                    	          = 29002,          // [NRU]设备下线
    ERR_NRU_PARAM							          = 29003,          // [NRU]NRU参数错
	ERR_NRU_REACH_MAX_TASK						      = 29004,          // [NRU]NRU达到最大任务数
	ERR_NRU_TASK_TIMEOUT							  = 29005,          // [NRU]NRU处理超时
    ERR_NRU_GET_REC_TASK_PARAM						  = 29006,          // [NRU]获取录像任务信息失败
    ERR_NRU_PU_NOT_ONLINE                             = 29007,          // [NRU]前端设备不在线
    ERR_NRU_PU_CHANNEL_NOT_ONLINE                     = 29008,          // [NRU]前端通道不在线
    ERR_NRU_PU_MANU_INVALID                           = 29009,          // [NRU]前端设备厂商非法
    ERR_NRU_PU_CONFIG_NOT_AVAILABLE                   = 29010,          // [NRU]前端配置参数不可得
    ERR_NRU_RECORD_PARAM_NOT_EXIST                    = 29011,          // [NRU]录像参数不存在
    ERR_NRU_RECORD_TASK_ALREADY_EXIST                 = 29012,          // [NRU]录像任务已经存在
    ERR_NRU_EXCEPTION                                 = 29013,          // [NRU]服务器异常
    ERR_NRU_ALLOC_RECORD_RES_FAILED                   = 29014,          // [NRU]分配录像资源失败
    ERR_NRU_START_RECORD_FAILED                       = 29015,          // [NRU]开启录像失败
    ERR_NRU_PLAN_REC_NOT_NOW                          = 29016,          // [NRU]计划录像未到开启时间

	ERR_NRU_UNKNOWN								= 29099,

	//nru err(by rmserr)
	ERR_NRU_RSERR_RPCTRL_NOT_INITIAL  				= 29100,
	ERR_NRU_RSERR_NO_MEMORY							= 29101,
	ERR_NRU_RSERR_RECORD_FILE_NOT_EXIST				= 29102,
	ERR_NRU_RSERR_REC_STATUS_ERROR  				= 29103,
    ERR_NRU_RSERR_RECTASK_TOO_MUCH					= 29104,
    ERR_NRU_RSERR_REC_OVER_CAPACITY					= 29105,
    ERR_NRU_RSERR_RESOLV_CHANGED					= 29106,
	ERR_NRU_RSERR_WAIT_KEYFRAME						= 29107,
    ERR_NRU_RSERR_TIMESTAMP_ERR						= 29108,
	ERR_NRU_RSERR_PREREC_TIME_INVALID				= 29109,
    ERR_NRU_RSERR_SMALL_FILE_INTERVAL_INVALID		= 29110,
	ERR_NRU_RSERR_ALREADY_ALARM_TWO					= 29111,
    ERR_NRU_RSERR_PLAYTASK_NOTEXIST					= 29112,
	ERR_NRU_RSERR_NO_PLAY_RESOURCE       			= 29113,
	ERR_NRU_RSERR_PLAY_OVER_CAPACITY    			= 29114,
	ERR_NRU_RSERR_PLAY_DROG_TIME_INVALID    		= 29115,
	ERR_NRU_RSERR_PLAY_GRAN_INVALID    				= 29116,
	ERR_NRU_RSERR_PLAY_RECORD_LENGTH_ZERO			= 29117,
    ERR_NRU_RSERR_PLAY_REACH_STOP_TIME				= 29118,
	ERR_NRU_RSERR_NO_DOWNLOAD_RESOURCE				= 29119,
	ERR_NRU_RSERR_DOWNLOADTASK_NOTEXIST    			= 29120,
	ERR_NRU_RSERR_DOWNLOAD_STATUS_ERROR				= 29121,
	ERR_NRU_RSERR_SOCKET_ERROR						= 29122,
    ERR_NRU_RSERR_CONNECT_ERROR  					= 29123,
	ERR_NRU_RSERR_PATH_ALREADY_EXIST				= 29124,
    ERR_NRU_RSERR_PATH_NOT_EXIST 					= 29125,
	ERR_NRU_RSERR_PATH_CREATE_FAIL_RPC				= 29126,
    ERR_NRU_RSERR_PATH_DELETE_FAIL 					= 29127,
	ERR_NRU_RSERR_FILE_PATH_INVALID					= 29128,
	ERR_NRU_RSERR_FILE_ALREADY_EXIST  				= 29129,
	ERR_NRU_RSERR_FILE_NOT_EXIST					= 29130,
	ERR_NRU_RSERR_FILE_DELETE_FAIL					= 29131,
	ERR_NRU_RSERR_FILE_RENAME_FAIL					= 29132,
    ERR_NRU_RSERR_FILE_NAME_INVALID					= 29133,
	ERR_NRU_RSERR_MEDIATYPE_UNSUPPORT    			= 29134,
	ERR_NRU_RSERR_START_TIME_INVALID    			= 29135,
	ERR_NRU_RSERR_RECLOG_FILE_NOT_EXIST				= 29136,
	ERR_NRU_RSERR_RPCMD_IS_BUSY						= 29137,
	ERR_NRU_RSERR_INODE_DESTROYED					= 29138,
	ERR_NRU_RSERR_FORMAT_CHANGE						= 29139,
    ERR_NRU_RSERR_FILEVER_TOOLOW					= 29140,
	ERR_NRU_RSERR_FILE_PROPERTY						= 29141,
	ERR_NRU_RSERR_FILE_FAIL							= 29142,
	ERR_NRU_RSERR_METHOD_NOTSUPPORT      			= 29143,
	
	ERR_NRU_END                                		= 29999,

    //未知错误码，尽量不使用未知错误
    ERR_UNKNOWN = 0xFFFFFFFF,
};

#endif // _CMS_ERRORCODE_H_
