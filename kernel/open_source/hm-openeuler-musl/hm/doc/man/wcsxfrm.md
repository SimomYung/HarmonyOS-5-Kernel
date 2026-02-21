# wcsxfrm()
Transform one wide-character string into another, to a given length

## Synopsis
```c
#include <wchar.h>

int wcsxfrm(wchar_t *ws1, const wchar_t *ws2, size_t n);
```

## Arguments
- **ws1**
Point to the destination to store the result wide-character string.
&nbsp;
- **ws2**
Point to the string to be transformed.
&nbsp;
- **n**
The maximum number of characters to transform.

## Library
- libc

## Description
The wcsxfrm() function transforms the string pointed to by *ws2* to the buffer pointed to by *ws1*, to a maximum of *n* wide-characters, including the terminating NULL.

## Returns
The length of the transformed wide-character string. If this value is greater than *n*, the lower *n-1* characters are copied and an terminating NULL added to the end of *ws1*.

## Errors
N/A

## Example
&nbsp;

## Classification
Unix

## Function Safety

TODO
