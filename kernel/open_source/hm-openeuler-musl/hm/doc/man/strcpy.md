# strcpy(), strncpy(), strlcpy()
Copy a string

## Synopsis
```c
#include <strings.h>

char *strcpy(char *dest, const char *src);

char *strncpy(char *dest, const char *src, size_t n);

size_t strlcpy(char *dest, const char *src, size_t size);
```

## Arguments

- **dest**
A pointer to the destination buffer.
&nbsp;
- **src**
A pointer to the source buffer.
&nbsp;
- **n**
The length of the string that is needed to copy.
&nbsp;
- **size**
Ideally, it is the size of the destination buffer.

## Library
libc

## Description
strcpy() copies *n* characters including the terminating null byte ('\0') from *src* to *dest*. When *src* and *dest* overlap, the result is not correct. The programmer must ensure the buffer size of *dest* is larger enough.
&nbsp;

strncpy() is like strcpy(), except it copies at most n characters. If the length of *src* is smaller than *n*, the remaining spaces in *dest* are filled with null bytes ('\0') . If the length of *src* is equal to or larger than *n*, the string in *dest* is not ended by a terminating null byte ('\0').
&nbsp;

strlcpy() is like strncpy(), except it copies at most *size* - 1 characters from *src* to *dest* and it doesn't fill redundant null byte ('\0'). This function returns the length of *src*, so if the return value is equal or greater than *size*, data loss occurred. The programmer needs to check the arguments or the return value to ensure correctness.

## Returns
strcpy() and strncpy() return a pointer to *dest*.

strlcpy() returns the length of *src*.

## Errors
N/A

## Example
&nbsp;

## Classification

- **strcpy**
1003.1-2017
&nbsp;
- **strncpy**
1003.1-2017
&nbsp;
- **strlcpy**
UNIX

## Function Safety
TODO