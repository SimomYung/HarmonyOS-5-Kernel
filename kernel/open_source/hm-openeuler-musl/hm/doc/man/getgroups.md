# getgroups()
Get supplementary group IDs

## Synopsis
```c
#include <unistd.h>

int getgroups(int gidsetsize, gid_t grouplist[]);
```

## Arguments

- **gidsetsize**
The number of elements in the array *grouplist*.
&nbsp;
- **grouplist**
The array that saves the current supplementary group IDs of the calling process.

## Library
libc

## Description

The getgroups() function saves the current supplementary group IDs of the calling process in the array *grouplist*. The *gidsetsize* argument specifies the number of elements in *grouplist*. The real number of group IDs saved in *grouplist* will be returned. The values of array with index that is equal to or greater than the return value are unspecified.
&nbsp;

If *gidsetsize* is zero, the getgroups() function returns the number of group IDs with *grouplist* unmodified.
&nbsp;

If the effective group ID of the process is returned with the supplementary group IDs, the return value will be greater than or equal to one and less than or equal to {NGROUPS\_MAX}+1.

## Returns

- **the number of supplementary group IDs**
The function succeeds.
&nbsp;

- **-1**
An error occurred.

## Errors

- **EFAULT**
The *list* argument has an invalid address.
&nbsp;
- **EINVAL**
The *gidsetsize* argument is not zero and less than the number of supplementary group IDs that would have been returned.

## Example

**Getting the Supplementary Group IDs of the Calling Process**

The following example places the current supplementary group IDs of the calling process into the group array.

```c
#include <sys/types.h>
#include <unistd.h>

int main()
{
	gid_t *group;
	int ngroups;
	long ngroups_max;

	ngroups_max = sysconf(_SC_NGROUPS_MAX) + 1;
	group = (gid_t *)malloc(ngroups_max *sizeof(gid_t));
	if (group == NULL) {
		printf("malloc group failed.\n");
		return -1;
	}

	ngroups = getgroups(ngroups_max, group);
	printf("The number of supplementary groups is %d\n", ngroups);
	return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
