/*****************************************************************************
   模块名      : cms_landesc 
   文件名      : cms_landesc.h
   相关文件    : 
   文件实现功能: 定义整个CMS系统的语言描述常量
   作者        : wangwenxuan
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/11/22   1.0      wangwenxuan      创建
******************************************************************************/

#ifndef _CMS_LANDESC_H_
#define _CMS_LANDESC_H_

#define     LAN_DESC_USER                 "User"                   //"用户"
#define     LAN_DESC_USERID               "UserId"                 //"用户编号"
#define     LAN_DESC_USERNAME             "UserName"               //"用户名"
#define     LAN_DESC_OPRUSERID            "OperatorUserId"         //"操作者ID" 设备组划归时需要
#define     LAN_DESC_PWD                  "Password"               //"密码"
#define     LAN_DESC_OPR                  "Operate"                //"操作"
#define     LAN_DESC_DEV                  "Device"                 //"设备"
#define     LAN_DESC_DEV_NAME             "DeviceName"             //"设备名称"
#define     LAN_DESC_VIDSRC               "VideoSource"            //"视频源"
#define     LAN_DESC_VIDSRCALIAS          "VideoSourceAlias"       //"视频源别名"
#define     LAN_DESC_LOGIN                "Login"                  //"登陆"
#define     LAN_DESC_LOGOUT               "Logout"                 //"登出"
#define     LAN_DESC_MODULE               "Module"                 //"模块"
#define     LAN_DESC_ADD                  "Add"                    //"增加"
#define     LAN_DESC_DEL                  "Delete"                 //"删除"
#define     LAN_DESC_MOD                  "Modify"                 //"修改"
#define     LAN_DESC_QRY                  "Query"                  //"查询"
#define     LAN_DESC_TAG                  "Tag"                    //"标签"
#define     LAN_DESC_NTF                  "Notify"                 //"通知"
#define     LAN_DESC_DATA                 "Data"                   //"数据"
#define     LAN_DESC_SET                  "Set"                    //"设置"
#define     LAN_DESC_GET                  "Get"                    //"获取"
#define     LAN_DESC_SAVE                 "Save"                   //"保存"
#define     LAN_DESC_LOAD                 "Load"                   //"载入"
#define     LAN_DESC_CREATE               "Create"                 //"创建"
#define     LAN_DESC_MODEL                "Model"                  //"型号"
#define     LAN_DESC_DATE                 "Date"                   //"日期"
#define     LAN_DESC_ALIAS                "Alias"                  //"别名"
#define     LAN_DESC_DOMAINNAME           "DomainName"             //"域名"
#define     LAN_DESC_DOMAINID             "DomainId"               //"域编号"
#define     LAN_DESC_PARENT_DOMAINNAME    "ParentDomainName"       //"父域名称"
#define     LAN_DESC_PARENT_DOMAINID      "ParentDomainId"         //"父域编号"
#define     LAN_DESC_START                "Start"                  //"启动"
#define     LAN_DESC_STOP                 "Stop"                   //"停止"
#define     LAN_DESC_TYPE                 "Type"                   //"类型"
#define     LAN_DESC_CAP                  "Capability"             //"能力"
#define     LAN_DESC_MANUFACTURER         "Manufacturer"           //"厂商"
#define     LAN_DESC_USERGROUP            "UserGroup"              //"用户组"
#define     LAN_DESC_USERGROUP_ID         "UserGroupID"            //"用户组编号"
#define     LAN_DESC_USERGROUP_NAME       "UserGroupName"          //"用户组名"
#define     LAN_DESC_DESC                 "Description"            //"描述"
#define     LAN_DESC_TVWALL               "TvWall"                 //"电视墙"
#define     LAN_DESC_TVWALLID             "TvWallId"               //"电视墙编号"
#define     LAN_DESC_TVWALL_NAME          "TvWallName"             //"电视墙名称"
#define     LAN_DESC_TVID                 "TvId"                   //"电视编号"
#define     LAN_DESC_TVDIV_ID             "TvDivID"                //"电视分画面ID"
#define     LAN_DESC_DECODERID            "DecoderId"              //"解码器编号"
#define     LAN_DESC_TVNUM                "TvNum"                  //"电视机数目"   
#define     LAN_DESC_TVWALL_STYLE         "TvWallStyle"            //"电视墙风格"
#define     LAN_DESC_TVDECODER_BINDER     "BinderofTv-Decoder"     //"电视与解码器绑定关系"
#define     LAN_DESC_FILTER_CONDITIONS    "FilterConditions"       //"过滤条件"
#define     LAN_DESC_DEVPRIVILEGE         "DevicePrivilege"        //"设备权限"
#define     LAN_DESC_DEVGRPPRIVILEGE         "DeviceGroupPrivilege"        //"设备组权限"
#define     LAN_DESC_DEVCAP               "DeviceCapability"       //"设备能力"
#define     LAN_DESC_PRIVILEGE            "Privilege"              //"权限"
#define     LAN_DESC_CREATE_DATE          "CreateDate"             //"创建日期"
#define     LAN_DESC_USERTYPE             "UserType"               //"用户类型"
#define     LAN_DESC_START_DATE           "StartDate"              //"起始日期"
#define     LAN_DESC_STOP_DATE            "StopDate"               //"结束日期"   
#define     LAN_DESC_USER_PRIVILEGE       "UserPrivilege"          //"用户权限"
#define     LAN_DESC_GROUP_CLASSIFIED     "GroupClassified"        //"分组划归"
#define     LAN_DESC_AUTH                 "Auth"                   //"授权" 
#define     LAN_DESC_CROSS_DOMAIN_AUTH    "CrossDomainAuth"        //"跨域授权"
#define     LAN_DESC_GROUP_DEVICE         "GroupDevice"            //"设备分组"
#define     LAN_DESC_DEVICE_GROUP         "DeviceGroup"            //"设备组"
#define     LAN_DESC_DEVICE_GROUP_NAME    "DeviceGroupName"        //"设备组名"
#define     LAN_DESC_DEVICE_ID            "DeviceID"               //"设备ID"
#define     LAN_DESC_DEVICE_GROUP_ID      "DeviceGroupID"          //"设备组ID"
#define     LAN_DESC_DEV_GROUP_AUTO_ASSIGN "DeviceGroupAutoAssign" //"设备组自动同步标记"
#define     LAN_DESC_SUCCESS              "Success"                //"成功"
#define     LAN_DESC_FAIL                 "Fail"                   //"失败"
#define     LAN_DESC_TIMEOUT              "TimeOut"                //"超时"
#define     LAN_DESC_LOGINTYPE            "LoginType"              //"登陆类型"
#define     LAN_DESC_EXTRA_INFO           "ExtraInfo"              //"扩展信息"
#define     LAN_DESC_DEVADDRLIST          "DeviceAddrList"         //"设备地址列表"
#define     LAN_DESC_DEVURI               "DeviceURI"              //"设备URI"
#define     LAN_DESC_VERSION              "Version"                //"版本"
#define     LAN_DESC_CUSTOM               "Custom"                 //"自定义"
#define     LAN_DESC_SUBSCRIBER           "Subscriber"             //"订阅者"
#define     LAN_DESC_BY_SUBSCRIBER        "BySubscriber"          //"被订阅者"
#define     LAN_DESC_SUB                  "Subscribe"              //"订阅"
#define     LAN_DESC_SUB_STATUS           "SubscribeStatus"        //"订阅状态"
#define     LAN_DESC_ONLINE_STATUS        "OnlineStatus"           //"上下线状态"
#define     LAN_DESC_ONLINE               "Online"                 //"上线"
#define     LAN_DESC_OFFLINE              "Offline"                //"下线"
#define     LAN_DESC_PU_CONFIG            "PuConfig"              //"前端配置"
#define     LAN_DESC_PU_SERVICE_STATUS    "PuServiceStatus"      //"前端设备状态"
#define     LAN_DESC_GPS_STATUS            "GPSStatus"              // "GPS状态"
#define     LAN_DESC_DEV_CHN              "DevChannel"            //"设备通道"
#define     LAN_DESC_CAM_CTRL             "CameraCtrl"               //"摄像头控制"
#define     LAN_DESC_ERROR_DESC           "ErrorDesc"             //"错误描述"
#define     LAN_DESC_SERIAL_DATA          "SerialData"            //"串口数据"
#define     LAN_DESC_LOCK                 "Lock"                   //"加锁"
#define     LAN_DESC_UNLOCK               "Unlock"                 //"解锁"
#define     LAN_DESC_VIDEOENCODER_PARAM   "VideoEncoderParam"     //"视频编码参数"
#define     LAN_DESC_MEDIA_TYPE           "MediaType"             //"媒体格式"
#define     LAN_DESC_MEDIA_DESC           "MediaDesc"             //"媒体描述"
#define     LAN_DESC_PUSYS_PARAM          "PuSystemParam"        //"前端系统参数"
#define     LAN_DESC_SYNC_SWITCH          "SyncSwitch"           //"同步开关"
#define	    LAN_DESC_SYNC_VS_NAME_PU2PLAT "SyncVideoSrcNamePu2Plat" //"视频源别名从前端同步到平台"
#define	    LAN_DESC_SYNC_VS_NAME_PLAT2PU "SyncVideoSrcNamePlat2Pu" //"视频源别名从平台同步到前端"
#define     LAN_DESC_DOWNLOAD_PLATFORMRECORD "DownLoadPlatformRecord"        //"平台录像下载"
#define     LAN_DESC_DOWNLOAD_PURECORD    "DownLoadPuRecord"     //"前端录像下载"
#define     LAN_DESC_RECORDSYS_PARAM      "RecordSystemParam"    //"录像系统参数"
#define     LAN_DESC_RECORD_PARAM         "RecordParam"          //"录像参数"
#define     LAN_DESC_RECORD_MODE          "RecordMode"           //"录像模式"
#define     LAN_DESC_RECODE_SUPPORT_CHANGE_STREAM_type  "SupportChangeStreamType"   //支持码流切换
#define     LAN_DESC_RECORD_STREAM_TYPE     "RecodeStreamType"      //录像码流类型
#define     LAN_DESC_PRERECORD_DURATION   "PreRecordDuration"      //"预录时长"
#define     LAN_DESC_REBOOT               "Reboot"                 //"重启"
#define     LAN_DESC_THIRD_PARTY_CALL     "ThirdPartyCall"         //"三方呼叫"
#define     LAN_DESC_SWITCH               "Switch"                 //"交换"
#define     LAN_DESC_SWITCH_NUM           "SwitchNum"             //"交换数"
#define     LAN_DESC_RCV_GROUP            "ReceiveGroup"           //"接收组"
#define     LAN_DESC_GAP                  "Gap"                    //"网闸"
#define     LAN_DESC_ALARM_INFO           "AlarmInfo"             //"告警信息"
#define     LAN_DESC_ALARM_INPUT          "AlarmInput"            //"告警输入"
#define     LAN_DESC_ALARM_INPUT_ID       "AlarmInputID"         //"告警输入通道"
#define     LAN_DESC_ALARM_TYPE           "AlarmType"             //"告警类型"
#define     LAN_DESC_ALARM_FLAG           "AlarmFlag"             //"告警标识"
#define     LAN_DESC_ALARM_TIME           "AlarmTime"             //"告警时间"
#define     LAN_DESC_ALARM_DATA           "AlarmData"             //"告警数据"
#define     LAN_DESC_ALARM_AREA_ID        "AlarmAreaID"            //"告警区域号"
#define     LAN_DESC_ALARM_LINK           "AlarmLink"             //"告警联动"
#define     LAN_DESC_ALARM_LINK_CFG       "AlarmLinkCfg"          //"告警联动配置"
#define     LAN_DESC_ALRAM_ACTION         "AlarmAction"           //"联动行为"
#define     LAN_DESC_ALARM_SRC            "AlarmSource"           //"告警源"
#define     LAN_DESC_ALARM_READ_TIME      "AlarmReadTime"          //"告警读秒时长"
#define     LAN_DESC_IO_OUT               "IOOut"                 //"开关量输出"
#define     LAN_DESC_TV_OPR_TYPE          "TVOperateType"          //"电视操作类型"
#define     LAN_DESC_TV_CHN               "TVChannel"             //"电视通道"
#define     LAN_DESC_TVWIN_POLL           "TVWindowPoll"         //"电视窗口轮巡"
#define     LAN_DESC_POLL_INDEX           "PollIndex"             //"轮巡序号"
#define     LAN_DESC_POLL_TIME            "PollTime"              //"轮巡时间"
#define     LAN_DESC_TVWIN_STYLE          "TVWindowStyle"         //"画面风格"
#define     LAN_DESC_TVWALL_SCHEME        "TVWallScheme"          //"电视墙预案"
#define     LAN_DESC_TVWALL_SCHEME_GROUP  "TVWallSchemeGroup"     //"电视墙预案组"
#define     LAN_DESC_SCHEME_POLL          "SchemePoll"            //"预案轮巡"
#define     LAN_DESC_SCHEME_NAME          "SchemeName"            //"预案名称"
#define     LAN_DESC_POLL_STATE           "PollState"             //"轮巡状态"
#define     LAN_DESC_MEDIA_SERVICE        "MediaService"          //"媒体服务类型"
#define     LAN_DESC_REC_TOKEN            "RecordToken"           //"录像Token"
#define     LAN_DESC_TIME_RANGE           "TimeRange"             //"时间段"
#define     LAN_DESC_ENC_CHN              "EncoderChannel"        //"编码器通道"
#define     LAN_DESC_MOTION_DETECT        "MotionDetect"          //"移动侦测"
#define     LAN_DESC_LEFT_TOPX            "LeftTopX"             //"顶点横坐标"
#define     LAN_DESC_LEFT_TOPY            "LeftTopY"             //"顶点纵坐标"
#define     LAN_DESC_WIDTH                "Width"                  //"宽度"
#define     LAN_DESC_HEIGHT               "Height"                 //"高度"
#define     LAN_DESC_ALARM_OCCUR_RATE     "AlarmOccurRate"       //"告警产生灵敏率" 
#define     LAN_DESC_ALRAM_RESTORE_RATE   "AlarmRestoreRate"     //"告警恢复灵敏率"
#define     LAN_DESC_AREA_SHADE           "AreaShade"             //"区域遮蔽"
#define     LAN_DESC_GPS_CFG              "GPSConfig"             //"GPS配置"
#define     LAN_DESC_GPS_SS               "GPSSubscribe"          //"GPS订阅"
#define     LAN_DESC_GPS_DATA             "GPSData"               //"GPS数据"
#define     LAN_DESC_TRANSPARENT_DATA     "TransparentDate"       //"透明数据"
#define     LAN_DESC_REC_TYPE             "RecordType"             //"录像类型"
#define     LAN_DESC_REC_INFO             "RecordInfo"             //"录像信息"
#define     LAN_DESC_RECORD_TASK          "RecordTask"             //"录像任务"
#define     LAN_DESC_FILE_PATH            "FilePath"              //"文件路径"
#define     LAN_DESC_FILE_NAME            "FileName"              //"文件名称"
#define     LAN_DESC_DEC_STYLE            "DecoderStyle"          //"解码器风格"
#define     LAN_DESC_VCR_CTRL             "VCRCtrl"               //"VCR控制"
#define     LAN_DESC_VCR_CMD              "VCRCmd"                //"VCR命令"
#define     LAN_DESC_PLAY_RATE            "PlayRate"              //"放像速率"
#define     LAN_DESC_SEEK_TIME            "SeekTime"              //"相对时间"
#define     LAN_DESC_CUR_PLAYTIME         "CurrentPlayTime"       //"当前放像时间"
#define     LAN_DESC_TOTAL_PLAYTIME       "TotalPlayTime"         //"放像总时长"
#define     LAN_DESC_START_PLAYTIME       "StartPlayTime"         //"起始放像时间"
#define     LAN_DESC_STOP_PLAYTIME        "StopPlayTime"          //"结束放像时间"
#define     LAN_DESC_PTZ_CTRL             "PTZCtrl"               //"云镜控制"
#define     LAN_DESC_PTZ_LEVEL            "PTZLevel"              //"云镜控制级别"
#define     LAN_DESC_PTZ_LOCK             "PTZLock"               //"云镜控制加锁"
#define     LAN_DESC_PTZ_UNLOCK           "PTZUnLock"             //"云镜控制解锁"
#define     LAN_DESC_PTZ_AUTOUNLOCK       "PTZAutoUnlock"            // "云镜控制自动解锁"
#define     LAN_DESC_PTZ_UNLOCKAll        "PTZUnlockAll"             // "云镜控制全部解锁"
#define     LAN_DESC_HOLD_TIME            "HoldTime"              //"把持时间"
#define     LAN_DESC_DIRECT_TRANS_NUM     "DirectTransNum"       //"直传路数"
#define     LAN_DESC_VID_FORMAT           "VideoFormat"           //"视频格式"
#define     LAN_DESC_AUD_FORMAT           "AudioFormat"           //"音频格式"
#define     LAN_DESC_VID_RES              "VideoResolution"      //"视频分辨率"
#define     LAN_DESC_FRAME_RATE           "FrameRate"             //"帧率"
#define     LAN_DESC_QUALITY              "Quality"                //"质量"
#define     LAN_DESC_KEYFRAME_INTERVAL    "KeyFrameInterval"               //"关键帧间隔"
#define     LAN_DESC_BRIGHTNESS           "Brightness"             //"亮度"
#define     LAN_DESC_CONTRAST             "Contrast"               //"对比度"
#define     LAN_DESC_SATURATION           "Saturation"             //"饱和度"
#define     LAN_DESC_SHARPNESS            "Sharpness"              //"锐度"
#define     LAN_DESC_INPUT_PIN            "InputPin"              //"并口输入"
#define     LAN_DESC_OUTPUT_PIN           "OutputPin"             //"并口输出"
#define     LAN_DESC_VIDEO_IN             "VideoInput"            //"视频输入"
#define     LAN_DESC_DEV_MODEL            "DeviceModel"            //"设备型号"
#define     LAN_DESC_CAM_TYPE             "CameraType"            //"摄像头类型"
#define     LAN_DESC_CAM_ID               "CameraID"              //"摄像头地址码"
#define     LAN_DESC_BIT_RATE             "BitRate"               //"比特率"
#define     LAN_DESC_DIRECT_TRANS         "DirectTrans"           //"直传" 
#define     LAN_DESC_PLAT_PRE_REC         "PlatformPreRecord"      //"平台预录"
#define     LAN_DESC_PLAT_REC             "PlatformRecord"         //"平台录像"
#define     LAN_DESC_PLAT_REC_ON_TV       "PlatformRecordOnTv"     //"平台录像上墙"
#define     LAN_DESC_PU_PRE_REC           "PuPreRecord"            //"前端预录"
#define     LAN_DESC_PU_REC               "PuRecord"               //"前端录像"
#define     LAN_DESC_CAM_PRE_POS          "CameraPresetPos"        //"摄像头预置位"
#define     LAN_DESC_MSG_NO               "MessageNo"              //"短信号码"
#define     LAN_DESC_VOC_NO               "VoiceNo"               //"声讯号码"
#define     LAN_DESC_EMAIL                "EMail"                  //"电子邮箱"
#define     LAN_DESC_TRANS                "Transport"              //"传输"
#define     LAN_DESC_TRANS_TYPE           "TransportType"         //"传输模式"  
#define     LAN_DESC_MEDIA_TRANSPORT      "MediaTransport"        //"媒体传输"
#define     LAN_DESC_NET_ADDR             "NetAddress"            //"网络地址"       
#define     LAN_DESC_PAUSE                "Pause"                  //"暂停"
#define     LAN_DESC_RESUME               "Resume"                 //"恢复"
#define     LAN_DESC_SEEK                 "Seek"                   //"定位"
#define     LAN_DESC_FAST_PLAY            "FastPlay"              //"快放"
#define     LAN_DESC_SLOW_PLAY            "SlowPlay"              //"慢放"
#define     LAN_DESC_ONESTEP_FRAME            "OneStepFrame"              //"单帧播放"
#define     LAN_DESC_KEY_FRAME            "KeyFrame"              //"关键帧播放"
#define     LAN_DESC_INVALID              "Invalid"                //"无效"
#define     LAN_DESC_DISK_FULL            "DiskFull"              //"磁盘满"
#define     LAN_DESC_PUCHN_BINDING        "PuChnBinding"          //"通道绑定"
#define     LAN_DESC_PUCHN_UNBINDING      "PuChnUnBinding"        //"解除通道绑定"
#define     LAN_DESC_NRU_ID               "NruId"                 //"NruID"
#define     LAN_DESC_DISK_MOUNT_PATH      "DiskMountPath"         //"磁盘挂载路径"
#define     LAN_DESC_RECORD_DAYS          "RecordDays"            //"录像天数"
#define     LAN_DESC_SCHEME               "Scheme"                //"预案"
#define     LAN_DESC_E_MAP                "EMap"                  //"电子地图"
#define     LAN_DESC_MAP_ID               "MapID"                 //"地图编号"
#define     LAN_DESC_MAP_NAME             "MapName"               //"地图名称"
#define     LAN_DESC_MAP_TYPE             "MapType"               //"地图类型"
#define     LAN_DESC_MAP_ELEMENT          "MapElement"            //"图元"
#define     LAN_DESC_MAP_ELEMENT_ID       "MapElementID"          //"图元编号"
#define     LAN_DESC_MAP_ELEMENT_NAME     "MapElementName"        //"图元名称"
#define     LAN_DESC_MAP_ELEMENT_TYPE     "MapElementType"        //"图元类型"
#define     LAN_DESC_COORDINATES          "Coordinates "          //"坐标"
#define     LAN_DESC_TRANSFORMATION       "Transformation"        //"转换"
#define     LAN_DESC_MARS_COORDINATES     "MarsCoordinates"       //"火星坐标"
#define     LAN_DESC_EARTH_COORDINATES    "EarthCoordinates"      //"地球坐标"
#define     LAN_DESC_LNG                  "Longitude"             //"经度"
#define     LAN_DESC_LAT                  "Latitude "             //"纬度"
#define     LAN_DESC_UAS_CFG              "UasConfig"             //"Uas配置" 暂时改成日志参数配置
#define     LAN_DESC_REAL_MEDIA_ON_TV     "RealMediaOnTv"         //"实时码流上墙"
#define     LAN_DESC_REAL_MEDIA_OFF_TV    "RealMediaOffTv"        //"实时码流下墙"
#define     LAN_DESC_RECORD_ON_TV         "RecordOnTv"            //"录像上墙"
#define     LAN_DESC_RECORD_OFF_TV        "RecordOffTv"           //"录像下墙"
#define     LAN_DESC_REAL_MEDIA_INVITE    "RealMediaInvite"       //"实时浏览"
#define     LAN_DESC_PU_PLAYBACK          "PuPlayBack"            //"前端录像回放"
#define     LAN_DESC_PLATFORM_PLAYBACK    "PlatformPlayBack"      //"平台录像回放"
#define     LAN_DESC_SWITCH_STOP_BY_BYE   "SwitchStopByBye"       //"交换被挂断"
#define     LAN_DESC_SIP_EXCEPTION        "SipException"          //"Sip消息异常"
#define     LAN_DESC_LOAD_BALANCE         "LoadBalance"           //"负载均衡"
#define     LAN_DESC_PARTITION_FULL       "PartitionFull"         //"磁盘空间满"
#define     LAN_DESC_BIGFILE_SWITCH_FAIL  "BigFileSwitchFail"     //"大文件切换失败"
#define     LAN_DESC_RESOLUTION_OR_FORMAT_CHANGE        "ResolutionOrFormatChange"          //"分辨率或格式变更"
#define     LAN_DESC_REC_NO_STREAM		  "RecNoStream"           //"无码流"
#define     LAN_DESC_PU_OFFLINE_TOO_BRIEF "PuOfflineTooBrief"     //"设备频繁下线"
#define     LAN_DESC_REASON_UNKNOWN       "ReasonUnknow"          //"原因未知"
#define     LAN_DESC_ERROR_CODE           "ErrorCode"             //"错误码"
#define     LAN_DESC_DEV_TREE             "DevTree"               //"设备树"
#define     LAN_DESC_DEV_TREE_ID          "DevTreeId"             //"设备树编号"
#define     LAN_DESC_DEV_TREE_NAME        "DevTreeName"           //"设备树名称"
#define     LAN_DESC_WATER_MARK_MODIFICATION  "WaterMarkModification"    //"水印变更"
#define     LAN_DESC_FONT_SIZE            "FontSize"            //"字体大小"
#define     LAN_DESC_FONT_NAME            "FontName"            //"字体名称"
#define     LAN_DESC_ROTATION             "Rotation"            //"转角"
#define     LAN_DESC_TRANSPARENCY         "Transparency"        //"透明度"
#define     LAN_DESC_TILE                 "Tile"               //"平铺性"
#define     LAN_DESC_COLOR                "Color"               //"颜色"

