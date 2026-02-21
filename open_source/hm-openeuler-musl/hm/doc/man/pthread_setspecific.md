# pthread\_setspecific()
Set thread-specific data

## Synopsis
```c
#include <pthread.h>

int pthread_setspecific(pthread_key_t key, const void *value);
```

## Arguments

- **key**
The key value of the thread-specific data.
&nbsp;
- **value**
The value associated with the key.

## Library
libc

## Description
The pthread\_setspecific() function can associate a thread-specific data with the *key*. For the different threads in the same process, different data can be bound to the same *key*. The *value* is typically a pointer to the memory that stores thread-specific data of the calling thread.
&nbsp;
Using pthread\_setspecific() function in the destructor routine of the thread-specific data may result in lost storage. Using pthread\_setspecific() function to set the *key* which is not obtained from pthread\_key\_create() results in undefined behavior. Using pthread\_setspecific() function to set the *key* which has been deleted with pthread\_key\_delete() results in undefined behavior.

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
