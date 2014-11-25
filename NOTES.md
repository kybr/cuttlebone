## MTU

From Wikipedia:

    A larger MTU brings greater efficiency because each network packet carries more
    user data while protocol overheads, such as headers or underlying per-packet
    delays, remain fixed; the resulting higher efficiency means an improvement in
    bulk protocol throughput. A larger MTU also means processing of fewer packets
    for the same amount of data. In some systems, per-packet-processing can be a
    critical performance limitation.

Cuttlebone chops your state into chunks of N bytes. You are expected to choose
an N that less than the [MTU][] of your network. To discover this value use the
ifconfig command. Here's an example:

    $ ifconfig
    lo0: flags=8049<UP,LOOPBACK,RUNNING,MULTICAST> mtu 16384
      options=3<RXCSUM,TXCSUM>
      inet6 ::1 prefixlen 128
      inet 127.0.0.1 netmask 0xff000000
      inet6 fe80::1%lo0 prefixlen 64 scopeid 0x1
      nd6 options=1<PERFORMNUD>
    gif0: flags=8010<POINTOPOINT,MULTICAST> mtu 1280
    stf0: flags=0<> mtu 1280
    en0: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
      ether 10:9a:dd:aa:e3:ea
      inet6 fe80::129a:ddff:feaa:e3ea%en0 prefixlen 64 scopeid 0x4
      inet 192.168.0.109 netmask 0xffffff00 broadcast 192.168.0.255
      nd6 options=1<PERFORMNUD>
      media: autoselect
      status: active
    p2p0: flags=8843<UP,BROADCAST,RUNNING,SIMPLEX,MULTICAST> mtu 2304
      ether 02:9a:dd:aa:e3:ea
      media: autoselect
      status: inactive

Above, the interface named en0 is the primary ethernet interface which is
probably what you'll want to broadcast from. Note that it seems to have an
[MTU][] of 1500. This is a very common setting for ethernet interfaces. That's
why the default packet size for Cuttlebone is 1400. 


If your network supports it, you may increase the [MTU][] to increase
efficiency and throughput. To accomplish this, you must first determine the
largest [MTU][] that your router will support. The largest I've seen is 9212.
This so called [Jumbo frame][] 

set the [MTU][] of the 


IP: 20
UDP: 8
CB: 16
    44
MTU: 1500
MAX: 1456

[MTU]: http://en.wikipedia.org/wiki/Maximum_transmission_unit
[Jumbo frame]: http://en.wikipedia.org/wiki/Jumbo_frame
