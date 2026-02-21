# mkstemp()
Create a unique temporary file

## Synopsis
```c
#include <stdlib.h>

int mkstemp(char *template);
```
## Arguments

- **template**
The file name template for temporary file must be ended with "xxxxxx".

## Library
libc

## Description
The mkstemp() function generates a unique temporary filename from template, then creates and opens the file, and returns an open file descriptor for the temporary file. The *template* must be ended with "XXXXXX" and these are replaced with a string that makes the filename unique. Since it will be modified, template should be declared as a character array.

## Returns
On success, the mkstemp() function returns a file descriptor. on error, the mkstemp() function returns -1, errno is set to indicate the error.
&nbsp;
## Errors
- **EINVAL**
The argument template is not ended with "xxxxxx".
&nbsp;
- **ENAMETOOLONG**
The argument template is too long.
&nbsp;
- **EFAULT**
The address of argument is valid.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EMFILE**
The per-process limit on the number of open file descriptors has been reached.
&nbsp;
- **EBADF**
Can not get a valid fd for the temporary file.
&nbsp;
- **EROFS**
Read-only filesystem.
&nbsp;
- **EACCES**
No permission to access the file.
&nbsp;
- **EPERM**
The user has no permission to access the file.
&nbsp;
- **EEXIST**
Could not create a unique temporary filename.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
