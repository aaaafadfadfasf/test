#!/bin/bash
###############################################################################
# 导出所有国标设备，用于填补设备经纬度信息 #  
# usage: exportgbdev.sh exportgblist | all         #
###############################################################################

#判断运行参数是否正确
if [ $# -ne 1 ]; then
    echo "usage: $0 exportgblist | all"
    exit
else
	if [ ! -f $1 ]; then
		if [ "$1" != "all" ]; then
			echo "usage: $0 exportgblist | all"
			exit
		fi
	fi
fi

# 取export.sh的所在目录作为最终的导出目录
SaveDir=$(cd $(dirname $0); pwd)

# 临时文件目录
TmpDir=/tmp

# 脚本执行时的进程PID
PID=$$
ParentFile="ParentFile"
NameFile="NameFile"
GroupFile="GroupFile"

# 获取通道类型
localgbua=`/opt/kdm/system/inifile_tool --get /opt/kdm/gbs/conf/gbscfg.ini GBINFO GBS_GBDOMAINID`

# 获取所有需要导出的国标域列表
if [ -f $1 ]; then
	dos2unix $1 > /dev/null
    while read Line
    do
        if [ -z $Line ]; then
            continue
        fi
        if [ -z "$GbuaList" ]; then
			if [ "$localgbua" = "$Line" ]; then
				GbuaList+="''"
			else
				GbuaList="'$Line'"
			fi
        else
			if [ "$localgbua" = "$Line" ]; then
				GbuaList+=", ''"
			else
				GbuaList+=", '$Line'"
			fi
        fi
    done < $1
fi
#echo $GbuaList

#创建保存目录
TimeDir="GbDevExport-$(date +"%Y-%m-%d-%H%M%S")"
TmpDataDir="$TmpDir/$TimeDir"
test ! -d $TmpDataDir&&mkdir -m 777 $TmpDataDir


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
#echo $channelTypes

# 获取国标设备类型
GbdevTypes=`/opt/kdm/system/inifile_tool --get /opt/kdm/gbs/conf/gbscfg.ini GBINFO EXTERNAL_ENCODE_DEV_TYPE`
Types=`echo $GbdevTypes | sed "s/|/ /g"`
GbdevTypes=""
for type in $Types
do
	GbdevTypes=${GbdevTypes}"'"${type}"',"
done
len=${#GbdevTypes}-1
GbdevTypes=${GbdevTypes:0:$len} 
#echo $GbdevTypes

#导出设备列表
SQL="mysql -uroot -pkdc --default-character-set=utf8 GBS"
TmpTblGbua="tblGbua.csv";
TmpTblGbDevice="tblGbDevice.csv";
#OutTblGbDevice="allGbDevice.csv";


rm -f $TmpDir/$TmpTblGbua
if [ -z "$GbuaList" ]; then
	#查找所有所有国标域，包括本级
    $SQL -e "SELECT distinct gbua into outfile '$TmpDir/$TmpTblGbua' FROM tblGbDevice where (SUBSTRING(gbua,11,3) in('200',''))"
else
	#查找所有指定国标域
	$SQL -e "SELECT distinct gbua into outfile '$TmpDir/$TmpTblGbua' FROM tblGbDevice where gbua in($GbuaList)"
fi

while read gbualine
do
	vargbua=`echo $gbualine`
	
	rm -f $TmpDir/$TmpTblGbDevice
	rm -f $TmpDir/$GroupFile
	
	# 设备表tblGbDevice
	if [ -z $vargbua ]; then
		OutTblGbDevice="GbDevice_local.csv"
		echo "export gbua:local"

		$SQL -e "SELECT gbid,kdid,name,kdChannelIndex,parentID,longitude,latitude into outfile '$TmpDir/$TmpTblGbDevice' FROM tblGbDevice where (gbua='$vargbua'  or SUBSTRING(gbua,11,3) in($GbdevTypes)) and (SUBSTRING(gbid,11,3) in($channelTypes)) "
	else
		OutTblGbDevice="GbDevice_"${vargbua}".csv"
		echo "export gbua:"$vargbua
		$SQL -e "SELECT gbid,kdid,name,kdChannelIndex,parentID,longitude,latitude,ipAddress into outfile '$TmpDir/$TmpTblGbDevice' FROM tblGbDevice where gbua='$vargbua' and (SUBSTRING(gbid,11,3) in($channelTypes)) "
	fi


	total=$(awk 'END{print NR}' $TmpDir/$TmpTblGbDevice)
	b=''
	i=1
	p=0
	lp=0
	while read line
	do
		let "p=i*100/total"
		if [ $lp -ne $p ]; then
			printf "progress:[%-50s]%d%%\r" $b $p
			b=#$b
			lp=$p
			if [ 100 -eq $p ]; then
				printf "\n"
			fi
		fi
		
		varid=`echo $line | awk '{print $5}' | sed "s/\// /g" |awk '{print $2}'`
		grpidlist=$varid
		grpnamelist=""
		
		while [ -n "$($SQL -e "select parentID from tblGbDevice where gbid = '$varid'" > $TmpDir/$ParentFile && sed -n '2p' $TmpDir/$ParentFile ; rm -f $TmpDir/$ParentFile)" ]
		do
			#获取本级名称
			$SQL -e "select name into outfile '$TmpDir/$NameFile' from tblGbDevice where gbid = '$varid'"
			CurGrpName=$(awk '{print $1}' $TmpDir/$NameFile)
			grpnamelist=${CurGrpName}"/"${grpnamelist}
			rm -f $TmpDir/$NameFile
		
			$SQL -e "select parentID into outfile '$TmpDir/$PID' from tblGbDevice where gbid = '$varid'"
			grpId=$(awk '{print $1}' $TmpDir/$PID)
			grpidlist=${grpId}"/"${grpidlist}
			varid=$(awk '{print $1}' $TmpDir/$PID)

			rm -f $TmpDir/$PID
			if [ -z "$grpId" ]; then
				break
			fi
		done
		#echo $grpidlist"	"$grpnamelist
		echo $grpnamelist >> $TmpDir/$GroupFile
		
		let "i+=1"
	done < "$TmpDir/$TmpTblGbDevice";

	echo '设备所在组,国标编号,科达编号,经度,纬度,设备通道,设备IP,设备名' >$TmpDir/$OutTblGbDevice
	paste $TmpDir/$TmpTblGbDevice $TmpDir/$GroupFile | awk '{print $9",#"$1","$2","$6","$7","$4","$8","$3}' |sort >>$TmpDir/$OutTblGbDevice

	#编码格式转换
	iconv -f UTF-8 -t GBK $TmpDir/$OutTblGbDevice -o $TmpDir/$OutTblGbDevice
	
	mv $TmpDir/$OutTblGbDevice $TmpDataDir
done < "$TmpDir/$TmpTblGbua";

cd $TmpDir > /dev/null

exportDataFile=${TimeDir}".tar"
tar -cf $exportDataFile $TimeDir

if [ "$TmpDataDir" != "$SaveDir" ]; then
    mv $TmpDir/$exportDataFile $SaveDir
fi

rm -rf $TmpDataDir
rm -f $TmpDir/$TmpTblGbDevice
rm -f $TmpDir/$GroupFile
rm -f $TmpDir/$TmpTblGbua

echo "all device's info have exported to file[$SaveDir/$exportDataFile]"



