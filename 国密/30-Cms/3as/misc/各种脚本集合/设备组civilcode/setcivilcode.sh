#!/bin/bash
###############################################################################
# usage:根据配置的设备组CivilCode设置所有点位的CivilCode  #       
#       setcivilcode.sh grpcivilcodefile #
###############################################################################

if [ $# -ne 2 ]; then
    echo "usage: $0 grpcivilcodefile gbdomainId"
    exit
fi

if [ ! -f $1 ]; then
    echo "$1 is not exist"
    exit
fi

dos2unix $1
gbdomainId=$2

# 脚本执行时的进程PID
PID=$$
TmpDir=/tmp

# 临时文件目录
CurDir=$(cd $(dirname $0); pwd)
FileFullPath=$CurDir/$1
echo $FileFullPath

SQL="mysql -uroot -pkdc --default-character-set=utf8 GBS"
NameFile="NameFile"

#编码格式转换
#iconv -f GBK -t UTF-8 $devfile -o $devfile

#导入设备列表
while IFS=, read grpname civilcode
do
	echo $grpname"		"$civilcode
	
	if [ $($SQL -e "select count(*) from tblGbDevice where name='$grpname'" > $TmpDir/$PID && sed -n '2p' $TmpDir/$PID && rm -f $TmpDir/$PID) -ne 1 ]; then
        echo "Invalid group [ $grpname ] can not find or find more than one"
	else	
		#获取设备组国标ID
		$SQL -e "select gbid into outfile '$TmpDir/$NameFile' from tblGbDevice where  name='$grpname'" 
		curGrpId=$(awk '{print $1}' $TmpDir/$NameFile)
		rm -f $TmpDir/$NameFile
		
		#插入civilcode
		$SQL -e "insert into tblGbDevice(gbid,name,parental,parentID) values('$civilcode','$grpname',1,'$gbdomainId')"
		#echo "insert into tblGbDevice(gbid,name,parental,parentID) values('$civilcode','$grpname',1,'$gbdomainId')"
		#更新改组下面节点civilcode
		$SQL -e "update tblGbDevice set civilCode='$civilcode' where parentID LIKE '%$curGrpId%'"
		#echo "update tblGbDevice set civilCode='$civilcode' where parentID LIKE '%$curGrpId%'"
    fi
done < "$FileFullPath";

#iconv -f UTF-8 -t GBK $devfile -o $devfile

echo "all group's info  in $1 have process complete!"
