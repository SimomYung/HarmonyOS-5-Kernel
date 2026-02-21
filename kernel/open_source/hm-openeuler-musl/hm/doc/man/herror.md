# herror()
Print the error message associated with the current value of h\_errno on stderr

## Synopsis
```c
#include <netdb.h>

void herror(const char *s);
```

## Arguments

- **s**
Error description specified by users.

## Library
libc

## Description
The herror() function prints the error message associated with the current value of h\_errno on stderr.

## Returns
N/A

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
