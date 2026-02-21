# sysconf()
Get configuration information at run time

## Synopsis
```c
#include <unistd.h>

long sysconf(int name);
```

## Arguments

- **name**
The *name* argument represents the system variable to be queried.

## Library
libc

## Description
POSIX allows an application to test at compile or run time whether certain options are supported, or what the value is of certain configurable constants or limits. At compile time this is done by including &lt;unistd.h&gt; and/or &lt;limits.h&gt; and testing the value of certain macros. At run time, one can ask for numerical values using the present function sysconf().  One can ask for numerical values that may depend on the filesystem in which a file resides using fpathconf() and pathconf().  One can ask for string values using confstr(). The values obtained from these functions are system configuration constants.  They do not change during the lifetime of a process.

The following lists the minimal set of system variables from &lt;limits.h&gt; or &lt;unistd.h&gt; that can be returned by sysconf(), and the symbolic constants defined in &lt;unistd.h&gt; that are the corresponding values used for *name*:
- **ARG_MAX - _SC_ARG_MAX**
  The maximum length of the arguments to the exec() family of functions. Must not be less than _POSIX_ARG_MAX.
&nbsp;
- **HOST_NAME_MAX - _SC_HOST_NAME_MAX**
  Maximum length of a hostname, not including the terminating null byte, as returned by gethostname(). Must not be less than _POSIX_HOST_NAME_MAX.
&nbsp;
- **LOGIN_NAME_MAX - _SC_LOGIN_NAME_MAX**
  Maximum length of a login name, including the terminating null byte. Must not be less than _POSIX_LOGIN_NAME_MAX.
&nbsp;
- **NGROUPS_MAX - _SC_NGROUPS_MAX**
  Maximum number of supplementary group IDs.
&nbsp;
- **clock ticks - _SC_CLK_TCK**
  The number of clock ticks per second. The corresponding variable is obsolete. It was of course called CLK_TCK.
&nbsp;
- **OPEN_MAX - _SC_OPEN_MAX**
  The maximum number of files that a process can have open at any time. Must not be less than _POSIX_OPEN_MAX.
&nbsp;
- **PAGESIZE - _SC_PAGESIZE**
  Size of a page in bytes. Must not be less than 1.
&nbsp;
- **PAGE_SIZE - _SC_PAGE_SIZE**
  A synonym for PAGESIZE/_SC_PAGESIZE. (Both PAGESIZE and PAGE_SIZE are specified in POSIX.)
