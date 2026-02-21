/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Oct 20 17:49:32 2019
 */

#include <stdlib.h>
#include <unistd.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsync/thread.h>
#include <libhmucap/ucap.h>
#include <libvsyscall/hmvsyscall.h>
#include <libhmsrv_io/io.h>
#include <libhmsrv_io/mgmt.h>
#include <libhmsrv_sys/hm_path.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>

#include <vfs.h>

#define TRACE_IMPL

#include "udk_tests.h"
#include <udk_test_helper.h>

static int load_driver(void)
{
	int ret = 0;

	ret = hmsrv_io_mgmt_load_module(DM_MGMT_ROOT_TARGET,
					UDK_TEST_BASE_MODNAME, NULL);
	if (ret != E_HM_OK) {
		hm_error("load module \"%s\" to devid %d failed: %s\n",
			 UDK_TEST_BASE_MODNAME, DEFAULT_DEVID, hmstrerror(ret));
		return ret;
	}

	return 0;
}

int udk_test_device_ioctl(int cmd)
{
	int ret;
	int fd = -1;
	struct ioctl_test_arg arg = { .a = 0 };

	fd = open("/dev/udktest", O_RDWR);
	if (fd < 0) {
		hm_error("open /dev/udktest failed\n");
		/* TODO: we are supposed to rmmod in the end of test */
		return fd;
	}

	ret = ioctl(fd, cmd, &arg, sizeof(arg));
	if (ret == -1) {
		hm_error("ioctl failed\n");
		return ret;
	}

	close(fd);

	return 0;
}

static int unload_driver(void)
{
	int ret = 0;

	ret = hmsrv_io_mgmt_unload_module(DM_MGMT_ROOT_TARGET, UDK_TEST_BASE_MODNAME);
	if (ret != E_HM_OK) {
		hm_error("unload module \"%s\" from root devhost failed: %s\n",
			 UDK_TEST_BASE_MODNAME, hmstrerror(ret));
		return ret;
	}

	return 0;
}

int udk_test_setup(void)
{
	int ret = 0;

	ret = hmsrv_io_init();
	if (ret < 0) {
		hm_error("udk_test_setup: Failed to prepare insmod\n");
		return ret;
	}

	ret = load_driver();
	if (ret < 0) {
		hm_error("udk_test_setup: Failed to insmod\n");
		return ret;
	}

	return 0;
}

int udk_test_teardown(void)
{
	int ret = 0;

	ret = unload_driver();
	if (ret < 0) {
		hm_error("udk_test_teardown: Failed to rmmod\n");
		return ret;
	}

	return 0;
}

static int test_udk_spawn(const char *path, const char *argv[], const char *envp[])
{
	int err = 0;
	pid_t pid;
	int wstatus;

	err = hm_spawn(&pid, path, NULL, NULL, argv, envp);
	if (err) {
		hm_error("test_udk_spawn failed: %s\n", hmstrerror(err));
		return -1;
	}
	err = hm_waitpid(pid, 0, &wstatus);
	if (err < 0) {
		hm_error("wait udk child process failed: %s\n", hmstrerror(err));
		return -1;
	}
	if (wstatus != 0) {
		hm_error("udk child process exit with failed.\n");
		return -1;
	}

	return 0;
}

int test_udk__net(void)
{
	const char *path = "/usr/bin/udk-test-net.elf";
	const char *argv[] = {
		"/usr/bin/udk-test-net.elf",
		NULL,
	};
	const char *envp[] = {
		"PATH=/:/bin:/usr/bin",
		NULL,
	};

	return test_udk_spawn(path, argv, envp);
}

int udk_test__block(void)
{
	const char *path = "/usr/bin/udk-test-blk.elf";
	const char *argv[] = {
		"/usr/bin/udk-test-blk.elf",
		NULL,
	};
	const char *envp[] = {
		"PATH=/:/bin:/usr/bin",
		NULL,
	};

	return test_udk_spawn(path, argv, envp);
}

int udk_test__unload(void)
{
	const char *path = "/usr/bin/udk-test-unload.elf";
	const char *argv[] = {
		"/usr/bin/udk-test-unload.elf",
		NULL,
	};
	const char *envp[] = {
		"PATH=/:/bin:/usr/bin",
		NULL,
	};

	return test_udk_spawn(path, argv, envp);
}
