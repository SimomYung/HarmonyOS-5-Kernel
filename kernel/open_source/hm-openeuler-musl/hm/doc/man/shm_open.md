# shm\_open()
Open a shared memory object

## Synopsis
```c
#include <sys/mman.h>

int shm_open(const char *name, int oflag, mode_t mode);
```

## Arguments

- **name**
The name of the shared memory object.
&nbsp;
- **oflag**
The file status flags and file access modes.
&nbsp;
- **mode**
Not supported now.

## Library
libc

## Description
The shm\_open() function establishes a connection between a shared memory object and return a file descriptor for the shared memory object. The file descriptor is used by other functions to refer to that shared memory object.
The file status flags and file access modes of the open file description are according to the value of oflag:
- **O\_RDONLY**
Open for read access only.
&nbsp;
- **O\_RDWR**
Open for read or write access.
&nbsp;
- **O\_CREAT**
If the shared memory object exists, this flag has no effect. Otherwise, the shared memory object is created.
&nbsp;
- **O\_EXCL**
If O\_EXCL and O\_CREAT are set, shm\_open() fails if the shared memory object exists.
&nbsp;
- **O\_TRUNC**
If the shared memory object exists, and it is successfully opened O\_RDWR, then the object is truncated to zero length and the mode and owner is unchanged by this function call.
&nbsp;

## Returns

- **non-negative integer**
An unused file descriptor
&nbsp;

- **-1**
An error occurred.

## Errors

- **EACCES**
The shared memory object exists and the permissions specified by oflag are denied, or the shared memory object does not exist and permission to create the shared memory object is denied, or O\_TRUNC is specified and write permission is denied.
&nbsp;
- **EEXIST**
O\_CREAT and O\_EXCL are set and the named shared memory object already exists.
&nbsp;
- **EINTR**
Interrupted by a signal.
&nbsp;
- **EINVAL**
Invalid name.
&nbsp;
- **EMFILE**
Not enough file descriptors can be used in this process.
&nbsp;
- **ENAMETOOLONG**
The length of the name argument is too long.
&nbsp;
- **ENFILE**
Too many shared memory objects are currently open in the system.
&nbsp;
- **ENOENT**
O\_CREAT is not set and the named shared memory object does not exist.
&nbsp;
- **ENOSPC**
There is insufficient space for the creation of the new shared memory object.
&nbsp;

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
