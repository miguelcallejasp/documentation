Cloud Ops
## MQTT Broker Commands
```
apt-get install mosquitto-client
mosquitto_sub -h ip -v -t "#"
```
Password protected
```
mosquitto_pub -h localhost -p 1883 -t "/test/1" -m "hi" -u admin -P adminpass
mosquitto_pub -h localhost -p 1883 -t "/test/1" -m "hi" -u owner -P ownerpass
mosquitto_pub -h localhost -p 1883 -t "/test/1" -m "hi" -u write -P writepass

mosquitto_sub -h localhost -p 1883 -t "/test/1" -u admin -P adminpass
mosquitto_sub -h localhost -p 1883 -t "/test/1" -u owner -P ownerpass
mosquitto_sub -h localhost -p 1883 -t "/test/1" -u write -P writepass
```
## Mongo Commands
#### Mongo Dump

`mongodump --host $MONGO_HOST --port $MONGO_PORT --db $MONGO_DATABASE --username $MONGO_USERNAME --password $MONGO_PASSWORD --authenticationDatabase $MONGO_USERNAME --gzip --archive=$BACKUP_DIR/mongodb_${DM}.archive`
`mongodump --host 127.0.0.1 --port 27017 --db riot_main --username admin --password control123! --authenticationDatabase admin --archive=/mongodb_show.archive`
`mongodump --host 127.0.0.1 --port 27017 --db riot_main --username admin --password control123! --authenticationDatabase admin --gzip --archive=/mongodb_show.gz`

#### Mongo Restore
`mongorestore --gzip --archive=test.20150715.gz --db riot_main`
`mongorestore --archive=test.20150715.archive --db riot_main`
`mongorestore -u admin -p control123! --authenticationDatabase admin --gzip --archive=mongo_backup_demo.gz --db riot_main`

#### Mongo Perf
```
echo "{ nThreads: 8, fileSizeMB: 256, sleepMicros: 0, mmf: false, r: true, w: true, syncDelay: 0 }" | mongoperf
echo "{ nThreads:1024, fileSizeMB:1000, mmf:false, r:true, w:true, syncDelay:60 }" | mongoperf
```
In a monolitich environment
```
ls /sys/kernel/mm/transparent_hugepage
cat /sys/kernel/mm/transparent_hugepage/enabled
echo 'never' >  /sys/kernel/mm/transparent_hugepage/enabled
cat /sys/kernel/mm/transparent_hugepage/enabled
echo 'never' >  /sys/kernel/mm/transparent_hugepage/defrag
```
#### Mongo Replica Set
Generate a Security Key
```
openssl rand -base64 756 > mnsmongo
chmod 400 mnsmongo
chown mongodb:mongodb mnsmongo
```
Mongo Primary

`cat /etc/mongod.conf`
```
#Mongo
#Where and how to store data.
storage:
  dbPath: /data
  journal:
    enabled: true
  engine: wiredTiger
  directoryPerDB: true
#mmapv1:
  wiredTiger:
   engineConfig:
    directoryForIndexes: true
#network interfaces
systemLog:
  verbosity: 0
  traceAllExceptions: true
  destination: file
  logAppend: true
  logRotate: reopen
  timeStampFormat: ctime
  path: /data/mongod.log
net:
  port: 27017
#bindIp: 127.0.0.1
security:
  keyFile: /opt/mnsmongo
#processManagement:
#security:
#operationProfiling:
replication:
  oplogSizeMB: 10240
  replSetName: mns
  enableMajorityReadConcern: true
```
And this should be in the Arbiter
```
storage:
  dbPath: /data/arb
  journal:
    enabled: true

#where to write logging data.
systemLog:
  destination: file
  logAppend: true
  path: /var/log/mongodb/mongod.log

#network interfaces
net:
  port: 27017
#bindIp: 127.0.0.1
#security:
security:
  keyFile: /opt/mnsmongo
#operationProfiling:
replication:
  oplogSizeMB: 10240
  replSetName: mns
  enableMajorityReadConcern: true
```

## MySQL Commands
#### Backup
```
mysqldump -u$MYSQL_USER -p$MYSQL_PASSWORD -h$HOST_MYSQL --add-drop-database --add-drop-table --hex-blob --databases $MYSQL_DB | gzip > $BACKUP_DIR/mysql_${DM}.sql.gz
mysqldump -uroot -p -h127.0.0.1 --add-drop-database --add-drop-table --hex-blob --databases riot_main | gzip > /mysql_show.sql.gz
```
#### Add permissions to user
```
mysql -hlocalhost -uroot -p

mysql>show databases;
mysql>show tables in riot_main;   #Expect at least 69 rows
mysql>GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'control123!';
mysql>exit
```
## DBNuke Things
### MySQL
```
mysql -p
SET FOREIGN_KEY_CHECKS = 0;
use riot_main;
TRUNCATE apc_thing;
TRUNCATE thingImage;
TRUNCATE ThingParentHistory;
SET FOREIGN_KEY_CHECKS = 1;
```
```
mongo admin -u admin -p control123!
use riot_main
db.getCollection('things').remove({})
db.getCollection('thingSnapshotIds').remove({})
db.getCollection('thingSnapshots').remove({})
db.getCollection('timeseries').remove({})
db.getCollection('timeseriesControl').remove({})
db.getCollection('exit_report').remove({})
db.repairDatabase()
```
## GIT
### Create Tags
Create Tags
```
git fetch
git checkout 4.x.x  #hotfix/5.0.0_RC10
git pull
```
For example
```
git tag -a h5.0.0_RC10 -m "Tagget hotfix5.0.0_RC10"
git push origin --tags
```
Delete Tags
```
git tag -d [tag]
git push origin :refs/tags/[tag]
```

### Management
