# posix\_spawnattr\_getsigdefault(), posix\_spawnattr\_setsigdefault()
Get and set the spawn-sigdefault attribute of a spawn attributes object

## Synopsis
```c
#include <signal.h>
#include <spawn.h>

int posix_spawnattr_getsigdefault(const posix_spawnattr_t *restrict attr,
				  sigset_t *restrict sigdefault);
int posix_spawnattr_setsigdefault(posix_spawnattr_t *restrict attr,
				  const sigset_t *restrict sigdefault);
```

## Arguments
- **attr**
The attributes object.
&nbsp;
- **sigdefault**
The sigdefault attribute to be set or get.

## Library
libc

## Description

The posix\_spawnattr\_getsigdefault() function shall get the sigdefault attribute from the attributes object represented by **attr**.

The posix\_spawnattr\_setsigdefault() function shall set the sigdefault attribute to the attributes object represented by **attr**.

## Returns

**0**
The functions succeed.
&nbsp;
**errno**
Error happens. See **Errors** for detail.

## Errors

- **EINVAL**(for posix\_spawnattr\_getsigdefault() and posix\_spawnattr\_setsigdefault())
The value of attr is invalid.
&nbsp;
- **EINVAL**(for posix\_spawnattr\_setsigdefault())
The argument *sigdefault* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
