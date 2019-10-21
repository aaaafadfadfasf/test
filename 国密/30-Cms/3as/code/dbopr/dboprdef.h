#ifndef _DBOPRDEF_H_
#define _DBOPRDEF_H_
#include "kdvtype.h"

/* 数据库表和字段常量定义 */ 
#define T_CUSER "tblCuser"
#define F_CUSER_CUSERID "cuserId"
#define F_CUSER_USERNAME "userName"
#define F_CUSER_USERPASSWORD "userPassword"
#define F_CUSER_CUSERTYPE "cuserType"
#define F_CUSER_USERGROUPID "usergroupId"
#define F_CUSER_CUSERPRIVILEGE "cuserPrivilege"
#define F_CUSER_MULTILOGIN "multiLogin"
#define F_CUSER_USEFLAG "useFlag"
#define F_CUSER_USERDESC "userDesc"
#define F_CUSER_USEREXPIREDATE "userExpireDate"
#define F_CUSER_PASSWORDEXPIREDATE "passwordExpireDate"
#define F_CUSER_PASSWORDEXPIRETIME "pwdExpireTime"
#define F_CUSER_PASSWORDEXPIREENABLEDFLAG "pwdExpireEnabledFlag"
#define F_CUSER_LOGINIPLIST "loginIpList"
#define F_CUSER_IPLISTENABLEDFLAG "ipListEnabledFlag"
#define F_CUSER_IPBINDFLAGFORMOBILECLIENT "ipBindFlagForMobileClient"
#define F_CUSER_SWITCHNUM "switchNum"
#define F_CUSER_WATERMARK_ENABLED "waterMarkEnabled"
#define F_CUSER_WATERMARK_FONT "waterMarkFont"
#define F_CUSER_WATERMARK_FONT_SIZE "waterMarkFontSize"
#define F_CUSER_WATERMARK_ROTATION_ANGLE "waterMarkRotationAngle"
#define F_CUSER_WATERMARK_COLOR "waterMarkColor"
#define F_CUSER_WATERMARK_TRANSPARENT "waterMarkTransparent"
#define F_CUSER_WATERMARK_TILED "waterMarkTiled"

#define F_CUSER_USERCREATEDATE "userCreateDate"
#define F_CUSER_USERCREATORID "userCreatorId"
#define F_CUSER_DEVTREEVER "devTreeVer"
#define F_CUSER_DEVTREEID "devtreeId"

#define T_CUSER2DEV "tblCuser2Dev"
#define F_CUSER2DEV_CUSERID "cuserId"
#define F_CUSER2DEV_DEVID "devId"
#define F_CUSER2DEV_DEVPRIVILEGE "devPrivilege"

#define T_CUSER2DEVICECAP "tblCuser2DeviceCap"
#define F_CUSER2DEVICECAP_CUSERID "cuserId"
#define F_CUSER2DEVICECAP_DEVID "devId"
#define F_CUSER2DEVICECAP_DEVCAPID "devCapId"
#define F_CUSER2DEVICECAP_DEVCAPINDEX "devCapIndex"
#define F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE "devCapIndexPrivilege"

#define T_CUSERGROUP "tblCusergroup"
#define F_CUSERGROUP_CUSERGROUPID "cusergroupId"
#define F_CUSERGROUP_GROUPNAME "groupName"
#define F_CUSERGROUP_GROUPDESC "groupDesc"
#define F_CUSERGROUP_CREATEDATE "createDate"
#define F_CUSERGROUP_CUSERTYPE "cuserType"
#define F_CUSERGROUP_CUSERPRIVILEGE "cuserPrivilege"
#define F_CUSERGROUP_USERCREATORID "userCreatorId"
#define F_CUSERGROUP_DEVTREEVER "devTreeVer"
#define F_CUSERGROUP_SWITCHNUM "switchNum"
#define F_CUSERGROUP_WATERMARK_ENABLED "waterMarkEnabled"
#define F_CUSERGROUP_WATERMARK_FONT "waterMarkFont"
#define F_CUSERGROUP_WATERMARK_FONT_SIZE "waterMarkFontSize"
#define F_CUSERGROUP_WATERMARK_ROTATION_ANGLE "waterMarkRotationAngle"
#define F_CUSERGROUP_WATERMARK_COLOR "waterMarkColor"
#define F_CUSERGROUP_WATERMARK_TRANSPARENT "waterMarkTransparent"
#define F_CUSERGROUP_WATERMARK_TILED "waterMarkTiled"

