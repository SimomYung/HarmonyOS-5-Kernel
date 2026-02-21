/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Procmgr cred interface header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 04 17:11:26 2019
 */
#ifndef SYSMGR_PROCMGR_CRED_H
#define SYSMGR_PROCMGR_CRED_H

#include <stdint.h>

#include <intapi/process/process.h>
#include <intapi/process/proc_prctl.h>
#include <libsec/cred.h>

#define INVAL_UID ((uid_t)-1)
#define INVAL_GID ((gid_t)-1)

int process_getuid(struct process_s *process, uid_t *uid);
int process_geteuid(struct process_s *process, uid_t *euid);
int process_getresuid(struct process_s *process, uid_t *ruid, uid_t *euid, uid_t *suid);
int process_getgid(struct process_s *process, gid_t *gid);
int process_getegid(struct process_s *process, gid_t *egid);
int process_geteugid(struct process_s *process, uid_t *euid, gid_t *egid);
int process_getresgid(struct process_s *process, gid_t *rgid, gid_t *egid, gid_t *sgid);
uid_t process_userns_uid_of(struct process_s *process);

int procmgr_setuid(struct process_s *process, struct thread_s *thread, uid_t uid);
int procmgr_setreuid(struct process_s *process, struct thread_s *thread, uid_t ruid, uid_t euid);
int procmgr_setresuid(struct process_s *process, struct thread_s *thread, uid_t ruid, uid_t euid, uid_t suid);
/* get self xid, use nolock version; get non-self xid, use lock version */
int procmgr_getuid(struct thread_s *thread, uid_t *uid);
int procmgr_geteuid(struct thread_s *thread, uid_t *euid);
int procmgr_getresuid(struct thread_s *thread, uid_t *ruid, uid_t *euid, uid_t *suid);
int procmgr_getuid_nolock(struct thread_s *thread, uid_t *uid);
int procmgr_geteuid_nolock(struct thread_s *thread, uid_t *euid);
int procmgr_getresuid_nolock(struct thread_s *thread, uid_t *ruid, uid_t *euid, uid_t *suid);
int procmgr_setgid(struct process_s *process, struct thread_s *thread, gid_t gid);
int procmgr_setregid(struct process_s *process, struct thread_s *thread, gid_t rgid, gid_t egid);
int procmgr_setresgid(struct process_s *process, struct thread_s *thread, gid_t rgid, gid_t egid, gid_t sgid);
int procmgr_getgid(struct thread_s *thread, gid_t *gid);
int procmgr_getegid(struct thread_s *thread, gid_t *egid);
int procmgr_geteugid(struct thread_s *thread, uid_t *euid, gid_t *egid);
int procmgr_getresgid(struct thread_s *thread, gid_t *rgid, gid_t *egid, gid_t *sgid);
int procmgr_getgid_nolock(struct thread_s *thread, gid_t *gid);
int procmgr_getegid_nolock(struct thread_s *thread, gid_t *egid);
int procmgr_geteugid_nolock(struct thread_s *thread, uid_t *euid, gid_t *egid);
int procmgr_getresgid_nolock(struct thread_s *thread, gid_t *rgid, gid_t *egid, gid_t *sgid);
int procmgr_setgroups(struct process_s *process, struct thread_s *thread,
		      uint32_t data_size, const gid_t *group_list,
		      bool is_from_umem);
int procmgr_getgroups(struct thread_s *thread, int data_size, gid_t *group_list);
int procmgr_setfsuid(struct process_s *process, struct thread_s *thread, uid_t uid);
int procmgr_setfsgid(struct process_s *process, struct thread_s *thread, gid_t gid);
void update_process_cred(struct process_s *process, const struct credential *cred, uint64_t bmp_cred);

#define SEC_CONCAT(a, b) a##b
#define CRED_UPDATE_BITMAP(name, ...) \
  CRED_BITMAP(VA_ARGS_NUM(__VA_ARGS__), __VA_ARGS__, name)
#define CRED_BITMAP(n, ...) \
  SEC_CONCAT(CRED_BITMAP, n)(__VA_ARGS__)
#define CRED_BITMAP11(n, ...) CRED_BITMAP10(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP10(n, ...) CRED_BITMAP9(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP9(n, ...) CRED_BITMAP8(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP8(n, ...) CRED_BITMAP7(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP7(n, ...) CRED_BITMAP6(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP6(n, ...) CRED_BITMAP5(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP5(n, ...) CRED_BITMAP4(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP4(n, ...) CRED_BITMAP3(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP3(n, ...) CRED_BITMAP2(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP2(n, ...) CRED_BITMAP1(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP1(n, ...) CRED_BITMAP0(__VA_ARGS__) | (1ULL << n)
#define CRED_BITMAP0(...) __VA_ARGS__ = __VA_ARGS__

