# calloc()
Allocate dynamic memory

## Synopsis
```c
#include <stdlib.h>

void *calloc(size_t nmemb, size_t size);
```

## Arguments

- **nmemb**
The number of the elements.
&nbsp;
- **size**
The size of the element.

## Library
libc

## Description
The calloc() function allocates a chunk of memory for an array which has *nmemb* elements of *size* bytes each, and return a pointer to the allocated memory. The memory will be initialized by zero. If *nmemb* is zero, the call will succeed and a pointer which can be passed to free() will be returned, but the pointer can not be used to store the data of user. If the multiplication of *nmemb* and *size* exceeds the integer limit, the call will fail.

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