#define T_CUSERGRP2DEV "tblCusergrp2Dev"
#define F_CUSERGRP2DEV_CUSERGROUPID "cusergroupId"
#define F_CUSERGRP2DEV_DEVID "devId"
#define F_CUSERGRP2DEV_DEVPRIVILEGE "devPrivilege"

#define T_CUSERGRP2DEVICECAP "tblCusergrp2DeviceCap"
#define F_CUSERGRP2DEVICECAP_DEVID "devId"
#define F_CUSERGRP2DEVICECAP_CUSERGROUPID "cusergroupId"
#define F_CUSERGRP2DEVICECAP_DEVCAPID "devCapId"
#define F_CUSERGRP2DEVICECAP_DEVCAPINDEX "devCapIndex"
#define F_CUSERGRP2DEVICECAP_DEVCAPINDEXPRIVILEGE "devCapIndexPrivilege"

#define T_DEVGROUP "tblDevGroup"
#define F_DEVGROUP_DEVGROUPID "devgroupId"
#define F_DEVGROUP_DOMAINID "domainId"
#define F_DEVGROUP_PARENTDEVGROUPID "parentDevgroupId"
#define F_DEVGROUP_GROUPNAME "groupName"
#define F_DEVGROUP_GROUPDESC "groupDesc"
#define F_DEVGROUP_CREATEDATE "createDate"
#define F_DEVGROUP_CREATER "creater"
#define F_DEVGROUP_PARENTDOMAINID "parentDomainId"
#define F_DEVGROUP_OWNER "owner"
#define F_DEVGROUP_FULLPATH "fullpath"
#define F_DEVGROUP_GRPSEQ "grpseq"
#define F_DEVGROUP_OLDGBID "oldGbid"
#define F_DEVGROUP_NEWGBID "newGbid"


#define T_DEVGRP2DEV "tblDevGrp2Dev"
#define F_DEVGRP2DEV_DEVGROUPID "devgroupId"
#define F_DEVGRP2DEV_DEVID "devId"
#define F_DEVGRP2DEV_DEVCAPID "devCapId"
#define F_DEVGRP2DEV_DEVCAPINDEX "devCapIndex"
#define F_DEVGRP2DEV_DEVSEQ   "devSeq"
#define F_DEVGRP2DEV_PRIMARYOLDGBID "primaryOldGbid" //国标选择推送字段
#define F_DEVGRP2DEV_PRIMARYNEWGBID "primaryNewGbid" //国标选择推送字段
#define F_DEVGRP2DEV_SECONDARYOLDGBID "secondaryOldGbid"   //国标选择推送字段
#define F_DEVGRP2DEV_SECONDARYNEWGBID "secondaryNewGbid"   //国标选择推送字段


#define T_DEVICE "tblDevice"
#define F_DEVICE_DEVID "devId"
#define F_DEVICE_DOMAINID "domainId"
#define F_DEVICE_DEVNAME "devName"
#define F_DEVICE_DEVALIAS "devAlias"
#define F_DEVICE_DEVTYPE "devType"
#define F_DEVICE_DEVMODELID "devModelId"
#define F_DEVICE_DEVPROPERTYFLAGS "devPropertyFlags"
#define F_DEVICE_USEFLAG "useFlag"
#define F_DEVICE_NETTYPE "netType"
#define F_DEVICE_NETADDR "netAddr"
#define F_DEVICE_DEVDESC "devDesc"
#define F_DEVICE_CREATEDATE "createDate"
#define F_DEVICE_EXPIREDATE "expireDate"
#define F_DEVICE_CREATER "creater"
#define F_DEVICE_DEVDATA "devData"
#define F_DEVICE_OWNER "owner"

#define T_DEVICECAP "tblDeviceCap"
#define F_DEVICECAP_CAPID "capId"
#define F_DEVICECAP_CAPNAME "capName"
#define F_DEVICECAP_CAPUNIT "capUnit"

