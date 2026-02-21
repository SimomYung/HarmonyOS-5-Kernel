/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Bunch ipc model based on share memory
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 15 21:21:31 2019
 */

#include <libmem/utils.h>
#include <libstrict/strict.h>
#include <libbunch/bunch_ipc.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhwsecurec/securec.h>

#define BUNCH_IPC_PAYLOAD_EXPAND_FACTOR		2U
/* bunch shm max is 64M, default 4K * 2^16 */
#define BUNCH_IPC_FETCH_RETRY_MAX		16U

static int bunch_ipc_map_populate(int shm_id, size_t data_size)
{
	int err = E_HM_OK;
	void *addr = NULL;

	addr = hm_mem_mmap_posix(NULL, data_size, MPROT_READ | MPROT_WRITE,
			  MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED | MFLAG_MAP_POPULATE,
			  shm_id, 0, &err);
	if (addr != MAP_FAILED) {
		(void)hm_mem_munmap(addr, data_size);
	} else {
		hm_debug("bunch ipc mmap failed, err:%s\n", hmstrerror(err));
	}

	return err;
}

int bunch_ipc_inner_alloc(struct bunch_ipc_attr *attr, size_t data_size)
{
	int err = E_HM_OK;
	int shm_id = -1;

	if (attr == NULL || data_size == 0) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		shm_id = hm_ashm_open_auth(0, 0U, data_size, SHM_O_CREAT | SHM_O_EXCL |
				      SHM_O_RDWR, NULL);
		if (shm_id < 0) {
			err = shm_id;
			hm_debug("bunch_ipc create ashm with size:%zu failed:%s\n",
				 data_size, hmstrerror(err));
		}
	}

	if (err == E_HM_OK) {
		err = bunch_ipc_map_populate(shm_id, data_size);
		if (err == E_HM_OK) {
			attr->shm_id = shm_id;
			attr->size = (uint32_t)data_size;
		} else {
			(void)hm_ashm_close(shm_id);
			(void)hm_ashm_unlink(shm_id);
		}
	}

	return err;
}

int bunch_ipc_inner_free(struct bunch_ipc_attr *attr)
{
	int err = E_HM_OK;

	if (attr == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		(void)hm_ashm_close(attr->shm_id);
		(void)hm_ashm_unlink(attr->shm_id);
		attr->shm_id = -1;
	}

	return err;
}

static size_t bunch_ipc_resize(size_t old_size)
{
	size_t new_size = old_size * BUNCH_IPC_PAYLOAD_EXPAND_FACTOR;

	if (new_size / BUNCH_IPC_PAYLOAD_EXPAND_FACTOR != old_size) {
		/* if overflow, set size to default size */
		new_size = BUNCH_IPC_DEFAULT_SIZE;
	}

	if (new_size < BUNCH_IPC_DEFAULT_SIZE) {
		new_size = BUNCH_IPC_DEFAULT_SIZE;
	}

	return new_size;
}

static int __ipc_ashm_grant(struct bunch_ipc_attr *attr, uint64_t target_info)
{
	int err;
	xref_t xref;

	if (IS_REF_VALID(target_info)) {
		/* target_info is xref */
		xref.rref = target_info;
		err = hm_ashm_grant(attr->shm_id, SHM_GRANT_RDWR,
				    xref, &attr->key);
	} else {
		/* target_info is cnode_idx */
		err = hm_ashm_grant_by_cnode_idx(attr->shm_id, SHM_GRANT_RDWR,
						 (uint32_t)target_info, &attr->key);
	}

	if (err < 0) {
		hm_debug("bunch_ipc grant shm_id:%d to target:%#"PRIx64" failed:%s\n",
			 attr->shm_id, target_info, hmstrerror(err));
	}

	return err;
}

static int __do_ipc_fetch(struct bunch_ipc_attr *attr, struct bunch_ipc_reader_ctx *ctx)
{
	int err = E_HM_OK;

	err = __ipc_ashm_grant(attr, ctx->target_info);
	if (err == E_HM_OK) {
		err = ctx->ipc(attr, ctx->private_data);
	}
	return err;
}

