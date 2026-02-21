# perror()
Write an error message to the standard error

## Synopsis
```c
#include <stdio.h>

void perror(const char *s);
```

## Arguments 
- **s**
A pointer points to a string which will be written.

## Library
libc

## Description
The perror() function maps the error number accessed through *errno* to a language-dependent error message that shall be written to the standard error stream.

## Returns
N/A

## Errors
- **EAGAIN**
The *O_NONBLOCK* flag is set for the file descriptor, and the write operation will be delayed.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
