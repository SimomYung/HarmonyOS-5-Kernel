/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The seccomp uses the co-object-related encapsulation interface.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 23 16:25:00 2023
 */

/* libs */
#include <string.h>

/* hongmeng ulibs */
#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_coobj.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmseccomp/seccomp.h>

static uintptr_t g_filter_ator;
static bool initialized = false;
static struct raw_mutex filter_ator_mutex = RAW_MUTEX_INITIALIZER;

static int __sec_cofilter_init(bool is_holder)
{
	int ret = E_HM_OK;

	if (is_holder) {
		ret = coobj_init("seccomp", &g_filter_ator, COOBJ_ROLE_HOLDER, false, S_IRUSR);
	} else {
		ret = coobj_init("seccomp", &g_filter_ator, COOBJ_ROLE_RENTER, false, S_IRUSR);
	}

	return ret;
}

int sec_cofilter_init(bool is_holder)
{
	int ret = E_HM_OK;

	if (!initialized) {
		raw_mutex_lock(&filter_ator_mutex);
		if (!initialized) {
			ret = __sec_cofilter_init(is_holder);
			if(ret == E_HM_OK) {
				initialized = true;
			}
		}
		raw_mutex_unlock(&filter_ator_mutex);
	}

	return ret;
}

int sec_filter_alloc(size_t sz, cofilter_idx *idx)
{
	cofilter_idx r_idx;

	r_idx = coobj_malloc(g_filter_ator, sz);
	if (r_idx != (cofilter_idx)____COOBJ_INVALID_POS) {
		*idx = r_idx;
		return E_HM_OK;
	} else {
		hm_error("co-filter alloc failed, obj size %zu\n", sz);
		return E_HM_NOMEM;
	}
}

void sec_filter_free(cofilter_idx idx)
{
	coobj_free(g_filter_ator, idx);
}

void *sec_filter_idx_to_ptr(cofilter_idx idx)
{
	return coobj_idx_to_ptr(g_filter_ator, idx);
}
