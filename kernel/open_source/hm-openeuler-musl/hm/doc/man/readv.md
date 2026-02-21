# readv(), writev()
Read or write data into multiple buffers

## Synopsis

```c
#include <sys/uio.h>

ssize_t readv(int fd, const struct iovec *iov, int count);
ssize_t writev(int fd, const struct iovec *iov, int count);
```

## Arguments

- **fd**
  A file descriptor refers to a file.
  &nbsp;
- **iov**
  Multiple buffers to read or write.
  &nbsp;
- **count**
  The number of multiple buffers

## Library
libc

## Description
The readv() reads *count* buffers from the file associated with the file descriptor fd into the buffers described by *iov*.The writev() writes *count* buffers of data described by *iov* to the file associated with the file descriptor *fd*.The pointer *iov* points to an array of iovec structures, defined in &lt;sys/uio.h&gt; as:
```c
	struct iovec {
		void  *iov_base;
		size_t iov_len;
	};
```
&nbsp;
The data transfers performed by readv() and writev() are atomic.
&nbsp;
The behavior of accessing the iovec array out of bounds (*count* is larger than the number of iovecs in *iov*) is undefined, it may success or return EINVAL.

## Returns
On success, readv() returns the number of bytes read, writev() returns the number of bytes written. On error, return -1 and errno is set appropriately.

## Errors
- **EFAULT**
The *iov* argument is NULL, or the sum of the iov_len values overflows an ssize_t value.
&nbsp;
- **EINVAL**
The *count* argument is bigger than const IOV_MAX, or is less than zero, or the *iov_len* member of *struct iovec* is less than zero.
&nbsp;
- **EINVAL**
The *iov* argument is outside your accessible address space.
&nbsp;
- **EBADF**
The *fd* argument is less than 0, or is bigger than the max value, or is not a valid file descriptor, or is not opened for reading or writing.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EISDIR**
The *fd* refers to a directory.
&nbsp;
- **ENOTSUP**
The *fd* refers to a socket(readv), or the operation of reading or writing is not supported.
&nbsp;
- **EACCES**
No appropriate privileges to read or write.
&nbsp;
- **EFBIG**
Exceed the maximum file size.
&nbsp;
- **ENOSPC**
Not enough space to write.
&nbsp;
- **ENXIO**
A request was made of a nonexistent device, or the request was outside the capabilities of the device.
&nbsp;

## Example
&nbsp;
```c
/* readv.c */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>

#define BUF_SIZE 50
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

	len = readv(fd,vec,2);
	puts((char *)(vec[0].iov_base));
	puts((char *)(vec[1].iov_base));
	return 0;
}

/* writev.c */
#include <sys/uio.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char *argv[])
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

	size=writev(fd, iov, 2);

	close(fd);
	return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
