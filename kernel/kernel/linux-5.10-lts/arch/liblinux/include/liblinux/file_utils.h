/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_FILE_UTILS_H__
#define __LIBLINUX_FILE_UTILS_H__

int preload_firmware_file(const char *path, const char *filename);
int unload_firmware_file(const char *path, int free_rel);

struct file;
struct file *liblinux_ext_pidfd_fget(int pid, unsigned int fd);
int liblinux_ext_alloc_unused_fd(unsigned flags, struct file *file);

#endif	/* __LIBLINUX_FILE_UTILS_H__ */
