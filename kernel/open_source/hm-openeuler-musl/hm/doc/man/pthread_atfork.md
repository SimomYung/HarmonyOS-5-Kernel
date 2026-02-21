# pthread\_atfork()
Register handlers executed before and after fork

## Synopsis
```c
#include <pthread.h>

int pthread_atfork(void (*prepare)(void), void (*parent)(void),
		   void (*child)(void));
```

## Arguments

- **prepare**
The handler that is executed before fork.
&nbsp;
- **parent**
The handler that is executed in the parent process after fork.
&nbsp;
- **child**
The handler that is executed in the child process after fork.

## Library
libc

## Description
The pthread\_atfork() function register handlers which is executed when fork() is called. If *prepare* is not NULL, *prepare* will be executed before fork in the context of the calling thread. If *parent* is not NULL, *parent* will be executed after fork in the context of the calling thread which is belong to the parent process. If *child* is not NULL, *child* will be executed after fork in the context of the new thread which is belong to the child process.
&nbsp;
If pthread\_atfork() is called multiple times, *prepare* will be called in reverse order of registration, in the meanwhile *parent* and *child* will be called in the order of registration.

## Returns

- **0**
Success.
&nbsp;
- **-1**
Fail to allocate memory to record the handlers.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
