delimiter $$
use KDMAAA $$

DROP PROCEDURE IF EXISTS procConvertCuser $$
CREATE PROCEDURE procConvertCuser()
BEGIN
    declare varAdminId varchar(32);
    declare varAdminIdOld varchar(32);
    
    insert ignore into tblCuser
    select KDM3ADB.CUserBaseInfo.CUserNO,convert(CUserName using utf8),'21218cca77804d2ba1922c33e0151105',2-CUserType,CU2CUGroupNO,255,CUserMLoginFlag,
    CUserUseState,convert(CUserDescription using utf8),'2030-01-01 00:00:00','2030-01-01 00:00:00','2014-09-05 00:00:00',
    CUserCreatorNO, '2014-09-05 00:00:00'
    from KDM3ADB.CUserBaseInfo inner join KDM3ADB.CUser2SysPrivilegeInfo
    where CUserName <> 'admin';
    
    select cuserId into varAdminId from tblCuser where userName='admin';
    select CUserNO into varAdminIdOld from KDM3ADB.CUserBaseInfo where CUserName='admin';
    
    update tblCuser set userCreatorId=varAdminId
    where userCreatorId=varAdminIdOld;
    
    update tblCuser set usergroupId='' where usergroupId='00000000000000000000000000000000';

    update tblCuser set usergroupId='' where exists(select 1 from KDM3ADB.CUGroupInfo where CUGroupNO=usergroupId and CUGroupForceUniform=0);
END$$
-- userPassword:为888888 
-- cuserPrivilege:从CU2SystemPrivilege转换待调查 当前为最大权限 255
-- userExpireDate,passwordExpireDate, userCreateDate: 转换  当前写死比如'2030-01-01 00:00:00'
-- 现场的数据应该被人改过？有300多个超级管理员
call procConvertCuser $$
drop PROCEDURE procConvertCuser $$

insert ignore into tblCuser2Dev
select CUserNO,CU2DeviceNO,-1
from KDM3ADB.CUser2DeviceInfo inner join tblCuser inner join tblDevice
on KDM3ADB.CUser2DeviceInfo.CUserNO=tblCuser.cuserId and KDM3ADB.CUser2DeviceInfo.CU2DeviceNO=tblDevice.devId
where tblDevice.devType=1 $$
-- 只保留编码器与解码器的权限，不包括电视墙的
-- devPrivilege：从CU2DevicePrivilege转换，当前为最大权限-1

DROP PROCEDURE IF EXISTS procConvertCuserDeviceCapPrivi $$
CREATE PROCEDURE procConvertCuserDeviceCapPrivi()
BEGIN
    DECLARE done INT DEFAULT 0;
    declare varVidsrcNum int;
    declare varVidEncNum int;
    declare varCUserNO varchar(32);
    declare varCU2DeviceNO varchar(32);
    declare varIntTmp int;
    
    DECLARE cur CURSOR FOR select CUserNO,CU2DeviceNO
    from KDM3ADB.CUser2DeviceInfo inner join tblCuser inner join tblDevice
    on KDM3ADB.CUser2DeviceInfo.CUserNO=tblCuser.cuserId and KDM3ADB.CUser2DeviceInfo.CU2DeviceNO=tblDevice.devId
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
            insert ignore into tblCuser2DeviceCap values(varCUserNO,varCU2DeviceNO,1,varIntTmp,-1);
            
            set varIntTmp=varIntTmp+1;
        end while;
        
        set varIntTmp=0;
        while varIntTmp < varVidEncNum do
            insert ignore into tblCuser2DeviceCap values(varCUserNO,varCU2DeviceNO,1,varIntTmp,-1);
            
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


