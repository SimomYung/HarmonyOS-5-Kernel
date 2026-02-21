/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_DEVICE_H
#define __LIBLINUX_ASM_DEVICE_H

struct dev_archdata {
	/* liblinux private data */
	void *liblinux_private;
	struct inode *inode;
};

struct pdev_archdata {
};

#endif /* __LIBLINUX_ASM_DEVICE_H */
