# alphasort()
Compare two directories in alphabetical order

## Synopsis
```c
#include <dirent.h>

int alphasort(const struct dirent **a, const struct dirent **b);
```

## Arguments

- **a**
The pointer of the first input directory.
&nbsp;
- **b**
The pointer of the second input directory.

## Library
libc

## Description
The alphasort() function performs comparison between the two inputted directories' names.

## Returns

- **negative integer**
When the first directory's name (i.e., a\-\>d\_name) is smaller than the second (i.e., b\-\>d\_name).
&nbsp;
- **0**
Two names are treated as equal.
&nbsp;
- **positive integer**
If the first name is greater than the second.

## Errors

- **N/A**

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
