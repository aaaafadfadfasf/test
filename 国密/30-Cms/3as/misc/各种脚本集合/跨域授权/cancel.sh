#!/bin/bash
###############################################################################
#                 usage: cancel.sh FileName | DeviceId                        #
###############################################################################
# cancel.sh������Ŀ¼��Ϊ�����������Ŀ¼
SaveDir=$(cd $(dirname $0); pwd)

# ��ʱ�ļ�Ŀ¼
TmpDir=/tmp


# �ű�ִ��ʱ�Ľ���PID
PID=$$

# ��ȡ�������������ڴ��ڿ�����Ȩ���޷�ֱ�ӵĴ����ݿ�����жϳ�������Ϣ��
LocalDomainId=`/opt/kdm/system/xmlfile_tool --get /etc/kdm/commonconfig.ini commonconfig domain uuid`

if [ $# -ne 1 ]; then
    echo "usage: $0 FileName | DeviceId"
    exit
fi

# ��ȡ������Ҫȡ����Ȩ���豸�б�
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
# ��ѯ�������¼������ϵ�б�
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
#��������Ŀ¼
TimeDir="cancel-$(date +"%Y-%m-%d-%H%M%S")"
TmpDataDir="$TmpDir/$TimeDir"
test ! -d $TmpDataDir&&mkdir -m 777 $TmpDataDir

#�ȱ��ݽ�Ҫɾ�������ݣ�ע�⣺��ɾ���ű�����һ�£�����ֹ��ɾ������ʹ�ñ������ݽ��лָ���
# �����豸�ͺű�tblDeviceModel����
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDeviceModel' from tblDeviceModel where exists (select * from tblDevice dev1 where dev1.devId in ($DeviceList) and dev1.devModelId = modelId) and not exists (select * from tblDevice dev2 where dev2.devId not in ($DeviceList) and dev2.devModelId = modelId) "

# �����豸�����û���tblCuser2Dev����
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblCuser2Dev' from tblCuser2Dev  where devId in ($DeviceList) "

# �����豸���������û���tblCuser2DeviceCap����
$SQL -e "select *  into outfile '$TmpDataDir/$TmpTblCuser2DeviceCap' from tblCuser2DeviceCap where devId in ($DeviceList) "

# �����豸���tblDevGroup����
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

# �����豸�����豸���tblDevGrp2Dev����
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDevGrp2Dev' from tblDevGrp2Dev where devId in ($DeviceList) "

# ������ƵԴ������tblDeviceCapIndexName����
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDeviceCapIndexName' from tblDeviceCapIndexName where deviceId in ($DeviceList) "

# �������ϵ��tblDomainRelation����
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDomainRelation' from tblDomainRelation where domainId in (select domainId from (select domainId from tblDevice dev1 where dev1.devId in ($DeviceList)) tmp where not exists (select * from tblDevice dev2 where dev2.devId not in ($DeviceList) and dev2.domainId = tmp.domainId))"

# �����豸��tblDevice���ݣ��������ɾ��������ǰ�沿�����������㣩
$SQL -e "select * into outfile '$TmpDataDir/$TmpTblDevice' from tblDevice where devId in ($DeviceList) "

# ���ɱ������ݰ�����������ʱ�ļ�
cd $TmpDataDir > /dev/null
tar -cf backupdata.tar $TmpTblDevice $TmpTblDeviceModel $TmpTblCuser2Dev $TmpTblCuser2DeviceCap $TmpTblDevGrp2Dev $TmpTblDevGroup $TmpTblDeviceCapIndexName $TmpTblDomainRelation
rm -f $TmpTblDevice $TmpTblDeviceModel $TmpTblCuser2Dev $TmpTblCuser2DeviceCap $TmpTblDevGrp2Dev $TmpTblDevGroup $TmpTblDeviceCapIndexName $TmpTblDomainRelation

if [ "$TmpDataDir" != "$SaveDir/$TimeDir" ]; then
    mv $TmpDataDir $SaveDir
fi

#-------------------------------------------------------------------------------------------------------------------------
# ɾ���豸�ͺű�tblDeviceModel����
$SQL -e "delete from tblDeviceModel where exists (select * from tblDevice dev1 where dev1.devId in ($DeviceList) and dev1.devModelId = modelId) and not exists (select * from tblDevice dev2 where dev2.devId not in ($DeviceList) and dev2.devModelId = modelId) "

# ɾ���豸�����û���tblCuser2Dev����
$SQL -e "delete from tblCuser2Dev where devId in ($DeviceList) "

# ɾ���豸���������û���tblCuser2DeviceCap����
$SQL -e "delete from tblCuser2DeviceCap where devId in ($DeviceList) "

# ɾ���豸���tblDevGroup����
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

# ɾ���豸�����豸���tblDevGrp2Dev����
$SQL -e "delete from tblDevGrp2Dev where devId in ($DeviceList) "

# ɾ����ƵԴ������tblDeviceCapIndexName����
$SQL -e "delete from tblDeviceCapIndexName where deviceId in ($DeviceList) "

# ɾ�����ϵ��tblDomainRelation����
$SQL -e "delete from tblDomainRelation where domainId in (select domainId from (select domainId from tblDevice dev1 where dev1.devId in ($DeviceList)) tmp where not exists (select * from tblDevice dev2 where dev2.devId not in ($DeviceList) and dev2.domainId = tmp.domainId))"

# ɾ���豸��tblDevice���ݣ��������ɾ��������ǰ�沿�����������㣩
$SQL -e "delete from tblDevice where devId in ($DeviceList) "

# ɾ����Ȩ�豸���ݺ���Ҫ���¶�Ӧ�İ汾��Ϣ������cu���ܲ����ȡ���µ�����
CurrentDate=`date +"%Y-%m-%d"`
CurrentTime=`date +"%H-%M-%S"`
#ʹ��Ĭ�ϵ�ʱ��������������
DevTreeVer="${CurrentDate}T${CurrentTime}+08:00"
$SQL -e "update tblCuser set devTreeVer = '"$DevTreeVer"'"

# ��������������ϣ���ʾ��Ҫ����ƽ̨
echo "all data deleted is  backup to file[$SaveDir/$TimeDir/backupdata.tar]"
echo "finish completely, press any key to reboot!"
read -n 1
reboot

