# preadv(), pwritev()
Read or write data into multiple buffers

## Synopsis

```c
#include <sys/uio.h>

ssize_t preadv(int fd, const struct iovec *iov, int count, off_t ofs);
ssize_t pwritev(int fd, const struct iovec *iov, int count, off_t ofs);
```

## Arguments

- **fd**
A file descriptor refers to a file.
&nbsp;
- **iov**
Multiple buffers to read or write.
&nbsp;
- **count**
The number of multiple buffers.
&nbsp;
- **ofs**
The offset from the start of a file.

## Library
libc

## Description
preadv() reads *count* buffers from file descriptor *fd* at offset *ofs* (from the start of the file) into the buffers described by *iov*.
&nbsp;
pwritev() writes *count* buffers of data described by *iov* to the file descriptor fd at offset *ofs* (from the start of the file).
&nbsp;
The file referenced by fd must be capable of seeking.
&nbsp;
The pointer *iov* points to an array of iovec structures, defined as:
```c
struct iovec {
        void  *iov_base;
        size_t iov_len;
};
```
## Returns
On success, preadv() returns the number of bytes read, pwritev() returns the number of bytes written. On error, -1 is returned and errno is set appropriately.

## Errors
- **EFAULT**
The argument *iov* is a invalid address.
&nbsp;
- **EINVAL**
The argument *fd* is not a valid file descriptor or the iov\_len of *iov* is invalid.
&nbsp;
- **EBADF**
The argument *fd* does not exist.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EISDIR**
The *fd* refers to a directory.
&nbsp;
- **ESPIPE**
The file is a pipe, FIFO, or socket.
&nbsp;
- **EOPNOTSUPP**
Operation is not supported.

## Example
&nbsp;
```c
/* preadv.c */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>

#define BUF_SIZE 50
#define OFF_SET 100
int main(int argc, char *argv[])
{
        int len;
        struct iovec vec[2];
        char buf1[BUF_SIZE] = {0};
        char buf2[BUF_SIZE] = {0};

        int fd = open(argv[1], O_RDONLY);

        vec[0].iov_base = calloc(1, BUF_SIZE);
        vec[0].iov_len = 2;

        vec[1].iov_base = calloc(1, BUF_SIZE);
        vec[1].iov_len = BUF_SIZE;

        len = preadv(fd, vec, 2, OFF_SET);
        puts((char *)(vec[0].iov_base));
        puts((char *)(vec[1].iov_base));
        return 0;
}

/* pwritev.c */
#include <sys/uio.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
        ssize_t size;
        char buf1[9] = "abc";
        char buf2[9] = "123";
        struct iovec iov[2];

        int fd = open(argv[1], O_WRONLY);

        iov[0].iov_base = buf1;
        iov[0].iov_len = strlen(buf1);
        iov[1].iov_base = buf2;
        iov[1].iov_len = strlen(buf2);

        size=pwritev(fd, iov, 2, OFF_SET);

        close(fd);
        return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
