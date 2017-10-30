// Cisco IOS router Commands
// Monitoring and Management
!
show terminal
show history
show process
show processes cpu history
show version
show diag
show inventory
show file systems
show ip arp
show cdp entry * protocol
show ip dhcp pool
show ip dhcp binding
show ip dhcp conflict
traceroute
clear counters serial0/0    //Reinicia el keepalive de la interfaz
config-register 0x2102      //Inicio del router con Proceso normal

// Configuración Miscelaneos
!
(config)# hostname [Nombre del equipo]
(config)# banner motd $[Mensaje del día]$
!
(config)# ip host [NOMBRE] [IPAddress]
!
//Habilitación de Autosecure
#auto secure

//Security
!
(config)#enable secret [PASSWORD]
(config)#security passwords min-length 10
(config)#username [USER] password [PASSWORD] privilege [0-15]
(config)#username [USER] secret [PASSWORD] privilege [0-15]
(config)#aaa new-model
(config)#aaa authentication login LOCAL_AUTH local
!
(config)#line console 0 / vty 0 5
(config-lin)#login authentication LOCAL_AUTH
(config-lin)#exec-timeout 5 0
(config-lin)#login block-for 300 attempt 2 within 120   //Bloque 300 segundos si intenta 5 veces en 120 seg
(config-lin)#security authentication failure rate 5 log //Además guarda la infor en el Syslog
!
(config)#service password-encryption
!
(config)#aaa accounting exec default start-stop group radius
!

// SNMP
// SNMP Server and Client
(config) snmp-server community (palabra) rw
(config) snmp-server enable
(config) snmp-server host
!
// Syslog Configuration
!
(config)#logging host [ip-address] transport udp port [port]
//Lo del puerto es por si se necesita un puerto en particular (Papertrail)
(config)#logging trap [level-debugging]
(config)#logging facility syslog
//Es bueno modificar la hora después de esto
#set clock hh:mm:ss
(config)#clock timezone GMT -4 0

//Better performance with Papertrail
no service sequence-numbers
no service timestamps debug uptime
no service timestamps log uptime
!
// Deshabilitar Servicios Globales Poco Utilizados
(config)#no service pad
(config)#no service finger
(config)#no service udp-small-server
(config)#no service tcp-small-server
(config)#no ip bootp server
(config)#no ip http server
(config)#no ip finger
(config)#no ip source-route
(config)#no ip gratuitous-arps
(config)#no cdp run
!
// Desahibilitar Servicios Interfaces Poco Utilizados
(config-if)#no ip redirects
(config-if)#no ip proxy-arp
(config-if)#no ip unreachables
(config-if)#no ip directed-broadcast
(config-if)#no ip mask-reply
(config-if)#no mop enabled
!
// Configuración SDM en Routers
(config)# ip http server
(config)# ip http secure-server
(config)# ip http authentication local

// Acceso SSH y telnet
(config)# username [USER] privilege 15 secret [PASSWORD]
(config)# line vty 0 4
(config-line)# privilege level 15
(config-line)# login local
(config-line)# transport input telnet ssh

// Manipulando la Flash
#show flash
#dir all
#copy tftp flash
(config)#tftp-server nvram:startup-config alias test //El router como SERVER FTP
#delete flash:[nombre_archivo]

// Recuperación de Contraseñas
#show version
#reload
>confreg 0x2142  //Configurar el router para NO ver el Password al BOOTEO
>reset
#copy startup-config running-config
(config)#enable secret [new_password]
(config)#username [new_user] password [new_password]
(config-if)#no shutdown
(config)#config-register 0x2102
#copy running-config startup-config

// Secure Device Manager SDM
#show flash  //Verificar la instalación de sdmconfig, xx.cfg y sdm.tar)
(config)#username [user] password [pass]  //Necesario
(config)#ip http secure-server            // Se habilita el http
#copy run start

