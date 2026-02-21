# getrusage()
Get information about resource utilization

## Synopsis
```c
#include <sys/time.h>
#include <sys/resource.h>

int getrusage(int who, struct rusage *usage);
```
## Arguments
- **who**
Specify whose resource usage to get.
&nbsp;
- **usage**
Pointer to a *rusage* structure to return the resource usages. The *rusage* structure is defined as follows:

```c
struct rusage {
	struct timeval ru_utime; /* user CPU time used */
	struct timeval ru_stime; /* system CPU time used */
	long	ru_maxrss;       /* maximum resident set size */
	long	ru_ixrss;        /* integral shared memory size */
	long	ru_idrss;        /* integral unshared data size */
	long	ru_isrss;        /* integral unshared stack size */
	long	ru_minflt;       /* page reclaims (soft page faults) */
	long	ru_majflt;       /* page faults (hard page faults) */
	long	ru_nswap;        /* swaps */
	long	ru_inblock;      /* block input operations */
	long	ru_oublock;      /* block output operations */
	long	ru_msgsnd;       /* IPC messages sent */
	long	ru_msgrcv;       /* IPC messages received */
	long	ru_nsignals;     /* signals received */
	long	ru_nvcsw;        /* voluntary context switches */
	long	ru_nivcsw;       /* involuntary context switches */
	long    __reserved[16];
};
```

## Library
libc

## Description
The getrusage() function returns resource usage measures for *who*, which can be one of the following:
- **RUSAGE_SELF**
Return resource usage statistics for the calling process.
&nbsp;
- **RUSAGE_CHILDREN**
Return resource usage statistics for all children of the calling process that have terminated and been waited for.
&nbsp;
- **RUSAGE_THREAD**
Return resource usage statistics for the calling thread.
&nbsp;
The *ru_stime*, *ru_idrss*, *ru_isrss*, *ru_nswap*, *ru_inblock*, *ru_oublock*, *ru_msgsnd*, *ru_msgrcv* and *ru_nsignals* members of *rusage* structure are not supported. For RUSAGE_CHILDREN, the *ru_utime* member is not supported either. The rusage of the original process couldn't be inherited to the new process when call execve(), which needs to be fixed.
## Returns
- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors
- **EFAULT**
*usage* points outside the accessible address space.
&nbsp;
- **EINVAL**
*who* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
