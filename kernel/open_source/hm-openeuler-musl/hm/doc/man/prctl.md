# prctl()
Operate on the calling process or thread

## Synopsis
```c
#include <sys/prctl.h>

int prctl(int option, unsigned long arg2, unsigned long arg3,
          unsigned long arg4, unsigned long arg5);
```

## Arguments

- **option**
A flag which describes what to do with the calling process or thread.
&nbsp;
- **arg2**
The first argument that may be needed.
&nbsp;
- **arg3**
The second argument that may be needed.
&nbsp;
- **arg4**
The third argument that may be needed.
&nbsp;
- **arg5**
The forth argument that may be needed.

## Library
libc

## Description
The prctl() function operates on the calling process or thread, what to do depends on the first argument *option*. The further arguments are optional and their meanings depend on *option*.
&nbsp;
The supported *option* and the corresponding requirement for the arguments are described as following (unused arguments are typically assigned to zero):
&nbsp;
- **PR\_GET\_NAME**
This option obtains the name of the calling thread and places the returned name in the buffer pointed to by *arg2*. Make sure that the length (in bytes) of the buffer should be equal to or more than 16 bytes. The returned name is null-terminated.
&nbsp;
- **PR\_SET\_NAME**
This option sets the name specified by *arg2* for a region of memory of the calling thread. Make sure that the name can be up to 16 bytes long (including the terminating byte). If the length of the name (including the terminating byte) exceeds 16 bytes, it will be silently truncated.
&nbsp;
- **PR\_SET\_VMA**
This option sets the name of the memory specified by *arg3* and *arg4*, using the value in the location pointed by *(char \*)arg5*.  The name can be up to 64 bytes long (including the terminating byte). The *arg2* argument should be PR\_SET\_VMA\_ANON\_NAME. The *arg3* argument should provide the beginning address of the memory. The *arg4* argument is the size of the memory.
&nbsp;
- **PR\_CAPBSET\_READ**
Return 1 if the capability specified in *arg2* is in the capability bounding set of the calling thread, or 0 otherwise.
&nbsp;
- **PR\_CAPBSET\_DROP**
Drop the capability specified by *arg2* from the capability bounding set of the calling thread, if the calling thread has the CAP\_SETPCAP capability. Any child of the calling thread will inherit the newly reduced bounding set.
&nbsp;
- **PR\_CAP\_AMBIENT**
This option obtains or resets the ambient capability set specified by *arg2* to the calling thread. The value of *arg2* must be one of the following:
&nbsp;
  - **PR\_CAP\_AMBIENT\_RAISE**
  Add the capability specified in *arg3* to the ambient set. The specified capability must already exist in both the permitted and the inheritable sets.
&nbsp;
  - **PR\_CAP\_AMBIENT\_LOWER**
  Remove the capability specified in *arg3* from the ambient set.
&nbsp;
  - **PR\_CAP\_AMBIENT\_IS\_SET**
  Return 1 if the capability specified in *arg3* is in the ambient set and 0 if it is not.
&nbsp;
  - **PR\_CAP\_AMBIENT\_CLEAR\_ALL**
  Clear all capabilities from the ambient set.
