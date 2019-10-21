delimiter $$

use KDMAAA $$

DROP PROCEDURE IF EXISTS procConvertDevFromPlatform1 $$
CREATE PROCEDURE procConvertDevFromPlatform1()
BEGIN
 DECLARE done INT DEFAULT 0;
 
 declare varDevModelId varchar(32);
 declare varVideoSrcNum int;  # 1
 declare varLocalStorage int; # 2
 declare varAlarmInput int;   # 3
 declare varEncodeChnNum int; # 4
 declare varEncodeLabel int;  # BULL 9, DOME 10, NVR 7(videoSrcNum>1)
 
 declare varDomainId varchar(32);
 declare varIntTmp int;
 
 declare varDECODE_CHANNEL int; # 5
 declare varDECODE_OUTPUTCHANNEL int; # 13
 
 DECLARE varDeviceNO varchar(32);
 DECLARE varDeviceName varchar(100);
 declare varDeviceType int;
 declare varDeviceNetType int;
 declare varDeviceNetAddress varchar(64);
 declare varDeviceFixAddress varchar(64);
 declare varDeviceManufacturer varchar(64);
 declare varDeviceDescription varchar(1024);
 declare varDeviceUnusedDate int;
 declare varDeviceCreateDate int;
 declare varCameraNum int;
 declare varEdcChnNum int;
 declare varVidSrcNum int;
 declare varAlarmInNum int;
 declare varAlarmOutNum int;
 declare varLicenseNum int;
 declare varGbLable int;
 
 DECLARE cur CURSOR FOR SELECT DeviceNO,DeviceName,DeviceType,DeviceNetType,DeviceNetAddress,DeviceFixAddress,
 DeviceManufacturer, DeviceDescription, DeviceUnusedDate, DeviceCreateDate, CameraNum, EdcChnNum, VidSrcNum, AlarmInNum, AlarmOutNum, EncodeLabel
 FROM KDM3ADB.DeviceBaseInfo 
 where D2DomainNO in (select DomainNO from KDM3ADB.DomainInfo where DomainFlag = 1) and (DeviceType=21 or DeviceType=22 or DeviceType=24);
 DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;
 
 select cuserId into varDomainId from tblCuser 
 where userName = 'admin';

 set done=0;
 OPEN cur;
 FETCH cur INTO varDeviceNO,varDeviceName,varDeviceType,varDeviceNetType,varDeviceNetAddress,varDeviceFixAddress,
 varDeviceManufacturer,varDeviceDescription,varDeviceUnusedDate,varDeviceCreateDate,varCameraNum,varEdcChnNum,
 varVidSrcNum,varAlarmInNum,varAlarmOutNum,varEncodeLabel;
 WHILE done=0 DO
         set varDevModelId='';
         if varDeviceType = 21 or varDeviceType = 22 then
            set varVideoSrcNum=varVidSrcNum;
            set varLicenseNum=varCameraNum;
            set varEncodeChnNum=varEdcChnNum;
            set varAlarmInput=varAlarmInNum;
            
			if varDeviceType = 21 then
				set varLocalStorage=1;
			else
				set varLocalStorage=0;
			end if;
			
			if SUBSTRING(varDeviceManufacturer,1,2) = 'GB' then
				set varGbLable=1;
				set varDeviceManufacturer=SUBSTRING(varDeviceManufacturer,4);
			else
				set varGbLable=0;
			end if;
			
            call procGetDevModelId4Encoder(varVideoSrcNum, varLicenseNum, varLocalStorage, varAlarmInput, varEncodeChnNum, varEncodeLabel,varDeviceManufacturer,varGbLable, varDevModelId);
         elseif varDeviceType = 24 then
            call procGetDevModelId4Decoder(1, varDevModelId);
         else
            select concat('assert invalid type:', varDeviceType) as '';
         end if;
         
         if varDevModelId<>'' then
            if varDeviceType<>24 then
                INSERT IGNORE INTO tblCuser2Dev VALUES (varDomainId, varDeviceNO, -1);
                
                set varIntTmp=0;
                while varIntTmp < varVideoSrcNum do
                    INSERT IGNORE INTO tblCuser2DeviceCap VALUES (varDomainId, varDeviceNO,1,varIntTmp,-1);
					 INSERT IGNORE INTO tblDevGrp2Dev VALUES (varDomainId, varDeviceNO,1,varIntTmp);
                    set varIntTmp=varIntTmp+1;
                end while;
                
                set varIntTmp=0;
                while varIntTmp < varEncodeChnNum do
                    INSERT IGNORE INTO tblCuser2DeviceCap VALUES (varDomainId, varDeviceNO,4,varIntTmp,-1);
                    set varIntTmp=varIntTmp+1;
                end while;
                                
               
		    INSERT IGNORE INTO tblDevice(devId,domainId,devName,devAlias,devType,devModelId,devPropertyFlags,useFlag,netType,netAddr,devDesc,createDate,expireDate) 
            VALUES (varDeviceNO,varDomainId,varDeviceName,'',1,varDevModelId,0,1,0,varDeviceNetAddress,'','2015-05-15 11:02:55','2036-12-28 00:00:00');
            else
                INSERT IGNORE INTO tblDevGrp2Dev VALUES (varDomainId, varDeviceNO,6,0);
				INSERT IGNORE INTO tblDevice(devId,domainId,devName,devAlias,devType,devModelId,devPropertyFlags,useFlag,netType,netAddr,devDesc,createDate,expireDate) 
               VALUES (varDeviceNO,varDomainId,varDeviceName,'',2,varDevModelId,0,1,0,varDeviceNetAddress,'','2015-05-15 11:02:55','2036-12-28 00:00:00');
            end if;
            

         else
            select concat('we can not find Device model for ', varDeviceName) as '';
         end if;
         
		 set done=0;
		  
         FETCH cur INTO varDeviceNO,varDeviceName,varDeviceType,varDeviceNetType,varDeviceNetAddress,varDeviceFixAddress,
         varDeviceManufacturer,varDeviceDescription,varDeviceUnusedDate,varDeviceCreateDate,varCameraNum,varEdcChnNum,
         varVidSrcNum,varAlarmInNum,varAlarmOutNum,varEncodeLabel;
 END WHILE;
 CLOSE cur;
 update tblCuser set devTreeVer=uuid();
END$$

call procConvertDevFromPlatform1 $$
drop PROCEDURE procConvertDevFromPlatform1 $$


DROP PROCEDURE IF EXISTS procConvertKdDomain $$
CREATE PROCEDURE procConvertKdDomain()
BEGIN
    declare varDomainId varchar(32);
	SELECT DomainNO into varDomainId FROM KDM3ADB.DomainInfo;
    update tblDomainRelation set domainAlias=concat("<TDomainExtData><extData size='2'><item index='0'><key>fixAddr</key></item><item index='1'><key>kdmNO</key><value>",varDomainId,"</value></item></extData></TDomainExtData>");
END$$
call procConvertKdDomain $$
drop PROCEDURE procConvertKdDomain $$

DELIMITER ;

