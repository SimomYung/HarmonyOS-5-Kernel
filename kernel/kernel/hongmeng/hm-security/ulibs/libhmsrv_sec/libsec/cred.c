/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Credential base functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:58:09 2019
 */
#include "libsec/cred.h"

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmencaps/encaps_str2int_hash.h>
#include "libsec/audit.h"

#ifndef __HOST_LLT__
static vatomic32_t sec_cred_inited = VATOMIC_INIT(0U);
#endif
int sec_cred_init(bool is_creator)
{
#ifndef __HOST_LLT__
	if (vatomic32_cmpxchg(&sec_cred_inited, 0U, 1U) == 1U) {
		return E_HM_OK;
	}
#endif
	int rc;

	rc = sec_cocred_init((is_creator == true) ? CRED_OWNER : CRED_USER);
	if (rc != E_HM_OK) {
		hm_error("cocred init failed, error =%s\n", hmstrerror(rc));
	}

	return rc;
}

void sec_cred_fini(void)
{
}

int sec_cred_groups_search(const gid_t *group_list, uint32_t size, gid_t gid)
{
	int left;
	int right;
	int mid;
	int rc = E_HM_POSIX_NOENT;

	if (group_list == NULL) {
		rc = E_HM_INVAL;
	} else {
		left = 0;
		right = (int)size - 1;
		/*
		 * Loop termination condition:
		 * Following loop terminate when left is larger than rignt, Since left keep increasing
		 * or the right keep decreasing in loop body, it has a bound.
		 * Loop condition is constant and reasonable: right is dac_cred->groups.offset,
		 * which is limited, so the loop condition is reasonable.
		 */
		while (left <= right) {
			mid = (right - left) / 2 + left; /* 2 for binary search */
			if (gid > group_list[mid]) {
				left = mid + 1;
			} else if (gid < group_list[mid]) {
				right = mid - 1;
			} else {
				rc = E_HM_OK;
				break;
			}
		}
	}

	return rc;
}

/* Check whether the gid is in the group of the dac */
int sec_cred_dac_in_group_check(const struct sec_cred_dac *dac_cred, gid_t gid)
{
	if (dac_cred != NULL) {
		return gid_in_group(dac_cred, gid);
	}
	return E_HM_INVAL;
}

#ifdef CONFIG_ENCAPS
static int find_encaps_in_list(const struct sec_encaps *encaps_list, uint32_t count,
		const char *key, uint32_t *type, uint32_t *value)
{
	int left;
	int right;
	int mid;
	uint32_t int_key = 0;

	const struct encaps_str2int *key_int = find_encaps_str2int(key, strlen(key));
	if (key_int == NULL) {
		return E_HM_NOOBJ;
	}
	int_key = key_int->hash_vl;

	if (encaps_list != NULL) {
		left = 0;
		right = (int)count - 1;
		/*
		* Loop termination condition:
		* Following loop terminate when left is larger than rignt, Since left keep increasing
		* or the right keep decreasing in loop body, it has a bound.
		* Loop condition is constant and reasonable: right is encaps_cred->encaps_list.offset,
		* which is limited, so the loop condition is reasonable.
		*/
		while (left <= right) {
			mid = (right - left) / 2 + left; /* 2 for binary search */
			if (int_key > encaps_list[mid].key) {
				left = mid + 1;
			} else if (int_key < encaps_list[mid].key) {
				right = mid - 1;
			} else {
				*type = encaps_list[mid].type;
				*value = encaps_list[mid].value;
				return E_HM_OK;
			}
		}
	}
	return E_HM_NOOBJ;
}

bool sec_cred_check_encaps(struct sec_cred_encaps *encaps_cred, const char *key, uint32_t type, uint32_t value)
{
	struct sec_encaps *encaps_list = NULL;
	uint32_t saved_type = 0, saved_value = 0;
	bool ret = false;
	if (encaps_cred == NULL) {
		return ret;
	}
	if (encaps_cred->proc_type == ENCAPS_KERNEL_PROC) {
		ret = true;
		goto out;
	} else if ((encaps_cred->proc_type != ENCAPS_HAP_PROC && encaps_cred->proc_type != ENCAPS_SYS_PROC) ||
			encaps_cred->count == 0U) {
		ret = false;
		goto out;
	}
	encaps_list = sec_cred_idx_to_ptr(encaps_cred->idx);
	if (encaps_list == NULL ||
		find_encaps_in_list(encaps_list, encaps_cred->count, key, &saved_type, &saved_value) != E_HM_OK) {
		ret = false;
		goto out;
	}

	if (saved_type == type && saved_value == value) {
		ret = true;
	}
out:
	return ret;
}

