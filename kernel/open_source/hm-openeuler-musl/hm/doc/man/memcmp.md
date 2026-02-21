# memcmp()
Check whether the contents of two memory areas are equal or not

## Synopsis
```c
#include <strings.h>

int memcmp(const void *s1, const void *s2, size_t n);
```

## Arguments

- **s1**
A pointer to one of the memory area.
&nbsp;
- **s2**
A pointer to the other one of the memory area.
&nbsp;
- **n**
The number of bytes that are needed to check.

## Library
libc

## Description
Check the first *n* bytes of *s1* and *s2* to see if they are equal or not. If they are equal or *n* is 0 then memcmp() returns 0. Otherwise a nonzero result is returned.

## Returns
- **0**
The contents of two memory areas are equal or *n* is 0.
&nbsp;

- **non-zero**
The contents of two memory areas are not equal. The sign is calculated by the difference of the first pair of bytes that are different in *s1* and *s2*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO