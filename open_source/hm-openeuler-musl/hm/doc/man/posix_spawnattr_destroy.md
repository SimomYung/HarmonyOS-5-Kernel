# posix\_spawnattr\_destroy(), posix\_spawnattr\_init()
Destroy and initialize spawn attributes object

## Synopsis
```c
#include <spawn.h>

int posix_spawnattr_destroy(posix_spawnattr_t *attr);
int posix_spawnattr_init(posix_spawnattr_t *attr);
```

## Arguments
- **attr**
The attributes object.

## Library
libc

## Description

The posix\_spawnattr\_init() function shall initialize a spawn attributes object **attr** with the default
values of all attributes. If **attr** has been initialized already, the result is undefined.

The posix\_spawnattr\_destroy() function shall destroy a spawn attributes object **attr**. It can be initialized
again after destroyed.

## Returns

**0**
The functions succeed.
&nbsp;
**errno**
Error happens. See **Errors** for detail.

## Errors

- **ENOMEM**(for posix\_spawnattr\_init())
No memory for initializing attr.
&nbsp;
- **EINVAL**(for posix\_spawnattr\_destroy())
The argument *attr* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
