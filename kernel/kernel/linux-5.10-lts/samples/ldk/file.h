/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_FILE_H__
#define __LIBDH_LINUX_FILE_H__

void put_filp(struct file *file);
void fput_ex(struct file *file);
ssize_t vfs_read_full(struct file *filep, char __user *buf, size_t size, loff_t *pos);
ssize_t vfs_write_full(struct file *filep, const char __user *buf, size_t size, loff_t *pos);

#endif /* __LIBDH_LINUX_FILE_H__ */
