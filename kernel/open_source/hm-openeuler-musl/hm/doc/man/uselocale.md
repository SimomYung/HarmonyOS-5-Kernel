# uselocale()
Set or query the current locale for the calling thread

## Synopsis
```c
#include <locale.h>

locale_t uselocale(locale_t newloc);
```

## Arguments

- **newloc**
The locale to be set for the calling thread.

## Library
- libc

## Description
Set the locale of the calling thread to *newloc* and returns the thread's previously current locale.
&nbsp;
If *newloc* is (locale_t)0, the current locale of the calling thread is left unchanged and it is returned.
&nbsp;
*LC_GLOBAL_LOCALE* is used to represent the global locale, both for input parameter and return value.

## Returns

The calling thread's previously current locale.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX

## Function Safety
TODO