struct procmgr_cred_ops_s {
	int (*setuid)(struct process_s *process, struct thread_s *thread,
		      uid_t uid);
	int (*setreuid)(struct process_s *process, struct thread_s *thread,
			uid_t ruid, uid_t euid);
	int (*setresuid)(struct process_s *process, struct thread_s *thread,
			 uid_t ruid, uid_t euid, uid_t suid);
	int (*setgid)(struct process_s *process, struct thread_s *thread,
		      gid_t gid);
	int (*setregid)(struct process_s *process, struct thread_s *thread,
			gid_t rgid, gid_t egid);
	int (*setresgid)(struct process_s *process, struct thread_s *thread,
			 gid_t rgid, gid_t egid, gid_t sgid);
	int (*setgroups)(struct process_s *process, struct thread_s *thread,
			 uint32_t data_size, const gid_t *group_list,
			 bool is_from_umem);
	int (*setcaps_post)(struct thread_s *thread, struct proc_prctl_args args);
};

int procmgr_cred_ops_setuid(struct process_s *process, struct thread_s *thread, uid_t uid);
int procmgr_cred_ops_setreuid(struct process_s *process, struct thread_s *thread, uid_t ruid, uid_t euid);
int procmgr_cred_ops_setresuid(struct process_s *process, struct thread_s *thread, uid_t ruid, uid_t euid, uid_t suid);
int procmgr_cred_ops_setgroups(struct process_s *process, struct thread_s *thread,
			       uint32_t data_size, const gid_t *group_list);
int procmgr_cred_ops_setgid(struct process_s *process, struct thread_s *thread, uid_t gid);
int procmgr_cred_ops_setregid(struct process_s *process, struct thread_s *thread, uid_t rgid, uid_t egid);
int procmgr_cred_ops_setresgid(struct process_s *process, struct thread_s *thread, uid_t rgid, uid_t egid, uid_t sgid);
int procmgr_cred_ops_setcaps_post(struct thread_s *thread, struct proc_prctl_args args);
#ifdef CONFIG_POSIX_CAPS
void procmgr_getposixcaps(struct process_s *process, struct thread_s *thread, struct sec_cred_caps *caps);
#endif

/* until now, we do not have any available implementation of rcu or ebr, so we use mutex to protect
 * geting or puting cred refcnts */
extern struct raw_rwlock cred_lock;

static inline int copy_groups_from_security(struct sec_cred_dac *dac_cred, struct security_s *security)
{
	return sec_copy_groups(dac_cred, security->cred.group.size, security->cred.group.group_list);
}

static inline void copy_base_dac_from_security(struct sec_cred_dac *dac_cred, struct security_s *security)
{
	struct cred_dac *dac = &security->cred.dac;
	dac_cred->ruid = dac->ruid;
	dac_cred->euid = dac->euid;
	dac_cred->suid = dac->suid;
	dac_cred->rgid = dac->rgid;
	dac_cred->egid = dac->egid;
	dac_cred->sgid = dac->sgid;
	dac_cred->fsuid = dac->fsuid;
	dac_cred->fsgid = dac->fsgid;
}

static inline int copy_dac_from_security(struct sec_cred_dac *dac_cred, struct security_s *security)
{
#ifdef CONFIG_DAC
	copy_base_dac_from_security(dac_cred, security);
	return copy_groups_from_security(dac_cred, security);
#else
	return E_HM_OK;
#endif
}

static inline void copy_sel_from_security(struct sec_cred_seharmony *sel_cred, struct security_s *security)
{
#ifdef CONFIG_SEHARMONY
	struct cred_sel *sel = &security->cred.sel;
	sel_cred->osid = sel->osid;
	sel_cred->sid = sel->sid;
	sel_cred->exec_sid = sel->exec_sid;
	sel_cred->create_sid = sel->create_sid;
	sel_cred->keycreate_sid = sel->keycreate_sid;
	sel_cred->sockcreate_sid = sel->sockcreate_sid;
#endif
}

static inline void copy_mac_from_security(struct sec_cred_mac *mac_cred, struct security_s *security)
{
	mac_cred->sid = security->cred.mac.sid;
#ifdef CONFIG_INNER_MAC
	mac_cred->inner_sid = security->cred.mac.inner_sid;
#endif
}

