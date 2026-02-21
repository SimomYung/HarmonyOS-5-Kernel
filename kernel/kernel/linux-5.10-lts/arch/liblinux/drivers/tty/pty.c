// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <linux/tty.h>
#include <linux/string.h>
#include <linux/devpts_fs.h>
#include <linux/major.h>
#include <linux/file.h>

struct device *devpts_find_device(int index);
devfs_open_handler_t devfs_open_handler = NULL;

void liblinux_register_devfs_open_handler(devfs_open_handler_t fn)
{
	devfs_open_handler = fn;
}
EXPORT_SYMBOL(liblinux_register_devfs_open_handler);

int __override ptm_open_peer(struct file *master, struct tty_struct *tty, int flags)
{
	int fd;
	int ret;
	struct device *pts_dev;
	struct file *filp = NULL;

	if (tty->driver->major != UNIX98_PTY_MASTER_MAJOR)
		return -EIO;

	if (!devfs_open_handler)
		return -EOPNOTSUPP;

	fd = get_unused_fd_flags(flags);
	if (fd < 0) {
		ret = fd;
		goto err;
	}

	pts_dev = devpts_find_device(tty->index);
	if (pts_dev == NULL) {
		ret = -ENODEV;
		goto err_put;
	}

	filp = devfs_open_handler(pts_dev, flags);
	if (IS_ERR(filp)) {
		ret = PTR_ERR(filp);
		goto err_put;
	}

	fd_install(fd, filp);
	return fd;

err_put:
	put_unused_fd(fd);
err:
	return ret;
}
