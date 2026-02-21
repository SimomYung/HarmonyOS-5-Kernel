# getpt()
Open a pseudo-terminal master

## Synopsis
```c
#define _GNU_SOURCE
#include <stdlib.h>

int getpt(void);
```

## Arguments

N/A

## Library
libc

## Description
The getpt() function opens a pseudo-terminal master and returns its file descriptor.

## Returns

- **fd**
A file descriptor.
&nbsp;

- **-1**
An error occurred.

## Errors

Same as the open() function.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