static void copy_basic_from_process(struct sec_cred_basic *basic_cred, struct process_s *proc)
{
	uint32_t flags = 0;

	/* accelerate when using property for service */
	if (proc->security.props.system_service == 1U) {
		sec_cred_set_flag(flags, SEC_BASIC_SYS_SRVC);
	}
#ifdef CONFIG_SEC_DENY_LOG
	if (sec_cred_is_flag_set(proc->security.cred.log_enable,
				 SEC_BASIC_LOG_ENABLE)) {
		sec_cred_set_flag(flags, SEC_BASIC_LOG_ENABLE);
	}
	if (sec_cred_is_flag_set(proc->security.cred.log_enable,
				 SEC_BASIC_LOG_TRACE)) {
		sec_cred_set_flag(flags, SEC_BASIC_LOG_TRACE);
	}
#endif
	basic_cred->flags = flags;
	basic_cred->pid = process_vpid_of(proc);

#ifdef CONFIG_AUDIT
	basic_cred->ppid = process_ppid_in_pidns(proc, NULL);
#endif
}

#ifdef CONFIG_POSIX_CAPS
static inline void copy_caps_from_security(struct sec_cred_caps *caps, struct security_s *security)
{
	copy_cred_caps(caps, &(security->cred.posix_cap));
}
#endif

#ifdef CONFIG_KEYMGR
static inline void copy_keyring_from_security(struct sec_cred_keyring *keyring, struct security_s *security)
{
	keyring->proc_keyring_addr = security->cred.keyring.proc_keyring_addr;
	keyring->ses_keyring_addr = security->cred.keyring.ses_keyring_addr;
}
#endif

static inline void copy_prop_from_process(struct sec_cred_property *prop, struct process_s *proc)
{
	/* copy from struct sysmgr_property_props to struct property_props */
#define PROP_REGISTER_UNFIXED_LEN_TYPE(...)
#define PROP_REGISTER_FIXED_LEN_TYPE(name, ptype, dtype, unit, ...) (prop->props).name = (proc->security.props).name;
#include <libsec/property_kind_decl.h>
#undef PROP_REGISTER_FIXED_LEN_TYPE
#undef PROP_REGISTER_UNFIXED_LEN_TYPE
}

#ifdef CONFIG_AUDIT
static inline void copy_audit_from_process(struct sec_cred_audit *audit, struct process_s *proc)
{
	audit->session_id = proc->security.cred.audit.session_id;
	audit->login_uid = proc->security.cred.audit.login_uid;
	if (proc->security.cred.audit.cmdline_node != NULL) {
		audit->cmdline_idx = proc->security.cred.audit.cmdline_node->idx;
	}

	if (proc->security.cred.audit.tty_node != NULL) {
		audit->tty_idx = proc->security.cred.audit.tty_node->idx;
	}
}
#endif

static inline void copy_usr_ns_from_security(struct base_cred *cred, struct security_s *security)
{
#ifdef CONFIG_USER_NS
	struct usr_ns_cred *old_usrns_cred = cred->usr_ns_cred;
	sec_usr_ns_cred_get(security->cred.usr_ns.ns->usr_ns_cred);
	cred->usr_ns_idx = security->cred.usr_ns.ns->idx;
	cred->usr_ns_cred = security->cred.usr_ns.ns->usr_ns_cred;
	if (old_usrns_cred != NULL) {
		sec_usr_ns_cred_put(old_usrns_cred);
	}
#endif
}

static inline void delete_usr_ns(struct base_cred *cred)
{
#ifdef CONFIG_USER_NS
	struct usr_ns_cred *usrns_cred = cred->usr_ns_cred;
	if (usrns_cred != NULL) {
		cred->usr_ns_idx = ____COOBJ_INVALID_POS;
		cred->usr_ns_cred = NULL;
		sec_usr_ns_cred_put(usrns_cred);
	}
#endif
}

#ifdef CONFIG_ENCAPS
static inline int copy_encaps_list_from_security(struct base_cred *cred, struct security_s *security)
{
	int ret = E_HM_OK;
	raw_rwlock_wrlock(&cred->cred_encaps->rwlock);
	cred->cred_encaps->proc_type = security->cred.encaps.proc_type;
	if (cred->cred_encaps->version != security->cred.encaps.version) {
		cred->cred_encaps->version = security->cred.encaps.version;
		ret = sec_copy_encaps_list(cred->cred_encaps, security->cred.encaps.count, security->cred.encaps.encaps_list);
	}
	raw_rwlock_unlock(&cred->cred_encaps->rwlock);
	return ret;
}

