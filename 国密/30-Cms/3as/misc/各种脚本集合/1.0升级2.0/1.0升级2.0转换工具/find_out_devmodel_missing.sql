delimiter $$

use KDMAAA $$

DROP PROCEDURE IF EXISTS procFindoutDevmodelMissing $$
CREATE PROCEDURE procFindoutDevmodelMissing()
BEGIN
 DECLARE done INT DEFAULT 0;
 
 declare varDevModelId varchar(32);
 declare varVideoSrcNum int; 
 declare varLicenseNum int; 
 declare varEncodeChnNum int; 
 declare varLocalStorage int; 
 declare varAlarmInput int;   
 declare varEncodeLabel int;  # BULL 9, DOME 10, NVR 7(videoSrcNum>1)
 
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
 declare varGbLable int;
 
 DECLARE cur CURSOR FOR SELECT DeviceNO,DeviceName,DeviceType,DeviceNetType,DeviceNetAddress,DeviceFixAddress,
 DeviceManufacturer, DeviceDescription, DeviceUnusedDate, DeviceCreateDate, CameraNum, EdcChnNum, VidSrcNum, AlarmInNum, AlarmOutNum, EncodeLabel
 FROM KDM3ADB.DeviceBaseInfo 
 where D2DomainNO in (select DomainNO from KDM3ADB.DomainInfo where DomainFlag = 1) and (DeviceType=21 or DeviceType=22 or DeviceType=24);
 DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
 
 if not exists(SELECT * FROM information_schema.COLUMNS where TABLE_SCHEMA = 'KDM3ADB'
			and TABLE_NAME = 'DeviceBaseInfo' and COLUMN_NAME = 'EncodeLabel') then
		alter table KDM3ADB.DeviceBaseInfo ADD EncodeLabel int default 10;
		update KDM3ADB.DeviceBaseInfo set EncodeLabel=7 where VidSrcNum > 1;
 end if;
 
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
            if varDevModelId = '' then
                select concat('DevModel not found, capDetail:', 'DevName--', varDeviceName,',varDeviceManufacturer--', varDeviceManufacturer,
				',varGbLable--', varGbLable,',varEncodeLabel--', varEncodeLabel,',varEncodeChnNum--', varEncodeChnNum,',varVideoSrcNum--', varVideoSrcNum, 
				',varLicenseNum--', varLicenseNum, ',varLocalStorage--', varLocalStorage, ',varAlarmInput--', varAlarmInput
				) as '';
            end if;
         elseif varDeviceType = 24 then
            call procGetDevModelId4Decoder(1, varDevModelId);
            if varDevModelId = '' then
                select concat('DevModel not found, capDetail:', 'DevName--', varDeviceName, ',varDecodeChannel--1') as '';
            end if;
         else
            select concat('assert invalid type:', varDeviceType);
         end if;
         
		 set done=0;
         FETCH cur INTO varDeviceNO,varDeviceName,varDeviceType,varDeviceNetType,varDeviceNetAddress,varDeviceFixAddress,
         varDeviceManufacturer,varDeviceDescription,varDeviceUnusedDate,varDeviceCreateDate,varCameraNum,varEdcChnNum,
         varVidSrcNum,varAlarmInNum,varAlarmOutNum,varEncodeLabel;
 END WHILE;
 CLOSE cur;
 
END$$

call procFindoutDevmodelMissing $$
drop PROCEDURE procFindoutDevmodelMissing $$

DELIMITER ;