&nbsp;
- **RE_DUP_MAX - _SC_RE_DUP_MAX**
The number of repeated occurrences of a BRE permitted by regexec() and regcomp().  Must not be less than _POSIX2_RE_DUP_MAX.
&nbsp;
- **SYMLOOP_MAX - _SC_SYMLOOP_MAX**
The maximum number of symbolic links seen in a pathname before  resolution returns ELOOP. Must not be less than  _POSIX_SYMLOOP_MAX.
&nbsp;
- **TTY_NAME_MAX - _SC_TTY_NAME_MAX**
The maximum length of terminal device name, including the  terminating null byte. Must not be less than  _POSIX_TTY_NAME_MAX.
&nbsp;
- **TZNAME_MAX - _SC_TZNAME_MAX**
The maximum number of bytes in a timezone name. Must not be less than _POSIX_TZNAME_MAX.
&nbsp;
- **_POSIX_VERSION - _SC_VERSION**
Indicates the year and month the POSIX.1 standard was approved in the format YYYYMML; the value 199009L indicates the Sept. 1990 revision.
&nbsp;
- **BC_BASE_MAX - _SC_BC_BASE_MAX**
Indicates the maximum obase value accepted by the bc() utility.
&nbsp;
- **BC_DIM_MAX - _SC_BC_DIM_MAX**
Indicates the maximum value of elements permitted in an array by bc().
&nbsp;
- **BC_SCALE_MAX - _SC_BC_SCALE_MAX**
Indicates the maximum scale value allowed by bc().
&nbsp;
- **BC_STRING_MAX - _SC_BC_STRING_MAX**
Indicates the maximum length of a string accepted by bc().
&nbsp;
- **COLL_WEIGHTS_MAX - _SC_COLL_WEIGHTS_MAX**
Indicates the maximum numbers of weights that can be assigned to an entry of the LC_COLLATE order keyword in the locale definition file.
&nbsp;
- **RE_DUP_MAX - _SC_RE_DUP_MAX**
The maximum number of repeated occurrences of a regular expression when the interval notation \{m,n\} is used.
&nbsp;
- **POSIX2_VERSION - _SC_2_VERSION**
Indicates the version of the POSIX.2 standard in the format of YYYYMML.
&nbsp;
- **- _SC_NPROC_ONLN**
&nbsp;
- **_POSIX_JOB_CONTROL - _SC_JOB_CONTROL**
The implementation supports job control. This symbol shall always be set to a value greater than zero.
&nbsp;
- **_POSIX_SAVED_IDS - _SC_SAVED_IDS**
Each process has a saved set-user-ID and a saved set-group-ID. This symbol shall always be set to a value greater than zero.
&nbsp;
- **_POSIX_TIMERS - _SC_TIMERS**
The implementation supports timers. This symbol shall always be set to the value 200809L.
&nbsp;
- **_POSIX_FSYNC - _SC_FSYNC**
The implementation supports the File Synchronization option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_MAPPED_FILES - _SC_MAPPED_FILES**
The implementation supports memory mapped Files. This symbol shall always be set to the value 200809L.
&nbsp;
- **_POSIX_MEMLOCK - _SC_MEMLOCK**
The implementation supports the Process Memory Locking option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_MEMLOCK_RANGE - _SC_MEMLOCK_RANGE**
The implementation supports the Range Memory Locking option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_MEMORY_PROTECTION - _SC_MEMORY_PROTECTION**
The implementation supports memory protection. This symbol shall always be set to the value 200809L.
&nbsp;
- **_POSIX_MESSAGE_PASSING - _SC_MESSAGE_PASSING**
The implementation supports the Message Passing option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_SEMAPHORES - _SC_SEMAPHORES**
The implementation supports semaphores. This symbol shall always be set to the value 200809L.
&nbsp;
- **_POSIX_SHARED_MEMORY_OBJECTS - _SC_SHARED_MEMORY_OBJECTS**
The implementation supports the Shared Memory Objects option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **AIO_LISTIO_MAX - _SC_AIO_LISTIO_MAX**
Maximum number of I/O operations in a single list I/O call supported by the implementation.
&nbsp;
- **AIO_MAX - _SC_AIO_MAX**
Maximum number of outstanding asynchronous I/O operations supported by the implementation.
&nbsp;
- **AIO_PRIO_DELTA_MAX - _SC_AIO_PRIO_DELTA_MAX**
The maximum amount by which a process can decrease its asynchronous I/O priority level from its own scheduling priority.
&nbsp;
- **DELAYTIMER_MAX - _SC_DELAYTIMER_MAX**
Maximum number of timer expiration overruns.
&nbsp;
- **MQ_PRIO_MAX - _SC_MQ_PRIO_MAX**
The maximum number of message priorities supported by the implementation.
&nbsp;
- **RTSIG_MAX - _SC_RTSIG_MAX**
Maximum number of realtime signals reserved for application use in this implementation.
&nbsp;
- **SEM_VALUE_MAX - _SC_SEM_VALUE_MAX**
The maximum value a semaphore may have.
&nbsp;
- **_POSIX2_C_BIND - _SC_2_C_BIND**
The implementation supports the C-Language Binding option. This symbol shall always have the value 200809L.
&nbsp;
- **IOV_MAX - _SC_IOV_MAX**
Maximum number of iovec structures that one process has available for use with readv() or writev().
&nbsp;
- **_POSIX_THREADS - _SC_THREADS**
The implementation supports threads. This symbol shall always be set to the value 200809L.
&nbsp;
- **PTHREAD_DESTRUCTOR_ITERATIONS - _SC_THREAD_DESTRUCTOR_ITERATIONS**
Maximum number of attempts made to destroy a thread's thread-specific data values on thread exit.
&nbsp;
- **PTHREAD_KEYS_MAX - _SC_THREAD_KEYS_MAX**
Maximum number of data keys that can be created by a process.
&nbsp;
- **PTHREAD_STACK_MIN - _SC_THREAD_STACK_MIN**
Minimum size in bytes of thread stack storage.
&nbsp;
- **_POSIX_THREAD_ATTR_STACKADDR - _SC_THREAD_ATTR_STACKADDR**
The implementation supports the Thread Stack Address Attribute option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_THREAD_ATTR_STACKSIZE - _SC_THREAD_ATTR_STACKSIZE**
The implementation supports the Thread Stack Size Attribute option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_THREAD_PRIORITY_SCHEDULING - _SC_THREAD_PRIORITY_SCHEDULING**
The implementation supports the Thread Execution Scheduling option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_THREAD_PROCESS_SHARED - _SC_THREAD_PROCESS_SHARED**
The implementation supports the Thread Process-Shared Synchronization option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_XOPEN_VERSION - _SC_XOPEN_VERSION**
Integer value indicating version of the X/Open Portability Guide to which the implementation conforms. The value shall be 700.
&nbsp;
- **_XOPEN_VERSION - _SC_XOPEN_XCU_VERSION**
IEEE Std 1003.1-2001/Cor 2-2004, item XSH/TC2/D6/135 is applied, removing _XOPEN_XCU_VERSION (and their associated _SC_* variables) from the DESCRIPTION and APPLICATION USAGE sections. Now it is equal to _SC_XOPEN_VERSION.
&nbsp;
- **_XOPEN_UNIX - _SC_XOPEN_UNIX**
The implementation supports the XSI option.
&nbsp;
- **_XOPEN_ENH_I18N - _SC_XOPEN_ENH_I18N**
The implementation supports the Issue 4, Version 2 Enhanced Internationalization Option Group. This symbol shall always be set to a value other than -1.
&nbsp;
- **_XOPEN_SHM - _SC_XOPEN_SHM**
The implementation supports the Issue 4, Version 2 Shared Memory Option Group. This symbol shall always be set to a value other than -1.
&nbsp;
- **_POSIX_BARRIERS - _SC_BARRIERS**
The implementation supports barriers. This symbol shall always be set to the value 200809L.
&nbsp;
- **_POSIX_CPUTIME - _SC_CPUTIME**
The implementation supports the Process CPU-Time Clocks option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_THREAD_CPUTIME - _SC_THREAD_CPUTIME**
The implementation supports the Thread CPU-Time Clocks option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_MONOTONIC_CLOCK - _SC_MONOTONIC_CLOCK**
The implementation supports the Monotonic Clock option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_READER_WRITER_LOCKS - _SC_READER_WRITER_LOCKS**
The implementation supports read-write locks. This symbol shall always be set to the value 200809L.
&nbsp;
- **_POSIX_SPIN_LOCKS - _SC_SPIN_LOCKS**
The implementation supports spin locks. This symbol shall always be set to the value 200809L.
&nbsp;
- **_POSIX_REGEXP - _SC_REGEXP**
The implementation supports the Regular Expression Handling option. This symbol shall always be set to a value greater than zero.
&nbsp;
- **_POSIX_SHELL - _SC_SHELL**
The implementation supports the POSIX shell. This symbol shall always be set to a value greater than zero.
&nbsp;
- **_POSIX_SPAWN - _SC_SPAWN**
The implementation supports the Spawn option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_TIMEOUTS - _SC_TIMEOUTS**
The implementation supports timeouts. This symbol shall always be set to the value 200809L.
&nbsp;
- **SYMLOOP_MAX - _SC_SYMLOOP_MAX**
Maximum number of symbolic links that can be reliably traversed in the resolution of a pathname in the absence of a loop.
&nbsp;
- **_POSIX_V6_ILP32_OFFBIG - _SC_V6_ILP32_OFFBIG**
The implementation provides a C-language compilation environment with 32-bit int, long, and pointer types and an off_t type using at least 64 bits.
&nbsp;
- **_POSIX_V6_LP64_OFF64 - _SC_V6_LP64_OFF64**
The implementation provides a C-language compilation environment with 32-bit int and 64-bit long, pointer, and off_t types.
&nbsp;
- **_POSIX_RAW_SOCKETS - _SC_RAW_SOCKETS**
The implementation supports the Raw Sockets option. If this symbol is defined in &lt;unistd.h&gt;, it shall be defined to be -1, 0, or 200809L. The value of this symbol reported by sysconf() shall either be -1 or 200809L.
&nbsp;
- **_POSIX_V7_ILP32_OFFBIG - _SC_V7_ILP32_OFFBIG**
The implementation provides a C-language compilation environment with 32-bit int, long, and pointer types and an off_t type using at least 64 bits.
&nbsp;
- **_POSIX_V7_LP64_OFF64 - _SC_V7_LP64_OFF64**
The implementation provides a C-language compilation environment with 32-bit int and 64-bit long, pointer, and off_t types.
&nbsp;

