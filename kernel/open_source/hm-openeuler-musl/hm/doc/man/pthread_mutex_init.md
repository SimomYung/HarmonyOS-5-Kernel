#  pthread\_mutex\_init()
Initialize a mutex

## Synopsis
```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *restrict mutex,
                       const pthread_mutexattr_t *restrict attr);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

## Arguments

- **mutex**
The pointer to the mutex.
&nbsp;
- **attr**
The pointer to the attributes used to initialize the mutex.

## Library
libc

## Description
The pthread\_mutex\_init() can initialize the mutex with attributes specified by *attr*. The mutex will get default attributes if attr is NULL or *attr* points to a default mutex attributes object. In addition, the macro PTHREAD\_MUTEX\_INITIALIZER can also give mutex default attributes, except that no error checks will be performed. The mutex will become initialized and unlocked after successful initialization.
&nbsp;
Using an uninitialized mutex results in undefined behavior. Referencing a destroyed mutex results in undefined behavior, unless it is reinitialized using pthread\_mutex\_init(). Using copies of mutex in calls to pthread\_mutex\_lock(), pthread\_mutex\_trylock(), pthread\_mutex\_timedlock(), pthread\_mutex\_unlock(), pthread\_mutex\_destroy() results in undefined behavior. Initializing an already initialized mutex results in undefined behavior. Passing an uninitialized *attr* argument to pthread\_mutex\_init() results in undefined behavior.

## Returns

- **0**
Success.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
