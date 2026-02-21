# rewinddir()
Reset the stream of a directory

## Synopsis
```c
#include <dirent.h>

void rewinddir(DIR *dir);
```

## Arguments

- **dir**
The name of the directory that will be operated.

## Library
libc

## Description
The rewinddir() function resets the *dir*'s offset to 0, similar to *lseek(fd, 0, SEEK_SET)*.

## Returns
The function has no return value. While it will fail if the parameter *dir* is invalid.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