&nbsp;
- **PR\_GET\_SECUREBITS**
This option obtains the "securebits" flag of the calling thread.
&nbsp;
- **PR\_SET\_SECUREBITS**
This option sets the "securebits" flag of the calling thread to the value provided in *arg2*.
&nbsp;
- **PR\_GET\_KEEPCAPS**
This option obtains the state of the "keep capabilities" flag of the calling thread.
&nbsp;
- **PR\_SET\_KEEPCAPS**
This option sets the state of the "keep capabilities" flag of the calling thread.
&nbsp;
- **PR\_GET\_CHILD\_SUBREAPER**
This option obtains the "child subreaper" setting of the calling process.
&nbsp;
- **PR\_SET\_CHILD\_SUBREAPER**
This option sets the "child subreaper" attribute of the calling process if *arg2* is nonzero, otherwise unsets the attribute.
&nbsp;
- **PR\_GET\_DUMPABLE**
This option obtains the current state of the calling process's dumpable attribute.
&nbsp;
- **PR\_SET\_DUMPABLE**
This option sets the state of the "dumpable" attribute that determines whether core dumps are produced for the calling process upon delivery of a signal whose default behavior is to produce a core dump.
&nbsp;
- **PR\_GET\_NO\_NEW\_PRIVS**
This option obtains the value of the "no_new_privs" attribute for the calling process.
&nbsp;
- **PR\_SET\_NO\_NEW\_PRIVS**
This option sets calling process's "no_new_privs" attribute to the value in *arg2*. With "no_new_privs" set to 1, execve promises not to grant privileges to do anything that could not have been done without the execve call (for example, rendering the set-user-ID and set-group-ID mode bits, and file capabilities non-functional). Once set, the "no_new_privs" attribute cannot be unset. The setting of this attribute is inherited by children created by fork and preserved across execve.
&nbsp;
- **PR\_SET\_PDEATHSIG**
This option sets the parent death signal of the calling process to arg2 (either a signal value in the range 1..maxsig, or 0 to clear). The parent death signal is that the calling process will get when its parent thread dies. This value is cleared for the child of a fork and is preserved across execve. Currently, the value is not cleared when executing a set-user-ID or set-group-ID binary, or a binary that has associated capabilities.
&nbsp;
- **PR\_GET\_PDEATHSIG**
This option returns the current value of the parent death signal, in the location pointed to by (int *) arg2.

## Returns

- **non-negative integer**
On success, PR\_CAP\_AMBIENT\_IS\_SET, PR\_CAPBSET\_READ, PR\_GET\_DUMPABLE, PR\_GET\_KEEPCAPS, PR\_GET\_NO\_NEW\_PRIVS, and PR\_GET\_SECUREBITS return the nonnegative values described above.
&nbsp;
- **0**
All other *option* values return 0 on success.
&nbsp;
- **-1**
On error, -1 is returned, and errno is set appropriately.

## Errors

- **EFAULT**
*arg2* is an invalid address.
&nbsp;
- **EINVAL**
The value specified by *option* is invalid or it indicates an unsupported option. *arg2* has an invalid value or is not valid value for this *option*. *option* is PR\_CAP\_AMBIENT and an unused argument is nonzero. *arg2* is PR\_CAP\_AMBIENT\_LOWER, PR\_CAP\_AMBIENT\_RAISE, or PR\_CAP\_AMBIENT\_IS\_SET and *arg3* does not specify a valid capability. *option* is PR\_SET\_DUMPABLE and *arg2* is neither SUID\_DUMP\_DISABLE nor SUID\_DUMP\_USER. *option* is PR\_SET\_NO\_NEW\_PRIVS and *arg2* is not equal to 1 or *arg3*, *arg4*, or *arg5* is nonzero. *option* is PR\_GET\_NO\_NEW\_PRIVS and *arg2*, *arg3*, *arg4*, or *arg5* is nonzero. *option* is PR\_SET\_PDEATHSIG and *arg2* is not a valid signal number.
&nbsp;
- **EPERM**
*option* is PR\_SET\_SECUREBITS, PR\_CAPBSET\_DROP and the caller does not have the CAP\_SETPCAP capability. *option* is PR\_SET\_KEEPCAPS and the caller's SECBIT\_KEEP\_CAPS\_LOCKED securebit has been set. *option* is PR\_CAP\_AMBIENT and *arg2* is PR\_CAP\_AMBIENT\_RAISE, but either the capability specified in *arg3* does not exist in the process's permitted and inheritable capability sets, or the PR\_CAP\_AMBIENT\_LOWER securebit has been set.
&nbsp;
- **ENAMETOOLONG**
*option* is PR\_SET\_VMA and the name specified by *arg5* is too long.

## Example
&nbsp;

## Classification
Unix

## Function Safety
TODO
