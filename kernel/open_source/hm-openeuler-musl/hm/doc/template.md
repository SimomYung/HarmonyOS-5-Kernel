# access()
Check to see if a file or directory can be accessed

## Synopsis
```c
#include <unistd.h>

int access(const char *pathname, int mode);
```

## Arguments 

- **pathname**
The path to the file or directory that you want to access.
&nbsp;
- **mode**
The access mode you want to check.


## Library
libc

## Description
The access() function checks to see if the file or directory specified by path exists and if it can be accesssed with the file acess permission given by amode.

## Returns

- **0**
The file or directory exists and can be accessed with the specified mode.
&nbsp;

- **-1**
An error occurred.
  
## Errors

- **EACCES**
The permissions...
&nbsp;
- **EINTR**
This function was interrupted by a signal.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