bool sec_cred_has_encaps(struct sec_cred_encaps *encaps_cred, const char *key)
{
	struct sec_encaps *encaps_list = NULL;
	uint32_t saved_type = 0, saved_value = 0;
	bool ret = false;
	if (encaps_cred == NULL) {
		return ret;
	}
	if (encaps_cred->proc_type == ENCAPS_KERNEL_PROC) {
		ret = true;
		goto out;
	} else if ((encaps_cred->proc_type != ENCAPS_HAP_PROC && encaps_cred->proc_type != ENCAPS_SYS_PROC) ||
			encaps_cred->count == 0U) {
		ret = false;
		goto out;
	}
	encaps_list = sec_cred_idx_to_ptr(encaps_cred->idx);
	if (encaps_list == NULL ||
		find_encaps_in_list(encaps_list, encaps_cred->count, key, &saved_type, &saved_value) != E_HM_OK) {
		ret = false;
	} else {
		ret = true;
	}
out:
	return ret;
}

int sec_cred_get_encaps(struct sec_cred_encaps *encaps_cred, const char *key, uint32_t *type, uint32_t *value)
{
	struct sec_encaps *encaps_list;
	*type = 0;
	*value = 0;
	int ret = E_HM_OK;
	if (encaps_cred == NULL) {
		return E_HM_NOOBJ;
	}
	if (encaps_cred->proc_type == ENCAPS_UNKNOWN_PROC || encaps_cred->proc_type == ENCAPS_NO_SET_PROC ||
		encaps_cred->proc_type == ENCAPS_KERNEL_PROC || encaps_cred->count == 0U) {
		ret = E_HM_NOOBJ;
		goto out;
	}
	encaps_list = sec_cred_idx_to_ptr(encaps_cred->idx);
	if (encaps_list == NULL || find_encaps_in_list(encaps_list, encaps_cred->count, key, type, value) != E_HM_OK) {
		ret = E_HM_NOOBJ;
	}
out:
	return ret;
}

bool sec_check_encaps(const struct sec_chk_cred *cred, const char *key, uint32_t type, uint32_t value)
{
	bool ret = false;
	struct sec_cred_encaps *encaps_cred = NULL;
    struct base_cred *b_cred = NULL;

	if (cred == NULL || cred->base_cred == NULL) {
		hm_warn("cred is NULL\n");
		return ret;
	}
	b_cred = (struct base_cred *)(cred->base_cred);
	if (b_cred->encaps_idx != 0) {
		encaps_cred = sec_cred_idx_to_ptr(b_cred->encaps_idx);
		if (encaps_cred != NULL) {
			raw_rwlock_rdlock(&encaps_cred->rwlock);
#ifdef ENABLE_PAC_HARD_CODE
			if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
				// local
				hm_pacga_bcred_check(b_cred, SEC_PAC_BASE_ENCAPS, PAC_CHECK_NO_RETRY);
			} else {
				// shared_mem
				hm_pacdb_bcred_check(b_cred, SEC_PAC_BASE_ENCAPS, cred->cred_idx, PAC_CHECK_NO_RETRY);
			}
#endif
			ret = sec_cred_check_encaps(encaps_cred, key, type, value);
			raw_rwlock_unlock(&encaps_cred->rwlock);
		}
	}

	return ret;
}
bool sec_has_encaps(const struct sec_chk_cred *cred, const char *key)
{
	bool ret = false;
	struct sec_cred_encaps *encaps_cred = NULL;
	struct base_cred *b_cred = NULL;

	if (cred == NULL || cred->base_cred == NULL) {
		hm_warn("cred is NULL\n");
		return ret;
	}
    b_cred = (struct base_cred *)(cred->base_cred);
	if (b_cred->encaps_idx != 0) {
		encaps_cred = sec_cred_idx_to_ptr(b_cred->encaps_idx);
		if (encaps_cred != NULL) {
			raw_rwlock_rdlock(&encaps_cred->rwlock);
#ifdef ENABLE_PAC_HARD_CODE
			if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
				// local
				hm_pacga_bcred_check(b_cred, SEC_PAC_BASE_ENCAPS, PAC_CHECK_NO_RETRY);
			} else {
				// shared_mem
				hm_pacdb_bcred_check(b_cred, SEC_PAC_BASE_ENCAPS, cred->cred_idx, PAC_CHECK_NO_RETRY);
			}
#endif
			ret = sec_cred_has_encaps(encaps_cred, key);
			raw_rwlock_unlock(&encaps_cred->rwlock);
		}
	}

	return ret;
}
int sec_get_encaps(const struct sec_chk_cred *cred, const char *key, uint32_t *type, uint32_t *value)
{
	int ret = E_HM_NOOBJ;
	struct sec_cred_encaps *encaps_cred = NULL;
	struct base_cred *b_cred = NULL;

	if (cred == NULL || cred->base_cred == NULL) {
		hm_warn("cred is NULL\n");
		return ret;
	}
	b_cred = (struct base_cred *)(cred->base_cred);
	if (b_cred->encaps_idx != 0) {
		encaps_cred = sec_cred_idx_to_ptr(b_cred->encaps_idx);
		if (encaps_cred != NULL) {
			raw_rwlock_rdlock(&encaps_cred->rwlock);
#ifdef ENABLE_PAC_HARD_CODE
			if (cred->local_mem_flag == LOCAL_CRED_FLAG) {
				// local
				hm_pacga_bcred_check(b_cred, SEC_PAC_BASE_ENCAPS, PAC_CHECK_NO_RETRY);
			} else {
				// shared_mem
				hm_pacdb_bcred_check(b_cred, SEC_PAC_BASE_ENCAPS, cred->cred_idx, PAC_CHECK_NO_RETRY);
			}
#endif
			ret = sec_cred_get_encaps(encaps_cred, key, type, value);
			raw_rwlock_unlock(&encaps_cred->rwlock);
		}
	}

	return ret;
}

