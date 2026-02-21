# chdir()
Change the current working directory

## Synopsis

```c
#include <unisted.h>

int chdir(const char *path);
```

## Arguments

- **path**
The new current working directory.
&nbsp;


## Library
libc

## Description
The function chdir() changes the current working directory of the calling process.


## Returns

- **0**
On success, this function return 0.
&nbsp;

- **-1**
On failure, this function return -1, and errno is set.

## Errors

- **EACCESS**
The file is permission denied.
&nbsp;
- **ENOENT**
The file does not exist.
&nbsp;
- **ENAMETOOLONG**
The *path* is too long.
&nbsp;
- **EINVAL**
The *path* is not valid.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **ENOTDIR**
The *path* is not a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