//Copia de archivos de configuración MANUAL
#copy startup-config ftp://[username]:[password]@[ip_address]
(config)#ip ftp username [USER]
(config)#ip ftp password [PASSWORD]
#copy startup-config ftp://[ip_address]

//Copia de archivos de configuración AUTOMÁTICA
(config)#archive
(config-archive)#path ftp://[username]:[password]@[ip_address]/[nombre_archivo]
(config-archive)#path ftp://[ip_address]/[nombre_archivo]
(config-archive)#time-period [minutos]
(config-archive)#write-memory

#show archive
#configure replace ftp://[username]:[passw]@[ip_address]/[nombre_archivo]
#configure replace ftp://[ip_address]/[nombre_archivo]

//Logging Server Levels
(config)#no logging console
(config)#logging console <0-7>
(config)#logging buffered 4096 <0-7>
(config)#logging [ip_address]
#show logging
#terminal monitor
#terminal no monitor

// SSH Configuration
(config)#hostname R2    //Obligatorio el hostname
(config)#ip domain-name [DOMAIN]
(config)#crypto key generate rsa
(config)#username [miguel] secret [cisco]
(config)#line vty 0 4
(config-line)#transport input ssh
(config-line)#login local
(config)#ip ssh time-out 15
(config)#ip ssh authentication-retries 2

//Listas de Acceso
(config)#ip access-list standard [nombre_lista_std]
(config-std-nacl)#deny [ip_address] [wildcard] log
(config-std-nacl)#permit any
!
(config-if)#ip access-group [nombre_lista_std] in/out
(config)#ip access-list extended [nombre_lista_ext]
(config-ext-nacl)#deny [IP] [ip_ad_orig] [wildcard_ori] [ip_ad_dest] [wildcard_dest] [OPS]
(config-ext-nacl)#permit any any
!
(config-if)#ip access-group [nombre_lista_ext] in/out
(config-line)#access-class [nombre_lista_ext] in/out    //Aplicarlista de acceso a Linea de Consola

//DHCP
R1(config)#ip dhcp excluded-address [ip_ad_rango_inic] [ip_ad_rango_fina]
R1(config)#ip dhcp pool [nombre_del_pool]
R1(dhcp-config)#network [ip_bloq] [subnet_mask]
R1(dhcp-config)#dns-server [ip_address] [sec_ip_address]
R1(dhcp-config)#default-router [ip_address]
R2(config-if)#ip helper-address [ip_addres_R1]
#show ip dhcp pool
#show ip dhcp binding
#show ip dhcp conflict

//NAT
(config)#ip nat inside source static [inside] [global]
(config-if)#ip nat outside
(config-if)#ip nat inside
(config)#ip nat pool [pool-name] [firs_addres] [last_address] netmask [mask]
(config)#ip nat inside source list [access-list] pool [pool-name]
(config)#ip nat inside source list [access-list] pool [pool-name] overload //La mejora del anterior comando. Con OVERLOAD se natean muchas más direcciones usando las disponibles en el Pool
(config)#ip nat inside source list [access-list] interface [int] overload

