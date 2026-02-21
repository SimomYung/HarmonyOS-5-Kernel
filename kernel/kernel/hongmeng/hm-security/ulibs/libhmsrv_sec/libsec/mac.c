/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Init the module of mac and provide hints for check failure
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:14 2019
 */
#include "libsec/mac.h"
#include "libsec/utility.h"

#include <stdbool.h>
#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex_guard.h>

#ifdef SEC_PLC_PERMISSIVE_CACHE
#define LOG_CACHE_SIZE 128U
struct sec_chk_fail_log_mac_cache {
	int			action;
	uint64_t		subject;
	uint64_t		object;
	struct dlist_node	node;
};

static struct sec_chk_fail_log_mac_cache *log_cache;
static DLIST_HEAD(log_cache_list);
static struct raw_mutex log_cache_lock;

static inline bool log_cache_is_same(const struct sec_chk_fail_log_mac_cache *one,
				     const struct sec_chk_fail_log_mac_cache *other)
{
	return one->action == other->action &&
		one->subject == other->subject &&
		one->object == other->object;
}

static int log_cache_init(void)
{
	int ret = E_HM_OK;
	raw_mutex_init(&log_cache_lock);
	log_cache = malloc(sizeof(struct sec_chk_fail_log_mac_cache) * LOG_CACHE_SIZE);
	if (log_cache == NULL) {
		hm_error("failed to malloc ac cache\n");
		ret = E_HM_NOMEM;
	} else {
		/*
		 * Loop termination condition:
		 * Following loop will terminate when i reach LOG_CACHE_SIZE. Since i is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: LOG_CACHE_SIZE is 128 according to mac.c,
		 * so loop condition is reasonable.
		 */
		for (size_t i = 0; i < LOG_CACHE_SIZE; i++) {
			dlist_insert(&log_cache_list, &log_cache[i].node);
		}
	}

	return ret;
}

static void log_cache_fini(void)
{
	free(log_cache);
	log_cache = NULL;
	dlist_init(&log_cache_list);
}

static bool sec_chk_add_cache(struct sec_chk_fail_log_mac_cache cur)
{
	bool need_log = true;
	struct sec_chk_fail_log_mac_cache *old = NULL;
	struct sec_chk_fail_log_mac_cache *tmp = NULL;

	RAW_MUTEX_GUARD_START(_, &log_cache_lock);
	dlist_for_each_entry_safe(old, tmp, &log_cache_list,
				  struct sec_chk_fail_log_mac_cache,
				  node) {
		if (log_cache_is_same(old, &cur)) {
			dlist_delete(&old->node);
			dlist_insert_head(&log_cache_list, &old->node);
			need_log = false;
			break;
		}
	}
	if (need_log) {
		old = dlist_last_entry(&log_cache_list,
				       struct sec_chk_fail_log_mac_cache, node);
		dlist_delete(&old->node);
		*old = cur;
		dlist_insert_head(&log_cache_list, &old->node);
	}
	RAW_MUTEX_GUARD_END(_, &log_cache_lock);

	return need_log;
}

static inline bool _chk_add_cache(uint64_t subj, uint64_t obj, int act)
{
	struct sec_chk_fail_log_mac_cache cur = {
		.action    = act,
		.subject   = subj,
		.object    = obj,
		.node      = {NULL, NULL},
	};
	return sec_chk_add_cache(cur);
}

static inline bool
_is_cache_case(uint64_t subj,  uint32_t *p_subj_prop, uint64_t obj, uint32_t *p_obj_prop)
{
	if (sec_plc_mode_is_pmsv_cache()) {
		return true;
	} else if (sec_plc_type_is_cache_subj(subj, p_subj_prop)) {
		return sec_plc_type_is_permissive_subj(subj, p_subj_prop) || sec_plc_mode_is_permissive();
	} else if (sec_plc_type_is_cache_obj(obj, p_obj_prop)) {
		return sec_plc_type_is_permissive_obj(obj, p_obj_prop) || sec_plc_mode_is_permissive();
	}
	return false;
}
#endif

#ifdef CONFIG_AUDIT
static void audit_log_mac(const struct sec_chk_cred *cred, uint64_t subj,
			  uint64_t obj, int act)
{
	int ret;
	pid_t pid;
	char *exe = NULL;
	char *comm = NULL;
	uint32_t subj_prop = INVALID_PROPERTY;
	uint32_t obj_prop = INVALID_PROPERTY;

	ret = sec_log_read_pid_node(cred, &pid, "exe", &exe);
	if (ret == E_HM_OK) {
		comm = strrchr(exe, (int)'/');
		sec_deny_log_hook(" avc: denied { %s } for pid=%d comm=\"%s\" scontext=%s tcontext=%s tclass=null"
				  " permissive=(subj : %u, obj : %u)"
#ifdef SEC_PLC_PERMISSIVE_CACHE
				  " cache=(subj : %u, obj : %u)"
#endif
				  "\n",
				  sec_plc_get_action_name((uint64_t)(unsigned)act),
				  pid, comm == NULL ? exe : comm + 1,
				  sec_plc_get_type_name(subj),
				  sec_plc_get_type_name(obj),
				  sec_plc_type_is_permissive_subj(subj, &subj_prop),
				  sec_plc_type_is_permissive_obj(obj, &obj_prop)
#ifdef SEC_PLC_PERMISSIVE_CACHE
				  ,sec_plc_type_is_cache_subj(subj, &subj_prop)
				  ,sec_plc_type_is_cache_obj(obj, &obj_prop)
#endif
				  );
		free(exe);
	} else {
		hm_error("failed to read comm of process. (process=0x%x, ret=%s)\n",
			 cred->cnode_idx, hmstrerror(ret));
	}
}
#endif

