# wmemcpy()
Copy the contents of a source wide character array to a target wide character array

## Synopsis
```c
#include <wchar.h>

wchar_t *wmemcpy(wchar_t *__restrict dest, const wchar_t *__restrict src, size_t n);
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
The wmemcpy() function copies *n* wide characters from *src* to *dest*.
&nbsp;
Take care, *src* and *dest* shall not overlap. And developers need to ensure that *dest* has enough space to keep *n* wide characters.

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
