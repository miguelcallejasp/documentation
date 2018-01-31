## Build Tools for ViZix ESCROW Tool
### Operating System

CentOS 7 64 bits

### Tool Box
`GIT` version `1.8.3.1-12.el7_4`
`JAVA` version `jdk-8u131-linux-x64`
`wget` version ` 1.14-15.el7_4.1`
`Unzip` version `6.0-16.el7`
`VIM` version `2:7.4.160-2.el7`
`Mongo` version `3.4.10-1.el7`
`Gradle` version `3.4.1`
`Tomcat` version `7.0.76-3.el7_4`
`RPM Build` version `4.11.3-25.el7`
`HTTP` version `2.4.6-67.el7.centos.6`
`Mosquitto Server` version `1.4.14-1.2`
`MySQL`version `5.7`

### Cloned Projects Repositories
Tierconnect GitHub Repositories
Version `release_5.1.4`

`riot-core-bridges`
`riot-core-services`
`riot-core-ui`
`vizix-flow`

## Preparing the OS
Tools to copy and manage the repository

`yum update`
`yum install git`
`yum install wget`
`yum install unzip`
`yum install rpm-build`

To set SELinux to permisive
`setenforce 0`
To verify the SEStatus:
`sestatus`

To change it definitely:
`vi /etc/selinux/config` and change the value:
`SELINUX=disabled`

* Note: If SELINUX was on a different setting, the server needs to be rebooted at this point.

### Installing Oracle Java

`wget https://s3.amazonaws.com/mojixjava/jdk-8u151-linux-x64.rpm`
`yum install jdk-8u151-linux-x64.rpm`

Installing Gradle

```
mkdir /opt/gradle/
cd /opt/gradle/
wget https://services.gradle.org/distributions/gradle-3.4.1-bin.zip
unzip gradle-3.4.1-bin.zip
export PATH=$PATH:/opt/gradle/gradle-3.4.1/bin/
```

### Building ViZix from Source Code

Uncompress the folders that were give. We recommend to use this PATH `cd ~/`
The following tutorial will use this path as reference.

`tar -zxvf riot-core-services.tar.gz`
`tar -zxvf riot-core-bridges.tar.gz`
`tar -zxvf riot-core-ui.tar.gz`
`tar -zxvf vizix-flow`

#### Install NPM for LogIO
Installing EPEL CentOS 7
`sudo wget http://dl.fedoraproject.org/pub/epel/7/x86_64/Packages/e/epel-release-7-11.noarch.rpm && sudo rpm -ivh epel-release-7-11.noarch.rpm`

```
yum install npm nodejs
yum install gcc-c++
npm install -g log.io --user "root"
```

Change values to HTTPD CentOS Format
`vim  ~/.log.io/harvester.conf`
```
exports.config = {
  nodeName: "ITzGEEK_server", # Node Name
  logStreams: {
    apache: [
      "/var/log/httpd/access_log",  # Monitoring Apache Logs
      "/var/log/httpd/error_log"
    ],
  },
server: {
host: '127.0.0.1',
port: 28777
  }
}
```
Start LOGio on the background. This is needed to build the application

`log.io-server &`

Press `Enter` to continue.

#### Build Gradle Configuration ViZix Services

`cd ~/riot-core-services`
`gradle clean dist publish rpm`

Building of this repo needs to be finished before continuing.

#### Build ViZix Bridges
`cd ~/riot-core-bridges`
`gradle clean dist rpmagent`

The building is done here. The following STEPS will INSTALL ViZix Services and its dependencies. ViZix UI Composer will be built and installed after these steps.

## ViZix Services Installation

### Installing MySQL

```
sudo wget https://dev.mysql.com/get/mysql57-community-release-el7-9.noarch.rpm
sudo rpm -Uvh mysql57-community-release-el7-9.noarch.rpm
sudo yum install mysql-server -y
sudo service mysqld start
tail -n 150 /var/log/mysqld.log |grep "password"
```
Generated temporary password for this test: `yIgq#q:1f=KR`

#### Configuring MySQL
`vim /etc/my.cnf`
Adding:
```
[mysqld]
wait_timeout=3000
interactive_timeout=3000
max_allowed_packet=512M
validate_password_policy=LOW #This is to allow control123!
lower_case_table_names=0
```
Restart MySQL
`service mysqld restart`

