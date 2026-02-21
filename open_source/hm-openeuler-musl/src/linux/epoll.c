#include <sys/epoll.h>
#include <signal.h>
#include <errno.h>
#include "syscall.h"

static void dummy_post_epoll_ctl_handler(int fd, int op, int fd2, struct epoll_event *ev)
{
	/* do nothing */
}

weak_alias(dummy_post_epoll_ctl_handler, post_epoll_ctl_handler);

int epoll_create(int size)
{
	if (size <= 0) {
		errno = EINVAL;
		return -1;
	}

	return epoll_create1(0);
}

int epoll_create1(int flags)
{
	int r = __syscall(SYS_epoll_create1, flags);
#ifdef SYS_epoll_create
	if (r==-ENOSYS && !flags) r = __syscall(SYS_epoll_create, 1);
#endif
	return __syscall_ret(r);
}

int epoll_ctl(int fd, int op, int fd2, struct epoll_event *ev)
{
	int r = syscall(SYS_epoll_ctl, fd, op, fd2, ev);
	if (r == 0) {
		/*
		 * for performance reasons, we put some fd resources in user process
		 * space. for epoll_ctl, need to notify the class fd to process
		 * related resources.
		 */
		post_epoll_ctl_handler(fd, op, fd2, ev);
	}
	return r;
}

int epoll_pwait(int fd, struct epoll_event *ev, int cnt, int to, const sigset_t *sigs)
{
	int r = __syscall_cp(SYS_epoll_pwait, fd, ev, cnt, to, sigs, _NSIG/8);
#ifdef SYS_epoll_wait
	if (r==-ENOSYS && !sigs) r = __syscall_cp(SYS_epoll_wait, fd, ev, cnt, to);
#endif
	return __syscall_ret(r);
}

int epoll_wait(int fd, struct epoll_event *ev, int cnt, int to)
{
	return epoll_pwait(fd, ev, cnt, to, 0);
}
