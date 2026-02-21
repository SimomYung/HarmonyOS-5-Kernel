# lseek()
Reposition read/write file offset

## Synopsis
```c
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);
```
## Arguments

- **fd**
An open file descriptor to a file.
&nbsp;
- **offset**
Position according to the directive *whence*.
&nbsp;
- **whence**
A flag specified for SEEK_SET, SEEK_CUR, and SEEK_END.

## Library
libc

## Description
The lseek() function repositions the offset of the open file associated with the file descriptor fd to the argument offset according to the directive whence as follows:
	- SEEK_SET
			The offset is set to *offset* bytes.
	- SEEK_CUR
			The offset is set to its current position plus *offset* bytes.
	- SEEK_END
			The offset is set to the size of the file plus *offset* bytes.

## Returns

On success, this function returns the offset from the start of the open file. On failure, this function returns -1, and errno is set.

## Errors

- **ENOENT**
The file does not exist.
&nbsp;
- **ESPIPE**
Illegal seek.
&nbsp;
- **EBADF**
The argument *fd* is not valid.
&nbsp;
- **EINVAL**
The argument *offset* is not valid, or the argument *whence* is not one of SEEK_SET, SEEK_CUR and SEEK_END.
&nbsp;
- **ENOMEM**
Insufficient memory was available.
&nbsp;
- **EOPNOTSUPP**
Operation is not supported.

## Examples
```c
#include <sys/uio.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char *argv[])
{
	ssize_t size;
	off_t len;
	char str[] = "Write hello world to file\n";
	int fd = open(argv[1], O_WRONLY);
	len = lseek(fd, 0, SEEK_END); //append to the file
	printf("len of file %s %d \n", argv[1], len);

	size = write(fd, str, strlen(str));
	printf("write %d bytes to file\n", size);

	close(fd);
	return 0;
}
```
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
