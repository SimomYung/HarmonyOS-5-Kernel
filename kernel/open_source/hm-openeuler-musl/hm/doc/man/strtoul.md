# strtoul(), strtoull()
Convert a string into an unsigned long integer

## Synopsis
```c
#include <stdlib.h>

unsigned long strtoul(const char * s, char ** p, int base);
unsigned long long strtoull(const char * s, char ** p, int base);
```

## Arguments
- **s**
A string to be converted.
&nbsp;
- **p**
If *p* isn't null, this pointer stores the first unrecognized character found in the string.
&nbsp;
- **base**
The parameter *base* ranges from 2 to 36, or 0. *base* represents the base method used. For example, if the *base* value is 10, the *base* 10 is used. If the *base* value is 16, the *base* 16 is used. When the *base* value is 0, the decimal conversion is used, but when encountering the leading characters such as '0x', the conversion is performed in hexadecimal. Use octal for conversion.

## Library
libc

## Description
The strtoul() functions used to convert a string into a unsigned long integer number.
The strtoull() functions used to convert a string into a unsigned long long integer number.

## Returns
The strtoul() and strtoull() functions return the result of the conversion for successful. These functions return ULONG\_MAX || ULONGLONG\_MAX if an overflow occurs. In both cases, errno is set to ERANGE.

## Errors
- **ERANGE**
The value to be returned would cause overflow or underflow.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
