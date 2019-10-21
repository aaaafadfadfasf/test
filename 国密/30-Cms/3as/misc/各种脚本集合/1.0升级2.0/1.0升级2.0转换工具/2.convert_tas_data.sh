mount -o rw,remount /
SQL_CMD2='mysql -uroot -pkdc --default-character-set=utf8 KDMAAA'
$SQL_CMD2 < ./get_devmodel.sql
bash ./adjust_domain_id.sh
$SQL_CMD2 < ./convert_dev_from_platform1.sql >dev_not_convert.txt
$SQL_CMD2 < ./convert_gbdev_from_platform1.sql
$SQL_CMD2 < ./convert_vidsrc_alias.sql
bash ./write_gbscfg_xml.sh

$SQL_CMD2 -e 'drop database KDM3ADB'
$SQL_CMD2 -e 'drop database UASDB'
$SQL_CMD2 -e 'drop procedure procGetDevModelId4Encoder'
$SQL_CMD2 -e 'drop procedure procGetDevModelId4Decoder'
