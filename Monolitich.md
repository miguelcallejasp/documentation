Instalación Monolítico
# CENTOS 7                            
### Tools
`yum update -y &&`
`yum install mc vim nmap wget git curl httpd -y`

### Java INSTALLATION
```
cd /opt/
wget --no-cookies --no-check-certificate --header "Cookie: gpw_e24=http%3A%2F%2Fwww.oracle.com%2F; oraclelicense=accept-securebackup-cookie" "http://download.oracle.com/otn-pub/java/jdk/8u121-b13/e9e7ea248e2c4826b92b3f075a80e441/jdk-8u121-linux-x64.rpm"
rpm -ivh jdk-8u121-linux-x64.rpm
```

### Alternative to Oracle
```
wget https://s3.amazonaws.com/mojixjava/jdk-8u131-linux-x64.rpm
yum install jdk-8u131-linux-x64.rpm
```
# MySQL Server
Installing MySQL on CentOS 6
`sudo wget https://dev.mysql.com/get/mysql57-community-release-el6-7.noarch.rpm`
`sudo rpm -Uvh mysql57-community-release-el6-7.noarch.rpm`

Installing MySQL on CentOS 7
`sudo wget https://dev.mysql.com/get/mysql57-community-release-el7-9.noarch.rpm`
`sudo rpm -Uvh mysql57-community-release-el7-9.noarch.rpm`

Change versions in mysql VERIFY the number between /6/ for CentOS version
Enable=1 on mysql 5.5 and disable on 5.7 or superior

`vi /etc/yum.repos.d/mysql-community.repo`
`sudo yum install mysql-server -y`

Then enable and set the password control123!
say NO when prompted "disallow root remote cononectios" when server of db is in
a different instance. Otherwise it will be bind to localhost

`sudo service mysqld start`

If DISTRIBUTED DEPLOYMENT Disallow root login remotely must be OFF (n)
`tail -n 150 /var/log/mysqld.log |grep "password"`

Install
`mysql_secure_installation`

Set configurations
`vim /etc/my.cnf`

add the following
```
[mysqld]
wait_timeout=3000
interactive_timeout=3000
max_allowed_packet=512M
lower_case_table_names=1
```
For mysql CLIENTS on different SERVER
First, grant priviliges to connections on mysql database
`$mysql -h localhost -u root -p`
`mysql> GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'control123!';`

Change password
```
mysql>
update mysql.user set password=PASSWORD("control123!") where User='root';
update mysql.user set authentication_string=password('control123!') where user='root';
```
Newer versions require this command:
```
mysql>
SET PASSWORD = PASSWORD('m0j1xInc1!');
```
# MySQL CLIENT

Installing MySQL on CentOS 6
`sudo wget https://dev.mysql.com/get/mysql57-community-release-el6-7.noarch.rpm`
`sudo rpm -Uvh mysql57-community-release-el6-7.noarch.rpm`

Installing MySQL on CentOS 7
`sudo wget https://dev.mysql.com/get/mysql57-community-release-el7-9.noarch.rpm`
`sudo rpm -Uvh mysql57-community-release-el7-9.noarch.rpm`

Change versions in mysql VERIFY the number between /6/ for CentOS version
Enable=1 on mysql 5.5 and disable on 5.7 or superior
```
vi /etc/yum.repos.d/mysql-community.repo
sudo yum install mysql-community-client
```
then try
```
$mysql -h[ip-address] -uroot -p
mysql>
```

# MONGO

`vim /etc/yum.repos.d/mongodb-org-3.2.repo`
```
[mongodb-org-3.2]
name=MongoDB Repository
baseurl=https://repo.mongodb.org/yum/redhat/$releasever/mongodb-org/3.2/x86_64/
gpgcheck=0
enabled=1
```
`yum install mongodb-org mongodb-org-server mongodb-org-shell mongodb-org-mongos mongodb-org-tools`

Or you can install 3.4

`vim /etc/yum.repos.d/mongodb-org-3.4.repo`
```
[mongodb-org-3.4]
name=MongoDB Repository
baseurl=https://repo.mongodb.org/yum/redhat/$releasever/mongodb-org/3.2/x86_64/
gpgcheck=0
enabled=1
```
`yum install mongodb-org mongodb-org-server mongodb-org-shell mongodb-org-mongos mongodb-org-tools`


`service mongod start`
```
mongo
use admin
db.system.users.remove({})
db.system.version.remove({})
db.dropUser("admin")
db.createRole( { role: "executeFunctions", privileges: [ { resource: { anyResource: true }, actions: [ "anyAction" ] } ], roles: [] } )
db.createUser({ user: "admin", pwd: "control123!", roles: ["userAdminAnyDatabase", "executeFunctions"] })
>exit
```
Aauthentication

