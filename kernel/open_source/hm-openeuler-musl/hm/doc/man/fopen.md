# fopen(), fopen64()
Open a stream

## Synopsis
```c
#include <stdio.h>

FILE *fopen(const char *restrict pathname, const char *restrict mode);
FILE *fopen64(const char *restrict pathname, const char *restrict mode);
```

## Arguments 
- **pathname**
The argument *pathname* refers to a file.
&nbsp;
- **mode**
The argument *mode* points to a string which indicated the mode for opening, otherwise the behavior is undefined.

## Library
libc

## Description
The fopen() function opens a file pointed to by *pathname* in the indicated *mode*.

The file position is not reliable when switching between reading and writing on a stream that is opened for both without a successful seek in between, unless the read ended with hitting EOF.

Consider below code snippet.

```C
FILE *fp = fopen("foobar", "w+");
fputs("hello", fp);
rewind(fp);
fgetc(fp);
fputc('x', fp);
off = ftello(fp);
```

'off' will be 6 if you run it, this is undefined behavior.

## Returns
A pointer to the object controlling the stream is returned while opening the file successfully.

When open the file failed, the NULL pointer is returned and *errno* is set.

## Errors
- **EACCES**
The prefix component of *pathname* is denied to search, or the file exists and the permissions *mode* are denied, or the file dose not exist and the file is denied to be created in the parent directory.
&nbsp;
- **EISDIR**
The argument *pathname* refers to a directory and  *mode* requires write access.
&nbsp;
- **ELOOP**
A loop exists in symbolic links encountered during resolution of *pathname*.
&nbsp;
- **EMFILE**
Too many files are opened by the process.
&nbsp;
- **ENAMETOOLONG**
Too argument *pathname* is too long.
&nbsp;
- **ENFILE**
Too many files in the system.
&nbsp;
- **ENOENT**
The argument *mode* begins with *r* and the *pathname* does not exist, or *mode* begins with *a* or *w* and the prefix of "pathname" does not refers to a existing file, or *pathname* is an empty string.
&nbsp;
- **ENOTDIR**
The prefix component of argument "pathname" refers to a file other than a file.
&nbsp;
- **EOVERFLOW**
The size of the file pointed to by *pathname* can not be represented correctly in an object of type *off_t*.
&nbsp;
- **EROFS**
The file pointed to by *pathname* resides on a read-only file system but "mode" requires write permission.
&nbsp;
- **EINVAL**
The argument *mode* is not valid.
&nbsp;
- **ENOMEM**
Insufficient memory space is available.
&nbsp;
- **ETXTBSY**
The file pointed to by *pathname* is a pure procedure file which is working and it requires write access.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
