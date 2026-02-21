# ispunct()

Check to see if the character is a printable character which is not a space or an alphanumeric character

## Synopsis

```c
#include <ctype.h>

int ispunct(int c);
```

## Arguments

- **c**
  The character to check.

## Library

libc

## Description

The ispunct() function checks to see if the character is a printable character which is not a space or an alphanumeric character.

## Returns

- **non-zero**
  This character is a printable character which is not a space or an alphanumeric character.
&nbsp;
- **0**
  This character is not a printable character which is not a space or an alphanumeric character.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO