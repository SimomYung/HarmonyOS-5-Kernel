# sysinfo()
Returns information on overall system statistics

## Synopsis
```c
#include <sys/sysinfo.h>

int sysinfo(struct sysinfo *info);
```

## Arguments
- **info**
Pointer to a *sysinfo* structure where the information to be stored. The *sysinfo* structure is defined as follows:

```c
struct sysinfo {
	long uptime;              /* Seconds since boot */
	unsigned long loads[3];   /* 1, 5, and 15 minute load averages */
	unsigned long totalram;   /* Total usable main memory size */
	unsigned long freeram;    /* Available memory size */
	unsigned long sharedram;  /* Amount of shared memory */
	unsigned long bufferram;  /* Memory used by buffers */
	unsigned long totalswap;  /* Total swap space size */
	unsigned long freeswap;   /* swap space still available */
	unsigned short procs, pad;/* Number of current processes */
	unsigned long totalhigh;  /* Total high memory size */
	unsigned long freehigh;   /* Available high memory size */
	unsigned mem_unit;        /* Memory unit size in bytes */
	char __reserved[256];
};
```
## Library
libc

## Description
The sysinfo() function gets the overall system statistics and returns them in the *info* structure. Only *totalram* and *freeram* members of the structure are filled at present.

## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
*info* is an invalid pointer.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO