use KDMAAA;
DELIMITER $$

DROP PROCEDURE IF EXISTS `KDMAAA`.`ProcDelDevOwnerTmp` $$
CREATE PROCEDURE `KDMAAA`.`ProcDelDevOwnerTmp`(varOwnerId varchar(32))
BEGIN
	delete from tblCuser2Dev where devId in (select devId from tblDevice where owner=varOwnerId);

	delete from tblCuser2DeviceCap where devId in (select devId from tblDevice where owner=varOwnerId);
		
	delete from tblCusergrp2Dev where devId in (select devId from tblDevice where owner=varOwnerId);
		
	delete from tblCusergrp2DeviceCap where devId in (select devId from tblDevice where owner=varOwnerId);

	delete from tblDevGrp2Dev where devId in (select devId from tblDevice where owner=varOwnerId);
		
	delete from tblDeviceCapIndexData where deviceId in (select devId from tblDevice where owner=varOwnerId);
		
	delete from tblDevice where owner=varOwnerId;

END $$

call ProcDelDevOwnerTmp()$$	
DROP PROCEDURE IF EXISTS `KDMAAA`.`ProcDelDevOwnerTmp` $$	
	
DELIMITER ;
