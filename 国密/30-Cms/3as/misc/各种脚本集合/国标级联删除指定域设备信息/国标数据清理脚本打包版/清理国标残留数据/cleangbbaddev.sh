#!/bin/bash
###############################################################################
# 清理残留国标设备 #  
# usage: cleangbbaddev.sh          #
###############################################################################

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


# 获取通道类型
channelTypes=`/opt/kdm/system/inifile_tool --get /opt/kdm/gbs/conf/gbscfg.ini GBINFO EXTERNAL_ENCODE_CHANNEL_TYPE`
Types=`echo $channelTypes | sed "s/|/ /g"`
channelTypes=""
for type in $Types
do
	channelTypes=${channelTypes}"'"${type}"',"
done
len=${#channelTypes}-1
channelTypes=${channelTypes:0:$len} 
echo $channelTypes

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
$SQL -e "insert into tblTmpGbDev select gbid,kdid,kdDomainId,kdChannelIndex,gbua,name,owner,civilCode,parentID from tblGbDevice where SUBSTRING(gbua,11,3)!='200' and kdDomainId='$domainId' and (SUBSTRING(gbid,11,3) in($channelTypes))"

tmptblGbDevice="tmptblGbDevice.csv"
rm -f $tmptblGbDevice
$SQL -e "select gbid,kdid,kdDomainId,kdChannelIndex,gbua,name,owner,civilCode,parentID  into outfile '$TmpDir/$tmptblGbDevice' from tblTmpGbDev"; 

$SQL3A < ./tasdeldev.sql
CleanGbDevice="CleanGbDevice.csv"
while read line
do
	varkdid=`echo $line | awk '{print $2}'`
	vargbid=`echo $line | awk '{print $1}'`	
	echo "delete:$vargbid kdid:$varkdid"
	
	if [ $($SQL3A -e "select count(*) from tblDevice where devId='$varkdid' and domainId='$domainId' and creater=3" > $TmpDir/$PID && sed -n '2p' $TmpDir/$PID ; rm -f $TmpDir/$PID) -ne 0 ];then
		#删除GBS中的数据
		$SQL -e "delete from tblGbDevice where gbid='$vargbid'";
		
		#KDMAAA-------------------------------------------------------------------
		$SQL3A -e "call ProcDelDevTmp('$varkdid')"
		echo $line>>$CleanDataDir/$CleanGbDevice
		#KDMAAA---------------------------------------END----------------------------
	fi
done < "$TmpDir/$tmptblGbDevice";

rm -f $tmptblGbDevice

$SQL -e "truncate table tblTmpGbDev"
	
#删除临时表
$SQL -e "drop table tblTmpGbDev"
 
exportDataFile=${TimeDir}".tar"
tar -cf $exportDataFile $TimeDir
rm -rf $TimeDir

echo "all device's info need to clean exported to file[$CleanDataDir]"



