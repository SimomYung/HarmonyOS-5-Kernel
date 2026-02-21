# ioctl()
Control a file or device

## Synopsis
```c
#include <sys/ioctl.h>

int ioctl(int fd, int req, ...);
```
## Arguments

- **fd**
The file descriptor of the opened file (or device).
&nbsp;
- **req**
The control request code (it depends on different device).
&nbsp;
- **...**
Some other parameters and their sizes can be here.

## Library
libc

## Description
Function ioctl() controls and performs multiple operating characteristics on a file or device (indicated by *fd*).

If fd is a tty device, supported request include:
TIOCGWINSZ - Only support TTY devices.

If fd is a socket, supported request include:

SIOCSIFVLAN - Create or delete a VLAN.

SIOCGIFCONF - Return a list of interface (transport layer) addresses.

SIOCGIFFLAGS, SIOCSIFFLAGS
Get or set the active flag word of the device. ifr\_flags contains a bit mask. Setting the active flag word is a privileged operation, but any process may read it.

SIOCGIFADDR, SIOCSIFADDR
Get or set the address of the device using ifr\_addr. Setting the interface address is a privileged operation.
For compatibility, only AF\_INET addresses are accepted or returned.

SIOCGIFDSTADDR, SIOCSIFDSTADDR
Get or set the destination address of a point-to-point device using ifr\_dstaddr. For compatibility,
only AF\_INET addresses are accepted or returned. Setting the destination address is a privileged operation.

SIOCGIFBRDADDR
Get the broadcast address for a device using ifr\_brdaddr. For compatibility,
only AF\_INET addresses are accepted or returned. Setting the broadcast address is a privileged operation.

SIOCGIFNETMASK, SIOCSIFNETMASK
Get or set the network mask for a device using ifr\_netmask. For compatibility, only AF\_INET addresses
are accepted or returned. Setting the network mask is a privileged operation.

SIOCGIFMTU, SIOCSIFMTU
Get or set the MTU (Maximum Transfer Unit) of a device using ifr\_mtu. Setting the MTU is a privileged operation.
Setting the MTU to too small values may cause kernel crashes.

SIOCGIFHWADDR, SIOCSIFHWADDR
Get or set the hardware address of a device using ifr\_hwaddr. The hardware address is specified in a struct sockaddr.
sa\_family contains the ARPHRD\_(device type), sa\_data the L2 hardware address starting from byte 0.
Setting the hardware address is a privileged operation.

SIOCADDRT, SIOCDELRT - Set or unset network route.

SIOCETHTOOL - Used for ethtool interface.

SIOCGIFMETRIC - Get metric.

SIOCGIFTXQLEN - Get the tx queue length.

SIOCGIFINDEX - Get the net device index by net device name.

SIOCGIFNAME - Get the net device name by net device index.

SIOCBONDENSLAVE - Enslave a device to the bond.

SIOCBONDRELEASE - Release a slave from the bond.

SIOCBONDCHANGEACTIVE - Update to a new active slave.

SIOCGSTAMP - Get the timestamp for packet socket.

FIONBIO - Mark socket nonblocking.

TIOCOUTQ - Get the recv queue size for unix socket.

FIONREAD - Get the number of bytes that are immediately available for reading

TUNSETIFF - Create a tun/tap net device and map it to a AF\_TUN socket.

TUNSETPERSIST - Set true to retain a tun/tap net device when close corresponding socket, false on the contrary.

TUNSETOWNER - Set the owner of a AF\_TUN socket.

SIOCGARP - Get ARP table entry.

SIOCSARP - Set ARP table entry.

SIOCDARP - delete ARP table entry.

## Returns

- **Non-negative number**
Function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.

## Errors
- **EBADF**
The input *fd* is invalid.
&nbsp;

- **EINVAL**
The request code is invalid.
&nbsp;
- **EFAULT**
The requests is not appropriate, i.e., request messages length exceeds the limitation.
&nbsp;
- **ENOTTY**
The request code is not supported according to the input file descriptor.
&nbsp;
- **ENOMEM**
There is not enough memory during the function (unlikely to happen).

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
