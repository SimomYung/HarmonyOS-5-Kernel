# wcscoll()
Compare two wide-character strings, using the locale's collating sequence

## Synopsis
```c
#include <wchar.h>

int wcscoll( const wchar_t *ws1, const wchar_t *ws2 );
```

## Arguments
- **ws1**
The first wide-character string to be compared.
&nbsp;
- **ws2**
The second wide-character string to be compared.

## Library
- libc

## Description
Compare two wide-character strings but the regulation shall be affect by the program's current locale for category *LC_COLLATE*.
&nbsp;
Currently, the locale we support has no effect on the comparison and wcscoll() performs just like wcscmp().

## Returns
- **less than zero**
*ws1* is less than *ws2*.
&nbsp;
- **equal to zero**
*ws1* is equal to *ws2*.
&nbsp;
- **greater than zero**
*ws1* is greater than *ws2*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX

## Function Safety

TODO
