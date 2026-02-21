# reboot()
Reboot the system

## Synopsis
```c
#include <unistd.h>
#include <sys/reboot.h>

int reboot(int cmd);
```

## Arguments
- **cmd**
The command ID.

## Library
libc

## Description
The reboot() function reboots the system. The *cmd* argument can have the following values:
- **RB_AUTOBOOT**
Restart the system.
&nbsp;
- **RB_HALT_SYSTEM**
Shutdown the system.
&nbsp;
- **RB_POWER_OFF**
Shutdown the system, same as RB_HALT_SYSTEM.
&nbsp;
Other commands are not supported.

## Returns
Call to reboot() will return.
&nbsp;
- **0**
Success.
- **-1**
An error occurred.

## Errors
- **ENOSYS**
The command specified by *cmd* is not supported.
&nbsp;
- **EPERM**
The calling process has insufficient privilege to call reboot().

## Example
&nbsp;

## Classification
UNIX

## Function Safety
TODO