///////////////////////////////////////////////////////////////////////////////////////////////////
#define     LAN_CU_PASSWORD_LOCKED            "CuPassowordLocked"
#define     LAN_CU_CHANGE_PASSWORD_SUCCESS    "CuSuccessInPwdMod" //修改密码
#define     LAN_CU_CHANGE_PASSWORD_FAIL       "CuFailToChangePwd" //修改密码
#define     LAN_CU_CHANGE_PRIVILEGE_SUCCESS       "CuSuccessToChangePri" //权限修改
#define     LAN_CU_CHANGE_PRIVILEGE_FAIL       "CuFailToChangePri" //权限修改


#define     LAN_PMC_USER_LOGIN                "PmcUserLogin"          //登录%s
#define     LAN_PMC_USER_LOGOUT               "PmcUserLogout"          //登出%s
#define     LAN_PMC_MOD_PASSWORD              "PmcModPassword"         //修改密码%s
#define     LAN_PMC_PASSWORD_LOCKED           "PmcPasswordLocked"      //连续输入密码错误，已限制登录
#define     LAN_PMC_ADD_USER                  "PmcAddUser"             //添加用户(用户名:%s)%s
#define     LAN_PMC_MOD_USER                  "PmcModUser"             //修改用户[用户名:%s-用户类型:%s-用户描述:%s]为[用户名:%s-用户类型:%s-用户描述:%s]%s
#define     LAN_PMC_DEL_USER                  "PmcDelUser"             //删除用户[用户ID:%s]%s
#define     LAN_PMC_MOD_SECURE_EMAIL          "PmcModSecureEmail"      //修改安全邮箱:%s为%s %s
#define     LAN_PMC_USER_TYPE_SUPERADMIN      "PmcUserTypeSuperAdmin"  //超级管理员
#define     LAN_PMC_USER_TYPE_ADMIN           "PmcUserTypeAdmin"       //管理员
#define     LAN_PMC_USER_TYPE_INVALID         "PmcUserTypeInvalid"     //无效用户类型
#define     LAN_PMC_DESC_UPDATE              "PmcUpdate"               //升级设备 %s
#define     LAN_PMC_DESC_EXPORT              "PmcExport"               //导出镜像 %s
#define     LAN_PMC_DESC_IMPORT              "PmcImport"               //导入镜像 %s
#define     LAN_PMC_DESC_RESTORE             "PmcRestore"              //恢复出厂设置 %s
#define     LAN_PMC_SOFTWAREINFO_MOD         "PmcSoftwareInfoModify"      //修改软件信息 %s
#define     LAN_PMC_CTRL_PMS                 "PmcCtrlPms"              //控制(重启或关机)平台系统 %s
#define     LAN_PMC_TIMEINFO_MOD             "PmcTimeInfoModify"          //修改平台时间 %s
#define     LAN_PMC_NETCARDINFO_MOD          "PmcNetCardInfoModdify"       //修改网卡信息 %s
#define     LAN_PMC_NETROUTEINFO_ADD         "PmcNetRouteInfoAdd"       //添加网络路由 %s
#define     LAN_PMC_NETROUTEINFO_DEL         "PmcNetRouteInfoDel"       //删除网络路由 %s
#define     LAN_PMC_NETDEFAULTROUTE_SET      "PmcNetDefaultRouteSet"    //设置默认网关 %s
#define     LAN_PMC_STUNSERVER_MOD           "PmcStunServerModdify"         //修改stun服务配置 %s
#define     LAN_PMC_NETNAT_ADD               "PmcNetnatAdd"             //添加网络NAT %s
#define     LAN_PMC_NETNAT_DEL               "PmcNetnatDel"             //删除网络NAT %s
#define     LAN_PMC_ADD_UP_PLATFORM          "PmcAddUpPlatform"         //入网上级平台%s
#define     LAN_PMC_ADD_DOWN_PLATFORM        "PmcAddDownPlatform"       //入网下级平台%s
#define     LAN_PMC_ADD_SLAVE_PLATFORM       "PmcAddSlavePlatform"      //入网从平台%s
#define     LAN_PMC_ADD_MASTER_PLATFORM      "PmcAddMasterPlatform"     //入网主平台%s
#define     LAN_PMC_DEL_UP_PLATFORM          "PmcDelUpPlatform"         //退网上级平台%s
#define     LAN_PMC_DEL_DOWN_PLATFORM        "PmcDelDownPlatform"       //退网下级平台%s
#define     LAN_PMC_DEL_SLAVE_PLATFORM       "PmcDelSlavePlatform"      //退网从平台%s
#define     LAN_PMC_DEL_MASTER_PLATFORM      "PmcDelMasterPlatform"     //退网主平台%s
#define     LAN_PMC_MOD_PLATFORM             "PmcModPlatform"           //修改平台信息%s
#define     LAN_PMC_MOD_MODULE_CONFIG        "PmcModModuleConfig"       //修改模块信息%s
#define     LAN_PLATFORM_MOD_IPCONFIG        "PlatformModIpconfig"      //修改IP信息(devName:%s ip:%s netmask:%s gw:%s)%s
#define     LAN_PLATFORM_POWERON             "PlatformPoweron"          //平台启动%s
#define     LAN_PLATFORM_POWEROFF            "PlatformPoweroff"         //平台关机%s
#define     LAN_PMC_ADD_DEVICE_MANUFACTURER  "PmcAddDeviceManufacturer" //添加设备厂商(厂商名:%s)%s
#define     LAN_PMC_DEL_DEVICE_MANUFACTURER  "PmcDelDeviceManufacturer" //删除设备厂商(厂商名:%s)%s
#define     LAN_PMC_ADD_DEVICE_MODEL         "PmcAddDeviceModel"        //添加设备型号(型号名:%s)%s
#define     LAN_PMC_DEL_DEVICE_MODEL         "PmcDelDeviceModel"        //删除设备型号(型号id:%s)%s
#define     LAN_PMC_ADD_DEVICE               "PmcAddDevice"             //添加设备(设备名:%s)%s
#define     LAN_PMC_DEL_DEVICE               "PmcDelDevice"             //删除设备(设备id:%s)%s
#define     LAN_PMC_MOD_DEVICE               "PmcModDevice"             //修改设备(设备id:%s)%s
#define     LAN_PMC_LICENSE_UPDATE           "PmcUpdateLicense"         //更新许可证%s
#define     LAN_PMC_ADD_NRU                  "PmcAddNru"                //添加NRU(设备名:%s)%s
#define     LAN_PMC_MOD_NRU                  "PmcModNru"                //修改NRU(设备id:%s)%s
#define     LAN_PMC_DEL_NRU                  "PmcDelNru"                //删除NRU(设备id:%s)%s
#define     LAN_PMC_MOD_NRU_NETWORK          "PmcModNruNetwork"         //修改NRU网络配置(dev:%s)%s
#define     LAN_PMC_FDISK                    "PmcFdisk"                 //磁盘分区(设备名:%s)%s
#define     LAN_PMC_FORMAT_DISK              "PmcFormatDisk"            //磁盘格式化(设备名:%s)%s
#define     LAN_PMC_MOUNT_DISK               "PmcMountDisk"             //挂载磁盘(磁盘名:%s)%s
#define     LAN_PMC_CHECK_DISK               "PmcCheckDisk"             //修复磁盘(磁盘名:%s)%s
#define     LAN_PMC_UNMOUNT_DISK             "PmcUnmountDisk"           //解挂磁盘(磁盘名:%s)%s
#define     LAN_PMC_CANCEL_DISK_OP           "PmcCancelDiskOP"          //取消磁盘操作(磁盘名:%s)%s
#define     LAN_PMC_ADD_ISCSI_CONF           "PmcAddIscsiConf"          //添加ISCSI配置(nruid:%s)%s
#define     LAN_PMC_DEL_ISCSI_CONF           "PmcDelIscsiConf"          //删除ISCSI配置(nruid:%s)%s
#define     LAN_PMC_CTRL_ISCSI               "PmcCtrlIscsi"             //ISCSI操作(%s)%s
#define     LAN_PMC_DISCONNECT_ISCSI         "PmcDisconnectIscsi"       //断开ISCSI连接(nruid:%s)%s
#define     LAN_PMC_DEL_OFFLINE_DISK         "PmcDelOfflineDisk"        //删除离线磁盘(磁盘名:%s)%s
#define     LAN_PMC_SET_MAIL_CFG             "PmcSetMailCfg"            //设置邮箱配置项%s
#define     LAN_PMC_SET_CFG                  "PmcSetCfg"                //国标设置配置项%s
#define     LAN_PMC_ADVANCED_SET_CFG         "PmcAdvancedSetCfg"        //国标高级设置配置项%s
#define     LAN_PMC_ADD_KDDOMAIN             "PmcAddKDDomain"           //国标添加域映射%s
#define     LAN_PMC_DEL_KDDOMAIN             "PmcDelKDDomain"           //国标删除域映射%s
#define     LAN_PMC_ADD_PU                   "PmcAddPu"                 //国标添加PU(devname:%s)%s
#define     LAN_PMC_DEL_PU                   "PmcDelPu"                 //国标删除PU(devid:%s)%s
#define     LAN_PMC_MOD_PU                   "PmcModPu"                 //国标修改PU(devid:%s)%s
#define     LAN_PMC_ADD_CHILD                "PmcAddChild"              //国标添加下级(gbdevid:%s)%s
#define     LAN_PMC_DEL_CHILD                "PmcDelChild"              //国标删除下级(gbdevid:%s)%s
#define     LAN_PMC_MOD_CHILD                "PmcModChild"              //国标修改下级(gbdevid:%s)%s
#define     LAN_PMC_ADD_PARENT               "PmcAddParent"             //国标添加上级(gbdevid:%s)%s
#define     LAN_PMC_DEL_PARENT               "PmcDelParent"             //国标删除上级(gbdevid:%s)%s
#define     LAN_PMC_MOD_PARENT               "PmcModParent"             //国标修改上级(gbdevid:%s)%s
#define     LAN_PMC_RESET_CU_PASSWORD      "PmcResetCUPassword"          //恢复cu超级用户密码%s
#define     LAN_PMC_SET_SSH_IPTABLES         "PmcSetSshIptables"         //设置ssh白名单%s
#define     LAN_PMC_SET_MYSQL_IPTABLES       "PmcSetMysqlIptables"       //设置mysql白名单%s
#define     LAN_PMC_SET_HEARTBEART_HOST      "PmcSetHeartbeatHost"       //设置本机为主机%s
#define     LAN_PMC_SET_HEARTBEART_BAK       "PmcSetHeartbeatBak"        //设置本机为备机%s
#define     LAN_PMC_GET_LICENSE_NOT_EXIST    "PmcGetLicenseNotExist"     //license不存在

