# notice: it can work only on the just inited platform

new_domainId=$(mysql -uroot -pkdc --default-character-set=utf8 KDM3ADB -e 'select DomainNO from DomainInfo where DomainFlag=1' |tail -n 1)
domainalias="<TDomainExtData><extData size=\"2\"><item index=\"0\"><key>fixAddr</key></item><item index=\"1\"><key>kdmNO</key><value>$new_domainId</value></item></extData></TDomainExtData>"
if [ ${#new_domainId} -ne 32 ]; then
    echo 'new domainid is not correct'; exit 1
fi

old_domainId=$(/opt/kdm/system/xmlfile_tool --get /etc/kdm/commonconfig.ini commonconfig domain uuid)
if [ ${#old_domainId} -ne 32 ]; then
    echo "read old domainid failed"; exit 2
fi
/opt/kdm/system/xmlfile_tool --set /etc/kdm/commonconfig.ini commonconfig domain uuid $new_domainId
if [ $? -ne 0 ]; then
    echo 'write new domainid failed'; exit 3
fi

SQL='mysql -uroot -pkdc --default-character-set=utf8 KDMAAA'
SQLGB='mysql -uroot -pkdc --default-character-set=utf8 GBS'

$SQL -e "update tblDeviceModel set modelId='$new_domainId', domainId='$new_domainId' where modelId='$old_domainId'";
$SQL -e "update tblDevGroup set devgroupId='$new_domainId', domainId='$new_domainId' where devgroupId='$old_domainId'";
$SQL -e "update tblDevGroup set domainId='$new_domainId', parentDevgroupId='$new_domainId' where parentDevgroupId='$old_domainId'";
$SQL -e "update tblDomainRelation set domainId='$new_domainId' where domainId='$old_domainId'";
$SQL -e "update tblDomainRelation set parentDomainId='$new_domainId' where parentDomainId='$old_domainId'";
$SQL -e "update tblCuser set cuserId='$new_domainId' where cuserId='$old_domainId'";
$SQL -e "update tblTuser set tuserId='$new_domainId' where tuserId='$old_domainId'";
$SQL -e "update tblDeviceModel set domainId=$new_domainId where domainId='$old_domainId'"
$SQL -e "update tblDomainRelation set domainAlias='$domainalias' where domainId='$old_domainId'";

$SQLGB -e "update tblGbDevice set kdid=$new_domainId,kdDomainId=$new_domainId where kdDomainId='$old_domainId'"