#define T_DEVICECAPINDEXDATA "tblDeviceCapIndexData"
#define F_DEVICECAPINDEXDATA_DEVICEID "deviceId"
#define F_DEVICECAPINDEXDATA_DEVICECAPID "deviceCapId"
#define F_DEVICECAPINDEXDATA_DEVICECAPINDEX "deviceCapIndex"
#define F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME "deviceCapIndexName"
#define F_DEVICECAPINDEXDATA_DEVICECAPINDEXEXTDATA "deviceCapIndexExtData"

#define T_VIDEONAMESEARCH "tblVideoNameSearch"
#define F_VIDEONAMESEARCH_DEVID "devId"
#define F_VIDEONAMESEARCH_VIDEOID "videoId"
#define F_VIDEONAMESEARCH_SEARCHTEXT "searchText"
#define F_VIDEONAMESEARCH_VIDEONAME "videoName"

#define T_TVWALL "tblTvwall"
#define F_TVWALL_TVWALLID "tvwallId"
#define F_TVWALL_TVNUM "tvNum"
#define F_TVWALL_TVWALLSTYLE "tvwallStyle"
#define F_TVWALL_TVWALLCUSTOMSTYLEDATA "tvwallCustomStyleData"

#define T_TV2DECODER "tblTv2Decoder"
#define F_TV2DECODER_TVWALLID "tvwallId"
#define F_TV2DECODER_TVID "tvId"
#define F_TV2DECODER_DECODERID "decoderId"
#define F_TV2DECODER_DECODEROUTPUTID "decoderOutputId"

#define T_DEVICEMANUFACTURER "tblDeviceManufacturer"
#define F_DEVICEMANUFACTURER_MANUFACTURERNAME "manufacturerName"
#define F_DEVICEMANUFACTURER_BUILDIN "buildin"

#define T_DEVICEMODEL "tblDeviceModel"
#define F_DEVICEMODEL_MODELID "modelId"
#define F_DEVICEMODEL_DEVICETYPEID "deviceTypeId"
#define F_DEVICEMODEL_DOMAINID "domainId"
#define F_DEVICEMODEL_MODELNAME "modelName"
#define F_DEVICEMODEL_MANUFACTURER "manufacturer"
#define F_DEVICEMODEL_DEVCAPID "devCapId"
#define F_DEVICEMODEL_CAPNUM "capNum"
#define F_DEVICEMODEL_LICENSENUM "licenseNum"
#define F_DEVICEMODEL_CREATER "creater"
#define F_DEVICEMODEL_OWNER "owner"

#define T_DEVICETYPE "tblDeviceType"
#define F_DEVICETYPE_DEVTYPEID "devTypeId"
#define F_DEVICETYPE_DEVTYPENAME "devTypeName"
#define F_DEVICETYPE_DEVCAPID "devCapId"
#define F_DEVICETYPE_DEVCAPALIAS "devCapAlias"
#define F_DEVICETYPE_DEVCAPUNITALIAS "devCapUnitAlias"
#define F_DEVICETYPE_DEVASSIGNUNITVALUE "devAssignUnitValue"

#define T_DOMAINAUTHORIZATION "tblDomainAuthorization"
#define F_DOMAINAUTHORIZATION_INNERID "innerId"
#define F_DOMAINAUTHORIZATION_DEVICEID "deviceId"
#define F_DOMAINAUTHORIZATION_CUSERNAME "cuserName"
#define F_DOMAINAUTHORIZATION_CUSERDOMAINID "cuserDomainId"
#define F_DOMAINAUTHORIZATION_DEVPRIVILEGE "devPrivilege"
#define F_DOMAINAUTHORIZATION_AUTHORIZATIONDESC "authorizationDesc"
#define F_DOMAINAUTHORIZATION_AUTHORIZATIONDATE "authorizationDate"
#define F_DOMAINAUTHORIZATION_CREATORID "creatorId"
#define F_DOMAINAUTHORIZATION_PROCESSSTATE "processState"

#define T_FOREIGNCUSER2DEVCAPPRIVILEGE "tblForeignCuser2DevCapPrivilege"
#define F_FOREIGNCUSER2DEVCAPPRIVILEGE_INNERID "innerId"
#define F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPID "deviceCapId"
#define F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPINDEX "deviceCapIndex"
#define F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPPRIVILEGE "deviceCapPrivilege"

