# strerror_r()
Return the description string of an error number

## Synopsis
```c
#include <strings.h>

int strerror_r(int errnum, char *buf, size_t buflen);
```

## Arguments

- **errnum**
The error number.
&nbsp;
- **buf**
A pointer to the buffer that stores the description string.
&nbsp;
- **buflen**
The length of the description string to be returned.

## Library
libc

## Description
Return the description string of an error number *errnum*. The string and its length are returned in *buf* and *buflen* respectively.

## Returns

- **0**
Success.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EINVAL**
Argument *errnum* is invalid.
&nbsp;

- **ERANGE**
Argument *buf* is too small to store the description string.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
