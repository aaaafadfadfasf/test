delimiter $$
use KDMAAA $$

DROP PROCEDURE IF EXISTS procConvertVidsrcAlias2 $$
CREATE PROCEDURE procConvertVidsrcAlias2()
BEGIN
 DECLARE varDeviceId char(32);
 DECLARE varDeviceCapIndex int(11);
 DECLARE varDeviceCapIndexName varchar(64);
 DECLARE varDevVidSrcNum int(11);
 DECLARE done INT DEFAULT 0;
 DECLARE cur2 CURSOR FOR SELECT DeviceNO,SeqNum,convert(ChnAlias using utf8) from KDM3ADB.DeviceChnInfo f inner join tblDevice e on f.DeviceNO = e.devId and e.devType = 1 where f.ChnAlias<>'' and (f.NumberType = 131 or f.NumberType = 132);
 DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;
 
 open cur2;
 fetch cur2 into varDeviceId, varDeviceCapIndex,varDeviceCapIndexName;
 WHILE done=0 DO
	select h.capNum into varDevVidSrcNum from tblDevice g inner join tblDeviceModel h ON h.modelId=g.devModelId and g.devId=varDeviceId and h.deviceTypeId=1 and h.devCapId=1 limit 1;
	
	if varDevVidSrcNum = 0 then
	   SET varDevVidSrcNum = 1;
	end if;
	
	if varDeviceCapIndex < varDevVidSrcNum then
	    insert ignore into tblDeviceCapIndexData values(varDeviceId, 1, varDeviceCapIndex, varDeviceCapIndexName, "");
	end if;
	
	set done=0;
	fetch cur2 into varDeviceId, varDeviceCapIndex,varDeviceCapIndexName;
 END WHILE;
 CLOSE cur2;
END$$


DROP PROCEDURE IF EXISTS procConvertVidsrcAlias $$
CREATE PROCEDURE procConvertVidsrcAlias()
BEGIN
 DECLARE varDeviceId char(32);
 DECLARE varDeviceCapIndex int(11);
 DECLARE varDeviceCapIndexName varchar(64);
 DECLARE varDevVidSrcNum int(11);
 DECLARE done INT DEFAULT 0;
 DECLARE cur1 CURSOR FOR SELECT PuId,Channel,convert(VideoAlias using utf8) from UASDB.PuVideoChn a inner join tblDevice b on a.PuId = b.devId and b.devType = 1 where a.VideoAlias<>'';
 DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;

 open cur1;
 fetch cur1 into varDeviceId, varDeviceCapIndex,varDeviceCapIndexName;
 WHILE done=0 DO
	select c.capNum into varDevVidSrcNum from tblDevice d inner join tblDeviceModel c ON c.modelId=d.devModelId and d.devId=varDeviceId and c.deviceTypeId=1 and c.devCapId=1 limit 1;
	
	if varDevVidSrcNum = 0 then
	   SET varDevVidSrcNum = 1;
	end if;
	
	if varDeviceCapIndex < varDevVidSrcNum then
	    insert ignore into tblDeviceCapIndexData values(varDeviceId, 1, varDeviceCapIndex, varDeviceCapIndexName, "");
	end if;
	
	set done=0;
	fetch cur1 into varDeviceId, varDeviceCapIndex,varDeviceCapIndexName;
 END WHILE;
 CLOSE cur1;
 if exists(select TABLE_NAME from information_schema.TABLES where TABLE_SCHEMA='KDM3ADB' and TABLE_NAME='DeviceChnInfo') then
		call procConvertVidsrcAlias2();
 end if;
 
END$$

call procConvertVidsrcAlias $$
drop PROCEDURE procConvertVidsrcAlias $$

