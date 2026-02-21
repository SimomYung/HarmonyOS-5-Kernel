/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Desciption: Define shmlock in udktest module
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 29 10:37:35 2019
 */
#include <udk/mm.h>
#include <udk/api.h>
#include <udk/log.h>
#include <udk/sync.h>
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/errno.h>
#include <udk/device.h>
#include <udk/securec.h>
#include <libhmucap/ucap.h>
#include <libmem/flags.h>
#include <libstrict/strict.h>

#include <udktests.h>

#include <helper.h>

#define SHMLOCK_TEST_SUBCMD_PREPARE	0x1
#define SHMLOCK_TEST_SUBCMD_LOCK	0x2
#define SHMLOCK_TEST_SUBCMD_FINISH	0x3

struct shmlock_test_arg {
	int subcmd;
	uint32_t owner_cnode_idx;
	uint64_t shm_id;
};

static void *shmlock_test_lock;
static int shmlock_shm_id;

static int shmlock_init_shm(unsigned long out)
{
#define SHMLOCK_TEST_SHM_SIZE	0x1000UL
	uint64_t key;
	int err = err;
	uint32_t owner_cnode_idx;
	struct udk_shm_info shm_info;
	struct shmlock_test_arg output;

	mem_zero_s(shm_info);
	mem_zero_s(output);

	/* alloc shm anon */
	err = udk_alloc_shm_anon(SHMLOCK_TEST_SHM_SIZE,
				 UDK_SHM_ALLOC_POPULATE, &shm_info);
	if (err < 0) {
		udk_error("alloc shm anon failed\n");
		return err;
	}
	shmlock_test_lock = shm_info.vaddr;
	shmlock_shm_id = shm_info.id;

	err = udk_grant_shm_to_caller(shm_info.id, SHM_GRANT_RDWR, &key);
	if (err < 0) {
		udk_error("grant shmid to caller failed\n");
		return err;
	}
	output.shm_id = key;
	owner_cnode_idx = hm_ucap_self_cnode_idx();
	if (is_cnode_idx_err(owner_cnode_idx)) {
		err = cnode_idx_to_err(owner_cnode_idx);
		hm_error("hm_ucap_self_cnode_idx fails: %s\n",
			 hmstrerror(err));
		udk_free_shm_anon(&shm_info);
		return err;
	}
	output.owner_cnode_idx = owner_cnode_idx;

	err = udk_copy_to_user((void *)out, &output, sizeof(output));
	if (err < 0) {
		udk_error("udk_copy_to_user shm_id output failed\n");
		return err;
	}

	/* init the shm as pthread_lock */
	err = udk_shared_mutex_init(shmlock_test_lock);
	if (err < 0) {
		udk_error("init shared lock failed\n");
		return err;
	}

	return err;
}

static int shmlock_lock_shm(void)
{
	int err;

	err = udk_shared_mutex_lock(shmlock_test_lock);
	if (err < 0) {
		udk_error("udk lock shmlock_test_lock failed\n");
		return err;
	}

	return 0;
}

static int shmlock_free_resources(void)
{
	int err;
	struct udk_shm_info shm_info = {
		.id = shmlock_shm_id,
		.len = SHMLOCK_TEST_SHM_SIZE,
		.vaddr = shmlock_test_lock,
	};

	/* don't care if unlock is failed or not */
	(void)udk_shared_mutex_unlock(shmlock_test_lock);

	err = udk_free_shm_anon(&shm_info);
	if (err < 0) {
		udk_error("udk free shm anon failed\n");
		return err;
	}

	return 0;
}

int udktest__shmlock(void *ctx, int cmd, unsigned long _arg)
{
	int err = 0;
	struct shmlock_test_arg arg;

	UNUSED(ctx, cmd);

	mem_zero_s(arg);

	err = udk_copy_from_user(&arg, (void *)_arg, sizeof(arg));
	if (err != 0) {
		udk_error("udk_copy_from_user shmlock arg failed\n");
		return -EINVAL;
	}

	switch (arg.subcmd) {
	case SHMLOCK_TEST_SUBCMD_PREPARE:
		/* init use arg as output arg as well */
		err = shmlock_init_shm(_arg);
		break;
	case SHMLOCK_TEST_SUBCMD_LOCK:
		err = shmlock_lock_shm();
		break;
	case SHMLOCK_TEST_SUBCMD_FINISH:
		err = shmlock_free_resources();
		break;
	default:
		udk_error("unknown shmlock subcmd\n");
		err = -EINVAL;
		break;
	}

	return err;
}
