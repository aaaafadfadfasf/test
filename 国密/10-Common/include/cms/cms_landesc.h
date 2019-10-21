/*****************************************************************************
   ģ����      : cms_landesc 
   �ļ���      : cms_landesc.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ��������CMSϵͳ��������������
   ����        : wangwenxuan
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/11/22   1.0      wangwenxuan      ����
******************************************************************************/

#ifndef _CMS_LANDESC_H_
#define _CMS_LANDESC_H_

#define     LAN_DESC_USER                 "User"                   //"�û�"
#define     LAN_DESC_USERID               "UserId"                 //"�û����"
#define     LAN_DESC_USERNAME             "UserName"               //"�û���"
#define     LAN_DESC_OPRUSERID            "OperatorUserId"         //"������ID" �豸�黮��ʱ��Ҫ
#define     LAN_DESC_PWD                  "Password"               //"����"
#define     LAN_DESC_OPR                  "Operate"                //"����"
#define     LAN_DESC_DEV                  "Device"                 //"�豸"
#define     LAN_DESC_DEV_NAME             "DeviceName"             //"�豸����"
#define     LAN_DESC_VIDSRC               "VideoSource"            //"��ƵԴ"
#define     LAN_DESC_VIDSRCALIAS          "VideoSourceAlias"       //"��ƵԴ����"
#define     LAN_DESC_LOGIN                "Login"                  //"��½"
#define     LAN_DESC_LOGOUT               "Logout"                 //"�ǳ�"
#define     LAN_DESC_MODULE               "Module"                 //"ģ��"
#define     LAN_DESC_ADD                  "Add"                    //"����"
#define     LAN_DESC_DEL                  "Delete"                 //"ɾ��"
#define     LAN_DESC_MOD                  "Modify"                 //"�޸�"
#define     LAN_DESC_QRY                  "Query"                  //"��ѯ"
#define     LAN_DESC_TAG                  "Tag"                    //"��ǩ"
#define     LAN_DESC_NTF                  "Notify"                 //"֪ͨ"
#define     LAN_DESC_DATA                 "Data"                   //"����"
#define     LAN_DESC_SET                  "Set"                    //"����"
#define     LAN_DESC_GET                  "Get"                    //"��ȡ"
#define     LAN_DESC_SAVE                 "Save"                   //"����"
#define     LAN_DESC_LOAD                 "Load"                   //"����"
#define     LAN_DESC_CREATE               "Create"                 //"����"
#define     LAN_DESC_MODEL                "Model"                  //"�ͺ�"
#define     LAN_DESC_DATE                 "Date"                   //"����"
#define     LAN_DESC_ALIAS                "Alias"                  //"����"
#define     LAN_DESC_DOMAINNAME           "DomainName"             //"����"
#define     LAN_DESC_DOMAINID             "DomainId"               //"����"
#define     LAN_DESC_PARENT_DOMAINNAME    "ParentDomainName"       //"��������"
#define     LAN_DESC_PARENT_DOMAINID      "ParentDomainId"         //"������"
#define     LAN_DESC_START                "Start"                  //"����"
#define     LAN_DESC_STOP                 "Stop"                   //"ֹͣ"
#define     LAN_DESC_TYPE                 "Type"                   //"����"
#define     LAN_DESC_CAP                  "Capability"             //"����"
#define     LAN_DESC_MANUFACTURER         "Manufacturer"           //"����"
#define     LAN_DESC_USERGROUP            "UserGroup"              //"�û���"
#define     LAN_DESC_USERGROUP_ID         "UserGroupID"            //"�û�����"
#define     LAN_DESC_USERGROUP_NAME       "UserGroupName"          //"�û�����"
#define     LAN_DESC_DESC                 "Description"            //"����"
#define     LAN_DESC_TVWALL               "TvWall"                 //"����ǽ"
#define     LAN_DESC_TVWALLID             "TvWallId"               //"����ǽ���"
#define     LAN_DESC_TVWALL_NAME          "TvWallName"             //"����ǽ����"
#define     LAN_DESC_TVID                 "TvId"                   //"���ӱ��"
#define     LAN_DESC_TVDIV_ID             "TvDivID"                //"���ӷֻ���ID"
#define     LAN_DESC_DECODERID            "DecoderId"              //"���������"
#define     LAN_DESC_TVNUM                "TvNum"                  //"���ӻ���Ŀ"   
#define     LAN_DESC_TVWALL_STYLE         "TvWallStyle"            //"����ǽ���"
#define     LAN_DESC_TVDECODER_BINDER     "BinderofTv-Decoder"     //"������������󶨹�ϵ"
#define     LAN_DESC_FILTER_CONDITIONS    "FilterConditions"       //"��������"
#define     LAN_DESC_DEVPRIVILEGE         "DevicePrivilege"        //"�豸Ȩ��"
#define     LAN_DESC_DEVGRPPRIVILEGE         "DeviceGroupPrivilege"        //"�豸��Ȩ��"
#define     LAN_DESC_DEVCAP               "DeviceCapability"       //"�豸����"
#define     LAN_DESC_PRIVILEGE            "Privilege"              //"Ȩ��"
#define     LAN_DESC_CREATE_DATE          "CreateDate"             //"��������"
#define     LAN_DESC_USERTYPE             "UserType"               //"�û�����"
#define     LAN_DESC_START_DATE           "StartDate"              //"��ʼ����"
#define     LAN_DESC_STOP_DATE            "StopDate"               //"��������"   
#define     LAN_DESC_USER_PRIVILEGE       "UserPrivilege"          //"�û�Ȩ��"
#define     LAN_DESC_GROUP_CLASSIFIED     "GroupClassified"        //"���黮��"
#define     LAN_DESC_AUTH                 "Auth"                   //"��Ȩ" 
#define     LAN_DESC_CROSS_DOMAIN_AUTH    "CrossDomainAuth"        //"������Ȩ"
#define     LAN_DESC_GROUP_DEVICE         "GroupDevice"            //"�豸����"
#define     LAN_DESC_DEVICE_GROUP         "DeviceGroup"            //"�豸��"
#define     LAN_DESC_DEVICE_GROUP_NAME    "DeviceGroupName"        //"�豸����"
#define     LAN_DESC_DEVICE_ID            "DeviceID"               //"�豸ID"
#define     LAN_DESC_DEVICE_GROUP_ID      "DeviceGroupID"          //"�豸��ID"
#define     LAN_DESC_DEV_GROUP_AUTO_ASSIGN "DeviceGroupAutoAssign" //"�豸���Զ�ͬ�����"
#define     LAN_DESC_SUCCESS              "Success"                //"�ɹ�"
#define     LAN_DESC_FAIL                 "Fail"                   //"ʧ��"
#define     LAN_DESC_TIMEOUT              "TimeOut"                //"��ʱ"
#define     LAN_DESC_LOGINTYPE            "LoginType"              //"��½����"
#define     LAN_DESC_EXTRA_INFO           "ExtraInfo"              //"��չ��Ϣ"
#define     LAN_DESC_DEVADDRLIST          "DeviceAddrList"         //"�豸��ַ�б�"
#define     LAN_DESC_DEVURI               "DeviceURI"              //"�豸URI"
#define     LAN_DESC_VERSION              "Version"                //"�汾"
#define     LAN_DESC_CUSTOM               "Custom"                 //"�Զ���"
#define     LAN_DESC_SUBSCRIBER           "Subscriber"             //"������"
#define     LAN_DESC_BY_SUBSCRIBER        "BySubscriber"          //"��������"
#define     LAN_DESC_SUB                  "Subscribe"              //"����"
#define     LAN_DESC_SUB_STATUS           "SubscribeStatus"        //"����״̬"
#define     LAN_DESC_ONLINE_STATUS        "OnlineStatus"           //"������״̬"
#define     LAN_DESC_ONLINE               "Online"                 //"����"
#define     LAN_DESC_OFFLINE              "Offline"                //"����"
#define     LAN_DESC_PU_CONFIG            "PuConfig"              //"ǰ������"
#define     LAN_DESC_PU_SERVICE_STATUS    "PuServiceStatus"      //"ǰ���豸״̬"
#define     LAN_DESC_GPS_STATUS            "GPSStatus"              // "GPS״̬"
#define     LAN_DESC_DEV_CHN              "DevChannel"            //"�豸ͨ��"
#define     LAN_DESC_CAM_CTRL             "CameraCtrl"               //"����ͷ����"
#define     LAN_DESC_ERROR_DESC           "ErrorDesc"             //"��������"
#define     LAN_DESC_SERIAL_DATA          "SerialData"            //"��������"
#define     LAN_DESC_LOCK                 "Lock"                   //"����"
#define     LAN_DESC_UNLOCK               "Unlock"                 //"����"
#define     LAN_DESC_VIDEOENCODER_PARAM   "VideoEncoderParam"     //"��Ƶ�������"
#define     LAN_DESC_MEDIA_TYPE           "MediaType"             //"ý���ʽ"
#define     LAN_DESC_MEDIA_DESC           "MediaDesc"             //"ý������"
#define     LAN_DESC_PUSYS_PARAM          "PuSystemParam"        //"ǰ��ϵͳ����"
#define     LAN_DESC_SYNC_SWITCH          "SyncSwitch"           //"ͬ������"
#define	    LAN_DESC_SYNC_VS_NAME_PU2PLAT "SyncVideoSrcNamePu2Plat" //"��ƵԴ������ǰ��ͬ����ƽ̨"
#define	    LAN_DESC_SYNC_VS_NAME_PLAT2PU "SyncVideoSrcNamePlat2Pu" //"��ƵԴ������ƽ̨ͬ����ǰ��"
#define     LAN_DESC_DOWNLOAD_PLATFORMRECORD "DownLoadPlatformRecord"        //"ƽ̨¼������"
#define     LAN_DESC_DOWNLOAD_PURECORD    "DownLoadPuRecord"     //"ǰ��¼������"
#define     LAN_DESC_RECORDSYS_PARAM      "RecordSystemParam"    //"¼��ϵͳ����"
#define     LAN_DESC_RECORD_PARAM         "RecordParam"          //"¼�����"
#define     LAN_DESC_RECORD_MODE          "RecordMode"           //"¼��ģʽ"
#define     LAN_DESC_RECODE_SUPPORT_CHANGE_STREAM_type  "SupportChangeStreamType"   //֧�������л�
#define     LAN_DESC_RECORD_STREAM_TYPE     "RecodeStreamType"      //¼����������
#define     LAN_DESC_PRERECORD_DURATION   "PreRecordDuration"      //"Ԥ¼ʱ��"
#define     LAN_DESC_REBOOT               "Reboot"                 //"����"
#define     LAN_DESC_THIRD_PARTY_CALL     "ThirdPartyCall"         //"��������"
#define     LAN_DESC_SWITCH               "Switch"                 //"����"
#define     LAN_DESC_SWITCH_NUM           "SwitchNum"             //"������"
#define     LAN_DESC_RCV_GROUP            "ReceiveGroup"           //"������"
#define     LAN_DESC_GAP                  "Gap"                    //"��բ"
#define     LAN_DESC_ALARM_INFO           "AlarmInfo"             //"�澯��Ϣ"
#define     LAN_DESC_ALARM_INPUT          "AlarmInput"            //"�澯����"
#define     LAN_DESC_ALARM_INPUT_ID       "AlarmInputID"         //"�澯����ͨ��"
#define     LAN_DESC_ALARM_TYPE           "AlarmType"             //"�澯����"
#define     LAN_DESC_ALARM_FLAG           "AlarmFlag"             //"�澯��ʶ"
#define     LAN_DESC_ALARM_TIME           "AlarmTime"             //"�澯ʱ��"
#define     LAN_DESC_ALARM_DATA           "AlarmData"             //"�澯����"
#define     LAN_DESC_ALARM_AREA_ID        "AlarmAreaID"            //"�澯�����"
#define     LAN_DESC_ALARM_LINK           "AlarmLink"             //"�澯����"
#define     LAN_DESC_ALARM_LINK_CFG       "AlarmLinkCfg"          //"�澯��������"
#define     LAN_DESC_ALRAM_ACTION         "AlarmAction"           //"������Ϊ"
#define     LAN_DESC_ALARM_SRC            "AlarmSource"           //"�澯Դ"
#define     LAN_DESC_ALARM_READ_TIME      "AlarmReadTime"          //"�澯����ʱ��"
#define     LAN_DESC_IO_OUT               "IOOut"                 //"���������"
#define     LAN_DESC_TV_OPR_TYPE          "TVOperateType"          //"���Ӳ�������"
#define     LAN_DESC_TV_CHN               "TVChannel"             //"����ͨ��"
#define     LAN_DESC_TVWIN_POLL           "TVWindowPoll"         //"���Ӵ�����Ѳ"
#define     LAN_DESC_POLL_INDEX           "PollIndex"             //"��Ѳ���"
#define     LAN_DESC_POLL_TIME            "PollTime"              //"��Ѳʱ��"
#define     LAN_DESC_TVWIN_STYLE          "TVWindowStyle"         //"������"
#define     LAN_DESC_TVWALL_SCHEME        "TVWallScheme"          //"����ǽԤ��"
#define     LAN_DESC_TVWALL_SCHEME_GROUP  "TVWallSchemeGroup"     //"����ǽԤ����"
#define     LAN_DESC_SCHEME_POLL          "SchemePoll"            //"Ԥ����Ѳ"
#define     LAN_DESC_SCHEME_NAME          "SchemeName"            //"Ԥ������"
#define     LAN_DESC_POLL_STATE           "PollState"             //"��Ѳ״̬"
#define     LAN_DESC_MEDIA_SERVICE        "MediaService"          //"ý���������"
#define     LAN_DESC_REC_TOKEN            "RecordToken"           //"¼��Token"
#define     LAN_DESC_TIME_RANGE           "TimeRange"             //"ʱ���"
#define     LAN_DESC_ENC_CHN              "EncoderChannel"        //"������ͨ��"
#define     LAN_DESC_MOTION_DETECT        "MotionDetect"          //"�ƶ����"
#define     LAN_DESC_LEFT_TOPX            "LeftTopX"             //"���������"
#define     LAN_DESC_LEFT_TOPY            "LeftTopY"             //"����������"
#define     LAN_DESC_WIDTH                "Width"                  //"���"
#define     LAN_DESC_HEIGHT               "Height"                 //"�߶�"
#define     LAN_DESC_ALARM_OCCUR_RATE     "AlarmOccurRate"       //"�澯����������" 
#define     LAN_DESC_ALRAM_RESTORE_RATE   "AlarmRestoreRate"     //"�澯�ָ�������"
#define     LAN_DESC_AREA_SHADE           "AreaShade"             //"�����ڱ�"
#define     LAN_DESC_GPS_CFG              "GPSConfig"             //"GPS����"
#define     LAN_DESC_GPS_SS               "GPSSubscribe"          //"GPS����"
#define     LAN_DESC_GPS_DATA             "GPSData"               //"GPS����"
#define     LAN_DESC_TRANSPARENT_DATA     "TransparentDate"       //"͸������"
#define     LAN_DESC_REC_TYPE             "RecordType"             //"¼������"
#define     LAN_DESC_REC_INFO             "RecordInfo"             //"¼����Ϣ"
#define     LAN_DESC_RECORD_TASK          "RecordTask"             //"¼������"
#define     LAN_DESC_FILE_PATH            "FilePath"              //"�ļ�·��"
#define     LAN_DESC_FILE_NAME            "FileName"              //"�ļ�����"
#define     LAN_DESC_DEC_STYLE            "DecoderStyle"          //"���������"
#define     LAN_DESC_VCR_CTRL             "VCRCtrl"               //"VCR����"
#define     LAN_DESC_VCR_CMD              "VCRCmd"                //"VCR����"
#define     LAN_DESC_PLAY_RATE            "PlayRate"              //"��������"
#define     LAN_DESC_SEEK_TIME            "SeekTime"              //"���ʱ��"
#define     LAN_DESC_CUR_PLAYTIME         "CurrentPlayTime"       //"��ǰ����ʱ��"
#define     LAN_DESC_TOTAL_PLAYTIME       "TotalPlayTime"         //"������ʱ��"
#define     LAN_DESC_START_PLAYTIME       "StartPlayTime"         //"��ʼ����ʱ��"
#define     LAN_DESC_STOP_PLAYTIME        "StopPlayTime"          //"��������ʱ��"
#define     LAN_DESC_PTZ_CTRL             "PTZCtrl"               //"�ƾ�����"
#define     LAN_DESC_PTZ_LEVEL            "PTZLevel"              //"�ƾ����Ƽ���"
#define     LAN_DESC_PTZ_LOCK             "PTZLock"               //"�ƾ����Ƽ���"
#define     LAN_DESC_PTZ_UNLOCK           "PTZUnLock"             //"�ƾ����ƽ���"
#define     LAN_DESC_PTZ_AUTOUNLOCK       "PTZAutoUnlock"            // "�ƾ������Զ�����"
#define     LAN_DESC_PTZ_UNLOCKAll        "PTZUnlockAll"             // "�ƾ�����ȫ������"
#define     LAN_DESC_HOLD_TIME            "HoldTime"              //"�ѳ�ʱ��"
#define     LAN_DESC_DIRECT_TRANS_NUM     "DirectTransNum"       //"ֱ��·��"
#define     LAN_DESC_VID_FORMAT           "VideoFormat"           //"��Ƶ��ʽ"
#define     LAN_DESC_AUD_FORMAT           "AudioFormat"           //"��Ƶ��ʽ"
#define     LAN_DESC_VID_RES              "VideoResolution"      //"��Ƶ�ֱ���"
#define     LAN_DESC_FRAME_RATE           "FrameRate"             //"֡��"
#define     LAN_DESC_QUALITY              "Quality"                //"����"
#define     LAN_DESC_KEYFRAME_INTERVAL    "KeyFrameInterval"               //"�ؼ�֡���"
#define     LAN_DESC_BRIGHTNESS           "Brightness"             //"����"
#define     LAN_DESC_CONTRAST             "Contrast"               //"�Աȶ�"
#define     LAN_DESC_SATURATION           "Saturation"             //"���Ͷ�"
#define     LAN_DESC_SHARPNESS            "Sharpness"              //"���"
#define     LAN_DESC_INPUT_PIN            "InputPin"              //"��������"
#define     LAN_DESC_OUTPUT_PIN           "OutputPin"             //"�������"
#define     LAN_DESC_VIDEO_IN             "VideoInput"            //"��Ƶ����"
#define     LAN_DESC_DEV_MODEL            "DeviceModel"            //"�豸�ͺ�"
#define     LAN_DESC_CAM_TYPE             "CameraType"            //"����ͷ����"
#define     LAN_DESC_CAM_ID               "CameraID"              //"����ͷ��ַ��"
#define     LAN_DESC_BIT_RATE             "BitRate"               //"������"
#define     LAN_DESC_DIRECT_TRANS         "DirectTrans"           //"ֱ��" 
#define     LAN_DESC_PLAT_PRE_REC         "PlatformPreRecord"      //"ƽ̨Ԥ¼"
#define     LAN_DESC_PLAT_REC             "PlatformRecord"         //"ƽ̨¼��"
#define     LAN_DESC_PLAT_REC_ON_TV       "PlatformRecordOnTv"     //"ƽ̨¼����ǽ"
#define     LAN_DESC_PU_PRE_REC           "PuPreRecord"            //"ǰ��Ԥ¼"
#define     LAN_DESC_PU_REC               "PuRecord"               //"ǰ��¼��"
#define     LAN_DESC_CAM_PRE_POS          "CameraPresetPos"        //"����ͷԤ��λ"
#define     LAN_DESC_MSG_NO               "MessageNo"              //"���ź���"
#define     LAN_DESC_VOC_NO               "VoiceNo"               //"��Ѷ����"
#define     LAN_DESC_EMAIL                "EMail"                  //"��������"
#define     LAN_DESC_TRANS                "Transport"              //"����"
#define     LAN_DESC_TRANS_TYPE           "TransportType"         //"����ģʽ"  
#define     LAN_DESC_MEDIA_TRANSPORT      "MediaTransport"        //"ý�崫��"
#define     LAN_DESC_NET_ADDR             "NetAddress"            //"�����ַ"       
#define     LAN_DESC_PAUSE                "Pause"                  //"��ͣ"
#define     LAN_DESC_RESUME               "Resume"                 //"�ָ�"
#define     LAN_DESC_SEEK                 "Seek"                   //"��λ"
#define     LAN_DESC_FAST_PLAY            "FastPlay"              //"���"
#define     LAN_DESC_SLOW_PLAY            "SlowPlay"              //"����"
#define     LAN_DESC_ONESTEP_FRAME            "OneStepFrame"              //"��֡����"
#define     LAN_DESC_KEY_FRAME            "KeyFrame"              //"�ؼ�֡����"
#define     LAN_DESC_INVALID              "Invalid"                //"��Ч"
#define     LAN_DESC_DISK_FULL            "DiskFull"              //"������"
#define     LAN_DESC_PUCHN_BINDING        "PuChnBinding"          //"ͨ����"
#define     LAN_DESC_PUCHN_UNBINDING      "PuChnUnBinding"        //"���ͨ����"
#define     LAN_DESC_NRU_ID               "NruId"                 //"NruID"
#define     LAN_DESC_DISK_MOUNT_PATH      "DiskMountPath"         //"���̹���·��"
#define     LAN_DESC_RECORD_DAYS          "RecordDays"            //"¼������"
#define     LAN_DESC_SCHEME               "Scheme"                //"Ԥ��"
#define     LAN_DESC_E_MAP                "EMap"                  //"���ӵ�ͼ"
#define     LAN_DESC_MAP_ID               "MapID"                 //"��ͼ���"
#define     LAN_DESC_MAP_NAME             "MapName"               //"��ͼ����"
#define     LAN_DESC_MAP_TYPE             "MapType"               //"��ͼ����"
#define     LAN_DESC_MAP_ELEMENT          "MapElement"            //"ͼԪ"
#define     LAN_DESC_MAP_ELEMENT_ID       "MapElementID"          //"ͼԪ���"
#define     LAN_DESC_MAP_ELEMENT_NAME     "MapElementName"        //"ͼԪ����"
#define     LAN_DESC_MAP_ELEMENT_TYPE     "MapElementType"        //"ͼԪ����"
#define     LAN_DESC_COORDINATES          "Coordinates "          //"����"
#define     LAN_DESC_TRANSFORMATION       "Transformation"        //"ת��"
#define     LAN_DESC_MARS_COORDINATES     "MarsCoordinates"       //"��������"
#define     LAN_DESC_EARTH_COORDINATES    "EarthCoordinates"      //"��������"
#define     LAN_DESC_LNG                  "Longitude"             //"����"
#define     LAN_DESC_LAT                  "Latitude "             //"γ��"
#define     LAN_DESC_UAS_CFG              "UasConfig"             //"Uas����" ��ʱ�ĳ���־��������
#define     LAN_DESC_REAL_MEDIA_ON_TV     "RealMediaOnTv"         //"ʵʱ������ǽ"
#define     LAN_DESC_REAL_MEDIA_OFF_TV    "RealMediaOffTv"        //"ʵʱ������ǽ"
#define     LAN_DESC_RECORD_ON_TV         "RecordOnTv"            //"¼����ǽ"
#define     LAN_DESC_RECORD_OFF_TV        "RecordOffTv"           //"¼����ǽ"
#define     LAN_DESC_REAL_MEDIA_INVITE    "RealMediaInvite"       //"ʵʱ���"
#define     LAN_DESC_PU_PLAYBACK          "PuPlayBack"            //"ǰ��¼��ط�"
#define     LAN_DESC_PLATFORM_PLAYBACK    "PlatformPlayBack"      //"ƽ̨¼��ط�"
#define     LAN_DESC_SWITCH_STOP_BY_BYE   "SwitchStopByBye"       //"�������Ҷ�"
#define     LAN_DESC_SIP_EXCEPTION        "SipException"          //"Sip��Ϣ�쳣"
#define     LAN_DESC_LOAD_BALANCE         "LoadBalance"           //"���ؾ���"
#define     LAN_DESC_PARTITION_FULL       "PartitionFull"         //"���̿ռ���"
#define     LAN_DESC_BIGFILE_SWITCH_FAIL  "BigFileSwitchFail"     //"���ļ��л�ʧ��"
#define     LAN_DESC_RESOLUTION_OR_FORMAT_CHANGE        "ResolutionOrFormatChange"          //"�ֱ��ʻ��ʽ���"
#define     LAN_DESC_REC_NO_STREAM		  "RecNoStream"           //"������"
#define     LAN_DESC_PU_OFFLINE_TOO_BRIEF "PuOfflineTooBrief"     //"�豸Ƶ������"
#define     LAN_DESC_REASON_UNKNOWN       "ReasonUnknow"          //"ԭ��δ֪"
#define     LAN_DESC_ERROR_CODE           "ErrorCode"             //"������"
#define     LAN_DESC_DEV_TREE             "DevTree"               //"�豸��"
#define     LAN_DESC_DEV_TREE_ID          "DevTreeId"             //"�豸�����"
#define     LAN_DESC_DEV_TREE_NAME        "DevTreeName"           //"�豸������"
#define     LAN_DESC_WATER_MARK_MODIFICATION  "WaterMarkModification"    //"ˮӡ���"
#define     LAN_DESC_FONT_SIZE            "FontSize"            //"�����С"
#define     LAN_DESC_FONT_NAME            "FontName"            //"��������"
#define     LAN_DESC_ROTATION             "Rotation"            //"ת��"
#define     LAN_DESC_TRANSPARENCY         "Transparency"        //"͸����"
#define     LAN_DESC_TILE                 "Tile"               //"ƽ����"
#define     LAN_DESC_COLOR                "Color"               //"��ɫ"

