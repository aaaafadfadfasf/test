#!/bin/bash
SQL='mysql -uroot --default-character-set=utf8 KDMAAA'
DATA=$1
LEN=${#DATA}
if [ $LEN -ne 32 ] ; then
   echo "wrong treeId" && exit 1
fi
$SQL < ./backupdata.sql
$SQL -e "call procBackupData('${DATA}')"
exit 0
