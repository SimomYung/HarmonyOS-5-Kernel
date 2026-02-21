# pipe()
Create an interprocess channel

## Synopsis
```c
#include <unistd.h>

int pipe(int fildes[2]);
```

## Arguments
- **fildes**
The array of file descriptor. It contains 2 elements, *fildes[0]* is used for read and *fildes[1]* is used for write.

## Library
libc

## Description
The *pipe()* function creates a pipe and places two file descriptor in the array *fildes[2]* which refer to the open file descriptor for read and write of pipe.

## Returns
- **0**
Create a pipe successfully, 0 is returned.
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set.

## Errors
- **EFAULT**
The *fildes* is not valid.
&nbsp;
- **EMFILE**
All or all but one of the file descriptor available to the process are currently open.
&nbsp;
- **ENFILE**
Too many files opened in the system that exceeds the limit of the system.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