//@yu 2014.7.22
#define     LAN_PMC_ADD_PERIPHERAL_DEV              "PmcAddPeripheralDev"             //国标添加外设设备(gbdevid:%s)%s
#define     LAN_PMC_DEL_PERIPHERAL_DEV              "PmcDelPeripheralDev"             //国标删除外设设备(gbdevid:%s)%s
#define     LAN_PMC_MOD_PERIPHERAL_DEV              "PmcModPeripheralDev"             //国标修改外设设备(gbdevid:%s)%s

//@yu 2014.9.23 加入导入设备英文版
#define		LAN_PMC_IMPORT_DEV_TAS_DATABASE_ERROR			"PmcImportDevTasDatabaseError"				//3A操作数据库失败
#define		LAN_PMC_IMPORT_DEV_INVALID_ARGUMENT				"PmcImportDevInvalidArgument"				//参数非法
#define		LAN_PMC_IMPORT_DEV_GBS_DEVINFO_INCOMPLETE		"PmcImportDevGBSDevInfoIncomplete"			//提供的国标编号不够，要求设备和通道都需要指定
#define		LAN_PMC_IMPORT_DEV_INVALID_GB_ID				"PmcImportDevInvalidGBID"					//国标编号不可用，也许是国标编号非法或者非本域域号或者类型不支持或者编号已经被占用
#define		LAN_PMC_IMPORT_DEV_INVALID_GB_DOMAIN			"PmcImportDevInvalidGBDomain"				//国标域号不可用，也许存在相同的域号或存在环
#define		LAN_PMC_IMPORT_DEV_SAME_DEV_MODLE				"PmcImportDevSameDevModel"					//相同的设备型号
#define		LAN_PMC_IMPORT_ERROR_CODE						"PmcImportDevErrorCode"						//错误码是[%d]
#define		LAN_PMC_IMPORT_FILE_CONTENT						"PmcImportDevFileContent"					//设备名,设备型号,IP,入网结果,失败原因
#define     LAN_PMC_IMPORT_DEV_FALIURE						"PmcImportDevFailure"						//失败
#define     LAN_PMC_IMPORT_DEV_SUCCESS						"PmcImportDevSuccess"						//成功
#define		LAN_PMC_IMPORT_DEV_GBS_NOT_SERVICE				"PmcImportDevGbsNotService"					//gbs未在服务态
#define		LAN_PMC_IMPORT_DEV_TAS_NOT_SERVICE				"PmcImportDevTasNotService"					//tas未在服务态
#define		LAN_PMC_IMPORT_DEV_FAILED_RESOLUTION			"PmcImportDevFailedResolution"				//设备信息解析失败
#define		LAN_PMC_IMPORT_DEV_INVALID_DEV_NAME				"PmcImportDevInvalidDevName"				//设备名中含有非法字符
#define		LAN_PMC_IMPORT_DEV_ILLEGAL_DEV_IP				"PmcImportDevIllegalDevIP"					//非法的IP地址
#define		LAN_PMC_IMPORT_DEV_NOT_FIND_DEV_MODEL			"PmcImportDevModelNotFind"					//未找到匹配的设备型号
#define		LAN_PMC_IMPORT_DEV_INVALID_DOMAIN_NAME			"PmcImportDevInvalidDomainName"				//域名未匹配本域
#define		LAN_PMC_IMPORT_DEV_INVALID_KEDA_NUM				"PmcImportDevInvalidKEDANum"				//科达编号非法
//@yu 2014.9.23 加入导出设备英文版
#define		LAN_PMC_EXPORT_DEV_FILE_CONTENT					"PmcExportDevFileContent"	                		//设备名(必填),型号名(必填),IP(必填),用户名(可选),用户密码(可选),设备国标编号(可选),通道国标编号(要与国标编号同时填或者同时不填),域名(可选),设备编号(可选),科达编号(可选),启停标志(可选 1为启动 0为停止),接入线路(可选),是否国标设备(仅导出使用 1为国标设备 0为非国标设备),平台IP,平台端口
#define		LAN_PMC_EXPORT_DEV_GBEXT_FILE_CONTENT		"PmcExportDevGbExtFileContent"			 		//设备号,型号名,厂商,设备型号,摄像机类型扩展,摄像机安装位置,摄像机用途属性,摄像机补光属性,摄像机监视方位属性,摄像机支持的分辨率,虚拟组织所属的业务分组ID,下载倍速范围,空域编码能力,时域编码能力
#define     LAN_PMC_RESET_ADMIN_PASSWORD                     "PmcResetAdminPassword"                   //恢复初始密码%s



