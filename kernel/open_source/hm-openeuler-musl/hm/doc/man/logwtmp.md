# logwtmp()

logwtmp - append an entry to the wtmp file

## Synopsis

```c
#include <utmp.h>

void logwtmp(const char *line, const char *name, const char * host);
```

## Arguments 

- **line** : line of wtmp file
- **name** : username who login, e.g. "root"
- **host** : hostname who login, e.g. "10.102.12.32"


## Library

libc

## Description

logwtmp() function constructs a utmp structure with line, name, host, current time and current process ID. Then append the structure to the wtmp file by call updwtmp() function.


## Files

/var/log/wtmp

## Errors

N/A

## Example

N/A

## Classification
Not in POSIX.1-2001. Present on Solaris, NetBSD, and perhaps other systems.

## Function Safety
TODO