#define T_DOMAINRELATION "tblDomainRelation"
#define F_DOMAINRELATION_DOMAINID "domainId"
#define F_DOMAINRELATION_DOMAINNAME "domainName"
#define F_DOMAINRELATION_PARENTDOMAINID "parentDomainId"
#define F_DOMAINRELATION_DOMAINALIAS "domainAlias"

#define T_PRIVATEDEVICE "tblPrivateDevice"
#define F_PRIVATEDEVICE_PRIVATEDEVICEID "privateDeviceId"
#define F_PRIVATEDEVICE_CREATEDATE "createDate"

#define T_TUSER "tblTuser"
#define F_TUSER_TUSERID "tuserId"
#define F_TUSER_USERNAME "userName"
#define F_TUSER_USERPASSWORD "userPassword"
#define F_TUSER_TUSERTYPE "tuserType"
#define F_TUSER_MULTILOGIN "multiLogin"
#define F_TUSER_USEFLAG "useFlag"
#define F_TUSER_USERDESC "userDesc"
#define F_TUSER_EXPIREDATE "expireDate"
#define F_TUSER_PASSWORDEXPIREDATE "passwordExpireDate"
#define F_TUSER_CREATEDATE "createDate"

#define T_SYNCDESTDOMAINS "tblSyncDestDomains"
#define F_SYNCDESTDOMAINS_DOMAINID "domainId"
#define F_SYNCDESTDOMAINS_DOAMAINNAME "doamainName"
#define F_SYNCDESTDOMAINS_SYNCTYPE "syncType"

#define T_SYNCDATARECORDS_PREFIX "tblSyncDataRecords_"
#define F_SYNCDATARECORDS_SN "sn"
#define F_SYNCDATARECORDS_DATATYPE "dataType"
#define F_SYNCDATARECORDS_DATAID "dataId"
#define F_SYNCDATARECORDS_OPERATETYPE "operateType"
#define F_SYNCDATARECORDS_DATADOMAINID "dataDomainId"

#define T_DEVICEAUTOASSIGN "tblDeviceAutoAssign"
#define F_DEVICEAUTOASSIGN_GROUPID "groupId"
#define F_DEVICEAUTOASSIGN_OWNERID "ownerId"
#define F_DEVICEAUTOASSIGN_OWNERTYPE "ownerType"
#define F_DEVICEAUTOASSIGN_OPERTIME "operTime"

#define T_DEVTREE "tblDevTree"
#define F_DEVTREE_ID "devtreeId"
#define F_DEVTREE_NAME "devtreeName"
#define F_DEVTREE_VERSION "devTreeVer"
#define F_DEVTREE_CANBEUSE4GB "canBeUse4Gb"
#define F_DEVTREE_TYPE  "devtreeType"

#define T_PTZLOCK "tblPtzLock"
#define F_PTZLOCK_DEVID "devId"
#define F_PTZLOCK_VIDSRCID "videoSrcId"
#define F_PTZLOCK_BEGINTIME "begintime"
#define F_PTZLOCK_ENDTIME "endtime"
#define F_PTZLOCK_USERID "userId"
#define F_PTZLOCK_PASSWORD "password"


#define F_DEVERRGBINFO_DEVGRPID  "devGrpId"
#define F_DEVERRGBINFO_DEVID     "devId"
#define F_DEVERRGBINFO_VIDSRCIDX "vidSrcIdx"
#define F_DEVERRGBINFO_ERRTYPE   "errType"
#define F_DEVERRGBINFO_ERRIDX    "errIdx"


/* 视图和常量定义 */
#define V_DEVICEFULLINFOANDPRIVILEGE_DEV "vDevicefullinfoAndPrivilege_dev"    // 按照用户权限
#define V_DEVICEFULLINFOANDPRIVILEGE_DEV2 "vDevicefullinfoAndPrivilege_dev2"  // 按照用户组权限
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVID "devId"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVCAPID "devCapId"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVCAPINDEX "devCapIndex"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVICECAPINDEXNAME "deviceCapIndexName"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVICECAPINDEXDATA "deviceCapIndexExtData"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVCAPINDEXPRIVILEGE "devCapIndexPrivilege"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DOMAINID "domainId"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVTYPE "devType"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVMODELID "devModelId"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVNAME "devName"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVALIAS "devAlias"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_USEFLAG "useFlag"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_NETTYPE "netType"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_NETADDR "netAddr"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVDESC "devDesc"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_CREATEDATE "createDate"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_EXPIREDATE "expireDate"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_CREATER "creater"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVDATA "devData"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_OWNER "owner"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_DEVPRIVILEGE "devPrivilege"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_CUSERID "cuserId"
#define F_V_DEVICEFULLINFOANDPRIVILEGE_CUSERGROUPID "cusergroupId"