#define     LAN_PMC_DESC_EXPORT_USERLOG      "PmcExportUserLog"         //导出用户操作日志 %s
#define     LAN_PMC_DESC_EXPORT_PLATFORMLOG  "PmcExportPlatformLog"     //导出平台 %s
#define     LAN_PMC_DESC_EXPORT_NMSHISTORYALARM          "PmcExportNmsHistoryLog"                //导出历史告警 %s
#define     LAN_PMC_DESC_IMPORT_DEV          "PmcImportDev"             //批量导入设备 %s
#define     LAN_PMC_DESC_EXPORT_DEV          "PmcExportDev"             //批量导出设备 %s
#define     LAN_PMC_DESC_IMPORT_DEV_GBEXT    "PmcImportDevGbExt"             //批量导入设备国标扩展信息 %s
#define     LAN_PMC_DESC_EXPORT_DEV_GBEXT    "PmcExportDevGbExt"             //批量导出设备国标扩展信息 %s
#define     LAN_CMSSDK_PLAT_REC_PLAY_START   "CmssdkPlatRecPlayStart"   //平台录像回放开启%s, 设备uri: %s, token: %s
#define     LAN_CMSSDK_PU_REC_PLAY_START     "CmssdkPuRecPlayStart"     //前端录像回放开启%s, 设备uri: %s, token: %s
#define     LAN_CMSSDK_PLAT_REC_PLAY_STOP    "CmssdkPlatRecPlayStop"    //平台录像回放停止%s, 设备uri: %s, token: %s
#define     LAN_CMSSDK_PU_REC_PLAY_STOP      "CmssdkPuRecPlayStop"      //前端录像回放停止%s, 设备uri: %s, token: %s
#define     LAN_CMSSDK_PLAT_REC_DOWNLOAD_START     "CmssdkPlatRecDownloadStart"    //平台录像下载开启%s, 设备uri: %s, token: %s
#define     LAN_CMSSDK_PU_REC_DOWNLOAD_START       "CmssdkPuRecDownloadStart"      //前端录像下载开启%s, 设备uri: %s, token: %s
#define     LAN_CMSSDK_PLAT_REC_DOWNLOAD_STOP     "CmssdkPlatRecDownloadStop"     //平台录像下载开启%s, 设备uri: %s, token: %s
#define     LAN_CMSSDK_PU_REC_DOWNLOAD_STOP       "CmssdkPuRecDownloadStop"       //前端录像下载停止%s, 设备uri: %s, token: %s

