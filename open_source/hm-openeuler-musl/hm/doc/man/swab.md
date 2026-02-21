# swab()
Swap every pair of adjacent bytes

## Synopsis
```c
#define _XOPEN_SOURCE
#include <unistd.h>

void swab(const void *from, void *to, ssize_t n);
```

## Arguments

- **from**
A pointer to the source buffer.
&nbsp;
- **to**
A pointer to the destination buffer.
&nbsp;
- **n**
The number of characters that are needed to copy and swap.

## Library
libc

## Description
swab() copies *n* bytes from *from* to *to*, swaps every pair of adjacent bytes. It does nothing when *n* is negative or zero. It handles the first *n*-1 bytes if n is positive and odd, and the processing behavior of the last byte is undefined.

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