///////////////////////////////////////////////////////////////////////////////////////////////////
#define     LAN_CU_PASSWORD_LOCKED            "CuPassowordLocked"
#define     LAN_CU_CHANGE_PASSWORD_SUCCESS    "CuSuccessInPwdMod" //�޸�����
#define     LAN_CU_CHANGE_PASSWORD_FAIL       "CuFailToChangePwd" //�޸�����
#define     LAN_CU_CHANGE_PRIVILEGE_SUCCESS       "CuSuccessToChangePri" //Ȩ���޸�
#define     LAN_CU_CHANGE_PRIVILEGE_FAIL       "CuFailToChangePri" //Ȩ���޸�


#define     LAN_PMC_USER_LOGIN                "PmcUserLogin"          //��¼%s
#define     LAN_PMC_USER_LOGOUT               "PmcUserLogout"          //�ǳ�%s
#define     LAN_PMC_MOD_PASSWORD              "PmcModPassword"         //�޸�����%s
#define     LAN_PMC_PASSWORD_LOCKED           "PmcPasswordLocked"      //��������������������Ƶ�¼
#define     LAN_PMC_ADD_USER                  "PmcAddUser"             //����û�(�û���:%s)%s
#define     LAN_PMC_MOD_USER                  "PmcModUser"             //�޸��û�[�û���:%s-�û�����:%s-�û�����:%s]Ϊ[�û���:%s-�û�����:%s-�û�����:%s]%s
#define     LAN_PMC_DEL_USER                  "PmcDelUser"             //ɾ���û�[�û�ID:%s]%s
#define     LAN_PMC_MOD_SECURE_EMAIL          "PmcModSecureEmail"      //�޸İ�ȫ����:%sΪ%s %s
#define     LAN_PMC_USER_TYPE_SUPERADMIN      "PmcUserTypeSuperAdmin"  //��������Ա
#define     LAN_PMC_USER_TYPE_ADMIN           "PmcUserTypeAdmin"       //����Ա
#define     LAN_PMC_USER_TYPE_INVALID         "PmcUserTypeInvalid"     //��Ч�û�����
#define     LAN_PMC_DESC_UPDATE              "PmcUpdate"               //�����豸 %s
#define     LAN_PMC_DESC_EXPORT              "PmcExport"               //�������� %s
#define     LAN_PMC_DESC_IMPORT              "PmcImport"               //���뾵�� %s
#define     LAN_PMC_DESC_RESTORE             "PmcRestore"              //�ָ��������� %s
#define     LAN_PMC_SOFTWAREINFO_MOD         "PmcSoftwareInfoModify"      //�޸������Ϣ %s
#define     LAN_PMC_CTRL_PMS                 "PmcCtrlPms"              //����(������ػ�)ƽ̨ϵͳ %s
#define     LAN_PMC_TIMEINFO_MOD             "PmcTimeInfoModify"          //�޸�ƽ̨ʱ�� %s
#define     LAN_PMC_NETCARDINFO_MOD          "PmcNetCardInfoModdify"       //�޸�������Ϣ %s
#define     LAN_PMC_NETROUTEINFO_ADD         "PmcNetRouteInfoAdd"       //�������·�� %s
#define     LAN_PMC_NETROUTEINFO_DEL         "PmcNetRouteInfoDel"       //ɾ������·�� %s
#define     LAN_PMC_NETDEFAULTROUTE_SET      "PmcNetDefaultRouteSet"    //����Ĭ������ %s
#define     LAN_PMC_STUNSERVER_MOD           "PmcStunServerModdify"         //�޸�stun�������� %s
#define     LAN_PMC_NETNAT_ADD               "PmcNetnatAdd"             //�������NAT %s
#define     LAN_PMC_NETNAT_DEL               "PmcNetnatDel"             //ɾ������NAT %s
#define     LAN_PMC_ADD_UP_PLATFORM          "PmcAddUpPlatform"         //�����ϼ�ƽ̨%s
#define     LAN_PMC_ADD_DOWN_PLATFORM        "PmcAddDownPlatform"       //�����¼�ƽ̨%s
#define     LAN_PMC_ADD_SLAVE_PLATFORM       "PmcAddSlavePlatform"      //������ƽ̨%s
#define     LAN_PMC_ADD_MASTER_PLATFORM      "PmcAddMasterPlatform"     //������ƽ̨%s
#define     LAN_PMC_DEL_UP_PLATFORM          "PmcDelUpPlatform"         //�����ϼ�ƽ̨%s
#define     LAN_PMC_DEL_DOWN_PLATFORM        "PmcDelDownPlatform"       //�����¼�ƽ̨%s
#define     LAN_PMC_DEL_SLAVE_PLATFORM       "PmcDelSlavePlatform"      //������ƽ̨%s
#define     LAN_PMC_DEL_MASTER_PLATFORM      "PmcDelMasterPlatform"     //������ƽ̨%s
#define     LAN_PMC_MOD_PLATFORM             "PmcModPlatform"           //�޸�ƽ̨��Ϣ%s
#define     LAN_PMC_MOD_MODULE_CONFIG        "PmcModModuleConfig"       //�޸�ģ����Ϣ%s
#define     LAN_PLATFORM_MOD_IPCONFIG        "PlatformModIpconfig"      //�޸�IP��Ϣ(devName:%s ip:%s netmask:%s gw:%s)%s
#define     LAN_PLATFORM_POWERON             "PlatformPoweron"          //ƽ̨����%s
#define     LAN_PLATFORM_POWEROFF            "PlatformPoweroff"         //ƽ̨�ػ�%s
#define     LAN_PMC_ADD_DEVICE_MANUFACTURER  "PmcAddDeviceManufacturer" //����豸����(������:%s)%s
#define     LAN_PMC_DEL_DEVICE_MANUFACTURER  "PmcDelDeviceManufacturer" //ɾ���豸����(������:%s)%s
#define     LAN_PMC_ADD_DEVICE_MODEL         "PmcAddDeviceModel"        //����豸�ͺ�(�ͺ���:%s)%s
#define     LAN_PMC_DEL_DEVICE_MODEL         "PmcDelDeviceModel"        //ɾ���豸�ͺ�(�ͺ�id:%s)%s
#define     LAN_PMC_ADD_DEVICE               "PmcAddDevice"             //����豸(�豸��:%s)%s
#define     LAN_PMC_DEL_DEVICE               "PmcDelDevice"             //ɾ���豸(�豸id:%s)%s
#define     LAN_PMC_MOD_DEVICE               "PmcModDevice"             //�޸��豸(�豸id:%s)%s
#define     LAN_PMC_LICENSE_UPDATE           "PmcUpdateLicense"         //�������֤%s
#define     LAN_PMC_ADD_NRU                  "PmcAddNru"                //���NRU(�豸��:%s)%s
#define     LAN_PMC_MOD_NRU                  "PmcModNru"                //�޸�NRU(�豸id:%s)%s
#define     LAN_PMC_DEL_NRU                  "PmcDelNru"                //ɾ��NRU(�豸id:%s)%s
#define     LAN_PMC_MOD_NRU_NETWORK          "PmcModNruNetwork"         //�޸�NRU��������(dev:%s)%s
#define     LAN_PMC_FDISK                    "PmcFdisk"                 //���̷���(�豸��:%s)%s
#define     LAN_PMC_FORMAT_DISK              "PmcFormatDisk"            //���̸�ʽ��(�豸��:%s)%s
#define     LAN_PMC_MOUNT_DISK               "PmcMountDisk"             //���ش���(������:%s)%s
#define     LAN_PMC_CHECK_DISK               "PmcCheckDisk"             //�޸�����(������:%s)%s
#define     LAN_PMC_UNMOUNT_DISK             "PmcUnmountDisk"           //��Ҵ���(������:%s)%s
#define     LAN_PMC_CANCEL_DISK_OP           "PmcCancelDiskOP"          //ȡ�����̲���(������:%s)%s
#define     LAN_PMC_ADD_ISCSI_CONF           "PmcAddIscsiConf"          //���ISCSI����(nruid:%s)%s
#define     LAN_PMC_DEL_ISCSI_CONF           "PmcDelIscsiConf"          //ɾ��ISCSI����(nruid:%s)%s
#define     LAN_PMC_CTRL_ISCSI               "PmcCtrlIscsi"             //ISCSI����(%s)%s
#define     LAN_PMC_DISCONNECT_ISCSI         "PmcDisconnectIscsi"       //�Ͽ�ISCSI����(nruid:%s)%s
#define     LAN_PMC_DEL_OFFLINE_DISK         "PmcDelOfflineDisk"        //ɾ�����ߴ���(������:%s)%s
#define     LAN_PMC_SET_MAIL_CFG             "PmcSetMailCfg"            //��������������%s
#define     LAN_PMC_SET_CFG                  "PmcSetCfg"                //��������������%s
#define     LAN_PMC_ADVANCED_SET_CFG         "PmcAdvancedSetCfg"        //����߼�����������%s
#define     LAN_PMC_ADD_KDDOMAIN             "PmcAddKDDomain"           //���������ӳ��%s
#define     LAN_PMC_DEL_KDDOMAIN             "PmcDelKDDomain"           //����ɾ����ӳ��%s
#define     LAN_PMC_ADD_PU                   "PmcAddPu"                 //�������PU(devname:%s)%s
#define     LAN_PMC_DEL_PU                   "PmcDelPu"                 //����ɾ��PU(devid:%s)%s
#define     LAN_PMC_MOD_PU                   "PmcModPu"                 //�����޸�PU(devid:%s)%s
#define     LAN_PMC_ADD_CHILD                "PmcAddChild"              //��������¼�(gbdevid:%s)%s
#define     LAN_PMC_DEL_CHILD                "PmcDelChild"              //����ɾ���¼�(gbdevid:%s)%s
#define     LAN_PMC_MOD_CHILD                "PmcModChild"              //�����޸��¼�(gbdevid:%s)%s
#define     LAN_PMC_ADD_PARENT               "PmcAddParent"             //��������ϼ�(gbdevid:%s)%s
#define     LAN_PMC_DEL_PARENT               "PmcDelParent"             //����ɾ���ϼ�(gbdevid:%s)%s
#define     LAN_PMC_MOD_PARENT               "PmcModParent"             //�����޸��ϼ�(gbdevid:%s)%s
#define     LAN_PMC_RESET_CU_PASSWORD      "PmcResetCUPassword"          //�ָ�cu�����û�����%s
#define     LAN_PMC_SET_SSH_IPTABLES         "PmcSetSshIptables"         //����ssh������%s
#define     LAN_PMC_SET_MYSQL_IPTABLES       "PmcSetMysqlIptables"       //����mysql������%s
#define     LAN_PMC_SET_HEARTBEART_HOST      "PmcSetHeartbeatHost"       //���ñ���Ϊ����%s
#define     LAN_PMC_SET_HEARTBEART_BAK       "PmcSetHeartbeatBak"        //���ñ���Ϊ����%s
#define     LAN_PMC_GET_LICENSE_NOT_EXIST    "PmcGetLicenseNotExist"     //license������

