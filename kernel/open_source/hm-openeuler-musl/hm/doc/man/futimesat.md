# futimesat()
Change the timestamps of a file relative to a directory

## Synopsis
```c
#include <fcntl.h>
#include <sys/time.h>

int futimesat(int dirfd, const char *pathname, const struct timeval times[2]);
```

## Arguments
- **dirfd**
The file descriptor points to a directory.
&nbsp;
- **pathname**
The relative path to the parent directory.
&nbsp;
- **times[2]**
The argument *times[2]* is the *timeval* structure data where the last access time and modification time are stored.

## Library
libc

## Description
The function futimesat() set the last access time and modification time of the specific file (located by *dirfd* and *pathname*) with the give *times[2]*.
It is worth noting that if *times* is NULL, then function will set the current time to file's last access and last modification.
If the pathname given in pathname is relative, then it is interpreted relative to the directory referred to by the file descriptor *dirfd*.
If pathname is relative and *dirfd* is the special value AT\_FDCWD, then pathname is interpreted relative to the current working directory of the calling process.
If pathname is absolute, then *dirfd* is ignored.

## Returns
- **0**
Function finished successfully.
- **-1**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.

## Errors
- **EACCES**
The argument *times[2]* is NULL, the caller does not have the write permission to the file.
&nbsp;
- **ENOENT**
The file referred by *pathname* does not exit.
&nbsp;
- **EPERM**
The argument *times[2]* is not NULL, the caller does not have the write permission to the file.
&nbsp;
- **EROFS**
The argument *pathname* resides on a read-only filesystem.
&nbsp;
- **EBADF**
The argument *dirfd* in an invalid file descriptor.
&nbsp;
- **ENOTDIR**
The argument *pathname* is relative and *dirfd* refers to a file other than a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
