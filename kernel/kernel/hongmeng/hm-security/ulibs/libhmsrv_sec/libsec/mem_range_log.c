/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation of mem range logging
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 25 11:27:14 2022
 */
#include <libsec/utility.h>
#include <libsec/mem_range_log.h>

#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libsec/module.h>
#include <libsec/log.h>
#include <securec.h>

#ifdef SEC_PLC_PERMISSIVE_CACHE
#define LOG_CACHE_SIZE 128u

struct mem_range_fail_log_cache {
	mrange_t mrange;
	uint64_t subject;
	struct dlist_node node;
};

static struct mem_range_fail_log_cache *log_cache;

static DLIST_HEAD(log_cache_list);

static struct raw_mutex log_cache_lock;

static inline bool log_cache_is_same(const struct mem_range_fail_log_cache *one,
				     const struct mem_range_fail_log_cache *other)
{
	return one->mrange.start == other->mrange.start &&
		one->mrange.end == other->mrange.end &&
		one->subject == other->subject;
}

static int log_cache_init(void)
{
	int rc = E_HM_OK;

	raw_mutex_init(&log_cache_lock);
	log_cache = calloc(LOG_CACHE_SIZE, sizeof(struct mem_range_fail_log_cache));
	if (log_cache == NULL) {
		hm_error("failed to allocate space for mem range fail log cache\n");
		rc = E_HM_NOMEM;
	}
	if (rc == E_HM_OK) {
		/*
		 * Loop termination condition:
		 * Following loop will terminate when i reach LOG_CACHE_SIZE. Since i is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: LOG_CACHE_SIZE is 128 according to this file,
		 * so loop condition is reasonable.
		 */
		for (size_t i = 0; i < LOG_CACHE_SIZE; i++) {
			dlist_insert(&log_cache_list, &log_cache[i].node);
		}
	}
	return rc;
}

static void log_cache_fini(void)
{
	free(log_cache);
	log_cache = NULL;
	dlist_init(&log_cache_list);
}

static int pmsv_cache_add(uint64_t subj, const mrange_t *mrange)
{
	int rc = E_HM_OK;
	struct mem_range_fail_log_cache cur = {
		.mrange		= *mrange,
		.subject	= subj,
		.node		= {NULL, NULL},
	};
	struct mem_range_fail_log_cache *old = NULL;
	struct mem_range_fail_log_cache *tmp = NULL;

	RAW_MUTEX_GUARD_START(_, &log_cache_lock);

	dlist_for_each_entry_safe(old, tmp, &log_cache_list,
				  struct mem_range_fail_log_cache,
				  node) {
		if (log_cache_is_same(old, &cur)) {
			dlist_delete(&old->node);
			dlist_insert_head(&log_cache_list, &old->node);
			rc = E_HM_OBJEXIST;
			break;
		}
	}

	if (rc == E_HM_OK) {
		old = dlist_last_entry(&log_cache_list, struct mem_range_fail_log_cache,
				       node);
		dlist_delete(&old->node);
		*old = cur;
		dlist_insert_head(&log_cache_list, &old->node);
	}

	RAW_MUTEX_GUARD_END(_, &log_cache_lock);
	return rc;
}
#endif

#ifdef CONFIG_AUDIT
static void audit_log_mem_range(const struct sec_chk_cred *cred, uint64_t subj,
				const mrange_t *mrange)
{
	int ret = E_HM_OK;
	char *exe = NULL;
	char *comm = NULL;
	pid_t pid;

	ret = sec_log_read_pid_node(cred, &pid, "exe", &exe);
	if (ret == E_HM_OK) {
		comm = strrchr(exe, (int)'/');
		sec_deny_log_hook(" avc: denied { mem range [%"PRIx64"-%"PRIx64"] } "
				  "for pid=%d comm=\"%s\" scontext=%s tcontext=null tclass=null\n",
				  mrange->start, mrange->end,
				  pid, comm == NULL ? exe : comm + 1,
				  sec_plc_get_type_name(subj));
		free(exe);
	} else {
		hm_error("failed to read comm of process. (process=0x%x, ret=%s)\n",
			 cred->cnode_idx, hmstrerror(ret));
	}
}
#endif

void sec_mem_range_chk_fail_log(const struct sec_chk_cred *cred, const struct sec_deny_log_bundle *log)
{
	int rc = E_HM_OK;

	if (cred == NULL) {
		hm_error("cred is NULL\n");
		rc = E_HM_INVAL;
	}

	uint64_t subj = log->check_info.subj;
	const mrange_t *mrange = &(log->check_info.mrange);
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (rc == E_HM_OK) {
		if (sec_plc_mode_is_pmsv_cache()) {
			rc = pmsv_cache_add(subj, mrange);
		}
	}
#endif

	if (rc == E_HM_OK) {
		struct sec_log_extra_info *info = log->obj_info;
		if (info != NULL && info->func != NULL) {
			info->func(cred, info->arg);
		}
		sec_deny_log("mem range denied. (process=0x%x, %s(0x%"PRIx64"), mem range=[%"PRIx64"-%"PRIx64"])\n",
			     cred->cnode_idx, sec_plc_get_type_name(subj), subj,
			     mrange->start, mrange->end);
#ifdef CONFIG_AUDIT
		audit_log_mem_range(cred, subj, mrange);
#endif
	}
	return;
}

static int mem_range_init(const struct sec_server_env *env)
{
	UNUSED(env);
	int ret = E_HM_OK;
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (sec_plc_mode_is_pmsv_cache()) {
		ret = log_cache_init();
		if (ret != E_HM_OK) {
			log_cache_fini();
		}
	}
#endif
	return ret;
}

static void mem_range_fini(void)
{
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (sec_plc_mode_is_pmsv_cache()) {
		log_cache_fini();
	}
#endif
}

struct sec_mod sec_mod_mem_range = {
	.name = "mem_range",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = mem_range_init,
		.fini = mem_range_fini,
	},
};
