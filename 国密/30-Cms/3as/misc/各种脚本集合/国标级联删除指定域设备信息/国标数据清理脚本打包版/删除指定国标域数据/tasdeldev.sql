use KDMAAA;
DELIMITER $$

DROP PROCEDURE IF EXISTS `KDMAAA`.`ProcDelDevTmp` $$
CREATE PROCEDURE `KDMAAA`.`ProcDelDevTmp`(varDevId varchar(32))
BEGIN
	delete from tblCuser2Dev where devId=varDevId;

	delete from tblCuser2DeviceCap where devId=varDevId;
		
	delete from tblCusergrp2Dev where devId=varDevId;
		
	delete from tblCusergrp2DeviceCap where devId=varDevId;

	delete from tblDevGrp2Dev where devId=varDevId;
		
	delete from tblDeviceCapIndexData where deviceId=varDevId;
		
	delete from tblDevice where devId=varDevId;

END $$
	
DELIMITER ;
