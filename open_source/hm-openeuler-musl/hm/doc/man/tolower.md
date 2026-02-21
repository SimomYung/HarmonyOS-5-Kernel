# tolower()

Converts the given character to lowercase

## Synopsis

```c
#include <ctype.h>

int tolower(int c);
```

## Arguments

- **c**
  The character to be converted.

## Library

libc

## Description

The tolower() function converts the given character to lowercase.
&nbsp;
If the value of *c* can not be represented as *unsigned char* and is not equal to EOF, the behavior is undefined.

## Returns

The tolower() function will return the lowercase letter corresponding to *c*. Otherwise, *c* will not change.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO