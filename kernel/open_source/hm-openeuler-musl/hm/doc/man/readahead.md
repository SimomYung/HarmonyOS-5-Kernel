# readahead()
Preread file data to page cache

## Synopsis
```c
#include <fcntl.h>

ssize_t readahead(int fd, off_t pos, size_t len);
```

## Arguments

- **fd**
The file descriptor of the target file.
&nbsp;
- **pos**
The start point of the pre-read operation
&nbsp;
- **len**
The number of bytes (also, the length of data) that need to be pre-read.

## Library
libc

## Description
Function readahead() performs pre-read on a specified file. It is worth noting that function will return 0 if fd refers to a directory.

## Returns

- **0**
Function finished successfully.

- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors

- **EBADF**
The input *fd* is invalid.
&nbsp;
- **EINVAL**
The node or node's mapping (related to *fd*) is NULL.
&nbsp;

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