//NAT Route Map Based
// ----- All configuration -----
//The purpose of this NATing is to NAT specific address (10.100.0.0/23) to (10.100.100.0/23) inside
//the same router to send traffic to an IPSec tunnel. And, the rest of the traffic to be sent to Internet
!
(config)#ip nat inside source route-map GLOBALROUTE interface [interface] overload
!
//This route map doest NOT has to be implemented in any interface (it is only for NAT)
(config)#route-map GLOBALROUTE permit 10
(config-route-map)#match ip address GLOBAL
!
//If traffic is going to the tunnel, it will go to the next route map
//The rest is going to the internet.
(config)#ip access-list extended GLOBAL
(config-ext-nacl)#remark Globlal IP Access to Internet NAT-T
(config-ext-nacl)#deny ip 10.100.0.0 0.0.1.255 10.200.3.0 0.0.0.255
(config-ext-nacl)#permit ip 10.100.0.0 0.0.1.255 any
!
(config)#ip nat inside source static 10.100.0.A 10.100.100.A route-map RUTAVPNROUTE
!
//RUTAVPN (Designed to classify traffic to the IPSec Tunnel)
//So the previous NAT instruction says that will NAT (1-1) the address always that the
//the route map returns a TRUE (destintation=10.200.3.0)
(config)#ip access-list extended RUTAVPN
(config-ext-nacl)#permit ip 10.100.161.0 0.0.0.255 10.200.3.0 0.0.0.255
(config-ext-nacl)#permit ip 10.100.0.0 0.0.1.255 10.200.3.0 0.0.0.255
!
(config)#route-map RUTAVPNROUTE permit 10
(config-route-map)#match ip address RUTAVPN
!
//Note: This route maps act as a NAT instruction. No #ip policy route-map is applied.
//Every packet is analized just before it goes out an interface.
//-------------------------------------

//Rate Limiting QoS
//Speed limiting - DSCP can also be applied
!
(config)#interface GigabitEthernet0/0.148
(config-if)#rate-limit input 4000000 750000 1500000 conform-action transmit exceed-action drop
(config-if)#rate-limit output 4000000 750000 1500000 conform-action transmit exceed-action drop
!

//Cambio del MTU
(config-if)#ip mtu 1400 //Por ejemplo

//Class of Service configuration Policy Framework
//Fase 1 - Reconocimiento de protocolos
R1(config)#class-map match-any [class-name]
//match-any clasifica paquetes siempre y cuando se cumplan al menos 1 de las condiciones siguientes
//match-all clasifica paquetes siempre y cuando se cumplan TODAS las condiciones siguientes
R1(config-cmap)#match protocol skype
R1(config-cmap)#match protocol rtp
R1(config-cmap)#match protocol rtcp
//We can match specific request from and to IP addresses and specific ports

R1(config)#access-list 100 permit tcp 172.0.0.0 0.255.255.255 host 190.15.15.15 eq www
R1(config-cmap)#match access-group 100
R1(config-cmap)#exit

//Fase 2 - Creamos el policy-map
R1(config)#policy-map [policy-map-name]
R1(config-pmap)#class [class-name]
R1(config-pmap-c)#bandwidth [kpbs]  //especifica el mínimo BW reservado
R1(config-pmap-c)#bandwidth percent [0-100]  //especifica el porcentaje mínimo reservado basado en el ancho de la interfaz (comando bandwidth)
R1(config-pmap-c)#bandwidth remaining percent [0-100]  //especifica el porcentaje mínimo reservado después de las otras reservas del mismo tipo
//Uso con priority percent
R1(config-pmap-c)#priority percent [0-100]  //Especifica el BW máximo para el flujo

// ------ IPV6
//Static IPv6 address (64 bits in prefix)
(config-if)#ipv6 address 2001:1111:0000:aaaa:2222:1111:0:a /64
//Dynamic EUI-64 (64 bits in prefix)
(config-if)#ipv6 address 2001:1111:0000:aaaa::/64 eui-64

// --- RIP Routing Information Protocol ---
// RIP Interfaces pasivas
!
(config)#router rip
(config-router)#version 2
(config-router)#passive-interface default
(config-router)#no passive-interface s0/0/0
!
//SEGURIDAD RIP
(config)#key chain RIP_KEY
(config-keychain)#key 1
(config-keychain-key)#key-string [PASSWORD]
(config-if)#ip rip authentication mode md5
(config-if)#ip rip authentication key-chain RIP_KEY
!

//RIP for IPv6 a.k.a. RIPng
(config)#ipv6 router rip [NAME]
(config-rtr)exit
(config)#interface fa0/0
(config-if)#ipv6 rip [NAME] enable  //en cada interfaz que quieras habilitar

