delimiter $$
use KDMAAA

DROP PROCEDURE if EXISTS  `procRecoverData` $$
CREATE PROCEDURE procRecoverData (devTreeId char(32))
BEGIN
   declare tsql varchar(1000) default '';
   declare varDevGrpId char(32) default '';
   declare varDevName varchar(100) default '';
   declare varDevId char(32) default '';
   declare varExist int default 0;
   declare done int default 0;
   declare cur cursor for select a.*,b.devId from tmpTblBackupRecords a, tblDevice b where b.devName=a.deviceName ;
   declare CONTINUE HANDLER FOR NOT FOUND SET done = 1;
   
   drop temporary table if exists tmpTblExistRecords;
   create temporary table tmpTblExistRecords(devGrpId char(32), deviceId char(32));
   delete from tmpTblExistRecords; 
   
   drop temporary table if exists tmpTblBackupRecords;
   set tsql = concat("create temporary table tmpTblBackupRecords select * from tmpTblDevGrp2Dev_",devTreeId);
   SET @tsql_=tsql;
   PREPARE tsqlcmd FROM @tsql_;
   EXECUTE tsqlcmd;
   DEALLOCATE PREPARE tsqlcmd;
   
   
   open cur;
   FETCH cur into varDevGrpId,varDevName,varDevId;
   WHILE done = 0 DO
         delete from tmpTblExistRecords;
		 
		 set tsql = concat("insert into tmpTblExistRecords select a.devgroupId,a.devId from tblDevGrp2Dev_",devTreeId," a inner join tblDevice b ON b.devId= a.devId and a.devgroupId=\'",varDevGrpId,"\' and b.devName=\'",varDevName,"\'");
         SET @tsql_=tsql;
         PREPARE tsqlcmd FROM @tsql_;
         EXECUTE tsqlcmd;
         DEALLOCATE PREPARE tsqlcmd;
		 
		 select count(*) into varExist from tmpTblExistRecords;
		 if varExist = 0 then
             if varDevId <> '' then
		         set tsql = concat("insert ignore into tblDevGrp2Dev_",devTreeId, " values(\'",varDevGrpId,"\',\'",varDevId,"\',1,0)");
                 SET @tsql_=tsql;
                 PREPARE tsqlcmd FROM @tsql_;
                 EXECUTE tsqlcmd;
                 DEALLOCATE PREPARE tsqlcmd;
             end if;
		 end if;
		 set done = 0;
         FETCH cur into varDevGrpId,varDevName,varDevId; 
   END WHILE;
   close cur;
   
   set tsql=concat("drop table tmpTblDevGrp2Dev_",devTreeId);
   SET @tsql_=tsql;
   PREPARE tsqlcmd FROM @tsql_;
   EXECUTE tsqlcmd;
   DEALLOCATE PREPARE tsqlcmd;
END $$

delimiter ;