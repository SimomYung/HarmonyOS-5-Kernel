/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_FILE_UTILS_H__
#define __LIBLINUX_FILE_UTILS_H__

int preload_firmware_file(const char *path, const char *filename);
int unload_firmware_file(const char *path, int free_rel);

#endif	/* __LIBLINUX_FILE_UTILS_H__ */
