/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Desciption: Define test of udk shmlock
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 29 15:00:30 2020
 */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <hmext/mm.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <hongmeng/errno.h>

#include <lib/utils.h>
#include <libtest/test.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_mem.h>

#include "setup.h"
#include "udk_tests.h"

#define SHMLOCK_TEST_SUBCMD_PREPARE	0x1
#define SHMLOCK_TEST_SUBCMD_LOCK	0x2
#define SHMLOCK_TEST_SUBCMD_FINISH	0x3

struct shmlock_test_arg {
	int subcmd;
	uint32_t owner_cnode_idx;
	uint64_t shm_id;
};

static int udktest_fd;
static uint64_t udktest_shm_id;
static uint32_t udktest_owner_cnode_idx;
static pthread_mutex_t *udktest_lock;

static int shmlock_test__init(void)
{
	int fd = open("/dev/udktest", O_RDWR);
	if (fd < 0) {
		hm_error("open /dev/udktest failed\n");
		return fd;
	}

	udktest_fd = fd;

	return 0;
}

static int shmlock_test__fetchid(void)
{
	int ret = 0;
	struct shmlock_test_arg arg = {
		.subcmd = SHMLOCK_TEST_SUBCMD_PREPARE,
		.shm_id = 0,
	};

	ret = ioctl(udktest_fd, UDKTEST_IOC_SHMLOCK, &arg);
	if (ret < 0) {
		hm_error("fetch shmid from udktest failed\n");
		return ret;
	}

	/* output shm_id is saved in arg.shm_id */
	udktest_shm_id = arg.shm_id;
	udktest_owner_cnode_idx = arg.owner_cnode_idx;
	return 0;
}

static void *acquire_shm(uint64_t udktest_shm_id)
{
	int rc;
	size_t len;
	int shm_id;
	void *rvaddr = ulong_to_ptr(MAP_FAILED, void);

	shm_id = hm_ashm_open_auth(udktest_shm_id, udktest_owner_cnode_idx, 0, SHM_O_RDWR, &len);
	if (shm_id > 0) {
		rvaddr = hm_mem_mmap_posix(NULL, len, MPROT_READ | MPROT_WRITE,
					   MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED,
					   shm_id, 0, &rc);
		if (rc != E_HM_OK) {
			hm_error("map shm failed:%s\n", hmstrerror(rc));
		}
		(void)hm_ashm_close(shm_id);
	}
	return rvaddr;
}

static int shmlock_test__mapshm(void)
{
	void *vaddr = NULL;

	vaddr = acquire_shm(udktest_shm_id);
	if (vaddr == ulong_to_ptr(MAP_FAILED, void)) {
		return -1;
	}
	/* the shm is a pthread lock and inited by module */
	udktest_lock = (pthread_mutex_t *)vaddr;
	return 0;
}

static int shmlock_test__lock(void)
{
	int err;

	err = pthread_mutex_trylock(udktest_lock);
	if (err != 0) {
		hm_error("pthread lock failed\n");
		return err;
	}

	err = pthread_mutex_unlock(udktest_lock);
	if (err != 0) {
		hm_error("pthread unlock failed\n");
		return err;
	}

	return 0;
}

static int shmlock_test__trylock(void)
{
	int err;
	struct shmlock_test_arg arg = {
		.subcmd = SHMLOCK_TEST_SUBCMD_LOCK,
		.shm_id = 0,
	};

	/* let module lock it */
	err = ioctl(udktest_fd, UDKTEST_IOC_SHMLOCK, &arg);
	if (err != 0) {
		hm_error("module lock shmlock failed\n");
		return err;
	}

	/* try lock and should return EBUSY */
	err = pthread_mutex_trylock(udktest_lock);
	if (err != EBUSY) {
		return -1;
	}

	return 0;
}

static int shmlock_test__finish(void)
{
	int err = 0;
	int ret = 0;
	struct shmlock_test_arg arg = {
		.subcmd = SHMLOCK_TEST_SUBCMD_FINISH,
		.shm_id = udktest_shm_id,
	};

	/* release shared memory */
	err = hm_mem_munmap(udktest_lock, ALIGN_UP(sizeof(pthread_mutex_t), PAGE_SIZE));
	if (err < 0) {
		hm_error("release shm page failed:%s\n", strerror(-err));
		ret = err;
	}

	/* still need to release resources even release failed */
	err = ioctl(udktest_fd, UDKTEST_IOC_SHMLOCK, &arg);
	if (ret == 0) {
		/* record first err code */
		ret = err;
	}

	close(udktest_fd);

	udktest_fd = 0;
	udktest_shm_id = 0;
	udktest_lock = 0;

	return ret;
}

static struct test shmlock_tests[] = {
	TESTHM("shmlock_test__init", shmlock_test__init)
	TESTHM("shmlock_test__fetchid", shmlock_test__fetchid)
	TESTHM("shmlock_test__mapshm", shmlock_test__mapshm)
	TESTHM("shmlock_test__lock", shmlock_test__lock)
	TESTHM("shmlock_test__trylock", shmlock_test__trylock)
	TESTHM("shmlock_test__finish", shmlock_test__finish)
	{
		.desc = NULL,
		.func = NULL,
		.funcname = NULL,
	}
};

int udk_test__shmlock(void)
{
	int ret;
	struct test_result result;

	test_run_all(1, NULL, shmlock_tests, &result);

	if (result.failed == 0) {
		hm_info("\nFinish udk-test-shmlock, "
			"result: [total: %d, all test success].\n",
			result.total);
		ret = 0;
	} else {
		hm_error("\nFinish udk-test-shmlock, "
			 "result: [total: %d, failed: %d].\n",
			 result.total, result.failed);
		return -1;
	}

	return ret;
}