//@yu 2014.7.22
#define     LAN_PMC_ADD_PERIPHERAL_DEV              "PmcAddPeripheralDev"             //������������豸(gbdevid:%s)%s
#define     LAN_PMC_DEL_PERIPHERAL_DEV              "PmcDelPeripheralDev"             //����ɾ�������豸(gbdevid:%s)%s
#define     LAN_PMC_MOD_PERIPHERAL_DEV              "PmcModPeripheralDev"             //�����޸������豸(gbdevid:%s)%s

//@yu 2014.9.23 ���뵼���豸Ӣ�İ�
#define		LAN_PMC_IMPORT_DEV_TAS_DATABASE_ERROR			"PmcImportDevTasDatabaseError"				//3A�������ݿ�ʧ��
#define		LAN_PMC_IMPORT_DEV_INVALID_ARGUMENT				"PmcImportDevInvalidArgument"				//�����Ƿ�
#define		LAN_PMC_IMPORT_DEV_GBS_DEVINFO_INCOMPLETE		"PmcImportDevGBSDevInfoIncomplete"			//�ṩ�Ĺ����Ų�����Ҫ���豸��ͨ������Ҫָ��
#define		LAN_PMC_IMPORT_DEV_INVALID_GB_ID				"PmcImportDevInvalidGBID"					//�����Ų����ã�Ҳ���ǹ����ŷǷ����߷Ǳ�����Ż������Ͳ�֧�ֻ��߱���Ѿ���ռ��
#define		LAN_PMC_IMPORT_DEV_INVALID_GB_DOMAIN			"PmcImportDevInvalidGBDomain"				//������Ų����ã�Ҳ�������ͬ����Ż���ڻ�
#define		LAN_PMC_IMPORT_DEV_SAME_DEV_MODLE				"PmcImportDevSameDevModel"					//��ͬ���豸�ͺ�
#define		LAN_PMC_IMPORT_ERROR_CODE						"PmcImportDevErrorCode"						//��������[%d]
#define		LAN_PMC_IMPORT_FILE_CONTENT						"PmcImportDevFileContent"					//�豸��,�豸�ͺ�,IP,�������,ʧ��ԭ��
#define     LAN_PMC_IMPORT_DEV_FALIURE						"PmcImportDevFailure"						//ʧ��
#define     LAN_PMC_IMPORT_DEV_SUCCESS						"PmcImportDevSuccess"						//�ɹ�
#define		LAN_PMC_IMPORT_DEV_GBS_NOT_SERVICE				"PmcImportDevGbsNotService"					//gbsδ�ڷ���̬
#define		LAN_PMC_IMPORT_DEV_TAS_NOT_SERVICE				"PmcImportDevTasNotService"					//tasδ�ڷ���̬
#define		LAN_PMC_IMPORT_DEV_FAILED_RESOLUTION			"PmcImportDevFailedResolution"				//�豸��Ϣ����ʧ��
#define		LAN_PMC_IMPORT_DEV_INVALID_DEV_NAME				"PmcImportDevInvalidDevName"				//�豸���к��зǷ��ַ�
#define		LAN_PMC_IMPORT_DEV_ILLEGAL_DEV_IP				"PmcImportDevIllegalDevIP"					//�Ƿ���IP��ַ
#define		LAN_PMC_IMPORT_DEV_NOT_FIND_DEV_MODEL			"PmcImportDevModelNotFind"					//δ�ҵ�ƥ����豸�ͺ�
#define		LAN_PMC_IMPORT_DEV_INVALID_DOMAIN_NAME			"PmcImportDevInvalidDomainName"				//����δƥ�䱾��
#define		LAN_PMC_IMPORT_DEV_INVALID_KEDA_NUM				"PmcImportDevInvalidKEDANum"				//�ƴ��ŷǷ�
//@yu 2014.9.23 ���뵼���豸Ӣ�İ�
#define		LAN_PMC_EXPORT_DEV_FILE_CONTENT					"PmcExportDevFileContent"	                		//�豸��(����),�ͺ���(����),IP(����),�û���(��ѡ),�û�����(��ѡ),�豸������(��ѡ),ͨ��������(Ҫ�������ͬʱ�����ͬʱ����),����(��ѡ),�豸���(��ѡ),�ƴ���(��ѡ),��ͣ��־(��ѡ 1Ϊ���� 0Ϊֹͣ),������·(��ѡ),�Ƿ�����豸(������ʹ�� 1Ϊ�����豸 0Ϊ�ǹ����豸),ƽ̨IP,ƽ̨�˿�
#define		LAN_PMC_EXPORT_DEV_GBEXT_FILE_CONTENT		"PmcExportDevGbExtFileContent"			 		//�豸��,�ͺ���,����,�豸�ͺ�,�����������չ,�������װλ��,�������;����,�������������,��������ӷ�λ����,�����֧�ֵķֱ���,������֯������ҵ�����ID,���ر��ٷ�Χ,�����������,ʱ���������
#define     LAN_PMC_RESET_ADMIN_PASSWORD                     "PmcResetAdminPassword"                   //�ָ���ʼ����%s