`vim /etc/init.d/mongo`

NOTE: if you change any OPTIONS here, you get what you pay for:
this script assumes all options are in the config file.
```
CONFIGFILE="/etc/mongod.conf"
OPTIONS=" -f $CONFIGFILE --auth"
```

On Mongo 3.4 the security is added here:

`/etc/mongod.conf`
```
security:
	authorization: enabled
```


then restart systemctl

`systemctl daemon-reload`
`service mongod restart`

comment for remote ACCESOS
`vim /etc/mongod.conf`

Network interfaces
```
net:
  port: 27017
#bindIp: 127.0.0.1  
#Listen to local interface only, comment to listen on all interfaces.
```

# MOSQUITTO
`vim /etc/yum.repos.d/mosquitto.repo`

Centos 6
```
[home_oojah_mqtt]
name=mqtt (CentOS_CentOS-6)
type=rpm-md
baseurl=http://download.opensuse.org/repositories/home:/oojah:/mqtt/CentOS_CentOS-6/
gpgcheck=1
gpgkey=http://download.opensuse.org/repositories/home:/oojah:/mqtt/CentOS_CentOS-6//repodata/repomd.xml.key
enabled=1
```
Centos 7
```
[home_oojah_mqtt]
name=mqtt (CentOS_CentOS-7)
type=rpm-md
baseurl=http://download.opensuse.org/repositories/home:/oojah:/mqtt/CentOS_CentOS-7/
gpgcheck=1
gpgkey=http://download.opensuse.org/repositories/home:/oojah:/mqtt/CentOS_CentOS-7//repodata/repomd.xml.key
enabled=1
```
`yum install mosquitto mosquitto-clients -y`

Create LOG File
```
mkdir -p /var/log/mosquitto
touch /var/log/mosquitto/mosquitto.log chown -R mosquitto: /var/log/mosquitto
```
Edit the mosquitto.conf file

`vim /etc/mosquitto/mosquitto.conf`

Add the following content at the end of the file:
```
persistent_client_expiration 1d
max_inflight_messages 1
max_queued_messages 0
sys_interval 5
log_dest file /var/log/mosquitto/mosquitto.log
log_type error
log_timestamp true
```
#  EPEL FOR TOMCAT

REPOLIST FOR CENTOS 7
`sudo wget http://dl.fedoraproject.org/pub/epel/7/x86_64/e/epel-release-7-9.noarch.rpm &&
sudo rpm -ivh epel-release-7-9.noarch.rpm`

Updated link redirects here
`sudo wget http://dl.fedoraproject.org/pub/epel/7/x86_64/Packages/e/epel-release-7-11.noarch.rpm && sudo rpm -ivh epel-release-7-11.noarch.rpm`

EPEL Repo
`vim /etc/yum.repos.d/epel.repo`
```
[epel]
name=Extra Packages for Enterprise Linux 7 - $basearch
#baseurl=http://download.fedoraproject.org/pub/epel/7/$basearch
mirrorlist=https://mirrors.fedoraproject.org/metalink?repo=epel-7&arch=$basearch
failovermethod=priority
enabled=1
gpgcheck=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-EPEL-7

[epel-debuginfo]
name=Extra Packages for Enterprise Linux 7 - $basearch - Debug
#baseurl=http://download.fedoraproject.org/pub/epel/7/$basearch/debug
mirrorlist=https://mirrors.fedoraproject.org/metalink?repo=epel-debug-7&arch=$basearch
failovermethod=priority
enabled=0
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-EPEL-7
gpgcheck=1

[epel-source]
name=Extra Packages for Enterprise Linux 7 - $basearch - Source
#baseurl=http://download.fedoraproject.org/pub/epel/7/SRPMS
mirrorlist=https://mirrors.fedoraproject.org/metalink?repo=epel-source-7&arch=$basearch
failovermethod=priority
enabled=0
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-EPEL-7
gpgcheck=1
```
Test using
`yum repolist`

REPOLIST FOR CENTOS 6

`sudo wget http://download.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm &&
sudo rpm -ivh epel-release-6-8.noarch.rpm`

# TOMCAT

Install version 7
`yum install tomcat`

Add this configuration on /etc/tomcat/tomcat.conf
`JAVA_OPTS="-Xmx2048m -Xms512m"`

To startup
`chkconfig tomcat on`
`service tomcat restart`

Start httpd
`yum install httpd`

`service httpd start`

