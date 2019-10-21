
DROP VIEW IF EXISTS vDevInfo;
CREATE VIEW vDevInfo AS
select ifnull(ExtractValue(dev.devData,"TDevExtData/extData//key[text()='kdmNO']/../value"),'') as puid,gbdev.kdChannelIndex as channnelid, gbdev.latitude as lat,gbdev.longitude as lon 
from tblDevice dev,GBS.tblGbDevice gbdev
where dev.devId=gbdev.kdid  and gbdev.kdChannelIndex<>-1;

