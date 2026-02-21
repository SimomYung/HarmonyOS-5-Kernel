/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of ability function
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:14 2019
 */
#include "libsec/ability.h"
#include "libsec/utility.h"
#include "libsec/server_env.h"
#include "libsec/cred.h"

#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex_guard.h>

#ifdef SEC_PLC_PERMISSIVE_CACHE
#define LOG_CACHE_SIZE 128u
#define LOG_CACHE_EXPABILITIES_SIZE 64u

struct sec_chk_fail_log_ability_cache {
	int			ability;
	uint64_t		subject;
	struct dlist_node	node;
};

struct sec_chk_fail_log_expabilities_cache {
	ability_t		ablty;
	uint64_t		subject;
	struct dlist_node	node;
};

static struct sec_chk_fail_log_ability_cache *log_cache;
static struct sec_chk_fail_log_expabilities_cache *log_expablty_cache;

static DLIST_HEAD(log_cache_list);
static DLIST_HEAD(log_expablty_cache_list);

static struct raw_mutex log_cache_lock;
static struct raw_mutex log_expablty_cache_lock;

static inline bool log_cache_is_same(const struct sec_chk_fail_log_ability_cache *one,
				     const struct sec_chk_fail_log_ability_cache *other)
{
	return one->ability == other->ability &&
		one->subject == other->subject;
}

static bool log_expablty_cache_is_same(
	const struct sec_chk_fail_log_expabilities_cache *one,
	const struct sec_chk_fail_log_expabilities_cache *other)
{
	uint32_t i;
	bool is_same = true;

	if (one->subject != other->subject) {
		is_same = false;
	} else {
		/*
		 * Loop termination condition:
		 * Following loop will terminate when i reach ABILITY_WIDTH. Since i is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: ABILITY_WIDTH is 2 according to libsecplc/policy_fmt.h,
		 * so loop condition is reasonable.
		 */
		for (i = 0; i < ABILITY_WIDTH; i++) {
			if (one->ablty.ability[i] != other->ablty.ability[i]) {
				is_same = false;
				break;
			}
		}
	}
	return is_same;
}

static int log_cache_init(void)
{
	int rc = E_HM_OK;

	raw_mutex_init(&log_cache_lock);
	log_cache = malloc(sizeof(struct sec_chk_fail_log_ability_cache) * LOG_CACHE_SIZE);
	if (log_cache == NULL) {
		hm_error("failed to malloc ability log cache\n");
		rc = E_HM_NOMEM;
	}
	if (rc == E_HM_OK) {
		/*
		 * Loop termination condition:
		 * Following loop will terminate when i reach LOG_CACHE_SIZE. Since i is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: LOG_CACHE_SIZE is 128 according to ability.c,
		 * so loop condition is reasonable.
		 */
		for (size_t i = 0; i < LOG_CACHE_SIZE; i++) {
			dlist_insert(&log_cache_list, &log_cache[i].node);
		}
	}
	return rc;
}