#define    LAN_ALARM_TYPE_PIN             "AlarmTypePin"             //并口
#define    LAN_ALARM_TYPE_MOVE            "AlarmTypeMove"            //移动监测
#define    LAN_ALARM_TYPE_VIDLOST         "AlarmTypeVidlost"         //视频源丢失
#define    LAN_ALARM_TYPE_DISKFULL        "AlarmTypeDiskfull"        //磁盘满
#define    LAN_ALARM_TYPE_INTELLIGENT     "AlarmTypeIntelligent"     //智能
#define    LAN_ALARM_TYPE_VIDEOPOLL       "AlarmTypeVideopoll"       //视质轮询
#define    LAN_ALARM_TYPE_EXCEPTION_EVENT "AlarmTypeExceptionEvent"  //异常事件消息
#define    LAN_ALARM_TYPE_DEFOCUS         "AlarmTypeDefocus"         //虚焦检测
#define    LAN_ALARM_TYPE_SCENE_CHANGE    "AlarmTypeScemeChange"     //场景变换
#define    LAN_ALARM_TYPE_REGION_INVASION "AlarmTypeRegionInvasion"  //区域入侵
#define    LAN_ALARM_TYPE_REGION_LEAVING  "AlarmTypeRegionLeaving"   //区域离开
#define    LAN_ALARM_TYPE_OBJECT_TAKEN    "AlarmTypeObjectTaken"     //物品拿取
#define    LAN_ALARM_TYPE_OBJECT_LEFT     "AlarmTypeObjectLeft"      //物品遗留
#define    LAN_ALARM_TYPE_PEOPLE_GATHER   "AlarmTypePeopleGather"    //人员聚集
#define    LAN_ALARM_TYPE_CORGON          "AlarmTypeCorgon"          //警戒线
#define    LAN_ALARM_TYPE_AUDIO_ABNORMAL  "AlarmTypeAudioAbnormal"   //声音异常
#define    LAN_ALARM_TYPE_LUMA_CHANGE     "AlarmTypeLumaChange"      //亮度变化

#define 	LAN_ALARM_TYPE_ILLEGAL_DEV_DISASSEMBLY	 "AlarmTypeIllegalDevDisassembly"
#define 	LAN_ALARM_TYPE_DEVICE_OVER_HEAT			 "AlarmTypeDeviceOverHeat"
#define 	LAN_ALARM_TYPE_DEVICE_OVER_COLD			 "AlarmTypeDeviceOverCold"
#define 	LAN_ALARM_TYPE_ARTIFICIAL_SCENE			 "AlarmTypeArtificialScene"
#define 	LAN_ALARM_TYPE_OBJECT_MOTION			 "AlarmTypeObjectMotion"
#define 	LAN_ALARM_TYPE_TRIP_WIRE				 "AlarmTypeTripWire"
#define 	LAN_ALARM_TYPE_MOVING_AROUND			 "AlarmTypeMovingAround"
#define 	LAN_ALARM_TYPE_INDIVIDUAL_FLOW_STATISTICS		 "AlarmTypeIndividualFlowStatistics"
#define 	LAN_ALARM_TYPE_INDIVIDUAL_THICKNESS_STATISTICS	 "AlarmTypeIndividualThicknessStatistics"
#define 	LAN_ALARM_TYPE_VIDEO_EXCEPTION			 "AlarmTypeVideoException"
#define 	LAN_ALARM_TYPE_MOVING_SWIFTLY			 "AlarmTypeMovingSwiftly"
#define 	LAN_ALARM_TYPE_STORAGE_DEV_DISK_FAULT	 "AlarmTypeStorageDevDiskFault"
#define 	LAN_ALARM_TYPE_STORAGE_DEV_FAN_FAULT	 "AlarmTypeStorageDevFanFault"
#define 	LAN_ALARM_TYPE_GO_AGAINST_TRAFFIC        "AlarmTypeGoAgainstTraffic"
#define 	LAN_ALARM_TYPE_GPS_OVERSPEED	            "AlarmTypeGpsOverspeed"
#define 	LAN_ALARM_TYPE_SIM_DATA_SERVICE_OVERUSED	"AlarmTypeSimDataServiceOverused"


