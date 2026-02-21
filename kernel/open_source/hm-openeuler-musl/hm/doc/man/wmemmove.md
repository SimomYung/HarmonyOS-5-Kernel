# wmemmove()
Copy the contents of a source wide character array to a target wide character array

## Synopsis
```c
#include <wchar.h>

wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t n);
```

## Arguments

- **dest**
The contents will be copied to *dest* wide character array.
&nbsp;
- **src**
The contents from *src* wide character array will be copied.
&nbsp;
- **n**
*n* wide characters starting at *src* will be copied to *dest*.


## Library
libc

## Description
The wmemmove() function copies *n* wide characters from *src* to *dest*. *src* and *dest* may overlap.
&nbsp;
Take care, developers need to ensure that *dest* has enough space to keep *n* wide characters.

## Returns

- **dest**
A pointer to destination wide character array will be returned.


## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
