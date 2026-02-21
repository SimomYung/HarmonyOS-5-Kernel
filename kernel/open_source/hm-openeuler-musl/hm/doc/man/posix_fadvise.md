# posix\_fadvise()
Show the advisory information of the file

## Synopsis
```c
#include <fcntl.h>

int posix_fadvise(int fd, off_t base, off_t len, int advice);
```

## Arguments

- **fd**
The file descriptor relates to the file.
&nbsp;
- **base**
The beginning point where advices starts.
&nbsp;
- **len**
From the starting point *base*, the function will work on the following *len* bytes.
&nbsp;
- **advice**
Different types of advice.
&nbsp;

## Library
libc

## Description
The posix\_fadvise() function gives advise to user how to properly use the memory of file (looks like some good advices). The *fd* parameter will link to the file which is going to be operated. The *base* and *len* will specify the range of data where the function will work on. And the list parameter *advice* indicates different types of advices. Currently, our system supports advice as following,
&nbsp;
POSIX\_FADV\_NORMAL:
No specified advice on the selected data, just list the property (characteristic) information.
&nbsp;
POSIX\_FADV\_WILLNEED:
The function will pre-read the selected data (to the page cache).
&nbsp;
POSIX\_FADV\_DONTNEED:
The function will drop the selected data from the page cache.


## Returns

- **0**
The function is finished successfully, the advice is done.
&nbsp;
- **Error number**
Operation failed since unexpected situations happened, more details of the failure will be explained in Errors.
&nbsp;

## Errors
- **EBADF**
Failed to obtain the file since the *fd* is invalid.
&nbsp;
- **EINVAL**
Parameters such as *base* or *len* are invalid.
&nbsp;
- **ESPIPE**
The type of the corresponding file is pipe (not supported yet).
&nbsp;

## Example
&nbsp;


## Classification
POSIX 1003.1-2017

## Function Safety
TODO
