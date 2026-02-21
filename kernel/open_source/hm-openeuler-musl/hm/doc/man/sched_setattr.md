# sched\_setattr()
Set scheduling policy and attributes

## Synopsis
```c
#include <sched.h>

int sched_setattr(pid_t pid, struct sched_attr *attr,
		  unsigned int flags);

```

## Arguments

- **pid**
Process ID.
&nbsp;
- **attr**
The new scheduling policy and attributes for the specified thread.


## Library
libc

## Description
The  sched_setattr() system call sets the scheduling policy and associated attributes for the thread whose ID is specified in pid.  If pid equals zero, the scheduling policy and
attributes of the calling thread will be set.
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

       size   This  field  should  be  set  to the size of the structure in bytes, as in sizeof(struct sched_attr).
       
       flags
	     This field can be set to SCHED_RESET_ON_FORK, SCHED_FLAG_UTIL_CLAMP_MIN, SCHED_FLAG_UTIL_CLAMP_MAX.

       sched_policy
              This field specifies the scheduling policy, as one of the SCHED_* values listed above. Now only support SCHED_RT, SCHED_OTHER.

       sched_flags
              This field contains flags controlling scheduling behavior.  Only one such flag is currently defined: SCHED_FLAG_RESET_ON_FORK.

       sched_nice
              The nice value of SCHED_OTHER.  The nice value is a number in the range -20 (high priority) to +19 (low priority).

       sched_priority
              The static priority of SCHED_FIFO or SCHED_RR.  Priority range is [1, 99].

       sched_runtime
	      Now not support.

       sched_deadline
	      Now not support.

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
1)*Invalid arguments*:*pid* is negative or flag is nonzero. 

2)attr->sched_flags now only support SCHED_RESET_ON_FORK. EINVAL will be returned if other value is set.

3)attr is NULL; attr->policy is not support; attr->priority is invalid; attr->nice is invalid.

4)size in attr is smaller than the sizeof(struct sched_attr), but the flags has been set to uclamp related value.
&nbsp;
- **EPERM**
The caller process doesn't have enough privileges.
- **E2BIG**
E2BIG  The buffer specified by size in attr is bigger than PAGE_SIZE or smaller than 48, which is the size of sched_attr in old version, or
	the size is bigger than the sizeof(struct sched_attr) bug the bits located outside are not zero.
&nbsp;
- **EFAULT**
EFAULT  Failed when copy to attr.
&nbsp;
