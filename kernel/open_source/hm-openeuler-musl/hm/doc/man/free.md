# free()
Free dynamic memory

## Synopsis
```c
#include <stdlib.h>

void free(void *ptr);
```

## Arguments

- **ptr**
The pointer to the memory which is to be freed.

## Library
libc

## Description
The free() function frees the memory pointed to by *ptr*. If *ptr* is NULL, the call will return immediately with no operation. *ptr* must be obtained by dynamic memory allocator. Passing *ptr* which has been freed to free() results in undefined behavior.

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
