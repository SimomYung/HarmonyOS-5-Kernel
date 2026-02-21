# pthread\_mutex\_lock(), pthread\_mutex\_trylock()
Lock a mutex

## Synopsis
```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```

## Arguments

- **mutex**
The pointer to the mutex.

## Library
libc

## Description
The pthread\_mutex\_lock() function will return with the unlocked mutex in locked state and the mutex owner will be set to the calling thread. If the mutex is already locked by other thread, the calling thread will get blocked until the mutex is unlocked by its owner.
&nbsp;
The behavior of relock for different mutex type is described in the following table. Relock means that a thread tries to lock a mutex which has been locked by itself.
&nbsp;

|Mutex Type|Robust|Relock|
|----------|------|------|
| NORMAL/DEFAULT | N | deadlock |
| NORMAL/DEFAULT | Y | deadlock |
| ERRORCHECK | N/A | return error |
| RECURSIVE | N/A | recursive |

&nbsp;
For a mutex whose type is PTHREAD\_MUTEX\_RECURSIVE, it has a lock counter. The first time a thread locks it, the counter will be set to one and the mutex will be owned by the thread. Each time the owner relocks it, the counter will be incremented by one. Each time the mutex is unlocked by its owner, the counter will be decremented by one, until the counter reaches zero and the mutex will be available for other threads.
&nbsp;
The pthread\_mutex\_trylock() function is equivalent to pthread\_mutex\_lock(), except that pthread\_mutex\_trylock() will not get blocked and return EBUSY immediately if the mutex is in locked state. But if the mutex type is PTHREAD\_MUTEX\_RECURSIVE, EBUSY will not be returned and the mutex lock count will be incremented like pthread\_mutex\_lock() and the pthread\_mutex\_trylock() will return success.
&nbsp;
If the owning thread of a robust mutex terminated before unlock the robust mutex, a call to pthread\_mutex\_lock() will lock the mutex and EOWNERDEAD will be returned, but the mutex will be marked as inconsistent. It should be ensured that pthread\_mutex\_consistent() is called to recover this robust mutex consistent again. If it failed to recover, the mutex will be unusable permanently after being unlocked.
&nbsp;
If a blocked thread gets a signal when waiting for a mutex, it will resume waiting after handling the signal as if it is not interrupted.
&nbsp;
Calling pthread\_mutex\_lock(), pthread\_mutex\_trylock() if the mutex is uninitialized results in undefined behavior.

## Returns

- **0**
Success.
&nbsp;
- **EAGAIN**
The mutex type is PTHREAD\_MUTEX\_RECURSIVE and the maximum number of recursive locks has been exceeded.
&nbsp;
- **ENOTRECOVERABLE**
The mutex is not recoverable.
&nbsp;
- **EOWNERDEAD**
The mutex is a robust mutex and the owner thread terminated before unlocking the mutex. The calling thread will acquire the mutex and should make the mutex state consistent.
&nbsp;
The pthread\_mutex\_lock() shall return:
&nbsp;
- **EDEADLK**
The mutex type is PTHREAD\_MUTEX\_ERRORCHECK and the owner thread tries to lock it again, in which condition deadlock happens.
&nbsp;
The pthread\_mutex\_trylock() shall return:
&nbsp;
- **EBUSY**
The mutex is in locked state.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