// --- EIGRP Enhanced Interior Gateway Routing Protocol ---
//SEGURIDAD EIGRP
(config)#key chain EIGRP_KEY
(config-keychain)#key 1
(config-keychain-key)#key-string [PASSWORD]
(config)#intserface serial0/0/0
(config-if)#ip authentication mode eigrp 1 md5
(config-if)#ip authentication key-chain eigrp 1 EIGRP_KEY

//EIGRP Configuración Básica
(config)#router eigrp [AS]
(config-router)# eigrp router-id 192.168.1.255 //Dirección de Identificación (no necesariamente un IP Válida)
(config-router)#network 192.168.0.0  //Red de clase
(config-router)#network 172.16.0.0 0.0.0.3 //Red con wildcard
(config-router)#network 0.0.0.0 255.255.255.255 //todas las Interfaces

//EIGRP Interfaces pasivas
(config-router)#passive-interface default //Deshabilita todas las interfaces para envio de HELLOS
(config-router)#no passive-interface se0/1 //Empieza a enviar HELLOS por esta interfaz

//Desahibilitar los HELLOS con interfaz pasiva simplemente hace que se dejen de enviar paquetes (recomendable con la LAN)
//Pero si por otra interfaz se advierte una red que tiene una interfaz pasiva, esta seguirá siendo enrutada.
//Basic EIGRP GNS3

//Cambio de tiempos intervalo HELLO EIGRP
(config)#interface serial0/0
(config-if)#ip hello-interval eigrp [AS] 10
(config-if)#ip hold-time eigrp [AS] 30
//Podría causar inestabilidad en la red. Pero es permitido.

//EIGRP Stub Configuration
(config-router)#eigrp stub   //Activa stub por defecto. CAEN LAS adyacencias

//Verificación en router vecino
#show ip eigrp neighbor detail

//Redistribución en EIGRP de OSPF
(config)#router eigrp [AS]
(config-router)#redistribute ospf [PROCESS-ID] metric <?>  //Consultar parametros
(config-router)#redistribute ospf [PROCESS-ID] metric 1500 100 255 1 1500
(config-router)#redistribute ospf [PROCESS-ID] metric 1500 100 255 1 1500 included-connected

//IPv6 EIGRP
(config)#ipv6 router eigrp [AS]
(config-router)#redistribute ospf [PROCESS-ID] metric 1500 100 255 1 1500

// --- OSPF Open Shortest Path First --- //
//SEGURIDAD OSPF
(config)#interface serial0/0/0
(config-if)#ip ospf message-digest-key 1 md5 [PASSWORD]
(config-if)#ip ospf authentication message-digest
(config-if)#exit
(config)#router ospf 10
(config-router)#area 0 authentication message-diggest

//OSPF
#clear ip ospf processes    //Ojo que pierde las adyacencias por segundos
(config-if)#ip ospf priority [0-XX]  //Cambiamos la prioridad de cada enlace para la elección de DR y BDR (Se necesita limpiar el proceso después)
(config-if)#ip ospf hello-interval [seconds]
(config-if)#ip ospf dead-interval [seconds]
(config)#router ospf [process_ID]
(config-router)#router-id [A.A.A.A]
!
#show ip ospf database
#show ip ospf database router
#show ip ospf database summary
#show ip ospf database external
!
#show ip ospf [PROCESS-ID]
#show ip ospf interface [int] //Aqui se puede ver el COSTO
!
(config-router)#auto-cost reference-bandwidth 10000000  //Por defecto es 10^7 y la división es 1 a partir del FEthernet
(config-if)#bandwidth 10000 //Se puede cambiar la velocidad aunque no se soporte eso
(config-if)#ip ospf cost [COST]
!
//Sumarización de OSPF en ABR
(config)#router ospf [PROCESS-ID]
(config-router)#area 1 range [A.A.A.A] [M.M.M.M] // La mascara con los bits sumarizados
(config-router)#area 2 range [A.A.A.A] [M.M.M.M] // Podemos introducir sumarizaciones por AREAs
!
//Sumarización de OSPF en ASBR
(config)#router ospf [PROCESS-ID]
(config-router)#summary-address [A.A.A.A] [M.M.M.M] //Son todas las direcciones a sumarizar del otro SA.
!
//OSPF Stub Configuration
(config)#router ospf [PROCESS-ID]
(config-router)#area [X] stub   //En ambos extremos de una area (ABR - Router)
!
//Totally Stub Area OSPF
(config-router)#area [X] stub no-summary //En el extremo ABR
(config-router)#area [X] stub            //En el extremo Stub Router
!
//Esto no se puede hacer desde el router principal porque se pierde conectividad. Cae la adyacencia.

