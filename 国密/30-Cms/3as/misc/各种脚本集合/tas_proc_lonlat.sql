delimiter $$
DROP PROCEDURE IF EXISTS `procGetDevInfo` $$
CREATE PROCEDURE procGetDevInfo ()
BEGIN
 DECLARE vardevId             varchar(32);
 DECLARE vardevName            varchar(128);
 DECLARE vardevModelId        varchar(32);
 DECLARE vardevData           varchar(4096);
 DECLARE varkdmno             varchar(32);
 DECLARE varvidindex          int;
 DECLARE vargroupId        varchar(32);
 DECLARE vargrpname        varchar(128);
 DECLARE vardomainId       varchar(32);
 DECLARE vardomainName     varchar(128);
 DECLARE varManufacture    varchar(64);
 DECLARE varchannelnum        int;
 DECLARE varvidnum            int;

 DECLARE i int;
 DECLARE j int;
  
 DECLARE done int;  
 DECLARE rs_cursor CURSOR FOR select devId,devname,devModelId,devData,domainId from tblDevice where devType=1;
 declare continue handler FOR SQLSTATE '02000' SET done = 1;  
 
  CREATE TABLE IF NOT EXISTS tmpDevLstqry(puid varchar(32),devname varchar(128),channnelid int,groupid varchar(32),grpname varchar(128),
  devId varchar(32), domainId varchar(32), domainName varchar(64), manufacture varchar(64), vidsrcNum int) ENGINE = MEMORY;
  delete FROM tmpDevLstqry;
 
set done=0;
OPEN rs_cursor;
FETCH rs_cursor INTO vardevId,vardevName,vardevModelId,vardevData,vardomainId;
WHILE done=0 DO
select ifnull(ExtractValue(vardevData,"TDevExtData/extData//key[text()='kdmNO']/../value"),'') into varkdmno;
select devgroupId into vargroupId from tblDevGrp2Dev where devId=vardevId and devCapIndex=0;
select groupName into vargrpname from tblDevGroup where devGroupId = vargroupId;
if varkdmno<>'' then		
	select capNum into varchannelnum from tblDeviceModel where devCapId=4 and modelId=vardevModelId;
	select capNum into varvidnum from tblDeviceModel where devCapId=1 and modelId=vardevModelId;
	select manufacturer into varManufacture from tblDeviceModel where devCapId=1 and modelId=vardevModelId;
	select domainName into vardomainName from tblDomainRelation where domainId=vardomainId;
	set i = 0;
	while i < varchannelnum do	
		insert into tmpDevLstqry values (varkdmno,vardevName,i,vargroupId,vargrpname, vardevId, vardomainId, vardomainName, varManufacture, varvidnum); 
		set i = i +1;  
	end while;  	
end if;
set done=0;
FETCH rs_cursor INTO vardevId,vardevName,vardevModelId,vardevData,vardomainId;
END WHILE;
CLOSE rs_cursor;

select * from tmpDevLstqry;	

END$$
