# getlogin_r()
Get the name of the user who logged in.

## Synopsis
```c
#include <unistd.h>

int getlogin_r(char *buf, size_t bufsize);
```

## Arguments

- **buf**
*buf* is a pointer to a buffer.
&nbsp;
- **bufsize**
*bufsize* is the length of *buf*.

## Library
libc

## Description
The getlogin_r() function gets the name by the environment variable named *LOGNAME* to *buf* with `bufsize` bytes.

## Returns

- **0**
The user name is filled into *buf* successfully.
&nbsp;
- nonzero
The details are the same as errors below.

## Errors

- **ENXIO**
No users logged in.
&nbsp;
- **ERANGE**
The size of user name with the terminating sign is larger than *bufsize*.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
