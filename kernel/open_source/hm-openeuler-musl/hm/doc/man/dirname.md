# dirname()
Obtain the directory name

## Synopsis
```c
#include <libgen.h>

char *dirname(char *s)
```

## Arguments

- **s**
The input parameter.


## Library
libc

## Description
Function dirname() obtains the directory name from *s*. If there were no '/' in *s*, then the '.' will be returned.


## Returns
- **char\***
Pointer to the directory name of the input string.

## Errors
Function will fail if *s* has 2 or more directory deepth, i.e., dirname("/test1/test2/file") will fail.

## Example
```c
#include <stdio.h>
#include <libgen.h>

int main()
{
	char *p;
	p = dirname("dir1/test.txt");
	printf("%s\n", p);
	return 0;
}

The output is: dir1
```
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
