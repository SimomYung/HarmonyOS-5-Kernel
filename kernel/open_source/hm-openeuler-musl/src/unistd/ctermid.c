#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

char *ctermid(char *s)
{
	static char buf[TTY_NAME_MAX];
	int ret;

	ret = ttyname_r(fileno(stdin), buf, sizeof buf);
	if (ret != 0) {
		errno = ret;
		return NULL;
	}

	if (s == NULL) {
		return buf;
	} else {
		return strncpy(s, buf, strlen(buf) + 1);
	}
}