static int __do_ipc_fetch_with_retry(struct bunch_ipc_attr *attr, struct bunch_ipc_reader_ctx *ctx)
{
	int err = E_HM_OK;
	unsigned int i;

	err = __do_ipc_fetch(attr, ctx);
	/* do retry */
	for (i = 0U; i < BUNCH_IPC_FETCH_RETRY_MAX; i++) {
		if (err == E_HM_OK) {
			/* shm size is enough break the retry loop */
			break;
		} else if (err == E_HM_NOMEM) {
			/* only E_HM_NOMEM could be handled with larger memory,
			 * free old shm and re-alloc new with larger size
			 */
			(void)bunch_ipc_inner_free(attr);
			err = bunch_ipc_inner_alloc(attr, bunch_ipc_resize((size_t)attr->size));
			if (err != E_HM_OK) {
				/* alloc or fetch fail, return directly */
				break;
			}
			err = __do_ipc_fetch(attr, ctx);
		} else {
			/* other error, return directly */
			break;
		}
	}

	return err;
}

int bunch_ipc_inner_fetch(struct bunch_ipc_attr *attr, struct bunch_ipc_reader_ctx *ctx)
{
	int err = E_HM_OK;
	if (attr == NULL || ctx == NULL || ctx->ipc == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = __do_ipc_fetch(attr, ctx);
	}

	return err;
}

int bunch_ipc_inner_tryfetch(struct bunch_ipc_attr *attr, struct bunch_ipc_reader_ctx *ctx)
{
	int err = E_HM_OK;
	if (attr == NULL || ctx == NULL || ctx->ipc == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = __do_ipc_fetch_with_retry(attr, ctx);
	}

	return err;
}

static int __do_fill_ipc_bunch(struct bunch_ipc_attr *attr, struct bunch *bunch)
{
	int err = E_HM_OK;
	void *old_payload = bunch_payload_of(bunch);
	void *payload = NULL;

	payload = hm_mem_mmap_posix(NULL, (size_t)attr->size, MPROT_READ | MPROT_WRITE,
				    MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED,
				    attr->shm_id, 0, &err);
	if (err != E_HM_OK) {
		hm_debug("bunch_ipc fill bunch failed:%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		if (old_payload != NULL) {
			(void)hm_mem_munmap(old_payload, bunch->size);
		}
		bunch->payload = payload;
		bunch->size = (size_t)attr->size;
		bunch->status = BUNCH_STATUS_OK;
		bunch->position = 0;
	}

	return err;
}

int bunch_ipc_inner_fill(struct bunch_ipc_attr *attr, struct bunch *bunch)
{
	int err = E_HM_OK;

	if (attr == NULL || bunch == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = __do_fill_ipc_bunch(attr, bunch);
	}

	return err;
}

int bunch_ipc_inner_drain(struct bunch *bunch)
{
	int err = E_HM_OK;

	if (bunch == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		(void)hm_mem_munmap(bunch->payload, bunch->size);
		mem_zero_s(*bunch);
	}

	return err;
}

static int __ipc_alloc_fill_prepare(struct bunch **out_bunch,
				    struct bunch_ipc_attr **out_attr)
{
	int err = E_HM_OK;
	struct bunch *bunch = NULL;
	struct bunch_ipc_attr *attr = NULL;

	bunch = malloc(sizeof(struct bunch));
	if (bunch == NULL) {
		err = E_HM_NOMEM;
	}

	if (err == E_HM_OK) {
		attr = malloc(sizeof(struct bunch_ipc_attr));
		if (attr == NULL) {
			err = E_HM_NOMEM;
			free(bunch);
		}
	}

	if (err == E_HM_OK) {
		mem_zero_b(bunch, sizeof(struct bunch));
		mem_zero_b(attr, sizeof(struct bunch_ipc_attr));
		attr->shm_id = -1;
		*out_bunch = bunch;
		*out_attr = attr;
	}

	return err;
}

struct bunch *bunch_ipc_create(size_t size)
{
	int err = E_HM_OK;
	size_t real_size = size;
	struct bunch *bunch = NULL;
	struct bunch_ipc_attr *attr = NULL;

	if (real_size < BUNCH_IPC_DEFAULT_SIZE) {
		real_size = BUNCH_IPC_DEFAULT_SIZE;
	}

