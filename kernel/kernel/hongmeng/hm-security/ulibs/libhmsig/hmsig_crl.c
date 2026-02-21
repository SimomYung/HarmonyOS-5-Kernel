/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Operations of crl service
 * Author: Huawei OS Kernel Lab
 * Create: Mon June 28 10:51:16 2021
 */
#include <libhmsig/hmsig_crl.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/param.h>

#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>

#include <libseqlock/seqlock.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_security.h>

#include "cms_sig_verify.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

/* local on process */
INNER struct hm_sec_shconf_crl_cfg *cfg_crl_base;

INNER uint32_t crl_backup_number(uint32_t mode)
{
	uint32_t ret = CRL_RAW_SHM_NUM_MAX;
	if (mode == CRL_RAW_SHM_NUM_1) {
		ret = CRL_RAW_SHM_NUM_2;
	} else if (mode == CRL_RAW_SHM_NUM_2) {
		ret = CRL_RAW_SHM_NUM_1;
	}
	return ret;
}

INNER const char *crl_shm_name(uint32_t mode)
{
	const char *ret = NULL;
	if (mode == CRL_RAW_SHM_NUM_1) {
		ret = CRL_RAW_SHM_NAME_1;
	} else if (mode == CRL_RAW_SHM_NUM_2) {
		ret = CRL_RAW_SHM_NAME_2;
	}
	return ret;
}

INNER int crl_shm_get(struct hmsig_shm *shm, const char *name, size_t sz,
		      bool create, bool writable)
{
	void *addr = NULL;
	size_t len = 0;
	int ret = E_HM_OK;
	int fd = -1;

	if (shm == NULL || sz == 0) {
		hm_error("parameters are invalid, sz=%zu\n", sz);
		ret = E_HM_INVAL;
		goto out;
	}
	/* owner has the permission of write, user and group have the permission of read */
	mode_t mode = (unsigned)(S_ISYS_SRVC | S_IRUSR | S_IRGRP);
	fd = hm_mem_shm_open_with_mode(name, sz, create ? O_CREAT | O_EXCL | O_RDONLY : O_RDONLY, mode, &len);
	if (fd < 0) {
		hm_error("failed to open the shared memory, name=%s, err=%s\n",
			 name, hmstrerror(fd));
		ret = fd;
		goto out;
	}
	if (len < sz) {
		hm_error("the actual length of shm is smaller than the expected size, len=%zu, sz=%zu\n", len, sz);
		ret = E_HM_RESFAULT;
		goto close_fd;
	}
	addr = hm_mem_mmap(0, len, (create || writable) ? (PROT_READ | PROT_WRITE) : PROT_READ,
			   MAP_SHARED | MAP_ANON, fd, 0);
	if (addr == MAP_FAILED) {
		hm_error("failed to map shm, name=%s\n", name);
		ret = E_HM_NOMEM;
		goto close_fd;
	}

	shm->base = (uintptr_t)addr;
	shm->size = len;
	shm->shm_fd = fd;

	return ret;
close_fd:
	(void)hm_mem_shm_close(fd);
out:
	return ret;
}

INNER void crl_shm_put(const struct hmsig_shm *shm)
{
	(void)hm_mem_munmap(ptr_to_type(shm->base, void *), shm->size);
	(void)hm_mem_shm_close(shm->shm_fd);
}

INNER void crl_shm_destroy(struct hmsig_shm *shm, const char *shm_nm)
{
	crl_shm_put(shm);
	(void)hm_mem_shm_unlink(shm_nm);
}

/* When server spawn, init crl ctrl cfg. Include
 * sysmgr(patchmgr), devmgr ...
 */
INNER void crl_ctrl_shm_init(bool create)
{
	cfg_crl_base = hm_sec_shconf_crl_ctrl();
	if (create) {
		cfg_crl_base->real_space = 0;
		cfg_crl_base->raw_crl_sz = 0;
		cfg_crl_base->seq_lock = 0;
		cfg_crl_base->raw_shm_num = CRL_RAW_SHM_NUM_1;
		cfg_crl_base->has_default_crl = false;
	}
}

