# pthread\_spin\_unlock()
Unlock a spin lock

## Synopsis
```c
#include <pthread.h>

int pthread_spin_unlock(pthread_spinlock_t *lock);
```

## Arguments

- **lock**
The pointer to the spin lock.

## Library
libc

## Description
The pthread\_spin\_unlock() function will unlock the spin lock. If there are spinning threads waiting for the spin lock, one of the threads will acquire the spin lock after it is unlocked.
&nbsp;
Calling pthread\_spin\_unlock() if the spin lock is uninitialized results in undefined behavior.

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
