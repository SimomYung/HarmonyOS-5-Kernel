# __ctype_b_loc()
Accessor function for __ctype_b array for ctype functions

## Synopsis
```c
#include <ctype.h>

const unsigned short * * __ctype_b_loc(void);
```

## Arguments

N/A

## Library
libc

## Description
The function returns a pointer to the character array, which contains the characteristics of each character in the current character set. The array contains a total of 384 characters and can be indexed using any signed or unsigned char (that is, the index value is between -128 and 255).

## Returns

The function returns a pointer to the character array, which contains the characteristics of each character in the current character set.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
N/A