#define     LAN_PMC_DESC_EXPORT_USERLOG      "PmcExportUserLog"         //�����û�������־ %s
#define     LAN_PMC_DESC_EXPORT_PLATFORMLOG  "PmcExportPlatformLog"     //����ƽ̨ %s
#define     LAN_PMC_DESC_EXPORT_NMSHISTORYALARM          "PmcExportNmsHistoryLog"                //������ʷ�澯 %s
#define     LAN_PMC_DESC_IMPORT_DEV          "PmcImportDev"             //���������豸 %s
#define     LAN_PMC_DESC_EXPORT_DEV          "PmcExportDev"             //���������豸 %s
#define     LAN_PMC_DESC_IMPORT_DEV_GBEXT    "PmcImportDevGbExt"             //���������豸������չ��Ϣ %s
#define     LAN_PMC_DESC_EXPORT_DEV_GBEXT    "PmcExportDevGbExt"             //���������豸������չ��Ϣ %s
#define     LAN_CMSSDK_PLAT_REC_PLAY_START   "CmssdkPlatRecPlayStart"   //ƽ̨¼��طſ���%s, �豸uri: %s, token: %s
#define     LAN_CMSSDK_PU_REC_PLAY_START     "CmssdkPuRecPlayStart"     //ǰ��¼��طſ���%s, �豸uri: %s, token: %s
#define     LAN_CMSSDK_PLAT_REC_PLAY_STOP    "CmssdkPlatRecPlayStop"    //ƽ̨¼��ط�ֹͣ%s, �豸uri: %s, token: %s
#define     LAN_CMSSDK_PU_REC_PLAY_STOP      "CmssdkPuRecPlayStop"      //ǰ��¼��ط�ֹͣ%s, �豸uri: %s, token: %s
#define     LAN_CMSSDK_PLAT_REC_DOWNLOAD_START     "CmssdkPlatRecDownloadStart"    //ƽ̨¼�����ؿ���%s, �豸uri: %s, token: %s
#define     LAN_CMSSDK_PU_REC_DOWNLOAD_START       "CmssdkPuRecDownloadStart"      //ǰ��¼�����ؿ���%s, �豸uri: %s, token: %s
#define     LAN_CMSSDK_PLAT_REC_DOWNLOAD_STOP     "CmssdkPlatRecDownloadStop"     //ƽ̨¼�����ؿ���%s, �豸uri: %s, token: %s
#define     LAN_CMSSDK_PU_REC_DOWNLOAD_STOP       "CmssdkPuRecDownloadStop"       //ǰ��¼������ֹͣ%s, �豸uri: %s, token: %s