// OSPFv3 for IPv6
(config)#ipv6 unicast-routing
(config)#ipv6 router ospf 1
(config-rtr)#router-id 1.1.1.1
(config-rtr)#passive-interface Lo0
!
(config-if)#ipv6 ospf 1 area [X]
(config-if)#ipv6 ospf neighbor FE80::2    //Para los enlaces NBMA Network
!
#show ipv6 ospf neighbor
#show ipv6 route ospf
#show ipv6 ospf database
!
//OSPFv3 for IPv6 - New approach - Sobreescribe el anteior
(config)#router ospfv3 1
(config-router)#router-id 1.1.1.1
(config-router)#passive-interface Lo0
!
(config-if)#ospfv3 1 ipv6 area [X]    //Habilita el proceso por interfaz
(config-if)#ospfv3 1 ipv4 area [X]
!
#show ospfv3 neighbor
#show ip route ospfv3
#show ospfv3 database inter-area prefix
// Ejemplo de configuración paralela de IPv4 con IPv6
!
(config)#router ospfv3 1
(config-router)#address-family ipv4 unicast
(config-router-af)#router-id 1.1.1.1
(config-router)#address-family ipv6 unicast
(config-router-af)#passive-interface Lo0
(config-router-af)#router-id 1.1.1.1
!
(config)#router eigrip [AS]
(config-router)#distance eigrp 90 170
!
//OSPF Redistribución
(config)#router ospf [PROCESS-ID]
(config-router)#redistribute static
(config-router)#default-information originate   //Esto distribuye las estáticas
(config-router)#default-information originate always //SIEMPRE distribuye una salida
!
//Redistribución en OSPF de EIGRP
(config)#router ospf [PROCESS-ID]
(config-router)#redistribute eigrp [AS_Number] subnets
(config-router)#redistribute eigrp [AS_Number] metric-type [E1/E2] subnets
(config-router)#redistribute eigro [AS_Number] subnets metric-type [E1/E2]

//Filtrar direcciones en Redistribución de EIGRP a OSPF
(config)#access-list 7 permit [A.A.A.A] [W.W.W.W]
(config)#access-list 7 permit [B.B.B.B] [W.W.W.W]
(config-router)#router ospf [PROCESS-ID]
(config-router)#redistribute eigrp [AS]
(config-router)#distribute-list 7 out eigrp [AS]

//Filtrar direcciones en Redistribución de OSPF a EIGRP
(config)#access-list 7 permit [A.A.A.A] [W.W.W.W]  //También con DENY
(config)#access-list 7 permit [B.B.B.B] [W.W.W.W]
(config-router)#router eigrp [AS]
(config-router)#redistribute ospf [PROCESS-ID] metric 1500 100 255 1 1500
(config-router)#distribute-list 7 out ospf [PROCESS-ID]