static int log_expablty_cache_init(void)
{
	int rc = E_HM_OK;
	raw_mutex_init(&log_expablty_cache_lock);
	log_expablty_cache =
		malloc(sizeof(struct sec_chk_fail_log_expabilities_cache) * LOG_CACHE_EXPABILITIES_SIZE);
	if (log_expablty_cache == NULL) {
		hm_error("failed to malloc exp ability log cache\n");
		rc = E_HM_NOMEM;
	}

	if (rc == E_HM_OK) {
		/*
		 * Loop termination condition:
		 * Following loop will terminate when i reach LOG_CACHE_EXPABILITIES_SIZE. Since i is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: LOG_CACHE_EXPABILITIES_SIZE is 64 according to ability.c,
		 * so loop condition is reasonable.
		 */
		for (size_t i = 0; i < LOG_CACHE_EXPABILITIES_SIZE; i++) {
			dlist_insert(&log_expablty_cache_list, &log_expablty_cache[i].node);
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

static void log_expablty_cache_fini(void)
{
	free(log_expablty_cache);
	log_expablty_cache = NULL;
	dlist_init(&log_expablty_cache_list);
}

#endif

#ifdef SEC_PLC_PERMISSIVE_CACHE
static int pmsv_cache_add(uint64_t subj, int ability)
{
	int rc = E_HM_OK;
	struct sec_chk_fail_log_ability_cache cur = {
		.ability   = ability,
		.subject   = subj,
		.node      = {NULL, NULL},
	};
	struct sec_chk_fail_log_ability_cache *old = NULL;
	struct sec_chk_fail_log_ability_cache *tmp = NULL;

	RAW_MUTEX_GUARD_START(_, &log_cache_lock);

	dlist_for_each_entry_safe(old, tmp, &log_cache_list,
				  struct sec_chk_fail_log_ability_cache,
				  node) {
		if (log_cache_is_same(old, &cur)) {
			dlist_delete(&old->node);
			dlist_insert_head(&log_cache_list, &old->node);
			rc = E_HM_OBJEXIST;
			break;
		}
	}

	if (rc == E_HM_OK) {
		old = dlist_last_entry(&log_cache_list, struct sec_chk_fail_log_ability_cache,
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
static void audit_log_ability(const struct sec_chk_cred *cred, uint64_t subj,
			      enum ability_def ability)
{
	int ret;
	char *exe = NULL;
	char *comm = NULL;
	pid_t pid;

	ret = sec_log_read_pid_node(cred, &pid, "exe", &exe);
	if (ret == E_HM_OK) {
		comm = strrchr(exe, (int)'/');
		sec_deny_log_hook(" avc: denied { %s } for pid=%d comm=\"%s\" scontext=%s tcontext=null tclass=null\n",
				  sec_plc_get_ability_name((uint64_t)(unsigned)ability),
				  pid, comm == NULL ? exe : comm + 1,
				  sec_plc_get_type_name(subj));
		free(exe);
	} else {
		hm_error("failed to read comm of process. (process=0x%x, ret=%s)\n",
			 cred->cnode_idx, hmstrerror(ret));
	}
}
#endif

void sec_chk_fail_log_ability(const struct sec_chk_cred *cred, uint64_t subj,
			      enum ability_def ability, struct sec_log_extra_info *info)
{
	int rc = E_HM_OK;

	if (cred == NULL) {
		hm_error("cred is NULL\n");
		rc = E_HM_INVAL;
	}

#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (rc == E_HM_OK) {
		if (sec_plc_mode_is_pmsv_cache()) {
			rc = pmsv_cache_add(subj, (int)ability);
		}
	}
#endif

	if (rc == E_HM_OK) {
		if (info != NULL && info->func != NULL) {
			info->func(cred, info->arg);
		}
		sec_deny_log("ability denied. (process=0x%x, %s(0x%"PRIx64"), ability=%s(0x%x)%s)\n",
			     cred->cnode_idx, sec_plc_get_type_name(subj), subj,
			     sec_plc_get_ability_name((uint64_t)(unsigned)ability),
			     (unsigned int)ability, sec_plc_is_internal_ab(ability) ? "is internal" : "");
#ifdef CONFIG_AUDIT
		audit_log_ability(cred, subj, ability);
#endif
	}
	return;
}

#ifdef SEC_PLC_PERMISSIVE_CACHE
static int pmsv_log_expabilities_cache_add(uint64_t subj, ability_t exp_ablty)
{
	int rc = E_HM_OK;
	struct sec_chk_fail_log_expabilities_cache cur = {
		.ablty     = exp_ablty,
		.subject   = subj,
		.node      = {NULL, NULL},
	};
	struct sec_chk_fail_log_expabilities_cache *old = NULL;
	struct sec_chk_fail_log_expabilities_cache *tmp = NULL;

	RAW_MUTEX_GUARD_START(_, &log_expablty_cache_lock);

	dlist_for_each_entry_safe(old, tmp, &log_expablty_cache_list,
		struct sec_chk_fail_log_expabilities_cache, node) {
		if (log_expablty_cache_is_same(old, &cur)) {
			dlist_delete(&old->node);
			dlist_insert_head(&log_expablty_cache_list, &old->node);
			rc = E_HM_OBJEXIST;
			break;
		}
	}

	if (rc == E_HM_OK) {
		old = dlist_last_entry(&log_expablty_cache_list,
			struct sec_chk_fail_log_expabilities_cache, node);
		dlist_delete(&old->node);
		*old = cur;
		dlist_insert_head(&log_expablty_cache_list, &old->node);
	}

	RAW_MUTEX_GUARD_END(_, &log_expablty_cache_lock);
	return rc;
}
#endif

/*
 * print all the abilities that subj lacks in detail
 */
static void print_missed_abilities(uint64_t subj, const ability_t *missed_ab)
{
	uint32_t i;
	uint64_t cur_idx = 0;
	uint64_t one_idx = ABILITY_BITS_PER_ELEMENT;
	const char* subj_name = sec_plc_get_type_name(subj);
	sec_deny_log("abilities denied. (sub_type=%s(0x%"PRIx64"))\n",
		     subj_name, subj);
	/*
	 * Loop termination condition:
	 * Following loop will terminate when i reach ABILITY_WIDTH. Since i is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: ABILITY_WIDTH is 2 according to libsecplc/policy_fmt.h,
	 * so loop condition is reasonable.
	 */
	for (i = 0; i < ABILITY_WIDTH; i++) {
		one_idx = raw_bitmap_find_first_one(ptr_to_type(&missed_ab->ability[i], unsigned long long*),
						    (unsigned int)ABILITY_BITS_PER_ELEMENT);
		while (one_idx < ABILITY_BITS_PER_ELEMENT) {
			cur_idx = one_idx + ABILITY_BITS_PER_ELEMENT * i;
			sec_deny_log("%s(0x%"PRIx64") lacks ability %s(0x%"PRIx64"), property=%s\n", subj_name, subj,
				     sec_plc_get_ability_name(cur_idx), cur_idx,
				     sec_plc_is_internal_ab((enum ability_def)cur_idx) ? "internal" : "external");
			one_idx = raw_bitmap_find_next_one(
					ptr_to_type(&missed_ab->ability[i], unsigned long long*),
					(unsigned int)ABILITY_BITS_PER_ELEMENT, (unsigned int)one_idx);
		}
	}
}

static void sec_chk_fail_log_chk_exp_ablty(
	const struct sec_chk_cred *cred, uint64_t subj, const ability_t *missed_ab,
	struct sec_log_extra_info *info)
{
	int rc = E_HM_OK;
	if (cred == NULL) {
		hm_error("cred is NULL\n");
		rc = E_HM_INVAL;
	}
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (rc == E_HM_OK) {
		if (sec_plc_mode_is_pmsv_cache()) {
			rc = pmsv_log_expabilities_cache_add(subj, *missed_ab);
		}
	}
#endif
	if (rc == E_HM_OK) {
		if (info != NULL && info->func != NULL) {
			info->func(cred, info->arg);
		}
		print_missed_abilities(subj, missed_ab);
	}
	return;
}

static void _merge_avail_abilities(ability_t *avail, ability_t *internal, ability_t *externel,
				   const ability_t *internal_map)
{
	uint64_t i = 0;

	for (i = 0; i < ABILITY_WIDTH; i++) {
		internal->ability[i] &= internal_map->ability[i];
	}

	for (i = 0; i < ABILITY_WIDTH; i++) {
		externel->ability[i] &= ~(internal_map->ability[i]);
	}

	for (i = 0; i < ABILITY_WIDTH; i++) {
		avail->ability[i] |= internal->ability[i];
		avail->ability[i] |= externel->ability[i];
	}
}

int sec_chk_do_export_abilities(const struct sec_chk_cred *cred, ability_t *avail_ablty)
{
	int ret = E_HM_OK;
	ability_t internal_map = {0};
	ability_t internal_avail_ablty = {0};
	ability_t externel_avail_ablty = {0};

	if (avail_ablty == NULL) {
		hm_error("avail_ablty is NULL\n");
		ret = E_HM_INVAL;
	}

#ifdef CONFIG_INNER_MAC
	/* inner sid's abilities: if inner sid is invalid, internal_avail_ablty allows to be empty */
	if (ret == E_HM_OK) {
		/* invalid sid will return empty avail ablty */
		ret = sec_plc_get_avail_abilities(inner_sid_of_sec_cred(cred), &internal_avail_ablty);
	}
#endif

	if (ret == E_HM_OK) {
		/*  mac sid's abilities */
		ret = sec_plc_get_avail_abilities(sid_of_sec_cred(cred), &externel_avail_ablty);
	}

	if (ret == E_HM_OK) {
		/* permissive or disable mode, externel part will escalate privileges. */
		if (sec_plc_mode() != SEC_PLC_ENFORCE) {
			NOFAIL(memset_s(&externel_avail_ablty, sizeof(ability_t), 0xFF, sizeof(ability_t)));
		}
		ret = sec_plc_get_internal_abs(&internal_map);
		if (ret != E_HM_OK) {
			hm_error("failed to get internal abilities map, ret=%s\n", hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		/* merge internal and external abilities to avail_ablty */
		_merge_avail_abilities(avail_ablty, &internal_avail_ablty, &externel_avail_ablty, &internal_map);
	}

	return ret;
}

static bool _is_not_empty_abilities(const ability_t *ably)
{
	bool ret = false;
	uint64_t i = 0;
	for (i = 0; i < ABILITY_WIDTH; i++) {
		if (ably->ability[i] != 0u) {
			ret = true;
			break;
		}
	}
	return ret;
}

static void _split_expect_abilities(const ability_t *expect,
	ability_t *internal, ability_t *externel, const ability_t *internal_map)
{
	uint64_t i = 0;

	for (i = 0; i < ABILITY_WIDTH; i++) {
		internal->ability[i] = expect->ability[i] & internal_map->ability[i];
	}

	for (i = 0; i < ABILITY_WIDTH; i++) {
		externel->ability[i] = expect->ability[i] & ~(internal_map->ability[i]);
	}
}

static int _chk_abilities_by_subj(const struct sec_chk_cred *cred,
	uint64_t subj, ability_t *exp_ab, sec_chk_show_extra show_proc, bool is_inner)
{
	int ret = E_HM_OK;
	ability_t avail_ab = {0};

	ret = sec_plc_check_abilities(subj, exp_ab, &avail_ab);
	if (ret != E_HM_OK) {
		ability_t missed_ab = {0};
		sec_calc_missed_abilities(exp_ab, &avail_ab, &missed_ab);
		sec_log_extra_info_init(extra_info, show_proc, NULL);
		/* try to log on disable mode */
		if (!sec_plc_mode_is_disable()) {
			sec_chk_fail_log_chk_exp_ablty(cred, subj, &missed_ab, &extra_info);
		}
		/* not inner case: recorrect result on disable/permissive mode */
		if (!is_inner && (sec_plc_mode_is_disable() || sec_plc_mode_is_permissive())) {
			ret = E_HM_OK;
		}
	}

	return ret;
}

#define INTERNAL_CHK true
#define EXTERNAL_CHK false

// split expect ability automatically
int sec_chk_do_check_abilities(const struct sec_chk_cred *cred, const ability_t *exp_ablty,
			       sec_chk_show_extra show_proc)
{
	int ret = E_HM_OK;
	ability_t internal_map = {0};
	ability_t internal_exp_ablty = {0};
	ability_t externel_exp_ablty = {0};

	if (exp_ablty == NULL) {
		hm_error("exp_ablty is NULL\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = sec_plc_get_internal_abs(&internal_map);
		if (ret != E_HM_OK) {
			hm_error("failed to get internal abilities map, ret=%s\n", hmstrerror(ret));
		}
	}

	/* split expect abilities */
	if (ret == E_HM_OK) {
		_split_expect_abilities(exp_ablty, &internal_exp_ablty, &externel_exp_ablty, &internal_map);
	}

#ifdef CONFIG_INNER_MAC
	/* expect internal abilities check without privileges escalation. */
	if (ret == E_HM_OK && _is_not_empty_abilities(&internal_exp_ablty)) {
		ret = _chk_abilities_by_subj(cred, mac_cred_of_sec_cred(cred)->inner_sid,
					     &internal_exp_ablty, show_proc, INTERNAL_CHK);
	}
#endif
	/* expect externel abilities, allow to elevate privileges on permissive/disable mode */
	if (ret == E_HM_OK && _is_not_empty_abilities(&externel_exp_ablty)) {
		ret = _chk_abilities_by_subj(cred, mac_cred_of_sec_cred(cred)->sid,
					     &externel_exp_ablty, show_proc, EXTERNAL_CHK);
	}

	return ret;
}

/*
 * accepts missed abilities in bitmap format and reports the event if enabled
 */
int sec_check_abilities_failed(const struct sec_chk_cred *cred,
			       const struct sec_cred_mac *mac_cred,
			       const ability_t *missed_ab,
			       sec_chk_show_extra show_proc)
{
	int rc = E_HM_OK;

	struct sec_log_extra_info info = {NULL, NULL};
	if (!sec_plc_mode_is_disable()) {
		info.func = show_proc;
		info.arg = NULL;
		sec_chk_fail_log_chk_exp_ablty(cred, mac_cred->sid, missed_ab, &info);
		if (!sec_plc_mode_is_permissive()) {
			rc = E_HM_ACCES;
		}
	}
	return rc;
}

bool sec_check_ability_extra_failed(const struct sec_chk_cred *cred,
				    uint64_t subj, enum ability_def ability,
				    sec_chk_show_extra func, void *arg)
{
	struct sec_log_extra_info info = {NULL, NULL};
	bool ret = true;
	if (!sec_plc_mode_is_disable()) {
		info.func = func;
		info.arg = arg;
		sec_chk_fail_log_ability(cred, subj, ability, &info);
		if (!sec_plc_mode_is_permissive()) {
			ret = false;
		}
	}

	return ret;
}

int sec_check_ability_post_failed(const struct sec_chk_cred *cred,
				  const struct sec_cred_mac *mac_cred,
				  enum ability_def ability,
				  sec_chk_show_extra func, void *arg)
{
	int ret = E_HM_OK;
	bool is_inner_mac = false;
	sec_log_extra_info_init(info, func, arg);

	if (mac_cred == NULL) {
		hm_error("mac_cred is NULL\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		/* check failed return E_HM_ACCES on default */
		ret = E_HM_ACCES;

		/* case of internal ability doesn't fixup the result */
		is_inner_mac = sec_plc_is_internal_ab(ability);

		/* disable mode doesn't generate log, permissive&cache mode will suppress generating logs too fast.
		 * but for inner mac, it always generates access deny log.
		 */
		if (!sec_plc_mode_is_disable() || is_inner_mac) {
			sec_chk_fail_log_ability(cred, is_inner_mac ? mac_cred->inner_sid : mac_cred->sid,
						 ability, &info);
		}

		/* disable or permissive mode recorrect result to OK, when ability is not internal */
		if (!is_inner_mac && (sec_plc_mode_is_disable() || sec_plc_mode_is_permissive())) {
			ret = E_HM_OK;
		}
	}

	return ret;
}

static int ability_init(const struct sec_server_env *env)
{
	UNUSED(env);
	int ret = E_HM_OK;
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (sec_plc_mode_is_pmsv_cache()) {
		ret = log_cache_init();
		if (ret == E_HM_OK) {
			ret = log_expablty_cache_init();
			if (ret != E_HM_OK) {
				log_cache_fini();
			}
		}
	}
#endif
	return ret;
}

static void ability_fini(void)
{
#ifdef SEC_PLC_PERMISSIVE_CACHE
	if (sec_plc_mode_is_pmsv_cache()) {
		log_cache_fini();
		log_expablty_cache_fini();
	}
#endif
}
struct sec_mod sec_mod_ability = {
	.name = "ability",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = ability_init,
		.fini = ability_fini,
	},
};
