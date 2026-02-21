/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Share memory function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 07 19:54:36 2021
 */

#include <libmem/utils.h>
#include <libmem/flags.h>
#include <libhmucap/ucap.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_thread.h>

static int do_hm_ashm_open(uint64_t key, uint32_t owner_cnode_idx,
			   size_t sz, uint64_t flags, size_t *rsz)
{
	struct __actvret_hmcall_mem_shm_open_anon actv_ret;
	int rc;

	mem_zero_s(actv_ret);
	rc = actvcall_hmcall_mem_shm_open_anon(key, owner_cnode_idx,
					       flags, sz, &actv_ret);
	if (rc == E_HM_OK) {
		if (rsz != NULL) {
			*rsz = (size_t)actv_ret.ret_len;
		}
		/* return shm id if success */
		rc = actv_ret.shm_id;
	}

	return rc;
}

int hm_ashm_open_auth(uint64_t key, uint32_t owner_cnode_idx, size_t sz,
		      uint64_t flags, size_t *rsz)
{
	size_t align_len;
	int rc;

	align_len = PAGE_ALIGN_UP(sz);
	rc = do_hm_ashm_open(key, owner_cnode_idx, align_len, flags, rsz);
	while ((rc == E_HM_NOMEM) && ((flags & SHM_O_NOFAIL) != 0U)) {
		(void)hm_thread_yield();
		rc = do_hm_ashm_open(key, owner_cnode_idx, align_len, flags, rsz);
	}

	return rc;
}

int hm_ashm_open(uint64_t key, size_t sz, uint64_t flags, size_t *rsz)
{
	size_t align_len;
	int rc;

	align_len = PAGE_ALIGN_UP(sz);
	rc = do_hm_ashm_open(key, CNODE_IDX_INVALD_ZERO, align_len, flags, rsz);
	while ((rc == E_HM_NOMEM) && ((flags & SHM_O_NOFAIL) != 0U)) {
		(void)hm_thread_yield();
		rc = do_hm_ashm_open(key, CNODE_IDX_INVALD_ZERO, align_len, flags, rsz);
	}

	return rc;
}

int hm_ashm_close(int sd)
{
	return actvcall_hmcall_mem_shm_close(sd);
}

int hm_ashm_unlink(int sd)
{
	return actvcall_hmcall_mem_shm_unlink_anon(sd);
}

int hm_ashm_check_key(uint64_t key, uint32_t owner)
{
	int rc;

	rc = actvcall_hmcall_mem_shm_check_owner(key, owner);
	if (rc < 0) {
		hm_debug("actvcall_hmcall_mem_shm_check_owner failed: "
			 "error=%s\n", hmstrerror(rc));
	}
	return rc;
}

int hm_ashm_grant_to_caller(int sd, uint32_t flags, uint64_t *key)
{
	struct __actvret_hmcall_mem_grant_shm_to_caller actv_ret;
	int rc;

	mem_zero_s(actv_ret);
	rc = actvxactcall_hmcall_mem_grant_shm_to_caller(true, false, sd, flags, &actv_ret);
	if (rc == E_HM_OK) {
		if (key != NULL) {
			*key = actv_ret.key;
		}
	}
	return rc;
}

int hm_ashm_grant(int sd, uint32_t flags, xref_t xref, uint64_t *key)
{
	struct __actvret_hmcall_mem_grant_shm_by_xref actv_ret;
	int rc;

	mem_zero_s(actv_ret);
	rc = actvcall_hmcall_mem_grant_shm_by_xref(sd, flags, xref, &actv_ret);
	if (rc == E_HM_OK) {
		if (key != NULL) {
			*key = actv_ret.key;
		}
	}
	return rc;
}

int hm_ashm_grant_by_cnode_idx(int sd, uint32_t flags, uint32_t tgt_idx, uint64_t *key)
{
	int rc;
	xref_t xref;

	mem_zero_s(xref);
	/*
	 * export self thread cref to target process with tgt_idx so that we
	 * could get a valid uref for hm_ashm_grant to process since hm_ashm_grant
	 * only accept valid xref. Export with QUERY RIGHTS as hm_ashm_grant
	 * need to stat info using this xref.
	 */
	xref.uref = hm_ucap_export(0, tgt_idx,
				   CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX,
				   hm_ucap_self_cnode_cref(), 0, 0);
	if (IS_REF_ERR(xref.uref)) {
		rc = REF_TO_ERR(xref.uref);
	} else {
		rc = hm_ashm_grant(sd, flags, xref, key);
		(void)hm_ucap_revoke(0, xref.uref);
	}

	return rc;
}

int hm_ashm_grant_trans(uint64_t key, xref_t xref)
{
	return actvcall_hmcall_mem_ashm_grant_trans(key, xref);
}
