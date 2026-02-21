# isspace()

Check to see if the character is a white-space character

## Synopsis

```c
#include <ctype.h>

int isspace(int c);
```

## Arguments

- **c**
  The character to check.

## Library

libc

## Description

The isspace() function checks to see if the character is a white-space character which are space*(' ')*, form-feed (*'\f'*), newline (*'\n'*), carriage return (*'\r'*), horizontal tab *('\t'*), and vertical tab (*'\v'*).

## Returns

- **non-zero**
  This character is a white-space character.
&nbsp;
- **0**
  This character is not a white-space character..

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO