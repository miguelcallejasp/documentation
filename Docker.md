Docker Scripting
### Ubuntu Set Up
```
check_ubuntu_version
apt-get update
DEBIAN_FRONTEND='noninteractive' apt-get -y -o Dpkg::Options::='--force-confdef' -o Dpkg::Options::='--force-confold' upgrade
DEBIAN_FRONTEND='noninteractive' apt-get -y -o Dpkg::Options::='--force-confdef' -o Dpkg::Options::='--force-confold' dist-upgrade
apt-get install apt-transport-https ca-certificates curl vim software-properties-common htop nano sysfsutils -y
echo 'kernel/mm/transparent_hugepage/enabled = never' >> /etc/sysfs.conf
echo 'kernel/mm/transparent_hugepage/defrag = never' >> /etc/sysfs.conf
echo 'never' > /sys/kernel/mm/transparent_hugepage/enabled
echo 'never' > /sys/kernel/mm/transparent_hugepage/defrag
```

###  Docker Installation
```
check_ubuntu_version
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
add-apt-repository 'deb [arch=amd64] https://download.docker.com/linux/ubuntu xenial stable'
apt-get update
apt-get install docker-ce=17.06.0~ce-0~ubuntu -y
```

### Utilities
```
curl -L https://github.com/docker/compose/releases/download/1.16.1/docker-compose-Linux-x86_64 > /usr/bin/docker-compose
chmod +x /usr/bin/docker-compose
```
```
curl -L https://github.com/bcicen/ctop/releases/download/v0.6.1/ctop-0.6.1-linux-amd64 > /usr/bin/ctop
chmod +x /usr/bin/ctop
```

### Docker Inspect
Cleaning Clear Images and Docker Process
`docker rm -v $(docker ps -a -q -f status=exited) && docker rmi $(docker images -q)`

Inspect from application
`docker inspect --format='{{json .NetworkSettings.Networks}}' [repository]`

Get IP from application
`docker inspect --format='{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' [repository]`

Save images to tar to have them in different places
`docker save mojix/riot-core-ui:release_5.0.0_RC10 > ui.tar`
`cp ui.tar /tmp/`
`docker run --rm -p 81:80 -v /tmp:/usr/share/nginx/html nginx:alpine`

On the other side
`wget http://<ip-address>:81/ui.tar`
`docker load --input ui.tar`

### Docker Networking
```
docker network ls
docker network inspect [network-name]
docker network disconnect [network-name] [name]
```
Create a network bridge
`docker network create -d bridge [network-name]`

Attach containers to a bridge
`docker network connect [network-name] [name]`

### CAdvisor
```
    docker run                                      \
      --volume=/:/rootfs:ro                         \
      --volume=/var/run:/var/run:rw                 \
      --volume=/sys:/sys:ro                         \
      --volume=/var/lib/docker/:/var/lib/docker:ro  \
      --publish=8080:8080                           \
      --detach=true                                 \
      --name=cadvisor                               \
      google/cadvisor:latest
```