///////////////////////////////////////////////////////////////////////////////////////////////////
#define LAN_PMS_ALARM_DEVICE_MODEL_NOT_MATCHING                        "PmsAlarmDeviceModelNotMatching"
#define LAN_PMS_ALARM_DEVICE_MODEL_NOT_MATCHING_RESTORE                "PmsAlarmDeviceModelNotMatchingRestore"
#define LAN_PMS_ALARM_DEVICE_MODEL_NOT_MATCHING_RESUME                 "PmsAlarmDeviceModelNotMatchingResume"
#define LAN_PMS_ALARM_DOMAIN_ALIAS_NOT_VALID                           "PmsAlarmDomainAliasNotValid"
#define LAN_PMS_ALARM_DOMAIN_ALIAS_NOT_VALID_RESTORE                   "PmsAlarmDomainAliasNotValidRestore"
#define LAN_PMS_ALARM_DOMAIN_ALIAS_NOT_VALID_RESUME                    "PmsAlarmDomainAliasNotValidResume"
#define LAN_PMS_ALARM_DOMAIN_RELATION_NOT_LEGAL                        "PmsAlarmDomainRelationNotLegal"
#define LAN_PMS_ALARM_DOMAIN_RELATION_NOT_LEGAL_RESTORE                "PmsAlarmDomainRelationNotLegalRestore"
#define LAN_PMS_ALARM_DOMAIN_RELATION_NOT_LEGAL_RESUME                 "PmsAlarmDomainRelationNotLegalResume"
#define LAN_PMS_ALARM_SUBDOMAIN_DIFF_KDMNO                             "PmsAlarmSubDomainDiffKdmNO"
#define LAN_PMS_ALARM_SUBDOMAIN_DIFF_KDMNO_RESTORE                     "PmsAlarmSubDomainDiffKdmNORestore"
#define LAN_PMS_ALARM_SUBDOMAIN_DIFF_KDMNO_RESUME                      "PmsAlarmSubDomainDiffKdmNOResume"
#define LAN_PMS_ALARM_DEVICE_OFFLINE                                   "PmsAlarmDeviceOffline"
#define LAN_PMS_ALARM_DEVICE_OFFLINE_RESTORE                           "PmsAlarmDeviceOfflineRestore"
#define LAN_PMS_ALARM_DEVICE_OFFLINE_RESUME                            "PmsAlarmDeviceOfflineResume"
#define LAN_PMS_ALARM_VTDU_STREAM_CHANGING                             "PmsAlarmVtduStreamChanging"
#define LAN_PMS_ALARM_VTDU_STREAM_CHANGING_RESTORE                     "PmsAlarmVtduStreamChangingRestore"
#define LAN_PMS_ALARM_VTDU_STREAM_CHANGING_RESUME                      "PmsAlarmVtduStreamChangingResume"
#define LAN_PMS_ALARM_RECEIVE_NO_BIT_STREAM                            "PmsAlarmReceiveNoBitStream"
#define LAN_PMS_ALARM_RECEIVE_NO_BIT_STREAM_RESTORE                    "PmsAlarmReceiveNoBitStreamRestore"
#define LAN_PMS_ALARM_RECEIVE_NO_BIT_STREAM_RESUME                     "PmsAlarmReceiveNoBitStreamResume"
#define LAN_PMS_ALARM_LOST_PACKET                                      "PmsAlarmLostPacket"
#define LAN_PMS_ALARM_LOST_PACKET_RESTORE                              "PmsAlarmLostPacketRestore"
#define LAN_PMS_ALARM_LOST_PACKET_RESUME                               "PmsAlarmLostPacketResume"
#define LAN_PMS_ALARM_MAP_STATUS                                       "PmsAlarmMapStatus"
#define LAN_PMS_ALARM_MAP_STATUS_RESTORE                               "PmsAlarmMapStatusRestore"
#define LAN_PMS_ALARM_MAP_STATUS_RESUME                                "PmsAlarmMapStatusResume"
#define LAN_PMS_ALARM_MOTION_DETECT_STATUS                             "PmsAlarmMotionDetectStatus"
#define LAN_PMS_ALARM_MOTION_DETECT_STATUS_RESTORE                     "PmsAlarmMotionDetectStatusRestore"
#define LAN_PMS_ALARM_MOTION_DETECT_STATUS_RESUME                      "PmsAlarmMotionDetectStatusResume"
#define LAN_PMS_ALARM_LPT_STATUS                                       "PmsAlarmLptStatus"
#define LAN_PMS_ALARM_LPT_STATUS_RESTORE                               "PmsAlarmLptStatusRestore"
#define LAN_PMS_ALARM_LPT_STATUS_RESUME                                "PmsAlarmLptStatusResume"
#define LAN_PMS_ALARM_POWER_STATUS                                     "PmsAlarmPowerStatus"
#define LAN_PMS_ALARM_POWER_STATUS_RESTORE                             "PmsAlarmPowerStatusRestore"
#define LAN_PMS_ALARM_POWER_STATUS_RESUME                              "PmsAlarmPowerStatusResume"
#define LAN_PMS_ALARM_CPU_STATUS                                       "PmsAlarmCpuStatus"
#define LAN_PMS_ALARM_CPU_STATUS_RESTORE                               "PmsAlarmCpuStatusRestore"
#define LAN_PMS_ALARM_CPU_STATUS_RESUME                                "PmsAlarmCpuStatusResume"
#define LAN_PMS_ALARM_MEMORY_STATUS                                    "PmsAlarmMemoryStatus"
#define LAN_PMS_ALARM_MEMORY_STATUS_RESTORE                            "PmsAlarmMemoryStatusRestore"
#define LAN_PMS_ALARM_MEMORY_STATUS_RESUME                             "PmsAlarmMemoryStatusResume"
#define LAN_PMS_ALARM_GPS_STORE_CONFIG_ERROR                           "PmsAlarmGpsStoreConfigError"
#define LAN_PMS_ALARM_GPS_STORE_CONFIG_ERROR_RESTORE                   "PmsAlarmGpsStoreConfigErrorRestore"
#define LAN_PMS_ALARM_GPS_STORE_CONFIG_ERROR_RESUME                    "PmsAlarmGpsStoreConfigErrorResume"
#define LAN_PMS_ALARM_DISK_FULL_STATUS                                 "PmsAlarmDiskFullStatus"
#define LAN_PMS_ALARM_DISK_FULL_STATUS_RESTORE                         "PmsAlarmDiskFullStatusRestore"
#define LAN_PMS_ALARM_DISK_FULL_STATUS_RESUME                          "PmsAlarmDiskFullStatusResume"
#define LAN_PMS_ALARM_FAN_STATUS                                       "PmsAlarmFanStatus"
#define LAN_PMS_ALARM_FAN_STATUS_RESTORE                               "PmsAlarmFanStatusRestore"
#define LAN_PMS_ALARM_FAN_STATUS_RESUME                                "PmsAlarmFanStatusResume"
#define LAN_PMS_ALARM_ETH_CARD_STATUS                                  "PmsAlarmEthCardStatus"
#define LAN_PMS_ALARM_ETH_CARD_STATUS_RESTORE                          "PmsAlarmEthCardStatusRestore"
#define LAN_PMS_ALARM_ETH_CARD_STATUS_RESUME                           "PmsAlarmEthCardStatusResume"
#define LAN_PMS_ALARM_NET_CARD_OFFLINE                                 "PmsAlarmNetCardOffline"
#define LAN_PMS_ALARM_NET_CARD_OFFLINE_RESTORE                         "PmsAlarmNetCardOfflineRestore"
#define LAN_PMS_ALARM_NET_CARD_OFFLINE_RESUME                          "PmsAlarmNetCardOfflineResume"
#define LAN_PMS_ALARM_LOST_VIDEO_SOURCE_STATUS                         "PmsAlarmLostVideoSourceStatus"
#define LAN_PMS_ALARM_LOST_VIDEO_SOURCE_STATUS_RESTORE                 "PmsAlarmLostVideoSourceStatusRestore"
#define LAN_PMS_ALARM_LOST_VIDEO_SOURCE_STATUS_RESUME                  "PmsAlarmLostVideoSourceStatusResume"
#define LAN_PMS_ALARM_REG_CMU_ERROR                                    "PmsAlarmRegCmuError"
#define LAN_PMS_ALARM_REG_CMU_ERROR_RESTORE                            "PmsAlarmRegCmuErrorRestore"
#define LAN_PMS_ALARM_REG_CMU_ERROR_RESUME                             "PmsAlarmRegCmuErrorResume"
#define LAN_PMS_ALARM_DISK_DETECT_ERROR                                "PmsAlarmDiskDetectError"
#define LAN_PMS_ALARM_DISK_DETECT_ERROR_RESTORE                        "PmsAlarmDiskDetectErrorRestore"
#define LAN_PMS_ALARM_DISK_DETECT_ERROR_RESUME                         "PmsAlarmDiskDetectErrorResume"
#define LAN_PMS_ALARM_CU_OVERLOAD                                      "PmsAlarmCuOverload"
#define LAN_PMS_ALARM_CU_OVERLOAD_RESTORE                              "PmsAlarmCuOverloadRestore"
#define LAN_PMS_ALARM_CU_OVERLOAD_RESUME                               "PmsAlarmCuOverloadResume"
#define LAN_PMS_ALARM_PU_OVERLOAD                                      "PmsAlarmPuOverload"
#define LAN_PMS_ALARM_PU_OVERLOAD_RESTORE                              "PmsAlarmPuOverloadRestore"
#define LAN_PMS_ALARM_PU_OVERLOAD_RESUME                               "PmsAlarmPuOverloadResume"
#define LAN_PMS_ALARM_VTDU_OVERLOAD                                    "PmsAlarmVtduOverload"
#define LAN_PMS_ALARM_VTDU_OVERLOAD_RESTORE                            "PmsAlarmVtduOverloadRestore"
#define LAN_PMS_ALARM_VTDU_OVERLOAD_RESUME                             "PmsAlarmVtduOverloadResume"
#define LAN_PMS_ALARM_NO_DISK_ERROR                                    "PmsAlarmNoDiskError"
#define LAN_PMS_ALARM_NO_DISK_ERROR_RESTORE                            "PmsAlarmNoDiskErrorRestore"
#define LAN_PMS_ALARM_NO_DISK_ERROR_RESUME                             "PmsAlarmNoDiskErrorResume"
#define LAN_PMS_ALARM_DISK_ERROR                                       "PmsAlarmDiskError"
#define LAN_PMS_ALARM_DISK_ERROR_RESTORE                               "PmsAlarmDiskErrorRestore"
#define LAN_PMS_ALARM_DISK_ERROR_RESUME                                "PmsAlarmDiskErrorResume"
#define LAN_PMS_ALARM_DISK_NO_FORMAT                                   "PmsAlarmDiskNoFormat"
#define LAN_PMS_ALARM_DISK_NO_FORMAT_RESTORE                           "PmsAlarmDiskNoFormatRestore"
#define LAN_PMS_ALARM_DISK_NO_FORMAT_RESUME                            "PmsAlarmDiskNoFormatResume"
#define LAN_PMS_ALARM_STOP_RECORD_LOG_ERROR                            "PmsAlarmStopRecordLogError"
#define LAN_PMS_ALARM_STOP_RECORD_LOG_ERROR_RESTORE                    "PmsAlarmStopRecordLogErrorRestore"
#define LAN_PMS_ALARM_STOP_RECORD_LOG_ERROR_RESUME                     "PmsAlarmStopRecordLogErrorResume"
#define LAN_PMS_ALARM_STOP_RECORD_DISK_ERROR                           "PmsAlarmStopRecordDiskError"
#define LAN_PMS_ALARM_STOP_RECORD_DISK_ERROR_RESTORE                   "PmsAlarmStopRecordDiskErrorRestore"
#define LAN_PMS_ALARM_STOP_RECORD_DISK_ERROR_RESUME                    "PmsAlarmStopRecordDiskErrorResume"
#define LAN_PMS_ALARM_DISCONNECT_TAS                                   "PmsAlarmDisconnectTas"
#define LAN_PMS_ALARM_DISCONNECT_TAS_RESTORE                           "PmsAlarmDisconnectTasRestore"
#define LAN_PMS_ALARM_DISCONNECT_TAS_RESUME                            "PmsAlarmDisconnectTasResume"
#define LAN_PMS_ALARM_DISCONNECT_UAS                                   "PmsAlarmDisconnectUas"
#define LAN_PMS_ALARM_DISCONNECT_UAS_RESTORE                           "PmsAlarmDisconnectUasRestore"
#define LAN_PMS_ALARM_DISCONNECT_UAS_RESUME                            "PmsAlarmDisconnectUasResume"
#define LAN_PMS_ALARM_DISCONNECT_PUI                                   "PmsAlarmDisconnectPui"
#define LAN_PMS_ALARM_DISCONNECT_PUI_RESTORE                           "PmsAlarmDisconnectPuiRestore"
#define LAN_PMS_ALARM_DISCONNECT_PUI_RESUME                            "PmsAlarmDisconnectPuiResume"
#define LAN_PMS_ALARM_DISCONNECT_CUI                                   "PmsAlarmDisconnectCui"
#define LAN_PMS_ALARM_DISCONNECT_CUI_RESTORE                           "PmsAlarmDisconnectCuiRestore"
#define LAN_PMS_ALARM_DISCONNECT_CUI_RESUME                            "PmsAlarmDisconnectCuiResume"
#define LAN_PMS_ALARM_DISCONNECT_NRU                                   "PmsAlarmDisconnectNru"
#define LAN_PMS_ALARM_DISCONNECT_NRU_RESTORE                           "PmsAlarmDisconnectNruRestore"
#define LAN_PMS_ALARM_DISCONNECT_NRU_RESUME                            "PmsAlarmDisconnectNruResume"
#define LAN_PMS_ALARM_DISCONNECT_VTDU                                  "PmsAlarmDisconnectVtdu"
#define LAN_PMS_ALARM_DISCONNECT_VTDU_RESTORE                          "PmsAlarmDisconnectVtduRestore"
#define LAN_PMS_ALARM_DISCONNECT_VTDU_RESUME                           "PmsAlarmDisconnectVtduResume"
#define LAN_PMS_ALARM_DISCONNECT_CMU                                   "PmsAlarmDisconnectCmu"
#define LAN_PMS_ALARM_DISCONNECT_CMU_RESTORE                           "PmsAlarmDisconnectCmuRestore"
#define LAN_PMS_ALARM_DISCONNECT_CMU_RESUME                            "PmsAlarmDisconnectCmuResume"
#define LAN_PMS_ALARM_DISCONNECT_DB                                    "PmsAlarmDisconnectDb"
#define LAN_PMS_ALARM_DISCONNECT_DB_RESTORE                            "PmsAlarmDisconnectDbRestore"
#define LAN_PMS_ALARM_DISCONNECT_DB_RESUME                             "PmsAlarmDisconnectDbResume"
#define LAN_PMS_ALARM_DISCONNECT_USBKEY                                "PmsAlarmDisconnectUsbKey"
#define LAN_PMS_ALARM_DISCONNECT_USBKEY_RESTORE                        "PmsAlarmDisconnectUsbKeyRestore"
#define LAN_PMS_ALARM_DISCONNECT_USBKEY_RESUME                         "PmsAlarmDisconnectUsbKeyResume"
#define LAN_PMS_ALARM_TERM_REMIND_USBKEY                               "PmsAlarmTermRemindUsbKey"
#define LAN_PMS_ALARM_TERM_REMIND_USBKEY_RESTORE                       "PmsAlarmTermRemindUsbKeyRestore"
#define LAN_PMS_ALARM_TERM_REMIND_USBKEY_RESUME                        "PmsAlarmTermRemindUsbKeyResume"
#define LAN_PMS_ALARM_LICENSE_INVALID                                  "PmsAlarmLicenseInvalid"
#define LAN_PMS_ALARM_LICENSE_INVALID_RESTORE                          "PmsAlarmLicenseInvalidRestore"
#define LAN_PMS_ALARM_LICENSE_INVALID_RESUME                           "PmsAlarmLicenseInvalidResume"
#define LAN_PMS_ALARM_DISK_PART_READ_ONLY                              "PmsAlarmDiskPartReadOnly"
#define LAN_PMS_ALARM_DISK_PART_READ_ONLY_RESTORE                      "PmsAlarmDiskPartReadOnlyRestore"
#define LAN_PMS_ALARM_DISK_PART_READ_ONLY_RESUME                       "PmsAlarmDiskPartReadOnlyResume"
#define LAN_PMS_ALARM_DISK_FILE_SYSTEM_ERROR                           "PmsAlarmDiskFileSystemError"
#define LAN_PMS_ALARM_DISK_FILE_SYSTEM_ERROR_RESTORE                   "PmsAlarmDiskFileSystemErrorRestore"
#define LAN_PMS_ALARM_DISK_FILE_SYSTEM_ERROR_RESUME                    "PmsAlarmDiskFileSystemErrorResume"
#define LAN_PMS_ALARM_DISK_OFFLINE                                     "PmsAlarmDiskOffline"
#define LAN_PMS_ALARM_DISK_OFFLINE_RESTORE                             "PmsAlarmDiskOfflineRestore"
#define LAN_PMS_ALARM_DISK_OFFLINE_RESUME                              "PmsAlarmDiskOfflineResume"
#define LAN_PMS_ALARM_DISK_ABNORMAL                                    "PmsAlarmDiskAbnormal"
#define LAN_PMS_ALARM_DISK_ABNORMAL_RESTORE                            "PmsAlarmDiskAbnormalRestore"
#define LAN_PMS_ALARM_DISK_ABNORMAL_RESUME                             "PmsAlarmDiskAbnormalResume"
#define LAN_PMS_ALARM_RAID_ABNORMAL                                    "PmsAlarmRaidAbnormal"
#define LAN_PMS_ALARM_RAID_ABNORMAL_RESTORE                            "PmsAlarmRaidAbnormalRestore"
#define LAN_PMS_ALARM_RAID_ABNORMAL_RESUME                             "PmsAlarmRaidAbnormalResume"
#define LAN_PMS_ALARM_PLATFORM_IN_REPLACE_MODE                         "PmsAlarmPlatformInReplaceMode"
#define LAN_PMS_ALARM_PLATFORM_IN_REPLACE_MODE_RESTORE                 "PmsAlarmPlatformInReplaceModeRestore"
#define LAN_PMS_ALARM_PLATFORM_IN_REPLACE_MODE_RESUME                  "PmsAlarmPlatformInReplaceModeResume"
#define LAN_PMS_ALARM_DETECT_NULL_IP_NIC                               "PmsAlarmDetectNullIpNic"
#define LAN_PMS_ALARM_DETECT_NULL_IP_NIC_RESTORE                       "PmsAlarmDetectNullIpNicRestore"
#define LAN_PMS_ALARM_DETECT_NULL_IP_NIC_RESUME                        "PmsAlarmDetectNullIpNicResume"
#define LAN_PMS_ALARM_DISCONNECT_SUPER_CMU                             "PmsAlarmDistinctSuperCmu"
#define LAN_PMS_ALARM_DISCONNECT_SUPER_CMU_RESTORE                     "PmsAlarmDistinctSuperCmuRestore"
#define LAN_PMS_ALARM_DISCONNECT_SUPER_CMU_RESUME                      "PmsAlarmDistinctSuperCmuResume"
#define LAN_PMS_ALARM_DISCONNECT_SUB_CMU                               "PmsAlarmDistinctSubCmu"
#define LAN_PMS_ALARM_DISCONNECT_SUB_CMU_RESTORE                       "PmsAlarmDistinctSubCmuRestore"
#define LAN_PMS_ALARM_DISCONNECT_SUB_CMU_RESUME                        "PmsAlarmDistinctSubCmuResume"
#define LAN_PMS_ALARM_DISCONNECT_BCMU_CMU                              "PmsAlarmDistinctBcmuCmu"
#define LAN_PMS_ALARM_DISCONNECT_BCMU_CMU_RESTORE                      "PmsAlarmDistinctBcmuCmuRestore"
#define LAN_PMS_ALARM_DISCONNECT_BCMU_CMU_RESUME                       "PmsAlarmDistinctBcmuCmuResume"
#define LAN_PMS_ALARM_PLATFORM_DISK_NUM_OVERLOAD                       "PmsAlarmPlatformDiskNumOverload"
#define LAN_PMS_ALARM_PLATFORM_DISK_NUM_OVERLOAD_RESTORE               "PmsAlarmPlatformDiskNumOverloadRestore"
#define LAN_PMS_ALARM_PLATFORM_DISK_NUM_OVERLOAD_RESUME                "PmsAlarmPlatformDiskNumOverloadResume"
#define LAN_PMS_ALARM_NRU_DISK_ABNORMAL                                "PmsAlarmNruDiskAbnormal"
#define LAN_PMS_ALARM_NRU_DISK_ABNORMAL_RESTORE                        "PmsAlarmNruDiskAbnormalRestore"
#define LAN_PMS_ALARM_NRU_DISK_ABNORMAL_RESUME                         "PmsAlarmNruDiskAbnormalResume"
#define LAN_PMS_ALARM_NRU_RECORD_TOO_MUCH                              "PmsAlarmNruRecordTooMuch"
#define LAN_PMS_ALARM_NRU_RECORD_TOO_MUCH_RESTORE                      "PmsAlarmNruRecordTooMuchRestore"
#define LAN_PMS_ALARM_NRU_RECORD_TOO_MUCH_RESUME                       "PmsAlarmNruRecordTooMuchResume"
#define LAN_PMS_ALARM_CU_TOO_MUCH                                      "PmsAlarmCuTooMuch"
#define LAN_PMS_ALARM_CU_TOO_MUCH_RESTORE                              "PmsAlarmCuTooMuchRestore"
#define LAN_PMS_ALARM_CU_TOO_MUCH_RESUME                               "PmsAlarmCuTooMuchResume"
#define LAN_PMS_ALARM_PU_TOO_MUCH                                      "PmsAlarmPuTooMuch"
#define LAN_PMS_ALARM_PU_TOO_MUCH_RESTORE                              "PmsAlarmPuTooMuchRestore"
#define LAN_PMS_ALARM_PU_TOO_MUCH_RESUME                               "PmsAlarmPuTooMuchResume"
#define LAN_PMS_ALARM_VTDU_SWITCH_TOO_MUCH                             "PmsAlarmVtduSwitchTooMuch"
#define LAN_PMS_ALARM_VTDU_SWITCH_TOO_MUCH_RESTORE                     "PmsAlarmVtduSwitchTooMuchRestore"
#define LAN_PMS_ALARM_VTDU_SWITCH_TOO_MUCH_RESUME                      "PmsAlarmVtduSwitchTooMuchResume"
#define LAN_PMS_ALARM_MODULE_DISCONNECT                                "PmsAlarmModuleDisconnect"
#define LAN_PMS_ALARM_MODULE_DISCONNECT_RESTORE                        "PmsAlarmModuleDisconnectRestore"
#define LAN_PMS_ALARM_MODULE_DISCONNECT_RESUME                         "PmsAlarmModuleDisconnectResume"
#define LAN_PMS_ALARM_USBBAK_DISK_FULL                                 "PmsAlarmUsbBakDiskFull"
#define LAN_PMS_ALARM_USBBAK_DISK_FULL_RESTORE                         "PmsAlarmUsbBakDiskFullRestore"
#define LAN_PMS_ALARM_USBBAK_DISK_FULL_RESUME                          "PmsAlarmUsbBakDiskFullResume"
#define LAN_PMS_ALARM_MIRROR_EXPORT_FAIL                               "PmsAlarmMirrorExportFail"
#define LAN_PMS_ALARM_MIRROR_EXPORT_FAIL_RESTORE                       "PmsAlarmMirrorExportFailRestore"
#define LAN_PMS_ALARM_MIRROR_EXPORT_FAIL_RESUME                        "PmsAlarmMirrorExportFailResume"
#define LAN_PMS_ALARM_RCS_DISK_FULL                                    "PmsAlarmRcsDiskFull"
#define LAN_PMS_ALARM_RCS_DISK_FULL_RESTORE                            "PmsAlarmRcsDiskFullRestore"
#define LAN_PMS_ALARM_RCS_DISK_FULL_RESUME                             "PmsAlarmRcsDiskFullResume"
#define LAN_PMS_ALARM_RCS_RUNNING_ERROR                                "PmsAlarmRcsRunningError"
#define LAN_PMS_ALARM_RCS_RUNNING_ERROR_RESTORE                        "PmsAlarmRcsRunningErrorRestore"
#define LAN_PMS_ALARM_RCS_RUNNING_ERROR_RESUME                         "PmsAlarmRcsRunningErrorResume"
#define LAN_PMS_ALARM_DISK_IO_STATUS                                   "PmsAlarmDiskIoStatus"
#define LAN_PMS_ALARM_DISK_IO_STATUS_RESTORE                           "PmsAlarmDiskIoStatusRestore"
#define LAN_PMS_ALARM_DISK_IO_STATUS_RESUME                            "PmsAlarmDiskIoStatusResume"
#define LAN_PMS_ALARM_NETCARD_IO_STATUS                                "PmsAlarmNetCardIoStatus"
#define LAN_PMS_ALARM_NETCARD_IO_STATUS_RESTORE                        "PmsAlarmNetCardIoStatusRestore"
#define LAN_PMS_ALARM_NETCARD_IO_STATUS_RESUME                         "PmsAlarmNetCardIoStatusResume"
#define LAN_PMS_ALARM_MIRROR_IMPORT_FAIL                               "PmsAlarmMirrorImportFail"
#define LAN_PMS_ALARM_MIRROR_IMPORT_FAIL_RESTORE                       "PmsAlarmMirrorImportFailRestore"
#define LAN_PMS_ALARM_MIRROR_IMPORT_FAIL_RESUME                        "PmsAlarmMirrorImportFailResume"
#define LAN_PMS_ALARM_USBBAK_DISK_DAMAGED                              "PmsAlarmUsbBakDiskDamaged"
#define LAN_PMS_ALARM_USBBAK_DISK_DAMAGED_RESTORE                      "PmsAlarmUsbBakDiskDamagedRestore"
#define LAN_PMS_ALARM_USBBAK_DISK_DAMAGED_RESUME                       "PmsAlarmUsbBakDiskDamagedResume"
#define LAN_PMS_ALARM_CMU_PU_OFFLINE                                   "PmsAlarmCmuPuOffline"
#define LAN_PMS_ALARM_CMU_PU_OFFLINE_RESTORE                           "PmsAlarmCmuPuOfflineRestore"
#define LAN_PMS_ALARM_CMU_PU_OFFLINE_RESUME                            "PmsAlarmCmuPuOfflineResume"
#define LAN_PMS_ALARM_TAS_ADD_PLATFORM_LOOP                            "PmsAlarmTasAddPlatformLoop"
#define LAN_PMS_ALARM_TAS_ADD_PLATFORM_LOOP_RESTORE                    "PmsAlarmTasAddPlatformLoopRestore"
#define LAN_PMS_ALARM_TAS_ADD_PLATFORM_LOOP_RESUME                     "PmsAlarmTasAddPlatformLoopResume"
#define LAN_PMS_ALARM_RMS_DELETE_RECORD                                "PmsAlarmRmsDeleteRecord"
#define LAN_PMS_ALARM_RMS_DELETE_RECORD_RESTORE                        "PmsAlarmRmsDeleteRecordRestore"
#define LAN_PMS_ALARM_RMS_DELETE_RECORD_RESUME                         "PmsAlarmRmsDeleteRecordResume"
#define LAN_PMS_ALARM_PLATFORM_TIME_ERROR                              "PmsAlarmPlatformTimeError"
#define LAN_PMS_ALARM_PLATFORM_TIME_ERROR_RESTORE                      "PmsAlarmPlatformTimeErrorRestore"
#define LAN_PMS_ALARM_PLATFORM_TIME_ERROR_RESUME                       "PmsAlarmPlatformTimeErrorResume"
#define LAN_PMS_ALARM_USBBAK_DISK_NOT_EXIST                            "PmsAlarmUsbBakDiskNotExist"
#define LAN_PMS_ALARM_USBBAK_DISK_NOT_EXIST_RESTORE                    "PmsAlarmUsbBakDiskNotExistRestore"
#define LAN_PMS_ALARM_USBBAK_DISK_NOT_EXIST_RESUME                     "PmsAlarmUsbBakDiskNotExistResume"
#define LAN_PMS_ALARM_OPT_DISK_FULL_STATUS                             "PmsAlarmOptDiskFullStatus"
#define LAN_PMS_ALARM_OPT_DISK_FULL_STATUS_RESTORE                     "PmsAlarmOptDiskFullStatusRestore"
#define LAN_PMS_ALARM_OPT_DISK_FULL_STATUS_RESUME                      "PmsAlarmOptDiskFullStatusResume"
#define LAN_PMS_ALARM_DB_DISK_FULL_STATUS                              "PmsAlarmDbDiskFullStatus"
#define LAN_PMS_ALARM_DB_DISK_FULL_STATUS_RESTORE                      "PmsAlarmDbDiskFullStatusRestore"
#define LAN_PMS_ALARM_DB_DISK_FULL_STATUS_RESUME                       "PmsAlarmDbDiskFullStatusResume"
#define LAN_PMS_ALARM_LOG_DISK_FULL_STATUS                             "PmsAlarmLogDiskFullStatus"
#define LAN_PMS_ALARM_LOG_DISK_FULL_STATUS_RESTORE                     "PmsAlarmLogDiskFullStatusRestore"
#define LAN_PMS_ALARM_LOG_DISK_FULL_STATUS_RESUME                      "PmsAlarmLogDiskFullStatusResume"
#define LAN_PMS_ALARM_OPT_DISK_PART_READ_ONLY                          "PmsAlarmOptDiskPartReadOnly"
#define LAN_PMS_ALARM_OPT_DISK_PART_READ_ONLY_RESTORE                  "PmsAlarmOptDiskPartReadOnlyRestore"
#define LAN_PMS_ALARM_OPT_DISK_PART_READ_ONLY_RESUME                   "PmsAlarmOptDiskPartReadOnlyResume"
#define LAN_PMS_ALARM_DB_DISK_PART_READ_ONLY                           "PmsAlarmDbDiskPartReadOnly"
#define LAN_PMS_ALARM_DB_DISK_PART_READ_ONLY_RESTORE                   "PmsAlarmDbDiskPartReadOnlyRestore"
#define LAN_PMS_ALARM_DB_DISK_PART_READ_ONLY_RESUME                    "PmsAlarmDbDiskPartReadOnlyResume"
#define LAN_PMS_ALARM_LOG_DISK_PART_READ_ONLY                          "PmsAlarmLogDiskPartReadOnly"
#define LAN_PMS_ALARM_LOG_DISK_PART_READ_ONLY_RESTORE                  "PmsAlarmLogDiskPartReadOnlyRestore"
#define LAN_PMS_ALARM_LOG_DISK_PART_READ_ONLY_RESUME                   "PmsAlarmLogDiskPartReadOnlyResume"
#define LAN_PMS_ALARM_USBBAK_DISK_PART_READ_ONLY                       "PmsAlarmUsbBakDiskPartReadOnly"
#define LAN_PMS_ALARM_USBBAK_DISK_PART_READ_ONLY_RESTORE               "PmsAlarmUsbBakDiskPartReadOnlyRestore"
#define LAN_PMS_ALARM_USBBAK_DISK_PART_READ_ONLY_RESUME                "PmsAlarmUsbBakDiskPartReadOnlyResume"
#define LAN_PMS_ALARM_UNKNOWN_ALARM                                    "PmsAlarmUnknownAlarm"
#define LAN_PMS_ALARM_UNKNOWN_ALARM_RESTORE                            "PmsAlarmUnknownAlarmRestore"
#define LAN_PMS_ALARM_UNKNOWN_ALARM_RESUME                             "PmsAlarmUnknownAlarmResume"
#define LAN_PMS_USER_OPERATE_LOG_TITLE                                 "PmsUserOperateLogTitle"
#define LAN_PMS_PLATFORM_LOG_TITLE                                     "PmsPlatformLogTitle"
#define LAN_PMS_NMS_HISTORY_ALARM_TITLE                                "PmsNmsHistoryAlarmTitle"
#define LAN_PMS_ALARM_MANAGE_LICENSE_TOO_MUCH                "PmsAlarmManageLicenseTooMuch"
#define LAN_PMS_ALARM_MANAGE_LICENSE_TOO_MUCH_RETORE              "PmsAlarmManageLicenseTooMuchRestore"
#define LAN_PMS_ALARM_MANAGE_LICENSE_TOO_MUCH_RESUME              "PmsAlarmManageLicenseTooMuchResume"
#define LAN_PMS_ALARM_TAS_LICENSE_GETFILE_VALIDATE_NOT_PASS      "PmsAlarmTasLicenseGetfileValidateNotPass"
#define LAN_PMS_ALARM_TAS_LICENSE_GETFILE_VALIDATE_NOT_PASS_RESTORE      "PmsAlarmTasLicenseGetfileValidateNotPassRestore"
#define LAN_PMS_ALARM_TAS_LICENSE_GETFILE_VALIDATE_NOT_PASS_RESUME      "PmsAlarmTasLicenseGetfileValidateNotPassResume"
#define LAN_PMS_ERROR_SUB_DOMAIN_DEL_PROGRESS_QRY               "PmsErrorSubDomainDelProgressQry"    //退网下级平台进度查询失败错误描述,对应错误码22081
#define LAN_PMS_ERROR_MODIFY_CONFIG                                       "PmsErrorModifyConfigFail"                 //修改配置文件失败错误描述
#define LAN_PMS_ALARM_RCS_OTHER_STORAGE_OVER_LICENSE_LIMIT               "PmsAlarmRcsOtherStorageOverLicenseLimit"
#define LAN_PMS_ALARM_RCS_OTHER_STORAGE_OVER_LICENSE_LIMIT_RESTORE              "PmsAlarmRcsOtherStorageOverLicenseLimitRestore"
#define LAN_PMS_ALARM_RCS_OTHER_STORAGE_OVER_LICENSE_LIMIT_RESUME               "PmsAlarmRcsOtherStorageOverLicenseLimitResume"

