# wcpcpy()
Copy a wide-character string

## Synopsis
```c
#include <wchar.h>

wchar_t *wcpcpy(wchar_t *__restrict dest, const wchar_t *__restrict src);
```

## Arguments

- **dest**
A pointer to the destination buffer.
&nbsp;
- **src**
A pointer to the source buffer.

## Library
libc

## Description

wcpcpy() copies *n* wide characters including the terminating null wide character (L'\0') from *src* to *dest*.
&nbsp;

When *src* and *dest* overlap, the result is not correct. The programmer must ensure the buffer size of *dest* is large enough.

## Returns
Return a pointer to the terminating null wide character of *dest*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
