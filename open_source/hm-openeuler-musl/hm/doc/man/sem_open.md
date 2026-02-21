# sem\_open()
Open and possibly create a named semaphore

## Synopsis
```c
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

sem_t *sem_open(const char *name, int oflag);
sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
```

## Arguments

- **name**
The name of the semaphore.
&nbsp;
- **oflag**
The flags that control the operation of the call.
&nbsp;
- **mode**
The permissions applied to the new semaphore. Unsupported by now.
&nbsp;
- **value**
The initial value of the new semaphore.


## Library
libc

## Description
The sem\_open() function opens an existing semaphore or create a new semaphore. The identifier of the semaphore is specified by *name*. The argument *name* is a C string consisting of one or more initial '/' followed by characters, none of which is '/'. The argument *name* is also allowed to start with no '/'. The semaphore will be created if it does not exist and O\_CREAT is specified in *oflag*. If both O\_CREAT and O\_EXCL are specified in *oflag* and the semaphore specified by *name* already exists, it will fail and EEXIST will be returned.
&nbsp;
If O\_CREAT is specified in *oflag*, two additional arguments must be applied. The argument *mode* is not supported by now. The argument *value* specifies the initial value of the new semaphore. If O\_CREAT is specified in *oflag* and the semaphore already exists, *mode* and *value* will be ignored.

## Returns

- **semaphore address**
On success, the address of the semaphore will be returned.
&nbsp;
- **SEM_FAILED**
An error occurred.

## Errors

- **EEXIST**
Both O\_CREAT and O\_EXCL are specified in *oflag* and the semaphore already exists.
&nbsp;
- **ENAMETOOLONG**
The length of *name* exceeds the maximum limit.
&nbsp;
- **EINVAL**
Character '/' or substring ".." exists after the initial '/' in *name*.
&nbsp;
- **EINVAL**
There is no other character followed the initial '/' in *name*.
&nbsp;
- **EINVAL**
*value* exceeds the maximum allowable value of semaphore.
&nbsp;
- **ENOENT**
The O\_CREAT is not specified in *oflag* but the semaphore does not exists.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EMFILE**
The number of open file descriptors exceeds the per-process limit.
&nbsp;
- **ENFILE**
The number of open file descriptors exceeds the system-wide limit.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
