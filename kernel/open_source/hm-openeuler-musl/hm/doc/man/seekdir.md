# seekdir()
Set the offset of the directory being read next

## Synopsis
```c
#include <dirent.h>

void seekdir(DIR *dir, long off);
```

## Arguments

- **dir**
The name of the directory that will be operated.
&nbsp;
- **off**
The offset (position) of next directory in the stream.

## Library
libc

## Description
The seekdir() function will reset the *dir*'s offset to *off*, similar to *lseek(fd, off, SEEK_SET)*. Such that the readdir() will start from *off*.
## Returns

The function has no return value. While it will fail if the parameters *dir* or *off* were invalid.

## Errors
N/A
&nbsp;

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
