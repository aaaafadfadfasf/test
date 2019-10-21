delimiter $$

use KDMAAA $$

DROP PROCEDURE IF EXISTS procGetDevModelId4Encoder $$
CREATE PROCEDURE procGetDevModelId4Encoder(varVidSrcNum int, varLicense int, varLocalStorage int, varAlarmInput int, varEncodeChnNum int, varDevLabel int ,varManufacturer char(32),varGbLabel int, out varModelId char(32))
BEGIN
    
    -- modelId                          | vidsrc | license | localstorage | alarmInput | chn | devLabel
    -- select t3.*, e.capNum as devLabel from
	
    set varModelId='';
    
	if varGbLabel = 1 then
	 select t4.modelId into varModelId from
	   (select t3.*, e.capNum as gbLable from
		 (select t.*, d.capNum as chn from
			  (select a.modelId,a.capNum as vidsrc, a.licenseNum as license, b.capNum as localstorage from tblDeviceModel a inner join tblDeviceModel b
				 on a.modelId=b.modelId and a.deviceTypeId=1 and a.devCapId=1 and b.devCapId=2 and a.manufacturer=varManufacturer) as t
			  inner join tblDeviceModel as d on t.modelId=d.modelId and d.devCapId=4) as t3
			  inner join tblDeviceModel as e on t3.modelId=e.modelId and e.devCapId=14) as t4
			  inner join tblDeviceModel as f on t4.modelId=f.modelId and f.devCapId=varDevLabel
		where vidsrc=varVidSrcNum and license=varLicense and localstorage=varLocalStorage and chn=varEncodeChnNum and gbLable=varGbLabel
			  
		limit 1;
	else
        select t3.modelId into varModelId from
		(select t2.*, d.capNum as chn from
		   (select t.*,c.capNum as alarmInput from
			  (select a.modelId,a.capNum as vidsrc, a.licenseNum as license, b.capNum as localstorage from tblDeviceModel a inner join tblDeviceModel b
				 on a.modelId=b.modelId and a.deviceTypeId=1 and a.devCapId=1 and b.devCapId=2 and a.manufacturer=varManufacturer) as t
			  inner join tblDeviceModel as c on t.modelId=c.modelId and c.devCapId=3) as t2
			  inner join tblDeviceModel as d on t2.modelId=d.modelId and d.devCapId=4) as t3
			  inner join tblDeviceModel as e on t3.modelId=e.modelId and e.devCapId=varDevLabel
		where vidsrc=varVidSrcNum and license=varLicense and localstorage=varLocalStorage and alarmInput=varAlarmInput and chn=varEncodeChnNum
			  and not exists(select 1 from tblDeviceModel t4 where t4.modelId=t3.modelId and t4.devCapId=14 and t4.capNum > 0)
		limit 1;
    end if;
END$$


DROP PROCEDURE IF EXISTS procGetDevModelId4Decoder $$
CREATE PROCEDURE procGetDevModelId4Decoder(varDecodeChannel int, out varModelId char(32))
BEGIN

    set varModelId='';
    
    select modelId into varModelId 
    from tblDeviceModel where deviceTypeId=2 and devCapId=5 and capNum=varDecodeChannel and manufacturer='kedacom'
    limit 1;
    
END$$

DELIMITER ;

