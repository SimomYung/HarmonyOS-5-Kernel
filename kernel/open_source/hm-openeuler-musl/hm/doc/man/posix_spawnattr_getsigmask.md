# posix\_spawnattr\_getsigmask(), posix\_spawnattr\_setsigmask()
Get and set the spawn-sigmask attribute of a spawn attributes object

## Synopsis
```c
#include <signal.h>
#include <spawn.h>

int posix_spawnattr_getsigmask(const posix_spawnattr_t *restrict attr,
			       sigset_t *restrict sigmask);
int posix_spawnattr_setsigmask(posix_spawnattr_t *restrict attr,
			       const sigset_t *restrict sigmask);
```

## Arguments
- **attr**
The attributes object.
&nbsp;
- **sigmask**
The sigmask attribute to be set or get.

## Library
libc

## Description

The posix\_spawnattr\_getsigmask() function shall get the sigmask attribute from the attributes object represented by **attr**.

The posix\_spawnattr\_setsigmask() function shall set the sigmask attribute to the attributes object represented by **attr**.

## Returns

**0**
The functions succeed.
&nbsp;
**errno**
Error happens. See **Errors** for detail.

## Errors

- **EINVAL**(for posix\_spawnattr\_getsigmask() and posix\_spawnattr\_setsigmask())
The value of attr is invalid.
&nbsp;
- **EINVAL**(for posix\_spawnattr\_setsigmask())
The argument *sigmask* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
