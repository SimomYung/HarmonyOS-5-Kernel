# getgrgid()
Get the group structure with the specified gid

## Synopsis
```c
#include <sys/types.h>

#include <grp.h>

struct group *getgrgid(gid_t gid);
```
## Arguments

- **gid**
The group ID stored in the local group file /etc/group.

## Library
libc

## Description
The getgrgid() function gets a pointer to the group structure containing the fields of the record in the group database that matches the group ID *gid*.

## Returns

- **NULL**
The group matching the specified group ID can not be found.
&nbsp;

- **A pointer to the group structure**
The getgrgid() function successfully obtains a pointer to the group structure containing fields recorded in the group database that match the group ID *gid*.

## Errors

The getgrgid() functions may fail if:

- **EIO**    An I/O error has occurred.
&nbsp;

- **EINTR**  A signal was caught during getgrgid().
&nbsp;

- **EMFILE** The maximum allowable number of open file descriptors for the process has reached the limit.
&nbsp;

- **ENFILE** The maximum allowable number of files has reached the limit in the system.

## Examples
The following example uses getgrgid() to search the group database for a group ID that was previously stored in a stat structure, then prints out the group name if it is found. If the group is not found, the program prints the numeric value of the group for the entry.
```c
	#include <sys/types.h>
	#include <grp.h>
	#include <stdio.h>
	...
	struct stat statbuf;
	statbuf.st_gid = 0;
	struct group *grp;
	if ((grp = getgrgid(statbuf.st_gid)) != NULL) {
		printf(" %-8.8s", grp->gr_name);
	} else {
		printf(" %-8d", statbuf.st_gid);
	}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