/* uas */
#define LAN_UAS_USER_OPERATE_LOG_TITLE                                 "UasUserOperateLogTitle"
#define LAN_UAS_ALARM_LOG_TITLE                                        "UasAlarmLogTitle"
#define LAN_UAS_DEVICE_LOG_TITLE                                       "UasDeviceLogTitle"

/*gbs*/
#define LAN_GBS_GB_SUBDOMAIN_DEVGROUP      "GbsGbSubdomainDevGroup"        //国标下级域设备组，请勿手工修改
#define LAN_GBS_GB_CHN_STREAM_TYPE_MAIN    "GbsGbChnStreamTypeMain"   //国标通道流属性：主流
#define LAN_GBS_GB_CHN_STREAM_TYPE_ASSIST  "GbsGbChnStreamTypeAssist" //国标通道流属性：辅流
#define LAN_GBS_GB_PARENT_GBDOMAIN_DISCONNECT       "ParentGbdomainDisconnect"    //与国标上级断链
#define	LAN_GBS_EXPORT_DEV_GBEXT_FILE_CONTENT		"GbsExportDevGbExtFileContent"	//设备号,型号名,厂商,设备型号,摄像机类型扩展,摄像机安装位置,室内室外位置,摄像机用途属性,摄像机补光属性,摄像机监视方位属性,摄像机支持的分辨率,虚拟组织所属的业务分组ID,下载倍速范围,空域编码能力,时域编码能力,经度,纬度


