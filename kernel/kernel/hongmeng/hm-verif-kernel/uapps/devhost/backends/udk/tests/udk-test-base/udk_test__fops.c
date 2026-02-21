/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 27 14:23:26 2019
 */

#include "udk_test__fops.h"
#include <stddef.h>
#include <unistd.h>
#include <sys/mman.h>
#include <libtest/test.h>
#include <libhwsecurec/securec.h>

#include <vfs.h>

#include <udk_tests.h>
#include <udk_test_helper.h>

const unsigned int TEST_BUF_SIZE = 1024;
const unsigned int MAGIC_POS     = 123;
const unsigned char MAGIC_BYTE1   = 0x79;
const unsigned char MAGIC_BYTE2   = 0xdd;
const unsigned char MAGIC_BYTE3   = '$';
const unsigned int MAGIC_RETURN = 0xdecafbad;

static int chrdev_setup(void)
{
	int fd;

	fd = open("/dev/udktest", O_RDWR);
	if (fd < 0) {
		hm_error("open /dev/udktest failed\n");
	}

	return fd;
}

static int read_and_verify(int fd, unsigned char *buf, unsigned int size, unsigned int count, off_t offset, int whence)
{
	const char tmp[2] = {MAGIC_BYTE1, MAGIC_BYTE2};
	off_t off;
	int ret;

	if (count > size) {
		return -1;
	}

	off = lseek(fd, offset, whence);
	if (off < 0) {
		return -1;
	}

	ret = read(fd, buf, count);
	if (ret < 0) {
		return -1;
	}

	/* Now ret is non-negative, safe to convert */
	if ((unsigned int)ret != count) {
		return -1;
	}

	for (unsigned int i = 0; i < count; i++) {
		if (buf[i] != tmp[(i + off) % 2]) {
			return -1;
		}
	}
	return 0;
}

static int udk_test__fops_read(void)
{
	int ret;
	int fd;
	unsigned char buf[TEST_BUF_SIZE];

	fd = chrdev_setup();
	if (fd < 0) {
		return -1;
	}

	ret = read_and_verify(fd, buf, TEST_BUF_SIZE, TEST_BUF_SIZE,
			      0, SEEK_SET);
	if (ret < 0) {
		close(fd);
		return -1;
	}

	ret = read_and_verify(fd, buf, TEST_BUF_SIZE, TEST_BUF_SIZE / 2,
			      TEST_BUF_SIZE / 2, SEEK_SET);
	if (ret < 0) {
		close(fd);
		return -1;
	}

	ret = read_and_verify(fd, buf, TEST_BUF_SIZE, TEST_BUF_SIZE / 4,
			      -((int)TEST_BUF_SIZE / 2), SEEK_END);
	if (ret < 0) {
		close(fd);
		return -1;
	}

	ret = read_and_verify(fd, buf, TEST_BUF_SIZE, TEST_BUF_SIZE / 8 + 1,
			      TEST_BUF_SIZE / 8 - 1, SEEK_CUR);
	if (ret < 0) {
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

static int udk_test__fops_write(void)
{
	int ret;
	int fd;
	unsigned char buf[TEST_BUF_SIZE];

	fd = chrdev_setup();
	if (fd < 0) {
		return -1;
	}

	ret = memset_s(buf, TEST_BUF_SIZE, MAGIC_BYTE1, TEST_BUF_SIZE);
	if (ret != 0) {
		return -1;
	}

	ret = write(fd, buf, TEST_BUF_SIZE);
	if (ret < 0) {
		printf("Invalid write\n");
		close(fd);
		return -1;
	}

	/* Now ret is non-negative, safe to convert */
	if ((unsigned int)ret != TEST_BUF_SIZE) {
		printf("Incomplete write\n");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

static int udk_test__fops_ioctl(void)
{
	int ret;
	int fd;
	char *str = NULL;
	struct {
		unsigned long long str_addr;
		unsigned long long len;
		unsigned long long pos;
		unsigned int ret;
	} s0 = {
		.str_addr = 0,
		.len = 0,
		.pos = 0,
	};
	struct {
		short  s;
		unsigned long long p;
		char   c;
	} s1 = {
		.s = (short)0xdead,
		.p = (unsigned long long)(uintptr_t)&s0,
		.c = '\t',
	};

	str = (char *)malloc(TEST_BUF_SIZE);
	if (str == NULL) {
		return -1;
	}

	/* Zero the buffer */
	mem_zero_b(str, TEST_BUF_SIZE);

	/* Set magic char */
	str[MAGIC_POS] = MAGIC_BYTE3;
	s0.len = (unsigned long long)TEST_BUF_SIZE;
	s0.pos = (unsigned long long)MAGIC_POS;

	fd = open("/dev/udktest", O_RDWR);
	if (fd < 0) {
		hm_error("open /dev/udktest failed\n");
		free(str);
		str = NULL;
		return -1;
	}

	/* Set s0 str_addr */
	s0.str_addr = (unsigned long long)(uintptr_t)str;

	ret = ioctl(fd, UDKTEST_IOC_IOCTL, &s1);
	if (ret == -1) {
		hm_error("ioctl failed\n");
		ret = -1;
	} else if (ret != 0) {
		hm_error("ioctl abnormal return\n");
		ret = -1;
	} else if (s0.ret != MAGIC_RETURN) {
		hm_error("ioctl wrong return\n");
		ret = -1;
	}

	hm_info("Correct return value 0x%08x\n", s0.ret);

	free(str);
	str = NULL;
	close(fd);
	return ret;
}

static int udk_test__fops_mmap(void)
{
	int ret = 0;
	void *ret_vaddr = NULL;
	size_t len = 0x100;
	int fd = 0;

	fd = open("/dev/udktest", O_RDWR);
	if (fd < 0) {
		hm_error("open /dev/udktest failed\n");
		return -1;
	}

	ret_vaddr = mmap(0, len, PROT_READ, MAP_SHARED, fd, 0);
	if (ret_vaddr == MAP_FAILED) {
		hm_error("mmap failed\n");
		ret = -1;
	}

	close(fd);
	return ret;
}

static int udk_test__setpid(void)
{
	int ret;
	int fd;

	struct {
		pid_t pid;
	} s0 = {
		.pid = getpid(),
	};

	fd = open("/dev/udktest", O_RDWR);
	if (fd < 0) {
		hm_error("open /dev/udktest failed\n");
		return -1;
	}

	ret = ioctl(fd, UDKTEST_IOC_SETDHPID, &s0);
	if (ret == -1) {
		hm_error("ioctl failed\n");
		ret = -1;
	} else if (ret != 0) {
		hm_error("ioctl abnormal return\n");
		ret = -1;
	}

	close(fd);
	return ret;
}

static struct test udk_fops_tests[] = {
	/* udk_test__setpid must be the first case
	 * for test get pid from devhost */
	TESTHM("udk_test__fops_read", udk_test__setpid)
	TESTHM("udk_test__fops_read", udk_test__fops_read)
	TESTHM("udk_test__fops_write", udk_test__fops_write)
	TESTHM("udk_test__fops_read", udk_test__fops_ioctl)
	TESTHM("udk_test__fops_mmap", udk_test__fops_mmap)
	{
		.desc = NULL,
		.func = NULL,
		.funcname = NULL,
	}
};

int udk_test__fops(void)
{
	struct test_result result;

	mem_zero_s(result);

	test_run_all(1, NULL, udk_fops_tests, &result);
	if (result.failed != 0) {
		return -1;
	}

	return 0;
}
