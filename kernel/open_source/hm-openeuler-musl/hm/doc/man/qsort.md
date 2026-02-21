# qsort()
Sort a given array

## Synopsis
```c
#include <stdlib.h>

void qsort(void *base, size_t nelem, size_t size,
           int (*compar)(const void *, const void *));
```

## Arguments

- **base**
The starting address of the array to be sorted.
&nbsp;
- **nelem**
The number of elements in the array.
&nbsp;
- **size**
The size of an element.
&nbsp;
- **compar**
The comparison function, which is called to compare two elements.

## Library
libc

## Description
The qsort() function sorts an array using a specified comparison function to determine the order. Note that the order of equivalent elements is undefined.

The comparison function must return an integer greater than, equal to, or less than 0 if the first argument goes after, is equivalent, or goes before the second argument.

## Returns

N/A

## Errors

N/A

## Example

```c
#include <stdlib.h>
#include <stdio.h>

int comp(const void *p1, const void *p2)
{
        int a = *(int *)p1;
        int b = *(int *)p2;

        return a - b;
}

int main(void)
{
        int array[] = { 10, 5, 17, 1, 8, 5 };
        int n = sizeof(array) / sizeof(int);
        int i;

        qsort(array, n, sizeof(int), comp);

        for (i = 0; i < n; i++)
                printf("%d ", array[i]);
        printf("\n");

        return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety

TODO
