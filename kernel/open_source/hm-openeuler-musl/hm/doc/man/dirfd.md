# dirfd()
Get the fd of a directory

## Synopsis
```c
#include <dirent.h>
#include <sys/types.h>

int dirfd(DIR *dir);
```

## Arguments

- **dir**
Pointer of the directory which will be operated.

## Library
libc

## Description
The dirfd() function obtains the corresponding fd (internally descriptor) of *dir* and then return it.

## Returns

- **Non-negative integer**
The fd of the directory pointed by *dir*.
&nbsp;
- **Negative integer**
Operation failed since unexpected situations happened, more details of the failure will be explained in *Errors*.
&nbsp;


## Errors

- **EINVAL**
Invalid input parameters, for example the *dir* is NULL.
&nbsp;

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
