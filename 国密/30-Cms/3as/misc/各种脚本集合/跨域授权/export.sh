#!/bin/bash
###############################################################################
#                 usage: export.sh FileName | DeviceId                        #
###############################################################################

# 取export.sh的所在目录作为最终的导出目录
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

SQL="mysql -uroot -pkdc KDMAAA"
TmpTblDevice="tblDevice.csv";
TmpTblDeviceModel="tblDeviceModel.csv";
TmpTblCuser2Dev="tblCuser2Dev.csv";
TmpTblCuser2DeviceCap="tblCuser2DeviceCap.csv";
TmpTblDevGrp2Dev="tblDevGrp2Dev.csv";
TmpTblDevGroup="tblDevGroup.csv";
TmpTblDeviceCapIndexName="tblDeviceCapIndexName.csv";
TmpTblDomainRelation="tblDomainRelation.csv";

# 检测不合法的设备ID，并提示用户
HasInvalid=0
function fCheckInvalid()
{  
    if [ $($SQL -e "select count(*) from tblDevice where devId='$1' and domainId='$LocalDomainId'" > $TmpDataDir/$PID && sed -n '2p' $TmpDataDir/$PID && rm -f $TmpDataDir/$PID) -ne 1 ]; then
        echo "Invalid deviceId: $1 "
        HasInvalid=1
   fi
}

# 获取所有需要授权的设备列表
if [ -f $1 ]; then
	dos2unix $1
    while read Line
    do
        if [ -z $Line ]; then
            continue
        fi
        fCheckInvalid $Line
        if [ -z "$DeviceList" ]; then
            DeviceList="'$Line'"
        else
            DeviceList+=", '$Line'"
        fi
    done < $1
else
    DeviceList="'$1'"
    fCheckInvalid $1
fi

if [ $HasInvalid -eq 1 ]; then
    echo "Has invalid device ids,please check the device ids."
    exit
fi

#创建保存目录
TimeDir="export-$(date +"%Y-%m-%d-%H%M%S")"
TmpDataDir="$TmpDir/$TimeDir"
test ! -d $TmpDataDir&&mkdir -m 777 $TmpDataDir

# 设备表tblDevice
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDevice' from tblDevice where devId in ($DeviceList) "

# 设备型号表tblDeviceModel
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDeviceModel' from tblDeviceModel model where exists (select * from tblDevice dev where dev.devId in ($DeviceList) and dev.devModelId = model.modelId) "

# 设备划归用户表tblCuser2Dev（授权时直接将设备授权给admin用户，故只需导出admin用户的划归信息，且不需要原admin用户的id）
$SQL -e "select 'abcdefghij1234567890', dev.devId, dev.devPrivilege into outfile '$TmpDataDir/$TmpTblCuser2Dev' from tblCuser2Dev dev where dev.devId in ($DeviceList) and exists (select * from tblCuser usr where usr.cuserType = 0 and usr.cuserId = dev.cuserId) "

# 设备能力划归用户表tblCuser2DeviceCap（同上）
$SQL -e "select 'abcdefghij1234567890', cap.devId, cap.devCapId, cap.devCapIndex, cap.devCapIndexPrivilege into outfile '$TmpDataDir/$TmpTblCuser2DeviceCap' from tblCuser2DeviceCap cap where cap.devId in ($DeviceList) and exists (select * from tblCuser usr where usr.cuserType = 0 and usr.cuserId = cap.cuserId) "

# 设备划归设备组表tblDevGrp2Dev
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDevGrp2Dev' from tblDevGrp2Dev where devId in ($DeviceList) "

# 设备组表tblDevGroup（只需要导出包含有授权设备的设备组，需要递归进行树查询）
$SQL -e "create table tblTmpGroupId(depth int(5), groupId varchar(64)) "
Depth=0
$SQL -e "insert into tblTmpGroupId select $Depth, devgroupId from tblDevGrp2Dev where devId in ($DeviceList) "
while [ $($SQL -e "select count(*) from tblTmpGroupId where depth = $Depth" > $TmpDataDir/$PID && sed -n '2p' $TmpDataDir/$PID ; rm -f $TmpDataDir/$PID) -ne 0 ]
do
    $SQL -e "insert into tblTmpGroupId select $(expr $Depth + 1), grp.parentDevgroupId from tblDevGroup grp where exists (select * from tblTmpGroupId tmp where tmp.groupId = grp.devgroupId and tmp.Depth = $Depth) "
    Depth=$(expr $Depth + 1)
done

$SQL -e "select devgroupId, domainId, case parentDevgroupId when '' then 'abcdefghij1234567890' else parentDevgroupId end, groupName, groupDesc, createDate, creater, owner into outfile '$TmpDataDir/$TmpTblDevGroup' from tblDevGroup where exists(select * from tblTmpGroupId where groupId = devgroupId) "
$SQL -e "drop table tblTmpGroupId"

# 视频源别名表tblDeviceCapIndexName
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDeviceCapIndexName' from tblDeviceCapIndexName where deviceId in ($DeviceList) "

# 域关系表tblDomainRelation
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDomainRelation' from tblDomainRelation where domainId='$LocalDomainId'"

# 生成导出数据包，并清理临时文件
cd $TmpDataDir > /dev/null
tar -cf data.tar $TmpTblDevice $TmpTblDeviceModel $TmpTblCuser2Dev $TmpTblCuser2DeviceCap $TmpTblDevGrp2Dev $TmpTblDevGroup $TmpTblDeviceCapIndexName $TmpTblDomainRelation
rm -f $TmpTblDevice $TmpTblDeviceModel $TmpTblCuser2Dev $TmpTblCuser2DeviceCap $TmpTblDevGrp2Dev $TmpTblDevGroup $TmpTblDeviceCapIndexName $TmpTblDomainRelation

if [ "$TmpDataDir" != "$SaveDir/$TimeDir" ]; then
    mv $TmpDataDir $SaveDir
fi

echo "all device's info have exported to file[$SaveDir/$TimeDir/data.tar]"

