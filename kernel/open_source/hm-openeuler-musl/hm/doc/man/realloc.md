# realloc()
Change the size of dynamic memory

## Synopsis
```c
#include <stdlib.h>

void *realloc(void *ptr, size_t size);
```

## Arguments

- **ptr**
The pointer to the memory whose size will be changed.
&nbsp;
- **size**
The new required size of the memory.

## Library
libc

## Description
The realloc() function changes the size of the memory pointed to by *ptr* to *size* bytes. The contents of the old memory will be copied to the new memory. The copied region will begin at the start of the old memory and continue for the size which is the minimum of the old memory size and the new memory size. After the copy is done, the old memory will be freed. If the new size is larger than the old size, the added region of the new memory will be uninitialized.
&nbsp;
If *ptr* is NULL, the call will be equivalent to calling malloc() with *size*. If *size* is zero and *ptr* is not NULL, the call will be equivalent to calling free() with *ptr*. *ptr* must be obtained by dynamic memory allocator.

## Returns

- **pointer to the memory**
If the call succeeds, the pointer to the new memory will be returned.
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
