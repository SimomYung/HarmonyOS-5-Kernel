# toupper()

Converts the given character to uppercase

## Synopsis

```c
#include <ctype.h>

int toupper(int c);
```

## Arguments

- **c**
  The character to be converted.

## Library

libc

## Description

The toupper() function converts the given character to uppercase.
&nbsp;
If the value of *c* can not be represented as *unsigned char* and is not equal to EOF, the behavior is undefined.

## Returns

The toupper() function will return the uppercase letter corresponding to *c*. Otherwise, *c* will not change.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO