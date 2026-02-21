# glob()
Used to find pathname matching a pattern

## Synopsis
```c
#include <glob.h>;

int glob(const char *pat, int flags,
         int (*errfunc)(const char *path, int err),
         glob_t *g);
```
## Arguments
- **pat**
The pattern to be matched.
&nbsp;
- **flags**
Flags that affect the search.
&nbsp;
- **errfunc**
A callback function that glob() calls when it encounters a directory that it can't open or read.
&nbsp;
- **g**
A glob\_t structure to store information which glob() found.

## Library
libc

## Description
The glob() function finds pathnames matching the given pattern. glob() must have permission to search every directory and sub-directory.

The *errfunc* argument is a callback function with this prototype:
```c
    int errfunc(const char *path, int err);
```
- **path**
A pointer to the path where failed.
&nbsp;
- **err**
The value of errno from the failure.

If glob() should continue, the *errfunc* function should return 0; and the *errfunc* function should return a non-zero value if glob() should stop searching.

You can set *errfunc* to NULL to ignore these types of errors.

The flag argument can be more of the following bits:
- **GLOB_MARK**
Add a slash to each matching directory pathname.
&nbsp;
- **GLOB_NOSORT**
Do not sort the returned pathnames; The default sort is by the pathnames.
&nbsp;
- **GLOB_NOCHECK**
If *pattern* does not match any pathnames, return only the contents of *pattern*.
&nbsp;
- **GLOB_DOOFFS**
Use the value in glob_t->gl_offs to specify how many NULL pointers to add at the beginning of glob_t->gl_pathv.
&nbsp;
- **GLOB_APPEND**
Append found pathnames to the one from a previous call from glob().
&nbsp;
- **GLOB_NOESCAPE**
Disable backslash escapes in pattern.

## Returns
Zero for success, or an error value.
- **GLOB_NOMATCH**
The value of *pattern* does not match any existing pathnames.
&nbsp;
- **GLOB_ABORTED**
For a read error.
&nbsp;
- **GLOB_NOSCAPE**
For running out of memory.

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
