# puts()
Write a string with a trailing newline to stdout

## Synopsis
```c
#include <stdio.h>

int puts(const char *s);
```
## Arguments

- **s**
A string to be written to stdout.

## Library
libc

## Description
puts() writes a string with a trailing newline to stdout.


## Returns

On success, puts() returns a nonnegative number. On error, puts returns EOF.

## Errors

N/A

## Example
```c
#include <stdio.h>
int main()
{
	(void)puts("Hello");
	(void)puts("Word");
	return 0;
}

Hello
Word
```
## Classification
POSIX 1003.1-2017

## Function Safety
TODO
