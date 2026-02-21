# __ctype_toupper_loc()
Accessor function for __ctype_b_toupper array for ctype toupper() function

## Synopsis
```c
#include <ctype.h>

int32_t * * __ctype_toupper_loc(void);
```

## Arguments

N/A

## Library
libc

## Description
The function returns a pointer to the character array, which contains the uppercase equivalent of each character in the current character set. The array contains a total of 384 characters and can be indexed using any signed or unsigned char (that is, the index value is between -128 and 255).

## Returns

The function returns a pointer to the character array, which contains the uppercase equivalent of each character in the current character set.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety
N/A
