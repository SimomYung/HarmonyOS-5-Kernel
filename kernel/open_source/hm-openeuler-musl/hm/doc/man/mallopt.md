# mallopt()
Set parameters of memory allocator

## Synopsis
```c
#include <malloc.h>

int mallopt(int param, int value);
```

## Arguments

- **param**
The parameter of the memory allocator.
&nbsp;
- **value**
The new value for *param*.

## Library
libc

## Description
The mallopt() function sets the value of the parameters of the memory allocator. *param* can be one of the following values:
&nbsp;
- **M\_TRIM\_THRESHOLD**
When the amount of the free memory at the top of heap is larger than the value of M\_TRIM\_THRESHOLD, sbrk() will be called by free() to trim the heap, so that the memory can be released back to the system. The default value of M\_TRIM\_THRESHOLD is 2*1024*1024 bytes.
&nbsp;
- **M\_MMAP\_THRESHOLD**
When the free list of the memory allocator can not satisfy the memory allocation, the value of M\_MMAP\_THRESHOLD specifies the limit greater than which mmap() will be called rather than sbrk() to allocate memory from system. The default value of M\_MMAP\_THRESHOLD is 256*1024 bytes.
&nbsp;
It is not supported by now to modify these parameters using environment variables.

## Returns

- **1**
Success.
&nbsp;
- **0**
An error occurred.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
