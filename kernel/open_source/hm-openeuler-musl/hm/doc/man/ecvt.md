# ecvt()
Convert a floating number to a string

## Synopsis
```c
#include <stdlib.h>

char *ecvt(double number, int ndigits, int *decpt, int *sign);
```

## Arguments
- **number**
Double-precision floating-point number to be converted.
&nbsp;
- **ndigits**
The number of significant digits stored.
&nbsp;
- **decpt**
The location of the decimal point.
&nbsp;
- **sign**
Indicate whether *number* is signed or unsigned.

## Library
libc

## Description
The ecvt function converts a double-precision floating-point number into a string. The value parameter is the floating-point number to be converted. This function stores a maximum of ndigit numeric values as a string and adds a null character ('\ 0'). If the number of digits in value exceeds ndigit, the lower digits are rounded. If there are fewer than ndigit digits, the string is padded with zeros. Only numbers are stored in the string. The decimal point position and value symbol are obtained from decpt and sign after the call. The decpt parameter indicates the integer value giving the position of the decimal point, which is calculated from the beginning of the string. A zero or negative number indicates that the decimal point is to the left of the first digit. The sign parameter indicates an integer that indicates the sign of the number being converted. If the integer is 0, the number is positive, otherwise it is negative.

## Returns
Return a generated string.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
