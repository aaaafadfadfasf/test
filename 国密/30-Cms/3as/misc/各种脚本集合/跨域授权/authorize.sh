#!/bin/bash
###############################################################################
#                 usage: authorize.sh datapath/data.tar | data.tar                               #
###############################################################################

CurDir=$(cd $(dirname $0); pwd)

# 临时文件目录
TmpDir=/tmp

if [ $# -ne 1 ]; then
    echo "usage: $0  datapath/data.tar | data.tar"
    exit
fi

if [ ! -f $1 ]; then
    echo "$1 is not exist"
    exit
fi


# 解包
tar -xf $1 -C $TmpDir

SQL="mysql -uroot -pkdc KDMAAA"

# 设备表tblDevice
if [ -f $TmpDir/tblDevice.csv ]; then
    $SQL -e "load data infile '$TmpDir/tblDevice.csv' ignore into table tblDevice "
	rm -f $TmpDir/tblDevice.csv
fi

# 设备型号表tblDeviceModel
if [ -f $TmpDir/tblDeviceModel.csv ]; then
    $SQL -e "load data infile '$TmpDir/tblDeviceModel.csv' ignore into table tblDeviceModel "
	rm -f $TmpDir/tblDeviceModel.csv
fi

# 设备划归用户表tblCuser2Dev（需要将导入文件中的用户id更新为本级平台的admin用户id）
if [ -f $TmpDir/tblCuser2Dev.csv ]; then
    $SQL -e "load data infile '$TmpDir/tblCuser2Dev.csv' ignore into table tblCuser2Dev "
	$SQL -e "update tblCuser2Dev as dev, tblCuser as usr set dev.cuserId = usr.cuserId where dev.cuserId = 'abcdefghij1234567890' and usr.cuserType = 0 "
	rm -f $TmpDir/tblCuser2Dev.csv
fi

# 设备能力划归用户表tblCuser2DeviceCap（同上）
if [ -f $TmpDir/tblCuser2DeviceCap.csv ]; then
    $SQL -e "load data infile '$TmpDir/tblCuser2DeviceCap.csv' ignore into table tblCuser2DeviceCap "
	$SQL -e "update tblCuser2DeviceCap as cap, tblCuser as usr set cap.cuserId = usr.cuserId where cap.cuserId = 'abcdefghij1234567890' and usr.cuserType = 0 "
	rm -f $TmpDir/tblCuser2DeviceCap.csv
fi

# 设备划归设备组表tblDevGrp2Dev
if [ -f $TmpDir/tblDevGrp2Dev.csv ]; then
    $SQL -e "load data infile '$TmpDir/tblDevGrp2Dev.csv' ignore into table tblDevGrp2Dev "
	rm -f $TmpDir/tblDevGrp2Dev.csv
fi

# 设备组表tblDevGroup（一个平台只可能存在着一个根组）
if [ -f $TmpDir/tblDevGroup.csv ]; then
    $SQL -e "load data infile '$TmpDir/tblDevGroup.csv' ignore into table tblDevGroup "
	$SQL -e "update tblDevGroup as grp1, tblDevGroup as grp2 set grp1.parentDevgroupId = grp2.devgroupId where grp1.parentDevgroupId = 'abcdefghij1234567890' and grp2.parentDevgroupId = '' "
	rm -f $TmpDir/tblDevGroup.csv
fi

# 视频源别名表tblDeviceCapIndexName
if [ -f $TmpDir/tblDeviceCapIndexName.csv ]; then
    $SQL -e "load data infile '$TmpDir/tblDeviceCapIndexName.csv' ignore into table tblDeviceCapIndexName "
	rm -f $TmpDir/tblDeviceCapIndexName.csv
fi

# 域关系表tblDomainRelation
if [ -f $TmpDir/tblDomainRelation.csv ]; then
    $SQL -e "load data infile '$TmpDir/tblDomainRelation.csv' ignore into table tblDomainRelation "
	rm -f $TmpDir/tblDomainRelation.csv
fi

# 导入所有数据后，需要更新对应的版本信息，否则cu可能不会获取最新的数据
CurrentDate=`date +"%Y-%m-%d"`
CurrentTime=`date +"%H-%M-%S"`
#使用默认的时区（即东八区）
DevTreeVer="${CurrentDate}T${CurrentTime}+08:00"
$SQL -e "update tblCuser set devTreeVer = '"$DevTreeVer"'"

# 所有数据导入完毕，提示需要重启平台
echo "finish completely, press any key to reboot!"
read -n 1
reboot


