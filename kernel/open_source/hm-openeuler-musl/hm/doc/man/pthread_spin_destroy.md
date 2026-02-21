# pthread\_spin\_destroy()
Destroy a spin lock

## Synopsis
```c
#include <pthread.h>

int pthread_spin_destroy(pthread_spinlock_t *lock);
```

## Arguments

- **lock**
The pointer to the spin lock.

## Library
libc

## Description
The pthread\_spin\_destroy() destroys the spin lock, meaning the spin lock is no longer usable.
&nbsp;
Destroying a spin lock held by other thread results in undefined behavior. Referencing a destroyed spin lock results in undefined behavior, unless it is reinitialized using pthread\_spin\_init(). Using copies of the spin lock in calls to pthread\_spin\_lock(), pthread\_spin\_unlock(), pthread\_spin\_destroy() results in undefined behavior. Destroying a spin lock which is not initialized results in undefined behavior.

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
