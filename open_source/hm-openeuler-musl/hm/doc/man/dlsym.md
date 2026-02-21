# dlsym()
Translate a string symbol to its loaded address in memory.

## Synopsis
```c
#include <dlfcn.h>

void *dlsym(void *handle, const char *symbol);
```

## Arguments

- **handle**
The shared object handle for the shared object.
&nbsp;
- **symbol**
The given zero-terminated string of the symbol to lookup.

## Library
libc

## Description
The function dlsym() translates a string symbol to its loaded address in memory. If the symbol is not found, NULL will be returned. If *handle* is NULL, dlsym() will search all the open shared objects until this symbol is found. Or it will just search the shared object specified by *handle*.

## Returns

- **0**
Errors occur, use dlerror() to check what happened.
&nbsp;
- **nonzero**
If the symbol is found, the loaded address will be returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
