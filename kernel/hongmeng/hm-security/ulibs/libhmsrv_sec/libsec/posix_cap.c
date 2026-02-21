/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Define posix cap security module
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 11 14:14:20 2019
 */
#include "libsec/posix_cap.h"
#include "libsec/server_env.h"

#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#ifdef CONFIG_SEHARMONY
#include <libhmseharmony/avc.h>
#endif
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_security.h>
#include "libsec/dac.h"

const char *g_cap_names[POSIX_CAP_NR] = {
	[POSIX_CAP_CHOWN]		= "CAP_CHOWN",
	[POSIX_CAP_DAC_OVERRIDE]	= "CAP_DAC_OVERRIDE",
	[POSIX_CAP_DAC_READ_SEARCH]	= "CAP_DAC_READ_SEARCH",
	[POSIX_CAP_FOWNER]		= "CAP_FOWNER",
	[POSIX_CAP_FSETID]		= "CAP_FSETID",
	[POSIX_CAP_KILL]		= "CAP_KILL",
	[POSIX_CAP_SETGID]		= "CAP_SETGID",
	[POSIX_CAP_SETUID]		= "CAP_SETUID",
	[POSIX_CAP_SETPCAP]		= "CAP_SETPCAP",
	[POSIX_CAP_LINUX_IMMUTABLE]	= "CAP_LINUX_IMMUTABLE",
	[POSIX_CAP_NET_BIND_SERVICE]	= "CAP_NET_BIND_SERVICE",
	[POSIX_CAP_NET_BROADCAST]	= "CAP_NET_BROADCAST",
	[POSIX_CAP_NET_ADMIN]		= "CAP_NET_ADMIN",
	[POSIX_CAP_NET_RAW]		= "CAP_NET_RAW",
	[POSIX_CAP_IPC_LOCK]		= "CAP_IPC_LOCK",
	[POSIX_CAP_IPC_OWNER]		= "CAP_IPC_OWNER",
	[POSIX_CAP_SYS_MODULE]		= "CAP_SYS_MODULE",
	[POSIX_CAP_SYS_RAWIO]		= "CAP_SYS_RAWIO",
	[POSIX_CAP_SYS_CHROOT]		= "CAP_SYS_CHROOT",
	[POSIX_CAP_SYS_PTRACE]		= "CAP_SYS_PTRACE",
	[POSIX_CAP_SYS_PACCT]		= "CAP_SYS_PACCT",
	[POSIX_CAP_SYS_ADMIN]		= "CAP_SYS_ADMIN",
	[POSIX_CAP_SYS_BOOT]		= "CAP_SYS_BOOT",
	[POSIX_CAP_SYS_NICE]		= "CAP_SYS_NICE",
	[POSIX_CAP_SYS_RESOURCE]	= "CAP_SYS_RESOURCE",
	[POSIX_CAP_SYS_TIME]		= "CAP_SYS_TIME",
	[POSIX_CAP_SYS_TTY_CONFIG]	= "CAP_SYS_TTY_CONFIG",
	[POSIX_CAP_MKNOD]		= "CAP_MKNOD",
	[POSIX_CAP_LEASE]		= "CAP_LEASE",
	[POSIX_CAP_AUDIT_WRITE]		= "CAP_AUDIT_WRITE",
	[POSIX_CAP_AUDIT_CONTROL]	= "CAP_AUDIT_CONTROL",
	[POSIX_CAP_SETFCAP]		= "CAP_SETFCAP",
	[POSIX_CAP_MAC_OVERRIDE]	= "CAP_MAC_OVERRIDE",
	[POSIX_CAP_MAC_ADMIN]		= "CAP_MAC_ADMIN",
	[POSIX_CAP_SYSLOG]		= "CAP_SYSLOG",
	[POSIX_CAP_WAKE_ALARM]		= "CAP_WAKE_ALARM",
	[POSIX_CAP_BLOCK_SUSPEND]	= "CAP_BLOCK_SUSPEND",
	[POSIX_CAP_AUDIT_READ]		= "CAP_AUDIT_READ",
	[POSIX_CAP_PERFMON]		= "CAP_PERFMON",
	[POSIX_CAP_BPF]			= "CAP_BPF",
};
const char *sec_cap_name(unsigned int cap_index)
{
	/* Check index here in function, then we need not check at call place */
	if (cap_index < POSIX_CAP_NR) {
		return g_cap_names[cap_index];
	} else {
		return "CAP_UNKNOW";
	}
}

#ifdef SEC_POSIX_CAP_PERMISSIVE
#define LOG_CACHE_SIZE 4

struct sec_chk_fail_log_posix_cap_cache {
	uint32_t		cnode_idx;
	sht_idx_t		sequence;
	unsigned int		posix_cap;
	struct dlist_node	node;
};

static struct sec_chk_fail_log_posix_cap_cache log_cache[LOG_CACHE_SIZE];
static DLIST_HEAD(log_cache_list);
static struct raw_mutex log_cache_lock;