Change password :
```
mysql -uroot -p
Enter password:
mysql>
ALTER USER 'root'@'localhost' IDENTIFIED BY 'control123!';
CREATE database riot_main;
```

#### Installing Mongo 3.4

`vim /etc/yum.repos.d/mongodb-org-3.4.repo`
```
[mongodb-org-3.4]
name=MongoDB Repository
baseurl=https://repo.mongodb.org/yum/redhat/$releasever/mongodb-org/3.4/x86_64/
gpgcheck=0
enabled=1
```
**Installing Mongo**
`yum install mongodb-org mongodb-org-server mongodb-org-shell mongodb-org-mongos mongodb-org-tools`

**Startup Mongo**
`service mongod start`

**Creating user**
`mongo admin`
```
use admin
db.system.users.remove({})
db.system.version.remove({})
db.dropUser("admin")
db.createRole( { role: "executeFunctions", privileges: [ { resource: { anyResource: true }, actions: [ "anyAction" ] } ], roles: [] } )
db.createUser({ user: "admin", pwd: "control123!", roles: ["userAdminAnyDatabase", "executeFunctions"] })
>exit
```
**Adding security**
`vim /etc/mongod.conf`
```
#  bindIp: 127.0.0.1
security:
	authorization: enabled
```
Setting up systemctl for Mongo
`systemctl daemon-reload`
`service mongod restart`

Test it:
`mongo -u admin -p control123! --authenticationDatabase admin`

### EMQTT INSTALLATION
Install Repository

`wget --content-disposition http://emqtt.io/downloads/latest/centos7`
`unzip emqttd-centos7-*.zip -d /opt/`

Adding the EMQTT Path
`export PATH=$PATH:/opt/emqttd/bin/`

Change API Management port
`sed -i "s/8080/8089/g" /opt/emqttd/etc/emq.conf`

Starting EMQTT
`emqttd start`

See status
`emqttd_ctl status`

### TOMCAT

#### Installing Tomcat 8

Installing Dependencies for Tomcat 8

`yum install alsa-lib copy-jdk-configs fontconfig fontpackages-filesystem giflib java-1.8.0-openjdk-headless javapackages-tools libICE libSM libX11 libX11-common libXau libXcomposite libXext libXfont libXi libXrender libXtst libfontenc libpng libxcb libxslt lksctp-tools python-javapackages python-lxml stix-fonts ttmkfdir tzdata-java xorg-x11-font-utils xorg-x11-fonts-Type1`

Adding User

```
sudo groupadd tomcat
sudo useradd -M -s /bin/nologin -g tomcat -d /opt/tomcat tomcat
wget http://apache.mirrors.ionfish.org/tomcat/tomcat-8/v8.0.47/bin/apache-tomcat-8.0.47.tar.gz
sudo mkdir /opt/tomcat
sudo tar xvf apache-tomcat-8.0*tar.gz -C /opt/tomcat --strip-components=1
```
Then we add and modify permissions
```
cd /opt/tomcat/
sudo chgrp -R tomcat conf
sudo chmod g+rwx conf
sudo chmod g+r conf/*
sudo chown -R tomcat webapps/ work/ temp/ logs/
```
Then we generate the starter script
```
sudo vi /etc/systemd/system/tomcat.service
```
With the following content
```
# Systemd unit file for tomcat
[Unit]
Description=Apache Tomcat Web Application Container
After=syslog.target network.target

[Service]
Type=forking

Environment=JAVA_HOME=/usr/lib/jvm/jre
Environment=CATALINA_PID=/opt/tomcat/temp/tomcat.pid
Environment=CATALINA_HOME=/opt/tomcat/
Environment=CATALINA_BASE=/opt/tomcat/
Environment='CATALINA_OPTS=-Xms512M -Xmx1024M -server -XX:+UseParallelGC'
Environment='JAVA_OPTS=-Djava.awt.headless=true -Djava.security.egd=file:/dev/./urandom'

ExecStart=/opt/tomcat/bin/startup.sh
ExecStop=/bin/kill -15 $MAINPID

User=tomcat
Group=tomcat

[Install]
WantedBy=multi-user.target
```
```
sudo systemctl daemon-reload
sudo systemctl start tomcat
sudo systemctl enable tomcat
```

