#define _GNU_SOURCE
#include <utmpx.h>
#include <utmp.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/file.h>
#include <limits.h>

static int ut_fd = -1;
static struct utmp utmp_buffer;
static const char default_ufile[] = _PATH_UTMP;
static const char *ut_file = default_ufile;
static const char default_wfile_name[] = _PATH_WTMP;
static pthread_mutex_t ut_mutex = PTHREAD_MUTEX_INITIALIZER;

void endutxent(void)
{
	if (ut_fd >= 0) {
		(void)pthread_mutex_lock(&ut_mutex);
		close(ut_fd);
		(void)pthread_mutex_unlock(&ut_mutex);
		ut_fd = -1;
	}
}

void setutxent(void)
{
	char path[PATH_MAX];

	if (ut_fd < 0) {
		const char *file = (access(ut_file, F_OK) == 0) ? \
				ut_file : default_ufile;
		if (realpath(file, path) == NULL) {
			return;
		}
		ut_fd = open(path, O_RDONLY | O_CLOEXEC);
		if (ut_fd < 0)
			return;
	}
	(void)pthread_mutex_lock(&ut_mutex);
	(void)lseek(ut_fd, 0, SEEK_SET);
	(void)pthread_mutex_unlock(&ut_mutex);
}

static struct utmpx *local_getutxent(void)
{
	ssize_t read_size = 0;
	struct utmpx *result = NULL;
	char path[PATH_MAX];

	if (ut_fd < 0) {
		const char *file = (access(ut_file, F_OK) == 0) ? \
				ut_file : default_ufile;
		if (realpath(file, path) == NULL) {
			return NULL;
		}
		ut_fd = open(path, O_RDONLY | O_CLOEXEC);
		if (ut_fd < 0)
			return NULL;
		(void)lseek(ut_fd, 0, SEEK_SET);
	}

	read_size = read(ut_fd, &utmp_buffer, sizeof(struct utmpx));
	if (read_size != (ssize_t)sizeof(struct utmpx)) {
		result = NULL;
	} else {
		result = (struct utmpx *)&utmp_buffer;
	}

	return result;
}

struct utmpx *getutxent(void)
{
	struct utmpx *result = NULL;
	(void)pthread_mutex_lock(&ut_mutex);
	result = local_getutxent();
	(void)pthread_mutex_unlock(&ut_mutex);
	return result;
}

static int ut_is_process(const struct utmpx *ut)
{
	if (ut->ut_type < RUN_LVL || ut->ut_type > DEAD_PROCESS) {
		return -1;
	} else {
		switch(ut->ut_type) {
			case RUN_LVL:
			case BOOT_TIME:
			case NEW_TIME:
			case OLD_TIME:
				return 0;
			case INIT_PROCESS:
			case LOGIN_PROCESS:
			case USER_PROCESS:
			case DEAD_PROCESS:
				return 1;
			default:
				break;
		}
	}
	return -1;
}
struct utmpx *getutxid(const struct utmpx *ut)
{
	struct utmpx *result = NULL;
	int is_process = -1;

	(void)pthread_mutex_lock(&ut_mutex);
	is_process = ut_is_process(ut);
	if (is_process < 0) {
		errno = EINVAL;
	} else {
		for (; (result = local_getutxent()) != NULL; ) {
			if (((is_process > 0) && (strcmp(result->ut_id, ut->ut_id) == 0))
			     || ((is_process == 0) && (result->ut_type == ut->ut_type))) {
				break;
			}
		}
	}
	(void)pthread_mutex_unlock(&ut_mutex);

	return result;
}

struct utmpx *getutxline(const struct utmpx *ut)
{
	struct utmpx *result = NULL;

