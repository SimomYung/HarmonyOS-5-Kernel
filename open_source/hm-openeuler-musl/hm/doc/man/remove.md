# remove()
Delete a file or directory

## Synopsis
```c
#include <stdio.h>

int remove(const char *path);
```

## Arguments

- **path**
Path of the target file or directory.

## Library
libc

## Description
Function remove() deletes a file or directory specified by the input *path*. Currently, name referred to socket and FIFO is not supported.
## Returns

- **0**
Function finished successfully.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors

- **EINVAL**
The *path* is invalid.
&nbsp;
- **ENOTEMPTY**
The directory that *path* refers to is not empty.
&nbsp;
- **ENOMEM**
There is not enough memory during the function processing (unlikely to happen).
&nbsp;


## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
