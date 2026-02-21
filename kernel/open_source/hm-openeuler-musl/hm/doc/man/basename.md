# basename()
Obtain the basename

## Synopsis
```c
#include <libgen.h>

char *basename(char *s);
```

## Arguments

- **s**
The input parameter.


## Library
libc

## Description
Function basename() obtains the basename of *s*. There is no restricts on the input *s*, it can equal NULL.


## Returns
- **char\***
Pointer to the base name of the input string.

## Errors
N/A

## Example
```c
#include <stdio.h>
#include <libgen.h>

int main()
{
	char *p;
	p = basename("dir1/dir2/test.txt");
	printf("%s\n", p);
	return 0;
}

The output is: test.txt
```
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
