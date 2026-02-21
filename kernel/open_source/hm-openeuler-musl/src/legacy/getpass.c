#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char *getpass(const char *prompt)
{
	int fd;
	struct termios s, t;
	ssize_t l;
	static char password[128];
	char *input_name = NULL;
	char *full_path = NULL;

	input_name = ttyname(fileno(stdin));
	if (input_name == NULL) {
		return 0;
	}

	full_path = realpath(input_name, NULL);
	if (full_path == NULL) {
		return 0;
	}

	if ((fd = open(full_path, O_RDWR|O_NOCTTY|O_CLOEXEC)) < 0) {
		free(full_path);
		return 0;
	}
	free(full_path);

	tcgetattr(fd, &t);
	s = t;
	t.c_lflag &= ~(ECHO|ISIG);
	t.c_lflag |= ICANON;
	t.c_iflag &= ~(INLCR|IGNCR);
	t.c_iflag |= ICRNL;
	tcsetattr(fd, TCSAFLUSH, &t);
	tcdrain(fd);

	dprintf(fd, "%s", prompt);

	l = read(fd, password, sizeof password);
	if (l >= 0) {
		if (l > 0 && password[l-1] == '\n' || l==sizeof password) l--;
		password[l] = 0;
	}

	tcsetattr(fd, TCSAFLUSH, &s);

	dprintf(fd, "\n");
	close(fd);

	return l<0 ? 0 : password;
}
