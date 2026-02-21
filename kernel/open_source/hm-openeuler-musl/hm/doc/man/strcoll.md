# strcoll()
Compare two strings using current locale

## Synopsis
```c
#include <string.h>

int strcoll(const char *s1, const char *s2);
```

## Arguments
- **s1**
The first string to be compared.
&nbsp;
- **s2**
The second string to be compared.

## Library
- libc

## Description
Compare two strings but the regulation shall be affect by the program's current locale for category *LC_COLLATE*.
&nbsp;
Currently, the locale we support has no effect on the comparison and strcoll() performs just like strcmp().

## Returns

- **less than zero**
*s1* is less than *s2*.
&nbsp;
- **equal to zero**
*s1* is equal to *s2*.
&nbsp;
- **greater than zero**
*s1* is greater than *s2*.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety

TODO
