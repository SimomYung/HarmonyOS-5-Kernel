# pthread\_getspecific()
Get thread-specific data

## Synopsis
```c
#include <pthread.h>

void *pthread_getspecific(pthread_key_t key);
```

## Arguments

- **key**
The key value of the thread-specific data.

## Library
libc

## Description
The pthread\_getspecific() function can get the thread-specific data of the calling thread associated with the *key*.
&nbsp;
Using pthread\_getspecific() function for the *key* which is being destroyed will return NULL, unless the value of the *key* is changed by pthread\_setspecific() in the destructor routine. Using pthread\_getspecific() function for the *key* which is not obtained from pthread\_key\_create() results in undefined behavior. Using pthread\_getspecific() function for the *key* which has been deleted with pthread\_key\_delete() results in undefined behavior.

## Returns

- **data value**
The pthread\_getspecific() function will return the data value associated with the *key* or NULL if there is no data associated with the *key*.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
