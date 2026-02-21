// SPDX-License-Identifier: GPL-2.0
#include <vfs.h>

int liblinux_vfs_lock_in(unsigned int user_id)
{
	return vfs_lock_in(user_id);
}

int liblinux_vfs_unlock_in(unsigned int user_id, unsigned int file,
	unsigned char *iv, unsigned int iv_len)
{
	return vfs_unlock_in(user_id, file, iv, iv_len);
}
