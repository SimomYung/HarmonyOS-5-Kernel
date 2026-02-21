/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of shared memory list
 * Author: Huawei OS Kernel Lab
 * Create: Thur Feb 2 14:50:00 2023
 */

/* direct header */
#include "sec_cred.h"

/* libs */
#include <fcntl.h>
#include <string.h>

/* hongmeng ulibs */
#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_coobj.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libsec/cred.h>

cred_ator g_sec_cred_ator;
vatomic32_t sec_cocred_inited = VATOMIC_INIT(0U);;

__thread struct cred_context sysif_cred_cache;

int sec_cocred_init(enum cred_role role)
{
	int ret;
	if (vatomic32_cmpxchg(&sec_cocred_inited, 0U, 1U) == 1U) {
		return E_HM_OK;
	}
	ret = coobj_init("cred", &g_sec_cred_ator, (role == CRED_OWNER) ? COOBJ_ROLE_HOLDER : COOBJ_ROLE_RENTER,
			 true, S_IRUSR);
	return ret;
}

int sec_cred_alloc(size_t sz, cred_idx_t *idx)
{
	size_t r_idx;

	r_idx = coobj_malloc(g_sec_cred_ator, sz);
	if (r_idx != (size_t)____COOBJ_INVALID_POS) {
		*idx = r_idx;
		return E_HM_OK;
	} else {
		hm_error("cred alloc failed \n");
		return E_HM_NOMEM;
	}
}

void sec_cred_free(cred_idx_t idx)
{
	coobj_free(g_sec_cred_ator, idx);
}

int sec_cred_dup(void *src_ptr, cred_idx_t *new, size_t src_sz, size_t dst_sz)
{
	size_t r_idx;
	int err = E_HM_NOMEM;
	void *dst_ptr;
	r_idx = coobj_malloc(g_sec_cred_ator, dst_sz);
	if (r_idx != (size_t)____COOBJ_INVALID_POS) {
		*new = r_idx;
#ifdef __aarch64__
		dst_ptr = coobj_offset_to_ptr(r_idx);
#else
		dst_ptr = coobj_idx_to_ptr(g_sec_cred_ator, r_idx);
#endif
		if (dst_ptr == NULL)
			goto free_out;
		err = memcpy_s(dst_ptr, dst_sz, src_ptr, src_sz);
		if (err != 0) {
			err = E_HM_INVAL;
			goto free_out;
		} else {
			return E_HM_OK;
		}
	}
	goto out;
free_out:
	coobj_free(g_sec_cred_ator, r_idx);
out:
	return err;
}

void sec_shared_cred_ref_release(struct raw_refcnt *ref)
{
	struct shared_cred *ptr = container_of(ref, struct shared_cred, cred_refcnt);
	sec_cred_free(ptr->idx);
}

void sec_base_cred_ref_release(struct raw_refcnt *ref)
{
	struct base_cred *ptr = container_of(ref, struct base_cred, cred_refcnt);
	if (ptr->sh_cred != NULL) {
		(void)raw_refcnt_put(&ptr->sh_cred->cred_refcnt);
	}

	if (ptr->cred_dac.groups.size != 0)
		sec_cred_free(ptr->cred_dac.groups.idx);
	if (ptr->cred_encaps != NULL) {
		(void)raw_refcnt_put(&ptr->cred_encaps->cred_refcnt);
	}
	if (ptr->usr_ns_cred != NULL) {
		(void)raw_refcnt_put(&ptr->usr_ns_cred->refcnt);
		ptr->usr_ns_cred = NULL;
		ptr->usr_ns_idx = (size_t)____COOBJ_INVALID_POS;
	}
	sec_cred_free(ptr->idx);
}

void sec_str_cred_ref_release(struct raw_refcnt *ref)
{
	struct str_cred *ptr = container_of(ref, struct str_cred, cred_refcnt);
	if (ptr->size != 0) {
		sec_cred_free(ptr->str_idx);
	}
	sec_cred_free(ptr->idx);
}

void sec_cred_encaps_ref_release(struct raw_refcnt *ref)
{
	struct sec_cred_encaps *ptr = container_of(ref, struct sec_cred_encaps, cred_refcnt);
	if (ptr->count != 0) {
		sec_cred_free(ptr->idx);
	}
	sec_cred_free(ptr->cred_idx);
}

