#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <spawn.h>
#include <errno.h>
#include "pthread_impl.h"
#include "hm_internal.h"

extern char **__environ;

static struct sigaction oldint, oldquit;
static int sa_refcnt;
static pthread_mutex_t sa_lock;

int system(const char *cmd)
{
	pid_t pid;
	sigset_t old, reset;
	struct sigaction sa = { .sa_handler = SIG_IGN };
	int status = -1, ret;
	posix_spawnattr_t attr;

	pthread_testcancel();

	if (!cmd) return 1;

	pthread_mutex_lock(&sa_lock);
	if (sa_refcnt == 0) {
		sigaction(SIGINT, &sa, &oldint);
		sigaction(SIGQUIT, &sa, &oldquit);
	}
	sa_refcnt++;
	pthread_mutex_unlock(&sa_lock);

	sigaddset(&sa.sa_mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &sa.sa_mask, &old);

	sigemptyset(&reset);
	if (oldint.sa_handler != SIG_IGN) sigaddset(&reset, SIGINT);
	if (oldquit.sa_handler != SIG_IGN) sigaddset(&reset, SIGQUIT);
	posix_spawnattr_init(&attr);
	posix_spawnattr_setsigmask(&attr, &old);
	posix_spawnattr_setsigdefault(&attr, &reset);
	posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGDEF|POSIX_SPAWN_SETSIGMASK);
#ifdef __arm__
	ret = hm_process_spawn(&pid, "/bin/sh", 0, &attr,
			       (char *[]){"/bin/sh", "-c", (char *)cmd, 0}, __environ);
#else
	ret = posix_spawn(&pid, "/bin/sh", 0, &attr,
		(char *[]){"/bin/sh", "-c", (char *)cmd, 0}, __environ);
#endif
	posix_spawnattr_destroy(&attr);

	if (!ret) while (waitpid(pid, &status, 0)<0 && errno == EINTR);

	pthread_mutex_lock(&sa_lock);
	sa_refcnt--;
	if (sa_refcnt == 0) {
		sigaction(SIGINT, &oldint, NULL);
		sigaction(SIGQUIT, &oldquit, NULL);
	}
	pthread_mutex_unlock(&sa_lock);

	sigprocmask(SIG_SETMASK, &old, NULL);

	if (ret) errno = ret;
	return status;
}
