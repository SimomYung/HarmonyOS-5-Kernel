// SPDX-License-Identifier: GPL-2.0-only
#ifndef __TTY_H__
#define __TTY_H__

#include <linux/tty.h>

extern struct list_head tty_drivers;
extern struct mutex tty_mutex;

struct tty_struct *__lookup_tty(dev_t device);

#endif /* __TTY_H__ */