#define    LAN_ALARM_TYPE_PIN             "AlarmTypePin"             //����
#define    LAN_ALARM_TYPE_MOVE            "AlarmTypeMove"            //�ƶ����
#define    LAN_ALARM_TYPE_VIDLOST         "AlarmTypeVidlost"         //��ƵԴ��ʧ
#define    LAN_ALARM_TYPE_DISKFULL        "AlarmTypeDiskfull"        //������
#define    LAN_ALARM_TYPE_INTELLIGENT     "AlarmTypeIntelligent"     //����
#define    LAN_ALARM_TYPE_VIDEOPOLL       "AlarmTypeVideopoll"       //������ѯ
#define    LAN_ALARM_TYPE_EXCEPTION_EVENT "AlarmTypeExceptionEvent"  //�쳣�¼���Ϣ
#define    LAN_ALARM_TYPE_DEFOCUS         "AlarmTypeDefocus"         //�齹���
#define    LAN_ALARM_TYPE_SCENE_CHANGE    "AlarmTypeScemeChange"     //�����任
#define    LAN_ALARM_TYPE_REGION_INVASION "AlarmTypeRegionInvasion"  //��������
#define    LAN_ALARM_TYPE_REGION_LEAVING  "AlarmTypeRegionLeaving"   //�����뿪
#define    LAN_ALARM_TYPE_OBJECT_TAKEN    "AlarmTypeObjectTaken"     //��Ʒ��ȡ
#define    LAN_ALARM_TYPE_OBJECT_LEFT     "AlarmTypeObjectLeft"      //��Ʒ����
#define    LAN_ALARM_TYPE_PEOPLE_GATHER   "AlarmTypePeopleGather"    //��Ա�ۼ�
#define    LAN_ALARM_TYPE_CORGON          "AlarmTypeCorgon"          //������
#define    LAN_ALARM_TYPE_AUDIO_ABNORMAL  "AlarmTypeAudioAbnormal"   //�����쳣
#define    LAN_ALARM_TYPE_LUMA_CHANGE     "AlarmTypeLumaChange"      //���ȱ仯

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
#define LAN_PMS_ERROR_SUB_DOMAIN_DEL_PROGRESS_QRY               "PmsErrorSubDomainDelProgressQry"    //�����¼�ƽ̨���Ȳ�ѯʧ�ܴ�������,��Ӧ������22081
#define LAN_PMS_ERROR_MODIFY_CONFIG                                       "PmsErrorModifyConfigFail"                 //�޸������ļ�ʧ�ܴ�������
#define LAN_PMS_ALARM_RCS_OTHER_STORAGE_OVER_LICENSE_LIMIT               "PmsAlarmRcsOtherStorageOverLicenseLimit"
#define LAN_PMS_ALARM_RCS_OTHER_STORAGE_OVER_LICENSE_LIMIT_RESTORE              "PmsAlarmRcsOtherStorageOverLicenseLimitRestore"
#define LAN_PMS_ALARM_RCS_OTHER_STORAGE_OVER_LICENSE_LIMIT_RESUME               "PmsAlarmRcsOtherStorageOverLicenseLimitResume"