### HTTPD
Start httpd
`yum install httpd`
`service httpd start`

Change file `vim /opt/tomcat/conf/server.xml`
```
<Connector port="8080" protocol="HTTP/1.1" connectionTimeout="20000" redirectPort="8443"
compression="on"
compressionMinSize="2048" noCompressionUserAgents="gozilla, traviata"
compressableMimeType="text/html,text/xml,application/json,text/plain"/>
```
Restart tomcat
`service tomcat restart`

Configure Proxy for API Rest Services vizix

`vim /etc/httpd/conf.d/vizix.conf`

With the following information
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
And restart HTTPD
`service httpd restart`

### Launch popDB
`cd ~/riot-core-services`
`yum install vizix-services-5.1.4-v5.1.4.x86_64.rpm`
`cd /usr/local/riot/services/`
`./popdb.sh gradle-default.properties platform-demo-mojix clean`

Stop Tomcat
`service tomcat stop`

Copy Parameters File
`cp /root/riot-core-services/build/tar/riot-core-services/linux-riot-core-services.xml /opt/tomcat/conf/Catalina/localhost/riot-core-services.xml`

Changing Driver for MySQL
```
sed -i "s/org.gjt.mm.mysql.Driver/com.mysql.jdbc.Driver/g" /opt/tomcat/conf/Catalina/localhost/riot-core-services.xml
```

Launch services
`service tomcat start`
See Logs (You will have to wait a few seconds before launching this command)
`tail -f /opt/tomcat/logs/riot-core-services.log`

At this point, ViZix Services should be working fine. You can browse the API endpoints using the following URL http://192.168.1.1/riot-core-services. Using your IP address.


## Build and Launch ViZix Composer UI

Installing tools to Build UI

```
cd ~/riot-core-ui
npm install grunt-cli bower -g
cp config-dist.json config-local.json
npm install grunt && npm install && bower install --allow-root
```
The following lines remove some functions that are not needed.
```
sed -i "s/gitinfo:/ nogitinfo:/g" ~/riot-core-ui/Gruntfile.js
sed -i "s/tar:/ nogitinfo:/g" ~/riot-core-ui/Gruntfile.js
sed -i "s/rpmbuild:/ nogitinfo:/g" ~/riot-core-ui/Gruntfile.js
```
Build the application

```
grunt dist
```
Some ERRORS will appear, as not every function was executed. Those can be ignored because it does not interrupt the building process.

Move files to HTTPD Home
```
rm -rf /var/www/html/*
cp -r ~/riot-core-ui/dist/* /var/www/html/
```
Change Config JS with the IP Address of the server you are using

`vim /var/www/html/scripts/config.js`

In this file, replace the `localhost:8080` for the IP Address of the server.
```
host: 'http://192.168.1.1/riot-core-services',
```

Where 192.168.1.1 is your IP Address.
At this point you can login to ViZix Composer using root/root as credentials.


### ViZix Composer

* The license file should be uploaded at this point.
* Configuration of CorebridgeMT and AlebridgeMT should be done (both in agent1)

## Installing Bridges

`cd ~/riot-core-bridges`
`yum install vizix-agent-5.1.4-v5.1.4.x86_64.rpm`

`vim /usr/local/riot/agent/configuration.yml`
Add the following information
```
httpPort: 9090
servicesHttpHost: localhost
servicesHttpPort: 80
servicesApikey: 7B4BCCDC
servicesContextPath: /riot-core-services
```

Start the Agent Container
`service agent start`
See Logs:
`tail -f /var/log/riot/agent.log`

* At this point you can turn on the bridges via ViZix Composer

## Building and Installing ViZix Flow

```
cd ~/vizix-flow
npm install -g --unsafe-perm node-red
npm install node-red-contrib-bcrypt
node-red &
```

#### Changing the Password

Introduce new Password

```
cd /usr/lib/node_modules/node-red
node -e "console.log(require('bcryptjs').hashSync(process.argv[1], 8));" m0j1xIncAdmin
```