#define V_TVWALLBASEINFO "vTvwallbaseInfo"   // 电视培基本信息
#define F_V_TVWALLBASEINFO_DEVID "devId"
#define F_V_TVWALLBASEINFO_DOMAINID "domainId"
#define F_V_TVWALLBASEINFO_DEVNAME "devName"
#define F_V_TVWALLBASEINFO_DEVALIAS "devAlias"
#define F_V_TVWALLBASEINFO_DEVTYPE "devType"
#define F_V_TVWALLBASEINFO_DEVMODELID "devModelId"
#define F_V_TVWALLBASEINFO_DEVPROPERTYFLAGS "devPropertyFlags"
#define F_V_TVWALLBASEINFO_USEFLAG "useFlag"
#define F_V_TVWALLBASEINFO_NETTYPE "netType"
#define F_V_TVWALLBASEINFO_NETADDR "netAddr"
#define F_V_TVWALLBASEINFO_DEVDESC "devDesc"
#define F_V_TVWALLBASEINFO_CREATEDATE "createDate"
#define F_V_TVWALLBASEINFO_EXPIREDATE "expireDate"
#define F_V_TVWALLBASEINFO_TVNUM "tvNum"
#define F_V_TVWALLBASEINFO_TVWALLSTYLE "tvwallStyle"
#define F_V_TVWALLBASEINFO_TVWALLCUSTOMSTYLEDATA "tvwallCustomStyleData"

/* 存储过程常量定义，一般需要用到游标的场景时采用存储过程实现 */
#define P_SPLITPAGEQRY "procSplitpageqry"      // 实现分页查询功能的存储过程
#define P_CUSERQRY "procCuserqry"              // 查询CU用户
#define P_CUSERQRY4COUNT "procCuserqry4Count"  // 查询CU用户(只查询总数)
#define P_DEVICEFULLINFOANDPRIVILEGE "procDevicefullinfoAndPrivilege"
#define P_DEVICEFULLINFOANDPRIVILEGE2 "procDevicefullinfoAndPrivilege2"
#define P_DEVICEFULLINFOANDPRIVILEGE3 "procDevicefullinfoAndPrivilege3"
#define P_DEVICEFULLINFOANDPRIVILEGE4 "procDevicefullinfoAndPrivilege4"
#define P_CUSERDEVQRYCOUNT "procCuserDevQryCount"
#define P_CUSERDEVQRY "procCuserDevQry"
#define P_CUSERDEVADD "procCuserDevAdd"
#define P_CUSERDEVCAPADD "procCuserDevCapAdd"
#define P_CUSERGRPDEVQRYCOUNT "procCusergrpDevQryCount"
#define P_CUSERGRPDEVQRY "procCusergrpDevQry"
#define P_CUSERGRPDEVADD "procCusergrpDevAdd"
#define P_CUSERGRPDEVCAPADD "procCusergrpDevCapAdd"
#define P_DEVGRPDEVQRY "procDevGrpDevQry"
#define P_DEVMODELQRY "procDevModelQry"
#define P_COPYCUSERGRPPRIVILEGE "procCopyCusergrpPrivilege"
#define P_CUSERGRPDEL "procCuserGrpDel"
#define P_CUSERGRPDELBYCREATORID "procCuserGrpDelByCreatorId"
#define P_TVWALLQRY "procTvwallQry"
#define P_CUSERDEVCAPWITHOUTASSIGNQRY "procCuserDevCapWithoutAssignQry"
#define P_CUSERDEVCAPALREADYASSIGNQRY "procCuserDevCapAlreadyAssignQry"
#define P_DEVGRPQRY "procDevGrpQry"
#define P_DEVGRPPATHQRY "procDevGrpPathQry"
#define P_DEVGRPPATHQRY4COUNT "procDevGrpPathQry4Count"
#define P_CUSERDEVGRPVIDSRCQRY "procCUserDevGrpVidSrcQry"
#define P_DISPERSEDEVSEQ "procDisperseDevSeq"

