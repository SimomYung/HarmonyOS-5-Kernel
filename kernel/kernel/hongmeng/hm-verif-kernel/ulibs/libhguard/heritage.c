/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Internal share memory API for init and hguard data communication
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 4 18:48:37 2022
 */

#include <hongmeng/compiler.h>
#include <libhguard/heritage.h>
#include <libhmsrv_sys/hm_mem.h>

#define INIT_SHM_MAX_BYTES (PAGE_SIZE << 4U)
#define INIT_SHM_NAME "init-shm"

struct heritage_arg_s {
	uint32_t shm_size;
	uint64_t shm_flag;
	mode_t shm_mode;

	uint32_t mmap_prot;
	uint32_t mmap_flag;
};

struct heritage_ret_s {
	uint32_t shm_size;
	int32_t shm_id;
	void *shm_addr;
};

static void close_shm(uint32_t size, int32_t id, void *addr)
{
	int err;
	if (addr != NULL) {
		err = hm_mem_munmap(addr, size);
		if (err != E_HM_OK) {
			hm_error("failed to unmap shm, err=%s\n", hmstrerror(err));
		}
	}
	if (id >= 0) {
		err = hm_mem_shm_close(id);
		if (err != E_HM_OK) {
			hm_error("failed to close shm, err=%s\n", hmstrerror(err));
		}
	}
}

static void unlink_shm(void)
{
	int err = hm_mem_shm_unlink(INIT_SHM_NAME);
	if (err != E_HM_OK) {
		hm_error("failed to unlink shm name=%s, err=%s\n", INIT_SHM_NAME, hmstrerror(err));
	}
}

void init_heritage_sender_close(uint32_t size, int32_t id, void *addr)
{
	close_shm(size, id, addr);
}

void init_heritage_receiver_close(uint32_t size, int32_t id, void *addr)
{
	close_shm(size, id, addr);
	unlink_shm();
}

static int open_shm(const struct heritage_arg_s *arg, struct heritage_ret_s *ret)
{
	int err = E_HM_OK;
	int shm_id;
	size_t shm_size = 0U;
	void *shm_addr = MAP_FAILED;

	shm_id = hm_mem_shm_open_with_mode(INIT_SHM_NAME, arg->shm_size,
					   arg->shm_flag, arg->shm_mode, &shm_size);
	if (unlikely(shm_id < 0)) {
		hm_error("failed to open shm, err=%s\n", hmstrerror(shm_id));
		err = E_HM_RESFAULT;
	}

	if (err == E_HM_OK) {
		shm_addr = hm_mem_mmap_posix(NULL, shm_size, arg->mmap_prot,
					     arg->mmap_flag, shm_id, 0, &err);
		if (unlikely(shm_addr == MAP_FAILED)) {
			hm_error("failed to mmap shm, err=%s\n", hmstrerror(err));
			shm_addr = NULL;
			err = E_HM_RESFAULT;
		}
	}

	/* assign even if failed
	 * so that caller can recycle resource */
	ret->shm_size = (uint32_t)shm_size;
	ret->shm_id = shm_id;
	ret->shm_addr = shm_addr;

	return err;
}

void init_heritage_sender_open(uint32_t size, int32_t *out_id, void **out_addr)
{
	int err = E_HM_OK;
	struct heritage_ret_s ret = { 0 };
	struct heritage_arg_s arg = {
		.shm_size = size,
		.shm_flag = SHM_O_CREAT |
			    SHM_O_EXCL |   /* fail if alreay opened */
			    SHM_O_KEEP,    /* don't close after process exit */
		.shm_mode = S_IRUSR,
		.mmap_prot = MPROT_WRITE,
		.mmap_flag = MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED
	};

	if (out_id == NULL || out_addr == NULL) {
		err = E_HM_INVAL;
	}
	if (size >= INIT_SHM_MAX_BYTES) {
		err = E_HM_TOOMANY;
		hm_error("size=%u, larger than max=%u\n",
			 size, (unsigned int)INIT_SHM_MAX_BYTES);
	}
	if (err == E_HM_OK) {
		err = open_shm(&arg, &ret);
	}
	if (err == E_HM_OK) {
		*out_id = ret.shm_id;
		*out_addr = ret.shm_addr;
	} else {
		close_shm(ret.shm_size, ret.shm_id, ret.shm_addr);
		unlink_shm();
	}
}

void init_heritage_receiver_open(uint32_t *out_size, int32_t *out_id, void **out_addr)
{
	int err = E_HM_OK;
	struct heritage_ret_s ret = { 0 };
	struct heritage_arg_s arg = {
		.shm_size = 0,         /* reader doesn't know size */
		.shm_flag = SHM_O_RDONLY,
		.shm_mode = S_IRUSR,
		.mmap_prot = MPROT_READ,
		.mmap_flag = MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED
	};

	if (out_size == NULL || out_id == NULL || out_addr == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = open_shm(&arg, &ret);
	}
	if (err == E_HM_OK) {
		*out_size = ret.shm_size;
		*out_id = ret.shm_id;
		*out_addr = ret.shm_addr;
	} else {
		close_shm(ret.shm_size, ret.shm_id, ret.shm_addr);
	}
}
