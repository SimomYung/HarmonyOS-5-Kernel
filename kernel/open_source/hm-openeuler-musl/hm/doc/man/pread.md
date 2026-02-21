# pread(), pwrite()
Read from or write to a file descriptor at a given offset

## Synopsis

```c
#include <unistd.h>

ssize_t pread(int fd, void *buf, size_t size, off_t ofs);
ssize_t pwrite(int fd, const void *buf, size_t size, off_t ofs);
```

## Arguments

- **fd**
A file descriptor refers to a file.
&nbsp;
- **buf**
The memory specified to read or write.
&nbsp;
- **size**
The bytes number of buf to read or write.
&nbsp;
- **ofs**
The offset from the start of the file.

## Library
libc

## Description
pread() reads up to *size* bytes from *fd* at offset *ofs* (from the start of the file) into the buffer starting at *buf*.
&nbsp;
pwrite() writes up to *size* bytes from the buffer starting at *buf* to the file descriptor fd at offset offset.
&nbsp;
The file offset is not changed. The file referred to by fd must be capable of seeking.

## Returns
On success, pread() returns the number of bytes read, pwrite() returns the number of bytes written. On error, return -1 and errno is set appropriately.

## Errors
- **EFAULT**
The *buf* is NULL or the argument *ofs* is not valid.
&nbsp;
- **EINVAL**
The argument *ofs* is invalid.
&nbsp;
- **EBADF**
The argument *fd* is invalid.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EISDIR**
The *fd* refers to a directory.
&nbsp;
- **ESPIPE**
The file is a pipe, FIFO, or socket.

## Example
```c
#include <sys/uio.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define OFF_SET 100

int main(int argc, char *argv[])
{
        ssize_t size;
        char str[] = "Write hello world to file";
        int fd = open(argv[1], O_WRONLY|O_CREAT|O_NONBLOCK);
        size = pwrite(fd, str, strlen(str), OFF_SET);
        printf("write %d bytes to file\n", size);
        close(fd);

        char strRead[100] = {};
        fd = open(argv[1], O_RDONLY|O_NONBLOCK);
        size = pread(fd, strRead, sizeof(strRead), OFF_SET);
        printf("get string from file: %s\n", strRead);
        close(fd);

        return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
