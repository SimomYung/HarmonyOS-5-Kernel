# malloc()
Allocate dynamic memory

## Synopsis
```c
#include <stdlib.h>

void *malloc(size_t size);
```

## Arguments

- **size**
The required size of the memory.

## Library
libc

## Description
The malloc() function allocates a chunk of uninitialized memory with *size* bytes and return a pointer to the allocated memory. If *size* is zero, the call will succeed and a pointer which can be passed to free() will be returned, but the pointer can not be used to store the data of user.

## Returns

- **pointer to the memory**
If the call succeeds, the pointer to the allocated memory will be returned.
&nbsp;
- **NULL**
An error occurred.

## Errors

- **ENOMEM**
There is insufficient memory to fulfill the allocation request.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
