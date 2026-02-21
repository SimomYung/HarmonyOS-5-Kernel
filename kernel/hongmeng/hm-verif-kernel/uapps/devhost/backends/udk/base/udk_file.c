/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: This is the implementation of udk_file related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 17 15:44:23 2020
 */
#include <udk/file.h>

#include <unistd.h>
#include <udk/mm.h>
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/types.h>
#include <udk/iolib.h>
#include <sys/types.h>
#include <libstrict/strict.h>

int udk_file_init(struct udk_file *file, unsigned char *buf, unsigned long size, unsigned long pos)
{
	if (file == NULL) {
		return -EINVAL;
	}
	file->buf = buf;
	file->size = size;
	file->pos = pos;
	return 0;
}

int udk_file_read(struct udk_file *file, void *buf, unsigned long cnt, unsigned long long off)
{
	unsigned long count = cnt;
	if (buf == NULL || file == NULL || file->buf == NULL) {
		return -EINVAL;
	}

	if (count == 0 || off >= file->size) {
		return 0;
	}

	if (count > file->size - off) {
		count = file->size - (unsigned long)off;
	}

	if (udk_copy_to_user(buf, file->buf + off, count) != 0) {
		return -EFAULT;
	}

	file->pos = (unsigned long)off + count;
	return (int)count;
}

int udk_file_write(struct udk_file *file, const void *buf, unsigned long cnt, unsigned long long off)
{
	unsigned long count = cnt;
	if (buf == NULL || file == NULL || file->buf == NULL) {
		return -EINVAL;
	}

	if (off >= file->size) {
		return -ENOSPC;
	}

	if (count == 0) {
		return 0;
	}

	if (count > file->size - off) {
		count = file->size - (unsigned long)off;
	}

	if (udk_copy_from_user(file->buf + off, buf, count) != 0) {
		return -EFAULT;
	}

	file->pos = (unsigned long)off + count;
	return (int)count;
}

long long udk_file_generic_llseek(struct udk_file *file, long long off, int whence)
{
	long long offset = off;
	if (file == NULL) {
		return -EINVAL;
	}

	switch (whence) {
	case SEEK_SET:
		break;
	case SEEK_CUR:
		offset += (long long)(unsigned long long)file->pos;
		break;
	case SEEK_END:
		offset += (long long)(unsigned long long)file->size;
		break;
	default:
		return -EINVAL;
	}

	if (offset < 0 || offset > (long long)(unsigned long long)file->size) {
		return -EINVAL;
	}
	file->pos = (unsigned long)(unsigned long long)offset;
	return offset;
}

long long udk_file_noop_llseek(const struct udk_file *file, long long off, int whence)
{
	UNUSED(whence);
	long long offset = off;
	if (file == NULL) {
		return -EINVAL;
	}
	offset = (long long)(unsigned long long)file->pos;
	return offset;
}
