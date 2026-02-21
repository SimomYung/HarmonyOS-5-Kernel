/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __ARCH_LIBLINUX_DEVICE_H
#define __ARCH_LIBLINUX_DEVICE_H

#include <linux/notifier.h>

int liblinux_register_device_notifier(struct notifier_block *nb);

#define LIBLINUX_DEVICE_ADD	1U
#define LIBLINUX_DEVICE_DEL	2U

#endif /* __ARCH_LIBLINUX_DEVICE_H */