These values also exist, but may not be standard:
- **- _SC_PHYS_PAGES**
The number of pages of physical memory.  Note that it is possible for the product of this value and the value of _SC_PAGESIZE to overflow.
&nbsp;
- **- _SC_AVPHYS_PAGES**
The number of currently available pages of physical memory.
&nbsp;
- **- _SC_NPROCESSORS_CONF**
The number of processors configured.  See also get_nprocs_conf().
&nbsp;
- **- _SC_NPROCESSORS_ONLN**
The number of processors currently online (available).  See also get_nprocs_conf().
&nbsp;
- **- _SC_NZERO**
Inquire about the value used to internally represent the zero priority level for the process execution.
&nbsp;
- **- _SC_XBS5_ILP32_OFFBIG**
This value is the set of initial options to be given to the cc and c89 utilities to build an application using a programming model with 32-bit int, long, and pointer types, and an off_t type using at least 64 bits.
&nbsp;
- **- _SC_XBS5_LP64_OFF64**
This value is the set of initial options to be given to the cc and c89 utilities to build an application using a programming model with 64-bit int, long, pointer, and off_t types.
&nbsp;
- **- _SC_AIX_KERNEL_BITMODE**
The kernel bit mode. If the application is running on the 32-bit kernel, it returns 32. If running on the 64-bit kernel, it returns 64.

## Returns

- **-1**
On error, -1 is returned and *errno* is set to indicate the cause of the error (for example, EINVAL, indicating that *name* is invalid).
&nbsp;
If *name* corresponds to a maximum or minimum limit, and that limit is indeterminate, -1 is returned and *errno* is not changed. (To distinguish an indeterminate limit from an error, set *errno* to zero before the call, and then check whether *errno* is nonzero when -1 is returned.)
&nbsp;
If *name* corresponds to an option, a positive value is returned if the option is supported, and -1 is returned if the option is not supported.
&nbsp;
- **The current value of the option or limit**
Otherwise, the current value of the option or limit is returned. This value will not be more restrictive than the corresponding value that was described to the application in &lt;unistd.h&gt; or &lt;limits.h&gt; when the application was compiled.

## Errors

- **EINVAL**
The parameter *name* is invalid.

## Example
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
