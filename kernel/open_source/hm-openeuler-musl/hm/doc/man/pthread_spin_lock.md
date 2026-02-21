# pthread\_spin\_lock(), pthread\_spin\_trylock()
Lock a spin lock

## Synopsis
```c
#include <pthread.h>

int pthread_spin_lock(pthread_spin_lock_t *lock);
int pthread_spin_trylock(pthread_spin_trylock_t *lock);
```

## Arguments

- **lock**
The pointer to the spin lock.

## Library
libc

## Description
The pthread\_spin\_lock() function will return with the unlocked spin lock in locked state. If the spin lock is already locked by other thread, the calling thread will spin, testing the spin lock until the spin lock is unlocked. The pthread\_spin\_trylock() function is equivalent to pthread\_spin\_lock(), except that pthread\_spin\_trylock() will not spin and return EBUSY immediately if the spin lock is in locked state.
&nbsp;
Calling pthread\_spin\_lock() on a spin lock which is already held by the caller results in deadlock. Calling pthread\_spin\_lock(), pthread\_spin\_trylock() if the spin lock is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
The pthread\_spin\_trylock() shall return:
&nbsp;
- **EBUSY**
The spin lock is in locked state.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
