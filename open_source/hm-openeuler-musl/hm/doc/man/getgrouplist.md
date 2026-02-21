# getgrouplist()
Determine group access list for a user

## Synopsis
```c
#include <grp.h>

int getgrouplist(const char *user, gid_t gid, gid_t *groups, int *ngroups)
```
## Arguments
- **user**
The name of user.
&nbsp;
- **gid**
The gid is automatically included in list of groups. Typically this value is given as group number from password file.
&nbsp;
- **groups**
A pointer to an array where function can store group IDs.
&nbsp;
- **ngroups**
A pointer to size of *groups* array. The function sets value pointed to by *ngroups* to be actual number of *groups* found.

## Library
libc

## Description
The getgrouplist() function reads group file and determines group access list for user specified in name.

## Returns
- **0**
Success; function fills in the group array and sets *ngroups* to the number of *groups* found.
&nbsp;
- **-1**
The *groups* array is too small to hold all user's groups. The function fills group array with as many groups as fit.

## Errors
N/A

## Examples
The program below displays group list for user named in its first command-line argument. The second command-line argument specifies ngroups value to be supplied to getgrouplist(). The following shell session shows examples of use of this program:
&nbsp;
    $ ./a.out cecilia 0
    getgrouplist() returned -1; ngroups = 3
    $ ./a.out cecilia 3
    ngroups = 3
    16 (dialout)
    33 (video)
    100 (users)
&nbsp;
```c
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
	int j, ngroups;
	gid_t *groups = NULL;
	struct passwd *pw = NULL;
	struct group *gr = NULL;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <user> <ngroups>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	ngroups = atoi(argv[2]);

	groups = malloc(ngroups * sizeof (gid_t));
	if (groups == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	/* Fetch passwd structure (contains first group ID for user) */
	pw = getpwnam(argv[1]);
	if (pw == NULL) {
		perror("getpwnam");
		exit(EXIT_SUCCESS);
	}

	/* Retrieve group list */
	if (getgrouplist(argv[1], pw->pw_gid, groups, &ngroups) == -1) {
		fprintf(stderr, "getgrouplist() returned -1; ngroups = %d\n", ngroups);
		exit(EXIT_FAILURE);
	}

	/* Display list of retrieved groups, along with group names */
	fprintf(stderr, "ngroups = %d\n", ngroups);
	for (j = 0; j < ngroups; j++) {
		printf("%d", groups[j]);
		gr = getgrgid(groups[j]);
		if (gr != NULL) {
			printf(" (%s)", gr->gr_name);
		}
		printf("\n");
	}

	exit(EXIT_SUCCESS);
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
