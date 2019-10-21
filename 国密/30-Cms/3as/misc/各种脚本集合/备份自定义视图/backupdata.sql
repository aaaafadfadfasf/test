delimiter $$
use KDMAAA

DROP PROCEDURE if EXISTS  `procBackupData` $$
CREATE PROCEDURE procBackupData (devTreeId char(32))
BEGIN
   declare tsql varchar(1000) default '';
   
   set tsql=concat("drop table if exists tmpTblDevGrp2Dev_",devTreeId);
   SET @tsql_=tsql;
   PREPARE tsqlcmd FROM @tsql_;
   EXECUTE tsqlcmd;
   DEALLOCATE PREPARE tsqlcmd;
   
   set tsql = concat("create table tmpTblDevGrp2Dev_",devTreeId,"(devGrpId char(32), deviceName varchar(100))");
   SET @tsql_=tsql;
   PREPARE tsqlcmd FROM @tsql_;
   EXECUTE tsqlcmd;
   DEALLOCATE PREPARE tsqlcmd;
   
   set tsql=concat("insert into tmpTblDevGrp2Dev_",devTreeId," select a.devgroupId,convert(b.devName using utf8) from tblDevGrp2Dev_",devTreeId," a, tblDevice b where b.devId=a.devId");
   SET @tsql_=tsql;
   PREPARE tsqlcmd FROM @tsql_;
   EXECUTE tsqlcmd;
   DEALLOCATE PREPARE tsqlcmd;	 

END $$

delimiter ;