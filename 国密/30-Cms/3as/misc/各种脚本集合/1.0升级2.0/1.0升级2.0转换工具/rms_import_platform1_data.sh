if [ ! -f rms.1.csv -o ! -f rms.2.csv ]; then
    printf "\n%s\n" "no data!"
    exit 1
fi

mv -f rms.1.csv /tmp
mv -f rms.2.csv /tmp
chmod 0777 /tmp/rms.*.csv

SQL='mysql -uroot -pkdc -e'

$SQL "load data infile '/tmp/rms.1.csv' into table rms.VideoRecord(VodId,EdcId,EdcChnId,StartTime,EndTime,RecType,FilePath,FileName,RecSize,IsHasAudio,AudioEncFmt,VidEncFmt)"

$SQL "load data infile '/tmp/rms.2.csv' into table rms.SubVideoRecord(VodId,RecType,StartTime,EndTime)"

rm -rf /tmp/rms.*.csv

$SQL "update rms.VideoRecord set EdcId=concat(EdcId, '@', (select domainId from KDMAAA.tblDevice where devId=EdcId)) where exists(select 1 from KDMAAA.tblDevice where devId=EdcId)"

rm -rf /tmp/rms.ignore_data.csv
$SQL "select DISTINCT EdcId into outfile '/tmp/rms.ignore_data.csv' from rms.VideoRecord where length(rms.VideoRecord.EdcId)<33"
$SQL "delete from rms.SubVideoRecord where exists(select 1 from rms.VideoRecord where rms.VideoRecord.VodId=rms.SubVideoRecord.VodId and length(rms.VideoRecord.EdcId)<33)"
$SQL "delete from rms.VideoRecord where length(rms.VideoRecord.EdcId)<33"


