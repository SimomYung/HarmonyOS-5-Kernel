# uname()
Get name and information about current kernel

## Synopsis
```c
#include <sys/utsname.h>

int uname(struct utsname *buf);
```

## Arguments
- **buf**
Pointer to an *utsname* structure where the system information returned. The *utsname* structure is defined as follows:

```c
struct utsname {
	char sysname[65];    /* Operating system name */
	char nodename[65];   /* Name within "some implementation-defined
	                      network" */
	char release[65];    /* Operating system release */
	char version[65];    /* Operating system version */
	char machine[65];    /* Hardware identifier */
#ifdef _GNU_SOURCE
	char domainname[65]; /* NIS or YP domain name */
#else
	char __domainname[65];
#endif
};
```

## Library
libc

## Description
The uname() function gets the system information and returns it in the *buf* argument.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
*buf* is an invalid pointer.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO