# sigaction()
Examine and change a signal action

## Synopsis
```c
#include <signal.h>

int sigaction(int sig, const struct sigaction *restrict act,
	          struct sigaction *restrict oact);
```

## Arguments

- **sig**
The signum of a signal.
&nbsp;
- **act**
The new action to be set.
&nbsp;
- **oact**
The old action.

## Library
libc

## Description
The sigaction() function allows a process to examine or specify the action to be associated with a signal. The argument *sig* specifies the signal. The structure *sigaction* which is used to describe an action, is defined in the &lt;signal.h&gt; header.
&nbsp;
Note that you should pass a valid *act* to this function, invalid *act* (such as -1) may cause segmentation fault.
&nbsp;
If act is non-NULL, the new action for signal signum is installed from act. If oldact is non-NULL, the previous action is saved in oldact.
&nbsp;
The sigaction structure is defined as:
```c
struct sigaction {
	union {
		void     (*sa_handler)(int);
		void     (*sa_sigaction)(int, siginfo_t *, void *);
	} __sa_handler;
	sigset_t   sa_mask;
	int        sa_flags;
	void     (*sa_restorer)(void);
};
```
&nbsp;
When the SA_SIGINFO flag is specified in act.sa_flags, the signal handler address is passed via the act.sa_sigaction field. This handler takes three arguments, as follows:
```c
void
handler(int sig, siginfo_t *info, void *ucontext)
{
	...
}
```
&nbsp;
These three arguments are as follows:

- **sig**
The number of the signal that caused invocation of the handler.
&nbsp;
- **info**
A pointer to a siginfo_t, which is a structure containing further information about the signal, as described below.
&nbsp;
- **ucontext**
This is a pointer to a ucontext_t structure, cast to void *.
The structure pointed to by this field contains signal context information that was saved on the user-space stack by the kernel.
&nbsp;

The siginfo_t data type is a structure with the following fields:
```c
siginfo_t {
	int      si_signo;
	int      si_errno;
	int      si_code;
	int      si_trapno;
	pid_t    si_pid;
	uid_t    si_uid;
	int      si_status;
	clock_t  si_utime;
	clock_t  si_stime;
	union sigval si_value;
	void    *si_ptr;
	int      si_overrun;
	int      si_timerid;
	void    *si_addr;
	long     si_band;
	int      si_fd;
	short    si_addr_lsb;
	void    *si_lower;
	void    *si_upper;
	int      si_pkey;
	void    *si_call_addr;
	int      si_syscall;
	unsigned int si_arch;
}
```
si_signo, si_errno and si_code are defined for all signals.  The rest of the struct maybe a union.  Currently only si_signo, si_code, si_value and si_addr have valid values.
&nbsp;
Currently, only the following flags are supported to set in *act->sa\_flags*: SA\_ONSTACK, SA\_SIGINFO, SA\_NODEFER.

If the SA\_RESTART flag is used, a blocked call to interfaces interrupted by a signal which otherwise fails with EINTR is automatically restarted after the signal handler returns.  The following interfaces are never restarted after being interrupted by a signal regardless of the SA\_RESTART flag and always fail with EINTR:

- Input socket interfaces when a timeout (SO\_RCVTIMEO) has been set: **accept**, **recv**, **recvfrom**, **recvmmsg**, **recvmsg**.
&nbsp;
- Interfaces used to wait for signals: **pause**, **sigsuspend**, **sigtimedwait**, **sigwaitinfo**.
&nbsp;
- File descriptor multiplexing interfaces: **epoll_wait**, **epoll_pwait**, **poll**, **ppoll**, **select**, **pselect**.
&nbsp;
- System V IPC interfaces: **msgrcv**, **msgsnd**, **semop**, **semtimedop**.
&nbsp;
- Sleep interfaces: **clock_nanosleep**, **nanosleep**, **usleep**, **sleep**.

## Returns

- **0**
Success.
&nbsp;
- **-1**
An error occurred.

## Errors

- **EINVAL**
*sig* is invalid, or an attempt is made to change the action for SIGKILL or SIGSTOP, which cannot be caught or ignored. The signals 32~34 are occupied by musl, try to set action for 32~34 also returns EINVAL.

## Examples
&nbsp;

## Classification
POSIX 1003.1-2017

## Function Safety
TODO
