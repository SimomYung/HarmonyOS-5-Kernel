# scandir()
Scan a directory and store the result(s)
## Synopsis
```c
#include <dirent.h>

int scandir(const char *path, struct dirent ***res,
            int (*sel)(const struct dirent *),
            int (*cmp)(const struct dirent **, const struct dirent **));
```

## Arguments

- **path**
The path of the directory that will be scanned.
&nbsp;
- **res**
The results are stored in this directory list.
&nbsp;
- **sel**
If *sel* is not NULL, the *sel* indicates the selection condition (works as a filter).
&nbsp;
- **cmp**
The selected results will be sorted using qsort with the comparison function *cmp*.

## Library
libc

## Description
The scandir() function performs the scanning work on directory *path*, then filter each entry by using *sel*, and sort the selected results, store all the available results in *res* at last.

## Returns

- **Positive number**
The function will return the number of all suitable directory entries.
&nbsp;
- **-1**
Operation failed since unexpected situations happened.


## Errors
- **ENOENT**
The *path* is invalid to find the entry.
&nbsp;
- **ENOMEM**
There is not enough memory during the operations in the function (unlikely to happen).
&nbsp;
- **ENOTDIR**
*path* is not a directory.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
