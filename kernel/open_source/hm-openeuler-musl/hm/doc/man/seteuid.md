# seteuid()
Set effective user ID

## Synopsis
```c
#include <unistd.h>

int seteuid(uid_t euid);
```
## Arguments
- **euid**
The effective user ID that you want to use for process.

## Library
libc

## Description
The seteuid() function lets calling process set effective user ID, based on following:
&nbsp;
If process has capability(POSIX\_CAP\_SETUID), the function sets the effective user ID by *euid*.
&nbsp;
If process doesn't have capability(POSIX\_CAP\_SETUID), the function sets the effective user ID, when *euid* is equal to the real user ID or the saved set-user ID or the effective user ID of the process.

## Returns
Zero for success, or -1 if an error occurs

## Errors
- **EINVAL**
Value of *euid* is out of range.
&nbsp;
- **EPERM**
Process doesn't have capability(POSIX\_CAP\_SETUID), and *euid* doesn't match real user ID or saved set-user ID.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO

## NOTES
This API is not enabled when system is compiled with CONFIG\_DAC off, Calling the API under this condition will return 0 and have no effect.
