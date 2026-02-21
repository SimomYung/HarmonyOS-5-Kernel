# aligned\_alloc()
Allocate aligned memory

## Synopsis
```c
#include <stdlib.h>

void *aligned_alloc(size_t alignment, size_t size);
```

## Arguments

- **alignment**
The alignment of the allocated memory.
&nbsp;
- **size**
The size of the allocated memory.


## Library
libc

## Description
The aligned\_alloc() function allocates a chunk of uninitialized memory with *size* bytes. The address of the allocated memory will be a multiple of *alignment*. *alignment* must be a power of two.

## Returns

- **pointer to the memory**
If success, the pointer to the allocated memory will be returned.
&nbsp;
- **NULL**
An error occurred.

## Errors

- **ENOMEM**
There is insufficient memory to fulfill the allocation request.
&nbsp;
- **EINVAL**
*alignment* is invalid.

## Example
&nbsp;

## Classification
C11

## Function Safety
TODO
