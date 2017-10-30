OS Unix Linux
### FTP Server
`sudo -s launchctl load -w /System/Library/LaunchDaemons/ftp.plist`
`sudo -s launchctl unload -w /System/Library/LaunchDaemons/ftp.plist`

### SSH Key Creation
`ssh-keygen -t rsa`

#### Verificacion de Distribución
`lsb_release -a`

#### SSH to a oKexAlgorithms
`ssh -oKexAlgorithms=+diffie-hellman-group1-sha1 administrator@172.16.11.1`

## Partition and Disk Allocation

Ver si el disco necesita rezising
`lsblk`
Ver el sistema de archivos que tienen las particiones
`sudo file -s /dev/xvd*`

Formatear un disco con xfs y montarlo
`apt-get update && apt-get install xfsprogs -y`
`lsblk`
`mkfs.xfs /dev/xxxxxxx`
`vim /etc/fstab`
```
/dev/xxxxxxx /data xfs defaults 0 0
```
`mount -a`

### Para Montar BINDS
`vim /etc/fstab`
```
/data/docker /var/lib/docker none bind
```
`mount -a`
### Para desmontar solamente el bind
`umount /var/lib/docker`

#### Information Commands
`df -h`
`df -i` For Ulimits. The amount of files that a system can have (XFS)

## Route Manipulation
```
netstat -nr
route delete default
route add default 192.168.0.1
```

### Add SUDOERS to Ubuntu

`adduser miguel`
`chmod -aG sudo miguel`

#### User Management
List all Users
`cut -d: -f1 /etc/passwd`
Add User
`adduser miguelcallejasp`

Agregar usuarios a grupo SUDO
`adduser miguelcallejasp sudo`
`login miguelcallejasp`

LDAP Query Test
`ldapsearch -vvv  -h 10.100.1.3  -D fflores@tierconnect.com -x -w fflores123
-b "dn=fcalsina@tierconnect.com,ou=coderoad,dc=tierconnect,dc=com"`

### OpenConnect for AnyConnect and IPSec Client Ubuntu
`sudo apt-get install -y network-manager-openconnect-gnome network-manager-openconnect network-manager-vpnc network-manager-vpnc-gnome vpnc vpnc-scripts`

### IP Networking Management
 `iptables -L`

Add and delete interfaces
`ip addr show eth0`
`ip link show`
`ip address show`

Para actualizar una interfaz eth0 se cambia en
`/etc/network/interfaces`
```
auto eth0
iface eth0 inet static
  address 147.75.104.251
  netmask 255.255.255.254
  gateway 147.75.104.250
  dns-nameservers 8.8.8.8 8.8.4.4
```
`sudo ifdown eth0 && sudo ifup eth0`

Update IP address using IPROUTE2
This is not PERSISTEN. Directly configured in System and lost on reboot

`sudo ip address add 192.168.0.10/24 dev eth1`
`sudo ip link set eth1 up`

Crear un loopback temporal (lost on reboot) on Linux

```
sudo ip link add dummy0 type dummy
sudo ip address add 192.168.1.10/24 dev dummy0
sudo ip link set dummy0 up
sudo ip link set dummy down
```
Interfaces dummy siempre están UP (you can ping it) pero no aparece en la
tabla de enrutamiento hasta que se la encienda.

#### Routing capable Linux distribution
`sudo apt-get install iproute2`
`sysctl net.ipv4.ip_forward`

#if =1; then it is routing capable
`more /proc/sys/net/ipv4/ip_forward`
#if =1; then it is routing capable
#If =0 (not routing capable Linux) then you activate it: (NOT PERSISTENT)

`sudo sysctl -w net.ipv4.ip_forward=1`
`echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward`

Habilitar IPv4 Forwarding PERSISTENT
`vim /etc/sysctl.conf`
Uncomment the next line to enable packet forwarding for IPv4
`net.ipv4.ip_forward=1`

#### Agregar ruta estática en Linux
`sudo ip route add default via 172.16.0.1`

### VPNC for FEDORA
`su -c 'rpm -Uvh http://download.fedoraproject.org/pub/epel/7/x86_64/e/epel-release-7-8.noarch.rpm'`
`yum install vpnc`

VPN IPSec
`cat /etc/vpnc/default.conf`
```
  IPSec gateway vpn.hostname.example
  IPSec ID DefaultRAGroup
  IPSec secret GROUPPASS
  Xauth username username
  Xauth password my-password
``` 
Conectar
 `vpnc`
Desconectar VPNC
  `vpnc-disconnect`

### Asterisk
`watch "asterisk -vvvvvrx 'core show channels' | egrep \"(call|channel)\""`
`watch -n 1 "sudo asterisk -vvvvvrx 'core show channels' | grep call"`

```
sudo apt-get install ufw
ufw allow ssh
ufw allow http
ufw allow https
ufw allow snmp
ufw allow 1194/udp  #port and protocol
ufw enable
ufw status
```
