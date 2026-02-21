# dlopen()
Load a dynamic library to process memory from filesystem.

## Synopsis
```c
#include <dlfcn.h>

void *dlopen(const char *filename, int flags);
```

## Arguments

- **filename**
The filename of the shared object file.
&nbsp;
- **flags**
The flags to control dlopen behavior.

## Library
libc

## Description
The function dlopen() loads the dynamic library to memory. If the filename is not starting with '/'. If it fails to load the dynamic library, use dlerror() to check what happened.
&nbsp;
One of the following two values must be included in *flags*:
&nbsp;
- **RTLD\_LAZY**
Perform lazy binding. Resolve the symbol when the symbol is called at the first time. If a symbol is never called, the symbol will never be resolved.
&nbsp;
- **RTLD\_NOW**
If this value is specified, or environment variable LD\_BIND\_NOW is set, all symbols will be resolved when the shared objects are loaded.
&nbsp;
- **RTLD\_GLOBAL**
The symbols defined by this library will be made available for the symbol resolution of the subsequently loaded libraries.
&nbsp;
- **RTLD\_LOCAL**
This is the converse of RTLD\_GLOBAL, and it is the default value *flags*. Symbols defined in this library are not made available to resolve references in subsequently loaded libraries.
- **RTLD\_DEEPBIND**
This option means symbols will be resolved from the library itself first. This means that a self-contained object will use its own symbols if the symbol exists both in the object and the global objects which have been loaded.
&nbsp;

The shared object will be searched as following orders:
&nbsp;
1. The path specified by DT\_RPATH.
&nbsp;
2. The path specified by environment variable LD\_LIBRARY\_PATH.
&nbsp;
3. The path specified by DT\_RUNPATH.
&nbsp;
4. The path /etc/ld-musl-$ARCH.path (The argument *ARCH* is the architecture of the machine, like arm).
&nbsp;
5. The path /lib and /usr/lib.

## Returns

- **0**
NULL is returned when failed to load shared objects to memory. Use dlerror() to check what happened.
&nbsp;
- **nonzero**
If the dynamic library is loaded to memory successfully, a non-zero shared object handle will be returned.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