#change file `vi /etc/tomcat/server.xml`
```
<Connector port="8080" protocol="HTTP/1.1" connectionTimeout="20000" redirectPort="8443"
compression="on"
compressionMinSize="2048" noCompressionUserAgents="gozilla, traviata"
compressableMimeType="text/html,text/xml,application/json,text/pla in"/>
```
Proxy configuration
`vim /etc/httpd/conf.d/vizix.conf`
```
<VirtualHost *:80>
  DocumentRoot /var/www/html/
  ProxyPreserveHost On
  # Servers to proxy the connection, or
  # List of application servers Usage
  ProxyPass /riot-core-services http://localhost:8080/riot-core-services
  ProxyPassReverse /riot-core-services http://localhost:8080/riot-core-services
  ServerName localhost
</VirtualHost>
```
`$service httpd restart`

# VIZIX REPO
VIZIX 4
Add repositories
```
vim /etc/yum.repos.d/vizix4.repo
vim /etc/yum.repos.d/vizix5.repo
```
```
[vizix4]
name=vizix4
baseurl=http://qamojix:q4m0j1x@vizixrepo.mojix.com/test/ViZix/main/4
baseurl=http://customers:v1z1xr3p0axxe55@vizixrepo.mojix.com/ViZix/main/4
gpgcheck=0
enabled=0
```
`yum install vizix-services-4.5.0 vizix-alebridge-4.5.0 vizix-corebridge-4.5.0 vizix-ui-4.5.0 vizix-simulator-4.5.0 --enablerepo=vizix4`
`yum remove vizix-services-4.5.0 vizix-alebridge-4.5.0 vizix-corebridge-4.5.0 vizix-ui-4.5.0 vizix-simulator-4.5.0 --enablerepo=vizix4`

`yum install vizix-services-5.0.0_RC09 vizix-alebridge-5.0.0_RC09 vizix-corebridge-5.0.0_RC09 vizix-ui-5.0.0_RC09 vizix-simulator-5.0.0_RC09 --enablerepo=vizix5`


`yum install vizix-ui-5.0.0_RC10-E5.0.0_RC10 vizix-services-5.0.0_RC10-E5.0.0_RC10 vizix-corebridge-5.0.0_RC10-E5.0.0_RC10 vizix-alebridge-5.0.0_RC10-E5.0.0_RC10 vizix-agent-5.0.0_RC10-E5.0.0_RC10vizix-simulator-5.0.0_RC10-E5.0.0_RC10 --enablerepo=vizix5`

#Despues de RC10, la instalacion es
`./popdb gradle-default.properties AppRetail o CoreTenant`

# ALEBRIDGE
Inicio de ALE
`/etc/init.d/aleBridge`

El ALEbridge se conecta con SERVICES y MOSQUITTO acá (no con MySQL)
Mosquitto sabe donde está gracias a que services le dice
NO SABE donde está services (busca en localhost) con esto:

Aca hay esto, pero al parecer es inutil. No consulta a services o hazelcast
`HAZELCAST_SERVER=localhost`

`ALEB_OPTIONS="-bp 9090 -c ALEB -apikey 7B4BCCDC"`
### En Docker mandamos toda la información
`ALEB_OPTIONS="-bp $VIZIX_ALEB_PORT -c $VIZIX_ALEB_CODE -httpHost $VIZIX_API_HOST -httpPort $VIZIX_API_PORT -apikey $VIZIX_API_KEY"`

# COREBRIDGE
### Inicio de Corebridge
`/etc/init.d/coreBridge`
El corebridge solamente se conecta con mysql para conseguir la información de otros modulos
(de aquí saca la info) despues se conecta con mosquitto para sacar el mensaje y publicarlo a mongo
Conoce las direcciones de hazelcast (para saber si el thing solamente necesita update)
Esto está seteado en localhost por defecto
`HAZELCAST_SERVER=<IP-ADDRESS-SERVICES>`

Cuando está con una IP asignada, cuestiona los puertos 5701 5702 y 5703

`PROPS="$PROPS -Dhibernate.connection.url=jdbc:mysql://<mysql-ip-address>:3306/riot_main"`
`PROPS="$PROPS -Dhibernate.cache.hazelcast.native_client_address=$HAZELCAST_SERVER"`

`MCB_OPTIONS="-c MCB -id 1 -apikey 7B4BCCDC";`
#En Docker mandamos toda la información
`MCB_OPTIONS="-httpHost $VIZIX_API_HOST -httpPort $VIZIX_API_PORT -c $VIZIX_CORE_CODE -apikey $VIZIX_API_KEY"`

# FILES

In case Databases are in different SERVER, configure parameters

`vim /usr/share/tomcat/conf/Catalina/localhost/riot-core-services.xml`

Logs for tomcat
`tail -f /var/log/tomcat/riot-core-services.log`

Edit riot-core-services
`/usr/share/tomcat/conf/Catalina/localhost/riot-core-services.xml`

to set memory settings
`/etc/tomcat/tomcat.conf`
To set riot-core-services json
`/var/www/html/scripts/config.js`
Other resources for RIOT
`/usr/local/riot`
