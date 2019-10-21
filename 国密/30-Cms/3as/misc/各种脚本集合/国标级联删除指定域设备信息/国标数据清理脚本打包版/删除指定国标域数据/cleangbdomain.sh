#!/bin/bash
###############################################################################
# 清理指定国标域设备 #  
# usage: cleangbdev.sh cleangblist          #
###############################################################################

#判断运行参数是否正确
if [ $# -ne 1 ]; then
    echo "usage: $0 cleangblist "
    exit
else
	if [ ! -f $1 ]; then
		echo "$1 not file gbid list"
		exit
	fi
fi

# 取export.sh的所在目录作为最终的导出目录
SaveDir=$(cd $(dirname $0); pwd)

# 临时文件目录
TmpDir=/tmp

# 脚本执行时的进程PID
PID=$$
ParentFile="ParentFile"

# 获取所有需要清理的国标域列表
dos2unix $1 > /dev/null


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
echo $sip_proxyIp
echo $localIp

#导出设备列表
SQL="mysql -h $localIp -uroot -pkdc --default-character-set=utf8 GBS"
SQL3A="mysql -h $sip_proxyIp -uroot -pkdc --default-character-set=utf8 KDMAAA"


#创建临时表
$SQL -e "create table tblTmpGbDev(depth int(5), gbid char(20),kdid char(32),kdDomainId char(32),kdChannelIndex int,gbua char(20),name varchar(64),owner varchar(64),civilCode varchar(64),parentID  varchar(512),primary key (gbid))"

$SQL3A < ./tasdeldev.sql

#查找数据
while read gbualine
do
	vargbua=`echo $gbualine`
	if [ ${#vargbua} -ne 20 ];then
        echo "$vargbua bad gbua length!"
		continue
	fi
	if [ "${vargbua:10:3}" != "200" ];then
		echo "$vargbua is not a gbua!"
		continue
	fi
	
	OutTblGbDevice="GbDevice_"${vargbua}".csv"
	echo "clean gbua:"$vargbua
	
	# 递归进行树查询
	Depth=0
	$SQL -e "insert into tblTmpGbDev select $Depth, gbid,kdid,kdDomainId,kdChannelIndex,gbua,name,owner,civilCode,parentID from tblGbDevice where gbid='$vargbua'"
	while [ $($SQL -e "select count(*) from tblTmpGbDev where depth = $Depth" > $TmpDir/$PID && sed -n '2p' $TmpDir/$PID ; rm -f $TmpDir/$PID) -ne 0 ]
	do
		$SQL -e "SELECT * into outfile '$TmpDir/$ParentFile' FROM tblTmpGbDev where depth = $Depth"
		while read line
		do
			vargbid=`echo $line | awk '{print $2}'`
			type=${vargbid:10:3}
			if [[ ! $channelTypes == *$type* ]];then
				$SQL -e "insert into tblTmpGbDev select $(expr $Depth + 1),gbid,kdid,kdDomainId,kdChannelIndex,gbua,name,owner,civilCode,parentID from tblGbDevice 
		        where parentID LIKE '%$vargbid%' or owner LIKE '%$vargbid%' and not exists (select * from tblTmpGbDev tmp where tmp.gbid=gbid)"
			fi
		done < "$TmpDir/$ParentFile";
		Depth=$(expr $Depth + 1)
		rm -f $TmpDir/$ParentFile
	done
	
	#删除行政区划：匹配UA及前缀
     civillen=8
     varcivilcode=${vargbua:0:$civillen}
     while [ "${varcivilcode:$civillen-2:2}" = "00" ]
    do
        civillen=${#varcivilcode}-2
        varcivilcode=${varcivilcode:0:$civillen}
    done
	echo $varcivilcode
	
	DepthUa=100
	$SQL -e "insert into tblTmpGbDev select $DepthUa,gbid,kdid,kdDomainId,kdChannelIndex,gbua,name,owner,civilCode,parentID from tblGbDevice where LENGTH(gbid)<10 and gbid like '$varcivilcode%' and gbua=(select gbua from tblGbDevice b where b.gbid='$vargbua') and not exists (select * from tblTmpGbDev tmp where tmp.gbid=gbid) ";
	
	$SQL -e "select gbid,kdid,kdDomainId,kdChannelIndex,gbua,name,owner,civilCode,parentID  into outfile '$CleanDataDir/$OutTblGbDevice' from tblTmpGbDev"; 

	while read line
	do
		varkdid=`echo $line | awk '{print $2}'`
		vargbid=`echo $line | awk '{print $1}'`	
		echo "delete:$vargbid kdid:$varkdid"
		#删除GBS中的数据
		$SQL -e "delete from tblGbDevice where gbid='$vargbid'";
		
		#KDMAAA-------------------------------------------------------------------
		type=${vargbid:10:3}
		if [[ "$type"!="200" && ${#vargbid} -eq 20 ]];then
			if [[ ! $channelTypes == *$type* ]];then
				$SQL3A -e "call ProcDelDevTmp('$varkdid')"
		else
			# 删除设备划归设备组表tblDeviceAutoAssign数据
			$SQL3A -e "delete from tblDeviceAutoAssign where groupId='$varkdid'"
					
			# 删除设备组表tblDevGroup数据
			$SQL3A -e "delete from tblDevGroup where devgroupId='$varkdid'"
		fi
		#KDMAAA---------------------------------------END----------------------------
		fi
	done < "$CleanDataDir/$OutTblGbDevice";
		
	$SQL -e "truncate table tblTmpGbDev"
	
 done < $1;
 
 # 更新对应的版本信息，
CurrentDate=`date +"%Y-%m-%d"`
CurrentTime=`date +"%H-%M-%S"`
#使用默认的时区（即东八区）
DevTreeVer="${CurrentDate}T${CurrentTime}+08:00"
$SQL3A -e "update tblCuser set devTreeVer = '"$DevTreeVer"'"
$SQL3A -e "update tblCusergroup set devTreeVer = '"$DevTreeVer"'"

#删除临时表
$SQL -e "drop table tblTmpGbDev"
 
exportDataFile=${TimeDir}".tar"
tar -cf $exportDataFile $TimeDir
rm -rf $TimeDir

echo "all device's info need to clean exported to file[$CleanDataDir]"



