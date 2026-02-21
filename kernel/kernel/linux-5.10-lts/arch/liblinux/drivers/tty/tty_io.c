// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <linux/tty.h>
#include <linux/string.h>
#include <trace/hooks/liblinux.h>

#include "tty.h"

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

struct tty_struct *__lookup_tty(dev_t device)
{
	struct tty_struct *tty = NULL;
	struct tty_driver *driver = NULL;
	struct tty_driver *p;
	int index = -1;
	struct file file;

	mutex_lock(&tty_mutex);

	list_for_each_entry(p, &tty_drivers, tty_drivers) {
		dev_t base = MKDEV(p->major, p->minor_start);
		if (device < base || device >= base + p->num)
			continue;
		index = device - base;
		driver = tty_driver_kref_get(p);
	}

	if (!driver) {
		mutex_unlock(&tty_mutex);
		return NULL;
	}

	if (!driver->ops->lookup) {
		tty = driver->ttys[index];
	} else {
		if (driver->type == TTY_DRIVER_TYPE_PTY &&
		    driver->subtype == PTY_TYPE_SLAVE)
			tty = driver->ops->lookup(driver, &file, index);
	}

	if (!IS_ERR(tty))
		tty_kref_get(tty);

	mutex_unlock(&tty_mutex);
	tty_driver_kref_put(driver);
	return tty;
}

#ifdef CONFIG_LIBLINUX_TTY_FLUSH_SYNC
static void hook_tty_port_cfg_sync_flush(void *__data, struct tty_port *port)
{
	set_bit(TTY_PORT_FLUSH_SYNC, &port->iflags);
}
INIT_VENDOR_HOOK(ldk_vh_tty_port_cfg_sync_flush, hook_tty_port_cfg_sync_flush);
#endif
