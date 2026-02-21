# pthread\_self()
Obtain descriptor of calling thread

## Synopsis
```c
#include <pthread.h>

pthread_t pthread_self(void);
```

## Arguments
N/A

## Library
libc

## Description
The pthread\_self() function returns the descriptor of calling thread.

## Returns

- **thread descriptor**
The pthread\_self() function always succeeds and returns the descriptor of calling thread.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
