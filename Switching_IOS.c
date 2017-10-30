
#show platform tcam utilization
#show sdm prefer
#show mac addres-table aging-time [sec]    //Time to refresh the MAC Table
#show lldp
#show lldp neighbors
#show power inline   //POE status

// Activar IPv6 en switching
(config)#sdm prefer dual-ipv4-and-ipv6 default   //Reboot is needed
(config)#ipv6 unicast-routing

//Change Aging time to MAC Address-table
(config)#mac addres-table aging-time 600   //Default is 300 (5 min)

//Configuring lLDP on switching
(config)#lldp run
(config-if)#no lldp enable //Disabling LLDP IEEE 802.1AB on a specific int.

//Implementing POE
(config-if)#power inline (auto/never)

//-------Spanning
//Show commands
#show spanning-tree vlan [number]
#show spanning-tree root
#show spanning-tree blocked ports
#show spanning-tree summary

//Changing the root priority
(config)#spanning-tree vlan [vlan-id] priority [priority]
(config)#spanning-tree vlan [vlan-id] root primary
(config)#spanning-tree vlan [vlan-id] root secondary

//RapidSpanning-tree (not default activated)
(config)#spanning-tree mode rapid-pvst


//Port Security Switching
#show port-security
#show port-security address
#show port-security interface fast0/X

(config-if)#switchport access vlan 20
(config-if)#switchport mode access
(config-if)#switchport port-security
(config-if)#switchport port-security violation restrict
(config-if)#swithcport port-security maximum 2

shuwdown (default): port goes into errdisable state
protect: silently the switch drops the traffic of host
restrict: same as protect BUT shows a log message in console

(config-if)#switchport port-security mac-address 00aa.cccc.12ab vlan access
(config-if)#switchport port-security mac-address sticky
