# closedir()
Close a directory

## Synopsis
```c
#include <dirent.h>
#include <sys/types.h>

int closedir(DIR *dir);
```

## Arguments

- **dir**
Pointer of the associated directory which will be operated.

## Library
libc

## Description
The closedir() function closes the directory pointed by the input parameter (i.e., *dir*).

## Returns

- **0**
The directory is successfully closed by closedir().
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of how closedir() failed will be explained in *Errors*.
&nbsp;


## Errors

- **EINVAL**
Invalid input parameters, for example the *dir* is NULL.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
