# freelocale()
Free a locale object

## Synopsis
```c
#include <locale.h>

void freelocale(locale_t locobj);
```

## Arguments
- **locobj**
A locale\_t object previously returned by a call to newlocale() or duplocale(), is expected to be freed.

## Library
libc

## Description
The freelocale() release the given locale object which is previously returned by a call to newlocale() or duplocale(). If invalid *locobj* or LC\_GLOBAL\_LOCALE is given, the behavior is undefined.

## Returns
N/A

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
