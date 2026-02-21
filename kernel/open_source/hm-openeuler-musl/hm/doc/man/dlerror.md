# dlerror()
Get a human readable error for recent failure on dlopen(),dlsym(),dlclose()

## Synopsis
```c
#include <dlfcn.h>

char *dlerror(void);
```

## Arguments
N/A

## Library
libc

## Description
The function dlerror() returns a string when failure on dlopen(),dlsym(),dlclose(), and returns NULL if there is no error.

## Returns

- **string**
Return a null-terminated string if there is error on dlopen(),dlsym(),dlclose().
&nbsp;
- **NULL**
If there is no error, NULL is returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
