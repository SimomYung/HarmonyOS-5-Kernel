// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <linux/tty.h>
#include <linux/string.h>

int __override tty_do_resize(struct tty_struct *tty, struct winsize *ws)
{
	/* Lock the tty */
	mutex_lock(&tty->winsize_mutex);
	if (!memcmp(ws, &tty->winsize, sizeof(*ws)))
		goto done;

	/* Signal the foreground process group */
	if (tty->hmpgrp != 0)
		liblinux_pal_process_signal(tty->hmpgrp, SIGWINCH);

	tty->winsize = *ws;
done:
	mutex_unlock(&tty->winsize_mutex);
	return 0;
}
