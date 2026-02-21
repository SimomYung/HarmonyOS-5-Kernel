# stpcpy()
Copy a string

## Synopsis
```c
#include <strings.h>

char *stpcpy(char *dest, const char *src);
```

## Arguments

- **dest**
A pointer to the destination buffer.
&nbsp;
- **src**
A pointer to the source buffer.

## Library
libc

## Description
Copy a string including the terminating null byte ('\0') from *src* to *dest*. When *src* and *dest* overlap, the result is not correct. The programmer must ensure the buffer size of *dest* is larger enough.

## Returns
A pointer to the the terminating null byte ('\0') in *dest*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO