### Windows Scripting
#### MTU customization

MTU Statically Change
MTU=1350 is a good minimum to fix and troubleshoot VPN GRE Tunnel issues with segmentation

```
C:\WINDOWS\system32>netsh interface ipv4 show subinterfaces

   MTU  MediaSenseState   Bytes In  Bytes Out  Interface
------  ---------------  ---------  ---------  -------------
  1500                1  5931210420  289815316  Wi-Fi 2
  1500                5          0          0  Local Area Connection* 13
  1500                1          0    7565521  VirtualBox Host-Only Network
  1500                2    4246464    2614581  Ethernet 5
```

`C:\WINDOWS\system32>netsh interface ipv4 set subinterface "Ethernet 5" mtu=1350 store=persisten`

Ok.

`C:\WINDOWS\system32>netsh interface ipv4 show subinterfaces`
```
   MTU  MediaSenseState   Bytes In  Bytes Out  Interface
------  ---------------  ---------  ---------  -------------
  1500                1  5933367784  290230229  Wi-Fi 2
  1500                5          0          0  Local Area Connection* 13
  1500                1          0    7582011  VirtualBox Host-Only Network
  1350                2    4246464    2614581  Ethernet 5
  ```
