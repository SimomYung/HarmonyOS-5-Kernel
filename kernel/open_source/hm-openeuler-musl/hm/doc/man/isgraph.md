# isgraph()

Check to see if the character is printable character except space

## Synopsis

```c
#include <ctype.h>

int isgraph(int c);
```

## Arguments

- **c**
  The character to check.

## Library

libc

## Description

The isgraph() function checks to see if the character is printable character except space.

## Returns

- **non-zero**
  This character is printable character except space.
&nbsp;
- **0**
  This character is not printable character except space.

## Errors

N/A

## Example
&nbsp;

## Classification

POSIX 1003.1-2017

## Function Safety

TODO