//Filtrar direcciones con PREFIX list
ip prefix-list ANY_ROUTE permit 0.0.0.0/0 le 32
ip prefix-list ONLY_DEFAULT permit 0.0.0.0/0
!
ip prefix-list [NAME] seq 5 [permit/deny] 192.168.0.0/16 le 20
ip prefix-list [NAME] seq 5 [permit/deny] 192.168.0.0/16 ge 18
ip prefix-list [NAME] seq 5 [permit/deny] 192.168.0.0/16 le 24 ge 19    //Hacen match las 192.168.X.X /19/20/21/22/23/24
!
(config-router)#distribute-list prefix [NAME] out [PROTOCOL] [AS/PROCESS-ID]
!
#show ip prefix-list  detail
!

// --- ROUTE MAP Configuration
! //En la misma linea funciona como OR
(config)#route-map [NAME] permit 10
(config-route-map)#match ip address ACL1 ACL2 ACL3
!
! //En diferentes líneas funciona como AND
(config)#route-map [NAME] deny [SEQ_Number]
(config-route-map)#match ACL1
(config-route-map)#match interface fastethernet0/0
(config-route-map)#match metric 3
!

//Ejemplo de Redistribución con Route MAP
(config)#ip prefix-list PFL1 permit 10.10.11.0/24
(config)#ip prefix-list PFL1 permit 10.10.12.0/24
!
(config)#route-map rm_into_OSPF permit 10
(config-route-map)#match ip address prefix-list PFL1
(config-route-map)#set metric 25
(config-route-map)#set metric-type type-1
!
(config)#router ospf 1
(config-router)#redistribute eigrp 100 subnets route-map rm_into_OSPF
!

//PBR Configuration
(config)#ip access-list extended PBR_ACL
(config-ext-nacl)#permit ip host 192.168.110.2 any
!
(config)#route-map PBR_RM
(config-route-map)#match ip address PBR_ACL
(config-route-map)#set ip next-hop 10.10.10.1
!
(config-if)#ip policy route-map PBR_RM   //En la interfaz de entrada del trafico
!
#show route-map
#debug ip policy

// Configuración IPSec completo Router
// Fase 1 - ISAKMP Sa (Security Association)

R1(config)#crypto isakmp policy 1
R1(config-isakmp)#authentication pre-share
R1(config-isakmp)#encr aes 128
R1(config-isakmp)#hash sha
R1(config-isakmp)#authentication pre-share
R1(config-isakmp)#group 1
R1(config-isakmp)#lifetime 86400
R1(config-isakmp)#exit

//Peer has to be reachability - creamos el key
R1(config)#crypto isakmp key [password] address 190.104.13.195

//Identify interest traffic (source and dest)
R1(config)#access-list 100 permit ip 10.100.100.0 0.0.0.255 10.200.3.0 0.0.0.255
//El retorno del sitio remoto debería tener todo invertido
R1(config)#access-list 100 permit ip 10.200.3.0 0.0.0.255 10.100.100.0 0.0.0.255

//Creamos el transform-set de IPSec para determinar cómo se encriptará
//Aquí usamos el (?) para ver los algoritmos.
//AH no encripta pero hace integrity check con TODO.
//ESP encripta el payload pero no hace integrity check con las direcciones IP del tunel
R1(config)#crypto ipsec transform-set MYSET esp-sha-hmac esp-aes
R1(cfg-crypto-trans)#exit
R1(config)#

//Ahora mexclamos todos los ingredientes
R1(config)#crypto map MYMAP 1 ipsec-isakmp
% NOTE: This new crypto map will remain disabled until a peer
	and a valid access list have been configured.

R1(config-crypto-map)#set transform-set MYSET
R1(config-crypto-map)#set peer 190.104.13.195
R1(config-crypto-map)#match address 100
R1(config-crypto-map)#exit

//Aplicamos a la interfaz de salida
R1(config)#int se2/0
R1(config-if)#crypto map MYMAP
*Jul  3 23:00:47.275: %CRYPTO-6-ISAKMP_ON_OFF: ISAKMP is ON
R1(config-if)#end

//Verification
#show crypto map
#show crypto ipsec sa
#show crypto session

