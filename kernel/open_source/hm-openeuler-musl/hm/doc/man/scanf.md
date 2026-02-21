# scanf(), sscanf()
Input format conversion

## Synopsis
```c
#include <stdio.h>

int scanf(const char *format, ...);
int sscanf(const char *str, const char *format, ...);
```

## Arguments
- **format**
A sequence of characters to describe how to process the sequence of input characters.
&nbsp;
- **str**
The character string is used as the input source.
&nbsp;
- **...**
A variable number of arguments as the target to store the input form stream or string.
&nbsp;

## Library
libc

## Description
The function scanf() or sscanf() scans input according to the format string. The format string contains conversion specifications; the results from such conversions are stored in the locations pointed to by the pointer arguments that follow the format string.  Each pointer argument must be of a type that is appropriate for the value returned by the corresponding conversion specification.
&nbsp;
The scanf() function reads input from the standard input stream stdin, and sscanf() reads its input from the character string pointed to by *str*.
&nbsp;
## Returns
On success, these functions return the number of input items successfully matched and assigned. On failure, EOF is returned, if a read error occurs, or if the end of input is reached before either the first successful conversion or a matching failure occurs.

## Errors
- **ENOMEM**
There is not enough memory during the operations in the function (unlikely to happen).
- **EINVAL**
Invalid arguments.
- **ERANGE**
The result of an conversion would exceed the size that can be stored in the corresponding type.
- **EILSEQ**
The input byte sequence does not form a valid character.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
