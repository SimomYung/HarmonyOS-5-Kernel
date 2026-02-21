# getentropy()
Fill a buffer with random bytes

## Synopsis
```c
#include <unistd.h>

int getentropy(void *buffer, size_t len);
```

## Arguments

- **buffer**
The pointer to the buffer where this function fills the random bytes.
&nbsp;
- **len**
The length of the buffer pointed to by the argument *buffer*.

## Library
libc

## Description

The getentropy() function writes length bytes of random data to the buffer.
&nbsp;
The max value for the length argument is 256.

## Returns

- **0**
This function finished successfully.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EIO**
The *len* is greater than 256.
&nbsp;
- **EFAULT**
The address referred to *buffer* is outside the accessible address space, or the source is not available.
&nbsp;
- **ENOSYS**
The getrandom() system call is not supported.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
