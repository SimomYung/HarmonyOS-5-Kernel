# stpncpy()
Copy a string with a given length

## Synopsis
```c
#include <strings.h>

char *stpncpy(char *dest, const char *src, size_t n);
```

## Arguments

- **dest**
A pointer to the destination buffer.
&nbsp;
- **src**
A pointer to the source buffer.
&nbsp;
- **n**
The number of characters that are needed to copy.

## Library
libc

## Description
Copy exactly *n* characters including the terminating null byte ('\0') from string *src* to *dest*. If the length of *src* is smaller than *n*, the remaining spaces in *dest* are filled with null bytes ('\0') . If the length of *src* is equal to or larger than *n*, the string in *dest* is not ended by a terminating null byte ('\0').
&nbsp;

When *src* and *dest* overlap, the result is not correct. The programmer must ensure the buffer size of *dest* is larger enough to store at least *n* characters.

## Returns
A pointer to the the terminating null byte ('\0') in *dest*. If *dest* is not null-terminated, return *dest* + *n*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO