# pclose()
Close a pipe stream to or from a process opened by popen()

## Synopsis
```c
#include <stdio.h>

int pclose(FILE *stream);
```

## Arguments 
- **stream**
The argument *stream* points to a pipe stream.

## Library
libc

## Description
The pclose() function closes a stream opened by *popen()*, wait for the command to terminate and return the termination status of the process which is running the command interpreter.

## Returns
Close the pipe stream successfully, the termination status of the command interpreter is returned.
&nbsp;
An error occurs, -1 is returned and *errno* is set.

## Errors
- **ECHILD**
The status of the child process can not be obtained.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
