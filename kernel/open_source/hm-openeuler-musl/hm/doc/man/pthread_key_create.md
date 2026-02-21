# pthread_key_create()
Create a thread-specific data key

## Synopsis
```c
#include <pthread.h>

int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
```

## Arguments

- **key**
A pointer to the buffer which stores the created thread-specific data key.
&nbsp;
- **destructor**
A function associated with *key* that will be called when the thread with *key* exits.

## Library
libc

## Description
The pthread\_key\_create() function creates a thread-specific data key associated with an optional destructor specified by *destructor*. Newly created key value is placed in the buffer pointed to by *key*. Although the key is visible to all threads within the process, each thread can bind the key with different thread-specific data through pthread\_setspecific() function.
&nbsp;
Upon key creation, the value NULL is associated with the new key in all active threads. Upon thread creation, the value NULL is associated with all defined (through pthread\_key\_create()) keys in the new thread.
&nbsp;
*destructor* is an optional function associated with *key* that will be called when the thread with *key* exits. When this function is invoked, the previously associated value with the *key* is passed as the single argument. The order of destructor calls is undefined if a thread has more than one thread-specific data key with non-NULL destructor.

## Returns

- **0**
Success.
&nbsp;
- **EAGAIN**
The system lacks enough resources to create another thread-specific data key.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