	(void)pthread_mutex_lock(&ut_mutex);
	for ( ; (result = local_getutxent()) != NULL; ) {
		if ((result->ut_type == USER_PROCESS || result->ut_type == LOGIN_PROCESS)
			&& (strncmp(result->ut_line, ut->ut_line, sizeof(ut->ut_line)) == 0)) {
				break;
		}
	}
	(void)pthread_mutex_unlock(&ut_mutex);
	return result;
}

struct utmpx *pututxline(const struct utmpx *ut)
{
	struct utmpx *cur_utmpx = NULL;
	off_t first_offset = 0;
	ssize_t write_size = 0;
	int new_fd = -1;
	char path[PATH_MAX];

	if (flock(ut_fd, LOCK_EX) < 0) {
		return NULL;
	}

	errno = 0;
	cur_utmpx = getutxid(ut);
	if (cur_utmpx == NULL && errno == EINVAL) {
		goto return_unlock;
	}

	/* Only privileged processes have write access */
	const char *file = (access(ut_file, F_OK) == 0) ? ut_file : default_ufile;
	if (realpath(file, path) == NULL) {
		goto return_unlock;
	}
	new_fd = open(path, O_RDWR | O_CLOEXEC);
	if (new_fd < 0) {
		goto return_unlock;
	}

	if (cur_utmpx != NULL) {
		first_offset = lseek(ut_fd, 0, SEEK_CUR) - (off_t)sizeof(struct utmpx);
		if (first_offset >= 0) {
			(void)lseek(new_fd, first_offset, SEEK_SET);
		}
	} else {
		(void)lseek(new_fd, 0, SEEK_END);
	}

	write_size = write(new_fd, ut, sizeof(struct utmpx));
	if (write_size != (ssize_t)sizeof(struct utmpx)) {
		close(new_fd);
		goto return_unlock;
	} else {
		(void)lseek(ut_fd, lseek(new_fd, 0, SEEK_CUR), SEEK_SET);
		close(new_fd);
		(void)flock(ut_fd, LOCK_UN);
		return (struct utmpx *)ut;
	}
return_unlock:
	(void)flock(ut_fd, LOCK_UN);
	return NULL;
}

void updwtmpx(const char *f, const struct utmpx *u)
{
	int fd;
	char path[PATH_MAX];

	const char *file = (access(f, F_OK) == 0) ? \
				f : default_wfile_name;
	if (realpath(file, path) == NULL) {
		return;
	}
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		return;
	}

	if (flock(fd, LOCK_EX) < 0) {
		close(fd);
		return;
	}
	(void)lseek(fd, 0, SEEK_END);
	(void)write(fd, u, sizeof(struct utmpx));
	(void)flock(fd, LOCK_UN);

	if (fd >= 0) {
		close(fd);
	}
	return;
}

static int __utmpxname(const char *file)
{
	int result = -1;
	if (file == NULL) {
		errno = EINVAL;
		return -1;
	}

	if (strcmp(file, ut_file) == 0) {
		return 0;
	}

	(void)pthread_mutex_lock(&ut_mutex);
	if (strcmp(file, default_ufile) != 0) {
		char *name = strdup(file);
		if (name == NULL) {
			result = -1;
			goto return_unlock;
		}

		if (ut_file != default_ufile) {
			free((char *)ut_file);
		}

		ut_file = name;
	} else {
		if (ut_file != default_ufile) {
			free((char *)ut_file);
			ut_file = default_ufile;
		}
	}

	if (ut_fd >= 0) {
		close(ut_fd);
		ut_fd = -1;
	}
	result = 0;
return_unlock:
	(void)pthread_mutex_unlock(&ut_mutex);
	return result;
}

weak_alias(endutxent, endutent);
weak_alias(setutxent, setutent);
weak_alias(getutxent, getutent);
weak_alias(getutxent, getutent_r);
weak_alias(getutxid, getutid);
weak_alias(getutxline, getutline);
weak_alias(pututxline, pututline);
weak_alias(updwtmpx, updwtmp);
weak_alias(__utmpxname, utmpname);
weak_alias(__utmpxname, utmpxname);
