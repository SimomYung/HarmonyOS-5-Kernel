// SPDX-License-Identifier: GPL-2.0
#include <linux/compiler.h>
#include <linux/err.h>
#include <linux/fs.h>

ssize_t vfs_read_full(struct file *filep, char __user *buf, size_t size, loff_t *pos)
{
	ssize_t ret = 0;
	ssize_t read_bytes = 0;

	do {
		ret = vfs_read(filep, buf + read_bytes, size - read_bytes, pos);
		if (ret == 0) {
			break;
		} else if (ret < 0) {
			return ret;
		} else {
			read_bytes += ret;
		}
	} while (read_bytes < size);

	if (read_bytes == size) {
		/* try read more from `pos` */
		ret = vfs_read(filep, buf, PAGE_SIZE, pos);
		if (ret > 0) {
			ret = -EFBIG;
		}
	}

	return (ret < 0) ? ret: read_bytes;
}

ssize_t vfs_write_full(struct file *filep, const char __user *buf, size_t size, loff_t *pos)
{
	ssize_t ret = 0;
	ssize_t write_bytes = 0;

	do {
		ret = vfs_write(filep, buf + write_bytes, size - write_bytes, pos);
		if (ret == 0) {
			break;
		} else if (ret == -ERESTARTNOINTR) {
			continue;
		} else if (ret < 0) {
			return ret;
		} else {
			write_bytes += ret;
		}
	} while (write_bytes < size);

	if (write_bytes != size) {
		ret = -EINVAL;
	}

	return (ret < 0) ? ret: write_bytes;
}

#include <linux/file.h>
int liblinux_pal_ksys_readlink(const char *pathname,
			      char *buf, unsigned int bufsiz)
{
	return liblinux_pal_ksys_readlinkat(AT_FDCWD, pathname, buf, bufsiz);
}

long __override do_readlinkat(int dfd, const char *pathname,
			      char *buf, int bufsiz)
{
	return (long)liblinux_pal_ksys_readlinkat(dfd, pathname, buf, bufsiz);
}
