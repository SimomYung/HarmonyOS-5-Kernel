# rewind()
Move the file position indicator to the beginning of the file

## Synopsis
```c
#include <stdio.h>

void rewind(FILE *f);
```
## Arguments

- **f**
The stream for the file to operate.

## Library
libc

## Description
The rewind() function moves the file position indicator to the beginning of the file. It is same with function fseek(stream, 0L, SEEK_SET).

## Returns
N/A

## Errors
N/A

## Example
&nbsp;
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
