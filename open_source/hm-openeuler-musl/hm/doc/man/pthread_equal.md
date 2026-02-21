# pthread\_equal()
Compare two thread descriptors

## Synopsis
```c
#include <pthread.h>

int pthread_equal(pthread_t t1, pthread_t t2);
```

## Arguments

- **t1**
A thread descriptor to be compared.
&nbsp;
- **t2**
The other thread descriptor to be compared.

## Library
libc

## Description
The pthread\_equal() function compares two pthread\_t values specified by *t1* and *t2* and returns zero if they are identical.

## Returns

- **0**
*t1* and *t2* are the same thread descriptor.
&nbsp;
- **non-zero**
*t1* and *t2* are different thread descriptors.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
