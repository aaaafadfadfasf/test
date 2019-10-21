#!/bin/bash
###############################################################################
# 清理残留国标设备 #  
# usage: cleangbdevbyua.sh   gbua       #
###############################################################################
if [ $# -ne 1 ]; then
    echo "usage: $0 gbua"
    exit
fi

gbua=$1

# 取export.sh的所在目录作为最终的导出目录
SaveDir=$(cd $(dirname $0); pwd)

# 临时文件目录
TmpDir=/tmp

# 脚本执行时的进程PID
PID=$$
ParentFile="ParentFile"

#创建保存目录
TimeDir="GbDevClean-$(date +"%Y-%m-%d-%H%M%S")"
CleanDataDir="$SaveDir/$TimeDir"
test ! -d $CleanDataDir&&mkdir -m 777 $CleanDataDir

sip_proxyIp=`/opt/kdm/system/xmlfile_tool --get /etc/kdm/commonconfig.ini commonconfig  sip_proxy ip`
localIp=`/opt/kdm/system/xmlfile_tool --get /etc/kdm/commonconfig.ini commonconfig  local ip`
domainId=`/opt/kdm/system/xmlfile_tool --get /etc/kdm/commonconfig.ini commonconfig  domain uuid`
echo $sip_proxyIp
echo $localIp 

#导出设备列表
SQL="mysql -h $localIp -uroot -pkdc --default-character-set=utf8 GBS"
SQL3A="mysql -h $sip_proxyIp -uroot -pkdc --default-character-set=utf8 KDMAAA"


#创建临时表
$SQL -e "create table tblTmpGbDev(gbid char(20),kdid char(32),kdDomainId char(32),kdChannelIndex int,gbua char(20),name varchar(64),owner varchar(64),civilCode varchar(64),parentID  varchar(512),primary key (gbid))"
$SQL -e "insert into tblTmpGbDev select gbid,kdid,kdDomainId,kdChannelIndex,gbua,name,owner,civilCode,parentID from tblGbDevice where gbua='$gbua' and gbid<>'$gbua' "

OutTblGbDevice="CleanGbDevice.csv"
$SQL -e "select *  into outfile '$CleanDataDir/$OutTblGbDevice' from tblTmpGbDev"; 

$SQL3A < ./tasdeldev.sql

#删除GBS中的数据
$SQL -e "delete from tblGbDevice where gbua='$gbua' and gbid<>'$gbua'";

#遍历删除KDMAAA中的数据
CleanGbDevice="CleanGbDevice.csv"
while read line
do
	varkdid=`echo $line | awk '{print $2}'`
	vargbid=`echo $line | awk '{print $1}'`	
	echo "delete:$vargbid kdid:$varkdid"
	
	if [ $($SQL3A -e "select count(*) from tblDevice where devId='$varkdid' and domainId='$domainId' and creater=3" > $TmpDir/$PID && sed -n '2p' $TmpDir/$PID ; rm -f $TmpDir/$PID) -ne 0 ];then		
		#KDMAAA-------------------------------------------------------------------
		$SQL3A -e "call ProcDelDevTmp('$varkdid')"
		#KDMAAA---------------------------------------END----------------------------
	fi
done < "$CleanDataDir/$OutTblGbDevice";

# 更新对应的版本信息，
CurrentDate=`date +"%Y-%m-%d"`
CurrentTime=`date +"%H-%M-%S"`
#使用默认的时区（即东八区）
DevTreeVer="${CurrentDate}T${CurrentTime}+08:00"
$SQL3A -e "update tblCuser set devTreeVer = '"$DevTreeVer"'"
$SQL3A -e "update tblCusergroup set devTreeVer = '"$DevTreeVer"'"

$SQL -e "truncate table tblTmpGbDev"
	
#删除临时表
$SQL -e "drop table tblTmpGbDev"
 
exportDataFile=${TimeDir}".tar"
tar -cf $exportDataFile $TimeDir
rm -rf $TimeDir

echo "all device's info need to clean exported to file[$CleanDataDir]"