bool cred_check_encaps_from_process(struct cred_encaps *encaps_cred, const char *key, uint32_t type, uint32_t value)
{
	struct sec_encaps *encaps_list = NULL;
	uint32_t saved_type = 0, saved_value = 0;
	if (encaps_cred == NULL) {
		return false;
	}
	if ((encaps_cred->proc_type != ENCAPS_HAP_PROC && encaps_cred->proc_type != ENCAPS_SYS_PROC) ||
			encaps_cred->count == 0U) {
		return false;
	}
	encaps_list = encaps_cred->encaps_list;
	if (encaps_list == NULL ||
		find_encaps_in_list(encaps_list, encaps_cred->count, key, &saved_type, &saved_value) != E_HM_OK) {
		return false;
	}

	if (saved_type == type && saved_value == value) {
		return true;
	}
	return false;
}

bool cred_has_encaps_from_process(struct cred_encaps *encaps_cred, const char *key)
{
	struct sec_encaps *encaps_list = NULL;
	uint32_t saved_type = 0, saved_value = 0;
	if (encaps_cred == NULL) {
		return false;
	}
	if ((encaps_cred->proc_type != ENCAPS_HAP_PROC && encaps_cred->proc_type != ENCAPS_SYS_PROC) ||
		encaps_cred->count == 0U) {
		return false;
	}
	encaps_list = encaps_cred->encaps_list;
	if (encaps_list == NULL ||
		find_encaps_in_list(encaps_list, encaps_cred->count, key, &saved_type, &saved_value) != E_HM_OK) {
		return false;
	}
	return true;
}

int cred_get_encaps_from_process(struct cred_encaps *encaps_cred, const char *key, uint32_t *type, uint32_t *value)
{
	struct sec_encaps *encaps_list = NULL;
	*type = 0;
	*value = 0;
	if (encaps_cred == NULL) {
		return E_HM_NOOBJ;
	}
	if (encaps_cred->proc_type == ENCAPS_UNKNOWN_PROC ||
		encaps_cred->proc_type == ENCAPS_NO_SET_PROC || encaps_cred->count == 0U) {
		return E_HM_NOOBJ;
	}
	encaps_list = encaps_cred->encaps_list;
	if (encaps_list == NULL || find_encaps_in_list(encaps_list, encaps_cred->count, key, type, value) != E_HM_OK) {
		return E_HM_NOOBJ;
	}
	return E_HM_OK;
}

#endif