/* uas */
#define LAN_UAS_USER_OPERATE_LOG_TITLE                                 "UasUserOperateLogTitle"
#define LAN_UAS_ALARM_LOG_TITLE                                        "UasAlarmLogTitle"
#define LAN_UAS_DEVICE_LOG_TITLE                                       "UasDeviceLogTitle"

/*gbs*/
#define LAN_GBS_GB_SUBDOMAIN_DEVGROUP      "GbsGbSubdomainDevGroup"        //�����¼����豸�飬�����ֹ��޸�
#define LAN_GBS_GB_CHN_STREAM_TYPE_MAIN    "GbsGbChnStreamTypeMain"   //����ͨ�������ԣ�����
#define LAN_GBS_GB_CHN_STREAM_TYPE_ASSIST  "GbsGbChnStreamTypeAssist" //����ͨ�������ԣ�����
#define LAN_GBS_GB_PARENT_GBDOMAIN_DISCONNECT       "ParentGbdomainDisconnect"    //������ϼ�����
#define	LAN_GBS_EXPORT_DEV_GBEXT_FILE_CONTENT		"GbsExportDevGbExtFileContent"	//�豸��,�ͺ���,����,�豸�ͺ�,�����������չ,�������װλ��,��������λ��,�������;����,�������������,��������ӷ�λ����,�����֧�ֵķֱ���,������֯������ҵ�����ID,���ر��ٷ�Χ,�����������,ʱ���������,����,γ��


