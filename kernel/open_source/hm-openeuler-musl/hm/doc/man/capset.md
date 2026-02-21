# capset()

Set capabilities of thread(s)

## Synopsis
```c
#include <sys/capability.h>

int capset(cap_user_header_t hdrp, const cap_user_data_t datap);
```

## Arguments
- **hdrp**
The pointer to \_\_user\_cap\_header\_struct structure which specifies thread ID and capability version.
&nbsp;
- **datap**
The pointer to \_\_user\_cap\_data\_struct structure which is used for holding capabilities of target thread.

## Library

libc

## Description

This system call is used for setting capabilities of thread whose ID is specified by *hdrp->pid*. Note that only 64-bit capability set with version \_LINUX\_CAPABILITY\_VERSION\_3 is supported.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EFAULT**
Bad memory address. *hdrp* must not be NULL.
&nbsp;
- **EINVAL**
One of the arguments was invalid.
&nbsp;
- **ESRCH**
No such thread.
&nbsp;
- **EPERM**
The caller attempts to set the capabilities of a thread but lack sufficient privilege.

## Examples
&nbsp;

## Classification
Unix

## Function Safety
TODO
