[ -f ./kdm3adb ] && mv ./kdm3adb ./KDM3ADB
[ ! -f ./KDM3ADB ] && echo 'please provide KDM3ADB database file first' && exit 1

SQL_CMD1='mysql -uroot -pkdc'
$SQL_CMD1 -e 'drop database KDM3ADB' >/dev/null 2>&1
$SQL_CMD1 -e 'create database KDM3ADB default character set utf8'
$SQL_CMD1 --default-character-set=utf8 KDM3ADB < ./KDM3ADB

SQL_CMD2='mysql -uroot -pkdc --default-character-set=utf8 KDMAAA'
$SQL_CMD2 < ./get_devmodel.sql
$SQL_CMD2 < ./find_out_devmodel_missing.sql >dev_missing.txt
echo '# 编码器设备型号(都不是国标设备型号)，具体字段意义如下：' >model_missing.txt
echo '# varDeviceManufacturer: 厂商 varGbLable: 是否国标设备 varEncodeLabel: 9 固定IPC, 10 云台IPC, 7 NVR ' >>model_missing.txt
echo '# varEncodeChnNum：编码通道 varVideoSrcNum: 视频源，varLicenseNum：计费路数 varLocalStorage：本地存储 varAlarmInput：并口告警  ' >>model_missing.txt
LANG=en
sed -n -e 's/.*,\(varDeviceManufacturer--.\+\)/\1/p' dev_missing.txt|sort|uniq >>model_missing.txt

echo '# 解码器设备型号，具体字段意义 varDecodeChannel: 解码通道' >>model_missing.txt
sed -n -e 's/.*,\(varDecodeChannel--.\+\)/\1/p' dev_missing.txt|sort|uniq >>model_missing.txt

SQL_CMD3='mysql -uroot -pkdc'
$SQL_CMD3 -e 'drop database UASDB' >/dev/null 2>&1
$SQL_CMD3 -e 'create database UASDB default character set utf8'
$SQL_CMD3 --default-character-set=utf8 UASDB < ./UASDB
