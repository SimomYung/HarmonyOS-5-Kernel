# tempnam()
Create a unique name for a temporary file

## Synopsis
```c
#include <stdio.h>

char *tempnam(const char *dir, const char *pfx);
```

## Arguments

- **dir**
The path of a directory to create the temporary file.
&nbsp;
- **pfx**
The prefix part of the file name of the temporary file.

## Library
libc

## Description
tempnam() returns a pointer to a unique temporary file name. If the argument *dir* is valid, it is used as the directory of the temporary file name. If the argument *dir* is null, a default directory name is used to create the temporary file name. The filename suffix of the pathname generated will start with *pfx* in case *pfx* is a non-NULL string of at most five bytes.

## Returns
On success, the tempnam() function returns a pointer to a unique temporary filename which is allocated by malloc and needs to be freed. On error, NULL is returned and errno is set to indicate the error.

## Errors

- **ENAMETOOLONG**
The total len of the pathname is too long.
&nbsp;
- **ENOMEM**
Insufficient memory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
