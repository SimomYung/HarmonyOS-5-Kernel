# getpwnam\_r()
Get information about user with a given name

## Synopsis
```c
#include <sys/types.h>
#include <pwd.h>

int getpwnam_r(const char *name, struct passwd *pw, char *buf, size_t size, struct passwd **res)
```
## Arguments
- **name**
The name of user whose entry you want to find.
&nbsp;
- **pw**
A pointer to a passwd structure where function can store entry.
&nbsp;
- **buf**
A block of memory that function can use to allocate storage referenced by passwd structure.
&nbsp;
- **size**
The size of block that *buf* points to, in characters.
&nbsp;
- **res**
The address of a pointer to a passwd structure. If getpwnam_r() finds entry, it stores a pointer to *pw* in location indicated by *res*; otherwise function stores a NULL pointer there.

## Library
libc

## Description
The getpwnam\_r() function is a reentrant version of getpwnam(). It gets information about user with given name.
&nbsp;
The getpwnam\_r() function updates passwd structure pointed to by *pw* and stores a pointer to that structure at location pointed by *res*. The structure contains an entry from user database with given *name*.
&nbsp;
The function stores a NULL pointer at location pointed by *res* on error or if it can't find requested entry.

## Returns
Zero for success, or an error number.

## Errors
- **ERANGE**
Insufficient storage was supplied via *buf* and *size* to contain resulting passwd structure.

- **EIO**
Input output error during syscall.

- **ENOMEM**
Memory is not enough when creating certain structures.

## Examples
```c
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct passwd pwd;
	struct passwd *result = NULL;
	char *buf = NULL;
	size_t bufsize;
	int s;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s username\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1) {            /* Value was indeterminate */
		bufsize = 16384;        /* Should be more than enough */
	}

	buf = malloc(bufsize);
	if (buf == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	s = getpwnam_r(argv[1], &pwd, buf, bufsize, &result);
	if (result == NULL) {
		if (s == 0) {
			printf("Not found\n");
		} else {
			errno = s;
			perror("getpwnam_r");
		}
		exit(EXIT_FAILURE);
	}

	printf("Name: %s; UID: %ld\n", pwd.pw_gecos, (long) pwd.pw_uid);
	exit(EXIT_SUCCESS);
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
