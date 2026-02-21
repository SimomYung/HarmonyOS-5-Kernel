# posix\_spawnattr\_getflags(), posix\_spawnattr\_setflags()
Get and set the spawn-flags attribute of a spawn attributes object

## Synopsis
```c
#include <spawn.h>

int posix_spawnattr_getflags(const posix_spawnattr_t *restrict attr,
			     short *restrict flags);
int posix_spawnattr_setflags(posix_spawnattr_t *attr, short flags);
```

## Arguments
- **attr**
The attributes object.
&nbsp;
- **flags**
The flags attribute to be set or get.

Following flags are supported:

POSIX\_SPAWN\_RESETIDS

POSIX\_SPAWN\_SETPGROUP

POSIX\_SPAWN\_SETSIGDEF

POSIX\_SPAWN\_SETSIGMASK

POSIX\_SPAWN\_SETSID

## Library
libc

## Description

The posix\_spawnattr\_getflags() function shall get the flags attribute from the attributes object represented by **attr**.

The posix\_spawnattr\_setflags() function shall set the flags attribute to the attributes object represented by **attr**.

## Returns

**0**
The functions succeed.
&nbsp;
**errno**
Error happens. See **Errors** for detail.

## Errors

- **EINVAL**(for posix\_spawnattr\_getflags() and posix\_spawnattr\_setflags())
The value of attr is invalid.
&nbsp;
- **EINVAL**(for posix\_spawnattr\_setflags())
The argument *flags* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
