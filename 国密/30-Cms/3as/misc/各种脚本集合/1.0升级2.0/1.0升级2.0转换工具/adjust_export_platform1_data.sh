MYSQLDUMP=mysqldump
[ -f /opt/mysql/bin/mysqldump ] && MYSQLDUMP=/opt/mysql/bin/mysqldump
TELNET=telnet.sh
[ -f ./telnet.sh ] && chmod +x ./telnet.sh && TELNET=./telnet.sh

export LANG=zh_CN.GB2312
printf "%s" "please wait(about 2 seconds per pu)..."
(echo "";  echo "";  echo "cp 9";sleep 30)|$TELNET cmu 2>/dev/null >online_pu.txt
grep -P '^\s*\d{32}\s+' online_pu.txt |cut -f 1 >online_pu_id.txt
printf "%s" "..."
mysql -uroot -pkdc -e 'create database if not exists KDM3ADB_tmp';$MYSQLDUMP KDM3ADB -uroot -pkdc --add-drop-table |mysql KDM3ADB_tmp -uroot -pkdc
mysql -uroot -pkdc KDM3ADB_tmp -e 'delete from DeviceBaseInfo where DeviceMapNO!="00000000000000000000000000000000"'

echo '' >incorrect_dev_list.txt
cat online_pu_id.txt |while read line
do
    while true
    do
        line="$(echo $line|tr -cd [:digit:])"
        IsEncode="$(mysql -uroot -pkdc KDM3ADB_tmp -e "select * from DeviceBaseInfo where DeviceNO=\"$line\" and (DeviceType=21 or DeviceType=22)")"
        [ -z "$IsEncode" ] && break
        
        (echo "";  echo "";  echo "cpn \"${line}\"";sleep 2)|$TELNET cmu 2>/dev/null|grep -P '(^\s*编码通道-\d+:$)|(^\s*视频源通道-\d+:$)' >chn_num_tmp.txt
        VarEdcChnNum=$(grep -P '(^\s*编码通道-\d+:$)' chn_num_tmp.txt |cut -f2 -d-|cut -f 1 -d:)
        VarVidSrcNum=$(grep -P '(^\s*视频源通道-\d+:$)' chn_num_tmp.txt |cut -f2 -d-|cut -f 1 -d:)
        
        [ ! -z $VarVidSrcNum ] && break
    done
    
    mysql -uroot -pkdc KDM3ADB_tmp -e "select DeviceNO,DeviceName from DeviceBaseInfo where DeviceNO='$line' and (EdcChnNum!=$VarEdcChnNum or VidSrcNum!=$VarVidSrcNum)" >>incorrect_dev_list.txt
    mysql -uroot -pkdc KDM3ADB_tmp -e "update DeviceBaseInfo set EdcChnNum=$VarEdcChnNum, VidSrcNum=$VarVidSrcNum where DeviceNO='$line'"
    printf "%s" "..."
done

$MYSQLDUMP KDM3ADB_tmp -uroot -pkdc --add-drop-table >KDM3ADB
$MYSQLDUMP uas -uroot -pkdc > UASDB
mysql -uroot -pkdc -e 'drop database if exists KDM3ADB_tmp'
printf "\n%s\n" "finished!"
