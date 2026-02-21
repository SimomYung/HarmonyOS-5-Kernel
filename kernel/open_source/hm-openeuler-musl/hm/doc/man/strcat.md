# strcat(), strncat(), strlcat()
Concatenate two strings

## Synopsis
```c
#include <strings.h>

char *strcat(char *dest, const char *src);

char *strncat(char *dest, const char *src, size_t n);

size_t strlcat(char *dest, const char *src, size_t size);
```

## Arguments

- **dest**
A pointer to the destination buffer.
&nbsp;
- **src**
A pointer to the source buffer.
&nbsp;
- **n**
The number of characters that are needed to copy at most.
&nbsp;
- **size**
Ideally, it is the size of the destination buffer.

## Library
libc

## Description
strcat() appends the string in *src* to *dest* and overwrites the terminating null byte ('\0') of *dest*, finally it adds a new terminating null byte.
&nbsp;

When *src* and *dest* overlap, the result is not correct. The programmer must ensure the buffer size of *dest* is larger enough otherwise the function behavior is unpredictable and it may cause security problems.
&nbsp;

strncat() is like strcat(), except it copies at most *n* characters of *src* and *src* doesn't have to be null-terminated if the number of characters in *src* is *n* or more.
&nbsp;

strlcat() is like strcat(), except it copies at most *size* - strlen(*dest*) - 1 characters from *src*. When *src* and *dest* overlap, the result is correct. The programmer must ensure *size* is not smaller than strlen(*dest*). This function returns the length of string it tried to create, so if the return value is equal or greater than *size*, data loss occurred. The programmer needs to check the arguments or the return value to ensure correctness.

## Returns
strcat() and strncat() return a pointer to *dest*.

strlcat() returns the length of string it tried to create.

## Errors
N/A

## Example
&nbsp;

## Classification

- **strcat**
1003.1-2017
&nbsp;
- **strncat**
1003.1-2017
&nbsp;
- **strlcat**
UNIX

## Function Safety
TODO