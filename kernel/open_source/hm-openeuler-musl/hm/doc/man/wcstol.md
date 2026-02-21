# wcstol()
Convert a wide-character string into a long integer number

## Synopsis
```c
#include <wchar.h>

long wcstol(const wchar_t *restrict s, wchar_t **restrict p, int base)
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
The wcstol() function is used to convert a wide-character string to a long integer number.

## Returns
It returns the converted value on success.

If the correct value will cause an overflow, return positive and negative HUGE\_VAL according to the sign and set errno to ERANGE. If the correct value will cause an underflow, then zero is returned and errno is set to ERANGE.

## Errors
- **ERANGE**
The value to be returned would cause overflow or underflow.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
