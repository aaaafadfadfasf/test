# 临时文件目录
TmpDir=/tmp

#导出设备列表
SQL="mysql -uroot -pkdc --default-character-set=utf8 KDM3ADB"
TmpTblGbDevice="tblGbDevice.csv";

# 设备表tblGbDevice
rm -f $TmpDir/$TmpTblGbDevice
$SQL -e "SELECT DeviceName,DeviceNetAddress,VidSrcNum,EdcChnNum,CameraNum,GBDeviceNO,Gbdev.DeviceNO into outfile '$TmpDir/$TmpTblGbDevice'
 FROM GBDeviceInfo as Gbdev inner join DeviceBaseInfo as devBase on Gbdev.DeviceNO=devBase.DeviceNO
 where D2DomainNO in (select DomainNO from DomainInfo where DomainFlag = 1) and (DeviceType=21 or DeviceType=22 or DeviceType=24)
 and NumberType in(112,111) and DeviceManufacturer like('GB%')"
 
total=$(awk 'END{print NR}' $TmpDir/$TmpTblGbDevice)
Entry="Entry"
Split="&#x09;"
defPass="888888"
i=0
while read line
do
	DeviceName=`echo $line | awk  '{print $1}'`
	IpAddress=`echo $line | awk  '{print $2}'`
	VidSrcNum=`echo $line | awk  '{print $3}'`
	EdcChnNum=`echo $line | awk  '{print $4}'`
	licenseNum=`echo $line | awk  '{print $5}'`
	gbDevId=`echo $line | awk  '{print $6}'`
	kdDevId=`echo $line | awk '{print $7}'`
	
	value=$DeviceName$Split$IpAddress$Split$gbDevId$Split$defPass$Split$VidSrcNum$Split$EdcChnNum$Split$licenseNum$Split$gbDevId$Split$kdDevId
	
	/opt/kdm/system/xmlfile_tool --set /opt/kdm/gbs/conf/gbscfg.xml  XmlRoot GBPU $Entry$i $value
	if [ $? -ne 0 ]; then
		echo 'write gbpu failed'; 
	fi
	
	let "i+=1"
done < "$TmpDir/$TmpTblGbDevice";

/opt/kdm/system/xmlfile_tool --set /opt/kdm/gbs/conf/gbscfg.xml XmlRoot GBPU EntryNum $i

rm -f $TmpDir/$TmpTblGbDevice


