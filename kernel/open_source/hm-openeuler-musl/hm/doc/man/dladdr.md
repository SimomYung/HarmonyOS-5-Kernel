# dladdr()
Search and get a symbol information by given address

## Synopsis
```c
#define _GNU_SOURCE
#include <dlfcn.h>

int dladdr(void *addr, Dl_info *info);
```

## Arguments

- **addr**
The given symbol address to search.
&nbsp;
- **info**
The symbol information about this symbol, it is an output parameter.

## Library
libc

## Description
The function dladdr() checks if the given address is located at a symbol, and if it is, a structure Dl\_info will be filled to describe this symbol:
&nbsp;

```c
  typedef struct {
    const char * dli_fname;  /* pathname of shared boject that contains address */
    void  *dli_fbase;        /* Base address which the shared object is loadded to */
    const char *dli_sname;   /* The symbol name */
    void  *dli_saddr;        /* Exact address of symbol named in dli_sname */
  } Dl_info;
```

## Returns

- **0**
No symbol can be found on given loaded address. In this case, the error message is not available via dlerror().
&nbsp;
- **nonzero**
A symbol is found on given loaded address.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
