/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: This is the header of udk_file interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 17 16:02:29 2020
 */
#ifndef __UDK_FILE_H__
#define __UDK_FILE_H__

struct udk_file {
	unsigned char *buf;
	unsigned long size;
	unsigned long pos;
};

int udk_file_init(struct udk_file *file, unsigned char *buf, unsigned long size, unsigned long pos);
int udk_file_read(struct udk_file *file, void *buf, unsigned long count, unsigned long long off);
int udk_file_write(struct udk_file *file, const void *buf, unsigned long count, unsigned long long off);
long long udk_file_generic_llseek(struct udk_file *file, long long offset, int whence);
long long udk_file_noop_llseek(const struct udk_file *file, long long offset, int whence);

#endif /* __UDK_FILE_H__ */