#define P_DELALLDATAFROMCERTAINDOMAIN "procDelAllDataFromCertainDomain"
#define P_PARENTDOMAINSYNCDATACONFIGADD "procParentDomainSyncDataConfigAdd"
#define P_PARENTDOMAINSYNCDATACONFIGDEL "procParentDomainSyncDataConfigDel"
#define P_EXPORTCUSERDEVICEDATA "procExportCuserDeviceData"
#define P_GRPPATHCALC "procGrpPathCalc"
#define P_GRPSEQCALC "procGrpSeqCalc"
#define P_UPDATEALLTREEVERBYDEVID      "procUpdateAllTreeVerByDevId"
#define P_COPYDEVTREE        "procCopyDevTree"
#define P_MATCHDEVGRPCIVILCODE "procMatchDevGrpCivilcode"
#define P_MATCHGBIDLASTBITS  "procMatchGbidLastBits"
#define P_QRYDEVGBINFOBYNAME "procQryDevGbInfoByName"
#define P_QRYNEXTDEVGBIDERRINFO "procQryNextDevGbidErrInfo"
#define P_QRYVIRGRPCIVILCODE  "procQryVirGrpCivilcode"

#define T_DEVGRP_PREFIX "tblDevGrp_"
static const char* s_szDevGroupTableTmpl = "create table if not exists tblDevGrp_%s						   \
										   (														\
										   devgroupId           char(32) not null,				   \
										   domainId             char(32) not null,				   \
										   parentDevgroupId     char(32),						   \
										   groupName            varchar(64) not null,			   \
										   groupDesc            varchar(1024),					   \
										   createDate           datetime,						   \
										   creater              int not null default 0, 		   \
										   owner                varchar(64) default '',			   \
										   fullpath			    varchar(2048) default '',  		   \
										   grpseq    	         double not null default 0, 		   \
										   primary key (devgroupId)								   \
										   );";

static const char* s_szDevGroupTableTmplGb = "create table if not exists tblDevGrp_%s						   \
											 (														\
											 devgroupId           char(32) not null,				   \
											 domainId             char(32) not null,				   \
											 parentDevgroupId     char(32),						   \
											 groupName            varchar(64) not null,			   \
											 groupDesc            varchar(1024),					   \
											 createDate           datetime,						   \
											 creater              int not null default 0, 		   \
											 owner                varchar(64) default '',			   \
											 fullpath			    varchar(2048) default '',  		   \
											 grpseq    	         double not null default 0, 		   \
											 oldGbid              char(20),                          \
											 newGbid              char(20),                          \
											 primary key (devgroupId)								   \
											 );";



#define T_DEVGRP2DEV_PREFIX "tblDevGrp2Dev_"
static const char* s_szDevGrp2DevTableTmpl = "create table if not exists tblDevGrp2Dev_%s				 \
										   (														 \
										   devgroupId           char(32) not null,					\
										   devId                char(32) not null,					\
										   devCapId             integer not null,					\
										   devCapIndex          integer not null,					\
										   devSeq               double default 0,                   \
										   primary key (devgroupId, devId, devCapId, devCapIndex),	\
                                           index index4devid(devId)                                 \
										   );";

static const char* s_szDevGrp2DevTableTmplGb = "create table if not exists tblDevGrp2Dev_%s				 \
											   (														 \
											   devgroupId           char(32) not null,					\
											   devId                char(32) not null,					\
											   devCapId             integer not null,					\
											   devCapIndex          integer not null,					\
											   primaryOldGbid       char(20),                           \
											   primaryNewGbid       char(20),                           \
											   secondaryOldGbid     char(20),                           \
											   secondaryNewGbid     char(20),                           \
											   devSeq               double default 0,                   \
											   primary key (devgroupId, devId, devCapId, devCapIndex),	\
                                               index index4devid(devId)                                 \
											   );";


/* 确定TQueryOrderFieldId对应的表列名 */
#include "tas_struct.h"
const s8* GetQueryOrderFieldName(TQueryOrderFieldId id);

#endif // _DBOPRDEF_H_
