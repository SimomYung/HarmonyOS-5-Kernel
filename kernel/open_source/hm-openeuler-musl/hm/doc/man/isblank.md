# isblank()

Check to see if the character is a blank character

## Synopsis

```c
#include <ctype.h>

int isblank(int c);
```

## Arguments

- **c**
  The character to check.

## Library

libc

## Description

The isblank() function checks to see if the character is a blank character which is a space or a tab.

## Returns

- **non-zero**
  This character is a blank character.
&nbsp;
- **0**
  This character is not a blank character.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO