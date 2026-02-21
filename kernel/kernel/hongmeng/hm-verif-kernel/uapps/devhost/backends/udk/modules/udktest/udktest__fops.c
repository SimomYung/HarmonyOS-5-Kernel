/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 10:37:35 2019
 */

#include "udktest__fops.h"
#include <udk/device.h>
#include <udk/iolib.h>
#include <libstrict/strict.h>
#include <udk/panic.h>
#include <udk/errno.h>
#include <udk/log.h>
#include <udk/securec.h>
#include <udk/mm.h>
#include <udk/file.h>

#include <helper.h>
#include <udktests.h>

const unsigned int TEST_BUF_SIZE = 1024;
const unsigned char MAGIC_BYTE1   = 0x79;
const unsigned char MAGIC_BYTE2   = 0xdd;
const unsigned char MAGIC_BYTE3   = '$';
const unsigned int MAGIC_RETURN = 0xdecafbad;

static pid_t g_getpid = -1;
extern int devhost_current_pid(void);

#define UDK_CHECK_DHPID() do { \
	pid_t dhpid; \
	dhpid = devhost_current_pid(); \
	if (g_getpid != dhpid) { \
		udk_error("get pid fail, getpid =%d, dhpid =%d\n", \
			  (int)g_getpid, (int)dhpid); \
		return -1; \
	} \
} while (0)

int udktest__fops_open(const struct device_info *devinfo, void **ctx)
{
	struct udk_file *file = NULL;
	unsigned char *buf = NULL;

	UNUSED(devinfo);
	buf = (unsigned char *)udk_malloc(TEST_BUF_SIZE);
	if (buf == NULL) {
		return -ENOMEM;
	}
	for (unsigned int i = 0; i < TEST_BUF_SIZE; i += 2) {
		buf[i] = MAGIC_BYTE1;
		buf[i + 1] = MAGIC_BYTE2;
	}

	file = (struct udk_file *)udk_malloc(sizeof(struct udk_file));
	if (file == NULL) {
		free(buf);
		return -ENOMEM;
	}
	(void)udk_file_init(file, buf, TEST_BUF_SIZE, 0);
	*ctx = (void *)file;
	return 0;
}

int udktest__fops_release(void *ctx)
{
	struct udk_file *file = (struct udk_file *)ctx;
	udk_free(file->buf);
	udk_free(file);
	return 0;
}

int udktest__fops_read(void *ctx, void *buf, unsigned long count,
		       unsigned long long off)
{
	UDK_CHECK_DHPID();

	struct udk_file *file = (struct udk_file *)ctx;
	return udk_file_read(file, buf, count, off);
}

int udktest__fops_write(void *ctx, const void *buf, unsigned long count,
			unsigned long long off)
{
	struct udk_file *file = (struct udk_file *)ctx;
	int ret;

	UDK_CHECK_DHPID();

	ret = udk_file_write(file, buf, count, off);
	if (ret < 0) {
		return ret;
	}

	for (unsigned int i = 0; i < count; i++) {
		if (file->buf[i] != MAGIC_BYTE1) {
			return (int)i;
		}
	}

	return (int)count;
}

long long udktest__fops_llseek(void *ctx, long long offset, int whence)
{
	UDK_CHECK_DHPID();

	struct udk_file *file = (struct udk_file *)ctx;
	return udk_file_generic_llseek(file, offset, whence);
}

int udktest__fops_ioctl(const void *ctx, unsigned int cmd, unsigned long arg)
{
	struct test_struct0 {
		unsigned long long str_addr;
		unsigned long long len;
		unsigned long long pos;
		unsigned int ret;
	} s0;
	struct {
		short  s;
		unsigned long long p;
		char   c;
	} s1;
	char *buf = NULL;
	unsigned int ioctl_ret = MAGIC_RETURN;
	int ret;

	UNUSED(ctx, cmd);
	UDK_CHECK_DHPID();

	mem_zero_s(s0);
	mem_zero_s(s1);

	ret = udk_copy_from_user(&s1, (void *)arg, sizeof(s1));
	if (ret != 0) {
		udk_error("udk_copy_from_user s1 failed\n");
		return -EINVAL;
	}

	if (s1.s != (short)0xdead || (void *)(uintptr_t)s1.p == NULL || s1.c != '\t') {
		udk_error("udk_copy_from_user s1 invalid\n");
		return -EINVAL;
	}

	ret = udk_copy_from_user(&s0, (void *)(uintptr_t)s1.p, sizeof(s0));
	if (ret != 0) {
		udk_error("udk_copy_from_user s0 failed\n");
		return -EINVAL;
	}

	if (s0.len == 0 || s0.pos == 0 || s0.len <= s0.pos) {
		return -EINVAL;
	}

	buf = (char *)udk_malloc((size_t)s0.len);
	if (buf == NULL) {
		return -ENOMEM;
	}

	ret = udk_copy_from_user(buf, (void *)(uintptr_t)s0.str_addr, (unsigned long)s0.len);
	if (ret != 0) {
		udk_error("udk_copy_from_user string failed\n");
		udk_free(buf);
		buf = NULL;
		return ret;
	}

	if (buf[s0.pos] != MAGIC_BYTE3) {
		udk_error("Invalid string\n");
		udk_free(buf);
		buf = NULL;
		return -EINVAL;
	}

	ret = udk_copy_to_user(&((struct test_struct0 *)(uintptr_t)s1.p)->ret,
			       &ioctl_ret, sizeof(int));
	if (ret != 0) {
		udk_error("udk_copy_to_user failed\n");
	}

	udk_free(buf);
	buf = NULL;
	return ret;
}

int udktest_setpid(const void *ctx, unsigned int cmd, unsigned long arg)
{
	struct test_struct {
		pid_t pid;
	} s0;
	int ret;

	UNUSED(ctx, cmd);
	mem_zero_s(s0);

	ret = udk_copy_from_user(&s0, (void *)arg, sizeof(s0));
	if (ret != 0) {
		udk_error("udk_copy_from_user s0 failed\n");
		return -EINVAL;
	}

	g_getpid = s0.pid;
	return 0;
}

int udktest__fops_mmap(const void *ctx, unsigned long addr, unsigned long length,
		       unsigned long prot, unsigned long offset)
{
	UNUSED(ctx, addr, length, prot, offset);
	UDK_CHECK_DHPID();
	return 0;
}
