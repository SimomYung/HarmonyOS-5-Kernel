# memmem()
Find a substring

## Synopsis
```c
#define _GNU_SOURCE
#include <strings.h>

void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
```

## Arguments

- **haystack**
A pointer to the memory to find *needle*.
&nbsp;
- **haystacklen**
The length of *haystack*.
&nbsp;
- **needle**
A pointer to the string that is needed to find.
&nbsp;
- **needlelen**
The length of *needle*.

## Library
libc

## Description
memmem() scans the memory area *haystack* and returns the first location when the substring *needle* is found.

## Returns

- **not NULL**
A pointer to the matched *needle* in *haystack*.
&nbsp;
- **NULL**
*needle* could not be found.

## Errors
N/A

## Example
&nbsp;

## Classification
UNIX

## Function Safety
TODO