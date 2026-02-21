# umask()
Set the mode creation mask of a file

## Synopsis
```c
#include <sys/types.h>
#include <sys/stat.h>

mode_t umask(mode_t mask);
```

## Arguments 
- **mask**
The argument *mask* & 0777 is used to create file mode mask.

## Library
libc

## Description
The function umask() is a stub implementation for now.

## Returns
The function umask() returns the argument *mask* as a stub implementation.

## Errors
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