//rcs
#define LAN_ALARM_EVENT_TYPE_RECORD        "AlarmEventTypeRecord"     //�澯�¼�����-¼��
#define LAN_ALARM_EVENT_TYPE_PU            "AlarmEventTypePu"         //�澯�¼�����-�豸
#define LAN_ALARM_EVENT_TYPE_DISK          "AlarmEventTypeDisk"       //�澯�¼�����-����
#define LAN_ALARM_EVENT_TYPE_BUSINESS      "AlarmEventTypeBusiness"   //�澯�¼�����-ҵ��

#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY       "AlarmEventRecStoppedAbnormally"      //¼���쳣��ֹ�������� 
#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY_PART0 "AlarmEventRecStoppedAbnormallyPart0" //¼���쳣��ֹ��������0 
#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY_PART1 "AlarmEventRecStoppedAbnormallyPart1" //¼���쳣��ֹ��������1 
#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY_PART2 "AlarmEventRecStoppedAbnormallyPart2" //¼���쳣��ֹ��������2 
#define LAN_ALARM_EVENT_REC_STOPPED_ABNORMALLY_PART3 "AlarmEventRecStoppedAbnormallyPart3" //¼���쳣��ֹ��������3

#define LAN_RCS_REC_STOPPED_BY_REASON_LOAD_BALANCE         "RcsRecStopByReasonLoadBalance"       //¼��ֹͣ��ԭ�򣺸��ؾ���
#define LAN_RCS_REC_STOPPED_BY_REASON_BIGFILE_SWITCH_FAIL  "RcsRecStopByReasonBigFileSwitchFail" //¼��ֹͣ��ԭ�򣺴��ļ��л�ʧ��
#define LAN_RCS_REC_STOPPED_BY_REASON_PARTITION_FULL       "RcsRecStopByReasonPartitionFull"     //¼��ֹͣ��ԭ�򣺴�����
#define LAN_RCS_REC_STOPPED_BY_REASON_FORMAT_CHANGE        "RcsRecStopByReasonFormatChange"      //¼��ֹͣ��ԭ�򣺸�ʽ�仯
#define LAN_RCS_REC_STOPPED_BY_REASON_NOSTREAM		       "RcsRecStopByReasonNoStream"          //¼��ֹͣ��ԭ��������
#define LAN_RCS_REC_STOPPED_BY_REASON_UNKNOWN              "RcsRecStopByReasonUnknown"           //¼��ֹͣ��ԭ��δ֪

