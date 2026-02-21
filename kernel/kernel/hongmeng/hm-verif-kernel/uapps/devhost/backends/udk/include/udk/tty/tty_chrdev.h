/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define structures and prototypes of char-device tty
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 19:14:01 2019
 */

#ifndef TTY_UDK_CHRDEV_H
#define TTY_UDK_CHRDEV_H

#include <udk/char.h>
#include <udk/device.h>

#define CHRDEV_MAX_NAME_LEN 16

struct tty_chrdrv {
	unsigned int	major;
	unsigned int	minor_start;
	const struct udk_char_operations *fops; /* pointer to the file operations */
	const char	*dev_base_name;
};

struct tty_chrdev {
	devnum_t		dev_no;
	struct udk_device	*dev;
	char			name[CHRDEV_MAX_NAME_LEN]; /* the tty device name, e.g tty1 */
	struct tty_chrdrv 	*drv;
};

int tty_chrdrv_init(struct tty_chrdrv *drv, unsigned int max_dev_num,
		    const char *dev_name);

void tty_chrdrv_exit(const struct tty_chrdrv *drv, unsigned int max_dev_num);

int tty_chrdev_create(struct tty_chrdev *chrdev, struct tty_chrdrv *chrdrv,
		      unsigned int max_dev_num);

void tty_chrdev_destroy(struct tty_chrdev *chrdev);

#endif
