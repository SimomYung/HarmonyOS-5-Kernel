# bsearch()
Binary search of a sorted array

## Synopsis
```c
#include <stdlib.h>

void *bsearch(const void *key, const void *base,
              size_t nel, size_t width,
              int (*cmp)(const void *, const void *));
```

## Arguments
- **key**
The target to search for.
&nbsp;
- **base**
The first element in the array.
&nbsp;
- **nel**
The number of objects in the array.
&nbsp;
- **width**
The size of an element.
&nbsp;
- **cmp**
A callback function used to compare an element with the key.

## Library
libc

## Description
The bsearch() is used to search the key in element array by binary search.

## Returns
A pointer to a matching element of the array, or NULL if the key couldn't be found.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
