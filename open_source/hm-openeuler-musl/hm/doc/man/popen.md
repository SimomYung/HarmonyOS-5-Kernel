# popen()
Initiate a pipe stream from or to a process

## Synopsis
```c
 #include <stdio.h>

FILE *popen(const char *command, const char *mode);
```

## Arguments
- **command**
The command to be executed is specified by the string *command*.
&nbsp;
- **mode**
The argument *mode* indicates the I/O mode of popen()

## Library
libc

## Description
The popen() function executes the command specified by *command*, it creates a pipe between the calling program and the executed command, it returns a pointer to a stream which is used to read or write a pipe.
&nbsp;
The mode argument is a pointer to a null-terminated string which must contain either the letter 'r' for reading or the letter 'w' for writing.If mode is any other value, the result is undefined.

## Returns
On success, returns a pointer to an open stream that can be used to read or write to the pipe.
&nbsp;
When the function can not allocate memory, NULL is returned.

## Errors
- **EMFILE**
The caller opens too many streams, more than {STREAM_MAX}.
&nbsp;
- **EINVAL**
The argument *mode* is invalid.
&nbsp;
The popen() function may also set errno values as described by fdopen() or pipe().

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
