# pthread\_sigmask()
Examine and change signal mask

## Synopsis
```c
#include <signal.h>

int pthread_sigmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);
```

## Arguments

- **how**
The way to change the signal mask.
&nbsp;
- **set**
The pointer of the signal set to be used to change the signal mask.
&nbsp;
- **oset**
The pointer of the signal set to store the old signal mask.

## Library
libc

## Description
The pthread\_sigmask() function can examine and change the signal mask of the calling thread. The argument *how* should be one of the following values:
&nbsp;
- **SIG\_BLOCK**
The new signal mask of the calling thread will be the union of current set and the set pointed to by *set*.
&nbsp;
- **SIG\_SETMASK**
The new signal mask of the calling thread will be the set pointed to by *set*.
&nbsp;
- **SIG\_UNBLOCK**
The new signal mask of the calling thread will be the intersection of current set and the complement of the set pointed to by *set*.
&nbsp;
If the argument *set* is NULL, the signal mask of the calling thread will not be changed and the value of *how* will be ignored. If the argument *oset* is NULL, the old signal mask will not be enquired. If there are any pending unblocked signals after the call to pthread\_sigmask(), at least one of those signals will be delivered before the call of pthread\_sigmask() returns. The signal mask will not be changed if pthread\_sigmask() function fails.
&nbsp;
It is not allowed to block SIGKILL or SIGSTOP. It will have no effect if try to block SIGKILL or SIGSTOP and no error will be returned.
&nbsp;
An invalid *set* or *oset* may cause segmentation fault.

## Returns

- **0**
Success.
&nbsp;
- **EINVAL**
The value of *how* is not one of the defined values.

## Errors
N/A

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
