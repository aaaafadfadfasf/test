#!/bin/bash
###############################################################################
# usage:导入所有国标下级设备的经纬度信息  #       
#       importgbdev.sh gbdevimport #
###############################################################################

if [ $# -ne 1 ]; then
    echo "usage: $0 gbdevimport"
    exit
fi

if [ ! -d $1 ]; then
    echo "$1 is not exist or not a directory"
    exit
fi

# 脚本执行时的进程PID
PID=$$
TmpDir=/tmp

# 临时文件目录
CurDir=$(cd $(dirname $0); pwd)
DirFullPath=$CurDir/$1
#echo $DirFullPath

SQL="mysql -uroot -pkdc --default-character-set=utf8 GBS"
SQL3A="mysql -uroot -pkdc --default-character-set=utf8 KDMAAA"

for devfile in `ls $1`
do
    if [ -d $devfile ]
	then
        continue
	else
		devfile=$DirFullPath/$devfile
		echo "import file:"$devfile
		#编码格式转换
		iconv -f GBK -t UTF-8 $devfile -o $devfile
		
		#导入设备列表
		total=$(awk 'END{print NR}' $devfile)
		b=''
		i=1
		p=0
		lp=0
		while IFS=, read grpname gbid kdid lon lat chnum ipaddress chnName 
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
			
			#忽略第一行文字提示
			if [ 1 -eq $i ]; then
				let "i+=1"
				continue
			fi
			
			#去掉#号
			len=${#gbid}
			gbid=${gbid:1:$len} 
			
			#取设备名作为视频源名
			chnName=${chnName%_*}
			
			#echo $gbid"		"$kdid"	"$chnum"	"$lon"	"$lat"	"$chnName
		
			$SQL -e "update tblGbDevice set longitude=$lon,latitude=$lat where kdid = '$kdid' and kdChannelIndex=$chnum"
			
			#获取设备视频源数及编码通道数
			$SQL3A -e "SELECT capNum  into outfile '$TmpDir/$PID' FROM tblDeviceModel where devCapId=1 and modelId=(select devModelId from tblDevice where devId='$kdid')"
			ModelvideSrcNum=$(awk '{print $1}' $TmpDir/$PID)
			rm -f $TmpDir/$PID
			
			$SQL3A -e "SELECT capNum  into outfile '$TmpDir/$PID' FROM tblDeviceModel where devCapId=4 and modelId=(select devModelId from tblDevice where devId='$kdid')"
			ModelEncChnNum=$(awk '{print $1}' $TmpDir/$PID)
			rm -f $TmpDir/$PID
			
			#使用约定算法计算视频源，视频源和编码通道相等则视频源等于编码通道，否则视频源等于编码通道除2
			if [ $ModelvideSrcNum -eq $ModelEncChnNum ]
			then
				videSrcNum=$chnum
			else
				videSrcNum=`expr $chnum / 2`
			fi
			
			extData="<TDeviceCapExtData><extData size=\"2\"><item index=\"0\"><key>Lat</key><value>$lat</value></item><item index=\"1\"><key>Lon</key><value>$lon</value></item></extData></TDeviceCapExtData>"
			
			$SQL3A -e "insert into tblDeviceCapIndexData(deviceId,deviceCapId,deviceCapIndex,deviceCapIndexName,deviceCapIndexExtData)values('$kdid',1,$videSrcNum,'','$extData') on duplicate key update deviceCapIndexExtData='$extData'"
			
			let "i+=1"
		done < "$devfile";
		
		iconv -f UTF-8 -t GBK $devfile -o $devfile
	fi
done

echo "all device's info  in dir $1 have import success!"