static inline int set_encaps_for_kernel_proc(struct base_cred *cred, struct security_s *security)
{
	int ret = E_HM_OK;
	/* cred_encaps is not NULL that means it has been setted */
	if (cred->cred_encaps == NULL && cred->encaps_idx == 0) {
		cred->cred_encaps = sec_cred_encaps_alloc(&cred->encaps_idx);
		if (cred->cred_encaps != NULL) {
			cred->cred_encaps->proc_type = ENCAPS_KERNEL_PROC;
		} else {
			ret = E_HM_NOMEM;
		}
	}
	return ret;
}


static inline int set_encaps_common(struct base_cred *cred, struct security_s *security)
{
	/* dlopen come here with proc->encaps_lock locked,
	* execve & hap come here no need lock because they are one thread process now
	*/
	hm_pacga_cred_encaps_check(&security->cred.encaps);
	if (cred->cred_encaps == NULL && cred->encaps_idx == 0) {
		cred->cred_encaps = sec_cred_encaps_alloc(&cred->encaps_idx);
	}
	if (cred->cred_encaps != NULL) {
		return copy_encaps_list_from_security(cred, security);
	} else {
		return E_HM_NOMEM;
	}
}

static inline int copy_encaps_from_security(struct base_cred *cred, struct security_s *security)
{
	int ret = E_HM_OK;

	if (security->props.system_service == 1U) {
		ret = set_encaps_for_kernel_proc(cred, security);
	} else if (security->cred.encaps.count != 0) {
		ret = set_encaps_common(cred, security);
	}
	if (ret != E_HM_OK) {
		hm_warn("copy encaps from security failed, err=%d\n", ret);
	}
	return ret;
}
#endif

static inline int copy_base_cred_from_process(struct base_cred *cred, struct process_s *proc)
{
	int rc = copy_dac_from_security(&cred->cred_dac, &proc->security);
	copy_sel_from_security(&cred->cred_sel, &proc->security);

#ifdef CONFIG_POSIX_CAPS
	copy_caps_from_security(&cred->cred_caps, &proc->security);
#endif
#ifdef CONFIG_KEYMGR
	copy_keyring_from_security(&cred->cred_key, &proc->security);
#endif
#ifdef CONFIG_ENCAPS
	copy_encaps_from_security(cred, &proc->security);
#endif
	copy_usr_ns_from_security(cred, &proc->security);
	if (rc != E_HM_OK) {
		delete_usr_ns(cred);
	}
	hm_pacdb_bcred_mem(cred);
	return rc;
}

static inline void copy_shared_cred_from_process(struct shared_cred *sh_cred, struct process_s *proc)
{
	copy_mac_from_security(&sh_cred->cred_mac, &proc->security);
	DUMP_OF_SH_CRED(sh_cred) = proc->security.cred.dac.is_dumpable;
	UMASK_OF_SH_CRED(sh_cred) = proc->security.cred.dac.umask;
	copy_basic_from_process(&sh_cred->cred_basic, proc);
	copy_prop_from_process(&sh_cred->prop, proc);
#ifdef CONFIG_AUDIT
	copy_audit_from_process(&sh_cred->cred_audit, proc);
#endif
	hm_pacdb_shcred_mem(sh_cred);
}

static inline void copy_creds_from_security(struct shared_cred *sh_cred, struct base_cred *b_cred,
					   struct security_s *security)
{
	/* copy shared cred */
	copy_mac_from_security(&sh_cred->cred_mac, security);
	DUMP_OF_SH_CRED(sh_cred) = security->cred.dac.is_dumpable;
	UMASK_OF_SH_CRED(sh_cred) = security->cred.dac.umask;
	/* copy base cred */
	copy_base_dac_from_security(&b_cred->cred_dac, security);
	copy_sel_from_security(&b_cred->cred_sel, security);
#ifdef CONFIG_POSIX_CAPS
	copy_caps_from_security(&b_cred->cred_caps, security);
#endif
#ifdef CONFIG_ENCAPS
	copy_encaps_from_security(b_cred, security);
#endif
#ifdef CONFIG_KEYMGR
	copy_keyring_from_security(&b_cred->cred_key, security);
#endif
	hm_pacdb_bcred_mem(b_cred);
	hm_pacdb_shcred_mem(sh_cred);
}
#endif // PROCMGR_CRED_H
