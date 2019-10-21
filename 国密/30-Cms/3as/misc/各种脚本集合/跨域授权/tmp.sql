
select * from tblDevice;

select * from tblCuser2Dev where devId = 'aaa' and exists (select * from tblCuser usr where cuserId = usr.cuserId and cuserType = 0)


select * from tblDevGroup;

select * from tblDevGrp2Dev;

select * from tblDeviceCapIndexName;












