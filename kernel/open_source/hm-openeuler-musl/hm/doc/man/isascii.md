# isascii()

Check to see if the character is a 7-bit unsigned char value in the ASCII character set

## Synopsis
```c
#include <ctype.h>

int isascii(int c);
```

## Arguments 

- **c**
  The character to check.


## Library
libc

## Description
The isascii() function checks to see if the character is a 7-bit unsigned char value in the ASCII character set.

## Returns

- **non-zero**
  This character is a 7-bit unsigned char value in the ASCII character set.
&nbsp;
- **0**
  This character is not a 7-bit unsigned char value in the ASCII character set.

## Errors

N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO