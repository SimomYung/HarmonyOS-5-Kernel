# pthread\_key\_delete()
Delete a thread-specific data key

## Synopsis
```c
#include <pthread.h>

int pthread_key_delete(pthread_key_t key);
```

## Arguments

- **key**
The thread-specific data key to be deleted.

## Library
libc

## Description
The pthread\_key\_delete() function deletes the thread-specific data key specified by *key*. Upon key deletion, the value NULL is associated with the key in all active threads. Any destructor function that has been previously associated with a deleted key will not be called at thread exit.
&nbsp;
Using *key* which has been deleted results in undefined behavior.

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
