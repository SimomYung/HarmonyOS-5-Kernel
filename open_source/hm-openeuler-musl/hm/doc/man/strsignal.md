# strsignal()
Return the description string of a signal

## Synopsis
```c
#include <strings.h>

char *strsignal(int sig);

extern const char * const sys_siglist[];
```

## Arguments

- **sig**
The signal number.

## Library
libc

## Description
Return the description string of a signal number *sig*. The string is only valid before the next call of strsignal().
&nbsp;

All the description strings are stored in sys_siglist. However, strsignal() is recommended rather than using sys_siglist directly.

## Returns
Return the corresponding description string of *sig* or an unknown string if *sig* is invalid.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO