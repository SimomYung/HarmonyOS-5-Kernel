# write()
write data to a file descriptor

## Synopsis

```c
#include <unistd.h>

ssize_t write(int fd, void *buf, size_t count);
```

## Arguments

- **fd**
A file descriptor refers to a file or a device or a pipe and so on.
&nbsp;
- **buf**
The memory specified to write to *fd*
&nbsp;
- **count**
The size of *buf*.

## Library
libc

## Description
The write() writes *count* bytes of data described by *buf* to the file referred to the file descriptor *fd*.

## Returns

On success, return the number of bytes write. On error, return -1 and errno is set.

## Errors

- **EFAULT**
The address of *buf* is not valid.
&nbsp;
- **EBADF**
The value of *fd* is not valid.
&nbsp;
- **EISDIR**
The *fd* refers to a directory.
&nbsp;
- **ESPIPE**
The file is a pipe, FIFO, or socket.
&nbsp;
- **EOPNOTSUPP**
Operation is not supported.
&nbsp;
- **EINVAL**
The *buf* is not valid.
&nbsp;

## Example
```c
#include <sys/uio.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char *argv[])
{
	ssize_t size;
	char str[] = "Write hello world to file";
	int fd = open(argv[1], O_WRONLY);
	size = write(fd, str, strlen(str));
	printf("write %d bytes to file\n", size);
	close(fd);

	return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
