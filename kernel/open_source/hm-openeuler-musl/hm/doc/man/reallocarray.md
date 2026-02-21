# reallocarray()
Memory allocation and deallocation function

## Synopsis
```c
#include <malloc.h>

void *reallocarray(void *p, size_t nmemb, size_t elem_size);

```

## Arguments

- **p**
The pointer to the memory whose size will be changed.
&nbsp;
- **nmemb**
The size of new memory should be sufficient to contain the nmemb elements.
&nbsp;
- **elem_size**
And size of each element will be elem_size.

## Library
libc

## Description
The reallocarray() function allocates memory of which the size is calculated by multiplication of *nmemb* and *elem_size*. Overflow is checked on allocation.

## Returns

- **pointer to the memory**
If the call succeeds, the pointer to the new memory will be returned.
&nbsp;

- **NULL**
An error occurred.

## Errors

- **ENOMEM**
There is insufficient memory to fulfill the allocation request.
&nbsp;

## Example
&nbsp;

# Classification
POSIX 1003.1-2017

## Function Safety
TODO
