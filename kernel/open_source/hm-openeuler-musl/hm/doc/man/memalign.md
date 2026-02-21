# memalign()
Allocate aligned memory

## Synopsis
```c
#include <malloc.h>

void *memalign(size_t alignment, size_t size);
```

## Arguments
- **alignment**
The alignment that you want to use for the memory. This must be a multiple of *sizeof(void \*)*.
&nbsp;
- **size**
The amount of memory you want to allocate, in bytes.

## Library
libc

## Description
The memalign() function allocates *size* bytes aligned on a boundary specified by *alignment*.

## Returns
A pointer to the allocated block, or NULL if an error occurred (errno is set).

## Errors
- **EINVAL**
The value of *alignment* isn't a multiple of *sizeof(void \*)*.
&nbsp;
- **ENOMEM**
There's insufficient memory available with the requested *alignment*.

## Examples
&nbsp;

## Classifications
POSIX 1003.1-2017

## Function Safety
TODO