Copy the hash generated. Example: `$2a$08$iusjxE4QOdcCdUKDVC2SieZw8lPrnRIiMQxH/z/NPslSXKd5/HDZ2`
The password used here is: `m0j1xIncAdmin`

Once it has started you can stop the process using: Ctrl+C
```
vim ~/.node-red/settings.js
```

Uncomment the following lines and add the hash given in the previous step.
```
adminAuth: {
    type: "credentials",
    users: [{
        username: "admin",
        password: "$2a$08$YKUYgnozYnFv3JuMvkgVKuTj/NhC0taqrOD8GqJ5vDXATspbAJQ2K",
        permissions: "*"
    }]
},
```
Make FLOW a CentOS service

`vim /etc/init.d/nodered`

Copy the following:

```
#! /bin/sh
# Starts and stops Node-RED
# /etc/init.d/nodered
### BEGIN INIT INFO
# Provides: node-red
# Required-Start: $syslog
# Required-Stop: $syslog
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: Node-RED initialisation
### END INIT INFO
# Can be downloaded and installed in one go by using this command
# sudo wget -O /tmp/download https://gist.github.com/bigmonkeyboy/9962293/download && sudo tar -zxf /tmp/download --strip-components 1 -C /etc/init.d && sudo chmod 755 /etc/init.d/nodered && sudo update-rc.d nodered defaults
# This runs as the user called pi - please change as you require
USER=root
# The log is written to here - please make sure your user has write permissions.
LOG=/var/log/node-red.log
#Load up node red when called
case "$1" in
start)
if pgrep ^node-red$ > /dev/null
then
echo "Node-RED already running."
else
echo "Starting Node-Red.."
touch $LOG
chown $USER:$USER $LOG
echo "" >> $LOG
echo "Node-RED service start: "$(date) >> $LOG
# su -l $USER -c "cd ~/.node-red && screen -dmS red node-red-pi --max-old-space-size=128"
# or
su -l $USER -c "node-red-pi --max-old-space-size=128 -u ~/.node-red >> $LOG &"
echo "Logging to "$LOG
fi
;;
stop)
echo "Stopping Node-Red.."
# su -l $USER -c "screen -S red -X quit"
# or
pkill -SIGINT ^node-red$
sleep 2
echo "" >> $LOG
echo "Node-RED service stop: "$(date) >> $LOG
;;
restart)
echo "Restarting Node-Red.."
$0 stop
sleep 2
$0 start
echo "Restarted."
;;
*)
echo "Usage: $0 {start|stop|restart}"
exit 1
esac
```
Add execution permissions

```
chmod +x /etc/init.d/nodered
chkconfig --add nodered
chkconfig nodered on
service nodered stop
service nodered start
```

Changing Splash Welcome Image for ViZix FLOW
```
cd  ~/vizix-flow/images
mv /usr/lib/node_modules/node-red/public/favicon.ico /usr/lib/node_modules/node-red/public/favicon.ico.original
cp favicon.ico /usr/lib/node_modules/node-red/public/
cp logo.png /usr/lib/node_modules/node-red/public/red/images/
cp mojix-splash.png /usr/lib/node_modules/node-red/public/red/images/
```

Updating the Context File
```
sed -i "s/Node-RED/ViZix Flow/g" /usr/lib/node_modules/node-red/red/api/theme.js
sed -i "s/node-red.png/logo.png/g" /usr/lib/node_modules/node-red/red/api/theme.js
```

Chaging the file
```
cd /usr/lib/node_modules/node-red/public/red/images/
mv node-red-256.png node-red-256B.png
mv mojix-splash.png node-red-256.png
```

Copying Mojix Nodes
```
cd ~/vizix-flow
npm install request
cp -R ~/vizix-flow/nodes/* ~/.node-red/nodes/
cp -R ~/vizix-flow/lib/* ~/.node-red/lib/
\cp -R ~/vizix-flow/node_modules/* ~/.node-red/node_modules/
```

Start ViZix Flow
```
service nodered stop
service nodered start
```

At this point you can log into ViZix Flow using http://192.168.1.1:1880
with the IP address used in the environment.
