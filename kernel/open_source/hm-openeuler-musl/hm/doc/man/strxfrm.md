# strxfrm()
Transform string to another form

## Synopsis
```c
#include <string.h>

size_t strxfrm(char *dest, const char *src, size_t n);
```

## Arguments
- **dest**
The destination for the output string.
&nbsp;
- **src**
The string to be transformed.
&nbsp;
- **n**
Specified the destination size.

## Library
- libc

## Description
Transform the input string specified by *src* to anther form and place it in the memory specified by *dest* of size *n* bytes. The new form of string can be passed to strcmp and get the same result as a calling to strcoll() with the original form of string passed.
&nbsp;
Currently, the locale we support has no effect on the comparison of strcoll(), and it performs just like strcmp(), so the function strxfrm() just copies the string to destination if the destination size is proper.

## Returns
The size of the new form of the string excluding the terminating NULL byte. And if the returned value is bigger than *n*, that means the destination is too small and the transformation is ignored.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX

## Function Safety

TODO
