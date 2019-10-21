delimiter $$

use KDMAAA $$

DROP PROCEDURE IF EXISTS procConvertGbDevFromPlatform1 $$
CREATE PROCEDURE procConvertGbDevFromPlatform1()
BEGIN
 DECLARE done INT DEFAULT 0;
 
 DECLARE varDomainId varchar(32);
 DECLARE varIntTmp int;
 DECLARE varTmpGbPDeviceNO varchar(32);
 
 DECLARE varGbDeviceNO varchar(32);
 DECLARE varDeviceNO varchar(32);
 DECLARE varChnAlias varchar(120);
 DECLARE varSeqNum int;
 DECLARE varNumberType int;
 
DECLARE varkdChannelIndex int;
DECLARE vargbLabel int;
DECLARE varManufacturer varchar(64);
DECLARE varModel varchar(32);
DECLARE varparental smallint; 
DECLARE varParentID varchar(512);
DECLARE varIpAddress varchar(64);
DECLARE varChannel int;
DECLARE varChannelNum int;

DECLARE varRootGbDeviceNO varchar(32);
DECLARE varRootGrpGbId varchar(32);
DECLARE varGbUa varchar(32);

 DECLARE cur CURSOR FOR SELECT GBDeviceNO,Gbdev.DeviceNO,ChnAlias,SeqNum,NumberType,DeviceManufacturer,DeviceNetAddress,EdcChnNum 
 FROM KDM3ADB.GBDeviceInfo as Gbdev inner join KDM3ADB.DeviceBaseInfo as devBase on Gbdev.DeviceNO=devBase.DeviceNO  and devBase.DeviceManufacturer like("GB%")
 where D2DomainNO in (SELECT DomainNO from KDM3ADB.DomainInfo where DomainFlag = 1) and (DeviceType=21 or DeviceType=22 or DeviceType=24) and NumberType in(112,131,111) ;
 DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;

SELECT cuserId into varDomainId from tblCuser where userName = 'admin';
SELECT GBDeviceNO into varRootGbDeviceNO FROM KDM3ADB.GBDeviceInfo where DeviceNO in (SELECT DomainNO from KDM3ADB.DomainInfo where DomainFlag = 1) and NumberType=200;
SELECT gbid into varRootGrpGbId from GBS.tblGbDevice where kdid=varDomainId;

 set varModel='';
 
 set done=0;
 OPEN cur;
 FETCH cur INTO varGbDeviceNO,varDeviceNO,varChnAlias,varSeqNum,varNumberType,varManufacturer,varIpAddress,varChannelNum;
 WHILE done=0 DO
		 if varNumberType = 112 or varNumberType=111 then
			set varkdChannelIndex=-1;
			set varparental=1;
			set varParentID=varRootGbDeviceNO;
			set varChannel=varChannelNum;
			set varGbUa=varGbDeviceNO;
		 else
			set varkdChannelIndex=varSeqNum;
			set varparental=0;
			SELECT GBDeviceNO into varTmpGbPDeviceNO from KDM3ADB.GBDeviceInfo where DeviceNO=varDeviceNO and (NumberType=112 or NumberType=111);
			set varParentID=concat(varTmpGbPDeviceNO,'/', varRootGrpGbId);
			set varChannel=1;
			set varGbUa=varTmpGbPDeviceNO;
		 end if;
		
		if SUBSTRING(varManufacturer,1,2) = 'GB' then
			set varManufacturer=SUBSTRING(varManufacturer,4);
			set vargbLabel=1;
		else
			set vargbLabel=0;
		end if;
		 
            INSERT IGNORE INTO GBS.tblGbDevice(gbid,kdid,kdDomainId,kdChannelIndex,gbLabel,gbua,name,manufacturer,model,owner,civilCode,block,address,parental,
			parentID,safetyWay,registerWay,certNum,certifiable,certErrCode,certEndTime,secrecy,ipAddress,ipPort,password,devStatus,longitude,latitude,
			firmware,channel,online,runStatus,abnormalReason,businessGroupID) 
            VALUES (varGbDeviceNO,varDeviceNO,varDomainId,varkdChannelIndex,vargbLabel,varGbUa,varChnAlias,varManufacturer,varModel,'','','','',varparental,
			        varParentID,0,1,'',0,0,2114352000,0,varIpAddress,0,'','OFF',0.0,0.0,'',varChannel,'OFFLINE','OK','','');
					
		 set done=0;
		  
         FETCH cur INTO varGbDeviceNO,varDeviceNO,varChnAlias,varSeqNum,varNumberType,varManufacturer,varIpAddress,varChannelNum;
 END WHILE;
 CLOSE cur;
 
END$$

call procConvertGbDevFromPlatform1 $$
drop PROCEDURE procConvertGbDevFromPlatform1 $$

DELIMITER ;

