# readdir(), readdir64(), readdir_r(), readdir64_r()
Read a directory

## Synopsis
```c
#include <dirent.h>

struct dirent *readdir(DIR *dir);
struct dirent *readdir64(DIR *dir);
int readdir_r(DIR *dir, struct dirent *buf, struct dirent **result);
int readdir64_r(DIR *dir, struct dirent *buf, struct dirent **result);
```

## Arguments

- **dir**
An opened directory stream.
&nbsp;
- **buf**
A memory buffer specified to store the returned struct dirent entry.
&nbsp;
- **result**
A pointer to store the address of *buf*.

## Library
libc

## Description

readdir() function returns a pointer of a dirent structure representing the next directory entry in the directory stream *dir*. It returns NULL on reaching the end of the directory stream or if an error occurred. The data returned by readdir() may be overwritten by subsequent calls to readdir() for the same directory stream. The dirent structure is defined as follows:
```c
	struct dirent {
		ino_t d_ino;
		off_t d_off;
		unsigned short d_reclen;
		unsigned char d_type;
		char d_name[256];
	};
```
The readdir_r() function is a reentrant version of readdir(). It reads the next directory entry from the directory stream *dir*, copies the next directory entry to the *buf*, and stores the pointer of *buf* to *result*.

Regards to 64 version, they follow:
```c
#define readdir64 readdir
#define readdir64_r readdir_r
```

## Returns
On success, readdir() returns a pointer to a dirent structure or NULL (if reaching the end of the directory stream ). On error, NULL is returned, and errno is set to indicate the error.
&nbsp;
On success, the readdir_r() function returns 0. On error, it returns a positive error number.  If the end of the directory stream is reached, readdir_r() returns 0, and returns NULL in *result*.

&nbsp;
## Errors
- **EFAULT**
The argument *dir* is NULL.
&nbsp;
- **ENOMEM**
Insufficient memory.
&nbsp;
- **EBADF**
The argument *dir* is not a valid directory stream.
&nbsp;
- **ENOTDIR**
The argument *dir* is not a directory stream.
&nbsp;
- **EOPNOTSUPP**
The operation is not supported.

## Example
```c
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(void)
{
	DIR *dirp;
	struct dirent *dp1 = malloc(sizeof(struct dirent));
	struct dirent *dp2 = NULL;

	if (dp1 == NULL) {
		return 0;
	}
	dirp = opendir(".");
	while (1) {
		if ((readdir_r(dirp,dp1,&dp2)) != 0) {
			perror("readdir_r");
			return 0;
		}
		if (dp2 == NULL) {
			break;
		}
		if (dp2->d_name[0] == '.') {
			continue;
		}
		printf("inode=%d\t", dp2->d_ino);
		printf("reclen=%d\t", dp2->d_reclen);
		printf("name=%s\n", dp2->d_name);
	}
	close(dirp);
	free(dp1);
	return 0;
}
```
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
