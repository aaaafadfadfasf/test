create table vDevicefullinfoAndPrivilege
(
 devId                | char(32)      | NO   |     |         |       |
 devCapId             | int(11)       | NO   |     | 0       |       |
 devCapIndex          | int(11)       | NO   |     | 0       |       |
 deviceCapIndexName   | varchar(64)   | YES  |     | NULL    |       |
 devCapIndexPrivilege | int(11)       | YES  |     | NULL    |       |
 domainId             | char(32)      | NO   |     |         |       |
 devType              | int(11)       | NO   |     | 0       |       |
 devModelId           | char(32)      | YES  |     | NULL    |       |
 devName              | varchar(100)  | YES  |     | NULL    |       |
 devAlias             | varchar(64)   | YES  |     | NULL    |       |
 useFlag              | tinyint(1)    | YES  |     | NULL    |       |
 netType              | int(11)       | YES  |     | NULL    |       |
 netAddr              | varchar(64)   | YES  |     | NULL    |       |
 devDesc              | varchar(1024) | YES  |     | NULL    |       |
 createDate           | datetime      | YES  |     | NULL    |       |
 expireDate           | datetime      | YES  |     | NULL    |       |
 devPrivilege         | int(11)       | YES  |     | NULL    |       |
 cuserId              | char(32)      | NO   |     |         |       |
);