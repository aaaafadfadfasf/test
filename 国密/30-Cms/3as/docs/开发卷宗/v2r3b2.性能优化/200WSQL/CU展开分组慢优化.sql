CU展开分组慢优化：耗时2-3秒。

问题：
select count(*) from 
((select b.*, a.cuserId as cuserId, a.devPrivilege as devPrivilege 
FROM tblCuser2Dev a, tblDevice b, tblDevGrp2Dev c
where a.devId = b.devId and a.devId = c.devId and a.cuserId='f986d5f0be4243fab03c9ccf83f8a2d5'
 and c.devgroupId='cf087060f09d4a9b989215765b4955f5')
 union
 (select b.*, '' as cuserId, 0 as devPrivilege FROM tblDevice b, tblDevGrp2Dev c, tblDeviceModel d  
  where b.devId = c.devId and c.devgroupId='cf087060f09d4a9b989215765b4955f5' and b.devModelId=d.modelId 
  and d.deviceTypeId=2 and d.devCapId=5)) res
-- 数据没加载进来时1.72s，加载之后可以0.02s
  
call procDevicefullinfoAndPrivilege('f986d5f0be4243fab03c9ccf83f8a2d5', '', 'cf087060f09d4a9b989215765b4955f5','', 0, 30)  
-- 数据没加载进来时1.86s，加载之后可以0.03s

解决：
索引预加载
LOAD INDEX INTO CACHE tb_name_list...
key_buffer_size=4G

set global key_buffer_size=1024000000
load index into cache tblCuser2DeviceCap,tblCuser2Dev,tblDevGroup,tblDevGrp2Dev,tblDeviceCapIndexData,tblDevice 


hot_cache.key_buffer_size=2G
cold_cache.key_buffer_size=2G
init_file=kdm_mysqld_init.sql

7f3eba1e1e3a4641a169d3ea7fd9c420

select count(*) from 
((select b.*, a.cuserId as cuserId, a.devPrivilege as devPrivilege 
FROM tblCuser2Dev a, tblDevice b, tblDevGrp2Dev c
where a.devId = b.devId and a.devId = c.devId and a.cuserId='f986d5f0be4243fab03c9ccf83f8a2d5'
 and c.devgroupId='7f3eba1e1e3a4641a169d3ea7fd9c420')
 union
 (select b.*, '' as cuserId, 0 as devPrivilege FROM tblDevice b, tblDevGrp2Dev c, tblDeviceModel d  
  where b.devId = c.devId and c.devgroupId='7f3eba1e1e3a4641a169d3ea7fd9c420' and b.devModelId=d.modelId 
  and d.deviceTypeId=2 and d.devCapId=5)) res

call procDevicefullinfoAndPrivilege('f986d5f0be4243fab03c9ccf83f8a2d5', '', 
'cf56ea99c44b4961b15b46745cecc3f1','', 0, 30)  
