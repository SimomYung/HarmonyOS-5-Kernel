# tee()
Re-direct the standard input file (stream) to another out file

## Synopsis
```c
#include <fcntl.h>

ssize_t tee(int src, int dest, size_t len, unsigned flags);
```

## Arguments

- **src**
The source file (or stream).
&nbsp;
- **dest**
The destination of the data re-direct to.
&nbsp;
- **len**
The size (length) of the file.
&nbsp;
- **flags**
The flgas to set different types of operations, such as: SPLICE\_F\_NONBLOCK.

## Library
libc

## Description
Function tee() re-directs a standard input file to a standard output file. More specifically, the operation will be done through pipe.

## Returns

- **Positive number**
Function finished successfully, the length of data will be returned.
&nbsp;
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.

## Errors

- **EPIPE**
The input or output pipe is not ready.
&nbsp;
- **EINVAL**
Failed to obtain the pipe of source file and target file.
&nbsp;

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
