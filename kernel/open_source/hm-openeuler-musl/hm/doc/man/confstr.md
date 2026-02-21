# confstr()
Get configuration dependent string variables

## Synopsis
```c
#include <unistd.h>

size_t confstr(int name, char *buf, size_t len);
```

## Arguments

- **name**
The *name* argument is the system variable to be queried.
&nbsp;
- **buf**
The *buf* you want to receive.
&nbsp;
- **len**
The size of the *buf*.

## Library
libc

## Description
The confstr() function gets the value of configuration-dependent string variables. The *name* argument is the system variable to be queried. The following variables are supported:
- **_CS_PATH**
A value for the PATH variable which indicates where all the POSIX.2 standard utilities can be found.

&nbsp;
If *buf* is not NULL and *len* is not zero, confstr() copies the value of the string to *buf* truncated to *len* \- 1 bytes if necessary, with a null byte ('\\0') as terminator. This can be detected by comparing the return value of confstr() against *len*.
&nbsp;
If *len* is zero and *buf* is NULL, confstr() just returns the value as defined below.

## Returns

- **the number of bytes**
If *name* is a valid configuration variable, confstr() returns the number of bytes (including the terminating null byte) that would be required to hold the entire value of that variable. This value may be greater than *len*, which means that the value in *buf* is truncated.
&nbsp;
- **0**
If *name* is a valid configuration variable, but that variable does not have a value, then confstr() returns 0. If *name* does not correspond to a valid configuration variable, confstr() returns 0, and errno is set to EINVAL.

## Errors

- **EINVAL**
The value of *name* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
