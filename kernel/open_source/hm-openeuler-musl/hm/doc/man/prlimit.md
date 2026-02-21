# prlimit()
Get/set resource limits

## Synopsis
```c
#include <sys/resource.h>

int prlimit(pid_t pid, int resource, const struct rlimit *new_limit,
            struct rlimit *old_limit);
```

## Arguments

- **pid**
The ID of process you want to operate.
&nbsp;
- **resource**
The resource type you want to access.
&nbsp;
- **new_limit**
The resource limits to be set.
&nbsp;
- **old_limit**
The resource limits to be get.

## Library
libc

## Description
The prlimit() function can be used to get/set the resource limits. If the *new_limit* is not NULL, it will be used to set the new value of the limit. If the *old_limit* is not NULL, it will be set to the previous value of the limit.
&nbsp;

The resource type and structure rlimit is the same as getrlimit(), setrlimit().
&nbsp;

If pid is 0 it operate on the calling process itself. To operate on the process other than it self, the caller need to have the ability ab_rlimit and capability of **POSIX_CAP_SYS_RESOURCE** or the real, effective, and saved user IDs of the target process must match the real user ID of the caller process and saved group IDs of the target process must match the real group ID of the caller.

## Returns

- **0**
The system call success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
Invalid resource type.
&nbsp;
- **EPERM**
No permission to raise the hard limit or to set the limits of other process.
&nbsp;
- **ESRCH**
No such process of the specified *pid*.

## Example

```C
#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/resource.h>

int main(int argc, char *argv[])
{
    int rc;
    struct rlimit rlim;

    rc = prlimit(0, RLIMIT_NOFILE, NULL, &rlim);
    if (rc < 0) {
        exit(-1);
    }

    rlim.rlim_cur = 4096;
    rc = prlimit(0, RLIMIT_NOFILE, &rlim, NULL);
    if (rc < 0) {
        exit(-2);
    }

    return 0;
}
```

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
