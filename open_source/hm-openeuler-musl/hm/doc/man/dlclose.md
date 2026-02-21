# dlclose()
Unload a shared object from memory

## Synopsis
```c
#include <dlfcn.h>

int dlclose(void *handle);
```

## Arguments

- **handle**
The given shared object handle returned by dlopen().

## Library
libc

## Description
Now in our system, dlclose() unloads the shared object from memory directly, no matter whether it is used by others. It can only close libraries opened by dlopen().

## Returns

- **0**
Unload the shared object successfully.
&nbsp;
- **nonzero**
An error occurred, use dlerror() to check what happened.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
