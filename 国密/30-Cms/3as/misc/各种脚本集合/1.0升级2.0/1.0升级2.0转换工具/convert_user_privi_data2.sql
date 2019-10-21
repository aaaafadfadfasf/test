insert ignore into tblCusergrp2Dev
select KDM3ADB.CUGroup2DeviceInfo.CUGroupNO,CUG2DeviceNO,-1
from KDM3ADB.CUGroup2DeviceInfo inner join tblDevice
on KDM3ADB.CUGroup2DeviceInfo.CUG2DeviceNO=tblDevice.devId
where tblDevice.devType=1;
-- 只保留编码器与解码器的权限，不包括电视墙的
-- devPrivilege：从CU2DevicePrivilege转换，当前为最大权限-1

delimiter $$
use KDMAAA $$

DROP PROCEDURE IF EXISTS procConvertCusergrp $$
CREATE PROCEDURE procConvertCusergrp()
BEGIN
    declare varAdminId varchar(32);
    select cuserId into varAdminId from tblCuser where userName='admin';

    insert ignore into tblCusergroup
    select KDM3ADB.CUGroupInfo.CUGroupNO,convert(CUGroupName using utf8),convert(CUGroupDescription using utf8),
    '2014-09-05 00:00:00', 1, 255, varAdminId, '2014-09-05 00:00:00'
    from KDM3ADB.CUGroupInfo inner join KDM3ADB.CUGroup2SysPrivilegeInfo;
END$$
-- cuserPrivilege:从CU2SystemPrivilege转换待调查 当前为最大权限 255
-- userExpireDate,passwordExpireDate, userCreateDate: 转换  当前写死比如'2030-01-01 00:00:00'
-- cuserType：1.0的用户组没有这个概念    当前写死1,这会造成有些用户划分组不对的
-- userCreatorId：1.0没有这个概念  全部写为超级管理员，其它用户看不到了 
-- CUGroupForceUniform强制统一的概念 需要研究确认，2.0中都是强制的（此次现场26个用户组属于非强制的，21个属于强制的，由于非强制的比较多，与laiqi讨论了非强制组中的用户全部移动至未分组）
call procConvertCusergrp $$
drop PROCEDURE procConvertCusergrp $$ 

DROP PROCEDURE IF EXISTS procConvertCuserDeviceCapPrivi $$
CREATE PROCEDURE procConvertCuserDeviceCapPrivi()
BEGIN    
    DECLARE done INT DEFAULT 0;
    declare varVidsrcNum int;
    declare varVidEncNum int;
    declare varCUserNO varchar(32);
    declare varCU2DeviceNO varchar(32);
    declare varIntTmp int;
    
    DECLARE cur CURSOR FOR select CUGroupNO,CUG2DeviceNO
    from KDM3ADB.CUGroup2DeviceInfo inner join tblDevice
    on KDM3ADB.CUGroup2DeviceInfo.CUG2DeviceNO=tblDevice.devId
    where tblDevice.devType=1;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1; 
    
    set done=0;
    OPEN cur;
    FETCH cur INTO varCUserNO, varCU2DeviceNO;
    WHILE done=0 DO
        set varVidsrcNum=0;
        set varVidEncNum=0;
        select capNum into varVidsrcNum from tblDeviceModel
        where modelId in (select devModelId from tblDevice where tblDevice.devId=varCU2DeviceNO)
        and devCapId = 1;
        select capNum into varVidEncNum from tblDeviceModel
        where modelId in (select devModelId from tblDevice where tblDevice.devId=varCU2DeviceNO)
        and devCapId = 4;
        
        set varIntTmp=0;
        while varIntTmp < varVidsrcNum do
            insert ignore into tblCusergrp2DeviceCap values(varCUserNO,varCU2DeviceNO,1,varIntTmp,-1);
            
            set varIntTmp=varIntTmp+1;
        end while;
        
        set varIntTmp=0;
        while varIntTmp < varVidEncNum do
            insert ignore into tblCusergrp2DeviceCap values(varCUserNO,varCU2DeviceNO,1,varIntTmp,-1);
            
            set varIntTmp=varIntTmp+1;
        end while;
        
		set done=0;
		
        FETCH cur INTO varCUserNO, varCU2DeviceNO;
    END WHILE;
END$$
-- 拆解1.0 CU2DeviceChnPriv中的通道划归数据，当前为全部通道权限
-- devCapIndexPrivilege：从拆解出来的通道权限数值转换，当前为最大权限-1

call procConvertCuserDeviceCapPrivi $$
drop PROCEDURE procConvertCuserDeviceCapPrivi $$

delimiter ;


