rm -rf /tmp/rms.1.csv /tmp/rms.2.csv

SQL='mysql -uroot -pkdc -e'

$SQL "select VodId,EdcId,EdcChnId,StartTime,EndTime,RecType,FilePath,FileName,RecSize,IsHasAudio,AudioEncFmt,VidEncFmt into outfile '/tmp/rms.1.csv' from uas.VideoRecord"

$SQL "select VodId,0,StartTime,EndTime into outfile '/tmp/rms.2.csv' from uas.VideoRecord"

mv -f /tmp/rms.1.csv .
mv -f /tmp/rms.2.csv .