	err = __ipc_alloc_fill_prepare(&bunch, &attr);
	if (err < 0) {
		hm_debug("prepare mem for bunch failed:%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		err = bunch_ipc_inner_alloc(attr, real_size);
		if (err < 0) {
			free(bunch);
			free(attr);
			bunch = NULL;
			attr = NULL;
		}
	}

	if (err == E_HM_OK) {
		bunch_set_private_data(bunch, attr);
	}

	return bunch;
}

void bunch_ipc_destroy(struct bunch *bunch)
{
	struct bunch_ipc_attr *attr = NULL;

	if (bunch != NULL) {
		attr = ptr_from_void(bunch_get_private_data(bunch), struct bunch_ipc_attr);
		bunch_ipc_close(bunch);
	}

	if (attr != NULL) {
		if (attr->shm_id >= 0) {
			(void)hm_ashm_unlink(attr->shm_id);
			attr->shm_id = -1;
		}
		free(attr);
	}
}

/* check and update local shm_id according to key */
static int __ipc_open_update_check(struct bunch_ipc_attr *attr)
{
	int err = E_HM_OK;
	size_t real_size = 0U;
	int shm_id = -1;
	uint32_t owner_idx = attr->owner_idx;

	/* owner_idx 0 means server forget to check key */
	if (owner_idx == (uint32_t)0) {
		hm_debug("bunch_ipc: owner_idx should not be 0\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		shm_id = hm_ashm_open_auth(attr->key, owner_idx, attr->size,
					  SHM_O_RDWR | SHM_O_PHYMEM_ALLOCATED,
					  &real_size);
		if (shm_id < 0) {
			err = shm_id;
			hm_debug("open_shm:%#"PRIx64", size:%u, failed:%s\n",
				 attr->key, attr->size, hmstrerror(err));
		}
	}
	if ((err == E_HM_OK) && (real_size != attr->size)) {
		(void)hm_ashm_close(shm_id);
		err = E_HM_INVAL;
		hm_debug("real_size != attr->size:%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		attr->shm_id = shm_id;
	}

	return err;
}

struct bunch *bunch_ipc_open(struct bunch_ipc_attr *attr)
{
	int err = E_HM_OK;
	struct bunch *bunch = NULL;

	if (attr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		bunch = malloc(sizeof(struct bunch));
		if (bunch == NULL) {
			err = E_HM_NOMEM;
			hm_debug("malloc memory for bunch failed\n");
		}
	}

	if (err == E_HM_OK) {
		err = __ipc_open_update_check(attr);
		if (err < 0) {
			free(bunch);
			bunch = NULL;
		}
	}

	if (err == E_HM_OK) {
		mem_zero_b(bunch, sizeof(struct bunch));
		err = __do_fill_ipc_bunch(attr, bunch);
		if (err < 0) {
			(void)hm_ashm_close(attr->shm_id);
			attr->shm_id = -1;
			free(bunch);
			bunch = NULL;
		}
	}

	if (err == E_HM_OK) {
		bunch_set_private_data(bunch, attr);
	}

	return bunch;
}

void __noinline bunch_ipc_close(struct bunch *bunch)
{
	struct bunch_ipc_attr *attr = NULL;

	if (bunch != NULL) {
		attr = ptr_from_void(bunch_get_private_data(bunch), struct bunch_ipc_attr);
		bunch_set_private_data(bunch, NULL);
		if (bunch_payload_of(bunch) != NULL) {
			(void)hm_mem_munmap(bunch_payload_of(bunch), bunch_size_of(bunch));
			(void)bunch_payload_install(bunch, NULL, 0);
		}
		free(bunch);
	}

	if (attr != NULL) {
		if (attr->shm_id >= 0) {
			/* bunch_ipc_destroy will unlink attr->shm_id, do not set it to -1 */
			(void)hm_ashm_close(attr->shm_id);
		}
	}
}

int bunch_ipc_request(struct bunch *bunch, struct bunch_ipc_reader_ctx *ctx)
{
	int err = E_HM_OK;
	struct bunch_ipc_attr *attr = NULL;

	if (bunch == NULL || ctx == NULL || ctx->ipc == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		attr = ptr_from_void(bunch_get_private_data(bunch), struct bunch_ipc_attr);
		if (attr == NULL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		err = __do_ipc_fetch_with_retry(attr, ctx);
	}
	if (err == E_HM_OK) {
		err = __do_fill_ipc_bunch(attr, bunch);
	}

	return err;
}