INNER int crl_shm_create(const void *raw_data, size_t sz, const char *shm_name, struct hmsig_shm *raw_shm)
{
	int ret;
	/* create(open) and mmap the shm of crl */
	ret = crl_shm_get(raw_shm, shm_name, PAGE_ALIGN_UP(sz), true, false);
	if (ret != E_HM_OK) {
		hm_error("failed to create shared memory, ret=%s, name=%s\n",
			 hmstrerror(ret), shm_name);
	} else {
		ret = memcpy_s(ptr_to_void(raw_shm->base), PAGE_ALIGN_UP(sz), raw_data, sz);
		if (ret != 0) {
			hm_error("failed to copy crl file into mpool\n");
			crl_shm_destroy(raw_shm, shm_name);
			ret = E_HM_INVAL;
		} else {
			hm_info("create crl, shm=%s, sz=%zu\n", shm_name, sz);
		}
	}

	return ret;
}

/* Add seq write lock protection when using, we need to do lazy create by sysmgr */
INNER int crl_shm_do_create(const void *raw_data, size_t raw_sz)
{
	int ret = E_HM_OK;
	struct hmsig_shm raw_shm = {0};
	if (cfg_crl_base->has_default_crl == true) {
		hm_error("already have default crl\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = cms_check_crl_format(raw_data, raw_sz);
	}

	if (ret == E_HM_OK) {
		ret = crl_shm_create(raw_data, raw_sz, CRL_RAW_SHM_NAME_1, &raw_shm);
		if (ret == E_HM_OK) {
			cfg_crl_base->raw_crl_sz = raw_sz;
			cfg_crl_base->real_space = raw_shm.size;
			cfg_crl_base->has_default_crl = true;
			/* unmap and close */
			crl_shm_put(&raw_shm);
		}
	}
	return ret;
}

/*
 * Sysmgr is responsible for completing the creation of shm.
 * 1. ctrl shm：
 * 	saved the configuration of the real crl shm
 * 	record the rwlock state of real crl shm
 * 	sysmgr, devmgr ... should have the write permission of it
 * 2. real shm:
 * 	saved the data of crl
 * 	only have the read permission except sysmgr
 */
int hmsig_crl_shm_create(const void *raw_data, size_t raw_sz)
{
	int ret = E_HM_OK;
	/* check input when need to create raw shm */
	if (raw_sz >= UINT_MAX) {
		hm_error("raw_sz is invalid on create raw shm, raw_sz=%zu\n", raw_sz);
		ret = E_HM_INVAL;
	}
	/* init crl ctrl shm */
	if (ret == E_HM_OK) {
		crl_ctrl_shm_init(true);
		/* check dynamic switch from dtb */
		if (!hm_sec_shconf_cms_builtin_crl()) {
			/* During sysmgr booting, allow init without default crl */
			if (raw_data != NULL && raw_sz != 0) {
				seq_write_start(&cfg_crl_base->seq_lock);
				ret = crl_shm_do_create(raw_data, raw_sz);
				seq_write_end(&cfg_crl_base->seq_lock);
			}
		}
	}

	return ret;
}

/*
 * Devmgr ... initialize ctrl shm
 */
int hmsig_crl_shm_init(void)
{
	int ret;
	ret = hm_sec_shconf_init();
	if (ret != E_HM_OK) {
		hm_error("failed to initialize shconf, err=%s\n", hmstrerror(ret));
	} else {
		crl_ctrl_shm_init(false);
		hm_info("crl ctrl shm initialized successfully\n");
	}

	return ret;
}

INNER int _crl_cms_verify_sig(const void *data, size_t size, const void *sig, size_t sig_size)
{
	int ret = E_HM_OK;
	struct hmsig_shm shm = {0};
	struct hmsig_shm verif_shm = {0};

	/* page align up of raw_crl_sz is always not larger than real_space(length of the shm). */
	ret = crl_shm_get(&shm, crl_shm_name(cfg_crl_base->raw_shm_num), cfg_crl_base->raw_crl_sz, false, false);
	if (ret != E_HM_OK) {
		hm_error("failed to map the shared memory, ret=%s, shm_num=%u\n",
			 hmstrerror(ret), cfg_crl_base->raw_shm_num);
	} else {
		verif_shm.base = shm.base;
		verif_shm.size = cfg_crl_base->raw_crl_sz;
		ret = cms_verify_sig(&verif_shm, data, size, sig, sig_size);
		/* put the shm of default crl */
		crl_shm_put(&shm);
	}

	return ret;
}

INNER int crl_cms_verify_sig(const void *data, size_t size, const void *sig, size_t sig_size, uint32_t verif_mode)
{
	int ret = E_HM_OK;

	if (cfg_crl_base->has_default_crl == true) {
		ret = _crl_cms_verify_sig(data, size, sig, sig_size);
	} else {
		ret = cms_verify_sig(NULL, data, size, sig, sig_size);
	}

	if (ret != E_HM_OK && verif_mode == HM_SEC_SHCONF_SIG_PERMISSIVE) {
		/* permissive mode recorrects the result after signature verification. */
		hm_info("ignore cms verification due to permissive mode\n");
		ret = E_HM_OK;
	}

	return ret;
}

/*
 * Support signature verification of sysmgr, devmgr and ...
 */
int hmsig_crl_verify_sig(const void *data, size_t size, const void *sig, size_t sig_size)
{
	seq_t ctx_seq;
	int ret = E_HM_OK;
	uint32_t verif_mode;

	if (data == NULL || sig == NULL || size == 0 || sig_size == 0) {
		hm_error("input parameters are invalid\n");
		ret = E_HM_INVAL;
	} else {
		/* check dynamic open from dtb */
		verif_mode = hm_sec_shconf_cms_verif_mode();
		if (verif_mode == HM_SEC_SHCONF_SIG_DISABLED) {
			hm_debug("skip cms verification due to disabled mode\n");
			ret = E_HM_OK;
		} else {
			do {
				ctx_seq = seq_try(&cfg_crl_base->seq_lock);
				ret = crl_cms_verify_sig(data, size, sig, sig_size, verif_mode);
			} while (seq_need_retry(&cfg_crl_base->seq_lock, ctx_seq));
		}
	}

	return ret;
}

INNER int crl_verif_mode_check(void)
{
	int ret = E_HM_OK;
	if (hm_sec_shconf_cms_builtin_crl()) {
		hm_error("Does not manage revocation lists, mode %u\n", hm_sec_shconf_cms_verif_mode());
		ret = E_HM_NOSYS;
	}

	return ret;
}

INNER int crl_shm_read(struct hmsig_crl_read_ctx *ctx)
{
	int ret = E_HM_OK;
	unsigned int crl_sz;
	seq_t ctx_seq;
	struct hmsig_shm shm = {0};
	/*
	 * Loop termination condition:
	 * Following loop terminate when seq_need_retry return false.
	 * Since ctx_seq is always changing within a finite state,
	 * it must be stable with consistent results.
	 * Loop condition is constant and reasonable:
	 * the ctx_seq is seq_try in loop body,
	 * seq_need_retry will return false after seq_retry,
	 * so the loop condition is reasonable.
	 */
	do {
		ctx_seq = seq_try(&cfg_crl_base->seq_lock);
		ctx->read_sz = 0U;
		ctx->left_to_end = 0U;
		/* check the existence of default_crl under seq lock */
		if (!cfg_crl_base->has_default_crl) {
			ret = E_HM_NOOBJ;
			continue;
		}
		ret = crl_shm_get(&shm, crl_shm_name(cfg_crl_base->raw_shm_num),
				  cfg_crl_base->raw_crl_sz, false, false);
		if (ret != E_HM_OK) {
			hm_error("failed to map the shared memory of crl, ret=%s, shm_num=%u\n",
				 hmstrerror(ret), cfg_crl_base->raw_shm_num);
			break;
		}
		/* raw_crl_sz were always less than UINT_MAX.
		 * If raw_crl_sz is smaller than offset, nothing will be read.
		 */
		crl_sz = (unsigned int)cfg_crl_base->raw_crl_sz;
		if (ctx->offset < crl_sz) {
			ctx->read_sz = MIN(ctx->dst_sz, crl_sz - ctx->offset);
			hm_debug("read record: dst_sz=%u, read_sz=%u \n", ctx->dst_sz, ctx->read_sz);
			ret = ctx->copy(ctx->dst, ctx->dst_sz,
				ptr_to_void(ptr_to_type(shm.base, char *) + ctx->offset), ctx->read_sz);
			if (ret != E_HM_OK) {
				ctx->read_sz = 0U;
				crl_shm_put(&shm);
				break;
			}
			ctx->left_to_end = (crl_sz - ctx->offset) - ctx->read_sz;
		}
		crl_shm_put(&shm);
	} while (seq_need_retry(&cfg_crl_base->seq_lock, ctx_seq));

	return ret;
}

/*
 * When batch reading, it can not guarantee that the data will be changeless.
 * It's sure that the shm will be locked on one-time reading process.
 * Of course, it is allowed to read all at once.
 */
int hmsig_crl_shm_read(struct hmsig_crl_read_ctx *ctx)
{
	int ret = E_HM_OK;

	/* paramter check, dst will be checked by copy function */
	if (ctx == NULL || ctx->dst_sz == 0 || ctx->copy == NULL) {
		hm_error("input parameters are invalid\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = crl_verif_mode_check();
	}

	if (ret == E_HM_OK) {
		ret = crl_shm_read(ctx);
	}

	return ret;
}

/* Only owner can get crl size, the right is same with read crl */
int hmsig_crl_size(size_t *crl_sz)
{
	int ret = E_HM_OK;
	seq_t ctx_seq;
	struct hmsig_shm shm = {0};

	if (crl_sz == NULL) {
		hm_error("input parameter is invalid\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = crl_verif_mode_check();
	}

	if (ret == E_HM_OK) {
		/*
		 * Loop termination condition:
		 * Following loop terminate when seq_need_retry return false.
		 * Since ctx_seq is always changing within a finite state,
		 * it must be stable with consistent results.
		 * Loop condition is constant and reasonable:
		 * the ctx_seq is seq_try in loop body,
		 * seq_need_retry will return false after seq_retry,
		 * so the loop condition is reasonable.
		 */
		do {
			ctx_seq = seq_try(&cfg_crl_base->seq_lock);
			/* check the existence of default_crl */
			if (cfg_crl_base->has_default_crl == false) {
				hm_debug("default crl doesn't exist\n");
				ret = E_HM_NOOBJ;
				continue;
			}
			ret = crl_shm_get(&shm, crl_shm_name(cfg_crl_base->raw_shm_num),
					  cfg_crl_base->raw_crl_sz, false, false);
			if (ret != E_HM_OK) {
				hm_error("failed to map the shared memory, ret=%s, shm_num=%u\n",
					 hmstrerror(ret), cfg_crl_base->raw_shm_num);
				break;
			}
			*crl_sz = cfg_crl_base->raw_crl_sz;
			crl_shm_put(&shm);
		} while (seq_need_retry(&cfg_crl_base->seq_lock, ctx_seq));
	}

	return ret;
}

INNER void crl_shm_unlink(const char* shm_name)
{
	int ret;
	ret = hm_mem_shm_unlink(shm_name);
	if (ret < 0) {
		hm_error("failed to unlink default_crl, err=%s\n", hmstrerror(ret));
	}
	cfg_crl_base->raw_crl_sz = 0;
	cfg_crl_base->real_space = 0;
}

INNER int crl_shm_do_update(const void *raw_data, size_t raw_sz)
{
	int ret;
	size_t cur_shm_sz;
	uint32_t new_shm_num;
	struct hmsig_shm new_crl_shm = {0};
	struct hmsig_shm old_crl_shm = {0};

	/* map the shm with the permission of write */
	ret = crl_shm_get(&old_crl_shm,
			  crl_shm_name(cfg_crl_base->raw_shm_num), cfg_crl_base->real_space, false, true);
	if (ret != E_HM_OK) {
		hm_error("failed to map the shared memory, ret=%s, shm_num=%u\n",
			 hmstrerror(ret), cfg_crl_base->raw_shm_num);
	}

	/* check time and format, raw_crl_sz is always not larger than real_space */
	if (ret == E_HM_OK) {
		ret = cms_check_crl(ptr_to_void(old_crl_shm.base), cfg_crl_base->raw_crl_sz, raw_data, raw_sz);
		if (ret != E_HM_OK) {
			crl_shm_put(&old_crl_shm);
		}
	}

	if (ret == E_HM_OK) {
		/* get the shm real size that be mapped before */
		cur_shm_sz = old_crl_shm.size;
		if (raw_sz <= cur_shm_sz) {
			NOFAIL(memcpy_s(ptr_to_void(old_crl_shm.base), cur_shm_sz, raw_data, raw_sz));
			cfg_crl_base->raw_crl_sz = raw_sz;
		} else {
			/* prepare to get shm number */
			new_shm_num = crl_backup_number(cfg_crl_base->raw_shm_num);
			/* relloc the new crl shm and memcpy data as sysmgr */
			ret = crl_shm_create(raw_data, raw_sz, crl_shm_name(new_shm_num), &new_crl_shm);
			if (ret != E_HM_OK) {
				hm_error("failed to create new crl shm, ret=%s, new shm_num=%u\n",
					 hmstrerror(ret), new_shm_num);
			} else {
				/* unlink the original shm before unmap, the actual release time
				 * of the shm is when the reference count is zero.
				 */
				crl_shm_unlink(crl_shm_name(cfg_crl_base->raw_shm_num));
				/* init raw crl shm and cfg crl base infomation */
				cfg_crl_base->raw_shm_num = new_shm_num;
				cfg_crl_base->raw_crl_sz = raw_sz;
				cfg_crl_base->real_space = new_crl_shm.size;
				/* ummap and close the new crl shm */
				crl_shm_put(&new_crl_shm);
			}
		}
		/* ummap and close the old crl shm */
		crl_shm_put(&old_crl_shm);
	}

	return ret;
}

INNER int crl_shm_write(const void *raw_data, size_t raw_sz)
{
	int ret;
	seq_write_start(&cfg_crl_base->seq_lock);
	/* If there has never been a default crl, create a new one.
	 * If the CRL already exists, it needs to be compared with the old one.
	 * And if the requirements are met, the CRL will be updated.
	 */
	if (cfg_crl_base->has_default_crl == false) {
		ret = crl_shm_do_create(raw_data, raw_sz);
	} else {
		ret = crl_shm_do_update(raw_data, raw_sz);
	}
	seq_write_end(&cfg_crl_base->seq_lock);
	return ret;
}

/*
 * Before update crl on shm, the format and time of the new revocation list
 * should be checked. Moreover, if the new crl is larger than older, a new
 * section of shared memory will be allocated. Raw crl shm cannot be smaller.
 */
int hmsig_crl_shm_write(const void *raw_data, size_t raw_sz)
{
	int ret;

	/* paramter check */
	if (raw_data == NULL || raw_sz == 0 || raw_sz >= UINT_MAX) {
		hm_error("input parameters are invalid on crl write, raw_sz=%zu\n", raw_sz);
		ret = E_HM_INVAL;
	} else {
		ret = crl_verif_mode_check();
		if (ret == E_HM_OK) {
			ret = crl_shm_write(raw_data, raw_sz);
			if (ret != E_HM_OK) {
				hm_debug("failed to write crl shm, ret=%s\n", hmstrerror(ret));
			}
		}
	}

	return ret;
}