//rcs
#define LAN_ALARM_EVENT_TYPE_RECORD        "AlarmEventTypeRecord"     //告警事件类型-录像
#define LAN_ALARM_EVENT_TYPE_PU            "AlarmEventTypePu"         //告警事件类型-设备
#define LAN_ALARM_EVENT_TYPE_DISK          "AlarmEventTypeDisk"       //告警事件类型-磁阵
#define LAN_ALARM_EVENT_TYPE_BUSINESS      "AlarmEventTypeBusiness"   //告警事件类型-业务

#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY       "AlarmEventRecStoppedAbnormally"      //录像异常终止，测试用 
#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY_PART0 "AlarmEventRecStoppedAbnormallyPart0" //录像异常终止描述部分0 
#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY_PART1 "AlarmEventRecStoppedAbnormallyPart1" //录像异常终止描述部分1 
#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY_PART2 "AlarmEventRecStoppedAbnormallyPart2" //录像异常终止描述部分2 
#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY_PART3 "AlarmEventRecStoppedAbnormallyPart3" //录像异常终止描述部分3

#define LAN_RCS_REC_STOPPED_BY_REASON_LOAD_BALANCE         "RcsRecStopByReasonLoadBalance"       //录像停止，原因：负载均衡
#define LAN_RCS_REC_STOPPED_BY_REASON_BIGFILE_SWITCH_FAIL  "RcsRecStopByReasonBigFileSwitchFail" //录像停止，原因：大文件切换失败
#define LAN_RCS_REC_STOPPED_BY_REASON_PARTITION_FULL       "RcsRecStopByReasonPartitionFull"     //录像停止，原因：磁盘满
#define LAN_RCS_REC_STOPPED_BY_REASON_FORMAT_CHANGE        "RcsRecStopByReasonFormatChange"      //录像停止，原因：格式变化
#define LAN_RCS_REC_STOPPED_BY_REASON_NOSTREAM		       "RcsRecStopByReasonNoStream"          //录像停止，原因：无码流
#define LAN_RCS_REC_STOPPED_BY_REASON_UNKNOWN              "RcsRecStopByReasonUnknown"           //录像停止，原因：未知

#define LAN_RCS_DISK_LICENSE_LIMITTED      "RcsDiskLicenseLimitted"   //License超过限制
#define LAN_RCS_DISK                       "RcsDisk"                  //磁盘

#define LAN_RCS_ERRCODE                    "RcsErrCode"               //错误码
#define LAN_RCS_ERRCODE_19002              "RcsErrCode19002"          //错误码19002
#define LAN_RCS_ERRCODE_19010              "RcsErrCode19010"          //错误码19010
#define LAN_RCS_ERRCODE_23105              "RcsErrCode23105"          //错误码23105
#define LAN_RCS_ERRCODE_23106              "RcsErrCode23106"          //错误码23106
#define LAN_RCS_ERRCODE_23111              "RcsErrCode23111"          //错误码23111
#define LAN_RCS_ERRCODE_23131              "RcsErrCode23131"          //错误码23131
#define LAN_RCS_ERRCODE_23133              "RcsErrCode23133"          //错误码23133
#define LAN_RCS_ERRCODE_23137              "RcsErrCode23137"          //错误码23137
#define LAN_RCS_ERRCODE_23148              "RcsErrCode23148"          //错误码23148
#define LAN_RCS_ERRCODE_23164              "RcsErrCode23164"          //错误码23164
#define LAN_RCS_ERRCODE_23165              "RcsErrCode23165"          //错误码23165
#define LAN_RCS_ERRCODE_23166              "RcsErrCode23166"          //错误码23166
#define LAN_RCS_ERRCODE_23167              "RcsErrCode23167"          //错误码23167
#define LAN_RCS_ERRCODE_UNKNOWN            "RcsErrCodeUnknown"        //其他未知错误码



#endif // _CMS_LANDESC_H_

//end of file