// ------- PPP authentication ------- //




// ------- pptp
//Creación de PPTP VPNDN Server en IOS Router
(config)#vpdn enable
(config)#vpdn-group 1
(config-vpdn)#access-dialin
(config-vpdn-acc-in)#protocol pptp
(config-vpdn-acc-in)#Virtual-Template1
(config-vpdn-acc-in)#exit

//Creamos el pool de direcciones para las conexiones
(config)#ip local pool [NAME] 192.168.1.10 192.168.1.50

//Se debe tener usuario en el router
(config)#username mcallejas password cisco123

//Interfaz de conexión
(config)#interface Virtual-Template1
(config-if)#peer default ip address pool [NAME]
(config-if)#ip unnumbered FastEthernet1 //De esta interfaz se usará la dirección de traducción
(config-if)#no keepalive //Para cerrar conexiones que no estén activas
(config-if)#ppp encrypt mppe auto
(config-if)#ppp authentication pap chap ms-chap


// ----- Frame Relay Configuration
// CHAPTER: Frame Relay

//Configuracion con Inverse ARP
// Router LPZ -- FR -- SCZ
LPZ(config)#interface serial0/0
LPZ(config-if)#ip address 210.0.0.1 255.255.255.0
LPZ(config-if)#encapsulation frame-relay
LPZ(config-if)#frame-relay interface-dlci 120
LPZ(config-if)#frame-relay lmi-type cisco
LPZ(config-if)#no shutdown
!
FR_SWITCH(config)#frame-relay switching
FR_SWITCH(config)#interface serial0/0 //contra LPZ
FR_SWITCH(config-if)#no ip address
FR_SWITCH(config-if)#encapsulation frame-relay
FR_SWITCH(config-if)#frame-relay intf-type dce
FR_SWITCH(config-if)#frame-relay lmi-type cisco
FR_SWITCH(config-if)#clock rate 64000
FR_SWITCH(config-if)#frame-relay route 120 interface serial0/1 210  //Serial a SCZ
FR_SWITCH(config-if)#no shutdown
!
FR_SWITCH(config)#interface serial0/1 //contra SCZ
FR_SWITCH(config-if)#no ip address
FR_SWITCH(config-if)#encapsulation frame-relay
FR_SWITCH(config-if)#frame-relay intf-type dce
FR_SWITCH(config-if)#frame-relay lmi-type cisco
FR_SWITCH(config-if)#clock rate 64000
FR_SWITCH(config-if)#frame-relay route 210 interface serial0/0 120  //Serial a LPZ
FR_SWITCH(config-if)#no shutdown
!
SCZ(config)#interface serial0/0
SCZ(config-if)#ip address 210.0.0.2 255.255.255.0
SCZ(config-if)#encapsulation frame-relay
SCZ(config-if)#frame-relay interface-dlci 210
SCZ(config-if)#frame-relay lmi-type cisco
SCZ(config-if)#no shutdown

//Configuración con MAP
// Router LPZ -- FR -- SCZ
LPZ(config)#interface serial0/0
LPZ(config-if)#ip address 210.0.0.1 255.255.255.0
LPZ(config-if)#encapsulation frame-relay
LPZ(config-if)#no frame-relay inverse arp
LPZ(config-if)#frame-relay lmi-type cisco
LPZ(config-if)#frame-relay map ip 210.0.0.2 120 [broadcast]
LPZ(config-if)#no shutdown

FR_SWITCH(config)#//Remains the same as with Inverse ARP

SCZ(config)#interface serial0/0
SCZ(config-if)#ip address 210.0.0.2 255.255.255.0
SCZ(config-if)#encapsulation frame-relay
SCZ(config-if)#no frame-relay inverse arp
SCZ(config-if)#frame-relay lmi-type cisco
SCZ(config-if)#frame-relay map ip 210.0.0.1 210 [broadcast]
SCZ(config-if)#no shutdown
!