static inline bool
_is_permissive_case(uint64_t subj,  uint32_t *p_subj_prop, uint64_t obj, uint32_t *p_obj_prop)
{
	return sec_plc_mode_is_permissive() ||
	       sec_plc_type_is_permissive_subj(subj, p_subj_prop) || sec_plc_type_is_permissive_obj(obj, p_obj_prop);
}

static inline bool
_is_no_log_case(uint64_t subj,  uint32_t *p_subj_prop, uint64_t obj, uint32_t *p_obj_prop)
{
	if (sec_plc_type_is_no_log_subj(subj, p_subj_prop)) {
		// no-log is only enabled under (partial/global)permissive mode(include global permissive cache mode)
		return sec_plc_type_is_permissive_subj(subj, p_subj_prop) || sec_plc_mode_is_permissive();
	} else if (sec_plc_type_is_no_log_obj(obj, p_obj_prop)) {
		// no-log is only enabled under (partial/global)permissive mode(include global permissive cache mode)
		return sec_plc_type_is_permissive_obj(obj, p_obj_prop) || sec_plc_mode_is_permissive();
	}
	return false;
}

static void sec_chk_fail_log_mac(const struct sec_chk_cred *cred, uint64_t subj,
				 uint64_t obj, int act,
				 struct sec_log_extra_info *info)
{
	bool need_log = true;

	uint32_t subj_prop = INVALID_PROPERTY;
	uint32_t obj_prop = INVALID_PROPERTY;

	if (cred == NULL) {
		hm_error("cred is NULL\n");
		need_log = false;
	}
	need_log = need_log && !_is_no_log_case(subj, &subj_prop, obj, &obj_prop);
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (need_log && _is_cache_case(subj, &subj_prop, obj, &obj_prop)) {
		need_log = _chk_add_cache(subj, obj, act);
	}
#endif
	if (need_log && info != NULL && info->func != NULL) {
		info->func(cred, info->arg);
	}
	if (need_log) {
#ifdef SEC_PLC_PERMISSIVE_CACHE
		sec_deny_log("action denied. (process=0x%x, %s(0x%"PRIx64"), action=%s(0x%x), obj_sid=%s(0x%"PRIx64"),"
			     " permissive=(subj : %d, obj : %d), cache=(subj : %d, obj : %d))\n",
			     cred->cnode_idx, sec_plc_get_type_name(subj), subj,
			     sec_plc_get_action_name((uint64_t)(unsigned)act), (unsigned int)act,
			     sec_plc_get_type_name(obj), obj,
			     sec_plc_type_is_permissive_subj(subj, &subj_prop),
			     sec_plc_type_is_permissive_obj(obj, &obj_prop),
			     sec_plc_type_is_cache_subj(subj, &subj_prop),
			     sec_plc_type_is_cache_obj(subj, &obj_prop));
#else
		sec_deny_log("action denied. (process=0x%x, %s(0x%"PRIx64"), action=%s(0x%x), obj_sid=%s(0x%"PRIx64"),"
			     " permissive=(subj : %d, obj : %d))\n",
			     cred->cnode_idx, sec_plc_get_type_name(subj), subj,
			     sec_plc_get_action_name((uint64_t)(unsigned)act), (unsigned int)act,
			     sec_plc_get_type_name(obj), obj,
			     sec_plc_type_is_permissive_subj(subj, &subj_prop),
			     sec_plc_type_is_permissive_obj(obj, &obj_prop));
#endif
#ifdef CONFIG_AUDIT
		audit_log_mac(cred, subj, obj, act);
#endif
	}
	return;
}

bool sec_check_te_policy_failed(const struct sec_chk_cred *cred, uint64_t subj,
				uint64_t obj, int act)
{
	bool check_pass = true;
	if (!sec_plc_mode_is_disable()) {
		sec_chk_fail_log_mac(cred, subj, obj, act, NULL);
		if (!_is_permissive_case(subj, NULL, obj, NULL)) {
			check_pass = false;
		}
	}

	return check_pass;
}

bool sec_check_te_policy_ex_failed(const struct sec_chk_cred *cred,
				   uint64_t subj, uint64_t obj, int act,
				   struct sec_log_extra_info *info)
{
	bool check_pass = true;
	if (!sec_plc_mode_is_disable()) {
		sec_chk_fail_log_mac(cred, subj, obj, act, info);
		if (!_is_permissive_case(subj, NULL, obj, NULL)) {
			check_pass = false;
		}
	}

	return check_pass;
}

static int mac_init(const struct sec_server_env *env)
{
	UNUSED(env);
	int rc = E_HM_OK;
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (!sec_plc_mode_is_disable()) {
		rc = log_cache_init();
	}
#endif
	return rc;
}

static void mac_fini(void)
{
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (!sec_plc_mode_is_disable()) {
		log_cache_fini();
	}
#endif
}

struct sec_mod sec_mod_mac = {
	.name = "mac",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = mac_init,
		.fini = mac_fini,
	},
};
