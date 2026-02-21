# getrandom()
Fill random bytes into buffer

## Synopsis
```c
#include <sys/random.h>

ssize_t getrandom(void *buf, size_t buflen, unsigned flags);
```

## Arguments 

- **buf**
The pointer to the buffer where this function fills the random bytes.
&nbsp;
- **buflen**
The size of the buffer pointed to by the argument *buf*.
&nbsp;
- **flags**
A bit mask that contains the control marks which can change the behavior of the function.


## Library
libc

## Description
The getrandom() function fills the buffer pointed to by *buf* with up to buflen random bytes. By default, getrandom() draws entropy from the urandom source (i.e., the same source as the /dev/urandom device). If the GRND\_RANDOM is set in the flags argument, then random bytes are drawn from the random source (i.e., the same source as the /dev/random device) instead of the urandom source.

The source will be initialized before registration.

## Returns

- **-1**
An error occurred.
&nbsp;

- **non-negative**
The number of the random bytes filled in the buffer.
  
## Errors

- **EFAULT**
The address referred to by *buf* is outside the accessible address space, or the source is not available.
&nbsp;
- **EINTR**
An invalid flag was specified in flags.
&nbsp;
- **ENOSYS**
The random/urandom source was not available.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