static inline
bool log_cache_is_same(const struct sec_chk_fail_log_posix_cap_cache *one,
		       const struct sec_chk_fail_log_posix_cap_cache *other)
{
	return one->cnode_idx == other->cnode_idx &&
		one->sequence == other->sequence &&
		one->posix_cap == other->posix_cap;
}

static void log_cache_init(void)
{
	raw_mutex_init(&log_cache_lock);
	/*
	 * Loop termination condition:
	 * Following loop will terminate when i reach LOG_CACHE_SIZE. Since i is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: LOG_CACHE_SIZE is 4 according to posix_cap.c,
	 * so loop condition is reasonable.
	 */
	for (size_t i = 0; i < LOG_CACHE_SIZE; i++)
		dlist_insert(&log_cache_list, &log_cache[i].node);
}
#endif

void sec_chk_fail_log_posix_cap(const struct sec_chk_cred *cred,
				unsigned int posix_cap)
{
	bool need_log = true;

	if (cred == NULL) {
		hm_error("cred is NULL\n");
		need_log = false;
	}

#ifdef SEC_POSIX_CAP_PERMISSIVE
	if (need_log) {
		struct sec_chk_fail_log_posix_cap_cache cur = {
			.cnode_idx = cred->cnode_idx,
			.sequence  = cred->seq,
			.posix_cap = posix_cap,
		};
		struct sec_chk_fail_log_posix_cap_cache *old = NULL;
		struct sec_chk_fail_log_posix_cap_cache *tmp = NULL;

		RAW_MUTEX_GUARD(_, &log_cache_lock);

		dlist_for_each_entry_safe(old, tmp, &log_cache_list,
			struct sec_chk_fail_log_posix_cap_cache, node) {
			if (log_cache_is_same(old, &cur)) {
				dlist_delete(&old->node);
				dlist_insert_head(&log_cache_list, &old->node);
				need_log = false;
				break;
			}
		}
		if (need_log) {
			old = dlist_last_entry(&log_cache_list,
				struct sec_chk_fail_log_posix_cap_cache, node);
			dlist_delete(&old->node);
			*old = cur;
			dlist_insert_head(&log_cache_list, &old->node);
		}
	}
#endif

	if (need_log) {
		if (hm_sec_debug_is_enabled() == false) {
			return;
		}
		sec_deny_log_info_init(log_info, NULL, NULL,
				       "capability denied.(process=0x%x, need cap=%u(%s))\n",
				       false, true);
		sec_deny_log_debug(&log_info, cred->cnode_idx, posix_cap,
				   sec_cap_name(posix_cap));
	}
	return;
}

#ifdef CONFIG_USER_NS
bool check_ns_posix_cap(const struct sec_chk_cred *cred, posix_cap_t eff,
	struct usr_ns_cred *targ_usr_ns_cred, unsigned int posix_cap)
{
	if (cred == NULL) {
		hm_error("invalid cred is NULL\n");
		return false;
	}

	struct usr_ns_cred *usrns_cred = targ_usr_ns_cred;
	if (usrns_cred == NULL) {
		hm_error("invalid target cred is NULL\n");
		return false;
	}

	/* See if cred has the capability in the target user namespace
	 * by examining the target user namespace and all of the target
	 * user namespace's parents.
	 */
	for (;;) {
		/* Do we have the necessary capabilities? */
		if (usrns_cred == usr_ns_cred_of_sec_cred(cred)) {
			return posix_cap_added(eff, posix_cap) ? true : false;
		}

		/*
		 * If we're already at a lower level than we're looking for,
		 * we're done searching.
		 */
		if (usrns_cred->level <= usr_ns_cred_of_sec_cred(cred)->level) {
			return false;
		}

		/*
		 * The owner of the user namespace in the parent of the
		 * user namespace has all caps.
		 */
		if ((usrns_cred->parent == usr_ns_cred_of_sec_cred(cred)) &&
		    (usrns_cred->owner == dac_cred_of_sec_cred(cred)->euid)) {
			return true;
		}

		/*
		 * If you have a capability in a parent user usrns_cred, then you have
		 * it over all children user namespaces as well.
		 */
		usrns_cred = usrns_cred->parent;
	}

	return false;
}
#endif

#ifdef CONFIG_SEHARMONY
static int default_sel_cap_check(const struct sec_chk_cred *cred, hmsel_sid_t sid, int cap
	, unsigned int opts, bool initns)
{
	UNUSED(cred, sid, cap, opts, initns);
	return E_HM_OK;
}
static cap_check_func g_sel_cap_check __ro_after_init = default_sel_cap_check;
void sec_set_sel_cap_check_func(cap_check_func fun)
{
	g_sel_cap_check = fun;
}
#else
void sec_set_sel_cap_check_func(cap_check_func fun)
{
	(void)fun;
}
#endif

