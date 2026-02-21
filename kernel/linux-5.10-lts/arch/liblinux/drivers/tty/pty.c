// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <linux/tty.h>
#include <linux/string.h>
#include <linux/devpts_fs.h>
#include <linux/major.h>
#include <linux/file.h>

struct file *alloc_file(const struct path *path, int flags,
		        const struct file_operations *fop);
struct device *devpts_find_device(int index);

int __override ptm_open_peer(struct file *master, struct tty_struct *tty, int flags)
{
	int fd = -1;
	struct file *filp;
	int retval = -EINVAL;
	struct device *pts_dev;
	struct inode *inode;
	struct dentry dentry;
	char name[] = "pts";
	struct path path = {
		.mnt = NULL,
		.dentry = &dentry,
	};

	if (tty->driver->major != UNIX98_PTY_MASTER_MAJOR)
		return -EIO;

	fd = get_unused_fd_flags(flags);
	if (fd < 0) {
		retval = fd;
		goto err;
	}

	pts_dev = devpts_find_device(tty->index);
	if (pts_dev == NULL) {
		retval = -ENODEV;
		goto err_put;
	}

	inode = pts_dev->archdata.inode;
	if (inode == NULL) {
		retval = -ENODEV;
		goto err_put;
	}

	dentry.d_inode = inode;
	dentry.d_name.name = name;
	dentry.d_name.len = strlen(name);
	filp = alloc_file(&path, flags | FMODE_NONOTIFY, inode->i_fop);
	if (IS_ERR(filp)) {
		retval = PTR_ERR(filp);
		goto err_put;
	}

	filp->f_mapping = &inode->i_data;
	retval = filp->f_op->open(inode, filp);
	if (retval < 0) {
		pr_err("ldk: %s: pts file open failed\n", __func__);
		put_filp(filp);
		goto err_put;
	}

	fd_install(fd, filp);
	filp->f_path.dentry = NULL;
	return fd;

err_put:
	put_unused_fd(fd);
err:
	return retval;
}