void sec_usr_ns_cred_ref_release(struct raw_refcnt *ref)
{
	struct usr_ns_cred *ptr = container_of(ref, struct usr_ns_cred, refcnt);
	sec_cred_free(ptr->idx);
}

struct usr_ns_cred *sec_usr_ns_cred_alloc(cred_idx_t *idx_t)
{
	struct usr_ns_cred *ptr = NULL;
	cred_idx_t idx;
	int rc;

	rc = sec_cred_alloc(sizeof(struct usr_ns_cred), &idx);
	if (rc == E_HM_OK) {
		ptr = (struct usr_ns_cred *)sec_cred_idx_to_ptr(idx);
		if (ptr == NULL) {
			sec_cred_free(idx);
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		NOFAIL(memset_s(ptr, sizeof(struct usr_ns_cred), 0, sizeof(struct usr_ns_cred)));
		ptr->idx = idx;
		*idx_t = idx;
		raw_refcnt_init(&ptr->refcnt, 1, sec_usr_ns_cred_ref_release);
	}
	return ptr;
}

struct sec_cred_encaps *sec_cred_encaps_alloc(cred_idx_t *idx_t)
{
	struct sec_cred_encaps *ptr = NULL;
	cred_idx_t idx;
	int rc;

	rc = sec_cred_alloc(sizeof(struct sec_cred_encaps), &idx);
	if (rc != E_HM_OK) {
		return NULL;
	}
	ptr = (struct sec_cred_encaps *)sec_cred_idx_to_ptr(idx);
	if (ptr == NULL) {
		sec_cred_free(idx);
		return NULL;
	}
	NOFAIL(memset_s(ptr, sizeof(struct sec_cred_encaps), 0, sizeof(struct sec_cred_encaps)));
	ptr->cred_idx = idx;
	*idx_t = idx;
	raw_rwlock_init(&ptr->rwlock, NULL);
	raw_refcnt_init(&ptr->cred_refcnt, 1, sec_cred_encaps_ref_release);
	return ptr;
}

#define BASE_CRED_TYPE_ENUM CRED_BASE
#define SHARED_CRED_TYPE_ENUM CRED_SHARED
#define STR_CRED_TYPE_ENUM CRED_STR

#define DEFINE_CRED_ALLOC(cred_type, upper_cred_type) \
struct cred_type##_cred *sec_##cred_type##_cred_alloc(cred_idx_t *idx_t) \
{ \
	struct cred_type##_cred *ptr; \
	cred_idx_t idx; \
	int rc; \
	rc = sec_cred_alloc(sizeof(struct cred_type##_cred), &idx); \
	if (rc != E_HM_OK) \
		return NULL; \
	ptr = (struct cred_type##_cred *)sec_cred_idx_to_ptr(idx); \
	if (ptr == NULL) { \
		sec_cred_free(idx); \
		return NULL; \
	} \
	NOFAIL(memset_s(ptr, sizeof(struct cred_type##_cred), \
			0, sizeof(struct cred_type##_cred))); \
	ptr->idx = idx; \
	ptr->type = upper_cred_type##_CRED_TYPE_ENUM; \
	*idx_t = idx; \
	raw_refcnt_init(&ptr->cred_refcnt, 1, sec_##cred_type##_cred_ref_release); \
	return ptr; \
}

DEFINE_CRED_ALLOC(base, BASE)
DEFINE_CRED_ALLOC(shared, SHARED)
DEFINE_CRED_ALLOC(str, STR)

bool sec_cocred_is_inited(void)
{
	if (vatomic32_read(&sec_cocred_inited) == 1U) {
		return true;
	} else {
		return false;
	}
}

/*
* cred: the out buffer to store proc_cred whitch contains mac cred
* cnode_idx: specify the process id which is needed to fetch mac cred
*/

int sec_get_mac_cred_by_cidx(struct proc_cred *cred, uint32_t cnode_idx)
{
	int ret;
	int flags = PROC_CRED_FLAGS_MAC | PROC_ID_FLAGS_CNODE_IDX;
	ret = hm_proc_get_cred_by_id(cred, sizeof(struct proc_cred),
						  (unsigned long long)cnode_idx, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_seharmony_cred_by_cidx(struct proc_cred *cred, uint32_t cnode_idx)
{
	int ret;
	int flags = PROC_CRED_FLAGS_SEHARMONY | PROC_ID_FLAGS_CNODE_IDX;
	ret = hm_proc_get_cred_by_id(cred, sizeof(struct proc_cred),
						  (unsigned long long)cnode_idx, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_mac_cred_by_pid_nsid(struct proc_cred *cred, pid_t pid, unsigned int nsid)
{
	int ret;
	int flags = PROC_CRED_FLAGS_MAC | PROC_ID_FLAGS_PID;
	ret = hm_proc_get_cred_by_id_in_pidns(cred, sizeof(struct proc_cred),
					      (unsigned long long)(unsigned)pid, nsid, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_mac_cred_by_pid(struct proc_cred *cred, pid_t pid)
{
	int ret;
	int flags = PROC_CRED_FLAGS_MAC | PROC_ID_FLAGS_PID;
	ret = hm_proc_get_cred_by_id(cred, sizeof(struct proc_cred),
						  (unsigned long long)(unsigned)pid, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_seharmony_cred_by_pid(struct proc_cred *cred, pid_t pid)
{
	int ret;
	int flags = PROC_CRED_FLAGS_SEHARMONY | PROC_ID_FLAGS_PID;
	ret = hm_proc_get_cred_by_id(cred, sizeof(struct proc_cred),
						  (unsigned long long)(unsigned)pid, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_dac_cred_by_pid(struct proc_cred *cred, pid_t pid)
{
	int ret;
	int flags = PROC_CRED_FLAGS_DAC | PROC_ID_FLAGS_PID;
	ret = hm_proc_get_cred_by_id(cred, sizeof(struct proc_cred),
						  (unsigned long long)(unsigned)pid, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_dac_cred_dump_by_pid_nsid(struct proc_cred *cred, pid_t pid, unsigned int nsid)
{
	int ret;
	int flags = PROC_CRED_FLAGS_DAC | PROC_CRED_FLAGS_DUMP | PROC_ID_FLAGS_PID;
	ret = hm_proc_get_cred_by_id_in_pidns(cred, sizeof(struct proc_cred),
					      (unsigned long long)(unsigned)pid, nsid, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_proc_cred_dump_by_pid_nsid(struct proc_cred *cred, pid_t pid, unsigned int nsid)
{
	int ret;
	int flags = PROC_CRED_FLAGS_DAC | PROC_CRED_FLAGS_DUMP | PROC_ID_FLAGS_PID |
		    PROC_CRED_FLAGS_SEHARMONY | PROC_CRED_FLAGS_MAC;
	ret = hm_proc_get_cred_by_id_in_pidns(cred, sizeof(struct proc_cred),
					      (unsigned long long)(unsigned)pid, nsid, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_dac_cred_dump_by_pid(struct proc_cred *cred, pid_t pid)
{
	int ret;
	int flags = PROC_CRED_FLAGS_DAC | PROC_CRED_FLAGS_DUMP | PROC_ID_FLAGS_PID;
	ret = hm_proc_get_cred_by_id(cred, sizeof(struct proc_cred),
						  (unsigned long long)(unsigned)pid, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

int sec_get_prop_by_pid(struct proc_cred *cred, pid_t pid)
{
	int ret;
	int flags = PROC_CRED_FLAGS_PROP | PROC_ID_FLAGS_PID;
	ret = hm_proc_get_cred_by_id(cred, sizeof(struct proc_cred),
						  (unsigned long long)(unsigned)pid, flags);
	if (ret < 0) {
		hm_debug("hm process get cred by id failed, err=%s\n",
		hmstrerror(ret));
	}

	return ret;
}

static int sec_cred_fill_src_cred(struct requester *req)
{
	__u64 tmp_idx = 0U;
	void *sh_cred;
	void *b_cred;
	int err = sec_sysfast_actv_cred(&tmp_idx);
	if ((err != E_HM_OK || tmp_idx == 0)) {
		hm_error("self cred idx is invalid\n");
		return E_HM_INVAL;
	}
	sec_cred_idx_to_cred((cred_idx_t)tmp_idx,
			&b_cred, &sh_cred);
	req->src_base_cred = b_cred;
	req->src_sh_cred = sh_cred;
	req->src_cred_idx = (cred_idx_t)tmp_idx;
	req->src_cnode_idx = (uint32_t)tmp_idx;
	return E_HM_OK;
}

int sec_cred_copy_from_req(struct sec_local_cred *ret_cred)
{
	int rc;
	struct requester req = {0};

	if (ret_cred == NULL) {
		return E_HM_INVAL;
	}
	rc = requester_init(&req);
	if (rc != E_HM_OK) {
		hm_error("failed to init requester\n");
		return rc;
	}
	rc = sec_cred_fill_src_cred(&req);
	if (rc != E_HM_OK) {
		return rc;
	}
	ret_cred->fwd_sh_cred = *(struct shared_cred *)req.fwd_sh_cred;
	ret_cred->fwd_base_cred = *(struct base_cred *)req.fwd_base_cred;
	ret_cred->fwd_cnode_idx = req.fwd_cnode_idx;
	ret_cred->src_sh_cred = *(struct shared_cred *)req.src_sh_cred;
	ret_cred->src_base_cred = *(struct base_cred *)req.src_base_cred;
	ret_cred->src_cnode_idx = req.src_cnode_idx;
#ifdef ENABLE_PAC_HARD_CODE
	ret_cred->fwd_cred_idx = req.fwd_cred_idx;
	ret_cred->src_cred_idx = req.src_cred_idx;
#endif
	return E_HM_OK;
}

int sec_copy_groups(struct sec_cred_dac *dac_cred, uint32_t group_size, gid_t *group_list)
{
	int ret;
	cred_idx_t idx;
	void *ptr = NULL;
	/* if exec, no need to copy groups again
	 * if fork/spawn, pgrp will be null
	 */
	if (dac_cred->groups.pgrp != NULL)
		return E_HM_OK;
	if (group_size == 0) {
		dac_cred->groups.pgrp = DAC_GROUP_INITIALIZED;
		return E_HM_OK;
	}

	size_t sz = sizeof(gid_t) * group_size;
	ret = sec_cred_alloc(sz, &idx);
	if (ret != E_HM_OK)
		return ret;
	ptr = sec_cred_idx_to_ptr(idx);
	if (ptr == NULL) {
		ret = E_HM_INVAL;
		goto free_idx;
	}
	ret = memcpy_s(ptr, sz, group_list, sz);
	if (ret != 0)
		goto free_idx;
	dac_cred->groups.pgrp = ptr;
	dac_cred->groups.idx = idx;
	dac_cred->groups.size = group_size;

	return E_HM_OK;
free_idx:
	sec_cred_free(idx);
	return ret;
}

int sec_copy_encaps_list(struct sec_cred_encaps *encaps_cred, uint32_t count, struct sec_encaps *encaps_list)
{
	int ret;
	cred_idx_t idx;
	void *ptr = NULL;
	/*
	 * if we go here, thread encaps version is older, we must flash encaps_list
	 */
	encaps_cred->count = count;
	if (count == 0) {
		encaps_cred->encaps_list = NULL;
		return E_HM_OK;
	}

	size_t sz = sizeof(struct sec_encaps) * count;
	ret = sec_cred_alloc(sz, &idx);
	if (ret != E_HM_OK)
		return ret;
	ptr = sec_cred_idx_to_ptr(idx);
	if (ptr == NULL) {
		ret = E_HM_INVAL;
		goto free_idx;
	}
	ret = memcpy_s(ptr, sz, encaps_list, sz);
	if (ret != EOK) {
		ret = E_HM_FAULT;
		goto free_idx;
	}
	if (encaps_cred->encaps_list != NULL && encaps_cred->idx != 0) {
		sec_cred_free(encaps_cred->idx);
	}
	encaps_cred->encaps_list = ptr;
	encaps_cred->idx = idx;

	return E_HM_OK;
free_idx:
	sec_cred_free(idx);
	return ret;
}
