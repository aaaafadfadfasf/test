#!/bin/bash
###############################################################################
#                 usage: cancel.sh FileName | DeviceId                        #
###############################################################################
# cancel.sh的所在目录作为备份数据输出目录
SaveDir=$(cd $(dirname $0); pwd)

# 临时文件目录
TmpDir=/tmp


# 脚本执行时的进程PID
PID=$$

# 获取本域域名（由于存在跨域授权，无法直接的从数据库表中判断出本域信息）
LocalDomainId=`/opt/kdm/system/xmlfile_tool --get /etc/kdm/commonconfig.ini commonconfig domain uuid`

if [ $# -ne 1 ]; then
    echo "usage: $0 FileName | DeviceId"
    exit
fi

# 获取所有需要取消授权的设备列表
if [ -f $1 ]; then
    while read Line
    do
	    if [ -z $Line ]; then
		    continue
		fi
		
        if [ -z "$DeviceList" ]; then
            DeviceList="'$Line'"
        else
            DeviceList+=", '$Line'"
        fi
    done < $1
else
    DeviceList="'$1'"
fi

SQL="mysql -uroot -pkdc KDMAAA"
TmpTblDevice="tblDevice.csv";
TmpTblDeviceModel="tblDeviceModel.csv";
TmpTblCuser2Dev="tblCuser2Dev.csv";
TmpTblCuser2DeviceCap="tblCuser2DeviceCap.csv";
TmpTblDevGrp2Dev="tblDevGrp2Dev.csv";
TmpTblDevGroup="tblDevGroup.csv";
TmpTblDeviceCapIndexName="tblDeviceCapIndexName.csv";
TmpTblDomainRelation="tblDomainRelation.csv";

#-------------------------------------------------------------------------------------------------------------------------
# 查询本域及其下级域域关系列表
$SQL -e "create table tblTmpDomainId(depth int(5), domainId varchar(64)) "
Depth=0
$SQL -e "insert into tblTmpDomainId select $Depth, domainId from tblDomainRelation where domainId = '$LocalDomainId' "
while [ $($SQL -e "select count(*) from tblTmpDomainId where depth = $Depth" > $TmpDir/$PID && sed -n '2p' $TmpDir/$PID && rm -f $TmpDir/$PID) -ne 0 ]
do
    $SQL -e "insert into tblTmpDomainId select $(expr $Depth + 1), domain.domainId from tblDomainRelation domain where exists (select * from tblTmpDomainId tmp where tmp.domainId = domain.parentDomainId and tmp.Depth = $Depth) "
    Depth=$(expr $Depth + 1)
done

ExceptNum=`$SQL -e "select devId from tblDevice dev where dev.devId in ($DeviceList) and exists (select * from tblTmpDomainId tmp where tmp.domainId = dev.domainId) " > $TmpDir/$PID && wc -l $TmpDir/$PID | awk '{print $1}' `
$SQL -e "drop table tblTmpDomainId"

if [ $ExceptNum -ne 0 ]; then
	echo "there are some invalid device id, details in $TmpDir/$PID "
	exit
fi

rm -f $TmpDir/$PID

#-------------------------------------------------------------------------------------------------------------------------
#创建保存目录
TimeDir="cancel-$(date +"%Y-%m-%d-%H%M%S")"
TmpDataDir="$TmpDir/$TimeDir"
test ! -d $TmpDataDir&&mkdir -m 777 $TmpDataDir

#先备份将要删除的数据（注意：和删除脚本保持一致），防止误删（可以使用备份数据进行恢复）
# 备份设备型号表tblDeviceModel数据
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDeviceModel' from tblDeviceModel where exists (select * from tblDevice dev1 where dev1.devId in ($DeviceList) and dev1.devModelId = modelId) and not exists (select * from tblDevice dev2 where dev2.devId not in ($DeviceList) and dev2.devModelId = modelId) "

# 备份设备划归用户表tblCuser2Dev数据
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblCuser2Dev' from tblCuser2Dev  where devId in ($DeviceList) "

# 备份设备能力划归用户表tblCuser2DeviceCap数据
$SQL -e "select *  into outfile '$TmpDataDir/$TmpTblCuser2DeviceCap' from tblCuser2DeviceCap where devId in ($DeviceList) "

# 备份设备组表tblDevGroup数据
$SQL -e "create table tblTmpGroupId(depth int(5), groupId varchar(64)) "
Depth=0
$SQL -e "insert into tblTmpGroupId select $Depth, dev1.devgroupId from tblDevGrp2Dev dev1 where dev1.devId in ($DeviceList) "
while [ $($SQL -e "select count(*) from tblTmpGroupId where depth = $Depth" > $TmpDir/$PID && sed -n '2p' $TmpDir/$PID ; rm -f $TmpDir/$PID) -ne 0 ]
do
    $SQL -e "delete from tblTmpGroupId where depth = $Depth and exists (select * from tblDevGrp2Dev where devId not in ($DeviceList) and devgroupId = groupId) "
    $SQL -e "delete from tblTmpGroupId where groupId in (select groupId from (select * from tblTmpGroupId where depth = $Depth and exists (select * from tblDevGroup where parentDevgroupId = groupId and devgroupId not in (select groupId from tblTmpGroupId))) tmp) "
    $SQL -e "insert into tblTmpGroupId select $(expr $Depth + 1), grp.parentDevgroupId from tblDevGroup grp where exists (select * from tblTmpGroupId where groupId = grp.devgroupId and depth = $Depth) "
	
    Depth=$(expr $Depth + 1)
done

$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDevGroup' from tblDevGroup  where exists (select * from tblTmpGroupId where groupId = devgroupId) and domainId in (select domainId from tblDevice where devId in ($DeviceList)) "
$SQL -e "drop table tblTmpGroupId"

# 备份设备划归设备组表tblDevGrp2Dev数据
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDevGrp2Dev' from tblDevGrp2Dev where devId in ($DeviceList) "

# 备份视频源别名表tblDeviceCapIndexName数据
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDeviceCapIndexName' from tblDeviceCapIndexName where deviceId in ($DeviceList) "

# 备份域关系表tblDomainRelation数据
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDomainRelation' from tblDomainRelation where domainId in (select domainId from (select domainId from tblDevice dev1 where dev1.devId in ($DeviceList)) tmp where not exists (select * from tblDevice dev2 where dev2.devId not in ($DeviceList) and dev2.domainId = tmp.domainId))"

# 备份设备表tblDevice数据（必须最后删除，否则前面部分条件不满足）
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDevice' from tblDevice where devId in ($DeviceList) "

# 生成备份数据包，并清理临时文件
cd $TmpDataDir > /dev/null
tar -cf backupdata.tar $TmpTblDevice $TmpTblDeviceModel $TmpTblCuser2Dev $TmpTblCuser2DeviceCap $TmpTblDevGrp2Dev $TmpTblDevGroup $TmpTblDeviceCapIndexName $TmpTblDomainRelation
rm -f $TmpTblDevice $TmpTblDeviceModel $TmpTblCuser2Dev $TmpTblCuser2DeviceCap $TmpTblDevGrp2Dev $TmpTblDevGroup $TmpTblDeviceCapIndexName $TmpTblDomainRelation

if [ "$TmpDataDir" != "$SaveDir/$TimeDir" ]; then
    mv $TmpDataDir $SaveDir
fi

#-------------------------------------------------------------------------------------------------------------------------
# 删除设备型号表tblDeviceModel数据
$SQL -e "delete from tblDeviceModel where exists (select * from tblDevice dev1 where dev1.devId in ($DeviceList) and dev1.devModelId = modelId) and not exists (select * from tblDevice dev2 where dev2.devId not in ($DeviceList) and dev2.devModelId = modelId) "

# 删除设备划归用户表tblCuser2Dev数据
$SQL -e "delete from tblCuser2Dev where devId in ($DeviceList) "

# 删除设备能力划归用户表tblCuser2DeviceCap数据
$SQL -e "delete from tblCuser2DeviceCap where devId in ($DeviceList) "

# 删除设备组表tblDevGroup数据
$SQL -e "create table tblTmpGroupId(depth int(5), groupId varchar(64)) "
Depth=0
$SQL -e "insert into tblTmpGroupId select $Depth, dev1.devgroupId from tblDevGrp2Dev dev1 where dev1.devId in ($DeviceList) "
while [ $($SQL -e "select count(*) from tblTmpGroupId where depth = $Depth" > $TmpDir/$PID && sed -n '2p' $TmpDir/$PID ; rm -f $TmpDir/$PID) -ne 0 ]
do
    $SQL -e "delete from tblTmpGroupId where depth = $Depth and exists (select * from tblDevGrp2Dev where devId not in ($DeviceList) and devgroupId = groupId) "
    $SQL -e "delete from tblTmpGroupId where groupId in (select groupId from (select * from tblTmpGroupId where depth = $Depth and exists (select * from tblDevGroup where parentDevgroupId = groupId and devgroupId not in (select groupId from tblTmpGroupId))) tmp) "
    $SQL -e "insert into tblTmpGroupId select $(expr $Depth + 1), grp.parentDevgroupId from tblDevGroup grp where exists (select * from tblTmpGroupId where groupId = grp.devgroupId and depth = $Depth) "
	
    Depth=$(expr $Depth + 1)
done

$SQL -e "delete from tblDevGroup where exists (select * from tblTmpGroupId where groupId = devgroupId) and domainId in (select domainId from tblDevice where devId in ($DeviceList)) "
$SQL -e "drop table tblTmpGroupId"

# 删除设备划归设备组表tblDevGrp2Dev数据
$SQL -e "delete from tblDevGrp2Dev where devId in ($DeviceList) "

# 删除视频源别名表tblDeviceCapIndexName数据
$SQL -e "delete from tblDeviceCapIndexName where deviceId in ($DeviceList) "

# 删除域关系表tblDomainRelation数据
$SQL -e "delete from tblDomainRelation where domainId in (select domainId from (select domainId from tblDevice dev1 where dev1.devId in ($DeviceList)) tmp where not exists (select * from tblDevice dev2 where dev2.devId not in ($DeviceList) and dev2.domainId = tmp.domainId))"

# 删除设备表tblDevice数据（必须最后删除，否则前面部分条件不满足）
$SQL -e "delete from tblDevice where devId in ($DeviceList) "

# 删除授权设备数据后，需要更新对应的版本信息，否则cu可能不会获取最新的数据
CurrentDate=`date +"%Y-%m-%d"`
CurrentTime=`date +"%H-%M-%S"`
#使用默认的时区（即东八区）
DevTreeVer="${CurrentDate}T${CurrentTime}+08:00"
$SQL -e "update tblCuser set devTreeVer = '"$DevTreeVer"'"

# 所有数据清理完毕，提示需要重启平台
echo "all data deleted is  backup to file[$SaveDir/$TimeDir/backupdata.tar]"
echo "finish completely, press any key to reboot!"
read -n 1
reboot

