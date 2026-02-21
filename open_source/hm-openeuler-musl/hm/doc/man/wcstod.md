# wcstod()
Convert a wide-character string into a double

## Synopsis
```c
#include <wchar.h>

double wcstod(const wchar_t *s, wchar_t **p);
```

## Arguments
- **s**
A string to be converted.
&nbsp;
- **p**
If *p* isn't null, this pointer stores the first unrecognized character found in the string.

## Library
libc

## Description
The wcstod() function is used to convert a wide-character string to a double number.

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
