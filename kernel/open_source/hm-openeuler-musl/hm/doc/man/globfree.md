# globfree()
Used to free memory from glob()

## Synopsis
```c
#include <glob.h>;

void globfree(glob_t *pglob);
```
## Arguments
- **pglob**
Point to the glob_t structure to be freed.

## Library
libc

## Description
The globfree() function is used to free the allocated memory from glob().

## Returns
N/A

## Errors
N/A

## Example
```c
#include <unistd.h>
#include <stdio.h>
#include <glob.h>

int main(void)
{
	glob_t paths;
	int retval;

	paths.gl_pathc = 0;
	paths.gl_pathv = NULL;
	paths.gl_offs = 0;

	retval = glob("*.c", GLOB_NOCHECK | GLOB_NOSORT, NULL, &paths);
	if (retval == 0) {
		int idx;

		for (idx = 0; idx < paths.gl_pathc; idx++) {
			printf("[%d]: %s\n", idx, paths.gl_pathv[idx]);
		}

		globfree(&paths);
	} else {
		puts("glob() failed");
	}

	return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
