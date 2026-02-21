# posix\_memalign()
Allocate aligned memory

## Synopsis
```c
#include <stdlib.h>

int posix_memalign(void **memptr, size_t alignment, size_t size);
```

## Arguments

- **memptr**
The pointer to the address of the allocated memory.
&nbsp;
- **alignment**
The alignment of the allocated memory.
&nbsp;
- **size**
The size of the allocated memory.


## Library
libc

## Description
The posix\_memaligned() function allocates a chunk of uninitialized memory with *size* bytes and place the address of the allocated memory in *\*memptr*. *alignment* must be a power of two and a multiple of *sizeof(void \*)*. The address of the allocated memory will be a multiple of *alignment*. If *size* is zero, the call will succeed and a pointer which can be passed to free() will be returned.

## Returns

- **0**
Success.
&nbsp;
- **ENOMEM**
There is insufficient memory to fulfill the allocation request.
&nbsp;
- **EINVAL**
*alignment* is invalid.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
