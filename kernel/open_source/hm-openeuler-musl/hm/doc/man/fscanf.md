# fscanf()
Get the formatted input from the stream

## Synopsis
```c
#include <stdio.h>

int fscanf(FILE *restrict stream, const char *restrict format, ...);
```

## Arguments 
- **stream**
A pointer points to the input stream.
&nbsp;
- **format**
The *format* is a character string, it contains the form of a conversion specification, such as *%d* and *%s*.

## Library
libc

## Description
The fscanf() function reads from the input stream pointed to by *stream* and reads bytes, interprets them to a format, stores the results in the argument.

## Returns
Get the input successfully, these functions shall return the number of successfully matched and assigned input items; this number can be 0 in the event of an early matching failure. If the input ends before the first conversion (if any) has completed, and without a matching failure having occurred, EOF shall be returned. 

If an error occurs before the first conversion (if any) has completed, and without a matching failure having occurred, EOF shall be returned and *errno* shall be set to indicate the error.  If a read error occurs, the error indicator for the stream shall be set.

## Errors
- **EILSEQ**
The input byte sequence does not form a valid character.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.
&nbsp;
- **EINVAL**
There are insufficient arguments.
&nbsp;
In addition, the conditions under which the fscanf() function shall fail, refer to fgetc() or fgetwc().

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