#ifdef CONFIG_USER_NS
bool sec_ns_capable(const struct sec_chk_cred *cred, struct usr_ns_cred *targ_usr_ns_cred, unsigned int posix_cap)
{
	if (cred == NULL || caps_cred_of_sec_cred(cred) == NULL)
		return false;

	bool res = check_ns_posix_cap(cred, caps_cred_of_sec_cred(cred)->eff_cap, targ_usr_ns_cred, posix_cap);
	if (!res) {
		sec_chk_fail_log_posix_cap(cred, posix_cap);
	}
#ifdef SEC_POSIX_CAP_PERMISSIVE
	res = true;
#endif
#ifndef CONFIG_SEHARMONY
	return res;
#else
	struct sec_cred_seharmony *sel_cred = sel_cred_of_sec_cred(cred);
	if (!res || sel_cred == NULL)
		return false;

	return (g_sel_cap_check(cred, sel_cred->sid, posix_cap, HM_CAP_OPT_NONE,
				targ_usr_ns_cred == sec_get_root_usr_ns_cred()) == E_HM_OK);
#endif
}
#endif

static inline bool sec_do_capable_opts(const struct sec_chk_cred *cred, posix_cap_t eff, unsigned int cap
	, unsigned int opts)
{
#ifndef CONFIG_SEHARMONY
	UNUSED(opts);
	return sec_check_posix_cap(cred, eff, cap);
#else
	struct sec_cred_seharmony *sel_cred = sel_cred_of_sec_cred(cred);
	return sec_check_posix_cap(cred, eff, cap) && (g_sel_cap_check(cred, sel_cred->sid, cap, opts, true) == E_HM_OK);
#endif
}

static struct usr_ns_cred *g_root_usr_ns_cred = NULL;
struct usr_ns_cred *get_root_usr_ns_cred(void)
{
	return g_root_usr_ns_cred;
}

void set_root_usr_ns_cred(struct usr_ns_cred *usr_ns_cred)
{
	g_root_usr_ns_cred = usr_ns_cred;
}

cred_idx_t get_root_usr_ns_idx_by_shconf(void)
{
	if (hm_sec_shconf_init() != E_HM_OK) {
		hm_error("hm_sec_shconf_init failed\n");
		return (cred_idx_t)____COOBJ_INVALID_POS;
	}
	return (cred_idx_t)(*hm_sec_shconf_root_usr_ns_idx());
}

bool sec_do_capable(const struct sec_chk_cred *cred, posix_cap_t eff, unsigned int cap)
{
	return sec_do_capable_opts(cred, eff, cap, HM_CAP_OPT_NONE);
}
bool sec_do_capable_noaudit(const struct sec_chk_cred *cred, posix_cap_t eff, unsigned int cap)
{
	return sec_do_capable_opts(cred, eff, cap, HM_CAP_OPT_NOAUDIT);
}

bool sec_capable(const struct sec_chk_cred *cred, unsigned int cap)
{
	struct sec_cred_caps *posix_cap;
	posix_cap = caps_cred_of_sec_cred(cred);
	return sec_do_capable(cred, posix_cap->eff_cap, cap);
}
bool sec_capable_noaudit(const struct sec_chk_cred *cred, unsigned int cap)
{
	struct sec_cred_caps *posix_cap;
	posix_cap = caps_cred_of_sec_cred(cred);
	return sec_do_capable_noaudit(cred, posix_cap->eff_cap, cap);
}
bool cap_capable(const struct sec_chk_cred *cred, unsigned int cap)
{
	struct sec_cred_caps *posix_cap;
	posix_cap = caps_cred_of_sec_cred(cred);
	return sec_check_posix_cap(cred, posix_cap->eff_cap, cap);
}

/*
 * Check the permission of posix capability when override dac access
 *
 * @params cred [I] Security credential of the process
 *
 * @params obj_mode [I] Mode of the object that will be accessed
 *
 * @params acc_mode [I] Mode of access rights will be checked
 *
 * @return Returns E_HM_OK on success. On any error, returns an errno.
 */
int sec_chk_posix_cap_overide_dac(const struct sec_chk_cred *cred,
				  mode_t obj_mode, unsigned short acc_mode)
{
	int rc = E_HM_ACCES;
	struct sec_cred_caps *posix_cap;

	posix_cap = caps_cred_of_sec_cred(cred);
	acc_mode &= SEC_CAN_RWX;
	if (acc_mode == SEC_CAN_READ) {
		if (sec_check_posix_cap(cred, posix_cap->eff_cap,
					POSIX_CAP_DAC_READ_SEARCH)) {
			rc = E_HM_OK;
		}
	}

	if ((rc != E_HM_OK) &&
		(((acc_mode & SEC_CAN_EXEC) == 0U) ||
			((obj_mode & (mode_t)((unsigned int)S_IXUSR | (unsigned int)S_IXGRP | (unsigned int)S_IXOTH)) != 0U))) {
		if (sec_check_posix_cap(cred, posix_cap->eff_cap,
					POSIX_CAP_DAC_OVERRIDE)) {
			rc = E_HM_OK;
		}
	}
	return rc;
}

static int posix_cap_init(const struct sec_server_env *env)
{
	UNUSED(env);
#ifdef SEC_POSIX_CAP_PERMISSIVE
	log_cache_init();
#endif
	return E_HM_OK;
}

struct sec_mod sec_mod_posix_cap = {
	.name = "posix_cap",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = posix_cap_init,
		.fini = NULL,
	},
};
