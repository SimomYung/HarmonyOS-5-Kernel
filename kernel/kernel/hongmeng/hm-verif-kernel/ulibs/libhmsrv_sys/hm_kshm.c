/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Kshare-ng function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 13 11:38:29 2023
 */
#include <libmem/utils.h>
#include <libmem/flags.h>
#include <libhmucap/ucap.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_kshm.h>
#include <libhmsrv_sys/hm_thread.h>

int hm_kshm_open(uint64_t key, uint32_t owner_cnode_idx, size_t sz, uint64_t flags,
		 size_t *rsz)
{
	struct __actvret_hmcall_mem_kshm_open_anon actv_ret;
	size_t align_sz;
	int rc;

	mem_zero_s(actv_ret);
	align_sz = PAGE_ALIGN_UP(sz);
	rc = actvcall_hmcall_mem_kshm_open_anon(key, owner_cnode_idx, flags, align_sz,
						&actv_ret);
	if (rc == E_HM_OK) {
		if (rsz != NULL) {
			*rsz = (size_t)actv_ret.ret_len;
		}
		/* return kshm descriptor if succeed, error code if fail */
		rc = actv_ret.kshmd;
	}

	return rc;
}

int hm_kshm_close(int kshmd)
{
	return actvcall_hmcall_mem_shm_close(kshmd);
}

int hm_kshm_unlink(int kshmd)
{
	return actvcall_hmcall_mem_shm_unlink_anon(kshmd);
}

int hm_kshm_grant_to_caller(int kshmd, uint32_t flags, uint64_t *key)
{
	struct __actvret_hmcall_mem_grant_shm_to_caller actv_ret;
	int rc;

	mem_zero_s(actv_ret);
	rc = actvxactcall_hmcall_mem_grant_shm_to_caller(true, false, kshmd, flags, &actv_ret);
	if (rc == E_HM_OK) {
		if (key != NULL) {
			*key = actv_ret.key;
		}
	}
	return rc;
}

int hm_kshm_grant(int kshmd, uint32_t flags, xref_t xref, uint64_t *key)
{
	struct __actvret_hmcall_mem_grant_shm_by_xref actv_ret;
	int rc;

	mem_zero_s(actv_ret);
	rc = actvcall_hmcall_mem_grant_shm_by_xref(kshmd, flags, xref, &actv_ret);
	if (rc == E_HM_OK) {
		if (key != NULL) {
			*key = actv_ret.key;
		}
	}
	return rc;
}

int hm_kshm_grant_by_cnode_idx(int kshmd, uint32_t flags, uint32_t tgt_idx, uint64_t *key)
{
	int rc;
	xref_t xref;

	mem_zero_s(xref);
	/*
	 * export self thread cref to target process with tgt_idx so that we
	 * could get a valid uref for hm_kshm_grant to process since hm_kshm_grant
	 * only accept valid xref. Export with QUERY RIGHTS as hm_kshm_grant
	 * need to stat info using this xref.
	 */
	xref.uref = hm_ucap_export(0, tgt_idx,
				   CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX,
				   hm_ucap_self_cnode_cref(), 0, 0);
	if (IS_REF_ERR(xref.uref)) {
		rc = REF_TO_ERR(xref.uref);
	} else {
		rc = hm_kshm_grant(kshmd, flags, xref, key);
		(void)hm_ucap_revoke(0, xref.uref);
	}

	return rc;
}

int hm_kshm_grant_trans(uint64_t key, xref_t xref)
{
	return actvcall_hmcall_mem_ashm_grant_trans(key, xref);
}
