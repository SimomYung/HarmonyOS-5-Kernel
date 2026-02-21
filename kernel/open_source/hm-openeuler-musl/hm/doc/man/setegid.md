# setegid()
Set effective group ID for a process

## Synopsis
```c
#include <unistd.h>

int setegid(gid_t egid);
```
## Arguments
- **egid**
Effective group ID that you want to use for process.

## Library
libc

## Description
The setegid() function lets calling process set effective group ID based on following:
&nbsp;
If process has capability(POSIX\_CAP\_SETGID), the setegid() function sets effective group ID to *egid*.
&nbsp;
If process doesn't have capability(POSIX\_CAP\_SETGID), but *egid* is equal to real group ID or saved set-group ID, setegid() sets effective group ID to *egid*.

## Returns
Zero for success, or -1 if an error occurs.

## Errors
- **EINVAL**
Value of *egid* is out of range.
&nbsp;
- **EPERM**
Process doesn't have capability(POSIX\_CAP\_SETGID), and *egid* doesn't match real group ID or saved set-group ID.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
