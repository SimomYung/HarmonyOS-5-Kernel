# freopen()
Open a stream

## Synopsis
```c
#include <stdio.h>

FILE *freopen(const char *restrict pathname, const char *restrict mode, FILE *restrict stream);
```

## Arguments 
- **pathname**
The argument *pathname* points to a file for redirecting.
&nbsp;
- **mode**
The mode of the stream specified by *mode* shall be open. 
&nbsp;
- **stream**
The I/O stream is pointed to by *stream*.

## Library
libc

## Description
The freopen() function first attempts to flush the stream associated with stream as if by a call to fflush(stream).  Failure to flush the stream successfully shall be ignored. If pathname is not a null pointer, freopen() shall close any file descriptor associated with stream.  Failure to close the file descriptor successfully shall be ignored.  The error and end-of-file indicators for the stream shall be cleared.
&nbsp;
The freopen() function shall open the file whose pathname is the string pointed to by pathname and associate the stream pointed to by stream with it. The mode argument shall be used just as in fopen().
&nbsp;
The original stream shall be closed regardless of whether the subsequent open succeeds.
&nbsp;
If pathname is a null pointer, the freopen() function shall attempt to change the mode of the stream to that specified by mode, as if the name of the file currently associated with the stream had been used. In this case, the file descriptor associated with the stream need not be closed if the call to freopen() succeeds. It is implementation defined which changes of mode are permitted (if any), and under what circumstances.
&nbsp;
After a successful call to the freopen() function, the orientation of the stream shall be cleared, the encoding rule shall be cleared, and the associated mbstate_t object shall be set to describe an initial conversion state.
&nbsp;
If pathname is not a null pointer, or if pathname is a null pointer and the specified mode change necessitates the file descriptor associated with the stream to be closed and reopened, the file descriptor associated with the reopened stream shall be allocated and opened as if by a call to open() with the following flags:
&nbsp;
| freopen() Mode   | open() Flags              |
|------------------|---------------------------|
| r or rb          | O_RDONLY                  |
| w or wb          | O_WRONLY|O_CREAT|O_TRUNC  |
| a or ab          | O_WRONLY|O_CREAT|O_APPEND |
| r+ or rb+ or r+b | O_RDWR                    |
| w+ or wb+ or w+b | O_RDWR|O_CREAT|O_TRUNC    |
| a+ or ab+ or a+b | O_RDWR|O_CREAT|O_APPEND   |

## Returns
- **0**
Open the stream successfully, 0 is returned.
&nbsp;
- **-1**
An error occurs, -1 is returned and *errno* is set.

## Errors
- **EACCES**
The component of the path prefix is defined for searching, or the file exists and the permissions specified by *mode* are denied, or the file does not exist and write permission is denied for the parent directory of the file to be created.
&nbsp;
- **EBADF**
The file descriptor underlying the *stream* is not a valid file descriptor when *pathname* is a null pointer.
&nbsp;
- **EISDIR**
The file is a directory and mode requires write access.
&nbsp;
- **ELOOP**
A loop exists in symbolic links encountered during resolution of the *pathnem* argument.
&nbsp;
- **ENAMETOOLONG**
The *pathname* is too long.
&nbsp;
- **ENFILE**
Too many files are opened in the filesystem.
&nbsp;
- **ENOENT**
The *mode* string begins with 'r' and a component of *pathname* does not name an existing file, or mode begins with 'w' or 'a' and a component of the path prefix of *pathname* does not name an existing file, or pathname is an empty string.
&nbsp;
- **ENOSPC**
The directory or file system that would contain the new file cannot be expanded, the file does not exist, and it was to be created.
&nbsp;
- **EOVERFLOW**
The file is a regular file and the size of the file cannot be represented correctly in an object of type off_t.
&nbsp;
- **EROFS**
The file resides on a read-only file system and mode requires write access.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
