# getgrgid\_r()
Get the details of group structure with the specified gid

## Synopsis
```c
#include <sys/types.h>

#include <grp.h>

int getgrgid_r(gid_t gid, struct group *grp, char *buf, size_t size, struct group **res);
```
## Arguments

- **gid**
The group ID stored in the local group file /etc/group.
&nbsp;
- **grp**
A pointer to a group structure where the function stores group information.
&nbsp;
- **buf**
The buffer to store the string fields pointed to by the members of the group structure.
&nbsp;
- **size**
The size of *buf*.
&nbsp;
- **res**
A pointer to the result (in case of success) or NULL (in case no entry was found or an error
 occurred).

## Library
libc

## Description
The getgrgid\_r() function updates the data structure pointed to by *grp* and store a pointer to the structure at the location pointed to by *res*. The structure is matched with the specified *gid*. The storage referenced by the group structure is the memory of *bufsize* bytes allocated in advance for the buffer parameter *buf*. Generally call sysconf (\_SC\_GETGR\_R\_SIZE\_MAX) to set the buffer size. If it returns -1, the buffer size will not be set and needs to be set manually. If the group is not found or an error occurs, the function returns a null pointer.

## Returns

- **0**
The function succeeded to get the details of group structure with the specified *gid*.
&nbsp;

- **EIO**
An I/O error has occurred.
&nbsp;

- **EMFILE**
The maximum allowable number of open file descriptors for the process has reached the limit.
&nbsp;

- **ENFILE**
The maximum allowable number of files has reached the limit in the system.
&nbsp;

- **ERANGE**
The buffer *buf* supplied is insufficient.

## Errors

N/A

## Examples
Note that sysconf(\_SC\_GETGR\_R\_SIZE\_MAX) may return −1 if there is no hard limit on the size of the buffer needed to store all the groups returned. This example attempts to get the details of the group structure using *gid* 42 and shows how the application allocates a buffer of sufficient size to work with getgrid\_r().
```c
	#include <sys/types.h>
	#include <grp.h>
	#include <stdio.h>
	...
	int e;
	struct group result;
	struct group *resultp = NULL;
	char *buffer = NULL;
	long int initlen = sysconf(_SC_GETGR_R_SIZE_MAX);
	size_t len;
	size_t newlen;
	char *newbuffer = NULL;

	if (initlen == −1) {
		/* Default initial length. */
		len = 1024;
	} else {
		len = (size_t) initlen;
	}

	buffer = malloc(len);
	if (buffer == NULL)
		...handle error...

	while ((e = getgrgid_r(42, &result, buffer, len, &resultp)) == ERANGE) {
		newlen = 2 * len;
		if (newlen < len)
			...handle error...
		len = newlen;
		newbuffer = realloc(buffer, len);
		if (newbuffer == NULL)
			...handle error...
		buffer = newbuffer;
	}

	if (e != 0)
		...handle error...
	free (buffer);
```
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
