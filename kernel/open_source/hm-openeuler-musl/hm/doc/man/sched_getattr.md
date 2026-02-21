# sched\_getattr()
Get scheduling policy and attributes

## Synopsis
```c
#include <sched.h>

int sched_getattr(pid_t pid, struct sched_attr *attr,
		  unsigned int size, unsigned int flags);
```

## Arguments

- **pid**
Process ID.
&nbsp;
- **attr**
The scheduling policy and attributes for the specified thread.
&nbsp;
- **size**
The size of the struct attr.
&nbsp;
- **flags**
The future extensions to the interface.
&nbsp;

## Library
libc

## Description
The sched_getattr() system call fetches the scheduling policy and the associated attributes for the thread whose ID is specified in pid.  If pid equals zero, the scheduling policy and attributes of the calling thread will be retrieved.
Now HongMeng supprts policy:
	SCHED_FIFO 	a first-in, first-out policy;
	SCHED_RR 	a round-robin policy;
	SCHED_OTHER 	the standard round-robin time-sharing policy.

The attr argument is a pointer to a structure that defines the new scheduling policy and attributes for the specified thread.  This structure has the following form:

           struct sched_attr {
               u32 size;              /* Size of this structure */
               u32 sched_policy;      /* Policy (SCHED_*) */
               u64 sched_flags;       /* Flags */
               s32 sched_nice;        /* Nice value (SCHED_OTHER,
                                         SCHED_BATCH) */
               u32 sched_priority;    /* Static priority (SCHED_FIFO,
                                         SCHED_RR) */
               /* Remaining fields are for SCHED_DEADLINE */
               u64 sched_runtime;
               u64 sched_deadline;
               u64 sched_period;

               /* uclamp */
               u32 sched_util_min;
               u32 sched_util_max;
           };

The fields of this structure are as follows:

- **size**       
This  field  should  be  set  to the size of the structure in bytes, as in sizeof(struct sched_attr).
&nbsp;
- **sched_policy**
This field specifies the scheduling policy, as one of the SCHED_* values listed above.
&nbsp;
- **sched_flags**
This field contains flags controlling scheduling behavior.  Only one such flag is currently defined: SCHED_FLAG_RESET_ON_FORK.
&nbsp;
- **sched_nice**
The nice value of SCHED_OTHER.  The nice value is a number in the range -20 (high priority) to +19 (low priority).
&nbsp;
- **sched_priority**
The static priority of SCHED_FIFO or SCHED_RR.  Priority range is [1, 99].
&nbsp;
- **sched_runtime**
Now not support.
&nbsp;
- **sched_deadline**
Now not support.
&nbsp;
- **sched_period**
Now not support.
&nbsp;
## Returns

- **0**
Success.
&nbsp;

- **-1**
An error occurred.

## Errors
- **ESRCH**
ESRCH  The thread whose ID is pid could not be found.
&nbsp;
- **EINVAL**
1)*Invalid arguments*:*pid* is negative, attr is NULL or flag is nonzero. 

2)size is invalid; that is, it is smaller than the initial version of the sched_attr structure or larger than the system page size.
&nbsp;
- **EFAULT**
EFAULT  Failed when copy to attr.
&nbsp;

## Example
&nbsp;
 
## Classification
Unix
 
## Function Safety
N/A