#define LAN_RCS_DISK_LICENSE_LIMITTED      "RcsDiskLicenseLimitted"   //License��������
#define LAN_RCS_DISK                       "RcsDisk"                  //����

#define LAN_RCS_ERRCODE                    "RcsErrCode"               //������
#define LAN_RCS_ERRCODE_19002              "RcsErrCode19002"          //������19002
#define LAN_RCS_ERRCODE_19010              "RcsErrCode19010"          //������19010
#define LAN_RCS_ERRCODE_23105              "RcsErrCode23105"          //������23105
#define LAN_RCS_ERRCODE_23106              "RcsErrCode23106"          //������23106
#define LAN_RCS_ERRCODE_23111              "RcsErrCode23111"          //������23111
#define LAN_RCS_ERRCODE_23131              "RcsErrCode23131"          //������23131
#define LAN_RCS_ERRCODE_23133              "RcsErrCode23133"          //������23133
#define LAN_RCS_ERRCODE_23137              "RcsErrCode23137"          //������23137
#define LAN_RCS_ERRCODE_23148              "RcsErrCode23148"          //������23148
#define LAN_RCS_ERRCODE_23164              "RcsErrCode23164"          //������23164
#define LAN_RCS_ERRCODE_23165              "RcsErrCode23165"          //������23165
#define LAN_RCS_ERRCODE_23166              "RcsErrCode23166"          //������23166
#define LAN_RCS_ERRCODE_23167              "RcsErrCode23167"          //������23167
#define LAN_RCS_ERRCODE_UNKNOWN            "RcsErrCodeUnknown"        //����δ֪������



#endif // _CMS_LANDESC_H_

//end of file
